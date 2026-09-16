#include <Enemy/Igaiga.hpp>
#include <Enemy/Conductor.hpp>
#include <Enemy/Graph.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/SharedParts.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/SDLModel.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/ModelWaterManager.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <Map/MapEventSink.hpp>
#include <Enemy/AreaCylinder.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

static const char* igaiga_bastable[] = {
	"/scene/igaiga/bas/igaiga_down1.bas",
	"/scene/igaiga/bas/igaiga_down2.bas",
	nullptr,
	nullptr,
	"/scene/igaiga/bas/igaiga_shoot1.bas",
	"/scene/igaiga/bas/igaiga_waterdown1.bas",
	"/scene/igaiga/bas/igaiga_waterhit1.bas",
	nullptr,
};

static const char* gorogoro_bastable[] = {
	nullptr,
	nullptr,
	nullptr,
	nullptr,
};

f32 TRollEnemy::mBoundVal     = 80.0f;
f32 TRollEnemy::mTransYOffset;
f32 TIgaiga::mReachNodeDist   = 300.0f;
// UNUSED in the map; the values are not recoverable from the binary.
f32 TIgaiga::mTremblePow      = 1.0f;
f32 TIgaiga::mTrembleAcc      = 1.0f;
f32 TIgaiga::mTrembleBrk      = 1.0f;

static TRollEnemy* gpCurRollEnemy;

static int RollEnemyBodyCallback(J3DNode* node, int param);

// UNUSED, 0x160 in the map: only ever constructed inline by the managers.
TRollEnemySaveLoadParams::TRollEnemySaveLoadParams(const char* prm)
    : TWalkerEnemyParams(prm)
    , PARAM_INIT(mSLGenerateInterval, 300)
    , PARAM_INIT(mSLExpandRate, 1.0f)
    , PARAM_INIT(mSLExpandMax, 1.5f)
    , PARAM_INIT(mSLBoundVYMax, 15.0f)
    , PARAM_INIT(mSLGroundOffsetY, 150.0f)
{
	TParams::load(mPrmPath);
}

TRollEnemy::TRollEnemy(const char* name)
    : TWalkerEnemy(name)
{
	mRollAngle = 0.0f;
	unk198     = 0.0f;
	unk19C     = 0.0f;
	unk1A0     = 0.0f;
	unk1A4     = nullptr;
	unk1A8     = 0;
	unk1AC     = 0.0f;
	unk1B0     = 1.0f;
}

void TRollEnemy::attackToMario() { SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK); }

void TRollEnemy::flagJump()
{
	// Hop toward the current graph node, 30 up, at the tracer's speed.
	JGeometry::TVec3<f32> target;
	unk124->getCurrent().getPoint((Vec*)&target);
	mPosition.y += 30.0f;

	JGeometry::TVec3<f32> vel
	    = calcVelocityToJumpToY(target, unk124->unkC, getGravityY());
	unk1A8    = 1;
	mVelocity = vel;
	onLiveFlag(LIVE_FLAG_AIRBORNE);
}

bool TRollEnemy::isCollidMove(THitActor* param_1)
{
	// Running into a cannon shell kills it; anything else it just hits.
	if (param_1->isActorType(0x4000022B))
		kill();
	param_1->receiveMessage(this, HIT_MESSAGE_ATTACK);
	return true;
}

bool TRollEnemy::isReachedToGoalXZ()
{
	JGeometry::TVec3<f32> d(unk104.getPoint());
	d.x -= mPosition.x;
	d.y -= mPosition.y;
	d.z -= mPosition.z;
	if (!unk1A8)
		d.y = 0.0f;
	return MsVECMag2(d) < 200.0f;
}

