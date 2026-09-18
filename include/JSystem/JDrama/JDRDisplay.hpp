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
	// TODO: this inline is the unique carrier of the 8/16 bytes of dead
	// frame that all four System/RenderModeObj `SMSSetup*RenderingInfo`
	// functions are short of (GCLogo 0x18 vs 0x20, Movie 0x20 vs 0x28, Title
	// 0x28 vs 0x30, Game 0x28 vs 0x38 -- Game pays twice). Any one extra
	// 4-byte object bound inside it lands all four exactly, with zero
	// instruction change and no regression anywhere in the tree: both a dead
	// non-trivial 4-byte local and `GXRenderModeObj& rmo = unk10; return rmo;`
	// do it. Both are unnecessary temporaries with nothing to name them
	// after, so neither is committed.
	// Measured as +0 (do not retry): a pointer-returning accessor
	// (`GXRenderModeObj* getRenderMode() { return &unk10; }`, the JUTVideo
	// spelling, with the call sites adjusted); dropping the const overload; a
	// `(GXRenderModeObj&)unk10` cast; on/offFlag routed through
	// JDrama::TFlagT::on/off; a named `GXRenderModeObj&` at the two call sites
	// that do not already bind one; and a TU-local
	// `SMSGetRenderMode(TDisplay*)` free function above the accessor (a
	// reference return folds, so the free function's pointer parameter binds
	// nothing -- unlike the value-returning case in MirrorActor.cpp).
	const GXRenderModeObj& getRenderMode() const { return unk10; }
	GXRenderModeObj& getRenderMode() { return unk10; }
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
