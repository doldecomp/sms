#!/usr/bin/env python3
"""
Get symbols for a specific source file from symbols.txt.
Usage: python get_symbols.py <search_term>
Example: python get_symbols.py J3DSysFlag
Example: python get_symbols.py TButterfly
"""

import sys
import re
from pathlib import Path

def demangle_simple(name):
    """Simple demangling for common patterns."""
    # Constructor
    if name.startswith("__ct__"):
        class_name = name[6:].split("F")[0]
        return f"{class_name}::{class_name.split('Q')[-1].lstrip('0123456789')}()"

    # Destructor
    if name.startswith("__dt__"):
        class_name = name[6:].split("F")[0]
        return f"{class_name}::~{class_name.split('Q')[-1].lstrip('0123456789')}()"

    # Virtual table
    if name.startswith("__vt__"):
        class_name = name[6:]
        return f"{class_name}::__vtable"

    # Regular function
    if "__" in name:
        parts = name.split("__")
        fn_name = parts[0]
        if len(parts) > 1:
            class_info = parts[1]
            # Extract class name (before F which starts parameters)
            class_match = re.match(r"(\d*)([A-Za-z_]\w*)", class_info)
            if class_match:
                class_name = class_match.group(2)
                return f"{class_name}::{fn_name}()"

    return name

def main():
    if len(sys.argv) < 2:
        print("Usage: python get_symbols.py <search_term>")
        print("Example: python get_symbols.py J3DSysFlag")
        print("Example: python get_symbols.py TButterfly")
        sys.exit(1)

    search = sys.argv[1]

    # Find project root
    script_dir = Path(__file__).parent
    project_root = script_dir.parent.parent
    symbols_path = project_root / "config" / "GMSJ01" / "symbols.txt"

    if not symbols_path.exists():
        print(f"Error: {symbols_path} not found")
        sys.exit(1)

    # Read and filter symbols
    functions = []
    data_symbols = []
    other = []

    with open(symbols_path, encoding="utf-8") as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith("#"):
                continue

            if search.lower() in line.lower():
                # Parse symbol line
                # Format: name = section:address; // type:X size:Y scope:Z
                try:
                    name_part = line.split("=")[0].strip()
                    rest = line.split("//")[1].strip() if "//" in line else ""

                    # Extract metadata
                    sym_type = "unknown"
                    size = 0
                    scope = "global"

                    for part in rest.split():
                        if part.startswith("type:"):
                            sym_type = part.split(":")[1]
                        elif part.startswith("size:"):
                            size_str = part.split(":")[1]
                            size = int(size_str, 16) if size_str.startswith("0x") else int(size_str)
                        elif part.startswith("scope:"):
                            scope = part.split(":")[1]

                    entry = {
                        "name": name_part,
                        "demangled": demangle_simple(name_part),
                        "type": sym_type,
                        "size": size,
                        "scope": scope,
                        "line": line
                    }

                    if sym_type == "function":
                        functions.append(entry)
                    elif sym_type == "object":
                        data_symbols.append(entry)
                    else:
                        other.append(entry)

                except Exception:
                    other.append({"line": line})

    # Display results
    if functions:
        print(f"=== Functions ({len(functions)}) ===")
        # Sort by size descending
        functions.sort(key=lambda x: x["size"], reverse=True)
        for f in functions:
            scope_marker = "[weak]" if f["scope"] == "weak" else ""
            print(f"  {f['demangled']}")
            print(f"    Size: 0x{f['size']:X} ({f['size']} bytes) {scope_marker}")
            print(f"    Symbol: {f['name']}")
            print()

    if data_symbols:
        print(f"=== Data ({len(data_symbols)}) ===")
        for d in data_symbols:
            print(f"  {d['name']}: {d['size']} bytes")

    if other:
        print(f"=== Other ({len(other)}) ===")
        for o in other:
            if "line" in o:
                print(f"  {o.get('line', '')[:80]}")

    if not functions and not data_symbols and not other:
        print(f"No symbols found matching '{search}'")

    # Summary
    total_code = sum(f["size"] for f in functions)
    print(f"\nTotal: {len(functions)} functions, {total_code} bytes of code")

if __name__ == "__main__":
    main()
