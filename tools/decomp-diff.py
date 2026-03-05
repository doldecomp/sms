#!/usr/bin/env python3

"""
Wrapper around objdiff-cli for agent-friendly diff output.

Two modes:
  Overview (default): List symbols in a unit with match status
  Diff (-d):          Show side-by-side instruction diff for a function

Usage:
  python scripts/decomp-diff.py -u main/MetroidPrime/CIOWinManager
  python scripts/decomp-diff.py -u main/MetroidPrime/CIOWinManager -s nonmatching
  python scripts/decomp-diff.py -u main/MetroidPrime/CIOWinManager -d DistributeOneMessage
"""

import argparse
import json
import os
import subprocess
import sys
from typing import Any, Dict, List, Optional, Tuple

script_dir = os.path.dirname(os.path.realpath(__file__))
root_dir = os.path.abspath(os.path.join(script_dir, ".."))

OBJDIFF_CLI = os.environ.get("OBJDIFF_CLI", os.path.join(root_dir, "build", "tools", "objdiff-cli"))


def run_objdiff(unit: str) -> Dict[str, Any]:
    """Run objdiff-cli diff and return parsed JSON."""
    result = subprocess.run(
        [OBJDIFF_CLI, "diff", "-c", "functionRelocDiffs=data_value", "-u", unit, "-o", "-", "--format", "json"],
        capture_output=True,
        cwd=root_dir,
    )
    if result.returncode != 0:
        print(f"objdiff-cli error: {result.stderr.decode()}", file=sys.stderr)
        sys.exit(1)
    return json.loads(result.stdout)


def classify_symbol(sym: Dict[str, Any]) -> str:
    """Classify a symbol as 'function', 'object', or 'section'."""
    kind = sym.get("kind", "")
    if kind == "SYMBOL_FUNCTION":
        return "function"
    if kind == "SYMBOL_OBJECT":
        return "object"
    if kind == "SYMBOL_SECTION":
        return "section"
    # Fallback for external/relocation-only symbols (empty kind)
    if "instructions" in sym:
        return "function"
    if "data_diff" in sym:
        return "object"
    return "unknown"


def symbol_section(sym: Dict[str, Any], sections: List[Dict[str, Any]]) -> str:
    """Determine which section a symbol belongs to."""
    # For named section data symbols like [.rodata-0]
    name = sym.get("name", "")
    if name.startswith("[."):
        return name[1:].split("-")[0].rstrip("]")
    # Use content type as best indicator
    if classify_symbol(sym) == "function":
        return ".text"
    # Check sections for data
    for sec in sections:
        kind = sec.get("kind", "")
        if kind in ("SECTION_DATA", "SECTION_BSS"):
            return sec["name"]
    return ".data"


def fuzzy_match(pattern: str, name: str) -> bool:
    """Case-insensitive substring match."""
    return pattern.lower() in name.lower()


def build_overview(data: Dict[str, Any], args) -> None:
    """Print overview of all symbols in a unit."""
    left_syms = data.get("left", {}).get("symbols", [])
    right_syms = data.get("right", {}).get("symbols", [])
    left_sections = data.get("left", {}).get("sections", [])
    right_sections = data.get("right", {}).get("sections", [])

    rows = []

    # Process left (original/target) symbols
    for i, sym in enumerate(left_syms):
        sym_type = classify_symbol(sym)
        # Skip section symbols and external references
        if sym_type in ("section", "unknown"):
            continue
        # Skip symbols without size
        size = int(sym.get("size", "0"))
        if size == 0:
            continue

        name = sym.get("demangled_name", sym.get("name", "?"))
        section = symbol_section(sym, left_sections)
        ts = sym.get("target_symbol")
        mp = sym.get("match_percent")

        if ts is None:
            status = "missing"
            match_str = "-"
        elif mp is not None and mp >= 100.0:
            status = "match"
            match_str = f"{mp:.1f}%"
        elif mp is not None:
            status = "nonmatching"
            match_str = f"{mp:.1f}%"
        else:
            status = "missing"
            match_str = "-"

        rows.append((status, match_str, size, section, sym_type, name, "left"))

    # Process right (decomp/base) symbols that aren't targeted (extra)
    for i, sym in enumerate(right_syms):
        if sym.get("target_symbol") is not None:
            continue  # Already covered via left side
        sym_type = classify_symbol(sym)
        if sym_type in ("section", "unknown"):
            continue
        size = int(sym.get("size", "0"))
        if size == 0:
            continue
        name = sym.get("demangled_name", sym.get("name", "?"))
        section = symbol_section(sym, right_sections)
        rows.append(("extra", "-", size, section, sym_type, name, "right"))

    # Apply filters
    if args.type:
        types = set(t.strip() for t in args.type.split(","))
        rows = [r for r in rows if r[4] in types]

    if args.status:
        statuses = set(s.strip() for s in args.status.split(","))
        rows = [r for r in rows if r[0] in statuses]

    if args.section:
        rows = [r for r in rows if r[3] == args.section]

    if args.search:
        rows = [r for r in rows if fuzzy_match(args.search, r[5])]

    if not rows:
        print("No symbols match the given filters.")
        return

    # Print header
    print(f"{'STATUS':<10} {'MATCH':>7}  {'SIZE':>6}  {'SECTION':<10} {'NAME'}")
    print("-" * 80)
    for status, match_str, size, section, sym_type, name, side in rows:
        print(f"{status:<10} {match_str:>7}  {size:>5}B  {section:<10} {name}")


