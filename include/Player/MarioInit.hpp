#ifndef MARIOINIT_HPP
#define MARIOINIT_HPP

#include "System/ParamInst.hpp"

// TODO: move to a better file
#include "System/BaseParam.hpp"
class TParams {
    public:
        char *filename;
        TBaseParam *head;
};

// length 0x42C
class TDeParams : public TParams {
    public:
        TParamT<s16> hpMax;
        TParamT<f32> runningMax;
        TParamT<f32> dashMax;
        TParamT<f32> dashAcc;
        TParamT<f32> dashBrake;
        TParamT<s16> dashStartTime;
        TParamT<s16> waitingRotSp;
        TParamT<s16> runningRotSpMin;
        TParamT<s16> runningRotSpMax;
        TParamT<s16> rocketRotSp;
        TParamT<s16> pumpingRotSpMin;
        TParamT<s16> pumpingRotSpMax;
        TParamT<s16> invincibleTime;
        TParamT<s16> footPrintTimerMax;
        TParamT<u8> waterTriggerRate;
        TParamT<u8> graffitoNoDmgTime;
        TParamT<u8> restMax;
        TParamT<f32> shadowSize;
        TParamT<f32> shadowErase;
        TParamT<f32> holdRadius;
        TParamT<f32> damageRadius;
        TParamT<f32> damageHeight;
        TParamT<f32> attackHeight;
        TParamT<f32> trampleRadius;
        TParamT<f32> pushupRadius;
        TParamT<f32> pushupHeight;
        TParamT<f32> hipdropRadius;
        TParamT<f32> quakeRadius;
        TParamT<f32> quakeHeight;
        TParamT<f32> tramplePowStep1;
        TParamT<f32> tramplePowStep2;
        TParamT<f32> tramplePowStep3;
        TParamT<f32> jumpWallHeight;
        TParamT<f32> throwPower;
        TParamT<f32> slipStart;
        TParamT<f32> wasOnWaterSlip;
        TParamT<f32> inWaterSlip;
        TParamT<f32> toroccoRotSp;
        TParamT<s16> recoverTimer;
        TParamT<s16> hotTimer;
        TParamT<f32> feelDeep;
        TParamT<f32> damageFallHeight;
        TParamT<f32> forceSlipAngle;
        TParamT<f32> clashSpeed;
        TParamT<f32> hangWallMovableAngle;
        TParamT<f32> colMvMax;
        TParamT<s16> noFreezeTime;
        TParamT<s16> kickFreezeTime;
        TParamT<s16> surfStartFreezeTime;
        TParamT<f32> sleepingCheckDist;
        TParamT<f32> sleepingCheckHeight;
        TParamT<s16> illegalPlaneCtInc;
        TParamT<s16> illegalPlaneTime;
};

class TBodyAngleParams : public TParams {
    public:
        TParamT<f32> headRot;
        TParamT<f32> waistRoll;
        TParamT<f32> waistPitch;
        TParamT<s16> waistRollMax;
        TParamT<s16> waistPitchMax;
        TParamT<f32> waistAngleChangeRate;
};

class TAttackParams : public TParams {
    public:
        TParamT<f32> radius;
        TParamT<f32> height;
};

class THangingParams : public TParams {
    public:
        TParamT<f32> moveS;
        TParamT<f32> anmRate;
        TParamT<s16> rapidTime;
        TParamT<s16> limitTime;
        TParamT<f32> anmRapid;
        TParamT<f32> descentSp;
};

class TPullParams : public TParams {
    public:
        TParamT<f32> pullRateV;
        TParamT<f32> pullRateH;
        TParamT<f32> oilPullRateV;
        TParamT<f32> oilPullRateH;
};

class TDmgParams : public TParams {
    public:
        TParamT<u8> damage;
        TParamT<u8> downType;
        TParamT<u8> waterEmit;
        TParamT<u8> motor;
        TParamT<f32> minSpeed;
        TParamT<f32> dirty;
        TParamT<s16> invincibleTime;
};

class TUpperBodyParams : public TParams {
    public:
        TParamT<s16> pumpWaitTime;
        TParamT<f32> pumpAnmSpeed;
        TParamT<s16> hoverHeadAngle;
        TParamT<s16> feelDeepHeadAngle;
        TParamT<s16> frontWallHeadAngle;
};

