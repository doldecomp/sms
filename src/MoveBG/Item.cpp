#include <MoveBG/Item.hpp>
#include <MoveBG/ItemManager.hpp>
#include <Map/MapMirror.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <System/Application.hpp>
#include <System/StageUtil.hpp>
#include <System/FlagManager.hpp>
#include <System/MarDirector.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/Particles.hpp>
#include <Strategic/MirrorActor.hpp>
#include <Strategic/question.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/WaterGun.hpp>
#include <Player/Yoshi.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorUtil.hpp>
#include <Strategic/ObjModel.hpp>
#include <Camera/Camera.hpp>
#include <Camera/CameraMapTool.hpp>
#include <MarioUtil/LightUtil.hpp>
#include <MarioUtil/PacketUtil.hpp>
#include <GC2D/GCConsole2.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/JParticle/JPAResourceManager.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoaderFlags.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

f32 TItem::mAppearedScaleSpeed = 0.01f;

void TItem::appeared()
{
	if (checkMapObjFlag(MAP_OBJ_FLAG_DISAPPEARING) && !isStateTimerEngaged()) {
		if (mContainer != nullptr)
			mContainer->receiveMessage(this, HIT_MESSAGE_UNK5);

		if (isActorType(0x2000000f) || isActorType(0x20000010)) {
			if (gpMSound->gateCheck(MSD_SE_SY_COIN_DISAPPEAR))
				MSoundSESystem::MSoundSE::startSoundActor(
				    MSD_SE_SY_COIN_DISAPPEAR, mPosition, 0, nullptr, 0, 4);
		}
	}

	TMapObjGeneral::appeared();
}

void TItem::taken(THitActor* param_1)
{
	param_1->receiveMessage(this, HIT_MESSAGE_ATTACK);
	kill();
	if (checkMapObjFlag(MAP_OBJ_FLAG_RESPAWNING)) {
		makeObjDefault();
		appear();
	}
}

void TItem::touchPlayer(THitActor* param_1)
{
	if ((param_1->isActorType(0x80000001) || param_1->isActorType(0x8000083))
	    && !checkHitFlag(HIT_FLAG_NO_COLLISION))
		taken(param_1);
}

BOOL TItem::receiveMessage(THitActor* sender, u32 message)
{
	if (message == HIT_MESSAGE_SPRAYED_BY_WATER)
		return false;

	if (message == HIT_MESSAGE_UNKB) {
		taken(sender);
		return true;
	}

	return TMapObjGeneral::receiveMessage(sender, message);
}

void TItem::calcRootMatrix()
{
	if (!checkMapObjFlag(MAP_OBJ_FLAG_UNK8000000))
		TMapObjGeneral::calcRootMatrix();
}

void TItem::calc()
{
	if (!checkMapObjFlag(MAP_OBJ_FLAG_UNK4000000) && !isState(STATE_HOLDING)) {
		MtxPtr src = gpItemManager->unk40;

		MtxPtr mtx;
		if (checkMapObjFlag(MAP_OBJ_FLAG_UNK100))
			mtx = getModel()->getAnmMtx(0);
		else
			mtx = getModel()->getBaseTRMtx();

		mtx[0][0] = src[0][0];
		mtx[0][1] = src[0][1];
		mtx[0][2] = src[0][2];
		mtx[0][3] = mPosition.x;

		mtx[1][0] = src[1][0];
		mtx[1][1] = src[1][1];
		mtx[1][2] = src[1][2];
		mtx[1][3] = mPosition.y;

		mtx[2][0] = src[2][0];
		mtx[2][1] = src[2][1];
		mtx[2][2] = src[2][2];
		mtx[2][3] = mPosition.z;
	}

	if (isState(STATE_HOLDING) && checkMapObjFlag(MAP_OBJ_FLAG_UNK100))
		TMapObjGeneral::calcRootMatrix();
}

void TItem::appearing()
{
	if (checkMapObjFlag(MAP_OBJ_FLAG_UNK2000000)) {
		if (mScaling.x < 2.0f) {
			mScaling.add((Vec) { mAppearedScaleSpeed * 2.0f,
			                     mAppearedScaleSpeed * 2.0f,
			                     mAppearedScaleSpeed * 2.0f });
		} else {
			makeObjAppeared();
			onHitFlag(HIT_FLAG_NO_COLLISION);
			offMapObjFlag(MAP_OBJ_FLAG_DISAPPEARING);
		}
	} else {
		TMapObjGeneral::appearing();
	}
}

void TItem::killByTimer(int param_1)
{
	unk14C      = param_1;
	mStateTimer = unk150;

	offMapObjFlag(MAP_OBJ_FLAG_UNK10000000);
	onHitFlag(HIT_FLAG_NO_COLLISION);
	offMapObjFlag(MAP_OBJ_FLAG_DISAPPEARING);
}

void TItem::appear()
{
	TMapObjGeneral::appear();
	onHitFlag(HIT_FLAG_NO_COLLISION);
	mStateTimer = unk150;
	offMapObjFlag(MAP_OBJ_FLAG_DISAPPEARING);
}

void TItem::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (checkLiveFlag(LIVE_FLAG_DEAD))
		return;

	if ((cue & CUE_MOVE) && checkHitFlag(HIT_FLAG_NO_COLLISION)
	    && !isStateTimerEngaged()) {
		offHitFlag(HIT_FLAG_NO_COLLISION);
		if (!checkMapObjFlag(MAP_OBJ_FLAG_UNK10000000)) {
			onMapObjFlag(MAP_OBJ_FLAG_DISAPPEARING);
			mStateTimer = unk14C;
		}
	}

	TMapObjGeneral::perform(cue, graphics);
}

