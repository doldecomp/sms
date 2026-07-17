#include <MoveBG/MapObjSirena.hpp>
#include <Enemy/Conductor.hpp>
#include <Map/Map.hpp>
#include <MSound/MSound.hpp>
#include <MSound/SoundEffects.hpp>
#include <Player/MarioAccess.hpp>
#include <M3DUtil/MActorData.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/PacketUtil.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <Strategic/ObjModel.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <System/Application.hpp>
#include <System/MarDirector.hpp>
#include <System/MarioGamePad.hpp>
#include <Player/Mario.hpp>
#include <math.h>

// Definition order in this file is the REVERSE of the order symbols appear in
// mario.MAP, because MoveBG is compiled with -inline deferred.

static TSirenaRollMapObj* gpCurObject;

BOOL TRouletteSw::receiveMessage(THitActor* sender, u32 message)
{
	if (message == 1) {
		unk6C = 1;
		return 1;
	}
	return 0;
}

void TRouletteSw::perform(u32 cue, JDrama::TGraphics* graphics)
{
	THitActor::perform(cue, graphics);
	unk68->switchStop();
}

TRoulette::TRoulette(const char* name)
    : TMapObjBase(name)
    , unk138(500.0f)
    , unk13C(0.0f)
    , unk140(0)
    , unk141(0)
    , unk142(0)
    , unk144(0.2f)
    , unk150(nullptr)
{
	unk148 = 0;
	unk14A = 0;
	unk14C = 0;
	unk14E = 255;
	if (gpApplication.mCurrArea.unk0 == 14 && gpMarDirector->unk7D == 1) {
		unk141 = 1;
		unk14C = 255;
	}
	if (gpApplication.mCurrArea.unk0 == 56) {
		unk14C = 255;
		unk142 = 1;
	}
}

void TRoulette::moveObject()
{
	TLiveActor::moveObject();
	if (unk142 != 0) {
		mRotation.x += unk13C;
		if (unk141 != 0 && unk140 != 0) {
			gpMarioOriginal->mGamePad->onNeutralMarioKey();
			gpMarioOriginal->mGamePad->mDisabledFrames = 5;
			gpMSound->startSoundActor(MSD_SE_BS_TELESA_RLT_DOWN, &mPosition, 0,
			                          nullptr, 0, 4);
			mPosition.y -= 0.5f;
			MtxPtr jnt                   = mMActor->getModel()->getAnmMtx(1);
			JGeometry::TVec3<f32>& swPos = unk150->mPosition;
			swPos.set(jnt[0][3], mPosition.y - 100.0f, jnt[2][3]);
		}
	}
}

void TRoulette::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TMapObjBase::perform(cue, graphics);
	unk150->perform(cue, graphics);
}

void TRoulette::calcRootMatrix()
{
	J3DModel* model = getModel();
	MsMtxSetXYZRPH(model->getBaseTRMtx(), mPosition.x, mPosition.y, mPosition.z,
	               mRotation.x, mRotation.y, mRotation.z);
	model->setBaseScale(mScaling);
}

void TRoulette::switchStop()
{
	if (unk150->unk6C != 0) {
		if (SMS_GetMarioPos().y < 20.0f + SMS_GetMarioGrLevel()
		    && unk13C != 0.0f) {
			unk150->unk6C = 0;
			unk13C        = 0.0f;
			unk148        = 0;
			unk14A        = 0;
			unk14C        = 0;
			gpMSound->startSoundActor(MSD_SE_BS_TELESA_RLT_STOP, &mPosition, 0,
			                          nullptr, 0, 4);
		}
		if (unk150->unk6C != 0 && unk141 != 0) {
			unk150->unk6C = 0;
			unk148        = 0;
			unk14A        = 0;
			unk14C        = 0;
			gpMSound->startSoundActor(MSD_SE_BS_TELESA_RLT_STOP, &mPosition, 0,
			                          nullptr, 0, 4);
			unk140 = 1;
		}
	}
}

