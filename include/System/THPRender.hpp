#ifndef SYSTEM_THP_RENDER_HPP
#define SYSTEM_THP_RENDER_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/JDrama/JDRGraphics.hpp>
#include <JSystem/JDrama/JDRSize.hpp>
#include <dolphin/gx/GXTransform.h>

#include <MarioUtil/DrawUtil.hpp>
#include <THPPlayer/THPPlayer.h>

class TTHPRender : public JDrama::TViewObj {
public:
	TTHPRender(const char* name = "<THPRender>");

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	s32 getFrameNumber() const { return frameNumber; }

	void setPos(const JGeometry::TVec2<u32>& pos) { mPos = pos; }
	void setSize(const JDrama::TSize& size) { mSize = size; }

public:
	/* 0x10 */ JGeometry::TVec2<u32> mPos;
	/* 0x18 */ JDrama::TSize mSize;
	/* 0x20 */ s32 frameNumber;
};

#endif // SYSTEM_THP_RENDER_HPP
