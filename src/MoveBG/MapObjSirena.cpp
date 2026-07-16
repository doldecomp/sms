#include <MoveBG/MapObjSirena.hpp>
#include <Enemy/Conductor.hpp>
#include <Map/Map.hpp>
#include <MSound/MSound.hpp>
#include <MSound/SoundEffects.hpp>
#include <Player/MarioAccess.hpp>
#include <M3DUtil/MActorData.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <Strategic/ObjModel.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <System/Application.hpp>
#include <System/MarDirector.hpp>
#include <math.h>

// Definition order in this file is the REVERSE of the order symbols appear in
// mario.MAP, because MoveBG is compiled with -inline deferred.

static TSirenaRollMapObj* gpCurObject;

static int partsRollCallback(J3DNode* node, int flag)
{
	if (flag == 0) {
		if (gpCurObject == nullptr)
			return 1;
		int jntNo     = ((J3DJoint*)node)->getJntNo();
		MtxPtr jntMtx = &gpCurObject->getModel()->getAnmMtx(jntNo)[0];
		int idx       = jntNo - 1;
		Mtx rot;
		Mtx scale;
		scale[0][3] = 0.0f;
		scale[1][3] = 0.0f;
		scale[2][3] = 0.0f;
		scale[0][0] = gpCurObject->mScaling.x;
		scale[0][1] = 0.0f;
		scale[0][2] = 0.0f;
		scale[1][0] = 0.0f;
		scale[1][1] = gpCurObject->mScaling.y;
		scale[1][2] = 0.0f;
		scale[2][0] = 0.0f;
		scale[2][1] = 0.0f;
		scale[2][2] = gpCurObject->mScaling.z;
		f32 rollZ = gpCurObject->getRollAngZ(idx);
		f32 rollY = gpCurObject->getRollAngY(idx);
		f32 rollX = gpCurObject->getRollAngX(idx);
		MsMtxSetRotRPH(rot, rollX, rollY, rollZ);
		PSMTXConcat(jntMtx, rot, jntMtx);
		PSMTXConcat(jntMtx, scale, jntMtx);
		PSMTXConcat(J3DSys::mCurrentMtx, rot, J3DSys::mCurrentMtx);
		PSMTXConcat(J3DSys::mCurrentMtx, scale, J3DSys::mCurrentMtx);
	}
	return 1;
}

TSirenaRollMapObj::TSirenaRollMapObj(const char* name)
    : TMapObjBase(name)
    , unk138(nullptr)
    , unk13C(nullptr)
    , unk148(0)
    , unk14C(0.0f)
    , unk150(0.0f)
    , unk154(1.0f)
    , unk158(10.0f)
    , unk15C(0.1f)
    , unk160(0.2f)
    , unk164(1)
{
	gpCurObject = nullptr;
}

TDonchou::TDonchou(const char* name)
    : TMapObjBase(name)
    , unk138(nullptr)
    , unk13C(0)
    , unk140(0.0f)
    , unk14C(0)
{
}

void TDonchou::initMapObj()
{
	unk140 = 0.0f;
	unk13C = 0;
	unk148 = nullptr;
	unk144 = nullptr;
	TMapObjBase::initMapObj();
	getModel();
	Mtx mtx;
	MsMtxSetXYZRPH(mtx, mPosition.x, 2.0f * unk140 + mPosition.y, mPosition.z,
	               mRotation.x, mRotation.y, mRotation.z);
	unk138 = new TMapCollisionWarp();
	unk138->init("/mapObj/Donchou", 0, this);
	PSMTXCopy(mtx, unk138->unk20);
	unk138->setUp();
}

void TDonchou::loadAfter()
{
	TMapObjBase::loadAfter();
	if (gpApplication.mCurrArea.unk0 == 14 && gpMarDirector->unk7D == 0) {
		unk144 = JDrama::TNameRefGen::search<TSlotDrum>("srotdram");
		unk148 = JDrama::TNameRefGen::search<TItemSlotDrum>("itemsrotdram");
	}
}

