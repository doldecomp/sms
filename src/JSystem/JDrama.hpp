#include <dolphin.h>


namespace JDrama
{
//* JDRRenderMode:
	/* TODO */bool IsEqualRenderModeVIParams(const GXRenderModeObj &, const GXRenderModeObj &);
	/* TODO */bool IsRenderModeHalfAspectRatio(const GXRenderModeObj &);
	/* TODO */f32 GetRenderModeYScale(const GXRenderModeObj &rmo);
	/* TODO */void CalcRenderModeXFBHeight(GXRenderModeObj *rmo, u16);

	void CalcRenderModeVIXOrigin(GXRenderModeObj *rmo);
	void CalcRenderModeVIYOrigin(GXRenderModeObj *rmo);
	void CopyRenderModeSamplePattern(GXRenderModeObj *rmo, const u8 (*s)[2]);
	void CopyRenderModeVFilter(GXRenderModeObj *rmo, const u8 *s);

//* JDRResolution: 
	s32 GetVIWidthMax(void);
	s32 GetVIHeightMax(void);

};
