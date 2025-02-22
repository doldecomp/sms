#ifndef JDR_EFB_SETTING_HPP
#define JDR_EFB_SETTING_HPP

#include <JSystem/JDrama/JDRGraphics.hpp>
#include <JSystem/JUtility/JUTColor.hpp>

namespace JDrama {

GXPixelFmt DecidePixelFmt(bool, bool);
bool IsCanEnableDither(GXPixelFmt);
void IssueGXPixelFormatSetting(bool, bool, bool, bool, bool);
void IssueGXPixelFormatSetting(const GXRenderModeObj&, bool, bool);
void IssueGXSetCopyFilter(bool, const u8 (*)[2], bool, const u8*);
bool IssueGXSetCopyClear(JUtility::TColor, u32, u16);
void IssueGXCopyDisp(void*, const TRect&, const GXRenderModeObj&,
                     JUtility::TColor, u32, GXFBClamp, u16);

} // namespace JDrama

#endif
