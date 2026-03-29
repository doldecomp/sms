#include <MoveBG/MapObjHide.hpp>

#include <printf.h>

#include <MoveBG/Item.hpp>
#include <MoveBG/ItemManager.hpp>
#include <MoveBG/MapObjBall.hpp>

#include <Map/Map.hpp>
#include <Map/MapData.hpp>

#include <System/EmitterViewObj.hpp>
#include <System/FlagManager.hpp>
#include <System/MarDirector.hpp>
#include <System/Particles.hpp>

#include <Player/MarioAccess.hpp>
#include <Player/ModelWaterManager.hpp>

#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>

#include <MSound/MSound.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <M3DUtil/MActor.hpp>

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

void THideObj::touchPlayer(THitActor* param_1)
{
	if (marioHeadAttack() && unk138 != nullptr) {
		appearObj(100.0f);
		makeObjDead();
	}
}

u32 TWaterHitHideObj::touchWater(THitActor* param_1)
{
	if (unk138 != 0) {
		appearObj(100.0f);
		makeObjDead();
	}
	return 1;
}

void TWaterHitHideObj::load(JSUMemoryInputStream& stream)
{
	THideObjBase::load(stream);
}

TWaterHitHideObj::TWaterHitHideObj(const char* name)
    : THideObjBase(name)
{
}

void TFruitHitHideObj::touchFruit(THitActor* param_1)
{
	if (unk138 != 0) {
		appearObj(50.0f);
		emitEffect();
		makeObjDead();
	}

	((TMapObjBase*)param_1)->kill();
}

void TFruitHitHideObj::touchActor(THitActor* param_1)
{
	if (TMapObjBase::isFruit(param_1)) {
		touchFruit(param_1);
	}
}

void TFruitHitHideObj::load(JSUMemoryInputStream& stream)
{
	THideObjBase::load(stream);
}

TFruitHitHideObj::TFruitHitHideObj(const char* name)
    : THideObjBase(name)
{
}

void TFruitBasket::countFruit(THitActor* param_1)
{
	mMActor->setBck("basket");

	if (unk138 != nullptr) {
		appearObj(0.0f);

		SMSGetMSound()->startSoundActor(0x3809, &mPosition, 0, nullptr, 0, 4);
		SMSGetMSound()->startSoundSystemSE(0x480A, 0, nullptr, 0);
	} else {
		if (unk150 == 0) {
			SMSGetMSound()->startSoundActor(0x384E, &mPosition, 0, nullptr, 0,
			                                4);
		} else if (param_1->isActorType(unk150)) {

			SMSGetMSound()->startSoundActor(0x3809, &mPosition, 0, nullptr, 0,
			                                4);
			SMSGetMSound()->startSoundSystemSE(0x480A, 0, nullptr, 0);
		} else {
			SMSGetMSound()->startSoundSystemSE(0x483D, 0, nullptr, 0);
		}
	}
	((TResetFruit*)param_1)->makeObjWaitingToAppear();
}

void TFruitBasket::touchFruit(THitActor* param_1)
{
	if (fabsf(mRotation.x) < 45.0f) {
		if (((TLiveActor*)param_1)->getGroundPlane()->getActor() != this)
			return;
	} else {
		const TBGCheckData* roofPlane;
		gpMap->checkRoof(param_1->mPosition.x, param_1->mPosition.y,
		                 param_1->mPosition.z, &roofPlane);
		if ((TFruitBasket*)roofPlane->getActor() != this)
			return;
	}

	countFruit(param_1);
}

void TFruitBasket::loadAfter()
{
	TFruitHitHideObj::loadAfter();

	if (mRotation.x != 0.0f) {
		setAttackRadius(400.0f);
		setAttackHeight(200.0f);
	}
}

TFruitBasket::TFruitBasket(const char* name)
    : TFruitHitHideObj(name)
    , unk150(0)
{
}

void TFruitBasketEvent::countFruit(THitActor* fruit)
{
	TFruitBasket::countFruit(fruit);

	switch (fruit->getActorType()) {
	case 0x40000394:
		unk154[0] += 1;
		break;
	case 0x40000393:
		unk154[1] += 1;
		break;
	case 0x40000391:
		unk154[2] += 1;
		break;
	case 0x40000392:
		unk154[3] += 1;
		break;
	case 0x40000390:
		unk154[4] += 1;
		break;
	case 0x40000395:
		break;
	default:
		return;
	}
	// This feels wrong... symbols say param is a THitActor tho..
	// Sane approach would be to cast after validating earlier
	((TResetFruit*)fruit)->makeObjWaitingToAppear();
	mColCount = 0;
}

