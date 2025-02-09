#ifndef J3D_MODEL_HPP
#define J3D_MODEL_HPP

#include <types.h>
#include <JSystem/J3D/J3DGraphBase/J3DVertex.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DMaterialAttach.hpp>
#include <JSystem/ResTIMG.hpp>

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

public:
	char padding0[0x14];
	/* 0x3C */ J3DVertexData mVertexData;
	J3DMaterialTable mMaterialTable;
	/* 0x98 */ J3DDrawMtxData mDrawMtxData;
	char padding1[0x34];
	/* 0xB4 */ JUTNameTab* mMaterialName;
	char padding2[0x48];
	/* 0xA8 */ JUTNameTab* unkA8;

	struct UnknownStruct {
		u16 unk0;
		char unk2[2];
		ResTIMG* unk4;
	};

	UnknownStruct* unkAC;
	char padding3[0x10];
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

	virtual ~J3DModel();

	struct UnknownStruct {
		char padding[0x14];
		float unk14;
	};

	/* 0x004 */ J3DModelData* mModelData;
	char padding0[0x50];
	UnknownStruct* unk58;
	char padding1[0x24];
	void* unk80; // array of things of size 0x48 judging by SMS_UnifyMaterial
	char padding5c[0x1c];
};

#endif
