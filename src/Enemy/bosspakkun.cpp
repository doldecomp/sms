#include <Enemy/BossPakkun.hpp>
#include <Enemy/AreaCylinder.hpp>
#include <Enemy/Conductor.hpp>
#include <Enemy/Graph.hpp>
#include <Enemy/NameKuri.hpp>
#include <Enemy/Walker.hpp>
#include <Camera/cameralib.hpp>
#include <Camera/CameraShake.hpp>
#include <GC2D/GCConsole2.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/JMath.hpp>
#include <Map/Map.hpp>
#include <Map/MapCollisionManager.hpp>
#include <Map/MapData.hpp>
#include <Map/PollutionManager.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <MarioUtil/ShadowUtil.hpp>
#include <MarioUtil/TexUtil.hpp>
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
#include <Strategic/Strategy.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/MarDirector.hpp>
#include <System/Particles.hpp>
#include <System/TargetArrow.hpp>
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
	unk6C.zero();
	initHitActor(0x800000F, 1, 0x80000000, 0.0f, 0.0f, 100.0f, 200.0f);
	offHitFlag(HIT_FLAG_NO_COLLISION);
	JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ")
	    ->getChildren()
	    .push_back(this);
}

void TBPPolDrop::drop()
{
	unk80 = 2;
	unk6C.zero();
	unk7C->setBck("pollut_ball_stamp");
	gpMarioParticleManager->emit(0x52, &mPosition, 0, nullptr);
	SMSGetMSound()->startSoundActor(MSD_SE_BS_BSPAKU_POLLUT_GND, &mPosition, 0,
	                                nullptr, 0, 4);
	mOwner->rumblePad(2, mPosition);
}

void TBPPolDrop::move()
{
	if (unk80 == 0) {
		onHitFlag(HIT_FLAG_NO_COLLISION);
		return;
	}

	JGeometry::TVec3<f32> nextPosition = mPosition;
	nextPosition += unk6C;

	if (unk80 == 1) {
		if (unk78->curAnmEndsNext(ANM_TYPE_BCK, nullptr))
			unk78->setBck("pollut_ball");

		unk6C.y -= 0.1f;
		if (unk84 >= 60 || mOwner->is2ndFightNow()) {
			const TBGCheckData* ground;
			f32 groundHeight = gpMap->checkGround(nextPosition.x, mPosition.y,
			                                      nextPosition.z, &ground);
			groundHeight += 1.0f;
			if (ground->isIllegalData())
				groundHeight = unk88;
			unk88 = groundHeight;

			if (nextPosition.y < groundHeight) {
				drop();
				nextPosition.y = groundHeight;
				onHitFlag(HIT_FLAG_NO_COLLISION);
				return;
			}

			offHitFlag(HIT_FLAG_NO_COLLISION);
			if (gpMap->isTouchedOneWallAndMoveXZ(
			        &nextPosition.x, nextPosition.y, &nextPosition.z, 80.0f))
				unk80 = 0;

			SMSGetMSound()->startSoundActorWithInfo(
			    MSD_SE_BS_BSPAKU_POLLUT_IMI, &mPosition, nullptr, -unk6C.y, 0,
			    0, nullptr, 0, 4);
			SMSGetMSound()->startSoundActorWithInfo(
			    MSD_SE_BS_BSPAKU_POLLUT_FLY, &mPosition, nullptr, -unk6C.y, 0,
			    0, nullptr, 0, 4);
		}
	} else if (unk80 == 2 && unk7C->curAnmEndsNext(ANM_TYPE_BCK, nullptr)) {
		unk80 = 0;
	}

	mPosition = nextPosition;
}

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