void TRoulette::setRollSp(f32 sp)
{
	unk13C          = sp;
	unk148          = 0;
	unk14A          = 0;
	unk14C          = 255;
	unk150->unk6C = 0;
}

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

TSlotDrum::TSlotDrum(const char* name)
    : TSirenaRollMapObj(name)
    , unk194(0)
{
}

void TSlotDrum::initNeonMatColor()
{
	const char* matNames[3] = { "_NEON_A", "_NEON_B", "_NEON_C" };
	for (int i = 0; i < 3; i++) {
		unk170[i].r = 120;
		unk170[i].g = 230;
		unk170[i].b = 255;
		unk170[i].a = 255;
		SMS_InitPacket_OneTevColor(
		    mMActor->getModel(),
		    getModel()->getModelData()->getMaterialName()->getIndex(matNames[i]),
		    GX_TEVREG0, &unk170[i]);
	}
}

void TSlotDrum::initMapObj()
{
	unk148 = 3;
	unk14C = 400.0f;
	unk150 = mPosition.y;
	unk194 = 0;
	unk154 = 2.0f;
	unk158 = 10.0f;
	unk15C = 0.1f;
	unk160 = 0.5f;
	unk164 = 0;
	unk168 = 90;
	unk138 = new f32[unk148];
	unk13C = new f32[unk148];
	for (int i = 0; i < unk148; i++) {
		unk138[i] = 0.0f;
		unk13C[i] = 90.0f * (i + 1);
		unk188[i] = 0.0f;
	}
	TMapObjBase::initMapObj();
	for (u8 i = 0; i < getModel()->getModelData()->getJointNum(); i++)
		;
	for (int i = 1; i <= unk148; i++)
		mMActor->setJointCallback(i, partsRollCallback);
	unk140 = mDamageRadius / 3.0f;
	unk144 = mDamageHeight;
	initNeonMatColor();
}

void TSlotDrum::calcRootMatrix()
{
	gpCurObject     = this;
	J3DModel* model = getModel();
	MsMtxSetXYZRPH(model->getBaseTRMtx(), mPosition.x, mPosition.y - unk14C,
	               mPosition.z, mRotation.x, mRotation.y, mRotation.z);
	model->setBaseScale(mScaling);
}

u32 TSlotDrum::touchWater(THitActor* water)
{
	if (unk194 != 0)
		return 1;
	if (fabsf(mPosition.x - water->mPosition.x) < 150.0f) {
		f32 halfDepth = 0.6f * unk140;
		int idx;
		if (water->mPosition.z < mPosition.z - halfDepth) {
			idx = 2;
			if (mRotation.y < 0.0f)
				idx = 0;
		} else if (water->mPosition.z > mPosition.z + halfDepth) {
			idx = 0;
			if (mRotation.y < 0.0f)
				idx = 2;
		} else {
			idx = 1;
		}
		unk164 = 1;
		unk138[idx] += unk154 * unk164;
		if (fabsf(unk138[idx]) > unk158)
			unk138[idx] = unk158 * unk164;
		return 1;
	}
	return 0;
}

TItemSlotDrum::TItemSlotDrum(const char* name)
    : TSlotDrum(name)
    , unk198(0)
    , unk1A4(0)
    , unk1A8(5.0f)
{
	for (int i = 0; i < 3; i++) {
		unk19C[i] = 0;
		unk19F[i] = 1;
	}
	unk1A2 = 1;
}

void TItemSlotDrum::calcRootMatrix()
{
	gpCurObject = this;
	u8 spinning = 0;
	for (int i = 0; i < 3; i++) {
		if (0.0f != unk138[i])
			spinning = 1;
	}
	if (spinning)
		gpMSound->startSoundActor(MSD_SE_OBJ_SLOT_SPIN, &mPosition, 0, nullptr,
		                          0, 4);
	J3DModel* model = getModel();
	MsMtxSetXYZRPH(model->getBaseTRMtx(), mPosition.x, mPosition.y - unk14C,
	               mPosition.z, mRotation.x, mRotation.y, mRotation.z);
	model->setBaseScale(mScaling);
}

