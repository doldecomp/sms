#ifndef SYSTEM_THP_RENDER_HPP
#define SYSTEM_THP_RENDER_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/JDrama/JDRGraphics.hpp>
#include <dolphin/gx/GXTransform.h>
#include <JSystem/JGeometry/JGVec2.hpp>

#include <MarioUtil/DrawUtil.hpp>
#include <THPPlayer/THPPlayer.h>

class TTHPRender : public JDrama::TViewObj {
public:
	TTHPRender(const char* name = "<THPRender>");

	virtual void perform(u32 cue, JDrama::TGraphics* graphics); /* override */

	s32 getFrameNumber() const { return frameNumber; }

public:
	/* 0x10 */ JGeometry::TVec2<u32> mPos;
	/* 0x18 */ JGeometry::TVec2<u32> mSize;
	/* 0x20 */ s32 frameNumber;
};

#endif // SYSTEM_THP_RENDER_HPP
