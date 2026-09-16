#ifndef ENEMY_YUNBO_HPP
#define ENEMY_YUNBO_HPP

#include <Strategic/Nerve.hpp>

class TLiveActor;

DECLARE_NERVE(TNerveYumboAppearing, TLiveActor)
DECLARE_NERVE(TNerveYumboAttack, TLiveActor)
DECLARE_NERVE(TNerveYumboDancing, TLiveActor)
DECLARE_NERVE(TNerveYumboFreeze, TLiveActor)
DECLARE_NERVE(TNerveYumboHiding, TLiveActor)

#endif
