#!/usr/bin/env python3
"""Compare the from-source (--non-matching) link map against the retail US map."""
import os, re, struct, sys, collections, json
ROOT = os.environ.get('SMS_ROOT') or os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', '..'))
ORIG_MAP = ROOT + '/orig/GMSE01/files/marioUS.MAP'
NEW_MAP = ROOT + '/build/GMSE01/mario.elf.MAP'
ORIG_DOL = ROOT + '/orig/GMSE01/sys/main.dol'
OUT = os.environ.get('OUT', 'nonmatching-link-out') + '/'
LIVE = re.compile(r'^\s+([0-9a-f]{8}) ([0-9a-f]{6,8}) ([0-9a-f]{8})(?: ([0-9a-f]{8}))?\s+(\d+) (\S+) \t(.*)$')
UNUSED = re.compile(r'^\s+UNUSED\s+([0-9a-f]{6,8}) \.{8} (\S+) (.*)$')
HDR = re.compile(r'^(\S+) section layout')
def base(o):
    o = o.strip().split()[-1] if o.strip() else '?'
    return re.sub(r'\.(o|c|cpp|cp|s|S)$', '', o)
def parse(path):
    sec = None; live = []; unused = []; secsz = collections.OrderedDict()
    for line in open(path, encoding='latin-1'):
        line = line.rstrip('\r\n')
        m = HDR.match(line)
        if m: sec = m.group(1); continue
        m = LIVE.match(line)
        if m and sec:
            size = int(m.group(2), 16); va = int(m.group(3), 16); sym = m.group(6); ob = base(m.group(7))
            if sym != sec:
                live.append((sec, sym, size, va, ob))
                secsz[(sec, ob)] = secsz.get((sec, ob), 0) + size  # live symbol bytes (retail section lines report pre-strip sizes)
            continue
        m = UNUSED.match(line)
        if m and sec:
            unused.append((sec, m.group(2), int(m.group(1), 16), base(m.group(3))))
    return live, unused, secsz
ol, ou, os_ = parse(ORIG_MAP); nl, nu, ns = parse(NEW_MAP)
out = open(OUT + 'link-map-compare.txt', 'w')
def p(*a): print(*a, file=out)
# 1. per-section totals and per-object deltas
p('== Section totals (sum of per-object contributions): orig -> from-source')
secs = sorted({k[0] for k in list(os_) + list(ns)})
for s in secs:
    a = sum(v for k, v in os_.items() if k[0] == s); b = sum(v for k, v in ns.items() if k[0] == s)
    p(f'  {s:12} {a:9d} -> {b:9d}  {b-a:+d}')
p('\n== Per-object section size deltas (nonzero)')
keys = sorted(set(os_) | set(ns))
deltas = [(k[0], k[1], os_.get(k, 0), ns.get(k, 0)) for k in keys if os_.get(k, 0) != ns.get(k, 0)]
for s, o, a, b in sorted(deltas, key=lambda t: (t[0], t[3]-t[2])):
    p(f'  {s:10} {o:40} {a:8d} -> {b:8d}  {b-a:+d}')
# 2. objects present in one map only (any section)
oobj = {k[1] for k in os_}; nobj = {k[1] for k in ns}
p(f'\n== Objects only in retail map: {sorted(oobj - nobj)}')
p(f'== Objects only in from-source map: {sorted(nobj - oobj)}')
# 3. symbol liveness differences (ignore compiler-local @ labels)
def named(x): return not x.startswith('@') and '$' not in x and not x.startswith('...')
olive = {(s, y) for s, y, *_ in ol if named(y)}; nlive = {(s, y) for s, y, *_ in nl if named(y)}
oun = {(s, y) for s, y, *_ in ou}; nun = {(s, y) for s, y, *_ in nu}
oliven = {y for _, y in olive}; nliven = {y for _, y in nlive}
objof = {}
for s, y, sz, va, ob in ol: objof.setdefault(y, ob)
for s, y, sz, ob in ou: objof.setdefault(y, ob)
for s, y, sz, va, ob in nl: objof.setdefault(y, ob)
for s, y, sz, ob in nu: objof.setdefault(y, ob)
lost = sorted(y for y in oliven - nliven)
gained = sorted(y for y in nliven - oliven)
def status(y, liveset, unset):
    return 'unused' if any(k[1] == y for k in unset) else 'absent'
nun_names = {y for _, y in nun}; oun_names = {y for _, y in oun}
p(f'\n== Named symbols live in retail but not live from source: {len(lost)}')
for y in lost: p(f'  {objof.get(y,"?"):32} {y:70} now {"UNUSED(dead-stripped)" if y in nun_names else "absent"}')
p(f'\n== Named symbols live from source but not live in retail: {len(gained)}')
for y in gained: p(f'  {objof.get(y,"?"):32} {y:70} retail {"UNUSED" if y in oun_names else "absent"}')
# 4. section moves of named symbols
osec = {}; nsec = {}
for s, y, *_ in ol: osec.setdefault(y, s)
for s, y, *_ in nl: nsec.setdefault(y, s)
moved = sorted((y, osec[y], nsec[y]) for y in osec if y in nsec and osec[y] != nsec[y] and named(y))
# original bytes for moved data
d = open(ORIG_DOL, 'rb').read()
off = struct.unpack('>18I', d[0:72]); adr = struct.unpack('>18I', d[72:144]); siz = struct.unpack('>18I', d[144:216])
def dolbytes(va, n):
    for i in range(18):
        if siz[i] and adr[i] <= va < adr[i] + siz[i]:
            o = off[i] + va - adr[i]; return d[o:o+n]
    return None
ova = {y: (va, sz) for s, y, sz, va, ob in ol}
p(f'\n== Named symbols whose section changed: {len(moved)}')
for y, a, b in moved:
    va, sz = ova[y]; bts = dolbytes(va, sz)
    z = 'n/a(bss)' if bts is None else ('all-zero' if not any(bts) else 'NONZERO retail bytes')
    p(f'  {objof.get(y,"?"):32} {y:60} {a} -> {b}  size {sz}  {z}')
# 5. .ctors owners
oct_ = sorted(o for (s, o), v in os_.items() if s == '.ctors'); nct = sorted(o for (s, o), v in ns.items() if s == '.ctors')
p(f'\n== .ctors contributors only in from-source: {sorted(set(nct)-set(oct_))}')
p(f'== .ctors contributors only in retail: {sorted(set(oct_)-set(nct))}')
# 6. function size differences (.text), by object
osz = {y: sz for s, y, sz, va, ob in ol if s == '.text'}; nsz = {y: sz for s, y, sz, va, ob in nl if s == '.text'}
diffs = [(objof.get(y, '?'), y, osz[y], nsz[y]) for y in osz if y in nsz and osz[y] != nsz[y]]
p(f'\n== .text functions live in both with different size: {len(diffs)}')
for ob, y, a, b in sorted(diffs): p(f'  {ob:32} {y:70} {a:6d} -> {b:6d} {b-a:+d}')
out.close()
print(open(OUT + 'link-map-compare.txt').read()[:200000].split('\n== Per-object')[0])
print('objects retail/new', len(oobj), len(nobj), 'only-retail', sorted(oobj - nobj)[:20], 'only-new', sorted(nobj - oobj)[:20])
print('lost', len(lost), 'gained', len(gained), 'moved', len(moved), 'ctors new-only', sorted(set(nct)-set(oct_)), 'retail-only', sorted(set(oct_)-set(nct)), 'textdiffs', len(diffs))
print('moved nonzero:', sum(1 for y,a,b in moved if (lambda v: v is not None and any(v))(dolbytes(*ova[y]))))
