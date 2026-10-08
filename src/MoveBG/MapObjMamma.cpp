#include <JSystem/JAudio/JALibrary/JALSystem.hpp>
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

#include <MoveBG/MapObjMamma.hpp>
#include <Camera/Camera.hpp>
#include <Camera/CameraShake.hpp>
#include <Enemy/Beam.hpp>
#include <Enemy/SleepBossHanachan.hpp>
#include <GC2D/GCConsole2.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorUtil.hpp>
#include <MSound/MSound.hpp>
#include <MSound/SoundEffects.hpp>
#include <Map/Map.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <Map/MapCollisionManager.hpp>
#include <Map/MapData.hpp>
#include <Map/MapMirror.hpp>
#include <Map/MapModel.hpp>
#include <Map/MapStaticObject.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <MoveBG/ItemManager.hpp>
#include <MoveBG/MapObjFlag.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <MoveBG/MapObjWave.hpp>
#include <Player/MarioAccess.hpp>
#include <Strategic/MirrorActor.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/MarDirector.hpp>
#include <System/Particles.hpp>
#include <System/TargetArrow.hpp>
#include <Enemy/ChuuHana.hpp>
#include <math.h>
#include <stdio.h>
#include <string.h>

s32 TSandBase::mWitherTime               = 800;
f32 TSandBase::mScaleMin                 = 0.00001f;
f32 TSandBombBase::mFiringFrameSpeed     = 3.0f;
f32 TSandBombBase::mFiringFrameDownSpeed = 0.2f;
f32 TSandBombBase::mExplodeFrameSpeed    = 1.0f;
f32 TSandBombBase::mMarioJumpRate        = 0.12f;
s32 TSandBombBase::mExlodingRumbleTime   = 20;
f32 TSandCastle::mCollisionRate          = 1.7f;
s32 TLeanMirror::mGoTargetTime           = 600;
s32 TLeanMirror::mDemoWaitTime           = -1;
s32 TLeanMirror::mDemoLightTime          = 360;
f32 TMammaBlockRotate::mRotSpeed         = 0.1f;
f32 TMammaBlockRotate::mRotReturnSpeed   = 0.01f;
f32 TMammaBlockRotate::mRotEnd           = 130.0f;
f32 TMammaBlockRotate::mMapGoSpeed       = 1.0f;
f32 TMammaBlockRotate::mMapBackSpeed     = 0.1f;
s32 TMammaBlockRotate::mWaitTime         = 600;

// fabricated
static inline J3DFrameCtrl* getAnmFrameCtrl(TLiveActor* actor, int type)
{
	return actor->getMActor()->getFrameCtrl(type);
}

// fabricated
static inline void addAnmFrame(TLiveActor* actor, int type, f32 speed)
{
	getAnmFrameCtrl(actor, type)
	    ->setFrame(speed + getAnmFrameCtrl(actor, type)->getFrame());
}

u32 TSandLeaf::touchWater(THitActor*)
{
	unk138->grow();
	return 1;
}

void TSandLeaf::control()
{
	TMapObjBase::control();
	mGroundHeight = gpMap->checkGround(mPosition.x, mPosition.y + 200.0f,
	                                   mPosition.z, &mGroundPlane);
	mPosition.y   = mGroundHeight;
}

bool TSandBase::isDown() const { return false; }

bool TSandBase::withering()
{
	mScaling.y -= unk13C;
	if (mScaling.y < mScaleMin)
		mScaling.y = mScaleMin;

	SMSGetMSound()->startSoundActor(MSD_SE_OBJ_SANDBUD_NORMAL,
	                                &unk144->mPosition, 0, nullptr, 0, 4);
	if (mScaling.y <= mScaleMin)
		return true;
	return false;
}

TSandBase::TSandBase(const char* name)
    : TMapObjBase(name)
    , unk138(0.0f)
    , unk13C(0.0f)
    , unk144(nullptr)
{
}

void TSandLeafBase::grow()
{
	if (mState == STATE_NORMAL || mState == STATE_GROWN) {
		if (mScaling.y < 1.0f) {
			mScaling.y += unk138;
			if (mScaling.y > 1.0f)
				mScaling.y = 1.0f;

			if (mState == STATE_NORMAL) {
				mMapCollisionManager->changeCollision(1);
				mMapCollisionManager->setUpActiveCollisionTRS(
				    mPosition, mRotation, mScaling);
				unk144->startControlAnim(2);
				mState = STATE_GROWN;
			}

			addAnmFrame(unk144, 0, SMSGetAnmFrameRate());
			SMSRumbleMgr->start(0x15, 5, &mPosition);
			SMSGetMSound()->startSoundActor(MSD_SE_OBJ_SANDBUD_NORMAL,
			                                &unk144->mPosition, 0, nullptr, 0,
			                                4);
			mStateTimer = mWitherTime;
		}
	}
}

void TSandLeafBase::control()
{
	TMapObjBase::control();
	switch (mState) {
	case STATE_WITHERING:
		SMSRumbleMgr->start(0x13, -1, &mPosition);
		if (withering()) {
			SMSRumbleMgr->stop(0x13);
			mMapCollisionManager->changeCollision(0);
			mMapCollisionManager->setUpActiveCollisionTRS(mPosition, mRotation,
			                                              mScaling);
			mStateTimer = unk140;
			mState      = STATE_WITHERED;
		}
		break;
	case STATE_NORMAL:
		break;
	case STATE_WITHERED:
		if (!isStateTimerEngaged() && unk144->animIsFinished()) {
			unk144->awake();
			unk144->startAnim(1);
			SMSGetMSound()->startSoundActor(
			    MSD_SE_IT_COMMON_APPEAR, &unk144->mPosition, 0, nullptr, 0, 4);
			mState = STATE_SPROUTING;
		}
		break;
	case STATE_SPROUTING:
		if (unk144->animIsFinished()) {
			unk144->startAnim(0);
			mState = STATE_NORMAL;
		}
		break;
	}
}

