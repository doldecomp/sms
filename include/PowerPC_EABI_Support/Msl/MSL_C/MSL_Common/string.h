#ifndef _MSL_COMMON_STRING_H
#define _MSL_COMMON_STRING_H

#include "stddef.h"

#ifdef __cplusplus
extern "C" {
#endif

__declspec(section ".init") void* memcpy(void* dest, const void* src, size_t n);
__declspec(section ".init") void __fill_mem(void* dest, int val, size_t count);
/* The section attribute is right (the map puts memset at 0x80003100) and it is
 * not what keeps memset out of line: MWCC 1.2.5 emits a `bl memset` with or
 * without it, probed both ways. Game code that shows an inlined zero-fill loop
 * (TGCConsole2::perform) wrote the loop by hand; do not chase it here. */
__declspec(section ".init") void* memset(void* dest, int val, size_t count);

int memcmp(const void* lhs, const void* rhs, size_t count);
void* __memrchr(const void* ptr, int ch, size_t count);
void* memchr(const void* ptr, int ch, size_t count);
void* memmove(void* dst, const void* src, size_t n);
char* strrchr(const char* str, int c);
char* strchr(const char* str, int c);
int strncmp(const char* str1, const char* str2, size_t n);
int strcmp(const char* str1, const char* str2);
char* strcat(char* dst, const char* src);
char* strncpy(char* dst, const char* src, size_t n);
char* strcpy(char* dst, const char* src);
size_t strlen(const char* str);
char* strstr(const char* str, const char* pat);

#ifdef __cplusplus
};

namespace std {
using ::strlen;
using ::strrchr;
}; // namespace std
#endif

#endif /* _MSL_COMMON_STRING_H */
