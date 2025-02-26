#ifndef M3DUTIL_M_ACTOR_ANM_HPP
#define M3DUTIL_M_ACTOR_ANM_HPP

#include <dolphin/types.h>

class J3DModel;
class J3DAnmClusterKey;
class J3DAnmTevRegKey;
class J3DAnmColorKey;
class J3DAnmTextureSRTKey;
class J3DAnmTexPattern;
class J3DAnmTransformKey;
class J3DAnmTransform;

class MActorAnmBase {
public:
	MActorAnmBase();
	~MActorAnmBase();

	virtual void setModel(J3DModel*) { }
	virtual void updateIn() { }
	virtual void updateOut() { }
	virtual void checkUseMaterialIDInit(u16*);
	virtual void checkUseMaterialID(u16*);
	virtual void setAnmFromIndex(int, u16*) = 0;
	virtual void setAnm(const char*, u16*)  = 0;
	virtual void verifySuffix(const char*)  = 0;
};

template <class T> class MActorAnmEach : public MActorAnmBase {
public:
	~MActorAnmEach();

	virtual void setAnm(const char*, u16*);

	void setFrameCtrl(int);
};

template <class T> class MActorAnmMatEach : public MActorAnmEach<T> {
public:
	~MActorAnmMatEach();

	virtual void setAnmFromIndex(int, u16*);
};

class MActorAnmBlk : public MActorAnmEach<J3DAnmClusterKey> {
public:
	virtual void updateIn();
	virtual void updateOut();
	virtual void setAnmFromIndex(int, u16*);
	virtual void verifySuffix(const char*) { }
};

class MActorAnmBrk : public MActorAnmMatEach<J3DAnmTevRegKey> {
public:
	virtual void updateIn();
	virtual void updateOut();
	virtual void checkUseMaterialIDInit(u16*);
	virtual void checkUseMaterialID(u16*);
	virtual void verifySuffix(const char*) { }

	void setTevKColorAnmKeyPtr();
	void setTevColorAnmKeyPtr();
};

class MActorAnmBpk : public MActorAnmMatEach<J3DAnmColorKey> {
public:
	virtual void updateIn();
	virtual void updateOut();
	virtual void checkUseMaterialIDInit(u16*);
	virtual void checkUseMaterialID(u16*);
	virtual void verifySuffix(const char*) { }

	void setMatColorAnmKeyPtr();
};

class MActorAnmBtk : public MActorAnmMatEach<J3DAnmTextureSRTKey> {
public:
	virtual void updateIn();
	virtual void updateOut();
	virtual void checkUseMaterialIDInit(u16*);
	virtual void checkUseMaterialID(u16*);
	virtual void verifySuffix(const char*) { }

	void setTexMtxAnmKeyPtr();
};

class MActorAnmBtp : public MActorAnmMatEach<J3DAnmTexPattern> {
public:
	virtual void updateIn();
	virtual void updateOut();
	virtual void checkUseMaterialIDInit(u16*);
	virtual void checkUseMaterialID(u16*);
	virtual void verifySuffix(const char*) { }

	void setTexNoAnmFullPtr();
};

class MActorAnmBck : public MActorAnmEach<J3DAnmTransformKey> {
public:
	virtual void updateIn();
	virtual void updateOut();
	virtual void setAnmFromIndex(int, u16*);
	virtual void verifySuffix(const char*) { }

	void setModel(J3DModel*);
	void getOldMotionBlendFrame() const;
	void setOldMotionBlendAnmPtr(J3DAnmTransform*);
	void getOldMotionBlendAnmPtr() const;
	void setMotionBlendRatio(float);
	void getMotionBlendRatio() const;
	void initNormalMotionBlend();
	void initSimpleMotionBlend(int);
	void changeMtxCalcType(u8);
};

#endif
