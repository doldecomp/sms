
#include <dolphin/gx.h>
#include <dolphin/gx/GXGeometry.h>
#include <dolphin/gx/GXLighting.h>
#include <dolphin/gx/GXTev.h>
#include <dolphin/gx/GXTransform.h>
#include <GC2D/BoundPane.hpp>
#include <GC2D/ExPane.hpp>
#include <GC2D/MessageUtil.hpp>
#include <GC2D/SelectMenu.hpp>
#include <GC2D/SelectShine2.hpp>
#include <JSystem/J2D/J2DOrthoGraph.hpp>
#include <JSystem/J2D/J2DPicture.hpp>
#include <JSystem/J2D/J2DScreen.hpp>
#include <JSystem/J2D/J2DScreen.hpp>
#include <JSystem/J2D/J2DTextBox.hpp>
#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/JParticle/JPAEmitterManager.hpp>
#include <JSystem/JUtility/JUTColor.hpp>
#include <JSystem/JUtility/JUTPoint.hpp>
#include <JSystem/JUtility/JUTRect.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <MarioUtil/ReinitGX.hpp>
#include <MSound/MSound.hpp>
#include <stdio.h>
#include <System/Application.hpp>
#include <System/MarioGamePad.hpp>
#include <System/SelectDir.hpp>
#include <System/StageUtil.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// fabricated and not matching
inline void bzero(void* pDst, u32 len)
{
	u8* dst           = (u8*)pDst;
	u32 alignedBlocks = len / 8;
	if (alignedBlocks > 0) {
		for (; alignedBlocks > 0; alignedBlocks--) {
			*dst++ = 0;
			*dst++ = 0;
			*dst++ = 0;
			*dst++ = 0;
			*dst++ = 0;
			*dst++ = 0;
			*dst++ = 0;
			*dst++ = 0;
		}

		len = len % 8;
		if (len == 0) {
			return;
		}
	}

	for (; len != 0; len--) {
		*dst++ = 0;
	}
}

TSelectGrad::TSelectGrad(const char* pName)
    : JDrama::TViewObj(pName)
{
	mRgbAnimCycle[0] = 2;
	mRgbAnimCycle[1] = 0;
	mRgbAnimCycle[2] = 4;
	mTopLeftCol.set(255, 0, 0, 255);
	mBottomRightCol.set(255, 255, 0, 255);
}

void TSelectGrad::setStageColor(u8 stage)
{
	switch (stage) {
	case 2:
		mRgbAnimCycle[0] = 3;
		mRgbAnimCycle[1] = 1;
		mRgbAnimCycle[2] = 5;
		mTopLeftCol.set(255, 255, 0, 255);
		mBottomRightCol.set(0, 255, 0, 255);
		break;
	case 3:
		mRgbAnimCycle[0] = 4;
		mRgbAnimCycle[1] = 2;
		mRgbAnimCycle[2] = 0;
		mTopLeftCol.set(0, 255, 0, 255);
		mBottomRightCol.set(0, 255, 255, 255);
		break;
	case 4:
		mRgbAnimCycle[0] = 2;
		mRgbAnimCycle[1] = 0;
		mRgbAnimCycle[2] = 4;
		mTopLeftCol.set(255, 0, 0, 255);
		mBottomRightCol.set(255, 255, 0, 255);
		break;
	case 13:
		mRgbAnimCycle[0] = 0;
		mRgbAnimCycle[1] = 4;
		mRgbAnimCycle[2] = 2;
		mTopLeftCol.set(0, 0, 255, 255);
		mBottomRightCol.set(255, 0, 255, 255);
		break;
	}
}

void TSelectGrad::perform(u32 flags, JDrama::TGraphics* gfx)
{
	if (flags & 0x2) {
		bool nextCycle = false;
		for (s32 i = 0; i < 3; i++) {
			u8* value = (i == 0) ? &mBottomRightCol.r
			                     : ((i == 1) ? &mBottomRightCol.g
			                                 : &mBottomRightCol.b);

			switch (mRgbAnimCycle[i]) {
			case 0: {
				s16 newValue = *value + 2;
				if (newValue > 255) {
					newValue  = 255;
					nextCycle = true;
				}
				*value = newValue;
			} break;
			case 3: {
				s16 newValue = *value - 2;
				if (newValue < 0) {
					newValue  = 0;
					nextCycle = true;
				}
				*value = newValue;
			} break;
			}

			s32 topLeftAnimCycle = mRgbAnimCycle[i] - 1;
			if (topLeftAnimCycle < 0) {
				topLeftAnimCycle = 5;
			}

			value = (i == 0) ? &mTopLeftCol.r
			                 : ((i == 1) ? &mTopLeftCol.g : &mTopLeftCol.b);

			switch (topLeftAnimCycle) {
			case 0: {
				s16 newValue = *value + 2;
				if (newValue > 255) {
					newValue = 255;
				}
				*value = newValue;
			} break;
			case 3: {
				s16 newValue = *value - 2;
				if (newValue < 0) {
					newValue = 0;
				}
				*value = newValue;
			} break;
			}
		}

		if (nextCycle) {
			// TODO: This doesn't fully match.
			mRgbAnimCycle[0]++;
			mRgbAnimCycle[0] = (mRgbAnimCycle[0] >= 6) ? 0 : mRgbAnimCycle[0];

			mRgbAnimCycle[1]++;
			mRgbAnimCycle[1] = (mRgbAnimCycle[1] >= 6) ? 0 : mRgbAnimCycle[1];

			mRgbAnimCycle[2]++;
			mRgbAnimCycle[2] = (mRgbAnimCycle[2] >= 6) ? 0 : mRgbAnimCycle[2];
		}
	}

	if (flags & 0x8) {
		GXSetDither(GX_TRUE);

		Mtx mtx;
		PSMTXIdentity(mtx);
		GXLoadPosMtxImm(mtx, GX_PNMTX0);

		GXSetCullMode(GX_CULL_BACK);
		GXSetNumTexGens(0);
		GXSetNumTevStages(1);
		GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
		GXSetNumChans(1);
		GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, 0,
		              GX_DF_NONE, GX_AF_NONE);

		GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0,
		              GX_DF_NONE, GX_AF_NONE);

		GXSetChanAmbColor(GX_COLOR0A0, (GXColor) { 0xff, 0xff, 0xff, 0xff });

		GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
		GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_NRM_XYZ, GX_S8, 0);

		GXClearVtxDesc();
		GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
		GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);

		u8 midR = (mTopLeftCol.r + mBottomRightCol.r) >> 1;
		u8 midG = (mTopLeftCol.g + mBottomRightCol.g) >> 1;
		u8 midB = (mTopLeftCol.b + mBottomRightCol.b) >> 1;

		GXBegin(GX_QUADS, GX_VTXFMT0, 4);
		GXPosition3f32(0.0f, 16.0f, -100.0f);
		GXColor3u8(mTopLeftCol.r, mTopLeftCol.g, mTopLeftCol.b);
		GXPosition3f32(600.0f, 16.0f, -100.0f);
		GXColor3u8(midR, midG, midB);
		GXPosition3f32(600.0f, 464.0f, -100.0f);
		GXColor3u8(mBottomRightCol.r, mBottomRightCol.g, mBottomRightCol.b);
		GXPosition3f32(0.0f, 464.0f, -100.0f);
		GXColor3u8(midR, midG, midB);
		GXEnd();
	}
}

