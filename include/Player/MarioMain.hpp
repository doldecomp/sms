#ifndef MARIOMAIN_HPP
#define MARIOMAIN_HPP

#include <JSystem/JGeometry.hpp>
#include <System/Params.hpp>
#include <System/ParamInst.hpp>
#include <System/DrawSyncCallback.hpp>
#include <Strategic/TakeActor.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DDrawBuffer.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <M3DUtil/M3UModelMario.hpp>
#include <Player/Yoshi.hpp>

class TLiveActor;
class TWaterGun;
class TBGCheckData;
class J3DAnmTexPattern;
class J3DModelData;
class J3DAnmTransform;
struct TBGWallCheckRecord;

// TODO: where should this be?
enum E_SIDEWALK_TYPE {};

enum E_MARIO_FLAG {
	MARIO_FLAG_ABOVE_SEWER_FLOOR   = (1 << 0),
	MARIO_FLAG_VISIBLE             = (1 << 1),
	MARIO_FLAG_NPC_TALKING         = (1 << 3),
	MARIO_FLAG_RECENTLY_LEFT_WATER = (1 << 4),
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

struct TRidingInfo {
	const TLiveActor* unk0;
	Vec localPos;
	f32 unk10;
	// maybe more
};

class TMarioGamePad;

class TMario : public TTakeActor, public TDrawSyncCallback {
public:
	struct JumpSlipRecord;

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
		TParamRT<s32> mSwingRate;
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
		TParamRT<f32> mDamageFallHeight;
		TParamRT<f32> mForceSlipAngle;
		TParamRT<f32> mClashSpeed;
		TParamRT<f32> mHangWallMovableAngle;
		TParamRT<f32> mColMvMax;
		TParamRT<s16> mNoFreezeTime;
		TParamRT<s16> mKickFreezeTime;
		TParamRT<s16> mSurfStartFreezeTime;
		TParamRT<f32> mSleepingCheckDist;
		TParamRT<f32> mSleepingCheckHeight;
		TParamRT<s16> mIllegalPlaneCtInc;
		TParamRT<s16> mIllegalPlaneTime;
	};

public:
	TMario();

	virtual ~TMario() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual MtxPtr getTakingMtx();
	virtual bool moveRequest(const JGeometry::TVec3<f32>&);

	virtual void drawSyncCallback(u16);
	virtual void initValues();
	virtual void checkReturn();
	virtual void checkController(JDrama::TGraphics*);
	virtual void playerControl(JDrama::TGraphics*);
	virtual void initModel();
	virtual void drawSpecial(JDrama::TGraphics*);
	virtual void checkCollision();
	virtual void damageExec(THitActor*, int, int, int, f32, int, f32, s16);
	virtual void getVoiceStatus();

