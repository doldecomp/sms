#ifndef ENEMY_KUKKUNERVE_HPP
#define ENEMY_KUKKUNERVE_HPP

#include <Strategic/Nerve.hpp>

class TLiveActor;

DECLARE_NERVE(TNerveKukkuFall, TLiveActor)
DECLARE_NERVE(TNerveKukkuGraphWander, TLiveActor)
DECLARE_NERVE(TNerveKukkuPostFall, TLiveActor)
DECLARE_NERVE(TNerveKukkuRecoverGraph, TLiveActor)

#endif
