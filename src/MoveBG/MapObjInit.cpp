#include <MoveBG/MapObjManager.hpp>
#include <MoveBG/MapObjGeneral.hpp>
#include <MoveBG/MapObjBase.hpp>

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

// fabricated
struct TMapObjAnimData {
	/* 0x0 */ const char* unk0;
	/* 0x4 */ const char* unk4;
	/* 0x8 */ u8 unk8;
	/* 0x9 */ char unk9[7];
	/* 0x10 */ const char* unk10;
};

// the only real name we have, everything else is fabricated
struct TMapObjAnimDataInfo {
	/* 0x0 */ u16 unk0;
	/* 0x2 */ u16 unk2;
	/* 0x4 */ const TMapObjAnimData* unk4;
};

struct TMapObjHitDataTable {
	/* 0x0 */ f32 unk0;
	/* 0x4 */ f32 unk4;
	/* 0x8 */ f32 unk8;
	/* 0xC */ f32 unkC;
};

// fabricated
struct TMapObjHitInfo {
	/* 0x0 */ int unk0;
	/* 0x4 */ int unk4;
	/* 0x8 */ f32 unk8;
	/* 0xC */ const TMapObjHitDataTable* unkC;
};

// fabricated
struct TMapObjCollisionData {
	/* 0x0 */ const char* unk0;
	/* 0x4 */ u16 unk4;
};

// fabricated
struct TMapObjCollisionInfo {
	/* 0x0 */ u16 unk0;
	/* 0x2 */ u16 unk2;
	/* 0x4 */ const TMapObjCollisionData* unk4;
};

// fabricated
struct TMapObjSoundData {
	/* 0x0 */ u32 unk0[10];
};

// fabricated
struct TMapObjSoundInfo {
	/* 0x0 */ u32 unk0;
	/* 0x4 */ const TMapObjSoundData* unk4;
};

// fabricated
struct TMapObjPhysicalData {
	/* 0x0 */ f32 unk0;
	/* 0x4 */ f32 unk4;
	/* 0x8 */ f32 unk8;
	/* 0xC */ f32 unkC;
	/* 0x10 */ f32 unk10;
	/* 0x14 */ f32 unk14;
	/* 0x18 */ f32 unk18;
	/* 0x1C */ f32 unk1C;
	/* 0x20 */ f32 unk20;
	/* 0x24 */ f32 unk24;
	/* 0x28 */ f32 unk28;
	/* 0x2C */ f32 unk2C;
	/* 0x30 */ f32 unk30;
};

// fabricated
struct TMapObjPhysicalInfo {
	/* 0x0 */ u32 unk0;
	/* 0x4 */ TMapObjPhysicalData* unk4;
	/* 0x8 */ u32 unk8;
};

// fabricated
struct TMapObjSinkData {
	/* 0x0 */ f32 unk0;
	/* 0x4 */ f32 unk4;
};

class J3DModelData;

// fabricated
struct TMapObjHoldData {
	/* 0x0 */ const char* unk0;
	/* 0x4 */ const char* unk4;
	/* 0x8 */ J3DModelData* unk8;
	/* 0xC */ J3DModel* unkC;
	/* 0x10 */ Mtx* unk10;
};

class J3DAnmBase;
class J3DFrameCtrl;

// fabricated
struct TMapObjMoveData {
	/* 0x0 */ const char* unk0;
	/* 0x4 */ J3DAnmBase* unk4;
	/* 0x8 */ J3DFrameCtrl* unk8;
};

// fabricated
struct TMapObjData {
	/* 0x0 */ const char* unk0;
	/* 0x4 */ u32 unk4;
	/* 0x8 */ const char* unk8;
	/* 0xC */ const char* unkC;
	/* 0x10 */ const TMapObjAnimDataInfo* mAnim;
	/* 0x14 */ const TMapObjHitInfo* mHit;
	/* 0x18 */ const TMapObjCollisionInfo* mCollision;
	/* 0x1C */ const TMapObjSoundInfo* mSound;
	/* 0x20 */ const TMapObjPhysicalInfo* mPhysical;
	/* 0x24 */ const TMapObjSinkData* mSink;
	/* 0x28 */ TMapObjHoldData* mHold;
	/* 0x2C */ TMapObjMoveData* mMove;
	/* 0x30 */ u32 unk30;
	/* 0x34 */ u32 unk34;
	/* 0x38 */ u32 unk38;
};

///////////////////////////////////////////// START SORTED AND NICE

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
	0,
	0,
	0,
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
	0,
	0,
	0,
};

static const TMapObjAnimData billboard_dolphin_anim_data[3] = {
	{ "billboardDol.bmd" },
	{ nullptr, "billboardrot" },
	{ nullptr, "billboardrotrev" },
};

static const TMapObjAnimDataInfo billboard_dolphin_anim_info
    = { 3, 1, billboard_dolphin_anim_data };

static const TMapObjHitDataTable billboard_hit_data_table[1] = {
	{ 100.0f, 60.0f, 100.0f, 60.0f },
};