TSelectMenu::TSelectMenu(const char* pName)
    : JDrama::TViewObj(pName)
    , mMenuState(CLOSE_MENU)
    , mMenuScreen(nullptr)
    , mLetterBoxTop(nullptr)
    , mLetterBoxBottom(nullptr)
    , mStageName(nullptr)
    , mStageBannerPane(nullptr)
    , mStageBannerShadow(nullptr)
    , unk3C(0)
    , mScenarioPane1(nullptr)
    , mScenarioText1(nullptr)
    , mScenarioImg1(nullptr)
    , mScenarioShadow1(nullptr)
    , unk50(nullptr)
    , mSelectNext(false)
    , unk58()
    , mScenarioPane2(nullptr)
    , mScenarioText2(nullptr)
    , mScenarioImg2(nullptr)
    , mScenarioShadow2(nullptr)
    , unk78(nullptr)
    , mScenarioPaneDist(0)
    , mShineList(nullptr)
    , mScorePane(nullptr)
    , mShineGotMark(nullptr)
    , mShineUnlockedMark(nullptr)
    , mMarkPulseDir(true)
    , mArrowL(nullptr)
    , mArrowR(nullptr)
    , mArrowAnimDir(true)
    , mArrowAnimPos(0)
    , mArrowLBounds()
    , mArrowRBounds()
    , mLetterboxAnimFrame(0)
    , mSelectShineAnimFrame(0)
    , mStage(0)
    , mSelectedShine(0)
    , mNumUnlockedShines(0)
    , mSelectedMarkCol(-1)
    , mMarkCol(-1)
    , mSelectedMarkAlpha(0)
    , mMarkAlpha(0)
    , mCloseMenu(false)
    , unk14B(0)
    , mRcpAnmFrameRate(0.0f)
    , mScenarioBmg(nullptr)
    , mScenarioBmg2(nullptr)
    , unk160()
    , unk164()
    , unk168(200)
    , unk16A(200)
    , mWaitBeforeCloseTimer(10)
{
}

