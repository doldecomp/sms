#ifndef MARIOMAIN_HPP
#define MARIOMAIN_HPP

#include <JSystem/JGeometry.hpp>
#include <System/Params.hpp>
#include <System/ParamInst.hpp>
#include <System/DrawSyncCallback.hpp>
#include <Strategic/TakeActor.hpp>
#include <Player/WaterGun.hpp>

class TLiveActor;
class TWaterGun;
class TBGCheckData;
class J3DAnmTexPattern;
class J3DModelData;
class J3DAnmTransform;
struct TBGWallCheckRecord;
class TMarioCap;
class TWaterEmitInfo;
class TBaseNPC;
class J3DFrameCtrl;
class J3DDrawBuffer;
class M3UModelMario;
class TYoshi;
class MActor;
struct ResTIMG;
class MAnmSound;
class JAIAnimeSound;
class JAISound;
class J3DModel;
class TTrembleModelEffect;
class SampleCtrlModelData;
class TMultiMtxEffect;

// TODO: where should this be?
enum E_SIDEWALK_TYPE {
	SIDEWALK_TYPE_UNK0 = 0,
	SIDEWALK_TYPE_UNK1 = 1,
	SIDEWALK_TYPE_UNK2 = 2,
};

enum E_MARIO_FLAG {
	MARIO_FLAG_ABOVE_SEWER_FLOOR   = (1 << 0),
	MARIO_FLAG_VISIBLE             = (1 << 1),
	MARIO_FLAG_NPC_TALKING         = (1 << 3),
	MARIO_FLAG_RECENTLY_LEFT_WATER = (1 << 4),
	MARIO_FLAG_UNK_40              = (1 << 6),
	MARIO_FLAG_GAME_OVER           = (1 << 10),
	MARIO_FLAG_GROUND_POUND_SIT_UP = (1 << 11),
	MARIO_FLAG_HELMET_FLW_CAMERA   = (1 << 12),
	MARIO_FLAG_HELMET              = (1 << 13),
	MARIO_FLAG_FLUDD_EMITTING      = (1 << 14),
	MARIO_FLAG_HAS_FLUDD           = (1 << 15),
	MARIO_FLAG_IN_SHALLOW_WATER    = (1 << 16),
	MARIO_FLAG_IN_WATER            = (1 << 17),
	MARIO_FLAG_HAS_SHIRT           = (1 << 20),
	MARIO_FLAG_IS_PERFORMING       = (1 << 21),
};

enum E_MARIO_RAIL_TYPE {
	MARIO_RAIL_TYPE_PINNA_RAIL = 0,
	MARIO_RAIL_TYPE_KOOPA_RAIL = 1,

	MARIO_RAIL_TYPE_MAX
};

class TRidingInfo {
public:
	TRidingInfo()
	    : unk0(nullptr)
	    , localPos(0.0f, 0.0f, 0.0f)
	    , unk10(0.0f)
	{
	}

public:
	/* 0x0 */ const TLiveActor* unk0;
	/* 0x4 */ JGeometry::TVec3<f32> localPos;
	/* 0x10 */ f32 unk10;
};

struct TMarioSoundValues {
	/* 0x00 */ int unk00;
	/* 0x04 */ int unk04;
	/* 0x08 */ int unk08;
	/* 0x0C */ int unk0C;
	/* 0x10 */ int unk10;
	/* 0x14 */ int unk14;
	/* 0x18 */ u8 unk18;
	/* 0x1C */ int unk1C;
	/* 0x20 */ u8 unk20;
	/* 0x22 */ u16 unk22;
	/* 0x24 */ u8 unk24;
	/* 0x26 */ u16 unk26;
	/* 0x28 */ u8 unk28;
	/* 0x29 */ u8 unk29;
	/* 0x2A */ u8 unk2A;
	/* 0x2B */ u8 unk2B;
	/* 0x2C */ u8 unk2C;
};

class TMarioGamePad;
struct TMarioControllerWork;

class TMario : public TTakeActor, public TDrawSyncCallback {
public:
	struct JumpSlipRecord {
		/* 0x00 */ s16 mMaxTimer;
		/* 0x04 */ u32 mEndStatus;
		/* 0x08 */ u32 mJumpStatus;
		/* 0x0C */ u32 unkC;
		/* 0x10 */ u32 unk10;
	};

	class TOptionParams : public TParams {
	public:
		TOptionParams(const char* prm);

		TParamRT<f32> mZ;
		TParamRT<f32> mXMin;
		TParamRT<f32> mXMax;
	};

	class TSoundParams : public TParams {
	public:
		TSoundParams();

		TParamRT<f32> mStartFallVoiceSpeed;
	};

	class TAutoDemoParams : public TParams {
	public:
		TAutoDemoParams();

		TParamRT<s16> mWarpInBallsDispTime;
		TParamRT<s16> mWarpInBallsTime;
		TParamRT<s16> mWarpInCapturedTime;
		TParamRT<f32> mWarpInTremble;
		TParamRT<f32> mWarpInVecBase;
		TParamRT<f32> mWarpTransTremble;
		TParamRT<s16> mReadRotSp;
	};

	class TEParams : public TParams {
	public:
		TEParams(const char* prm);

		TParamRT<u8> mDamage;
		TParamRT<u8> mDownType;
		TParamRT<u8> mWaterEmit;
		TParamRT<u8> mMotor;
		TParamRT<f32> mMinSpeed;
		TParamRT<f32> mDirty;
		TParamRT<s16> mInvincibleTime;
	};

	class TDivingParams : public TParams {
	public:
		TDivingParams(const char* prm);

		TParamRT<s16> mRotSp;
		TParamRT<f32> mGravity;
		TParamRT<f32> mAccelControl;
		TParamRT<f32> mSeaBrake;
		TParamRT<f32> mSeaBrakeY;
	};

	class THHoverParams : public TParams {
	public:
		THHoverParams(const char* prm);

		TParamRT<s16> mRotSp;
		TParamRT<f32> mAccelRate;
		TParamRT<f32> mBrake;
	};

	class TSurfingParams : TParams {
	public:
		TSurfingParams(const char* prm);

		TParamRT<f32> mRotMin;
		TParamRT<f32> mRotMax;
		TParamRT<f32> mPowMin;
		TParamRT<f32> mPowMax;
		TParamRT<f32> mAccel;
		TParamRT<f32> mWaistRoll;
		TParamRT<f32> mWaistPitch;
		TParamRT<s16> mWaistRollMax;
		TParamRT<s16> mWaistPitchMax;
		TParamRT<s32> mRoll;
		TParamRT<f32> mPitch;
		TParamRT<s16> mRollMax;
		TParamRT<s16> mPitchMax;
		TParamRT<f32> mAngleChangeRate;
		TParamRT<f32> mWaistAngleChangeRate;
		TParamRT<f32> mScaleMin;
		TParamRT<f32> mScaleMax;
		TParamRT<f32> mScaleMinSpeed;
		TParamRT<f32> mScaleMaxSpeed;
		TParamRT<f32> mJumpPow;
		TParamRT<f32> mJumpXZRatio;
		TParamRT<f32> mClashSpeed;
		TParamRT<s16> mClashAngle;
	};

	class TUpperParams : public TParams {
	public:
		TUpperParams(const char* prm);

		TParamRT<s16> mPumpWaitTime;
		TParamRT<f32> mPumpAnmSpeed;
		TParamRT<s16> mHoverHeadAngle;
		TParamRT<s16> mFeelDeepHeadAngle;
		TParamRT<s16> mFrontWallHeadAngle;
	};

	class TSlipParams : TParams {
	public:
		TSlipParams(const char* prm);

		TParamRT<f32> mSlipFriction;
		TParamRT<f32> mSlopeAcceleUp;
		TParamRT<f32> mSlopeAcceleDown;
		TParamRT<f32> mSlideAcceleUp;
		TParamRT<f32> mSlideAcceleDown;
		TParamRT<f32> mSlideStopNormal;
		TParamRT<f32> mSlideStopCatch;
		TParamRT<u8> mJumpEnable;
		TParamRT<u8> mMissJump;
		TParamRT<s16> mSlideAngleYSp;
		TParamRT<f32> mStickSlideMult;
	};

	class TEffectParams : public TParams {
	public:
		TEffectParams();

		TParamRT<f32> mDashInc;
		TParamRT<f32> mDashDec;
		TParamRT<u8> mDashMaxBlendInBlur;
		TParamRT<u8> mDashMaxBlendInIris;
		TParamRT<f32> mDashBlendScale;
	};

	class TParticleParams : public TParams {
	public:
		TParticleParams();

		TParamRT<f32> mMeltInWaterMax;
		TParamRT<f32> mWaveEmitSpeed;
		TParamRT<s16> mWaveAlphaDec;
		TParamRT<f32> mBubbleDepth;
		TParamRT<f32> mBodyBubbleSpMin;
		TParamRT<f32> mBodyBubbleSpMax;
		TParamRT<f32> mBodyBubbleEmitMin;
		TParamRT<f32> mBodyBubbleEmitMax;
		TParamRT<f32> mBubbleToRipple;
		TParamRT<f32> mToroccoWind;
		TParamRT<f32> mToroccoSpark;
	};

	class TMotorParams : TParams {
	public:
		TMotorParams();

		TParamRT<s16> mMotorReturn;
		TParamRT<s16> mMotorTrample;
		TParamRT<s16> mMotorHipDrop;
		TParamRT<s16> mMotorWall;
	};

	class TDirtyParams : TParams {
	public:
		TDirtyParams();

		TParamRT<f32> mIncRunning;
		TParamRT<f32> mIncCatching;
		TParamRT<f32> mIncSlipping;
		TParamRT<f32> mDecSwimming;
		TParamRT<f32> mDecWaterHit;
		TParamRT<f32> mDecRotJump;
		TParamRT<f32> mBrakeStartValSlip;
		TParamRT<f32> mBrakeStartValRun;
		TParamRT<s16> mDirtyTimeSlip;
		TParamRT<s16> mDirtyTimeRun;
		TParamRT<f32> mPolSizeSlip;
		TParamRT<f32> mPolSizeRun;
		TParamRT<f32> mPolSizeFootPrint;
		TParamRT<f32> mPolSizeJump;
		TParamRT<f32> mSlopeAngle;
		TParamRT<f32> mDirtyMax;
		TParamRT<f32> mSlipAnmSpeed;
		TParamRT<f32> mSlipRunSp;
		TParamRT<f32> mSlipCatchSp;
		TParamRT<s16> mSlipRotate;
		TParamRT<s16> mSlipCatchRotate;
		TParamRT<f32> mBrakeSlipNoPollute;
		TParamRT<s16> mFogTimeYellow;
		TParamRT<s16> mFogTimeRed;
	};

	class TGraffitoParams : TParams {
	public:
		TGraffitoParams();