void TBPPolDrop::perform(u32 flags, JDrama::TGraphics* graphics)
{
	if (unk80 == 0)
		return;

	if (flags & CUE_MOVE) {
		move();
		++unk84;
	}

	if (flags & CUE_MOVE) {
		for (s32 i = 0; i < mColCount; ++i) {
			THitActor* collision = mCollisions[i];
			if (!collision->isActorType(0x80000001))
				continue;

			collision->receiveMessage(this, HIT_MESSAGE_ATTACK);
			mOwner->rumblePad(2, mPosition);
			if (SMS_IsMarioTouchGround4cm())
				unk80 = 2;
			else
				unk80 = 0;
		}
	}

	u32 calcAnim = flags & CUE_CALC_ANIM;
	if (calcAnim) {
		MtxPtr mtx = unk78->getModel()->getBaseTRMtx();
		MTXIdentity(mtx);
		mtx[0][3] = mPosition.x;
		mtx[1][3] = mPosition.y;
		mtx[2][3] = mPosition.z;
		unk78->getModel()->setBaseScale(mScaling);

		if (unk80 == 2) {
			f32 scale
			    = mOwner->getBossPakkunParams()->mSLPollBallStampScale.get();
			JGeometry::TVec3<f32> stampScale(scale, scale, scale);
			unk7C->getModel()->setBaseScale(stampScale);
			MTXCopy(mtx, unk7C->getModel()->getBaseTRMtx());
		}
	}

	if (unk80 == 1) {
		unk78->perform(flags, graphics);

		if (flags & CUE_CALC_VIEW) {
			TCircleShadowRequest request;
			request.unk0  = mPosition;
			request.unkC  = 400.0f;
			request.unk10 = 400.0f;
			request.unk14 = 0.0f;
			request.unk1C = 0;
			gpBindShadowManager->request(request, 0);
		}
	}

	if (unk80 == 2) {
		if (calcAnim)
			unk7C->calcAnm();
		if (flags & CUE_ENTRY)
			gpPollution->stampModel(unk7C->getModel());
	}
}

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

void TBPVomit::vomitFinished()
{
	unk14->setBckFromIndex(-1);
	unk18->setBckFromIndex(-1);
}

