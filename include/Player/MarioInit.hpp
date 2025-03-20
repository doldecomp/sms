#ifndef MARIOINIT_HPP
#define MARIOINIT_HPP

#include "System/ParamInst.hpp"
#include "System/Params.hpp"

// length 0x42C
class TDeParams : public TParams {
public:
	TDeParams(const char* prm)
	    : TParams(prm)
	    , PARAM_INIT(mHpMax, 8)
	    , PARAM_INIT(mRunningMax, 45.0f)
	    , PARAM_INIT(mDashMax, 60.0f)
	    , PARAM_INIT(mDashAcc, 0.5f)
	    , PARAM_INIT(mDashBrake, 0.8f)
	    , PARAM_INIT(mDashStartTime, 0x78)
	    , PARAM_INIT(mWaitingRotSp, 0x100)
	    , PARAM_INIT(mRunningRotSpMin, 0x200)
	    , PARAM_INIT(mRunningRotSpMax, 0x400)
	    , PARAM_INIT(mRocketRotSp, 0x100)
	    , PARAM_INIT(mPumpingRotSpMin, 0x100)
	    , PARAM_INIT(mPumpingRotSpMax, 0x200)
	    , PARAM_INIT(mInvincibleTime, 0x14)
	    , PARAM_INIT(mFootPrintTimerMax, 0x208)
	    , PARAM_INIT(mWaterTriggerRate, 1)
	    , PARAM_INIT(mGraffitoNoDmgTime, 10)
	    , PARAM_INIT(mRestMax, 0xff)
	    , PARAM_INIT(mShadowSize, 200.0f)
	    , PARAM_INIT(mShadowErase, 0.2f)
	    , PARAM_INIT(mHoldRadius, 100.0f)
	    , PARAM_INIT(mDamageRadius, 42.0f)
	    , PARAM_INIT(mDamageHeight, 110.0f)
	    , PARAM_INIT(mAttackHeight, 50.0f)
	    , PARAM_INIT(mTrampleRadius, 100.0f)
	    , PARAM_INIT(mPushupRadius, 100.0f)
	    , PARAM_INIT(mPushupHeight, 200.0f)
	    , PARAM_INIT(mHipdropRadius, 250.0f)
	    , PARAM_INIT(mQuakeRadius, 500.0f)
	    , PARAM_INIT(mQuakeHeight, 500.0f)
	    , PARAM_INIT(mTramplePowStep1, 30.0f)
	    , PARAM_INIT(mTramplePowStep2, 40.0f)
	    , PARAM_INIT(mTramplePowStep3, 50.0f)
	    , PARAM_INIT(mJumpWallHeight, 1000.0f)
	    , PARAM_INIT(mThrowPower, 2.0f)
	    , PARAM_INIT(mSlipStart, 0.34202015f)
	    , PARAM_INIT(mWasOnWaterSlip, 0.97f)
	    , PARAM_INIT(mInWaterSlip, 0.89999998f)
	    , PARAM_INIT(mToroccoRotSp, 1.0f)
	    , PARAM_INIT(mRecoverTimer, 0xf0)
	    , PARAM_INIT(mHotTimer, 0x4b0)
	    , PARAM_INIT(mFeelDeep, 500.0f)
	    , PARAM_INIT(mDamageFallHeight, 1000.0f)
	    , PARAM_INIT(mForceSlipAngle, 0.5f)
	    , PARAM_INIT(mClashSpeed, 50.0f)
	    , PARAM_INIT(mHangWallMovableAngle, 0.3f)
	    , PARAM_INIT(mColMvMax, 10.0f)
	    , PARAM_INIT(mNoFreezeTime, 0x78)
	    , PARAM_INIT(mKickFreezeTime, 5)
	    , PARAM_INIT(mSurfStartFreezeTime, 0x78)
	    , PARAM_INIT(mSleepingCheckDist, 50.0f)
	    , PARAM_INIT(mSleepingCheckHeight, 30.0f)
	    , PARAM_INIT(mIllegalPlaneCtInc, 4)
	    , PARAM_INIT(mIllegalPlaneTime, 0x1e0)
	{
	}

	TParamT<s16> mHpMax;
	TParamT<f32> mRunningMax;
	TParamT<f32> mDashMax;
	TParamT<f32> mDashAcc;
	TParamT<f32> mDashBrake;
	TParamT<s16> mDashStartTime;
	TParamT<s16> mWaitingRotSp;
	TParamT<s16> mRunningRotSpMin;
	TParamT<s16> mRunningRotSpMax;
	TParamT<s16> mRocketRotSp;
	TParamT<s16> mPumpingRotSpMin;
	TParamT<s16> mPumpingRotSpMax;
	TParamT<s16> mInvincibleTime;
	TParamT<s16> mFootPrintTimerMax;
	TParamT<u8> mWaterTriggerRate;
	TParamT<u8> mGraffitoNoDmgTime;
	TParamT<u8> mRestMax;
	TParamT<f32> mShadowSize;
	TParamT<f32> mShadowErase;
	TParamT<f32> mHoldRadius;
	TParamT<f32> mDamageRadius;
	TParamT<f32> mDamageHeight;
	TParamT<f32> mAttackHeight;
	TParamT<f32> mTrampleRadius;
	TParamT<f32> mPushupRadius;
	TParamT<f32> mPushupHeight;
	TParamT<f32> mHipdropRadius;
	TParamT<f32> mQuakeRadius;
	TParamT<f32> mQuakeHeight;
	TParamT<f32> mTramplePowStep1;
	TParamT<f32> mTramplePowStep2;
	TParamT<f32> mTramplePowStep3;
	TParamT<f32> mJumpWallHeight;
	TParamT<f32> mThrowPower;
	TParamT<f32> mSlipStart;
	TParamT<f32> mWasOnWaterSlip;
	TParamT<f32> mInWaterSlip;
	TParamT<f32> mToroccoRotSp;
	TParamT<s16> mRecoverTimer;
	TParamT<s16> mHotTimer;
	TParamT<f32> mFeelDeep;
	TParamT<f32> mDamageFallHeight;
	TParamT<f32> mForceSlipAngle;
	TParamT<f32> mClashSpeed;
	TParamT<f32> mHangWallMovableAngle;
	TParamT<f32> mColMvMax;
	TParamT<s16> mNoFreezeTime;
	TParamT<s16> mKickFreezeTime;
	TParamT<s16> mSurfStartFreezeTime;
	TParamT<f32> mSleepingCheckDist;
	TParamT<f32> mSleepingCheckHeight;
	TParamT<s16> mIllegalPlaneCtInc;
	TParamT<s16> mIllegalPlaneTime;
};

