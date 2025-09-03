#include <Enemy/HamuKuri.hpp>
#include <Enemy/Graph.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/ObjModel.hpp>
#include <System/EmitterViewObj.hpp>
#include <Map/MapData.hpp>
#include <Map/Map.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/MarioMain.hpp>
#include <M3DUtil/MActorData.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/SDLModel.hpp>
#include <MarioUtil/PacketUtil.hpp>
#include <MarioUtil/TexUtil.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MoveBG/ItemManager.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <MoveBG/MapObjBase.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSe.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

f32 THamuKuri::mCapGravityY         = 0.15f;
f32 THamuKuri::mCapSpeed            = 12.0f;
f32 THamuKuri::mVGenerateGravityY   = 0.15f;
f32 THamuKuri::mLandAnmFrameNum     = 90.0f;
bool THamuKuriManager::mSearchActSw = true;

bool THaneHamuKuri::mBoundFly;
bool TDangoHamuKuri::mAttackSw;

static const char* hanekuri_bastable[] = {
	"/scene/hanekuri/bas/hanekuri_crash.bas",
	"/scene/hanekuri/bas/hanekuri_down.bas",
	"/scene/hanekuri/bas/hanekuri_roll.bas",
	"/scene/hanekuri/bas/hanekuri_wait.bas",
};

static const char* hamukurianm_bastable[] = {
	nullptr,
	nullptr,
	"/scene/hamukurianm/bas/hamukuri_down.bas",
	"/scene/hamukurianm/bas/hamukuri_jitabata.bas",
	nullptr,
	nullptr,
	"/scene/hamukurianm/bas/hamukuri_return.bas",
	"/scene/hamukurianm/bas/hamukuri_roll.bas",
	"/scene/hamukurianm/bas/hamukuri_run.bas",
	"/scene/hamukurianm/bas/hamukuri_set.bas",
	"/scene/hamukurianm/bas/hamukuri_turn_down.bas",
	"/scene/hamukurianm/bas/hamukuri_turn_end.bas",
	"/scene/hamukurianm/bas/hamukuri_turn_start.bas",
	nullptr,
	"/scene/hamukurianm/bas/hamukuri_walk.bas",
};

void THamuKuriLauncher::drawObject(JDrama::TGraphics* param_1)
{
	TLiveActor::drawObject(param_1);
}

void THamuKuriLauncher::stateLaunch() { }

THamuKuriSaveLoadParams::THamuKuriSaveLoadParams(const char* path)
    : TWalkerEnemyParams(path)
    , PARAM_INIT(mSLWaterCoeff, 6.0f)
    , PARAM_INIT(mSLWaterAttackCoeff, 12.0f)
    , PARAM_INIT(mSLFirstVelocityY, 5.0f)
    , PARAM_INIT(mSLVelocityRate, 0.8f)
    , PARAM_INIT(mSLBoundNum, 3)
    , PARAM_INIT(mSLSearchActorTime, 500)
    , PARAM_INIT(mSLCanSearchDist, 800.0f)
    , PARAM_INIT(mSLJitabataTimer, 200)
    , PARAM_INIT(mSLFirstKickVelocityY, 2.0f)
    , PARAM_INIT(mSLFlyTimer, 500)
    , PARAM_INIT(mSLTrampleBonusNum, 3)
    , PARAM_INIT(mSLCrashBonusNum, 3)
    , PARAM_INIT(mSLSerialCrashFrame, 15)
    , PARAM_INIT(mSLKyoroTimer, 500)
{
	TParams::load(mPrmPath);
}

THaneHamuKuriSaveLoadParams::THaneHamuKuriSaveLoadParams(const char* path)
    : THamuKuriSaveLoadParams(path)
    , PARAM_INIT(mSLNormalJumpVy, 8.0f)
    , PARAM_INIT(mSLAttackJumpVy, 4.0f)
    , PARAM_INIT(mSLFlyBaseHeight, 100.0f)
    , PARAM_INIT(mSLFlyBaseAmplitude, 50.0f)
    , PARAM_INIT(mSLFlyBaseFrequency, 100.0f)
{
	TParams::load(mPrmPath);
}

TBossDangoHamuKuriSaveLoadParams::TBossDangoHamuKuriSaveLoadParams(
    const char* path)
    : THamuKuriSaveLoadParams(path)
{
}

TFireHamuKuriSaveLoadParams::TFireHamuKuriSaveLoadParams(const char* path)
    : THamuKuriSaveLoadParams(path)
{
}

THamuKuriManager::THamuKuriManager(const char* name)
    : TSmallEnemyManager(name)
    , unk60(0)
    , unk64(nullptr)
    , unk68(0)
    , unk6C(0)
    , unk70(0)
{
}

void THamuKuriManager::load(JSUMemoryInputStream& stream)
{
	unk38 = new THamuKuriSaveLoadParams("/enemy/hamukuri.prm");
	TSmallEnemyManager::load(stream);
}

void THamuKuriManager::loadAfter()
{
	TSmallEnemyManager::loadAfter();

	TMapObjBaseManager::newAndRegisterObj("mushroom1up");
	unk60 = 0x40000064;
	unk68 = 0;

	u32 iVar1 = gpMapObjManager->getObjNumWithActorType(unk60);
	if (iVar1 > 0) {
		unk64 = new TMapObjBase*[iVar1];
		for (int i = 0; i < gpMapObjManager->getObjNum(); ++i) {
			if (gpMapObjManager->getObj(i)->isActorType(unk60)) {
				unk64[unk68] = gpMapObjManager->getObj(i);
				++unk68;
			}
		}
	}
}

void THamuKuriManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "default.bmd", 0x10220000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

void THamuKuriManager::createAnmData()
{
	MActorAnmData* data = new MActorAnmData;
	data->init("/scene/hamukurianm", nullptr);
	unk20 = data;
}

void THamuKuriManager::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (unk68 > 0 && mSearchActSw && (param_1 & 2))
		setSearchHamuKuri();

	if (param_1 & 1)
		checkSerialKill();

	TEnemyManager::perform(param_1, param_2);
}