class TBarParams : public TParams {
    public:
        TParamT<f32> climbSp;
        TParamT<f32> rotateSp;
        TParamT<f32> climbAnmRate;
        TParamT<f32> catchRadius;
        TParamT<f32> catchAngle;
};

class TSoundParams : public TParams {
    public:
        TParamT<f32> startFallVoiceSpeed;
};

class TOptionParams : public TParams {
    public:
        TParamT<f32> z;
        TParamT<f32> xMin;
        TParamT<f32> xMax;
};

class TMarioEffectParams : public TParams {
    public:
        TParamT<f32> dashInc;
        TParamT<f32> dashDec;
        TParamT<u8> dashMaxBlendInBlur;
        TParamT<u8> dashMaxBlendInIris;
        TParamT<f32> dashBlendScale;
};

class THoverParams : public TParams {
    public:
        TParamT<s16> rotSp;
        TParamT<f32> accelRate;
        TParamT<f32> brake;
};

class TDivingParams : public TParams {
    public:
        TParamT<s16> rotSp;
        TParamT<f32> gravity;
        TParamT<f32> accelControl;
        TParamT<f32> seaBrake;
        TParamT<f32> seaBrakeY;
};

class TYoshiParams : public TParams {
    public:
        TParamT<f32> runYoshiMult;
        TParamT<f32> jumpYoshiMult;
        TParamT<f32> rotYoshiMult;
        TParamT<f32> headFront;
        TParamT<f32> headRadius;
        TParamT<f32> holdOutAccCtrlF;
        TParamT<f32> holdOutAccCtrlB;
        TParamT<f32> holdOutSldCtrl;
        TParamT<f32> decBrake;
};

class THangRoofParams : public TParams {
    public:
        TParamT<f32> anmMult;
};

class TWireParams : public TParams {
    public:
        TParamT<s16> rotSpeed;
        TParamT<s16> rotSpeedTrgHover;
        TParamT<s16> rotSpeedTrgTurbo;
        TParamT<s16> rotSpeedTrgRocket;
        TParamT<s16> rotSpeedMax;
        TParamT<s16> rotStop;
        TParamT<s16> rotGravity;
        TParamT<f32> rotBrake;
        TParamT<f32> jumpRate;
        TParamT<s32> swingRate;
        TParamT<f32> wireJumpAccelControl;
        TParamT<f32> wireJumpSlideControl;
        TParamT<f32> wireJumpMult;
        TParamT<f32> wireJumpBase;
        TParamT<f32> wireSwingBrake;
        TParamT<f32> wireSwingMax;
};

class TAutoDemoParams : public TParams {
    public:
        TParamT<s16> warpInBallsDispTime;
        TParamT<s16> warpInBallsTime;
        TParamT<s16> warpInCapturedTime;
        TParamT<f32> warpInTremble;
        TParamT<f32> warpInVecBase;
        TParamT<f32> warpTransTremble;
        TParamT<s16> readRotSp;
};

class TSlipParams : TParams {
    public:
        TParamT<f32> slipFriction;
        TParamT<f32> slopeAccelUp;
        TParamT<f32> slopeAccelDown;
        TParamT<f32> slideAccelUp;
        TParamT<f32> slideAccelDown;
        TParamT<f32> slideAccelNormal;
        TParamT<f32> slideStopCatch;
        TParamT<u8> jumpEnable;
        TParamT<f32> missJump;
        TParamT<f32> slideAngleYSp;
        TParamT<f32> stickSlideMult;
};

class TWaterEffectParams : public TParams {
    public:
        TParamT<f32> jumpIntoMdlEffectSpY;
        TParamT<f32> jumpIntoMinY;
        TParamT<f32> jumpIntoMaxY;
        TParamT<f32> jumpIntoScaleMin;
        TParamT<f32> jumpIntoScaleWidth;
        TParamT<f32> runningRippleSpeed;
        TParamT<f32> runningRippleDepth;
};

