#include <JSystem/J2D/J2DOrthoGraph.hpp>
#include <dolphin/gx.h>

J2DOrthoGraph::J2DOrthoGraph()
    : J2DGrafContext(0.0f, 0.0f, 0.0f, 0.0f)
{
	setLookat();
}

J2DOrthoGraph::J2DOrthoGraph(f32 x, f32 y, f32 width, f32 height, f32 far,
                             f32 near)
    : J2DGrafContext(x, y, width, height)
{
	mOrtho = JGeometry::TBox2<f32>(0.0f, 0.0f, width, height);
	mNear  = far;
	mFar   = near;
	setLookat();
}

void J2DOrthoGraph::setPort()
{
	J2DGrafContext::setPort();
	MTXOrtho(mMtx44, mOrtho.i.y, mOrtho.f.y + 0.5f, mOrtho.i.x, mOrtho.f.x,
	           mNear, mFar);
	GXSetProjection(mMtx44, GX_ORTHOGRAPHIC);
}

void J2DOrthoGraph::setOrtho(const JGeometry::TBox2<f32>& bounds, f32 far,
                             f32 near)
{
	mOrtho = bounds;
	mNear  = -near;
	mFar   = -far;
}

void J2DOrthoGraph::setLookat()
{
	MTXIdentity(mPosMtx);
	GXLoadPosMtxImm(mPosMtx, GX_PNMTX0);
}

void J2DOrthoGraph::scissorBounds(JGeometry::TBox2<f32>* param_0,
                                  const JGeometry::TBox2<f32>* param_1)
{
	f32 widthPower  = getWidthPower();
	f32 heightPower = getHeightPower();
	f32 f0          = mBounds.i.x + widthPower * (param_1->i.x - mOrtho.i.x);
	f32 f2          = mBounds.i.x + widthPower * (param_1->f.x - mOrtho.i.x);
	f32 f1          = mBounds.i.y + heightPower * (param_1->i.y - mOrtho.i.y);
	f32 f3          = mBounds.i.y + heightPower * (param_1->f.y - mOrtho.i.y);
	param_0->set(f0, f1, f2, f3);
}

void J2DDrawLine(f32 x1, f32 y1, f32 x2, f32 y2, JUtility::TColor color,
                 int line_width)
{
	J2DOrthoGraph oGrph;
	oGrph.setLineWidth(line_width);
	oGrph.setColor(color);
	oGrph.moveTo(x1, y1);
	oGrph.lineTo(x2, y2);
}

void J2DFillBox(f32 x, f32 y, f32 width, f32 height, JUtility::TColor color)
{
	J2DFillBox(JGeometry::TBox2<f32>(x, y, x + width, y + height), color);
}

void J2DFillBox(const JGeometry::TBox2<f32>& box, JUtility::TColor color)
{
	J2DOrthoGraph oGrph;
	oGrph.setColor(color);
	oGrph.fillBox(box);
}

void J2DDrawFrame(f32 x, f32 y, f32 width, f32 height, JUtility::TColor color,
                  u8 line_width)
{
	J2DDrawFrame(JGeometry::TBox2<f32>(x, y, x + width, y + height), color,
	             line_width);
}

void J2DDrawFrame(const JGeometry::TBox2<f32>& box, JUtility::TColor color,
                  u8 line_width)
{
	J2DOrthoGraph oGrph;
	oGrph.setColor(color);
	oGrph.setLineWidth(line_width);
	oGrph.drawFrame(box);
}