void THamuKuriManager::setSearchHamuKuri()
{
	for (int i = 0; i < getActiveObjNum(); ++i) {
		THamuKuri* kuri = (THamuKuri*)getObj(i);

		// TODO: inline
		bool bVar6;
		if (kuri->mSpine->getCurrentNerve() == &TNerveWalkerAttack::theNerve()
		    || kuri->mSpine->getCurrentNerve()
		           == &TNerveWalkerGraphWander::theNerve())
			bVar6 = true;
		else
			bVar6 = false;

		if (!bVar6)
			continue;

		if (kuri->checkLiveFlag(LIVE_FLAG_DEAD | LIVE_FLAG_UNK2
		                        | LIVE_FLAG_CLIPPED_OUT))
			continue;

		THamuKuriSaveLoadParams* params = (THamuKuriSaveLoadParams*)unk38;
		s32 searchActorTime             = params->mSLSearchActorTime.get();
		if (kuri->unk19C >= searchActorTime) {
			f32 canSearchDist = params->mSLCanSearchDist.get();

			TMapObjBase* pTVar18 = nullptr;
			f32 fVar1            = canSearchDist * canSearchDist;

			for (int i = 0; i < unk68; ++i) {
				TMapObjBase* uVar16 = unk64[i];
				if (uVar16->checkLiveFlag(LIVE_FLAG_DEAD
				                          | LIVE_FLAG_CLIPPED_OUT))
					continue;

				if (uVar16->isAirborne())
					continue;

				JGeometry::TVec3<f32> tmp = kuri->mPosition;
				tmp.sub(uVar16->mPosition);
				f32 len_sq = tmp.squared();

				if (len_sq < fVar1) {
					pTVar18 = uVar16;
					fVar1   = len_sq;
				}
			}

			if (pTVar18 != nullptr)
				kuri->setSearchActor(pTVar18);
		}
	}
}

void THamuKuriManager::requestSerialKill(THamuKuri* param_1)
{
	int trampled = 1;

	THamuKuriSaveLoadParams* params = (THamuKuriSaveLoadParams*)unk38;

	s32 trampleBonusNum = params->mSLTrampleBonusNum.get();

	for (int i = 0; i < getActiveObjNum(); ++i) {
		THamuKuri* obj = (THamuKuri*)unk18[i];
		if (obj != param_1 && obj->isBckAnm(3)) {
			obj->kill();
			++trampled;
		}
	}

	if (trampled >= trampleBonusNum) {
		gpItemManager->makeObjAppear(param_1->mPosition.x, param_1->mPosition.y,
		                             param_1->mPosition.z, 0x20000005, true);
	}
}

void THamuKuriManager::checkSerialKill()
{
	if (unk6C != 0) {
		int count                       = 0;
		THamuKuriSaveLoadParams* params = (THamuKuriSaveLoadParams*)unk38;

		int crashBonusNum    = params->mSLCrashBonusNum.value;
		int serialCrashFrame = params->mSLSerialCrashFrame.value;

		++unk6C;
		if (unk6C > serialCrashFrame)
			unk6C = 0;

		THamuKuri* rep = nullptr;

		for (int i = 0; i < getActiveObjNum(); ++i) {
			THamuKuri* obj = (THamuKuri*)getObj(i);

			bool bVar1;

			if (obj->unk1A3 != 0
			    && obj->mSpine->getCurrentNerve()
			           == &TNerveHamuKuriWallDie::theNerve())
				bVar1 = true;
			else
				bVar1 = false;

			if (bVar1) {
				++count;
				rep = obj;
			}
		}

		if (rep != nullptr && count >= crashBonusNum) {
			gpItemManager->makeObjAppear(rep->mPosition.x, rep->mPosition.y,
			                             rep->mPosition.z, 0x20000003, true);
		}
	}
}

TSpineEnemy* THamuKuriManager::createEnemyInstance() { return new THamuKuri; }

THaneHamuKuriManager::THaneHamuKuriManager(const char* name)
    : THamuKuriManager(name)
{
}

void THaneHamuKuriManager::load(JSUMemoryInputStream& stream)
{
	unk38 = new THaneHamuKuriSaveLoadParams("/enemy/hanehamukuri.prm");
	TSmallEnemyManager::load(stream);
}

TSpineEnemy* THaneHamuKuriManager::createEnemyInstance()
{
	return new THaneHamuKuri;
}

void THaneHamuKuriManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "hanekuri.bmd", 0x10250000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

void THaneHamuKuriManager::createAnmData() { TObjManager::createAnmData(); }

TDoroHaneKuriManager::TDoroHaneKuriManager(const char* name)
    : THaneHamuKuriManager(name)
{
}

void TDoroHaneKuriManager::load(JSUMemoryInputStream& stream)
{
	unk38 = new THaneHamuKuriSaveLoadParams("/enemy/dorohanekuri.prm");
	TSmallEnemyManager::load(stream);
}

void TDoroHaneKuriManager::loadAfter()
{
	THaneHamuKuriManager::loadAfter();

	TMapObjManager::newAndRegisterObj("mario_cap");
}

TSpineEnemy* TDoroHaneKuriManager::createEnemyInstance()
{
	return new TDoroHaneKuri;
}

void TDoroHaneKuriManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "dorohane.bmd", 0x10250000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

TDangoHamuKuriManager::TDangoHamuKuriManager(const char* name)
    : THamuKuriManager(name)
{
}

void TDangoHamuKuriManager::load(JSUMemoryInputStream&) { }

TSpineEnemy* TDangoHamuKuriManager::createEnemyInstance() { }

void TDangoHamuKuriManager::createModelDataArray(const TModelDataLoadEntry*) { }

void TDoroHaneKuriManager::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	THamuKuriManager::perform(param_1, param_2);

	if (unk74)
		unk74->perform(param_1, param_2);
}

