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

static TMapObjBase* newItemByName(const char*);
static TMapObjBase* newUniqueObjByName(const char*);

void TMapObjManager::entryStaticDrawBufferShadow(J3DModel*) { }

void TMapObjManager::entryStaticDrawBufferSun(J3DModel*) { }

void TMapObjManager::loadAfter()
{
	TMapObjBaseManager::loadAfter();
	unk38 = 100000.0f;
	unk3C = 1000.0f;

	TMapObjBase* pTVar1 = newItemByName("NormalBlock");
	if (!pTVar1)
		pTVar1 = newUniqueObjByName("NormalBlock");

	if (pTVar1->isActorType(0x2000000E)) {
		pTVar1->mPosition.x = 0.0f;
		pTVar1->mPosition.y = 0.0f;
		pTVar1->mPosition.z = 0.0f;
		pTVar1->mRotation.x = 0.0f;
		pTVar1->mRotation.y = 0.0f;
		pTVar1->mRotation.z = 0.0f;
		pTVar1->mScaling.x  = 1.0f;
		pTVar1->mScaling.y  = 1.0f;
		pTVar1->mScaling.z  = 1.0f;
		pTVar1->initAndRegister("NormalBlock");
	}

	for (int i = 0; i < 10; ++i) {
		TMapObjBase* pTVar1 = newItemByName("JuiceBlock");
		if (!pTVar1)
			pTVar1 = newUniqueObjByName("JuiceBlock");

		if (pTVar1->isActorType(0x2000000E)) {
			pTVar1->mPosition.x = 0.0f;
			pTVar1->mPosition.y = 0.0f;
			pTVar1->mPosition.z = 0.0f;
			pTVar1->mRotation.x = 0.0f;
			pTVar1->mRotation.y = 0.0f;
			pTVar1->mRotation.z = 0.0f;
			pTVar1->mScaling.x  = 1.0f;
			pTVar1->mScaling.y  = 1.0f;
			pTVar1->mScaling.z  = 1.0f;
			pTVar1->initAndRegister("JuiceBlock");
		}
	}
}

void TMapObjManager::initDrawBuffer() { }

void TMapObjManager::loadMatTable(const char*) { }

void TMapObjManager::load(JSUMemoryInputStream&) { }

TMapObjManager::TMapObjManager(const char* name)
    : TMapObjBaseManager(name)
{
}

void TMapObjBaseManager::canAppear(const TMapObjBase*, u32) const { }

void TMapObjBaseManager::makeObjAppear(f32, f32, f32, u32, bool) { }

void TMapObjBaseManager::makeObjAppear(u32) { }

void TMapObjBaseManager::makeObjAppeared(u32) { }

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
		return new TMapObjBase("すなやまの芽");
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
		return new TRevolvingFenceInner;
	else
		return nullptr;
}

TMapObjBase* TMapObjBaseManager::newAndRegisterObj(const char*,
                                                   const JGeometry::TVec3<f32>&,
                                                   const JGeometry::TVec3<f32>&,
                                                   const JGeometry::TVec3<f32>&)
{
}

void TMapObjBaseManager::newAndRegisterObjByEventID(u32, const char*) { }

void TMapObjBaseManager::getActorTypeByEventID(u32) { }

void TMapObjBaseManager::clipActors(JDrama::TGraphics*) { }

void TMapObjBaseManager::createModelData() { }

void TMapObjBaseManager::getObjNumWithActorType(u32) const { }

void TMapObjBaseManager::load(JSUMemoryInputStream&) { }

TMapObjBaseManager::TMapObjBaseManager(const char* name)
    : TLiveManager(name)
{
}
