#include <Enemy/BossPakkun.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/Spine.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// TODO: none of these nerve bodies is reconstructed. Each carries the size the
// map records for its execute. Defining them does emit theNerve() and the
// destructor, both compiler-generated, which is what matches so far.

// TODO: incorrect size. Map records 3088 bytes.
DEFINE_NERVE(TNerveBPWait, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 292 bytes.
DEFINE_NERVE(TNerveBPCannon, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 864 bytes.
DEFINE_NERVE(TNerveBPVomit, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 380 bytes.
DEFINE_NERVE(TNerveBPTornado, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 344 bytes.
DEFINE_NERVE(TNerveBPPivot, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 496 bytes.
DEFINE_NERVE(TNerveBPSwallow, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 340 bytes.
DEFINE_NERVE(TNerveBPTumbleIn, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 376 bytes.
DEFINE_NERVE(TNerveBPTumble, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 628 bytes.
DEFINE_NERVE(TNerveBPTumbleOut, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 184 bytes.
DEFINE_NERVE(TNerveBPGetUp, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 176 bytes.
DEFINE_NERVE(TNerveBPSwing, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 316 bytes.
DEFINE_NERVE(TNerveBPStompReact, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 100 bytes.
DEFINE_NERVE(TNerveBPJumpReact, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 472 bytes.
DEFINE_NERVE(TNerveBPPreDie, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 280 bytes.
DEFINE_NERVE(TNerveBPDie, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 552 bytes.
DEFINE_NERVE(TNerveBPTakeOff, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 716 bytes.
DEFINE_NERVE(TNerveBPFly, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 400 bytes.
DEFINE_NERVE(TNerveBPTouchDown, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 148 bytes.
DEFINE_NERVE(TNerveBPFlyCannon, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 172 bytes.
DEFINE_NERVE(TNerveBPFlyPivot, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 904 bytes.
DEFINE_NERVE(TNerveBPHover, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 1308 bytes.
DEFINE_NERVE(TNerveBPFall, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 56 bytes.
DEFINE_NERVE(TNerveBPSleep, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 244 bytes.
DEFINE_NERVE(TNerveBPBreakSleep, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 564 bytes.
DEFINE_NERVE(TNerveBPWaitL, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 312 bytes.
DEFINE_NERVE(TNerveBPCannonL, TLiveActor) { return FALSE; }

DEFINE_NERVE(TNervePakkunGenerate, TLiveActor) { return FALSE; }

DEFINE_NERVE(TNervePakkunStay, TLiveActor) { return FALSE; }

DEFINE_NERVE(TNervePakkunAppear, TLiveActor) { return FALSE; }

DEFINE_NERVE(TNervePakkunHide, TLiveActor) { return FALSE; }

DEFINE_NERVE(TNervePakkunShoot, TLiveActor) { return FALSE; }

DEFINE_NERVE(TNervePakkunFreeze, TLiveActor) { return FALSE; }

DEFINE_NERVE(TNerveStayPakkunHide, TLiveActor) { return FALSE; }

DEFINE_NERVE(TNerveStayPakkunAppear, TLiveActor) { return FALSE; }
