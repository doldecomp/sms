#include <JSystem/J2D/J2DWindow.hpp>
#include <JSystem/JSupport/JSURandomInputStream.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <JSystem/JUtility/JUTPalette.hpp>
#include <JSystem/JUtility/JUTResource.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <dolphin/gx.h>

J2DWindow::J2DWindow(J2DPane*, JSURandomInputStream*, bool) { }

J2DWindow::~J2DWindow()
{
	if (unk100)
		delete unk100;
	if (unk104)
		delete unk104;
	if (unk108)
		delete unk108;
	if (unk10C)
		delete unk10C;
	if (unkFC)
		delete unkFC;
	if (unk110)
		delete unk110;
}

J2DWindow::Texture::~Texture() { }

void J2DWindow::draw_private(const JUTRect& param_1, const JUTRect& param_2,
                             Mtx* param_3)
{
	if (param_1.getWidth() >= unk130 && param_1.getHeight() >= unk134) {
		Mtx afStack_50;
		MTXConcat(*param_3, mGlobalMtx, afStack_50);
		GXLoadPosMtxImm(afStack_50, 0);
		drawContents(param_2);
		GXClearVtxDesc();
		GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
		GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
		GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
		GXSetNumTexGens(1);
		if (unk100 && unk104 && unk108 && unk10C) {
			int iVar6 = param_1.getWidth() - unk10C->getWidth();
			int iVar8 = param_1.getHeight() - unk10C->getHeight();
			u32 uVar7 = unk100->getWidth();
			u32 uVar5 = unk100->getHeight();
			unk100->draw(0, 0, !!(unk114 & 0x80), !!(unk114 & 0x40),
			             mColorAlpha, unk12C, unk128);
			unk104->draw(iVar6, 0, !!(unk114 & 0x20), !!(unk114 & 0x10),
			             mColorAlpha, unk12C, unk128);
			unk108->draw(0, iVar8, !!(unk114 & 8), !!(unk114 & 4), mColorAlpha,
			             unk12C, unk128);
			unk10C->draw(iVar6, iVar8, !!(unk114 & 2), !!(unk114 & 1),
			             mColorAlpha, unk12C, unk128);

			{
				u16 uVar2 = (unk114 & 0x20) ? 0x8000 : 0;
				u16 uVar3 = (unk114 & 0x10) ? 0 : 0x8000;
				unk104->draw(uVar7, 0, iVar6 - uVar7, unk104->getHeight(),
				             uVar2, uVar3, uVar2, uVar3 ^ 0x8000, mColorAlpha,
				             unk12C, unk128);
			}

			{
				u16 uVar2 = (unk114 & 0x2) ? 0x8000 : 0;
				u16 uVar3 = (unk114 & 0x1) ? 0 : 0x8000;
				unk10C->draw(uVar7, iVar8, iVar6 - uVar7, unk10C->getHeight(),
				             uVar2, uVar3, uVar2, uVar3 ^ 0x8000, mColorAlpha,
				             unk12C, unk128);
			}

			{
				u16 uVar2 = (unk114 & 0x8) ? 0 : 0x8000;
				u16 uVar3 = (unk114 & 0x4) ? 0x8000 : 0;
				unk108->draw(0, uVar5, unk108->getWidth(), iVar8 - uVar5, uVar2,
				             uVar3, uVar2 ^ 0x8000, uVar3, mColorAlpha, unk12C,
				             unk128);
			}

			{
				u16 uVar2 = (unk114 & 0x2) ? 0 : 0x8000;
				u16 uVar3 = (unk114 & 0x1) ? 0x8000 : 0;
				unk10C->draw(iVar6, uVar5, unk10C->getWidth(), iVar8 - uVar5,
				             uVar2, uVar3, uVar2 ^ 0x8000, uVar3, mColorAlpha,
				             unk12C, unk128);
			}
		}
		GXSetNumTexGens(0);
		GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
		GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
		              GX_COLOR0A0);
		GXSetVtxDesc(GX_VA_TEX0, GX_NONE);
	}
}

void J2DWindow::resize(int width, int height)
{
	int oldW = mBounds.getWidth();
	int oldH = mBounds.getHeight();
	mBounds.resize(width, height);
	int dw = width - oldW;
	int dh = height - oldH;
	unkEC.x2 += dw;
	unkEC.y2 += dh;

	for (JSUTreeIterator<J2DPane> iter = mPaneTree.getFirstChild();
	     iter != mPaneTree.getEndChild(); ++iter) {
		if (iter->mInfoTag == 0x13 && iter->mConnected) {
			int nw = dw + iter->getWidth();
			int nh = dh + iter->getHeight();
			iter->mBounds.resize(nw, nh);
		}
	}
}

