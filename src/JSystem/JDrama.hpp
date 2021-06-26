#include <dolphin.h>


namespace JDrama
{
//* JDRRenderMode:
	bool IsEqualRenderModeVIParams(const GXRenderModeObj &rmo1, const GXRenderModeObj &rmo2);
	bool IsRenderModeHalfAspectRatio(const GXRenderModeObj &rmo);
	f32 GetRenderModeYScale(const GXRenderModeObj &rmo);
	void CalcRenderModeXFBHeight(GXRenderModeObj *rmo, u16);
	void CalcRenderModeVIXOrigin(GXRenderModeObj *rmo);
	void CalcRenderModeVIYOrigin(GXRenderModeObj *rmo);
	void CopyRenderModeSamplePattern(GXRenderModeObj *rmo, const u8 (*s)[2]);
	void CopyRenderModeVFilter(GXRenderModeObj *rmo, const u8 *s);

//* JDRResolution: 
	s32 GetVIWidthMax(void);
	s32 GetVIHeightMax(void);

};
