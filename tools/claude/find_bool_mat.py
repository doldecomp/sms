import json
import subprocess
import re
from pathlib import Path

# Parse report.json
report_path = Path(r'C:\users\ryana\documents\sms\build\GMSJ01\report.json')
with open(report_path) as f:
    data = json.load(f)

# Find non-matching functions in src/ with 80-99% match
candidates = []
for unit in data.get('units', []):
    metadata = unit.get('metadata', {})
    source_path = metadata.get('source_path', '')

    # Only look at game code (src/), not SDK
    if not source_path.startswith('src/'):
        continue

    # Skip if it's SDK code based on progress categories
    categories = metadata.get('progress_categories', [])
    if 'sdk' in categories:
        continue

    for func in unit.get('functions', []):
        match_pct = func.get('fuzzy_match_percent', 0)
        if 80 <= match_pct < 100:
            candidates.append({
                'unit': unit.get('name', ''),
                'source_path': source_path,
                'function': func['name'],
                'match_pct': match_pct
            })

# Sort by match percentage (highest first)
candidates.sort(key=lambda x: -x['match_pct'])

print(f"Found {len(candidates)} candidates between 80-99% match")
print("\nTop 30 candidates:")
for i, c in enumerate(candidates[:30], 1):
    print(f"{i}. {c['match_pct']:.1f}% {c['source_path']} :: {c['function']}")

# Now check for boolean materialization pattern in top candidates
objdump = Path(r'C:\users\ryana\documents\sms\build\binutils\powerpc-eabi-objdump.exe')
bool_mat_pattern = b'\x54\x00\x06\x3f'  # clrlwi. r0, r0, 24

print("\n\nChecking for boolean materialization pattern...")
print("=" * 80)

results = []
for c in candidates[:50]:  # Check top 50
    # Convert src/path.cpp to path.o
    source_path = c['source_path']
    unit_path = source_path.replace('src/', '').replace('.cpp', '').replace('.c', '')
    orig_obj = Path(f"C:/users/ryana/documents/sms/build/GMSJ01/obj/{unit_path}.o")
    comp_obj = Path(f"C:/users/ryana/documents/sms/build/GMSJ01/src/{unit_path}.o")

    if not orig_obj.exists() or not comp_obj.exists():
        continue

    try:
        # Disassemble original
        orig_asm = subprocess.run(
            [str(objdump), '-d', str(orig_obj)],
            capture_output=True,
            text=True,
            timeout=5
        )

        # Disassemble compiled
        comp_asm = subprocess.run(
            [str(objdump), '-d', str(comp_obj)],
            capture_output=True,
            text=True,
            timeout=5
        )

        # Extract function assembly
        func_name = c['function']
        orig_func = extract_function(orig_asm.stdout, func_name)
        comp_func = extract_function(comp_asm.stdout, func_name)

        if not orig_func or not comp_func:
            continue

        # Count clrlwi. r0, r0, 24 pattern (boolean materialization)
        orig_count = orig_func.count('clrlwi.')
        comp_count = comp_func.count('clrlwi.')

        # Also check for li r0, 1 / li r0, 0 pattern
        orig_li_pattern = len(re.findall(r'li\s+r0,\s*[01]', orig_func))
        comp_li_pattern = len(re.findall(r'li\s+r0,\s*[01]', comp_func))

        if orig_count > comp_count or (orig_count > 0 and orig_li_pattern > comp_li_pattern):
            results.append({
                'source_path': c['source_path'],
                'function': func_name,
                'match_pct': c['match_pct'],
                'orig_clrlwi': orig_count,
                'comp_clrlwi': comp_count,
                'orig_li': orig_li_pattern,
                'comp_li': comp_li_pattern
            })
    except Exception as e:
        pass

def extract_function(asm_text, func_name):
    """Extract assembly for a specific function"""
    lines = asm_text.split('\n')
    in_func = False
    func_lines = []

    for line in lines:
        if f'<{func_name}>' in line or f'<{func_name}+' in line:
            in_func = True
        elif in_func and re.match(r'^[0-9a-f]+\s+<\w+>', line):
            # Next function started
            break

        if in_func:
            func_lines.append(line)

    return '\n'.join(func_lines)

print(f"\nFound {len(results)} functions with boolean materialization opportunities:")
print("=" * 80)
for r in results:
    print(f"{r['match_pct']:.1f}% {r['source_path']} :: {r['function']}")
    print(f"  clrlwi.: orig={r['orig_clrlwi']}, comp={r['comp_clrlwi']}")
    print(f"  li r0,0/1: orig={r['orig_li']}, comp={r['comp_li']}")
    print()
