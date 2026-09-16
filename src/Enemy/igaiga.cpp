#include <Enemy/Igaiga.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/Spine.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// TODO: no nerve body below is reconstructed; each carries its map size.
// Defining them emits theNerve() and the destructor, both compiler-generated.

// TODO: incorrect size. Map records 288 bytes.
DEFINE_NERVE(TNerveIgaigaRollOnGraph, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 624 bytes.
DEFINE_NERVE(TNerveIgaigaWaterHit, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 376 bytes.
DEFINE_NERVE(TNerveIgaigaShootFromCannon, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 192 bytes.
DEFINE_NERVE(TNerveGorogoroRollOnGraph, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 564 bytes.
DEFINE_NERVE(TNerveGorogoroDie, TLiveActor) { return FALSE; }
