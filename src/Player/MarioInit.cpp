#include <Player/MarioMain.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// TODO: stuff from other rogue includes
static JGeometry::TVec3<f32> cDeformedTerrainCenter
    = JGeometry::TVec3<f32>(0.0f, 5000.0f, 0.0f);
static const char* dummyMactorStringValue1 = "\0\0\0\0\0\0\0\0\0\0\0";
static const char* SMS_NO_MEMORY_MESSAGE   = "メモリが足りません\n";
const char cDirtyFileName[] = "/scene/map/pollution/H_ma_rak.bti";
const char cDirtyTexName[]  = "H_ma_rak_dummy";

TMario::TOptionParams::TOptionParams(const char* prm)
    : TParams(prm)
    , PARAM_INIT(mZ, -1000.0f)
    , PARAM_INIT(mXMin, 846.0f)
    , PARAM_INIT(mXMax, 2000.0f)
{
	TParams::load(mPrmPath);
}

TMario::TSoundParams::TSoundParams()
    : TParams("/Mario/Sound.prm")
    , PARAM_INIT(mStartFallVoiceSpeed, 60.0f)
{
	TParams::load(mPrmPath);
}

TMario::TAutoDemoParams::TAutoDemoParams()
    : TParams("/Mario/AutoDemo.prm")
    , PARAM_INIT(mWarpInBallsDispTime, 6)
    , PARAM_INIT(mWarpInBallsTime, 0x46)
    , PARAM_INIT(mWarpInCapturedTime, 0x78)
    , PARAM_INIT(mWarpInTremble, 15.0f)
    , PARAM_INIT(mWarpInVecBase, 0.3f)
    , PARAM_INIT(mWarpTransTremble, 50.0f)
    , PARAM_INIT(mReadRotSp, 0x400f)
{
	TParams::load(mPrmPath);
}

TMario::TEParams::TEParams(const char* prm)
    : TParams(prm)
    , PARAM_INIT(mDamage, 1)
    , PARAM_INIT(mDownType, 0)
    , PARAM_INIT(mWaterEmit, 0)
    , PARAM_INIT(mMotor, 0)
    , PARAM_INIT(mMinSpeed, 0.0f)
    , PARAM_INIT(mDirty, 0.0f)
    , PARAM_INIT(mInvincibleTime, 0)
{
	TParams::load(mPrmPath);
}

TMario::TDivingParams::TDivingParams(const char* prm)
    : TParams(prm)
    , PARAM_INIT(mRotSp, 0x80)
    , PARAM_INIT(mGravity, 0.5f)
    , PARAM_INIT(mAccelControl, 0.02f)
    , PARAM_INIT(mSeaBrake, 0.999f)
    , PARAM_INIT(mSeaBrakeY, 0.98f)
{
	TParams::load(mPrmPath);
}

TMario::THHoverParams::THHoverParams(const char* prm)
    : TParams(prm)
    , PARAM_INIT(mRotSp, 0x80)
    , PARAM_INIT(mAccelRate, 0.029999999f)
    , PARAM_INIT(mBrake, 0.94999999f)
{
	TParams::load(mPrmPath);
}

TMario::TSurfingParams::TSurfingParams(const char* prm)
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
    , PARAM_INIT(mRoll, -0.45f)
    , PARAM_INIT(mPitch, -170.0f)
    , PARAM_INIT(mRollMax, 0x4000)
    , PARAM_INIT(mPitchMax, 0x1555)
    , PARAM_INIT(mAngleChangeRate, 0.01f)
    , PARAM_INIT(mWaistAngleChangeRate, 0.01f)
    , PARAM_INIT(mScaleMin, 0.5f)
    , PARAM_INIT(mScaleMax, 1.0f)
    , PARAM_INIT(mScaleMinSpeed, 24.0f)
    , PARAM_INIT(mScaleMaxSpeed, 60.0f)
    , PARAM_INIT(mJumpPow, 42.0f)
    , PARAM_INIT(mJumpXZRatio, 0.25f)
    , PARAM_INIT(mClashSpeed, 40.0f)
    , PARAM_INIT(mClashAngle, 0x5555)
{
	TParams::load(mPrmPath);
}

TMario::TUpperParams::TUpperParams(const char* prm)
    : TParams(prm)
    , PARAM_INIT(mPumpWaitTime, 10)
    , PARAM_INIT(mPumpAnmSpeed, 0.01f)
    , PARAM_INIT(mHoverHeadAngle, 0xe000)
    , PARAM_INIT(mFeelDeepHeadAngle, 0x2000)
    , PARAM_INIT(mFrontWallHeadAngle, 0xe000)
{
	TParams::load(mPrmPath);
}

