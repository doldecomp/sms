#include <MoveBG/MapObjManager.hpp>
#include <MoveBG/MapObjBase.hpp>
#include <MoveBG/Item.hpp>
#include <MoveBG/MapObjBall.hpp>
#include <MoveBG/Item.hpp>
#include <MoveBG/ItemManager.hpp>
#include <MoveBG/MapObjBlock.hpp>
#include <MoveBG/MapObjRicco.hpp>
#include <MoveBG/MapObjFence.hpp>
#include <MoveBG/MapObjItem2.hpp>
#include <MoveBG/MapObjMamma.hpp>
#include <MoveBG/MapObjTown.hpp>
#include <MoveBG/MapObjPinna.hpp>
#include <MoveBG/MapObjMare.hpp>
#include <MoveBG/MapObjMonte.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <M3DUtil/MActorData.hpp>
#include <M3DUtil/MActorUtil.hpp>
#include <M3DUtil/MActor.hpp>
#include <System/MarDirector.hpp>
#include <JSystem/JDrama/JDRDrawBufObj.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <stdio.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

static void dummy(Vec* v) { *v = (Vec) { 0.0f, 0.0f, 0.0f }; }
static void dummy2(Vec* v) { *v = (Vec) { 1.0f, 1.0f, 1.0f }; }

TMapObjManager* gpMapObjManager;

static TMapObjBase* newItemByName(const char*);
static TMapObjBase* newUniqueObjByName(const char*);

void TMapObjManager::entryStaticDrawBufferShadow(J3DModel* model)
{
	j3dSys.setDrawBuffer(mDrawBufferShadowOpa->getDrawBuffer(), 0);
	j3dSys.setDrawBuffer(mDrawBufferShadowXlu->getDrawBuffer(), 1);
	model->entry();
}

void TMapObjManager::entryStaticDrawBufferSun(J3DModel* model)
{
	j3dSys.setDrawBuffer(mDrawBufferSunOpa->getDrawBuffer(), 0);
	j3dSys.setDrawBuffer(mDrawBufferSunXlu->getDrawBuffer(), 1);
	model->entry();
}

void TMapObjManager::loadAfter()
{
	TMapObjBaseManager::loadAfter();
	mClipFar     = 100000.0f;
	mActorRadius = 1000.0f;

	newAndRegisterObj("NormalBlock");

	for (int i = 0; i < 10; ++i) {
		newAndRegisterObj("JuiceBlock");
	}
}

void TMapObjManager::initDrawBuffer()
{
	mDrawBufferSunOpa = static_cast<JDrama::TDrawBufObj*>(
	    JDrama::TNameRefGen::getInstance()->getRootNameRef()->search(
	        "DrawBuf StaticMapObj SunOpa"));
	mDrawBufferSunXlu = static_cast<JDrama::TDrawBufObj*>(
	    JDrama::TNameRefGen::getInstance()->getRootNameRef()->search(
	        "DrawBuf StaticMapObj SunXlu"));
	mDrawBufferShadowOpa = static_cast<JDrama::TDrawBufObj*>(
	    JDrama::TNameRefGen::getInstance()->getRootNameRef()->search(
	        "DrawBuf StaticMapObj ShadowOpa"));
	mDrawBufferShadowXlu = static_cast<JDrama::TDrawBufObj*>(
	    JDrama::TNameRefGen::getInstance()->getRootNameRef()->search(
	        "DrawBuf StaticMapObj ShadowXlu"));
	mDrawBufferAfterIndirectOpa = static_cast<JDrama::TDrawBufObj*>(
	    JDrama::TNameRefGen::getInstance()->getRootNameRef()->search(
	        "DrawBuf AfterIndirect Opa"));
	mDrawBufferAfterIndirectXlu = static_cast<JDrama::TDrawBufObj*>(
	    JDrama::TNameRefGen::getInstance()->getRootNameRef()->search(
	        "DrawBuf AfterIndirect Xlu"));
}

J3DMaterialTable* TMapObjManager::loadMatTable(const char* name)
{
	void* res = JKRGetResource(name);
	J3DMaterialTable* ret;
	if (res)
		ret = J3DModelLoaderDataBase::loadMaterialTable(res);
	else
		ret = nullptr;
	return ret;
}

