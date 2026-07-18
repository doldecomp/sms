#ifndef NPC_INIT_DATA_HPP
#define NPC_INIT_DATA_HPP

#include <dolphin/gx/GXStruct.h>

struct TColorChangeInfo {
	/* 0x0 */ u32 unk0;               // some enum?
	/* 0x4 */ const char* unk4;       // material name to update?
	/* 0x8 */ const GXColorS10* unk8; // Array of colors?
	/* 0xC */ const GXColorS10* unkC; // Another array of colors?
};

// fabricated
struct TNpcTakeData {
	/* 0x0 */ const char* unk0;
	/* 0x4 */ const char* unk4;
};

// fabricated
struct TNpcModelDataEntry {
	/* 0x0 */ const TColorChangeInfo* unk0;
	/* 0x4 */ u32 unk4;
};

// fabricated
struct TNpcModelData {
	/* 0x0 */ const char* unk0;
	/* 0x4 */ u32 unk4;
	/* 0x8 */ const char* unk8[2];
	/* 0x10 */ TNpcModelDataEntry unk10[3];
	/* 0x28 */ s16 unk28;
	/* 0x2A */ bool unk2A;
	/* 0x2B */ u8 unk2B;
};

struct TNpcInitInfo {
	/* 0x0 */ const TNpcTakeData* unk0;
	/* 0x4 */ const TNpcModelData* unk4[12];
	/* 0x34 */ const TColorChangeInfo* unk34[2][2];
	/* 0x44 */ f32 mAttackRadius;
	/* 0x48 */ f32 mAttackHeight;
	/* 0x4C */ f32 mDamageRadius;
	/* 0x50 */ f32 mDamageHeight;
};

const TNpcInitInfo* SMSGetNpcInitData(u32 idx);

#endif