TMario::TSlipParams::TSlipParams(const char* prm)
    : TParams(prm)
    , PARAM_INIT(mSlipFriction, 0.9f)
    , PARAM_INIT(mSlopeAcceleUp, 0.0f)
    , PARAM_INIT(mSlopeAcceleDown, 0.0f)
    , PARAM_INIT(mSlideAcceleUp, 0.0f)
    , PARAM_INIT(mSlideAcceleDown, 0.0f)
    , PARAM_INIT(mSlideStopNormal, 15.0f)
    , PARAM_INIT(mSlideStopCatch, 15.0f)
    , PARAM_INIT(mJumpEnable, 1)
    , PARAM_INIT(mMissJump, 1)
    , PARAM_INIT(mSlideAngleYSp, 0x200)
    , PARAM_INIT(mStickSlideMult, 0.05f)
{
	TParams::load(mPrmPath);
}

TMario::TEffectParams::TEffectParams()
    : TParams("/Mario/MarioEffect.prm")
    , PARAM_INIT(mDashInc, 0.033333335f)
    , PARAM_INIT(mDashDec, 0.016666668f)
    , PARAM_INIT(mDashMaxBlendInBlur, 0xb4)
    , PARAM_INIT(mDashMaxBlendInIris, 0xb4)
    , PARAM_INIT(mDashBlendScale, 0.2f)
{
	TParams::load(mPrmPath);
}

TMario::TParticleParams::TParticleParams()
    : TParams("/Mario/MarioParticle.prm")
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
	TParams::load(mPrmPath);
}

TMario::TMotorParams::TMotorParams()
    : TParams("/Mario/MarioMotor.prm")
    , PARAM_INIT(mMotorReturn, 0x19)
    , PARAM_INIT(mMotorTrample, 8)
    , PARAM_INIT(mMotorHipDrop, 0xf)
    , PARAM_INIT(mMotorWall, 6)
{
	TParams::load(mPrmPath);
}

TMario::TDirtyParams::TDirtyParams()
    : TParams("/Mario/Dirty.prm")
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
    , PARAM_INIT(mSlipRunSp, 0.01f)
    , PARAM_INIT(mSlipCatchSp, 0.01f)
    , PARAM_INIT(mSlipRotate, 0x100)
    , PARAM_INIT(mSlipCatchRotate, 0x100)
    , PARAM_INIT(mBrakeSlipNoPollute, 0.98f)
    , PARAM_INIT(mFogTimeYellow, 0xf0)
    , PARAM_INIT(mFogTimeRed, 600)
{
	TParams::load(mPrmPath);
}

TMario::TGraffitoParams::TGraffitoParams()
    : TParams("/Mario/Graffito.prm")
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
	TParams::load(mPrmPath);
}

TMario::TControllerParams::TControllerParams()
    : TParams("/Mario/Controller.prm")
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
	TParams::load(mPrmPath);
}

TMario::TYoshiParams::TYoshiParams()
    : TParams("/Mario/Yoshi.prm")
    , PARAM_INIT(mRunYoshiMult, 1.2f)
    , PARAM_INIT(mJumpYoshiMult, 1.0f)
    , PARAM_INIT(mRotYoshiMult, 1.5f)
    , PARAM_INIT(mHeadFront, 80.0f)
    , PARAM_INIT(mHeadRadius, 50.0f)
    , PARAM_INIT(mHoldOutAccCtrlF, 0.01f)
    , PARAM_INIT(mHoldOutAccCtrlB, 0.023f)
    , PARAM_INIT(mHoldOutSldCtrl, 0.3f)
    , PARAM_INIT(mDecBrake, 1.0f)
{
	TParams::load(mPrmPath);
}

TMario::TWaterEffectParams::TWaterEffectParams()
    : TParams("/Mario/WaterEffect.prm")
    , PARAM_INIT(mJumpIntoMdlEffectSpY, 10.0f)
    , PARAM_INIT(mJumpIntoMinY, 20.0f)
    , PARAM_INIT(mJumpIntoMaxY, 50.0f)
    , PARAM_INIT(mJumpIntoScaleMin, 0.75f)
    , PARAM_INIT(mJumpIntoScaleWidth, 1.0f)
    , PARAM_INIT(mRunningRippleSpeed, 30.0f)
    , PARAM_INIT(mRunningRippleDepth, 30.0f)
{
	TParams::load(mPrmPath);
}

TMario::TBarParams::TBarParams()
    : TParams("/Mario/Bar.prm")
    , PARAM_INIT(mClimbSp, 0.035f)
    , PARAM_INIT(mRotateSp, 3.0f)
    , PARAM_INIT(mClimbAnmRate, 0.00390625f)
    , PARAM_INIT(mCatchRadius, 100.0f)
    , PARAM_INIT(mCatchAngle, 0.8f)
{
	TParams::load(mPrmPath);
}

