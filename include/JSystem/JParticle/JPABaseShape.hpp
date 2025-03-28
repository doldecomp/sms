#ifndef JPA_BASE_SHAPE_HPP
#define JPA_BASE_SHAPE_HPP

#include <dolphin/gx/GXEnum.h>
#include <JSystem/JParticle/JPADataBlock.hpp>

class JPABaseShape : public JPADataBlock {
public:
	JPABaseShape(const u8*, JKRHeap*);
	~JPABaseShape() { }

public:
	/* 0x8 */ void* unk8;
	/* 0xC */ void* unkC;
	/* 0x10 */ void* unk10;
	/* 0x14 */ f32 unk14;
	/* 0x18 */ f32 unk18;
	/* 0x1C */ f32 unk1C;
	/* 0x20 */ f32 unk20;
	/* 0x24 */ f32 unk24;
	/* 0x28 */ f32 unk28;
	/* 0x2C */ f32 unk2C;
	/* 0x30 */ f32 unk30;
	/* 0x34 */ f32 unk34;
	/* 0x38 */ f32 unk38;
	/* 0x3C */ f32 unk3C;
	/* 0x40 */ f32 unk40;
	/* 0x44 */ f32 unk44;
	/* 0x48 */ GXTevColorArg unk48;
	/* 0x4C */ GXTevColorArg unk4C;
	/* 0x50 */ GXTevColorArg unk50;
	/* 0x54 */ GXTevColorArg unk54;
	/* 0x58 */ s16 unk58;
	/* 0x5A */ s16 unk5A;
	/* 0x5C */ s16 unk5C;
	/* 0x5E */ s16 unk5E;
	/* 0x60 */ u32 unk60;
	/* 0x64 */ u32 unk64;
	/* 0x68 */ u8 unk68;
	/* 0x69 */ u8 unk69;
	/* 0x6A */ u8 unk6A;
	/* 0x6B */ u8 unk6B;
	/* 0x6C */ u8 unk6C;
	/* 0x6D */ u8 unk6D;
	/* 0x6E */ u8 unk6E;
	/* 0x6F */ u8 unk6F;
	/* 0x70 */ u8 unk70;
	/* 0x71 */ u8 unk71;
	/* 0x72 */ u8 unk72;
	/* 0x73 */ u8 unk73;
	/* 0x74 */ u8 unk74;
	/* 0x75 */ u8 unk75;
	/* 0x76 */ u8 unk76;
	/* 0x77 */ u8 unk77;
	/* 0x78 */ u8 unk78;
	/* 0x79 */ u8 unk79;
	/* 0x7A */ u8 unk7A;
	/* 0x7B */ u8 unk7B;
	/* 0x7C */ u8 unk7C;
	/* 0x7D */ u8 unk7D;
	/* 0x7E */ u8 unk7E;
	/* 0x7F */ u8 unk7F;
	/* 0x80 */ u8 unk80;
	/* 0x80 */ u8 unk81;
	/* 0x82 */ u8 unk82;
	/* 0x83 */ u8 unk83;
	/* 0x84 */ u8 unk84;
	/* 0x85 */ u8 unk85;
	/* 0x86 */ u8 unk86;
	/* 0x87 */ u8 unk87;
};

#endif
