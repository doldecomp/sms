#ifndef SYSTEM_THP_RENDER_HPP
#define SYSTEM_THP_RENDER_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/JDrama/JDRGraphics.hpp>
#include <dolphin/gx/GXTransform.h>

#include <MarioUtil/DrawUtil.hpp>
#include <THPPlayer/THPPlayer.h>

class TTHPRender : public JDrama::TViewObj {
public:
	TTHPRender(const char* name = "<THPRender>");

	virtual void perform(u32, JDrama::TGraphics*); /* override */

	s32 getFrameNumber() const { return frameNumber; }

	void setParams(u32 a, u32 b, u32 c, u32 d)
	{
		x     = a;
		y     = b;
		polyW = c;
		polyH = d;
	}

public:
	/* 0x10 */ u32 x;
	/* 0x14 */ u32 y;
	/* 0x18 */ u32 polyW;
	/* 0x1c */ u32 polyH;
	/* 0x20 */ s32 frameNumber;
};

#endif // SYSTEM_THP_RENDER_HPP