TMario::TPullParams::TPullParams(const char* prm)
    : TParams(prm)
    , PARAM_INIT(mPullRateV, 0.3f)
    , PARAM_INIT(mPullRateH, 0.05f)
    , PARAM_INIT(mOilPullRateV, 0.1f)
    , PARAM_INIT(mOilPullRateH, 0.01f)
{
	TParams::load(mPrmPath);
}

TMario::TWireParams::TWireParams()
    : TParams("/Mario/Wire.prm")
    , PARAM_INIT(mRotSpeed, -8)
    , PARAM_INIT(mRotSpeedTrgHover, 8)
    , PARAM_INIT(mRotSpeedTrgTurbo, 1000)
    , PARAM_INIT(mRotSpeedTrgRocket, 1000)
    , PARAM_INIT(mRotSpeedMax, 0x578)
    , PARAM_INIT(mRotStop, 100)
    , PARAM_INIT(mRotGravity, 0x14)
    , PARAM_INIT(mRotBrake, 0.98f)
    , PARAM_INIT(mJumpRate, 0.09f)
    , PARAM_INIT(mSwingRate, 0.005f)
    , PARAM_INIT(mWireJumpAccelControl, 0.01f)
    , PARAM_INIT(mWireJumpSlideControl, 0.3f)
    , PARAM_INIT(mWireJumpMult, 5.0f)
    , PARAM_INIT(mWireJumpBase, 20.0f)
    , PARAM_INIT(mWireSwingBrake, 0.99f)
    , PARAM_INIT(mWireSwingMax, 100.0f)
{
	TParams::load(mPrmPath);
}

TMario::THangRoofParams::THangRoofParams()
    : TParams("/Mario/HangRoof.prm")
    , PARAM_INIT(mAnmMult, 0.3f)
{
	TParams::load(mPrmPath);
}

TMario::THangingParams::THangingParams()
    : TParams("/Mario/Hanging.prm")
    , PARAM_INIT(mMoveSp, 0.1f)
    , PARAM_INIT(mAnmRate, 0.5f)
    , PARAM_INIT(mRapidTime, 2000)
    , PARAM_INIT(mLimitTime, 0x960)
    , PARAM_INIT(mAnmRapid, 8.0f)
    , PARAM_INIT(mDescentSp, 10.0f)
{
	TParams::load(mPrmPath);
}

TMario::TSwimParams::TSwimParams()
    : TParams("/Mario/Swim.prm")
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
	TParams::load(mPrmPath);
}

TMario::TRunParams::TRunParams()
    : TParams("/Mario/Run.prm")
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
    , PARAM_INIT(mInWaterBrake, 0.9f)
    , PARAM_INIT(mInWaterAnmBrake, 0.6f)
    , PARAM_INIT(mPumpingSlideSp, 0.1f)
    , PARAM_INIT(mPumpingSlideAnmSp, 0.5f)
    , PARAM_INIT(mDoJumpCatchSp, 15.0f)
    , PARAM_INIT(mTurnNeedSp, 10.0f)
    , PARAM_INIT(mDashRotSp, 100)
{
	TParams::load(mPrmPath);
}

TMario::TJumpParams::TJumpParams()
    : TParams("/Mario/Jump.prm")
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
    , PARAM_INIT(mPopUpForceYMult, 10.0f)
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
	TParams::load(mPrmPath);
}

TMario::TAttackParams::TAttackParams(const char* prm)
    : TParams(prm)
    , PARAM_INIT(mRadius, 100.0f)
    , PARAM_INIT(mHeight, 50.0f)
{
	TParams::load(mPrmPath);
}

TMario::TBodyAngleParams::TBodyAngleParams(const char* prm)
    : TParams(prm)
    , PARAM_INIT(mHeadRot, 0.0f)
    , PARAM_INIT(mWaistRoll, 0.0f)
    , PARAM_INIT(mWaistPitch, 80.0f)
    , PARAM_INIT(mWaistRollMax, 0)
    , PARAM_INIT(mWaistPitchMax, 1000)
    , PARAM_INIT(mWaistAngleChangeRate, 0.07f)
{
	TParams::load(mPrmPath);
}

TMario::TDeParams::TDeParams()
    : TParams("/Mario/Mario.prm")
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
    , PARAM_INIT(mInWaterSlip, 0.9f)
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
	TParams::load(mPrmPath);
}

void TMario::setGamePad(TMarioGamePad* pad) { mGamePad = pad; }

void TMario::resetHistory() { }

void TMario::initValues() { }

void TMario::loadAfter() { }

