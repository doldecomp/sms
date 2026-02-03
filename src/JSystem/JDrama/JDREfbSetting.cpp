#include <JSystem/JDrama/JDREfbSetting.hpp>
#include <JSystem/JDrama/JDRRenderMode.hpp>
#include <dolphin/gx.h>
#include <types.h>

using namespace JDrama;

GXPixelFmt JDrama::DecidePixelFmt(bool param_1, bool param_2)
{
	if (param_1)
		return GX_PF_RGB565_Z16;
	else if (param_2)
		return GX_PF_RGBA6_Z24;
	else
		return GX_PF_RGB8_Z24;
}

bool JDrama::IsCanEnableDither(GXPixelFmt pix_fmt)
{
	bool res = true;
	if (!(pix_fmt == GX_PF_RGBA6_Z24 || pix_fmt == GX_PF_RGB565_Z16))
		res = false;
	return res;
}

void JDrama::IssueGXPixelFormatSetting(bool param_1, bool param_2, bool param_3,
                                       bool param_4, bool param_5)
{

	GXPixelFmt pix_fmt = DecidePixelFmt(param_1, param_2);
	GXSetPixelFmt(pix_fmt, GX_ZC_LINEAR);

	GXBool dither = false;
	if (param_3 && IsCanEnableDither(pix_fmt))
		dither = true;

	GXSetDither(dither);
	GXSetFieldMode(param_4, param_5);
}

void JDrama::IssueGXPixelFormatSetting(const GXRenderModeObj& param_1,
                                       bool param_2, bool param_3)
{
	GXBool half_aspect_ratio = IsRenderModeHalfAspectRatio(param_1);

	bool uVar1 = !!param_1.field_rendering;

	GXPixelFmt pix_fmt = DecidePixelFmt(!!param_1.aa, param_2);

	GXSetPixelFmt(pix_fmt, GX_ZC_LINEAR);
	GXBool dither = false;
	if (param_3 && IsCanEnableDither(pix_fmt))
		dither = true;

	GXSetDither(dither);

	GXSetFieldMode(uVar1, half_aspect_ratio);
}

void JDrama::IssueGXSetCopyFilter(bool antialias, const u8 (*sample_pattern)[2],
                                  bool use_vfilter, const u8* vfilter)
{
	GXBool vf = GX_FALSE;
	if (use_vfilter && vfilter != nullptr) {
		vf = GX_TRUE;
	}

	GXBool aa = GX_FALSE;
	if (antialias && sample_pattern != nullptr) {
		aa = GX_TRUE;
	}

	GXSetCopyFilter(aa, sample_pattern, vf, vfilter);
}

bool JDrama::IssueGXSetCopyClear(JUtility::TColor clear_color, u32 clear_z,
                                 u16 flags)
{
	bool iVar2 = true;

	bool bVar1 = true;
	if (((flags & 1) != 0) && ((flags & 2) != 0)) {
		bVar1 = false;
	}

	if (!bVar1 && ((flags & 4) != 0)) {
		iVar2 = false;
	}

	if (iVar2) {
		GXSetCopyClear(clear_color, clear_z);
		GXSetColorUpdate((flags & 1) == 0);
		GXSetAlphaUpdate((flags & 2) == 0);

		bool zTest = (flags & 4) == 0;

		GXSetZMode(zTest, GX_ALWAYS, zTest);

		if (zTest)
			GXSetZCompLoc(GX_TRUE);
	}

	return iVar2;
}

void JDrama::IssueGXCopyDisp(void* param_1, const TRect& src_rect,
                             const GXRenderModeObj& render_mode,
                             JUtility::TColor clear_color, u32 clear_z,
                             GXFBClamp framebuffer_clamp, u16 flags)
{
	GXSetCopyClamp(framebuffer_clamp);
	IssueGXSetCopyFilter(render_mode.aa, render_mode.sample_pattern, flags & 20,
	                     render_mode.vfilter);
	bool doClear = IssueGXSetCopyClear(clear_color, clear_z, flags);

	GXSetDispCopySrc(src_rect.x1, src_rect.y1, src_rect.x2, src_rect.y2);
	u32 uVar2 = GXSetDispCopyYScale(GetRenderModeYScale(render_mode));
	GXSetDispCopyDst(render_mode.fbWidth + 0xf & 0xfffffff0, uVar2);
	GXCopyDisp(param_1, doClear);
}
