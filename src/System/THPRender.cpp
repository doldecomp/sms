#include "System/THPRender.hpp"

void TTHPRender::perform(u32 flags, JDrama::TGraphics* gfx)
{
	if ((flags & 8)) {

		SMS_DrawInit();
		GXLoadPosMtxImm(gfx->unkB4, GX_PNMTX0);
		GXSetCurrentMtx(GX_PNMTX0);
		this->unk10 = THPPlayerDrawCurrentFrame(0, this->x, this->y,
		                                        this->polyW, this->polyH);
	}
}
TTHPRender::TTHPRender(const char* name) : JDrama::TViewObj(name)
{
	this->x     = 0;
	this->y     = 0;
	this->unk10 = -1;
}
