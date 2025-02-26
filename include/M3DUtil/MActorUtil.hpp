#ifndef M3DUTIL_M_ACTOR_UTIL_HPP
#define M3DUTIL_M_ACTOR_UTIL_HPP

#include <dolphin/types.h>

class MActorAnmData;
class SDLModelData;

void SMS_MakeMActor(const char*, const char*, u32, u32);
void SMS_MakeMActorWithAnmData(const char*, MActorAnmData*, u32, u32);
void SMS_MakeMActorsWithAnmData(const char*, MActorAnmData*, int, u32, u32);
void SMS_MakeSDLModelData(const char*, u32);
void SMS_MakeMActorFromSDLModelData(SDLModelData*, MActorAnmData*, u32);

#endif
