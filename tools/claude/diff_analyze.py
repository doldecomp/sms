import re

with open('C:/Users/ryana/Documents/sms/diff_out.txt') as f:
    lines = f.readlines()

compiled_start = None
for i, line in enumerate(lines):
    if 'COMPILED' in line:
        compiled_start = i
        break

orig = lines[3:compiled_start]
comp = lines[compiled_start+3:]

def get_hex(line):
    m = re.match(r'\s+([0-9a-f]+):\t([0-9a-f ]+)\t', line)
    if m:
        return m.group(2).strip()
    return None

orig_hex = []
comp_hex = []
for line in orig:
    h = get_hex(line)
    if h:
        orig_hex.append(h)
for line in comp:
    h = get_hex(line)
    if h:
        comp_hex.append(h)

print(f'Orig instructions: {len(orig_hex)}, Compiled: {len(comp_hex)}')

for i in range(min(len(orig_hex), len(comp_hex))):
    if orig_hex[i] != comp_hex[i]:
        print(f'\nFirst diff at instruction {i}:')
        for j in range(max(0,i-3), min(i+8, len(orig_hex), len(comp_hex))):
            marker = '>>>' if j >= i and orig_hex[j] != comp_hex[j] else '   '
            print(f'{marker} {j}: O={orig_hex[j]:24s}  C={comp_hex[j]}')
        break
else:
    if len(orig_hex) != len(comp_hex):
        print(f'Instructions match up to {min(len(orig_hex), len(comp_hex))}, but lengths differ')
        extra_in = 'orig' if len(orig_hex) > len(comp_hex) else 'comp'
        longer = orig_hex if len(orig_hex) > len(comp_hex) else comp_hex
        start = min(len(orig_hex), len(comp_hex))
        print(f'Extra in {extra_in}:')
        for j in range(start, min(start+10, len(longer))):
            print(f'  {j}: {longer[j]}')
