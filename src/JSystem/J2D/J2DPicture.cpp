#include <JSystem/J2D/J2DPicture.hpp>
#include <JSystem/JUtility/JUTPalette.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <JSystem/JUtility/JUTResource.hpp>
#include <JSystem/JSupport/JSURandomInputStream.hpp>
#include <dolphin/gx.h>

#pragma opt_strength_reduction off

J2DPicture::J2DPicture(J2DPane* parent, JSURandomInputStream* stream,
                       bool is_ex)
    : J2DPane(parent, stream, is_ex)
    , mPalette(nullptr)
{
	JUTResReference res;
	mInfoTag = 0x12;

	ResTIMG* timg;
	ResTLUT* tlut;

	if (is_ex) {
		u8 fields = stream->readU8();

		timg = (ResTIMG*)res.getResource(stream, 'TIMG', nullptr);
		tlut = (ResTLUT*)res.getResource(stream, 'TLUT', nullptr);

		mBinding = (J2DBinding)stream->readU8();
		fields -= 3;

		if (fields != 0) {
			u8 mirror = stream->readU8();
			mMirror   = (J2DMirror)(mirror & 3);
			unk130    = mirror & 4;
			fields -= 1;
		} else {
			mMirror = MIRROR0;
			unk130  = false;
		}
		if (fields != 0) {
			u8 wrapMode  = stream->readU8();
			mWrapmodeHor = (J2DWrapmode)(wrapMode >> 2 & 3);
			mWrapmodeVer = (J2DWrapmode)(wrapMode & 3);
			fields -= 1;
		} else {
			mWrapmodeHor = J2DWrapmode_NONE;
			mWrapmodeVer = J2DWrapmode_NONE;
		}

		mBlack = 0x0;
		mWhite = 0xffffffff;

		if (fields != 0) {
			mBlack = stream->readU32();
			fields -= 1;
		}
		if (fields != 0) {
			mWhite = stream->readU32();
			fields -= 1;
		}

		setCornerColor(0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff);

		for (int iVar3 = 0; fields != 0 && iVar3 < 4; ++iVar3) {
			mCornerColor[iVar3] = stream->readU32();
			fields -= 1;
		}

		stream->align(4);
	} else {
		timg = (ResTIMG*)res.getResource(stream, 'TIMG', nullptr);
		tlut = (ResTLUT*)res.getResource(stream, 'TLUT', nullptr);

		mBinding     = (J2DBinding)stream->readU8();
		u8 mode      = stream->readU8();
		mMirror      = (J2DMirror)(mode & 3);
		unk130       = mode & 4;
		mWrapmodeHor = (J2DWrapmode)(mode >> 3 & 3);
		mWrapmodeVer = mWrapmodeHor;
		stream->align(4);

		mBlack = 0x0;
		mWhite = 0xffffffff;

		setCornerColor(0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff);
	}

	mTextures[0] = nullptr;
	mTextureNum  = 0;
	unkFD[0]     = 1;

	if (timg != nullptr) {
		mTextures[0] = new JUTTexture(timg);
		++mTextureNum;
	}

	if (tlut != nullptr) {
		mPalette = new JUTPalette(GX_TLUT0, tlut);
		mTextures[0]->attachPalette(mPalette);
	}

	unk104[0] = 1.0;
	unk104[1] = 1.0;
	unk104[2] = 1.0;
	unk104[3] = 1.0;
	setBlendKonstColor();

	unk114[0] = 1.0;
	unk114[1] = 1.0;
	unk114[2] = 1.0;
	unk114[3] = 1.0;
	setBlendKonstAlpha();
}

J2DPicture::~J2DPicture()
{
	for (int i = 0; i < mTextureNum; ++i)
		if (mTextures[i] && unkFD[i])
			delete mTextures[i];

	if (mPalette)
		delete mPalette;
}

