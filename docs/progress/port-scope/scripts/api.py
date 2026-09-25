#!/usr/bin/env python3
"""Count SDK/hardware API uses in game + JSystem + THPPlayer sources."""
import os, re, glob, collections
ROOT = os.environ.get('SMS_ROOT') or os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', '..', '..'))
SCR = os.path.dirname(os.path.abspath(__file__))
os.chdir(ROOT)
def strip_comments(t):
    t = re.sub(r'/\*.*?\*/', lambda m: '\n' * m.group(0).count('\n'), t, flags=re.S)
    return re.sub(r'//[^\n]*', '', t)
PFX = r'(?:GX|GD|OS|DVD|PAD|VI|AI|ARQ|AR|DSP|CARD|SI|EXI|DC|IC|LC|PPC|DB|PSMTX|PSVEC|PSQUAT|C_MTX|C_VEC|C_QUAT|MTX|VEC|THP|HW|L2|AX|MIX|SP|CMPR|DEMO)'
# 1) names declared by the SDK headers (include/dolphin/**, dolphin.h, os/)
sdk_names = {}
hdrs = glob.glob('include/dolphin/**/*.h', recursive=True) + ['include/dolphin.h'] + glob.glob('include/os/**/*.h', recursive=True)
for h in hdrs:
    t = strip_comments(open(h, errors='replace').read())
    for m in re.finditer(r'\b(' + PFX + r'[A-Za-z0-9_]*)\s*\(', t):
        n = m.group(1)
        if re.fullmatch(r'[A-Z0-9_]+', n) and '_' in n and not n.startswith(('GX_', 'OS_')):  # ALLCAPS macros kept only if prefixed like OSxxx
            pass
        sdk_names.setdefault(n, os.path.relpath(h, 'include'))
# 2) walk the in-scope sources
files = [l.strip() for l in open(SCR + '/files.txt') if l.strip()]
files += [h for h in glob.glob('include/**/*.h*', recursive=True)
          if not h.startswith(('include/dolphin', 'include/PowerPC_EABI_Support', 'include/TRK_MINNOW_DOLPHIN', 'include/os/'))
          and h.endswith(('.h', '.hpp', '.pch'))]
def area(f):
    if f.startswith('include/'): return 'headers'
    top = f.split('/')[1]
    if top == 'JSystem': return 'jsystem'
    if top in ('THPPlayer', 'OdemuExi2'): return top.lower()
    return 'game'
calls = collections.defaultdict(lambda: collections.Counter())
tus = collections.defaultdict(set)
for f in files:
    t = strip_comments(open(f, errors='replace').read())
    for m in re.finditer(r'(?<![\w.>:])(' + PFX + r'[A-Za-z0-9_]*)\s*\(', t):
        n = m.group(1)
        if n not in sdk_names and re.fullmatch(r'GX(Position|Normal|Color|TexCoord|MatrixIndex|Cmd|Param)\d\w*', n):
            sdk_names[n] = 'dolphin/gx/GXVert.h (macro-generated WGPipe writers)'
        if n not in sdk_names: continue
        calls[n][area(f)] += 1
        tus[n].add(f)
def subsystem(n, hdr):
    for p, s in [('PSMTX', 'MTX (paired-single)'), ('PSVEC', 'MTX (paired-single)'), ('PSQUAT', 'MTX (paired-single)'),
                 ('C_MTX', 'MTX (C)'), ('C_VEC', 'MTX (C)'), ('C_QUAT', 'MTX (C)'), ('MTX', 'MTX'), ('VEC', 'MTX'),
                 ('GD', 'GD'), ('GX', 'GX'), ('DVD', 'DVD'), ('PAD', 'PAD'), ('VI', 'VI'), ('ARQ', 'ARQ'), ('AR', 'AR'),
                 ('AI', 'AI'), ('DSP', 'DSP'), ('CARD', 'CARD'), ('SI', 'SI'), ('EXI', 'EXI'), ('DC', 'OS cache'),
                 ('IC', 'OS cache'), ('LC', 'OS cache'), ('PPC', 'OS/PPC'), ('DB', 'DB'), ('THP', 'THP'), ('OS', 'OS')]:
        if n.startswith(p): return s
    return 'other'
rows = []
for n in sorted(calls, key=lambda n: (-sum(calls[n].values()), n)):
    c = calls[n]
    rows.append((subsystem(n, sdk_names[n]), n, sum(c.values()), c['game'], c['jsystem'], c['headers'], c['thpplayer'], c['odemuexi2'], len(tus[n]), sdk_names[n]))
with open(SCR + '/api-surface.tsv', 'w') as f:
    f.write('subsystem\tfunction\ttotal_uses\tgame_src\tjsystem_src\tnon_sdk_headers\tthpplayer_src\todemuexi2_src\tfiles\tdeclared_in\n')
    for r in rows: f.write('\t'.join(map(str, r)) + '\n')
agg = collections.defaultdict(lambda: [0, 0, 0, 0, set(), 0, 0])
for r in rows:
    a = agg[r[0]]; a[0] += 1; a[1] += r[2]; a[2] += r[3]; a[3] += r[4]; a[4] |= tus[r[1]]; a[5] += r[5]; a[6] += r[6]
print('subsystem | distinct fns | uses | game src | jsystem src | non-SDK headers | THP | files')
for s, a in sorted(agg.items(), key=lambda kv: -kv[1][1]):
    print(s, a[0], a[1], a[2], a[3], a[5], a[6], len(a[4]), sep=' | ')
print('distinct', len(rows), 'total calls', sum(r[2] for r in rows))
print('top 40:', ', '.join('%s %d' % (r[1], r[2]) for r in rows[:40]))
