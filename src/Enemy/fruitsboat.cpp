#include <Enemy/FruitsBoat.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/Spine.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// TODO: no nerve body below is reconstructed; each carries its map size.
// Defining them emits theNerve() and the destructor, both compiler-generated.

// TODO: incorrect size. Map records 352 bytes.
DEFINE_NERVE(TNerveFruitsBoatBckTrace, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 1060 bytes.
DEFINE_NERVE(TNerveFruitsBoatGraphWander, TLiveActor) { return FALSE; }
