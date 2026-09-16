#include <Enemy/Popo.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/Spine.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// TODO: no nerve body below is reconstructed; each carries its map size.
// Defining them emits theNerve() and the destructor, both compiler-generated.

// TODO: incorrect size. Map records 408 bytes.
DEFINE_NERVE(TNervePopoAttack, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 516 bytes.
DEFINE_NERVE(TNervePopoExplosion, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 668 bytes.
DEFINE_NERVE(TNervePopoFly, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 464 bytes.
DEFINE_NERVE(TNervePopoPossessedNozzle, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 152 bytes.
DEFINE_NERVE(TNervePopoThrown, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 240 bytes.
DEFINE_NERVE(TNervePopoWait, TLiveActor) { return FALSE; }
