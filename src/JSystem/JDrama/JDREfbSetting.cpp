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
	if (pix_fmt - 1 > 1u)
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

void JDrama::IssueGXSetCopyFilter(bool param_1, const u8 (*param_2)[2],
                                  bool param_3, const u8* param_4)
{
	GXBool vf = GX_FALSE;
	if (param_3 && param_4 != nullptr) {
		vf = GX_TRUE;
	}

	GXBool aa = GX_FALSE;
	if (param_1 && param_2 != nullptr) {
		aa = GX_TRUE;
	}

	GXSetCopyFilter(aa, param_2, vf, param_4);
}

bool JDrama::IssueGXSetCopyClear(JUtility::TColor param_1, u32 param_2,
                                 u16 param_3)
{
	bool iVar2 = true;

	bool bVar1 = true;
	if (((param_3 & 1) != 0) && ((param_3 & 2) != 0)) {
		bVar1 = false;
	}

	if (!bVar1 && ((param_3 & 4) != 0)) {
		iVar2 = false;
	}

	if (iVar2) {
		GXSetCopyClear(param_1, param_2);
		GXSetColorUpdate((param_3 & 1) == 0);
		GXSetAlphaUpdate((param_3 & 2) == 0);

		bool zTest = (param_3 & 4) == 0;

		GXSetZMode(zTest, GX_ALWAYS, zTest);

		if (zTest)
			GXSetZCompLoc(GX_TRUE);
	}

	return iVar2;
}

void JDrama::IssueGXCopyDisp(void* param_1, const TRect& param_2,
                             const GXRenderModeObj& param_3,
                             JUtility::TColor param_4, u32 param_5,
                             GXFBClamp param_6, u16 param_7)
{
	IssueGXSetCopyFilter(param_3.aa, param_3.sample_pattern, param_7 & 20,
	                     param_3.vfilter);
	bool iVar3 = IssueGXSetCopyClear(param_4, param_5, param_7);

	GXSetDispCopySrc(param_2.x1, param_2.y1, param_2.x2, param_2.y2);
	u32 uVar2 = GXSetDispCopyYScale(GetRenderModeYScale(param_3));
	GXSetDispCopyDst(param_3.fbWidth + 0xf & 0xfffffff0, uVar2);
	GXCopyDisp(param_1, iVar3);
}
