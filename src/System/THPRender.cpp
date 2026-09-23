#include <System/THPRender.hpp>

TTHPRender::TTHPRender(const char* name)
    : JDrama::TViewObj(name)
{
	mPos.x      = 0;
	mPos.y      = 0;
	frameNumber = -1;
}

void TTHPRender::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if ((cue & CUE_DRAW)) {

		SMS_DrawInit();
		GXLoadPosMtxImm(graphics->mViewMtx, GX_PNMTX0);
		GXSetCurrentMtx(GX_PNMTX0);
		frameNumber = THPPlayerDrawCurrentFrame(0, mPos.x, mPos.y, mSize.x,
		                                        mSize.y);
	}
}
