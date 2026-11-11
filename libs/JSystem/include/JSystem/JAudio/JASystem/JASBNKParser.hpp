#ifndef JASBNKPARSER_HPP
#define JASBNKPARSER_HPP

#include <JSystem/JAudio/JASystem/JASBasicBank.hpp>
#include <JSystem/JAudio/JASystem/JASOscillator.hpp>
#include <JSystem/JSupport.hpp>
#include <dolphin/types.h>

namespace JASystem {

namespace BNKParser {

	template <typename T> struct TOffset {
		T* ptr(const void* base) const
		{
			return JSUConvertOffsetToPtr<T>(base, mOffset);
		}

		/* 0x00 */ u32 mOffset;
	};

	struct TVmap;
	struct TRand;
	struct TSense;
	struct TKeymap;
	struct TOsc;
	struct TInst;
	struct TPmap;
	struct TPerc;

	struct THeader {
		/* 0x000 */ char unk0[0x24];
		/* 0x024 */ TOffset<TInst> mInstOffsets[128];
		/* 0x224 */ char unk224[0x190];
		/* 0x3B4 */ TOffset<TPerc> mPercOffsets[12];
	};
	struct TInst {
		/* 0x00 */ u8 unk0[8];
		/* 0x08 */ f32 mVolume;
		/* 0x0C */ f32 mPitch;
		/* 0x10 */ TOffset<TOsc> mOscOffsets[2];
		/* 0x18 */ TOffset<TRand> mRandOffsets[2];
		/* 0x20 */ TOffset<TSense> mSenseOffsets[2];
		/* 0x28 */ u32 mKeyRegionCount;
		/* 0x2C */ TOffset<TKeymap> mKeymapOffsets[2];
	};
	struct TKeymap {
		/* 0x00 */ u8 unk0;
		/* 0x04 */ u32 mVeloRegionCount;
		/* 0x08 */ TOffset<TVmap> mVeloRegionOffsets[];
	};
	struct TOsc {
		/* 0x00 */ u8 mTarget;
		/* 0x04 */ f32 mRate;
		/* 0x08 */ TOffset<s16> mAdsTableOffset;
		/* 0x0C */ TOffset<s16> mRelTableOffset;
		/* 0x10 */ f32 mDepth;
		/* 0x14 */ f32 mBias;
	};
	struct TPerc {
		/* 0x000 */ u32 mMagic;
		/* 0x004 */ char unk4[0x84];
		/* 0x088 */ TOffset<TPmap> mPmapOffsets[128];
		/* 0x288 */ s8 mPan[128];
		/* 0x308 */ u16 mRelease[128];
	};
	struct TPmap {
		/* 0x00 */ f32 mVolume;
		/* 0x04 */ f32 mPitch;
		/* 0x08 */ TOffset<TRand> mRandOffsets[2];
		/* 0x10 */ u32 mVeloRegionCount;
		/* 0x14 */ TOffset<TVmap> mVeloRegionOffsets[];
	};
	struct TRand {
		/* 0x00 */ u8 mTarget;
		/* 0x04 */ f32 mBase;
		/* 0x08 */ f32 mWidth;
	};
	struct TSense {
		/* 0x00 */ u8 mTarget;
		/* 0x01 */ u8 mSource;
		/* 0x02 */ u8 mCenter;
		/* 0x04 */ f32 mLowValue;
		/* 0x08 */ f32 mHighValue;
	};
	struct TVmap {
		/* 0x00 */ u8 mHighVelocity;
		/* 0x04 */ u32 mWaveId;
		/* 0x08 */ f32 mVolume;
		/* 0x0C */ f32 mPitch;
	};

	TBasicBank* createBasicBank(void* data);
	u32 getUsedHeapSize();

	extern u32 sUsedHeapSize;

} // namespace BNKParser

} // namespace JASystem

#endif // JASBNKPARSER_HPP
