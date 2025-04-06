#include <Map/MapDraw.hpp>
#include <Map/Map.hpp>
#include <Camera/Camera.hpp>
#include <System/Resolution.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <dolphin/gx.h>

// rouge include needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>

void TMapDrawWall::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (!(param_1 & 8))
		return;

	if (gpCamera->unk2C8 == -1) {
		if (unk18 > 0) {
			unk18 -= 5;
			if (unk18 < 0)
				unk18 = 0;
		}
	} else {
		if (unk14.a > unk18) {
			unk18 += 3;
		}
	}

	if (!unk18)
		return;

	u32 sVar1 = unk14.a - unk18;
	Mtx mtx;
	MTXIdentity(mtx);
	GXLoadPosMtxImm(mtx, 0);
	GXSetCurrentMtx(0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_S16, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_U16, 0);
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
	GXSetNumChans(1);
	GXSetChanCtrl(GX_COLOR0A0, 0, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanCtrl(GX_COLOR1A1, 0, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanMatColor(GX_COLOR0A0,
	                  (GXColor) { unk14.r, unk14.g, unk14.b, unk18 });
	GXSetNumTexGens(1);
	GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, 0, 0x7D);
	JUTTexture texture(unk10);
	texture.load(GX_TEXMAP0);
	GXSetNumTevStages(1);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
	GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_RASC, GX_CC_ZERO, GX_CC_ZERO,
	                GX_CC_ZERO);
	GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1,
	                GX_TEVPREV);
	GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_RASA, GX_CA_TEXA,
	                GX_CA_ZERO);
	GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1,
	                GX_TEVPREV);
	GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
	GXSetZCompLoc(0);
	GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
	GXSetZMode(0, GX_ALWAYS, 0);
	GXSetCullMode(GX_CULL_NONE);
	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition2s16(-sVar1, -sVar1);
	GXTexCoord2u16(0, 0);
	GXPosition2s16(sVar1 + SMSGetGameRenderWidth(), -sVar1);
	GXTexCoord2u16(1, 0);
	GXPosition2s16(sVar1 + SMSGetGameRenderWidth(),
	               sVar1 + SMSGetGameRenderHeight());
	GXTexCoord2u16(1, 1);
	GXPosition2s16(-sVar1, sVar1 + SMSGetGameRenderHeight());
	GXTexCoord2u16(0, 1);
	GXEnd();
}

void TMapDrawWall::load(JSUMemoryInputStream& stream)
{
	unk10 = (ResTIMG*)JKRGetResource("/common/timg/wall.bti");
}

TMapDrawWall::TMapDrawWall(const char* name)
    : JDrama::TViewObj(name)
    , unk10(nullptr)
    , unk14((GXColor) { 0x50, 0x50, 0xc8, 0x96 })
    , unk18(0)
{
}

void TMap::draw(unsigned long, JDrama::TGraphics*) const { }
