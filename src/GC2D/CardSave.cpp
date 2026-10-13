#include <GC2D/CardSave.hpp>
#include <stdio.h>
#include <dolphin/gx/GXCull.h>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <JSystem/J2D/J2DOrthoGraph.hpp>
#include <JSystem/J2D/J2DScreen.hpp>
#include <JSystem/J2D/J2DTextBox.hpp>
#include <JSystem/J2D/J2DPicture.hpp>
#include <JSystem/JParticle/JPAEmitterManager.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <System/Application.hpp>
#include <System/FlagManager.hpp>
#include <System/MarioGamePad.hpp>
#include <System/MarDirector.hpp>
#include <GC2D/MessageUtil.hpp>
#include <GC2D/PauseMenu2.hpp>
#include <GC2D/ExPane.hpp>
#include <MSound/MSound.hpp>

// rogue includes needed for matching sinit & bss
#include <System/DummyStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

extern JPAEmitterManager* gpEmitterManager4D2;

u32 TCardSave::cMessageID[] = {
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xC,        0xF,        0x12,
	0x4,        0x5,        0x6,        0x16,       0x1A,       0x14,
	0xE,        0xD,        0x1,        0xA,        0x10,       0x19,
	0x13,       0x18,       0x2,        0x8,        0x17,       0x1B,
	0x15,       0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0x3,        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0x11,       0xB,        0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x9,        0x7,
};

TEProgress TCardSave::changeMode(s32 param_1)
{
	TEProgress result = PROGRESS_UNK4;
	switch (param_1) {
	case CARD_RESULT_NOCARD:
		result = PROGRESS_UNK3;
		break;
	case CARD_RESULT_FATAL_ERROR:
	case CARD_RESULT_WRONGDEVICE:
		result = PROGRESS_UNK4;
		break;
	case CARD_RESULT_IOERROR:
		result = PROGRESS_UNK2D;
		break;
	case -0x100:
		result = PROGRESS_UNK5;
		break;
	case CARD_RESULT_BROKEN:
		result = PROGRESS_UNK6;
		break;
	case CARD_RESULT_ENCODING:
		result = PROGRESS_UNK7;
		break;
	case CARD_RESULT_INSSPACE:
		result = PROGRESS_UNKC;
		break;
	case CARD_RESULT_NOENT:
		result = PROGRESS_UNKD;
		break;
	case CARD_RESULT_NOFILE:
		result = PROGRESS_UNKE;
		break;
	}
	return result;
}

TCardSave::TCardSave(const char* name, bool param_2)
    : JDrama::TViewObj(name)
    , unk18(param_2)
    , unk48(nullptr)
    , unk5C(0)
    , unk60(0)
    , unkA0(nullptr)
    , unkA4(nullptr)
    , unkA8(nullptr)
    , unkBC(0)
    , unkC0(0)
    , mConfirmWindow(nullptr)
    , unkF4(nullptr)
    , unkF8(nullptr)
    , mConfirmWindowYes(nullptr)
    , mConfirmWindowNo(nullptr)
    , unk124(nullptr)
    , unk128(nullptr)
    , mConfirmWindowText(nullptr)
    , mConfirmWindowTextShadow(nullptr)
    , mShineCountOver100(nullptr)
    , unk160(nullptr)
    , unk174(nullptr)
    , unk178(nullptr)
    , unk2DC(1)
    , unk2DE(0)
    , unk2DF(1)
    , unk2E0(0)
    , unk2E8(0)
    , unk2E9(0)
    , unk2FC(0)
    , unk300(200)
    , unk302(200)
    , unk304(1)
    , unk310(PROGRESS_UNK1)
    , unk314(PROGRESS_UNK1)
{
}

void TCardSave::load(JSUMemoryInputStream& stream)
{
	JDrama::TViewObj::load(stream);
	initData(SMSGetMarDirector()->getGamePad());
}

void TCardSave::initData(TMarioGamePad* game_pad)
{
	if (unk18) {
		JKRArchive* arch = (JKRArchive*)JKRFileLoader::getVolume("endsave");
		mSaveScreen      = new J2DSetScreen("save_end.blo", arch);
	} else {
		JKRArchive* arch = (JKRArchive*)JKRFileLoader::getVolume("game_6");
		mSaveScreen      = new J2DSetScreen("save.blo", arch);
	}

	mSaveScreen->setCullBack(GX_CULL_BACK);

	for (int i = 0; i < 10; ++i) {
		char acStack_48[0x28];
		if (unk18) {
			snprintf(acStack_48, 0x28, "/endsave/timg/coin_number_%d.bti", i);
		} else {
			snprintf(acStack_48, 0x28, "/game_6/timg/coin_number_%d.bti", i);
		}

		mLargeDigitFontTextures[i]
		    = new JUTTexture((ResTIMG*)JKRGetResource(acStack_48));
	}

	// Window 0: some kind of error window
	unk48 = new TExPane(mSaveScreen, '\0w_0');
	unk4C = unk48->getPane()->getBounds();

	unkA0 = (J2DTextBox*)mSaveScreen->search('m_5a');
	unkA4 = (J2DTextBox*)mSaveScreen->search('m_5b');
	makeBuffer(unkA0, 0x200);
	makeBuffer(unkA4, 0x200);

	unkA8 = new TExPane(mSaveScreen, '\0w_3');
	unkAC = unkA8->getPane()->getBounds();
	unkC4 = new TExPane(mSaveScreen, '\0w_3');
	unkC8 = unkC4->getPane()->getBounds();

	unkD8 = (J2DTextBox*)mSaveScreen->search('m_7a');
	unkDC = (J2DTextBox*)mSaveScreen->search('m_7b');
	makeBuffer(unkD8, 0x200);
	makeBuffer(unkDC, 0x200);

	// Regular confirm/saving/continue or quit window
	mConfirmWindow              = new TExPane(mSaveScreen, '\0w_1');
	mConfirmWindowInitialBounds = mConfirmWindow->getPane()->getBounds();

	unkF4 = (J2DTextBox*)mSaveScreen->search('m_2a');
	unkF8 = (J2DTextBox*)mSaveScreen->search('m_2b');
	makeBuffer(unkF4, 0x200);
	makeBuffer(unkF8, 0x200);

	mConfirmWindowYes              = new TExPane(mSaveScreen, '\0s_6');
	mConfirmWindowYesInitialBounds = mConfirmWindowYes->getPane()->getBounds();

	mConfirmWindowNo              = new TExPane(mSaveScreen, '\0s_7');
	mConfirmWindowNoInitialBounds = mConfirmWindowNo->getPane()->getBounds();

	unk124 = (J2DTextBox*)mSaveScreen->search('m_3a');
	unk128 = (J2DTextBox*)mSaveScreen->search('m_3b');
	makeBuffer(unk124, 0x20);
	makeBuffer(unk128, 0x20);

	mConfirmWindowText       = (J2DTextBox*)mSaveScreen->search('m_4a');
	mConfirmWindowTextShadow = (J2DTextBox*)mSaveScreen->search('m_4b');
	makeBuffer(mConfirmWindowText, 0x200);
	makeBuffer(mConfirmWindowTextShadow, 0x200);

	mShineCountOver100         = mSaveScreen->search('f_si');
	mShineCountUnder100        = mSaveScreen->search('f_sh');
	mShineCountHundredsOver100 = (J2DPicture*)mSaveScreen->search('n_4a');
	mShineCountTensOver100     = (J2DPicture*)mSaveScreen->search('n_4b');
	mShineCountOnesOver100     = (J2DPicture*)mSaveScreen->search('n_4c');
	mShineCountTensUnder100    = (J2DPicture*)mSaveScreen->search('n_3a');
	mShineCountOnesUnder100    = (J2DPicture*)mSaveScreen->search('n_3b');
	mConfirmWindowMarioText    = mSaveScreen->search('\0t_0');

	for (int i = 0; i < 3; ++i)
		mSaveNameABC[i] = mSaveScreen->search('\0t_1' + i);

	// Window for a save that requires card formatting
	unk160 = new TExPane(mSaveScreen, '\0w_2');
	unk164 = unk160->getPane()->getBounds();

	unk174 = (J2DTextBox*)mSaveScreen->search('m_6a');
	unk178 = (J2DTextBox*)mSaveScreen->search('m_6b');
	makeBuffer(unk174, 0x200);
	makeBuffer(unk178, 0x200);

	// Event save window
	mEventSaveWindow              = new TExPane(mSaveScreen, '\0w_4');
	mEventSaveWindowInitialBounds = mEventSaveWindow->getPane()->getBounds();

	mEventSaveWindowText       = (J2DTextBox*)mSaveScreen->search('sm0a');
	mEventSaveWindowTextShadow = (J2DTextBox*)mSaveScreen->search('sm0b');
	makeBuffer(mEventSaveWindowText, 0x200);
	makeBuffer(mEventSaveWindowTextShadow, 0x200);

	for (int i = 0; i < 2; ++i) {
		mEventSaveOptions[i][0]
		    = (J2DTextBox*)mSaveScreen->search('sm1a' + 0x100 * i);
		mEventSaveOptions[i][0]->setFont((JUTFont*)gpSystemFont);

		mEventSaveOptions[i][1]
		    = (J2DTextBox*)mSaveScreen->search('sm1b' + 0x100 * i);
		mEventSaveOptions[i][1]->setFont((JUTFont*)gpSystemFont);

		mEventSavePointerHands[i][0] = mSaveScreen->search('sc1a' + 0x100 * i);
		mEventSavePointerHands[i][1] = mSaveScreen->search('sc1b' + 0x100 * i);
	}
	unk1B8 = mEventSavePointerHands[0][0]->getBounds().x1;

	// Save window after getting a shine
	mShineGetSaveWindow = new TExPane(mSaveScreen, '\0w_5');
	mShineGetSaveWindowInitialBounds
	    = mShineGetSaveWindow->getPane()->getBounds();

	mShineGetSaveWindowText       = (J2DTextBox*)mSaveScreen->search('sm6a');
	mShineGetSaveWindowTextShadow = (J2DTextBox*)mSaveScreen->search('sm6b');
	makeBuffer(mShineGetSaveWindowText, 0x200);
	makeBuffer(mShineGetSaveWindowTextShadow, 0x200);

	unk208 = (J2DTextBox*)mSaveScreen->search('m_8a');
	unk20C = (J2DTextBox*)mSaveScreen->search('m_8b');
	makeBuffer(unk208, 0x20);
	makeBuffer(unk20C, 0x20);

	unk210 = mSaveScreen->search('f_sk');
	unk214 = mSaveScreen->search('f_sj');
	unk218 = mSaveScreen->search('n_6a');
	unk21C = mSaveScreen->search('n_6b');
	unk220 = mSaveScreen->search('n_6c');
	unk224 = mSaveScreen->search('n_5a');
	unk228 = mSaveScreen->search('n_5b');

	mShineGetSaveMarioText = mSaveScreen->search('\0t_4');

	for (int i = 0; i < 3; ++i)
		mShineGetSaveNameABC[i] = mSaveScreen->search('\0t_5' + i);

	for (int i = 0; i < 3; ++i) {
		mShineGetSaveOptions[i][0]
		    = (J2DTextBox*)mSaveScreen->search('sm3a' + 0x100 * i);
		mShineGetSaveOptions[i][0]->setFont((JUTFont*)gpSystemFont);
		mShineGetSaveOptions[i][1]
		    = (J2DTextBox*)mSaveScreen->search('sm3b' + 0x100 * i);
		mShineGetSaveOptions[i][1]->setFont((JUTFont*)gpSystemFont);

		mShineGetSavePointerHands[i][0]
		    = (J2DTextBox*)mSaveScreen->search('sc3a' + 0x100 * i);
		mShineGetSavePointerHands[i][1]
		    = (J2DTextBox*)mSaveScreen->search('sc3b' + 0x100 * i);
	}
	unk23C = mShineGetSavePointerHands[0][0]->getBounds().x1;

	mGameOverSaveWindow = new TExPane(mSaveScreen, '\0w_6');
	mGameOverSaveWindowInitialBounds
	    = mGameOverSaveWindow->getPane()->getBounds();

	for (int i = 0; i < 2; ++i) {
		mGameOverSavePointerHands[i][0]
		    = (J2DTextBox*)mSaveScreen->search('sc6a' + 0x100 * i);
		mGameOverSavePointerHands[i][1]
		    = (J2DTextBox*)mSaveScreen->search('sc6b' + 0x100 * i);

		mGameOverSaveOptions[i]
		    = (J2DTextBox*)mSaveScreen->search('sm7b' + 0x100 * i);
		((J2DTextBox*)mSaveScreen->search('sm7a' + 0x100 * i))
		    ->setFont((JUTFont*)gpSystemFont);
		mGameOverSaveOptions[i]->setFont((JUTFont*)gpSystemFont);
	}

	unk26C = mGameOverSavePointerHands[0][0]->getBounds().x1;

	mSaveScreen->search('mask')->hide();
	unk2E4 = JKRGetResource("/common/2d/savemessage.bmg");
	if (!unk18)
		unk2D8 = static_cast<TPauseMenu2*>(
		    JDrama::TNameRefGen::search("ポーズメニュー"));
	mGamePad = game_pad;
}

