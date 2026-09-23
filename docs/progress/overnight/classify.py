import json,subprocess,re,sys,concurrent.futures as cf
r=json.load(open('build/GMSE01/report.json'))
rows=[]
for u in r['units']:
  for f in u.get('functions',[]):
    p=f.get('fuzzy_match_percent',0); s=int(f.get('size',0))
    if 99<=p<100: rows.append((u['name'],(f.get('metadata') or {}).get('demangled_name') or f['name'],p,s))
def go(row):
    unit,dm,p,s=row
    o=subprocess.run(['python3','tools/decomp-diff.py','-u',unit,'-d',dm,'--clusters','-C','0'],capture_output=True,text=True).stdout
    ins=dele=stk=reg=opc=other=0; fr=''
    for l in o.split('\n'):
        if l.startswith('markers:'):
            m=re.search(r'frame: (.*)',l); fr=m.group(1) if m else ''
        if not l or l[0] not in '~<>': continue
        if l[0]=='<': dele+=1; continue
        if l[0]=='>': ins+=1; continue
        parts=l.split('|')
        if len(parts)<3: other+=1; continue
        L=parts[1].strip(); R=parts[2].strip()
        if L.split()[:1]!=R.split()[:1]: opc+=1
        elif 'r1)' in L or 'r1,' in L: stk+=1
        elif re.sub(r'[rf]\d+','R',L)==re.sub(r'[rf]\d+','R',R): reg+=1
        else: other+=1
    if ins or dele or opc: cls='instr'
    elif other: cls='operand'
    elif stk and not reg: cls='stack'
    elif reg and not stk: cls='reg'
    else: cls='stack+reg'
    return (s,round(p,2),cls,ins,dele,opc,stk,reg,other,unit.split('/',1)[1],dm[:90],fr)
with cf.ThreadPoolExecutor(3) as ex:
    res=list(ex.map(go,rows))
res.sort(reverse=True)
with open(sys.argv[1],'w') as w:
    for x in res: w.write('\t'.join(map(str,x))+'\n')