void TItem::initMapObj()
{
	TMapObjGeneral::initMapObj();
	unk14C = 480;
	unk150 = 120;
}

void TItem::load(JSUMemoryInputStream& stream)
{
	TMapObjGeneral::load(stream);
	onMapObjFlag(MAP_OBJ_FLAG_UNK10000000);
}

TItem::TItem(const char* name)
    : TMapObjGeneral(name)
    , mContainer(nullptr)
    , unk14C(0)
    , unk150(0)
{
}

void TCoin::taken(THitActor* param_1)
{
	u8 thing = gpApplication.mCurrArea.unk0;
	TFlagManager::getInstance()->incGoldCoinFlag(SMS_getShineStage(thing), 1);

	if (gpMSound->gateCheck(MSD_SE_SY_COIN))
		MSoundSESystem::MSoundSE::startSoundActor(MSD_SE_SY_COIN, mPosition, 0,
		                                          nullptr, 0, 4);

	if (mContainer)
		mContainer->receiveMessage(this, HIT_MESSAGE_UNK8);

	if (TFlagManager::smInstance->getFlag(0x40002) == 100) {
		TShine* shine = JDrama::TNameRefGen::search<TShine>(
		    "シャイン（１００枚コイン用）");

		gpItemManager->makeShineAppearWithDemo(
		    "シャイン（１００枚コイン用）",
		    "シャイン（１００枚コイン用）カメラ", mPosition.x, mPosition.y,
		    mPosition.z);
	}

	TItem::taken(param_1);
}

void TCoin::makeObjDead()
{
	TItem::makeObjDead();
	if (unk154)
		unk154->unk1A |= 1;
}

void TCoin::appearWithoutSound()
{
	TItem::appear();
	gpMarioParticleManager->emitAndBindToMtxPtr(
	    MAPOBJ_MS_WATCOIN_KIRA, getModel()->getAnmMtx(0), 0, this);
	if (isActorType(0x2000000e))
		offMapObjFlag(MAP_OBJ_FLAG_UNK10000000);
}

void TCoin::appear()
{
	if (isActorType(0x20000010)) {
		if (!TFlagManager::smInstance->getBlueCoinFlag(
		        gpMarDirector->getCurrentMap(), mEventId))
			SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_TIMECOIN_APPEAR, 0,
			                                   nullptr, 0);
	} else {
		SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_COIN_APPEAR, 0, nullptr,
		                                   0);
	}

	appearWithoutSound();
}

void TCoin::makeObjAppeared()
{
	TItem::makeObjAppeared();
	if (unk154)
		unk154->unk1A &= ~1;
}

void TCoin::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (checkLiveFlag(LIVE_FLAG_DEAD))
		return;

	if ((cue & CUE_MOVE) && checkLiveFlag(LIVE_FLAG_UNK10)) {

		if (gpMarDirector->isTalkModeNow() && !gpMarDirector->isDemoModeNow())
			return;

		if (isStateTimerEngaged()) {
			--mStateTimer;
		} else {
			if (checkHitFlag(HIT_FLAG_NO_COLLISION)) {
				offHitFlag(HIT_FLAG_NO_COLLISION);
				if (!checkMapObjFlag(MAP_OBJ_FLAG_UNK10000000)) {
					onMapObjFlag(MAP_OBJ_FLAG_DISAPPEARING);
					mStateTimer = unk14C;
				}
			} else {
				if (!checkMapObjFlag(MAP_OBJ_FLAG_UNK10000000)) {
					if (mContainer != nullptr)
						mContainer->receiveMessage(this, HIT_MESSAGE_UNK5);
					makeObjDead();
				}
			}
		}

		if (getColNum())
			for (int i = 0; i < getColNum(); ++i)
				touchActor(mCollisions[i]);

	} else {
		if ((cue & CUE_CALC_VIEW) && getMActor() == nullptr) {
			gpQuestionManager->request(mPosition, 60.0f);
		}

		TItem::perform(cue, graphics);
	}
}

void TCoin::loadAfter()
{
	TItem::loadAfter();
	if (!gpMirrorModelManager->isInMirror(mPosition))
		return;

	if (gpMarDirector->getCurrentMap() == 2) {
		const TBGCheckData* check;
		gpMap->checkGround(mPosition, &check);
		if (!check->isWaterSurface())
			return;
	}

	unk154 = new TMirrorActor("コインin鏡");
	unk154->init(getModel(), 0x18);
}

void TCoin::initMapObj()
{
	TItem::initMapObj();
	SMS_LoadParticle("/scene/mapObj/ms_watcoin_kira.jpa", 0x58);
}

TCoin::TCoin(const char* name)
    : TItem(name)
    , unk154(0)
{
}

void TFlowerCoin::load(JSUMemoryInputStream& stream)
{
	TCoin::load(stream);
	stream >> unk158;
}

void TCoinEmpty::warning() { }

void TCoinEmpty::appear() { }

void TCoinEmpty::makeObjAppeared() { }

void TCoinEmpty::kill() { }

TCoinEmpty::TCoinEmpty(const char* name)
    : TCoin(name)
{
}

void TCoinRed::taken(THitActor* param_1)
{
	TFlagManager::getInstance()->incFlag(0x60000, 1);

	if (gpMSound->gateCheck(MSD_SE_SY_RED_COIN_GET))
		MSoundSESystem::MSoundSE::startSoundActor(MSD_SE_SY_RED_COIN_GET,
		                                          mPosition, 0, nullptr, 0, 4);

	if (mContainer)
		mContainer->receiveMessage(this, HIT_MESSAGE_UNK8);

	TItem::taken(param_1);
}

