#ifndef ENEMY_NPC_SAVE_HPP
#define ENEMY_NPC_SAVE_HPP

#include <System/ParamInst.hpp>
#include <System/Params.hpp>

class TNpcSaveStageFarClip : public TParams {
public:
	TNpcSaveStageFarClip();

	/* 0x8 */ TParamRT<f32> mSLFarAirport;
	/* 0x1C */ TParamRT<f32> mSLFarDolpicTown;
	/* 0x30 */ TParamRT<f32> mSLFarBiancoHills;
	/* 0x44 */ TParamRT<f32> mSLFarRiccoHarbor;
	/* 0x58 */ TParamRT<f32> mSLFarMammaBeach;
	/* 0x6C */ TParamRT<f32> mSLFarPinnaBeach;
	/* 0x80 */ TParamRT<f32> mSLFarPinnaParco;
	/* 0x94 */ TParamRT<f32> mSLFarSirenaBeach;
	/* 0xA8 */ TParamRT<f32> mSLFarHotelDelfino;
	/* 0xBC */ TParamRT<f32> mSLFarMareVillage;
	/* 0xD0 */ TParamRT<f32> mSLFarMonteVillage;
	/* 0xE4 */ TParamRT<f32> mSLFarCoronaMountain;
	/* 0xF8 */ TParamRT<f32> mSLFarOthers;
};

class TNpcSaveNormal : public TParams {
public:
	TNpcSaveNormal();

	/* 0x8 */ TParamRT<f32> mSLMarioTalkAcceptDegree;
	/* 0x1C */ TParamRT<f32> mTalkAcceptDist;
	/* 0x30 */ TParamRT<f32> mTalkAcceptHeight;
	/* 0x44 */ TParamRT<f32> mTalkAcceptDegree;
	/* 0x58 */ TParamRT<f32> mSLSitTalkAcceptDegree;
	/* 0x6C */ TParamRT<f32> mSLSunflowerLTalkDist;
	/* 0x80 */ TParamRT<f32> mSLThrowTalkAcceptDist;
	/* 0x94 */ TParamRT<f32> mSLThrowTalkAcceptHeight;
	/* 0xA8 */ TParamRT<s16> mSLThrowStartFrame;
	/* 0xBC */ TParamRT<s16> mSLTrampleShakeFrames;
	/* 0xD0 */ TParamRT<f32> mSLTrampleAmplitude;
	/* 0xE4 */ TParamRT<s16> mSLTrampleVelocity;
	/* 0xF8 */ TParamRT<s16> mSLTrampleToMadFrames;
	/* 0x10C */ TParamRT<f32> mSLHeadHeightNormal;
	/* 0x120 */ TParamRT<f32> mSLHeadHeightSandBomb;
	/* 0x134 */ TParamRT<f32> mSLBlownVelocity;
	/* 0x148 */ TParamRT<f32> mGravityY;
	/* 0x15C */ TParamRT<f32> mMoveWalkAnmRateChase;
	/* 0x170 */ TParamRT<f32> mStopWalkAnmRateChase;
	/* 0x184 */ TParamRT<s32> mStopWalkAnmRateFrame;
	/* 0x198 */ TParamRT<s32> mMotionBlendFrame;
	/* 0x1AC */ TParamRT<f32> mSLDanceAnmOffDist;
	/* 0x1C0 */ TParamRT<f32> mThrowSpeedXZ;
	/* 0x1D4 */ TParamRT<f32> mThrowSpeedY;
	/* 0x1E8 */ TParamRT<s32> mPosInbetweenFrame;
	/* 0x1FC */ TParamRT<s16> mSLGraphWanderMinFrame;
	/* 0x210 */ TParamRT<s16> mSLGraphWanderMaxFrame;
	/* 0x224 */ TParamRT<s16> mSLGraphWaitMinFrame;
	/* 0x238 */ TParamRT<s16> mSLGraphWaitMaxFrame;
	/* 0x24C */ TParamRT<f32> mSLCleanEffectScale;
	/* 0x260 */ TParamRT<f32> mSLSmokeRunMagnif;
	/* 0x274 */ TParamRT<f32> mSLFireDecSpeed;
};

