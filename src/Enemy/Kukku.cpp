#include <Enemy/KukkuNerve.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/Spine.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// TODO: no nerve body below is reconstructed; each carries its map size.
// Defining them emits theNerve() and the destructor, both compiler-generated.

// TODO: incorrect size. Map records 664 bytes.
DEFINE_NERVE(TNerveKukkuFall, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 984 bytes.
DEFINE_NERVE(TNerveKukkuGraphWander, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 316 bytes.
DEFINE_NERVE(TNerveKukkuPostFall, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 364 bytes.
DEFINE_NERVE(TNerveKukkuRecoverGraph, TLiveActor) { return FALSE; }
