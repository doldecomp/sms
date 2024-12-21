

#ifndef _DOLPHIN_GD_BASE_H
#define _DOLPHIN_GD_BASE_H

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif // ifdef __cplusplus

// TODO: this is part of the SDK and so parts are probably present in other decomps, steal them

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