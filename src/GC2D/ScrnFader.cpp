#include <GC2D/ScrnFader.hpp>
#include <GC2D/hx_wiper.h>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <dolphin/gx.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

static const char* dummyMactorStringValue1 = "\0\0\0\0\0\0\0\0\0\0\0";
static const char* SMS_NO_MEMORY_MESSAGE   = "メモリが足りません\n";

namespace {

void fill_rect(const JDrama::TRect& param_1, JUtility::TColor param_2)
{
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
	GXSetNumChans(1);
	GXSetNumTexGens(0);
	GXSetNumTevStages(1);
	GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);

	if (param_2.a == 0xff)
		GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_NOOP);
	else
		GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA,
		               GX_LO_NOOP);

	u32 color = param_2;

	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition3s16(param_1.x1, param_1.y1, 0);
	GXColor1u32(color);
	GXPosition3s16(param_1.x2, param_1.y1, 0);
	GXColor1u32(color);
	GXPosition3s16(param_1.x2, param_1.y2, 0);
	GXColor1u32(color);
	GXPosition3s16(param_1.x1, param_1.y2, 0);
	GXColor1u32(color);
	GXEnd();
}

void draw_wipe_box(const JDrama::TRect& param_1, JUtility::TColor param_2)
{
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
	GXSetNumChans(1);
	GXSetNumTexGens(0);
	GXSetNumTevStages(1);
	GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);

	if (param_2.a == 0xff)
		GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_NOOP);
	else
		GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA,
		               GX_LO_NOOP);

	f32 w = param_2.a * (param_1.getWidth() >> 1) / 255.0f;
	f32 h = param_2.a * (param_1.getHeight() >> 1) / 255.0f;

	JUTRect local_4c(param_1.x1 + int(w), param_1.y1 + int(h),
	                 param_1.x2 - int(w), param_1.y2 - int(h));

	u32 color = 0xff;
	GXBegin(GX_QUADS, GX_VTXFMT0, 0x10);
	GXPosition3s16(param_1.x1, param_1.y1, 0);
	GXColor1u32(color);

	GXPosition3s16(local_4c.x2, param_1.y1, 0);
	GXColor1u32(color);

	GXPosition3s16(local_4c.x2, local_4c.y1, 0);
	GXColor1u32(color);

	GXPosition3s16(param_1.x1, local_4c.y1, 0);
	GXColor1u32(color);

	GXPosition3s16(local_4c.x2, param_1.y1, 0);
	GXColor1u32(color);

	GXPosition3s16(param_1.x2, param_1.y1, 0);
	GXColor1u32(color);

	GXPosition3s16(param_1.x2, local_4c.y2, 0);
	GXColor1u32(color);

	GXPosition3s16(local_4c.x2, local_4c.y2, 0);
	GXColor1u32(color);

	GXPosition3s16(local_4c.x1, local_4c.y2, 0);
	GXColor1u32(color);

	GXPosition3s16(param_1.x2, local_4c.y2, 0);
	GXColor1u32(color);

	GXPosition3s16(param_1.x2, param_1.y2, 0);
	GXColor1u32(color);

	GXPosition3s16(local_4c.x1, param_1.y2, 0);
	GXColor1u32(color);

	GXPosition3s16(param_1.x1, local_4c.y1, 0);
	GXColor1u32(color);

	GXPosition3s16(local_4c.x1, local_4c.y1, 0);
	GXColor1u32(color);

	GXPosition3s16(local_4c.x1, param_1.y2, 0);
	GXColor1u32(color);

	GXPosition3s16(param_1.x1, param_1.y2, 0);
	GXColor1u32(color);

	GXEnd();
}

} // namespace

TSMSFader::TSMSFader(JUtility::TColor param_1, f32 param_2, const char* param_3)
    : JDrama::TViewObj(param_3)
    , unk10(0)
    , unk12(0)
    , mRate(param_2)
    , mFadeColor(param_1)
    , unk1C(false)
    , mFadeStatus(FADE_STATUS_FULLY_FADED_OUT)
    , unk30(UNK30_UNK_14)
    , unk34(0.0f)
{
	Hx_ResetWipe(640, 480);
	mWipeRequest.unk0 = UNK30_UNK_18;
}

void TSMSFader::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (param_1 & 0x1)
		update();

	if (param_1 & 0x8)
		draw(JDrama::TRect(0, 0, 640, 480));
}

