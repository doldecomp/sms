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
	j3dSys.setDrawBuffer(unk58->getDrawBuffer(), 0);
	j3dSys.setDrawBuffer(unk5C->getDrawBuffer(), 1);
	model->entry();
}

void TMapObjManager::entryStaticDrawBufferSun(J3DModel* model)
{
	j3dSys.setDrawBuffer(unk50->getDrawBuffer(), 0);
	j3dSys.setDrawBuffer(unk54->getDrawBuffer(), 1);
	model->entry();
}

void TMapObjManager::loadAfter()
{
	TMapObjBaseManager::loadAfter();
	unk38 = 100000.0f;
	unk3C = 1000.0f;

	newAndRegisterObj("NormalBlock");

	for (int i = 0; i < 10; ++i) {
		newAndRegisterObj("JuiceBlock");
	}
}

void TMapObjManager::initDrawBuffer()
{
	unk50 = JDrama::TNameRefGen::search<JDrama::TDrawBufObj>(
	    "DrawBuf StaticMapObj SunOpa");
	unk54 = JDrama::TNameRefGen::search<JDrama::TDrawBufObj>(
	    "DrawBuf StaticMapObj SunXlu");
	unk58 = JDrama::TNameRefGen::search<JDrama::TDrawBufObj>(
	    "DrawBuf StaticMapObj ShadowOpa");
	unk5C = JDrama::TNameRefGen::search<JDrama::TDrawBufObj>(
	    "DrawBuf StaticMapObj ShadowXlu");
	unk60 = JDrama::TNameRefGen::search<JDrama::TDrawBufObj>(
	    "DrawBuf AfterIndirect Opa");
	unk64 = JDrama::TNameRefGen::search<JDrama::TDrawBufObj>(
	    "DrawBuf AfterIndirect Xlu");
}

J3DMaterialTable* TMapObjManager::loadMatTable(const char* name)
{
	void* res = JKRGetResource(name);
	if (res)
		return J3DModelLoaderDataBase::loadMaterialTable(res);
	else
		return nullptr;
}

void TMapObjManager::load(JSUMemoryInputStream& stream)
{
	TMapObjBaseManager::load(stream);
	unk40 = new MActorAnmData;

	unk40->init("/common/map", nullptr);

	initDrawBuffer();

	unk68 = loadMatTable("/scene/map/map/sky.bmt");
	unk6C = loadMatTable("/scene/mapObj/nozzleItem.bmt");
	unk70 = loadMatTable("/scene/mapObj/nozzleBox.bmt");
	unk74 = loadMatTable("/scene/mapObj/flower.bmt");
	unk78 = loadMatTable("/scene/mapObj/ArrowBoard.bmt");
	unk7C = loadMatTable("/scene/mapObj/kibako.bmt");
	unk80 = loadMatTable("/scene/mapObj/barrel.bmt");
	unk84 = loadMatTable("/scene/mapObj/BrickBlock.bmt");
	unk88 = loadMatTable("/scene/mapObj/WaterMelonBlock.bmt");
	if (gpMarDirector->getCurrentMap() == 2)
		unk8C = loadMatTable("/scene/mapObj/bianco.bmt");
	unk90 = loadMatTable("/scene/mapObj/LeafBoat.bmt");
	unk94 = loadMatTable("/scene/mapObj/riccoShip.bmt");

	if ((gpMarDirector->getCurrentMap() == 3
	     && (gpMarDirector->_07D == 1 || gpMarDirector->_07D == 5))
	    || gpMarDirector->getCurrentMap() == 0x1E) {
		unk98 = SMS_MakeSDLModelData("/scene/mapObj/surfgeso.bmd", 0x10220000);
		unk9C = SMS_MakeMActorFromSDLModelData(unk98, getMActorAnmData(), 3);
		unkA0 = SMS_MakeMActorFromSDLModelData(unk98, getMActorAnmData(), 3);
		unkA4 = SMS_MakeMActorFromSDLModelData(unk98, getMActorAnmData(), 3);
		TMapObjBase::initPacketMatColor(unk9C->unk4, GX_TEVREG1, &unkA8);
		TMapObjBase::initPacketMatColor(unkA0->unk4, GX_TEVREG1, &unkB0);
		TMapObjBase::initPacketMatColor(unkA4->unk4, GX_TEVREG1, &unkB8);
	}

	unkC0 = loadMatTable("/scene/mapObj/SandBombBase.bmt");
	unkC4 = loadMatTable("/scene/mapObj/mirror.bmt");

	if (gpMarDirector->getCurrentMap() == 4)
		unkCC = (ResTIMG*)JKRGetResource("/scene/mapObj/mon_bri_rope.bti");
	if (gpMarDirector->getCurrentMap() == 13)
		unkCC = (ResTIMG*)JKRGetResource("/scene/mapObj/mon_bri_rope.bti");
	if (gpMarDirector->getCurrentMap() == 9)
		unkC8 = (ResTIMG*)JKRGetResource("/scene/mapObj/cogwheel_rope.bti");
	if (gpMarDirector->getCurrentMap() == 8)
		unkCC = (ResTIMG*)JKRGetResource("/scene/mapObj/mon_bri_rope.bti");
}

