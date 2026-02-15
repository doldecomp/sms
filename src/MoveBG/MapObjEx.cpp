#include <MoveBG/MapObjEx.hpp>
#include <MoveBG/ItemManager.hpp>
#include <MoveBG/Item.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorUtil.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <System/Application.hpp>
#include <System/Particles.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/JParticle/JPAResourceManager.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

f32 TMapObjNail::mDownHeight = 50.0f;

BOOL TMapObjNail::receiveMessage(THitActor* sender, u32 message)
{
	if (message == 1 && !isUnk104Positive() && unk150 < 3) {
		mPosition.y -= mDownHeight;
		removeMapCollision();
		setUpCurrentMapCollision();

		if (gpMSound->gateCheck(0x3850))
			MSoundSESystem::MSoundSE::startSoundActor(0x3850, mPosition, 0,
			                                          nullptr, 0, 4);

		unk104 = 0x78;
		++unk150;
		if (unk150 == 3 && unk138) {
			TMapObjBase* obj = unk138;
			if (obj->isActorType(0x2000000e))
				obj = gpItemManager->makeObjAppear(0x2000000e);
			if (obj)
				throwObjToFront(obj, 200.0f, unk13C, unk140);
		}
		return true;
	}

	return false;
}

TMapObjNail::TMapObjNail(const char* name)
    : THideObjBase(name)
    , unk150(0)
{
}

void TJointCoin::control()
{
	unk138->frameUpdate();
	unk138->calc();
	MtxPtr mtx = unk138->getModel()->getAnmMtx(0);
	mPosition.set(mtx[0][3], mtx[1][3], mtx[2][3]);
	getModel()->setBaseTRMtx(mtx);
	mMActor->frameUpdate();
	mMActor->calc();

	for (int i = 0; i < unk13C; ++i) {
		TMapObjBase* obj = unk140[i];
		u16 idx          = unk144[i];

		MtxPtr mtx = getModel()->getAnmMtx(idx);
		if (obj->mMActor)
			obj->getModel()->setAnmMtx(0, mtx);

		obj->mPosition.set(mtx[0][3], mtx[1][3] + obj->mYOffset, mtx[2][3]);
	}

	TMapObjBase::control();
}

TMapObjBase* TJointCoin::makeObj(const char* name, u16 i)
{
	TMapObjBase* pTVar2;

	if (strcmp(name, "coin") == 0) {
		pTVar2 = gpItemManager->newAndRegisterCoinReal();
	} else {
		pTVar2 = TMapObjBaseManager::newAndRegisterObj(name);
	}

	unk140[unk13C] = pTVar2;
	if (pTVar2->isActorType(0x2000000e)) {
		pTVar2->offMapObjFlag(0x40000);
		pTVar2->onMapObjFlag(0x10000000);
	}

	unk140[unk13C]->makeObjAppeared();
	unk140[unk13C]->onMapObjFlag(0x100);
	unk140[unk13C]->offMapObjFlag(0x40000);
	unk140[unk13C]->offLiveFlag(LIVE_FLAG_UNK100);
	unk140[unk13C]->offHitFlag(HIT_FLAG_NO_COLLISION);
	unk144[unk13C] = i;
	++unk13C;
	return pTVar2;
}

TMapObjBase* TJointCoin::makeObjFromJointName(const char* name, u16 i)
{
	if (strstr(name, "redcoin"))
		return makeObj("coin_red", i);
	else if (strstr(name, "bluecoin"))
		return makeObj("coin_blue", i);
	else if (strstr(name, "coin"))
		return makeObj("coin", i);
	else if (strstr(name, "dust"))
		return makeObj("no_data", i);
	else
		return nullptr;
}

void TJointCoin::loadAfter()
{
	TMapObjBase::loadAfter();
	unk13C = 0;

	for (u16 i = 0; i < getModel()->getModelData()->getJointNum() - 1; ++i) {
		const char* name
		    = getModel()->getModelData()->getJointName()->getName(i);
		makeObjFromJointName(name, i);
	}

	mMActor->getFrameCtrl(0)->setSpeed(SMSGetAnmFrameRate() * 0.25f);
	unk138->getFrameCtrl(0)->setSpeed(SMSGetAnmFrameRate() * 0.25f);
}

bool TJointCoin::nameIsObj(const char* name)
{
	return strstr(name, "coin") != 0 ? true : false;
}

void TJointCoin::initMapObj()
{
	TMapObjBase::initMapObj();

	for (u16 i = 0; i < getModel()->getModelData()->getJointNum(); ++i) {
		if (nameIsObj(getModel()->getModelData()->getJointName()->getName(i)))
			++unk13C;
	}

	unk140 = new TMapObjBase*[unk13C];
	unk144 = new u16[unk13C];

	if (isActorType(0x40000232)) {
		unk138 = SMS_MakeMActorWithAnmData("/scene/mapObj/CoinFishRail.bmd",
		                                   mManager->getMActorAnmData(), 3,
		                                   0x10210000);
		unk138->setBck("coinfishrail");
	} else if (isActorType(0x400000c9)) {
		unk138 = SMS_MakeMActorWithAnmData("/scene/mapObj/SandBirdRail.bmd",
		                                   mManager->getMActorAnmData(), 3,
		                                   0x10210000);
		unk138->setBck("sandbirdrail");
	}

	MsMtxSetXYZRPH(unk138->getModel()->getBaseTRMtx(), mPosition.x, mPosition.y,
	               mPosition.z, mRotation.x, mRotation.y, mRotation.z);
}

TJointCoin::TJointCoin(const char* name)
    : TMapObjBase(name)
    , unk138(nullptr)
    , unk13C(0)
    , unk140(nullptr)
    , unk144(nullptr)
{
}

void TMapObjSteam::control() { emitAndRotateScale(0x15C, 1, &mPosition); }

void TMapObjSteam::load(JSUMemoryInputStream& stream)
{
	THideObjBase::load(stream);
	SMS_LoadParticle("/scene/mapObj/ms_funsui_yuge.jpa", 0x15C);
}

static void dummy(f32* f) { *f = 0.0f; }