bool J2DPicture::insert(JUTTexture* tex, u8 idx, float alpha)
{
	if (tex == nullptr || mTextureNum >= 4 || idx >= 4 || idx > mTextureNum)
		return false;

	for (u8 i = 3; idx < i; --i) {
		mTextures[i] = mTextures[i - 1];
		unk104[i]    = unk104[i - 1];
		unk114[i]    = unk104[i + 3];
		unkFD[i]     = unkFD[i - 1];
	}
	mTextures[idx] = tex;
	unkFD[idx]     = 0;
	unk104[idx]    = alpha;
	unk114[idx]    = alpha;
	if (mTextureNum == 0) {
		JUTRect newBounds(0, 0, mTextures[0]->getWidth(),
		                  mTextures[0]->getHeight());
		mBounds = newBounds;
	}
	++mTextureNum;
	setBlendKonstColor();
	setBlendKonstAlpha();
	return true;
}

bool J2DPicture::remove(u8 idx)
{
	if (mTextureNum <= idx || mTextureNum == 1)
		return false;

	if (unkFD[idx])
		delete mTextures[idx];

	for (u8 i = idx; i < mTextureNum - 1; ++i) {
		mTextures[i] = mTextures[i + 1];
		unk104[i]    = unk104[i + 1];
		unk114[i]    = unk114[i + 1];
		unkFD[i]     = unkFD[i + 1];
	}
	--mTextureNum;
	setBlendKonstColor();
	setBlendKonstAlpha();
	return true;
}

const ResTIMG* J2DPicture::changeTexture(const ResTIMG* timg, u8 idx)
{
	if (idx >= mTextureNum) {
		return nullptr;
	}

	if (getTexture(idx) == nullptr || timg == nullptr) {
		return nullptr;
	}

	const ResTIMG* pRVar3 = getTexture(idx)->getTexInfo();

	getTexture(idx)->storeTIMG(timg);

	return pRVar3;
}

void J2DPicture::drawSelf(int x, int y)
{
	Mtx id;
	MTXIdentity(id);
	drawSelf(x, y, &id);
}

void J2DPicture::drawSelf(int x, int y, Mtx* mtx)
{
	char trash[0x4]; // probably getter for mGlobalBounds or something?
	if (!mTextures[0])
		return;

	drawFullSet(mGlobalBounds.x1 + x, mGlobalBounds.y1 + y, mBounds.getWidth(),
	            mBounds.getHeight(), mBinding, mMirror, unk130, mWrapmodeHor,
	            mWrapmodeVer, mtx);
}

