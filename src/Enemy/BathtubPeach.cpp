#include <Enemy/BathtubPeach.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/Spine.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// TODO: no nerve body below is reconstructed; each carries its map size.
// Defining them emits theNerve() and the destructor, both compiler-generated.

// TODO: incorrect size. Map records 1324 bytes.
DEFINE_NERVE(TNervePeachEscape, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 240 bytes.
DEFINE_NERVE(TNervePeachStagger, TLiveActor) { return FALSE; }
