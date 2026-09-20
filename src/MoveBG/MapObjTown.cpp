#include <MoveBG/MapObjTown.hpp>
#include <MoveBG/ItemManager.hpp>
#include <MoveBG/Item.hpp>
#include <System/MarDirector.hpp>
#include <System/Particles.hpp>
#include <System/StageUtil.hpp>
#include <System/FlagManager.hpp>
#include <Player/Mario.hpp>
#include <Player/Yoshi.hpp>
#include <Player/MarioAccess.hpp>
#include <Map/MapData.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <Map/MapCollisionManager.hpp>
#include <M3DUtil/MActor.hpp>
#include <MSound/MSound.hpp>
#include <MSound/SoundEffects.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/string.h>
#include <stdio.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

TMapObjSwitch* gpMapObjSwitch;

f32 TManhole::mDownHeight            = 12.0f;
f32 TManhole::mDownSpeed             = 1.5f;
f32 TManhole::mVibrationSpeed        = 0.05f;
f32 TManhole::mVibrationEndHeight    = 0.001f;
f32 TManhole::mVibrationDecreaseRate = 0.07f;

void TDoor::touchPlayer(THitActor* param_1)
{
	if (param_1->isActorType(0x80000001)) {
		u32 message = HIT_MESSAGE_UNK11;
		if (unk138)
			message = HIT_MESSAGE_UNK12;
		if (param_1->receiveMessage(this, message)) {
			TMario* mario = (TMario*)param_1;
			if (mario->getStatus() == MARIO_STATUS_DOOR_OPEN_R) {
				if (mario->getHeldObject() != nullptr)
					startAnim(4);
				else
					startAnim(2);
			} else if (mario->getStatus() == MARIO_STATUS_DOOR_OPEN_L) {
				if (mario->getHeldObject() != nullptr)
					startAnim(3);
				else
					startAnim(1);
			}
		}
	}
}

void TDoor::load(JSUMemoryInputStream& stream)
{
	TMapObjBase::load(stream);
	if (stream.readS32())
		unk138 = true;
}

TDoor::TDoor(const char* name)
    : TMapObjBase(name)
    , unk138(false)
{
}

// TODO: 98.8%, frame 0x48 against retail's 0x90. Retail binds
// &mInitialPosition.y once (`addi r3, r31, 0x110`) and reads the sink offset
// through it; a TU-local reference-returning inline is folded straight back to
// the member offset, so the binding has to come from somewhere else.
void TManhole::touchPlayer(THitActor*)
{
	mState = STATE_NORMAL;
	if (!animationFinished()) {
		mPosition.y = mInitialPosition.y;
		return;
	}
	if (gpMarioOriginal->getStatus() == MARIO_STATUS_HIP_DROP
	    && gpMarioOriginal->mPosition.y < mPosition.y) {
		getMActor()->getFrameCtrl(ANM_TYPE_BCK)->setRate(SMSGetAnmFrameRate());
		getMActor()
		    ->getFrameCtrl(ANM_TYPE_BCK)
		    ->setFrame(getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getFrame()
		               + SMSGetAnmFrameRate());
		SMSGetMSound()->startSoundActor(MSD_SE_OBJ_MANHOLE_OPEN, &mPosition, 0,
		                                nullptr, 0, 4);
		offMapObjFlag(MAP_OBJ_FLAG_UNK100);
		SMSRumbleMgr->start(0x15, 0xF, (f32*)nullptr);
		return;
	}

	if (gpMarioOriginal->mPosition.y < mPosition.y
	    && gpMarioOriginal->mVel.y > 0.0f) {
		getMActor()->getFrameCtrl(ANM_TYPE_BCK)->setRate(SMSGetAnmFrameRate());
		getMActor()
		    ->getFrameCtrl(ANM_TYPE_BCK)
		    ->setFrame(getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getFrame()
		               + SMSGetAnmFrameRate());
		offMapObjFlag(MAP_OBJ_FLAG_UNK100);
		mMapCollisionManager->unk8->setAllBGType(0x400);
		SMSGetMSound()->startSoundActor(MSD_SE_OBJ_MANHOLE_OPEN, &mPosition, 0,
		                                nullptr, 0, 4);
		unk150 = 1;
		SMSRumbleMgr->start(0x15, 0xF, (f32*)nullptr);
		return;
	}

	if (gpMarioOriginal->isTouchGround4cm()) {
		if (unk151) {
			setUpMapCollision(1);
			unk151 = 0;
		}
		if (!unk152) {
			unk152 = 1;
			SMSGetMSound()->startSoundActor(MSD_SE_OBJ_MANHOLE_DOWN, &mPosition,
			                                0, nullptr, 0, 4);
		}
		if (mPosition.y > mInitialPosition.y - mDownHeight)
			mPosition.y = mPosition.y - mDownSpeed;
		else
			mPosition.y = mInitialPosition.y - mDownHeight;
		unk148 = 1.0f;
		unk14C = mInitialPosition.y - mPosition.y;
		offMapObjFlag(MAP_OBJ_FLAG_UNK100);
		return;
	}

	if (unk152) {
		unk152 = 0;
		SMSGetMSound()->startSoundActor(MSD_SE_OBJ_MANHOLE_UP, &mPosition, 0,
		                                nullptr, 0, 4);
	}
	appeared();
}