u32 TDonchou::touchWater(THitActor* water)
{
	if (fabsf(mPosition.z - water->mPosition.z) < 50.0f)
		return 1;
	return 0;
}

TCloset::TCloset(const char* name)
    : TSirenaRollMapObj(name)
    , mMapCollisionWarp(nullptr)
    , unk16C(false)
    , unk16D(0)
{
}

void TCloset::initMapObj()
{
	unk148 = 4;
	unk14C = 0.0f;
	unk150 = mPosition.y;
	unk154 = 2.5f;
	unk158 = 8.0f;
	unk15C = 0.1f;
	unk160 = 0.5f;
	unk164 = 0;
	unk16C = false;
	unk16D = 0;
	unk138 = new f32[unk148];
	unk13C = new f32[unk148];
	for (int i = 0; i < unk148; i++) {
		unk138[i] = 0.0f;
		unk13C[i] = 180.0f;
	}
	TMapObjBase::initMapObj();
	for (int i = 1; i <= unk148; i++)
		mMActor->setJointCallback(i, partsRollCallback);
	unk140 = 0.25f * mDamageRadius;
	unk144 = mDamageHeight;
	getModel();
	Mtx mtx;
	MsMtxSetXYZRPH(mtx, mPosition.x, 2.0f * unk14C + mPosition.y, mPosition.z,
	               mRotation.x, mRotation.y, mRotation.z);
	mMapCollisionWarp = new TMapCollisionWarp();
	mMapCollisionWarp->init("/mapObj/Closet", 0, this);
	PSMTXCopy(mtx, mMapCollisionWarp->unk20);
	mMapCollisionWarp->setUp();
	initAnmSound();
}

TSakuCasino::TSakuCasino(const char* name)
    : TMapObjBase(name)
    , unk138(nullptr)
    , unk13C(0)
    , unk140(0.0f)
    , unk144(nullptr)
{
}

void TSakuCasino::initMapObj()
{
	unk140 = 0.0f;
	unk13C = 0;
	TMapObjBase::initMapObj();
	getModel();
	Mtx mtx;
	MsMtxSetXYZRPH(mtx, mPosition.x, 2.0f * unk140 + mPosition.y, mPosition.z,
	               mRotation.x, mRotation.y, mRotation.z);
	unk138 = new TMapCollisionWarp();
	unk138->init("/mapObj/SakuCasino", 0, this);
	PSMTXCopy(mtx, unk138->unk20);
	unk138->setUp();
}

void TSakuCasino::loadAfter()
{
	TMapObjBase::loadAfter();
	unk144 = JDrama::TNameRefGen::search<TCasinoPanelGate>("pazul");
}

void TSakuCasino::calcRootMatrix()
{
	J3DModel* model = getModel();
	Mtx mtx;
	MsMtxSetXYZRPH(mtx, mPosition.x, mPosition.y + unk140, mPosition.z,
	               mRotation.x, mRotation.y, mRotation.z);
	PSMTXCopy(mtx, model->getBaseTRMtx());
	model->setBaseScale(mScaling);
	mtx[1][3] += unk140;
	if (unk144 != nullptr && unk144->unk16D != 0) {
		unk13C = 1;
		unk138->remove();
	}
	if (unk13C != 0) {
		unk140 -= 1.0f;
		mScaling.y *= 0.99f;
	}
}

void TSirenabossWall::initMapObj()
{
	TMapObjBase::initMapObj();
	mMActor->offMakeDL();
	MActorAnmData* anmData = mMActorKeeper->getMActorAnmData();
	mMultiBtk               = new TMultiBtk(3, getModel()->getModelData());
	for (int i = 0; i <= 2; i++) {
		J3DAnmTextureSRTKey* data;
		if (i < anmData->getUnk38()->getAnmNum())
			data = (J3DAnmTextureSRTKey*)anmData->getUnk38()->unkC[i];
		else
			data = nullptr;
		mMultiBtk->setNthData(i, data);
	}
}