TCoinRed::TCoinRed(const char* name)
    : TCoin(name)
{
}

void TCoinBlue::makeObjAppeared()
{
	if (TFlagManager::getInstance()->getBlueCoinFlag(
	        gpMarDirector->getCurrentMap(), getEventId()))
		return;

	TCoin::makeObjAppeared();
}

void TCoinBlue::taken(THitActor* param_1)
{
	SMSGetMarDirector()->fireGetBlueCoin(this);

	if (mContainer)
		mContainer->receiveMessage(this, HIT_MESSAGE_UNK8);

	TItem::taken(param_1);
}

void TCoinBlue::loadBeforeInit(JSUMemoryInputStream& stream)
{
	s32 eventId;
	stream >> eventId;
	if (eventId == -1)
		eventId = 0;
	setEventId(eventId);
}

void TCoinBlue::load(JSUMemoryInputStream& stream)
{
	TCoin::load(stream);
	if (TFlagManager::getInstance()->getBlueCoinFlag(
	        gpMarDirector->getCurrentMap(), getEventId()))
		makeObjDead();
}

TCoinBlue::TCoinBlue(const char* name)
    : TCoin(name)
{
}

int TShine::mPromiLife[4]  = { 30, 15, 0, 0 };
f32 TShine::mSenkoRate[4]  = { 0.15f, 0.1f, 0.05f, 0.025f };
f32 TShine::mKiraRate[4]   = { 1.0f, 0.6f, 0.3f, 0.1f };
f32 TShine::mBowRate[4]    = { 1.0f, 1.0f, 0.0f, 0.0f };
f32 TShine::mCircleRateY   = 0.5f;
f32 TShine::mUpSpeed       = 1.0f;
f32 TShine::mSpeedDownRate = 0.99f;

void TShine::calc()
{
	MtxPtr mtxPos = getMActor()->getModel()->getAnmMtx(2);

	if (checkLiveFlag(LIVE_FLAG_UNK200 | LIVE_FLAG_CLIPPED_OUT
	                  | LIVE_FLAG_DEAD))
		return;

	unk198 = gpMarioParticleManager->emitAndBindToMtxPtr(
	    PARTICLE_MS_SHINE_SENKO, mtxPos, 1, this);
	unk19C = gpMarioParticleManager->emitAndBindToMtxPtr(PARTICLE_MS_SHINE_KIRA,
	                                                     mtxPos, 1, this);
	if (unk1B4 == 0) {
		unk194 = gpMarioParticleManager->emitAndBindToMtxPtr(
		    PARTICLE_MS_SHINE_PROMI, mtxPos, 1, this);
		unk1A0 = gpMarioParticleManager->emitAndBindToMtxPtr(
		    PARTICLE_MS_SHINE_BOW, mtxPos, 1, this);
	}

	f32 dist2 = gpCamera->unk124.squared(mPosition);
	f32 dist  = JGeometry::TUtil<f32>::sqrt(dist2);

	s16 promiLife;
	f32 senkoRate, kiraRate, bowRate;
	if (dist < 2000.0f) {
		promiLife = mPromiLife[0];
		kiraRate  = mKiraRate[0];
		bowRate   = mBowRate[0];
		senkoRate = mSenkoRate[0];
	} else if (dist < 4000.0f) {
		promiLife = mPromiLife[1];
		kiraRate  = mKiraRate[1];
		bowRate   = mBowRate[1];
		senkoRate = mSenkoRate[1];
	} else if (dist < 6000.0f) {
		promiLife = mPromiLife[2];
		kiraRate  = mKiraRate[2];
		bowRate   = mBowRate[2];
		senkoRate = mSenkoRate[2];
	} else {
		promiLife = mPromiLife[3];
		kiraRate  = mKiraRate[3];
		bowRate   = mBowRate[3];
		senkoRate = mSenkoRate[3];
	}

	if (unk194) {
		unk194->mBaseLifetime = promiLife;
		unk194->setScale(unk1A8);
	}
	if (unk198) {
		unk198->mChildSpawnRate = senkoRate;
		unk198->setScale(unk1A8);
	}
	if (unk19C) {
		unk19C->mChildSpawnRate = kiraRate;
		unk19C->setScale(unk1A8);
	}
	if (unk1A0) {
		unk1A0->mChildSpawnRate = bowRate;
		unk1A0->setScale(unk1A8);
	}
	unk1A4 = 1;
}

void TShine::movingCircle()
{
	// TODO: hack, remove
	(void)0;
	(void)0;

	f32 prevY = mPosition.y;
	unk158 += 180.0f / (f32)unk168;

	f32 tmp = (f32)(unk168 - mStateTimer) / (f32)unk168;

	mPosition.x += unk17C.x;

	mPosition.y = unk160 * JMASin(unk158)
	              + (tmp * (mInitialPosition.y - unk164) + unk164);
	unk188 = mPosition.y - prevY;

	mPosition.z += unk17C.z;
	mRotation.y += 7.0f;
	MsWrap(mRotation.y, 0.0f, 360.0f);

	if (!isStateTimerEngaged()) {
		unk16C      = 7.0f;
		mStateTimer = unk178;
		mState      = STATE_UNKF;
	}
}

void TShine::movingUp()
{
	mPosition.y += mUpSpeed;
	if (isStateTimerEngaged())
		return;

	if (unk154 == 3) {
		mStateTimer = unk170;
		mState      = STATE_MOVING_DOWN;
	} else {
		unk164      = mPosition.y;
		mStateTimer = unk168;
		mState      = STATE_MOVING_CIRCLE;
	}
}

