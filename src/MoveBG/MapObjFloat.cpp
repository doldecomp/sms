#include <MoveBG/MapObjFloat.hpp>
#include <MoveBG/MapObjLibWave.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/JParticle/JPAResourceManager.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

struct MapObjFloatParams {
	/* 0x0 */ const char* unk0;
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
};

// clang-format off
static const MapObjFloatParams param_table[] = {
	{ "riccoShip", 0.01f, 0.01f, 0.5f, 150.0f, 400.0f, 40.0f, 80.0f, 3.0f, 1.0f, 0.0f },
	{ "ukisima", 0.01f, 0.01f, 0.1f, 150.0f, 400.0f, 40.0f, 80.0f, 3.0f, 1.0f, 6.25f },
	{ "Buoy", 0.01f, 0.01f, 0.2f, 100.0f, 100.0f, 40.0f, 40.0f, 2.0f, 1.0f, 3.2f },
	{ "riccoLog", 0.01f, 0.01f, 0.25f, 100.0f, 100.0f, 40.0f, 90.0f, 3.0f, 1.0f, 4.5f },
	{ "riccoYachtL", 0.001f, 0.005f, 0.1f, 250.0f, 400.0f, 40.0f, 40.0f, 3.0f, 1.0f, 0.0f },
	{ "riccoYachtS", 0.001f, 0.005f, 0.15f, 250.0f, 400.0f, 40.0f, 40.0f, 3.0f, 1.0f, 0.0f },
	{ "riccoBoatL", 0.001f, 0.005f, 0.08f, 250.0f, 400.0f, 40.0f, 40.0f, 3.0f, 1.0f, 0.0f },
	{ "riccoBoatS", 0.001f, 0.005f, 0.05f, 250.0f, 400.0f, 40.0f, 40.0f, 3.0f, 1.0f, 0.0f },
	{ "riccoShipLog", 0.0f, 0.0f, 0.0f, 1000.0f, 500.0f, 100.0f, 0.0f, 1.0f, 0.5f, 0.0f },
	{ "riccoShipDol", 0.0f, 0.0f, 0.0f, 1000.0f, 500.0f, 100.0f, 0.0f, 1.0f, 0.5f, 0.0f },
	{ "mareWaterJump", 0.01f, 0.01f, 0.3f, 150.0f, 400.0f, 40.0f, 80.0f, 3.0f, 1.0f, 6.5f },
	{ nullptr, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
};
// clang-format on

void TMapObjFloatOnSea::touchPlayer(THitActor* param_1)
{
	TLeanBlock::touchPlayer(param_1);
	if (marioIsOn())
		unk194->push();
}

void TMapObjFloatOnSea::control()
{
	TLeanBlock::control();
	unk194->movement();

	static f32 timer = 0.0f;

	MtxPtr mtx = getModel()->getAnmMtx(0);
	mtx[1][3] += unk194->getCurrentHeight(mPosition.x, mPosition.z);
}

void TMapObjFloatOnSea::calc()
{
	unk1A0.x = mPosition.x;
	unk1A0.y = 0.0f;
	unk1A0.z = mPosition.z;

	if (unk19C > unk198) {
		JGeometry::TVec3<f32> v(unk1AC, unk1AC, unk1AC);
		emitAndScale(0x1F6, 3, &unk1A0, v);
		emitAndScale(0x1C6, 1, &unk1A0, v);
		unk19C = 0;
	} else {
		unk19C += 1;
	}
}

// TODO: move this
extern u8 gParticleFlagLoaded[];
extern JPAResourceManager* gpResourceManager;
inline static void loadParticle(const char* path, u32 id)
{
	if (gParticleFlagLoaded[id] == 0) {
		gpResourceManager->load(path, id);
		gParticleFlagLoaded[id] = 1;
	}
}

void TMapObjFloatOnSea::initMapObj()
{
	TLeanBlock::initMapObj();

	int idx = 0;
	while (strcmp(param_table[idx].unk0, unkF4))
		++idx;

	unk140 = param_table[idx].unk4;
	unk144 = param_table[idx].unk8;
	unk148 = param_table[idx].unkC;
	unk138 = param_table[idx].unk10;
	unk13C = param_table[idx].unk14;
	unk198 = 0xF;
	unk1AC = param_table[idx].unk28;

	loadParticle("/scene/mapObj/ms_obj_hamon_b.jpa", 0x1F6);
	loadParticle("/scene/mapObj/ms_obj_hamon_a.jpa", 0x1C6);

	unk194 = new TMapObjLibWave(param_table[idx].unk18, param_table[idx].unk1C,
	                            param_table[idx].unk20, param_table[idx].unk24);
}

TMapObjFloatOnSea::TMapObjFloatOnSea(const char* name)
    : TLeanBlock(name)
    , unk194(nullptr)
    , unk198(0)
    , unk19C(0)
    , unk1A0(0.0f, 0.0f, 0.0f)
    , unk1AC(0.0f)
{
}