void TMapObjManager::load(JSUMemoryInputStream& stream)
{
	TMapObjBaseManager::load(stream);
	unk40 = new MActorAnmData;

	unk40->init("/common/map", nullptr);

	initDrawBuffer();

	mSkyMatTable        = loadMatTable("/scene/map/map/sky.bmt");
	mNozzleItemMatTable = loadMatTable("/scene/mapObj/nozzleItem.bmt");
	mNozzleBoxMatTable  = loadMatTable("/scene/mapObj/nozzleBox.bmt");
	mFlowerMatTable     = loadMatTable("/scene/mapObj/flower.bmt");
	mArrowBoardMatTable = loadMatTable("/scene/mapObj/ArrowBoard.bmt");
	mWoodBoxMatTable    = loadMatTable("/scene/mapObj/kibako.bmt");
	mBarrelMatTable     = loadMatTable("/scene/mapObj/barrel.bmt");
	mBrickBlockMatTable = loadMatTable("/scene/mapObj/BrickBlock.bmt");
	mWaterMelonBlockMatTable
	    = loadMatTable("/scene/mapObj/WaterMelonBlock.bmt");
	if (SMSGetMarDirector()->getCurrentMap() == 2)
		mBiancoMatTable = loadMatTable("/scene/mapObj/bianco.bmt");
	mLeafBoatMatTable  = loadMatTable("/scene/mapObj/LeafBoat.bmt");
	mRiccoShipMatTable = loadMatTable("/scene/mapObj/riccoShip.bmt");

	if ((SMSGetMarDirector()->getCurrentMap() == 3
	     && (SMSGetMarDirector()->getCurrentStage() == 1
	         || SMSGetMarDirector()->getCurrentStage() == 5))
	    || SMSGetMarDirector()->getCurrentMap() == 0x1E) {
		mSurfGessoModelData = SMS_MakeSDLModelData(
		    "/scene/mapObj/surfgeso.bmd", J3DMLF_MaterialPEFull
		                                      | J3DMLF_UseUniqueMaterials
		                                      | (2 << J3DMLF_TevStageNumShift));
		mRedGesso    = SMS_MakeMActorFromSDLModelData(mSurfGessoModelData,
		                                              getMActorAnmData(), 3);
		mYellowGesso = SMS_MakeMActorFromSDLModelData(mSurfGessoModelData,
		                                              getMActorAnmData(), 3);
		mGreenGesso  = SMS_MakeMActorFromSDLModelData(mSurfGessoModelData,
		                                              getMActorAnmData(), 3);
		TMapObjBase::initPacketMatColor(mRedGesso->getModel(), GX_TEVREG1,
		                                &unkA8);
		TMapObjBase::initPacketMatColor(mYellowGesso->getModel(), GX_TEVREG1,
		                                &unkB0);
		TMapObjBase::initPacketMatColor(mGreenGesso->getModel(), GX_TEVREG1,
		                                &unkB8);
	}

	mSandBombBaseMatTable = loadMatTable("/scene/mapObj/SandBombBase.bmt");
	mMirrorMatTable       = loadMatTable("/scene/mapObj/mirror.bmt");

	if (SMSGetMarDirector()->getCurrentMap() == 4)
		mBridgeRopeTexture
		    = (ResTIMG*)JKRGetResource("/scene/mapObj/mon_bri_rope.bti");
	if (SMSGetMarDirector()->getCurrentMap() == 13)
		mBridgeRopeTexture
		    = (ResTIMG*)JKRGetResource("/scene/mapObj/mon_bri_rope.bti");
	if (SMSGetMarDirector()->getCurrentMap() == 9)
		mCogwheelRopeTexture
		    = (ResTIMG*)JKRGetResource("/scene/mapObj/cogwheel_rope.bti");
	if (SMSGetMarDirector()->getCurrentMap() == 8)
		mBridgeRopeTexture
		    = (ResTIMG*)JKRGetResource("/scene/mapObj/mon_bri_rope.bti");
}

