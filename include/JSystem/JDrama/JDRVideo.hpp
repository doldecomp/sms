#ifndef JDR_VIDEO_HPP
#define JDR_VIDEO_HPP

#include <dolphin/gx/GXStruct.h>

namespace JDrama {

class TVideo {
public:
	TVideo();
	void setNextXFB(const void*);
	void waitForRetrace(u16);

public:
	/* 0x00 */ GXRenderModeObj mCurRenderMode;
	/* 0x3C */ GXRenderModeObj mNextRenderMode;
	/* 0x78 */ const void* mCurFrameBuffer;
	/* 0x7C */ const void* mNextFrameBuffer;
	/* 0x80 */ s32 mLastRetraceTime;
	/* 0x84 */ s32 mNextRetraceIndex;
};

}; // namespace JDrama

#endif
