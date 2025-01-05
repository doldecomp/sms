#ifndef _WCHAR_H_
#define _WCHAR_H_

#include <stdio.h>

#ifndef __cplusplus
typedef unsigned short wchar_t;
#endif

int fwide(FILE* stream, int mode);

#endif
