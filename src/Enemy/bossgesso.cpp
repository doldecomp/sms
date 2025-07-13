#include <Enemy/BossGesso.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

// TBGBeakHit::~TBGBeakHit()
// TBGEyeHit::~TBGEyeHit()
// TBGBodyHit::~TBGBodyHit()
// TBossGessoMtxCalc::~TBossGessoMtxCalc()
// TBGBinder::~TBGBinder()
// TBossGesso::~TBossGesso()
// TBossGessoManager::~TBossGessoManager()

static void getAttackModeStr(int) { }

static void isNozzleWater(THitActor*) { }

TBossGessoParams::TBossGessoParams(const char* path)
    : TParams(path)
{
}

TBGBeakHit::TBGBeakHit(TBossGesso* owner, const char* name)
    : TTakeActor(name)
{
}

MtxPtr TBGBeakHit::getTakingMtx() { }

void TBGBeakHit::moveRequest(const JGeometry::TVec3<f32>&) { }

BOOL TBGBeakHit::receiveMessage(THitActor*, u32) { }

void TBGBeakHit::perform(u32, JDrama::TGraphics*) { }

TBGEyeHit::TBGEyeHit(TBossGesso*, int, const char*) { }

BOOL TBGEyeHit::receiveMessage(THitActor*, u32) { }

void TBGEyeHit::perform(u32, JDrama::TGraphics*) { }

TBGBodyHit::TBGBodyHit(TBossGesso*, int, const char*) { }

BOOL TBGBodyHit::receiveMessage(THitActor*, u32) { }

void TBGBodyHit::perform(u32, JDrama::TGraphics*) { }

TBossGessoMtxCalc::TBossGessoMtxCalc(TBossGesso*) { }

void TBossGessoMtxCalc::joinAnm(int) { }

void TBossGessoMtxCalc::setAnm(int) { }

void TBossGessoMtxCalc::calc(u16) { }

TBGBinder::TBGBinder() { }

void TBGBinder::bind(TLiveActor*) { }

TBGCork::TBGCork(TBossGesso*) { }

void TBGCork::crush() { }

void TBGCork::perform(u32, JDrama::TGraphics*) { }

TBossGesso::TBossGesso(const char* name)
    : TSpineEnemy(name)
{
}

void TBossGesso::init(TLiveManager*) { }

void TBossGesso::rumblePad(int, const JGeometry::TVec3<f32>&) { }

void TBossGesso::definiteRumble() { }

void TBossGesso::continuousRumble() { }

void TBossGesso::lenFromToeToMario() { }

void TBossGesso::showMessage(u32) { }

void TBossGesso::checkTakeMsg() { }

void TBossGesso::changeBck(int) { }

void TBossGesso::inSightAngle(f32) { }

void TBossGesso::inSight() { }

void TBossGesso::is2ndFightNow() const { }

void TBossGesso::stopIfRoll() { }

void TBossGesso::changeAttackMode(int) { }

void TBossGesso::gotTentacleDamage() { }

void TBossGesso::gotEyeDamage() { }

void TBossGesso::gotBeakDamage() { }

void TBossGesso::changeAllTentacleState(int) { }

void TBossGesso::forceAllTentacleState(int) { }

void TBossGesso::startPollute() { }

void TBossGesso::stopPollute() { }

void TBossGesso::launchPolDrop() { }

void TBossGesso::setEyeDamageBtp(int) { }

void TBossGesso::tentacleHeld() const { }

void TBossGesso::tentacleAttack() { }

void TBossGesso::beakHeld() const { }

void TBossGesso::tentacleWait() { }

const char** TBossGesso::getBasNameTable() const { }

BOOL TBossGesso::receiveMessage(THitActor*, u32) { }

void TBossGesso::doAttackSingle() { }

void TBossGesso::doAttackDouble() { }

void TBossGesso::doAttackSkipRope() { }

void TBossGesso::doAttackUnison() { }

void TBossGesso::doAttackShoot() { }

void TBossGesso::doAttackGuard() { }

void TBossGesso::doAttackRoll() { }

// void SMS_GetMarioPos() { }
// void MsGetRotFromZaxisY(const JGeometry::TVec3<f32>&) { }
// void MsWrap<f32>(f32, f32, f32) { }

void TBossGesso::moveObject() { }

void TBossGesso::reset() { }

void TBossGesso::calcRootMatrix() { }

void TBossGesso::performInContainer(u32, JDrama::TGraphics*) { }

void TBossGesso::perform(u32, JDrama::TGraphics*) { }

TBossGessoManager::TBossGessoManager(const char* name)
    : TEnemyManager(name)
{
}

void TBossGessoManager::createModelData() { }

void TBossGessoManager::initJParticle() { }

void TBossGessoManager::load(JSUMemoryInputStream&) { }

DEFINE_NERVE(TNerveBGWait, TLiveActor) { }

DEFINE_NERVE(TNerveBGEyeDamage, TLiveActor) { }

DEFINE_NERVE(TNerveBGBeakDamage, TLiveActor) { }

DEFINE_NERVE(TNerveBGTentacleDamage, TLiveActor) { }

DEFINE_NERVE(TNerveBGTug, TLiveActor) { }

DEFINE_NERVE(TNerveBGDie, TLiveActor) { }

DEFINE_NERVE(TNerveBGPollute, TLiveActor) { }

DEFINE_NERVE(TNerveBGPolDrop, TLiveActor) { }

DEFINE_NERVE(TNerveBGRoll, TLiveActor) { }

// void TMActorKeeper::getMActorAnmData() const {}
// void MActorAnmDataEach<J3DAnmTransformKey>::getAnmPtr(int) const {}
// void MActorAnmEach<J3DAnmTransformKey>::setFrameCtrl(int) {}
