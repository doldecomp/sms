#include <MoveBG/MapObjHide.hpp>

#include <printf.h>

#include <MoveBG/Item.hpp>
#include <MoveBG/ItemManager.hpp>
#include <MoveBG/MapObjBall.hpp>

#include <System/EmitterViewObj.hpp>
#include <System/FlagManager.hpp>
#include <System/MarDirector.hpp>
#include <System/Particles.hpp>

#include <MSound/MSound.hpp>

void THideObjBase::appearObj(f32 param1)
{
	JGeometry::TVec3<f32> pos;
	pos.set(mPosition.x, mPosition.y + param1, mPosition.z);
	appearObjFromPoint(pos);
}

void THideObjBase::appearObjFromPoint(const JGeometry::TVec3<f32>& param1)
{
	if (unk138 != nullptr && unk14C != 0) {
		if (isActorType(0x20000013)) {
			onLiveFlag(0x10);
			unk138->mPosition.set(mPosition);
			((TShine*)unk138)->appearWithDemo(unk144);
		} else {
			TMapObjBase* obj;
			if (isActorType(0x2000000E)) {
				obj = gpItemManager->makeObjAppear(0x2000000E);
				if (!obj) {
					return;
				}
			} else {
				obj = unk138;
			}

			throwObjToFrontFromPoint(obj, param1, unk13C, unk140);
			if (TMapObjBase::isCoin(obj)) {
				TCoin* coin  = (TCoin*)obj;
				coin->unk148 = this;
				coin->unk14C = unk148;
			}

			if (TMapObjBase::isFruit(obj)) {
				((TResetFruit*)obj)->makeObjLiving();
			}
		}
		emitEffect();
		unk14C = false;
	}
}

void THideObjBase::emitEffect()
{
	gpMarioParticleManager->emit(0x57, &mPosition, 0, nullptr);
}

BOOL THideObjBase::receiveMessage(THitActor* sender, u32 message)
{
	if (message == 5
	    && (isActorType(0x2000000E) || isActorType(0x2000000F)
	        || isActorType(0x20000010))) {
		unk14C = true;
	}
	TMapObjBase::receiveMessage(sender, message);
}

void THideObjBase::loadAfter()
{
	TMapObjBase::loadAfter();
	const char* name = mName;
	unk138 = TMapObjBaseManager::newAndRegisterObjByEventID(unk134, name);
	if (unk138 != nullptr) {
		if (unk138->isActorType(0x20000010)) {
			bool isBlueCollected = TFlagManager::smInstance->getBlueCoinFlag(
			    gpMarDirector->getCurrentMap(), unk134);
			if (isBlueCollected) {
				unk14C = false;
			}
		}

		if (unk138->isActorType(0x20000013)) {
			size_t nameLen    = strlen(mName);
			unk144            = new char[nameLen + 0x13];
			const char* name2 = mName;
			snprintf(unk144, nameLen + 0x13, "シャイン（%s）カメラ", name2);
		}
	}
}

void THideObjBase::load(JSUMemoryInputStream& stream)
{
	TMapObjBase::load(stream);

	s32 unk134Val;
	TMapObjBase::loadHideObjInfo(stream, &unk134Val, &unk13C, &unk140, &unk148);
	unk134 = unk134Val;
	SMS_LoadParticle("/scene/mapObj/ms_watcoin_hit.jpa", 0x57);
}

THideObjBase::THideObjBase(const char* name)
    : TMapObjBase(name)
    , unk138(nullptr)
    , unk13C(0.0f)
    , unk140(0.0f)
    , unk144(nullptr)
    , unk148(0)
    , unk14C(true)
{
}

void TBreakHideObj::kill()
{
	startAnim(2);
	removeMapCollision();
	onHitFlag(1);
	onLiveFlag(LIVE_FLAG_UNK10 | LIVE_FLAG_UNK8);
	mTimeTilAppear = -1;
	mState         = 2;
}

BOOL TBreakHideObj::receiveMessage(THitActor* sender, u32 message)
{
	if (message == 1) {
		if (isActorType(0x400002C3)) {
			emitAndScale(0x6B, 0, &mPosition);
			emitAndScale(0x6C, 0, &mPosition);
			SMSGetMSound()->startSoundActor(0x38A3, &mPosition, 0, nullptr, 0,
			                                4);
		}
		kill();
		return TRUE;
	}
	return FALSE;
}

void TBreakHideObj::control()
{
	TMapObjBase::control();

	switch (mState) {
	case 1:
		break;
	case 2:
		if (animIsFinished()) {
			appearObj(0);
			makeObjDead();
		}
		break;
	}
}

void TBreakHideObj::initMapObj()
{
	TMapObjBase::initMapObj();
	if (isActorType(0x400002C3)) {
		SMS_LoadParticle("/scene/mapObj/WaterMelonBlockA.jpa", 0x6B);
		SMS_LoadParticle("/scene/mapObj/WaterMelonBlockB.jpa", 0x6C);
	}
}
