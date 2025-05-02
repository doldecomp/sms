#ifndef J3D_MODEL_HPP
#define J3D_MODEL_HPP

#include <types.h>
#include <JSystem/J3D/J3DGraphBase/J3DVertex.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DPacket.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DMaterialAttach.hpp>
#include <JSystem/ResTIMG.hpp>
#include <dolphin/mtx.h>

class J3DAnmColor;
class J3DAnmTexPattern;
class J3DAnmTextureSRTKey;
class J3DAnmTevRegKey;

class J3DMatColorAnm;
class J3DTexNoAnm;
class J3DTexMtxAnm;
class J3DTevColorAnm;
class J3DTevKColorAnm;

class J3DNode;
class J3DJoint;
class J3DModel;
class J3DMaterial;
class J3DMtxCalc;

enum J3DMaterialCopyFlag {
	J3DMatCopyFlag_Material = 0x01,
	J3DMatCopyFlag_Texture  = 0x02,
	J3DMatCopyFlag_All      = 0x03,
};

typedef void (*J3DCalcCallBack)(J3DModel*, u32 timing);

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
	JUTNameTab* getJointName() const { return unkB0; }
	J3DJoint* getJointNodePointer(u16 idx) const
	{
		return mJointNodePointer[idx];
	}
	J3DShape* getShapeNodePointer(u16 idx) const
	{
		return mShapeNodePointer[idx];
	}
	u16 getDrawMtxNum() const { return mDrawMtxData.mEntryNum; }
	u16 getWEvlpMtxNum() const { return mWEvlpMtxNum; }
	u16 getJointNum() const { return mJointNum; }
	u16 getShapeNum() const { return mShapeNum; }
	u16 getMaterialNum() const { return mMaterialNum; }

	J3DMaterial* getMaterialNodePointer(u16 idx) const
	{
		return mMaterials[idx];
	}
	void setTexture(J3DTexture* texture) { unkAC = texture; }
	J3DTexture* getTexture() const { return unkAC; }
	void setTextureName(JUTNameTab* texture_name) { unkA8 = texture_name; }
	JUTNameTab* getTextureName() const { return unkA8; }
	u8 getDrawMtxFlag(u16 idx) const { return mDrawMtxData.mDrawMtxFlag[idx]; }
	u16 getDrawMtxIndex(u16 idx) const
	{
		return mDrawMtxData.mDrawMtxIndex[idx];
	}

	u32 getVtxNum() const { return mVertexData.getVtxNum(); }
	u32 getNrmNum() const { return mVertexData.getNrmNum(); }

	bool checkBBoardFlag() const { return unk1A == 1; }

	u16 getDrawFullWgtMtxNum() const { return mDrawMtxData.mDrawFullWgtMtxNum; }

	const J3DModelHierarchy* getHierarchy() const { return unk8; }
	void setHierarchy(J3DModelHierarchy* hierarchy) { unk8 = hierarchy; }

	J3DDrawMtxData& getDrawMtxData() { return mDrawMtxData; }
	J3DVertexData& getVertexData() { return mVertexData; }

	void* getVtxPosArray() const { return mVertexData.getVtxPosArray(); }

	// This is the J3DMtxCalcAnm type this model needs supposedly
	u32 getUnkC() const { return unkC & 0xf; }

