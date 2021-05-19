#ifndef __JDR_RENDER_MODE_HPP__
#define __JDR_RENDER_MODE_HPP__

namespace JDrama
{
#if 0
	bool IsEqualRenderModeVIParams(const GXRenderModeObj &, const GXRenderModeObj &);
	bool IsRenderModeHalfAspectRatio(const GXRenderModeObj &);
	f32 GetRenderModeYScale(const GXRenderModeObj &);
	void CalcRenderModeXFBHeight(GXRenderModeObj *, u16);
#endif
	void CalcRenderModeVIXOrigin(GXRenderModeObj *rmo);
	void CalcRenderModeVIYOrigin(GXRenderModeObj *rmo);
	void CopyRenderModeSamplePattern(GXRenderModeObj *rmo, const u8 *s);
	void CopyRenderModeVFilter(GXRenderModeObj *rmo, const u8 *s);

	s32 GetVIWidthMax(VITVMode tvm);
	s32 GetVIHeightMax(VITVMode tvm);
}

#endif /*__JDR_RENDER_MODE_HPP__*/
