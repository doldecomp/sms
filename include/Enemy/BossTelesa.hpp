#ifndef ENEMY_BOSSTELESA_HPP
#define ENEMY_BOSSTELESA_HPP

#include <Strategic/Nerve.hpp>

class TLiveActor;

DECLARE_NERVE(TNerveBossTelesaAppear, TLiveActor)
DECLARE_NERVE(TNerveBossTelesaDie, TLiveActor)
DECLARE_NERVE(TNerveBossTelesaFallDemo, TLiveActor)
DECLARE_NERVE(TNerveBossTelesaFreeze, TLiveActor)
DECLARE_NERVE(TNerveBossTelesaHide, TLiveActor)
DECLARE_NERVE(TNerveBossTelesaHideWait, TLiveActor)
DECLARE_NERVE(TNerveBossTelesaPrepareSlot, TLiveActor)
DECLARE_NERVE(TNerveBossTelesaSlotStart, TLiveActor)
DECLARE_NERVE(TNerveBossTelesaSpit, TLiveActor)
DECLARE_NERVE(TNerveBossTelesaSpitSlotItem, TLiveActor)
DECLARE_NERVE(TNerveBubbleLive, TLiveActor)
DECLARE_NERVE(TNerveBubbleSplit, TLiveActor)

#endif