		TParamRT<s16> mSinkTime;
		TParamRT<s16> mSinkDmgTime;
		TParamRT<f32> mSinkHeight;
		TParamRT<f32> mSinkMoveMin;
		TParamRT<f32> mSinkMoveMax;
		TParamRT<f32> mSinkRecover;
		TParamRT<f32> mSinkJumpRateMin;
		TParamRT<f32> mSinkJumpRateMax;
		TParamRT<f32> mSinkPumpLimit;
		TParamRT<f32> mSinkDmgDepth;
		TParamRT<f32> mFireHeight;
		TParamRT<s16> mDizzySlipCtMax;
		TParamRT<s16> mDizzyWalkCtMax;
		TParamRT<s16> mDizzyAngleY;
		TParamRT<f32> mDizzyAngleRate;
		TParamRT<f32> mDizzyPowerRate;
		TParamRT<f32> mDizzyPower;
		TParamRT<s16> mFireInvincibleTime;
		TParamRT<s16> mFootEraseTimes;
		TParamRT<f32> mFootEraseSize;
		TParamRT<f32> mFootEraseFront;
	};

	class TControllerParams : TParams {
	public:
		TControllerParams();

		TParamRT<u8> mAnalogLRToZeroVal;
		TParamRT<u8> mAnalogLRToMiddleVal;
		TParamRT<u8> mAnalogLRToMaxVal;
		TParamRT<f32> mAnalogLRMiddleLevel;
		TParamRT<f32> mStartToWalkLevel;
		TParamRT<s16> mStickRotateTime;
		TParamRT<s16> mLengthMultTimes;
		TParamRT<f32> mLengthMult;
		TParamRT<f32> mSquatRotMidAnalog;
		TParamRT<f32> mSquatRotMidValue;
	};

	class TYoshiParams : public TParams {
	public:
		TYoshiParams();

		TParamRT<f32> mRunYoshiMult;
		TParamRT<f32> mJumpYoshiMult;
		TParamRT<f32> mRotYoshiMult;
		TParamRT<f32> mHeadFront;
		TParamRT<f32> mHeadRadius;
		TParamRT<f32> mHoldOutAccCtrlF;
		TParamRT<f32> mHoldOutAccCtrlB;
		TParamRT<f32> mHoldOutSldCtrl;
		TParamRT<f32> mDecBrake;
	};

	class TWaterEffectParams : public TParams {
	public:
		TWaterEffectParams();

		TParamRT<f32> mJumpIntoMdlEffectSpY;
		TParamRT<f32> mJumpIntoMinY;
		TParamRT<f32> mJumpIntoMaxY;
		TParamRT<f32> mJumpIntoScaleMin;
		TParamRT<f32> mJumpIntoScaleWidth;
		TParamRT<f32> mRunningRippleSpeed;
		TParamRT<f32> mRunningRippleDepth;
	};

	class TBarParams : public TParams {
	public:
		TBarParams();

		TParamRT<f32> mClimbSp;
		TParamRT<f32> mRotateSp;
		TParamRT<f32> mClimbAnmRate;
		TParamRT<f32> mCatchRadius;
		TParamRT<f32> mCatchAngle;
	};

	class TPullParams : public TParams {
	public:
		TPullParams(const char* prm);

		TParamRT<f32> mPullRateV;
		TParamRT<f32> mPullRateH;
		TParamRT<f32> mOilPullRateV;
		TParamRT<f32> mOilPullRateH;
	};

	class TWireParams : public TParams {
	public:
		TWireParams();

		TParamRT<s16> mRotSpeed;
		TParamRT<s16> mRotSpeedTrgHover;
		TParamRT<s16> mRotSpeedTrgTurbo;
		TParamRT<s16> mRotSpeedTrgRocket;
		TParamRT<s16> mRotSpeedMax;
		TParamRT<s16> mRotStop;
		TParamRT<s16> mRotGravity;
		TParamRT<f32> mRotBrake;
		TParamRT<f32> mJumpRate;
		TParamRT<f32> mSwingRate;
		TParamRT<f32> mWireJumpAccelControl;
		TParamRT<f32> mWireJumpSlideControl;
		TParamRT<f32> mWireJumpMult;
		TParamRT<f32> mWireJumpBase;
		TParamRT<f32> mWireSwingBrake;
		TParamRT<f32> mWireSwingMax;
	};

	class THangRoofParams : public TParams {
	public:
		THangRoofParams();

		TParamRT<f32> mAnmMult;
	};

	class THangingParams : public TParams {
	public:
		THangingParams();

		TParamRT<f32> mMoveSp;
		TParamRT<f32> mAnmRate;
		TParamRT<s16> mRapidTime;
		TParamRT<s16> mLimitTime;
		TParamRT<f32> mAnmRapid;
		TParamRT<f32> mDescentSp;
	};

	class TSwimParams : TParams {
	public:
		TSwimParams();

		TParamRT<f32> mStartSp;
		TParamRT<f32> mMoveSp;
		TParamRT<f32> mMoveBrake;
		TParamRT<s16> mSwimmingRotSpMin;
		TParamRT<s16> mSwimmingRotSpMax;
		TParamRT<s16> mPumpingRotSpMin;
		TParamRT<s16> mPumpingRotSpMax;
		TParamRT<f32> mGravity;
		TParamRT<f32> mWaitBouyancy;
		TParamRT<f32> mMoveBouyancy;
		TParamRT<f32> mUpDownBrake;
		TParamRT<f32> mCanJumpDepth;
		TParamRT<f32> mEndDepth;
		TParamRT<f32> mFloatHeight;
		TParamRT<f32> mStartVMult;
		TParamRT<f32> mStartVYMult;
		TParamRT<f32> mRush;
		TParamRT<f32> mAnmBrake;
		TParamRT<f32> mPaddleSpeedUp;
		TParamRT<f32> mPaddleJumpUp;
		TParamRT<f32> mFloatUp;
		TParamRT<f32> mWaterLevelCheckHeight;
		TParamRT<f32> mPaddleDown;
		TParamRT<s16> mWaitSinkTime;
		TParamRT<f32> mCanBreathDepth;
		TParamRT<f32> mWaitSinkSpeed;
		TParamRT<f32> mAirDec;
		TParamRT<f32> mAirDecDive;
		TParamRT<f32> mAirInc;
	};

	class TRunParams : TParams {
	public:
		TRunParams();

		TParamRT<f32> mMaxSpeed;
		TParamRT<f32> mVelMinusBrake;
		TParamRT<f32> mAddBase;
		TParamRT<f32> mAddVelDiv;
		TParamRT<f32> mDecStartNrmY;
		TParamRT<f32> mDecBrake;
		TParamRT<f32> mSoft2Walk;
		TParamRT<f32> mWalk2Soft;
		TParamRT<f32> mSoftStepAnmMult;
		TParamRT<f32> mRunAnmSpeedBase;
		TParamRT<f32> mRunAnmSpeedMult;
		TParamRT<f32> mMotBlendWalkSp;
		TParamRT<f32> mMotBlendRunSp;
		TParamRT<f32> mSwimDepth;
		TParamRT<f32> mInWaterBrake;
		TParamRT<f32> mInWaterAnmBrake;
		TParamRT<f32> mPumpingSlideSp;
		TParamRT<f32> mPumpingSlideAnmSp;
		TParamRT<f32> mDoJumpCatchSp;
		TParamRT<f32> mTurnNeedSp;
		TParamRT<s16> mDashRotSp;
	};

	class TJumpParams : TParams {
	public:
		TJumpParams();

		TParamRT<f32> mGravity;
		TParamRT<f32> mSpinJumpGravity;
		TParamRT<f32> mJumpingMax;
		TParamRT<f32> mJumpSpeedBrake;
		TParamRT<f32> mJumpAccelControl;
		TParamRT<f32> mJumpSlideControl;
		TParamRT<f32> mTurnJumpForce;
		TParamRT<f32> mFenceSpeed;
		TParamRT<f32> mFireDownForce;
		TParamRT<f32> mFireDownControl;
		TParamRT<f32> mFireBackVelocity;
		TParamRT<f32> mBroadJumpForce;
		TParamRT<f32> mBroadJumpForceY;
		TParamRT<f32> mRotateJumpForceY;
		TParamRT<f32> mPopUpSpeedY;
		TParamRT<f32> mPopUpForceYMult;
		TParamRT<f32> mBackJumpForce;
		TParamRT<f32> mBackJumpForceY;
		TParamRT<f32> mHipAttackSpeedY;
		TParamRT<f32> mSuperHipAttackSpeedY;
		TParamRT<s16> mJumpCatchRotXSp;
		TParamRT<s16> mJumpCatchRotXMax;
		TParamRT<f32> mRotBroadEnableV;
		TParamRT<f32> mRotBroadJumpForce;
		TParamRT<f32> mRotBroadJumpForceY;
		TParamRT<f32> mTrampolineDec;
		TParamRT<f32> mSecJumpEnableSp;
		TParamRT<f32> mSecJumpForce;
		TParamRT<f32> mSecJumpSpeedMult;
		TParamRT<f32> mSecJumpXZMult;
		TParamRT<f32> mTriJumpEnableSp;
		TParamRT<f32> mUltraJumpForce;
		TParamRT<f32> mUltraJumpSpeedMult;
		TParamRT<f32> mUltraJumpXZMult;
		TParamRT<f32> mValleyDepth;
		TParamRT<f32> mThrownAccel;
		TParamRT<f32> mThrownSlide;
		TParamRT<f32> mThrownBrake;
		TParamRT<f32> mTremblePower;
		TParamRT<f32> mTrembleAccele;
		TParamRT<f32> mTrembleBrake;
		TParamRT<s16> mTrembleTime;
		TParamRT<s16> mClashAngle;
		TParamRT<f32> mJumpJumpCatchSp;
		TParamRT<f32> mGetOffYoshiY;
		TParamRT<s16> mSuperHipAttackCt;
	};

	class TAttackParams : public TParams {
	public:
		TAttackParams(const char* prm);

		TParamRT<f32> mRadius;
		TParamRT<f32> mHeight;
	};

	class TBodyAngleParams : public TParams {
	public:
		TBodyAngleParams(const char* prm);

		TParamRT<f32> mHeadRot;
		TParamRT<f32> mWaistRoll;
		TParamRT<f32> mWaistPitch;
		TParamRT<s16> mWaistRollMax;
		TParamRT<s16> mWaistPitchMax;
		TParamRT<f32> mWaistAngleChangeRate;
	};

	class TDeParams : public TParams {
	public:
		TDeParams();