class TBodyAngleParams : public TParams {
public:
	TBodyAngleParams(const char* prm)
	    : TParams(prm)
	    , PARAM_INIT(mHeadRot, 0.0f)
	    , PARAM_INIT(mWaistRoll, 0.0f)
	    , PARAM_INIT(mWaistPitch, 80.0f)
	    , PARAM_INIT(mWaistRollMax, 0)
	    , PARAM_INIT(mWaistPitchMax, 1000)
	    , PARAM_INIT(mWaistAngleChangeRate, 0.07f)
	{
	}
	TParamT<f32> mHeadRot;
	TParamT<f32> mWaistRoll;
	TParamT<f32> mWaistPitch;
	TParamT<s16> mWaistRollMax;
	TParamT<s16> mWaistPitchMax;
	TParamT<f32> mWaistAngleChangeRate;
};

class TAttackParams : public TParams {
public:
	TAttackParams(const char* prm)
	    : TParams(prm)
	    , PARAM_INIT(mRadius, 100.0f)
	    , PARAM_INIT(mHeight, 50.0f)
	{
	}
	TParamT<f32> mRadius;
	TParamT<f32> mHeight;
};

class THangingParams : public TParams {
public:
	THangingParams(const char* prm)
	    : TParams(prm)
	    , PARAM_INIT(mMoveS, 0.1f)
	    , PARAM_INIT(mAnmRate, 0.5f)
	    , PARAM_INIT(mRapidTime, 2000)
	    , PARAM_INIT(mLimitTime, 0x960)
	    , PARAM_INIT(mAnmRapid, 8.0f)
	    , PARAM_INIT(mDescentSp, 10.0f)
	{
	}
	TParamT<f32> mMoveS;
	TParamT<f32> mAnmRate;
	TParamT<s16> mRapidTime;
	TParamT<s16> mLimitTime;
	TParamT<f32> mAnmRapid;
	TParamT<f32> mDescentSp;
};

class TPullParams : public TParams {
public:
	TPullParams(const char* prm)
	    : TParams(prm)
	    , PARAM_INIT(mPullRateV, 0.3f)
	    , PARAM_INIT(mPullRateH, 0.05f)
	    , PARAM_INIT(mOilPullRateV, 0.1f)
	    , PARAM_INIT(mOilPullRateH, 0.0099999998f)
	{
	}

	TParamT<f32> mPullRateV;
	TParamT<f32> mPullRateH;
	TParamT<f32> mOilPullRateV;
	TParamT<f32> mOilPullRateH;
};

class TDmgParams : public TParams {
public:
	TDmgParams(const char* prm)
	    : TParams(prm)
	    , PARAM_INIT(mDamage, 1)
	    , PARAM_INIT(mDownType, 0)
	    , PARAM_INIT(mWaterEmit, 0)
	    , PARAM_INIT(mMotor, 0)
	    , PARAM_INIT(mMinSpeed, 0.0f)
	    , PARAM_INIT(mDirty, 0.0f)
	    , PARAM_INIT(mInvincibleTime, 0)
	{
	}

	TParamT<u8> mDamage;
	TParamT<u8> mDownType;
	TParamT<u8> mWaterEmit;
	TParamT<u8> mMotor;
	TParamT<f32> mMinSpeed;
	TParamT<f32> mDirty;
	TParamT<s16> mInvincibleTime;
};

class TUpperBodyParams : public TParams {
public:
	TUpperBodyParams(const char* prm)
	    : TParams(prm)
	    , PARAM_INIT(mPumpWaitTime, 10)
	    , PARAM_INIT(mPumpAnmSpeed, 0.0099999998f)
	    , PARAM_INIT(mHoverHeadAngle, 0xe000)
	    , PARAM_INIT(mFeelDeepHeadAngle, 0x2000)
	    , PARAM_INIT(mFrontWallHeadAngle, 0xe000)
	{
	}

	TParamT<s16> mPumpWaitTime;
	TParamT<f32> mPumpAnmSpeed;
	TParamT<s16> mHoverHeadAngle;
	TParamT<s16> mFeelDeepHeadAngle;
	TParamT<s16> mFrontWallHeadAngle;
};

class TBarParams : public TParams {
public:
	TBarParams(const char* prm)
	    : TParams(prm)
	    , PARAM_INIT(mClimbSp, 0.035f)
	    , PARAM_INIT(mRotateSp, 3.0f)
	    , PARAM_INIT(mClimbAnmRate, 0.00390625f)
	    , PARAM_INIT(mCatchRadius, 100.0f)
	    , PARAM_INIT(mCatchAngle, 0.8f)
	{
	}

