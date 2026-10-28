#include <GC2D/CardLoad.hpp>
#include <stdio.h>
#include <macros.h>
#include <version.h>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/J2D/J2DScreen.hpp>
#include <JSystem/J2D/J2DPicture.hpp>
#include <JSystem/J2D/J2DTextBox.hpp>
#include <JSystem/J2D/J2DWindow.hpp>
#include <JSystem/J2D/J2DPrint.hpp>
#include <JSystem/J2D/J2DOrthoGraph.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <JSystem/JParticle/JPAEmitterManager.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <System/MarioGamePad.hpp>
#include <System/Application.hpp>
#include <System/StageUtil.hpp>
#include <System/MarDirector.hpp>
#include <System/FlagManager.hpp>
#include <MSound/MSound.hpp>
#include <GC2D/Option.hpp>
#include <GC2D/ExPane.hpp>
#include <GC2D/MessageUtil.hpp>
#include <MoveBG/MapObjOption.hpp>
#include <Player/Mario.hpp>
#include <Camera/CameraOption.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/MActorAnm.hpp>

TCardLoad* gpCardLoad;

// TODO: where should this go?
extern JPAEmitterManager* gpEmitterManager4D2;

#if defined(VERSION_GMSP01) || defined(VERSION_GMSP01_NX)
#define CARD_LOAD_PRESS_START_NUM mPressStartLetterNum
#else
#define CARD_LOAD_PRESS_START_NUM ARRAY_COUNT(mPressStartTextLetters)
#endif

#define CARD_LOAD_FORMAT_CANCEL                                                \
	VERSION_SELECT(PROGRESS_CONTINUE_WITHOUT_SAVING,                           \
	               GMSJ01(PROGRESS_WRONG_DEVICE))

static void clearBookmark(u32 bm)
{
	JSUMemoryOutputStream stream;
	gpCardManager->getWriteStream(&stream);
	gpCardManager->writeBlock(bm);
}

static void loadBookmark()
{
	JSUMemoryInputStream stream;
	gpCardManager->getReadStream(&stream);
	TFlagManager::getInstance()->load(stream);
}

#if defined(VERSION_GMSP01)
const char* TCardLoad::cToOptionFileName   = "select_options.bti";
const char* TCardLoad::cBMarkNewFileName   = "select_new.bti";
const char* TCardLoad::cWindowYesFileName  = "select_yes.bti";
const char* TCardLoad::cWindowNoFileName   = "select_no.bti";
const char* TCardLoad::cScoreTotalFileName = "select_total.bti";

const char* TCardLoad::cBMarkMenuFileName[] = {
	"select_start.bti",
	"select_copy.bti",
	"select_erase.bti",
	"select_score.bti",
};

const char* TCardLoad::cScoreStageFileName[] = {
	"stage_bianco.bti", "stage_ricco.bti",      "stage_mamma.bti",
	"stage_pinna.bti",  "stage_sirena.bti",     "stage_monte.bti",
	"stage_mare.bti",   "stage_dolpicTown.bti", "stage_airport.bti",
};
#endif

u32 TCardLoad::cMessageID[] = {
#if defined(VERSION_GMSJ01)
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x1000E,    0x10012,    0x10013,
	0x10004,    0x10005,    0x10006,    0x10018,    0x1001F,    0x10017,
	0x10011,    0x1000F,    0x10001,    0x1000A,    0x10010,    0x1001D,
	0x10015,    0x1001A,    0xFFFFFFFF, 0x10008,    0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0x10007,    0xFFFFFFFF, 0x1001B,    0x10003,
	0x10002,    0x1000B,    0x1001E,    0x10016,    0x10000,    0xFFFFFFFF,
	0x10009,    0x10009,    0x10014,    0x1001C,    0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x1000D,    0xFFFFFFFF, 0x10019,
	0x0,        0x0,        0x0,        0x0,        0x0,
#elif defined(VERSION_GMSE01)
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x10010,    0x10014,    0x10015,
	0x10005,    0x10006,    0x10007,    0x1001A,    0x10022,    0x10019,
	0x10013,    0x10011,    0x10002,    0x1000B,    0x10012,    0x10020,
	0x10017,    0x1001D,    0xFFFFFFFF, 0x10009,    0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0x10008,    0xFFFFFFFF, 0x1001E,    0x10004,
	0x10003,    0x1000C,    0x10021,    0x10018,    0x10001,    0xFFFFFFFF,
	0x1000A,    0x1000A,    0x10016,    0x1001F,    0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x1000F,    0xFFFFFFFF, 0x1001C,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x1000E,
#else
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x10010,    0x10014,    0x10015,
	0x10005,    0x10006,    0x10007,    0x1001A,    0x10023,    0x10019,
	0x10013,    0x10011,    0x10002,    0x1000B,    0x10012,    0x10021,
	0x10017,    0x1001E,    0xFFFFFFFF, 0x10009,    0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0x10008,    0xFFFFFFFF, 0x1001F,    0x10004,
	0x10003,    0x1000C,    0x10022,    0x10018,    0x10001,    0xFFFFFFFF,
	0x1000A,    0x1000A,    0x10016,    0x10020,    0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x1000F,    0xFFFFFFFF, 0x1001D,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x1000E,
#endif
};

TCardLoad::TCardLoad(const char* name)
    : JDrama::TViewObj(name)
    , mWindowStep(WINDOW_DONE)
    , mScreenState(SCREEN_INIT_TITLE)
    , unk1C(PROGRESS_UNK0)
    , mLoadScreen(nullptr)
    , mScoreScreen(nullptr)
    , unk30(0)
    , unkA0(nullptr)
    , unkA4(0)
    , unkA8(60.0f)
    , unkAC(nullptr)
    , unkB8(false)
    , unkBC(0)
    , mToSettingsArrow(nullptr)
    , mToSettingsText(nullptr)
    , unk274(0)
    , unk275(0)
    , unk284(0)
    , mChoseSaveWindow(nullptr)
    , unk524(nullptr)
    , unk538(nullptr)
    , unk53C(nullptr)
    , unk754(0)
{
	gpCardLoad = this;
}

void TCardLoad::changePattern(J2DPicture* picture, s16 period, u32 frame)
{
	if (frame % period == 0) {
		if ((frame / period) % 2 == 0) {
			picture->setBlendColorRatio(0.0f, 1.0f, 0.0f, 0.0f);
			picture->setBlendAlphaRatio(0.0f, 1.0f, 0.0f, 0.0f);
		} else {
			picture->setBlendColorRatio(1.0f, 0.0f, 0.0f, 0.0f);
			picture->setBlendAlphaRatio(1.0f, 0.0f, 0.0f, 0.0f);
		}
	}
}

void TCardLoad::load(JSUMemoryInputStream& stream)
{
	JDrama::TViewObj::load(stream);

	if (SMSGetMarDirector()->getCurrentStage() != 15)
		return;

	JKRArchive* optionVolume = (JKRArchive*)JKRFileLoader::getVolume("option");

#if defined(VERSION_GMSP01)
	mLangMessageData[MSLANG_ENGLISH]
	    = JKRGetResource("/option/loadmessage_en.bmg");
	mLangMessageData[MSLANG_GERMAN]
	    = JKRGetResource("/option/loadmessage_ge.bmg");
	mLangMessageData[MSLANG_FRENCH]
	    = JKRGetResource("/option/loadmessage_fr.bmg");
	mLangMessageData[MSLANG_SPANISH]
	    = JKRGetResource("/option/loadmessage_sp.bmg");
	mLangMessageData[MSLANG_ITALIAN]
	    = JKRGetResource("/option/loadmessage_it.bmg");

	mLoadScreen = new J2DSetScreen("load.blo", optionVolume);
	mLoadScreen->setCullBack(GX_CULL_BACK);
	mLangScoreScreens[MSLANG_ENGLISH]
	    = new J2DSetScreen("load_score_en.blo", optionVolume);
	mLangScoreScreens[MSLANG_GERMAN]
	    = new J2DSetScreen("load_score_ge.blo", optionVolume);
	mLangScoreScreens[MSLANG_FRENCH]
	    = new J2DSetScreen("load_score_fr.blo", optionVolume);
	mLangScoreScreens[MSLANG_SPANISH]
	    = new J2DSetScreen("load_score_sp.blo", optionVolume);
	mLangScoreScreens[MSLANG_ITALIAN]
	    = new J2DSetScreen("load_score_it.blo", optionVolume);
	mScoreScreen = mLangScoreScreens[MSLANG_ENGLISH];
	mScoreScreen->setCullBack(GX_CULL_BACK);
	unk754 = new TOptionControl;
	unk754->load();

	int lang = TFlagManager::getInstance()->getFlag(MSF_LANGUAGE);
	const char* titleScreenName;
	switch (lang) {
	default:
	case MSLANG_ENGLISH:
		titleScreenName = "title_1.blo";
		break;
	case MSLANG_GERMAN:
		titleScreenName = "title_1_ge.blo";
		break;
	case MSLANG_FRENCH:
		titleScreenName = "title_1_fr.blo";
		break;
	case MSLANG_SPANISH:
		titleScreenName = "title_1_sp.blo";
		break;
	case MSLANG_ITALIAN:
		titleScreenName = "title_1_it.blo";
		break;
	}
	mTitleScreen = new J2DSetScreen(titleScreenName, optionVolume);
	mTitleScreen->setCullBack(GX_CULL_BACK);
#elif defined(VERSION_GMSP01_NX)
	static const char* loadLangMessageName[] = {
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		"/option/loadmessage_jp.bmg",
		"/option/loadmessage_us.bmg",
		"/option/loadmessage_en.bmg",
		"/option/loadmessage_ge.bmg",
		"/option/loadmessage_fr.bmg",
		"/option/loadmessage_sp.bmg",
		"/option/loadmessage_it.bmg",
	};
	static const char* loadScreenName[] = {
		nullptr,       nullptr,       nullptr,       nullptr,
		nullptr,       "load_jp.blo", "load.blo",    "load.blo",
		"load_ge.blo", "load_fr.blo", "load_sp.blo", "load_it.blo",
	};
	static const char* scoreLangScreenName[] = {
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		"load_score_jp.blo",
		"load_score_us.blo",
		"load_score_en.blo",
		"load_score_ge.blo",
		"load_score_fr.blo",
		"load_score_sp.blo",
		"load_score_it.blo",
	};
	static const char* titleScrnName[] = {
		nullptr,          nullptr,          nullptr,          nullptr,
		nullptr,          "title_1_jp.blo", "title_1_us.blo", "title_1_en.blo",
		"title_1_ge.blo", "title_1_fr.blo", "title_1_sp.blo", "title_1_it.blo",
	};

	unkA0 = JKRGetResource(
	    loadLangMessageName[TFlagManager::getInstance()->getFlag(
	        MSF_LANGUAGE)]);

	mLoadScreen = new J2DSetScreen(
	    loadScreenName[TFlagManager::getInstance()->getFlag(MSF_LANGUAGE)],
	    optionVolume);
	mLoadScreen->setCullBack(GX_CULL_BACK);
	mScoreScreen = new J2DSetScreen(
	    scoreLangScreenName[TFlagManager::getInstance()->getFlag(MSF_LANGUAGE)],
	    optionVolume);
	mScoreScreen->setCullBack(GX_CULL_BACK);
	unk754 = new TOptionControl;
	unk754->load();

	mTitleScreen = new J2DSetScreen(
	    titleScrnName[TFlagManager::getInstance()->getFlag(MSF_LANGUAGE)],
	    optionVolume);
	mTitleScreen->setCullBack(GX_CULL_BACK);
#else
	unkA0 = JKRGetResource("/option/loadmessage.bmg");

	mLoadScreen = new J2DSetScreen("load.blo", optionVolume);
	mLoadScreen->setCullBack(GX_CULL_BACK);
	mScoreScreen = new J2DSetScreen("load_score.blo", optionVolume);
	mScoreScreen->setCullBack(GX_CULL_BACK);
	unk754 = new TOptionControl;
	unk754->load();

	mTitleScreen = new J2DSetScreen("title_1.blo", optionVolume);
	mTitleScreen->setCullBack(GX_CULL_BACK);
#endif

	for (int i = 0; i < 10; ++i) {
		char buffer[0x28];
		snprintf(buffer, 0x28, "/option/timg/coin_number_%d_min.bti", i);
		mDigitTextures[i]
		    = new JUTTexture((const ResTIMG*)JKRGetResource(buffer));
	}

	setupTitleScreen();
#if defined(VERSION_GMSP01)
	changeTitleTexture(lang);
#endif

	// Title screen setup

	mTitleScreenRoot = mTitleScreen->search('ROOT');

	mGameLogoPicture     = new TExPane(mTitleScreen, 'titl');
	mNintendoLogoPicture = new TExPane(mTitleScreen, 'nint');

	for (int i = 0; i < CARD_LOAD_PRESS_START_NUM; ++i) {
		int key;
		if (i < 9)
			key = 's_01' + i;
		else
			key = 's_00' + ((i + 1) / 10 << 8) + (i + 1) % 10;

		mPressStartTextLetters[i] = new TExPane(mTitleScreen, key);
		mPressStartTextLetters[i]->getPane()->setAlpha(0);
		mPressStartTextLettersInitialBounds[i]
		    = mPressStartTextLetters[i]->getPane()->getBounds();
	}

	for (int i = 0; i < CARD_LOAD_GAME_LOGO_NUM; ++i) {
		int key;
		if (i < 9)
			key = 'p_01' + i;
		else
			key = 'p_00' + ((i + 1) / 10 << 8) + (i + 1) % 10;

		mGameLogoLetters[i] = new TExPane(mTitleScreen, key);

		((J2DPicture*)mGameLogoLetters[i]->getPane())->setBlack(0x00FFFF00);

		mGameLogoLetters[i]->setPaneAlpha(20, 0xff, 0);
	}

	// Save load screen setup

	mToSettingsArrow              = mLoadScreen->search('yaji');
	mToSettingsArrowInitialBounds = mToSettingsArrow->getBounds();
	mToSettingsText               = mLoadScreen->search('\0s_1');
	mToSettingsArrow->show();
	mToSettingsText->show();
	mToSettingsArrow->setAlpha(0);

	mChoseSaveWindow              = new TExPane(mLoadScreen, '\0w_0');
	mChoseSaveWindowInitialBounds = mChoseSaveWindow->getPane()->getBounds();

	mChoseSaveText = (J2DTextBox*)mLoadScreen->search('m_0a');
	makeBuffer(mChoseSaveText, VERSION_SELECT(0x100, GMSJ01(0x80)));

	mChoseSaveTextShadow = (J2DTextBox*)mLoadScreen->search('m_0b');
	makeBuffer(mChoseSaveTextShadow, VERSION_SELECT(0x100, GMSJ01(0x80)));

	JUTTexture* pJVar5 = new JUTTexture(
	    (const ResTIMG*)JKRGetResource("/option/timg/load_sun_1.bti"));
	for (int i = 0; i < 3; ++i) {
		mSaveWindows[i] = new TExPane(mLoadScreen, '\0w_1' + i);

		unk2C0[i] = (J2DTextBox*)mLoadScreen->search('m_1a' + 2 * i);
		makeBuffer(unk2C0[i], 0x80);

		unk2CC[i] = (J2DTextBox*)mLoadScreen->search('m_1b' + 2 * i);
		makeBuffer(unk2CC[i], 0x80);

		mSaveShineIconsUnder100[i] = mLoadScreen->search('shn0' + i);
		mSaveShineCountTensUnder100[i]
		    = (J2DPicture*)mLoadScreen->search('n_0a' + (i << 8));
		mSaveShineCountOnesUnder100[i]
		    = (J2DPicture*)mLoadScreen->search('n_0b' + (i << 8));

		mSaveShineIconsOver100[i] = mLoadScreen->search('shp0' + i);
		mSaveShineCountHundredsOver100[i]
		    = (J2DPicture*)mLoadScreen->search('np0a' + (i << 8));
		mSaveShineCountTensOver100[i]
		    = (J2DPicture*)mLoadScreen->search('np0b' + (i << 8));
		mSaveShineCountOnesOver100[i]
		    = (J2DPicture*)mLoadScreen->search('np0c' + (i << 8));
		unk32C[i] = (J2DPicture*)mLoadScreen->search('cra1' + (i << 8));

		unk32C[i]->append(pJVar5, 0.0f);
		mLoadScreen->search('cra2' + (i << 8))->hide();
	}

	unk2B0 = mSaveWindows[0]->getPane()->getBounds();

	for (int i = 0; i < 3; ++i) {
		mSaveManageOptionsWindow[i] = new TExPane(mLoadScreen, '\0w_4' + i);
		mSaveManageOptionsWindowInitialBounds[i]
		    = mSaveManageOptionsWindow[i]->getPane()->getBounds();

		u32 key = 's_2a' + i;
		for (int j = 0; j < 4; ++j) {
			mSaveManageOptionsEntries[i][j]
			    = new TExPane(mLoadScreen, key + (j << 8));
			mSaveManageOptionsEntriesInitialBounds[i][j]
			    = mSaveManageOptionsEntries[i][j]->getPane()->getBounds();
		}
	}

	unk468 = new TExPane(mLoadScreen, '\0w_7');
	unk46C = unk468->getPane()->getBounds();

	unk47C = (J2DTextBox*)mLoadScreen->search('m_2a');
	makeBuffer(unk47C, 0x400);

	unk480 = (J2DTextBox*)mLoadScreen->search('m_2b');
	makeBuffer(unk480, 0x400);

	for (int i = 0; i < 2; ++i) {
		unk484[i] = new TExPane(mLoadScreen, 's_6a' + (i << 8));
		unk48C[i] = unk484[i]->getPane()->getBounds();
	}

	mConfirmationWindow = new TExPane(mLoadScreen, '\0w_8');
	mConfirmationWindowInitialBounds
	    = mConfirmationWindow->getPane()->getBounds();

	mConfirmationText = (J2DTextBox*)mLoadScreen->search('m_4a');
	makeBuffer(mConfirmationText, 0x400);

	mConfirmationTextShadow = (J2DTextBox*)mLoadScreen->search('m_4b');
	makeBuffer(mConfirmationTextShadow, 0x400);

	mConfirmationTextMario = mLoadScreen->search('\0t_0');

	for (int i = 0; i < 3; ++i)
		mConfirmationTextTargetABC[i] = mLoadScreen->search('\0t_1' + i);

	for (int i = 0; i < 2; ++i) {
		mConfirmationYesNo[i] = new TExPane(mLoadScreen, 's_6b' + (i << 8));
		mConfirmationYesNoInitialBounds[i]
		    = mConfirmationYesNo[i]->getPane()->getBounds();
#if !defined(VERSION_GMSJ01) && !defined(VERSION_GMSE01)
		mConfirmationYesNo[i]->getPane()->hide();
#endif
	}

	mConfirmationShineCountOver100  = mLoadScreen->search('f_sh');
	mConfirmationShineCountUnder100 = mLoadScreen->search('f_si');
#if !defined(VERSION_GMSJ01) && !defined(VERSION_GMSE01)
	mConfirmationShineCountOver100->hide();
	mConfirmationShineCountUnder100->hide();
#endif

	for (int i = 0; i < 3; ++i)
		mConfirmationShineCountDigitsOver100[i]
		    = (J2DPicture*)mLoadScreen->search('n_3a' + i);

	for (int i = 0; i < 2; ++i)
		mConfirmationShineCountDigitsUnder100[i]
		    = (J2DPicture*)mLoadScreen->search('n_4a' + i);

	mConfirmationNewSaveText = (J2DTextBox*)mLoadScreen->search('m_3a');
	makeBuffer(mConfirmationNewSaveText, 0x14);

	mConfirmationNewSaveTextShadow = (J2DTextBox*)mLoadScreen->search('m_3b');
	makeBuffer(mConfirmationNewSaveTextShadow, 0x14);

	unk524 = new TExPane(mLoadScreen, '\0w_9');
	unk528 = unk524->getPane()->getBounds();

	unk538 = (J2DTextBox*)mLoadScreen->search('m_5a');
	makeBuffer(unk538, 0x400);

	unk53C = (J2DTextBox*)mLoadScreen->search('m_5b');
	makeBuffer(unk53C, 0x400);
	unk53C->hide();

	for (int i = 0; i < 3; ++i)
		unk540[i] = mLoadScreen->search('s_0a' + i);

	unk54C = new TExPane(mLoadScreen, 'w_11');
	unk550 = unk54C->getPane()->getBounds();

	unk560 = (J2DTextBox*)mLoadScreen->search('m_7a');
	makeBuffer(unk560, 0x400);

	unk564 = (J2DTextBox*)mLoadScreen->search('m_7b');
	makeBuffer(unk564, 0x400);
	unk564->hide();

	mErasedWindow              = new TExPane(mLoadScreen, 'w_10');
	mErasedWindowInitialBounds = mErasedWindow->getPane()->getBounds();

	mErasedText = (J2DTextBox*)mLoadScreen->search('m_6a');
	makeBuffer(mErasedText, 0x400);

	mErasedTextShadow = (J2DTextBox*)mLoadScreen->search('m_6b');
	makeBuffer(mErasedTextShadow, 0x400);

#if defined(VERSION_GMSJ01) || defined(VERSION_GMSE01)
	// Score screen setup

	int local_90[] = { 2, 3, 4, 5, 6, 8, 7 };

	for (int i = 0; i < 7; ++i) {
		mScoreTableStageRows[i].mRoot
		    = (J2DPicture*)mScoreScreen->search('st_0' + i);
		for (int j = 0; j < 3; ++j)
			mScoreTableStageRows[i].mCoinCountDigits[j]
			    = (J2DPicture*)mScoreScreen->search('n_0a' + (i << 8) + j);

		for (int j = 0; j < 8; ++j) {
			mScoreTableStageRows[i].mShineIcons[j]
			    = mScoreScreen->search('sh0a' + (i << 8) + j);

			if (!SMS_isGetShine(local_90[i], j, false))
				mScoreTableStageRows[i].mShineIcons[j]->hide();
		}

		for (int j = 0; j < 2; ++j) {
			mScoreTableStageRows[i].unk30[j]
			    = mScoreScreen->search('sh0i' + (i << 8) + j);
			mScoreTableStageRows[i].unk30[j]->hide();
		}

		mScoreTableStageRows[i].unk38 = mScoreScreen->search('sh0k' + (i << 8));
		mScoreTableStageRows[i].unk38->hide();
	}

	for (int i = 0; i < 3; ++i) {
		mScoreTableWhichSaveABC[i] = mScoreScreen->search('\0t_1' + i);
		mScoreTableWhichSaveABC[i]->hide();
	}

	mScoreTableHeader           = mScoreScreen->search('\0t_p');
	mScoreTotal                 = mScoreScreen->search('s_tl');
	mScoreTotalShineCountDigit1 = (J2DPicture*)mScoreScreen->search('\0n_a');
	mScoreTotalShineCountDigit2 = (J2DPicture*)mScoreScreen->search('\0n_b');
	mScoreTotalShineCountDigit3 = (J2DPicture*)mScoreScreen->search('\0n_c');
#elif defined(VERSION_GMSP01)
	loadLangTexture();
	changeLanguage(TFlagManager::getInstance()->getFlag(MSF_LANGUAGE));
#else
	resetScoreScreenObjects();
#endif
}

