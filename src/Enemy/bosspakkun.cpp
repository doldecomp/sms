#include <Enemy/BossPakkun.hpp>
#include <Enemy/AreaCylinder.hpp>
#include <Enemy/Conductor.hpp>
#include <Enemy/Graph.hpp>
#include <Enemy/NameKuri.hpp>
#include <Camera/CameraShake.hpp>
#include <GC2D/GCConsole2.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/JMath.hpp>
#include <Map/MapData.hpp>
#include <Map/PollutionManager.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorData.hpp>
#include <MoveBG/ItemManager.hpp>
#include <MSound/MSoundBGM.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSModBgm.hpp>
#include <MSound/MSSetSound.hpp>
#include <Player/ModelWaterManager.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/Mario.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Spine.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/MarDirector.hpp>
#include <System/Particles.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <stdlib.h>

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

// fabricated
static inline JGeometry::TVec3<f32> fromPolar(s16 angle, f32 radius)
{
	return JGeometry::TVec3<f32>(radius * JMASSin(angle), 0.0f,
	                             radius * JMASCos(angle));
}

// fabricated
static inline JGeometry::TVec3<f32> fromPolar(f32 angle, f32 radius)
{
	return fromPolar(static_cast<s16>(DEG2SHORTANGLE(angle)), radius);
}

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

void TBPPolDrop::launch(const JGeometry::TVec3<f32>& position,
                        const JGeometry::TVec3<f32>& velocity)
{
	unk6C     = velocity;
	mPosition = position;
	mScaling.set(1.0f, 1.0f, 1.0f);
	mRotation.zero();
	unk80 = 1;
	unk84 = 0;
	unk78->setBck("pollut_ball");
	unk78->setBtk("pollut_ball_01");
	unk78->setBtk("pollut_ball_02");
	unk88 = position.y;
}

void TBPPolDrop::perform(u32, JDrama::TGraphics*) { }

TBPVomit::TBPVomit(TBossPakkun* owner, const char* name)
    : JDrama::TViewObj(name)
    , mOwner(owner)
    , unk14(nullptr)
    , unk18(nullptr)
{
}

void TBPVomit::vomit()
{
	unk14->setBckFromIndex(0);
	unk18->setBckFromIndex(1);

	MtxPtr mtx = mOwner->getModel()->getBaseTRMtx();
	unk14->getModel()->setBaseTRMtx(mtx);
	unk14->getModel()->setBaseScale(mOwner->mScaling);
	unk18->getModel()->setBaseTRMtx(mtx);
	unk18->getModel()->setBaseScale(mOwner->mScaling);
}

void TBPVomit::vomitFinished() { }

void TBPVomit::perform(u32 flags, JDrama::TGraphics* graphics)
{
	if (unk14->getCurAnmIdx(ANM_TYPE_BCK) < 0)
		return;

	u32 calcAnim = flags & CUE_CALC_ANIM;
	if (calcAnim && unk14->curAnmEndsNext(ANM_TYPE_BCK, nullptr)) {
		unk14->setBckFromIndex(-1);
		unk18->setBckFromIndex(-1);
		return;
	}

	if (calcAnim)
		unk18->calcAnm();

	if (flags & CUE_ENTRY)
		gpPollution->stampModel(unk18->getModel());

	unk14->perform(flags, graphics);
}

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

void TBPTornado::launch(const JGeometry::TVec3<f32>& target)
{
	unk98     = 1;
	unk70     = target;
	mPosition = mOwner->mPosition;
	unk7C     = mOwner->mPosition;
	unk94     = mOwner->getBossPakkunParams()->mSLTornadoMoveInit.get();
	offHitFlag(HIT_FLAG_NO_COLLISION);

	J3DFrameCtrl* frameCtrl = mActor->getFrameCtrl(ANM_TYPE_BRK);
	frameCtrl->setFrame(0.0f);
	frameCtrl->setRate(0.0f);
}

TBPHeadHit::TBPHeadHit(TBossPakkun* owner, const char* name)
    : THitActor(name)
    , mOwner(owner)
{
	initHitActor(0x8000010, 5, 0x81000000, 300.0f, 500.0f, 300.0f, 500.0f);
	offHitFlag(HIT_FLAG_NO_COLLISION);
}

BOOL TBPHeadHit::receiveMessage(THitActor* sender, u32 message)
{
	TBossPakkun* boss = mOwner;
	if (boss->mSpine->getLatestNerve() == &TNerveBPSleep::theNerve())
		return boss->receiveMessage(sender, message);

	u32 actorType = sender->getActorType();
	if (boss->unk16C == 3
	    && (actorType == 0x1000000d || actorType == 0x1000001)) {
		boss->unk16C = 0;
		boss->mSpine->reset();
		boss->mSpine->setNext(&TNerveBPFall::theNerve());
		SMSGetMSound()->startSoundActor(MSD_SE_BS_BSPAKU_FALL, &boss->mPosition,
		                                0, nullptr, 0, 4);
		return true;
	}

	if (boss->unk16C != 2) {
		if (boss->is2ndFightNow()
		    && boss->mSpine->getLatestNerve() == &TNerveBPFly::theNerve())
			boss->showMessage(0xe0002);
		return actorType == 0x1000001;
	}

	if (actorType == 0x1000001 && message == HIT_MESSAGE_SPRAYED_BY_WATER) {
		JGeometry::TVec3<f32> toMario = *gpMarioPos;
		toMario -= mPosition;

		f32 angle = MsAngleWrap(MsGetRotFromZaxisY(toMario));
		angle     = MsAngleDiff(MsGetRotFromZaxisY(toMario), boss->mRotation.y);
		if (fabsf(angle)
		    < 0.5f * boss->getBossPakkunParams()->mSLDamageAngle.get()) {
			if (boss->unk17C == 0) {
				boss->unk170 += 1;
				if (boss->unk178
				    < boss->getBossPakkunParams()->mSLWaterMarkLimit.get())
					boss->unk178 += 1;
				boss->unk174
				    = boss->getBossPakkunParams()->mSLWaterHitTimer.get();

				if (boss->mSpine->getLatestNerve()
				    != &TNerveBPSwallow::theNerve()) {
					boss->mSpine->reset();
					boss->mSpine->setNext(&TNerveBPSwallow::theNerve());
				}
			}
		}
	}

	return true;
}

