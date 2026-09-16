#include <Enemy/BossTelesaObj.hpp>
#include <Enemy/BossTelesa.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/Spine.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// TODO: no nerve body below is reconstructed; each carries its map size.
// Defining them emits theNerve() and the destructor, both compiler-generated.

// TODO: incorrect size. Map records 972 bytes.
DEFINE_NERVE(TNerveBossTelesaAppear, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 2248 bytes.
DEFINE_NERVE(TNerveBossTelesaDie, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 492 bytes.
DEFINE_NERVE(TNerveBossTelesaFallDemo, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 432 bytes.
DEFINE_NERVE(TNerveBossTelesaFreeze, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 472 bytes.
DEFINE_NERVE(TNerveBossTelesaHide, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 512 bytes.
DEFINE_NERVE(TNerveBossTelesaHideWait, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 1640 bytes.
DEFINE_NERVE(TNerveBossTelesaPrepareSlot, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 692 bytes.
DEFINE_NERVE(TNerveBossTelesaSlotStart, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 304 bytes.
DEFINE_NERVE(TNerveBossTelesaSpit, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 592 bytes.
DEFINE_NERVE(TNerveBossTelesaSpitSlotItem, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 1000 bytes.
DEFINE_NERVE(TNerveBubbleLive, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 192 bytes.
DEFINE_NERVE(TNerveBubbleSplit, TLiveActor) { return FALSE; }

TBossTelesa::~TBossTelesa() { }
TBossTelesaBody::~TBossTelesaBody() { }
TBossTelesaKillSmallEnemy::~TBossTelesaKillSmallEnemy() { }
TBossTelesaManager::~TBossTelesaManager() { }
TBossTelesaTongue::~TBossTelesaTongue() { }
TBubble::~TBubble() { }
TBubbleManager::~TBubbleManager() { }
TTelesaSlot::~TTelesaSlot() { }
