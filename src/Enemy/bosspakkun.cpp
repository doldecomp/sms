#include <Enemy/BossPakkun.hpp>
#include <Camera/CameraShake.hpp>
#include <Enemy/AreaCylinder.hpp>
#include <Enemy/Conductor.hpp>
#include <Enemy/Graph.hpp>
#include <Enemy/Walker.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Strategic/LiveActor.hpp>
#include <Map/MapCollisionManager.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/Strategy.hpp>
#include <M3DUtil/MActor.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <MarioUtil/TexUtil.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <MarioUtil/ShadowUtil.hpp>
#include <Map/Map.hpp>
#include <MoveBG/ItemManager.hpp>
#include <Player/ModelWaterManager.hpp>
#include <Map/MapData.hpp>
#include <Map/PollutionManager.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/JMath.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <Player/Mario.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/MarioStatus.hpp>
#include <System/MarDirector.hpp>
#include <System/Particles.hpp>
#include <System/TargetArrow.hpp>
#include <GC2D/GCConsole2.hpp>
#include <MSound/MSModBgm.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// The .bas table the shared model's BCK slots index into. Slots with no sound
// list are null; see the BOSSPAKU_BCK_* enum for the names the .bck files carry.
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

TBossPakkunParams::TBossPakkunParams(const char* prm)
    : TSpineEnemyParams(prm)
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
    , mBallMActor(nullptr)
    , mStampMActor(nullptr)
    , mState(BOSSPAKU_POLDROP_DEAD)
    , mFlyTimer(0)
    , mGroundY(0.0f)
{
	mVelocity.x = mVelocity.y = mVelocity.z = 0.0f;

	initHitActor(0x800000F, 1, 0x80000000, 0.0f, 0.0f, 100.0f, 200.0f);
	offHitFlag(HIT_FLAG_NO_COLLISION);

	TIdxGroupObj* group
	    = JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ");
	group->getChildren().push_back(this);
}

// UNUSED, 0xa8 in the map: move() inlines it when the ball reaches the ground.
void TBPPolDrop::drop()
{
	mState      = BOSSPAKU_POLDROP_STAMPED;
	mVelocity.x = mVelocity.y = mVelocity.z = 0.0f;
	mStampMActor->setBck("pollut_ball_stamp");
	gpMarioParticleManager->emit(PARTICLE_MS_OSENDAN, &mPosition, 0,
	                             nullptr);
	if (gpMSound->gateCheck(MSD_SE_BS_BSPAKU_POLLUT_GND))
		MSoundSESystem::MSoundSE::startSoundActor(MSD_SE_BS_BSPAKU_POLLUT_GND,
		                                          &mPosition, 0, nullptr, 0,
		                                          4);
	mOwner->rumblePad(2, mPosition);
}

void TBPPolDrop::move()
{
	if (mState == BOSSPAKU_POLDROP_DEAD) {
		onHitFlag(HIT_FLAG_NO_COLLISION);
		return;
	}

	JGeometry::TVec3<f32> pos = mPosition;
	pos.x += mVelocity.x;
	pos.y += mVelocity.y;
	pos.z += mVelocity.z;

	if (mState == BOSSPAKU_POLDROP_FLYING) {
		if (mBallMActor->curAnmEndsNext(ANM_TYPE_BCK, nullptr))
			mBallMActor->setBck("pollut_ball");

		mVelocity.y -= 0.1f;

		if (mFlyTimer >= 60 || mOwner->is2ndFightNow()) {
			const TBGCheckData* ground;
			// `x += 1.0f` after the call puts the return in f1 first
			// (`fadds f31, f1, f0`); `checkGround() + 1.0f` swapped them.
			// TODO: 99.8%, frame 0x30 vs retail 0x70. drop() is UNUSED 0xa8
			// and already spelled out here; the remaining 0x40 is accessor
			// pool inside that expansion.
			f32 groundY
			    = gpMap->checkGround(pos.x, mPosition.y, pos.z, &ground);
			groundY += 1.0f;
			if (ground->isIllegalData())
				groundY = mGroundY;
			mGroundY = groundY;

			if (pos.y < groundY) {
				drop();
				pos.y = groundY;
				onHitFlag(HIT_FLAG_NO_COLLISION);
				return;
			}

			offHitFlag(HIT_FLAG_NO_COLLISION);
			if (gpMap->isTouchedOneWallAndMoveXZ(&pos.x, pos.y, &pos.z, 80.0f))
				mState = BOSSPAKU_POLDROP_DEAD;

			f32 fallSpeed = -mVelocity.y;
			if (gpMSound->gateCheck(MSD_SE_BS_BSPAKU_POLLUT_IMI))
				MSoundSESystem::MSoundSE::startSoundActorWithInfo(
				    MSD_SE_BS_BSPAKU_POLLUT_IMI, &mPosition, nullptr,
				    fallSpeed, 0, 0, nullptr, 0, 4);

			fallSpeed = -mVelocity.y;
			if (gpMSound->gateCheck(MSD_SE_BS_BSPAKU_POLLUT_FLY))
				MSoundSESystem::MSoundSE::startSoundActorWithInfo(
				    MSD_SE_BS_BSPAKU_POLLUT_FLY, &mPosition, nullptr,
				    fallSpeed, 0, 0, nullptr, 0, 4);
		}
	} else if (mState == BOSSPAKU_POLDROP_STAMPED) {
		if (mStampMActor->curAnmEndsNext(ANM_TYPE_BCK, nullptr))
			mState = BOSSPAKU_POLDROP_DEAD;
	}

	mPosition = pos;
}

// UNUSED, 0xd0 in the map: TBossPakkun::launchPolDrop inlines it.
void TBPPolDrop::launch(const JGeometry::TVec3<f32>& from,
                        const JGeometry::TVec3<f32>& velocity)
{
	mVelocity = velocity;
	mPosition = from;
	mScaling.set(1.0f, 1.0f, 1.0f);
	mRotation.x = mRotation.y = mRotation.z = 0.0f;
	mState                                  = BOSSPAKU_POLDROP_FLYING;
	mFlyTimer                               = 0;
	mBallMActor->setBck("pollut_ball");
	mBallMActor->setBtk("pollut_ball_01");
	mBallMActor->setBtk("pollut_ball_02");
	mGroundY = from.y;
}

void TBPPolDrop::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (mState == BOSSPAKU_POLDROP_DEAD)
		return;

	if (cue & 1) {
		move();
		mFlyTimer += 1;
	}

	if (cue & 1) {
		for (int i = 0; i < getColNum(); ++i) {
			THitActor* other = getCollision(i);
			if (other->isActorType(ACTOR_TYPE_PLAYER | 1)) {
				other->receiveMessage(this, HIT_MESSAGE_ATTACK);
				mOwner->rumblePad(2, mPosition);
				if (SMS_IsMarioTouchGround4cm())
					mState = BOSSPAKU_POLDROP_STAMPED;
				else
					mState = BOSSPAKU_POLDROP_DEAD;
			}
		}
	}

	if (cue & 2) {
		MtxPtr mtx = mBallMActor->getModel()->getBaseTRMtx();
		MTXIdentity(mtx);
		mtx[0][3] = mPosition.x;
		mtx[1][3] = mPosition.y;
		mtx[2][3] = mPosition.z;
		mBallMActor->getModel()->setBaseScale(mScaling);

		if (mState == BOSSPAKU_POLDROP_STAMPED) {
			JGeometry::TVec3<f32> scale;
			scale.z = scale.y = scale.x
			    = mOwner->getSaveParam2()->mSLPollBallStampScale.value;
			mStampMActor->getModel()->setBaseScale(scale);
			MTXCopy(mtx, mStampMActor->getModel()->getBaseTRMtx());
		}
	}

	if (mState == BOSSPAKU_POLDROP_FLYING) {
		mBallMActor->perform(cue, graphics);
		if (cue & 4) {
			TCircleShadowRequest request;
			request.mPosition   = mPosition;
			request.mRadiusX    = request.mRadiusZ = 400.0f;
			request.mRotationY  = 0.0f;
			request.mShadowType = SHADOW_TYPE_CIRCLE;
			gpBindShadowManager->request(request, 0);
		}
	}

	if (mState == BOSSPAKU_POLDROP_STAMPED) {
		if (cue & 2)
			mStampMActor->calcAnm();
		if (cue & 0x200)
			gpPollution->stampModel(mStampMActor->getModel());
	}
}

// UNUSED, 0x80 in the map: TBossPakkun::init spells it out.
TBPVomit::TBPVomit(TBossPakkun* owner, const char* name)
    : JDrama::TViewObj(name)
    , mOwner(owner)
    , mMActor(nullptr)
    , mStampMActor(nullptr)
{
}

// UNUSED, 0xc4 in the map: the Vomit nerve inlines it. Both models are parked
// on the boss's own base matrix and scale, so the puddle lands at its feet.
void TBPVomit::vomit()
{
	mMActor->setBckFromIndex(0);
	mStampMActor->setBckFromIndex(1);

	MtxPtr base = mOwner->getModel()->getBaseTRMtx();
	MTXCopy(base, mMActor->getModel()->getBaseTRMtx());
	mMActor->getModel()->setBaseScale(mOwner->getScaling());
	MTXCopy(base, mStampMActor->getModel()->getBaseTRMtx());
	mStampMActor->getModel()->setBaseScale(mOwner->getScaling());
}

// UNUSED, 0x3c in the map: perform() inlines it once the puddle animation has
// run out. TODO: one instruction over the map's size.
void TBPVomit::vomitFinished()
{
	mMActor->setBckFromIndex(-1);
	mStampMActor->setBckFromIndex(-1);
}

void TBPVomit::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (mMActor->getCurAnmIdx(ANM_TYPE_BCK) < 0)
		return;

	if ((cue & 2) && mMActor->curAnmEndsNext(ANM_TYPE_BCK, nullptr)) {
		vomitFinished();
		return;
	}

	if (cue & 2)
		mStampMActor->calcAnm();

	if (cue & 0x200)
		gpPollution->stampModel(mStampMActor->getModel());

	mMActor->perform(cue, graphics);
}

TBPTornado::TBPTornado(TBossPakkun* owner, const char* name)
    : THitActor(name)
    , mOwner(owner)
    , mMove(0.0f)
    , mState(BOSSPAKU_TORNADO_DEAD)
{
	mMActor = mOwner->getActorKeeper()->createMActor("trunade.bmd", 0);

	initHitActor(0x8000010, 5, 0x81000000, 150.0f, 600.0f, 100.0f, 600.0f);
	onHitFlag(HIT_FLAG_NO_COLLISION);

	mMActor->setBtkFromIndex(2);
	mMActor->setBckFromIndex(29);
	mMActor->setBrkFromIndex(1);

	mScaling.set(2.0f, 2.0f, 2.0f);
}

// UNUSED, 0x58 in the map: perform spells the three vanish sites out.
void TBPTornado::vanish()
{
	onHitFlag(HIT_FLAG_NO_COLLISION);
	mState = BOSSPAKU_TORNADO_VANISH;

	J3DFrameCtrl* ctrl = mMActor->getFrameCtrl(ANM_TYPE_BRK);
	ctrl->setFrame(0.0f);
	ctrl->setRate(SMSGetAnmFrameRate());
}

