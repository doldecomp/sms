# Sweep of TU-local static inline helper shapes: tail named-return -> direct,
# direct return -> named result, TVec3 const& <-> by value params.
import sys, os, re, json, importlib.util, subprocess
from concurrent.futures import ThreadPoolExecutor
ROOT=os.environ.get('SMS_ROOT') or os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', '..'))
sys.path.insert(0, ROOT+"/tools")
spec=importlib.util.spec_from_file_location("ls", ROOT+"/tools/lever-search.py")
ls=importlib.util.module_from_spec(spec); sys.argv=[sys.argv[0]]; spec.loader.exec_module(ls)
import tempfile
WORK=os.environ.get("SWEEP_WORK", tempfile.mkdtemp(prefix="helper_sweep_"))
os.makedirs(WORK, exist_ok=True)
ud=json.load(open(ROOT+"/objdiff.json"))
src2unit={u["metadata"].get("source_path"):u["name"] for u in ud["units"] if u.get("metadata")}
SKIP=("MSoundMainSide","fireWanwan","THPDec")
HDR=re.compile(r"^static inline ([^\n(]*?)\b(\w+)\(([^)\n]*)\)\s*\n?\{", re.M)
TAIL=re.compile(r"\n(\t+)([^\n=;]+?)\s*\b(\w+) = ([^\n;]+);\n\1return \3;\n")
ONE=re.compile(r"^\{\s*\n?\s*return ([^;\n]+);\s*\n?\}$")
VEC_CR=re.compile(r"const (JGeometry::TVec3<f32>|TVec3f|JGeometry::TVec3f|Vec)& (\w+)")
VEC_BV=re.compile(r"(?<!const )\b(JGeometry::TVec3<f32>|TVec3f|JGeometry::TVec3f) (\w+)")
files=sorted(set(subprocess.run("grep -rlE '^static inline' src", shell=True, cwd=ROOT, capture_output=True, text=True).stdout.split()))
def helpers(text):
    for m in HDR.finditer(text):
        i=m.end()-1; d=0
        for j in range(i, len(text)):
            if text[j]=="{": d+=1
            elif text[j]=="}":
                d-=1
                if d==0: break
        yield m, i, j+1
def variants(text):
    for m,bs,be in helpers(text):
        rt,name,params=m.group(1).strip(),m.group(2),m.group(3)
        body=text[bs:be]
        pre=text[max(0,m.start()-400):m.start()].split("\n}\n")[-1]
        old=("batch 127" in pre or "Binding level" in pre)
        t=list(TAIL.finditer(body))
        if t and not old:
            tm=t[-1]; lt=tm.group(2)
            nb=body[:tm.start()]+"\n%sreturn %s;\n"%(tm.group(1),tm.group(4))+body[tm.end():]
            yield name,"tail-direct",text[:bs]+nb+text[be:]
            if "&" not in lt and "*" not in lt and "const" not in lt:
                nb=body[:tm.start()]+"\n%sconst %s& %s = %s;\n%sreturn %s;\n"%(tm.group(1),lt,tm.group(3),tm.group(4),tm.group(1),tm.group(3))+body[tm.end():]
                yield name,"tail-constref",text[:bs]+nb+text[be:]
        o=ONE.match(body)
        if o and rt not in ("void","") and "&" not in rt and "static" not in rt and "(" in o.group(1) or (o and re.search(r"[*+/-]", o.group(1)) and "&" not in rt and rt!="void"):
            nb="{\n\t%s r = %s;\n\treturn r;\n}"%(rt.replace("inline ","").strip(),o.group(1))
            yield name,"named-result",text[:bs]+nb+text[be:]
        if VEC_CR.search(params):
            np=VEC_CR.sub(lambda x: "%s %s"%(x.group(1),x.group(2)) if x.group(1)!="Vec" else x.group(0), params)
            if np!=params: yield name,"param-byvalue",text[:m.start(3)]+np+text[m.end(3):]
        if VEC_BV.search(params):
            np=VEC_BV.sub(lambda x: "const %s& %s"%(x.group(1),x.group(2)), params)
            yield name,"param-constref",text[:m.start(3)]+np+text[m.end(3):]
def work(f):
    out=[]
    if any(s in f for s in SKIP): return out
    unit=src2unit.get(f)
    if not unit: return out
    text=open(ROOT+"/"+f).read()
    vs=list(variants(text))
    if not vs: return out
    try:
        s=ls.Searcher(unit.split("/",1)[1] if "/" in unit else unit, "", None, 1, WORK, verbose=False)
        s.u=[u for u in ud["units"] if u["name"]==unit][0]
    except SystemExit as e:
        return [f"{f}\t{e}"]
    try:
        _,d=s.compile(text, keep_json=True)
        if d is None: return [f"{f}\tbase compile fail"]
        base=ls.unit_profile(d)
        if all(v>=100-1e-6 for k,v in base.items() if k!="[extra]" and not k.startswith("[section]")):
            return [f"{f}\tall matched, skipped {len(vs)}"]
        for name,vn,t2 in vs:
            sc,d2=s.compile(t2, keep_json=True)
            if d2 is None: out.append(f"{f}\t{name}\t{vn}\tFAIL"); continue
            prof=ls.unit_profile(d2)
            gains=[(k,base[k],prof[k]) for k in prof if k!="[extra]" and not k.startswith("[section]") and k in base and prof[k]>base[k]+1e-6]
            reg=ls.regressions(base, prof, "")
            tag="GAIN" if gains and not reg else ("mixed" if gains else "-")
            out.append(f"{f}\t{name}\t{vn}\t{tag}\tgains={['%s %.2f->%.2f'%g for g in gains]}\tregs={reg}")
            if tag=="GAIN":
                open(WORK+"/../gain_%s_%s_%s.cpp"%(os.path.basename(f),name,vn),"w").write(t2)
    finally:
        s.close()
    return out
if __name__=="__main__":
    if len(sys.argv)>1 and sys.argv[1]=="count":
        n=0
        for f in files:
            if any(s in f for s in SKIP): continue
            n+=len(list(variants(open(ROOT+"/"+f).read())))
        print(n); sys.exit()
    with ThreadPoolExecutor(int(os.environ.get("J","4"))) as ex:
        for r in ex.map(work, files):
            for l in r: print(l, flush=True)
