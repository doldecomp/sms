#include <JSystem/JDrama/JDREfbSetting.hpp>
#include <JSystem/JDrama/JDRRenderMode.hpp>
#include <dolphin/gx.h>
#include <macros.h>
#include <types.h>

using namespace JDrama;

GXPixelFmt JDrama::DecidePixelFmt(bool antialiasing, bool field_rendering)
{
	return antialiasing ? GX_PF_RGB565_Z16
	                    : (field_rendering ? GX_PF_RGBA6_Z24 : GX_PF_RGB8_Z24);
}

bool JDrama::IsCanEnableDither(GXPixelFmt pix_fmt)
{
	return pix_fmt == GX_PF_RGBA6_Z24 || pix_fmt == GX_PF_RGB565_Z16;
}

void JDrama::IssueGXPixelFormatSetting(bool antialiasing, bool field_rendering,
                                       bool dither, bool field_mode,
                                       bool half_aspect_ratio)
{
	GXPixelFmt pix_fmt
	    = (GXPixelFmt)DecidePixelFmt(antialiasing, field_rendering);
	GXSetPixelFmt(pix_fmt, GX_ZC_LINEAR);
	GXSetDither(dither && IsCanEnableDither(pix_fmt));
	GXSetFieldMode(field_mode, half_aspect_ratio);
}

void JDrama::IssueGXPixelFormatSetting(const GXRenderModeObj& param_1,
                                       bool param_2, bool param_3)
{
	IssueGXPixelFormatSetting(param_1.aa, param_2, param_3,
	                          param_1.field_rendering,
	                          IsRenderModeHalfAspectRatio(param_1));
}

void JDrama::IssueGXSetCopyFilter(bool antialias, const u8 (*sample_pattern)[2],
                                  bool use_vfilter, const u8* vfilter)
{
	GXSetCopyFilter(antialias && sample_pattern != nullptr, sample_pattern,
	                use_vfilter && vfilter != nullptr, vfilter);
}

bool JDrama::IssueGXSetCopyClear(JUtility::TColor clear_color, u32 clear_z,
                                 u16 flags)
{
	bool bVar1 = !(flags & 0x1) || !(flags & 0x2) || !(flags & 0x4);

	if (bVar1) {
		GXSetCopyClear(clear_color, clear_z);
		GXSetColorUpdate((flags & 1) == 0);
		GXSetAlphaUpdate((flags & 2) == 0);

		bool zTest = (flags & 4) == 0;

		GXSetZMode(zTest, GX_ALWAYS, zTest);

		if (zTest)
			GXSetZCompLoc(GX_TRUE);
	}

	return bVar1;
}

void JDrama::IssueGXCopyDisp(void* param_1, const TRect& src_rect,
                             const GXRenderModeObj& render_mode,
                             JUtility::TColor clear_color, u32 clear_z,
                             GXFBClamp framebuffer_clamp, u16 flags)
{
	GXSetCopyClamp(framebuffer_clamp);
	IssueGXSetCopyFilter(render_mode.aa, render_mode.sample_pattern,
	                     flags & 0x20, render_mode.vfilter);
	bool doClear = IssueGXSetCopyClear(clear_color, clear_z, flags);

	GXSetDispCopySrc(src_rect.x1, src_rect.y1, src_rect.x2, src_rect.y2);
	u32 uVar2 = GXSetDispCopyYScale(GetRenderModeYScale(render_mode));
	GXSetDispCopyDst(ALIGN_NEXT(render_mode.fbWidth, 16), uVar2);
	GXCopyDisp(param_1, doClear);
}
