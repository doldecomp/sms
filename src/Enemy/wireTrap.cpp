#include <Enemy/WireTrap.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/Spine.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// TODO: no nerve body below is reconstructed; each carries its map size.
// Defining them emits theNerve() and the destructor, both compiler-generated.

// TODO: incorrect size. Map records 88 bytes.
DEFINE_NERVE(TNerveWireTrapGoWait, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 912 bytes.
DEFINE_NERVE(TNerveWireTrapOnewayMove, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 236 bytes.
DEFINE_NERVE(TNerveWireTrapOnewayMoveEnd, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 188 bytes.
DEFINE_NERVE(TNerveWireTrapOnewayMoveStart, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 488 bytes.
DEFINE_NERVE(TNerveWireTrapReturnMove, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 648 bytes.
DEFINE_NERVE(TNerveWireTrapSearch, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 36 bytes.
DEFINE_NERVE(TNerveWireTrapWait, TLiveActor) { return FALSE; }
