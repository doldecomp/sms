#include <GC2D/CardSave.hpp>
#include <stdio.h>
#include <version.h>
#include <JSystem/JSupport/JSUMemoryOutputStream.hpp>
#include <JSystem/JSupport/JSUMemoryInputStream.hpp>
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
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

extern JPAEmitterManager* gpEmitterManager4D2;

#define CARD_SAVE_FORMAT_CANCEL                                                \
	VERSION_SELECT(PROGRESS_CONTINUE_WITHOUT_SAVING,                           \
	               GMSJ01(PROGRESS_WRONG_DEVICE))

u32 TCardSave::cMessageID[] = {
#if defined(VERSION_GMSJ01)
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xC,        0xF,        0x12,
	0x4,        0x5,        0x6,        0x16,       0x1A,       0x14,
	0xE,        0xD,        0x1,        0xA,        0x10,       0x19,
	0x13,       0x18,       0x2,        0x8,        0x17,       0x1B,
	0x15,       0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0x3,        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0x11,       0xB,        0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x9,        0x7,
#else
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xE,        0x11,       0x14,
	0x5,        0x6,        0x7,        0x18,       0x22,       0x16,
	0x10,       0xF,        0x2,        0xB,        0x12,       0x21,
	0x15,       0x1B,       0x3,        0x9,        0x1A,       0x23,
	0x17,       0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0x4,        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0x13,       0xD,        0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xA,        0x8,        0xC,
#endif
};

TEProgress TCardSave::changeMode(s32 param_1)
{
	TEProgress result = PROGRESS_WRONG_DEVICE;
	switch (param_1) {
	case CARD_RESULT_NOCARD:
		result = PROGRESS_NO_CARD;
		break;
	case CARD_RESULT_FATAL_ERROR:
	case CARD_RESULT_WRONGDEVICE:
		result = PROGRESS_WRONG_DEVICE;
		break;
	case CARD_RESULT_IOERROR:
		result = PROGRESS_IO_ERROR;
		break;
	case -0x100:
		result = PROGRESS_INCOMPATIBLE_CARD;
		break;
	case CARD_RESULT_BROKEN:
		result = PROGRESS_BROKEN_CARD;
		break;
	case CARD_RESULT_ENCODING:
		result = PROGRESS_WRONG_ENCODING;
		break;
	case CARD_RESULT_INSSPACE:
		result = PROGRESS_INSUFFICIENT_SPACE;
		break;
	case CARD_RESULT_NOENT:
		result = PROGRESS_NO_FREE_ENTRY;
		break;
	case CARD_RESULT_NOFILE:
		result = PROGRESS_CREATE_FILE_CONFIRM;
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
		    = (J2DTextBox*)mSaveScreen->search('sm1a' + (i << 8));
		mEventSaveOptions[i][0]->setFont((JUTFont*)gpSystemFont);

		mEventSaveOptions[i][1]
		    = (J2DTextBox*)mSaveScreen->search('sm1b' + (i << 8));
		mEventSaveOptions[i][1]->setFont((JUTFont*)gpSystemFont);

		mEventSavePointerHands[i][0] = mSaveScreen->search('sc1a' + (i << 8));
		mEventSavePointerHands[i][1] = mSaveScreen->search('sc1b' + (i << 8));
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
		    = (J2DTextBox*)mSaveScreen->search('sm3a' + (i << 8));
		mShineGetSaveOptions[i][0]->setFont((JUTFont*)gpSystemFont);
		mShineGetSaveOptions[i][1]
		    = (J2DTextBox*)mSaveScreen->search('sm3b' + (i << 8));
		mShineGetSaveOptions[i][1]->setFont((JUTFont*)gpSystemFont);

		mShineGetSavePointerHands[i][0]
		    = (J2DTextBox*)mSaveScreen->search('sc3a' + (i << 8));
		mShineGetSavePointerHands[i][1]
		    = (J2DTextBox*)mSaveScreen->search('sc3b' + (i << 8));
	}
	unk23C = mShineGetSavePointerHands[0][0]->getBounds().x1;

	mGameOverSaveWindow = new TExPane(mSaveScreen, '\0w_6');
	mGameOverSaveWindowInitialBounds
	    = mGameOverSaveWindow->getPane()->getBounds();

	for (int i = 0; i < 2; ++i) {
		mGameOverSavePointerHands[i][0]
		    = (J2DTextBox*)mSaveScreen->search('sc6a' + (i << 8));
		mGameOverSavePointerHands[i][1]
		    = (J2DTextBox*)mSaveScreen->search('sc6b' + (i << 8));

		mGameOverSaveOptions[i]
		    = (J2DTextBox*)mSaveScreen->search('sm7b' + (i << 8));
		((J2DTextBox*)mSaveScreen->search('sm7a' + (i << 8)))
		    ->setFont((JUTFont*)gpSystemFont);
		mGameOverSaveOptions[i]->setFont((JUTFont*)gpSystemFont);
	}

	unk26C = mGameOverSavePointerHands[0][0]->getBounds().x1;

#if defined(VERSION_GMSP01_NX)
	J2DPane* mask  = mSaveScreen->search('mask');
	JUTRect bounds = mask->getBounds();
	mask->add(-110, 0);
	mask->resize(bounds.getWidth() + 220, bounds.getHeight());
	mask->hide();
#else
	mSaveScreen->search('mask')->hide();
#endif
	unk2E4 = JKRGetResource(VERSION_SELECT(
	    "/common/2d/savemessage.bmg", GMSP01("/cmn2d/savemessage.bmg"),
	    GMSP01_NX("/cmn2d/savemessage.bmg")));
	if (!unk18)
		unk2D8 = static_cast<TPauseMenu2*>(
		    JDrama::TNameRefGen::search("ポーズメニュー"));
#if !defined(VERSION_GMSJ01)
	changeMessage('sm1a', 'sm1b', 0x1C);
	changeMessage('sm2a', 'sm2b', 0x1D);
	changeMessage('sm3a', 'sm3b', 0x1C);
	changeMessage('sm4a', 'sm4b', 0x1E);
	changeMessage('sm5a', 'sm5b', 0x1D);
	changeMessage('sm7a', 'sm7b', 0x1F);
	changeMessage('sm8a', 'sm8b', 0x20);
#endif
	mGamePad = game_pad;
}

