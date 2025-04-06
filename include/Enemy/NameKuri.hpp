#ifndef ENEMY_NAME_KURI_HPP
#define ENEMY_NAME_KURI_HPP

#include <Strategic/Nerve.hpp>

class TLiveActor;

DECLARE_NERVE(TNerveNameKuriLand, TLiveActor);
DECLARE_NERVE(TNerveNameKuriJumpAttack, TLiveActor);
DECLARE_NERVE(TNerveNameKuriJumpAttackPrepare, TLiveActor);
DECLARE_NERVE(TNerveNameKuriExplosion, TLiveActor);
DECLARE_NERVE(TNerveNameKuriDiffuse, TLiveActor);
DECLARE_NERVE(TNerveNameKuriDrawPollute, TLiveActor);
DECLARE_NERVE(TNerveNKFollowMario, TLiveActor);

#endif
