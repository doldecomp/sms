#ifndef GC2D_BOUND_PANE_HPP
#define GC2D_BOUND_PANE_HPP

#include <JSystem/JUtility/JUTRect.hpp>
#include <JSystem/JUtility/JUTPoint.hpp>
#include <dolphin/gx/GXEnum.h>

class J2DScreen;
class JUTTexture;
class J2DPane;

// fabricated: plain POD, no non-trivial ctor (unlike JUTRect)
struct SBoundRect {
	SBoundRect() : x1(0), y1(0), x2(0), y2(0) {}
	SBoundRect(s32 a, s32 b, s32 c, s32 d) : x1(a), y1(b), x2(c), y2(d) {}
	operator JUTRect() const { return JUTRect(x1, y1, x2, y2); }
	s32 x1;
	s32 y1;
	s32 x2;
	s32 y2;
};

class TBoundPane {
public:
	TBoundPane(J2DScreen*, u32);
	TBoundPane(JUTTexture*, GXCullMode);
	void setPanePosition(s32, const JUTPoint&, const JUTPoint&,
	                     const JUTPoint&);
	void setPaneSize(s32, const JUTPoint&, const JUTPoint&, const JUTPoint&);
	bool update();
	void makeNewPosition(f32, JUTPoint&, JUTPoint&, JUTPoint&, JUTPoint&);

	// fabricated
	J2DPane* getPane() const { return unk0; }

public:
	/* 0x0 */ J2DPane* unk0;
	/* 0x4 */ JUTRect unk4;
	/* 0x14 */ SBoundRect unk14;
	/* 0x24 */ bool unk24;
	/* 0x25 */ bool unk25;
	/* 0x28 */ f32 unk28;
	/* 0x2C */ f32 unk2C;
	/* 0x30 */ f32 unk30;
	/* 0x34 */ f32 unk34;
	/* 0x38 */ JUTPoint unk38;
	/* 0x40 */ JUTPoint unk40;
	/* 0x48 */ JUTPoint unk48;
	/* 0x50 */ JUTPoint unk50;
	/* 0x58 */ JUTPoint unk58;
	/* 0x60 */ JUTPoint unk60;
};

#endif
