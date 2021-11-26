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

#endif
