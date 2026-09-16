#ifndef ENEMY_TABEPUKUNERVE_HPP
#define ENEMY_TABEPUKUNERVE_HPP

#include <Strategic/Nerve.hpp>

class TLiveActor;

DECLARE_NERVE(TNerveTabePukuAttack, TLiveActor)
DECLARE_NERVE(TNerveTabePukuBite, TLiveActor)
DECLARE_NERVE(TNerveTabePukuDive, TLiveActor)
DECLARE_NERVE(TNerveTabePukuDrag, TLiveActor)
DECLARE_NERVE(TNerveTabePukuFound, TLiveActor)
DECLARE_NERVE(TNerveTabePukuGraphWander, TLiveActor)
DECLARE_NERVE(TNerveTabePukuRecoverGraph, TLiveActor)

#endif
