#ifndef JPA_EXTRA_SHAPE_HPP
#define JPA_EXTRA_SHAPE_HPP

#include <JSystem/JParticle/JPADataBlock.hpp>
#include <dolphin/gx/GXStruct.h>

class JPAExtraShape : public JPADataBlock {
public:
	JPAExtraShape(const u8*);
	~JPAExtraShape() { }

	BOOL isEnableScale() { return unk7E & 1; }
	BOOL isEnableAlpha() { return unk78 & 1; }
	BOOL isEnableRotate() { return unk7F; }

	f32 getRotateAngle() { return mRotateAngle; }
	f32 getRotateRandomAngle() { return mRotateRandomAngle; }
	f32 getRotateDirection() { return mRotateDirection; }
	f32 getRotateSpeed() { return mRotateSpeed; }
	f32 getRotateRandomSpeed() { return mRotateRandomSpeed; }

	f32 getRandomScale() { return mRandomScale; }
	f32 getScaleInTiming() { return mScaleInTiming; }

	f32 getIncreaseRateX() { return mIncreaseRateX; }
	f32 getIncreaseRateY() { return mIncreaseRateY; }

	f32 getScaleInValueX() { return mScaleInValueX; }
	f32 getScaleInValueY() { return mScaleInValueY; }

	f32 getDecreaseRateX() { return mDecreaseRateX; }
	f32 getDecreaseRateY() { return mDecreaseRateY; }

	f32 getScaleOutTiming() { return mScaleOutTiming; }

	s16 getAnmCycleX() { return mAnmCycleX; }
	s16 getAnmCycleY() { return mAnmCycleY; }

	f32 getAlphaWaveRandom() { return mAlphaWaveRandom; }
	f32 getAlphaInTiming() { return mAlphaInTiming; }
	f32 getAlphaIncreaseRate() { return mAlphaIncreaseRate; }
	f32 getAlphaInValue() { return mAlphaInValue; }
	f32 getAlphaOutTiming() { return mAlphaOutTiming; }
	f32 getAlphaBaseValue() { return mAlphaBaseValue; }
	f32 getAlphaDecreaseRate() { return mAlphaDecreaseRate; }

	f32 getAlphaWaveParam1() { return mAlphaWaveParam1; }
	f32 getAlphaWaveParam2() { return mAlphaWaveParam2; }
	f32 getAlphaWaveParam3() { return mAlphaWaveParam3; }

public:
	/* 0x8 */ f32 mAlphaInTiming;
	/* 0xC */ f32 mAlphaOutTiming;
	/* 0x10 */ f32 mAlphaInValue;
	/* 0x14 */ f32 mAlphaBaseValue;
	/* 0x18 */ f32 unk18;
	/* 0x1C */ f32 mAlphaWaveParam1;
	/* 0x20 */ f32 mAlphaWaveParam2;
	/* 0x24 */ f32 mAlphaWaveParam3;
	/* 0x28 */ f32 mAlphaWaveRandom;
	/* 0x2C */ f32 mAlphaIncreaseRate;
	/* 0x30 */ f32 mAlphaDecreaseRate;
	/* 0x34 */ f32 mRandomScale;
	/* 0x38 */ f32 mScaleInTiming;
	/* 0x3C */ f32 mScaleOutTiming;
	/* 0x40 */ f32 mScaleInValueY;
	/* 0x44 */ f32 unk44;
	/* 0x48 */ f32 mScaleInValueX;
	/* 0x4C */ f32 unk4C;
	/* 0x50 */ f32 mIncreaseRateX;
	/* 0x54 */ f32 mDecreaseRateX;
	/* 0x58 */ f32 mIncreaseRateY;
	/* 0x5C */ f32 mDecreaseRateY;
	/* 0x60 */ f32 mRotateAngle;
	/* 0x64 */ f32 mRotateSpeed;
	/* 0x68 */ f32 mRotateRandomSpeed;
	/* 0x6C */ f32 mRotateRandomAngle;
	/* 0x70 */ f32 mRotateDirection;
	/* 0x74 */ s16 mAnmCycleY;
	/* 0x76 */ s16 mAnmCycleX;
	/* 0x78 */ u8 unk78;
	/* 0x79 */ u8 unk79;
	/* 0x7A */ u8 unk7A;
	/* 0x7B */ u8 unk7B;
	/* 0x7C */ u8 unk7C;
	/* 0x7D */ u8 unk7D;
	/* 0x7E */ u8 unk7E;
	/* 0x7F */ u8 unk7F;
};

#endif