void TBPTornado::perform(u32 cue, JDrama::TGraphics* graphics)
{
	THitActor::perform(cue, graphics);

	if (mOwner->checkLiveFlag(LIVE_FLAG_DEAD))
		return;
	if (mState == BOSSPAKU_TORNADO_DEAD)
		return;

	if (cue & 1) {
		if (mState == BOSSPAKU_TORNADO_VANISH) {
			mPosition.x += mVelocity.x;
			mPosition.y += mVelocity.y;
			mPosition.z += mVelocity.z;
			if (mMActor->curAnmEndsNext(ANM_TYPE_BRK, nullptr)) {
				mState = BOSSPAKU_TORNADO_DEAD;
				return;
			}
		} else {
			mMove += mOwner->getSaveParam2()->mSLTornadoMoveInc.get();
			if (mMove > mOwner->getSaveParam2()->mSLTornadoMoveLimit.get()) {
				vanish();
				return;
			}

			f32 angle  = 360.0f - (f32)((int)mMove % 360);
			f32 radius = mMove
			             * mOwner->getSaveParam2()->mSLTornadoRollSpeed.get();

			JGeometry::TVec3<f32> toGoal = mTarget;
			toGoal.x -= mCenter.x;
			toGoal.y -= mCenter.y;
			toGoal.z -= mCenter.z;

			if (VECMag(toGoal) < 100.0f) {
				vanish();
				return;
			}

			VECNormalize(toGoal, toGoal);
			f32 step = mOwner->getSaveParam2()->mSLTornadoSpeed.get();
			toGoal.x *= step;
			toGoal.y *= step;
			toGoal.z *= step;
			mCenter.x += toGoal.x;
			mCenter.y += toGoal.y;
			mCenter.z += toGoal.z;

			JGeometry::TVec3<f32> next(mCenter.x + radius * MsCos(angle),
			                           mCenter.y,
			                           mCenter.z + radius * MsSin(angle));

			const TBGCheckData* ground;
			f32 groundY
			    = gpMap->checkGround(next.x, 200.0f + next.y, next.z, &ground);
			if (!ground->isIllegalData())
				next.y = groundY;

			if (gpMap->isTouchedOneWallAndMoveXZ(&next.x, next.y, &next.z,
			                                     80.0f)) {
				vanish();
			}

			mVelocity = next;
			mVelocity.x -= mPosition.x;
			mVelocity.y -= mPosition.y;
			mVelocity.z -= mPosition.z;
			mPosition = next;

			for (int i = 0; i < getColNum(); ++i) {
				if (getCollision(i)->isActorType(ACTOR_TYPE_PLAYER | 1)) {
					static JGeometry::TVec3<f32> up(0.0f, 1.0f, 0.0f);
					SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
					SMS_SendMessageToMario(this, HIT_MESSAGE_THROWN);
					SMS_ThrowMario(up, 100.0f);
					vanish();
				}
			}
		}
	}

	if (cue & 2) {
		J3DModel* model = mMActor->getModel();
		MtxPtr mtx      = model->getBaseTRMtx();
		MTXIdentity(mtx);
		mtx[0][3] = mPosition.x;
		mtx[1][3] = mPosition.y;
		mtx[2][3] = mPosition.z;
		model->setBaseScale(getScaling());
	}

	if (cue & 2) {
		MtxPtr mtx = mMActor->getModel()->getBaseTRMtx();

		JPABaseEmitter* rock = gpMarioParticleManager->emitAndBindToMtxPtr(
		    BOSSPAKKUN_JPA_MS_BOPA_TR_ROCK, mtx, 1, this);
		if (rock)
			rock->setGlobalScale(getScaling());

		JPABaseEmitter* smoke = gpMarioParticleManager->emitAndBindToMtxPtr(
		    BOSSPAKKUN_JPA_MS_BOPA_TR_SMOKE, mtx, 1, (const u8*)this + 1);
		if (smoke)
			smoke->setGlobalScale(getScaling());

		JPABaseEmitter* weed = gpMarioParticleManager->emitAndBindToMtxPtr(
		    BOSSPAKKUN_JPA_MS_BOPA_TR_WEED, mtx, 1, (const u8*)this + 2);
		if (weed)
			weed->setGlobalScale(mScaling);
	}

	if (cue & 2) {
		JGeometry::TVec3<f32> toMario = mPosition;
		toMario.x -= gpMarioPos->x;
		toMario.y -= gpMarioPos->y;
		toMario.z -= gpMarioPos->z;
		f32 dist = toMario.length();
		if (gpMSound->gateCheck(MSD_SE_BS_BSPAKU_TORNADO))
			MSoundSESystem::MSoundSE::startSoundActorWithInfo(
			    MSD_SE_BS_BSPAKU_TORNADO, &mPosition, nullptr, dist, 0, 0,
			    nullptr, 0, 4);
	}

	mMActor->perform(cue, graphics);
}

// UNUSED, 0xc0 in the map: the Tornado nerve inlines it.
void TBPTornado::launch(const JGeometry::TVec3<f32>& target)
{
	mState    = BOSSPAKU_TORNADO_MOVING;
	mTarget   = target;
	mPosition = mOwner->getPosition();
	mCenter   = mOwner->mPosition;
	mMove     = mOwner->getSaveParam2()->mSLTornadoMoveInit.get();
	offHitFlag(HIT_FLAG_NO_COLLISION);

	J3DFrameCtrl* ctrl = mMActor->getFrameCtrl(ANM_TYPE_BRK);
	ctrl->setFrame(0.0f);
	ctrl->setRate(0.0f);
}

// UNUSED, 0x94 in the map: TBossPakkun::init spells it out.
TBPHeadHit::TBPHeadHit(TBossPakkun* owner, const char* name)
    : THitActor(name)
    , mOwner(owner)
{
	initHitActor(0x8000010, 5, 0x81000000, 300.0f, 500.0f, 300.0f, 500.0f);
	offHitFlag(HIT_FLAG_NO_COLLISION);
}

BOOL TBPHeadHit::receiveMessage(THitActor* sender, u32 message)
{
	if (mOwner->getLatestNerve() == &TNerveBPSleep::theNerve())
		return mOwner->receiveMessage(sender, message);

	TBossPakkun* boss = mOwner;
	int state         = boss->mState;

	if (state == BOSSPAKU_STATE_FLYING) {
		// Sprayed while flying -- either by the nozzle's own hit actor or by
		// a water-gun droplet: drop out of the sky.
		if (sender->getActorType() == 0x1000000D
		    || sender->getActorType() == 0x1000001) {
			boss->mState = BOSSPAKU_STATE_NORMAL;
			boss->mSpine->reset();
			boss->mSpine->setNext(&TNerveBPFall::theNerve());
			if (gpMSound->gateCheck(MSD_SE_BS_BSPAKU_FALL))
				MSoundSESystem::MSoundSE::startSoundActor(
				    MSD_SE_BS_BSPAKU_FALL, &boss->mPosition, 0, nullptr, 0, 4);
			return TRUE;
		}
	}

	if (state != BOSSPAKU_STATE_UNK2) {
		if (boss->is2ndFightNow()) {
			if (&TNerveBPFly::theNerve() == boss->getLatestNerve())
				boss->showMessage(2);
		}

		if (sender->getActorType() == 0x1000001)
			return TRUE;

		return FALSE;
	}

	if (sender->getActorType() != 0x1000001)
		return TRUE;
	if (message != HIT_MESSAGE_SPRAYED_BY_WATER)
		return TRUE;

	JGeometry::TVec3<f32> toMario = *gpMarioPos;
	toMario.x -= mPosition.x;
	toMario.y -= mPosition.y;
	toMario.z -= mPosition.z;

	// The wrapped value is overwritten before it is ever read: the ROM
	// computes the head-to-Mario yaw twice here.
	f32 angle = MsAngleWrap(MsGetRotFromZaxisY(toMario));
	angle     = MsGetRotFromZaxisY(toMario);
	f32 diff  = MsAngleDiff(angle, mOwner->mRotation.y);

	if (fabsf(diff) < 0.5f * mOwner->getSaveParam2()->mSLDamageAngle.get()) {
		boss = mOwner;
		if (!boss->unk17C) {
			boss->unk170 += 1;
			if (boss->mWaterMark
			    < boss->getSaveParam2()->mSLWaterMarkLimit.get())
				boss->mWaterMark += 1;
			boss->unk174 = boss->getSaveParam2()->mSLWaterHitTimer.get();

			if (&TNerveBPSwallow::theNerve() != boss->getLatestNerve()) {
				boss->mSpine->reset();
				boss->mSpine->setNext(&TNerveBPSwallow::theNerve());
			}
		}
	}

	return TRUE;
}

void TBPHeadHit::throwActor(THitActor* actor)
{
	if (actor->getActorType() == 0x80000001
	    && mOwner->getMActor()->checkCurBckFromIndex(BOSSPAKU_BCK_HEAD)) {
		static JGeometry::TVec3<f32> up(0.0f, 1.0f, 0.0f);

		JGeometry::TVec3<f32> toActor = mOwner->mPosition;
		toActor.x -= actor->mPosition.x;
		toActor.y -= actor->mPosition.y;
		toActor.z -= actor->mPosition.z;
		if (toActor.isZero())
			toActor.set(0.0f, 0.0f, 1.0f);
		else
			VECNormalize(toActor, toActor);

		JGeometry::TVec3<f32> side;
		side.cross(up, toActor);
		if (side.isZero())
			side.set(1.0f, 0.0f, 0.0f);
		else
			VECNormalize(side, side);

		side.x *= 2.0f;
		side.y *= 2.0f;
		side.z *= 2.0f;
		side.x += up.x;
		side.y += up.y;
		side.z += up.z;

		SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
		SMS_SendMessageToMario(this, HIT_MESSAGE_THROWN);
		SMS_ThrowMario(side, 100.0f);
	}
}

// Binding level worth +8 of low region, landing TBPHeadHit::perform's frame
// at 0x38 (batch 121).
static inline u16 BosspakkunGetColNum(TBPHeadHit* p)
{
	u16 colNum = p->getColNum();
	return colNum;
}

void TBPHeadHit::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if ((cue & 1) && mOwner->mState != BOSSPAKU_STATE_BELLY_UP) {
		for (int i = 0; i < BosspakkunGetColNum(this); ++i) {
			THitActor* other = getCollision(i);
			if (other->isActorType(ACTOR_TYPE_PLAYER | 1))
				throwActor(other);
		}
	}

	if (cue & 2)
		mOwner->getJointTransByIndex(1, &mPosition);

	THitActor::perform(cue, graphics);
}

// UNUSED, 0x94 in the map: TBossPakkun::init spells it out.
TBPNavel::TBPNavel(TBossPakkun* owner, const char* name)
    : THitActor(name)
    , mOwner(owner)
{
	initHitActor(0x8000011, 1, 0x80000000, 200.0f, 300.0f, 200.0f, 300.0f);
	offHitFlag(HIT_FLAG_NO_COLLISION);
}

// Binding level over the navel's owner pointer. One site on the
// getLatestNerve() sleep test is +0x10, which closes receiveMessage.
static inline TBossPakkun* BosspakkunNavelOwner(const TBPNavel* p)
{
	TBossPakkun* owner = p->mOwner;
	return owner;
}