void TBPHeadHit::throwActor(THitActor* actor)
{
	if (actor->getActorType() != 0x80000001)
		return;
	if (!mOwner->mMActor->checkCurBckFromIndex(15))
		return;

	static JGeometry::TVec3<f32> up(0.0f, 1.0f, 0.0f);
	JGeometry::TVec3<f32> direction = mOwner->mPosition;
	direction -= actor->mPosition;
	if (direction.isZero())
		direction.set(0.0f, 0.0f, 1.0f);
	else
		PSVECNormalize(&direction, &direction);

	JGeometry::TVec3<f32> perpendicular;
	perpendicular.cross(up, direction);
	if (perpendicular.isZero())
		perpendicular.set(1.0f, 0.0f, 0.0f);
	else
		PSVECNormalize(&perpendicular, &perpendicular);

	perpendicular *= 2.0f;
	perpendicular += up;
	SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
	SMS_SendMessageToMario(this, HIT_MESSAGE_THROWN);
	SMS_ThrowMario(perpendicular, 100.0f);
}

void TBPHeadHit::perform(u32 flags, JDrama::TGraphics* graphics)
{
	if ((flags & CUE_MOVE) && mOwner->unk16C != 1) {
		for (int i = 0; i < mColCount; ++i) {
			THitActor* actor = mCollisions[i];
			if (actor->isActorType(0x80000001))
				throwActor(actor);
		}
	}

	if (flags & CUE_CALC_ANIM)
		mOwner->getJointTransByIndex(1, &mPosition);

	THitActor::perform(flags, graphics);
}

TBPNavel::TBPNavel(TBossPakkun* owner, const char* name)
    : THitActor(name)
    , mOwner(owner)
{
	initHitActor(0x8000011, 1, 0x80000000, 200.0f, 300.0f, 200.0f, 300.0f);
	offHitFlag(HIT_FLAG_NO_COLLISION);
}

BOOL TBPNavel::receiveMessage(THitActor* sender, u32 message)
{
	if (mOwner->mSpine->getLatestNerve() == &TNerveBPSleep::theNerve())
		return mOwner->receiveMessage(sender, message);

	if (sender->getActorType() == 0x1000001)
		return false;

	if (mOwner->unk16C != 1)
		return true;

	if (sender->getActorType() == 0x80000001 && message == HIT_MESSAGE_HIP_DROP)
		mOwner->gotHipDropDamage();

	return true;
}

void TBPNavel::perform(u32 flags, JDrama::TGraphics* graphics)
{
	if (flags & 2)
		mOwner->getJointTransByIndex(6, &mPosition);

	THitActor::perform(flags, graphics);
}

TBossPakkunMtxCalc::TBossPakkunMtxCalc(TBossPakkun* owner)
    : M3UMtxCalcSIAnmBlendQuat(false)
    , mOwner(owner)
{
}

void TBossPakkunMtxCalc::calcBellyScale(u16 jointIndex)
{
	if (jointIndex != 4 && jointIndex != 36)
		return;

	f32 progress;
	if (mOwner->unk17C != 0) {
		progress = static_cast<f32>(mOwner->unk1B8) / 50.0f;
	} else {
		s32 limit = mOwner->getBossPakkunParams()->mSLWaterMarkLimit.get();
		s32 mark  = mOwner->unk178;
		if (mark > limit)
			mark = limit;
		progress = static_cast<f32>(mark) / static_cast<f32>(limit);
	}

	f32 blend = JMAHermiteInterpolation(progress, 0.0f, 0.0f, 10.0f, 1.0f, 1.0f,
	                                    0.0f);
	MtxPtr jointMtx = mOwner->getModel()->getAnmMtx(jointIndex);
	Mtx scaleMtx;

	if (jointIndex == 36) {
		static JGeometry::TVec3<f32> goal(1.4f, 1.4f, 1.6f);
		static JGeometry::TVec3<f32> start(1.0f, 0.8f, 0.8f);
		MTXScale(scaleMtx, blend * (goal.x - start.x) + start.x,
		         blend * (goal.y - start.y) + start.y,
		         blend * (goal.z - start.z) + start.z);
	} else {
		static JGeometry::TVec3<f32> goal(1.3f, 1.7f, 1.7f);
		static JGeometry::TVec3<f32> start(1.0f, 0.9f, 0.9f);
		MTXScale(scaleMtx, blend * (goal.x - start.x) + start.x,
		         blend * (goal.y - start.y) + start.y,
		         blend * (goal.z - start.z) + start.z);
	}

	MTXConcat(jointMtx, scaleMtx, jointMtx);
	MTXCopy(jointMtx, J3DSys::mCurrentMtx);
}

void TBossPakkunMtxCalc::calcHeadDir(u16 jointIndex)
{
	if (jointIndex != 18)
		return;

	MtxPtr headMtx                = mOwner->getModel()->getAnmMtx(jointIndex);
	JGeometry::TVec3<f32> toMario = SMS_GetMarioPos();
	toMario.x -= headMtx[0][3];
	toMario.y -= headMtx[1][3];
	toMario.z -= headMtx[2][3];

	JGeometry::TVec3<f32> headAxis(headMtx[0][1], 0.0f, headMtx[2][1]);
	f32 headRotation = mOwner->unk184;
	f32 headAngle    = MsGetRotFromZaxisY(headAxis);
	f32 desiredAngle;
	if (mOwner->mMActor->checkCurBckFromIndex(25)) {
		desiredAngle
		    = MsWrap(headRotation + MsGetRotFromZaxisY(toMario), 0.0f, 360.0f);
	} else {
		desiredAngle = headAngle;
	}

	f32 delta = MsAngleDiff(desiredAngle, headAngle);
	f32 limit = mOwner->getBossPakkunParams()->mSLHeadHomingLimit.get();

	f32 limitedDelta;
	if (0.0f < delta)
		limitedDelta = limit > delta ? delta : limit;
	else
		limitedDelta = -limit > delta ? -limit : delta;

	f32 turn = MsAngleDiff(limitedDelta, headRotation);
	if (0.0f < turn)
		turn = 1.0f > turn ? turn : 1.0f;
	else
		turn = -1.0f > turn ? -1.0f : turn;

	headRotation += turn;
	mOwner->unk184 = headRotation;

	f32 s = JMASin(headRotation);
	f32 c = JMACos(headRotation);

	Mtx rotation;
	rotation[0][0] = 1.0f;
	rotation[0][1] = 0.0f;
	rotation[0][2] = 0.0f;
	rotation[0][3] = 0.0f;
	rotation[1][0] = 0.0f;
	rotation[1][1] = c;
	rotation[1][2] = -s;
	rotation[1][3] = 0.0f;
	rotation[2][0] = 0.0f;
	rotation[2][1] = s;
	rotation[2][2] = c;
	rotation[2][3] = 0.0f;

	MTXConcat(headMtx, rotation, headMtx);
	MTXConcat(J3DSys::mCurrentMtx, rotation, J3DSys::mCurrentMtx);
}

