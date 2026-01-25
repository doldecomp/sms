#include "MoveBG/Item.hpp"
#include <Enemy/HamuKuri.hpp>
#include <Enemy/Graph.hpp>
#include <Enemy/Conductor.hpp>
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

void THamuKuriLauncher::stateLaunch()
{
	if (mTicksSpentInCurState == 0) {
		THamuKuri* hamukuri
		    = (THamuKuri*)getProperEnemy("ハムクリマネージャー");
		if (hamukuri) {
			Mtx afStack_6c;
			MsMtxSetRotRPH(afStack_6c, mRotation.x, mRotation.y, mRotation.z);
			JGeometry::TVec3<f32> local_3c(0.0f, 4.0f, 4.0f);
			JGeometry::TVec3<f32> local_30(0.0f, mRotation.y, 0.0f);
			hamukuri->unk1A1 = 0;
			int iVar2        = mRotation.x;
			int iVar1        = iVar2 / 180 + (iVar2 >> 15);
			if ((int)mRotation.x % 180 != 0) {
				hamukuri->unk1A1 = 1;
				float dVar5      = THamuKuri::mLandAnmFrameNum;
				const TBGCheckData* apTStack_74;
				float dVar3
				    = gpMap->checkGround(mPosition.x, mPosition.y + 10.0f,
				                         mPosition.z, &apTStack_74);
				local_3c.set(0.0f,
				             (dVar5 * (dVar5 * hamukuri->getGravityY()) * 0.5f
				              + (dVar3 - mPosition.y))
				                 / dVar5,
				             200.0f / dVar5);
			} else {
				local_3c.set(0.0f, 0.0f, 0.0f);
			}
			PSMTXMultVec(afStack_6c, &local_3c, &local_3c);
			hamukuri->resetSRTV(mPosition, local_30, hamukuri->mScaling,
			                    local_3c);
		}
	}

	if (mMActor->curAnmEndsNext()) {
		resetLaunchTimer();
		changeState(STATE_NORMAL);
	}
}

THamuKuriSaveLoadParams::THamuKuriSaveLoadParams(const char* path)
    : TWalkerEnemyParams(path)
    , PARAM_INIT(mSLWaterCoeff, 6.0f)
    , PARAM_INIT(mSLWaterAttackCoeff, 12.0f)
    , PARAM_INIT(mSLFirstVelocityY, 5.0f)
    , PARAM_INIT(mSLVelocityRate, 0.8f)
    , PARAM_INIT(mSLBoundNum, 3)
    , PARAM_INIT(mSLSearchActorTimer, 500)
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
    , PARAM_INIT(mSLNumArray, 3)
{
	TParams::load(mPrmPath);
}

TFireHamuKuriSaveLoadParams::TFireHamuKuriSaveLoadParams(const char* path)
    : THamuKuriSaveLoadParams(path)
    , PARAM_INIT(mSLRecoverTimer, 500)
{
	TParams::load(mPrmPath);
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

static const char* anmlist[] = {
	"hamukuri_walk",
	"hamukuri_run",
	// TODO: this shouldn't be here but rodata ordering looks like it should?!
	// "default.bmd",
	"hanekuri_wait",
};

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
		s32 searchActorTime             = params->mSLSearchActorTimer.get();
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

void TDangoHamuKuriManager::load(JSUMemoryInputStream& stream)
{
	unk38 = new THamuKuriSaveLoadParams("/enemy/dangohamukuri.prm");
	TSmallEnemyManager::load(stream);
}

TSpineEnemy* TDangoHamuKuriManager::createEnemyInstance()
{
	return new TDangoHamuKuri;
}

void TDangoHamuKuriManager::createModelDataArray(
    const TModelDataLoadEntry* param_1)
{
	THamuKuriManager* manager
	    = (THamuKuriManager*)gpConductor->getManagerByName(
	        "ハムクリマネージャー");

	if (manager) {
		unk28            = manager->unk28;
		mModelDataKeeper = manager->getModelDataKeeper();
	} else {
		TObjManager::createModelDataArray(param_1);
	}
}

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
	mSearchActSw = false;
}

void TBossDangoHamuKuriManager::load(JSUMemoryInputStream& stream)
{
	unk38
	    = new TBossDangoHamuKuriSaveLoadParams("/enemy/bossdangohamukuri.prm");
	TSmallEnemyManager::load(stream);
}

TSpineEnemy* TBossDangoHamuKuriManager::createEnemyInstance()
{
	return new TBossDangoHamuKuri;
}

static const GXColorS10 mFireHamNoseColorStart = { 0x1F4, 0xC8, 0x78, 0xFF };
static const GXColorS10 mFireHamNoseColorEnd = { 0xFFEC, 0xFFA6, 0xFF74, 0xFF };
static const GXColorS10 mFireHamOtherColorStart
    = { 0x1F4, 0xFFD8, 0xFFBA, 0xFF };
static const GXColorS10 mFireHamOtherColorEnd = { 0x50, 0x5, 0xFFCE, 0xFF };

static GXColorS10 mFireHamNoseColorDiff = {
	mFireHamNoseColorEnd.r - mFireHamNoseColorStart.r,
	mFireHamNoseColorEnd.g - mFireHamNoseColorStart.g,
	mFireHamNoseColorEnd.b - mFireHamNoseColorStart.b,
	mFireHamNoseColorEnd.a - mFireHamNoseColorStart.a,
};
static GXColorS10 mFireHamOtherColorDiff = {
	mFireHamOtherColorEnd.r - mFireHamOtherColorStart.r,
	mFireHamOtherColorEnd.g - mFireHamOtherColorStart.g,
	mFireHamOtherColorEnd.b - mFireHamOtherColorStart.b,
	mFireHamOtherColorEnd.a - mFireHamOtherColorStart.a,
};

TFireHamuKuriManager::TFireHamuKuriManager(const char* name)
    : THamuKuriManager(name)
{
}

void TFireHamuKuriManager::load(JSUMemoryInputStream& stream)
{
	unk38 = new TFireHamuKuriSaveLoadParams("/enemy/firehamukuri.prm");
	TSmallEnemyManager::load(stream);
}

TSpineEnemy* TFireHamuKuriManager::createEnemyInstance()
{
	return new TFireHamuKuri;
}

