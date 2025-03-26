#ifndef JAIPARAMETERS_HPP
#define JAIPARAMETERS_HPP

#include <types.h>

struct JAIMoveParaSet {
	/* 0x0 */ f32 unk0;
	/* 0x4 */ f32 unk4;
	/* 0x8 */ f32 unk8;
	/* 0xC */ u32 unkC;
};

class JAISeParameter {
public:
	/* 0x0 */ char unk0[0x20 - 0x0];
	/* 0x20 */ u16 unk20;
	/* 0x22 */ char unk22[0x124 - 0x22];
	/* 0x124 */ JAIMoveParaSet unk124[8];
	/* 0x1A4 */ JAIMoveParaSet unk1A4[8];
	/* 0x224 */ JAIMoveParaSet unk224[8];
	/* 0x2A4 */ JAIMoveParaSet unk2A4[8];
	/* 0x324 */ JAIMoveParaSet unk324[8];
	/* 0x3A4 */ JAIMoveParaSet unk3A4[8];
	/* 0x424 */ u32 unk424;
	/* 0x428 */ u32 unk428;
	/* 0x42C */ u32 unk42C;
	/* 0x430 */ u32 unk430;
	/* 0x434 */ u32 unk434;
	/* 0x438 */ u32 unk438;
	/* 0x43C */ JAISeParameter* unk43C;
	/* 0x440 */ JAISeParameter* unk440;
};

class MuteBit {
public:
	u8 flag1 : 1;
	u8 flag2 : 1;
	u8 flag3 : 1;
	u8 flag4 : 1;
	u8 flag5 : 1;
	u8 flag6 : 1;
	u8 flag7 : 1;
	u8 flag8 : 1;
};

class JAISeqUpdateData;

class JAISeqParameter {
public:
	/* 0x0 */ u32 unk0;
	/* 0x4 */ JAIMoveParaSet unk4;
	/* 0x14 */ JAIMoveParaSet unk14[16];

	/* 0x114 */ JAIMoveParaSet unk114[20];
	/* 0x254 */ JAIMoveParaSet unk254[20];
	/* 0x394 */ JAIMoveParaSet unk394[20];
	/* 0x4D4 */ JAIMoveParaSet unk4D4[20];
	/* 0x614 */ JAIMoveParaSet unk614[20];

	/* 0x754 */ JAIMoveParaSet unk754[32];
	/* 0x954 */ JAIMoveParaSet unk954[32];
	/* 0xB54 */ JAIMoveParaSet unkB54[32];
	/* 0xD54 */ JAIMoveParaSet unkD54[32];
	/* 0xF54 */ JAIMoveParaSet unkF54[32];
	/* 0x1154 */ JAIMoveParaSet unk1154[32];

	/* 0x1354 */ u16 unk1354[32][16];

	/* 0x1754 */ u8 unk1754;
	/* 0x1755 */ u8 unk1755;
	/* 0x1756 */ u16 unk1756;
	/* 0x1758 */ u32 unk1758;
	/* 0x175C */ u32 unk175C;
	/* 0x1760 */ u32 unk1760;
	/* 0x1764 */ u32 unk1764;
	/* 0x1768 */ u32 unk1768;
	/* 0x176C */ u32 unk176C;
	/* 0x1770 */ u32 unk1770;
	/* 0x1774 */ u32 unk1774;
	/* 0x1778 */ u32 unk1778;
	/* 0x177C */ u32 unk177C;
	/* 0x1780 */ u32 unk1780;
	/* 0x1784 */ u32 unk1784;
	/* 0x1788 */ u32 unk1788;
	/* 0x178C */ u32 unk178C;

	/* 0x1790 */ u32 unk1790[32];
	/* 0x1810 */ u8 unk1810[32];
	/* 0x1830 */ MuteBit unk1830[32];
	/* 0x1850 */ JAISeqUpdateData* unk1850;
	/* 0x1854 */ JAISeqParameter* unk1854;
	/* 0x1858 */ JAISeqParameter* unk1858;
};

class JAIStreamUpdateParameter;

class JAIStreamParameter {
public:
	/* 0x0 */ u8 unk0;
	/* 0x4 */ u32 unk4;
	/* 0x8 */ u32 unk8;
	/* 0xC */ u32 unkC;
	/* 0x10 */ char unk10[0x4];
	/* 0x14 */ JAIMoveParaSet unk14[20];
	/* 0x154 */ JAIMoveParaSet unk154[20];
	/* 0x294 */ JAIMoveParaSet unk294[20];
	/* 0x3D4 */ JAIStreamUpdateParameter* unk3D4;
	/* 0x3D8 */ JAIStreamParameter* unk3D8;
	/* 0x3DC */ JAIStreamParameter* unk3DC;
};

class JAISound;

// fabricated
struct JAIStreamUpdateParameter {
	/* 0x0 */ u8 unk0;
	/* 0x1 */ u8 unk1;
	/* 0x2 */ u8 unk2;
	/* 0x4 */ f32 unk4;
	/* 0x8 */ f32 unk8;
	/* 0xC */ f32 unkC;
	/* 0x10 */ u32 unk10;
	/* 0x14 */ JAISound* unk14;
};

#endif
