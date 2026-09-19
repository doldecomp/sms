#include <MoveBG/MapObjMamma.hpp>
#include <System/DummyStrings.hpp>
#include <M3DUtil/InfectiousStrings.hpp>
#include <Map/MapCollisionManager.hpp>
#include <Camera/Camera.hpp>
#include <Camera/CameraShake.hpp>
#include <Enemy/Beam.hpp>
#include <Enemy/ChuuHana.hpp>
#include <Enemy/SleepBossHanachan.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JGeometry.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorUtil.hpp>
#include <Map/JointModel.hpp>
#include <Map/JointObj.hpp>
#include <Map/Map.hpp>
#include <Map/MapCollisionData.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <Map/MapCollisionManager.hpp>
#include <Map/MapMirror.hpp>
#include <Map/MapModel.hpp>
#include <Map/MapData.hpp>
#include <Map/MapStaticObject.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <MoveBG/ItemManager.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <MoveBG/MapObjWave.hpp>
#include <MSound/BackgroundMusic.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <Player/MarioAccess.hpp>
#include <Strategic/MirrorActor.hpp>
#include <System/MarDirector.hpp>
#include <System/Particles.hpp>
#include <System/TargetArrow.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <GC2D/GCConsole2.hpp>
#include <math.h>
#include <stdio.h>
#include <string.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

u32 TSandLeaf::touchWater(THitActor* actor)
{
	mOwner->grow();
	return 1;
}

void TSandLeaf::control()
{
	TMapObjBase::control();
	mGroundHeight = gpMap->checkGround(mPosition.x, mPosition.y + 200.0f,
	                                   mPosition.z, &mGroundPlane);
	mPosition.y = mGroundHeight;
}

int TSandBase::mWitherTime = 800;
f32 TSandBase::mScaleMin    = 0.00001f;

// UNUSED: never emitted. The only place the shrunk state is tested is
// withering's own tail, so this is the predicate it was factored out of.
// TODO: 0x34 in the map; check the compiled size against that.
bool TSandBase::isDown() const
{
	if (mScaling.y == mScaleMin)
		return true;

	return false;
}

bool TSandBase::withering()
{
	mScaling.y -= mWitherSpeed;
	if (mScaling.y < mScaleMin)
		mScaling.y = mScaleMin;

	gpMSound->startSoundActor(MSD_SE_OBJ_SANDBUD_NORMAL, &mTrigger->mPosition,
	                          0, nullptr, 0, 4);

	if (mScaling.y <= mScaleMin)
		return true;

	return false;
}

TSandBase::TSandBase(const char* name)
    : TMapObjBase(name)
{
	mGrowSpeed   = 0.0f;
	mWitherSpeed = 0.0f;
	mTrigger     = nullptr;
}

// 99.9%: every instruction matches, only the 24-byte frame gap is left. The
// rumble is on the leaf's own position, not the trigger's, and retail names
// the frame rate and the current frame as two f32 locals (f31 then f30) plus
// the trigger pointer as a third.
void TSandLeafBase::grow()
{
	if (mState == STATE_GROWN || mState == STATE_GROWING) {
		if (mScaling.y < 1.0f) {
			mScaling.y += mGrowSpeed;
			if (mScaling.y > 1.0f)
				mScaling.y = 1.0f;

			if (mState == STATE_GROWN) {
				mMapCollisionManager->changeCollision(1);
				TMapCollisionManager* manager = mMapCollisionManager;
				Mtx mtx;
				MsMtxSetTRS(mtx, mPosition.x, mPosition.y, mPosition.z,
				            mRotation.x, mRotation.y, mRotation.z, mScaling.x,
				            mScaling.y, mScaling.z);
				TMapCollisionBase* entry = manager->unk8;
				entry->setMtx(mtx);
				entry->setUp();
				mTrigger->startControlAnim(2);
				mState = STATE_GROWING;
			}

			f32 f31          = SMSGetAnmFrameRate();
			TMapObjBase* r31 = mTrigger;
			f32 f30          = r31->getMActor()->getFrameCtrl(0)->getFrame();
			r31->getMActor()->getFrameCtrl(0)->setFrame(f31 + f30);
			SMSRumbleMgr->start(0x15, 5, &mPosition);
			gpMSound->startSoundActor(MSD_SE_OBJ_SANDBUD_NORMAL,
			                          &mTrigger->mPosition, 0, nullptr, 0, 4);
			mStateTimer = mWitherTime;
		}
	}
}

// Binding level over a raw member read, worth +16 of low region in
// TSandLeafBase::control (batch 127).
static inline TMapCollisionManager* MapObjMammaMapCollisionManager(const TSandLeafBase* p)
{
	TMapCollisionManager* mapCollisionManager = p->mMapCollisionManager;
	return mapCollisionManager;
}

void TSandLeafBase::control()
{
	TMapObjBase::control();

	switch (mState) {
	case STATE_WITHER:
		SMSRumbleMgr->start(0x13, -1, &mPosition);
		if (withering()) {
			SMSRumbleMgr->stop(0x13);
			MapObjMammaMapCollisionManager(this)->changeCollision(0);
			TMapCollisionManager* manager = MapObjMammaMapCollisionManager(this);
			Mtx mtx;
			MsMtxSetTRS(mtx, mPosition.x, mPosition.y, mPosition.z,
			            mRotation.x, mRotation.y, mRotation.z, mScaling.x,
			            mScaling.y, mScaling.z);
			TMapCollisionBase* entry = manager->unk8;
			entry->setMtx(mtx);
			entry->setUp();
			mStateTimer = mReviveTime;
			mState      = STATE_REVIVING;
			break;
		}
		// fallthrough
	case STATE_GROWN:
		break;

	case STATE_REVIVING:
		if (!isStateTimerEngaged() && mTrigger->animIsFinished()) {
			mTrigger->awake();
			mTrigger->startAnim(1);
			gpMSound->startSoundActor(MSD_SE_IT_COMMON_APPEAR,
			                          &mTrigger->mPosition, 0, nullptr, 0, 4);
			mState = STATE_REVIVED;
		}
		break;

	case STATE_REVIVED:
		if (mTrigger->animIsFinished()) {
			mTrigger->startAnim(0);
			mState = STATE_GROWN;
		}
		break;
	}
}

// Binding level over a raw member read, worth +8 of low region in
// TSandLeafBase::initMapObj (batch 127).
static inline TSandBomb* MapObjMammaTrigger(const TSandLeafBase* p)
{
	TSandBomb* trigger = p->mTrigger;
	return trigger;
}

