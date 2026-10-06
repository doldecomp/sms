#include <Enemy/Conductor.hpp>
#include <Enemy/Graph.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JSupport/JSUInputStream.hpp>
#include <M3DUtil/MActor.hpp>
#include <MSound/MSound.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MSound/SoundEffects.hpp>
#include <Map/MapCollisionManager.hpp>
#include <MoveBG/MapObjFence.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <MoveBG/MapObjMessenger.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/Yoshi.hpp>
#include <Strategic/Strategy.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

static const char cDirtyFileName[] = "/scene/map/pollution/H_ma_rak.bti";
static const char cDirtyTexName[]  = "H_ma_rak_dummy";

BOOL TFence::receiveMessage(THitActor* param_1, u32 param_2)
{
	if (param_2 == HIT_MESSAGE_SUPER_HIP_DROP) {
		startBck("fence_normal_shake");
		return TRUE;
	}
	return FALSE;
}

void TFence::initMapCollisionData()
{
	mMapCollisionManager = new TMapCollisionManager(1, "mapObj", this);
	if (strcmp(unkF4, "fence3x3") != 0) {
		if (fabsf(mRotation.x) < 1.0f && fabsf(mRotation.z) < 1.0f)
			mMapCollisionManager->init("fence_normal_v_tool", 0, nullptr);
		else
			mMapCollisionManager->init("fence_h_tool", 0, nullptr);
	} else {
		if (fabsf(mRotation.x) < 1.0f && fabsf(mRotation.z) < 1.0f)
			mMapCollisionManager->init("fence_half_v_tool", 0, nullptr);
		else
			mMapCollisionManager->init("fence_half_h_tool", 0, nullptr);
	}

	mMapCollisionManager->setUpActiveCollisionTRS(mPosition, mRotation,
	                                              mScaling);
}

void TFence::initMapObj()
{
	if (strstr(unkF4, "bamboo") != nullptr)
		unk138 = 1;

	TMapObjBase::initMapObj();
}

BOOL TRevolvingFenceOuter::receiveMessage(THitActor* param_1, u32 param_2)
{
	if (param_2 == HIT_MESSAGE_SUPER_HIP_DROP) {
		startBck("fence_revolve_outer_shake");
		unk13C->startBck("fence_revolve_inner_shake");
		return TRUE;
	}
	return FALSE;
}

void TRevolvingFenceOuter::initMapCollisionData()
{
	mMapCollisionManager = new TMapCollisionManager(1, "mapObj", this);
	if (fabsf(mRotation.x) < 1.0f && fabsf(mRotation.z) < 1.0f)
		mMapCollisionManager->init("fence_revolve_outer_v_tool", 0, nullptr);
	else
		mMapCollisionManager->init("fence_revolve_outer_h_tool", 0, nullptr);
	mMapCollisionManager->setUpActiveCollisionTRS(mPosition, mRotation,
	                                              mScaling);
	unk13C = unk138 ? TMapObjBaseManager::newAndRegisterObj(
	                      "bambooFence_revolve_inner", mPosition, mRotation)
	                : TMapObjBaseManager::newAndRegisterObj(
	                      "fence_revolve_inner", mPosition, mRotation);
	unk13C->appear();
}

BOOL TRevolvingFenceInner::receiveMessage(THitActor* param_1, u32 param_2)
{
	if (param_2 == HIT_MESSAGE_SUPER_HIP_DROP && !unk140) {
		if (isState(STATE_NORMAL)) {
			SMSGetMSound()->startSoundActor(MSD_SE_OBJ_FENCE_REVERSE1,
			                                &mPosition, 0, nullptr, 0, 4);
			mState = STATE_UNK3;
			startBck("fence_revolve_inner_roll_down");
			offMapObjFlag(MAP_OBJ_FLAG_UNK100);
			return TRUE;
		} else if (isState(STATE_UNK2)) {
			SMSGetMSound()->startSoundActor(MSD_SE_OBJ_FENCE_REVERSE2,
			                                &mPosition, 0, nullptr, 0, 4);
			mState = STATE_UNK4;
			startBck("fence_revolve_inner_roll_up");
			offMapObjFlag(MAP_OBJ_FLAG_UNK100);
			return TRUE;
		}
	}
	if (param_2 == HIT_MESSAGE_SUPER_HIP_DROP && unk140) {
		f32 angle = 180.0f * (getRotYFromAxisZ(SMS_GetMarioPos()) / 3.14f)
		            + mInitialRotation.y;
		angle = MsWrap(angle, -180.0f, 180.0f);
		if ((-180.0f < angle && angle < -90.0f)
		    || (0.0f < angle && angle < 90.0f)) {
			SMSGetMSound()->startSoundActor(MSD_SE_OBJ_FENCE_REVERSE1,
			                                &mPosition, 0, nullptr, 0, 4);
			if (isState(STATE_NORMAL))
				mState = STATE_UNK3;
			else
				mState = STATE_UNK4;
		} else {
			SMSGetMSound()->startSoundActor(MSD_SE_OBJ_FENCE_REVERSE2,
			                                &mPosition, 0, nullptr, 0, 4);
			if (isState(STATE_NORMAL))
				mState = STATE_UNK5;
			else
				mState = STATE_UNK6;
		}
		return TRUE;
	}
	return FALSE;
}

