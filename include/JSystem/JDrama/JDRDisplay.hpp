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

public:
	/* 0x4 */ void* unk4[2];
	/* 0xC */ u16 unkC;

	/* 0x10 */ GXRenderModeObj unk10;
	/* 0x4C */ u16 unk4C;
	/* 0x50 */ GXGamma unk50;
	/* 0x54 */ GXFBClamp unk54;
	/* 0x58 */ JUtility::TColor unk58;
	/* 0x5C */ u32 unk5C;
	/* 0x60 */ TVideo* unk60;
	/* 0x64 */ u16 unk64; // TODO: TFlagT?
};

} // namespace JDrama

#endif
