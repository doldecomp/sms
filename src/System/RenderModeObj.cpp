#include "dolphin/os/OSRtc.h"
#include <System/RenderModeObj.hpp>
#include <dolphin/vi.h>
#include <JSystem/JDrama/JDRRenderMode.hpp>
#include <System/Resolution.hpp>

u8 SMSAASamplePattern_non[12][2] = {
	{ 6, 6 }, { 6, 6 }, { 6, 6 }, { 6, 6 }, { 6, 6 }, { 6, 6 },
	{ 6, 6 }, { 6, 6 }, { 6, 6 }, { 6, 6 }, { 6, 6 }, { 6, 6 },
};
u8 SMSAASamplePattern_aa[12][2] = {
	{ 0x03, 0x02 }, { 0x09, 0x06 }, { 0x03, 0x0a }, { 0x03, 0x02 },
	{ 0x09, 0x06 }, { 0x03, 0x0a }, { 0x09, 0x02 }, { 0x03, 0x06 },
	{ 0x09, 0x0a }, { 0x09, 0x02 }, { 0x03, 0x06 }, { 0x09, 0x0a },
};
u8 SMSVFilter_non[7]     = { 0x0, 0x0, 0x15, 0x16, 0x15, 0x0, 0x0 };
u8 SMSVFilter_flicker[7] = { 0x8, 0x8, 0xA, 0xC, 0xA, 0x8, 0x8 };

// All four SMSSetup*RenderingInfo functions are instruction-exact and only
// 8 bytes short of retail's frame (0x18/0x28/0x20/0x28 vs 0x20/0x30/0x28/0x38);
// `volatile char trash[8]` in each body takes all four to 100%, so the bodies
// are correct.
//
// TODO: the missing object lives in JDrama::TDisplay::getRenderMode(), a header
// inline with no map symbol (so it is invisible to validate-symbol-order, the
// same shape as MSound::startSoundActor).  Measured: one dead 4-byte
// non-trivial local there (`struct { ~S() {} u32 v; }`) closes all four
// functions with **zero** instruction change and zero regressions anywhere else
// in the tree (`ninja changes_all`: +4 functions to 100%, unit matched_code
// 25.57% -> 100%, total matched_code 48.00 -> 48.03).  getRenderMode() is the
// unique carrier: the emitted out-of-line SMSSetupTitleRenderMode (frame 0x20,
// no locals) is 100% and does not call it, and the same probe local placed in
// TDisplay::on/offFlag() instead lands only Title and Movie (GCLogo does not
// move at all).  A real JDrama::TFlagT<u16> temporary in on/offFlag
// (`unk64 = TFlagT<u16>(unk64.mValue | flag)`) closes GCLogo/Title/Movie with no
// instruction change but leaves Game at 0x30.
// What the 4-byte object *is* remains unnamed, so the header edit is left to a
// header round rather than fabricated here.

JDrama::TRect SMSGetRederRect_Game()
{
	return JDrama::TRect(0, 0, (u16)SMSGetGameRenderWidth(),
	                     (u16)SMSGetGameRenderHeight());
}

void SMSSetupGCLogoRenderMode(GXRenderModeObj* rmo)
{
	rmo->viTVmode        = (VITVMode)VI_TVMODE(VIGetTvFormat(), VI_INTERLACE);
	rmo->fbWidth         = SMSGetGCLogoRenderWidth();
	rmo->efbHeight       = SMSGetGCLogoRenderHeight();
	rmo->viWidth         = SMSGetGCLogoVideoWidth();
	rmo->xFBmode         = VI_XFBMODE_DF;
	rmo->field_rendering = 0;
	rmo->aa              = 0;
	JDrama::CalcRenderModeXFBHeight(rmo, SMSGetGCLogoVideoHeight());
	JDrama::CalcRenderModeVIXOrigin(rmo);
	JDrama::CalcRenderModeVIYOrigin(rmo);
	JDrama::CopyRenderModeSamplePattern(rmo, SMSAASamplePattern_non);
	JDrama::CopyRenderModeVFilter(rmo, SMSVFilter_flicker);
}

void SMSSetupGCLogoRenderingInfo(JDrama::TDisplay* param_1)
{
	SMSSetupGCLogoRenderMode(&param_1->getRenderMode());
	param_1->offFlag(0x8);
}

