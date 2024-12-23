#ifndef _DOLPHIN_LIBC_CTYPE_H_
#define _DOLPHIN_LIBC_CTYPE_H_

#include <types.h>


#ifdef __cplusplus
extern "C"
{
#endif // ifdef __cplusplus

int tolower(int c);

extern unsigned char __lower_map[256];

inline int _tolower(int c) { return (c == -1 ? -1 : (int)__lower_map[(u8)c]); }

#ifdef __cplusplus
};
#endif // ifdef __cplusplus

#endif // _DOLPHIN_LIBC_CTYPE_H_
