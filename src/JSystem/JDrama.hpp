#include <dolphin.h>


namespace JDrama
{

/* JDRRenderMode: */
	bool IsEqualRenderModeVIParams(const GXRenderModeObj &, const GXRenderModeObj &);
	bool IsRenderModeHalfAspectRatio(const GXRenderModeObj &);
	f32 GetRenderModeYScale(const GXRenderModeObj &);
	void CalcRenderModeXFBHeight(GXRenderModeObj *, u16);
	void CalcRenderModeVIXOrigin(GXRenderModeObj *rmo);
	void CalcRenderModeVIYOrigin(GXRenderModeObj *rmo);
	void CopyRenderModeSamplePattern(GXRenderModeObj *rmo, const u8 (*s)[2]);
	void CopyRenderModeVFilter(GXRenderModeObj *rmo, const u8 *s);

/* JDRResolution: */
	s32 GetVIWidthMax(VITVMode tvm);
	s32 GetVIHeightMax(VITVMode tvm);
} // namespace JDrama
