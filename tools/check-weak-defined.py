#!/usr/bin/env python3
"""Find weak symbols the original linker saw that our tree never defines.

A weak symbol in the map is a header inline: an in-class member body, an
out-of-class `inline`, an implicit destructor, or an instantiated template.
If our tree declares it but never writes the body, MWCC has nothing to emit,
and for a class whose virtuals are *all* inline that silently externalises the
class's vtable -- the vtable stops being emitted into the TU that should carry
it and the object's `.data` is short by exactly sizeof(vtable).  objdiff still
scores the unit 100/100, but the link shifts every later object and the DOL
breaks (see docs/catalog/linking.md, "Header round 16").

The map's closure section ("Link map of __start") records linkage for every
symbol the linker resolved, so `(func,weak)` / `(object,weak)` there is the
authoritative list.  This script cross-checks it against `nm` over every
object under build/GMSE01/src/ and prints the ones nobody defines, grouped by
the class the mangled name belongs to.

  NM=build/binutils/powerpc-eabi-nm build/venv/bin/python3 \
      tools/check-weak-defined.py --map orig/GMSE01/files/marioUS.MAP

Exit codes: 0 nothing undefined, 1 undefined weak symbols found,
2 could not run (map or objects missing).
"""

import argparse
import os
import re
import subprocess
import sys

# "  8] getCurrentGroupId__7JKRHeapFv (func,weak) found in JSystem.a JKRHeap.cpp"
CLOSURE_RE = re.compile(
    r"^\s*\d+\]\s+(\S+)\s+\((func|object),(weak|global|local)\)"
    r"\s+found in\s+(\S+)\s+(\S+)\s*$"
)

# The closure section is everything before the first "* section layout" block.
SECTION_RE = re.compile(r"^\S.*section layout\s*$")

# nm type letters that count as a *definition* of a weak symbol.  `W`/`V` are
# weak text/data; `T`/`D`/`R`/`B` appear when our tree wrote the body out of
# line (still a definition, just the wrong linkage -- validate-symbol-order
# reports that separately).  `U` is a reference only.
DEFINED_TYPES = set("TtDdRrBbGgSsVvWwAaCi")


def read_map(path):
    """Return {name: (lib, file)} for every weak symbol in the closure."""
    weak = {}
    with open(path, "r", errors="replace") as fp:
        for line in fp:
            if SECTION_RE.match(line):
                break
            m = CLOSURE_RE.match(line)
            if not m:
                continue
            name, _kind, binding, lib, src = m.groups()
            if binding != "weak":
                continue
            # ">>>" is MWLD's marker for a dropped weak duplicate, not a name.
            if name == ">>>":
                continue
            weak.setdefault(name, (lib, src))
    return weak


def read_objects(objdir, nm):
    """Return the set of symbol names defined by any object under objdir."""
    objs = []
    for root, _dirs, files in os.walk(objdir):
        for f in files:
            if f.endswith(".o"):
                objs.append(os.path.join(root, f))
    if not objs:
        return None
    defined = set()
    # nm accepts many files at once; chunk to stay inside ARG_MAX.
    for i in range(0, len(objs), 200):
        out = subprocess.run(
            [nm, "--no-demangle"] + objs[i : i + 200],
            capture_output=True,
            text=True,
        ).stdout
        for line in out.splitlines():
            parts = line.split()
            # "0000001c W name" or "         U name" or "file.o:" / blank
            if len(parts) == 3 and parts[1] in DEFINED_TYPES:
                defined.add(parts[2])
            elif len(parts) == 2 and parts[0] in DEFINED_TYPES:
                defined.add(parts[1])
    return defined


def split_mangled(name):
    """(member, class) for an MWCC mangled symbol; either half may be None.

    A member mangles as `<member>__<class><args>`, where `<class>` is
    `<len><name>` or `Q<n><len><name><len><name>...` for a nested one.  A free
    function has no class part, and `__ct`/`__dt`/`__as`/`__amu`/`__vc` and
    friends have no source identifier of their own -- the class name is then
    the only thing to look for in the tree.
    """
    if name.startswith("__vt__") or name.startswith("__RTTI__"):
        return None, parse_class(name.split("__", 2)[-1])
    # Scan `__` separators right to left for one followed by a class name.
    idx = name.rfind("__")
    while idx > 0:
        cls = parse_class(name[idx + 2 :])
        if cls:
            member = name[:idx]
            return (member or None), cls
        idx = name.rfind("__", 0, idx)
    # No class part: a free function, `name__Fargs`.
    member = name.split("__", 1)[0]
    return (member or None), None


def source_name(member):
    """None for a mangled tag that no source identifier corresponds to."""
    if member is None:
        return None
    # `__ct`/`__dt`/`__as`/`__amu`/`__vc`/... are structors and operators.
    if member.startswith("__"):
        return None
    return member