BOOL TBPNavel::receiveMessage(THitActor* sender, u32 message)
{
	if (BosspakkunNavelOwner(this)->getLatestNerve()
	    == &TNerveBPSleep::theNerve())
		return mOwner->receiveMessage(sender, message);

	u32 type = sender->getActorType();
	if (type == 0x1000001)
		return FALSE;

	if (mOwner->mState != BOSSPAKU_STATE_BELLY_UP)
		return TRUE;

	if (type == 0x80000001) {
		if (message == HIT_MESSAGE_HIP_DROP)
			mOwner->gotHipDropDamage();
		// gotTrampleDamage is UNUSED (map size 0x4) and empty; the call
		// inlines to the lone `cmplwi r31, 0` retail keeps after the hip-drop
		// arm. An empty else-if drops that compare.
		else if (message == HIT_MESSAGE_TRAMPLE)
			mOwner->gotTrampleDamage();
	}

	return TRUE;
}

void TBPNavel::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & 2)
		mOwner->getJointTransByIndex(6, &mPosition);

	THitActor::perform(cue, graphics);
}

// UNUSED, 0x90 in the map: TBossPakkun::init spells it out.
TBossPakkunMtxCalc::TBossPakkunMtxCalc(TBossPakkun* owner)
    : M3UMtxCalcSIAnmBlendQuat(false)
    , mOwner(owner)
{
}

void TBossPakkunMtxCalc::calcBellyScale(u16 joint)
{
	if (joint != 4 && joint != 0x24)
		return;

	f32 swell;
	if (mOwner->unk17C) {
		swell = (f32)mOwner->unk1B8 / 50.0f;
	} else {
		int limit = mOwner->getSaveParam2()->mSLWaterMarkLimit.get();
		int mark  = mOwner->mWaterMark;
		if (mark > limit)
			mark = limit;
		swell = (f32)mark / (f32)limit;
	}

	f32 rate = JMAHermiteInterpolation(swell, 0.0f, 0.0f, 10.0f, 1.0f, 1.0f,
	                                   0.0f);

	MtxPtr jointMtx = mOwner->getModel()->getAnmMtx(joint);

	Mtx scaleMtx;
	if (joint == 0x24) {
		static JGeometry::TVec3<f32> goal(1.4f, 1.4f, 1.6f);
		static JGeometry::TVec3<f32> start(1.0f, 0.8f, 0.8f);
		MTXScale(scaleMtx, start.x + rate * (goal.x - start.x),
		         start.y + rate * (goal.y - start.y),
		         start.z + rate * (goal.z - start.z));
	} else {
		static JGeometry::TVec3<f32> goal(1.3f, 1.7f, 1.7f);
		static JGeometry::TVec3<f32> start(1.0f, 0.9f, 0.9f);
		MTXScale(scaleMtx, start.x + rate * (goal.x - start.x),
		         start.y + rate * (goal.y - start.y),
		         start.z + rate * (goal.z - start.z));
	}

	MTXConcat(jointMtx, scaleMtx, jointMtx);
	MTXCopy(jointMtx, J3DSys::mCurrentMtx);
}

void TBossPakkunMtxCalc::calcHeadDir(u16 joint)
{
	if (joint != 0x12)
		return;

	MtxPtr jointMtx = mOwner->getModel()->getAnmMtx(joint);

	JGeometry::TVec3<f32> toMario = *gpMarioPos;
	toMario.x -= jointMtx[0][3];
	toMario.y -= jointMtx[1][3];
	toMario.z -= jointMtx[2][3];

	f32 yaw = mOwner->mHeadYaw;

	JGeometry::TVec3<f32> headAxis(jointMtx[0][1], jointMtx[1][1],
	                               jointMtx[2][1]);
	f32 anmYaw = MsGetRotFromZaxisY(headAxis);

	f32 goal;
	if (mOwner->getMActor()->checkCurBckFromIndex(BOSSPAKU_BCK_WAIT))
		goal = MsWrap(yaw + MsGetRotFromZaxisY(toMario), 0.0f, 360.0f);
	else
		goal = anmYaw;

	f32 diff  = MsAngleDiff(goal, anmYaw);
	f32 limit = mOwner->getSaveParam2()->mSLHeadHomingLimit.get();

	f32 turn;
	if (diff > 0.0f) {
		if (diff > limit)
			diff = limit;
		turn = diff;
	} else {
		diff = diff > -limit ? diff : -limit;
		turn = diff;
	}

	f32 step = MsAngleDiff(turn, yaw);
	if (step > 0.0f)
		step = std::min(step, 1.0f);
	else
		step = std::max(step, -1.0f);

	yaw += step;
	mOwner->mHeadYaw = yaw;

	Mtx rot;
	MsMtxSetRotX(rot, yaw);
	MTXConcat(jointMtx, rot, jointMtx);
	MTXConcat(J3DSys::mCurrentMtx, rot, J3DSys::mCurrentMtx);
}

// UNUSED, 0x50 in the map: TBossPakkun::changeBck spells it out.
void TBossPakkunMtxCalc::joinAnm(int index)
{
	M3UMtxCalcSIAnmBlendQuat::joinAnm(
	    mOwner->getActorKeeper()->getMActorAnmData()->getUnk2C()->getAnmPtr(
	        index));
}

// UNUSED, 0x48 in the map.
void TBossPakkunMtxCalc::setAnm(int index)
{
	M3UMtxCalcSIAnmBlendQuat::setAnm(
	    mOwner->getActorKeeper()->getMActorAnmData()->getUnk2C()->getAnmPtr(
	        index));
}

void TBossPakkunMtxCalc::calc(u16 joint)
{
	M3UMtxCalcSIAnmBlendQuat::calc(joint);
	calcBellyScale(joint);
	calcHeadDir(joint);
}

TBossPakkun::TBossPakkun(const char* name)
    : TSpineEnemy(name)
    , mMotionBlendStep(0.0f)
    , mPolDrop(nullptr)
    , mVomit(nullptr)
    , mTornado(nullptr)
    , mHeadHit(nullptr)
    , mNavel(nullptr)
    , mState(BOSSPAKU_STATE_NORMAL)
    , unk170(0)
    , unk174(0)
    , mWaterMark(0)
    , unk17C(0)
    , mEndMActor(nullptr)
    , mHeadYaw(0.0f)
    , mVomitArea(nullptr)
    , mWaterEmitInfo(nullptr)
    , mIsMarioRiding(0)
    , unk1B8(0)
    , unk1BC(0)
    , mBalloonsShown(0)
    , unk1C4(1)
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

	if (!((TBossPakkunManager*)mManager)->mIsLightVersion) {
		mEndMActor = mMActorKeeper->createMActor("bosspaku_end.bmd", 0);
		mEndMActor->setBckFromIndex(BOSSPAKU_BCK_DOWN_START);
		mEndMActor->setBrkFromIndex(0);
	}

	TIdxGroupObj* group
	    = JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ");

	initHitActor(0x800000F, 1, 0x80000000, 80.0f, 300.0f, 80.0f, 300.0f);
	offHitFlag(HIT_FLAG_NO_COLLISION);

	if (!((TBossPakkunManager*)mManager)->mIsLightVersion) {
		mHeadHit = new TBPHeadHit(this, "ボスパックン頭部");
		mNavel   = new TBPNavel(this, "ボスパックンへそ");
		group->getChildren().push_back(mHeadHit);
		group->getChildren().push_back(mNavel);

		mMapCollisionManager
		    = new TMapCollisionManager(1, "/scene/bosspakkun", this);
		mMapCollisionManager->init("col_body.col", 1, nullptr);
		mMapCollisionManager->setUpUnk8TRS(mPosition, mRotation, mScaling);

		mMtxCalc = new TBossPakkunMtxCalc(this);
		getMActor()->setCalcForBck(mMtxCalc);
		getMActor()->calc();
	}

	if (((TBossPakkunManager*)mManager)->mIsLightVersion) {
		mSpine->initWith(&TNerveBPWaitL::theNerve());
	} else if (gpMarDirector->mMap == 0x37) {
		mSpine->initWith(&TNerveBPFall::theNerve());
	} else if (gpMarDirector->unk7D == 4) {
		mSpine->initWith(&TNerveBPSleep::theNerve());
	} else {
		mSpine->initWith(&TNerveBPWait::theNerve());
	}

	mPolDrop      = new TBPPolDrop(this, "<TBPPolDrop>");
	MActor* stamp = mMActorKeeper->createMActor("pollut_ball_stamp.bmd", 0);
	MActor* ball  = mMActorKeeper->createMActor("pollut_ball.bmd", 0);

	TBPPolDrop* drop  = mPolDrop;
	drop->mBallMActor = ball;
	// The stamp model is the one the pollution map takes its imprint from.
	drop->mStampMActor = stamp;

	ResTIMG* rak = (ResTIMG*)JKRFileLoader::getGlbResource(
	    "/scene/map/pollution/H_ma_rak.bti");
	if (rak)
		SMS_ChangeTextureAll(mPolDrop->mBallMActor->getModel()->getModelData(),
		                     "M_dummy", *rak);

	if (!((TBossPakkunManager*)mManager)->mIsLightVersion) {
		mVomit = new TBPVomit(this, "<TBPVomit>");
		MActor* white
		    = mMActorKeeper->createMActor("bosspakuPollut_white.bmd", 0);
		MActor* pollut = mMActorKeeper->createMActor("bosspakuPollut.bmd", 0);

		TBPVomit* vomit     = mVomit;
		vomit->mMActor      = pollut;
		vomit->mStampMActor = white;

		mTornado = new TBPTornado(this, "<TBPTornado>");
		group->getChildren().push_back(mTornado);

		mWaterEmitInfo = new TWaterEmitInfo("/enemy/bosspakuwater.prm");
	}

	initAnmSound();
	onLiveFlag(LIVE_FLAG_UNK400);
	mScaledBodyRadius = 400.0f;

	unk124->setGraph(gpConductor->getGraphByName("bosspakkun"));
	if (unk124->getGraph()) {
		unk124->reset();
		goToShortestNextGraphNode();
	}

	mHitPoints = getMaxHitPoints();
}

BOOL TBossPakkun::checkMarioRiding()
{
	const TBGCheckData* plane = SMS_GetMarioGrPlane();

	if (!mIsMarioRiding) {
		if (plane && plane->getActor() == this
		    && SMS_IsMarioTouchGround4cm()) {
			u32 status = SMS_GetMarioStatus();
			if ((status & MARIO_STATUS_FLAG_UNK200)
			    && !(status & MARIO_STATUS_FLAG_UNK200000)) {
				mIsMarioRiding = 1;
				return TRUE;
			}
		}
	} else if (!(plane && plane->getActor() == this
	             && SMS_IsMarioTouchGround4cm())) {
		mIsMarioRiding = 0;
	}

	return FALSE;
}

// UNUSED, 0x48 in the map: the Fly nerve inlines it. The flag makes sure the
// boss battle theme only starts once.
void TBossPakkun::startBGM()
{
	if (!unk1CC) {
		MSBgm::startBGM(0x8001000D);
		unk1CC = 1;
	}
}

