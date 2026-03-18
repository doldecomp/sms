"""Find game category units with 1-5 unmatched functions, sorted by easiest targets."""
import json
import sys

with open('build/GMSJ01/report.json', 'r') as f:
    report = json.load(f)

units = report.get('units', [])

# Collect candidates from game category
candidates = []
for unit in units:
    # Check if this is a game unit
    cats = unit.get('metadata', {}).get('progress_categories', [])
    if 'game' not in cats:
        continue

    name = unit.get('name', '')
    measures = unit.get('measures', {})
    functions = unit.get('functions', [])

    total_code = int(measures.get('total_code', '0') or '0')
    matched_code = int(measures.get('matched_code', '0') or '0')
    matched_code_pct = float(measures.get('matched_code_percent', 0) or 0)

    total_funcs = len(functions)
    if total_funcs == 0:
        continue

    # Count matched vs unmatched functions
    matched_funcs = 0
    unmatched_funcs = 0
    unmatched_details = []
    for func in functions:
        fuzz = float(func.get('fuzzy_match_percent', 0) or 0)
        func_size = int(func.get('size', '0') or '0')
        # A function is matched if fuzzy_match_percent == 100
        if fuzz == 100.0:
            matched_funcs += 1
        else:
            unmatched_funcs += 1
            demangled = func.get('metadata', {}).get('demangled_name', '')
            unmatched_details.append({
                'name': func.get('name', '?'),
                'demangled': demangled,
                'size': func_size,
                'fuzzy_pct': fuzz,
            })

    # Filter: has 1-5 unmatched functions, total code > 0
    if 1 <= unmatched_funcs <= 5 and total_code > 0:
        unmatched_code = total_code - matched_code
        candidates.append({
            'name': name,
            'total_funcs': total_funcs,
            'matched_funcs': matched_funcs,
            'unmatched_funcs': unmatched_funcs,
            'total_code': total_code,
            'matched_code': matched_code,
            'unmatched_code': unmatched_code,
            'matched_pct': matched_code_pct,
            'unmatched_details': unmatched_details,
        })

# Sort by: unmatched code size (smallest first), then fewest unmatched functions
candidates.sort(key=lambda x: (x['unmatched_code'], x['unmatched_funcs']))

# Print top 20
print(f'Found {len(candidates)} game units with 1-5 unmatched functions\n')
print(f'{"#":<3} {"Unit Name":<55} {"Funcs":>5} {"Match":>5} {"Unm":>4} {"TotalCode":>9} {"UnmCode":>8} {"MatchPct":>8}')
print('-' * 100)
for i, c in enumerate(candidates[:20]):
    print(f'{i+1:<3} {c["name"]:<55} {c["total_funcs"]:>5} {c["matched_funcs"]:>5} {c["unmatched_funcs"]:>4} {c["total_code"]:>9} {c["unmatched_code"]:>8} {c["matched_pct"]:>7.1f}%')
    for ud in c['unmatched_details']:
        dn = ud['demangled'] if ud['demangled'] else ud['name']
        print(f'      -> {dn} ({ud["size"]} bytes, fuzzy={ud["fuzzy_pct"]:.1f}%)')
    print()
