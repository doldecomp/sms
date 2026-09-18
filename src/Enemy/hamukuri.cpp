#include <Enemy/HamuKuri.hpp>
#include <Enemy/Graph.hpp>
#include <Enemy/Conductor.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/ObjModel.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/Particles.hpp>
#include <Map/MapData.hpp>
#include <Map/Map.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/Mario.hpp>
#include <M3DUtil/MActorData.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/SDLModel.hpp>
#include <MarioUtil/PacketUtil.hpp>
#include <MarioUtil/LightUtil.hpp>
#include <MarioUtil/TexUtil.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MoveBG/ItemManager.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <MoveBG/MapObjBase.hpp>
#include <MoveBG/Item.hpp>
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

// TODO: both of these lists are dead in retail too (the strings survive in
// .rodata with nothing referencing them), so only their position is known.
// The map's ids pin it: "hamukuri_walk"/"hamukuri_run" are @3024/@3025,
// THamuKuriManager::createModelData's "default.bmd" is @3042 and
// "hanekuri_wait" is @3102, so the hanekuri name is requested by a separate
// construct further down the file.
static const char* anmlist[] = {
	"hamukuri_walk",
	"hamukuri_run",
};

void THamuKuriManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "default.bmd",
		  J3DMLF_MaterialPEFull | J3DMLF_UseUniqueMaterials
		      | (2 << J3DMLF_TevStageNumShift),
		  0 },
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

void THamuKuriManager::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (unk68 > 0 && mSearchActSw && (cue & CUE_CALC_ANIM))
		setSearchHamuKuri();

	if (cue & CUE_MOVE)
		checkSerialKill();

	TEnemyManager::perform(cue, graphics);
}

