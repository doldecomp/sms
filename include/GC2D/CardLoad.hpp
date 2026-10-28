#ifndef GC2D_CARD_LOAD_HPP
#define GC2D_CARD_LOAD_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>
#include <System/CardManager.hpp>
#include <GC2D/Progress.hpp>
#include <GC2D/ExPane.hpp>
#include <macros.h>
#include <version.h>

#if defined(VERSION_GMSP01_NX)
#define CARD_LOAD_GAME_LOGO_NUM mGameLogoLetterNum
#else
#define CARD_LOAD_GAME_LOGO_NUM ARRAY_COUNT(mGameLogoLetters)
#endif

class J2DPane;
class J2DPicture;
class J2DTextBox;
class J2DSetScreen;
class J2DScreen;
class J2DWindow;
class JPABaseEmitter;
class TCardLoad;
class TExPane;
class JUTTexture;
class TFileLoadBlock;
class TOptionControl;
class TMapObjOptionWall;
class TMarioGamePad;

extern TCardLoad* gpCardLoad;

#define CARD_LOAD_PRESS_START_LETTERS                                          \
	VERSION_SELECT(11, GMSP01(15), GMSP01_NX(15))
#define CARD_LOAD_GAME_LOGO_LETTERS                                            \
	VERSION_SELECT(18, GMSJ01(13), GMSJ01_01(13))

class TCardLoad : public JDrama::TViewObj {
public:
	enum ScreenState {
		SCREEN_FILE_SELECT   = 0,
		SCREEN_SCORE         = 1,
		SCREEN_OPTIONS       = 2,
		SCREEN_TITLE         = 3,
		SCREEN_ENTER_SCORE   = 4,
		SCREEN_EXIT_SCORE    = 5,
		SCREEN_ENTER_OPTIONS = 6,
		SCREEN_EXIT_OPTIONS  = 7,
		SCREEN_EXIT_TITLE    = 8,
		SCREEN_ENTER_TITLE   = 9,
		SCREEN_INIT_TITLE    = 10,
	};

	enum WindowStep {
		WINDOW_OPEN    = 0,
		WINDOW_OPENING = 1,
		WINDOW_SHOWN   = 2,
		WINDOW_CLOSE   = 3,
		WINDOW_CLOSING = 4,
		WINDOW_DONE    = 5,
	};

	TCardLoad(const char* name = "<TCardLoad>");

	virtual void load(JSUMemoryInputStream& stream);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	void changePattern(J2DPicture*, s16, u32);
	void resetScoreScreenObjects();
	void changeTitleTexture(int);
	void loadLangTexture();
	void loadTextureFile(JUTTexture**, const char*);
	void replaceTexture(u32, JUTTexture*, J2DScreen*);
	void replaceTexture_fixCenter(u32, JUTTexture*, J2DScreen*);
	void changeLanguage(int);
	void setupTitleScreen();
	void setupScoreScreen();
	void loadAfter();
	bool titleDraw();
	void makeBuffer(J2DTextBox*, int);
	TEProgress changeMode(s32);
	void setMessage(J2DTextBox*, s32, int);
	void resizeBox(J2DWindow*, J2DTextBox*, J2DTextBox*);
	s8 waitForChoice(TEProgress, TEProgress, int);
	s8 waitForChoiceBM(TEProgress, TEProgress, int);
	s8 waitForAnyKey(TEProgress);
	s8 waitForAnyKeyBM(TEProgress);
	s8 waitForStart(TEProgress);
	s8 drawMessage(TEProgress);
	s8 drawMessageBM(TEProgress);
	s8 selectBookmark(TEProgress, TEProgress, bool);
	s8 selectFunction();
	void setSelected(u8);
	void changeScene();

	// fabricated
	int updateToSettingsArrowAlpha()
	{
		int alpha = mToSettingsArrow->getAlpha();
		if (unk275 && alpha < 255) {
			alpha += 8;
			if (alpha > 255)
				alpha = 255;
			mToSettingsArrow->setAlpha(alpha);
		}

		if (!unk275 && alpha > 0) {
			alpha -= 8;
			if (alpha < 0)
				alpha = 0;
			mToSettingsArrow->setAlpha(alpha);
		}
		return alpha;
	}

	// fabricated
	int calcToSettingsArrowMoveX(u8 phase) const
	{
		f32 fVar1 = (phase < 50 ? phase : 100 - phase) / 50.0f;
		f32 fVar2 = 1.0f - fVar1;
		return fVar1 * 8.0f * fVar1 + (fVar2 * -8.0f * fVar2 + fVar1 * fVar2);
	}

	// fabricated
	void updateToSettingsArrowScale()
	{
		s16 move = calcToSettingsArrowMoveX(unk274);
		mToSettingsArrow->place(JUTRect(mToSettingsArrowInitialBounds.x1,
		                                mToSettingsArrowInitialBounds.y1,
		                                mToSettingsArrowInitialBounds.x2 + move,
		                                mToSettingsArrowInitialBounds.y2));
		unk274 += 1;
		if (unk274 > 100)
			unk274 = 0;
	}

