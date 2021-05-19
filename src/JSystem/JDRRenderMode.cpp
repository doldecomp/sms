#include "JDRRenderMode.hpp"

#if 0
bool IsEqualRenderModeVIParams(const GXRenderModeObj &, const GXRenderModeObj &)
{
}

bool IsRenderModeHalfAspectRatio(const GXRenderModeObj &)
{
}

f32 GetRenderModeYScale(const GXRenderModeObj &)
{
}

void JDrama::CalcRenderModeXFBHeight(GXRenderModeObj *rmo, ushort param_2)
{
	bool bVar1;
	bool bVar2;
	uint uVar3;
	uint uVar4;
	double dVar5;

	bVar2 = false;
	if (rmo->aa == '\x01')
	{
		bVar1 = true;
		if (rmo->xFBmode != VI_XFBMODE_DF)
			if (rmo->viTVmode != VI_TVMODE_NTSC_PROG)
				bVar1 = false;
		if (bVar1)
			bVar2 = true;
	}
	if (bVar2)
		os::OSPanic("JDRRenderMode.cpp", 0x48, "future not implemented!\n");
	if (rmo->field_rendering == '\0')
		uVar4 = 1;
	else
		uVar4 = 2;
	dVar5 = gx::GXGetYScaleFactor((uint)rmo->efbHeight, param_2 / uVar4);
	uVar3 = gx::GXGetNumXfbLines(dVar5, (uint)rmo->efbHeight);
	rmo->xfbHeight = (ushort)uVar3;
	rmo->viHeight = rmo->xfbHeight * (short)uVar4;
	return;
}
#endif

void JDrama::CalcRenderModeVIXOrigin(GXRenderModeObj *rmo)
{
	u32 region;
	s32 max_width;
	u32 dif;

	region = (rmo->viTVmode >> 2); /*	TV modes are laid out so every
										region is alligned to a power of four	*/
	max_width = JDrama::GetVIWidthMax((VITVMode)region);
	dif = (u16)max_width;
	dif -= rmo->viHeight;
	rmo->viYOrigin = (((s32)dif / 2) + ((dif < 0) && ((dif & 1) != 0)));

	return;
}

void JDrama::CalcRenderModeVIYOrigin(GXRenderModeObj *rmo)
{
	u32 region;
	s32 max_height;
	u32 dif;

	region = (rmo->viTVmode >> 2); /*	TV modes are laid out so every
										region is alligned to a power of four	*/
	max_height = JDrama::GetVIHeightMax((VITVMode)region);
	dif = (u16)max_height;
	dif -= rmo->viHeight;
	rmo->viYOrigin = (((s32)dif / 2) + ((dif < 0) && ((dif & 1) != 0)));

	return;
}

void JDrama::CopyRenderModeSamplePattern(GXRenderModeObj *rmo, u8 *s)
{
	/*Runtime.PPCEABI.H::*/ memcpy(rmo->sample_pattern, s, 24);
	return;
}

void JDrama::CopyRenderModeVFilter(GXRenderModeObj *rmo, u8 *s)
{
	/*Runtime.PPCEABI.H::*/ memcpy(rmo->vfilter, s, 7);
	return;
}

s32 JDrama::GetVIWidthMax(VITVMode tvm)
{
	switch (tvm)
	{
	case VI_TVMODE_NTSC_INT: // 0
		return 720;
	case VI_TVMODE_NTSC_PROG: // 2
		return 720;
	case VI_TVMODE_PAL_DS: // 5
		return 720;
	}
	return 720;
}

s32 JDrama::GetVIHeightMax(VITVMode tvm)
{
	switch (tvm)
	{
	case VI_TVMODE_NTSC_INT: // 0
		return 574;
	case VI_TVMODE_NTSC_PROG: // 2
		return 480;
	case VI_TVMODE_PAL_DS: // 5
		return 480;
	}
	//	default:
	return 480;
}
