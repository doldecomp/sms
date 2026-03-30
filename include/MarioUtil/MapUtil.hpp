#ifndef MARIO_UTIL_MAP_UTIL_HPP
#define MARIO_UTIL_MAP_UTIL_HPP

#include <dolphin/types.h>

class TBGCheckData;
class TLiveActor;

int SMS_GetMonteVillageAreaInMario();
void* SMS_GetGroundActor(const TBGCheckData* bgData, u32 value);
float SMS_GetSandRiseUpRatio(const TLiveActor* actor);

#endif
