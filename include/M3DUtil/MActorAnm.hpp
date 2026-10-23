#ifndef M3DUTIL_M_ACTOR_ANM_HPP
#define M3DUTIL_M_ACTOR_ANM_HPP

#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <M3DUtil/MActorData.hpp>
#include <System/Application.hpp>
#include <string.h>

static const char* MtxCalcTypeName[] = {
	"MActorMtxCalcType_Basic クラシックスケールＯＮ",
	"MActorMtxCalcType_Softimage クラシックスケールＯＦＦ",
	"MActorMtxCalcType_MotionBlend モーションブレンド",
	"MActorMtxCalcType_User ユーザー定義",
};

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
protected:
	/* 0x0 */ s32 mCurAnmIdx;
	/* 0x4 */ J3DFrameCtrl mFrameCtrl;
	/* 0x18 */ J3DModel* mModel;
	/* 0x1C */ MActorAnmDataBase* mAnmData;

public:
	MActorAnmBase()
	    : mCurAnmIdx(-1)
	    , mModel(nullptr)
	    , mAnmData(nullptr)
	{
	}
	~MActorAnmBase() { }

	// fabricated
	J3DFrameCtrl* getFrameCtrl() { return &mFrameCtrl; }
	s32 getCurAnmIdx() { return mCurAnmIdx; }
	void setAnmData(MActorAnmDataBase* param_1) { mAnmData = param_1; }
	void stopAnm() { mCurAnmIdx = -1; }

	BOOL endsNext()
	{
		if (mFrameCtrl.getFrame() + mFrameCtrl.getRate() + 0.1f
		    >= mFrameCtrl.getEnd())
			return true;
		else
			return false;
	}

	int findName(const char* name)
	{
		u16 key = MActorCalcKeyCode(name);

		int totalKeyMatches = 0;
		int match           = -1;
		for (int i = 0; i < mAnmData->getAnmNum(); ++i)
			if (key == mAnmData->getKeyCode(i)) {
				++totalKeyMatches;
				match = i;
			}

		if (totalKeyMatches == 1)
			return match;

		for (int i = 0; i < mAnmData->getAnmNum(); ++i)
			if (key == mAnmData->getKeyCode(i)
			    && strcmp(name, mAnmData->getName(i)) == 0)
				return i;

		mAnmData->checkLower(name);
		verifySuffix(name);
		return -1;
	}

	int findName2(const char* name)
	{
		u16 key = MActorCalcKeyCode(name);

		int totalKeyMatches = 0;
		int match           = -1;
		for (int i = 0; i < mAnmData->getAnmNum(); ++i)
			if (key == mAnmData->getKeyCode(i)) {
				++totalKeyMatches;
				match = i;
			}

		if (totalKeyMatches == 0)
			return -1;

		if (totalKeyMatches == 1)
			return match;

		for (int i = 0; i < mAnmData->getAnmNum(); ++i)
			if (key == mAnmData->getKeyCode(i)
			    && strcmp(name, mAnmData->getName(i)) == 0)
				return i;

		return -1;
	}

	virtual void setModel(J3DModel* model) { mModel = model; }
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
	MActorAnmEach() { mCurAnmPtr = nullptr; }
	~MActorAnmEach() { }

	virtual void setAnm(const char* name, u16* param_2)
	{
		this->setAnmFromIndex(findName(name), param_2);
	}

	// matches in bossgesso.cpp
	bool setFrameCtrl(int anm_idx)
	{
		mCurAnmIdx = anm_idx;

		if (anm_idx < 0)
			return false;

		mCurAnmPtr = getData()->getAnmPtr(anm_idx);
		mFrameCtrl.init(mCurAnmPtr->getFrameMax());
		mFrameCtrl.setAttribute(mCurAnmPtr->getAttribute());
		mFrameCtrl.setRate(SMSGetAnmFrameRate());

		return true;
	}

	// fabricated
	MActorAnmDataEach<T>* getData()
	{
		return static_cast<MActorAnmDataEach<T>*>(mAnmData);
	}

	T* getCurAnmPtr() { return mCurAnmPtr; }

protected:
	/* 0x24 */ T* mCurAnmPtr;
};

template <class T> class MActorAnmMatEach : public MActorAnmEach<T> {
public:
	~MActorAnmMatEach() { }

	virtual void setAnmFromIndex(int anm_idx, u16* param_2)
	{
		if (this->setFrameCtrl(anm_idx)) {
			this->mCurAnmPtr->searchUpdateMaterialID(
			    this->mModel->getModelData());
			this->checkUseMaterialID(param_2);
		}
	}
};

class MActorAnmBlk : public MActorAnmEach<J3DAnmClusterKey> {
public:
	MActorAnmBlk() { mDeformData = nullptr; }