int TItemSlotDrum::getResultFromAng(f32 ang)
{
	if (ang < 89.0f)
		return 0;
	if (ang < 179.0f)
		return 1;
	if (ang < 269.0f)
		return 2;
	return 3;
}

TCasinoPanelGate::TCasinoPanelGate(const char* name)
    : TSirenaRollMapObj(name)
    , mMapCollisionWarp(nullptr)
    , unk16C(false)
    , unk16D(0)
{
}

void TCasinoPanelGate::initMapObj()
{
	unk148 = 16;
	unk14C = 410.0f;
	unk150 = mPosition.y;
	unk154 = 4.0f;
	unk158 = 15.0f;
	unk15C = 0.2f;
	unk160 = 0.5f;
	unk164 = 0;
	unk138 = new f32[unk148];
	unk13C = new f32[unk148];
	for (int i = 0; i < unk148; i++) {
		unk138[i] = 0.0f;
		unk13C[i] = 0.0f;
	}
	TMapObjBase::initMapObj();
	for (u16 i = 1; i <= unk148; i++)
		mMActor->setJointCallback(i, partsRollCallback);
}

void TCasinoPanelGate::calcRootMatrix()
{
	gpCurObject     = this;
	J3DModel* model = getModel();
	MsMtxSetXYZRPH(model->getBaseTRMtx(), mPosition.x, mPosition.y + unk14C,
	               mPosition.z, mRotation.x, mRotation.y, mRotation.z);
	model->setBaseScale(mScaling);
	unk140 = 0.25f * mDamageRadius;
	unk144 = 0.25f * mDamageHeight;
}

