import os, re, glob, collections
ROOT = os.environ.get('SMS_ROOT') or os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', '..', '..')); os.chdir(ROOT)
SCR = os.path.dirname(os.path.abspath(__file__))
src = [l.strip() for l in open(SCR + '/files.txt') if l.strip()]
hdr = [h for h in glob.glob('include/**/*', recursive=True) if os.path.isfile(h) and not h.startswith(('include/PowerPC_EABI_Support', 'include/TRK_MINNOW_DOLPHIN'))]
out = []
for f in src + hdr:
    L = open(f, errors='replace').read().split('\n')
    stack = []
    for i, l in enumerate(L, 1):
        m = re.match(r'\s*#\s*(if|ifdef|ifndef|elif|else|endif)\b(.*)', l)
        if not m: continue
        d, rest = m.group(1), m.group(2)
        if d in ('if', 'ifdef', 'ifndef'):
            mw = '__MWERKS__' in rest
            neg = d == 'ifndef' or bool(re.search(r'!\s*defined\s*\(?\s*__MWERKS__', rest))
            stack.append(dict(start=i, mw=mw, neg=neg, els=None, cond=(d + rest).strip()))
        elif d in ('else', 'elif') and stack:
            if stack[-1]['els'] is None: stack[-1]['els'] = i
        elif d == 'endif' and stack:
            b = stack.pop()
            if b['mw']:
                end_mw = b['els'] or i
                body = '\n'.join(L[b['start']:end_mw - 1])
                has_asm = bool(re.search(r'\basm\b', body))
                area = 'sdk-header' if f.startswith('include/dolphin') else ('header' if f.startswith('include/') else 'src')
                out.append((f, b['start'], area, b['cond'], 'else' if b['els'] else 'no-else', end_mw - b['start'] - 1, has_asm,
                            L[b['start']].strip()[:60]))
with open(SCR + '/guards.tsv', 'w') as fo:
    fo.write('file\tline\tarea\tcondition\telse\tlines_in_mwcc_branch\tasm\tfirst_line\n')
    for o in out: fo.write('\t'.join(map(str, o)) + '\n')
c = collections.Counter((o[2], o[4], o[6]) for o in out)
for k, v in sorted(c.items()): print(k, v)
print('files with guards:', len(set(o[0] for o in out)))
for o in out:
    if o[2] != 'sdk-header': print('%s:%d %s %s asm=%s lines=%d | %s' % (o[0], o[1], o[3], o[4], o[6], o[5], o[7]))
