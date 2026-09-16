#include <Enemy/HanaSamboObj.hpp>
#include <Enemy/HanaSambo.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/Spine.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// TODO: no nerve body below is reconstructed; each carries its map size.
// Defining them emits theNerve() and the destructor, both compiler-generated.

// TODO: incorrect size. Map records 592 bytes.
DEFINE_NERVE(TNerveHanaSamboAppear, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 500 bytes.
DEFINE_NERVE(TNerveHanaSamboAttack, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 572 bytes.
DEFINE_NERVE(TNerveHanaSamboDie, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 252 bytes.
DEFINE_NERVE(TNerveHanaSamboFreeze, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 660 bytes.
DEFINE_NERVE(TNerveHanaSamboHide, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 524 bytes.
DEFINE_NERVE(TNerveHanaSamboWait, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 640 bytes.
DEFINE_NERVE(TNerveSamboHeadAppear, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 896 bytes.
DEFINE_NERVE(TNerveSamboHeadAttack, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 576 bytes.
DEFINE_NERVE(TNerveSamboHeadHide, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 532 bytes.
DEFINE_NERVE(TNerveSamboHeadHitWall, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 1280 bytes.
DEFINE_NERVE(TNerveSamboHeadHitWater, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 140 bytes.
DEFINE_NERVE(TNerveSamboHeadRecoverWater, TLiveActor) { return FALSE; }

THanaSambo::~THanaSambo() { }
THanaSamboHead::~THanaSamboHead() { }
THanaSamboManager::~THanaSamboManager() { }
TSamboFlower::~TSamboFlower() { }
TSamboFlowerManager::~TSamboFlowerManager() { }
TSamboHead::~TSamboHead() { }
TSamboHeadManager::~TSamboHeadManager() { }
TSamboLeaf::~TSamboLeaf() { }
