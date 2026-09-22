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

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>
#include <Map/MapCollisionManager.hpp>

void THideObjBase::appearObj(f32 y_offset)
{
	JGeometry::TVec3<f32> pos;
	pos.set(mPosition.x, mPosition.y + y_offset, mPosition.z);
	appearObjFromPoint(pos);
}

void THideObjBase::appearObjFromPoint(const JGeometry::TVec3<f32>& point)
{
	if (mHiddenObj != nullptr && mAllowReveal) {
		if (mHiddenObj->isActorType(0x20000013)) {
			mHiddenObj->onLiveFlag(LIVE_FLAG_UNK10);
			mHiddenObj->mPosition.set(mPosition);
			((TShine*)mHiddenObj)->appearWithDemo(mHiddenShineDemoName);
		} else {
			TMapObjBase* obj;
			if (mHiddenObj->isActorType(0x2000000E)) {
				obj = gpItemManager->makeObjAppear(0x2000000E);
				if (!obj)
					return;
			} else {
				obj = mHiddenObj;
			}

			throwObjToFrontFromPoint(obj, point, mAppearSpeed, mAppearYSpeed);
			if (TMapObjBase::isCoin(obj)) {
				TCoin* coin      = (TCoin*)obj;
				coin->mContainer = this;
				coin->unk14C     = unk148;
			}

			if (TMapObjBase::isFruit(obj))
				((TResetFruit*)obj)->makeObjLiving();
		}
		emitEffect();
		mAllowReveal = false;
	}
}

void THideObjBase::emitEffect()
{
	gpMarioParticleManager->emit(MAPOBJ_MS_WATCOIN_HIT, &mPosition, 0, nullptr);
}

BOOL THideObjBase::receiveMessage(THitActor* sender, u32 message)
{
	if (message == HIT_MESSAGE_UNK5
	    && (sender->isActorType(0x2000000E) || sender->isActorType(0x2000000F)
	        || sender->isActorType(0x20000010)))
		mAllowReveal = true;

	return TMapObjBase::receiveMessage(sender, message);
}

// Binding level worth +8 of low region, landing THideObjBase::loadAfter's
// frame at 0x30 (batch 121).
static inline const char* MapObjHideGetName(const THideObjBase* p)
{
	const char* name = p->getName();
	return name;
}

// Named getModel / getAnmMtx stack: +0x10 then +8 on
// TWaterHitPictureHideObj::loadAfter / load.
static inline J3DModel* MapObjHideGetModel(TLiveActor* p)
{
	J3DModel* model = p->getModel();
	return model;
}

static inline MtxPtr MapObjHideGetRootMtx(TLiveActor* p)
{
	MtxPtr mtx = MapObjHideGetModel(p)->getAnmMtx(0);
	return mtx;
}

// Named-local fork over the particle singleton, +8 on touchWater.
static inline TMarioParticleManager* MapObjHideParticles()
{
	TMarioParticleManager* mgr = gpMarioParticleManager;
	return mgr;
}

// Unnamed extra level over gpModelWaterManager, last +8 on touchWater.
static inline TModelWaterManager* MapObjHideWaterMgr()
{
	return gpModelWaterManager;
}

void THideObjBase::loadAfter()
{
	TMapObjBase::loadAfter();
	mHiddenObj
	    = TMapObjBaseManager::newAndRegisterObjByEventID(
	        mEventId, MapObjHideGetName(this));
	if (mHiddenObj != nullptr) {
		if (mHiddenObj->isActorType(0x20000010)) {
			bool isBlueCollected = TFlagManager::smInstance->getBlueCoinFlag(
			    gpMarDirector->getCurrentMap(), mEventId);
			if (isBlueCollected)
				mAllowReveal = false;
		}

		if (mHiddenObj->isActorType(0x20000013)) {
			size_t nameLen       = strlen(mName);
			mHiddenShineDemoName = new char[nameLen + 0x13];
			const char* name2    = mName;
			snprintf(mHiddenShineDemoName, nameLen + 0x13,
			         "シャイン（%s）カメラ", name2);
		}
	}
}