void J2DPicture::drawFullSet(int x, int y, int w, int h, J2DBinding binding,
                             J2DMirror mirror, bool flip, J2DWrapmode wrap_hor,
                             J2DWrapmode wrap_vert, Mtx* mtx)
{
	int renderX = x;
	int renderY = y;
	if (wrap_hor == 0) {
		float newWidth = !flip ? (float)mTextures[0]->getWidth()
		                       : (float)mTextures[0]->getHeight();
		if (binding & 8) {
			if (!(binding & 4) && newWidth < (float)w) {
				w = (int)newWidth;
			}
		} else if (newWidth < (float)w) {
			renderX = (binding & 4) ? (x + w) - (int)newWidth
			                        : x + (w - (int)newWidth) / 2;
			w       = (int)newWidth;
		}
	}
	if (wrap_vert == 0) {
		float newHeight = !flip ? (float)mTextures[0]->getHeight()
		                        : (float)mTextures[0]->getWidth();

		if (binding & 2) {
			if (!(binding & 1) && newHeight < (float)h) {
				h = (int)newHeight;
			}
		} else if (newHeight < (float)h) {
			renderY = (binding & 1) ? (y + h) - (int)newHeight
			                        : y + (h - (int)newHeight) / 2;
			h       = (int)newHeight;
		}
	}

	bool boolR0;
	bool boolR3;
	bool boolR4;
	bool boolR5;
	if (!flip) {
		boolR0 = (mirror & 2) ? !!(binding & 4) : !!(binding & 8);
		boolR3 = (mirror & 2) ? !!(binding & 8) : !!(binding & 4);
		boolR4 = (mirror & 1) ? !!(binding & 1) : !!(binding & 2);
		boolR5 = (mirror & 1) ? !!(binding & 2) : !!(binding & 1);
	} else {
		boolR0 = (mirror & 2) ? !!(binding & 1) : !!(binding & 2);
		boolR3 = (mirror & 2) ? !!(binding & 2) : !!(binding & 1);
		boolR4 = (mirror & 1) ? !!(binding & 8) : !!(binding & 4);
		boolR5 = (mirror & 1) ? !!(binding & 4) : !!(binding & 8);
	}

	float fullWidth  = (float)mTextures[0]->getWidth();
	float fullHeight = (float)mTextures[0]->getHeight();
	int renderWidth  = !flip ? w : h;
	int renderHeight = !flip ? h : w;

	float v2;
	float u2;
	float v1;
	float u1;
	if (boolR0) {
		u1 = 0.0f;
		u2 = boolR3 ? 1.0f : (float)renderWidth / fullWidth;
	} else if (boolR3) {
		u1 = 1.0f - (float)renderWidth / fullWidth;
		u2 = 1.0f;
	} else {
		u1 = 0.5f - ((float)renderWidth / fullWidth) / 2.0f;
		u2 = 0.5f + ((float)renderWidth / fullWidth) / 2.0f;
	}

	if (boolR4) {
		v1 = 0.0f;
		v2 = boolR5 ? 1.0f : (float)renderHeight / fullHeight;
	} else if (boolR5) {
		v1 = 1.0f - (float)renderHeight / fullHeight;
		v2 = 1.0f;
	} else {
		v1 = 0.5f - ((float)renderHeight / fullHeight) / 2.0f;
		v2 = 0.5f + ((float)renderHeight / fullHeight) / 2.0f;
	}

	if (mirror & 2) {
		swap(u1, u2);
	}
	if (mirror & 1) {
		swap(v1, v2);
	}

	if (!flip) {
		drawTexCoord(renderX - x, renderY - y, w, h, u1, v1, u2, v1, u1, v2, u2,
		             v2, mtx);
	} else {
		drawTexCoord(renderX - x, renderY - y, w, h, u1, v2, u1, v1, u2, v2, u2,
		             v1, mtx);
	}
}

void J2DPicture::draw(int x, int y, int param_3, int param_4, bool param_5,
                      bool param_6, bool param_7)
{
	char trash[0x8];

	if (!mVisible)
		return;

	for (u8 i = 0; i < mTextureNum; ++i) {
		GXTexMapID idx = (GXTexMapID)i;
		if (i < mTextureNum)
			mTextures[idx]->load(idx);
	}

	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);

	GXSetNumTexGens(mTextureNum);

	makeMatrix(x, y);

	GXLoadPosMtxImm(mPositionMtx, 0);
	GXSetCurrentMtx(0);
	mColorAlpha = mAlpha;

	setTevMode();

	GXBegin(GX_QUADS, GX_VTXFMT0, 4);

	GXPosition3s16(0, 0, 0);
	GXColor1u32(mCornerColor[0]);
	if (!param_7) {
		GXTexCoord1s16(param_5 ? (u16)0x8000 : (u16)0x0000);
		GXTexCoord1s16(param_6 ? (u16)0x8000 : (u16)0x0000);
	} else {
		GXTexCoord1s16(param_5 ? (u16)0x8000 : (u16)0x0000);
		GXTexCoord1s16(param_6 ? (u16)0x0000 : (u16)0x8000);
	}

	GXPosition3s16(param_3, 0, 0);
	GXColor1u32(mCornerColor[1]);
	if (!param_7) {
		GXTexCoord1s16(param_5 ? (u16)0x0000 : (u16)0x8000);
		GXTexCoord1s16(param_6 ? (u16)0x8000 : (u16)0x0000);
	} else {
		GXTexCoord1s16(param_5 ? (u16)0x8000 : (u16)0x0000);
		GXTexCoord1s16(param_6 ? (u16)0x8000 : (u16)0x0000);
	}

	GXPosition3s16(param_3, param_4, 0);
	GXColor1u32(mCornerColor[3]);
	if (!param_7) {
		GXTexCoord1s16(param_5 ? (u16)0x0000 : (u16)0x8000);
		GXTexCoord1s16(param_6 ? (u16)0x0000 : (u16)0x8000);
	} else {
		GXTexCoord1s16(param_5 ? (u16)0x0000 : (u16)0x8000);
		GXTexCoord1s16(param_6 ? (u16)0x8000 : (u16)0x0000);
	}

	GXPosition3s16(0, param_4, 0);
	GXColor1u32(mCornerColor[2]);
	if (!param_7) {
		GXTexCoord1s16(param_5 ? (u16)0x8000 : (u16)0x0000);
		GXTexCoord1s16(param_6 ? (u16)0x0000 : (u16)0x8000);
	} else {
		GXTexCoord1s16(param_5 ? (u16)0x0000 : (u16)0x8000);
		GXTexCoord1s16(param_6 ? (u16)0x0000 : (u16)0x8000);
	}

	GXEnd();

	Mtx id;
	MTXIdentity(id);
	GXLoadPosMtxImm(id, 0);
	GXSetNumTexGens(0);
	GXSetNumTevStages(1);
	GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
	GXSetChanCtrl(GX_COLOR0A0, 0, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetVtxDesc(GX_VA_TEX0, GX_NONE);
}

