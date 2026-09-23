#!/usr/bin/env python3
"""Semantic-signature triage: compare retail vs ours per function as multisets of
calls, non-stack memory offsets, immediates and resolved literals."""
import re, subprocess, sys, struct, collections, os

HERE = os.path.dirname(os.path.abspath(__file__))
# Worktree root: $SMS_ROOT, else the cwd when it holds a build, else the checkout holding this script.
ROOT = os.environ.get('SMS_ROOT') or (os.getcwd() if os.path.isdir('build/GMSE01') else os.path.dirname(os.path.dirname(HERE)))
OBJDUMP = ROOT + '/build/binutils/powerpc-eabi-objdump'
TSV = ROOT + '/docs/progress/nonmatching-link/functions-below-95-fuzzy.tsv'

_symcache = {}
_seccache = {}

def run(args):
    return subprocess.run(args, capture_output=True, text=True, errors='replace').stdout

def symtab(path):
    if path in _symcache:
        return _symcache[path]
    d = {}
    for line in run([OBJDUMP, '-t', path]).splitlines():
        m = re.match(r'^([0-9a-f]{8}) (.{7}) (\S+)\s+([0-9a-f]{8}) (.*)$', line)
        if m:
            d.setdefault(m.group(5), (m.group(3), int(m.group(1), 16), int(m.group(4), 16), m.group(2)))
    _symcache[path] = d
    return d

