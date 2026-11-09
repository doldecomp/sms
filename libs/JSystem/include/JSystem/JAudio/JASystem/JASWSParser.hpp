#ifndef JASWSPARSER_HPP
#define JASWSPARSER_HPP

#include <JSystem/JSupport.hpp>
#include <dolphin/types.h>

namespace JASystem {

class TWaveBank;

namespace WSParser {

	template <typename T> struct TOffset {
		T* ptr(const void* base) const
		{
			return JSUConvertOffsetToPtr<T>(base, mOffset);
		}

		/* 0x00 */ u32 mOffset;
	};

	struct TCtrlWave {
		/* 0x00 */ u32 unk0;
	};

	struct TWave {
		/* 0x00 */ u8 unk0;
		/// ADPCM_HQ = 0, ADPCM_LQ = 1, PCM8 = 2, PCM16 = 3
		/// 4-7 are unimplemented
		/* 0x01 */ u8 mFormat;
		/* 0x02 */ u8 mKey;
		/* 0x04 */ f32 mSampleRate;
		/* 0x08 */ s32 mOffset;
		/* 0x0C */ s32 mDataSize;
		/* 0x10 */ s32 mLoop;
		/* 0x14 */ s32 mLoopStart;
		/* 0x18 */ s32 mLoopEnd;
		/* 0x1C */ s32 mSampleCount;
		/* 0x20 */ s16 mLoopHistory1;
		/* 0x22 */ s16 mLoopHistory2;
		/* 0x24 */ u32* mLoadFlagPtr;
		/* 0x28 */ s32 unk28;
	};
	struct TWaveArchive {
		/* 0x00 */ char mFileName[0x74];
		/* 0x74 */ TOffset<TWave> mWaveOffsets[];
	};
	struct TWaveArchiveBank {
		/* 0x00 */ u32 mMagic;
		/* 0x04 */ u8 unk4[0x08 - 0x04];
		/* 0x08 */ TOffset<TWaveArchive> mArchiveOffsets[];
	};
	struct TCtrl {
		/* 0x00 */ u32 mMagic;
		/* 0x04 */ u32 mWaveCount;
		/* 0x08 */ TOffset<TCtrlWave> mCtrlWaveOffsets[];
	};
	struct TCtrlScene {
		/* 0x00 */ u32 mMagic;
		/* 0x04 */ u8 unk4[0x0C - 0x04];
		/* 0x0C */ TOffset<TCtrl> mCtrlOffset;
	};
	struct TCtrlGroup {
		/* 0x00 */ u32 mMagic;
		/* 0x04 */ u8 unk4[0x08 - 0x04];
		/* 0x08 */ u32 mCtrlGroupCount;
		/* 0x0C */ TOffset<TCtrlScene> mCtrlSceneOffsets[1];
	};
	struct THeader {
		/* 0x00 */ u8 unk0[0x10];
		/* 0x10 */ TOffset<TWaveArchiveBank> mArchiveBankOffset;
		/* 0x14 */ TOffset<TCtrlGroup> mCtrlGroupOffset;
	};

	u32 getGroupCount(void* data);
	TWaveBank* createBasicWaveBank(void* data);
	TWaveBank* createSimpleWaveBank(void* data);
	u32 getUsedHeapSize();

	extern u32 sUsedHeapSize;
}; // namespace WSParser

} // namespace JASystem

#endif // JASWSPARSER_HPP
