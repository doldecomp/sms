#!/usr/bin/env python3
"""Syntax-check every in-scope unit with g++/gcc for one pass configuration."""
import os, re, sys, subprocess, json, time
from concurrent.futures import ThreadPoolExecutor
ROOT = os.environ.get('SMS_ROOT') or os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', '..', '..'))
SCR = os.path.dirname(os.path.abspath(__file__))
PASSES = {
    # name: (m32, permissive, gekko, msl_includes, u32_is_int, compat_shim)
    'p32':    (True,  True,  True,  True,  False, False),
    's32':    (True,  False, True,  True,  False, False),
    'p32ng':  (True,  True,  False, True,  False, False),
    # 'p64' = 64-bit STRICT with u32/s32 forced to int (LLP64/port-like widths)
    'p64':    (False, False, True,  True,  True,  False),
    'host64': (False, True,  True,  False, False, False),
    # compat64 = port-like: 64-bit strict, host libc/libstdc++, u32=int, compat.h, -fno-gnu-keywords, case-fixed includes
    'compat64': (False, False, True, False, True, True),
}
def ninja_objs():
    txt = open(ROOT + '/build.ninja').read().replace('$\n', '')
    objs, cur = {}, None
    for line in txt.split('\n'):
        m = re.match(r'build build/GMSE01/(src/\S+)\.o: (mwcc\S*) (\S+)', line)
        if m:
            cur = m.group(3); objs[cur] = {}; continue
        if cur and line.startswith('  '):
            m2 = re.match(r'\s+(\w+) = (.*)', line)
            if m2: objs[cur][m2.group(1)] = m2.group(2)
        elif not line.startswith('  '):
            cur = None
    return objs
OBJS = ninja_objs()
CFG = open(ROOT + '/configure.py').read()
def unit_info(path):
    rel = path[len('src/'):]
    o = OBJS.get(path)
    if o and 'cflags' in o:
        cf = o['cflags']
        defs = re.findall(r'-D\s*(\S+)', cf)
        incs = re.findall(r'-i\s+(\S+)', cf)
        cxx = bool(re.search(r'-lang[=\s]+c\+\+', cf)) or path.endswith('.cpp')
        pch = bool(re.search(r'-prefix\s+SMS\.mch', cf))
        src = 'ninja'
    else:
        jsys = path.startswith('src/JSystem/')
        defs = ['BUILD_VERSION=2', 'VERSION_GMSE01', 'GEKKO', 'NDEBUG=1']
        if re.search(r'Object\([^)]*"%s"[^)]*cflags=cflags_jsystem_dsp' % re.escape(rel), CFG): defs.remove('GEKKO')
        incs = ['include', 'include/PowerPC_EABI_Support/Msl/MSL_C/MSL_Common',
                'include/PowerPC_EABI_Support/Msl/MSL_C++/MSL_Common', 'build/GMSE01/include']
        cxx = jsys or path.startswith('src/THPPlayer/') or path.endswith('.cpp')  # THPPlayer lib has -lang=c++
        pch = bool(re.search(r'PCHObject\([^)]*"%s"' % re.escape(rel), CFG))
        src = 'configure'
    return dict(defs=defs, incs=incs, cxx=cxx, pch=pch, flagsrc=src)
def cmd_for(path, pas):
    m32, perm, gekko, msl, u32int, compat = PASSES[pas]
    u = unit_info(path)
    if u['cxx']:
        c = ['g++', '-x', 'c++', '-std=gnu++03']
        if perm: c.append('-fpermissive')
    else:
        c = ['gcc', '-x', 'c', '-std=gnu99']
    c += ['-fsyntax-only', '-w', '-fmax-errors=0', '-fno-diagnostics-show-caret',
          '-fdiagnostics-plain-output', '-m32' if m32 else '-m64']
    for i in u['incs']:
        if not msl and 'PowerPC_EABI_Support' in i: continue
        c.append('-I' + i)
    if compat:
        c.append('-I' + SCR + '/casefix')
        if u['cxx']: c.append('-fno-gnu-keywords')
    for d in u['defs']:
        if d == 'GEKKO' and not gekko: continue
        c.append('-D' + d)
    if gekko and 'GEKKO' not in u['defs']: pass  # dsp units never had it
    if u32int: c += ['-include', SCR + '/shadow_types.h']
    if compat: c += ['-include', SCR + '/compat.h']
    if u['pch']: c += ['-include', 'include/SMS.pch']
    c.append(path)
    return c, u
def run_one(args):
    path, pas = args
    c, u = cmd_for(path, pas)
    t = time.time()
    p = subprocess.run(c, cwd=ROOT, capture_output=True, text=True, errors='replace')
    dt = time.time() - t
    out = SCR + '/passes/%s/%s.txt' % (pas, path.replace('/', '__'))
    with open(out, 'w') as f: f.write(p.stderr)
    return dict(path=path, rc=p.returncode, secs=round(dt, 2), cmd=' '.join(c), **u)
if __name__ == '__main__':
    pas = sys.argv[1]
    files = sys.argv[2:] or [l.strip() for l in open(SCR + '/files.txt') if l.strip()]
    os.makedirs(SCR + '/passes/' + pas, exist_ok=True)
    with ThreadPoolExecutor(2) as ex:
        res = list(ex.map(run_one, [(f, pas) for f in files]))
    jp = SCR + '/passes/%s.json' % pas
    if len(sys.argv) > 2 and os.path.exists(jp):
        old = {r['path']: r for r in json.load(open(jp))}
        for r in res: old[r['path']] = r
        res_all = list(old.values())
    else:
        res_all = res
    json.dump(res_all, open(jp, 'w'), indent=0)
    ok = sum(1 for r in res if r['rc'] == 0)
    print(pas, 'units', len(res), 'pass', ok, 'total_secs', round(sum(r['secs'] for r in res), 1))
    if len(sys.argv) > 2:
        for r in res: print(r['rc'], r['secs'], r['flagsrc'], r['cmd'])
