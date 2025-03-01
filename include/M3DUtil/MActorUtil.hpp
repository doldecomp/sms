#ifndef M3DUTIL_M_ACTOR_UTIL_HPP
#define M3DUTIL_M_ACTOR_UTIL_HPP

#include <dolphin/types.h>

class MActorAnmData;
class SDLModelData;
class MActor;

MActor* SMS_MakeMActorFromSDLModelData(SDLModelData*, MActorAnmData*, u32);
SDLModelData* SMS_MakeSDLModelData(const char*, u32);
MActor** SMS_MakeMActorsWithAnmData(const char*, MActorAnmData*, int, u32, u32);
MActor* SMS_MakeMActorWithAnmData(const char*, MActorAnmData*, u32, u32);
MActor** SMS_MakeMActors(const char*, const char*, int, u32, u32);
MActor* SMS_MakeMActor(const char*, const char*, u32, u32);

#endif