#if !defined(VERSION_GMSJ01) && !defined(VERSION_GMSE01)
void TCardLoad::resetScoreScreenObjects()
{
	int local_90[] = { 2, 3, 4, 5, 6, 8, 7 };

	for (int i = 0; i < 7; ++i) {
		mScoreTableStageRows[i].mRoot
		    = (J2DPicture*)mScoreScreen->search('st_0' + i);
		for (int j = 0; j < 3; ++j)
			mScoreTableStageRows[i].mCoinCountDigits[j]
			    = (J2DPicture*)mScoreScreen->search('n_0a' + (i << 8) + j);

		for (int j = 0; j < 8; ++j) {
			mScoreTableStageRows[i].mShineIcons[j]
			    = mScoreScreen->search('sh0a' + (i << 8) + j);

			if (!SMS_isGetShine(local_90[i], j, false))
				mScoreTableStageRows[i].mShineIcons[j]->hide();
		}

		for (int j = 0; j < 2; ++j) {
			mScoreTableStageRows[i].unk30[j]
			    = mScoreScreen->search('sh0i' + (i << 8) + j);
			mScoreTableStageRows[i].unk30[j]->hide();
		}

		mScoreTableStageRows[i].unk38 = mScoreScreen->search('sh0k' + (i << 8));
		mScoreTableStageRows[i].unk38->hide();
	}

	for (int i = 0; i < 3; ++i) {
		mScoreTableWhichSaveABC[i] = mScoreScreen->search('\0t_1' + i);
		mScoreTableWhichSaveABC[i]->hide();
	}

	mScoreTableHeader           = mScoreScreen->search('\0t_p');
	mScoreTotal                 = mScoreScreen->search('s_tl');
	mScoreTotalShineCountDigit1 = (J2DPicture*)mScoreScreen->search('\0n_a');
	mScoreTotalShineCountDigit2 = (J2DPicture*)mScoreScreen->search('\0n_b');
	mScoreTotalShineCountDigit3 = (J2DPicture*)mScoreScreen->search('\0n_c');
}
#endif

#if defined(VERSION_GMSP01)
void TCardLoad::changeTitleTexture(int lang)
{
	const char* dirs[] = {
		"/option/timg/",    "/option/timg/ge/", "/option/timg/fr/",
		"/option/timg/sp/", "/option/timg/it/",
	};
	const char* dir = dirs[lang];
	for (int i = 0; i < mPressStartLetterNum; ++i) {
		char path[256];
		snprintf(path, 255, "%stitle_start_%02d.bti", dir, i + 1);
		JUTTexture* texture
		    = new JUTTexture((const ResTIMG*)JKRGetResource(path));
		replaceTexture('s_00' + (i + 1) % 10 + ((i + 1) / 10 << 8), texture,
		               mTitleScreen);
	}
}

void TCardLoad::loadLangTexture()
{
	loadTextureFile(mToOptionTextures, cToOptionFileName);
	for (int i = 0; i < 4; ++i)
		loadTextureFile(mBMarkMenuTextures[i], cBMarkMenuFileName[i]);
	loadTextureFile(mBMarkNewTextures, cBMarkNewFileName);
	loadTextureFile(mWindowYesTextures, cWindowYesFileName);
	loadTextureFile(mWindowNoTextures, cWindowNoFileName);
	for (int i = 0; i < 9; ++i)
		loadTextureFile(mScoreStageTextures[i], cScoreStageFileName[i]);
	loadTextureFile(mScoreTotalTextures, cScoreTotalFileName);
}

void TCardLoad::loadTextureFile(JUTTexture** textures, const char* name)
{
	static const char* langArray[] = { "", "ge/", "fr/", "sp/", "it/" };
	for (int i = 0; i < 5; ++i) {
		char path[256];
		snprintf(path, 255, "/option/timg/%s%s", langArray[i], name);
		textures[i] = new JUTTexture((const ResTIMG*)JKRGetResource(path));
	}
}

void TCardLoad::replaceTexture(u32 tag, JUTTexture* texture, J2DScreen* screen)
{
	J2DPicture* picture = (J2DPicture*)screen->search(tag);
	picture->changeTexture(texture->getTexInfo(), 0);
}

void TCardLoad::replaceTexture_fixCenter(u32 tag, JUTTexture* texture,
                                         J2DScreen* screen)
{
}

void TCardLoad::changeLanguage(int lang)
{
	unkA0        = mLangMessageData[lang];
	mScoreScreen = mLangScoreScreens[lang];
	resetScoreScreenObjects();
	replaceTexture('\0s_1', mToOptionTextures[lang], mLoadScreen);
	replaceTexture('s_6a', mWindowYesTextures[lang], mLoadScreen);
	replaceTexture('s_7a', mWindowNoTextures[lang], mLoadScreen);
	replaceTexture('s_6b', mWindowYesTextures[lang], mLoadScreen);
	replaceTexture('s_7b', mWindowNoTextures[lang], mLoadScreen);
	for (int i = 0; i < 3; ++i) {
		replaceTexture('s_0a' + i, mBMarkNewTextures[lang], mLoadScreen);
		for (int j = 0; j < 4; ++j)
			replaceTexture('s_2a' + (j << 8) + i, mBMarkMenuTextures[j][lang],
			               mLoadScreen);
	}
	for (int i = 0; i < 9; ++i)
		replaceTexture('st_0' + i, mScoreStageTextures[i][lang], mScoreScreen);
	replaceTexture('s_tl', mScoreTotalTextures[lang], mScoreScreen);
}
#endif

