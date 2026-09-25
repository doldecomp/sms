#include <JSystem/JParticle/JPAExtraShape.hpp>
#include <JSystem/JParticle/JPAMath.hpp>

JPAExtraShape::JPAExtraShape(const u8* data)
{
	mAlphaInTiming  = JPAConvertFixToFloat(*(s16*)(data + 0x14));
	mAlphaOutTiming = JPAConvertFixToFloat(*(s16*)(data + 0x16));
	mAlphaInValue   = JPAConvertFixToFloat(*(s16*)(data + 0x18));
	mAlphaBaseValue = JPAConvertFixToFloat(*(s16*)(data + 0x1A));
	unk18           = JPAConvertFixToFloat(*(s16*)(data + 0x1C));

	unk78 = data[0x1E];
	unk79 = data[0x1F];

	mAlphaWaveParam1 = JPAConvertFixToFloat(*(s16*)(data + 0x20));
	mAlphaWaveParam2 = JPAConvertFixToFloat(*(s16*)(data + 0x22));
	mAlphaWaveParam3 = JPAConvertFixToFloat(*(s16*)(data + 0x24));
	mAlphaWaveRandom = JPAConvertFixToFloat(*(s16*)(data + 0x26));

	mAlphaIncreaseRate
	    = mAlphaInTiming != 0.0f
	          ? (mAlphaBaseValue - mAlphaInValue) / mAlphaInTiming
	          : 1.0f;
	mAlphaDecreaseRate
	    = mAlphaOutTiming != 1.0f
	          ? (unk18 - mAlphaBaseValue) / (1.0f - mAlphaOutTiming)
	          : 1.0f;

	mRandomScale    = JPAConvertFixToFloat(*(s16*)(data + 0x34));
	mScaleInTiming  = JPAConvertFixToFloat(*(s16*)(data + 0x36));
	mScaleOutTiming = JPAConvertFixToFloat(*(s16*)(data + 0x38));
	mScaleInValueY  = JPAConvertFixToFloat(*(s16*)(data + 0x3A)) * 10.0f;
	unk44           = JPAConvertFixToFloat(*(s16*)(data + 0x3E)) * 10.0f;

	unk7A          = data[0x40];
	unk7B          = data[0x41];
	mAnmCycleY     = *(s16*)(data + 0x42);
	mScaleInValueX = JPAConvertFixToFloat(*(s16*)(data + 0x44)) * 10.0f;
	unk4C          = JPAConvertFixToFloat(*(s16*)(data + 0x48)) * 10.0f;
	unk7C          = data[0x4A];
	unk7D          = data[0x4B];
	mAnmCycleX     = *(s16*)(data + 0x4C);
	unk7E          = data[0x4E];

	if (mScaleInTiming != 0.0f) {
		mIncreaseRateX = (1.0f - mScaleInValueX) / mScaleInTiming;
		mIncreaseRateY = (1.0f - mScaleInValueY) / mScaleInTiming;
	} else {
		mIncreaseRateY = 1.0f;
		mIncreaseRateX = 1.0f;
	}

	if (mScaleOutTiming != 1.0f) {
		mDecreaseRateX = (unk4C - 1.0f) / (1.0f - mScaleOutTiming);
		mDecreaseRateY = (unk44 - 1.0f) / (1.0f - mScaleOutTiming);
	} else {
		mDecreaseRateY = 1.0f;
		mDecreaseRateX = 1.0f;
	}

	mRotateAngle       = JPAConvertFixToFloat(*(s16*)(data + 0x5A));
	mRotateSpeed       = JPAConvertFixToFloat(*(s16*)(data + 0x5C));
	mRotateRandomSpeed = JPAConvertFixToFloat(*(s16*)(data + 0x5E));
	mRotateRandomAngle = JPAConvertFixToFloat(*(s16*)(data + 0x60));
	mRotateDirection   = JPAConvertFixToFloat(*(s16*)(data + 0x62));

	unk7F = data[0x64];
}
