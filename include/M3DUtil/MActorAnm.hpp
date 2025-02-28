#ifndef M3DUTIL_M_ACTOR_ANM_HPP
#define M3DUTIL_M_ACTOR_ANM_HPP

#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <M3DUtil/MActorData.hpp>
#include <M3DUtil/M3UJoint.hpp>
#include <string.h>

class J3DModel;
class J3DAnmClusterKey;
class J3DAnmTevRegKey;
class J3DAnmColorKey;
class J3DAnmTextureSRTKey;
class J3DAnmTexPattern;
class J3DAnmTransformKey;
class J3DAnmTransform;
class J3DDeformData;
class J3DTevColorAnm;
class J3DTevKColorAnm;
class J3DMatColorAnm;
class J3DTexMtxAnm;
class J3DTexNoAnm;
class J3DMtxCalc;
class J3DMtxCalcBasicAnm;
class J3DMtxCalcSoftimageAnm;
class TMotionBlendCtrl;

class MActorAnmBase {
public:
	/* 0x0 */ s32 unk0;
	/* 0x4 */ J3DFrameCtrl unk4;
	/* 0x18 */ J3DModel* unk18;
	/* 0x1C */ MActorAnmDataBase* unk1C;

public:
	MActorAnmBase();
	~MActorAnmBase() { }

	virtual void setModel(J3DModel*) { }
	virtual void updateIn() { }
	virtual void updateOut() { }
	virtual void checkUseMaterialIDInit(u16*);
	virtual void checkUseMaterialID(u16*);
	virtual void setAnmFromIndex(int, u16*)       = 0;
	virtual void setAnm(const char*, u16*)        = 0;
	virtual const char* verifySuffix(const char*) = 0;
};

template <class T> class MActorAnmEach : public MActorAnmBase {
public:
	~MActorAnmEach() { }

	virtual void setAnm(const char*, u16*);

	void setFrameCtrl(int);

	// fabricated
	MActorAnmDataEach<T>* getData()
	{
		return static_cast<MActorAnmDataEach<T>*>(unk1C);
	}
};

template <class T> class MActorAnmMatEach : public MActorAnmEach<T> {
public:
	~MActorAnmMatEach();

	virtual void setAnmFromIndex(int, u16*);

public:
	/* 0x24 */ T* unk24; // TODO: might be in MActorAnmEach
};

class MActorAnmBlk : public MActorAnmEach<J3DAnmClusterKey> {
public:
	virtual void updateIn();
	virtual void updateOut();
	virtual void setAnmFromIndex(int, u16*);
	virtual const char* verifySuffix(const char* param_1)
	{
		return strstr(param_1, ".blk");
	}

public:
	/* 0x24 */ J3DAnmCluster* unk24;
	/* 0x28 */ J3DDeformData* unk28;
};

class MActorAnmBrk : public MActorAnmMatEach<J3DAnmTevRegKey> {
public:
	virtual void updateIn();
	virtual void updateOut();
	virtual void checkUseMaterialIDInit(u16*);
	virtual void checkUseMaterialID(u16*);
	virtual const char* verifySuffix(const char* param_1)
	{
		return strstr(param_1, ".brk");
	}

	void setTevKColorAnmKeyPtr();
	void setTevColorAnmKeyPtr();

public:
	/* 0x28 */ J3DTevColorAnm** unk28;
	/* 0x2C */ J3DTevKColorAnm** unk2C;
};

class MActorAnmBpk : public MActorAnmMatEach<J3DAnmColorKey> {
public:
	virtual void updateIn();
	virtual void updateOut();
	virtual void checkUseMaterialIDInit(u16*);
	virtual void checkUseMaterialID(u16*);
	virtual const char* verifySuffix(const char* param_1)
	{
		return strstr(param_1, ".bpk");
	}

	void setMatColorAnmKeyPtr();

public:
	/* 0x28 */ J3DMatColorAnm** unk28;
};

class MActorAnmBtk : public MActorAnmMatEach<J3DAnmTextureSRTKey> {
public:
	virtual void updateIn();
	virtual void updateOut();
	virtual void checkUseMaterialIDInit(u16*);
	virtual void checkUseMaterialID(u16*);
	virtual const char* verifySuffix(const char* param_1)
	{
		return strstr(param_1, ".btk");
	}

	void setTexMtxAnmKeyPtr();

public:
	/* 0x28 */ J3DTexMtxAnm** unk28;
};

class MActorAnmBtp : public MActorAnmMatEach<J3DAnmTexPattern> {
public:
	virtual void updateIn();
	virtual void updateOut();
	virtual void checkUseMaterialIDInit(u16*);
	virtual void checkUseMaterialID(u16*);
	virtual const char* verifySuffix(const char* param_1)
	{
		return strstr(param_1, ".btp");
	}

	void setTexNoAnmFullPtr();

public:
	/* 0x28 */ J3DTexNoAnm** unk28;
};

class MActorAnmBck : public MActorAnmEach<J3DAnmTransformKey> {
public:
	virtual void updateIn();
	virtual void updateOut();
	virtual void setAnmFromIndex(int, u16*);
	virtual const char* verifySuffix(const char* param_1)
	{
		return strstr(param_1, ".bck");
	}

	void setModel(J3DModel*);
	float getOldMotionBlendFrame() const;
	void setOldMotionBlendAnmPtr(J3DAnmTransform*);
	J3DAnmTransform* getOldMotionBlendAnmPtr() const;
	void setMotionBlendRatio(float);
	float getMotionBlendRatio() const;
	void initNormalMotionBlend();
	void initSimpleMotionBlend(int);
	void changeMtxCalcType(u8);

public:
	/* 0x24 */ J3DAnmTransformKey* unk24;
	/* 0x28 */ u16 unk28;
	/* 0x2A */ u8 unk2A;
	/* 0x2C */ J3DMtxCalcBasicAnm* unk2C;
	/* 0x30 */ J3DMtxCalcSoftimageAnm* unk30;
	/* 0x34 */ TMotionBlendCtrl* unk34;
	/* 0x38 */ J3DMtxCalc* unk38;
};

#endif
