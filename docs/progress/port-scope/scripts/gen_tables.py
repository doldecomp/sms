#!/usr/bin/env python3
import os, sys, json, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import parse, categorize as C
SCR = os.path.dirname(os.path.abspath(__file__))
PASSES = ['p32', 's32', 'p32ng', 'p64', 'host64', 'compat64']
rows, summary = [], {}
for pas in PASSES:
    res, per_tu, uniq, cat_of, cats, tu_cats = C.analyse(pas)
    area_n = collections.Counter(C.area(r['path']) for r in res)
    area_ok = collections.Counter(C.area(r['path']) for r in res if r['rc'] == 0)
    # errors that -fpermissive would downgrade are tagged in strict passes
    perm_only = {tu for tu, es in per_tu.items() if all('[-fpermissive]' in e[4] for e in es)}
    # greedy fix curve
    remaining = {tu: set(cs) for tu, cs in tu_cats.items()}
    fixed, curve, ok = set(), [], sum(r['rc'] == 0 for r in res)
    while True:
        gain = collections.Counter()
        for tu, cs in remaining.items():
            left = cs - fixed
            if len(left) == 1: gain[next(iter(left))] += 1
        cand = {c for cs in remaining.values() for c in cs} - fixed
        if not cand: break
        best = max(cand, key=lambda c: (gain[c], len(cats[c]['tus'])))
        fixed.add(best)
        ok_now = ok + sum(1 for tu, cs in remaining.items() if cs <= fixed)
        curve.append((best, ok_now))
    summary[pas] = dict(
        units=len(res), clean=sum(r['rc'] == 0 for r in res),
        clean_if_permissive=sum(r['rc'] == 0 for r in res) + len(perm_only),
        by_area={a: [area_ok[a], area_n[a]] for a in area_n},
        raw_errors=sum(len(es) for es in per_tu.values()), unique_locations=len(uniq),
        categories={k: dict(unique=v['unique'], raw=v['raw'], tus=len(v['tus']),
                            examples=['%s:%d: %s' % (e[1][0], e[1][1], C.Q(e[1][4])) for e in sorted(v['ex'], key=lambda x: (-x[0], x[1]))[:3]])
                    for k, v in cats.items()},
        fix_curve=curve)
    for e, tus in uniq.items():
        rows.append((pas, cat_of[e], e[0], e[1], e[2], e[3], C.Q(e[4]), len(tus), sorted(tus)[0], parse.srcline(e[0], e[1])[:160]))
rows.sort(key=lambda r: (PASSES.index(r[0]), r[1], -r[7], r[2], r[3]))
with open(SCR + '/errors.tsv', 'w') as f:
    f.write('pass\tcategory\tfile\tline\tcol\tkind\tmessage\ttus_affected\tsample_tu\tsource_line\n')
    for r in rows: f.write('\t'.join(str(x).replace('\t', ' ') for x in r) + '\n')
json.dump(summary, open(SCR + '/summary.json', 'w'), indent=1)
for pas in PASSES:
    s = summary[pas]
    print('%-9s clean %3d/%d  (if permissive-tagged ignored: %d)  area %s  raw %d uniq %d' % (pas, s['clean'], s['units'], s['clean_if_permissive'], s['by_area'], s['raw_errors'], s['unique_locations']))
    for k, v in sorted(s['categories'].items(), key=lambda kv: -kv[1]['tus']):
        print('    %-30s uniq %4d raw %5d TUs %3d  %s' % (k, v['unique'], v['raw'], v['tus'], v['examples'][0][:110]))
    print('    curve:', ' -> '.join('%s:%d' % c for c in s['fix_curve']))