	TParamT<f32> mClimbSp;
	TParamT<f32> mRotateSp;
	TParamT<f32> mClimbAnmRate;
	TParamT<f32> mCatchRadius;
	TParamT<f32> mCatchAngle;
};

class TSoundParams : public TParams {
public:
	TSoundParams(const char* prm)
	    : TParams(prm)
	    , PARAM_INIT(mStartFallVoiceSpeed, 60.0f)
	{
	}

	TParamT<f32> mStartFallVoiceSpeed;
};

class TOptionParams : public TParams {
public:
	TOptionParams(const char* prm)
	    : TParams(prm)
	    , PARAM_INIT(mZ, -1000.0f)
	    , PARAM_INIT(mXMin, 846.0f)
	    , PARAM_INIT(mXMax, 2000.0f)
	{
	}

	TParamT<f32> mZ;
	TParamT<f32> mXMin;
	TParamT<f32> mXMax;
};

class TMarioEffectParams : public TParams {
public:
	TMarioEffectParams(const char* prm)
	    : TParams(prm)
	    , PARAM_INIT(mDashInc, 0.033333335f)
	    , PARAM_INIT(mDashDec, 0.016666668f)
	    , PARAM_INIT(mDashMaxBlendInBlur, 0xb4)
	    , PARAM_INIT(mDashMaxBlendInIris, 0xb4)
	    , PARAM_INIT(mDashBlendScale, 0.2f)
	{
	}

	TParamT<f32> mDashInc;
	TParamT<f32> mDashDec;
	TParamT<u8> mDashMaxBlendInBlur;
	TParamT<u8> mDashMaxBlendInIris;
	TParamT<f32> mDashBlendScale;
};

class THoverParams : public TParams {
public:
	THoverParams(const char* prm)
	    : TParams(prm)
	    , PARAM_INIT(mRotSp, 0x80)
	    , PARAM_INIT(mAccelRate, 0.029999999f)
	    , PARAM_INIT(mBrake, 0.94999999f)
	{
	}

	TParamT<s16> mRotSp;
	TParamT<f32> mAccelRate;
	TParamT<f32> mBrake;
};

class TDivingParams : public TParams {
public:
	TDivingParams(const char* prm)
	    : TParams(prm)
	    , PARAM_INIT(mRotSp, 0x80)
	    , PARAM_INIT(mGravity, 0.5f)
	    , PARAM_INIT(mAccelControl, 0.02f)
	    , PARAM_INIT(mSeaBrake, 0.999f)
	    , PARAM_INIT(mSeaBrakeY, 0.98f)
	{
	}

	TParamT<s16> mRotSp;
	TParamT<f32> mGravity;
	TParamT<f32> mAccelControl;
	TParamT<f32> mSeaBrake;
	TParamT<f32> mSeaBrakeY;
};

class TYoshiParams : public TParams {
public:
	TYoshiParams(const char* prm)
	    : TParams(prm)
	    , PARAM_INIT(mRunYoshiMult, 1.2f)
	    , PARAM_INIT(mJumpYoshiMult, 1.0f)
	    , PARAM_INIT(mRotYoshiMult, 1.5f)
	    , PARAM_INIT(mHeadFront, 80.0f)
	    , PARAM_INIT(mHeadRadius, 50.0f)
	    , PARAM_INIT(mHoldOutAccCtrlF, 0.0099999998f)
	    , PARAM_INIT(mHoldOutAccCtrlB, 0.023f)
	    , PARAM_INIT(mHoldOutSldCtrl, 0.3f)
	    , PARAM_INIT(mDecBrake, 1.0f)
	{
	}

	TParamT<f32> mRunYoshiMult;
	TParamT<f32> mJumpYoshiMult;
	TParamT<f32> mRotYoshiMult;
	TParamT<f32> mHeadFront;
	TParamT<f32> mHeadRadius;
	TParamT<f32> mHoldOutAccCtrlF;
	TParamT<f32> mHoldOutAccCtrlB;
	TParamT<f32> mHoldOutSldCtrl;
	TParamT<f32> mDecBrake;
};

class THangRoofParams : public TParams {
public:
	THangRoofParams(const char* prm)
	    : TParams(prm)
	    , PARAM_INIT(mAnmMult, 0.3f)
	{
	}

	TParamT<f32> mAnmMult;
};

class TWireParams : public TParams {
public:
	TWireParams(const char* prm)
	    : TParams(prm)
	    , PARAM_INIT(mRotSpeed, -8)
	    , PARAM_INIT(mRotSpeedTrgHover, 8)
	    , PARAM_INIT(mRotSpeedTrgTurbo, 1000)
	    , PARAM_INIT(mRotSpeedTrgRocket, 1000)
	    , PARAM_INIT(mRotSpeedMax, 0x578)
	    , PARAM_INIT(mRotStop, 100)
	    , PARAM_INIT(mRotGravity, 0x14)
	    , PARAM_INIT(mRotBrake, 0.98f)
	    , PARAM_INIT(mJumpRate, 0.09f)
	    , PARAM_INIT(mSwingRate, 0.0049999999f)
	    , PARAM_INIT(mWireJumpAccelControl, 0.0099999998f)
	    , PARAM_INIT(mWireJumpSlideControl, 0.3f)
	    , PARAM_INIT(mWireJumpMult, 5.0f)
	    , PARAM_INIT(mWireJumpBase, 20.0f)
	    , PARAM_INIT(mWireSwingBrake, 0.99f)
	    , PARAM_INIT(mWireSwingMax, 100.0f)
	{
	}

