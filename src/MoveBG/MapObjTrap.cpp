#include <MoveBG/MapObjTrap.hpp>
#include <Map/MapData.hpp>
#include <M3DUtil/MActor.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <Player/MarioAccess.hpp>
#include <System/Application.hpp>
#include <System/EmitterViewObj.hpp>
#include <Strategic/Strategy.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

static void dummy(Vec* v)
{
	*v = (Vec) { 0.0f, 0.0f, 0.0f };
	*v = (Vec) { 1.0f, 1.0f, 1.0f };
}

TLampTrapSpikeHit::TLampTrapSpikeHit(TLampTrapSpike* trap, const char* name)
    : THitActor(name)
    , unk68(trap)
{
	initHitActor(0x4000001E, 3, -0x80000000, 500.0f, 300.0f, 500.0f, 300.0f);
	JDrama::TNameRefGen::search<TIdxGroupObj>("アイテムグループ")
	    ->getChildren()
	    .push_back(this);
	offHitFlag(HIT_FLAG_NO_COLLISION);
}

BOOL TLampTrapSpikeHit::receiveMessage(THitActor* sender, u32 message)
{
	return unk68->receiveMessage(sender, message);
}

void TLampTrapSpikeHit::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	THitActor::perform(param_1, param_2);
	if (param_1 & 1) {
		mPosition = unk68->mPosition;
		mPosition.y += 2300.0f;
		if (unk68->unk138 == 2 || unk68->unk138 == 0 || unk68->unk138 == 1) {
			for (int i = 0; i < getColNum(); ++i)
				if (getCollision(i)->isActorType(-0x7fffffff))
					SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
		}
	}
}

TLampTrapSpike::TLampTrapSpike(const char* name)
    : TMapObjBase(name)
{
}

void TLampTrapSpike::initMapObj()
{
	TMapObjBase::initMapObj();
	onHitFlag(HIT_FLAG_NO_COLLISION);
}

void TLampTrapSpike::loadAfter()
{
	TMapObjBase::loadAfter();
	unk140 = new TLampTrapSpikeHit(this, "トゲあたり");
}

BOOL TLampTrapSpike::receiveMessage(THitActor* sender, u32 message)
{
	TMapObjBase::receiveMessage(sender, message);
}

void TLampTrapSpike::control()
{
	BOOL bVar1 = false;
	int thing  = unk138;

	switch (thing) {
	case 0: {
		J3DFrameCtrl* ctrl = mMActor->getFrameCtrl(0);
		if (unk13C == 0) {
			mMActor->setBck("lamptrapspike_up");
			J3DFrameCtrl* ctrl = mMActor->getFrameCtrl(0);
			ctrl->setFrame(6.0f);
			ctrl->setSpeed(SMSGetAnmFrameRate());
		}
		if (mMActor->curAnmEndsNext(0, nullptr)) {
			unk13C = 0;
			unk138 = 2;
		}
		if (ctrl->getCurrentFrame() > ctrl->getEndFrame() * 0.5f)
			bVar1 = true;
	} break;

	case 1: {
		J3DFrameCtrl* ctrl = mMActor->getFrameCtrl(0);
		if (unk13C == 0) {
			mMActor->setBck("lamptrapspike_down");
			J3DFrameCtrl* ctrl = mMActor->getFrameCtrl(0);
			ctrl->setFrame(0.0f);
			ctrl->setSpeed(SMSGetAnmFrameRate() * 0.8f);
		}
		if (mMActor->curAnmEndsNext(0, nullptr)) {
			unk13C = 0;
			unk138 = 3;
		}
		if (ctrl->getCurrentFrame() > ctrl->getEndFrame() * 0.5f)
			bVar1 = true;
	} break;

	case 2:
		if (unk13C == 0) {
			mMActor->setBck("lamptrapspike_up");
			J3DFrameCtrl* ctrl = mMActor->getFrameCtrl(0);
			ctrl->setFrame(ctrl->getEndFrame());
			ctrl->setSpeed(0.0f);
			if (gpMSound->gateCheck(0x3863))
				MSoundSESystem::MSoundSE::startSoundActor(0x3863, mPosition, 0,
				                                          nullptr, 0, 4);
		}
		if (unk13C >= 360) {
			unk13C = 0;
			unk138 = 1;
		}
		bVar1 = true;
		break;

	case 3:
		if (unk13C == 0) {
			mMActor->setBck("lamptrapspike_down");
			if (J3DFrameCtrl* ctrl = mMActor->getFrameCtrl(0)) {
				ctrl->setFrame(ctrl->getEndFrame());
				ctrl->setSpeed(0.0f);
			}
		}
		if (unk13C >= 120) {
			unk13C = 0;
			unk138 = 4;
		}
		bVar1 = false;
		break;

	case 4: {
		J3DFrameCtrl* ctrl = mMActor->getFrameCtrl(0);
		if (unk13C == 0) {
			mMActor->setBck("lamptrapspike_up");
			J3DFrameCtrl* ctrl = mMActor->getFrameCtrl(0);
			ctrl->setFrame(0.0f);
			ctrl->setSpeed(SMSGetAnmFrameRate() * 0.1f);
		}
		if (ctrl->checkPass(6.0f))
			ctrl->setSpeed(0.0f);
		if (unk13C >= 240) {
			unk13C = 0;
			unk138 = 0;
		}
	} break;

	default:
	case 5:
		J3DFrameCtrl* ctrl = mMActor->getFrameCtrl(0);
		if (unk13C == 0) {
			mMActor->setBck("lamptrapspike_up");
			J3DFrameCtrl* ctrl = mMActor->getFrameCtrl(0);
			ctrl->setFrame(6.0f);
			ctrl->setSpeed(-SMSGetAnmFrameRate());
		}
		if (ctrl->checkPass(0.0f)) {
			unk13C = 0;
			unk138 = 0;
		}
		break;
	}

	if (unk138 == thing) {
		++unk13C;
		if (unk13C == 0)
			unk13C = 1;
	}

	TMapObjBase::control();
	TBGCheckData* plane = SMS_GetMarioGrPlane();
	if (bVar1 && plane && plane->getActor() == this
	    && SMS_IsMarioTouchGround4cm())
		SMS_SendMessageToMario(this, 0xA);
}