u32 TCasinoPanelGate::touchWater(THitActor* water)
{
	if (unk16D != 0)
		return 1;
	if (fabsf(mPosition.z - water->mPosition.z) < 50.0f) {
		unk164 = 1;
		int idx;
		if (water->mPosition.y > 3.0f * unk144 + mPosition.y) {
			if (water->mPosition.x < mPosition.x - unk140)
				idx = 12;
			else if (water->mPosition.x < mPosition.x)
				idx = 13;
			else if (water->mPosition.x > mPosition.x + unk140)
				idx = 15;
			else
				idx = 14;
			if (water->mPosition.y < 3.5f * unk144 + mPosition.y)
				unk164 = -1;
		} else if (water->mPosition.y > 2.0f * unk144 + mPosition.y) {
			if (water->mPosition.x < mPosition.x - unk140)
				idx = 8;
			else if (water->mPosition.x < mPosition.x)
				idx = 9;
			else if (water->mPosition.x > mPosition.x + unk140)
				idx = 11;
			else
				idx = 10;
			if (water->mPosition.y < 2.5f * unk144 + mPosition.y)
				unk164 = -1;
		} else if (water->mPosition.y > mPosition.y + unk144) {
			if (water->mPosition.x < mPosition.x - unk140)
				idx = 4;
			else if (water->mPosition.x < mPosition.x)
				idx = 5;
			else if (water->mPosition.x > mPosition.x + unk140)
				idx = 7;
			else
				idx = 6;
			if (water->mPosition.y < 1.5f * unk144 + mPosition.y)
				unk164 = -1;
		} else {
			if (water->mPosition.x < mPosition.x - unk140)
				idx = 0;
			else if (water->mPosition.x < mPosition.x)
				idx = 1;
			else if (water->mPosition.x > mPosition.x + unk140)
				idx = 3;
			else
				idx = 2;
			if (water->mPosition.y < 0.5f * unk144 + mPosition.y)
				unk164 = -1;
		}
		unk138[idx] += unk154 * unk164;
		if (fabsf(unk138[idx]) > unk158)
			unk138[idx] = unk158 * unk164;
		return 1;
	}
	return 0;
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

void TDonchou::calcRootMatrix()
{
	J3DModel* model = getModel();
	Mtx mtx;
	MsMtxSetXYZRPH(mtx, mPosition.x, mPosition.y + unk140, mPosition.z,
	               mRotation.x, mRotation.y, mRotation.z);
	PSMTXCopy(mtx, model->getBaseTRMtx());
	model->setBaseScale(mScaling);
	mtx[1][3] += unk140;
	if (unk144 != nullptr && unk144->unk194 != 0 && unk148->unk194 != 0)
		unk13C = 1;
	if (unk13C != 0) {
		unk14C++;
		if (unk14C > 100) {
			if (mMActor->checkCurAnm("donchou", 0)) {
				if (mMActor->curAnmEndsNext(0, 0))
					unk138->remove();
			} else {
				gpMSound->startSoundActor(MSD_SE_SY_DONCHO_OPEN, &mPosition, 0,
				                          nullptr, 0, 4);
				mMActor->setBck("donchou");
				gpMarDirector->fireStartDemoCamera("どんちょカメラ", &mPosition,
				                                   -1, 0.0f, true, nullptr, 0,
				                                   nullptr, JDrama::TFlagT<u16>(0));
				J3DFrameCtrl* fc = mMActor->getFrameCtrl(0);
				fc->setRate(0.5f * fc->getRate());
			}
		}
	}
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

void TCloset::calcRootMatrix()
{
	gpCurObject     = this;
	J3DModel* model = getModel();
	Mtx mtx;
	MsMtxSetXYZRPH(mtx, mPosition.x, mPosition.y + unk14C, mPosition.z,
	               mRotation.x, mRotation.y, mRotation.z);
	PSMTXCopy(mtx, model->getBaseTRMtx());
	model->setBaseScale(mScaling);
	mtx[1][3] += unk14C;
	if (unk16C != 0 && mMActor->checkCurAnm("closetopen", 0)
	    && mMActor->curAnmEndsNext(0, 0))
		mMapCollisionWarp->remove();
}

u32 TCloset::touchWater(THitActor* water)
{
	if (unk16C != 0)
		return 0;
	if (fabsf(mPosition.x - water->mPosition.x) < 50.0f) {
		f32 halfDepth = 1.1f * unk140;
		int idx;
		if (water->mPosition.z < mPosition.z - halfDepth) {
			idx = 0;
			if (mRotation.y < 0.0f)
				idx = 3;
		} else if (water->mPosition.z < mPosition.z) {
			idx = 1;
			if (mRotation.y < 0.0f)
				idx = 2;
		} else if (water->mPosition.z < mPosition.z + halfDepth) {
			idx = 2;
			if (mRotation.y < 0.0f)
				idx = 1;
		} else {
			idx = 3;
			if (mRotation.y < 0.0f)
				idx = 0;
		}
		unk164 = 1;
		unk138[idx] += unk154 * unk164;
		if (fabsf(unk138[idx]) > unk158)
			unk138[idx] = unk158 * unk164;
		return 1;
	}
	return 0;
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

void TPictureTelesa::afterFinishedAnim()
{
	TWaterHitPictureHideObj::afterFinishedAnim();
	if (isActorType(0x400001A2)) {
		gpMSound->startSoundSystemSE(MSD_SE_SY_CLEAR_SIGN_BIG, 0, nullptr, 0);
		gpMSound->startSoundActor(MSD_SE_BS_TELESA_V_LAUGH2, &mPosition, 0,
		                          nullptr, 0, 4);
	}
}

void TPictureTelesa::touchActor(THitActor* actor)
{
	TWaterHitPictureHideObj::touchActor(actor);
	if (isActorType(0x400001A2) && unk174 == 0 && isState(3)
	    && !isStateTimerEngaged()) {
		if (actor->mPosition.distance(mPosition) < 200.0f) {
			startStateTimer(60);
			gpMSound->startSoundActor(MSD_SE_BS_TELESA_DISAPPEAR, &mPosition, 0,
			                          nullptr, 0, 4);
			unk174 = 1;
		}
	}
}

void TPictureTelesa::control()
{
	TWaterHitPictureHideObj::control();
	if (unk174 != 0 && getColNum() == 0)
		unk174 = 0;
}
