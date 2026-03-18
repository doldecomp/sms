#!/usr/bin/env python3
"""
Compare assembly between original and compiled object files.
Usage: python compare_asm.py <path> [function_name]
Example: python compare_asm.py System/J3DSysFlag perform__11TJ3DSysFlagFUlPQ26JDrama9TGraphics
"""

import subprocess
import sys
from pathlib import Path

def disassemble(objdump, obj_path, function=None):
    """Disassemble an object file, optionally filtering to a specific function."""
    cmd = [str(objdump), "-d", str(obj_path)]
    result = subprocess.run(cmd, capture_output=True, text=True)

    if result.returncode != 0:
        return f"Error: {result.stderr}"

    output = result.stdout

    if function:
        # Extract just the specified function
        lines = output.split("\n")
        in_function = False
        func_lines = []

        for line in lines:
            if f"<{function}>:" in line:
                in_function = True
            elif in_function:
                if line.strip() == "" or (line[0] != " " and ":" in line and "<" in line):
                    break
                func_lines.append(line)

        if func_lines:
            return "\n".join(func_lines)
        else:
            return f"Function '{function}' not found"

    return output

def main():
    if len(sys.argv) < 2:
        print("Usage: python compare_asm.py <path> [function_name]")
        print("Example: python compare_asm.py System/J3DSysFlag")
        print("Example: python compare_asm.py System/J3DSysFlag perform__11TJ3DSysFlagFUlPQ26JDrama9TGraphics")
        sys.exit(1)

    file_path = sys.argv[1].replace("\\", "/")
    function = sys.argv[2] if len(sys.argv) > 2 else None

    # Find project root
    script_dir = Path(__file__).parent
    project_root = script_dir.parent.parent

    objdump = project_root / "build" / "binutils" / "powerpc-eabi-objdump.exe"
    if not objdump.exists():
        print(f"Error: objdump not found at {objdump}")
        print("Run 'ninja' first to download tools.")
        sys.exit(1)

    # Find object files
    orig_obj = project_root / "build" / "GMSJ01" / "obj" / f"{file_path}.o"
    comp_obj = project_root / "build" / "GMSJ01" / "src" / f"{file_path}.o"

    if not orig_obj.exists():
        print(f"Error: Original object not found: {orig_obj}")
        sys.exit(1)

    if not comp_obj.exists():
        print(f"Error: Compiled object not found: {comp_obj}")
        print("Make sure the source file exists and run 'ninja'.")
        sys.exit(1)

    # Disassemble both
    print("=" * 60)
    print("ORIGINAL")
    print("=" * 60)
    orig_asm = disassemble(objdump, orig_obj, function)
    print(orig_asm)

    print("\n" + "=" * 60)
    print("COMPILED")
    print("=" * 60)
    comp_asm = disassemble(objdump, comp_obj, function)
    print(comp_asm)

    # Quick diff summary
    if function:
        orig_lines = [l.strip() for l in orig_asm.split("\n") if l.strip()]
        comp_lines = [l.strip() for l in comp_asm.split("\n") if l.strip()]

        print("\n" + "=" * 60)
        print("SUMMARY")
        print("=" * 60)
        print(f"Original: {len(orig_lines)} instructions")
        print(f"Compiled: {len(comp_lines)} instructions")

        if len(orig_lines) == len(comp_lines):
            matches = sum(1 for o, c in zip(orig_lines, comp_lines)
                         if o.split("\t")[-1] == c.split("\t")[-1])
            print(f"Matching instructions: {matches}/{len(orig_lines)}")

if __name__ == "__main__":
    main()