void TLampTrapSpike::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	TMapObjBase::perform(param_1, param_2);
	unk140->perform(param_1, param_2);
}

TLampTrapIronHit::TLampTrapIronHit(TLampTrapIron* trap, const char* name)
    : THitActor(name)
    , unk68(trap)
{
	initHitActor(0x4000001D, 3, -0x80000000, 500.0f, 300.0f, 500.0f, 300.0f);
	JDrama::TNameRefGen::search<TIdxGroupObj>("アイテムグループ")
	    ->getChildren()
	    .push_back(this);
	offHitFlag(HIT_FLAG_NO_COLLISION);
}

BOOL TLampTrapIronHit::receiveMessage(THitActor* sender, u32 message)
{
	return unk68->receiveMessage(sender, message);
}

void TLampTrapIronHit::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	THitActor::perform(param_1, param_2);
	if (param_1 & 1) {
		mPosition = unk68->mPosition;
		mPosition.y += 2300.0f;
		if (unk68->unk13C > 0)
			for (int i = 0; i < getColNum(); ++i)
				if (getCollision(i)->isActorType(-0x7fffffff))
					SMS_SendMessageToMario(this, 0xA);
	}
}

u32 TLampTrapIron::mHitPointMax  = 60;
u32 TLampTrapIron::mFireTimerMax = 1200;

TLampTrapIron::TLampTrapIron(const char* name)
    : TMapObjBase(name)
    , unk138(nullptr)
    , unk13C(0)
    , unk140(0)
{
}

void TLampTrapIron::initMapObj()
{
	TMapObjBase::initMapObj();
	onHitFlag(HIT_FLAG_NO_COLLISION);
	unk140 = 0;
	unk13C = mHitPointMax;
}

void TLampTrapIron::loadAfter()
{
	TMapObjBase::loadAfter();
	unk138 = new TLampTrapIronHit(this, "鉄板あたり");
}

BOOL TLampTrapIron::receiveMessage(THitActor* sender, u32 message)
{
	if (sender->isActorType(0x1000001)) {
		if (unk13C > 0) {
			--unk13C;
			if (unk13C == 0) {
				unk140 = mFireTimerMax;
				gpMarioParticleManager->emitAndBindToMtxPtr(
				    100, getModel()->getBaseTRMtx(), 0, this);
			}
		}
		return true;
	}

	return TMapObjBase::receiveMessage(sender, message);
}

void TLampTrapIron::control()
{
	TMapObjBase::control();
	if (unk140 <= 0) {
		TBGCheckData* plane = SMS_GetMarioGrPlane();
		if (plane && plane->getActor() == this && SMS_IsMarioTouchGround4cm())
			SMS_SendMessageToMario(this, 10);
	} else {
		--unk140;
		if (unk140 == 0)
			unk13C = mHitPointMax;
	}
}

void TLampTrapIron::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	TMapObjBase::perform(param_1, param_2);
	unk138->perform(param_1, param_2);
	if ((param_1 & 2) && unk13C > 0) {
		gpMarioParticleManager->emit(0x1F1, &mPosition, 3, this);
		gpMarioParticleManager->emit(0x12C, &mPosition, 1, this);
	}
}

static void dummy2(f32* f) { *f = 0.0f; }