public:
	/* 0x04 */ const void* unk4;
	/* 0x08 */ J3DModelHierarchy* unk8;
	/* 0x0C */ u32 unkC;
	/* 0x10 */ J3DJoint* mRootNode;
	/* 0x14 */ J3DMtxCalc* unk14;
	/* 0x18 */ u16 unk18;
	/* 0x1A */ u16 unk1A;
	/* 0x1C */ u16 mJointNum;

	/* 0x20 */ J3DJoint** mJointNodePointer;

	// J3DMaterialTable but not refactored out yet
	/* 0x24 */ u16 mMaterialNum;
	/* 0x26 */ u16 unk26;
	/* 0x28 */ J3DMaterial** mMaterials;
	/* 0x2C */ u16 mShapeNum;

	/* 0x30 */ J3DShape** mShapeNodePointer;
	/* 0x34 */ u16 unk34;

	/* 0x38 */ J3DMaterial* unk38;
	/* 0x3C */ J3DVertexData mVertexData;
	/* 0x80 */ u32 unk80;
	/* 0x84 */ u16 mWEvlpMtxNum;

	/* 0x88 */ u8* unk88;
	/* 0x8C */ u16* unk8C;
	/* 0x90 */ f32* unk90;
	/* 0x94 */ Mtx* unk94;
	/* 0x98 */ J3DDrawMtxData mDrawMtxData;

	/* 0xA4 */ u32 unkA4;

	/* 0xA8 */ JUTNameTab* unkA8;
	/* 0xAC */ J3DTexture* unkAC;

	/* 0xB0 */ JUTNameTab* unkB0;
	/* 0xB4 */ JUTNameTab* mMaterialName;
	/* 0xB8 */ JUTNameTab* unkB8;
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
	void setAnmMtx(int idx, Mtx mtx) { MTXCopy(mtx, mNodeMatrices[idx]); }
	J3DMatPacket* getMatPacket(u16 idx) { return &mMatPackets[idx]; }
	J3DShapePacket* getShapePacket(u16 idx) { return &mShapePackets[idx]; }

	u8 getScaleFlag(int idx) const { return mScaleFlagArr[idx]; }
	void setScaleFlag(int idx, u8 param_1) { mScaleFlagArr[idx] = param_1; }
	u8 getEnvScaleFlag(int idx) const { return mEvlpScaleFlagArr[idx]; }

	J3DVertexBuffer* getVertexBuffer() { return mVertexBuffer; }
	MtxPtr getWeightAnmMtx(int idx) { return unk5C[idx]; }

	bool checkFlag(u32 flag) const { return (unk8 & flag) ? 1 : 0; }

	Mtx* getDrawMtxPtr() { return mDrawMtxBuf[1][mCurrentViewNo]; }
	Mtx& getDrawMtx(int idx) { return getDrawMtxPtr()[idx]; }
	Mtx33* getNrmMtxPtr() { return mNrmMtxBuf[1][mCurrentViewNo]; }
	Mtx33& getNrmMtx(int idx) { return getNrmMtxPtr()[idx]; }
	Mtx33* getBumpMtxPtr(int idx)
	{
		return mBumpMtxArr[1][idx][mCurrentViewNo];
	}

	void swapDrawMtx()
	{
		Mtx* tmp                       = mDrawMtxBuf[0][mCurrentViewNo];
		mDrawMtxBuf[0][mCurrentViewNo] = mDrawMtxBuf[1][mCurrentViewNo];
		mDrawMtxBuf[1][mCurrentViewNo] = tmp;
	}

	void swapNrmMtx()
	{
		Mtx33* tmp                    = mNrmMtxBuf[0][mCurrentViewNo];
		mNrmMtxBuf[0][mCurrentViewNo] = mNrmMtxBuf[1][mCurrentViewNo];
		mNrmMtxBuf[1][mCurrentViewNo] = tmp;
	}

	// TODO: might be wrong
	MtxPtr getBaseTRMtx() { return unk20; }
	void setBaseTRMtx(Mtx m) { MTXCopy(m, unk20); }

	virtual ~J3DModel();

public:
	/* 0x04 */ J3DModelData* mModelData;
	/* 0x08 */ u32 unk8;
	/* 0x0C */ J3DCalcCallBack unkC;
	char pad1[0x4];
	/* 0x14 */ Vec unk14;
	/* 0x20 */ Mtx unk20;
	/* 0x50 */ u8* mScaleFlagArr;
	/* 0x54 */ u8* mEvlpScaleFlagArr;
	/* 0x58 */ Mtx* mNodeMatrices;
	/* 0x5C */ Mtx* unk5C;
	/* 0x60 */ Mtx** mDrawMtxBuf[2];
	/* 0x68 */ Mtx33** mNrmMtxBuf[2];
	/* 0x70 */ Mtx33*** mBumpMtxArr[2];
	char pad2[0x4];
	/* 0x7C */ u32 mCurrentViewNo;
	/* 0x80 */ J3DMatPacket* mMatPackets;
	/* 0x84 */ J3DShapePacket* mShapePackets;
	/* 0x88 */ J3DDeformData* mDeformData;
	/* 0x8C */ J3DSkinDeform* mSkinDeform;
	/* 0x90 */ void* unk90;
	/* 0x94 */ void* unk94;
	/* 0x98 */ J3DVertexBuffer* mVertexBuffer;
	/* 0x9C */ void* unk9C;
};

#endif
