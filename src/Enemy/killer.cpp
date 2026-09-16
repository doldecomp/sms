#include <Enemy/KillerNerve.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/Spine.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// TODO: no nerve body below is reconstructed; each carries its map size.
// Defining them emits theNerve() and the destructor, both compiler-generated.

// TODO: incorrect size. Map records 1352 bytes.
DEFINE_NERVE(TNerveFlyEnemyChaseFly, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 592 bytes.
DEFINE_NERVE(TNerveFlyEnemyNormalFly, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 676 bytes.
DEFINE_NERVE(TNerveKillerExplosion, TLiveActor) { return FALSE; }