void TFireHamuKuriManager::initSetEnemies()
{
	for (int i = 0; i < mObjNum; ++i) {
		TFireHamuKuri* hamu = (TFireHamuKuri*)unk18[i];
		hamu->unk21C        = mFireHamNoseColorStart;
		hamu->unk224        = mFireHamOtherColorStart;

		static const GXColorS10 tevColorData[] = {
			{ 200, 200, 200, 161 },
			{ 0, 90, 0, 255 },
			{ 200, 160, 130, 255 },
		};

		u32 matBodyBottom1Idx = ((TFireHamuKuri*)unk18[0])
		                            ->getModel()
		                            ->getModelData()
		                            ->getMaterialName()
		                            ->getIndex("_mat_body_bottom1");
		SMS_InitPacket_ThreeTevColor(
		    hamu->getMActor()->getModel(), matBodyBottom1Idx, GX_TEVREG0,
		    &tevColorData[0], GX_TEVREG1, &tevColorData[1], GX_TEVREG2,
		    &tevColorData[2]);

		u32 matNose1Idx = hamu->getMActor()
		                      ->getModel()
		                      ->getModelData()
		                      ->getMaterialName()
		                      ->getIndex("_mat_nose1");
		SMS_InitPacket_OneTevColor(hamu->getMActor()->getModel(), matNose1Idx,
		                           GX_TEVREG0, &hamu->unk21C);
		u32 matBodyTop1Idx = hamu->getMActor()
		                         ->getModel()
		                         ->getModelData()
		                         ->getMaterialName()
		                         ->getIndex("_mat_body_top1");
		SMS_InitPacket_OneTevColorAndOneTevKColor(hamu->getMActor()->getModel(),
		                                          matBodyTop1Idx, GX_TEVREG0,
		                                          &hamu->unk224, &hamu->unk1FC);

		hamu->unk1FC.a = 0;

		u32 matLfoot1Idx = hamu->getMActor()
		                       ->getModel()
		                       ->getModelData()
		                       ->getMaterialName()
		                       ->getIndex("_mat_Lfoot1");
		SMS_InitPacket_OneTevColor(hamu->getMActor()->getModel(), matLfoot1Idx,
		                           GX_TEVREG0, &hamu->unk224);

		u32 matRfoot1Idx = hamu->getMActor()
		                       ->getModel()
		                       ->getModelData()
		                       ->getMaterialName()
		                       ->getIndex("_mat_Rfoot1");
		SMS_InitPacket_OneTevColor(hamu->getMActor()->getModel(), matRfoot1Idx,
		                           GX_TEVREG0, &hamu->unk224);
	}
}

void TFireHamuKuriManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "default.bmd", 0x10240000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

void TDoroHige::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (!unk1C->isUnk198()
	    || unk1C->checkLiveFlag(LIVE_FLAG_CLIPPED_OUT | LIVE_FLAG_UNK2
	                            | LIVE_FLAG_DEAD))
		return;

	TSharedParts::perform(param_1, param_2);
}

TDoroHamuKuriManager::TDoroHamuKuriManager(const char* name)
    : THamuKuriManager(name)
    , unk74(nullptr)
{
}

void TDoroHamuKuriManager::load(JSUMemoryInputStream& stream)
{
	unk38 = new THamuKuriSaveLoadParams("/enemy/dorohamukuri.prm");
	TSmallEnemyManager::load(stream);
}

void TDoroHamuKuriManager::loadAfter()
{
	THamuKuriManager::loadAfter();
	TMapObjBaseManager::newAndRegisterObj("mario_cap");
}

TSpineEnemy* TDoroHamuKuriManager::createEnemyInstance()
{
	return new TDoroHamuKuri;
}

void TDoroHamuKuriManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "dorokuri_model.bmd", 0x10240000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

void TDoroHamuKuriManager::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	THamuKuriManager::perform(param_1, param_2);

	if (unk74)
		unk74->perform(param_1, param_2);
}

void TDoroHamuKuriManager::createHige()
{
	void* rawModelData = JKRGetResource("/scene/dorokuri/dorokuriHige.bmd");
	SDLModelData* modelData = new SDLModelData(
	    J3DModelLoaderDataBase::load(rawModelData, 0x10210000));
	unk74 = new TDoroHige((TLiveActor*)unk18[0], 5, modelData);
}

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

bool THamuKuri::isGiveUpSearchActor()
{
	TLiveActor* casted = (TLiveActor*)unk1F8;

	if (unk128 > 100
	    || abs(mGroundHeight - casted->getGroundHeight()) > 100.0f) {
		unk1A0 = true;
		unk1F8 = nullptr;
		unk19C = 0;
		return true;
	} else {
		return false;
	}
}

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
		if (getManager()->unk70 == nullptr) {
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

			// TODO: this is an inline
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

void THaneHamuKuri::setCrashAnm() { setBckAnm(0); }

void THaneHamuKuri::setDeadAnm()
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
	setBckAnm(1);
}

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

void THaneHamuKuri2::reset()
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

	onLiveFlag(LIVE_FLAG_UNK10);
	unk230 = mPosition.y;
	mSpine->initWith(&TNerveHaneHamuKuriMoveOnGraph::theNerve());
}

void THaneHamuKuri2::sendAttackMsgToMario()
{
	TSmallEnemy::sendAttackMsgToMario();
}

void THaneHamuKuri2::walkBehavior(int param_1, f32 param_2)
{
	f32 flyBaseFrequency = unk22C->mSLFlyBaseFrequency.get();
	f32 flyBaseAmplitude = unk22C->mSLFlyBaseAmplitude.get();

	unk20C += 1.0f;

	if (unk20C > flyBaseFrequency)
		unk20C = 0.0f;

	f32 flyBaseHeight = unk22C->mSLFlyBaseHeight.get();
	if (unk234 < flyBaseHeight)
		unk234 += 1.0f;

	if (unk234 > flyBaseHeight)
		unk234 -= 1.0f;

	unk210      = JMASin(unk20C * 360.0f / flyBaseFrequency) * flyBaseAmplitude;
	mPosition.y = unk210 + unk230 + unk234;
	mTurnSpeed
	    = ((THaneHamuKuriSaveLoadParams*)getSaveParam())->mSLTurnSpeedLow.get();
	mMarchSpeed = ((THaneHamuKuriSaveLoadParams*)getSaveParam())
	                  ->mSLMarchSpeedLow.get();

	if (!isBckAnm(4))
		TWalkerEnemy::walkBehavior(param_1, param_2);
}

