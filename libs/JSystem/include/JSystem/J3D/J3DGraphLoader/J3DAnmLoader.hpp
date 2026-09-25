#ifndef J3D_ANM_LOADER_HPP
#define J3D_ANM_LOADER_HPP

#include <dolphin/types.h>
#include <JSystem/JUtility/JUTDataHeader.hpp>

class J3DAnmBase;

struct J3DAnmVtxColorFullData {
	/* 0x00 */ JUTDataBlockHeader mHeader;
	/* 0x08 */ u8 field_0x8;
	/* 0x09 */ u8 field_0x9; // padding?
	/* 0x0A */ s16 mFrameMax;
	/* 0x0C */ u16 mAnmTableNum[2];
	/* 0x10 */ u16 mIndexNum[2];
	/* 0x14 */ u32 field_0x14;
	/* 0x18 */ s32 mTableOffsets[2];
	/* 0x20 */ s32 mVtxColorIndexDataOffsets[2];
	/* 0x28 */ s32 mVtxColorIndexPointerOffsets[2];
	/* 0x30 */ s32 mRValuesOffset;
	/* 0x34 */ s32 mGValuesOffset;
	/* 0x38 */ s32 mBValuesOffset;
	/* 0x3C */ s32 mAValuesOffset;
}; // Size = 0x40

struct J3DAnmVisibilityFullData {
	/* 0x00 */ JUTDataBlockHeader mHeader;
	/* 0x08 */ u8 field_0x8;
	/* 0x09 */ u8 field_0x9; // padding?
	/* 0x0A */ s16 mFrameMax;
	/* 0x0C */ u16 field_0xc;
	/* 0x0E */ u16 field_0xe;
	/* 0x10 */ s32 mTableOffset;
	/* 0x14 */ s32 mValuesOffset;
}; // Size = 0x18

struct J3DAnmTransformFullData {
	/* 0x00 */ JUTDataBlockHeader mHeader;
	/* 0x08 */ u8 field_0x8;
	/* 0x09 */ u8 field_0x9;
	/* 0x0A */ s16 mFrameMax;
	/* 0x0C */ u16 field_0xc;
	/* 0x0E */ u8 field_0xe[0x14 - 0xe];
	/* 0x14 */ s32 mTableOffset;
	/* 0x18 */ s32 mScaleValOffset;
	/* 0x1C */ s32 mRotValOffset;
	/* 0x20 */ s32 mTransValOffset;
}; // Size = 0x24

struct J3DAnmColorKeyData {
	/* 0x00 */ JUTDataBlockHeader mHeader;
	/* 0x08 */ u8 field_0x8;
	/* 0x09 */ u8 field_0x9[3];
	/* 0x0C */ s16 mFrameMax;
	/* 0x0E */ u16 mUpdateMaterialNum;
	/* 0x10 */ u16 field_0x10;
	/* 0x12 */ u16 field_0x12;
	/* 0x14 */ u16 field_0x14;
	/* 0x16 */ u16 field_0x16;
	/* 0x18 */ s32 mTableOffset;
	/* 0x1C */ s32 mUpdateMaterialIDOffset;
	/* 0x20 */ s32 mNameTabOffset;
	/* 0x24 */ s32 mRValOffset;
	/* 0x28 */ s32 mGValOffset;
	/* 0x2C */ s32 mBValOffset;
	/* 0x30 */ s32 mAValOffset;
}; // Size = 0x34