def parse_class(rest):
    """Leading class name of a mangled suffix: `<len><name>` or `Q<n>...`."""
    m = re.match(r"Q(\d)(\d+)(.+)", rest)
    if m:  # nested: take the innermost component, which is what sources name
        rest = rest[2:]
        last = None
        while True:
            m2 = re.match(r"(\d+)(.+)", rest)
            if not m2:
                break
            n = int(m2.group(1))
            if len(m2.group(2)) < n:
                break
            last = m2.group(2)[:n]
            rest = m2.group(2)[n:]
        return last
    m = re.match(r"(\d+)(.+)", rest)
    if m and len(m.group(2)) >= int(m.group(1)):
        return m.group(2)[: int(m.group(1))]
    return None


IDENT_RE = re.compile(r"[A-Za-z_]\w*")


def source_identifiers(dirs):
    """Every identifier appearing anywhere in our headers and sources."""
    idents = set()
    for d in dirs:
        for root, _dirs, files in os.walk(d):
            for f in files:
                if not f.endswith((".h", ".hpp", ".c", ".cpp")):
                    continue
                with open(os.path.join(root, f), errors="replace") as fp:
                    idents.update(IDENT_RE.findall(fp.read()))
    return idents


def owner(name):
    """Class name an MWCC mangled symbol belongs to, for grouping."""
    return split_mangled(name)[1] or "(free functions and data)"


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument(
        "--map",
        default="orig/GMSE01/files/marioUS.MAP",
        help="original linker map (default: %(default)s)",
    )
    ap.add_argument(
        "--objdir",
        default="build/GMSE01/src",
        help="directory of our compiled objects (default: %(default)s)",
    )
    ap.add_argument(
        "--nm",
        default=os.environ.get("NM", "powerpc-eabi-nm"),
        help="nm binary (default: $NM or powerpc-eabi-nm)",
    )
    ap.add_argument(
        "--filter",
        default=None,
        help="only report symbols whose map source file matches this substring",
    )
    ap.add_argument(
        "--only",
        choices=("nobody", "inlined", "NOBODY", "INLINED"),
        default=None,
        help="report only one class of defect (see the output headings)",
    )
    args = ap.parse_args()

    if not os.path.exists(args.map):
        sys.stderr.write("error: map not found: %s\n" % args.map)
        return 2
    weak = read_map(args.map)
    if not weak:
        sys.stderr.write("error: no weak symbols parsed from %s\n" % args.map)
        return 2

    defined = read_objects(args.objdir, args.nm)
    if defined is None:
        sys.stderr.write(
            "error: no objects under %s -- build first\n" % args.objdir
        )
        return 2

    idents = source_identifiers(["include", "src"])

    # Two very different defects hide behind "our objects define no copy":
    #  * NOBODY  - the member does not exist in our tree at all.  This is the
    #    link hazard of "Header round 16": for a class whose virtuals are all
    #    inline, one missing body externalises the whole vtable.
    #  * INLINED - we do have the body, in a header, and MWCC inlined it at
    #    every one of our call sites while retail kept one out-of-line copy.
    #    Not a link hazard; it is evidence that some caller in the naming TU
    #    refused the inline (validate-symbol-order reports it there as
    #    MISSING, and the map address says which caller).
    groups = {"NOBODY": {}, "INLINED": {}}
    for name, (lib, src) in sorted(weak.items()):
        if name in defined:
            continue
        if args.filter and args.filter not in src and args.filter not in lib:
            continue
        member, cls_raw = split_mangled(name)
        member = source_name(member)
        cls = cls_raw or "(free functions and data)"
        # A local class carries its `$NNNN<file>_cpp` suffix and a template
        # instantiation its `<args>`; the source names neither.
        cls_src = re.split(r"[$<]", cls_raw)[0] if cls_raw else None
        known = (member in idents) if member else (cls_src in idents)
        kind = "INLINED" if known else "NOBODY"
        if args.only and args.only.upper() != kind:
            continue
        groups[kind].setdefault(cls, []).append((name, lib, src))

    print(
        "%d weak symbols in the map, %d symbols defined by %s"
        % (len(weak), len(defined), args.objdir)
    )
    total = sum(len(v) for g in groups.values() for v in g.values())
    if not total:
        print("all weak map symbols are defined somewhere: OK")
        return 0

    for kind, blurb in (
        ("NOBODY", "no body anywhere in our tree -- LINK HAZARD"),
        ("INLINED", "body exists in a header; retail kept an out-of-line copy"),
    ):
        missing = groups[kind]
        if not missing:
            continue
        n = sum(len(v) for v in missing.values())
        print("\n== %s: %d symbol(s), %d group(s) -- %s\n" % (kind, n, len(missing), blurb))
        for cls in sorted(missing):
            print("%s" % cls)
            for name, lib, src in missing[cls]:
                print("    %-62s %s %s" % (name, lib, src))
            print("")
    return 1


if __name__ == "__main__":
    sys.exit(main())
