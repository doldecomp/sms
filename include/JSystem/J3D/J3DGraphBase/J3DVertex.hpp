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

	J3DVertexData* mVertexData;
	void* unk4;
	u32 unk8;
	void* unkC;
	u32 unk10;
	void* unk14;
	u32 unk18;
	// TODO: figure out what actually lives here
	void* unk1C[2];
	void* unk24[2];
	void* unk2C;
	void* unk30;
	void* unk34;

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

	s16 unk0;
	u32 unk4;
	u32 unk8;
};

#endif
