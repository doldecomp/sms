#ifndef JDR_RENDER_MODE_HPP
#define JDR_RENDER_MODE_HPP

#include <dolphin/gx/GXStruct.h>

namespace JDrama {

bool IsEqualRenderModeVIParams(const GXRenderModeObj&, const GXRenderModeObj&);
bool IsRenderModeAA2Times(const GXRenderModeObj&);
bool IsRenderModeHalfAspectRatio(const GXRenderModeObj&);

float GetRenderModeYScale(const GXRenderModeObj&);

void CalcRenderModeXFBHeight(GXRenderModeObj*, u16);
void CalcRenderModeVIXOrigin(GXRenderModeObj*);
void CalcRenderModeVIYOrigin(GXRenderModeObj*);

void CopyRenderModeSamplePattern(GXRenderModeObj*, const u8 (*)[2]);
void CopyRenderModeVFilter(GXRenderModeObj*, const u8*);

} // namespace JDrama

#endif