static const TMapObjHitInfo billboard_obj_hit_info
    = { 1, 0x80000000, -100.0f, billboard_hit_data_table };

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
	0,
	0x800,
	0,
};

static const TMapObjAnimData billboard_sun_anim_data[3] = {
	{ "billboardSun.bmd" },
	{ nullptr, "billboardrot" },
	{ nullptr, "billboardrotrev" },
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
	0,
	0x800,
	0,
};

static const TMapObjAnimData billboard_restaurant_anim_data[3] = {
	{ "billboardRestaurant.bmd" },
	{ nullptr, "billboardrot" },
	{ nullptr, "billboardrotrev" },
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
	0,
	0x800,
	0,
};

static const TMapObjAnimData billboard_fish_anim_data[3] = {
	{ "billboardFish.bmd" },
	{ nullptr, "billboardrot" },
	{ nullptr, "billboardrotrev" },
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
	0,
	0x800,
	0,
};

static const TMapObjHitDataTable HideObj_hit_data_table[1] = {
	{ 100.0f, 60.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo HideObj_obj_hit_info
    = { 1, 0x80000000, -10.0f, HideObj_hit_data_table };

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
	0,
	0,
	0,
};

static const TMapObjHitDataTable WaterHitHideObj_hit_data_table[1] = {
	{ 0.0f, 0.0f, 60.0f, 20.0f },
};

static const TMapObjHitInfo WaterHitHideObj_obj_hit_info
    = { 1, 0, 0.0f, WaterHitHideObj_hit_data_table };

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
	0,
	0,
	0,
};

