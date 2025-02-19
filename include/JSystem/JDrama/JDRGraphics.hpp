#ifndef JDR_GRAPHICS_HPP
#define JDR_GRAPHICS_HPP

#include <JSystem/JUtility/JUTRect.hpp>
#include <dolphin/mtx.h>

namespace JDrama {

struct TRect : JUTRect {
	TRect(int x1, int y1, int x2, int y2)
	    : JUTRect(x1, y1, x2, y2)
	{
	}
};

struct TGraphics {
	void setViewport(const TRect&, float, float);

	char unk0[0xB4];
	Mtx unkB4;
};

} // namespace JDrama

#endif