void TDoroHaneKuriManager::createHige()
{
	void* rawModelData = JKRGetResource("/scene/hanekuri/dorokuriHige.bmd");
	SDLModelData* modelData = new SDLModelData(
	    J3DModelLoaderDataBase::load(rawModelData, 0x10210000));
	unk74 = new TDoroHige((TLiveActor*)unk18[0], 7, modelData);
}

TBossDangoHamuKuriManager::TBossDangoHamuKuriManager(const char* name)
    : TDangoHamuKuriManager(name)
{
}

void TBossDangoHamuKuriManager::load(JSUMemoryInputStream&) { }

TSpineEnemy* TBossDangoHamuKuriManager::createEnemyInstance() { }

static GXColorS10 mFireHamNoseColorDiff  = { 0, 0, 0, 0 };
static GXColorS10 mFireHamOtherColorDiff = { 0, 0, 0, 0 };

static GXColorS10 mFireHamNoseColorStart  = { 0x1F4, 0xC8, 0x78, 0xFF };
static GXColorS10 mFireHamNoseColorEnd    = { 0xFFEC, 0xFFA6, 0xFF74, 0xFF };
static GXColorS10 mFireHamOtherColorStart = { 0x1F4, 0xFFD8, 0xFFBA, 0xFF };
static GXColorS10 mFireHamOtherColorEnd   = { 0x50, 0x5, 0xFFCE, 0xFF };

TFireHamuKuriManager::TFireHamuKuriManager(const char* name)
    : THamuKuriManager(name)
{
}

void TFireHamuKuriManager::load(JSUMemoryInputStream&) { }

TSpineEnemy* TFireHamuKuriManager::createEnemyInstance() { }

void TFireHamuKuriManager::initSetEnemies() { }

void TFireHamuKuriManager::createModelData() { }

void TDoroHige::perform(u32, JDrama::TGraphics*) { }

TDoroHamuKuriManager::TDoroHamuKuriManager(const char* name)
    : THamuKuriManager(name)
{
}

void TDoroHamuKuriManager::load(JSUMemoryInputStream&) { }

void TDoroHamuKuriManager::loadAfter() { }

TSpineEnemy* TDoroHamuKuriManager::createEnemyInstance() { }

void TDoroHamuKuriManager::createModelData() { }

void TDoroHamuKuriManager::perform(u32, JDrama::TGraphics*) { }

void TDoroHamuKuriManager::createHige() { }

THamuKuri::THamuKuri(const char* name)
    : TWalkerEnemy(name)
    , unk194(0.0f)
    , unk198(0)
    , unk19C(0)
    , unk1A0(0)
    , unk1A1(0)
    , unk1A2(0)
    , unk1A3(0)
    , unk1A4(0)
    , unk1AC(3)
    , unk1E0(0)
    , unk1F0(0)
    , unk1F8(nullptr)
{
}

void THamuKuri::init(TLiveManager* param_1)
{
	TWalkerEnemy::init(param_1);
	mActorType = 0x10000002;
	unk150     = 17;
	unk1F4     = (THamuKuriSaveLoadParams*)getSaveParam();
	onHitFlag(0x40000000);
	mSpine->initWith(&TNerveWalkerGenerate::theNerve());
	mMActor->setLightType(1);
}

void THamuKuri::setMActorAndKeeper()
{
	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("default.bmd", 3);
	int idx       = getModel()->getModelData()->getMaterialName()->getIndex(
        "_mat_body_top1");
	SMS_InitPacket_OneTevKColor(mMActor->getModel(), idx, GX_KCOLOR0, &unk1FC);
	unk1FC.a = 0x80;
	ResTIMG* img
	    = (ResTIMG*)JKRGetResource("/scene/map/pollution/H_ma_rak.bti");
	if (img != nullptr)
		SMS_ChangeTextureAll(mMActor->getModel()->getModelData(),
		                     "H_ma_rak_dummy", *img);
	else
		unk1FC.a = 0;
}

void THamuKuri::reset()
{
	TWalkerEnemy::reset();
	offLiveFlag(0x2);
	unk194 = unk1F4->mSLGiveUpLength.get();
	unk1F0 = 0;
	unk1A3 = 0;
	unk1A4 = 0;
}

void THamuKuri::bind()
{
	if (unk1A4) {
		if (mSpine->getTime() > unk1F4->mSLFlyTimer.get()
		    || getGroundPlane()->isWaterSurface())
			unk1A4 = 0;

		mVelocity.y = 0.0f;

		// NOTE: copypasta from TLiveActor::bind

		JGeometry::TVec3<f32> nextPos = mPosition;
		nextPos += mLinearVelocity;
		nextPos += mVelocity;

		{
			if (checkLiveFlag(LIVE_FLAG_UNK1000)) {
				mGroundHeight = gpMap->checkGroundIgnoreWaterSurface(
				    nextPos.x, nextPos.y + mHeadHeight, nextPos.z,
				    &mGroundPlane);
			} else {
				mGroundHeight
				    = gpMap->checkGround(nextPos.x, nextPos.y + mHeadHeight,
				                         nextPos.z, &mGroundPlane);
			}
			mGroundHeight += 1.0f;

			if (nextPos.y <= mGroundHeight + 0.05f) {
				if (mGroundPlane->checkFlag(BG_CHECK_FLAG_ILLEGAL))
					kill();

				nextPos.y = mGroundHeight + 1.0f;
			} else {
				onLiveFlag(LIVE_FLAG_AIRBORNE);
			}
		}

		gpMap->isTouchedOneWallAndMoveXZ(&nextPos.x, nextPos.y + mHeadHeight,
		                                 &nextPos.z, mBodyRadius);

		mLinearVelocity = nextPos - mPosition;
	} else {
		TLiveActor::bind();
	}
}

void THamuKuri::releaseCap() { }

