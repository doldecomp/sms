#include <GC2D/ScrnFader.hpp>
#include <GC2D/hx_wiper.h>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <dolphin/gx.h>
#include <version.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

#include <System/Application.hpp>

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

	JUTRect local_4c(
	    param_1.x1 + int(param_2.a * (param_1.getWidth() >> 1) / 255.0f),
	    param_1.y1 + int(param_2.a * (param_1.getHeight() >> 1) / 255.0f),
	    param_1.x2 - int(param_2.a * (param_1.getWidth() >> 1) / 255.0f),
	    param_1.y2 - int(param_2.a * (param_1.getHeight() >> 1) / 255.0f));

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

TSMSFader::TSMSFader(JUtility::TColor color, f32 frame_rate, const char* name)
    : JDrama::TViewObj(name)
    , mFadeDurationFrames(0)
    , mFadeFrame(0)
    , mFrameRate(frame_rate)
    , mFadeColor(color)
    , mUseBoxWipe(false)
    , mFadeStatus(FADE_STATUS_FULLY_FADED_OUT)
    , mWipeType(WIPE_FADE_IN)
    , unk34(0.0f)
{
	Hx_ResetWipe(640, 480);
	mPendingWipeRequest.mWipeType = WIPE_NONE;
}

void TSMSFader::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_MOVE)
		update();

	if (cue & CUE_DRAW)
		draw(JDrama::TRect(0, 0, 640, 480));
}

void TSMSFader::update()
{
	updateRequest();

	if (mFadeStatus == TSMSFader::FADE_STATUS_FULLY_FADED_OUT)
		mFadeColor.a = 0xff;

	if (mWipeType >= WIPE_NONE || mWipeType < WIPE_FADE_IN)
		return;

	updateFadeinout();
}

void TSMSFader::updateRequest()
{
	if (mPendingWipeRequest.mWipeType == WIPE_NONE)
		return;

	f32 delay = mPendingWipeRequest.mDelaySeconds;
	delay -= 1.0f / mFrameRate;
	if (delay < 0.0f)
		delay = 0.0f;
	mPendingWipeRequest.mDelaySeconds = delay;

	if (delay <= 0.0f) {
		requestWipe(&mPendingWipeRequest);
		mPendingWipeRequest.mWipeType = WIPE_NONE;
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
		mFadeColor.a = 0xff - (mFadeFrame++ * 0xff) / (mFadeDurationFrames + 1);
		if (mFadeFrame > mFadeDurationFrames)
			mFadeStatus = FADE_STATUS_FULLY_FADED_IN;
		break;
	case FADE_STATUS_FADING_OUT:
		mFadeColor.a = (++mFadeFrame * 0xff) / (mFadeDurationFrames + 1);
		if (mFadeFrame > mFadeDurationFrames)
			mFadeStatus = FADE_STATUS_FULLY_FADED_OUT;
		break;
	}
}

void TSMSFader::draw(const JDrama::TRect& viewport)
{
	switch (mFadeStatus) {
	case FADE_STATUS_FULLY_FADED_OUT: {
		setupGraphicsFadeinout();
		JUtility::TColor color = mFadeColor;
		color.a                = 0xff;
		fill_rect(viewport, color);
		break;
	}

	case FADE_STATUS_FULLY_FADED_IN:
		break;

	default:
		drawWipe(viewport);
	}
}

void TSMSFader::drawWipe(const JDrama::TRect& viewport)
{
	switch (mWipeType) {
	case WIPE_FADE_IN:
	case WIPE_FADE_OUT:
	case WIPE_BOX_OPEN:
	case WIPE_BOX_CLOSE:
		drawFadeinout(viewport);
		break;
	default:
		if (Hx_UpdateWipe(mFrameRate) == 3)
			mFadeStatus = Hx_GetWipeType(mWipeType) != 0
			                  ? FADE_STATUS_FULLY_FADED_IN
			                  : FADE_STATUS_FULLY_FADED_OUT;
		break;
	}
}