void TRevolvingFenceInner::calcCurrentMtx()
{
	mRotation.y = unk13C + mInitialRotation.y;
	MsWrap(mRotation.y, 0.0f, 360.0f);

	MtxPtr mtx = getModel()->getAnmMtx(0);
	MsMtxSetRotY(mtx, mRotation.y);
	mtx[0][3] = mPosition.x;
	mtx[1][3] = mPosition.y - mYOffset;
	mtx[2][3] = mPosition.z;
}

void TRevolvingFenceInner::controlWall()
{
	switch (mState) {
	case STATE_NORMAL:
	case STATE_UNK2:
		break;
	case STATE_UNK3:
		unk13C += mSpeed;
		if (unk13C > 180.0f) {
			unk13C      = 180.0f;
			mRotation.y = unk13C + mInitialRotation.y;
			mState      = STATE_UNK2;
		}
		calcCurrentMtx();
		break;
	case STATE_UNK4:
		unk13C += mSpeed;
		if (unk13C > 360.0f) {
			unk13C      = 0.0f;
			mRotation.y = unk13C + mInitialRotation.y;
			mState      = STATE_NORMAL;
		}
		calcCurrentMtx();
		break;
	case STATE_UNK5:
		unk13C -= mSpeed;
		if (unk13C < -180.0f) {
			unk13C      = 180.0f;
			mRotation.y = unk13C + mInitialRotation.y;
			mState      = STATE_UNK2;
		}
		calcCurrentMtx();
		break;
	case STATE_UNK6:
		unk13C -= mSpeed;
		if (unk13C < 0.0f) {
			unk13C      = 0.0f;
			mRotation.y = unk13C + mInitialRotation.y;
			mState      = STATE_NORMAL;
		}
		calcCurrentMtx();
		break;
	}
}

void TRevolvingFenceInner::controlGroundRoof()
{
	switch (mState) {
	case STATE_NORMAL:
	case STATE_UNK2:
		break;
	case STATE_UNK3:
	case STATE_UNK5:
		if (mMActor->curAnmEndsNext()) {
			mState = STATE_UNK2;
			mMActor->setFrameRate(0.0f, ANM_TYPE_BCK);
			mMActor->getFrameCtrl(ANM_TYPE_BCK)->setFrame(0.0f);
			mMActor->calc();
			onMapObjFlag(MAP_OBJ_FLAG_UNK100);
		}
		break;
	case STATE_UNK4:
	case STATE_UNK6:
		if (mMActor->curAnmEndsNext()) {
			mState = STATE_NORMAL;
			mMActor->setFrameRate(0.0f, ANM_TYPE_BCK);
			mMActor->getFrameCtrl(ANM_TYPE_BCK)->setFrame(0.0f);
			mMActor->calc();
			onMapObjFlag(MAP_OBJ_FLAG_UNK100);
		}
		break;
	}
}

void TRevolvingFenceInner::setGroundCollision()
{
	if (SMS_GetYoshi()->isHatched()
	    && mPosition.x - mBodyRadius < SMS_GetYoshi()->getTranslation().x
	    && mPosition.x + mBodyRadius > SMS_GetYoshi()->getTranslation().x
	    && mPosition.z - mBodyRadius < SMS_GetYoshi()->getTranslation().z
	    && mPosition.z + mBodyRadius > SMS_GetYoshi()->getTranslation().z) {
		TMtx34f mtx;
		mtx.set(getModel()->getAnmMtx(0));
		mMapCollisionManager->moveActiveCollisionMtx(mtx);
	}
	TMapObjBase::setGroundCollision();
}

void TRevolvingFenceInner::control()
{
	TMapObjBase::control();

	if (unk140 != 0)
		controlWall();
	else
		controlGroundRoof();
}