void J2DPicture::drawTexCoord(int x, int y, int w, int h, float u1, float v1,
                              float u2, float v2, float u4, float v4, float u3,
                              float v3, Mtx* mtx)
{
	for (u8 i = 0; i < mTextureNum; ++i) {
		GXTexMapID idx = (GXTexMapID)i;
		if (i < mTextureNum)
			mTextures[idx]->load(idx);
	}
	int x2 = x + w;
	int y2 = y + h;
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_F32, 0);
	GXSetNumTexGens(mTextureNum);
	Mtx transform;
	MTXConcat(*mtx, mGlobalMtx, transform);
	GXLoadPosMtxImm(transform, 0);
	setTevMode();

	GXBegin(GX_QUADS, GX_VTXFMT0, 4);

	GXPosition3s16(x, y, 0);
	GXColor1u32(mCornerColor[0]);
	GXTexCoord2f32(u1, v1);

	GXPosition3s16(x2, y, 0);
	GXColor1u32(mCornerColor[1]);
	GXTexCoord2f32(u2, v2);

	GXPosition3s16(x2, y2, 0);
	GXColor1u32(mCornerColor[3]);
	GXTexCoord2f32(u3, v3);

	GXPosition3s16(x, y2, 0);
	GXColor1u32(mCornerColor[2]);
	GXTexCoord2f32(u4, v4);

	GXEnd();

	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_RGBX8, 15);
}