void TBossPakkunMtxCalc::joinAnm(int index)
{
	J3DAnmTransformKey* anm
	    = mOwner->getActorKeeper()->getMActorAnmData()->getUnk2C()->getAnmPtr(
	        index);

	if (mNewAnm == anm)
		return;

	mOldAnm           = mNewAnm;
	mNewAnm           = anm;
	mMotionBlendRatio = 1.0f;
}

void TBossPakkunMtxCalc::setAnm(int index)
{
	J3DAnmTransformKey* anm
	    = mOwner->getActorKeeper()->getMActorAnmData()->getUnk2C()->getAnmPtr(
	        index);

	mNewAnm           = anm;
	mOldAnm           = nullptr;
	mMotionBlendRatio = 0.0f;
}

void TBossPakkunMtxCalc::calc(u16 jointIndex)
{
	M3UMtxCalcSIAnmBlendQuat::calc(jointIndex);
	calcBellyScale(jointIndex);
	calcHeadDir(jointIndex);
}

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

void TBossPakkun::rumblePad(int type, const JGeometry::TVec3<f32>& position)
{
	if (!SMS_IsMarioTouchGround4cm())
		return;

	JGeometry::TVec3<f32> delta = SMS_GetMarioPos();
	delta -= position;
	f32 distance = delta.length();
	f32 power    = (3000.0f - distance) / 1000.0f;

	if (power < 0.0f)
		return;
	if (power > 1.0f)
		power = 1.0f;

	switch (type) {
	case 0:
		power *= 0.4f;
		break;
	case 1:
		power *= 0.7f;
		break;
	case 2:
		break;
	}

	unk1C8 = power;
	SMSRumbleMgr->start(8, &unk1C8);
}

void TBossPakkun::showMessage(u32) { }

bool TBossPakkun::is2ndFightNow() const
{
	if (gpMarDirector->mMap == 2 && gpMarDirector->unk7D == 4)
		return true;
	return false;
}

void TBossPakkun::ignoreWaterCheck() { }

void TBossPakkun::startTornadoBlur() { }

void TBossPakkun::resetWaterMark() { }

BOOL TBossPakkun::inArea(const JGeometry::TVec3<f32>& position)
{
	if (unk188 == nullptr) {
		unk188 = static_cast<TAreaCylinderManager*>(
		    gpConductor->search("ゲロエリアマネージャー"));
	}

	return unk188 == nullptr ? false : unk188->contain(position);
}

void TBossPakkun::gotFlyingDamage() { }

void TBossPakkun::gotWaterDamage() { }

void TBossPakkun::gotHipDropDamage() { }

void TBossPakkun::gotTrampleDamage() { }

void TBossPakkun::launchPolDrop()
{
	if (mPolDrop->unk80 != 0)
		return;

	JGeometry::TVec3<f32> launchPosition;
	if (checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)) {
		launchPosition = mPosition;
		launchPosition.x += 1.0f;
	} else {
		getJointTransByIndex(18, &launchPosition);
	}

	f32 marioYaw = gpMarioOriginal->mRotation.y;
	f32 front    = getBossPakkunParams()->mSLPollBallFront.get();
	JGeometry::TVec3<f32> targetOffset = fromPolar(marioYaw, front);
	JGeometry::TVec3<f32> target       = targetOffset;
	target += SMS_GetMarioPos();

	JGeometry::TVec3<f32> velocity;
	SMSCalcJumpVelocityXZ(target, launchPosition,
	                      getBossPakkunParams()->mSLPollBallSpeed.get(), 0.1f,
	                      &velocity);
	mPolDrop->launch(launchPosition, velocity);
}

void TBossPakkun::launchTornado() { }

void TBossPakkun::killSmallEnemies() { }

void TBossPakkun::changeBck(int index)
{
	if (mMActor->checkCurBckFromIndex(index)
	    && !mMActor->curAnmEndsNext(ANM_TYPE_BCK, nullptr))
		return;

	int previous = mMActor->getCurAnmIdx(ANM_TYPE_BCK);
	mMtxCalc->joinAnm(index);
	mMActor->setFrameCtrlForBck(index);

	if (index == 21) {
		mMActor->getFrameCtrl(ANM_TYPE_BCK)
		    ->setRate(getBossPakkunParams()->mSLVomitAnmRate.get());
	}

	f32 blendTime = -1.0f;
	switch (previous) {
	case 25:
		if (index == 21 || index == 26)
			blendTime = getBossPakkunParams()->mSLAnmBlendTime0.get();
		break;
	case 18:
	case 2:
	case 20:
		if (index == 25)
			blendTime = getBossPakkunParams()->mSLAnmBlendTime0.get();
		break;
	case 26:
		if (index == 22)
			blendTime = getBossPakkunParams()->mSLAnmBlendTime0.get();
		break;
	}

	if (blendTime < 0.0f) {
		J3DFrameCtrl* ctrl = mMActor->getFrameCtrl(ANM_TYPE_BCK);
		if (ctrl != nullptr)
			blendTime = 0.1f * ctrl->getEnd();
	}

	unk154 = blendTime == 0.0f ? 1.0f : 1.0f / blendTime;

	const char** table = getBasNameTable();
	setAnmSound(table == nullptr ? nullptr : table[index]);
}

