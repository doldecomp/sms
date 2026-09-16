#ifndef ENEMY_ELECNOKONOKO_HPP
#define ENEMY_ELECNOKONOKO_HPP

#include <Strategic/Nerve.hpp>

class TLiveActor;

DECLARE_NERVE(TNerveElecCarapaceMove, TLiveActor)
DECLARE_NERVE(TNerveElecCarapaceReturn, TLiveActor)
DECLARE_NERVE(TNerveElecCarapaceWait, TLiveActor)
DECLARE_NERVE(TNerveElecNokonokoAttack, TLiveActor)
DECLARE_NERVE(TNerveElecNokonokoCollect, TLiveActor)
DECLARE_NERVE(TNerveElecNokonokoFreeze, TLiveActor)
DECLARE_NERVE(TNerveElecNokonokoRebirth, TLiveActor)
DECLARE_NERVE(TNerveElecNokonokoShoot, TLiveActor)
DECLARE_NERVE(TNerveElecNokonokoTurn, TLiveActor)

#endif