// Binding level worth +16 of low region, landing
// TManhole::animationFinished's frame at 0x58 (batch 121).
static inline MActor* MapObjTownGetMActor(const TManhole* p)
{
	MActor* mActor = p->getMActor();
	return mActor;
}

bool TManhole::animationFinished()
{
	J3DFrameCtrl* frameCtrl
	    = MapObjTownGetMActor(this)->getFrameCtrl(ANM_TYPE_BCK);
	if (frameCtrl->getRate() == 0.0f)
		return true;
	f32 next = getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getFrame()
	           + getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getRate();
	if (frameCtrl->getFrame() < 79.0f && next >= 79.0f) {
		frameCtrl->setFrame(79.0f);
		frameCtrl->setRate(0.0f);
		calcRootMatrix();
		getModel()->calc();
		onMapObjFlag(MAP_OBJ_FLAG_UNK100);
		return true;
	}
	f32 end = frameCtrl->getEnd();
	if (frameCtrl->getFrame() < end && next >= end) {
		frameCtrl->setFrame(0.0f);
		frameCtrl->setRate(0.0f);
		calcRootMatrix();
		getModel()->calc();
		onMapObjFlag(MAP_OBJ_FLAG_UNK100);
		return true;
	}
	return false;
}

void TManhole::appeared()
{
	const TMapObjBase* assoc = unk154;
	if (assoc) {
		if (assoc->checkLiveFlag(LIVE_FLAG_DEAD)) {
			unk158->remove();
			unk154 = nullptr;
		} else {
			return;
		}
	}
	if (unk150) {
		if (gpMarioOriginal->mVel.y <= 0.0f) {
			mMapCollisionManager->unk8->setAllBGType(
			    BG_TYPE_GROUND_POUND_TO_PASS_THROUGH);
			unk150 = 0;
		}
	}
	if (animationFinished()) {
		if (unk152 == 1 && mColCount == 0) {
			unk152 = 0;
			SMSGetMSound()->startSoundActor(MSD_SE_OBJ_MANHOLE_UP, &mPosition,
			                                0, nullptr, 0, 4);
		}
		if (unk14C > mVibrationEndHeight) {
			mPosition.y = unk14C * JMASCos((s16)(unk148 * 32768.0f))
			              + mInitialPosition.y;
			unk148 += mVibrationSpeed;
			if (unk148 >= 2.0f)
				unk148 = unk148 - 2.0f;
			unk14C -= mVibrationDecreaseRate;
			return;
		}
		if (!unk151) {
			setUpMapCollision(0);
			unk151 = 1;
		}
		onMapObjFlag(MAP_OBJ_FLAG_UNK100);
		mPosition.y = mInitialPosition.y;
	}
}

void TManhole::calc()
{
	f32 next = getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getFrame()
	           + getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getRate();
	if ((getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getFrame() <= 45.0f
	     && 45.0f < next)
	    || (getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getFrame() <= 125.0f
	        && 125.0f < next)) {
		SMSGetMSound()->startSoundActor(MSD_SE_OBJ_MANHOLE_CLOSE, &mPosition, 0,
		                                nullptr, 0, 4);
	}
}