		TParamRT<s16> mHpMax;
		TParamRT<f32> mRunningMax;
		TParamRT<f32> mDashMax;
		TParamRT<f32> mDashAcc;
		TParamRT<f32> mDashBrake;
		TParamRT<s16> mDashStartTime;
		TParamRT<s16> mWaitingRotSp;
		TParamRT<s16> mRunningRotSpMin;
		TParamRT<s16> mRunningRotSpMax;
		TParamRT<s16> mRocketRotSp;
		TParamRT<s16> mPumpingRotSpMin;
		TParamRT<s16> mPumpingRotSpMax;
		TParamRT<s16> mInvincibleTime;
		TParamRT<s16> mFootPrintTimerMax;
		TParamRT<u8> mWaterTriggerRate;
		TParamRT<u8> mGraffitoNoDmgTime;
		TParamRT<u8> mRestMax;
		TParamRT<f32> mShadowSize;
		TParamRT<f32> mShadowErase;
		TParamRT<f32> mHoldRadius;
		TParamRT<f32> mDamageRadius;
		TParamRT<f32> mDamageHeight;
		TParamRT<f32> mAttackHeight;
		TParamRT<f32> mTrampleRadius;
		TParamRT<f32> mPushupRadius;
		TParamRT<f32> mPushupHeight;
		TParamRT<f32> mHipdropRadius;
		TParamRT<f32> mQuakeRadius;
		TParamRT<f32> mQuakeHeight;
		TParamRT<f32> mTramplePowStep1;
		TParamRT<f32> mTramplePowStep2;
		TParamRT<f32> mTramplePowStep3;
		TParamRT<f32> mJumpWallHeight;
		TParamRT<f32> mThrowPower;
		TParamRT<f32> mSlipStart;
		TParamRT<f32> mWasOnWaterSlip;
		TParamRT<f32> mInWaterSlip;
		TParamRT<f32> mToroccoRotSp;
		TParamRT<s16> mRecoverTimer;
		TParamRT<s16> mHotTimer;
		TParamRT<f32> mFeelDeep;
		/* 0x33C */ TParamRT<f32> mDamageFallHeight;
		/* 0x350 */ TParamRT<f32> mForceSlipAngle;
		TParamRT<f32> mClashSpeed;
		TParamRT<f32> mHangWallMovableAngle;
		TParamRT<f32> mColMvMax;
		TParamRT<s16> mNoFreezeTime;
		TParamRT<s16> mKickFreezeTime;
		TParamRT<s16> mSurfStartFreezeTime;
		TParamRT<f32> mSleepingCheckDist;
		TParamRT<f32> mSleepingCheckHeight;
		/* 0x404 */ TParamRT<s16> mIllegalPlaneCtInc;
		/* 0x418 */ TParamRT<s16> mIllegalPlaneTime;
	};

public:
	TMario();

	virtual ~TMario() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual MtxPtr getTakingMtx();
	virtual BOOL moveRequest(const JGeometry::TVec3<f32>&);

	virtual void drawSyncCallback(u16);
	virtual void initValues();
	virtual void checkReturn();
	virtual void checkController(JDrama::TGraphics*);
	virtual void playerControl(JDrama::TGraphics*);
	virtual void initModel();
	virtual void drawSpecial(JDrama::TGraphics*);
	virtual void damageExec(THitActor* hittingActor, int damage,
	                        int damageAnimType, int waterEmit,
	                        f32 knockbackSpeed, int rumbleFrames,
	                        f32 pollutionAmount, s16 invincibilityFrames);
	virtual void checkCollision();
	virtual void getVoiceStatus();

	// Action stuff
	BOOL taking();
	BOOL takePose();
	BOOL catchLost();
	BOOL putting();
	BOOL pitching();
	BOOL actnMain();

	// Demo stuff
	BOOL winDemo();
	BOOL readBillboard();
	BOOL bottleIn();
	BOOL elecDowning();
	BOOL jumpingDemoCommon(u32, int, f32);
	BOOL openDoor();
	BOOL sinkLoser();
	BOOL downLoser();
	BOOL warpIn();
	BOOL isUnUsualStageStart();
	void startCommon(const JGeometry::TVec3<f32>*, f32);
	BOOL rollingStart(const JGeometry::TVec3<f32>*, f32);
	BOOL returnStart(const JGeometry::TVec3<f32>*, f32, bool, int);
	BOOL waitingStart(const JGeometry::TVec3<f32>*, f32);
	BOOL toroccoStart();
	BOOL warpOut();
	BOOL electricDamage();
	BOOL footDowning();
	BOOL nomotion();
	BOOL disappear();
	BOOL demoMain();

	// Collision stuff
	void considerTake();
	void calcDamagePos(const JGeometry::TVec3<f32>&);
	void floorDamageExec(const TMario::TEParams&);
	void floorDamageExec(int, int, int, int);
	void loserExec();
	void normalizeNozzle();
	void resetNozzle();
	bool trampleExec(THitActor*);
	bool canTake(THitActor*);
	bool isTakeSituation(THitActor*);
	void dropObject();
	s16 getAttackAngle(const THitActor*);
	void decHP(int);
	void incHP(int);
	void rumbleStart(int, int);

	// Draw stuff
	void getJumpIntoWaterModelData();
	void getHeadRot();
	Mtx* getRootAnmMtx();
	MtxPtr getCenterAnmMtx();
	f32 getPumpFrame() const;
	s16 getWallAngle() const;
	void setDivHelm();
	bool isWearingCap();
	bool isWearingHelm();
	void wearGlass();
	void takeOffGlass();
	void setPositions();
	u32 getTrampleCt();
	void calcBodyPos(JGeometry::TVec3<f32>*);
	MtxPtr getTakenMtx();
	void getRailMtx() const;
	f32 getCurrentFrame(int);
	J3DFrameCtrl& getMotionFrameCtrl();
	BOOL isLast1AnimeFrame();
	BOOL isAnimeLoopOrStop();
	void changeHand(int);
	void changeHandByRate(f32);
	f32 setAnimation(int, f32);
	f32 setReverseAnimation(int, f32);
	void loadAnm(J3DAnmTransform**, const char*);
	void loadBas(void**, const char*);
	void loadAnmTexPattern(J3DAnmTexPattern**, char*, J3DModelData*);
	void initMirrorModel();
	void finalDrawInitialize();
	bool isUpperPumpingStyle() const;
	void considerWaist();
	void calcBaseMtxTorocco(MtxPtr);
	void calcBaseMtxPole(MtxPtr);
	void calcBaseMtxSwim(MtxPtr);
	void calcBaseMtx(MtxPtr);
	void calcAnimBody(u32, JDrama::TGraphics*);
	void calcAnimHands();
	void addCallBack(JDrama::TGraphics*);
	void setUpperDamageRun();
	void addUpper();
	void removeCallBack();
	void calcAnim(u32, JDrama::TGraphics*);
	void calcView(JDrama::TGraphics*);
	void entryModels(JDrama::TGraphics*);
	void drawLogic();
	void boxDrawPrepare(MtxPtr);
	void addDirty();
	void addDamageFog(JDrama::TGraphics*);
	void thinkDashEffect();

	const TEParams& getDmgMapCode(int param_1) const
	{
		switch (param_1) {
		case 0:
			return mDmgMapParams0;
		case 1:
			return mDmgMapParams1;
		case 2:
			return mDmgMapParams2;
		case 3:
			return mDmgMapParams3;
		case 4:
			return mDmgMapParams4;
		case 5:
			return mDmgMapParams5;
		case 6:
			return mDmgMapParams6;
		case 7:
			return mDmgMapParams7;
		case 8:
			return mDmgMapParams8;
		case 9:
			return mDmgMapParams9;
		default:
			return mDmgMapParams0;
		}
	}

	// Jump stuff
	BOOL startJumpWall();
	void checkJumpingThrowStart();
	void doSlipJumping();
	void doSpinJumping();
	void setJumpingAttackArea();
	void doJumping();
	void askStrongGroundTouch();
	int jumpingBasic(int, int, int);
	int considerJumpRotate();
	int checkBackTrig();
	int jumpingCommonEvents();
	int jumping();
	int secJumping();
	int ultraJumping();
	int backJumping();
	int landing();
	int uTurnJumping();
	int jumpWall();
	int jumpCatch();
	int jumpingThrow();
	int jumpDownCommon(int, int, f32);
	void checkWallJumping();
	int jumpShortBackDown();
	int jumpShortForeDown();
	int jumpBackDown();
	int jumpForeDown();
	void landSafeDown();
	int stayWall();
	BOOL catchStop();
	int slipFalling();
	int fireDowning();
	int thrownDowning();
	void trample();
	void missJumping();
	void fireJumping();
	void fireLanding();
	int broadJumping();
	int rotateBroadJumping();
	int boardJumping();
	int rocketCheck();
	int rocketing();
	int rotateJumping();
	void wireJumping();
	void pullJumping();
	int hipAttacking();
	int diving();
	void fallDead();
	int jumpMain();

	// ?
	void thinkAloha();
	void thinkCube();
	void thinkFreeze();
	BOOL isMario();

	// Move stuff
	f32 getJumpAccelControl() const;
	f32 getJumpSlideControl() const;
	bool canSquat() const;
	bool isUnderWater() const;
	bool isInvincible() const;
	bool isWallInFront() const;
	void getRidingMtx(MtxPtr);
	bool isForceSlip();
	void warpRequest(const JGeometry::TVec3<f32>&, f32);
	void flowMove(const JGeometry::TVec3<f32>&);
	void windMove(const JGeometry::TVec3<f32>&);
	void getGroundJumpPower() const;
	BOOL onYoshi() const;
	void addVelocity(f32);
	BOOL considerRotateJumpStart();
	BOOL canBendBody();
	void isSpeedZero();
	void changePos(const Vec&);
	void setNormalAttackArea();
	void setPlayerVelocity(f32);
	TBGCheckData* checkWallPlane(Vec*, f32, f32);
	f32 checkRoofPlane(const Vec&, f32, const TBGCheckData**);
	BOOL isFrontSlip(int);
	BOOL isSlipStart();
	BOOL canSlipJump();
	f32 getSlideStopNormal();
	f32 getSlideStopCatch();
	void isSlipLimit();
	BOOL isJumpMiss();
	f32 checkPlayerAround(int, f32);
	void isTurnStart();
	void isTurnning();
	void setMissJumping();
	void setPlayerJumpSpeed(f32 speed_mult, f32 force);
	u32 setStatusToJumping(u32, u32);
	u32 setStatusToRunning(u32, u32);
	void throwMario(const JGeometry::TVec3<f32>&, f32);
	BOOL changePlayerStatus(u32, u32, bool);
	BOOL changePlayerTriJump();
	BOOL changePlayerJumping(u32, u32);
	BOOL changePlayerDropping(u32, u32);
	int checkAllMotions();
	void makeGraffitoDamage(const TMario::TEParams&);
	void checkGraffitoDamage();
	void checkGraffitoFire();
	void checkGraffitoLava();
	void checkGraffitoSlip();
	void checkGraffitoElec();
	void checkGraffito();
	void dirtyLimitCheck();
	void thinkDirty();
	void thinkHeight();
	void checkSink();
	void checkEnforceJump();
	void doReturn();
	void checkThrowObject();
	void getDizzyAngle();
	void getDizzyPower();
	void getLRLevel(u8);
	int checkStickRotate(int*);
	void checkStickSmash();
	void makeHistory();
	BOOL checkGroundPlane(f32, f32, f32, f32*, const TBGCheckData**);
	void checkCurrentPlane();
	void getActorMtx(const THitActor&, MtxPtr);
	void checkRideMovement();
	void checkRideReCalc();
	void checkPlayerAction(JDrama::TGraphics*);
	void stateMachine();
	void canReadBillboard(int);
	void talkMove();
	void view1stMove();
	void calcGroundMtx(const JGeometry::TVec3<f32>&);
	void thinkSituation();
	void thinkWaterSurface();
	void thinkSand();
	void thinkParams();
	void getOffYoshi(bool);
	void checkYoshiGetOff();
	void thinkYoshiHeadCollision();
	void thinkDiving();
	void thinkTorocco();
	void thinkSound();
	void checkWet();
	void gunExec();

