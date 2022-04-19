#ifndef __MEM_H
#define __MEM_H

#include "dolphin/types.h"

#ifdef __cplusplus
extern "C" {
#endif

void* memcpy(void* dest, const void* src, u32 n);

#ifdef __cplusplus
}
#endif

#endif