void TBossPakkun::rumblePad(int kind, const JGeometry::TVec3<f32>& from)
{
	if (!SMS_IsMarioTouchGround4cm())
		return;

	JGeometry::TVec3<f32> toMario = *gpMarioPos;
	toMario.x -= from.x;
	toMario.y -= from.y;
	toMario.z -= from.z;

	f32 length = toMario.length();
	f32 power  = (3000.0f - length) / 1000.0f;
	if (power < 0.0f)
		return;
	if (power > 1.0f)
		power = 1.0f;

	switch (kind) {
	case 0:
		power *= 0.4f;
		break;
	case 1:
		power *= 0.7f;
		break;
	case 2:
		break;
	}

	mRumblePower = power;
	SMSRumbleMgr->start(8, &mRumblePower);
}

static inline TModelWaterManager* BosspakkunWaterManager()
{
	TModelWaterManager* manager = gpModelWaterManager;
	return manager;
}

static inline TWaterEmitInfo* BosspakkunWaterEmitInfoRaw(TBossPakkun* p)
{
	return p->mWaterEmitInfo;
}

static inline TWaterEmitInfo* BosspakkunWaterEmitInfo(TBossPakkun* p)
{
	TWaterEmitInfo* info = BosspakkunWaterEmitInfoRaw(p);
	return info;
}

static inline MSound* BosspakkunSound()
{
	MSound* sound = gpMSound;
	return sound;
}

// Binding level over a raw member read, worth +16 of low region in
// TBossPakkun::showMessage (batch 127).
static inline TMarDirector* BosspakkunGetMarDirector()
{
	TMarDirector* marDirector = gpMarDirector;
	return marDirector;
}

// Binding level over TBossPakkun::getSaveParam2(): worth +8 of low region per
// expansion where the plain accessor call is +0.
static inline TBossPakkunParams* BosspakkunParams(const TBossPakkun* p)
{
	TBossPakkunParams* params = p->getSaveParam2();
	return params;
}

void TBossPakkun::showMessage(u32 message)
{
	u32 mask;
	if (message == 1)
		mask = 0;
	else
		mask = 1 << message;

	if (!(mBalloonsShown & mask))
		BosspakkunGetMarDirector()->getConsole()->startAppearBalloon(message, true);

	mBalloonsShown |= mask;
}

bool TBossPakkun::is2ndFightNow() const
{
	if (gpMarDirector->mMap == 2 && gpMarDirector->unk7D == 4)
		return true;
	return false;
}

// UNUSED, 0xec in the map, and genuinely dead: no call site of this shape is
// left anywhere in the unit once every function is reconstructed.
// TODO: body unknown.
void TBossPakkun::ignoreWaterCheck() { }

// UNUSED, 0x58 in the map: the Tornado nerve inlines it. unk194 and unk1A0 are
// the two hand joints perform() samples while the tornado animation plays.
void TBossPakkun::startTornadoBlur()
{
	gpMarioParticleManager->emitAndBindToPosPtr(BOSSPAKKUN_JPA_MS_BOPA_BLUR1,
	                                            &unk194, 0, nullptr);
	gpMarioParticleManager->emitAndBindToPosPtr(BOSSPAKKUN_JPA_MS_BOPA_BLUR1,
	                                            &unk1A0, 0, nullptr);
}

// UNUSED, 0xa4 in the map: the StompReact, TumbleOut and PreDie nerves all
// spell it out. Spits the swallowed water back out and starts the belly
// deflating.
void TBossPakkun::resetWaterMark()
{
	unk17C = 1;
	unk174 = 0;
	unk170 = 0;
	unk1B8 = 50;

	if (mWaterEmitInfo) {
		JGeometry::TVec3<f32> mouth;
		getJointTransByIndex(0x12, &mouth);
		mouth.y += 250.0f;
		BosspakkunWaterEmitInfo(this)->mPos.value = mouth;
		BosspakkunWaterManager()->emitRequest(*mWaterEmitInfo);
	}
}

// UNUSED, 0xa0 in the map: the Wait, WaitL and Hover nerves all inline it.
// The area manager is looked up lazily, because it is placed in the stage and
// so does not exist yet when the boss is created.
BOOL TBossPakkun::inArea(const JGeometry::TVec3<f32>& pos)
{
	if (!mVomitArea)
		mVomitArea = (TAreaCylinderManager*)gpConductor->search(
		    "ゲロエリアマネージャー");

	if (mVomitArea == nullptr)
		return FALSE;

	return mVomitArea->contain(pos);
}

// UNUSED, 0xd0 in the map, and genuinely dead: the flying damage reaction now
// lives in TBPHeadHit::receiveMessage's BOSSPAKU_STATE_FLYING arm, which is
// its own code rather than an expansion of this.
// TODO: body unknown.
void TBossPakkun::gotFlyingDamage() { }

// UNUSED, 0x160 in the map, and genuinely dead: the spray reaction now lives
// in TBPHeadHit::receiveMessage's BOSSPAKU_STATE_UNK2 arm.
// TODO: body unknown.
void TBossPakkun::gotWaterDamage() { }

void TBossPakkun::gotHipDropDamage()
{
	decHitPoints();
	mState = BOSSPAKU_STATE_NORMAL;

	if (getHitPoints() == 0) {
		const TNerveBase<TLiveActor>* nerve = &TNerveBPPreDie::theNerve();
		if (mSpine->getLatestNerve() == nerve)
			return;
		nerve = &TNerveBPDie::theNerve();
		if (mSpine->getLatestNerve() == nerve)
			return;

		mSpine->setNext(&TNerveBPPreDie::theNerve());

		if (BosspakkunSound()->gateCheck(MSD_SE_BS_BSPAKU_DOWN))
			MSoundSESystem::MSoundSE::startSoundActor(MSD_SE_BS_BSPAKU_DOWN, &mPosition, 0,
			                          nullptr, 0, 4);
		return;
	}

	const TNerveBase<TLiveActor>* tumbleOut = &TNerveBPTumbleOut::theNerve();
	if (mSpine->getLatestNerve() == tumbleOut)
		return;

	if (BosspakkunSound()->gateCheck(MSD_SE_BS_BSPAKU_DAMAGE))
		MSoundSESystem::MSoundSE::startSoundActor(MSD_SE_BS_BSPAKU_DAMAGE, &mPosition, 0,
		                          nullptr, 0, 4);

	if (BosspakkunGetMarDirector()->unk7D == 4) {
		// getSpine() at this one reset is the last +8; the sibling
		// else-arm reset stays on the raw member.
		getSpine()->reset();
		mSpine->setNext(&TNerveBPTakeOff::theNerve());
		const TNerveBase<TLiveActor>* getUp = &TNerveBPGetUp::theNerve();
		mSpine->pushNerve(getUp);
		const TNerveBase<TLiveActor>* stomp = &TNerveBPStompReact::theNerve();
		mSpine->pushNerve(stomp);
	} else {
		mSpine->reset();
		mSpine->setNext(&TNerveBPWait::theNerve());
		const TNerveBase<TLiveActor>* getUp = &TNerveBPGetUp::theNerve();
		mSpine->pushNerve(getUp);
		const TNerveBase<TLiveActor>* stomp = &TNerveBPStompReact::theNerve();
		mSpine->pushNerve(stomp);
	}
}

// UNUSED, 0x4 in the map: an empty function. Trampling the boss does nothing;
// only a hip drop on the navel takes a hit point off.
void TBossPakkun::gotTrampleDamage() { }

void TBossPakkun::launchPolDrop()
{
	if (mPolDrop->mState != BOSSPAKU_POLDROP_DEAD)
		return;

	JGeometry::TVec3<f32> from;
	if (checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)) {
		from = mPosition;
		from.x += 1.0f;
	} else {
		getJointTransByIndex(0x12, &from);
	}

	// Aim a little ahead of Mario, in the direction he is facing.
	f32 marioYaw = gpMarioOriginal->mRotation.y;
	f32 reach    = getSaveParam2()->mSLPollBallFront.get();

	JGeometry::TVec3<f32> front;
	front.set(reach * MsSin(marioYaw), 0.0f, reach * MsCos(marioYaw));

	JGeometry::TVec3<f32> goal = front;
	goal.x += gpMarioPos->x;
	goal.y += gpMarioPos->y;
	goal.z += gpMarioPos->z;

	JGeometry::TVec3<f32> velocity;
	SMSCalcJumpVelocityXZ(goal, from, getSaveParam2()->mSLPollBallSpeed.get(),
	                      0.1f, &velocity);

	mPolDrop->launch(from, velocity);
}

// UNUSED, 0xc4 in the map: the Tornado nerve inlines it.
void TBossPakkun::launchTornado() { mTornado->launch(*gpMarioPos); }

// UNUSED, 0x6c in the map: the PreDie nerve spells it out. The slugs the boss
// spat out go with it.
void TBossPakkun::killSmallEnemies()
{
	TEnemyManager* manager
	    = JDrama::TNameRefGen::search<TEnemyManager>("ナメクリマネージャー");
	if (manager)
		manager->killChildren();
}

void TBossPakkun::changeBck(int index)
{
	if (getMActor()->checkCurBckFromIndex(index)
	    && !getMActor()->curAnmEndsNext(ANM_TYPE_BCK, nullptr))
		return;

	int prev = getMActor()->getCurAnmIdx(ANM_TYPE_BCK);

	mMtxCalc->joinAnm(index);
	getMActor()->setFrameCtrlForBck(index);

	if (index == BOSSPAKU_BCK_POLLUT_START)
		getMActor()
		    ->getFrameCtrl(ANM_TYPE_BCK)
		    ->setRate(getSaveParam2()->mSLVomitAnmRate.value);

	f32 blendFrames = -1.0f;
	if (prev == BOSSPAKU_BCK_WAIT) {
		if (index == BOSSPAKU_BCK_POLLUT_START
		    || index == BOSSPAKU_BCK_WATER_HIT)
			blendFrames = (f32)getSaveParam2()->mSLAnmBlendTime0.get();
	} else if (prev == BOSSPAKU_BCK_LAND) {
		if (index == BOSSPAKU_BCK_WAIT)
			blendFrames = (f32)getSaveParam2()->mSLAnmBlendTime0.get();
	} else if (prev == BOSSPAKU_BCK_BALL_END) {
		if (index == BOSSPAKU_BCK_WAIT)
			blendFrames = (f32)getSaveParam2()->mSLAnmBlendTime0.get();
	} else if (prev == BOSSPAKU_BCK_POLLUT_END) {
		if (index == BOSSPAKU_BCK_WAIT)
			blendFrames = (f32)getSaveParam2()->mSLAnmBlendTime0.get();
	} else if (prev == BOSSPAKU_BCK_WATER_HIT) {
		if (index == BOSSPAKU_BCK_RETURN)
			blendFrames = (f32)getSaveParam2()->mSLAnmBlendTime0.get();
	}

	if (blendFrames < 0.0f) {
		J3DFrameCtrl* ctrl = getMActor()->getFrameCtrl(ANM_TYPE_BCK);
		if (ctrl)
			blendFrames = 0.1f * (f32)ctrl->getEnd();
	}

	if (blendFrames == 0.0f)
		mMotionBlendStep = 1.0f;
	else
		mMotionBlendStep = 1.0f / blendFrames;

	const char** table = getBasNameTable();
	setAnmSound(table == nullptr ? nullptr : table[index]);
}