void TShine::movingDown()
{
	mPosition.y -= mUpSpeed;
	if (isStateTimerEngaged())
		return;
	unk16C      = 7.0f;
	mStateTimer = unk178;
	mState      = STATE_UNKF;
}

void TShine::control()
{
	if (!isState(0x10))
		TMapObjGeneral::control();

	if (isState(0x10)) {
		mPosition.set(SMS_GetMarioPos());
		return;
	}

	switch (mState) {
	case STATE_NORMAL: {
		mRotation.y += unk16C;
		if (gpMSound->gateCheck(MSD_SE_SHINE_EXIST))
			MSoundSESystem::MSoundSE::startSoundActor(
			    MSD_SE_SHINE_EXIST, &mPosition, 0, nullptr, 0, 4);

		J3DModel* model      = getMActor()->getModel();
		MtxPtr mtx           = model->getAnmMtx(2);
		const GXColor& color = (GXColor) { 0xff, 0xff, 0xff, 0xff };
		JGeometry::TVec3<f32> trans(mtx[0][3], mtx[1][3], mtx[2][3]);
		TLightWithDBSetManager* mgr = gpLightManager;

		mgr->unk18.r = color.r;
		mgr->unk18.g = color.g;
		mgr->unk18.b = color.b;
		mgr->unk18.a = color.a;

		mgr->unk54 = 1;

		mgr = gpLightManager;
		mgr->unk1C.set(trans);
		mgr->unk54 = 1;
	} break;

	case STATE_UNKB:
		if (isStateTimerEngaged())
			break;
		unkF8 &= 0xF7FFFEFF;
		mStateTimer = unk170;
		mState      = STATE_MOVING_UP;
		break;

	case STATE_MOVING_UP:
		if (gpMSound->gateCheck(MSD_SE_SHINE_EXIST))
			MSoundSESystem::MSoundSE::startSoundActor(
			    MSD_SE_SHINE_EXIST, &mPosition, 0, nullptr, 0, 4);
		movingUp();
		break;

	case STATE_MOVING_DOWN:
		if (gpMSound->gateCheck(MSD_SE_SHINE_EXIST))
			MSoundSESystem::MSoundSE::startSoundActor(
			    MSD_SE_SHINE_EXIST, &mPosition, 0, nullptr, 0, 4);
		movingDown();
		break;

	case STATE_MOVING_CIRCLE:
		if (gpMSound->gateCheck(MSD_SE_SHINE_EXIST))
			MSoundSESystem::MSoundSE::startSoundActor(
			    MSD_SE_SHINE_EXIST, &mPosition, 0, nullptr, 0, 4);
		movingCircle();
		break;

	case STATE_UNKF: {
		if (gpMSound->gateCheck(MSD_SE_SHINE_EXIST))
			MSoundSESystem::MSoundSE::startSoundActor(
			    MSD_SE_SHINE_EXIST, &mPosition, 0, nullptr, 0, 4);
		if (mPosition.y > mInitialPosition.y) {
			mPosition.y += unk188;
			unk188 *= mSpeedDownRate;
		} else {
			mPosition.y = mInitialPosition.y;
		}
		if (unk16C > 2.0f)
			unk16C -= 0.1f;
		else
			unk16C = 2.0f;
		mRotation.y += unk16C;
		// Huh? Result discarded?
		MsWrap(mRotation.y, 0.0f, 360.0f);

		if (isStateTimerEngaged())
			break;
		if (unkF8 & 0x20000000)
			MSBgm::setTrackVolume(0, 1.0f, 10, 0);
		offHitFlag(HIT_FLAG_NO_COLLISION);
		mState = STATE_UNK11;
	} break;

	case STATE_UNK11: {
		mRotation.y += unk16C;
		// Huh? Result discarded?
		MsWrap(mRotation.y, 0.0f, 360.0f);
		if (SMSGetMSound()->gateCheck(MSD_SE_SHINE_EXIST))
			MSoundSESystem::MSoundSE::startSoundActor(
			    MSD_SE_SHINE_EXIST, &mPosition, 0, nullptr, 0, 4);
	} break;

	case STATE_UNK12: {
		if (gpCamera->isDemoCamera())
			break;
		if (isStateTimerEngaged())
			break;
		appearWithDemo("シャイン（いきなり出現）カメラ");
		mState = STATE_UNK11;
	} break;
	}
}

void TShine::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if ((cue & CUE_CALC_ANIM) && !checkLiveFlag(LIVE_FLAG_DEAD)) {
		if (!isState(STATE_NORMAL))
			offLiveFlag(LIVE_FLAG_UNK200);
	}
	TMapObjGeneral::perform(cue, graphics);
}

BOOL TShine::receiveMessage(THitActor* sender, u32 message)
{
	unkF8 &= 0xF7FFFFFF;
	mPosition.set(SMS_GetMarioPos());
	mRotation.y = 180.0f * (f32)*gpMarioAngleY / 32768.0f;

	MsMtxSetXYZRPH(getModel()->getBaseTRMtx(), mPosition.x,
	               mPosition.y - mYOffset, mPosition.z, mRotation.x,
	               mRotation.y, mRotation.z);

	if (SMS_IsMarioOnYoshi()) {
		if (unk1B4)
			getMActor()->setBck("shine_empty_demo_shine_get_yo");
		else
			getMActor()->setBck("shine_demo_shine_get_yo");
	} else {
		if (unk1B4)
			getMActor()->setBck("shine_empty_demo_shine_get");
		else
			getMActor()->setBck("shine_demo_shine_get");
	}

	unk1A8.set(0.5f, 0.5f, 0.5f);
	mState = STATE_UNK10;
	return TRUE;
}