	TParamT<s16> mRotSpeed;
	TParamT<s16> mRotSpeedTrgHover;
	TParamT<s16> mRotSpeedTrgTurbo;
	TParamT<s16> mRotSpeedTrgRocket;
	TParamT<s16> mRotSpeedMax;
	TParamT<s16> mRotStop;
	TParamT<s16> mRotGravity;
	TParamT<f32> mRotBrake;
	TParamT<f32> mJumpRate;
	TParamT<s32> mSwingRate;
	TParamT<f32> mWireJumpAccelControl;
	TParamT<f32> mWireJumpSlideControl;
	TParamT<f32> mWireJumpMult;
	TParamT<f32> mWireJumpBase;
	TParamT<f32> mWireSwingBrake;
	TParamT<f32> mWireSwingMax;
};

class TAutoDemoParams : public TParams {
public:
	TAutoDemoParams(const char* prm)
	    : TParams(prm)
	    , PARAM_INIT(mWarpInBallsDispTime, 6)
	    , PARAM_INIT(mWarpInBallsTime, 0x46)
	    , PARAM_INIT(mWarpInCapturedTime, 0x78)
	    , PARAM_INIT(mWarpInTremble, 15.0f)
	    , PARAM_INIT(mWarpInVecBase, 0.3f)
	    , PARAM_INIT(mWarpTransTremble, 50.0f)
	    , PARAM_INIT(mReadRotSp, 0x400f)
	{
	}

	TParamT<s16> mWarpInBallsDispTime;
	TParamT<s16> mWarpInBallsTime;
	TParamT<s16> mWarpInCapturedTime;
	TParamT<f32> mWarpInTremble;
	TParamT<f32> mWarpInVecBase;
	TParamT<f32> mWarpTransTremble;
	TParamT<s16> mReadRotSp;
};

class TSlipParams : TParams {
public:
	TSlipParams(const char* prm)
	    : TParams(prm)
	    , PARAM_INIT(mSlipFriction, 0.89999998f)
	    , PARAM_INIT(mSlopeAccelUp, 0.0f)
	    , PARAM_INIT(mSlopeAccelDown, 0.0f)
	    , PARAM_INIT(mSlideAccelUp, 0.0f)
	    , PARAM_INIT(mSlideAccelDown, 0.0f)
	    , PARAM_INIT(mSlideAccelNormal, 15.0f)
	    , PARAM_INIT(mSlideStopCatch, 15.0f)
	    , PARAM_INIT(mJumpEnable, 1)
	    , PARAM_INIT(mMissJump, 1)
	    , PARAM_INIT(mSlideAngleYSp, 0x200)
	    , PARAM_INIT(mStickSlideMult, 0.05f)
	{
	}

	TParamT<f32> mSlipFriction;
	TParamT<f32> mSlopeAccelUp;
	TParamT<f32> mSlopeAccelDown;
	TParamT<f32> mSlideAccelUp;
	TParamT<f32> mSlideAccelDown;
	TParamT<f32> mSlideAccelNormal;
	TParamT<f32> mSlideStopCatch;
	TParamT<u8> mJumpEnable;
	TParamT<u8> mMissJump;
	TParamT<s16> mSlideAngleYSp;
	TParamT<f32> mStickSlideMult;
};

class TWaterEffectParams : public TParams {
public:
	TWaterEffectParams(const char* prm)
	    : TParams(prm)
	    , PARAM_INIT(mJumpIntoMdlEffectSpY, 10.0f)
	    , PARAM_INIT(mJumpIntoMinY, 20.0f)
	    , PARAM_INIT(mJumpIntoMaxY, 50.0f)
	    , PARAM_INIT(mJumpIntoScaleMin, 0.75f)
	    , PARAM_INIT(mJumpIntoScaleWidth, 1.0f)
	    , PARAM_INIT(mRunningRippleSpeed, 30.0f)
	    , PARAM_INIT(mRunningRippleDepth, 30.0f)
	{
	}

	TParamT<f32> mJumpIntoMdlEffectSpY;
	TParamT<f32> mJumpIntoMinY;
	TParamT<f32> mJumpIntoMaxY;
	TParamT<f32> mJumpIntoScaleMin;
	TParamT<f32> mJumpIntoScaleWidth;
	TParamT<f32> mRunningRippleSpeed;
	TParamT<f32> mRunningRippleDepth;
};

class TDirtyParams : TParams {
public:
	TDirtyParams(const char* prm)
	    : TParams(prm)
	    , PARAM_INIT(mIncRunning, 0.1f)
	    , PARAM_INIT(mIncCatching, 0.3f)
	    , PARAM_INIT(mIncSlipping, 0.2f)
	    , PARAM_INIT(mDecSwimming, 0.5f)
	    , PARAM_INIT(mDecWaterHit, 0.2f)
	    , PARAM_INIT(mDecRotJump, 0.1f)
	    , PARAM_INIT(mBrakeStartValSlip, 0.99f)
	    , PARAM_INIT(mBrakeStartValRun, 0.98f)
	    , PARAM_INIT(mDirtyTimeSlip, 600)
	    , PARAM_INIT(mDirtyTimeRun, 600)
	    , PARAM_INIT(mPolSizeSlip, 200.0f)
	    , PARAM_INIT(mPolSizeRun, 80.0f)
	    , PARAM_INIT(mPolSizeFootPrint, 200.0f)
	    , PARAM_INIT(mPolSizeJump, 200.0f)
	    , PARAM_INIT(mSlopeAngle, 0.99f)
	    , PARAM_INIT(mDirtyMax, 200.0f)
	    , PARAM_INIT(mSlipAnmSpeed, 3.0f)
	    , PARAM_INIT(mSlipRunSp, 0.0099999998f)
	    , PARAM_INIT(mSlipCatchSp, 0.0099999998f)
	    , PARAM_INIT(mSlipRotate, 0x100)
	    , PARAM_INIT(mSlipCatchRotate, 0x100)
	    , PARAM_INIT(mBrakeSlipNoPollute, 0.98f)
	    , PARAM_INIT(mFogTimeYellow, 0xf0)
	    , PARAM_INIT(mFogTimeRed, 600)
	{
	}

