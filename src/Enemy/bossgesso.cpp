#include <Enemy/BossGesso.hpp>
#include <System/Particles.hpp>

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
    : TSpineEnemyParams(path)
    , PARAM_INIT(mSLUnisonInter, 2000)
    , PARAM_INIT(mSLUnisonHoming, 200)
    , PARAM_INIT(mSLSingleHoming, 200)
    , PARAM_INIT(mSLBeakHoming, 200)
    , PARAM_INIT(mSLRegenFoot, 4800)
    , PARAM_INIT(mSLAmputeeTime, 1200)
    , PARAM_INIT(mSLRestTime, 360)
    , PARAM_INIT(mSLStunTime, 1200)
    , PARAM_INIT(mSLBeakDamageHeight, 150.0f)
    , PARAM_INIT(mSLBeakDamageRadius, 150.0f)
    , PARAM_INIT(mSLBeakLengthLimit, 1200.0f)
    , PARAM_INIT(mSLBeakLengthDamage, 600.0f)
    , PARAM_INIT(mSLBeakLengthPollute, 500.0f)
    , PARAM_INIT(mSLUnisonAttackSpeed, 2.0f)
    , PARAM_INIT(mSLDoubleAttackSpeed, 1.2f)
    , PARAM_INIT(mSLSkipRopeAttackSpeed, 1.0f)
    , PARAM_INIT(mSLSingleAttackLen, 2200.0f)
    , PARAM_INIT(mSLDoubleAttackLen, 1900.0f)
    , PARAM_INIT(mSLUnisonAttackLen, 1900.0f)
    , PARAM_INIT(mSLForceUnisonLen, 800.0f)
    , PARAM_INIT(mSLGuardLen, 500.0f)
    , PARAM_INIT(mSLTentacleStretch, 0.1f)
    , PARAM_INIT(mSLBeakStretch, 0.07f)
    , PARAM_INIT(mSLEyeDamageRadius, 100.0f)
    , PARAM_INIT(mSLEyeDamageHeight, 150.0f)
    , PARAM_INIT(mSLShootRadius, 8000.0f)
    , PARAM_INIT(mSLBlurJoint, 10)
    , PARAM_INIT(mSLBlurScale, 1.0f)
    , PARAM_INIT(mSLColumnScale, 3.0f)
    , PARAM_INIT(mSLSightAngle, 120.0f)
    , PARAM_INIT(mSLAmputeeWait, 720)
{
	TParams::load(mPrmPath);
}

TBGBeakHit::TBGBeakHit(TBossGesso* owner, const char* name)
    : TTakeActor(name)
{
}

MtxPtr TBGBeakHit::getTakingMtx() { }

void TBGBeakHit::moveRequest(const JGeometry::TVec3<f32>& param_1) { }

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

void TBossGessoManager::initJParticle()
{
	SMS_LoadParticle("/scene/bgeso/jpa/ms_boge_attack_a.jpa", 0x8c);
	SMS_LoadParticle("/scene/bgeso/jpa/ms_boge_attack_b.jpa", 0x8d);
	SMS_LoadParticle("/scene/bgeso/jpa/ms_boge_attack_c.jpa", 0x8e);
	SMS_LoadParticle("/scene/bgeso/jpa/ms_boge_attack_d.jpa", 0x8f);
	SMS_LoadParticle("/scene/bgeso/jpa/ms_boge_blur.jpa", 0x90);
	SMS_LoadParticle("/scene/bgeso/jpa/ms_boge_blur_f.jpa", 0x91);
	SMS_LoadParticle("/scene/bgeso/jpa/ms_boge_blur_j.jpa", 0x92);
	SMS_LoadParticle("/scene/bgeso/jpa/ms_boge_canon_a.jpa", 0x93);
	SMS_LoadParticle("/scene/bgeso/jpa/ms_boge_canon_b.jpa", 0x94);
	SMS_LoadParticle("/scene/bgeso/jpa/ms_boge_damage_a.jpa", 0x95);
	SMS_LoadParticle("/scene/bgeso/jpa/ms_boge_damage_b.jpa", 0x96);
	SMS_LoadParticle("/scene/bgeso/jpa/ms_boge_hit_a.jpa", 0x97);
	SMS_LoadParticle("/scene/bgeso/jpa/ms_boge_hit_b.jpa", 0x98);
	SMS_LoadParticle("/scene/bgeso/jpa/ms_boge_hit_c.jpa", 0x99);
	SMS_LoadParticle("/scene/bgeso/jpa/ms_boge_hitdown.jpa", 0x9a);
	SMS_LoadParticle("/scene/bgeso/jpa/ms_boge_jump.jpa", 0x9b);
	SMS_LoadParticle("/scene/bgeso/jpa/ms_boge_kizetsu.jpa", 0x9c);
	SMS_LoadParticle("/scene/bgeso/jpa/ms_boge_kizetsu_r.jpa", 0x9d);
	SMS_LoadParticle("/scene/bgeso/jpa/ms_boge_odanhit_a.jpa", 0x9e);
	SMS_LoadParticle("/scene/bgeso/jpa/ms_boge_odanhit_b.jpa", 0x9f);
	SMS_LoadParticle("/scene/bgeso/jpa/ms_boge_osen.jpa", 0xa0);
	SMS_LoadParticle("/scene/bgeso/jpa/ms_boge_ase.jpa", 0x138);
	SMS_LoadParticle("/scene/bgeso/jpa/ms_boge_namida.jpa", 0x139);
	SMS_LoadParticle("/scene/bgeso/jpa/ms_boge_kiseki.jpa", 0x13a);
	SMS_LoadParticle("/scene/bgeso/jpa/ms_boge_wash.jpa", 0x13b);
}

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
