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

	char unk0[0x20];
	/* 0x20 */ u8 unk20;
	char pad[3];
	char unk24[0x20];
	/* 0x44 */ TRect unk44;
	/* 0x54 */ TRect unk54;
	/* 0x64 */ TRect unk64;
	/* 0x74 */ Mtx unk74;
	char unkA4[0x10];
	/* 0xB4 */ Mtx unkB4;
	/* 0xE4 */ u32 unkE4;
	/* 0xE8 */ float unkE8;
	/* 0xEC */ float unkEC;
};

} // namespace JDrama

#endif
