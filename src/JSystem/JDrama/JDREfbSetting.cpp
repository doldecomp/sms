#include <JSystem/JDrama/JDREfbSetting.hpp>
#include <JSystem/JDrama/JDRRenderMode.hpp>
#include <dolphin/gx.h>
#include <macros.h>
#include <types.h>

using namespace JDrama;

GXPixelFmt JDrama::DecidePixelFmt(bool antialiasing, bool field_rendering)
{
	GXPixelFmt fmt;
	if (antialiasing) {
		fmt = GX_PF_RGB565_Z16;
	} else if (field_rendering) {
		fmt = GX_PF_RGBA6_Z24;
	} else {
		fmt = GX_PF_RGB8_Z24;
	}
	return fmt;
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

// TODO: 95 of 101 instructions and the 0x50 frame are exact; the residue is a
// six-instruction scratch-register permutation inside the inlined
// `IssueGXSetCopyFilter`, where the *dead* `flags & 0x20` value and the
// `render_mode.aa` byte swap homes: retail is `lbz r0, 0x19(r28)` /
// `rlwinm. r4, r25, ...` and materialises the bool through r3/r0, ours is
// `lbz r3` / `rlwinm. r0` through r4/r3. Both `rlwinm.` results are unused
// (only CR0 is read), so this is allocator ranking, not a value.
// `IssueGXSetCopyFilter`'s own out-of-line copy is byte-exact, which pins its
// body: named `aa`/`vf` bools drop it to 93-94.5%, reversing the `&&` operands
// to 70%, and neither fixes the call site. Dropping the `!= nullptr` tests and
// spelling the third argument `(flags & 0x20) != 0` or through a named `bool`
// are codegen-identical here.
// Closure re-pass 2026-09-18: the new register rule is about callee-saved
// GPRs and this is a scratch permutation, so it does not apply. Two more
// call-site trials measured: a named `bool useVFilter = flags & 0x20` grows the
// frame to 0x58 (11 operands), and a named `bool antialias = render_mode.aa`
// is byte-identical to the current spelling (6 operands). Still open.
// Library re-pass 2026-09-18: two more call-site conversions of the first
// argument are refuted -- `render_mode.aa != 0` is 98.0% (it turns the
// normalisation into an opcode difference, 1 `|` and 1 `<`) and
// `(bool)render_mode.aa` is byte-identical to the plain member read. What the
// permutation really is: retail spends r0 on the live `aa` byte and coalesces
// the *dead* `flags & 0x20` result into r4, the register `sample_pattern` is
// loaded into at 0x348; we spend r0 on the dead value and run the bool chain
// through r3/r4. Nothing at the call site reaches that coalescing.
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
