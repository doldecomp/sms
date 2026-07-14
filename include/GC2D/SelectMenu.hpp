#ifndef GC2D_SELECT_MENU_2_HPP
#define GC2D_SELECT_MENU_2_HPP

#include <GC2D/BoundPane.hpp>
#include <JSystem/J2D/J2DScreen.hpp>
#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/JUtility/JUTRect.hpp>

class J2DPane;
class J2DPicture;
class J2DSetScreen;
class J2DTextBox;
class JKRArchive;
class JUTTexture;
class TBoundPane;
class TExPane;
class TMarioGamePad;
class TSelectDir;
class TSelectShineManager;

class TSelectGrad : public JDrama::TViewObj {
public:
	TSelectGrad(const char* pName);

	void setStageColor(u8 param1);

	void perform(u32 flags, JDrama::TGraphics* gfx);

	/* 0x10 */ s32 mRgbAnimCycle[3];
	/* 0x1C */ JUtility::TColor mTopLeftCol;
	/* 0x20 */ JUtility::TColor mBottomRightCol;
};

class TSelectMenu : public JDrama::TViewObj {
public:
	TSelectMenu(const char* pName);
	void initData(u8 stage, JKRArchive* pArch, TSelectShineManager* pShineMgr,
	              TSelectDir* pSelectDir);
	void startMove();
	void perform(u32 flags, JDrama::TGraphics* gfx);

	s8 getNextIndex();
	s8 getPrevIndex();
	void startOpenWindow();

public:
	enum SelectMenuState {
		// Close the menu.
		CLOSE_MENU = 0,

		// Animate letterbox bars from top and bottom of screen.
		LETTERBOX_ANIMATION = 1,

		// Slide the stage banner in from the right of the screen.
		STAGE_BANNER_SLIDE = 2,

		// Squash the stage banner when it hits the left side.
		STAGE_BANNER_SQUASH = 3,

		// Stretch the banner after squash.
		STAGE_BANNER_STRETCH = 4,

		// Fade the menu items in.
		APPEAR_MENU = 5,

		// Await and process player input.
		MENU_INPUT_LOOP = 6,

		// Animate the menu elements.
		MENU_ANIM_LOOP = 7,

		// Fade the menu elements out.
		DISAPPEAR_MENU = 8,

		// Wait for a moment before closing the menu.
		WAIT_BEFORE_CLOSE = 9,
	};

	/* 0x10 */ SelectMenuState mMenuState;

	// Unused; look like this has been moved to TSelectGrad.
	/* 0x14 */ s32 unk14[3];

	/* 0x20 */ J2DSetScreen* mMenuScreen;

	// Leterbox bars at the top and bottom of the menu.
	/* 0x24 */ TExPane* mLetterBoxTop;
	/* 0x28 */ TExPane* mLetterBoxBottom;

	// Text box that contains the stage name.
	/* 0x2C */ J2DTextBox* mStageName;

	// Pane that contains the stage banner image.
	// (Uses English text in the Japanese version.)
	/* 0x30 */ TExPane* mStageBannerPane;
	/* 0x34 */ TBoundPane* mStageBannerText;
	/* 0x38 */ TBoundPane* mStageBannerShadow;

	/* 0x3C */ s32 unk3C;

	// Pane that contains the scenario (shine) name and a banner image.
	/* 0x40 */ TExPane* mScenarioPane1;
	/* 0x44 */ J2DTextBox* mScenarioText1;
	/* 0x48 */ J2DPicture* mScenarioImg1;
	/* 0x4C */ J2DPicture* mScenarioShadow1;

	/* 0x50 */ J2DPane* unk50;
	/* 0x54 */ bool mSelectNext;
	/* 0x58 */ JUTRect unk58;

	// Second scenario pane for animating between adjacent scenarios.
	/* 0x68 */ TExPane* mScenarioPane2;
	/* 0x6C */ J2DTextBox* mScenarioText2;
	/* 0x70 */ J2DPicture* mScenarioImg2;
	/* 0x74 */ J2DPicture* mScenarioShadow2;

	/* 0x78 */ J2DPicture* unk78;
	/* 0x7C */ s16 mScenarioPaneDist;

	// Textures for the scenario (shine) banners.
	// (E.g. "Story 1" in the Japanese version.)
	/* 0x80 */ JUTTexture* mScenarioTex[8];

	// Shine selection menu pane.
	/* 0xA0 */ J2DPane* mShineList;

	// Pane that displays the collected coins (score).
	/* 0xA4 */ J2DPane* mScorePane;
	/* 0xA8 */ JUTTexture* mCoinNumTex[10]; // coin letter 0-9

	// Shine selection menu using small shine icons at the bottom of the screen.
	/* 0xD0 */ JUTTexture* mShineGotMark;
	/* 0xD4 */ JUTTexture* mShineUnlockedMark;
	/* 0xD8 */ bool mMarkPulseDir;
	/* 0xDC */ J2DPicture* mShineMarks[8];

	/* 0xFC */ s32 unkFC;
	/* 0x100 */ TMarioGamePad* mGamePad;

	// Arrows next to the shine icons.
	/* 0x104 */ J2DPane* mArrowL;
	/* 0x108 */ J2DPane* mArrowR;
	/* 0x10C */ bool mArrowAnimDir;
	/* 0x10D */ u8 mArrowAnimPos;
	/* 0x110 */ JUTRect mArrowLBounds;
	/* 0x120 */ JUTRect mArrowRBounds;

	/* 0x130 */ TSelectShineManager* mSelectShineMgr;
	/* 0x134 */ TSelectDir* mSelectDir;

	// Animation frame counters.
	/* 0x138 */ u8 mLetterboxAnimFrame;
	/* 0x139 */ u8 mSelectShineAnimFrame;

	// Current stage and unlocked shines.
	/* 0x13A */ u8 mStage;
	/* 0x13B */ u8 mSelectedShine;
	/* 0x13C */ u8 mNumUnlockedShines;
	/* 0x13D */ u8 unk13D;
	/* 0x13E */ u8 unk13E;
	/* 0x13F */ u8 unk13F;

	// Colours for the shine icons.
	/* 0x140 */ JUtility::TColor mSelectedMarkCol;
	/* 0x144 */ JUtility::TColor mMarkCol;
	/* 0x148 */ u8 mSelectedMarkAlpha;
	/* 0x149 */ u8 mMarkAlpha;

	/* 0x14A */ bool mCloseMenu;
	/* 0x14B */ u8 unk14B;
	/* 0x14C */ f32 mRcpAnmFrameRate;

	// Tracks which shines are unlocked:
	// 0 - locked
	// 1 - unused?
	// 2 - unlocked
	// 3 - got
	/* 0x150 */ u8 mShineUnlockStates[8];

	/* 0x158 */ void* mScenarioBmg;
	/* 0x15C */ void* mScenarioBmg2;

	// Unused; look like this has been moved to TSelectGrad.
	/* 0x160 */ JUtility::TColor unk160;
	/* 0x164 */ JUtility::TColor unk164;

	/* 0x168 */ s16 unk168;
	/* 0x16A */ s16 unk16A;

	/* 0x16C */ s16 mWaitBeforeCloseTimer;
};

#endif
