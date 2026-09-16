#ifndef ENEMY_ROCKETNERVE_HPP
#define ENEMY_ROCKETNERVE_HPP

#include <Strategic/Nerve.hpp>

class TLiveActor;

DECLARE_NERVE(TNerveRocketFly, TLiveActor)
DECLARE_NERVE(TNerveRocketPossessedNozzle, TLiveActor)
DECLARE_NERVE(TNerveRocketWait, TLiveActor)

#endif
