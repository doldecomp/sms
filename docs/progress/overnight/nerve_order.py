#!/usr/bin/env python3
"""Owner-order check for nerve statics: key each .sbss/.bss static (instance$N, init$N, nerve$localstatic,
@N dtor entries) by the nerve vtable (__vt__..TNerve.., not TNerveBase) relocated nearest to it, then compare
the retail split object's address order with ours. Usage: nerve_order3.py [-v] [unit-substring]"""
import re, subprocess, sys, glob, os
OBJD = "build/binutils/powerpc-eabi-objdump"; NM = "build/binutils/powerpc-eabi-nm"
def analyze(obj):
    nm = subprocess.run([NM, "-n", obj], capture_output=True, text=True).stdout
    datasec = {}
    for l in nm.splitlines():
        p = l.split()
        if len(p) >= 3 and p[1].lower() in "bs" and not p[-1].startswith("..."):
            datasec[p[-1]] = (p[1].lower(), int(p[0], 16))
    dis = subprocess.run([OBJD, "-dr", obj], capture_output=True, text=True).stdout
    rel = [m.group(1).split('+')[0] for m in re.finditer(r'R_PPC_\S+\s+(\S+)', dis)]
    owner = {}
    for i, s in enumerate(rel):
        if s not in datasec or s in owner: continue
        if not re.match(r"(instance|init|nerve)\$|@\d+$", s): continue
        if "localstatic" in s and "theNerve" in s:
            owner[s] = s.split("$")[-1]; continue
        best = None
        pat = r'__dt__(\d+TNerve\w*?)Fv' if s.startswith("@") else r'__vt__(\d+TNerve(?!Base)\w*)'
        for d in range(1, 5):
            for j in (i - d, i + d):
                if 0 <= j < len(rel) and re.match(pat, rel[j]) and "TNerveBase" not in rel[j]:
                    best = rel[j]; break
            if best: break
        if best: owner[s] = re.match(pat, best).group(1)
    res = {}
    for s, (sec, addr) in sorted(datasec.items(), key=lambda kv: (kv[1][0], kv[1][1])):
        if s in owner:
            k = (sec, "dtorent" if s.startswith("@") else ("init" if s.startswith("init") else "inst"))
            lst = res.setdefault(k, [])
            if owner[s] not in lst: lst.append(owner[s])
    return res
flt = [a for a in sys.argv[1:] if a != "-v"]
for tgt in sorted(glob.glob("build/GMSE01/obj/**/*.o", recursive=True)):
    if flt and flt[0] not in tgt: continue
    ours = tgt.replace("/obj/", "/src/", 1)
    if not os.path.exists(ours): continue
    r = analyze(tgt)
    if not r: continue
    o = analyze(ours)
    bad = False
    for key, lst in r.items():
        ol = [x for x in o.get(key, []) if x in lst]
        rl = [x for x in lst if x in ol]
        if ol != rl:
            if not bad: print("DIFF", tgt); bad = True
            print("  ", key, "\n     retail:", [x.replace("__vt__","") for x in rl], "\n     ours:  ", [x.replace("__vt__","") for x in ol])
    if not bad and "-v" in sys.argv: print("OK", tgt, {k: len(v) for k, v in r.items()})