void TShine::touchPlayer(THitActor* actor)
{
	actor->receiveMessage(this, HIT_MESSAGE_ATTACK);
	TLightWithDBSetManager* mgr = gpLightManager;
	mgr->unk1C.set(200000.0f, 500000.0f, 200000.0f);
	if (gpMSound->gateCheck(MSD_SE_SY_GET_SHINE))
		MSoundSESystem::MSoundSE::startSoundActor(MSD_SE_SY_GET_SHINE,
		                                          mPosition, 0, nullptr, 0, 4);
	getMActor()->setBck("shine_float");
	onHitFlag(HIT_FLAG_NO_COLLISION);
}

void TShine::appearWithTime(int param_1, int param_2, int param_3, int param_4)
{
	TMapObjGeneral::appear();
	onHitFlag(HIT_FLAG_NO_COLLISION);
	mStateTimer = unk150;
	offMapObjFlag(MAP_OBJ_FLAG_DISAPPEARING);
	TFlagManager::smInstance->setBool(true, 0x50000);

	if (param_2 >= 0)
		unk174 = param_2;
	if (param_3 >= 0)
		unk170 = param_3;
	if (param_4 >= 0)
		unk178 = param_4;

	unk168 = param_1 - (unk174 + unk170 + unk178);
	unk158 = 0.0f;

	f32 yDelta = mInitialPosition.y - (mUpSpeed * (f32)unk170 + mPosition.y);

	unk17C.x = (mInitialPosition.x - mPosition.x) / (f32)unk168;
	unk17C.y = yDelta / (f32)unk168;
	unk17C.z = (mInitialPosition.z - mPosition.z) / (f32)unk168;

	unk15C = getDistanceXZ(mInitialPosition);
	if (unk15C == 0.0f)
		unk15C = 1000.0f;
	if (yDelta > 0.0f)
		unk15C += fabsf(yDelta);
	unk160 = unk15C * mCircleRateY;

	if (SMSGetMSound()->gateCheck(MSD_SE_SHINE_APPEAR))
		MSoundSESystem::MSoundSE::startSoundActor(MSD_SE_SHINE_APPEAR,
		                                          mPosition, 0, nullptr, 0, 4);

	mStateTimer = unk174;
	mState      = STATE_UNKB;
	onHitFlag(HIT_FLAG_NO_COLLISION);
}

s32 TShine::appearWithTimeCallback(u32 param_1, u32 param_2)
{
	TShine* shine = (TShine*)param_1;
	if (param_2 == 0) {
		shine->appearWithTime(shine->unk18C, -1, -1, -1);
		gpMarDirector->unk4E |= 1;
	} else if (param_2 == 1) {
		gpMarDirector->unk4E &= ~1;
	}
	return 0;
}

void TShine::appearSimple(int param_1)
{
	TMapObjGeneral::appear();
	onHitFlag(HIT_FLAG_NO_COLLISION);
	mStateTimer = unk150;
	offMapObjFlag(MAP_OBJ_FLAG_DISAPPEARING);
	TFlagManager::smInstance->setBool(true, 0x50000);

	unk174   = 60;
	unk170   = param_1;
	unk178   = 60;
	unk154   = 3;
	unk158   = 0.0f;
	unk15C   = 0.0f;
	unk160   = 0.0f;
	mUpSpeed = 2.0f;

	mInitialPosition = mPosition;

	if (SMSGetMSound()->gateCheck(MSD_SE_SHINE_APPEAR))
		MSoundSESystem::MSoundSE::startSoundActor(MSD_SE_SHINE_APPEAR,
		                                          mPosition, 0, nullptr, 0, 4);

	mStateTimer = unk174;
	mState      = STATE_UNKB;
	onHitFlag(HIT_FLAG_NO_COLLISION);
}

void TShine::appearWithDemo(const char* param_1)
{
	unk18C = JDrama::TNameRefGen::instance->search<TCameraMapTool>(param_1)
	             ->mDemoLengthFrames;
	SMSGetMarDirector()->fireStartDemoCamera(
	    param_1, &mPosition, -1, 0.0f, true, appearWithTimeCallback, (u32)this,
	    nullptr, JDrama::TFlagT<u16>());
}

void TShine::kill()
{
	TMapObjGeneral::kill();
	unk154 = 1;
}

void TShine::makeMActors()
{
	mMActorKeeper                    = new TMActorKeeper(mManager, 1);
	mMActorKeeper->mModelLoaderFlags = J3DMLF_MaterialPEFull
	                                   | J3DMLF_UseUniqueMaterials
	                                   | (2 << J3DMLF_TevStageNumShift);
	MActor* result;
	if (TFlagManager::smInstance->getShineFlag(mEventId)
	    && strcmp("シャイン（１００枚コイン用）", getName()) != 0) {
		result = initMActor("shine_empty.bmd", nullptr, getSDLModelFlag());
		unk1B4 = 1;
	} else {
		result = initMActor("shine.bmd", nullptr, getSDLModelFlag());
	}
	mMActor = result;
}

void TShine::initMapObj()
{
	TItem::initMapObj();
	unk1A4 = 0;
	unk1A8.set(1.0f, 1.0f, 1.0f);
	unk170 = 240;
	unk174 = 0;
	unk178 = 240;
}

void TShine::loadAfter()
{
	TMapObjGeneral::loadAfter();
	if (unk154 == 2) {
		mStateTimer = 240;
		mState      = STATE_UNK12;
	} else if (unk154 == 1) {
		makeObjDead();
	}
}

