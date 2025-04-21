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

	s16 mStickHS16;
	s16 mStickVS16;
	Buttons mInput;
	Buttons mFrameInput;
	u8 mAnalogRU8;
	u8 mAnalogLU8;
	f32 mStickH;
	f32 mStickV;
	f32 mStickDist;
	f32 mAnalogL;
	f32 mAnalogR;
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
		u16 dc    = this->_DC;
		this->_DC = 0;

		if (this->mButton.mButton
		    & (DPAD_UP | DPAD_DOWN | DPAD_LEFT | DPAD_RIGHT)) {
			this->_DC |= 1;
		}

		if ((dc & 1) != 0) {
			if (this->mButton.mAnalogRf > 0.5f) {
				this->_DC |= 1;
			}
		} else if (this->mButton.mAnalogRf > 0.25f) {
			this->_DC |= 1;
		}

		this->_DE = this->_DC & ~dc;
		this->_E0 = dc & ~this->_DC;

		for (int i = 0; i < VARIANTS; i++) {
			resetStick(this->mCompSPos[i]);
		}

		u32 prevMeaning = this->mMeaning;
		this->mMeaning  = 0;
		return prevMeaning;
	}

	// Fabricated
	inline void updateMeaningLCamera(u32 prevMeaning)
	{
		// This is for when in y camera
		this->mCompSPos[1].x = (f32)this->mButton.mAnalogL;
		this->mCompSPos[1].y = (f32)this->mButton.mAnalogR;

		this->updateMeaning(R, MEANING_0x400, prevMeaning);
		this->updateMeaning(Z, MEANING_0x1000, prevMeaning);
		this->updateMeaning(L, MEANING_0x2000, prevMeaning);
		this->mCompSPos[2].x = this->mMainStick.mPosX;
		this->mCompSPos[2].y = this->mMainStick.mPosY;
		this->updateMeaning(A, MEANING_0x10000, prevMeaning);
		this->updateMeaning(B, MEANING_0x10000, prevMeaning);
		this->updateMeaning(Y, MEANING_0x4000, prevMeaning);

		if (((this->_DE & 0x1) != 0)
		    || (((this->_DC & 1) != 0
		         && ((prevMeaning & MEANING_0x200) != 0)))) {
			this->mMeaning |= MEANING_0x200;
		}

		this->updateMeaning(X, MEANING_0x200000, prevMeaning);
	}

	// Fabricated
	inline void updateMeaningRegular2(u32 prevMeaning)
	{
		f32 stickScaling = 176.0f;
		bool _unk3       = false;
		if (0 < this->_E4) {
			this->_E4 -= 1;
		}

		if (0 < this->_E4) {
			s16 _unk2 = 0x3d - this->_E4;
			_unk3     = true;
			if (_unk2 <= 0x28) {
				stickScaling = 0.0f;
			} else {
				// TODO: CLBCalcRatio
				// stickScaling =
				stickScaling = CLBCalcRatio<s16>(0x28, 0x3c, _unk2);
			}
		}

		if (_unk3) {
			this->mCompSPos[0].x = stickScaling * this->mMainStick.mPosX;
			this->mCompSPos[0].y = stickScaling * this->mMainStick.mPosY;
		} else {
			this->mCompSPos[0].x = this->mMainStick.mPosX;
			this->mCompSPos[0].y = this->mMainStick.mPosY;
		}

		this->mCompSPos[1].x = (f32)this->mButton.mAnalogL;
		this->mCompSPos[1].y = (f32)this->mButton.mAnalogR;

		this->updateMeaning(A, MEANING_0x80, prevMeaning);

		if ((this->mFlags & 0x4) != 0) {
			this->updateMeaning(B, MEANING_0x800, prevMeaning);
		} else {
			if ((this->mFlags & 0x20) == 0) {
				this->updateMeaning(B, MEANING_0x100, prevMeaning);
			}
		}

		this->updateMeaning(R, MEANING_0x400, prevMeaning);
		this->updateMeaning(Z, MEANING_0x1000, prevMeaning);
		this->updateMeaning(L, MEANING_0x2000, prevMeaning);

		this->mCompSPos[3].x = this->mSubStick.mPosX;
		this->mCompSPos[3].y = this->mSubStick.mPosY;

		this->updateMeaning(Y, MEANING_0x4000, prevMeaning);
		this->updateMeaning(L, MEANING_0x8000, prevMeaning);

		if (((this->_DE & 0x1) != 0)
		    || (((this->_DC & 1) != 0
		         && ((prevMeaning & MEANING_0x200) != 0)))) {
			this->mMeaning |= MEANING_0x200;
		}

		this->updateMeaning(X, MEANING_0x200000, prevMeaning);
	}

	// Fabricated
	inline void updateMeaning0x1(u32 prevMeaning)
	{
		// Some kind of 2d menu navigation?
		if (mButton.mRepeat & (MAINSTICK_UP | DPAD_UP)) {
			mMeaning |= MEANING_0x2;
		}
		if (mButton.mRepeat & (MAINSTICK_DOWN | DPAD_DOWN)) {
			mMeaning |= MEANING_0x4;
		}
		if (mButton.mRepeat & (MAINSTICK_LEFT | DPAD_LEFT)) {
			mMeaning |= MEANING_0x8;
		}
		if (mButton.mRepeat & (MAINSTICK_RIGHT | DPAD_RIGHT)) {
			mMeaning |= MEANING_0x10;
		}
		this->updateMeaning(A, MEANING_0x20, prevMeaning);
		this->updateMeaning(B, MEANING_0x40, prevMeaning);
	}

	// Fabricated
	inline void updateMeaning0x8(u32 prevMeaning)
	{
		// This is definitely some kind of menu navigation
		if (mButton.mRepeat & (MAINSTICK_UP | DPAD_UP)) {
			mMeaning |= MEANING_0x80000;
		}
		if (mButton.mRepeat & (MAINSTICK_DOWN | DPAD_DOWN)) {
			mMeaning |= MEANING_0x100000;
		}
		this->updateMeaning(A, MEANING_0x20000, prevMeaning);
		this->updateMeaning(B, MEANING_0x40000, prevMeaning);
		this->mCompSPos[1].x = (f32)(this->mButton.mAnalogL);
		this->mCompSPos[1].y = (f32)(this->mButton.mAnalogR);
		this->updateMeaning(R, MEANING_0x400, prevMeaning);
		this->updateMeaning(Z, MEANING_0x1000, prevMeaning);
		this->updateMeaning(L, MEANING_0x2000, prevMeaning);
	}
	// Fabricated
	inline void updateMeaning0x80(u32 prevMeaning)
	{
		this->updateMeaning(A, MEANING_0x20, prevMeaning);
		this->updateMeaning(B, MEANING_0x40, prevMeaning);
		this->mCompSPos[2].x = this->mMainStick.mPosX;
		this->mCompSPos[2].y = this->mMainStick.mPosY;
	}

	// Fabricated
	inline bool checkFlag(u32 flag) { return (this->mFlags & flag) != 0; }

	u32 read();
	void onNeutralMarioKey();
	void reset();
	void updateMeaning();

	StickPos mCompSPos[VARIANTS]; // _A8
	u32 mMeaning;                 // _D0
	u32 mEnabledFrameMeaning;     // _D4
	u32 mDisabledFrameMeaning;    // _D8
	u16 _DC;
	u16 _DE;
	u16 _E0;

	u16 mFlags;

	s16 _E4;
	u16 _E6;             // padding?
	s32 mDisabledFrames; // _E8
	u32 _EC;             // padding?

	static u16 mResetFlag;
};

#endif
