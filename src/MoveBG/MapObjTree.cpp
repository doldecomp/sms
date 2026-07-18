#include <MoveBG/MapObjTree.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <macros.h>

#include <Camera/CameraShake.hpp>
#include <Map/MapCollisionManager.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <Map/MapData.hpp>
#include <Map/MapEventSink.hpp>
#include <Map/PollutionManager.hpp>
#include <MarioUtil/PacketUtil.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MSound/MSound.hpp>
#include <Player/MarioAccess.hpp>
#include <System/Particles.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/MarDirector.hpp>

#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

static int sWaitTime                      = 1;
f32 TMapObjTreeScale::mScaleMin           = 0.1f;
f32 TMapObjTreeScale::mScaleSpeedXZ       = 0.007f;
f32 TMapObjTreeScale::mStatusChangeScaleY = 0.3f;
f32 TMapObjTreeScale::mScaleSpeedY        = 0.005f;
f32 TMapObjTree::mBananaTreeJumpPower     = 1000.0f;

TMapObjLeaf::TMapObjLeaf()
    : mAngle(0.0f)
    , mAngularVelocity(0.0f)
    , mCollision(nullptr)
{
	mTransform.identity();
	mCollision = new TMapCollisionMove();
}

void TMapObjTree::touchPlayer(THitActor* player)
{
	mFreezeLeaves = false;
	s16 data      = SMS_GetMarioGroundPlane()->getData();
	if (!(SMS_GetMarioGroundPlane()->getActor() == this && data >= 0
	      && data < mLeafNum)) {
		return;
	}
	if (marioHipAttack()) {
		mLeaves[data].mAngularVelocity += mLeafHipDropImpulse;
	} else if (marioIsOn()) {
		mLeaves[data].mAngularVelocity += mLeafTouchImpulse;
	}
}

int TMapObjTree::controlLeaf(int index)
{
	TMapObjLeaf& leaf = mLeaves[index];

	if (leaf.mAngularVelocity == 0.0f) {
		// This feels wrong, probably wrapper function?
		// Was likely not SMS_GetMarioSpeedY since that takes a THitActor
		if (SMS_GetMarioSpeedY() <= 0.0f) {
			TMtx34f mtx;
			mtx.set(leaf.mTransform);
			leaf.mCollision->moveMtx(mtx);
		}
		return 1;
	}

	// Integrate the spring equation with damping
	leaf.mAngle += leaf.mAngularVelocity;
	leaf.mAngularVelocity -= leaf.mAngle * mLeafStiffness;
	leaf.mAngularVelocity *= mLeafDamping;

	TMtx34f rotation;
	rotation.identity();
	TMtx34f mtx;
	JGeometry::TVec3<f32> axis(1.0f, 0.0f, 0.0f);
	MTXRotAxisRad(rotation, axis, leaf.mAngle);

	mtx.set(leaf.mTransform);
	MTXConcat(mtx, rotation, mtx);
	getModel()->setAnmMtx(mLeafNum - index, mtx);

	if (SMS_GetMarioSpeedY() <= 0.0f)
		leaf.mCollision->moveMtx(mtx);

	// BUG: they probably meant to compare both angle and velocity here, but
	// forgot to change it after copy-pasting?
	if (abs(leaf.mAngle) < mLeafTouchImpulse
	    && abs(leaf.mAngle) < mLeafTouchImpulse)
		return 1;
	return 0;
}

void TMapObjTree::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (!mFreezeLeaves && (cue & CUE_MOVE)) {
		int count = 0;
		for (int i = 0; i < mLeafNum; ++i)
			count += controlLeaf(i);

		if (mColCount == 0 && count == mLeafNum)
			mFreezeLeaves = true;
	}

	TMapObjGeneral::perform(cue, graphics);
}

