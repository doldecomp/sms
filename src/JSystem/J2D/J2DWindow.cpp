#include <JSystem/J2D/J2DWindow.hpp>
#include <JSystem/JSupport/JSURandomInputStream.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <JSystem/JUtility/JUTPalette.hpp>
#include <JSystem/JUtility/JUTResource.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <dolphin/gx.h>

// NOTE: for .sdata ordering
static void dummy(float* f) { *f = 1.0f; }

J2DWindow::J2DWindow(J2DPane* parent, JSURandomInputStream* stream, bool is_ex)
    : J2DPane(parent, stream, is_ex)
    , unkFC(nullptr)
    , unk100(nullptr)
    , unk104(nullptr)
    , unk108(nullptr)
    , unk10C(nullptr)
{
	JUTResReference res;
	mInfoTag = 0x11;
	if (is_ex) {
		u8 fields = stream->readU8();

		unkEC.x1 = stream->readU16();
		unkEC.y1 = stream->readU16();
		unkEC.x2 = unkEC.x1 + stream->readU16();
		unkEC.y2 = unkEC.y1 + stream->readU16();

		if (ResTIMG* timg = (ResTIMG*)res.getResource(stream, 'TIMG', nullptr))
			unk100 = new Texture(timg);
		if (ResTIMG* timg = (ResTIMG*)res.getResource(stream, 'TIMG', nullptr))
			unk104 = new Texture(timg);
		if (ResTIMG* timg = (ResTIMG*)res.getResource(stream, 'TIMG', nullptr))
			unk108 = new Texture(timg);
		if (ResTIMG* timg = (ResTIMG*)res.getResource(stream, 'TIMG', nullptr))
			unk10C = new Texture(timg);
		if (ResTLUT* tlut = (ResTLUT*)res.getResource(stream, 'TLUT', nullptr))
			unkFC = new JUTPalette(GX_TLUT0, tlut);

		unk114 = stream->readU8();
		unk118.set(stream->readU32());
		unk120.set(stream->readU32());
		unk11C.set(stream->readU32());
		unk124.set(stream->readU32());
		fields -= 14;

		unk110 = nullptr;
		if (fields) {
			if (ResTIMG* timg
			    = (ResTIMG*)res.getResource(stream, 'TIMG', nullptr))
				unk110 = new Texture(timg);
			fields--;
		}
		unk12C = 0;
		unk128 = 0xffffffff;
		if (fields) {
			unk12C = stream->readU32();
			fields--;
		}
		if (fields) {
			unk128 = stream->readU32();
		}
		stream->align(4);
	} else {
		unkEC.x1 = stream->readU16();
		unkEC.y1 = stream->readU16();
		unkEC.x2 = unkEC.x1 + stream->readU16();
		unkEC.y2 = unkEC.y1 + stream->readU16();

		if (ResTIMG* timg = (ResTIMG*)res.getResource(stream, 'TIMG', nullptr))
			unk100 = new Texture(timg);
		if (ResTIMG* timg = (ResTIMG*)res.getResource(stream, 'TIMG', nullptr))
			unk104 = new Texture(timg);
		if (ResTIMG* timg = (ResTIMG*)res.getResource(stream, 'TIMG', nullptr))
			unk108 = new Texture(timg);
		if (ResTIMG* timg = (ResTIMG*)res.getResource(stream, 'TIMG', nullptr))
			unk10C = new Texture(timg);
		if (ResTLUT* tlut = (ResTLUT*)res.getResource(stream, 'TLUT', nullptr))
			unkFC = new JUTPalette(GX_TLUT0, tlut);

		unk114 = stream->readU8();
		unk118.set(stream->readU32());
		unk120.set(stream->readU32());
		unk11C.set(stream->readU32());
		unk124.set(stream->readU32());

		stream->align(4);
		unk110 = nullptr;
		unk12C = 0x0;
		unk128 = 0xffffffff;
	}
	if (unk100 && unk104 && unk108 && unk10C) {
		unk130 = unk100->getWidth() + unk104->getWidth();
		unk134 = unk100->getHeight() + unk108->getHeight();
	} else {
		unk130 = 1;
		unk134 = 1;
	}
}

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
			int iVar6        = param_1.getWidth() - unk10C->getWidth();
			int iVar8        = param_1.getHeight() - unk10C->getHeight();
			u32 unk100Width  = unk100->getWidth();
			u32 unk100Height = unk100->getHeight();
			unk100->draw(0, 0, !!(unk114 & 0x80), !!(unk114 & 0x40),
			             mColorAlpha, unk12C, unk128);
			unk104->draw(iVar6, 0, !!(unk114 & 0x20), !!(unk114 & 0x10),
			             mColorAlpha, unk12C, unk128);
			unk108->draw(0, iVar8, !!(unk114 & 8), !!(unk114 & 4), mColorAlpha,
			             unk12C, unk128);
			unk10C->draw(iVar6, iVar8, !!(unk114 & 2), !!(unk114 & 1),
			             mColorAlpha, unk12C, unk128);

			u16 a, b, c, d;

			b = a = (unk114 & 0x20) ? (u16)0x8000 : (u16)0;
			c     = (unk114 & 0x10) ? (u16)0 : (u16)0x8000;
			d     = c ^ 0x8000;
			unk104->draw(unk100Width, 0, iVar6 - unk100Width,
			             unk104->getHeight(), b, c, a, d, mColorAlpha, unk12C,
			             unk128);

			d = a = (unk114 & 0x2) ? (u16)0x8000 : (u16)0;
			b     = (unk114 & 0x1) ? (u16)0 : (u16)0x8000;
			c     = b ^ 0x8000;
			unk10C->draw(unk100Width, iVar8, iVar6 - unk100Width,
			             unk10C->getHeight(), d, b, a, c, mColorAlpha, unk12C,
			             unk128);

			a = (unk114 & 0x8) ? (u16)0 : (u16)0x8000;
			b = a ^ 0x8000;
			d = c = (unk114 & 0x4) ? (u16)0x8000 : (u16)0;
			unk108->draw(0, unk100Height, unk108->getWidth(),
			             iVar8 - unk100Height, a, d, b, c, mColorAlpha, unk12C,
			             unk128);

			a = (unk114 & 0x2) ? (u16)0 : (u16)0x8000;
			b = a ^ 0x8000;
			d = c = (unk114 & 0x1) ? (u16)0x8000 : (u16)0;
			unk10C->draw(iVar6, unk100Height, unk10C->getWidth(),
			             iVar8 - unk100Height, a, d, b, c, mColorAlpha, unk12C,
			             unk128);
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