void TBossPakkun::flyToCurPathNode(f32 flySpeed, f32 turnSpeed)
{
	turnToCurPathNode(turnSpeed);

	JGeometry::TVec3<f32> velocityStep = getUnkF4().getPoint();
	velocityStep -= mPosition;
	PSVECNormalize(&velocityStep, &velocityStep);
	velocityStep *= flySpeed;
	JGeometry::TVec3<f32> velocity = mLinearVelocity;
	velocity += velocityStep;
	mLinearVelocity = velocity;
}

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

void TBossPakkunManager::initJParticle()
{
	SMS_LoadParticle("/scene/bosspakkun/jpa/ms_bopa_blur1.jpa", 0xa9);
	SMS_LoadParticle("/scene/bosspakkun/jpa/ms_bopa_down.jpa", 0xaa);
	SMS_LoadParticle("/scene/bosspakkun/jpa/ms_bopa_swing1.jpa", 0xab);
	SMS_LoadParticle("/scene/bosspakkun/jpa/ms_bopa_swing2.jpa", 0xac);
	SMS_LoadParticle("/scene/bosspakkun/jpa/ms_bopa_wathit.jpa", 0x15d);
	SMS_LoadParticle("/scene/bosspakkun/jpa/ms_bopa_wathit_w.jpa", 0x15e);
	SMS_LoadParticle("/scene/bosspakkun/jpa/ms_bopa_ase.jpa", 0x15f);
	SMS_LoadParticle("/scene/bosspakkun/jpa/ms_bopa_blur2.jpa", 0x160);
	SMS_LoadParticle("/scene/bosspakkun/jpa/ms_bopa_jita.jpa", 0x161);
	SMS_LoadParticle("/scene/bosspakkun/jpa/ms_bopa_tr_rock.jpa", 0x162);
	SMS_LoadParticle("/scene/bosspakkun/jpa/ms_bopa_tr_smoke.jpa", 0x163);
	SMS_LoadParticle("/scene/bosspakkun/jpa/ms_bopa_tr_weed.jpa", 0x164);
}

void TBossPakkunManager::createModelData()
{
	if (unk54 != 0) {
		static const TModelDataLoadEntry entry[] = {
			{ "bosspaku_model.bmd",
			  J3DMLF_MaterialPEFull | (1 << J3DMLF_TevStageNumShift), 0 },
			{ "pollut_ball.bmd",
			  J3DMLF_MaterialPEFull | J3DMLF_MaterialUseIndirect
			      | (4 << J3DMLF_TevStageNumShift),
			  0 },
			{ "pollut_ball_stamp.bmd",
			  J3DMLF_MaterialPEFull | (1 << J3DMLF_TevStageNumShift), 0 },
			{ nullptr, 0, 0 },
		};
		createModelDataArray(entry);
	} else {
		static const TModelDataLoadEntry entry[] = {
			{ "bosspaku_model.bmd",
			  J3DMLF_MaterialPEFull | (1 << J3DMLF_TevStageNumShift), 0 },
			{ "bosspaku_end.bmd",
			  J3DMLF_MaterialPEFull | (16 << J3DMLF_TevStageNumShift), 0 },
			{ "pollut_ball.bmd",
			  J3DMLF_MaterialPEFull | J3DMLF_MaterialUseIndirect
			      | (4 << J3DMLF_TevStageNumShift),
			  0 },
			{ "pollut_ball_stamp.bmd",
			  J3DMLF_MaterialPEFull | (1 << J3DMLF_TevStageNumShift), 0 },
			{ "bosspakuPollut.bmd",
			  J3DMLF_MaterialPEFull | J3DMLF_MaterialUseIndirect
			      | (2 << J3DMLF_TevStageNumShift),
			  0 },
			{ "bosspakuPollut_white.bmd",
			  J3DMLF_MaterialPEFull | (1 << J3DMLF_TevStageNumShift), 0 },
			{ "trunade.bmd",
			  J3DMLF_MaterialPEFull | (2 << J3DMLF_TevStageNumShift), 0 },
			{ nullptr, 0, 0 },
		};
		createModelDataArray(entry);
	}
}

void TBossPakkunManager::load(JSUMemoryInputStream& stream)
{
	unk38 = new TBossPakkunParams("/enemy/bosspakkun.prm");
	TEnemyManager::load(stream);

	if (unk54 == 0)
		initJParticle();
}

DEFINE_NERVE(TNerveBPWait, TLiveActor)
{
	TBossPakkun* boss = static_cast<TBossPakkun*>(spine->getBody());

	JGeometry::TVec3<f32> toMario = boss->mPosition;
	toMario -= *gpMarioPos;
	f32 swingLength = boss->getBossPakkunParams()->mSLSwingLength.get();
	if (toMario.squared() < swingLength * swingLength) {
		JGeometry::TVec3<f32> facing(-toMario.x, -toMario.y, -toMario.z);
		f32 desiredYaw = MsGetRotFromZaxisY(facing);
		f32 angleDiff  = MsAngleDiff(desiredYaw, boss->mRotation.y);
		if (fabsf(angleDiff) < 60.0f) {
			spine->pushAfterCurrent(&TNerveBPWait::theNerve());
			spine->pushAfterCurrent(&TNerveBPSwing::theNerve());
			return true;
		}

		spine->pushAfterCurrent(&TNerveBPWait::theNerve());
		spine->pushAfterCurrent(&TNerveBPVomit::theNerve());
		TPathNode goal(*gpMarioPos);
		boss->unk114.push(boss->unkF4);
		boss->unkF4 = goal;
		spine->pushAfterCurrent(&TNerveBPPivot::theNerve());
		return true;
	}

	if (spine->getTime() == 0)
		boss->changeBck(25);

	if (spine->getTime() < boss->getBossPakkunParams()->mSLWaitFrameStg0.get()
	    || !boss->mMActor->isCurAnmAlreadyEnd(ANM_TYPE_BCK)) {
		return false;
	}

	if (gpMarDirector->mMap == 2
	    && (gpMarDirector->unk7D == 0 || gpMarDirector->unk7D == 1)) {
		JGeometry::TVec3<f32>* marioPos = gpMarioPos;
		if (boss->unk188 == nullptr) {
			boss->unk188 = static_cast<TAreaCylinderManager*>(
			    gpConductor->search("ゲロエリアマネージャー"));
		}
		BOOL marioInArea = boss->unk188 == nullptr
		                       ? false
		                       : boss->unk188->contain(*marioPos);
		if (marioInArea && !(*gpMarioGroundPlane)->isWaterSurface()) {
			spine->pushAfterCurrent(&TNerveBPCannon::theNerve());
		} else {
			spine->pushAfterCurrent(&TNerveBPWait::theNerve());
		}
		return true;
	}

	if (gpMarDirector->unk7D == 4) {
		f32 tornadoProp = boss->getBossPakkunParams()->mSLTornadoProp.get();
		if (boss->mTornado->unk98 != 0
		    || rand() * (1.0f / 32768.0f) < tornadoProp) {
			spine->pushAfterCurrent(&TNerveBPTakeOff::theNerve());
			spine->pushAfterCurrent(&TNerveBPVomit::theNerve());
		} else if (boss->mTornado->unk98 == 0) {
			spine->pushAfterCurrent(&TNerveBPWait::theNerve());
			spine->pushAfterCurrent(&TNerveBPTornado::theNerve());
		} else {
			spine->pushAfterCurrent(&TNerveBPWait::theNerve());
		}
		return true;
	}

	spine->pushAfterCurrent(&TNerveBPWait::theNerve());
	spine->pushAfterCurrent(&TNerveBPVomit::theNerve());
	JGeometry::TVec3<f32> goalPosition = boss->mPosition;
	f32 randomX                        = rand() * (1.0f / 32768.0f);
	goalPosition.x += 10000.0f * (randomX - 0.5f);
	f32 randomZ = rand() * (1.0f / 32768.0f);
	goalPosition.z += 10000.0f * (randomZ - 0.5f);
	TPathNode goal(goalPosition);
	boss->unk114.push(boss->unkF4);
	boss->unkF4 = goal;
	spine->pushAfterCurrent(&TNerveBPPivot::theNerve());
	return true;
}

