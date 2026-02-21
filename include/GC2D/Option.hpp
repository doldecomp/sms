#ifndef GC2D_OPTION_HPP
#define GC2D_OPTION_HPP

#include <dolphin/types.h>
#include <JSystem/J2D/J2DPane.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/JUtility/JUTRect.hpp>
#include <System/ArrayWrapper.hpp>

class J2DPane;
class J2DScreen;
class J2DOrthoGraph;
class J2DSetScreen;
class TExPane;
class JAISound;

/**
 * @brief A control that makes a pane's left side
 * stretch horizontally in a loop.
 */
class TArrowControl {
public:
	TArrowControl(J2DPane* pane)
	    : mPane(pane)
	    , unk14(true)
	    , mPhase(0)
	{
		mBounds = mPane->getBounds();
	}

	void update();
	void updateAlpha();
	void updateScale();
	int calcMoveX(int phase) const;

public:
	/* 0x0 */ J2DPane* mPane;
	/* 0x4 */ JUTRect mBounds;
	/* 0x14 */ bool unk14;
	/* 0x18 */ int mPhase;
};

class TBalloonControl {
public:
	TBalloonControl(int size);

	void add(TExPane* pane);
	void setupAnm();
	void startAnm();
	void stopAnm();
	void update();

public:
	struct UnknownBalloonControlStruct {
		/* 0x0 */ TExPane* unk0;
		/* 0x4 */ u8 unk4;
	};

	/* 0x0 */ UnknownBalloonControlStruct* unk0;
	/* 0x4 */ int unk4;
	/* 0x8 */ int unk8;
	/* 0xC */ J3DFrameCtrl mFrameCtrl;
	/* 0x20 */ u32 unk20;
};

/**
 * @brief Pulsating control for a pane.
 * The given pane gets smaller/larger relative to it's center in a loop.
 */
class TPaneScalingControl {
public:
	TPaneScalingControl(J2DPane* pane);

	void setupAnm(f32 amplitude, f32 speed);
	void startAnm();
	void stopAnm();
	void update();

public:
	/* 0x0 */ J2DPane* mPane;
	/* 0x4 */ J3DFrameCtrl mFrameCtrl;
	/* 0x18 */ JUTRect mInitialBounds;
	/* 0x28 */ f32 mAmplitude;
};

/**
 * @brief Animation sequence control for a set of panes with durations.
 * Loops through the given set of TAnmChunks, showing each pane for the
 * specified duration and then hiding it before moving to showing the next.
 * Intended use is to have multiple panes in the exact same position with
 * different key frames of an animation. No interpolation occurs.
 */
class TPatternAnmControl {
public:
	struct TAnmChunk {
		/* 0x0 */ u32 mTag;
		/* 0x4 */ f32 mDuration;
	};

	TPatternAnmControl(J2DScreen* screen);

	void set(const TAnmChunk* chunks, int num_chunks);
	void setupAnm();
	void update();
	void show();
	void hide();

	bool checkCompletedOnce() const
	{
		return mFrameCtrl.checkState(J3DFrameCtrl::STATE_LOOPED_ONCE);
	}

	u32 getCurrentPaneTag() const { return mCurrentChunk->mTag; }

public:
	/* 0x0 */ J2DScreen* mScreen;
	/* 0x4 */ J3DFrameCtrl mFrameCtrl;
	/* 0x18 */ ArrayWrapper<const TAnmChunk> mChunks;
	/* 0x20 */ const TAnmChunk* mCurrentChunk;
	/* 0x24 */ f32 mNextTriggerFrame;
};

/**
 * @brief A toggle control for switching between alternative panes.
 * The intended use is to have multiple panes in the same position and toggle
 * between them, like a yes/no select text. Only one pane is shown at a time.
 */
class TToggleControl {
public:
	TToggleControl(J2DScreen* screen);

	void setupToggle(const u32* tags, int num_tags);
	void toggle();
	s32 getNumber() const;
	void setNumber(int num);

public:
	/* 0x0 */ J2DScreen* mScreen;
	/* 0x4 */ ArrayWrapper<const u32> mItems;
	/* 0xC */ const u32* mCurItem;
};

class TOptionRumbleUnit {
public:
	enum RumbleType {
		RUMBLE_TYPE_UNK0 = 0,
		RUMBLE_TYPE_UNK1 = 1,
	};
	enum State {
		STATE_ACTIVE       = 0,
		STATE_DEACTIVATING = 1,
		STATE_INACTIVE     = 2,
	};

	TOptionRumbleUnit(J2DScreen* screen);