#if !defined(VERSION_GMSJ01)
void TCardSave::changeMessage(u32 tag, u32 shadowTag, int message)
{
	J2DTextBox* text   = (J2DTextBox*)mSaveScreen->search(tag);
	J2DTextBox* shadow = (J2DTextBox*)mSaveScreen->search(shadowTag);
	makeBuffer(text, 0x80);
	makeBuffer(shadow, 0x80);
	setMessage(text, 0x80, message);
	setMessage(shadow, 0x80, message);
}
#endif

void TCardSave::init(int purpose)
{
	unk310         = PROGRESS_UNK0;
	unk2DF         = 0;
	mDialogPurpose = (DialogPurpose)purpose;
	unk2F8         = 0;
	unk2F9         = 1;
	if (mDialogPurpose == PURPOSE_GAME_OVER
	    || mDialogPurpose == PURPOSE_ALL_SHINES_SAVE
	    || mDialogPurpose == PURPOSE_EXPLANATION_AGAIN) {
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

#if !defined(VERSION_GMSJ01) && !defined(VERSION_GMSJ01_01)
void TCardSave::setMessageC(J2DTextBox* text_box, s32 message, u32 size)
{
#if defined(VERSION_GMSP01_NX)
	if (TFlagManager::getInstance()->getFlag(MSF_LANGUAGE)
	    != MSLANG_NX_JAPANESE) {
		JSUMemoryInputStream in(SMSGetMessageData(unk2E4, (u16)message), size);
		JSUMemoryOutputStream out(text_box->getStringPtr(), size);
		while (in.getAvailable() != 0 && out.getAvailable() != 0) {
			u8 c;
			in >> c;
			switch (c) {
			case 0x1A: {
				u8 length;
				in >> length;
				in.skip(length - 2);
				break;
			}
			case 0:
				out << c;
				return;
			default:
				out << c;
				break;
			}
		}
	} else {
		setMessage(text_box, size, (u16)message);
	}
#else
	char buffer[256];
	JSUMemoryInputStream in(SMSGetMessageData(unk2E4, (u16)message), size);
	JSUMemoryOutputStream out(text_box->getStringPtr(), size);
	while (in.getAvailable() != 0 && out.getAvailable() != 0) {
		u8 c;
		in >> c;
		JUtility::TColor color;
		switch (c) {
		case 0x1A: {
			u8 length;
			in >> length;
			in.skip(length - 2);
			continue;
		}
		case 0:
			out << c;
			return;
		case 0x0A:
			out << c;
			continue;
		}

		bool colored = true;
		switch (c) {
		case '@':
			color.set(100, 255, 100, 255);
			break;
		case '#':
			color.set(255, 160, 100, 255);
			break;
		case '%':
			color.set(255, 255, 0, 255);
			break;
		case '+':
		case '<':
		case '>':
		case 0xA5:
			color.set(220, 220, 220, 255);
			break;
		case '$':
			color.set(110, 230, 255, 255);
			break;
		default:
			colored = false;
			break;
		}

		if (colored) {
			snprintf(buffer, 255,
			         "\x1BGM[0]\x1B"
			         "CC[%02x%02x%02x]\x1BSH[3]\x1B"
			         "CD[4]",
			         color.r, color.g, color.b);
			out.write(buffer, 29);
		}
		out << c;
		if (colored) {
			snprintf(buffer, 255,
			         "\x1BGM[0]\x1B"
			         "CC\x1B"
			         "FX\x1B"
			         "FY\x1BSH\x1B"
			         "CU[4]");
			out.write(buffer, 24);
		}
	}
#endif
}
#endif

s8 TCardSave::waitForStop(TEProgress param_1)
{
	s8 result = -1;

	switch (mWindowStep) {
	case WINDOW_OPEN:
		setMessage(unkA0, 0x200, getCurMessageID());
		setMessage(unkA4, 0x200, getCurMessageID());
		setMessage(unkD8, 0x200, VERSION_SELECT(1, GMSJ01(0)));
		setMessage(unkDC, 0x200, VERSION_SELECT(1, GMSJ01(0)));

		unkA4->hide();
		unk48->getPane()->show();
		unk48->setCenteredSize(20, unk4C.getWidth(), unk4C.getHeight(), 0, 0);

		unkDC->hide();
		unkC4->setCenteredSize(20, unkC8.getWidth(), unkC8.getHeight(), 0, 0);

		if (unk310 == PROGRESS_WRONG_DEVICE || unk310 == PROGRESS_NO_CARD
		    || unk310 == PROGRESS_INCOMPATIBLE_CARD
		    || unk310 == PROGRESS_INSUFFICIENT_SPACE
		    || unk310 == PROGRESS_NO_FREE_ENTRY || unk310 == PROGRESS_IO_ERROR
#if !defined(VERSION_GMSJ01)
		    || unk310 == PROGRESS_CONTINUE_WITHOUT_SAVING
#endif
		) {
			unkC4->getPane()->show();
		} else {
			unkC4->getPane()->hide();
		}

		mWindowStep = WINDOW_OPENING;
		break;

	case WINDOW_OPENING: {
		u8 done = 1;
		done &= unk48->update();
		done &= unkC4->update();
		if (done) {
			mWindowStep = WINDOW_SHOWN;
			unkA4->show();
			unkDC->show();
		}
		break;
	}

	case WINDOW_SHOWN:
		if (mGamePad->checkFrameMeaning(TMarioGamePad::MEANING_MENU_A)) {
			gpMSound->startSoundSystemSE(0x481CU, 0, nullptr, 0);

			unk48->setCenteredSize(20, 0, 0, unk4C.getWidth(),
			                       unk4C.getHeight());
			unkC4->setCenteredSize(20, 0, 0, unkC8.getWidth(),
			                       unkC8.getHeight());
			unkA4->hide();
			unkDC->hide();
			mWindowStep = WINDOW_CLOSING;
		}
		break;

	case WINDOW_CLOSING: {
		u8 done = 1;
		done &= unk48->update();
		done &= unkC4->update();
		if (done) {
			unk48->getPane()->hide();
			unkC4->getPane()->hide();
			mWindowStep = WINDOW_DONE;
		}
		break;
	}

	case WINDOW_DONE:
		unk310 = param_1;
		result = 0;
		break;
	}

	return result;
}

void TCardSave::endWaitForChoice()
{
	if (mWindowStep != WINDOW_SHOWN)
		return;

	mShineCountOver100->hide();
	mShineCountUnder100->hide();
	unk128->hide();
	unkF8->hide();
	mConfirmWindowTextShadow->hide();
	mConfirmWindowMarioText->hide();
	mConfirmWindowYes->getPane()->hide();
	mConfirmWindowNo->getPane()->hide();
	mWindowStep = WINDOW_CLOSING;
	mConfirmWindow->setCenteredSize(20, 0, 0,
	                                mConfirmWindowInitialBounds.getWidth(),
	                                mConfirmWindowInitialBounds.getHeight());
}

s8 TCardSave::waitForChoice(TEProgress param_1, TEProgress param_2, s8 param_3)
{
	s8 result = -1;

	switch (mWindowStep) {
	case WINDOW_OPEN:
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
		mWindowStep = WINDOW_OPENING;
		break;

	case WINDOW_OPENING:
		if (mConfirmWindow->update()) {
			unkF8->show();
			mConfirmWindowYes->getPane()->show();
			mConfirmWindowNo->getPane()->show();

			unk2E9 = param_3;
			if (param_3 == 0) {
				((J2DPicture*)mConfirmWindowYes->getPane())
				    ->setWhite(JUtility::TColor(0, 0xFF, 0, 0xFF));
				((J2DPicture*)mConfirmWindowNo->getPane())
				    ->setWhite(JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF));
			} else {
				((J2DPicture*)mConfirmWindowYes->getPane())
				    ->setWhite(JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF));
				((J2DPicture*)mConfirmWindowNo->getPane())
				    ->setWhite(JUtility::TColor(0, 0xFF, 0, 0xFF));
			}

			mConfirmWindowYes->updateCenteredSize(
			    1, mConfirmWindowYesInitialBounds.getWidth(),
			    mConfirmWindowYesInitialBounds.getHeight());
			mConfirmWindowNo->updateCenteredSize(
			    1, mConfirmWindowNoInitialBounds.getWidth(),
			    mConfirmWindowNoInitialBounds.getHeight());
			mConfirmWindowYes->update();
			mConfirmWindowNo->update();

			unk2FC      = 0;
			mWindowStep = WINDOW_SHOWN;
		}
		break;

	case WINDOW_SHOWN: {
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
			mWindowStep = WINDOW_CLOSING;
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
			mWindowStep = WINDOW_CLOSING;
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
				JUTRect bounds = selectedPane->getPane()->getGlbBounds();
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
		((J2DPicture*)selectedPane->getPane())
		    ->setWhite((thing << 24) + 0xFF00FF);
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
				((J2DPicture*)mConfirmWindowYes->getPane())
				    ->setWhite(JUtility::TColor(0, 0xFF, 0, 0xFF));
				((J2DPicture*)mConfirmWindowNo->getPane())
				    ->setWhite(JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF));
			} else {
				((J2DPicture*)mConfirmWindowYes->getPane())
				    ->setWhite(JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF));
				((J2DPicture*)mConfirmWindowNo->getPane())
				    ->setWhite(JUtility::TColor(0, 0xFF, 0, 0xFF));
			}
		}
		break;
	}

	case WINDOW_CLOSING:
		if (mConfirmWindow->update()) {
			mConfirmWindow->getPane()->hide();
			mWindowStep = WINDOW_DONE;
		}
		break;

	case WINDOW_DONE:
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

	switch (mWindowStep) {
	case WINDOW_OPEN:
		if (unk2EA == -1) {
			mWindowStep = WINDOW_DONE;
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
		mWindowStep = WINDOW_OPENING;
		break;

	case WINDOW_OPENING:
		if (mConfirmWindow->update()) {
			mSaveNameABC[unk2EA]->show();
			mConfirmWindowMarioText->show();
			mConfirmWindowTextShadow->show();

			if (unk310 == PROGRESS_SAVE_DONE) {
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
#if defined(VERSION_GMSJ01)
					strncpy(unk124->getStringPtr(), "こわれています", 0x14);
					strncpy(unk128->getStringPtr(), "こわれています", 0x14);
#else
					setMessage(unk124, 0x14, 0);
					setMessage(unk128, 0x14, 0);
#endif
					unk128->show();
					mShineCountOver100->hide();
					mShineCountUnder100->hide();
				} else if (getBookmarkInfo().unk18 == 0) {
#if defined(VERSION_GMSJ01)
					strncpy(unk124->getStringPtr(), "NEW", 0x14);
					strncpy(unk128->getStringPtr(), "NEW", 0x14);
#else
					setMessage(unk124, 0x14, 0x19);
					setMessage(unk128, 0x14, 0x19);
#endif
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
				((J2DPicture*)mConfirmWindowYes->getPane())
				    ->setWhite(JUtility::TColor(0, 0xFF, 0, 0xFF));
				((J2DPicture*)mConfirmWindowNo->getPane())
				    ->setWhite(JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF));
			} else {
				((J2DPicture*)mConfirmWindowYes->getPane())
				    ->setWhite(JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF));
				((J2DPicture*)mConfirmWindowNo->getPane())
				    ->setWhite(JUtility::TColor(0, 0xFF, 0, 0xFF));
			}

			mWindowStep = WINDOW_SHOWN;
		}
		break;

	case WINDOW_SHOWN: {
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
			mWindowStep = WINDOW_CLOSING;
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
				JUTRect bounds = selectedPane->getPane()->getGlbBounds();
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
		((J2DPicture*)selectedPane->getPane())
		    ->setWhite((thing << 24) + 0xFF00FF);
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
				((J2DPicture*)mConfirmWindowYes->getPane())
				    ->setWhite(JUtility::TColor(0, 0xFF, 0, 0xFF));
				((J2DPicture*)mConfirmWindowNo->getPane())
				    ->setWhite(JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF));
			} else {
				((J2DPicture*)mConfirmWindowYes->getPane())
				    ->setWhite(JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF));
				((J2DPicture*)mConfirmWindowNo->getPane())
				    ->setWhite(JUtility::TColor(0, 0xFF, 0, 0xFF));
			}
		}
		break;
	}

	case WINDOW_CLOSING:
		if (mConfirmWindow->update()) {
			mConfirmWindow->getPane()->hide();
			mWindowStep = WINDOW_DONE;
		}
		break;

	case WINDOW_DONE:
		unk310 = unk2E9 == 0 ? param_1 : param_2;
		result = unk2E9;
		break;
	}

	return result;
}

