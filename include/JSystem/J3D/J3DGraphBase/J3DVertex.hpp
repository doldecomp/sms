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

	u32 getVtxNum() const { return unk0; }
	u32 getNrmNum() const { return unk4; }

	const GXVtxAttrFmtList* getVtxAttrFmtList() const
	{
		return mVtxAttrFmtList;
	}

public:
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

enum J3DDeformAttachFlag {
	J3D_DEFORM_ATTACH_FLAG_UNK_1 = 1,
};

class J3DVertexBuffer {
public:
	/* 0x00 */ J3DVertexData* mVertexData;
	/* 0x04 */ void* unk4[2];
	/* 0x0C */ void* unkC[2];
	/* 0x14 */ void* unk14;
	/* 0x18 */ u32 unk18;
	// TODO: figure out what actually lives here
	/* 0x1C */ void* unk1C[2];
	/* 0x24 */ void* unk24[2];
	/* 0x2C */ void* unk2C;
	/* 0x30 */ void* unk30;
	/* 0x34 */ void* unk34;

public:
	J3DVertexBuffer(J3DVertexData* vertex_data);

	virtual ~J3DVertexBuffer(); // vt at 0x38

	void frameInit()
	{
		unk2C = unk4;
		unk30 = unkC;
		unk34 = unk14;
	}

	void swapVtxPosArrayPointer()
	{
		void* tmp = unk4[0];
		unk4[0]   = unk4[1];
		unk4[1]   = tmp;
	}

	void swapVtxNrmArrayPointer()
	{
		void* tmp = unkC[0];
		unkC[0]   = unkC[1];
		unkC[1]   = tmp;
	}

	void swapTransformedVtxPos()
	{
		void* tmp = unk1C[0];
		unk1C[0]  = unk1C[1];
		unk1C[1]  = tmp;
	}

	void swapTransformedVtxNrm()
	{
		void* tmp = unk24[0];
		unk24[0]  = unk24[1];
		unk24[1]  = tmp;
	}

	void* getVtxPosArrayPointer(int idx) { return unk4[idx]; }
	void* getVtxNrmArrayPointer(int idx) { return unkC[idx]; }

	J3DVertexData* getVertexData() { return mVertexData; }

	void setCurrentVtxPos(void* pVtxPos) { unk2C = pVtxPos; }
	void setCurrentVtxNrm(void* pVtxNrm) { unk30 = pVtxNrm; }

	void* getCurrentVtxPos() { return unk2C; }
	void* getCurrentVtxNrm() { return unk30; }
	void* getTransformedVtxPos(int idx) { return unk1C[idx]; }
	void* getTransformedVtxNrm(int idx) { return unk24[idx]; }

	void copyLocalVtxArray(J3DDeformAttachFlag);
	void copyVtxColorArray(J3DDeformAttachFlag);
	void copyTransformedVtxArray();
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