void THamuKuri::behaveToWater(THitActor* param_1)
{
	if (mSpine->getCurrentNerve() != &TNerveHamuKuriBoundFreeze::theNerve()
	    && mSpine->getCurrentNerve() != &TNerveHamuKuriWallDie::theNerve()) {
		if (mSpine->getCurrentNerve() == &TNerveHamuKuriJitabata::theNerve()) {
			unk165 = 1;
			return;
		}

		if (canDoJitabata()) {
			mSpine->pushNerve(&TNerveHamuKuriJitabata::theNerve());
			return;
		}

		if (mVelocity.y < 0.0f)
			forceRoll(SMS_GetMarioPos(), true);
	}
}

bool THamuKuri::isFindMario(f32 param_1)
{
	unk1A2 = 0;

	if (unk198)
		return false;

	if (gpMarioOriginal->unk380 == 0
	    && !*(int*)((u8*)gpMarioOriginal->mWaterGun + 0x1C80) /* TODO: */) {
		unk194 = unk1F4->mSLGiveUpLength.get();
		unk194 *= 3.0f;
		return TSmallEnemy::isFindMario(param_1 * 5.0f);
	} else {
		return TSmallEnemy::isFindMario(param_1);
	}
}

void THamuKuri::setSearchActor(THitActor* param_1)
{
	unk19C = 0;
	unk1A0 = 0;
	mSpine->pushNerve(&TNerveHamuKuriGoForSearchActor::theNerve());
	unk1F8 = param_1;
	setGoalPath(TPathNode(param_1->mPosition));
}

void THamuKuri::isGiveUpSearchActor() { }

void THamuKuri::jumpToSearchActor()
{
	TLiveActor* pTVar5 = (TLiveActor*)unk1F8;
	if (pTVar5) {
		JGeometry::TVec3<f32> local_1C = mPosition;
		local_1C -= pTVar5->getPosition();

		if (local_1C.squared() > 40000.0f) {
			JGeometry::TVec3<f32> tgt = pTVar5->getPosition();
			tgt.y = ((TLiveActor*)pTVar5)->getGroundHeight();

			setGoalPath(TPathNode(tgt));
			return;
		}

		mVelocity
		    = calcVelocityToJumpToY(unk1F8->mPosition, 1.0f, getGravityY());
		onLiveFlag(LIVE_FLAG_AIRBORNE);

		if (gpMSound->gateCheck(0x2939))
			MSoundSESystem::MSoundSE::startSoundActor(0x2939, &mPosition, 0,
			                                          nullptr, 0, 4);
	}
}

void THamuKuri::canGoForSearchActor() { }

void THamuKuri::behaveToFindMario()
{
	TWalkerEnemy::behaveToFindMario();

	if (gpMSound->gateCheck(0x2826))
		MSoundSESystem::MSoundSE::startSoundActor(0x2826, &mPosition, 0,
		                                          nullptr, 0, 4);
}

void THamuKuri::attackToMario()
{
	if (mSpine->getCurrentNerve() == &TNerveHamuKuriJitabata::theNerve()) {
		if (mPosition.y + 10.0f > gpMarioPos->y) {
			forceRoll(SMS_GetMarioPos(), false);
			SMSRumbleMgr->start(0x15, 5, (f32*)nullptr);
		}
	} else {
		TWalkerEnemy::attackToMario();
	}
}

void THamuKuri::moveObject()
{
	if (unk198) {
		offLiveFlag(LIVE_FLAG_CLIPPED_OUT);
		if (!isAirborne()) {
			if (checkLiveFlag(LIVE_FLAG_UNK2)) {
				mSpine->reset();
				mSpine->setNext(&TNerveWalkerGenerate::theNerve());
				TLiveActor* heldObj = (TLiveActor*)mHeldObject;
				if (heldObj)
					heldObj->onLiveFlag(LIVE_FLAG_UNK2);
			}

			offLiveFlag(LIVE_FLAG_UNK2);
			offHitFlag(HIT_FLAG_UNK1);
		}
	}

	// TODO: inline
	bool bVar1;
	if (mSpine->getCurrentNerve() == &TNerveWalkerAttack::theNerve()
	    || mSpine->getCurrentNerve() == &TNerveWalkerGraphWander::theNerve()) {
		bVar1 = true;
	} else {
		bVar1 = false;
	}

	if (bVar1)
		unk19C += 1;

	if (mSpine->getCurrentNerve() == &TNerveWalkerGraphWander::theNerve()) {
		s32 kyoroTimer = unk1F4->mSLKyoroTimer.get();
		unk1A8 += 1;

		if (unk1A8 > kyoroTimer) {
			unk1A8 = 0;
			setBckAnm(4);
		}

		if (isBckAnm(4) && checkCurAnmEnd(0))
			setBckAnm(14);
	}

	TWalkerEnemy::moveObject();
}

void THamuKuri::setBehavior()
{
	if (isAirborne() && mPosition.y > mGroundHeight + 250.0f
	    && mSpine->getCurrentNerve() != &TNerveWalkerGenerate::theNerve()) {
		unk1F0 = 1;
	}

	if (!isAirborne() && unk1F0
	    && (mSpine->getCurrentNerve() == &TNerveWalkerGraphWander::theNerve()
	        || mSpine->getCurrentNerve() == &TNerveWalkerAttack::theNerve())) {
		mSpine->pushNerve(&TNerveHamuKuriLand::theNerve());
	}
}

void THamuKuri::changeCapHolder() { }

void THamuKuri::selectCapHolder()
{
	if (!gpMarioOriginal->isWearingCap()) {
		sendAttackMsgToMario();
	} else {
		sendAttackMsgToMario();
		if (getManager()->unk70 == 0) {
			TMapObjBase* obj = gpItemManager->makeObjAppear(
			    mPosition.x, mPosition.y, mPosition.z, 0x2000003C, false);

			if (obj) {
				offLiveFlag(LIVE_FLAG_CLIPPED_OUT);
				makeCapFly(obj);
			}
		}
	}
}