static const TMapObjHitDataTable FruitHitHideObj_hit_data_table[1] = {
	{ 100.0f, 60.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo FruitHitHideObj_obj_hit_info
    = { 1, 0x40000000, 0.0f, FruitHitHideObj_hit_data_table };

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
	0,
	0,
	0,
};

static const TMapObjHitDataTable HipDropHideObj_hit_data_table[1] = {
	{ 100.0f, 60.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo HipDropHideObj_obj_hit_info
    = { 1, 0x40000000, 0.0f, HipDropHideObj_hit_data_table };

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
	0,
	0,
	0,
};

static const TMapObjAnimData MonteChair_anim_data[1] = {
	{ "monte_chair_model.bmd" },
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
	0,
	0xB00,
	0,
};

static const TMapObjAnimData door_anim_data[5] = {
	{ "door.bmd" },
	{ nullptr, "door_open_l", 0, { 0 }, "/scene/mapObj/door_open_l.bas" },
	{ nullptr, "door_open_r", 0, { 0 }, "/scene/mapObj/door_open_r.bas" },
	{ nullptr, "door_kick_l", 0, { 0 }, "/scene/mapObj/door_kick_l.bas" },
	{ nullptr, "door_kick_r", 0, { 0 }, "/scene/mapObj/door_kick_r.bas" },
};

static const TMapObjHitDataTable door_hit_data_table[1] = {
	{ 150.0f, 250.0f, 150.0f, 250.0f },
};

static const TMapObjHitInfo door_obj_hit_info
    = { 1, 0x80000000, 0.0f, door_hit_data_table };

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
	0,
	0x800,
	0,
};

static const TMapObjAnimData doorHotel_anim_data[5] = {
	{ "doorHotel.bmd" },
	{ nullptr, "door_open_l", 0, { 0 }, "/scene/mapObj/door_open_l.bas" },
	{ nullptr, "door_open_r", 0, { 0 }, "/scene/mapObj/door_open_r.bas" },
	{ nullptr, "door_kick_l", 0, { 0 }, "/scene/mapObj/door_kick_l.bas" },
	{ nullptr, "door_kick_r", 0, { 0 }, "/scene/mapObj/door_kick_r.bas" },
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
	0,
	0x200,
	0,
};

static const TMapObjAnimData manhole_anim_data[1] = {
	{ "manhole.bmd", "manhole" },
};

static const TMapObjAnimDataInfo manhole_anim_info
    = { 1, 1, manhole_anim_data };

static const TMapObjHitDataTable manhole_hit_data_table[1] = {
	{ 100.0f, 50.0f, 130.0f, 50.0f },
};

static const TMapObjHitInfo manhole_obj_hit_info
    = { 1, 0x80000000, 0.0f, manhole_hit_data_table };

static const TMapObjCollisionData manhole_map_collision_data[2] = {
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
	0,
	0x100,
	0,
};

static const TMapObjAnimData FruitBasket_anim_data[2] = {
	{ "Basket.bmd" },
	{ nullptr, "basket" },
};

static const TMapObjAnimDataInfo FruitBasket_anim_info
    = { 2, 2, FruitBasket_anim_data };

static const TMapObjHitDataTable FruitBasket_hit_data_table[1] = {
	{ 50.0f, 100.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo FruitBasket_obj_hit_info
    = { 1, 0x40000000, -10.0f, FruitBasket_hit_data_table };

static const TMapObjCollisionData FruitBasket_map_collision_data[1] = {
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
	0,
	0,
	0,
};

static const TMapObjAnimData BasketReverse_anim_data[1] = {
	{ "Basket.bmd" },
};

static const TMapObjAnimDataInfo BasketReverse_anim_info
    = { 1, 1, BasketReverse_anim_data };

static const TMapObjHitDataTable BasketReverse_hit_data_table[1] = {
	{ 0.0f, 0.0f, 100.0f, 200.0f },
};

static const TMapObjHitInfo BasketReverse_obj_hit_info
    = { 1, 0, -200.0f, BasketReverse_hit_data_table };

static const TMapObjCollisionData BasketReverse_map_collision_data[1] = {
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
	0,
	0x808,
	0,
};

static const TMapObjHitDataTable CoconutJuice_hit_data_table[1] = {
	{ 0.0f, 0.0f, 50.0f, 50.0f },
};

static const TMapObjHitInfo CoconutJuice_obj_hit_info
    = { 1, 0, 0.0f, CoconutJuice_hit_data_table };

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
	0,
	0x800,
	0,
};

static const TMapObjHitDataTable Pile_hit_data_table[1] = {
	{ 0.0f, 0.0f, 30.0f, 100.0f },
};

static const TMapObjHitInfo Pile_obj_hit_info
    = { 1, 0, 0.0f, Pile_hit_data_table };

static const TMapObjCollisionData Pile_map_collision_data[1] = {
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
	0x42A00000,
	0x290A,
	0,
};

static const TMapObjHitDataTable ChangeStage_hit_data_table[1] = {
	{ 100.0f, 100.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo ChangeStage_obj_hit_info
    = { 1, 0x80000000, 0.0f, ChangeStage_hit_data_table };

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
	0,
	0,
	0,
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
	0,
	0,
	0,
};

static const TMapObjHitDataTable StartDemo_hit_data_table[1] = {
	{ 100.0f, 100.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo StartDemo_obj_hit_info
    = { 1, 0x80000000, 0.0f, StartDemo_hit_data_table };

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
	0,
	0,
	0,
};

static const TMapObjAnimData ChipShine_anim_data[1] = {
	{ "chip_shine_model1.bmd" },
};

static const TMapObjAnimDataInfo ChipShine_anim_info
    = { 1, 1, ChipShine_anim_data };

static const TMapObjHitDataTable ChipShine_hit_data_table[1] = {
	{ 0.0f, 0.0f, 150.0f, 300.0f },
};

static const TMapObjHitInfo ChipShine_obj_hit_info
    = { 1, 0, -150.0f, ChipShine_hit_data_table };

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
	0,
	0x900,
	0,
};

static const TMapObjAnimData KoopaJrSignM_anim_data[1] = {
	{ "koopa_jr_sign.bmd" },
};

static const TMapObjAnimDataInfo KoopaJrSignM_anim_info
    = { 1, 1, KoopaJrSignM_anim_data };

static const TMapObjHitDataTable KoopaJrSignM_hit_data_table[1] = {
	{ 0.0f, 0.0f, 150.0f, 300.0f },
};

static const TMapObjHitInfo KoopaJrSignM_obj_hit_info
    = { 1, 0, -150.0f, KoopaJrSignM_hit_data_table };

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
	0,
	0x900,
	0,
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
	0,
	0x900,
	0,
};

static const TMapObjAnimData SignCircle_anim_data[1] = {
	{ "maru_sign.bmd" },
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
	0,
	0x900,
	0,
};

static const TMapObjAnimData SignCross_anim_data[1] = {
	{ "batu_sign.bmd" },
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
	0x0,
	0x900,
	0x0,
};

static const TMapObjAnimData SignTriangle_anim_data[1] = {
	{ "3kaku_sign.bmd" },
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
	0x0,
	0x900,
	0x0,
};

static const TMapObjHitDataTable ArrowBoardLR_hit_data_table[1] = {
	{ 0.0f, 0.0f, 120.0f, 300.0f },
};

static const TMapObjHitInfo ArrowBoardLR_obj_hit_info
    = { 1, 0x80000000, 0.0f, ArrowBoardLR_hit_data_table };

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
	0x0,
	0x900,
	0x0,
};

static const TMapObjHitDataTable ArrowBoardUp_hit_data_table[1] = {
	{ 0.0f, 0.0f, 80.0f, 440.0f },
};

static const TMapObjHitInfo ArrowBoardUp_obj_hit_info
    = { 1, 0x80000000, 0.0f, ArrowBoardUp_hit_data_table };

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
	0,
	0x900,
	0,
};

static const TMapObjHitDataTable ArrowBoardDown_hit_data_table[1] = {
	{ 0.0f, 0.0f, 80.0f, 440.0f },
};

static const TMapObjHitInfo ArrowBoardDown_obj_hit_info
    = { 1, 0x80000000, 0.0f, ArrowBoardDown_hit_data_table };

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
	0,
	0x900,
	0,
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
	0,
	0,
	0,
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
	0,
	0,
	0,
};

static const TMapObjCollisionData ObjSwitch_map_collision_data[1] = {
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
	0x0,
	0x201,
	0x0,
};