class TDirtyParams : TParams {
    public:
        TParamT<f32> incRunning;
        TParamT<f32> incCatching;
        TParamT<f32> incSlipping;
        TParamT<f32> decSwimming;
        TParamT<f32> decWaterHit;
        TParamT<f32> decRotJump;
        TParamT<f32> brakeStartValSlip;
        TParamT<f32> brakeStartValRun;
        TParamT<s16> dirtyTimeSlip;
        TParamT<s16> dirtyTimeRun;
        TParamT<f32> polSizeSlip;
        TParamT<f32> polSizeRun;
        TParamT<f32> polSizeFootPrint;
        TParamT<f32> polSizeJump;
        TParamT<f32> slopeAngle;
        TParamT<f32> dirtyMax;
        TParamT<f32> slipAnmSpeed;
        TParamT<f32> slipRunSp;
        TParamT<f32> slipCatchSp;
        TParamT<s16> slipRotate;
        TParamT<s16> slipCatchRotate;
        TParamT<f32> brakeSlipNoPollute;
        TParamT<s16> fogTimeYellow;
        TParamT<s16> fogTimeRed;
};

class TMarioMotorParams : TParams {
    public:
        TParamT<s16> motorReturn;
        TParamT<s16> motorTrample;
        TParamT<s16> motorHipDrop;
        TParamT<s16> motorWall;
};

class TSwimParams : TParams {
    public:
        TParamT<f32> startSp;
        TParamT<f32> moveSp;
        TParamT<f32> moveBrake;
        TParamT<s16> swimmingRotSpMin;
        TParamT<s16> swimmingRotSpMax;
        TParamT<s16> pumpingRotSpMin;
        TParamT<s16> pumpingRotSpMax;
        TParamT<f32> gravity;
        TParamT<f32> waitBouyancy;
        TParamT<f32> moveBouyancy;
        TParamT<f32> upDownBrake;
        TParamT<f32> canJumpDepth;
        TParamT<f32> endDepth;
        TParamT<f32> floatHeight;
        TParamT<f32> startVMult;
        TParamT<f32> startVYMult;
        TParamT<f32> rush;
        TParamT<f32> anmBrake;
        TParamT<f32> paddleSpeedUp;
        TParamT<f32> paddleJumpUp;
        TParamT<f32> floatUp;
        TParamT<f32> waterLevelCheckHeight;
        TParamT<f32> paddleDown;
        TParamT<s16> waitSinkTime;
        TParamT<f32> canBreathDepth;
        TParamT<f32> waitSinkSpeed;
        TParamT<f32> airDec;
        TParamT<f32> airDecDive;
        TParamT<f32> airInc;
};

class TMarioParticleParams : public TParams {
    public:
        TParamT<f32> meltInWaterMax;
        TParamT<f32> waveEmitSpeed;
        TParamT<s16> waveAlphaDec;
        TParamT<f32> bubbleDepth;
        TParamT<f32> bodyBubbleSpMin;
        TParamT<f32> bodyBubbleSpMax;
        TParamT<f32> bodyBubbleEmitMin;
        TParamT<f32> bodyBubbleEmitMax;
        TParamT<f32> bubbleToRipple;
        TParamT<f32> toroccoWind;
        TParamT<f32> toroccoSpark;
};

class TSurfingParams : TParams {
    public:
        TParamT<f32> rotMin;
        TParamT<f32> rotMax;
        TParamT<f32> powMin;
        TParamT<f32> powMax;
        TParamT<f32> accel;
        TParamT<f32> waistRoll;
        TParamT<f32> waistPitch;
        TParamT<s16> waistRollMax;
        TParamT<s16> waistPitchMax;
        TParamT<s32> roll;
        TParamT<f32> pitch;
        TParamT<s16> rollMax;
        TParamT<s16> pitchMax;
        TParamT<f32> angleChangeRate;
        TParamT<f32> waistAngleChangeRate;
        TParamT<f32> scaleMin;
        TParamT<f32> scaleMax;
        TParamT<f32> scaleMinSpeed;
        TParamT<f32> scaleMaxSpeed;
        TParamT<f32> jumpPow;
        TParamT<f32> jumpXZRatio;
        TParamT<f32> clashSpeed;
        TParamT<s16> clashAngle;
};

class TControllerParams : TParams {
    public:
        TParamT<u8> analogLRToZeroVal;
        TParamT<u8> analogLRToMiddleVal;
        TParamT<u8> analogLRToMaxVal;
        TParamT<f32> analogLRMiddleLevel;
        TParamT<f32> startToWalkLevel;
        TParamT<s16> stickRotateTime;
        TParamT<s16> lengthMultTimes;
        TParamT<f32> lengthMult;
        TParamT<f32> squatRotMidAnalog;
        TParamT<f32> squatRotMidValue;
};