def render_instruction(
    inst_entry: Dict[str, Any],
    all_syms: List[Dict[str, Any]],
    is_diff: bool = False,
) -> str:
    """Build instruction text from parts, wrapping diffing args in {}."""
    inst = inst_entry.get("instruction", {})
    parts = inst.get("parts", [])
    arg_diffs = inst_entry.get("arg_diff", [])

    text_parts = []
    arg_idx = 0  # Index into arg_diff array

    for part in parts:
        if "opcode" in part:
            text_parts.append(part["opcode"]["mnemonic"])
            text_parts.append(" ")
        elif "arg" in part:
            arg = part["arg"]
            # Extract the value
            if "opaque" in arg:
                val = str(arg["opaque"])
            elif "signed" in arg:
                val = str(arg["signed"])
                try:
                    n = int(val)
                    if n < 0:
                        val = f"-0x{-n:x}"
                    elif n > 9:
                        val = f"0x{n:x}"
                except ValueError:
                    pass
            elif "unsigned" in arg:
                val = str(arg["unsigned"])
                try:
                    n = int(val)
                    if n > 9:
                        val = f"0x{n:x}"
                except ValueError:
                    pass
            elif "branch_dest" in arg:
                val = f"0x{int(arg['branch_dest']):x}"
            elif "reloc" in arg:
                # Resolve relocation target from instruction.relocation
                reloc_info = inst.get("relocation", {})
                ts = reloc_info.get("target_symbol")
                if ts is not None and ts < len(all_syms):
                    target_sym = all_syms[ts]
                    val = target_sym.get("demangled_name", target_sym.get("name", "?"))
                else:
                    # Fallback: extract from formatted text
                    formatted = inst.get("formatted", "")
                    val = formatted.split()[-1] if formatted else "?"
            else:
                val = str(arg)

            # Check if this arg has a diff
            has_diff = False
            if is_diff and arg_idx < len(arg_diffs):
                if arg_diffs[arg_idx].get("diff_index") is not None:
                    has_diff = True
            arg_idx += 1

            if has_diff:
                text_parts.append("{" + val + "}")
            else:
                text_parts.append(val)
        elif "separator" in part:
            text_parts.append(", ")
        elif "basic" in part:
            text_parts.append(part["basic"])

    return "".join(text_parts).rstrip()