void TSandLeafBase::initMapObj()
{
	unk138     = 0.003f;
	unk13C     = 0.001f;
	unk140     = 0;
	mScaling.y = mScaleMin;
	TMapObjBase::initMapObj();
	unk144 = TMapObjBaseManager::newAndRegisterObj(
	    "SandLeaf", mPosition, mRotation,
	    JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
	((TSandLeaf*)unk144)->unk138 = this;
	unk144->appear();
}

void TSandBomb::makeObjAppeared()
{
	TMapObjBase::makeObjAppeared();
	startControlAnim(1);
	startControlAnim(2);
}

u32 TSandBomb::touchWater(THitActor*)
{
	addAnmFrame(this, 0, TSandBombBase::mFiringFrameSpeed);
	addAnmFrame(this, 5, TSandBombBase::mFiringFrameSpeed);
	getMActor()->getFrameCtrl(0);
	soundBas(MSD_SE_OBJ_SANDBOMB_WATER_1, 7.0f,
	         TSandBombBase::mFiringFrameSpeed);
	soundBas(MSD_SE_OBJ_SANDBOMB_WATER_2, 50.0f,
	         TSandBombBase::mFiringFrameSpeed);
	soundBas(MSD_SE_OBJ_SANDBOMB_WATER_3, 100.0f,
	         TSandBombBase::mFiringFrameSpeed);
	soundBas(MSD_SE_OBJ_SANDBOMB_WATER_4, 150.0f,
	         TSandBombBase::mFiringFrameSpeed);
	if (getMActor()->curAnmEndsNext(0, nullptr)) {
		unk138->grow();
		startControlAnim(3);
		startControlAnim(4);
		startControlAnim(5);
		onHitFilter(HIT_FILTER_NO_COLLISION);
	}
	return 1;
}

u32 TSandBomb::getSDLModelFlag() const { return 0; }

void TSandBomb::initMapObj() { TMapObjBase::initMapObj(); }

void TSandBombBase::withered()
{
	mStateTimer = unk140;
	mState      = STATE_WITHERED;
	unk144->sleep();
}

void TSandBombBase::expanded()
{
	addAnmFrame(unk144, 0, unk150);
	SMSGetMSound()->startSoundActor(MSD_SE_OBJ_SAMDBOMB_REVERSE,
	                                &unk144->mPosition, 0, nullptr, 0, 4);
	if (unk144->animIsFinished())
		mState = STATE_WITHERING;
}

void TSandBombBase::exploding()
{
	addAnmFrame(this, 0, mExplodeFrameSpeed);
	addAnmFrame(unk144, 0, mExplodeFrameSpeed);

	f32 dist = getDistanceXZ(*gpMarioPos);
	if (!isFootHandOrStairs()
	    && getMActor()->getFrameCtrl(0)->getFrame() < 80.0f
	    && SMS_GetMarioGrLevel() > SMS_GetMarioPos().y - 30.0f
	    && dist < unk154) {
		SMS_SendMessageToMario(this, HIT_MESSAGE_THROWN);
		SMS_ThrowMario(JGeometry::TVec3<f32>(0.0f, 1.0f, 0.0f),
		               mMarioJumpRate * (unk154 - dist));
	}

	if (animIsFinished()) {
		unk144->startControlAnim(6);
		mState = STATE_EXPANDED;
	}
}

void TSandBombBase::explode()
{
	startControlAnim(1);
	mScaling.y = 1.0f;
	mMapCollisionManager->changeCollision(1);
	mMapCollisionManager->getActiveCollision()->setUp();
	mMapCollisionManager->moveActiveCollisionSRT(mPosition, mRotation,
	                                             mScaling);

	JPABaseEmitter* emitter
	    = gpMarioParticleManager->emit(MAPOBJ_SANDBOMB, &mPosition, 0, nullptr);
	emitter->setGlobalScale(JGeometry::TVec3<f32>(unk14C));

	if (!SMSGetMarDirector()->isDemoModeNow())
		gpCameraShake->startShake(CAM_SHAKE_MODE_UNKD, 1.0f);

	SMSGetMSound()->startSoundActor(MSD_SE_OBJ_SANDBOMB_BANG, &mPosition, 0,
	                                nullptr, 0, 4);
	SMSRumbleMgr->start(0x15, mExlodingRumbleTime, &mPosition);
	mState = STATE_EXPLODING;
}

void TSandBombBase::waitBeforeExplode()
{
	mState      = STATE_WAIT_BEFORE_EXPLODE;
	mStateTimer = unk148;
}

void TSandBombBase::grow() { mState = STATE_GROW; }

void TSandBombBase::control()
{
	TMapObjBase::control();
	TSandBomb* bomb = (TSandBomb*)unk144;
	switch (mState) {
	case STATE_NORMAL: {
		f32 frame = bomb->getMActor()->getFrameCtrl(0)->getFrame()
		            - mFiringFrameDownSpeed;
		if (frame >= 0.0f) {
			unk144->getMActor()->getFrameCtrl(0)->setFrame(frame);
			unk144->getMActor()->getFrameCtrl(5)->setFrame(frame);
		}
		break;
	}
	case STATE_GROW:
		addAnmFrame(bomb, 0, mExplodeFrameSpeed);
		addAnmFrame(unk144, 5, mExplodeFrameSpeed);
		addAnmFrame(unk144, 3, mExplodeFrameSpeed);
		if (unk144->animIsFinished())
			waitBeforeExplode();
		break;
	case STATE_WAIT_BEFORE_EXPLODE:
		if (!isStateTimerEngaged())
			explode();
		break;
	case STATE_EXPLODING:
		exploding();
		break;
	case STATE_EXPANDED:
		expanded();
		break;
	case STATE_WITHERING:
		SMSRumbleMgr->start(0x13, -1, &mPosition);
		if (withering()) {
			withered();
			SMSRumbleMgr->stop(0x13);
		}
		break;
	case STATE_WITHERED:
		if (!isStateTimerEngaged()) {
			mState = STATE_NORMAL;
			unk144->awake();
			unk144->startControlAnim(1);
			unk144->startControlAnim(2);
		}
		break;
	}

	if (unk144->getColNum() == 0)
		bomb->unk140 = 0;
}

TMapObjBase* TSandBombBase::findTriggerActor()
{
	return TMapObjBaseManager::newAndRegisterObj(
	    "SandBomb", mPosition, mRotation,
	    JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
}

void TSandBombBase::loadAfter()
{
	unk144                       = findTriggerActor();
	((TSandLeaf*)unk144)->unk138 = this;
	unk144->appear();
}

void TSandBombBase::initMapObj()
{
	unk138     = 0.006f;
	unk13C     = 0.005f;
	unk140     = 0;
	unk148     = 60;
	unk154     = 1000.0f;
	mScaling.y = mScaleMin;
	TMapObjBase::initMapObj();
	unk150 = 0.5f;
	if (strcmp(mIndividualName, "SandBombBasePyramid") == 0) {
		unk14C = 1.3f;
		unk154 = 1200.0f;
	} else if (strcmp(mIndividualName, "SandBombBaseShit") == 0) {
		unk14C = 1.3f;
		unk154 = 1500.0f;
	} else if (strcmp(mIndividualName, "SandBombBaseStar") == 0) {
		unk14C = 1.2f;
	} else if (strcmp(mIndividualName, "SandBombBaseTurtle") == 0) {
		unk14C = 1.2f;
	}
	SMS_LoadParticle("/scene/mapObj/SandBomb.jpa", MAPOBJ_SANDBOMB);
}

TSandBombBase::TSandBombBase(const char* name)
    : TSandBase(name)
    , unk148(0)
    , unk14C(1.0f)
    , unk150(0.0f)
    , unk154(0.0f)
{
}

bool TSandCastle::withering()
{
	addAnmFrame(this, 0, unk13C);
	addAnmFrame(this, 5, unk13C);

	f32 frame  = getMActor()->getFrameCtrl(0)->getFrame();
	f32 end    = getMActor()->getFrameCtrl(0)->getEnd();
	mScaling.y = mCollisionRate * ((end - frame) / end);
	if (frame > 240.0f) {
		if (!unk158->checkLiveFlag(LIVE_FLAG_DEAD)) {
			unk158->kill();
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
	TSandBombBase::expanded();
	if (unk144->animIsFinished()) {
		mState = STATE_WITHERING;
		startControlAnim(2);
		startControlAnim(3);
	}
}

void TSandCastle::explode()
{
	startControlAnim(1);
	mScaling.y = 1.0f;
	mMapCollisionManager->changeCollision(1);
	mMapCollisionManager->getActiveCollision()->setUp();
	mMapCollisionManager->moveActiveCollisionSRT(mPosition, mRotation,
	                                             mScaling);

	JPABaseEmitter* emitter
	    = gpMarioParticleManager->emit(MAPOBJ_SANDBOMB, &mPosition, 0, nullptr);
	emitter->setGlobalScale(JGeometry::TVec3<f32>(unk14C));

	if (!SMSGetMarDirector()->isDemoModeNow())
		gpCameraShake->startShake(CAM_SHAKE_MODE_UNKD, 1.0f);

	SMSGetMSound()->startSoundActor(MSD_SE_OBJ_SANDBOMB_BANG, &mPosition, 0,
	                                nullptr, 0, 4);
	SMSRumbleMgr->start(0x15, mExlodingRumbleTime, &mPosition);
	mState = STATE_EXPLODING;
	awake();
	unk158->appear();
	startControlAnim(3);
}

static s32 SandCastleCallBack(uintptr_t, u32 param_2)
{
	if (param_2 == 1) {
		gpTargetArrow->unk14 = 1;
		gpTargetArrow->setPos(JGeometry::TVec3<f32>(8400.0f, 300.0f, 8150.0f));
		const JGeometry::TVec3<f32>& pos = gpCamera->unk124;
		const JGeometry::TVec3<f32>& at  = gpCamera->unk148;
		s16 yaw                          = matan(pos.z - at.z, pos.x - at.x);
		gpCamera->warpPosAndAt(gpCamera->mCurrentTarget.unk28, yaw);
	}
	return 1;
}

void TSandCastle::waitBeforeExplode()
{
	TSandBombBase::waitBeforeExplode();
	SMSGetMarDirector()->fireStartDemoCamera("mamma1_sandcastle", nullptr, -1,
	                                         0.0f, true, SandCastleCallBack, 0,
	                                         nullptr, JDrama::TFlagT<u16>(0));
	unk15C = 1;
}

void TSandCastle::calcRootMatrix()
{
	if (!isState(STATE_WITHERING))
		TMapObjBase::calcRootMatrix();
}

TMapObjBase* TSandCastle::findTriggerActor()
{
	return (TMapObjBase*)JDrama::TNameRefGen::search("砂の城爆発の芽");
}

void TSandCastle::loadAfter()
{
	TSandBombBase::loadAfter();
	unk158
	    = (TMapObjBase*)JDrama::TNameRefGen::search("ステージ切替（砂の城）");
	unk158->makeObjDead();
}

void TSandCastle::initMapObj()
{
	TSandBombBase::initMapObj();
	unk13C = 0.11f;
	unk148 = 120;
	sleep();
}

TSandCastle::TSandCastle(const char* name)
    : TSandBombBase(name)
    , unk158(nullptr)
    , unk15C(0)
{
}

bool TLeanMirror::enemyIsOn() const { return unk19C != 0 ? true : false; }

void TLeanMirror::draw() const
{
	MtxPtr mtx = getModel()->getAnmMtx(0);
	JGeometry::TVec3<f32> dir;
	dir.x = mtx[0][1];
	dir.y = mtx[1][1];
	dir.z = mtx[2][1];

	JGeometry::TVec3<f32> start = dir;
	start.scale(0.001f * (350.0f * mBodyRadius));
	start.add(mPosition);

	JGeometry::TVec3<f32> end = dir;
	end.scale(10000.0f);
	end.add(mPosition);

	gpBeamManager->requestCone(end, start, 1.7f * mBodyRadius, true, true,
	                           false);
}

void TLeanMirror::updateSpeedVec(const JGeometry::TVec3<f32>& pos, f32 rate)
{
	MtxPtr mtx = getModel()->getAnmMtx(0);
	f32 dx     = (pos.x - mPosition.x) / fabsf(unk138 * mtx[0][0]);
	f32 dz     = (pos.z - mPosition.z) / fabsf(unk138 * mtx[2][2]);
	unk14C.x += rate * (dx - mtx[0][1]);
	unk14C.z += rate * (dz - mtx[2][1]);
}

BOOL TLeanMirror::receiveMessage(THitActor* sender, u32 message)
{
	if (message == HIT_MESSAGE_TRAMPLE) {
		sendMsg(ACTOR_TYPE_CHUU_HANA, message);
		updateSpeedVec(sender->mPosition, unk164);
		return true;
	}

	if (message == HIT_MESSAGE_HIP_DROP) {
		sendMsg(ACTOR_TYPE_CHUU_HANA, message);
		updateSpeedVec(sender->mPosition, unk168);
		return true;
	}

	if (message == HIT_MESSAGE_SUPER_HIP_DROP) {
		updateSpeedVec(sender->mPosition, unk170);
		return true;
	}

	if (message == HIT_MESSAGE_DETACH) {
		--unk19C;
		if (unk19C == 0)
			release();
		return true;
	}

	return false;
}

void TLeanMirror::touchPlayer(THitActor* player)
{
	if (isState(STATE_NORMAL) && marioIsOn()) {
		updateSpeedVec(player->mPosition, unk160);
		if (!unk1AC) {
			MSBgm::startBGM(MSD_BGM_EXTRA);
			MSBgm::setTrackVolume(0, 0.0f, 10, 0);
			unk1AC = 1;
		}
	}
}

void TLeanMirror::touchEnemy(THitActor* enemy)
{
	if (enemy->isActorType(ACTOR_TYPE_CHUU_HANA) && ((TChuuHana*)enemy)->unk1B0)
		updateSpeedVec(enemy->mPosition, unk16C);
}

void TLeanMirror::calcCurrentMtx(MtxPtr mtx)
{
	JGeometry::TVec3<f32> axis(unk14C.x, 0.0f, unk14C.z);
	rotateVecByAxisY(&axis, 1.5707964f);
	f32 len   = MsSqrtf(unk14C.x * unk14C.x + unk14C.z * unk14C.z);
	f32 speed = len * unk158;
	TPosition3f rot;
	rot.identity();
	makeMtxRotByAxis(axis, speed, rot);
	concatOnlyRotFromLeft(rot, mtx, mtx);
}

void TLeanMirror::release()
{
	MtxPtr mtx = getModel()->getAnmMtx(0);
	JGeometry::TVec3<f32> up;
	up.x = mtx[0][1];
	up.y = mtx[1][1];
	up.z = mtx[2][1];
	unk18C.cross(up, unk180);
	unk198      = MsAngleBetween(up, unk180) / mGoTargetTime;
	mStateTimer = mGoTargetTime;
	mState      = STATE_GO_TARGET;
	offMapObjFlag(MAP_OBJ_FLAG_MOVE_COLLISION_ON_CONTACT);
	SMS_MarioMoveRequest(unk1A0);

	if (strcmp(mIndividualName, "mirrorS") == 0) {
		SMSGetMarDirector()->fireStartDemoCamera(
		    "ぐらぐら鏡Ｓカメラ", &unk17C->mPosition,
		    mGoTargetTime + mDemoWaitTime, 0.0f, true, nullptr, 0, nullptr,
		    JDrama::TFlagT<u16>(0));
	} else if (strcmp(mIndividualName, "mirrorM") == 0) {
		SMSGetMarDirector()->fireStartDemoCamera(
		    "ぐらぐら鏡Ｍカメラ", &unk17C->mPosition,
		    mGoTargetTime + mDemoWaitTime, 0.0f, true, nullptr, 0, nullptr,
		    JDrama::TFlagT<u16>(0));
	} else if (strcmp(mIndividualName, "mirrorL") == 0) {
		SMSGetMarDirector()->fireStartDemoCamera(
		    "ぐらぐら鏡Ｌカメラ", &unk17C->mPosition,
		    mGoTargetTime + mDemoWaitTime, 0.0f, true, nullptr, 0, nullptr,
		    JDrama::TFlagT<u16>(0));
	}
	MSBgm::stopTrackBGM(1, 10);
}

static s32 startCameraShakeSE(uintptr_t param_1, u32 param_2)
{
	if (param_2 == 0) {
		const JGeometry::TVec3<f32>* pos
		    = (const JGeometry::TVec3<f32>*)param_1;
		SMSGetMSound()->startSoundActor(MSD_SE_OBJ_QUAKE, pos, 0, nullptr, 0,
		                                4);
	}
	return 0;
}

void TLeanMirror::controlGoTarget()
{
	MtxPtr mtx = getModel()->getAnmMtx(0);
	TRotation3f rot;
	rot.identity();
	makeMtxRotByAxis(unk18C, unk198, rot);
	concatOnlyRotFromLeft(rot, mtx, mtx);
	if (!isStateTimerEngaged()) {
		unk17C->putOnLight(this);
		if (unk17C->unk73) {
			TSleepBossHanachan* hanachan
			    = (TSleepBossHanachan*)JDrama::TNameRefGen::search(
			        "居眠りボスハナチャン");
			if (hanachan)
				hanachan->startFall(unk17C->mPosition.x,
				                    unk17C->mPosition.y + 1100.0f,
				                    unk17C->mPosition.z);
			SMSGetMarDirector()->fireStartDemoCamera(
			    "demohanatyan_cam01", nullptr, -1, 0.0f, true,
			    startCameraShakeSE, (uintptr_t)&mPosition, nullptr,
			    JDrama::TFlagT<u16>(0));
		} else {
			SMSGetMarDirector()->fireStartDemoCamera(
			    "太陽石点灯カメラ", &unk17C->mPosition, mDemoLightTime, 0.0f,
			    true, nullptr, 0, nullptr, JDrama::TFlagT<u16>(0));
		}
		mStateTimer = mDemoLightTime;
		mState      = STATE_LIGHT_DEMO;
	}
}

void TLeanMirror::controlShake()
{
	if (enemyIsOn() && unk1AC && SMS_IsMarioTouchGround4cm()
	    && SMS_GetMarioGrPlane()->getActor() != this) {
		MSBgm::stopTrackBGM(1, 10);
		MSBgm::setTrackVolume(0, 1.0f, 10, 0);
		unk1AC = 0;
	}

	if (!unk14C.isZero()) {
		unk14C.scale(unk15C);
		calcCurrentMtx(getModel()->getAnmMtx(0));
		if (getModel()->getAnmMtx(0)[1][1] < unk174) {
			if (unk14C.x * getModel()->getAnmMtx(0)[0][1]
			        + unk14C.z * getModel()->getAnmMtx(0)[2][1]
			    > 0.0f) {
				SMSGetMSound()->startSoundActorWithInfo(
				    MSD_SE_OBJ_MA_MIRROR_IMPACT, &mPosition, nullptr,
				    fabsf(unk14C.length()), 0, 0, nullptr, 0, 4);
				unk14C.scale(-unk178);
				MtxPtr anm = getModel()->getAnmMtx(0);
				MTXCopy(getModel()->getBaseTRMtx(), anm);
				return;
			}
		}
		MtxPtr anm = getModel()->getAnmMtx(0);
		getModel()->setBaseTRMtx(anm);
	}
}

void TLeanMirror::control()
{
	TMapObjBase::control();
	switch (mState) {
	case STATE_NORMAL:
		controlShake();
		SMSGetMSound()->startSoundActorWithInfo(
		    MSD_SE_OBJ_MA_MIRROR_MOVE, &mPosition, nullptr,
		    fabsf(unk14C.length()), 0, 0, nullptr, 0, 4);
		break;
	case STATE_GO_TARGET:
		controlGoTarget();
		SMSGetMSound()->startSoundActorWithInfo(
		    MSD_SE_OBJ_MA_MIRROR_DEMOMV, &mPosition, nullptr,
		    fabsf(unk14C.length()), 0, 0, nullptr, 0, 4);
		break;
	case STATE_LIGHT_DEMO:
		if (!isStateTimerEngaged()) {
			unk17C->endDemo();
			mState = STATE_DONE;
		}
		break;
	case STATE_DONE:
		break;
	}
}

void TLeanMirror::loadAfter()
{
	TMapObjBase::loadAfter();
	unk17C = (TShiningStone*)JDrama::TNameRefGen::search("ShiningStone");
	unk180 = unk17C->mPosition - mPosition;
	unk180.normalize();
}

u32 TLeanMirror::getSDLModelFlag() const { return 0; }

void TLeanMirror::initMapObj()
{
	TMapObjBase::initMapObj();
	unk158 = 0.03f;
	unk15C = 0.999f;
	unk160 = 0.0001f;
	unk168 = 1.0f;
	unk16C = 0.0002f;
	unk170 = 0.0001f;
	unk174 = 0.865f;
	unk178 = 0.5f;
	if (strcmp(mIndividualName, "mirrorS") == 0) {
		unk164 = 0.002f;
		unk168 = 1.0f;
		unk174 = 0.87f;
		unk19C = 1;
	} else if (strcmp(mIndividualName, "mirrorM") == 0) {
		unk164 = 0.004f;
		unk19C = 2;
	} else {
		unk164 = 0.006f;
		unk19C = 3;
	}
}

void TLeanMirror::load(JSUMemoryInputStream& stream)
{
	TMapObjBase::load(stream);
	f32 size;
	stream >> size;
	unk138 = 100.0f * size / 2.0f;
	unk13C = unk138;
	if (gpMarDirector->unk7D == 1) {
		char buffer[0x40];
		stream.readString(buffer, sizeof(buffer));
		stream >> unk1A0.x >> unk1A0.y >> unk1A0.z;
	}

	TMirrorModelObj* mirror = new TMirrorModelObj;
	char path[0x40];
	snprintf(path, sizeof(path), "/scene/mapObj/%sTop.bmd", mIndividualName);
	mirror->init(path);
	mirror->unk28 = getModel();
	if (gpMarDirector->unk7D != 1)
		mState = STATE_DONE;
}

TLeanMirror::TLeanMirror(const char* name)
    : TMapObjBase(name)
    , unk138(0.0f)
    , unk13C(0.0f)
    , unk158(0.0f)
    , unk15C(0.0f)
    , unk160(0.0f)
    , unk164(0.0f)
    , unk168(0.0f)
    , unk16C(0.0f)
    , unk170(0.0f)
    , unk17C(nullptr)
    , unk198(0.0f)
    , unk19C(0)
    , unk1AC(0)
    , unk1AE(0)
{
	unk140.zero();
	unk14C.zero();
	unk180.zero();
	unk18C.zero();
	unk1A0.zero();
}

void TShiningStone::endDemo()
{
	if (unk74 < 3)
		MSBgm::setTrackVolume(0, 1.0f, 10, 0);
	if (unk7C > 0.0f)
		unk78->setRate(unk7C);
}

void TShiningStone::putOnLight(TLiveActor* mirror)
{
	if (strcmp(mirror->getName(), "mirrorS") == 0) {
		unk68[0]->setBck("shiningstonegreen");
		unk68[0]->setBrk("shiningstonegreen");
		unk70 = 1;
	} else if (strcmp(mirror->getName(), "mirrorM") == 0) {
		unk68[1]->setBck("shiningstoneblue");
		unk68[1]->setBrk("shiningstoneblue");
		unk71 = 1;
	} else if (strcmp(mirror->getName(), "mirrorL") == 0) {
		unk68[2]->setBck("shiningstonered");
		unk68[2]->setBrk("shiningstonered");
		unk72 = 1;
	}

	switch (unk74) {
	case 0:
		unk78 = gpMarioParticleManager->emit(MAPOBJ_SHININGSTONE1, &mPosition,
		                                     1, this);
		unk78->setRate(3.0f);
		unk7C = 1.5f;
		SMSGetMSound()->startSoundActor(MSD_SE_DM_REFLECTION_1, &mPosition, 0,
		                                nullptr, 0, 4);
		break;
	case 1:
		unk78 = gpMarioParticleManager->emit(MAPOBJ_SHININGSTONE2, &mPosition,
		                                     1, this);
		unk78->setRate(0.4f);
		unk7C = 0.2f;
		SMSGetMSound()->startSoundActor(MSD_SE_DM_REFLECTION_2, &mPosition, 0,
		                                nullptr, 0, 4);
		break;
	case 2:
		unk78 = gpMarioParticleManager->emit(MAPOBJ_SHININGSTONE3, &mPosition,
		                                     1, this);
		unk7C = 0.0f;
		SMSGetMSound()->startSoundActor(MSD_SE_DM_REFLECTION_3, &mPosition, 0,
		                                nullptr, 0, 4);
		break;
	}

	gpMarioParticleManager->emit(MAPOBJ_SHININGSTONEF, &mPosition, 0, nullptr);
	++unk74;
	if (unk74 == 3) {
		unk68[3]->setBck("shiningstonewhite");
		unk68[3]->setBrk("shiningstonewhite");
		unk73 = 1;
	}
}

void TShiningStone::perform(u32 cue, JDrama::TGraphics* graphics)
{
	for (int i = 0; i < 4; ++i) {
		unk68[i]->perform(cue, graphics);
		if (unk74 > 0)
			gpMarioParticleManager->emit(MAPOBJ_SHININGSTONE1, &mPosition, 1,
			                             this);
		if (unk74 > 1)
			gpMarioParticleManager->emit(MAPOBJ_SHININGSTONE2, &mPosition, 1,
			                             this);
		if (unk74 > 2)
			gpMarioParticleManager->emit(MAPOBJ_SHININGSTONE3, &mPosition, 1,
			                             this);
	}
	unk6C->perform(cue, graphics);
}

void TShiningStone::load(JSUMemoryInputStream& stream)
{
	JDrama::TActor::load(stream);
	const char* names[] = {
		"/scene/mapObj/ShiningStoneGreen.bmd",
		"/scene/mapObj/ShiningStoneBlue.bmd",
		"/scene/mapObj/ShiningStoneRed.bmd",
		"/scene/mapObj/ShiningStoneWhite.bmd",
	};
	TPosition3f mtx;
	MsMtxSetXYZRPH(mtx, mPosition.x, mPosition.y, mPosition.z, mRotation.x,
	               mRotation.y, mRotation.z);
	unk68 = new MActor*[4];
	for (int i = 0; i < 4; ++i) {
		unk68[i] = SMS_MakeMActorWithAnmData(
		    names[i], gpMapObjManager->getMActorAnmData(), 3, 0x10020000);
		unk68[i]->getModel()->setBaseTRMtx(mtx);
		TMirrorActor* mirror = new TMirrorActor("太陽石in鏡");
		mirror->init(unk68[i]->getModel(), 0x1a);
	}
	unk6C = SMS_MakeMActorWithAnmData("/scene/mapObj/ShiningStone.bmd",
	                                  gpMapObjManager->getMActorAnmData(), 3,
	                                  0x10020000);
	unk6C->setBpk("shiningstone");
	unk6C->setBtk("shiningstone");
	unk6C->getModel()->setBaseTRMtx(mtx);
	SMS_LoadParticle("/scene/mapObj/ShiningStone1.jpa", MAPOBJ_SHININGSTONE1);
	SMS_LoadParticle("/scene/mapObj/ShiningStone2.jpa", MAPOBJ_SHININGSTONE2);
	SMS_LoadParticle("/scene/mapObj/ShiningStone3.jpa", MAPOBJ_SHININGSTONE3);
	SMS_LoadParticle("/scene/mapObj/ShiningStoneF.jpa", MAPOBJ_SHININGSTONEF);
}

TShiningStone::TShiningStone(const char* name)
    : THitActor(name)
    , unk74(0)
    , unk78(nullptr)
    , unk7C(0.0f)
{
	unk70 = 0;
	unk71 = 0;
	unk72 = 0;
	unk73 = 0;
}

u32 TMammaBlockRotate::touchWater(THitActor*)
{
	if (isState(STATE_NORMAL)) {
		mRotation.y += mRotSpeed;
		if (mRotation.y > mRotEnd)
			mState = STATE_MAP_GO;
	}
	return 1;
}

void TMammaBlockRotate::control()
{
	TMapObjBase::control();
	JGeometry::TVec3<f32> trans;
	switch (mState) {
	case STATE_NORMAL:
		if (mRotation.y > 0.0f)
			mRotation.y -= mRotReturnSpeed;
		else
			mRotation.y = 0.0f;
		break;
	case STATE_MAP_GO: {
		moveJoint(unk140->getJoint(), 0.0f, -mMapGoSpeed, 0.0f);
		moveJoint(unk13C->getJoint(), 0.0f, -mMapGoSpeed, 0.0f);
		J3DTransformInfo& info = unk140->getJoint()->getTransformInfo();
		unk138->getModel()->calc();
		trans.set(0.0f, info.mTranslate.y, 0.0f);
		unk144->moveTrans(trans);
		trans.set(0.0f, info.mTranslate.y, 0.0f);
		unk148->moveTrans(trans);
		if (info.mTranslate.y < 0.0f) {
			mStateTimer = mWaitTime;
			mState      = STATE_MAP_WAIT;
		}
		break;
	}
	case STATE_MAP_WAIT:
		if (!isStateTimerEngaged())
			mState = STATE_MAP_BACK;
		break;
	case STATE_MAP_BACK: {
		moveJoint(unk140->getJoint(), 0.0f, mMapBackSpeed, 0.0f);
		moveJoint(unk13C->getJoint(), 0.0f, mMapBackSpeed, 0.0f);
		J3DTransformInfo& info = unk140->getJoint()->getTransformInfo();
		trans.set(0.0f, info.mTranslate.y, 0.0f);
		unk144->moveTrans(trans);
		trans.set(0.0f, info.mTranslate.y, 0.0f);
		unk148->moveTrans(trans);
		unk138->getModel()->calc();
		if (info.mTranslate.y
		    > unk140->getJoint()->getMax().y - unk140->getJoint()->getMin().y)
			mState = STATE_NORMAL;
		break;
	}
	}
}

void TMammaBlockRotate::initMapObj()
{
	TMapObjBase::initMapObj();
	unk138 = gpMap->getRootJointModel();
	unk13C = unk138->getChild(0)->getChild(0)->getChild(0)->getChild(1);
	J3DJoint* joint = unk13C->getJoint();
	f32 height      = joint->getMax().y - joint->getMin().y;
	moveJoint(joint, 0.0f, height, 0.0f);
	JGeometry::TVec3<f32> trans(0.0f, height, 0.0f);
	unk144->setUp();
	unk144->moveTrans(trans);
	unk140 = unk138->getChild(0)->getChild(0)->getChild(0)->getChild(2);
	joint  = unk140->getJoint();
	height = joint->getMax().y - joint->getMin().y;
	moveJoint(joint, 0.0f, joint->getMax().y - joint->getMin().y, 0.0f);
	unk148->setUp();
	trans.set(0.0f, height, 0.0f);
	unk148->moveTrans(trans);
	unk138->getModel()->calc();
}

void TMammaBlockRotate::load(JSUMemoryInputStream& stream)
{
	unk144 = new TMapCollisionMove;
	unk144->init("/scene/mapObj/MammaBlockDown.col", 0, this);
	unk148 = new TMapCollisionMove;
	unk148->init("/scene/mapObj/MammaBlockUp.col", 0, this);
	TMapObjBase::load(stream);
}

TMammaBlockRotate::TMammaBlockRotate(const char* name)
    : TMapObjBase(name)
    , unk13C(nullptr)
    , unk140(nullptr)
    , unk144(nullptr)
    , unk148(nullptr)
{
}

void TMammaYacht::control()
{
	TMapObjBase::control();
	if (mGroundPlane->isWaterSurface()) {
		mPosition.y = mInitialPosition.y
		              + gpMapObjWave->getWaveHeight(mPosition.x, mPosition.z);
		unk138->mPosition.y = mPosition.y - 50.0f;
	}
}

void TMammaYacht::initMapObj()
{
	TMapObjBase::initMapObj();
	unk138 = new TMapObjFlag;
	unk138->mPosition.set(mPosition.x + 2.0f, (mPosition.y + 1315.0f) - 190.0f,
	                      mPosition.z - 15.0f);
	unk138->mRotation.set(0.0f, 180.0f, 0.0f);
	unk138->mScaling.set(1.0f, 2.5f, 3.8f);
	unk138->init("MammaYacht00");
}

void TSandBird::control()
{
	TJointCoin::control();
	SMSGetMSound()->startSoundActor(MSD_SE_EN_SANDBIRD_CRY, &mPosition, 0,
	                                nullptr, 0, 4);
	SMSGetMSound()->startSoundSystemSE(MSD_SE_ENV_SANDBIRD_WIND, 0, nullptr, 0);

	for (int i = 0; i < unk13C; ++i) {
		if (unk140[i]->isActorType(ACTOR_TYPE_COIN)
		    || unk140[i]->isActorType(ACTOR_TYPE_NO_DATA)) {
			gpMarioParticleManager->emitAndBindToPosPtr(
			    MAP_MAP_MS_SUNADORI_A, &unk140[i]->mPosition, 1, unk140[i]);
			gpMarioParticleManager->emitAndBindToPosPtr(
			    MAP_MAP_MS_SUNADORI_B, &unk140[i]->mPosition, 1, unk140[i]);
		}
	}

	if (!gpCamera->isDemoCamera() && !unk150) {
		const TBGCheckData* plane = SMS_GetMarioGroundPlane();
		if (plane->getActor() != nullptr
		    && plane->getActor()->isActorType(ACTOR_TYPE_SAND_BIRD_BLOCK)) {
			gpMarDirector->mConsole->startAppearBalloon(0xE002F, false);
			mStateTimer = 2400;
			unk150      = 1;
		}
	}

	if (!unk151 && unk150 && !isStateTimerEngaged()) {
		gpMarDirector->mConsole->startDisappearBalloon(0xE002F, false);
		unk151 = 1;
	}
}

TMapObjBase* TSandBird::makeObjFromJointName(const char* name, u16 index)
{
	TMapObjBase* obj = TJointCoin::makeObjFromJointName(name, index);
	if (obj != nullptr)
		return obj;
	if (strstr(name, "none") == nullptr)
		return makeObj("SandBirdBlock", index);
	return nullptr;
}

bool TSandBird::nameIsObj(const char* name)
{
	if (strstr(name, "none") == nullptr)
		return true;
	return false;
}

void TSandBird::initMapObj()
{
	TJointCoin::initMapObj();
	SMS_LoadParticle("/scene/map/map/ms_sunadori_a.jpa", MAP_MAP_MS_SUNADORI_A);
	SMS_LoadParticle("/scene/map/map/ms_sunadori_b.jpa", MAP_MAP_MS_SUNADORI_B);
}

TSandBird::TSandBird(const char* name)
    : TJointCoin(name)
    , unk150(0)
    , unk151(0)
{
}

void TWatermelon::control() { }

void TGoalWatermelon::touchActor(THitActor* actor)
{
	if (isState(STATE_NORMAL) && actor->isActorType(ACTOR_TYPE_WATERMELON)) {
		unk13C = (TMapObjBase*)actor;
		unk13C->getMActor()->setBck("watermelon_shrink");
		unk13C->offMapObjFlag(MAP_OBJ_FLAG_NO_ANIMATIONS);
		unk13C->mVelocity = JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f);
		SMSGetMarDirector()->fireStartDemoCamera(
		    "スイカゴールカメラ", &unk13C->mPosition, -1, 0.0f, true, nullptr,
		    0, nullptr, JDrama::TFlagT<u16>(0));
		mState = STATE_SHRINK;
	}
}

void TGoalWatermelon::control()
{
	TMapObjBase::control();
	switch (mState) {
	case STATE_NORMAL:
		break;
	case STATE_SHRINK:
		if (unk13C->animIsFinished()) {
			gpItemManager->makeShineAppearWithDemoOffset(
			    "シャイン（お化けスイカ用）", "スイカシャインカメラ", 0.0f,
			    0.0f, 0.0f);
			mState = STATE_SHINE;
		}
		break;
	case STATE_SHINE:
		break;
	}
}

void TGoalWatermelon::loadAfter()
{
	TMapObjBase::loadAfter();
	onHitFilter(HIT_FILTER_NO_DAMAGE);
	unk138 = (TMapObjBase*)JDrama::TNameRefGen::search(
	    "シャイン（お化けスイカ用）");
	unk138->mPosition.set(unk140);
	unk138->appear();
}

void TGoalWatermelon::load(JSUMemoryInputStream& stream)
{
	TMapObjBase::load(stream);
	char buffer[0x20];
	stream.readString(buffer, sizeof(buffer));
	stream >> unk140.x >> unk140.y >> unk140.z;
}

TGoalWatermelon::TGoalWatermelon(const char* name)
    : TMapObjBase(name)
    , unk138(nullptr)
    , unk13C(nullptr)
{
	unk140.zero();
}

void TMammaMirrorMapOperator::show(int i)
{
	if (unkB0[i]) {
		SMS_ShowJoint(unk10[i]->getMesh(), true);
		unkB0[i] = 0;
	}
}

void TMammaMirrorMapOperator::hide(int i)
{
	if (!unkB0[i]) {
		SMS_ShowJoint(unk10[i]->getMesh(), false);
		unkB0[i] = 1;
	}
}

void TMammaMirrorMapOperator::perform(u32 cue, JDrama::TGraphics*)
{
	if (cue & 2) {
		const int& mirrorIdx = gpMirrorModelManager->unk18;
		if (gpMirrorModelManager->isUnk18Present()) {
			const JGeometry::TVec3<f32>& camPos
			    = gpMirrorModelManager->unk24->getUnk98();
			const JGeometry::TVec3<f32>& mirror = unkB8[mirrorIdx];
			f32 dist                            = camPos.distance(mirror);
			for (int i = 0; i < 8; ++i) {
				f32 d = camPos.distance(unk30[i]);
				if (d > unk90[i] || d > dist)
					show(i);
				else
					hide(i);
			}
		} else {
			for (int i = 0; i < 8; ++i)
				hide(i);
		}
	}
}

void TMammaMirrorMapOperator::loadAfter()
{
	JDrama::TActor* mirror
	    = (JDrama::TActor*)JDrama::TNameRefGen::search("mirrorS");
	unkB8[0].set(mirror->mPosition);
	mirror = (JDrama::TActor*)JDrama::TNameRefGen::search("mirrorM");
	unkB8[1].set(mirror->mPosition);
	mirror = (JDrama::TActor*)JDrama::TNameRefGen::search("mirrorL");
	unkB8[2].set(mirror->mPosition);
	J3DJoint* joint = ((TMapStaticObj*)JDrama::TNameRefGen::search("鏡内地形"))
	                      ->getModelData()
	                      ->getJointNodePointer(2);
	for (int i = 0; i < 8; ++i) {
		unk10[i] = joint;
		unk30[i].set(0.5f * (joint->getMax().x + joint->getMin().x),
		             0.5f * (joint->getMax().y + joint->getMin().y),
		             0.5f * (joint->getMax().z + joint->getMin().z));
		f32 x = 0.5f * (joint->getMax().x - joint->getMin().x);
		f32 z = 0.5f * (joint->getMax().z - joint->getMin().z);
		if (x > z)
			unk90[i] = x;
		else
			unk90[i] = z;
		unk90[i] += 2000.0f;
		if (unk90[i] > 3000.0f)
			unk90[i] = 3000.0f;
		joint = (J3DJoint*)joint->getYounger();
	}
}

TMammaMirrorMapOperator::TMammaMirrorMapOperator(const char* name)
    : JDrama::TViewObj(name)
{
	for (int i = 0; i < 8; ++i) {
		unk10[i] = nullptr;
		unk30[i].zero();
		unk90[i] = 0.0f;
		unkB0[i] = 0;
	}
	for (int i = 0; i < 3; ++i)
		unkB8[i].zero();
}

u32 TSandEgg::getSDLModelFlag() const { return 0; }