void THideObjBase::load(JSUMemoryInputStream& stream)
{
	TMapObjBase::load(stream);

	s32 eventId;
	TMapObjBase::loadHideObjInfo(stream, &eventId, &mAppearSpeed,
	                             &mAppearYSpeed, &unk148);
	setEventId(eventId);
	SMS_LoadParticle("/scene/mapObj/ms_watcoin_hit.jpa", 0x57);
}

THideObjBase::THideObjBase(const char* name)
    : TMapObjBase(name)
    , mHiddenObj(nullptr)
    , mAppearSpeed(0.0f)
    , mAppearYSpeed(0.0f)
    , mHiddenShineDemoName(nullptr)
    , unk148(0)
    , mAllowReveal(true)
{
}

void THideObj::touchPlayer(THitActor* param_1)
{
	if (marioHeadAttack() && mHiddenObj != nullptr) {
		appearObj(100.0f);
		makeObjDead();
	}
}

u32 TWaterHitHideObj::touchWater(THitActor* param_1)
{
	if (mHiddenObj != nullptr) {
		appearObj(50.0f);
		makeObjDead();
	}
	return 1;
}

void TWaterHitHideObj::load(JSUMemoryInputStream& stream)
{
	THideObjBase::load(stream);
}

void TFruitHitHideObj::touchFruit(THitActor* param_1)
{
	if (mHiddenObj != nullptr) {
		appearObj(50.0f);
		emitEffect();
		makeObjDead();
	}

	((TMapObjBase*)param_1)->kill();
}

void TFruitHitHideObj::touchActor(THitActor* param_1)
{
	if (TMapObjBase::isFruit(param_1))
		touchFruit(param_1);
}

void TFruitHitHideObj::load(JSUMemoryInputStream& stream)
{
	THideObjBase::load(stream);
}

static inline MSound* FruitBasketSound()
{
	MSound* sound = SMSGetMSound();
	return sound;
}

void TFruitBasket::countFruit(THitActor* param_1)
{
	mMActor->setBck("basket");

	if (mHiddenObj != nullptr) {
		appearObj(0.0f);

		FruitBasketSound()->startSoundActor(MSD_SE_IT_SOCCER_GOAL, &mPosition, 0,
		                                nullptr, 0, 4);
		FruitBasketSound()->startSoundSystemSE(MSD_SE_FGM_SOCCER_GOAL, 0, nullptr,
		                                   0);
	} else {
		if (unk150 == 0) {
			SMSGetMSound()->startSoundActor(MSD_SE_OBJ_BASKET_BOUND, &mPosition,
			                                0, nullptr, 0, 4);
		} else if (param_1->isActorType(unk150)) {
			SMSGetMSound()->startSoundActor(MSD_SE_IT_SOCCER_GOAL, &mPosition,
			                                0, nullptr, 0, 4);
			SMSGetMSound()->startSoundSystemSE(MSD_SE_FGM_SOCCER_GOAL, 0,
			                                   nullptr, 0);
		} else {
			SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_NOT_COLLECT, 0,
			                                   nullptr, 0);
		}
	}
	((TResetFruit*)param_1)->makeObjWaitingToAppear();
}

// roofPlane declared at function scope, the fruit named inside the first arm
// and the roof's actor read raw put roofPlane at retail's 0x28.
void TFruitBasket::touchFruit(THitActor* param_1)
{
	const TBGCheckData* roofPlane;
	if (fabsf(mRotation.x) < 45.0f) {
		TLiveActor* fruit = (TLiveActor*)param_1;
		// Upwards facing basket -- check that the fruit's on top of us
		if (fruit->getGroundPlane()->getActor() != this)
			return;
	} else {
		// Basket lying on it's side -- check that the fruit rolled inside
		// enough to be under our side
		gpMap->checkRoof(param_1->mPosition.x, param_1->mPosition.y,
		                 param_1->mPosition.z, &roofPlane);
		if (roofPlane->mActor != this)
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

	((TResetFruit*)fruit)->makeObjWaitingToAppear();
	mColCount = 0;
}

int TFruitBasketEvent::getFruitNum(int idx) const { return unk154[idx]; }

void TFruitBasketEvent::reset()
{
	for (int i = 0; i < 5; ++i)
		unk154[i] = 0;
}

TFruitBasketEvent::TFruitBasketEvent(const char* name)
    : TFruitBasket(name)
{
	reset();
}

void THipDropHideObj::touchPlayer(THitActor* param_1)
{
	if (SMS_IsMarioStatusHipDrop()) {
		appearObj(0.0f);
		makeObjDead();
	}
}

void TWaterHitPictureHideObj::afterFinishedAnim()
{
	if (isActorType(0x400001A1)) {
		SMSGetMSound()->startSoundActor(MSD_SE_OBJ_POSTER_RIP2, &mPosition);
		SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_CLEAR_SIGN_BIG, 0, nullptr,
		                                   0);
	}

	removeMapCollision();
	onHitFlag(HIT_FLAG_CANNOT_GET_HIT);
	appearObjFromPoint(getObjAppearPos());
	mState = STATE_FINISHED;
}