	// Physics stuff
	void playerRefrection(int);
	void keepDistance(const JGeometry::TVec3<f32>&, f32, f32);
	void keepDistance(const THitActor&, f32);
	void checkDescent();
	int checkGroundAtWalking(Vec*);
	void stopProcess();
	int waitProcess();
	int walkProcess();
	int barProcess();
	BOOL hangonCheck(const TBGCheckData*, const Vec&, const Vec&);
	int checkGroundAtJumping(const Vec&, int);
	BOOL isFallCancel();
	void fallProcess();
	int jumpProcess(int);

	// Run stuff
	BOOL considerRotateStart();
	BOOL isThrowStart();
	void postureControl();
	void clashStandard(u32, u32);
	void changePlayerPower(f32, u32, u32);
	BOOL isRunningSlipStart();
	BOOL isRunningTurnning();
	void changePlayerCatching();
	bool isRunningInWater();
	void getRunningInWaterBrake();
	BOOL doRunningAnimation();
	void getSlopeNormalAccele(f32*, f32*);
	void getSlopeSlideAccele(f32*, f32*);
	f32 getChangeAngleSpeed();
	f32 getSlideStickMult();
	void slideProcess(f32, f32);
	BOOL doSliding(f32);
	void slopeProcess();
	void doSlipping(f32);
	void doStopping();
	void doRunning();
	TSurfingParams* getSurfingParamsWater();
	TSurfingParams* getSurfingParamsGround();
	void doSurfing();
	void doBraking(f32);
	void changePlayerWaiting();
	void doPushingAnimation(const Vec&);
	BOOL running();
	BOOL rotating();
	BOOL turnning();
	BOOL turnEnd();
	int braking();
	BOOL surfing();
	int toroccoing();
	BOOL walkEnd();
	int speedSliding();
	BOOL fireDashing();
	void slippingBasic(int, int, int);
	BOOL slipForeCommon(int, int, int, int);
	int slipFore();
	BOOL slipBackCommon(int, int, int);
	int slipBack();
	BOOL catching();
	int squatSlipping();
	int oilRun();
	int oilSlip();
	int oilSlope();
	f32 downingCommon(int, f32, int);
	int backDown();
	int foreDown();
	int shortBackDown();
	int shortForeDown();
	int safeBackDown();
	int safeForeDown();
	int catchDown();
	BOOL loserDown();
	BOOL jumpSlipCommon(s16, u32);
	BOOL jumpSlipEvents(TMario::JumpSlipRecord*);
	void jumpSlip();
	void landSlip();
	void secJumpSlip();
	void uTurnJumpSlip();
	void ultraJumpSlip();
	void broadJumpSlip();
	BOOL moveMain();

	// Special stuff
	void barJumpSetting();
	BOOL barWait();
	BOOL barClimb();
	BOOL barHang();
	int hangingCheckRoof(JGeometry::TVec3<f32>*);
	int doRoofMovingProcess();
	void doRoofWaitingProcess();
	BOOL roofCommonEvents();
	BOOL hangRoof();
	BOOL waitRoof();
	BOOL moveRoof();
	BOOL kickRoof();
	BOOL kickRoofRollUp();
	BOOL kickRoofRollDown();
	BOOL startHangLanding(u32);
	void hangingCommon(int, int);
	void findNearestWall(const TBGWallCheckRecord&);
	BOOL hanging();
	BOOL ascend();
	BOOL descend();
	BOOL hangJumping();
	BOOL taken();
	void getOnWirePosAngle(JGeometry::TVec3<f32>*, s16*);
	BOOL wireMove(f32);
	BOOL wireWait();
	BOOL wireSWait();
	BOOL wireWaitToSWaitL();
	BOOL wireWaitToSWaitR();
	void changeWireHanging();
	void wireWaitToHang();
	void wireSWaitToHang();
	void wireReturn();
	BOOL wireHanging();
	s16 getNozzleEmitVX();
	BOOL wireRolling();
	void wireSWaitToWaitL();
	void wireSWaitToWaitR();
	void getCurrentPullParams(f32*, f32*);
	void setPullingAnm(const JGeometry::TVec3<f32>&, f32);
	BOOL pulling();
	void fenceFootCheck();
	BOOL fenceCatch();
	BOOL fenceJumpCatch();
	BOOL fenceMove();
	BOOL fencePunch();
	BOOL specMain();

	// Upper stuff
	void checkPumping();
	BOOL checkPumpEnable();
	void stateMachineUpper();

	// Particle stuff
	void initParticle();
	void moveParticle();
	void emitParticle(int);
	void emitParticle(int, const JGeometry::TVec3<f32>*);
	void emitParticle(int, s16);
	void emitSmoke(s16);
	void emitSweat(s16);
	void emitSweatSometimes();
	void emitSweatSometimes(s16);
	void emitRecover();
	void emitGetEffect();
	void emitGetWaterEffect();
	void emitGetCoinEffect(JGeometry::TVec3<f32>*);
	void strongTouchDownEffectDisp();
	void strongTouchDownEffect();
	void smallTouchDownEffect();
	void rippleEffect();
	void rippleEffectSmall();
	void inOutWaterEffect(f32);
	void bubbleFromMouth(int);
	void bubbleFromBody();
	void swimmingBubbleEffect();
	void smallRippleEffect(JGeometry::TVec3<f32>*);
	void runningRippleEffect();
	void blurEffect();
	void wallSlipEffect();
	void treeSlipEffect();
	void frontSlipEffect();
	void surfingEffect();
	void warpInEffect();
	void warpInLight();
	void warpOutEffect(int, f32);
	void emitBlurHipDrop();
	void emitBlurHipDropSuper();
	void emitBlurSpinJump();
	void emitRotateShootEffect();
	void setFootPrint(const JGeometry::TVec3<f32>&, int);
	void emitFootPrintWithEffect(int, int);
	void emitFootPrint(int);
	void emitDirtyFootPrint();
	void emitSandEffect();
	void meltInWaterEffect();
	void rocketEffectNozzle();
	void rocketEffectStart();
	void elecEffect();
	void elecEndEffect();
	void kickRoofEffect();
	void sleepingEffect();
	void sleepingEffectKill();
	void toroccoEffect();
	void kickFruitEffect();
	void sinkInSandEffect();
	bool askJumpIntoWaterEffectExist() const;

	// Wait stuff
	BOOL startTalking();
	bool canSleep();
	BOOL canPut();
	void checkPutStart();
	BOOL waitingCommonEvents();
	void stopCommon(int anim_id, int status_on_end);
	void changeMontemanWaitingAnim();
	BOOL waiting();
	BOOL sleepily();
	BOOL sleeping();
	BOOL wakeup();
	void getSideWalkValues(E_SIDEWALK_TYPE*, f32*, f32*);
	BOOL squating();
	BOOL squatStart();
	BOOL squatStandup();
	BOOL pullEnd();
	BOOL jumpEndCommon(int, int);
	BOOL jumpEndEvents(u32);
	BOOL jumpEnd();
	BOOL secJumpEnd();
	BOOL landEnd();
	BOOL ultraJumpEnd();
	BOOL uTurnJumpEnd();
	BOOL jumpThrowEnd();
	BOOL fireJumpEnd();
	BOOL broadJumpEnd();
	BOOL hipAttackEnd();
	BOOL brakeEnd();
	BOOL slipEnd();
	BOOL waitMain();

	// Swim stuff
	void isSwimWaiting();
	void doSwimming();
	BOOL checkSwimJump();
	BOOL checkSwimToHangFence();
	BOOL swimStart();
	BOOL swimWait();
	BOOL swimWaitToPaddle();
	BOOL swimPaddleStart();
	BOOL swimPaddle();
	BOOL swimPaddleEnd();
	BOOL swimPaddleEndToWait();
	BOOL swimUp();
	BOOL swimDive();
	BOOL swimPDamage();
	BOOL swimDown();
	BOOL swimPDown();
	int swimMain();

	// ?
	void setGamePad(TMarioGamePad*);
	void stageSetting();
	void resetHistory();
	void hitNoKeepPull(THitActor*);
	void hitSurfingBoard(THitActor*);
	void hitPickUpEnemy(THitActor*);
	void hangPole(THitActor*);
	void hitBrakable(THitActor*);
	void hitJumpBase(THitActor*);
	void hitBarrel(THitActor*);
	void hitWantToTake(THitActor*);
	void wantToTakeActor(THitActor*);
	void hitPool(THitActor*);
	void hitNpc(THitActor*);
	void hitMario(THitActor*);
	void hitPull(THitActor*);
	void hitPushup(THitActor*);
	void hitHipDrop(THitActor*);
	void hitNormal(THitActor*);
	void getCoinBlue();
	void getCoinRed();
	void getCoin();
	void getGesso(THitActor*);
	bool getNozzle(THitActor*, TWaterGun::TNozzleType);
	void startSoundActor(u32);
	void stopVoice();
	void startVoiceIfNoVoice(u32);
	void startVoice(u32);
	void soundTorocco();
	void soundHitBound();
	void animSound();
	void soundMovement();
	void startVoiceYoshi(u32);
	void checkStatusType(s32) const;

	// fabricated
	f32 getIntendedMag() const { return mIntendedMag; }
	f32 getIntendedYaw() const { return mIntendedYaw * (360.0f / 65536.0f); }
	THitActor* getFloorHitActor() { return &mFloorHitActor; }

	// fabricated
	bool isTouchGround4cm() const
	{
		return mPosition.y <= mFloorPosition.y + 4.0f ? true : false;
	}

	bool checkActionThing3()
	{
		if (checkFlag(0x1000))
			return true;
		if (checkStatusFlag(STATUS_FLAG_JUMPING))
			return false;
		return true;
	}

	const TBGCheckData* getGroundPlane() const { return mGroundPlane; }

	// Fabricated
	bool checkFlag(u32 attribute) const
	{
		return unk118 & attribute ? true : false;
	}

	// Fabricated
	bool fabricatedIsPumping() const
	{
		return checkFlag(MARIO_FLAG_HAS_FLUDD) && isUpperPumpingStyle() != 0;
	}

	// Fabricated
	bool checkUnk380(u32 message) const
	{
		return unk380 == message ? true : false;
	}