void TManhole::setGroundCollision()
{
	if (SMS_GetYoshi()->isHatched()
	    && mPosition.x - mBodyRadius < SMS_GetYoshi()->getTranslation().x
	    && mPosition.x + mBodyRadius > SMS_GetYoshi()->getTranslation().x
	    && mPosition.z - mBodyRadius < SMS_GetYoshi()->getTranslation().z
	    && mPosition.z + mBodyRadius > SMS_GetYoshi()->getTranslation().z) {
		if (mMapCollisionManager->unk8)
			mMapCollisionManager->unk8->moveTrans(mPosition);
	} else {
		TMapObjBase::setGroundCollision();
	}
}

void TManhole::makeManholeUnuseful(const TMapObjBase* param_1)
{
	if (unk154 == nullptr) {
		unk154 = param_1;
		unk158->setUpMtx(getModel()->getAnmMtx(0));
	}
}

void TManhole::loadAfter()
{
	TMapObjBase::loadAfter();
	getMActor()->getFrameCtrl(ANM_TYPE_BCK)->setRate(0.0f);
}

void TManhole::initMapObj()
{
	TMapObjGeneral::initMapObj();
	TMapCollisionWarp* col = new TMapCollisionWarp;
	unk158                 = col;
	unk158->init("/scene/mapObj/manholeRoof.col", 0, this);
}

TManhole::TManhole(const char* name)
    : TMapObjGeneral(name)
    , unk148(0.0f)
    , unk14C(0.0f)
    , unk150(0)
    , unk151(1)
    , unk152(0)
    , unk154(nullptr)
    , unk158(nullptr)
{
}

// Named-local fork over gpMSound, worth +8 of low region at each site that
// expands TMapObjBillboard::swing and in TMapObjChangeStage::touchPlayer
// (batch 127).
static inline MSound* MapObjTownGetMSound()
{
	MSound* mSound = gpMSound;
	return mSound;
}

void TMapObjBillboard::swing(THitActor* param_1)
{
	if (animIsFinished()
	    || !(getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getFrame() < 33.0f)) {
		f32 tmp   = 57.295776f * getRotYFromAxisX(param_1->mPosition);
		f32 angle = MsWrap(mRotation.y + tmp, 0.0f, 360.0f);
		if (angle < 0.0f || 180.0f < angle)
			startAnim(2);
		else
			startAnim(1);
		MapObjTownGetMSound()->startSoundActor(MSD_SE_OBJ_BILLBOARD_MOVE,
		                                       &mPosition, 0, &unk150, 0, 4);
	}
}

void TMapObjBillboard::touchActor(THitActor* param_1) { swing(param_1); }

// Direct-return fork over the hidden-object member read.
static inline TMapObjBase* MapObjTownHiddenObj(const TMapObjBillboard* p)
{
	return p->mHiddenObj;
}

// Binding level over a raw member read, worth +16 of low region in
// TMapObjBillboard::touchWater (batch 127).
// Binding level over a raw member read, worth +16 of low region in
// TMapObjBillboard::touchWater (batch 127).
static inline bool MapObjTownAllowReveal(const TMapObjBillboard* p)
{
	bool allowReveal = p->mAllowReveal;
	return allowReveal;
}

u32 TMapObjBillboard::touchWater(THitActor* param_1)
{
	swing(param_1);
	if (MapObjTownHiddenObj(this) && MapObjTownAllowReveal(this)) {
		JGeometry::TVec3<f32> rot = mRotation;
		JGeometry::TVec3<f32> pos = mPosition;
		rot.y -= 90.0f;
		pos.y += getObjCollisionHeightOffset();
		TMapObjBase* obj = MapObjTownHiddenObj(this);
		if (obj->isActorType(0x2000000E))
			obj = gpItemManager->makeObjAppear(0x2000000E);
		if (obj) {
			TMapObjBase::throwObjFromPointWithRot(obj, pos, rot, mAppearSpeed,
			                                      mAppearYSpeed);
			emitEffect();
			mAllowReveal = false;
		}
	}
	return 1;
}

void TMapObjChangeStage::touchPlayer(THitActor*)
{
	gpMarDirector->setNextStage(unk138, nullptr);
	onHitFlag(HIT_FLAG_NO_COLLISION);
	mColCount = 0;
	MapObjTownGetMSound()->startSoundActor(MSD_SE_MA_WARP_EX, &mPosition, 0, nullptr, 0, 4);
}