void THamuKuri::makeCapFly(TMapObjBase* param_1)
{
	JGeometry::TVec3<f32> local_3c = mPosition;
	local_3c.x += -mVelocity.x * 20.0f;
	local_3c.z += -mVelocity.z * 20.0f;

	// TODO: control flow all over the place, definitely inlines needed

	TSmallEnemy* holder = getManager()->getHolder(mInstanceIndex);
	if (holder == nullptr && mGroundPlane->checkFlag(BG_CHECK_FLAG_ILLEGAL)
	    && mGroundPlane->isPool() && mGroundPlane->isWaterSurface()) {
		mPosition = SMS_GetMarioPos();
		mVelocity.set(0.0f, 10.0f, 0.0f);
		offLiveFlag(LIVE_FLAG_UNK10);
	} else {
		mPosition = local_3c;
		mPosition.y += 100.0f;
		param_1->mPosition.y = mPosition.y;

		if (param_1->receiveMessage(this, 0x4)) {
			onLiveFlag(LIVE_FLAG_DEAD);
		} else {
			reset();
			onHaveCap();
			mHeldObject = param_1;
			onLiveFlag(LIVE_FLAG_UNK2);
			offLiveFlag(LIVE_FLAG_DEAD);
			offLiveFlag(LIVE_FLAG_CLIPPED_OUT);
			onHitFlag(HIT_FLAG_UNK1);
			getManager()->unk70 = this;

			int uVar11 = unk124->getCurrentIndex();

			int count  = MsRandF(2, 3);
			int uVar10 = -1;
			for (int i = 0; i < count; ++i) {
				int next = unk124->unk0->getRandomNextIndex(uVar11, uVar10,
				                                            0xffffffff);
				uVar10   = uVar11;
				uVar11   = next;
			}

			if (uVar11 < 0)
				uVar11 = 0;

			JGeometry::TVec3<f32> VStack_60;
			unk124->getGraph()->getGraphNode(uVar11).getPoint(&VStack_60);

			JGeometry::TVec3<f32> local_6c
			    = calcVelocityToJumpToY(VStack_60, mCapSpeed, getGravityY());
			onLiveFlag(LIVE_FLAG_AIRBORNE);
			mVelocity = local_6c;
		}
	}
}

f32 THamuKuri::getGravityY() const
{
	if (checkLiveFlag(LIVE_FLAG_UNK2))
		return mCapGravityY;

	if (mSpine->getCurrentNerve() == &TNerveWalkerGenerate::theNerve()
	    && unk1A1)
		return mVGenerateGravityY;
	else
		return mGravity;
}

void THamuKuri::genRandomItem() { TSmallEnemy::genRandomItem(); }

void THamuKuri::setAfterDeadEffect()
{
	if (unk198) {
		TMapObjBase* obj = gpItemManager->makeObjAppear(
		    mPosition.x, mPosition.y, mPosition.z, 0x2000003c, true);

		if (obj) {
			unk200.set(mPosition.x, mGroundHeight, mPosition.z);
			gpMarioParticleManager->emitAndBindToPosPtr(0xE4, &unk200, 0,
			                                            nullptr);
		}
		unk198              = 0;
		getManager()->unk70 = 0;
	}

	JPABaseEmitter* emitter;
	if (isBckAnm(1)) {
		emitter = gpMarioParticleManager->emit(0xE5, &mPosition, 0, nullptr);

		if (emitter)
			emitter->setScale(mScaling);
	} else {
		emitter = gpMarioParticleManager->emit(0xE4, &mPosition, 0, nullptr);
	}

	if (emitter)
		emitter->setScale(mScaling);

	emitter = gpMarioParticleManager->emit(0xE6, &mPosition, 0, nullptr);
	if (emitter)
		emitter->setScale(mScaling);

	if (gpMSound->gateCheck(0x295F))
		MSoundSESystem::MSoundSE::startSoundActor(0x295F, &mPosition, 0,
		                                          nullptr, 0, 4);
}

void THamuKuri::endHitWaterJump()
{
	if (!isFindMario(1.3f))
		unk1A2 = 1;
}

void THamuKuri::setBckAnm(int param_1) { TSmallEnemy::setBckAnm(param_1); }

void THamuKuri::setGenerateAnm() { setBckAnm(9); }

void THamuKuri::setWalkAnm() { setBckAnm(4); }

void THamuKuri::setDeadAnm()
{
	if (unk198 && mHeldObject != nullptr
	    && mHeldObject->receiveMessage(this, 6)) {
		TMapObjBase* heldObj = (TMapObjBase*)mHeldObject;
		heldObj->mHolder     = nullptr;
		heldObj->offLiveFlag(LIVE_FLAG_UNK2);
		heldObj->mPosition   = mPosition;
		heldObj->mPosition.y = mGroundHeight;
		heldObj->offHitFlag(HIT_FLAG_UNK1);
		heldObj->makeObjDead();
		mHeldObject = nullptr;
	}

	if (unk184) {
		onLiveFlag(0x20000);
		gpMarioParticleManager->emit(0xE4, &mPosition, 0, nullptr);
	} else {
		if (isBckAnm(3))
			setBckAnm(10);
		else
			setBckAnm(2);
	}

	unk19C = 0;
}

void THamuKuri::setWaitAnm() { setBckAnm(13); }

void THamuKuri::setRunAnm() { setBckAnm(8); }

void THamuKuri::setRollAnm() { setBckAnm(7); }

void THamuKuri::setCrashAnm()
{
	if (unk198 && mHeldObject != nullptr
	    && mHeldObject->receiveMessage(this, 6)) {
		TMapObjBase* heldObj = (TMapObjBase*)mHeldObject;
		heldObj->mHolder     = nullptr;
		heldObj->offLiveFlag(LIVE_FLAG_UNK2);
		heldObj->mPosition   = mPosition;
		heldObj->mPosition.y = mGroundHeight;
		heldObj->offHitFlag(HIT_FLAG_UNK1);
		heldObj->makeObjDead();
		mHeldObject = nullptr;
	}

	unk1A4 = 0;
	setBckAnm(1);

	if (unk1A3 && getManager()->getUnk6C() == 0)
		getManager()->setUnk6C(1);
}

