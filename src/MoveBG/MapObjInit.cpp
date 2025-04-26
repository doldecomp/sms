#include <MoveBG/MapObjManager.hpp>
#include <MoveBG/MapObjGeneral.hpp>
#include <MoveBG/MapObjBase.hpp>
#include <Map/MapCollisionManager.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <Map/MapData.hpp>
#include <Map/Map.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorAnm.hpp>
#include <MarioUtil/ScreenUtil.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/MirrorActor.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTexture.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

static void dummy() { static Vec data_2100 = { 1.0f, 1.0f, 1.0f }; }
static void dummy2() { static Vec data_2100 = { 1.0f, 1.0f, 1.0f }; }
static void dummy3() { static u32 data_2100[] = { 0, 2, 1, 3 }; }

#include <M3DUtil/InfectiousStrings.hpp>

static void dummy4(Vec& v)
{
	v = (Vec) { 0.0f, 0.0f, 0.0f };
	v = (Vec) { 1.0f, 1.0f, 1.0f };
}

TMapObjSoundData TMapObjGeneral::mDefaultSound = {
	{ 0xFFFFFFFF, 0x3802, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	  0xFFFFFFFF, 0xFFFFFFFF, 0x3812, 0xFFFFFFFF },
};

static TMapObjSoundData normal_sound_data = {
	{ 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	  0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF },
};

static const TMapObjAnimDataInfo no_data_anim_info = { 0, 0, nullptr };

static const TMapObjSoundInfo normal_sound_info = { 10, &normal_sound_data };

static const TMapObjHitDataTable no_data_obj_hit_table
    = { 0.0f, 0.0f, 0.0f, 0.0f };

static const TMapObjHitInfo no_data_obj_hit_info
    = { 1, 0, 0.0f, &no_data_obj_hit_table };

static TMapObjData end_data = {
	nullptr,
	0,
	nullptr,
	nullptr,
	&no_data_anim_info,
	&no_data_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000000,
	0x00000000,
};

static TMapObjData no_data = {
	"no_data",
	0x40000023,
	"地形オブジェマネージャー",
	nullptr,
	&no_data_anim_info,
	&no_data_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000000,
	0x00000000,
};