TMapObjManager::TMapObjManager(const char* name)
    : TMapObjBaseManager(name)
    , unk40(nullptr)
    , mDrawBufferSunOpa(nullptr)
    , mDrawBufferSunXlu(nullptr)
    , mDrawBufferShadowOpa(nullptr)
    , mDrawBufferShadowXlu(nullptr)
    , mDrawBufferAfterIndirectOpa(nullptr)
    , mDrawBufferAfterIndirectXlu(nullptr)
    , mSkyMatTable(nullptr)
    , mNozzleItemMatTable(nullptr)
    , mNozzleBoxMatTable(nullptr)
    , mFlowerMatTable(nullptr)
    , mArrowBoardMatTable(nullptr)
    , mWoodBoxMatTable(nullptr)
    , mBarrelMatTable(nullptr)
    , mBrickBlockMatTable(nullptr)
    , mWaterMelonBlockMatTable(nullptr)
    , mBiancoMatTable(nullptr)
    , mLeafBoatMatTable(nullptr)
    , mRiccoShipMatTable(nullptr)
    , mSurfGessoModelData(nullptr)
    , mRedGesso(nullptr)
    , mYellowGesso(nullptr)
    , mGreenGesso(nullptr)
    , mSandBombBaseMatTable(nullptr)
    , mMirrorMatTable(nullptr)
    , mCogwheelRopeTexture(nullptr)
    , mBridgeRopeTexture(nullptr)
{
	gpMapObjManager = this;
	unkD0.zero();
	initKeyCode();
	unk44.zero();

	unkA8.r = 0xff;
	unkA8.g = 0xb4;
	unkA8.b = 0xff;
	unkA8.a = 0xff;

	unkB0.r = 0xff;
	unkB0.g = 0xff;
	unkB0.b = 0x7d;
	unkB0.a = 0xff;

	unkB8.r = 0xb4;
	unkB8.g = 0xff;
	unkB8.b = 0xb4;
	unkB8.a = 0xff;
}

bool TMapObjBaseManager::canAppear(const TMapObjBase* param_1,
                                   u32 param_2) const
{
	if (param_1->isActorType(param_2)
	    && !param_1->checkMapObjFlag(TMapObjBase::MAP_OBJ_FLAG_RESPAWNING)
	    && param_1->checkLiveFlag(LIVE_FLAG_DEAD)
	    && (!param_1->isActorType(ACTOR_TYPE_COIN)
	        || param_1->getMActor() != nullptr))
		return true;

	return false;
}

TMapObjBase* TMapObjBaseManager::makeObjAppear(f32 x, f32 y, f32 z, u32 param_4,
                                               bool param_5)
{
	f32 y2;
	if (param_5) {
		const TBGCheckData* checkData;
		y2 = gpMap->checkGround(x, y + 5.0f, z, &checkData);
		if (checkData->isIllegalData())
			return nullptr;
	} else {
		y2 = y;
	}

	for (int i = 0; i < getObjNum(); ++i) {
		TMapObjBase* obj = (TMapObjBase*)TObjManager::getObj(i);

		if (canAppear(obj, param_4)) {
			obj->mPosition.set(x, y2, z);
			obj->appear();
			return obj;
		}
	}

	return nullptr;
}

TMapObjBase* TMapObjBaseManager::makeObjAppear(u32 param_1)
{
	for (int i = 0; i < getObjNum(); ++i) {
		TMapObjBase* obj = (TMapObjBase*)getObj(i);

		if (canAppear(obj, param_1)) {
			obj->appear();
			return obj;
		}
	}

	return nullptr;
}

TMapObjBase* TMapObjBaseManager::makeObjAppeared(u32 param_1)
{
	for (int i = 0; i < getObjNum(); ++i) {
		TMapObjBase* obj = (TMapObjBase*)getObj(i);

		if (canAppear(obj, param_1)) {
			obj->makeObjAppeared();
			return obj;
		}
	}

	return nullptr;
}

static TMapObjBase* newItemByName(const char* name)
{
	static const char* item_names[]
	    = { "mario_cap",           "bottle_large",  "bottle_short",
		    "GesoSurfBoardStatic", "GesoSurfBoard", nullptr };

	for (int i = 0; item_names[i]; ++i)
		if (strcmp(name, item_names[i]) == 0)
			return new TItem(name);

	return nullptr;
}