TMapObjManager::TMapObjManager(const char* name)
    : TMapObjBaseManager(name)
    , unk40(nullptr)
    , unk50(nullptr)
    , unk54(nullptr)
    , unk58(nullptr)
    , unk5C(nullptr)
    , unk60(nullptr)
    , unk64(nullptr)
    , unk68(nullptr)
    , unk6C(nullptr)
    , unk70(nullptr)
    , unk74(nullptr)
    , unk78(nullptr)
    , unk7C(nullptr)
    , unk80(nullptr)
    , unk84(nullptr)
    , unk88(nullptr)
    , unk8C(nullptr)
    , unk90(nullptr)
    , unk94(nullptr)
    , unk98(nullptr)
    , unk9C(nullptr)
    , unkA0(nullptr)
    , unkA4(nullptr)
    , unkC0(nullptr)
    , unkC4(nullptr)
    , unkC8(nullptr)
    , unkCC(nullptr)
{
	gpMapObjManager = this;
	unkD0.x = unkD0.y = unkD0.z = 0.0f;
	initKeyCode();
	unk44.x = unk44.y = unk44.z = 0.0f;

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

void TMapObjBaseManager::canAppear(const TMapObjBase*, u32) const { }

TMapObjBase* TMapObjBaseManager::makeObjAppear(f32 x, f32 y, f32 z, u32 param_4,
                                               bool param_5)
{

	f32 y2;
	if (param_5) {
		const TBGCheckData* checkData;
		y2 = gpMap->checkGround(x, y + 5.0f, z, &checkData);
		if (checkData->checkFlag2(0x10))
			return nullptr;
	} else {
		y2 = y;
	}

	for (int i = 0; i < objNum(); ++i) {
		TMapObjBase* obj = (TMapObjBase*)getObj(i);
		bool bVar1;
		if (obj->isActorType(param_4) && !obj->checkMapObjFlag(0x80000)
		    && obj->checkLiveFlag(0x1)
		    && (!obj->isActorType(0x2000000e) || obj->getMActor() != nullptr))
			bVar1 = true;
		else
			bVar1 = false;

		if (bVar1) {
			obj->mPosition.set(x, y2, z);
			obj->appear();
			return obj;
		}
	}

	return nullptr;
}

TMapObjBase* TMapObjBaseManager::makeObjAppear(u32 param_1)
{
	for (int i = 0; i < objNum(); ++i) {
		TMapObjBase* obj = (TMapObjBase*)getObj(i);
		bool bVar1;
		if (obj->isActorType(param_1) && !obj->checkMapObjFlag(0x80000)
		    && obj->checkLiveFlag(0x1)
		    && (!obj->isActorType(0x2000000e) || obj->getMActor() != nullptr))
			bVar1 = true;
		else
			bVar1 = false;

		if (bVar1) {
			obj->appear();
			return obj;
		}
	}

	return nullptr;
}

TMapObjBase* TMapObjBaseManager::makeObjAppeared(u32 param_1)
{
	for (int i = 0; i < objNum(); ++i) {
		TMapObjBase* obj = (TMapObjBase*)getObj(i);
		bool bVar1;
		if (obj->isActorType(param_1) && !obj->checkMapObjFlag(0x80000)
		    && obj->checkLiveFlag(0x1)
		    && (!obj->isActorType(0x2000000e) || obj->getMActor() != nullptr))
			bVar1 = true;
		else
			bVar1 = false;

		if (bVar1) {
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

	if (ret->isActorType(0x2000000E))
		return ret;

	ret->mPosition = param_2;
	ret->mRotation = param_3;
	ret->mScaling  = param_4;
	ret->initAndRegister(param_1);

	return ret;
}

TMapObjBase* TMapObjBaseManager::newAndRegisterObjByEventID(u32 param_1,
                                                            const char* param_2)
{
	TMapObjBase* ret = TItemManager::newAndRegisterCoin(param_1);
	if (ret)
		return ret;

	switch (param_1) {
	case 777: {
		char buffer[64];
		snprintf(buffer, 64, "シャイン（%s）", param_2);
		return JDrama::TNameRefGen::search<TMapObjBase>(buffer);
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
		return 0x2000000E;

	switch (param_1) {
	case 100:
		return 0x2000000E;
	case 200:
		return 0x2000000F;
	case 777:
		return 0x20000013;
	case 1000:
		return 0x40000394;
	case 1001:
		return 0x40000393;
	case 1002:
		return 0x40000391;
	case 1003:
		return 0x40000392;
	case 1004:
		return 0x40000390;
	case 2000:
		return 0x20000005;
	case 2001:
		return 0x20000005;
	default:
		return 0;
	}
}

void TMapObjBaseManager::clipActors(JDrama::TGraphics* param_1)
{
	if (!(unk30 & 2))
		clipActorsAux(param_1, unk38, unk3C);
}

void TMapObjBaseManager::createModelData()
{
	static const TModelDataLoadEntry entry = { nullptr, 0, 0 };
	createModelDataArray(&entry);
}

int TMapObjBaseManager::getObjNumWithActorType(u32 param_1) const
{
	int result = 0;
	for (int i = 0; i < unk14; ++i)
		if (unk18[i]->isActorType(param_1))
			++result;
	return result;
}

void TMapObjBaseManager::load(JSUMemoryInputStream& stream)
{
	TLiveManager::load(stream);
	stream.read(&unk38, sizeof(unk38));
	stream.read(&unk3C, sizeof(unk3C));
}

TMapObjBaseManager::TMapObjBaseManager(const char* name)
    : TLiveManager(name)
    , unk38(0.0f)
    , unk3C(0.0f)
{
}