	// Fabricated
	bool fabricatedUnk380Inline() const
	{
		if (unk380 == 0 || unk380 == 1) {
			return true;
		}
		return false;
	}

	// Fabricated
	bool checkStatusFlag(u32 actionFlag) const
	{
		return mStatus & actionFlag ? true : false;
	}

	// TODO: rename and sort out the status category checks

	// fabricated
	bool isActionCoolOrSomethingIdk() const
	{
		if ((mStatus & STATUS_TYPE_AND_ID_MASK) >= 0x147
		    && (mStatus & STATUS_TYPE_AND_ID_MASK) <= 0x14A)
			return true;

		return false;
	}

	// Fabricated
	bool fabricatedActionInline() const
	{
		if ((mStatus & STATUS_TYPE_AND_ID_MASK) >= 0x168
		    && 0x16c >= (mStatus & STATUS_TYPE_AND_ID_MASK))
			return true;

		return false;
	}

	// Fabricated
	bool isActionThing() const
	{
		if ((mStatus & STATUS_TYPE_AND_ID_MASK) >= 0x14C
		    && 150 >= (mStatus & STATUS_TYPE_AND_ID_MASK))
			return true;

		return false;
	}

	// Fabricated and probably wrong
	void damageExec(THitActor* hittingActor, int code)
	{
		const TEParams& params = getDmgMapCode(code);

		hittingActor->mPosition.x = mPosition.x + JMASSin(mFaceAngle.y);
		hittingActor->mPosition.z = mPosition.z + JMASCos(mFaceAngle.y);
		damageExec(hittingActor, params.mDamage.get(), params.mDownType.get(),
		           params.mWaterEmit.get(), params.mMinSpeed.get(),
		           params.mMotor.get(), params.mDirty.get(),
		           params.mInvincibleTime.get());
	}

	// Fabricated
	s32 checkUnk368() const { return unk368 > 0.0f ? 1 : 0; }

	bool checkActionThing() { return mStatus & 0x1000 ? true : false; }

public:
	/* 0x74 */ u32 mInput;
	/* 0x78 */ u32 unk78;

	enum {
		STATUS_FLAG_UNK200      = 0x200,
		STATUS_FLAG_RUNNING     = 0x400,
		STATUS_FLAG_JUMPING     = 0x800,
		STATUS_FLAG_UNK1000     = 0x1000,
		STATUS_FLAG_SWIMMING    = 0x2000,
		STATUS_FLAG_UNK8000     = 0x8000,
		STATUS_FLAG_UNK10000    = 0x10000,
		STATUS_FLAG_UNK20000    = 0x20000,
		STATUS_FLAG_UNK40000    = 0x40000,
		STATUS_FLAG_UNK100000   = 0x100000,
		STATUS_FLAG_UNK800000   = 0x800000,
		STATUS_FLAG_UNK2000000  = 0x2000000,
		STATUS_FLAG_UNK4000000  = 0x4000000,
		STATUS_FLAG_UNK10000000 = 0x10000000,
		STATUS_FLAG_UNK20000000 = 0x20000000,
		STATUS_FLAG_UNK80000000 = 0x80000000,

		STATUS_TYPE_WAITING  = 0x000,
		STATUS_TYPE_RUNNING  = 0x040,
		STATUS_TYPE_JUMPING  = 0x080,
		STATUS_TYPE_SWIMMING = 0x0C0,
		STATUS_TYPE_DEMO     = 0x100,
		STATUS_TYPE_SPECIAL  = 0x140,
		STATUS_TYPE_ACTION   = 0x180,
		STATUS_TYPE_MASK     = 0x1C0,

		STATUS_RUN = STATUS_FLAG_UNK4000000 | STATUS_FLAG_RUNNING
		             | STATUS_TYPE_RUNNING | 0x0, // 0x4000440
		STATUS_ROTATE_L
		= STATUS_FLAG_RUNNING | STATUS_TYPE_RUNNING | 0x1, // 0x441
		STATUS_ROTATE_R
		= STATUS_FLAG_RUNNING | STATUS_TYPE_RUNNING | 0x2,             // 0x442
		STATUS_TURN = STATUS_FLAG_RUNNING | STATUS_TYPE_RUNNING | 0x3, // 0x443
		STATUS_TURN_END
		= STATUS_FLAG_RUNNING | STATUS_TYPE_RUNNING | 0x4, // 0x444
		STATUS_BRAKE = STATUS_FLAG_UNK4000000 | STATUS_FLAG_RUNNING
		               | STATUS_TYPE_RUNNING | 0x5, // 0x4000445
		STATUS_SURF = STATUS_FLAG_UNK800000 | STATUS_FLAG_UNK10000
		              | STATUS_FLAG_RUNNING | STATUS_TYPE_RUNNING
		              | 0x6, // 0x810446
		// TODO: rename, this is like the rail thing in pina park I think?
		STATUS_TOROCCO = STATUS_FLAG_UNK800000 | STATUS_FLAG_RUNNING
		                 | STATUS_TYPE_RUNNING | 0x7, // 0x800447
		STATUS_WALK_END = STATUS_FLAG_UNK4000000 | STATUS_FLAG_RUNNING
		                  | STATUS_TYPE_RUNNING | 0xA, // 0x400044A
		STATUS_SPEED_SLIDING
		= STATUS_FLAG_RUNNING | STATUS_TYPE_RUNNING | 0xC, // 0x44C
		STATUS_FIRE_DASH = STATUS_FLAG_UNK20000 | STATUS_FLAG_RUNNING
		                   | STATUS_TYPE_RUNNING | 0x9, // 0x20449
		STATUS_SLIP_FORE = STATUS_FLAG_UNK800000 | STATUS_FLAG_UNK40000
		                   | STATUS_FLAG_RUNNING | STATUS_TYPE_RUNNING
		                   | 0x12, // 0x840452
		STATUS_SLIP_BACK = STATUS_FLAG_UNK800000 | STATUS_FLAG_UNK40000
		                   | STATUS_FLAG_RUNNING | STATUS_TYPE_RUNNING
		                   | 0x13, // 0x453
		STATUS_CATCH = STATUS_FLAG_UNK800000 | STATUS_FLAG_RUNNING
		               | STATUS_TYPE_RUNNING | 0x16, // 0x456
		STATUS_SQUAT_SLIP = STATUS_FLAG_UNK4000000 | STATUS_FLAG_UNK800000
		                    | STATUS_FLAG_UNK8000 | STATUS_FLAG_RUNNING
		                    | STATUS_TYPE_RUNNING | 0x19, // 0x4808459
		STATUS_OIL_RUN = STATUS_FLAG_UNK40000 | STATUS_FLAG_RUNNING
		                 | STATUS_TYPE_RUNNING | 0x1C, // 0x4045C
		STATUS_OIL_SLIP = STATUS_FLAG_UNK800000 | STATUS_FLAG_UNK40000
		                  | STATUS_FLAG_RUNNING | STATUS_TYPE_RUNNING
		                  | 0x1D, // 0x84045D
		STATUS_OIL_SLOPE = STATUS_FLAG_UNK40000 | STATUS_FLAG_RUNNING
		                   | STATUS_TYPE_RUNNING | 0x1E, // 0x4045EF
		STATUS_BACK_DOWN = STATUS_FLAG_UNK20000 | STATUS_FLAG_RUNNING
		                   | STATUS_TYPE_RUNNING | 0x20, // 0x20460
		STATUS_FORE_DOWN = STATUS_FLAG_UNK20000 | STATUS_FLAG_RUNNING
		                   | STATUS_TYPE_RUNNING | 0x21, // 0x20461
		STATUS_SHORT_BACK_DOWN = STATUS_FLAG_UNK20000 | STATUS_FLAG_RUNNING
		                         | STATUS_TYPE_RUNNING | 0x22, // 0x20462
		STATUS_SHORT_FORE_DOWN = STATUS_FLAG_UNK20000 | STATUS_FLAG_RUNNING
		                         | STATUS_TYPE_RUNNING | 0x23, // 0x20463
		STATUS_SAFE_BACK_DOWN = STATUS_FLAG_UNK20000 | STATUS_FLAG_RUNNING
		                        | STATUS_TYPE_RUNNING | 0x24, // 0x20464
		STATUS_SAFE_FORE_DOWN = STATUS_FLAG_UNK20000 | STATUS_FLAG_RUNNING
		                        | STATUS_TYPE_RUNNING | 0x25, // 0x20465
		STATUS_CATCH_DOWN = STATUS_FLAG_UNK20000 | STATUS_FLAG_RUNNING
		                    | STATUS_TYPE_RUNNING | 0x26, // 0x20466
		STATUS_LOSER_DOWN = STATUS_FLAG_UNK20000 | STATUS_FLAG_RUNNING
		                    | STATUS_TYPE_RUNNING | 0x27, // 0x20467

		STATUS_JUMP = STATUS_FLAG_UNK2000000 | STATUS_FLAG_JUMPING
		              | STATUS_TYPE_JUMPING | 0x0, // 0x2000880
		STATUS_SECOND_JUMP = STATUS_FLAG_UNK2000000 | STATUS_FLAG_JUMPING
		                     | STATUS_TYPE_JUMPING | 0x1, // 0x2000881
		STATUS_ULTRA_JUMP
		= STATUS_FLAG_JUMPING | STATUS_TYPE_JUMPING | 0x2, // 0x882
		STATUS_BACK_JUMP
		= STATUS_FLAG_JUMPING | STATUS_TYPE_JUMPING | 0x3, // 0x883

		STATUS_WALL_JUMP = STATUS_FLAG_UNK2000000 | STATUS_FLAG_JUMPING
		                   | STATUS_TYPE_JUMPING | 0x6, // 0x2000886
		STATUS_U_TURN_JUMP
		= STATUS_FLAG_JUMPING | STATUS_TYPE_JUMPING | 0x7, // 0x887
		STATUS_BROAD_JUMP
		= STATUS_FLAG_JUMPING | STATUS_TYPE_JUMPING | 0x8, // 0x888
		STATUS_JUMP_SHORT_BACK_DOWN = STATUS_FLAG_UNK20000 | STATUS_FLAG_JUMPING
		                              | STATUS_TYPE_JUMPING | 0x30, // 0x208B0
		STATUS_JUMP_SHORT_FORE_DOWN = STATUS_FLAG_UNK20000 | STATUS_FLAG_JUMPING
		                              | STATUS_TYPE_JUMPING | 0x31, // 0x208B1
		STATUS_JUMP_FORE_DOWN = STATUS_FLAG_UNK20000 | STATUS_FLAG_JUMPING
		                        | STATUS_TYPE_JUMPING | 0x32, // 0x208B2
		STATUS_JUMP_BACK_DOWN = STATUS_FLAG_UNK20000 | STATUS_FLAG_JUMPING
		                        | STATUS_TYPE_JUMPING | 0x33, // 0x208B3