void THamuKuri::setWallDeadEffect() { }

void THamuKuri::setAppearAnm() { }

void THamuKuri::walkBehavior(int param_1, f32 param_2)
{
	if (!isBckAnm(4))
		TWalkerEnemy::walkBehavior(param_1, param_2);
}

void THamuKuri::initAttacker(THitActor* param_1)
{
	TWalkerEnemy::initAttacker(param_1);
	mSpine->pushNerve(&TNerveWalkerTraceMario::theNerve());
}

const char** THamuKuri::getBasNameTable() const { return hamukurianm_bastable; }

MtxPtr THamuKuri::getTakingMtx()
{
	f32 dVar4 = gpMap->checkGround(mPosition.x, mPosition.y + mHeadHeight,
	                               mPosition.z, &mGroundPlane);

	mHeldObject->onHitFlag(HIT_FLAG_UNK1);

	TPosition3f mat;
	mat.translation(mPosition.x, mPosition.y, mPosition.z);

	Mtx afStack_84;
	MsMtxSetRotRPH(afStack_84, 0.0f, 0.0f, 0.0f);
	MTXConcat(mat.mMtx, afStack_84, mat.mMtx);
	unk1B0[0][0] = 1.0f;
	unk1B0[0][1] = 0.0f;
	unk1B0[0][2] = 0.0f;

	unk1B0[1][0] = 0.0f;
	unk1B0[1][1] = 1.0f;
	unk1B0[1][2] = 0.0f;

	unk1B0[2][0] = 0.0f;
	unk1B0[2][1] = 0.0f;
	unk1B0[2][2] = 1.0f;
	MTXConcat(mat.mMtx, unk1B0, unk1B0);
}

bool THamuKuri::isResignationAttack()
{
	if ((unk104.getPoint() - mPosition).length() > unk194) {
		unk194 = unk1F4->mSLGiveUpLength.get();
		return true;
	}

	if (unk1A2) {
		unk1A2 = false;
		return true;
	}

	return false;
}

bool THamuKuri::isHitValid(u32 param_1)
{
	if (isBckAnm(3)) {
		getManager()->requestSerialKill(this);
		return true;
	}

	if (checkLiveFlag(LIVE_FLAG_UNK2))
		return false;

	return true;
}

bool THamuKuri::isCollidMove(THitActor* param_1)
{
	if (param_1->isActorType(0x8000013))
		if (mSpine->getCurrentNerve() == &TNerveHamuKuriBoundFreeze::theNerve())
			param_1->receiveMessage(this, 0);

	if (param_1->isActorType(0x10000002) || param_1->isActorType(0x1000000F)
	    || param_1->isActorType(0x10000013)
	    || param_1->isActorType(0x10000011)) {
		THamuKuri* hamu = (THamuKuri*)param_1;
		if (hamu->mSpine->getCurrentNerve()
		        != &TNerveHamuKuriBoundFreeze::theNerve()
		    && hamu->mSpine->getCurrentNerve()
		           != &TNerveHamuKuriBoundFreeze::theNerve()
		    && hamu->mSpine->getCurrentNerve()
		           != &TNerveHamuKuriWallDie::theNerve()) {
			if (!isHitWallInBound()) {
				unk1A3 = 1;
				forceRoll(mPosition, false);
				return false;
			}
		}
	}

	// TODO: need more checks & HitActor inlines
	if ((param_1->getActorType() & 0xFFFF0000) == 0x40000000) {
		TLiveActor* enemy         = (TLiveActor*)param_1;
		JGeometry::TVec3<f32> vel = enemy->mVelocity;
		if (abs(vel.x) > 2.0f && abs(vel.y) > 2.0f && abs(vel.z) > 2.0f) {
			if (mSpine->getCurrentNerve() != &TNerveHamuKuriJitabata::theNerve()
			    && isAirborne()) {
				mSpine->pushNerve(&TNerveHamuKuriJitabata::theNerve());
			}
		}
	}

	if (!TSmallEnemy::isCollidMove(param_1))
		return false;

	// TODO: inline
	if (!(param_1->getActorType() == getManager()->unk60 ? true : false))
		return true;

	unk1A0 = 1;
	unk1F8 = nullptr;
	return false;
}

void THamuKuri::isAttackToHam() { }

void THamuKuri::isSerialWallDie() { }

void THamuKuri::forceRoll(JGeometry::TVec3<f32> param_1, bool param_2)
{
	JGeometry::TVec3<f32> local_20(mPosition.x - param_1.x, 10.0f,
	                               mPosition.z - param_1.z);

	MsVECNormalize(&local_20, &local_20);

	if (mSpine->getCurrentNerve() == &TNerveWalkerAttack::theNerve()) {
		local_20 = local_20 * unk1F4->mSLWaterAttackCoeff.get();
	} else {
		local_20 = local_20 * unk1F4->mSLWaterCoeff.get();
	}

	onLiveFlag(LIVE_FLAG_AIRBORNE);
	mPosition.y += 10.0f;

	if (param_2) {
		local_20.y = unk1F4->mSLFirstVelocityY.get();
	} else {
		local_20.y = unk1F4->mSLFirstKickVelocityY.get();
		unk1A4     = 1;
	}

	mVelocity = local_20;

	mSpine->pushNerve(&TNerveHamuKuriBoundFreeze::theNerve());
}

THaneHamuKuri::THaneHamuKuri(const char* name)
    : THamuKuri(name)
    , unk20C(0.0f)
    , unk210(0.0f)
    , unk214(0.0f)
    , unk21C(0.0f)
    , unk230(0.0f)
    , unk234(0.0f)
{
}

void THaneHamuKuri::init(TLiveManager* manager)
{
	THamuKuri::init(manager);

	mSpine->initWith(&TNerveWalkerGraphWander::theNerve());

	mActorType = 0x1000000F;
	unk22C     = (THaneHamuKuriSaveLoadParams*)getSaveParam();
	unk188     = 0.0f;
}