void THamuKuriManager::setSearchHamuKuri()
{
	for (int i = 0; i < getActiveObjNum(); ++i) {
		THamuKuri* kuri = (THamuKuri*)getObj(i);

		if (!kuri->canGoForSearchActor())
			continue;

		if (kuri->checkLiveFlag(LIVE_FLAG_DEAD | LIVE_FLAG_HIDDEN
		                        | LIVE_FLAG_CLIPPED_OUT))
			continue;

		THamuKuriSaveLoadParams* params = (THamuKuriSaveLoadParams*)unk38;
		s32 searchActorTime             = params->mSLSearchActorTimer.get();
		if (kuri->unk19C >= searchActorTime) {
			f32 canSearchDist = params->mSLCanSearchDist.get();

			TMapObjBase* pTVar18 = nullptr;
			f32 fVar1            = canSearchDist;
			fVar1 *= canSearchDist;

			int objNum = unk68;
			for (int i = 0; i < objNum; ++i) {
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
					fVar1   = len_sq;
					pTVar18 = uVar16;
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

	s32 trampleBonusNum = params->getTrampleBonusNum();

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

			if (obj->isSerialWallDie()) {
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

static const char* hanekuriAnmList[] = {
	"hanekuri_wait",
};

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
    , unk74(nullptr)
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

void TDoroHaneKuriManager::perform(u32 cue, JDrama::TGraphics* graphics)
{
	THamuKuriManager::perform(cue, graphics);

	if (unk74)
		unk74->perform(cue, graphics);
}

void TDoroHaneKuriManager::createHige()
{
	void* rawModelData = JKRGetResource("/scene/hanekuri/dorokuriHige.bmd");
	SDLModelData* modelData = new SDLModelData(J3DModelLoaderDataBase::load(
	    rawModelData, J3DMLF_MaterialPEFull | J3DMLF_UseUniqueMaterials
	                      | (1 << J3DMLF_TevStageNumShift)));
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

void TDoroHige::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (!unk1C->isUnk198()
	    || unk1C->checkLiveFlag(LIVE_FLAG_CLIPPED_OUT | LIVE_FLAG_HIDDEN
	                            | LIVE_FLAG_DEAD))
		return;

	TSharedParts::perform(cue, graphics);
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

void TDoroHamuKuriManager::perform(u32 cue, JDrama::TGraphics* graphics)
{
	THamuKuriManager::perform(cue, graphics);

	if (unk74)
		unk74->perform(cue, graphics);
}

void TDoroHamuKuriManager::createHige()
{
	void* rawModelData = JKRGetResource("/scene/dorokuri/dorokuriHige.bmd");
	SDLModelData* modelData = new SDLModelData(J3DModelLoaderDataBase::load(
	    rawModelData, J3DMLF_MaterialPEFull | J3DMLF_UseUniqueMaterials
	                      | (1 << J3DMLF_TevStageNumShift)));
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
	mMActor->setLightType(LIGHT_TYPE_OBJECT);
}

void THamuKuri::setMActorAndKeeper()
{
	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("default.bmd", 3);
	int idx       = getModel()->getModelData()->getMaterialName()->getIndex(
        "_mat_body_top1");
	SMS_InitPacket_OneTevKColor(getMActor()->getModel(), idx, GX_KCOLOR0,
	                            &unk1FC);
	unk1FC.a = 0x80;
	ResTIMG* img
	    = (ResTIMG*)JKRGetResource("/scene/map/pollution/H_ma_rak.bti");
	if (img != nullptr)
		SMS_ChangeTextureAll(getMActor()->getModel()->getModelData(),
		                     "H_ma_rak_dummy", *img);
	else
		unk1FC.a = 0;
}

void THamuKuri::reset()
{
	TWalkerEnemy::reset();
	offLiveFlag(LIVE_FLAG_HIDDEN);
	unk194 = unk1F4->mSLGiveUpLength.get();
	unk1F0 = 0;
	unk1A3 = 0;
	unk1A4 = 0;
}

void THamuKuri::bind()
{
	if (unk1A4) {
		if (mSpine->getTime() > unk1F4->getFlyTimer()
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

// UNUSED (0xb4). Both setDeadAnm overrides open with this exact block, so it
// is the obvious candidate; they paste it rather than call it (see the
// pasted-UNUSED family in docs/catalog).
void THamuKuri::releaseCap()
{
	if (mHeldObject != nullptr
	    && mHeldObject->receiveMessage(this, HIT_MESSAGE_PUT)) {
		TMapObjBase* heldObj = (TMapObjBase*)mHeldObject;
		heldObj->mHolder     = nullptr;
		heldObj->offLiveFlag(LIVE_FLAG_HIDDEN);
		heldObj->mPosition   = mPosition;
		heldObj->mPosition.y = mGroundHeight;
		heldObj->offHitFlag(HIT_FLAG_NO_COLLISION);
		heldObj->makeObjDead();
		mHeldObject = nullptr;
	}
}

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

	if (gpMarioOriginal->mUpperState == TMario::UPPER_STATE_PUMPING
	    && gpMarioOriginal->mWaterGun->mCurrentWater == 0) {
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

// Binding level worth +8 of low region in THamuKuri::jumpToSearchActor
// (batch 121).
static inline f32 HamukuriGetGroundHeight(const TLiveActor* p)
{
	f32 groundHeight = p->getGroundHeight();
	return groundHeight;
}

void THamuKuri::jumpToSearchActor()
{
	(void)0; // TODO: hack, need to figure out canGoForSearchActor?
	TLiveActor* pTVar5 = (TLiveActor*)unk1F8;
	if (pTVar5) {
		JGeometry::TVec3<f32> local_1C = mPosition;
		local_1C -= pTVar5->mPosition;

		if (local_1C.squared() > 40000.0f) {
			JGeometry::TVec3<f32> tgt = pTVar5->mPosition;
			tgt.y                     = HamukuriGetGroundHeight(pTVar5);

			setGoalPath(tgt);
			return;
		}

		mVelocity
		    = calcVelocityToJumpToY(unk1F8->mPosition, 1.0f, getGravityY());
		onLiveFlag(LIVE_FLAG_AIRBORNE);

		SMSGetMSound()->startSoundActor(MSD_SE_EN_DOROKURI_JUMP, &mPosition, 0,
		                                nullptr, 0, 4);
	}
}

// UNUSED (0x64), size-exact: the materialised bool that
// THamuKuriManager::setSearchHamuKuri pastes for every object it scans.
bool THamuKuri::canGoForSearchActor()
{
	if (mSpine->getCurrentNerve() == &TNerveWalkerAttack::theNerve()
	    || mSpine->getCurrentNerve() == &TNerveWalkerGraphWander::theNerve())
		return true;

	return false;
}

void THamuKuri::behaveToFindMario()
{
	TWalkerEnemy::behaveToFindMario();
	SMSGetMSound()->startSoundActor(MSD_SE_EN_HMKRI_VO_ATTACK, &mPosition);
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
			if (checkLiveFlag(LIVE_FLAG_HIDDEN)) {
				mSpine->reset();
				mSpine->setNext(&TNerveWalkerGenerate::theNerve());
				TLiveActor* heldObj = (TLiveActor*)mHeldObject;
				if (heldObj)
					heldObj->onLiveFlag(LIVE_FLAG_HIDDEN);
			}

			offLiveFlag(LIVE_FLAG_HIDDEN);
			offHitFlag(HIT_FLAG_NO_COLLISION);
		}
	}

	if (canGoForSearchActor())
		unk19C += 1;

	if (mSpine->getCurrentNerve() == &TNerveWalkerGraphWander::theNerve()) {
		s32 kyoroTimer = unk1F4->getKyoroTimer();
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
	if (isAirborne() && getPosition().y > getGroundHeight() + 250.0f
	    && mSpine->getCurrentNerve() != &TNerveWalkerGenerate::theNerve()) {
		unk1F0 = 1;
	}

	if (!isAirborne() && unk1F0
	    && (mSpine->getCurrentNerve() == &TNerveWalkerGraphWander::theNerve()
	        || mSpine->getCurrentNerve() == &TNerveWalkerAttack::theNerve())) {
		mSpine->pushNerve(&TNerveHamuKuriLand::theNerve());
	}
}

// TODO: UNUSED (0x1b4 = 109 instructions), still a stub. It takes no
// arguments, so it cannot be the cap-hop block that makeCapFly and
// TDoroHamuKuri::isCollidMove share (those are parameterised on the other
// actor); the only other cap bookkeeping in the TU is selectCapHolder.
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

	THamuKuri* holder = (THamuKuri*)getManager()->getHolder(mInstanceIndex);
	if (holder == nullptr) {
		// The negated `&&` chain is what puts the `holder = this` block first,
		// as the ROM has it; the `||` spelling emits the water branch first.
		if (!getGroundPlane()->isIllegalData() && !getGroundPlane()->isPool()
		    && !mGroundPlane->isWaterSurface()) {
			holder = this;
		} else {
			param_1->mPosition = SMS_GetMarioPos();
			param_1->mVelocity.set(0.0f, 10.0f, 0.0f);
			param_1->offLiveFlag(LIVE_FLAG_UNK10);
			return;
		}
	}

	holder->mPosition = local_3c;
	holder->mPosition.y += 100.0f;
	param_1->mPosition.y = holder->mPosition.y;

	if (param_1->receiveMessage(holder, HIT_MESSAGE_TAKE)) {
		holder->reset();
		holder->onHaveCap();
		holder->mHeldObject = param_1;
		holder->onLiveFlag(LIVE_FLAG_HIDDEN);
		holder->offLiveFlag(LIVE_FLAG_DEAD);
		holder->offLiveFlag(LIVE_FLAG_CLIPPED_OUT);
		holder->onHitFlag(HIT_FLAG_NO_COLLISION);
		getManager()->unk70 = holder;

		int uVar11 = getTracer()->getCurGraphIndex();

		int uVar10 = -1;
		TMsRange<s32> hops(2, 3);
		int count = hops.rand();
		for (int i = 0; i < count; ++i) {
			int next = getTracer()->getGraph()->getRandomNextIndex(uVar11, uVar10,
			                                                  0xffffffff);
			uVar10   = uVar11;
			uVar11   = next;
		}

		if (uVar11 < 0)
			uVar11 = 0;

		// TODO: instruction-exact with the right frame; the ROM lays the four
		// aggregates out as local_3c, local_6c, hops, VStack_60 and declaring
		// local_6c that early only grows the frame.
		JGeometry::TVec3<f32> VStack_60;
		getTracer()->getGraph()->getGraphNode(uVar11).getPoint(&VStack_60);

		JGeometry::TVec3<f32> local_6c = calcVelocityToJumpToY(
		    VStack_60, mCapSpeed, holder->getGravityY());
		holder->onLiveFlag(LIVE_FLAG_AIRBORNE);
		holder->mVelocity = local_6c;
	} else {
		holder->onLiveFlag(LIVE_FLAG_DEAD);
	}
}

f32 THamuKuri::getGravityY() const
{
	if (checkLiveFlag(LIVE_FLAG_HIDDEN))
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
		    mPosition.x, 200.0f + mPosition.y, mPosition.z, 0x2000003c, true);

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
		emitter = gpMarioParticleManager->emit(PARTICLE_MS_ENM_DISAP_A_W,
		                                       &mPosition, 0, nullptr);

		if (emitter)
			emitter->setGlobalScale(mScaling);
	} else {
		emitter = gpMarioParticleManager->emit(PARTICLE_MS_ENM_DISAP_A,
		                                       &mPosition, 0, nullptr);
	}

	if (emitter)
		emitter->setGlobalScale(mScaling);

	emitter = gpMarioParticleManager->emit(PARTICLE_MS_ENM_DISAP_B, &mPosition,
	                                       0, nullptr);
	if (emitter)
		emitter->setGlobalScale(mScaling);

	SMSGetMSound()->startSoundActor(MSD_SE_EN_COMMON_SMOKE, &mPosition, 0,
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
	if (unk198)
		releaseCap();

	if (unk184) {
		onLiveFlag(LIVE_FLAG_UNK20000);
		gpMarioParticleManager->emit(PARTICLE_MS_ENM_DISAP_A, &mPosition, 0,
		                             nullptr);
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
	// Pasted rather than routed through releaseCap(): the call costs this
	// function 16 bytes of frame, while both setDeadAnm overrides want it.
	if (unk198 && mHeldObject != nullptr
	    && mHeldObject->receiveMessage(this, HIT_MESSAGE_PUT)) {
		TMapObjBase* heldObj = (TMapObjBase*)mHeldObject;
		heldObj->mHolder     = nullptr;
		heldObj->offLiveFlag(LIVE_FLAG_HIDDEN);
		heldObj->mPosition   = mPosition;
		heldObj->mPosition.y = mGroundHeight;
		heldObj->offHitFlag(HIT_FLAG_NO_COLLISION);
		heldObj->makeObjDead();
		mHeldObject = nullptr;
	}

	unk1A4 = 0;
	setBckAnm(1);

	if (unk1A3 && getManager()->getUnk6C() == 0)
		getManager()->setUnk6C(1);
}

// UNUSED (0x130). The wall-death particle burst that
// TNerveHamuKuriWallDie::execute performs on its first frame.
void THamuKuri::setWallDeadEffect()
{
	JGeometry::TVec3<f32> local_34;
	if (checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)) {
		local_34 = getPosition();
	} else {
		MtxPtr mtx = getMActor()->getModel()->getAnmMtx(1);
		local_34.x = mtx[0][3];
		local_34.y = mtx[1][3];
		local_34.z = mtx[2][3];
	}

	if (JPABaseEmitter* emitter = gpMarioParticleManager->emitWithRotate(
	        PARTICLE_MS_ENM_WALLHIT, &local_34, 0,
	        DEG2SHORTANGLE(mRotation.y), 0, 0, nullptr)) {
		emitter->setGlobalScale(mScaling);
	}

	if (JPABaseEmitter* emitter = gpMarioParticleManager->emitWithRotate(
	        PARTICLE_MS_ENM_WALLHIT_O, &mPosition, 0,
	        DEG2SHORTANGLE(getRotation().y), 0, 0, nullptr)) {
		SMSSetEmitterPolColor(emitter, 6);
	}

	SMSGetMSound()->startSoundActor(MSD_SE_EN_HAMUKURI_CRUSHED, &mPosition, 0,
	                                nullptr, 0, 4);
}

// UNUSED (0x30): the same single virtual setBckAnm call as setGenerateAnm
// (also 0x30). Slot 0 is the first .bck alphabetically, and the two unnamed
// slots below "down" in hamukurianm_bastable are appear (0) and crash (1),
// with setCrashAnm already using 1.
void THamuKuri::setAppearAnm() { setBckAnm(0); }

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

	mHeldObject->onHitFlag(HIT_FLAG_NO_COLLISION);

	TPosition3f mat;
	mat.translation(mPosition.x, dVar4, mPosition.z);

	Mtx afStack_84;
	MsMtxSetRotRPH(afStack_84, 0.0f, 0.0f, 0.0f);
	MTXConcat(mat, afStack_84, mat);

	MtxPtr takingMtx = unk1B0;

	// TODO: identity33 but order is transposed?!
	unk1B0[0][0] = 1.0f;
	unk1B0[0][1] = 0.0f;
	unk1B0[0][2] = 0.0f;

	unk1B0[1][0] = 0.0f;
	unk1B0[1][1] = 1.0f;
	unk1B0[1][2] = 0.0f;

	unk1B0[2][0] = 0.0f;
	unk1B0[2][1] = 0.0f;
	unk1B0[2][2] = 1.0f;

	MTXConcat(mat, takingMtx, takingMtx);

	return takingMtx;
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

	// Raw read, not checkLiveFlag(): that accessor is const, so MWCC will not
	// share its load with the non-const onLiveFlag below, and the ROM loads
	// mLiveFlag exactly once here.
	if (mLiveFlag & LIVE_FLAG_HIDDEN)
		return false;

	// Dead in TFireHamuKuri's override (which returns early on the same
	// message), but the store is in the ROM for all three overrides, so the
	// original really did test the message here.
	if (param_1 == 11)
		onLiveFlag(LIVE_FLAG_HIDDEN);

	return true;
}

// Binding level worth +16 of low region, landing THamuKuri::isCollidMove's
// frame at 0xe8 (batch 121).
static inline THamuKuriManager* HamukuriGetManager(THamuKuri* p)
{
	THamuKuriManager* manager = p->getManager();
	return manager;
}

// Binding level worth +16 of low region, landing THamuKuri::isCollidMove's
// frame at 0xe8 (batch 121).
static inline bool HamukuriIsAirborne(const THamuKuri* p)
{
	bool airborne = p->isAirborne();
	return airborne;
}

bool THamuKuri::isCollidMove(THitActor* param_1)
{
	if (param_1->isActorType(0x8000013))
		if (mSpine->getCurrentNerve() == &TNerveHamuKuriBoundFreeze::theNerve())
			param_1->receiveMessage(this, HIT_MESSAGE_TRAMPLE);

	if (param_1->isActorType(0x10000002) || param_1->isActorType(0x1000000F)
	    || param_1->isActorType(0x10000013)
	    || param_1->isActorType(0x10000011)) {
		THamuKuri* hamu = (THamuKuri*)param_1;
		if (hamu->isAttackToHam()
		    && mSpine->getCurrentNerve() != &TNerveSmallEnemyDie::theNerve()
		    && mSpine->getCurrentNerve()
		           != &TNerveHamuKuriBoundFreeze::theNerve()
		    && mSpine->getCurrentNerve() != &TNerveHamuKuriWallDie::theNerve()
		    && !isHitWallInBound()) {
			unk1A3 = 1;
			forceRoll(param_1->mPosition, false);
			return false;
		}
	}

	if ((param_1->getActorType() & 0xFFFF0000) == 0x40000000
	    && param_1->getActorType() >= 0x40000390
	    && param_1->getActorType() <= 0x40000394) {
		TLiveActor* enemy         = (TLiveActor*)param_1;
		JGeometry::TVec3<f32> vel = enemy->mVelocity;
		if (fabsf(vel.y) > 2.0f
		    && (fabsf(vel.x) > 2.0f || fabsf(vel.z) > 2.0f)) {
			if (mSpine->getCurrentNerve() != &TNerveHamuKuriJitabata::theNerve()
			    && !HamukuriIsAirborne(this)) {
				mSpine->pushNerve(&TNerveHamuKuriJitabata::theNerve());
			}
		}
	}

	if (!TSmallEnemy::isCollidMove(param_1))
		return false;

	// TODO: inline
	if (!(param_1->getActorType() == HamukuriGetManager(this)->unk60 ? true
	                                                                 : false))
		return true;

	unk1A0 = 1;
	unk1F8 = nullptr;
	return false;
}

// UNUSED (0x8c). The materialised bool at THamuKuri::isCollidMove's first
// nerve test is an inlined bool predicate, and this is the only UNUSED
// candidate of the right size; the `if`/`return true` shape (rather than
// `return a == b;`) is what materialises at the call site.
bool THamuKuri::isAttackToHam()
{
	if (mSpine->getCurrentNerve() == &TNerveHamuKuriBoundFreeze::theNerve())
		return true;

	return false;
}

// UNUSED (0x98), size-exact: the materialised bool that
// THamuKuriManager::checkSerialKill pastes for every object it scans.
bool THamuKuri::isSerialWallDie()
{
	if (unk1A3 != 0
	    && mSpine->getCurrentNerve() == &TNerveHamuKuriWallDie::theNerve())
		return true;

	return false;
}

void THamuKuri::forceRoll(JGeometry::TVec3<f32> param_1, bool param_2)
{
	JGeometry::TVec3<f32> local_20(mPosition.x - param_1.x, 10.0f,
	                               mPosition.z - param_1.z);

	MsVECNormalize(&local_20, &local_20);

	// TODO: the ROM makes two copies here (into operator*'s by-value parameter
	// and back); ours makes three because JGVec3.hpp's operator*(TVec3, f32)
	// returns by value where operator-/operator+ return `const TVec3&`.
	// Ruled out: making operator* return `const TVec3&` too -- it fixes this
	// site but regresses ~15 functions across boid, telesa, beam, MapObjMare,
	// MarioMove, MarioPhysics and Tongue.
	if (mSpine->getCurrentNerve() == &TNerveWalkerAttack::theNerve()) {
		local_20 = local_20 * unk1F4->mSLWaterAttackCoeff.get();
	} else {
		local_20 = local_20 * unk1F4->mSLWaterCoeff.get();
	}

	onLiveFlag(LIVE_FLAG_AIRBORNE);
	mPosition.y += 10.0f;

	if (param_2) {
		local_20.y = unk1F4->getFirstVelocityY();
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

// fabricated: the squash clamp in THaneHamuKuri::walkBehavior tests the upper
// bound first, so it is not JGeometry::TUtil<f32>::clamp (which tests the lower
// bound first, confirmed by BeeHive).
static inline f32 HaneClampScale(f32 value, f32 min, f32 max)
{
	if (value > max)
		value = max;
	else if (value < min)
		value = min;
	return value;
}

// TODO: instruction-exact with the right frame size (0x190); the only residue
// is slot assignment for the `operator-` parameter temporary (retail 0x94, ours
// 0x118, i.e. retail puts it in the inline-expansion region and leaves a
// 12-byte hole at 0x118 -- looks like an unreferenced TVec3 local we have not
// identified), the amplitude/frequency load order (declaring the amplitude
// first fixes the order but swaps f29/f30) and the params pointer landing in
// r5 instead of r6.
void THaneHamuKuri::walkBehavior(int param_1, f32 param_2)
{
	f32 flyBaseHeight = getSaveLoadParam()->getSLFlyBaseHeight();

	if (mBoundFly) {
		if (!isAirborne()) {
			f32 jumpVy = getSaveLoadParam()->getSLNormalJumpVy();
			if (param_1 == 2)
				jumpVy = getSaveLoadParam()->getSLAttackJumpVy();

			JGeometry::TVec3<f32> vel(0.0f, jumpVy, 0.0f);
			mPosition.y += 10.0f;
			onLiveFlag(LIVE_FLAG_AIRBORNE);
			setVelocity(vel);
		}
	} else {
		if (!unk21C) {
			if (unk214 == 0.0f) {
				if (fabsf(unk230 - mGroundHeight) > 5.0f) {
					unk214 = (mGroundHeight - unk230) / 120.0f;
					if (unk214 > 10.0f)
						unk214 = 10.0f;
					if (unk214 < -10.0f)
						unk214 = -10.0f;
				}
			}

			if (unk214 > 0.0f) {
				unk230 += unk214;
				if (unk230 > mGroundHeight)
					unk214 = 0.0f;
			}

			if (unk214 < 0.0f) {
				unk230 += unk214;
				if (unk230 < mGroundHeight) {
					unk214 = 0.0f;
					unk230 = 1.0f + mGroundHeight;
				}
			}
		}

		f32 flyBaseFrequency = getSaveLoadParam()->getSLFlyBaseFrequency();
		f32 flyBaseAmplitude = getSaveLoadParam()->getSLFlyBaseAmplitude();

		if (mSpine->getCurrentNerve() == &TNerveWalkerGraphWander::theNerve()
		    || mSpine->getCurrentNerve() == &TNerveWalkerAttack::theNerve()
		    || mSpine->getCurrentNerve()
		        == &TNerveDoroHanePrepareAttack::theNerve()) {
			if (!unk21C) {
				unk20C += 1.0f;
				if (unk20C > flyBaseFrequency)
					unk20C = 0.0f;

				if (unk234 < flyBaseHeight)
					unk234 += 1.0f;

				if (unk234 > flyBaseHeight)
					unk234 -= 1.0f;
			}

			if (mSpine->getCurrentNerve() == &TNerveWalkerAttack::theNerve()
			    || mSpine->getCurrentNerve()
			        == &TNerveWalkerGraphWander::theNerve()) {
				if (unk21C) {
					mGroundHeight = gpMap->checkGround(
					    mPosition.x, mPosition.y + 2.0f * mHeadHeight,
					    mPosition.z, &mGroundPlane);

					if (unk210 + (unk230 + unk234) > mGroundHeight) {
						param_1 = 3;
						unk234 -= 15.0f;
						unk230 = mGroundHeight;
						unk210 = 0.0f;
						unk214 = 0.0f;
						unk20C = 0.0f;

						if (unk234 + unk210 > 200.0f) {
							mScaling.y = HaneClampScale(1.1f * mScaling.y, 0.0f,
							                            1.3f * mBodyScale);
						} else {
							f32 maxScale = 1.3f * getBodyScale();
							mScaling.y   = HaneClampScale(0.8f * mScaling.x,
							                              0.5f * getBodyScale(),
							                              maxScale);
						}
					} else {
						unk21C = 0.0f;
						unk214 = 0.0f;
						unk210 = 0.0f;
						unk234 = 0.0f;
						unk20C = 0.0f;

						gpMarioParticleManager->emit(PARTICLE_MS_HIPDROP_C,
						                             &mPosition, 0, nullptr);
						SMSRumbleMgr->start(0x15, 5, (f32*)nullptr);
						SMSGetMSound()->startSoundActor(MSD_SE_MA_HIP_ATTACK,
						                                &mPosition, 0, nullptr,
						                                0, 4);

						setGoalPath((THitActor*)gpMarioAddress);
						mSpine->pushNerve(
						    &TNerveHaneHamuKuriUpWait::theNerve());
					}
				} else if (isReachedToGoal()
				           && unk234 > flyBaseHeight - 10.0f) {
					unk21C = 1.0f;
					mSpine->pushNerve(
					    &TNerveDoroHanePrepareAttack::theNerve());
				}
			}
		} else {
			if (unk234 > 0.0f)
				unk234 -= 1.0f;
		}

		if (!unk21C)
			unk210 = JMASin(360.0f * unk20C / flyBaseFrequency)
			         * flyBaseAmplitude;

		mPosition.y = unk210 + (unk230 + unk234);
	}

	if (mSpine->getCurrentNerve() == &TNerveHaneHamuKuriUpWait::theNerve()) {
		if (unk234 < flyBaseHeight)
			unk234 += 1.0f;
	}

	JGeometry::TVec3<f32> dir = mPosition - unk220;
	MsVECNormalize(&dir, &dir);

	if (mSpine->getCurrentNerve() == &TNerveHaneHamuKuriUpWait::theNerve())
		mRotation.x *= 0.8f;

	mRotation.z = MsGetRotFromZaxis(dir).z;

	if (!unk21C) {
		if (!isBckAnm(4))
			TWalkerEnemy::walkBehavior(param_1, param_2);
	}

	unk220 = mPosition;
	unk218 = mRotation.y;

	if (unk21C) {
		if (mPosition.y < mGroundHeight)
			mPosition.y = mGroundHeight;
	}
}

void THaneHamuKuri::bind()
{
	if (checkLiveFlag(LIVE_FLAG_UNK10))
		return;

	JGeometry::TVec3<f32> local_18 = mPosition;
	local_18.y -= unk234 + unk210;
	local_18 += mLinearVelocity;
	local_18 += getVelocity();

	mVelocity.y -= getGravityY();

	if (getVelocity().y < mVelocityMinY)
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
	SMSGetMSound()->startSoundActor(MSD_SE_EN_HANEKURI_ATTACK, &mPosition);
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
	if (unk198)
		releaseCap();
	setBckAnm(1);
}

bool THaneHamuKuri::isCollidMove(THitActor* param_1)
{
	return TSmallEnemy::isCollidMove(param_1);
}

bool THaneHamuKuri::isHitValid(u32 param_1)
{
	if (mLiveFlag & LIVE_FLAG_HIDDEN)
		return false;

	if (param_1 == 11)
		onLiveFlag(LIVE_FLAG_HIDDEN);

	return true;
}

// UNUSED (0x1c): exactly the five zero stores THaneHamuKuri::reset makes, in
// that order (one lfs, five stfs, blr).
void THaneHamuKuri::resetFlyParam()
{
	unk214 = 0.0f;
	unk210 = 0.0f;
	unk234 = 0.0f;
	unk20C = 0.0f;
	unk21C = 0.0f;
}

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

	if (getInstanceIndex() == 0) {
		((TDoroHaneKuriManager*)getManager())->createHige();

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

// Binding level worth +8 of low region, landing
// TDoroHaneKuri::attackToMario's frame at 0x50 (batch 121).
static inline J3DModel* HamukuriGetModel(const MActor* p)
{
	J3DModel* model = p->getModel();
	return model;
}

void TDoroHaneKuri::attackToMario()
{
	if (!gpMarioOriginal->isWearingCap()) {
		if (SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK))
			SMSGetMSound()->startSoundActor(MSD_SE_EN_HANEKURI_ATTACK,
			                                &mPosition, 0, nullptr, 0, 4);
	} else {
		if (SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK)) {
			SMSGetMSound()->startSoundActor(MSD_SE_EN_HANEKURI_ATTACK,
			                                &mPosition, 0, nullptr, 0, 4);

			mSpine->pushNerve(&TNerveDoroHaneRise::theNerve());
			onHaveCap();
			MtxPtr mtx = HamukuriGetModel(mMActor)->getAnmMtx(unk1AC);
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
	if (mSpine->getCurrentNerve() == &TNerveSmallEnemyDie::theNerve())
		releaseCap();
}

bool TDoroHaneKuri::isCollidMove(THitActor* param_1)
{
	if ((param_1->getActorType() & 0xffff0000) == 0x40000000) {
		TMapObjBase* mapObj = (TMapObjBase*)param_1;
		if (mapObj->isHideObj(mapObj))
			return false;

		if (getSpine()->getCurrentNerve() == &TNerveWalkerAttack::theNerve()) {
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
	f32 flyBaseFrequency = getSaveLoadParam()->mSLFlyBaseFrequency.get();
	f32 flyBaseAmplitude = getSaveLoadParam()->mSLFlyBaseAmplitude.get();

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

void TDangoHamuKuri::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (mBoss
	    && !(mBoss->mSpine->getCurrentNerve()
	                 == &TNerveWalkerGenerate::theNerve()
	             ? true
	             : false))
		TSmallEnemy::perform(cue, graphics);
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
	getMActor()->calc();
	MtxPtr mtx = getMActor()->getModel()->getAnmMtx(unk1AC);
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
					TMsRange<f32> spin(10.0f, 20.0f);
					unk210 = -spin.rand();
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
	TMsRange<f32> phase(0.0f, 1.0f);
	unk20C = phase.rand();
	mMActor->calc();
}

BOOL TDangoHamuKuri::receiveMessage(THitActor* sender, u32 message)
{
	if (message == HIT_MESSAGE_TAKE && mHolder == nullptr && mBoss != this) {
		onHitFlag(HIT_FLAG_NO_COLLISION);
		mHolder = (TLiveActor*)sender;
		behaveToTaken(sender);
		return true;
	}

	if ((message == HIT_MESSAGE_PUT || message == HIT_MESSAGE_THROWN)
	    && mHolder == sender) {
		mHolder = nullptr;
		behaveToRelease();
		offHitFlag(HIT_FLAG_NO_COLLISION);
		return true;
	}

	if (message == HIT_MESSAGE_TRAMPLE || message == HIT_MESSAGE_HIP_DROP
	    || message == HIT_MESSAGE_SUPER_HIP_DROP
	    || message == HIT_MESSAGE_UNKB) {
		if (isHitValid(message)) {
			unk184 = 0;
			kill();
		}
		return true;
	}

	if (message == HIT_MESSAGE_UNKD) {
		mHitPoints = 0;
		onLiveFlag(LIVE_FLAG_DEAD);
		onHitFlag(HIT_FLAG_NO_COLLISION);
	}

	if (message == HIT_MESSAGE_SPRAYED_BY_WATER) {
		gpMarioParticleManager->emit(PARTICLE_MS_ENM_WATHIT, &mPosition, 0,
		                             nullptr);
		gpMSound->startSoundSet(MSD_SE_EN_COMMON_W_HIT_OK, &mPosition, 0.0f,
		                        0.0f, 0, 0, 4);
		if (mSprayedByWaterCooldown == 0) {
			mSprayedByWaterCooldown = 1;
			if (!changeByJuice()) {
				decHpByWater(sender);
				behaveToWater(sender);
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

// UNUSED (0x9c). One theNerve() expansion plus initWith compiles to 0x8c, so
// four instructions are still missing -- probably one or two member resets
// before the initWith, but nothing in the TU names them.
void TDangoHamuKuri::nerveInit()
{
	mSpine->initWith(&TNerveDangoHamuKuriWait::theNerve());
}

void TDangoHamuKuri::behaveToWater(THitActor* param_1)
{
	if (!mNext) {
		if (!mPrev) {
			THamuKuri::behaveToWater(param_1);
		} else if (mSprayedByWaterCooldown <= 1
		           && receiveMessage(mPrev, HIT_MESSAGE_PUT)) {
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
		SMSGetMSound()->startSoundActor(MSD_SE_EN_DANGOKURI_TURN, &mPosition);
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
					mRotation = mBoss->mRotation;
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
    , unk238(0)
{
}

void TBossDangoHamuKuri::init(TLiveManager* param_1)
{
	TDangoHamuKuri::init(param_1);
	unk23C = (TBossDangoHamuKuriSaveLoadParams*)getSaveParam();
	mSpine->initWith(&TNerveWalkerGenerate::theNerve());
	mBoss = this;
}

void TBossDangoHamuKuri::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TSmallEnemy::perform(cue, graphics);
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
	s32 numArray = unk23C->getNumArray();
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
			if (newHamu->receiveMessage(currHamu, HIT_MESSAGE_TAKE))
				currHamu->mHeldObject = newHamu;
			newHamu->mBoss = this;
			break;
		}
		currHamu = currHamu->mNext;
	}

	newHamu->offHitFlag(HIT_FLAG_NO_COLLISION);
}

// UNUSED (0x44 each): one comparison against an out-of-line theNerve(), which
// is exactly 17 instructions; the walker nerve set has both an Attack and a
// Generate nerve and nothing else fits the names.
bool TBossDangoHamuKuri::isNowAttack()
{
	if (mSpine->getCurrentNerve() == &TNerveWalkerAttack::theNerve())
		return true;

	return false;
}

bool TBossDangoHamuKuri::isNowGenerate()
{
	if (mSpine->getCurrentNerve() == &TNerveWalkerGenerate::theNerve())
		return true;

	return false;
}

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
		if (mHitPoints == 0)
			dieFire();
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
	mMActorKeeper = new TMActorKeeper(getManager(), 1);
	mMActor       = getActorKeeper()->createMActor("default.bmd", 3);
	ResTIMG* img
	    = (ResTIMG*)JKRGetResource("/scene/map/pollution/H_ma_rak.bti");
	if (img)
		SMS_ChangeTextureAll(getMActor()->getModel()->getModelData(),
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
		        PARTICLE_MS_MOE_FIRE_C,
		        getMActor()->getModel()->getAnmMtx(unk1AC), 3, this)) {
			emitter->setGlobalScale(mScaling);
		}
		if (JPABaseEmitter* emitter
		    = gpMarioParticleManager->emitAndBindToMtxPtr(
		        PARTICLE_MS_MOE_FIRE_A,
		        getMActor()->getModel()->getAnmMtx(unk1AC), 1, this)) {
			emitter->setGlobalScale(mScaling);
		}
		if (JPABaseEmitter* emitter
		    = gpMarioParticleManager->emitAndBindToMtxPtr(
		        PARTICLE_MS_MOE_FIRE_B,
		        getMActor()->getModel()->getAnmMtx(unk1AC), 1, this)) {
			emitter->setGlobalScale(mScaling);
		}
		if (JPABaseEmitter* emitter
		    = gpMarioParticleManager->emitAndBindToMtxPtr(
		        PARTICLE_MS_MOE_FIRE_D,
		        getMActor()->getModel()->getAnmMtx(unk1AC), 1, this)) {
			emitter->setGlobalScale(mScaling);
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

// TODO: instruction-exact except that MWCC inlines
// THamuKuriManager::requestSerialKill here while the ROM `bl`s it -- yet the
// ROM *does* inline the same callee into THamuKuri::isHitValid at the same
// depth. Ruled out: a TFireHamuKuri-side getManager() forwarder, the raw
// ((THamuKuriManager*)mManager) receiver, and reordering the guards.
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

	// Raw read, not checkLiveFlag(): that accessor is const, so MWCC will not
	// share its load with the non-const onLiveFlag below, and the ROM loads
	// mLiveFlag exactly once here.
	if (mLiveFlag & LIVE_FLAG_HIDDEN)
		return false;

	// Dead in TFireHamuKuri's override (which returns early on the same
	// message), but the store is in the ROM for all three overrides, so the
	// original really did test the message here.
	if (param_1 == 11)
		onLiveFlag(LIVE_FLAG_HIDDEN);

	return true;
}

// TODO: this is the wrong inline, size doesn't match at all!
// TODO: UNUSED (0x88) but ours is 0xa8. The inlined copy inside
// changeTevColor is instruction-exact, so the shape is right at the call site;
// the standalone copy is eight instructions over, probably the `result` local.
bool TFireHamuKuri::recoverFire()
{
	bool result = false;
	if (!unk210) {
		if (unk218 < 30) {
			unk218 += 1;
			result = true;
		}
	} else {
		SMSGetMSound()->startSoundActor(MSD_SE_EN_MOEKURI_FLAME, &mPosition);
		if (unk218 > 0) {
			unk218 -= 1;
			result = true;
		}
	}
	return result;
}

void TFireHamuKuri::setWalkAnm() { setBckAnm(14); }

// TODO: UNUSED (0x24 = 9 instructions), still a stub. Nine instructions is a
// frame plus a single non-virtual call with one argument; the flag block that
// TFireHamuKuri::reset and TNerveFireHamuKuriRecover share is already 10
// instructions on its own, so it is not that.
void TFireHamuKuri::genFire() { }

// UNUSED (0xe8): the fire going out, as TFireHamuKuri::behaveToWater performs
// it when the last hit point is gone.
void TFireHamuKuri::dieFire()
{
	unk210 = 0;
	unk1A2 = 1;
	unk150 |= 0x2;
	unk150 &= ~0x1;
	unk214 = 1;
	SMSGetMSound()->startSoundActor(MSD_SE_EN_MOEKURI_COOL, &mPosition, 0,
	                                nullptr, 0, 4);
	if (JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
	        PARTICLE_MS_MOE_FIRE_OFF,
	        mMActor->getModel()->getAnmMtx(unk1AC), 0, nullptr)) {
		emitter->setGlobalScale(mScaling);
	}
}

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

	if (getInstanceIndex() == 0) {
		((TDoroHamuKuriManager*)getManager())->createHige();
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
	if (!unk198 && man->getUnk70()) {
		if (mSpine->getCurrentNerve() == &TNerveWalkerGraphWander::theNerve()
		    && !isAirborne()) {
			mSpine->pushNerve(&TNerveDoroHamuKuriRobCap::theNerve());
			unk1F8 = man->getUnk70();
		}
	} else {
		THamuKuri::setBehavior();
	}
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
				JGeometry::TVec3<f32> local_4c = param_1->mPosition;
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

				if (pTVar1->receiveMessage(param_1, HIT_MESSAGE_PUT)) {
					pTVar1->mPosition = param_1->mPosition;
					if (pTVar1->receiveMessage(this, HIT_MESSAGE_TAKE)) {
						other->unk198      = 0;
						other->mHeldObject = nullptr;
						onHaveCap();
						mHeldObject = pTVar1;

						// The cap hops along the *other* hamukuri's graph.
						int uVar11 = other->getTracer()->getCurGraphIndex();

						int uVar10 = -1;
						TMsRange<s32> hops(2, 3);
						int count = hops.rand();
						for (int i = 0; i < count; ++i) {
							int next
							    = other->getTracer()->getGraph()
							          ->getRandomNextIndex(uVar11, uVar10,
							                               0xffffffff);
							uVar10 = uVar11;
							uVar11 = next;
						}

						JGeometry::TVec3<f32> VStack_60;
						other->getTracer()
						    ->getGraph()
						    ->getGraphNode(uVar11)
						    .getPoint(&VStack_60);

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
		if (self->unk1E0 >= params->getBoundNum()) {
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
		self->setWallDeadEffect();
		self->onHitFlag(HIT_FLAG_NO_COLLISION);
		self->mHitPoints = 0;
	} else {
		int pTVar7 = self->getManager()->unk5C;
		if (self->checkCurAnmEnd(0)) {
			J3DFrameCtrl* ctrl = self->getMActor()->getFrameCtrl(ANM_TYPE_BCK);

			if (spine->getTime() > pTVar7 + ctrl->getEnd()) {
				self->onLiveFlag(LIVE_FLAG_DEAD);
				self->onLiveFlag(LIVE_FLAG_UNK8);
				self->onLiveFlag(LIVE_FLAG_UNK20000);
				self->offLiveFlag(TSmallEnemy::LIVE_FLAG_MELT_ON_DEATH);
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
		self->offHitFlag(HIT_FLAG_NO_COLLISION);
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
		TMsRange<f32> frame(0.0f, 30.0f);
		self->getMActor()->getFrameCtrl(ANM_TYPE_BCK)->setFrame(frame.rand());
	}

	return false;
}

// TODO: UNUSED (0xb4), still a stub, and the nerve is dead (its vtable is
// UNUSED too). 0xb4 is exactly the size of TNerveDangoHamuKuriWait::execute,
// so the body is very likely that shape with a different animation setter --
// but setWaitAnm, setRunAnm and setRollAnm all compile to the same size, so
// nothing here discriminates.
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
			    ->getGraphNode(self->getTracer()->getCurGraphIndex())
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
		self->setGoalPath(manager->unk70);
	} else if (self->unk1F8 != manager->unk70) {
		self->setGoalPath((THitActor*)gpMarioAddress);
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

// Binding level worth +8 of low region, landing
// TNerveDoroHaneRise::execute's frame at 0x58 (batch 121).
static inline f32 HamukuriGetBodyScale(const TDoroHaneKuri* p)
{
	f32 bodyScale = p->getBodyScale();
	return bodyScale;
}

DEFINE_NERVE(TNerveDoroHaneRise, TLiveActor)
{
	TDoroHaneKuri* self = (TDoroHaneKuri*)spine->getBody();

	if (self->mPosition.y < self->mGroundHeight + 800.0f)
		self->unk234 += MsClamp(self->mSpine->getTime() * 0.01f, 0.01f, 5.0f);

	self->mScaling.x = self->mScaling.z
	    = MsClamp(self->mScaling.x * 0.9f, HamukuriGetBodyScale(self),
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
		self->getMActor()->setFrameRate(SMSGetAnmFrameRate() * 1.5f,
		                                ANM_TYPE_BCK);
	}

	if (self->mPosition.y > self->getGroundHeight() + 50.0f)
		self->unk234 -= 4.0f;

	self->unk210 = 0.0f;
	self->unk214 = 0.0f;
	self->unk20C = 0.0f;
	self->unk230 = self->getGroundHeight();
	if (spine->getTime() > 300) {
		self->getMActor()->setFrameRate(SMSGetAnmFrameRate(), ANM_TYPE_BCK);
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
