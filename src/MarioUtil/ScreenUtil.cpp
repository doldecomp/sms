#include <MarioUtil/ScreenUtil.hpp>
#include <System/Resolution.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTexture.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <dolphin/os/OSCache.h>
#include <dolphin/gx.h>

TAfterEffect* gpAfterEffect;
TScreenTexture* gpScreenTexture;

void TAfterEffect::load(JSUMemoryInputStream& stream)
{
	JDrama::TViewObj::load(stream);
	unk10 = nullptr;
	unk14 = 1;
	unk15 = 0;
	unk19 = 0;
	unk1A = 0;
	unk1B = unk19;
	unk1C = unk1A;
	unk20 = unk1B;
	unk24 = unk1C;
	unk16 = 0xff;
	unk17 = 0xff;
	unk18 = 0xff;
	unk28 = 0.0f;
	unk2C = 0.0f;
	unk30 = 1.0f;
	unk34 = 1.0f;
	unk38 = unk28;
	unk3C = unk2C;
	unk40 = unk30;
	unk44 = unk34;
	unk48 = 0.05f;
	unk50 = 0.0f;
	unk54 = 0.01f;
}

void TAfterEffect::loadAfter()
{
	TScreenTexture* tex
	    = JDrama::TNameRefGen::search<TScreenTexture>("スクリーンテクスチャ");
	unk10         = tex->getTexture();
	gpAfterEffect = this;
}

void TAfterEffect::setBlurDefaultValue()
{
	unk14 &= ~2;
	unk1B = unk19;
	unk1C = unk1A;
	unk16 = 0xff;
	unk17 = 0xff;
	unk18 = 0xff;
	unk28 = 0.0f;
	unk2C = 0.0f;
	unk30 = 1.0f;
	unk34 = 1.0f;
}

void TAfterEffect::calcDashBlurValue()
{
	if (unk50 > 0.0f) {
		unk28 = unk5C * unk50 * 0.5f;
		unk2C = -unk60 * unk50 * 0.5f;
		unk30 = unk64 * unk50 + 1.0f;
		unk34 = unk64 * unk50 + 1.0f;
		unk50 = unk50 - unk54;
		unk58 = unk59;
		unk14 |= 4;
		return;
	}

	unk50 = 0.0f;
	if (unk58 != 0) {
		unk58 -= 1;
		unk28 = 0.0f;
		unk2C = 0.0f;
		unk30 = 1.0f;
		unk34 = 1.0f;
		unk14 |= 4;
		return;
	}

	unk14 &= ~4;
	unk15 = 0;
}

