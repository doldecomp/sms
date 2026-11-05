#include <Map/MapEventSink.hpp>
#include <Map/PollutionManager.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <Map/MapStaticObject.hpp>
#include <MoveBG/MapObjBase.hpp>
#include <MoveBG/ItemManager.hpp>
#include <Enemy/GateKeeper.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <M3DUtil/MActor.hpp>
#include <Player/ModelWaterManager.hpp>
#include <Player/MarioAccess.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/MSoundMainSide.hpp>
#include <System/MarDirector.hpp>
#include <System/Particles.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/JParticle/JPAResourceManager.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

u32 TMapEventSink::mCleanedDegree = 10;

TJointObj* TMapEventSink::getBuilding(int i) const
{
	return mRootMapJointModel->getChild(0)->getChild(i + mFirstBuildingIdx);
}

bool TMapEventSink::isBuried(int i) const
{
	return !mIsBuildingRecovered[i - mFirstBuildingIdx];
}

f32 TMapEventSink::getSinkOffsetY() const
{
	return mRaisingJoint->getMax().y - mRaisingJoint->getMin().y;
}

TPollutionObj* TMapEventSink::getPollutionObj(int i)
{
	return gpPollution->getLayer(mBuildingPollutionRefs[i].mLayerIndex)
	    ->getObj(mBuildingPollutionRefs[i].mObjIndex);
}

bool TMapEventSink::isFinishedAll() const
{
	for (int i = 0; i < mBuildingNum; ++i)
		if (!mIsBuildingRecovered[i])
			return false;

	return true;
}

void TMapEventSink::makeBuildingRecovered(int i)
{
	getBuilding(i)->alive();

	J3DTransformInfo& info = getBuilding(i)->getJoint()->getTransformInfo();
	info.mTranslate.y      = mInitialY;
	getBuilding(i)->getJoint()->setTransformInfo(info);

	mRisingBuildingCollisions[i]->remove();
	mRaisedBuildingCollisions[i]->setUp();
	mIsBuildingRecovered[i] = true;
}

void TMapEventSink::finishControl()
{
	makeBuildingRecovered(mRaisingBuildingIdx);
	mRaisingBuilding    = nullptr;
	mRaisingJoint       = nullptr;
	mRaisingBuildingIdx = -1;
	if (strcmp("イベント（リコゲート）", mName) == 0)
		MSMainProc::setBossLivesFlag2(false);
	else
		MSMainProc::setBossLivesFlag(false);
	TMapEvent::finishControl();
}

void TMapEventSink::rising()
{
	J3DTransformInfo& info = mRaisingJoint->getTransformInfo();
	info.mTranslate.y += mRiseSpeed;
	mRaisingJoint->setTransformInfo(info);
	mRootMapJointModel->getMActor()->getModel()->calc();
}

bool TMapEventSink::control()
{
	if (mDemoTimer < mDemoDuration - mRiseStartDelay
	    && mDemoTimer > mRiseEndDelay)
		rising();

	const J3DTransformInfo& info = mRaisingJoint->getTransformInfo();
	mRisingBuildingCollisions[mRaisingBuildingIdx]->moveTrans(
	    JGeometry::TVec3<f32>(info.mTranslate.x, info.mTranslate.y,
	                          info.mTranslate.z));

	if (mDemoTimer > mRiseEndDelay
	    && (SMSGetMarDirector()->getCurrentStage() != 2
	        || mIsBuildingRecovered[1 - mFirstBuildingIdx] == 0
	        || mRaisingBuildingIdx != 1)) {
		SMSRumbleMgr->start(0x13, (f32*)nullptr);
		SMSGetMSound()->startSoundActor(
		    MSD_SE_OBJ_QUAKE, &mBuildingEventPositions[mRaisingBuildingIdx], 0,
		    nullptr, 0, 4);
	}

	if (mDemoTimer > 0) {
		--mDemoTimer;
		return false;
	} else {
		return true;
	}
}

