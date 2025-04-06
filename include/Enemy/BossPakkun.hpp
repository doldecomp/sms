#ifndef ENEMY_BOSS_PAKKUN_HPP
#define ENEMY_BOSS_PAKKUN_HPP

#include <Strategic/Nerve.hpp>

class TLiveActor;

DECLARE_NERVE(TNerveBPWait, TLiveActor);
DECLARE_NERVE(TNerveBPCannon, TLiveActor);
DECLARE_NERVE(TNerveBPVomit, TLiveActor);
DECLARE_NERVE(TNerveBPTornado, TLiveActor);
DECLARE_NERVE(TNerveBPPivot, TLiveActor);
DECLARE_NERVE(TNerveBPSwallow, TLiveActor);
DECLARE_NERVE(TNerveBPTumbleIn, TLiveActor);
DECLARE_NERVE(TNerveBPTumble, TLiveActor);
DECLARE_NERVE(TNerveBPTumbleOut, TLiveActor);
DECLARE_NERVE(TNerveBPGetUp, TLiveActor);
DECLARE_NERVE(TNerveBPSwing, TLiveActor);
DECLARE_NERVE(TNerveBPStompReact, TLiveActor);
DECLARE_NERVE(TNerveBPJumpReact, TLiveActor);
DECLARE_NERVE(TNerveBPPreDie, TLiveActor);
DECLARE_NERVE(TNerveBPDie, TLiveActor);
DECLARE_NERVE(TNerveBPTakeOff, TLiveActor);
DECLARE_NERVE(TNerveBPFly, TLiveActor);
DECLARE_NERVE(TNerveBPTouchDown, TLiveActor);
DECLARE_NERVE(TNerveBPFlyCannon, TLiveActor);
DECLARE_NERVE(TNerveBPFlyPivot, TLiveActor);
DECLARE_NERVE(TNerveBPHover, TLiveActor);
DECLARE_NERVE(TNerveBPFall, TLiveActor);
DECLARE_NERVE(TNerveBPSleep, TLiveActor);
DECLARE_NERVE(TNerveBPBreakSleep, TLiveActor);
DECLARE_NERVE(TNerveBPWaitL, TLiveActor);
DECLARE_NERVE(TNerveBPCannonL, TLiveActor);
DECLARE_NERVE(TNervePakkunGenerate, TLiveActor);
DECLARE_NERVE(TNervePakkunStay, TLiveActor);
DECLARE_NERVE(TNervePakkunAppear, TLiveActor);
DECLARE_NERVE(TNervePakkunHide, TLiveActor);
DECLARE_NERVE(TNervePakkunShoot, TLiveActor);
DECLARE_NERVE(TNervePakkunFreeze, TLiveActor);
DECLARE_NERVE(TNerveStayPakkunHide, TLiveActor);
DECLARE_NERVE(TNerveStayPakkunAppear, TLiveActor);

#endif
