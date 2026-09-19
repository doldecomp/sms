#include <System/GCLogoDir.hpp>
#include <dolphin/vi.h>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/JDrama/JDRDStageGroup.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <JSystem/JDrama/JDRDStage.hpp>
#include <JSystem/JDrama/JDREfbCtrl.hpp>
#include <JSystem/JDrama/JDRCamera.hpp>
#include <JSystem/JDrama/JDRScreen.hpp>
#include <System/Application.hpp>
#include <System/MarioGamePad.hpp>
#include <System/Resolution.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <GC2D/ScrnFader.hpp>
#include <GC2D/ProgSelect.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <System/DummyStrings.hpp>

void TNintendo2D::perform(u32 cue, JDrama::TGraphics*)
{
	if (cue & CUE_DRAW) {
		GXSetCullMode(GX_CULL_BACK);
		Mtx afStack_3c;
		MTXTrans(afStack_3c, 0.0, 0.0, 0.0);
		GXLoadPosMtxImm(afStack_3c, GX_PNMTX0);
		GXSetCurrentMtx(GX_PNMTX0);
		GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
		GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
		GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_S8, 0);
		GXClearVtxDesc();
		GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
		GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
		GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
		GXSetNumChans(1);
		GXSetNumTexGens(1);
		GXSetNumTevStages(1);
		mLogoTex->load(GX_TEXMAP0);
		GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c,
		                  GX_FALSE, 0x7d);
		GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
		GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);

		GXBegin(GX_QUADS, GX_VTXFMT0, 4);
		GXPosition3s16(mRect.x1, mRect.y1, 0);
		GXColor1u32(mColor);
		GXTexCoord2s8(0, 0);
		GXPosition3s16(mRect.x2, mRect.y1, 0);
		GXColor1u32(mColor);
		GXTexCoord2s8(1, 0);
		GXPosition3s16(mRect.x2, mRect.y2, 0);
		GXColor1u32(mColor);
		GXTexCoord2s8(1, 1);
		GXPosition3s16(mRect.x1, mRect.y2, 0);
		GXColor1u32(mColor);
		GXTexCoord2s8(0, 1);
		GXEnd();
	}
}

TGCLogoDir::TGCLogoDir()
    : JDrama::TDirector()
    , mOverallState(OVERALL_STATE_NLOGO)
    , mState(STATE_WAIT_FADE_IN)
{
	f32 sync       = SMSGetVSyncTimesPerSec();
	mLogoShowTimer = 0;
	mRefreshRate   = sync;
	unk3C          = 0;
	mBHoldTimer    = 0;
}

void TGCLogoDir::setup(JDrama::TDisplay* param_1, TMarioGamePad* param_2)
{
	unk14            = new JDrama::TDStageGroup(param_1);
	mGamePad         = param_2;
	mGamePad->mFlags = TMarioGamePad::PAD_FLAG_MENU_INPUT;

	JDrama::TViewObjPtrListT<JDrama::TViewObj>* rootViewObjs
	    = new JDrama::TViewObjPtrListT<JDrama::TViewObj>("root View Objs");
	unk10 = rootViewObjs;

	JDrama::TViewObjPtrListT<JDrama::TViewObj>* group2d
	    = new JDrama::TViewObjPtrListT<JDrama::TViewObj>("Group 2D");
	rootViewObjs->getChildren().push_back(group2d);

	mNintendoTexture = new JUTTexture(
	    (const ResTIMG*)JKRGetResource("/nintendo/timg/nintendo_376x104.bti"));
	mDolbyTexture = new JUTTexture(
	    (const ResTIMG*)JKRGetResource("/nintendo/timg/title_dolby_mark.bti"));
	mLogoView = new TNintendo2D(mNintendoTexture);

	group2d->getChildren().push_back(mLogoView);

	mProgSelect           = new TProgSelect(0);
	mProgSelect->mGamePad = mGamePad;
	mProgSelect->unkC.on(0xffff);
	group2d->getChildren().push_back(mProgSelect);

	JDrama::TDStageDisp* stageDisp = new JDrama::TDStageDisp;
	unk14->getChildren().push_back(stageDisp);
	JDrama::TRect logoRendArea(0, 0, SMSGetGCLogoRenderWidth(),
	                           SMSGetGCLogoRenderHeight());
	stageDisp->getEfbCtrlDisp()->TEfbCtrl::setSrcRect(logoRendArea);

	f32 h     = logoRendArea.getHeight() * 1.09375f;
	f32 fVar3 = (640 - logoRendArea.getWidth()) / 2;
	f32 fVar2 = (480 - h) / 2;

	JDrama::TOrthoProj* proj = new JDrama::TOrthoProj(
	    -1.0f, 1.0f, fVar3, fVar2, fVar3 + logoRendArea.getWidth(), fVar2 + h);
	group2d->getChildren().push_back(proj);

	JDrama::TScreen* screen = new JDrama::TScreen(logoRendArea, "Screen 2D");
	stageDisp->getUnk14()->getChildren().push_back(screen);
	screen->assignCamera(proj);
	screen->assignViewObj(stageDisp);

	gpApplication.mFader->setColor(JUtility::TColor(0, 0, 0, 0));
	gpApplication.mFader->startWipe(14, 0.4f, 0.0f);
}