BOOL THaneHamuKuri2::isReachedToGoal() const
{
	JGeometry::TVec3<f32> local_c = unk104.getPoint();
	local_c -= mPosition;
	local_c.y = 0.0f;
	if (MsVECMag2(&local_c) < 20.0f)
		return true;
	else
		return false;
}

TDangoHamuKuri::TDangoHamuKuri(const char* name)
    : THamuKuri(name)
    , unk20C(0.0f)
    , unk210(0.0f)
    , unk214(0.0f)
    , unk218(0.0f)
    , mNext(0)
    , mPrev(nullptr)
    , unk230(0)
    , mBoss(nullptr)
{
	unk21C = 0.0f;
	unk220 = 0.0f;
	unk224 = 0.0f;
}

void TDangoHamuKuri::init(TLiveManager* param_1)
{
	THamuKuri::init(param_1);
	mSpine->initWith(&TNerveDangoHamuKuriWait::theNerve());
	mActorType = 0x10000010;
}

void TDangoHamuKuri::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (mBoss
	    && !(mBoss->mSpine->getCurrentNerve()
	                 == &TNerveWalkerGenerate::theNerve()
	             ? true
	             : false))
		TSmallEnemy::perform(param_1, param_2);
}

bool TDangoHamuKuri::changeByJuice()
{
	if (mPrev == nullptr)
		return TSmallEnemy::changeByJuice();
	else
		return false;
}

void TDangoHamuKuri::moveObject()
{
	THamuKuri::moveObject();
	swingBody();
	if (mBoss && !mBoss->checkLiveFlag(LIVE_FLAG_CLIPPED_OUT))
		offLiveFlag(LIVE_FLAG_CLIPPED_OUT);
}

void TDangoHamuKuri::updateAnmSound()
{
	TSmallEnemy::updateAnmSound();
	if (mBoss)
		offLiveFlag(LIVE_FLAG_CLIPPED_OUT);
}

bool TDangoHamuKuri::isCollidMove(THitActor* param_1)
{
	return TSmallEnemy::isCollidMove(param_1);
}

void TDangoHamuKuri::attackToMario()
{
	if (mPrev != nullptr || mPosition.y + 20.0f > SMS_GetMarioPos().y) {
		if (mSpine->getCurrentNerve() == &TNerveHamuKuriJitabata::theNerve()) {
			if (mPosition.y + 10.0f > SMS_GetMarioPos().y) {
				forceRoll(SMS_GetMarioPos(), false);
				SMSRumbleMgr->start(0x15, 5, (float*)nullptr);
			}
		} else {
			TWalkerEnemy::attackToMario();
		}
	} else if (mBoss
	           && (mBoss->mSpine->getCurrentNerve()
	                       == &TNerveWalkerAttack::theNerve()
	                   ? true
	                   : false)) {
		sendAttackMsgToMario();
	}
}

MtxPtr TDangoHamuKuri::getTakingMtx()
{
	mMActor->calc();
	MtxPtr mtx = mMActor->getModel()->getAnmMtx(unk1AC);
	f32 fVar2  = 0.0f;
	if (mBoss == this)
		fVar2 = 40.0f;

	TPosition3f pos;
	pos.translation(mtx[0][3], mtx[1][3] + fVar2, mtx[2][3]);
	Mtx afSstack_84;
	MsMtxSetRotRPH(afSstack_84, 0.0f, mBoss->mRotation.y, 0.0f);
	MtxPtr result = unk1B0;
	MTXConcat(pos.mMtx, afSstack_84, result);
	return result;
}

void TDangoHamuKuri::setRunAnm()
{
	if (mNext == nullptr)
		setBckAnm(8);
	else
		setBckAnm(14);
}

void TDangoHamuKuri::calcRootMatrix()
{
	getModel()->setBaseScale(mPosition);
	if (mHolder && mHolder->mHeldObject == this) {
		MtxPtr takingMtx = getTakingMtx();
		if (takingMtx) {
			if (unk230) {
				unk210 += 40.0f;
				if (unk210 > 360.0f) {
					// TODO: should be a rand interval
					unk210 = -MsRandF(10.0f, 20.0f);
					unk230 = 0;
				}
				TDangoHamuKuri* holder = (TDangoHamuKuri*)mHolder;
				if (holder->unk230)
					unk210 = -holder->unk210;
				takingMtx[3][0] += unk21C;
				takingMtx[3][1] += unk220;
				takingMtx[3][2] += unk224;

				getModel()->setBaseScale(mScaling);
				Mtx afStack_68;
				MsMtxSetRotRPH(afStack_68, 0.0f, unk210, unk214);
				MTXConcat(takingMtx, afStack_68, takingMtx);
				getModel()->setBaseTRMtx(takingMtx);

				mPosition.set(takingMtx[3][0], takingMtx[3][1],
				              takingMtx[3][2]);
				return;
			}
		}
	}

	TSpineEnemy::calcRootMatrix();
}

void TDangoHamuKuri::reset()
{
	THamuKuri::reset();
	mPrev = nullptr;
	mNext = nullptr;
	mBoss = nullptr;
	// TODO: rand interval
	unk20C = MsRandF(0.0f, 1.0f);
	mMActor->calc();
}