	bool actnMain();
	void pitching();
	void putting();
	void catchLost();
	void takePose();
	bool taking();
	void demoMain();
	void disappear();
	void nomotion();
	void footDowning();
	void electricDamage();
	void warpOut();
	void toroccoStart();
	void waitingStart(const JGeometry::TVec3<f32>*, f32);
	void returnStart(const JGeometry::TVec3<f32>*, f32, bool, int);
	void rollingStart(const JGeometry::TVec3<f32>*, f32);
	void startCommon(const JGeometry::TVec3<f32>*, f32);
	void isUnUsualStageStart();
	void warpIn();
	void downLoser();
	void sinkLoser();
	void openDoor();
	void jumpingDemoCommon(u32, int, f32);
	void elecDowning();
	void bottleIn();
	void readBillboard();
	BOOL winDemo();
	void considerTake();
	void calcDamagePos(const JGeometry::TVec3<f32>&);
	void floorDamageExec(const TMario::TEParams&);
	void floorDamageExec(int, int, int, int);
	void loserExec();
	void normalizeNozzle();
	void resetNozzle();
	void trampleExec(THitActor*);
	void canTake(THitActor*);
	bool isTakeSituation(THitActor*);
	void dropObject();
	void getAttackAngle(const THitActor*);
	void decHP(int);
	void incHP(int);
	void rumbleStart(int, int);
	void thinkDashEffect();
	void addDamageFog(JDrama::TGraphics*);
	void addDirty();
	void boxDrawPrepare(f32 (*)[4]);
	void drawLogic();
	void entryModels(JDrama::TGraphics*);
	void calcView(JDrama::TGraphics*);
	void calcAnim(u32, JDrama::TGraphics*);
	void removeCallBack();
	void addUpper();
	void setUpperDamageRun();
	void addCallBack(JDrama::TGraphics*);
	void calcAnimHands();
	void calcAnimBody(u32, JDrama::TGraphics*);
	void calcBaseMtx(f32 (*)[4]);
	void calcBaseMtxSwim(f32 (*)[4]);
	void calcBaseMtxPole(f32 (*)[4]);
	void calcBaseMtxTorocco(f32 (*)[4]);
	void considerWaist();
	void isUpperPumpingStyle() const;
	void finalDrawInitialize();
	void initMirrorModel();
	void loadAnmTexPattern(J3DAnmTexPattern**, char*, J3DModelData*);
	void loadBas(void**, const char*);
	void loadAnm(J3DAnmTransform**, const char*);
	void setReverseAnimation(int, f32);
	void setAnimation(int, f32);
	void changeHandByRate(f32);
	void changeHand(int);
	void isAnimeLoopOrStop();
	BOOL isLast1AnimeFrame();
	void getMotionFrameCtrl();
	void getCurrentFrame(int);
	void getRailMtx() const;
	void getTakenMtx();
	void calcBodyPos(JGeometry::TVec3<f32>*);
	void getTrampleCt();
	void setPositions();
	void takeOffGlass();
	void wearGlass();
	bool isWearingHelm();
	bool isWearingCap();
	void setDivHelm();
	void getWallAngle() const;
	f32 getPumpFrame() const;
	void getCenterAnmMtx();
	void getRootAnmMtx();
	void getHeadRot();
	void getJumpIntoWaterModelData();
	void jumpMain();
	void fallDead();
	void diving();
	void hipAttacking();
	void pullJumping();
	void wireJumping();
	void rotateJumping();
	void rocketing();
	void rocketCheck();
	void boardJumping();
	void rotateBroadJumping();
	void broadJumping();
	void fireLanding();
	void fireJumping();
	void missJumping();
	void trample();
	void thrownDowning();
	void fireDowning();
	void slipFalling();
	void catchStop();
	void stayWall();
	void landSafeDown();
	void jumpForeDown();
	void jumpBackDown();
	void jumpShortForeDown();
	void jumpShortBackDown();
	void checkWallJumping();
	void jumpDownCommon(int, int, f32);
	void jumpingThrow();
	void jumpCatch();
	void jumpWall();
	void uTurnJumping();
	void landing();
	void backJumping();
	void ultraJumping();
	void secJumping();
	void jumping();
	void jumpingCommonEvents();
	void checkBackTrig();
	void considerJumpRotate();
	void jumpingBasic(int, int, int);
	void askStrongGroundTouch();
	void doJumping();
	void setJumpingAttackArea();
	void doSpinJumping();
	void doSlipJumping();
	void checkJumpingThrowStart();
	void startJumpWall();
	void thinkAloha();
	void thinkCube();
	void thinkFreeze();
	void isMario();
	void gunExec();
	void checkWet();
	void thinkSound();
	void thinkTorocco();
	void thinkDiving();
	void thinkYoshiHeadCollision();
	void checkYoshiGetOff();
	void getOffYoshi(bool);
	void thinkParams();
	void thinkSand();
	void thinkWaterSurface();
	void thinkSituation();
	void calcGroundMtx(const JGeometry::TVec3<f32>&);
	void view1stMove();
	void talkMove();
	void canReadBillboard(int);
	void stateMachine();
	void checkPlayerAction(JDrama::TGraphics*);
	void checkRideReCalc();
	void checkRideMovement();
	void getActorMtx(const THitActor&, f32 (*)[4]);
	void checkCurrentPlane();
	void getDmgMapCode(int) const;
	void checkGroundPlane(f32, f32, f32, f32*, const TBGCheckData**);
	void makeHistory();
	void checkStickSmash();
	void checkStickRotate(int*);
	void getLRLevel(unsigned char);
	void getDizzyPower();
	void getDizzyAngle();
	void checkThrowObject();
	void doReturn();
	void checkEnforceJump();
	void checkSink();
	void thinkHeight();
	void thinkDirty();
	void dirtyLimitCheck();
	void checkGraffito();
	void checkGraffitoElec();
	void checkGraffitoSlip();
	void checkGraffitoLava();
	void checkGraffitoFire();
	void checkGraffitoDamage();
	void makeGraffitoDamage(const TMario::TEParams&);
	void checkAllMotions();
	bool changePlayerDropping(u32, u32);
	bool changePlayerJumping(u32, u32);
	void changePlayerTriJump();
	bool changePlayerStatus(u32, u32, bool);
	void throwMario(const JGeometry::TVec3<f32>&, f32);
	void setStatusToRunning(u32, u32);
	void setStatusToJumping(u32, u32);
	void setPlayerJumpSpeed(f32, f32);
	void setMissJumping();
	void isTurnning();
	void isTurnStart();
	void checkPlayerAround(int, f32);
	void isJumpMiss();
	void isSlipLimit();
	void getSlideStopCatch();
	void getSlideStopNormal();
	void canSlipJump();
	void isSlipStart();
	void isFrontSlip(int);
	void checkRoofPlane(const Vec&, f32, const TBGCheckData**);
	void checkWallPlane(Vec*, f32, f32);
	void setPlayerVelocity(f32);
	void setNormalAttackArea();
	void changePos(const Vec&);
	void isSpeedZero();
	void canBendBody();
	BOOL considerRotateJumpStart();
	void addVelocity(f32);
	s32 onYoshi() const;
	void getGroundJumpPower() const;
	void windMove(const JGeometry::TVec3<f32>&);
	void flowMove(const JGeometry::TVec3<f32>&);
	void warpRequest(const JGeometry::TVec3<f32>&, f32);
	void isForceSlip();
	void getRidingMtx(f32 (*)[4]);
	void isWallInFront() const;
	bool isInvincible() const;
	void isUnderWater() const;
	void canSquat() const;
	void getJumpSlideControl() const;
	void getJumpAccelControl() const;
	BOOL jumpProcess(int);
	void fallProcess();
	void isFallCancel();
	void checkGroundAtJumping(const Vec&, int);
	void hangonCheck(const TBGCheckData*, const Vec&, const Vec&);
	void barProcess();
	void walkProcess();
	void waitProcess();
	void stopProcess();
	void checkGroundAtWalking(Vec*);
	void checkDescent();
	void keepDistance(const THitActor&, f32);
	void keepDistance(const JGeometry::TVec3<f32>&, f32, f32);
	void playerRefrection(int);
	void moveMain();
	void broadJumpSlip();
	void ultraJumpSlip();
	void uTurnJumpSlip();
	void secJumpSlip();
	void landSlip();
	void jumpSlip();
	void jumpSlipEvents(TMario::JumpSlipRecord*);
	void jumpSlipCommon(short, u32);
	void loserDown();
	void catchDown();
	void safeForeDown();
	void safeBackDown();
	void shortForeDown();
	void shortBackDown();
	void foreDown();
	void backDown();
	void downingCommon(int, f32, int);
	void oilSlope();
	void oilSlip();
	void oilRun();
	void squatSlipping();
	void catching();
	void slipBack();
	void slipBackCommon(int, int, int);
	void slipFore();
	void slipForeCommon(int, int, int, int);
	void slippingBasic(int, int, int);
	void fireDashing();
	void speedSliding();
	void walkEnd();
	void toroccoing();
	void surfing();
	void braking();
	void turnEnd();
	void turnning();
	void rotating();
	void running();
	void doPushingAnimation(const Vec&);
	void changePlayerWaiting();
	void doBraking(f32);
	void doSurfing();
	void getSurfingParamsGround();
	void getSurfingParamsWater();
	void doRunning();
	void doStopping();
	void doSlipping(f32);
	void slopeProcess();
	void doSliding(f32);
	void slideProcess(f32, f32);
	void getSlideStickMult();
	void getChangeAngleSpeed();
	void getSlopeSlideAccele(f32*, f32*);
	void getSlopeNormalAccele(f32*, f32*);
	void doRunningAnimation();
	void getRunningInWaterBrake();
	void isRunningInWater();
	void changePlayerCatching();
	void isRunningTurnning();
	void isRunningSlipStart();
	void changePlayerPower(f32, u32, u32);
	void clashStandard(u32, u32);
	void postureControl();
	void isThrowStart();
	void considerRotateStart();
	void specMain();
	void fencePunch();
	void fenceMove();
	void fenceJumpCatch();
	void fenceCatch();
	void fenceFootCheck();
	void pulling();
	void setPullingAnm(const JGeometry::TVec3<f32>&, f32);
	void getCurrentPullParams(f32*, f32*);
	void wireSWaitToWaitR();
	void wireSWaitToWaitL();
	void wireRolling();
	void getNozzleEmitVX();
	void wireHanging();
	void wireReturn();
	void wireSWaitToHang();
	void wireWaitToHang();
	void changeWireHanging();
	void wireWaitToSWaitR();
	void wireWaitToSWaitL();
	void wireSWait();
	void wireWait();
	void wireMove(f32);
	void getOnWirePosAngle(JGeometry::TVec3<f32>*, short*);
	void taken();
	void hangJumping();
	void descend();
	void ascend();
	void hanging();
	void findNearestWall(const TBGWallCheckRecord&);
	void hangingCommon(int, int);
	void startHangLanding(u32);
	void kickRoofRollDown();
	void kickRoofRollUp();
	void kickRoof();
	void moveRoof();
	void waitRoof();
	void hangRoof();
	void roofCommonEvents();
	void doRoofWaitingProcess();
	void doRoofMovingProcess();
	void hangingCheckRoof(JGeometry::TVec3<f32>*);
	void barHang();
	void barClimb();
	void barWait();
	void barJumpSetting();
	void stateMachineUpper();
	void checkPumpEnable();
	void checkPumping();
	u32 askJumpIntoWaterEffectExist() const;
	void sinkInSandEffect();
	void kickFruitEffect();
	void toroccoEffect();
	void sleepingEffectKill();
	void sleepingEffect();
	void kickRoofEffect();
	void elecEndEffect();
	void elecEffect();
	void rocketEffectStart();
	void rocketEffectNozzle();
	void meltInWaterEffect();
	void emitSandEffect();
	void emitDirtyFootPrint();
	void emitFootPrint(int);
	void emitFootPrintWithEffect(int, int);
	void setFootPrint(const JGeometry::TVec3<f32>&, int);
	void emitRotateShootEffect();
	void emitBlurSpinJump();
	void emitBlurHipDropSuper();
	void emitBlurHipDrop();
	void warpOutEffect(int, f32);
	void warpInLight();
	void warpInEffect();
	void surfingEffect();
	void frontSlipEffect();
	void treeSlipEffect();
	void wallSlipEffect();
	void blurEffect();
	void runningRippleEffect();
	void smallRippleEffect(JGeometry::TVec3<f32>*);
	void swimmingBubbleEffect();
	void bubbleFromBody();
	void bubbleFromMouth(int);
	void inOutWaterEffect(f32);
	void rippleEffectSmall();
	void rippleEffect();
	void smallTouchDownEffect();
	void strongTouchDownEffect();
	void strongTouchDownEffectDisp();
	void emitGetCoinEffect(JGeometry::TVec3<f32>*);
	void emitGetWaterEffect();
	void emitGetEffect();
	void emitRecover();
	void emitSweatSometimes(short);
	void emitSweatSometimes();
	void emitSweat(short);
	void emitSmoke(short);
	void emitParticle(int, short);
	void emitParticle(int, const JGeometry::TVec3<f32>*);
	void emitParticle(int);
	void moveParticle();
	void initParticle();
	void waitMain();
	void slipEnd();
	void brakeEnd();
	void hipAttackEnd();
	void broadJumpEnd();
	void fireJumpEnd();
	void jumpThrowEnd();
	void uTurnJumpEnd();
	void ultraJumpEnd();
	void landEnd();
	void secJumpEnd();
	void jumpEnd();
	void jumpEndEvents(u32);
	void jumpEndCommon(int, int);
	void pullEnd();
	void squatStandup();
	void squatStart();
	void squating();
	void getSideWalkValues(E_SIDEWALK_TYPE*, f32*, f32*);
	void wakeup();
	void sleeping();
	void sleepily();
	void waiting();
	void changeMontemanWaitingAnim();
	void stopCommon(int, int);
	void waitingCommonEvents();
	void checkPutStart();
	void canPut();
	void canSleep();
	void startTalking();
	void swimMain();
	void swimPDown();
	void swimDown();
	void swimPDamage();
	void swimDive();
	void swimUp();
	void swimPaddleEndToWait();
	void swimPaddleEnd();
	void swimPaddle();
	void swimPaddleStart();
	void swimWaitToPaddle();
	void swimWait();
	void swimStart();
	void checkSwimToHangFence();
	void checkSwimJump();
	void doSwimming();
	void isSwimWaiting();
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
	// TODO: TWaterGun
	// void getNozzle(THitActor*, TWaterGun::TNozzleType);
	void startSoundActor(u32);
	void stopVoice();
	void startVoiceIfNoVoice(u32);
	void startVoice(u32);
	void soundTorocco();
	void soundHitBound();
	void animSound();
	void soundMovement();
	void startVoiceYoshi(u32);
	void checkStatusType(long) const;

