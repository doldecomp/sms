#ifndef J3D_MODEL_HPP
#define J3D_MODEL_HPP

#include <types.h>
#include <JSystem/J3D/J3DVertex.hpp>
#include <JSystem/J3D/J3DMaterialAttach.hpp>
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
class J3DModelData : public J3DDrawMtxData, public J3DVertexData {
public:
	typedef void IDK;
	IDK clear();
	J3DModelData();
	virtual ~J3DModelData();

	IDK makeHierarchy(J3DNode*, const J3DModelHierarchy**);
	IDK newSharedDisplayList();
	IDK isDeformableVertexFormat() const;
	IDK setMaterialTable(J3DMaterialTable*, J3DMaterialCopyFlag);
	IDK entryMatColorAnimator(J3DAnmColor*);
	IDK entryTexNoAnimator(J3DAnmTexPattern*);
	IDK entryTexMtxAnimator(J3DAnmTextureSRTKey*);
	IDK entryTevRegAnimator(J3DAnmTevRegKey*);
	IDK removeMatColorAnimator(J3DAnmColor*);
	IDK removeTexNoAnimator(J3DAnmTexPattern*);
	IDK removeTexMtxAnimator(J3DAnmTextureSRTKey*);
	IDK removeTevRegAnimator(J3DAnmTevRegKey*);
	IDK setMatColorAnimator(J3DAnmColor*, J3DMatColorAnm*);
	IDK setTexNoAnimator(J3DAnmTexPattern*, J3DTexNoAnm*);
	IDK setTexMtxAnimator(J3DAnmTextureSRTKey*, J3DTexMtxAnm*, J3DTexMtxAnm*);
	IDK setTevRegAnimator(J3DAnmTevRegKey*, J3DTevColorAnm*, J3DTevKColorAnm*);

	char padding0[0x18];
	J3DMaterialTable mMaterialTable;
	char padding1[0x7c];
	JUTNameTab* unkA8;

	struct UnknownStruct {
		u16 unk0;
		char unk2[2];
		ResTIMG* unk4;
	};

	UnknownStruct* unkAC;
	char padding2[0x10];
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

	typedef void IDK;
	IDK initialize();
	IDK entryModelData(J3DModelData*, u32, u32);
	IDK lock();
	IDK unlock();
	IDK makeDL();
	IDK patch();
	IDK setDeformData(J3DDeformData*, J3DDeformAttachFlag);
	IDK setSkinDeform(J3DSkinDeform*, J3DDeformAttachFlag);
	IDK setVtxColorCalc(J3DVtxColorCalc*, J3DDeformAttachFlag);
	IDK setVtxShader(J3DVtxShader*, J3DDeformAttachFlag);
	IDK calcWeightEnvelopeMtx();
	IDK calcBaseMtx();
	virtual IDK update();
	virtual IDK entry();
	virtual IDK calc();
	virtual IDK viewCalc();
	IDK calcNrmMtx();
	IDK calcBumpMtx();
	IDK calcBBoard();
	IDK prepareShapePackets();

	virtual ~J3DModel();

	struct UnknownStruct {
		char padding[0x14];
		float unk14;
	};

	void* unk0; // some JThing w/ a lot of data inside of it
	char padding0[0x50];
	UnknownStruct* unk58;
	char padding1[0x24];
	void* unk80; // array of things of size 0x48 judging by SMS_UnifyMaterial
	char padding5c[0x1c];
};

#endif
