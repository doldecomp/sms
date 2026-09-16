#include <Enemy/RocketNerve.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/Spine.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// TODO: no nerve body below is reconstructed; each carries its map size.
// Defining them emits theNerve() and the destructor, both compiler-generated.

// TODO: incorrect size. Map records 592 bytes.
DEFINE_NERVE(TNerveRocketFly, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 668 bytes.
DEFINE_NERVE(TNerveRocketPossessedNozzle, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 80 bytes.
DEFINE_NERVE(TNerveRocketWait, TLiveActor) { return FALSE; }
