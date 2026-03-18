#!/usr/bin/env python3
"""
Find non-matching files sorted by number of functions (easier targets first).
Usage: python find_easy_targets.py [--category game|jsystem|sdk] [--max-functions N]
"""

import json
import sys
import argparse
from pathlib import Path

def main():
    parser = argparse.ArgumentParser(description="Find easy decompilation targets")
    parser.add_argument("--category", choices=["game", "jsystem", "sdk"],
                       help="Filter by category")
    parser.add_argument("--max-functions", type=int, default=20,
                       help="Maximum number of functions (default: 20)")
    parser.add_argument("--min-match", type=float, default=0,
                       help="Minimum match percentage to show")
    parser.add_argument("--show-functions", action="store_true",
                       help="Show individual function details")
    args = parser.parse_args()

    # Find project root
    script_dir = Path(__file__).parent
    project_root = script_dir.parent.parent
    report_path = project_root / "build" / "GMSJ01" / "report.json"

    if not report_path.exists():
        print(f"Error: {report_path} not found. Run 'ninja' first.")
        sys.exit(1)

    with open(report_path) as f:
        data = json.load(f)

    # Collect non-matching units
    targets = []
    for unit in data.get("units", []):
        name = unit.get("name", "")
        functions = unit.get("functions", [])
        total_code = unit.get("total_code", 0)
        complete_code = unit.get("complete_code", 0)

        # Skip fully matching
        if complete_code == total_code and total_code > 0:
            continue

        # Calculate match percentage
        match_pct = (complete_code / total_code * 100) if total_code > 0 else 0

        # Filter by minimum match
        if match_pct < args.min_match:
            continue

        # Determine category
        category = "game"
        if "JSystem" in name or "jsystem" in name.lower():
            category = "jsystem"
        elif "dolphin" in name.lower() or "sdk" in name.lower() or "PowerPC" in name or "TRK" in name or "MSL" in name:
            category = "sdk"

        # Filter by category
        if args.category and category != args.category:
            continue

        # Filter by function count
        num_functions = len(functions)
        if num_functions > args.max_functions:
            continue

        # Count matching functions
        matching_fns = sum(1 for f in functions if f.get("fuzzy_match_percent", 0) == 100)

        targets.append({
            "name": name,
            "category": category,
            "num_functions": num_functions,
            "matching_functions": matching_fns,
            "total_bytes": total_code,
            "match_pct": match_pct,
            "functions": functions
        })

    # Sort by number of functions (ascending), then by match percentage (descending)
    targets.sort(key=lambda x: (x["num_functions"], -x["match_pct"]))

    # Display results
    print(f"Found {len(targets)} potential targets (max {args.max_functions} functions)")
    print()

    for t in targets[:50]:  # Show top 50
        name = t["name"]
        num_fns = t["num_functions"]
        matching = t["matching_functions"]
        pct = t["match_pct"]
        category = t["category"]

        status = f"{matching}/{num_fns} fns match"
        print(f"[{category:7}] {name}")
        print(f"          {status}, {pct:.1f}% bytes, {t['total_bytes']} total bytes")

        if args.show_functions:
            for fn in t["functions"]:
                fn_name = fn.get("name", "?")
                fn_pct = fn.get("fuzzy_match_percent", 0)
                fn_size = fn.get("size", 0)
                fn_status = "MATCH" if fn_pct == 100 else f"{fn_pct:.1f}%"
                print(f"            - {fn_name[:50]}: {fn_status} ({fn_size}b)")

        print()

if __name__ == "__main__":
    main()