	// fabricated
	void hideGameLogoLetters()
	{
		for (int i = 0; i < CARD_LOAD_GAME_LOGO_NUM; ++i)
			mGameLogoLetters[i]->getPane()->setAlpha(0);
	}

	// fabricated
	void updateEnterScore();

	static const char* cToOptionFileName;
	static const char* cBMarkNewFileName;
	static const char* cWindowYesFileName;
	static const char* cWindowNoFileName;
	static const char* cScoreTotalFileName;
	static const char* cBMarkMenuFileName[];
	static const char* cScoreStageFileName[];
	static u32 cMessageID[];

public:
	/* 0x10 */ WindowStep mWindowStep;
	/* 0x14 */ ScreenState mScreenState;
	/* 0x18 */ int unk18;
	/* 0x1C */ TEProgress unk1C;
	/* 0x20 */ u32 unk20;
	/* 0x24 */ int unk24;
	/* 0x28 */ J2DSetScreen* mLoadScreen;
	/* 0x2C */ J2DSetScreen* mScoreScreen;
	/* 0x30 */ u32 unk30;
	/* 0x34 */ J2DSetScreen* mTitleScreen;
	/* 0x38 */ TMarioGamePad* mGamePad;
	/* 0x3C */ char unk3C[4];
	/* 0x40 */ TCardBookmarkInfo unk40[3];
	/* 0xA0 */ void* unkA0;
	/* 0xA4 */ u32 unkA4;
	/* 0xA8 */ f32 unkA8;
	/* 0xAC */ JPABaseEmitter* unkAC;
	/* 0xB0 */ s8 unkB0;
	/* 0xB1 */ s8 unkB1;
	/* 0xB2 */ s8 unkB2;
	/* 0xB4 */ s16 unkB4;
	/* 0xB6 */ s8 unkB6;
	/* 0xB7 */ s8 unkB7;
	/* 0xB8 */ bool unkB8;
	/* 0xBC */ int unkBC;
	/* 0xC0 */ int unkC0;
	/* 0xC4 */ int unkC4;
	/* 0xC8 */ JUTTexture* mDigitTextures[10];

	// Title screen elements
	/* 0xF0 */ TExPane* mGameLogoPicture;
	/* 0xF4 */ TExPane* mNintendoLogoPicture;
	/* 0xF8 */ TExPane* mPressStartTextLetters[CARD_LOAD_PRESS_START_LETTERS];
	/* 0x124 */ JUTRect
	    mPressStartTextLettersInitialBounds[CARD_LOAD_PRESS_START_LETTERS];
	/* 0x1D4 */ TExPane* mGameLogoLetters[CARD_LOAD_GAME_LOGO_LETTERS];
	/* 0x208 */ J2DPane* mTitleScreenRoot;

	/* 0x20C */ u16 mPressStartAnimTimes[CARD_LOAD_PRESS_START_LETTERS];
	/* 0x222 */ u8 mPressStartAnimState[CARD_LOAD_PRESS_START_LETTERS];
	/* 0x22E */ u16 mGameLogoAnimTimes[CARD_LOAD_GAME_LOGO_LETTERS];
	/* 0x248 */ u8 mGameLogoAnimState[CARD_LOAD_GAME_LOGO_LETTERS];
	/* 0x256 */ u16 unk256;
	/* 0x258 */ u16 mTitleAnimTimer;
	/* 0x25C */ J2DPane* mToSettingsArrow;
	/* 0x260 */ JUTRect mToSettingsArrowInitialBounds;
	/* 0x270 */ J2DPane* mToSettingsText;
	/* 0x274 */ u8 unk274;
	/* 0x275 */ u8 unk275;
	/* 0x278 */ TFileLoadBlock* unk278[3];
	/* 0x284 */ TMapObjOptionWall* unk284;

	// Header window above the 3 saves
	/* 0x288 */ TExPane* mChoseSaveWindow;
	/* 0x28C */ JUTRect mChoseSaveWindowInitialBounds;
	/* 0x29C */ J2DTextBox* mChoseSaveText;
	/* 0x2A0 */ J2DTextBox* mChoseSaveTextShadow;

	// The 3 saves, "A", "B", and "C"
	/* 0x2A4 */ TExPane* mSaveWindows[3];
	/* 0x2B0 */ JUTRect unk2B0;
	/* 0x2C0 */ J2DTextBox* unk2C0[3];
	/* 0x2CC */ J2DTextBox* unk2CC[3];
	/* 0x2D8 */ J2DPane* mSaveShineIconsUnder100[3];
	/* 0x2E4 */ J2DPicture* mSaveShineCountTensUnder100[3];
	/* 0x2F0 */ J2DPicture* mSaveShineCountOnesUnder100[3];
	/* 0x2FC */ J2DPane* mSaveShineIconsOver100[3];
	/* 0x308 */ J2DPicture* mSaveShineCountHundredsOver100[3];
	/* 0x314 */ J2DPicture* mSaveShineCountTensOver100[3];
	/* 0x320 */ J2DPicture* mSaveShineCountOnesOver100[3];
	/* 0x32C */ J2DPicture* unk32C[3];
	/* 0x338 */ int unk338;