	// fabricated
	f32 getIntendedMag() const { return mIntendedMag; }
	f32 getIntendedYaw() const { return mIntendedYaw * (360.0f / 65536.0f); }

	// fabricated
	bool isTouchGround4cm() const
	{
		return mFloorPosition.y + 4.0f <= mPosition.y ? true : false;
	}

	// fabricated
	bool isActionCoolOrSomethingIdk() const
	{
		if ((mAction & 0x1FF) >= 0x147 && (mAction & 0x1FF) <= 0x14A)
			return true;

		return false;
	}

	TBGCheckData* getGroundPlane() const { return mGroundPlane; }

	// Fabricated
	bool checkFlag(u32 attribute) const
	{
		return unk118 & attribute ? true : false;
	}

	// Fabricated
	bool fabricatedActionInline() const
	{
		if (mAction >= 0x168 && 0x16c >= mAction) {
			return true;
		}
		return false;
	}

public:
	/* 0x74 */ u32 mInput;
	/* 0x78 */ u32 unk78;
	/* 0x7C */ u32 mAction;
	/* 0x80 */ u32 mPrevAction;
	/* 0x84 */ u16 mActionState;
	/* 0x86 */ s16 mActionTimer;
	/* 0x88 */ u32 mActionArg;
	/* 0x8C */ f32 mIntendedMag;
	/* 0x90 */ s16 mIntendedYaw;
	/* 0x92 */ u16 unk92;

