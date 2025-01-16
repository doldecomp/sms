#ifndef J2D_GRAFCONTEXT_HPP
#define J2D_GRAFCONTEXT_HPP

#include <JSystem/JUtility/JUTColor.hpp>
#include <JSystem/JUtility/JUTRect.hpp>
#include <JSystem/JUtility/JUTPoint.hpp>
#include <dolphin/mtx.h>
#include <dolphin/types.h>

class J2DGrafContext {
public:
	struct Blend {
		GXBlendMode mType;
		GXBlendFactor mSrcFactor;
		GXBlendFactor mDstFactor;
	};

	J2DGrafContext(const JUTRect& rect);
	J2DGrafContext(int x, int y, int width, int height);

	void scissor(const JUTRect& bounds);
	void setColor(JUtility::TColor c) { setColor(c, c, c, c); }
	void setColor(JUtility::TColor colorTL, JUtility::TColor colorTR,
	              JUtility::TColor colorBR, JUtility::TColor colorBL);
	void setLineWidth(u8);
	void fillBox(const JUTRect& box);
	void drawFrame(const JUTRect&);
	void line(JUTPoint start, JUTPoint end);
	void lineTo(JUTPoint pos);
	void polyline(JUTPoint*, int);
	void polyline(int*, int);
	void polylineTo(JUTPoint*, int);
	void polylineTo(int*, int);

	virtual ~J2DGrafContext() { }
	virtual void place(JUTRect const& bounds);
	virtual void place(int x, int y, int width, int height)
	{
		place(JUTRect(x, y, x + width, y + height));
	}
	virtual void setPort();
	virtual void setup2D();
	virtual void setScissor();
	virtual void setLookat() { }

public:
	/* 0x04 */ u32 unk4;
	/* 0x08 */ JUTRect mBounds;
	/* 0x18 */ JUTRect mScissorBounds;
	/* 0x28 */ JUtility::TColor mColorTL;
	/* 0x2C */ JUtility::TColor mColorTR;
	/* 0x30 */ JUtility::TColor mColorBR;
	/* 0x34 */ JUtility::TColor mColorBL;
	/* 0x38 */ u8 mLineWidth;
	/* 0x3C */ JUTPoint mPrevPos;
	/* 0x44 */ Mtx44 mMtx44;
	/* 0x84 */ Mtx mPosMtx;
	/* 0xB4 */ Blend field_0xb4;
	/* 0xC0 */ Blend mLinePart;
	/* 0xCC */ Blend mBoxPart;
};

#endif
