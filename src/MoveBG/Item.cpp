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
#include <Strategic/MirrorActor.hpp>
#include <Strategic/question.hpp>
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
	if (checkMapObjFlag(0x40000) && !isUnk104Positive()) {
		if (unk148)
			unk148->receiveMessage(this, 5);

		if (isActorType(0x2000000f) || isActorType(0x20000010)) {
			if (gpMSound->gateCheck(0x484C))
				MSoundSESystem::MSoundSE::startSoundActor(0x484C, mPosition, 0,
				                                          nullptr, 0, 4);
		}
	}

	TMapObjGeneral::appeared();
}

void TItem::taken(THitActor* param_1)
{
	param_1->receiveMessage(this, 0xE);
	kill();
	if (checkMapObjFlag(0x80000)) {
		makeObjDefault();
		appear();
	}
}

void TItem::touchPlayer(THitActor* param_1)
{
	if ((param_1->isActorType(0x80000001) || param_1->isActorType(0x8000083))
	    && !checkHitFlag(1))
		taken(param_1);
}

u32 TItem::receiveMessage(THitActor* param_1, u32 param_2)
{
	if (param_2 == 0xF)
		return false;

	if (param_2 == 0xb) {
		taken(param_1);
		return true;
	}

	return TMapObjGeneral::receiveMessage(param_1, param_2);
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
	unk14C = param_1;
	unk104 = unk150;

	offMapObjFlag(0x10000000);
	onHitFlag(1);
	offMapObjFlag(0x40000);
}

void TItem::appear()
{
	TMapObjGeneral::appear();
	onHitFlag(1);
	unk104 = unk150;
	offMapObjFlag(0x40000);
}

void TItem::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (checkLiveFlag(0x1))
		return;

	if ((param_1 & 1) && checkHitFlag(1) && !isUnk104Positive()) {
		offHitFlag(1);
		if (!checkMapObjFlag(0x10000000)) {
			onMapObjFlag(0x40000);
			unk104 = unk14C;
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
	u8 thing = gpApplication.currArea._000;
	TFlagManager::getInstance()->incGoldCoinFlag(SMS_getShineStage(thing), 1);

	if (gpMSound->gateCheck(0x4811))
		MSoundSESystem::MSoundSE::startSoundActor(0x4811, mPosition, 0, nullptr,
		                                          0, 4);

	if (unk148)
		unk148->receiveMessage(this, 8);

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
			if (gpMSound->gateCheck(0x4843))
				MSoundSESystem::MSoundSE::startSoundSystemSE(0x4843, 0, nullptr,
				                                             0);
	} else {
		if (gpMSound->gateCheck(0x4813))
			MSoundSESystem::MSoundSE::startSoundSystemSE(0x4813, 0, nullptr, 0);
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
	if (checkLiveFlag(1))
		return;

	if ((param_1 & 1) && checkLiveFlag(0x10)) {
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

		if (isUnk104Positive()) {
			--unk104;
		} else {
			if (checkHitFlag(1)) {
				offHitFlag(1);
				if (!checkMapObjFlag(0x10000000)) {
					onMapObjFlag(0x40000);
					unk104 = unk14C;
				}
			} else {
				if (!checkMapObjFlag(0x10000000)) {
					if (unk148 != 0)
						unk148->receiveMessage(this, 5);
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
		bool bVar2;
		if (check->unk0 == 0x100 || check->unk0 == 0x101 || check->unk0 == 0x102
		    || check->unk0 == 0x103 || check->unk0 == 0x104
		    || check->unk0 == 0x105 || check->unk0 == 0x4104)
			bVar2 = true;
		else
			bVar2 = false;

		if (!bVar2)
			return;
	}

	unk154 = new TMirrorActor("コインin鏡");
	unk154->init(getModel(), 0x18);
}

// TODO: move this
extern u8 gParticleFlagLoaded[];
extern JPAResourceManager* gpResourceManager;

void TCoin::initMapObj()
{
	TItem::initMapObj();

	// TODO: inline
	if (gParticleFlagLoaded[0x58] == 0) {
		gpResourceManager->load("/scene/mapObj/ms_watcoin_kira.jpa", 0x58);
		gParticleFlagLoaded[0x58] = 1;
	}
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

	if (gpMSound->gateCheck(0x4846))
		MSoundSESystem::MSoundSE::startSoundActor(0x4846, mPosition, 0, nullptr,
		                                          0, 4);

	if (unk148)
		unk148->receiveMessage(this, 8);

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
		unk148->receiveMessage(this, 8);

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

u32 TShine::receiveMessage(THitActor*, u32) { }

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

void TEggYoshi::decideRandomLoveFruit() { }

void TEggYoshi::startBalloonAnim() { }

void TEggYoshi::touchFruit(THitActor*) { }

void TEggYoshi::touchActor(THitActor*) { }

void TEggYoshi::control() { }

void TEggYoshi::perform(u32, JDrama::TGraphics*) { }

void TEggYoshi::startFruit() { }

u32 TEggYoshi::receiveMessage(THitActor*, u32) { }

void TEggYoshi::load(JSUMemoryInputStream&) { }

TEggYoshi::TEggYoshi(const char* name)
    : TMapObjGeneral(name)
{
}

void TItemNozzle::touchPlayer(THitActor*) { }

void TItemNozzle::put() { }

u32 TItemNozzle::receiveMessage(THitActor*, u32) { }

void TItemNozzle::appearing() { }

void TItemNozzle::control() { }

void TItemNozzle::initMapObj() { }

void TItemNozzle::load(JSUMemoryInputStream&) { }

void TNozzleBox::makeModelValid() { }

void TNozzleBox::makeModelInvalid() { }

void TNozzleBox::breaking() { }

u32 TNozzleBox::receiveMessage(THitActor*, u32) { }

void TNozzleBox::touchPlayer(THitActor*) { }

void TNozzleBox::control() { }

void TNozzleBox::loadAfter() { }

void TNozzleBox::load(JSUMemoryInputStream&) { }

TNozzleBox::TNozzleBox(const char* name)
    : TMapObjGeneral(name)
{
}
