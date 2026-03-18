#!/usr/bin/env python3
"""
View original assembly for a source file.
Usage: python view_asm.py <path> [function_name]
Example: python view_asm.py System/J3DSysFlag
Example: python view_asm.py System/J3DSysFlag TJ3DSysFlag::perform
"""

import sys
import re
from pathlib import Path

def main():
    if len(sys.argv) < 2:
        print("Usage: python view_asm.py <path> [function_search]")
        print("Example: python view_asm.py System/J3DSysFlag")
        print("Example: python view_asm.py System/J3DSysFlag TJ3DSysFlag::perform")
        sys.exit(1)

    file_path = sys.argv[1].replace("\\", "/")
    search = sys.argv[2] if len(sys.argv) > 2 else None

    # Find project root
    script_dir = Path(__file__).parent
    project_root = script_dir.parent.parent
    asm_path = project_root / "build" / "GMSJ01" / "asm" / f"{file_path}.s"

    if not asm_path.exists():
        print(f"Error: Assembly file not found: {asm_path}")
        print("Make sure the file path is correct and run 'ninja' first.")
        sys.exit(1)

    with open(asm_path, encoding="utf-8") as f:
        content = f.read()

    if search:
        # Find functions matching search
        lines = content.split("\n")
        in_function = False
        function_lines = []
        current_fn = ""

        for line in lines:
            # Check for function start
            fn_match = re.match(r"^\.fn (\S+),", line)
            if fn_match:
                current_fn = fn_match.group(1)
                if search.lower() in current_fn.lower():
                    in_function = True
                    function_lines.append(f"\n{'='*60}")
                    function_lines.append(f"# {current_fn}")
                    function_lines.append(f"{'='*60}")

            if in_function:
                function_lines.append(line)

            # Check for function end
            if line.startswith(".endfn") and in_function:
                in_function = False
                function_lines.append("")

        if function_lines:
            print("\n".join(function_lines))
        else:
            print(f"No functions found matching '{search}'")
            print("\nAvailable functions in this file:")
            for line in lines:
                fn_match = re.match(r"^\.fn (\S+),", line)
                if fn_match:
                    print(f"  {fn_match.group(1)}")
    else:
        # Print entire file
        print(content)

if __name__ == "__main__":
    main()
