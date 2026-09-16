#include <Enemy/TinKoopa.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/Spine.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// TODO: no nerve body below is reconstructed; each carries its map size.
// Defining them emits theNerve() and the destructor, both compiler-generated.

// TODO: incorrect size. Map records 1000 bytes.
DEFINE_NERVE(TNerveTinKoopaBreak, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 380 bytes.
DEFINE_NERVE(TNerveTinKoopaDamage, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 224 bytes.
DEFINE_NERVE(TNerveTinKoopaWait, TLiveActor) { return FALSE; }