static TMapObjBase* newUniqueObjByName(const char* name)
{
	if (strcmp(name, "FruitCoconut") == 0)
		return new TResetFruit("無限フルーツ");
	else if (strcmp(name, "FruitPine") == 0)
		return new TResetFruit("無限フルーツ");
	else if (strcmp(name, "FruitDurian") == 0)
		return new TResetFruit("無限フルーツ");
	else if (strcmp(name, "FruitPapaya") == 0)
		return new TResetFruit("無限フルーツ");
	else if (strcmp(name, "FruitBanana") == 0)
		return new TResetFruit("無限フルーツ");
	else if (strcmp(name, "coin") == 0)
		return gpItemManager->unk78;
	else if (strcmp(name, "coin_red") == 0)
		return new TCoinRed("赤コイン");
	else if (strcmp(name, "coin_blue") == 0)
		return new TCoinBlue("青コイン");
	else if (strcmp(name, "normal_nozzle_item") == 0)
		return new TItemNozzle;
	else if (strcmp(name, "back_nozzle_item") == 0)
		return new TItemNozzle;
	else if (strcmp(name, "rocket_nozzle_item") == 0)
		return new TItemNozzle;
	else if (strcmp(name, "yoshi_whistle_item") == 0)
		return new TItemNozzle;
	else if (strcmp(name, "breakable_block") == 0)
		return new TBreakableBlock;
	else if (strcmp(name, "NormalBlock") == 0)
		return new TMapObjBase("地形オブジェ基底");
	else if (strcmp(name, "JuiceBlock") == 0)
		return new TJuiceBlock;
	else if (strcmp(name, "TelesaBlock") == 0)
		return new TTelesaBlock;
	else if (strcmp(name, "lean_block") == 0)
		return new TLeanBlock("傾くブロック");
	else if (strcmp(name, "crane_cargo") == 0)
		return new TCraneCargo;
	else if (strcmp(name, "craneCargoUpDown") == 0)
		return new TMapObjBase("craneCargoUpDown");
	else if (strcmp(name, "SandBirdBlock") == 0)
		return new TMapObjBase("地形オブジェ基底");
	else if (strcmp(name, "joint_coin") == 0)
		return new TCoin("コイン");
	else if (strcmp(name, "sand_bird_test") == 0)
		return new TItem("アイテム");
	else if (strcmp(name, "fence_revolve_inner") == 0)
		return new TRevolvingFenceInner;
	else if (strcmp(name, "mushroom1up") == 0)
		return new TMushroom1up(0, "１ＵＰキノコ");
	else if (strcmp(name, "mushroom1upR") == 0)
		return new TMushroom1up(1, "１ＵＰキノコ");
	else if (strcmp(name, "mushroom1upX") == 0)
		return new TMushroom1up(2, "１ＵＰキノコ");
	else if (strcmp(name, "no_data") == 0)
		return new TMapObjBase("地形オブジェ基底");
	else if (strcmp(name, "maregate") == 0)
		return new TMapObjBase("地形オブジェ基底");
	else if (strcmp(name, "bigWindmillBlock") == 0)
		return new TMapObjBase("風車の台");
	else if (strcmp(name, "SandLeaf") == 0)
		return new TSandLeaf;
	else if (strcmp(name, "SandBomb") == 0)
		return new TSandBomb;
	else if (strcmp(name, "FerrisGondola") == 0)
		return new TMapObjBase("観覧車ゴンドラ");
	else if (strcmp(name, "merry_pole") == 0)
		return new TMerryPole;
	else if (strcmp(name, "GateManta") == 0)
		return new TMapObjBase("GateManta");
	else if (strcmp(name, "merry_egg") == 0)
		return new TMapObjBase("メリーゴーランド用たまご");
	else if (strcmp(name, "ChangeStage") == 0)
		return new TMapObjChangeStage;
	else if (strcmp(name, "ChangeStageMerrygoround") == 0)
		return new TChangeStageMerrygoround;
	else if (strcmp(name, "cogwheel_plate") == 0)
		return new TCogwheelScale("天秤皿");
	else if (strcmp(name, "cogwheel_pot") == 0)
		return new TCogwheelScale("天秤ポット");
	else if (strcmp(name, "HangingBridgeBoard") == 0)
		return new THangingBridgeBoard("つり橋の板");
	else if (strcmp(name, "PinnaHangingBridgeBoard") == 0)
		return new THangingBridgeBoard("つり橋の板");
	else if (strcmp(name, "bambooFence_revolve_inner") == 0)
		return new TRevolvingFenceInner("竹フェンス内側");
	else
		return nullptr;
}