void TBPVomit::perform(u32 flags, JDrama::TGraphics* graphics)
{
	if (unk14->getCurAnmIdx(ANM_TYPE_BCK) < 0)
		return;

	u32 calcAnim = flags & CUE_CALC_ANIM;
	if (calcAnim && unk14->curAnmEndsNext(ANM_TYPE_BCK, nullptr)) {
		vomitFinished();
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
    , unk94(0.0f)
    , unk98(0)
{
	mActor = mOwner->getActorKeeper()->createMActor("trunade.bmd", 0);
	initHitActor(0x8000010, 5, 0x81000000, 150.0f, 600.0f, 100.0f, 600.0f);
	onHitFlag(HIT_FLAG_NO_COLLISION);
	mActor->setBtkFromIndex(2);
	mActor->setBckFromIndex(29);
	mActor->setBrkFromIndex(1);
	mScaling.set(2.0f, 2.0f, 2.0f);
}

void TBPTornado::vanish()
{
	onHitFlag(HIT_FLAG_NO_COLLISION);
	unk98 = 2;

	J3DFrameCtrl* frameCtrl = mActor->getFrameCtrl(ANM_TYPE_BRK);
	frameCtrl->setFrame(0.0f);
	frameCtrl->setRate(SMSGetAnmFrameRate());
}

void TBPTornado::perform(u32 flags, JDrama::TGraphics* graphics)
{
	THitActor::perform(flags, graphics);

	if (mOwner->checkLiveFlag(LIVE_FLAG_DEAD))
		return;
	if (unk98 == 0)
		return;

	if (flags & CUE_MOVE) {
		if (unk98 == 2) {
			mPosition += unk88;
			if (mActor->curAnmEndsNext(ANM_TYPE_BRK, nullptr)) {
				unk98 = 0;
				return;
			}
		} else {
			unk94 += mOwner->getBossPakkunParams()->mSLTornadoMoveInc.get();
			if (unk94
			    > mOwner->getBossPakkunParams()->mSLTornadoMoveLimit.get()) {
				vanish();
				return;
			}

			f32 angle = 360.0f - static_cast<s32>(unk94) % 360;
			f32 radius
			    = unk94
			      * mOwner->getBossPakkunParams()->mSLTornadoRollSpeed.get();
			JGeometry::TVec3<f32> direction;
			direction = unk70;
			direction.sub(unk7C);
			if (PSVECMag(&direction) < 100.0f) {
				vanish();
				return;
			}

			PSVECNormalize(&direction, &direction);
			direction *= mOwner->getBossPakkunParams()->mSLTornadoSpeed.get();
			unk7C += direction;

			s16 shortAngle = DEG2SHORTANGLE(angle);
			JGeometry::TVec3<f32> nextPosition(
			    unk7C.x + radius * JMASCos(shortAngle), unk7C.y,
			    unk7C.z + radius * JMASSin(shortAngle));

			const TBGCheckData* ground;
			f32 groundHeight
			    = gpMap->checkGround(nextPosition.x, nextPosition.y + 200.0f,
			                         nextPosition.z, &ground);
			if (!ground->isIllegalData())
				nextPosition.y = groundHeight;

			if (gpMap->isTouchedOneWallAndMoveXZ(
			        &nextPosition.x, nextPosition.y, &nextPosition.z, 80.0f))
				vanish();

			unk88 = nextPosition;
			unk88.sub(mPosition);
			mPosition = nextPosition;

			for (s32 i = 0; i < mColCount; ++i) {
				if (!mCollisions[i]->isActorType(0x80000001))
					continue;

				static const JGeometry::TVec3<f32> up(0.0f, 1.0f, 0.0f);
				SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
				SMS_SendMessageToMario(this, HIT_MESSAGE_THROWN);
				SMS_ThrowMario(up, 100.0f);
				vanish();
			}
		}
	}

	if (flags & CUE_CALC_ANIM) {
		J3DModel* model = mActor->getModel();
		MtxPtr mtx      = model->getBaseTRMtx();
		MTXIdentity(mtx);
		mtx[0][3] = mPosition.x;
		mtx[1][3] = mPosition.y;
		mtx[2][3] = mPosition.z;
		model->setBaseScale(mScaling);
	}

	if (flags & CUE_CALC_ANIM) {
		MtxPtr mtx = mActor->getModel()->getBaseTRMtx();
		JPABaseEmitter* emitter
		    = gpMarioParticleManager->emitAndBindToMtxPtr(0x162, mtx, 1, this);
		if (emitter)
			emitter->setGlobalScale(mScaling);

		emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
		    0x163, mtx, 1, reinterpret_cast<u8*>(this) + 1);
		if (emitter)
			emitter->setGlobalScale(mScaling);

		emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
		    0x164, mtx, 1, reinterpret_cast<u8*>(this) + 2);
		if (emitter)
			emitter->setGlobalScale(mScaling);
	}

	if (flags & CUE_CALC_ANIM) {
		JGeometry::TVec3<f32> toMario;
		toMario = mPosition;
		toMario.sub(*gpMarioPos);
		SMSGetMSound()->startSoundActorWithInfo(
		    MSD_SE_BS_BSPAKU_TORNADO, &mPosition, nullptr, toMario.length(), 0,
		    0, nullptr, 0, 4);
	}

	mActor->perform(flags, graphics);
}

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
		boss->gotFlyingDamage();
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
			boss->gotWaterDamage();
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
    , unk1CC(0)
{
	offLiveFlag(LIVE_FLAG_UNK100);
	mBinder = new TWalker;
}