struct J3DAnmTextureSRTKeyData {
	/* 0x00 */ JUTDataBlockHeader mHeader;
	/* 0x08 */ u8 field_0x8;
	/* 0x09 */ u8 field_0x9;
	/* 0x0A */ s16 field_0xa;
	/* 0x0C */ u16 field_0xc;
	/* 0x0E */ u16 field_0xe;
	/* 0x10 */ u16 field_0x10;
	/* 0x12 */ u16 field_0x12;
	/* 0x14 */ s32 mTableOffset;
	/* 0x18 */ s32 mUpdateMatIDOffset;
	/* 0x1C */ s32 mNameTab1Offset;
	/* 0x20 */ s32 mUpdateTexMtxIDOffset;
	/* 0x24 */ s32 unkOffset;
	/* 0x28 */ s32 mScaleValOffset;
	/* 0x2C */ s32 mRotValOffset;
	/* 0x30 */ s32 mTransValOffset;
	/* 0x34 */ u16 field_0x34;
	/* 0x36 */ u16 field_0x36;
	/* 0x38 */ u16 field_0x38;
	/* 0x3A */ u16 field_0x3a;
	/* 0x3C */ s32 mInfoTable2Offset;
	/* 0x40 */ s32 field_0x40;
	/* 0x44 */ u32 mNameTab2Offset;
	/* 0x48 */ s32 field_0x48;
	/* 0x4C */ s32 field_0x4c;
	/* 0x50 */ s32 field_0x50;
	/* 0x54 */ s32 field_0x54;
	/* 0x58 */ s32 field_0x58;
	/* 0x5C */ s32 field_0x5c;
}; // Size = 0x60

struct J3DAnmVtxColorKeyData {
	/* 0x00 */ JUTDataBlockHeader mHeader;
	/* 0x08 */ u8 field_0x8;
	/* 0x09 */ u8 field_0x9;
	/* 0x0A */ s16 mFrameMax;
	/* 0x0C */ u16 mAnmTableNum[2];
	/* 0x10 */ u32 mIndexNum[2];
	/* 0x18 */ s32 mTableOffsets[2];
	/* 0x20 */ s32 mVtxColorIndexDataOffsets[2];
	/* 0x28 */ s32 mVtxColorIndexPointerOffsets[2];
	/* 0x30 */ s32 mRValuesOffset;
	/* 0x34 */ s32 mGValuesOffset;
	/* 0x38 */ s32 mBValuesOffset;
	/* 0x3C */ s32 mAValuesOffset;
}; // Size = 0x40

struct J3DAnmTexPatternFullData {
	/* 0x00 */ JUTDataBlockHeader mHeader;
	/* 0x08 */ u8 field_0x8;
	/* 0x09 */ u8 field_0x9;
	/* 0x0A */ s16 mFrameMax;
	/* 0x0C */ u16 field_0xc;
	/* 0x0E */ u16 field_0xe;
	/* 0x10 */ s32 mTableOffset;
	/* 0x14 */ s32 mValuesOffset;
	/* 0x18 */ s32 mUpdateMaterialIDOffset;
	/* 0x1C */ s32 mNameTabOffset;
}; // Size = 0x20

struct J3DAnmTevRegKeyData {
	/* 0x00 */ JUTDataBlockHeader mHeader;
	/* 0x08 */ u8 field_0x8;
	/* 0x09 */ u8 field_0x9; // maybe padding
	/* 0x0A */ s16 mFrameMax;
	/* 0x0C */ u16 mCRegUpdateMaterialNum;
	/* 0x0E */ u16 mKRegUpdateMaterialNum;
	/* 0x10 */ u16 field_0x10;
	/* 0x12 */ u16 field_0x12;
	/* 0x14 */ u16 field_0x14;
	/* 0x16 */ u16 field_0x16;
	/* 0x18 */ u16 field_0x18;
	/* 0x1A */ u16 field_0x1a;
	/* 0x1C */ u16 field_0x1c;
	/* 0x1E */ u16 field_0x1e;
	/* 0x20 */ s32 mCRegTableOffset;
	/* 0x24 */ s32 mKRegTableOffset;
	/* 0x28 */ s32 mCRegUpdateMaterialIDOffset;
	/* 0x2C */ s32 mKRegUpdateMaterialIDOffset;
	/* 0x30 */ s32 mCRegNameTabOffset;
	/* 0x34 */ s32 mKRegNameTabOffset;
	/* 0x38 */ s32 mCRValuesOffset;
	/* 0x3C */ s32 mCGValuesOffset;
	/* 0x40 */ s32 mCBValuesOffset;
	/* 0x44 */ s32 mCAValuesOffset;
	/* 0x48 */ s32 mKRValuesOffset;
	/* 0x4C */ s32 mKGValuesOffset;
	/* 0x50 */ s32 mKBValuesOffset;
	/* 0x54 */ s32 mKAValuesOffset;
}; // Size = 0x58

