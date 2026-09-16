#ifndef ENEMY_LIMITKOOPA_HPP
#define ENEMY_LIMITKOOPA_HPP

#include <Strategic/Nerve.hpp>

class TLiveActor;

DECLARE_NERVE(TNerveLimitKoopaGetDown, TLiveActor)
DECLARE_NERVE(TNerveLimitKoopaGetShowered, TLiveActor)
DECLARE_NERVE(TNerveLimitKoopaHipDropJump, TLiveActor)
DECLARE_NERVE(TNerveLimitKoopaHipDropStart, TLiveActor)
DECLARE_NERVE(TNerveLimitKoopaStagger, TLiveActor)
DECLARE_NERVE(TNerveLimitKoopaTumble, TLiveActor)
DECLARE_NERVE(TNerveLimitKoopaTurn, TLiveActor)
DECLARE_NERVE(TNerveLimitKoopaWait, TLiveActor)

#endif
