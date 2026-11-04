#include <Map/MapEventDolpic.hpp>
#include <Map/Map.hpp>
#include <Map/MapModel.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <Map/PollutionManager.hpp>
#include <MoveBG/MapObjBase.hpp>
#include <Camera/CameraShake.hpp>
#include <Player/MarioAccess.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <MSound/MSound.hpp>
#include <MSound/SoundEffects.hpp>
#include <System/FlagManager.hpp>
#include <System/MarDirector.hpp>
#include <System/Particles.hpp>
#include <JSystem/JParticle/JPAResourceManager.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <string.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/MActorAnm.hpp>

bool TDolpicEventBiancoGate::isFinishedAll() const
{
	if (stateIs(STATE_FINISHED))
		return true;
	return false;
}

bool TDolpicEventBiancoGate::control()
{
	mKingStatue->mPosition.y += mKingStatueRiseSpeed;
	gpCameraShake->keepShake(CAM_SHAKE_MODE_UNK5, 1.0f);
	SMSRumbleMgr->start(0, (f32*)nullptr);
	{
		JGeometry::TVec3<f32>& pos = mKingStatue->mPosition;
		gpPollution->clean(pos.x, pos.y, pos.z, 10000.0f);
	}
	if (mKingStatue->mPosition.y >= mKingStatue->getInitialPosition().y) {
		mKingStatue->mPosition.y = mKingStatue->getInitialPosition().y;
		mKingStatue->setUpMapCollision(0);
		mState = STATE_FINISHED;
		return true;
	}
	return false;
}

bool TDolpicEventBiancoGate::watch()
{
	if (TFlagManager::getInstance()->getBool(
	        MSF_BIANCO_POLLUTED_PIRANHA_DEFEATED)) {
		mKingStatue->appear();
		mKingStatue->setUpMapCollision(1);
		return true;
	}
	return false;
}

void TDolpicEventBiancoGate::loadAfter()
{
	JDrama::TNameRef::loadAfter();
	mKingStatue
	    = static_cast<TMapObjBase*>(JDrama::TNameRefGen::search("dptKing"));
	mKingStatue->kill();
	mKingStatue->mPosition.y -= 1800.0f;
}

TDolpicEventBiancoGate::TDolpicEventBiancoGate(const char* name)
    : TMapEvent(name)
    , mKingStatue(nullptr)
    , mKingStatueRiseSpeed(3.0f)
{
}

bool TDolpicEventRiccoMammaGate::isFinishedAll() const
{
	if (stateIs(STATE_FINISHED))
		return true;
	return false;
}

void TDolpicEventRiccoMammaGate::rising()
{
	f32 scale
	    = TMapObjBase::getJointScaleY(mEventBuildingJoint) + mScaleUpSpeed;

	TPosition3f mtx;
	mtx.identity();
	mtx.ref(1, 1) = scale;
	mRisingBuildingCollision->moveMtx(mtx.mMtx);

	gpCameraShake->keepShake(CAM_SHAKE_MODE_UNK5, 1.0f);
	SMSRumbleMgr->start(0, (f32*)nullptr);
	TMapObjBase::setJointScaleY(mEventBuildingJoint, scale);
	TMapObjBase::setJointTransY(mEventBuildingJoint, 300.0f * (1.0f - scale));
	gpMap->getRootJointModel()->getModel()->calc();
}

bool TDolpicEventRiccoMammaGate::control()
{
	if (mDemoTimer < mDemoDuration - mRiseStartDelay
	    && mDemoTimer > mRiseEndDelay)
		rising();

	if (mDemoTimer > mRiseEndDelay) {
		SMSRumbleMgr->start(0x13, (f32*)nullptr);
		SMSGetMSound()->startSoundActor(MSD_SE_OBJ_QUAKE, &mEffectPosition, 0,
		                                nullptr, 0, 4);
	}

	if (mDemoTimer > 0) {
		mDemoTimer -= 1;
		return false;
	}

	TMapObjBase::setJointScaleY(mEventBuildingJoint, 1.0f);
	mRisingBuildingCollision->remove();
	mRisenBuildingCollision->setUp();
	SMSGetMarDirector()->fireEndDemoCamera();
	MSBgm::setTrackVolume(0, 1.0f, 15, 0);
	mState = STATE_FINISHED;
	return true;
}

