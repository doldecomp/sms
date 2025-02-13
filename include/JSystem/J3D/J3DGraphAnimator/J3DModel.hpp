#ifndef J3D_MODEL_HPP
#define J3D_MODEL_HPP

#include <types.h>
#include <JSystem/J3D/J3DGraphBase/J3DVertex.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DPacket.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DMaterialAttach.hpp>
#include <JSystem/ResTIMG.hpp>
#include <dolphin/mtx.h>

struct J3DNode;
struct J3DModelHierarchy;
struct J3DAnmColor;
struct J3DAnmTexPattern;
struct J3DAnmTextureSRTKey;
struct J3DAnmTevRegKey;
struct J3DMatColorAnm;
struct J3DTexNoAnm;
struct J3DTexMtxAnm;
struct J3DTevColorAnm;
struct J3DTevKColorAnm;
class J3DMaterial;
class J3DJoint;

enum J3DMaterialCopyFlag {
	J3DMatCopyFlag_Material = 0x01,
	J3DMatCopyFlag_Texture  = 0x02,
	J3DMatCopyFlag_All      = 0x03,
};

class JUTNameTab;

struct J3DModelHierarchy {
	/* 0x0 */ u16 mType; // TODO enum
	/* 0x2 */ u16 mValue;

	inline u16 getValue() const { return mValue; }
};

// Should be 0xbc bytes
class J3DModelData {
public:
	void clear();
	J3DModelData();
	virtual ~J3DModelData();

	void makeHierarchy(J3DNode*, const J3DModelHierarchy**);
	void newSharedDisplayList();
	bool isDeformableVertexFormat() const;
	void setMaterialTable(J3DMaterialTable*, J3DMaterialCopyFlag);
	// TODO: these seem to return some kind of an error code?
	int entryMatColorAnimator(J3DAnmColor*);
	int entryTexNoAnimator(J3DAnmTexPattern*);
	int entryTexMtxAnimator(J3DAnmTextureSRTKey*);
	int entryTevRegAnimator(J3DAnmTevRegKey*);

	int removeMatColorAnimator(J3DAnmColor*);
	int removeTexNoAnimator(J3DAnmTexPattern*);
	int removeTexMtxAnimator(J3DAnmTextureSRTKey*);
	int removeTevRegAnimator(J3DAnmTevRegKey*);

	int setMatColorAnimator(J3DAnmColor*, J3DMatColorAnm*);
	int setTexNoAnimator(J3DAnmTexPattern*, J3DTexNoAnm*);
	int setTexMtxAnimator(J3DAnmTextureSRTKey*, J3DTexMtxAnm*, J3DTexMtxAnm*);
	int setTevRegAnimator(J3DAnmTevRegKey*, J3DTevColorAnm*, J3DTevKColorAnm*);

	JUTNameTab* getMaterialName() const { return mMaterialName; }
	J3DJoint* getJointNodePointer(u16 idx) const
	{
		return mJointNodePointer[idx];
	}

	J3DMaterial* getMaterialNodePointer(u16 idx) const
	{
		return mMaterials[idx];
	}
	void setTexture(J3DTexture* texture) { unkAC = texture; }
	void setTextureName(JUTNameTab* texture_name) { unkA8 = texture_name; }

public:
	/* 0x04 */ u32 unk4;
	/* 0x08 */ u32 unk8;
	/* 0x0C */ u32 unkC;
	/* 0x10 */ J3DJoint* mRootNode;
	/* 0x14 */ u32 unk14;
	/* 0x18 */ u16 unk18;
	/* 0x1A */ u16 unk1A;
	/* 0x1C */ u16 unk1C;

	/* 0x20 */ J3DJoint** mJointNodePointer;

	// J3DMaterialTable but not refactored out yet
	/* 0x24 */ u16 unk24;
	/* 0x26 */ u16 unk26;
	/* 0x28 */ J3DMaterial** mMaterials;
	/* 0x2C */ u16 unk2C;

	/* 0x30 */ J3DShape** mShapes;
	/* 0x34 */ u16 unk34;

	/* 0x38 */ u32 unk38;
	/* 0x3C */ J3DVertexData mVertexData;
	/* 0x80 */ u32 unk80;
	/* 0x84 */ u16 unk84;

	/* 0x88 */ u32 unk88;
	/* 0x8C */ u32 unk8C;
	/* 0x90 */ u32 unk90;
	/* 0x94 */ u32 unk94;
	/* 0x98 */ J3DDrawMtxData mDrawMtxData;

	/* 0xA4 */ u32 unkA4;
	/* 0xA8 */ JUTNameTab* unkA8;

	/* 0xAC */ J3DTexture* unkAC;
	/* 0xB0 */ u32 unkB0;
	/* 0xB4 */ JUTNameTab* mMaterialName;
	/* 0xB8 */ u32 unkB8;
};

struct J3DDeformData;
struct J3DSkinDeform;
struct J3DVtxColorCalc;
struct J3DVtxShader;

// size should be 0xa0
class J3DModel {
public:
	J3DModel();
	J3DModel(J3DModelData*, u32, u32);

	void initialize();
	void entryModelData(J3DModelData*, u32, u32);
	void lock();
	void unlock();
	void makeDL();
	void patch();
	void setDeformData(J3DDeformData*, J3DDeformAttachFlag);
	void setSkinDeform(J3DSkinDeform*, J3DDeformAttachFlag);
	void setVtxColorCalc(J3DVtxColorCalc*, J3DDeformAttachFlag);
	void setVtxShader(J3DVtxShader*, J3DDeformAttachFlag);
	void calcWeightEnvelopeMtx();
	void calcBaseMtx();

	virtual void update();
	virtual void entry();
	virtual void calc();
	virtual void viewCalc();

	void calcNrmMtx();
	void calcBumpMtx();
	void calcBBoard();
	void prepareShapePackets();

	J3DModelData* getModelData() { return mModelData; }
	MtxPtr getAnmMtx(int idx) { return mNodeMatrices[idx]; }
	J3DMatPacket* getMatPacket(u16 idx) { return &mMatPackets[idx]; }
	J3DShapePacket* getShapePacket(u16 idx) { return &mShapePackets[idx]; }

	u8 getScaleFlag(int idx) const { return mScaleFlagArr[idx]; }
	void setScaleFlag(int idx, u8 param_1) { mScaleFlagArr[idx] = param_1; }

	virtual ~J3DModel();

public:
	/* 0x04 */ J3DModelData* mModelData;
	/* 0x08 */ u32 unk8;
	/* 0x0C */ void* unkC;
	char pad1[0x4];
	/* 0x14 */ float unk14;
	/* 0x18 */ float unk18;
	/* 0x1C */ float unk1C;
	/* 0x20 */ Mtx unk20;
	/* 0x50 */ u8* mScaleFlagArr;
	/* 0x54 */ void* unk54;
	/* 0x58 */ Mtx* mNodeMatrices;
	/* 0x5C */ void* unk5C;
	/* 0x60 */ void* unk60[2];
	/* 0x68 */ void* unk68[2];
	/* 0x70 */ void* unk70[2];
	char pad2[0x4];
	/* 0x7C */ void* unk7C;
	/* 0x80 */ J3DMatPacket* mMatPackets;
	/* 0x84 */ J3DShapePacket* mShapePackets;
	/* 0x88 */ u32 unk88;
	/* 0x8C */ J3DSkinDeform* mSkinDeform;
	/* 0x90 */ void* unk90;
	/* 0x94 */ void* unk94;
	/* 0x98 */ J3DVertexBuffer* mVertexBuffer;
	/* 0x9C */ void* unk9C;
};

#endif