def section(path, sec):
    key = (path, sec)
    if key in _seccache:
        return _seccache[key]
    data = bytearray()
    base = None
    for line in run([OBJDUMP, '-s', '-j', sec, path]).splitlines():
        m = re.match(r'^ ([0-9a-f]{4,8}) ((?:[0-9a-f]{2,8} ?){1,4})', line)
        if m:
            off = int(m.group(1), 16)
            hexs = ''.join(m.group(2).split())
            if base is None:
                base = off
            need = off - base
            if len(data) < need:
                data.extend(b'\0' * (need - len(data)))
            data[need:need + len(hexs) // 2] = bytes.fromhex(hexs)
    _seccache[key] = bytes(data)
    return _seccache[key]

def resolve(path, target):
    """(section, offset, symbol size) of a relocation target, or None."""
    m = re.match(r'^(.*?)(?:([+-])0x([0-9a-f]+))?$', target)
    name, sign, add = m.group(1), m.group(2), m.group(3)
    addend = int(add, 16) * (-1 if sign == '-' else 1) if add else 0
    st = symtab(path)
    if name.startswith('.') and name not in st:
        return name, addend, 0, name
    if name in st:
        sec, val, size, _ = st[name]
        return sec, val + addend, size, name
    return None

READONLY = ('.rodata', '.sdata2')

def bytes_at(path, target, extra, n):
    """n bytes at target+extra when they are constant: read-only sections, or compiler-generated
    (anonymous, '...data.N' / '@N') objects in .data; None for anything that may be written."""
    r = resolve(path, target)
    if r is None:
        return None
    sec, off, size, name = r
    anon = name.startswith('@') or name.startswith('...') or name.startswith('.')
    if sec not in READONLY and not (sec in ('.data', '.sdata') and anon and '$' not in name):
        return None
    data = section(path, sec)
    off += extra
    if off < 0 or off + n > len(data):
        return None
    return data[off:off + n]

def canon(path, target):
    """Canonicalise a relocation target into a value-based token."""
    m = re.match(r'^(.*?)(?:([+-])0x([0-9a-f]+))?$', target)
    name, sign, add = m.group(1), m.group(2), m.group(3)
    addend = int(add, 16) * (-1 if sign == '-' else 1) if add else 0
    st = symtab(path)
    anon = name.startswith('@') or name.startswith('...') or name.startswith('.') or name.startswith('lbl_')
    if not anon:
        return 'sym:' + name + (('+%x' % addend) if addend else '')
    if name.startswith('.') and name not in st:
        # a local object reached through its section symbol: use its own name when it has one
        for nm, (sec2, val2, size2, fl) in st.items():
            if sec2 == name and val2 == addend and size2 and not (nm.startswith('@') or nm.startswith('.')):
                return 'sym:' + nm
        sec, val, size = name, 0, 0
    elif name in st:
        sec, val, size, _ = st[name]
    else:
        return 'sym:' + target
    if sec in ('.text', '*UND*'):
        return 'sym:' + target
    data = section(path, sec)
    off = val + addend
    if sec == '.bss' or sec == '.sbss' or not data:
        # compiler-named statics ('@123', '...bss.0', '.bss') differ between builds for the same object
        return 'bss:%s+%x' % (name if not (name.startswith('@') or name.startswith('.')) else 'anon', addend)
    # an all-zero object (empty string, 0.0f, zero vector) pools under different sizes per build
    if off < len(data) and not any(data[off:off + max(4, size - addend if size > addend else 4)]):
        return 'zero'
    # string?
    end = data.find(b'\0', off)
    if end > off + 1 and all(32 <= c < 127 or c in (9, 10) for c in data[off:end]) and (size == 0 or size >= end - off):
        return 'str:' + repr(data[off:end].decode())[:60]
    if sec in ('.data', '.sdata'):
        # compiler-pooled .data objects: name by content, not by the pool symbol's size
        return 'data:%s:%s' % (sec, data[off:off + 16].hex())
    if size == 8 or (sec == '.sdata2' and size == 0 and (off % 8 == 0) and False):
        return 'f64:%r' % struct.unpack('>d', data[off:off + 8])[0]
    if size in (0, 4) or sec in ('.sdata2',):
        if off + 4 <= len(data):
            f = struct.unpack('>f', data[off:off + 4])[0]
            i = struct.unpack('>I', data[off:off + 4])[0]
            return 'lit4:%r/0x%x' % (f, i)
    return 'data:%s:%d:%s' % (sec, size, data[off:off + min(size, 16)].hex())

BR_CLASS = {'lt': 'lt/ge', 'ge': 'lt/ge', 'gt': 'gt/le', 'le': 'gt/le', 'eq': 'eq/ne', 'ne': 'eq/ne',
            'nl': 'lt/ge', 'ng': 'gt/le', 'so': 'so', 'ns': 'so', 'un': 'so', 'nu': 'so'}

def sig(path, sym):
    out = run([OBJDUMP, '-dr', '--no-show-raw-insn', '--disassemble=' + sym, path])
    insns = []  # (addr, mnem, ops, reloc)
    for line in out.splitlines():
        m = re.match(r'^\s+([0-9a-f]+):\s+(\S+)\s*(.*)$', line)
        if m and not m.group(2).startswith('R_PPC'):
            insns.append([int(m.group(1), 16), m.group(2), m.group(3).strip(), None])
            continue
        m = re.match(r'^\s+([0-9a-f]+): (R_PPC_\S+)\s+(\S+)', line)
        if m and insns:
            insns[-1][3] = (m.group(2), m.group(3))
    c = collections.Counter()
    for addr, mn, ops, rel in insns:
        mn0 = mn.rstrip('+-')
        if rel:
            rt, tgt = rel
            if rt.endswith('_HA') or rt.endswith('_HI'):
                continue
            tok = canon(path, tgt)
            if mn0 in ('bl', 'b'):
                c['call:' + tok[4:] if tok.startswith('sym:') else 'call:' + tok] += 1
            else:
                kind = 'ld' if mn0.startswith('l') else ('st' if mn0.startswith('st') else 'ref')
                c['%s:%s' % (kind, tok)] += 1
            continue
        if mn0 in ('bl',):
            c['call:?' + ops] += 1
            continue
        mm = re.match(r'^(r\d+|f\d+),(-?\d+)\((r\d+)\)$', ops)
        if mm and (mn0[0] in 'ls' or mn0.startswith('psq')):
            if mm.group(3) in ('r1',):
                continue
            base = 'r13' if mm.group(3) == 'r13' else ('r2' if mm.group(3) == 'r2' else 'R')
            op = mn0.rstrip('u') if mn0 not in ('lu',) else mn0
            c['mem:%s:%s%s' % (op, mm.group(2), '' if base == 'R' else '@' + base)] += 1
            continue
        mm = re.match(r'^(psq_\w+)\s*', mn0)
        if mm:
            c['psq:' + ops.split(',', 1)[1] if ',' in ops else mn0] += 1
            continue
        if mn0 in ('li', 'lis', 'cmpwi', 'cmplwi', 'mulli', 'ori', 'oris', 'xori', 'andi.', 'subfic', 'srawi', 'slwi', 'srwi', 'clrlwi', 'clrrwi', 'rlwinm', 'rotlwi', 'extlwi', 'extrwi', 'rlwimi', 'inslwi', 'insrwi'):
            parts = ops.split(',')
            imm = ','.join(p for p in parts if not re.match(r'^(r|cr|f)\d+$', p))
            c['imm:%s:%s' % (mn0, imm)] += 1
            continue
        if mn0 in ('addi', 'addic', 'addic.', 'addis', 'subi'):
            parts = ops.split(',')
            if len(parts) == 3 and parts[1] != 'r1':
                v = int(parts[2], 0)
                if mn0 == 'subi':
                    v = -v
                c['addi:%d' % v] += 1
            continue
        if mn0.startswith('f') and mn0 not in ('fmr', 'frsp') or mn0 in ('mullw', 'divw', 'divwu', 'add', 'subf', 'neg', 'and', 'or', 'xor', 'slw', 'srw', 'sraw', 'mulhw', 'mulhwu', 'extsh', 'extsb', 'cntlzw', 'nor', 'andc', 'orc'):
            c['op:' + mn0] += 1
            continue
        mm = re.match(r'^b(lt|ge|gt|le|eq|ne|nl|ng|so|ns|un|nu)(lr|ctr)?$', mn0)
        if mm:
            c['br:' + BR_CLASS[mm.group(1)]] += 1
            continue
    return c, len(insns)

def band_rows(lo, hi, report=None):
    """Functions with lo <= fuzzy < hi from report.json, lowest first, in the TSV's column layout."""
    import json
    r = json.load(open(report or ROOT + '/build/GMSE01/report.json'))
    rows = []
    for u in r['units']:
        cat = ((u.get('metadata') or {}).get('progress_categories') or ['?'])[0]
        for f in u.get('functions', []):
            fz = f.get('fuzzy_match_percent', 0.0)
            if lo <= fz < hi:
                md = f.get('metadata') or {}
                rows.append([cat, u['name'], '%.2f' % fz, f.get('size', '0'), md.get('demangled_name', f['name']), f['name'], md.get('virtual_address', '0')])
    rows.sort(key=lambda x: float(x[2]))
    return rows

def load_rows(argv):
    """Options: --band LO HI (report.json, lowest first) or --tsv PATH (default: the below-95 TSV).
    Returns (rows, remaining filter words); a filter matches the demangled name, the symbol or the unit."""
    args = list(argv)
    if '--band' in args:
        k = args.index('--band')
        rows = band_rows(float(args[k + 1]), float(args[k + 2]))
        del args[k:k + 3]
    else:
        path = TSV
        if '--tsv' in args:
            k = args.index('--tsv'); path = args[k + 1]; del args[k:k + 2]
        rows = [l.rstrip('\n').split('\t') for l in open(path)][1:]
    return rows, args

def selected(rows, only):
    for cat, unit, fz, size, dem, sym, vaddr in rows:
        if float(fz) == 0.0:
            continue
        u = unit.split('/', 1)[1]
        if only and not any(o in dem or o in u or o == sym for o in only):
            continue
        yield unit, u, fz, dem, sym

def main():
    rows, only = load_rows(sys.argv[1:])
    for unit, u, fz, dem, sym in selected(rows, only):
        ret = '%s/build/GMSE01/obj/%s.o' % (ROOT, u)
        our = '%s/build/GMSE01/src/%s.o' % (ROOT, u)
        a, na = sig(ret, sym)
        b, nb = sig(our, sym)
        print('=== %s | %s | %s%% | insns %d -> %d' % (u, dem, fz, na, nb))
        keys = sorted(set(a) | set(b))
        minus = ['%s x%d' % (k, a[k] - b[k]) for k in keys if a[k] > b[k] and not k.startswith('br:') and not k.startswith('op:')]
        plus = ['%s x%d' % (k, b[k] - a[k]) for k in keys if b[k] > a[k] and not k.startswith('br:') and not k.startswith('op:')]
        ops = ['%s %d->%d' % (k, a[k], b[k]) for k in keys if a[k] != b[k] and (k.startswith('br:') or k.startswith('op:'))]
        if not minus and not plus and not ops:
            print('   CLEAN')
        if minus:
            print('   - retail only: ' + '; '.join(minus))
        if plus:
            print('   + ours only:   ' + '; '.join(plus))
        if ops:
            print('   ~ counts:      ' + '; '.join(ops))

if __name__ == '__main__':
    main()