BOOL TDangoHamuKuri::receiveMessage(THitActor* param_1, u32 param_2)
{
	if (param_2 == 4 && mHolder == nullptr && mBoss != this) {
		onHitFlag(HIT_FLAG_UNK1);
		mHolder = (TLiveActor*)param_1;
		behaveToTaken(param_1);
		return true;
	}

	if ((param_2 == 6 || param_2 == 7) && mHolder == param_1) {
		mHolder = nullptr;
		behaveToRelease();
		offHitFlag(HIT_FLAG_UNK1);
		return true;
	}

	if (param_2 == 0 || param_2 == 1 || param_2 == 3 || param_2 == 11) {
		if (isHitValid(param_2)) {
			unk184 = 0;
			kill();
		}
		return true;
	}

	if (param_2 == 13) {
		mHitPoints = 0;
		onLiveFlag(LIVE_FLAG_DEAD);
		onHitFlag(HIT_FLAG_UNK1);
	}

	if (param_2 == 15) {
		gpMarioParticleManager->emit(0xE7, &mPosition, 0, nullptr);
		gpMSound->startSoundSet(0x6802, &mPosition, 0.0f, 0.0f, 0, 0, 4);
		if (mSprayedByWaterCooldown == 0) {
			mSprayedByWaterCooldown = 1;
			if (!changeByJuice()) {
				decHpByWater(param_1);
				behaveToWater(param_1);
			}
		}

		return true;
	}

	return false;
}

bool TDangoHamuKuri::isHitValid(u32)
{
	if (mPrev != nullptr || mNext != nullptr)
		return false;
	else
		return true;
}

void TDangoHamuKuri::forceKill()
{
	if (mPrev == nullptr)
		TSmallEnemy::forceKill();

	if (mBoss == nullptr)
		return;

	if (mBoss->isDead()
	    && mSpine->getCurrentNerve() != &TNerveSmallEnemyDie::theNerve()) {
		mSpine->reset();
		mSpine->setNext(&TNerveSmallEnemyDie::theNerve());
		mSpine->pushAfterCurrent(mSpine->getDefault());
		onLiveFlag(LIVE_FLAG_UNK20000);
	}
}

void TDangoHamuKuri::nerveInit() { }

void TDangoHamuKuri::behaveToWater(THitActor* param_1)
{
	if (!mNext) {
		if (!mPrev) {
			THamuKuri::behaveToWater(param_1);
		} else if (mSprayedByWaterCooldown <= 1 && receiveMessage(mPrev, 6)) {
			mHolder            = nullptr;
			mPrev->mHeldObject = nullptr;
			mPrev->mNext       = nullptr;

			mPrev->mSprayedByWaterCooldown = 1;

			mPrev = nullptr;
			mNext = nullptr;
			mSpine->setNext(&TNerveWalkerGraphWander::theNerve());
			unk210 = 0.0f;
			unk214 = 0.0f;

			JGeometry::TVec3<f32> local_20 = mPosition - SMS_GetMarioPos();
			MsVECNormalize(&local_20, &local_20);
			local_20.x *= 5.0f;
			local_20.y = 1.0f;
			local_20.z *= 5.0f;
			mVelocity = local_20;
			mPosition.y += 20.0f;
			onLiveFlag(LIVE_FLAG_AIRBORNE);
			unk230 = 0;
		}
	} else if (mPrev == nullptr) {
		if (!isAirborne()) {
			JGeometry::TVec3<f32> local_2c(0.0f, 2.0f, 0.0f);
			mPosition.y += 10.0f;
			onLiveFlag(LIVE_FLAG_AIRBORNE);
			mVelocity = local_2c;
			mSpine->pushNerve(&TNerveSmallEnemyHitWaterJump::theNerve());
		}
	} else if (!mPrev->unk230) {
		unk230 = 1;
		unk210 = 1.0f;
		if (gpMSound->gateCheck(0x295E))
			MSoundSESystem::MSoundSE::startSoundActor(0x295E, &mPosition, 0,
			                                          nullptr, 0, 4);
	}
}

void TDangoHamuKuri::swingBody()
{
	if (unk20C > 1.0f)
		unk20C -= 1.0f;

	unk20C += 0.01f;
	f32 fVar1 = 10.0f;

	if (mBoss->mSpine->getCurrentNerve() == &TNerveWalkerAttack::theNerve()
	        ? true
	        : false) {
		if (mAttackSw) {
			if (mPrev != nullptr) {
				if (mPrev == mBoss) {
					mPosition = mBoss->mPosition;
					unk210 += 10.0f;
				}

				if (mPrev->mPrev == mBoss) {
					unk218 += 1000.0f;
					unk214 = JMASSin(unk218) * 30.0f + 90.0f;
				}
			} else if (mPrev != nullptr && mPrev->mPrev == mBoss) {
				unk214 = 0.0f;
			}
		}
		fVar1 = 16.0f;
	}

	unk21C = fVar1 * JMACos(unk20C * 360.0f);
	unk224 = fVar1 * JMASin(unk20C * 360.0f);
}

TBossDangoHamuKuri::TBossDangoHamuKuri(const char* name)
    : TDangoHamuKuri(name)
{
}

void TBossDangoHamuKuri::init(TLiveManager* param_1)
{
	TDangoHamuKuri::init(param_1);
	unk23C = (TBossDangoHamuKuriSaveLoadParams*)getSaveParam();
	mSpine->initWith(&TNerveWalkerGenerate::theNerve());
	mBoss = this;
}

void TBossDangoHamuKuri::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	TSmallEnemy::perform(param_1, param_2);
}

void TBossDangoHamuKuri::reset()
{
	TDangoHamuKuri::reset();
	mBoss  = this;
	unk238 = 0;
}

bool TBossDangoHamuKuri::isDead()
{
	if (mSpine->getCurrentNerve() == &TNerveSmallEnemyDie::theNerve()
	    || mSpine->getCurrentNerve() == &TNerveHamuKuriWallDie::theNerve())
		return true;
	else
		return false;
}

void TBossDangoHamuKuri::setGenerateAnm()
{
	setBckAnm(9);
	// -----O-O-O-O-O-
	// Dangos ^
	int dangoNum = unk23C->mSLNumArray.get();
	for (int i = 0; i < dangoNum; ++i)
		generateBody();
}

void TBossDangoHamuKuri::moveObject() { TDangoHamuKuri::moveObject(); }

void TBossDangoHamuKuri::genEventCoin()
{
	if (!mCoin)
		return;

	TMapObjBase* coin;

	if (mCoin->isActorType(0x2000000E)) {
		coin = gpItemManager->makeObjAppear(0x2000000E);
	} else {
		coin = mCoin;
		coin->appear();
	}

	if (coin) {
		coin->mPosition = mPosition;
		coin->mVelocity.set(0.0f, 20.0f, 0.0f);
		coin->offLiveFlag(LIVE_FLAG_UNK10);
		unk18C -= 1;
	}
}