void TCardSave::endDrawMessage()
{
	if (mWindowStep != WINDOW_SHOWN)
		return;

	mShineCountOver100->hide();
	mShineCountUnder100->hide();
	unk128->hide();
	unkF8->hide();
	mConfirmWindowTextShadow->hide();
	mConfirmWindowMarioText->hide();
	mConfirmWindowYes->getPane()->hide();
	mConfirmWindowNo->getPane()->hide();
	mWindowStep = WINDOW_CLOSING;
	mConfirmWindow->setCenteredSize(20, 0, 0,
	                                mConfirmWindowInitialBounds.getWidth(),
	                                mConfirmWindowInitialBounds.getHeight());
}

s8 TCardSave::drawMessage(TEProgress param_1)
{
	s8 result = -1;
	switch (mWindowStep) {
	case WINDOW_OPEN:
		setMessage(unk174, 0x200, getCurMessageID());
		setMessage(unk178, 0x200, getCurMessageID());
		unk178->hide();
		unk160->getPane()->show();
		unk160->setCenteredSize(20, unk164.getWidth(), unk164.getHeight(), 0,
		                        0);
		mWindowStep = WINDOW_OPENING;
		break;

	case WINDOW_OPENING:
		if (unk160->update()) {
			unk178->show();
			mWindowStep = WINDOW_SHOWN;
		}
		break;

	case WINDOW_CLOSING:
		if (unk160->update()) {
			unk160->getPane()->hide();
			mWindowStep = WINDOW_DONE;
		}
		break;

	case WINDOW_DONE:
		unk310 = param_1;
		result = 0;
		break;
	}
	return result;
}

