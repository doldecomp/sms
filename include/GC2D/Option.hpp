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

// NOTE: the GMSJ01 reconstruction this file grew from had a `TBalloonControl`
// member at +0xC in both option units, driving the 'bub0'..'bub4' panes.
// GMSE01 has no trace of it: the map lists no TBalloonControl symbol in
// Option.cpp, `new TOptionRumbleUnit` asks for 0x24 (not 0x28) bytes, and both
// constructors go straight from the parent pane to the selection bubble.
// See git history for the removed class if a Japanese build is ever added.

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
	// The map instantiates ArrayWrapper<Ul>, not ArrayWrapper<const Ul>:
	// see the weak begin__Q29@unnamed@16ArrayWrapper<Ul>CFv in Option.cpp.
	// TODO: the cast in setupToggle disappears if ArrayWrapper<T>::set() took
	// a `const T*` (open change in System/ArrayWrapper.hpp).
	/* 0x4 */ ArrayWrapper<u32> mItems;
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
	/* 0xC */ TPaneScalingControl* mSelectionBubble;
	/* 0x10 */ TPatternAnmControl* mGamepadIcon[2];
	/* 0x18 */ TToggleControl* mSelectionText;
	/* 0x1C */ State mState;
	/* 0x20 */ bool mShouldRumble;
};

/**
 * @brief The subtitle on/off option, US only.
 * The Japanese release has no subtitle option, so this whole class is
 * GMSE01-specific; the map lists it in Option.cpp between the sound and the
 * rumble unit.
 */
class TOptionSubtitleUnit {
public:
	enum SubtitleType {
		SUBTITLE_TYPE_UNK0 = 0,
		SUBTITLE_TYPE_UNK1 = 1,
	};
	enum State {
		STATE_ACTIVE       = 0,
		STATE_DEACTIVATING = 1,
		STATE_INACTIVE     = 2,
	};

	TOptionSubtitleUnit(J2DScreen* screen);

	void update();
	void toggle();
	void adjust();
	void show();
	void hide();
	void deactivate(bool force);
	void activate();
	void setValue(SubtitleType value);
	SubtitleType getValue() const
	{
		return (SubtitleType)mSelectionText->getNumber();
	}
	void setState(State state);
	void setInfluencedAlphaRecursive(J2DPane* pane, bool influenced_alpha);

public:
	// fabricated: header round 20 accessor candidates
	TExPane* getParentPane() const { return mParentPane; }

	/* 0x0 */ J2DScreen* mScreen;
	/* 0x4 */ TExPane* mParentPane;
	/* 0x8 */ u8 mInitialAlpha;
	/* 0xC */ TPaneScalingControl* mSelectionBubble;
	/* 0x10 */ TToggleControl* mSelectionText;
	/* 0x14 */ State mState;
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
	/* 0xC */ TPaneScalingControl* mSelectionBubble;
	/* 0x10 */ TToggleControl* mSelectionText;
	/* 0x14 */ State mState;
	/* 0x18 */ J3DFrameCtrl mMusicFrameCtrl;
	/* 0x2C */ JAISound* mMusic;
	/* 0x30 */ TPatternAnmControl* mMonoAnimations[2];
	/* 0x38 */ TPatternAnmControl* mStereoAnimations[3];
	/* 0x44 */ TPatternAnmControl* mSurroundAnimations[5];
	/* 0x58 */ ArrayWrapper<TPatternAnmControl*> mMonteIcons[3];
};

class TOptionControl {
public:
	enum SelectType {
		SELECT_TYPE_RUMBLE_OPTION   = 0,
		SELECT_TYPE_SOUND_OPTION    = 1,
		SELECT_TYPE_SUBTITLE_OPTION = 2,
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
	// fabricated: header round 20 accessor candidates
	TOptionRumbleUnit* getRumbleOption() const { return mRumbleOption; }
	TOptionSoundUnit* getSoundOption() const { return mSoundOption; }
	TOptionSubtitleUnit* getSubtitleOption() const { return mSubtitleOption; }

	/* 0x0 */ J2DSetScreen* mScreen;
	/* 0x4 */ TArrowControl* mBackArrow;
	/* 0x8 */ TOptionRumbleUnit* mRumbleOption;
	/* 0xC */ TOptionSoundUnit* mSoundOption;
	/* 0x10 */ TOptionSubtitleUnit* mSubtitleOption;
	/* 0x14 */ SelectType mSelectedOption;
	/* 0x18 */ bool mWasJumping;
	/* 0x19 */ bool mStickNeutral;
	/* 0x1C */ int mInitialRumbleValue;
	/* 0x20 */ int mInitialSoundValue;
	/* 0x24 */ int mInitialSubtitleValue;
};

#endif