void TMapEventSink::startControl()
{
	mState           = STATE_CONTROLLING;
	mRaisingBuilding = getBuilding(getRaisingBuildingIdx());
	mRaisingBuilding->alive();
	mRaisingJoint = getBuilding(getRaisingBuildingIdx())->getJoint();

	J3DTransformInfo& info = mRaisingJoint->getTransformInfo();
	mInitialY              = info.mTranslate.y;

	f32 sinkDepth;
	if (mSinkDepthOverride != 0.0f)
		sinkDepth = mSinkDepthOverride;
	else
		sinkDepth = getSinkOffsetY();

	info.mTranslate.y -= sinkDepth;
	mRaisingJoint->setTransformInfo(info);

	J3DModel* model = mRootMapJointModel->getMActor()->getModel();
	model->calc();
	int iVar3  = (mDemoDuration - mRiseStartDelay) - mRiseEndDelay;
	mRiseSpeed = sinkDepth / iVar3;
	mDemoTimer = mDemoDuration;

	JGeometry::TVec3<f32> trans(info.mTranslate.x, info.mTranslate.y,
	                            info.mTranslate.z);
	mRisingBuildingCollisions[mRaisingBuildingIdx]->setUpTrans(trans);
}

void TMapEventSink::initBuilding(int index, JSUMemoryInputStream& stream)
{
	u32 value;
	stream >> value;
	mBuildingPollutionRefs[index].mLayerIndex = value;
	stream >> value;
	mBuildingPollutionRefs[index].mObjIndex = value;
	mRaisedBuildingCollisions[index]
	    = TMapObjBase::newAndInitBuildingCollisionWarp(index + 1, nullptr);
	mRisingBuildingCollisions[index]
	    = TMapObjBase::newAndInitBuildingCollisionMove(index + 1, nullptr);
}

void TMapEventSink::initWithBuildingNum(JSUMemoryInputStream& stream)
{
	u32 value;
	stream >> value;
	mFirstBuildingIdx         = value;
	mBuildingEventPositions   = new JGeometry::TVec3<f32>[mBuildingNum];
	mRaisedBuildingCollisions = new TMapCollisionWarp*[mBuildingNum];
	mRisingBuildingCollisions = new TMapCollisionMove*[mBuildingNum];
	mIsBuildingRecovered      = new bool[mBuildingNum];
	mBuildingPollutionRefs    = new PollutionRef[mBuildingNum];
}

void TMapEventSink::load(JSUMemoryInputStream& stream)
{
	TMapEvent::load(stream);
	mBuildingNum = stream.readU32();
	initWithBuildingNum(stream);
	for (int i = 0; i < mBuildingNum; ++i) {
		mIsBuildingRecovered[i] = false;
		getBuilding(i)->kill();
		initBuilding(i, stream);
	}

	if (SMSGetMarDirector()->getCurrentStage() == 0) {
		mCleanedDegree     = 30;
		mSinkDepthOverride = 200.0f;
	} else if (SMSGetMarDirector()->getCurrentStage() == 2) {
		mCleanedDegree = 30;
	}
}

TMapEventSink::TMapEventSink(const char* name)
    : TMapEvent(name)
{
	mBuildingNum              = 0;
	mFirstBuildingIdx         = 0;
	mRaisingBuildingIdx       = -1;
	mRaisingBuilding          = nullptr;
	mRaisingJoint             = nullptr;
	mInitialY                 = 0.0f;
	mSinkDepthOverride        = 0.0f;
	mRiseSpeed                = 5.0f;
	mDemoDuration             = 600;
	mRiseStartDelay           = 0;
	mRiseEndDelay             = 0;
	mDemoTimer                = 0;
	mBuildingEventPositions   = nullptr;
	mIsBuildingRecovered      = nullptr;
	mRaisedBuildingCollisions = nullptr;
	mRisingBuildingCollisions = nullptr;
	mBuildingPollutionRefs    = nullptr;
	mCleanedDegree            = 10;
}

bool TMapEventSinkInPollution::watch()
{
	for (int i = 0; i < mBuildingNum; ++i) {
		if (!mIsBuildingRecovered[i] && getPollutionObj(i)->isCleaned()) {
			mRaisingBuildingIdx = i;
			return true;
		}
	}
	return false;
}

void TMapEventSinkInPollution::initBuriedBuilding()
{
	for (int i = 0; i < mBuildingNum; ++i)
		if (getPollutionObj(i)->isCleaned())
			makeBuildingRecovered(i);
}

