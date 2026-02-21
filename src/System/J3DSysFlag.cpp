#include <System/J3DSysFlag.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <MarioUtil/DrawUtil.hpp>

void TJ3DSysFlag::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (param_1 & 0x80)
		j3dSys.mFlags &= ~0x2;

	if (param_1 & 0x8) {
		j3dSys.mFlags |= 0x2;
		GXSetChanMatColor(GX_COLOR0A0, (GXColor) { 0xff, 0xff, 0xff, 0xff });
		GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
		              GX_COLOR0A0);
	}
}

void TJ3DSysSetViewMtx::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (param_1 & 0x4)
		MTXCopy(param_2->mViewMtx, j3dSys.mViewMtx);
}

void TSMSDrawInit::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (param_1 & 0x8)
		SMS_DrawInit();
}

extern "C" void ReInitializeGX();

void TReInitGX::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (param_1 & 0x80)
		ReInitializeGX();
}

void TGXAlphaUpdate::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (param_1 & 0x40000000)
		GXSetAlphaUpdate(GX_TRUE);

	if (param_1 & 0x20000000) {
		GXSetAlphaUpdate(GX_FALSE);
		GXSetDstAlpha(GX_FALSE, 0);
	}
}
