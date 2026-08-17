#ifndef JUT_VIDEO_HPP
#define JUT_VIDEO_HPP

#include <dolphin/gx.h>
#include <types.h>

class JUTVideo {
public:
	virtual ~JUTVideo();

	u32 getFbWidth() const { return mRenderObj->fbWidth; }
	u32 getEfbHeight() const { return mRenderObj->efbHeight; }
	GXRenderModeObj* getRenderMode() const { return mRenderObj; }

	static JUTVideo* getManager() { return sManager; }

	static JUTVideo* sManager;

public:
	/* 0x04 */ GXRenderModeObj* mRenderObj;
};

inline JUTVideo* JUTGetVideoManager() { return JUTVideo::getManager(); }

#endif