static const TMapObjCollisionData RedCoinSwitch_map_collision_data[1] = {
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
	0x0,
	0x201,
	0x0,
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
	0x0,
	0x800,
	0x0,
};

static const TMapObjHitDataTable GeneralHitObj_hit_data_table[1] = {
	{ 0.0f, 0.0f, 50.0f, 100.0f },
};

static const TMapObjHitInfo GeneralHitObj_obj_hit_info
    = { 1, 0x80000000, 0.0f, GeneralHitObj_hit_data_table };

static TMapObjData GeneralHitObj_data = {
	"pGeneralHitObj",
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
	0x0,
	0x900,
	0x0,
};

static const TMapObjHitDataTable item_hit_data_table[1] = {
	{ 40.0f, 40.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo item_obj_hit_info
    = { 1, 0x80000000, 0.0f, item_hit_data_table };

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
	0x42200000,
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
	0x42200000,
	0x0A040100,
	0x00000000,
};

static const TMapObjHitDataTable WaterRecoverObj_hit_data_table[1] = {
	{ 100.0f, 100.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo WaterRecoverObj_obj_hit_info
    = { 1, 0x80000000, 0.0f, item_hit_data_table };

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
	0,
	0x100,
	0,
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
	0x42200000,
	0x4000800,
	0,
};

static TMapObjPhysicalData nozzle_normal_physical_data = {
	0.34999999, 0.5, 0.0, 5.0, 0.8, 0.94999999, 1.0,
	0.0,        0.0, 0.0, 0.0, 0.0, 0.0,
};

static TMapObjPhysicalInfo nozzle_normal_physical_info
    = { 0xD, &nozzle_normal_physical_data, 0 };

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
	0x42200000,
	0xA040100,
	0,
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
	0x42200000,
	0xA040100,
	0,
};

static const TMapObjHitDataTable nozzle_item_hit_data_table[1] = {
	{ 40.0f, 40.0f, 100.0f, 40.0f },
};

static const TMapObjHitInfo nozzle_item_obj_hit_info
    = { 1, 0x80000000, 0.0f, nozzle_item_hit_data_table };

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
	0x42200000,
	0xA040100,
	0,
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
	0x42200000,
	0xA040100,
	0,
};

static const TMapObjHitDataTable coin_hit_data_table[1] = {
	{ 70.0f, 100.0f, 70.0f, 100.0f },
};

static const TMapObjHitInfo coin_obj_hit_info
    = { 1, 0x80000000, 0.0f, coin_hit_data_table };

static const TMapObjSoundData coin_sound_data = {
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x4842,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
};

static const TMapObjSoundInfo coin_sound_info = { 0xA, &coin_sound_data };

static TMapObjPhysicalData coin_physical_data = {
	0.34999999, 0.5, 0.6, 1.0, 0.5, 0.94999999, 0.99,
	0.0,        0.0, 0.0, 0.0, 0.0, 0.0,
};

static TMapObjPhysicalInfo coin_physical_info
    = { 0xD, &coin_physical_data, 0x0 };

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
	0x42480000,
	0x8144100,
	0,
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
	0,
	0xC100000,
	0,
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
	0x42480000,
	0x18104100,
	0,
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
	0x42480000,
	0x18104100,
	0,
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
	0x42480000,
	0x4000,
	0,
};

static const TMapObjAnimData mario_cap_anim_data[1] = {
	{ "mariocap.bmd" },
};

static const TMapObjAnimDataInfo mario_cap_anim_info
    = { 1, 1, mario_cap_anim_data };

static const TMapObjHitDataTable mario_cap_hit_data_table[1] = {
	{ 100.0f, 50.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo mario_cap_obj_hit_info
    = { 1, 0x80000000, 0.0f, mario_cap_hit_data_table };

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
	0,
	0x14100001,
	0,
};

static const TMapObjAnimData coin_anim_data[1] = {
	{ "coin.bmd" },
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
	0x42A00000,
	0x8104100,
	0,
};

static const TMapObjAnimData eggYoshi_anim_data[4] = {
	{ "eggYoshi_normal.bmd", "eggyoshi_wait" },
	{ nullptr, "eggyoshi_true" },
	{ nullptr, "eggyoshi_false" },
	{ "eggYoshi_broken.bmd", "eggyoshi_broken" },
};

static const TMapObjAnimDataInfo eggYoshi_anim_info
    = { 4, 2, eggYoshi_anim_data };

static const TMapObjHitDataTable eggYoshi_hit_data_table[10] = {
	{ 300.0f, 100.0f, 50.0f, 50.0f }, { 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f, 0.0f },       { 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f, 0.0f },       { 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f, 0.0f },       { 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f, 0.0f },       { 0.0f, 0.0f, 0.0f, 0.0f },
};

static const TMapObjHitInfo eggYoshi_obj_hit_info
    = { 10, 0xC0000000, 0.0f, eggYoshi_hit_data_table };

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
	0x42A00000,
	0,
	0,
};