void TCardSave::init(int param_1)
{
	unk310 = PROGRESS_UNK0;
	unk2DF = 0;
	unk308 = param_1;
	unk2F8 = 0;
	unk2F9 = 1;
	if (unk308 == 7 || unk308 == 8 || unk308 == 9) {
		mSaveScreen->search('mask')->show();
		mSaveScreen->search('mask')->setAlpha(255);
	} else {
		mSaveScreen->search('mask')->hide();
	}
}

void TCardSave::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (unk2DF)
		return;

	if (cue & CUE_MOVE)
		execMovement_();

	if (cue & CUE_DRAW)
		execIssueGX_(graphics);
}

void TCardSave::makeBuffer(J2DTextBox* param_1, int size)
{
	SMSMakeTextBuffer(param_1, size);
	param_1->setFont((JUTFont*)gpSystemFont);
}

void TCardSave::setMessage(J2DTextBox* text_box, s32 param_2, u32 param_3)
{
	strncpy(text_box->getStringPtr(), SMSGetMessageData(unk2E4, param_3),
	        param_2);
}

s8 TCardSave::waitForStop(TEProgress param_1)
{
	s8 result = -1;

	switch (unk10) {
	case 0:
		setMessage(unkA0, 0x200, getCurMessageID());
		setMessage(unkA4, 0x200, getCurMessageID());
		setMessage(unkD8, 0x200, 0);
		setMessage(unkDC, 0x200, 0);

		unkA4->hide();
		unk48->getPane()->show();
		unk48->setCenteredSize(20, unk4C.getWidth(), unk4C.getHeight(), 0, 0);

		unkDC->hide();
		unkC4->setCenteredSize(20, unkC8.getWidth(), unkC8.getHeight(), 0, 0);

		if (unk310 == PROGRESS_UNK4 || unk310 == PROGRESS_UNK3
		    || unk310 == PROGRESS_UNK5 || unk310 == PROGRESS_UNKC
		    || unk310 == PROGRESS_UNKD || unk310 == PROGRESS_UNK2D) {
			unkC4->getPane()->show();
		} else {
			unkC4->getPane()->hide();
		}

		unk10 = 1;
		break;

	case 1: {
		u8 done = 1;
		done &= unk48->update();
		done &= unkC4->update();
		if (done) {
			unk10 = 2;
			unkA4->show();
			unkDC->show();
		}
		break;
	}

	case 2:
		if (mGamePad->checkFrameMeaning(TMarioGamePad::MEANING_MENU_A)) {
			gpMSound->startSoundSystemSE(0x481CU, 0, nullptr, 0);

			unk48->setCenteredSize(20, 0, 0, unk4C.getWidth(),
			                       unk4C.getHeight());
			unkC4->setCenteredSize(20, 0, 0, unkC8.getWidth(),
			                       unkC8.getHeight());
			unkA4->hide();
			unkDC->hide();
			unk10 = 3;
		}
		break;

	case 3: {
		u8 done = 1;
		done &= unk48->update();
		done &= unkC4->update();
		if (done) {
			unk48->getPane()->hide();
			unkC4->getPane()->hide();
			unk10 = 5;
		}
		break;
	}

	case 5:
		unk310 = param_1;
		result = 0;
		break;
	}

	return result;
}

void TCardSave::endWaitForChoice()
{
	if (unk10 != 2)
		return;

	mShineCountOver100->hide();
	mShineCountUnder100->hide();
	unk128->hide();
	unkF8->hide();
	mConfirmWindowTextShadow->hide();
	mConfirmWindowMarioText->hide();
	mConfirmWindowYes->getPane()->hide();
	mConfirmWindowNo->getPane()->hide();
	unk10 = 3;
	mConfirmWindow->setCenteredSize(20, 0, 0,
	                                mConfirmWindowInitialBounds.getWidth(),
	                                mConfirmWindowInitialBounds.getHeight());
}

