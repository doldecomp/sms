#ifndef ENEMY_BOSSHANACHANCHANGESAVEPARAMS_HPP
#define ENEMY_BOSSHANACHANCHANGESAVEPARAMS_HPP

#include <System/ParamInst.hpp>
#include <System/Params.hpp>

class TBossHanachanChangeSaveParams : public TParams {
public:
	TBossHanachanChangeSaveParams(const char*);

	/* 0x8 */ TParamRT<f32> mSLWalkBckRateMagnif;
	/* 0x1C */ TParamRT<f32> mSLWalkBckRateMin;
	/* 0x30 */ TParamRT<f32> mSLWalkAnmMarchSpeed;
	/* 0x44 */ TParamRT<f32> mSLRunAnmMarchSpeed;
	/* 0x58 */ TParamRT<f32> mSLMaxMarchSpeed;
	/* 0x6C */ TParamRT<f32> mSLMarchAccel;
	/* 0x80 */ TParamRT<f32> mSLMarchDecrease;
	/* 0x94 */ TParamRT<f32> mSLWalkTurnSpeed;
	/* 0xA8 */ TParamRT<f32> mSLFallDecideRotateZ;
	/* 0xBC */ TParamRT<f32> mSLWaveFallDownSpeed;
	/* 0xD0 */ TParamRT<f32> mSLFallDecideMinSpeed;
	/* 0xE4 */ TParamRT<f32> mSLSandSlopeForce;
	/* 0xF8 */ TParamRT<f32> mSLMaxRotateZNotSand;
	/* 0x10C */ TParamRT<f32> mSLRotateZLeanSpeed;
	/* 0x120 */ TParamRT<f32> mSLRotateZRestorationSpeed;
	/* 0x134 */ TParamRT<f32> mSLDiffMaxRotateZ;
	/* 0x148 */ TParamRT<f32> mSLCentrifugalForce;
	/* 0x15C */ TParamRT<f32> mSLCentrifugalSpeed;
	/* 0x170 */ TParamRT<f32> mSLWaveVelocity;
	/* 0x184 */ TParamRT<f32> mSLWaveDecrease;
	/* 0x198 */ TParamRT<s16> mSLDownFrames;
	/* 0x1AC */ TParamRT<s16> mSLDamageFrames;
	/* 0x1C0 */ TParamRT<s16> mSLNotFallDownFrames;
	/* 0x1D4 */ TParamRT<u8> mSLNormalBckFrameDiff;
	/* 0x1E8 */ TParamRT<u8> mSLGetUpFrameDiff;
	/* 0x1FC */ TParamRT<u8> mSLSnortFrameDiff;
	/* 0x210 */ TParamRT<u8> mSLDamageFrameDiff;
	/* 0x224 */ TParamRT<u8> mSLDeadFrameDiff;
	/* 0x238 */ TParamRT<f32> mSLParticleProbability;
	/* 0x24C */ TParamRT<f32> mSLThrowTotalPower;
	/* 0x260 */ TParamRT<f32> mSLThrowSpeedMin;
	/* 0x274 */ TParamRT<f32> mSLThrowSpeedMax;
	/* 0x288 */ TParamRT<f32> mSLThrowMoveDirPower;
	/* 0x29C */ TParamRT<f32> mSLThrowVecY;
};

class TBossHanachanCommonSaveParams : public TParams {
public:
	TBossHanachanCommonSaveParams(const char*);

	/* 0x8 */ TParamRT<f32> mSLViewClipFar;
	/* 0x1C */ TParamRT<f32> mSLViewClipRadius;
	/* 0x30 */ TParamRT<f32> mSLHeadHitOffsetY;
	/* 0x44 */ TParamRT<f32> mSLHeadAttackRadius;
	/* 0x58 */ TParamRT<f32> mSLHeadAttackHeight;
	/* 0x6C */ TParamRT<f32> mSLHeadDamageRadius;
	/* 0x80 */ TParamRT<f32> mSLHeadDamageHeight;
	/* 0x94 */ TParamRT<f32> mSLBodyHitOffsetY;
	/* 0xA8 */ TParamRT<f32> mSLBodyAttackRadius;
	/* 0xBC */ TParamRT<f32> mSLBodyAttackHeight;
	/* 0xD0 */ TParamRT<f32> mSLBodyDamageRadius;
	/* 0xE4 */ TParamRT<f32> mSLBodyDamageHeight;
	/* 0xF8 */ TParamRT<f32> mSLFootHitOffsetY;
	/* 0x10C */ TParamRT<f32> mSLFootAttackRadius;
	/* 0x120 */ TParamRT<f32> mSLFootAttackHeight;
	/* 0x134 */ TParamRT<f32> mSLFootDamageRadius;
	/* 0x148 */ TParamRT<f32> mSLFootDamageHeight;
	/* 0x15C */ TParamRT<f32> mSLHeadLength;
	/* 0x170 */ TParamRT<f32> mSLBodyLength;
	/* 0x184 */ TParamRT<f32> mSLHeadShadowSize;
	/* 0x198 */ TParamRT<f32> mSLBodyShadowSize;
	/* 0x1AC */ TParamRT<f32> mSLHeadPlusYByRotateZ;
	/* 0x1C0 */ TParamRT<f32> mSLBodyPlusYByRotateZ;
	/* 0x1D4 */ TParamRT<f32> mSLRecoverSearchDist;
	/* 0x1E8 */ TParamRT<f32> mSLRecoverSearchDegree;
	/* 0x1FC */ TParamRT<f32> mSLShineAppearOffsetY;
	/* 0x210 */ TParamRT<f32> mSLCamShakeZeroDist;
	/* 0x224 */ TParamRT<f32> mSLCamShakeMaxDist;
	/* 0x238 */ TParamRT<s16> mSLMotionBlendFrames;
};

#endif // ENEMY_BOSSHANACHANCHANGESAVEPARAMS_HPP
