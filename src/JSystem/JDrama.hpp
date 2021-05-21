#include "JSystem/JDRCommon.h"


extern "C"
{
void OSPanic(const char *file, int line, const char *msg, ...);
#define OSHalt(msg) OSPanic(__FILE__, __LINE__, msg)

f32 GXGetYScaleFactor(u16 efbHeight, u16 xfbHeight);
u16 GXGetNumXfbLines(u16 efbHeight, f32 yScale);
}

namespace JDrama
{

/* JDRRenderMode: */
#if 1
	bool IsEqualRenderModeVIParams(const GXRenderModeObj &, const GXRenderModeObj &);
	bool IsRenderModeHalfAspectRatio(const GXRenderModeObj &);
	f32 GetRenderModeYScale(const GXRenderModeObj &);
	void CalcRenderModeXFBHeight(GXRenderModeObj *, u16);
#endif
	void CalcRenderModeVIXOrigin(GXRenderModeObj *rmo);
	void CalcRenderModeVIYOrigin(GXRenderModeObj *rmo);
	void CopyRenderModeSamplePattern(GXRenderModeObj *rmo, const u8 (*s)[2]);
	void CopyRenderModeVFilter(GXRenderModeObj *rmo, const u8 *s);

/* JDRResolution: */
	s32 GetVIWidthMax(VITVMode tvm);
	s32 GetVIHeightMax(VITVMode tvm);
} // namespace JDrama

#ifndef _EU

// JP version
extern "C"
{
VITVMode VIGetTvFormat(void);
}

#endif
