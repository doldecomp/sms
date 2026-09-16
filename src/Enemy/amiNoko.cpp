#include <Enemy/AmiNoko.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/Spine.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// TODO: no nerve body below is reconstructed; each carries its map size.
// Defining them emits theNerve() and the destructor, both compiler-generated.

// TODO: incorrect size. Map records 1192 bytes.
DEFINE_NERVE(TNerveAmiNokoDie, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 284 bytes.
DEFINE_NERVE(TNerveAmiNokoFreeze, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 1296 bytes.
DEFINE_NERVE(TNerveAmiNokoTurn, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 1144 bytes.
DEFINE_NERVE(TNerveAmiNokoWalkOnFence, TLiveActor) { return FALSE; }