void TSandLeafBase::initMapObj()
{
	mGrowSpeed   = 0.003f;
	mWitherSpeed = 0.001f;
	mReviveTime  = 0;
	mScaling.y   = mScaleMin;
	TMapObjBase::initMapObj();

	mTrigger = (TSandBomb*)TMapObjManager::newAndRegisterObj(
	    "SandLeaf", mPosition, mRotation,
	    JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
	mTrigger->mOwner = this;
	MapObjMammaTrigger(this)->appear();
}

void TSandBomb::makeObjAppeared()
{
	TMapObjBase::makeObjAppeared();
	startControlAnim(1);
	startControlAnim(2);
}

u32 TSandBomb::touchWater(THitActor* actor)
{
	mMActor->getFrameCtrl(0)->setFrame(TSandBombBase::mFiringFrameSpeed
	                                   + mMActor->getFrameCtrl(0)->getFrame());
	mMActor->getFrameCtrl(5)->setFrame(TSandBombBase::mFiringFrameSpeed
	                                   + mMActor->getFrameCtrl(5)->getFrame());

	mMActor->getFrameCtrl(0);
	soundBas(MSD_SE_OBJ_SANDBOMB_WATER_1, 7.0f,
	         TSandBombBase::mFiringFrameSpeed);
	soundBas(MSD_SE_OBJ_SANDBOMB_WATER_2, 50.0f,
	         TSandBombBase::mFiringFrameSpeed);
	soundBas(MSD_SE_OBJ_SANDBOMB_WATER_3, 100.0f,
	         TSandBombBase::mFiringFrameSpeed);
	soundBas(MSD_SE_OBJ_SANDBOMB_WATER_4, 150.0f,
	         TSandBombBase::mFiringFrameSpeed);

	if (mMActor->curAnmEndsNext(0, nullptr)) {
		mOwner->grow();
		startControlAnim(3);
		startControlAnim(4);
		startControlAnim(5);
		mHitFlags |= 1;
	}

	return 1;
}

u32 TSandBomb::getSDLModelFlag() const { return 0; }

void TSandBomb::initMapObj() { TMapObjBase::initMapObj(); }

void TSandBombBase::withered()
{
	mStateTimer = mReviveTime;
	mState      = STATE_REVIVING;
	mTrigger->sleep();
}

void TSandBombBase::expanded()
{
	TSandBomb* trigger = mTrigger;
	trigger->getMActor()->getFrameCtrl(0)->setFrame(
	    mExpandFrameSpeed + trigger->getMActor()->getFrameCtrl(0)->getFrame());

	gpMSound->startSoundActor(MSD_SE_OBJ_SAMDBOMB_REVERSE,
	                          &mTrigger->mPosition, 0, nullptr, 0, 4);

	if (mTrigger->animIsFinished())
		mState = STATE_WITHER;
}

// TODO: 77.9%. The frame-control advance and the Mario throw are right; the
// residual is the order of the mTrigger reloads.
void TSandBombBase::exploding()
{
	mMActor->getFrameCtrl(0)->setFrame(mExplodeFrameSpeed
	                                   + mMActor->getFrameCtrl(0)->getFrame());
	mTrigger->getMActor()->getFrameCtrl(0)->setFrame(
	    mExplodeFrameSpeed + mTrigger->getMActor()->getFrameCtrl(0)->getFrame());

	f32 distance = getDistanceXZ(*gpMarioPos);

	if (!isActorType(0x400000CE) && mMActor->getFrameCtrl(0)->getFrame() < 80.0f
	    && SMS_GetMarioGrLevel() > gpMarioPos->y - 30.0f
	    && distance < mMarioJumpRange) {
		SMS_SendMessageToMario(this, 7);
		SMS_ThrowMario(JGeometry::TVec3<f32>(0.0f, 1.0f, 0.0f),
		               mMarioJumpRate * (mMarioJumpRange - distance));
	}

	if (animIsFinished()) {
		mTrigger->startControlAnim(6);
		mState = STATE_EXPANDED;
	}
}

void TSandBombBase::explode()
{
	startControlAnim(1);
	mScaling.y = 1.0f;
	mMapCollisionManager->changeCollision(1);
	mMapCollisionManager->unk8->setUp();
	if (mMapCollisionManager->unk8)
		mMapCollisionManager->unk8->moveSRT(mPosition, mRotation, mScaling);

	JPABaseEmitter* emitter = gpMarioParticleManager->emit(
	    0x55, (JGeometry::TVec3<f32>*)&mPosition, 0, nullptr);
	JGeometry::TVec3<f32> effectScale(mExplodeEffectScale,
	                                  mExplodeEffectScale,
	                                  mExplodeEffectScale);
	emitter->setGlobalParticleScale(effectScale);
	emitter->setGlobalDynamicsScale(effectScale);

	if (SMSGetMarDirector()->unk124 != 3 && SMSGetMarDirector()->unk124 != 4)
		gpCameraShake->startShake(CAM_SHAKE_MODE_SAND_BOMB_APPEAR, 1.0f);

	gpMSound->startSoundActor(MSD_SE_OBJ_SANDBOMB_BANG, &mPosition, 0, nullptr,
	                          0, 4);
	SMSRumbleMgr->start(0x15, mExlodingRumbleTime, &mPosition);
	mState = STATE_EXPLODE;
}

void TSandBombBase::waitBeforeExplode()
{
	mState      = STATE_WAIT_BOM;
	mStateTimer = mExplodeWaitTime;
}

void TSandBombBase::grow() { mState = STATE_FIRING; }

void TSandBombBase::control()
{
	TMapObjBase::control();

	switch (mState) {
	case STATE_GROWN: {
		f32 frame = mTrigger->getMActor()->getFrameCtrl(0)->getFrame()
		    - mFiringFrameDownSpeed;
		if (frame >= 0.0f) {
			mTrigger->getMActor()->getFrameCtrl(0)->setFrame(frame);
			mTrigger->getMActor()->getFrameCtrl(5)->setFrame(frame);
		}
		break;
	}

	case STATE_FIRING:
		mTrigger->getMActor()->getFrameCtrl(0)->setFrame(
		    mExplodeFrameSpeed
		    + mTrigger->getMActor()->getFrameCtrl(0)->getFrame());
		mTrigger->getMActor()->getFrameCtrl(5)->setFrame(
		    mExplodeFrameSpeed
		    + mTrigger->getMActor()->getFrameCtrl(5)->getFrame());
		mTrigger->getMActor()->getFrameCtrl(3)->setFrame(
		    mExplodeFrameSpeed
		    + mTrigger->getMActor()->getFrameCtrl(3)->getFrame());
		if (mTrigger->animIsFinished())
			waitBeforeExplode();
		break;

	case STATE_WAIT_BOM:
		if (!isStateTimerEngaged())
			explode();
		break;

	case STATE_EXPLODE:
		exploding();
		break;

	case STATE_EXPANDED:
		expanded();
		break;

	case STATE_WITHER:
		SMSRumbleMgr->start(0x13, -1, &mPosition);
		if (withering()) {
			withered();
			SMSRumbleMgr->stop(0x13);
		}
		break;

	case STATE_REVIVING:
		if (!isStateTimerEngaged()) {
			mState = STATE_GROWN;
			mTrigger->awake();
			mTrigger->startControlAnim(1);
			mTrigger->startControlAnim(2);
		}
		break;
	}

	if (mTrigger->mColCount == 0)
		mTrigger->unk140 = false;
}

TSandBomb* TSandBombBase::findTriggerActor()
{
	return (TSandBomb*)TMapObjManager::newAndRegisterObj(
	    "SandBomb", mPosition, mRotation,
	    JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
}

void TSandBombBase::loadAfter()
{
	mTrigger         = findTriggerActor();
	mTrigger->mOwner = this;
	mTrigger->appear();
}

void TSandBombBase::initMapObj()
{
	mGrowSpeed        = 0.006f;
	mWitherSpeed      = 0.005f;
	mReviveTime       = 0;
	mExplodeWaitTime  = 60;
	mMarioJumpRange   = 1000.0f;
	mScaling.y        = mScaleMin;
	TMapObjBase::initMapObj();
	mExpandFrameSpeed = 0.5f;

	if (strcmp(unkF4, "SandBombBasePyramid") == 0) {
		mExplodeEffectScale = 1.3f;
		mMarioJumpRange     = 1200.0f;
	} else if (strcmp(unkF4, "SandBombBaseShit") == 0) {
		mExplodeEffectScale = 1.3f;
		mMarioJumpRange     = 1500.0f;
	} else if (strcmp(unkF4, "SandBombBaseStar") == 0) {
		mExplodeEffectScale = 1.2f;
	} else if (strcmp(unkF4, "SandBombBaseTurtle") == 0) {
		mExplodeEffectScale = 1.2f;
	}

	SMS_LoadParticle("/scene/mapObj/SandBomb.jpa", 0x55);
}

f32 TSandBombBase::mFiringFrameSpeed     = 3.0f;
f32 TSandBombBase::mFiringFrameDownSpeed = 0.2f;
f32 TSandBombBase::mExplodeFrameSpeed    = 1.0f;
f32 TSandBombBase::mMarioJumpRate        = 0.12f;
int TSandBombBase::mExlodingRumbleTime   = 20;

TSandBombBase::TSandBombBase(const char* name)
    : TSandBase(name)
{
	mExplodeWaitTime    = 0;
	mExplodeEffectScale = 1.0f;
	mExpandFrameSpeed   = 0.0f;
	mMarioJumpRange     = 0.0f;
}

f32 TSandCastle::mCollisionRate = 1.7f;

// TODO: 76.2%. The animation advance matches; the scale term and the
// stage-change kill still differ in register use.
bool TSandCastle::withering()
{
	mMActor->getFrameCtrl(0)->setFrame(mWitherSpeed
	                                   + mMActor->getFrameCtrl(0)->getFrame());
	mMActor->getFrameCtrl(5)->setFrame(mWitherSpeed
	                                   + mMActor->getFrameCtrl(5)->getFrame());

	f32 frame  = mMActor->getFrameCtrl(0)->getFrame();
	f32 end    = mMActor->getFrameCtrl(0)->getEnd();
	mScaling.y = mCollisionRate * ((end - frame) / end);

	if (frame > 240.0f) {
		if (!mChangeStage->checkLiveFlag(LIVE_FLAG_DEAD)) {
			mChangeStage->kill();
			gpTargetArrow->unk14 = 0;
		}
	}

	if (animIsFinished()) {
		sleep();
		return true;
	}

	return false;
}

void TSandCastle::expanded()
{
	TSandBomb* trigger = mTrigger;
	trigger->getMActor()->getFrameCtrl(0)->setFrame(
	    mExpandFrameSpeed + trigger->getMActor()->getFrameCtrl(0)->getFrame());

	gpMSound->startSoundActor(MSD_SE_OBJ_SAMDBOMB_REVERSE,
	                          &mTrigger->mPosition, 0, nullptr, 0, 4);

	if (mTrigger->animIsFinished())
		mState = STATE_WITHER;

	if (mTrigger->animIsFinished()) {
		mState = STATE_WITHER;
		startControlAnim(2);
		startControlAnim(3);
	}
}

void TSandCastle::explode()
{
	startControlAnim(1);
	mScaling.y = 1.0f;
	mMapCollisionManager->changeCollision(1);
	mMapCollisionManager->unk8->setUp();
	if (mMapCollisionManager->unk8)
		mMapCollisionManager->unk8->moveSRT(mPosition, mRotation, mScaling);

	JPABaseEmitter* emitter = gpMarioParticleManager->emit(
	    0x55, (JGeometry::TVec3<f32>*)&mPosition, 0, nullptr);
	JGeometry::TVec3<f32> effectScale(mExplodeEffectScale,
	                                  mExplodeEffectScale,
	                                  mExplodeEffectScale);
	emitter->setGlobalParticleScale(effectScale);
	emitter->setGlobalDynamicsScale(effectScale);

	if (SMSGetMarDirector()->unk124 != 3 && SMSGetMarDirector()->unk124 != 4)
		gpCameraShake->startShake(CAM_SHAKE_MODE_SAND_BOMB_APPEAR, 1.0f);

	gpMSound->startSoundActor(MSD_SE_OBJ_SANDBOMB_BANG, &mPosition, 0, nullptr,
	                          0, 4);
	SMSRumbleMgr->start(0x15, mExlodingRumbleTime, &mPosition);
	mState = STATE_EXPLODE;

	awake();
	mChangeStage->appear();
	startControlAnim(3);
}

// TODO: the ROM takes the address of each of the two camera vectors
// (`addi r4, gpCamera, 0x124`, then reads at 0 and 8) where we read them at
// absolute offsets off gpCamera.  Ruled out: TU-local `static inline`s
// returning a reference to each member -- byte-identical.
static s32 SandCastleCallBack(u32 param_1, u32 param_2)
{
	if (param_2 == 1) {
		gpTargetArrow->unk14 = 1;
		gpTargetArrow->setPos(
		    JGeometry::TVec3<f32>(8400.0f, 300.0f, 8150.0f));
		const JGeometry::TVec3<f32>& eye = gpCamera->unk124;
		const JGeometry::TVec3<f32>& at  = gpCamera->unk148;
		gpCamera->warpPosAndAt(gpCamera->mCurrentTarget.unk28,
		                       matan(eye.z - at.z, eye.x - at.x));
	}

	return 1;
}

void TSandCastle::waitBeforeExplode()
{
	JDrama::TFlagT<u16> flag(0);
	mState      = STATE_WAIT_BOM;
	mStateTimer = mExplodeWaitTime;
	SMSGetMarDirector()->fireStartDemoCamera("mamma1_sandcastle", nullptr, -1,
	                                         0.0f, true, SandCastleCallBack, 0,
	                                         nullptr, flag);
	mDemoFired = true;
}

void TSandCastle::calcRootMatrix()
{
	if (!isState(STATE_WITHER))
		TMapObjBase::calcRootMatrix();
}

TSandBomb* TSandCastle::findTriggerActor()
{
	// The 「砂の城爆発の芽」 bud stands in for the sand bomb here: it keeps the
	// same owner pointer at 0x138 and takes the water jet for the castle.
	return (TSandBomb*)JDrama::TNameRefGen::search2("砂の城爆発の芽");
}

void TSandCastle::loadAfter()
{
	mTrigger         = findTriggerActor();
	mTrigger->mOwner = this;
	mTrigger->appear();

	mChangeStage
	    = (TMapObjBase*)JDrama::TNameRefGen::search2("ステージ切替（砂の城）");
	mChangeStage->makeObjDead();
}

void TSandCastle::initMapObj()
{
	TSandBombBase::initMapObj();
	mWitherSpeed     = 0.11f;
	mExplodeWaitTime = 120;
	sleep();
}

TSandCastle::TSandCastle(const char* name)
    : TSandBombBase(name)
{
	mChangeStage = nullptr;
	mDemoFired   = false;
}

int TLeanMirror::mGoTargetTime  = 600;
int TLeanMirror::mDemoWaitTime  = -1;
int TLeanMirror::mDemoLightTime = 360;

// UNUSED (0x1c). controlShake's first condition is a materialised bool built
// from mHitNum, the count of chuu-hanas still riding the mirror, so this is
// the predicate it was factored out of. The explicit return true/return false
// shape is what materialises at the call site.
bool TLeanMirror::enemyIsOn() const
{
	if (mHitNum != 0)
		return true;

	return false;
}

void TLeanMirror::draw() const
{
	MtxPtr mtx = getModel()->getAnmMtx(0);
	JGeometry::TVec3<f32> up(mtx[0][1], mtx[1][1], mtx[2][1]);

	JGeometry::TVec3<f32> nearPos(up);
	f32 length = 0.001f * (350.0f * mBodyRadius);
	nearPos.scale(length);
	nearPos.add(mPosition);

	JGeometry::TVec3<f32> farPos(up);
	farPos.scale(10000.0f);
	farPos.add(mPosition);

	gpBeamManager->requestCone(farPos, nearPos, 1.7f * mBodyRadius, true, true,
	                           false);
}

// UNUSED: the shared body of the four places that tilt the mirror towards the
// pusher's position.
// TODO: 0xac in the map; check the compiled size against that.
void TLeanMirror::updateSpeedVec(const JGeometry::TVec3<f32>& pos, f32 power)
{
	MtxPtr mtx = getModel()->getAnmMtx(0);
	f32 dz     = (pos.z - mPosition.z) / fabsf(mSize * mtx[2][2]);
	mSpeed.x += power * ((pos.x - mPosition.x) / fabsf(mSize * mtx[0][0])
	                     - mtx[0][1]);
	mSpeed.z += power * (dz - mtx[2][1]);
}

BOOL TLeanMirror::receiveMessage(THitActor* sender, u32 message)
{
	switch (message) {
	case 0:
		sendMsg(0x10000016, message);
		updateSpeedVec(sender->mPosition, mWaterPower);
		return TRUE;

	case 1:
		sendMsg(0x10000016, message);
		updateSpeedVec(sender->mPosition, mWaterJetPower);
		return TRUE;

	case 3:
		updateSpeedVec(sender->mPosition, mFruitPower);
		return TRUE;

	case 8:
		mHitNum--;
		if (mHitNum == 0)
			release();
		return TRUE;

	default:
		return FALSE;
	}
}

void TLeanMirror::touchPlayer(THitActor* actor)
{
	if (isState(STATE_SHAKE) && marioIsOn()) {
		updateSpeedVec(actor->mPosition, mMarioPower);
		if (!mBgmStarted) {
			MSBgm::startBGM(MSD_BGM_CHUBOSS2);
			MSBgm::setTrackVolume(0, 0.0f, 0xA, 0);
			mBgmStarted = true;
		}
	}
}

void TLeanMirror::touchEnemy(THitActor* actor)
{
	// 0x10000016 is TChuuHana's actor type and 0x1b0 its own flag; the
	// mirror is only pushed by a chuu-hana that is rolling.
	if (actor->isActorType(0x10000016) && ((TChuuHana*)actor)->unk1B0)
		updateSpeedVec(actor->mPosition, mEnemyPower);
}

// UNUSED (0x100): the rotation step shared by controlShake and
// controlGoTarget. TMatrix34::identity()'s chained assignments reproduce the
// ROM's exact store order for the scratch matrix.
void TLeanMirror::calcCurrentMtx(Mtx mtx)
{
	JGeometry::TMatrix34<JGeometry::SMatrix34C<f32> > rot;
	rot.identity();
	makeMtxRotByAxis(mRotAxis, mRotSpeed, rot);
	concatOnlyRotFromLeft(rot, mtx, mtx);
}

void TLeanMirror::release()
{
	MtxPtr mtx = getModel()->getAnmMtx(0);
	f32 f31    = mtx[0][1];
	f32 f30    = mtx[1][1];
	f32 f29    = mtx[2][1];

	mRotAxis.x = f30 * mToStone.z - f29 * mToStone.y;
	mRotAxis.y = f29 * mToStone.x - f31 * mToStone.z;
	mRotAxis.z = f31 * mToStone.y - f30 * mToStone.x;

	f32 cx = f30 * mToStone.z - f29 * mToStone.y;
	f32 cy = f29 * mToStone.x - f31 * mToStone.z;
	f32 cz = f31 * mToStone.y - f30 * mToStone.x;
	JGeometry::TUtil<f32>::sqrt(cx * cx + cy * cy + cz * cz);

	mRotSpeed = fabsf(atan2f(f31 * mToStone.x + f30 * mToStone.y
	                             + f29 * mToStone.z,
	                         mToStone.z))
	    / mGoTargetTime;
	mStateTimer = mGoTargetTime;
	mState      = STATE_GO_TARGET;
	offMapObjFlag(MAP_OBJ_FLAG_UNK2);

	SMS_MarioMoveRequest(mMarioMovePos);

	if (strcmp(unkF4, "mirrorS") == 0) {
		SMSGetMarDirector()->fireStartDemoCamera(
		    "ぐらぐら鏡Ｓカメラ", &mShiningStone->mPosition,
		    mGoTargetTime + mDemoWaitTime, 0.0f, true, nullptr, 0, nullptr,
		    JDrama::TFlagT<u16>(0));
	} else if (strcmp(unkF4, "mirrorM") == 0) {
		SMSGetMarDirector()->fireStartDemoCamera(
		    "ぐらぐら鏡Ｍカメラ", &mShiningStone->mPosition,
		    mGoTargetTime + mDemoWaitTime, 0.0f, true, nullptr, 0, nullptr,
		    JDrama::TFlagT<u16>(0));
	} else if (strcmp(unkF4, "mirrorL") == 0) {
		SMSGetMarDirector()->fireStartDemoCamera(
		    "ぐらぐら鏡Ｌカメラ", &mShiningStone->mPosition,
		    mGoTargetTime + mDemoWaitTime, 0.0f, true, nullptr, 0, nullptr,
		    JDrama::TFlagT<u16>(0));
	}

	MSBgm::stopTrackBGM(1, 0xA);
}

static s32 startCameraShakeSE(u32 param_1, u32 param_2) { return 0; }

void TLeanMirror::controlGoTarget()
{
	calcCurrentMtx(getModel()->getAnmMtx(0));

	if (!isStateTimerEngaged()) {
		mShiningStone->putOnLight(this);

		if (mShiningStone->mLitAll) {
			TSleepBossHanachan* hanachan
			    = (TSleepBossHanachan*)JDrama::TNameRefGen::search2(
			        "居眠りボスハナチャン");
			if (hanachan) {
				hanachan->startFall(mShiningStone->mPosition.x,
				                    mShiningStone->mPosition.y + 1100.0f,
				                    mShiningStone->mPosition.z);
			}

			SMSGetMarDirector()->fireStartDemoCamera(
			    "demohanatyan_cam01", nullptr, -1, 0.0f, true,
			    startCameraShakeSE, (u32)&mPosition, nullptr,
			    JDrama::TFlagT<u16>(0));
		} else {
			SMSGetMarDirector()->fireStartDemoCamera(
			    "太陽石点灯カメラ", &mShiningStone->mPosition, mDemoLightTime,
			    0.0f, true, nullptr, 0, nullptr, JDrama::TFlagT<u16>(0));
		}

		// Two seconds past the end of the lighting camera.
		startStateTimer(mDemoLightTime + 120);
		mState = STATE_LIGHT;
	}
}

// TODO: literal-pool order. The target asks for controlShake's 1.5708f
// (@4479) before controlGoTarget's 1100.0f (@4524), with an alignment hole
// in front of the 0.5/3.0 double pair that follows; ours emits the 1100.0f
// first and drops the two holes (65 objects against our 62). Order only.
void TLeanMirror::controlShake()
{
	if (enemyIsOn() && mBgmStarted && SMS_IsMarioTouchGround4cm()
	    && SMS_GetMarioGrPlane()->mActor != this) {
		// TODO: the argument order below is what the ROM passes; it disagrees
		// with the parameter names guessed in MSound/MSoundBGM.hpp.
		MSBgm::stopTrackBGM(1, 0xA);
		MSBgm::setTrackVolume(0, 1.0f, 0xA, 0);
		mBgmStarted = false;
	}

	if (!mSpeed.isZero()) {
		f32 down = mSpeedDownRate;
		mSpeed.x *= down;
		mSpeed.y *= down;
		mSpeed.z *= down;

		MtxPtr mtx = getModel()->getAnmMtx(0);

		JGeometry::TVec3<f32> axis(mSpeed.x, 0.0f, mSpeed.z);
		rotateVecByAxisY(&axis, 1.5707963f);

		// The ROM never stores mRotAxis/mRotSpeed here: the rotation step is
		// spelled out over locals instead of going through calcCurrentMtx,
		// which is why that UNUSED helper only shows up in controlGoTarget.
		f32 f31 = MsSqrtf(mSpeed.x * mSpeed.x + mSpeed.z * mSpeed.z)
		    * mSpeedRate;

		// TODO: retail `bl`s the weak JGeometry::SMatrix34C<f32> default
		// constructor for `rot` here and we expand it to nothing, so this
		// block still sits at the wrong inline depth; that and the 0x40
		// frame gap are all that is left (86.7 -> 91.3).
		JGeometry::TMatrix34<JGeometry::SMatrix34C<f32> > rot;
		rot.identity();
		makeMtxRotByAxis(axis, f31, rot);
		concatOnlyRotFromLeft(rot, mtx, mtx);

		if (getModel()->getAnmMtx(0)[1][1] < mLeanLimit) {
			MtxPtr now = getModel()->getAnmMtx(0);
			if (mSpeed.x * getModel()->getAnmMtx(0)[0][1]
			        + mSpeed.z * now[2][1]
			    > 0.0f) {
				gpMSound->startSoundActorWithInfo(
				    MSD_SE_OBJ_MA_MIRROR_IMPACT, &mPosition, nullptr,
				    fabsf(mSpeed.length()), 0, 0, nullptr, 0, 4);

				f32 rebound = -mReboundRate;
				mSpeed.x *= rebound;
				mSpeed.y *= rebound;
				mSpeed.z *= rebound;

				MTXCopy(getModel()->getBaseTRMtx(),
				        getModel()->getAnmMtx(0));
				return;
			}
		}

		MTXCopy(getModel()->getAnmMtx(0), getModel()->getBaseTRMtx());
	}
}

// TODO: 27.3%. Retail calls controlGoTarget out of line; ours still inlines
// it, because our calcCurrentMtx compiles to 0x84 where the map records
// 0x100, leaving controlGoTarget under MWCC's per-callee budget. The switch's
// comparison tree also pivots on 2 rather than retail's 3.
void TLeanMirror::control()
{
	TMapObjBase::control();

	switch (mState) {
	case STATE_SHAKE:
		controlShake();
		gpMSound->startSoundActorWithInfo(MSD_SE_OBJ_MA_MIRROR_MOVE,
		                                  &mPosition, nullptr,
		                                  fabsf(mSpeed.length()), 0, 0,
		                                  nullptr, 0, 4);
		break;

	case STATE_GO_TARGET:
		controlGoTarget();
		gpMSound->startSoundActorWithInfo(MSD_SE_OBJ_MA_MIRROR_DEMOMV,
		                                  &mPosition, nullptr,
		                                  fabsf(mSpeed.length()), 0, 0,
		                                  nullptr, 0, 4);
		break;

	case STATE_LIGHT:
		if (!isStateTimerEngaged()) {
			TShiningStone* stone = mShiningStone;
			if (stone->getLightNum() < 3) {
				MSBgm::setTrackVolume(0, 1.0f, 0xA, 0);
				if (stone->getLightNum() == 1)
					gpMarDirector->getConsole()->startAppearBalloon(0x32,
					                                                true);
				else
					gpMarDirector->getConsole()->startAppearBalloon(0x33,
					                                                true);
			}

			f32 scale = stone->mEmitterScale;
			if (scale > 0.0f)
				stone->mEmitter->setGlobalParticleScale(
				    JGeometry::TVec3<f32>(scale, scale, scale));

			mState = STATE_DONE;
		}
		break;
	}
}

void TLeanMirror::loadAfter()
{
	TMapObjBase::loadAfter();

	mShiningStone = (TShiningStone*)JDrama::TNameRefGen::search2("ShiningStone");

	// `a = b - c` reaches the map's out-of-line TVec3::sub: operator= is one
	// inline level and the difference nested in its argument two more (and
	// retail's re-read of mShiningStone is the assignment's own load).
	mToStone = mShiningStone->mPosition - mPosition;
	mToStone.setLength(1.0f);
}

u32 TLeanMirror::getSDLModelFlag() const { return 0; }

void TLeanMirror::initMapObj()
{
	TMapObjBase::initMapObj();
	mSpeedRate      = 0.03f;
	mSpeedDownRate  = 0.999f;
	mMarioPower     = 0.0001f;
	mWaterJetPower  = 1.0f;
	mEnemyPower     = 0.0002f;
	mFruitPower     = 0.0001f;
	mLeanLimit      = 0.865f;
	mReboundRate    = 0.5f;

	if (strcmp(unkF4, "mirrorS") == 0) {
		mWaterPower    = 0.002f;
		mWaterJetPower = 1.0f;
		mLeanLimit     = 0.87f;
		mHitNum        = 1;
	} else if (strcmp(unkF4, "mirrorM") == 0) {
		mWaterPower = 0.004f;
		mHitNum     = 2;
	} else {
		mWaterPower = 0.006f;
		mHitNum     = 3;
	}
}

void TLeanMirror::load(JSUMemoryInputStream& stream)
{
	TMapObjBase::load(stream);

	f32 size;
	stream >> size;
	mSize        = 100.0f * size * 0.5f;
	mDefaultSize = mSize;

	if (gpMarDirector->unk7D == 1) {
		char demoName[0x40];
		stream.readString(demoName, sizeof(demoName));
		stream >> mMarioMovePos.x;
		stream >> mMarioMovePos.y;
		stream >> mMarioMovePos.z;
	}

	TMirrorModelObj* mirror = new TMirrorModelObj;
	char path[0x40];
	snprintf(path, sizeof(path), "/scene/mapObj/%sTop.bmd", unkF4);
	mirror->init(path);
	mirror->unk28 = getModel();

	if (gpMarDirector->unk7D != 1)
		mState = STATE_DONE;
}

TLeanMirror::TLeanMirror(const char* name)
    : TMapObjBase(name)
{
	mSize          = 0.0f;
	mDefaultSize   = 0.0f;
	mSpeedRate     = 0.0f;
	mSpeedDownRate = 0.0f;
	mMarioPower    = 0.0f;
	mWaterPower    = 0.0f;
	mWaterJetPower = 0.0f;
	mEnemyPower    = 0.0f;
	mFruitPower    = 0.0f;
	mShiningStone  = nullptr;
	mRotSpeed      = 0.0f;
	mHitNum        = 0;
	mBgmStarted    = false;
	unk1AE         = 0;

	unk140.zero();
	mSpeed.zero();
	mToStone.zero();
	mRotAxis.zero();
	mMarioMovePos.zero();
}

// UNUSED: called when the lighting demo of the last mirror ends.
// TODO: 0x98 in the map; check the compiled size against that.
void TShiningStone::endDemo()
{
	// TODO: guessed body. The demo end has to stop the four mirror
	// animations; nothing in the TU pins the exact calls down.
	for (int i = 0; i < MIRROR_NUM; i++) {
		mMirrors[i]->getFrameCtrl(0)->setRate(0.0f);
		mMirrors[i]->getFrameCtrl(3)->setRate(0.0f);
	}
}

void TShiningStone::putOnLight(TLiveActor* mirror)
{
	if (strcmp(mirror->getName(), "mirrorS") == 0) {
		mMirrors[0]->setBck("shiningstonegreen");
		mMirrors[0]->setBrk("shiningstonegreen");
		mLitS = true;
	} else if (strcmp(mirror->getName(), "mirrorM") == 0) {
		mMirrors[1]->setBck("shiningstoneblue");
		mMirrors[1]->setBrk("shiningstoneblue");
		mLitM = true;
	} else if (strcmp(mirror->getName(), "mirrorL") == 0) {
		mMirrors[2]->setBck("shiningstonered");
		mMirrors[2]->setBrk("shiningstonered");
		mLitL = true;
	}

	switch (mLightNum) {
	case 0:
		mEmitter = gpMarioParticleManager->emit(
		    0x143, (JGeometry::TVec3<f32>*)&mPosition, 1, this);
		mEmitter->setRate(3.0f);
		mEmitterScale = 1.5f;
		gpMSound->startSoundActor(MSD_SE_DM_REFLECTION_1, &mPosition, 0,
		                          nullptr, 0, 4);
		break;

	case 1:
		mEmitter = gpMarioParticleManager->emit(
		    0x144, (JGeometry::TVec3<f32>*)&mPosition, 1, this);
		mEmitter->setRate(0.4f);
		mEmitterScale = 0.2f;
		gpMSound->startSoundActor(MSD_SE_DM_REFLECTION_2, &mPosition, 0,
		                          nullptr, 0, 4);
		break;

	case 2:
		mEmitter = gpMarioParticleManager->emit(
		    0x145, (JGeometry::TVec3<f32>*)&mPosition, 1, this);
		mEmitterScale = 0.0f;
		gpMSound->startSoundActor(MSD_SE_DM_REFLECTION_3, &mPosition, 0,
		                          nullptr, 0, 4);
		break;
	}

	gpMarioParticleManager->emit(0x56, (JGeometry::TVec3<f32>*)&mPosition, 0,
	                             nullptr);

	mLightNum++;
	if (mLightNum == 3) {
		mMirrors[3]->setBck("shiningstonewhite");
		mMirrors[3]->setBrk("shiningstonewhite");
		mLitAll = true;
	}
}

void TShiningStone::perform(u32 cue, JDrama::TGraphics* graphics)
{
	for (int i = 0; i < MIRROR_NUM; i++) {
		mMirrors[i]->perform(cue, graphics);

		if (mLightNum > 0)
			gpMarioParticleManager->emit(
			    0x143, (JGeometry::TVec3<f32>*)&mPosition, 1, this);
		if (mLightNum > 1)
			gpMarioParticleManager->emit(
			    0x144, (JGeometry::TVec3<f32>*)&mPosition, 1, this);
		if (mLightNum > 2)
			gpMarioParticleManager->emit(
			    0x145, (JGeometry::TVec3<f32>*)&mPosition, 1, this);
	}

	mTop->perform(cue, graphics);
}

void TShiningStone::load(JSUMemoryInputStream& stream)
{
	JDrama::TActor::load(stream);

	const char* bmdNames[MIRROR_NUM] = {
		"/scene/mapObj/ShiningStoneGreen.bmd",
		"/scene/mapObj/ShiningStoneBlue.bmd",
		"/scene/mapObj/ShiningStoneRed.bmd",
		"/scene/mapObj/ShiningStoneWhite.bmd",
	};

	Mtx mtx;
	MsMtxSetXYZRPH(mtx, mPosition.x, mPosition.y, mPosition.z, mRotation.x,
	               mRotation.y, mRotation.z);

	mMirrors = new MActor*[MIRROR_NUM];
	for (int i = 0; i < MIRROR_NUM; i++) {
		mMirrors[i] = SMS_MakeMActorWithAnmData(
		    bmdNames[i], gpMapObjManager->getMActorAnmData(), 3, 0x10020000);
		MTXCopy(mtx, mMirrors[i]->getModel()->getBaseTRMtx());

		TMirrorActor* mirrorActor = new TMirrorActor("太陽石in鏡");
		mirrorActor->init(mMirrors[i]->getModel(), 0x1A);
	}

	mTop = SMS_MakeMActorWithAnmData("/scene/mapObj/ShiningStone.bmd",
	                                 gpMapObjManager->getMActorAnmData(), 3,
	                                 0x10020000);
	mTop->setBpk("shiningstone");
	mTop->setBtk("shiningstone");
	MTXCopy(mtx, mTop->getModel()->getBaseTRMtx());

	SMS_LoadParticle("/scene/mapObj/ShiningStone1.jpa", 0x143);
	SMS_LoadParticle("/scene/mapObj/ShiningStone2.jpa", 0x144);
	SMS_LoadParticle("/scene/mapObj/ShiningStone3.jpa", 0x145);
	SMS_LoadParticle("/scene/mapObj/ShiningStoneF.jpa", 0x56);
}

TShiningStone::TShiningStone(const char* name)
    : THitActor(name)
{
	mLightNum     = 0;
	mEmitter      = nullptr;
	mEmitterScale = 0.0f;
	mLitS         = false;
	mLitM         = false;
	mLitL         = false;
	mLitAll       = false;
}

f32 TMammaBlockRotate::mRotSpeed       = 0.1f;
f32 TMammaBlockRotate::mRotReturnSpeed = 0.01f;
f32 TMammaBlockRotate::mRotEnd         = 130.0f;
f32 TMammaBlockRotate::mMapGoSpeed     = 1.0f;
f32 TMammaBlockRotate::mMapBackSpeed   = 0.1f;
int TMammaBlockRotate::mWaitTime       = 600;

u32 TMammaBlockRotate::touchWater(THitActor* actor)
{
	if (isState(STATE_WAIT)) {
		mRotation.y += mRotSpeed;
		if (mRotation.y > mRotEnd)
			mState = STATE_GOING;
	}

	return 1;
}

void TMammaBlockRotate::control()
{
	TMapObjBase::control();

	switch (mState) {
	case STATE_WAIT:
		if (mRotation.y > 0.0f)
			mRotation.y -= mRotReturnSpeed;
		else
			mRotation.y = 0.0f;
		break;

	case STATE_GOING: {
		moveJoint(mUpJointObj->getJoint(), 0.0f, -mMapGoSpeed, 0.0f);
		moveJoint(mDownJointObj->getJoint(), 0.0f, -mMapGoSpeed, 0.0f);
		mBuilding->getModel()->calc();

		J3DJoint* joint = mUpJointObj->getJoint();
		JGeometry::TVec3<f32> trans(0.0f, TMapObjBase::getJointTransY(joint),
		                            0.0f);
		mDownCollision->moveTrans(trans);
		trans.set(0.0f, TMapObjBase::getJointTransY(joint), 0.0f);
		mUpCollision->moveTrans(trans);

		if (TMapObjBase::getJointTransY(joint) < 0.0f) {
			mStateTimer = mWaitTime;
			mState      = STATE_GOAL_WAIT;
		}
		break;
	}

	case STATE_GOAL_WAIT:
		if (!isStateTimerEngaged())
			mState = STATE_BACKING;
		break;

	case STATE_BACKING: {
		moveJoint(mUpJointObj->getJoint(), 0.0f, mMapBackSpeed, 0.0f);
		moveJoint(mDownJointObj->getJoint(), 0.0f, mMapBackSpeed, 0.0f);

		J3DJoint* joint = mUpJointObj->getJoint();
		JGeometry::TVec3<f32> trans(0.0f, TMapObjBase::getJointTransY(joint),
		                            0.0f);
		mDownCollision->moveTrans(trans);
		trans.set(0.0f, TMapObjBase::getJointTransY(joint), 0.0f);
		mUpCollision->moveTrans(trans);

		mBuilding->getModel()->calc();

		if (TMapObjBase::getJointTransY(joint)
		    > mUpJointObj->getJoint()->getMax().y
		        - mUpJointObj->getJoint()->getMin().y)
			mState = STATE_WAIT;
		break;
	}
	}
}

void TMammaBlockRotate::initMapObj()
{
	TMapObjBase::initMapObj();

	mBuilding     = gpMap->getModelManager()->getJointModel(0);
	mDownJointObj = mBuilding->getChild(0)->getChild(0)->getChild(0)->getChild(
	    1);

	J3DJoint* downJoint = mDownJointObj->getJoint();
	f32 height          = downJoint->getMax().y - downJoint->getMin().y;
	moveJoint(downJoint, 0.0f, height, 0.0f);

	JGeometry::TVec3<f32> trans(0.0f, height, 0.0f);
	mDownCollision->setUp();
	mDownCollision->moveTrans(trans);

	mUpJointObj
	    = mBuilding->getChild(0)->getChild(0)->getChild(0)->getChild(2);

	J3DJoint* upJoint = mUpJointObj->getJoint();
	f32 upHeight      = upJoint->getMax().y - upJoint->getMin().y;
	moveJoint(upJoint, 0.0f, upJoint->getMax().y - upJoint->getMin().y, 0.0f);

	mUpCollision->setUp();
	trans.set(0.0f, upHeight, 0.0f);
	mUpCollision->moveTrans(trans);

	mBuilding->getModel()->calc();
}

void TMammaBlockRotate::load(JSUMemoryInputStream& stream)
{
	mDownCollision = new TMapCollisionMove;
	mDownCollision->init("/scene/mapObj/MammaBlockDown.col", 0, this);

	mUpCollision = new TMapCollisionMove;
	mUpCollision->init("/scene/mapObj/MammaBlockUp.col", 0, this);

	TMapObjBase::load(stream);
}

TMammaBlockRotate::TMammaBlockRotate(const char* name)
    : TMapObjBase(name)
{
	mDownJointObj  = nullptr;
	mUpJointObj    = nullptr;
	mDownCollision = nullptr;
	mUpCollision   = nullptr;
}

void TMammaYacht::control()
{
	TMapObjBase::control();

	if (mGroundPlane->isWaterSurface()) {
		mPosition.y = mInitialPosition.y
		    + gpMapObjWave->getWaveHeight(mPosition.x, mPosition.z);
		mFlag->mPosition.y = mPosition.y - 50.0f;
	}
}

void TMammaYacht::initMapObj()
{
	TMapObjBase::initMapObj();

	mFlag = new TMapObjFlag("旗");
	mFlag->mPosition.set(2.0f + mPosition.x, (1315.0f + mPosition.y) - 190.0f,
	                     mPosition.z - 15.0f);
	mFlag->mRotation.set(0.0f, 180.0f, 0.0f);
	mFlag->mScaling.set(1.0f, 2.5f, 3.8f);
	mFlag->init("MammaYacht00");
}

void TSandBird::control()
{
	TJointCoin::control();

	gpMSound->startSoundActor(MSD_SE_EN_SANDBIRD_CRY, &mPosition, 0, nullptr,
	                          0, 4);
	gpMSound->startSoundSystemSE(MSD_SE_ENV_SANDBIRD_WIND, 0, nullptr, 0);

	for (int i = 0; i < unk13C; i++) {
		TMapObjBase* obj = unk140[i];
		if (obj->isActorType(0x2000000E) || obj->isActorType(0x40000023)) {
			gpMarioParticleManager->emitAndBindToPosPtr(0x159, &obj->mPosition,
			                                            1, obj);
			gpMarioParticleManager->emitAndBindToPosPtr(
			    0x15A, &unk140[i]->mPosition, 1, unk140[i]);
		}
	}

	bool demo = true;
	if (!SMSGetCamera()->isSimpleDemoCamera() && SMSGetCamera()->mMode != 0x49)
		demo = false;

	if (!demo && !mHelpShown) {
		const TLiveActor* actor = (*gpMarioGroundPlane)->getActor();
		if (actor) {
			if (actor->isActorType(0x400002C9)) {
				gpMarDirector->getConsole()->startAppearBalloon(0x2C, false);
				mStateTimer = 2400;
				mHelpShown  = true;
			}
		}
	}

	if (!mHelpHidden && mHelpShown) {
		if (!isStateTimerEngaged()) {
			gpMarDirector->getConsole()->startDisappearBalloon(0x2C, false);
			mHelpHidden = true;
		}
	}
}

TMapObjBase* TSandBird::makeObjFromJointName(const char* name, u16 index)
{
	TMapObjBase* obj = TJointCoin::makeObjFromJointName(name, index);
	if (obj)
		return obj;

	if (!strstr(name, "none"))
		return makeObj("SandBirdBlock", index);

	return nullptr;
}

bool TSandBird::nameIsObj(const char* name)
{
	if (!strstr(name, "none"))
		return true;

	return false;
}

void TSandBird::initMapObj()
{
	TJointCoin::initMapObj();
	SMS_LoadParticle("/scene/map/map/ms_sunadori_a.jpa", 0x159);
	SMS_LoadParticle("/scene/map/map/ms_sunadori_b.jpa", 0x15A);
}

TSandBird::TSandBird(const char* name)
    : TJointCoin(name)
{
	mHelpShown  = false;
	mHelpHidden = false;
}

// UNUSED: never emitted, together with the rest of the class.
// TODO: 0xa4 in the map; check the compiled size against that.
void TWatermelon::control()
{
	TMapObjBase::control();

	if (mState == STATE_NORMAL) {
		mRotation.y += mVelocity.x;
		mRotation.x += mVelocity.z;
	}
}

u32 TWatermelonStatic::touchWater(THitActor* actor) { return 1; }

void TGoalWatermelon::touchActor(THitActor* actor)
{
	if (isState(STATE_NORMAL) && actor->isActorType(0x400000D0)) {
		mWatermelon = (TMapObjBase*)actor;
		mWatermelon->getMActor()->setBck("watermelon_shrink");
		mWatermelon->offMapObjFlag(MAP_OBJ_FLAG_UNK100);
		JGeometry::TVec3<f32> stop(0.0f, 0.0f, 0.0f);
		mWatermelon->mVelocity = stop;
		gpMarDirector->fireStartDemoCamera(
		    "スイカゴールカメラ", &mWatermelon->mPosition, -1, 0.0f, true,
		    nullptr, 0, nullptr, JDrama::TFlagT<u16>(0));
		mState = 2;
	}
}

void TGoalWatermelon::control()
{
	TMapObjBase::control();

	switch (mState) {
	case 1:
		break;
	case 2:
		if (mWatermelon->animIsFinished()) {
			gpItemManager->makeShineAppearWithDemoOffset(
			    "シャイン（お化けスイカ用）", "スイカシャインカメラ", 0.0f,
			    0.0f, 0.0f);
			mState = 3;
		}
		break;
	}
}

void TGoalWatermelon::loadAfter()
{
	TMapObjBase::loadAfter();
	mHitFlags |= 4;

	mShine = (TMapObjBase*)JDrama::TNameRefGen::search2(
	    "シャイン（お化けスイカ用）");
	TMapObjBase* shine = mShine;
	shine->mPosition.x = mShinePosition.x;
	shine->mPosition.y = mShinePosition.y;
	shine->mPosition.z = mShinePosition.z;
	mShine->appear();
}

void TGoalWatermelon::load(JSUMemoryInputStream& stream)
{
	TMapObjBase::load(stream);

	char shineName[0x20];
	stream.readString(shineName, sizeof(shineName));
	stream >> mShinePosition.x;
	stream >> mShinePosition.y;
	stream >> mShinePosition.z;
}

TGoalWatermelon::TGoalWatermelon(const char* name)
    : TMapObjBase(name)
{
	mShine      = nullptr;
	mWatermelon = nullptr;
	mShinePosition.zero();
}

// UNUSED: the two halves of perform's visibility update.
// TODO: 0x54 each in the map; check the compiled sizes against that.
void TMammaMirrorMapOperator::show(int i)
{
	if (mJointHidden[i]) {
		SMS_ShowJoint(mJoints[i]->getMesh(), true);
		mJointHidden[i] = false;
	}
}

void TMammaMirrorMapOperator::hide(int i)
{
	if (!mJointHidden[i]) {
		SMS_ShowJoint(mJoints[i]->getMesh(), false);
		mJointHidden[i] = true;
	}
}

void TMammaMirrorMapOperator::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (!(cue & 2))
		return;

	if (gpMirrorModelManager->isUnk18Present()) {
		TMirrorCamera* camera = gpMirrorModelManager->unk24;
		JGeometry::TVec3<f32> toMirror(
		    camera->unk98.x - mMirrorPos[gpMirrorModelManager->unk18].x,
		    camera->unk98.y - mMirrorPos[gpMirrorModelManager->unk18].y,
		    camera->unk98.z - mMirrorPos[gpMirrorModelManager->unk18].z);
		f32 mirrorDist = toMirror.length();

		for (int i = 0; i < MIRROR_JOINT_NUM; i++) {
			JGeometry::TVec3<f32> toJoint(camera->unk98.x - mJointCenter[i].x,
			                              camera->unk98.y - mJointCenter[i].y,
			                              camera->unk98.z
			                                  - mJointCenter[i].z);
			f32 jointDist = toJoint.length();

			if (jointDist > mJointRadius[i] || jointDist > mirrorDist)
				show(i);
			else
				hide(i);
		}

		return;
	}

	for (int i = 0; i < MIRROR_JOINT_NUM; i++)
		hide(i);
}

void TMammaMirrorMapOperator::loadAfter()
{
	mMirrorPos[0].set(
	    ((JDrama::TActor*)JDrama::TNameRefGen::search2("mirrorS"))->mPosition);
	mMirrorPos[1].set(
	    ((JDrama::TActor*)JDrama::TNameRefGen::search2("mirrorM"))->mPosition);
	mMirrorPos[2].set(
	    ((JDrama::TActor*)JDrama::TNameRefGen::search2("mirrorL"))->mPosition);

	J3DJoint* joint = ((TMapStaticObj*)JDrama::TNameRefGen::search2("鏡内地形"))
	                      ->getModelData()
	                      ->getJointNodePointer(2);

	for (int i = 0; i < MIRROR_JOINT_NUM; i++) {
		mJoints[i] = joint;
		mJointCenter[i].set(0.5f * (joint->getMax().x + joint->getMin().x),
		                    0.5f * (joint->getMax().y + joint->getMin().y),
		                    0.5f * (joint->getMax().z + joint->getMin().z));

		f32 sizeX = 0.5f * (joint->getMax().x - joint->getMin().x);
		f32 sizeZ = 0.5f * (joint->getMax().z - joint->getMin().z);
		if (sizeX > sizeZ)
			mJointRadius[i] = sizeX;
		else
			mJointRadius[i] = sizeZ;

		mJointRadius[i] += 2000.0f;
		if (mJointRadius[i] > 3000.0f)
			mJointRadius[i] = 3000.0f;

		joint = (J3DJoint*)joint->getYounger();
	}
}

TMammaMirrorMapOperator::TMammaMirrorMapOperator(const char* name)
    : JDrama::TViewObj(name)
{
	for (int i = 0; i < MIRROR_JOINT_NUM; i++) {
		mJoints[i] = nullptr;
		mJointCenter[i].zero();
		mJointRadius[i]  = 0.0f;
		mJointHidden[i] = false;
	}

	for (int i = 0; i < MIRROR_NUM; i++)
		mMirrorPos[i].zero();
}

u32 TSandEgg::getSDLModelFlag() const { return 0; }