int TFruitBasketEvent::getFruitNum(int idx) const { return unk154[idx]; }

void TFruitBasketEvent::reset()
{
	for (int i = 0; i < 5; ++i) {
		unk154[i] = 0;
	}
}

TFruitBasketEvent::TFruitBasketEvent(const char* name)
    : TFruitBasket(name)
{
	reset();
}

void THipDropHideObj::touchPlayer(THitActor* param_1)
{
	if (SMS_IsMarioStatusHipDrop()) {
		appearObj(0);
		makeObjDead();
	}
}

THipDropHideObj::THipDropHideObj(const char* name)
    : THideObjBase(name)
{
}

void TWaterHitPictureHideObj::afterFinishedAnim()
{
	if (isActorType(0x400001A1)) {
		SMSGetMSound()->startSoundActor(0x296F, &mPosition, 0, nullptr, 0, 4);
		SMSGetMSound()->startSoundSystemSE(0x484D, 0, nullptr, 0);
	}

	removeMapCollision();
	onHitFlag(HIT_FLAG_UNK4);
	appearObjFromPoint(getObjAppearPos());
	mState = 3;
}

void TWaterHitPictureHideObj::forward(f32 param_1)
{
	if (unk150) {
		unk160 -= param_1;
		if (unk160 < unk164) {
			afterFinishedAnim();
		}
	} else {
		unk160 += param_1;
		if (unk160 > unk168) {
			afterFinishedAnim();
		}
	}
	unk160   = MsClamp(unk160, unk164, unk168);
	unk16C.a = (u16)unk160 & 0xff;
}

u32 TWaterHitPictureHideObj::touchWater(THitActor* param_1)
{
	volatile u32 padding[12];
	const JGeometry::TVec3<f32>& waterSpeed = getWaterSpeed(param_1);

	MtxPtr rootMtx = getModel()->getAnmMtx(0);
	if ((rootMtx[0][2] * waterSpeed.x + rootMtx[1][2] * waterSpeed.y
	     + rootMtx[2][2] * waterSpeed.z)
	    > 0.0f) {
		return 0;
	}
	int id = getWaterID(param_1);
	if (gpModelWaterManager->checkFlagBottom4Bits(id, 0x1)) {
		gpMarioParticleManager->emit(0xe7, &mPosition, 0, nullptr);
		SMSGetMSound()->startSoundSet(0x6802, &mPosition, 0, 0, 0, 0, 4);
	}
	forward(unk154);
	if (isActorType(0x400001A1)) {
		soundBas(0x296E, 200.0f, unk154);
	}
	return 1;
}

void TWaterHitPictureHideObj::touchActor(THitActor* param_1)
{
	if (param_1->isActorType(0x4000005A)) {
		mState = 2;
	}
}

void TWaterHitPictureHideObj::control()
{
	TMapObjBase::control();
	switch (mState) {
	case 1:
		if (unk150) {
			unk160 += unk15C;
			if (unk160 > unk168) {
				unk160 = unk168;
			}
		} else {
			unk160 -= unk15C;
			if (unk160 < unk164) {
				unk160 = unk164;
			}
		}
		unk16C.a = (u16)unk160 & 0xff;
		break;
	case 2:
		forward(unk158);
		break;
	case 3:
		break;
	}
}

BOOL TWaterHitPictureHideObj::receiveMessage(THitActor* sender, u32 message)
{
	if (message == 5
	    && (sender->isActorType(0x2000000E) || sender->isActorType(0x2000000F)
	        || sender->isActorType(0x20000010))) {
		offHitFlag(HIT_FLAG_NO_COLLISION);
		offHitFlag(HIT_FLAG_UNK4);
		offHitFlag(HIT_FLAG_UNK2);
		mState = 1;
		unk14C = 1;
		return TRUE;
	}
	if (isState(3)) {
		return FALSE;
	}
	if (sender->isActorType(0x4000005A)) {
		mState = 2;
		return TRUE;
	}

	if (message == 5
	    && (sender->isActorType(0x2000000E) || sender->isActorType(0x2000000F)
	        || sender->isActorType(0x20000010))) {
		unk14C = true;
	}
	return TMapObjBase::receiveMessage(sender, message);
}

