#include <JSystem/JParticle/JPASweepShape.hpp>
#include <JSystem/JParticle/JPAMath.hpp>

JPASweepShape::JPASweepShape(const u8* data)
{
	mType    = data[0x10];
	mDirType = data[0x11];
	mRotType = data[0x12];

	mLife = *(s16*)(data + 0x14);
	mRate = *(s16*)(data + 0x16);

	mStep = data[0x1A];

	unk8     = *(f32*)(data + 0x28);
	mBaseVel = *(f32*)(data + 0x2C);

	mChildrenAffectedByFields = data[0x36];
	unk49                     = data[0x44];
	mEnableScaleOut           = data[0x45];
	mEnableAlphaOut           = data[0x46];
	mTextureIndex             = data[0x47];

	mScaleY = *(f32*)(data + 0x4C);
	mScaleX = *(f32*)(data + 0x50);

	mEnableRotate = data[0x56];
	unk4E         = data[0x57];

	mPrmColor = *(GXColor*)(data + 0x58);
	mEnvColor = *(GXColor*)(data + 0x5C);

	mTiming       = JPAConvertFixToFloat(*(s16*)(data + 0x18));
	mVelInhRate   = JPAConvertFixToFloat(*(s16*)(data + 0x30));
	mBaseVelRndm  = JPAConvertFixToFloat(*(s16*)(data + 0x32));
	unk24         = JPAConvertFixToFloat(*(s16*)(data + 0x34));
	mInheritScale = JPAConvertFixToFloat(*(s16*)(data + 0x48));
	mInheritAlpha = JPAConvertFixToFloat(*(s16*)(data + 0x4A));
	mInheritRGB   = JPAConvertFixToFloat(*(s16*)(data + 0x60));
	mRotateSpeed  = JPAConvertFixToFloat(*(s16*)(data + 0x54));
}
