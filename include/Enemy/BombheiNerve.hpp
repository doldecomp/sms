#ifndef ENEMY_BOMBHEINERVE_HPP
#define ENEMY_BOMBHEINERVE_HPP

#include <Strategic/Nerve.hpp>

class TLiveActor;

DECLARE_NERVE(TNerveBombHeiAttack, TLiveActor)
DECLARE_NERVE(TNerveBombHeiExplosion, TLiveActor)
DECLARE_NERVE(TNerveBombHeiGenerate, TLiveActor)
DECLARE_NERVE(TNerveBombHeiPickUp, TLiveActor)
DECLARE_NERVE(TNerveBombHeiThrown, TLiveActor)
DECLARE_NERVE(TNerveBombHeiWaitExplosion, TLiveActor)
DECLARE_NERVE(TNerveBombHeiWalkExplosion, TLiveActor)

#endif