static TMapObjPhysicalData EggYoshi_physical_data = {
	0.34999999, 0.1, 0.0, 5.0, 1.0, 0.94999999, 1.0,
	0.0,        0.0, 0.0, 0.0, 0.0, 0.0,
};

static TMapObjPhysicalInfo EggYoshi_physical_info
    = { 0xD, &EggYoshi_physical_data, 0 };

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
	0x42A00000,
	1,
	0,
};

static const TMapObjAnimData NozzleBox_anim_data[4] = {
	{ "nozzleBox.bmd" },
	{},
	{ "nozzleBoxCrash.bmd", "nozzleboxcrash" },
	{ "nozzleBoxAlpha.bmd" },
};

static const TMapObjAnimDataInfo NozzleBox_anim_info
    = { 4, 3, NozzleBox_anim_data };

static const TMapObjHitDataTable NozzleBox_hit_data_table[1] = {
	{ 200.0f, 200.0f, 100.0f, 200.0f },
};

static const TMapObjHitInfo NozzleBox_obj_hit_info
    = { 1, 0x80000000, 0.0f, NozzleBox_hit_data_table };

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
	0,
	0x14000000,
	0,
};

static const TMapObjAnimData mushroom1up_anim_data[1] = {
	{ "mushroom1up.bmd" },
};

static const TMapObjAnimDataInfo mushroom1up_anim_info
    = { 1, 1, mushroom1up_anim_data };

static const TMapObjHitDataTable mushroom1up_hit_data_table[1] = {
	{ 20.0f, 40.0f, 20.0f, 40.0f },
};

static const TMapObjHitInfo mushroom1up_obj_hit_info
    = { 1, 0, 0.0f, mushroom1up_hit_data_table };

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
	0x42200000,
	0,
	0,
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
	0x42200000,
	0,
	0,
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
	0x42200000,
	0,
	0,
};

static TMapObjData jumpbase_data = {

};

static TMapObjPhysicalData coconut_physical_data = {

};

static TMapObjPhysicalInfo coconut_physical_info = {

};

static TMapObjData coconut_data = {

};

static TMapObjPhysicalData papaya_physical_data = {

};

static TMapObjPhysicalInfo papaya_physical_info = {

};

static TMapObjData papaya_data = {

};

static TMapObjPhysicalData pine_physical_data = {

};

static TMapObjPhysicalInfo pine_physical_info = {

};

static TMapObjData pine_data = {

};

static TMapObjData CoverPine_data = {

};

static TMapObjPhysicalData durian_physical_data = {

};

static TMapObjPhysicalInfo durian_physical_info = {

};

static TMapObjData durian_data = {

};

static TMapObjPhysicalData banana_physical_data = {

};

static TMapObjPhysicalInfo banana_physical_info = {

};

static TMapObjData banana_data = {

};

static TMapObjPhysicalData red_pepper_physical_data = {

};

static TMapObjPhysicalInfo red_pepper_physical_info = {

};

static TMapObjData RedPepper_data = {

};

static TMapObjData fence_normal_data = {

};

static TMapObjData fence3x3_data = {

};

static TMapObjData fence_revolve_data = {

};

static TMapObjData fence_revolve_inner_data = {

};

static TMapObjData fenceInnerGreen_data = {

};

static TMapObjData FenceWaterV_data = {

};

static TMapObjData FenceWaterH_data = {

};

static TMapObjData RailFence_data = {

};

static TMapObjData bambooFenceRevolveInner_data = {

};

static TMapObjData bambooFenceRevolveOuter_data = {

};

static TMapObjData bambooRailFence_data = {

};

static TMapObjPhysicalData wood_barrel_physical_data = {

};

static TMapObjPhysicalInfo wood_barrel_physical_info = {

};

static TMapObjHoldData wood_barrel_hold_data = {

};

static TMapObjData wood_barrel_data = {

};

static TMapObjData wood_barrel_once_data = {

};

static TMapObjData barrel_float_data = {

};

static TMapObjData drum_can_data = {

};

static TMapObjData barrel_oil_data = {

};

static TMapObjData breakable_block_data = {

};

static TMapObjData supermario_block_data = {

};

static TMapObjData move_block_data = {

};

static TMapObjData fall_slow_block_data = {

};

static TMapObjData expand_block_data = {

};

static TMapObjData spread_block_data = {

};

static TMapObjData water_roll_block_data = {

};

static TMapObjData sand_block_data = {

};

static TMapObjData water_power_lift_data = {

};

static TMapObjData water_power_inertial_lift_data = {

};

static TMapObjData water_power_ship_data = {

};

static TMapObjData lean_direct_block_data = {

};

static TMapObjData lean_indirect_block_data = {

};

static TMapObjData lean_block_data = {

};

static TMapObjData skate_block_data = {

};

static TMapObjData MoveCoin_data = {

};

static TMapObjData cluster_block_data = {

};

static TMapObjData NormalBlock_data = {

};

static TMapObjData IceBlock_data = {

};

static TMapObjData BrickBlock_data = {

};

static TMapObjData WaterMelonBlock_data = {

};

