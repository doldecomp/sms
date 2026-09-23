import json,re,os,subprocess
r=json.load(open('build/GMSE01/report.json'))
recent=set(subprocess.run(['git','log','--since=6 hours ago','--name-only','--format='],capture_output=True,text=True).stdout.split())
out=[]
for u in r['units']:
    n=u['name'].split('/',1)[1]
    src=None
    for ext in ('.cpp','.c'):
        if os.path.exists('src/'+n+ext): src='src/'+n+ext
    if not src or src in recent: continue
    txt=open(src,errors='ignore').read().split('\n')
    for f in u.get('functions',[]):
        p=f.get('fuzzy_match_percent',0); s=int(f.get('size',0))
        if not (85<=p<99.9 and s>=150): continue
        dm=(f.get('metadata') or {}).get('demangled_name') or f['name']
        short=re.sub(r'\(.*','',dm)
        key=short.split('::')[-1]+'('
        cls=short.split('::')[-2] if '::' in short else ''
        idx=[i for i,l in enumerate(txt) if key in l and (cls in l or not cls) and not l.strip().startswith('//') and not l.rstrip().endswith(';')]
        if not idx: continue
        i=idx[0]
        # find body end: next line starting with '}' at col 0
        j=i
        while j<len(txt) and not txt[j].startswith('}'): j+=1
        region=txt[max(0,i-12):j]
        if any('//' in l or '/*' in l for l in region): continue
        out.append((round(s*(100-p)/100),s,round(p,2),n,dm[:70]))
out.sort(reverse=True)
for o in out[:60]: print('\t'.join(map(str,o)))
