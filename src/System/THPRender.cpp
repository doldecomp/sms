#include <System/THPRender.hpp>

void TTHPRender::perform(u32 flags, JDrama::TGraphics* gfx)
{
	if ((flags & 8)) {

		SMS_DrawInit();
		GXLoadPosMtxImm(gfx->mViewMtx, GX_PNMTX0);
		GXSetCurrentMtx(GX_PNMTX0);
		frameNumber = THPPlayerDrawCurrentFrame(0, x, y, polyW, polyH);
	}
}

TTHPRender::TTHPRender(const char* name)
    : JDrama::TViewObj(name)
{
	x           = 0;
	y           = 0;
	frameNumber = -1;
}