		STATUS_LEFT_ROTATE_JUMP
		= STATUS_FLAG_JUMPING | STATUS_TYPE_JUMPING | 0x15, // 0x895
		STATUS_RIGHT_ROTATE_JUMP
		= STATUS_FLAG_JUMPING | STATUS_TYPE_JUMPING | 0x16, // 0x896
		STATUS_WALL_SLIDE
		= STATUS_FLAG_JUMPING | STATUS_TYPE_JUMPING | 0x27, // 0x8A7
		STATUS_HIP_DROP = STATUS_FLAG_UNK800000 | STATUS_FLAG_JUMPING
		                  | STATUS_TYPE_JUMPING | 0x29, // 0x8008A9

		STATUS_WAIT            = 0xC400201,
		STATUS_SLEEPY          = 0xC400202,
		STATUS_SLEEP           = 0xC000203,
		STATUS_WAKEUP          = 0xC000204,
		STATUS_SQUAT           = 0xC008220,
		STATUS_SQUAT_START     = 0xC008221,
		STATUS_PULL_END        = 0xC00022F,
		STATUS_U_TURN_JUMP_END = 0xC000233,
		STATUS_JUMP_THROW_END  = 0x80000A36,
		STATUS_FIRE_JUMP_END   = 0x8000239,
		STATUS_ULTRA_JUMP_END  = 0x800023A,
		STATUS_BROAD_JUMP_END  = 0x800023B,
		STATUS_HIP_ATTACK_END  = 0x80023C,
		STATUS_BRAKE_END       = 0xC00023D,
		STATUS_SLIP_END        = 0xC00023E,

		STATUS_SWIM_START = STATUS_FLAG_SWIMMING | STATUS_FLAG_UNK200
		                    | STATUS_TYPE_SWIMMING | 0x11,
		STATUS_SWIM_WAIT = STATUS_FLAG_SWIMMING | STATUS_FLAG_UNK200
		                   | STATUS_TYPE_SWIMMING | 0x12,
		STATUS_SWIM_WAIT_TO_PADDLE = STATUS_FLAG_SWIMMING | STATUS_FLAG_RUNNING
		                             | STATUS_TYPE_SWIMMING | 0x13,
		STATUS_SWIM_PADDLE_START = STATUS_FLAG_SWIMMING | STATUS_FLAG_RUNNING
		                           | STATUS_TYPE_SWIMMING | 0x14,
		STATUS_SWIM_PADDLE = STATUS_FLAG_SWIMMING | STATUS_FLAG_RUNNING
		                     | STATUS_TYPE_SWIMMING | 0x15,
		STATUS_SWIM_PADDLE_END = STATUS_FLAG_SWIMMING | STATUS_FLAG_RUNNING
		                         | STATUS_TYPE_SWIMMING | 0x16,
		STATUS_SWIM_PADDLE_END_TO_WAIT = STATUS_FLAG_SWIMMING
		                                 | STATUS_FLAG_RUNNING
		                                 | STATUS_TYPE_SWIMMING | 0x17,
		STATUS_SWIM_UP = STATUS_FLAG_SWIMMING | STATUS_FLAG_RUNNING
		                 | STATUS_TYPE_SWIMMING | 0x18,
		STATUS_SWIM_DIVE = STATUS_FLAG_SWIMMING | STATUS_FLAG_RUNNING
		                   | STATUS_TYPE_SWIMMING | 0x19,
		STATUS_SWIM_P_DAMAGE = STATUS_FLAG_SWIMMING | STATUS_FLAG_RUNNING
		                       | STATUS_TYPE_SWIMMING | 0x1A,

		STATUS_BAR_WAIT              = 0x18100340,
		STATUS_BAR_HANG              = 0x10100341,
		STATUS_BAR_CLIMB             = 0x10100343,
		STATUS_KICK_ROOF_ROLL_UP     = 0x200345,
		STATUS_KICK_ROOF_ROLL_DOWN   = 0x200346,
		STATUS_KICK_ROOF             = 0x200347,
		STATUS_HANG_ROOF             = 0x8200348,
		STATUS_WAIT_ROOF             = 0x200349,
		STATUS_MOVE_ROOF             = 0x20054A,
		STATUS_HANGING               = 0x3800034B,
		STATUS_TAKEN                 = 0x10020370,
		STATUS_WIRE_WAIT             = 0x350,
		STATUS_WIRE_S_WAIT           = 0x351,
		STATUS_WIRE_WAIT_TO_S_WAIT_L = 0x352,
		STATUS_WIRE_WAIT_TO_S_WAIT_R = 0x353,
		STATUS_WIRE_HANGING          = 0x10000357,
		STATUS_WIRE_ROLLING          = 0x10000358,
		STATUS_FENCE_CATCH           = 0x3000036B,
		STATUS_FENCE_JUMP_CATCH      = 0x3000036C,
		STATUS_FENCE_PUNCH           = 0x3000036A,

		STATUS_TAKE       = 0x383,
		STATUS_TAKE_POSE  = 0x384,
		STATUS_CATCH_LOST = 0x386,
		STATUS_PUTTING    = 0x80000387,
		STATUS_PITCHING   = 0x80000588,

		STATUS_WIN_DEMO        = 0x1302,
		STATUS_READ_BILLBOARD  = 0x10001308,
		STATUS_WARP_OUT        = 0x1337,
		STATUS_WARP_IN         = 0x1336,
		STATUS_ELECTRIC_DAMAGE = 0x20338,
		STATUS_FOOT_DOWN       = 0x2033C,
		STATUS_NOMOTION        = 0x133E,
		STATUS_DISAPPEAR       = 0x133F,

		// TODO: these ones I'm not 100% sure about, or they need better names
		STATUS_THROWN_DOWN = 0x208B8,
		STATUS_DIVE_JUMP   = 0x80088A,
		STATUS_JUMP_THROW  = 0x820008AB,
		STATUS_SLIP_FALL   = 0x200088E,
		STATUS_FIRE_DOWN   = 0x208B7,
		STATUS_DIVE        = 0x891,

		STATUS_ID_MASK = 0x3F,

		STATUS_TYPE_AND_ID_MASK = 0x1FF,
	};

