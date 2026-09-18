#ifndef JDR_DISPLAY_HPP
#define JDR_DISPLAY_HPP

#include <dolphin/gx/GXStruct.h>
#include <JSystem/JUtility/JUTColor.hpp>
#include <JSystem/JDrama/JDRFlag.hpp>

namespace JDrama {

class TVideo;

class TDisplay {
public:
	TDisplay(u16, void*, void*, const GXRenderModeObj&);

	virtual void startRendering();
	virtual void endRendering();

	void* getCurrentFrameBuffer() { return unk4[unkC]; }
	// The named pointer is the binding level of frame-gaps.md batches 110/127/
	// 130: it emits nothing, and it is the 4 bytes of dead low region that all
	// four System/RenderModeObj `SMSSetup*RenderingInfo` functions were short of
	// (8/8/8/16 of frame, Game paying twice). All four are byte-exact with it and
	// RenderModeObj.cpp is source-linked; the DOL is unchanged. `rmo` is the name
	// RenderModeObj.cpp gives the same object at every one of its own sites.
	//
	// Measured as +0 (do not retry): `return unk10;` or `return *(&unk10);` --
	// the address-of alone is not the lever, the binding is; a pointer-returning
	// accessor with the call sites adjusted, or a pointer accessor plus a
	// reference forwarder (`return *getRenderModePtr();`); dropping the const
	// overload; a `(GXRenderModeObj&)unk10` cast; on/offFlag routed through
	// JDrama::TFlagT::on/off; a named `GXRenderModeObj*`/`&` local at the call
	// sites; and a TU-local `SMSGetRenderMode(TDisplay*)` free function.
	// Defining this out of class as `inline` is equivalent, not more natural.
	const GXRenderModeObj& getRenderMode() const { return unk10; }
	GXRenderModeObj& getRenderMode()
	{
		GXRenderModeObj* rmo = &unk10;
		return *rmo;
	}
	GXFBClamp getFBClamp() const { return unk54; }
	JUtility::TColor getClearColor() const { return mFrameBufferClearColor; }
	u32 getClearZ() const { return mFrameBufferClearZ; }
	TVideo* getVideo() { return unk60; }

	void onFlag(u16 flag) { unk64.mValue |= flag; }
	void offFlag(u16 flag) { unk64.mValue &= ~flag; }

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
	/* 0x64 */ TFlagT<u16> unk64; // TODO: TFlagT?
};

} // namespace JDrama

#endif
