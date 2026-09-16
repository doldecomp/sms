#ifndef ENEMY_AMINOKO_HPP
#define ENEMY_AMINOKO_HPP

#include <Strategic/Nerve.hpp>

class TLiveActor;

DECLARE_NERVE(TNerveAmiNokoDie, TLiveActor)
DECLARE_NERVE(TNerveAmiNokoFreeze, TLiveActor)
DECLARE_NERVE(TNerveAmiNokoTurn, TLiveActor)
DECLARE_NERVE(TNerveAmiNokoWalkOnFence, TLiveActor)

#endif