void TMapObjChangeStage::load(JSUMemoryInputStream& stream)
{
	TMapObjBase::load(stream);
	u32 val;
	stream >> val;
	unk138 = val;
}

// Binding level over the director singleton, worth +0x10 of low region in
// TMapObjChangeStageHipDrop::touchPlayer.
static inline TMarDirector* MapObjTownGetMarDirector()
{
	TMarDirector* director = SMSGetMarDirector();
	return director;
}

void TMapObjChangeStageHipDrop::touchPlayer(THitActor*)
{
	if (SMS_IsMarioStatusHipDrop()
	    && gpMarioPos->y + *gpMarioSpeedY < SMS_GetMarioGrLevel()) {
		MapObjTownGetMarDirector()->setNextStage(unk138, nullptr);
		gpMarioParticleManager->emit(MAPOBJ_MS_EX_HAHEN, &mPosition, 0,
		                             nullptr);
	}
}

void TMapObjChangeStageHipDrop::initMapObj()
{
	TMapObjChangeStage::initMapObj();
	SMS_LoadParticle("/scene/mapObj/ms_ex_hahen.jpa", MAPOBJ_MS_EX_HAHEN);
}

void TMapObjStartDemo::touchPlayer(THitActor*)
{
	SMSGetMarDirector()->fireStreamingMovie((u8)unk138);
}

void TMapObjStartDemo::load(JSUMemoryInputStream& stream)
{
	TMapObjBase::load(stream);
	s32 v;
	stream >> v;
	unk138 = v;
}

void TDamageObj::perform(u32 cue, JDrama::TGraphics* graphics)
{
	THitActor::perform(cue, graphics);
	if (mColCount)
		mCollisions[0]->receiveMessage(this, HIT_MESSAGE_ATTACK);
}

void TDamageObj::init(u32 param_1)
{
	initHitActor(param_1, 1, 0x80000000, 50.0f * mScaling.x,
	             100.0f * mScaling.y, 0.0f, 0.0f);
	offHitFlag(HIT_FLAG_NO_COLLISION);
	TMapObjBase::joinToGroup("マップグループ", this);
}

void TDamageObj::load(JSUMemoryInputStream& stream)
{
	JDrama::TActor::load(stream);
	char name[0x20];
	stream.readString(name, 0x20);
	if (strcmp(name, "normal") == 0) {
		initHitActor(0x10000036, 1, 0x80000000, 50.0f * mScaling.x,
		             100.0f * mScaling.y, 0.0f, 0.0f);
		offHitFlag(HIT_FLAG_NO_COLLISION);
		TMapObjBase::joinToGroup("マップグループ", this);
		return;
	}
	if (strcmp(name, "water") == 0) {
		initHitActor(0x40000053, 1, 0x80000000, 50.0f * mScaling.x,
		             100.0f * mScaling.y, 0.0f, 0.0f);
		offHitFlag(HIT_FLAG_NO_COLLISION);
		TMapObjBase::joinToGroup("マップグループ", this);
	}
}

void TShadowObj::load(JSUMemoryInputStream&) { }

void TMapObjWaterSpray::calc()
{
	JPABaseEmitter* em
	    = gpMarioParticleManager->emit(unk138, &mPosition, 1, this);
	if (em) {
		em->setRotation(mRotation.x, mRotation.y, mRotation.z);
		em->setGlobalScale(getScaling());
		em->setRate(unk13C);
		em->setGlobalParticleScale(unk140);
		em->setGlobalPrmColor(unk14C.r, unk14C.g, unk14C.b);
		em->setGlobalAlpha(unk14C.a);
	}
}