void TSMSFader::drawFadeinout(const JDrama::TRect& viewport)
{
	switch (mWipeType) {
	case WIPE_FADE_IN:
	case WIPE_FADE_OUT: {
		if (mFadeColor.a == 0)
			break;

		setupGraphicsFadeinout();
		fill_rect(viewport, mFadeColor);
		break;
	}

	case WIPE_BOX_OPEN:
	case WIPE_BOX_CLOSE: {
		if (mFadeColor.a == 0)
			break;
		setupGraphicsFadeinout();
		draw_wipe_box(viewport, mFadeColor);
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
	GXLoadPosMtxImm(afStack_40, GX_PNMTX0);
	GXSetCurrentMtx(GX_PNMTX0);
	GXSetZMode(0, GX_LEQUAL, 0);
	GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c, 0, 0x7d);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_RGBA4, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
}

void TSMSFader::requestWipe(TSMSFader::WipeRequest* request)
{
	mUseBoxWipe = false;
	mWipeType   = request->mWipeType;
	unk34       = request->mDelaySeconds;

	int durationFrames = request->mDurationSeconds * mFrameRate;

	switch (request->mWipeType) {
	case WIPE_GAME_OVER:
		getGameOverBtiResource();
		break;
	case WIPE_LOGO:
		getMmarkBtiResource();
		getLogoBtiResource();
		break;
	}

	switch (request->mWipeType) {
	case WIPE_FADE_IN:
	case WIPE_BOX_OPEN:
		startFadein(durationFrames);
		break;

	case WIPE_FADE_OUT:
	case WIPE_BOX_CLOSE:
		startFadeout(durationFrames);
		break;

	default:
		Hx_StartWipe(request->mWipeType, durationFrames);
		mFadeStatus = Hx_GetWipeType(request->mWipeType) == 1
		                  ? FADE_STATUS_FADING_IN
		                  : FADE_STATUS_FADING_OUT;
		break;
	}

	request->mWipeType = WIPE_NONE;
}

void TSMSFader::startWipe(u32 wipe_type, f32 duration_seconds,
                          f32 delay_seconds)
{
	mPendingWipeRequest.mWipeType        = wipe_type;
	mPendingWipeRequest.mDurationSeconds = duration_seconds;
	mPendingWipeRequest.mDelaySeconds    = delay_seconds;
}

void TSMSFader::startFadein(int duration_frames)
{
	switch (mFadeStatus) {
	case FADE_STATUS_FULLY_FADED_OUT:
		mFadeStatus         = FADE_STATUS_FADING_IN;
		mFadeFrame          = 0;
		mFadeDurationFrames = duration_frames;
		break;
	case FADE_STATUS_FADING_OUT:
		mFadeStatus = FADE_STATUS_FADING_IN;
		mFadeFrame  = duration_frames * (mFadeDurationFrames - mFadeFrame)
		             / mFadeDurationFrames;
		mFadeDurationFrames = duration_frames;
		break;
	}
}

void TSMSFader::startFadeinT(f32 duration_seconds)
{
	mPendingWipeRequest.mWipeType = mUseBoxWipe ? WIPE_BOX_OPEN : WIPE_FADE_IN;
	mPendingWipeRequest.mDurationSeconds = duration_seconds;
	mPendingWipeRequest.mDelaySeconds    = 0.0f;
}

void TSMSFader::startFadeout(int duration_frames)
{
	switch (mFadeStatus) {
	case FADE_STATUS_FULLY_FADED_IN:
		mFadeStatus         = FADE_STATUS_FADING_OUT;
		mFadeFrame          = 0;
		mFadeDurationFrames = duration_frames;
		break;
	case FADE_STATUS_FADING_IN:
		mFadeStatus = FADE_STATUS_FADING_OUT;
		mFadeFrame  = duration_frames * (mFadeDurationFrames - mFadeFrame)
		             / mFadeDurationFrames;
		mFadeDurationFrames = duration_frames;
		break;
	}
}

void TSMSFader::startFadeoutT(f32 duration_seconds)
{
	mPendingWipeRequest.mWipeType
	    = mUseBoxWipe ? WIPE_BOX_CLOSE : WIPE_FADE_OUT;
	mPendingWipeRequest.mDurationSeconds = duration_seconds;
	mPendingWipeRequest.mDelaySeconds    = 0.0f;
}

void TSMSFader::setColor(JUtility::TColor color)
{
	mFadeColor = color;
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

	startFadein(stream.readS32());

	u32 local_18 = stream.readU32();

	setColor(JUtility::TColor(local_18 >> 24, local_18 >> 16, local_18 >> 8,
	                          local_18));
}

void TSMSFader::setDisplaySize(int width, int height)
{
	Hx_ResetWipe(width, height);
}

void TSMSFader::setFadeStatus(TSMSFader::EFadeStatus status)
{
	switch (status) {
	case FADE_STATUS_FULLY_FADED_IN:
		mFadeStatus                   = status;
		mFadeColor.a                  = 0;
		mPendingWipeRequest.mWipeType = WIPE_NONE;
		break;
	case FADE_STATUS_FULLY_FADED_OUT:
		mFadeStatus                   = status;
		mFadeColor.a                  = 0xFF;
		mPendingWipeRequest.mWipeType = WIPE_NONE;
		break;
	}
}

void TSMSFader::getGameOverBtiResource()
{
	Hx_RemoveResource();
	Hx_ProvideResource(
	    JKRGetResource(VERSION_SELECT("/common/2d/wipe_gameover.bti",
	                                  GMSP01("/cmn2d/wipe_gameover.bti"),
	                                  GMSP01_NX("/cmn2d/wipe_gameover.bti"))),
	    0);
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
