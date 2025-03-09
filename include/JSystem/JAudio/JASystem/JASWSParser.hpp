#ifndef JASWSPARSER_HPP
#define JASWSPARSER_HPP

#include <dolphin/types.h>

namespace JASystem {

class TWaveBank;

namespace WSParser {

	struct TCtrl {
		/* 0x00 */ u32 mMagic;
		/* 0x04 */ u32 mWaveCount;
		/* 0x08 */ u32 mCtrlWaveOffsets[1];
	};
	struct TCtrlGroup {
		/* 0x00 */ u32 mMagic;
		/* 0x04 */ u8 unk4[0x08 - 0x04];
		/* 0x08 */ u32 mCtrlGroupCount;
		/* 0x0C */ u32 mCtrlSceneOffsets[1];
	};
	struct TCtrlScene {
		/* 0x00 */ u32 mMagic;
		/* 0x04 */ u8 unk4[0x0C - 0x04];
		/* 0x0C */ u32 mCtrlOffset;
	};
	struct TCtrlWave {
		/* 0x00 */ u32 unk0;
	};
	struct THeader {
		/* 0x00 */ u8 unk0[0x10];
		/* 0x10 */ u32 mArchiveBankOffset;
		/* 0x14 */ u32 mCtrlGroupOffset;
	};
	struct TWave {
		/* 0x00 */ u8 unk0;
		/* 0x01 */ u8 unk1;
		/* 0x02 */ u8 unk2;
		/* 0x04 */ f32 unk4;
		/* 0x08 */ u32 mOffset;
		/* 0x0C */ u32 unkC;
		/* 0x10 */ u32 unk10;
		/* 0x14 */ u32 unk14;
		/* 0x18 */ u32 unk18;
		/* 0x1C */ u32 unk1c;
		/* 0x20 */ s16 unk20;
		/* 0x22 */ s16 unk22;
		/* 0x24 */ u8 unk24[0x28 - 0x24];
		/* 0x28 */ int unk28;
	};
	struct TWaveArchive {
		/* 0x00 */ char mFileName[0x74];
		/* 0x74 */ u32 mWaveOffsets[1];
	};
	struct TWaveArchiveBank {
		/* 0x00 */ u32 mMagic;
		/* 0x04 */ u8 unk4[0x08 - 0x04];
		/* 0x08 */ u32 mArchiveOffsets[1];
	};

	u32 getGroupCount(void* data);
	TWaveBank* createBasicWaveBank(void* data);
	TWaveBank* createSimpleWaveBank(void* data);
	u32 getUsedHeapSize();

	extern u32 sUsedHeapSize;
}; // namespace WSParser

} // namespace JASystem

template <typename T> T* JSUConvertOffsetToPtr(const void* base, u32 offset);

#endif // JASWSPARSER_HPP
