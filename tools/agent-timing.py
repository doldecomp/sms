#!/usr/bin/env python3
"""Break an agent transcript's wall time into model turns and tool calls.

usage: agent-timing.py <agent.jsonl>... [--top N]

Model time = gap between a tool result (or the prompt) and the next assistant
message; tool time = gap between a tool_use and its tool_result. Bash commands
are bucketed by what they run (ninja, decomp-diff, lever-search, grep/read ...).
"""
import json, re, sys
from datetime import datetime
from collections import defaultdict

def ts(s): return datetime.fromisoformat(s.replace('Z', '+00:00')).timestamp()

def bucket(name, inp):
    if name != 'Bash': return name
    c = inp.get('command', '')
    for k, pat in [('ninja changes_all', r'ninja changes_all'), ('ninja full', r'ninja\s*($|[;&|>])'),
                   ('ninja obj', r'ninja \S+\.o'), ('decomp-diff', r'decomp-diff'), ('lever-search', r'lever-search'),
                   ('symbol-order', r'validate-symbol'), ('objdump/nm', r'objdump|powerpc-eabi-nm'),
                   ('git', r'^\s*(cd [^;&]+[;&]+\s*)?git '), ('python script', r'python3? .*\.py|python3? -|<<'),
                   ('grep/sed/cat', r'\b(grep|sed|cat|head|tail|awk|rg)\b')]:
        if re.search(pat, c): return 'Bash:' + k
    return 'Bash:other'

def analyse(path, top):
    ev = [json.loads(l) for l in open(path)]
    pending, tools, model = {}, defaultdict(lambda: [0, 0.0]), []
    last = None; slow = []; first = None; out_tok = 0; cache_read = 0
    for e in ev:
        t = e.get('timestamp');  msg = e.get('message') or {}
        if not t: continue
        t = ts(t); first = first or t
        if e.get('type') == 'assistant':
            u = msg.get('usage') or {}
            out_tok += u.get('output_tokens', 0); cache_read += u.get('cache_read_input_tokens', 0)
            if last is not None: model.append(t - last); last = None
            for c in msg.get('content', []):
                if isinstance(c, dict) and c.get('type') == 'tool_use':
                    pending[c['id']] = (t, bucket(c['name'], c.get('input', {})), json.dumps(c.get('input', {}))[:140])
        elif e.get('type') == 'user':
            cs = msg.get('content'); cs = cs if isinstance(cs, list) else []
            for c in cs:
                if isinstance(c, dict) and c.get('type') == 'tool_result' and c.get('tool_use_id') in pending:
                    t0, b, s = pending.pop(c['tool_use_id']); d = t - t0
                    tools[b][0] += 1; tools[b][1] += d; slow.append((d, b, s))
            last = t
    wall = t - first; mt = sum(model); tt = sum(v[1] for v in tools.values())
    print(f"== {path.split('/')[-1]}: wall {wall:.0f}s  model {mt:.0f}s ({100*mt/max(wall,1):.0f}%, {len(model)} turns, "
          f"{mt/max(len(model),1):.1f}s/turn)  tools {tt:.0f}s  out {out_tok} tok  cache-read {cache_read/1e6:.2f}M")
    for b, (n, d) in sorted(tools.items(), key=lambda x: -x[1][1]):
        print(f"   {b:24s} {n:4d} calls {d:7.1f}s  ({d/n:.1f}s avg)")
    for d, b, s in sorted(slow, reverse=True)[:top]:
        print(f"   slow {d:6.1f}s {b}: {s}")
    return wall, mt, tt, tools

if __name__ == '__main__':
    a = sys.argv[1:]; top = 5
    if '--top' in a: i = a.index('--top'); top = int(a[i + 1]); del a[i:i + 2]
    for p in a: analyse(p, top)
