#include "Enemy/PathNode.hpp"
#include "Strategic/SolidStack.hpp"
#include <NPC/NpcNerve.hpp>

DEFINE_NERVE(TNerveNPCGraphWander, TLiveActor) { }

DEFINE_NERVE(TNerveNPCUTurn, TLiveActor) { }

DEFINE_NERVE(TNerveNPCGraphWait, TLiveActor) { }

DEFINE_NERVE(TNerveNPCWaitContinue, TLiveActor) { }

DEFINE_NERVE(TNerveNPCWaitMarioApproach, TLiveActor) { }

DEFINE_NERVE(TNerveNPCTurnToMario, TLiveActor) { }

DEFINE_NERVE(TNerveNPCWet, TLiveActor) { }

DEFINE_NERVE(TNerveNPCSink, TLiveActor) { }

DEFINE_NERVE(TNerveNPCRecoverFromSink, TLiveActor) { }

DEFINE_NERVE(TNerveNPCRecoverAfter, TLiveActor) { }

DEFINE_NERVE(TNerveNPCSetPosAfterSinkBottom, TLiveActor) { }

DEFINE_NERVE(TNerveNPCTalk, TLiveActor) { }

DEFINE_NERVE(TNerveNPCThrow, TLiveActor) { }

DEFINE_NERVE(TNerveNPCMad, TLiveActor) { }

DEFINE_NERVE(TNerveNPCBlown, TLiveActor) { }

DEFINE_NERVE(TNerveNPCMareStand, TLiveActor) { }