static TMapObjData TelesaBlock_data = {

};

static TMapObjData WoodBlockPole_data = {

};

static TMapObjData JuiceBlock_data = {

};

static TMapObjData SuperHipDropBlock_data = {

};

static TMapObjData palmNormal_data = {

};

static TMapObjData palmOugi_data = {

};

static TMapObjData palmSago_data = {

};

static TMapObjData palmNatume_data = {

};

static TMapObjData palmLeaf_data = {

};

static TMapObjData BananaTree_data = {

};

static TMapObjData FruitTree_data = {

};

static TMapObjData flower_data = {

};

static TMapObjData flowerOrange_data = {

};

static TMapObjData flowerPink_data = {

};

static TMapObjData flowerPurple_data = {

};

static TMapObjData flowerRed_data = {

};

static TMapObjData flowerYellow_data = {

};

static TMapObjData flowerSunflower_data = {

};

static TMapObjData telegraph_pole_l_data = {

};

static TMapObjData telegraph_pole_s_data = {

};

static TMapObjData streetlamp_data = {

};

static TMapObjData PoleNormal_data = {

};

static TMapObjPhysicalData football_physical_data = {

};

static TMapObjPhysicalInfo football_physical_info = {

};

static TMapObjData football_data = {

};

static TMapObjData football_goal_data = {

};

static TMapObjPhysicalData baloonball_physical_data = {

};

static TMapObjPhysicalInfo baloonball_physical_info = {

};

static TMapObjData baloonball_data = {

};

static TMapObjData coconutball_data = {

};

static TMapObjPhysicalData watermelon_physical_data = {

};

static TMapObjPhysicalInfo watermelon_physical_info = {

};

static TMapObjData watermelon_data = {

};

static TMapObjData WatermelonStatic_data = {

};

static TMapObjData cloud_data = {

};

static TMapObjData normallift_data = {

};

static TMapObjData exrollcube_data = {

};

static TMapObjData exkickboard_data = {

};

static TMapObjData railblockr_data = {

};

static TMapObjData railblocky_data = {

};

static TMapObjData railblockb_data = {

};

static TMapObjData rollblockr_data = {

};

static TMapObjData rollblocky_data = {

};

static TMapObjData rollblockb_data = {

};

static TMapObjData umaibou_data = {

};

static TMapObjData kamaboko_data = {

};

static TMapObjData getag_data = {

};

static TMapObjData getao_data = {

};

static TMapObjData uirou_data = {

};

static TMapObjData hikidashi_data = {

};

static TMapObjData castella_data = {

};

static TMapObjData yoshiblock_data = {

};

static TMapObjData WoodBlockTriangle_data = {

};

static TMapObjData WoodBlockPyramid_data = {

};

static TMapObjData WoodBlockLong_data = {

};

static TMapObjData WoodBlockLarge_data = {

};

static TMapObjData WoodBlockCone_data = {

};

static TMapObjData WoodBlockL_data = {

};

static TMapObjData WoodBlockBridge_data = {

};

static TMapObjData WoodBlockCube_data = {

};

static TMapObjData normalvariant0_data = {

};

static TMapObjData normalvariant1_data = {

};

static TMapObjData normalvariant2_data = {

};

static TMapObjData normalvariant3_data = {

};

static TMapObjData normalvariant4_data = {

};

static TMapObjData railvariant0_data = {

};

static TMapObjData railvariant1_data = {

};

static TMapObjData railvariant2_data = {

};

static TMapObjData railvariant3_data = {

};

static TMapObjData railvariant4_data = {

};

static TMapObjData rollvariant0_data = {

};

static TMapObjData rollvariant1_data = {

};

static TMapObjData rollvariant2_data = {

};

static TMapObjData rollvariant3_data = {

};

static TMapObjData rollvariant4_data = {

};

static TMapObjData lamptrapspike_data = {

};

static TMapObjData lamptrapiron_data = {

};

static TMapObjData airplane_data = {

};

static TMapObjData WoodBox_data = {

};

static TMapObjData AirportPole_data = {

};

static TMapObjData ice_car_data = {

};

static TMapObjData move_ice_car_data = {

};

static TMapObjData AirportBuoy_data = {

};

static TMapObjData dptCannon_data = {

};

static TMapObjData dptKing_data = {

};

static TMapObjData KoopaJrSubmarine_data = {

};

static TMapObjData monumentshine_data = {

};

static TMapObjData belldolpic_data = {

};

static TMapObjData dptWeathercock_data = {

};

static TMapObjData dptMonteFence_data = {

};

static TMapObjData dptCoronaFence_data = {

};

static TMapObjData MapSmoke_data = {

};

static TMapObjData MareGate_data = {

};

static TMapObjData DemoCannon_data = {

};

static TMapObjData NozzleDoor_data = {

};

static TMapObjData DokanGate_data = {

};

static TMapObjData crane_data = {

};

static TMapObjData crane90Scene2_data = {

};

static TMapObjData crane180Scene1_data = {

};

static TMapObjData craneUpDown_data = {

};

