#include <JSystem/JDrama/JDRRenderMode.hpp>
#include <JSystem/JDrama/JDRResolution.hpp>
#include <dolphin/gx.h>
#include <dolphin/os.h>
#include <string.h>

using namespace JDrama;

bool JDrama::IsEqualRenderModeVIParams(const GXRenderModeObj& param_1,
                                       const GXRenderModeObj& param_2)
{
	return param_1.viTVmode == param_2.viTVmode
	       && param_1.fbWidth == param_2.fbWidth
	       && param_1.efbHeight == param_2.efbHeight
	       && param_1.xfbHeight == param_2.xfbHeight
	       && param_1.viXOrigin == param_2.viXOrigin
	       && param_1.viYOrigin == param_2.viYOrigin
	       && param_1.viWidth == param_2.viWidth
	       && param_1.viHeight == param_2.viHeight
	       && param_1.xFBmode == param_2.xFBmode;
}

bool JDrama::IsRenderModeAA2Times(const GXRenderModeObj& rmo)
{
	bool bVar2 = false;
	if (rmo.aa == 1) {
		bool bVar1 = true;
		if (rmo.xFBmode != VI_XFBMODE_DF)
			if (rmo.viTVmode != VI_TVMODE_NTSC_PROG)
				bVar1 = false;

		if (bVar1)
			bVar2 = true;
	}

	return bVar2;
}

bool JDrama::IsRenderModeHalfAspectRatio(const GXRenderModeObj& rmo)
{
	return 2 * rmo.xfbHeight == rmo.viHeight;
}

float JDrama::GetRenderModeYScale(const GXRenderModeObj& rmo)
{
	float fVar3 = 1.0;
	if (IsRenderModeAA2Times(rmo))
		OSPanic("JDRRenderMode.cpp", 0x37, "future not implemented!");
	else
		fVar3 = GXGetYScaleFactor(rmo.efbHeight, rmo.xfbHeight);

	return fVar3;
}

void JDrama::CalcRenderModeXFBHeight(GXRenderModeObj* rmo, u16 param_2)
{
	if (IsRenderModeAA2Times(*rmo))
		OSPanic("JDRRenderMode.cpp", 0x48, "future not implemented!\n");

	int uVar4;
	if (rmo->field_rendering)
		uVar4 = 2;
	else
		uVar4 = 1;

	float yScale = GXGetYScaleFactor(rmo->efbHeight, (u16)(param_2 / uVar4));

	rmo->xfbHeight = GXGetNumXfbLines(rmo->efbHeight, yScale);
	rmo->viHeight  = rmo->xfbHeight * uVar4;
}

void JDrama::CalcRenderModeVIXOrigin(GXRenderModeObj* rmo)
{
	rmo->viXOrigin = (GetVIWidthMax() - rmo->viWidth) / 2;
}

void JDrama::CalcRenderModeVIYOrigin(GXRenderModeObj* rmo)
{
	rmo->viYOrigin = (GetVIHeightMax() - rmo->viHeight) / 2;
}

void JDrama::CopyRenderModeSamplePattern(GXRenderModeObj* rmo,
                                         const u8 (*sample_pattern)[2])
{
	memcpy(rmo->sample_pattern, sample_pattern, 24);
}

void JDrama::CopyRenderModeVFilter(GXRenderModeObj* rmo, const u8* vfilter)
{
	memcpy(rmo->vfilter, vfilter, 7);
}
