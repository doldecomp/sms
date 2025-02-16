#ifndef J3D_MODEL_LOADER_HPP
#define J3D_MODEL_LOADER_HPP

#include <types.h>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/JUtility/JUTDataHeader.hpp>

struct J3DModelInfoBlock : public JUTDataBlockHeader {
	/* 0x08 */ u16 mFlags;
	/* 0x0C */ u32 mPacketNum;
	/* 0x10 */ u32 mVtxNum;
	/* 0x14 */ void* mpHierarchy;
}; // Size: 0x18

struct J3DVertexBlock : public JUTDataBlockHeader {
	/* 0x08 */ void* mpVtxAttrFmtList;
	/* 0x0C */ void* mpVtxPosArray;
	/* 0x10 */ void* mpVtxNrmArray;
	/* 0x14 */ void* mpVtxNBTArray;
	/* 0x18 */ void* mpVtxColorArray[2];
	/* 0x20 */ void* mpVtxTexCoordArray[8];
}; // Size: 0x40

struct J3DEnvelopBlock : public JUTDataBlockHeader {
	/* 0x08 */ u16 mWEvlpMtxNum;
	/* 0x0C */ void* mpWEvlpMixMtxNum;
	/* 0x10 */ void* mpWEvlpMixMtxIndex;
	/* 0x14 */ void* mpWEvlpMixWeight;
	/* 0x18 */ void* mpInvJointMtx;
}; // Size: 0x1C

struct J3DDrawBlock : public JUTDataBlockHeader {
	/* 0x08 */ u16 mMtxNum;
	/* 0x0C */ void* mpDrawMtxFlag;
	/* 0x10 */ void* mpDrawMtxIndex;
}; // Size: 0x14

struct J3DJointBlock;
struct J3DMaterialBlock;
struct J3DMaterialBlock_v21;

struct J3DMaterialDLBlock : public JUTDataBlockHeader {
	/* 0x08 */ u16 mMaterialNum;
	/* 0x0C */ void* mpDisplayListInit;
	/* 0x10 */ void* mpPatchingInfo;
	/* 0x14 */ void* mpCurrentMtxInfo;
	/* 0x18 */ void* field_0x18;
	/* 0x1C */ void* field_0x1c;
	/* 0x20 */ void* mpNameTable;
	/* more */
};

struct J3DShapeBlock;

struct J3DTextureBlock : public JUTDataBlockHeader {
	/* 0x08 */ u16 mTextureNum;
	/* 0x0C */ void* mpTextureRes;
	/* 0x10 */ void* mpNameTable;
};

class J3DModelLoaderDataBase {
public:
	// NOTE: I have no idea why, but it seems like they pass in the
	// J3DModelLoaderDataBase through a void* pointer here?!
	static J3DModelData* load(const void*, u32);
	static J3DMaterialTable* loadMaterialTable(const void*);
};

class J3DModelLoader {
public:
	J3DModelLoader()
	{
		mpModelData     = nullptr;
		mpShapeBlock    = nullptr;
		mpMaterialTable = nullptr;
	}

	virtual J3DModelData* load(const void*, u32);
	virtual J3DMaterialTable* loadMaterialTable(const void*);
	virtual void setupBBoardInfo();
	virtual ~J3DModelLoader() { }
	virtual void readMaterial(const J3DMaterialBlock*, u32) { }
	virtual void readMaterial_v21(const J3DMaterialBlock_v21*, u32) { }
	virtual void readMaterialTable(const J3DMaterialBlock*, u32) { }
	virtual void readMaterialTable_v21(const J3DMaterialBlock_v21*, u32) { }

	void readInformation(const J3DModelInfoBlock*, u32);
	void readVertex(const J3DVertexBlock*);
	void readEnvelop(const J3DEnvelopBlock*);
	void readDraw(const J3DDrawBlock*);
	void readJoint(const J3DJointBlock*);
	void readShape(const J3DShapeBlock*, u32);
	void readTexture(const J3DTextureBlock*);
	void readTextureTable(const J3DTextureBlock*);

protected:
	/* 0x04 */ J3DModelData* mpModelData;
	/* 0x08 */ J3DMaterialTable* mpMaterialTable;
	/* 0x0C */ const J3DShapeBlock* mpShapeBlock;
};

class J3DModelLoader_v26 : public J3DModelLoader {
public:
	virtual ~J3DModelLoader_v26() { }
	virtual void readMaterial(const J3DMaterialBlock*, u32);
	virtual void readMaterialTable(const J3DMaterialBlock*, u32);
};

class J3DModelLoader_v21 : public J3DModelLoader {
public:
	~J3DModelLoader_v21() { }
	void readMaterial_v21(const J3DMaterialBlock_v21*, u32);
	void readMaterialTable_v21(const J3DMaterialBlock_v21*, u32);
};

#endif
