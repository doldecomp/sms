#ifndef J2D_GRAFCONTEXT_HPP
#define J2D_GRAFCONTEXT_HPP

#include <JSystem/JGeometry.hpp>
#include <JSystem/JUtility/JUTColor.hpp>
#include <JSystem/JUtility/JUTRect.hpp>
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
	void scissor(JUTRect const& bounds);
	void setColor(JUtility::TColor c) { setColor(c, c, c, c); }
	void setColor(JUtility::TColor colorTL, JUtility::TColor colorTR,
	              JUtility::TColor colorBR, JUtility::TColor colorBL);
	void setLineWidth(u8);
	void fillBox(JUTRect const& box);
	void drawFrame(JGeometry::TBox2<f32> const& box);
	void line(JGeometry::TVec2<f32> start, JGeometry::TVec2<f32> end);
	void lineTo(JGeometry::TVec2<f32> pos);
	void lineTo(f32 x, f32 y) { this->lineTo(JGeometry::TVec2<f32>(x, y)); }
	void moveTo(f32 x, f32 y) { this->moveTo(JGeometry::TVec2<f32>(x, y)); }

	void moveTo(JGeometry::TVec2<f32> pos);

	virtual ~J2DGrafContext() { }
	virtual void place(JUTRect const& bounds);
	virtual void place(int x, int y, int width, int height)
	{
		JUTRect box(x, y, x + width, y + height);
		place(box);
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
	/* 0x3C */ u32 unk3C;
	/* 0x40 */ u32 unk40;
	/* 0x44 */ Mtx44 mMtx44;
	/* 0x84 */ Mtx mPosMtx;
	/* 0xB4 */ Blend field_0xb0;
	/* 0xC0 */ Blend mLinePart;
	/* 0xCC */ Blend mBoxPart;
};

#endif