void TCardLoad::setupTitleScreen()
{
#if defined(VERSION_GMSP01)
	switch (TFlagManager::getInstance()->getFlag(MSF_LANGUAGE)) {
	default:
	case MSLANG_ENGLISH:
		mPressStartLetterNum = 11;
		break;
	case MSLANG_GERMAN:
		mPressStartLetterNum = 11;
		break;
	case MSLANG_FRENCH:
		mPressStartLetterNum = 15;
		break;
	case MSLANG_SPANISH:
		mPressStartLetterNum = 12;
		break;
	case MSLANG_ITALIAN:
		mPressStartLetterNum = 10;
		break;
	}

	int pressStartLetterNum = mPressStartLetterNum;
	for (int i = 0; i < pressStartLetterNum; ++i) {
		mPressStartAnimTimes[i] = 25 * i + 150;
		mPressStartAnimState[i] = 4;
	}
#elif defined(VERSION_GMSP01_NX)
	static const int euStartLength[] = { 0, 0, 0, 0, 0, 6, 6, 6, 7, 11, 6, 6 };

	int lang             = TFlagManager::getInstance()->getFlag(MSF_LANGUAGE);
	mPressStartLetterNum = euStartLength[lang];
	int pressStartLetterNum = mPressStartLetterNum;
	mGameLogoLetterNum      = lang == 5 ? 13 : 18;
	for (int i = 0; i < pressStartLetterNum; ++i) {
		mPressStartAnimTimes[i] = 25 * i + 150;
		mPressStartAnimState[i] = 4;
	}
#else
#if defined(VERSION_GMSJ01) || defined(VERSION_GMSE01)
	for (int i = 0; i < ARRAY_COUNT(mPressStartAnimTimes); ++i) {
#else
	int pressStartLetterNum = ARRAY_COUNT(mPressStartAnimTimes);
	for (int i = 0; i < pressStartLetterNum; ++i) {
#endif
		mPressStartAnimTimes[i] = 25 * i + 150;
		if (i > 4)
			mPressStartAnimTimes[i] += 20;
		mPressStartAnimState[i] = 4;
	}
#endif

	for (int i = 0; i < CARD_LOAD_GAME_LOGO_NUM; ++i) {
		mGameLogoAnimTimes[i] = 400 * i;
		mGameLogoAnimState[i] = 4;
	}
}

void TCardLoad::setupScoreScreen()
{
	int shineCount = TFlagManager::getInstance()->getFlag(MSF_SHINE_COUNT);
	if (shineCount < 100) {
		mScoreTotalShineCountDigit1->changeTexture(
		    mDigitTextures[shineCount / 10]->getTexInfo(), 0);
		mScoreTotalShineCountDigit2->changeTexture(
		    mDigitTextures[shineCount % 10]->getTexInfo(), 0);
		mScoreTotalShineCountDigit3->hide();
	} else {
		mScoreTotalShineCountDigit1->changeTexture(
		    mDigitTextures[shineCount / 100]->getTexInfo(), 0);
		mScoreTotalShineCountDigit2->changeTexture(
		    mDigitTextures[(shineCount - 100) / 10]->getTexInfo(), 0);
		mScoreTotalShineCountDigit3->changeTexture(
		    mDigitTextures[(shineCount - 100) % 10]->getTexInfo(), 0);
		mScoreTotalShineCountDigit3->show();
	}

	u16 iVar8        = 0;
	int stageOrder[] = { 2, 3, 4, 5, 6, 7, 8 };

	for (int i = 0; i < 7; ++i) {
		u8 shineCount = 0;

		if (TFlagManager::getInstance()->getBool(MSF_VISITED_BASE
		                                         + stageOrder[i]))
			mScoreTableStageRows[i].mRoot->show();
		else
			mScoreTableStageRows[i].mRoot->hide();

		for (int j = 0; j < 8; ++j)
			if (SMS_isGetShine(stageOrder[i], j, false))
				++shineCount;

		iVar8 += shineCount;

		for (int j = 0; j < 8; ++j) {
			J2DPane* pane = mScoreTableStageRows[i].mShineIcons[j];
			if (!pane)
				continue;

			if (j < shineCount)
				pane->show();
			else
				pane->hide();
		}

		for (int j = 0; j < 2; ++j) {
			if (SMS_isGetShine(stageOrder[i], j + 1, true)) {
				iVar8 += 1;
				mScoreTableStageRows[i].unk30[j]->show();
			} else {
				mScoreTableStageRows[i].unk30[j]->hide();
			}
		}

		if (SMS_isGetShine(stageOrder[i], 0, true)) {
			iVar8 += 1;
			mScoreTableStageRows[i].unk38->show();
		} else {
			mScoreTableStageRows[i].unk38->hide();
		}

		int coinRecord = TFlagManager::getInstance()->getFlag(
		    MSF_COIN_RECORD_BASE + stageOrder[i]);
		if (coinRecord > 999)
			coinRecord = 999;
		if (coinRecord < 100) {
			mScoreTableStageRows[i].mCoinCountDigits[2]->hide();
			mScoreTableStageRows[i].mCoinCountDigits[0]->changeTexture(
			    mDigitTextures[coinRecord / 10]->getTexInfo(), 0);
			mScoreTableStageRows[i].mCoinCountDigits[1]->changeTexture(
			    mDigitTextures[coinRecord % 10]->getTexInfo(), 0);
		} else {
			mScoreTableStageRows[i].mCoinCountDigits[0]->changeTexture(
			    mDigitTextures[coinRecord / 100]->getTexInfo(), 0);
			u8 iVar4 = coinRecord - (coinRecord / 100) * 100;
			mScoreTableStageRows[i].mCoinCountDigits[1]->changeTexture(
			    mDigitTextures[iVar4 / 10]->getTexInfo(), 0);
			mScoreTableStageRows[i].mCoinCountDigits[2]->changeTexture(
			    mDigitTextures[iVar4 % 10]->getTexInfo(), 0);
			mScoreTableStageRows[i].mCoinCountDigits[2]->show();
		}

		mScoreTableStageRows[i].mRoot->setAlpha(0);
	}

	u16 asdf = 0;
	if (TFlagManager::getInstance()->getBool(MSF_SHINE_AIRSTRIP))
		asdf += 1;
	if (TFlagManager::getInstance()->getBool(MSF_SHINE_AIRSTRIP_REDS))
		asdf += 1;
	iVar8 += asdf;
	if (asdf > 9)
		asdf = 9;
	const ResTIMG* pRVar4 = mDigitTextures[asdf % 10]->getTexInfo();
	((J2DPicture*)mScoreScreen->search('\0n_f'))->changeTexture(pRVar4, 0);

	if (TFlagManager::getInstance()->getBool(MSF_PLAZA_VISITED))
		mScoreScreen->search('st_7')->show();
	else
		mScoreScreen->search('st_7')->hide();
	mScoreScreen->search('st_7')->setAlpha(255);
	if (SMS_isGetShine(1, 0, true)) {
		mScoreScreen->search('sh7a')->show();
		iVar8 += 1;
	} else {
		mScoreScreen->search('sh7a')->hide();
	}

	int delfinoCoinRecord
	    = TFlagManager::getInstance()->getFlag(MSF_COIN_RECORD_DELFINO);
	if (delfinoCoinRecord > 999)
		delfinoCoinRecord = 999;

	if (delfinoCoinRecord < 100) {
		mScoreScreen->search('n_7c')->hide();
		((J2DPicture*)mScoreScreen->search('n_7a'))
		    ->changeTexture(
		        mDigitTextures[delfinoCoinRecord / 10]->getTexInfo(), 0);
		((J2DPicture*)mScoreScreen->search('n_7b'))
		    ->changeTexture(
		        mDigitTextures[delfinoCoinRecord % 10]->getTexInfo(), 0);
	} else {
		mScoreScreen->search('n_7c')->show();
		((J2DPicture*)mScoreScreen->search('n_7a'))
		    ->changeTexture(
		        mDigitTextures[delfinoCoinRecord / 100]->getTexInfo(), 0);
		delfinoCoinRecord -= delfinoCoinRecord / 100 * 100;
		((J2DPicture*)mScoreScreen->search('n_7b'))
		    ->changeTexture(
		        mDigitTextures[delfinoCoinRecord / 10]->getTexInfo(), 0);
		((J2DPicture*)mScoreScreen->search('n_7c'))
		    ->changeTexture(
		        mDigitTextures[delfinoCoinRecord % 10]->getTexInfo(), 0);
	}

	int totalShineCount
	    = TFlagManager::getInstance()->getFlag(MSF_SHINE_COUNT) - iVar8;
	if (totalShineCount > 100)
		totalShineCount = 99;
	if (totalShineCount < 0)
		totalShineCount = 0;
	((J2DPicture*)mScoreScreen->search('\0n_d'))
	    ->changeTexture(mDigitTextures[totalShineCount / 10]->getTexInfo(), 0);
	((J2DPicture*)mScoreScreen->search('\0n_e'))
	    ->changeTexture(mDigitTextures[totalShineCount % 10]->getTexInfo(), 0);

	mScoreTotal->setAlpha(0);
	mScoreTableHeader->setAlpha(0);
}

void TCardLoad::loadAfter()
{
	JDrama::TNameRef::loadAfter();
	unk278[0] = static_cast<TFileLoadBlock*>(
	    JDrama::TNameRefGen::search("ロードブロックＡ"));
	unk278[1] = static_cast<TFileLoadBlock*>(
	    JDrama::TNameRefGen::search("ロードブロックＢ"));
	unk278[2] = static_cast<TFileLoadBlock*>(
	    JDrama::TNameRefGen::search("ロードブロックＣ"));
	unk284 = static_cast<TMapObjOptionWall*>(
	    JDrama::TNameRefGen::search("オプション用壁"));
}

// fabricated and very questionable
inline void TCardLoad::updateEnterScore()
{
	bool done = true;
	int rc    = gpCardManager->getLastStatus();
	if (rc != CARD_RESULT_BUSY) {
		if (rc == CARD_RESULT_READY) {
			if (!unkB8) {
				loadBookmark();
				setupScoreScreen();
				unkB8 = true;
			}
		} else {
			mScreenState = SCREEN_FILE_SELECT;
			unk1C        = PROGRESS_NO_CARD;
			mWindowStep  = WINDOW_OPENING;
		}
	}

	if (gpCameraOption->unk16 != 0)
		unkB8 = false;

	if (unkB8) {
		J2DPane* root1 = mScoreScreen->search('ROOT');
		J2DPane* root2 = mLoadScreen->search('ROOT');

		int alpha1 = root1->getAlpha();
		alpha1 += 4;
		if (alpha1 > 255)
			alpha1 = 255;
		else
			done = false;
		root1->setAlpha(alpha1);
		for (int i = 0; i < 7; ++i)
			mScoreTableStageRows[i].mRoot->setAlpha(alpha1);
		mScoreTotal->setAlpha(alpha1);
		mScoreTableHeader->setAlpha(alpha1);

		int alpha2 = root2->getAlpha() - 4;
		if (alpha2 < 0)
			alpha2 = 0;
		else
			done = false;
		root2->setAlpha(alpha2);

		if (done)
			mScreenState = SCREEN_SCORE;
	}
}

void TCardLoad::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_MOVE) {
		switch (mScreenState) {
		case SCREEN_FILE_SELECT: {
			changeScene();
			int alpha = updateToSettingsArrowAlpha();
			if (alpha != 0)
				updateToSettingsArrowScale();

			if (unk1C == PROGRESS_SELECT_FILE
			    || unk1C == PROGRESS_INSUFFICIENT_SPACE
			    || unk1C == PROGRESS_NO_FREE_ENTRY || unk1C == PROGRESS_NO_CARD
			    || unk1C == PROGRESS_WRONG_DEVICE
			    || unk1C == PROGRESS_INCOMPATIBLE_CARD
			    || unk1C == PROGRESS_IO_ERROR || unk1C == PROGRESS_NO_SAVE_FILE
#if !defined(VERSION_GMSJ01)
			    || unk1C == PROGRESS_CONTINUE_WITHOUT_SAVING
#endif
			) {
				unk284->offCollision();
				if (!(gpCameraOption->unk0 & 1)) {
					if (mWindowStep == WINDOW_SHOWN)
						mWindowStep = WINDOW_CLOSE;
					if (mWindowStep == WINDOW_DONE)
						mScreenState = SCREEN_ENTER_OPTIONS;
				}
			}
		} break;

		case SCREEN_ENTER_SCORE:
			updateEnterScore();
			break;

		case SCREEN_EXIT_SCORE: {
			bool done      = true;
			J2DPane* root1 = mScoreScreen->search('ROOT');
			J2DPane* root2 = mLoadScreen->search('ROOT');

			int alpha1 = root1->getAlpha();
			alpha1 -= 4;
			if (alpha1 < 0)
				alpha1 = 0;
			else
				done = false;
			root1->setAlpha(alpha1);
			for (int i = 0; i < 7; ++i)
				mScoreTableStageRows[i].mRoot->setAlpha(alpha1);
			mScoreTotal->setAlpha(alpha1);
			mScoreTableHeader->setAlpha(alpha1);

			int alpha2 = root2->getAlpha();
			alpha2 += 4;
			if (alpha2 > 255)
				alpha2 = 255;
			else
				done = false;
			root2->setAlpha(alpha2);

			if (done) {
				if (unkB8) {
					gpCameraOption->moveToDown();
					unkB8 = false;
				} else if (gpCameraOption->unk16 == 0) {
					mScreenState = SCREEN_FILE_SELECT;
					unk1C        = PROGRESS_UNK2;
					gpCardManager->getBookmarkInfos(unk40);
					mWindowStep = WINDOW_OPEN;
				}
			}
		} break;
		case SCREEN_SCORE:
			if (mGamePad->checkFrameMeaning(TMarioGamePad::MEANING_MENU_A)
			    || mGamePad->checkFrameMeaning(TMarioGamePad::MEANING_MENU_B)) {
				SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_CANCEL_COMMON, 0,
				                                   nullptr, 0);
				unkB8        = true;
				mScreenState = SCREEN_EXIT_SCORE;
			}
			break;

		case SCREEN_ENTER_OPTIONS:
			if (unk754->movementCard2Option()) {
				mGamePad->offFlag(TMarioGamePad::PAD_FLAG_MENU_INPUT);
				mScreenState = SCREEN_OPTIONS;
			} else if (gpCameraOption->unk0 & 1) {
				mScreenState = SCREEN_EXIT_OPTIONS;
			}
			break;

		case SCREEN_OPTIONS:
			if (unk754->movementOption()) {
#if defined(VERSION_GMSP01)
				changeLanguage(
				    TFlagManager::getInstance()->getFlag(MSF_LANGUAGE));
#endif
				mScreenState = SCREEN_EXIT_OPTIONS;
			}
			break;

		case SCREEN_EXIT_OPTIONS:
			if (!(gpCameraOption->unk0 & 1)) {
				mScreenState = SCREEN_ENTER_OPTIONS;
			} else {
				if (unk754->movementOption2Card()) {
					unk284->onCollision();
					mScreenState = SCREEN_FILE_SELECT;
					mGamePad->onFlag(TMarioGamePad::PAD_FLAG_MENU_INPUT);
					if (unk754->isChangedSetting()) {
						gpCardManager->getBookmarkInfos(unk40);
						unk1C       = PROGRESS_UNK32;
						mWindowStep = WINDOW_OPEN;
					} else {
						gpCardManager->getBookmarkInfos(unk40);
						unk1C       = PROGRESS_UNK2;
						mWindowStep = WINDOW_OPEN;
					}
					unk754->resetChangedSetting();
				}
			}
			break;

		case SCREEN_EXIT_TITLE: {
			s16 alpha = mTitleScreenRoot->getAlpha();
			alpha -= 4;
			if (alpha < 0)
				alpha = 0;
			mTitleScreenRoot->setAlpha(alpha);
			if (gpCameraOption->unkE == 0) {
				MSBgm::startBGM(MSD_BGM_BOSSPAKU_DEMO);
				unk1C        = PROGRESS_UNK30;
				mScreenState = SCREEN_FILE_SELECT;
			}
		} break;

		case SCREEN_TITLE:
			titleDraw();
			if (unk18 >= 4) {
				if (unk18 >= 4 && unkBC >= 100 && gpCameraOption->unkA == 0
				    && (mGamePad->checkFrameMeaning(
				            TMarioGamePad::MEANING_MENU_A)
				        || mGamePad->getTrigger() & 0x1000)) {
					SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_DECIDE, 0,
					                                   nullptr, 0);
					gpCameraOption->moveToLoadFromTitle();
					gpMarioOriginal->waitingStart(nullptr, 0.0f);
					mScreenState = SCREEN_EXIT_TITLE;
				}
				mGameLogoPicture->update();
				mNintendoLogoPicture->update();
				unkBC += 1;
			} else if (mGamePad->checkFrameMeaning(
			               TMarioGamePad::MEANING_MENU_A)
			           || mGamePad->getTrigger() & 0x1000) {
				mGameLogoPicture->setPaneAlpha(
				    10, 255, mGameLogoPicture->getPane()->getAlpha());
				mNintendoLogoPicture->setPaneAlpha(
				    10, 255, mNintendoLogoPicture->getPane()->getAlpha());
				hideGameLogoLetters();
				mTitleAnimTimer = 0;
				if (unk18 < 4)
					unk18 = 4;
			}

#if !defined(VERSION_GMSP01_NX)
			if (unkC0 / 120.0f > 45.0f) {
				if (TFlagManager::getInstance()->getBool(MSF_UNK3001C)) {
					SMSGetMarDirector()->fireStreamingMovie(9);
					TFlagManager::getInstance()->setBool(false, MSF_UNK3001C);
					unkC0 = 0;
				} else {
					SMSGetMarDirector()->fireStreamingMovie(12);
					TFlagManager::getInstance()->setBool(true, MSF_UNK3001C);
					unkC0 = 0;
				}
			}
#endif
			break;

		case SCREEN_ENTER_TITLE:
			if (gpCameraOption->unkA == 0)
				mScreenState = SCREEN_TITLE;

			if (mGamePad->checkFrameMeaning(TMarioGamePad::MEANING_MENU_A)
			    || mGamePad->getTrigger() & 0x1000) {
				mGameLogoPicture->setPaneAlpha(
				    10, 255, mGameLogoPicture->getPane()->getAlpha());
				mNintendoLogoPicture->setPaneAlpha(
				    10, 255, mNintendoLogoPicture->getPane()->getAlpha());
				hideGameLogoLetters();
				mTitleAnimTimer = 0;
				unkBC           = 0;
				unk18           = 4;
				mScreenState    = SCREEN_TITLE;
			}
			titleDraw();
			break;

		case SCREEN_INIT_TITLE:
			MSBgm::startBGM(MSD_BGM_CHUBOSS2);
			mGamePad->onFlag(TMarioGamePad::PAD_FLAG_MENU_INPUT);
			gpMarioOriginal->offFlag(MARIO_FLAG_HAS_FLUDD);
			mScreenState = SCREEN_ENTER_TITLE;
			unk18        = 1;
			mGameLogoPicture->getPane()->setAlpha(0);
			mNintendoLogoPicture->getPane()->setAlpha(0);
			mTitleAnimTimer = 0;
			unkC0           = 0;
			mTitleScreenRoot->show();
			break;
		}

		if (unkC0 < 0x0FFFFFFF)
			unkC0 += 1;
	}

	if (cue & CUE_DRAW) {
		const JDrama::TRect& rect = graphics->getScissor();
		J2DOrthoGraph graph(graphics->getViewport());
		graph.setup2D();
		switch (mScreenState) {
		case SCREEN_FILE_SELECT:
			mLoadScreen->draw(0, 0, &graph);
			break;
		case SCREEN_SCORE:
			mScoreScreen->draw(0, 0, &graph);
			break;
		case SCREEN_OPTIONS:
			unk754->draw(&graph);
			break;
		case SCREEN_ENTER_SCORE:
		case SCREEN_EXIT_SCORE:
			mScoreScreen->draw(0, 0, &graph);
			graph.setup2D();
			mLoadScreen->draw(0, 0, &graph);
			break;
		case SCREEN_TITLE:
		case SCREEN_EXIT_TITLE:
		case SCREEN_ENTER_TITLE:
			mTitleScreen->draw(0, 0, &graph);
			break;
		}

		graphics->setScissor(rect);
	}
}

