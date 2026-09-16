#ifndef ENEMY_WIRETRAP_HPP
#define ENEMY_WIRETRAP_HPP

#include <Strategic/Nerve.hpp>

class TLiveActor;

DECLARE_NERVE(TNerveWireTrapGoWait, TLiveActor)
DECLARE_NERVE(TNerveWireTrapOnewayMove, TLiveActor)
DECLARE_NERVE(TNerveWireTrapOnewayMoveEnd, TLiveActor)
DECLARE_NERVE(TNerveWireTrapOnewayMoveStart, TLiveActor)
DECLARE_NERVE(TNerveWireTrapReturnMove, TLiveActor)
DECLARE_NERVE(TNerveWireTrapSearch, TLiveActor)
DECLARE_NERVE(TNerveWireTrapWait, TLiveActor)

#endif