void TBossDangoHamuKuri::generateBody()
{
	s32 numArray = unk23C->mSLNumArray.get();
	if (unk238 >= numArray)
		return;

	TDangoHamuKuri* newHamu = (TDangoHamuKuri*)gpConductor->makeOneEnemyAppear(
	    mPosition, "だんごハムクリマネージャー", 0);
	TDangoHamuKuri* currHamu = this;

	if (!newHamu)
		return;

	newHamu->reset();
	newHamu->mSpine->reset();
	newHamu->mSpine->setNext(&TNerveDangoHamuKuriWait::theNerve());
	newHamu->mPosition.set(0.0f, 0.0f, 0.0f);
	newHamu->unk124->unk0 = unk124->unk0;
	++unk238;

	for (int i = 0; i < numArray; ++i) {
		if (!currHamu->mNext) {
			currHamu->mNext = newHamu;
			newHamu->mPrev  = currHamu;
			if (newHamu->receiveMessage(currHamu, 4))
				currHamu->mHeldObject = newHamu;
			newHamu->mBoss = this;
			break;
		}
		currHamu = currHamu->mNext;
	}

	newHamu->offHitFlag(HIT_FLAG_UNK1);
}

void TBossDangoHamuKuri::isNowAttack() { }

void TBossDangoHamuKuri::isNowGenerate() { }

TFireHamuKuri::TFireHamuKuri(const char* name)
    : THamuKuri(name)
    , unk210(0)
    , unk214(0)
    , unk218(0)
{
}

void TFireHamuKuri::init(TLiveManager* param_1)
{
	THamuKuri::init(param_1);
	mSpine->initWith(&TNerveWalkerGenerate::theNerve());
	mActorType = 0x10000011;
	unk20C     = (TFireHamuKuriSaveLoadParams*)getSaveParam();
}

void TFireHamuKuri::behaveToWater(THitActor* param_1)
{
	if (unk210) {
		JGeometry::TVec3<f32> local_20(mPosition.x - gpMarioPos->x, 0.0f,
		                               mPosition.z - gpMarioPos->z);

		if (local_20.x == 0.0f && local_20.y == 0.0f && local_20.z == 0.0f)
			local_20.x += 1.0f;

		MsVECNormalize(&local_20, &local_20);
		local_20.scale(8.0f);
		mVelocity = local_20;
		onLiveFlag(LIVE_FLAG_AIRBORNE);
		mPosition.y += 5.0f;
		if (mHitPoints == 0) {
			unk210 = 0;
			unk1A2 = 1;
			unk150 |= 0x2;
			unk150 &= ~0x1;
			unk214 = 1;
			if (gpMSound->gateCheck(0x2903))
				MSoundSESystem::MSoundSE::startSoundActor(0x2903, &mPosition, 0,
				                                          nullptr, 0, 4);
			if (JPABaseEmitter* emitter
			    = gpMarioParticleManager->emitAndBindToMtxPtr(
			        0x8B, mMActor->getModel()->getAnmMtx(unk1AC), 0, nullptr)) {
				emitter->setScale(mScaling);
			}
		}
		mSprayedByWaterCooldown = 20;
		return;
	}

	THamuKuri::behaveToWater(param_1);
}

void TFireHamuKuri::reset()
{
	THamuKuri::reset();
	mHitPoints = getSaveParam() ? getSaveParam()->mSLHitPointMax.get() : 1;
	unk150 &= ~0x2;
	unk150 |= 0x1;
	unk214 = 0;
	unk210 = 0;
}

void TFireHamuKuri::setMActorAndKeeper()
{
	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("default.bmd", 3);
	ResTIMG* img
	    = (ResTIMG*)JKRGetResource("/scene/map/pollution/H_ma_rak.bti");
	if (img)
		SMS_ChangeTextureAll(mMActor->getModel()->getModelData(),
		                     "H_ma_rak_dummy", *img);
}

void TFireHamuKuri::moveObject()
{
	THamuKuri::moveObject();
	changeTevColor();
}

void TFireHamuKuri::calcRootMatrix()
{
	TSpineEnemy::calcRootMatrix();
	if (unk210 && !checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)) {
		if (JPABaseEmitter* emitter
		    = gpMarioParticleManager->emitAndBindToMtxPtr(
		        0x1ED, mMActor->getModel()->getAnmMtx(unk1AC), 3, this)) {
			emitter->setScale(mScaling);
		}
		if (JPABaseEmitter* emitter
		    = gpMarioParticleManager->emitAndBindToMtxPtr(
		        0x135, mMActor->getModel()->getAnmMtx(unk1AC), 1, this)) {
			emitter->setScale(mScaling);
		}
		if (JPABaseEmitter* emitter
		    = gpMarioParticleManager->emitAndBindToMtxPtr(
		        0x136, mMActor->getModel()->getAnmMtx(unk1AC), 1, this)) {
			emitter->setScale(mScaling);
		}
		if (JPABaseEmitter* emitter
		    = gpMarioParticleManager->emitAndBindToMtxPtr(
		        0x137, mMActor->getModel()->getAnmMtx(unk1AC), 1, this)) {
			emitter->setScale(mScaling);
		}
	}
}

void TFireHamuKuri::walkBehavior(int param_1, f32 param_2)
{
	if (!isBckAnm(4))
		TWalkerEnemy::walkBehavior(param_1, param_2);

	s32 recoverTimer = unk20C->mSLRecoverTimer.get();
	if (unk214 > 0)
		unk214 += 1;

	if (unk214 == 0 && unk210 == 0)
		unk210 = 1;

	if (unk214 > recoverTimer) {
		mSpine->reset();
		mSpine->setNext(&TNerveWalkerGraphWander::theNerve());
		mSpine->pushNerve(&TNerveFireHamuKuriRecover::theNerve());
		setBckAnm(13);
	}
}

bool TFireHamuKuri::isHitValid(u32 param_1)
{
	if (param_1 == 11)
		return true;

	if (unk210)
		return false;

	if (isBckAnm(3)) {
		getManager()->requestSerialKill(this);
		return true;
	}

	if (checkLiveFlag(LIVE_FLAG_UNK2))
		return false;

	return true;
}

