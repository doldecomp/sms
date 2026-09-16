#ifndef ANIMAL_BIRDNERVE_HPP
#define ANIMAL_BIRDNERVE_HPP

#include <Strategic/Nerve.hpp>

class TLiveActor;

DECLARE_NERVE(TNerveAnimalBirdActionOnGround, TLiveActor)
DECLARE_NERVE(TNerveAnimalBirdChangeToCoin, TLiveActor)
DECLARE_NERVE(TNerveAnimalBirdComeback, TLiveActor)
DECLARE_NERVE(TNerveAnimalBirdGraphWander, TLiveActor)
DECLARE_NERVE(TNerveAnimalBirdLanding, TLiveActor)
DECLARE_NERVE(TNerveAnimalBirdPreLanding, TLiveActor)
DECLARE_NERVE(TNerveAnimalBirdTakeoff, TLiveActor)
DECLARE_NERVE(TNerveAnimalBirdWaitOnGround, TLiveActor)
DECLARE_NERVE(TNerveAnimalBirdWalkOnGround, TLiveActor)

#endif
