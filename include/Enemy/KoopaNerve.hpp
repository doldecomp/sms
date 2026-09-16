#ifndef ENEMY_KOOPANERVE_HPP
#define ENEMY_KOOPANERVE_HPP

#include <Strategic/Nerve.hpp>

class TLiveActor;

DECLARE_NERVE(TNerveKoopaFall, TLiveActor)
DECLARE_NERVE(TNerveKoopaFlame, TLiveActor)
DECLARE_NERVE(TNerveKoopaGetDown, TLiveActor)
DECLARE_NERVE(TNerveKoopaGetShowered, TLiveActor)
DECLARE_NERVE(TNerveKoopaProvoke, TLiveActor)
DECLARE_NERVE(TNerveKoopaStagger, TLiveActor)
DECLARE_NERVE(TNerveKoopaTumble, TLiveActor)
DECLARE_NERVE(TNerveKoopaTurn, TLiveActor)
DECLARE_NERVE(TNerveKoopaTurnL, TLiveActor)
DECLARE_NERVE(TNerveKoopaTurnR, TLiveActor)
DECLARE_NERVE(TNerveKoopaWait, TLiveActor)

#endif
