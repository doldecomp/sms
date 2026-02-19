#ifndef JDR_GRAPHICS_HPP
#define JDR_GRAPHICS_HPP

#include <JSystem/JDrama/JDRFlag.hpp>
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

	TRect& getUnk44() { return mDisplayRect; }
	TRect& getUnk54() { return mViewportRect; }
	const TRect& getUnk64() { return mScissorRect; }

	MtxPtr getUnkB4() { return mViewMtx.mMtx; }
	f32 getNearPlane() { return mNearPlane; }

	TFlagT<u16>& getUnkFC() { return unkFC; }

	/* 0x00 */ u16 unk0;
	/* 0x02 */ u16 unk2;
	/* 0x04 */ void* mFrameBuffer;
	/* 0x08 */ GXRenderModeObj mRenderMode;
	/* 0x44 */ TRect mDisplayRect;
	/* 0x54 */ TRect mViewportRect;
	/* 0x64 */ TRect mScissorRect;
	/* 0x74 */ TProjection3f mProjMtx;
	/* 0xB4 */ TPosition3f mViewMtx;
	/* 0xE4 */ u32 unkE4;
	/* 0xE8 */ float mNearPlane;
	/* 0xEC */ float mFarPlane;
	/* 0xF0 */ GXFBClamp mFBClamp;
	/* 0xF4 */ JUtility::TColor mClearColor;
	/* 0xF8 */ u32 mClearZ;
	/* 0xFC */ TFlagT<u16> unkFC;
	/* 0xFE */ TFlagT<u16> unkFE;
};

} // namespace JDrama

#endif
