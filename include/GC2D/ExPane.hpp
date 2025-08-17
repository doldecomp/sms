#ifndef GC2D_EX_PANE_HPP
#define GC2D_EX_PANE_HPP

#include <GC2D/Coord2D.hpp>
#include <JSystem/JUtility/JUTRect.hpp>
#include <dolphin/gx/GXEnum.h>

class J2DScreen;
class JUTTexture;
class J2DPane;

class TExPane {
public:
	TExPane(JUTTexture*, GXCullMode);
	TExPane(J2DScreen*, u32);

	bool update();

	void setPaneOffset(s32, s32, s32, s32, s32) { }
	void setPaneSize(s32, s32, s32, s32, s32) { }
	void setPaneAlpha(s32, s16, s16) { }

public:
	/* 0x0 */ J2DPane* unk0;
	/* 0x4 */ JUTRect unk4;
	/* 0x14 */ TCoord2D unk14;
	/* 0x2C */ TCoord2D unk2C;
	/* 0x44 */ f32 unk44;
	/* 0x48 */ f32 unk48;
	/* 0x4C */ s16 unk4C;
	/* 0x4E */ bool unk4E;
	/* 0x4F */ bool unk4F;
	/* 0x50 */ bool unk50;
};

#endif