void TWaterHitPictureHideObj::forward(f32 param_1)
{
	if (mReverse) {
		mProgress -= param_1;
		if (mProgress < mMinProgress)
			afterFinishedAnim();
	} else {
		mProgress += param_1;
		if (mProgress > mMaxProgress)
			afterFinishedAnim();
	}

	mProgress = MsClamp(mProgress, mMinProgress, mMaxProgress);
	mColor.a  = (u16)mProgress & 0xff;
}

u32 TWaterHitPictureHideObj::touchWater(THitActor* param_1)
{
	const JGeometry::TVec3<f32>& waterSpeed = getWaterSpeed(param_1);

	MtxPtr rootMtx = MapObjHideGetRootMtx(this);
	if ((rootMtx[0][2] * waterSpeed.x + rootMtx[1][2] * waterSpeed.y
	     + rootMtx[2][2] * waterSpeed.z)
	    > 0.0f)
		return 0;

	int id = getWaterID(param_1);
	if (MapObjHideWaterMgr()->checkFlagBottom4Bits(id, 0x1)) {
		MapObjHideParticles()->emit(PARTICLE_MS_ENM_WATHIT,
		                            &param_1->getPosition(), 0, nullptr);
		FruitBasketSound()->startSoundSet(MSD_SE_EN_COMMON_W_HIT_OK, &mPosition,
		                                  0, 0, 0, 0, 4);
	}
	forward(mSprayProgressSpeed);
	if (isActorType(0x400001A1))
		soundBas(MSD_SE_OBJ_POSTER_RIP1, 200.0f, mSprayProgressSpeed);

	return 1;
}

void TWaterHitPictureHideObj::touchActor(THitActor* param_1)
{
	if (param_1->isActorType(0x4000005A))
		mState = STATE_HIT_BY_BARREL;
}

void TWaterHitPictureHideObj::control()
{
	TMapObjBase::control();
	switch (mState) {
	case STATE_NORMAL:
		if (mReverse) {
			mProgress += unk15C;
			if (mProgress > mMaxProgress)
				mProgress = mMaxProgress;
		} else {
			mProgress -= unk15C;
			if (mProgress < mMinProgress)
				mProgress = mMinProgress;
		}
		mColor.a = (u16)mProgress & 0xff;
		break;
	case STATE_HIT_BY_BARREL:
		forward(mBarrelProgressSpeed);
		break;
	case STATE_FINISHED:
		break;
	}
}

BOOL TWaterHitPictureHideObj::receiveMessage(THitActor* sender, u32 message)
{
	if (message == HIT_MESSAGE_UNK5
	    && (sender->isActorType(0x2000000E) || sender->isActorType(0x2000000F)
	        || sender->isActorType(0x20000010))) {
		offHitFlag(HIT_FLAG_NO_COLLISION);
		offHitFlag(HIT_FLAG_CANNOT_GET_HIT);
		offHitFlag(HIT_FLAG_CANNOT_ATTACK);
		mState       = STATE_NORMAL;
		mAllowReveal = true;
		return TRUE;
	}

	if (isState(STATE_FINISHED))
		return FALSE;

	if (sender->isActorType(0x4000005A)) {
		mState = STATE_HIT_BY_BARREL;
		return TRUE;
	}

	return THideObjBase::receiveMessage(sender, message);
}