s8 TCardSave::waitForChoice(TEProgress param_1, TEProgress param_2, s8 param_3)
{
	s8 result = -1;

	switch (unk10) {
	case 0:
		setMessage(unkF4, 0x200, getCurMessageID());
		setMessage(unkF8, 0x200, getCurMessageID());

		mShineCountOver100->hide();
		mShineCountUnder100->hide();
		unk128->hide();
		unkF8->hide();
		mConfirmWindowTextShadow->hide();
		mConfirmWindowMarioText->hide();
		mConfirmWindowYes->getPane()->hide();
		mConfirmWindowNo->getPane()->hide();

		mConfirmWindow->getPane()->show();
		mConfirmWindow->setCenteredSize(
		    20, mConfirmWindowInitialBounds.getWidth(),
		    mConfirmWindowInitialBounds.getHeight(), 0, 0);
		unk10 = 1;
		break;

	case 1:
		if (mConfirmWindow->update()) {
			unkF8->show();
			mConfirmWindowYes->getPane()->show();
			mConfirmWindowNo->getPane()->show();

			unk2E9 = param_3;
			if (param_3 == 0) {
				((J2DPicture*)mConfirmWindowYes->getPane())->mWhite
				    = JUtility::TColor(0, 0xFF, 0, 0xFF);
				((J2DPicture*)mConfirmWindowNo->getPane())->mWhite
				    = JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF);
			} else {
				((J2DPicture*)mConfirmWindowYes->getPane())->mWhite
				    = JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF);
				((J2DPicture*)mConfirmWindowNo->getPane())->mWhite
				    = JUtility::TColor(0, 0xFF, 0, 0xFF);
			}

			mConfirmWindowYes->updateCenteredSize(
			    1, mConfirmWindowYesInitialBounds.getWidth(),
			    mConfirmWindowYesInitialBounds.getHeight());
			mConfirmWindowNo->updateCenteredSize(
			    1, mConfirmWindowNoInitialBounds.getWidth(),
			    mConfirmWindowNoInitialBounds.getHeight());
			mConfirmWindowYes->update();
			mConfirmWindowNo->update();

			unk2FC = 0;
			unk10  = 2;
		}
		break;

	case 2: {
		s8 old    = unk2E9;
		u32 input = mGamePad->mEnabledFrameMeaning;

		if (input & TMarioGamePad::MEANING_MENU_A) {
			if (unk44 != nullptr)
				gpEmitterManager4D2->forceDeleteEmitter(unk44);

			if (unk2E9 == 0) {
				gpMSound->startSoundSystemSE(0x481CU, 0, nullptr, 0);
			} else {
				gpMSound->startSoundSystemSE(0x481DU, 0, nullptr, 0);
			}

			unkF8->hide();
			mConfirmWindowMarioText->hide();
			mConfirmWindowYes->getPane()->hide();
			mConfirmWindowNo->getPane()->hide();
			unk10 = 3;
			mConfirmWindow->setCenteredSize(
			    20, 0, 0, mConfirmWindowInitialBounds.getWidth(),
			    mConfirmWindowInitialBounds.getHeight());
		} else if (input & TMarioGamePad::MEANING_MENU_B) {
			if (unk44 != nullptr)
				gpEmitterManager4D2->forceDeleteEmitter(unk44);

			gpMSound->startSoundSystemSE(0x481DU, 0, nullptr, 0);

			unkF8->hide();
			mConfirmWindowMarioText->hide();
			mConfirmWindowYes->getPane()->hide();
			mConfirmWindowNo->getPane()->hide();
			unk10 = 3;
			mConfirmWindow->setCenteredSize(
			    20, 0, 0, mConfirmWindowInitialBounds.getWidth(),
			    mConfirmWindowInitialBounds.getHeight());
			unk2E9 = 1;
		} else if (input & TMarioGamePad::MEANING_MENU_LEFT) {
			unk2E9 = 0;
		} else if (input & TMarioGamePad::MEANING_MENU_RIGHT) {
			unk2E9 = 1;
		}

		TExPane* selectedPane;
		JUTRect selectedRect;
		if (unk2E9 == 0) {
			selectedPane = mConfirmWindowYes;
			selectedRect = mConfirmWindowYesInitialBounds;
		} else {
			selectedPane = mConfirmWindowNo;
			selectedRect = mConfirmWindowNoInitialBounds;
		}

		if (unk2FC == 4) {
			selectedPane->setCenteredSize(40, selectedRect.getWidth() * 1.5f,
			                              selectedRect.getHeight() * 1.5f,
			                              selectedRect.getWidth(),
			                              selectedRect.getHeight());
			if (!unk18) {
				JUTRect bounds = selectedPane->getPane()->mGlobalBounds;
				JGeometry::TVec3<f32> pos;
				pos.x = bounds.x1 + bounds.getWidth() * 0.5f;
				pos.y = bounds.y1 + bounds.getHeight() * 0.5f;
				pos.z = 0.0f;
				gpEmitterManager4D2->createEmitter(pos, 0x1FA, nullptr,
				                                   nullptr);

				unk44 = gpEmitterManager4D2->unkC8[0][0];
				unk44->setRotation(0, 0, DEG2SHORTANGLE(12));
				unk44->setEmitterScale(JGeometry::TVec3<f32>(0.9f, 1.0f, 0.1f));
			}
		} else if (unk2FC == 44) {
			selectedPane->setCenteredSize(40, selectedRect.getWidth(),
			                              selectedRect.getHeight(),
			                              selectedRect.getWidth() * 1.5f,
			                              selectedRect.getHeight() * 1.5f);
		}

		s32 thing = unk2FC < 40 ? unk2FC : 80 - unk2FC;
		thing     = (s32)(thing * 255.0f / 40.0f);
		((J2DPicture*)selectedPane->getPane())->mWhite
		    = (thing << 24) + 0xFF00FF;
		selectedPane->update();

		unk2FC += 1;
		if (unk2FC > 80)
			unk2FC = 0;

		if (old != unk2E9) {
			if (!unk18 && unk44 != nullptr)
				gpEmitterManager4D2->forceDeleteEmitter(unk44);

			unk2FC = 0;

			gpMSound->startSoundSystemSE(0x481EU, 0, nullptr, 0);

			mConfirmWindowYes->updateCenteredSize(
			    1, mConfirmWindowYesInitialBounds.getWidth(),
			    mConfirmWindowYesInitialBounds.getHeight());
			mConfirmWindowNo->updateCenteredSize(
			    1, mConfirmWindowNoInitialBounds.getWidth(),
			    mConfirmWindowNoInitialBounds.getHeight());
			mConfirmWindowYes->update();
			mConfirmWindowNo->update();

			if (unk2E9 == 0) {
				((J2DPicture*)mConfirmWindowYes->getPane())->mWhite
				    = JUtility::TColor(0, 0xFF, 0, 0xFF);
				((J2DPicture*)mConfirmWindowNo->getPane())->mWhite
				    = JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF);
			} else {
				((J2DPicture*)mConfirmWindowYes->getPane())->mWhite
				    = JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF);
				((J2DPicture*)mConfirmWindowNo->getPane())->mWhite
				    = JUtility::TColor(0, 0xFF, 0, 0xFF);
			}
		}
		break;
	}

	case 3:
		if (mConfirmWindow->update()) {
			mConfirmWindow->getPane()->hide();
			unk10 = 5;
		}
		break;

	case 5:
		unk310 = unk2E9 == 0 ? param_1 : param_2;
		result = unk2E9;
		break;
	}

	return result;
}