void TMapObjTree::initEach()
{
	switch (mActorType) {
	case 0x40000034:
	case 0x40000038:
		mMinCanopyRadius    = 20.0f;
		mMaxCanopyRadius    = 95.0f;
		mLeafNum            = 12;
		mLeafTouchImpulse   = 0.001f;
		mLeafHipDropImpulse = 0.006f;
		mLeafStiffness      = 0.01f;
		mLeafDamping        = 0.97f;
		return;
	case 0x40000035:
		mMinCanopyRadius    = 20.0f;
		mMaxCanopyRadius    = 100.0f;
		mLeafNum            = 8;
		mLeafTouchImpulse   = 0.001f;
		mLeafHipDropImpulse = 0.006f;
		mLeafStiffness      = 0.01f;
		mLeafDamping        = 0.97f;
		return;
	case 0x40000036:
		mMinCanopyRadius    = 50.0f;
		mMaxCanopyRadius    = 100.0f;
		mLeafNum            = 12;
		mLeafTouchImpulse   = 0.001f;
		mLeafHipDropImpulse = 0.006f;
		mLeafStiffness      = 0.01f;
		mLeafDamping        = 0.97f;
		return;
	case 0x40000037:
		mMinCanopyRadius    = 95.0f;
		mMaxCanopyRadius    = 60.0f;
		mLeafNum            = 8;
		mLeafTouchImpulse   = 0.001f;
		mLeafHipDropImpulse = 0.006f;
		mLeafStiffness      = 0.01f;
		mLeafDamping        = 0.97f;
		return;
	case 0x40000039:
		mMinCanopyRadius    = 70.0f;
		mMaxCanopyRadius    = 100.0f;
		mLeafNum            = 8;
		mLeafTouchImpulse   = 0.004f;
		mLeafHipDropImpulse = 0.008f;
		mLeafStiffness      = 0.03f;
		mLeafDamping        = 0.9f;
		return;
	}
	return;
}

void TMapObjTree::initMapObj()
{
	TMapObjGeneral::initMapObj();
	initEach();
	mLeaves = new TMapObjLeaf[mLeafNum];
	for (s32 i = 0; i < mLeafNum; ++i) {
		TMapObjLeaf& leaf = mLeaves[i];
		// BUG: memory leak, mCollision was already allocated in ctor
		leaf.mCollision = new TMapCollisionMove;
		char buffer[64];
		if (isActorType(0x40000038)) {
			snprintf(buffer, 0x100, "/mapObj/palmLeaf%02d", i + 1);
		} else {
			snprintf(buffer, 0x100, "/mapObj/%sLeaf%02d", unkF4, i + 1);
		}
		leaf.mCollision->init(buffer, 0, this);
		leaf.mCollision->setAllData(i);
		leaf.mCollision->remove();

		leaf.mTransform.set(getModel()->getAnmMtx(mLeafNum - i));
		leaf.mCollision->setUpMtx(leaf.mTransform);
	}

	if (mMapCollisionManager != nullptr)
		mMapCollisionManager->unk10 = nullptr;
}

TMapObjTree::TMapObjTree(const char* name)
    : TMapObjGeneral(name)
    , mMinCanopyRadius(0.0f)
    , mMaxCanopyRadius(0.0f)
    , mLeafNum(0)
    , mLeaves(nullptr)
    , mFreezeLeaves(true)
    , mLeafTouchImpulse(0.0f)
    , mLeafHipDropImpulse(0.0f)
    , mLeafStiffness(0.0f)
    , mLeafDamping(0.0f)
    , unk16C(0)
{
}

void TMapObjTreeScale::startScaleUp()
{
	awake();
	mActorType = 0x40000039;
	removeMapCollision();
	mState = STATE_SCALING_UP_Y_ONLY;
}

u32 TMapObjTreeScale::touchWater(THitActor* water)
{
	if (mScaling.x == 1.0f)
		return TMapObjGeneral::touchWater(water);

	if (isState(STATE_SMALL))
		startScaleUp();

	return 1;
}

