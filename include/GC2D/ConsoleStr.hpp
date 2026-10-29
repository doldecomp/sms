#ifndef GC2D_CONSOLE_STR_HPP
#define GC2D_CONSOLE_STR_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/JUtility/JUTPoint.hpp>

class J2DSetScreen;
class TExPane;
class J2DTextBox;
class TBoundPane;
class JPABaseEmitter;

class TConsoleStr : public JDrama::TViewObj {
public:
	TConsoleStr(const char* name = "<ConsoleStr>");
	f32 getWipeCloseTime();
	void load(JSUMemoryInputStream&);
	void loadAfter();
	void perform(u32 cue, JDrama::TGraphics* graphics);
	void startAppearReady();
	void startAppearGo();
	void startAppearShineGet();
	void startAppearMiss();
	void startAppearScenario();
	bool processReady(int);
	bool processGo(float);
	bool processShineGet(int);
	bool processMiss(int);
	bool processScenario(int);
	void startCloseWipe(bool);
	void startOpenWipe();

	// TODO: wrong types
	static JUTPoint cShineGetRight1;
	static JUTPoint cShineGetLeft1;
	static JUTPoint cShineGetRight2;
	static JUTPoint cShineGetLeft2;
	static JUTPoint cShineGetRight3;
	static JUTPoint cShineGetLeft3;

	enum {
		DISPLAY_NONE          = 0,
		DISPLAY_GO            = 1,
		DISPLAY_SHINE_GET     = 2,
		DISPLAY_MISS          = 3,
		DISPLAY_SCENARIO_DEMO = 4,
		DISPLAY_READY_GO      = 5,
	};

	enum {
		SCENARIO_DEMO_TITLE_ENTERING             = 0x0,
		SCENARIO_DEMO_TITLE_VISIBLE              = 0x1,
		SCENARIO_DEMO_TITLE_CLOSING              = 0x2,
		SCENARIO_DEMO_TITLE_CLOSED_WAIT          = 0x3,
		SCENARIO_DEMO_TITLE_CLOSED               = 0x4,
		SCENARIO_DEMO_WIPER_OPENING              = 0x5,
		SCENARIO_DEMO_WIPER_OPENED               = 0x6,
		SCENARIO_DEMO_IDLE                       = 0x7,
		SCENARIO_DEMO_TITLE_CLOSING_WITHOUT_TEXT = 0x8,
	};

public:
#if defined(VERSION_GMSP01) || defined(VERSION_GMSP01_NX)
	enum {
		GO_LETTERS_CAPACITY    = 11,
		SHINE_LETTERS_CAPACITY = 11,
		MISS_LETTERS_CAPACITY  = 12,
	};

	/* 0x010 */ s32 mGoLettersLength;
	/* 0x014 */ s32 mShineLettersLength;
	/* 0x018 */ s32 mMissLettersLength;
	/* 0x01C */ s32 mMissInitialRotations[MISS_LETTERS_CAPACITY];
#elif defined(VERSION_GMSE01)
	enum {
		GO_LETTERS_CAPACITY    = 3,
		SHINE_LETTERS_CAPACITY = 6,
		MISS_LETTERS_CAPACITY  = 7,
	};
#else
	enum {
		GO_LETTERS_CAPACITY    = 3,
		SHINE_LETTERS_CAPACITY = 9,
		MISS_LETTERS_CAPACITY  = 5,
	};
#endif
	/* 0x10 */ J2DSetScreen* mSplashTextScreen;
	/* 0x14 */ J2DSetScreen* mScenarioDemoScreen;
	/* 0x18 */ f32 mAnimationTimer;
	/* 0x1C */ int mTitleFadeCounter;
	/* 0x20 */ s32 mShineGetDelay;
	/* 0x24 */ u32 unk24;
	/* 0x28 */ TBoundPane* mGoLetters[GO_LETTERS_CAPACITY];
	/* 0x34 */ JUTPoint mGoLetterPositionHistory[GO_LETTERS_CAPACITY][22];
	/* 0x244 */ TBoundPane* mShineGetLetters[SHINE_LETTERS_CAPACITY];
	/* 0x268 */ TBoundPane* mMissLetters[MISS_LETTERS_CAPACITY];
	/* 0x27C */ TExPane* mReadyLetters[5];
	/* 0x290 */ TExPane* mHeaderFooterMasks[2];
	/* 0x298 */ TExPane* mLeftRightWipers[2];
	/* 0x2A0 */ J2DTextBox* mMapStoryTitles[2];
	/* 0x2A8 */ bool mCloseWipeRequested;
	/* 0x2A9 */ bool mParticlesStopped;
	/* 0x2AC */ JPABaseEmitter* mGoSplashEmitters[GO_LETTERS_CAPACITY];
	/* 0x2B8 */ int mDisplayState;
	/* 0x2BC */ int mScenarioDemoState;
};

#endif