s8 TCardSave::waitForChoiceBM(TEProgress param_1, TEProgress param_2,
                              s8 param_3)
{
	s8 result = -1;

	switch (unk10) {
	case 0:
		if (unk2EA == -1) {
			unk10 = 5;
		}

		setMessage(mConfirmWindowText, 0x200, getCurMessageID());
		setMessage(mConfirmWindowTextShadow, 0x200, getCurMessageID());

		mShineCountOver100->hide();
		mShineCountUnder100->hide();
		unk128->hide();
		unkF8->hide();
		mConfirmWindowTextShadow->hide();
		mConfirmWindowMarioText->hide();
		mConfirmWindowYes->getPane()->hide();
		mConfirmWindowNo->getPane()->hide();
		mSaveNameABC[0]->hide();
		mSaveNameABC[1]->hide();
		mSaveNameABC[2]->hide();

		mConfirmWindow->getPane()->show();
		mConfirmWindow->setCenteredSize(
		    20, mConfirmWindowInitialBounds.getWidth(),
		    mConfirmWindowInitialBounds.getHeight(), 0, 0);
		unk10 = 1;
		break;

	case 1:
		if (mConfirmWindow->update()) {
			mSaveNameABC[unk2EA]->show();
			mConfirmWindowMarioText->show();
			mConfirmWindowTextShadow->show();

			if (unk310 == PROGRESS_UNK17) {
				unk128->hide();
				u16 score
				    = TFlagManager::getInstance()->getFlag(MSF_SHINE_COUNT);
				if (score < 100) {
					mShineCountTensUnder100->changeTexture(
					    mLargeDigitFontTextures[score / 10]->getTexInfo(), 0);
					mShineCountOnesUnder100->changeTexture(
					    mLargeDigitFontTextures[score % 10]->getTexInfo(), 0);
					mShineCountOver100->hide();
					mShineCountUnder100->show();
				} else {
					mShineCountHundredsOver100->changeTexture(
					    mLargeDigitFontTextures[score / 100]->getTexInfo(), 0);
					score -= 100;
					mShineCountTensOver100->changeTexture(
					    mLargeDigitFontTextures[score / 10]->getTexInfo(), 0);
					mShineCountOnesOver100->changeTexture(
					    mLargeDigitFontTextures[score % 10]->getTexInfo(), 0);
					mShineCountOver100->show();
					mShineCountUnder100->hide();
				}
			} else {
				if (getBookmarkInfo().unk0 == 1) {
					strncpy(unk124->getStringPtr(), "こわれています", 0x14);
					strncpy(unk128->getStringPtr(), "こわれています", 0x14);
					unk128->show();
					mShineCountOver100->hide();
					mShineCountUnder100->hide();
				} else if (getBookmarkInfo().unk18 == 0) {
					strncpy(unk124->getStringPtr(), "NEW", 0x14);
					strncpy(unk128->getStringPtr(), "NEW", 0x14);
					unk128->show();
					mShineCountOver100->hide();
					mShineCountUnder100->hide();
				} else {
					if (getBookmarkInfo().unk1C >= 100) {
						mShineCountOver100->show();
						mShineCountUnder100->hide();
					} else {
						mShineCountOver100->hide();
						mShineCountUnder100->show();
					}
					unk128->hide();
				}
			}

			mConfirmWindowYes->getPane()->show();
			mConfirmWindowNo->getPane()->show();
			mConfirmWindowYes->updateCenteredSize(
			    1, mConfirmWindowYesInitialBounds.getWidth(),
			    mConfirmWindowYesInitialBounds.getHeight());
			mConfirmWindowNo->updateCenteredSize(
			    1, mConfirmWindowNoInitialBounds.getWidth(),
			    mConfirmWindowNoInitialBounds.getHeight());
			mConfirmWindowYes->update();
			mConfirmWindowNo->update();
			unk2E9 = param_3;

			if (param_3 == 0) {
				((J2DPicture*)mConfirmWindowYes->getPane())->mWhite
				    = JUtility::TColor(0, 0xFF, 0, 0xFF);
				((J2DPicture*)mConfirmWindowNo->getPane())->mWhite
				    = JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF);
			} else {
				((J2DPicture*)mConfirmWindowYes->getPane())->mWhite
				    = JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF);
				((J2DPicture*)mConfirmWindowNo->getPane())->mWhite
				    = JUtility::TColor(0, 0xFF, 0, 0xFF);
			}

			unk10 = 2;
		}
		break;

	case 2: {
		s8 old    = unk2E9;
		u32 input = mGamePad->mEnabledFrameMeaning;

		if (input & TMarioGamePad::MEANING_MENU_A) {
			if (unk44 != nullptr)
				gpEmitterManager4D2->forceDeleteEmitter(unk44);

			if (unk2E9 == 0) {
				gpMSound->startSoundSystemSE(0x481CU, 0, nullptr, 0);
			} else {
				gpMSound->startSoundSystemSE(0x481DU, 0, nullptr, 0);
			}

			mShineCountOver100->hide();
			mShineCountUnder100->hide();
			unk128->hide();
			unkF8->hide();
			mConfirmWindowTextShadow->hide();
			mConfirmWindowMarioText->hide();
			mConfirmWindowYes->getPane()->hide();
			mConfirmWindowNo->getPane()->hide();
			unk10 = 3;
			mConfirmWindow->setCenteredSize(
			    20, 0, 0, mConfirmWindowInitialBounds.getWidth(),
			    mConfirmWindowInitialBounds.getHeight());
		} else if (input & TMarioGamePad::MEANING_MENU_LEFT) {
			unk2E9 = 0;
		} else if (input & TMarioGamePad::MEANING_MENU_RIGHT) {
			unk2E9 = 1;
		}

		TExPane* selectedPane;
		JUTRect selectedRect;
		if (unk2E9 == 0) {
			selectedPane = mConfirmWindowYes;
			selectedRect = mConfirmWindowYesInitialBounds;
		} else {
			selectedPane = mConfirmWindowNo;
			selectedRect = mConfirmWindowNoInitialBounds;
		}

		if (unk2FC == 4) {
			selectedPane->setCenteredSize(40, selectedRect.getWidth() * 1.5f,
			                              selectedRect.getHeight() * 1.5f,
			                              selectedRect.getWidth(),
			                              selectedRect.getHeight());
			if (!unk18) {
				JUTRect bounds = selectedPane->getPane()->getGlobalBounds();
				JGeometry::TVec3<f32> pos;
				pos.x = bounds.x1 + bounds.getWidth() * 0.5f;
				pos.y = bounds.y1 + bounds.getHeight() * 0.5f;
				pos.z = 0.0f;
				gpEmitterManager4D2->createEmitter(pos, 0x1FA, nullptr,
				                                   nullptr);

				unk44 = gpEmitterManager4D2->unkC8[0][0];
				unk44->setRotation(0, 0, DEG2SHORTANGLE(12));
				unk44->setEmitterScale(JGeometry::TVec3<f32>(0.9f, 1.0f, 0.1f));
			}
		} else if (unk2FC == 44) {
			selectedPane->setCenteredSize(40, selectedRect.getWidth(),
			                              selectedRect.getHeight(),
			                              selectedRect.getWidth() * 1.5f,
			                              selectedRect.getHeight() * 1.5f);
		}

		s32 thing = unk2FC < 40 ? unk2FC : 80 - unk2FC;
		thing     = (s32)(thing * 255.0f / 40.0f);
		((J2DPicture*)selectedPane->getPane())->mWhite
		    = (thing << 24) + 0xFF00FF;
		selectedPane->update();

		unk2FC += 1;
		if (unk2FC > 80)
			unk2FC = 0;

		if (old != unk2E9) {
			if (!unk18 && unk44 != nullptr)
				gpEmitterManager4D2->forceDeleteEmitter(unk44);

			unk2FC = 0;

			gpMSound->startSoundSystemSE(0x481EU, 0, nullptr, 0);

			mConfirmWindowYes->updateCenteredSize(
			    1, mConfirmWindowYesInitialBounds.getWidth(),
			    mConfirmWindowYesInitialBounds.getHeight());
			mConfirmWindowNo->updateCenteredSize(
			    1, mConfirmWindowNoInitialBounds.getWidth(),
			    mConfirmWindowNoInitialBounds.getHeight());
			mConfirmWindowYes->update();
			mConfirmWindowNo->update();
			if (unk2E9 == 0) {
				((J2DPicture*)mConfirmWindowYes->getPane())->mWhite
				    = JUtility::TColor(0, 0xFF, 0, 0xFF);
				((J2DPicture*)mConfirmWindowNo->getPane())->mWhite
				    = JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF);
			} else {
				((J2DPicture*)mConfirmWindowYes->getPane())->mWhite
				    = JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF);
				((J2DPicture*)mConfirmWindowNo->getPane())->mWhite
				    = JUtility::TColor(0, 0xFF, 0, 0xFF);
			}
		}
		break;
	}

	case 3:
		if (mConfirmWindow->update()) {
			mConfirmWindow->getPane()->hide();
			unk10 = 5;
		}
		break;

	case 5:
		unk310 = unk2E9 == 0 ? param_1 : param_2;
		result = unk2E9;
		break;
	}

	return result;
}

void TCardSave::endDrawMessage()
{
	if (unk10 != 2)
		return;

	mShineCountOver100->hide();
	mShineCountUnder100->hide();
	unk128->hide();
	unkF8->hide();
	mConfirmWindowTextShadow->hide();
	mConfirmWindowMarioText->hide();
	mConfirmWindowYes->getPane()->hide();
	mConfirmWindowNo->getPane()->hide();
	unk10 = 3;
	mConfirmWindow->setCenteredSize(20, 0, 0,
	                                mConfirmWindowInitialBounds.getWidth(),
	                                mConfirmWindowInitialBounds.getHeight());
}

s8 TCardSave::drawMessage(TEProgress param_1)
{
	s8 result = -1;
	switch (unk10) {
	case 0:
		setMessage(unk174, 0x200, getCurMessageID());
		setMessage(unk178, 0x200, getCurMessageID());
		unk178->hide();
		unk160->getPane()->show();
		unk160->setCenteredSize(20, unk164.getWidth(), unk164.getHeight(), 0,
		                        0);
		unk10 = 1;
		break;

	case 1:
		if (unk160->update()) {
			unk178->show();
			unk10 = 2;
		}
		break;

	case 3:
		if (unk160->update()) {
			unk160->getPane()->hide();
			unk10 = 5;
		}
		break;

	case 5:
		unk310 = param_1;
		result = 0;
		break;
	}
	return result;
}

s8 TCardSave::drawMessageBM(TEProgress param_1)
{

	s8 result = -1;

	switch (unk10) {
	case 0:
		if (unk2EA == -1) {
			unk10 = 5;
		}

		setMessage(mConfirmWindowText, 0x200, getCurMessageID());
		setMessage(mConfirmWindowTextShadow, 0x200, getCurMessageID());

		mShineCountOver100->hide();
		mShineCountUnder100->hide();
		unk128->hide();
		unkF8->hide();
		mConfirmWindowTextShadow->hide();
		mConfirmWindowMarioText->hide();
		mConfirmWindowYes->getPane()->hide();
		mConfirmWindowNo->getPane()->hide();
		mSaveNameABC[0]->hide();
		mSaveNameABC[1]->hide();
		mSaveNameABC[2]->hide();

		mConfirmWindow->getPane()->show();
		mConfirmWindow->setCenteredSize(
		    20, mConfirmWindowInitialBounds.getWidth(),
		    mConfirmWindowInitialBounds.getHeight(), 0, 0);
		unk10 = 1;
		break;

	case 1:
		if (mConfirmWindow->update()) {
			mConfirmWindowTextShadow->show();
			mSaveNameABC[unk2EA]->show();
			mConfirmWindowMarioText->show();

			if (unk308 == 0 || unk308 == 6 || unk308 == 8) {
				if (getBookmarkInfo().unk0 == 1) {
					strncpy(unk124->getStringPtr(), "こわれています", 0x14);
					strncpy(unk128->getStringPtr(), "こわれています", 0x14);
					unk128->show();
					mShineCountOver100->hide();
					mShineCountUnder100->hide();
				} else if (getBookmarkInfo().unk18 == 0) {
					strncpy(unk124->getStringPtr(), "NEW", 0x14);
					strncpy(unk128->getStringPtr(), "NEW", 0x14);
					unk128->show();
					mShineCountOver100->hide();
					mShineCountUnder100->hide();
				} else {
					unk128->hide();
					if (getBookmarkInfo().unk1C >= 100) {
						mShineCountOver100->show();
						mShineCountUnder100->hide();
					} else {
						mShineCountOver100->hide();
						mShineCountUnder100->show();
					}
				}
			}

			unk10 = 2;
		}
		break;

	case 3:
		if (mConfirmWindow->update()) {
			mConfirmWindow->getPane()->hide();
			unk10 = 5;
		}
		break;

	case 5:
		unk310 = param_1;
		result = 0;
		break;
	}

	return result;
}

