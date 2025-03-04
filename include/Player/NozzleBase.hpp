#ifndef NOZZLEBASE_HPP
#define NOZZLEBASE_HPP

#include <Dolphin/types.h>
#include <JSystem/JGeometry.hpp>
#include "System/ParamInst.hpp"
#include "System/Params.hpp"
#include <M3DUtil/MActor.hpp>

// struct TWaterEmitInfo : public TParams {
//     TWaterEmitInfo(const char *prm);

//     TParamT<s32> mNum;
//     TParamT<s16> mAlive;
//     TParamT<s16> mAttack;
//     TParamT<JGeometry::TVec3<f32>> mDir;
//     TParamT<JGeometry::TVec3<f32>> mPos;
//     TParamT<JGeometry::TVec3<f32>> mV;
//     TParamT<f32> mDirTremble;
//     TParamT<f32> mPow;
//     TParamT<f32> mPowTremble;
//     TParamT<f32> mSize;
//     TParamT<f32> mSizeTremble;
//     TParamT<f32> mHitRadius;
//     TParamT<f32> mHitHeight;
//     TParamT<s32> mFlag;
//     TParamT<s16> mType;

// private:
//     TParamT<s16> __padding;
// };

class TWaterGun;
// class TMarioControllerWork;

class TNozzleBase {
public:
	struct TEmitParams : public TParams {
		TParamT<u8> mRocketType;
		TParamT<f32> mNum;
		TParamT<u16> mAttack;
		TParamT<f32> mDirTremble;
		TParamT<f32> mEmitPow;
		TParamT<f32> mEmitCtrl;
		TParamT<f32> mPowTremble;
		TParamT<f32> mSize;
		TParamT<f32> mSizeTremble;
		TParamT<s32> mAmountMax;
		TParamT<f32> mReactionPow;
		TParamT<f32> mReactionY;
		TParamT<s16> mDecRate;
		TParamT<s16> mTriggerRate;
		TParamT<s32> mDamageLoss;
		TParamT<f32> mSuckRate;
		TParamT<f32> mHitRadius;
		TParamT<f32> mHitHeight;
		TParamT<s16> mLAngleBase;
		TParamT<s16> mLAngleNormal;
		TParamT<s16> mLAngleSquat;
		TParamT<s16> mLAngleMin;
		TParamT<s16> mLAngleMax;
		TParamT<s16> mLAngleChase;
		TParamT<f32> mSizeMinPressure;
		TParamT<f32> mSizeMaxPressure;
		TParamT<f32> mNumMin;
		TParamT<s16> mAttackMin;
		TParamT<f32> mDirTrembleMin;
		TParamT<f32> mEmitPowMin;
		TParamT<f32> mSizeMin;
		TParamT<f32> mMotorPowMin;
		TParamT<f32> mMotorPowMax;
		TParamT<f32> mReactionPowMin;
		TParamT<f32> mInsidePressureDec;
		TParamT<f32> mInsidePressureMax;
		TParamT<s16> mTriggerTime;
		TParamT<s16> mType;
		TParamT<s16> mSideAngleMaxSide;
		TParamT<s16> mSideAngleMaxFront;
		TParamT<s16> mSideAngleMaxBack;
		TParamT<f32> mRButtonMult;
		TParamT<f32> mEmitPowScale;
	} mEmitParams; // 0x0000

	TNozzleBase(const char* name, const char* prm, TWaterGun* fludd);

	virtual void init();
	virtual s32 getNozzleKind() const;
	virtual s16 getGunAngle();
	virtual s16 getWaistAngle();
	virtual void movement(const void*);
	virtual void emitCommon(int, u32* /* WaterEmitInfo*/);
	virtual void emit(int);
	virtual void animation(int);

	void calcGunAngle(const void*);

	TWaterGun* mFludd; // 0x0368
	u16 mAnimState;
	s16 mGunAngle;
	s16 mWaistAngle;
	f32 _374;
	f32 _378;
	f32 _37C;
	MActor* mActor;
}; // 0x384

#endif
