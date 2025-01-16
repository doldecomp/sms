#ifndef J2D_ORTHOGRAPH_HPP
#define J2D_ORTHOGRAPH_HPP

#include <JSystem/J2D/J2DGrafContext.hpp>

class J2DOrthoGraph : public J2DGrafContext {
public:
	J2DOrthoGraph();
	J2DOrthoGraph(const JUTRect&);
	J2DOrthoGraph(int x, int y, int width, int height);
	void setOrtho(JGeometry::TBox2<f32> const& bounds, f32 far, f32 near);
	void scissorBounds(JUTRect*, JUTRect*);

	virtual ~J2DOrthoGraph() { }
	virtual void setPort();
	virtual s32 getGrafType() const { return 1; }
	virtual void setLookat();

	f32 getWidthPower() const
	{
		return (float)mBounds.getWidth() / (float)mOrtho.getWidth();
	}
	f32 getHeightPower() const
	{
		return (float)mBounds.getHeight() / (float)mOrtho.getHeight();
	}

	void setOrtho(f32 x, f32 y, f32 width, f32 height, f32 far, f32 near)
	{
		JGeometry::TBox2<f32> ortho(x, y, x + width, y + height);
		setOrtho(ortho, far, near);
	}

private:
	/* 0xD8 */ JUTRect mOrtho;
	/* 0xE8 */ f32 mNear;
	/* 0xEC */ f32 mFar;
};

void J2DDrawLine(int x1, int y1, int x2, int y2, JUtility::TColor color,
                 int line_width);
void J2DFillBox(int x, int y, int width, int height, JUtility::TColor color);
void J2DFillBox(JUTRect box, JUtility::TColor color);
void J2DDrawFrame(int x, int y, int width, int height, JUtility::TColor color,
                  u8 line_width);
void J2DDrawFrame(JUTRect box, JUtility::TColor color, u8 line_width);

#endif
