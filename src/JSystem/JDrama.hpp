#include <dolphin.h>

/* VIGetTvFormat has been refactored in the Dolphin SDK, so this is a workaround for now */
extern "C" VITVMode _VIGetTvFormat(GXRenderModeObj *);
#define VIGetTvFormat _VIGetTvFormat


namespace JDrama
{

/* JDRRenderMode: */
	/* TODO */bool IsEqualRenderModeVIParams(const GXRenderModeObj &, const GXRenderModeObj &);
	/* TODO */bool IsRenderModeHalfAspectRatio(const GXRenderModeObj &);

	f32 GetRenderModeYScale(const GXRenderModeObj &rmo);
	void CalcRenderModeXFBHeight(GXRenderModeObj *rmo, u16);
	void CalcRenderModeVIXOrigin(GXRenderModeObj *rmo);
	void CalcRenderModeVIYOrigin(GXRenderModeObj *rmo);
	void CopyRenderModeSamplePattern(GXRenderModeObj *rmo, const u8 (*s)[2]);
	void CopyRenderModeVFilter(GXRenderModeObj *rmo, const u8 *s);

/* JDRResolution: */
	s32 GetVIWidthMax(GXRenderModeObj *rmo);
	s32 GetVIHeightMax(GXRenderModeObj *rmo);

}
