#!/usr/bin/env python3
"""
equiv-check.py -- certify non-matching functions as semantically equivalent to retail.

For every function it compares the retail object (build/GMSE01/obj/<unit>.o)
with ours (build/GMSE01/src/<unit>.o).  Both are disassembled with objdump,
relocations are read straight from the ELF, and each basic block is executed
symbolically.  Values are hash-consed expression trees, so register names,
instruction order inside a block, and stack-slot placement do not matter;
what is compared is:

  * the CFG (same blocks in address order, same terminators, same successors);
  * per block, the ordered list of side effects: non-stack loads, stores,
    calls (target + argument registers the callee reads), special-purpose
    instructions;
  * branch conditions, and r3/r4/f1 at every return.

Block-entry values are equivalence classes of (side, register/slot) computed
from all predecessors' exit states (optimistic start, refined to a fixpoint on
loops), so register allocation may differ per block as long as the live values
agree at every edge.

Verdicts:
  EQUIVALENT   L1: same opcode sequence, only registers / stack slots / frame
               differ.  L2: same CFG, per-block dataflow equal (scheduling).
  DIFFERENT    first differing block and instruction pair.
  UNSUPPORTED  CFG differs, symbol missing, or a construct the checker does
               not model (it never guesses).

Usage:
  tools/equiv-check.py                       # all non-exact functions, print summary
  tools/equiv-check.py --write               # ... and write docs/progress/equiv/*
  tools/equiv-check.py -u Camera/cameralib -f CLBCalcPointInCubeRatio__FRC3VecRC3VecRC3VecRC3VecPfPfPf [--show]
  tools/equiv-check.py --exact-sample 200    # validation: exact functions must be EQUIVALENT
  tools/equiv-check.py --selftest 40         # validation: mutated exact functions
"""

import argparse
import collections
import json
import os
import random
import re
import struct
import subprocess
import sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(os.path.realpath(__file__)), ".."))
VERSION = "GMSE01"
BUILD = os.path.join(ROOT, "build", VERSION)
OBJDUMP = os.path.join(ROOT, "build", "binutils", "powerpc-eabi-objdump")
CALLEE_CACHE = os.path.join(BUILD, "equiv-callees.json")
OUT_DIR = os.path.join(ROOT, "docs", "progress", "equiv")

GPR = ["r%d" % i for i in range(32)]
FPR = ["f%d" % i for i in range(32)]
CRF = ["cr%d" % i for i in range(8)]
ALLREGS = GPR + FPR + CRF + ["ca", "ctr", "lr"]
REGIDX = {r: i for i, r in enumerate(ALLREGS)}
CALLEE_SAVED = frozenset(GPR[14:] + FPR[14:] + ["cr2", "cr3", "cr4"])
VOLATILE = ["r0"] + GPR[3:13] + FPR[0:14] + ["cr0", "cr1", "cr5", "cr6", "cr7", "ca", "ctr"]
IND_ARGS = GPR[3:11] + FPR[1:9] + ["cr1"]
RET_REGS = ["r3", "r4", "f1"]


def mask(regs):
    m = 0
    for r in regs:
        m |= 1 << REGIDX[r]
    return m


def unmask(m):
    return [r for r in ALLREGS if m >> REGIDX[r] & 1]


VOLMASK = mask(VOLATILE)
INDMASK = mask(IND_ARGS)

# ----------------------------------------------------------------------------
# ELF + objdump


