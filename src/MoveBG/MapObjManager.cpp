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
	unk38 = 100000.0f;
	unk3C = 1000.0f;

	newAndRegisterObj("NormalBlock");

	for (int i = 0; i < 10; ++i) {
		newAndRegisterObj("JuiceBlock");
	}
}

void TMapObjManager::initDrawBuffer()
{
	mDrawBufferSunOpa = JDrama::TNameRefGen::search<JDrama::TDrawBufObj>(
	    "DrawBuf StaticMapObj SunOpa");
	mDrawBufferSunXlu = JDrama::TNameRefGen::search<JDrama::TDrawBufObj>(
	    "DrawBuf StaticMapObj SunXlu");
	mDrawBufferShadowOpa = JDrama::TNameRefGen::search<JDrama::TDrawBufObj>(
	    "DrawBuf StaticMapObj ShadowOpa");
	mDrawBufferShadowXlu = JDrama::TNameRefGen::search<JDrama::TDrawBufObj>(
	    "DrawBuf StaticMapObj ShadowXlu");
	mDrawBufferAfterIndirectOpa
	    = JDrama::TNameRefGen::search<JDrama::TDrawBufObj>(
	        "DrawBuf AfterIndirect Opa");
	mDrawBufferAfterIndirectXlu
	    = JDrama::TNameRefGen::search<JDrama::TDrawBufObj>(
	        "DrawBuf AfterIndirect Xlu");
}