struct J3DAnmColorFullData { /* PlaceHolder Structure */
	/* 0x00 */ JUTDataBlockHeader mHeader;
	/* 0x08 */ u8 field_0x8;
	/* 0x09 */ u8 field_0x9[3];
	/* 0x0C */ s16 mFrameMax;
	/* 0x0E */ u16 mUpdateMaterialNum;
	/* 0x10 */ u8 field_0x10[0x18 - 0x10];
	/* 0x18 */ s32 mTableOffset;
	/* 0x1C */ s32 mUpdateMaterialIDOffset;
	/* 0x20 */ s32 mNameTabOffset;
	/* 0x24 */ s32 mRValuesOffset;
	/* 0x28 */ s32 mGValuesOffset;
	/* 0x2C */ s32 mBValuesOffset;
	/* 0x30 */ s32 mAValuesOffset;
}; // Size = 0x34

struct J3DAnmClusterFullData {
	/* 0x00 */ JUTDataBlockHeader mHeader;
	/* 0x08 */ u8 field_0x8;
	/* 0x0A */ s16 mFrameMax;
	/* 0x0C */ s32 field_0xc;
	/* 0x10 */ s32 mTableOffset;
	/* 0x14 */ s32 mWeightOffset;
};

struct J3DAnmTransformKeyData {
	/* 0x00 */ JUTDataBlockHeader mHeader;
	/* 0x08 */ u8 mAttribute;
	/* 0x09 */ u8 mDecShift;
	/* 0x0A */ s16 mFrameMax;
	/* 0x0C */ u16 field_0xc;
	/* 0x10 */ int field_0x10;
	/* 0x14 */ s32 mTableOffset;
	/* 0x18 */ s32 mScaleOffset;
	/* 0x1c */ s32 mRotOffset;
	/* 0x20 */ s32 mTransOffset;
};

struct J3DAnmClusterKeyData {
	/* 0x00 */ JUTDataBlockHeader mHeader;
	/* 0x08 */ u8 field_0x8;
	/* 0x0A */ s16 mFrameMax;
	/* 0x0C */ s32 field_0xc;
	/* 0x10 */ s32 mTableOffset;
	/* 0x14 */ s32 mWeightOffset;
};

class J3DAnmLoaderDataBase {
public:
	static J3DAnmBase* load(const void*);
};

class J3DAnmLoader {
public:
	virtual J3DAnmBase* load(const void*) = 0;
	virtual ~J3DAnmLoader() { }
};

class J3DAnmFullLoader_v15 : public J3DAnmLoader {
public:
	J3DAnmFullLoader_v15();
	virtual ~J3DAnmFullLoader_v15();
	virtual J3DAnmBase* load(const void*);
	void readAnmTransform(const J3DAnmTransformFullData*);
	void readAnmColor(const J3DAnmColorFullData*);
	void readAnmTexPattern(const J3DAnmTexPatternFullData*);
	void readAnmVisibility(const J3DAnmVisibilityFullData*);
	void readAnmCluster(const J3DAnmClusterFullData*);
	void readAnmVtxColor(const J3DAnmVtxColorFullData*);

	J3DAnmBase* mAnm;
};

class J3DAnmKeyLoader_v15 : public J3DAnmLoader {
public:
	J3DAnmKeyLoader_v15();
	virtual ~J3DAnmKeyLoader_v15();
	virtual J3DAnmBase* load(const void*);
	void readAnmTransform(const J3DAnmTransformKeyData*);
	void readAnmTextureSRT(const J3DAnmTextureSRTKeyData*);
	void readAnmColor(const J3DAnmColorKeyData*);
	void readAnmCluster(const J3DAnmClusterKeyData*);
	void readAnmTevReg(const J3DAnmTevRegKeyData*);
	void readAnmVtxColor(const J3DAnmVtxColorKeyData*);

	J3DAnmBase* mAnm;
};

#endif