void J2DWindow::drawSelf(int x, int y)
{
	Mtx id;
	MTXIdentity(id);
	drawSelf(x, y, &id);
}

void J2DWindow::drawSelf(int x, int y, Mtx* mtx)
{
	JUTRect tmp(mGlobalBounds.x1, mGlobalBounds.y1, mGlobalBounds.x2,
	            mGlobalBounds.y2);
	tmp.add(x, y);
	draw_private(tmp, unkEC, mtx);
	clip(unkEC);
}

void J2DWindow::drawContents(const JUTRect& rect)
{
	if (!rect.isEmpty()) {
		GXClearVtxDesc();
		GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
		GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
		GXSetNumChans(1);
		GXSetNumTexGens(0);
		GXSetNumTevStages(1);
		GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
		GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
		              GX_COLOR0A0);
		GXSetChanCtrl(GX_COLOR0A0, 0, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_NONE,
		              GX_AF_NONE);
		if ((unk118 & 0xff) == 0xff && (unk120 & 0xff) == 0xff
		    && (unk11C & 0xff) == 0xff && (unk124 & 0xff) == 0xff
		    && mColorAlpha == 0xff) {
			GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_SET);
		} else {
			GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA,
			               GX_LO_SET);
		}
		JUtility::TColor col1 = unk118;
		JUtility::TColor col2 = unk11C;
		JUtility::TColor col3 = unk120;
		JUtility::TColor col4 = unk124;
		if (mColorAlpha != 0xff) {
			col1.a = (u8)((col1.a * mColorAlpha) / 0xff);
			col2.a = (u8)((col2.a * mColorAlpha) / 0xff);
			col3.a = (u8)((col3.a * mColorAlpha) / 0xff);
			col4.a = (u8)((col4.a * mColorAlpha) / 0xff);
		}
		GXBegin(GX_QUADS, GX_VTXFMT0, 4);
		GXPosition3s16(rect.x1, rect.y1, 0);
		GXColor1u32(col1);
		GXPosition3s16(rect.x2, rect.y1, 0);
		GXColor1u32(col3);
		GXPosition3s16(rect.x2, rect.y2, 0);
		GXColor1u32(col4);
		GXPosition3s16(rect.x1, rect.y2, 0);
		GXColor1u32(col2);
		GXEnd();
		if (unk110) {
			GXClearVtxDesc();
			GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
			GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
			GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
			GXSetNumTexGens(1);
			unk110->drawContentsTexture(rect.x1, rect.y1, rect.x2 - rect.x1,
			                            rect.y2 - rect.y1, mColorAlpha);
		}
	}
}

void J2DWindow::Texture::draw(int x, int y, int width, int height, u16 param_5,
                              u16 param_6, u16 param_7, u16 param_8, u8 param_9,
                              JUtility::TColor param_10,
                              JUtility::TColor param_11)
{
	int x2 = x + width;
	int y2 = y + height;

	load(GX_TEXMAP0);
	setTevMode(param_9, param_10, param_11);
	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition3s16(x, y, 0);
	GXParam1s32(-1);
	GXTexCoord2u16(param_7, param_8);
	GXPosition3s16(x2, y, 0);
	GXParam1s32(-1);
	GXTexCoord2u16(param_5, param_8);
	GXPosition3s16(x2, y2, 0);
	GXParam1s32(-1);
	GXTexCoord2u16(param_5, param_6);
	GXPosition3s16(x, y2, 0);
	GXParam1s32(-1);
	GXTexCoord2u16(param_7, param_6);
	GXEnd();
}

void J2DWindow::Texture::draw(int x, int y, bool param_3, bool param_4,
                              u8 param_5, JUtility::TColor param_6,
                              JUtility::TColor param_7)
{
	draw(x, y, getWidth(), getHeight(), param_3 ? 0 : 0x8000,
	     param_4 ? 0 : 0x8000, param_3 ? 0x8000 : 0, param_4 ? 0x8000 : 0,
	     param_5, param_6, param_7);
}

