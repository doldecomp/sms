#include <MoveBG/MapObjFence.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <MoveBG/MapObjMessenger.hpp>
#include <Enemy/Conductor.hpp>
#include <Enemy/Graph.hpp>
#include <M3DUtil/MActor.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <Map/MapCollisionManager.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MSound/MSound.hpp>
#include <MSound/SoundEffects.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/Yoshi.hpp>
#include <Strategic/Strategy.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/JGeometry/JGPosition3.hpp>
#include <math.h>
#include <string.h>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

#include <Player/MarioDirtyStrings.hpp>

BOOL TFence::receiveMessage(THitActor* sender, u32 message)
{
	if (message == HIT_MESSAGE_SUPER_HIP_DROP) {
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

	mMapCollisionManager->setUpUnk8TRS(mPosition, mRotation, mScaling);
}

void TFence::initMapObj()
{
	if (strstr(unkF4, "bamboo"))
		mIsBamboo = 1;

	TMapObjBase::initMapObj();
}

BOOL TRevolvingFenceOuter::receiveMessage(THitActor* sender, u32 message)
{
	if (message == HIT_MESSAGE_SUPER_HIP_DROP) {
		startBck("fence_revolve_outer_shake");
		mInner->startBck("fence_revolve_inner_shake");
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

	mMapCollisionManager->setUpUnk8TRS(mPosition, mRotation, mScaling);

	TRevolvingFenceInner* inner;
	if (mIsBamboo) {
		inner = (TRevolvingFenceInner*)TMapObjBaseManager::newAndRegisterObj(
		    "bambooFence_revolve_inner", mPosition, mRotation,
		    JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
	} else {
		inner = (TRevolvingFenceInner*)TMapObjBaseManager::newAndRegisterObj(
		    "fence_revolve_inner", mPosition, mRotation,
		    JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
	}

	mInner = inner;
	mInner->appear();
}

f32 TRevolvingFenceInner::mSpeed = 4.0f;

BOOL TRevolvingFenceInner::receiveMessage(THitActor* sender, u32 message)
{
	if (message == HIT_MESSAGE_SUPER_HIP_DROP && !mIsWall) {
		if (isState(STATE_WAIT_FRONT)) {
			gpMSound->startSoundActor(MSD_SE_OBJ_FENCE_REVERSE1, &mPosition, 0,
			                          nullptr, 0, 4);
			setState(STATE_TURN_TO_BACK_CW);
			startBck("fence_revolve_inner_roll_down");
			offMapObjFlag(MAP_OBJ_FLAG_UNK100);
			return TRUE;
		}

		if (isState(STATE_WAIT_BACK)) {
			gpMSound->startSoundActor(MSD_SE_OBJ_FENCE_REVERSE2, &mPosition, 0,
			                          nullptr, 0, 4);
			setState(STATE_TURN_TO_FRONT_CW);
			startBck("fence_revolve_inner_roll_up");
			offMapObjFlag(MAP_OBJ_FLAG_UNK100);
			return TRUE;
		}
	}

	if (message == HIT_MESSAGE_SUPER_HIP_DROP && mIsWall) {
		// Which side of the panel Mario is standing on decides the turn
		// direction, so that he always gets pushed away from the fence.
		f32 toMario = 180.0f * (getRotYFromAxisZ(SMS_GetMarioPos()) / 3.14f)
		    + mInitialRotation.y;
		toMario = MsWrap(toMario, -180.0f, 180.0f);

		if ((-180.0f < toMario && toMario < -90.0f)
		    || (0.0f < toMario && toMario < 90.0f)) {
			gpMSound->startSoundActor(MSD_SE_OBJ_FENCE_REVERSE1, &mPosition, 0,
			                          nullptr, 0, 4);
			if (isState(STATE_WAIT_FRONT))
				setState(STATE_TURN_TO_BACK_CW);
			else
				setState(STATE_TURN_TO_FRONT_CW);
		} else {
			gpMSound->startSoundActor(MSD_SE_OBJ_FENCE_REVERSE2, &mPosition, 0,
			                          nullptr, 0, 4);
			if (isState(STATE_WAIT_FRONT))
				setState(STATE_TURN_TO_BACK_CCW);
			else
				setState(STATE_TURN_TO_FRONT_CCW);
		}

		return TRUE;
	}

	return FALSE;
}

// UNUSED (0x10c, size-exact). The tail every controlWall case repeats; MWCC
// inlines it into all four of them, and the out-of-line copy expands
// MsMtxSetRotY where the inlined ones keep the call.
void TRevolvingFenceInner::calcCurrentMtx()
{
	mRotation.y = mAngle + mInitialRotation.y;
	// The result is thrown away in retail too: the wrap never lands back in
	// mRotation.y, which is why MWCC keeps MsWrap out of line here.
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
	case STATE_WAIT_FRONT:
	case STATE_WAIT_BACK:
		break;

	case STATE_TURN_TO_BACK_CW:
		mAngle += mSpeed;
		if (mAngle > 180.0f) {
			mAngle      = 180.0f;
			mRotation.y = mAngle + mInitialRotation.y;
			setState(STATE_WAIT_BACK);
		}
		calcCurrentMtx();
		break;

	case STATE_TURN_TO_FRONT_CW:
		mAngle += mSpeed;
		if (mAngle > 360.0f) {
			mAngle      = 0.0f;
			mRotation.y = mAngle + mInitialRotation.y;
			setState(STATE_WAIT_FRONT);
		}
		calcCurrentMtx();
		break;

	case STATE_TURN_TO_BACK_CCW:
		mAngle -= mSpeed;
		if (mAngle < -180.0f) {
			mAngle      = 180.0f;
			mRotation.y = mAngle + mInitialRotation.y;
			setState(STATE_WAIT_BACK);
		}
		calcCurrentMtx();
		break;

	case STATE_TURN_TO_FRONT_CCW:
		mAngle -= mSpeed;
		if (mAngle < 0.0f) {
			mAngle      = 0.0f;
			mRotation.y = mAngle + mInitialRotation.y;
			setState(STATE_WAIT_FRONT);
		}
		calcCurrentMtx();
		break;
	}
}

// Binding level over a raw member read, worth +16 of low region in
// TRevolvingFenceInner::controlGroundRoof (batch 127).
static inline MActor* MapObjFenceMActorL0(const TRevolvingFenceInner* p)
{
	MActor* mActor = p->mMActor;
	return mActor;
}

static inline MActor* MapObjFenceMActor(const TRevolvingFenceInner* p)
{
	MActor* mActor = MapObjFenceMActorL0(p);
	return mActor;
}

void TRevolvingFenceInner::controlGroundRoof()
{
	switch (mState) {
	case STATE_WAIT_FRONT:
	case STATE_WAIT_BACK:
		break;

	case STATE_TURN_TO_BACK_CW:
	case STATE_TURN_TO_BACK_CCW:
		if (MapObjFenceMActor(this)->curAnmEndsNext(0, nullptr)) {
			setState(STATE_WAIT_BACK);
			mMActor->setFrameRate(0.0f, 0);
			mMActor->getFrameCtrl(0)->setFrame(0.0f);
			mMActor->calc();
			onMapObjFlag(MAP_OBJ_FLAG_UNK100);
		}
		break;

	case STATE_TURN_TO_FRONT_CW:
	case STATE_TURN_TO_FRONT_CCW:
		if (mMActor->curAnmEndsNext(0, nullptr)) {
			setState(STATE_WAIT_FRONT);
			mMActor->setFrameRate(0.0f, 0);
			mMActor->getFrameCtrl(0)->setFrame(0.0f);
			mMActor->calc();
			onMapObjFlag(MAP_OBJ_FLAG_UNK100);
		}
		break;
	}
}

void TRevolvingFenceInner::setGroundCollision()
{
	// While Yoshi stands in the panel's footprint the collision follows the
	// spinning model matrix instead of the object's own SRT.
	if (SMS_GetYoshi()->isHatched()
	    && getPosition().x - mBodyRadius < SMS_GetYoshi()->getTranslation().x
	    && mPosition.x + mBodyRadius > SMS_GetYoshi()->getTranslation().x
	    && mPosition.z - mBodyRadius < SMS_GetYoshi()->getTranslation().z
	    && mPosition.z + mBodyRadius > SMS_GetYoshi()->getTranslation().z) {
		JGeometry::TMatrix34<JGeometry::SMatrix34C<f32> > mtx;
		mtx.set(getModel()->getAnmMtx(0));
		if (mMapCollisionManager->unk8)
			mMapCollisionManager->unk8->moveMtx(mtx);
	}

	TMapObjBase::setGroundCollision();
}

void TRevolvingFenceInner::control()
{
	TMapObjBase::control();

	if (mIsWall)
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
		mIsWall = 1;
	else
		mIsWall = 0;

	mMapCollisionManager->setUpUnk8TRS(mPosition, mRotation, mScaling);
}

f32 TFenceWater::mWaterAccel     = 2.1f;
f32 TFenceWater::mBackSpeed      = 3.0f;
int TFenceWater::mTurnedWaitTime = 600;

void TFenceWater::draw() const { }

BOOL TFenceWater::receiveMessage(THitActor* sender, u32 message)
{
	if (!isState(STATE_TURNED) && message == HIT_MESSAGE_SPRAYED_BY_WATER) {
		mTurnSpeed = mWaterAccel;
		if (mTurnSpeed > 0.0f)
			changeStatusToGo();
		return TRUE;
	}

	return FALSE;
}

void TFenceWater::changeStatusToGo()
{
	gpMSound->startSoundActor(MSD_SE_OBJ_WATER_FENCE_FW, &mPosition);
	setState(STATE_GO);
}

void TFenceWater::changeStatusToWait()
{
	mTurnAngle = 0.0f;
	mTurnSpeed = 0.0f;
	setState(STATE_WAIT);
}

// Binding level over a raw member read, worth +16 of low region in
// TFenceWater::controlRotation (batch 127).
static inline f32 MapObjFenceTurnAngle(const TFenceWater* p)
{
	f32 turnAngle = p->mTurnAngle;
	return turnAngle;
}

void TFenceWater::controlRotation()
{
	switch (mState) {
	case STATE_WAIT:
		break;

	case STATE_GO:
		mTurnAngle -= mTurnSpeed;
		if (MapObjFenceTurnAngle(this) <= -90.0f) {
			mTurnAngle  = -90.0f;
			mTurnSpeed  = 0.0f;
			setState(STATE_TURNED);
			startStateTimer(mTurnedWaitTime);
		}
		break;

	case STATE_TURNED:
		if (!isStateTimerEngaged()) {
			gpMSound->startSoundActor(MSD_SE_OBJ_WATER_FENCE_REV, &mPosition, 0,
			                          nullptr, 0, 4);
			mTurnSpeed = mBackSpeed;
			setState(STATE_BACK);
		}
		break;

	case STATE_BACK:
		mTurnAngle += mTurnSpeed;
		if (mTurnAngle >= 0.0f)
			changeStatusToWait();
		break;
	}
}

void TFenceWater::control()
{
	TMapObjBase::control();
	controlRotation();

	mRotation.y = MsWrap(mTurnAngle + mInitialRotation.y, 0.0f, 360.0f);

	mMessenger->mPosition.x = 500.0f * JMACos(getRotation().y) + mPosition.x;
	mMessenger->mPosition.z = mPosition.z - 500.0f * JMASin(mRotation.y);
}

void TFenceWater::initMapCollisionData() { TMapObjBase::initMapCollisionData(); }

void TFenceWater::initMapObj()
{
	TFence::initMapObj();

	mMessenger = new TMapObjMessenger("地形オブジェメッセンジャー");
	mMessenger->unk68 = this;
	mMessenger->initHitActor(getActorType(), 1, 0, 0.0f, 0.0f, 100.0f,
	                         300.0f);
	mMessenger->offHitFlag(HIT_FLAG_NO_COLLISION);
	mMessenger->mPosition.set(mPosition.x, mPosition.y - 150.0f, mPosition.z);

	JDrama::TNameRefGen::search<TIdxGroupObj>("オブジェクトグループ")
	    ->getChildren()
	    .push_back(mMessenger);
}

void TFenceWaterH::control()
{
	TMapObjBase::control();
	controlRotation();

	mRotation.z = MsWrap(mTurnAngle + mInitialRotation.z, 0.0f, 360.0f);

	JGeometry::TPosition3<JGeometry::TMatrix34<JGeometry::SMatrix34C<f32> > >
	    mtx;
	mtx.identity();
	mtx.setEular(0.0f, 0.017453294f * mRotation.y, 0.0f);

	JGeometry::TRotation3<JGeometry::TMatrix34<JGeometry::SMatrix34C<f32> > >
	    spin;
	spin.identity();
	spin.setEular(0.0f, 0.0f, 0.017453294f * mRotation.z);

	MTXConcat(mtx, spin, mtx);
	mtx.setTrans(mPosition.x, mPosition.y, mPosition.z);
	MTXCopy(mtx, getModel()->getAnmMtx(0));

	mMessenger->mPosition.x = mPosition.x;
	mMessenger->mPosition.y = mPosition.y - 150.0f;
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

f32 TRailFence::mFallHeight = 50000.0f;
int TRailFence::mWaitTime   = 240;

BOOL TRailFence::receiveMessage(THitActor* sender, u32 message)
{
	if (message == HIT_MESSAGE_SUPER_HIP_DROP) {
		gpMSound->startSoundActor(MSD_SE_OBJ_MVING_FENCT_PNCH, &mPosition);
		setUpMapCollision(1);
		offMapObjFlag(MAP_OBJ_FLAG_UNK100);
		setState(STATE_RUN);
		return TRUE;
	}

	return FALSE;
}

// UNUSED (0x11c): the STATE_FALL arm of control(), inlined there.
void TRailFence::falling()
{
	JGeometry::TVec3<f32> velocity = mVelocity;
	mPosition.y += velocity.y;

	mVelocity.y -= mGravity;
	if (mVelocity.y < -100.0f)
		mVelocity.y = -100.0f;

	if (mPosition.y < mInitialPosition.y - mFallHeight) {
		mPosition.x = mInitialPosition.x;
		mPosition.y = mInitialPosition.y;
		mPosition.z = mInitialPosition.z;
		setUpMapCollision(0);
		mTracer->setTo(
		    mTracer->getGraph()->findNearestNodeIndex(mPosition, 0xffffffff));
		makeObjAppeared();
		calcRootMatrix();
		getModel()->calc();
		onMapObjFlag(MAP_OBJ_FLAG_UNK100);
	}
}

void TRailFence::goOnRail()
{
	if (mTracer->getGraph()) {
		JGeometry::TVec3<f32> toNode = mTracer->getCurrentPos();
		toNode.x -= mPosition.x;
		toNode.y -= mPosition.y;
		toNode.z -= mPosition.z;

		if (toNode.x * toNode.x + toNode.y * toNode.y + toNode.z * toNode.z
		    < 50.0f) {
			TGraphTracer* tracer = mTracer;
			TRailNode* node      = tracer->getCurrent().getRailNode();
			if (node->mConnectionNum == 0 && (node->mFlags & 8)) {
				gpMSound->startSoundActor(MSD_SE_OBJ_MVING_FENCT_SET,
				                          &mPosition, 0, nullptr, 0, 4);
				startStateTimer(mWaitTime);
				startAnim(1);
				setState(STATE_AT_GOAL);
				return;
			}

			tracer->moveToShortestNext();
			toNode.set(mTracer->getCurrentPos());
		}

		gpMSound->startSoundActor(MSD_SE_OBJ_MVING_FENCE_MOVE, &mPosition, 0,
		                          nullptr, 0, 4);
		VECNormalize(toNode, toNode);
		toNode.scale(mMoveSpeed);
		mLinearVelocity.add(toNode);
	}
}

void TRailFence::control()
{
	TMapObjBase::control();

	switch (mState) {
	case STATE_WAIT:
		break;

	case STATE_RUN:
		goOnRail();
		break;

	case STATE_AT_GOAL:
		if (!isStateTimerEngaged()) {
			removeMapCollision();
			gpMSound->startSoundActor(MSD_SE_OBJ_SUPERBLOCK_BREAK, &mPosition,
			                          0, nullptr, 0, 4);
			setState(STATE_FALL);
		}
		break;

	case STATE_FALL:
		// falling()'s body, spelled out: MWCC refuses to inline the helper at
		// its size and retail has no call here.
		{
			JGeometry::TVec3<f32> velocity = mVelocity;
			mPosition.y += velocity.y;

			mVelocity.y -= mGravity;
			if (mVelocity.y < -100.0f)
				mVelocity.y = -100.0f;

			if (mPosition.y < mInitialPosition.y - mFallHeight) {
				mPosition.x = mInitialPosition.x;
				mPosition.y = mInitialPosition.y;
				mPosition.z = mInitialPosition.z;
				setUpMapCollision(0);
				mTracer->setTo(
				    mTracer->getGraph()->findNearestNodeIndex(mPosition,
				                                              0xffffffff));
				makeObjAppeared();
				calcRootMatrix();
				getModel()->calc();
				onMapObjFlag(MAP_OBJ_FLAG_UNK100);
			}
		}
		break;
	}
}

void TRailFence::initMapCollisionData() { TMapObjBase::initMapCollisionData(); }

void TRailFence::load(JSUMemoryInputStream& stream)
{
	TMapObjBase::load(stream);

	char graphName[64];
	stream.readString(graphName, 64);

	TGraphWeb* graph = gpConductor->getGraphByName(graphName);
	if (graph && !graph->isDummy()) {
		mTracer->setGraph(graph);
		mTracer->setTo(graph->findNearestNodeIndex(mPosition, 0xffffffff));
	}

	mMoveSpeed = 8.0f;
	mGravity   = 0.3f;
}

TRailFence::TRailFence(const char* name)
    : TFence(name)
    , mTracer(new TGraphTracer)
    , mMoveSpeed(0.0f)
{
}
