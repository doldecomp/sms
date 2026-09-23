#!/usr/bin/env python3
"""Symbolic execution of a PPC function (retail vs ours): compares the multiset of
side effects (non-stack stores, calls with args, compares, returns, and branch
predicates) using canonical expression trees. Register allocation, scheduling,
stack layout and fp-contract fusion all normalise away.

Branches: every conditional branch becomes a 'br' effect holding its compare
operands in canonical order, the outcome set that takes it (<, =, >, U for an
unordered float), and the first effect on each successor; an inverted branch
with swapped blocks canonicalises to the same effect, a flipped condition does not.
Loops: values that change around a back edge are tagged with the loop's number
(L1, L2 ... in address order), so swapped nested counters show up.

Usage, from any worktree root (or set SMS_ROOT):
  python3 tools/semantic/symx.py [--band LO HI | --tsv PATH] [filter ...]
"""
import re, sys, os, collections, hashlib, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import sem
run, canon, OBJDUMP = sem.run, sem.canon, sem.OBJDUMP

def S(e):
    s = e if isinstance(e, str) else e[0] + '(' + ','.join(S(x) for x in e[1:]) + ')'
    if len(s) > 100000:
        s = 'H' + hashlib.sha1(s.encode()).hexdigest()[:10]
    return s

def mk(op, *a):
    if op in ('fadd', 'fmul', 'and', 'or', 'xor'):
        a = tuple(sorted(a, key=S))
    if op == 'fneg':
        x = a[0]
        if isinstance(x, tuple) and x[0] == 'fneg':
            return x[1]
        if isinstance(x, tuple) and x[0] == 'fsub':
            return ('fsub', x[2], x[1])
    return (op,) + tuple(a)

def const(v):
    return 'c%d' % v

def iadd(*terms):
    k = 0; ts = []
    for t in terms:
        if isinstance(t, str) and re.fullmatch(r'c-?\d+', t):
            k += int(t[1:])
        elif isinstance(t, tuple) and t[0] == 'add':
            for u in t[1:]:
                if isinstance(u, str) and re.fullmatch(r'c-?\d+', u):
                    k += int(u[1:])
                else:
                    ts.append(u)
        else:
            ts.append(t)
    if k:
        ts.append(const(k))
    if len(ts) == 1:
        return ts[0]
    if not ts:
        return const(0)
    return ('add',) + tuple(sorted(ts, key=S))

def is_sp(e):
    if e == 'SP':
        return 0
    if isinstance(e, tuple) and e[0] == 'add' and 'SP' in e[1:]:
        rest = [u for u in e[1:] if u != 'SP']
        if len(rest) == 1 and isinstance(rest[0], str) and re.fullmatch(r'c-?\d+', rest[0]):
            return int(rest[0][1:])
    return None

def has_sp(e):
    return 'SP' in S(e) if not S(e).startswith('H') else False

def symname(path, tgt):
    t = canon(path, tgt)
    t = re.sub(r'\$\d+', '$', t)
    return t

def parse_params(m):
    """Return (n_int, n_float) for a mangled MWCC param list string, or None."""
    i=0; ni=0; nf=0
    def skip_type(i):
        # returns (next_index, cls) cls in 'i','f','v'
        while i < len(m) and m[i] in 'CV':
            i+=1
        if i>=len(m): return i,None
        c=m[i]
        if c in 'PR':
            j,_=skip_type(i+1); return j,'i'
        if c=='U' or c=='S':
            return i+2,'i'
        if c in 'ilscbwx': return i+1,'i'
        if c in 'fd': return i+1,'f'
        if c=='v': return i+1,'v'
        if c=='e': return i+1,'e'
        if c=='Q':
            n=int(m[i+1]); j=i+2
            for _ in range(n):
                k=j
                while m[k].isdigit(): k+=1
                j=k+int(m[j:k])
            return j,'i'
        if c.isdigit():
            k=i
            while m[k].isdigit(): k+=1
            return k+int(m[i:k]),'i'
        if c=='A':
            k=i+1
            while m[k].isdigit(): k+=1
            j,_=skip_type(k+1); return j,'i'
        if c=='F':
            # function pointer: F<params>_<ret>
            j=m.find('_',i); j2,_=skip_type(j+1); return j2,'i'
        return len(m),None
    try:
        while i < len(m):
            i,c=skip_type(i)
            if c is None: return None
            if c=='i': ni+=1
            elif c=='f': nf+=1
            elif c=='e': break
    except Exception:
        return None
    return ni,nf

