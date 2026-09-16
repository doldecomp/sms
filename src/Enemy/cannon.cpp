#include <Enemy/CannonNerve.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/Spine.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// TODO: no nerve body below is reconstructed; each carries its map size.
// Defining them emits theNerve() and the destructor, both compiler-generated.

// TODO: incorrect size. Map records 824 bytes.
DEFINE_NERVE(TNerveCannonClose, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 1556 bytes.
DEFINE_NERVE(TNerveCannonDamage, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 556 bytes.
DEFINE_NERVE(TNerveCannonDamageDemo, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 684 bytes.
DEFINE_NERVE(TNerveCannonForceBombShoot, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 480 bytes.
DEFINE_NERVE(TNerveCannonObject, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 456 bytes.
DEFINE_NERVE(TNerveCannonOpen, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 1732 bytes.
DEFINE_NERVE(TNerveCannonSearch, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 924 bytes.
DEFINE_NERVE(TNerveCannonShoot, TLiveActor) { return FALSE; }
