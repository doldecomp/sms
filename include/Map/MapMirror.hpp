#ifndef MAP_MAP_MIRROR_HPP
#define MAP_MAP_MIRROR_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/JDrama/JDRCamera.hpp>
#include <JSystem/JDrama/JDRDrawBufObj.hpp>
#include <JSystem/JGeometry/JGPartition3.hpp>

struct ResTIMG;
class MActor;
class MActorAnmData;
class J3DModel;

class TMirrorCamera : public JDrama::TCamera {
public:
	TMirrorCamera(const char* name = "鏡用カメラ");

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	void makeMirrorViewMtx();
	void drawSetting(MtxPtr);
	void calcEffectMtx(MtxPtr);

	MtxPtr getViewMtx() { return mViewMtx; }
	// fabricated
	void setPlane(f32 x, f32 y, f32 z, f32 dist) { mPlane.set(x, y, z, dist); }

	const ResTIMG* getFrameBuffer() const { return mFrameBuffer; }
	// fabricated
	const JGeometry::TVec3<f32>& getUnk98() const { return mPosition; }
	const GXTexObj& getFrameBufferTexObj() const { return mFrameBufferTexObj; }

private:
	/* 0x30 */ Mtx mViewMtx;
	/* 0x60 */ GXTexObj mFrameBufferTexObj;
	/* 0x80 */ f32 mFovyMultiplier;
	/* 0x84 */ JGeometry::TPartition3<f32> mPlane;
	/* 0x94 */ ResTIMG* mFrameBuffer;
	/* 0x98 */ JGeometry::TVec3<f32> mPosition;
};

class TMirrorModel {
public:
	TMirrorModel();

	void initPlaneInfo();
	void entry();
	void calcView();
	ResTIMG* getMirrorTexInfo();

	virtual void init(const char*);
	virtual void calc() { }
	virtual void setPlane();
	virtual const JGeometry::TVec3<f32>& getNormalVec() const
	{
		return mPlane.mNormal;
	}
	virtual f32 getD() const { return mPlane.mDist; }

protected:
	/* 0x4 */ MActor* mMActor;
	/* 0x8 */ TMirrorCamera* mMirrorCamera;
	/* 0xC */ JGeometry::TVec3<f32> mModelVertexSample;
	/* 0x18 */ JGeometry::TPartition3<f32> mPlane;
};

class TMirrorModelObj : public TMirrorModel {
public:
	TMirrorModelObj()
	    : mModel(nullptr)
	{
	}

	virtual void init(const char*);
	virtual void calc();
	virtual void setPlane();

	void setModel(J3DModel* model) { mModel = model; }

public:
	/* 0x28 */ J3DModel* mModel;
};

class TMirrorModelManager;

extern TMirrorModelManager* gpMirrorModelManager;

class TMirrorModelManager : public JDrama::TViewObj {
public:
	TMirrorModelManager(const char* name = "鏡表示モデル管理");

	virtual void load(JSUMemoryInputStream& stream);
	virtual void loadAfter();
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	bool isUpperThanMirrorPlane(const JGeometry::TVec3<f32>&) const;
	bool isInMirror(JGeometry::TVec3<f32>&) const;
	void findMirrorCamera();
	void registerObjMirror(TMirrorModel*);

	// fabricated
	MActorAnmData* getSharedAnmData() { return mSharedEmptyAnmData; }
	int& getActiveMirrorIdx() { return mActiveMirrorModelIdx; }
	bool isMirrorActive() { return mActiveMirrorModelIdx != -1 ? true : false; }
	TMirrorCamera* getMirrorCamera() { return mMirrorCamera; }

public:
	/* 0x10 */ int mMirrorModelNum;
	/* 0x14 */ int mMirrorModelMax;
	/* 0x18 */ int mActiveMirrorModelIdx;
	/* 0x1C */ TMirrorModel** mMirrorModels;
	/* 0x20 */ MActorAnmData* mSharedEmptyAnmData;
	/* 0x24 */ TMirrorCamera* mMirrorCamera;
	/* 0x28 */ u16 unk28;
};

class TMirrorMapDrawBuf : public JDrama::TDrawBufObj {
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
};

#endif