	void update();
	void checkRumble();
	void toggle();
	void adjust();
	void adjustView();
	void show();
	void hide();
	void deactivate(bool force);
	void activate();
	void setValue(RumbleType value);
	RumbleType getValue() const
	{
		return (RumbleType)mSelectionText->getNumber();
	}
	void setState(State state);
	void setInfluencedAlphaRecursive(J2DPane* pane, bool influenced_alpha);

	TPatternAnmControl* getCurrentGamepadAnm() const
	{
		return mGamepadIcon[mSelectionText->getNumber()];
	}

public:
	/* 0x0 */ J2DScreen* mScreen;
	/* 0x4 */ TExPane* mParentPane;
	/* 0x8 */ u8 mInitialAlpha;
	/* 0xC */ TBalloonControl* unkC;
	/* 0x10 */ TPaneScalingControl* mSelectionBubble;
	/* 0x14 */ TPatternAnmControl* mGamepadIcon[2];
	/* 0x1C */ TToggleControl* mSelectionText;
	/* 0x20 */ State mState;
	/* 0x24 */ bool mShouldRumble;
};

class TOptionSoundUnit {
public:
	enum SoundType {
		SOUND_TYPE_MONO     = 0,
		SOUND_TYPE_STEREO   = 1,
		SOUND_TYPE_SURROUND = 2,
	};
	enum State {
		STATE_ACTIVE       = 0,
		STATE_DEACTIVATING = 1,
		STATE_INACTIVE     = 2,
	};

	TOptionSoundUnit(J2DScreen* screen);

	void initMonoAnm();
	void initSteleoAnm();
	void initSurroundAnm();
	void update();
	void updatePatternAnm();
	void foreachPatternAnm(ArrayWrapper<TPatternAnmControl*>& ary,
	                       void (TPatternAnmControl::*ptmf)());
	void toggle();
	void adjust();
	void show();
	void hide();
	void deactivate(bool force);
	void activate();
	void setValue(int value);
	int getValue() const;
	void stopSound();
	static SoundType flagToType(int flag);
	static int typeToFlag(SoundType type);
	void setState(State state);
	void adjustView();
	void adjustSound();
	void setInfluencedAlphaRecursive(J2DPane* pane, bool influenced_alpha);

	struct FabricatedSoundSettings {
		/* 0x0 */ u32 mSoundSystemSE;
		/* 0x4 */ u32 mOutputMode;
	};
	const static FabricatedSoundSettings cSoundSettings[];

	struct FabricatedFlagInfo {
		/* 0x0 */ SoundType mSoundType;
		/* 0x4 */ int mFlag;
	};
	const static FabricatedFlagInfo cFlagInfos[];

public:
	/* 0x0 */ J2DScreen* mScreen;
	/* 0x4 */ TExPane* mParentPane;
	/* 0x8 */ u8 mInitialAlpha;
	/* 0xC */ TBalloonControl* unkC;
	/* 0x10 */ TPaneScalingControl* mSelectionBubble;
	/* 0x14 */ TToggleControl* mSelectionText;
	/* 0x18 */ State mState;
	/* 0x1C */ J3DFrameCtrl mMusicFrameCtrl;
	/* 0x30 */ JAISound* mMusic;
	/* 0x34 */ TPatternAnmControl* mMonoAnimations[2];
	/* 0x3C */ TPatternAnmControl* mStereoAnimations[3];
	/* 0x48 */ TPatternAnmControl* mSurroundAnimations[5];
	/* 0x5C */ ArrayWrapper<TPatternAnmControl*> mMonteIcons[3];
};

class TOptionControl {
public:
	enum SelectType {
		SELECT_TYPE_RUMBLE_OPTION = 0,
		SELECT_TYPE_SOUND_OPTION  = 1,
	};

	void load();
	void loadSetting();
	void movementCommon();
	void draw(J2DOrthoGraph* graph);
	bool movementCard2Option();
	bool movementOption();
	bool movementOption2Card();
	void setType(TOptionControl::SelectType type, bool initial_options_entry);
	void toggleCurType();
	void checkInput();
	void writeValue();
	bool isChangedSetting() const;
	void resetChangedSetting();

public:
	/* 0x0 */ J2DSetScreen* mScreen;
	/* 0x4 */ TArrowControl* mBackArrow;
	/* 0x8 */ TOptionRumbleUnit* mRumbleOption;
	/* 0xC */ TOptionSoundUnit* mSoundOption;
	/* 0x10 */ SelectType mSelectedOption;
	/* 0x14 */ bool mWasJumping;
	/* 0x18 */ int mInitialRumbleValue;
	/* 0x1C */ int mInitialSoundValue;
};

#endif