	TParamT<f32> mIncRunning;
	TParamT<f32> mIncCatching;
	TParamT<f32> mIncSlipping;
	TParamT<f32> mDecSwimming;
	TParamT<f32> mDecWaterHit;
	TParamT<f32> mDecRotJump;
	TParamT<f32> mBrakeStartValSlip;
	TParamT<f32> mBrakeStartValRun;
	TParamT<s16> mDirtyTimeSlip;
	TParamT<s16> mDirtyTimeRun;
	TParamT<f32> mPolSizeSlip;
	TParamT<f32> mPolSizeRun;
	TParamT<f32> mPolSizeFootPrint;
	TParamT<f32> mPolSizeJump;
	TParamT<f32> mSlopeAngle;
	TParamT<f32> mDirtyMax;
	TParamT<f32> mSlipAnmSpeed;
	TParamT<f32> mSlipRunSp;
	TParamT<f32> mSlipCatchSp;
	TParamT<s16> mSlipRotate;
	TParamT<s16> mSlipCatchRotate;
	TParamT<f32> mBrakeSlipNoPollute;
	TParamT<s16> mFogTimeYellow;
	TParamT<s16> mFogTimeRed;
};

class TMarioMotorParams : TParams {
public:
	TMarioMotorParams(const char* prm)
	    : TParams(prm)
	    , PARAM_INIT(mMotorReturn, 0x19)
	    , PARAM_INIT(mMotorTrample, 8)
	    , PARAM_INIT(mMotorHipDrop, 0xf)
	    , PARAM_INIT(mMotorWall, 6)
	{
	}

	TParamT<s16> mMotorReturn;
	TParamT<s16> mMotorTrample;
	TParamT<s16> mMotorHipDrop;
	TParamT<s16> mMotorWall;
};

class TSwimParams : TParams {
public:
	TSwimParams(const char* prm)
	    : TParams(prm)
	    , PARAM_INIT(mStartSp, 1.0f)
	    , PARAM_INIT(mMoveSp, 0.8f)
	    , PARAM_INIT(mMoveBrake, 1.0f)
	    , PARAM_INIT(mSwimmingRotSpMin, 0x200)
	    , PARAM_INIT(mSwimmingRotSpMax, 0x400)
	    , PARAM_INIT(mPumpingRotSpMin, 0x100)
	    , PARAM_INIT(mPumpingRotSpMax, 0x200)
	    , PARAM_INIT(mGravity, 0.1f)
	    , PARAM_INIT(mWaitBouyancy, 1.0f)
	    , PARAM_INIT(mMoveBouyancy, 1.0f)
	    , PARAM_INIT(mUpDownBrake, 0.94999999f)
	    , PARAM_INIT(mCanJumpDepth, 1.0f)
	    , PARAM_INIT(mEndDepth, 80.0f)
	    , PARAM_INIT(mFloatHeight, 120.0f)
	    , PARAM_INIT(mStartVMult, 0.1f)
	    , PARAM_INIT(mStartVYMult, 0.1f)
	    , PARAM_INIT(mRush, 3.0f)
	    , PARAM_INIT(mAnmBrake, 0.02f)
	    , PARAM_INIT(mPaddleSpeedUp, 0.3f)
	    , PARAM_INIT(mPaddleJumpUp, 1.0f)
	    , PARAM_INIT(mFloatUp, 2.0f)
	    , PARAM_INIT(mWaterLevelCheckHeight, 10.0f)
	    , PARAM_INIT(mPaddleDown, 1.0f)
	    , PARAM_INIT(mWaitSinkTime, 0x32)
	    , PARAM_INIT(mCanBreathDepth, 50.0f)
	    , PARAM_INIT(mWaitSinkSpeed, 5.0f)
	    , PARAM_INIT(mAirDec, 0.001f)
	    , PARAM_INIT(mAirDecDive, 0.001f)
	    , PARAM_INIT(mAirInc, 0.029999999f)
	{
	}

	TParamT<f32> mStartSp;
	TParamT<f32> mMoveSp;
	TParamT<f32> mMoveBrake;
	TParamT<s16> mSwimmingRotSpMin;
	TParamT<s16> mSwimmingRotSpMax;
	TParamT<s16> mPumpingRotSpMin;
	TParamT<s16> mPumpingRotSpMax;
	TParamT<f32> mGravity;
	TParamT<f32> mWaitBouyancy;
	TParamT<f32> mMoveBouyancy;
	TParamT<f32> mUpDownBrake;
	TParamT<f32> mCanJumpDepth;
	TParamT<f32> mEndDepth;
	TParamT<f32> mFloatHeight;
	TParamT<f32> mStartVMult;
	TParamT<f32> mStartVYMult;
	TParamT<f32> mRush;
	TParamT<f32> mAnmBrake;
	TParamT<f32> mPaddleSpeedUp;
	TParamT<f32> mPaddleJumpUp;
	TParamT<f32> mFloatUp;
	TParamT<f32> mWaterLevelCheckHeight;
	TParamT<f32> mPaddleDown;
	TParamT<s16> mWaitSinkTime;
	TParamT<f32> mCanBreathDepth;
	TParamT<f32> mWaitSinkSpeed;
	TParamT<f32> mAirDec;
	TParamT<f32> mAirDecDive;
	TParamT<f32> mAirInc;
};

