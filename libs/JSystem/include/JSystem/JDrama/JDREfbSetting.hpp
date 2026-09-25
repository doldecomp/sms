#ifndef JDR_EFB_SETTING_HPP
#define JDR_EFB_SETTING_HPP

#include <JSystem/JDrama/JDRRect.hpp>
#include <JSystem/JUtility/JUTColor.hpp>

namespace JDrama {

GXPixelFmt DecidePixelFmt(bool, bool);
bool IsCanEnableDither(GXPixelFmt);
void IssueGXPixelFormatSetting(bool, bool, bool, bool, bool);
void IssueGXPixelFormatSetting(const GXRenderModeObj&, bool, bool);
void IssueGXSetCopyFilter(bool antialias, const u8 (*sample_pattern)[2],
                          bool use_vfilter, const u8* vfilter);
bool IssueGXSetCopyClear(JUtility::TColor clear_color, u32 clear_z, u16 flags);
void IssueGXCopyDisp(void* param_1, const TRect& src_rect,
                     const GXRenderModeObj& render_mode,
                     JUtility::TColor clear_color, u32 clear_z,
                     GXFBClamp framebuffer_clamp, u16 flags);

} // namespace JDrama

#endif
