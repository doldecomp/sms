#include <MoveBG/MapObjTree.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <Camera/CameraShake.hpp>
#include <Map/MapCollisionManager.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <Map/MapData.hpp>
#include <Map/MapEventSink.hpp>
#include <Map/PollutionManager.hpp>
#include <MarioUtil/PacketUtil.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <MSound/MSound.hpp>
#include <Player/MarioAccess.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/MarDirector.hpp>

#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>

static int sWaitTime                      = 1;
f32 TMapObjTreeScale::mScaleMin           = 0.1f;
f32 TMapObjTreeScale::mScaleSpeedXZ       = 0.007f;
f32 TMapObjTreeScale::mStatusChangeScaleY = 0.3f;
f32 TMapObjTreeScale::mScaleSpeedY        = 0.005f;
f32 TMapObjTree::mBananaTreeJumpPower     = 1000.0f;

TMapObjLeaf::TMapObjLeaf()
    : unk0(0.0f)
    , unk4(0.0f)
    , unk8(nullptr)
{
	unkC.identity();
	unk8 = new TMapCollisionMove();
}

void TMapObjTree::touchPlayer(THitActor* player)
{
	unk158   = false;
	s16 data = SMS_GetMarioGroundPlane()->getData();
	if (!(SMS_GetMarioGroundPlane()->getActor() == this && data >= 0
	      && data < unk150)) {
		return;
	}
	if (marioHipAttack()) {
		unk154[data].unk4 += unk160;
	} else if (marioIsOn()) {
		unk154[data].unk4 += unk15C;
	}
}

int TMapObjTree::controlLeaf(int index)
{
	TMapObjLeaf& leaf = unk154[index];

	if (leaf.unk4 == 0.0f) {
		// This feels wrong, probably wrapper function?
		// Was likely not SMS_GetMarioSpeedY since that takes a THitActor
		if (*gpMarioSpeedY <= 0.0f) {
			JGeometry::TMatrix34<JGeometry::SMatrix34C<f32> > mtx;
			mtx.set(leaf.unkC);
			leaf.unk8->moveMtx(mtx);
		}
		return 1;
	}

	leaf.unk0 += leaf.unk4;
	leaf.unk4 -= leaf.unk0 * unk164;
	leaf.unk4 *= unk168;

	JGeometry::TMatrix34<JGeometry::SMatrix34C<f32> > rotation;
	rotation.identity();
	JGeometry::TMatrix34<JGeometry::SMatrix34C<f32> > mtx;
	JGeometry::TVec3<f32> axis(1.0f, 0.0f, 0.0f);
	PSMTXRotAxisRad(rotation, axis, leaf.unk0);

	mtx.set(leaf.unkC);
	MTXConcat(mtx, rotation, mtx);
	MTXCopy(mtx, getModel()->getAnmMtx(unk150 - index));

	if (*gpMarioSpeedY <= 0.0f)
		leaf.unk8->moveMtx(mtx);

	// TODO: There is a weird bge thing here
	// I could not get a unary matching, probably skill issue
	if (fabs(leaf.unk0) < unk15C)
		return 1;
	return 0;
}

void TMapObjTree::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (!unk158 && (cue & CUE_MOVE)) {
		int count = 0;
		for (int i = 0; i < unk150; ++i)
			count += controlLeaf(i);

		if (mColCount == 0 && count == unk150)
			unk158 = true;
	}

	TMapObjGeneral::perform(cue, graphics);
}

void TMapObjTree::initEach()
{
	switch (mActorType) {
	case 0x40000034:
	case 0x40000038:
		unk148 = 20.0f;
		unk14C = 95.0f;
		unk150 = 12;
		unk15C = 0.001f;
		unk160 = 0.006f;
		unk164 = 0.01f;
		unk168 = 0.97f;
		return;
	case 0x40000035:
		unk148 = 20.0f;
		unk14C = 100.0f;
		unk150 = 8;
		unk15C = 0.001f;
		unk160 = 0.006f;
		unk164 = 0.01f;
		unk168 = 0.97f;
		return;
	case 0x40000036:
		unk148 = 50.0f;
		unk14C = 100.0f;
		unk150 = 12;
		unk15C = 0.001f;
		unk160 = 0.006f;
		unk164 = 0.01f;
		unk168 = 0.97f;
		return;
	case 0x40000037:
		unk148 = 95.0f;
		unk14C = 60.0f;
		unk150 = 8;
		unk15C = 0.001f;
		unk160 = 0.006f;
		unk164 = 0.01f;
		unk168 = 0.97f;
		return;
	case 0x40000039:
		unk148 = 70.0f;
		unk14C = 100.0f;
		unk150 = 8;
		unk15C = 0.004f;
		unk160 = 0.008f;
		unk164 = 0.03f;
		unk168 = 0.9f;
		return;
	}
	return;
}