void TBossPakkun::init(TLiveManager* manager)
{
	mManager = manager;
	mManager->manageActor(this);

	mMActorKeeper = new TMActorKeeper(mManager, 7);
	mMActor       = mMActorKeeper->createMActor("bosspaku_model.bmd", 0);

	if (static_cast<TBossPakkunManager*>(mManager)->unk54 == 0) {
		unk180 = mMActorKeeper->createMActor("bosspaku_end.bmd", 0);
		unk180->setBckFromIndex(7);
		unk180->setBrkFromIndex(0);
	}

	TIdxGroupObj* group
	    = JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ");
	initHitActor(0x800000F, 1, 0x80000000, 80.0f, 300.0f, 80.0f, 300.0f);
	offHitFlag(HIT_FLAG_NO_COLLISION);

	if (static_cast<TBossPakkunManager*>(mManager)->unk54 == 0) {
		mHeadHit = new TBPHeadHit(this, "ボスパックン頭部");
		mNavel   = new TBPNavel(this, "ボスパックンへそ");
		group->getChildren().push_back(mHeadHit);
		group->getChildren().push_back(mNavel);

		mMapCollisionManager
		    = new TMapCollisionManager(1, "/scene/bosspakkun", this);
		mMapCollisionManager->init("col_body.col", 1, nullptr);

		mMapCollisionManager->setUpUnk8TRS(mPosition, mRotation, mScaling);

		mMtxCalc = new TBossPakkunMtxCalc(this);
		mMActor->setCalcForBck(mMtxCalc);
		mMActor->calc();
	}

	if (static_cast<TBossPakkunManager*>(mManager)->unk54 != 0) {
		mSpine->initWith(&TNerveBPWaitL::theNerve());
	} else if (gpMarDirector->mMap == 0x37) {
		mSpine->initWith(&TNerveBPFall::theNerve());
	} else if (gpMarDirector->unk7D == 4) {
		mSpine->initWith(&TNerveBPSleep::theNerve());
	} else {
		mSpine->initWith(&TNerveBPWait::theNerve());
	}

	mPolDrop        = new TBPPolDrop(this);
	MActor* stamp   = mMActorKeeper->createMActor("pollut_ball_stamp.bmd", 0);
	mPolDrop->unk78 = mMActorKeeper->createMActor("pollut_ball.bmd", 0);
	mPolDrop->unk7C = stamp;
	const ResTIMG* res = static_cast<const ResTIMG*>(
	    JKRFileLoader::getGlbResource("/scene/map/pollution/H_ma_rak.bti"));
	if (res != nullptr) {
		SMS_ChangeTextureAll(mPolDrop->unk78->getModel()->getModelData(),
		                     "M_dummy", *res);
	}

	if (static_cast<TBossPakkunManager*>(mManager)->unk54 == 0) {
		mVomit = new TBPVomit(this, "<TBPVomit>");
		MActor* white
		    = mMActorKeeper->createMActor("bosspakuPollut_white.bmd", 0);
		MActor* pollut = mMActorKeeper->createMActor("bosspakuPollut.bmd", 0);
		mVomit->unk14  = pollut;
		mVomit->unk18  = white;

		mTornado = new TBPTornado(this, "<TBPTornado>");
		group->getChildren().push_back(mTornado);

		unk18C = new TWaterEmitInfo("/enemy/bosspakuwater.prm");
	}

	initAnmSound();
	onLiveFlag(LIVE_FLAG_UNK400);
	mScaledBodyRadius = 400.0f;

	unk124->setGraph(gpConductor->getGraphByName("bosspakkun"));
	if (unk124->getGraph() != nullptr) {
		unk124->reset();
		goToShortestNextGraphNode();
	}

	mHitPoints = getMaxHitPoints();
}

BOOL TBossPakkun::checkMarioRiding()
{
	const TBGCheckData* ground = SMS_GetMarioGrPlane();
	if (unk190 == 0) {
		if (ground != nullptr && ground->getActor() == this
		    && SMS_IsMarioTouchGround4cm()) {
			u32 status = SMS_GetMarioStatus();
			if ((status & MARIO_STATUS_FLAG_UNK200)
			    && !(status & MARIO_STATUS_FLAG_UNK200000)) {
				unk190 = 1;
				return true;
			}
		}
	} else if (ground == nullptr || ground->getActor() != this
	           || !SMS_IsMarioTouchGround4cm()) {
		unk190 = 0;
	}
	return false;
}

