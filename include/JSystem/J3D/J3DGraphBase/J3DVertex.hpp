#ifndef J3D_VERTEX_HPP
#define J3D_VERTEX_HPP

#include <types.h>
#include <dolphin/gx.h>

class J3DVertexData {
public:
	J3DVertexData();
	~J3DVertexData();

	// From TP debug
	GXVtxAttrFmtList* getVtxAttrFmtList() { return mVtxAttrFmtList; }
	void* getVtxPosArray() const { return mVtxPosArray; }
	void* getVtxNormArray() const { return mVtxNormArray; }
	void* getVtxNBTArray() const { return mVtxNBTArray; }
	GXColor* getVtxColorArray(u8 idx) const { return mVtxColorArray[idx]; }
	void* getVtxTexCoordArray(u8 idx) const { return mVtxTexCoordArray[idx]; }

	/* 0x0 */ u32 unk0;
	/* 0x4 */ u32 unk4;
	/* 0x8 */ u32 unk8;
	/* 0xC */ GXVtxAttrFmtList* mVtxAttrFmtList;
	/* 0x10 */ void* mVtxPosArray;
	/* 0x14 */ void* mVtxNormArray;
	/* 0x18 */ void* mVtxNBTArray;
	/* 0x1C */ GXColor* mVtxColorArray[2];
	/* 0x24 */ void* mVtxTexCoordArray[8];
};

// TODO: is this an enum?
enum J3DDeformAttachFlag {};

class J3DVertexBuffer {
public:
	J3DVertexBuffer(J3DVertexData* vertex_data);

	void frameInit()
	{
		unk2C = unk4;
		unk30 = unkC;
		unk34 = unk14;
	}

public:
	/* 0x00 */ J3DVertexData* mVertexData;
	/* 0x04 */ void* unk4;
	/* 0x08 */ u32 unk8;
	/* 0x0C */ void* unkC;
	/* 0x10 */ u32 unk10;
	/* 0x14 */ void* unk14;
	/* 0x18 */ u32 unk18;
	// TODO: figure out what actually lives here
	/* 0x1C */ void* unk1C[2];
	/* 0x24 */ void* unk24[2];
	/* 0x2C */ void* unk2C;
	/* 0x30 */ void* unk30;
	/* 0x34 */ void* unk34;

	virtual ~J3DVertexBuffer(); // vt at 0x38

	typedef void IDK;
	IDK copyLocalVtxArray(J3DDeformAttachFlag);
	IDK copyVtxColorArray(J3DDeformAttachFlag);
	IDK copyTransformedVtxArray();
};

class J3DDrawMtxData {
public:
	J3DDrawMtxData();
	~J3DDrawMtxData();

	/* 0x0 */ u16 mEntryNum;
	/* 0x2 */ u16 mDrawFullWgtMtxNum;
	/* 0x4 */ u8* mDrawMtxFlag;
	/* 0x8 */ u16* mDrawMtxIndex;
};

#endif