// TODO: this is the wrong inline, size doesn't match at all!
bool TFireHamuKuri::recoverFire()
{
	bool result = false;
	if (!unk210) {
		if (unk218 < 30) {
			unk218 += 1;
			result = true;
		}
	} else {
		if (gpMSound->gateCheck(0x20C3))
			MSoundSESystem::MSoundSE::startSoundActor(0x20C3, &mPosition, 0,
			                                          nullptr, 0, 4);
		if (unk218 > 0) {
			unk218 -= 1;
			result = true;
		}
	}
	return result;
}

void TFireHamuKuri::setWalkAnm() { setBckAnm(14); }

void TFireHamuKuri::genFire() { }

void TFireHamuKuri::dieFire() { }

void TFireHamuKuri::sendAttackMsgToMario()
{
	if (unk210)
		SMS_SendMessageToMario(this, 10);
	else
		SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
}

void TFireHamuKuri::changeTevColor()
{
	if (recoverFire()) {
		unk21C.r = (mFireHamNoseColorDiff.r * unk218) / 30
		           + mFireHamNoseColorStart.r;
		unk21C.g = (mFireHamNoseColorDiff.g * unk218) / 30
		           + mFireHamNoseColorStart.g;
		unk21C.b = (mFireHamNoseColorDiff.b * unk218) / 30
		           + mFireHamNoseColorStart.b;

		unk224.r = (mFireHamOtherColorDiff.r * unk218) / 30
		           + mFireHamOtherColorStart.r;
		unk224.g = (mFireHamOtherColorDiff.g * unk218) / 30
		           + mFireHamOtherColorStart.g;
		unk224.b = (mFireHamOtherColorDiff.b * unk218) / 30
		           + mFireHamOtherColorStart.b;
	}
}

TDoroHamuKuri::TDoroHamuKuri(const char* name)
    : THamuKuri(name)
{
}

void TDoroHamuKuri::init(TLiveManager* param_1)
{
	THamuKuri::init(param_1);
	mActorType = 0x10000013;
	mSpine->initWith(&TNerveWalkerGenerate::theNerve());

	if (mInstanceIndex == 0) {
		((TDoroHamuKuriManager*)mManager)->createHige();
	}
}

void TDoroHamuKuri::reset()
{
	THamuKuri::reset();
	mSpine->initWith(&TNerveWalkerGenerate::theNerve());
}

void TDoroHamuKuri::kill() { THamuKuri::kill(); }

void TDoroHamuKuri::setMActorAndKeeper()
{
	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("dorokuri_model.bmd", 3);
}

void TDoroHamuKuri::attackToMario()
{
	if (mSpine->getCurrentNerve() == &TNerveHamuKuriJitabata::theNerve()) {
		if (mPosition.y + 10.0f > gpMarioPos->y) {
			forceRoll(SMS_GetMarioPos(), false);
			SMSRumbleMgr->start(0x15, 5, (f32*)nullptr);
		}
	} else {
		THamuKuri::selectCapHolder();
	}
}

void TDoroHamuKuri::setBehavior()
{
	TDoroHamuKuriManager* man = (TDoroHamuKuriManager*)getManager();
	if (!unk198 && man->unk70) {
		if (mSpine->getCurrentNerve() == &TNerveWalkerGraphWander::theNerve()
		    && !isAirborne()) {
			mSpine->pushNerve(&TNerveDoroHamuKuriRobCap::theNerve());
			unk1F8 = man->unk70;
		}
	} else {
		THamuKuri::setBehavior();
	}
}

void TDoroHamuKuri::onHaveCap()
{
	unk198                    = 1;
	TDoroHamuKuriManager* man = (TDoroHamuKuriManager*)getManager();
	man->unk70                = this;
	man->unk74->setOwner(this);
}

bool TDoroHamuKuri::isCollidMove(THitActor* param_1)
{
	if (unk198)
		return false;

	if (param_1->isActorType(0x10000013)) {
		TDoroHamuKuri* other = (TDoroHamuKuri*)param_1;
		if (other->isUnk198() && !other->isAirborne()) {
			other->mVelocity = JGeometry::TVec3<f32>(0.0f, 3.0f, 0.0f);
			other->mPosition.y += 2.0f;
			other->onLiveFlag(LIVE_FLAG_AIRBORNE);

			if (!isAirborne()) {
				JGeometry::TVec3<f32> local_4c = mPosition;
				mVelocity
				    = calcVelocityToJumpToY(local_4c, 6.0f, getGravityY());
				mPosition.y += 2.0f;
				onLiveFlag(LIVE_FLAG_AIRBORNE);
			}

			if (!unk198 && isAirborne()
			    && mPosition.y > param_1->mPosition.y + 10.0f) {
				TTakeActor* pTVar1 = other->mHeldObject;
				if (pTVar1 == nullptr) {
					other->unk198      = 0;
					other->mHeldObject = nullptr;
					return true;
				}

				if (receiveMessage(param_1, 6)) {
					pTVar1->mPosition = param_1->mPosition;
					if (pTVar1->receiveMessage(this, 4)) {
						other->unk198      = 0;
						other->mHeldObject = nullptr;
						onHaveCap();
						mHeldObject = pTVar1;

						// TODO: this is an inline
						int uVar11 = unk124->getCurrentIndex();

						int count  = MsRandF(2, 3);
						int uVar10 = -1;
						for (int i = 0; i < count; ++i) {
							int next = unk124->unk0->getRandomNextIndex(
							    uVar11, uVar10, 0xffffffff);
							uVar10 = uVar11;
							uVar11 = next;
						}

						if (uVar11 < 0)
							uVar11 = 0;

						JGeometry::TVec3<f32> VStack_60;
						unk124->getGraph()->getGraphNode(uVar11).getPoint(
						    &VStack_60);

						JGeometry::TVec3<f32> local_6c = calcVelocityToJumpToY(
						    VStack_60, mCapSpeed, getGravityY());
						onLiveFlag(LIVE_FLAG_AIRBORNE);
						mVelocity = local_6c;
						return false;
					}
				}
			}
		}

		return true;
	}

	return THamuKuri::isCollidMove(param_1);
}