	// Management options for the 3 saves: start, copy, erase, score.
	/* 0x33C */ TExPane* mSaveManageOptionsWindow[3];
	/* 0x348 */ JUTRect mSaveManageOptionsWindowInitialBounds[3];
	/* 0x378 */ TExPane* mSaveManageOptionsEntries[3][4];
	/* 0x3A8 */ JUTRect mSaveManageOptionsEntriesInitialBounds[3][4];

	/* 0x468 */ TExPane* unk468;
	/* 0x46C */ JUTRect unk46C;
	/* 0x47C */ J2DTextBox* unk47C;
	/* 0x480 */ J2DTextBox* unk480;
	/* 0x484 */ TExPane* unk484[2];
	/* 0x48C */ JUTRect unk48C[2];

	// Confirmation window for copying/erasing saves
	/* 0x4AC */ TExPane* mConfirmationWindow;
	/* 0x4B0 */ JUTRect mConfirmationWindowInitialBounds;
	/* 0x4C0 */ J2DTextBox* mConfirmationText;
	/* 0x4C4 */ J2DTextBox* mConfirmationTextShadow;
	/* 0x4C8 */ J2DPane* mConfirmationTextMario;
	/* 0x4CC */ J2DPane* mConfirmationTextTargetABC[3];
	/* 0x4D8 */ TExPane* mConfirmationYesNo[2];
	/* 0x4E0 */ JUTRect mConfirmationYesNoInitialBounds[2];
	/* 0x500 */ J2DPane* mConfirmationShineCountOver100;
	/* 0x504 */ J2DPicture* mConfirmationShineCountDigitsOver100[3];
	/* 0x510 */ J2DPane* mConfirmationShineCountUnder100;
	/* 0x514 */ J2DPicture* mConfirmationShineCountDigitsUnder100[2];
	/* 0x51C */ J2DTextBox* mConfirmationNewSaveText;
	/* 0x520 */ J2DTextBox* mConfirmationNewSaveTextShadow;

	// Card error stuff
	/* 0x524 */ TExPane* unk524;
	/* 0x528 */ JUTRect unk528;
	/* 0x538 */ J2DTextBox* unk538;
	/* 0x53C */ J2DTextBox* unk53C;
	/* 0x540 */ J2DPane* unk540[3];
	/* 0x54C */ TExPane* unk54C;
	/* 0x550 */ JUTRect unk550;
	/* 0x560 */ J2DTextBox* unk560;
	/* 0x564 */ J2DTextBox* unk564;

	// Window shown after a save is erased
	/* 0x568 */ TExPane* mErasedWindow;
	/* 0x56C */ JUTRect mErasedWindowInitialBounds;
	/* 0x57C */ J2DTextBox* mErasedText;
	/* 0x580 */ J2DTextBox* mErasedTextShadow;

	struct ScoreTableStageRow {
		/* 0x0 */ J2DPicture* mRoot;
		/* 0x4 */ J2DPicture* mCoinCountDigits[3];
		/* 0x10 */ J2DPane* mShineIcons[8];
		/* 0x30 */ J2DPane* unk30[2];
		/* 0x38 */ J2DPane* unk38;
	};

	// Scoreboard with per-stage shine and coin counts, and total shine count
	/* 0x584 */ ScoreTableStageRow mScoreTableStageRows[7];
	/* 0x728 */ J2DPane* mScoreTableWhichSaveABC[3];
	/* 0x734 */ char unk734[0x740 - 0x734];
	/* 0x740 */ J2DPane* mScoreTableHeader;
	/* 0x744 */ J2DPane* mScoreTotal;
	/* 0x748 */ J2DPicture* mScoreTotalShineCountDigit1;
	/* 0x74C */ J2DPicture* mScoreTotalShineCountDigit2;
	/* 0x750 */ J2DPicture* mScoreTotalShineCountDigit3;
	/* 0x754 */ TOptionControl* unk754;
#if defined(VERSION_GMSP01)
	/* 0x7D4 */ int mPressStartLetterNum;
	/* 0x7D8 */ void* mLangMessageData[5];
	/* 0x7EC */ J2DSetScreen* mLangScoreScreens[5];
	/* 0x800 */ JUTTexture* mToOptionTextures[5];
	/* 0x814 */ JUTTexture* mBMarkMenuTextures[4][5];
	/* 0x864 */ JUTTexture* mBMarkNewTextures[5];
	/* 0x878 */ JUTTexture* mWindowYesTextures[5];
	/* 0x88C */ JUTTexture* mWindowNoTextures[5];
	/* 0x8A0 */ JUTTexture* mScoreStageTextures[9][5];
	/* 0x954 */ JUTTexture* mScoreTotalTextures[5];
#elif defined(VERSION_GMSP01_NX)
	/* 0x7D4 */ int mPressStartLetterNum;
	/* 0x7D8 */ int mGameLogoLetterNum;
	/* 0x7DC */ u32 unk7DC;
#endif
};

#endif