void TIgaigaPolluteModelManager::init(TLiveActor* param_1)
{
	TEnemyPolluteModelManager::init(param_1);

	void* res = JKRFileLoader::getGlbResource(
	    "/scene/igaiga/stamp_igaiga_model1.bmd");
	SDLModelData* modelData = new SDLModelData(J3DModelLoaderDataBase::load(
	    res, J3DMLF_MaterialPEFull | J3DMLF_UseUniqueMaterials
	             | (2 << J3DMLF_TevStageNumShift)));

	for (int i = 0; i < unk14; ++i)
		unk18[i] = new TIgaigaPolluteModel(param_1, modelData);
}

void TIgaigaPolluteModel::setAnm()
{
	unk10->unk18->setBckFromIndex(7);
	unk10->unk18->getFrameCtrl(0)->setFrame(0.0f);
}

TIgaigaManager::TIgaigaManager(const char* name)
    : TSmallEnemyManager(name)
{
	unk64          = 0;
	unk68          = nullptr;
	gpCurRollEnemy = nullptr;
}

void TIgaigaManager::load(JSUMemoryInputStream& stream)
{
	TSmallEnemyManager::load(stream);
	unk38 = new TRollEnemySaveLoadParams("/enemy/igaiga.prm");
	unk68 = new TWaterEmitInfo("/enemy/igaigawater.prm");
}

void TIgaigaManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "igaiga_model1.bmd", 0x11240000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

TSpineEnemy* TIgaigaManager::createEnemyInstance()
{
	return new TIgaiga("イガイガ");
}

void TIgaigaManager::initSetEnemies()
{
	unk60 = new TIgaigaPolluteModelManager;
	unk60->init((TLiveActor*)unk18[0]);
}

void TIgaigaManager::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TEnemyManager::perform(cue, graphics);
	unk60->perform(cue, graphics);
}

TIgaiga::TIgaiga(const char* name)
    : TRollEnemy(name)
{
	unk1B4 = 0;
	unk1B8 = 0;
	unk1BC = 1;
	unk1CC = 1.0f;
	unk1D0 = 0;
	unk1E4 = 1.0f;
	unk1E8 = 0;
}

void TIgaiga::setMActorAndKeeper()
{
	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("igaiga_model1.bmd", 0);
}

void TIgaiga::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TSmallEnemy::perform(cue, graphics);
}

void TIgaiga::calcRootMatrix()
{
	gpCurRollEnemy = this;
	TSpineEnemy::calcRootMatrix();
}

bool TIgaiga::isRolling()
{
	return mSpine->getCurrentNerve() == &TNerveIgaigaRollOnGraph::theNerve()
	    || mSpine->getCurrentNerve() == &TNerveIgaigaShootFromCannon::theNerve()
	    || mSpine->getCurrentNerve() == &TNerveIgaigaWaterHit::theNerve();
}

void TIgaiga::behaveToWater(THitActor* param_1)
{
	mSprayedByWaterCooldown = 0;

	// Every hit swells it by mSLExpandRate, up to mSLExpandMax.
	if (unk1E4 < unk1A4->mSLExpandMax.get())
		unk1E4 *= unk1A4->mSLExpandRate.get();

	unk165 = true;
	if (mSpine->getCurrentNerve() != &TNerveIgaigaWaterHit::theNerve())
		mSpine->pushNerve(&TNerveIgaigaWaterHit::theNerve());
}

void TIgaiga::kill()
{
	mRollAngle = 0.0f;
	TSmallEnemy::kill();
}

bool TIgaiga::isReachedToGoalXZ()
{
	JGeometry::TVec3<f32> d(unk104.getPoint());
	d.x -= mPosition.x;
	d.y -= mPosition.y;
	d.z -= mPosition.z;
	if (!unk1A8)
		d.y = 0.0f;
	d.y = 0.0f;
	return MsVECMag2(d) < mReachNodeDist;
}

void TIgaiga::setWalkAnm() { setBckAnm(3); }

const char** TIgaiga::getBasNameTable() const { return igaiga_bastable; }