DEFINE_NERVE(TNerveHamuKuriGoForSearchActor, TLiveActor)
{
	THamuKuri* self = (THamuKuri*)spine->getBody();
	if (spine->getTime() == 0)
		self->setRunAnm();

	if (!self->isAirborne()) {
		if ((self->unk104.getPoint() - self->mPosition).length() < 200.0f)
			self->jumpToSearchActor();
	}

	if (self->unk1A0 || self->isGiveUpSearchActor()) {
		spine->reset();
		spine->setNext(&TNerveWalkerGraphWander::theNerve());
		spine->pushAfterCurrent(&TNerveWalkerGraphWander::theNerve());
		return true;
	}

	self->walkBehavior(0, 2.5f);
	return false;
}

DEFINE_NERVE(TNerveHamuKuriBoundFreeze, TLiveActor)
{
	THamuKuri* self = (THamuKuri*)spine->getBody();

	if (spine->getTime() == 0) {
		self->setRollAnm();
		JGeometry::TVec3<f32> thing = self->mVelocity;
		self->unk1E4.x              = thing.x;
		self->unk1E4.y              = thing.y;
		self->unk1E4.z              = thing.z;
		self->setGoalPathMario();
		self->unk1E0 = 1;
	}

	if (self->isHitWallInBound()) {
		spine->pushAfterCurrent(&TNerveHamuKuriWallDie::theNerve());
		return true;
	}

	if (self->unk1A4 == 0 && !self->isAirborne()) {
		THamuKuriSaveLoadParams* params
		    = (THamuKuriSaveLoadParams*)self->getSaveParam();
		if (self->unk1E0 >= params->mSLBoundNum.get()) {
			self->mRotation.x = 0;
			spine->reset();
			spine->setNext(&TNerveWalkerGraphWander::theNerve());
			spine->pushAfterCurrent(&TNerveWalkerGraphWander::theNerve());
			return true;
		}

		self->unk1E0 += 1;
		self->mPosition.y += 5.0f;
		JGeometry::TVec3<f32> thing = self->unk1E4;
		thing *= params->mSLVelocityRate.get();
		self->unk1E4.x  = thing.x;
		self->unk1E4.y  = thing.y;
		self->unk1E4.z  = thing.z;
		self->mVelocity = thing;
	}

	if (!self->isBckAnm(4))
		self->TWalkerEnemy::walkBehavior(3, 1000.0f);

	return false;
}

DEFINE_NERVE(TNerveHamuKuriWallDie, TLiveActor)
{
	THamuKuri* self = (THamuKuri*)spine->getBody();

	if (spine->getTime() == 0) {
		self->setCrashAnm();
		JGeometry::TVec3<f32> local_34;
		if (self->checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)) {
			local_34 = self->getPosition();
		} else {
			MtxPtr mtx = self->getMActor()->getModel()->getAnmMtx(1);
			local_34.x = mtx[0][3];
			local_34.y = mtx[1][3];
			local_34.z = mtx[2][3];
		}

		if (JPABaseEmitter* emitter = gpMarioParticleManager->emitWithRotate(
		        0xE2, &local_34, 0, DEG2SHORTANGLE(self->mRotation.y), 0, 0,
		        nullptr)) {
			emitter->setScale(self->mScaling);
		}

		if (JPABaseEmitter* emitter = gpMarioParticleManager->emitWithRotate(
		        0xE3, &self->getPosition(), 0,
		        DEG2SHORTANGLE(self->getRotation().y), 0, 0, nullptr)) {
			SMSSetEmitterPolColor(emitter, 6);
		}

		if (gpMSound->gateCheck(0x2804))
			MSoundSESystem::MSoundSE::startSoundActor(
			    0x2804, &self->getPosition(), 0, nullptr, 0, 4);

		self->onHitFlag(HIT_FLAG_UNK1);
		self->mHitPoints = 0;
	} else {
		int pTVar7 = self->getManager()->unk5C;
		if (self->checkCurAnmEnd(0)) {
			J3DFrameCtrl* ctrl = self->getMActor()->getFrameCtrl(0);

			if (spine->getTime() > pTVar7 + ctrl->getEndFrame()) {
				self->onLiveFlag(LIVE_FLAG_DEAD);
				self->onLiveFlag(LIVE_FLAG_UNK8);
				self->onLiveFlag(LIVE_FLAG_UNK20000);
				self->offLiveFlag(LIVE_FLAG_UNK10000);
				self->mHolder = nullptr;
				self->stopAnmSound();
				spine->reset();
				spine->setNext(&TNerveSmallEnemyDie::theNerve());
				spine->pushAfterCurrent(spine->getDefault());
				self->genRandomItem();
				return true;
			}
		}
	}

	return false;
}

