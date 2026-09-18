import json,re,sys
p="config/GMSE01/objects.json"; s=open(p).read()
if "<<<<<<<" not in s: sys.exit(0)
# take every quoted entry in file order, dedupe
entries=[]
for m in re.finditer(r'"([^"]+)"', s):
    if m.group(1) not in entries: entries.append(m.group(1))
json.dump(entries,open(p,"w"),indent=2); open(p,"a").write("\n"); print("resolved",len(entries))