void TRevolvingFenceInner::initMapCollisionData()
{
	mMapCollisionManager = new TMapCollisionManager(1, "mapObj", this);
	if (fabsf(mRotation.x) < 80.0f && fabsf(mRotation.z) < 80.0f)
		mMapCollisionManager->init("fence_revolve_inner_v_tool", 1, nullptr);
	else
		mMapCollisionManager->init("fence_revolve_inner_h_tool", 1, nullptr);
}

void TRevolvingFenceInner::initMapObj()
{
	TFence::initMapObj();
	if (fabsf(mRotation.x) < 1.0f && fabsf(mRotation.z) < 1.0f)
		unk140 = 1;
	else
		unk140 = 0;
	mMapCollisionManager->setUpActiveCollisionTRS(mPosition, mRotation,
	                                              mScaling);
}

void TFenceWater::draw() const { }

BOOL TFenceWater::receiveMessage(THitActor* param_1, u32 param_2)
{
	if (!isState(STATE_UNK3) && param_2 == HIT_MESSAGE_SPRAYED_BY_WATER) {
		unk13C = mWaterAccel;
		if (unk13C > 0.0f)
			changeStatusToGo();
		return TRUE;
	}
	return FALSE;
}

void TFenceWater::changeStatusToGo()
{
	SMSGetMSound()->startSoundActor(MSD_SE_OBJ_WATER_FENCE_FW, &mPosition, 0,
	                                nullptr, 0, 4);
	mState = STATE_UNK2;
}

void TFenceWater::changeStatusToWait()
{
	unk140 = 0.0f;
	unk13C = 0.0f;
	mState = STATE_NORMAL;
}

void TFenceWater::controlRotation()
{
	switch (mState) {
	case STATE_NORMAL:
		break;
	case STATE_UNK2:
		unk140 -= unk13C;
		if (unk140 <= -90.0f) {
			unk140 = -90.0f;
			unk13C = 0.0f;
			mState = STATE_UNK3;
			startStateTimer(mTurnedWaitTime);
		}
		break;
	case STATE_UNK3:
		if (!isStateTimerEngaged()) {
			SMSGetMSound()->startSoundActor(MSD_SE_OBJ_WATER_FENCE_REV,
			                                &mPosition, 0, nullptr, 0, 4);
			unk13C = mBackSpeed;
			mState = STATE_UNK4;
		}
		break;
	case STATE_UNK4:
		unk140 += unk13C;
		if (unk140 >= 0.0f)
			changeStatusToWait();
		break;
	}
}

void TFenceWater::control()
{
	TMapObjBase::control();
	controlRotation();
	mRotation.y         = MsWrap(unk140 + mInitialRotation.y, 0.0f, 360.0f);
	unk144->mPosition.x = mPosition.x + 500.0f * MsCos(mRotation.y);
	unk144->mPosition.z = mPosition.z - 500.0f * MsSin(mRotation.y);
}

void TFenceWater::initMapCollisionData()
{
	TMapObjBase::initMapCollisionData();
}

void TFenceWater::initMapObj()
{
	TFence::initMapObj();
	unk144        = new TMapObjMessenger("地形オブジェメッセンジャー");
	unk144->unk68 = this;
	unk144->initHitActor(getActorType(), 1, 0, 0.0f, 0.0f, 100.0f, 300.0f);
	unk144->offHitFilter(HIT_FILTER_NO_COLLISION);
	unk144->mPosition.set(mPosition.x, mPosition.y - 150.0f, mPosition.z);
	static_cast<TIdxGroupObj*>(
	    JDrama::TNameRefGen::search("オブジェクトグループ"))
	    ->getChildren()
	    .push_back(unk144);
}

void TFenceWaterH::control()
{
	TMapObjBase::control();
	controlRotation();
	mRotation.z = MsWrap(unk140 + mInitialRotation.z, 0.0f, 360.0f);
	TPosition3f rotationY;
	rotationY.identity();
	rotationY.setEular(0.0f, 0.017453294f * mRotation.y, 0.0f);
	TPosition3f rotationZ;
	rotationZ.identity();
	rotationZ.setEular(0.0f, 0.0f, 0.017453294f * mRotation.z);
	MTXConcat(rotationY, rotationZ, rotationY);
	rotationY.setTrans(mPosition.x, mPosition.y, mPosition.z);
	getModel()->setAnmMtx(0, rotationY);
	unk144->mPosition.x = mPosition.x;
	unk144->mPosition.y = mPosition.y - 150.0f;
}

