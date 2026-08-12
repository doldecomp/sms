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

	u32 getVtxNum() const { return mVtxNum; }
	u32 getNrmNum() const { return mNrmNum; }

	const GXVtxAttrFmtList* getVtxAttrFmtList() const
	{
		return mVtxAttrFmtList;
	}

public:
	/* 0x0 */ u32 mVtxNum;
	/* 0x4 */ u32 mNrmNum;
	/* 0x8 */ u32 mColNum;
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
	/* 0x04 */ void* mVtxPosArray[2];
	/* 0x0C */ void* mVtxNrmArray[2];
	/* 0x14 */ GXColor* mVtxColArray[2];
	// TODO: figure out what actually lives here
	/* 0x1C */ void* mTransformedVtxPosArray[2];
	/* 0x24 */ void* mTransformedVtxNrmArray[2];
	/* 0x2C */ void* unk2C;
	/* 0x30 */ void* unk30;
	/* 0x34 */ GXColor* unk34;

public:
	J3DVertexBuffer(J3DVertexData* vertex_data);

	virtual ~J3DVertexBuffer(); // vt at 0x38

	void frameInit()
	{
		setCurrentVtxPos(mVtxPosArray[0]);
		setCurrentVtxNrm(mVtxNrmArray[0]);
		setCurrentVtxCol(mVtxColArray[0]);
	}

	void swapVtxPosArrayPointer()
	{
		void* tmp       = mVtxPosArray[0];
		mVtxPosArray[0] = mVtxPosArray[1];
		mVtxPosArray[1] = tmp;
	}

	void swapVtxNrmArrayPointer()
	{
		void* tmp       = mVtxNrmArray[0];
		mVtxNrmArray[0] = mVtxNrmArray[1];
		mVtxNrmArray[1] = tmp;
	}

	void swapTransformedVtxPos()
	{
		void* tmp                  = mTransformedVtxPosArray[0];
		mTransformedVtxPosArray[0] = mTransformedVtxPosArray[1];
		mTransformedVtxPosArray[1] = tmp;
	}

	void swapTransformedVtxNrm()
	{
		void* tmp                  = mTransformedVtxNrmArray[0];
		mTransformedVtxNrmArray[0] = mTransformedVtxNrmArray[1];
		mTransformedVtxNrmArray[1] = tmp;
	}

	void* getVtxPosArrayPointer(int idx) { return mVtxPosArray[idx]; }
	void* getVtxNrmArrayPointer(int idx) { return mVtxNrmArray[idx]; }

	J3DVertexData* getVertexData() { return mVertexData; }

	void setCurrentVtxPos(void* pVtxPos) { unk2C = pVtxPos; }
	void setCurrentVtxNrm(void* pVtxNrm) { unk30 = pVtxNrm; }
	void setCurrentVtxCol(GXColor* pVtxCol) { unk34 = pVtxCol; }

	void* getCurrentVtxPos() { return unk2C; }
	void* getCurrentVtxNrm() { return unk30; }
	GXColor* getCurrentVtxCol() { return unk34; }

	void* getTransformedVtxPos(int idx) { return mTransformedVtxPosArray[idx]; }
	void* getTransformedVtxNrm(int idx) { return mTransformedVtxNrmArray[idx]; }

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
