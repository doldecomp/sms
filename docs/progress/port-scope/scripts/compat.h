/* Minimal PC compat shim used only by the scoping measurement (compat64 pass). */
#ifndef PC_SCOPE_COMPAT_H
#define PC_SCOPE_COMPAT_H
#define __declspec(x)
#define __cntlzw(x) ((unsigned int)__builtin_clz(x))
#endif