s8 TCardSave::waitForAnyKey(TEProgress param_1)
{
	s8 result = -1;

	switch (unk10) {
	case 0:
		setMessage(unk174, 0x200, getCurMessageID());
		setMessage(unk178, 0x200, getCurMessageID());
		unk178->hide();
		unk160->getPane()->show();
		unk160->setCenteredSize(20, unk164.getWidth(), unk164.getHeight(), 0,
		                        0);
		unk10 = 1;
		break;

	case 1:
		if (unk160->update()) {
			unk178->show();
			unk10 = 2;
		}
		break;

	case 2:
		if (unk2E0 > 300) {
			unk178->hide();
			unk160->setCenteredSize(20, 0, 0, unk164.getWidth(),
			                        unk164.getHeight());
			unk10 = 3;
		} else {
			unk2E0++;
		}
		break;

	case 3:
		if (unk160->update()) {
			unk160->getPane()->hide();
			unk10 = 5;
		}
		break;

	case 5:
		unk310 = param_1;
		result = 0;
		break;
	}

	return result;
}

s8 TCardSave::waitForSelectOver()
{
	s8 result = -1;

	switch (unk10) {
	case 0:
		mGameOverSaveWindow->getPane()->show();
		mGameOverSaveWindow->setCenteredSize(
		    20, mGameOverSaveWindowInitialBounds.getWidth(),
		    mGameOverSaveWindowInitialBounds.getHeight(), 0, 0);
		mGameOverSaveOptions[0]->hide();
		mGameOverSavePointerHands[0][0]->hide();
		mGameOverSavePointerHands[0][1]->hide();
		mGameOverSaveOptions[1]->hide();
		mGameOverSavePointerHands[1][0]->hide();
		mGameOverSavePointerHands[1][1]->hide();
		unk10 = 1;
		break;

	case 1:
		if (mGameOverSaveWindow->update()) {
			unk10  = 2;
			unk2E9 = 0;
			mGameOverSaveOptions[0]->show();
			mGameOverSaveOptions[1]->show();
			mGameOverSavePointerHands[0][0]->show();
			mGameOverSavePointerHands[0][1]->show();
		}
		break;

	case 2: {
		s8 oldSelect = unk2E9;
		u32 input    = mGamePad->mEnabledFrameMeaning;

		if (input & TMarioGamePad::MEANING_MENU_A) {
			gpMSound->startSoundSystemSE(0x481CU, 0, nullptr, 0);
			unk10 = 4;
		} else if (input & TMarioGamePad::MEANING_MENU_UP) {
			unk2E9 = 0;
		} else if (input & TMarioGamePad::MEANING_MENU_DOWN) {
			unk2E9 = 1;
		}

		if (oldSelect != unk2E9) {
			gpMSound->startSoundSystemSE(0x481EU, 0, nullptr, 0);

			mGameOverSavePointerHands[oldSelect][0]->hide();
			mGameOverSavePointerHands[oldSelect][1]->hide();
			mGameOverSavePointerHands[unk2E9][0]->show();
			mGameOverSavePointerHands[unk2E9][1]->show();
		}
		break;
	}

	case 4:
		mGameOverSaveWindow->setCenteredSize(
		    20, 0, 0, mGameOverSaveWindowInitialBounds.getWidth(),
		    mGameOverSaveWindowInitialBounds.getHeight());
		mGameOverSaveOptions[0]->hide();
		mGameOverSaveOptions[1]->hide();
		mGameOverSavePointerHands[unk2E9][0]->hide();
		mGameOverSavePointerHands[unk2E9][1]->hide();
		unk10 = 3;
		break;

	case 3:
		if (mGameOverSaveWindow->update()) {
			mGameOverSaveWindow->getPane()->hide();
			unk10 = 5;
		}
		break;

	case 5:
		if (unk2E9 == 0) {
			unk310 = PROGRESS_UNK1;
		} else {
			unk310 = PROGRESS_UNK19;
		}
		result = unk2E9;
		break;
	}

	return result;
}

s8 TCardSave::waitForSelect2(TEProgress param_1, TEProgress param_2)
{
	s8 result = -1;

	switch (unk10) {
	case 0: {
		u32 messageID = cMessageID[unk310];
		switch (unk308) {
		case 1:
			messageID = 0x1C;
			break;
		case 2:
			messageID = 0x1D;
			break;
		case 3:
			messageID = 0x1E;
			break;
		case 4:
			messageID = 0x1F;
			break;
		case 5:
			messageID = 0x20;
			break;
		}

		setMessage(mEventSaveWindowText, 0x200, (u16)messageID);
		setMessage(mEventSaveWindowTextShadow, 0x200, (u16)messageID);

		mEventSaveWindowTextShadow->hide();
		mEventSaveWindow->getPane()->show();
		mEventSaveWindow->setCenteredSize(
		    20, mEventSaveWindowInitialBounds.getWidth(),
		    mEventSaveWindowInitialBounds.getHeight(), 0, 0);

		mEventSaveOptions[0][1]->hide();
		mEventSavePointerHands[0][0]->hide();
		mEventSavePointerHands[0][1]->hide();
		mEventSaveOptions[1][1]->hide();
		mEventSavePointerHands[1][0]->hide();
		mEventSavePointerHands[1][1]->hide();

		unk10 = 1;
		break;
	}

	case 1:
		if (mEventSaveWindow->update()) {
			mEventSaveWindowTextShadow->show();
			unk10  = 2;
			unk2E9 = 0;

			mEventSaveOptions[0][1]->show();
			mEventSaveOptions[1][1]->show();
			mEventSavePointerHands[0][0]->show();
			mEventSavePointerHands[0][1]->show();

			unk2F8 = 0;
			unk2F9 = 1;
		}
		break;

	case 2: {
		s8 oldSelect = unk2E9;
		u32 input    = mGamePad->mEnabledFrameMeaning;

		if (input & TMarioGamePad::MEANING_MENU_A) {
			gpMSound->startSoundSystemSE(0x481CU, 0, nullptr, 0);
			unk10 = 4;
		} else if (input & TMarioGamePad::MEANING_MENU_UP) {
			unk2E9 = 0;
		} else if (input & TMarioGamePad::MEANING_MENU_DOWN) {
			unk2E9 = 1;
		}

		if (oldSelect != unk2E9) {
			gpMSound->startSoundSystemSE(0x481EU, 0, nullptr, 0);

			mEventSavePointerHands[oldSelect][0]->hide();
			mEventSavePointerHands[oldSelect][1]->hide();
			mEventSavePointerHands[unk2E9][0]->show();
			mEventSavePointerHands[unk2E9][1]->show();

			unk2F8 = 0;
			unk2F9 = 1;
		}
		break;
	}

	case 4:
		mEventSaveWindowTextShadow->hide();
		mEventSaveWindow->setCenteredSize(
		    20, 0, 0, mEventSaveWindowInitialBounds.getWidth(),
		    mEventSaveWindowInitialBounds.getHeight());

		mEventSaveOptions[0][1]->hide();
		mEventSaveOptions[1][1]->hide();
		mEventSavePointerHands[unk2E9][0]->hide();
		mEventSavePointerHands[unk2E9][1]->hide();

		unk10 = 3;
		break;

	case 3:
		if (mEventSaveWindow->update()) {
			mEventSaveWindow->getPane()->hide();
			unk10 = 5;
		}
		break;

	case 5:
		if (unk2E9 == 0) {
			unk310 = param_1;
		} else {
			unk310 = param_2;
		}
		result = unk2E9;
		break;
	}

	return result;
}