void TFenceWaterH::changeStatusToGo()
{
	TFenceWater::changeStatusToGo();
	setUpMapCollision(1);
}

void TFenceWaterH::changeStatusToWait()
{
	TFenceWater::changeStatusToWait();
	setUpMapCollision(0);
}

BOOL TRailFence::receiveMessage(THitActor* param_1, u32 param_2)
{
	if (param_2 == HIT_MESSAGE_SUPER_HIP_DROP) {
		SMSGetMSound()->startSoundActor(MSD_SE_OBJ_MVING_FENCT_PNCH, &mPosition,
		                                0, nullptr, 0, 4);
		setUpMapCollision(1);
		offMapObjFlag(MAP_OBJ_FLAG_UNK100);
		mState = STATE_UNK2;
		return TRUE;
	}
	return FALSE;
}

void TRailFence::falling()
{
	JGeometry::TVec3<f32> velocity = getVelocity();
	mPosition.y += velocity.y;
	mVelocity.y -= mGravity;
	if (mVelocity.y < -100.0f)
		mVelocity.y = -100.0f;
	if (mPosition.y < mInitialPosition.y - mFallHeight) {
		mPosition.set(mInitialPosition);
		setUpMapCollision(0);
		unk13C->setTo(
		    unk13C->getGraph()->findNearestNodeIndex(mPosition, 0xffffffff));
		makeObjAppeared();
		calcRootMatrix();
		getModel()->calc();
		onMapObjFlag(MAP_OBJ_FLAG_UNK100);
	}
}

void TRailFence::goOnRail()
{
	if (!unk13C->getGraph())
		return;
	JGeometry::TVec3<f32> direction = unk13C->getCurrentPos();
	direction -= mPosition;
	if (direction.squared() < 50.0f) {
		TGraphTracer* tracer  = unk13C;
		const TRailNode* node = tracer->getGraph()
		                            ->getGraphNode(tracer->getCurGraphIndex())
		                            .getRailNode();
		if (node->mConnectionNum == 0 && (node->mFlags & 8)) {
			SMSGetMSound()->startSoundActor(MSD_SE_OBJ_MVING_FENCT_SET,
			                                &mPosition, 0, nullptr, 0, 4);
			startStateTimer(mWaitTime);
			startAnim(1);
			mState = STATE_UNK3;
			return;
		}
		tracer->moveToShortestNext();
		direction.set(unk13C->getCurrentPos());
	}
	SMSGetMSound()->startSoundActor(MSD_SE_OBJ_MVING_FENCE_MOVE, &mPosition, 0,
	                                nullptr, 0, 4);
	VECNormalize(&direction, &direction);
	direction.scale(unk140);
	mPositionDelta += direction;
}

void TRailFence::control()
{
	TMapObjBase::control();
	switch (mState) {
	case STATE_NORMAL:
		break;
	case STATE_UNK2:
		goOnRail();
		break;
	case STATE_UNK3:
		if (!isStateTimerEngaged()) {
			removeMapCollision();
			SMSGetMSound()->startSoundActor(MSD_SE_OBJ_SUPERBLOCK_BREAK,
			                                &mPosition, 0, nullptr, 0, 4);
			mState = STATE_UNK4;
		}
		break;
	case STATE_UNK4:
		falling();
		break;
	}
}

void TRailFence::initMapCollisionData() { TMapObjBase::initMapCollisionData(); }

void TRailFence::load(JSUMemoryInputStream& param_1)
{
	TMapObjBase::load(param_1);
	char graphName[64];
	param_1.readString(graphName, sizeof(graphName));
	TGraphWeb* graph = gpConductor->getGraphByName(graphName);
	if (graph != nullptr && !graph->isDummy()) {
		unk13C->init(graph);
		unk13C->setTo(graph->findNearestNodeIndex(mPosition, 0xffffffff));
	}
	unk140   = 8.0f;
	mGravity = 0.3f;
}

TRailFence::TRailFence(const char* param_1)
    : TFence(param_1)
    , unk13C(new TGraphTracer)
    , unk140(0.0f)
{
}

f32 TRevolvingFenceInner::mSpeed = 4.0f;

f32 TFenceWater::mWaterAccel     = 2.1f;
f32 TFenceWater::mBackSpeed      = 3.0f;
int TFenceWater::mTurnedWaitTime = 600;

f32 TRailFence::mFallHeight = 50000.0f;
int TRailFence::mWaitTime   = 240;
