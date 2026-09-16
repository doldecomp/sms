#include <Enemy/ElecNokonoko.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/Spine.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// TODO: no nerve body below is reconstructed; each carries its map size.
// Defining them emits theNerve() and the destructor, both compiler-generated.

// TODO: incorrect size. Map records 1272 bytes.
DEFINE_NERVE(TNerveElecCarapaceMove, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 912 bytes.
DEFINE_NERVE(TNerveElecCarapaceReturn, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 28 bytes.
DEFINE_NERVE(TNerveElecCarapaceWait, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 140 bytes.
DEFINE_NERVE(TNerveElecNokonokoAttack, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 732 bytes.
DEFINE_NERVE(TNerveElecNokonokoCollect, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 772 bytes.
DEFINE_NERVE(TNerveElecNokonokoFreeze, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 280 bytes.
DEFINE_NERVE(TNerveElecNokonokoRebirth, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 504 bytes.
DEFINE_NERVE(TNerveElecNokonokoShoot, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 548 bytes.
DEFINE_NERVE(TNerveElecNokonokoTurn, TLiveActor) { return FALSE; }
