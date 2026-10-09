#ifndef NOZZLEBASE_HPP
#define NOZZLEBASE_HPP

#include <M3DUtil/MActor.hpp>
#include <Player/ModelWaterManager.hpp>
#include <System/MarioGamePad.hpp>

class TWaterGun;

class TNozzleBase : public TParams {
public:
	/* 0x008 */ TParamRT<u8> mRocketType;
	/* 0x01C */ TParamRT<f32> mNum;
	/* 0x030 */ TParamRT<s16> mAttack;
	/* 0x044 */ TParamRT<f32> mDirTremble;
	/* 0x058 */ TParamRT<f32> mEmitPow;
	/* 0x06C */ TParamRT<f32> mEmitCtrl;
	/* 0x080 */ TParamRT<f32> mPowTremble;
	/* 0x094 */ TParamRT<f32> mSize;
	/* 0x0A8 */ TParamRT<f32> mSizeTremble;
	/* 0x0BC */ TParamRT<s32> mAmountMax;
	/* 0x0D0 */ TParamRT<f32> mReactionPow;
	/* 0x0E4 */ TParamRT<f32> mReactionY;
	/* 0x0F8 */ TParamRT<s16> mDecRate;
	/* 0x10C */ TParamRT<s16> mTriggerRate;
	/* 0x120 */ TParamRT<s32> mDamageLoss;
	/* 0x134 */ TParamRT<f32> mSuckRate;
	/* 0x148 */ TParamRT<f32> mHitRadius;
	/* 0x15C */ TParamRT<f32> mHitHeight;
	/* 0x170 */ TParamRT<s16> mLAngleBase;
	/* 0x184 */ TParamRT<s16> mLAngleNormal;
	/* 0x198 */ TParamRT<s16> mLAngleSquat;
	/* 0x1AC */ TParamRT<s16> mLAngleMin;
	/* 0x1C0 */ TParamRT<s16> mLAngleMax;
	/* 0x1D4 */ TParamRT<f32> mLAngleChase;
	/* 0x1E8 */ TParamRT<f32> mSizeMinPressure;
	/* 0x1FC */ TParamRT<f32> mSizeMaxPressure;
	/* 0x210 */ TParamRT<f32> mNumMin;
	/* 0x224 */ TParamRT<s16> mAttackMin;
	/* 0x238 */ TParamRT<f32> mDirTrembleMin;
	/* 0x24C */ TParamRT<f32> mEmitPowMin;
	/* 0x260 */ TParamRT<f32> mSizeMin;
	/* 0x274 */ TParamRT<f32> mMotorPowMin;
	/* 0x288 */ TParamRT<f32> mMotorPowMax;
	/* 0x29C */ TParamRT<f32> mReactionPowMin;
	/* 0x2B0 */ TParamRT<f32> mInsidePressureDec;
	/* 0x2C4 */ TParamRT<f32> mInsidePressureMax;
	/* 0x2D8 */ TParamRT<s16> mTriggerTime;
	/* 0x2EC */ TParamRT<s16> mType;
	/* 0x300 */ TParamRT<s16> mSideAngleMaxSide;
	/* 0x314 */ TParamRT<s16> mSideAngleMaxFront;
	/* 0x328 */ TParamRT<s16> mSideAngleMaxBack;
	/* 0x33C */ TParamRT<f32> mRButtonMult;
	/* 0x350 */ TParamRT<f32> mEmitPowScale;

	TNozzleBase(const char* name, const char* prm, TWaterGun* fludd);

	virtual void init();
	virtual s32 getNozzleKind() const { return 0; }
	virtual s16 getGunAngle() { return unk36E; }
	virtual s16 getWaistAngle() { return unk370; }
	virtual void movement(const TMarioControllerWork&);
	virtual void emitCommon(int, TWaterEmitInfo*);
	virtual void emit(int);
	virtual void animation(int);

	void calcGunAngle(const TMarioControllerWork&);
	MActor* getMActor() { return unk380; }

public:
	/* 0x368 */ TWaterGun* mFludd;
	/* 0x36C */ u16 unk36C; // Some animation state
	/* 0x36E */ s16 unk36E; // Gun angle
	/* 0x370 */ s16 unk370; // Waist angle
	/* 0x372 */ u16 unk372;
	/* 0x374 */ f32 unk374;
	/* 0x378 */ f32 unk378;
	/* 0x37C */ f32 unk37C;
	/* 0x380 */ MActor* unk380; // MActor
};

#endif