void TSelectMenu::initData(u8 stage, JKRArchive* pArch,
                           TSelectShineManager* pShineMgr,
                           TSelectDir* pSelectDir)
{
	mStage           = stage;
	mSelectShineMgr  = pShineMgr;
	mSelectDir       = pSelectDir;
	mRcpAnmFrameRate = 1.0f / SMSGetAnmFrameRate();

	// Some stages skip the menu.
	switch (mStage) {
	case 10:
	case 1:
	case 0:
		mCloseMenu     = true;
		mSelectedShine = 255;
		return;
	}

	mMenuScreen      = new J2DSetScreen("scenario_select_1.blo", nullptr);
	mLetterBoxTop    = new TExPane(mMenuScreen, 'msk1');
	mLetterBoxBottom = new TExPane(mMenuScreen, 'msk2');
	mStageName       = (J2DTextBox*)mMenuScreen->search('map');
	mScenarioPane1   = new TExPane(mMenuScreen, 's_0');
	mScenarioPane2   = new TExPane(mMenuScreen, '0_0');

	mScenarioPane2->getPane()->hide();
	unk58            = mScenarioPane1->mPane->getBounds();
	mScenarioImg1    = (J2DPicture*)mMenuScreen->search('s_2a');
	mScenarioShadow1 = (J2DPicture*)mMenuScreen->search('s_2b');
	mScenarioImg2    = (J2DPicture*)mMenuScreen->search('0_2a');
	mScenarioShadow2 = (J2DPicture*)mMenuScreen->search('0_2b');
	unk50            = (J2DPicture*)mMenuScreen->search('s_2b');
	unk78            = (J2DPicture*)mMenuScreen->search('0_2b');

	for (s32 i = 0; i < 8; i++) {
		char buf[254];
		snprintf(buf, sizeof(buf), "/select/timg/sc_number_%d.bti", i);
		mScenarioTex[i] = new JUTTexture((const ResTIMG*)JKRGetResource(buf));
	}

	mScenarioText1 = (J2DTextBox*)mMenuScreen->search('sttx');
	SMSMakeTextBuffer(mScenarioText1, 0x80);

	mScenarioText2 = (J2DTextBox*)mMenuScreen->search('0ttx');
	SMSMakeTextBuffer(mScenarioText2, 0x80);

	mScenarioPaneDist = mScenarioPane2->mPane->getBounds().x1
	                    - mScenarioPane1->mPane->getBounds().x1;

	mShineList = mMenuScreen->search('i_0');
	mScorePane = mMenuScreen->search('sc_0');

	for (s32 i = 0; i < 10; i++) {
		char buf[256];
		snprintf(buf, sizeof(buf), "/select/timg/coin_number_%d.bti", i);
		mCoinNumTex[i] = new JUTTexture((const ResTIMG*)JKRGetResource(buf));
	}

	J2DPicture* coinDigits[3];
	for (s32 i = 0; i < 3; i++) {
		coinDigits[i] = (J2DPicture*)mMenuScreen->search('sc_1' + i);
	}

	s32 const tags[] = { 0x0,   0x0, 'bi_', 'rc_', 'mm_', 'pi_',
		                 'sr_', 0x0, 'mo_', 'mr_', 0x0 };

	// TODO: Unused but still compiled in?
	volatile s32 const unkArr[]
	    = { 0x0, 0x0, 0x2, 0x3, 0x4, 0x5, 0x6, 0x0, 0x7, 0x8, 0x0 };

	u8* const stages[] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
		                   nullptr, nullptr, nullptr, nullptr, nullptr };

	s32 numCoins = TFlagManager::getInstance()->getFlag(SMS_getShineStage(stage)
	                                                    + 0x20005);
	if (numCoins > 999) {
		numCoins = 999;
	}
	if (numCoins < 0) {
		numCoins = 0;
	}
	mMenuScreen->search('sc_s')->hide();

	if (numCoins < 100) {
		s32 digit1 = numCoins * 0.1f;
		coinDigits[1]->changeTexture(mCoinNumTex[digit1]->getTexInfo(), 0);

		s32 digit2 = numCoins % 10;
		coinDigits[2]->changeTexture(mCoinNumTex[digit2]->getTexInfo(), 0);

		coinDigits[0]->hide();
	} else {
		f32 hundreds = numCoins * 0.01f;

		s32 digit0 = hundreds;
		coinDigits[0]->changeTexture(mCoinNumTex[digit0]->getTexInfo(), 0);

		s32 digit1 = 0.1f * (numCoins - 100 * (s32)(hundreds));
		coinDigits[1]->changeTexture(mCoinNumTex[digit1]->getTexInfo(), 0);

		s32 digit2 = numCoins % 10;
		coinDigits[2]->changeTexture(mCoinNumTex[digit2]->getTexInfo(), 0);

		if (TFlagManager::getInstance()->getShineFlag(stages[mStage][0])) {
			mMenuScreen->search('sc_s')->show();
		}

		s8 count = 0;
		for (s32 i = 1; i < 3; i++) {
			if (TFlagManager::getInstance()->getShineFlag(stages[mStage][i])) {
				count++;
			}
		}

		if (count == 0) {
			mMenuScreen->search('sc_0')->add(83, 0);
			mMenuScreen->search('r_i')->hide();
		} else if (count == 1) {
			mMenuScreen->search('r_s2')->hide();
		}

		// Mark for a gotten shine, but it's unused here.
		mShineGotMark = new JUTTexture(
		    (const ResTIMG*)JKRGetResource("/select/timg/sc_mark_1.bti"));

		// Mark for an unlocked shine.
		mShineUnlockedMark = new JUTTexture(
		    (const ResTIMG*)JKRGetResource("/select/timg/sc_mark_0.bti"));

		unk160.r = 255;
		unk160.g = 0;
		unk160.b = 0;
		unk160.a = 255;

		unk164.r = 255;
		unk164.g = 255;
		unk164.b = 0;
		unk164.a = 255;

		unk14[0] = 2;
		unk14[1] = 0;
		unk14[2] = 4;

		mMenuScreen->search('bi_0')->hide();

		mStageBannerPane = new TExPane(mMenuScreen, tags[mStage] * 0x100 + '0');
		mStageBannerPane->getPane()->show();
		mStageBannerText
		    = new TBoundPane(mMenuScreen, tags[mStage] * 0x100 + 'a');
		mStageBannerText->getPane()->show();
		mStageBannerShadow
		    = new TBoundPane(mMenuScreen, tags[mStage] * 0x100 + 'b');
		mStageBannerShadow->getPane()->show();

		mScenarioBmg = JKRGetResource("/common/2d/scenarioname.bmg");

		strncpy(mStageName->getStringPtr(),
		        SMSGetMessageData(mScenarioBmg, tags[mStage] & 0xFFFF), 0x11);
		mStageName->setFont((JUTFont*)gpSystemFont);

		mShineUnlockStates[0] = 2;
		mShineUnlockStates[1] = 2;
		mShineUnlockStates[2] = 2;
		mShineUnlockStates[3] = 2;
		mShineUnlockStates[4] = 2;
		mShineUnlockStates[5] = 2;
		mShineUnlockStates[6] = 2;
		mShineUnlockStates[7] = 2;

		// Check which shines are unlocked.
		for (s8 i = 0; i < 8; i++) {
			u32 stage = SMS_getShineStage(mStage);
			if (SMS_isGetShine(stage, i, false)) {
				mShineUnlockStates[i] = 3;
				mNumUnlockedShines    = i + 2;
			}
		}

		if (mNumUnlockedShines > 8) {
			mNumUnlockedShines = 8;
		}

		if (mNumUnlockedShines == 0) {
			mNumUnlockedShines = 1;
		}

		s32 lastShineIdx = mNumUnlockedShines - 1;
		if (lastShineIdx < 0) {
			lastShineIdx = 0;
		}
		mSelectedShine = lastShineIdx;

		if (mNumUnlockedShines < 8) {
			// TODO: I tried matching this as best as I could but the compiler
			// keeps unrolling the loops and I'm running out of ideas...
			bzero(mShineUnlockStates + mNumUnlockedShines,
			      8 - mNumUnlockedShines);
		}

		// Show arrows if we have more than one shine unlocked.
		if (mNumUnlockedShines > 1) {
			mArrowL = mMenuScreen->search('a_l0' + mNumUnlockedShines);
			mArrowR = mMenuScreen->search('a_r0' + mNumUnlockedShines);

			// Cache original arrow bounds.
			mArrowLBounds = mArrowL->getBounds();
			mArrowRBounds = mArrowR->getBounds();

			if (mSelectedShine != 0) {
				mArrowL->show();
			}

			if (mSelectedShine != (mNumUnlockedShines - 1)) {
				mArrowR->show();
			}
		}

		mShineMarks[0] = nullptr;
		mShineMarks[1] = nullptr;
		mShineMarks[2] = nullptr;
		mShineMarks[3] = nullptr;
		mShineMarks[4] = nullptr;
		mShineMarks[5] = nullptr;
		mShineMarks[6] = nullptr;
		mShineMarks[7] = nullptr;

		// This code displays the icons of the unlocked shines at the bottom of
		// the screen. It handles even and odd numbers differently to centre
		// them correctly.
		s32 i = 0;
		if ((mNumUnlockedShines & 1) == 1) {
			// Odd number of shines.
			s32 firstSlot = (7 - mNumUnlockedShines) / 2;
			s32 lastSlot  = firstSlot + mNumUnlockedShines - 1;

			for (s32 slot = firstSlot; slot <= lastSlot; slot++) {
				mShineMarks[i]
				    = (J2DPicture*)mMenuScreen->search('i_o0' + slot);
				mShineMarks[i]->show();

				if (mShineUnlockStates[i] == 2 || mShineUnlockStates[i] == 1) {
					JUTTexture* mark = mShineUnlockedMark;
					mShineMarks[i]->insert(mark, 0, 1.0f);
					mShineMarks[i]->remove(1);
				} else if (mShineUnlockStates[i] == 0) {
					mShineMarks[i]->hide();
				}
				i++;
			}
		} else {
			// Even number of shines.
			s32 firstSlot = (8 - mNumUnlockedShines) / 2;
			s32 lastSlot  = firstSlot + mNumUnlockedShines - 1;

			for (s32 slot = firstSlot; slot <= lastSlot; slot++) {
				mShineMarks[i]
				    = (J2DPicture*)mMenuScreen->search('i_e0' + slot);
				mShineMarks[i]->mVisible = true;

				if (mShineUnlockStates[i] == 2 || mShineUnlockStates[i] == 1) {
					JUTTexture* mark = mShineUnlockedMark;
					mShineMarks[i]->insert(mark, 0, 1.0f);
					mShineMarks[i]->remove(1);
				} else if (mShineUnlockStates[i] == 0) {
					mShineMarks[i]->hide();
				}
				i++;
			}
		}

		mSelectedMarkCol
		    = ((J2DPicture*)mMenuScreen->search('i_o0'))->getWhite();
		mSelectedMarkAlpha = 255;
		mMarkCol   = ((J2DPicture*)mMenuScreen->search('i_o2'))->getWhite();
		mMarkAlpha = mMenuScreen->search('i_o2')->getAlpha();

		((J2DPicture*)mMenuScreen->search('i_o0'))->setWhite(mMarkCol);
		((J2DPicture*)mMenuScreen->search('i_o0'))->setAlpha(mMarkAlpha);

		mShineMarks[mSelectedShine]->setWhite(mSelectedMarkCol);
		mShineMarks[mSelectedShine]->setAlpha(mSelectedMarkAlpha);

		mScenarioImg1->insert(mScenarioTex[mSelectedShine], 0, 1.0f);
		mScenarioImg1->remove(1);
		mScenarioShadow1->insert(mScenarioTex[mSelectedShine], 0, 1.0f);
		mScenarioShadow1->remove(1);

		char buf[254];
		snprintf(buf, sizeof(buf), "/common/2d/scenarioname.bmg");
		mScenarioBmg2 = JKRGetResource(buf);

		mScenarioText1->setFont((JUTFont*)gpSystemFont);
		mScenarioText2->setFont((JUTFont*)gpSystemFont);

		s16 shineID
		    = SMS_getShineID(SMS_getShineStage(mStage), mSelectedShine, false);

		strncpy(mScenarioText1->getStringPtr(),
		        SMSGetMessageData(mScenarioBmg2, SMS_getNormalStage(shineID)),
		        127);
	}
}

