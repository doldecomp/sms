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
#include <Player/Yoshi.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JParticle/JPAResourceManager.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

f32 TItem::mAppearedScaleSpeed = 0.01f;

void TItem::appeared()
{
	if (checkMapObjFlag(0x40000) && !isWaitingToAppear()) {
		if (unk148)
			unk148->receiveMessage(this, HIT_MESSAGE_UNK5);

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
	if (checkMapObjFlag(0x80000)) {
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
	if (!checkMapObjFlag(0x8000000))
		TMapObjGeneral::calcRootMatrix();
}

void TItem::calc()
{
	if (!checkMapObjFlag(0x4000000) && !isState(6)) {
		MtxPtr src = gpItemManager->unk40;

		MtxPtr mtx;
		if (checkMapObjFlag(0x100))
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

	if (isState(6) && checkMapObjFlag(0x100))
		TMapObjGeneral::calcRootMatrix();
}

void TItem::appearing()
{
	if (unkF8 & 0x2000000) {
		if (mScaling.x < 2.0f) {
			mScaling.add((Vec) { mAppearedScaleSpeed * 2.0f,
			                     mAppearedScaleSpeed * 2.0f,
			                     mAppearedScaleSpeed * 2.0f });
		} else {
			makeObjAppeared();
			unk64 |= 1;
			unkF8 &= ~0x40000;
		}
	} else {
		TMapObjGeneral::appearing();
	}
}

void TItem::killByTimer(int param_1)
{
	unk14C         = param_1;
	mTimeTilAppear = unk150;

	offMapObjFlag(0x10000000);
	onHitFlag(HIT_FLAG_NO_COLLISION);
	offMapObjFlag(0x40000);
}

void TItem::appear()
{
	TMapObjGeneral::appear();
	onHitFlag(HIT_FLAG_NO_COLLISION);
	mTimeTilAppear = unk150;
	offMapObjFlag(0x40000);
}

void TItem::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (checkLiveFlag(LIVE_FLAG_DEAD))
		return;

	if ((param_1 & 1) && checkHitFlag(HIT_FLAG_NO_COLLISION)
	    && !isWaitingToAppear()) {
		offHitFlag(HIT_FLAG_NO_COLLISION);
		if (!checkMapObjFlag(0x10000000)) {
			onMapObjFlag(0x40000);
			mTimeTilAppear = unk14C;
		}
	}

	TMapObjGeneral::perform(param_1, param_2);
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
	onMapObjFlag(0x10000000);
}

TItem::TItem(const char* name)
    : TMapObjGeneral(name)
    , unk148(0)
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

	if (unk148)
		unk148->receiveMessage(this, HIT_MESSAGE_UNK8);

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
	gpMarioParticleManager->emitAndBindToMtxPtr(0x58, getModel()->getAnmMtx(0),
	                                            0, this);
	if (isActorType(0x2000000e))
		offMapObjFlag(0x10000000);
}

void TCoin::appear()
{
	if (isActorType(0x20000010)) {
		if (!TFlagManager::smInstance->getBlueCoinFlag(
		        gpMarDirector->getCurrentMap(), unk134))
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

void TCoin::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (checkLiveFlag(LIVE_FLAG_DEAD))
		return;

	if ((param_1 & 1) && checkLiveFlag(LIVE_FLAG_UNK10)) {
		// TODO: this is some kind of a tricky inline, used in a few places
		bool bVar2 = true;
		if (gpMarDirector->unk124 != 1 && gpMarDirector->unk124 != 2)
			bVar2 = false;

		if (bVar2) {
			bVar2 = true;
			if (gpMarDirector->unk124 != 3 && gpMarDirector->unk124 != 4)
				bVar2 = false;
			if (!bVar2)
				return;
		}

		if (isWaitingToAppear()) {
			--mTimeTilAppear;
		} else {
			if (checkHitFlag(HIT_FLAG_NO_COLLISION)) {
				offHitFlag(HIT_FLAG_NO_COLLISION);
				if (!checkMapObjFlag(0x10000000)) {
					onMapObjFlag(0x40000);
					mTimeTilAppear = unk14C;
				}
			} else {
				if (!checkMapObjFlag(0x10000000)) {
					if (unk148 != 0)
						unk148->receiveMessage(this, HIT_MESSAGE_UNK5);
					makeObjDead();
				}
			}
		}

		if (getColNum())
			for (int i = 0; i < getColNum(); ++i)
				touchActor(mCollisions[i]);

	} else {
		if ((param_1 & 4) && getMActor() == nullptr) {
			gpQuestionManager->request(mPosition, 60.0f);
		}

		TItem::perform(param_1, param_2);
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
	stream.read(&unk158, 4);
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

	if (unk148)
		unk148->receiveMessage(this, HIT_MESSAGE_UNK8);

	TItem::taken(param_1);
}

TCoinRed::TCoinRed(const char* name)
    : TCoin(name)
{
}

void TCoinBlue::makeObjAppeared()
{
	if (TFlagManager::getInstance()->getBlueCoinFlag(
	        gpMarDirector->getCurrentMap(), getUnk134()))
		return;

	TCoin::makeObjAppeared();
}

void TCoinBlue::taken(THitActor* param_1)
{
	gpMarDirector->fireGetBlueCoin(this);

	if (unk148)
		unk148->receiveMessage(this, HIT_MESSAGE_UNK8);

	TItem::taken(param_1);
}

void TCoinBlue::loadBeforeInit(JSUMemoryInputStream& stream)
{
	int thing;
	stream.read(&thing, 4);
	if (thing == -1)
		thing = 0;
	setUnk134(thing);
}

void TCoinBlue::load(JSUMemoryInputStream& stream)
{
	TCoin::load(stream);
	if (TFlagManager::getInstance()->getBlueCoinFlag(
	        gpMarDirector->getCurrentMap(), getUnk134()))
		makeObjDead();
}

TCoinBlue::TCoinBlue(const char* name)
    : TCoin(name)
{
}

u32 TShine::mPromiLife     = 1;
u32 TShine::mSenkoRate     = 1;
u32 TShine::mKiraRate      = 1;
u32 TShine::mBowRate       = 1;
u32 TShine::mCircleRateY   = 1;
u32 TShine::mUpSpeed       = 1;
u32 TShine::mSpeedDownRate = 1;
u32 TShine::mSpeedDownTime = 1;

void TShine::calc() { }

void TShine::movingCircle() { }

void TShine::movingUp() { }

void TShine::movingDown() { }

void TShine::control() { }

void TShine::perform(u32, JDrama::TGraphics*) { }

BOOL TShine::receiveMessage(THitActor*, u32) { }

void TShine::touchPlayer(THitActor*) { }

void TShine::appearWithTime(int, int, int, int) { }

void TShine::appearWithTimeCallback(u32, u32) { }

void TShine::appearSimple(int) { }

void TShine::appearWithDemo(const char*) { }

void TShine::kill() { }

void TShine::makeMActors() { }

void TShine::initMapObj() { }

void TShine::loadAfter() { }

void TShine::loadBeforeInit(JSUMemoryInputStream& stream) { }

TShine::TShine(const char* name)
    : TItem(name)
{
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
	if (isState(0xE) || isState(6))
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
	if (!isState(1) && !isState(0xD))
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
			mState = 1;
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
			mState = 0;
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

void TEggYoshi::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	TMapObjGeneral::perform(param_1, param_2);

	if (!isState(0xC) && !isState(0) && !isState(6) && !isState(2)
	    && !isState(0xE) && !isState(0xF) && !isState(0x10)) {
		if (param_1 & 2)
			unk148->getModel()->setBaseTRMtx(getModel()->getAnmMtx(0));

		unk148->perform(param_1, param_2);
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

	if (message == HIT_MESSAGE_UNK7 || message == HIT_MESSAGE_UNK8) {
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
		mState = 1;
		return TRUE;
	}

	if (message == HIT_MESSAGE_UNK6) {
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

	unk148 = SMS_MakeMActorWithAnmData("/scene/mapObj/eggYoshi_fukidashi.bmd",
	                                   mManager->getMActorAnmData(), 3,
	                                   0x10210000);
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

void TItemNozzle::touchPlayer(THitActor*) { }

void TItemNozzle::put() { }

BOOL TItemNozzle::receiveMessage(THitActor*, u32) { }

void TItemNozzle::appearing() { }

void TItemNozzle::control() { }

void TItemNozzle::initMapObj() { }

void TItemNozzle::load(JSUMemoryInputStream&) { }

void TNozzleBox::makeModelValid() { }

void TNozzleBox::makeModelInvalid() { }

void TNozzleBox::breaking() { }

BOOL TNozzleBox::receiveMessage(THitActor*, u32) { }

void TNozzleBox::touchPlayer(THitActor*) { }

void TNozzleBox::control() { }

void TNozzleBox::loadAfter() { }

void TNozzleBox::load(JSUMemoryInputStream&) { }

TNozzleBox::TNozzleBox(const char* name)
    : TMapObjGeneral(name)
{
}