// UNUSED, 0x130 in the map: the Fly nerve inlines it.
void TBossPakkun::flyToCurPathNode(f32 speed, f32 turn_speed)
{
	turnToCurPathNode(turn_speed);

	JGeometry::TVec3<f32> step = getUnkF4().getPoint();
	step.x -= mPosition.x;
	step.y -= mPosition.y;
	step.z -= mPosition.z;
	VECNormalize(step, step);
	step.x *= speed;
	step.y *= speed;
	step.z *= speed;

	JGeometry::TVec3<f32> velocity = mLinearVelocity;
	velocity.x += step.x;
	velocity.y += step.y;
	velocity.z += step.z;
	mLinearVelocity = velocity;
}

const char** TBossPakkun::getBasNameTable() const { return bosspakkun_bastable; }

void TBossPakkun::setGroundCollision()
{
	// Retail compares `cmplw nerve, instance`. Naming the instance keeps
	// theNerve() from being CSE'd above its static-init guard, then the
	// `getLatestNerve() == named` compare emits retail's operand order.
	const TNerveBase<TLiveActor>* nerve = &TNerveBPDie::theNerve();
	if (mSpine->getLatestNerve() == nerve)
		return;
	nerve = &TNerveBPTumbleOut::theNerve();
	if (mSpine->getLatestNerve() == nerve)
		return;
	if (!mMapCollisionManager)
		return;

	JGeometry::SMatrix34C<f32> mtx;
	mtx.set(getModel()->getAnmMtx(2));
	if (mMapCollisionManager->unk8)
		mMapCollisionManager->getUnk8()->moveMtx(mtx);
}

void TBossPakkun::kill()
{
	TLiveActor::kill();

	onHitFlag(HIT_FLAG_NO_COLLISION);
	if (mHeadHit)
		mHeadHit->onHitFlag(HIT_FLAG_NO_COLLISION);
	if (mNavel)
		mNavel->onHitFlag(HIT_FLAG_NO_COLLISION);
	if (mPolDrop)
		mPolDrop->onHitFlag(HIT_FLAG_NO_COLLISION);
}

BOOL TBossPakkun::receiveMessage(THitActor* sender, u32 message)
{
	if (((TBossPakkunManager*)getManager())->mIsLightVersion)
		return FALSE;

	if (&TNerveBPSleep::theNerve() == mSpine->getLatestNerve()) {
		if (sender->getActorType() == 0x1000000D) {
			mSpine->reset();
			mSpine->setNext(&TNerveBPBreakSleep::theNerve());
			return TRUE;
		}
	}

	if (mState == BOSSPAKU_STATE_FLYING) {
		if (sender->getActorType() == 0x1000000D
		    || sender->getActorType() == 0x1000001) {
			if (mPosition.y - 300.0f > sender->mPosition.y)
				return TRUE;
			if (1500.0f + mPosition.y < sender->mPosition.y)
				return TRUE;

			mState = BOSSPAKU_STATE_NORMAL;
			mSpine->reset();
			mSpine->setNext(&TNerveBPFall::theNerve());

			if (gpMSound->gateCheck(MSD_SE_BS_BSPAKU_FALL))
				MSoundSESystem::MSoundSE::startSoundActor(MSD_SE_BS_BSPAKU_FALL, &mPosition, 0,
				                          nullptr, 0, 4);
			return TRUE;
		}
	}

	return FALSE;
}

void TBossPakkun::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (mPolDrop)
		mPolDrop->perform(cue, graphics);
	if (mVomit)
		mVomit->perform(cue, graphics);
	if (mTornado)
		mTornado->perform(cue, graphics);

	if (checkLiveFlag(LIVE_FLAG_DEAD))
		return;

	if (mHeadHit)
		mHeadHit->perform(cue, graphics);
	if (mNavel)
		mNavel->perform(cue, graphics);

	if (!((TBossPakkunManager*)mManager)->mIsLightVersion && (cue & 1)) {
		mMtxCalc->advanceMotionBlend(-mMotionBlendStep);

		if (unk17C) {
			if (mWaterMark > 0) {
				int drain = getSaveParam2()->mSLWaterMarkLimit.get() / 50;
				if (drain == 0)
					drain = 1;
				mWaterMark -= drain;
				if (mWaterMark < 0)
					mWaterMark = 0;
			} else {
				unk17C     = 0;
				mWaterMark = 0;
			}
		}

		if (!unk17C && unk174 > 0) {
			unk174 -= 1;
			unk170 += 1;
		}

		if (unk1BC) {
			if (unk1B8 > 0)
				unk1B8 -= 1;
			else {
				unk1BC = 0;
				unk1B8 = 0;
			}
		}

		if (mState == BOSSPAKU_STATE_BELLY_UP && checkMarioRiding()) {
			if (&TNerveBPJumpReact::theNerve() != mSpine->getLatestNerve())
				mSpine->pushNerve(&TNerveBPJumpReact::theNerve());
		}
	}

	if (!((TBossPakkunManager*)mManager)->mIsLightVersion && (cue & 2)) {
		if (getMActor()->checkCurBckFromIndex(BOSSPAKU_BCK_TORNADO)) {
			MtxPtr left = getMActor()->getModel()->getAnmMtx(43);
			unk194.set(left[0][3], left[1][3], left[2][3]);
			MtxPtr right = getMActor()->getModel()->getAnmMtx(44);
			unk1A0.set(right[0][3], right[1][3], right[2][3]);
		}

		if (getMActor()->checkCurBckFromIndex(BOSSPAKU_BCK_SLEEP)) {
			MtxPtr nose = getMActor()->getModel()->getAnmMtx(32);
			unk1AC.set(nose[0][3], nose[1][3], nose[2][3]);

			JPABaseEmitter* zzz = gpMarioParticleManager->emitAndBindToPosPtr(
			    PARTICLE_MS_POI_ZZZ, &unk1AC, 1, this);
			if (zzz) {
				static JGeometry::TVec3<f32> s(2.5f, 2.5f, 2.5f);
				zzz->setGlobalScale(s);
			}
		}

		if (getMActor()->checkCurBckFromIndex(BOSSPAKU_BCK_FLY)
		    || getMActor()->checkCurBckFromIndex(BOSSPAKU_BCK_FLY_START)
		    || getMActor()->checkCurBckFromIndex(BOSSPAKU_BCK_FLY_POLLUT)) {
			gpMarioParticleManager->emitAndBindToMtxPtr(
			    BOSSPAKKUN_JPA_MS_BOPA_BLUR2, getModel()->getAnmMtx(38), 1,
			    this);
			gpMarioParticleManager->emitAndBindToMtxPtr(
			    BOSSPAKKUN_JPA_MS_BOPA_BLUR2, getModel()->getAnmMtx(46), 1,
			    this + 1);
		}

		if (getMActor()->checkCurBckFromIndex(BOSSPAKU_BCK_FLY)
		    || getMActor()->checkCurBckFromIndex(BOSSPAKU_BCK_DOWN_LOOP)
		    || getMActor()->checkCurBckFromIndex(BOSSPAKU_BCK_WATER_HIT)) {
			gpMarioParticleManager->emitAndBindToMtxPtr(
			    BOSSPAKKUN_JPA_MS_BOPA_ASE, getModel()->getAnmMtx(32), 1,
			    this + 1);
		}
	}

	if (!((TBossPakkunManager*)mManager)->mIsLightVersion) {
		// The death animation lives on its own model, so the base class is
		// handed mEndMActor for one frame.
		if (&TNerveBPDie::theNerve() == mSpine->getLatestNerve()) {
			MActor* body = getMActor();
			mMActor      = mEndMActor;
			TSpineEnemy::perform(cue, graphics);
			mMActor = body;
			return;
		}
	}

	if (!((TBossPakkunManager*)mManager)->mIsLightVersion && (cue & 2)) {
		updateSquareToMario();
		getModel()->getModelData()->getJointNodePointer(0)->setMtxCalc(
		    mMtxCalc);
	}

	if (!((TBossPakkunManager*)mManager)->mIsLightVersion && (cue & 2)) {
		if (mState == BOSSPAKU_STATE_BELLY_UP) {
			JGeometry::TVec3<f32> target = mNavel->mPosition;
			target.y += 100.0f;
			gpTargetArrow->unk14 = 1;
			gpTargetArrow->setPos(target);
		} else {
			gpTargetArrow->unk14 = 0;
		}
	}

	if (!((TBossPakkunManager*)mManager)->mIsLightVersion && (cue & 0x200)) {
		if (&TNerveBPPreDie::theNerve() == mSpine->getLatestNerve()
		    || &TNerveBPStompReact::theNerve() == mSpine->getLatestNerve()) {
			getMActor()->offMakeDL();
			SMS_AddDamageFogEffect(getMActor()->getModel()->getModelData(),
			                       mPosition, graphics);
		} else {
			SMS_ResetDamageFogEffect(getMActor()->getModel()->getModelData());
		}
	}

	TSpineEnemy::perform(cue, graphics);
}

TBossPakkunManager::TBossPakkunManager(const char* name, int is_light_version)
    : TEnemyManager(name)
    , mIsLightVersion(is_light_version)
{
}

// UNUSED, 0x280 in the map: TBossPakkunManager::load loads the twelve .jpa
// files inline.
void TBossPakkunManager::initJParticle()
{
	SMS_LoadParticle("/scene/bosspakkun/jpa/ms_bopa_blur1.jpa",
	                 BOSSPAKKUN_JPA_MS_BOPA_BLUR1);
	SMS_LoadParticle("/scene/bosspakkun/jpa/ms_bopa_down.jpa",
	                 BOSSPAKKUN_JPA_MS_BOPA_DOWN);
	SMS_LoadParticle("/scene/bosspakkun/jpa/ms_bopa_swing1.jpa",
	                 BOSSPAKKUN_JPA_MS_BOPA_SWING1);
	SMS_LoadParticle("/scene/bosspakkun/jpa/ms_bopa_swing2.jpa",
	                 BOSSPAKKUN_JPA_MS_BOPA_SWING2);
	SMS_LoadParticle("/scene/bosspakkun/jpa/ms_bopa_wathit.jpa",
	                 BOSSPAKKUN_JPA_MS_BOPA_WATHIT);
	SMS_LoadParticle("/scene/bosspakkun/jpa/ms_bopa_wathit_w.jpa",
	                 BOSSPAKKUN_JPA_MS_BOPA_WATHIT_W);
	SMS_LoadParticle("/scene/bosspakkun/jpa/ms_bopa_ase.jpa",
	                 BOSSPAKKUN_JPA_MS_BOPA_ASE);
	SMS_LoadParticle("/scene/bosspakkun/jpa/ms_bopa_blur2.jpa",
	                 BOSSPAKKUN_JPA_MS_BOPA_BLUR2);
	SMS_LoadParticle("/scene/bosspakkun/jpa/ms_bopa_jita.jpa",
	                 BOSSPAKKUN_JPA_MS_BOPA_JITA);
	SMS_LoadParticle("/scene/bosspakkun/jpa/ms_bopa_tr_rock.jpa",
	                 BOSSPAKKUN_JPA_MS_BOPA_TR_ROCK);
	SMS_LoadParticle("/scene/bosspakkun/jpa/ms_bopa_tr_smoke.jpa",
	                 BOSSPAKKUN_JPA_MS_BOPA_TR_SMOKE);
	SMS_LoadParticle("/scene/bosspakkun/jpa/ms_bopa_tr_weed.jpa",
	                 BOSSPAKKUN_JPA_MS_BOPA_TR_WEED);
}