static TMapObjData craneCargoUpDown_data = {

};

static TMapObjData tank_data = {

};

static TMapObjData container_data = {

};

static TMapObjData submarine_data = {

};

static TMapObjData riccoShip_data = {

};

static TMapObjData riccoPole_data = {

};

static TMapObjData riccoLog_data = {

};

static TMapObjData gesoSurfboard_data = {

};

static TMapObjData gesoSurfboardStatic_data = {

};

static TMapObjData riccoWatermill_data = {

};

static TMapObjData riccoShipLog_data = {

};

static TMapObjData riccoShipDol_data = {

};

static TMapObjData riccoYachtL_data = {

};

static TMapObjData riccoYachtS_data = {

};

static TMapObjData riccoBoatL_data = {

};

static TMapObjData riccoBoatS_data = {

};

static TMapObjData riccoBasket_data = {

};

static TMapObjData riccoGangway_data = {

};

static TMapObjData riccoShipFish_data = {

};

static TMapObjData riccoShipGeso_data = {

};

static TMapObjData riccoSwitch_data = {

};

static TMapObjData riccoSwitchShine_data = {

};

static TMapObjData riccoFenceMaze_data = {

};

static TMapObjData riccoStand_data = {

};

static TMapObjData riccoGrille_data = {

};

static TMapObjData riccoArrow_data = {

};

static TMapObjData riccoStreetStall_data = {

};

static TMapObjData riccoHericopter_data = {

};

static TMapObjData surfgeso_red_data = {

};

static TMapObjData surfgeso_yellow_data = {

};

static TMapObjData surfgeso_green_data = {

};

static TMapObjData big_windmill_data = {

};

static TMapObjData windmill_far_data = {

};

static TMapObjData MiniWindmillL_data = {

};

static TMapObjData MiniWindmillS_data = {

};

static TMapObjData WindmillRoof_data = {

};

static TMapObjData lamp_bianco_data = {

};

static TMapObjData root_pakkun_data = {

};

static TMapObjData windmill_block_data = {

};

static TMapObjData PolluterPakkun_data = {

};

static TMapObjData BiaBell_data = {

};

static TMapObjData BiaWatermill00_data = {

};

static TMapObjData BiaWatermill01_data = {

};

static TMapObjData BiaWatermillVertical_data = {

};

static TMapObjData BiaTurnBridge_data = {

};

static TMapObjData LeafBoat_data = {

};

static TMapObjData LeafBoatRotten_data = {

};

static TMapObjData LampSeesaw_data = {

};

static TMapObjData BiancoDoor_data = {

};

static TMapObjData BiaBridge_data = {

};

static TMapObjData SandBird_data = {

};

static TMapObjData SandBirdBlock_data = {

};

static TMapObjData SkyIsland_data = {

};

static TMapObjData SandLeaf_data = {

};

static TMapObjData SandLeafBase00_data = {

};

static TMapObjData SandLeafBase01_data = {

};

static TMapObjData SandLeafBase02_data = {

};

static TMapObjData SandLeafBase03_data = {

};

static TMapObjData SandBomb_data = {

};

static TMapObjData MirrorL_data = {

};

static TMapObjData MirrorM_data = {

};

static TMapObjData MirrorS_data = {

};

static TMapObjData SandCastle_data = {

};

static TMapObjData MammaBlockRotate_data = {

};

static TMapObjData SandEgg_data = {

};

static TMapObjData SandEggBroken_data = {

};

static TMapObjData ShiningStone_data = {

};

static TMapObjData MammaSurfboard00_data = {

};

static TMapObjData MammaSurfboard01_data = {

};

static TMapObjData MammaSurfboard02_data = {

};

static TMapObjData MammaSurfboard03_data = {

};

static TMapObjData MammaSurfboardStand_data = {

};

static TMapObjData MammaYacht00_data = {

};

static TMapObjData MammaYacht01_data = {

};

static TMapObjData MammaYacht02_data = {

};

static TMapObjData SandBombBase00_data = {

};

static TMapObjData SandBombBaseMushroom_data = {

};

static TMapObjData SandBombBasePyramid_data = {

};

static TMapObjData SandBombBaseShit_data = {

};

static TMapObjData SandBombBaseStar_data = {

};

static TMapObjData SandBombBaseTurtle_data = {

};

static TMapObjData SandBombBaseFoot_data = {

};

static TMapObjData SandBombBaseHand_data = {

};

static TMapObjData SandBombBaseStairs_data = {

};

static TMapObjData coral00_data = {

};

static TMapObjData coral01_data = {

};

static TMapObjData GoalWatermelon_data = {

};

static TMapObjData TeethOfJuicer_data = {

};

static TMapObjData SandEggRepair_data = {

};

static TMapObjData merrygoround_data = {

};

static TMapObjData merry_egg_data = {

};

static TMapObjData merry_pole_data = {

};

static TMapObjData FerrisWheel_data = {

};

static TMapObjData gondola_data = {

};

static TMapObjData FerrisLOD_data = {

};

static TMapObjData viking_data = {

};