class TMarioParticleParams : public TParams {
public:
	TMarioParticleParams(const char* prm)
	    : TParams(prm)
	    , PARAM_INIT(mMeltInWaterMax, 0.5f)
	    , PARAM_INIT(mWaveEmitSpeed, 5.0f)
	    , PARAM_INIT(mWaveAlphaDec, 5)
	    , PARAM_INIT(mBubbleDepth, 10.0f)
	    , PARAM_INIT(mBodyBubbleSpMin, 0.0f)
	    , PARAM_INIT(mBodyBubbleSpMax, 40.0f)
	    , PARAM_INIT(mBodyBubbleEmitMin, 0.0f)
	    , PARAM_INIT(mBodyBubbleEmitMax, 0.5f)
	    , PARAM_INIT(mBubbleToRipple, 0.3f)
	    , PARAM_INIT(mToroccoWind, 0.001f)
	    , PARAM_INIT(mToroccoSpark, 0.001f)
	{
	}

	TParamT<f32> mMeltInWaterMax;
	TParamT<f32> mWaveEmitSpeed;
	TParamT<s16> mWaveAlphaDec;
	TParamT<f32> mBubbleDepth;
	TParamT<f32> mBodyBubbleSpMin;
	TParamT<f32> mBodyBubbleSpMax;
	TParamT<f32> mBodyBubbleEmitMin;
	TParamT<f32> mBodyBubbleEmitMax;
	TParamT<f32> mBubbleToRipple;
	TParamT<f32> mToroccoWind;
	TParamT<f32> mToroccoSpark;
};

class TSurfingParams : TParams {
public:
	TSurfingParams(const char* prm)
	    : TParams(prm)
	    , PARAM_INIT(mRotMin, 2048.0f)
	    , PARAM_INIT(mRotMax, 1024.0f)
	    , PARAM_INIT(mPowMin, 24.0f)
	    , PARAM_INIT(mPowMax, 64.0f)
	    , PARAM_INIT(mAccel, 58.0f)
	    , PARAM_INIT(mWaistRoll, 0.25f)
	    , PARAM_INIT(mWaistPitch, 170.0f)
	    , PARAM_INIT(mWaistRollMax, 0x400)
	    , PARAM_INIT(mWaistPitchMax, 0x1555)
	    , PARAM_INIT(mRoll, -0.44999999f)
	    , PARAM_INIT(mPitch, -170.0f)
	    , PARAM_INIT(mRollMax, 0x4000)
	    , PARAM_INIT(mPitchMax, 0x1555)
	    , PARAM_INIT(mAngleChangeRate, 0.0099999998f)
	    , PARAM_INIT(mWaistAngleChangeRate, 0.0099999998f)
	    , PARAM_INIT(mScaleMin, 0.5f)
	    , PARAM_INIT(mScaleMax, 1.0f)
	    , PARAM_INIT(mScaleMinSpeed, 24.0f)
	    , PARAM_INIT(mScaleMaxSpeed, 60.0f)
	    , PARAM_INIT(mJumpPow, 42.0f)
	    , PARAM_INIT(mJumpXZRatio, 0.25f)
	    , PARAM_INIT(mClashSpeed, 40.0f)
	    , PARAM_INIT(mClashAngle, 0x5555)
	{
	}

	TParamT<f32> mRotMin;
	TParamT<f32> mRotMax;
	TParamT<f32> mPowMin;
	TParamT<f32> mPowMax;
	TParamT<f32> mAccel;
	TParamT<f32> mWaistRoll;
	TParamT<f32> mWaistPitch;
	TParamT<s16> mWaistRollMax;
	TParamT<s16> mWaistPitchMax;
	TParamT<s32> mRoll;
	TParamT<f32> mPitch;
	TParamT<s16> mRollMax;
	TParamT<s16> mPitchMax;
	TParamT<f32> mAngleChangeRate;
	TParamT<f32> mWaistAngleChangeRate;
	TParamT<f32> mScaleMin;
	TParamT<f32> mScaleMax;
	TParamT<f32> mScaleMinSpeed;
	TParamT<f32> mScaleMaxSpeed;
	TParamT<f32> mJumpPow;
	TParamT<f32> mJumpXZRatio;
	TParamT<f32> mClashSpeed;
	TParamT<s16> mClashAngle;
};

class TControllerParams : TParams {
public:
	TControllerParams(const char* prm)
	    : TParams(prm)
	    , PARAM_INIT(mAnalogLRToZeroVal, 0x1e)
	    , PARAM_INIT(mAnalogLRToMiddleVal, 0x5a)
	    , PARAM_INIT(mAnalogLRToMaxVal, 0x96)
	    , PARAM_INIT(mAnalogLRMiddleLevel, 0.1f)
	    , PARAM_INIT(mStartToWalkLevel, 15.0f)
	    , PARAM_INIT(mStickRotateTime, 0x18)
	    , PARAM_INIT(mLengthMultTimes, 10)
	    , PARAM_INIT(mLengthMult, 0.935f)
	    , PARAM_INIT(mSquatRotMidAnalog, 0.69999999f)
	    , PARAM_INIT(mSquatRotMidValue, 0.05f)
	{
	}

	TParamT<u8> mAnalogLRToZeroVal;
	TParamT<u8> mAnalogLRToMiddleVal;
	TParamT<u8> mAnalogLRToMaxVal;
	TParamT<f32> mAnalogLRMiddleLevel;
	TParamT<f32> mStartToWalkLevel;
	TParamT<s16> mStickRotateTime;
	TParamT<s16> mLengthMultTimes;
	TParamT<f32> mLengthMult;
	TParamT<f32> mSquatRotMidAnalog;
	TParamT<f32> mSquatRotMidValue;
};

