#ifndef JASINST_HPP
#define JASINST_HPP

#include <JSystem/JAudio/JASystem/JASOscillator.hpp>
#include <types.h>

namespace JASystem {

struct TVeloRegion {
	/* 0x00 */ s32 unk0;
	/* 0x04 */ s32 unk4;
	/* 0x08 */ f32 unk8;
	/* 0x0C */ f32 unkC;
};

struct TInstParam {
	TInstParam()
	{
		mSourceType   = 0;
		mWaveId       = 0;
		mOscData      = nullptr;
		mOscCount     = 0;
		mVolume       = 1.0f;
		mPitch        = 1.0f;
		mEffectVolume = 1.0f;
		mEffectPitch  = 1.0f;
		mPan          = 0.5f;
		mFxmix        = 0.0f;
		mDolby        = 0.0f;
		mEffectPan    = 0.5f;
		mEffectFxmix  = 0.0f;
		mEffectDolby  = 0.0f;
		mFixedPitch   = 0;
		mRelease      = 0;
		unk3C         = 0;
		unk40         = 0;
	}

	/* 0x00 */ u8 mSourceType;
	/* 0x04 */ int mWaveId;
	/* 0x08 */ TOscillator::Osc_** mOscData;
	/* 0x0C */ u32 mOscCount;
	/* 0x10 */ f32 mVolume;
	/* 0x14 */ f32 mPitch;
	/* 0x18 */ f32 mEffectVolume;
	/* 0x1C */ f32 mEffectPitch;
	/* 0x20 */ f32 mPan;
	/* 0x24 */ f32 mFxmix;
	/* 0x28 */ f32 mDolby;
	/* 0x2C */ f32 mEffectPan;
	/* 0x30 */ f32 mEffectFxmix;
	/* 0x34 */ f32 mEffectDolby;
	/* 0x38 */ u8 mFixedPitch;
	/* 0x3A */ u16 mRelease;
	/* 0x3C */ int unk3C;
	/* 0x40 */ int unk40;
};

class TInst {
public:
	virtual ~TInst() { }

	virtual bool getParam(int, int, TInstParam*) const = 0;
	virtual int getType() const                        = 0;
};

} // namespace JASystem

#endif