class TNpcSaveIndividual : public TParams {
public:
	TNpcSaveIndividual(const char*);

	/* 0x8  */ TParamRT<f32> mSLBodyHeight;
	/* 0x1C  */ TParamRT<f32> mSLCursorHeight;
	/* 0x30  */ TParamRT<f32> mSLLookatHeight;
	/* 0x44  */ TParamRT<f32> mCircleShadowSize;
	/* 0x58  */ TParamRT<f32> mSLDamageRadiusSmall;
	/* 0x6C  */ TParamRT<f32> mSLMinWalkAnmRate;
	/* 0x80  */ TParamRT<f32> mSLMaxWalkAnmRate;
	/* 0x94  */ TParamRT<f32> mSLMaxRunAnmRate;
	/* 0xA8  */ TParamRT<f32> mSLMinMarchSpeed;
	/* 0xBC  */ TParamRT<f32> mMaxMarchSpeed;
	/* 0xD0  */ TParamRT<f32> mSLMaxRunSpeed;
	/* 0xE4  */ TParamRT<f32> mSLRunAccel;
	/* 0xF8  */ TParamRT<f32> mMarchAccel;
	/* 0x10C  */ TParamRT<f32> mMarchDecrease;
	/* 0x120  */ TParamRT<f32> mWalkTurnSpeed;
	/* 0x134  */ TParamRT<f32> mWaitTurnSpeed;
	/* 0x148  */ TParamRT<f32> mTurnAnmMinRate;
	/* 0x15C  */ TParamRT<f32> mTurnAnmMaxRate;
	/* 0x170  */ TParamRT<f32> mTurnAnmRate;
	/* 0x184  */ TParamRT<f32> mWaitAnmOffDist0;
	/* 0x198  */ TParamRT<f32> mWaitAnmOffDist1;
	/* 0x1AC  */ TParamRT<f32> mLodChangeDist;
	/* 0x1C0  */ TParamRT<f32> mAllDLLockDist;
	/* 0x1D4  */ TParamRT<f32> mNeckTurnSearchDist;
	/* 0x1E8  */ TParamRT<f32> mNeckTurnSearchHeight;
	/* 0x1FC  */ TParamRT<f32> mBodyTurnSearchDist;
	/* 0x210  */ TParamRT<f32> mBodyTurnSearchHeight;
	/* 0x224  */ TParamRT<f32> mBodyTurnSearchDegree;
	/* 0x238  */ TParamRT<f32> mBodyTurnSearchAware;
	/* 0x24C  */ TParamRT<f32> mMadSearchDist;
	/* 0x260  */ TParamRT<f32> mMadSearchHeight;
	/* 0x274  */ TParamRT<f32> mMadSearchDegree;
	/* 0x288  */ TParamRT<f32> mMadSearchAware;
	/* 0x29C  */ TParamRT<f32> mMadTurnSpeed;
	/* 0x2B0  */ TParamRT<f32> mFirstStateTurnSpeed;
	/* 0x2C4  */ TParamRT<f32> mUTurnSpeed;
	/* 0x2D8  */ TParamRT<f32> mSinkSpeed;
	/* 0x2EC  */ TParamRT<f32> mSinkHeight;
	/* 0x300  */ TParamRT<u8> mPollutionMax;
	/* 0x314  */ TParamRT<f32> mPollutionCleanSpeed;
	/* 0x328  */ TParamRT<s16> mNeckMinAngleX;
	/* 0x33C  */ TParamRT<s16> mNeckMaxAngleX;
	/* 0x350  */ TParamRT<s16> mNeckMaxAngleY;
	/* 0x364  */ TParamRT<s16> mNeckAngleXSpeed;
	/* 0x378  */ TParamRT<s16> mNeckAngleYSpeed;
};

class TNpcParams {
public:
	TNpcParams();

	/* 0x0 */ TNpcSaveStageFarClip* unk0;
	/* 0x4 */ TNpcSaveNormal* unk4;
	/* 0x8 */ TNpcSaveIndividual* unk8[29]; // TODO: is size right?
};

#endif
