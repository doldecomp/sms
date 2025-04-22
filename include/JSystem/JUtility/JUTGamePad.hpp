#ifndef JUT_GAMEPAD_HPP
#define JUT_GAMEPAD_HPP

#include <JSystem/JKernel/JKRDisposer.hpp>
#include <JSystem/JUtility/JUTAssert.hpp>
#include <dolphin/os.h>
#include <dolphin/pad.h>

typedef void (*callbackFn)(int, void*);

class JUTGamePadRecord {
public:
	u8* read();
	void write(PADStatus*);
	void streamDataToPadStatus(PADStatus*, u8*);
	void padStatusToStreamData(PADStatus*, u8*);

	bool isActive() const { return mStreamStart; }

	inline void step() // fabricated
	{
		u8* next = mStream + mStep;
		if (next < mStreamStart)
			next = mStreamStart;
		if (next > mStreamEnd)
			next = mStreamEnd;
		mStream = next;
	}

	/* 0x0 */ u8* mStreamStart;
	/* 0x4 */ u8* mStreamEnd;
	/* 0x8 */ u32 unk8;
	/* 0xC */ u32 mRecordedStates;
	/* 0x10 */ u8* mStream;
	/* 0x14 */ u8 unk14;
	/* 0x15 */ u8 mStateFlags;
	/* 0x18 */ u32 mStep;
};

struct JUTGamePad : public JKRDisposer {
public:
	enum EStickMode {
		EStickMode1 = 1,
	};

	enum EWhichStick {
		EMainStick,
		ESubStick,
	};

	enum EPadPort {
		EPortInvalid = -1,
		EPort1,
		EPort2,
		EPort3,
		EPort4,
	};

	enum EClampMode {
		EClampNone,
		EClampStick,
		EClampCircle,
	};

	enum EButtons {
		DPAD_LEFT       = 0x1,
		DPAD_RIGHT      = 0x2,
		DPAD_DOWN       = 0x4,
		DPAD_UP         = 0x8,
		Z               = 0x10,
		R               = 0x20,
		L               = 0x40,
		A               = 0x100,
		B               = 0x200,
		X               = 0x400,
		Y               = 0x800,
		START           = 0x1000,
		CSTICK_LEFT     = 0x10000,
		CSTICK_RIGHT    = 0x20000,
		CSTICK_DOWN     = 0x40000,
		CSTICK_UP       = 0x80000,
		MAINSTICK_LEFT  = 0x1000000,
		MAINSTICK_RIGHT = 0x2000000,
		MAINSTICK_DOWN  = 0x4000000,
		MAINSTICK_UP    = 0x8000000,
	};

	JUTGamePad(JUTGamePad::EPadPort port);
	virtual ~JUTGamePad();

	void initList();
	static BOOL init();
	static u32 read();
	void assign();
	void checkResetCallback(OSTime holdTime);
	void update();
	void stopPatternedRumble() { mRumble.stopPatternedRumble(mPortNum); }

	static void checkResetSwitch();
	static void clearForReset();
	static JUTGamePad* getGamePad(int port);
	static bool recalibrate(u32 mask);

	static void setAnalogMode(u32 mode)
	{
		sAnalogMode = mode;
		PADSetAnalogMode(mode);
	}

	static void clearResetOccurred() { C3ButtonReset::sResetOccurred = false; }

	static void setResetCallback(callbackFn callback, void* arg)
	{
		C3ButtonReset::sCallback    = callback;
		C3ButtonReset::sCallbackArg = arg;
	}

	u32 getButton() const { return mButton.mButton; }
	u32 getTrigger() const { return mButton.mTrigger; }
	f32 getMainStickX() const { return mMainStick.mPosX; }
	f32 getMainStickY() const { return mMainStick.mPosY; }
	f32 getMainStickValue() const { return mMainStick.mValue; }
	s16 getMainStickAngle() const { return mMainStick.mAngle; }
	f32 getSubStickX() const { return mSubStick.mPosX; }
	f32 getSubStickY() const { return mSubStick.mPosY; }
	f32 getSubStickValue() const { return mSubStick.mValue; }
	s16 getSubStickAngle() const { return mSubStick.mAngle; }
	u8 getAnalogA() const { return mButton.mAnalogA; }
	u8 getAnalogB() const { return mButton.mAnalogB; }
	u8 getAnalogL() const { return mButton.mAnalogL; }
	u8 getAnalogR() const { return mButton.mAnalogR; }
	s8 getErrorStatus() const { return mErrorStatus; }

	s16 getPortNum() const { return mPortNum; }

	JUTGamePadRecord* getPadReplay() const { return mPadReplay; }
	JUTGamePadRecord* getPadRecord() const { return mPadRecord; }

	u32 testTrigger(u32 button) const { return mButton.mTrigger & button; }

	bool isPushing3ButtonReset() const
	{
		return mPortNum != EPortInvalid && mButtonReset.mReset != false;
	}