void TWaterHitPictureHideObj::loadAfter()
{
	volatile u32 padding[10];
	TMapObjBase::loadAfter();

	const char* name = mName;
	unk138 = TMapObjBaseManager::newAndRegisterObjByEventID(unk134, name);

	// very likely inline, duplicate from another function
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

	if (unk138 != nullptr) {
		if (unk138->isActorType(0x20000010)) {
			bool isBlueCollected = TFlagManager::smInstance->getBlueCoinFlag(
			    gpMarDirector->getCurrentMap(), unk134);
			if (isBlueCollected) {
				makeObjDead();
				return;
			}
		}
	}
	switch (mActorType) {
	case 0x40000012:
		unk164 = 32.0f;
		unk168 = 100.0f;
		if (getModel()->getAnmMtx(0)[1][2] > 0.7f) {
			unk154 = 1.5f;
		} else {
			unk154 = 0.48f;
		}
		unk15C = 0.2f;
		unk150 = false;
		break;
	case 0x40000013:
	case 0x40000018:
	case 0x40000019:
	case 0x4000001A:
		unk164 = 20.0f;
		unk150 = true;
		break;
	case 0x40000020:
		unk154 = 1.2f;
		unk164 = 30.0f;
		unk168 = 170.0f;
		unk150 = false;
		break;
	case 0x400001A2:
		unk164 = 0.0f;
		unk168 = 200.0f;
		unk154 = 0.4f;
		unk150 = false;
		break;
	case 0x400001A1:
		unk164 = 0.0f;
		unk168 = 255.0f;
		unk150 = true;
		break;
	}

	if (unk150) {
		unk160 = unk168;
	} else {
		unk160 = unk164;
	}

	if (unk138 != nullptr) {
		if (TMapObjBase::isCoin(unk138)) {
			unk138->offMapObjFlag(0x10000000);
		}

		if (unk138->isActorType(0x20000013)) {

			bool isShineCollected
			    = TFlagManager::smInstance->getShineFlag(unk138->unk134);
			if (isShineCollected) {
				if (unk150) {
					unk160 = unk164;
				} else {
					unk160 = unk168;
				}
				unk16C.a = (u16)unk160 & 0xff;
				mState   = 3;
			}
		}
	}
}

void TWaterHitPictureHideObj::load(JSUMemoryInputStream& stream)
{
	THideObjBase::load(stream);

	u32 r;
	u32 g;
	u32 b;

	stream.read(r);
	stream.read(g);
	stream.read(b);
	unk16C.r = r & 0xff;
	unk16C.g = g & 0xff;
	unk16C.b = b & 0xff;

	unk16C.a = 0xFF;
	unk154   = 1.8f;
	unk158   = 1.0f;
	unk15C   = 0.4f;

	f32 unk = getModel()->getAnmMtx(0)[1][2];
	if (unk > 0.7f) {
		mYOffset = 0.0f;
		setDamageHeight(40.0f);
		mPosition.y = mInitialPosition.y + mYOffset;
		unk154      = 2.8f;
	} else {
		if (unk < -0.7f) {
			mYOffset = mScaling.y * -10.0f;
			setDamageHeight(30.0f);
			mPosition.y = mInitialPosition.y + mYOffset;
			unk154      = 2.5f;
		} else {
			unk154 = 1.8f;
		}
	}

	initPacketMatColor(getModel(), GX_TEVREG0, &unk16C);
}

TWaterHitPictureHideObj::TWaterHitPictureHideObj(const char* name)
    : THideObjBase(name)
    , unk150(true)
    , unk154(0.0f)
    , unk158(0.0f)
    , unk15C(0.0f)
    , unk160(0.0f)
    , unk164(0.0f)
    , unk168(255.0f)
{
	unk16C.r = 0;
	unk16C.g = 0;
	unk16C.b = 0;
	unk16C.a = 0;
}