	/* 0x94 */ JGeometry::TVec3<s16> mFaceAngle;
	/* 0x98 */ u16 mModelFaceAngle;
	/* 0x9C */ u32 unk9C;
	/* 0xA0 */ u32 unkA0;
	/* 0xA4 */ JGeometry::TVec3<f32> mVel;

	/* 0xB0 */ f32 mForwardVel;
	/* 0xB4 */ f32 mSlideVelX;
	/* 0xB8 */ f32 mSlideVelZ;

	/* 0xBC */ char unkBC[0x1C];

	/* 0xD8 */ TBGCheckData* mWallPlane;   // TBGCheckData 0xD8
	/* 0xDC */ TBGCheckData* mRoofPlane;   // TBGCheckData 0xDC
	/* 0xE0 */ TBGCheckData* mGroundPlane; // TBGCheckData 0xE0
	/* 0xE4 */ TBGCheckData* mWaterFloor;  // TBGCheckData 0xE4

	/* 0xE8 */ JGeometry::TVec3<f32> mFloorPosition;

	/* 0xF4 */ s16 mSlopeAngle;
	/* 0xF6 */ u16 unkF6;

	/* 0xF8 */ u16 mLightID;
	/* 0xFA */ u16 unk0FA;

	/* 0xFC */ u32 unkFC[2];

