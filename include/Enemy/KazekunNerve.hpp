#ifndef ENEMY_KAZEKUNNERVE_HPP
#define ENEMY_KAZEKUNNERVE_HPP

#include <Strategic/Nerve.hpp>

class TLiveActor;

DECLARE_NERVE(TNerveKazekunAppear, TLiveActor)
DECLARE_NERVE(TNerveKazekunAttack, TLiveActor)
DECLARE_NERVE(TNerveKazekunDisappear, TLiveActor)
DECLARE_NERVE(TNerveKazekunHitWater, TLiveActor)
DECLARE_NERVE(TNerveKazekunPreAttack, TLiveActor)
DECLARE_NERVE(TNerveKazekunSearch, TLiveActor)
DECLARE_NERVE(TNerveKazekunTurn, TLiveActor)
DECLARE_NERVE(TNerveKazekunWait, TLiveActor)

#endif