void TWaterHitPictureHideObj::loadAfter()
{
	THideObjBase::loadAfter();

	if (mHiddenObj != nullptr) {
		if (mHiddenObj->isActorType(0x20000010)) {
			bool isBlueCollected = TFlagManager::smInstance->getBlueCoinFlag(
			    SMSGetMarDirector()->getCurrentMap(), mEventId);
			if (isBlueCollected) {
				makeObjDead();
				return;
			}
		}
	}
	switch (mActorType) {
	case 0x40000012:
		mMinProgress = 32.0f;
		mMaxProgress = 100.0f;
		if (MapObjHideGetRootMtx(this)[1][2] > 0.7f) {
			mSprayProgressSpeed = 1.5f;
		} else {
			mSprayProgressSpeed = 0.48f;
		}
		unk15C   = 0.2f;
		mReverse = false;
		break;
	case 0x40000013:
	case 0x40000018:
	case 0x40000019:
	case 0x4000001A:
		mMinProgress = 20.0f;
		mReverse     = true;
		break;
	case 0x40000020:
		mSprayProgressSpeed = 1.2f;
		mMinProgress        = 30.0f;
		mMaxProgress        = 170.0f;
		mReverse            = false;
		break;
	case 0x400001A2:
		mMinProgress        = 0.0f;
		mMaxProgress        = 200.0f;
		mSprayProgressSpeed = 0.4f;
		mReverse            = false;
		break;
	case 0x400001A1:
		mMinProgress = 0.0f;
		mMaxProgress = 255.0f;
		mReverse     = true;
		break;
	}

	if (mReverse)
		mProgress = mMaxProgress;
	else
		mProgress = mMinProgress;

	if (mHiddenObj != nullptr) {
		if (TMapObjBase::isCoin(mHiddenObj))
			mHiddenObj->offMapObjFlag(MAP_OBJ_FLAG_UNK10000000);

		if (mHiddenObj->isActorType(0x20000013)) {
			bool isShineCollected
			    = TFlagManager::smInstance->getShineFlag(mHiddenObj->mEventId);
			if (isShineCollected) {
				if (mReverse)
					mProgress = mMinProgress;
				else
					mProgress = mMaxProgress;

				mColor.a = (u16)mProgress & 0xff;
				mState   = STATE_FINISHED;
			}
		}
	}
}

void TWaterHitPictureHideObj::load(JSUMemoryInputStream& stream)
{
	THideObjBase::load(stream);

	u32 r, g, b;
	stream >> r >> g >> b;
	mColor.r = r & 0xff;
	mColor.g = g & 0xff;
	mColor.b = b & 0xff;

	mColor.a             = 0xFF;
	mSprayProgressSpeed  = 1.8f;
	mBarrelProgressSpeed = 1.0f;
	unk15C               = 0.4f;

	f32 yOffset = MapObjHideGetRootMtx(this)[1][2];
	if (yOffset > 0.7f) {
		mYOffset = 0.0f;
		setDamageHeight(40.0f);
		mPosition.y         = mInitialPosition.y + mYOffset;
		mSprayProgressSpeed = 2.8f;
	} else if (yOffset < -0.7f) {
		f32 scaleY = mScaling.y;
		mYOffset   = scaleY * -10.0f;
		setDamageHeight(30.0f);
		mPosition.y         = mInitialPosition.y + mYOffset;
		mSprayProgressSpeed = 2.5f;
	} else {
		mSprayProgressSpeed = 1.8f;
	}

	initPacketMatColor(MapObjHideGetModel(this), GX_TEVREG0, &mColor);
}

TWaterHitPictureHideObj::TWaterHitPictureHideObj(const char* name)
    : THideObjBase(name)
    , mReverse(true)
    , mSprayProgressSpeed(0.0f)
    , mBarrelProgressSpeed(0.0f)
    , unk15C(0.0f)
    , mProgress(0.0f)
    , mMinProgress(0.0f)
    , mMaxProgress(255.0f)
{
	mColor.r = 0;
	mColor.g = 0;
	mColor.b = 0;
	mColor.a = 0;
}

// Binding level over the director: with getPosition() on the twin it lands
// afterFinishedAnim's 0x70 frame and matrix slot.
static inline TMarDirector* HideObjGetMarDirector()
{
	TMarDirector* director = gpMarDirector;
	return director;
}