	// Definitions derived from gMarioAnimeData
	enum {
		ANIM_HGUP               = 0,
		ANIM_BDWN               = 1,
		ANIM_BKDWN              = 2,
		ANIM_TREE_CLIMB         = 5,
		ANIM_TREE_CATCH         = 6,
		ANIM_TREE_WAIT          = 13,
		ANIM_BRAKE              = 15,
		ANIM_HGDWN              = 28,
		ANIM_FJPEND             = 40,
		ANIM_FIREJMP            = 41,
		ANIM_SDWNF              = 44,
		ANIM_JFDWN              = 45,
		ANIM_HANG               = 51,
		ANIM_HGJMP              = 52,
		ANIM_LADDER_HANG_CATCH  = 53,
		ANIM_HIPED              = 58,
		ANIM_HIPSR              = 60,
		ANIM_HIPAT              = 61,
		ANIM_RUN1               = 72,
		ANIM_2JMED              = 75,
		ANIM_2JMP2              = 76,
		ANIM_JUMP               = 77,
		ANIM_JMPED              = 78,
		ANIM_2JMP1              = 80,
		ANIM_LAND               = 86,
		ANIM_LAEND              = 87,
		ANIM_LOST               = 90,
		ANIM_LADDER_HANG_MOVE_L = 92,
		ANIM_LADDER_HANG_MOVE_R = 93,
		ANIM_DOOR_OPENR         = 95,
		ANIM_DOOR_OPENL         = 96,
		ANIM_THROW              = 101,
		ANIM_RAISE              = 107,
		ANIM_PUSH               = 108,
		ANIM_RIDE_SHELL         = 109,
		ANIM_PUT                = 110,
		ANIM_ROLL               = 111,
		ANIM_RUN2               = 114,
		ANIM_SFBDN              = 116,
		ANIM_SFFDN              = 117,
		ANIM_SHOCK_DOWN         = 121,
		ANIM_SHOCK              = 122,
		ANIM_SDOWN              = 123,
		ANIM_SHFDN              = 124,
		ANIM_SWAIT              = 126,
		ANIM_SWLKL              = 127,
		ANIM_SWLKR              = 128,
		ANIM_SLDCT              = 136,
		ANIM_SLPBK              = 137,
		ANIM_SLDWN              = 138,
		ANIM_SLPED              = 143,
		ANIM_SLPLA              = 144,
		ANIM_SLIP               = 145,
		ANIM_SSTEP              = 146,
		ANIM_SQEND              = 150,
		ANIM_SQSTA              = 151,
		ANIM_SQWAT              = 152,
		ANIM_TURN               = 188,
		ANIM_TRNED              = 189,
		ANIM_TJMP2              = 190,
		ANIM_TJMP1              = 191,
		// Duplicate entry for TJMP2?
		// ANIM_TJMP2_UNK = 192
		ANIM_JUMP_ROLLING       = 193,
		ANIM_WAIT               = 195,
		ANIM_LADDER_HANG_WAIT_L = 198,
		ANIM_LADDER_HANG_WAIT_R = 199,
		ANIM_WALK               = 202,
		ANIM_WJMP               = 203,
		ANIM_WSLD               = 204,
		ANIM_DEMO_SHINE_GET     = 205,
		ANIM_STEP1              = 209,
		ANIM_STEP2              = 210,
		ANIM_STEP3              = 211,
		ANIM_JKICK              = 212,
		ANIM_DGRUN              = 213,
		// ANIM_LAND w/ changed parameters
		// (hand id is 1, unk7 is 0x16)
		// ANIM_LAND_UNK = 214,
		ANIM_HMOV_L       = 215,
		ANIM_HMOV_R       = 216,
		ANIM_T_WAIT       = 217,
		ANIM_HOT_WAIT     = 218,
		ANIM_ROPE_WALK    = 219,
		ANIM_ROPE_RUN     = 220,
		ANIM_ROPE_WAIT    = 221,
		ANIM_ROPE_WTOSW   = 222,
		ANIM_ROPE_WTOSW_R = 223,
		ANIM_ROPE_SWAIT   = 224,
		ANIM_ROPE_WHG     = 225,
		ANIM_ROPE_SWHG    = 226,
		ANIM_ROPE_HGWAT   = 227,
		ANIM_ROPE_RETURN  = 228,
		ANIM_ROPE_HMOVR   = 229,
		ANIM_ROPE_HMOVL   = 230,
		ANIM_SINKING      = 231,
		ANIM_SINK_DOWN    = 232,
		ANIM_DOOR_KICK    = 233,
		ANIM_HOLD         = 234,
		ANIM_HOLD_WAIT    = 235,
		ANIM_HOLD_BACK    = 236,
		ANIM_HOLD_MOVE_R  = 237,
		ANIM_HOLD_MOVE_L  = 238,
		ANIM_HOLD_DRAG    = 239,
		ANIM_HOLD_TO_HANG = 240,
		ANIM_HOLD_HANG    = 241,
		ANIM_HANG_TO_HOLD = 242,
		ANIM_HOLD_RETURN  = 243,
		ANIM_SPIN_P       = 244,
		ANIM_TURBO_DASH   = 245,
		ANIM_BROAD_JUMP   = 246,
		// ANIM_JUMP_ROLLING w/ a slightly different param
		// (hand id in it's entry is set to 0 instead of 2)
		// ANIM_JUMP_ROLLING_UNK = 247,
		ANIM_GIANT_ROLLING    = 248,
		ANIM_FENCE_CATCH      = 249,
		ANIM_FENCE_JCATCH     = 250,
		ANIM_FENCE_WAIT       = 251,
		ANIM_FENCE_MOVE_L     = 252,
		ANIM_FENCE_MOVE_R     = 253,
		ANIM_FENCE_MOVE_UP    = 254,
		ANIM_FENCE_MOVE_DOWN  = 255,
		ANIM_FENCE_PUNCH      = 256,
		ANIM_LADDER_HANG_KICK = 257,
		ANIM_LADDER_ROLL_UP   = 258,
		ANIM_LADDER_ROLL_DOWN = 259,
		// Duplicate entry for ANIM_RIDE_SHELL with the hand id set to 1
		// ANIM_RIDE_SHELL_UNK = 260
		ANIM_RIDE_SHELL_WAIT = 261,
		ANIM_SWIM_START      = 262,
		ANIM_SWIM_WAIT       = 263,
		ANIM_WAIT_TO_SWIM    = 264,
		ANIM_SWIM            = 265,
		ANIM_SWIM_TO_WAIT    = 266,
		ANIM_SWIM_DAMAGE     = 267,
		ANIM_SWIM_DOWN       = 268,
		ANIM_DEMO_GATE_IN    = 269,
		ANIM_DEMO_GATE_OUT   = 270,
		ANIM_ROLL_JUMP       = 271,
		ANIM_GET_FAIL        = 272,
		ANIM_TREE_MOVE_L     = 273,
		ANIM_TREE_MOVE_R     = 274,
		ANIM_DIE             = 275,
		ANIM_MONTEMAN_WAIT   = 276,
		// duplicate entry of ANIM_SWIM_START for paddling (?)
		ANIM_PADDLE_SWIM_START = 277,
		// duplicate entry of ANIM_SWIM_WAIT for paddling (?)
		ANIM_PADDLE_SWIM_WAIT          = 278,
		ANIM_WAIT_TO_PADDLE            = 279,
		ANIM_PADDLE_START              = 280,
		ANIM_PADDLE_SWIM               = 281,
		ANIM_PADDLE_END                = 282,
		ANIM_PADDLE_TO_WAIT            = 283,
		ANIM_FLOAT                     = 284,
		ANIM_DAMAGE_WAIT               = 285,
		ANIM_FEPMP                     = 286,
		ANIM_SWPMP                     = 287,
		ANIM_THROWN                    = 288,
		ANIM_THROWN_END                = 289,
		ANIM_BOTTLE_IN                 = 290,
		ANIM_SAND_FILL_HEAD            = 291,
		ANIM_SAND_FILL_HEAD_END        = 292,
		ANIM_SANDFILL_LEG              = 293,
		ANIM_SANDFILL_LEG_END          = 294,
		ANIM_DAMAGE_WAIT_START         = 295,
		ANIM_SWIM_DIVE                 = 296,
		ANIM_DRAW                      = 297,
		ANIM_SWIM_P_DAMAGE             = 298,
		ANIM_SWIM_P_DOWN               = 299,
		ANIM_PIVOT                     = 300,
		ANIM_DEMO_GATE_OUT_GET2        = 301,
		ANIM_DEMO_GATE_OUT_APPEAR      = 302,
		ANIM_BELT_UP                   = 303,
		ANIM_YAWN                      = 304,
		ANIM_SIT                       = 305,
		ANIM_SIT_WAIT                  = 306,
		ANIM_SIT_END                   = 307,
		ANIM_SLEEP                     = 308,
		ANIM_SLEEP_WAIT                = 309,
		ANIM_SLEEP_END                 = 310,
		ANIM_DIVE_WAIT                 = 311,
		ANIM_DIVE_LAND                 = 312,
		ANIM_DOOR_GACHA_L              = 313,
		ANIM_DOOR_GACHA_R              = 314,
		ANIM_DEMO_GATE_OUT_APPEAR_GET  = 315,
		ANIM_DEMO_GATE_OUT_ROLLING_GET = 316,
		ANIM_DEMO_GATE_OUT_ROLLING     = 317,
		ANIM_FALL_DOWN_WAIT            = 318,
		ANIM_YO_WAIT                   = 319,
		ANIM_YO_WALK                   = 320,
		ANIM_YO_RUN                    = 321,
		ANIM_YO_EAT                    = 322,
		ANIM_YO_EAT_END                = 323,
		ANIM_YO_JUMP                   = 324,
		ANIM_YO_JUMP_FALL              = 325,
		ANIM_YO_JUMP_END               = 326,
		ANIM_YO_HOLD_JUMP              = 327,
		ANIM_YO_RIDE                   = 328,
		ANIM_YO_HIP_START              = 329,
		ANIM_YO_HIP_POSE               = 330,
		ANIM_YO_HIP_END                = 331,
		ANIM_YO_DAMAGE                 = 332,
		ANIM_DEMO_SHINE_GET_YO         = 333,
		ANIM_YO_SLIDE_POSE             = 334,
		ANIM_YO_SLIDE_END              = 335,
	};

	/* 0x7C */ u32 mStatus;
	/* 0x80 */ u32 mPrevStatus;
	/* 0x84 */ u16 mStatusState;
	/* 0x86 */ u16 mStatusTimer;
	/* 0x88 */ u32 mStatusArg;

	/* 0x8C */ f32 mIntendedMag;
	/* 0x90 */ s16 mIntendedYaw;
	/* 0x92 */ u16 unk92;

	/* 0x94 */ JGeometry::TVec3<s16> mFaceAngle;
	/* 0x9A */ s16 mModelFaceAngle;
	/* 0x9C */ s16 unk9C;
	/* 0x9E */ s16 unk9E;
	/* 0xA0 */ s16 unkA0;
	/* 0xA2 */ u16 unkA2;
	/* 0xA4 */ JGeometry::TVec3<f32> mVel;

	/* 0xB0 */ f32 mForwardVel;
	/* 0xB4 */ f32 mSlideVelX;
	/* 0xB8 */ f32 mSlideVelZ;

	/* 0xBC */ f32 unkBC;
	/* 0xC0 */ f32 unkC0;
	/* 0xC4 */ s16 unkC4;
	/* 0xC6 */ char unkC6[0xD8 - 0xC6];

	/* 0xD8 */ const TBGCheckData* mWallPlane;   // TBGCheckData 0xD8
	/* 0xDC */ const TBGCheckData* mRoofPlane;   // TBGCheckData 0xDC
	/* 0xE0 */ const TBGCheckData* mGroundPlane; // TBGCheckData 0xE0
	/* 0xE4 */ const TBGCheckData* mWaterFloor;  // TBGCheckData 0xE4

	/* 0xE8 */ JGeometry::TVec3<f32> mFloorPosition;

	/* 0xF4 */ s16 mSlopeAngle;
	/* 0xF6 */ s16 unkF6;

	/* 0xF8 */ s16 mLightID;
	/* 0xFA */ u16 mAnimationId;

	/* 0xFC */ s16 unkFC;
	/* 0xFE */ s16 unkFE;

	/* 0x100 */ s16 unk100;
	/* 0x102 */ s16 unk102;
	/* 0x104 */ f32 unk104;

	/* 0x108 */ TMarioControllerWork* unk108;
	/* 0x10C */ f32 unk10C;
	/* 0x110 */ f32 unk110;

	/* 0x114 */ u16 unk114;
	/* 0x116 */ u16 unk116;
	/* 0x118 */ u32 unk118; // some flag / attribute

	/* 0x11C */ u32 unk11C;

	/* 0x120 */ s16 mHealth;

	/* 0x122 */ u16 unk122;

	/* 0x124 */ u16 unk124;
	/* 0x126 */ u16 unk126;
	/* 0x128 */ s16 unk128;
	/* 0x12C */ f32 unk12C;
	/* 0x130 */ f32 unk130;
	/* 0x134 */ f32 unk134; // Amount of dirty?
	/* 0x138 */ f32 unk138;
	/* 0x13C */ s16 unk13C;
	/* 0x140 */ u32 unk140;
	/* 0x144 */ u32 unk144;
	/* 0x148 */ THitActor* unk148;
	/* 0x14C */ s16 unk14C;
	/* 0x14E */ s16 unk14E;
	/* 0x150 */ s16 unk150;
	/* 0x154 */ TWaterEmitInfo* unk154;
	/* 0x158 */ TWaterEmitInfo* unk158;
	/* 0x15C */ f32 unk15C;
	/* 0x160 */ JGeometry::TVec3<f32>
	    unk160[4]; // Bone position, probably larger array
	/* 0x190 */ f32 unk190;
	/* 0x194 */ f32 unk194;
	/* 0x198 */ f32 unk198;
	/* 0x19C */ JGeometry::TVec3<f32> unk19C; // damage pos
	/* 0x1A8 */ char unk1A8[0xC];
	/* 0x1B4 */ JGeometry::TVec3<f32> unk1B4;
	/* 0x1C0 */ Mtx unk1C0;
	/* 0x1F0 */ Mtx unk1F0;
	/* 0x220 */ Mtx unk220;
	/* 0x250 */ Mtx unk250;
	/* 0x280 */ char unk280[0x29C - 0x280];
	/* 0x29C */ JGeometry::TVec3<f32> unk29C;
	/* 0x2A8 */ JGeometry::TVec3<f32> unk2A8;
	/* 0x2B4 */ S16Vec unk2B4;
	/* 0x2BA */ s16 unk2BA;
	/* 0x2BC */ f32 unk2BC;
	/* 0x2C0 */ const TLiveActor* unk2C0;
	/* 0x2C4 */ Mtx unk2C4;
	/* 0x2F4 */ JGeometry::TVec3<f32> unk2F4;
	/* 0x300 */ JGeometry::TVec3<f32> unk300;
	/* 0x30C */ f32 unk30C;
	/* 0x310 */ char unk310[0x4];
	/* 0x314 */ f32 unk314;
	/* 0x318 */ Mtx unk318;
	/* 0x348 */ f32 unk348;
	/* 0x34C */ u16 unk34C;
	/* 0x34E */ u16 unk34E;
	/* 0x350 */ s32 unk350;
	/* 0x354 */ char unk354[0x360 - 0x354];
	/* 0x360 */ s16 unk360;
	/* 0x362 */ s16 unk362;
	/* 0x364 */ char unk364[0x2];
	/* 0x366 */ s16 unk366;
	/* 0x368 */ f32 unk368;
	/* 0x36C */ f32 unk36C;
	/* 0x370 */ f32 unk370;
	/* 0x374 */ f32 unk374;
	/* 0x378 */ f32 unk378;
	/* 0x37C */ u16 unk37C;
	/* 0x37E */ u16 unk37E;        // some kind of timer for the pump?
	/* 0x380 */ u32 unk380;        // pump state?
	/* 0x384 */ THitActor* unk384; // Last receiveMessage sender
	/* 0x388 */ u8 unk388;
	// TODO: Make enum (0 = red, 1 = yellow, 2 = green)
	/* 0x389 */ u8 unk389; // Blooper color
	/* 0x38A */ u16 unk38A;
	/* 0x38C */ f32 mHolderHeightDiff;
	/* 0x390 */ u32 unk390;
	/* 0x394 */ J3DDrawBuffer* unk394;
	/* 0x398 */ J3DDrawBuffer* unk398;
	/* 0x39C */ u32 unk39C;
	/* 0x3A0 */ u32 unk3A0;
	/* 0x3A4 */ SampleCtrlModelData* unk3A4;
	/* 0x3A8 */ M3UModelMario* mModel;        // Full model data
	/* 0x3AC */ J3DModelData* mBodyModelData; // Body model data
	/* 0x3B0 */ J3DModel* mHandModels[2][2];  // Hand models
	/* 0x3C0 */ J3DModel* mRHand4ndModel;
	/* 0x3C4 */ u8 unk3C4;       // Cemter Anm mtx idx
	/* 0x3C5 */ u8 mBoneIDs[12]; // Array of bone ids
	/* 0x3D1 */ u8 unk3D1;
	/* 0x3D2 */ u8 unk3D2;
	/* 0x3D3 */ u8 unk3D3;
	/* 0x3D4 */ u16 unk3D4; // _mat_eye_L idx
	/* 0x3D6 */ u16 unk3D6; // _mat_eye_R idx
	/* 0x3D8 */ f32 unk3D8;
	/* 0x3DC */ f32 unk3DC;
	/* 0x3E0 */ TMarioCap* mCap;