void J2DWindow::Texture::drawContentsTexture(int x, int y, int width,
                                             int height, u8 param_5)
{
	int x2 = x + width;
	int y2 = y + height;

	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_F32, 0);

	f32 w  = (f32)getWidth();
	f32 h  = (f32)getHeight();
	f32 u1 = -((f32)width / w - 1.0f) * 0.5f;
	f32 v1 = -((f32)height / h - 1.0f) * 0.5f;
	f32 u2 = u1 + (f32)width / w;
	f32 v2 = v1 + (f32)height / h;

	load(GX_TEXMAP0);
	setTevMode(param_5, 0, 0xffffffff);

	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition3s16(x, y, 0);
	GXParam1s32(-1);
	GXTexCoord2f32(u1, v1);
	GXPosition3s16(x2, y, 0);
	GXParam1s32(-1);
	GXTexCoord2f32(u2, v1);
	GXPosition3s16(x2, y2, 0);
	GXParam1s32(-1);
	GXTexCoord2f32(u2, v2);
	GXPosition3s16(x, y2, 0);
	GXParam1s32(-1);
	GXTexCoord2f32(u1, v2);
	GXEnd();

	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_RGBX8, 15);
}

void J2DWindow::Texture::setTevMode(u8 param_1, JUtility::TColor param_2,
                                    JUtility::TColor param_3)
{
	GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c, 0, 0x7d);
	GXSetChanCtrl(GX_COLOR0A0, 0, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanMatColor(GX_COLOR0A0, (GXColor) { 0xff, 0xff, 0xff, param_1 });
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
	if (param_1 == 0xff && param_2 == 0 && param_3 == 0xffffffff) {
		int alpha = getTransparency();
		GXSetNumTevStages(1);
		if (field_0x2c != nullptr) {
			alpha = field_0x2c->getTransparency();
		}

		if (alpha == 0) {
			GXSetTevColor(GX_TEVREG2, JUtility::TColor());
			GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_TEXC, GX_CC_ZERO, GX_CC_ZERO,
			                GX_CC_ZERO);
			GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A2, GX_CA_ZERO, GX_CA_ZERO,
			                GX_CA_ZERO);
			GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
			                1, GX_TEVPREV);
			GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
			                1, GX_TEVPREV);
			GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_SET);
		} else {
			GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
			GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA,
			               GX_LO_SET);
		}
	} else {
		u8 stage = GX_TEVSTAGE1;
		GXSetTevColor(GX_TEVREG2, JUtility::TColor());
		GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_TEXC, GX_CC_ZERO, GX_CC_ZERO,
		                GX_CC_ZERO);

		if ((s32)mAlphaEnabled != 0)
			GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_TEXA, GX_CA_ZERO, GX_CA_ZERO,
			                GX_CA_ZERO);
		else
			GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A2, GX_CA_ZERO, GX_CA_ZERO,
			                GX_CA_ZERO);

		GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1,
		                GX_TEVPREV);
		GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1,
		                GX_TEVPREV);

		if (param_2 != 0 || param_3 != 0xffffffff) {
			GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
			              GX_COLOR_NULL);
			GXSetTevColor(GX_TEVREG0, param_2);
			GXSetTevColor(GX_TEVREG1, param_3);
			GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_C0, GX_CC_C1, GX_CC_CPREV,
			                GX_CC_ZERO);
			GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_A0, GX_CA_A1, GX_CA_APREV,
			                GX_CA_ZERO);
			GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
			                1, GX_TEVPREV);
			GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
			                1, GX_TEVPREV);
			stage = GX_TEVSTAGE2;
		}

		if (param_1 != 0xff) {
			GXSetTevOrder((GXTevStageID)stage, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
			              GX_COLOR0A0);
			GXSetTevColorIn((GXTevStageID)stage, GX_CC_CPREV, GX_CC_ZERO,
			                GX_CC_ZERO, GX_CC_ZERO);
			GXSetTevAlphaIn((GXTevStageID)stage, GX_CA_ZERO, GX_CA_APREV,
			                GX_CA_RASA, GX_CA_ZERO);
			GXSetTevColorOp((GXTevStageID)stage, GX_TEV_ADD, GX_TB_ZERO,
			                GX_CS_SCALE_1, 1, GX_TEVPREV);
			GXSetTevAlphaOp((GXTevStageID)stage, GX_TEV_ADD, GX_TB_ZERO,
			                GX_CS_SCALE_1, 1, GX_TEVPREV);
			stage++;
		}

		GXSetNumTevStages(stage);
		GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA,
		               GX_LO_SET);
	}
}
