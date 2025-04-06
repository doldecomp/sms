#ifndef ENEMY_BOSS_GESSO_HPP
#define ENEMY_BOSS_GESSO_HPP

#include <Strategic/Nerve.hpp>

class TLiveActor;

DECLARE_NERVE(TNerveBGWait, TLiveActor);
DECLARE_NERVE(TNerveBGEyeDamage, TLiveActor);
DECLARE_NERVE(TNerveBGBeakDamage, TLiveActor);
DECLARE_NERVE(TNerveBGTentacleDamage, TLiveActor);
DECLARE_NERVE(TNerveBGTug, TLiveActor);
DECLARE_NERVE(TNerveBGDie, TLiveActor);
DECLARE_NERVE(TNerveBGPollute, TLiveActor);
DECLARE_NERVE(TNerveBGPolDrop, TLiveActor);
DECLARE_NERVE(TNerveBGRoll, TLiveActor);

#endif
