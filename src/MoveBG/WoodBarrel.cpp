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
	if (unkC4->unk44 != nullptr && unkC4->unk44->isActorType(0x4000007b)) {
		kill();
		return;
	}

	if (unkC8 < mPosition.y - 50.0f) {
		onLiveFlag(0x80);
		offLiveFlag(0x10);
	} else {
		offLiveFlag(0x80);
		onLiveFlag(0x10);
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
		mDamageHeight = unk130->mHit->unkC->unkC + 90.0f;
		calcEntryRadius();
	} else {
		mDamageHeight = unk130->mHit->unkC->unkC;
		calcEntryRadius();
	}

	unkC8 = gpMap->checkGround(mPosition, &unkC4);
	if (mPosition.y - 50.0f > unkC8)
		offLiveFlag(0x10);
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
	if (unkC4->checkSomething())
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
	unkC8       = gpMap->checkGround(mPosition, &unkC4);
	mPosition.y = unkC8;
	unk10C      = mPosition;
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
