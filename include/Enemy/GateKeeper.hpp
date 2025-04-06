#ifndef ENEMY_GATE_KEEPER_HPP
#define ENEMY_GATE_KEEPER_HPP

#include <Strategic/Nerve.hpp>

class TLiveActor;

DECLARE_NERVE(TNerveBGKSleep, TLiveActor);
DECLARE_NERVE(TNerveBGKAppear, TLiveActor);
DECLARE_NERVE(TNerveBGKWait, TLiveActor);
DECLARE_NERVE(TNerveBGKWait2, TLiveActor);
DECLARE_NERVE(TNerveBGKSleepDamage, TLiveActor);
DECLARE_NERVE(TNerveBGKAwakeDamage, TLiveActor);
DECLARE_NERVE(TNerveBGKDie, TLiveActor);
DECLARE_NERVE(TNerveBGKDive, TLiveActor);
DECLARE_NERVE(TNerveBGKLaunchGoro, TLiveActor);
DECLARE_NERVE(TNerveBGKLaunchName, TLiveActor);

#endif
