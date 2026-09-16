#include <Enemy/TabePukuNerve.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/Spine.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// TODO: no nerve body below is reconstructed; each carries its map size.
// Defining them emits theNerve() and the destructor, both compiler-generated.

// TODO: incorrect size. Map records 912 bytes.
DEFINE_NERVE(TNerveTabePukuAttack, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 248 bytes.
DEFINE_NERVE(TNerveTabePukuBite, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 452 bytes.
DEFINE_NERVE(TNerveTabePukuDive, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 864 bytes.
DEFINE_NERVE(TNerveTabePukuDrag, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 680 bytes.
DEFINE_NERVE(TNerveTabePukuFound, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 488 bytes.
DEFINE_NERVE(TNerveTabePukuGraphWander, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 516 bytes.
DEFINE_NERVE(TNerveTabePukuRecoverGraph, TLiveActor) { return FALSE; }