s8 TCardSave::waitForSelect3(TEProgress param_1, TEProgress param_2,
                             TEProgress param_3)
{
	s8 result = -1;

	switch (unk10) {
	case 0:
		mShineGetSaveWindow->getPane()->show();
		mShineGetSaveWindow->setCenteredSize(
		    20, mShineGetSaveWindowInitialBounds.getWidth(),
		    mShineGetSaveWindowInitialBounds.getHeight(), 0, 0);

		mShineGetSaveWindowTextShadow->hide();
		setMessage(mShineGetSaveWindowText, 0x200, 0x21);
		setMessage(mShineGetSaveWindowTextShadow, 0x200, 0x21);

		mShineGetSaveOptions[0][1]->hide();
		mShineGetSavePointerHands[0][0]->hide();
		mShineGetSavePointerHands[0][1]->hide();
		mShineGetSaveOptions[1][1]->hide();
		mShineGetSavePointerHands[1][0]->hide();
		mShineGetSavePointerHands[1][1]->hide();
		mShineGetSaveOptions[2][1]->hide();
		mShineGetSavePointerHands[2][0]->hide();
		mShineGetSavePointerHands[2][1]->hide();

		mShineGetSaveNameABC[0]->hide();
		mShineGetSaveNameABC[1]->hide();
		mShineGetSaveNameABC[2]->hide();
		unk20C->hide();
		unk210->hide();
		unk214->hide();
		mShineGetSaveMarioText->hide();

		unk10 = 1;
		break;

	case 1:
		if (mShineGetSaveWindow->update()) {
			unk10  = 2;
			unk2E9 = 0;

			mShineGetSaveOptions[0][1]->show();
			mShineGetSaveOptions[1][1]->show();
			mShineGetSaveOptions[2][1]->show();
			mShineGetSavePointerHands[0][0]->show();
			mShineGetSavePointerHands[0][1]->show();
			mShineGetSaveWindowTextShadow->show();

			mShineGetSaveNameABC[unk2EA]->show();
			mShineGetSaveMarioText->show();

			if (getBookmarkInfo().unk0 == 1) {
				strncpy(unk208->getStringPtr(), "こわれています", 0x14);
				strncpy(unk20C->getStringPtr(), "こわれています", 0x14);
				unk20C->show();
				unk210->hide();
				unk214->hide();
			} else if (getBookmarkInfo().unk18 == 0) {
				strncpy(unk208->getStringPtr(), "NEW", 0x14);
				strncpy(unk20C->getStringPtr(), "NEW", 0x14);
				unk20C->show();
				unk210->hide();
				unk214->hide();
			} else {
				s16 score = (s16)getBookmarkInfo().unk1C;
				if (score < 0) {
					score = 0;
				}

				if (score < 100) {
					((J2DPicture*)unk224)
					    ->changeTexture(
					        mLargeDigitFontTextures[score / 10]->getTexInfo(),
					        0);
					((J2DPicture*)unk228)
					    ->changeTexture(
					        mLargeDigitFontTextures[score % 10]->getTexInfo(),
					        0);
					unk214->show();
				} else {
					((J2DPicture*)unk218)
					    ->changeTexture(
					        mLargeDigitFontTextures[score / 100]->getTexInfo(),
					        0);
					score -= 100;
					((J2DPicture*)unk21C)
					    ->changeTexture(
					        mLargeDigitFontTextures[score / 10]->getTexInfo(),
					        0);
					((J2DPicture*)unk220)
					    ->changeTexture(
					        mLargeDigitFontTextures[score % 10]->getTexInfo(),
					        0);
					unk210->show();
				}

				unk20C->hide();
			}
		}
		break;

	case 2: {
		s8 oldSelect = unk2E9;
		u32 input    = mGamePad->mEnabledFrameMeaning;

		if (input & TMarioGamePad::MEANING_MENU_A) {
			gpMSound->startSoundSystemSE(0x481CU, 0, nullptr, 0);
			unk10 = 4;
		} else if (input & TMarioGamePad::MEANING_MENU_UP) {
			if (oldSelect > 0) {
				unk2E9 = oldSelect - 1;
			}
		} else if (input & TMarioGamePad::MEANING_MENU_DOWN) {
			if (oldSelect < 2) {
				unk2E9 = oldSelect + 1;
			}
		}

		if (oldSelect != unk2E9) {
			gpMSound->startSoundSystemSE(0x481EU, 0, nullptr, 0);

			mShineGetSavePointerHands[oldSelect][0]->hide();
			mShineGetSavePointerHands[oldSelect][1]->hide();
			mShineGetSavePointerHands[unk2E9][0]->show();
			mShineGetSavePointerHands[unk2E9][1]->show();
		}
		break;
	}

	case 4:
		mShineGetSaveWindow->setCenteredSize(
		    20, 0, 0, mShineGetSaveWindowInitialBounds.getWidth(),
		    mShineGetSaveWindowInitialBounds.getHeight());

		mShineGetSaveOptions[0][1]->hide();
		mShineGetSaveOptions[1][1]->hide();
		mShineGetSaveOptions[2][1]->hide();
		mShineGetSavePointerHands[unk2E9][0]->hide();
		mShineGetSavePointerHands[unk2E9][1]->hide();

		mShineGetSaveNameABC[0]->hide();
		mShineGetSaveNameABC[1]->hide();
		mShineGetSaveNameABC[2]->hide();
		mShineGetSaveWindowTextShadow->hide();
		unk20C->hide();
		unk210->hide();
		unk214->hide();
		mShineGetSaveMarioText->hide();

		unk10 = 3;
		break;

	case 3:
		if (mShineGetSaveWindow->update()) {
			mShineGetSaveWindow->getPane()->hide();
			unk10 = 5;
		}
		break;

	case 5:
		if (unk2E9 == 0) {
			unk310 = param_1;
		} else if (unk2E9 == 1) {
			unk310 = param_2;
		} else {
			unk310 = param_3;
		}
		result = unk2E9;
		break;
	}

	return result;
}

s8 TCardSave::waitForAnyKeyBM(TEProgress param_1)
{
	s8 result = -1;

	switch (unk10) {
	case 0:
		setMessage(mConfirmWindowText, 0x200, getCurMessageID());
		setMessage(mConfirmWindowTextShadow, 0x200, getCurMessageID());

		unkF8->hide();
		unk128->hide();
		mShineCountOver100->hide();
		mShineCountUnder100->hide();
		unk128->hide();
		mShineCountOver100->hide();
		mShineCountUnder100->hide();
		unk128->hide();
		mShineCountOver100->hide();
		mShineCountUnder100->hide();
		mConfirmWindowYes->getPane()->hide();
		mConfirmWindowNo->getPane()->hide();

		mConfirmWindow->getPane()->show();
		mConfirmWindow->setCenteredSize(
		    20, mConfirmWindowInitialBounds.getWidth(),
		    mConfirmWindowInitialBounds.getHeight(), 0, 0);
		unk10 = 1;
		break;

	case 1:
		if (mConfirmWindow->update()) {
			mSaveNameABC[unk2EA]->show();
			mConfirmWindowMarioText->show();
			mConfirmWindowTextShadow->show();

			if (getBookmarkInfo().unk0 == 1) {
				strncpy(unk124->getStringPtr(), "こわれています", 0x14);
				strncpy(unk128->getStringPtr(), "こわれています", 0x14);
				unk128->show();
				mShineCountOver100->hide();
				mShineCountUnder100->hide();
			} else if (getBookmarkInfo().unk18 == 0) {
				strncpy(unk124->getStringPtr(), "NEW", 0x14);
				strncpy(unk128->getStringPtr(), "NEW", 0x14);
				unk128->show();
				mShineCountOver100->hide();
				mShineCountUnder100->hide();
			} else {
				unk128->hide();
				u16 score = getBookmarkInfo().unk1C;
				if (score < 100) {
					((J2DPicture*)mShineCountTensUnder100)
					    ->changeTexture(
					        mLargeDigitFontTextures[score / 10]->getTexInfo(),
					        0);
					((J2DPicture*)mShineCountOnesUnder100)
					    ->changeTexture(
					        mLargeDigitFontTextures[score % 10]->getTexInfo(),
					        0);
					mShineCountOver100->hide();
					mShineCountUnder100->show();
				} else {
					((J2DPicture*)mShineCountHundredsOver100)
					    ->changeTexture(
					        mLargeDigitFontTextures[score / 100]->getTexInfo(),
					        0);
					score -= 100;
					((J2DPicture*)mShineCountTensOver100)
					    ->changeTexture(
					        mLargeDigitFontTextures[score / 10]->getTexInfo(),
					        0);
					((J2DPicture*)mShineCountOnesOver100)
					    ->changeTexture(
					        mLargeDigitFontTextures[score % 10]->getTexInfo(),
					        0);
					mShineCountOver100->show();
					mShineCountUnder100->hide();
				}
			}

			unk10 = 2;
		}
		break;

	case 2:
		if (unk2E0 > 300) {
			endWaitForChoice();
			unk10 = 3;
		} else {
			unk2E0++;
		}
		break;

	case 3:
		if (mConfirmWindow->update()) {
			mConfirmWindow->getPane()->hide();
			unk10 = 5;
		}
		break;

	case 5:
		unk310 = param_1;
		result = 0;
		break;
	}

	return result;
}

void saveBookmark(u32 idx)
{
	JSUMemoryOutputStream stream;
	gpCardManager->getWriteStream(&stream);
	TFlagManager::getInstance()->save(stream);
	gpCardManager->writeBlock(idx);
}

void TCardSave::selectBookmarks(TEProgress, TEProgress, TEProgress, TEProgress)
{
}

void TCardSave::changePattern(J2DPicture*, s16, u32) { }