// A lot in this function is likely wrong still
// Some stuff feels pretty fof
void THideObjPictureTwin::afterFinishedAnim()
{
	removeMapCollision();
	onHitFlag(HIT_FLAG_NO_COLLISION);

	TMapObjBase* target = unk138;
	if (target != nullptr && unk14C != 0) {
		if (TMapObjBase::isCoin(target)) {
			TCoin* coin = (TCoin*)target;
			if (coin->isActorType(0x2000000E)) {
				target = gpItemManager->makeObjAppear(0x2000000E);
				if (target == nullptr) {
					return;
				}
			}
			((TCoin*)target)->appearWithoutSound();
		} else {
			appear();
		}

		target->mPosition.set(unk174->mPosition);
		Mtx mtx;
		MsMtxSetRotRPH(mtx, unk174->mRotation.x, unk174->mRotation.y,
		               unk174->mRotation.z);
		target->mVelocity.set(mtx[0][2] * unk13C, mtx[1][2] * unk13C + unk140,
		                      mtx[2][2] * unk13C);
		target->offLiveFlag(LIVE_FLAG_UNK10);

		if (TMapObjBase::isCoin(target)) {
			TCoin* coin  = (TCoin*)target;
			coin->unk148 = this;
			coin->unk14C = unk148;
		}
		SMSGetMSound()->startSoundSystemSE(0x4843, 0, nullptr, 0);

		// Possible getter?
		TMarDirector* director = gpMarDirector;
		director->fireStartDemoCamera(unk178, &mPosition, -1, 0.0f, true,
		                              nullptr, 0, nullptr, 0);
	}
	mState = 3;
}

void THideObjPictureTwin::loadAfter()
{
	TWaterHitPictureHideObj::loadAfter();
	char* wrapName = strstr(mName, "ふたご落書きＡ");
	if (wrapName != nullptr) {
		size_t len = strlen("ふたご落書きＡ");
		char buffer[4];
		buffer[0] = mName[len];
		buffer[1] = mName[len + 1];
		buffer[2] = mName[len + 2];
		buffer[3] = mName[len + 3];

		char buffer2[0x4C];
		snprintf(buffer2, 0x40, "ふたご落書きＢ００");
		buffer2[len]     = buffer[0];
		buffer2[len + 1] = buffer[1];
		buffer2[len + 2] = buffer[2];
		buffer2[len + 3] = buffer[3];

		THideObjPictureTwin* hitActor
		    = (THideObjPictureTwin*)JDrama::TNameRefGen::getInstance()
		          ->getRootNameRef()
		          ->searchF(TNameRef::calcKeyCode(buffer2), buffer2);
		unk174         = hitActor;
		unk174->unk174 = this;
	}
}

void THideObjPictureTwin::initMapObj()
{
	TMapObjBase::initMapObj();
	snprintf(unk178, 0x19, "%sカメラ", getName());
}

THideObjPictureTwin::THideObjPictureTwin(const char* name)
    : TWaterHitPictureHideObj(name)
    , unk174(nullptr)
{
	memset(unk178, 0, 0x19);
}

void TBreakHideObj::kill()
{
	startAnim(2);
	removeMapCollision();
	onHitFlag(HIT_FLAG_NO_COLLISION);
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

void TWoodBox::fabricatedGroundKillCheck(f32 dX, f32 dY)
{
	const TBGCheckData* groundPlane;
	f32 resY = gpMap->checkGround(dX + gpMarioPos->x, gpMarioPos->y + 1000.0f,
	                              dY + gpMarioPos->z, &groundPlane);
	if (resY + 10.0f > gpMarioPos->y) {
		const TLiveActor* actor = groundPlane->getActor();
		if (actor != nullptr && actor != this
		    && actor->isActorType(0x4000001C)) {
			((TBreakHideObj*)actor)->kill();
		}
	}
}
void TWoodBox::kill()
{
	startAnim(2);
	removeMapCollision();
	onHitFlag(HIT_FLAG_NO_COLLISION);
	onLiveFlag(LIVE_FLAG_UNK10 | LIVE_FLAG_UNK8);
	mTimeTilAppear = -1;
	mState         = 2;

	SMSGetMSound()->startSoundActor(0x380A, &mPosition, 0, nullptr, 0, 4);

	fabricatedGroundKillCheck(50.0f, 50.0f);
	fabricatedGroundKillCheck(50.0f, -50.0f);
	fabricatedGroundKillCheck(-50.0f, 50.0f);
	fabricatedGroundKillCheck(-50.0f, -50.0f);
}

void TWoodBox::loadAfter()
{
	TBreakHideObj::loadAfter();
	checkOnManhole();
}

TWoodBox::TWoodBox(const char* name)
    : TBreakHideObj(name)
{
}
