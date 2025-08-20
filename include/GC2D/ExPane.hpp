#ifndef GC2D_EX_PANE_HPP
#define GC2D_EX_PANE_HPP

#include <GC2D/Coord2D.hpp>
#include <JSystem/JUtility/JUTRect.hpp>
#include <JSystem/J2D/J2DPane.hpp>
#include <dolphin/gx/GXEnum.h>

class J2DScreen;
class JUTTexture;
class J2DPane;

// fabricated
inline static s16 clapAlpha(s16 alpha)
{
	s16 a = alpha < 0 ? s16(0) : alpha;
	if (a > 255)
		return 255;
	return a;
}

class TExPane {
public:
	TExPane(JUTTexture*, GXCullMode);
	TExPane(J2DScreen*, u32);

	bool update();

	void setPaneOffset(s32 param_1, s32 param_2, s32 param_3, s32 param_4,
	                   s32 param_5)
	{
		unk14.setValue(param_1, param_2, param_3, param_4, param_5);
		unk0->move(unk4.x1 + param_4, unk4.y1 + param_5);
		unk4E = 1;
	}
	void setPaneSize(s32 param_1, s32 param_2, s32 param_3, s32 param_4,
	                 s32 param_5)
	{
		unk14.setValue(param_1, param_2, param_3, param_4, param_5);
		unk0->resize(unk4.x1, unk4.x1);
		unk4F = 1;
	}
	void setPaneAlpha(s32 param_1, s16 param_2, s16 param_3)
	{
		unk0->setAlpha(clapAlpha(param_3));

		unk44 = param_3;
		unk48 = f32(param_2 - param_3) / param_1;
		unk4C = param_2;
		unk50 = 1;
	}

	// fabricated
	J2DPane* getPane() const { return unk0; }
	const JUTRect& getUnk4() const { return unk4; }

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