void TCardSave::execMovement_()
{

	TEProgress prevState = unk310;
	s32 prevMode         = unk10;

	mGamePad->onFlag(TMarioGamePad::PAD_FLAG_MENU_INPUT);

	switch (unk310) {
	case PROGRESS_UNK0:
		unk2EA = SMSGetApplication()->mSaveFile;

		if (unk308 == 7) {
			unk310 = PROGRESS_UNK2E;
			break;
		}
		if (unk308 == 9) {
			unk310 = PROGRESS_UNK34;
			break;
		}

		unk310 = PROGRESS_UNK2;
		gpCardManager->getBookmarkInfos(&unk278[0]);
		break;

	case PROGRESS_UNK1:
	case PROGRESS_UNK19:
		mGamePad->offFlag(TMarioGamePad::PAD_FLAG_MENU_INPUT);
		gpCardManager->unmount();

		if (!unk18 && unk308 == 0)
			unk2D8->setDrawEnd();

		unk2DF = 1;
		break;

	case PROGRESS_UNK2: {
		s32 status = gpCardManager->getLastStatus();
		if (status == CARD_RESULT_BUSY)
			break;

		if (status == CARD_RESULT_READY) {
			u16 score = getBookmarkInfo().unk1C;
			if (score > 0x3E7)
				score = 0x3E7;

			if (score < 100) {
				mShineCountTensUnder100->changeTexture(
				    mLargeDigitFontTextures[score / 10]->getTexInfo(), 0);
				mShineCountOnesUnder100->changeTexture(
				    mLargeDigitFontTextures[score % 10]->getTexInfo(), 0);
			} else {
				mShineCountHundredsOver100->changeTexture(
				    mLargeDigitFontTextures[score / 100]->getTexInfo(), 0);
				score -= int(score * 0.01f) * 100;
				mShineCountTensOver100->changeTexture(
				    mLargeDigitFontTextures[score / 10]->getTexInfo(), 0);
				mShineCountOnesOver100->changeTexture(
				    mLargeDigitFontTextures[score % 10]->getTexInfo(), 0);
			}

			switch (unk308) {
			case 0:
				unk310 = PROGRESS_UNK15;
				break;
			case 6:
				unk310 = PROGRESS_UNK2B;
				break;
			case 8:
				unk310 = PROGRESS_UNK33;
				break;
			default:
				unk310 = PROGRESS_UNK2A;
				break;
			}
		} else {
			unk310 = changeMode(status);
		}
		break;
	}

	case PROGRESS_UNK3:
		if (gpCardManager->getLastStatus() == CARD_RESULT_NOCARD) {
			waitForStop(PROGRESS_UNK1);
			gpCardManager->probe();
		} else {
			if (unkA4->isVisible()) {
				unk48->setCenteredSize(20, 0, 0, unk4C.getWidth(),
				                       unk4C.getHeight());
				unkA4->hide();
				unk10 = 3;
			}

			if (waitForStop(PROGRESS_UNK2) != -1)
				gpCardManager->getBookmarkInfos(&unk278[0]);
		}
		break;

	case PROGRESS_UNK4:
	case PROGRESS_UNK5:
	case PROGRESS_UNKC:
	case PROGRESS_UNKD:
	case PROGRESS_UNK2D:
		if (gpCardManager->getLastStatus() != CARD_RESULT_NOCARD) {
			waitForStop(PROGRESS_UNK1);
			gpCardManager->probe();
		} else {
			if (unkA4->isVisible()) {
				unk48->setCenteredSize(20, 0, 0, unk4C.getWidth(),
				                       unk4C.getHeight());
				unkA4->hide();
				unk10 = 3;
			}
			waitForStop(PROGRESS_UNK3);
		}
		break;

	case PROGRESS_UNK6:
	case PROGRESS_UNK7:
		if (gpCardManager->getLastStatus() != CARD_RESULT_NOCARD) {
			waitForChoice(PROGRESS_UNK8, PROGRESS_UNK4, 1);
			gpCardManager->probe();
		} else {
			if (unkF8->isVisible())
				endWaitForChoice();

			waitForChoice(PROGRESS_UNK3, PROGRESS_UNK3, 1);
		}
		break;

	case PROGRESS_UNK8: {
		s32 status = gpCardManager->getLastStatus();
		if (status == CARD_RESULT_BUSY)
			break;

		if (status == CARD_RESULT_READY) {
			s8 r = waitForChoice(PROGRESS_UNK9, PROGRESS_UNK4, 1);
			if (r == 0) {
				unk2E0 = 0;
				gpCardManager->format();
			}
			gpCardManager->probe();
		} else {
			endWaitForChoice();

			TEProgress mode = changeMode(status);
			waitForChoice(mode, mode, 1);
		}
		break;
	}

	case PROGRESS_UNK9: {
		s32 status = gpCardManager->getLastStatus();
		if (status != CARD_RESULT_BUSY) {
			if (status == CARD_RESULT_READY) {
				drawMessage(PROGRESS_UNKA);
				if (unk2E0 > 0x12C) {
					if (unk178->isVisible()) {
						unk178->hide();
						unk160->setCenteredSize(20, 0, 0, unk164.getWidth(),
						                        unk164.getHeight());
						unk10 = 3;
					}
				} else {
					unk2E0++;
				}
			} else {
				if (unk178->isVisible()) {
					unk178->hide();
					unk160->setCenteredSize(20, 0, 0, unk164.getWidth(),
					                        unk164.getHeight());
					unk10 = 3;
				}
				drawMessage(PROGRESS_UNKB);
			}
		} else {
			drawMessage(PROGRESS_UNK0);
		}
		break;
	}

	case PROGRESS_UNKA:
		if (gpCardManager->getLastStatus() == CARD_RESULT_READY) {
			s32 r = waitForAnyKey(PROGRESS_UNK2);
			gpCardManager->probe();
			if (r != -1)
				gpCardManager->getBookmarkInfos(&unk278[0]);
		} else {
			if (unk178->isVisible()) {
				unk160->setCenteredSize(20, 0, 0, unk164.getWidth(),
				                        unk164.getHeight());
				unk178->hide();
			}
			waitForAnyKey(PROGRESS_UNK3);
		}
		break;

	case PROGRESS_UNKB:
		if (waitForAnyKey(PROGRESS_UNK2) != -1)
			gpCardManager->getBookmarkInfos(&unk278[0]);
		break;

	case PROGRESS_UNKE:
		if (unk10 == 0)
			gpCardManager->probe();

		if (gpCardManager->getLastStatus() == CARD_RESULT_READY) {
			s32 r = waitForChoice(PROGRESS_UNKF, PROGRESS_UNK10, 0);
			gpCardManager->probe();
			if (r == 0)
				gpCardManager->createFile();
		} else {
			endWaitForChoice();
			waitForChoice(PROGRESS_UNK3, PROGRESS_UNK3, 1);
		}
		break;

	case PROGRESS_UNKF: {
		s32 status = gpCardManager->getLastStatus();
		if (status != CARD_RESULT_BUSY) {
			if (status == CARD_RESULT_READY) {
				drawMessage(PROGRESS_UNK11);
				if (unk2E0 > 0x12C) {
					if (unk178->isVisible()) {
						unk178->hide();
						unk160->setCenteredSize(20, 0, 0, unk164.getWidth(),
						                        unk164.getHeight());
						unk10 = 3;
					}
				} else {
					unk2E0++;
				}
			} else {
				if (unk178->isVisible()) {
					unk178->hide();
					unk160->setCenteredSize(20, 0, 0, unk164.getWidth(),
					                        unk164.getHeight());
					unk10 = 3;
				}
				drawMessage(PROGRESS_UNK12);
			}
		} else {
			drawMessage(PROGRESS_UNK0);
		}
		break;
	}

	case PROGRESS_UNK11:
		if (waitForAnyKey(PROGRESS_UNK2) != -1)
			gpCardManager->getBookmarkInfos(&unk278[0]);
		break;

	case PROGRESS_UNK12:
		if (waitForAnyKey(PROGRESS_UNK2) != -1)
			gpCardManager->getBookmarkInfos(&unk278[0]);
		break;

	case PROGRESS_UNK10:
		waitForStop(PROGRESS_UNK1);
		break;

	case PROGRESS_UNK13: {
		unk2E8 = unk2EA;

		TCardBookmarkInfo& bm = getBookmarkInfo();
		if (bm.unk0 == 1) {
			unk310 = PROGRESS_UNK14;
		} else if (bm.unk18 == 0) {
			unk310 = PROGRESS_UNK16;
			saveBookmark(unk2EA);
		} else if (bm.unk18 == 0
		           || bm.unk8
		                  == TFlagManager::getInstance()->getLastSaveTime()) {
			unk310 = PROGRESS_UNK16;
		} else {
			unk310 = PROGRESS_UNK2C;
		}
		break;
	}

	case PROGRESS_UNK14:
	case PROGRESS_UNK1E: {
		s32 status = gpCardManager->getLastStatus();
		if (status == CARD_RESULT_READY) {
			s32 r = waitForChoiceBM(PROGRESS_UNK16, PROGRESS_UNK2, 1);
			gpCardManager->probe();
			if (r == 0)
				saveBookmark(unk2EA);
		} else {
			endWaitForChoice();

			TEProgress mode = changeMode(status);
			waitForChoiceBM(mode, mode, 1);
		}
		break;
	}

	case PROGRESS_UNK15: {
		s32 status = gpCardManager->getLastStatus();
		if (status == CARD_RESULT_READY) {
			s32 r = waitForChoiceBM(PROGRESS_UNK16, PROGRESS_UNK1, 1);
			gpCardManager->probe();
			if (r == 0) {
				TCardBookmarkInfo& bm = getBookmarkInfo();
				if (bm.unk0 == 1) {
					unk310 = PROGRESS_UNK1E;
				} else {
					if (bm.unk18 == 0
					    || bm.unk8
					           == TFlagManager::getInstance()
					                  ->getLastSaveTime()) {

						saveBookmark(unk2EA);
					} else {
						unk310 = PROGRESS_UNK2C;
					}
				}
			}
		} else {
			endWaitForChoice();

			waitForChoiceBM(PROGRESS_UNK3, PROGRESS_UNK3, 1);
		}
		break;
	}

	case PROGRESS_UNK16: {
		s32 status = gpCardManager->getLastStatus();

		if (status != CARD_RESULT_BUSY) {
			if (status == CARD_RESULT_READY) {
				if (unk2DC != 0) {
					unk2DC = 0;
					if (unk2DD == 0)
						TFlagManager::getInstance()->saveSuccess();
				}

				s8 r = drawMessageBM(PROGRESS_UNK17);
				if (r != -1 && unk2DE != 0)
					unk310 = PROGRESS_UNK19;

				if (unk2E0 > 0x12C) {
					if (unk2DD != 0) {
						endWaitForChoice();
					} else {
						JSUMemoryOutputStream stream;
						gpCardManager->getOptionWriteStream(&stream);
						TFlagManager::getInstance()->saveOption(stream);
						gpCardManager->writeOptionBlock();
						unk2DD = 1;
					}
				} else {
					if (unk2DD == 0 && unk2E0 == 0xC8) {
						gpMSound->startSoundSystemSE(MSD_SE_SY_DECIDE_COMMON, 0,
						                             nullptr, 0);

						if (unk308 == 0 || unk308 == 6 || unk308 == 8) {
							unk128->hide();
							u16 score = TFlagManager::getInstance()->getFlag(
							    MSF_SHINE_COUNT);
							if (score > 0x3E7)
								score = 0x3E7;

							if (score < 100) {
								mShineCountUnder100->show();
								mShineCountOver100->hide();
								mShineCountTensUnder100->changeTexture(
								    mLargeDigitFontTextures[score / 10]
								        ->getTexInfo(),
								    0);
								mShineCountOnesUnder100->changeTexture(
								    mLargeDigitFontTextures[score % 10]
								        ->getTexInfo(),
								    0);
							} else {
								mShineCountOver100->show();
								mShineCountUnder100->hide();
								mShineCountHundredsOver100->changeTexture(
								    mLargeDigitFontTextures[score / 100]
								        ->getTexInfo(),
								    0);
								score -= ((s32)((f32)score * 0.01f)) * 100;
								mShineCountTensOver100->changeTexture(
								    mLargeDigitFontTextures[score / 10]
								        ->getTexInfo(),
								    0);
								mShineCountOnesOver100->changeTexture(
								    mLargeDigitFontTextures[score % 10]
								        ->getTexInfo(),
								    0);
							}
						}

						unk2E0++;
					} else {
						unk2E0++;
					}
				}
				gpCardManager->probe();
			} else {
				drawMessageBM(PROGRESS_UNK18);

				endWaitForChoice();

				if (unk2DC != 0) {
					unk2DC = 0;
					if (unk2DD == 0)
						TFlagManager::getInstance()->saveFail();
				}
			}
		} else {
			gpCardManager->probe();
			drawMessageBM(PROGRESS_UNK0);
			unk2E0++;
		}
		break;
	}

	case PROGRESS_UNK17:
		if (unk308 != 0 || unk18) {
			unk310 = PROGRESS_UNK1;
		} else {
			waitForChoiceBM(PROGRESS_UNK1, PROGRESS_UNK19, 0);
		}
		break;

	case PROGRESS_UNK18:
		if (waitForAnyKeyBM(PROGRESS_UNK2) != -1)
			gpCardManager->getBookmarkInfos(&unk278[0]);
		break;

	case PROGRESS_UNK2A:
		if (gpCardManager->getLastStatus() == CARD_RESULT_READY) {
			s32 r = waitForSelect2(PROGRESS_UNK16, PROGRESS_UNK1);
			gpCardManager->probe();
			if (r == 0) {
				TCardBookmarkInfo& bm = getBookmarkInfo();
				if (bm.unk0 == 1) {
					unk310 = PROGRESS_UNK1E;
				} else {
					if (bm.unk18 == 0
					    || bm.unk8
					           == TFlagManager::getInstance()
					                  ->getLastSaveTime()) {
						saveBookmark(unk2EA);
						unk310 = PROGRESS_UNK16;
					} else {
						unk310 = PROGRESS_UNK2C;
					}
				}
			}
		} else {
			if (unk10 < 3)
				unk10 = 4;
			waitForSelect2(PROGRESS_UNK3, PROGRESS_UNK3);
		}
		break;

	case PROGRESS_UNK2B:
		if (gpCardManager->getLastStatus() == CARD_RESULT_READY) {
			s32 r
			    = waitForSelect3(PROGRESS_UNK16, PROGRESS_UNK16, PROGRESS_UNK1);
			gpCardManager->probe();

			if (r != -1) {
				if (r == 0 || r == 1) {
					TCardBookmarkInfo& bm = getBookmarkInfo();
					if (bm.unk0 == 1) {
						unk310 = PROGRESS_UNK1E;
					} else {
						if (bm.unk18 == 0
						    || bm.unk8
						           == TFlagManager::getInstance()
						                  ->getLastSaveTime()) {
							saveBookmark(unk2EA);
							unk310 = PROGRESS_UNK16;
						} else {
							unk310 = PROGRESS_UNK2C;
						}
					}

					if (r == 0) {
						unk2DE = 0;
					} else if (r == 1) {
						unk2DE = 1;
					}
				} else {
					unk310 = PROGRESS_UNK1;
				}
			}
		} else {
			if (unk10 < 3)
				unk10 = 4;
			waitForSelect3(PROGRESS_UNK3, PROGRESS_UNK3, PROGRESS_UNK3);
		}
		break;

	case PROGRESS_UNK2C:
		if (gpCardManager->getLastStatus() != CARD_RESULT_NOCARD) {
			waitForStop(PROGRESS_UNK1);
			gpCardManager->probe();
		} else {
			if (unkA4->isVisible()) {
				unk48->setCenteredSize(20, 0, 0, unk4C.getWidth(),
				                       unk4C.getHeight());
				unkA4->hide();
				unk10 = 3;
			}
			waitForStop(PROGRESS_UNK3);
		}
		break;

	case PROGRESS_UNK2E:
		waitForSelectOver();
		break;

	case PROGRESS_UNK33: {
		s32 status = gpCardManager->getLastStatus();
		if (status == CARD_RESULT_READY) {
			s32 r = waitForChoiceBM(PROGRESS_UNK16, PROGRESS_UNK1, 1);
			gpCardManager->probe();
			if (r == 0) {
				TCardBookmarkInfo& bm = getBookmarkInfo();
				if (bm.unk0 == 1) {
					unk310 = PROGRESS_UNK1E;
				} else {
					if (bm.unk18 == 0
					    || bm.unk8
					           == TFlagManager::getInstance()
					                  ->getLastSaveTime()) {
						saveBookmark(unk2EA);
						unk310 = PROGRESS_UNK16;
					} else {
						unk310 = PROGRESS_UNK2C;
					}
				}
			}
		} else {
			endWaitForChoice();

			TEProgress mode = changeMode(status);
			waitForChoiceBM(mode, mode, 1);
		}
		break;
	}

	case PROGRESS_UNK34:
		waitForChoice(PROGRESS_UNK19, PROGRESS_UNK1, 1);
		break;

	default:
		break;
	}

	if (prevState != unk310) {
		unk314 = unk310;
		unk318 = 0;
		unk2E0 = 0;
		unk2F8 = 0;
		unk2F9 = 1;
		unk2DC = 1;
		unk30C = 0;
		unk2DD = 0;
		unk10  = 0;
		unk44  = nullptr;
	}

	if (prevMode != unk10) {
		if (prevMode == 1) {
			gpMSound->startSoundSystemSE(MSD_SE_SY_2D_IN, 0, nullptr, 0);
		}

		if (unk10 == 3) {
			gpMSound->startSoundSystemSE(MSD_SE_SY_2D_OUT, 0, nullptr, 0);
		}
	}
}

u8 TCardSave::getNextState()
{
	if (unk2DF != 0) {
		if (unk310 == 0x19)
			return 1;

		if (unk310 == PROGRESS_UNK1)
			return 0;

		return 3;
	}

	return 3;
}

void TCardSave::execIssueGX_(JDrama::TGraphics* param_1)
{
	const JDrama::TRect& scissorRect = param_1->getScissor();
	{
		J2DOrthoGraph graph(param_1->getViewport());
		graph.setup2D();
		mSaveScreen->draw(0, 0, &graph);
	}

	param_1->setScissor(scissorRect);
}
