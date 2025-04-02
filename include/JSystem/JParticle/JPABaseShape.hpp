#ifndef JPA_BASE_SHAPE_HPP
#define JPA_BASE_SHAPE_HPP

#include <dolphin/gx/GXEnum.h>
#include <JSystem/JParticle/JPADataBlock.hpp>
#include <dolphin/gx/GXStruct.h>

class JPABaseShape : public JPADataBlock {
	static GXBlendMode stBlendMode[];
	static GXBlendFactor stBlendFactor[];
	static GXLogicOp stLogicOp[];
	static GXCompare stCompare[];
	static GXAlphaOp stAlphaOp[];

public:
	JPABaseShape(const u8*, JKRHeap*);
	~JPABaseShape() { }

	// Names from tww
	GXCompare getAlphaCmpComp0()
	{
		return JPABaseShape::stCompare[mAlphaCmpComp0Index];
	}
	GXCompare getAlphaCmpComp1()
	{
		return JPABaseShape::stCompare[mAlphaCmpComp1Index];
	}
	GXAlphaOp getAlphaCmpOp()
	{
		return JPABaseShape::stAlphaOp[mAlphaCmpOpIndex];
	}
	u8 getAlphaCmpRef0() { return mAlphaCmpRef0; }
	u8 getAlphaCmpRef1() { return mAlphaCmpRef1; }

	GXBool isEnableAlphaUpdate() { return unk6C >> 3 & 1; }

	GXBlendMode getBlendMode1()
	{
		return JPABaseShape::stBlendMode[mBlendMode1Index];
	}
	GXBlendFactor getSrcBlendFactor1()
	{
		return JPABaseShape::stBlendFactor[mSrcBlendFactor1Index];
	}
	GXBlendFactor getDstBlendFactor1()
	{
		return JPABaseShape::stBlendFactor[mDstBlendFactor1Index];
	}
	GXLogicOp getBlendOp1() { return JPABaseShape::stLogicOp[mBlendOp1Index]; }

	GXBool isEnableZCmp() { return unk77; }
	GXCompare getZCmpFunction()
	{
		return JPABaseShape::stCompare[mZCmpFunctionIndex];
	}
	GXBool isEnableZCmpUpdate() { return unk79; }
	GXBool getZCompLoc() { return mZCompLoc; }

	u32 getListOrder() { return !(mFlags & 1); }
	u8 getChildOrder() { return mFlags >> 1 & 1; }
	u32 isClipOn() { return mFlags >> 2 & 1; }

	u8 getType() { return mType; }
	u8 getDirType() { return mDirType; }
	u8 getRotType() { return mRotType; }

	f32 getTilingX() { return mTilingX; }
	f32 getTilingY() { return mTilingY; }

	GXBool textureIsEmpty() { return unk81; }
	BOOL isEnableTextureAnm() { return unk80; }

	u8 getTextureIndex() { return mTextureIndex; }
	u8 getTextureIndex(u8 idx) { return mTextureIndices[idx]; }

	f32 getBaseSizeX() { return mBaseSizeX; }
	f32 getBaseSizeY() { return mBaseSizeY; }

	BOOL isEnablePrm() { return unk83 & 1; }
	BOOL isEnablePrmAnm() { return unk83 & 2; }
	BOOL isEnableEnv() { return unk84 & 1; }
	BOOL isEnableEnvAnm() { return unk84 & 2; }

	GXColor getPrmColor() { return mPrmColor; }
	GXColor getEnvColor() { return mEnvColor; }
	GXColor getPrmColor(s16 i) { return mPrmColors[i]; }
	GXColor getEnvColor(s16 i) { return mEnvColors[i]; }

	s16 getLoopOffset() { return mLoopOffset; }

	f32 getTexStaticTransX() { return mTexStaticTransX; }
	f32 getTexStaticTransY() { return mTexStaticTransY; }
	f32 getTexStaticScaleX() { return mTexStaticScaleX; }
	f32 getTexStaticScaleY() { return mTexStaticScaleY; }
	f32 getTexScrollTransX() { return mTexScrollTransX; }
	f32 getTexScrollTransY() { return mTexScrollTransY; }
	f32 getTexScrollScaleX() { return mTexScrollScaleX; }
	f32 getTexScrollScaleY() { return mTexScrollScaleY; }

	f32 getTexScrollRotate() { return mTexScrollRotate; }
	s16 getColorRegAnmMaxFrm() { return mColorRegAnmMaxFrm; }
	u8 getTextureAnmKeyNum() { return mTextureAnmKeyNum; }
	s16 getColLoopOffset() { return mColLoopOffset; }
	s16 getTexLoopOffset() { return mTexLoopOffset; }

public:
	/* 0x8 */ u8* mTextureIndices;
	/* 0xC */ GXColor* mPrmColors;
	/* 0x10 */ GXColor* mEnvColors;
	/* 0x14 */ f32 mBaseSizeX;
	/* 0x18 */ f32 mBaseSizeY;
	/* 0x1C */ f32 mTexStaticTransX;
	/* 0x20 */ f32 mTexStaticScaleX;
	/* 0x24 */ f32 mTexStaticTransY;
	/* 0x28 */ f32 mTexStaticScaleY;
	/* 0x2C */ f32 mTilingX;
	/* 0x30 */ f32 mTilingY;
	/* 0x34 */ f32 mTexScrollTransX;
	/* 0x38 */ f32 mTexScrollScaleX;
	/* 0x3C */ f32 mTexScrollTransY;
	/* 0x40 */ f32 mTexScrollScaleY;
	/* 0x44 */ f32 mTexScrollRotate;

	// fabricated
	/* 0x48 */ struct TevArgs {
		/* 0x0 */ GXTevColorArg unk0;
		/* 0x4 */ GXTevColorArg unk4;
		/* 0x8 */ GXTevColorArg unk8;
		/* 0xC */ GXTevColorArg unkC;
	} unk48;

	/* 0x58 */ s16 mLoopOffset;
	/* 0x5A */ s16 mColLoopOffset;
	/* 0x5C */ s16 mTexLoopOffset;
	/* 0x5E */ s16 mColorRegAnmMaxFrm;
	/* 0x60 */ GXColor mPrmColor;
	/* 0x64 */ GXColor mEnvColor;
	/* 0x68 */ u8 unk68;
	/* 0x69 */ u8 mType;
	/* 0x6A */ u8 mDirType;
	/* 0x6B */ u8 mRotType;
	/* 0x6C */ u8 unk6C;
	/* 0x6D */ u8 mBlendMode1Index;
	/* 0x6E */ u8 mSrcBlendFactor1Index;
	/* 0x6F */ u8 mDstBlendFactor1Index;
	/* 0x70 */ u8 mBlendOp1Index;
	/* 0x71 */ u8 mAlphaCmpComp0Index;
	/* 0x72 */ u8 mAlphaCmpRef0;
	/* 0x73 */ u8 mAlphaCmpOpIndex;
	/* 0x74 */ u8 mAlphaCmpComp1Index;
	/* 0x75 */ u8 mAlphaCmpRef1;
	/* 0x76 */ u8 mZCompLoc;
	/* 0x77 */ u8 unk77;
	/* 0x78 */ u8 mZCmpFunctionIndex;
	/* 0x79 */ u8 unk79;
	/* 0x7A */ u8 unk7A;
	/* 0x7B */ u8 unk7B;
	/* 0x7C */ u8 mFlags;
	/* 0x7D */ u8 unk7D;
	/* 0x7E */ u8 mTextureAnmKeyNum;
	/* 0x7F */ u8 mTextureIndex;
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
