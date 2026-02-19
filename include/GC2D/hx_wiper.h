#ifndef GC2D_HX_WIPER_H
#define GC2D_HX_WIPER_H

#include <dolphin/types.h>

#if __cplusplus
extern "C" {
#endif

void Hx_Test5();
void Hx_Test4();
void Hx_Test2R();
void Hx_Test2();
void Hx_Test1();
int Hx_MovieStartSyncEx();
void Hx_MovieStartSync();
void Hx_Logo();
void Hx_GameOver();
void Hx_Door();
void Hx_Circle();
void Hx_MotionUpdate();
void Hx_MotionSet();
void Hx_TimerCountDown();
u32 Hx_UpdateWipe(f32);
int Hx_GetWipeType(int);
void Hx_StartWipe(int, int);
void Hx_RemoveResource();
void Hx_ProvideResourceEx(void*);
void Hx_ProvideResource(void*, int);
void Hx_ResetWipe(u32, u32);
void Hx_Warning();
void Hx_FrBufferMorf();
void Hx_SetVFilterFade();
void Hx_SetVFilter();
void Hx_GetFrBuffer();
void Hx_GxInit();
void Hx_CameraInit();

#if __cplusplus
}
#endif

#endif