	/* 0x3E4 */ TWaterGun* mWaterGun;

	/* 0x3E8 */ u32 unk3E8;
	/* 0x3EC */ u32 unk3EC;

	/* 0x3F0 */ TYoshi* mYoshi;
	/* 0x3F4 */ MActor* mSurfGesso;
	/* 0x3F8 */ MActor* mTorocco;                  // pinna rail coaster actor
	/* 0x3FC */ MActor* mPinaRail;                 // Pinna_rail model actor
	/* 0x400 */ MActor* mKoopaRail;                // Koopa_rail model actor
	/* 0x404 */ JGeometry::TVec3<f32> mToroccoPos; // position of coaster
	/* 0x410 */ s16 mToroccoAngle;                 // angle of coaster
	/* 0x412 */ s16 mRailType;                     // type of rail
	/* 0x414 */ JGeometry::TVec3<f32> unk414;
	/* 0x420 */ TMultiMtxEffect* mMultiMtxEffect;
	/* 0x424 */ void* mMarioEffect; // TMarioEffect*
	/* 0x428 */ JGeometry::TVec3<f32> mWireStartPos;
	/* 0x434 */ JGeometry::TVec3<f32> mWireEndPos;
	/* 0x440 */ f32 mWirePosRatio; // ratio from 0.0 to 1.0 of mario on the wire
	/* 0x444 */ f32 mWireBounceVel;     // current wire bounce velocity
	/* 0x448 */ f32 mWireBounceVelPrev; // previous wire bounce velocity
	/* 0x44C */ f32 mWireSag;           // wire Y sag amount
	/* 0x450 */ JGeometry::TVec3<f32>
	    mMarioScreenPos; // position of mario on screen
	/* 0x45C */ JGeometry::TVec3<f32>
	    mWarpInDir; // direction of the warp in effect
	/* 0x468 */ f32 unk468;
	/* 0x46C */ f32 unk46C;
	/* 0x470 */ ResTIMG* mBodyPollutionTex;
	/* 0x474 */ THitActor mFloorHitActor;
	/* 0x4DC */ MAnmSound* mAnmSound;
	/* 0x4E0 */ JAIAnimeSound** mAnmSoundTbl;
	/* 0x4E4 */ JAISound* mSound;
	/* 0x4E8 */ u32 mSoundFlags;
	/* 0x4EC */ s8 unk4EC; // Bool if should do draw logic?
	/* 0x4ED */ u8 mBlendLogicOp;
	/* 0x4EE */ u16 mWaterWakeAlpha; // should be verified
	/* 0x4F0 */ JGeometry::TVec3<f32> unk4F0;
	/* 0x4FC */ TMarioGamePad* mGamePad;
	/* 0x500 */ TMarioSoundValues mSoundValues;
	/* 0x530 */ s16* unk530;
	/* 0x534 */ u8 unk534;
	/* 0x535 */ u8 unk535;
	/* 0x536 */ s16 unk536;
	/* 0x538 */ s16 unk538;
	/* 0x53A */ u8 unk53A;
	/* 0x53B */ u8 unk53B;
	/* 0x53C */ TTrembleModelEffect* mTrembleModelEffect;
	/* 0x540 */ f32 mWireSfx0MinVel; // minimum velocity for wire sfx0
	/* 0x544 */ f32 mWireSfx1MinVel; // minimum velocity for wire sfx1
	/* 0x548 */ u32 mWireQueuedSfxID;
	/* 0x54C */ u8 mWireSfxTimer;
	/* 0x54D */ u8 mWireSfxDelay; // initial value for mWireSfxTimer
	/* 0x54E */ s16 unk54E;
	/* 0x550 */ s16 mWireSwingPosAngle; // positive angle on wire to play sfx
	/* 0x552 */ s16 mWireSwingNegAngle; // negative angle on wire to play sfx
	/* 0x554 */ s16 mWireRollAngle;     // roll angle to play roll sfx on wire
	/* 0x556 */ s16 unk556;
	/* 0x558 */ int
	    mCoinCount; // number of coins mario has collected (write only?)
	/* 0x55C */ f32
	    unk55C; // might be something like body radius but I'm not sure enough
	/* 0x560 */ f32 unk560;
	/* 0x564 */ f32 unk564;
	/* 0x568 */ f32 unk568;
	/* 0x56C */ f32 unk56C;
	/* 0x570 */ f32 unk570;

	/* 0x574 */ TDeParams mDeParams;

	// TODO: Should these be an array indexed by an enum?
	/* 0x998 */ TBodyAngleParams mBodyAngleParamsFree;
	/* 0xA18 */ TBodyAngleParams mBodyAngleParamsWaterGun;

	/* 0xA98 */ TAttackParams mAttackParamsFencePunch;
	/* 0xAC8 */ TAttackParams mAttackParamsKickRoof;
	/* 0xAF8 */ TJumpParams mJumpParams;
	/* 0xE98 */ TRunParams mRunParams;
	/* 0x1044 */ TSwimParams mSwimParams;
	/* 0x1290 */ THangingParams mHangingParams;
	/* 0x1310 */ THangRoofParams mHangRoofParams;
	/* 0x132C */ TWireParams mWireParams;

	// TODO: Should these be an array indexed by an enum?
	/* 0x1474 */ TPullParams mPullParamsBGBeak;
	TPullParams mPullParamsBGTentacle;
	TPullParams mPullParamsBGFireWanWanBossTail;
	TPullParams mPullParamsFireWanWanTail;

	TBarParams mBarParams;

	/* 0x1640 */ TSurfingParams mSurfingParamsWaterRed;
	TSurfingParams mSurfingParamsGroundRed;
	TSurfingParams mSurfingParamsWaterYellow;
	TSurfingParams mSurfingParamsGroundYellow;
	TSurfingParams mSurfingParamsWaterGreen;
	TSurfingParams mSurfingParamsGroundGreen;

	/* 0x2138 */ THHoverParams mHoverParams;
	/* 0x217C */ TDivingParams mDivingParams;
	/* 0x21E8 */ TYoshiParams mYoshiParams;
	/* 0x22A4 */ TWaterEffectParams mWaterEffectParams;
	/* 0x2338 */ TControllerParams mControllerParams;
	/* 0x2408 */ TGraffitoParams mGraffitoParams;
	/* 0x25B4 */ TDirtyParams mDirtyParams;
	/* 0x279C */ TMotorParams mMotorParams;
	TParticleParams mParticleParams;
	TEffectParams mEffectParams;

	// TODO: Should these be an array indexed by an enum?
	/* 0x2944 */ TSlipParams mSlipParamsNormal;
	/* 0x2A28 */ TSlipParams mSlipParamsOil;
	/* 0x2B0C */ TSlipParams mSlipParamsAll;
	/* 0x2BF0 */ TSlipParams mSlipParamsAllSlider;
	/* 0x2CD4 */ TSlipParams mSlipParams45;
	/* 0x2DB8 */ TSlipParams mSlipParamsWaterSlope;
	/* 0x2E9C */ TSlipParams mSlipParamsWaterGround;
	/* 0x2F80 */ TSlipParams mSlipParamsYoshi;

	TUpperParams mUpperBodyParams;

	// TODO: Should these be an array indexed by an enum?
	/* 0x30D0 */ TEParams mDmgParamsEnemyCommon;
	TEParams mDmgParamsHamakuri;
	TEParams mDmgParamsNamekuri;
	TEParams mDmgParamsHinokuri;
	TEParams mDmgParamsFire;
	TEParams mDmgParamsBGTentacle;
	TEParams mDmgParamsBossEel;
	TEParams mDmgParamsHanachanBoss;
	TEParams mDmgParamsPoihana;
	TEParams mDmgParamsKiller;
	TEParams mDmgParamsLampTrapIron;
	TEParams mDmgParamsLampTrapSpike;
	TEParams mDmgParamsEnemyMario;
	TEParams mDmgParamsCannotBreath;
	TEParams mDmgParamsGraffitoFire;
	TEParams mDmgParamsGraffitoPoison;
	TEParams mDmgParamsGraffitoElec;
	TEParams mDmgParamsGraffitoLava;
	TEParams mDmgParamsWaterSurface;

	/* 0x3BCC */ TEParams mDmgMapParams0;
	/* 0x3C60 */ TEParams mDmgMapParams1;
	TEParams mDmgMapParams2;
	TEParams mDmgMapParams3;
	TEParams mDmgMapParams4;
	TEParams mDmgMapParams5;
	TEParams mDmgMapParams6;
	TEParams mDmgMapParams7;
	TEParams mDmgMapParams8;
	TEParams mDmgMapParams9;

	/* 0x4194 */ TAutoDemoParams mAutoDemoParams;
	/* 0x4228 */ TSoundParams mSoundParams;
	/* 0x4244 */ TOptionParams mOptionParams;

	char unk4290[0x80];
};

extern TMario* gpMarioOriginal;
extern TMario* gpMarioForCallBack;

#endif