class TJumpParams : TParams {
public:
	TJumpParams(const char* prm)
	    : TParams(prm)
	    , PARAM_INIT(mGravity, 0.8f)
	    , PARAM_INIT(mSpinJumpGravity, 0.6f)
	    , PARAM_INIT(mJumpingMax, 80.0f)
	    , PARAM_INIT(mJumpSpeedBrake, 0.999f)
	    , PARAM_INIT(mJumpAccelControl, 0.5f)
	    , PARAM_INIT(mJumpSlideControl, 0.5f)
	    , PARAM_INIT(mTurnJumpForce, 62.0f)
	    , PARAM_INIT(mFenceSpeed, 2.0f)
	    , PARAM_INIT(mFireDownForce, 80.0f)
	    , PARAM_INIT(mFireDownControl, 1.0f)
	    , PARAM_INIT(mFireBackVelocity, 1.0f)
	    , PARAM_INIT(mBroadJumpForce, 80.0f)
	    , PARAM_INIT(mBroadJumpForceY, 30.0f)
	    , PARAM_INIT(mRotateJumpForceY, 62.0f)
	    , PARAM_INIT(mPopUpSpeedY, 5.0f)
	    , PARAM_INIT(mPopUpSpeedYMult, 10.0f)
	    , PARAM_INIT(mBackJumpForce, -16.0f)
	    , PARAM_INIT(mBackJumpForceY, 62.0f)
	    , PARAM_INIT(mHipAttackSpeedY, -50.0f)
	    , PARAM_INIT(mSuperHipAttackSpeedY, -80.0f)
	    , PARAM_INIT(mJumpCatchRotXSp, 0x100)
	    , PARAM_INIT(mJumpCatchRotXMax, 0x2aaa)
	    , PARAM_INIT(mRotBroadEnableV, 30.0f)
	    , PARAM_INIT(mRotBroadJumpForce, 60.0f)
	    , PARAM_INIT(mRotBroadJumpForceY, 20.0f)
	    , PARAM_INIT(mTrampolineDec, 1.0f)
	    , PARAM_INIT(mSecJumpEnableSp, 20.0f)
	    , PARAM_INIT(mSecJumpForce, 52.0f)
	    , PARAM_INIT(mSecJumpSpeedMult, 0.0f)
	    , PARAM_INIT(mSecJumpXZMult, 0.8f)
	    , PARAM_INIT(mTriJumpEnableSp, 20.0f)
	    , PARAM_INIT(mUltraJumpForce, 70.0f)
	    , PARAM_INIT(mUltraJumpSpeedMult, 0.25f)
	    , PARAM_INIT(mUltraJumpXZMult, 0.8f)
	    , PARAM_INIT(mValleyDepth, 500.0f)
	    , PARAM_INIT(mThrownAccel, 0.5f)
	    , PARAM_INIT(mThrownSlide, 0.5f)
	    , PARAM_INIT(mThrownBrake, 0.98f)
	    , PARAM_INIT(mTremblePower, 5.0f)
	    , PARAM_INIT(mTrembleAccele, 2.0f)
	    , PARAM_INIT(mTrembleBrake, 0.99f)
	    , PARAM_INIT(mTrembleTime, 600)
	    , PARAM_INIT(mClashAngle, 0x3555)
	    , PARAM_INIT(mJumpJumpCatchSp, 50.0f)
	    , PARAM_INIT(mGetOffYoshiY, 30.0f)
	    , PARAM_INIT(mSuperHipAttackCt, 0x32)
	{
	}

	TParamT<f32> mGravity;
	TParamT<f32> mSpinJumpGravity;
	TParamT<f32> mJumpingMax;
	TParamT<f32> mJumpSpeedBrake;
	TParamT<f32> mJumpAccelControl;
	TParamT<f32> mJumpSlideControl;
	TParamT<f32> mTurnJumpForce;
	TParamT<f32> mFenceSpeed;
	TParamT<f32> mFireDownForce;
	TParamT<f32> mFireDownControl;
	TParamT<f32> mFireBackVelocity;
	TParamT<f32> mBroadJumpForce;
	TParamT<f32> mBroadJumpForceY;
	TParamT<f32> mRotateJumpForceY;
	TParamT<f32> mPopUpSpeedY;
	TParamT<f32> mPopUpSpeedYMult;
	TParamT<f32> mBackJumpForce;
	TParamT<f32> mBackJumpForceY;
	TParamT<f32> mHipAttackSpeedY;
	TParamT<f32> mSuperHipAttackSpeedY;
	TParamT<s16> mJumpCatchRotXSp;
	TParamT<s16> mJumpCatchRotXMax;
	TParamT<f32> mRotBroadEnableV;
	TParamT<f32> mRotBroadJumpForce;
	TParamT<f32> mRotBroadJumpForceY;
	TParamT<f32> mTrampolineDec;
	TParamT<f32> mSecJumpEnableSp;
	TParamT<f32> mSecJumpForce;
	TParamT<f32> mSecJumpSpeedMult;
	TParamT<f32> mSecJumpXZMult;
	TParamT<f32> mTriJumpEnableSp;
	TParamT<f32> mUltraJumpForce;
	TParamT<f32> mUltraJumpSpeedMult;
	TParamT<f32> mUltraJumpXZMult;
	TParamT<f32> mValleyDepth;
	TParamT<f32> mThrownAccel;
	TParamT<f32> mThrownSlide;
	TParamT<f32> mThrownBrake;
	TParamT<f32> mTremblePower;
	TParamT<f32> mTrembleAccele;
	TParamT<f32> mTrembleBrake;
	TParamT<s16> mTrembleTime;
	TParamT<s16> mClashAngle;
	TParamT<f32> mJumpJumpCatchSp;
	TParamT<f32> mGetOffYoshiY;
	TParamT<s16> mSuperHipAttackCt;
};