static const TMapObjAnimData billboard_dolphin_anim_data[] = {
	{ "billboardDol.bmd", nullptr, 0, nullptr, nullptr },
	{ nullptr, "billboardrot", 0, nullptr, nullptr },
	{ nullptr, "billboardrotrev", 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo billboard_dolphin_anim_info
    = { 3, 1, billboard_dolphin_anim_data };

static const TMapObjHitDataTable billboard_hit_data_table[] = {
	{ 100.0f, 60.0f, 100.0f, 60.0f },
};

static const TMapObjHitInfo billboard_obj_hit_info
    = { 0x1, 0x80000000, -100.0f, billboard_hit_data_table };

static TMapObjData billboard_dolphin_data = {
	"billboard_dolphin",
	0x4000000C,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&billboard_dolphin_anim_info,
	&billboard_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000800,
	0x00000000,
};

static const TMapObjAnimData billboard_sun_anim_data[] = {
	{ "billboardSun.bmd", nullptr, 0, nullptr, nullptr },
	{ nullptr, "billboardrot", 0, nullptr, nullptr },
	{ nullptr, "billboardrotrev", 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo billboard_sun_anim_info
    = { 3, 1, billboard_sun_anim_data };

static TMapObjData billboard_sun_data = {
	"billboard_sun",
	0x4000000C,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&billboard_sun_anim_info,
	&billboard_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000800,
	0x00000000,
};

static const TMapObjAnimData billboard_restaurant_anim_data[] = {
	{ "billboardRestaurant.bmd", nullptr, 0, nullptr, nullptr },
	{ nullptr, "billboardrot", 0, nullptr, nullptr },
	{ nullptr, "billboardrotrev", 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo billboard_restaurant_anim_info
    = { 3, 1, billboard_restaurant_anim_data };

static TMapObjData billboard_restaurant_data = {
	"billboard_restaurant",
	0x4000000C,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&billboard_restaurant_anim_info,
	&billboard_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000800,
	0x00000000,
};

static const TMapObjAnimData billboard_fish_anim_data[] = {
	{ "billboardFish.bmd", nullptr, 0, nullptr, nullptr },
	{ nullptr, "billboardrot", 0, nullptr, nullptr },
	{ nullptr, "billboardrotrev", 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo billboard_fish_anim_info
    = { 3, 1, billboard_fish_anim_data };

static TMapObjData billboard_fish_data = {
	"billboard_fish",
	0x4000000C,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&billboard_fish_anim_info,
	&billboard_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000800,
	0x00000000,
};

static const TMapObjHitDataTable HideObj_hit_data_table[] = {
	{ 100.0f, 60.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo HideObj_obj_hit_info
    = { 0x1, 0x80000000, -10.0f, HideObj_hit_data_table };

static TMapObjData HideObj_data = {
	"HideObj",
	0x20000011,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&no_data_anim_info,
	&HideObj_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000000,
	0x00000000,
};

static const TMapObjHitDataTable WaterHitHideObj_hit_data_table[] = {
	{ 0.0f, 0.0f, 60.0f, 20.0f },
};

static const TMapObjHitInfo WaterHitHideObj_obj_hit_info
    = { 0x1, 0x00000000, 0.0f, WaterHitHideObj_hit_data_table };

static TMapObjData WaterHitHideObj_data = {
	"WaterHitHideObj",
	0x20000011,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&no_data_anim_info,
	&WaterHitHideObj_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000000,
	0x00000000,
};

static const TMapObjHitDataTable FruitHitHideObj_hit_data_table[] = {
	{ 100.0f, 60.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo FruitHitHideObj_obj_hit_info
    = { 0x1, 0x40000000, 0.0f, FruitHitHideObj_hit_data_table };

static TMapObjData FruitHitHideObj_data = {
	"FruitHitHideObj",
	0x20000011,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&no_data_anim_info,
	&FruitHitHideObj_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000000,
	0x00000000,
};

static const TMapObjHitDataTable HipDropHideObj_hit_data_table[] = {
	{ 100.0f, 60.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo HipDropHideObj_obj_hit_info
    = { 0x1, 0x40000000, 0.0f, HipDropHideObj_hit_data_table };

static TMapObjData HipDropHideObj_data = {
	"HipDropHideObj",
	0x20000011,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&no_data_anim_info,
	&HipDropHideObj_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000000,
	0x00000000,
};

static const TMapObjAnimData MonteChair_anim_data[] = {
	{ "monte_chair_model.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo MonteChair_anim_info
    = { 1, 1, MonteChair_anim_data };

static TMapObjData MonteChair_data = {
	"monte_chair",
	0x40000007,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&MonteChair_anim_info,
	&no_data_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000B00,
	0x00000000,
};

static const TMapObjAnimData door_anim_data[] = {
	{ "door.bmd", nullptr, 0, nullptr, nullptr },
	{ nullptr, "door_open_l", 0, nullptr, "/scene/mapObj/door_open_l.bas" },
	{ nullptr, "door_open_r", 0, nullptr, "/scene/mapObj/door_open_r.bas" },
	{ nullptr, "door_kick_l", 0, nullptr, "/scene/mapObj/door_kick_l.bas" },
	{ nullptr, "door_kick_r", 0, nullptr, "/scene/mapObj/door_kick_r.bas" },
};

static const TMapObjHitDataTable door_hit_data_table[] = {
	{ 150.0f, 250.0f, 150.0f, 250.0f },
};

static const TMapObjHitInfo door_obj_hit_info
    = { 0x1, 0x80000000, 0.0f, door_hit_data_table };

static TMapObjData door_data = {
	"door",
	0x4000002C,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&door_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000800,
	0x00000000,
};

static const TMapObjAnimData doorHotel_anim_data[] = {
	{ "doorHotel.bmd", nullptr, 0, nullptr, nullptr },
	{ nullptr, "door_open_l", 0, nullptr, "/scene/mapObj/door_open_l.bas" },
	{ nullptr, "door_open_r", 0, nullptr, "/scene/mapObj/door_open_r.bas" },
	{ nullptr, "door_kick_l", 0, nullptr, "/scene/mapObj/door_kick_l.bas" },
	{ nullptr, "door_kick_r", 0, nullptr, "/scene/mapObj/door_kick_r.bas" },
};

static const TMapObjAnimDataInfo doorHotel_anim_info
    = { 5, 1, doorHotel_anim_data };

static TMapObjData doorHotel_data = {
	"doorHotel",
	0x4000002C,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&doorHotel_anim_info,
	&door_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000200,
	0x00000000,
};

static const TMapObjAnimData manhole_anim_data[] = {
	{ "manhole.bmd", "manhole", 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo manhole_anim_info
    = { 1, 1, manhole_anim_data };

static const TMapObjHitDataTable manhole_hit_data_table[] = {
	{ 100.0f, 50.0f, 130.0f, 50.0f },
};

static const TMapObjHitInfo manhole_obj_hit_info
    = { 0x1, 0x80000000, 0.0f, manhole_hit_data_table };

static const TMapObjCollisionData manhole_map_collision_data[] = {
	{ "manhole", 2 },
	{ "manhole", 1 },
};

static const TMapObjCollisionInfo manhole_map_collision_info
    = { 2, 2, manhole_map_collision_data };

static TMapObjData manhole_data = {
	"manhole",
	0x4000000B,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&manhole_anim_info,
	&manhole_obj_hit_info,
	&manhole_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000100,
	0x00000000,
};

static const TMapObjAnimData FruitBasket_anim_data[] = {
	{ "Basket.bmd", nullptr, 0, nullptr, nullptr },
	{ nullptr, "basket", 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo FruitBasket_anim_info
    = { 2, 2, FruitBasket_anim_data };

static const TMapObjHitDataTable FruitBasket_hit_data_table[] = {
	{ 50.0f, 100.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo FruitBasket_obj_hit_info
    = { 0x1, 0x40000000, -10.0f, FruitBasket_hit_data_table };

static const TMapObjCollisionData FruitBasket_map_collision_data[] = {
	{ "Basket", 0 },
};

static const TMapObjCollisionInfo FruitBasket_map_collision_info
    = { 1, 1, FruitBasket_map_collision_data };

static TMapObjData FruitBasket_data = {
	"FruitBasket",
	0x40000014,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&FruitBasket_anim_info,
	&FruitBasket_obj_hit_info,
	&FruitBasket_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000000,
	0x00000000,
};

static const TMapObjAnimData BasketReverse_anim_data[] = {
	{ "Basket.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo BasketReverse_anim_info
    = { 1, 1, BasketReverse_anim_data };

static const TMapObjHitDataTable BasketReverse_hit_data_table[] = {
	{ 0.0f, 0.0f, 100.0f, 200.0f },
};

static const TMapObjHitInfo BasketReverse_obj_hit_info
    = { 0x1, 0x00000000, -200.0f, BasketReverse_hit_data_table };

static const TMapObjCollisionData BasketReverse_map_collision_data[] = {
	{ "Basket", 2 },
};

static const TMapObjCollisionInfo BasketReverse_map_collision_info
    = { 1, 1, BasketReverse_map_collision_data };

static TMapObjData BasketReverse_data = {
	"BasketReverse",
	0x4000000A,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&BasketReverse_anim_info,
	&BasketReverse_obj_hit_info,
	&BasketReverse_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000808,
	0x00000000,
};

static const TMapObjHitDataTable CoconutJuice_hit_data_table[] = {
	{ 0.0f, 0.0f, 50.0f, 50.0f },
};

static const TMapObjHitInfo CoconutJuice_obj_hit_info
    = { 0x1, 0x00000000, 0.0f, CoconutJuice_hit_data_table };

static TMapObjData CoconutJuice_data = {
	"CoconutJuice",
	0x40000015,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&CoconutJuice_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000800,
	0x00000000,
};

static const TMapObjHitDataTable Pile_hit_data_table[] = {
	{ 0.0f, 0.0f, 30.0f, 100.0f },
};

static const TMapObjHitInfo Pile_obj_hit_info
    = { 0x1, 0x00000000, 0.0f, Pile_hit_data_table };

static const TMapObjCollisionData Pile_map_collision_data[] = {
	{ "Pile", 0 },
};

static const TMapObjCollisionInfo Pile_map_collision_info
    = { 1, 1, Pile_map_collision_data };

static TMapObjData Pile_data = {
	"Pile",
	0x40000016,
	"大型地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&Pile_obj_hit_info,
	&Pile_map_collision_info,
	&normal_sound_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	80.0f,
	0x0000290A,
	0x00000000,
};

static const TMapObjHitDataTable ChangeStage_hit_data_table[] = {
	{ 100.0f, 100.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo ChangeStage_obj_hit_info
    = { 0x1, 0x80000000, 0.0f, ChangeStage_hit_data_table };

static TMapObjData ChangeStage_data = {
	"ChangeStage",
	0x40000050,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&no_data_anim_info,
	&ChangeStage_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000000,
	0x00000000,
};

static TMapObjData ChangeStageMerrygoround_data = {
	"ChangeStageMerrygoround",
	0x40000050,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&no_data_anim_info,
	&ChangeStage_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000000,
	0x00000000,
};

static const TMapObjHitDataTable StartDemo_hit_data_table[] = {
	{ 100.0f, 100.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo StartDemo_obj_hit_info
    = { 0x1, 0x80000000, 0.0f, StartDemo_hit_data_table };

static TMapObjData StartDemo_data = {
	"StartDemo",
	0x40000051,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&no_data_anim_info,
	&StartDemo_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000000,
	0x00000000,
};

static const TMapObjAnimData ChipShine_anim_data[] = {
	{ "chip_shine_model1.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo ChipShine_anim_info
    = { 1, 1, ChipShine_anim_data };

static const TMapObjHitDataTable ChipShine_hit_data_table[] = {
	{ 0.0f, 0.0f, 150.0f, 300.0f },
};

static const TMapObjHitInfo ChipShine_obj_hit_info
    = { 0x1, 0x00000000, -150.0f, ChipShine_hit_data_table };

static TMapObjData ChipShine_data = {
	"ChipShine",
	0x40000012,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&ChipShine_anim_info,
	&ChipShine_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000900,
	0x00000000,
};

static const TMapObjAnimData KoopaJrSignM_anim_data[] = {
	{ "koopa_jr_sign.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo KoopaJrSignM_anim_info
    = { 1, 1, KoopaJrSignM_anim_data };

static const TMapObjHitDataTable KoopaJrSignM_hit_data_table[] = {
	{ 0.0f, 0.0f, 150.0f, 300.0f },
};

static const TMapObjHitInfo KoopaJrSignM_obj_hit_info
    = { 0x1, 0x00000000, -150.0f, KoopaJrSignM_hit_data_table };

static TMapObjData KoopaJrSignM_data = {
	"KoopaJrSignM",
	0x40000013,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&KoopaJrSignM_anim_info,
	&KoopaJrSignM_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000900,
	0x00000000,
};

static TMapObjData StarSign_data = {
	"star_sign",
	0x40000020,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&KoopaJrSignM_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000900,
	0x00000000,
};

static const TMapObjAnimData SignCircle_anim_data[] = {
	{ "maru_sign.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo SignCircle_anim_info
    = { 1, 1, SignCircle_anim_data };

static TMapObjData SignCircle_data = {
	"SignCircle",
	0x40000018,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&SignCircle_anim_info,
	&KoopaJrSignM_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000900,
	0x00000000,
};

static const TMapObjAnimData SignCross_anim_data[] = {
	{ "batu_sign.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo SignCross_anim_info
    = { 1, 1, SignCross_anim_data };

static TMapObjData SignCross_data = {
	"SignCross",
	0x40000019,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&SignCross_anim_info,
	&KoopaJrSignM_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000900,
	0x00000000,
};

static const TMapObjAnimData SignTriangle_anim_data[] = {
	{ "3kaku_sign.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo SignTriangle_anim_info
    = { 1, 1, SignTriangle_anim_data };

static TMapObjData SignTriangle_data = {
	"SignTriangle",
	0x4000001A,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&SignTriangle_anim_info,
	&KoopaJrSignM_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000900,
	0x00000000,
};

static const TMapObjHitDataTable ArrowBoardLR_hit_data_table[] = {
	{ 0.0f, 0.0f, 120.0f, 300.0f },
};

static const TMapObjHitInfo ArrowBoardLR_obj_hit_info
    = { 0x1, 0x80000000, 0.0f, ArrowBoardLR_hit_data_table };

static TMapObjData ArrowBoardLR_data = {
	"ArrowBoardLR",
	0x4000001B,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&ArrowBoardLR_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000900,
	0x00000000,
};

static const TMapObjHitDataTable ArrowBoardUp_hit_data_table[] = {
	{ 0.0f, 0.0f, 80.0f, 440.0f },
};

static const TMapObjHitInfo ArrowBoardUp_obj_hit_info
    = { 0x1, 0x80000000, 0.0f, ArrowBoardUp_hit_data_table };

static TMapObjData ArrowBoardUp_data = {
	"ArrowBoardUp",
	0x4000001B,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&ArrowBoardUp_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000900,
	0x00000000,
};

static const TMapObjHitDataTable ArrowBoardDown_hit_data_table[] = {
	{ 0.0f, 0.0f, 80.0f, 440.0f },
};

static const TMapObjHitInfo ArrowBoardDown_obj_hit_info
    = { 0x1, 0x80000000, 0.0f, ArrowBoardDown_hit_data_table };

static TMapObjData ArrowBoardDown_data = {
	"ArrowBoardDown",
	0x4000001B,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&ArrowBoardDown_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000900,
	0x00000000,
};

static TMapObjData WaterSprayCylinder_data = {
	"WaterSprayCylinder",
	0x40000025,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&no_data_anim_info,
	&no_data_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000000,
	0x00000000,
};

static TMapObjData WaterSprayBox_data = {
	"WaterSprayBox",
	0x40000025,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&no_data_anim_info,
	&no_data_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000000,
	0x00000000,
};

static const TMapObjCollisionData ObjSwitch_map_collision_data[] = {
	{ "ObjSwitch", 2 },
};

static const TMapObjCollisionInfo ObjSwitch_map_collision_info
    = { 1, 1, ObjSwitch_map_collision_data };

static TMapObjData ObjSwitch_data = {
	"ObjSwitch",
	0x40000021,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	&ObjSwitch_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000201,
	0x00000000,
};

static const TMapObjCollisionData RedCoinSwitch_map_collision_data[] = {
	{ "RedCoinSwitch", 2 },
};

static const TMapObjCollisionInfo RedCoinSwitch_map_collision_info
    = { 1, 1, RedCoinSwitch_map_collision_data };

static TMapObjData RedCoinSwitch_data = {
	"RedCoinSwitch",
	0x40000022,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	&RedCoinSwitch_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000201,
	0x00000000,
};

static TMapObjData bucket_data = {
	"bucket",
	0x4000000E,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000800,
	0x00000000,
};

static const TMapObjHitDataTable GeneralHitObj_hit_data_table[] = {
	{ 0.0f, 0.0f, 50.0f, 100.0f },
};

static const TMapObjHitInfo GeneralHitObj_obj_hit_info
    = { 0x1, 0x80000000, 0.0f, GeneralHitObj_hit_data_table };

static TMapObjData GeneralHitObj_data = {
	"GeneralHitObj",
	0x40000026,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&no_data_anim_info,
	&GeneralHitObj_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000900,
	0x00000000,
};

static const TMapObjHitDataTable item_hit_data_table[] = {
	{ 40.0f, 40.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo item_obj_hit_info
    = { 0x1, 0x80000000, 0.0f, item_hit_data_table };

static TMapObjData bottle_large_data = {
	"bottle_large",
	0x20000002,
	"アイテムマネージャー",
	"アイテムグループ",
	nullptr,
	&item_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	40.0f,
	0x0A040100,
	0x00000000,
};

static TMapObjData bottle_short_data = {
	"bottle_short",
	0x20000001,
	"アイテムマネージャー",
	"アイテムグループ",
	nullptr,
	&item_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	40.0f,
	0x0A040100,
	0x00000000,
};

static const TMapObjHitDataTable WaterRecoverObj_hit_data_table[] = {
	{ 100.0f, 100.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo WaterRecoverObj_obj_hit_info
    = { 0x1, 0x80000000, 0.0f, item_hit_data_table };

static TMapObjData WaterRecoverObj_data = {
	"WaterRecoverObj",
	0x20000002,
	"アイテムマネージャー",
	"アイテムグループ",
	&no_data_anim_info,
	&WaterRecoverObj_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000100,
	0x00000000,
};

static TMapObjData watergun_item_data = {
	"watergun_item",
	0x2000001F,
	"アイテムマネージャー",
	"アイテムグループ",
	nullptr,
	&item_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	40.0f,
	0x04000800,
	0x00000000,
};

static TMapObjPhysicalData nozzle_normal_physical_data
    = { 0.35f, 0.5f, 0.0f, 5.0f, 0.8f, 0.95f, 1.0f,
	    0.0f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

static TMapObjPhysicalInfo nozzle_normal_physical_info
    = { 0xD, &nozzle_normal_physical_data, 0x0 };

static TMapObjData nozzle_normal_item_data = {
	"normal_nozzle_item",
	0x20000026,
	"アイテムマネージャー",
	"オブジェクトグループ",
	nullptr,
	&item_obj_hit_info,
	nullptr,
	nullptr,
	&nozzle_normal_physical_info,
	nullptr,
	nullptr,
	nullptr,
	40.0f,
	0x0A040100,
	0x00000000,
};

static TMapObjData yoshi_whistle_item_data = {
	"yoshi_whistle_item",
	0x20000025,
	"アイテムマネージャー",
	"オブジェクトグループ",
	nullptr,
	&item_obj_hit_info,
	nullptr,
	nullptr,
	&nozzle_normal_physical_info,
	nullptr,
	nullptr,
	nullptr,
	40.0f,
	0x0A040100,
	0x00000000,
};

static const TMapObjHitDataTable nozzle_item_hit_data_table[] = {
	{ 40.0f, 40.0f, 100.0f, 40.0f },
};

static const TMapObjHitInfo nozzle_item_obj_hit_info
    = { 0x1, 0x80000000, 0.0f, nozzle_item_hit_data_table };

static TMapObjData nozzle_rocket_item_data = {
	"rocket_nozzle_item",
	0x20000022,
	"アイテムマネージャー",
	"オブジェクトグループ",
	nullptr,
	&nozzle_item_obj_hit_info,
	nullptr,
	nullptr,
	&nozzle_normal_physical_info,
	nullptr,
	nullptr,
	nullptr,
	40.0f,
	0x0A040100,
	0x00000000,
};

static TMapObjData nozzle_back_item_data = {
	"back_nozzle_item",
	0x2000002A,
	"アイテムマネージャー",
	"オブジェクトグループ",
	nullptr,
	&nozzle_item_obj_hit_info,
	nullptr,
	nullptr,
	&nozzle_normal_physical_info,
	nullptr,
	nullptr,
	nullptr,
	40.0f,
	0x0A040100,
	0x00000000,
};

static TMapObjPhysicalData coin_physical_data
    = { 0.35f, 0.5f, 0.6f, 1.0f, 0.5f, 0.95f, 0.99f,
	    0.0f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

static TMapObjPhysicalInfo coin_physical_info
    = { 0xD, &coin_physical_data, 0x0 };

static const TMapObjHitDataTable coin_hit_data_table[] = {
	{ 70.0f, 100.0f, 70.0f, 100.0f },
};

static const TMapObjHitInfo coin_obj_hit_info
    = { 0x1, 0x80000000, 0.0f, coin_hit_data_table };

static const TMapObjSoundData coin_sound_data
    = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00004842,
	    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF };

static const TMapObjSoundInfo coin_sound_info = { 0xA, &coin_sound_data };

static TMapObjData coin_data = {
	"coin",
	0x2000000E,
	"アイテムマネージャー",
	"アイテムグループ",
	nullptr,
	&coin_obj_hit_info,
	nullptr,
	&coin_sound_info,
	&coin_physical_info,
	nullptr,
	nullptr,
	nullptr,
	50.0f,
	0x08144100,
	0x00000000,
};

static TMapObjData invisible_coin_data = {
	"invisible_coin",
	0x2000000E,
	"アイテムマネージャー",
	"アイテムグループ",
	&no_data_anim_info,
	&coin_obj_hit_info,
	nullptr,
	&normal_sound_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x0C100000,
	0x00000000,
};

static TMapObjData coin_red_data = {
	"coin_red",
	0x2000000F,
	"アイテムマネージャー",
	"オブジェクトグループ",
	nullptr,
	&coin_obj_hit_info,
	nullptr,
	&coin_sound_info,
	&coin_physical_info,
	nullptr,
	nullptr,
	nullptr,
	50.0f,
	0x18104100,
	0x00000000,
};

static TMapObjData coin_blue_data = {
	"coin_blue",
	0x20000010,
	"アイテムマネージャー",
	"オブジェクトグループ",
	nullptr,
	&coin_obj_hit_info,
	nullptr,
	&coin_sound_info,
	&coin_physical_info,
	nullptr,
	nullptr,
	nullptr,
	50.0f,
	0x18104100,
	0x00000000,
};

static TMapObjData shine_data = {
	"shine",
	0x20000013,
	"シャインマネージャー",
	"アイテムグループ",
	&no_data_anim_info,
	&coin_obj_hit_info,
	nullptr,
	&normal_sound_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	50.0f,
	0x00004000,
	0x00000000,
};

static const TMapObjAnimData mario_cap_anim_data[] = {
	{ "mariocap.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo mario_cap_anim_info
    = { 1, 1, mario_cap_anim_data };

static const TMapObjHitDataTable mario_cap_hit_data_table[] = {
	{ 100.0f, 50.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo mario_cap_obj_hit_info
    = { 0x1, 0x80000000, 0.0f, mario_cap_hit_data_table };

static TMapObjData mario_cap_data = {
	"mario_cap",
	0x2000003C,
	"アイテムマネージャー",
	"アイテムグループ",
	&mario_cap_anim_info,
	&mario_cap_obj_hit_info,
	nullptr,
	&normal_sound_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x14100001,
	0x00000000,
};

static const TMapObjAnimData coin_anim_data[] = {
	{ "coin.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo coin_anim_info = { 1, 1, coin_anim_data };

static TMapObjData joint_coin_data = {
	"joint_coin",
	0x2000000E,
	"アイテムマネージャー",
	"オブジェクトグループ",
	&coin_anim_info,
	&item_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	80.0f,
	0x08104100,
	0x00000000,
};

static const TMapObjAnimData eggYoshi_anim_data[] = {
	{ "eggYoshi_normal.bmd", "eggyoshi_wait", 0, nullptr, nullptr },
	{ nullptr, "eggyoshi_true", 0, nullptr, nullptr },
	{ nullptr, "eggyoshi_false", 0, nullptr, nullptr },
	{ "eggYoshi_broken.bmd", "eggyoshi_broken", 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo eggYoshi_anim_info
    = { 4, 2, eggYoshi_anim_data };

static const TMapObjHitDataTable eggYoshi_hit_data_table[] = {
	{ 300.0f, 100.0f, 50.0f, 50.0f }, { 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f, 0.0f },       { 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f, 0.0f },       { 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f, 0.0f },       { 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f, 0.0f },       { 0.0f, 0.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo eggYoshi_obj_hit_info
    = { 0xA, 0xC0000000, 0.0f, eggYoshi_hit_data_table };

static TMapObjData eggYoshi_data = {
	"eggYoshi",
	0x20000009,
	"アイテムマネージャー",
	"オブジェクトグループ",
	&eggYoshi_anim_info,
	&eggYoshi_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	80.0f,
	0x00000000,
	0x00000000,
};

static TMapObjPhysicalData EggYoshi_physical_data
    = { 0.35f, 0.1f, 0.0f, 5.0f, 1.0f, 0.95f, 1.0f,
	    0.0f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

static TMapObjPhysicalInfo EggYoshi_physical_info
    = { 0xD, &EggYoshi_physical_data, 0x0 };

static TMapObjData eggYoshiEvent_data = {
	"eggYoshiEvent",
	0x20000009,
	"アイテムマネージャー",
	"オブジェクトグループ",
	&eggYoshi_anim_info,
	&eggYoshi_obj_hit_info,
	nullptr,
	nullptr,
	&EggYoshi_physical_info,
	nullptr,
	nullptr,
	nullptr,
	80.0f,
	0x00000001,
	0x00000000,
};

static const TMapObjAnimData NozzleBox_anim_data[] = {
	{ "nozzleBox.bmd", nullptr, 0, nullptr, nullptr },
	{ nullptr, nullptr, 0, nullptr, nullptr },
	{ "nozzleBoxCrash.bmd", "nozzleboxcrash", 0, nullptr, nullptr },
	{ "nozzleBoxAlpha.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo NozzleBox_anim_info
    = { 4, 3, NozzleBox_anim_data };

static const TMapObjHitDataTable NozzleBox_hit_data_table[] = {
	{ 200.0f, 200.0f, 100.0f, 200.0f },
};

static const TMapObjHitInfo NozzleBox_obj_hit_info
    = { 0x1, 0x80000000, 0.0f, NozzleBox_hit_data_table };

static TMapObjData NozzleBox_data = {
	"NozzleBox",
	0x20000068,
	"アイテムマネージャー",
	"アイテムグループ",
	&NozzleBox_anim_info,
	&NozzleBox_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x14000000,
	0x00000000,
};

static const TMapObjAnimData mushroom1up_anim_data[] = {
	{ "mushroom1up.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo mushroom1up_anim_info
    = { 1, 1, mushroom1up_anim_data };

static const TMapObjHitDataTable mushroom1up_hit_data_table[] = {
	{ 20.0f, 40.0f, 20.0f, 40.0f },
};

static const TMapObjHitInfo mushroom1up_obj_hit_info
    = { 0x1, 0x00000000, 0.0f, mushroom1up_hit_data_table };

static TMapObjData mushroom1up_data = {
	"mushroom1up",
	0x20000005,
	"アイテムマネージャー",
	"アイテムグループ",
	&mushroom1up_anim_info,
	&mushroom1up_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	40.0f,
	0x00000000,
	0x00000000,
};

static TMapObjData mushroom1upR_data = {
	"mushroom1upR",
	0x20000006,
	"アイテムマネージャー",
	"アイテムグループ",
	&mushroom1up_anim_info,
	&mushroom1up_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	40.0f,
	0x00000000,
	0x00000000,
};

static TMapObjData mushroom1upX_data = {
	"mushroom1upX",
	0x20000007,
	"アイテムマネージャー",
	"アイテムグループ",
	&mushroom1up_anim_info,
	&mushroom1up_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	40.0f,
	0x00000000,
	0x00000000,
};

static const TMapObjAnimData jumpbase_anim_data[] = {
	{ "jumpbase.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo jumpbase_anim_info
    = { 1, 1, jumpbase_anim_data };

static const TMapObjHitDataTable jumpbase_hit_data_table[] = {
	{ 50.0f, 80.0f, 50.0f, 80.0f },
};

static const TMapObjHitInfo jumpbase_obj_hit_info
    = { 0x1, 0x00000000, 0.0f, jumpbase_hit_data_table };

static const TMapObjCollisionData jumpbase_map_collision_data[] = {
	{ "jumpbase", 1 },
};

static const TMapObjCollisionInfo jumpbase_map_collision_info
    = { 1, 1, jumpbase_map_collision_data };

static TMapObjData jumpbase_data = {
	"jumpbase",
	0x40000017,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&jumpbase_anim_info,
	&jumpbase_obj_hit_info,
	&jumpbase_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	100.0f,
	0x00000000,
	0x00000000,
};

static const TMapObjHitDataTable fruit_hit_data_table[] = {
	{ 60.0f, 60.0f, 80.0f, 90.0f },
};

static const TMapObjHitInfo fruit_obj_hit_info
    = { 0x1, 0xDC000000, 0.0f, fruit_hit_data_table };

static TMapObjPhysicalData coconut_physical_data
    = { 0.3f, 0.6f, 0.3f,  0.3f, 0.97f, 0.98f, 1.0f,
	    5.0f, 0.5f, 0.72f, 0.0f, 2.0f,  15.0f };

static TMapObjPhysicalInfo coconut_physical_info
    = { 0xD, &coconut_physical_data, 0x2 };

static const TMapObjHitDataTable coconut_hit_data_table[] = {
	{ 40.0f, 60.0f, 50.0f, 80.0f },
};

static const TMapObjHitInfo coconut_obj_hit_info
    = { 0x1, 0xDC000000, 0.0f, coconut_hit_data_table };

static const TMapObjSoundData fruit_sound_data
    = { 0xFFFFFFFF, 0x00003801, 0xFFFFFFFF, 0x00001807, 0x00003862,
	    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF };

static const TMapObjSoundInfo fruit_sound_info = { 0xA, &fruit_sound_data };

static TMapObjData coconut_data = {
	"FruitCoconut", 0x40000390,        "アイテムマネージャー",
	"敵グループ",   nullptr,           &coconut_obj_hit_info,
	nullptr,        &fruit_sound_info, &coconut_physical_info,
	nullptr,        nullptr,           nullptr,
	40.0f,          0x02130100,        0x00000000,
};

static TMapObjPhysicalData papaya_physical_data
    = { 0.3f, 0.6f, 0.3f,  0.3f, 0.97f, 0.98f, 1.0f,
	    5.0f, 0.5f, 0.72f, 0.0f, 5.0f,  20.0f };

static TMapObjPhysicalInfo papaya_physical_info
    = { 0xD, &papaya_physical_data, 0x2 };

static const TMapObjHitDataTable papaya_hit_data_table[] = {
	{ 40.0f, 60.0f, 50.0f, 80.0f },
};

static const TMapObjHitInfo papaya_obj_hit_info
    = { 0x1, 0xDC000000, 0.0f, papaya_hit_data_table };

static TMapObjData papaya_data = {
	"FruitPapaya", 0x40000391,        "アイテムマネージャー",
	"敵グループ",  nullptr,           &papaya_obj_hit_info,
	nullptr,       &fruit_sound_info, &papaya_physical_info,
	nullptr,       nullptr,           nullptr,
	40.0f,         0x02130100,        0x00000000,
};

static TMapObjPhysicalData pine_physical_data
    = { 0.3f, 0.6f, 0.3f,  0.3f, 0.97f, 0.98f, 1.0f,
	    5.0f, 0.5f, 0.72f, 0.0f, 2.0f,  22.0f };

static TMapObjPhysicalInfo pine_physical_info
    = { 0xD, &pine_physical_data, 0x2 };

static const TMapObjHitDataTable pine_hit_data_table[] = {
	{ 80.0f, 60.0f, 50.0f, 80.0f },
};

static const TMapObjHitInfo pine_obj_hit_info
    = { 0x1, 0xDC000000, 0.0f, pine_hit_data_table };

static TMapObjData pine_data = {
	"FruitPine",  0x40000392,        "アイテムマネージャー",
	"敵グループ", nullptr,           &pine_obj_hit_info,
	nullptr,      &fruit_sound_info, &pine_physical_info,
	nullptr,      nullptr,           nullptr,
	40.0f,        0x02130100,        0x00000000,
};

static const TMapObjAnimData CoverPine_anim_data[] = {
	{ "FruitPine.bmd", nullptr, 0, nullptr, nullptr },
	{ nullptr, nullptr, 0, nullptr, nullptr },
	{ nullptr, nullptr, 0, nullptr, nullptr },
	{ nullptr, nullptr, 0, nullptr, nullptr },
	{ nullptr, nullptr, 0, nullptr, nullptr },
	{ nullptr, nullptr, 0, nullptr, nullptr },
	{ nullptr, nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo CoverPine_anim_info
    = { 7, 7, CoverPine_anim_data };

static const TMapObjHitDataTable cover_pine_hit_data_table[] = {
	{ 0.0f, 0.0f, 40.0f, 180.0f },
};

static const TMapObjHitInfo cover_pine_obj_hit_info
    = { 0x1, 0x00000000, -120.0f, cover_pine_hit_data_table };

static TMapObjData CoverPine_data = {
	"FruitCoverPine",
	0x40000396,
	"アイテムマネージャー",
	"敵グループ",
	&CoverPine_anim_info,
	&cover_pine_obj_hit_info,
	nullptr,
	&fruit_sound_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	40.0f,
	0x00100100,
	0x00000000,
};

static TMapObjPhysicalData durian_physical_data
    = { 0.3f, 0.6f, 0.2f,  0.2f, 0.97f, 0.98f, 1.0f,
	    5.0f, 0.5f, 0.72f, 0.0f, 0.01f, 5.0f };

static TMapObjPhysicalInfo durian_physical_info
    = { 0xD, &durian_physical_data, 0x2 };

static const TMapObjHitDataTable durian_hit_data_table[] = {
	{ 150.0f, 60.0f, 60.0f, 90.0f },
};

static const TMapObjHitInfo durian_obj_hit_info
    = { 0x1, 0xDC000000, 0.0f, durian_hit_data_table };

static const TMapObjSoundData durian_sound_data
    = { 0xFFFFFFFF, 0x00003802, 0xFFFFFFFF, 0xFFFFFFFF, 0x00003862,
	    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00003812, 0xFFFFFFFF };

static const TMapObjSoundInfo durian_sound_info = { 0xA, &durian_sound_data };

static TMapObjData durian_data = {
	"FruitDurian",
	0x40000393,
	"アイテムマネージャー",
	"敵グループ",
	nullptr,
	&durian_obj_hit_info,
	nullptr,
	&durian_sound_info,
	&durian_physical_info,
	nullptr,
	nullptr,
	nullptr,
	40.0f,
	0x00170100,
	0x00000000,
};

static TMapObjPhysicalData banana_physical_data
    = { 0.3f, 0.4f, 0.2f,  1.0f, 0.4f, 0.5f, 1.0f,
	    5.0f, 0.5f, 0.72f, 0.0f, 2.5f, 14.0f };

static TMapObjPhysicalInfo banana_physical_info
    = { 0xD, &banana_physical_data, 0x2 };

static const TMapObjHitDataTable banana_hit_data_table[] = {
	{ 80.0f, 60.0f, 60.0f, 90.0f },
};

static const TMapObjHitInfo banana_obj_hit_info
    = { 0x1, 0xDC000000, 0.0f, banana_hit_data_table };

static TMapObjData banana_data = {
	"FruitBanana", 0x40000394,        "アイテムマネージャー",
	"敵グループ",  nullptr,           &banana_obj_hit_info,
	nullptr,       &fruit_sound_info, &banana_physical_info,
	nullptr,       nullptr,           nullptr,
	40.0f,         0x02130100,        0x00000000,
};

static TMapObjPhysicalData red_pepper_physical_data
    = { 0.3f, 0.6f, 0.3f,  0.3f, 0.97f, 0.98f, 1.0f,
	    5.0f, 0.5f, 0.72f, 0.0f, 5.0f,  12.0f };

static TMapObjPhysicalInfo red_pepper_physical_info
    = { 0xD, &red_pepper_physical_data, 0x2 };

static TMapObjData RedPepper_data = {
	"RedPepper",  0x40000395,        "アイテムマネージャー",
	"敵グループ", nullptr,           &fruit_obj_hit_info,
	nullptr,      &fruit_sound_info, &red_pepper_physical_info,
	nullptr,      nullptr,           nullptr,
	40.0f,        0x02130000,        0x00000000,
};

static TMapObjData fence_normal_data = {
	"fence_normal", 0x40000069, "地形オブジェマネージャー",
	nullptr,        nullptr,    &no_data_obj_hit_info,
	nullptr,        nullptr,    nullptr,
	nullptr,        nullptr,    nullptr,
	0.0f,           0x00000A00, 0x00000000,
};

static const TMapObjAnimData fence3x3_anim_data[] = {
	{ "fence_half.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo fence3x3_anim_info
    = { 1, 1, fence3x3_anim_data };

static TMapObjData fence3x3_data = {
	"fence3x3",
	0x40000069,
	"地形オブジェマネージャー",
	nullptr,
	&fence3x3_anim_info,
	&no_data_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000A00,
	0x00000000,
};

static const TMapObjAnimData fence_revolve_outer_anim_data[] = {
	{ "fence_revolve_outer.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo fence_revolve_outer_anim_info
    = { 1, 1, fence_revolve_outer_anim_data };

static TMapObjData fence_revolve_data = {
	"fence_revolve",
	0x40000069,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&fence_revolve_outer_anim_info,
	&no_data_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000A00,
	0x00000000,
};

static const TMapObjHitDataTable FenceInner_hit_data_table[] = {
	{ 280.0f, 500.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo FenceInner_obj_hit_info
    = { 0x1, 0x90000000, -250.0f, FenceInner_hit_data_table };

static TMapObjData fence_revolve_inner_data = {
	"fence_revolve_inner",
	0x4000006A,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&FenceInner_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x0000090A,
	0x00000000,
};

static TMapObjData fenceInnerGreen_data = {
	"fenceInnerGreen",
	0x4000006A,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&FenceInner_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x0000090A,
	0x00000000,
};

static const TMapObjCollisionData FenceWaterV_map_collision_data[] = {
	{ "FenceWaterV", 1 },
};

static const TMapObjCollisionInfo FenceWaterV_map_collision_info
    = { 1, 1, FenceWaterV_map_collision_data };

static TMapObjData FenceWaterV_data = {
	"FenceWaterV",
	0x4000006B,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	&FenceWaterV_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000804,
	0x00000000,
};

static const TMapObjCollisionData FenceWaterH_map_collision_data[] = {
	{ "FenceWaterH", 2 },
	{ "FenceWaterH_ground", 1 },
};

static const TMapObjCollisionInfo FenceWaterH_map_collision_info
    = { 2, 2, FenceWaterH_map_collision_data };

static TMapObjData FenceWaterH_data = {
	"FenceWaterH",
	0x4000006B,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	&FenceWaterH_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000908,
	0x00000000,
};

static const TMapObjAnimData RailFence_anim_data[] = {
	{ "fence_normal.bmd", nullptr, 0, nullptr, nullptr },
	{ nullptr, nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo RailFence_anim_info
    = { 2, 1, RailFence_anim_data };

static const TMapObjCollisionData RailFence_map_collision_data[] = {
	{ "fence_normal_v_tool", 2 },
	{ "fence_normal_v_tool", 1 },
};

static const TMapObjCollisionInfo RailFence_map_collision_info
    = { 2, 2, RailFence_map_collision_data };

static TMapObjData RailFence_data = {
	"RailFence",
	0x4000006C,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&RailFence_anim_info,
	&no_data_obj_hit_info,
	&RailFence_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000904,
	0x00000000,
};

static TMapObjData bambooFenceRevolveInner_data = {
	"bambooFence_revolve_inner",
	0x4000006A,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&FenceInner_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x0000090A,
	0x00000000,
};

static TMapObjData bambooFenceRevolveOuter_data = {
	"bambooFence_revolve_outer",
	0x40000069,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000A00,
	0x00000000,
};

static const TMapObjAnimData BambooRailFence_anim_data[] = {
	{ "bambooFence_rail.bmd", nullptr, 0, nullptr, nullptr },
	{ nullptr, "bamboofence_rail", 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo BambooRailFence_anim_info
    = { 2, 1, BambooRailFence_anim_data };

static TMapObjData bambooRailFence_data = {
	"bambooRailFence",
	0x4000006C,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&BambooRailFence_anim_info,
	&no_data_obj_hit_info,
	&RailFence_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000904,
	0x00000000,
};

static TMapObjPhysicalData wood_barrel_physical_data
    = { 0.35f, 0.0f, 0.0f, 0.0f, 0.95f, 0.95f, 1.0f,
	    0.0f,  0.0f, 0.0f, 0.0f, 3.5f,  20.0f };

static TMapObjPhysicalInfo wood_barrel_physical_info
    = { 0xD, &wood_barrel_physical_data, 0x0 };

static const TMapObjAnimData wood_barrel_anim_data[] = {
	{ "barrel_normal.bmd", nullptr, 0, nullptr, nullptr },
	{ nullptr, nullptr, 0, nullptr, nullptr },
	{ "barrel_crash.bmd", "barrel_crash", 0, nullptr,
	  "/scene/mapObj/barrel_crash.bas" },
	{ nullptr, nullptr, 0, nullptr, nullptr },
	{ nullptr, nullptr, 0, nullptr, nullptr },
	{ nullptr, "barrel_rot", 0, nullptr, "/scene/mapObj/barrel_rot.bas" },
	{ nullptr, nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo wood_barrel_anim_info
    = { 7, 2, wood_barrel_anim_data };

static const TMapObjHitDataTable wood_barrel_hit_data_table[] = {
	{ 65.0f, 130.0f, 65.0f, 110.0f },
	{ 200.0f, 200.0f, 0.0f, 0.0f },
	{ 65.0f, 150.0f, 65.0f, 130.0f },
};

static const TMapObjHitInfo wood_barrel_obj_hit_info
    = { 0x3, 0xDC000000, 0.0f, wood_barrel_hit_data_table };

static const TMapObjCollisionData wood_barrel_map_collision_data[] = {
	{ "barrel", 2 },
	{ "barrel", 1 },
};

static const TMapObjCollisionInfo wood_barrel_map_collision_info
    = { 2, 2, wood_barrel_map_collision_data };

static const TMapObjSoundData wood_barrel_sound_data
    = { 0xFFFFFFFF, 0x00003802, 0x0000380A, 0xFFFFFFFF, 0xFFFFFFFF,
	    0x0000380B, 0xFFFFFFFF, 0xFFFFFFFF, 0x00003812, 0xFFFFFFFF };

static const TMapObjSoundInfo wood_barrel_sound_info
    = { 0xA, &wood_barrel_sound_data };

static const TMapObjSinkData wood_barrel_sink_data = { 0.25f, 10.0f };

static TMapObjHoldData wood_barrel_hold_data
    = { "/scene/mapObj/barrel_offset.bmd", "barrel_center" };

static TMapObjData wood_barrel_data = {
	"wood_barrel",
	0x4000005A,
	"乗り物マネージャー",
	"敵グループ",
	&wood_barrel_anim_info,
	&wood_barrel_obj_hit_info,
	&wood_barrel_map_collision_info,
	&wood_barrel_sound_info,
	&wood_barrel_physical_info,
	&wood_barrel_sink_data,
	&wood_barrel_hold_data,
	nullptr,
	80.0f,
	0x01390000,
	0x00000000,
};

static TMapObjData wood_barrel_once_data = {
	"wood_barrel_once",
	0x4000005A,
	"乗り物マネージャー",
	"敵グループ",
	&wood_barrel_anim_info,
	&wood_barrel_obj_hit_info,
	&wood_barrel_map_collision_info,
	&wood_barrel_sound_info,
	&wood_barrel_physical_info,
	&wood_barrel_sink_data,
	&wood_barrel_hold_data,
	nullptr,
	80.0f,
	0x01310000,
	0x00000000,
};

static const TMapObjHitDataTable barrel_float_hit_data_table[] = {
	{ 120.0f, 100.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo barrel_float_obj_hit_info
    = { 0x1, 0x80000000, 0.0f, barrel_float_hit_data_table };

static const TMapObjCollisionData barrel_float_map_collision_data[] = {
	{ "barrel_float", 1 },
};

static const TMapObjCollisionInfo barrel_float_map_collision_info
    = { 1, 1, barrel_float_map_collision_data };

static TMapObjData barrel_float_data = {
	"barrel_float",
	0x4000005B,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&barrel_float_obj_hit_info,
	&barrel_float_map_collision_info,
	&normal_sound_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000800,
	0x00000000,
};

static const TMapObjAnimData drum_can_anim_data[] = {
	{ "drum_can_model.bmd", nullptr, 0, nullptr, nullptr },
	{ nullptr, nullptr, 0, nullptr, nullptr },
	{ "drum_can_crash_model.bmd", "drum_can_crash", 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo drum_can_anim_info
    = { 3, 2, drum_can_anim_data };

static const TMapObjHitDataTable drum_can_hit_data_table[] = {
	{ 50.0f, 180.0f, 50.0f, 170.0f },
};

static const TMapObjHitInfo drum_can_obj_hit_info
    = { 0x1, 0x80000000, 0.0f, drum_can_hit_data_table };

static const TMapObjCollisionData drum_can_map_collision_data[] = {
	{ "drum_can", 2 },
	{ "drum_can_sink", 1 },
};

static const TMapObjCollisionInfo drum_can_map_collision_info
    = { 2, 2, drum_can_map_collision_data };

static TMapObjData drum_can_data = {
	"drum_can",
	0x4000005D,
	"地形オブジェマネージャー",
	"敵グループ",
	&drum_can_anim_info,
	&drum_can_obj_hit_info,
	&drum_can_map_collision_info,
	&normal_sound_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	60.0f,
	0x00200800,
	0x00000000,
};

static const TMapObjAnimData barrel_oil_anim_data[] = {
	{ "barrel_oil.bmd", nullptr, 0, nullptr, nullptr },
	{ nullptr, nullptr, 0, nullptr, nullptr },
	{ "barrel_crash.bmd", "barrel_crash", 0, nullptr,
	  "/scene/mapObj/barrel_crash_model.bas" },
	{ nullptr, nullptr, 0, nullptr, nullptr },
	{ nullptr, nullptr, 0, nullptr, nullptr },
	{ "barrel_oil.bmd", "barrel_rot", 0, nullptr,
	  "/scene/mapObj/barrel_rot.bas" },
	{ nullptr, nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo barrel_oil_anim_info
    = { 7, 2, barrel_oil_anim_data };

static const TMapObjHitDataTable barrel_oil_hit_data_table[] = {
	{ 0.0f, 0.0f, 50.0f, 170.0f },
};

static const TMapObjHitInfo barrel_oil_obj_hit_info
    = { 0x1, 0x9C000000, 0.0f, barrel_oil_hit_data_table };

static TMapObjData barrel_oil_data = {
	"barrel_oil",
	0x4000005C,
	"地形オブジェマネージャー",
	"敵グループ",
	&barrel_oil_anim_info,
	&barrel_oil_obj_hit_info,
	&wood_barrel_map_collision_info,
	&wood_barrel_sound_info,
	&wood_barrel_physical_info,
	&wood_barrel_sink_data,
	&wood_barrel_hold_data,
	nullptr,
	80.0f,
	0x01350800,
	0x00000000,
};

static const TMapObjCollisionData warp_block_map_collision_data[] = {
	{ "lower_block", 2 },
};

static const TMapObjCollisionData move_block_center_map_collision_data[] = {
	{ "center_block", 1 },
};

static const TMapObjAnimData breakable_block_anim_data[] = {
	{ "breakable_block.bmd", nullptr, 0, nullptr, nullptr },
	{ nullptr, nullptr, 0, nullptr, nullptr },
	{ "breakable_block_break_model.bmd", "breakable_block_break", 0, nullptr,
	  nullptr },
};

static const TMapObjAnimDataInfo breakable_block_anim_info
    = { 3, 2, breakable_block_anim_data };

static const TMapObjHitDataTable breakable_block_hit_data_table[] = {
	{ 70.0f, 140.0f, 70.0f, 140.0f },
};

static const TMapObjHitInfo breakable_block_obj_hit_info
    = { 0x1, 0x80000000, 0.0f, breakable_block_hit_data_table };

static const TMapObjCollisionData breakable_block_map_collision_data[] = {
	{ "breakable_block", 2 },
};

static const TMapObjCollisionInfo breakable_block_map_collision_info
    = { 1, 1, breakable_block_map_collision_data };

static TMapObjData breakable_block_data = {
	"breakable_block",
	0x400002BC,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&breakable_block_anim_info,
	&breakable_block_obj_hit_info,
	&breakable_block_map_collision_info,
	&normal_sound_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	80.0f,
	0x00002005,
	0x00000000,
};

static const TMapObjHitInfo supermario_block_obj_hit_info
    = { 0x1, 0x80000000, -10.0f, breakable_block_hit_data_table };

static const TMapObjCollisionInfo warp_block_map_collision_info
    = { 1, 1, warp_block_map_collision_data };

static TMapObjData supermario_block_data = {
	"supermario_block",
	0x400002BC,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&breakable_block_anim_info,
	&supermario_block_obj_hit_info,
	&warp_block_map_collision_info,
	&normal_sound_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	80.0f,
	0x00002005,
	0x00000000,
};

static const TMapObjCollisionData move_block_map_collision_data[] = {
	{ "lower_block", 1 },
};

static const TMapObjCollisionInfo move_block_map_collision_info
    = { 1, 1, move_block_map_collision_data };

static TMapObjData move_block_data = {
	"move_block",
	0x400002BD,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&breakable_block_anim_info,
	&breakable_block_obj_hit_info,
	&move_block_map_collision_info,
	&normal_sound_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	80.0f,
	0x00002005,
	0x00000000,
};

static TMapObjData fall_slow_block_data = {
	"fall_slow_block",
	0x400002BD,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&breakable_block_anim_info,
	&breakable_block_obj_hit_info,
	&move_block_map_collision_info,
	&normal_sound_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	80.0f,
	0x00002005,
	0x00000000,
};

static const TMapObjHitDataTable expand_block_hit_data_table[] = {
	{ 50.0f, 140.0f, 50.0f, 140.0f },
};

static const TMapObjHitInfo expand_block_obj_hit_info
    = { 0x1, 0x80000000, -10.0f, expand_block_hit_data_table };

static TMapObjData expand_block_data = {
	"expand_block",
	0x400002BD,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&breakable_block_anim_info,
	&expand_block_obj_hit_info,
	&move_block_map_collision_info,
	&normal_sound_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	80.0f,
	0x00002005,
	0x00000000,
};

static const TMapObjHitDataTable spread_block_hit_data_table[] = {
	{ 50.0f, 150.0f, 50.0f, 150.0f },
};

static const TMapObjHitInfo spread_block_obj_hit_info
    = { 0x1, 0x80000000, -10.0f, spread_block_hit_data_table };

static TMapObjData spread_block_data = {
	"spread_block",
	0x400002BD,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&breakable_block_anim_info,
	&spread_block_obj_hit_info,
	&move_block_map_collision_info,
	&normal_sound_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	80.0f,
	0x00002005,
	0x00000000,
};

static const TMapObjAnimData water_roll_block_anim_data[] = {
	{ "water_roll_block.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo water_roll_block_anim_info
    = { 1, 1, water_roll_block_anim_data };

static const TMapObjHitInfo water_roll_block_obj_hit_info
    = { 0x1, 0x00000000, 0.0f, breakable_block_hit_data_table };

static const TMapObjCollisionData water_roll_block_map_collision_data[] = {
	{ "water_roll_block", 1 },
};

static const TMapObjCollisionInfo water_roll_block_map_collision_info
    = { 1, 1, water_roll_block_map_collision_data };

static TMapObjData water_roll_block_data = {
	"water_roll_block",
	0x400002BE,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&water_roll_block_anim_info,
	&water_roll_block_obj_hit_info,
	&water_roll_block_map_collision_info,
	&normal_sound_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	80.0f,
	0x00000109,
	0x00000000,
};

static const TMapObjAnimData sand_block_anim_data[] = {
	{ "SandBlock.bmd", nullptr, 0, nullptr, nullptr },
	{ nullptr, nullptr, 0, nullptr, nullptr },
	{ "SandBlock.bmd", "SandBlock", 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo sand_block_anim_info
    = { 3, 2, sand_block_anim_data };

static const TMapObjHitDataTable sand_block_hit_data_table[] = {
	{ 100.0f, 140.0f, 100.0f, 140.0f },
};

static const TMapObjHitInfo sand_block_obj_hit_info
    = { 0x1, 0x80000000, -10.0f, sand_block_hit_data_table };

static const TMapObjCollisionData sand_block_map_collision_data[] = {
	{ "SandBlock", 2 },
	{ "SandBlock", 1 },
};

static const TMapObjCollisionInfo sand_block_map_collision_info
    = { 2, 2, sand_block_map_collision_data };

static TMapObjData sand_block_data = {
	"sand_block",
	0x400002BC,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&sand_block_anim_info,
	&sand_block_obj_hit_info,
	&sand_block_map_collision_info,
	&normal_sound_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	80.0f,
	0x00002007,
	0x00000000,
};

static const TMapObjCollisionInfo move_block_center_map_collision_info
    = { 1, 1, move_block_center_map_collision_data };

static TMapObjData water_power_lift_data = {
	"water_power_lift",
	0x400002ED,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&breakable_block_anim_info,
	&breakable_block_obj_hit_info,
	&move_block_center_map_collision_info,
	&normal_sound_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	80.0f,
	0x00002005,
	0x00000000,
};

static TMapObjData water_power_inertial_lift_data = {
	"water_power_inertial_lift",
	0x400002ED,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&breakable_block_anim_info,
	&breakable_block_obj_hit_info,
	&move_block_center_map_collision_info,
	&normal_sound_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	80.0f,
	0x00002005,
	0x00000000,
};

static TMapObjData water_power_ship_data = {
	"water_power_ship",
	0x400002ED,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&breakable_block_anim_info,
	&breakable_block_obj_hit_info,
	&move_block_center_map_collision_info,
	&normal_sound_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	80.0f,
	0x00000109,
	0x00000000,
};

static TMapObjData lean_direct_block_data = {
	"lean_direct_block",
	0x400002BF,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&breakable_block_anim_info,
	&breakable_block_obj_hit_info,
	&move_block_center_map_collision_info,
	&normal_sound_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	80.0f,
	0x00000109,
	0x00000000,
};

static TMapObjData lean_indirect_block_data = {
	"lean_indirect_block",
	0x400002BF,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&breakable_block_anim_info,
	&breakable_block_obj_hit_info,
	&move_block_center_map_collision_info,
	&normal_sound_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	80.0f,
	0x00000109,
	0x00000000,
};

static TMapObjData lean_block_data = {
	"lean_block",
	0x400002BF,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&breakable_block_anim_info,
	&breakable_block_obj_hit_info,
	&move_block_center_map_collision_info,
	&normal_sound_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	80.0f,
	0x00000109,
	0x00000000,
};

static const TMapObjHitDataTable skate_block_hit_data_table[] = {
	{ 40.0f, 130.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo skate_block_obj_hit_info
    = { 0x1, 0x80000000, 0.0f, skate_block_hit_data_table };

static TMapObjData skate_block_data = {
	"skate_block",
	0x400002BD,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&breakable_block_anim_info,
	&skate_block_obj_hit_info,
	&move_block_center_map_collision_info,
	&normal_sound_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	80.0f,
	0x00002005,
	0x00000000,
};

static const TMapObjAnimData MoveCoin_anim_data[] = {
	{ "SandMoveCoin.bmd", "sandmovecoin", 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo MoveCoin_anim_info
    = { 1, 1, MoveCoin_anim_data };

static TMapObjData MoveCoin_data = {
	"MoveCoin",
	0x400002BD,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&MoveCoin_anim_info,
	&no_data_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000001,
	0x00000000,
};

static const TMapObjAnimData cluster_block_anim_data[] = {
	{ "test_cluster.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo cluster_block_anim_info
    = { 1, 1, cluster_block_anim_data };

static TMapObjData cluster_block_data = {
	"cluster_block",
	0x400002BD,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&cluster_block_anim_info,
	&no_data_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000001,
	0x00000000,
};

static const TMapObjCollisionData NormalBlock_map_collision_data[] = {
	{ "NormalBlock", 1 },
};

static const TMapObjCollisionInfo NormalBlock_map_collision_info
    = { 1, 1, NormalBlock_map_collision_data };

static TMapObjData NormalBlock_data = {
	"NormalBlock",
	0x400002C0,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	&NormalBlock_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00002005,
	0x00000000,
};

static const TMapObjHitDataTable ice_block_hit_data_table[] = {
	{ 0.0f, 0.0f, 70.0f, 70.0f },
};

static const TMapObjHitInfo ice_block_obj_hit_info
    = { 0x1, 0x80000000, 0.0f, ice_block_hit_data_table };

static const TMapObjCollisionData IceBlock_map_collision_data[] = {
	{ "IceBlock", 1 },
};

static const TMapObjCollisionInfo IceBlock_map_collision_info
    = { 1, 1, IceBlock_map_collision_data };

static TMapObjData IceBlock_data = {
	"IceBlock",
	0x400002C1,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&ice_block_obj_hit_info,
	&IceBlock_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	80.0f,
	0x0000A004,
	0x00000000,
};

static const TMapObjAnimData BrickBlock_anim_data[] = {
	{ "BrickBlock.bmd", nullptr, 0, nullptr, nullptr },
	{ nullptr, nullptr, 0, nullptr, nullptr },
	{ "BrickBlockBreak.bmd", "brickblockbreak", 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo BrickBlock_anim_info
    = { 3, 2, BrickBlock_anim_data };

static const TMapObjCollisionData BrickBlock_map_collision_data[] = {
	{ "BrickBlock", 2 },
};

static const TMapObjCollisionInfo BrickBlock_map_collision_info
    = { 1, 1, BrickBlock_map_collision_data };

static TMapObjData BrickBlock_data = {
	"BrickBlock",
	0x400002C2,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&BrickBlock_anim_info,
	&no_data_obj_hit_info,
	&BrickBlock_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	80.0f,
	0x00002005,
	0x00000000,
};

static const TMapObjAnimData WaterMelonBlock_anim_data[] = {
	{ "WaterMelonBlock.bmd", nullptr, 0, nullptr, nullptr },
	{ nullptr, nullptr, 0, nullptr, nullptr },
	{ "WaterMelonBlockBreak.bmd", "watermelonblockbreak", 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo WaterMelonBlock_anim_info
    = { 3, 2, WaterMelonBlock_anim_data };

static const TMapObjCollisionData WaterMelonBlock_map_collision_data[] = {
	{ "WaterMelonBlock", 2 },
};

static const TMapObjCollisionInfo WaterMelonBlock_map_collision_info
    = { 1, 1, WaterMelonBlock_map_collision_data };

static TMapObjData WaterMelonBlock_data = {
	"WaterMelonBlock",
	0x400002C3,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&WaterMelonBlock_anim_info,
	&no_data_obj_hit_info,
	&WaterMelonBlock_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	80.0f,
	0x00002005,
	0x00000000,
};

static const TMapObjCollisionData TelesaBlock_map_collision_data[] = {
	{ "TelesaBlock", 1 },
};

static const TMapObjCollisionInfo TelesaBlock_map_collision_info
    = { 1, 1, TelesaBlock_map_collision_data };

static TMapObjData TelesaBlock_data = {
	"TelesaBlock",
	0x400002C4,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&breakable_block_obj_hit_info,
	&TelesaBlock_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	80.0f,
	0x00002005,
	0x00000000,
};

static const TMapObjHitDataTable WoodBlockPole_hit_data_table[] = {
	{ 30.0f, 500.0f, 100.0f, 500.0f },
};

static const TMapObjHitInfo WoodBlockPole_obj_hit_info
    = { 0x1, 0x80000000, 0.0f, WoodBlockPole_hit_data_table };

static TMapObjData WoodBlockPole_data = {
	"WoodBlockPole",
	0x400002C5,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&WoodBlockPole_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000041,
	0x00000000,
};

static const TMapObjCollisionData JuiceBlock_map_collision_data[] = {
	{ "NormalBlock", 1 },
};

static const TMapObjCollisionInfo JuiceBlock_map_collision_info
    = { 1, 1, JuiceBlock_map_collision_data };

static TMapObjData JuiceBlock_data = {
	"JuiceBlock",
	0x400002C6,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	&JuiceBlock_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00002005,
	0x00000000,
};

static const TMapObjAnimData SuperHipDropBlock_anim_data[] = {
	{ "super_rock.bmd", nullptr, 0, nullptr, nullptr },
	{ nullptr, nullptr, 0, nullptr, nullptr },
	{ "super_rock_down.bmd", "super_rock_down", 0, nullptr,
	  "/scene/mapObj/super_rock_down.bas" },
};

static const TMapObjAnimDataInfo SuperHipDropBlock_anim_info
    = { 3, 2, SuperHipDropBlock_anim_data };

static const TMapObjCollisionData SuperHipDropBlock_map_collision_data[] = {
	{ "super_rock", 2 },
};

static const TMapObjCollisionInfo SuperHipDropBlock_map_collision_info
    = { 1, 1, SuperHipDropBlock_map_collision_data };

static TMapObjData SuperHipDropBlock_data = {
	"SuperHipDropBlock",
	0x400002C8,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&SuperHipDropBlock_anim_info,
	&no_data_obj_hit_info,
	&SuperHipDropBlock_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	100.0f,
	0x00002005,
	0x00000000,
};

static const TMapObjSoundData tree_sound_data
    = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0000300F, 0x0000300F };

static const TMapObjAnimData PalmNormal_anim_data[] = {
	{ "palmNormal.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo PalmNormal_anim_info
    = { 1, 1, PalmNormal_anim_data };

static const TMapObjHitDataTable palmNormal_hit_data_table[] = {
	{ 600.0f, 1400.0f, 80.0f, 950.0f },
};

static const TMapObjHitInfo palmNormal_obj_hit_info
    = { 0x1, 0x80000000, 0.0f, palmNormal_hit_data_table };

static const TMapObjCollisionData PalmNormalTree_map_collision_data[] = {
	{ "palmNormal", 0 },
};

static const TMapObjCollisionInfo PalmNormalTree_map_collision_info
    = { 1, 1, PalmNormalTree_map_collision_data };

static TMapObjData palmNormal_data = {
	"palmNormal",
	0x40000034,
	"木マネージャー",
	"オブジェクトグループ",
	&PalmNormal_anim_info,
	&palmNormal_obj_hit_info,
	&PalmNormalTree_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000D40,
	0x00000000,
};

static const TMapObjHitDataTable palmOugi_hit_data_table[] = {
	{ 530.0f, 1200.0f, 80.0f, 950.0f },
};

static const TMapObjHitInfo palmOugi_obj_hit_info
    = { 0x1, 0x80000000, 0.0f, palmOugi_hit_data_table };

static const TMapObjCollisionData PalmOugiTree_map_collision_data[] = {
	{ "palmOugi", 0 },
};

static const TMapObjCollisionInfo PalmOugiTree_map_collision_info
    = { 1, 1, PalmOugiTree_map_collision_data };

static TMapObjData palmOugi_data = {
	"palmOugi",
	0x40000035,
	"木マネージャー",
	"オブジェクトグループ",
	nullptr,
	&palmOugi_obj_hit_info,
	&PalmOugiTree_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000940,
	0x00000000,
};

static const TMapObjHitDataTable palmSago_hit_data_table[] = {
	{ 750.0f, 1600.0f, 80.0f, 950.0f },
};

static const TMapObjHitInfo palmSago_obj_hit_info
    = { 0x1, 0x80000000, 0.0f, palmSago_hit_data_table };

static const TMapObjCollisionData PalmSagoTree_map_collision_data[] = {
	{ "palmSago", 0 },
};

static const TMapObjCollisionInfo PalmSagoTree_map_collision_info
    = { 1, 1, PalmSagoTree_map_collision_data };

static TMapObjData palmSago_data = {
	"palmSago",
	0x40000036,
	"木マネージャー",
	"オブジェクトグループ",
	nullptr,
	&palmSago_obj_hit_info,
	&PalmSagoTree_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000940,
	0x00000000,
};

static const TMapObjHitDataTable palmNatume_hit_data_table[] = {
	{ 530.0f, 1400.0f, 80.0f, 950.0f },
};

static const TMapObjHitInfo palmNatume_obj_hit_info
    = { 0x1, 0x80000000, 0.0f, palmNatume_hit_data_table };

static const TMapObjCollisionData PalmNatumeTree_map_collision_data[] = {
	{ "palmNatume", 0 },
};

static const TMapObjCollisionInfo PalmNatumeTree_map_collision_info
    = { 1, 1, PalmNatumeTree_map_collision_data };

static TMapObjData palmNatume_data = {
	"palmNatume",
	0x40000037,
	"木マネージャー",
	"オブジェクトグループ",
	nullptr,
	&palmNatume_obj_hit_info,
	&PalmNatumeTree_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000940,
	0x00000000,
};

static const TMapObjAnimData PalmLeaf_anim_data[] = {
	{ "palmLeaf.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo PalmLeaf_anim_info
    = { 1, 1, PalmLeaf_anim_data };

static const TMapObjHitDataTable palmLeaf_hit_data_table[] = {
	{ 600.0f, 300.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo palmLeaf_obj_hit_info
    = { 0x1, 0x80000000, 0.0f, palmLeaf_hit_data_table };

static const TMapObjCollisionData PalmLeafTree_map_collision_data[] = {
	{ "palmLeaf", 0 },
};

static const TMapObjCollisionInfo PalmLeafTree_map_collision_info
    = { 1, 1, PalmLeafTree_map_collision_data };

static TMapObjData palmLeaf_data = {
	"palmLeaf",
	0x40000038,
	"木マネージャー",
	"オブジェクトグループ",
	&PalmLeaf_anim_info,
	&palmLeaf_obj_hit_info,
	&PalmLeafTree_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000900,
	0x00000000,
};

static const TMapObjAnimData BananaTree_anim_data[] = {
	{ "BananaTree.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo BananaTree_anim_info
    = { 1, 1, BananaTree_anim_data };

static const TMapObjHitDataTable BananaTree_hit_data_table[] = {
	{ 700.0f, 1600.0f, 80.0f, 700.0f },
};

static const TMapObjHitInfo BananaTree_obj_hit_info
    = { 0x1, 0x80000000, 0.0f, BananaTree_hit_data_table };

static const TMapObjCollisionData BananaTree_map_collision_data[] = {
	{ "BananaTree", 2 },
};

static const TMapObjCollisionInfo BananaTree_map_collision_info
    = { 1, 1, BananaTree_map_collision_data };

static TMapObjData BananaTree_data = {
	"BananaTree",
	0x40000039,
	"木マネージャー",
	"オブジェクトグループ",
	&BananaTree_anim_info,
	&BananaTree_obj_hit_info,
	&BananaTree_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00004D40,
	0x00000000,
};

static const TMapObjHitDataTable FruitTree_hit_data_table[] = {
	{ 0.0f, 0.0f, 80.0f, 400.0f },
};

static const TMapObjHitInfo FruitTree_obj_hit_info
    = { 0x1, 0x80000000, 0.0f, FruitTree_hit_data_table };

static const TMapObjCollisionData FruitTree_map_collision_data[] = {
	{ "fruitTree", 0 },
};

static const TMapObjCollisionInfo FruitTree_map_collision_info
    = { 1, 1, FruitTree_map_collision_data };

static TMapObjData FruitTree_data = {
	"fruitTree",
	0x4000003A,
	"木マネージャー",
	"オブジェクトグループ",
	nullptr,
	&FruitTree_obj_hit_info,
	&FruitTree_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	80.0f,
	0x00000940,
	0x00000000,
};

static const TMapObjHitDataTable flower_hit_data_table[] = {
	{ 0.0f, 0.0f, 40.0f, 80.0f },
};

static const TMapObjHitInfo flower_obj_hit_info
    = { 0x1, 0x90000000, 0.0f, flower_hit_data_table };

static TMapObjData flower_data = {
	"plantFlower",
	0x40000046,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&flower_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000100,
	0x00000000,
};

static const TMapObjAnimData flowerOrange_anim_data[] = {
	{ "flowerOrange.bmd", nullptr, 0, "flower", nullptr },
	{ "flowerOrangeL.bmd", nullptr, 0, "flower", nullptr },
};

static const TMapObjAnimDataInfo flowerOrange_anim_info
    = { 2, 2, flowerOrange_anim_data };

static TMapObjData flowerOrange_data = {
	"flowerOrange",
	0x40000048,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&flowerOrange_anim_info,
	&flower_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000100,
	0x00000000,
};

static const TMapObjAnimData flowerPink_anim_data[] = {
	{ "flowerPink.bmd", nullptr, 0, "flower", nullptr },
	{ "flowerPinkL.bmd", nullptr, 0, "flower", nullptr },
};

static const TMapObjAnimDataInfo flowerPink_anim_info
    = { 2, 2, flowerPink_anim_data };

static TMapObjData flowerPink_data = {
	"flowerPink",
	0x40000048,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&flowerPink_anim_info,
	&flower_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000100,
	0x00000000,
};

static const TMapObjAnimData flowerPurple_anim_data[] = {
	{ "flowerPurple.bmd", nullptr, 0, "flower", nullptr },
	{ "flowerPurpleL.bmd", nullptr, 0, "flower", nullptr },
};

static const TMapObjAnimDataInfo flowerPurple_anim_info
    = { 2, 2, flowerPurple_anim_data };

static TMapObjData flowerPurple_data = {
	"flowerPurple",
	0x40000048,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&flowerPurple_anim_info,
	&flower_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000100,
	0x00000000,
};

static const TMapObjAnimData flowerRed_anim_data[] = {
	{ "flowerRed.bmd", nullptr, 0, "flower", nullptr },
	{ "flowerRedL.bmd", nullptr, 0, "flower", nullptr },
};

static const TMapObjAnimDataInfo flowerRed_anim_info
    = { 2, 2, flowerRed_anim_data };

static TMapObjData flowerRed_data = {
	"flowerRed",
	0x40000048,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&flowerRed_anim_info,
	&flower_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000100,
	0x00000000,
};

static const TMapObjAnimData flowerYellow_anim_data[] = {
	{ "flowerYellow.bmd", nullptr, 0, "flower", nullptr },
	{ "flowerYellowL.bmd", nullptr, 0, "flower", nullptr },
};

static const TMapObjAnimDataInfo flowerYellow_anim_info
    = { 2, 2, flowerYellow_anim_data };

static TMapObjData flowerYellow_data = {
	"flowerYellow",
	0x40000048,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&flowerYellow_anim_info,
	&flower_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000100,
	0x00000000,
};

static const TMapObjAnimData flowerSunflower_anim_data[] = {
	{ "flowerSunflower.bmd", nullptr, 0, "flower", nullptr },
	{ "flowerSunflowerL.bmd", nullptr, 0, "flower", nullptr },
};

static const TMapObjAnimDataInfo flowerSunflower_anim_info
    = { 2, 2, flowerSunflower_anim_data };

static TMapObjData flowerSunflower_data = {
	"flowerSunflower",
	0x40000048,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&flowerSunflower_anim_info,
	&flower_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000100,
	0x00000000,
};

static const TMapObjSoundData pole_sound_data
    = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00003010, 0x00003010 };

static const TMapObjHitDataTable telegraph_pole_l_hit_data_table[] = {
	{ 50.0f, 1150.0f, 50.0f, 1150.0f },
};

static const TMapObjHitInfo telegraph_pole_l_obj_hit_info
    = { 0x1, 0x9C000000, 0.0f, telegraph_pole_l_hit_data_table };

static const TMapObjSoundInfo pole_sound_info = { 0xA, &pole_sound_data };

static TMapObjData telegraph_pole_l_data = {
	"telegraph_pole_l",
	0x4000002E,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&no_data_anim_info,
	&telegraph_pole_l_obj_hit_info,
	nullptr,
	&pole_sound_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x000000C0,
	0x00000000,
};

static const TMapObjHitDataTable telegraph_pole_s_hit_data_table[] = {
	{ 50.0f, 900.0f, 50.0f, 900.0f },
};

static const TMapObjHitInfo telegraph_pole_s_obj_hit_info
    = { 0x1, 0x9C000000, 0.0f, telegraph_pole_s_hit_data_table };

static TMapObjData telegraph_pole_s_data = {
	"telegraph_pole_s",
	0x4000002E,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&no_data_anim_info,
	&telegraph_pole_s_obj_hit_info,
	nullptr,
	&pole_sound_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x000000C0,
	0x00000000,
};

static const TMapObjAnimData dptlight_anim_data[] = {
	{ "dptlight.bmd", nullptr, 0, nullptr, nullptr },
	{ nullptr, nullptr, 0, nullptr, nullptr },
	{ nullptr, nullptr, 0, nullptr, nullptr },
	{ nullptr, nullptr, 0, nullptr, nullptr },
	{ nullptr, nullptr, 0, nullptr, nullptr },
	{ "dptlight_recover.bmd", "dptlight_recover", 0, nullptr, nullptr },
	{ nullptr, nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo dptlight_anim_info
    = { 7, 2, dptlight_anim_data };

static const TMapObjHitDataTable streetlamp_hit_data_table[] = {
	{ 20.0f, 700.0f, 20.0f, 700.0f },
	{ 20.0f, 700.0f, 20.0f, 700.0f },
	{ 100.0f, 740.0f, 100.0f, 740.0f },
};

static const TMapObjHitInfo streetlamp_obj_hit_info
    = { 0x3, 0x9C000000, 0.0f, streetlamp_hit_data_table };

static const TMapObjCollisionData streetlamp_map_collision_data[] = {
	{ "dptlight", 2 },
	{ "dptlight_sink", 1 },
};

static const TMapObjCollisionInfo streetlamp_map_collision_info
    = { 2, 2, streetlamp_map_collision_data };

static TMapObjSinkData streetlamp_sink_data = { 0.0f, 3.0f };

static TMapObjData streetlamp_data = {
	"dptlight",
	0x4000002D,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&dptlight_anim_info,
	&streetlamp_obj_hit_info,
	&streetlamp_map_collision_info,
	&pole_sound_info,
	nullptr,
	&streetlamp_sink_data,
	nullptr,
	nullptr,
	0.0f,
	0x01000841,
	0x00000000,
};

static const TMapObjHitDataTable PoleNormal_hit_data_table[] = {
	{ 0.0f, 0.0f, 10.0f, 0.0f },
};

static const TMapObjHitInfo PoleNormal_obj_hit_info
    = { 0x1, 0x00000000, 0.0f, PoleNormal_hit_data_table };

static TMapObjData PoleNormal_data = {
	"PoleNormal",
	0x4000002F,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&no_data_anim_info,
	&PoleNormal_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x000001C0,
	0x00000000,
};

static TMapObjPhysicalData football_physical_data
    = { 0.35f, 0.6f,  0.6f,  0.1f, 0.98f, 0.98f, 1.0f,
	    0.0f,  0.85f, 0.72f, 0.0f, 0.0f,  0.0f };

static TMapObjPhysicalInfo football_physical_info
    = { 0xD, &football_physical_data, 0x2 };

static const TMapObjAnimData football_anim_data[] = {
	{ "soccerball.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo football_anim_info
    = { 1, 1, football_anim_data };

static const TMapObjHitDataTable football_hit_data_table[] = {
	{ 45.0f, 90.0f, 45.0f, 50.0f },
};

static const TMapObjHitInfo football_obj_hit_info
    = { 0x1, 0xDC000000, 0.0f, football_hit_data_table };

static const TMapObjSoundData football_sound_data
    = { 0xFFFFFFFF, 0x00003801, 0xFFFFFFFF, 0x00001807, 0x00001807,
	    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF };

static const TMapObjSoundInfo football_sound_info
    = { 0xA, &football_sound_data };

static TMapObjData football_data = {
	"football",
	0x40000064,
	"地形オブジェマネージャー",
	"敵グループ",
	&football_anim_info,
	&football_obj_hit_info,
	nullptr,
	&football_sound_info,
	&football_physical_info,
	nullptr,
	nullptr,
	nullptr,
	57.0f,
	0x000B0100,
	0x00000000,
};

static const TMapObjAnimData football_goal_anim_data[] = {
	{ "soccergoal_model.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo football_goal_anim_info
    = { 1, 1, football_goal_anim_data };

static const TMapObjHitDataTable football_goal_hit_data_table[] = {
	{ 85.0f, 115.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo football_goal_obj_hit_info
    = { 0x1, 0x40000000, 0.0f, football_goal_hit_data_table };

static const TMapObjCollisionData football_goal_map_collision_data[] = {
	{ "soccer_goal", 2 },
	{ "soccer_goal_stand", 2 },
};

static const TMapObjCollisionInfo football_goal_map_collision_info
    = { 2, 2, football_goal_map_collision_data };

static TMapObjData football_goal_data = {
	"football_goal",
	0x40000065,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&football_goal_anim_info,
	&football_goal_obj_hit_info,
	&football_goal_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	57.0f,
	0x00000000,
	0x00000000,
};

static TMapObjPhysicalData baloonball_physical_data
    = { 0.05f, 0.6f,  0.6f, 0.1f, 0.98f, 0.98f, 1.0f,
	    0.0f,  0.08f, 0.3f, 0.0f, 0.0f,  0.0f };

static TMapObjPhysicalInfo baloonball_physical_info
    = { 0xD, &baloonball_physical_data, 0x2 };

static TMapObjData baloonball_data = {
	"baloonball",
	0x40000064,
	"地形オブジェマネージャー",
	"敵グループ",
	&football_anim_info,
	&football_obj_hit_info,
	nullptr,
	&football_sound_info,
	&baloonball_physical_info,
	nullptr,
	nullptr,
	nullptr,
	57.0f,
	0x00010100,
	0x00000000,
};

static TMapObjData coconutball_data = {
	"coconut_ball",
	0x40000064,
	"地形オブジェマネージャー",
	"敵グループ",
	&football_anim_info,
	&football_obj_hit_info,
	nullptr,
	&football_sound_info,
	&football_physical_info,
	nullptr,
	nullptr,
	nullptr,
	57.0f,
	0x000B0100,
	0x00000000,
};

static TMapObjPhysicalData watermelon_physical_data
    = { 0.25f,  0.4f, 0.3f, 0.1f, 0.97f, 0.97f, 1.0f,
	    300.0f, 0.5f, 0.9f, 0.0f, 0.0f,  0.0f };

static TMapObjPhysicalInfo watermelon_physical_info
    = { 0xD, &watermelon_physical_data, 0x2 };

static const TMapObjAnimData Watermelon_anim_data[] = {
	{ "watermelon.bmd", nullptr, 0, nullptr, nullptr },
	{ nullptr, nullptr, 0, nullptr, nullptr },
	{ nullptr, nullptr, 0, nullptr, nullptr },
	{ nullptr, nullptr, 0, nullptr, nullptr },
	{ nullptr, nullptr, 0, nullptr, nullptr },
	{ nullptr, nullptr, 0, nullptr, nullptr },
	{ nullptr, nullptr, 0, nullptr, nullptr },
	{ nullptr, "watermelon_shrink", 0, nullptr,
	  "/scene/mapObj/watermelon_shrink.bas" },
};

static const TMapObjAnimDataInfo Watermelon_anim_info
    = { 8, 1, Watermelon_anim_data };

static const TMapObjHitDataTable watermelon_hit_data_table[] = {
	{ 45.0f, 100.0f, 45.0f, 100.0f },
};

static const TMapObjHitInfo watermelon_obj_hit_info
    = { 0x1, 0xDC000000, 0.0f, watermelon_hit_data_table };

static const TMapObjSoundData watermelon_sound_data
    = { 0xFFFFFFFF, 0x00003801, 0xFFFFFFFF, 0x0000194F, 0x00003862,
	    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000 };

static const TMapObjSoundInfo watermelon_sound_info
    = { 0xA, &watermelon_sound_data };

static TMapObjData watermelon_data = {
	"watermelon",
	0x400000D0,
	"地形オブジェマネージャー",
	"敵グループ",
	&Watermelon_anim_info,
	&watermelon_obj_hit_info,
	nullptr,
	&watermelon_sound_info,
	&watermelon_physical_info,
	nullptr,
	nullptr,
	nullptr,
	57.0f,
	0x008B0100,
	0x00000000,
};

static const TMapObjAnimData WatermelonStatic_anim_data[] = {
	{ "watermelon.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo WatermelonStatic_anim_info
    = { 1, 1, WatermelonStatic_anim_data };

static TMapObjData WatermelonStatic_data = {
	"WatermelonStatic",
	0x400000DB,
	"地形オブジェマネージャー",
	"敵グループ",
	&WatermelonStatic_anim_info,
	&watermelon_obj_hit_info,
	nullptr,
	&watermelon_sound_info,
	&watermelon_physical_info,
	nullptr,
	nullptr,
	nullptr,
	57.0f,
	0x00000100,
	0x00000000,
};

static const TMapObjAnimData cloud_anim_data[] = {
	{ "cloud.bmd", "cloud_wait", 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo cloud_anim_info = { 1, 1, cloud_anim_data };

static const TMapObjHitDataTable cloud_hit_data_table[] = {
	{ 100.0f, 100.0f, 300.0f, 50.0f },
};

static const TMapObjHitInfo cloud_obj_hit_info
    = { 0x1, 0x00000000, 0.0f, cloud_hit_data_table };

static const TMapObjCollisionData cloud_map_collision_data[] = {
	{ "cloud", 1 },
};

static const TMapObjCollisionInfo cloud_map_collision_info
    = { 1, 1, cloud_map_collision_data };

static TMapObjData cloud_data = {
	"cloud",
	0x400002C7,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&cloud_anim_info,
	&cloud_obj_hit_info,
	&cloud_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	300.0f,
	0x00000005,
	0x00000000,
};

static const TMapObjAnimData normallift_anim_data[] = {
	{ "NormalBlock.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo normallift_anim_info
    = { 1, 1, normallift_anim_data };

static const TMapObjHitDataTable normallift_hit_data_table[] = {
	{ 100.0f, 100.0f, 300.0f, 50.0f },
};

static const TMapObjHitInfo normallift_obj_hit_info
    = { 0x1, 0x00000000, 0.0f, normallift_hit_data_table };

static const TMapObjCollisionData normallift_map_collision_data[] = {
	{ "NormalBlock", 1 },
};

static const TMapObjCollisionInfo normallift_map_collision_info
    = { 1, 1, normallift_map_collision_data };

static TMapObjData normallift_data = {
	"normallift",
	0x400002BD,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&normallift_anim_info,
	&normallift_obj_hit_info,
	&normallift_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000005,
	0x00000000,
};

static const TMapObjAnimData exrollcube_anim_data[] = {
	{ "EXRollCube.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo exrollcube_anim_info
    = { 1, 1, exrollcube_anim_data };

static const TMapObjHitDataTable exrollcube_hit_data_table[] = {
	{ 100.0f, 100.0f, 300.0f, 50.0f },
};

static const TMapObjHitInfo exrollcube_obj_hit_info
    = { 0x1, 0x00000000, 0.0f, exrollcube_hit_data_table };

static const TMapObjCollisionData exrollcube_map_collision_data[] = {
	{ "EXRollCube", 1 },
};

static const TMapObjCollisionInfo exrollcube_map_collision_info
    = { 1, 1, exrollcube_map_collision_data };

static TMapObjData exrollcube_data = {
	"exrollcube",
	0x400002BD,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&exrollcube_anim_info,
	&exrollcube_obj_hit_info,
	&exrollcube_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	100.0f,
	0x00000004,
	0x00000000,
};

static const TMapObjAnimData exkickboard_anim_data[] = {
	{ "EXKickBoard.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo exkickboard_anim_info
    = { 1, 1, exkickboard_anim_data };

static const TMapObjHitDataTable exkickboard_hit_data_table[] = {
	{ 100.0f, 100.0f, 300.0f, 50.0f },
};

static const TMapObjHitInfo exkickboard_obj_hit_info
    = { 0x1, 0x00000000, 0.0f, exkickboard_hit_data_table };

static const TMapObjCollisionData exkickboard_map_collision_data[] = {
	{ "EXKickBoard", 1 },
};

static const TMapObjCollisionInfo exkickboard_map_collision_info
    = { 1, 1, exkickboard_map_collision_data };

static TMapObjData exkickboard_data = {
	"exkickboard",
	0x400002BD,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&exkickboard_anim_info,
	&exkickboard_obj_hit_info,
	&exkickboard_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	100.0f,
	0x00000004,
	0x00000000,
};

static const TMapObjAnimData railblockr_anim_data[] = {
	{ "AllPurposeBoardR.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo railblockr_anim_info
    = { 1, 1, railblockr_anim_data };

static const TMapObjAnimData railblocky_anim_data[] = {
	{ "AllPurposeBoardY.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo railblocky_anim_info
    = { 1, 1, railblocky_anim_data };

static const TMapObjAnimData railblockb_anim_data[] = {
	{ "AllPurposeBoardB.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo railblockb_anim_info
    = { 1, 1, railblockb_anim_data };

static const TMapObjHitDataTable railblock_hit_data_table[] = {
	{ 100.0f, 100.0f, 300.0f, 500.0f },
};

static const TMapObjHitInfo railblock_obj_hit_info
    = { 0x1, 0x00000000, 0.0f, railblock_hit_data_table };

static const TMapObjCollisionData railblock_map_collision_data[] = {
	{ "AllPurposeBoard", 1 },
};

static const TMapObjCollisionInfo railblock_map_collision_info
    = { 1, 1, railblock_map_collision_data };

static TMapObjData railblockr_data = {
	"railblockr",
	0x400002BD,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&railblockr_anim_info,
	&railblock_obj_hit_info,
	&railblock_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	100.0f,
	0x00000004,
	0x00000000,
};

static TMapObjData railblocky_data = {
	"railblocky",
	0x400002BD,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&railblocky_anim_info,
	&railblock_obj_hit_info,
	&railblock_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	100.0f,
	0x00000004,
	0x00000000,
};

static TMapObjData railblockb_data = {
	"railblockb",
	0x400002BD,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&railblockb_anim_info,
	&railblock_obj_hit_info,
	&railblock_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	100.0f,
	0x00000004,
	0x00000000,
};

static TMapObjData rollblockr_data = {
	"rollblockr",
	0x400002BD,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&railblockr_anim_info,
	&railblock_obj_hit_info,
	&railblock_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	100.0f,
	0x00000004,
	0x00000000,
};

static TMapObjData rollblocky_data = {
	"rollblocky",
	0x400002BD,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&railblocky_anim_info,
	&railblock_obj_hit_info,
	&railblock_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	100.0f,
	0x00000004,
	0x00000000,
};

static TMapObjData rollblockb_data = {
	"rollblockb",
	0x400002BD,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&railblockb_anim_info,
	&railblock_obj_hit_info,
	&railblock_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	100.0f,
	0x00000004,
	0x00000000,
};

static const TMapObjAnimData umaibou_anim_data[] = {
	{ "umaibou.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo umaibou_anim_info
    = { 1, 1, umaibou_anim_data };

static const TMapObjHitDataTable umaibou_hit_data_table[] = {
	{ 100.0f, 100.0f, 300.0f, 50.0f },
};

static const TMapObjHitInfo umaibou_obj_hit_info
    = { 0x1, 0x00000000, 0.0f, umaibou_hit_data_table };

static const TMapObjCollisionData umaibou_map_collision_data[] = {
	{ "umaibou", 1 },
};

static const TMapObjCollisionInfo umaibou_map_collision_info
    = { 1, 1, umaibou_map_collision_data };

static TMapObjData umaibou_data = {
	"umaibou",
	0x400002BD,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&umaibou_anim_info,
	&umaibou_obj_hit_info,
	&umaibou_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	100.0f,
	0x00000005,
	0x00000000,
};

static const TMapObjAnimData kamaboko_anim_data[] = {
	{ "kamaboko.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo kamaboko_anim_info
    = { 1, 1, kamaboko_anim_data };

static const TMapObjHitDataTable kamaboko_hit_data_table[] = {
	{ 100.0f, 100.0f, 300.0f, 50.0f },
};

static const TMapObjHitInfo kamaboko_obj_hit_info
    = { 0x1, 0x00000000, 0.0f, kamaboko_hit_data_table };

static const TMapObjCollisionData kamaboko_map_collision_data[] = {
	{ "kamaboko", 1 },
};

static const TMapObjCollisionInfo kamaboko_map_collision_info
    = { 1, 1, kamaboko_map_collision_data };

static TMapObjData kamaboko_data = {
	"kamaboko",
	0x400002BD,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&kamaboko_anim_info,
	&kamaboko_obj_hit_info,
	&kamaboko_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	100.0f,
	0x00000004,
	0x00000000,
};

static const TMapObjAnimData getag_anim_data[] = {
	{ "getaGreen.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo getag_anim_info = { 1, 1, getag_anim_data };

static const TMapObjAnimData getao_anim_data[] = {
	{ "getaOrange.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo getao_anim_info = { 1, 1, getao_anim_data };

static const TMapObjHitDataTable geta_hit_data_table[] = {
	{ 100.0f, 100.0f, 300.0f, 50.0f },
};

static const TMapObjHitInfo geta_obj_hit_info
    = { 0x1, 0x00000000, 0.0f, geta_hit_data_table };

static const TMapObjCollisionData geta_map_collision_data[] = {
	{ "geta", 1 },
};

static const TMapObjCollisionInfo geta_map_collision_info
    = { 1, 1, geta_map_collision_data };

static TMapObjData getag_data = {
	"getag",
	0x400002BD,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&getag_anim_info,
	&geta_obj_hit_info,
	&geta_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	100.0f,
	0x00000004,
	0x00000000,
};

static TMapObjData getao_data = {
	"getao",
	0x400002BD,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&getao_anim_info,
	&geta_obj_hit_info,
	&geta_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	100.0f,
	0x00000004,
	0x00000000,
};

static const TMapObjAnimData uirou_anim_data[] = {
	{ "uirou.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo uirou_anim_info = { 1, 1, uirou_anim_data };

static const TMapObjHitDataTable uirou_hit_data_table[] = {
	{ 100.0f, 100.0f, 300.0f, 50.0f },
};

static const TMapObjHitInfo uirou_obj_hit_info
    = { 0x1, 0x00000000, 0.0f, uirou_hit_data_table };

static const TMapObjCollisionData uirou_map_collision_data[] = {
	{ "uirou", 1 },
};

static const TMapObjCollisionInfo uirou_map_collision_info
    = { 1, 1, uirou_map_collision_data };

static TMapObjData uirou_data = {
	"uirou",
	0x400002BD,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&uirou_anim_info,
	&uirou_obj_hit_info,
	&uirou_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	100.0f,
	0x00000004,
	0x00000000,
};

static const TMapObjAnimData hikidashi_anim_data[] = {
	{ "hikidashi.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo hikidashi_anim_info
    = { 1, 1, hikidashi_anim_data };

static const TMapObjHitDataTable hikidashi_hit_data_table[] = {
	{ 100.0f, 100.0f, 300.0f, 50.0f },
};

static const TMapObjHitInfo hikidashi_obj_hit_info
    = { 0x1, 0x00000000, 0.0f, hikidashi_hit_data_table };

static const TMapObjCollisionData hikidashi_map_collision_data[] = {
	{ "hikidashi", 1 },
};

static const TMapObjCollisionInfo hikidashi_map_collision_info
    = { 1, 1, hikidashi_map_collision_data };

static TMapObjData hikidashi_data = {
	"hikidashi",
	0x400002BD,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&hikidashi_anim_info,
	&hikidashi_obj_hit_info,
	&hikidashi_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	100.0f,
	0x00000004,
	0x00000000,
};

static const TMapObjAnimData castella_anim_data[] = {
	{ "castella.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo castella_anim_info
    = { 1, 1, castella_anim_data };

static const TMapObjHitDataTable castella_hit_data_table[] = {
	{ 100.0f, 100.0f, 300.0f, 50.0f },
};

static const TMapObjHitInfo castella_obj_hit_info
    = { 0x1, 0x00000000, 0.0f, castella_hit_data_table };

static const TMapObjCollisionData castella_map_collision_data[] = {
	{ "castella", 1 },
};

static const TMapObjCollisionInfo castella_map_collision_info
    = { 1, 1, castella_map_collision_data };

static TMapObjData castella_data = {
	"castella",
	0x400002BD,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&castella_anim_info,
	&castella_obj_hit_info,
	&castella_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	100.0f,
	0x00000004,
	0x00000000,
};

static const TMapObjAnimData yoshiblock_anim_data[] = {
	{ "yoshiblock.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo yoshiblock_anim_info
    = { 1, 1, yoshiblock_anim_data };

static const TMapObjHitDataTable yoshiblock_hit_data_table[] = {
	{ 100.0f, 100.0f, 300.0f, 400.0f },
};

static const TMapObjHitInfo yoshiblock_obj_hit_info
    = { 0x1, 0x00000000, 0.0f, yoshiblock_hit_data_table };

static const TMapObjCollisionData yoshiblock_map_collision_data[] = {
	{ "yoshiblock", 1 },
};

static const TMapObjCollisionInfo yoshiblock_map_collision_info
    = { 1, 1, yoshiblock_map_collision_data };

static TMapObjData yoshiblock_data = {
	"yoshiblock",
	0x400002BD,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&yoshiblock_anim_info,
	&yoshiblock_obj_hit_info,
	&yoshiblock_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	100.0f,
	0x00000005,
	0x00000000,
};

static const TMapObjCollisionData WoodBlockTriangle_map_collision_data[] = {
	{ "WoodBlockTriangle", 1 },
};

static const TMapObjCollisionInfo WoodBlockTriangle_map_collision_info
    = { 1, 1, WoodBlockTriangle_map_collision_data };

static TMapObjData WoodBlockTriangle_data = {
	"WoodBlockTriangle",
	0x400002C5,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&normallift_obj_hit_info,
	&WoodBlockTriangle_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000001,
	0x00000000,
};

static const TMapObjCollisionData WoodBlockPyramid_map_collision_data[] = {
	{ "WoodBlockPyramid", 1 },
};

static const TMapObjCollisionInfo WoodBlockPyramid_map_collision_info
    = { 1, 1, WoodBlockPyramid_map_collision_data };

static TMapObjData WoodBlockPyramid_data = {
	"WoodBlockPyramid",
	0x400002C5,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&normallift_obj_hit_info,
	&WoodBlockPyramid_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000001,
	0x00000000,
};

static const TMapObjCollisionData WoodBlockLong_map_collision_data[] = {
	{ "WoodBlockLong", 1 },
};

static const TMapObjCollisionInfo WoodBlockLong_map_collision_info
    = { 1, 1, WoodBlockLong_map_collision_data };

static TMapObjData WoodBlockLong_data = {
	"WoodBlockLong",
	0x400002C5,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&normallift_obj_hit_info,
	&WoodBlockLong_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000001,
	0x00000000,
};

static const TMapObjCollisionData WoodBlockLarge_map_collision_data[] = {
	{ "WoodBlockLarge", 1 },
};

static const TMapObjCollisionInfo WoodBlockLarge_map_collision_info
    = { 1, 1, WoodBlockLarge_map_collision_data };

static TMapObjData WoodBlockLarge_data = {
	"WoodBlockLarge",
	0x400002C5,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&normallift_obj_hit_info,
	&WoodBlockLarge_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000001,
	0x00000000,
};

static const TMapObjCollisionData WoodBlockCone_map_collision_data[] = {
	{ "WoodBlockCone", 1 },
};

static const TMapObjCollisionInfo WoodBlockCone_map_collision_info
    = { 1, 1, WoodBlockCone_map_collision_data };

static TMapObjData WoodBlockCone_data = {
	"WoodBlockCone",
	0x400002C5,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&normallift_obj_hit_info,
	&WoodBlockCone_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000001,
	0x00000000,
};

static const TMapObjCollisionData WoodBlockL_map_collision_data[] = {
	{ "WoodBlockL", 1 },
};

static const TMapObjCollisionInfo WoodBlockL_map_collision_info
    = { 1, 1, WoodBlockL_map_collision_data };

static TMapObjData WoodBlockL_data = {
	"WoodBlockL",
	0x400002C5,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&normallift_obj_hit_info,
	&WoodBlockL_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000001,
	0x00000000,
};

static const TMapObjCollisionData WoodBlockBridge_map_collision_data[] = {
	{ "WoodBlockBridge", 1 },
};

static const TMapObjCollisionInfo WoodBlockBridge_map_collision_info
    = { 1, 1, WoodBlockBridge_map_collision_data };

static TMapObjData WoodBlockBridge_data = {
	"WoodBlockBridge",
	0x400002C5,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&normallift_obj_hit_info,
	&WoodBlockBridge_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000001,
	0x00000000,
};

static const TMapObjCollisionData WoodBlockCube_map_collision_data[] = {
	{ "WoodBlockCube", 1 },
};

static const TMapObjCollisionInfo WoodBlockCube_map_collision_info
    = { 1, 1, WoodBlockCube_map_collision_data };

static TMapObjData WoodBlockCube_data = {
	"WoodBlockCube",
	0x400002C5,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&normallift_obj_hit_info,
	&WoodBlockCube_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000001,
	0x00000000,
};

static const TMapObjHitDataTable normalvariant0_hit_data_table[] = {
	{ 100.0f, 100.0f, 300.0f, 50.0f },
};

static const TMapObjHitInfo normalvariant0_obj_hit_info
    = { 0x1, 0x00000000, 0.0f, normalvariant0_hit_data_table };

static const TMapObjCollisionData normalvariant0_map_collision_data[] = {
	{ "normalvariant0", 1 },
};

static const TMapObjCollisionInfo normalvariant0_map_collision_info
    = { 1, 1, normalvariant0_map_collision_data };

static TMapObjData normalvariant0_data = {
	"normalvariant0",
	0x400002BD,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&normalvariant0_obj_hit_info,
	&normalvariant0_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000001,
	0x00000000,
};

static const TMapObjHitDataTable normalvariant1_hit_data_table[] = {
	{ 100.0f, 100.0f, 300.0f, 50.0f },
};

static const TMapObjHitInfo normalvariant1_obj_hit_info
    = { 0x1, 0x00000000, 0.0f, normalvariant1_hit_data_table };

static const TMapObjCollisionData normalvariant1_map_collision_data[] = {
	{ "normalvariant1", 1 },
};

static const TMapObjCollisionInfo normalvariant1_map_collision_info
    = { 1, 1, normalvariant1_map_collision_data };

static TMapObjData normalvariant1_data = {
	"normalvariant1",
	0x400002BD,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&normalvariant1_obj_hit_info,
	&normalvariant1_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000001,
	0x00000000,
};

static const TMapObjHitDataTable normalvariant2_hit_data_table[] = {
	{ 100.0f, 100.0f, 300.0f, 50.0f },
};

static const TMapObjHitInfo normalvariant2_obj_hit_info
    = { 0x1, 0x00000000, 0.0f, normalvariant2_hit_data_table };

static const TMapObjCollisionData normalvariant2_map_collision_data[] = {
	{ "normalvariant2", 1 },
};

static const TMapObjCollisionInfo normalvariant2_map_collision_info
    = { 1, 1, normalvariant2_map_collision_data };

static TMapObjData normalvariant2_data = {
	"normalvariant2",
	0x400002BD,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&normalvariant2_obj_hit_info,
	&normalvariant2_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000001,
	0x00000000,
};

static const TMapObjHitDataTable normalvariant3_hit_data_table[] = {
	{ 100.0f, 100.0f, 300.0f, 50.0f },
};

static const TMapObjHitInfo normalvariant3_obj_hit_info
    = { 0x1, 0x00000000, 0.0f, normalvariant3_hit_data_table };

static const TMapObjCollisionData normalvariant3_map_collision_data[] = {
	{ "normalvariant3", 1 },
};

static const TMapObjCollisionInfo normalvariant3_map_collision_info
    = { 1, 1, normalvariant3_map_collision_data };

static TMapObjData normalvariant3_data = {
	"normalvariant3",
	0x400002BD,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&normalvariant3_obj_hit_info,
	&normalvariant3_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000001,
	0x00000000,
};

static const TMapObjHitDataTable normalvariant4_hit_data_table[] = {
	{ 100.0f, 100.0f, 300.0f, 50.0f },
};

static const TMapObjHitInfo normalvariant4_obj_hit_info
    = { 0x1, 0x00000000, 0.0f, normalvariant4_hit_data_table };

static const TMapObjCollisionData normalvariant4_map_collision_data[] = {
	{ "normalvariant4", 1 },
};

static const TMapObjCollisionInfo normalvariant4_map_collision_info
    = { 1, 1, normalvariant4_map_collision_data };

static TMapObjData normalvariant4_data = {
	"normalvariant4",
	0x400002BD,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&normalvariant4_obj_hit_info,
	&normalvariant4_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000001,
	0x00000000,
};

static const TMapObjHitDataTable railvariant0_hit_data_table[] = {
	{ 100.0f, 100.0f, 300.0f, 50.0f },
};

static const TMapObjHitInfo railvariant0_obj_hit_info
    = { 0x1, 0x00000000, 0.0f, railvariant0_hit_data_table };

static const TMapObjCollisionData railvariant0_map_collision_data[] = {
	{ "railvariant0", 1 },
};

static const TMapObjCollisionInfo railvariant0_map_collision_info
    = { 1, 1, railvariant0_map_collision_data };

static TMapObjData railvariant0_data = {
	"railvariant0",
	0x400002BD,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&railvariant0_obj_hit_info,
	&railvariant0_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000001,
	0x00000000,
};

static const TMapObjHitDataTable railvariant1_hit_data_table[] = {
	{ 100.0f, 100.0f, 300.0f, 50.0f },
};

static const TMapObjHitInfo railvariant1_obj_hit_info
    = { 0x1, 0x00000000, 0.0f, railvariant1_hit_data_table };

static const TMapObjCollisionData railvariant1_map_collision_data[] = {
	{ "railvariant1", 1 },
};

static const TMapObjCollisionInfo railvariant1_map_collision_info
    = { 1, 1, railvariant1_map_collision_data };

static TMapObjData railvariant1_data = {
	"railvariant1",
	0x400002BD,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&railvariant1_obj_hit_info,
	&railvariant1_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000001,
	0x00000000,
};

static const TMapObjHitDataTable railvariant2_hit_data_table[] = {
	{ 100.0f, 100.0f, 300.0f, 50.0f },
};

static const TMapObjHitInfo railvariant2_obj_hit_info
    = { 0x1, 0x00000000, 0.0f, railvariant2_hit_data_table };

static const TMapObjCollisionData railvariant2_map_collision_data[] = {
	{ "railvariant2", 1 },
};

static const TMapObjCollisionInfo railvariant2_map_collision_info
    = { 1, 1, railvariant2_map_collision_data };

static TMapObjData railvariant2_data = {
	"railvariant2",
	0x400002BD,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&railvariant2_obj_hit_info,
	&railvariant2_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000001,
	0x00000000,
};

static const TMapObjHitDataTable railvariant3_hit_data_table[] = {
	{ 100.0f, 100.0f, 300.0f, 50.0f },
};

static const TMapObjHitInfo railvariant3_obj_hit_info
    = { 0x1, 0x00000000, 0.0f, railvariant3_hit_data_table };

static const TMapObjCollisionData railvariant3_map_collision_data[] = {
	{ "railvariant3", 1 },
};

static const TMapObjCollisionInfo railvariant3_map_collision_info
    = { 1, 1, railvariant3_map_collision_data };

static TMapObjData railvariant3_data = {
	"railvariant3",
	0x400002BD,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&railvariant3_obj_hit_info,
	&railvariant3_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000001,
	0x00000000,
};

static const TMapObjHitDataTable railvariant4_hit_data_table[] = {
	{ 100.0f, 100.0f, 300.0f, 50.0f },
};

static const TMapObjHitInfo railvariant4_obj_hit_info
    = { 0x1, 0x00000000, 0.0f, railvariant4_hit_data_table };

static const TMapObjCollisionData railvariant4_map_collision_data[] = {
	{ "railvariant4", 1 },
};

static const TMapObjCollisionInfo railvariant4_map_collision_info
    = { 1, 1, railvariant4_map_collision_data };

static TMapObjData railvariant4_data = {
	"railvariant4",
	0x400002BD,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&railvariant4_obj_hit_info,
	&railvariant4_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000001,
	0x00000000,
};

static const TMapObjHitDataTable rollvariant0_hit_data_table[] = {
	{ 100.0f, 100.0f, 300.0f, 50.0f },
};

static const TMapObjHitInfo rollvariant0_obj_hit_info
    = { 0x1, 0x00000000, 0.0f, rollvariant0_hit_data_table };

static const TMapObjCollisionData rollvariant0_map_collision_data[] = {
	{ "rollvariant0", 1 },
};

static const TMapObjCollisionInfo rollvariant0_map_collision_info
    = { 1, 1, rollvariant0_map_collision_data };

static TMapObjData rollvariant0_data = {
	"rollvariant0",
	0x400002BD,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&rollvariant0_obj_hit_info,
	&rollvariant0_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000001,
	0x00000000,
};

static const TMapObjHitDataTable rollvariant1_hit_data_table[] = {
	{ 100.0f, 100.0f, 300.0f, 50.0f },
};

static const TMapObjHitInfo rollvariant1_obj_hit_info
    = { 0x1, 0x00000000, 0.0f, rollvariant1_hit_data_table };

static const TMapObjCollisionData rollvariant1_map_collision_data[] = {
	{ "rollvariant1", 1 },
};

static const TMapObjCollisionInfo rollvariant1_map_collision_info
    = { 1, 1, rollvariant1_map_collision_data };

static TMapObjData rollvariant1_data = {
	"rollvariant1",
	0x400002BD,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&rollvariant1_obj_hit_info,
	&rollvariant1_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000001,
	0x00000000,
};

static const TMapObjHitDataTable rollvariant2_hit_data_table[] = {
	{ 100.0f, 100.0f, 300.0f, 50.0f },
};

static const TMapObjHitInfo rollvariant2_obj_hit_info
    = { 0x1, 0x00000000, 0.0f, rollvariant2_hit_data_table };

static const TMapObjCollisionData rollvariant2_map_collision_data[] = {
	{ "rollvariant2", 1 },
};

static const TMapObjCollisionInfo rollvariant2_map_collision_info
    = { 1, 1, rollvariant2_map_collision_data };

static TMapObjData rollvariant2_data = {
	"rollvariant2",
	0x400002BD,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&rollvariant2_obj_hit_info,
	&rollvariant2_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000001,
	0x00000000,
};

static const TMapObjHitDataTable rollvariant3_hit_data_table[] = {
	{ 100.0f, 100.0f, 300.0f, 50.0f },
};

static const TMapObjHitInfo rollvariant3_obj_hit_info
    = { 0x1, 0x00000000, 0.0f, rollvariant3_hit_data_table };

static const TMapObjCollisionData rollvariant3_map_collision_data[] = {
	{ "rollvariant3", 1 },
};

static const TMapObjCollisionInfo rollvariant3_map_collision_info
    = { 1, 1, rollvariant3_map_collision_data };

static TMapObjData rollvariant3_data = {
	"rollvariant3",
	0x400002BD,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&rollvariant3_obj_hit_info,
	&rollvariant3_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000001,
	0x00000000,
};

static const TMapObjHitDataTable rollvariant4_hit_data_table[] = {
	{ 100.0f, 100.0f, 300.0f, 50.0f },
};

static const TMapObjHitInfo rollvariant4_obj_hit_info
    = { 0x1, 0x00000000, 0.0f, rollvariant4_hit_data_table };

static const TMapObjCollisionData rollvariant4_map_collision_data[] = {
	{ "rollvariant4", 1 },
};

static const TMapObjCollisionInfo rollvariant4_map_collision_info
    = { 1, 1, rollvariant4_map_collision_data };

static TMapObjData rollvariant4_data = {
	"rollvariant4",
	0x400002BD,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&rollvariant4_obj_hit_info,
	&rollvariant4_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000001,
	0x00000000,
};

static const TMapObjAnimData lamptrapspike_anim_data[] = {
	{ "lamptrapspike.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo lamptrapspike_anim_info
    = { 1, 1, lamptrapspike_anim_data };

static const TMapObjAnimData lamptrapiron_anim_data[] = {
	{ "lamptrapiron.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo lamptrapiron_anim_info
    = { 1, 1, lamptrapiron_anim_data };

static const TMapObjHitDataTable lamptrap_hit_data_table[] = {
	{ 20.0f, 700.0f, 20.0f, 700.0f },
};

static const TMapObjHitInfo lamptrap_obj_hit_info
    = { 0x1, 0x9C000000, 0.0f, lamptrap_hit_data_table };

static const TMapObjCollisionData lamptrap_map_collision_data[] = {
	{ "lamptrap", 1 },
};

static const TMapObjCollisionInfo lamptrap_map_collision_info
    = { 1, 1, lamptrap_map_collision_data };

static TMapObjData lamptrapspike_data = {
	"lamptrapspike",
	0x4000001E,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&lamptrapspike_anim_info,
	&lamptrap_obj_hit_info,
	&lamptrap_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000001,
	0x00000000,
};

static TMapObjData lamptrapiron_data = {
	"lamptrapiron",
	0x4000001D,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&lamptrapiron_anim_info,
	&lamptrap_obj_hit_info,
	&lamptrap_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000001,
	0x00000000,
};

static const TMapObjCollisionData airplane_map_collision_data[] = {
	{ "airplane", 2 },
};

static const TMapObjCollisionInfo airplane_map_collision_info
    = { 1, 1, airplane_map_collision_data };

static TMapObjData airplane_data = {
	"airplane",
	0x4000006E,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	&airplane_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000801,
	0x00000000,
};

static const TMapObjAnimData WoodBox_anim_data[] = {
	{ "kibako.bmd", nullptr, 0, nullptr, nullptr },
	{ nullptr, nullptr, 0, nullptr, nullptr },
	{ "kibako_crash.bmd", "kibako_crash", 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo WoodBox_anim_info
    = { 3, 2, WoodBox_anim_data };

static const TMapObjCollisionData WoodBoxNozzle_map_collision_data[] = {
	{ "kibako", 2 },
};

static const TMapObjCollisionInfo WoodBoxNozzle_map_collision_info
    = { 1, 1, WoodBoxNozzle_map_collision_data };

static const TMapObjSoundData WoodBox_sound_data
    = { 0xFFFFFFFF, 0x00003802, 0x0000380A, 0xFFFFFFFF, 0xFFFFFFFF,
	    0x0000380B, 0xFFFFFFFF, 0xFFFFFFFF, 0x00003812, 0xFFFFFFFF };

static TMapObjData WoodBox_data = {
	"WoodBox",
	0x4000001C,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&WoodBox_anim_info,
	&no_data_obj_hit_info,
	&WoodBoxNozzle_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	125.0f,
	0x00002101,
	0x00000000,
};

static const TMapObjHitDataTable AirportPole_hit_data_table[] = {
	{ 0.0f, 0.0f, 60.0f, 2800.0f },
};

static const TMapObjHitInfo AirportPole_obj_hit_info
    = { 0x1, 0x00000000, 0.0f, AirportPole_hit_data_table };

static TMapObjData AirportPole_data = {
	"AirportPole",
	0x4000002F,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&no_data_anim_info,
	&AirportPole_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x000000C0,
	0x00000000,
};

static const TMapObjAnimData ice_car_anim_data[] = {
	{ "yatai.bmd", nullptr, 0, nullptr, nullptr },
	{ nullptr, nullptr, 0, nullptr, nullptr },
	{ nullptr, nullptr, 0, nullptr, nullptr },
	{ nullptr, nullptr, 0, nullptr, nullptr },
	{ "yatai.bmd", "yatai_bound", 0, nullptr, nullptr },
	{ nullptr, nullptr, 0, nullptr, nullptr },
	{ nullptr, nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo ice_car_anim_info
    = { 7, 1, ice_car_anim_data };

static const TMapObjHitDataTable ice_car_hit_data_table[] = {
	{ 90.0f, 280.0f, 90.0f, 280.0f },
};

static const TMapObjHitInfo ice_car_obj_hit_info
    = { 0x1, 0x00000000, 0.0f, ice_car_hit_data_table };

static const TMapObjCollisionData ice_car_map_collision_data[] = {
	{ "yatai", 2 },
};

static const TMapObjCollisionInfo ice_car_map_collision_info
    = { 1, 1, ice_car_map_collision_data };

static TMapObjData ice_car_data = {
	"ice_car",
	0x40000078,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&ice_car_anim_info,
	&ice_car_obj_hit_info,
	&ice_car_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000000,
	0x00000000,
};

static const TMapObjCollisionData move_ice_car_map_collision_data[] = {
	{ "yatai", 1 },
};

static const TMapObjCollisionInfo move_ice_car_map_collision_info
    = { 1, 1, move_ice_car_map_collision_data };

static TMapObjData move_ice_car_data = {
	"move_ice_car",
	0x40000078,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&ice_car_anim_info,
	&ice_car_obj_hit_info,
	&move_ice_car_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000000,
	0x00000000,
};

static const TMapObjHitDataTable AirportBuoy_hit_data_table[] = {
	{ 300.0f, 400.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo AirportBuoy_obj_hit_info
    = { 0x1, 0x80000000, -100.0f, AirportBuoy_hit_data_table };

static const TMapObjCollisionData AirportBuoy_map_collision_data[] = {
	{ "Buoy", 1 },
};

static const TMapObjCollisionInfo AirportBuoy_map_collision_info
    = { 1, 1, AirportBuoy_map_collision_data };

static TMapObjData AirportBuoy_data = {
	"Buoy",
	0x400002BD,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&AirportBuoy_obj_hit_info,
	&AirportBuoy_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	300.0f,
	0x0000090A,
	0x00000000,
};

static TMapObjData dptCannon_data = {
	"dptCannon",
	0x1000001C,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000800,
	0x00000000,
};

static const TMapObjCollisionData dptKing_map_collision_data[] = {
	{ "dptKing", 2 },
	{ "dptKing", 1 },
};

static const TMapObjCollisionInfo dptKing_map_collision_info
    = { 2, 2, dptKing_map_collision_data };

static TMapObjData dptKing_data = {
	"dptKing",
	0x4000007C,
	"大型地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	&dptKing_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000800,
	0x00000000,
};

static const TMapObjCollisionData KoopaJrSubmarine_map_collision_data[] = {
	{ "KoopaJrSubmarine", 0 },
};

static const TMapObjCollisionInfo KoopaJrSubmarine_map_collision_info
    = { 1, 1, KoopaJrSubmarine_map_collision_data };

static TMapObjData KoopaJrSubmarine_data = {
	"KoopaJrSubmarine",
	0x4000007D,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	&KoopaJrSubmarine_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000800,
	0x00000000,
};

static const TMapObjAnimData monumentshine_anim_data[] = {
	{ "monumentshine.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo monumentshine_anim_info
    = { 1, 1, monumentshine_anim_data };

static const TMapObjHitDataTable monumentshine_hit_data_table[] = {
	{ 300.0f, 600.0f, 300.0f, 600.0f },
};

static const TMapObjHitInfo monumentshine_obj_hit_info
    = { 0x1, 0x81000000, -300.0f, monumentshine_hit_data_table };

static TMapObjData monumentshine_data = {
	"monumentshine",
	0x4000007C,
	"大型地形オブジェマネージャー",
	"オブジェクトグループ",
	&monumentshine_anim_info,
	&monumentshine_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	40.0f,
	0x00000000,
	0x00000000,
};

static const TMapObjAnimData belldolpic_anim_data[] = {
	{ "BellDolpic.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo belldolpic_anim_info
    = { 1, 1, belldolpic_anim_data };

static const TMapObjHitDataTable belldolpic_hit_data_table[] = {
	{ 300.0f, 600.0f, 300.0f, 600.0f },
};

static const TMapObjHitInfo belldolpic_obj_hit_info
    = { 0x1, 0x81000000, -300.0f, belldolpic_hit_data_table };

static TMapObjData belldolpic_data = {
	"belldolpic",
	0x4000007C,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&belldolpic_anim_info,
	&belldolpic_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	40.0f,
	0x00000000,
	0x00000000,
};

static const TMapObjAnimData DptWeathercock_anim_data[] = {
	{ "DptWeathercock.bmd", "dptweathercock", 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo DptWeathercock_anim_info
    = { 1, 1, DptWeathercock_anim_data };

static const TMapObjHitDataTable DptWeathercock_hit_data_table[] = {
	{ 0.0f, 0.0f, 80.0f, 280.0f },
};

static const TMapObjHitInfo DptWeathercock_obj_hit_info
    = { 0x1, 0x00000000, 0.0f, DptWeathercock_hit_data_table };

static TMapObjData dptWeathercock_data = {
	"dptWeathercock",
	0x4000007E,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&DptWeathercock_anim_info,
	&DptWeathercock_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000900,
	0x00000000,
};

static const TMapObjHitDataTable DptMonteFence_hit_data_table[] = {
	{ 50.0f, 80.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo DptMonteFence_obj_hit_info
    = { 0x1, 0x80000000, 0.0f, DptMonteFence_hit_data_table };

static const TMapObjCollisionData DptMonteFence_map_collision_data[] = {
	{ "dptMonteFence", 2 },
};

static const TMapObjCollisionInfo DptMonteFence_map_collision_info
    = { 1, 1, DptMonteFence_map_collision_data };

static TMapObjData dptMonteFence_data = {
	"dptMonteFence",
	0x40000080,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&DptMonteFence_obj_hit_info,
	&DptMonteFence_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000900,
	0x00000000,
};

static const TMapObjCollisionData DptCoronaFence_map_collision_data[] = {
	{ "dptCoronaFence", 2 },
};

static const TMapObjCollisionInfo DptCoronaFence_map_collision_info
    = { 1, 1, DptCoronaFence_map_collision_data };

static TMapObjData dptCoronaFence_data = {
	"dptCoronaFence",
	0x4000007F,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	&DptCoronaFence_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000800,
	0x00000000,
};

static const TMapObjHitDataTable MapSmoke_hit_data_table[] = {
	{ 0.0f, 0.0f, 50.0f, 100.0f },
};

static const TMapObjHitInfo MapSmoke_obj_hit_info
    = { 0x1, 0x00000000, 0.0f, MapSmoke_hit_data_table };

static TMapObjData MapSmoke_data = {
	"MapSmoke",
	0x4000001F,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&no_data_anim_info,
	&MapSmoke_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000000,
	0x00000000,
};

static const TMapObjAnimData MareGate_anim_data[] = {
	{ "maregate.bmd", "maregate", 4, nullptr, nullptr },
};

static const TMapObjAnimDataInfo MareGate_anim_info
    = { 1, 1, MareGate_anim_data };

static TMapObjData MareGate_data = {
	"maregate",
	0x40000081,
	"大型地形オブジェマネージャー",
	"オブジェクトグループ",
	&MareGate_anim_info,
	&no_data_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000000,
	0x00000000,
};

static TMapObjData DemoCannon_data = {
	"demoCannon",
	0x40000082,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000001,
	0x00000000,
};

static const TMapObjAnimData NozzleDoor_anim_data[] = {
	{ "nozzleDoor.bmd", nullptr, 0, nullptr, nullptr },
	{ "nozzleDoor.bmd", "nozzledoor", 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo NozzleDoor_anim_info
    = { 2, 2, NozzleDoor_anim_data };

static const TMapObjHitDataTable NozzleDoor_hit_data_table[] = {
	{ 100.0f, 300.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo NozzleDoor_obj_hit_info
    = { 0x1, 0x80000000, 0.0f, NozzleDoor_hit_data_table };

static const TMapObjCollisionData NozzleDoor_map_collision_data[] = {
	{ "nozzleDoor", 2 },
};

static const TMapObjCollisionInfo NozzleDoor_map_collision_info
    = { 1, 1, NozzleDoor_map_collision_data };

static TMapObjData NozzleDoor_data = {
	"nozzleDoor",
	0x40000083,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&NozzleDoor_anim_info,
	&NozzleDoor_obj_hit_info,
	&NozzleDoor_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00004000,
	0x00000000,
};

static const TMapObjAnimData DokanGate_anim_data[] = {
	{ "efDokanGate.bmd", "efdokangate", 4, nullptr, nullptr },
};

static const TMapObjAnimDataInfo DokanGate_anim_info
    = { 1, 1, DokanGate_anim_data };

static TMapObjData DokanGate_data = {
	"DokanGate",
	0x40000084,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&DokanGate_anim_info,
	&no_data_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00008000,
	0x00000000,
};

static const TMapObjHitDataTable Crane_hit_data_table[] = {
	{ 2050.0f, 1700.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo Crane_obj_hit_info
    = { 0x1, 0x90000000, 600.0f, Crane_hit_data_table };

static const TMapObjCollisionData crane_map_collision_data[] = {
	{ "crane", 1 },
};

static const TMapObjCollisionInfo crane_map_collision_info
    = { 1, 1, crane_map_collision_data };

static TMapObjData crane_data = {
	"crane",
	0x400000B4,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&Crane_obj_hit_info,
	&crane_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000907,
	0x00000000,
};

static const TMapObjCollisionData crane90Scene2_map_collision_data[] = {
	{ "crane90Scene2", 1 },
};

static const TMapObjCollisionInfo crane90Scene2_map_collision_info
    = { 1, 1, crane90Scene2_map_collision_data };

static TMapObjData crane90Scene2_data = {
	"crane90Scene2",
	0x400000B4,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&Crane_obj_hit_info,
	&crane90Scene2_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000907,
	0x00000000,
};

static const TMapObjCollisionData crane180Scene1_map_collision_data[] = {
	{ "crane180Scene1", 1 },
};

static const TMapObjCollisionInfo crane180Scene1_map_collision_info
    = { 1, 1, crane180Scene1_map_collision_data };

static TMapObjData crane180Scene1_data = {
	"crane180Scene1",
	0x400000B4,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&Crane_obj_hit_info,
	&crane180Scene1_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000907,
	0x00000000,
};

static const TMapObjHitDataTable CraneUpDown_hit_data_table[] = {
	{ 1600.0f, 1000.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo CraneUpDown_obj_hit_info
    = { 0x1, 0x90000000, -500.0f, CraneUpDown_hit_data_table };

static const TMapObjCollisionData crane_updown_map_collision_data[] = {
	{ "craneUpDown", 1 },
};

static const TMapObjCollisionInfo crane_updown_map_collision_info
    = { 1, 1, crane_updown_map_collision_data };

static TMapObjData craneUpDown_data = {
	"craneUpDown",
	0x400000B4,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&CraneUpDown_obj_hit_info,
	&crane_updown_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000807,
	0x00000000,
};

static const TMapObjHitDataTable crane_cargo_hit_data_table[] = {
	{ 600.0f, 700.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo crane_cargo_obj_hit_info
    = { 0x1, 0x90000000, -1650.0f, crane_cargo_hit_data_table };

static const TMapObjCollisionData crane_cargo_map_collision_data[] = {
	{ "craneCargoUpDown", 1 },
};

static const TMapObjCollisionInfo crane_cargo_map_collision_info
    = { 1, 1, crane_cargo_map_collision_data };

static TMapObjData craneCargoUpDown_data = {
	"craneCargoUpDown",
	0x400000B4,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&crane_cargo_obj_hit_info,
	&crane_cargo_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000807,
	0x00000000,
};

static const TMapObjCollisionData tank_map_collision_data[] = {
	{ "tank", 2 },
};

static const TMapObjCollisionInfo tank_map_collision_info
    = { 1, 1, tank_map_collision_data };

static TMapObjData tank_data = {
	"tank",
	0x400000B5,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	&tank_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000901,
	0x00000000,
};

static const TMapObjCollisionData container_map_collision_data[] = {
	{ "container", 2 },
};

static const TMapObjCollisionInfo container_map_collision_info
    = { 1, 1, container_map_collision_data };

static TMapObjData container_data = {
	"container",
	0x400000B6,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	&container_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000101,
	0x00000000,
};

static const TMapObjHitDataTable Submarine_hit_data_table[] = {
	{ 400.0f, 100.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo Submarine_obj_hit_info
    = { 0x1, 0x90000000, 0.0f, Submarine_hit_data_table };

static const TMapObjCollisionData submarine_map_collision_data[] = {
	{ "submarine", 2 },
	{ "submarine", 1 },
};

static const TMapObjCollisionInfo submarine_map_collision_info
    = { 2, 2, submarine_map_collision_data };

static TMapObjData submarine_data = {
	"submarine",
	0x400000B7,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&Submarine_obj_hit_info,
	&submarine_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000801,
	0x00000000,
};

static const TMapObjHitDataTable ricco_ship_hit_data_table[] = {
	{ 700.0f, 400.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo ricco_ship_obj_hit_info
    = { 0x1, 0x80000000, 0.0f, ricco_ship_hit_data_table };

static const TMapObjCollisionData riccoShip_map_collision_data[] = {
	{ "riccoShip", 1 },
};

static const TMapObjCollisionInfo riccoShip_map_collision_info
    = { 1, 1, riccoShip_map_collision_data };

static TMapObjData riccoShip_data = {
	"riccoShip",
	0x400000BA,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&ricco_ship_obj_hit_info,
	&riccoShip_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000909,
	0x00000000,
};

static const TMapObjHitDataTable ricco_pole_hit_data_table[] = {
	{ 0.0f, 0.0f, 60.0f, 2800.0f },
};

static const TMapObjHitInfo ricco_pole_obj_hit_info
    = { 0x1, 0x00000000, 0.0f, ricco_pole_hit_data_table };

static TMapObjData riccoPole_data = {
	"riccoPole",
	0x4000002F,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&no_data_anim_info,
	&ricco_pole_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x000001C0,
	0x00000000,
};

static const TMapObjHitDataTable riccoLog_hit_data_table[] = {
	{ 500.0f, 400.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo riccoLog_obj_hit_info
    = { 0x1, 0x80000000, -200.0f, riccoLog_hit_data_table };

static const TMapObjCollisionData riccoLog_map_collision_data[] = {
	{ "riccoLog", 1 },
};

static const TMapObjCollisionInfo riccoLog_map_collision_info
    = { 1, 1, riccoLog_map_collision_data };

static TMapObjData riccoLog_data = {
	"riccoLog",
	0x4000000F,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&riccoLog_obj_hit_info,
	&riccoLog_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x0000090B,
	0x00000000,
};

static const TMapObjAnimData gesoSurfboard_anim_data[] = {
	{ "surf_geso.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo gesoSurfboard_anim_info
    = { 1, 1, gesoSurfboard_anim_data };

static const TMapObjHitDataTable gesoSurfboard_hit_data_table[] = {
	{ 40.0f, 40.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo gesoSurfboard_obj_hit_info
    = { 0x1, 0x80000000, 0.0f, gesoSurfboard_hit_data_table };

static TMapObjData gesoSurfboard_data = {
	"GesoSurfBoard",
	0x400000BC,
	"アイテムマネージャー",
	"アイテムグループ",
	&gesoSurfboard_anim_info,
	&gesoSurfboard_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000001,
	0x00000000,
};

static TMapObjData gesoSurfboardStatic_data = {
	"GesoSurfBoardStatic",
	0x400000BC,
	"アイテムマネージャー",
	"アイテムグループ",
	&gesoSurfboard_anim_info,
	&gesoSurfboard_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00080801,
	0x00000000,
};

static const TMapObjHitDataTable riccoWatermill_hit_data_table[] = {
	{ 0.0f, 0.0f, 400.0f, 600.0f },
};

static const TMapObjHitInfo riccoWatermill_obj_hit_info
    = { 0x1, 0x00000000, -300.0f, riccoWatermill_hit_data_table };

static TMapObjData riccoWatermill_data = {
	"riccoWatermill",
	0x400000BD,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&riccoWatermill_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000801,
	0x00000000,
};

static const TMapObjHitDataTable riccoShipLog_hit_data_table[] = {
	{ 2200.0f, 1250.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo riccoShipLog_obj_hit_info
    = { 0x1, 0x80000000, 0.0f, riccoShipLog_hit_data_table };

static const TMapObjCollisionData riccoShipLog_map_collision_data[] = {
	{ "riccoShipLog", 1 },
};

static const TMapObjCollisionInfo riccoShipLog_map_collision_info
    = { 1, 1, riccoShipLog_map_collision_data };

static TMapObjData riccoShipLog_data = {
	"riccoShipLog",
	0x400000BA,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&riccoShipLog_obj_hit_info,
	&riccoShipLog_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x0000090B,
	0x00000000,
};

static const TMapObjHitDataTable riccoShipDol_hit_data_table[] = {
	{ 2200.0f, 1500.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo riccoShipDol_obj_hit_info
    = { 0x1, 0x80000000, -400.0f, riccoShipDol_hit_data_table };

static const TMapObjCollisionData riccoShipDol_map_collision_data[] = {
	{ "riccoShipDol", 1 },
};

static const TMapObjCollisionInfo riccoShipDol_map_collision_info
    = { 1, 1, riccoShipDol_map_collision_data };

static TMapObjData riccoShipDol_data = {
	"riccoShipDol",
	0x400000BA,
	"大型地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&riccoShipDol_obj_hit_info,
	&riccoShipDol_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x0000090A,
	0x00000000,
};

static const TMapObjHitDataTable riccoYachtL_hit_data_table[] = {
	{ 1250.0f, 1200.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo riccoYachtL_obj_hit_info
    = { 0x1, 0x80000000, -600.0f, riccoYachtL_hit_data_table };

static const TMapObjCollisionData riccoYachtL_map_collision_data[] = {
	{ "riccoYachtL", 1 },
};

static const TMapObjCollisionInfo riccoYachtL_map_collision_info
    = { 1, 1, riccoYachtL_map_collision_data };

static TMapObjData riccoYachtL_data = {
	"riccoYachtL",
	0x400000BA,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&riccoYachtL_obj_hit_info,
	&riccoYachtL_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x0000090B,
	0x00000000,
};

static const TMapObjHitDataTable riccoYachtS_hit_data_table[] = {
	{ 700.0f, 900.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo riccoYachtS_obj_hit_info
    = { 0x1, 0x80000000, -500.0f, riccoYachtS_hit_data_table };

static const TMapObjCollisionData riccoYachtS_map_collision_data[] = {
	{ "riccoYachtS", 1 },
};

static const TMapObjCollisionInfo riccoYachtS_map_collision_info
    = { 1, 1, riccoYachtS_map_collision_data };

static TMapObjData riccoYachtS_data = {
	"riccoYachtS",
	0x400000BA,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&riccoYachtS_obj_hit_info,
	&riccoYachtS_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x0000090B,
	0x00000000,
};

static const TMapObjHitDataTable riccoBoatL_hit_data_table[] = {
	{ 1050.0f, 800.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo riccoBoatL_obj_hit_info
    = { 0x1, 0x80000000, -200.0f, riccoBoatL_hit_data_table };

static const TMapObjCollisionData riccoBoatL_map_collision_data[] = {
	{ "riccoBoatL", 1 },
};

static const TMapObjCollisionInfo riccoBoatL_map_collision_info
    = { 1, 1, riccoBoatL_map_collision_data };

static TMapObjData riccoBoatL_data = {
	"riccoBoatL",
	0x400000BA,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&riccoBoatL_obj_hit_info,
	&riccoBoatL_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x0000090B,
	0x00000000,
};

static const TMapObjHitDataTable riccoBoatS_hit_data_table[] = {
	{ 800.0f, 700.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo riccoBoatS_obj_hit_info
    = { 0x1, 0x80000000, -200.0f, riccoBoatS_hit_data_table };

static const TMapObjCollisionData riccoBoatS_map_collision_data[] = {
	{ "riccoBoatS", 1 },
};

static const TMapObjCollisionInfo riccoBoatS_map_collision_info
    = { 1, 1, riccoBoatS_map_collision_data };

static TMapObjData riccoBoatS_data = {
	"riccoBoatS",
	0x400000BA,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&riccoBoatS_obj_hit_info,
	&riccoBoatS_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x0000090B,
	0x00000000,
};

static const TMapObjCollisionData riccoBasket_map_collision_data[] = {
	{ "riccoBasket", 0 },
};

static const TMapObjCollisionInfo riccoBasket_map_collision_info
    = { 1, 1, riccoBasket_map_collision_data };

static TMapObjData riccoBasket_data = {
	"riccoBasket",
	0x400000BF,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	&riccoBasket_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000901,
	0x00000000,
};

static const TMapObjCollisionData riccoGangway_map_collision_data[] = {
	{ "riccoGangway", 0 },
};

static const TMapObjCollisionInfo riccoGangway_map_collision_info
    = { 1, 1, riccoGangway_map_collision_data };

static TMapObjData riccoGangway_data = {
	"riccoGangway",
	0x400000C0,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	&riccoGangway_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000901,
	0x00000000,
};

static const TMapObjCollisionData riccoShipFish_map_collision_data[] = {
	{ "riccoShipFish", 0 },
};

static const TMapObjCollisionInfo riccoShipFish_map_collision_info
    = { 1, 1, riccoShipFish_map_collision_data };

static TMapObjData riccoShipFish_data = {
	"riccoShipFish",
	0x400000C3,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	&riccoShipFish_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000901,
	0x00000000,
};

static const TMapObjCollisionData riccoShipGeso_map_collision_data[] = {
	{ "riccoShipGeso", 0 },
};

static const TMapObjCollisionInfo riccoShipGeso_map_collision_info
    = { 1, 1, riccoShipGeso_map_collision_data };

static TMapObjData riccoShipGeso_data = {
	"riccoShipGeso",
	0x400000C3,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	&riccoShipGeso_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000901,
	0x00000000,
};

static const TMapObjCollisionData riccoSwitch_map_collision_data[] = {
	{ "riccoSwitch", 2 },
};

static const TMapObjCollisionInfo riccoSwitch_map_collision_info
    = { 1, 1, riccoSwitch_map_collision_data };

static TMapObjData riccoSwitch_data = {
	"riccoSwitch",
	0x400000C1,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	&riccoSwitch_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000901,
	0x00000000,
};

static TMapObjData riccoSwitchShine_data = {
	"riccoSwitchShine",
	0x400000C2,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&no_data_anim_info,
	&no_data_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000000,
	0x00000000,
};

static const TMapObjCollisionData riccoFenceMaze_map_collision_data[] = {
	{ "riccoFenceMaze", 0 },
};

static const TMapObjCollisionInfo riccoFenceMaze_map_collision_info
    = { 1, 1, riccoFenceMaze_map_collision_data };

static TMapObjData riccoFenceMaze_data = {
	"riccoFenceMaze",
	0x400000C4,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	&riccoFenceMaze_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000901,
	0x00000000,
};

static const TMapObjCollisionData riccoStand_map_collision_data[] = {
	{ "riccoStand", 0 },
};

static const TMapObjCollisionInfo riccoStand_map_collision_info
    = { 1, 1, riccoStand_map_collision_data };

static TMapObjData riccoStand_data = {
	"riccoStand",
	0x400000AA,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	&riccoStand_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000901,
	0x00000000,
};

static const TMapObjCollisionData riccoGrille_map_collision_data[] = {
	{ "riccoGrille", 0 },
};

static const TMapObjCollisionInfo riccoGrille_map_collision_info
    = { 1, 1, riccoGrille_map_collision_data };

static TMapObjData riccoGrille_data = {
	"riccoGrille",
	0x400000AB,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	&riccoGrille_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000901,
	0x00000000,
};

static const TMapObjCollisionData riccoArrow_map_collision_data[] = {
	{ "riccoArrow", 0 },
};

static const TMapObjCollisionInfo riccoArrow_map_collision_info
    = { 1, 1, riccoArrow_map_collision_data };

static TMapObjData riccoArrow_data = {
	"riccoArrow",
	0x400000AC,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	&riccoArrow_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000901,
	0x00000000,
};

static const TMapObjCollisionData riccoStreetStall_map_collision_data[] = {
	{ "riccoStreetStall", 0 },
};

static const TMapObjCollisionInfo riccoStreetStall_map_collision_info
    = { 1, 1, riccoStreetStall_map_collision_data };

static TMapObjData riccoStreetStall_data = {
	"riccoStreetStall",
	0x400000AD,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	&riccoStreetStall_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000901,
	0x00000000,
};

static const TMapObjCollisionData riccoHericopter_map_collision_data[] = {
	{ "riccoHericopter", 0 },
};

static const TMapObjCollisionInfo riccoHericopter_map_collision_info
    = { 1, 1, riccoHericopter_map_collision_data };

static TMapObjData riccoHericopter_data = {
	"riccoHericopter",
	0x400000AE,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	&riccoHericopter_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000901,
	0x00000000,
};

static const TMapObjHitDataTable surfgeso_hit_data_table[] = {
	{ 40.0f, 50.0f, 40.0f, 50.0f },
};

static const TMapObjHitInfo surfgeso_obj_hit_info
    = { 0x1, 0x80000000, 0.0f, surfgeso_hit_data_table };

static const TMapObjAnimData surfgeso_anim_data[] = {
	{ "surfgeso.bmd", nullptr, 0, nullptr, nullptr },
};

static TMapObjData surfgeso_red_data = {
	"SurfGesoRed",
	0x400000C5,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&no_data_anim_info,
	&surfgeso_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000001,
	0x00000000,
};

static TMapObjData surfgeso_yellow_data = {
	"SurfGesoYellow",
	0x400000C6,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&no_data_anim_info,
	&surfgeso_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000001,
	0x00000000,
};

static TMapObjData surfgeso_green_data = {
	"SurfGesoGreen",
	0x400000C7,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&no_data_anim_info,
	&surfgeso_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000001,
	0x00000000,
};

static const TMapObjAnimData windmill_anim_data[] = {
	{ "bigWindmill.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo windmill_anim_info
    = { 1, 1, windmill_anim_data };

static const TMapObjHitDataTable Windmill_hit_data_table[] = {
	{ 2500.0f, 5000.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo Windmill_obj_hit_info
    = { 0x1, 0x80000000, -2500.0f, Windmill_hit_data_table };

static const TMapObjCollisionData windmill_map_collision_data[] = {
	{ "bigWindmill", 1 },
};

static const TMapObjCollisionInfo windmill_map_collision_info
    = { 1, 1, windmill_map_collision_data };

static TMapObjData big_windmill_data = {
	"bigWindmill",
	0x40000096,
	"大型地形オブジェマネージャー",
	"オブジェクトグループ",
	&windmill_anim_info,
	&Windmill_obj_hit_info,
	&windmill_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000D26,
	0x00000000,
};

static TMapObjData windmill_far_data = {
	"windmill_far",
	0x400000A7,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&windmill_anim_info,
	&no_data_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000C01,
	0x00000000,
};

static const TMapObjHitDataTable MiniWindmill_hit_data_table[] = {
	{ 0.0f, 0.0f, 20.0f, 6400.0f },
};

static const TMapObjHitInfo MiniWindmill_obj_hit_info
    = { 0x1, 0x80000000, 0.0f, MiniWindmill_hit_data_table };

static const TMapObjCollisionData MiniWindmillL_map_collision_data[] = {
	{ "MiniWindmillL", 2 },
};

static const TMapObjCollisionInfo MiniWindmillL_map_collision_info
    = { 1, 1, MiniWindmillL_map_collision_data };

static TMapObjData MiniWindmillL_data = {
	"MiniWindmillL",
	0x400000A0,
	"大型地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&MiniWindmill_obj_hit_info,
	&MiniWindmillL_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00004900,
	0x00000000,
};

static const TMapObjCollisionData MiniWindmillS_map_collision_data[] = {
	{ "MiniWindmillS", 2 },
};

static const TMapObjCollisionInfo MiniWindmillS_map_collision_info
    = { 1, 1, MiniWindmillS_map_collision_data };

static TMapObjData MiniWindmillS_data = {
	"MiniWindmillS",
	0x400000A0,
	"大型地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	&MiniWindmillS_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00004C00,
	0x00000000,
};

static const TMapObjCollisionData windmill_roof_map_collision_data[] = {
	{ "WindmillRoof", 2 },
};

static const TMapObjCollisionInfo windmill_roof_map_collision_info
    = { 1, 1, windmill_roof_map_collision_data };

static TMapObjData WindmillRoof_data = {
	"WindmillRoof",
	0x400000A6,
	"大型地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	&windmill_roof_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000D00,
	0x00000000,
};

static const TMapObjAnimData lamp_bianco_anim_data[] = {
	{ "lampBianco.bmd", nullptr, 0, nullptr, nullptr },
	{ nullptr, nullptr, 0, nullptr, nullptr },
	{ nullptr, nullptr, 0, nullptr, nullptr },
	{ nullptr, nullptr, 0, nullptr, nullptr },
	{ nullptr, nullptr, 0, nullptr, nullptr },
	{ nullptr, nullptr, 0, nullptr, nullptr },
	{ nullptr, nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo lamp_bianco_anim_info
    = { 7, 2, lamp_bianco_anim_data };

static const TMapObjHitDataTable lamp_bianco_hit_data_table[] = {
	{ 0.0f, 0.0f, 20.0f, 2400.0f },
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo lamp_bianco_obj_hit_info
    = { 0x3, 0x9C000000, 0.0f, lamp_bianco_hit_data_table };

static const TMapObjCollisionData lamp_bianco_map_collision_data[] = {
	{ "lampBianco", 2 },
	{ "lampBianco", 1 },
};

static const TMapObjCollisionInfo lamp_bianco_map_collision_info
    = { 2, 2, lamp_bianco_map_collision_data };

static TMapObjSinkData lamp_bianco_sink_data = { 0.0f, 3.0f };

static TMapObjData lamp_bianco_data = {
	"lampBianco",
	0x400000A2,
	"大型地形オブジェマネージャー",
	"オブジェクトグループ",
	&lamp_bianco_anim_info,
	&lamp_bianco_obj_hit_info,
	&lamp_bianco_map_collision_info,
	nullptr,
	nullptr,
	&lamp_bianco_sink_data,
	nullptr,
	nullptr,
	0.0f,
	0x01000D40,
	0x00000000,
};

static const TMapObjCollisionData RootPakkun_map_collision_data[] = {
	{ "rootPaku", 0 },
};

static const TMapObjCollisionInfo RootPakkun_map_collision_info
    = { 1, 1, RootPakkun_map_collision_data };

static TMapObjData root_pakkun_data = {
	"rootPaku",
	0x40000097,
	"大型地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	&RootPakkun_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000400,
	0x00000000,
};

static const TMapObjHitDataTable WindmillBlock_hit_data_table[] = {
	{ 300.0f, 300.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo WindmillBlock_obj_hit_info
    = { 0x1, 0x80000000, -200.0f, WindmillBlock_hit_data_table };

static const TMapObjCollisionData windmill_block_map_collision_data[] = {
	{ "bigWindmillBlock", 1 },
};

static const TMapObjCollisionInfo windmill_block_map_collision_info
    = { 1, 1, windmill_block_map_collision_data };

static TMapObjData windmill_block_data = {
	"bigWindmillBlock",
	0x400000A8,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&WindmillBlock_obj_hit_info,
	&windmill_block_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	200.0f,
	0x00002D0A,
	0x00000000,
};

static const TMapObjHitDataTable PolluterPakkun_hit_data_table[] = {
	{ 0.0f, 0.0f, 100.0f, 100.0f },
};

static const TMapObjHitInfo PolluterPakkun_obj_hit_info
    = { 0x1, 0x00000000, 0.0f, PolluterPakkun_hit_data_table };

static TMapObjData PolluterPakkun_data = {
	"PolluterPakkun",
	0x40000099,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&PolluterPakkun_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000800,
	0x00000000,
};

static const TMapObjAnimData BiaBell_anim_data[] = {
	{ "BiaBell.bmd", nullptr, 0, nullptr, nullptr },
	{ nullptr, "biabella", 0, nullptr, nullptr },
	{ nullptr, "biabellb", 0, nullptr, nullptr },
	{ nullptr, "biabellc", 0, nullptr, nullptr },
	{ nullptr, "biabelld", 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo BiaBell_anim_info
    = { 5, 5, BiaBell_anim_data };

static const TMapObjHitDataTable BiaBell_hit_data_table[] = {
	{ 100.0f, 100.0f, 100.0f, 100.0f },
};

static const TMapObjHitInfo BiaBell_obj_hit_info
    = { 0x1, 0x80000000, 0.0f, BiaBell_hit_data_table };

static TMapObjData BiaBell_data = {
	"BiaBell",
	0x4000009E,
	"ファークリップ地形オブジェマネージャー",
	"オブジェクトグループ",
	&BiaBell_anim_info,
	&BiaBell_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00004800,
	0x00000000,
};

static const TMapObjAnimData watermill00_anim_data[] = {
	{ "BiaWatermill00.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo watermill00_anim_info
    = { 1, 1, watermill00_anim_data };

static const TMapObjHitDataTable BiaWatermill00_hit_data_table[] = {
	{ 0.0f, 0.0f, 320.0f, 500.0f },
};

static const TMapObjHitInfo BiaWatermill00_obj_hit_info
    = { 0x1, 0x00000000, -300.0f, BiaWatermill00_hit_data_table };

static TMapObjData BiaWatermill00_data = {
	"BiaWatermill00",
	0x4000009F,
	"大型地形オブジェマネージャー",
	"オブジェクトグループ",
	&watermill00_anim_info,
	&BiaWatermill00_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00004904,
	0x00000000,
};

static const TMapObjHitDataTable BiaWatermill01_hit_data_table[] = {
	{ 1200.0f, 2000.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo BiaWatermill01_obj_hit_info
    = { 0x1, 0x91000000, -1000.0f, BiaWatermill01_hit_data_table };

static const TMapObjCollisionData BiaWatermill01_map_collision_data[] = {
	{ "BiaWatermill01", 1 },
};

static const TMapObjCollisionInfo BiaWatermill01_map_collision_info
    = { 1, 1, BiaWatermill01_map_collision_data };

static TMapObjData BiaWatermill01_data = {
	"BiaWatermill01",
	0x4000009A,
	"大型地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&BiaWatermill01_obj_hit_info,
	&BiaWatermill01_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00004806,
	0x00000000,
};

static const TMapObjHitDataTable BiaWatermillVertical_hit_data_table[] = {
	{ 500.0f, 1850.0f, 500.0f, 1850.0f },
};

static const TMapObjHitInfo BiaWatermillVertical_obj_hit_info
    = { 0x1, 0x91000000, 0.0f, BiaWatermillVertical_hit_data_table };

static const TMapObjCollisionData BiaWatermillVertical_map_collision_data[] = {
	{ "BiaWatermillVertical", 1 },
};

static const TMapObjCollisionInfo BiaWatermillVertical_map_collision_info
    = { 1, 1, BiaWatermillVertical_map_collision_data };

static TMapObjData BiaWatermillVertical_data = {
	"BiaWatermillVertical",
	0x400000A1,
	"大型地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&BiaWatermillVertical_obj_hit_info,
	&BiaWatermillVertical_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000806,
	0x00000000,
};

static const TMapObjHitDataTable BiaTurnBridge_hit_data_table[] = {
	{ 1200.0f, 250.0f, 100.0f, 100.0f },
};

static const TMapObjHitInfo BiaTurnBridge_obj_hit_info
    = { 0x1, 0x80000000, 0.0f, BiaTurnBridge_hit_data_table };

static const TMapObjCollisionData BiaTurnBridge_map_collision_data[] = {
	{ "BiaTurnBridge", 1 },
};

static const TMapObjCollisionInfo BiaTurnBridge_map_collision_info
    = { 1, 1, BiaTurnBridge_map_collision_data };

static TMapObjData BiaTurnBridge_data = {
	"BiaTurnBridge",
	0x4000009B,
	"大型地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&BiaTurnBridge_obj_hit_info,
	&BiaTurnBridge_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000806,
	0x00000000,
};

static const TMapObjHitDataTable LeafBoat_hit_data_table[] = {
	{ 300.0f, 400.0f, 300.0f, 400.0f },
};

static const TMapObjHitInfo LeafBoat_obj_hit_info
    = { 0x1, 0xD0000000, -200.0f, LeafBoat_hit_data_table };

static const TMapObjCollisionData LeafBoat_map_collision_data[] = {
	{ "LeafBoat", 1 },
};

static const TMapObjCollisionInfo LeafBoat_map_collision_info
    = { 1, 1, LeafBoat_map_collision_data };

static TMapObjData LeafBoat_data = {
	"LeafBoat",
	0x4000009C,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&LeafBoat_obj_hit_info,
	&LeafBoat_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000806,
	0x00000000,
};

static TMapObjData LeafBoatRotten_data = {
	"LeafBoatRotten",
	0x400000A5,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&LeafBoat_obj_hit_info,
	&LeafBoat_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00040006,
	0x00000000,
};

static const TMapObjAnimData LampSeesaw_anim_data[] = {
	{ "lampBianco.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo LampSeesaw_anim_info
    = { 1, 1, LampSeesaw_anim_data };

static const TMapObjHitDataTable LampSeesaw_hit_data_table[] = {
	{ 300.0f, 2600.0f, 50.0f, 2400.0f },
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo LampSeesaw_obj_hit_info
    = { 0x3, 0x9C000000, 0.0f, LampSeesaw_hit_data_table };

static const TMapObjCollisionData LampSeesaw_map_collision_data[] = {
	{ "lampBianco", 1 },
};

static const TMapObjCollisionInfo LampSeesaw_map_collision_info
    = { 1, 1, LampSeesaw_map_collision_data };

static TMapObjData LampSeesaw_data = {
	"LampSeesaw",
	0x4000009D,
	"大型地形オブジェマネージャー",
	"オブジェクトグループ",
	&LampSeesaw_anim_info,
	&LampSeesaw_obj_hit_info,
	&LampSeesaw_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000842,
	0x00000000,
};

static const TMapObjCollisionData BiancoDoor_map_collision_data[] = {
	{ "BiaDoor", 0 },
};

static const TMapObjCollisionInfo BiancoDoor_map_collision_info
    = { 1, 1, BiancoDoor_map_collision_data };

static TMapObjData BiancoDoor_data = {
	"BiaDoor",
	0x400000A3,
	"大型地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	&BiancoDoor_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000800,
	0x00000000,
};

static const TMapObjCollisionData BiaBridge_map_collision_data[] = {
	{ "BiaBridge", 0 },
};

static const TMapObjCollisionInfo BiaBridge_map_collision_info
    = { 1, 1, BiaBridge_map_collision_data };

static TMapObjData BiaBridge_data = {
	"BiaBridge",
	0x400000A4,
	"大型地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	&BiaBridge_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000800,
	0x00000000,
};

static const TMapObjAnimData SandBird_anim_data[] = {
	{ "SandBird.bmd", "sandbird", 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo SandBird_anim_info
    = { 1, 1, SandBird_anim_data };

static TMapObjData SandBird_data = {
	"SandBird",
	0x400000C9,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&SandBird_anim_info,
	&no_data_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00001001,
	0x00000000,
};

static const TMapObjHitDataTable SandBirdBlock_hit_data_table[] = {
	{ 500.0f, 1000.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo SandBirdBlock_obj_hit_info
    = { 0x1, 0x80000000, -500.0f, SandBirdBlock_hit_data_table };

static const TMapObjCollisionData SandBirdBlock_map_collision_data[] = {
	{ "SandBirdBlock", 1 },
	{ nullptr, 0 },
};

static const TMapObjCollisionInfo SandBirdBlock_map_collision_info
    = { 2, 2, SandBirdBlock_map_collision_data };

static TMapObjData SandBirdBlock_data = {
	"SandBirdBlock",
	0x400002C9,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&SandBirdBlock_obj_hit_info,
	&SandBirdBlock_map_collision_info,
	&normal_sound_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x0000010B,
	0x00000000,
};

static const TMapObjAnimData SkyIsland_anim_data[] = {
	{ "SkyIsland.bmd", "skyisland", 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo SkyIsland_anim_info
    = { 1, 1, SkyIsland_anim_data };

static const TMapObjCollisionData SkyIsland_map_collision_data[] = {
	{ "SkyIsland", 0 },
};

static const TMapObjCollisionInfo SkyIsland_map_collision_info
    = { 1, 1, SkyIsland_map_collision_data };

static TMapObjData SkyIsland_data = {
	"SkyIsland",
	0x400002BD,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&SkyIsland_anim_info,
	&no_data_obj_hit_info,
	&SkyIsland_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	80.0f,
	0x00000901,
	0x00000000,
};

static const TMapObjAnimData SandLeaf_anim_data[] = {
	{ "SandLeaf.bmd", "sandleaf_wait", 0, nullptr, nullptr },
	{ nullptr, "sandleaf_set", 0, nullptr, nullptr },
	{ nullptr, "sandleaf_water", 0, nullptr, nullptr },
	{ nullptr, "sandleaf_end", 0, nullptr, nullptr },
	{ nullptr, "sandleaf_end", 5, nullptr, nullptr },
};

static const TMapObjAnimDataInfo SandLeaf_anim_info
    = { 5, 1, SandLeaf_anim_data };

static const TMapObjHitDataTable sand_leaf_hit_data_table[] = {
	{ 0.0f, 0.0f, 50.0f, 180.0f },
};

static const TMapObjHitInfo sand_leaf_obj_hit_info
    = { 0x1, 0x00000000, 0.0f, sand_leaf_hit_data_table };

static TMapObjData SandLeaf_data = {
	"SandLeaf",
	0x400000CA,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&SandLeaf_anim_info,
	&sand_leaf_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000000,
	0x00000000,
};

static const TMapObjAnimData SandLeafBase00_anim_data[] = {
	{ "SandLeafBase00.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo SandLeafBase00_anim_info
    = { 1, 1, SandLeafBase00_anim_data };

static const TMapObjCollisionData SandLeafBase00_map_collision_data[] = {
	{ "SandLeafBase00", 2 },
	{ "SandLeafBase00", 1 },
};

static const TMapObjCollisionInfo SandLeafBase00_map_collision_info
    = { 2, 2, SandLeafBase00_map_collision_data };

static TMapObjData SandLeafBase00_data = {
	"SandLeafBase00",
	0x400000CB,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&SandLeafBase00_anim_info,
	&no_data_obj_hit_info,
	&SandLeafBase00_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000005,
	0x00000000,
};

static const TMapObjAnimData SandLeafBase01_anim_data[] = {
	{ "SandLeafBase01.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo SandLeafBase01_anim_info
    = { 1, 1, SandLeafBase01_anim_data };

static const TMapObjCollisionData SandLeafBase01_map_collision_data[] = {
	{ "SandLeafBase01", 2 },
	{ "SandLeafBase01", 1 },
};

static const TMapObjCollisionInfo SandLeafBase01_map_collision_info
    = { 2, 2, SandLeafBase01_map_collision_data };

static TMapObjData SandLeafBase01_data = {
	"SandLeafBase01",
	0x400000CB,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&SandLeafBase01_anim_info,
	&no_data_obj_hit_info,
	&SandLeafBase01_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000005,
	0x00000000,
};

static const TMapObjAnimData SandLeafBase02_anim_data[] = {
	{ "SandLeafBase02.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo SandLeafBase02_anim_info
    = { 1, 1, SandLeafBase02_anim_data };

static const TMapObjCollisionData SandLeafBase02_map_collision_data[] = {
	{ "SandLeafBase02", 2 },
	{ "SandLeafBase02", 1 },
};

static const TMapObjCollisionInfo SandLeafBase02_map_collision_info
    = { 2, 2, SandLeafBase02_map_collision_data };

static TMapObjData SandLeafBase02_data = {
	"SandLeafBase02",
	0x400000CB,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&SandLeafBase02_anim_info,
	&no_data_obj_hit_info,
	&SandLeafBase02_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000005,
	0x00000000,
};

static const TMapObjAnimData SandLeafBase03_anim_data[] = {
	{ "SandLeafBase03.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo SandLeafBase03_anim_info
    = { 1, 1, SandLeafBase03_anim_data };

static const TMapObjCollisionData SandLeafBase03_map_collision_data[] = {
	{ "SandLeafBase03", 2 },
	{ "SandLeafBase03", 1 },
};

static const TMapObjCollisionInfo SandLeafBase03_map_collision_info
    = { 2, 2, SandLeafBase03_map_collision_data };

static TMapObjData SandLeafBase03_data = {
	"SandLeafBase03",
	0x400000CB,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&SandLeafBase03_anim_info,
	&no_data_obj_hit_info,
	&SandLeafBase03_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000005,
	0x00000000,
};

static const TMapObjAnimData SandBomb_anim_data[] = {
	{ "SandBomb.bmd", "sandbomb_wait", 0, nullptr, nullptr },
	{ nullptr, "sandbomb_water", 0, nullptr, nullptr },
	{ nullptr, "sandbomb_water", 5, nullptr, nullptr },
	{ nullptr, "sandbomb_ignition", 0, nullptr, nullptr },
	{ nullptr, "sandbomb_ignition", 5, nullptr, nullptr },
	{ nullptr, "sandbomb_ignition", 3, nullptr, nullptr },
	{ nullptr, "sandbomb_up", 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo SandBomb_anim_info
    = { 7, 1, SandBomb_anim_data };

static const TMapObjHitDataTable sand_bomb_hit_data_table[] = {
	{ 0.0f, 0.0f, 50.0f, 180.0f },
};

static const TMapObjHitInfo sand_bomb_obj_hit_info
    = { 0x1, 0x00000000, 0.0f, sand_bomb_hit_data_table };

static TMapObjData SandBomb_data = {
	"SandBomb",
	0x400000CC,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&SandBomb_anim_info,
	&sand_bomb_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000001,
	0x00000000,
};

static const TMapObjHitDataTable mirror_L_hit_data_table[] = {
	{ 1800.0f, 2000.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo mirror_L_obj_hit_info
    = { 0x1, 0x90000000, 0.0f, mirror_L_hit_data_table };

static const TMapObjCollisionData mirror_L_map_collision_data[] = {
	{ "mirrorL", 1 },
	{ "mirrorL", 2 },
};

static const TMapObjCollisionInfo mirror_L_map_collision_info
    = { 2, 2, mirror_L_map_collision_data };

static TMapObjData MirrorL_data = {
	"mirrorL",
	0x400000CF,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&mirror_L_obj_hit_info,
	&mirror_L_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00004909,
	0x00000000,
};

static const TMapObjHitDataTable mirror_M_hit_data_table[] = {
	{ 1400.0f, 1700.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo mirror_M_obj_hit_info
    = { 0x1, 0x90000000, 0.0f, mirror_M_hit_data_table };

static const TMapObjCollisionData mirror_M_map_collision_data[] = {
	{ "mirrorM", 1 },
	{ "mirrorM", 2 },
};

static const TMapObjCollisionInfo mirror_M_map_collision_info
    = { 2, 2, mirror_M_map_collision_data };

static TMapObjData MirrorM_data = {
	"mirrorM",
	0x400000CF,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&mirror_M_obj_hit_info,
	&mirror_M_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00004909,
	0x00000000,
};

static const TMapObjHitDataTable mirrorS_hit_data_table[] = {
	{ 1200.0f, 1300.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo mirrorS_obj_hit_info
    = { 0x1, 0x90000000, 0.0f, mirrorS_hit_data_table };

static const TMapObjCollisionData mirrorS_map_collision_data[] = {
	{ "mirrorS", 1 },
	{ "mirrorS", 2 },
};

static const TMapObjCollisionInfo mirrorS_map_collision_info
    = { 2, 2, mirrorS_map_collision_data };

static TMapObjData MirrorS_data = {
	"mirrorS",
	0x400000CF,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&mirrorS_obj_hit_info,
	&mirrorS_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00004909,
	0x00000000,
};

static const TMapObjAnimData SandCastle_anim_data[] = {
	{ "SandCastle.bmd", nullptr, 0, nullptr, nullptr },
	{ nullptr, "sandcastle_up", 0, nullptr, nullptr },
	{ nullptr, "sandcastle_end", 0, nullptr, nullptr },
	{ nullptr, "sandcastle_end", 5, nullptr, nullptr },
};

static const TMapObjAnimDataInfo SandCastle_anim_info
    = { 4, 1, SandCastle_anim_data };

static const TMapObjCollisionData SandCastle_map_collision_data[] = {
	{ "SandCastle", 2 },
	{ "SandCastle", 1 },
};

static const TMapObjCollisionInfo SandCastle_map_collision_info
    = { 2, 2, SandCastle_map_collision_data };

static TMapObjData SandCastle_data = {
	"SandCastle",
	0x400000D3,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&SandCastle_anim_info,
	&no_data_obj_hit_info,
	&SandCastle_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000005,
	0x00000000,
};

static const TMapObjCollisionData MammaBlockRotate_map_collision_data[] = {
	{ "MammaBlockRotate", 1 },
	{ "MammaBlockRotate", 2 },
};

static const TMapObjCollisionInfo MammaBlockRotate_map_collision_info
    = { 2, 2, MammaBlockRotate_map_collision_data };

static TMapObjData MammaBlockRotate_data = {
	"MammaBlockRotate",
	0x400000D4,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	&MammaBlockRotate_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000805,
	0x00000000,
};

static const TMapObjCollisionData SandEgg_map_collision_data[] = {
	{ "SandEggNormal", 0 },
};

static const TMapObjCollisionInfo SandEgg_map_collision_info
    = { 1, 1, SandEgg_map_collision_data };

static TMapObjData SandEgg_data = {
	"SandEggNormal",
	0x400000D5,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	&SandEgg_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000021,
	0x00000000,
};

static const TMapObjCollisionData SandEggBroken_map_collision_data[] = {
	{ "SandEggBroken", 0 },
};

static const TMapObjCollisionInfo SandEggBroken_map_collision_info
    = { 1, 1, SandEggBroken_map_collision_data };

static TMapObjData SandEggBroken_data = {
	"SandEggBroken",
	0x400000D5,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	&SandEggBroken_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000121,
	0x00000000,
};

static TMapObjData ShiningStone_data = {
	"ShiningStone",
	0x400000D1,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000921,
	0x00000000,
};

static const TMapObjHitDataTable MammaSurfboard_hit_data_table[] = {
	{ 100.0f, 200.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo MammaSurfboard_obj_hit_info
    = { 0x1, 0x80000000, 0.0f, MammaSurfboard_hit_data_table };

static TMapObjData MammaSurfboard00_data = {
	"MammaSurfboard00",
	0x400000D6,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&MammaSurfboard_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000801,
	0x00000000,
};

static TMapObjData MammaSurfboard01_data = {
	"MammaSurfboard01",
	0x400000D6,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&MammaSurfboard_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000901,
	0x00000000,
};

static TMapObjData MammaSurfboard02_data = {
	"MammaSurfboard02",
	0x400000D6,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&MammaSurfboard_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000801,
	0x00000000,
};

static TMapObjData MammaSurfboard03_data = {
	"MammaSurfboard03",
	0x400000D6,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&MammaSurfboard_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000901,
	0x00000000,
};

static const TMapObjCollisionData MammaSurfboardStand_map_collision_data[] = {
	{ "MammaSurfboardStand", 0 },
};

static const TMapObjCollisionInfo MammaSurfboardStand_map_collision_info
    = { 1, 1, MammaSurfboardStand_map_collision_data };

static TMapObjData MammaSurfboardStand_data = {
	"MammaSurfboardStand",
	0x400000D7,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	&MammaSurfboardStand_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000901,
	0x00000000,
};

static const TMapObjHitDataTable MammaYacht00_hit_data_table[] = {
	{ 1000.0f, 800.0f, 50.0f, 1000.0f },
};

static const TMapObjHitInfo MammaYacht00_obj_hit_info
    = { 0x1, 0x80000000, 0.0f, MammaYacht00_hit_data_table };

static const TMapObjCollisionData MammaYacht00_map_collision_data[] = {
	{ "MammaYacht00", 0 },
};

static const TMapObjCollisionInfo MammaYacht00_map_collision_info
    = { 1, 1, MammaYacht00_map_collision_data };

static TMapObjData MammaYacht00_data = {
	"MammaYacht00",
	0x400000D8,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	&MammaYacht00_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000807,
	0x00000000,
};

static const TMapObjCollisionData MammaYacht01_map_collision_data[] = {
	{ "MammaYacht01", 0 },
};

static const TMapObjCollisionInfo MammaYacht01_map_collision_info
    = { 1, 1, MammaYacht01_map_collision_data };

static TMapObjData MammaYacht01_data = {
	"MammaYacht01",
	0x400000D8,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	&MammaYacht01_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000901,
	0x00000000,
};

static const TMapObjCollisionData MammaYacht02_map_collision_data[] = {
	{ "MammaYacht02", 0 },
};

static const TMapObjCollisionInfo MammaYacht02_map_collision_info
    = { 1, 1, MammaYacht02_map_collision_data };

static TMapObjData MammaYacht02_data = {
	"MammaYacht02",
	0x400000D8,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	&MammaYacht02_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000901,
	0x00000000,
};

static const TMapObjAnimData SandBombBase00_anim_data[] = {
	{ "SandBombBase00.bmd", nullptr, 0, nullptr, nullptr },
	{ nullptr, "sandbombbase_up", 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo SandBombBase00_anim_info
    = { 2, 1, SandBombBase00_anim_data };

static const TMapObjCollisionData SandBombBase00_map_collision_data[] = {
	{ "SandBombBase00", 2 },
	{ "SandBombBase00", 1 },
};

static const TMapObjCollisionInfo SandBombBase00_map_collision_info
    = { 2, 2, SandBombBase00_map_collision_data };

static TMapObjData SandBombBase00_data = {
	"SandBombBase00",
	0x400000CD,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&SandBombBase00_anim_info,
	&no_data_obj_hit_info,
	&SandBombBase00_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000809,
	0x00000000,
};

static const TMapObjAnimData SandBombBaseMushroom_anim_data[] = {
	{ "SandBombBaseMushroom.bmd", nullptr, 0, nullptr, nullptr },
	{ nullptr, "sandbombbase_up", 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo SandBombBaseMushroom_anim_info
    = { 2, 1, SandBombBaseMushroom_anim_data };

static const TMapObjCollisionData SandBombBaseMushroom_map_collision_data[] = {
	{ "SandBombBaseMushroom", 2 },
	{ "SandBombBaseMushroom", 1 },
};

static const TMapObjCollisionInfo SandBombBaseMushroom_map_collision_info
    = { 2, 2, SandBombBaseMushroom_map_collision_data };

static TMapObjData SandBombBaseMushroom_data = {
	"SandBombBaseMushroom",
	0x400000CD,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&SandBombBaseMushroom_anim_info,
	&no_data_obj_hit_info,
	&SandBombBaseMushroom_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000809,
	0x00000000,
};

static const TMapObjAnimData SandBombBasePyramid_anim_data[] = {
	{ "SandBombBasePyramid.bmd", nullptr, 0, nullptr, nullptr },
	{ nullptr, "sandbombbase_up", 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo SandBombBasePyramid_anim_info
    = { 2, 1, SandBombBasePyramid_anim_data };

static const TMapObjCollisionData SandBombBasePyramid_map_collision_data[] = {
	{ "SandBombBasePyramid", 2 },
	{ "SandBombBasePyramid", 1 },
};

static const TMapObjCollisionInfo SandBombBasePyramid_map_collision_info
    = { 2, 2, SandBombBasePyramid_map_collision_data };

static TMapObjData SandBombBasePyramid_data = {
	"SandBombBasePyramid",
	0x400000CD,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&SandBombBasePyramid_anim_info,
	&no_data_obj_hit_info,
	&SandBombBasePyramid_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000809,
	0x00000000,
};

static const TMapObjAnimData SandBombBaseShit_anim_data[] = {
	{ "SandBombBaseShit.bmd", nullptr, 0, nullptr, nullptr },
	{ nullptr, "sandbombbase_up", 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo SandBombBaseShit_anim_info
    = { 2, 1, SandBombBaseShit_anim_data };

static const TMapObjCollisionData SandBombBaseShit_map_collision_data[] = {
	{ "SandBombBaseShit", 2 },
	{ "SandBombBaseShit", 1 },
};

static const TMapObjCollisionInfo SandBombBaseShit_map_collision_info
    = { 2, 2, SandBombBaseShit_map_collision_data };

static TMapObjData SandBombBaseShit_data = {
	"SandBombBaseShit",
	0x400000CD,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&SandBombBaseShit_anim_info,
	&no_data_obj_hit_info,
	&SandBombBaseShit_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000809,
	0x00000000,
};

static const TMapObjAnimData SandBombBaseStar_anim_data[] = {
	{ "SandBombBaseStar.bmd", nullptr, 0, nullptr, nullptr },
	{ nullptr, "sandbombbase_up", 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo SandBombBaseStar_anim_info
    = { 2, 1, SandBombBaseStar_anim_data };

static const TMapObjCollisionData SandBombBaseStar_map_collision_data[] = {
	{ "SandBombBaseStar", 2 },
	{ "SandBombBaseStar", 1 },
};

static const TMapObjCollisionInfo SandBombBaseStar_map_collision_info
    = { 2, 2, SandBombBaseStar_map_collision_data };

static TMapObjData SandBombBaseStar_data = {
	"SandBombBaseStar",
	0x400000CD,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&SandBombBaseStar_anim_info,
	&no_data_obj_hit_info,
	&SandBombBaseStar_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000809,
	0x00000000,
};

static const TMapObjAnimData SandBombBaseTurtle_anim_data[] = {
	{ "SandBombBaseTurtle.bmd", nullptr, 0, nullptr, nullptr },
	{ nullptr, "sandbombbase_up", 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo SandBombBaseTurtle_anim_info
    = { 2, 1, SandBombBaseTurtle_anim_data };

static const TMapObjCollisionData SandBombBaseTurtle_map_collision_data[] = {
	{ "SandBombBaseTurtle", 2 },
	{ "SandBombBaseTurtle", 1 },
};

static const TMapObjCollisionInfo SandBombBaseTurtle_map_collision_info
    = { 2, 2, SandBombBaseTurtle_map_collision_data };

static TMapObjData SandBombBaseTurtle_data = {
	"SandBombBaseTurtle",
	0x400000CD,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&SandBombBaseTurtle_anim_info,
	&no_data_obj_hit_info,
	&SandBombBaseTurtle_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000809,
	0x00000000,
};

static const TMapObjAnimData SandBombBaseFoot_anim_data[] = {
	{ "SandBombBaseFoot.bmd", nullptr, 0, nullptr, nullptr },
	{ nullptr, "sandbombbase_up", 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo SandBombBaseFoot_anim_info
    = { 2, 1, SandBombBaseFoot_anim_data };

static const TMapObjCollisionData SandBombBaseFoot_map_collision_data[] = {
	{ "SandBombBaseFoot", 2 },
	{ "SandBombBaseFoot", 1 },
};

static const TMapObjCollisionInfo SandBombBaseFoot_map_collision_info
    = { 2, 2, SandBombBaseFoot_map_collision_data };

static TMapObjData SandBombBaseFoot_data = {
	"SandBombBaseFoot",
	0x400000CE,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&SandBombBaseFoot_anim_info,
	&no_data_obj_hit_info,
	&SandBombBaseFoot_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000809,
	0x00000000,
};

static const TMapObjAnimData SandBombBaseHand_anim_data[] = {
	{ "SandBombBaseHand.bmd", nullptr, 0, nullptr, nullptr },
	{ nullptr, "sandbombbase_up", 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo SandBombBaseHand_anim_info
    = { 2, 1, SandBombBaseHand_anim_data };

static const TMapObjCollisionData SandBombBaseHand_map_collision_data[] = {
	{ "SandBombBaseHand", 2 },
	{ "SandBombBaseHand", 1 },
};

static const TMapObjCollisionInfo SandBombBaseHand_map_collision_info
    = { 2, 2, SandBombBaseHand_map_collision_data };

static TMapObjData SandBombBaseHand_data = {
	"SandBombBaseHand",
	0x400000CE,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&SandBombBaseHand_anim_info,
	&no_data_obj_hit_info,
	&SandBombBaseHand_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000809,
	0x00000000,
};

static const TMapObjAnimData SandBombBaseStairs_anim_data[] = {
	{ "SandBombBaseStairs.bmd", nullptr, 0, nullptr, nullptr },
	{ nullptr, "sandbombbase_up", 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo SandBombBaseStairs_anim_info
    = { 2, 1, SandBombBaseStairs_anim_data };

static const TMapObjCollisionData SandBombBaseStairs_map_collision_data[] = {
	{ "SandBombBaseStairs", 2 },
	{ "SandBombBaseStairs", 1 },
};

static const TMapObjCollisionInfo SandBombBaseStairs_map_collision_info
    = { 2, 2, SandBombBaseStairs_map_collision_data };

static TMapObjData SandBombBaseStairs_data = {
	"SandBombBaseStairs",
	0x400000CE,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&SandBombBaseStairs_anim_info,
	&no_data_obj_hit_info,
	&SandBombBaseStairs_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000809,
	0x00000000,
};

static TMapObjData coral00_data = {
	"coral00",
	0x400000D9,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000D01,
	0x00000000,
};

static TMapObjData coral01_data = {
	"coral01",
	0x400000D9,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000D01,
	0x00000000,
};

static const TMapObjHitDataTable GoalWatermelon_hit_data_table[] = {
	{ 120.0f, 180.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo GoalWatermelon_obj_hit_info
    = { 0x1, 0x40000000, 0.0f, GoalWatermelon_hit_data_table };

static TMapObjData GoalWatermelon_data = {
	"GoalWatermelon",
	0x400000DA,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&no_data_anim_info,
	&GoalWatermelon_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000001,
	0x00000000,
};

static const TMapObjAnimData TeethOfJuicer_anim_data[] = {
	{ "TeethOfJuicer.bmd", "teethofjuicer", 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo TeethOfJuicer_anim_info
    = { 1, 1, TeethOfJuicer_anim_data };

static const TMapObjHitDataTable TeethOfJuicer_hit_data_table[] = {
	{ 120.0f, 180.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo TeethOfJuicer_obj_hit_info
    = { 0x1, 0x40000000, 0.0f, TeethOfJuicer_hit_data_table };

static TMapObjData TeethOfJuicer_data = {
	"TeethOfJuicer",
	0x400000DC,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&TeethOfJuicer_anim_info,
	&TeethOfJuicer_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000001,
	0x00000000,
};

static const TMapObjCollisionData SandEggRepair_map_collision_data[] = {
	{ "SandEggrepair", 0 },
};

static const TMapObjCollisionInfo SandEggRepair_map_collision_info
    = { 1, 1, SandEggRepair_map_collision_data };

static TMapObjData SandEggRepair_data = {
	"SandEggrepair",
	0x400000DD,
	"大型地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	&SandEggRepair_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000120,
	0x00000000,
};

static const TMapObjAnimData merrygoround_anim_data[] = {
	{ "merry.bmd", "merry", 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo merrygoround_anim_info
    = { 1, 1, merrygoround_anim_data };

static const TMapObjHitDataTable merry_hit_data_table[] = {
	{ 2000.0f, 2000.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo merrygoround_obj_hit_info
    = { 0x1, 0x80000000, 0.0f, merry_hit_data_table };

static const TMapObjCollisionData merrygoround_map_collision_data[] = {
	{ "merry", 0 },
};

static const TMapObjCollisionInfo merrygoround_map_collision_info
    = { 1, 1, merrygoround_map_collision_data };

static TMapObjData merrygoround_data = {
	"merry",
	0x4000012C,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&merrygoround_anim_info,
	&no_data_obj_hit_info,
	&merrygoround_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x0000090B,
	0x00000000,
};

static const TMapObjHitDataTable merry_egg_hit_data_table[] = {
	{ 300.0f, 300.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo merry_egg_obj_hit_info
    = { 0x1, 0x80000000, 0.0f, merry_egg_hit_data_table };

static const TMapObjCollisionData merry_egg_map_collision_data[] = {
	{ "merry_egg", 1 },
};

static const TMapObjCollisionInfo merry_egg_map_collision_info
    = { 1, 1, merry_egg_map_collision_data };

static TMapObjData merry_egg_data = {
	"merry_egg",
	0x40000135,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&merry_egg_obj_hit_info,
	&merry_egg_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x0000090B,
	0x00000000,
};

static const TMapObjHitDataTable merry_pole_hit_data_table[] = {
	{ 0.0f, 0.0f, 50.0f, 600.0f },
};

static const TMapObjHitInfo merry_pole_obj_hit_info
    = { 0x1, 0x00000000, 0.0f, merry_pole_hit_data_table };

static TMapObjData merry_pole_data = {
	"merry_pole",
	0x40000136,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&no_data_anim_info,
	&merry_pole_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000040,
	0x00000000,
};

static const TMapObjAnimData FerrisWheel_anim_data[] = {
	{ "FerrisWheel.bmd", "ferriswheel", 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo FerrisWheel_anim_info
    = { 1, 1, FerrisWheel_anim_data };

static const TMapObjHitDataTable FerrisWheel_hit_data_table[] = {
	{ 2800.0f, 5600.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo FerrisWheel_obj_hit_info
    = { 0x1, 0x80000000, -2800.0f, FerrisWheel_hit_data_table };

static TMapObjData FerrisWheel_data = {
	"FerrisWheel",
	0x4000012D,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&FerrisWheel_anim_info,
	&FerrisWheel_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x0000092B,
	0x00000000,
};

static const TMapObjCollisionData gondola_map_collision_data[] = {
	{ "FerrisGondola", 1 },
};

static const TMapObjCollisionInfo gondola_map_collision_info
    = { 1, 1, gondola_map_collision_data };

static const TMapObjHitDataTable FerrisWheelGondola_hit_data_table[] = {
	{ 500.0f, 800.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo FerrisWheelGondola_obj_hit_info
    = { 0x1, 0x80000000, -700.0f, FerrisWheelGondola_hit_data_table };

static TMapObjData gondola_data = {
	"FerrisGondola",
	0x4000012E,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&FerrisWheelGondola_obj_hit_info,
	&gondola_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x0000092B,
	0x00000000,
};

static const TMapObjAnimData FerrisLOD_anim_data[] = {
	{ "FerrisLOD.bmd", "ferrislod", 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo FerrisLOD_anim_info
    = { 1, 1, FerrisLOD_anim_data };

static TMapObjData FerrisLOD_data = {
	"FerrisLOD",
	0x4000012D,
	"大型地形オブジェマネージャー",
	"オブジェクトグループ",
	&FerrisLOD_anim_info,
	&no_data_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000801,
	0x00000000,
};

static const TMapObjHitDataTable Viking_hit_data_table[] = {
	{ 1300.0f, 2400.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo Viking_obj_hit_info
    = { 0x1, 0x81000000, -1000.0f, Viking_hit_data_table };

static const TMapObjCollisionData viking_map_collision_data[] = {
	{ "viking", 1 },
};

static const TMapObjCollisionInfo viking_map_collision_info
    = { 1, 1, viking_map_collision_data };

static TMapObjData viking_data = {
	"viking",
	0x4000012F,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&Viking_obj_hit_info,
	&viking_map_collision_info,
	&normal_sound_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x0000092B,
	0x00000000,
};

static const TMapObjCollisionData PinnaEntrance_map_collision_data[] = {
	{ "PinnaDoor", 2 },
};

static const TMapObjCollisionInfo PinnaEntrance_map_collision_info
    = { 1, 1, PinnaEntrance_map_collision_data };

static TMapObjData PinnaEntrance_data = {
	"PinnaDoor",
	0x40000130,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	&PinnaEntrance_map_collision_info,
	&normal_sound_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000901,
	0x00000000,
};

static const TMapObjCollisionData PinnaEntranceOpen_map_collision_data[] = {
	{ "PinnaDoor", 2 },
};

static const TMapObjCollisionInfo PinnaEntranceOpen_map_collision_info
    = { 1, 1, PinnaEntranceOpen_map_collision_data };

static TMapObjData PinnaEntranceOpen_data = {
	"PinnaDoorOpen",
	0x40000130,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	&PinnaEntranceOpen_map_collision_info,
	&normal_sound_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	80.0f,
	0x00000901,
	0x00000000,
};

static const TMapObjCollisionData SirenaGate_map_collision_data[] = {
	{ "SirenaGate", 2 },
};

static const TMapObjCollisionInfo SirenaGate_map_collision_info
    = { 1, 1, SirenaGate_map_collision_data };

static TMapObjData SirenaGate_data = {
	"SirenaGate",
	0x40000131,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	&SirenaGate_map_collision_info,
	&normal_sound_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	80.0f,
	0x00000901,
	0x00000000,
};

static const TMapObjAnimData balloon_koopajr_anim_data[] = {
	{ "balloonKoopaJr.bmd", "balloonkoopajr_wait", 0, nullptr, nullptr },
	{ nullptr, nullptr, 0, nullptr, nullptr },
	{ nullptr, nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo balloon_koopajr_anim_info
    = { 3, 1, balloon_koopajr_anim_data };

static const TMapObjHitDataTable balloon_koopajr_hit_data_table[] = {
	{ 100.0f, 100.0f, 100.0f, 100.0f },
};

static const TMapObjHitInfo balloon_koopajr_obj_hit_info
    = { 0x1, 0x10000000, 0.0f, balloon_koopajr_hit_data_table };

static TMapObjData BalloonKoopaJr_data = {
	"balloonKoopaJr",
	0x40000132,
	"アイテムマネージャー",
	"アイテムグループ",
	&balloon_koopajr_anim_info,
	&balloon_koopajr_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	80.0f,
	0x00000001,
	0x00000000,
};

static const TMapObjAnimData ShellCup_anim_data[] = {
	{ "ShellCup.bmd", "shellcup", 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo ShellCup_anim_info
    = { 1, 1, ShellCup_anim_data };

static const TMapObjCollisionData ShellCup_map_collision_data[] = {
	{ "ShellCup_under", 1 },
};

static const TMapObjCollisionInfo ShellCup_map_collision_info
    = { 1, 1, ShellCup_map_collision_data };

static const TMapObjHitDataTable ShellCup_hit_data_table[] = {
	{ 1300.0f, 500.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo ShellCup_obj_hit_info
    = { 0x1, 0x80000000, 0.0f, ShellCup_hit_data_table };

static TMapObjData ShellCup_data = {
	"ShellCup",
	0x40000133,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&ShellCup_anim_info,
	&ShellCup_obj_hit_info,
	&ShellCup_map_collision_info,
	&normal_sound_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	80.0f,
	0x0000090B,
	0x00000000,
};

static const TMapObjAnimData Gateshell_anim_data[] = {
	{ "Gateshell.bmd", "gateshell", 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo Gateshell_anim_info
    = { 1, 1, Gateshell_anim_data };

static const TMapObjCollisionData Gateshell_map_collision_data[] = {
	{ "Gateshell", 2 },
};

static const TMapObjCollisionInfo Gateshell_map_collision_info
    = { 1, 1, Gateshell_map_collision_data };

static TMapObjData Gateshell_data = {
	"Gateshell",
	0x40000134,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&Gateshell_anim_info,
	&no_data_obj_hit_info,
	&Gateshell_map_collision_info,
	&normal_sound_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	80.0f,
	0x00000005,
	0x00000000,
};

static const TMapObjAnimData PinnaHangingBridgeBoard_anim_data[] = {
	{ "PinnaBoard.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo PinnaHangingBridgeBoard_anim_info
    = { 1, 1, PinnaHangingBridgeBoard_anim_data };

static const TMapObjHitDataTable PinnaHangingBridgeBoard_hit_data_table[] = {
	{ 300.0f, 300.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo PinnaHangingBridgeBoard_obj_hit_info
    = { 0x1, 0x80000000, 0.0f, PinnaHangingBridgeBoard_hit_data_table };

static const TMapObjCollisionData PinnaHangingBridgeBoard_map_collision_data[]
    = {
	      { "PinnaBoard", 1 },
      };

static const TMapObjCollisionInfo PinnaHangingBridgeBoard_map_collision_info
    = { 1, 1, PinnaHangingBridgeBoard_map_collision_data };

static TMapObjData PinnaHangingBridgeBoard_data = {
	"PinnaHangingBridgeBoard",
	0x40000248,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&PinnaHangingBridgeBoard_anim_info,
	&PinnaHangingBridgeBoard_obj_hit_info,
	&PinnaHangingBridgeBoard_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x0000490B,
	0x00000000,
};

static const TMapObjAnimData GateManta_anim_data[] = {
	{ "GateManta.bmd", "gatemanta", 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo GateManta_anim_info
    = { 1, 1, GateManta_anim_data };

static TMapObjData GateManta_data = {
	"GateManta",
	0x40000137,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&GateManta_anim_info,
	&no_data_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	80.0f,
	0x00000001,
	0x00000000,
};

static TMapObjData PinnaSunFlower_data = {
	"PinnaSunFlower",
	0x40000138,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	80.0f,
	0x00000001,
	0x00000000,
};

static const TMapObjAnimData AmiKing_anim_data[] = {
	{ "amiking_model1.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo AmiKing_anim_info
    = { 1, 1, AmiKing_anim_data };

static const TMapObjHitDataTable AmiKing_hit_data_table[] = {
	{ 300.0f, 500.0f, 300.0f, 500.0f },
};

static const TMapObjHitInfo AmiKing_obj_hit_info
    = { 0x1, 0x80000000, 0.0f, AmiKing_hit_data_table };

static TMapObjData AmiKing_data = {
	"amiking",
	0x10000034,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&AmiKing_anim_info,
	&AmiKing_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	300.0f,
	0x00000001,
	0x00000000,
};

static const TMapObjHitDataTable PinnaCoaster_hit_data_table[] = {
	{ 0.0f, 0.0f, 300.0f, 300.0f },
};

static const TMapObjHitInfo PinnaCoaster_obj_hit_info
    = { 0x1, 0x00000000, 0.0f, PinnaCoaster_hit_data_table };

static TMapObjData PinnaCoaster_data = {
	"Coaster",
	0x40000139,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&PinnaCoaster_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	100.0f,
	0x00000900,
	0x00000000,
};

static const TMapObjCollisionData casinoroulette_map_collision_data[] = {
	{ "casinorulet", 1 },
};

static const TMapObjCollisionInfo casinoroulette_map_collision_info
    = { 1, 1, casinoroulette_map_collision_data };

static TMapObjData casinoroulette_data = {
	"casinorulet",
	0x4000019A,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	nullptr,
	&casinoroulette_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000009,
	0x00000000,
};

static const TMapObjAnimData roulette_anim_data[] = {
	{ "rulet00.bmd", "rulet00", 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo roulette_anim_info
    = { 1, 1, roulette_anim_data };

static const TMapObjCollisionData roulette00_map_collision_data[] = {
	{ "rulet00", 1 },
};

static const TMapObjCollisionInfo roulette00_map_collision_info
    = { 1, 1, roulette00_map_collision_data };

static TMapObjData roulette00_data = {
	"rulet00",
	0x4000019A,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&roulette_anim_info,
	&no_data_obj_hit_info,
	&roulette00_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000009,
	0x00000000,
};

static const TMapObjCollisionData roulette01_map_collision_data[] = {
	{ "rulet01", 0 },
};

static const TMapObjCollisionInfo roulette01_map_collision_info
    = { 1, 1, roulette01_map_collision_data };

static TMapObjData roulette01_data = {
	"rulet01",
	0x4000019A,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	&roulette01_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000005,
	0x00000000,
};

static const TMapObjCollisionData roulette02_map_collision_data[] = {
	{ "rulet02", 0 },
};

static const TMapObjCollisionInfo roulette02_map_collision_info
    = { 1, 1, roulette02_map_collision_data };

static TMapObjData roulette02_data = {
	"rulet02",
	0x4000019A,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	&roulette02_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000005,
	0x00000000,
};

static const TMapObjHitDataTable slotdrum_hit_data_table[] = {
	{ 0.0f, 0.0f, 500.0f, 300.0f },
};

static const TMapObjHitInfo slotdrum_obj_hit_info
    = { 0x1, 0x00000000, 0.0f, slotdrum_hit_data_table };

static TMapObjData slotdrum_data = {
	"srotdram",
	0x4000019B,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&slotdrum_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000805,
	0x00000000,
};

// one more unused global?
static const char* dummy10 = "srotRulet";

static const TMapObjHitDataTable telesaslot_hit_data_table[] = {
	{ 0.0f, 0.0f, 400.0f, 300.0f },
};

static const TMapObjHitInfo telesaslot_obj_hit_info
    = { 0x1, 0x00000000, 0.0f, telesaslot_hit_data_table };

static TMapObjData telesaslot_data = {
	"srotRulet",
	0x400001A6,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&telesaslot_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000805,
	0x00000000,
};

static const TMapObjAnimData donchou_anim_data[] = {
	{ "Donchou.bmd", "donchou", 0, nullptr, nullptr },
};

static const TMapObjHitDataTable donchou_hit_data_table[] = {
	{ 0.0f, 0.0f, 400.0f, 800.0f },
};

static const TMapObjHitInfo donchou_obj_hit_info
    = { 0x1, 0x00000000, 0.0f, donchou_hit_data_table };

static TMapObjData donchou_data = {
	"Donchou",
	0x400001A7,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&donchou_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000001,
	0x00000000,
};

static const TMapObjHitDataTable casino_panel_gate_hit_data_table[] = {
	{ 0.0f, 0.0f, 500.0f, 800.0f },
};

static const TMapObjHitInfo casino_panel_gate_obj_hit_info
    = { 0x1, 0x00000000, 0.0f, casino_panel_gate_hit_data_table };

static const TMapObjCollisionData pazul_map_collision_data[] = {
	{ "pazul", 2 },
};

static const TMapObjCollisionInfo pazul_map_collision_info
    = { 1, 1, pazul_map_collision_data };

static TMapObjData casino_panel_gate_data = {
	"pazul",
	0x4000019C,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&casino_panel_gate_obj_hit_info,
	&pazul_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000001,
	0x00000000,
};

static TMapObjData SakuCasino_data = {
	"SakuCasino",
	0x400001A8,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000001,
	0x00000000,
};

static const TMapObjAnimData bosswall_anim_data[] = {
	{ "boss_wall.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo bosswall_anim_info
    = { 1, 1, bosswall_anim_data };

static TMapObjData SirenabossWall_data = {
	"SirenabossWall",
	0x400001A8,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&bosswall_anim_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000001,
	0x00000000,
};

static const TMapObjAnimData casinoroof_anim_data[] = {
	{ "casino_lighting.bmd", "casino_lighting", 5, nullptr, nullptr },
};

static const TMapObjAnimDataInfo casinoroof_anim_info
    = { 1, 1, casinoroof_anim_data };

static TMapObjData SirenaCasinoRoof_data = {
	"SirenaCasinoRoof",
	0x400001A8,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&casinoroof_anim_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000001,
	0x00000000,
};

static const TMapObjAnimData GlassBreak_anim_data[] = {
	{ "GlassBreak.bmd", nullptr, 0, nullptr, nullptr },
	{ nullptr, nullptr, 0, nullptr, nullptr },
	{ nullptr, "GlassBreak", 0, nullptr, nullptr },
	{ nullptr, nullptr, 0, nullptr, nullptr },
	{ nullptr, nullptr, 0, nullptr, nullptr },
	{ nullptr, nullptr, 0, nullptr, nullptr },
	{ nullptr, nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo GlassBreak_anim_info
    = { 7, 1, GlassBreak_anim_data };

static const TMapObjCollisionData GlassBreak_map_collision_data[] = {
	{ "GlassBreak", 2 },
};

static const TMapObjCollisionInfo GlassBreak_map_collision_info
    = { 1, 1, GlassBreak_map_collision_data };

static const TMapObjSoundData GlassBreak_sound_data
    = { 0xFFFFFFFF, 0xFFFFFFFF, 0x00003857, 0xFFFFFFFF, 0xFFFFFFFF,
	    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF };

static const TMapObjSoundInfo GlassBreak_sound_info
    = { 0xA, &GlassBreak_sound_data };

static TMapObjData GlassBreak_data = {
	"GlassBreak",
	0x4000019E,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&GlassBreak_anim_info,
	&no_data_obj_hit_info,
	&GlassBreak_map_collision_info,
	&GlassBreak_sound_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00200001,
	0x00000000,
};

static const TMapObjAnimData closet_anim_data[] = {
	{ "closet.bmd", "ClosetOpen", 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo closet_anim_info = { 1, 1, closet_anim_data };

static const TMapObjHitDataTable closet_hit_data_table[] = {
	{ 0.0f, 0.0f, 400.0f, 450.0f },
};

static const TMapObjHitInfo closet_obj_hit_info
    = { 0x1, 0x00000000, 0.0f, closet_hit_data_table };

static TMapObjData Closet_data = {
	"Closet",
	0x4000019F,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&closet_anim_info,
	&closet_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000001,
	0x00000000,
};

static const TMapObjAnimData ChestRevolve_anim_data[] = {
	{ "ChestRevolve.bmd", nullptr, 0, nullptr, nullptr },
	{ nullptr, "chestrevolve", 0, nullptr, "/scene/mapObj/ChestRevolve.bas" },
};

static const TMapObjAnimDataInfo ChestRevolve_anim_info
    = { 2, 1, ChestRevolve_anim_data };

static const TMapObjCollisionData ChestRevolve_map_collision_data[] = {
	{ "ChestRevolve", 2 },
	{ "ChestRevolve", 1 },
};

static const TMapObjCollisionInfo ChestRevolve_map_collision_info
    = { 2, 2, ChestRevolve_map_collision_data };

static TMapObjData ChestRevolve_data = {
	"ChestRevolve",
	0x400001A0,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&ChestRevolve_anim_info,
	&no_data_obj_hit_info,
	&ChestRevolve_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000009,
	0x00000000,
};

static const TMapObjAnimData PosterTeresa_anim_data[] = {
	{ "PosterTeresa.bmd", "posterteresa", 5, nullptr, nullptr },
};

static const TMapObjCollisionData PosterTeresa_map_collision_data[] = {
	{ "PosterTeresa", 2 },
};

static const TMapObjCollisionInfo PosterTeresa_map_collision_info
    = { 1, 1, PosterTeresa_map_collision_data };

static TMapObjData PosterTeresa_data = {
	"PosterTeresa",
	0x400001A1,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	&PosterTeresa_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000001,
	0x00000000,
};

static const TMapObjAnimData PictureTeresa_anim_data[] = {
	{ "PictureTeresa.bmd", "pictureteresa", 5, nullptr, nullptr },
};

static const TMapObjHitDataTable PictureTeresa_hit_data_table[] = {
	{ 200.0f, 400.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo PictureTeresa_obj_hit_info
    = { 0x1, 0x80000000, 0.0f, PictureTeresa_hit_data_table };

static const TMapObjCollisionData PictureTeresa_map_collision_data[] = {
	{ "PictureTeresa", 2 },
};

static const TMapObjCollisionInfo PictureTeresa_map_collision_info
    = { 1, 1, PictureTeresa_map_collision_data };

static TMapObjData PictureTeresa_data = {
	"PictureTeresa",
	0x400001A2,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&PictureTeresa_obj_hit_info,
	&PictureTeresa_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000001,
	0x00000000,
};

static const TMapObjAnimData PanelBreak_anim_data[] = {
	{ "PanelBreak.bmd", nullptr, 0, nullptr, nullptr },
	{ nullptr, nullptr, 0, nullptr, nullptr },
	{ nullptr, "PanelBreak", 0, nullptr, nullptr },
	{ nullptr, nullptr, 0, nullptr, nullptr },
	{ nullptr, nullptr, 0, nullptr, nullptr },
	{ nullptr, nullptr, 0, nullptr, nullptr },
	{ nullptr, nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo PanelBreak_anim_info
    = { 7, 1, PanelBreak_anim_data };

static const TMapObjCollisionData PanelBreak_map_collision_data[] = {
	{ "PanelBreak", 2 },
};

static const TMapObjCollisionInfo PanelBreak_map_collision_info
    = { 1, 1, PanelBreak_map_collision_data };

static const TMapObjSoundData PanelBreak_sound_data
    = { 0xFFFFFFFF, 0xFFFFFFFF, 0x00003878, 0xFFFFFFFF, 0xFFFFFFFF,
	    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF };

static const TMapObjSoundInfo PanelBreak_sound_info
    = { 0xA, &PanelBreak_sound_data };

static TMapObjData PanelBreak_data = {
	"PanelBreak",
	0x400001A3,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&PanelBreak_anim_info,
	&no_data_obj_hit_info,
	&PanelBreak_map_collision_info,
	&PanelBreak_sound_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00200001,
	0x00000000,
};

static const TMapObjAnimData PanelRevolve_anim_data[] = {
	{ "PanelRevolve.bmd", nullptr, 0, nullptr, nullptr },
	{ nullptr, "panelrevolve", 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo PanelRevolve_anim_info
    = { 2, 1, PanelRevolve_anim_data };

static const TMapObjCollisionData PanelRevolve_map_collision_data[] = {
	{ "PanelRevolve", 2 },
};

static const TMapObjCollisionInfo PanelRevolve_map_collision_info
    = { 1, 1, PanelRevolve_map_collision_data };

static TMapObjData PanelRevolve_data = {
	"PanelRevolve",
	0x400001A4,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&PanelRevolve_anim_info,
	&no_data_obj_hit_info,
	&PanelRevolve_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000001,
	0x00000000,
};

static TMapObjData SirenaBlockBreak_data = {
	"SirenaBlockBreak",
	0x400001A5,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000001,
	0x00000000,
};

static const TMapObjCollisionData SirenaShop_map_collision_data[] = {
	{ "SirenaShop", 0 },
};

static const TMapObjCollisionInfo SirenaShop_map_collision_info
    = { 1, 1, SirenaShop_map_collision_data };

static TMapObjData SirenaShop_data = {
	"SirenaShop",
	0x400001A9,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	&SirenaShop_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000101,
	0x00000000,
};

static TMapObjData StainHotel_data = {
	"SimiHotel",
	0x400001AA,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000101,
	0x00000000,
};

static const TMapObjAnimData cogwheel_anim_data[] = {
	{ "cogwheel_wheel.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo cogwheel_anim_info
    = { 1, 1, cogwheel_anim_data };

static const TMapObjHitDataTable Cogwheel_hit_data_table[] = {
	{ 1600.0f, 860.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo Cogwheel_obj_hit_info
    = { 0x1, 0x80000000, -800.0f, Cogwheel_hit_data_table };

static const TMapObjCollisionData cogwheel_map_collision_data[] = {
	{ "cogwheel_wheel", 2 },
};

static const TMapObjCollisionInfo cogwheel_map_collision_info
    = { 1, 1, cogwheel_map_collision_data };

static TMapObjData cogwheel_data = {
	"cogwheel",
	0x40000226,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&cogwheel_anim_info,
	&no_data_obj_hit_info,
	&cogwheel_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000909,
	0x00000000,
};

static const TMapObjHitDataTable CogwheelPlate_hit_data_table[] = {
	{ 200.0f, 200.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo CogwheelPlate_obj_hit_info
    = { 0x1, 0x80000000, -50.0f, CogwheelPlate_hit_data_table };

static const TMapObjCollisionData CogwheelPlate_map_collision_data[] = {
	{ "cogwheel_plate", 1 },
};

static const TMapObjCollisionInfo CogwheelPlate_map_collision_info
    = { 1, 1, CogwheelPlate_map_collision_data };

static TMapObjData CogwheelPlate_data = {
	"cogwheel_plate",
	0x40000227,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&CogwheelPlate_obj_hit_info,
	&CogwheelPlate_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000803,
	0x00000000,
};

static const TMapObjHitDataTable CogwheelPot_hit_data_table[] = {
	{ 300.0f, 300.0f, 250.0f, 500.0f },
};

static const TMapObjHitInfo CogwheelPot_obj_hit_info
    = { 0x1, 0x80000000, 0.0f, CogwheelPot_hit_data_table };

static const TMapObjCollisionData CogwheelPot_map_collision_data[] = {
	{ "cogwheel_pot", 1 },
};

static const TMapObjCollisionInfo CogwheelPot_map_collision_info
    = { 1, 1, CogwheelPot_map_collision_data };

static TMapObjData CogwheelPot_data = {
	"cogwheel_pot",
	0x40000228,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&CogwheelPot_obj_hit_info,
	&CogwheelPot_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000807,
	0x00000000,
};

static const TMapObjHitDataTable mare_float_house_hit_data_table[] = {
	{ 500.0f, 1200.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo mare_float_house_obj_hit_info
    = { 0x1, 0x84000000, -800.0f, mare_float_house_hit_data_table };

static const TMapObjCollisionData mare_float_house_map_collision_data[] = {
	{ "ukisima", 1 },
};

static const TMapObjCollisionInfo mare_float_house_map_collision_info
    = { 1, 1, mare_float_house_map_collision_data };

static TMapObjData mare_float_house_data = {
	"ukisima",
	0x40000229,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&mare_float_house_obj_hit_info,
	&mare_float_house_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x0000090B,
	0x00000000,
};

static TMapObjPhysicalData StopRock_physical_data
    = { 0.35f, 0.0f, 0.0f, 0.95f, 0.95f, 1.0f, 0.0f,
	    0.0f,  0.0f, 0.0f, 0.5f,  0.0f,  0.0f };

static TMapObjPhysicalInfo StopRock_physical_info
    = { 0xD, &StopRock_physical_data, 0x0 };

static const TMapObjHitDataTable StopRock_hit_data_table[] = {
	{ 250.0f, 350.0f, 250.0f, 350.0f },
};

static const TMapObjHitInfo StopRock_obj_hit_info
    = { 0x1, 0x00000000, 0.0f, StopRock_hit_data_table };

static const TMapObjCollisionData StopRock_map_collision_data[] = {
	{ "StopRock", 2 },
};

static const TMapObjCollisionInfo StopRock_map_collision_info
    = { 1, 1, StopRock_map_collision_data };

static TMapObjData StopRock_data = {
	"StopRock",
	0x4000022B,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&StopRock_obj_hit_info,
	&StopRock_map_collision_info,
	nullptr,
	&StopRock_physical_info,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00100005,
	0x00000000,
};

static const TMapObjHitDataTable ElasticCode_hit_data_table[] = {
	{ 200.0f, 200.0f, 200.0f, 200.0f },
};

static const TMapObjHitInfo ElasticCode_obj_hit_info
    = { 0x1, 0x80000000, 0.0f, ElasticCode_hit_data_table };

static TMapObjData ElasticCode_data = {
	"ElasticCode",
	0x400000BB,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&ElasticCode_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000841,
	0x00000000,
};

static const TMapObjAnimData GrowthTree_anim_data[] = {
	{ "Moyasi.bmd", "moyasi_wait", 0, nullptr, nullptr },
	{ nullptr, "moyasi_grow10", 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo GrowthTree_anim_info
    = { 2, 2, GrowthTree_anim_data };

static const TMapObjHitDataTable GrowthTree_hit_data_table[] = {
	{ 50.0f, 300.0f, 50.0f, 300.0f },
};

static const TMapObjHitInfo GrowthTree_obj_hit_info
    = { 0x1, 0x00000000, 0.0f, GrowthTree_hit_data_table };

static const TMapObjCollisionData GrowthTree_map_collision_data[] = {
	{ "Moyasi", 2 },
};

static const TMapObjCollisionInfo GrowthTree_map_collision_info
    = { 1, 1, GrowthTree_map_collision_data };

static TMapObjData GrowthTree_data = {
	"Moyasi",
	0x4000003C,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&GrowthTree_anim_info,
	&GrowthTree_obj_hit_info,
	&GrowthTree_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000841,
	0x00000000,
};

static TMapObjData WireBellPurple_data = {
	"WireBellPurple",
	0x4000022D,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000901,
	0x00000000,
};

static const TMapObjAnimData MuddyBoat_anim_data[] = {
	{ "MuddyBoat.bmd", nullptr, 0, nullptr, nullptr },
	{ nullptr, "muddyboat", 0, nullptr, "/scene/mapObj/muddyboat.bas" },
	{ nullptr, "muddyboat", 4, nullptr, nullptr },
};

static const TMapObjAnimDataInfo MuddyBoat_anim_info
    = { 3, 3, MuddyBoat_anim_data };

static const TMapObjHitDataTable MuddyBoat_hit_data_table[] = {
	{ 300.0f, 500.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo MuddyBoat_obj_hit_info
    = { 0x1, 0x80000000, -150.0f, MuddyBoat_hit_data_table };

static const TMapObjCollisionData MuddyBoat_map_collision_data[] = {
	{ "MuddyBoat", 1 },
};

static const TMapObjCollisionInfo MuddyBoat_map_collision_info
    = { 1, 1, MuddyBoat_map_collision_data };

static TMapObjData MuddyBoat_data = {
	"MuddyBoat",
	0x4000022E,
	"乗り物マネージャー",
	"オブジェクトグループ",
	&MuddyBoat_anim_info,
	&MuddyBoat_obj_hit_info,
	&MuddyBoat_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x0000010B,
	0x00000000,
};

static const TMapObjAnimData Puncher_anim_data[] = {
	{ "puncher_model1.bmd", nullptr, 0, nullptr, nullptr },
	{ nullptr, "puncher_appear1", 0, nullptr,
	  "/scene/mapObj/puncher_appear1.bas" },
};

static const TMapObjAnimDataInfo Puncher_anim_info
    = { 2, 2, Puncher_anim_data };

static const TMapObjHitDataTable Puncher_hit_data_table[] = {
	{ 100.0f, 200.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo Puncher_obj_hit_info
    = { 0x1, 0x80000000, 0.0f, Puncher_hit_data_table };

static TMapObjData Puncher_data = {
	"Puncher",
	0x4000022F,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&Puncher_anim_info,
	&Puncher_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000101,
	0x00000000,
};

static const TMapObjAnimData HatoPop_anim_data[] = {
	{ "hatopop_model1.bmd", nullptr, 0, nullptr, nullptr },
	{ nullptr, "hatopop_appear1", 0, nullptr,
	  "/scene/mapObj/hatopop_appear1.bas" },
};

static const TMapObjAnimDataInfo HatoPop_anim_info
    = { 2, 2, HatoPop_anim_data };

static const TMapObjHitDataTable HatoPop_hit_data_table[] = {
	{ 100.0f, 200.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo HatoPop_obj_hit_info
    = { 0x1, 0x80000000, 0.0f, HatoPop_hit_data_table };

static TMapObjData HatoPop_data = {
	"HatoPop",
	0x4000022F,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&HatoPop_anim_info,
	&HatoPop_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000101,
	0x00000000,
};

static const TMapObjHitDataTable MareWaterJump_hit_data_table[] = {
	{ 500.0f, 1200.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo MareWaterJump_obj_hit_info
    = { 0x1, 0x80000000, -800.0f, MareWaterJump_hit_data_table };

static const TMapObjCollisionData MareWaterJump_map_collision_data[] = {
	{ "mareWaterJump_tool", 1 },
};

static const TMapObjCollisionInfo MareWaterJump_map_collision_info
    = { 1, 1, MareWaterJump_map_collision_data };

static TMapObjData MareWaterJump_data = {
	"mareWaterJump",
	0x40000230,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&MareWaterJump_obj_hit_info,
	&MareWaterJump_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000B0A,
	0x00000000,
};

static const TMapObjHitDataTable BumpyWall_hit_data_table[] = {
	{ 0.0f, 0.0f, 100.0f, 200.0f },
};

static const TMapObjHitInfo BumpyWall_obj_hit_info
    = { 0x1, 0x00000000, 0.0f, BumpyWall_hit_data_table };

static TMapObjData BumpyWall_data = {
	"MareEventBumpyWall",
	0x40000231,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&no_data_anim_info,
	&BumpyWall_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000000,
	0x00000000,
};

static const TMapObjHitDataTable ExBottle_hit_data_table[] = {
	{ 0.0f, 0.0f, 100.0f, 100.0f },
};

static const TMapObjHitInfo ExBottle_obj_hit_info
    = { 0x1, 0x00000000, 0.0f, ExBottle_hit_data_table };

static TMapObjData ExBottle_data = {
	"exBottle",
	0x40000233,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&ExBottle_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000000,
	0x00000000,
};

static const TMapObjAnimData CoinFish_anim_data[] = {
	{ "CoinFish.bmd", "coinfish", 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo CoinFish_anim_info
    = { 1, 1, CoinFish_anim_data };

static TMapObjData CoinFish_data = {
	"CoinFish",
	0x40000232,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&CoinFish_anim_info,
	&no_data_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000101,
	0x00000000,
};

static const TMapObjAnimData MareFall_anim_data[] = {
	{ "MareFall.bmd", "marefall", 4, nullptr, nullptr },
};

static const TMapObjAnimDataInfo MareFall_anim_info
    = { 1, 1, MareFall_anim_data };

static TMapObjData MareFall_data = {
	"mareFall",
	0x40000234,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&MareFall_anim_info,
	&no_data_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000001,
	0x00000000,
};

static const TMapObjCollisionData MareCork_map_collision_data[] = {
	{ "MareCork", 2 },
};

static const TMapObjCollisionInfo MareCork_map_collision_info
    = { 1, 1, MareCork_map_collision_data };

static TMapObjData MareCork_data = {
	"MareCork",
	0x40000235,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	&MareCork_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000001,
	0x00000000,
};

static const TMapObjAnimData MonteRoot_anim_data[] = {
	{ "nekko.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo MonteRoot_anim_info
    = { 1, 1, MonteRoot_anim_data };

static const TMapObjHitDataTable MonteRoot_hit_data_table[] = {
	{ 0.0f, 0.0f, 50.0f, 1200.0f },
};

static const TMapObjHitInfo MonteRoot_obj_hit_info
    = { 0x1, 0x80000000, 0.0f, MonteRoot_hit_data_table };

static TMapObjData MonteRoot_data = {
	"MonteRoot",
	0x40000244,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&MonteRoot_anim_info,
	&MonteRoot_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000947,
	0x00000000,
};

static const TMapObjAnimData JumpMushroom_anim_data[] = {
	{ "JumpKinoko.bmd", nullptr, 0, nullptr, nullptr },
	{ nullptr, "jumpkinoko_bound", 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo JumpMushroom_anim_info
    = { 2, 2, JumpMushroom_anim_data };

static const TMapObjCollisionData JumpMushroom_map_collision_data[] = {
	{ "JumpKinoko", 0 },
};

static const TMapObjCollisionInfo JumpMushroom_map_collision_info
    = { 1, 1, JumpMushroom_map_collision_data };

static TMapObjData JumpMushroom_data = {
	"JumpMushroom",
	0x40000245,
	"大型地形オブジェマネージャー",
	"オブジェクトグループ",
	&JumpMushroom_anim_info,
	&no_data_obj_hit_info,
	&JumpMushroom_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000801,
	0x00000000,
};

static const TMapObjAnimData MonteGoalFlag_anim_data[] = {
	{ "monteflag.bmd", "monteflag_wait", 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo MonteGoalFlag_anim_info
    = { 1, 1, MonteGoalFlag_anim_data };

static const TMapObjHitDataTable MonteGoalFlag_hit_data_table[] = {
	{ 50.0f, 860.0f, 20.0f, 860.0f },
};

static const TMapObjHitInfo MonteGoalFlag_obj_hit_info
    = { 0x1, 0x88000000, 0.0f, MonteGoalFlag_hit_data_table };

static TMapObjData MonteGoalFlag_data = {
	"MonteGoalFlag",
	0x40000246,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&MonteGoalFlag_anim_info,
	&MonteGoalFlag_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000041,
	0x00000000,
};

static const TMapObjAnimData HangingBridgeBoard_anim_data[] = {
	{ "mon_bri.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo HangingBridgeBoard_anim_info
    = { 1, 1, HangingBridgeBoard_anim_data };

static const TMapObjHitDataTable HangingBridgeBoard_hit_data_table[] = {
	{ 300.0f, 300.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo HangingBridgeBoard_obj_hit_info
    = { 0x1, 0x80000000, 0.0f, HangingBridgeBoard_hit_data_table };

static const TMapObjCollisionData HangingBridgeBoard_map_collision_data[] = {
	{ "mon_bri", 1 },
};

static const TMapObjCollisionInfo HangingBridgeBoard_map_collision_info
    = { 1, 1, HangingBridgeBoard_map_collision_data };

static TMapObjData HangingBridgeBoard_data = {
	"HangingBridgeBoard",
	0x40000248,
	"地形オブジェマネージャー",
	"敵グループ",
	&HangingBridgeBoard_anim_info,
	&HangingBridgeBoard_obj_hit_info,
	&HangingBridgeBoard_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x0000090B,
	0x00000000,
};

static TMapObjData HangingBridge_data = {
	"HangingBridge",
	0x40000247,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&no_data_anim_info,
	&no_data_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000000,
	0x00000000,
};

static const TMapObjHitDataTable SwingBoard_hit_data_table[] = {
	{ 300.0f, 300.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo SwingBoard_obj_hit_info
    = { 0x1, 0x80000000, 0.0f, SwingBoard_hit_data_table };

static const TMapObjCollisionData SwingBoard_map_collision_data[] = {
	{ "SwingBoard", 1 },
};

static const TMapObjCollisionInfo SwingBoard_map_collision_info
    = { 1, 1, SwingBoard_map_collision_data };

static TMapObjData SwingBoard_data = {
	"SwingBoard",
	0x40000249,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&SwingBoard_obj_hit_info,
	&SwingBoard_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00004909,
	0x00000000,
};

static const TMapObjAnimData Fluff_anim_data[] = {
	{ "Watage.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo Fluff_anim_info = { 1, 1, Fluff_anim_data };

static const TMapObjHitDataTable Fluff_hit_data_table[] = {
	{ 200.0f, 500.0f, 400.0f, 800.0f },
};

static const TMapObjHitInfo Fluff_obj_hit_info
    = { 0x1, 0x80000000, -100.0f, Fluff_hit_data_table };

static TMapObjData Fluff_data = {
	"Fluff",
	0x40000049,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&Fluff_anim_info,
	&Fluff_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	40.0f,
	0x00000041,
	0x00000000,
};

static TMapObjData FluffManager_data = {
	"FluffManager",
	0x40000049,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&no_data_anim_info,
	&no_data_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000000,
	0x00000000,
};

static const TMapObjAnimData Bathtub_anim_data[] = {
	{ "bath.bmd", nullptr, 0, nullptr, nullptr },
	{ "bath.bmd", "bath_overturn", 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo Bathtub_anim_info
    = { 2, 1, Bathtub_anim_data };

static TMapObjData Bathtub_data = {
	"bath",
	0x40000258,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&Bathtub_anim_info,
	&no_data_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000001,
	0x00000000,
};

static TMapObjData BathtubStand_data = {
	"stand",
	0x40000259,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000001,
	0x00000000,
};

static TMapObjMoveData BathtubStandBreaking_animation = {
	"/scene/mapObj/stand_break.bck",
};

static const TMapObjAnimData BathtubStandBreaking_anim_data[] = {
	{ "stand_break.bmd", "stand_break0", 0, nullptr, nullptr },
	{ "stand_break.bmd", "stand_break1", 0, nullptr, nullptr },
	{ "stand_break.bmd", "stand_break2", 0, nullptr, nullptr },
	{ "stand_break.bmd", "stand_break3", 0, nullptr, nullptr },
	{ "stand_break.bmd", "stand_break4", 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo BathtubStandBreaking_anim_info
    = { 5, 1, BathtubStandBreaking_anim_data };

static TMapObjData BathtubStandBreaking_data = {
	"stand_break",
	0x4000025A,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&BathtubStandBreaking_anim_info,
	&no_data_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000001,
	0x00000000,
};

static const TMapObjAnimData ex1_turn_lift_anim_data[] = {
	{ "TurnLift.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo ex1_turn_lift_anim_info
    = { 1, 1, ex1_turn_lift_anim_data };

static TMapObjMoveData ex1_turn_lift_move_data = {
	"/scene/mapObj/TurnLift.bck",
};

static const TMapObjCollisionData ex1_turn_lift_map_collision_data[] = {
	{ "TurnLift", 1 },
};

static const TMapObjCollisionInfo ex1_turn_lift_map_collision_info
    = { 1, 1, ex1_turn_lift_map_collision_data };

static TMapObjData ex1_turn_lift_data = {
	"ex1_turn_lift",
	0x40000262,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&ex1_turn_lift_anim_info,
	&no_data_obj_hit_info,
	&ex1_turn_lift_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	&ex1_turn_lift_move_data,
	0.0f,
	0x00000801,
	0x00000000,
};

static TMapObjMoveData z_turn_disk_move_data = {
	"/scene/mapObj/zTurnDisk.bck",
};

static const TMapObjCollisionData z_turn_disk_map_collision_data[] = {
	{ "zTurnDisk", 1 },
};

static const TMapObjCollisionInfo z_turn_disk_map_collision_info
    = { 1, 1, z_turn_disk_map_collision_data };

static TMapObjData z_turn_disk_data = {
	"zTurnDisk",
	0x40000262,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	&z_turn_disk_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	&z_turn_disk_move_data,
	0.0f,
	0x00000805,
	0x00000000,
};

static const TMapObjAnimData y_turn_lift_anim_data[] = {
	{ "yTurnLift.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo y_turn_lift_anim_info
    = { 1, 1, y_turn_lift_anim_data };

static TMapObjMoveData y_turn_lift_move_data = {
	"/scene/mapObj/yTurnLift.bck",
};

static const TMapObjCollisionData y_turn_lift_map_collision_data[] = {
	{ "yTurnLift", 1 },
};

static const TMapObjCollisionInfo y_turn_lift_map_collision_info
    = { 1, 1, y_turn_lift_map_collision_data };

static TMapObjData y_turn_lift_data = {
	"yTurnLift",
	0x40000262,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&y_turn_lift_anim_info,
	&no_data_obj_hit_info,
	&y_turn_lift_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	&y_turn_lift_move_data,
	0.0f,
	0x00000805,
	0x00000000,
};

static TMapObjData EXskyTumiki_data = {
	"EXskyTumiki",
	0x40000263,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000901,
	0x00000000,
};

static TMapObjData EXskyWindow_data = {
	"EXskyWindow",
	0x40000263,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000901,
	0x00000000,
};

static TMapObjData EXskySenro_data = {
	"EXskySenro",
	0x40000263,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	nullptr,
	&no_data_obj_hit_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000901,
	0x00000000,
};

static const TMapObjAnimData Nail_anim_data[] = {
	{ "kugi.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo Nail_anim_info = { 1, 1, Nail_anim_data };

static const TMapObjHitDataTable Nail_hit_data_table[] = {
	{ 50.0f, 200.0f, 40.0f, 140.0f },
};

static const TMapObjHitInfo Nail_obj_hit_info
    = { 0x1, 0x80000000, 0.0f, Nail_hit_data_table };

static const TMapObjCollisionData Nail_map_collision_data[] = {
	{ "kugi", 2 },
};

static const TMapObjCollisionInfo Nail_map_collision_info
    = { 1, 1, Nail_map_collision_data };

static TMapObjData Nail_data = {
	"MapObjNail",
	0x40000264,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&Nail_anim_info,
	&Nail_obj_hit_info,
	&Nail_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000801,
	0x00000000,
};

static const TMapObjAnimData PachinkoNail_anim_data[] = {
	{ "PachinkoKugi.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo PachinkoNail_anim_info
    = { 1, 1, PachinkoNail_anim_data };

static const TMapObjCollisionData PachinkoNail_map_collision_data[] = {
	{ "PachinkoKugi", 2 },
};

static const TMapObjCollisionInfo PachinkoNail_map_collision_info
    = { 1, 1, PachinkoNail_map_collision_data };

static TMapObjData PachinkoNail_data = {
	"MapObjPachinkoNail",
	0x40000265,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&PachinkoNail_anim_info,
	&no_data_obj_hit_info,
	&PachinkoNail_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	0.0f,
	0x00000801,
	0x00000000,
};

static const TMapObjCollisionData FileLoadBlock_map_collision_data[] = {
	{ "FileLoadBlock", 0 },
};

static const TMapObjCollisionInfo FileLoadBlock_map_collision_info
    = { 1, 1, FileLoadBlock_map_collision_data };

static const TMapObjAnimData FileLoadBlockA_anim_data[] = {
	{ "FileLoadBlockA.bmd", nullptr, 0, nullptr, nullptr },
	{ "FileLoadBlockRockA.bmd", nullptr, 0, nullptr, nullptr },
	{ "FileLoadBlockNoCard.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo FileLoadBlockA_anim_info
    = { 3, 3, FileLoadBlockA_anim_data };

static const TMapObjHitDataTable FileLoadBlock_hit_data_table[] = {
	{ 70.0f, 100.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo FileLoadBlock_obj_hit_info
    = { 0x1, 0x80000000, 0.0f, FileLoadBlock_hit_data_table };

static TMapObjData FileLoadBlockA_data = {
	"FileLoadBlockA",
	0x400002EE,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&FileLoadBlockA_anim_info,
	&no_data_obj_hit_info,
	&FileLoadBlock_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	85.0f,
	0x00006000,
	0x00000000,
};

static const TMapObjAnimData FileLoadBlockB_anim_data[] = {
	{ "FileLoadBlockB.bmd", nullptr, 0, nullptr, nullptr },
	{ "FileLoadBlockRockB.bmd", nullptr, 0, nullptr, nullptr },
	{ "FileLoadBlockNoCard.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo FileLoadBlockB_anim_info
    = { 3, 3, FileLoadBlockB_anim_data };

static TMapObjData FileLoadBlockB_data = {
	"FileLoadBlockB",
	0x400002EF,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&FileLoadBlockB_anim_info,
	&no_data_obj_hit_info,
	&FileLoadBlock_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	60.0f,
	0x00006000,
	0x00000000,
};

static const TMapObjAnimData FileLoadBlockC_anim_data[] = {
	{ "FileLoadBlockC.bmd", nullptr, 0, nullptr, nullptr },
	{ "FileLoadBlockRockC.bmd", nullptr, 0, nullptr, nullptr },
	{ "FileLoadBlockNoCard.bmd", nullptr, 0, nullptr, nullptr },
};

static const TMapObjAnimDataInfo FileLoadBlockC_anim_info
    = { 3, 3, FileLoadBlockC_anim_data };

static TMapObjData FileLoadBlockC_data = {
	"FileLoadBlockC",
	0x400002F0,
	"地形オブジェマネージャー",
	"オブジェクトグループ",
	&FileLoadBlockC_anim_info,
	&no_data_obj_hit_info,
	&FileLoadBlock_map_collision_info,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	100.0f,
	0x00006000,
	0x00000000,
};

static TMapObjData* sObjDataTable[] = {
	&bottle_large_data,
	&bottle_short_data,
	&WaterRecoverObj_data,
	&watergun_item_data,
	&nozzle_normal_item_data,
	&nozzle_rocket_item_data,
	&nozzle_back_item_data,
	&yoshi_whistle_item_data,
	&NozzleBox_data,
	&coin_data,
	&invisible_coin_data,
	&coin_red_data,
	&coin_blue_data,
	&shine_data,
	&mario_cap_data,
	&mushroom1up_data,
	&mushroom1upR_data,
	&mushroom1upX_data,
	&jumpbase_data,
	&eggYoshi_data,
	&eggYoshiEvent_data,
	&coconut_data,
	&papaya_data,
	&pine_data,
	&durian_data,
	&banana_data,
	&RedPepper_data,
	&CoverPine_data,
	&wood_barrel_data,
	&wood_barrel_once_data,
	&barrel_float_data,
	&barrel_oil_data,
	&drum_can_data,
	&football_data,
	&football_goal_data,
	&baloonball_data,
	&coconutball_data,
	&watermelon_data,
	&WatermelonStatic_data,
	&BananaTree_data,
	&FruitTree_data,
	&GrowthTree_data,
	&palmNormal_data,
	&palmOugi_data,
	&palmSago_data,
	&palmNatume_data,
	&palmLeaf_data,
	&flower_data,
	&flowerOrange_data,
	&flowerPink_data,
	&flowerPurple_data,
	&flowerRed_data,
	&flowerYellow_data,
	&flowerSunflower_data,
	&Fluff_data,
	&FluffManager_data,
	&PoleNormal_data,
	&streetlamp_data,
	&MonteChair_data,
	&door_data,
	&doorHotel_data,
	&manhole_data,
	&cloud_data,
	&ChipShine_data,
	&StarSign_data,
	&KoopaJrSignM_data,
	&SignCircle_data,
	&SignCross_data,
	&SignTriangle_data,
	&lamptrapspike_data,
	&lamptrapiron_data,
	&ArrowBoardLR_data,
	&ArrowBoardUp_data,
	&ArrowBoardDown_data,
	&WoodBox_data,
	&MapSmoke_data,
	&GeneralHitObj_data,
	&billboard_dolphin_data,
	&billboard_sun_data,
	&billboard_restaurant_data,
	&billboard_fish_data,
	&fence_normal_data,
	&fence3x3_data,
	&fence_revolve_data,
	&fence_revolve_inner_data,
	&fenceInnerGreen_data,
	&FenceWaterH_data,
	&FenceWaterV_data,
	&RailFence_data,
	&bambooRailFence_data,
	&bambooFenceRevolveInner_data,
	&bambooFenceRevolveOuter_data,
	&HideObj_data,
	&WaterHitHideObj_data,
	&FruitHitHideObj_data,
	&HipDropHideObj_data,
	&FruitBasket_data,
	&BasketReverse_data,
	&CoconutJuice_data,
	&Pile_data,
	&ObjSwitch_data,
	&RedCoinSwitch_data,
	&bucket_data,
	&ChangeStage_data,
	&ChangeStageMerrygoround_data,
	&StartDemo_data,
	&WaterSprayCylinder_data,
	&WaterSprayBox_data,
	&airplane_data,
	&AirportPole_data,
	&ice_car_data,
	&AirportBuoy_data,
	&dptCannon_data,
	&dptKing_data,
	&KoopaJrSubmarine_data,
	&monumentshine_data,
	&belldolpic_data,
	&dptWeathercock_data,
	&dptMonteFence_data,
	&dptCoronaFence_data,
	&MareGate_data,
	&DemoCannon_data,
	&NozzleDoor_data,
	&DokanGate_data,
	&crane_data,
	&crane180Scene1_data,
	&crane90Scene2_data,
	&craneUpDown_data,
	&craneCargoUpDown_data,
	&tank_data,
	&container_data,
	&submarine_data,
	&riccoShip_data,
	&riccoShipLog_data,
	&riccoShipDol_data,
	&riccoYachtS_data,
	&riccoYachtL_data,
	&riccoBoatS_data,
	&riccoBoatL_data,
	&riccoShipFish_data,
	&riccoShipGeso_data,
	&riccoPole_data,
	&riccoLog_data,
	&gesoSurfboard_data,
	&gesoSurfboardStatic_data,
	&riccoWatermill_data,
	&riccoBasket_data,
	&riccoGangway_data,
	&riccoSwitch_data,
	&riccoSwitchShine_data,
	&riccoFenceMaze_data,
	&riccoStand_data,
	&riccoGrille_data,
	&riccoArrow_data,
	&riccoStreetStall_data,
	&riccoHericopter_data,
	&surfgeso_red_data,
	&surfgeso_yellow_data,
	&surfgeso_green_data,
	&lamp_bianco_data,
	&big_windmill_data,
	&windmill_block_data,
	&windmill_far_data,
	&MiniWindmillL_data,
	&MiniWindmillS_data,
	&WindmillRoof_data,
	&root_pakkun_data,
	&PolluterPakkun_data,
	&BiaBell_data,
	&BiaWatermill00_data,
	&BiaWatermill01_data,
	&BiaWatermillVertical_data,
	&BiaTurnBridge_data,
	&LeafBoat_data,
	&LeafBoatRotten_data,
	&LampSeesaw_data,
	&BiancoDoor_data,
	&BiaBridge_data,
	&SandBird_data,
	&SandBirdBlock_data,
	&SkyIsland_data,
	&SandLeaf_data,
	&SandLeafBase00_data,
	&SandLeafBase01_data,
	&SandLeafBase02_data,
	&SandLeafBase03_data,
	&SandBomb_data,
	&SandBombBase00_data,
	&SandBombBaseMushroom_data,
	&SandBombBasePyramid_data,
	&SandBombBaseShit_data,
	&SandBombBaseStar_data,
	&SandBombBaseTurtle_data,
	&SandBombBaseFoot_data,
	&SandBombBaseHand_data,
	&SandBombBaseStairs_data,
	&MirrorL_data,
	&MirrorM_data,
	&MirrorS_data,
	&SandCastle_data,
	&MammaBlockRotate_data,
	&SandEgg_data,
	&SandEggBroken_data,
	&ShiningStone_data,
	&MammaSurfboard00_data,
	&MammaSurfboard01_data,
	&MammaSurfboard02_data,
	&MammaSurfboard03_data,
	&MammaSurfboardStand_data,
	&MammaYacht00_data,
	&MammaYacht01_data,
	&MammaYacht02_data,
	&coral00_data,
	&coral01_data,
	&GoalWatermelon_data,
	&TeethOfJuicer_data,
	&SandEggRepair_data,
	&merrygoround_data,
	&merry_egg_data,
	&merry_pole_data,
	&FerrisWheel_data,
	&FerrisLOD_data,
	&gondola_data,
	&viking_data,
	&PinnaEntrance_data,
	&PinnaEntranceOpen_data,
	&SirenaGate_data,
	&BalloonKoopaJr_data,
	&ShellCup_data,
	&Gateshell_data,
	&PinnaHangingBridgeBoard_data,
	&GateManta_data,
	&AmiKing_data,
	&PinnaSunFlower_data,
	&PinnaCoaster_data,
	&roulette00_data,
	&roulette01_data,
	&roulette02_data,
	&casinoroulette_data,
	&slotdrum_data,
	&telesaslot_data,
	&donchou_data,
	&casino_panel_gate_data,
	&GlassBreak_data,
	&Closet_data,
	&ChestRevolve_data,
	&PosterTeresa_data,
	&PictureTeresa_data,
	&PanelBreak_data,
	&PanelRevolve_data,
	&SirenaBlockBreak_data,
	&SakuCasino_data,
	&SirenabossWall_data,
	&SirenaCasinoRoof_data,
	&SirenaShop_data,
	&StainHotel_data,
	&cogwheel_data,
	&CogwheelPlate_data,
	&CogwheelPot_data,
	&mare_float_house_data,
	&StopRock_data,
	&ElasticCode_data,
	&WireBellPurple_data,
	&MuddyBoat_data,
	&Puncher_data,
	&HatoPop_data,
	&MareWaterJump_data,
	&BumpyWall_data,
	&ExBottle_data,
	&CoinFish_data,
	&MareFall_data,
	&MareCork_data,
	&MonteRoot_data,
	&JumpMushroom_data,
	&MonteGoalFlag_data,
	&HangingBridge_data,
	&HangingBridgeBoard_data,
	&SwingBoard_data,
	&Bathtub_data,
	&BathtubStand_data,
	&BathtubStandBreaking_data,
	&normallift_data,
	&exrollcube_data,
	&exkickboard_data,
	&railblockr_data,
	&railblocky_data,
	&railblockb_data,
	&rollblockr_data,
	&rollblocky_data,
	&rollblockb_data,
	&umaibou_data,
	&kamaboko_data,
	&getag_data,
	&getao_data,
	&uirou_data,
	&hikidashi_data,
	&castella_data,
	&yoshiblock_data,
	&Nail_data,
	&PachinkoNail_data,
	&ex1_turn_lift_data,
	&EXskyTumiki_data,
	&EXskyWindow_data,
	&EXskySenro_data,
	&z_turn_disk_data,
	&y_turn_lift_data,
	&FileLoadBlockA_data,
	&FileLoadBlockB_data,
	&FileLoadBlockC_data,
	&NormalBlock_data,
	&IceBlock_data,
	&breakable_block_data,
	&supermario_block_data,
	&move_block_data,
	&fall_slow_block_data,
	&expand_block_data,
	&spread_block_data,
	&sand_block_data,
	&water_roll_block_data,
	&water_power_lift_data,
	&water_power_inertial_lift_data,
	&water_power_ship_data,
	&lean_direct_block_data,
	&lean_indirect_block_data,
	&lean_block_data,
	&skate_block_data,
	&MoveCoin_data,
	&joint_coin_data,
	&cluster_block_data,
	&BrickBlock_data,
	&WaterMelonBlock_data,
	&TelesaBlock_data,
	&JuiceBlock_data,
	&SuperHipDropBlock_data,
	&WoodBlockTriangle_data,
	&WoodBlockPyramid_data,
	&WoodBlockPole_data,
	&WoodBlockLong_data,
	&WoodBlockLarge_data,
	&WoodBlockCone_data,
	&WoodBlockL_data,
	&WoodBlockBridge_data,
	&WoodBlockCube_data,
	&normalvariant0_data,
	&normalvariant1_data,
	&normalvariant2_data,
	&normalvariant3_data,
	&normalvariant4_data,
	&railvariant0_data,
	&railvariant1_data,
	&railvariant2_data,
	&railvariant3_data,
	&railvariant4_data,
	&rollvariant0_data,
	&rollvariant1_data,
	&rollvariant2_data,
	&rollvariant3_data,
	&rollvariant4_data,
	&no_data,
	&end_data,
};

void TMapObjBase::setMatTable(J3DMaterialTable* table)
{
	getModel()->getModelData()->setMaterialTable(table, J3DMatCopyFlag_All);
	unk74->initDL();
	unk74->getUnk4()->lock();
}

void TMapObjBase::setMatTableTex(J3DMaterialTable* table)
{
	getModel()->getModelData()->setMaterialTable(table, J3DMatCopyFlag_Texture);
	unk74->initDL();
	unk74->getUnk4()->lock();
}

void TMapObjBase::initUnique()
{
	// TODO: I hate switches, someone fix this please...
	switch (getActorType()) {
	case 0x2000003C:
		unk74->setLightType(0);
		break;
	case 0x2000000E:
		if (unk74) {
			u32 uVar4 = getModel()->getMatPacket(0)->unk3C;
			getModel()->getMatPacket(0)->unk3C = uVar4 & 0x7fffffff;
		}
		break;
	case 0x40000048:
		setMatTable(gpMapObjManager->unk74);
		SMS_UnifyMaterial(getModel());
		break;
	case 0x4000001C:
		for (int i = 0; i < 2; ++i) {
			unk74 = unk78->mActors[i];
			setMatTable(gpMapObjManager->unk7C);
			SMS_UnifyMaterial(getModel());
		}
		unk74 = unk78->mActors[0];
		if (unkC4->unk0 & 0x4000 ? true : false)
			unkEC->unk8->setAllBGType(0x4000);
		break;
	case 0x4000005A:
		for (int i = 0; i < 2; ++i) {
			unk74 = unk78->mActors[i];
			setMatTable(gpMapObjManager->unk80);
			SMS_UnifyMaterial(getModel());
		}
		unk74 = unk78->mActors[0];
		break;
	case 0x400000BA:
		setMatTable(gpMapObjManager->unk94);
		SMS_UnifyMaterial(getModel());
		break;
	case 0x40000263:
		startAllAnim(unk74, unkF4);
		break;
	case 0x4000003C:
		if (unk74->unkC)
			unk74->unkC->initSimpleMotionBlend(0x14);
		break;
	case 0x400000A8:
	case 0x40000096:
	case 0x4000009A:
	case 0x4000009D:
	case 0x4000009E:
	case 0x4000009F:
	case 0x400000A1:
	case 0x400000A2:
	case 0x400000A3:
		setMatTable(gpMapObjManager->unk8C);
		SMS_UnifyMaterial(getModel());
		break;
	case 0x400000A0:
		setMatTable(gpMapObjManager->unk8C);
		break;
	case 0x4000009C:
		setMatTable(gpMapObjManager->unk90);
		SMS_UnifyMaterial(getModel());
		break;
	case 0x400000A5:
		setMatTable(gpMapObjManager->unk90);
		break;
	case 0x400000CB:
		setMatTable(gpMapObjManager->unkC0);
		SMS_UnifyMaterial(getModel());
		break;
	case 0x400000CC:
		break;
	case 0x400000D3:
		setMatTableTex(gpMapObjManager->unkC0);
		break;
	case 0x400000CF:
		setMatTable(gpMapObjManager->unkC4);
		SMS_UnifyMaterial(getModel());
		break;
	case 0x20000026:
	case 0x2000002A:
	case 0x2000001F:
	case 0x20000022:
		setMatTable(gpMapObjManager->unk6C);
		SMS_UnifyMaterial(getModel());
		break;
	case 0x20000068:
		for (int i = 0; i < 3; ++i) {
			unk74 = unk78->mActors[i];
			setMatTableTex(gpMapObjManager->unk70);
		}
		unk74 = unk78->mActors[0];
		break;
	case 0x400002C2:
		for (int i = 0; i < 2; ++i) {
			unk74 = unk78->mActors[i];
			setMatTable(gpMapObjManager->unk84);
		}
		unk74 = unk78->mActors[0];
		break;
	case 0x400002C3:
		for (int i = 0; i < 2; ++i) {
			unk74 = unk78->mActors[i];
			setMatTable(gpMapObjManager->unk88);
		}
		unk74 = unk78->mActors[0];
		break;
	case 0x400000D0:
		unk74->setLightType(1);
		break;
	case 0x400000DB:
		mPosition.y += mScaling.y * 50.0f;
		calcRootMatrix();
		getModel()->calc();
		mPosition.y -= mScaling.y * 50.0f;
		break;
	case 0x4000001B:
		setMatTable(gpMapObjManager->unk78);
		SMS_UnifyMaterial(getModel());
		break;
	}
}

void TMapObjBase::checkIllegalAttr() const { }

void TMapObjBase::initEventData() { }

void TMapObjBase::initHoldData()
{
	if (getMapObjData()->mHold != nullptr) {
		TMapObjHoldData* hold = getMapObjData()->mHold;
		hold->unk8  = J3DModelLoaderDataBase::load(JKRGetResource(hold->unk0),
		                                           0x240000);
		hold->unkC  = new J3DModel(hold->unk8, 0, 1);
		u16 idx     = hold->unk8->unkB0->getIndex(hold->unk4);
		hold->unk10 = hold->unkC->getAnmMtx(idx);
	}
}

void TMapObjBase::initMapCollisionData()
{
	if (unk130->mCollision != nullptr) {
		const TMapObjCollisionInfo* col = unk130->mCollision;
		unkEC = new TMapCollisionManager(col->unk2, "mapObj", this);
		for (int i = 0; i < col->unk0; ++i)
			if (col->unk4[i].unk0)
				unkEC->init(col->unk4[i].unk0, col->unk4[i].unk4, nullptr);
	}
}

void TMapObjBase::initObjCollisionData()
{
	if (getMapObjData()->mHit != nullptr) {
		initHitActor(getMapObjData()->unk4, getHitObjNumMax(),
		             getMapObjData()->mHit->unk4, 0.0f, 0.0f, 0.0f, 0.0f);
		setObjHitData(0);

		const TMapObjHitDataTable* table = getMapObjData()->mHit->unkC;

		f32 fVar2;
		if (mScaling.x > mScaling.z)
			fVar2 = mScaling.x;
		else
			fVar2 = mScaling.z;

		if (table->unk8 > 0.0f) {
			unkBC = table->unk8 * fVar2;
			unkC0 = table->unkC * mScaling.y;
		} else {
			unkBC = table->unk0 * fVar2;
			unkC0 = table->unk4 * mScaling.y;
		}
	} else {
		initHitActor(0, 1, 0, 0.0f, 0.0f, 0.0f, 0.0f);
		unkBC = 0.0f;
		unkC0 = 0.0f;
	}

	if (mAttackRadius == 0.0f || mAttackHeight == 0.0f)
		unk64 |= 2;

	if (mDamageRadius == 0.0f || mDamageHeight == 0.0f)
		unk64 |= 4;
}

#pragma dont_inline on
void TMapObjBase::initBckMoveData()
{
	if (unk130->mMove != nullptr) {
		TMapObjMoveData* move = unk130->mMove;

		move->unk4 = (J3DAnmTransform*)J3DAnmLoaderDataBase::load(
		    JKRGetResource(move->unk0));

		J3DModelData* data         = unk74->getUnk4()->getModelData();
		data->mJointNodePointer[0] = data->getJointNodePointer(1);

		// TODO: this requires the J3DJoint.hpp header, but that has the dreaded
		// compound literal in .data problem that we share with TWW, so avoid it
		// for now

		// J3DTransformInfo& info
		//     = data->getJointNodePointer(0)->getTransformInfo();
		// info.mScale.x         = 1.0f;
		// info.mScale.y         = 1.0f;
		// info.mScale.z         = 1.0f;
		// info.mRotation.x      = 0;
		// info.mRotation.y      = 0;
		// info.mRotation.z      = 0;
		// info.mTranslate.x     = 0.0f;
		// info.mTranslate.y     = 0.0f;
		// info.mTranslate.z     = 0.0f;
		move->unk8            = new J3DFrameCtrl(move->unk4->mMaxFrame);
		move->unk8->mLoopMode = 2;
		move->unk8->mSpeed    = SMSGetAnmFrameRate();
	}
}
#pragma dont_inline off

bool isAlreadyRegistered(const TMapObjAnimDataInfo* anim, int i)
{
	for (int j = 0; j < i; ++j)
		if (anim->unk4[j].unk0
		    && strcmp(anim->unk4[i].unk0, anim->unk4[j].unk0) == 0)
			return true;
	return false;
}

static void dummy6(const char*) {};

MActor* TMapObjBase::initMActor(const char* param_1, const char* param_2,
                                u32 param_3)
{
	MActor* oldActor = unk74;
	MActor* newActor = getActorKeeper()->createMActor(param_1, param_3);
	unk74            = newActor;
	if (checkMapObjFlag(0x4000)) {
		unk74->setLightID(0);
		unk74->unmarkUnk40();
	}
	calcRootMatrix();
	unk74->calc();
	unk74->viewCalc();
	unk74 = oldActor;
	return newActor;
}

void TMapObjBase::makeMActors()
{
	u16 uVar6 = 1;
	if (unk130->mAnim)
		uVar6 = unk130->mAnim->unk2;

	if (uVar6 == 0)
		return;

	unk78 = new TMActorKeeper(unk70, uVar6);
	if (unkF8 & 0x8000)
		unk78->mModelLoaderFlags = 0x11220000;
	else
		unk78->mModelLoaderFlags = 0x10220000;

	if (unk130->mAnim) {
		const TMapObjAnimDataInfo* anim = unk130->mAnim;
		unk74 = initMActor(anim->unk4[0].unk0, nullptr, getSDLModelFlag());

		for (u16 i = 1; i < anim->unk0; ++i) {
			if (anim->unk4[i].unk10 && unk80 == nullptr)
				initAnmSound();

			if (anim->unk4[i].unk0 != nullptr
			    && !isAlreadyRegistered(anim, i)) {
				initMActor(anim->unk4[i].unk0, nullptr, getSDLModelFlag());
			}
		}
	} else {
		char buffer[64];
		snprintf(buffer, 64, "%s.bmd", unk130->unk0);
		unk74 = initMActor(buffer, nullptr, getSDLModelFlag());
	}
}

void TMapObjBase::initModelData()
{
	makeMActors();
	if (checkMapObjFlag(0x800) && getUnk74()) {
		unkC8 = gpMap->checkGround(getPosition(), &unkC4);
		if (getUnkC4()->checkFlag(0x4000) && !checkMapObjFlag(0x4000))
			gpMapObjManager->entryStaticDrawBufferShadow(getUnk74()->getUnk4());
		else
			gpMapObjManager->entryStaticDrawBufferSun(getUnk74()->getUnk4());
	}

	if (checkMapObjFlag(0x10) || checkMapObjFlag(0x20)) {
		TMirrorActor* ma = new TMirrorActor(getName());
		if (checkMapObjFlag(0x20))
			ma->init(getModel(), 0x1A);
		else
			ma->init(getModel(), 0x18);
	}
}

void TMapObjBase::initActorData()
{
	int i    = 0;
	u16 code = JDrama::TNameRef::calcKeyCode(unkF4);
	for (; sObjDataTable[i]->unk4; ++i) {
		if (code == sObjDataTable[i]->unk38
		    && strcmp(sObjDataTable[i]->unk0, unkF4) == 0)
			break;
	}

	if (strcmp(mName, "地形オブジェ") == 0)
		mName = unkF4;

	unk130 = sObjDataTable[i];
	unkF8  = unk130->unk34;

	unk70 = JDrama::TNameRefGen::search<TLiveManager>(unk130->unk8);
	unk70->manageActor(this);
	if (unk130->mHit)
		unk108 = mScaling.y * unk130->mHit->unk8;
	mPosition.y += unk108;
	unkB8 = unk130->unk30 * mScaling.x;
	if (checkMapObjFlag(0x1))
		offLiveFlag(0x100);
	if (checkMapObjFlag(0x100000))
		unkE8 = 2;
}

void TMapObjBase::initMapObj()
{
	unk10C = getPosition();
	unk118 = getRotation();
	unk124 = getScaling();

	initActorData();
	initModelData();
	initObjCollisionData();
	initMapCollisionData();
	initBckMoveData();
	initHoldData();
	initUnique();
	checkIllegalAttr();
	if (unk74 && checkActorType(0x40000000))
		unk74->setLightType(2);
	if (getMapObjData()->unk30 == 0.0f)
		mLiveFlag |= 0x8;
	if (checkMapObjFlag(0x8000) && !isActorType(0x40000084)) {
		TScreenTexture* ref = JDrama::TNameRefGen::search<TScreenTexture>(
		    "スクリーンテクスチャ");
		const ResTIMG* img = ref->getTexture()->getTexInfo();
		getModel()->getModelData()->getTexture()->setResTIMG(2, *img);
		unk74->setLightType(3);
	}
	makeObjDead();
}

void TMapObjGeneral::initPhysicalData()
{
	if (getMapObjData()->mPhysical)
		unkCC = getMapObjData()->mPhysical->unk4->unk0;
}

void TMapObjGeneral::initMapObj()
{
	TMapObjBase::initMapObj();
	initPhysicalData();
}

void TMapObjManager::initKeyCode()
{
	for (int i = 0; sObjDataTable[i]->unk4 != 0; ++i)
		sObjDataTable[i]->unk38
		    = JDrama::TNameRef::calcKeyCode(sObjDataTable[i]->unk0);
}
