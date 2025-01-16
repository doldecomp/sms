#include <JSystem/J2D/J2DOrthoGraph.hpp>
#include <dolphin/gx.h>

J2DOrthoGraph::J2DOrthoGraph()
    : J2DGrafContext(0, 0, 0, 0)
{
	setLookat();
	unk4 = 1;
}

J2DOrthoGraph::J2DOrthoGraph(const JUTRect& rect)
    : J2DGrafContext(rect)
{
	mOrtho = JUTRect(0, 0, rect.getWidth(), rect.getHeight());
	mNear  = -1.0f;
	mFar   = 1.0f;
	setLookat();
	unk4 = 1;
}

J2DOrthoGraph::J2DOrthoGraph(int x, int y, int width, int height)
    : J2DGrafContext(x, y, width, height)
{
	mOrtho = JUTRect(0, 0, width, height);
	mNear  = -1.0f;
	mFar   = 1.0f;
	setLookat();
	unk4 = 1;
}

void J2DOrthoGraph::setPort()
{
	J2DGrafContext::setPort();
	C_MTXOrtho(mMtx44, mOrtho.y1, mOrtho.y2 + 0.5f, mOrtho.x1, mOrtho.x2, mNear,
	           mFar);
	GXSetProjection(mMtx44, GX_ORTHOGRAPHIC);
}

void J2DOrthoGraph::setLookat()
{
	MTXIdentity(mPosMtx);
	GXLoadPosMtxImm(mPosMtx, GX_PNMTX0);
}

void J2DOrthoGraph::scissorBounds(JUTRect* param_0, JUTRect* param_1)
{
	param_0->x1
	    = mBounds.x1 + getWidthPower() * (param_1->x1 - mOrtho.x1) + 0.1f;
	param_0->x2
	    = mBounds.x1 + getWidthPower() * (param_1->x2 - mOrtho.x1) + 0.9f;
	param_0->y1
	    = mBounds.y1 + getHeightPower() * (param_1->y1 - mOrtho.y1) + 0.1f;
	param_0->y2
	    = mBounds.y1 + getHeightPower() * (param_1->y2 - mOrtho.y1) + 0.9f;
}

void J2DFillBox(int x, int y, int width, int height, JUtility::TColor color)
{
	J2DFillBox(JUTRect(x, y, x + width, y + height), color);
}

void J2DFillBox(JUTRect box, JUtility::TColor color)
{
	J2DOrthoGraph oGrph;
	oGrph.setColor(color);
	oGrph.fillBox(box);
}
