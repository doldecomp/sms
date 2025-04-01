#ifndef JPA_BASE_SHAPE_HPP
#define JPA_BASE_SHAPE_HPP

#include <dolphin/gx/GXEnum.h>
#include <JSystem/JParticle/JPADataBlock.hpp>
#include <dolphin/gx/GXStruct.h>

class JPABaseShape : public JPADataBlock {
public:
	JPABaseShape(const u8*, JKRHeap*);
	~JPABaseShape() { }

	static GXBlendMode stBlendMode[];
	static GXBlendFactor stBlendFactor[];
	static GXLogicOp stLogicOp[];
	static GXCompare stCompare[];
	static GXAlphaOp stAlphaOp[];

	// Names from tww
	GXCompare getAlphaCmpComp0() { return JPABaseShape::stCompare[unk71]; }
	GXCompare getAlphaCmpComp1() { return JPABaseShape::stCompare[unk74]; }
	GXAlphaOp getAlphaCmpOp() { return JPABaseShape::stAlphaOp[unk73]; }
	u8 getAlphaCmpRef0() { return unk72; }
	u8 getAlphaCmpRef1() { return unk75; }

	GXBool isEnableAlphaUpdate() { return unk6C >> 3 & 1; }

	GXBlendMode getBlendMode1() { return JPABaseShape::stBlendMode[unk6D]; }
	GXBlendFactor getSrcBlendFactor1()
	{
		return JPABaseShape::stBlendFactor[unk6E];
	}
	GXBlendFactor getDstBlendFactor1()
	{
		return JPABaseShape::stBlendFactor[unk6F];
	}
	GXLogicOp getBlendOp1() { return JPABaseShape::stLogicOp[unk70]; }

	GXBool isEnableZCmp() { return unk77; }
	GXCompare getZCmpFunction() { return JPABaseShape::stCompare[unk78]; }
	GXBool isEnableZCmpUpdate() { return unk79; }
	GXBool getZCompLoc() { return unk76; }

	u32 isClipOn() { return unk7C >> 2 & 1; }
	u32 getListOrder() { return !(unk7C & 1); }
	u8 getChildOrder() { return unk7C >> 1 & 1; }

	u8 getType() { return unk69; }
	u8 getDirType() { return unk6A; }
	u8 getRotType() { return unk6B; }

	f32 getTilingX() { return unk2C; }
	f32 getTilingY() { return unk30; }

	GXBool textureIsEmpty() { return unk81; }
	BOOL isEnableTextureAnm() { return unk80; }
	u8 getTextureIndex() { return unk7F; }
	f32 getBaseSizeX() { return unk14; }
	f32 getBaseSizeY() { return unk18; }

	BOOL isEnablePrm() { return unk83 & 1; }
	BOOL isEnablePrmAnm() { return unk83 & 2; }
	BOOL isEnableEnv() { return unk84 & 1; }
	BOOL isEnableEnvAnm() { return unk84 & 2; }
	GXColor getPrmColor() { return unk60; }
	GXColor getEnvColor() { return unk64; }
	GXColor getPrmColor(s16 i) { return unkC[i]; }
	GXColor getEnvColor(s16 i) { return unk10[i]; }

	s16 getLoopOffset() { return unk58; }

	f32 getTexStaticTransX() { return unk1C; }
	f32 getTexStaticTransY() { return unk24; }
	f32 getTexStaticScaleX() { return unk20; }
	f32 getTexStaticScaleY() { return unk28; }
	f32 getTexScrollTransX() { return unk34; }
	f32 getTexScrollTransY() { return unk3C; }
	f32 getTexScrollScaleX() { return unk38; }
	f32 getTexScrollScaleY() { return unk40; }

	f32 getTexScrollRotate() { return unk44; }

public:
	/* 0x8 */ u8* unk8;
	/* 0xC */ GXColor* unkC;
	/* 0x10 */ GXColor* unk10;
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

	// fabricated
	/* 0x48 */ struct TevArgs {
		/* 0x0 */ GXTevColorArg unk0;
		/* 0x4 */ GXTevColorArg unk4;
		/* 0x8 */ GXTevColorArg unk8;
		/* 0xC */ GXTevColorArg unkC;
	} unk48;

	/* 0x58 */ s16 unk58;
	/* 0x5A */ s16 unk5A;
	/* 0x5C */ s16 unk5C;
	/* 0x5E */ s16 unk5E;
	/* 0x60 */ GXColor unk60;
	/* 0x64 */ GXColor unk64;
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