DEFINE_NERVE(TNerveBPCannon, TLiveActor)
{
	TBossPakkun* boss = static_cast<TBossPakkun*>(spine->getBody());
	MActor* actor     = boss->mMActor;

	if (spine->getTime() == 0)
		boss->changeBck(21);

	if (actor->curAnmEndsNext(ANM_TYPE_BCK, nullptr)) {
		if (actor->checkCurBckFromIndex(21)) {
			boss->changeBck(2);
			boss->launchPolDrop();
		} else {
			spine->pushAfterCurrent(&TNerveBPWait::theNerve());
			return true;
		}
	}

	return false;
}

DEFINE_NERVE(TNerveBPVomit, TLiveActor)
{
	TBossPakkun* boss = static_cast<TBossPakkun*>(spine->getBody());
	MActor* actor     = boss->mMActor;
	if (spine->getTime() == 0)
		boss->changeBck(21);

	if (actor->checkCurBckFromIndex(21)) {
		f32 frame = actor->getFrameCtrl(ANM_TYPE_BCK)->getFrame();
		if (25.0f < frame && frame < 165.0f)
			boss->unk16C = 2;
		else
			boss->unk16C = 0;
	}

	if (actor->checkCurBckFromIndex(20) && rand() * (1.0f / 32768.0f) < 0.2f
	    && spine->getTime() == 500) {
		JGeometry::TVec3<f32> offset;
		offset = fromPolar(
		    static_cast<s16>(DEG2SHORTANGLE(boss->mRotation.y)), 700.0f);
		JGeometry::TVec3<f32> appearOffset = offset;
		gpItemManager->makeObjAppear(
		    boss->mPosition.x + appearOffset.x, boss->mPosition.y + 1.0f,
		    boss->mPosition.z + appearOffset.z, 0x20000002, false);
	}

	if (actor->curAnmEndsNext(ANM_TYPE_BCK, nullptr)) {
		if (actor->checkCurBckFromIndex(21)) {
			boss->unk16C = 0;
			boss->changeBck(20);
			boss->mVomit->vomit();
			boss->rumblePad(1, boss->mPosition);
		} else {
			if (!boss->is2ndFightNow()) {
				if (!(boss->unk1C0 & 1))
					gpMarDirector->getConsole()->startAppearBalloon(0xE0000,
					                                                true);
				boss->unk1C0 |= 1;
			}
			return true;
		}
	}

	if (actor->checkCurBckFromIndex(20)) {
		JGeometry::TVec3<f32> wind(JMASin(boss->mRotation.y), 0.0f,
		                           JMACos(boss->mRotation.y));
		gpModelWaterManager->wind(wind);
	}
	return false;
}

DEFINE_NERVE(TNerveBPTornado, TLiveActor)
{
	TBossPakkun* boss = static_cast<TBossPakkun*>(spine->getBody());
	MActor* actor     = boss->mMActor;
	if (spine->getTime() == 0) {
		boss->changeBck(24);
		gpMarioParticleManager->emitAndBindToSRTMtxPtr(
		    0xab, boss->getModel()->getAnmMtx(3), 0, boss);
		gpMarioParticleManager->emitAndBindToPosPtr(0xa9, &boss->unk194, 0,
		                                            nullptr);
		gpMarioParticleManager->emitAndBindToPosPtr(0xa9, &boss->unk1A0, 0,
		                                            nullptr);
	}
	if (spine->getTime() == 150)
		boss->mTornado->launch(*gpMarioPos);
	if (actor->isCurAnmAlreadyEnd(ANM_TYPE_BCK))
		return true;
	return false;
}

DEFINE_NERVE(TNerveBPPivot, TLiveActor)
{
	TBossPakkun* boss = static_cast<TBossPakkun*>(spine->getBody());
	if (spine->getTime() == 0)
		boss->changeBck(25);

	JGeometry::TVec3<f32> delta = boss->mPosition;
	delta -= SMS_GetMarioPos();
	f32 swingLength = boss->getBossPakkunParams()->mSLSwingLength.get();
	f32 pivotSpeed;
	if (delta.squared() < swingLength * swingLength)
		pivotSpeed = boss->getBossPakkunParams()->mSLPivotSpeedAware.get();
	else
		pivotSpeed = boss->getBossPakkunParams()->mSLPivotSpeed.get();

	if (!boss->turnToCurPathNode(pivotSpeed))
		return false;
	boss->switchNextGoalPath();
	return true;
}