void TBossPakkunManager::createModelData()
{
	if (mIsLightVersion) {
		static const TModelDataLoadEntry entry[] = {
			{ "bosspaku_model.bmd", 0x10010000 },
			{ "pollut_ball.bmd", 0x11040000 },
			{ "pollut_ball_stamp.bmd", 0x10010000 },
			{ nullptr, 0 },
		};
		createModelDataArray(entry);
	} else {
		static const TModelDataLoadEntry entry[] = {
			{ "bosspaku_model.bmd", 0x10010000 },
			{ "bosspaku_end.bmd", 0x10100000 },
			{ "pollut_ball.bmd", 0x11040000 },
			{ "pollut_ball_stamp.bmd", 0x10010000 },
			{ "bosspakuPollut.bmd", 0x11020000 },
			{ "bosspakuPollut_white.bmd", 0x10010000 },
			{ "trunade.bmd", 0x10020000 },
			{ nullptr, 0 },
		};
		createModelDataArray(entry);
	}
}

void TBossPakkunManager::load(JSUMemoryInputStream& stream)
{
	unk38 = new TBossPakkunParams("/enemy/bosspakkun.prm");
	TEnemyManager::load(stream);
	if (!mIsLightVersion)
		initJParticle();
}

DEFINE_NERVE(TNerveBPWait, TLiveActor)
{
	TBossPakkun* boss = (TBossPakkun*)spine->getBody();

	JGeometry::TVec3<f32> toMario = boss->getPosition();
	toMario.x -= gpMarioPos->x;
	toMario.y -= gpMarioPos->y;
	toMario.z -= gpMarioPos->z;

	f32 reach = boss->getSaveParam2()->mSLSwingLength.get();
	reach *= reach;
	if (toMario.squared() < reach) {
		// Mario is within head-swinging range. If the boss is already facing
		// him it swings; otherwise it spits and turns towards him.
		JGeometry::TVec3<f32> toMarioBack(-toMario.x, -toMario.y, -toMario.z);
		JGeometry::TVec3<f32> facing = toMarioBack;

		f32 angle = MsGetRotFromZaxisY(facing);
		if (fabsf(MsAngleDiff(angle, boss->mRotation.y)) < 60.0f) {
			spine->pushAfterCurrent(&TNerveBPWait::theNerve());
			spine->pushAfterCurrent(&TNerveBPSwing::theNerve());
			return TRUE;
		}

		spine->pushAfterCurrent(&TNerveBPWait::theNerve());
		spine->pushAfterCurrent(&TNerveBPVomit::theNerve());

		TPathNode node(*gpMarioPos);
		node.unk4.y = 0.0f;
		boss->unk114.push(boss->unkF4);
		boss->unkF4 = node;

		spine->pushAfterCurrent(&TNerveBPPivot::theNerve());
		return TRUE;
	}

	if (spine->getTime() == 0)
		boss->changeBck(BOSSPAKU_BCK_WAIT);

	if (spine->getTime() >= boss->getSaveParam2()->mSLWaitFrameStg0.get()
	    && boss->getMActor()->isCurAnmAlreadyEnd(ANM_TYPE_BCK)) {
		if (SMSGetMarDirector()->mMap == 2
		    && (SMSGetMarDirector()->unk7D == 0
		        || SMSGetMarDirector()->unk7D == 1)) {
			if (boss->inArea(*gpMarioPos)) {
				if (!SMS_GetMarioGroundPlane()->isWaterSurface()) {
					spine->pushAfterCurrent(&TNerveBPCannon::theNerve());
					return TRUE;
				}
			}

			spine->pushAfterCurrent(&TNerveBPWait::theNerve());
			return TRUE;
		}

		if (gpMarDirector->unk7D == 4) {
			f32 prop = boss->getSaveParam2()->mSLTornadoProp.get();
			if (boss->mTornado->mState != BOSSPAKU_TORNADO_DEAD
			    || MsRandF() < prop) {
				spine->pushAfterCurrent(&TNerveBPTakeOff::theNerve());
				spine->pushAfterCurrent(&TNerveBPVomit::theNerve());
			} else if (boss->mTornado->mState == BOSSPAKU_TORNADO_DEAD) {
				spine->pushAfterCurrent(&TNerveBPWait::theNerve());
				spine->pushAfterCurrent(&TNerveBPTornado::theNerve());
			} else {
				spine->pushAfterCurrent(&TNerveBPWait::theNerve());
			}
			return TRUE;
		}

		spine->pushAfterCurrent(&TNerveBPWait::theNerve());
		spine->pushAfterCurrent(&TNerveBPVomit::theNerve());

		JGeometry::TVec3<f32> goal = boss->mPosition;
		goal.x += 10000.0f * (MsRandF() - 0.5f);
		goal.z += 10000.0f * (MsRandF() - 0.5f);

		TPathNode node(goal);
		boss->unk114.push(boss->unkF4);
		boss->unkF4 = node;

		spine->pushAfterCurrent(&TNerveBPPivot::theNerve());
		return TRUE;
	}

	return FALSE;
}

DEFINE_NERVE(TNerveBPCannon, TLiveActor)
{
	TBossPakkun* boss = (TBossPakkun*)spine->getBody();
	MActor* actor     = boss->getMActor();

	if (spine->getTime() == 0)
		boss->changeBck(BOSSPAKU_BCK_POLLUT_START);

	if (actor->curAnmEndsNext(ANM_TYPE_BCK, nullptr)) {
		if (actor->checkCurBckFromIndex(BOSSPAKU_BCK_POLLUT_START)) {
			boss->changeBck(BOSSPAKU_BCK_BALL_END);
			boss->launchPolDrop();
		} else {
			spine->pushAfterCurrent(&TNerveBPWait::theNerve());
			return TRUE;
		}
	}

	return FALSE;
}

DEFINE_NERVE(TNerveBPVomit, TLiveActor)
{
	TBossPakkun* boss = (TBossPakkun*)spine->getBody();
	MActor* actor     = boss->getMActor();

	if (spine->getTime() == 0)
		boss->changeBck(BOSSPAKU_BCK_POLLUT_START);

	// The mouth is only open -- and only sprayable -- for the middle of the
	// wind-up animation.
	if (actor->checkCurBckFromIndex(BOSSPAKU_BCK_POLLUT_START)) {
		f32 frame = actor->getFrameCtrl(ANM_TYPE_BCK)->getFrame();
		if (25.0f < frame && frame < 165.0f)
			boss->mState = BOSSPAKU_STATE_UNK2;
		else
			boss->mState = BOSSPAKU_STATE_NORMAL;
	}

	if (actor->checkCurBckFromIndex(BOSSPAKU_BCK_POLLUT_END)) {
		if (MsRandF() < 0.2f && spine->getTime() == 500) {
			JGeometry::TVec3<f32> dir;
			dir.set(700.0f * MsSin(boss->mRotation.y), 0.0f,
			        700.0f * MsCos(boss->mRotation.y));
			JGeometry::TVec3<f32> front = dir;
			gpItemManager->makeObjAppear(boss->mPosition.x + front.x,
			                             1.0f + boss->mPosition.y,
			                             boss->mPosition.z + front.z,
			                             0x20000002, false);
		}
	}

	if (actor->curAnmEndsNext(ANM_TYPE_BCK, nullptr)) {
		if (actor->checkCurBckFromIndex(BOSSPAKU_BCK_POLLUT_START)) {
			boss->mState = BOSSPAKU_STATE_NORMAL;
			boss->changeBck(BOSSPAKU_BCK_POLLUT_END);

			boss->mVomit->vomit();
			boss->rumblePad(1, boss->mPosition);
		} else {
			// showMessage(0) spelled out: the ROM folds the mask for the
			// constant balloon id into this site.
			if (!boss->is2ndFightNow()) {
				if (!(boss->mBalloonsShown & 1))
					gpMarDirector->getConsole()->startAppearBalloon(0, true);
				boss->mBalloonsShown |= 1;
			}
			return TRUE;
		}
	}

	if (actor->checkCurBckFromIndex(BOSSPAKU_BCK_POLLUT_END)) {
		JGeometry::TVec3<f32> wind(MsSin(boss->mRotation.y), 0.0f,
		                           MsCos(boss->mRotation.y));
		gpModelWaterManager->wind(wind);
	}

	return FALSE;
}

DEFINE_NERVE(TNerveBPTornado, TLiveActor)
{
	TBossPakkun* boss = (TBossPakkun*)spine->getBody();
	MActor* actor     = boss->getMActor();

	if (spine->getTime() == 0) {
		boss->changeBck(BOSSPAKU_BCK_TORNADO);
		gpMarioParticleManager->emitAndBindToSRTMtxPtr(
		    BOSSPAKKUN_JPA_MS_BOPA_SWING1, boss->getModel()->getAnmMtx(3), 0,
		    boss);
		boss->startTornadoBlur();
	}

	if (spine->getTime() == 150)
		boss->launchTornado();

	if (actor->isCurAnmAlreadyEnd(ANM_TYPE_BCK))
		return TRUE;

	return FALSE;
}

DEFINE_NERVE(TNerveBPPivot, TLiveActor)
{
	TBossPakkun* boss = (TBossPakkun*)spine->getBody();

	if (spine->getTime() == 0)
		boss->changeBck(BOSSPAKU_BCK_WAIT);

	JGeometry::TVec3<f32> toMario = boss->getPosition();
	toMario.x -= gpMarioPos->x;
	toMario.y -= gpMarioPos->y;
	toMario.z -= gpMarioPos->z;

	f32 reach = boss->getSaveParam2()->mSLSwingLength.get();
	reach *= reach;

	f32 turn;
	if (toMario.squared() < reach)
		turn = boss->getSaveParam2()->mSLPivotSpeedAware.get();
	else
		turn = boss->getSaveParam2()->mSLPivotSpeed.get();

	if (boss->turnToCurPathNode(turn)) {
		boss->switchNextGoalPath();
		return TRUE;
	}

	return FALSE;
}

// Binding level over a raw member read, worth +16 of low region in
// TNerveBPSwallow::execute (batch 127).
static inline TMarioParticleManager* BosspakkunGetMarioParticleManager()
{
	TMarioParticleManager* marioParticleManager = gpMarioParticleManager;
	return marioParticleManager;
}

DEFINE_NERVE(TNerveBPSwallow, TLiveActor)
{
	TBossPakkun* boss = (TBossPakkun*)spine->getBody();

	if (spine->getTime() == 0)
		boss->changeBck(BOSSPAKU_BCK_WATER_HIT);

	if (boss->mWaterMark
	    >= boss->getSaveParam2()->mSLWaterMarkLimit.get()) {
		spine->pushAfterCurrent(&TNerveBPTumbleIn::theNerve());
		boss->mState = BOSSPAKU_STATE_NORMAL;
		boss->unk170 = 0;
		return TRUE;
	}

	MtxPtr mouth = boss->getModel()->getAnmMtx(18);
	BosspakkunGetMarioParticleManager()->emitAndBindToMtxPtr(
	    BOSSPAKKUN_JPA_MS_BOPA_WATHIT, mouth, 1, boss);
	BosspakkunGetMarioParticleManager()->emitAndBindToMtxPtr(
	    BOSSPAKKUN_JPA_MS_BOPA_WATHIT_W, mouth, 1, (u8*)boss + 1);

	if (boss->unk170 != 0) {
		boss->changeBck(BOSSPAKU_BCK_WATER_HIT);
		boss->unk170 = 0;
		return FALSE;
	}

	boss->mState = BOSSPAKU_STATE_NORMAL;
	spine->pushAfterCurrent(&TNerveBPWait::theNerve());
	return TRUE;
}

