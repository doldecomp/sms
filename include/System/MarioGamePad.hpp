#ifndef SYSTEM_MARIO_GAME_PAD_HPP
#define SYSTEM_MARIO_GAME_PAD_HPP

#include <Dolphin/types.h>
#include <JSystem/JUtility/JUTGamePad.hpp>
#include <Camera/cameralib.hpp>
#include <Camera/Camera.hpp>

#define VARIANTS 5

struct TMarioControllerWork {
	enum Buttons {
		R = 0x20,
		A = 0x100,
		B = 0x200,
		L = 0x4000,
	};

	/* 0x00 */ s16 mStickHS16;
	/* 0x02 */ s16 mStickVS16;
	/* 0x04 */ Buttons mInput;
	/* 0x08 */ Buttons mFrameInput;
	/* 0x0C */ u8 mAnalogRU8;
	/* 0x0D */ u8 mAnalogLU8;
	/* 0x10 */ f32 mStickH;
	/* 0x14 */ f32 mStickV;
	/* 0x18 */ f32 mStickDist;
	/* 0x1C */ f32 mAnalogL;
	/* 0x20 */ f32 mAnalogR;
};

class TMarioGamePad : public JUTGamePad {
public:
	TMarioGamePad(EPadPort port)
	    : JUTGamePad(port)
	{
	}
	virtual ~TMarioGamePad();

	enum PadMeanings {
		MEANING_0x1      = 0x1,
		MEANING_0x2      = 0x2,
		MEANING_0x4      = 0x4,
		MEANING_0x8      = 0x8,
		MEANING_0x10     = 0x10,
		MEANING_0x20     = 0x20,
		MEANING_0x40     = 0x40,
		MEANING_0x80     = 0x80,
		MEANING_0x100    = 0x100,
		MEANING_0x200    = 0x200, // Unused?
		MEANING_0x400    = 0x400,
		MEANING_0x800    = 0x800,
		MEANING_0x1000   = 0x1000,
		MEANING_0x2000   = 0x2000,
		MEANING_0x4000   = 0x4000,
		MEANING_0x8000   = 0x8000,
		MEANING_0x10000  = 0x10000,
		MEANING_0x20000  = 0x20000,
		MEANING_0x40000  = 0x40000,
		MEANING_0x80000  = 0x80000,
		MEANING_0x100000 = 0x100000,
		MEANING_0x200000 = 0x200000,
	};
	enum PadFlags {
		PAD_FLAG_0x1  = 0x1,
		PAD_FLAG_0x2  = 0x2,
		PAD_FLAG_0x4  = 0x4,
		PAD_FLAG_0x8  = 0x8,
		PAD_FLAG_0x10 = 0x10,
		PAD_FLAG_0x80 = 0x80,
	};

	struct StickPos {
		f32 x;
		f32 y;
	};

	// Fabricated
	inline bool checkReset(s32* resetPort)
	{
		if (resetPort != 0) {
			*resetPort = JUTGamePad::C3ButtonReset::sResetOccurredPort;
		}
		return JUTGamePad::C3ButtonReset::sResetOccurred;
	}

	// Fabricated
	inline void handleReset(s32 resetPort)
	{
		if (resetPort == JUTGamePad::EPortInvalid) {
			mResetFlag |= 0xf;
		} else {
			mResetFlag |= (u16)(1 << resetPort);
		}
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
	inline void resetStick(StickPos& stick)
	{
		stick.x = 0.0f;
		stick.y = 0.0f;
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
			if (mButton.mAnalogRf > 0.5f) {
				_DC |= 1;
			}
		} else if (mButton.mAnalogRf > 0.25f) {
			_DC |= 1;
		}

		_DE = _DC & ~dc;
		_E0 = dc & ~_DC;

		for (int i = 0; i < VARIANTS; i++) {
			resetStick(mCompSPos[i]);
		}

		u32 prevMeaning = mMeaning;
		mMeaning        = 0;
		return prevMeaning;
	}

	// Fabricated
	inline bool checkFlag(u32 flag) { return (mFlags & flag) != 0; }
	void onFlag(u32 flag) { mFlags |= flag; }
	void offFlag(u32 flag) { mFlags &= ~flag; }

	u32 read();
	void onNeutralMarioKey();
	void reset();
	void updateMeaning();

	/* 0xA8 */ StickPos mCompSPos[VARIANTS];
	/* 0xD0 */ u32 mMeaning;
	/* 0xD4 */ u32 mEnabledFrameMeaning;
	/* 0xD8 */ u32 mDisabledFrameMeaning;
	/* 0xDC */ u16 _DC;
	/* 0xDE */ u16 _DE;
	/* 0xE0 */ u16 _E0;

	/* 0xE2 */ u16 mFlags;

	/* 0xE4 */ s16 _E4;
	/* 0xE6 */ u16 _E6; // padding?
	/* 0xE8 */ s32 mDisabledFrames;
	/* 0xEC */ u32 _EC; // padding?

	static u16 mResetFlag;
};

#endif
