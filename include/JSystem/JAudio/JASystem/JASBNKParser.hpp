#ifndef JASBNKPARSER_HPP
#define JASBNKPARSER_HPP

#include <JSystem/JAudio/JASystem/JASBasicBank.hpp>
#include <JSystem/JAudio/JASystem/JASOscillator.hpp>
#include <dolphin/types.h>

namespace JASystem {

namespace BNKParser {

	struct THeader {
		/* 0x000 */ u8 unk0[0x24];
		/* 0x024 */ u32 mInstOffsets[0x80];
		/* 0x224 */ u8 unk224[0x190];
		/* 0x3B4 */ u32 mPercOffsets[12];
	};
	struct TInst {
		/* 0x00 */ u8 unk0[8];
		/* 0x08 */ f32 unk8;
		/* 0x0C */ f32 unkC;
		/* 0x10 */ u32 mOscOffsets[2];
		/* 0x18 */ u32 mRandOffsets[2];
		/* 0x20 */ u32 mSenseOffsets[2];
		/* 0x28 */ u32 mKeyRegionCount;
		/* 0x2C */ u32 mKeymapOffsets[2];
	};
	struct TKeymap {
		/* 0x00 */ u8 unk0;
		/* 0x04 */ u32 unk4;
		/* 0x08 */ u32 mVmapOffsets[1];
	};
	struct TOsc {
		/* 0x00 */ u8 unk0;
		/* 0x04 */ f32 unk4;
		/* 0x08 */ u32 unk8;
		/* 0x0C */ u32 unkC;
		/* 0x10 */ f32 unk10;
		/* 0x14 */ f32 unk14;
	};
	struct TPerc {
		/* 0x000 */ u32 mMagic;
		/* 0x004 */ u8 unk4[0x84];
		/* 0x088 */ u32 mPmapOffsets[0x80];
		/* 0x288 */ s8 unk288[0x80];
		/* 0x308 */ u16 unk308[0x80];
	};
	struct TPmap {
		/* 0x00 */ f32 unk0;
		/* 0x04 */ f32 unk4;
		/* 0x08 */ u32 mRandOffsets[2];
		/* 0x10 */ u32 mVeloRegionCount;
		/* 0x14 */ u32 mVeloRegionOffsets[1];
	};
	struct TRand {
		/* 0x00 */ u8 unk0;
		/* 0x04 */ f32 unk4;
		/* 0x08 */ f32 unk8;
	};
	struct TSense {
		/* 0x00 */ u8 unk0;
		/* 0x01 */ u8 unk1;
		/* 0x02 */ u8 unk2;
		/* 0x04 */ f32 unk4;
		/* 0x08 */ f32 unk8;
	};
	struct TVmap {
		/* 0x00 */ u8 unk0;
		/* 0x04 */ u32 unk4;
		/* 0x08 */ f32 unk8;
		/* 0x0C */ f32 unkC;
	};

	TBasicBank* createBasicBank(void* data);
	TOscillator::Osc_* findOscPtr(TBasicBank* bank, THeader* header, TOsc* osc);
	s16* getOscTableEndPtr(s16* ptr);
	u32 getUsedHeapSize();

	extern u32 sUsedHeapSize;

} // namespace BNKParser

} // namespace JASystem

#endif // JASBNKPARSER_HPP
