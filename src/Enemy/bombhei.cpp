#include <Enemy/BombheiNerve.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/Spine.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// TODO: no nerve body below is reconstructed; each carries its map size.
// Defining them emits theNerve() and the destructor, both compiler-generated.

// TODO: incorrect size. Map records 240 bytes.
DEFINE_NERVE(TNerveBombHeiAttack, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 648 bytes.
DEFINE_NERVE(TNerveBombHeiExplosion, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 564 bytes.
DEFINE_NERVE(TNerveBombHeiGenerate, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 44 bytes.
DEFINE_NERVE(TNerveBombHeiPickUp, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 436 bytes.
DEFINE_NERVE(TNerveBombHeiThrown, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 624 bytes.
DEFINE_NERVE(TNerveBombHeiWaitExplosion, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 432 bytes.
DEFINE_NERVE(TNerveBombHeiWalkExplosion, TLiveActor) { return FALSE; }
