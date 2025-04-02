#include <JSystem/JParticle/JPASweepShape.hpp>
#include <JSystem/JParticle/JPAMath.hpp>

JPASweepShape::JPASweepShape(const u8* data)
{
	mType    = data[0x10];
	mDirType = data[0x11];
	mRotType = data[0x12];

	unk40 = *(s16*)(data + 0x14);
	unk42 = *(s16*)(data + 0x16);

	mStep = data[0x1A];

	unk8 = *(f32*)(data + 0x28);
	unkC = *(f32*)(data + 0x2C);

	unk48         = data[0x36];
	unk49         = data[0x44];
	unk4A         = data[0x45];
	unk4B         = data[0x46];
	mTextureIndex = data[0x47];

	mScaleY = *(f32*)(data + 0x4C);
	mScaleX = *(f32*)(data + 0x50);

	unk4D = data[0x56];
	unk4E = data[0x57];

	mPrmColor = *(GXColor*)(data + 0x58);
	mEnvColor = *(GXColor*)(data + 0x5C);

	mTiming      = JPAConvertFixToFloat(*(s16*)(data + 0x18));
	unk1C        = JPAConvertFixToFloat(*(s16*)(data + 0x30));
	unk20        = JPAConvertFixToFloat(*(s16*)(data + 0x32));
	unk24        = JPAConvertFixToFloat(*(s16*)(data + 0x34));
	unk28        = JPAConvertFixToFloat(*(s16*)(data + 0x48));
	unk2C        = JPAConvertFixToFloat(*(s16*)(data + 0x4A));
	unk34        = JPAConvertFixToFloat(*(s16*)(data + 0x60));
	mRotateSpeed = JPAConvertFixToFloat(*(s16*)(data + 0x54));
}