	/* 0x104 */ void* mController; // TMarioControllerWork

	/* 0x108 */ u32 unk108[4];

	/* 0x118 */ u32 unk118;

	/* 0x11C */ u32 unk11C;

	/* 0x120 */ s16 mHealth;

	/* 0x122 */ u16 unk122;

	/* 0x124 */ char unk124[0x37C - 0x124];
	/* 0x37C */ u16 unk37C;
	/* 0x37E */ u16 unk37E;
	/* 0x380 */ u32 unk380;
	/* 0x384 */ THitActor* unk384; // Last receiveMessage sender

	// TODO: Make enum (0 = red, 1 = yellow, 2 = green)
	/* 0x388 */ u16 mBlooperColor;
	/* 0x38A */ u16 unk38A;
	/* 0x38C */ f32 mHolderHeightDiff;
	/* 0x390 */ u32 unk390;
	/* 0x394 */ J3DDrawBuffer* unk394;
	/* 0x398 */ J3DDrawBuffer* unk398;
	/* 0x39C */ u32 unk39C;
	/* 0x3A0 */ u32 unk3A0;
	/* 0x3A4 */ u32 unk3A4;
	/* 0x3A8 */ M3UModelMario* unk3A8; // Full model data
	/* 0x3AC */ J3DModelData* unk3AC;  // Body model data
	/* 0x3B0 */ J3DModel* unk3B0;      // R Hand 2nd model
	/* 0x3B4 */ J3DModel* unk3B4;      // L Hand 2nd model
	/* 0x3B8 */ J3DModel* unk3B8;      // R Hand 3nd model
	/* 0x3BC */ J3DModel* unk3BC;      // L Hand 3nd model
	/* 0x3C0 */ J3DModel* unk3C0;      // R Hand 4nd model
	/* 0x3C4 */ u8 unk3C4;
	/* 0x3C5 */ u8 unk3C5[12]; // Array of bone ids
	/* 0x3D1 */ u8 unk3D1;
	/* 0x3D2 */ u8 unk3D2;
	/* 0x3D3 */ u8 unk3D3;
	/* 0x3D4 */ u16 unk3D4;
	/* 0x3D6 */ u16 unk3D6;
	/* 0x3D8 */ f32 unk3D8;
	/* 0x3DC */ f32 unk3DC;
	/* 0x3E0 */ void* mCap; // TMarioCap

