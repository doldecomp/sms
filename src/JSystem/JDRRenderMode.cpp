#include "JSystem/JDrama.hpp"
#include <cstring>

#if 0
bool JDrama::IsEqualRenderModeVIParams(const GXRenderModeObj &, const GXRenderModeObj &)
{
	return false;
}

bool JDrama::IsRenderModeHalfAspectRatio(const GXRenderModeObj &)
{
	return false;
}
#endif

f32 JDrama::GetRenderModeYScale(const GXRenderModeObj &rmo)
{
	f32 ret = 1.0f;
	bool first = false;
	if (rmo.aa == 1)
	{
		bool second = true;
		if (rmo.xFBmode != VI_XFBMODE_DF)
			if (rmo.viTVmode != VI_TVMODE_NTSC_PROG)
				second = false;
		if (second)
			first = true;
	}
	if (first)
	{
#line 55 "JDRRenderMode.cpp"
		OSHalt("future not implemented!");
		return ret;
	}
	ret = static_cast<u16>(GXGetYScaleFactor(rmo.efbHeight, rmo.xfbHeight));
	return ret;
}

void JDrama::CalcRenderModeXFBHeight(GXRenderModeObj *rmo, u16 p2)
{
	bool first = false;
	if (rmo->aa == 1)
	{
		bool second = true;
		if (rmo->xFBmode != VI_XFBMODE_DF)
			if (rmo->viTVmode != VI_TVMODE_NTSC_PROG)
				second = false;
		if (second)
			first = true;
	}

	if (first)
#line 72 "JDRRenderMode.cpp"
		OSHalt("future not implemented!\n");

	s32 fr = (rmo->field_rendering != 0) ? 2 : 1;
	u16 fac = GXGetYScaleFactor(rmo->efbHeight, p2 / fr);

	rmo->xfbHeight = GXGetNumXfbLines(fac, rmo->efbHeight);
	rmo->viHeight = rmo->xfbHeight * fr;

	return;
}

void JDrama::CalcRenderModeVIXOrigin(GXRenderModeObj *rmo)
{
	u32 dif = static_cast<u16>(JDrama::GetVIWidthMax(rmo)) - rmo->viWidth;
	rmo->viXOrigin = (((s32)dif / 2) + ((dif < 0) && ((dif & 1) != 0)));
}

void JDrama::CalcRenderModeVIYOrigin(GXRenderModeObj *rmo)
{
	u32 dif = static_cast<u16>(JDrama::GetVIHeightMax(rmo)) - rmo->viHeight;
	rmo->viYOrigin = (((s32)dif / 2) + ((dif < 0) && ((dif & 1) != 0)));
}

void JDrama::CopyRenderModeSamplePattern(GXRenderModeObj *rmo, const u8 (*s)[2])
{
	memcpy(rmo->sample_pattern,
		   s,
		   sizeof(rmo->sample_pattern));
}

void JDrama::CopyRenderModeVFilter(GXRenderModeObj *rmo, const u8 *s)
{
	memcpy(rmo->vfilter,
		   s,
		   sizeof(rmo->vfilter));
}