DEFINE_NERVE(TNerveHamuKuriLand, TLiveActor)
{
	THamuKuri* self = (THamuKuri*)spine->getBody();

	if (spine->getTime() == 0)
		self->setBckAnm(5);

	if (self->checkCurAnmEnd(0)) {
		self->offHitFlag(HIT_FLAG_UNK1);
		self->unk1F0 = 0;
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveHamuKuriJitabata, TLiveActor)
{
	THamuKuri* self = (THamuKuri*)spine->getBody();

	if (spine->getTime() == 0)
		self->setBckAnm(12);

	if (self->checkCurAnmEnd(0)) {
		int timer = self->unk1F4->mSLJitabataTimer.get();
		if (self->isBckAnm(12)) {
			self->setBckAnm(3);
		} else {
			if (self->isBckAnm(11)) {
				spine->pushAfterCurrent(&TNerveSmallEnemyWait::theNerve());
				return true;
			}

			if (spine->getTime() > timer)
				self->setBckAnm(11);
		}
	}

	return 0;
}

DEFINE_NERVE(TNerveDangoHamuKuriWait, TLiveActor)
{
	THamuKuri* self = (THamuKuri*)spine->getBody();

	if (spine->getTime() < 2) {
		self->setWaitAnm();
		self->getMActor()->getFrameCtrl(0)->setFrame(MsRandF(0.0f, 30.0f));
	}

	return false;
}

DEFINE_NERVE(TNerveDangoHamuKuriAttack, TLiveActor) { }

DEFINE_NERVE(TNerveHaneHamuKuriUpWait, TLiveActor)
{
	THaneHamuKuri* self = (THaneHamuKuri*)spine->getBody();
	if (spine->getTime() < 1) {
		self->setWaitAnm();
		self->setGoalPathMario();
	}

	self->mScaling.x = self->mScaling.z
	    = MsClamp(self->mScaling.x * 0.9f, self->getBodyScale(),
	              self->getBodyScale() * 2.0f);

	self->mScaling.y
	    = MsClamp(self->mScaling.y * 1.3f, 0.0f, self->getBodyScale());

	if (spine->getTime() > 400)
		return true;

	self->unk234 += 1.0f;
	self->walkBehavior(3, 0.5f);
	return false;
}

DEFINE_NERVE(TNerveHaneHamuKuriMoveOnGraph, TLiveActor)
{
	THaneHamuKuri* self = (THaneHamuKuri*)spine->getBody();

	if (spine->getTime() == 0) {
		self->setWalkAnm();
		self->initialGraphNode();
		if (self->getTracer()->getGraph()->getNodeNum() == 1)
			self->setGoalPathMario();
	}

	if (self->getTracer()->getGraph()->getNodeNum() == 1) {
		self->walkBehavior(3, 0.5f);
	} else {
		if (self->isReachedToGoal())
			self->goToRandomNextGraphNode();
		if (self->getTracer()->getPrevIndex() >= 0) {
			JGeometry::TVec3<f32> curPos;
			self->getTracer()
			    ->getGraph()
			    ->getGraphNode(self->getTracer()->getPrevIndex())
			    .getPoint(&curPos);
			JGeometry::TVec3<f32> prevPos;
			self->getTracer()
			    ->getGraph()
			    ->getGraphNode(self->getTracer()->getCurrentIndex())
			    .getPoint(&prevPos);

			JGeometry::TVec3<f32> toCur  = curPos - self->mPosition;
			f32 distToCur                = toCur.length();
			JGeometry::TVec3<f32> toPrev = prevPos - self->mPosition;
			f32 distToPrev               = toPrev.length();
			self->unk230                 = curPos.y
			               + (distToCur * (prevPos.y - curPos.y))
			                     / (distToCur + distToPrev);
			self->walkBehavior(2, 1.0f);
		}
	}

	return false;
}

DEFINE_NERVE(TNerveDoroHamuKuriRobCap, TLiveActor)
{
	TDoroHamuKuri* self = (TDoroHamuKuri*)spine->getBody();

	TDoroHamuKuriManager* manager = (TDoroHamuKuriManager*)self->getManager();
	if (spine->getTime() == 0 || self->unk1F8 == nullptr) {
		self->unk1F8 = manager->unk70;
		self->setRunAnm();
		// TODO: one more inline?
		TPathNode target(manager->unk70);
		if (manager->unk70)
			target.unk4.set(manager->unk70->mPosition.x,
			                manager->unk70->mPosition.y,
			                manager->unk70->mPosition.z);
		self->setGoalPath(target);
	} else if (self->unk1F8 != manager->unk70) {
		self->setGoalPathMario();
		return true;
	}

	self->walkBehavior(0, 2.5f);
	return false;
}

DEFINE_NERVE(TNerveFireHamuKuriRecover, TLiveActor)
{
	TFireHamuKuri* self = (TFireHamuKuri*)spine->getBody();

	if (self->checkCurAnmEnd(0)) {
		if (self->isBckAnm(6)) {
			self->mHitPoints = self->getSaveParam()
			                       ? self->getSaveParam()->mSLHitPointMax.get()
			                       : 1;

			self->unk150 &= ~0x2;
			self->unk150 |= 0x1;
			self->unk214 = 0;
			self->unk210 = 0;
			return true;
		}

		self->setBckAnm(6);
	}

	return false;
}

DEFINE_NERVE(TNerveDoroHaneRise, TLiveActor)
{
	TDoroHaneKuri* self = (TDoroHaneKuri*)spine->getBody();

	if (self->mPosition.y < self->mGroundHeight + 800.0f)
		self->unk234 += MsClamp(self->mSpine->getTime() * 0.01f, 0.01f, 5.0f);

	self->mScaling.x = self->mScaling.z
	    = MsClamp(self->mScaling.x * 0.9f, self->getBodyScale(),
	              self->getBodyScale() * 2.0f);

	self->mScaling.y
	    = MsClamp(self->mScaling.y * 1.3f, 0.0f, self->getBodyScale());

	if (!self->isUnk198()) {
		spine->pushAfterCurrent(&TNerveWalkerGraphWander::theNerve());
		return true;
	}

	self->mRotation.x *= 0.9f;
	self->mPosition.y = self->unk230 + self->unk234;
	self->walkToCurPathNode(0.0f, 2.0f, 0.0f);
	return false;
}

DEFINE_NERVE(TNerveDoroHaneHitWater, TLiveActor)
{
	TDoroHaneKuri* self = (TDoroHaneKuri*)spine->getBody();
	if (spine->getTime() == 0) {
		self->setGoalPath((SMS_GetMarioPos()));
		self->getMActor()->setFrameRate(SMSGetAnmFrameRate() * 1.5f, 0);
	}

	if (self->mPosition.y > self->getGroundHeight() + 50.0f)
		self->unk234 -= 4.0f;

	self->unk210 = 0.0f;
	self->unk214 = 0.0f;
	self->unk20C = 0.0f;
	self->unk230 = self->getGroundHeight();
	if (spine->getTime() > 300) {
		self->getMActor()->setFrameRate(SMSGetAnmFrameRate(), 0);
		return true;
	}

	self->mRotation.y += 10.0f;
	if (!self->isBckAnm(4))
		self->TWalkerEnemy::walkBehavior(0, 0.5f);
	return false;
}

DEFINE_NERVE(TNerveDoroHanePrepareAttack, TLiveActor)
{
	TDoroHaneKuri* self = (TDoroHaneKuri*)spine->getBody();

	if (spine->getTime() < 50)
		self->unk234 += 4.0f;
	else
		self->unk234 -= 15.0f;

	if (spine->getTime() > 100)
		return true;

	self->walkBehavior(0, 0.5f);
	return false;
}
