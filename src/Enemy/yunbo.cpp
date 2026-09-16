#include <Enemy/Yunbo.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/Spine.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// TODO: no nerve body below is reconstructed; each carries its map size.
// Defining them emits theNerve() and the destructor, both compiler-generated.

// TODO: incorrect size. Map records 560 bytes.
DEFINE_NERVE(TNerveYumboAppearing, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 560 bytes.
DEFINE_NERVE(TNerveYumboAttack, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 688 bytes.
DEFINE_NERVE(TNerveYumboDancing, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 304 bytes.
DEFINE_NERVE(TNerveYumboFreeze, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 428 bytes.
DEFINE_NERVE(TNerveYumboHiding, TLiveActor) { return FALSE; }
