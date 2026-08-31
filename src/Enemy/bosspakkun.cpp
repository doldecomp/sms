#include <Enemy/BossPakkun.hpp>

static const char* bosspakkun_bastable[] = {
	nullptr,
	nullptr,
	"/scene/bosspakkun/bas/bosspaku_ball_end.bas",
	"/scene/bosspakkun/bas/bosspaku_down.bas",
	"/scene/bosspakkun/bas/bosspaku_down_end.bas",
	nullptr,
	"/scene/bosspakkun/bas/bosspaku_down_loop.bas",
	nullptr,
	"/scene/bosspakkun/bas/bosspaku_fall_end.bas",
	nullptr,
	"/scene/bosspakkun/bas/bosspaku_fall_start.bas",
	"/scene/bosspakkun/bas/bosspaku_fly.bas",
	"/scene/bosspakkun/bas/bosspaku_fly_pollut.bas",
	"/scene/bosspakkun/bas/bosspaku_fly_start.bas",
	"/scene/bosspakkun/bas/bosspaku_getup.bas",
	"/scene/bosspakkun/bas/bosspaku_head.bas",
	"/scene/bosspakkun/bas/bosspaku_hovering.bas",
	"/scene/bosspakkun/bas/bosspaku_jump_reaction.bas",
	"/scene/bosspakkun/bas/bosspaku_land.bas",
	"/scene/bosspakkun/bas/bosspaku_panpan.bas",
	"/scene/bosspakkun/bas/bosspaku_pollut_end.bas",
	"/scene/bosspakkun/bas/bosspaku_pollut_start.bas",
	"/scene/bosspakkun/bas/bosspaku_return.bas",
	"/scene/bosspakkun/bas/bosspaku_sleep.bas",
	"/scene/bosspakkun/bas/bosspaku_tornado.bas",
	nullptr,
	"/scene/bosspakkun/bas/bosspaku_water_hit.bas",
	nullptr,
	nullptr,
	nullptr,
};

TBossPakkunParams::TBossPakkunParams(const char* path)
    : TSpineEnemyParams(path)
    , PARAM_INIT(mSLWaitFrameStg0, 400)
    , PARAM_INIT(mSLWaterMarkLimit, 600)
    , PARAM_INIT(mSLSwingLength, 600.0f)
    , PARAM_INIT(mSLPollBallStampScale, 1.0f)
    , PARAM_INIT(mSLTumbleTime, 2400)
    , PARAM_INIT(mSLAnmBlendTime0, 60)
    , PARAM_INIT(mSLFlySpeed, 5.0f)
    , PARAM_INIT(mSLPivotSpeed, 0.7f)
    , PARAM_INIT(mSLPivotSpeedAware, 1.5f)
    , PARAM_INIT(mSLVomitAnmRate, 0.6f)
    , PARAM_INIT(mSLHeadHomingLimit, 30.0f)
    , PARAM_INIT(mSLDamageAngle, 180.0f)
    , PARAM_INIT(mSLTornadoProp, 0.4f)
    , PARAM_INIT(mSLTornadoSpeed, 2.0f)
    , PARAM_INIT(mSLTornadoRollSpeed, 0.06f)
    , PARAM_INIT(mSLTornadoMoveInit, 10000.0f)
    , PARAM_INIT(mSLTornadoMoveInc, 1.0f)
    , PARAM_INIT(mSLTornadoMoveLimit, 10720.0f)
    , PARAM_INIT(mSLWaterHitTimer, 20)
    , PARAM_INIT(mSLHoverTimer, 1200)
    , PARAM_INIT(mSLPollBallRange, 10000.0f)
    , PARAM_INIT(mSLPollBallSpeed, 20.0f)
    , PARAM_INIT(mSLPollBallFront, 1000.0f)
{
	TParams::load(mPrmPath);
}

TBPPolDrop::TBPPolDrop(TBossPakkun* owner, const char* name)
    : THitActor(name)
    , mOwner(owner)
    , unk78(nullptr)
    , unk7C(nullptr)
    , unk80(0)
    , unk84(0)
    , unk88(0.0f)
{
}

void TBPPolDrop::drop() { }

void TBPPolDrop::move() { }

void TBPPolDrop::launch(const JGeometry::TVec3<f32>&,
                        const JGeometry::TVec3<f32>&)
{
}

void TBPPolDrop::perform(u32, JDrama::TGraphics*) { }

TBPVomit::TBPVomit(TBossPakkun* owner, const char* name)
    : JDrama::TViewObj(name)
    , mOwner(owner)
    , unk14(nullptr)
    , unk18(nullptr)
{
}

void TBPVomit::vomit() { }

void TBPVomit::vomitFinished() { }

void TBPVomit::perform(u32, JDrama::TGraphics*) { }

TBPTornado::TBPTornado(TBossPakkun* owner, const char* name)
    : THitActor(name)
    , mOwner(owner)
    , mActor(nullptr)
    , unk94(0.0f)
    , unk98(0)
{
}

void TBPTornado::vanish() { }

void TBPTornado::perform(u32, JDrama::TGraphics*) { }

void TBPTornado::launch(const JGeometry::TVec3<f32>&) { }

TBPHeadHit::TBPHeadHit(TBossPakkun* owner, const char* name)
    : THitActor(name)
    , mOwner(owner)
{
}

BOOL TBPHeadHit::receiveMessage(THitActor*, u32) { return false; }

void TBPHeadHit::throwActor(THitActor*) { }

void TBPHeadHit::perform(u32, JDrama::TGraphics*) { }