class TRunParams : TParams {
public:
	TRunParams(const char* prm)
	    : TParams(prm)
	    , PARAM_INIT(mMaxSpeed, 32.0f)
	    , PARAM_INIT(mVelMinusBrake, 1.1f)
	    , PARAM_INIT(mAddBase, 1.1f)
	    , PARAM_INIT(mAddVelDiv, 0.023255814f)
	    , PARAM_INIT(mDecStartNrmY, 0.94999999f)
	    , PARAM_INIT(mDecBrake, 1.0f)
	    , PARAM_INIT(mSoft2Walk, 8.0f)
	    , PARAM_INIT(mWalk2Soft, 5.0f)
	    , PARAM_INIT(mSoftStepAnmMult, 0.125f)
	    , PARAM_INIT(mRunAnmSpeedBase, 1.0f)
	    , PARAM_INIT(mRunAnmSpeedMult, 0.059999999f)
	    , PARAM_INIT(mMotBlendWalkSp, 1.0f)
	    , PARAM_INIT(mMotBlendRunSp, 3.0f)
	    , PARAM_INIT(mSwimDepth, 120.0f)
	    , PARAM_INIT(mInWaterBrake, 0.89999998f)
	    , PARAM_INIT(mInWaterAnmBrake, 0.6f)
	    , PARAM_INIT(mPumpingSlideSp, 0.1f)
	    , PARAM_INIT(mPumpingSlideAnmSp, 0.5f)
	    , PARAM_INIT(mDoJumpCatchSp, 15.0f)
	    , PARAM_INIT(mTurnNeedSp, 10.0f)
	    , PARAM_INIT(mDashRotSp, 100)
	{
	}

	TParamT<f32> mMaxSpeed;
	TParamT<f32> mVelMinusBrake;
	TParamT<f32> mAddBase;
	TParamT<f32> mAddVelDiv;
	TParamT<f32> mDecStartNrmY;
	TParamT<f32> mDecBrake;
	TParamT<f32> mSoft2Walk;
	TParamT<f32> mWalk2Soft;
	TParamT<f32> mSoftStepAnmMult;
	TParamT<f32> mRunAnmSpeedBase;
	TParamT<f32> mRunAnmSpeedMult;
	TParamT<f32> mMotBlendWalkSp;
	TParamT<f32> mMotBlendRunSp;
	TParamT<f32> mSwimDepth;
	TParamT<f32> mInWaterBrake;
	TParamT<f32> mInWaterAnmBrake;
	TParamT<f32> mPumpingSlideSp;
	TParamT<f32> mPumpingSlideAnmSp;
	TParamT<f32> mDoJumpCatchSp;
	TParamT<f32> mTurnNeedSp;
	TParamT<s16> mDashRotSp;
};

class TGraffitoParams : TParams {
public:
	TGraffitoParams(const char* prm)
	    : TParams(prm)
	    , PARAM_INIT(mSinkTime, 0xf0)
	    , PARAM_INIT(mSinkDmgTime, 0xf0)
	    , PARAM_INIT(mSinkHeight, 150.0f)
	    , PARAM_INIT(mSinkMoveMin, 0.3f)
	    , PARAM_INIT(mSinkMoveMax, 0.5f)
	    , PARAM_INIT(mSinkRecover, 0.05f)
	    , PARAM_INIT(mSinkJumpRateMin, 0.1f)
	    , PARAM_INIT(mSinkJumpRateMax, 0.3f)
	    , PARAM_INIT(mSinkPumpLimit, 0.25f)
	    , PARAM_INIT(mSinkDmgDepth, 0.25f)
	    , PARAM_INIT(mFireHeight, 1000.0f)
	    , PARAM_INIT(mDizzySlipCtMax, 1000)
	    , PARAM_INIT(mDizzyWalkCtMax, 1000)
	    , PARAM_INIT(mDizzyAngleY, 0x7fff)
	    , PARAM_INIT(mDizzyAngleRate, 400.0f)
	    , PARAM_INIT(mDizzyPowerRate, 120.0f)
	    , PARAM_INIT(mDizzyPower, 20.0f)
	    , PARAM_INIT(mFireInvincibleTime, 0x96)
	    , PARAM_INIT(mFootEraseTimes, 4)
	    , PARAM_INIT(mFootEraseSize, 400.0f)
	    , PARAM_INIT(mFootEraseFront, 200.0f)
	{
	}

	TParamT<s16> mSinkTime;
	TParamT<s16> mSinkDmgTime;
	TParamT<f32> mSinkHeight;
	TParamT<f32> mSinkMoveMin;
	TParamT<f32> mSinkMoveMax;
	TParamT<f32> mSinkRecover;
	TParamT<f32> mSinkJumpRateMin;
	TParamT<f32> mSinkJumpRateMax;
	TParamT<f32> mSinkPumpLimit;
	TParamT<f32> mSinkDmgDepth;
	TParamT<f32> mFireHeight;
	TParamT<s16> mDizzySlipCtMax;
	TParamT<s16> mDizzyWalkCtMax;
	TParamT<s16> mDizzyAngleY;
	TParamT<f32> mDizzyAngleRate;
	TParamT<f32> mDizzyPowerRate;
	TParamT<f32> mDizzyPower;
	TParamT<s16> mFireInvincibleTime;
	TParamT<s16> mFootEraseTimes;
	TParamT<f32> mFootEraseSize;
	TParamT<f32> mFootEraseFront;
};

#endif