// Binding level over a raw member read, worth +8 of low region in
// TNerveBPTumbleIn::execute (batch 127).
// The 4-byte rung (frame ladder 318/320): a by-value scalar read through an
// existing accessor, landing in a named local.
static inline MActor* BosspakkunMActor(const TBossPakkun* p)
{
	MActor* actor = p->getMActor();
	return actor;
}

static inline f32 BosspakkunBckFrame(MActor* actor)
{
	f32 frame = actor->getFrameCtrl(ANM_TYPE_BCK)->getFrame();
	return frame;
}

static inline TCameraShake* BosspakkunGetCameraShake()
{
	TCameraShake* cameraShake = gpCameraShake;
	return cameraShake;
}

DEFINE_NERVE(TNerveBPTumbleIn, TLiveActor)
{
	TBossPakkun* boss = (TBossPakkun*)spine->getBody();
	MActor* actor     = boss->getMActor();

	if (spine->getTime() == 0)
		boss->changeBck(BOSSPAKU_BCK_DOWN);

	if (spine->getTime() == 336)
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    BOSSPAKKUN_JPA_MS_BOPA_DOWN, boss->getModel()->getAnmMtx(14), 0,
		    boss);

	if (spine->getTime() == 348) {
		BosspakkunGetCameraShake()->startShake(
		    (EnumCamShakeMode)CAM_SHAKE_MODE_BOPA_DOWN, 1.0f);
		boss->rumblePad(2, boss->mPosition);
	}

	if (actor->curAnmEndsNext(ANM_TYPE_BCK, nullptr)) {
		spine->pushAfterCurrent(&TNerveBPTumble::theNerve());
		return TRUE;
	}

	return FALSE;
}

DEFINE_NERVE(TNerveBPTumble, TLiveActor)
{
	TBossPakkun* boss = (TBossPakkun*)spine->getBody();

	if (spine->getTime() == 0) {
		boss->changeBck(BOSSPAKU_BCK_DOWN_LOOP);
		boss->mState = BOSSPAKU_STATE_BELLY_UP;
	}

	// The owner key is a byte-offset token, as in TBPTornado::perform's
	// three emitters (this, this + 1, this + 2).
	gpMarioParticleManager->emitAndBindToMtxPtr(
	    BOSSPAKKUN_JPA_MS_BOPA_JITA, boss->getModel()->getAnmMtx(0), 1,
	    (const u8*)boss + 8);
	gpCameraShake->keepShake(
	    (EnumCamShakeMode)CAM_SHAKE_MODE_BOPA_DOWN_LOOP, 1.0f);

	if ((spine->getTime() / 60) % 2)
		boss->rumblePad(0, boss->mPosition);

	if (spine->getTime() >= BosspakkunParams(boss)->mSLTumbleTime.get()) {
		boss->mState = BOSSPAKU_STATE_NORMAL;
		spine->pushAfterCurrent(&TNerveBPTumbleOut::theNerve());
		return TRUE;
	}

	return FALSE;
}

DEFINE_NERVE(TNerveBPTumbleOut, TLiveActor)
{
	TBossPakkun* boss = (TBossPakkun*)spine->getBody();
	MActor* actor     = BosspakkunMActor(boss);

	if (spine->getTime() == 0) {
		boss->changeBck(BOSSPAKU_BCK_GETUP);
		BosspakkunGetCameraShake()->startShake(
		    (EnumCamShakeMode)CAM_SHAKE_MODE_BOPA_GETUP, 1.0f);
		boss->rumblePad(0, boss->mPosition);
	}

	if (actor->curAnmEndsNext(ANM_TYPE_BCK, nullptr)) {
		if (actor->checkCurBckFromIndex(BOSSPAKU_BCK_GETUP)) {
			boss->changeBck(BOSSPAKU_BCK_RETURN);
			// Every third get-up of the first fight raises the "spray its
			// mouth" hint again.
			if (!boss->is2ndFightNow()) {
				boss->unk1C4 -= 1;
				if (boss->unk1C4 <= 0) {
					BosspakkunGetMarDirector()->getConsole()->startAppearBalloon(1, true);
					boss->unk1C4 = 3;
				}
			}
		} else {
			spine->pushAfterCurrent(&TNerveBPWait::theNerve());
			return TRUE;
		}
	}

	// TODO: frame-exact; the `mouth` block inside the resetWaterMark
	// expansion is still 4 low, and every rung here prices in 8s.
	if (actor->checkCurBckFromIndex(BOSSPAKU_BCK_RETURN)) {
		f32 frame = BosspakkunBckFrame(actor);
		if (140.0f < frame && frame < 160.0f && !boss->unk17C)
			boss->resetWaterMark();
		if (35.0f < frame)
			boss->unk1BC = 1;
	}

	return FALSE;
}

DEFINE_NERVE(TNerveBPGetUp, TLiveActor)
{
	TBossPakkun* boss = (TBossPakkun*)spine->getBody();
	MActor* actor     = boss->getMActor();

	if (spine->getTime() == 0) {
		boss->changeBck(BOSSPAKU_BCK_GETUP);
		gpCameraShake->startShake((EnumCamShakeMode)CAM_SHAKE_MODE_BOPA_GETUP, 1.0f);
		boss->rumblePad(0, boss->mPosition);
	}

	if (actor->curAnmEndsNext(ANM_TYPE_BCK, nullptr)) {
		if (actor->checkCurBckFromIndex(BOSSPAKU_BCK_GETUP))
			boss->changeBck(BOSSPAKU_BCK_PANPAN);
		else
			return TRUE;
	}

	return FALSE;
}

DEFINE_NERVE(TNerveBPSwing, TLiveActor)
{
	TBossPakkun* boss = (TBossPakkun*)spine->getBody();
	MActor* actor     = boss->getMActor();

	if (spine->getTime() == 0)
		boss->changeBck(BOSSPAKU_BCK_HEAD);

	if (spine->getTime() == 0)
		gpMarioParticleManager->emitAndBindToSRTMtxPtr(
		    BOSSPAKKUN_JPA_MS_BOPA_SWING2, boss->getModel()->getAnmMtx(18), 0,
		    boss);

	if (actor->curAnmEndsNext(ANM_TYPE_BCK, nullptr))
		return TRUE;

	return FALSE;
}

DEFINE_NERVE(TNerveBPStompReact, TLiveActor)
{
	TBossPakkun* boss = (TBossPakkun*)spine->getBody();
	MActor* actor     = boss->getMActor();

	if (spine->getTime() == 0) {
		boss->changeBck(BOSSPAKU_BCK_UNK5);
		boss->mHeadHit->onHitFlag(HIT_FLAG_NO_COLLISION);
	}

	if (spine->getTime() == 30 && !boss->unk17C)
		boss->resetWaterMark();

	if (spine->getTime() == 50)
		boss->unk1BC = 1;

	if (actor->curAnmEndsNext(ANM_TYPE_BCK, nullptr)) {
		boss->mHeadHit->offHitFlag(HIT_FLAG_NO_COLLISION);
		return TRUE;
	}

	return FALSE;
}

DEFINE_NERVE(TNerveBPJumpReact, TLiveActor)
{
	TBossPakkun* boss = (TBossPakkun*)spine->getBody();
	MActor* actor     = boss->getMActor();

	if (spine->getTime() == 0)
		boss->changeBck(BOSSPAKU_BCK_JUMP_REACTION);

	if (actor->curAnmEndsNext(ANM_TYPE_BCK, nullptr))
		return TRUE;

	return FALSE;
}

DEFINE_NERVE(TNerveBPPreDie, TLiveActor)
{
	TBossPakkun* boss = (TBossPakkun*)spine->getBody();
	MActor* actor     = boss->getMActor();

	if (spine->getTime() == 0) {
		boss->changeBck(BOSSPAKU_BCK_UNK5);
		boss->mHeadHit->onHitFlag(HIT_FLAG_NO_COLLISION);
		if (!boss->unk17C)
			boss->resetWaterMark();
		boss->killSmallEnemies();
		MSBgm::stopTrackBGM(1, 10);
	}

	if (actor->curAnmEndsNext(ANM_TYPE_BCK, nullptr)) {
		spine->pushAfterCurrent(&TNerveBPDie::theNerve());
		return TRUE;
	}

	return FALSE;
}

static inline MSound* BossPakkunDieSound()
{
	MSound* sound = gpMSound;
	return sound;
}

DEFINE_NERVE(TNerveBPDie, TLiveActor)
{
	TBossPakkun* boss = (TBossPakkun*)spine->getBody();
	MActor* actor     = boss->getMActor();

	// Both arms of this test compile to the same call; only the branch
	// itself is evidence that two statements were written. The accessor in
	// the first arm and the raw member in the second are the two spellings
	// that reach retail's frame.
	if (spine->getTime() == 0)
		BossPakkunDieSound()->getModBgm()->modBgm(0, 1);
	else
		BossPakkunDieSound()->unk98->modBgm(0, 1);

	if (actor->checkCurBckFromIndex(BOSSPAKU_BCK_DOWN_START)
	    && spine->getTime() == 680)
		boss->onLiveFlag(LIVE_FLAG_UNK8);

	if (actor->curAnmEndsNext(ANM_TYPE_BCK, nullptr)
	    && actor->checkCurBckFromIndex(BOSSPAKU_BCK_DOWN_START)) {
		boss->kill();
		gpItemManager->makeShineAppearWithDemo(
		    "シャイン（ボス用）", "ボスシャインカメラ", boss->mPosition.x,
		    boss->mPosition.y, boss->mPosition.z);
		return TRUE;
	}

	return FALSE;
}

DEFINE_NERVE(TNerveBPTakeOff, TLiveActor)
{
	TBossPakkun* boss = (TBossPakkun*)spine->getBody();
	MActor* actor     = boss->getMActor();

	if (spine->getTime() == 0) {
		boss->onLiveFlag(LIVE_FLAG_UNK10);
		boss->onLiveFlag(LIVE_FLAG_AIRBORNE);
		boss->changeBck(BOSSPAKU_BCK_FLY_START);
	}

	if (actor->checkCurBckFromIndex(BOSSPAKU_BCK_FLY_START)
	    && actor->curAnmEndsNext(ANM_TYPE_BCK, nullptr))
		boss->changeBck(BOSSPAKU_BCK_FLY);

	if (actor->checkCurBckFromIndex(BOSSPAKU_BCK_FLY)) {
		boss->mPosition.y += 5.0f;

		JGeometry::TVec3<f32> goal = boss->getUnk104().getPoint();
		if (goal.y < boss->mPosition.y) {
			boss->mPosition.y = goal.y;
			if (boss->unk124->getGraph())
				spine->pushAfterCurrent(&TNerveBPFly::theNerve());
			else
				spine->pushAfterCurrent(&TNerveBPTouchDown::theNerve());
			return TRUE;
		}
	}

	return FALSE;
}