void TShine::loadBeforeInit(JSUMemoryInputStream& stream)
{
	char name[0x20];
	stream.readString(name, sizeof(name));
	if (strcmp("normal", name) == 0)
		unk154 = 0;
	else if (strcmp("quickly", name) == 0)
		unk154 = 2;
	else
		unk154 = 1;

	s32 eventId;
	stream >> eventId;
	if (eventId == -1)
		eventId = 120;
	setEventId(eventId);

	s32 v;
	stream >> v;
	eventId = v;
	if (v + 1 >= 2)
		eventId = -1;
	unk190 = eventId + 1;
}

TShine::TShine(const char* name)
    : TItem(name)
    , unk154(0)
    , unk158(0.0f)
    , unk15C(0.0f)
    , unk160(0.0f)
    , unk164(0.0f)
    , unk168(0)
    , unk16C(2.0f)
    , unk188(0.0f)
    , unk18C(0)
    , unk190(0)
    , unk194(0)
    , unk198(0)
    , unk19C(0)
    , unk1A0(0)
    , unk1B4(0)
{
	unk17C.zero();
	unk1A8.zero();
}

void TEggYoshi::decideRandomLoveFruit()
{
	u8 map = gpMarDirector->mMap;

	if (map == 7 && gpMarDirector->unk7D == 1) {
		unk14C = 0x40000392;
		return;
	}

	if (map == 3) {
		unk14C = 0x40000393;
		return;
	}

	if (map == 1 && strcmp(getName(), "ヨッシーの卵（影マリオ用）") == 0) {
		unk14C = 0x40000394;
		return;
	}

	int r = 4 * MsRandF();
	switch (r) {
	case 0:
		unk14C = 0x40000394;
		break;
	case 1:
		unk14C = 0x40000391;
		break;
	case 2:
		unk14C = 0x40000392;
		break;
	default:
		unk14C = 0x40000390;
		break;
	}
}

void TEggYoshi::startBalloonAnim()
{
	switch (unk14C) {
	case 0x40000394:
		unk148->getFrameCtrl(3)->setFrame(1.0f);
		break;
	case 0x40000393:
		unk148->getFrameCtrl(3)->setFrame(3.0f);
		break;
	case 0x40000391:
		unk148->getFrameCtrl(3)->setFrame(5.0f);
		break;
	case 0x40000392:
		unk148->getFrameCtrl(3)->setFrame(7.0f);
		break;
	case 0x40000390:
		unk148->getFrameCtrl(3)->setFrame(9.0f);
		break;
	}
}

void TEggYoshi::touchFruit(THitActor* fruit)
{
	if (isState(0xE) || isState(STATE_HOLDING))
		return;

	if (unk14C == (u32)fruit->mActorType) {
		startAnim(1);
		unk148->getFrameCtrl(3)->setFrame(11.0f);
		mRotation.y = (360.0f / 65536.0f)
		              * matan(fruit->mPosition.z - mPosition.z,
		                      fruit->mPosition.x - mPosition.x);
		mState = 0xB;
		unk150 = fruit;
		if (gpMSound->gateCheck(MSD_SE_SY_COLLECT_PRETTY))
			MSoundSESystem::MSoundSE::startSoundSystemSE(
			    MSD_SE_SY_COLLECT_PRETTY, 0, nullptr, 0);
	} else if (animIsFinished()) {
		startAnim(2);
		unk148->getFrameCtrl(3)->setFrame(12.0f);
		if (gpMSound->gateCheck(MSD_SE_SY_NOT_COLLECT_YOSHI))
			MSoundSESystem::MSoundSE::startSoundSystemSE(
			    MSD_SE_SY_NOT_COLLECT_YOSHI, 0, nullptr, 0);
		mState = 0xD;
	}
}

void TEggYoshi::touchActor(THitActor* other)
{
	if (!isState(STATE_NORMAL) && !isState(0xD))
		return;

	if (other->isActorType(0x80000001)) {
		TTakeActor* casted = static_cast<TTakeActor*>(other);
		if (casted->getHeldObject()
		    && TMapObjBase::isFruit(casted->getHeldObject()))
			touchFruit(casted->getHeldObject());
	}

	if (TMapObjBase::isFruit(other))
		touchFruit(other);
}

void TEggYoshi::control()
{
	TMapObjBase::control();

	switch (mState) {
	case 0xD:
		if (animIsFinished()) {
			startAnim(0);
			startBalloonAnim();
			mState = STATE_NORMAL;
		}
		break;
	case 0xB:
		if (animIsFinished()) {
			startAnim(3);
			TYoshi* yoshi = SMS_GetYoshi();
			if (!yoshi->isHatched()) {
				JGeometry::TVec3<f32> pos = mPosition;
				yoshi->appearFromEgg(pos, mRotation.y, this);
				yoshi->setEggYoshiPtr(this);
			}
			mState = 0xC;
		}
		break;
	case 0xC:
		if (animIsFinished()) {
			kill();
			mState = STATE_DEAD;
		}
		break;
	case 0xF: {
		JGeometry::TVec3<f32> v = mVelocity;
		if (v.y == 0.0f)
			mState = 0x10;
		break;
	}
	case 0x0:
	case 0x1:
	case 0x2:
	case 0x3:
	case 0x4:
	case 0x5:
	case 0x6:
	case 0x7:
	case 0x8:
	case 0x9:
	case 0xA:
	case 0xE:
	case 0x10:
		break;
	}
}