TGCLogoDir::~TGCLogoDir()
{
	char trash[8];
	mGamePad->offFlag(TMarioGamePad::PAD_FLAG_MENU_INPUT);
}

static inline bool checkProgressiveSelect(TGCLogoDir* director)
{
	bool selected = false;
	if (director->mProgSelect->unkC.check(0xffff) && VIGetTvFormat() == 0
	    && VIGetDTVStatus() == 1) {
		if (OSGetProgressiveMode() == 1) {
			director->mProgSelect->unkC = 0;
			selected                    = true;
		} else if (director->mGamePad->getButton() & JUTGamePad::B) {
			director->mBHoldTimer += 1;
			if (director->mBHoldTimer / SMSGetVSyncTimesPerSec() > 1.0f) {
				director->mProgSelect->unkC = 0;
				selected                    = true;
			}
		} else {
			director->mBHoldTimer = 0;
		}
	}
	return selected;
}

int TGCLogoDir::direct()
{
	int desiredAppState = TApplication::APP_STATE_DEFAULT;
	JDrama::TDirector::direct();

	int nextState = mOverallState;
	switch (mOverallState) {
	case OVERALL_STATE_NLOGO:
		if (direct_nlogo()) {
			if (mGamePad->isSomethingPushed()) {
				desiredAppState = TApplication::APP_STATE_DONE;
				break;
			}

			mProgSelect->unkC.on(0xffff);
			mLogoView->mLogoTex = mDolbyTexture;
			mLogoView->mRect    = JUTRect(254, 201, 404, 271);
			mLogoView->mColor   = JUtility::TColor(255, 255, 255, 255);
			gpApplication.mFader->startWipe(14, 0.4f, 0.0f);
			nextState = OVERALL_STATE_DOLBY;
		}
		break;

	case OVERALL_STATE_DOLBY:
		if (direct_dolby())
			desiredAppState = TApplication::APP_STATE_DONE;
		break;
	}

	if (nextState != mOverallState) {
		mOverallState = nextState;
		mState        = STATE_WAIT_FADE_IN;
	}

	return desiredAppState;
}

bool TGCLogoDir::direct_nlogo()
{
	bool ended    = false;
	int nextState = mState;
	switch (mState) {
	case STATE_WAIT_FADE_IN:
		if (gpApplication.mFader->isFullyFadedIn()) {
			nextState = !mProgSelect->unkC.check(0xffff) ? STATE_ASK_PROGRESSIVE
			                                             : STATE_SHOW_LOGO;

			SMSGetMSound()->startSoundSystemSE(MSD_SE_MV_CHAO, 0, nullptr, 0);
			mLogoShowTimer = 0;
		} else {
			checkProgressiveSelect(this);
		}
		break;

	case STATE_SHOW_LOGO:
		mLogoShowTimer += 1;
		if (mLogoShowTimer / mRefreshRate >= 0.8f) {
			nextState = STATE_FADE_OUT;
			OSSetProgressiveMode(0);
		} else {
			if (checkProgressiveSelect(this)) {
				mLogoShowTimer = 0;
				nextState      = STATE_ASK_PROGRESSIVE;
			}
		}
		break;

	case STATE_ASK_PROGRESSIVE:
		if (mProgSelect->mHideTextBoxes) {
			mLogoShowTimer = 0;
			nextState      = STATE_SHOW_PROG_RESULT;
		}
		break;

	case STATE_SHOW_PROG_RESULT:
		mLogoShowTimer += 1;
		if (mLogoShowTimer / mRefreshRate >= 2.0f)
			nextState = STATE_FADE_OUT;
		break;

	case STATE_FADE_OUT:
		if (gpApplication.mFader->isFullyFadedOut())
			ended = true;
		break;
	}

	if (mState != STATE_FADE_OUT && mGamePad->isSomethingPushed())
		nextState = STATE_FADE_OUT;

	if (nextState != mState) {
		switch (nextState) {
		case STATE_FADE_OUT:
			gpApplication.mFader->startWipe(15, 0.4f, 0.0f);
			break;
		}
		mState = nextState;
	}

	return ended;
}

bool TGCLogoDir::direct_dolby()
{
	bool ended    = false;
	int nextState = mState;
	switch (mState) {
	case STATE_WAIT_FADE_IN:
		if (gpApplication.mFader->isFullyFadedIn()) {
			mLogoShowTimer = 0;
			nextState      = STATE_SHOW_LOGO;
		}
		break;

	case STATE_SHOW_LOGO:
		mLogoShowTimer += 1;
		if (mLogoShowTimer / mRefreshRate >= 0.8f)
			nextState = STATE_FADE_OUT;
		break;

	case STATE_FADE_OUT:
		if (gpApplication.mFader->isFullyFadedOut())
			ended = true;
		break;
	}

	if (mState != STATE_FADE_OUT && mGamePad->isSomethingPushed())
		nextState = STATE_FADE_OUT;

	if (nextState != mState) {
		switch (nextState) {
		case STATE_FADE_OUT:
			gpApplication.mFader->startWipe(15, 0.4f, 0.0f);
			break;
		}
		mState = nextState;
	}

	return ended;
}