// TODO: park for a shared header. PathNode.hpp's getPoint() reaches the
// node's actor through getPosition(); reading mPosition raw instead drops
// the +8 reference temp. Header round 27 measured the same change in the
// header as a wash (closes TNerveBPTouchDown, loses TNerveBPTakeOff and
// isReachedToGoal), so it stays TU-local.
static inline const JGeometry::TVec3<f32>&
BosspakkunGetPoint(const TPathNode& node)
{
	if (node.unk0 != 0)
		return node.unk0->mPosition;

	return node.unk4;
}

DEFINE_NERVE(TNerveBPFly, TLiveActor)
{
	TBossPakkun* boss = (TBossPakkun*)spine->getBody();

	if (spine->getTime() == 0) {
		boss->changeBck(BOSSPAKU_BCK_FLY);
		boss->goToRandomNextGraphNode();
		boss->startBGM();
	}

	JGeometry::TVec3<f32> toGoal = boss->getUnk104().getPoint();
	toGoal.x -= boss->mPosition.x;
	toGoal.y -= boss->mPosition.y;
	toGoal.z -= boss->mPosition.z;
	toGoal.y = 0.0f;

	if (VECMag(toGoal) < 100.0f) {
		// A graph node flagged 0x800 is one the boss hovers over instead of
		// flying straight past.
		if (!boss->getTracer()->getCurrent().checkFlag(0x800)) {
			boss->goToRandomNextGraphNode();
		} else {
			spine->pushAfterCurrent(&TNerveBPHover::theNerve());
			return TRUE;
		}
	}

	f32 turn  = boss->mTurnSpeed;
	// TODO: 99.8%, frame-exact 0xb8. BosspakkunParams lands the missing 8;
	// the remaining ~s are the toGoal / flyToCurPathNode temps 8 high
	// (allocation order). Swapping turn/speed declaration loses the f31
	// hoist of mTurnSpeed. BosspakkunGetPoint on toGoal drops the frame
	// 8 (99.7%); declaring turn first loads f31 early (98.6%).
	f32 speed = BosspakkunParams(boss)->mSLFlySpeed.get();
	boss->flyToCurPathNode(speed, turn);

	return FALSE;
}

// +4 setter rung (MapObjBianco ladder 337): lands the goal TVec3 at
// retail's 0x2c. The raw `mPosition.y = goal.y` leaves it 4 low.
static inline void BosspakkunSetPosY(TBossPakkun* p, f32 y)
{
	p->mPosition.y = y;
}

DEFINE_NERVE(TNerveBPTouchDown, TLiveActor)
{
	TBossPakkun* boss = (TBossPakkun*)spine->getBody();
	MActor* actor     = boss->mMActor;

	if (spine->getTime() == 0)
		boss->changeBck(BOSSPAKU_BCK_FLY);

	if (actor->checkCurBckFromIndex(BOSSPAKU_BCK_FLY)) {
		boss->mPosition.y -= 5.0f;

		JGeometry::TVec3<f32> goal = BosspakkunGetPoint(boss->getUnk104());
		if (goal.y > boss->mPosition.y) {
			BosspakkunSetPosY(boss, goal.y);
			boss->changeBck(BOSSPAKU_BCK_LAND);
			boss->offLiveFlag(LIVE_FLAG_UNK10);
		}
	}

	if (actor->checkCurBckFromIndex(BOSSPAKU_BCK_LAND)
	    && actor->curAnmEndsNext(ANM_TYPE_BCK, nullptr)) {
		spine->pushAfterCurrent(&TNerveBPWait::theNerve());
		return TRUE;
	}

	return FALSE;
}

DEFINE_NERVE(TNerveBPFlyCannon, TLiveActor)
{
	TBossPakkun* boss = (TBossPakkun*)spine->getBody();
	MActor* actor     = boss->getMActor();

	if (spine->getTime() == 0)
		boss->changeBck(BOSSPAKU_BCK_FLY_POLLUT);

	if (spine->getTime() == 168)
		boss->launchPolDrop();

	if (actor->curAnmEndsNext(ANM_TYPE_BCK, nullptr))
		return TRUE;

	return FALSE;
}

DEFINE_NERVE(TNerveBPFlyPivot, TLiveActor)
{
	TBossPakkun* boss = (TBossPakkun*)spine->getBody();

	if (spine->getTime() == 0)
		boss->changeBck(BOSSPAKU_BCK_FLY);

	if (boss->turnToCurPathNode(
	        boss->getSaveParam2()->mSLPivotSpeed.get())) {
		boss->switchNextGoalPath();
		return TRUE;
	}

	return FALSE;
}

DEFINE_NERVE(TNerveBPHover, TLiveActor)
{
	TSpineEnemy* body = spine->getBody();
	TBossPakkun* boss = (TBossPakkun*)body;

	if (spine->getTime() == 0) {
		boss->changeBck(BOSSPAKU_BCK_HOVERING);
		boss->mState = BOSSPAKU_STATE_FLYING;
	}

	TBossPakkunParams* params = BosspakkunParams(boss);
	f32 range = params->mSLPollBallRange.value;

	if (boss->inArea(*gpMarioPos)
	    && boss->mDistToMarioSquared < range * range) {
		spine->pushAfterCurrent(&TNerveBPHover::theNerve());
		spine->pushAfterCurrent(&TNerveBPFlyCannon::theNerve());

		TPathNode node(*gpMarioPos);
		boss->unk114.push(boss->unkF4);
		boss->unkF4 = node;

		spine->pushAfterCurrent(&TNerveBPFlyPivot::theNerve());
		return TRUE;
	}

	if (spine->getTime() >= BosspakkunParams(boss)->mSLHoverTimer.get()) {
		spine->pushAfterCurrent(&TNerveBPFly::theNerve());
		boss->mState = BOSSPAKU_STATE_NORMAL;
		return TRUE;
	}

	return FALSE;
}

DEFINE_NERVE(TNerveBPFall, TLiveActor)
{
	TBossPakkun* boss = (TBossPakkun*)spine->getBody();
	MActor* actor     = BosspakkunMActor(boss);

	if (spine->getTime() == 0) {
		boss->offLiveFlag(LIVE_FLAG_UNK10);
		boss->onLiveFlag(LIVE_FLAG_AIRBORNE);
		boss->changeBck(BOSSPAKU_BCK_FALL_START);
	}

	if (actor->checkCurBckFromIndex(BOSSPAKU_BCK_FALL_START)) {
		if (actor->curAnmEndsNext(ANM_TYPE_BCK, nullptr))
			boss->changeBck(BOSSPAKU_BCK_FALL_LOOP);
	} else if (actor->checkCurBckFromIndex(BOSSPAKU_BCK_FALL_LOOP)) {
		if (!boss->isAirborne()) {
			boss->changeBck(BOSSPAKU_BCK_FALL_END);
			BosspakkunGetCameraShake()->startShake(
			    (EnumCamShakeMode)CAM_SHAKE_MODE_BOPA_POPO, 1.0f);
			boss->rumblePad(2, boss->mPosition);
		}
	} else if (actor->checkCurBckFromIndex(BOSSPAKU_BCK_FALL_END)) {
		if (actor->curAnmEndsNext(ANM_TYPE_BCK, nullptr)) {
			boss->changeBck(BOSSPAKU_BCK_GETUP);
			BosspakkunGetCameraShake()->startShake(
			    (EnumCamShakeMode)CAM_SHAKE_MODE_BOPA_GETUP, 1.0f);
			boss->rumblePad(0, boss->mPosition);
		}
	} else if (actor->checkCurBckFromIndex(BOSSPAKU_BCK_GETUP)) {
		if (actor->curAnmEndsNext(ANM_TYPE_BCK, nullptr)) {
			if (boss->is2ndFightNow()) {
				f32 prop = BosspakkunParams(boss)->mSLTornadoProp.get();
				if (boss->mTornado->mState != BOSSPAKU_TORNADO_DEAD
				    || MsRandF() < prop) {
					spine->pushAfterCurrent(&TNerveBPTakeOff::theNerve());
					spine->pushAfterCurrent(&TNerveBPVomit::theNerve());
				} else if (boss->mTornado->mState
				           == BOSSPAKU_TORNADO_DEAD) {
					spine->pushAfterCurrent(&TNerveBPWait::theNerve());
					spine->pushAfterCurrent(&TNerveBPTornado::theNerve());
				} else {
					spine->pushAfterCurrent(&TNerveBPWait::theNerve());
				}
			} else {
				spine->pushAfterCurrent(&TNerveBPWait::theNerve());
			}
			return TRUE;
		}
	}

	return FALSE;
}

DEFINE_NERVE(TNerveBPSleep, TLiveActor)
{
	TBossPakkun* boss = (TBossPakkun*)spine->getBody();

	if (spine->getTime() == 0)
		boss->changeBck(BOSSPAKU_BCK_SLEEP);

	return FALSE;
}

DEFINE_NERVE(TNerveBPBreakSleep, TLiveActor)
{
	TBossPakkun* boss = (TBossPakkun*)spine->getBody();

	if (spine->getTime() == 0) {
		boss->changeBck(BOSSPAKU_BCK_GETUP);
		MSBgm::stopTrackBGMs(7, 10);
	}

	MActor* actor = boss->getMActor();
	if (actor->curAnmEndsNext(ANM_TYPE_BCK, nullptr)) {
		spine->pushAfterCurrent(&TNerveBPTakeOff::theNerve());
		return TRUE;
	}

	return FALSE;
}

DEFINE_NERVE(TNerveBPWaitL, TLiveActor)
{
	TBossPakkun* boss = (TBossPakkun*)spine->getBody();
	MActor* actor     = boss->getMActor();

	if (spine->getTime() == 0)
		actor->setBck("bosspaku_wait");

	if (spine->getTime()
	    >= boss->getSaveParam2()->mSLWaitFrameStg0.get()) {
		bool inArea = boss->inArea(*gpMarioPos);
		if (inArea) {
			if (!SMS_GetMarioGroundPlane()->isWaterSurface()) {
				spine->pushAfterCurrent(&TNerveBPCannonL::theNerve());
				return TRUE;
			}
		}

		if (actor->curAnmEndsNext(ANM_TYPE_BCK, nullptr)) {
			spine->pushAfterCurrent(&TNerveBPWaitL::theNerve());
			return TRUE;
		}
	}

	return FALSE;
}

DEFINE_NERVE(TNerveBPCannonL, TLiveActor)
{
	TBossPakkun* boss = (TBossPakkun*)spine->getBody();
	MActor* actor     = boss->getMActor();

	if (spine->getTime() == 0)
		actor->setBck("bosspaku_pollut_start");

	if (actor->curAnmEndsNext(ANM_TYPE_BCK, nullptr)) {
		if (actor->checkCurAnm("bosspaku_pollut_start", ANM_TYPE_BCK)) {
			actor->setBck("bosspaku_ball_end");
			boss->launchPolDrop();
		} else {
			spine->pushAfterCurrent(&TNerveBPWaitL::theNerve());
			return TRUE;
		}
	}

	return FALSE;
}
