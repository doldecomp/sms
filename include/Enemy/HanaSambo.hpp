#ifndef ENEMY_HANASAMBO_HPP
#define ENEMY_HANASAMBO_HPP

#include <Strategic/Nerve.hpp>

class TLiveActor;

DECLARE_NERVE(TNerveHanaSamboAppear, TLiveActor)
DECLARE_NERVE(TNerveHanaSamboAttack, TLiveActor)
DECLARE_NERVE(TNerveHanaSamboDie, TLiveActor)
DECLARE_NERVE(TNerveHanaSamboFreeze, TLiveActor)
DECLARE_NERVE(TNerveHanaSamboHide, TLiveActor)
DECLARE_NERVE(TNerveHanaSamboWait, TLiveActor)
DECLARE_NERVE(TNerveSamboHeadAppear, TLiveActor)
DECLARE_NERVE(TNerveSamboHeadAttack, TLiveActor)
DECLARE_NERVE(TNerveSamboHeadHide, TLiveActor)
DECLARE_NERVE(TNerveSamboHeadHitWall, TLiveActor)
DECLARE_NERVE(TNerveSamboHeadHitWater, TLiveActor)
DECLARE_NERVE(TNerveSamboHeadRecoverWater, TLiveActor)

#endif