void TSMSFader::update()
{
	updateRequest();

	if (mFadeStatus == TSMSFader::FADE_STATUS_FULLY_FADED_OUT)
		mFadeColor.a = 0xff;

	if (unk30 >= UNK30_UNK_18 || unk30 < UNK30_UNK_14)
		return;

	updateFadeinout();
}

void TSMSFader::updateRequest()
{
	if (mWipeRequest.unk0 == UNK30_UNK_18)
		return;

	f32 fVar1 = mWipeRequest.unk8 - 1.0f / mRate;
	if (fVar1 < 0.0f)
		fVar1 = 0.0f;
	mWipeRequest.unk8 = fVar1;

	if (fVar1 <= 0.0f) {
		requestWipe(&mWipeRequest);
		mWipeRequest.unk0 = UNK30_UNK_18;
	}
}

// TODO: What's this one?
void TSMSFader::updateDelay() { }

void TSMSFader::updateFadeinout()
{
	switch (mFadeStatus) {
	case FADE_STATUS_FULLY_FADED_OUT:
		mFadeColor.a = 0xff;
		break;
	case FADE_STATUS_FADING_IN:
		mFadeColor.a = 0xff - (unk12++ * 0xff) / (unk10 + 1);
		if (unk12 > unk10)
			mFadeStatus = FADE_STATUS_FULLY_FADED_IN;
		break;
	case FADE_STATUS_FADING_OUT:
		mFadeColor.a = (++unk12 * 0xff) / (unk10 + 1);
		if (unk12 > unk10)
			mFadeStatus = FADE_STATUS_FULLY_FADED_OUT;
		break;
	}
}

void TSMSFader::draw(const JDrama::TRect& param_1)
{
	switch (mFadeStatus) {
	case FADE_STATUS_FULLY_FADED_OUT: {
		setupGraphicsFadeinout();
		JUtility::TColor color = mFadeColor;
		color.a                = 0xff;
		fill_rect(param_1, color);
		break;
	}

	case FADE_STATUS_FULLY_FADED_IN:
		break;

	default:
		drawWipe(param_1);
	}
}

void TSMSFader::drawWipe(const JDrama::TRect& param_1)
{
	switch (unk30) {
	case UNK30_UNK_14:
	case UNK30_UNK_15:
	case UNK30_UNK_16:
	case UNK30_UNK_17:
		drawFadeinout(param_1);
		break;
	default:
		if (Hx_UpdateWipe(mRate) == 3)
			mFadeStatus = Hx_GetWipeType(unk30) != 0
			                  ? FADE_STATUS_FULLY_FADED_IN
			                  : FADE_STATUS_FULLY_FADED_OUT;
		break;
	}
}

void TSMSFader::drawFadeinout(const JDrama::TRect& param_1)
{
	switch (unk30) {
	case UNK30_UNK_14:
	case UNK30_UNK_15: {
		if (mFadeColor.a == 0)
			break;

		setupGraphicsFadeinout();
		fill_rect(param_1, mFadeColor);
		break;
	}

	case UNK30_UNK_16:
	case UNK30_UNK_17: {
		if (mFadeColor.a == 0)
			break;
		setupGraphicsFadeinout();
		draw_wipe_box(param_1, mFadeColor);
		break;
	}
	}
}

void TSMSFader::setupGraphicsFadeinout()
{
	GXSetNumChans(1);
	GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	Mtx afStack_40;
	MTXTrans(afStack_40, 0.0f, 0.0f, 0.0f);
	GXLoadPosMtxImm(afStack_40, 0);
	GXSetCurrentMtx(0);
	GXSetZMode(0, GX_LEQUAL, 0);
	GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c, 0, 0x7d);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_RGBA4, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
}

void TSMSFader::requestWipe(TSMSFader::WipeRequest* param_1)
{
	unk1C = false;
	unk30 = param_1->unk0;
	unk34 = param_1->unk8;

	int iVar3 = param_1->unk4 * mRate;

	switch (param_1->unk0) {
	case UNK30_UNK_13:
		getGameOverBtiResource();
		break;
	case UNK30_UNK_12:
		getMmarkBtiResource();
		getLogoBtiResource();
		break;
	}

	switch (param_1->unk0) {
	case UNK30_UNK_14:
	case UNK30_UNK_16:
		startFadein(iVar3);
		break;

	case UNK30_UNK_15:
	case UNK30_UNK_17:
		startFadeout(iVar3);
		break;

	default:
		Hx_StartWipe(param_1->unk0, iVar3);
		mFadeStatus = Hx_GetWipeType(param_1->unk0) == 1
		                  ? FADE_STATUS_FADING_IN
		                  : FADE_STATUS_FADING_OUT;
		break;
	}

	param_1->unk0 = UNK30_UNK_18;
}

