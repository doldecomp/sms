#ifndef JPA_SWEEP_SHAPE_HPP
#define JPA_SWEEP_SHAPE_HPP

#include <dolphin/types.h>
#include <JSystem/JUtility/JUTColor.hpp>
#include <JSystem/JParticle/JPADataBlock.hpp>
#include <dolphin/gx/GXStruct.h>

class JPASweepShape : public JPADataBlock {
public:
	JPASweepShape(const u8*);
	virtual ~JPASweepShape() { }

	u8 getType() { return mType; }
	u8 getDirType() { return mDirType; }
	u8 getRotType() { return mRotType; }
	s16 getLife() { return mLife; }
	s16 getRate() { return mRate; }
	f32 getTiming() { return mTiming; }
	s32 getStep() { return mStep; }
	f32 getPosRndm() { return unk8; }
	f32 getVelInhRate() { return mVelInhRate; }
	f32 getBaseVel() { return mBaseVel; }
	f32 getBaseVelRndm() { return mBaseVelRndm; }
	BOOL isEnableDrawParent() { return unk49 & 1; }
	BOOL isClipOn() { return unk49 & 2; }
	BOOL isEnableScaleOut() { return mEnableScaleOut; }
	BOOL isEnableAlphaOut() { return mEnableAlphaOut; }
	BOOL isEnableRotate() { return mEnableRotate; }
	BOOL isInheritedScale() { return unk4E & 1; }
	BOOL isInheritedAlpha() { return unk4E & 2; }
	BOOL isInheritedRGB() { return unk4E & 4; }
	u8 getTextureIndex() { return mTextureIndex; }
	f32 getScaleX() { return mScaleX; }
	f32 getScaleY() { return mScaleY; }
	GXColor getPrm() { return mPrmColor; }
	GXColor getEnv() { return mEnvColor; }
	u8 getPrmAlpha() { return mPrmColor.a; }
	u8 getEnvAlpha() { return mEnvColor.a; }
	f32 getRotateSpeed() { return mRotateSpeed; }
	f32 getInheritScale() { return mInheritScale; }
	f32 getInheritAlpha() { return mInheritAlpha; }
	f32 getInheritRGB() { return mInheritRGB; }

	// TODO: names might be wrong
	f32 getMoment() { return unk24; }

public:
	/* 0x8 */ f32 unk8;
	/* 0xC */ f32 mBaseVel;
	/* 0x10 */ f32 mScaleY;
	/* 0x14 */ f32 mScaleX;
	/* 0x18 */ f32 mTiming;
	/* 0x1C */ f32 mVelInhRate;
	/* 0x20 */ f32 mBaseVelRndm;
	/* 0x24 */ f32 unk24;
	/* 0x28 */ f32 mInheritScale;
	/* 0x2C */ f32 mInheritAlpha;
	/* 0x30 */ f32 mRotateSpeed;
	/* 0x34 */ f32 mInheritRGB;
	/* 0x38 */ GXColor mPrmColor;
	/* 0x3C */ GXColor mEnvColor;
	/* 0x40 */ s16 mLife;
	/* 0x42 */ s16 mRate;
	/* 0x44 */ u8 mType;
	/* 0x45 */ u8 mDirType;
	/* 0x46 */ u8 mRotType;
	/* 0x47 */ u8 mStep;
	/* 0x48 */ u8 mChildrenAffectedByFields;
	/* 0x49 */ u8 unk49;
	/* 0x4A */ u8 mEnableScaleOut;
	/* 0x4B */ u8 mEnableAlphaOut;
	/* 0x4C */ u8 mTextureIndex;
	/* 0x4D */ u8 mEnableRotate;
	/* 0x4E */ u8 unk4E;
};

#endif
