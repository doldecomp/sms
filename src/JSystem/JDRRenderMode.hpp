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
	void CalcRenderModeVIXOrigin(GXRenderModeObj *);
	void CalcRenderModeVIYOrigin(GXRenderModeObj *);
	void CopyRenderModeSamplePattern(GXRenderModeObj *, const u8 *[2]);
	void CopyRenderModeVFilter(GXRenderModeObj *, const u8 *);

	s32 GetVIWidthMax(VITVMode);
	s32 GetVIHeightMax(VITVMode);

}

#endif /*__JDR_RENDER_MODE_HPP__*/