void TSelectMenu::startMove()
{
	s32 padding[4];
	JPAEmitterManager* emitter = mSelectDir->unk30;
	mSelectShineMgr->initData(mShineUnlockStates, mNumUnlockedShines,
	                          mSelectedShine, emitter);
	mSelectShineMgr->mRumbleOption[mSelectedShine]->mShouldRumble = true;
}

void TSelectMenu::perform(u32 flags, JDrama::TGraphics* gfx)
{
	if (flags & 0x1) {
		switch (mMenuState) {
		case LETTERBOX_ANIMATION: {
			bool updated = true;

			// Animate the letterbox bars.
			updated &= mLetterBoxTop->update();
			updated &= mLetterBoxBottom->update();

			if (updated || mLetterboxAnimFrame > (s32)(20 * mRcpAnmFrameRate)) {
				// Slide the stage banner from the right side of the screen to
				// the left.
				mStageBannerPane->getPane()->show();
				mStageBannerPane->setPaneOffset(
				    20 * mRcpAnmFrameRate, 0, 0,
				    601 - mStageBannerPane->mInitialBounds.x1, 0.0f);
				mMenuState = STAGE_BANNER_SLIDE;
			}

			mLetterboxAnimFrame++;
		} break;
		case STAGE_BANNER_SLIDE: {
			bool updated = true;

			// Animate the letterbox bars.
			updated &= mLetterBoxTop->update();
			updated &= mLetterBoxBottom->update();

			// Animate the stage banner.
			updated &= mStageBannerPane->update();

			if (updated) {
				// Squash the stage banner on the left side of the screen.
				JUTRect bounds = mStageBannerText->getPane()->getBounds();
				s32 time       = 15.0f * mRcpAnmFrameRate;
				mStageBannerText->setPanePosition(
				    time, JUTPoint(0, 0), JUTPoint(0, -6), JUTPoint(0, -10));
				mStageBannerText->setPaneSize(time, JUTPoint(0, 0),
				                              JUTPoint(-110, 12),
				                              JUTPoint(-160, 20));
				mStageBannerShadow->setPanePosition(
				    time, JUTPoint(0, 0), JUTPoint(0, -6), JUTPoint(0, -10));
				mStageBannerShadow->setPaneSize(time, JUTPoint(0, 0),
				                                JUTPoint(-110, 12),
				                                JUTPoint(-160, 20));
				mMenuState = STAGE_BANNER_SQUASH;
			}
		} break;
		case STAGE_BANNER_SQUASH: {
			bool updated = true;

			// Animate the stage banner.
			updated &= mStageBannerText->update();
			updated &= mStageBannerShadow->update();

			if (updated) {
				// Stretch the stage banner after squash.
				JUTRect bounds = mStageBannerText->getPane()->getBounds();
				s32 time       = 20.0f * mRcpAnmFrameRate;
				mStageBannerText->setPanePosition(
				    time, JUTPoint(0, -10), JUTPoint(0, -6), JUTPoint(0, 4));
				mStageBannerText->setPaneSize(time, JUTPoint(-160, 20),
				                              JUTPoint(-110, 12),
				                              JUTPoint(40, -8));
				mStageBannerShadow->setPanePosition(
				    time, JUTPoint(0, -10), JUTPoint(0, -6), JUTPoint(0, 4));
				mStageBannerShadow->setPaneSize(time, JUTPoint(-160, 20),
				                                JUTPoint(-110, 12),
				                                JUTPoint(40, -8));
				mMenuState = STAGE_BANNER_STRETCH;
			}
		} break;
		case STAGE_BANNER_STRETCH: {
			bool updated = true;

			// Animate the stage banner.
			updated &= mStageBannerText->update();
			updated &= mStageBannerShadow->update();

			if (updated) {
				// "Bounce" banner size back to normal.
				JUTRect bounds = mStageBannerText->getPane()->getBounds();
				s32 time       = 15.0f * mRcpAnmFrameRate;
				mStageBannerText->setPanePosition(
				    time, JUTPoint(0, 4), JUTPoint(0, 3), JUTPoint(0, 0));
				mStageBannerText->setPaneSize(time, JUTPoint(40, -8),
				                              JUTPoint(15, -6), JUTPoint(0, 0));
				mStageBannerShadow->setPanePosition(
				    time, JUTPoint(0, 4), JUTPoint(0, 3), JUTPoint(0, 0));
				mStageBannerShadow->setPaneSize(
				    time, JUTPoint(40, -8), JUTPoint(15, -6), JUTPoint(0, 0));

				mStageName->show();
				mStageName->setAlpha(0);

				mScenarioPane1->getPane()->show();
				mScenarioPane1->getPane()->setAlpha(0);

				mShineList->show();
				mShineList->setAlpha(0);

				mScorePane->show();
				mScorePane->setAlpha(0);

				mMenuState = APPEAR_MENU;
			}
		} break;
		case APPEAR_MENU: {
			bool updated = true;

			// Animate the stage banner.
			updated &= mStageBannerText->update();
			updated &= mStageBannerShadow->update();

			// Fade the stage name in.
			f32 curAlpha   = mStageName->getAlpha();
			u16 stageAlpha = SMSGetAnmFrameRate() * 6.0f + curAlpha;
			if (stageAlpha > 255u) {
				stageAlpha = 255u;
			}
			mStageName->setAlpha(stageAlpha);

			// Fade the shine menu icons, scenario name, and coin counter in.
			s32 menuAlpha = 0;
			if (stageAlpha > 128) {
				s32 curAlpha = mScenarioPane1->getPane()->getAlpha();
				menuAlpha    = SMSGetAnmFrameRate() * 6.0f + curAlpha > 255.0f
				                   ? 255.0f
				                   : SMSGetAnmFrameRate() * 6.0f + curAlpha;

				mScenarioPane1->getPane()->setAlpha(menuAlpha);
				mShineList->setAlpha(menuAlpha);
				mScorePane->setAlpha(menuAlpha);
			}

			if (updated && (menuAlpha == 255)) {
				mMenuState = MENU_INPUT_LOOP;
			}
		} break;
		case WAIT_BEFORE_CLOSE: {
			if (mSelectShineAnimFrame > mWaitBeforeCloseTimer) {
				mMenuState = CLOSE_MENU;
			}
			mSelectShineAnimFrame++;
		} break;
		case MENU_INPUT_LOOP: {
			if (mGamePad->checkFrameMeaning(0x20)) {
				SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_SHINE_SELECT, 0,
				                                   nullptr, 0);

				JUTRect bounds = mLetterBoxTop->getPane()->getBounds();
				s32 time       = 30.0f * mRcpAnmFrameRate;

				s32 h = 224;
				mLetterBoxTop->setPaneSize(time, bounds.getWidth(), h,
				                           bounds.getWidth(),
				                           bounds.getHeight());

				mLetterBoxTop->setPaneAlpha(
				    time, 255, mLetterBoxTop->getPane()->getAlpha());

				bounds = mLetterBoxBottom->getPane()->getBounds();

				mLetterBoxBottom->setPaneSize(
				    time, bounds.getWidth(), bounds.y2 - 224, bounds.getWidth(),
				    bounds.getHeight());

				mLetterBoxBottom->setPaneOffset(time, 0.0f, 224 - bounds.y1,
				                                0.0f, 0.0f);

				mLetterBoxBottom->setPaneAlpha(
				    time, 255, mLetterBoxBottom->getPane()->getAlpha());

				mSelectDir->changeOrder();
				mSelectShineMgr->startClose();

				JGeometry::TVec3<f32> emitterPos(300.0f, 244.0f, 0.0f);
				JPAEmitterManager* emitter = mSelectDir->unk34;
				if (mShineUnlockStates[mSelectedShine] == 3) {
					emitter->createEmitter(emitterPos, 5, nullptr, nullptr);
				} else {
					emitter->createEmitter(emitterPos, 4, nullptr, nullptr);
				}
				emitter->createEmitter(emitterPos, 6, nullptr, nullptr);
				emitter->createEmitter(emitterPos, 7, nullptr, nullptr);
				emitter->createEmitter(emitterPos, 8, nullptr, nullptr);

				mMenuState = DISAPPEAR_MENU;
			} else if (mGamePad->checkFrameMeaning(0x8)) {
				if (getPrevIndex() != -1) {
					SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_SHINE_CURSOR,
					                                   0, nullptr, 0);

					u8 prevIndex = getPrevIndex();
					mSelectShineMgr->startDecrease(mSelectedShine - prevIndex);

					mSelectNext = false;

					mScenarioPane2->getPane()->show();
					mScenarioPane2->setPaneAlpha(10, 255, 0);
					mScenarioPane2->setPaneOffset(10, -mScenarioPaneDist, 0,
					                              -mScenarioPaneDist * 2, 0);

					mScenarioPane1->getPane()->show();
					mScenarioPane1->setPaneAlpha(10, 0, 255);
					mScenarioPane1->setPaneOffset(10, mScenarioPaneDist, 0, 0,
					                              0);

					mScenarioImg2->insert(mScenarioTex[prevIndex], 0, 1.0f);
					mScenarioImg2->remove(1);

					mScenarioShadow2->insert(mScenarioTex[prevIndex], 0, 1.0f);
					mScenarioShadow2->remove(1);

					mScenarioImg1->insert(mScenarioTex[mSelectedShine], 0,
					                      1.0f);
					mScenarioImg1->remove(1);

					mScenarioShadow1->insert(mScenarioTex[mSelectedShine], 0,
					                         1.0f);
					mScenarioShadow1->remove(1);

					s16 shineID = SMS_getShineID(SMS_getShineStage(mStage),
					                             mSelectedShine, false);
					const char* scenarioName = SMSGetMessageData(
					    mScenarioBmg2, SMS_getNormalStage(shineID));

					strncpy(mScenarioText1->getStringPtr(), scenarioName, 127);

					mShineMarks[mSelectedShine]->mWhite = mMarkCol;
					mShineMarks[mSelectedShine]->setAlpha(mMarkAlpha);

					mSelectShineMgr->mRumbleOption[mSelectedShine]
					    ->mShouldRumble
					    = false;

					mSelectedShine = prevIndex;

					s16 shineID2 = SMS_getShineID(SMS_getShineStage(mStage),
					                              mSelectedShine, false);
					const char* scenarioName2 = SMSGetMessageData(
					    mScenarioBmg2, SMS_getNormalStage(shineID));
					strncpy(mScenarioText2->getStringPtr(), scenarioName2, 127);

					mShineMarks[mSelectedShine]->mWhite = mSelectedMarkCol;
					mShineMarks[mSelectedShine]->setAlpha(mSelectedMarkAlpha);

					mSelectShineMgr->mRumbleOption[mSelectedShine]
					    ->mShouldRumble
					    = true;

					if (mNumUnlockedShines > 1) {
						if ((mSelectedShine != 0) && !mArrowL->isVisible()) {
							mArrowL->show();
						}
						if (mSelectedShine != (mNumUnlockedShines - 1)
						    && !mArrowR->isVisible()) {
							mArrowR->show();
						}
					}

					mMenuState = MENU_ANIM_LOOP;
				}
			} else if (mGamePad->checkFrameMeaning(0x10)) {
				if (getNextIndex() != -1) {
					SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_SHINE_CURSOR,
					                                   0, nullptr, 0);

					u8 nextIndex = getNextIndex();
					mSelectShineMgr->startIncrease(nextIndex - mSelectedShine);

					mSelectNext = true;

					mScenarioPane2->getPane()->show();
					mScenarioPane2->setPaneAlpha(10, 255, 0);
					mScenarioPane2->setPaneOffset(10, -mScenarioPaneDist, 0, 0,
					                              0);

					mScenarioPane1->getPane()->show();
					mScenarioPane1->setPaneAlpha(10, 0, 255);
					mScenarioPane1->setPaneOffset(10, -mScenarioPaneDist, 0, 0,
					                              0);

					mScenarioImg2->insert(mScenarioTex[nextIndex], 0, 1.0f);
					mScenarioImg2->remove(1);

					mScenarioShadow2->insert(mScenarioTex[nextIndex], 0, 1.0f);
					mScenarioShadow2->remove(1);

					mScenarioImg1->insert(mScenarioTex[mSelectedShine], 0,
					                      1.0f);
					mScenarioImg1->remove(1);

					mScenarioShadow1->insert(mScenarioTex[mSelectedShine], 0,
					                         1.0f);
					mScenarioShadow1->remove(1);

					s16 shineID = SMS_getShineID(SMS_getShineStage(mStage),
					                             mSelectedShine, false);
					const char* scenarioName = SMSGetMessageData(
					    mScenarioBmg2, SMS_getNormalStage(shineID));

					strncpy(mScenarioText1->getStringPtr(), scenarioName, 127);

					mSelectShineMgr->mRumbleOption[mSelectedShine]
					    ->mShouldRumble
					    = false;
					mShineMarks[mSelectedShine]->mWhite = mMarkCol;
					mShineMarks[mSelectedShine]->setAlpha(mMarkAlpha);

					mSelectedShine = nextIndex;

					s16 shineID2 = SMS_getShineID(SMS_getShineStage(mStage),
					                              mSelectedShine, false);
					const char* scenarioName2 = SMSGetMessageData(
					    mScenarioBmg2, SMS_getNormalStage(shineID));
					strncpy(mScenarioText2->getStringPtr(), scenarioName2, 127);

					mSelectShineMgr->mRumbleOption[mSelectedShine]
					    ->mShouldRumble
					    = true;

					mShineMarks[mSelectedShine]->mWhite = mSelectedMarkCol;
					mShineMarks[mSelectedShine]->setAlpha(mSelectedMarkAlpha);

					if (mNumUnlockedShines > 1) {
						if ((mSelectedShine != (mNumUnlockedShines - 1))
						    && !mArrowR->isVisible()) {
							mArrowR->show();
						}
						if (mSelectedShine > 0 && !mArrowL->isVisible()) {
							mArrowL->show();
						}
					}

					mMenuState = MENU_ANIM_LOOP;
				}
			}
		}
			// fall through
		case MENU_ANIM_LOOP: {
			if (mNumUnlockedShines > 1) {
				if (mArrowL->isVisible()) {
					s32 x = mArrowAnimPos * 0.5f * SMSGetAnmFrameRate();
					mArrowL->move(mArrowLBounds.x1 - x, mArrowLBounds.y1);
				}

				if (mArrowR->isVisible()) {
					s32 x = mArrowAnimPos * 0.5f * SMSGetAnmFrameRate();
					mArrowR->move(mArrowRBounds.x1 + x, mArrowRBounds.y1);
				}

				if (mArrowAnimDir) {
					mArrowAnimPos++;
					if (mArrowAnimPos > 10) {
						mArrowAnimDir = false;
					}
				} else {
					mArrowAnimPos--;
					if (mArrowAnimPos == 0) {
						mArrowAnimDir = true;
					}
				}

				s32 alpha = mArrowL->getAlpha();
				if (mSelectedShine == 0) {
					if (alpha != 0) {
						alpha -= 4;
						if (alpha < 0) {
							alpha = 0;
							mArrowL->hide();
						}
						mArrowL->setAlpha(alpha);
					}
				} else {
					u8 alphaRef = mMarkAlpha;
					if (alpha < alphaRef) {
						alpha = alpha + 4;
						if (alpha > alphaRef) {
							alpha = alphaRef;
						}
						mArrowL->setAlpha(alpha);
					}
				}

				alpha = mArrowR->getAlpha();
				if (getNextIndex() == -1) {
					if (alpha > 0) {
						s32 alpha2 = alpha - 4;
						if (alpha2 < 0) {
							alpha2 = 0;
							mArrowR->hide();
						}
						mArrowR->setAlpha(alpha2);
					}
				} else {
					u8 alphaRef = mMarkAlpha;
					if (alpha < alphaRef) {
						s32 alpha2 = alpha + 4;
						if (alpha2 > alphaRef) {
							alpha2 = alphaRef;
						}
						mArrowR->setAlpha(alpha2);
					}
				}
			}
			if (mMenuState != MENU_INPUT_LOOP) {

				bool updated = true;
				updated &= mScenarioPane1->update();
				updated &= mScenarioPane2->update();

				if (updated) {
					mScenarioPane1->getPane()->hide();
					mMenuState = MENU_INPUT_LOOP;
				}
			}
			s32 alpha = mShineMarks[mSelectedShine]->getAlpha();
			if (mMarkPulseDir) {
				u8 alphaRef = mSelectedMarkAlpha;
				alpha       = alpha + 6;
				if (alpha > alphaRef) {
					mMarkPulseDir = false;
					alpha         = alphaRef;
				}
			} else {
				alpha = alpha - 6;
				if (alpha < 64) {
					mMarkPulseDir = true;
					alpha         = 64;
				}
			}
			mShineMarks[mSelectedShine]->setAlpha(alpha);

		} break;
		case DISAPPEAR_MENU: {
			bool updated = true;
			s16 alpha    = mStageName->getAlpha() - 16;
			if (alpha <= 0) {
				alpha = 0;
			} else {
				updated = false;
			}
			mStageName->setAlpha(alpha);

			mStageBannerText->getPane()->setAlpha(alpha);

			if (alpha < mStageBannerShadow->getPane()->getAlpha()) {
				mStageBannerShadow->getPane()->setAlpha(alpha);
			}

			mScenarioPane1->getPane()->setAlpha(alpha);
			mScenarioPane2->getPane()->setAlpha(alpha);
			mShineList->setAlpha(alpha);
			mScorePane->setAlpha(alpha);

			updated &= mLetterBoxTop->update();
			updated &= mLetterBoxBottom->update();

			// TODO: This needs to be matched properly.
			// The compiler does a lot of inlining here that we don't want.
			{
				u32 ccol1
				    = ((J2DPicture*)mLetterBoxTop->getPane())->mCornerColor[0];
				u32 ccol2
				    = ((J2DPicture*)mLetterBoxTop->getPane())->mCornerColor[2];

				u32 var1 = (ccol1 & 0xFFu) + 16u;

				if (var1 > 255u) {
					var1 = 255u;
				} else {
					updated = false;
				}

				u32 var2 = (ccol2 & 0xFF) + 16;

				if (var2 > 255u) {
					var2 = 255u;
				} else {
					updated = false;
				}

				// These are probably inlined functions.
				u8 alpha              = var2;
				JUtility::TColor col1 = JUtility::TColor(0, 0, 0, alpha);

				alpha                 = var2;
				JUtility::TColor col2 = JUtility::TColor(0, 0, 0, alpha);

				alpha                 = var1;
				JUtility::TColor col3 = JUtility::TColor(0, 0, 0, alpha);

				alpha = var1;
				((J2DPicture*)mLetterBoxTop->getPane())
				    ->setCornerColor(JUtility::TColor(0, 0, 0, alpha), col3,
				                     col2, col1);
			}

			{
				u32 ccol1 = ((J2DPicture*)mLetterBoxBottom->getPane())
				                ->mCornerColor[0];
				u32 ccol2 = ((J2DPicture*)mLetterBoxBottom->getPane())
				                ->mCornerColor[2];

				u32 var1 = (ccol1 & 0xFFu) + 16u;

				if (var1 > 255u) {
					var1 = 255u;
				} else {
					updated = false;
				}
				u32 var2 = (ccol2 & 0xFF) + 16;

				if (var2 > 255u) {
					var2 = 255u;
				} else {
					updated = false;
				}

				u8 alpha              = var2;
				JUtility::TColor col1 = JUtility::TColor(0, 0, 0, alpha);

				alpha                 = var2;
				JUtility::TColor col2 = JUtility::TColor(0, 0, 0, alpha);

				alpha                 = var1;
				JUtility::TColor col3 = JUtility::TColor(0, 0, 0, alpha);

				alpha = var1;
				((J2DPicture*)mLetterBoxBottom->getPane())
				    ->setCornerColor(JUtility::TColor(0, 0, 0, alpha), col3,
				                     col2, col1);

				if (updated) {
					mMenuState            = WAIT_BEFORE_CLOSE;
					mSelectShineAnimFrame = 0;
				}
			}
		} break;

		case CLOSE_MENU:
			mCloseMenu = true;
			break;

		default:
			break;
		}
	}

	if (flags & 0x8) {
		if (mMenuState < 10 && mMenuState >= 0) {
			ReInitializeGX();
			SMS_DrawInit();
			J2DOrthoGraph graph(gfx->getViewport());
			graph.setup2D();
			graph.setup2D();
			mMenuScreen->draw(0, 0, &graph);
		}
	}
}

