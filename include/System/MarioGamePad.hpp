#ifndef SYSTEM_MARIO_GAME_PAD_HPP
#define SYSTEM_MARIO_GAME_PAD_HPP

#include <dolphin/types.h>
#include <JSystem/JUtility/JUTGamePad.hpp>
#include <Camera/cameralib.hpp>
#include <Camera/Camera.hpp>

#define VARIANTS 5

struct TMarioControllerWork {
	enum Buttons {
		UNK10 = 0x10,
		R     = 0x20,
		A     = 0x100,
		B     = 0x200,
		L     = 0x4000,
	};

	/* 0x00 */ s16 mStickHS16;
	/* 0x02 */ s16 mStickVS16;
	/* 0x04 */ u32 mInput;
	/* 0x08 */ u32 mFrameInput;
	/* 0x0C */ u8 mAnalogRU8;
	/* 0x0D */ u8 mAnalogLU8;
	/* 0x10 */ f32 mStickH;
	/* 0x14 */ f32 mStickV;
	/* 0x18 */ f32 mStickDist;
	/* 0x1C */ f32 mAnalogR;
	/* 0x20 */ f32 mAnalogL;

	bool isAHit() const
	{
		if (mFrameInput & A) {
			return true;
		}
		return false;
	}
	bool isBHit() const
	{
		if (mFrameInput & B) {
			return true;
		}
		return false;
	}
	bool isAPressed() const
	{
		if (mInput & A) {
			return true;
		}
		return false;
	}
	bool isBPressed() const
	{
		if (mInput & B) {
			return true;
		}
		return false;
	}
};

class TMarioGamePad : public JUTGamePad {
public:
	TMarioGamePad(EPadPort port)
	    : JUTGamePad(port)
	    , mMeaning(0)
	    , mEnabledFrameMeaning(0)
	    , mDisabledFrameMeaning(0)
	    , _DC(0)
	    , _DE(0)
	    , _E0(0)
	    , mFlags(0)
	{
		reset();
	}

	enum PadMeanings {
		MEANING_START       = 0x1,
		MEANING_MENU_UP     = 0x2,
		MEANING_MENU_DOWN   = 0x4,
		MEANING_MENU_LEFT   = 0x8,
		MEANING_MENU_RIGHT  = 0x10,
		MEANING_MENU_A      = 0x20,
		MEANING_MENU_B      = 0x40,
		MEANING_A           = 0x80,
		MEANING_B           = 0x100,
		MEANING_UNK200      = 0x200, // Unused?
		MEANING_R           = 0x400,
		MEANING_TALK_B      = 0x800,
		MEANING_Z           = 0x1000,
		MEANING_L           = 0x2000,
		MEANING_Y           = 0x4000,
		MEANING_CAM_L       = 0x8000,
		MEANING_CAM_AB      = 0x10000,
		MEANING_SELECT_A    = 0x20000,
		MEANING_SELECT_B    = 0x40000,
		MEANING_SELECT_UP   = 0x80000,
		MEANING_SELECT_DOWN = 0x100000,
		MEANING_X           = 0x200000,
	};
	enum TType {
		TTYPE_0 = 0,
	};

	enum PadFlags {
		PAD_FLAG_MENU_INPUT  = 0x1,
		PAD_FLAG_GAME_INPUT  = 0x2,
		PAD_FLAG_TALK_NPC    = 0x4,
		PAD_FLAG_TALK_SELECT = 0x8,
		PAD_FLAG_NO_INPUT    = 0x10,
		PAD_FLAG_NO_B        = 0x20,
		PAD_FLAG_0x40        = 0x40,
		PAD_FLAG_GUIDE_INPUT = 0x80,
	};

	// Fabricated
	static inline bool checkReset(s32* resetPort)
	{
		if (resetPort != nullptr)
			*resetPort = JUTGamePad::C3ButtonReset::sResetOccurredPort;
		return JUTGamePad::C3ButtonReset::sResetOccurred;
	}

	// Fabricated
	static inline void handleReset(s32 resetPort)
	{
		if (resetPort == JUTGamePad::EPortInvalid)
			mResetFlag.on(0xf);
		else
			mResetFlag.on(1 << resetPort);
		JUTGamePad::C3ButtonReset::sResetOccurred = false;
	}

	// Fabricated
	inline void updateMeaning(EButtons button, PadMeanings padButton,
	                          u32 prevMeaning)
	{
		if ((mButton.mTrigger & button) != 0
		    || ((mButton.mButton & button) != 0
		        && ((prevMeaning & padButton) != 0))) {
			mMeaning |= padButton;
		}
	}

	// Fabricated
	inline u32 resetMeaning()
	{
		u16 dc = _DC;
		_DC    = 0;

		if (mButton.mButton & (DPAD_UP | DPAD_DOWN | DPAD_LEFT | DPAD_RIGHT)) {
			_DC |= 1;
		}

		if ((dc & 1) != 0) {
			if (mButton.mAnalogRf > 0.25f) {
				_DC |= 1;
			}
		} else if (mButton.mAnalogRf > 0.5f) {
			_DC |= 1;
		}

		_DE = _DC & ~dc;
		_E0 = dc & ~_DC;

		for (int i = 0; i < 10; i++)
			mCompSPos[i] = 0.0f;

		u32 prevMeaning = mMeaning;
		mMeaning        = 0;
		return prevMeaning;
	}

	// Fabricated
	inline bool checkFlag(u32 flag) { return (mFlags & flag) != 0; }
	void onFlag(u32 flag) { mFlags |= flag; }
	void offFlag(u32 flag) { mFlags &= ~flag; }

	// fabricated
	bool isSomethingPushed() const { return mResetFlag.check(1 << mPortNum); }

	static void read();
	void rumble(TType type, u32 length);
	void keepRumble(TType type);
	void onNeutralMarioKey();
	void considerMarioStick(f32* stick);
	void reset();
	void updateMeaning();

	// fabricated
	bool checkMeaning(u32 meaning) const { return mMeaning & meaning; }

	// fabricated
	bool checkFrameMeaning(u32 meaning) const
	{
		return mEnabledFrameMeaning & meaning;
	}

	// fabricated
	f32 getMainStickInDir(f32 x, f32 y) const
	{
		return mMainStick.mPosX * x + mMainStick.mPosY * y;
	}

	void invalidate(s32 frames) { mDisabledFrames = frames; }

public:
	// NOTE: surprisingly, only flat array matches
	/* 0xA8 */ f32 mCompSPos[2 * VARIANTS];
	/* 0xD0 */ u32 mMeaning;
	/* 0xD4 */ u32 mEnabledFrameMeaning;
	/* 0xD8 */ u32 mDisabledFrameMeaning;
	/* 0xDC */ u16 _DC;
	/* 0xDE */ u16 _DE;
	/* 0xE0 */ u16 _E0;
	/* 0xE2 */ u16 mFlags;
	/* 0xE4 */ s16 mNeutralKeyTimer;
	/* 0xE8 */ s32 mDisabledFrames;
	/* 0xEC */ u32 _EC;

	static JDrama::TFlagT<u16> mResetFlag;
};

#endif