bool TCardLoad::titleDraw()
{
	switch (unk18) {
	case 0:
		unk18 = 1;
		break;

	case 1:
		for (int i = 0; i < CARD_LOAD_GAME_LOGO_NUM; ++i) {
			switch (mGameLogoAnimState[i]) {
			case 4:
				if (mTitleAnimTimer > mGameLogoAnimTimes[i]) {
					mGameLogoLetters[i]->getPane()->show();
					JUTRect bounds
					    = mGameLogoLetters[i]->getPane()->getBounds();
					mGameLogoLetters[i]->setPaneAlpha(40, 180, 0);
					mGameLogoAnimState[i] = 0;
				}
				break;

			case 0:
				if (mGameLogoLetters[i]->update()) {
					mGameLogoAnimState[i] = 1;
					mGameLogoAnimTimes[i] = 0;
				}
				break;

			case 1:
				if (i == CARD_LOAD_GAME_LOGO_NUM - 1) {
					mTitleAnimTimer = 0;
					for (int j = 0; j < CARD_LOAD_GAME_LOGO_NUM; ++j)
						mGameLogoLetters[j]->setPaneAlpha(40, 255, 180);
					unk18 = 3;
				}
				break;
			}

			++mTitleAnimTimer;
		}
		break;

	case 3:
		if (mTitleAnimTimer > 160) {
			bool any = true;
			for (int i = 0; i < CARD_LOAD_GAME_LOGO_NUM; ++i) {
				any &= mGameLogoLetters[i]->update();
				JUtility::TColor col
				    = ((J2DPicture*)mGameLogoLetters[i]->getPane())->getBlack();
				u16 r = col.r;
				r += 7;
				if (r > 255)
					r = 255;

				((J2DPicture*)mGameLogoLetters[i]->getPane())
				    ->setBlack((r << 24) + 0xFFFF00);
			}

			if (any) {
				for (int i = 0; i < CARD_LOAD_GAME_LOGO_NUM; ++i)
					mGameLogoLetters[i]->setPaneAlpha(140, 0, 255);
				mTitleAnimTimer = 0;
				unk18           = 2;
			}
		}
		++mTitleAnimTimer;
		break;

	case 2: {
		u16 alpha = mGameLogoPicture->getPane()->getAlpha();
		++alpha;
		if (alpha > 255) {
			bool any = true;
			alpha    = 255;
			for (int i = 0; i < CARD_LOAD_GAME_LOGO_NUM; ++i)
				any &= mGameLogoLetters[i]->update();
			if (any) {
				unkBC = 0;
				unk18 = 4;
			}
		}
		mGameLogoPicture->getPane()->setAlpha(alpha);
		mNintendoLogoPicture->getPane()->setAlpha(alpha);
	} break;

	case 4: {
		for (int i = 0; i < CARD_LOAD_PRESS_START_NUM; ++i) {
			switch (mPressStartAnimState[i]) {
			case 0:
				if (mPressStartTextLetters[i]->update()) {
					mPressStartAnimState[i] = 1;
					mPressStartAnimTimes[i] = 0;
				}
				break;

			case 1:
				mPressStartAnimTimes[i] += 1;
				if (mPressStartAnimTimes[i] > 500) {
					JUTRect bounds
					    = mPressStartTextLetters[i]->getPane()->getBounds();
					mPressStartTextLetters[i]->setPaneAlpha(25, 0, 255);
					mPressStartAnimState[i] = 2;
				}
				break;

			case 2:
				if (mPressStartTextLetters[i]->update()) {
					mPressStartAnimTimes[i] = 0;
					mPressStartAnimState[i] = 3;
				}
				break;

			case 3:
				++mPressStartAnimTimes[i];
				if (mPressStartAnimTimes[i] > 300) {
					JUTRect local_124
					    = mPressStartTextLetters[i]->getPane()->getBounds();

					mPressStartTextLetters[i]->setCenteredSize(
					    25, local_124.getWidth(), local_124.getHeight(),
					    local_124.getWidth() * 2, local_124.getHeight() * 2);

					mPressStartTextLetters[i]->setPaneAlpha(25, 255, 0);
					mPressStartAnimState[i] = 0;
				}
				break;

			case 4:
				if (mTitleAnimTimer > mPressStartAnimTimes[i]) {
					JUTRect local_124 = mPressStartTextLettersInitialBounds[i];
					mPressStartTextLetters[i]->setCenteredSize(
					    25, local_124.getWidth(), local_124.getHeight(),
					    local_124.getWidth() * 2, local_124.getHeight() * 2);
					mPressStartTextLetters[i]->setPaneAlpha(25, 255, 0);
					mPressStartAnimState[i] = 0;
					mPressStartAnimTimes[i] = 0;
				}
				break;
			}
		}
		++mTitleAnimTimer;
	} break;

	// Cases 5 and 6 fall through the default gap. Explicit empty cases here
	// merge the case ranges and prevent MWCC from emitting retail's table.
	case 7:
		break;
	}

	return unk18 > 4;
}

void TCardLoad::makeBuffer(J2DTextBox* text_box, int size)
{
	SMSMakeTextBuffer(text_box, size);
	text_box->setFont((JUTFont*)gpSystemFont);
}

TEProgress TCardLoad::changeMode(s32 param_1)
{
	gpCardManager->probe();
	TEProgress result = PROGRESS_WRONG_DEVICE;
	switch (param_1) {
	case CARD_RESULT_NOCARD:
		result = PROGRESS_NO_CARD;
		break;
	case CARD_RESULT_WRONGDEVICE:
	case -0x80:
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

void TCardLoad::setMessage(J2DTextBox* text_box, s32 param_2, int param_3)
{
	strncpy(text_box->getStringPtr(), SMSGetMessageData(unkA0, param_3),
	        param_2);
}

#if defined(VERSION_GMSP01) || defined(VERSION_GMSP01_NX)
void TCardLoad::resizeBox(J2DWindow* window, J2DTextBox* text,
                          J2DTextBox* shadow)
{
	const char* str      = text->getStringPtr();
	JUTRect windowBounds = window->getBounds();

	J2DPrint print((JUTFont*)gpSystemFont, 0);
	J2DPrint::TSize size;
	print.getSize(size, str);

	JUTRect contents = window->getContentsBounds();
	int width
	    = 10.0f + (size.unk0 + (windowBounds.getWidth() - contents.getWidth()));
	int height
	    = 10.0f
	      + (size.unk4 + (windowBounds.getHeight() - contents.getHeight()));
	window->resize(width, height);

	JUTRect newBounds = window->getBounds();
	int dx            = (windowBounds.x2 - newBounds.x2) / 2;
	int dy            = (windowBounds.y2 - newBounds.y2) / 2;
	text->resize(10.0f + size.unk0, 10.0f + size.unk4);
	shadow->resize(10.0f + size.unk0, 10.0f + size.unk4);
	window->add(dx, dy);
}
#endif

s8 TCardLoad::waitForChoice(TEProgress param_1, TEProgress param_2, int param_3)
{
	s8 result = -1;

	switch (mWindowStep) {
	case WINDOW_OPEN:
		setMessage(unk47C, 0x400, (u16)cMessageID[unk1C]);
		setMessage(unk480, 0x400, (u16)cMessageID[unk1C]);

		unk468->getPane()->show();

		if (param_3 == 1) {
			((J2DPicture*)unk484[0]->getPane())->setWhite(0xFFFFFFFF);
			((J2DPicture*)unk484[1]->getPane())->setWhite(0x00FF00FF);
		} else {
			((J2DPicture*)unk484[1]->getPane())->setWhite(0xFFFFFFFF);
			((J2DPicture*)unk484[0]->getPane())->setWhite(0x00FF00FF);
		}
		unk480->hide();
		unk484[0]->getPane()->hide();
		unk484[1]->getPane()->hide();
		unk468->setCenteredSize(20, unk46C.getWidth(), unk46C.getHeight(), 0,
		                        0);

		unkB7       = param_3;
		mWindowStep = WINDOW_OPENING;
		break;

	case WINDOW_OPENING:
		if (unk468->update()) {
			unk480->show();
			unk484[0]->getPane()->show();
			unk484[1]->getPane()->show();
			unkC4       = 0;
			mWindowStep = WINDOW_SHOWN;
		}
		break;

	case WINDOW_SHOWN: {
		s8 old = unkB7;
		if (mGamePad->checkFrameMeaning(TMarioGamePad::MEANING_MENU_LEFT)) {
			unkB7 = 0;
		} else if (mGamePad->checkFrameMeaning(
		               TMarioGamePad::MEANING_MENU_RIGHT)) {
			unkB7 = 1;
		} else if (mGamePad->checkFrameMeaning(TMarioGamePad::MEANING_MENU_A)) {
			if (old == 0)
				SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_SELECT_COMMON, 0,
				                                   nullptr, 0);
			else
				SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_CANCEL_COMMON, 0,
				                                   nullptr, 0);
			mWindowStep = WINDOW_CLOSE;
		} else if (mGamePad->checkFrameMeaning(TMarioGamePad::MEANING_MENU_B)) {
			unkB7 = 1;
			SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_CANCEL_COMMON, 0,
			                                   nullptr, 0);
			mWindowStep = WINDOW_CLOSE;
		}

		if (unkC4 == 4) {
			unk484[unkB7]->setCenteredSize(20, unk48C[unkB7].getWidth() * 0.5f,
			                               unk48C[unkB7].getHeight() * 0.5f,
			                               unk48C[unkB7].getWidth(),
			                               unk48C[unkB7].getHeight());
			JUTRect bounds = unk484[unkB7]->getPane()->getGlbBounds();
			JGeometry::TVec3<f32> local_108;
			local_108.x = bounds.x1 + bounds.getWidth() * 0.5f;
			local_108.y = bounds.y1 + bounds.getHeight() * 0.5f;
			local_108.z = 0.0f;
			gpEmitterManager4D2->createEmitter(local_108, 0x1FA, nullptr,
			                                   nullptr);
			unkAC = gpEmitterManager4D2->unkC8[0][0];
			unkAC->setRotation(0, 0, DEG2SHORTANGLE(12));
			unkAC->setEmitterScale(JGeometry::TVec3<f32>(0.9f, 1.0f, 0.1f));
		} else if (unkC4 == 44) {
			unk484[unkB7]->setCenteredSize(20, unk48C[unkB7].getWidth(),
			                               unk48C[unkB7].getHeight(),
			                               unk48C[unkB7].getWidth() * 0.5f,
			                               unk48C[unkB7].getHeight() * 0.5f);
		}

		int thing = unkC4 < 40 ? unkC4 : 80 - unkC4;
		thing     = (thing * 255.0f) / 40.0f;
		((J2DPicture*)unk484[unkB7]->getPane())
		    ->setWhite((thing << 24) + 0xFF00FF);
		unk484[unkB7]->update();
		unkC4 += 1;
		if (unkC4 > 80)
			unkC4 = 0;
		if (old != unkB7) {
			if (unkAC != nullptr)
				gpEmitterManager4D2->forceDeleteEmitter(unkAC);

			unkC4 = 0;

			unk484[old]->getPane()->place(unk48C[old]);
			SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_E3_MENU_CURSOR, 0,
			                                   nullptr, 0);
			((J2DPicture*)unk484[old]->getPane())->setWhite(0xFFFFFFFF);
			((J2DPicture*)unk484[unkB7]->getPane())->setWhite(0x00FF00FF);
		}
	} break;

	case WINDOW_CLOSE:
		if (unkB7 == -1)
			unkB7 = 2;
		unk468->setCenteredSize(20, 0, 0, unk46C.getWidth(),
		                        unk46C.getHeight());
		unk480->hide();
		unk484[0]->getPane()->hide();
		unk484[1]->getPane()->hide();
		unk484[0]->getPane()->place(unk48C[0]);
		unk484[1]->getPane()->place(unk48C[1]);

		mWindowStep = WINDOW_CLOSING;
		break;

	case WINDOW_CLOSING:
		if (unk468->update()) {
			unk468->getPane()->hide();
			mWindowStep = WINDOW_DONE;
		}
		break;

	case WINDOW_DONE:
		if (unkB7 == 0)
			unk1C = param_1;
		else
			unk1C = param_2;
		result = unkB7;
		break;
	}

	return result;
}

s8 TCardLoad::waitForChoiceBM(TEProgress param_1, TEProgress param_2,
                              int param_3)
{
	s8 result = -1;

	switch (mWindowStep) {
	case WINDOW_OPEN:
		setMessage(mConfirmationText, 0x400, (u16)cMessageID[unk1C]);
		setMessage(mConfirmationTextShadow, 0x400, (u16)cMessageID[unk1C]);

		mConfirmationWindow->getPane()->show();

		if (param_3 == 1) {
			((J2DPicture*)mConfirmationYesNo[0]->getPane())
			    ->setWhite(0xFFFFFFFF);
			mConfirmationYesNo[0]->getPane()->hide();
			((J2DPicture*)mConfirmationYesNo[1]->getPane())
			    ->setWhite(0x00FF00FF);
			mConfirmationYesNo[1]->getPane()->hide();
		} else {
			((J2DPicture*)mConfirmationYesNo[1]->getPane())
			    ->setWhite(0xFFFFFFFF);
			mConfirmationYesNo[1]->getPane()->hide();
			((J2DPicture*)mConfirmationYesNo[0]->getPane())
			    ->setWhite(0x00FF00FF);
			mConfirmationYesNo[0]->getPane()->hide();
		}
		mConfirmationShineCountOver100->hide();
		mConfirmationShineCountUnder100->hide();
		mConfirmationNewSaveTextShadow->hide();
		mConfirmationTextShadow->hide();
		mConfirmationTextMario->hide();
		for (int i = 0; i < 3; ++i)
			mConfirmationTextTargetABC[i]->hide();
		mConfirmationWindow->setCenteredSize(
		    20, mConfirmationWindowInitialBounds.getWidth(),
		    mConfirmationWindowInitialBounds.getHeight(), 0, 0);

		unkB7       = param_3;
		mWindowStep = WINDOW_OPENING;
		break;

	case WINDOW_OPENING:
		if (mConfirmationWindow->update()) {
			mConfirmationTextShadow->show();
			mConfirmationYesNo[0]->getPane()->show();
			mConfirmationYesNo[1]->getPane()->show();
			mConfirmationTextMario->show();
			mConfirmationTextTargetABC[unkB0]->show();
			TCardBookmarkInfo& info = unk40[unkB0];
			if (info.unk0 == 1) {
#if defined(VERSION_GMSJ01)
				strncpy(mConfirmationNewSaveText->getStringPtr(),
				        "こわれています", 0x14);
				strncpy(mConfirmationNewSaveTextShadow->getStringPtr(),
				        "こわれています", 0x14);
#else
				setMessage(mConfirmationNewSaveText, 0x14, 0);
				setMessage(mConfirmationNewSaveTextShadow, 0x14, 0);
#endif
				mConfirmationNewSaveTextShadow->show();
				mConfirmationShineCountOver100->hide();
				mConfirmationShineCountUnder100->hide();
			} else if (info.unk18 == 0) {
#if defined(VERSION_GMSJ01)
				strncpy(mConfirmationNewSaveText->getStringPtr(), "NEW", 0x14);
				strncpy(mConfirmationNewSaveTextShadow->getStringPtr(), "NEW",
				        0x14);
#else
				setMessage(mConfirmationNewSaveText, 0x14, 0x1B);
				setMessage(mConfirmationNewSaveTextShadow, 0x14, 0x1B);
#endif
				mConfirmationNewSaveTextShadow->show();
				mConfirmationShineCountOver100->hide();
				mConfirmationShineCountUnder100->hide();
			} else {
				mConfirmationNewSaveTextShadow->hide();
				u16 score = unk40[unkB0].unk1C;
				if (score > 999)
					score = 999;
				if (score < 100) {
					mConfirmationShineCountOver100->hide();
					mConfirmationShineCountUnder100->show();
					mConfirmationShineCountDigitsUnder100[0]->changeTexture(
					    mDigitTextures[score / 10]->getTexInfo(), 0);
					mConfirmationShineCountDigitsUnder100[1]->changeTexture(
					    mDigitTextures[score % 10]->getTexInfo(), 0);
				} else {
					mConfirmationShineCountOver100->show();
					mConfirmationShineCountUnder100->hide();
					mConfirmationShineCountDigitsOver100[0]->changeTexture(
					    mDigitTextures[score / 100]->getTexInfo(), 0);
					score -= int(score * 0.01f) * 100;
					mConfirmationShineCountDigitsOver100[1]->changeTexture(
					    mDigitTextures[score / 10]->getTexInfo(), 0);
					mConfirmationShineCountDigitsOver100[2]->changeTexture(
					    mDigitTextures[score % 10]->getTexInfo(), 0);
				}
			}
			unkC4       = 0;
			mWindowStep = WINDOW_SHOWN;
		}
		break;

	case WINDOW_SHOWN: {
		s8 old = unkB7;
		if (mGamePad->checkFrameMeaning(TMarioGamePad::MEANING_MENU_LEFT)) {
			unkB7 = 0;
		} else if (mGamePad->checkFrameMeaning(
		               TMarioGamePad::MEANING_MENU_RIGHT)) {
			unkB7 = 1;
		} else if (mGamePad->checkFrameMeaning(TMarioGamePad::MEANING_MENU_A)) {
			SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_SELECT_COMMON, 0,
			                                   nullptr, 0);
			mWindowStep = WINDOW_CLOSE;
		} else if (mGamePad->checkFrameMeaning(TMarioGamePad::MEANING_MENU_B)) {
			unkB7 = 1;
			SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_CANCEL_COMMON, 0,
			                                   nullptr, 0);
			mWindowStep = WINDOW_CLOSE;
		}

		if (unkC4 == 4) {
			mConfirmationYesNo[unkB7]->setCenteredSize(
			    40, mConfirmationYesNoInitialBounds[unkB7].getWidth() * 1.5f,
			    mConfirmationYesNoInitialBounds[unkB7].getHeight() * 1.5f,
			    mConfirmationYesNoInitialBounds[unkB7].getWidth(),
			    mConfirmationYesNoInitialBounds[unkB7].getHeight());
			JUTRect bounds
			    = mConfirmationYesNo[unkB7]->getPane()->getGlbBounds();
			JGeometry::TVec3<f32> local_108;
			local_108.x = bounds.x1 + bounds.getWidth() * 0.5f;
			local_108.y = bounds.y1 + bounds.getHeight() * 0.5f;
			local_108.z = 0.0f;
			gpEmitterManager4D2->createEmitter(local_108, 0x1FA, nullptr,
			                                   nullptr);
			unkAC = gpEmitterManager4D2->unkC8[0][0];
			unkAC->setRotation(0, 0, DEG2SHORTANGLE(12));
			unkAC->setEmitterScale(JGeometry::TVec3<f32>(0.9f, 1.0f, 0.1f));
		} else if (unkC4 == 44) {
			mConfirmationYesNo[unkB7]->setCenteredSize(
			    40, mConfirmationYesNoInitialBounds[unkB7].getWidth(),
			    mConfirmationYesNoInitialBounds[unkB7].getHeight(),
			    mConfirmationYesNoInitialBounds[unkB7].getWidth() * 1.5f,
			    mConfirmationYesNoInitialBounds[unkB7].getHeight() * 1.5f);
		}

		int thing = unkC4 < 40 ? unkC4 : 80 - unkC4;
		thing     = (thing * 255.0f) / 40.0f;
		((J2DPicture*)mConfirmationYesNo[unkB7]->getPane())
		    ->setWhite((thing << 24) + 0xFF00FF);
		mConfirmationYesNo[unkB7]->update();
		unkC4 += 1;
		if (unkC4 > 80)
			unkC4 = 0;
		if (old != unkB7) {
			if (unkAC != nullptr)
				gpEmitterManager4D2->forceDeleteEmitter(unkAC);

			unkC4 = 0;

			mConfirmationYesNo[old]->getPane()->place(
			    mConfirmationYesNoInitialBounds[old]);
			SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_E3_MENU_CURSOR, 0,
			                                   nullptr, 0);
			((J2DPicture*)mConfirmationYesNo[old]->getPane())
			    ->setWhite(0xFFFFFFFF);
			((J2DPicture*)mConfirmationYesNo[unkB7]->getPane())
			    ->setWhite(0x00FF00FF);
		}
	} break;

	case WINDOW_CLOSE: {
		mConfirmationWindow->setCenteredSize(
		    20, 0, 0, mConfirmationWindowInitialBounds.getWidth(),
		    mConfirmationWindowInitialBounds.getHeight());
		mConfirmationTextMario->hide();
		mConfirmationShineCountOver100->hide();
		mConfirmationShineCountUnder100->hide();
		mConfirmationNewSaveTextShadow->hide();
		mConfirmationTextShadow->hide();
		mConfirmationYesNo[0]->getPane()->hide();
		mConfirmationYesNo[1]->getPane()->hide();
		mConfirmationYesNo[0]->getPane()->place(
		    mConfirmationYesNoInitialBounds[0]);
		mConfirmationYesNo[1]->getPane()->place(
		    mConfirmationYesNoInitialBounds[1]);

		mWindowStep = WINDOW_CLOSING;
	} break;

	case WINDOW_CLOSING:
		if (mConfirmationWindow->update()) {
			mConfirmationWindow->getPane()->hide();
			mWindowStep = WINDOW_DONE;
		}
		break;

	case WINDOW_DONE:
		if (unkB7 == 0)
			unk1C = param_1;
		else
			unk1C = param_2;
		result = unkB7;
		break;
	}

	return result;
}

