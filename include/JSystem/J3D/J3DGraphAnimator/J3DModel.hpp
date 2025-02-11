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

// TODO: this could be an enum
struct J3DMaterialCopyFlag {
	u32 unk0b : 1;
	u32 unk1b : 1;
};

class JUTNameTab;

// Should be 0xbc bytes
class J3DModelData {
public:
	void clear();
	J3DModelData();
	virtual ~J3DModelData();

	void makeHierarchy(J3DNode*, const J3DModelHierarchy**);
	void newSharedDisplayList();
	void isDeformableVertexFormat() const;
	void setMaterialTable(J3DMaterialTable*, J3DMaterialCopyFlag);
	void entryMatColorAnimator(J3DAnmColor*);
	void entryTexNoAnimator(J3DAnmTexPattern*);
	void entryTexMtxAnimator(J3DAnmTextureSRTKey*);
	void entryTevRegAnimator(J3DAnmTevRegKey*);
	void removeMatColorAnimator(J3DAnmColor*);
	void removeTexNoAnimator(J3DAnmTexPattern*);
	void removeTexMtxAnimator(J3DAnmTextureSRTKey*);
	void removeTevRegAnimator(J3DAnmTevRegKey*);
	void setMatColorAnimator(J3DAnmColor*, J3DMatColorAnm*);
	void setTexNoAnimator(J3DAnmTexPattern*, J3DTexNoAnm*);
	void setTexMtxAnimator(J3DAnmTextureSRTKey*, J3DTexMtxAnm*, J3DTexMtxAnm*);
	void setTevRegAnimator(J3DAnmTevRegKey*, J3DTevColorAnm*, J3DTevKColorAnm*);

	JUTNameTab* getMaterialName() const { return mMaterialName; }
	J3DJoint* getJointNodePointer(u16 idx) const
	{
		return mJointNodePointer[idx];
	}

public:
	/* 0x04 */ u32 unk4;
	/* 0x08 */ u32 unk8;
	/* 0x0C */ u32 unkC;
	/* 0x10 */ u32 unk10;
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

	/* 0x30 */ u32 unk30;
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
	/* 0x004 */ J3DModelData* mModelData;
	char padding0[0x48];
	/* 0x50 */ u8* mScaleFlagArr;
	char padding1[0x4];
	/* 0x58 */ Mtx* mNodeMatrices;
	char padding2[0x24];
	/* 0x80 */ J3DMatPacket* mMatPackets;
	/* 0x84 */ J3DShapePacket* mShapePackets;
	char padding5c[0x18];
};

#endif