void TEggYoshi::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TMapObjGeneral::perform(cue, graphics);

	if (!isState(0xC) && !isState(STATE_DEAD) && !isState(STATE_HOLDING)
	    && !isState(STATE_APPEARING) && !isState(0xE) && !isState(0xF)
	    && !isState(0x10)) {
		if (cue & CUE_CALC_ANIM)
			unk148->getModel()->setBaseTRMtx(getModel()->getAnmMtx(0));

		unk148->perform(cue, graphics);
	}
}

void TEggYoshi::startFruit()
{
	receiveMessage(nullptr, HIT_MESSAGE_UNK10);
	if (isState(0) || isState(0xE) || isState(0xF) || isState(0x10))
		receiveMessage(nullptr, HIT_MESSAGE_UNK10);
}

BOOL TEggYoshi::receiveMessage(THitActor* sender, u32 message)
{
	if (message == HIT_MESSAGE_TAKE) {
		hold((TTakeActor*)sender);
		return TRUE;
	}

	if (message == HIT_MESSAGE_THROWN || message == HIT_MESSAGE_UNK8) {
		mVelocity.y = 10.0f;
		offLiveFlag(LIVE_FLAG_UNK10);
		mState = 0xF;
		return TRUE;
	}

	if (message == HIT_MESSAGE_UNK10) {
		JGeometry::TVec3<f32> v = mVelocity;
		makeObjAppeared();
		mVelocity.y = v.y;
		offLiveFlag(LIVE_FLAG_UNK10);
		decideRandomLoveFruit();
		startBalloonAnim();
		mState = STATE_NORMAL;
		return TRUE;
	}

	if (message == HIT_MESSAGE_PUT) {
		makeObjAppeared();
		decideRandomLoveFruit();
		startBalloonAnim();
	}

	return FALSE;
}

void TEggYoshi::load(JSUMemoryInputStream& stream)
{
	TMapObjBase::load(stream);

	if (strcmp(unkF4, "eggYoshiEvent") == 0) {
		if (TFlagManager::getInstance()->getFlag(0x60003) == 1) {
			mState = 0xE;
		} else {
			makeObjDead();
			return;
		}
	} else if (gpMarDirector->mMap == 1) {
		if (!TFlagManager::getInstance()->getBool(0x1038F)) {
			makeObjDead();
			return;
		}
	} else if (!TFlagManager::getInstance()->getShineFlag(0x21)) {
		makeObjDead();
		return;
	}

	unk148 = SMS_MakeMActorWithAnmData(
	    "/scene/mapObj/eggYoshi_fukidashi.bmd", mManager->getMActorAnmData(), 3,
	    J3DMLF_MaterialPEFull | J3DMLF_UseUniqueMaterials
	        | (1 << J3DMLF_TevStageNumShift));
	MtxPtr src = getModel()->getAnmMtx(0);
	PSMTXCopy(src, unk148->getModel()->getBaseTRMtx());
	unk148->setBck("eggyoshi_fukidashi_wait");
	unk148->setBtp("eggyoshi_fukidashi");
	unk148->getFrameCtrl(3)->setRate(0.0f);

	decideRandomLoveFruit();

	if (!isState(0xE))
		startBalloonAnim();
}

TEggYoshi::TEggYoshi(const char* name)
    : TMapObjGeneral(name)
    , unk148(nullptr)
    , unk14C(0)
    , unk150(nullptr)
{
}

void TItemNozzle::touchPlayer(THitActor* param_1)
{
	if (isState(STATE_HOLDING))
		return;

	if (SMS_IsMarioOnYoshi())
		return;

	if ((param_1->isActorType(0x80000001) || param_1->isActorType(0x8000083))
	    && !checkHitFlag(HIT_FLAG_NO_COLLISION))
		taken(param_1);

	int boxKind;
	if (isActorType(0x2000001F))
		boxKind = 4;
	else if (isActorType(0x20000022))
		boxKind = 1;
	else if (isActorType(0x2000002A))
		boxKind = 5;
	else
		boxKind = 4;

	if (SMSGetMSound()->gateCheck(0x484E))
		MSoundSESystem::MSoundSE::startSoundActor(0x484E, mPosition, 0, nullptr,
		                                          0, 4);
	gpItemManager->resetNozzleBoxesModel(boxKind);
	gpMarDirector->fireGetNozzle(this);
}

void TItemNozzle::put()
{
	offHitFlag(HIT_FLAG_NO_COLLISION);
	mState = STATE_NORMAL;
}

BOOL TItemNozzle::receiveMessage(THitActor* sender, u32 message)
{
	if (message == HIT_MESSAGE_TAKE) {
		hold((TTakeActor*)sender);
		return TRUE;
	}

	if (message == HIT_MESSAGE_THROWN) {
		mVelocity.set(0.0f, 20.0f, 0.0f);
		offLiveFlag(LIVE_FLAG_UNK10);
		onHitFlag(HIT_FLAG_NO_COLLISION);
		mState = 0xB;
		return TRUE;
	}

	return TItem::receiveMessage(sender, message);
}

void TItemNozzle::appearing()
{
	if (!checkLiveFlag(LIVE_FLAG_UNK10))
		return;

	mState = STATE_NORMAL;
	offHitFlag(HIT_FLAG_NO_COLLISION);
}

void TItemNozzle::control() { TMapObjGeneral::control(); }

void TItemNozzle::initMapObj()
{
	TItem::initMapObj();
	unk14C = 7200;
}

void TItemNozzle::load(JSUMemoryInputStream& stream)
{
	TMapObjBase::load(stream);
	onMapObjFlag(MAP_OBJ_FLAG_UNK10000000);
	if (strcmp(unkF4, "rocket_nozzle_item") == 0) {
		if (TFlagManager::smInstance->getFlag(0x60003) != 3)
			makeObjDead();
	} else if (strcmp(unkF4, "back_nozzle_item") == 0) {
		if (TFlagManager::smInstance->getFlag(0x60003) != 2)
			makeObjDead();
	}
}