void TMapObjWaterSpray::load(JSUMemoryInputStream& stream)
{
	TMapObjBase::load(stream);
	if (strcmp(unkF4, "WaterSprayCylinder") == 0) {
		unk138 = 0x154;
		SMS_LoadParticle("/scene/mapObj/ms_shib_cyl1.jpa", unk138);
	} else {
		unk138 = 0x155;
		SMS_LoadParticle("/scene/mapObj/ms_shib_cub1.jpa", unk138);
	}

	stream >> unk13C;
	if (unk13C > 100.0f)
		unk13C = 0.5f;
	else
		unk13C = unk13C / 100.0f;

	f32 scale;
	stream >> scale;
	if (scale == -1.0f)
		scale = 1.0f;
	else
		scale /= 100.0f;
	unk140.set(scale, scale, scale);

	u32 cVal;
	stream >> cVal;
	unk14C.r = cVal;
	stream >> cVal;
	unk14C.g = cVal;
	stream >> cVal;
	unk14C.b = cVal;
	stream >> cVal;
	unk14C.a = cVal;
}

TMapObjWaterSpray::TMapObjWaterSpray(const char* name)
    : TMapObjBase(name)
    , unk138(0x154)
    , unk13C(0.0f)
    , unk14C(-1)
{
	unk14C.set(0, 0, 0, 0);
	unk140.zero();
}

void THideObjInfo::action(s32 param_1)
{
	TMapObjBase* obj = gpItemManager->makeObjAppear(mPosition.x, mPosition.y,
	                                                mPosition.z, unk44, true);
	if (obj) {
		TMapObjBase::throwObjFromPointWithRot(obj, mPosition, mRotation, unk48,
		                                      unk4C);
		if (obj->isActorType(0x2000000E))
			((TItem*)obj)->unk14C = param_1;
	}
}

void THideObjInfo::load(JSUMemoryInputStream& stream)
{
	JDrama::TActor::load(stream);
	s32 eventId;
	s32 timer;
	TMapObjBase::loadHideObjInfo(stream, &eventId, &unk48, &unk4C, &timer);
	unk44 = TMapObjBaseManager::getActorTypeByEventID(eventId);
	gpMapObjSwitch->registerObjInfo(this);
}

THideObjInfo::THideObjInfo(const char* name)
    : JDrama::TActor(name)
{
	unk44 = 0;
	unk48 = 0.0f;
	unk4C = 0.0f;
}

// TODO: 92.3%. Retail reloads mStateTimer at the call (`lwz r4, 0x104`
// *after* the gate's own load) where MWCC keeps the gate's value alive; a
// `getStateTimer()` argument and a TU-local const-pointer gate are both
// byte-identical to this, so whatever breaks the CSE is not an accessor.
void TMapObjSwitch::control()
{
	TMapObjBase::control();
	if (isStateTimerEngaged())
		SMSGetMSound()->playTimer(mStateTimer);
}

BOOL TMapObjSwitch::receiveMessage(THitActor*, u32 message)
{
	if (message == HIT_MESSAGE_HIP_DROP) {
		startBck("objswitch");
		SMSGetMSound()->startSoundActor(MSD_SE_OBJ_AP_BUTTON, &mPosition);
		removeMapCollision();
		for (int i = 0; i < unk13C; ++i)
			unk144[i]->action(unk140);
		SMSGetMarDirector()->fireStartDemoCamera(
		    "オブジェスイッチ用カメラ", nullptr, -1, 0.0f, true, nullptr, 0,
		    nullptr, JDrama::TFlagT<u16>(0));
		mStateTimer = unk140;
		onHitFlag(HIT_FLAG_NO_COLLISION);
		return TRUE;
	}

	return FALSE;
}

void TMapObjSwitch::registerObjInfo(THideObjInfo* info)
{
	unk144[unk13C] = info;
	unk13C += 1;
}

// Binding level over a raw member read, worth +16 of low region in
// TMapObjSwitch::load (batch 127).
static inline s32 MapObjTownUnk138L0(const TMapObjSwitch* p)
{
	s32 v138 = p->unk138;
	return v138;
}

static inline s32 MapObjTownUnk138(const TMapObjSwitch* p)
{
	s32 v138 = MapObjTownUnk138L0(p);
	return v138;
}

void TMapObjSwitch::load(JSUMemoryInputStream& stream)
{
	TMapObjBase::load(stream);
	stream >> unk140;
	if (unk140 <= 0)
		unk140 = 1200;
	else
		unk140 *= 10;

	s32 r;
	s32 g;
	s32 b;
	stream >> r;
	stream >> g;
	stream >> b;
	unk148.r = (u8)r;
	unk148.g = (u8)g;
	unk148.b = (u8)b;

	unk138 = 100;
	unk144 = new THideObjInfo*[MapObjTownUnk138(this)];

	SMS_LoadParticle("/scene/mapObj/ms_watcoin_hit.jpa", 0x57);
}

