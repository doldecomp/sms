#include <Enemy/KoopaNerve.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/Spine.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// TODO: no nerve body below is reconstructed; each carries its map size.
// Defining them emits theNerve() and the destructor, both compiler-generated.

// TODO: incorrect size. Map records 212 bytes.
DEFINE_NERVE(TNerveKoopaFall, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 2740 bytes.
DEFINE_NERVE(TNerveKoopaFlame, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 932 bytes.
DEFINE_NERVE(TNerveKoopaGetDown, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 252 bytes.
DEFINE_NERVE(TNerveKoopaGetShowered, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 360 bytes.
DEFINE_NERVE(TNerveKoopaProvoke, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 252 bytes.
DEFINE_NERVE(TNerveKoopaStagger, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 464 bytes.
DEFINE_NERVE(TNerveKoopaTumble, TLiveActor) { return FALSE; }

DEFINE_NERVE(TNerveKoopaTurn, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 408 bytes.
DEFINE_NERVE(TNerveKoopaTurnL, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 400 bytes.
DEFINE_NERVE(TNerveKoopaTurnR, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 2268 bytes.
DEFINE_NERVE(TNerveKoopaWait, TLiveActor) { return FALSE; }