TMapObjBase* TMapObjBaseManager::newAndRegisterObj(
    const char* param_1, const JGeometry::TVec3<f32>& param_2,
    const JGeometry::TVec3<f32>& param_3, const JGeometry::TVec3<f32>& param_4)
{
	TMapObjBase* ret = newItemByName(param_1);
	if (!ret)
		ret = newUniqueObjByName(param_1);

	if (ret->isActorType(ACTOR_TYPE_COIN))
		return ret;

	ret->mPosition = param_2;
	ret->mRotation = param_3;
	ret->mScaling  = param_4;
	ret->initAndRegister(param_1);

	return ret;
}

TMapObjBase* TMapObjBaseManager::newAndRegisterObjByEventID(u32 event_id,
                                                            const char* name)
{
	TMapObjBase* ret = TItemManager::newAndRegisterCoin(event_id);
	if (ret)
		return ret;

	switch (event_id) {
	case 777: {
		char buffer[256];
		snprintf(buffer, sizeof(buffer), "シャイン（%s）", name);
		return static_cast<TMapObjBase*>(JDrama::TNameRefGen::search(buffer));
	} break;

	case 1000:
		ret = newAndRegisterObj("FruitBanana");
		break;
	case 1001:
		ret = newAndRegisterObj("FruitDurian");
		break;
	case 1002:
		ret = newAndRegisterObj("FruitPapaya");
		break;
	case 1003:
		ret = newAndRegisterObj("FruitPine");
		break;
	case 1004:
		ret = newAndRegisterObj("FruitCoconut");
		break;

	case 2000:
		ret = newAndRegisterObj("mushroom1up");
		break;
	case 2001:
		ret = newAndRegisterObj("mushroom1upR");
		break;

	default:
		return nullptr;
	}

	return ret;
}

u32 TMapObjBaseManager::getActorTypeByEventID(u32 param_1)
{
	if (param_1 < 50)
		return ACTOR_TYPE_COIN;

	switch (param_1) {
	case 100:
		return ACTOR_TYPE_COIN;
	case 200:
		return ACTOR_TYPE_COIN_RED;
	case 777:
		return ACTOR_TYPE_SHINE;
	case 1000:
		return ACTOR_TYPE_FRUIT_BANANA;
	case 1001:
		return ACTOR_TYPE_FRUIT_DURIAN;
	case 1002:
		return ACTOR_TYPE_FRUIT_PAPAYA;
	case 1003:
		return ACTOR_TYPE_FRUIT_PINE;
	case 1004:
		return ACTOR_TYPE_FRUIT_COCONUT;
	case 2000:
		return ACTOR_TYPE_MUSHROOM1UP;
	case 2001:
		return ACTOR_TYPE_MUSHROOM1UP;
	default:
		return 0;
	}
}

void TMapObjBaseManager::clipActors(JDrama::TGraphics* param_1)
{
	if (!(unk30 & 2))
		clipActorsAux(param_1, mClipFar, mActorRadius);
}

void TMapObjBaseManager::createModelData()
{
	static const TModelDataLoadEntry entry = { nullptr, 0, 0 };
	createModelDataArray(&entry);
}

int TMapObjBaseManager::getObjNumWithActorType(u32 param_1) const
{
	int result = 0;
	for (int i = 0; i < mObjNum; ++i)
		if (unk18[i]->isActorType(param_1))
			++result;
	return result;
}

void TMapObjBaseManager::load(JSUMemoryInputStream& stream)
{
	TLiveManager::load(stream);
	stream >> mClipFar;
	stream >> mActorRadius;
}

TMapObjBaseManager::TMapObjBaseManager(const char* name)
    : TLiveManager(name)
    , mClipFar(0.0f)
    , mActorRadius(0.0f)
{
}