void TSirenabossWall::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_CALC_ANIM)
		mMultiBtk->update();
	TMapObjBase::perform(cue, graphics);
}

void TSirenabossWall::drawObject(JDrama::TGraphics* graphics)
{
	mMActor->getModel()->calc();
}

void TSirenaCasinoRoof::initMapObj()
{
	TMapObjBase::initMapObj();
	mMActor->offMakeDL();
	MActorAnmData* anmData = mMActorKeeper->getMActorAnmData();
	mMultiBtk               = new TMultiBtk(3, getModel()->getModelData());
	for (int i = 0; i <= 2; i++) {
		J3DAnmTextureSRTKey* data;
		if (i < anmData->getUnk38()->getAnmNum())
			data = (J3DAnmTextureSRTKey*)anmData->getUnk38()->unkC[i];
		else
			data = nullptr;
		mMultiBtk->setNthData(i, data);
	}
	mMActor->setBrk("casino_lighting");
}

void TSirenaCasinoRoof::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_CALC_ANIM)
		mMultiBtk->update();
	TMapObjBase::perform(cue, graphics);
}

void TWarpAreaActor::perform(u32 cue, JDrama::TGraphics* graphics)
{
	THitActor::perform(cue, graphics);
	if (cue & CUE_MOVE) {
		if (getColNum() != 0) {
			if (*gpMarioSpeedY > 0.0f) {
				if (unk68 != -1)
					gpMap->changeModel(unk68);
			}
			if (*gpMarioSpeedY < 0.0f) {
				if (unk6A != -1)
					gpMap->changeModel(unk6A);
			}
		}
	}
}

void TWarpAreaActor::load(JSUMemoryInputStream& stream)
{
	THitActor::load(stream);
	u32 v;
	stream.read(&v, 4);
	unk68 = v;
	stream.read(&v, 4);
	unk6A = v;
	initHitActor(0x4000019D, 1, -0x80000000, 100.0f * mScaling.x,
	             100.0f * mScaling.y, 0.0f, 0.0f);
	offHitFlag(HIT_FLAG_NO_COLLISION);
	gpConductor->registerOtherObj(this);
}

TWarpAreaActor::TWarpAreaActor(const char* name)
    : THitActor(name)
    , unk68(-1)
    , unk6A(-1)
{
}

u32 TChestRevolve::touchWater(THitActor* actor)
{
	if (isState(1)) {
		mState = 2;
		startAnim(1);
		setUpMapCollision(1);
	}
	return 1;
}

void TChestRevolve::control()
{
	TMapObjBase::control();
	switch (mState) {
	case 2:
		if (animIsFinished()) {
			mState = 1;
			setUpMapCollision(0);
		}
		break;
	}
}

BOOL TPanelRevolve::receiveMessage(THitActor* actor, u32 message)
{
	if (isState(1)) {
		gpMSound->startSoundActor(MSD_SE_OBJ_PANEL_ROLL, &mPosition, 0,
		                          nullptr, 0, 4);
		mState = 2;
		startAnim(1);
		removeMapCollision();
	}
	return 1;
}

void TPanelRevolve::touchPlayer(THitActor* actor)
{
	if (marioHipAttack() && isState(1)) {
		gpMSound->startSoundActor(MSD_SE_OBJ_PANEL_ROLL, &mPosition, 0,
		                          nullptr, 0, 4);
		mState = 2;
		startAnim(1);
		removeMapCollision();
	}
}

void TPanelRevolve::control()
{
	TMapObjBase::control();
	switch (mState) {
	case 2:
		if (animIsFinished()) {
			mState = 1;
			setUpCurrentMapCollision();
		}
		break;
	}
}