def argregs(tgt):
    name=tgt[4:] if tgt.startswith('sym:') else tgt
    MATH={'sinf':(0,1),'cosf':(0,1),'atan2f':(0,2),'sqrtf':(0,1),'fmodf':(0,2),'acosf':(0,1),'asinf':(0,1),'tanf':(0,1),'powf':(0,2),'atan2':(0,2),'sin':(0,1),'cos':(0,1)}
    if name in MATH:
        ni,nf=MATH[name]
    else:
        # the separator is the '__' before the class/'F'; a name may end in '_' ('emitParticle_')
        m=re.search(r'.__(?=[0-9QFC])', name)
        if not m: return ('r3','r4')
        k=m.start()+1
        rest=name[k+2:]
        member = rest[:1].isdigit() or rest[:1]=='Q'
        j=0
        if member:
            if rest[0]=='Q':
                n=int(rest[1]); j=2
                for _ in range(n):
                    q=j
                    while rest[q].isdigit(): q+=1
                    j=q+int(rest[j:q])
            else:
                q=0
                while rest[q].isdigit(): q+=1
                j=q+int(rest[:q])
            while j<len(rest) and rest[j]=='C': j+=1
        if j>=len(rest) or rest[j]!='F': return ('r3','r4')
        pp=parse_params(rest[j+1:])
        if pp is None: return ('r3','r4')
        ni,nf=pp
        if member and not name.startswith('__ct') and True: ni+=1
    return tuple('r%d'%(3+x) for x in range(min(ni,8)))+tuple('f%d'%(1+x) for x in range(min(nf,8)))

CONDS = {'lt': ('lt', True), 'ge': ('lt', False), 'nl': ('lt', False), 'gt': ('gt', True), 'le': ('gt', False),
         'ng': ('gt', False), 'eq': ('eq', True), 'ne': ('eq', False), 'so': ('so', True), 'un': ('so', True),
         'ns': ('so', False), 'nu': ('so', False)}
MIRROR = {'<': '>', '>': '<', '=': '=', 'U': 'U'}
BRRE = re.compile(r'^b(lt|ge|nl|gt|le|ng|eq|ne|so|un|ns|nu)(lr|ctr)?$')
UNIV = {'s': frozenset('<=>'), 'u': frozenset('<=>'), 'i': frozenset('<=>'), 'f': frozenset('<=>U')}
CROPS = ('cror', 'crnor', 'crand', 'crandc', 'crorc', 'crnand', 'creqv', 'crxor')

def newcr(kind, a, b):
    return {'k': kind, 'a': a, 'b': b, 'bits': {'lt': frozenset('<'), 'gt': frozenset('>'), 'eq': frozenset('='),
                                                 'so': frozenset('U') if kind == 'f' else frozenset()}}

