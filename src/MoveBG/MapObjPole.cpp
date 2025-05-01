#include <MoveBG/MapObjPole.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <dolphin/gx.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

TMapObjPoleManager* gpMapObjPoleManager;

void TMapObjPole::draw()
{
	f32 fVar7 = mPosition.x + unk68;
	f32 fVar2 = mPosition.x - unk68;
	f32 fVar8 = mPosition.z + unk68;
	f32 fVar4 = mPosition.z - unk68;

	f32 fVar1 = mPosition.y;
	f32 fVar5 = mScaling.y * 100.0f + mPosition.y;

	f32 fVar3 = unk6C * (fVar5 - mPosition.y);
	f32 fVar6 = unk6C * (fVar1 - mPosition.y);

	GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 8);
	GXPosition3f32(fVar2, fVar5, fVar4);
	GXTexCoord2f32(0.0f, fVar3);
	GXPosition3f32(fVar2, fVar1, fVar4);
	GXTexCoord2f32(0.0f, fVar6);
	GXPosition3f32(fVar7, fVar5, fVar8);
	GXTexCoord2f32(1.0f, fVar3);
	GXPosition3f32(fVar7, fVar1, fVar8);
	GXTexCoord2f32(1.0f, fVar6);
	GXPosition3f32(fVar7, fVar5, fVar4);
	GXTexCoord2f32(0.0f, fVar3);
	GXPosition3f32(fVar7, fVar1, fVar4);
	GXTexCoord2f32(0.0f, fVar6);
	GXPosition3f32(fVar2, fVar5, fVar8);
	GXTexCoord2f32(1.0f, fVar3);
	GXPosition3f32(fVar2, fVar1, fVar8);
	GXTexCoord2f32(1.0f, fVar6);
	GXEnd();
}

static void dummy(f32* f) { *f = 0.0f; }

void TMapObjPole::load(JSUMemoryInputStream&) { }

TMapObjPole::TMapObjPole(const char* name)
    : THitActor(name)
{
}

void TMapObjPoleManager::initDraw()
{
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
	GXSetCurrentMtx(0);
	GXLoadPosMtxImm(j3dSys.getViewMtx(), 0);
	GXSetNumChans(0);
	GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanMatColor(GX_COLOR0A0, (GXColor) { 0xff, 0xff, 0xff, 0xff });
	GXSetNumTexGens(1);
	GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c, 0, 0x7d);
	GXSetNumTevStages(1);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
	GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_TEXC, GX_CC_ZERO, GX_CC_ZERO,
	                GX_CC_ZERO);
	GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_TRUE, GX_TEVPREV);
	GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_TEXA, GX_CA_ZERO, GX_CA_ZERO,
	                GX_CA_ZERO);
	GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_TRUE, GX_TEVPREV);
	GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ZERO, GX_LO_NOOP);
	GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_AND, GX_GREATER, 0);
	GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
	GXSetCullMode(GX_CULL_NONE);
}

void TMapObjPoleManager::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if ((param_1 & 8)) {
		initDraw();
		if (unk10[0].unk0 != 0) {
			JUTTexture texture(unk10[0].unk54);
			texture.load(GX_TEXMAP0);

			for (int i = 0; i < unk10[0].unk0; ++i)
				unk10[0].unk4[i]->draw();
		}
	}
}

void TMapObjPoleManager::loadPole(TMapObjPoleManager::TMapObjPoleInfo*,
                                  TMapObjPole*, const char*)
{
}

void TMapObjPoleManager::registerObj(TMapObjPole*) { }

void TMapObjPoleManager::load(JSUMemoryInputStream& stream)
{
	JDrama::TViewObj::load(stream);
	char buffer[32];
	stream.readString(buffer, 32);
}

TMapObjPoleManager::TMapObjPoleManager(const char* name)
    : JDrama::TViewObj(name)
{
	gpMapObjPoleManager = this;
}