void TMapObjTree::initMapObj()
{
	TMapObjGeneral::initMapObj();
	initEach();
	unk154 = new TMapObjLeaf[unk150];
	for (s32 i = 0; i < unk150; ++i) {
		TMapObjLeaf& leaf = unk154[i];
		leaf.unk8         = new TMapCollisionMove();
		char buffer[64];
		if (isActorType(0x40000038)) {
			snprintf(buffer, 0x100, "/mapObj/palmLeaf%02d", i + 1);
		} else {
			snprintf(buffer, 0x100, "/mapObj/%sLeaf%02d", getUnkF4(), i + 1);
		}
		leaf.unk8->init(buffer, 0, this);
		leaf.unk8->setAllData(i);
		leaf.unk8->remove();

		leaf.unkC.set(getModel()->getAnmMtx(unk150 - i));
		TMapCollisionMove* collision = leaf.unk8;
		collision->setMtx(leaf.unkC);
		collision->setUp();
	}

	if (getMapCollisionManager() != nullptr) {
		getMapCollisionManager()->unk10 = nullptr;
	}
}

TMapObjTree::TMapObjTree(const char* name)
    : TMapObjGeneral(name)
    , unk148(0.0f)
    , unk14C(0.0f)
    , unk150(0)
    , unk154(nullptr)
    , unk158(true)
    , unk15C(0.0f)
    , unk160(0.0f)
    , unk164(0.0f)
    , unk168(0.0f)
    , unk16C(0)
{
}

u32 TMapObjTreeScale::touchWater(THitActor* water)
{
	if (mScaling.x == 1.0f)
		return TMapObjGeneral::touchWater(water);

	if (isState(STATE_UNKB))
		startScaleUp();

	return 1;
}

void TMapObjTreeScale::control()
{
	switch (mState) {
	case STATE_UNKB:
		if (gpMarDirector->getCurrentMap() == 4)
			break;
		if (gpPollution->isPolluted(mPosition.x, mPosition.y, mPosition.z))
			break;
		startScaleUp();
		break;

	case STATE_UNKC:
		gpMSound->startSoundActor(MSD_SE_OBJ_TREE_APPEAR, &mPosition, 0,
		                          nullptr, 0, 4);
		mScaling.y += mScaleSpeedY;
		if (mScaling.y > mStatusChangeScaleY)
			mState = STATE_UNKD;
		break;

	case STATE_UNKD:
		gpMSound->startSoundActor(MSD_SE_OBJ_TREE_APPEAR, &mPosition, 0,
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

	if (!isState(STATE_UNKC) && !isState(STATE_UNKD))
		return;

	setObjHitData(0);
	if (gpMarDirector->mMap != 2
	    || (!gpMarDirector->isDemoModeNow()
	        && (unk2E0 == nullptr || unk2E0->isBuried(1)))) {
		SMSRumbleMgr->start(0x13, &mPosition);
		gpCameraShake->keepShake(CAM_SHAKE_MODE_UNK5, 1.0f);
	}

	if (unk2DC > sWaitTime) {
		// circular buffer of particle positions?
		unk170[unk2D8].set(
		    mPosition.x + 400.0f * (rand() * (1.0f / 32768.0f)) - 200.0f,
		    mPosition.y,
		    mPosition.z + 400.0f * (rand() * (1.0f / 32768.0f)) - 200.0f);

		gpMarioParticleManager->emit(0x1db, &unk170[unk2D8], 2, this);
		++unk2D8;
		if (unk2D8 >= 30)
			unk2D8 = 0;
		unk2DC = 0;
	}
	++unk2DC;
}

inline void TMapObjTreeScale::beSmall()
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
	mActorType = 0x4000003b;
	mState     = STATE_UNKB;
	SMS_HideAllShapePacket(getModel());
}

void TMapObjTreeScale::loadAfter()
{
	TMapObjGeneral::loadAfter();

	if (gpMarDirector->getCurrentMap() == 4
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
    , unk2D8(0)
    , unk2DC(0)
    , unk2E0(nullptr)
{
	for (int i = 0; i < 30; ++i)
		unk170[i].zero();
}