void TMapObjTreeScale::control()
{
	switch (mState) {
	case STATE_SMALL:
		if (SMSGetMarDirector()->getCurrentMap() != 4
		    && !gpPollution->isPolluted(mPosition.x, mPosition.y, mPosition.z))
			startScaleUp();
		break;

	case STATE_SCALING_UP_Y_ONLY:
		SMSGetMSound()->startSoundActor(MSD_SE_OBJ_TREE_APPEAR, &mPosition, 0,
		                                nullptr, 0, 4);
		mScaling.y += mScaleSpeedY;
		// TODO: does this mean that the naming scheme for map obj states is
		// actually same as Mario, "status" being the preferred term?
		if (mScaling.y > mStatusChangeScaleY)
			mState = STATE_SCALING_UP;
		break;

	case STATE_SCALING_UP:
		SMSGetMSound()->startSoundActor(MSD_SE_OBJ_TREE_APPEAR, &mPosition, 0,
		                                nullptr, 0, 4);
		if (mScaling.y < 1.0f)
			mScaling.y += mScaleSpeedY;
		else
			mScaling.y = 1.0f;

		if (mScaling.x < 1.0f) {
			mScaling.x += mScaleSpeedXZ;
			mScaling.z += mScaleSpeedXZ;
		} else {
			mScaling.x = 1.0f;
			mScaling.z = 1.0f;
			onMapObjFlag(MAP_OBJ_FLAG_UNK100);
			getModel()->calc();
			offHitFlag(HIT_FLAG_CANNOT_ATTACK);
			setUpCurrentMapCollision();
			mState = STATE_NORMAL;
		}
		break;

	default:
		TMapObjGeneral::control();
		break;
	}

	if (isState(STATE_SCALING_UP_Y_ONLY) || isState(STATE_SCALING_UP)) {
		setObjHitData(0);
		if (SMSGetMarDirector()->getCurrentMap() != 2
		    || (!SMSGetMarDirector()->isDemoModeNow()
		        && (unk2E0 == nullptr || unk2E0->isBuried(1)))) {
			SMSRumbleMgr->start(0x13, &mPosition);
			gpCameraShake->keepShake(CAM_SHAKE_MODE_UNK5, 1.0f);
		}

		if (mParticleEmitTimer > sWaitTime) {
			// circular buffer of particle positions
			mParticlePositions[mNextFreeParticlePos].set(
			    mPosition.x + 400.0f * MsRandF() - 200.0f, mPosition.y,
			    mPosition.z + 400.0f * MsRandF() - 200.0f);

			gpMarioParticleManager->emit(
			    PARTICLE_MS_RAKU_KIE, &mParticlePositions[mNextFreeParticlePos],
			    2, this);
			++mNextFreeParticlePos;
			if (mNextFreeParticlePos >= ARRAY_COUNT(mParticlePositions))
				mNextFreeParticlePos = 0;
			mParticleEmitTimer = 0;
		}
		++mParticleEmitTimer;
	}
}

void TMapObjTreeScale::beSmall()
{
	mScaling.set(mScaleMin, mScaleMin, mScaleMin);
	sleep();
	offHitFlag(HIT_FLAG_NO_COLLISION);
	onHitFlag(HIT_FLAG_CANNOT_ATTACK);
	setObjHitData(0);
	mDamageRadius = mAttackRadius;
	calcEntryRadius();
	mDamageHeight = 30.0f;
	calcEntryRadius();
	removeMapCollision();
	offMapObjFlag(MAP_OBJ_FLAG_UNK100);
	mActorType = 0x4000003B;
	mState     = STATE_SMALL;
	SMS_HideAllShapePacket(getModel());
}

void TMapObjTreeScale::loadAfter()
{
	TMapObjGeneral::loadAfter();

	if (SMSGetMarDirector()->getCurrentMap() == 4
	    || gpPollution->isPolluted(mPosition.x, mPosition.y, mPosition.z)) {
		beSmall();
	}

	// Translated: "Event (Bianco terrain sinking)"
	unk2E0 = (TMapEventSink*)JDrama::TNameRefGen::getInstance()
	             ->getRootNameRef()
	             ->search("イベント（地形沈むビアンコ）");
}

TMapObjTreeScale::TMapObjTreeScale(const char* name)
    : TMapObjTree(name)
    , mNextFreeParticlePos(0)
    , mParticleEmitTimer(0)
    , unk2E0(nullptr)
{
	for (int i = 0; i < ARRAY_COUNT(mParticlePositions); ++i)
		mParticlePositions[i].zero();
}
