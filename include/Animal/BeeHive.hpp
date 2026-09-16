#ifndef ANIMAL_BEEHIVE_HPP
#define ANIMAL_BEEHIVE_HPP

#include <Strategic/Nerve.hpp>

class TLiveActor;

DECLARE_NERVE(TNerveBeeHiveAttack, TLiveActor)
DECLARE_NERVE(TNerveBeeHiveBreak, TLiveActor)
DECLARE_NERVE(TNerveBeeHiveFall, TLiveActor)
DECLARE_NERVE(TNerveBeeHiveMarioWaterIn, TLiveActor)
DECLARE_NERVE(TNerveBeeHiveReset, TLiveActor)
DECLARE_NERVE(TNerveBeeHiveWait, TLiveActor)

#endif