class TJumpParams : TParams {
    public:
        TParamT<f32> gravity;
        TParamT<f32> spinJumpGravity;
        TParamT<f32> jumpingMax;
        TParamT<f32> jumpSpeedBrake;
        TParamT<f32> jumpAccelControl;
        TParamT<f32> jumpSlideControl;
        TParamT<f32> turnJumpForce;
        TParamT<f32> fenceSpeed;
        TParamT<f32> fireDownForce;
        TParamT<f32> fireDownControl;
        TParamT<f32> fireBackVelocity;
        TParamT<f32> broadJumpForce;
        TParamT<f32> broadJumpForceY;
        TParamT<f32> rotateJumpForceY;
        TParamT<f32> popUpSpeedY;
        TParamT<f32> popUpSpeedYMult;
        TParamT<f32> backJumpForce;
        TParamT<f32> backJumpForceY;
        TParamT<f32> hipAttackSpeedY;
        TParamT<f32> superHipAttackSpeedY;
        TParamT<s16> jumpCatchRotXSp;
        TParamT<s16> jumpCatchRotXMax;
        TParamT<f32> rotBroadEnableV;
        TParamT<f32> rotBroadJumpForce;
        TParamT<f32> rotBroadJumpForceY;
        TParamT<f32> trampolineDec;
        TParamT<f32> secJumpEnableSp;
        TParamT<f32> secJumpForce;
        TParamT<f32> secJumpSpeedMult;
        TParamT<f32> secJumpXZMult;
        TParamT<f32> triJumpEnableSp;
        TParamT<f32> ultraJumpForce;
        TParamT<f32> ultraJumpSpeedMult;
        TParamT<f32> ultraJumpXZMult;
        TParamT<f32> valleyDepth;
        TParamT<f32> thrownAccel;
        TParamT<f32> thrownSlide;
        TParamT<f32> thrownBrake;
        TParamT<f32> tremblePower;
        TParamT<f32> trembleAccele;
        TParamT<f32> trembleBrake;
        TParamT<s16> trembleTime;
        TParamT<s16> clashAngle;
        TParamT<f32> jumpJumpCatchSp;
        TParamT<f32> getOffYoshiY;
        TParamT<s16> superHipAttackCt;
};

class TRunParams : TParams {
    public:
        TParamT<f32> maxSpeed;
        TParamT<f32> velMinusBrake;
        TParamT<f32> addBase;
        TParamT<f32> addVelDiv;
        TParamT<f32> decStartNrmY;
        TParamT<f32> decBrake;
        TParamT<f32> soft2Walk;
        TParamT<f32> walk2Soft;
        TParamT<f32> softStepAnmMult;
        TParamT<f32> runAnmSpeedBase;
        TParamT<f32> runAnmSpeedMult;
        TParamT<f32> motBlendWalkSp;
        TParamT<f32> motBlendRunSp;
        TParamT<f32> swimDepth;
        TParamT<f32> inWaterBrake;
        TParamT<f32> inWaterAnmBrake;
        TParamT<f32> pumpingSlideSp;
        TParamT<f32> pumpingSlideAnmSp;
        TParamT<f32> doJumpCatchSp;
        TParamT<f32> turnNeedSp;
        TParamT<s16> dashRotSp;
};

class TGraffitoParams : TParams {
    public:
        TParamT<s16> sinkTime;
        TParamT<s16> sinkDmgTime;
        TParamT<f32> sinkHeight;
        TParamT<f32> sinkMoveMin;
        TParamT<f32> sinkMoveMax;
        TParamT<f32> sinkRecover;
        TParamT<f32> sinkJumpRateMin;
        TParamT<f32> sinkJumpRateMax;
        TParamT<f32> sinkPumpLimit;
        TParamT<f32> field14_0xbc;
        TParamT<f32> fireHeight;
        TParamT<s16> dizzySlipCtMax;
        TParamT<s16> dizzyWalkCtMax;
        TParamT<s16> dizzyAngleY;
        TParamT<f32> dizzyAngleRate;
        TParamT<f32> dizzyPowerRate;
        TParamT<f32> dizzyPower;
        TParamT<s16> fireInvincibleTime;
        TParamT<s16> footEraseTimes;
        TParamT<f32> footEraseSize;
        TParamT<f32> footEraseFront;
};

#endif