s8 TCardLoad::waitForAnyKey(TEProgress progress)
{
	s8 result = -1;

	switch (mWindowStep) {
	case WINDOW_OPEN: {
		setMessage(mErasedText, 0x400, (u16)cMessageID[unk1C]);
		setMessage(mErasedTextShadow, 0x400, (u16)cMessageID[unk1C]);
#if defined(VERSION_GMSP01_NX)
		if (TFlagManager::getInstance()->getFlag(MSF_LANGUAGE)
		    != MSLANG_NX_JAPANESE)
			resizeBox((J2DWindow*)mErasedWindow->getPane(), mErasedTextShadow,
			          mErasedText);
#elif defined(VERSION_GMSP01)
		resizeBox((J2DWindow*)mErasedWindow->getPane(), mErasedTextShadow,
		          mErasedText);
#endif

		mErasedTextShadow->hide();
		mErasedWindow->getPane()->show();
#if defined(VERSION_GMSP01_NX)
		if (TFlagManager::getInstance()->getFlag(MSF_LANGUAGE)
		    != MSLANG_NX_JAPANESE) {
			JUTRect bounds = mErasedWindow->getPane()->getBounds();
			mErasedWindow->setCenteredSize(20, bounds.getWidth(),
			                               bounds.getHeight(), 0, 0);
		} else {
			mErasedWindow->setCenteredSize(
			    20, mErasedWindowInitialBounds.getWidth(),
			    mErasedWindowInitialBounds.getHeight(), 0, 0);
		}
#elif defined(VERSION_GMSP01)
		{
			JUTRect bounds = mErasedWindow->getPane()->getBounds();
			mErasedWindow->setCenteredSize(20, bounds.getWidth(),
			                               bounds.getHeight(), 0, 0);
		}
#else
		mErasedWindow->setCenteredSize(
		    20, mErasedWindowInitialBounds.getWidth(),
		    mErasedWindowInitialBounds.getHeight(), 0, 0);
#endif
		mWindowStep = WINDOW_OPENING;
	} break;

	case WINDOW_OPENING:
		if (mErasedWindow->update()) {
			mErasedTextShadow->show();
			unkB4       = 0;
			mWindowStep = WINDOW_SHOWN;
		}
		break;

	case WINDOW_SHOWN:
		if (unkB4 > 600
		    || mGamePad->checkFrameMeaning(TMarioGamePad::MEANING_MENU_A
		                                   | TMarioGamePad::MEANING_MENU_B)) {
			SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_SELECT_COMMON, 0,
			                                   nullptr, 0);
			mWindowStep = WINDOW_CLOSE;
		} else {
			++unkB4;
		}
		break;

	case WINDOW_CLOSE: {
#if defined(VERSION_GMSP01_NX)
		if (TFlagManager::getInstance()->getFlag(MSF_LANGUAGE)
		    != MSLANG_NX_JAPANESE) {
			JUTRect bounds = mErasedWindow->getPane()->getBounds();
			mErasedWindow->setCenteredSize(20, 0, 0, bounds.getWidth(),
			                               bounds.getHeight());
		} else {
			mErasedWindow->setCenteredSize(
			    20, 0, 0, mErasedWindowInitialBounds.getWidth(),
			    mErasedWindowInitialBounds.getHeight());
		}
#elif defined(VERSION_GMSP01)
		{
			JUTRect bounds = mErasedWindow->getPane()->getBounds();
			mErasedWindow->setCenteredSize(20, 0, 0, bounds.getWidth(),
			                               bounds.getHeight());
		}
#else
		mErasedWindow->setCenteredSize(20, 0, 0,
		                               mErasedWindowInitialBounds.getWidth(),
		                               mErasedWindowInitialBounds.getHeight());
#endif
		mErasedTextShadow->hide();
		unkB4       = 0;
		mWindowStep = WINDOW_CLOSING;
	} break;

	case WINDOW_CLOSING:
		if (mErasedWindow->update()) {
			mErasedWindow->getPane()->hide();
			mWindowStep = WINDOW_DONE;
		}
		break;

	case WINDOW_DONE:
		result = 0;
		unk1C  = progress;
		break;
	}

	return result;
}

s8 TCardLoad::waitForAnyKeyBM(TEProgress param_1)
{
	s8 result = -1;

	switch (mWindowStep) {
	case WINDOW_OPEN:
		setMessage(mConfirmationText, 0x400, (u16)cMessageID[unk1C]);
		setMessage(mConfirmationTextShadow, 0x400, (u16)cMessageID[unk1C]);

		mConfirmationWindow->getPane()->show();
		mConfirmationShineCountOver100->hide();
		mConfirmationShineCountUnder100->hide();
		mConfirmationNewSaveTextShadow->hide();
		mConfirmationTextShadow->hide();
		mConfirmationTextMario->hide();
		for (int i = 0; i < 3; ++i)
			mConfirmationTextTargetABC[i]->hide();

		mConfirmationWindow->setCenteredSize(
		    20, mConfirmationWindowInitialBounds.getWidth(),
		    mConfirmationWindowInitialBounds.getHeight(), 0, 0);
		mWindowStep = WINDOW_OPENING;
		break;

	case WINDOW_OPENING:
		if (mConfirmationWindow->update()) {
			mConfirmationTextShadow->show();
			mConfirmationTextMario->show();
			mConfirmationTextTargetABC[unkB0]->show();
			if (unk1C == PROGRESS_COPY_DONE)
				unkB0 = unkB1;
			TCardBookmarkInfo& info = unk40[unkB0];
			if (info.unk0 == 1) {
#if defined(VERSION_GMSJ01)
				strncpy(mConfirmationNewSaveText->getStringPtr(),
				        "こわれています", 0x14);
				strncpy(mConfirmationNewSaveTextShadow->getStringPtr(),
				        "こわれています", 0x14);
#else
				setMessage(mConfirmationNewSaveText, 0x14, 0);
				setMessage(mConfirmationNewSaveTextShadow, 0x14, 0);
#endif
				mConfirmationNewSaveTextShadow->show();
				mConfirmationShineCountOver100->hide();
				mConfirmationShineCountUnder100->hide();
			} else if (info.unk18 == 0) {
#if defined(VERSION_GMSJ01)
				strncpy(mConfirmationNewSaveText->getStringPtr(), "NEW", 0x14);
				strncpy(mConfirmationNewSaveTextShadow->getStringPtr(), "NEW",
				        0x14);
#else
				setMessage(mConfirmationNewSaveText, 0x14, 0x1B);
				setMessage(mConfirmationNewSaveTextShadow, 0x14, 0x1B);
#endif
				mConfirmationNewSaveTextShadow->show();
				mConfirmationShineCountOver100->hide();
				mConfirmationShineCountUnder100->hide();
			} else {
				mConfirmationShineCountOver100->show();
				mConfirmationNewSaveTextShadow->hide();
				u16 score = unk40[unkB0].unk1C;
				if (score > 999)
					score = 999;
				if (score < 100) {
					mConfirmationShineCountOver100->hide();
					mConfirmationShineCountUnder100->show();
					mConfirmationShineCountDigitsUnder100[0]->changeTexture(
					    mDigitTextures[score / 10]->getTexInfo(), 0);
					mConfirmationShineCountDigitsUnder100[1]->changeTexture(
					    mDigitTextures[score % 10]->getTexInfo(), 0);
				} else {
					mConfirmationShineCountOver100->show();
					mConfirmationShineCountUnder100->hide();
					mConfirmationShineCountDigitsOver100[0]->changeTexture(
					    mDigitTextures[score / 100]->getTexInfo(), 0);
					score -= int(score * 0.01f) * 100;
					mConfirmationShineCountDigitsOver100[1]->changeTexture(
					    mDigitTextures[score / 10]->getTexInfo(), 0);
					mConfirmationShineCountDigitsOver100[2]->changeTexture(
					    mDigitTextures[score % 10]->getTexInfo(), 0);
				}
			}
			unkB4       = 0;
			mWindowStep = WINDOW_SHOWN;
		}
		break;

	case WINDOW_SHOWN: {
		if (unkB4 > 600
		    || mGamePad->checkFrameMeaning(TMarioGamePad::MEANING_MENU_A
		                                   | TMarioGamePad::MEANING_MENU_B)) {
			SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_SELECT_COMMON, 0,
			                                   nullptr, 0);
			mWindowStep = WINDOW_CLOSE;
		} else {
			++unkB4;
		}
	} break;

	case WINDOW_CLOSE:
		mConfirmationWindow->setCenteredSize(
		    20, 0, 0, mConfirmationWindowInitialBounds.getWidth(),
		    mConfirmationWindowInitialBounds.getHeight());
		mConfirmationTextShadow->hide();
		mConfirmationShineCountOver100->hide();
		mConfirmationShineCountUnder100->hide();
		mConfirmationNewSaveTextShadow->hide();
		mConfirmationTextMario->hide();
		unkB4       = 0;
		mWindowStep = WINDOW_CLOSING;
		break;

	case WINDOW_CLOSING:
		if (mConfirmationWindow->update()) {
			mConfirmationWindow->getPane()->hide();
			mWindowStep = WINDOW_DONE;
		}
		break;

	case WINDOW_DONE:
		result = 0;
		unk1C  = param_1;
		break;
	}

	return result;
}

s8 TCardLoad::waitForStart(TEProgress param_1)
{
	s8 result = -1;

	switch (mWindowStep) {
	case WINDOW_OPEN:
		setMessage(unk538, 0x400, (u16)cMessageID[unk1C]);
		setMessage(unk53C, 0x400, (u16)cMessageID[unk1C]);
		setMessage(unk560, 0x400, 12);
		setMessage(unk564, 0x400, 12);
#if defined(VERSION_GMSP01_NX)
		if (TFlagManager::getInstance()->getFlag(MSF_LANGUAGE)
		    != MSLANG_NX_JAPANESE)
			resizeBox((J2DWindow*)unk524->getPane(), unk53C, unk538);
#elif defined(VERSION_GMSP01)
		resizeBox((J2DWindow*)unk524->getPane(), unk53C, unk538);
#endif

		unk524->getPane()->show();
#if defined(VERSION_GMSP01_NX)
		if (TFlagManager::getInstance()->getFlag(MSF_LANGUAGE)
		    != MSLANG_NX_JAPANESE) {
			JUTRect bounds = unk524->getPane()->getBounds();
			unk524->setCenteredSize(20, bounds.getWidth(), bounds.getHeight(),
			                        0, 0);
		} else {
			unk524->setCenteredSize(20, unk528.getWidth(), unk528.getHeight(),
			                        0, 0);
		}
#elif defined(VERSION_GMSP01)
		{
			JUTRect bounds = unk524->getPane()->getBounds();
			unk524->setCenteredSize(20, bounds.getWidth(), bounds.getHeight(),
			                        0, 0);
		}
#else
		unk524->setCenteredSize(20, unk528.getWidth(), unk528.getHeight(), 0,
		                        0);
#endif
		unk564->hide();
		if (unk1C == PROGRESS_INSUFFICIENT_SPACE
		    || unk1C == PROGRESS_NO_FREE_ENTRY) {
			unk54C->getPane()->show();
			unk54C->setCenteredSize(20, unk550.getWidth(), unk550.getHeight(),
			                        0, 0);
		}
		mWindowStep = WINDOW_OPENING;
		break;

	case WINDOW_OPENING: {
		bool allDone = true;
		allDone &= unk524->update();
		allDone &= unk54C->update();
		if (allDone) {
			mWindowStep = WINDOW_SHOWN;
			unk53C->show();
			unk564->show();
			for (int i = 0; i < 3; ++i)
				unk540[i]->show();
			unkB0 = -1;
		}
	} break;

	case WINDOW_SHOWN:
		if (mGamePad->checkFrameMeaning(TMarioGamePad::MEANING_MENU_A)
		    || unkB0 != -1)
			mWindowStep = WINDOW_CLOSE;
		break;

	case WINDOW_CLOSE:
		unk53C->hide();
		for (int i = 0; i < 3; ++i)
			unk540[i]->hide();
#if defined(VERSION_GMSP01_NX)
		if (TFlagManager::getInstance()->getFlag(MSF_LANGUAGE)
		    != MSLANG_NX_JAPANESE) {
			JUTRect bounds = unk524->getPane()->getBounds();
			unk524->setCenteredSize(20, 0, 0, bounds.getWidth(),
			                        bounds.getHeight());
		} else {
			unk524->setCenteredSize(20, 0, 0, unk528.getWidth(),
			                        unk528.getHeight());
		}
#elif defined(VERSION_GMSP01)
		{
			JUTRect bounds = unk524->getPane()->getBounds();
			unk524->setCenteredSize(20, 0, 0, bounds.getWidth(),
			                        bounds.getHeight());
		}
#else
		unk524->setCenteredSize(20, 0, 0, unk528.getWidth(),
		                        unk528.getHeight());
#endif
		unk564->hide();
		unk54C->setCenteredSize(20, 0, 0, unk550.getWidth(),
		                        unk550.getHeight());
		mWindowStep = WINDOW_CLOSING;
		break;

	case WINDOW_CLOSING: {
		bool allDone = true;
		allDone &= unk524->update();
		allDone &= unk54C->update();
		if (allDone) {
			unk524->getPane()->hide();
			unk54C->getPane()->hide();
			mWindowStep = WINDOW_DONE;
		}
	} break;

	case WINDOW_DONE:
		unk1C  = param_1;
		result = 0;
		break;
	}

	return result;
}

