#include <JSystem/J2D/J2DGrafContext.hpp>
#include <dolphin/gx.h>

J2DGrafContext::J2DGrafContext(const JUTRect& rect)
    : mBounds(rect)
    , mScissorBounds(rect)
    , mPrevPos(0, 0)
{
	setColor(JUtility::TColor());
	setLineWidth(6);
}

J2DGrafContext::J2DGrafContext(int x, int y, int width, int height)
    : mBounds(x, y, x + width, y + height)
    , mScissorBounds(x, y, x + width, y + height)
    , mPrevPos(0, 0)
{
	setColor(JUtility::TColor());
	setLineWidth(6);
}

void J2DGrafContext::setPort()
{
	GXSetViewport(mBounds.x1, mBounds.y1, mBounds.getWidth(),
	              mBounds.getHeight(), 0.0f, 1.0f);
	setScissor();
	setup2D();
}

void J2DGrafContext::setup2D()
{
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX0, GX_NONE);
	GXLoadPosMtxImm(mPosMtx, GX_PNMTX0);
	GXSetNumChans(1);
	GXSetNumTexGens(0);
	GXSetNumTevStages(1);
	GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
	GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL,
	              GX_DF_NONE, GX_AF_NONE);
	GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_GREATER, 0);
	Mtx mtx;
	MTXIdentity(mtx);
	GXLoadTexMtxImm(mtx, GX_IDENTITY, GX_MTX3x4);
	GXSetCurrentMtx(GX_PNMTX0);
	GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
	GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, GX_FALSE,
	                  0x7D);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_U16, 0xf);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX1, GX_TEX_ST, GX_U16, 0xf);
	GXSetLineWidth(mLineWidth, GX_TO_ZERO);
	GXSetCullMode(GX_CULL_NONE);

	GXSetNumIndStages(0);
	for (int i = 0; i < GX_TEVSTAGE8; i++) {
		GXSetTevDirect((GXTevStageID)i);
	}
}

void J2DGrafContext::setScissor()
{
	JUTRect maxBounds(0, 0, 1024, 1000);
	JUTRect curBounds(mScissorBounds);

	mScissorBounds.intersect(maxBounds);
	curBounds.normalize();
	curBounds.add(0, -1);

	if (curBounds.intersect(maxBounds)) {
		GXSetScissor(curBounds.x1, curBounds.y1, curBounds.getWidth(),
		             curBounds.getHeight());
	} else {
		GXSetScissor(0, 0, 0, 0);
	}
}

void J2DGrafContext::scissor(const JUTRect& bounds) { mScissorBounds = bounds; }

void J2DGrafContext::place(const JUTRect& bounds)
{
	mBounds        = bounds;
	mScissorBounds = bounds;
}

void J2DGrafContext::setColor(JUtility::TColor colorTL,
                              JUtility::TColor colorTR,
                              JUtility::TColor colorBR,
                              JUtility::TColor colorBL)
{
	mColorTL              = colorTL;
	mColorTR              = colorTR;
	mColorBR              = colorBR;
	mColorBL              = colorBL;
	field_0xb4.mType      = GX_BM_BLEND;
	field_0xb4.mSrcFactor = GX_BL_SRCALPHA;
	field_0xb4.mDstFactor = GX_BL_INVSRCALPHA;
	mLinePart.mType       = GX_BM_BLEND;
	mLinePart.mSrcFactor  = GX_BL_SRCALPHA;
	mLinePart.mDstFactor  = GX_BL_INVSRCALPHA;
	mBoxPart.mType        = GX_BM_BLEND;
	mBoxPart.mSrcFactor   = GX_BL_SRCALPHA;
	mBoxPart.mDstFactor   = GX_BL_INVSRCALPHA;
	if ((mColorTL & 0xFF) != 0xFF) {
		return;
	}
	field_0xb4.mType      = GX_BM_NONE;
	field_0xb4.mSrcFactor = GX_BL_ONE;
	field_0xb4.mDstFactor = GX_BL_ZERO;
	if ((mColorBR & 0xFF) != 0xFF) {
		return;
	}
	mLinePart.mType      = GX_BM_NONE;
	mLinePart.mSrcFactor = GX_BL_ONE;
	mLinePart.mDstFactor = GX_BL_ZERO;
	if ((mColorTR & 0xFF) != 0xFF) {
		return;
	}
	if ((mColorBL & 0xFF) != 0xFF) {
		return;
	}
	mBoxPart.mType      = GX_BM_NONE;
	mBoxPart.mSrcFactor = GX_BL_ONE;
	mBoxPart.mDstFactor = GX_BL_ZERO;
}

void J2DGrafContext::setLineWidth(u8 lineWidth)
{
	mLineWidth = lineWidth;
	GXSetLineWidth(mLineWidth, GX_TO_ZERO);
}

void J2DGrafContext::fillBox(const JUTRect& box)
{
	GXSetBlendMode(mBoxPart.mType, mBoxPart.mSrcFactor, mBoxPart.mDstFactor,
	               GX_LO_SET);
	GXLoadPosMtxImm(mPosMtx, GX_PNMTX0);
	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition3s16(box.x1, box.y1, 0);
	GXColor1u32(mColorTL);
	GXPosition3s16(box.x2, box.y1, 0);
	GXColor1u32(mColorTR);
	GXPosition3s16(box.x2, box.y2, 0);
	GXColor1u32(mColorBL);
	GXPosition3s16(box.x1, box.y2, 0);
	GXColor1u32(mColorBR);
	GXEnd();
}