void THaneHamuKuri::moveObject() { TWalkerEnemy::moveObject(); }

void THaneHamuKuri::reset()
{
	TWalkerEnemy::reset();
	mHeadHeight = 200.0f;
	unk230 = mGroundHeight = gpMap->checkGround(
	    mPosition.x, mPosition.y + mHeadHeight, mPosition.z, &mGroundPlane);
	unk214 = 0.0f;
	unk210 = 0.0f;
	unk234 = 0.0f;
	unk20C = 0.0f;
	unk21C = 0.0f;
}

void THaneHamuKuri::walkBehavior(int, f32) { }

void THaneHamuKuri::bind()
{
	if (checkLiveFlag(LIVE_FLAG_UNK10))
		return;

	JGeometry::TVec3<f32> local_18 = mPosition;
	local_18.y -= unk234 + unk210;
	local_18 += mLinearVelocity;
	local_18 += mVelocity;

	mVelocity.y -= getGravityY();

	if (mVelocity.y < mVelocityMinY)
		mVelocity.y = mVelocityMinY;

	if (unk214 == 0.0f) {
		mGroundHeight
		    = gpMap->checkGround(local_18.x, local_18.y + mHeadHeight + 200.0f,
		                         local_18.z, &mGroundPlane);
		mGroundHeight += 1.0f;
	}

	if (local_18.y <= mGroundHeight + 0.05f) {
		offLiveFlag(LIVE_FLAG_AIRBORNE);
		mVelocity.set(0.0f, 0.0f, 0.0f);
	} else {
		onLiveFlag(LIVE_FLAG_AIRBORNE);
	}

	mLinearVelocity = local_18 - mPosition;
	mLinearVelocity.y += unk234 + unk210;
}

BOOL THaneHamuKuri::isReachedToGoal() const
{
	JGeometry::TVec3<f32> local_c = unk104.getPoint();
	local_c -= mPosition;
	local_c.y = 0.0f;
	if (MsVECMag2(&local_c) < 100.0f)
		return true;
	else
		return false;
}

void THaneHamuKuri::attackToMario()
{
	if (gpMSound->gateCheck(0x2965))
		MSoundSESystem::MSoundSE::startSoundActor(0x2965, &mPosition, 0,
		                                          nullptr, 0, 4);
	sendAttackMsgToMario();
}

void THaneHamuKuri::setMActorAndKeeper()
{
	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("hanekuri.bmd", 3);
}

void THaneHamuKuri::setWaitAnm() { setBckAnm(3); }

void THaneHamuKuri::setWalkAnm() { setBckAnm(3); }

void THaneHamuKuri::setRunAnm() { setBckAnm(3); }

void THaneHamuKuri::setRollAnm() { setBckAnm(2); }

void THaneHamuKuri::behaveToWater(THitActor*) { forceRoll(*gpMarioPos, true); }

void THaneHamuKuri::setCrashAnm() { }

void THaneHamuKuri::setDeadAnm() { }

bool THaneHamuKuri::isCollidMove(THitActor* param_1)
{
	return TSmallEnemy::isCollidMove(param_1);
}

bool THaneHamuKuri::isHitValid(u32)
{
	if (checkLiveFlag(LIVE_FLAG_UNK2))
		return false;
	else
		return true;
}

void THaneHamuKuri::resetFlyParam() { }

const char** THaneHamuKuri::getBasNameTable() const
{
	return hanekuri_bastable;
}

TDoroHaneKuri::TDoroHaneKuri(const char* name)
    : THaneHamuKuri(name)
{
	unk1AC = 5;
}

void TDoroHaneKuri::init(TLiveManager* param_1)
{
	THaneHamuKuri::init(param_1);

	mSpine->initWith(&TNerveWalkerGraphWander::theNerve());
	mActorType = 0x10000037;
	unk238     = (THaneHamuKuriSaveLoadParams*)getSaveParam();
	unk188     = 0.0f;

	if (mInstanceIndex == 0) {
		((TDoroHaneKuriManager*)mManager)->createHige();

		for (u8 i = 0; i < getModel()->getModelData()->getJointNum(); i++)
			(void)0; // assert?
	}
}

void TDoroHaneKuri::reset()
{
	unk18C = 5;
	THaneHamuKuri::reset();
	unk198 = 0;
	onLiveFlag(LIVE_FLAG_UNK400);
}

void TDoroHaneKuri::attackToMario()
{
	if (!gpMarioOriginal->isWearingCap()) {
		if (SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK))
			if (gpMSound->gateCheck(0x2965))
				MSoundSESystem::MSoundSE::startSoundActor(0x2965, &mPosition, 0,
				                                          nullptr, 0, 4);
	} else {
		if (SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK)) {
			if (gpMSound->gateCheck(0x2965))
				MSoundSESystem::MSoundSE::startSoundActor(0x2965, &mPosition, 0,
				                                          nullptr, 0, 4);

			mSpine->pushNerve(&TNerveDoroHaneRise::theNerve());
			onHaveCap();
			MtxPtr mtx = mMActor->getModel()->getAnmMtx(unk1AC);
			unk200.set(mtx[3][0], mtx[3][1], mtx[3][2]);
			gpMarioParticleManager->emitAndBindToPosPtr(0xCD, &unk200, 0,
			                                            nullptr);
		}
	}
}

void TDoroHaneKuri::setMActorAndKeeper()
{
	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("dorohane.bmd", 3);
}

void TDoroHaneKuri::behaveToWater(THitActor*)
{
	unk21C = 1.0f;
	if (mSpine->getCurrentNerve() != &TNerveDoroHaneHitWater::theNerve())
		mSpine->pushNerve(&TNerveDoroHaneHitWater::theNerve());
}

