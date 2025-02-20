#ifndef JDR_GRAPHICS_HPP
#define JDR_GRAPHICS_HPP

#include <JSystem/JUtility/JUTRect.hpp>
#include <dolphin/mtx.h>

namespace JDrama {

struct TRect : JUTRect {
	TRect()
	    : JUTRect()
	{
	}
	TRect(int x1, int y1, int x2, int y2)
	    : JUTRect(x1, y1, x2, y2)
	{
	}
};

struct TGraphics {
	void setViewport(const TRect&, float, float);

	TRect& getUnk44() { return unk44; }

	char unk0[0x44];
	/* 0x44 */ TRect unk44;
	/* 0x54 */ TRect unk54;
	/* 0x64 */ TRect unk64;
	char unk74[0x40];
	/* 0xB4 */ Mtx unkB4;
	/* 0xE4 */ u32 unkE4;
};

} // namespace JDrama

#endif