def build_diff(data: Dict[str, Any], symbol_name: str, args) -> None:
    """Print side-by-side instruction diff for a specific function."""
    left_syms = data.get("left", {}).get("symbols", [])
    right_syms = data.get("right", {}).get("symbols", [])

    # Find the symbol by fuzzy matching on either side, then resolve
    # the pair via target_symbol (direct index into other side's array).
    left_sym = None
    right_sym = None

    for sym in left_syms:
        name = sym.get("demangled_name", sym.get("name", ""))
        mangled = sym.get("name", "")
        if fuzzy_match(symbol_name, name) or fuzzy_match(symbol_name, mangled):
            left_sym = sym
            ts = sym.get("target_symbol")
            if ts is not None and ts < len(right_syms):
                right_sym = right_syms[ts]
            break

    # If not found in left, try right
    if left_sym is None:
        for sym in right_syms:
            name = sym.get("demangled_name", sym.get("name", ""))
            mangled = sym.get("name", "")
            if fuzzy_match(symbol_name, name) or fuzzy_match(symbol_name, mangled):
                right_sym = sym
                ts = sym.get("target_symbol")
                if ts is not None and ts < len(left_syms):
                    left_sym = left_syms[ts]
                break

    if left_sym is None and right_sym is None:
        print(f"Symbol not found: {symbol_name}", file=sys.stderr)
        sys.exit(1)

    # Header
    display_name = (left_sym or right_sym).get(
        "demangled_name", (left_sym or right_sym).get("name", "?")
    )
    mp = (left_sym or right_sym).get("match_percent")
    size = int((left_sym or right_sym).get("size", "0"))

    left_insts = (left_sym or {}).get("instructions", [])
    right_insts = (right_sym or {}).get("instructions", [])
    n_insts = max(len(left_insts), len(right_insts))

    mp_str = f"{mp:.1f}%" if mp is not None else "N/A"
    print(f"{display_name}: {mp_str} match ({size}B, {n_insts} instructions)")
    print()

    if n_insts == 0:
        print("(no instructions to diff)")
        return

    # Parse range filter
    range_start = 0
    range_end = float("inf")
    if args.range:
        parts = args.range.split("-")
        range_start = int(parts[0], 16)
        if len(parts) > 1 and parts[1]:
            range_end = int(parts[1], 16)

    context = args.context
    no_collapse = args.no_collapse

    # Build rows
    rows = []
    for i in range(n_insts):
        li = left_insts[i] if i < len(left_insts) else {}
        ri = right_insts[i] if i < len(right_insts) else {}

        l_inst = li.get("instruction", {})
        r_inst = ri.get("instruction", {})

        l_addr = l_inst.get("address", "")
        r_addr = r_inst.get("address", "")

        # Use the first available address for offset display
        addr_str = ""
        if l_addr:
            addr_str = f"{int(l_addr):x}"
        elif r_addr:
            addr_str = f"{int(r_addr):x}"

        l_text = render_instruction(li, left_syms, is_diff=True) if li else ""
        r_text = render_instruction(ri, right_syms, is_diff=True) if ri else ""

        l_kind = li.get("diff_kind", "")
        r_kind = ri.get("diff_kind", "")

        # Determine diff marker
        kind = l_kind or r_kind
        if kind == "":
            marker = " "
        elif kind in ("DIFF_ARG_MISMATCH", "DIFF_OP_MISMATCH"):
            marker = "~"
        elif kind == "DIFF_REPLACE":
            marker = "|"
        elif kind == "DIFF_INSERT":
            marker = ">"
        elif kind == "DIFF_DELETE":
            marker = "<"
        else:
            marker = "?"

        is_match = (marker == " ")
        offset_int = int(l_addr) if l_addr else (int(r_addr) if r_addr else 0)
        rows.append((addr_str, marker, l_text, r_text, is_match, offset_int))

    # Apply range filter
    if args.range:
        rows = [r for r in rows if range_start <= r[5] <= range_end]

    if not rows:
        print("No instructions in the specified range.")
        return

    # Determine column widths
    max_left = max((len(r[2]) for r in rows), default=20)
    max_left = max(max_left, 4)  # minimum width
    max_left = min(max_left, 50)  # cap width

    # Print with collapsing
    print(f" {'OFFSET':>6} | {'LEFT':<{max_left}} | RIGHT")
    print("-" * (10 + max_left + 30))

    if no_collapse:
        for addr_str, marker, l_text, r_text, is_match, _ in rows:
            print(f"{marker}{addr_str:>6} | {l_text:<{max_left}} | {r_text}")
        return

    # Collapse matching runs
    i = 0
    while i < len(rows):
        addr_str, marker, l_text, r_text, is_match, _ = rows[i]

        if not is_match:
            print(f"{marker}{addr_str:>6} | {l_text:<{max_left}} | {r_text}")
            i += 1
            continue

        # Count consecutive matches
        run_start = i
        while i < len(rows) and rows[i][4]:
            i += 1
        run_len = i - run_start

        if run_len <= context * 2 + 1:
            # Short run: print all
            for j in range(run_start, run_start + run_len):
                a, m, lt, rt, _, _ = rows[j]
                print(f"{m}{a:>6} | {lt:<{max_left}} | {rt}")
        else:
            # Print leading context
            for j in range(run_start, run_start + context):
                a, m, lt, rt, _, _ = rows[j]
                print(f"{m}{a:>6} | {lt:<{max_left}} | {rt}")
            # Collapse middle
            collapsed = run_len - context * 2
            print(f"       ... {collapsed} matching instructions ...")
            # Print trailing context
            for j in range(run_start + run_len - context, run_start + run_len):
                a, m, lt, rt, _, _ = rows[j]
                print(f"{m}{a:>6} | {lt:<{max_left}} | {rt}")


def main():
    parser = argparse.ArgumentParser(
        description="Agent-friendly objdiff wrapper for decomp projects"
    )
    parser.add_argument(
        "-u", "--unit", required=True, help="Unit name (e.g. main/MetroidPrime/CEntity)"
    )
    parser.add_argument(
        "-d", "--diff", metavar="SYMBOL", help="Show diff for a specific symbol"
    )

    # Overview filters
    parser.add_argument(
        "-t",
        "--type",
        help="Filter by type: function, object (comma-separated)",
    )
    parser.add_argument(
        "-s",
        "--status",
        help="Filter by status: missing, matching, nonmatching, extra (comma-separated)",
    )
    parser.add_argument("--section", help="Filter by section name (e.g. .text)")
    parser.add_argument(
        "--search", help="Fuzzy search on demangled symbol name"
    )

    # Diff options
    parser.add_argument(
        "-C",
        "--context",
        type=int,
        default=3,
        help="Context lines around mismatches (default: 3)",
    )
    parser.add_argument(
        "--range", help="Only show instruction offset range (hex, e.g. 100-200)"
    )
    parser.add_argument(
        "--no-collapse",
        action="store_true",
        help="Don't collapse matching instruction runs",
    )

    args = parser.parse_args()

    data = run_objdiff(args.unit)

    if args.diff:
        build_diff(data, args.diff, args)
    else:
        build_overview(data, args)


if __name__ == "__main__":
    main()
