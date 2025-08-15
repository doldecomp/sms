#include <MoveBG/WoodBarrel.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <Player/MarioAccess.hpp>
#include <System/EmitterViewObj.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

s32 TWoodBarrel::mWaitAppearTime     = 360;
int TWoodBarrel::mBreakTime          = 7200;
int TWoodBarrel::mOilBarrelFlushTime = 600;

void TWoodBarrel::put()
{
	TMapObjGeneral::put();
	if (mGroundPlane->mActor != nullptr
	    && mGroundPlane->mActor->isActorType(0x4000007b)) {
		kill();
		return;
	}

	if (mGroundHeight < mPosition.y - 50.0f) {
		onLiveFlag(LIVE_FLAG_AIRBORNE);
		offLiveFlag(LIVE_FLAG_UNK10);
	} else {
		offLiveFlag(LIVE_FLAG_AIRBORNE);
		onLiveFlag(LIVE_FLAG_UNK10);
	}
}

void TWoodBarrel::hold(TTakeActor* param_1)
{
	TMapObjGeneral::hold(param_1);
	if (isActorType(0x4000005c))
		unk104 = mBreakTime;
}

void TWoodBarrel::breaking()
{
	if (animIsFinished()) {
		makeObjDefault();
		makeObjDead();
		if (checkMapObjFlag(0x80000))
			waitToAppear(mWaitAppearTime);
	}
}

void TWoodBarrel::kill()
{
	TMapObjGeneral::kill();
	JGeometry::TVec3<f32> vec = mPosition;
	vec.y += 100.0f;
	unk148->mPos.value = vec;
	gpModelWaterManager->emitRequest(*unk148);
	if (mHolder) {
		mHolder->receiveMessage(this, 0x8);
		mHolder = nullptr;
	}
}

void TWoodBarrel::appeared()
{
	TMapObjGeneral::appeared();
	if (SMS_IsMarioStatusHipDrop()) {
		mDamageHeight = mMapObjData->mHit->unkC->unkC + 90.0f;
		calcEntryRadius();
	} else {
		mDamageHeight = mMapObjData->mHit->unkC->unkC;
		calcEntryRadius();
	}

	mGroundHeight = gpMap->checkGround(mPosition, &mGroundPlane);
	if (mPosition.y - 50.0f > mGroundHeight)
		offLiveFlag(LIVE_FLAG_UNK10);
}

void TWoodBarrel::appear()
{
	makeObjAppeared();
	gpMarioParticleManager->emitAndBindToPosPtr(0xE5, &mPosition, 0, nullptr);
	if (gpMSound->gateCheck(0x387D))
		MSoundSESystem::MSoundSE::startSoundActor(0x387D, &mPosition, 0,
		                                          nullptr, 0, 4);
}

void TWoodBarrel::touchWall(JGeometry::TVec3<f32>*, TBGWallCheckRecord*)
{
	kill();
}

void TWoodBarrel::touchGround(JGeometry::TVec3<f32>*)
{
	kill();
	if (mGroundPlane->isWaterSurface())
		emitColumnWater();
}

void TWoodBarrel::touchPlayer(THitActor* param_1)
{
	TMapObjGeneral::touchPlayer(param_1);
	if (SMS_IsMarioHeadSlideAttack())
		kill();
}

void TWoodBarrel::touchActor(THitActor* param_1)
{
	if (param_1->checkActorType(0x4000000)
	    || param_1->checkActorType(0x40000000)) {
		param_1->receiveMessage(this, 0xE);
		kill();
	} else {
		param_1->receiveMessage(this, 0xE);
		TMapObjGeneral::touchActor(param_1);
	}
}

BOOL TWoodBarrel::receiveMessage(THitActor* param_1, u32 param_2)
{
	if (param_2 == 0xD) {
		kill();
		return true;
	}

	return TMapObjGeneral::receiveMessage(param_1, param_2);
}

void TWoodBarrel::control()
{
	TMapObjGeneral::control();
	if (isActorType(0x4000005c) && isState(6) && !isUnk104Positive())
		kill();
}

void TWoodBarrel::loadAfter()
{
	TMapObjGeneral::loadAfter();
	mGroundHeight    = gpMap->checkGround(mPosition, &mGroundPlane);
	mPosition.y      = mGroundHeight;
	mInitialPosition = mPosition;
	checkOnManhole();
}

void TWoodBarrel::load(JSUMemoryInputStream& stream)
{
	TMapObjGeneral::load(stream);
	unk148 = new TWaterEmitInfo("/barrel.prm");
}

TWoodBarrel::TWoodBarrel(const char* name)
    : TMapObjGeneral(name)
    , unk148(nullptr)
{
}
