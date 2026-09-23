# Scan every non-100% function for ~ lines where only an integer immediate differs
import json,subprocess,re,sys
from concurrent.futures import ThreadPoolExecutor
r=json.load(open('build/GMSE01/report.json'))
IMM_OPS={'li','lis','cmpwi','cmplwi','addi','addic','addis','subfic','mulli','andi.','andis.','ori','oris','xori','xoris','rlwinm','rlwinm.','rlwimi','srawi','slwi','srwi','clrlwi','clrrwi','rotlwi','extlwi','extrwi','cmpw','twi'}
tok=re.compile(r'[\w.@$:<>\-]+')
def norm(s): return s.replace('{','').replace('}','').strip()
def isint(t): return re.fullmatch(r'-?(0x[0-9a-fA-F]+|\d+)',t) is not None
def classify(left,right):
    L=norm(left); R=norm(right)
    if L==R: return None
    lo=L.split(None,1); ro=R.split(None,1)
    if len(lo)<2 or len(ro)<2 or lo[0]!=ro[0]: return None
    op=lo[0]
    la=[a.strip() for a in lo[1].split(',')]; ra=[a.strip() for a in ro[1].split(',')]
    if len(la)!=len(ra): return None
    if 'r1' in la or 'r1' in ra or '(r1)' in lo[1] or '(r1)' in ro[1]: return None
    if op=='lfs' or op=='lfd':
        # same reg, pool entry differs
        if la[0]==ra[0] and la[1]!=ra[1] and '@' in la[1]+ra[1]: return 'float'
        return None
    if op not in IMM_OPS: return None
    if op in ('addi','addis','addic') and (la[1] in ('r2','r13') or ra[1] in ('r2','r13')): return None
    diff=[(a,b) for a,b in zip(la,ra) if a!=b]
    if not diff: return None
    for a,b in diff:
        if not (isint(a) and isint(b)): return None
    return 'imm'
def work(job):
    n,dn,fz=job
    try:
        out=subprocess.run(['python3','tools/decomp-diff.py','-u',n,'-d',dn,'--clusters','-C','0'],capture_output=True,text=True,timeout=120).stdout
    except Exception as e: return []
    res=[]
    for l in out.split('\n'):
        if not l.startswith('~'): continue
        parts=l[1:].split('|')
        if len(parts)<3: continue
        off=parts[0].strip(); left=parts[1]; right=parts[2]
        k=classify(left,right)
        if k: res.append((n,dn,'%.1f'%fz,off,k,norm(left),norm(right)))
    return res
jobs=[]
for u in r['units']:
    for f in u.get('functions',[]):
        fz=f.get('fuzzy_match_percent',0)
        if fz>=100: continue
        jobs.append((u['name'],f.get('metadata',{}).get('demangled_name',f['name']),fz))
print(len(jobs),file=sys.stderr)
with ThreadPoolExecutor(8) as ex, open(sys.argv[1],'w') as o:
    o.write('unit\tfunction\tfuzzy\toffset\tkind\ttarget\tours\n')
    for res in ex.map(work,jobs):
        for row in res: o.write('\t'.join(row)+'\n')
