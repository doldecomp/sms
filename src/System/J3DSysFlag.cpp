#include <System/J3DSysFlag.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <System/MarDirector.hpp>
#include <MarioUtil/DrawUtil.hpp>

void TJ3DSysFlag::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_DRAW_INIT)
		j3dSys.mFlags &= ~0x2;

	if (cue & CUE_DRAW) {
		j3dSys.mFlags |= 0x2;
		GXSetChanMatColor(GX_COLOR0A0, (GXColor) { 0xff, 0xff, 0xff, 0xff });
		GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
		              GX_COLOR0A0);
	}
}

void TJ3DSysSetViewMtx::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_CALC_VIEW)
		MTXCopy(graphics->mViewMtx, j3dSys.mViewMtx);
}

void TSMSDrawInit::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_DRAW)
		SMS_DrawInit();
}

extern "C" void ReInitializeGX();

void TReInitGX::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_DRAW_INIT)
		ReInitializeGX();
}

void TGXAlphaUpdate::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_UNK40000000)
		GXSetAlphaUpdate(GX_TRUE);

	if (cue & CUE_UNK20000000) {
		GXSetAlphaUpdate(GX_FALSE);
		GXSetDstAlpha(GX_FALSE, 0);
	}
}