void TMapEventSinkInPollution::loadAfter()
{
	TMapEventSink::loadAfter();
	for (int i = 0; i < mBuildingNum; ++i) {
		gpPollution->getCounterObj().registerPollutionObj(
		    getPollutionObj(i), &getPollutionObj(i)->mCounter);
	}
}

TPollutionObj* TMapEventSinkInPollutionReset::getResetPollutionObj(int i)
{
	return gpPollution->getLayer(mBuildingPollutionRefs[i].mLayerIndex)
	    ->getObj(mBuildingPollutionRefs[i].mObjIndex + 1);
}

void TMapEventSinkInPollutionReset::makeBuildingRecovered(int i)
{
	TMapEventSinkInPollution::makeBuildingRecovered(i);
	getPollutionObj(i)->kill();
	getResetPollutionObj(i)->alive();
	getResetPollutionObj(i)->updateDepthMap();
}

void TMapEventSinkInPollutionReset::loadAfter()
{
	TMapEventSinkInPollution::loadAfter();
	for (int i = 0; i < mBuildingNum; ++i) {
		getPollutionObj(i)->alive();
		getResetPollutionObj(i)->kill();
	}
}

void TMapEventSinkBianco::finishControl()
{
	char buffer[64];
	if (mRaisingBuildingIdx == 0) {
		TMapObjBase::setJointTransY(mRisingTerrainInMirrorJoint, 0.0f);
		for (int i = 0; i < 6; ++i) {
			snprintf(buffer, 0x40, "バナナツリー（スケール） %d", i);
			static_cast<TLiveActor*>(JDrama::TNameRefGen::search(buffer))
			    ->receiveMessage(gpModelWaterManager->unk2514[0],
			                     HIT_MESSAGE_SPRAYED_BY_WATER);
		}

		for (int i = 0; i < 7; ++i) {
			snprintf(buffer, 0x40, "落書き内%02d", i);
			static_cast<TLiveActor*>(JDrama::TNameRefGen::search(buffer))
			    ->receiveMessage(gpModelWaterManager->unk2514[0],
			                     HIT_MESSAGE_SPRAYED_BY_WATER);
		}
	}

	TMapEventSinkInPollutionReset::finishControl();

	for (int i = 0; i < gpPollution->getJointModelNum(); ++i)
		gpPollution->getLayer(i)->stopDecay();
}

void TMapEventSinkBianco::rising()
{
	TMapEventSinkInPollutionReset::rising();
	if (mRaisingBuildingIdx == 0)
		TMapObjBase::moveJoint(mRisingTerrainInMirrorJoint, 0.0f, mRiseSpeed,
		                       0.0f);
}

bool TMapEventSinkBianco::control()
{
	if (mRaisingBuildingIdx == 0 && mDemoTimer == mShineAppearDemoTime) {
		gpItemManager->makeShineAppearWithTime(
		    "シャイン（坂上げ用）", 300,
		    mBuildingEventPositions[mRaisingBuildingIdx].x,
		    mBuildingEventPositions[mRaisingBuildingIdx].y,
		    mBuildingEventPositions[mRaisingBuildingIdx].z, 0, 0x3C, 0x3C);
	}
	return TMapEventSinkInPollutionReset::control();
}

void TMapEventSinkBianco::startControl()
{
	switch (mRaisingBuildingIdx) {
	case 0: {
		mDemoDuration      = 1320;
		mRiseStartDelay    = 120;
		mRiseEndDelay      = 600;
		mSinkDepthOverride = 1700.0f;
		JGeometry::TVec3<f32> zero(0.0f, 0.0f, 0.0f);
		mShineAppearDemoTime = 360;

		gpMarioParticleManager->emit(MAP_MAP_MS_OBJUP_SLOPE_A, &zero, 0,
		                             nullptr);
		gpMarioParticleManager->emit(MAP_MAP_MS_OBJUP_SLOPE_B, &zero, 2,
		                             nullptr);
		break;
	}

	case 1:
		mDemoDuration      = 600;
		mRiseStartDelay    = 120;
		mRiseEndDelay      = 120;
		mSinkDepthOverride = 1500.0f;
		break;
	}

	TMapEventSinkInPollutionReset::startControl();

	if (mRaisingBuildingIdx == 0) {
		SMS_ShowJoint(mRisingTerrainInMirrorJoint->getMesh(), true);
		SMS_MarioWarpRequest(mMarioWarpPosition, mMarioWarpAngle);
		mBuildingEventPositions[mRaisingBuildingIdx].set(7170.0f, 3675.0f,
		                                                 -185.0f);
		SMSGetMarDirector()->fireStartDemoCamera(
		    "bianco0_event0", nullptr, -1, 0.0f, true, nullptr, 0, nullptr,
		    JDrama::TFlagT<u16>(0));
	}
}

