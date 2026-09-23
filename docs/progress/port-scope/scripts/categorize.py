#!/usr/bin/env python3
import os, re, sys, json, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import parse
Q = lambda m: m.replace('‘', "'").replace('’', "'")
INTR = r"'__(cntlzw|frsqrte|fres|fabsf?|fnabs|fsel|psq_\w+|dcb\w+|sync|isync|lhbrx|lwbrx|sthbrx|stwbrx|rlwimi|rlwinm|mftb|construct_array)'"
RULES = [
 ('case-include',   lambda m, s: 'No such file or directory' in m),
 ('declspec',       lambda m, s: '__declspec' in s),
 ('asm-function',   lambda m, s: re.match(r'((static|inline|extern)\s+)*asm\b', s) is not None),
 ('ppc-intrinsic',  lambda m, s: re.search(INTR, m) is not None),
 ('ptr-truncation', lambda m, s: 'loses precision' in m or 'of different size' in m),
 ('gnu-keyword',    lambda m, s: re.search(r'\btypeof\b', s) is not None),
 ('msl-cascade',    lambda m, s: re.search(r"'(memcpy|memset|memcmp|strlen|strcpy|__fill_mem)' was not declared", m) is not None),
 ('libc-decl',      lambda m, s: re.search(r"'(std::)?(sqrtf|powf|atan2f|fmodf|sinf|cosf|tanf|fabsf|floorf|ceilf|acosf|asinf|atanf|expf|logf|log10f|tolower|toupper|isdigit|isspace|isalpha|snprintf|sprintf|vsnprintf|vsprintf|printf|sscanf|strlen|strcpy|strncpy|strcmp|strncmp|strcat|strchr|strrchr|strstr|memmove|abs|fabs|sqrt|atan2|atan|sin|cos|tan|pow|floor|ceil|fmod|exp|log|acos|asin|atoi|atof|strtol|strtoul|rand|srand|qsort|va_start|va_end|va_arg|DEG_TO_RAD|RAD_TO_DEG|TAU|M_PI)' (is not a member of 'std'|was not declared)|no arguments to '(sqrtf|powf|atan2f|fmodf|sinf|cosf|tanf|fabsf|floorf|ceilf|acosf|asinf|atanf|expf|logf|log10f|tolower|toupper|isdigit|isspace|isalpha|snprintf|sprintf|vsnprintf|vsprintf|printf|sscanf|strlen|strcpy|strncpy|strcmp|strncmp|strcat|strchr|strrchr|strstr|memmove|abs|fabs|sqrt|atan2|atan|sin|cos|tan|pow|floor|ceil|fmod|exp|log|acos|asin|atoi|atof|strtol|strtoul|rand|srand|qsort|va_start|va_end|va_arg|DEG_TO_RAD|RAD_TO_DEG|TAU|M_PI)' that depend", m) is not None),
 ('explicit-msl-include', lambda m, s: False),
 ('host-libc-clash', lambda m, s: 'conflicts with a previous declaration' in m or re.search(r'\b(EOF|SEEK_SET|SEEK_CUR|SEEK_END|BUFSIZ|NULL)\b\s*=', s) is not None),
 ('missing-typename', lambda m, s: "need 'typename'" in m or 'dependent scope' in m),
 ('two-phase-lookup', lambda m, s: 'argument-dependent lookup' in m or 'depend on a template parameter' in m),
 ('size_t-width',   lambda m, s: "takes type 'size_t'" in m or "takes type 'std::size_t'" in m),
 ('ptr-truncation', lambda m, s: 'loses precision' in m or 'of different size' in m),
 ('u32-long-spelling', lambda m, s: 'no declaration matches' in m or 'conflicting return type specified' in m or 'invalid new-expression of abstract class' in m or 'no matching function for call' in m or 'cannot bind rvalue' in m),
 ('ptr-width-flags', lambda m, s: 'right operand of shift expression' in m or 'is not an integer constant' in m),
 ('implicit-conv',  lambda m, s: 'cannot convert' in m or 'invalid conversion' in m),
 ('ref-binding',    lambda m, s: 'cannot bind non-const lvalue reference' in m or 'discards qualifiers' in m),
 ('jump-over-init', lambda m, s: 'jump to case label' in m or 'jump to label' in m or 'crosses initialization' in m),
 ('incomplete-or-undeclared-type', lambda m, s: 'incomplete type' in m or 'has not been declared' in m),
 ('access-control', lambda m, s: 'within this context' in m),
 ('conflicting-redecl', lambda m, s: 'ambiguating new declaration' in m or 'conflicting declaration' in m or 'conflicting types' in m),
 ('overload-ambiguity', lambda m, s: 'is ambiguous' in m),
 ('main-signature', lambda m, s: "'::main' must return" in m),
]
def classify(e, tu_errs):
    f, l, c, kind, msg = e
    m, s = Q(msg), parse.srcline(f, l)
    if 'PowerPC_EABI_Support' in f and not any('PowerPC_EABI_Support' in x for x in sys.argv[1:2]) and PASS_HOST[0]:
        return 'explicit-msl-include'
    for name, fn in RULES:
        if fn(m, s): return name
    mm = re.search(r"'(\w+)' was not declared", m)
    if mm and any(mm.group(1) in Q(x[4]) and 'ambiguating' in x[4] for x in tu_errs): return 'conflicting-redecl'
    return 'other'
