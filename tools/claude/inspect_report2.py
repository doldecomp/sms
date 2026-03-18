"""Inspect report.json structure - top-level units."""
import json

with open('build/GMSJ01/report.json', 'r') as f:
    report = json.load(f)

print("Top-level keys:", list(report.keys()))

units = report.get('units', [])
print(f"Top-level units count: {len(units)}")

# Check category structure more carefully
cats = report.get('categories', [])
for cat in cats:
    print(f"\nCategory '{cat.get('id')}' keys: {list(cat.keys())}")

# Show first few units
for i, unit in enumerate(units[:3]):
    print(f"\n--- Unit {i} ---")
    print(f"  Keys: {list(unit.keys())}")
    # Don't print functions, too long
    unit_copy = {k: v for k, v in unit.items() if k != 'functions' and k != 'sections'}
    print(f"  Data: {json.dumps(unit_copy, indent=2)[:600]}")
    funcs = unit.get('functions', [])
    print(f"  Functions: {len(funcs)}")
    if funcs:
        f0 = {k: v for k, v in funcs[0].items()}
        print(f"  First func: {json.dumps(f0, indent=2)[:400]}")

# Find game units
print("\n\n=== Finding game units ===")
game_units = []
for unit in units:
    # Check if unit has category field
    cat = unit.get('category', unit.get('metadata', {}).get('category', ''))
    if not cat:
        # Maybe categories are mapped by some other mechanism
        name = unit.get('name', '')
        # Just count by prefix patterns
    game_units.append(unit)

# Show all unique category values
cat_values = set()
for unit in units:
    for key in unit.keys():
        if 'cat' in key.lower():
            cat_values.add(f"{key}={unit[key]}")
    # also check metadata
    if 'metadata' in unit:
        for key in unit['metadata'].keys():
            if 'cat' in key.lower():
                cat_values.add(f"metadata.{key}={unit['metadata'][key]}")

print(f"Category-related fields found: {cat_values}")

# Check how categories reference units
for cat in cats:
    print(f"\nCategory '{cat.get('id')}': {json.dumps(cat, indent=2)[:300]}")