void SMSSetupTitleRenderMode(GXRenderModeObj* rmo)
{
	bool noFilter = VIGetTvFormat() == 0 && OSGetProgressiveMode() == 1;

	rmo->viTVmode = (VITVMode)VI_TVMODE(
	    VIGetTvFormat(), noFilter ? VI_PROGRESSIVE : VI_INTERLACE);
	rmo->fbWidth         = SMSGetTitleRenderWidth();
	rmo->efbHeight       = SMSGetTitleRenderHeight();
	rmo->viWidth         = SMSGetTitleVideoWidth();
	rmo->xFBmode         = noFilter ? VI_XFBMODE_SF : VI_XFBMODE_DF;
	rmo->field_rendering = 0;
	rmo->aa              = 0;
	JDrama::CalcRenderModeXFBHeight(rmo, SMSGetTitleVideoHeight());
	JDrama::CalcRenderModeVIXOrigin(rmo);
	JDrama::CalcRenderModeVIYOrigin(rmo);
	JDrama::CopyRenderModeSamplePattern(rmo, SMSAASamplePattern_non);
	JDrama::CopyRenderModeVFilter(rmo, noFilter ? SMSVFilter_non
	                                            : SMSVFilter_flicker);
}

void SMSSetupTitleRenderingInfo(JDrama::TDisplay* param_1)
{
	SMSSetupTitleRenderMode(&param_1->getRenderMode());
	param_1->offFlag(0x8);
}

void SMSSetupGameRenderingInfo(JDrama::TDisplay* param_1, bool param_2)
{
	GXRenderModeObj& rmo = param_1->getRenderMode();

	bool noFilter = VIGetTvFormat() == 0 && OSGetProgressiveMode() == 1;

	rmo.viTVmode = (VITVMode)VI_TVMODE(
	    VIGetTvFormat(), noFilter ? VI_PROGRESSIVE : VI_INTERLACE);

	param_1->onFlag(0x8);
	if (param_2) {
		rmo.xFBmode         = VI_XFBMODE_SF;
		rmo.field_rendering = 1;
	} else {
		rmo.xFBmode         = noFilter ? VI_XFBMODE_SF : VI_XFBMODE_DF;
		rmo.field_rendering = 0;
	}
	rmo.fbWidth   = SMSGetGameRenderWidth();
	rmo.efbHeight = SMSGetGameRenderHeight();
	rmo.viWidth   = SMSGetGameVideoWidth();

	JDrama::CalcRenderModeXFBHeight(&rmo, SMSGetGameVideoHeight());
	JDrama::CalcRenderModeVIXOrigin(&rmo);
	JDrama::CalcRenderModeVIYOrigin(&rmo);
	if (param_2 || noFilter) {
		JDrama::CopyRenderModeVFilter(&rmo, SMSVFilter_non);
	} else {
		JDrama::CopyRenderModeVFilter(&rmo, SMSVFilter_flicker);
	}

	rmo.aa = 0;
	JDrama::CopyRenderModeSamplePattern(&rmo, SMSAASamplePattern_non);
}

void SMSSetupMovieRenderingInfo(JDrama::TDisplay* param_1)
{
	GXRenderModeObj& rmo = param_1->getRenderMode();

	bool noFilter = VIGetTvFormat() == 0 && OSGetProgressiveMode() == 1;

	rmo.viTVmode = (VITVMode)VI_TVMODE(
	    VIGetTvFormat(), noFilter ? VI_PROGRESSIVE : VI_INTERLACE);
	rmo.fbWidth         = SMSGetGameRenderWidth();
	rmo.efbHeight       = SMSGetGameRenderHeight();
	rmo.viWidth         = SMSGetGameVideoWidth();
	rmo.xFBmode         = noFilter ? VI_XFBMODE_SF : VI_XFBMODE_DF;
	rmo.field_rendering = 0;
	rmo.aa              = 0;
	JDrama::CalcRenderModeXFBHeight(&rmo, SMSGetGameVideoHeight());
	JDrama::CalcRenderModeVIXOrigin(&rmo);
	JDrama::CalcRenderModeVIYOrigin(&rmo);
	JDrama::CopyRenderModeSamplePattern(&rmo, SMSAASamplePattern_non);
	JDrama::CopyRenderModeVFilter(&rmo, SMSVFilter_non);
	param_1->offFlag(0x8);
}