def crbit(s):
    m = re.fullmatch(r'(?:4\*(cr\d)\+)?(lt|gt|eq|so|un)', s)
    if m:
        return (m.group(1) or 'cr0'), ('so' if m.group(2) == 'un' else m.group(2))
    if s.isdigit():
        n = int(s)
        return 'cr%d' % (n // 4), ('lt', 'gt', 'eq', 'so')[n % 4]
    return None

def sig1(e):
    """Light signature of an effect, used to name a branch successor."""
    k = e[0]
    if k.startswith('st'):
        return '%s@%s' % (k, short(e[1], 80))
    if k == 'call':
        return 'call ' + e[1]
    if k in ('vcall', 'ret', 'retf'):
        return 'ret' if k == 'retf' else k
    return short(e, 120)

def parse(path, sym):
    out = run([OBJDUMP, '-dr', '--no-show-raw-insn', '--disassemble=' + sym, path])
    insns = []
    for line in out.splitlines():
        m = re.match(r'^\s+([0-9a-f]+):\s+(\S+)\s*(.*)$', line)
        if m and not m.group(2).startswith('R_PPC'):
            insns.append([m.group(2), m.group(3).split('<')[0].strip(), None, int(m.group(1), 16)]); continue
        m = re.match(r'^\s+([0-9a-f]+): (R_PPC_\S+)\s+(\S+)', line)
        if m and insns:
            insns[-1][2] = (m.group(2), m.group(3))
    return insns

def cfg(insns):
    addrs = [i[3] for i in insns]
    idx = {a: k for k, a in enumerate(addrs)}
    preds = collections.defaultdict(list)
    leaders = set([addrs[0]] if addrs else [])
    for k, (mn0, ops0, rel0, ad) in enumerate(insns):
        mnb = mn0.rstrip('+-')
        if mnb.startswith('b') and mnb not in ('bl', 'blr', 'blrl', 'bctrl', 'bctr') and not mnb.endswith('lr') and not (rel0 and rel0[0].startswith('R_PPC_REL24')):
            tg = ops0.split(',')[-1].strip()
            try:
                t = int(tg, 16)
            except ValueError:
                continue
            if t in idx:
                leaders.add(t); preds[t].append(ad)
            if k + 1 < len(insns):
                leaders.add(addrs[k + 1])
                if mnb != 'b':
                    preds[addrs[k + 1]].append(ad)
        elif mnb in ('blr', 'bctr') and k + 1 < len(insns):
            leaders.add(addrs[k + 1])
    heads = sorted(t for t in leaders if any(p >= t for p in preds.get(t, [])))
    loops = {t: n + 1 for n, t in enumerate(heads)}
    return addrs, idx, preds, leaders, loops

def execute(path, sym):
    insns = parse(path, sym)
    cf = cfg(insns)
    _, out1, hdr1 = simulate(path, insns, cf)
    eff, _, _ = simulate(path, insns, cf, back=(out1, hdr1))
    return eff

def simulate(path, insns, cf, back=None):
    addrs, idx, preds, leaders, loops = cf
    R = {}
    for i in range(3, 11):
        R['r%d' % i] = 'a%d' % i
    for i in range(1, 9):
        R['f%d' % i] = 'fa%d' % i
    R['r1'] = 'SP'
    stack = {}
    mem = {}
    eff = []
    crs = {}
    ncall = collections.Counter()
    rawtok = {}
    effat = {}
    brs = []
    hdr = {}
    def g(r):
        if r == '0':
            return const(0)
        return R.get(r, 'init_' + r)
    def addr(d, ra):
        base = const(0) if ra == '0' else g(ra)
        return iadd(base, const(d))
    def stackish(e):
        # a stack address the offset tracking cannot follow (loop-carried pointer into a local array)
        if e == 'SP':
            return True
        if isinstance(e, tuple) and e[0] == 'add':
            return any(stackish(t) for t in e[1:])
        if isinstance(e, tuple) and e[0] == 'L':
            return stackish(e[2])
        return False
    def load(a, size):
        o = is_sp(a)
        if o is None and stackish(a):
            return 'stkL?'
        if o is not None:
            if (o, size) in stack:
                return stack[(o, size)]
            if size == 8 and (o, 4) in stack and (o + 4, 4) in stack:
                return mk('d', stack[(o, 4)], stack[(o + 4, 4)])
            if size == 4 and (o - 4, 8) in stack:
                return mk('lo', stack[(o - 4, 8)])
            if size == 4 and (o, 8) in stack:
                return mk('hi', stack[(o, 8)])
            for (oo, ss), v in stack.items():
                if oo <= o < oo + ss:
                    return mk('part', v, const(o - oo), const(size))
            return 'stk?'
        if (S(a), size) in mem:
            return mem[(S(a), size)]
        v = dataload(a, size)
        if v is not None:
            return v
        return mk('ld%d' % size, a)
    def dataload(a, size):
        # a load from constant data resolves to its value, whatever pool object holds it
        base = None; k = 0
        if isinstance(a, tuple) and a[0] == 'addr' and a[1] in rawtok:
            base = a[1]
        elif isinstance(a, tuple) and a[0] == 'add' and len(a) == 3:
            for p, q in ((a[1], a[2]), (a[2], a[1])):
                if isinstance(p, tuple) and p[0] == 'addr' and p[1] in rawtok and isinstance(q, str) and re.fullmatch(r'c-?\d+', q):
                    base = p[1]; k = int(q[1:])
        if base is None or size not in (1, 2, 4, 8):
            return None
        b = sem.bytes_at(path, rawtok[base], k, size)
        if b is None:
            return None
        if size == 4:
            return 'k4:%r/0x%x' % (struct.unpack('>f', b)[0], struct.unpack('>I', b)[0])
        if size == 8:
            return 'k8:%r' % struct.unpack('>d', b)[0]
        return 'k%d:%s' % (size, b.hex())
    def store(a, size, v):
        o = is_sp(a)
        if o is None and stackish(a):
            return
        if o is not None:
            for k in [k for k in stack if k[0] < o + size and o < k[0] + k[1]]:
                del stack[k]
            stack[(o, size)] = v
            return
        mem[(S(a), size)] = v
        eff.append(('st%d' % size, a, v))
    def setr(r, v):
        R[r] = v
    def stkarg(o):
        words = tuple(S(stack[(o + k, 4)]) if (o + k, 4) in stack else '-' for k in (0, 4, 8))
        return 'SP' if words == ('-', '-', '-') else ('SPobj',) + words
    outstate = {}
    last_addr = None
    def snapshot():
        return (dict(R), dict(stack), dict(mem))
    def merge(states):
        if not states:
            return None
        if len(states) == 1:
            r, st, mm = states[0]
            return dict(r), dict(st), dict(mm)
        keys = set().union(*[set(x[0]) for x in states])
        def phi(vals):
            # a flat set: merging three paths at once or two at a time is the same value
            ss = set()
            for v in vals:
                if isinstance(v, tuple) and v[0] == 'phi':
                    ss.update(v[1:])
                else:
                    ss.add(S(v))
            return vals[0] if len(ss) == 1 else ('phi',) + tuple(sorted(ss))
        r = {}
        for kk in keys:
            r[kk] = phi([x[0].get(kk, 'init_' + kk) for x in states])
        st = {}
        for kk in set.intersection(*[set(x[1]) for x in states]):
            st[kk] = phi([x[1][kk] for x in states])
        mm = {}
        for kk in set.intersection(*[set(x[2]) for x in states]):
            vals = [x[2][kk] for x in states]
            if len(set(S(v) for v in vals)) == 1:
                mm[kk] = vals[0]
        return r, st, mm
    def post(prev):
        pad, pmn, po, pn = prev
        if len(eff) > pn:
            effat.setdefault(pad, sig1(eff[pn]))
        if pmn.endswith('.') and po and re.fullmatch(r'r\d+', po[0]):
            crs['cr0'] = newcr('s', g(po[0]), const(0))
    prev_falls = True
    prev = None
    for mn, ops, rel, ad in insns:
        if prev is not None:
            post(prev)
        mn = mn.rstrip('+-')
        o = [x.strip() for x in ops.split(',')] if ops else []
        prev = (ad, mn, o, len(eff))
        if ad in leaders:
            # record the out-state of the previous block for its fall-through
            if last_addr is not None:
                outstate[last_addr] = snapshot()
            ins = [outstate[p] for p in preds.get(ad, []) if p in outstate]
            if prev_falls and last_addr is not None and last_addr not in preds.get(ad, []):
                ins.append(outstate[last_addr])
            mg = merge(ins)
            if mg is not None:
                R.clear(); R.update(mg[0]); stack.clear(); stack.update(mg[1]); mem.clear(); mem.update(mg[2])
            if ad in loops:
                if back is None:
                    hdr[ad] = snapshot()
                else:
                    # tag values that change around the back edge with this loop's number
                    out1, hdr1 = back
                    h = hdr1.get(ad)
                    bes = [out1[p] for p in preds.get(ad, []) if p >= ad and p in out1]
                    if h is not None and bes:
                        tag = 'L%d' % loops[ad]
                        for kk in list(R):
                            hv = S(h[0].get(kk, 'init_' + kk))
                            if any(S(b[0].get(kk, 'init_' + kk)) != hv for b in bes):
                                R[kk] = ('L', tag, R[kk])
                        for kk in list(stack):
                            hv = S(h[1][kk]) if kk in h[1] else None
                            if any(kk not in b[1] or S(b[1][kk]) != hv for b in bes):
                                stack[kk] = ('L', tag, stack[kk])
                    mem.clear()
        last_addr = ad
        prev_falls = not (mn in ('b', 'blr', 'bctr'))
        if mn.startswith('b') and mn not in ('bl', 'blrl', 'bctrl'):
            outstate[ad] = snapshot()
        if rel and rel[0].startswith('R_PPC_REL24'):
            tgt = symname(path, rel[1])
            ncall[tgt] += 1
            tgt_n = tgt if ncall[tgt] == 1 else '%s#%d' % (tgt, ncall[tgt])
            args = [(r, g(r)) for r in argregs(tgt)]
            # a pointer into the caller's frame carries what the callee reads there: the
            # first three words (a TVec3 / Vec argument) as stored before the call
            args = [(r, v if is_sp(v) is None else stkarg(is_sp(v))) for r, v in args]
            eff.append(('call', tgt) + tuple(('arg', r, v) for r, v in args))
            for rr in ('r3', 'r4', 'r5', 'r6', 'r7', 'r8', 'r9', 'r10'):
                sp = is_sp(g(rr))
                if sp is not None:
                    for k in [k for k in stack if sp <= k[0] < sp + 64]:
                        stack[k] = mk('post', tgt_n, const(k[0] - sp))
            mem.clear()
            for i in [0] + list(range(3, 13)):
                R['r%d' % i] = mk('clob', tgt)
            for i in range(0, 14):
                R['f%d' % i] = mk('clob', tgt)
            # the n-th call's result is its own value (two 'new's are two objects)
            R['r3'] = mk('ret', tgt_n); R['f1'] = mk('fret', tgt_n)
            continue
        if mn in ('blrl', 'bctrl'):
            t = R.get('lr' if mn == 'blrl' else 'ctr', '?')
            eff.append(('vcall', t, ('arg', 'r3', g('r3'))))
            mem.clear()
            for i in [0] + list(range(3, 13)):
                R['r%d' % i] = mk('clob', 'v')
            for i in range(0, 14):
                R['f%d' % i] = mk('clob', 'v')
            R['r3'] = mk('ret', S(t)); R['f1'] = mk('fret', S(t))
            continue
        if mn in ('mtlr', 'mtctr'):
            R['lr' if mn == 'mtlr' else 'ctr'] = g(o[0]); continue
        if mn in CROPS:
            bs = [crbit(x) for x in o]
            if len(bs) == 3 and all(bs) and len(set(b[0] for b in bs)) == 1 and bs[0][0] in crs:
                c = crs[bs[0][0]]; U = UNIV[c['k']]
                A = c['bits'][bs[1][1]]; B = c['bits'][bs[2][1]]
                v = {'cror': A | B, 'crnor': U - (A | B), 'crand': A & B, 'crandc': A - B, 'crorc': A | (U - B),
                     'crnand': U - (A & B), 'creqv': U - (A ^ B), 'crxor': A ^ B}[mn]
                c = dict(c); c['bits'] = dict(c['bits']); c['bits'][bs[0][1]] = frozenset(v)
                crs[bs[0][0]] = c
            continue
        if mn in ('stwu', 'stmw', 'lmw', 'mflr', 'nop', 'crclr', 'crset', 'isync', 'sync'):
            if mn == 'stwu' and o[1].endswith('(r1)'):
                continue
            if mn not in ('stwu',):
                continue
        if mn in ('cmpwi', 'cmplwi', 'cmpw', 'cmplw', 'fcmpo', 'fcmpu'):
            field = o[0] if o[0].startswith('cr') else 'cr0'
            oo = [x for x in o if not x.startswith('cr')]
            a1 = g(oo[0]); a2 = g(oo[1]) if re.fullmatch(r'[rf]\d+', oo[1]) else const(int(oo[1], 0))
            kind = 'f' if mn.startswith('f') else ('u' if mn.startswith('cmpl') else 's')
            crs[field] = newcr(kind, a1, a2)
            eff.append(('cmpf' if kind == 'f' else 'cmp',) + tuple(sorted([a1, a2], key=S)))
            continue
        if mn == 'blr':
            # r3 and f1 separately: one of them is garbage unless the return type uses it
            eff.append(('ret', g('r3')))
            eff.append(('retf', g('f1')))
            continue
        mb = BRRE.match(mn)
        if mb:
            field = o[0] if o and o[0].startswith('cr') else 'cr0'
            rest = o[1:] if o and o[0].startswith('cr') else o
            tgt = None
            if mb.group(2):
                tgt = mb.group(2)
            elif rest:
                try:
                    tgt = int(rest[-1], 16)
                except ValueError:
                    tgt = None
            bit, pol = CONDS[mb.group(1)]
            c = crs.get(field)
            k = idx[ad]
            nxt = addrs[k + 1] if k + 1 < len(addrs) else None
            if c is None:
                pred = ('?', field + bit + str(pol)); swap = False
            else:
                univ = UNIV[c['k']]
                taken = c['bits'][bit] if pol else univ - c['bits'][bit]
                a, b = c['a'], c['b']
                if S(a) > S(b):
                    a, b = b, a
                    taken = frozenset(MIRROR[x] for x in taken)
                ts = ''.join(sorted(taken)); cs = ''.join(sorted(univ - taken))
                swap = cs < ts
                rep = cs if swap else ts
                # signedness matters only for an ordering test
                kind = 'i' if c['k'] in 's u' and rep in ('=', '<>') else c['k']
                pred = (kind, a, b, rep)
            brs.append((ad, pred, tgt, nxt, swap))
            continue
        if mn.startswith('b'):
            continue
        relv = None
        if rel:
            rt, tg = rel
            nm = symname(path, tg)
            rawtok.setdefault(nm, tg)
            if rt.endswith('_HA') or rt.endswith('_HI'):
                setr(o[0], mk('hi', nm)); continue
            relv = nm
        m = re.fullmatch(r'(-?\d+)\((r\d+|0)\)', o[1]) if len(o) >= 2 else None
        # loads / stores with displacement
        LS = {'lwz': 4, 'lhz': 2, 'lha': 2, 'lbz': 1, 'lfs': 4, 'lfd': 8, 'lwzu': 4, 'lfsu': 4, 'lfdu': 8, 'lhzu': 2, 'lbzu': 1, 'lhau': 2}
        SS = {'stw': 4, 'sth': 2, 'stb': 1, 'stfs': 4, 'stfd': 8, 'stwu': 4, 'stfsu': 4, 'stfdu': 8, 'sthu': 2, 'stbu': 1}
        if mn in LS and m:
            if relv is not None:
                a = mk('addr', relv)
            else:
                a = addr(int(m.group(1)), m.group(2))
            v = load(a, LS[mn])
            if mn in ('lha', 'lhau'):
                v = mk('sx16', v)
            setr(o[0], v)
            if mn.endswith('u'):
                setr(m.group(2), a)
            continue
        if mn in SS and m:
            if relv is not None:
                a = mk('addr', relv)
            else:
                a = addr(int(m.group(1)), m.group(2))
            store(a, SS[mn], g(o[0]))
            if mn.endswith('u'):
                setr(m.group(2), a)
            continue
        LX = {'lwzx': 4, 'lhzx': 2, 'lhax': 2, 'lbzx': 1, 'lfsx': 4, 'lfdx': 8}
        SX = {'stwx': 4, 'sthx': 2, 'stbx': 1, 'stfsx': 4, 'stfdx': 8}
        if mn in LX:
            a = iadd(g(o[1]), g(o[2])); v = load(a, LX[mn])
            setr(o[0], mk('sx16', v) if mn == 'lhax' else v); continue
        if mn in SX:
            store(iadd(g(o[1]), g(o[2])), SX[mn], g(o[0])); continue
        if mn in ('li', 'lis', 'addi', 'addis', 'subi'):
            if relv is not None:
                setr(o[0], mk('addr', relv)); continue
            if mn == 'li':
                setr(o[0], const(int(o[1], 0))); continue
            if mn == 'lis':
                setr(o[0], const(int(o[1], 0) << 16)); continue
            imm = int(o[2], 0)
            if mn == 'addis':
                imm <<= 16
            if mn == 'subi':
                imm = -imm
            setr(o[0], iadd(g(o[1]) if o[1] != '0' else const(0), const(imm))); continue
        if mn in ('mr', 'fmr', 'frsp', 'mr.'):
            setr(o[0], g(o[1])); continue
        if mn == 'add' or mn == 'add.':
            setr(o[0], iadd(g(o[1]), g(o[2]))); continue
        if mn in ('subf', 'subf.'):
            setr(o[0], iadd(g(o[2]), mk('neg', g(o[1])))); continue
        if mn == 'neg':
            setr(o[0], mk('neg', g(o[1]))); continue
        if mn in ('slwi',):
            setr(o[0], mk('mul', g(o[1]), const(1 << int(o[2])))); continue
        if mn == 'mulli':
            setr(o[0], mk('mul', g(o[1]), const(int(o[2], 0)))); continue
        if mn == 'mullw':
            a, b = sorted([g(o[1]), g(o[2])], key=S); setr(o[0], mk('mul', a, b)); continue
        FB = {'fadds': 'fadd', 'fadd': 'fadd', 'fsubs': 'fsub', 'fsub': 'fsub', 'fmuls': 'fmul', 'fmul': 'fmul', 'fdivs': 'fdiv', 'fdiv': 'fdiv'}
        if mn in FB:
            setr(o[0], mk(FB[mn], g(o[1]), g(o[2]))); continue
        if mn in ('fmadds', 'fmadd', 'fmsubs', 'fmsub', 'fnmadds', 'fnmadd', 'fnmsubs', 'fnmsub'):
            p = mk('fmul', g(o[1]), g(o[2])); b = g(o[3])
            if mn.startswith('fmadd'):
                v = mk('fadd', p, b)
            elif mn.startswith('fmsub'):
                v = mk('fsub', p, b)
            elif mn.startswith('fnmadd'):
                v = mk('fneg', mk('fadd', p, b))
            else:
                v = mk('fsub', b, p)
            setr(o[0], v); continue
        if mn in ('fneg', 'fabs', 'fnabs', 'fctiwz', 'frsqrte', 'fres', 'extsh', 'extsb', 'cntlzw', 'extsh.', 'extsb.'):
            setr(o[0], mk(mn.rstrip('.'), g(o[1]))); continue
        if len(o) >= 2 and re.fullmatch(r'[rf]\d+', o[0]):
            srcs = [S(g(x)) if re.fullmatch(r'[rf]\d+', x) else x for x in o[1:]]
            setr(o[0], (mn.rstrip('.'),) + tuple(srcs))
    if prev is not None:
        post(prev)
    if back is None:
        return eff, outstate, hdr
    brat = {b[0]: b[1] for b in brs}
    def first(a0):
        if a0 in ('lr',):
            return 'ret'
        if a0 in ('ctr',):
            return 'ctr'
        k = idx.get(a0); steps = 0
        while k is not None and k < len(insns) and steps < 4000:
            mn0, ops0, rel0, a = insns[k]
            if a in effat:
                return effat[a]
            if a in brat:
                return 'br:' + short(('p',) + tuple(brat[a]), 120)
            mnb = mn0.rstrip('+-')
            if mnb == 'b' and not (rel0 and rel0[0].startswith('R_PPC_REL24')):
                try:
                    k = idx.get(int(ops0.split(',')[-1].strip(), 16))
                except ValueError:
                    return '?'
                steps += 1; continue
            if mnb == 'blr':
                return 'ret'
            if mnb == 'bctr':
                return 'switch'
            k += 1; steps += 1
        return 'end'
    for ad, pred, tgt, nxt, swap in brs:
        t = first(tgt) if tgt is not None else '?'
        f = first(nxt) if nxt is not None else 'end'
        if swap:
            t, f = f, t
        eff.append(('br',) + tuple(pred) + (t, f))
    return eff, outstate, hdr


def short(e, n=160):
    x = S(e); return x if len(x) <= n else x[:n] + '...'

def tdiff(a, b, out):
    if S(a) == S(b): return
    if isinstance(a, tuple) and isinstance(b, tuple) and a[0] == b[0] and len(a) == len(b):
        ca, cb = list(a[1:]), list(b[1:])
        if a[0] in ('fadd', 'fmul', 'add', 'and', 'or', 'xor', 'cmp'):
            for x in list(ca):
                for y in cb:
                    if S(x) == S(y):
                        ca.remove(x); cb.remove(y); break
        for x, y in zip(ca, cb):
            tdiff(x, y, out)
        return
    out.append((short(a), short(b)))

def loose(e):
    """Forget what the compared builds legitimately disagree on: which stack slot or call last
    wrote a temporary, call ordinals, garbage left in unused argument registers, static-local names."""
    if isinstance(e, str):
        if e in ('stk?', 'stkL?') or e.startswith('init_'):
            return 'stk'
        if re.fullmatch(r'sym:.*\$', e) or e.startswith('bss:anon'):
            return 'static'
        return re.sub(r'#\d+$', '', e)
    if e[0] == 'post':
        return 'stk'
    if e[0] in ('clob', 'hi'):
        return e[0]
    if e[0] == 'arg':
        v = loose(e[2])
        return ('arg', e[1], 'junk' if 'clob' in S(v) else v)
    if e[0] == 'phi':
        return ('phi',) + tuple(sorted(set(S(loose(x)) for x in e[1:])))
    if e[0] == 'br' and len(e) == 7:
        # operands may sort the other way once stack noise is gone: re-canonicalise
        kind, a, b, rep, t, f = e[1], loose(e[2]), loose(e[3]), e[4], e[5], e[6]
        univ = UNIV.get(kind, UNIV['s'])
        taken = frozenset(rep)
        if S(a) > S(b):
            a, b = b, a
            taken = frozenset(MIRROR[x] for x in taken)
        ts = ''.join(sorted(taken)); cs = ''.join(sorted(univ - taken))
        if cs < ts:
            ts, t, f = cs, f, t
        return ('br', kind, a, b, ts, loose(t), loose(f))
    return tuple(loose(x) if i else x for i, x in enumerate(e))

def main():
    rows, only = sem.load_rows(sys.argv[1:])
    for unit, u, fz, dem, sym in sem.selected(rows, only):
        ea = execute('%s/build/GMSE01/obj/%s.o' % (sem.ROOT, u), sym)
        eb = execute('%s/build/GMSE01/src/%s.o' % (sem.ROOT, u), sym)
        ka = collections.Counter(S(e) for e in ea); kb = collections.Counter(S(e) for e in eb)
        ra = ka - kb; rb = kb - ka
        tag = 'SAME' if not ra and not rb else 'diff -%d +%d' % (sum(ra.values()), sum(rb.values()))
        if ra or rb:
            la_ = collections.Counter(S(loose(e)) for e in ea if e[0] not in ('ret', 'retf'))
            lb_ = collections.Counter(S(loose(e)) for e in eb if e[0] not in ('ret', 'retf'))
            n = sum(((la_ - lb_) + (lb_ - la_)).values())
            tag += ' | loose SAME' if n == 0 else ' | loose %d' % n
        print('=== %s | %s | %s%% | effects %d/%d | %s' % (u, dem, fz, len(ea), len(eb), tag))
        la = [e for e in ea if ra[S(e)] > 0 and not ra.subtract({S(e): 1})]
        lb = [e for e in eb if rb[S(e)] > 0 and not rb.subtract({S(e): 1})]
        used = set()
        for e in la:
            # pair with same kind + same address / callee / compared operands
            best = None
            for j, f in enumerate(lb):
                if j in used or f[0] != e[0]: continue
                if e[0].startswith('st') and S(e[1]) != S(f[1]): continue
                if e[0] in ('call',) and e[1] != f[1]: continue
                if e[0] == 'br' and S(('p',) + tuple(e[2:4])) != S(('p',) + tuple(f[2:4])): continue
                best = j; break
            if best is None and e[0] == 'br':
                best = next((j for j, f in enumerate(lb) if j not in used and f[0] == 'br'), None)
            if best is None:
                print('   - %s' % short(e, 220)); continue
            used.add(best)
            out = []; tdiff(e, lb[best], out)
            head = e[0] + ' ' + (e[1] if isinstance(e[1], str) and e[0] in ('call', 'br') else short(e[1], 60))
            print('   ~ %s :: %s' % (head, ' | '.join('%s  =>  %s' % p for p in out[:3])))
        for j, f in enumerate(lb):
            if j not in used:
                print('   + %s' % short(f, 220))

if __name__ == '__main__':
    main()