s8 TCardSave::drawMessageBM(TEProgress param_1)
{

	s8 result = -1;

	switch (mWindowStep) {
	case WINDOW_OPEN:
		if (unk2EA == -1) {
			mWindowStep = WINDOW_DONE;
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
		mWindowStep = WINDOW_OPENING;
		break;

	case WINDOW_OPENING:
		if (mConfirmWindow->update()) {
			mConfirmWindowTextShadow->show();
			mSaveNameABC[unk2EA]->show();
			mConfirmWindowMarioText->show();

			if (mDialogPurpose == PURPOSE_SAVE
			    || mDialogPurpose == PURPOSE_SHINE_GET
			    || mDialogPurpose == PURPOSE_ALL_SHINES_SAVE) {
				if (getBookmarkInfo().unk0 == 1) {
#if defined(VERSION_GMSJ01)
					strncpy(unk124->getStringPtr(), "こわれています", 0x14);
					strncpy(unk128->getStringPtr(), "こわれています", 0x14);
#else
					setMessage(unk124, 0x14, 0);
					setMessage(unk128, 0x14, 0);
#endif
					unk128->show();
					mShineCountOver100->hide();
					mShineCountUnder100->hide();
				} else if (getBookmarkInfo().unk18 == 0) {
#if defined(VERSION_GMSJ01)
					strncpy(unk124->getStringPtr(), "NEW", 0x14);
					strncpy(unk128->getStringPtr(), "NEW", 0x14);
#else
					setMessage(unk124, 0x14, 0x19);
					setMessage(unk128, 0x14, 0x19);
#endif
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

			mWindowStep = WINDOW_SHOWN;
		}
		break;

	case WINDOW_CLOSING:
		if (mConfirmWindow->update()) {
			mConfirmWindow->getPane()->hide();
			mWindowStep = WINDOW_DONE;
		}
		break;

	case WINDOW_DONE:
		unk310 = param_1;
		result = 0;
		break;
	}

	return result;
}

s8 TCardSave::waitForAnyKey(TEProgress param_1)
{
	s8 result = -1;

	switch (mWindowStep) {
	case WINDOW_OPEN:
		setMessage(unk174, 0x200, getCurMessageID());
		setMessage(unk178, 0x200, getCurMessageID());
		unk178->hide();
		unk160->getPane()->show();
		unk160->setCenteredSize(20, unk164.getWidth(), unk164.getHeight(), 0,
		                        0);
		mWindowStep = WINDOW_OPENING;
		break;

	case WINDOW_OPENING:
		if (unk160->update()) {
			unk178->show();
			mWindowStep = WINDOW_SHOWN;
		}
		break;

	case WINDOW_SHOWN:
		if (unk2E0 > 300) {
			unk178->hide();
			unk160->setCenteredSize(20, 0, 0, unk164.getWidth(),
			                        unk164.getHeight());
			mWindowStep = WINDOW_CLOSING;
		} else {
			unk2E0++;
		}
		break;

	case WINDOW_CLOSING:
		if (unk160->update()) {
			unk160->getPane()->hide();
			mWindowStep = WINDOW_DONE;
		}
		break;

	case WINDOW_DONE:
		unk310 = param_1;
		result = 0;
		break;
	}

	return result;
}

s8 TCardSave::waitForSelectOver()
{
	s8 result = -1;

	switch (mWindowStep) {
	case WINDOW_OPEN:
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
		mWindowStep = WINDOW_OPENING;
		break;

	case WINDOW_OPENING:
		if (mGameOverSaveWindow->update()) {
			mWindowStep = WINDOW_SHOWN;
			unk2E9      = 0;
			mGameOverSaveOptions[0]->show();
			mGameOverSaveOptions[1]->show();
			mGameOverSavePointerHands[0][0]->show();
			mGameOverSavePointerHands[0][1]->show();
		}
		break;

	case WINDOW_SHOWN: {
		s8 oldSelect = unk2E9;
		u32 input    = mGamePad->mEnabledFrameMeaning;

		if (input & TMarioGamePad::MEANING_MENU_A) {
			gpMSound->startSoundSystemSE(0x481CU, 0, nullptr, 0);
			mWindowStep = WINDOW_CLOSE;
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

	case WINDOW_CLOSE:
		mGameOverSaveWindow->setCenteredSize(
		    20, 0, 0, mGameOverSaveWindowInitialBounds.getWidth(),
		    mGameOverSaveWindowInitialBounds.getHeight());
		mGameOverSaveOptions[0]->hide();
		mGameOverSaveOptions[1]->hide();
		mGameOverSavePointerHands[unk2E9][0]->hide();
		mGameOverSavePointerHands[unk2E9][1]->hide();
		mWindowStep = WINDOW_CLOSING;
		break;

	case WINDOW_CLOSING:
		if (mGameOverSaveWindow->update()) {
			mGameOverSaveWindow->getPane()->hide();
			mWindowStep = WINDOW_DONE;
		}
		break;

	case WINDOW_DONE:
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

	switch (mWindowStep) {
	case WINDOW_OPEN: {
		u32 messageID = cMessageID[unk310];
		switch (mDialogPurpose) {
		case PURPOSE_BLUE_COIN_GET:
			messageID = VERSION_SELECT(0x24, GMSJ01(0x1C), GMSJ01_01(0x1C));
			break;
		case PURPOSE_HOVER_NOZZLE_GET:
			messageID = VERSION_SELECT(0x25, GMSJ01(0x1D), GMSJ01_01(0x1D));
			break;
		case PURPOSE_ROCKET_NOZZLE_GET:
			messageID = VERSION_SELECT(0x26, GMSJ01(0x1E), GMSJ01_01(0x1E));
			break;
		case PURPOSE_TURBO_NOZZLE_GET:
			messageID = VERSION_SELECT(0x27, GMSJ01(0x1F), GMSJ01_01(0x1F));
			break;
		case PURPOSE_YOSHI_UNLOCKED:
			messageID = VERSION_SELECT(0x28, GMSJ01(0x20), GMSJ01_01(0x20));
			break;
		}

#if defined(VERSION_GMSJ01) || defined(VERSION_GMSJ01_01)
		setMessage(mEventSaveWindowText, 0x200, (u16)messageID);
#else
		setMessageC(mEventSaveWindowText, messageID, 0x200);
#endif
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

		mWindowStep = WINDOW_OPENING;
		break;
	}

	case WINDOW_OPENING:
		if (mEventSaveWindow->update()) {
			mEventSaveWindowTextShadow->show();
			mWindowStep = WINDOW_SHOWN;
			unk2E9      = 0;

			mEventSaveOptions[0][1]->show();
			mEventSaveOptions[1][1]->show();
			mEventSavePointerHands[0][0]->show();
			mEventSavePointerHands[0][1]->show();

			unk2F8 = 0;
			unk2F9 = 1;
		}
		break;

	case WINDOW_SHOWN: {
		s8 oldSelect = unk2E9;
		u32 input    = mGamePad->mEnabledFrameMeaning;

		if (input & TMarioGamePad::MEANING_MENU_A) {
			gpMSound->startSoundSystemSE(0x481CU, 0, nullptr, 0);
			mWindowStep = WINDOW_CLOSE;
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

	case WINDOW_CLOSE:
		mEventSaveWindowTextShadow->hide();
		mEventSaveWindow->setCenteredSize(
		    20, 0, 0, mEventSaveWindowInitialBounds.getWidth(),
		    mEventSaveWindowInitialBounds.getHeight());

		mEventSaveOptions[0][1]->hide();
		mEventSaveOptions[1][1]->hide();
		mEventSavePointerHands[unk2E9][0]->hide();
		mEventSavePointerHands[unk2E9][1]->hide();

		mWindowStep = WINDOW_CLOSING;
		break;

	case WINDOW_CLOSING:
		if (mEventSaveWindow->update()) {
			mEventSaveWindow->getPane()->hide();
			mWindowStep = WINDOW_DONE;
		}
		break;

	case WINDOW_DONE:
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

	switch (mWindowStep) {
	case WINDOW_OPEN:
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

		mWindowStep = WINDOW_OPENING;
		break;

	case WINDOW_OPENING:
		if (mShineGetSaveWindow->update()) {
			mWindowStep = WINDOW_SHOWN;
			unk2E9      = 0;

			mShineGetSaveOptions[0][1]->show();
			mShineGetSaveOptions[1][1]->show();
			mShineGetSaveOptions[2][1]->show();
			mShineGetSavePointerHands[0][0]->show();
			mShineGetSavePointerHands[0][1]->show();
			mShineGetSaveWindowTextShadow->show();

			mShineGetSaveNameABC[unk2EA]->show();
			mShineGetSaveMarioText->show();

			if (getBookmarkInfo().unk0 == 1) {
#if defined(VERSION_GMSJ01)
				strncpy(unk208->getStringPtr(), "こわれています", 0x14);
				strncpy(unk20C->getStringPtr(), "こわれています", 0x14);
#else
				setMessage(unk208, 0x14, 0);
				setMessage(unk20C, 0x14, 0);
#endif
				unk20C->show();
				unk210->hide();
				unk214->hide();
			} else if (getBookmarkInfo().unk18 == 0) {
#if defined(VERSION_GMSJ01)
				strncpy(unk208->getStringPtr(), "NEW", 0x14);
				strncpy(unk20C->getStringPtr(), "NEW", 0x14);
#else
				setMessage(unk208, 0x14, 0x19);
				setMessage(unk20C, 0x14, 0x19);
#endif
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

	case WINDOW_SHOWN: {
		s8 oldSelect = unk2E9;
		u32 input    = mGamePad->mEnabledFrameMeaning;

		if (input & TMarioGamePad::MEANING_MENU_A) {
			gpMSound->startSoundSystemSE(0x481CU, 0, nullptr, 0);
			mWindowStep = WINDOW_CLOSE;
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

	case WINDOW_CLOSE:
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

		mWindowStep = WINDOW_CLOSING;
		break;

	case WINDOW_CLOSING:
		if (mShineGetSaveWindow->update()) {
			mShineGetSaveWindow->getPane()->hide();
			mWindowStep = WINDOW_DONE;
		}
		break;

	case WINDOW_DONE:
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

	switch (mWindowStep) {
	case WINDOW_OPEN:
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
		mWindowStep = WINDOW_OPENING;
		break;

	case WINDOW_OPENING:
		if (mConfirmWindow->update()) {
			mSaveNameABC[unk2EA]->show();
			mConfirmWindowMarioText->show();
			mConfirmWindowTextShadow->show();

			if (getBookmarkInfo().unk0 == 1) {
#if defined(VERSION_GMSJ01)
				strncpy(unk124->getStringPtr(), "こわれています", 0x14);
				strncpy(unk128->getStringPtr(), "こわれています", 0x14);
#else
				setMessage(unk124, 0x14, 0);
				setMessage(unk128, 0x14, 0);
#endif
				unk128->show();
				mShineCountOver100->hide();
				mShineCountUnder100->hide();
			} else if (getBookmarkInfo().unk18 == 0) {
#if defined(VERSION_GMSJ01)
				strncpy(unk124->getStringPtr(), "NEW", 0x14);
				strncpy(unk128->getStringPtr(), "NEW", 0x14);
#else
				setMessage(unk124, 0x14, 0x19);
				setMessage(unk128, 0x14, 0x19);
#endif
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

			mWindowStep = WINDOW_SHOWN;
		}
		break;

	case WINDOW_SHOWN:
		if (unk2E0 > 300) {
			endWaitForChoice();
			mWindowStep = WINDOW_CLOSING;
		} else {
			unk2E0++;
		}
		break;

	case WINDOW_CLOSING:
		if (mConfirmWindow->update()) {
			mConfirmWindow->getPane()->hide();
			mWindowStep = WINDOW_DONE;
		}
		break;

	case WINDOW_DONE:
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

	TEProgress prevState      = unk310;
	WindowStep prevWindowStep = mWindowStep;

	mGamePad->onFlag(TMarioGamePad::PAD_FLAG_MENU_INPUT);

	switch (unk310) {
	case PROGRESS_UNK0:
		unk2EA = SMSGetApplication()->mSaveFile;

		if (mDialogPurpose == PURPOSE_GAME_OVER) {
			unk310 = PROGRESS_UNK2E;
			break;
		}
		if (mDialogPurpose == PURPOSE_EXPLANATION_AGAIN) {
			unk310 = PROGRESS_EXPLANATION_AGAIN_CONFIRM;
			break;
		}

		unk310 = PROGRESS_UNK2;
		gpCardManager->getBookmarkInfos(&unk278[0]);
		break;

	case PROGRESS_UNK1:
	case PROGRESS_UNK19:
		mGamePad->offFlag(TMarioGamePad::PAD_FLAG_MENU_INPUT);
		gpCardManager->unmount();

		if (!unk18 && mDialogPurpose == PURPOSE_SAVE)
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

			switch (mDialogPurpose) {
			case PURPOSE_SAVE:
				unk310 = PROGRESS_UNK15;
				break;
			case PURPOSE_SHINE_GET:
				unk310 = PROGRESS_UNK2B;
				break;
			case PURPOSE_ALL_SHINES_SAVE:
				unk310 = PROGRESS_ALL_SHINES_SAVE_CONFIRM;
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

	case PROGRESS_NO_CARD:
		if (gpCardManager->getLastStatus() == CARD_RESULT_NOCARD) {
			waitForStop(PROGRESS_UNK1);
			gpCardManager->probe();
		} else {
			if (unkA4->isVisible()) {
				unk48->setCenteredSize(20, 0, 0, unk4C.getWidth(),
				                       unk4C.getHeight());
				unkA4->hide();
				mWindowStep = WINDOW_CLOSING;
			}

			if (waitForStop(PROGRESS_UNK2) != -1)
				gpCardManager->getBookmarkInfos(&unk278[0]);
		}
		break;

	case PROGRESS_WRONG_DEVICE:
	case PROGRESS_INCOMPATIBLE_CARD:
	case PROGRESS_INSUFFICIENT_SPACE:
	case PROGRESS_NO_FREE_ENTRY:
	case PROGRESS_IO_ERROR:
#if !defined(VERSION_GMSJ01)
	case PROGRESS_CONTINUE_WITHOUT_SAVING:
#endif
		if (gpCardManager->getLastStatus() != CARD_RESULT_NOCARD) {
			waitForStop(PROGRESS_UNK1);
			gpCardManager->probe();
		} else {
			if (unkA4->isVisible()) {
				unk48->setCenteredSize(20, 0, 0, unk4C.getWidth(),
				                       unk4C.getHeight());
				unkA4->hide();
				mWindowStep = WINDOW_CLOSING;
			}
			waitForStop(PROGRESS_NO_CARD);
		}
		break;

	case PROGRESS_BROKEN_CARD:
	case PROGRESS_WRONG_ENCODING:
		if (gpCardManager->getLastStatus() != CARD_RESULT_NOCARD) {
			waitForChoice(PROGRESS_FORMAT_CONFIRM, CARD_SAVE_FORMAT_CANCEL, 1);
			gpCardManager->probe();
		} else {
			if (unkF8->isVisible())
				endWaitForChoice();

			waitForChoice(PROGRESS_NO_CARD, PROGRESS_NO_CARD, 1);
		}
		break;

	case PROGRESS_FORMAT_CONFIRM: {
		s32 status = gpCardManager->getLastStatus();
		if (status == CARD_RESULT_BUSY)
			break;

		if (status == CARD_RESULT_READY) {
			s8 r = waitForChoice(PROGRESS_FORMATTING, CARD_SAVE_FORMAT_CANCEL,
			                     1);
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

	case PROGRESS_FORMATTING: {
		s32 status = gpCardManager->getLastStatus();
		if (status != CARD_RESULT_BUSY) {
			if (status == CARD_RESULT_READY) {
				drawMessage(PROGRESS_FORMAT_DONE);
				if (unk2E0 > 0x12C) {
					if (unk178->isVisible()) {
						unk178->hide();
						unk160->setCenteredSize(20, 0, 0, unk164.getWidth(),
						                        unk164.getHeight());
						mWindowStep = WINDOW_CLOSING;
					}
				} else {
					unk2E0++;
				}
			} else {
				if (unk178->isVisible()) {
					unk178->hide();
					unk160->setCenteredSize(20, 0, 0, unk164.getWidth(),
					                        unk164.getHeight());
					mWindowStep = WINDOW_CLOSING;
				}
				drawMessage(PROGRESS_FORMAT_FAILED);
			}
		} else {
			drawMessage(PROGRESS_UNK0);
		}
		break;
	}

	case PROGRESS_FORMAT_DONE:
		if (gpCardManager->getLastStatus() == CARD_RESULT_READY) {
			s32 r = waitForAnyKey(PROGRESS_UNK2);
			gpCardManager->probe();
			if (r != -1)
				gpCardManager->getBookmarkInfos(&unk278[0]);
		} else {
#if defined(VERSION_GMSJ01)
			if (unk178->isVisible()) {
#else
			if (unk310 == PROGRESS_UNK2 && unk178->isVisible()) {
#endif
				unk160->setCenteredSize(20, 0, 0, unk164.getWidth(),
				                        unk164.getHeight());
				unk178->hide();
			}
			waitForAnyKey(PROGRESS_NO_CARD);
		}
		break;

	case PROGRESS_FORMAT_FAILED:
		if (waitForAnyKey(PROGRESS_UNK2) != -1)
#if defined(VERSION_GMSJ01)
			gpCardManager->getBookmarkInfos(&unk278[0]);
#else
			unk310 = changeMode(gpCardManager->getLastStatus());
#endif
		break;

	case PROGRESS_CREATE_FILE_CONFIRM:
		if (mWindowStep == WINDOW_OPEN)
			gpCardManager->probe();

		if (gpCardManager->getLastStatus() == CARD_RESULT_READY) {
			s32 r = waitForChoice(PROGRESS_CREATING_FILE, PROGRESS_NO_SAVE_FILE,
			                      0);
			gpCardManager->probe();
			if (r == 0)
				gpCardManager->createFile();
		} else {
			endWaitForChoice();
			waitForChoice(PROGRESS_NO_CARD, PROGRESS_NO_CARD, 1);
		}
		break;

	case PROGRESS_CREATING_FILE: {
		s32 status = gpCardManager->getLastStatus();
		if (status != CARD_RESULT_BUSY) {
			if (status == CARD_RESULT_READY) {
				drawMessage(PROGRESS_FILE_CREATED);
				if (unk2E0 > 0x12C) {
					if (unk178->isVisible()) {
						unk178->hide();
						unk160->setCenteredSize(20, 0, 0, unk164.getWidth(),
						                        unk164.getHeight());
						mWindowStep = WINDOW_CLOSING;
					}
				} else {
					unk2E0++;
				}
			} else {
#if defined(VERSION_GMSJ01)
				if (unk178->isVisible()) {
					unk178->hide();
					unk160->setCenteredSize(20, 0, 0, unk164.getWidth(),
					                        unk164.getHeight());
					mWindowStep = WINDOW_CLOSING;
				}
				drawMessage(PROGRESS_CREATE_FILE_FAILED);
#else
				drawMessage(PROGRESS_CREATE_FILE_FAILED);
				if (unk2E0 > 0x12C) {
					if (unk178->isVisible()) {
						unk178->hide();
						unk160->setCenteredSize(20, 0, 0, unk164.getWidth(),
						                        unk164.getHeight());
						mWindowStep = WINDOW_CLOSING;
					}
				} else {
					unk2E0++;
				}
#endif
			}
		} else {
			drawMessage(PROGRESS_UNK0);
		}
		break;
	}

	case PROGRESS_FILE_CREATED:
		if (waitForAnyKey(PROGRESS_UNK2) != -1)
			gpCardManager->getBookmarkInfos(&unk278[0]);
		break;

	case PROGRESS_CREATE_FILE_FAILED:
		if (waitForAnyKey(PROGRESS_UNK2) != -1)
#if defined(VERSION_GMSJ01)
			gpCardManager->getBookmarkInfos(&unk278[0]);
#else
			unk310 = changeMode(gpCardManager->getLastStatus());
#endif
		break;

	case PROGRESS_NO_SAVE_FILE:
		waitForStop(PROGRESS_UNK1);
		break;

	case PROGRESS_SELECT_FILE: {
		unk2E8 = unk2EA;

		TCardBookmarkInfo& bm = getBookmarkInfo();
		if (bm.unk0 == 1) {
			unk310 = PROGRESS_UNK14;
		} else if (bm.unk18 == 0) {
			unk310 = PROGRESS_SAVING;
			saveBookmark(unk2EA);
		} else if (bm.unk18 == 0
		           || bm.unk8
		                  == TFlagManager::getInstance()->getLastSaveTime()) {
			unk310 = PROGRESS_SAVING;
		} else {
			unk310 = PROGRESS_UNK2C;
		}
		break;
	}

	case PROGRESS_UNK14:
	case PROGRESS_UNK1E: {
		s32 status = gpCardManager->getLastStatus();
		if (status == CARD_RESULT_READY) {
			s32 r = waitForChoiceBM(PROGRESS_SAVING, PROGRESS_UNK2, 1);
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
			s32 r = waitForChoiceBM(PROGRESS_SAVING, PROGRESS_UNK1,
			                        VERSION_SELECT(0, GMSJ01(1)));
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

			waitForChoiceBM(PROGRESS_NO_CARD, PROGRESS_NO_CARD, 1);
		}
		break;
	}

	case PROGRESS_SAVING: {
		s32 status = gpCardManager->getLastStatus();

		if (status != CARD_RESULT_BUSY) {
			if (status == CARD_RESULT_READY) {
				if (unk2DC != 0) {
					unk2DC = 0;
					if (unk2DD == 0)
						TFlagManager::getInstance()->saveSuccess();
				}

				s8 r = drawMessageBM(PROGRESS_SAVE_DONE);
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

						if (mDialogPurpose == PURPOSE_SAVE
						    || mDialogPurpose == PURPOSE_SHINE_GET
						    || mDialogPurpose == PURPOSE_ALL_SHINES_SAVE) {
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
				drawMessageBM(PROGRESS_SAVE_FAILED);

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

	case PROGRESS_SAVE_DONE:
		if (mDialogPurpose != PURPOSE_SAVE || unk18) {
			unk310 = PROGRESS_UNK1;
		} else {
			waitForChoiceBM(PROGRESS_UNK1, PROGRESS_UNK19, 0);
		}
		break;

	case PROGRESS_SAVE_FAILED:
		if (waitForAnyKeyBM(PROGRESS_UNK2) != -1)
#if defined(VERSION_GMSJ01)
			gpCardManager->getBookmarkInfos(&unk278[0]);
#else
			unk310 = changeMode(gpCardManager->getLastStatus());
#endif
		break;

	case PROGRESS_UNK2A:
		if (gpCardManager->getLastStatus() == CARD_RESULT_READY) {
			s32 r = waitForSelect2(PROGRESS_SAVING, PROGRESS_UNK1);
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
						unk310 = PROGRESS_SAVING;
					} else {
						unk310 = PROGRESS_UNK2C;
					}
				}
			}
		} else {
			if (mWindowStep < WINDOW_CLOSING)
				mWindowStep = WINDOW_CLOSE;
			waitForSelect2(PROGRESS_NO_CARD, PROGRESS_NO_CARD);
		}
		break;

	case PROGRESS_UNK2B:
		if (gpCardManager->getLastStatus() == CARD_RESULT_READY) {
			s32 r = waitForSelect3(PROGRESS_SAVING, PROGRESS_SAVING,
			                       PROGRESS_UNK1);
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
							unk310 = PROGRESS_SAVING;
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
			if (mWindowStep < WINDOW_CLOSING)
				mWindowStep = WINDOW_CLOSE;
			waitForSelect3(PROGRESS_NO_CARD, PROGRESS_NO_CARD,
			               PROGRESS_NO_CARD);
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
				mWindowStep = WINDOW_CLOSING;
			}
			waitForStop(PROGRESS_NO_CARD);
		}
		break;

	case PROGRESS_UNK2E:
		waitForSelectOver();
		break;

	case PROGRESS_ALL_SHINES_SAVE_CONFIRM: {
		s32 status = gpCardManager->getLastStatus();
		if (status == CARD_RESULT_READY) {
			s32 r = waitForChoiceBM(PROGRESS_SAVING, PROGRESS_UNK1,
			                        VERSION_SELECT(0, GMSJ01(1)));
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
						unk310 = PROGRESS_SAVING;
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

	case PROGRESS_EXPLANATION_AGAIN_CONFIRM:
		waitForChoice(PROGRESS_UNK19, PROGRESS_UNK1, 1);
		break;

	default:
		break;
	}

	if (prevState != unk310) {
		unk314      = unk310;
		unk318      = 0;
		unk2E0      = 0;
		unk2F8      = 0;
		unk2F9      = 1;
		unk2DC      = 1;
		unk30C      = 0;
		unk2DD      = 0;
		mWindowStep = WINDOW_OPEN;
		unk44       = nullptr;
	}

	if (prevWindowStep != mWindowStep) {
		if (prevWindowStep == WINDOW_OPENING) {
			gpMSound->startSoundSystemSE(MSD_SE_SY_2D_IN, 0, nullptr, 0);
		}

		if (mWindowStep == WINDOW_CLOSING) {
			gpMSound->startSoundSystemSE(MSD_SE_SY_2D_OUT, 0, nullptr, 0);
		}
	}
}

u8 TCardSave::getNextState()
{
	if (unk2DF != 0) {
		if (unk310 == PROGRESS_UNK19)
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