TMapObjSwitch::TMapObjSwitch(const char* name)
    : TMapObjBase(name)
{
	unk138   = 0;
	unk13C   = 0;
	unk140   = 0;
	unk148.r = 0xff;
	unk148.g = 0xff;
	unk148.b = 0xff;
	unk148.a = 0xff;
	for (int i = 0; i < unk138; ++i)
		unk144[i] = nullptr;
	gpMapObjSwitch = this;
}

BOOL TRedCoinSwitch::receiveMessage(THitActor*, u32 message)
{
	if (message == HIT_MESSAGE_HIP_DROP) {
		startBck("redcoinswitch");
		gpMarDirector->unk18[0]->mDisabledFrames
		    = (s32)(getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getEnd() * 2
		            + 0x3C);
		gpMSound->startSoundActor(MSD_SE_OBJ_AP_BUTTON, &mPosition, 0, nullptr,
		                          0, 4);
		removeMapCollision();
		onHitFlag(HIT_FLAG_NO_COLLISION);
		mState = 2;
		return TRUE;
	}

	return FALSE;
}

// Binding level worth +8 of low region, landing TRedCoinSwitch::load's frame
// at 0x30 (batch 121).
static inline TFlagManager* MapObjTownGetInstance()
{
	TFlagManager* instance = TFlagManager::getInstance();
	return instance;
}

void TRedCoinSwitch::control()
{
	TMapObjBase::control();
	// TODO: 95.0%. `case 1: return;` is what keeps the case in the pivot
	// tree at all (as `break` MWCC folds it into the default and the tree
	// loses the `cmpwi 1`), but retail's tree still pivots on 3 and ours on
	// 2; reordering the arms only moves the bodies. Frame is 8 short.
	switch (mState) {
	case 1:
	case 4:
		break;
	case 2:
		if (getMActor()->curAnmEndsNext(ANM_TYPE_BCK, nullptr)) {
			mStateTimer = 120;
			mState      = 3;
			MapObjTownGetInstance()->setBool(true, 0x50009);
		}
		break;
	case 3:
		if (!isStateTimerEngaged())
			mState = 4;
		break;
	}
}

void TRedCoinSwitch::loadAfter()
{
	TMapObjBase::loadAfter();
	for (int i = 0; i < 8; ++i) {
		char buf[0x40];
		snprintf(buf, 0x40, "赤コイン %d", i);
		TMapObjBase* coin
		    = JDrama::TNameRefGen::getInstance()->search<TMapObjBase>(buf);
		coin->makeObjDead();
	}
}

void TRedCoinSwitch::load(JSUMemoryInputStream& stream)
{
	TMapObjBase::load(stream);
	u32 tmp;
	stream >> tmp;
	unk138 = tmp;
	if (unk138 <= 0)
		unk138 = 1200;
	else
		unk138 *= 10;

	u8 shineId = SMS_getShineIDofExStage(gpMarDirector->getCurrentMap());
	if (shineId != 0xFF
	    && !MapObjTownGetInstance()->getShineFlag(shineId)) {
		makeObjDead();
	}
}

TRedCoinSwitch::TRedCoinSwitch(const char* name)
    : TMapObjBase(name)
{
	unk138   = 0;
	unk13C.r = 0xFF;
	unk13C.g = 0xFF;
	unk13C.b = 0xFF;
	unk13C.a = 0xFF;
}

void TBasketReverse::kill()
{
	gpMarioParticleManager->emitAndBindToPosPtr(PARTICLE_MS_ENM_DISAP_A_W,
	                                            &mPosition, 0, nullptr);
	gpMarioParticleManager->emitAndBindToPosPtr(PARTICLE_MS_ENM_DISAP_B,
	                                            &mPosition, 0, nullptr);

	SMSGetMSound()->startSoundActor(MSD_SE_IT_BARREL_CRASH, &mPosition);
	SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_COLLECT_DELIGHT, 0, nullptr,
	                                   0);
	TMapObjBase::makeObjDead();
}

void TBasketReverse::initMapObj()
{
	mPosition.y += 200.0f;
	TMapObjBase::initMapObj();
}