void TMario::load(JSUMemoryInputStream&) { }

TMario::TMario()
    : TTakeActor("HitActor")
    , mBodyAngleParamsFree("/Mario/BodyAngleFree.prm")
    , mBodyAngleParamsWaterGun("/Mario/BodyAngleWaterGun.prm")
    , mAttackParamsFencePunch("/Mario/AttackFencePunch.prm")
    , mAttackParamsKickRoof("/Mario/AttackKickRoof.prm")
    , mPullParamsBGBeak("/Mario/PullParamBGBeak.prm")
    , mPullParamsBGTentacle("/Mario/PullParamBGTentacle.prm")
    , mPullParamsBGFireWanWanBossTail(
          "/Mario/PullParamBGFireWanWanBossTail.prm")
    , mPullParamsFireWanWanTail("/Mario/PullParamFireWanWanTail.prm")
    , mSurfingParamsWaterRed("/Mario/SurfingWaterRed.prm")
    , mSurfingParamsGroundRed("/Mario/SurfingGroundRed.prm")
    , mSurfingParamsWaterYellow("/Mario/SurfingWaterYellow.prm")
    , mSurfingParamsGroundYellow("/Mario/SurfingGroundYellow.prm")
    , mSurfingParamsWaterGreen("/Mario/SurfingWaterGreen.prm")
    , mSurfingParamsGroundGreen("/Mario/SurfingGroundGreen.prm")
    , mHoverParams("/Mario/HHover.prm")
    , mDivingParams("/Mario/Diving.prm")
    , mSlipParamsNormal("/Mario/SlipParamNormal.prm")
    , mSlipParamsOil("/Mario/SlipParamOil.prm")
    , mSlipParamsAll("/Mario/SlipParamAll.prm")
    , mSlipParamsAllSlider("/Mario/SlipParamAll_Slider.prm")
    , mSlipParams45("/Mario/SlipParam45.prm")
    , mSlipParamsWaterSlope("/Mario/SlipParamWaterSlope.prm")
    , mSlipParamsWaterGround("/Mario/SlipParamWaterGround.prm")
    , mSlipParamsYoshi("/Mario/SlipParamYoshi.prm")
    , mUpperBodyParams("/Mario/UpperBody.prm")
    , mDmgParamsEnemyCommon("/Mario/DmgEnemyCommon.prm")
    , mDmgParamsHamakuri("/Mario/DmgHamakuri.prm")
    , mDmgParamsNamekuri("/Mario/DmgNamekuri.prm")
    , mDmgParamsHinokuri("/Mario/DmgHinokuri.prm")
    , mDmgParamsFire("/Mario/DmgFire.prm")
    , mDmgParamsBGTentacle("/Mario/DmgBGTentacle.prm")
    , mDmgParamsBossEel("/Mario/DmgBossEel.prm")
    , mDmgParamsHanachanBoss("/Mario/DmgHanachanBoss.prm")
    , mDmgParamsPoihana("/Mario/DmgPoihana.prm")
    , mDmgParamsKiller("/Mario/DmgKiller.prm")
    , mDmgParamsLampTrapIron("/Mario/DmgLampTrapIron.prm")
    , mDmgParamsLampTrapSpike("/Mario/DmgLampTrapSpike.prm")
    , mDmgParamsEnemyMario("/Mario/DmgEnemyMario.prm")
    , mDmgParamsCannotBreath("/Mario/DmgCannotBreath.prm")
    , mDmgParamsGraffitoFire("/Mario/DmgGraffitoFire.prm")
    , mDmgParamsGraffitoPoison("/Mario/DmgGraffitoPoison.prm")
    , mDmgParamsGraffitoElec("/Mario/DmgGraffitoElec.prm")
    , mDmgParamsGraffitoLava("/Mario/DmgGraffitoLava.prm")
    , mDmgParamsWaterSurface("/Mario/DmgGraffitoWaterSurface.prm")
    , mDmgMapParams0("/Mario/DmgMapCode0.prm")
    , mDmgMapParams1("/Mario/DmgMapCode1.prm")
    , mDmgMapParams2("/Mario/DmgMapCode2.prm")
    , mDmgMapParams3("/Mario/DmgMapCode3.prm")
    , mDmgMapParams4("/Mario/DmgMapCode4.prm")
    , mDmgMapParams5("/Mario/DmgMapCode5.prm")
    , mDmgMapParams6("/Mario/DmgMapCode6.prm")
    , mDmgMapParams7("/Mario/DmgMapCode7.prm")
    , mDmgMapParams8("/Mario/DmgMapCode8.prm")
    , mDmgMapParams9("/Mario/DmgMapCode9.prm")
    , mOptionParams("/Mario/Option.prm")
{
}