s8 TCardLoad::drawMessage(TEProgress param_1)
{
	switch (mWindowStep) {
	case WINDOW_OPEN:
		setMessage(mErasedText, 0x400, (u16)cMessageID[unk1C]);
		setMessage(mErasedTextShadow, 0x400, (u16)cMessageID[unk1C]);
#if defined(VERSION_GMSP01_NX)
		if (TFlagManager::getInstance()->getFlag(MSF_LANGUAGE)
		    != MSLANG_NX_JAPANESE)
			resizeBox((J2DWindow*)mErasedWindow->getPane(), mErasedTextShadow,
			          mErasedText);
#elif defined(VERSION_GMSP01)
		resizeBox((J2DWindow*)mErasedWindow->getPane(), mErasedTextShadow,
		          mErasedText);
#endif

		mErasedWindow->getPane()->show();
		mErasedTextShadow->hide();
#if defined(VERSION_GMSP01_NX)
		if (TFlagManager::getInstance()->getFlag(MSF_LANGUAGE)
		    != MSLANG_NX_JAPANESE) {
			JUTRect bounds = mErasedWindow->getPane()->getBounds();
			mErasedWindow->setCenteredSize(20, bounds.getWidth(),
			                               bounds.getHeight(), 0, 0);
		} else {
			mErasedWindow->setCenteredSize(
			    20, mErasedWindowInitialBounds.getWidth(),
			    mErasedWindowInitialBounds.getHeight(), 0, 0);
		}
#elif defined(VERSION_GMSP01)
		{
			JUTRect bounds = mErasedWindow->getPane()->getBounds();
			mErasedWindow->setCenteredSize(20, bounds.getWidth(),
			                               bounds.getHeight(), 0, 0);
		}
#else
		mErasedWindow->setCenteredSize(
		    20, mErasedWindowInitialBounds.getWidth(),
		    mErasedWindowInitialBounds.getHeight(), 0, 0);
#endif
		mWindowStep = WINDOW_OPENING;
		break;

	case WINDOW_OPENING:
		if (mErasedWindow->update()) {
			mErasedTextShadow->show();
			unkB4       = 0;
			mWindowStep = WINDOW_SHOWN;
		}
		break;

	case WINDOW_CLOSE:
		if (unkB4 > 180) {
#if defined(VERSION_GMSP01_NX)
			if (TFlagManager::getInstance()->getFlag(MSF_LANGUAGE)
			    != MSLANG_NX_JAPANESE) {
				JUTRect bounds = mErasedWindow->getPane()->getBounds();
				mErasedWindow->setCenteredSize(20, 0, 0, bounds.getWidth(),
				                               bounds.getHeight());
			} else {
				mErasedWindow->setCenteredSize(
				    20, 0, 0, mErasedWindowInitialBounds.getWidth(),
				    mErasedWindowInitialBounds.getHeight());
			}
#elif defined(VERSION_GMSP01)
			{
				JUTRect bounds = mErasedWindow->getPane()->getBounds();
				mErasedWindow->setCenteredSize(20, 0, 0, bounds.getWidth(),
				                               bounds.getHeight());
			}
#else
			mErasedWindow->setCenteredSize(
			    20, 0, 0, mErasedWindowInitialBounds.getWidth(),
			    mErasedWindowInitialBounds.getHeight());
#endif
			mErasedTextShadow->hide();
			mWindowStep = WINDOW_CLOSING;
		}
		break;

	case WINDOW_CLOSING:
		if (mErasedWindow->update()) {
			mErasedWindow->getPane()->hide();
			mWindowStep = WINDOW_DONE;
		}
		break;

	case WINDOW_DONE:
		unk1C = param_1;
		break;
	}

	unkB4 += 1;

	return -1;
}

s8 TCardLoad::drawMessageBM(TEProgress param_1)
{
	s8 result = -1;

	switch (mWindowStep) {
	case WINDOW_OPEN:
		setMessage(mConfirmationText, 0x400, (u16)cMessageID[unk1C]);
		setMessage(mConfirmationTextShadow, 0x400, (u16)cMessageID[unk1C]);

		mConfirmationWindow->getPane()->show();
		mConfirmationShineCountOver100->hide();
		mConfirmationNewSaveTextShadow->hide();
		mConfirmationTextShadow->hide();
		mConfirmationTextMario->hide();
		mConfirmationTextTargetABC[0]->hide();
		mConfirmationTextTargetABC[1]->hide();
		mConfirmationTextTargetABC[2]->hide();

		mConfirmationWindow->setCenteredSize(
		    20, mConfirmationWindowInitialBounds.getWidth(),
		    mConfirmationWindowInitialBounds.getHeight(), 0, 0);
		mWindowStep = WINDOW_OPENING;
		break;

	case WINDOW_OPENING:
		if (mConfirmationWindow->update()) {
			mConfirmationTextShadow->show();
			mConfirmationTextMario->show();
			mConfirmationTextTargetABC[unkB0]->show();
			TCardBookmarkInfo* bm = &unk40[unkB0];
			if (bm->unk0 == 1) {
#if defined(VERSION_GMSJ01)
				strncpy(mConfirmationNewSaveText->getStringPtr(),
				        "こわれています", 20);
				strncpy(mConfirmationNewSaveTextShadow->getStringPtr(),
				        "こわれています", 20);
#else
				setMessage(mConfirmationNewSaveText, 20, 0);
				setMessage(mConfirmationNewSaveTextShadow, 20, 0);
#endif
				mConfirmationNewSaveTextShadow->show();
				mConfirmationShineCountOver100->hide();
			} else if (bm->unk18 == 0) {
#if defined(VERSION_GMSJ01)
				strncpy(mConfirmationNewSaveText->getStringPtr(), "NEW", 20);
				strncpy(mConfirmationNewSaveTextShadow->getStringPtr(), "NEW",
				        20);
#else
				setMessage(mConfirmationNewSaveText, 20, 0x1B);
				setMessage(mConfirmationNewSaveTextShadow, 20, 0x1B);
#endif
				mConfirmationNewSaveTextShadow->show();
				mConfirmationShineCountOver100->hide();
			} else {
				mConfirmationNewSaveTextShadow->hide();
				TCardBookmarkInfo* bm2 = &unk40[unkB0];
				u16 score              = bm2->unk1C;
				(void)score; // TODO: hack, regswap
				if (score > 999)
					score = 999;
				if (score < 100) {
					mConfirmationShineCountOver100->hide();
					mConfirmationShineCountUnder100->show();
					mConfirmationShineCountDigitsUnder100[0]->changeTexture(
					    mDigitTextures[score / 10]->getTexInfo(), 0);
					mConfirmationShineCountDigitsUnder100[1]->changeTexture(
					    mDigitTextures[score % 10]->getTexInfo(), 0);
				} else {
					mConfirmationShineCountOver100->show();
					mConfirmationShineCountUnder100->hide();
					mConfirmationShineCountDigitsOver100[0]->changeTexture(
					    mDigitTextures[score / 100]->getTexInfo(), 0);
					score -= int(score * 0.01f) * 100;
					mConfirmationShineCountDigitsOver100[1]->changeTexture(
					    mDigitTextures[score / 10]->getTexInfo(), 0);
					mConfirmationShineCountDigitsOver100[2]->changeTexture(
					    mDigitTextures[score % 10]->getTexInfo(), 0);
				}
			}
			unkB4       = 0;
			mWindowStep = WINDOW_SHOWN;
		}
		break;

	case WINDOW_CLOSE:
		if (unkB4 > 180) {
			mConfirmationWindow->setCenteredSize(
			    20, 0, 0, mConfirmationWindowInitialBounds.getWidth(),
			    mConfirmationWindowInitialBounds.getHeight());

			mConfirmationTextMario->hide();
			mConfirmationShineCountOver100->hide();
			mConfirmationShineCountUnder100->hide();
			mConfirmationNewSaveTextShadow->hide();
			mConfirmationTextShadow->hide();
			mWindowStep = WINDOW_CLOSING;
		}
		break;

	case WINDOW_CLOSING:
		if (mConfirmationWindow->update()) {
			mConfirmationWindow->getPane()->hide();
			mWindowStep = WINDOW_DONE;
		}
		break;

	case WINDOW_DONE:
		unk1C  = param_1;
		result = 0;
		break;
	}

	unkB4 += 1;

	return result;
}

s8 TCardLoad::selectBookmark(TEProgress param_1, TEProgress param_2,
                             bool param_3)
{
	s8 result = -1;

	switch (mWindowStep) {
	case WINDOW_OPEN:
		unk338 = 0;
		mChoseSaveTextShadow->hide();
		mChoseSaveWindow->getPane()->show();
		mChoseSaveWindow->setCenteredSize(
		    30, mChoseSaveWindowInitialBounds.getWidth(),
		    mChoseSaveWindowInitialBounds.getHeight(), 0, 0);
		if (unk1C == PROGRESS_SELECT_COPY_DEST) {
			unk278[unkB1]->makeBlockRock();
			setMessage(mChoseSaveText, 30,
			           VERSION_SELECT(0x1F, GMSJ01(0x1B), GMSE01(0x1E)));
			setMessage(mChoseSaveTextShadow, 30,
			           VERSION_SELECT(0x1F, GMSJ01(0x1B), GMSE01(0x1E)));
		} else {
			setMessage(mChoseSaveText, 30,
			           VERSION_SELECT(0x1E, GMSJ01(0x1A), GMSE01(0x1D)));
			setMessage(mChoseSaveTextShadow, 30,
			           VERSION_SELECT(0x1E, GMSJ01(0x1A), GMSE01(0x1D)));
		}

		for (int i = 0; i < 3; ++i) {
			if (mSaveWindows[i]->getPane()->isVisible())
				continue;

			unk2CC[i]->hide();
			mSaveShineIconsUnder100[i]->hide();
			mSaveShineIconsOver100[i]->hide();
			mSaveWindows[i]->getPane()->show();
			mSaveWindows[i]->setCenteredSize(30, unk2B0.getWidth(),
			                                 unk2B0.getHeight(), 0, 0);
			unk32C[i]->hide();
		}

		mWindowStep = WINDOW_OPENING;
		break;

	case WINDOW_OPENING: {
		bool done = true;
		done &= mChoseSaveWindow->update();
		for (int i = 0; i < 3; ++i)
			done &= mSaveWindows[i]->update();
		if (done) {
			mChoseSaveTextShadow->show();
			for (int i = 0; i < 3; ++i) {
				if (unk40[i].unk0 == 1) {
#if defined(VERSION_GMSJ01)
					strncpy(unk2C0[i]->getStringPtr(), "こわれています", 20);
					strncpy(unk2CC[i]->getStringPtr(), "こわれています", 20);
#else
					setMessage(unk2C0[i], 20, 0);
					setMessage(unk2CC[i], 20, 0);
#endif
					unk2CC[i]->show();
					mSaveShineIconsUnder100[i]->hide();
				} else if (unk40[i].unk18 == 0) {
#if defined(VERSION_GMSJ01)
					strncpy(unk2C0[i]->getStringPtr(), "NEW", 20);
					strncpy(unk2CC[i]->getStringPtr(), "NEW", 20);
#else
					setMessage(unk2C0[i], 20, 0x1B);
					setMessage(unk2CC[i], 20, 0x1B);
#endif
					unk2CC[i]->show();
					mSaveShineIconsUnder100[i]->hide();
				} else {
					if (unk40[i].unk1C >= 120)
						unk32C[i]->show();

					unk2CC[i]->hide();

					u16 score = unk40[i].unk1C > 999 ? 999 : unk40[i].unk1C;
					if (score < 100) {
						mSaveShineCountTensUnder100[i]->changeTexture(
						    mDigitTextures[score / 10]->getTexInfo(), 0);
						mSaveShineCountOnesUnder100[i]->changeTexture(
						    mDigitTextures[score % 10]->getTexInfo(), 0);
						mSaveShineIconsUnder100[i]->show();
						mSaveShineIconsOver100[i]->hide();
					} else {
						mSaveShineCountHundredsOver100[i]->changeTexture(
						    mDigitTextures[score / 100]->getTexInfo(), 0);
						score -= int(score * 0.01f) * 100;
						mSaveShineCountTensOver100[i]->changeTexture(
						    mDigitTextures[score / 10]->getTexInfo(), 0);
						mSaveShineCountOnesOver100[i]->changeTexture(
						    mDigitTextures[score % 10]->getTexInfo(), 0);
						mSaveShineIconsOver100[i]->show();
						mSaveShineIconsUnder100[i]->hide();
					}
				}
			}
			unkB0       = -1;
			mWindowStep = WINDOW_SHOWN;
		}
	} break;

	case WINDOW_SHOWN:
		if (param_2 != PROGRESS_UNK0 && mGamePad->getTrigger() & 0x200)
			mWindowStep = WINDOW_CLOSE;
		if (unkB0 != -1)
			mWindowStep = WINDOW_CLOSE;
		break;

	case WINDOW_CLOSE:
		mGamePad->onFlag(TMarioGamePad::PAD_FLAG_MENU_INPUT);
		mChoseSaveTextShadow->hide();
		mChoseSaveWindow->setCenteredSize(
		    30, 0, 0, mChoseSaveWindowInitialBounds.getWidth(),
		    mChoseSaveWindowInitialBounds.getHeight());
		for (int i = 0; i < 3; ++i)
			unk32C[i]->hide();
		for (int i = 0; i < 3; ++i) {
			if (i != unkB0 || unk1C == PROGRESS_SELECT_COPY_DEST
			    || unk40[unkB0].unk0 == 1) {
				unk2CC[i]->hide();
				mSaveShineIconsUnder100[i]->hide();
				mSaveShineIconsOver100[i]->hide();
				mSaveWindows[i]->setCenteredSize(30, 0, 0, unk2B0.getWidth(),
				                                 unk2B0.getHeight());
			}
		}
		mWindowStep = WINDOW_CLOSING;
		break;

	case WINDOW_CLOSING: {
		bool done = true;
		done &= mChoseSaveWindow->update();
		for (int i = 0; i < 3; ++i)
			done &= mSaveWindows[i]->update();
		if (done) {
			mChoseSaveWindow->getPane()->hide();
			if (param_3) {
				for (int i = 0; i < 3; ++i)
					if (i != unkB0 || unk1C == PROGRESS_SELECT_COPY_DEST
					    || unk40[unkB0].unk0 == 1)
						mSaveWindows[i]->getPane()->hide();
			}
			mWindowStep = WINDOW_DONE;
		}
	} break;

	case WINDOW_DONE:
		if (unk1C == PROGRESS_SELECT_COPY_DEST)
			unk278[unkB1]->makeBlockNormal();
		result = unkB0;
		if (mScreenState == SCREEN_FILE_SELECT)
			mGamePad->offFlag(TMarioGamePad::PAD_FLAG_MENU_INPUT);
		if (unkB0 != -1)
			unk1C = param_1;
		else
			unk1C = param_2;
		break;
	}

	for (int i = 0; i < ARRAY_COUNT(unk32C); ++i)
		changePattern(unk32C[i], 72, unk338);

	if (unk338 < 144)
		unk338 += 1;
	else
		unk338 = 0;

	return result;
}

