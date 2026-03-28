#ifndef JDR_GRAPHICS_HPP
#define JDR_GRAPHICS_HPP

#include <JSystem/JDrama/JDRRect.hpp>
#include <JSystem/JDrama/JDRFlag.hpp>
#include <JSystem/JGeometry.hpp>
#include <JSystem/JUtility/JUTColor.hpp>
#include <JSystem/JUtility/JUTRect.hpp>
#include <dolphin/mtx.h>
#include <dolphin/gx/GXCull.h>
#include <dolphin/gx/GXStruct.h>

namespace JDrama {

struct TGraphics {
	void setDisplayRect(const TRect& rect) { mDisplayRect = rect; }
	void setViewport(const TRect&, f32, f32);
	void setScissor(const TRect& scissor)
	{
		mScissorRect = scissor;
		GXSetScissor(mScissorRect.x1, mScissorRect.y1, mScissorRect.getWidth(),
		             mScissorRect.getHeight());
	}

	// fabricated
	void* getFrameBuffer() { return mFrameBuffer; }
	const GXRenderModeObj& getRenderMode() { return mRenderMode; }
	const TRect& getDisplayRect() { return mDisplayRect; }
	const TRect& getViewport() { return mViewportRect; }
	const TRect& getScissor() { return mScissorRect; }

	MtxPtr getUnkB4() { return mViewMtx.mMtx; }
	f32 getNearPlane() { return mNearPlane; }

	GXFBClamp getFBClamp() { return mFBClamp; }
	JUtility::TColor getClearColor() { return mClearColor; }
	u32 getClearZ() { return mClearZ; }
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
	/* 0xFE */ u16 unkFE;
};

} // namespace JDrama

#endif