DEFINE_NERVE(TNerveBPSwallow, TLiveActor)
{
	TBossPakkun* boss = static_cast<TBossPakkun*>(spine->getBody());
	if (spine->getTime() == 0)
		boss->changeBck(26);

	if (boss->unk178 >= boss->getBossPakkunParams()->mSLWaterMarkLimit.get()) {
		spine->pushAfterCurrent(&TNerveBPTumbleIn::theNerve());
		boss->unk16C = 0;
		boss->unk170 = 0;
		return true;
	}

	MtxPtr jointMtx = boss->getModel()->getAnmMtx(18);
	gpMarioParticleManager->emitAndBindToMtxPtr(0x15d, jointMtx, 1, boss);
	gpMarioParticleManager->emitAndBindToMtxPtr(0x15e, jointMtx, 1, boss);

	if (boss->unk170 != 0) {
		boss->changeBck(26);
		boss->unk170 = 0;
		return false;
	}

	boss->unk16C = 0;
	spine->pushAfterCurrent(&TNerveBPWait::theNerve());
	return true;
}

DEFINE_NERVE(TNerveBPTumbleIn, TLiveActor)
{
	TBossPakkun* boss = static_cast<TBossPakkun*>(spine->getBody());
	MActor* actor     = boss->mMActor;
	if (spine->getTime() == 0)
		boss->changeBck(3);

	if (spine->getTime() == 336) {
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    0xaa, boss->getModel()->getAnmMtx(14), 0, boss);
	}
	if (spine->getTime() == 348) {
		gpCameraShake->startShake(static_cast<EnumCamShakeMode>(0xe), 1.0f);
		boss->rumblePad(2, boss->mPosition);
	}

	if (actor->curAnmEndsNext(ANM_TYPE_BCK, nullptr)) {
		spine->pushAfterCurrent(&TNerveBPTumble::theNerve());
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveBPTumble, TLiveActor)
{
	TBossPakkun* boss = static_cast<TBossPakkun*>(spine->getBody());
	if (spine->getTime() == 0) {
		boss->changeBck(6);
		boss->unk16C = 1;
	}

	gpMarioParticleManager->emitAndBindToMtxPtr(
	    0x161, boss->getModel()->getAnmMtx(0), 1, boss);
	gpCameraShake->keepShake(static_cast<EnumCamShakeMode>(0x11), 1.0f);
	if ((spine->getTime() / 60) % 2 != 0)
		boss->rumblePad(0, boss->mPosition);

	if (spine->getTime() >= boss->getBossPakkunParams()->mSLTumbleTime.get()) {
		boss->unk16C = 0;
		spine->pushAfterCurrent(&TNerveBPTumbleOut::theNerve());
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveBPTumbleOut, TLiveActor)
{
	TBossPakkun* boss = static_cast<TBossPakkun*>(spine->getBody());
	MActor* actor     = boss->mMActor;
	if (spine->getTime() == 0) {
		boss->changeBck(14);
		gpCameraShake->startShake(static_cast<EnumCamShakeMode>(0x10), 1.0f);
		boss->rumblePad(0, boss->mPosition);
	}

	if (actor->curAnmEndsNext(ANM_TYPE_BCK, nullptr)) {
		if (actor->checkCurBckFromIndex(14)) {
			boss->changeBck(22);
			if (!boss->is2ndFightNow()) {
				boss->unk1C4 += 1;
				if (boss->unk1C4 >= 3) {
					gpMarDirector->getConsole()->startAppearBalloon(0xe0001,
					                                                true);
					boss->unk1C4 = 0;
				}
			}
		} else {
			spine->pushAfterCurrent(&TNerveBPWait::theNerve());
			return true;
		}
	}

	if (actor->checkCurBckFromIndex(22)) {
		f32 frame = actor->getFrameCtrl(ANM_TYPE_BCK)->getFrame();
		if (140.0f < frame && frame < 160.0f && boss->unk17C == 0) {
			boss->unk17C = 1;
			boss->unk174 = 0;
			boss->unk170 = 0;
			boss->unk1B8 = 50;
			if (boss->unk18C != nullptr) {
				JGeometry::TVec3<f32> position;
				boss->getJointTransByIndex(18, &position);
				position.y += 250.0f;
				boss->unk18C->mPos.value = position;
				gpModelWaterManager->emitRequest(*boss->unk18C);
			}
		}
		if (35.0f < frame)
			boss->unk1BC = 1;
	}
	return false;
}

DEFINE_NERVE(TNerveBPGetUp, TLiveActor)
{
	TBossPakkun* boss = static_cast<TBossPakkun*>(spine->getBody());
	MActor* actor     = boss->mMActor;
	if (spine->getTime() == 0) {
		boss->changeBck(14);
		gpCameraShake->startShake(static_cast<EnumCamShakeMode>(0x10), 1.0f);
		boss->rumblePad(0, boss->mPosition);
	}
	if (actor->curAnmEndsNext(0, nullptr)) {
		if (actor->checkCurBckFromIndex(14)) {
			boss->changeBck(19);
			return false;
		}
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveBPSwing, TLiveActor)
{
	TBossPakkun* boss = static_cast<TBossPakkun*>(spine->getBody());
	MActor* actor     = boss->mMActor;
	if (spine->getTime() == 0)
		boss->changeBck(15);
	if (spine->getTime() == 0) {
		gpMarioParticleManager->emitAndBindToSRTMtxPtr(
		    0xAC, boss->getModel()->getAnmMtx(18), 0, boss);
	}
	if (actor->curAnmEndsNext(0, nullptr))
		return true;
	return false;
}

DEFINE_NERVE(TNerveBPStompReact, TLiveActor)
{
	TBossPakkun* boss = static_cast<TBossPakkun*>(spine->getBody());
	MActor* actor     = boss->mMActor;

	if (spine->getTime() == 0) {
		boss->changeBck(5);
		boss->mHeadHit->onHitFlag(HIT_FLAG_NO_COLLISION);
	}

	if (spine->getTime() == 30 && boss->unk17C == 0) {
		boss->unk17C = 1;
		boss->unk174 = 0;
		boss->unk170 = 0;
		boss->unk1B8 = 50;

		if (boss->unk18C != nullptr) {
			JGeometry::TVec3<f32> position;
			boss->getJointTransByIndex(18, &position);
			position.y += 250.0f;
			boss->unk18C->mPos.value = position;
			gpModelWaterManager->emitRequest(*boss->unk18C);
		}
	}

	if (spine->getTime() == 50)
		boss->unk1BC = 1;

	if (actor->curAnmEndsNext(ANM_TYPE_BCK, nullptr)) {
		boss->mHeadHit->offHitFlag(HIT_FLAG_NO_COLLISION);
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveBPJumpReact, TLiveActor)
{
	TBossPakkun* boss = static_cast<TBossPakkun*>(spine->getBody());
	MActor* actor     = boss->mMActor;
	if (spine->getTime() == 0)
		boss->changeBck(17);
	if (actor->curAnmEndsNext(0, nullptr))
		return true;
	return false;
}

DEFINE_NERVE(TNerveBPPreDie, TLiveActor)
{
	TBossPakkun* boss = static_cast<TBossPakkun*>(spine->getBody());
	MActor* actor     = boss->mMActor;
	if (spine->getTime() == 0) {
		boss->changeBck(5);
		boss->mHeadHit->onHitFlag(HIT_FLAG_NO_COLLISION);
		if (boss->unk17C == 0) {
			boss->unk17C = 1;
			boss->unk174 = 0;
			boss->unk170 = 0;
			boss->unk1B8 = 50;
			if (boss->unk18C != nullptr) {
				JGeometry::TVec3<f32> position;
				boss->getJointTransByIndex(18, &position);
				position.y += 250.0f;
				boss->unk18C->mPos.value = position;
				gpModelWaterManager->emitRequest(*boss->unk18C);
			}
		}

		TNameKuriManager* manager
		    = JDrama::TNameRefGen::search<TNameKuriManager>(
		        "ナメクリマネージャー");
		if (manager != nullptr)
			manager->killChildren();
		MSBgm::stopTrackBGM(1, 10);
	}

	if (actor->curAnmEndsNext(ANM_TYPE_BCK, nullptr)) {
		spine->pushAfterCurrent(&TNerveBPDie::theNerve());
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveBPDie, TLiveActor)
{
	TBossPakkun* boss = static_cast<TBossPakkun*>(spine->getBody());
	MActor* actor     = boss->mMActor;

	SMSGetMSound()->unk98->modBgm(0, 1);

	if (actor->checkCurBckFromIndex(7) && spine->getTime() == 680)
		boss->onLiveFlag(LIVE_FLAG_UNK8);

	if (actor->curAnmEndsNext(ANM_TYPE_BCK, nullptr)
	    && actor->checkCurBckFromIndex(7)) {
		boss->kill();
		gpItemManager->makeShineAppearWithDemo(
		    "シャイン（ボス用）", "ボスシャインカメラ", boss->mPosition.x,
		    boss->mPosition.y, boss->mPosition.z);
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveBPTakeOff, TLiveActor)
{
	TBossPakkun* boss = static_cast<TBossPakkun*>(spine->getBody());
	MActor* actor     = boss->mMActor;
	if (spine->getTime() == 0) {
		boss->onLiveFlag(LIVE_FLAG_UNK10);
		boss->onLiveFlag(LIVE_FLAG_AIRBORNE);
		boss->changeBck(13);
	}

	if (actor->checkCurBckFromIndex(13)
	    && actor->curAnmEndsNext(ANM_TYPE_BCK, nullptr))
		boss->changeBck(11);

	if (actor->checkCurBckFromIndex(11)) {
		boss->mPosition.y += 5.0f;
		JGeometry::TVec3<f32> goal = boss->getUnk104().getPoint();
		if (goal.y < boss->mPosition.y) {
			boss->mPosition.y = goal.y;
			if (boss->getTracer()->getGraph() != nullptr)
				spine->pushAfterCurrent(&TNerveBPFly::theNerve());
			else
				spine->pushAfterCurrent(&TNerveBPTouchDown::theNerve());
			return true;
		}
	}
	return false;
}

DEFINE_NERVE(TNerveBPFly, TLiveActor)
{
	TBossPakkun* boss = static_cast<TBossPakkun*>(spine->getBody());
	if (spine->getTime() == 0) {
		boss->changeBck(11);
		boss->goToRandomNextGraphNode();
		if (boss->unk1CC == 0) {
			MSBgm::startBGM(MSD_BGM_MAP_SELECT);
			boss->unk1CC = 1;
		}
	}

	JGeometry::TVec3<f32> toNext = boss->getUnk104().getPoint();
	toNext -= boss->mPosition;
	toNext.y = 0.0f;
	if (PSVECMag(&toNext) < 100.0f) {
		if (!boss->getTracer()->getCurrent().checkFlag(0x800)) {
			boss->goToRandomNextGraphNode();
		} else {
			spine->pushAfterCurrent(&TNerveBPHover::theNerve());
			return true;
		}
	}

	boss->flyToCurPathNode(boss->getBossPakkunParams()->mSLFlySpeed.get(),
	                       boss->mTurnSpeed);
	return false;
}

DEFINE_NERVE(TNerveBPTouchDown, TLiveActor)
{
	TBossPakkun* boss = static_cast<TBossPakkun*>(spine->getBody());
	MActor* actor     = boss->mMActor;
	if (spine->getTime() == 0)
		boss->changeBck(11);

	if (actor->checkCurBckFromIndex(11)) {
		boss->mPosition.y -= 5.0f;
		JGeometry::TVec3<f32> goal = boss->getUnk104().getPoint();
		if (goal.y > boss->mPosition.y) {
			boss->mPosition.y = goal.y;
			boss->changeBck(18);
			boss->offLiveFlag(LIVE_FLAG_UNK10);
		}
	}

	if (actor->checkCurBckFromIndex(18)
	    && actor->curAnmEndsNext(ANM_TYPE_BCK, nullptr)) {
		spine->pushAfterCurrent(&TNerveBPWait::theNerve());
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveBPFlyCannon, TLiveActor)
{
	TBossPakkun* boss = static_cast<TBossPakkun*>(spine->getBody());
	MActor* actor     = boss->mMActor;
	if (spine->getTime() == 0)
		boss->changeBck(12);
	if (spine->getTime() == 168)
		boss->launchPolDrop();
	if (actor->curAnmEndsNext(0, nullptr))
		return true;
	return false;
}

DEFINE_NERVE(TNerveBPFlyPivot, TLiveActor)
{
	TBossPakkun* boss = static_cast<TBossPakkun*>(spine->getBody());
	if (spine->getTime() == 0)
		boss->changeBck(11);
	if (boss->turnToCurPathNode(
	        boss->getBossPakkunParams()->mSLPivotSpeed.get())) {
		boss->switchNextGoalPath();
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveBPHover, TLiveActor)
{
	TBossPakkun* boss = static_cast<TBossPakkun*>(spine->getBody());
	if (spine->getTime() == 0) {
		boss->changeBck(16);
		boss->unk16C = 3;
	}

	f32 range = boss->getBossPakkunParams()->mSLPollBallRange.get();
	if (boss->inArea(*gpMarioPos)
	    && boss->getDistToMarioSquared() < range * range) {
		spine->pushAfterCurrent(&TNerveBPHover::theNerve());
		spine->pushAfterCurrent(&TNerveBPFlyCannon::theNerve());

		TPathNode goal(*gpMarioPos);
		boss->unk114.push(boss->unkF4);
		boss->unkF4 = goal;

		spine->pushAfterCurrent(&TNerveBPFlyPivot::theNerve());
		return true;
	}

	if (spine->getTime() >= boss->getBossPakkunParams()->mSLHoverTimer.get()) {
		spine->pushAfterCurrent(&TNerveBPFly::theNerve());
		boss->unk16C = 0;
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveBPFall, TLiveActor)
{
	TBossPakkun* boss = static_cast<TBossPakkun*>(spine->getBody());
	MActor* actor     = boss->mMActor;
	if (spine->getTime() == 0) {
		boss->offLiveFlag(LIVE_FLAG_UNK10);
		boss->onLiveFlag(LIVE_FLAG_AIRBORNE);
		boss->changeBck(10);
	}

	if (actor->checkCurBckFromIndex(10)) {
		if (actor->curAnmEndsNext(ANM_TYPE_BCK, nullptr))
			boss->changeBck(9);
	} else if (actor->checkCurBckFromIndex(9)) {
		if (!boss->isAirborne()) {
			boss->changeBck(8);
			gpCameraShake->startShake(static_cast<EnumCamShakeMode>(15), 1.0f);
			boss->rumblePad(2, boss->mPosition);
		}
	} else if (actor->checkCurBckFromIndex(8)) {
		if (actor->curAnmEndsNext(ANM_TYPE_BCK, nullptr)) {
			boss->changeBck(14);
			gpCameraShake->startShake(static_cast<EnumCamShakeMode>(16), 1.0f);
			boss->rumblePad(0, boss->mPosition);
		}
	} else if (actor->checkCurBckFromIndex(14)
	           && actor->curAnmEndsNext(ANM_TYPE_BCK, nullptr)) {
		if (boss->is2ndFightNow()) {
			f32 tornadoProp = boss->getBossPakkunParams()->mSLTornadoProp.get();
			if (boss->mTornado->unk98 != 0
			    || rand() * (1.0f / 32768.0f) < tornadoProp) {
				spine->pushAfterCurrent(&TNerveBPTakeOff::theNerve());
				spine->pushAfterCurrent(&TNerveBPVomit::theNerve());
			} else if (boss->mTornado->unk98 == 0) {
				spine->pushAfterCurrent(&TNerveBPWait::theNerve());
				spine->pushAfterCurrent(&TNerveBPTornado::theNerve());
			} else {
				spine->pushAfterCurrent(&TNerveBPWait::theNerve());
			}
		} else {
			spine->pushAfterCurrent(&TNerveBPWait::theNerve());
		}
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveBPSleep, TLiveActor)
{
	TBossPakkun* boss = static_cast<TBossPakkun*>(spine->getBody());
	if (spine->getTime() == 0)
		boss->changeBck(23);
	return false;
}

DEFINE_NERVE(TNerveBPBreakSleep, TLiveActor)
{
	TBossPakkun* boss = static_cast<TBossPakkun*>(spine->getBody());
	if (spine->getTime() == 0) {
		boss->changeBck(14);
		MSBgm::stopTrackBGMs(7, 10);
	}
	if (boss->mMActor->curAnmEndsNext(ANM_TYPE_BCK, nullptr)) {
		spine->pushAfterCurrent(&TNerveBPTakeOff::theNerve());
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveBPWaitL, TLiveActor)
{
	TBossPakkun* boss = static_cast<TBossPakkun*>(spine->getBody());
	MActor* actor     = boss->mMActor;

	if (spine->getTime() == 0)
		actor->setBck("bosspaku_wait");

	if (spine->getTime()
	    >= boss->getBossPakkunParams()->mSLWaitFrameStg0.get()) {
		JGeometry::TVec3<f32>* marioPos = gpMarioPos;
		if (boss->unk188 == nullptr) {
			boss->unk188 = static_cast<TAreaCylinderManager*>(
			    gpConductor->search("ゲロエリアマネージャー"));
		}

		BOOL marioInArea = boss->unk188 == nullptr
		                       ? false
		                       : boss->unk188->contain(*marioPos);
		if (marioInArea) {
			if (!(*gpMarioGroundPlane)->isWaterSurface()) {
				spine->pushAfterCurrent(&TNerveBPCannonL::theNerve());
				return true;
			}
		}
	}

	if (actor->curAnmEndsNext(ANM_TYPE_BCK, nullptr)) {
		spine->pushAfterCurrent(&TNerveBPWaitL::theNerve());
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveBPCannonL, TLiveActor)
{
	TBossPakkun* boss = static_cast<TBossPakkun*>(spine->getBody());
	MActor* actor     = boss->mMActor;

	if (spine->getTime() == 0)
		actor->setBck("bosspaku_pollut_start");

	if (actor->curAnmEndsNext(ANM_TYPE_BCK, nullptr)) {
		if (actor->checkCurAnm("bosspaku_pollut_start", ANM_TYPE_BCK)) {
			actor->setBck("bosspaku_ball_end");
			boss->launchPolDrop();
		} else {
			spine->pushAfterCurrent(&TNerveBPWaitL::theNerve());
			return true;
		}
	}

	return false;
}