bool TDolpicEventRiccoMammaGate::watch()
{
	if (!TFlagManager::getInstance()->getBool(mTriggerFlag)) {
		SMS_ShowJoint(mEventBuildingJoint->getMesh(), true);
		TMapObjBase::setJointScaleY(mEventBuildingJoint, mScaleUpSpeed);

		TPosition3f mtx;
		mtx.identity();
		mtx.ref(1, 1) = mScaleUpSpeed;
		(void)&mtx; // unused?!

		mRisingBuildingCollision->setUp();

		mDemoTimer = mDemoDuration;

		if (mTriggerFlag == MSF_RICCO_UNLOCKABLE) {
			SMSGetMarDirector()->fireStartDemoCamera(
			    "マニ屋上げデモカメラ", &mEffectPosition, -1, 0.0f, false,
			    nullptr, 0, nullptr, JDrama::TFlagT<u16>(0));
			gpMarioParticleManager->emit(MAP_MAP_MS_OBJUP_MANIYA_A,
			                             &mEffectPosition, 0, this);
			gpMarioParticleManager->emit(MAP_MAP_MS_OBJUP_MANIYA_B,
			                             &mEffectPosition, 2, this);
			gpPollution->getLayer(0)->startDecay();
		} else {
			SMSGetMarDirector()->fireStartDemoCamera(
			    "灯台上げデモカメラ", &mEffectPosition, -1, 0.0f, false,
			    nullptr, 0, nullptr, JDrama::TFlagT<u16>(0));
			gpMarioParticleManager->emit(MAP_MAP_MS_OBJUP_TOUDAI_A,
			                             &mEffectPosition, 0, this);
			gpMarioParticleManager->emit(MAP_MAP_MS_OBJUP_TOUDAI_B,
			                             &mEffectPosition, 2, this);
			gpPollution->getLayer(1)->startDecay();
		}

		SMS_MarioWarpRequest(mMarioStartPos, mMarioStartAngle);
		return true;
	}

	return false;
}

void TDolpicEventRiccoMammaGate::loadAfter()
{
	JDrama::TNameRef::loadAfter();
	mDemoDuration   = 720;
	mRiseStartDelay = 120;
	mRiseEndDelay   = 120;
	mScaleUpSpeed
	    = 1.0f / (f32)((mDemoDuration - mRiseStartDelay) - mRiseEndDelay);
	if (mBuildingAlreadyRevealed) {
		mRisenBuildingCollision->setUp();
		mState = STATE_FINISHED;
		if (mTriggerFlag == MSF_RICCO_UNLOCKABLE)
			gpPollution->getCounterLayer().offLayer(0);
		else
			gpPollution->getCounterLayer().offLayer(1);
	}
}

void TDolpicEventRiccoMammaGate::load(JSUMemoryInputStream& stream)
{
	TMapEvent::load(stream);
	stream.readString();
	stream >> mMarioStartPos.x >> mMarioStartPos.y >> mMarioStartPos.z;
	f32 unused;
	stream >> unused;
	stream >> mMarioStartAngle;

	int idx;
	if (strcmp("イベント（リコゲート）", getName()) == 0) {
		mTriggerFlag = MSF_RICCO_UNLOCKABLE; // the tanuki house
		idx          = 0;
	} else {
		mTriggerFlag = MSF_GELATO_UNLOCKABLE; // the lighthouse
		idx          = 1;
	}

	mRisingBuildingCollision
	    = TMapObjBase::newAndInitBuildingCollisionMove(idx + 1, nullptr);
	mRisenBuildingCollision
	    = TMapObjBase::newAndInitBuildingCollisionWarp(idx + 1, nullptr);
	if (TFlagManager::getInstance()->getBool(mTriggerFlag)) {
		mEventBuildingJoint = getBuilding(idx + 1)->getJoint();
		TMapObjBase::setJointScaleY(mEventBuildingJoint, 0.008f);
		TMapObjBase::setJointTransY(mEventBuildingJoint, 295.0f);
		gpMap->getRootJointModel()->getModel()->calc();
		if (mTriggerFlag == MSF_RICCO_UNLOCKABLE) {
			mEffectPosition.x = -10500.0f;
			mEffectPosition.y = 300.0f;
			mEffectPosition.z = 2003.0f;
			SMS_LoadParticle("/scene/map/map/ms_objup_maniya_a.jpa",
			                 MAP_MAP_MS_OBJUP_MANIYA_A);
			SMS_LoadParticle("/scene/map/map/ms_objup_maniya_b.jpa",
			                 MAP_MAP_MS_OBJUP_MANIYA_B);
		} else {
			mEffectPosition.x = 9915.0f;
			mEffectPosition.y = 300.0f;
			mEffectPosition.z = -7565.0f;
			SMS_LoadParticle("/scene/map/map/ms_objup_toudai_a.jpa",
			                 MAP_MAP_MS_OBJUP_TOUDAI_A);
			SMS_LoadParticle("/scene/map/map/ms_objup_toudai_b.jpa",
			                 MAP_MAP_MS_OBJUP_TOUDAI_B);
		}
		mBuildingAlreadyRevealed = false;
	} else {
		mBuildingAlreadyRevealed = true;
	}
}

TDolpicEventRiccoMammaGate::TDolpicEventRiccoMammaGate(const char* name)
    : TMapEvent(name)
    , mEventBuildingJoint(nullptr)
    , mRisingBuildingCollision(nullptr)
    , mRisenBuildingCollision(nullptr)
    , mTriggerFlag(0)
    , mBuildingAlreadyRevealed(false)
    , mScaleUpSpeed(0.0f)
    , mDemoDuration(0)
    , mRiseStartDelay(0)
    , mRiseEndDelay(0)
    , mDemoTimer(0)
    , mMarioStartAngle(0.0f)
{
	mEffectPosition.zero();
	mMarioStartPos.zero();
}
