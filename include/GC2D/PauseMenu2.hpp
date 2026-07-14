#ifndef GC2D_PAUSE_MENU_2_HPP
#define GC2D_PAUSE_MENU_2_HPP

#include <JSystem/J2D/J2DScreen.hpp>
#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>

class J2DPicture;
class J2DSetScreen;
class J2DTextBox;
class J2DPane;
class TMarioGamePad;
class JPABaseEmitter;
class TCardSave;

class TPauseMenu2 : public JDrama::TViewObj {
public:
	TPauseMenu2(const char* = "<TPauseMenu>");
	void load(JSUMemoryInputStream&);
	void loadAfter();
	void appearWindow();
	void disappearWindow();
	void perform(u32 cue, JDrama::TGraphics* graphics);
	u8 getNextState();
	void setDrawStart();
	void setDrawEnd();
	void drawAppearPane(J2DPicture* picture, f32 anim, JUTRect& rect,
	                    f32 rotation);

	// fabricated; smells fake
	void setEmitterScale(f32 x, f32 y, f32 z)
	{
		mEmitter->setScale2(JGeometry::TVec3<f32>(x, y, z));
	}

	// fabricated
	inline void draw(JDrama::TGraphics* gfx);

public:
	enum PauseMenuState {
		MENU_APPEARING    = 0,
		MENU_OPEN         = 1,
		UNK2              = 2,
		MENU_SAVING       = 3,
		MENU_DISAPPEARING = 4,
		MENU_CLOSED       = 5
	};

	// Menu state.
	/* 0x10 */ PauseMenuState mState;

	// Screen and menu/background panes.
	/* 0x14 */ J2DSetScreen* mScreen;
	/* 0x18 */ J2DPane* mBackground;
	/* 0x1C */ J2DPane* mMenuPane;

	// "Pause" letters.
	/* 0x20 */ J2DPicture* mPauseLetters[5];
	/* 0x34 */ JUTRect mOrigLetterBounds[5];
	/* 0x84 */ f32 mOrigLetterAngles[5];

	// Menu items.
	/* 0x98 */ J2DPicture* mMenuItems[3];
	/* 0xA4 */ JUTRect mOrigItemBounds[3];

	// Stage and scenario name.
	/* 0xD4 */ J2DTextBox* mStageName;
	/* 0xD8 */ J2DPane* mStagePane;
	/* 0xDC */ J2DTextBox* mScenarioName;

	// Menu option items.
	/* 0xE0 */ u8 mSelectedItem;
	/* 0xE4 */ u32 mItemColor;
	/* 0xE8 */ f32 mBounceAnim;

	// Appear/disappear animation.
	/* 0xEC */ f32 mFadeAnim;
	/* 0xF0 */ u8 mBackgroundAlpha;
	/* 0xF4 */ f32 mBackgroundFadeInSpeed;
	/* 0xF8 */ s16 mFirstItemAngle;
	/* 0xFC */ u32 unkFC; // unused?

	// "Spark" effect for the bouncing animation.
	/* 0x100 */ s16 mEffectKeyFrame;
	/* 0x102 */ u16 mEffectStretch;

	/* 0x104 */ s32 mNumItems;
	/* 0x108 */ bool mPressedB;
	/* 0x109 */ bool mSelectionConfirmed;
	/* 0x10C */ TMarioGamePad* mGamePad;
	/* 0x110 */ JPABaseEmitter* mEmitter;
	/* 0x114 */ u32 unk114; // unused?
	/* 0x118 */ TCardSave* mCardSave;
};

#endif