static TMapObjData PinnaEntrance_data = {

};

static TMapObjData PinnaEntranceOpen_data = {

};

static TMapObjData SirenaGate_data = {

};

static TMapObjData BalloonKoopaJr_data = {

};

static TMapObjData ShellCup_data = {

};

static TMapObjData Gateshell_data = {

};

static TMapObjData PinnaHangingBridgeBoard_data = {

};

static TMapObjData GateManta_data = {

};

static TMapObjData PinnaSunFlower_data = {

};

static TMapObjData AmiKing_data = {

};

static TMapObjData PinnaCoaster_data = {

};

static TMapObjData casinoroulette_data = {

};

static TMapObjData roulette00_data = {

};

static TMapObjData roulette01_data = {

};

static TMapObjData roulette02_data = {

};

static TMapObjData slotdrum_data = {

};

static TMapObjData telesaslot_data = {

};

static TMapObjData donchou_data = {

};

static TMapObjData casino_panel_gate_data = {

};

static TMapObjData SakuCasino_data = {

};

static TMapObjData SirenabossWall_data = {

};

static TMapObjData SirenaCasinoRoof_data = {

};

static TMapObjData GlassBreak_data = {

};

static TMapObjData Closet_data = {

};

static TMapObjData ChestRevolve_data = {

};

static TMapObjData PosterTeresa_data = {

};

static TMapObjData PictureTeresa_data = {

};

static TMapObjData PanelBreak_data = {

};

static TMapObjData PanelRevolve_data = {

};

static TMapObjData SirenaBlockBreak_data = {

};

static TMapObjData SirenaShop_data = {

};

static TMapObjData StainHotel_data = {

};

static TMapObjData cogwheel_data = {

};

static TMapObjData CogwheelPlate_data = {

};

static TMapObjData CogwheelPot_data = {

};

static TMapObjData mare_float_house_data = {

};

static TMapObjPhysicalData StopRock_physical_data = {

};

static TMapObjPhysicalInfo StopRock_physical_info = {

};

static TMapObjData StopRock_data = {

};

static TMapObjData ElasticCode_data = {

};

static TMapObjData GrowthTree_data = {

};

static TMapObjData WireBellPurple_data = {

};

static TMapObjData MuddyBoat_data = {

};

static TMapObjData Puncher_data = {

};

static TMapObjData HatoPop_data = {

};

static TMapObjData MareWaterJump_data = {

};

static TMapObjData BumpyWall_data = {

};

static TMapObjData ExBottle_data = {

};

static TMapObjData CoinFish_data = {

};

static TMapObjData MareFall_data = {

};

static TMapObjData MareCork_data = {

};

static TMapObjData MonteRoot_data = {

};

static TMapObjData JumpMushroom_data = {

};

static TMapObjData MonteGoalFlag_data = {

};

static TMapObjData HangingBridgeBoard_data = {

};

static TMapObjData HangingBridge_data = {

};

static TMapObjData SwingBoard_data = {

};

static TMapObjData Fluff_data = {

};

static TMapObjData FluffManager_data = {

};

static TMapObjData Bathtub_data = {

};

static TMapObjData BathtubStand_data = {

};

static TMapObjData BathtubStandBreaking_data = {

};

static TMapObjMoveData ex1_turn_lift_move_data = {

};

static TMapObjData ex1_turn_lift_data = {

};

static TMapObjMoveData z_turn_disk_move_data = {

};

static TMapObjData z_turn_disk_data = {

};

static TMapObjMoveData y_turn_lift_move_data = {

};

static TMapObjData y_turn_lift_data = {

};

static TMapObjData EXskyTumiki_data = {

};

static TMapObjData EXskyWindow_data = {

};

static TMapObjData EXskySenro_data = {

};

static TMapObjData Nail_data = {

};

static TMapObjData PachinkoNail_data = {

};

static TMapObjData FileLoadBlockA_data = {

};

static TMapObjData FileLoadBlockB_data = {

};

static TMapObjData FileLoadBlockC_data = {

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

////////////////////////////////////////////////////////// END .data

void TMapObjManager::initKeyCode() { }

void TMapObjGeneral::initMapObj() { }

void TMapObjGeneral::initPhysicalData() { }

void TMapObjBase::initMapObj() { }

void TMapObjBase::initActorData() { }

void TMapObjBase::initModelData() { }

void TMapObjBase::makeMActors() { }

void TMapObjBase::initMActor(const char*, const char*, u32) { }

void isAlreadyRegistered(const TMapObjAnimDataInfo*, int) { }

void TMapObjBase::initBckMoveData() { }

void TMapObjBase::initObjCollisionData() { }

void TMapObjBase::initMapCollisionData() { }

void TMapObjBase::initHoldData() { }

void TMapObjBase::initEventData() { }

void TMapObjBase::checkIllegalAttr() const { }

void TMapObjBase::initUnique() { }

void TMapObjBase::setMatTableTex(J3DMaterialTable*) { }

void TMapObjBase::setMatTable(J3DMaterialTable*) { }
