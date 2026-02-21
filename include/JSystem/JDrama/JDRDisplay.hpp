#ifndef JDR_DISPLAY_HPP
#define JDR_DISPLAY_HPP

#include <dolphin/gx/GXStruct.h>
#include <JSystem/JUtility/JUTColor.hpp>

namespace JDrama {

class TVideo;

class TDisplay {
public:
	TDisplay(u16, void*, void*, const GXRenderModeObj&);

	virtual void startRendering();
	virtual void endRendering();

	void* getCurrentFrameBuffer() { return unk4[unkC]; }
	const GXRenderModeObj& getRenderMode() const { return unk10; }
	GXRenderModeObj& getRenderMode() { return unk10; }
	GXFBClamp getFBClamp() const { return unk54; }
	JUtility::TColor getClearColor() const { return mFrameBufferClearColor; }
	u32 getClearZ() const { return mFrameBufferClearZ; }
	TVideo* getVideo() { return unk60; }

	void onFlag(u16 flag) { unk64 |= flag; }
	void offFlag(u16 flag) { unk64 &= ~flag; }

public:
	/* 0x4 */ void* unk4[2];
	/* 0xC */ u16 unkC;

	/* 0x10 */ GXRenderModeObj unk10;
	/* 0x4C */ u16 unk4C;
	/* 0x50 */ GXGamma unk50;
	/* 0x54 */ GXFBClamp unk54;
	/* 0x58 */ JUtility::TColor mFrameBufferClearColor;
	/* 0x5C */ u32 mFrameBufferClearZ;
	/* 0x60 */ TVideo* unk60;
	/* 0x64 */ u16 unk64; // TODO: TFlagT?
};

} // namespace JDrama

#endif
