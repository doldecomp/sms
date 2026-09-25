import os, re, glob, sys
ROOT = os.environ.get('SMS_ROOT') or os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', '..', '..')); os.chdir(ROOT)
SCR = os.path.dirname(os.path.abspath(__file__))
INC = ['include', 'include/PowerPC_EABI_Support/Msl/MSL_C/MSL_Common', 'include/PowerPC_EABI_Support/Msl/MSL_C++/MSL_Common', 'build/GMSE01/include']
files = [l.strip() for l in open(SCR + '/files.txt') if l.strip()]
files += [h for h in glob.glob('include/**/*', recursive=True) if os.path.isfile(h) and not h.startswith(('include/PowerPC_EABI_Support', 'include/TRK_MINNOW_DOLPHIN'))]
def ci_resolve(base, rel):
    cur = base
    for part in rel.split('/'):
        if part in ('.', ''): continue
        if part == '..': cur = os.path.dirname(cur); continue
        try: ents = os.listdir(cur)
        except OSError: return None
        hit = [e for e in ents if e.lower() == part.lower()]
        if not hit: return None
        cur = os.path.join(cur, hit[0])
    return cur
bad = []
for f in files:
    try: txt = open(f, errors='replace').read().split('\n')
    except OSError: continue
    for i, l in enumerate(txt, 1):
        m = re.match(r'\s*#\s*include\s*([<"])([^>"]+)[>"]', l)
        if not m: continue
        rel = m.group(2)
        bases = ([os.path.dirname(f)] if m.group(1) == '"' else []) + INC
        if any(os.path.exists(os.path.join(b, rel)) for b in bases): continue
        for b in bases:
            r = ci_resolve(b, rel)
            if r: bad.append((f, i, rel, os.path.relpath(r, b))); break
        else:
            bad.append((f, i, rel, 'MISSING'))
for b in bad: print('%s:%d\t%s\t-> %s' % b)