s8 TCardLoad::selectFunction()
{
	s8 result = -1;
	s8 bVar1  = unkB6;

	switch (mWindowStep) {
	case WINDOW_OPEN: {
		mSaveManageOptionsWindow[unkB0]->getPane()->show();
		for (int i = 0; i < 4; ++i)
			mSaveManageOptionsEntries[unkB0][i]->getPane()->hide();
		JUTRect rect = mSaveManageOptionsWindowInitialBounds[unkB0];
		mSaveManageOptionsWindow[unkB0]->setPaneSize(
		    30, rect.getWidth(), rect.getHeight(), rect.getWidth(), 0);
		mSaveManageOptionsWindow[unkB0]->setPaneOffset(30, 0, 0, 0,
		                                               rect.getHeight());

		if (!mSaveWindows[0]->getPane()->isVisible()) {
			for (int i = 0; i < 3; ++i)
				if (i == unkB0 && !mSaveWindows[i]->getPane()->isVisible()) {
					unk2CC[i]->hide();
					mSaveShineIconsUnder100[i]->hide();
					mSaveShineIconsOver100[i]->hide();
					mSaveWindows[i]->getPane()->show();
					mSaveWindows[i]->setCenteredSize(30, unk2B0.getWidth(),
					                                 unk2B0.getHeight(), 0, 0);
				}
		}

		mWindowStep = WINDOW_OPENING;
	} break;

	case WINDOW_OPENING: {
		bool done = true;
		for (int i = 0; i < ARRAY_COUNT(mSaveWindows); ++i)
			done &= mSaveWindows[i]->update();
		done &= mSaveManageOptionsWindow[unkB0]->update();
		if (done) {
			for (int i = 0; i < 4; ++i) {
				mSaveManageOptionsEntries[unkB0][i]->getPane()->show();

				mSaveManageOptionsEntries[unkB0][i]->updateCenteredSize(
				    1,
				    mSaveManageOptionsEntriesInitialBounds[unkB0][i].getWidth(),
				    mSaveManageOptionsEntriesInitialBounds[unkB0][i]
				        .getHeight());

				mSaveManageOptionsEntries[unkB0][i]->update();
				if (unk40[unkB0].unk18 == 0) {
					if (i > 0)
						((J2DPicture*)mSaveManageOptionsEntries[unkB0][i]
						     ->getPane())
						    ->setWhite(0xFFFFFF7F);
				} else {
					((J2DPicture*)mSaveManageOptionsEntries[unkB0][i]
					     ->getPane())
					    ->setWhite(0xFFFFFFFF);
				}
			}

			((J2DPicture*)mSaveManageOptionsEntries[unkB0][unkB6]->getPane())
			    ->setWhite(0x00FF00FF);
			for (int i = 0; i < 3; ++i) {
				if (unk40[i].unk0 == 1) {
#if defined(VERSION_GMSJ01)
					strncpy(unk2C0[i]->getStringPtr(), "こわれています", 20);
					strncpy(unk2CC[i]->getStringPtr(), "こわれています", 20);
#else
					setMessage(mConfirmationNewSaveText, 20, 0);
					setMessage(mConfirmationNewSaveTextShadow, 20, 0);
#endif
					unk2CC[i]->show();
					mSaveShineIconsUnder100[i]->hide();
				} else if (unk40[i].unk18 == 0) {
#if defined(VERSION_GMSJ01)
					strncpy(unk2C0[i]->getStringPtr(), "NEW", 20);
					strncpy(unk2CC[i]->getStringPtr(), "NEW", 20);
#else
					setMessage(mConfirmationNewSaveText, 20, 0x1B);
					setMessage(mConfirmationNewSaveTextShadow, 20, 0x1B);
#endif
					unk2CC[i]->show();
					mSaveShineIconsUnder100[i]->hide();
				} else {
					unk2CC[i]->hide();

					u16 score = unk40[i].unk1C;

					if (score < 100) {
						mSaveShineCountTensUnder100[i]->changeTexture(
						    mDigitTextures[score / 10]->getTexInfo(), 0);
						mSaveShineCountOnesUnder100[i]->changeTexture(
						    mDigitTextures[score % 10]->getTexInfo(), 0);
						mSaveShineIconsUnder100[i]->show();
					} else {
						mSaveShineIconsOver100[i]->show();
					}
				}
			}

			unkC4       = 0;
			mWindowStep = WINDOW_SHOWN;
		}
	} break;

	case WINDOW_SHOWN:
		if (mGamePad->checkFrameMeaning(TMarioGamePad::MEANING_MENU_A)) {
			if (bVar1 == 0)
				SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_DECIDE_COMMON, 0,
				                                   nullptr, 0);
			else
				SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_SELECT_COMMON, 0,
				                                   nullptr, 0);
			mWindowStep = WINDOW_CLOSE;
		} else if (mGamePad->checkFrameMeaning(
		               TMarioGamePad::MEANING_MENU_DOWN)) {
			TCardBookmarkInfo* bm = &unk40[unkB0];
			if (bm->unk18 == 0)
				unkB6 = 0;
			else
				unkB6 = bVar1 + 1;
			if (unkB6 > 3)
				unkB6 = 0;
		} else if (mGamePad->checkFrameMeaning(
		               TMarioGamePad::MEANING_MENU_UP)) {
			TCardBookmarkInfo* bm = &unk40[unkB0];
			if (bm->unk18 == 0)
				unkB6 = 0;
			else
				unkB6 = bVar1 - 1;
			if (unkB6 < 0)
				unkB6 = 3;
		} else if (mGamePad->checkFrameMeaning(TMarioGamePad::MEANING_MENU_B)) {
			unkB6 = -1;
			SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_CANCEL_COMMON, 0,
			                                   nullptr, 0);
			mWindowStep = WINDOW_CLOSE;
		}

		if (unkB6 != -1) {
			if (unkC4 == 4) {
				mSaveManageOptionsEntries[unkB0][unkB6]->setCenteredSize(
				    40,
				    mSaveManageOptionsEntriesInitialBounds[unkB0][unkB6]
				            .getWidth()
				        * 1.5f,
				    mSaveManageOptionsEntriesInitialBounds[unkB0][unkB6]
				            .getHeight()
				        * 1.5f,
				    mSaveManageOptionsEntriesInitialBounds[unkB0][unkB6]
				        .getWidth(),
				    mSaveManageOptionsEntriesInitialBounds[unkB0][unkB6]
				        .getHeight());

				JUTRect rect = mSaveManageOptionsEntries[unkB0][unkB6]
				                   ->getPane()
				                   ->getGlbBounds();
				JGeometry::TVec3<f32> pos(rect.x1 + rect.getWidth() / 2.0f,
				                          rect.y1 + rect.getHeight() / 2.0f,
				                          0.0f);
				gpEmitterManager4D2->createEmitter(pos, 0x1FA, nullptr,
				                                   nullptr);
				unkAC = gpEmitterManager4D2->unkC8[0][0];
				unkAC->setRotation(0, 0, DEG2SHORTANGLE(12));
				unkAC->setEmitterScale(JGeometry::TVec3<f32>(0.9f, 1.0f, 0.1f));
			} else if (unkC4 == 44) {
				mSaveManageOptionsEntries[unkB0][unkB6]->setCenteredSize(
				    40,
				    mSaveManageOptionsEntriesInitialBounds[unkB0][unkB6]
				        .getWidth(),
				    mSaveManageOptionsEntriesInitialBounds[unkB0][unkB6]
				        .getHeight(),
				    mSaveManageOptionsEntriesInitialBounds[unkB0][unkB6]
				            .getWidth()
				        * 1.5f,
				    mSaveManageOptionsEntriesInitialBounds[unkB0][unkB6]
				            .getHeight()
				        * 1.5f);
			}

			int r = unkC4;
			r     = r < 40 ? r : 80 - r;
			r     = (r * 255.0f) / 40.0f;
			((J2DPicture*)mSaveManageOptionsEntries[unkB0][unkB6]->getPane())
			    ->setWhite((r << 24) + 0xFF00FF);
			mSaveManageOptionsEntries[unkB0][unkB6]->update();
			unkC4 += 1;
			if (unkC4 > 80)
				unkC4 = 0;
		}

		if (unkB6 != bVar1 && unkB6 != -1) {
			if (unkAC != nullptr)
				gpEmitterManager4D2->forceDeleteEmitter(unkAC);
			SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_E3_MENU_CURSOR, 0,
			                                   nullptr, 0);

			unkC4 = 0;

			mSaveManageOptionsEntries[unkB0][bVar1]->getPane()->place(
			    mSaveManageOptionsEntriesInitialBounds[unkB0][bVar1]);

			((J2DPicture*)mSaveManageOptionsEntries[unkB0][bVar1]->getPane())
			    ->setWhite(0xFFFFFFFF);
			((J2DPicture*)mSaveManageOptionsEntries[unkB0][unkB6]->getPane())
			    ->setWhite(0x00FF00FF);
		}
		break;

	case WINDOW_CLOSE: {
		for (int i = 0; i < 4; ++i)
			mSaveManageOptionsEntries[unkB0][i]->getPane()->hide();
		JUTRect rect = mSaveManageOptionsWindowInitialBounds[unkB0];
		mSaveManageOptionsWindow[unkB0]->updatePaneSize(30, rect.getWidth(), 0);
		mSaveManageOptionsWindow[unkB0]->updatePaneOffset(30, 0,
		                                                  rect.getHeight());

		mWindowStep = WINDOW_CLOSING;
		for (int i = 0; i < 3; ++i) {
			if (i == unkB0 && unkB6 != -1 && unkB6 != 1) {
				unk2CC[i]->hide();
				mSaveShineIconsUnder100[i]->hide();
				mSaveShineIconsOver100[i]->hide();
				mSaveWindows[i]->setCenteredSize(30, 0, 0, unk2B0.getWidth(),
				                                 unk2B0.getHeight());
			}
		}
	} break;

	case WINDOW_CLOSING: {
		bool done = true;
		done &= mSaveManageOptionsWindow[unkB0]->update();
		for (int i = 0; i < ARRAY_COUNT(mSaveWindows); ++i)
			done &= mSaveWindows[i]->update();
		if (done) {
			mSaveManageOptionsWindow[unkB0]->getPane()->hide();
			for (int i = 0; i < 3; ++i)
				if (unkB0 == i && unkB6 != -1 && unkB6 != 1)
					mSaveWindows[i]->getPane()->hide();
			mWindowStep = WINDOW_DONE;
		}
	} break;

	case WINDOW_DONE:
		if (bVar1 != -1) {
			unk1C  = PROGRESS_UNK1B;
			result = bVar1;
		} else {
			unk1C = PROGRESS_SELECT_FILE;
		}
		break;
	}

	return result;
}

void TCardLoad::setSelected(u8 param_1)
{
	if (mScreenState == SCREEN_FILE_SELECT) {
		SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_COIN_APPEAR, 0, nullptr,
		                                   0);
		unkB0 = param_1;
		setupScoreScreen();
	}
}

void TCardLoad::changeScene()
{
	TEProgress prevUnk1C      = unk1C;
	WindowStep prevWindowStep = mWindowStep;
	switch (prevUnk1C) {
	case PROGRESS_UNK30:
		gpCardManager->readOptionBlock();
		unk1C = PROGRESS_UNK31;
		break;

	case PROGRESS_UNK31: {
		int rc = gpCardManager->getLastStatus();
		if (rc != CARD_RESULT_BUSY) {
			if (rc == CARD_RESULT_READY) {
				JSUMemoryInputStream stream;
				gpCardManager->getOptionReadStream(&stream);
				TFlagManager::getInstance()->loadOption(stream);
				unk1C = PROGRESS_UNK0;
			} else {
				unk1C = changeMode(rc);
			}
		} else {
			gpCardManager->probe();
		}
	} break;

	case PROGRESS_UNK0:
		mGamePad->onFlag(TMarioGamePad::PAD_FLAG_MENU_INPUT);
		gpCardManager->getBookmarkInfos(unk40);
		unk1C = PROGRESS_UNK2;
		break;

	case PROGRESS_UNK1:
		gpCardManager->unmount();
		break;

	case PROGRESS_UNK2: {
		int rc = gpCardManager->getLastStatus();
		if (rc != CARD_RESULT_BUSY) {
			if (rc == CARD_RESULT_READY) {
				mWindowStep = WINDOW_OPENING;
				unk1C       = PROGRESS_SELECT_FILE;
			} else {
				unk1C = changeMode(rc);
			}
		}
	} break;

	case PROGRESS_NO_CARD: {
		int rc = gpCardManager->getLastStatus();
		if (rc == CARD_RESULT_NOCARD) {
			waitForStart(PROGRESS_START_WITHOUT_SAVE_CONFIRM);
			gpCardManager->probe();
		} else {
			if (mWindowStep < WINDOW_CLOSE)
				mWindowStep = WINDOW_CLOSE;
			if (waitForStart(PROGRESS_UNK2) != -1)
				gpCardManager->getBookmarkInfos(unk40);
		}
	} break;

	case PROGRESS_WRONG_DEVICE:
	case PROGRESS_INCOMPATIBLE_CARD:
	case PROGRESS_INSUFFICIENT_SPACE:
	case PROGRESS_NO_FREE_ENTRY:
	case PROGRESS_NO_SAVE_FILE:
	case PROGRESS_IO_ERROR:
#if !defined(VERSION_GMSJ01)
	case PROGRESS_CONTINUE_WITHOUT_SAVING:
#endif
	{
		int rc = gpCardManager->getLastStatus();
		if (rc != CARD_RESULT_NOCARD) {
			waitForStart(PROGRESS_START_WITHOUT_SAVE_CONFIRM);
			gpCardManager->probe();
		} else {
			if (mWindowStep < WINDOW_CLOSE)
				mWindowStep = WINDOW_CLOSE;
			if (waitForStart(PROGRESS_UNK2) != -1)
				gpCardManager->getBookmarkInfos(unk40);
		}
	} break;

	case PROGRESS_BROKEN_CARD:
	case PROGRESS_WRONG_ENCODING: {
		int rc = gpCardManager->getLastStatus();
		if (rc == CARD_RESULT_READY) {
			waitForChoice(PROGRESS_FORMAT_CONFIRM, CARD_LOAD_FORMAT_CANCEL, 1);
			gpCardManager->probe();
		} else if (rc != CARD_RESULT_BUSY) {
			if (mWindowStep < WINDOW_CLOSE)
				mWindowStep = WINDOW_CLOSE;
			if (waitForChoice(PROGRESS_FORMAT_CONFIRM, CARD_LOAD_FORMAT_CANCEL,
			                  1)
			    != -1)
				unk1C = changeMode(rc);
		}
	} break;

	case PROGRESS_FORMAT_CONFIRM: {
		int rc = gpCardManager->getLastStatus();
		if (rc == CARD_RESULT_READY) {
			s8 choice = waitForChoice(PROGRESS_FORMATTING,
			                          CARD_LOAD_FORMAT_CANCEL, 1);
			if (choice == 0)
				gpCardManager->format();
			if (choice == -1)
				gpCardManager->probe();
		} else if (rc != CARD_RESULT_BUSY) {
			if (mWindowStep < WINDOW_CLOSE)
				mWindowStep = WINDOW_CLOSE;
			if (waitForChoice(PROGRESS_FORMATTING, CARD_LOAD_FORMAT_CANCEL, 1)
			    != -1)
				unk1C = changeMode(rc);
		}
	} break;

	case PROGRESS_FORMATTING: {
		int rc = gpCardManager->getLastStatus();
		if (rc != CARD_RESULT_BUSY) {
			if (rc == CARD_RESULT_READY) {
				if (mWindowStep == WINDOW_SHOWN)
					mWindowStep
					    = VERSION_SELECT(WINDOW_CLOSE, GMSJ01(WINDOW_CLOSING),
					                     GMSE01(WINDOW_CLOSING));
				drawMessage(PROGRESS_FORMAT_DONE);
				gpCardManager->probe();
				if (mWindowStep == WINDOW_DONE)
					SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_DECIDE, 0,
					                                   nullptr, 0);
			} else {
				if (mWindowStep == WINDOW_SHOWN)
					mWindowStep = WINDOW_CLOSE;
				drawMessage(PROGRESS_FORMAT_FAILED);
			}
		} else {
			gpCardManager->probe();
			drawMessage(PROGRESS_UNK0);
		}
	} break;

	case PROGRESS_FORMAT_DONE:
#if defined(VERSION_GMSJ01)
	case PROGRESS_FORMAT_FAILED:
#endif
		if (waitForAnyKey(PROGRESS_UNK2) != -1)
			gpCardManager->getBookmarkInfos(unk40);
		break;

#if !defined(VERSION_GMSJ01)
	case PROGRESS_FORMAT_FAILED:
		if (waitForAnyKey(PROGRESS_UNK2) != -1)
			unk1C = changeMode(gpCardManager->getLastStatus());
		break;
#endif

	case PROGRESS_START_WITHOUT_SAVE_CONFIRM: {
		s8 choice = waitForChoice(PROGRESS_UNK29, PROGRESS_UNK2, 0);
		if (choice == 1)
			gpCardManager->getBookmarkInfos(unk40);
		else if (choice == 0)
			TFlagManager::getInstance()->firstStart();
	} break;

	case PROGRESS_CREATE_FILE_CONFIRM: {
		int rc = gpCardManager->getLastStatus();
		if (rc == CARD_RESULT_READY) {
			s8 c = waitForChoice(PROGRESS_CREATING_FILE, PROGRESS_NO_SAVE_FILE,
			                     0);
			if (c == 0)
				gpCardManager->createFile();
			if (c == -1)
				gpCardManager->probe();
		} else if (rc != CARD_RESULT_BUSY) {
			if (mWindowStep < WINDOW_CLOSE)
				mWindowStep = WINDOW_CLOSE;
			if (waitForChoice(PROGRESS_CREATING_FILE, PROGRESS_WRONG_DEVICE, 0)
			    != -1)
				unk1C = changeMode(rc);
		}
	} break;

	case PROGRESS_CREATING_FILE: {
		int rc = gpCardManager->getLastStatus();
		if (rc != CARD_RESULT_BUSY) {
			if (rc == CARD_RESULT_READY) {
				if (mWindowStep == WINDOW_SHOWN)
					mWindowStep
					    = VERSION_SELECT(WINDOW_CLOSE, GMSJ01(WINDOW_CLOSING));
				drawMessage(PROGRESS_FILE_CREATED);
				gpCardManager->probe();
				if (mWindowStep == WINDOW_DONE)
					SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_DECIDE, 0,
					                                   nullptr, 0);
			} else {
				if (mWindowStep == WINDOW_SHOWN)
					mWindowStep
					    = VERSION_SELECT(WINDOW_CLOSE, GMSJ01(WINDOW_CLOSING));
				drawMessage(PROGRESS_CREATE_FILE_FAILED);
			}
		} else {
			gpCardManager->probe();
			drawMessage(PROGRESS_UNK0);
		}
	} break;

	case PROGRESS_FILE_CREATED:
