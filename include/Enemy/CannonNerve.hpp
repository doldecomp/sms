#ifndef ENEMY_CANNONNERVE_HPP
#define ENEMY_CANNONNERVE_HPP

#include <Strategic/Nerve.hpp>

class TLiveActor;

DECLARE_NERVE(TNerveCannonClose, TLiveActor)
DECLARE_NERVE(TNerveCannonDamage, TLiveActor)
DECLARE_NERVE(TNerveCannonDamageDemo, TLiveActor)
DECLARE_NERVE(TNerveCannonForceBombShoot, TLiveActor)
DECLARE_NERVE(TNerveCannonObject, TLiveActor)
DECLARE_NERVE(TNerveCannonOpen, TLiveActor)
DECLARE_NERVE(TNerveCannonSearch, TLiveActor)
DECLARE_NERVE(TNerveCannonShoot, TLiveActor)

#endif
