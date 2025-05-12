#include <MoveBG/Pool.hpp>
#include <Map/MapData.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <dolphin/gx.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

TPoolManager* gpPoolManager;

void TPool::draw() const
{
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXLoadPosMtxImm(j3dSys.getViewMtx(), 0);
	GXSetCurrentMtx(0);
	GXSetNumChans(1);
	GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanMatColor(GX_COLOR0A0, unk148);
	GXSetNumTexGens(0);
	GXSetNumTevStages(1);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
	GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_RASC, GX_CC_ZERO, GX_CC_ZERO,
	                GX_CC_ZERO);
	GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_TRUE, GX_TEVPREV);
	GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_RASA, GX_CA_ZERO, GX_CA_ZERO,
	                GX_CA_ZERO);
	GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_TRUE, GX_TEVPREV);
	GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
	GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_AND, GX_GREATER, 0);
	GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
	GXSetCullMode(GX_CULL_NONE);
	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition3f32(mPosition.x - 200.0f, getWaterLevel(), mPosition.z - 200.0f);
	GXPosition3f32(mPosition.x + 200.0f, getWaterLevel(), mPosition.z - 200.0f);
	GXPosition3f32(mPosition.x + 200.0f, getWaterLevel(), mPosition.z + 200.0f);
	GXPosition3f32(mPosition.x - 200.0f, getWaterLevel(), mPosition.z + 200.0f);
	GXEnd();
}

bool TPool::subWaterLevel()
{
	unk13C -= unk144;
	if (unk13C < 5.0f) {
		makeObjDead();
		return false;
	}

	return true;
}

void TPool::makePoolUnlimited() { }

void TPool::makeObjAppeared()
{
	TMapObjBase::makeObjAppeared();
	unk13C = unk140;
}

void TPool::load(JSUMemoryInputStream& stream)
{
	TMapObjBase::load(stream);
	unk144 = 1.0f;
	unk140 = 100.0f;
	unk13C = unk140;
}

TPool::TPool(const char* name)
    : TMapObjBase(name)
    , unk138(0)
    , unk13C(0.0f)
    , unk140(0.0f)
    , unk144(0.0f)
    , unk148(0xffffffff)
{
	unk148.r = 200;
	unk148.g = 200;
	unk148.b = 255;
	unk148.a = 200;
}

f32 TPoolManager::getWaterLevel(const TBGCheckData* param_1) const
{
	if (param_1 == nullptr || param_1->unk44 == nullptr)
		return param_1->mMinY + 100.0f;

	int idx = param_1->unk44->unk7C;
	if (idx == 0xff ? true : false)
		return 99999.0f;
	return ((const TPool*)getObj(idx))->getWaterLevel();
}

bool TPoolManager::subWaterLevel(const TBGCheckData* param_1)
{
	if (param_1 == nullptr || param_1->unk44 == nullptr)
		return true;

	int idx = param_1->unk44->unk7C;
	if (idx == 0xff ? true : false)
		return true;

	return ((TPool*)getObj(idx))->subWaterLevel();
}

TPoolManager::TPoolManager(const char* name)
    : TMapObjBaseManager(name)
{
	gpPoolManager = this;
}

static void dummy(f32* f) { *f = 0.0f; }
