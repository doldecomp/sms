#ifndef ENEMY_POPO_HPP
#define ENEMY_POPO_HPP

#include <Strategic/Nerve.hpp>

class TLiveActor;

DECLARE_NERVE(TNervePopoAttack, TLiveActor)
DECLARE_NERVE(TNervePopoExplosion, TLiveActor)
DECLARE_NERVE(TNervePopoFly, TLiveActor)
DECLARE_NERVE(TNervePopoPossessedNozzle, TLiveActor)
DECLARE_NERVE(TNervePopoThrown, TLiveActor)
DECLARE_NERVE(TNervePopoWait, TLiveActor)

#endif