void TBossPakkun::startBGM()
{
	if (unk1CC == 0) {
		MSBgm::startBGM(MSD_BGM_MAP_SELECT);
		unk1CC = 1;
	}
}

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

void TBossPakkun::showMessage(u32 message)
{
	u32 index = message - 0xe0000;
	u32 mask;
	if (index == 1)
		mask = 0;
	else
		mask = 1 << index;

	if (!(unk1C0 & mask))
		gpMarDirector->getConsole()->startAppearBalloon(message, true);
	unk1C0 |= mask;
}

bool TBossPakkun::is2ndFightNow() const
{
	if (gpMarDirector->mMap == 2 && gpMarDirector->unk7D == 4)
		return true;
	return false;
}

void TBossPakkun::ignoreWaterCheck() { }

void TBossPakkun::startTornadoBlur()
{
	gpMarioParticleManager->emitAndBindToPosPtr(0xa9, &unk194, 0, nullptr);
	gpMarioParticleManager->emitAndBindToPosPtr(0xa9, &unk1A0, 0, nullptr);
}

void TBossPakkun::resetWaterMark()
{
	if (unk17C == 0) {
		unk17C = 1;
		unk174 = 0;
		unk170 = 0;
		unk1B8 = 50;
		if (unk18C != nullptr) {
			JGeometry::TVec3<f32> position;
			getJointTransByIndex(18, &position);
			position.y += 250.0f;
			unk18C->mPos.value = position;
			gpModelWaterManager->emitRequest(*unk18C);
		}
	}
}

BOOL TBossPakkun::inArea(const JGeometry::TVec3<f32>& position)
{
	if (unk188 == nullptr) {
		unk188 = static_cast<TAreaCylinderManager*>(
		    gpConductor->search("ゲロエリアマネージャー"));
	}

	return unk188 == nullptr ? false : unk188->contain(position);
}

void TBossPakkun::gotFlyingDamage()
{
	unk16C = 0;
	mSpine->reset();
	mSpine->setNext(&TNerveBPFall::theNerve());
	SMSGetMSound()->startSoundActor(MSD_SE_BS_BSPAKU_FALL, &mPosition, 0,
	                                nullptr, 0, 4);
}

void TBossPakkun::gotWaterDamage()
{
	if (unk17C == 0) {
		unk170 += 1;
		if (unk178 < getBossPakkunParams()->mSLWaterMarkLimit.get())
			unk178 += 1;
		unk174 = getBossPakkunParams()->mSLWaterHitTimer.get();

		if (mSpine->getLatestNerve() != &TNerveBPSwallow::theNerve()) {
			mSpine->reset();
			mSpine->setNext(&TNerveBPSwallow::theNerve());
		}
	}
}

void TBossPakkun::gotHipDropDamage()
{
	decHitPoints();
	unk16C = 0;
	if (getHitPoints() == 0) {
		if (&TNerveBPPreDie::theNerve() != mSpine->getLatestNerve()
		    && &TNerveBPDie::theNerve() != mSpine->getLatestNerve()) {
			mSpine->setNext(&TNerveBPPreDie::theNerve());
			SMSGetMSound()->startSoundActor(MSD_SE_BS_BSPAKU_DOWN, &mPosition,
			                                0, nullptr, 0, 4);
		}
	} else if (&TNerveBPTumbleOut::theNerve() != mSpine->getLatestNerve()) {
		SMSGetMSound()->startSoundActor(MSD_SE_BS_BSPAKU_DAMAGE, &mPosition, 0,
		                                nullptr, 0, 4);
		if (gpMarDirector->getCurrentStage() == 4) {
			mSpine->reset();
			mSpine->setNext(&TNerveBPTakeOff::theNerve());
			mSpine->pushNerve(&TNerveBPGetUp::theNerve());
			mSpine->pushNerve(&TNerveBPStompReact::theNerve());
		} else {
			mSpine->reset();
			mSpine->setNext(&TNerveBPWait::theNerve());
			mSpine->pushNerve(&TNerveBPGetUp::theNerve());
			mSpine->pushNerve(&TNerveBPStompReact::theNerve());
		}
	}
}

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