class Elf:
    def __init__(self, path):
        with open(path, "rb") as fh:
            d = fh.read()
        self.d = d
        shoff = struct.unpack_from(">I", d, 0x20)[0]
        shentsize, shnum, shstrndx = struct.unpack_from(">HHH", d, 0x2E)
        self.secs = []
        for i in range(shnum):
            f = struct.unpack_from(">10I", d, shoff + i * shentsize)
            self.secs.append({"name_off": f[0], "type": f[1], "flags": f[2], "offset": f[4], "size": f[5], "link": f[6], "info": f[7]})
        so = self.secs[shstrndx]["offset"]
        for s in self.secs:
            s["name"] = self.cstr(so + s["name_off"])
        self.syms = []
        self.relocs = collections.defaultdict(dict)
        for s in self.secs:
            if s["type"] == 2:
                st = self.secs[s["link"]]["offset"]
                for i in range(s["size"] // 16):
                    nm, val, size, info, _o, shndx = struct.unpack_from(">IIIBBH", d, s["offset"] + 16 * i)
                    self.syms.append({"name": self.cstr(st + nm), "value": val, "size": size, "type": info & 15, "shndx": shndx, "idx": i})
        for s in self.secs:
            if s["type"] == 4:
                for i in range(s["size"] // 12):
                    off, info, add = struct.unpack_from(">IIi", d, s["offset"] + 12 * i)
                    self.relocs[s["info"]][off] = (info & 255, info >> 8, add)

    def cstr(self, o):
        return self.d[o:self.d.index(b"\0", o)].decode("latin1")

    def secdata(self, idx):
        s = self.secs[idx]
        if s["type"] == 8:
            return None
        return self.d[s["offset"]:s["offset"] + s["size"]]


LINE_RE = re.compile(r"^\s*([0-9a-f]+):\t(\S+)\s*(.*)$")


class Obj:
    def __init__(self, path):
        self.path = path
        self.elf = Elf(path)
        secs = self.elf.secs
        pending = collections.defaultdict(list)
        for i, s in enumerate(secs):
            if s["flags"] & 4 and s["type"] == 1:
                pending[s["name"]].append(i)
        self.text = {}
        out = subprocess.run([OBJDUMP, "-d", "-M", "raw,750cl", "--no-show-raw-insn", path],
                             capture_output=True, text=True).stdout
        cur = None
        for line in out.splitlines():
            if line.startswith("Disassembly of section "):
                nm = line[len("Disassembly of section "):].rstrip(":")
                cur = pending[nm].pop(0) if pending[nm] else None
                if cur is not None:
                    self.text[cur] = {}
                continue
            if cur is None:
                continue
            m = LINE_RE.match(line)
            if m:
                self.text[cur][int(m.group(1), 16)] = (m.group(2), m.group(3))
        self.funcs = {}
        self.bysec = collections.defaultdict(list)
        for s in self.elf.syms:
            if 0 < s["shndx"] < 0xFF00 and s["type"] != 3 and s["type"] != 4:
                self.bysec[s["shndx"]].append(s)
            if s["type"] == 2 and s["shndx"] in self.text and s["size"] > 0:
                self.funcs.setdefault(s["name"], s)
        for v in self.bysec.values():
            v.sort(key=lambda s: (s["value"], -s["size"]))

    def covering(self, sec, off):
        best = None
        for s in self.bysec.get(sec, ()):
            if s["value"] > off:
                break
            if s["size"] and s["value"] <= off < s["value"] + s["size"]:
                if best is None or (is_anon(best["name"]) and not is_anon(s["name"])) or \
                        (is_anon(best["name"]) == is_anon(s["name"]) and s["value"] > best["value"]):
                    best = s
        if best is None:
            for s in self.bysec.get(sec, ()):
                if s["value"] <= off and not s["name"].startswith("..."):
                    best = s
        return best

    def next_start(self, sec, off):
        for s in self.bysec.get(sec, ()):
            if s["value"] > off:
                return s["value"]
        return self.elf.secs[sec]["size"]


def is_anon(name):
    return (not name) or name.startswith("@") or name.startswith("...") or \
        re.match(r"^lbl_[0-9A-Fa-f]{8}$", name) is not None or name.startswith("$")


def norm_name(name):
    return name


RO_SECS = (".rodata", ".sdata2", ".sbss2")


def secclass(name):
    if name in RO_SECS:
        return "ro"
    if name in (".bss", ".sbss"):
        return "bss"
    if name.startswith(".text") or name == ".init":
        return "text"
    return "rw"


class RelocCtx:
    """Turns ELF relocations into comparable keys (symbol names; anonymous data by bytes)."""

    def __init__(self, obj, fsec, fstart, fend):
        self.obj, self.fsec, self.fstart, self.fend = obj, fsec, fstart, fend
        self.ro = {}
        self.ordinals = {}

    def ordinal(self, ident):
        """Per-function bijection for compiler-numbered objects (first-reference order)."""
        return self.ordinals.setdefault(ident, len(self.ordinals))

    def named(self, name, sec):
        m = re.match(r"^(.*)\$(\d+)$", name)
        if m:
            return ("dollar", m.group(1), self.ordinal(("dollar", name)))
        return ("sym", norm_name(name))

    def key(self, symidx, addend, depth=0):
        """Returns (key, addend, secclass)."""
        obj = self.obj
        s = obj.elf.syms[symidx]
        sec = s["shndx"]
        if s["type"] == 3 or is_anon(s["name"]):
            if 0 < sec < 0xFF00:
                off = s["value"] + addend
                if sec == self.fsec and self.fstart <= off < self.fend:
                    return ("selfcode",), 0, "text"
                cov = obj.covering(sec, off)
                cls = secclass(obj.elf.secs[sec]["name"])
                if cov is not None and not is_anon(cov["name"]) and cov["type"] != 3:
                    return self.named(cov["name"], sec), off - cov["value"], cls
                k = self.anon(sec, cov, off, depth)
                if cls in ("rw", "bss") and depth == 0 and not (cov is not None and cov["name"].startswith("@stringBase")) \
                        and not any(r[2] == ("selfcode",) for r in (k[3] if len(k) > 3 and isinstance(k[3], tuple) else ())):
                    k = k + (("ord", self.ordinal(("anon", sec, cov["value"] if cov is not None else off))),)
                return k, 0, cls
            return ("sym", s["name"]), addend, "ext"
        cls = secclass(obj.elf.secs[sec]["name"]) if 0 < sec < 0xFF00 else "ext"
        return self.named(s["name"], sec), addend, cls

    def anon(self, sec, cov, off, depth):
        obj = self.obj
        secname = obj.elf.secs[sec]["name"]
        if cov is not None and cov["size"] and cov["value"] + cov["size"] > off:
            end = cov["value"] + cov["size"]
        else:
            end = obj.next_start(sec, off)
        data = obj.elf.secdata(sec)
        if data is None:
            return ("anon", secclass(secname), "zero", end - off)
        if cov is not None and cov["name"].startswith("@stringBase"):
            nul = data.find(b"\0", off)
            end = nul + 1 if 0 <= nul < end else end
        b = bytearray(data[off:end])
        rels = []
        rtab = obj.elf.relocs.get(sec, {})
        for roff in range(off, end):
            r = rtab.get(roff)
            if r is None:
                continue
            if depth >= 2:
                return ("deep",)
            k, a, _c = self.key(r[1], r[2], depth + 1)
            rels.append((roff - off, r[0], k, a))
            for j in range(roff - off, min(roff - off + 4, len(b))):
                b[j] = 0
        return ("anon", secclass(secname), bytes(b), tuple(rels))


# ----------------------------------------------------------------------------
# Instruction decoding


class Unsupported(Exception):
    pass


LOADS = {"lbz": 1, "lhz": 2, "lha": 2, "lwz": 4, "lfs": 4, "lfd": 8}
STORES = {"stb": 1, "sth": 2, "stw": 4, "stfs": 4, "stfd": 8}
LOADX = {"lbzx": 1, "lhzx": 2, "lhax": 2, "lwzx": 4, "lfsx": 4, "lfdx": 8, "lhbrx": 2, "lwbrx": 4}
STOREX = {"stbx": 1, "sthx": 2, "stwx": 4, "stfsx": 4, "stfdx": 8, "stfiwx": 4, "sthbrx": 2, "stwbrx": 4}
X3 = {"add", "addc", "adde", "subf", "subfc", "subfe", "mullw", "mulhw", "mulhwu", "divw", "divwu", "and", "andc",
      "or", "orc", "xor", "nand", "nor", "eqv", "slw", "srw", "sraw"}
X2 = {"neg", "extsb", "extsh", "cntlzw", "addze", "addme", "subfze", "subfme"}
CA_OUT = {"addc", "adde", "subfc", "subfe", "sraw", "addze", "addme", "subfze", "subfme", "addic", "subfic", "srawi"}
CA_IN = {"adde", "subfe", "addze", "addme", "subfze", "subfme"}
IMM = {"addi", "addis", "addic", "subfic", "mulli", "ori", "oris", "xori", "xoris", "andi", "andis"}
FP3 = {"fadd", "fadds", "fsub", "fsubs", "fmul", "fmuls", "fdiv", "fdivs"}
FP4 = {"fmadd", "fmadds", "fmsub", "fmsubs", "fnmadd", "fnmadds", "fnmsub", "fnmsubs", "fsel"}
FP2 = {"fneg", "fabs", "fnabs", "fmr", "frsp", "fctiw", "fctiwz", "fres", "frsqrte"}
CROPS = {"crand", "crandc", "creqv", "crnand", "crnor", "cror", "crorc", "crxor"}
COMM = {"add", "addc", "adde", "mullw", "mulhw", "mulhwu", "and", "or", "xor", "nand", "nor", "eqv",
        "fadd", "fadds", "fmul", "fmuls", "ps_add", "ps_mul"}
SIDE = {"sync", "isync", "eieio", "mftb", "mfmsr", "mtmsr", "mfsr", "mtsr", "mffs", "mtfsf", "mtfsb0", "mtfsb1",
        "mtfsfi", "dcbf", "dcbi", "dcbst", "dcbt", "dcbtst", "dcbz", "dcbz_l", "icbi", "mfsrin", "mtsrin"}
PS_CMP = {"ps_cmpu0", "ps_cmpo0", "ps_cmpu1", "ps_cmpo1"}
CRBIT = {"lt": 0, "gt": 1, "eq": 2, "so": 3, "un": 3}
MEM_RE = re.compile(r"^(-?(?:0x[0-9a-f]+|\d+))\((r\d+|0)\)$")
IDENT_PAIRS = {("stw", "lwz"), ("stfd", "lfd")}


def pint(t):
    neg = t.startswith("-")
    t = t[1:] if neg else t
    v = int(t, 16) if t.startswith("0x") else int(t, 10)
    return -v if neg else v


def isreg(t):
    return re.fullmatch(r"[rf]\d+", t) is not None


def crb(t):
    m = re.fullmatch(r"4\*cr(\d)\+(lt|gt|eq|so|un)", t)
    if m:
        return 4 * int(m.group(1)) + CRBIT[m.group(2)]
    if t in CRBIT:
        return CRBIT[t]
    return pint(t)


class Ins:
    __slots__ = ("addr", "raw", "mnem", "rc", "cls", "defs", "uses", "imms", "mem", "src", "width", "copy",
                 "target", "bo", "bi", "spr", "rel", "callkey", "stk", "idx", "vdefs")

    def __init__(self, addr, mnem, opstr):
        self.addr, self.raw = addr, (mnem + " " + opstr).strip()
        self.mnem = mnem.rstrip(".")
        self.rc = mnem.endswith(".")
        self.cls = "alu"
        self.defs, self.uses, self.imms = [], [], ()
        self.mem = self.src = self.width = self.target = self.bo = self.bi = self.spr = None
        self.rel = self.callkey = self.stk = None
        self.copy = False
        self.vdefs = None


def decode(addr, mnem, opstr):
    x = Ins(addr, mnem, opstr)
    m = x.mnem
    cut = opstr.find(" <")
    if cut >= 0:
        opstr = opstr[:cut]
    ops = [o.strip() for o in opstr.split(",") if o.strip()] if opstr.strip() else []

    def reg(i):
        if not isreg(ops[i]):
            raise Unsupported("operand %r of %s" % (ops[i], x.raw))
        return ops[i]

    def rA(i):  # rA=0 means literal zero
        return None if ops[i] == "0" else reg(i)

    def dmem(i):
        mm = MEM_RE.match(ops[i])
        if not mm:
            raise Unsupported("mem operand " + x.raw)
        return pint(mm.group(1)), (None if mm.group(2) == "0" else mm.group(2))

    rc_def = []
    if x.rc:
        if m in ("andi", "andis", "addic") or m in X3 or m in X2 or m in ("rlwinm", "rlwimi", "rlwnm", "srawi"):
            rc_def = ["cr0"]
        else:
            raise Unsupported("record form " + x.raw)
    if m in X3:
        x.defs, x.uses = [reg(0)], [reg(1), reg(2)]
        if m == "or" and ops[1] == ops[2]:
            x.copy = True
            x.uses = [reg(1)]
    elif m in X2:
        x.defs, x.uses = [reg(0)], [reg(1)]
    elif m in IMM:
        x.defs = [reg(0)]
        a = rA(1) if m in ("addi", "addis") else reg(1)
        x.uses = [a] if a else []
        x.imms = (pint(ops[2]),)
        if m in ("andi", "andis", "addic") and mnem.endswith("."):
            rc_def = ["cr0"]
        if m == "ori" and x.imms == (0,):
            x.copy = True
    elif m in ("rlwinm", "rlwnm", "rlwimi"):
        x.defs = [reg(0)]
        if m == "rlwinm":
            x.uses, x.imms = [reg(1)], tuple(pint(o) for o in ops[2:5])
            if x.imms == (0, 0, 31):
                x.copy = True
        elif m == "rlwnm":
            x.uses, x.imms = [reg(1), reg(2)], tuple(pint(o) for o in ops[3:5])
        else:
            x.uses, x.imms = [reg(0), reg(1)], tuple(pint(o) for o in ops[2:5])
    elif m == "srawi":
        x.defs, x.uses, x.imms = [reg(0)], [reg(1)], (pint(ops[2]),)
    elif m in ("cmp", "cmpl", "cmpi", "cmpli"):
        if not ops[0].startswith("cr"):
            raise Unsupported(x.raw)
        x.defs = [ops[0]]
        if m in ("cmp", "cmpl"):
            x.uses, x.imms = [reg(2), reg(3)], (pint(ops[1]),)
        else:
            x.uses, x.imms = [reg(2)], (pint(ops[1]), pint(ops[3]))
    elif m in LOADS or (m.endswith("u") and m[:-1] in LOADS):
        upd = m not in LOADS
        d, b = dmem(1)
        x.cls, x.width = "ld", LOADS[m.rstrip("u")] if not upd else LOADS[m[:-1]]
        x.defs = [reg(0)] + ([b] if upd else [])
        x.uses = [b] if b else []
        x.mem = (b, d, None, upd)
    elif m in STORES or (m.endswith("u") and m[:-1] in STORES):
        upd = m not in STORES
        d, b = dmem(1)
        x.cls, x.width = "st", STORES[m] if not upd else STORES[m[:-1]]
        x.src = reg(0)
        x.uses = [x.src] + ([b] if b else [])
        x.defs = [b] if upd else []
        x.mem = (b, d, None, upd)
    elif m in LOADX or (m.endswith("ux") and m[:-2] + "x" in LOADX):
        upd = m not in LOADX
        a, bb = rA(1), reg(2)
        x.cls, x.width = "ld", LOADX[m] if not upd else LOADX[m[:-2] + "x"]
        x.defs = [reg(0)] + ([a] if upd else [])
        x.uses = ([a] if a else []) + [bb]
        x.mem = (a, 0, bb, upd)
    elif m in STOREX or (m.endswith("ux") and m[:-2] + "x" in STOREX):
        upd = m not in STOREX
        a, bb = rA(1), reg(2)
        x.cls, x.width = "st", STOREX[m] if not upd else STOREX[m[:-2] + "x"]
        x.src = reg(0)
        x.uses = [x.src] + ([a] if a else []) + [bb]
        x.defs = [a] if upd else []
        x.mem = (a, 0, bb, upd)
    elif m in ("psq_l", "psq_lu", "psq_st", "psq_stu", "psq_lx", "psq_lux", "psq_stx", "psq_stux"):
        st = m.startswith("psq_st")
        upd = m.endswith("u") or m.endswith("ux")
        if m.endswith("x"):
            a, bb = rA(1), reg(2)
            x.mem = (a, 0, bb, upd)
            x.imms = (pint(ops[3]), pint(ops[4]))
            areg = ([a] if a else []) + [bb]
        else:
            d, a = dmem(1)
            x.mem = (a, d, None, upd)
            x.imms = (pint(ops[2]), pint(ops[3]))
            areg = [a] if a else []
        x.width = (4 if x.imms[0] else 8) if x.imms[1] == 0 else None
        if st:
            x.cls, x.src = "st", reg(0)
            x.uses = [x.src] + areg
            x.defs = [a] if upd else []
        else:
            x.cls = "ld"
            x.defs = [reg(0)] + ([a] if upd else [])
            x.uses = areg
    elif m in ("lmw", "stmw"):
        d, b = dmem(1)
        r0 = int(reg(0)[1:])
        regs = GPR[r0:]
        x.mem, x.width = (b, d, None, False), 4 * len(regs)
        if m == "lmw":
            x.cls, x.defs, x.uses = "ld", regs, [b]
        else:
            x.cls, x.src, x.uses, x.defs = "st", reg(0), regs + [b], []
    elif m in FP3 or m in FP4 or m in FP2:
        x.defs, x.uses = [reg(0)], [reg(i) for i in range(1, 2 if m in FP2 else len(ops))]
        if m == "fmr":
            x.copy = True
        if x.rc:
            raise Unsupported("fp record form " + x.raw)
    elif m in ("fcmpu", "fcmpo"):
        x.defs, x.uses = [ops[0]], [reg(1), reg(2)]
    elif m.startswith("ps_"):
        if x.rc:
            raise Unsupported(x.raw)
        if m in PS_CMP:
            x.defs, x.uses = [ops[0]], [reg(1), reg(2)]
        else:
            x.defs, x.uses = [reg(0)], [reg(i) for i in range(1, len(ops))]
            if m == "ps_mr":
                x.copy = True
    elif m in CROPS:
        d, a, b = crb(ops[0]), crb(ops[1]), crb(ops[2])
        x.defs = ["cr%d" % (d // 4)]
        x.uses = ["cr%d" % (d // 4), "cr%d" % (a // 4), "cr%d" % (b // 4)]
        x.imms = (d % 4, a % 4, b % 4)
    elif m == "mcrf":
        x.defs, x.uses, x.copy = [ops[0]], [ops[1]], True
    elif m == "mfcr":
        x.defs, x.uses = [reg(0)], list(CRF)
    elif m == "mtcrf":
        fxm = pint(ops[0])
        x.defs = ["cr%d" % i for i in range(8) if fxm >> (7 - i) & 1]
        x.uses, x.imms = [reg(1)], (fxm,)
    elif m == "mfspr":
        x.spr = pint(ops[1])
        x.defs = [reg(0)]
        if x.spr == 8:
            x.uses, x.copy = ["lr"], True
        elif x.spr == 9:
            x.uses, x.copy = ["ctr"], True
        elif x.spr == 1:
            x.uses = ["ca"]
        else:
            x.cls, x.imms = "ev", (x.spr,)
    elif m == "mtspr":
        x.spr = pint(ops[0])
        x.uses = [reg(1)]
        if x.spr == 8:
            x.defs, x.copy = ["lr"], True
        elif x.spr == 9:
            x.defs, x.copy = ["ctr"], True
        elif x.spr == 1:
            x.defs = ["ca"]
        else:
            x.cls, x.imms = "ev", (x.spr,)
    elif m in SIDE:
        x.cls = "ev"
        regs, imms = [], []
        for o in ops:
            if isreg(o):
                regs.append(o)
            elif o == "0":
                imms.append(0)
            else:
                try:
                    imms.append(pint(o))
                except ValueError:
                    raise Unsupported(x.raw)
        x.imms = tuple(imms)
        if m in ("mftb", "mfmsr", "mfsr", "mffs", "mfsrin"):
            x.defs, x.uses = regs[:1], regs[1:]
        else:
            x.uses = regs
    elif m in ("b", "bl"):
        x.cls = m
        x.target = int(ops[0], 16)
        if m == "bl":
            x.defs = list(VOLATILE) + ["lr"]
    elif m in ("bc", "bcl"):
        if m == "bcl":
            raise Unsupported(x.raw)
        x.cls, x.bo, x.bi, x.target = "bc", pint(ops[0]), crb(ops[1]), int(ops[2], 16)
    elif m in ("bclr", "bclrl", "bcctr", "bcctrl"):
        x.bo, x.bi = pint(ops[0]), crb(ops[1])
        if m in ("bclrl", "bcctrl"):
            if x.bo != 20:
                raise Unsupported("conditional indirect call " + x.raw)
            x.cls = "icall"
            x.uses = ["lr" if m == "bclrl" else "ctr"]
            x.defs = list(VOLATILE) + ["lr"]
        else:
            x.cls = "ret" if m == "bclr" else "bctr"
    else:
        raise Unsupported("instruction " + x.raw)
    if m in CA_OUT:
        x.defs = x.defs + ["ca"]
    if m in CA_IN:
        x.uses = x.uses + ["ca"]
    x.vdefs = len(x.defs)
    x.defs = x.defs + rc_def
    return x


RELOC_KIND = {4: "lo", 5: "hi", 6: "ha", 3: "a16", 109: "sda"}


class Func:
    pass


def load_func(obj, name, mutate=None):
    """Decode a function; attach relocation keys; build the CFG."""
    s = obj.funcs.get(name)
    if s is None:
        raise Unsupported("symbol missing")
    sec, start, size = s["shndx"], s["value"], s["size"]
    txt = obj.text[sec]
    rctx = RelocCtx(obj, sec, start, start + size)
    rtab = obj.elf.relocs.get(sec, {})
    raw = []
    for a in range(start, start + size, 4):
        if a not in txt:
            raise Unsupported("no disassembly at 0x%x" % a)
        raw.append((a, txt[a][0], txt[a][1]))
    if mutate:
        rel = {a for a in range(start, start + size, 4) if any(o in rtab for o in (a, a + 1, a + 2, a + 3))}
        raw = mutate(raw, rel)
    fn = Func()
    fn.name, fn.obj, fn.start, fn.size, fn.sec, fn.rctx = name, obj, start, size, sec, rctx
    fn.ins = []
    for i, (a, mn, op) in enumerate(raw):
        x = decode(a, mn, op)
        x.idx = i
        r = None
        for o in (a, a + 1, a + 2, a + 3):
            if o in rtab:
                r = rtab[o]
                break
        if r is not None:
            attach_reloc(x, r, rctx)
        fn.ins.append(x)
    build_cfg(fn)
    return fn


def attach_reloc(x, r, rctx):
    typ, symidx, add = r
    key, addend, cls = rctx.key(symidx, add)
    x.rel = (typ, symidx, add)
    if typ == 10:  # REL24
        if x.cls not in ("b", "bl"):
            raise Unsupported("rel24 on " + x.raw)
        x.callkey = key
        return
    kind = RELOC_KIND.get(typ)
    if kind is None:
        raise Unsupported("relocation type %d" % typ)
    val = (kind, key, addend)
    rctx.ro[(key, addend)] = cls
    rctx.ro[key] = cls
    if x.mem is not None and x.mem[2] is None:
        x.mem = (x.mem[0], val, None, x.mem[3])
    elif x.mnem in IMM:
        x.imms = (val,)
        x.copy = False
    else:
        raise Unsupported("relocation on " + x.raw)


class Block:
    __slots__ = ("start", "end", "succs", "kind", "term")


def build_cfg(fn):
    ins = fn.ins
    n = len(ins)
    leaders = {0}
    fn.jt = {}
    for i, x in enumerate(ins):
        if x.cls in ("b", "bc", "ret", "bctr"):
            if i + 1 < n:
                leaders.add(i + 1)
        if x.cls in ("b", "bc") and x.callkey is None:
            t = x.target - fn.start
            if t < 0 or t >= fn.size or t % 4:
                raise Unsupported("branch outside function " + x.raw)
            leaders.add(t // 4)
        if x.cls == "b" and x.callkey is not None:
            if i + 1 < n:
                leaders.add(i + 1)
        if x.cls == "bl" and x.callkey is None:
            t = x.target - fn.start
            if 0 <= t < fn.size:
                raise Unsupported("local call " + x.raw)
            x.callkey = resolve_addr_call(fn, x.target)
        if x.cls == "bctr":
            fn.jt[i] = jump_table(fn, i)
            leaders.update(fn.jt[i])
    order = sorted(leaders)
    idx_of = {s: k for k, s in enumerate(order)}
    fn.blocks = []
    for k, s in enumerate(order):
        b = Block()
        b.start, b.end = s, (order[k + 1] if k + 1 < len(order) else n)
        last = ins[b.end - 1]
        nxt = [idx_of[b.end]] if b.end < n else []
        if last.cls == "b" and last.callkey is None:
            b.kind, b.succs = "b", [idx_of[(last.target - fn.start) // 4]]
        elif last.cls == "b":
            b.kind, b.succs = "tail", []
        elif last.cls == "bc":
            t = idx_of[(last.target - fn.start) // 4]
            if last.bo & 0x14 == 0x14:
                b.kind, b.succs = "b", [t]
            else:
                if not nxt:
                    raise Unsupported("conditional branch falls off the end")
                b.kind, b.succs = "bc", [t] + nxt
        elif last.cls == "ret":
            if last.bo & 0x14 == 0x14:
                b.kind, b.succs = "ret", []
            else:
                if not nxt:
                    raise Unsupported("conditional return falls off the end")
                b.kind, b.succs = "retc", nxt
        elif last.cls == "bctr":
            jt = fn.jt[b.end - 1]
            b.kind, b.succs = ("jt", [idx_of[t] for t in jt]) if jt else ("itail", [])
        else:
            b.kind, b.succs = "fall", nxt
            if not nxt:
                raise Unsupported("falls off the end of the function")
        fn.blocks.append(b)


def resolve_addr_call(fn, target):
    for s in fn.obj.bysec.get(fn.sec, ()):
        if s["type"] == 2 and s["value"] <= target < s["value"] + s["size"]:
            if s["value"] != target:
                break
            return ("sym", s["name"])
    raise Unsupported("unresolved call target 0x%x" % target)


def jump_table(fn, i):
    """Successor instruction indices of the bctr at i (MWCC switch table), or [] for an indirect jump."""
    ins = fn.ins
    j = i - 1
    src = None
    while j >= 0 and j >= i - 8:
        x = ins[j]
        if x.mnem == "mtspr" and x.spr == 9:
            src = x.uses[0]
            break
        j -= 1
    if src is None:
        return []
    base = None
    for k in range(j - 1, max(-1, j - 12), -1):
        x = ins[k]
        if x.mnem == "lwzx" and x.defs[0] == src:
            base = x.mem[0]
            break
    if base is None:
        return []
    for k in range(k - 1, -1, -1):
        x = ins[k]
        if base in x.defs:
            if x.mnem == "addi" and x.rel is not None and x.rel[0] == 4:
                typ, symidx, add = x.rel
                s = fn.obj.elf.syms[symidx]
                sec = s["shndx"]
                off = s["value"] + add
                rtab = fn.obj.elf.relocs.get(sec, {})
                cov = fn.obj.covering(sec, off)
                end = cov["value"] + cov["size"] if cov is not None and cov["size"] else 1 << 30
                out = []
                while off < end and off in rtab:
                    t, si, a2 = rtab[off]
                    ts = fn.obj.elf.syms[si]
                    addr = ts["value"] + a2
                    if ts["shndx"] != fn.sec or not (fn.start <= addr < fn.start + fn.size) or (addr - fn.start) % 4:
                        break
                    out.append((addr - fn.start) // 4)
                    off += 4
                if out:
                    return out
            raise Unsupported("unrecognised jump table")
    raise Unsupported("unrecognised jump table")


# ----------------------------------------------------------------------------
# Callee summaries: argument registers read and volatile registers passed through


def fn_summary_ops(fn):
    """Per block: list of ('du', def, use) | ('call', name) | ('icall',) plus terminator info."""
    out = []
    for b in fn.blocks:
        ops = []
        for i in range(b.start, b.end):
            x = fn.ins[i]
            if x.cls == "bl":
                ops.append(("call", x.callkey[1] if x.callkey[0] == "sym" else None))
            elif x.cls == "icall":
                ops.append(("du", 0, mask(x.uses)))
                ops.append(("icall",))
            elif x.cls in ("b", "ret"):
                ops.append(("du", 0, mask(["cr%d" % (x.bi // 4)]) if x.bo is not None and not x.bo & 16 else 0))
            elif x.cls == "bc":
                u = 0
                if not x.bo & 16:
                    u |= mask(["cr%d" % (x.bi // 4)])
                if not x.bo & 4:
                    u |= mask(["ctr"])
                ops.append(("du", mask(["ctr"]) if not x.bo & 4 else 0, u))
            elif x.cls == "bctr":
                ops.append(("du", 0, mask(["ctr"]) | (mask(["cr%d" % (x.bi // 4)]) if not x.bo & 16 else 0)))
            else:
                ops.append(("du", mask(x.defs[:x.vdefs] + x.defs[x.vdefs:]), mask(x.uses)))
        term = b.kind
        tail = None
        if term == "tail":
            last = fn.ins[b.end - 1]
            tail = last.callkey[1] if last.callkey and last.callkey[0] == "sym" else None
        out.append((ops, term, tail, b.succs))
    return out



INF = 1 << 30
ARGREGS = GPR[3:11]
PX_WHY = None


def px_ops(fn, skip=frozenset(), esc_src=None):
    """Compact per-block ops for the pointer-extent analysis."""
    esc_src = esc_src or {}
    spills = not esc_src and not any(
        ("r1" in x.uses and not (x.cls in ("ld", "st") and x.mem[0] == "r1" and x.src != "r1" and x.mem[2] != "r1"))
        for x in fn.ins if x.mnem not in ("stwu", "addi") or x.defs != ["r1"])
    out = []
    for b in fn.blocks:
        ops = []
        for i in range(b.start, b.end):
            if i in skip:
                continue
            x = fn.ins[i]
            c = x.cls
            if i in esc_src:
                ops.append(("src", x.defs[0], esc_src[i]))
            elif c == "alu":
                if x.copy:
                    ops.append(("cp", x.defs[0], x.uses[0]))
                elif x.mnem == "addi" and x.uses and isinstance(x.imms[0], int):
                    ops.append(("addi", x.defs[0], x.uses[0], x.imms[0]))
                elif x.mnem in ("cmp", "cmpl", "cmpi", "cmpli"):
                    ops.append(("clr", tuple(x.defs), ()))
                else:
                    ops.append(("clr", tuple(x.defs), tuple(x.uses)))
            elif spills and c in ("ld", "st") and x.mem[0] == "r1" and x.mnem in ("lwz", "stw") \
                    and isinstance(x.mem[1], int) and not x.mem[3]:
                ops.append(("fill", x.defs[0], x.mem[1]) if c == "ld" else ("spill", x.src, x.mem[1]))
            elif c in ("ld", "st"):
                base, disp, index, upd = x.mem
                stored = (tuple(GPR[int(x.src[1:]):]) if x.mnem == "stmw" else (x.src,)) if c == "st" else ()
                defs = tuple(x.defs[:1] if upd else x.defs[:x.vdefs]) if c == "ld" else ()
                ops.append(("mem", base, disp if isinstance(disp, int) and index is None else None, index,
                            x.width or 0, upd, stored, defs))
            elif c == "bl":
                ops.append(("call", x.callkey[1] if x.callkey and x.callkey[0] == "sym" else None))
            elif c == "icall":
                ops.append(("icall", x.uses[0]))
            elif c == "ev":
                ops.append(("clr", tuple(x.defs), tuple(x.uses)))
            elif c == "ret":
                ops.append(("ret",))
            elif c == "bc" and not x.bo & 4:
                ops.append(("clr", ("ctr",), ()))
        out.append((ops, list(b.succs)))
    return out


def _pjoin(a, b, widen):
    out = dict(a)
    changed = False
    for r, vb in b.items():
        va = a.get(r)
        if not va:
            out[r] = dict(vb)
            changed = True
            continue
        m = dict(va)
        for k, rb in vb.items():
            ra = m.get(k, "missing")
            if ra == "missing":
                m[k] = rb
            elif ra is None or rb is None:
                m[k] = None
            else:
                m[k] = (min(ra[0], rb[0]), max(ra[1], rb[1]))
        if m != va:
            if widen:
                m = {k: (v if va.get(k, "missing") == v else None) for k, v in m.items()}
            out[r] = m
            changed = True
    return out, changed


def px_run(opsl, entry, callees, retinfo=None, lo_ext=None):
    """Max bytes accessed through each tracked pointer (INF = unknown/escapes)."""
    ext = {}

    def poison(v, why=None):
        if v:
            for k in v:
                ext[k] = INF
                if PX_WHY is not None:
                    PX_WHY.append((k, why or op))

    def shift(v, d):
        return {k: (None if r is None or abs(r[0] + d) > 65536 else (r[0] + d, r[1] + d)) for k, r in v.items()}
    nb = len(opsl)
    sin = [None] * nb
    sin[0] = dict(entry)
    visits = [0] * nb
    work = [0]
    while work:
        b = work.pop()
        st = dict(sin[b])
        for op in opsl[b][0]:
            t = op[0]
            if t == "cp":
                v = st.get(op[2])
                if v:
                    st[op[1]] = v
                else:
                    st.pop(op[1], None)
            elif t == "addi":
                v = st.get(op[2])
                if v:
                    st[op[1]] = shift(v, op[3])
                else:
                    st.pop(op[1], None)
            elif t == "src":
                st[op[1]] = {op[2]: (0, 0)}
            elif t == "spill":
                v = st.get(op[1])
                for kk in [kk for kk in st if isinstance(kk, tuple) and abs(kk[1] - op[2]) < 4]:
                    del st[kk]
                if v:
                    st[("stk", op[2])] = v
            elif t == "fill":
                v = st.get(("stk", op[2]))
                if v:
                    st[op[1]] = v
                else:
                    st.pop(op[1], None)
            elif t == "clr":
                for u in op[2]:
                    poison(st.get(u))
                for d in op[1]:
                    st.pop(d, None)
            elif t == "mem":
                _, base, disp, index, w, upd, stored, defs = op
                for r in stored:
                    poison(st.get(r))
                vb = st.get(base) if base else None
                if index:
                    poison(st.get(index))
                if vb:
                    if disp is None:
                        poison(vb)
                    else:
                        for k, r in vb.items():
                            if r is None or (r[0] + disp < 0 and lo_ext is None):
                                poison({k: 0}, ("neg/unknown offset",) + op)
                            elif r[0] + disp < 0:
                                lo_ext[k] = min(lo_ext.get(k, 0), r[0] + disp)
                            elif ext.get(k, 0) < r[1] + disp + w:
                                ext[k] = min(INF, r[1] + disp + w)
                for d in defs:
                    st.pop(d, None)
                if upd and base:
                    if vb and disp is not None:
                        st[base] = shift(vb, disp)
                    else:
                        st.pop(base, None)
            elif t in ("call", "icall"):
                known = t == "call" and op[1] in callees
                if known:
                    au, pt, px, pr = callees[op[1]][:4]
                else:
                    au, pt, px, pr = INDMASK, VOLMASK, {}, None
                    if t == "icall":
                        poison(st.get(op[1]))
                pre = {r: st.get(r) for r in ARGREGS}
                passed = {}
                for r in ARGREGS:
                    v = st.get(r)
                    if v and au >> REGIDX[r] & 1:
                        e = px.get(r, INF)
                        for k, rng in v.items():
                            if rng is None or (rng[0] < 0 and lo_ext is None) or e >= INF:
                                poison({k: 0}, (op[0], op[1], r, "px=%s" % e, rng))
                            elif rng[0] < 0:
                                lo_ext[k] = min(lo_ext.get(k, 0), rng[0])
                            elif ext.get(k, 0) < rng[1] + e:
                                ext[k] = min(INF, rng[1] + e)
                            passed[k] = None
                for r in VOLATILE:
                    if not pt >> REGIDX[r] & 1:
                        st.pop(r, None)
                if known:
                    rv = dict(st.get("r3") or {})
                    for r, R in (pr or {}).items():
                        for k, rng in (pre.get(r) or {}).items():
                            if rng is None or R is None or rv.get(k, 0) is None:
                                rv[k] = None
                            elif k in rv:
                                rv[k] = (min(rv[k][0], rng[0] + R[0]), max(rv[k][1], rng[1] + R[1]))
                            else:
                                rv[k] = (rng[0] + R[0], rng[1] + R[1])
                    if rv:
                        st["r3"] = rv
                elif passed:
                    for r in ("r3", "r4"):
                        v = dict(st.get(r) or {})
                        v.update(passed)
                        st[r] = v
            elif t == "ret" and retinfo is not None:
                for k, rng in (st.get("r3") or {}).items():
                    old = retinfo.get(k, "missing")
                    if old == "missing":
                        retinfo[k] = rng
                    elif old is None or rng is None:
                        retinfo[k] = None
                    else:
                        retinfo[k] = (min(old[0], rng[0]), max(old[1], rng[1]))
        for sc in opsl[b][1]:
            if sin[sc] is None:
                sin[sc] = dict(st)
                work.append(sc)
            else:
                m, ch = _pjoin(sin[sc], st, visits[sc] > 8)
                if ch:
                    sin[sc] = m
                    visits[sc] += 1
                    if visits[sc] > 40:
                        for v in m.values():
                            poison(v)
                        continue
                    work.append(sc)
    return ext


def stack_arg_reads(fn):
    """Bytes of the caller's outgoing-argument area (entry SP + 8 ...) this function may access."""
    N, k0, out = 0, None, 0
    for i in range(fn.blocks[0].start, fn.blocks[0].end):
        x = fn.ins[i]
        if x.mnem == "stwu" and x.mem[0] == "r1" and isinstance(x.mem[1], int):
            N, k0 = -x.mem[1], i
            break
    for x in fn.ins:
        kofs = N if (k0 is not None and x.idx > k0) else 0
        if x.cls in ("ld", "st") and x.mem is not None and x.mem[0] == "r1" and x.mnem not in ("stwu",):
            if not isinstance(x.mem[1], int) or x.mem[2] is not None:
                return INF
            A = x.mem[1] - kofs
            if A + (x.width or 8) > 8 and A < 8 + 4096:
                if A >= 8:
                    out = max(out, A + (x.width or 8) - 8)
        elif x.cls == "alu" and "r1" in x.uses and x.mnem != "addi":
            if not (x.copy or x.mnem in ("cmp", "cmpl", "cmpi", "cmpli")):
                return INF
            if x.copy:
                return INF
        elif x.mnem == "addi" and x.uses == ["r1"] and x.defs != ["r1"]:
            if not isinstance(x.imms[0], int) or x.imms[0] - kofs >= 8:
                return INF
    return out


def compute_callee_summaries(verbose=False):
    units = json.load(open(os.path.join(BUILD, "report.json")))["units"]
    progs = {}
    pxops = {}
    stk = {}
    taken = set()
    nfail = 0
    for u in units:
        path = os.path.join(BUILD, "obj", u["name"].split("/", 1)[1] + ".o")
        if not os.path.exists(path):
            continue
        obj = Obj(path)
        for name in obj.funcs:
            try:
                fn = load_func(obj, name)
                progs.setdefault(name, []).append(fn_summary_ops(fn))
                pxops.setdefault(name, []).append(px_ops(fn))
                stk[name] = max(stk.get(name, 0), stack_arg_reads(fn))
            except Unsupported:
                progs.setdefault(name, []).append(None)
                pxops.setdefault(name, []).append(None)
                stk[name] = INF
                nfail += 1
        # functions whose address is taken (vtables, callbacks): callers unknown
        starts = collections.defaultdict(list)
        for f in obj.funcs.values():
            starts[(f["shndx"], f["value"])].append(f["name"])
        for sec, tab in obj.elf.relocs.items():
            for off, (typ, si, add) in tab.items():
                if typ == 10:
                    continue
                sy = obj.elf.syms[si]
                if sy["shndx"] in obj.text:
                    tgt = sy["value"] + add
                    for f in starts.get((sy["shndx"], tgt), ()):
                        taken.add(f)
                elif add == 0:
                    taken.add(sy["name"])
    callers = collections.defaultdict(set)
    for name, lst in progs.items():
        for p in lst:
            if p is None:
                continue
            for ops, term, tail, _s in p:
                for op in ops:
                    if op[0] == "call" and op[1]:
                        callers[op[1]].add(name)
                if tail:
                    callers[tail].add(name)
    AU = {n: 0 for n in progs}
    PT = {n: 0 for n in progs}

    def au_of(n):
        return AU[n] if n in AU else INDMASK | mask(["r11", "r12"])

    def pt_of(n):
        return PT[n] if n in PT else VOLMASK

    def analyse(p):
        if p is None:
            return VOLMASK, VOLMASK
        nb = len(p)
        live = [0] * nb
        changed = True
        while changed:
            changed = False
            for b in range(nb - 1, -1, -1):
                ops, term, tail, succs = p[b]
                v = 0
                for s in succs:
                    v |= live[s]
                if term == "tail":
                    v |= au_of(tail) if tail else INDMASK
                elif term == "itail":
                    v |= INDMASK | mask(["r11", "r12"])
                for op in reversed(ops):
                    if op[0] == "du":
                        v = (v & ~op[1]) | op[2]
                    elif op[0] == "call":
                        v = (v & (pt_of(op[1]) | ~VOLMASK)) | au_of(op[1])
                    else:
                        v = v | INDMASK
                if v != live[b]:
                    live[b] = v
                    changed = True
        au = live[0] & VOLMASK
        und = [None] * nb
        und[0] = VOLMASK
        pt = 0
        changed = True
        while changed:
            changed = False
            for b in range(nb):
                if und[b] is None:
                    continue
                ops, term, tail, succs = p[b]
                v = und[b]
                for op in ops:
                    if op[0] == "du":
                        v &= ~op[1]
                    elif op[0] == "call":
                        v &= pt_of(op[1])
                if term in ("ret", "retc"):
                    pt |= v
                elif term == "tail":
                    pt |= v & (pt_of(tail) if tail else VOLMASK)
                elif term == "itail":
                    pt |= v
                for s in succs:
                    nv = (und[s] or 0) | v
                    if und[s] is None or nv != und[s]:
                        und[s] = nv
                        changed = True
        return au, pt

    work = collections.deque(progs)
    inq = set(progs)
    it = 0
    while work:
        n = work.popleft()
        inq.discard(n)
        it += 1
        au = pt = 0
        for p in progs[n]:
            a, t = analyse(p)
            au |= a
            pt |= t
        if au != AU[n] or pt != PT[n]:
            AU[n] |= au
            PT[n] |= pt
            for c in callers.get(n, ()):
                if c not in inq:
                    inq.add(c)
                    work.append(c)
    # pointer extents: bytes accessed through each argument register (least fixpoint, widened)
    PXd = {n: {r: 0 for r in ARGREGS} for n in progs}
    PRd = {n: {} for n in progs}
    changes = collections.Counter()
    view = {}

    class View(dict):
        def __missing__(self, n):
            raise KeyError(n)
    for n in progs:
        view[n] = (AU[n], PT[n], PXd[n], PRd[n])
    work = collections.deque(progs)
    inq = set(progs)
    it2 = 0
    while work:
        n = work.popleft()
        inq.discard(n)
        it2 += 1
        new = {r: 0 for r in ARGREGS}
        newr = {}
        for ops in pxops[n]:
            if ops is None:
                new = {r: INF for r in ARGREGS}
                newr = {r: None for r in ARGREGS}
                break
            ri = {}
            e = px_run(ops, {r: {r: (0, 0)} for r in ARGREGS}, view, ri)
            for r in ARGREGS:
                new[r] = max(new[r], e.get(r, 0))
            for k, rng in ri.items():
                old = newr.get(k, "missing")
                newr[k] = rng if old == "missing" else None if old is None or rng is None else (min(old[0], rng[0]), max(old[1], rng[1]))
        for k, rng in PRd[n].items():
            old = newr.get(k, "missing")
            newr[k] = rng if old == "missing" else None if old is None or rng is None else (min(old[0], rng[0]), max(old[1], rng[1]))
        if new != PXd[n] or newr != PRd[n]:
            changes[n] += 1
            if changes[n] > 6:
                new = {r: (INF if new[r] != PXd[n][r] else new[r]) for r in ARGREGS}
                newr = {k: None for k in newr}
            PXd[n] = {r: max(new[r], PXd[n][r]) for r in ARGREGS}
            PRd[n] = newr
            view[n] = (AU[n], PT[n], PXd[n], PRd[n])
            for c in callers.get(n, ()):
                if c not in inq:
                    inq.add(c)
                    work.append(c)
    # return registers read by any caller (address-taken: unknown callers -> all)
    RETM = mask(RET_REGS)
    RU = {n: (RETM if n in taken else 0) for n in progs}

    def ru_pass(p, exit_live, rec):
        nb = len(p)
        live = [0] * nb
        for final in (False, True):
            changed = True
            while changed or final:
                changed = False
                for b in range(nb - 1, -1, -1):
                    ops, term, tail, succs = p[b]
                    v = 0
                    for sc in succs:
                        v |= live[sc]
                    if term in ("ret", "retc"):
                        v |= exit_live
                    elif term == "tail":
                        if final and tail:
                            rec[tail] = rec.get(tail, 0) | exit_live
                        v |= au_of(tail) if tail else INDMASK
                    elif term == "itail":
                        v |= INDMASK | mask(["r11", "r12"])
                    for op in reversed(ops):
                        if op[0] == "du":
                            v = (v & ~op[1]) | op[2]
                        elif op[0] == "call":
                            if final and op[1]:
                                rec[op[1]] = rec.get(op[1], 0) | (v & RETM)
                            v = (v & (pt_of(op[1]) | ~VOLMASK)) | au_of(op[1])
                        else:
                            v = v | INDMASK
                    if v != live[b]:
                        live[b] = v
                        changed = True
                if final:
                    break
    work = collections.deque(progs)
    inq = set(progs)
    while work:
        n = work.popleft()
        inq.discard(n)
        rec = {}
        for p in progs[n]:
            if p is not None:
                ru_pass(p, RU[n], rec)
        for g, m in rec.items():
            if g in RU and RU[g] | m != RU[g]:
                RU[g] |= m
                if g not in inq:
                    inq.add(g)
                    work.append(g)
    stk_ind = max([stk.get(n, INF) for n in taken if n in progs] + [0])
    if verbose:
        big = sorted((stk.get(n, INF), n) for n in taken if n in progs and stk.get(n, INF) > 0)[-12:]
        print("largest stack-argument readers among address-taken functions:", big, file=sys.stderr)
    if verbose:
        print("callee summaries: %d functions, %d undecodable, %d+%d analyses; %d address-taken, "
              "indirect callees read %s bytes of stack arguments" % (len(progs), nfail, it, it2, len(taken & set(progs)),
                                                                     stk_ind if stk_ind < INF else "INF"), file=sys.stderr)
    out = {n: [AU[n], PT[n], PXd[n], PRd[n], stk.get(n, INF), RU[n]] for n in progs}
    out["<indirect>"] = [INDMASK, VOLMASK, {}, {}, stk_ind, RETM]
    return out


def load_callee_tuples(verbose=False):
    return {k: (v[0], v[1], v[2], {r: (tuple(x) if x is not None else None) for r, x in v[3].items()}, v[4], v[5])
            for k, v in load_callees(verbose).items()}


def load_callees(verbose=False):
    if os.path.exists(CALLEE_CACHE):
        try:
            d = json.load(open(CALLEE_CACHE))
            if d.get("regs") == ALLREGS and d.get("version") == 5:
                return d["funcs"]
        except (ValueError, KeyError):
            pass
    funcs = compute_callee_summaries(verbose)
    with open(CALLEE_CACHE, "w") as fh:
        json.dump({"regs": ALLREGS, "version": 5, "funcs": funcs}, fh)
    return funcs


# ----------------------------------------------------------------------------
# Frame (prologue / epilogue) and stack slots


def prepare(fn):
    """Strip prologue/epilogue, verify it, and collect r1-relative accesses."""
    ins, blocks = fn.ins, fn.blocks
    strip = set()
    e0 = blocks[0]
    N, stwu_i, lr_i, lr_reg, lr_saved = 0, None, None, None, False
    defined = set()
    saves = collections.defaultdict(set)   # reg -> {(A, mnem-kind)}
    for i in range(e0.start, e0.end):
        x = ins[i]
        kofs = -N if stwu_i is not None else 0
        if x.mnem == "stwu" and x.mem[0] == "r1" and x.src == "r1" and stwu_i is None and isinstance(x.mem[1], int):
            N, stwu_i = -x.mem[1], i
            strip.add(i)
            continue
        if x.mnem == "mfspr" and x.spr == 8 and lr_i is None:
            lr_i, lr_reg = i, x.defs[0]
            strip.add(i)
            continue
        if lr_i is not None and not lr_saved and x.mnem == "stw" and x.src == lr_reg and x.mem[0] == "r1" \
                and lr_reg not in defined and isinstance(x.mem[1], int) and x.mem[1] + kofs == 4:
            strip.add(i)
            lr_saved = True
            continue
        if x.mnem in ("stw", "stfd", "psq_st", "stmw") and x.mem[0] == "r1" and isinstance(x.mem[1], int) \
                and not x.mem[3] and x.mem[2] is None:
            regs = GPR[int(x.src[1:]):] if x.mnem == "stmw" else [x.src]
            if all(r in CALLEE_SAVED and r not in defined for r in regs):
                A = x.mem[1] + kofs
                for j, r in enumerate(regs):
                    saves[r].add((A + 4 * j if x.mnem == "stmw" else A, "w" if x.mnem in ("stw", "stmw") else x.mnem + repr(x.imms)))
                strip.add(i)
                continue
        defined.update(x.defs)
    if lr_i is not None and not lr_saved:
        strip.discard(lr_i)
    fn.N, fn.saves, fn.lr_saved = N, dict(saves), lr_saved
    restore_kind = {"lwz": "w", "lmw": "w", "lfd": "stfd()", "psq_l": "psq_st"}
    for b in blocks:
        if b.kind in ("retc", "tail", "itail") and (N or saves or lr_saved):
            raise Unsupported("frame: early exit from a framed function")
        if b.kind != "ret":
            continue
        used_later, got_addi, lr_rr, lr_loaded, restored = set(), False, None, False, collections.defaultdict(set)
        for i in range(b.end - 2, b.start - 1, -1):
            x = ins[i]
            if N and not got_addi and x.mnem == "addi" and x.defs == ["r1"] and x.uses == ["r1"] and x.imms == (N,):
                strip.add(i)
                got_addi = True
                continue
            if lr_saved and lr_rr is None and x.mnem == "mtspr" and x.spr == 8:
                lr_rr = x.uses[0]
                strip.add(i)
                continue
            if lr_rr and not lr_loaded and x.mnem == "lwz" and x.defs[0] == lr_rr and x.mem[0] == "r1" \
                    and isinstance(x.mem[1], int) and x.mem[1] - N == 4 and lr_rr not in used_later:
                strip.add(i)
                lr_loaded = True
                continue
            if x.cls == "ld" and x.mnem in ("lwz", "lfd", "psq_l", "lmw") and x.mem[0] == "r1" \
                    and isinstance(x.mem[1], int) and not x.mem[3] and x.mem[2] is None:
                regs = x.defs[:x.vdefs]
                A = x.mem[1] - N
                kind = restore_kind[x.mnem]
                if kind == "psq_st":
                    kind = "psq_st" + repr(x.imms)
                ok = True
                for j, r in enumerate(regs):
                    slot = (A + 4 * j if x.mnem == "lmw" else A, kind)
                    if r not in saves or slot not in saves[r] or r in used_later:
                        ok = False
                if ok:
                    for j, r in enumerate(regs):
                        restored[r].add((A + 4 * j if x.mnem == "lmw" else A, kind))
                    strip.add(i)
                    continue
            used_later.update(x.uses)
        if N and not got_addi:
            raise Unsupported("frame: missing frame pop")
        if lr_saved and not lr_loaded:
            raise Unsupported("frame: LR not restored")
        for r, sl in saves.items():
            if restored.get(r) != sl:
                raise Unsupported("frame: %s not restored" % r)
    fn.strip = strip
    # verify body
    for x in ins:
        if x.idx in strip:
            continue
        for d in x.defs:
            if d in CALLEE_SAVED and d not in saves:
                raise Unsupported("frame: clobbers unsaved %s" % d)
            if d == "r1":
                raise Unsupported("frame: r1 written in body")
    save_iv = [(A, A + (8 if k != "w" else 4)) for sl in saves.values() for A, k in sl]
    if lr_saved:
        save_iv.append((4, 8))
    # stack accesses
    fn.acc = [[] for _ in blocks]
    fn.esc = False
    for bi, b in enumerate(blocks):
        for i in range(b.start, b.end):
            if i in strip:
                continue
            x = ins[i]
            touches = ("r1" in x.uses) or (x.mem is not None and x.mem[0] == "r1")
            if not touches:
                continue
            if bi == 0 and stwu_i is not None and i < stwu_i:
                raise Unsupported("stack access before frame push")
            kofs = -N
            if x.cls in ("ld", "st") and x.mem[0] == "r1":
                if x.mem[2] is not None or x.mem[3] or not isinstance(x.mem[1], int) or x.mnem in ("lmw", "stmw") \
                        or x.width is None or x.src == "r1":
                    raise Unsupported("stack access form " + x.raw)
                A = x.mem[1] + kofs
                for lo, hi in save_iv:
                    if A < hi and lo < A + x.width:
                        raise Unsupported("body touches register save area")
                fn.acc[bi].append([x.cls, x.mnem, A, x.width, x])
            elif x.mnem == "addi" and x.uses == ["r1"] and isinstance(x.imms[0], int):
                fn.acc[bi].append(["esc", "addi", x.imms[0] + kofs, 0, x])
                fn.esc = True
            elif x.copy and x.uses == ["r1"]:
                fn.acc[bi].append(["esc", "addi", kofs, 0, x])
                fn.esc = True
            else:
                raise Unsupported("r1 used as a value: " + x.raw)
    return fn


def merge_iv(iv):
    out = []
    for lo, hi in sorted(iv):
        if out and lo < out[-1][1]:
            out[-1][1] = max(out[-1][1], hi)
        else:
            out.append([lo, hi])
    return out


def find_iv(ivs, lo, hi=None):
    hi = lo + 1 if hi is None else hi
    for k, (a, b) in enumerate(ivs):
        if lo < b and a < hi:
            return k
    return None


def escape_extents(F, callees):
    src = {a[4].idx: a[2] for accs in F.acc for a in accs if a[0] == "esc"}
    if not src:
        return {}, {}
    lo = {}
    hi = px_run(px_ops(F, F.strip, src), {}, callees, None, lo)
    return hi, lo


def clusters(F, bases):
    """Grow [base, base+extent) intervals over overlapping direct accesses until stable."""
    ivs = merge_iv([[lo, hi] for lo, hi in bases])
    direct = [(a[2], a[2] + a[3]) for accs in F.acc for a in accs if a[0] != "esc" and a[2] < 0]
    changed = True
    while changed and ivs:
        changed = False
        for lo, hi in direct:
            k = find_iv(ivs, lo, hi)
            if k is not None and not (ivs[k][0] <= lo and hi <= ivs[k][1]):
                ivs[k] = [min(lo, ivs[k][0]), max(hi, ivs[k][1])]
                ivs = merge_iv(ivs)
                changed = True
    return ivs


def map_stack(P, Q, callees):
    """Assign x.stk for every r1-relative access; returns private slot names (P, Q)."""
    pairs = []
    for b in range(len(P.blocks)):
        gp, gq = collections.defaultdict(list), collections.defaultdict(list)
        for a in P.acc[b]:
            gp[(a[0], a[1])].append(a)
        for a in Q.acc[b]:
            gq[(a[0], a[1])].append(a)
        for k, lst in gp.items():
            for a, c in zip(lst, gq.get(k, ())):
                pairs.append((a, c))
    for F in (P, Q):
        for accs in F.acc:
            for a in accs:
                if a[2] >= 0:
                    if a[0] == "esc":
                        raise Unsupported("stack: address of caller frame taken")
                    a[4].stk = ("key", ("sp", "caller", a[2]))
    if P.esc != Q.esc:
        raise Different("stack: address-taken slots only on one side")
    # address-taken (escaped) objects: extent from the pointer-extent analysis
    escp = [(a, c) for a, c in pairs if a[0] == "esc"]
    for F, side in ((P, 0), (Q, 1)):
        allesc = {id(a) for accs in F.acc for a in accs if a[0] == "esc"}
        if allesc - {id(pc[side]) for pc in escp}:
            raise Unsupported("stack: unpaired address-taken slot")
    (extP, loP), (extQ, loQ) = escape_extents(P, callees), escape_extents(Q, callees)
    bp, bq = [], []
    for a, c in escp:
        e = max(extP.get(a[2], 0), extQ.get(c[2], 0), 1)
        lo = min(loP.get(a[2], 0), loQ.get(c[2], 0))
        bp.append((a[2] + lo, a[2] + e if e < INF else 0))
        bq.append((c[2] + lo, c[2] + e if e < INF else 0))
    cp, cq = clusters(P, bp), clusters(Q, bq)
    cmap, rmap = {}, {}
    for a, c in escp:
        kp, kq = find_iv(cp, a[2]), find_iv(cq, c[2])
        d = a[2] - c[2]
        if cmap.get(kp, (kq, d)) != (kq, d) or rmap.get(kq, kp) != kp:
            raise Different("stack: address-taken object layout differs (P %d vs Q %d)" % (a[2] + P.N, c[2] + Q.N))
        cmap[kp], rmap[kq] = (kq, d), kp
    for kp, (kq, d) in cmap.items():
        if cp[kp][0] - d != cq[kq][0] or cp[kp][1] - d != cq[kq][1]:
            raise Different("stack: address-taken object extent differs (P %d..%d vs Q %d..%d)" % (
                cp[kp][0] + P.N, cp[kp][1] + P.N, cq[kq][0] + Q.N, cq[kq][1] + Q.N))
    for a, c in pairs:
        if a[0] == "esc" or a[2] >= 0 or c[2] >= 0:
            continue
        kp, kq = find_iv(cp, a[2], a[2] + a[3]), find_iv(cq, c[2], c[2] + c[3])
        if kp is None and kq is None:
            continue
        if kp is None or kq is None or cmap.get(kp) != (kq, a[2] - c[2]):
            raise Different("stack: access inside address-taken object differs (P %d vs Q %d)" % (a[2] + P.N, c[2] + Q.N))
    for F, cl, delta in ((P, cp, lambda k: 0), (Q, cq, lambda k: cmap[rmap[k]][1] if k in rmap else None)):
        for accs in F.acc:
            for a in accs:
                if a[2] >= 0:
                    continue
                k = find_iv(cl, a[2], a[2] + max(a[3], 1))
                if k is None:
                    continue
                d = delta(k)
                if d is None:
                    raise Unsupported("stack: unmatched address-taken object")
                kk = ("sp", "loc", a[2] + d)
                a[4].stk = ("esc", kk) if a[0] == "esc" else ("key", kk)
    # private regions: everything outside the escaped clusters
    def private(F, cl):
        iv = merge_iv([[a[2], a[2] + a[3]] for accs in F.acc for a in accs
                       if a[0] != "esc" and a[2] < 0 and find_iv(cl, a[2], a[2] + a[3]) is None])
        loaded = [False] * len(iv)
        for accs in F.acc:
            for a in accs:
                if a[0] == "ld" and a[2] < 0 and find_iv(cl, a[2], a[2] + a[3]) is None:
                    loaded[find_iv(iv, a[2])] = True
        return iv, loaded
    rp, lp = private(P, cp)
    rq, lq = private(Q, cq)
    mpq, mqp = {}, {}
    for a, c in pairs:
        if a[0] == "esc" or a[2] >= 0 or c[2] >= 0 or find_iv(cp, a[2], a[2] + a[3]) is not None:
            continue
        kp, kq = find_iv(rp, a[2]), find_iv(rq, c[2])
        if kq is None or not lp[kp] or not lq[kq]:
            continue
        if a[2] - rp[kp][0] != c[2] - rq[kq][0] or rp[kp][1] - rp[kp][0] != rq[kq][1] - rq[kq][0]:
            raise Different("stack: slot layout differs (P %d vs Q %d)" % (a[2] + P.N, c[2] + Q.N))
        if mpq.get(kp, kq) != kq or mqp.get(kq, kp) != kp:
            raise Different("stack: inconsistent slot mapping (P %d vs Q %d)" % (a[2] + P.N, c[2] + Q.N))
        mpq[kp], mqp[kq] = kq, kp
    def argbound(F):
        b = 0
        for x in F.ins:
            if x.idx in F.strip:
                continue
            if x.cls == "bl":
                nm = x.callkey[1] if x.callkey and x.callkey[0] == "sym" else None
                b = max(b, callees[nm][4] if nm in callees else INF)
            elif x.cls == "icall":
                b = max(b, callees["<indirect>"][4] if "<indirect>" in callees else INF)
        return b
    P.argbound, Q.argbound = argbound(P), argbound(Q)
    names = []
    for F, cl, regs, loaded, name in ((P, cp, rp, lp, lambda k: "S%d" % k),
                                      (Q, cq, rq, lq, lambda k: "S%d" % mqp[k] if k in mqp else "SQ%d" % k)):
        nm = set()
        for accs in F.acc:
            for a in accs:
                if a[2] >= 0 or a[0] == "esc" or find_iv(cl, a[2], a[2] + a[3]) is not None:
                    continue
                k = find_iv(regs, a[2])
                if not loaded[k] and regs[k][0] + F.N >= 8 + F.argbound:
                    a[4].stk = ("dead",)
                elif not loaded[k]:
                    a[4].stk = ("key", ("sp", "out", a[2] + F.N))
                else:
                    a[4].stk = ("priv", name(k), a[2] - regs[k][0], a[3])
                    nm.add(name(k))
        names.append(sorted(nm))
    return names[0], names[1]


# ----------------------------------------------------------------------------
# Symbolic execution


class Different(Exception):
    pass


class H:
    def __init__(self):
        self.tab = {}
        self.rev = []
        self.kids = []
        self.memo_uninit = {}
        self.memo_addr = {}

    def mk(self, t, kids=()):
        i = self.tab.get(t)
        if i is None:
            i = len(self.rev)
            self.tab[t] = i
            self.rev.append(t)
            self.kids.append(tuple(kids))
        return i

    def uninit(self, i):
        r = self.memo_uninit.get(i)
        if r is None:
            t = self.rev[i]
            if t[0] == "uninit":
                r = True
            elif t[0] == "in":
                r = t[3]
            else:
                r = any(self.uninit(k) for k in self.kids[i])
            self.memo_uninit[i] = r
        return r

    def addr_classes(self, i, ro):
        r = self.memo_addr.get((i, id(ro)))
        if r is None:
            t = self.rev[i]
            if t[0] == "addr" or t[0] == "ha":
                r = frozenset([ro.get(t[1], "rw")])
            elif t[0] == "sp":
                r = frozenset(["stack"])
            else:
                r = frozenset()
                for k in self.kids[i]:
                    r = r | self.addr_classes(k, ro)
            self.memo_addr[(i, id(ro))] = r
        return r


def addk(h, x, k):
    if k == 0:
        return x
    t = h.rev[x]
    if t[0] == "c":
        return h.mk(("c", (t[1] + k) & 0xFFFFFFFF))
    if t[0] == "addr":
        return h.mk(("addr", t[1], t[2] + k))
    if t[0] == "sp":
        return h.mk(("sp", t[1], t[2] + k))
    if t[0] == "addk":
        kk = (t[2] + k) & 0xFFFFFFFF
        return t[1] if kk == 0 else h.mk(("addk", t[1], kk), (t[1],))
    return h.mk(("addk", x, k & 0xFFFFFFFF), (x,))


def ea_of(x, env, h):
    base, disp, index, _upd = x.mem
    if index is not None:
        b = env[index]
        if base is None:
            return b
        a = env[base]
        lo, hi = min(a, b), max(a, b)
        return h.mk(("op", "eax", (), (lo, hi), 0), (lo, hi))
    if isinstance(disp, tuple):
        kind, key, add = disp
        if kind == "sda" and base is None:
            return h.mk(("addr", key, add))
        if kind == "lo" and base is not None and h.rev[env[base]] == ("ha", key, add):
            return h.mk(("addr", key, add))
        ch = (env[base],) if base else ()
        return h.mk(("op", "ear", (disp,), ch, 0), ch)
    if base is None:
        return h.mk(("c", disp & 0xFFFFFFFF))
    return addk(h, env[base], disp)


class Result:
    __slots__ = ("env", "events", "evins", "term", "termins", "termids")


def run_block(fn, b, env_in, h, callees, retregs=RET_REGS):
    env = dict(env_in)
    blk = fn.blocks[b]
    events, evins = [], []
    wr = collections.defaultdict(dict)
    ro = fn.rctx.ro
    end = blk.end - 1 if blk.kind not in ("fall",) else blk.end
    for i in range(blk.start, end):
        if i in fn.strip:
            continue
        x = fn.ins[i]
        c = x.cls
        if c == "alu":
            ch = [env[u] for u in x.uses]
            if x.stk is not None:          # escaped stack address
                env[x.defs[0]] = h.mk(x.stk[1])
                continue
            if x.copy:
                vals = [ch[0]]
            else:
                vals = alu(x, ch, h)
            for d, v in zip(x.defs, vals):
                env[d] = v
            if x.rc:
                env["cr0"] = h.mk(("rc0", vals[0]), (vals[0],))
        elif c == "ld":
            if x.stk is not None and x.stk[0] == "priv":
                _, slot, rel, w = x.stk
                env[x.defs[0]] = slot_load(h, env[slot], wr[slot], rel, w, x)
                continue
            ea = h.mk(x.stk[1]) if x.stk is not None else ea_of(x, env, h)
            if x.mem[3]:
                env[x.mem[0]] = ea
            cl = h.addr_classes(ea, ro)
            if x.stk is None and cl and cl <= {"ro"}:
                v = h.mk(("rod", x.mnem, x.imms, ea), (ea,))
            else:
                events.append(("ld", x.mnem, x.imms, ea))
                evins.append(x)
                v = h.mk(("ev", b, len(events) - 1, 0))
            env[x.defs[0]] = v
        elif c == "st":
            if x.mnem == "stmw":
                raise Unsupported("stmw in body")
            if x.stk == ("dead",):
                continue
            val = env[x.src]
            if x.stk is not None and x.stk[0] == "priv":
                _, slot, rel, w = x.stk
                sv = h.mk(("stv", x.mnem, x.imms, val), (val,))
                for j in range(w):
                    wr[slot][rel + j] = (sv, j, w)
                continue
            ea = h.mk(x.stk[1]) if x.stk is not None else ea_of(x, env, h)
            if x.mem[3]:
                env[x.mem[0]] = ea
            events.append(("st", x.mnem, x.imms, ea, val))
            evins.append(x)
        elif c == "bl":
            name = x.callkey[1] if x.callkey[0] == "sym" else None
            au, pt = callees[name][:2] if name in callees else (INDMASK | mask(["r11", "r12"]), VOLMASK)
            args = tuple((r, env[r]) for r in unmask(au))
            events.append(("call", x.callkey, args))
            evins.append(x)
            k = len(events) - 1
            for r in VOLATILE:
                if pt >> REGIDX[r] & 1:
                    env[r] = h.mk(("evp", b, k, r, env[r]), (env[r],))
                else:
                    env[r] = h.mk(("ev", b, k, r))
            env["lr"] = h.mk(("ev", b, k, "lr"))
        elif c == "icall":
            args = tuple((r, env[r]) for r in IND_ARGS)
            events.append(("icall", env[x.uses[0]], args))
            evins.append(x)
            k = len(events) - 1
            for r in VOLATILE:
                env[r] = h.mk(("evp", b, k, r, env[r]), (env[r],))
            env["lr"] = h.mk(("ev", b, k, "lr"))
        elif c == "ev":
            events.append(("sfx", x.mnem, x.imms, tuple(env[u] for u in x.uses)))
            evins.append(x)
            k = len(events) - 1
            for j, d in enumerate(x.defs):
                env[d] = h.mk(("ev", b, k, j))
        else:
            raise Unsupported("unexpected " + x.raw)
    # fold stack writes into slot values
    for slot, w in wr.items():
        if w:
            env[slot] = slot_fold(h, env[slot], w)
    # terminator
    term = (blk.kind,)
    tins = fn.ins[blk.end - 1]
    if blk.kind in ("bc", "retc"):
        x = tins
        bo = x.bo & 0x1E
        if bo & 16:
            bo &= ~8
        if bo & 4:
            bo &= ~2
        cr = env["cr%d" % (x.bi // 4)] if not x.bo & 16 else None
        ctr = None
        if not x.bo & 4:
            env["ctr"] = h.mk(("op", "dec", (), (env["ctr"],), 0), (env["ctr"],))
            ctr = env["ctr"]
        term = (blk.kind, bo, x.bi % 4 if cr is not None else None, cr, ctr)
    elif blk.kind == "jt":
        term = ("jt", env["ctr"])
    elif blk.kind == "tail":
        # b <function>: the target runs on our registers and returns to our caller
        name = tins.callkey[1] if tins.callkey[0] == "sym" else None
        if name not in callees:
            raise Unsupported("tail call to unknown function")
        au, pt = callees[name][:2]
        use = au | (pt & mask(retregs))
        events.append(("call", tins.callkey, tuple((r, env[r]) for r in unmask(use))))
        evins.append(tins)
    elif blk.kind == "itail":
        raise Unsupported("indirect tail jump")
    r = Result()
    r.env, r.events, r.evins, r.term, r.termins = env, events, evins, term, tins
    r.termids = [v for v in term[3:5] if v is not None] if blk.kind in ("bc", "retc") else list(term[1:2]) if blk.kind == "jt" else []
    if blk.kind in ("ret", "retc"):
        rv = tuple(env[x] for x in retregs)
        r.term = r.term + rv
        r.termids += list(rv)
    return r


def alu(x, ch, h):
    m = x.mnem
    if m in ("addi", "addis"):
        imm = x.imms[0]
        if isinstance(imm, tuple):
            kind, key, add = imm
            if m == "addis" and kind == "ha" and not ch:
                return [h.mk(("ha", key, add))]
            if m == "addi" and kind == "lo" and ch and h.rev[ch[0]] == ("ha", key, add):
                return [h.mk(("addr", key, add))]
            if m == "addi" and kind == "sda" and not ch:
                return [h.mk(("addr", key, add))]
            return [h.mk(("op", m, (imm,), tuple(ch), 0), ch)]
        k = imm if m == "addi" else imm << 16
        if not ch:
            return [h.mk(("c", k & 0xFFFFFFFF))]
        return [addk(h, ch[0], k)]
    if m == "cmpi" and x.imms == (0, 0):
        return [h.mk(("rc0", ch[0]), ch)]
    if m in COMM:
        ch = sorted(ch[:2]) + ch[2:]
    ch = tuple(ch)
    return [h.mk(("op", m, x.imms, ch, s), ch) for s in range(x.vdefs)]


def slot_load(h, base, wr, rel, w, x):
    runs = []
    for o in range(rel, rel + w):
        e = wr.get(o)
        if e is None:
            if runs and runs[-1][0] == "b" and runs[-1][1] + runs[-1][2] == o:
                runs[-1] = ("b", runs[-1][1], runs[-1][2] + 1)
            else:
                runs.append(("b", o, 1))
        else:
            sv, j, vw = e
            if runs and runs[-1][0] == "v" and runs[-1][1] == sv and runs[-1][2] + runs[-1][3] == j:
                runs[-1] = ("v", sv, runs[-1][2], runs[-1][3] + 1, vw)
            else:
                runs.append(("v", sv, j, 1, vw))
    if len(runs) == 1 and runs[0][0] == "v" and runs[0][2] == 0 and runs[0][3] == w == runs[0][4]:
        sv = runs[0][1]
        t = h.rev[sv]
        if (t[1], x.mnem) in IDENT_PAIRS:
            return t[3]
        return h.mk(("ldv", x.mnem, x.imms, sv), (sv,))
    if len(runs) == 1 and runs[0][0] == "b":
        s = h.mk(("sel", base, rel, w), (base,))
        return h.mk(("ldv", x.mnem, x.imms, s), (s,))
    kids = (base,) + tuple(r[1] for r in runs if r[0] == "v")
    s = h.mk(("cat", base, tuple(runs)), kids)
    return h.mk(("ldv", x.mnem, x.imms, s), (s,))


def slot_fold(h, base, w):
    ent = []
    for o in sorted(w):
        sv, j, vw = w[o]
        if ent and ent[-1][1] == sv and ent[-1][0] + ent[-1][3] == o and ent[-1][2] + ent[-1][3] == j:
            ent[-1] = (ent[-1][0], sv, ent[-1][2], ent[-1][3] + 1)
        else:
            ent.append((o, sv, j, 1))
    return h.mk(("upd", base, tuple(ent)), (base,) + tuple(e[1] for e in ent))


# ----------------------------------------------------------------------------
# Comparison driver


def describe(x):
    return "0x%x '%s'" % (x.addr, x.raw) if x is not None else "-"


def compare(P, Q, callees):
    """Returns (verdict, reason)."""
    if len(P.blocks) != len(Q.blocks):
        return "UNSUPPORTED", "cfg: %d vs %d blocks" % (len(P.blocks), len(Q.blocks))
    for k, (a, c) in enumerate(zip(P.blocks, Q.blocks)):
        if a.kind != c.kind or a.succs != c.succs:
            return "UNSUPPORTED", "cfg: block %d (%s->%s vs %s->%s)" % (k, a.kind, a.succs, c.kind, c.succs)
    try:
        prepare(P)
        prepare(Q)
        slotsP, slotsQ = map_stack(P, Q, callees)
    except Different as e:
        return ("UNSUPPORTED" if str(e).startswith("stack:") else "DIFFERENT"), str(e)
    h = H()
    itemsP = ALLREGS + slotsP
    itemsQ = ALLREGS + slotsQ
    items = [("P", n) for n in itemsP] + [("Q", n) for n in itemsQ]
    entry = {}
    for side, names in (("P", itemsP), ("Q", itemsQ)):
        entry[side] = {n: h.mk(("entry", n)) if n in REGIDX else h.mk(("uninit", n)) for n in names}
    nb = len(P.blocks)
    preds = [[] for _ in range(nb)]
    for b, blk in enumerate(P.blocks):
        for s in blk.succs:
            preds[s].append(b)
    seen, order = set(), []

    def dfs(b):
        stack = [(b, iter(P.blocks[b].succs))]
        seen.add(b)
        while stack:
            node, it = stack[-1]
            nxt = next(it, None)
            if nxt is None:
                order.append(node)
                stack.pop()
            elif nxt not in seen:
                seen.add(nxt)
                stack.append((nxt, iter(P.blocks[nxt].succs)))
    dfs(0)
    rpo = order[::-1]
    retregs = unmask(callees[P.name][5]) if P.name in callees else RET_REGS
    exitenv, part = {}, {}
    for _it in range(100):
        changed = False
        for b in rpo:
            srcs = ([entry] if b == 0 else []) + [exitenv[p] for p in preds[b] if p in exitenv]
            groups = collections.defaultdict(list)
            for it in items:
                groups[tuple(s[it[0]][it[1]] for s in srcs)].append(it)
            lab = {}
            for sig, mem in groups.items():
                flag = any(h.uninit(e) for e in sig)
                l = h.mk(("in", b, min(mem), flag))
                for it in mem:
                    lab[it] = l
            if part.get(b) != lab:
                part[b] = lab
                changed = True
            envP = {n: lab[("P", n)] for n in itemsP}
            envQ = {n: lab[("Q", n)] for n in itemsQ}
            rP = run_block(P, b, envP, h, callees, retregs)
            rQ = run_block(Q, b, envQ, h, callees, retregs)
            n = min(len(rP.events), len(rQ.events))
            for k in range(max(len(rP.events), len(rQ.events))):
                if k >= n or rP.events[k] != rQ.events[k]:
                    xp = rP.evins[k] if k < len(rP.evins) else None
                    xq = rQ.evins[k] if k < len(rQ.evins) else None
                    what = ""
                    if k < n:
                        ep, eq_ = rP.events[k], rQ.events[k]
                        if ep[0] == eq_[0] and ep[0] in ("call", "icall") and ep[:2] == eq_[:2]:
                            what = " %s args differ: %s" % (ep[0], "/".join(r for (r, u), (_r, v) in zip(ep[2], eq_[2]) if u != v))
                        elif ep[0] == eq_[0] and ep[0] in ("ld", "st") and ep[:3] == eq_[:3]:
                            what = " address differs" if ep[3] != eq_[3] else " stored value differs"
                            if ep[0] == "st" and ep[3] != eq_[3] and h.rev[ep[3]][:2] == ("sp", "out") == h.rev[eq_[3]][:2]:
                                return "UNSUPPORTED", "stack: store-only slot at a different SP offset (possible outgoing argument): %s vs %s" % (
                                    describe(xp), describe(xq))
                    return "DIFFERENT", "block %d effect %d%s: %s vs %s" % (b, k, what, describe(xp), describe(xq))
            if rP.term != rQ.term:
                nr = len(retregs)
                if P.blocks[b].kind in ("ret", "retc") and nr and rP.term[:-nr] == rQ.term[:-nr]:
                    bad = [r for r, u, v in zip(retregs, rP.term[-nr:], rQ.term[-nr:]) if u != v]
                    return "DIFFERENT", "block %d return value %s differs (%s)" % (b, "/".join(bad), describe(rP.termins))
                return "DIFFERENT", "block %d branch: %s vs %s" % (b, describe(rP.termins), describe(rQ.termins))
            exitenv[b] = {"P": rP.env, "Q": rQ.env}
        if not changed:
            break
    else:
        return "UNSUPPORTED", "no fixpoint"
    same_seq = all(
        [P.ins[i].mnem for i in range(a.start, a.end) if i not in P.strip] ==
        [Q.ins[i].mnem for i in range(c.start, c.end) if i not in Q.strip]
        for a, c in zip(P.blocks, Q.blocks))
    notes = []
    if P.N != Q.N:
        notes.append("frame %d/%d" % (P.N, Q.N))
    if set(P.saves) != set(Q.saves):
        notes.append("saved %d/%d regs" % (len(P.saves), len(Q.saves)))
    return "EQUIVALENT", ("L1" if same_seq else "L2") + (": " + ", ".join(notes) if notes else "")


def flat_ids(ev):
    if ev[0] in ("ld", "st"):
        return list(ev[3:])
    if ev[0] == "call":
        return [v for _r, v in ev[2]]
    if ev[0] == "icall":
        return [ev[1]] + [v for _r, v in ev[2]]
    return list(ev[3])


# ----------------------------------------------------------------------------
# Driver


_objcache = collections.OrderedDict()


def get_obj(path):
    o = _objcache.get(path)
    if o is None:
        o = Obj(path)
        _objcache[path] = o
        if len(_objcache) > 16:
            _objcache.popitem(last=False)
    else:
        _objcache.move_to_end(path)
    return o


def unit_paths(unit):
    rel = unit.split("/", 1)[1]
    return os.path.join(BUILD, "obj", rel + ".o"), os.path.join(BUILD, "src", rel + ".o")


def check(unit, name, callees, mutate=None):
    pp, qp = unit_paths(unit)
    if not os.path.exists(qp):
        return "UNSUPPORTED", "object not built"
    try:
        P = load_func(get_obj(pp), name)
    except Unsupported as e:
        return "UNSUPPORTED", "retail: " + str(e)
    try:
        Q = load_func(get_obj(qp), name, mutate)
    except Unsupported as e:
        return "UNSUPPORTED", "ours: " + str(e)
    try:
        return compare(P, Q, callees)
    except Unsupported as e:
        return "UNSUPPORTED", str(e)


def report_functions():
    r = json.load(open(os.path.join(BUILD, "report.json")))
    out = []
    for u in r["units"]:
        for f in u.get("functions", []):
            out.append((u["name"], f["name"], int(f.get("size", 0)), float(f.get("fuzzy_match_percent", 0.0))))
    return out, int(r["measures"]["total_code"])


def show(unit, name):
    pp, qp = unit_paths(unit)
    for tag, path in (("retail", pp), ("ours", qp)):
        fn = load_func(get_obj(path), name)
        prepare(fn)
        print("== %s  frame %d  saved %s" % (tag, fn.N, " ".join(sorted(fn.saves, key=lambda r: REGIDX[r]))))
        for k, b in enumerate(fn.blocks):
            print("  block %d %s -> %s" % (k, b.kind, b.succs))
            for i in range(b.start, b.end):
                x = fn.ins[i]
                print("    %s%5x  %s" % ("-" if i in fn.strip else " ", x.addr, x.raw))


# Mutations for --selftest.  Each returns a new raw list or None if not applicable.
def _mut_ops(raw, rel, pred, fn):
    idx = [i for i, (a, m, o) in enumerate(raw) if a not in rel and pred(m, o)]
    if not idx:
        return None
    i = idx[len(idx) // 2]
    a, m, o = raw[i]
    new = fn(m, o)
    if new is None:
        return None
    out = list(raw)
    out[i] = (a, new[0], new[1])
    return out


def mut_swap_sub(raw, rel):
    def f(m, o):
        p = o.split(",")
        return None if p[1] == p[2] else (m, ",".join([p[0], p[2], p[1]]))
    return _mut_ops(raw, rel, lambda m, o: m in ("subf", "fsubs", "fsub", "divw", "fdivs"), f)


def mut_const(raw, rel):
    def f(m, o):
        p = o.split(",")
        return (m, ",".join(p[:-1] + [str(pint(p[-1]) + 1)]))
    return _mut_ops(raw, rel, lambda m, o: m in ("addi", "cmpi", "cmpli", "mulli") and "r1," not in o and not o.startswith("r1"), f)


def mut_offset(raw, rel):
    def f(m, o):
        p = o.split(",")
        mm = MEM_RE.match(p[1])
        return (m, ",".join([p[0], "%d(%s)" % (pint(mm.group(1)) + 4, mm.group(2))] + p[2:]))
    return _mut_ops(raw, rel, lambda m, o: m in ("lwz", "stw", "lfs", "stfs") and "(r1)" not in o and "(0)" not in o, f)


def mut_cond(raw, rel):
    def f(m, o):
        p = o.split(",")
        bo = pint(p[0])
        return (m, ",".join([str(bo ^ 8)] + p[1:]))
    return _mut_ops(raw, rel, lambda m, o: m == "bc" and pint(o.split(",")[0]) in (4, 12), f)


def mut_rename(raw, rel):
    """Positive control: swap two callee-saved registers everywhere (should stay EQUIVALENT)."""
    text = " ".join(o for _a, _m, o in raw)
    if not (re.search(r"\br30\b", text) and re.search(r"\br31\b", text)) or "stmw" in text or "lmw" in text:
        return None
    sw = {"r30": "r31", "r31": "r30"}
    return [(a, m, re.sub(r"\br3[01]\b", lambda mm: sw[mm.group(0)], o)) for a, m, o in raw]


def mut_schedule(raw, rel):
    """Positive control: swap two adjacent independent ALU instructions (should stay EQUIVALENT, L2)."""
    alu_m = {"rlwinm", "fmuls", "fadds", "fsubs", "add", "subf", "fmr"}
    for i in range(len(raw) - 1):
        (a1, m1, o1), (a2, m2, o2) = raw[i], raw[i + 1]
        if m1 in alu_m and m2 in alu_m:
            try:
                x1, x2 = decode(a1, m1, o1), decode(a2, m2, o2)
            except Unsupported:
                continue
            if set(x1.defs) & (set(x2.uses) | set(x2.defs)) or set(x2.defs) & set(x1.uses) or "r1" in x1.uses + x2.uses:
                continue
            out = list(raw)
            out[i], out[i + 1] = (a1, m2, o2), (a2, m1, o1)
            return out
    return None


MUTATIONS = [("swap-sub-operands", mut_swap_sub, "DIFFERENT"), ("constant+1", mut_const, "DIFFERENT"),
             ("member-offset+4", mut_offset, "DIFFERENT"), ("branch-sense", mut_cond, "DIFFERENT"),
             ("rename-r30-r31", mut_rename, "EQUIVALENT"), ("swap-independent", mut_schedule, "EQUIVALENT")]


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("-u", "--unit")
    ap.add_argument("-f", "--function")
    ap.add_argument("--show", action="store_true")
    ap.add_argument("--write", action="store_true", help="write docs/progress/equiv/{equiv.tsv,summary.md}")
    ap.add_argument("--exact-sample", type=int, default=0)
    ap.add_argument("--selftest", type=int, default=0)
    ap.add_argument("--seed", type=int, default=1)
    ap.add_argument("--rebuild-callees", action="store_true")
    ap.add_argument("-v", "--verbose", action="store_true")
    args = ap.parse_args()
    if args.rebuild_callees and os.path.exists(CALLEE_CACHE):
        os.replace(CALLEE_CACHE, CALLEE_CACHE + ".old")
    callees = load_callee_tuples(args.verbose)
    funcs, total_code = report_functions()
    if args.unit:
        unit = args.unit if args.unit.startswith("mario/") else "mario/" + args.unit
        sel = [f for f in funcs if f[0] == unit and (not args.function or f[1] == args.function)]
        if not sel and args.function:
            sel = [(unit, args.function, 0, 0.0)]
        if args.show and args.function:
            show(unit, args.function)
        for u, n, size, fz in sel:
            v, why = check(u, n, callees)
            print("%s\t%s\t%d\t%.2f\t%s\t%s" % (u, n, size, fz, v, why))
        return
    rng = random.Random(args.seed)
    if args.exact_sample:
        ex = [f for f in funcs if f[3] == 100.0]
        sample = rng.sample(ex, min(args.exact_sample, len(ex)))
        cnt = collections.Counter()
        for u, n, size, fz in sample:
            v, why = check(u, n, callees)
            cnt[v] += 1
            if v != "EQUIVALENT":
                print("%s\t%s\t%s\t%s" % (u, n, v, why))
        print("exact sample:", dict(cnt))
        return
    if args.selftest:
        ex = [f for f in funcs if f[3] == 100.0 and f[2] >= 64]
        rng.shuffle(ex)
        res = collections.defaultdict(collections.Counter)
        done = collections.Counter()
        for u, n, size, fz in ex:
            base, _ = check(u, n, callees)
            if base != "EQUIVALENT":
                continue
            for mname, mfn, want in MUTATIONS:
                if done[mname] >= args.selftest:
                    continue
                applied = []

                def mut(raw, rel, mfn=mfn):
                    out = mfn(raw, rel)
                    applied.append(out is not None)
                    return out or raw
                v, why = check(u, n, callees, mutate=mut)
                if not applied or not applied[0]:
                    continue
                done[mname] += 1
                res[mname][v] += 1
                if v != want and args.verbose:
                    print("  %s on %s: %s (%s)" % (mname, n, v, why))
            if all(done[m[0]] >= args.selftest for m in MUTATIONS):
                break
        for mname, _f, want in MUTATIONS:
            print("%-20s want %-10s got %s" % (mname, want, dict(res[mname])))
        return
    # all non-exact functions
    rows = []
    for u, n, size, fz in funcs:
        if fz == 100.0:
            continue
        v, why = check(u, n, callees)
        rows.append((u, n, size, fz, v, why))
        if args.verbose:
            print("%s\t%s\t%s\t%s" % (u, n, v, why), file=sys.stderr)
    summarize(rows, funcs, total_code, args.write)


def load_raw(path, name):
    obj = get_obj(path)
    s = obj.funcs.get(name)
    if s is None:
        return []
    txt = obj.text[s["shndx"]]
    return [(a, txt[a][0], txt[a][1]) for a in range(s["value"], s["value"] + s["size"], 4) if a in txt]


def summarize(rows, funcs, total_code, write):
    cnt, byts = collections.Counter(), collections.Counter()
    lv = collections.Counter()
    reasons = collections.Counter()
    for u, n, size, fz, v, why in rows:
        cnt[v] += 1
        byts[v] += size
        if v == "EQUIVALENT":
            lv[why[:2]] += 1
        else:
            r = why.split(" (")[0] if why.startswith("stack:") else why.split(":")[0] if v == "UNSUPPORTED" and not why.startswith("cfg") else why
            reasons[(v, re.sub(r"0x[0-9a-f]+ '[^']*'|\d+", "#", r)[:70])] += 1
    exact_n = sum(1 for f in funcs if f[3] == 100.0)
    exact_b = sum(f[2] for f in funcs if f[3] == 100.0)
    cert_b = exact_b + byts["EQUIVALENT"]
    lines = []
    lines.append("# Semantic-equivalence certification (`tools/equiv-check.py`)")
    lines.append("")
    lines.append("Generated from `build/GMSE01/report.json` and the retail/our objects of the current build.")
    lines.append("Byte counts are retail function sizes.")
    lines.append("")
    lines.append("| verdict | functions | bytes |")
    lines.append("| --- | ---: | ---: |")
    lines.append("| exact (report.json 100%%) | %d | %d |" % (exact_n, exact_b))
    for v in ("EQUIVALENT", "DIFFERENT", "UNSUPPORTED"):
        extra = " (L1 %d, L2 %d)" % (lv["L1"], lv["L2"]) if v == "EQUIVALENT" else ""
        lines.append("| %s%s | %d | %d |" % (v, extra, cnt[v], byts[v]))
    lines.append("| **all code** | %d | %d |" % (len(funcs), total_code))
    lines.append("")
    lines.append("**exact + certified: %d / %d bytes = %.2f%% of all code** (exact alone %.2f%%; certification adds %.2f points)." % (
        cert_b, total_code, 100.0 * cert_b / total_code, 100.0 * exact_b / total_code, 100.0 * byts["EQUIVALENT"] / total_code))
    lines.append("")
    lines.append("Per-function verdicts: `equiv.tsv`.  How the checker was validated: `validation.md`.")
    lines.append("Regenerate with `python3 tools/equiv-check.py --write` after a build (about 20 s; the callee")
    lines.append("summaries are cached in `build/GMSE01/equiv-callees.json`, `--rebuild-callees` refreshes them).")
    lines.append("")
    lines.append("Most common non-certified reasons (numbers and addresses elided):")
    lines.append("")
    lines.append("| verdict | reason | functions |")
    lines.append("| --- | --- | ---: |")
    for (v, r), c in reasons.most_common(25):
        lines.append("| %s | %s | %d |" % (v, r.replace("|", "/"), c))
    text = "\n".join(lines) + "\n"
    print(text)
    if write:
        os.makedirs(OUT_DIR, exist_ok=True)
        with open(os.path.join(OUT_DIR, "equiv.tsv"), "w") as fh:
            fh.write("unit\tfunction\tsize\tfuzzy\tverdict\treason\n")
            for u, n, size, fz, v, why in rows:
                fh.write("%s\t%s\t%d\t%.2f\t%s\t%s\n" % (u, n, size, fz, v, why.replace("\t", " ")))
        with open(os.path.join(OUT_DIR, "summary.md"), "w") as fh:
            fh.write(text)
            fh.write(ASSUMPTIONS)


ASSUMPTIONS = """
## What EQUIVALENT means

The retail function (P) and ours (Q) have the same CFG, and for every basic
block the ordered side effects (non-stack loads, stores, calls with the
argument registers the callee may read, special-purpose instructions), the
branch conditions and the return registers some caller reads are the same
expressions over the function's inputs.  Values are hash-consed expression
trees; block-entry values are equivalence classes of registers/stack slots
computed from every predecessor (optimistic start, refined to a fixpoint on
loops), so register allocation may differ per block.
Level 1: the opcode sequences are identical (only registers, stack slots and
the frame differ).  Level 2: instructions are reordered inside blocks
(scheduling), or moves are added/removed.

## Assumptions (soundness)

- Compositional: a function is certified assuming every function it calls
  behaves like its retail version, and whole-program facts are taken from the
  retail objects (`build/GMSE01/obj`).  Per retail function the tool computes,
  by fixpoints over the call graph: the volatile registers it may read (argument
  registers compared at direct call sites), the volatile registers it may pass
  through unchanged, how many bytes it may access through each pointer argument
  (and which argument pointer it may return), how many bytes of its caller's
  outgoing-argument area it may read, and which of r3/r4/f1 any caller reads
  after calling it (all three for address-taken functions: vtables, callbacks).
- Indirect calls may read r3-r10, f1-f8 and cr1 (EABI) and access anything a
  pointer argument reaches.
- Return registers that no retail caller reads are not compared (a mutated
  constructor return value `this+1` is correctly reported EQUIVALENT when every
  caller ignores it).
- Compiler-anonymous data (`@NNNN`, `...data.0`, `@stringBase0` strings, switch
  tables) is compared by bytes and relocations and assumed to be accessed within
  its own extent; writable anonymous objects and `name$N` statics (local static
  guards) are additionally matched by first-reference order within the
  function.  Loads from `.rodata`/`.sdata2` are pure (may be reordered).
- Stack: the extent of an address-taken slot comes from the pointer-extent
  analysis; every access inside it must move with it (one translation per
  object) and its accesses are ordered memory effects.  Other regions may be
  permuted freely (consistent bijection) and are compared by dataflow.  A slot
  that is stored but never loaded keeps its SP offset if a callee may read it as
  a stack argument, and is dropped as dead otherwise.
- Equivalence is for executions that do not read uninitialised stack memory.
  NaN payload propagation of commutative FP operations (fadd/fmul operand order)
  and the XER summary-overflow bit are not modelled.  The saved-register sets
  may differ: each side is checked to save and restore every callee-saved
  register it writes.

## Known limitations (why an equivalent function can be DIFFERENT/UNSUPPORTED)

- A different CFG (inlined vs called helper, different block layout, unrolling)
  is UNSUPPORTED, as are indirect tail jumps and unmatched symbols.
- Non-stack loads, stores and calls must stay in the same order.
- Algebraic identities are limited to copies, commutativity, constant folding of
  add-immediate chains and `cmpwi 0` == record form.
- An address-taken stack object passed to an indirect (virtual) call, stored in
  memory, or passed to a callee that does either has unbounded extent, so a
  shifted frame layout around it is UNSUPPORTED (`stack: address-taken ...`).
- Stale values in argument registers at indirect calls, and in r3/r4/f1 at the
  return of address-taken (virtual) `void` functions, are compared and can make
  an equivalent function DIFFERENT (`icall args differ`, `return value differs`).
"""


if __name__ == "__main__":
    main()
