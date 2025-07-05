#ifndef NPC_NPC_NERVE_HPP
#define NPC_NPC_NERVE_HPP

#include <Strategic/Nerve.hpp>

class TLiveActor;

DECLARE_NERVE(TNerveNPCGraphWander, TLiveActor);
DECLARE_NERVE(TNerveNPCUTurn, TLiveActor);
DECLARE_NERVE(TNerveNPCGraphWait, TLiveActor);
DECLARE_NERVE(TNerveNPCWaitContinue, TLiveActor);
DECLARE_NERVE(TNerveNPCWaitMarioApproach, TLiveActor);
DECLARE_NERVE(TNerveNPCTurnToMario, TLiveActor);
DECLARE_NERVE(TNerveNPCWet, TLiveActor);
DECLARE_NERVE(TNerveNPCSink, TLiveActor);
DECLARE_NERVE(TNerveNPCRecoverFromSink, TLiveActor);
DECLARE_NERVE(TNerveNPCRecoverAfter, TLiveActor);
DECLARE_NERVE(TNerveNPCSetPosAfterSinkBottom, TLiveActor);
DECLARE_NERVE(TNerveNPCTalk, TLiveActor);
DECLARE_NERVE(TNerveNPCThrow, TLiveActor);
DECLARE_NERVE(TNerveNPCMad, TLiveActor);
DECLARE_NERVE(TNerveNPCBlown, TLiveActor);
DECLARE_NERVE(TNerveNPCMareStand, TLiveActor);

#endif
