#!/usr/bin/env python3
"""
Check match status for a specific source file.
Usage: python check_match.py <partial_path>
Example: python check_match.py System/J3DSysFlag
"""

import json
import sys
from pathlib import Path

def main():
    if len(sys.argv) < 2:
        print("Usage: python check_match.py <partial_path>")
        print("Example: python check_match.py System/J3DSysFlag")
        sys.exit(1)

    search = sys.argv[1].replace("\\", "/")

    # Find project root
    script_dir = Path(__file__).parent
    project_root = script_dir.parent.parent
    report_path = project_root / "build" / "GMSJ01" / "report.json"

    if not report_path.exists():
        print(f"Error: {report_path} not found. Run 'ninja' first.")
        sys.exit(1)

    with open(report_path) as f:
        data = json.load(f)

    found = False
    for unit in data.get("units", []):
        name = unit.get("name", "")
        if search.lower() in name.lower():
            found = True
            print(f"\n=== {name} ===")

            total_code = unit.get("total_code", 0)
            complete_code = unit.get("complete_code", 0)
            if total_code > 0:
                pct = complete_code / total_code * 100
                print(f"Overall: {complete_code}/{total_code} bytes ({pct:.1f}%)")

            functions = unit.get("functions", [])
            matching = 0
            print(f"\nFunctions ({len(functions)} total):")

            for fn in functions:
                fn_name = fn.get("name", "?")
                fuzzy = fn.get("fuzzy_match_percent", 0)
                size = fn.get("size", 0)

                if fuzzy == 100:
                    status = "MATCH"
                    matching += 1
                else:
                    status = f"{fuzzy:.1f}%"

                # Demangle name for readability
                short_name = fn_name
                if "__" in fn_name:
                    parts = fn_name.split("__")
                    short_name = parts[0]

                print(f"  {short_name}: {status} ({size} bytes)")

            print(f"\nSummary: {matching}/{len(functions)} functions at 100%")

    if not found:
        print(f"No units found matching '{search}'")
        print("\nAvailable units containing your search:")
        for unit in data.get("units", []):
            name = unit.get("name", "")
            if any(part.lower() in name.lower() for part in search.split("/")):
                print(f"  {name}")

if __name__ == "__main__":
    main()