void J2DPicture::setTevMode()
{
	u8 i;
	for (i = 0; i < mTextureNum; ++i) {
		GXSetTexCoordGen2((GXTexCoordID)i, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c, 0,
		                  0x7d);
	}
	GXSetNumChans(1);
	GXSetChanCtrl(GX_COLOR0A0, 1, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_NONE,
	              GX_AF_NONE);

	GXSetChanAmbColor(GX_COLOR0A0, (GXColor) { 0xff, 0xff, 0xff, mColorAlpha });

	for (i = 0; i < mTextureNum; ++i) {
		GXSetTevOrder((GXTevStageID)i, (GXTexCoordID)i, (GXTexMapID)i,
		              GX_COLOR_NULL);
	}

	GXSetTevColor(GX_TEVREG2, JUtility::TColor(0xffffffff));
	GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_TEXC, GX_CC_ZERO, GX_CC_ZERO,
	                GX_CC_ZERO);

	if (mTextures[0]->getTransparency()) {
		GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_TEXA, GX_CA_ZERO, GX_CA_ZERO,
		                GX_CA_ZERO);
	} else {
		GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A2, GX_CA_ZERO, GX_CA_ZERO,
		                GX_CA_ZERO);
	}
	GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1,
	                GX_TEVPREV);
	GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1,
	                GX_TEVPREV);
	GXSetTevKColor(GX_KCOLOR0, mBlendKonstColor);
	GXSetTevKColor(GX_KCOLOR2, mBlendKonstAlpha);

	for (i = 1; i < mTextureNum; ++i) {
		GXSetTevKColorSel((GXTevStageID)i, (GXTevKColorSel)(0x20 - (i << 2)));
		GXSetTevKAlphaSel((GXTevStageID)i, (GXTevKAlphaSel)(0x22 - (i << 2)));

		GXSetTevColorIn((GXTevStageID)i, GX_CC_CPREV, GX_CC_TEXC, GX_CC_KONST,
		                GX_CC_ZERO);

		if (mTextures[i]->getTransparency()) {
			GXSetTevAlphaIn((GXTevStageID)i, GX_CA_APREV, GX_CA_TEXA,
			                GX_CA_KONST, GX_CA_ZERO);
		} else {
			GXSetTevAlphaIn((GXTevStageID)i, GX_CA_APREV, GX_CA_A2, GX_CA_KONST,
			                GX_CA_ZERO);
		}
		GXSetTevColorOp((GXTevStageID)i, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
		                1, GX_TEVPREV);
		GXSetTevAlphaOp((GXTevStageID)i, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
		                1, GX_TEVPREV);
	}

	if (mBlack != 0x0 || mWhite != 0xffffffff) {
		GXSetTevOrder((GXTevStageID)i, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
		              GX_COLOR_NULL);
		GXSetTevColor(GX_TEVREG0, mBlack);
		GXSetTevColor(GX_TEVREG1, mWhite);
		GXSetTevColorIn((GXTevStageID)i, GX_CC_C0, GX_CC_C1, GX_CC_CPREV,
		                GX_CC_ZERO);
		GXSetTevAlphaIn((GXTevStageID)i, GX_CA_A0, GX_CA_A1, GX_CA_APREV,
		                GX_CA_ZERO);
		GXSetTevColorOp((GXTevStageID)i, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
		                1, GX_TEVPREV);
		GXSetTevAlphaOp((GXTevStageID)i, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
		                1, GX_TEVPREV);
		++i;
	}

	if (mColorAlpha != 0xff || mCornerColor[0] != 0xffffffff
	    || mCornerColor[1] != 0xffffffff || mCornerColor[2] != 0xffffffff
	    || mCornerColor[3] != 0xffffffff) {
		GXSetTevOrder((GXTevStageID)i, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
		              GX_COLOR0A0);
		GXSetTevColorIn((GXTevStageID)i, GX_CC_ZERO, GX_CC_CPREV, GX_CC_RASC,
		                GX_CC_ZERO);
		GXSetTevAlphaIn((GXTevStageID)i, GX_CA_ZERO, GX_CA_APREV, GX_CA_RASA,
		                GX_CA_ZERO);
		GXSetTevColorOp((GXTevStageID)i, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
		                1, GX_TEVPREV);
		GXSetTevAlphaOp((GXTevStageID)i, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
		                1, GX_TEVPREV);
		++i;
	}
	GXSetNumTevStages(i);
	GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
}

void J2DPicture::swap(float& fst, float& snd)
{
	float tmp = fst;
	fst       = snd;
	snd       = tmp;
}

void J2DPicture::setBlendKonstColor()
{
	u32 result = 0;

	for (u8 i = 1; i < mTextureNum; ++i) {
		float sum = 0.0;
		for (u8 j = 0; j < i; ++j)
			sum += unk104[j];

		float nextSum = sum + unk104[i];
		if (0.0f != nextSum) {
			u8 alpha = (u8)(255.0f * (1.0f - sum / nextSum));
			result |= alpha << ((i - 1) * 8);
		}
	}

	mBlendKonstColor = result;
}

void J2DPicture::setBlendKonstAlpha()
{
	u32 result = 0;

	for (u8 i = 1; i < mTextureNum; ++i) {
		float sum = 0.0;
		for (u8 j = 0; j < i; ++j)
			sum += unk114[j];

		float nextSum = sum + unk114[i];
		if (0.0f != nextSum) {
			u8 alpha = (u8)(255.0f * (1.0f - sum / nextSum));
			result |= alpha << ((i - 1) * 8);
		}
	}

	mBlendKonstAlpha = result;
}
