#include <Enemy/BossPakkun.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/Spine.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// TODO: no nerve body below is reconstructed; each carries its map size.
// Defining them emits theNerve() and the destructor, both compiler-generated.

// TODO: incorrect size. Map records 508 bytes.
DEFINE_NERVE(TNervePakkunGenerate, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 2740 bytes.
DEFINE_NERVE(TNervePakkunStay, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 276 bytes.
DEFINE_NERVE(TNervePakkunAppear, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 360 bytes.
DEFINE_NERVE(TNervePakkunHide, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 352 bytes.
DEFINE_NERVE(TNervePakkunShoot, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 312 bytes.
DEFINE_NERVE(TNervePakkunFreeze, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 688 bytes.
DEFINE_NERVE(TNerveStayPakkunHide, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 436 bytes.
DEFINE_NERVE(TNerveStayPakkunAppear, TLiveActor) { return FALSE; }