void TAfterEffect::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (!(unk14 & 1))
		return;

	if (!(param_1 & 0x10))
		return;

	switch (unk15) {
	case 0:
		setBlurDefaultValue();
		break;
	case 2:
		calcDashBlurValue();
		unk14 &= ~2;
		break;
	case 3:
		break;
	}

	GXColor color;
	color.r = 0;
	color.g = 0;
	color.b = 0;
	color.a = 0;

	JDrama::TRect& rect = param_2->getUnk54();

	unk20 += unk48 * (unk1B - unk20);
	unk24 += unk48 * (unk1C - unk24);
	unk38 += unk48 * (unk28 - unk38);
	unk3C += unk48 * (unk2C - unk3C);
	unk40 += unk48 * (unk30 - unk40);
	unk44 += unk48 * (unk34 - unk44);

	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
	GXSetCullMode(GX_CULL_BACK);
	GXSetCurrentMtx(0);
	GXLoadPosMtxImm(param_2->getUnkB4(), 0);
	GXSetNumChans(1);
	GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	unk10->load(GX_TEXMAP0);
	GXSetNumTexGens(1);
	GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c, 0, 0x7d);
	GXSetTevColor(GX_TEVREG0, color);
	GXSetNumTevStages(1);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
	GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_RASC, GX_CC_TEXC, GX_CC_C0);
	GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1,
	                GX_TEVPREV);
	GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_RASA, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
	GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1,
	                GX_TEVPREV);
	GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
	GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
	GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);

	f32 fVar4  = unk40 * -0.5f + 0.5f + unk38;
	f32 fVar5  = unk40 * 0.5f + 0.5f + unk38;
	f32 fVar6  = unk44 * -0.5f + 0.5f + unk3C;
	f32 fVar7  = unk44 * 0.5f + 0.5f + unk3C;
	f32 fVar3  = (fVar4 + fVar5) * 0.5f;
	f32 fVar10 = (fVar6 + fVar7) * 0.5f;

	u16 uVar14 = -param_2->getUnkE8();

	GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, 8);

	GXPosition3s16((rect.x1 + rect.x2) / 2, (rect.y1 + rect.y2) / 2, uVar14);
	if (checkFlag(4))
		GXColor4u8(unk16, unk17, unk18, unk24);
	else
		GXColor4u8(unk16, unk17, unk18, unk20);
	GXTexCoord2f32(fVar3, fVar10);

	GXPosition3s16(rect.x2, rect.y1, uVar14);
	GXColor4u8(unk16, unk17, unk18, unk24);
	GXTexCoord2f32(fVar5, fVar6);

	GXPosition3s16(rect.x2, rect.y2, uVar14);
	GXColor4u8(unk16, unk17, unk18, unk24);
	GXTexCoord2f32(fVar5, fVar7);

	GXPosition3s16((rect.x1 + rect.x2) / 2, rect.y2, uVar14);
	if (checkFlag(4))
		GXColor4u8(unk16, unk17, unk18, unk24);
	else
		GXColor4u8(unk16, unk17, unk18, unk20);
	GXTexCoord2f32(fVar3, fVar7);

	GXPosition3s16(rect.x1, rect.y2, uVar14);
	GXColor4u8(unk16, unk17, unk18, unk24);
	GXTexCoord2f32(fVar4, fVar7);

	GXPosition3s16(rect.x1, rect.y1, uVar14);
	GXColor4u8(unk16, unk17, unk18, unk24);
	GXTexCoord2f32(fVar4, fVar6);

	GXPosition3s16((rect.x1 + rect.x2) / 2, rect.y1, uVar14);
	if (checkFlag(4))
		GXColor4u8(unk16, unk17, unk18, unk24);
	else
		GXColor4u8(unk16, unk17, unk18, unk20);
	GXTexCoord2f32(fVar3, fVar6);

	GXPosition3s16(rect.x2, rect.y1, uVar14);
	GXColor4u8(unk16, unk17, unk18, unk24);
	GXTexCoord2f32(fVar5, fVar6);

	GXEnd();

	unk14 &= ~4;
}

void TScreenTexture::load(JSUMemoryInputStream& stream)
{
	JDrama::TViewObj::load(stream);
	unk10           = new JUTTexture((u16)SMSGetGameRenderWidth() / 2,
	                                 (u16)SMSGetGameRenderHeight() / 2, GX_TF_RGB565);
	gpScreenTexture = this;
}

bool TScreenTexture::replace(J3DModelData* param_1, const char* param_2)
{
	bool replaced = false;
	for (u16 i = 0; i < param_1->getTexture()->getNum(); ++i) {
		if (strcmp(param_1->getTextureName()->getName(i), param_2) == 0) {
			param_1->getTexture()->setResTIMG(i, *unk10->getTexInfo());
			DCFlushRange(param_1->getTexture()->getResTIMG(i), sizeof(ResTIMG));
			replaced = true;
		}
	}
	return replaced;
}

void SMS_FillScreenAlpha(u8 param_1)
{
	Mtx afStack_3c;
	MTXIdentity(afStack_3c);
	GXLoadPosMtxImm(afStack_3c, 0);
	GXSetNumChans(1);
	GXSetChanCtrl(GX_COLOR0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanCtrl(GX_ALPHA0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetNumTexGens(0);
	GXSetNumTevStages(1);
	GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
	GXSetColorUpdate(GX_FALSE);
	GXSetAlphaUpdate(GX_TRUE);
	GXSetDstAlpha(GX_TRUE, 0);
	GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_LO_NOOP);
	GXSetCullMode(GX_CULL_BACK);
	GXSetDstAlpha(GX_TRUE, param_1);
	GXSetZMode(GX_TRUE, GX_ALWAYS, GX_FALSE);
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);

	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition3f32(-1000.0f, 1000.0f, -200.0f);
	GXPosition3f32(1000.0f, 1000.0f, -200.0f);
	GXPosition3f32(1000.0f, -1000.0f, -200.0f);
	GXPosition3f32(-1000.0f, -1000.0f, -200.0f);
	GXEnd();

	GXSetColorUpdate(GX_TRUE);
	GXSetAlphaUpdate(GX_FALSE);
	GXSetDstAlpha(GX_FALSE, 0);
}