bool TMapEventSinkBianco::watch()
{
	if (!mIsBuildingRecovered[0]
	    && mGateKeeper->checkLiveFlag(LIVE_FLAG_DEAD)) {
		mRaisingBuildingIdx = 0;
		for (int i = 0; i < gpPollution->getJointModelNum(); ++i)
			gpPollution->getLayer(i)->startDecay();
		return true;
	}

	for (int i = 1; i < mBuildingNum; ++i) {
		if (!mIsBuildingRecovered[i]) {
			if (getPollutionObj(i)->isCleaned()) {
				mRaisingBuildingIdx = i;
				return true;
			}
		}
	}

	return false;
}

void TMapEventSinkBianco::loadAfter()
{
	TMapEventSinkInPollutionReset::loadAfter();

	TMapStaticObj* ref
	    = static_cast<TMapStaticObj*>(JDrama::TNameRefGen::search("鏡内地形"));
	mRisingTerrainInMirrorJoint = ref->getModelData()->getJointNodePointer(2);
	TMapObjBase::moveJoint(mRisingTerrainInMirrorJoint, 0.0f, -1700.0f, 0.0f);
	SMS_ShowJoint(mRisingTerrainInMirrorJoint->getMesh(), false);
	mGateKeeper = static_cast<TGateKeeperBase*>(
	    JDrama::TNameRefGen::search("ゲートキーパー"));
}

void TMapEventSinkBianco::load(JSUMemoryInputStream& stream)
{
	TMapEventSinkInPollutionReset::load(stream);
	stream.readString();
	stream >> mMarioWarpPosition.x >> mMarioWarpPosition.y
	    >> mMarioWarpPosition.z;
	s32 dummy;
	stream >> dummy;
	stream >> mMarioWarpAngle;

	SMS_LoadParticle("/scene/map/map/ms_objup_slope_a.jpa",
	                 MAP_MAP_MS_OBJUP_SLOPE_A);
	SMS_LoadParticle("/scene/map/map/ms_objup_slope_b.jpa",
	                 MAP_MAP_MS_OBJUP_SLOPE_B);
}

void TMapEventSinkShadowMario::rising()
{
	TMapEventSink::rising();
	mBuildingPlacements[mRaisingBuildingIdx]->mPosition.y += mRiseSpeed;
}

void TMapEventSinkShadowMario::raiseBuilding(int i)
{
	mRaisingBuildingIdx = i;
	startControl();
}

void TMapEventSinkShadowMario::loadAfter()
{
	TMapEventSink::loadAfter();
	for (int i = 0; i < mBuildingNum; ++i) {
		mBuildingPlacements[i] = static_cast<JDrama::TPlacement*>(
		    JDrama::TNameRefGen::search(mBuildingPlacementNames[i]));
		TJointObj* obj = getBuilding(i);
		mBuildingPlacements[i]->mPosition.y
		    -= obj->getJoint()->getMax().y - obj->getJoint()->getMin().y;
	}
}

void TMapEventSinkShadowMario::initBuilding(int i, JSUMemoryInputStream& stream)
{
	TMapEventSink::initBuilding(i, stream);
	mBuildingPlacementNames[i] = stream.readString();
}

void TMapEventSinkShadowMario::initWithBuildingNum(JSUMemoryInputStream& stream)
{
	TMapEventSink::initWithBuildingNum(stream);
	mBuildingPlacements     = new JDrama::TPlacement*[mBuildingNum];
	mBuildingPlacementNames = new const char*[mBuildingNum];
}