J3DMaterialTable* TMapObjManager::loadMatTable(const char* name)
{
	void* res = JKRGetResource(name);
	return res ? J3DModelLoaderDataBase::loadMaterialTable(res) : nullptr;
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
	     && (SMSGetMarDirector()->unk7D == 1
	         || SMSGetMarDirector()->unk7D == 5))
	    || gpMarDirector->getCurrentMap() == 0x1E) {
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
		TMapObjBase::initPacketMatColor(mRedGesso->mModel, GX_TEVREG1, &unkA8);
		TMapObjBase::initPacketMatColor(mYellowGesso->mModel, GX_TEVREG1,
		                                &unkB0);
		TMapObjBase::initPacketMatColor(mGreenGesso->mModel, GX_TEVREG1,
		                                &unkB8);
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
    , mDrawBufferSunOpa(nullptr)
    , mDrawBufferSunXlu(nullptr)
    , mDrawBufferShadowOpa(nullptr)
    , mDrawBufferShadowXlu(nullptr)
    , mDrawBufferAfterIndirectOpa(nullptr)
    , mDrawBufferAfterIndirectXlu(nullptr)
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
    , mSurfGessoModelData(nullptr)
    , mRedGesso(nullptr)
    , mYellowGesso(nullptr)
    , mGreenGesso(nullptr)
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

bool TMapObjBaseManager::canAppear(const TMapObjBase* param_1,
                                   u32 param_2) const
{
	if (param_1->isActorType(param_2)
	    && !param_1->checkMapObjFlag(TMapObjBase::MAP_OBJ_FLAG_RESPAWNING)
	    && param_1->checkLiveFlag(LIVE_FLAG_DEAD)
	    && (!param_1->isActorType(0x2000000E)
	        || param_1->getMActor() != nullptr))
		return true;

	return false;
}

TMapObjBase* TMapObjBaseManager::makeObjAppear(f32 x, f32 y, f32 z, u32 param_4,
                                               bool param_5)
{
	// TODO: instruction-exact and frame-exact (0x58); the only difference is
	// `checkData`'s slot, 0x34 here and 0x30 in the ROM.
	//
	// Batch 114 localised it exactly. `checkData` is the *topmost* item of
	// the local area and everything else grows below it, so its offset is a
	// direct byte count of the rest: the loop costs 24 (measured by deleting
	// this block: frame 0x40), the ground-check call 12, `checkData` itself
	// 4, and the flag test 4. The ROM pays 24 + 12 + 4 and nothing for the
	// flag test, which is the whole gap.
	//
	// The 4 bytes are the binding of a bool-returning inlined member call
	// whose receiver is a *fresh stack load*: dropping to the raw
	// `checkData->mFlags & BG_CHECK_FLAG_ILLEGAL` test puts the slot at 0x30
	// at once (but loses the `li 1 / b / li 0 / clrlwi.` materialisation the
	// ROM has, so it is not the answer). `canAppear()` in the loop below
	// materialises the same bool for free because its receiver lives in a
	// register.
	//
	// Measured and rejected, all still 0x34 with the frame unmoved:
	// `isIllegalData()`, `(*checkData).checkFlag()`, a TU-local
	// `static bool isIllegalGround(const TBGCheckData*)` and the same taking
	// a `const TBGCheckData&`, a `const TBGCheckData&` local bound before
	// the test, a named `bool illegal` over the spelled-out ternary, a
	// non-const `checkData`, function- vs block-scope `checkData`,
	// `SMSGetMap()` vs `gpMap`, `f32 y2 = y` with no `else`, the inverted
	// `if (!param_5)`, a named `f32 groundY`, and writing the three position
	// components instead of `set()` (all +0).
	// Levers that move it the wrong way: a named local anywhere in the body
	// is +8 of frame and never moves the slot (declared after `checkData` it
	// moves it *up*); `!isLegal()` +8; `== true` +8; raw `unk18[i]` for
	// `getObj(i)` is -8, four too many, and no +4 was found to pair with it.
	f32 y2;
	const TBGCheckData* checkData;
	if (param_5) {
		y2 = gpMap->checkGround(x, y + 5.0f, z, &checkData);
		if (checkData->checkFlag(BG_CHECK_FLAG_ILLEGAL))
			return nullptr;
	} else {
		y2 = y;
	}

	for (int i = 0; i < getObjNum(); ++i) {
		TMapObjBase* obj = (TMapObjBase*)getObj(i);

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
		// TODO: header items (measured, zero regressions, DOL unchanged;
		// all three together take this function 97.25 -> 100 and restore
		// the map's local set<f>__Q29JGeometry8TVec3<f>Ffff):
		//  1. include/MoveBG/MapObjBlock.hpp - TTelesaBlock's ctor takes a
		//     defaulted name (`TTelesaBlock(const char* name =
		//     "テレサブロック") : TJuiceBlock(name)`). The defaulted
		//     argument is one extra inline level, which is what puts the
		//     3-statement TVec3::set<f> at depth 4 and makes it the ROM's
		//     `bl`, and it also binds the `this` the ROM spills to 0x30(r1).
		//  2. include/MoveBG/MapObjPinna.hpp - TMerryPole::unk138 is
		//     TPosition3f, not TMtx34f: the ROM `bl`s the empty
		//     SMatrix34C<f> ctor, i.e. that ctor sits at depth 5.
		//  3. include/JSystem/JGeometry/JGMatrix34.hpp - identity()'s nine
		//     zero stores are ONE assignment chain, not four statements;
		//     the ROM keeps every zero in f0 where four statements need two
		//     `fmr`s. Written order is the reverse of the store order:
		//     ref(1,0) = ref(2,0) = ref(0,1) = ref(2,1) = ref(0,2) =
		//     ref(1,2) = ref(0,3) = ref(1,3) = ref(2,3) = 0.0f;
		//     (also closes TMapObjFlag::TMapObjFlag).
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

TMapObjBase* TMapObjBaseManager::newAndRegisterObjByEventID(u32 event_id,
                                                            const char* name)
{
	TMapObjBase* ret = TItemManager::newAndRegisterCoin(event_id);
	if (ret)
		return ret;

	switch (event_id) {
	case 777: {
		char buffer[256];
		snprintf(buffer, 256, "シャイン（%s）", name);
		return (TMapObjBase*)JDrama::TNameRefGen::search2(buffer);
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

u32 TMapObjBaseManager::getObjNumWithActorType(u32 param_1) const
{
	u32 result = 0;
	for (int i = 0; i < mObjNum; ++i)
		if (unk18[i]->isActorType(param_1))
			++result;
	return result;
}

void TMapObjBaseManager::load(JSUMemoryInputStream& stream)
{
	TLiveManager::load(stream);
	stream >> unk38;
	stream >> unk3C;
}

TMapObjBaseManager::TMapObjBaseManager(const char* name)
    : TLiveManager(name)
    , unk38(0.0f)
    , unk3C(0.0f)
{
}
