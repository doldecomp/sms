"""Inspect report.json structure to understand matching fields."""
import json

with open('build/GMSJ01/report.json', 'r') as f:
    report = json.load(f)

print("Top-level keys:", list(report.keys()))
print()

cats = report.get('categories', [])
print(f"Number of categories: {len(cats)}")
for cat in cats:
    print(f"  Category: id={cat.get('id')}, name={cat.get('name')}, units={len(cat.get('units', []))}")

# Find game category
game_cat = None
for cat in cats:
    if cat.get('id') == 'game':
        game_cat = cat
        break

if not game_cat:
    # Try other approaches
    print("\nNo 'game' category. Checking all category ids:")
    for cat in cats:
        print(f"  '{cat.get('id')}'")
    exit()

units = game_cat.get('units', [])
print(f"\nGame category has {len(units)} units")

# Show a few example units with functions
shown = 0
for unit in units:
    functions = unit.get('functions', [])
    measures = unit.get('measures', {})
    if len(functions) > 0 and measures.get('total_code', 0) > 0:
        print(f"\n--- Unit: {unit.get('name')} ---")
        print(f"  Unit keys: {list(unit.keys())}")
        print(f"  Measures: {measures}")
        print(f"  Functions count: {len(functions)}")
        if functions:
            f0 = functions[0]
            print(f"  First function keys: {list(f0.keys())}")
            print(f"  First function: {json.dumps(f0, indent=2)[:500]}")
        shown += 1
        if shown >= 3:
            break

# Also show units that have some non-100% match
print("\n\n=== Units with partial matches ===")
shown2 = 0
for unit in units:
    measures = unit.get('measures', {})
    mcp = measures.get('matched_code_percent', 0)
    tc = measures.get('total_code', 0)
    if tc > 0 and mcp > 0 and mcp < 100:
        functions = unit.get('functions', [])
        print(f"\nUnit: {unit.get('name')}")
        print(f"  Measures: matched_code_percent={mcp}, total_code={tc}, matched_code={measures.get('matched_code', 0)}")
        print(f"  Functions: {len(functions)}")
        for func in functions[:3]:
            fm = func.get('measures', {})
            print(f"    Func: {func.get('name')}, measures={fm}, fuzzy={func.get('fuzzy_match_percent', 'N/A')}")
        shown2 += 1
        if shown2 >= 5:
            break
