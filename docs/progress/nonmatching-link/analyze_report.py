#!/usr/bin/env python3
"""Runtime-risk lists from the matching build's objdiff report (main checkout, 6e4228e3)."""
import json, collections, sys
if len(sys.argv) < 2:
    sys.exit('usage: analyze_report.py REPORT.json [OUT_DIR]')
R = sys.argv[1]
OUT = (sys.argv[2] if len(sys.argv) > 2 else '.') + '/'
r = json.load(open(R))
m = r['measures']
def f(x, k, d=0): return float(x.get(k, d))
print('overall: fuzzy %.3f%%  matched_code %.3f%%  matched_data %.3f%%  funcs %s/%s  complete_units %s/%s  complete_code %.3f%%' % (
    f(m,'fuzzy_match_percent'), f(m,'matched_code_percent'), f(m,'matched_data_percent'), m.get('matched_functions'), m.get('total_functions'),
    m.get('complete_units'), m.get('total_units'), f(m,'complete_code_percent')))
for c in r.get('categories', []):
    cm = c['measures']
    print('  %-8s fuzzy %.3f%% matched_code %.3f%% complete_code %.3f%% units %s/%s' % (c['id'], f(cm,'fuzzy_match_percent'), f(cm,'matched_code_percent'), f(cm,'complete_code_percent'), cm.get('complete_units'), cm.get('total_units')))
rows = []; nonmatch = collections.Counter(); nonmatch_b = collections.Counter(); datarows = []
for u in r['units']:
    md = u.get('metadata', {}); cat = (md.get('progress_categories') or ['?'])[0]
    for fn in u.get('functions', []):
        fz = f(fn, 'fuzzy_match_percent'); sz = int(fn.get('size', 0))
        if fz < 100.0:
            nonmatch[cat] += 1; nonmatch_b[cat] += sz
        if fz < 95.0:
            dn = fn.get('metadata', {}).get('demangled_name') or fn['name']
            va = fn.get('metadata', {}).get('virtual_address')
            rows.append((cat, u['name'], fz, sz, dn, fn['name'], va))
    for s in u.get('sections', []):
        nm = s['name']; fz = f(s, 'fuzzy_match_percent'); sz = int(s.get('size', 0))
        if nm.split('-')[0] in ('.data', '.rodata', '.sdata', '.sdata2', '.ctors', '.dtors', '.init', 'extab', 'extabindex') and sz and fz < 100.0:
            datarows.append((cat, u['name'], nm, sz, fz))
rows.sort(key=lambda t: (t[0], t[2]))
with open(OUT + 'functions-below-95-fuzzy.tsv', 'w') as o:
    o.write('category\tunit\tfuzzy%\tsize\tdemangled\tsymbol\tvaddr\n')
    for t in rows: o.write('%s\t%s\t%.2f\t%d\t%s\t%s\t%s\n' % t)
with open(OUT + 'data-sections-below-100.tsv', 'w') as o:
    o.write('category\tunit\tsection\tsize\tfuzzy%\n')
    for t in sorted(datarows): o.write('%s\t%s\t%s\t%d\t%.2f\n' % t)
print('\nfunctions <100%% fuzzy by category:', dict(nonmatch), 'bytes', dict(nonmatch_b))
print('functions <95%% fuzzy: %d, %d bytes' % (len(rows), sum(t[3] for t in rows)))
band = collections.Counter(); byc = collections.Counter(); bycb = collections.Counter(); byunit = collections.Counter()
for cat, un, fz, sz, *_ in rows:
    band[('<50' if fz < 50 else '50-80' if fz < 80 else '80-90' if fz < 90 else '90-95')] += 1
    byc[cat] += 1; bycb[cat] += sz; byunit[un] += 1
print('  by band:', dict(band)); print('  by category (count, bytes):', {k: (byc[k], bycb[k]) for k in byc})
print('  zero-size/0%% (likely absent or stub):', sum(1 for t in rows if t[2] == 0.0))
print('  units with most <95%% functions:', byunit.most_common(15))
print('data sections <100%%:', len(datarows), 'in', len({t[1] for t in datarows}), 'units; bytes', sum(t[3] for t in datarows))
for t in sorted(datarows, key=lambda t: t[4])[:25]: print('   %s %s %s size=%d %.2f%%' % t)
print('\nlowest 30 functions:')
for t in sorted(rows, key=lambda t: (t[2], -t[3]))[:30]: print('   %-7s %-45s %6.2f%% %6d  %s' % (t[0], t[1], t[2], t[3], t[4][:90]))