void TSMSFader::startWipe(u32 param_1, f32 param_2, f32 param_3)
{
	mWipeRequest.unk0 = param_1;
	mWipeRequest.unk4 = param_2;
	mWipeRequest.unk8 = param_3;
}

void TSMSFader::startFadein(int param_1)
{
	switch (mFadeStatus) {
	case FADE_STATUS_FULLY_FADED_OUT:
		mFadeStatus = FADE_STATUS_FADING_IN;
		unk12       = 0;
		unk10       = param_1;
		break;
	case FADE_STATUS_FADING_OUT:
		mFadeStatus = FADE_STATUS_FADING_IN;
		unk12       = param_1 * (unk10 - unk12) / unk10;
		unk10       = param_1;
		break;
	}
}

void TSMSFader::startFadeinT(f32 param_1)
{
	mWipeRequest.unk0 = unk1C ? UNK30_UNK_16 : UNK30_UNK_14;
	mWipeRequest.unk4 = param_1;
	mWipeRequest.unk8 = 0.0f;
}

void TSMSFader::startFadeout(int param_1)
{
	switch (mFadeStatus) {
	case FADE_STATUS_FULLY_FADED_IN:
		mFadeStatus = FADE_STATUS_FADING_OUT;
		unk12       = 0;
		unk10       = param_1;
		break;
	case FADE_STATUS_FADING_IN:
		mFadeStatus = FADE_STATUS_FADING_OUT;
		unk12       = param_1 * (unk10 - unk12) / unk10;
		unk10       = param_1;
		break;
	}
}

void TSMSFader::startFadeoutT(f32 param_1)
{
	mWipeRequest.unk0 = unk1C ? UNK30_UNK_17 : UNK30_UNK_15;
	mWipeRequest.unk4 = param_1;
	mWipeRequest.unk8 = 0.0f;
}

void TSMSFader::setColor(JUtility::TColor param_1)
{
	mFadeColor = param_1;
	if (mFadeColor.r > 0xD2)
		mFadeColor.r = 0xD2;
	if (mFadeColor.g > 0xD2)
		mFadeColor.g = 0xD2;
	if (mFadeColor.b > 0xD2)
		mFadeColor.b = 0xD2;
}

void TSMSFader::load(JSUMemoryInputStream& stream)
{
	JDrama::TViewObj::load(stream);

	s32 local_1c = stream.readS32();

	startFadein(local_1c);

	u32 local_18;
	stream.read(&local_18, 4);

	setColor(JUtility::TColor(local_18 >> 24, local_18 >> 16 & 0xFF,
	                          local_18 >> 8 & 0xFF, local_18 & 0xFF));
}

void TSMSFader::setDisplaySize(int param_1, int param_2)
{
	Hx_ResetWipe(param_1, param_2);
}

void TSMSFader::setFadeStatus(TSMSFader::EFadeStatus param_1)
{
	switch (param_1) {
	case FADE_STATUS_FULLY_FADED_IN:
		mFadeStatus       = param_1;
		mFadeColor.a      = 0;
		mWipeRequest.unk0 = UNK30_UNK_18;
		break;
	case FADE_STATUS_FULLY_FADED_OUT:
		mFadeStatus       = param_1;
		mFadeColor.a      = 0xFF;
		mWipeRequest.unk0 = UNK30_UNK_18;
		break;
	}
}

void TSMSFader::getGameOverBtiResource()
{
	Hx_RemoveResource();
	Hx_ProvideResource(JKRGetResource("/common/2d/wipe_gameover.bti"), 0);
}

void TSMSFader::getMmarkBtiResource()
{
	Hx_RemoveResource();
	Hx_ProvideResource(JKRGetResource("/subtitle/timg/wipe_m_mark.bti"), 0);
}

void TSMSFader::getLogoBtiResource()
{
	Hx_ProvideResourceEx(JKRGetResource("/subtitle/timg/title_mini.bti"));
}
