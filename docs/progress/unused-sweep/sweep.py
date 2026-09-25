import json, os, re, subprocess, sys
from concurrent.futures import ThreadPoolExecutor
os.chdir(os.environ.get('SMS_ROOT') or os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', '..')))
rep = json.load(open('build/GMSE01/report.json'))
env = dict(os.environ, NM='build/binutils/powerpc-eabi-nm')
units = [u for u in rep['units'] if u['name'].startswith('mario/')]
funcs = {u['name']: u.get('functions', []) for u in units}
def run(u):
    name = u['name']
    try:
        out = subprocess.run(['build/venv/bin/python3','tools/validate-symbol-order.py','-u',name,'--map','orig/GMSE01/files/marioUS.MAP'],capture_output=True,text=True,env=env,timeout=120).stdout
    except Exception as e:
        return name, None, []
    src = None
    m = re.search(r'^Source\s*:\s*(\S+)', out, re.M)
    if m: src = m.group(1)
    res = []
    for m in re.finditer(r'  - (\S+)\n\s+map:\s+0x([0-9a-f]+).*\n\s+object:\s+0x([0-9a-f]+)', out):
        res.append((m.group(1), int(m.group(2),16), int(m.group(3),16)))
    return name, src, res
with ThreadPoolExecutor(4) as ex:
    results = list(ex.map(run, units))
def short(sym):
    m = re.match(r'(.+?)__(\d+)?', sym)
    base = sym.split('__')[0] if not sym.startswith('__') else sym
    return base
def callers(src, helper, fns):
    if not src or not os.path.exists(src) or len(helper) < 3 or helper.startswith('__'): return []
    lines = open(src, errors='replace').read().split('\n')
    cur = None; hits = {}
    defre = re.compile(r'^[A-Za-z_].*?(?:(\w+)::)?(~?\w+)\s*\(')
    for ln in lines:
        m = defre.match(ln)
        if m and not ln.rstrip().endswith(';'):
            cur = m.group(2)
        if cur and cur != helper and re.search(r'\b' + re.escape(helper) + r'\s*\(', ln):
            hits[cur] = 1
    out = []
    for f in fns:
        b = f['name'].split('__')[0]
        if b in hits and f.get('fuzzy_match_percent', 100) < 100:
            sz = int(f['size']); fz = f['fuzzy_match_percent']
            out.append((f['name'], sz, fz))
    return out
rows = []
for name, src, res in results:
    for sym, ms, os_ in res:
        h = short(sym)
        cs = callers(src, h, funcs[name])
        cb = sum(s for _, s, _ in cs)
        rows.append((cb, name, sym, os_, ms, os_ - ms, ';'.join('%s(%d,%.1f)' % c for c in cs)))
rows.sort(key=lambda r: (-r[0], r[1]))
with open('docs/progress/unused-sweep/unused_mismatch.tsv', 'w') as f:
    f.write('caller_nonexact_bytes\tunit\tsymbol\tours\tmap\tdelta\tnonexact_callers(size,fuzzy)\n')
    for r in rows: f.write('\t'.join(str(x) for x in r) + '\n')
print(len(rows), 'rows;', sum(1 for r in rows if r[0]), 'with nonexact callers')