#pragma dont_inline on
s8 TSelectMenu::getNextIndex()
{
	s8 res    = -1;
	u32 index = mSelectedShine;

	if (index >= 8) {
		return -1;
	}

	for (s32 i = index + 1; i < 8; i++) {
		if (mShineUnlockStates[i] == 2 || mShineUnlockStates[i] == 3) {
			res = i;
			break;
		}
	}
	return res;
}
#pragma dont_inline off

#pragma dont_inline on
s8 TSelectMenu::getPrevIndex()
{
	s8 res    = -1;
	u32 index = mSelectedShine;

	if (index == 0) {
		return -1;
	}

	for (s32 i = index - 1; i >= 0; i--) {
		if (mShineUnlockStates[i] == 2 || mShineUnlockStates[i] == 3) {
			res = i;
			break;
		}
	}
	return res;
}
#pragma dont_inline off

void TSelectMenu::startOpenWindow()
{
	if (mCloseMenu == false) {
		mMenuState = LETTERBOX_ANIMATION;
		mStageName->hide();
		mStageBannerPane->getPane()->hide();
		mScenarioPane1->getPane()->hide();
		mShineList->hide();
		mScorePane->hide();

		s32 time = 30.0f * mRcpAnmFrameRate;

		// Set the target positions of the top and bottom letterbox bars.
		JUTRect rect = mLetterBoxTop->getPane()->getBounds();
		mLetterBoxTop->setPaneSize(time, rect.getWidth(), rect.getHeight(),
		                           rect.getWidth(), 0);

		rect = mLetterBoxBottom->getPane()->getBounds();
		mLetterBoxBottom->setPaneSize(time, rect.getWidth(), rect.getHeight(),
		                              rect.getWidth(), 0);
		mLetterBoxBottom->setPaneOffset(time, 0, 0, 0, rect.getHeight());

		MSBgm::startBGM(MSD_BGM_FANFARE_CASINO);
		mLetterboxAnimFrame = 0u;
	}
}