void TNozzleBox::makeModelValid()
{
	if (!mContainedNozzleItem->checkLiveFlag(LIVE_FLAG_DEAD)) {
		mContainedNozzleItem->kill();
		appear();
	}
	makeObjAppeared();
	offHitFlag(HIT_FLAG_CANNOT_GET_HIT);
	SMS_ShowAllShapePacket(getModel());
	unk15C = true;
}

void TNozzleBox::makeModelInvalid()
{
	if (!mContainedNozzleItem->checkLiveFlag(LIVE_FLAG_DEAD)) {
		mContainedNozzleItem->kill();
		appear();
	}
	onHitFlag(HIT_FLAG_CANNOT_GET_HIT);
	startAnim(3);
	unk15C = false;
}

void TNozzleBox::breaking()
{
	if (animIsFinished())
		makeObjDead();
}

BOOL TNozzleBox::receiveMessage(THitActor* sender, u32 message)
{
	if (unk15C && sender->isActorType(0x80000001)
	    && message == HIT_MESSAGE_TRAMPLE && !SMS_IsMarioHeadSlideAttack()) {
		sender->receiveMessage(this, HIT_MESSAGE_ATTACK);
		throwObjToFront(mContainedNozzleItem, 50.0f, unk150, unk154);
		SMSGetMSound()->startSoundSystemSE(0x3801, 0, nullptr, 0);
		kill();
		return TRUE;
	}

	if (message == HIT_MESSAGE_UNK5)
		makeModelValid();

	return FALSE;
}

void TNozzleBox::touchPlayer(THitActor*)
{
	if (mContainedNozzleType == TWaterGun::Hover
	    && !TFlagManager::smInstance->getNozzleRight(
	        gpMarDirector->getCurrentMap(), 0)
	    && !TFlagManager::smInstance->getNozzleRight(
	        gpMarDirector->getCurrentMap(), 1)
	    && !unk166) {
		gpMarDirector->getConsole()->startAppearBalloon(0xE0057, true);
		unk166 = true;
	}
	if (!unk15C && !unk166) {
		gpMarDirector->getConsole()->startAppearBalloon(0xE0056, true);
		unk166 = true;
	}
}

void TNozzleBox::control()
{
	TMapObjGeneral::control();
	if (mContainedNozzleType != TWaterGun::Hover && unk166 && mColCount == 0)
		unk166 = false;
}

void TNozzleBox::loadAfter()
{
	TMapObjGeneral::loadAfter();
	mContainedNozzleItem = (TItemNozzle*)TMapObjBaseManager::newAndRegisterObj(
	    mContainedNozzleName);
	mContainedNozzleItem->setContainer(this);
	switch (mContainedNozzleType) {
	case TWaterGun::Hover:
		makeModelValid();
		break;
	case TWaterGun::Rocket:
	case TWaterGun::Turbo:
		if (unk15C) {
			makeModelValid();
		} else {
			makeModelInvalid();
		}
		break;
	}
}

void TNozzleBox::load(JSUMemoryInputStream& stream)
{
	TMapObjBase::load(stream);
	char strBuf[0x20];
	mContainedNozzleName = stream.readString();
	stream.readString(strBuf, 0x20);
	if (strcmp(strBuf, "valid") == 0)
		unk15C = true;
	else
		unk15C = false;

	if (strcmp(mContainedNozzleName, "normal_nozzle_item") == 0) {
		mContainedNozzleType = TWaterGun::Hover;
		unk15E.r             = 0;
		unk15E.g             = 0;
		unk15E.b             = 0xFF;
	} else if (strcmp(mContainedNozzleName, "rocket_nozzle_item") == 0) {
		mContainedNozzleType = TWaterGun::Rocket;
		unk15E.r             = 0xFF;
		unk15E.g             = 0;
		unk15E.b             = 0;
		if (TFlagManager::smInstance->getNozzleRight(gpMarDirector->mMap, 0)) {
			unk15C = true;
			unk166 = true;
		}
	} else if (strcmp(mContainedNozzleName, "back_nozzle_item") == 0) {
		mContainedNozzleType = TWaterGun::Turbo;
		unk15E.r             = 0x5A;
		unk15E.g             = 0x5A;
		unk15E.b             = 0x78;
		if (TFlagManager::smInstance->getNozzleRight(gpMarDirector->mMap, 1)) {
			unk15C = true;
			unk166 = true;
		}
	}

	stream >> unk150;
	unk150 *= 0.02f;
	stream >> unk154;
	if (unk154 < 0.0f)
		unk154 = 20.0f;

	initPacketMatColor(getModel(), GX_TEVREG1, &unk15E);
	startAnim(3);
	initPacketMatColor(getModel(), GX_TEVREG1, &unk15E);
	startAnim(2);
	initPacketMatColor(getModel(), GX_TEVREG1, &unk15E);
	startAnim(0);
}

TNozzleBox::TNozzleBox(const char* name)
    : TMapObjGeneral(name)
    , mContainedNozzleType(0)
    , mContainedNozzleItem(nullptr)
    , unk150(0.0f)
    , unk154(0.0f)
    , mContainedNozzleName(nullptr)
    , unk15C(true)
    , unk166(false)
{
	unk15E.r = 0xFF;
	unk15E.g = 0xFF;
	unk15E.b = 0xFF;
	unk15E.a = 0x64;
}