	virtual void updateIn();
	virtual void updateOut();
	virtual void setAnmFromIndex(int, u16*);
	virtual const char* verifySuffix(const char* param_1)
	{
		return strstr(param_1, ".blk");
	}

private:
	/* 0x28 */ J3DDeformData* mDeformData;
};

class MActorAnmBrk : public MActorAnmMatEach<J3DAnmTevRegKey> {
public:
	MActorAnmBrk() { }

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

private:
	/* 0x28 */ J3DTevColorAnm** mTevColorAnms;
	/* 0x2C */ J3DTevKColorAnm** mTevKColorAnms;
};

class MActorAnmBpk : public MActorAnmMatEach<J3DAnmColorKey> {
public:
	MActorAnmBpk() { mMatColorAnms = nullptr; }

	virtual void updateIn();
	virtual void updateOut();
	virtual void checkUseMaterialIDInit(u16*);
	virtual void checkUseMaterialID(u16*);
	virtual const char* verifySuffix(const char* param_1)
	{
		return strstr(param_1, ".bpk");
	}

	void setMatColorAnmKeyPtr();

private:
	/* 0x28 */ J3DMatColorAnm** mMatColorAnms;
};

class MActorAnmBtk : public MActorAnmMatEach<J3DAnmTextureSRTKey> {
public:
	MActorAnmBtk() { mTexMtxAnms = nullptr; }

	virtual void updateIn();
	virtual void updateOut();
	virtual void checkUseMaterialIDInit(u16*);
	virtual void checkUseMaterialID(u16*);
	virtual const char* verifySuffix(const char* param_1)
	{
		return strstr(param_1, ".btk");
	}

	void setTexMtxAnmKeyPtr();

private:
	/* 0x28 */ J3DTexMtxAnm** mTexMtxAnms;
};

class MActorAnmBtp : public MActorAnmMatEach<J3DAnmTexPattern> {
public:
	MActorAnmBtp() { mTexNoAnms = nullptr; }

	virtual void updateIn();
	virtual void updateOut();
	virtual void checkUseMaterialIDInit(u16*);
	virtual void checkUseMaterialID(u16*);
	virtual const char* verifySuffix(const char* param_1)
	{
		return strstr(param_1, ".btp");
	}

	void setTexNoAnmFullPtr();

private:
	/* 0x28 */ J3DTexNoAnm** mTexNoAnms;
};

enum MActorMtxCalcType {
	MActorMtxCalcType_Basic,
	MActorMtxCalcType_Softimage,
	MActorMtxCalcType_MotionBlend,
	MActorMtxCalcType_User,
};

class MActorAnmBck : public MActorAnmEach<J3DAnmTransformKey> {
public:
	MActorAnmBck()
	{
		mJointIndex       = 0;
		mMtxCalcType      = MActorMtxCalcType_Softimage;
		mMtxCalcBasic     = nullptr;
		mMtxCalcSoftimage = nullptr;
		mMotionBlendCtrl  = nullptr;
		mUserMtxCalc      = nullptr;
	}

	virtual void updateIn();
	virtual void updateOut();
	virtual void setAnmFromIndex(int, u16*);
	virtual const char* verifySuffix(const char* param_1)
	{
		return strstr(param_1, ".bck");
	}

	void setModel(J3DModel* model);
	f32 getOldMotionBlendFrame() const;
	void setOldMotionBlendAnmPtr(J3DAnmTransform* anm_ptr);
	J3DAnmTransform* getOldMotionBlendAnmPtr() const;
	void setMotionBlendRatio(f32 ratio);
	f32 getMotionBlendRatio() const;
	void initNormalMotionBlend();
	void initSimpleMotionBlend(int);
	void changeMtxCalcType(u8);

	// fabricated
	u8 getMtxCalcType() const { return mMtxCalcType; }

	// fabricated
	void setCalc(J3DMtxCalc* calc)
	{
		mUserMtxCalc = calc;
		mMtxCalcType = MActorMtxCalcType_User;
	}

	// fabricated for bosswanwan.cpp
	void setCalcType(u8 type) { mMtxCalcType = type; }

	void setJointIndex(u16 value) { mJointIndex = value; }

private:
	/* 0x28 */ u16 mJointIndex;
	/* 0x2A */ u8 mMtxCalcType;
	/* 0x2C */ J3DMtxCalcBasicAnm* mMtxCalcBasic;
	/* 0x30 */ J3DMtxCalcSoftimageAnm* mMtxCalcSoftimage;
	/* 0x34 */ TMotionBlendCtrl* mMotionBlendCtrl;
	/* 0x38 */ J3DMtxCalc* mUserMtxCalc;
};

#endif
