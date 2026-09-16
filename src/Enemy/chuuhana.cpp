#include <Enemy/ChuuHana.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/Spine.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// TODO: no nerve body below is reconstructed; each carries its map size.
// Defining them emits theNerve() and the destructor, both compiler-generated.

// TODO: incorrect size. Map records 1024 bytes.
DEFINE_NERVE(TNerveChuuHanaAttack, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 8 bytes.
DEFINE_NERVE(TNerveChuuHanaFall, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 300 bytes.
DEFINE_NERVE(TNerveChuuHanaFall2, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 688 bytes.
DEFINE_NERVE(TNerveChuuHanaForceJumped, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 420 bytes.
DEFINE_NERVE(TNerveChuuHanaJumpPrepare, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 1004 bytes.
DEFINE_NERVE(TNerveChuuHanaKeepBalance, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 8 bytes.
DEFINE_NERVE(TNerveChuuHanaObject, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 544 bytes.
DEFINE_NERVE(TNerveChuuHanaRoll, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 680 bytes.
DEFINE_NERVE(TNerveChuuHanaStick, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 108 bytes.
DEFINE_NERVE(TNerveChuuHanaWait, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 648 bytes.
DEFINE_NERVE(TNerveChuuHanaWalkOnPanel, TLiveActor) { return FALSE; }