#if defined(VERSION_GMSJ01)
	case PROGRESS_CREATE_FILE_FAILED:
#endif
		if (waitForAnyKey(PROGRESS_UNK2) != -1)
			gpCardManager->getBookmarkInfos(unk40);
		break;

#if !defined(VERSION_GMSJ01)
	case PROGRESS_CREATE_FILE_FAILED:
		if (waitForAnyKey(PROGRESS_UNK2) != -1)
			unk1C = changeMode(gpCardManager->getLastStatus());
		break;
#endif

	case PROGRESS_SELECT_FILE: {
		if (unk24 != 0)
			selectBookmark(PROGRESS_UNK2, changeMode(unk24), true);
		int rc = gpCardManager->probe();
		if (rc == CARD_RESULT_READY) {
			unkB6 = 0;
			s8 bm = selectBookmark(PROGRESS_UNK1B, PROGRESS_UNK0, true);
			if (bm != -1 && unk40[bm].unk0 == 1)
				unk1C = PROGRESS_UNK1E;
		} else if (rc != CARD_RESULT_BUSY) {
			unk24 = rc;
			if (mWindowStep == WINDOW_SHOWN)
				mWindowStep = WINDOW_CLOSE;
		}
	} break;

	case PROGRESS_SELECT_COPY_DEST: {
		int rc = gpCardManager->probe();
		if (rc == CARD_RESULT_READY) {
			s8 choice = selectBookmark(PROGRESS_UNK15, PROGRESS_UNK2, 1);
			if (choice != -1) {
				unkB2 = choice;

				TCardBookmarkInfo* bm = &unk40[unkB2];
				if (bm->unk0 == 1) {
					unk1C = PROGRESS_UNK22;
				}
#if !defined(VERSION_GMSJ01) && !defined(VERSION_GMSE01)
				else if (bm->unk18 == 0) {
					gpCardManager->readBlock(unkB1);
					unk1C = PROGRESS_COPY_READING;
				}
#endif
			}
			if (unk1C == PROGRESS_UNK2)
				gpCardManager->getBookmarkInfos(unk40);
		} else if (rc != CARD_RESULT_BUSY) {
			TEProgress next = changeMode(rc);
			selectBookmark(next, next, true);
			if (mWindowStep == WINDOW_SHOWN)
				mWindowStep = WINDOW_CLOSE;
		}
	} break;

	case PROGRESS_UNK1B: {
		int rc = gpCardManager->getLastStatus();
		if (rc == CARD_RESULT_READY) {
			s8 c = selectFunction();
			gpCardManager->probe();
			switch (c) {
			case 0: {
				SMSGetApplication()->setSaveFile(unkB0);
				TCardBookmarkInfo* bm = &unk40[unkB0];
				if (bm->unk18 == 0) {
					TFlagManager::getInstance()->firstStart();
					unk1C = PROGRESS_UNK29;
				} else {
					gpCardManager->readBlock(unkB0);
					unk1C = PROGRESS_UNK28;
				}
			} break;

			case 1:
				unkB1 = unkB0;
				unk1C = PROGRESS_SELECT_COPY_DEST;
				break;

			case 2:
				unk1C = PROGRESS_ERASE_CONFIRM;
				break;

			case 3:
				for (int i = 0; i < 3; ++i)
					if (i == unkB0)
						mScoreTableWhichSaveABC[i]->show();
					else
						mScoreTableWhichSaveABC[i]->hide();
				mScoreScreen->search('ROOT')->setAlpha(0);
				gpCameraOption->moveToUp();
				gpCardManager->readBlock(unkB0);
				mScreenState = SCREEN_ENTER_SCORE;
				unkB8        = false;
				break;
			}
		} else if (rc != CARD_RESULT_BUSY) {
			if (mSaveManageOptionsEntries[unkB0][0]->getPane()->isVisible()) {
				for (int i = 0; i < 4; ++i)
					mSaveManageOptionsEntries[unkB0][i]->getPane()->hide();
				JUTRect local_6c = mSaveManageOptionsWindowInitialBounds[unkB0];
				mSaveManageOptionsWindow[unkB0]->updatePaneSize(
				    30, local_6c.getWidth(), 0);
				mSaveManageOptionsWindow[unkB0]->updatePaneOffset(
				    30, 0, local_6c.getHeight());
				mWindowStep = WINDOW_CLOSING;
			}

			if (selectFunction() != -1)
				unk1C = changeMode(rc);
		}
	} break;

	case PROGRESS_UNK1E: {
		int rc = gpCardManager->getLastStatus();
		if (rc == CARD_RESULT_READY) {
			s8 choice = waitForChoiceBM(PROGRESS_ERASING, PROGRESS_UNK2, 1);
			gpCardManager->probe();
			if (choice == 0) {
				clearBookmark(unkB0);
			} else if (choice == 1) {
				gpCardManager->getBookmarkInfos(unk40);
				unk1C = PROGRESS_UNK2;
			}
		} else if (rc != CARD_RESULT_BUSY) {
			if (mWindowStep < WINDOW_CLOSE)
				mWindowStep = WINDOW_CLOSE;
			if (waitForChoiceBM(PROGRESS_UNK0, PROGRESS_UNK0, 1) != -1)
				unk1C = changeMode(rc);
		}
	} break;

	case PROGRESS_ERASING: {
		int rc = gpCardManager->getLastStatus();
		if (rc != CARD_RESULT_BUSY) {
			if (rc == CARD_RESULT_READY) {
				if (mWindowStep == WINDOW_SHOWN)
					mWindowStep = WINDOW_CLOSE;
				drawMessageBM(PROGRESS_ERASE_DONE);
				gpCardManager->probe();
				if (mWindowStep == WINDOW_DONE)
					SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_DECIDE, 0,
					                                   nullptr, 0);
			} else {
				if (mWindowStep == WINDOW_SHOWN)
					mWindowStep = WINDOW_CLOSE;
				drawMessageBM(PROGRESS_ERASE_FAILED);
			}
		} else {
			drawMessageBM(PROGRESS_UNK0);
		}
	} break;

	case PROGRESS_ERASE_DONE:
#if defined(VERSION_GMSJ01)
	case PROGRESS_ERASE_FAILED:
#endif
		if (waitForAnyKey(PROGRESS_UNK2) != -1)
			gpCardManager->getBookmarkInfos(unk40);
		break;

#if !defined(VERSION_GMSJ01)
	case PROGRESS_ERASE_FAILED:
		if (waitForAnyKeyBM(PROGRESS_UNK2) != -1)
			unk1C = changeMode(gpCardManager->getLastStatus());
		break;
#endif

	case PROGRESS_ERASE_CONFIRM: {
		int rc = gpCardManager->getLastStatus();
		if (rc == CARD_RESULT_READY) {
			s8 choice = waitForChoiceBM(PROGRESS_ERASING, PROGRESS_UNK2, 1);
			gpCardManager->probe();
			if (choice == 0) {
				clearBookmark(unkB0);
			} else if (choice == 1) {
				gpCardManager->getBookmarkInfos(unk40);
			}
		} else if (rc != CARD_RESULT_BUSY) {
			if (mWindowStep < WINDOW_CLOSE)
				mWindowStep = WINDOW_CLOSE;
			if (waitForChoiceBM(PROGRESS_UNK0, PROGRESS_UNK0, 0) != -1)
				unk1C = changeMode(rc);
		}
	} break;

	case PROGRESS_UNK22: {
		int rc = gpCardManager->getLastStatus();
		if (rc == CARD_RESULT_READY) {
			s8 choice
			    = waitForChoiceBM(PROGRESS_COPY_READING, PROGRESS_UNK2, 1);
			gpCardManager->probe();
			if (choice == 0)
				gpCardManager->readBlock(unkB1);
			else if (choice == 1)
				gpCardManager->getBookmarkInfos(unk40);
		} else if (rc != CARD_RESULT_BUSY) {
			if (mWindowStep < WINDOW_CLOSE)
				mWindowStep = WINDOW_CLOSE;
			if (waitForChoiceBM(PROGRESS_UNK0, PROGRESS_UNK0, 1) != -1)
				unk1C = changeMode(rc);
		}
	} break;

	case PROGRESS_COPY_READING: {
		int rc = gpCardManager->getLastStatus();
		if (rc != CARD_RESULT_BUSY) {
			if (rc == CARD_RESULT_READY) {
				drawMessageBM(PROGRESS_UNK0);
				gpCardManager->probe();
				if (mWindowStep == WINDOW_SHOWN) {
					gpCardManager->writeBlock(unkB2);
					unk1C     = PROGRESS_COPY_WRITING;
					prevUnk1C = PROGRESS_COPY_WRITING;
				}
			} else {
				if (mWindowStep == WINDOW_SHOWN)
					mWindowStep = WINDOW_CLOSE;
				drawMessageBM(PROGRESS_COPY_FAILED);
			}
		} else {
			drawMessageBM(PROGRESS_UNK0);
		}
	} break;

	case PROGRESS_COPY_WRITING: {
		int rc = gpCardManager->getLastStatus();
		if (rc != CARD_RESULT_BUSY) {
			if (rc == CARD_RESULT_READY) {
				if (mWindowStep == WINDOW_SHOWN)
					mWindowStep = WINDOW_CLOSE;
				drawMessageBM(PROGRESS_COPY_DONE);
				gpCardManager->probe();
				if (mWindowStep == WINDOW_DONE)
					SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_DECIDE, 0,
					                                   nullptr, 0);
			} else {
				if (mWindowStep == WINDOW_SHOWN)
					mWindowStep = WINDOW_CLOSE;
				drawMessageBM(PROGRESS_COPY_FAILED);
			}
		} else {
			drawMessageBM(PROGRESS_UNK0);
		}
	} break;

	case PROGRESS_UNK15: {
		int rc = gpCardManager->getLastStatus();
		if (rc == CARD_RESULT_READY) {
			s8 choice = waitForChoiceBM(PROGRESS_COPY_READING,
			                            PROGRESS_SELECT_FILE, 1);
			gpCardManager->probe();
			if (choice == 0)
				gpCardManager->readBlock(unkB1);
		} else if (rc != CARD_RESULT_BUSY) {
			if (mWindowStep < WINDOW_CLOSE)
				mWindowStep = WINDOW_CLOSE;
			if (waitForChoiceBM(PROGRESS_UNK0, PROGRESS_UNK0, 1) != -1)
				unk1C = changeMode(rc);
		}
	} break;

#if defined(VERSION_GMSJ01)
	case PROGRESS_COPY_FAILED:
#endif
	case PROGRESS_COPY_DONE: {
		int rc = gpCardManager->probe();
		if (rc == CARD_RESULT_READY) {
			if (waitForAnyKeyBM(PROGRESS_UNK2) != -1)
				gpCardManager->getBookmarkInfos(unk40);
		} else if (rc != CARD_RESULT_BUSY) {
			if (waitForAnyKeyBM(PROGRESS_UNK0) != -1)
				unk1C = changeMode(rc);
		}
	} break;

#if !defined(VERSION_GMSJ01)
	case PROGRESS_COPY_FAILED: {
		int rc = gpCardManager->getLastStatus();
		if (rc == CARD_RESULT_READY) {
			if (waitForAnyKeyBM(PROGRESS_UNK0) != -1)
				unk1C = changeMode(rc);
		} else if (rc != CARD_RESULT_BUSY) {
			if (waitForAnyKeyBM(PROGRESS_UNK0) != -1)
				unk1C = changeMode(rc);
		}
	} break;
#endif

	case PROGRESS_UNK28: {
		int rc = gpCardManager->getLastStatus();
		if (rc != CARD_RESULT_BUSY) {
			if (rc == CARD_RESULT_READY) {
				loadBookmark();
				unk1C = PROGRESS_UNK29;
			} else {
				unk1C = changeMode(rc);
			}
		}
	} break;

	case PROGRESS_UNK29: {
		if (TFlagManager::getInstance()->getFlag(MSF_SHINE_COUNT) < 1)
			SMSGetMarDirector()->setNextStage(0, nullptr);
		else
			SMSGetMarDirector()->setNextStage(1, nullptr);
		gpCardManager->unmount();
	} break;

	case PROGRESS_UNK32: {
		int rc = gpCardManager->getLastStatus();
		if (rc != CARD_RESULT_BUSY) {
			if (rc == CARD_RESULT_READY) {
				JSUMemoryOutputStream stream;
				gpCardManager->getOptionWriteStream(&stream);
				TFlagManager::getInstance()->saveOption(stream);
				gpCardManager->writeOptionBlock();
				unk1C = PROGRESS_UNK2F;
			} else {
				unk1C = changeMode(rc);
			}
		}
	} break;

	case PROGRESS_UNK2F: {
		int rc = gpCardManager->getLastStatus();
		if (rc != CARD_RESULT_BUSY) {
			if (rc == CARD_RESULT_READY) {
				if (mWindowStep == WINDOW_SHOWN)
					mWindowStep = WINDOW_CLOSE;
				drawMessage(PROGRESS_UNK2);
				if (mWindowStep == WINDOW_DONE) {
					SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_DECIDE, 0,
					                                   nullptr, 0);
					gpCardManager->getBookmarkInfos(unk40);
				}
			} else {
				drawMessage(PROGRESS_UNK0);
				if (mWindowStep == WINDOW_SHOWN)
					mWindowStep = WINDOW_CLOSE;
				if (mWindowStep == WINDOW_DONE)
					unk1C = changeMode(rc);
			}
		} else {
			drawMessage(PROGRESS_UNK0);
		}
	} break;
	}

	if (prevUnk1C != unk1C) {
		unk24       = 0;
		unk20       = unk1C;
		mWindowStep = WINDOW_OPEN;
#if !defined(VERSION_GMSJ01)
		unkB4 = 0;
#endif
		switch (prevUnk1C) {
		case PROGRESS_SELECT_COPY_DEST:
			unk284->offCollision();

		case PROGRESS_SELECT_FILE:
		case PROGRESS_NO_CARD:
		case PROGRESS_WRONG_DEVICE:
		case PROGRESS_INCOMPATIBLE_CARD:
		case PROGRESS_INSUFFICIENT_SPACE:
		case PROGRESS_NO_FREE_ENTRY:
		case PROGRESS_NO_SAVE_FILE:
		case PROGRESS_IO_ERROR:
#if !defined(VERSION_GMSJ01)
		case PROGRESS_CONTINUE_WITHOUT_SAVING:
#endif
			mGamePad->onFlag(TMarioGamePad::PAD_FLAG_MENU_INPUT);
			unk275 = 0;
			break;
		}

		switch (unk1C) {
		case PROGRESS_SELECT_COPY_DEST:
			unk284->onCollision();
			unk275 = 0;
			mGamePad->offFlag(TMarioGamePad::PAD_FLAG_MENU_INPUT);
			break;

		case PROGRESS_SELECT_FILE:
		case PROGRESS_NO_CARD:
		case PROGRESS_WRONG_DEVICE:
		case PROGRESS_INCOMPATIBLE_CARD:
		case PROGRESS_INSUFFICIENT_SPACE:
		case PROGRESS_NO_FREE_ENTRY:
		case PROGRESS_NO_SAVE_FILE:
		case PROGRESS_IO_ERROR:
#if !defined(VERSION_GMSJ01)
		case PROGRESS_CONTINUE_WITHOUT_SAVING:
#endif
			unk275 = 1;
			mGamePad->offFlag(TMarioGamePad::PAD_FLAG_MENU_INPUT);
			break;
		}

		if (unk1C == PROGRESS_SELECT_FILE) {
			for (int i = 0; i < 3; ++i)
				unk278[i]->makeBlockNormal();
		}
	}

	if (prevWindowStep != mWindowStep) {
		if (prevWindowStep == WINDOW_OPEN)
			SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_2D_IN, 0, nullptr, 0);
		if (mWindowStep == WINDOW_CLOSING)
			SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_2D_OUT, 0, nullptr, 0);
	}
}