TBPNavel::TBPNavel(TBossPakkun* owner, const char* name)
    : THitActor(name)
    , mOwner(owner)
{
}

BOOL TBPNavel::receiveMessage(THitActor*, u32) { return false; }

void TBPNavel::perform(u32, JDrama::TGraphics*) { }

TBossPakkunMtxCalc::TBossPakkunMtxCalc(TBossPakkun* owner)
    : M3UMtxCalcSIAnmBlendQuat(false)
    , mOwner(owner)
{
}

void TBossPakkunMtxCalc::calcBellyScale(u16) { }

void TBossPakkunMtxCalc::calcHeadDir(u16) { }

void TBossPakkunMtxCalc::joinAnm(int) { }

void TBossPakkunMtxCalc::setAnm(int) { }

void TBossPakkunMtxCalc::calc(u16) { }

TBossPakkun::TBossPakkun(const char* name)
    : TSpineEnemy(name)
    , mMtxCalc(nullptr)
    , unk154(0.0f)
    , mPolDrop(nullptr)
    , mVomit(nullptr)
    , mTornado(nullptr)
    , mHeadHit(nullptr)
    , mNavel(nullptr)
    , unk16C(0)
    , unk170(0)
    , unk174(0)
    , unk178(0)
    , unk17C(0)
    , unk180(nullptr)
    , unk184(0.0f)
    , unk188(nullptr)
    , unk18C(nullptr)
    , unk190(0)
    , unk1B8(0)
    , unk1BC(0)
    , unk1C0(0)
    , unk1C4(0)
    , unk1C8(0.0f)
    , unk1CC(0)
{
}

void TBossPakkun::init(TLiveManager*) { }

void TBossPakkun::checkMarioRiding() { }

void TBossPakkun::startBGM() { }

void TBossPakkun::rumblePad(int, const JGeometry::TVec3<f32>&) { }

void TBossPakkun::showMessage(u32) { }

BOOL TBossPakkun::is2ndFightNow() const { return false; }

void TBossPakkun::ignoreWaterCheck() { }

void TBossPakkun::startTornadoBlur() { }

void TBossPakkun::resetWaterMark() { }

BOOL TBossPakkun::inArea(const JGeometry::TVec3<f32>&) { return false; }

void TBossPakkun::gotFlyingDamage() { }

void TBossPakkun::gotWaterDamage() { }

void TBossPakkun::gotHipDropDamage() { }

void TBossPakkun::gotTrampleDamage() { }

void TBossPakkun::launchPolDrop() { }

void TBossPakkun::launchTornado() { }

void TBossPakkun::killSmallEnemies() { }

void TBossPakkun::changeBck(int) { }

void TBossPakkun::flyToCurPathNode(f32, f32) { }

const char** TBossPakkun::getBasNameTable() const
{
	return bosspakkun_bastable;
}

void TBossPakkun::setGroundCollision() { }

void TBossPakkun::kill() { }

BOOL TBossPakkun::receiveMessage(THitActor*, u32) { return false; }

void TBossPakkun::perform(u32, JDrama::TGraphics*) { }

TBossPakkunManager::TBossPakkunManager(const char* name, int value)
    : TEnemyManager(name)
    , unk54(value)
{
}

void TBossPakkunManager::initJParticle() { }

void TBossPakkunManager::createModelData() { }

void TBossPakkunManager::load(JSUMemoryInputStream&) { }

DEFINE_NERVE(TNerveBPWait, TLiveActor) { return false; }

DEFINE_NERVE(TNerveBPCannon, TLiveActor) { return false; }

DEFINE_NERVE(TNerveBPVomit, TLiveActor) { return false; }

DEFINE_NERVE(TNerveBPTornado, TLiveActor) { return false; }

DEFINE_NERVE(TNerveBPPivot, TLiveActor) { return false; }

DEFINE_NERVE(TNerveBPSwallow, TLiveActor) { return false; }

DEFINE_NERVE(TNerveBPTumbleIn, TLiveActor) { return false; }

DEFINE_NERVE(TNerveBPTumble, TLiveActor) { return false; }

DEFINE_NERVE(TNerveBPTumbleOut, TLiveActor) { return false; }

DEFINE_NERVE(TNerveBPGetUp, TLiveActor) { return false; }

DEFINE_NERVE(TNerveBPSwing, TLiveActor) { return false; }

DEFINE_NERVE(TNerveBPStompReact, TLiveActor) { return false; }

DEFINE_NERVE(TNerveBPJumpReact, TLiveActor) { return false; }

DEFINE_NERVE(TNerveBPPreDie, TLiveActor) { return false; }

DEFINE_NERVE(TNerveBPDie, TLiveActor) { return false; }

DEFINE_NERVE(TNerveBPTakeOff, TLiveActor) { return false; }

DEFINE_NERVE(TNerveBPFly, TLiveActor) { return false; }

DEFINE_NERVE(TNerveBPTouchDown, TLiveActor) { return false; }

DEFINE_NERVE(TNerveBPFlyCannon, TLiveActor) { return false; }

DEFINE_NERVE(TNerveBPFlyPivot, TLiveActor) { return false; }

DEFINE_NERVE(TNerveBPHover, TLiveActor) { return false; }

DEFINE_NERVE(TNerveBPFall, TLiveActor) { return false; }

DEFINE_NERVE(TNerveBPSleep, TLiveActor) { return false; }

DEFINE_NERVE(TNerveBPBreakSleep, TLiveActor) { return false; }

DEFINE_NERVE(TNerveBPWaitL, TLiveActor) { return false; }

DEFINE_NERVE(TNerveBPCannonL, TLiveActor) { return false; }
