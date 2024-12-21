#ifndef GD_BASE_H
#define GD_BASE_H

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif // ifdef __cplusplus

void GDInitGDLObj(void*, void*, u32);
void GDFlushCurrToMem();
void GDPadCurr32();
void GDOverflowed();
void GDSetOverflowCallback();
void GDGetOverflowCallback();

extern void* __GDCurrentDL;

#ifdef __cplusplus
}
#endif // ifdef __cplusplus


#endif