#ifndef NOZZLEBASE_HPP
#define NOZZLEBASE_HPP

#include <M3DUtil/MActor.hpp>
#include <Player/ModelWaterManager.hpp>
#include <System/MarioGamePad.hpp>

class TWaterGun;

class TNozzleBase {
public:
	struct TEmitParams : public TParams {
		TEmitParams(const char* prm)
		    : TParams(prm)
		    , PARAM_INIT(mRocketType, 0)
		    , PARAM_INIT(mNum, 1.0f)
		    , PARAM_INIT(mAttack, 1)
		    , PARAM_INIT(mDirTremble, 0.0099999998)
		    , PARAM_INIT(mEmitPow, 40.0)
		    , PARAM_INIT(mEmitCtrl, 1.0f)
		    , PARAM_INIT(mPowTremble, 1.0f)
		    , PARAM_INIT(mSize, 40.0f)
		    , PARAM_INIT(mSizeTremble, 16.0f)
		    , PARAM_INIT(mAmountMax, 0x834)
		    , PARAM_INIT(mReactionPow, 0.0f)
		    , PARAM_INIT(mReactionY, 0.0f)
		    , PARAM_INIT(mDecRate, 0)
		    , PARAM_INIT(mTriggerRate, 0x100)
		    , PARAM_INIT(mDamageLoss, 0xfa)
		    , PARAM_INIT(mSuckRate, 0.1f)
		    , PARAM_INIT(mHitRadius, 50.0f)
		    , PARAM_INIT(mHitHeight, 80.0f)
		    , PARAM_INIT(mLAngleBase, 0x1000)
		    , PARAM_INIT(mLAngleNormal, 12000)
		    , PARAM_INIT(mLAngleSquat, 12000)
		    , PARAM_INIT(mLAngleMin, -0x2000)
		    , PARAM_INIT(mLAngleMax, 0x2000)
		    , PARAM_INIT(mLAngleChase, 0.1f)
		    , PARAM_INIT(mSizeMinPressure, 0.0f)
		    , PARAM_INIT(mSizeMaxPressure, 1.0f)
		    , PARAM_INIT(mNumMin, 1.0f)
		    , PARAM_INIT(mAttackMin, 1)
		    , PARAM_INIT(mDirTrembleMin, 0.0099999998)
		    , PARAM_INIT(mEmitPowMin, 40.0f)
		    , PARAM_INIT(mSizeMin, 40.0f)
		    , PARAM_INIT(mMotorPowMin, 5.0f)
		    , PARAM_INIT(mMotorPowMax, 25.0f)
		    , PARAM_INIT(mReactionPowMin, 0.0f)
		    , PARAM_INIT(mInsidePressureDec, 100.0f)
		    , PARAM_INIT(mInsidePressureMax, 4500.0f)
		    , PARAM_INIT(mTriggerTime, 1)
		    , PARAM_INIT(mType, 0)
		    , PARAM_INIT(mSideAngleMaxSide, 0x4000)
		    , PARAM_INIT(mSideAngleMaxFront, 0x4000)
		    , PARAM_INIT(mSideAngleMaxBack, 0x2000)
		    , PARAM_INIT(mRButtonMult, 10000.0)
		    , PARAM_INIT(mEmitPowScale, 10.0f)

		{
		}
		// TODO: Validate these offsets...
		/* 0x00 */ TParamRT<u8> mRocketType;
		/* 0x14 */ TParamRT<f32> mNum;
		/* 0x28 */ TParamRT<s16> mAttack;
		/* 0x3C */ TParamRT<f32> mDirTremble;
		/* 0x50 */ TParamRT<f32> mEmitPow;
		/* 0x64 */ TParamRT<f32> mEmitCtrl;
		/* 0x78 */ TParamRT<f32> mPowTremble;
		/* 0x8C */ TParamRT<f32> mSize;
		/* 0xA0 */ TParamRT<f32> mSizeTremble;
		/* 0xB4 */ TParamRT<s32> mAmountMax;
		/* 0xC8 */ TParamRT<f32> mReactionPow;
		/* 0xDC */ TParamRT<f32> mReactionY;
		/* 0xF0 */ TParamRT<s16> mDecRate;
		/* 0x104 */ TParamRT<u16> mTriggerRate;
		/* 0x118 */ TParamRT<s32> mDamageLoss;
		/* 0x12C */ TParamRT<f32> mSuckRate;
		/* 0x140 */ TParamRT<f32> mHitRadius;
		/* 0x154 */ TParamRT<f32> mHitHeight;
		/* 0x168 */ TParamRT<s16> mLAngleBase;
		/* 0x17C */ TParamRT<s16> mLAngleNormal;
		/* 0x190 */ TParamRT<s16> mLAngleSquat;
		/* 0x1A4 */ TParamRT<s16> mLAngleMin;
		/* 0x1B8 */ TParamRT<s16> mLAngleMax;
		/* 0x1CC */ TParamRT<f32> mLAngleChase;
		/* 0x1E0 */ TParamRT<f32> mSizeMinPressure;
		/* 0x1F4 */ TParamRT<f32> mSizeMaxPressure;
		/* 0x208 */ TParamRT<f32> mNumMin;
		/* 0x21C */ TParamRT<s16> mAttackMin;
		/* 0x230 */ TParamRT<f32> mDirTrembleMin;
		/* 0x244 */ TParamRT<f32> mEmitPowMin;
		/* 0x258 */ TParamRT<f32> mSizeMin;
		/* 0x26C */ TParamRT<f32> mMotorPowMin;
		/* 0x280 */ TParamRT<f32> mMotorPowMax;
		/* 0x294 */ TParamRT<f32> mReactionPowMin;
		/* 0x2C0 */ TParamRT<f32> mInsidePressureDec; // These should be correct
		/* 0x2D4 */ TParamRT<f32> mInsidePressureMax;
		/* 0x2E8 */ TParamRT<s16> mTriggerTime;
		/* 0x2E4 */ TParamRT<s16> mType;
		/* 0x2F8 */ TParamRT<s16> mSideAngleMaxSide;
		/* 0x30C */ TParamRT<s16> mSideAngleMaxFront;
		/* 0x320 */ TParamRT<s16> mSideAngleMaxBack;
		/* 0x334 */ TParamRT<f32> mRButtonMult;
		/* 0x348 */ TParamRT<f32> mEmitPowScale;
	} mEmitParams; // 0x0000

	TNozzleBase(const char* name, const char* prm, TWaterGun* fludd);

	virtual void init();
	virtual inline s32 getNozzleKind() const;
	virtual inline s16 getGunAngle() { return unk36E; }
	virtual inline s16 getWaistAngle() { return unk370; }
	virtual void movement(const TMarioControllerWork&);
	virtual void emitCommon(int, TWaterEmitInfo*);
	virtual void emit(int);
	virtual void animation(int);

	void calcGunAngle(const TMarioControllerWork&);

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
