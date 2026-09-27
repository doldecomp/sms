#ifndef GC2D_HX_WIPER_H
#define GC2D_HX_WIPER_H

#include <dolphin/types.h>

#if __cplusplus
extern "C" {
#endif

int Hx_MovieStartSyncEx();
void Hx_MovieStartSync();
u32 Hx_UpdateWipe(f32);
int Hx_GetWipeType(int);
void Hx_StartWipe(int, int);
void Hx_RemoveResource();
void Hx_ProvideResourceEx(void*);
void Hx_ProvideResource(void*, int);
void Hx_ResetWipe(u32, u32);
void Hx_SetVFilterFade();

#if __cplusplus
}
#endif

#endif
