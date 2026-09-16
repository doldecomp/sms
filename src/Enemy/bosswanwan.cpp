#include <Enemy/BossWanwan.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/Spine.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// TODO: no nerve body below is reconstructed; each carries its map size.
// Defining them emits theNerve() and the destructor, which are
// compiler-generated and do match.

// TODO: incorrect size. Map records 2540 bytes.
DEFINE_NERVE(TNerveBWGraphWander, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 320 bytes.
DEFINE_NERVE(TNerveBWRoll, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 636 bytes.
DEFINE_NERVE(TNerveBWBark, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 384 bytes.
DEFINE_NERVE(TNerveBWJump, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 788 bytes.
DEFINE_NERVE(TNerveBWStun, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 404 bytes.
DEFINE_NERVE(TNerveBWWakeup, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 820 bytes.
DEFINE_NERVE(TNerveBWJumpToBath, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 1076 bytes.
DEFINE_NERVE(TNerveBWDie, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 652 bytes.
DEFINE_NERVE(TNerveBWJumpAway, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 408 bytes.
DEFINE_NERVE(TNerveBWShake, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 512 bytes.
DEFINE_NERVE(TNerveBWFall, TLiveActor) { return FALSE; }
