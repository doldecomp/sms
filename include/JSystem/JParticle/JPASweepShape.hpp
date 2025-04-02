#ifndef JPA_SWEEP_SHAPE_HPP
#define JPA_SWEEP_SHAPE_HPP

#include <dolphin/types.h>
#include <JSystem/JParticle/JPADataBlock.hpp>
#include <dolphin/gx/GXStruct.h>

class JPASweepShape : public JPADataBlock {
public:
	JPASweepShape(const u8*);
	virtual ~JPASweepShape() { }

	u8 getType() { return mType; }
	u8 getDirType() { return mDirType; }
	u8 getRotType() { return mRotType; }
	f32 getScaleX() { return mScaleX; }
	f32 getScaleY() { return mScaleY; }
	u8 getTextureIndex() { return mTextureIndex; }
	f32 getRotateSpeed() { return mRotateSpeed; }

	BOOL isClipOn() { return unk49 & 2; }
	BOOL isEnableDrawParent() { return unk49 & 1; }
	BOOL isEnableRotate() { return unk4D; }

	BOOL isInheritedScale() { return unk4E & 1; }
	f32 getInheritScale() { return unk28; }

	BOOL isInheritedRGB() { return unk4E & 4; }
	f32 getInheritRGB() { return unk34; }

	BOOL isInheritedAlpha() { return unk4E & 2; }
	f32 getInheritAlpha() { return unk2C; }

	GXColor getPrm() { return mPrmColor; }
	GXColor getEnv() { return mEnvColor; }

	u8 getPrmAlpha() { return mPrmColor.a; }
	u8 getEnvAlpha() { return mEnvColor.a; }

	f32 getTiming() { return mTiming; }
	s32 getStep() { return mStep; }

public:
	/* 0x8 */ f32 unk8;
	/* 0xC */ f32 unkC;
	/* 0x10 */ f32 mScaleY;
	/* 0x14 */ f32 mScaleX;
	/* 0x18 */ f32 mTiming;
	/* 0x1C */ f32 unk1C;
	/* 0x20 */ f32 unk20;
	/* 0x24 */ f32 unk24;
	/* 0x28 */ f32 unk28;
	/* 0x2C */ f32 unk2C;
	/* 0x30 */ f32 mRotateSpeed;
	/* 0x34 */ f32 unk34;
	/* 0x38 */ GXColor mPrmColor;
	/* 0x3C */ GXColor mEnvColor;
	/* 0x40 */ s16 unk40;
	/* 0x42 */ s16 unk42;
	/* 0x44 */ u8 mType;
	/* 0x45 */ u8 mDirType;
	/* 0x46 */ u8 mRotType;
	/* 0x47 */ u8 mStep;
	/* 0x48 */ u8 unk48;
	/* 0x49 */ u8 unk49;
	/* 0x4A */ u8 unk4A;
	/* 0x4B */ u8 unk4B;
	/* 0x4C */ u8 mTextureIndex;
	/* 0x4D */ u8 unk4D;
	/* 0x4E */ u8 unk4E;
};

#endif