	void stopMotorWave() { mRumble.stopPatternedRumbleAtThePeriod(); }
	void stopMotor() { mRumble.stopMotor(mPortNum); }
	void stopMotorHard() { mRumble.stopMotorHard(mPortNum); }

	static s8 getPortStatus(u32 port) { return mPadStatus[port].err; }

	struct CButton {
		CButton() { clear(); }
		void clear();
		void update(const PADStatus* padStatus, u32 stickStatus);
		void setRepeat(u32 mask, u32 delay, u32 rate);

		/* 0x00 */ u32 mButton;
		/* 0x04 */ u32 mTrigger; // Pressed Buttons
		/* 0x08 */ u32 mRelease; // Released Buttons
		/* 0x0C */ u8 mAnalogA;
		/* 0x0D */ u8 mAnalogB;
		/* 0x0E */ u8 mAnalogL;
		/* 0x0F */ u8 mAnalogR;
		/* 0x10 */ f32 mAnalogLf;
		/* 0x14 */ f32 mAnalogRf;
		/* 0x18 */ u32 mRepeat;
		/* 0x1C */ u32 mRepeatCount;
		/* 0x20 */ u32 mRepeatStart;
		/* 0x24 */ u32 mRepeatMask;
		/* 0x28 */ u32 mRepeatDelay;
		/* 0x2C */ u32 mRepeatRate;
	}; // Size: 0x30

	struct C3ButtonReset {
		C3ButtonReset() { mReset = false; }

		static u32 sResetPattern;
		static u32 sResetMaskPattern;
		static callbackFn sCallback;
		static void* sCallbackArg;
		static OSTime sThreshold;
		static s32 sResetOccurredPort;
		static bool sResetOccurred;
		static bool sResetSwitchPushing;

		/* 0x0 */ bool mReset;
	}; // Size: 0x4

	struct CStick {
		CStick() { clear(); }
		void clear();
		void clear(JUTGamePad* pad);
		u32 update(s8 x, s8 u, JUTGamePad::EStickMode mode,
		           JUTGamePad::EWhichStick stick);
		u32 getButton();

		/* 0x0 */ f32 mPosX;
		/* 0x4 */ f32 mPosY;
		/* 0x8 */ f32 mValue;
		/* 0xC */ s16 mAngle;
		/* 0xE */ s8 mRawX;
		/* 0xF */ s8 mRawY;
	}; // Size: 0x10

	void stopMotorWaveHard() { mRumble.stopPatternedRumble(mPortNum); }

	struct CRumble {
		CRumble(JUTGamePad* pad) { clear(pad); }

		static u32 sChannelMask[4];
		static bool mStatus[4];
		static u32 mEnable;

		enum ERumble {
			VAL_0 = 0,
			VAL_1 = 1,
			VAL_2 = 2,
		};

		void clear();
		void clear(JUTGamePad* pad);
		static void startMotor(int port);
		static void stopMotor(int port);
		static void stopMotorHard(int port);
		void update(s16 port);
		void triggerPatternedRumble(u32 length);
		void startPatternedRumble(void* data, ERumble rumble, u32 length);
		void stopPatternedRumble(s16 port);
		void stopPatternedRumbleAtThePeriod();
		static void setEnable(u32 mask);

		static bool isEnabled(u32 mask) { return mEnable & mask; }

		static bool isEnabledPort(int port)
		{
			JUT_ASSERT(250, 0 <= port && port < 4);
			return isEnabled(sChannelMask[port]);
		}

		/* 0x00 */ u32 mFrame;
		/* 0x04 */ u32 mLength;
		/* 0x08 */ u8* mPattern;
		/* 0x0C */ u32 mFrameCount;
	}; // Size: 0x14

	void startMotorWave(void* data, CRumble::ERumble rumble, u32 length)
	{
		mRumble.startPatternedRumble(data, rumble, length);
	}

	void setButtonRepeat(u32, u32, u32);

	static JSUList<JUTGamePad> mPadList;
	static bool mListInitialized;
	static PADStatus mPadStatus[4];
	static CButton mPadButton[4];
	static CStick mPadMStick[4];
	static CStick mPadSStick[4];
	static EStickMode mStickMode;
	static u8 mPadAssign[4];
	static u32 mSuppressPadReset;
	static s32 sAnalogMode;

	/* 0x18 */ CButton mButton;
	/* 0x48 */ CStick mMainStick;
	/* 0x58 */ CStick mSubStick;
	/* 0x68 */ CRumble mRumble;
	/* 0x7C */ s16 mPortNum;
	/* 0x7E */ s8 mErrorStatus;
	/* 0x80 */ JSULink<JUTGamePad> mLink;
	/* 0x90 */ JUTGamePadRecord* mPadRecord;
	/* 0x94 */ JUTGamePadRecord* mPadReplay;
	/* 0x94 */ u8 field_0x9c[4];
	/* 0x98 */ C3ButtonReset mButtonReset;
	/* 0xA0 */ OSTime mResetHoldStartTime;
};

#endif