	/* 0x3E4 */ TWaterGun* mWaterGun;

	/* 0x3E8 */ u32 unk3E8;
	/* 0x3EC */ u32 unk3EC;

	/* 0x3F0 */ TYoshi* mYoshi;

	/* 0x3F4 */ char unk3F4[0x0FC];

	/* 0x4F0 */ JGeometry::TVec3<f32> unk4F0;

	/* 0x4FC */ TMarioGamePad* mGamePad;

	/* 0x500 */ char unk500[0x74];

	/* 0x574 */ TDeParams mDeParams;

	// TODO: Should these be an array indexed by an enum?
	TBodyAngleParams mBodyAngleParamsFree;
	TBodyAngleParams mBodyAngleParamsWaterGun;

	TAttackParams mAttackParamsFencePunch;
	TAttackParams mAttackParamsKickRoof;
	TJumpParams mJumpParams;
	TRunParams mRunParams;
	TSwimParams mSwimParams;
	THangingParams mHangingParams;
	THangRoofParams mHangRoofParams;
	TWireParams mWireParams;

	// TODO: Should these be an array indexed by an enum?
	TPullParams mPullParamsBGBeak;
	TPullParams mPullParamsBGTentacle;
	TPullParams mPullParamsBGFireWanWanBossTail;
	TPullParams mPullParamsFireWanWanTail;

	TBarParams mBarParams;

	TSurfingParams mSurfingParamsWaterRed;
	TSurfingParams mSurfingParamsGroundRed;
	TSurfingParams mSurfingParamsWaterYellow;
	TSurfingParams mSurfingParamsGroundYellow;
	TSurfingParams mSurfingParamsWaterGreen;
	TSurfingParams mSurfingParamsGroundGreen;

	THHoverParams mHoverParams;
	TDivingParams mDivingParams;
	TYoshiParams mYoshiParams;
	TWaterEffectParams mWaterEffectParams;
	TControllerParams mControllerParams;
	TGraffitoParams mGraffitoParams;
	TDirtyParams mDirtyParams;
	TMotorParams mMotorParams;
	TParticleParams mParticleParams;
	TEffectParams mEffectParams;

	// TODO: Should these be an array indexed by an enum?
	TSlipParams mSlipParamsNormal;
	TSlipParams mSlipParamsOil;
	TSlipParams mSlipParamsAll;
	TSlipParams mSlipParamsAllSlider;
	TSlipParams mSlipParams45;
	TSlipParams mSlipParamsWaterSlope;
	TSlipParams mSlipParamsWaterGround;
	TSlipParams mSlipParamsYoshi;

	TUpperParams mUpperBodyParams;

	// TODO: Should these be an array indexed by an enum?
	TEParams mDmgParamsEnemyCommon;
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

	TEParams mDmgMapParams0;
	TEParams mDmgMapParams1;
	TEParams mDmgMapParams2;
	TEParams mDmgMapParams3;
	TEParams mDmgMapParams4;
	TEParams mDmgMapParams5;
	TEParams mDmgMapParams6;
	TEParams mDmgMapParams7;
	TEParams mDmgMapParams8;
	TEParams mDmgMapParams9;

	TAutoDemoParams mAutoDemoParams;
	TSoundParams mSoundParams;
	TOptionParams mOptionParams;

	char unk4290[0x80];
};

extern TMario* gpMarioOriginal;
extern TMario* gpMarioForCallBack;

#endif