bool TIgaiga::isHitValid(u32 param_1)
{
	// A hip drop is the one hit that does not count as a proper kill.
	unk1BC = 1;
	if (param_1 == HIT_MESSAGE_HIP_DROP)
		unk1BC = 0;
	return true;
}

void TIgaiga::shoot(JGeometry::TVec3<f32>& velocity)
{
	mSpine->pushNerve(&TNerveIgaigaShootFromCannon::theNerve());
	mShootVelocity = velocity;
	offLiveFlag(LIVE_FLAG_UNK10);
	unk1A8 = 1;
}

// TODO: incorrect size. Map records 288 bytes.
DEFINE_NERVE(TNerveIgaigaRollOnGraph, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 624 bytes.
DEFINE_NERVE(TNerveIgaigaWaterHit, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 376 bytes.
DEFINE_NERVE(TNerveIgaigaShootFromCannon, TLiveActor) { return FALSE; }

void TGorogoroPolluteModelManager::init(TLiveActor* param_1)
{
	TEnemyPolluteModelManager::init(param_1);

	void* res = JKRFileLoader::getGlbResource(
	    "/scene/gorogoro/bosspaku_head_stamp.bmd");
	SDLModelData* modelData = new SDLModelData(J3DModelLoaderDataBase::load(
	    res, J3DMLF_MaterialPEFull | J3DMLF_UseUniqueMaterials
	             | (2 << J3DMLF_TevStageNumShift)));

	for (int i = 0; i < unk14; ++i)
		unk18[i] = new TGorogoroPolluteModel(param_1, modelData);
}

void TGorogoroPolluteModel::setAnm()
{
	unk10->unk18->setBckFromIndex(3);
	unk10->unk18->getFrameCtrl(0)->setFrame(0.0f);
}

TGorogoroManager::TGorogoroManager(const char* name)
    : TSmallEnemyManager(name)
{
	unk60 = 0;
	unk64 = nullptr;
	unk68 = 1;
	unk6C = nullptr;
	unk70 = nullptr;
}

void TGorogoroManager::load(JSUMemoryInputStream& stream)
{
	TSmallEnemyManager::load(stream);
	unk38 = new TRollEnemySaveLoadParams("/enemy/gorogoro.prm");

	static const char* anmlist[] = { "bosspaku_head_move", nullptr };
	createSharedMActorSet(anmlist);
}

void TGorogoroManager::loadAfter()
{
	unk64 = JDrama::TNameRefGen::search<TMapEventSink>(
	    "イベント（地形沈むビアンコ）");
	unk70 = (TAreaCylinderManager*)gpConductor->search("ゴロゴロ発生マネージャー");
}

TSpineEnemy* TGorogoroManager::createEnemyInstance()
{
	return new TGorogoro("ゴロゴロ");
}

void TGorogoroManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "bosspaku_head.bmd", 0x10300000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

// UNUSED, 0x94 in the map: only ever constructed inline by the manager.
TGorogoro::TGorogoro(const char* name)
    : TRollEnemy(name)
{
	unk1E4            = 0;
	mGenerateGraphIdx = 0;
	unk1EC            = 0;
	gpCurRollEnemy    = nullptr;
}

void TGorogoro::reset()
{
	unk130 = 1;
	TRollEnemy::reset();
	offLiveFlag(LIVE_FLAG_UNK1000);
	mTevKColor.a = 0xFF;
	unk1AC       = -10.0f;
	unk1B0 = 1.0f;
}

const char** TGorogoro::getBasNameTable() const { return gorogoro_bastable; }

void TGorogoro::setMActorAndKeeper()
{
	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("bosspaku_head.bmd", 3);
}

// TODO: incorrect size. Map records 192 bytes.
DEFINE_NERVE(TNerveGorogoroRollOnGraph, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 564 bytes.
DEFINE_NERVE(TNerveGorogoroDie, TLiveActor) { return FALSE; }