void TBossPakkun::launchTornado() { mTornado->launch(*gpMarioPos); }

void TBossPakkun::killSmallEnemies()
{
	TNameKuriManager* manager
	    = JDrama::TNameRefGen::search<TNameKuriManager>("ナメクリマネージャー");
	if (manager != nullptr)
		manager->killChildren();
}

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

void TBossPakkun::setGroundCollision()
{
	const TNerveBase<TLiveActor>* dieNerve = &TNerveBPDie::theNerve();
	if (mSpine->getLatestNerve() != dieNerve
	    && mMapCollisionManager != nullptr) {
		TPosition3f collisionMtx;
		collisionMtx.set(getModel()->getAnmMtx(2));
		TMapCollisionBase* collision = mMapCollisionManager->unk8;
		if (collision != nullptr)
			collision->moveMtx(collisionMtx);
	}
}

void TBossPakkun::kill()
{
	TLiveActor::kill();
	onHitFlag(HIT_FLAG_NO_COLLISION);
	if (mHeadHit != nullptr)
		mHeadHit->onHitFlag(HIT_FLAG_NO_COLLISION);
	if (mNavel != nullptr)
		mNavel->onHitFlag(HIT_FLAG_NO_COLLISION);
	if (mPolDrop != nullptr)
		mPolDrop->onHitFlag(HIT_FLAG_NO_COLLISION);
}

BOOL TBossPakkun::receiveMessage(THitActor*, u32) { return false; }