void THideObjPictureTwin::afterFinishedAnim()
{
	removeMapCollision();
	onHitFlag(HIT_FLAG_NO_COLLISION);

	if (mHiddenObj != nullptr && mAllowReveal) {
		TMapObjBase* obj = mHiddenObj;
		if (TMapObjBase::isCoin(obj)) {
			TCoin* coin = (TCoin*)obj;
			if (coin->isActorType(0x2000000E)) {
				obj = gpItemManager->makeObjAppear(0x2000000E);
				if (obj == nullptr)
					return;
			}
			((TCoin*)obj)->appearWithoutSound();
		} else {
			obj->appear();
		}

		obj->mPosition.set(unk174->getPosition());
		Mtx mtx;
		MsMtxSetRotRPH(mtx, unk174->mRotation.x, unk174->mRotation.y,
		               unk174->mRotation.z);
		obj->mVelocity.set(mtx[0][2] * mAppearSpeed,
		                   mtx[1][2] * mAppearSpeed + mAppearYSpeed,
		                   mtx[2][2] * mAppearSpeed);
		obj->offLiveFlag(LIVE_FLAG_UNK10);

		if (TMapObjBase::isCoin(obj)) {
			TCoin* coin      = (TCoin*)obj;
			coin->mContainer = this;
			coin->unk14C     = unk148;
		}
		SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_TIMECOIN_APPEAR, 0,
		                                   nullptr, 0);

		HideObjGetMarDirector()->fireStartDemoCamera(
		    unk178, &obj->mPosition, -1, 0.0f, true, nullptr, 0, nullptr, 0);
	}
	mState = 3;
}

// Setter level over the partner link: +4 of low region, putting the name
// buffer at retail's 0x28.
static inline void HideObjSetTwin(THideObjPictureTwin* obj,
                                  THideObjPictureTwin* twin)
{
	obj->unk174 = twin;
}

void THideObjPictureTwin::loadAfter()
{
	TWaterHitPictureHideObj::loadAfter();
	char* wrapName = strstr(mName, "ふたご落書きＡ");
	if (wrapName != nullptr) {
		size_t len = strlen("ふたご落書きＡ");
		char c0    = mName[len];
		char c1    = mName[len + 1];
		char c2    = mName[len + 2];
		char c3    = mName[len + 3];

		char buffer2[0x48];
		snprintf(buffer2, 0x40, "ふたご落書きＢ００");
		buffer2[len]     = c0;
		buffer2[len + 1] = c1;
		buffer2[len + 2] = c2;
		buffer2[len + 3] = c3;

		unk174 = JDrama::TNameRefGen::getInstance()->search<THideObjPictureTwin>(
		    buffer2);
		HideObjSetTwin(unk174, this);
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
	mStateTimer = -1;
	mState      = 2;
}

BOOL TBreakHideObj::receiveMessage(THitActor* sender, u32 message)
{
	if (message == 1) {
		if (isActorType(0x400002C3)) {
			emitAndScale(0x6B, 0, &mPosition);
			emitAndScale(0x6C, 0, &mPosition);
			SMSGetMSound()->startSoundActor(MSD_SE_OBJ_WATERMELON_BLOCK, &mPosition);
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
			appearObj(0.0f);
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

// Binding levels: with kill()'s named sound pointer they supply the 0x98
// bytes of low region killNearWoodBox's four expansions leave in retail.
static inline const JGeometry::TVec3<f32>& MapObjHideMarioPos()
{
	const JGeometry::TVec3<f32>& pos = SMS_GetMarioPos();
	return pos;
}

static inline MSound* MapObjHideMSound()
{
	MSound* sound = gpMSound;
	return sound;
}

void TWoodBox::killNearWoodBox(f32 dX, f32 dY) const
{
	const TBGCheckData* groundPlane;
	f32 resY = gpMap->checkGround(dX + MapObjHideMarioPos().x, MapObjHideMarioPos().y + 1000.0f,
	                              dY + MapObjHideMarioPos().z, &groundPlane);
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
	mStateTimer = -1;
	mState      = 2;

	MSound* sound = MapObjHideMSound();
	sound->startSoundActor(MSD_SE_IT_BARREL_CRASH, &mPosition, 0, nullptr, 0,
	                       4);

	killNearWoodBox(-50.0f, -50.0f);
	killNearWoodBox(50.0f, -50.0f);
	killNearWoodBox(-50.0f, 50.0f);
	killNearWoodBox(50.0f, 50.0f);
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