void TDoroHaneKuri::setBehavior()
{
	if (mSpine->getCurrentNerve() == &TNerveSmallEnemyDie::theNerve()
	    && mHeldObject && mHeldObject->receiveMessage(this, 0x6)) {
		TMapObjBase* held = (TMapObjBase*)mHeldObject;
		held->mHolder     = nullptr;
		held->offLiveFlag(0x2);
		held->mPosition   = mPosition;
		held->mPosition.y = mGroundHeight;
		held->offHitFlag(HIT_FLAG_UNK1);
		held->makeObjDead();
		mHeldObject = nullptr;
	}
}

bool TDoroHaneKuri::isCollidMove(THitActor* param_1)
{
	if ((param_1->getActorType() & 0xffff0000) == 0x40000000) {
		TMapObjBase* mapObj = (TMapObjBase*)param_1;
		if (mapObj->isHideObj(mapObj))
			return false;

		if (mSpine->getCurrentNerve() == &TNerveWalkerAttack::theNerve()) {
			JGeometry::TVec3<f32> vel = mLinearVelocity;
			vel.x *= -5.0f;
			vel.z *= -5.0f;
			mPosition.x += vel.x;
			mPosition.z += vel.z;
		}
	}

	return true;
}

THaneHamuKuri2::THaneHamuKuri2(const char* name)
    : THaneHamuKuri(name)
{
}

void THaneHamuKuri2::reset() { }

void THaneHamuKuri2::sendAttackMsgToMario() { }

void THaneHamuKuri2::walkBehavior(int, f32) { }

BOOL THaneHamuKuri2::isReachedToGoal() const { }

TDangoHamuKuri::TDangoHamuKuri(const char* name)
    : THamuKuri(name)
{
}

void TDangoHamuKuri::init(TLiveManager*) { }

void TDangoHamuKuri::perform(u32, JDrama::TGraphics*) { }

bool TDangoHamuKuri::changeByJuice() { }

void TDangoHamuKuri::moveObject() { }

void TDangoHamuKuri::updateAnmSound() { }

bool TDangoHamuKuri::isCollidMove(THitActor*) { }

void TDangoHamuKuri::attackToMario() { }

MtxPtr TDangoHamuKuri::getTakingMtx() { }

void TDangoHamuKuri::setRunAnm() { }

void TDangoHamuKuri::calcRootMatrix() { }

void TDangoHamuKuri::reset() { }

BOOL TDangoHamuKuri::receiveMessage(THitActor*, u32) { }

bool TDangoHamuKuri::isHitValid(u32) { }

void TDangoHamuKuri::forceKill() { }

void TDangoHamuKuri::nerveInit() { }

void TDangoHamuKuri::behaveToWater(THitActor*) { }

void TDangoHamuKuri::swingBody() { }

TBossDangoHamuKuri::TBossDangoHamuKuri(const char* name)
    : TDangoHamuKuri(name)
{
}

void TBossDangoHamuKuri::init(TLiveManager*) { }

void TBossDangoHamuKuri::perform(u32, JDrama::TGraphics*) { }

void TBossDangoHamuKuri::reset() { }

void TBossDangoHamuKuri::isDead() { }

void TBossDangoHamuKuri::setGenerateAnm() { }

void TBossDangoHamuKuri::moveObject() { }

void TBossDangoHamuKuri::genEventCoin() { }

void TBossDangoHamuKuri::generateBody() { }

void TBossDangoHamuKuri::isNowAttack() { }

void TBossDangoHamuKuri::isNowGenerate() { }

TFireHamuKuri::TFireHamuKuri(const char* name)
    : THamuKuri(name)
{
}

void TFireHamuKuri::init(TLiveManager*) { }

void TFireHamuKuri::behaveToWater(THitActor*) { }

void TFireHamuKuri::reset() { }

void TFireHamuKuri::setMActorAndKeeper() { }

void TFireHamuKuri::moveObject() { }

void TFireHamuKuri::calcRootMatrix() { }

void TFireHamuKuri::walkBehavior(int, f32) { }

bool TFireHamuKuri::isHitValid(u32) { }

void TFireHamuKuri::recoverFire() { }

void TFireHamuKuri::setWalkAnm() { }

void TFireHamuKuri::genFire() { }

void TFireHamuKuri::dieFire() { }

void TFireHamuKuri::sendAttackMsgToMario() { }

void TFireHamuKuri::changeTevColor() { }

TDoroHamuKuri::TDoroHamuKuri(const char* name)
    : THamuKuri(name)
{
}

void TDoroHamuKuri::init(TLiveManager*) { }

void TDoroHamuKuri::reset() { }

void TDoroHamuKuri::kill() { }

void TDoroHamuKuri::setMActorAndKeeper() { }

void TDoroHamuKuri::attackToMario() { }

void TDoroHamuKuri::setBehavior() { }

void TDoroHamuKuri::onHaveCap() { }

bool TDoroHamuKuri::isCollidMove(THitActor*) { }

DEFINE_NERVE(TNerveHamuKuriGoForSearchActor, TLiveActor) { }

DEFINE_NERVE(TNerveHamuKuriBoundFreeze, TLiveActor) { }

DEFINE_NERVE(TNerveHamuKuriWallDie, TLiveActor) { }

DEFINE_NERVE(TNerveHamuKuriLand, TLiveActor) { }

DEFINE_NERVE(TNerveHamuKuriJitabata, TLiveActor) { }

DEFINE_NERVE(TNerveDangoHamuKuriWait, TLiveActor) { }

DEFINE_NERVE(TNerveDangoHamuKuriAttack, TLiveActor) { }

DEFINE_NERVE(TNerveHaneHamuKuriUpWait, TLiveActor) { }

DEFINE_NERVE(TNerveHaneHamuKuriMoveOnGraph, TLiveActor) { }

DEFINE_NERVE(TNerveDoroHamuKuriRobCap, TLiveActor) { }

DEFINE_NERVE(TNerveFireHamuKuriRecover, TLiveActor) { }

DEFINE_NERVE(TNerveDoroHaneRise, TLiveActor) { }

DEFINE_NERVE(TNerveDoroHaneHitWater, TLiveActor) { }

DEFINE_NERVE(TNerveDoroHanePrepareAttack, TLiveActor) { }