void TBossPakkun::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (mPolDrop != nullptr)
		mPolDrop->perform(cue, graphics);
	if (mVomit != nullptr)
		mVomit->perform(cue, graphics);
	if (mTornado != nullptr)
		mTornado->perform(cue, graphics);

	if (checkLiveFlag(LIVE_FLAG_DEAD))
		return;

	if (mHeadHit != nullptr)
		mHeadHit->perform(cue, graphics);
	if (mNavel != nullptr)
		mNavel->perform(cue, graphics);

	if (static_cast<TBossPakkunManager*>(mManager)->unk54 == 0
	    && (cue & CUE_MOVE)) {
		mMtxCalc->advanceMotionBlend(-unk154);

		if (unk17C) {
			if (unk178 <= 0) {
				unk17C = 0;
				unk178 = 0;
			} else {
				s32 step = getBossPakkunParams()->mSLWaterMarkLimit.get() / 100;
				if (step == 0)
					step = 1;
				unk178 -= step;
				if (unk178 < 0)
					unk178 = 0;
			}
		}

		if (unk17C == 0) {
			if (unk174 > 0) {
				unk174--;
				unk170++;
			}
		}

		if (unk1BC) {
			if (unk1B8 <= 0) {
				unk1BC = 0;
				unk1B8 = 0;
			} else {
				unk1B8--;
			}
		}

		if (unk16C == 1 && checkMarioRiding()) {
			if (&TNerveBPJumpReact::theNerve() != mSpine->getLatestNerve()) {
				mSpine->pushNerve(&TNerveBPJumpReact::theNerve());
			}
		}
	}

	if (static_cast<TBossPakkunManager*>(mManager)->unk54 == 0
	    && (cue & CUE_CALC_ANIM)) {
		if (mMActor->checkCurBckFromIndex(24)) {
			MtxPtr mtx = mMActor->getModel()->getAnmMtx(43);
			unk194.set(mtx[0][3], mtx[1][3], mtx[2][3]);
			mtx = mMActor->getModel()->getAnmMtx(44);
			unk1A0.set(mtx[0][3], mtx[1][3], mtx[2][3]);
		}

		if (mMActor->checkCurBckFromIndex(23)) {
			MtxPtr mtx = mMActor->getModel()->getAnmMtx(20);
			unk1AC.set(mtx[0][3], mtx[1][3], mtx[2][3]);
			JPABaseEmitter* emitter
			    = gpMarioParticleManager->emitAndBindToPosPtr(
			        PARTICLE_MS_POI_ZZZ, &unk1AC, 1, this);
			if (emitter != nullptr) {
				static JGeometry::TVec3<f32> scale(2.5f, 2.5f, 2.5f);
				emitter->setGlobalScale(scale);
			}
		}

		if (mMActor->checkCurBckFromIndex(11)
		    || mMActor->checkCurBckFromIndex(13)
		    || mMActor->checkCurBckFromIndex(12)) {
			gpMarioParticleManager->emitAndBindToMtxPtr(
			    0x160, getModel()->getAnmMtx(38), 1, this);
			gpMarioParticleManager->emitAndBindToMtxPtr(
			    0x160, getModel()->getAnmMtx(46), 1, (u8*)this + 1);
		}

		if (mMActor->checkCurBckFromIndex(11)
		    || mMActor->checkCurBckFromIndex(6)
		    || mMActor->checkCurBckFromIndex(26)) {
			gpMarioParticleManager->emitAndBindToMtxPtr(
			    0x15F, getModel()->getAnmMtx(20), 1, (u8*)this + 1);
		}
	}

	if (static_cast<TBossPakkunManager*>(mManager)->unk54 == 0) {
		if (&TNerveBPDie::theNerve() == mSpine->getLatestNerve()) {
			MActor* origActor = mMActor;
			mMActor           = unk180;
			TSpineEnemy::perform(cue, graphics);
			mMActor = origActor;
			return;
		}
	}

	if (static_cast<TBossPakkunManager*>(mManager)->unk54 == 0
	    && (cue & CUE_CALC_ANIM)) {
		updateSquareToMario();
		getModel()->getModelData()->getJointNodePointer(0)->setMtxCalc(
		    mMtxCalc);
	}

	if (static_cast<TBossPakkunManager*>(mManager)->unk54 == 0
	    && (cue & CUE_CALC_ANIM)) {
		if (unk16C == 1) {
			JGeometry::TVec3<f32> pos = mNavel->mPosition;
			pos.y += 100.0f;
			gpTargetArrow->unk14 = 1;
			gpTargetArrow->setPos(pos);
		} else {
			gpTargetArrow->unk14 = 0;
		}
	}

	if (static_cast<TBossPakkunManager*>(mManager)->unk54 == 0
	    && (cue & CUE_ENTRY)) {
		if (&TNerveBPPreDie::theNerve() == mSpine->getLatestNerve()
		    || &TNerveBPStompReact::theNerve() == mSpine->getLatestNerve()) {
			mMActor->offMakeDL();
			SMS_AddDamageFogEffect(mMActor->getModel()->getModelData(),
			                       mPosition, graphics);
		} else {
			SMS_ResetDamageFogEffect(mMActor->getModel()->getModelData());
		}
	}

	TSpineEnemy::perform(cue, graphics);
}

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
		offset = fromPolar(static_cast<s16>(DEG2SHORTANGLE(boss->mRotation.y)),
		                   700.0f);
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
		boss->startTornadoBlur();
	}
	if (spine->getTime() == 150)
		boss->launchTornado();
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
		if (140.0f < frame && frame < 160.0f)
			boss->resetWaterMark();
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

	if (spine->getTime() == 30)
		boss->resetWaterMark();

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
		boss->resetWaterMark();
		boss->killSmallEnemies();
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
		boss->startBGM();
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
