#ifndef MARIO_UTIL_MAP_UTIL_HPP
#define MARIO_UTIL_MAP_UTIL_HPP

#include <dolphin/types.h>

class TBGCheckData;
class TLiveActor;

int SMS_GetMonteVillageAreaInMario();
const TLiveActor* SMS_GetGroundActor(const TBGCheckData* check_data,
                                     u32 type_filter);
float SMS_GetSandRiseUpRatio(const TLiveActor* actor);

#endif