def area(tu):
    top = tu.split('/')[1]
    return {'JSystem': 'jsystem', 'THPPlayer': 'thpplayer', 'OdemuExi2': 'odemuexi2'}.get(top, 'game')
PASS_HOST = [False]
def analyse(pas):
    PASS_HOST[0] = pas in ('host64', 'compat64')
    res, errs = parse.load(pas)
    per_tu = collections.defaultdict(list)
    for e in errs: per_tu[e[0]].append(e[1:6])
    uniq = collections.defaultdict(set)
    cat_of = {}
    for tu, es in per_tu.items():
        for e in es:
            uniq[e].add(tu)
            if e not in cat_of: cat_of[e] = classify(e, es)
    cats = collections.defaultdict(lambda: dict(unique=0, raw=0, tus=set(), ex=[]))
    for e, tus in uniq.items():
        k = cats[cat_of[e]]; k['unique'] += 1; k['raw'] += len(tus); k['tus'] |= tus; k['ex'].append((len(tus), e))
    tu_cats = {tu: set(cat_of[e] for e in es) for tu, es in per_tu.items()}
    return res, per_tu, uniq, cat_of, cats, tu_cats
if __name__ == '__main__':
    pas = sys.argv[1]
    res, per_tu, uniq, cat_of, cats, tu_cats = analyse(pas)
    n = len(res); ok = sum(r['rc'] == 0 for r in res)
    byarea = collections.Counter(area(r['path']) for r in res); okarea = collections.Counter(area(r['path']) for r in res if r['rc'] == 0)
    print('%s: %d/%d clean (%.1f%%)' % (pas, ok, n, 100.0 * ok / n), {a: '%d/%d' % (okarea[a], byarea[a]) for a in byarea})
    nofile = [r['path'] for r in res if r['rc'] != 0 and r['path'] not in per_tu]
    if nofile: print('nonzero rc without parsed errors:', nofile[:5])
    for k, v in sorted(cats.items(), key=lambda kv: -len(kv[1]['tus'])):
        ex = sorted(v['ex'], key=lambda x: -x[0])[:2]
        print('  %-30s unique %4d raw %5d TUs %3d | %s' % (k, v['unique'], v['raw'], len(v['tus']), ' ; '.join('%s:%d %s' % (e[1][0], e[1][1], Q(e[1][4])[:60]) for e in ex)))
    if len(sys.argv) > 2:
        for e, c in cat_of.items():
            if c == 'other': print('   OTHER %3d %s:%d %s | %s' % (len(uniq[e]), e[0], e[1], Q(e[4])[:90], parse.srcline(e[0], e[1])[:60]))
