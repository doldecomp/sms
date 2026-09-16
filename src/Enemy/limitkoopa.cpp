#include <Enemy/LimitKoopa.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/Spine.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// TODO: no nerve body below is reconstructed; each carries its map size.
// Defining them emits theNerve() and the destructor, both compiler-generated.

// TODO: incorrect size. Map records 388 bytes.
DEFINE_NERVE(TNerveLimitKoopaGetDown, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 156 bytes.
DEFINE_NERVE(TNerveLimitKoopaGetShowered, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 244 bytes.
DEFINE_NERVE(TNerveLimitKoopaHipDropJump, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 296 bytes.
DEFINE_NERVE(TNerveLimitKoopaHipDropStart, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 156 bytes.
DEFINE_NERVE(TNerveLimitKoopaStagger, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 156 bytes.
DEFINE_NERVE(TNerveLimitKoopaTumble, TLiveActor) { return FALSE; }

DEFINE_NERVE(TNerveLimitKoopaTurn, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 552 bytes.
DEFINE_NERVE(TNerveLimitKoopaWait, TLiveActor) { return FALSE; }
