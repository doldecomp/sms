#ifndef JDR_GRAPHICS_HPP
#define JDR_GRAPHICS_HPP

#include <JSystem/JGeometry.hpp>
#include <JSystem/JUtility/JUTColor.hpp>
#include <JSystem/JUtility/JUTRect.hpp>
#include <dolphin/mtx.h>
#include <dolphin/gx/GXStruct.h>

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
	TRect& getUnk54() { return unk54; }

	bool checkFlag8() { return (unkFC & 0x8) != 0; }
	bool checkFlag10() { return (unkFC & 0x10) != 0; }

	MtxPtr getUnkB4() { return unkB4.mMtx; }
	f32 getUnkE8() { return unkE8; }

	char unk0[0x4];
	/* 0x04 */ void* unk4;
	/* 0x08 */ GXRenderModeObj unk8;
	/* 0x44 */ TRect unk44;
	/* 0x54 */ TRect unk54;
	/* 0x64 */ TRect unk64;
	/* 0x74 */ TMtx44f unk74;
	/* 0xB4 */ TSMtx34f unkB4;
	/* 0xE4 */ u32 unkE4;
	/* 0xE8 */ float unkE8;
	/* 0xEC */ float unkEC;
	/* 0xF0 */ GXFBClamp unkF0;
	/* 0xF4 */ JUtility::TColor unkF4;
	/* 0xF8 */ u32 unkF8;
	/* 0xFC */ u16 unkFC;
};

} // namespace JDrama

#endif
