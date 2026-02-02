#include <JSystem/JDrama/JDRDisplay.hpp>
#include <JSystem/JDrama/JDRVideo.hpp>
#include <JSystem/JDrama/JDRGraphics.hpp>
#include <JSystem/JDrama/JDREfbSetting.hpp>
#include <dolphin/gx.h>

using namespace JDrama;

TDisplay::TDisplay(u16 param_1, void* param_2, void* param_3,
                   const GXRenderModeObj& param_4)
    : unkC(0)
    , unk10(param_4)
    , unk4C(param_1)
    , unk50(GX_GM_1_0)
    , unk54((GXFBClamp)(GX_CLAMP_TOP | GX_CLAMP_BOTTOM))
    , mFrameBufferClearColor(0, 0, 0, 0)
    , mFrameBufferClearZ(0xffffff)
    , unk64(0x20)
{
	unk4[0] = param_2;
	unk4[1] = param_3;
	if (param_2 == param_3)
		unk64 = unk64 | 0x40;
	unk60 = new TVideo();
}

void TDisplay::startRendering()
{
	unk60->mNextRenderMode = unk10;
	unk60->setNextXFB(unk4[unkC]);
	GXSetDispCopyGamma(unk50);
	GXSetDispCopyFrame2Field(GX_COPY_PROGRESSIVE);
	IssueGXPixelFormatSetting(unk10, (unk64 & 8) != 0, (unk64 & 0x10) != 0);
}

void TDisplay::endRendering()
{
	unk60->waitForRetrace(unk4C);

	if ((unk64 & 0x40) != 0) {
		TRect rect(0, 0, unk10.fbWidth, unk10.efbHeight);
		IssueGXCopyDisp(unk4[unkC], rect, unk10, mFrameBufferClearColor,
		                mFrameBufferClearZ, unk54, unk64);
		GXFlush();
	}
	unkC = unkC ^ 1;
}
