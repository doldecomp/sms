#ifndef _STDDEF_H_
#define _STDDEF_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned long size_t;
typedef long ptrdiff_t;

#ifdef __clang__
#define offsetof(type, member) __builtin_offsetof(type, member)
#else
#define offsetof(type, member) ((size_t)&(((type*)0)->member))
#endif

#ifndef NULL
#define NULL 0
#endif

#ifdef __cplusplus
};
#endif

#endif
