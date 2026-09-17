#ifndef MARIO_UTIL_SHADOW_UTIL_HPP
#define MARIO_UTIL_SHADOW_UTIL_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/JGadget/std-list.hpp>
#include <dolphin/gx.h>
#include <dolphin/mtx.h>

class THitActor;
class J3DModel;
class J3DModelData;
class SDLModelData;
class TCircleShadowRequest;

enum {
	SHADOW_TYPE_CIRCLE = 0,
	SHADOW_TYPE_SQUARE = 1,
	SHADOW_TYPE_TREE   = 2,
	SHADOW_TYPE_SHIP   = 3,
};

class TCircleShadowRequest {
public:
	TCircleShadowRequest()
	    : mRadiusX(0.0f)
	    , mRadiusZ(0.0f)
	    , mRotationY(0.0f)
	    , mCameraDistSq(0.0f)
	    , mShadowType(SHADOW_TYPE_CIRCLE)
	    , mNeedsGroundCheck(1)
	    , mActorType(0)
	{
		mPosition.set(0.0f, 0.0f, 0.0f);
	}

public:
	/* 0x0 */ JGeometry::TVec3<f32> mPosition;
	/* 0xC */ f32 mRadiusX;
	/* 0x10 */ f32 mRadiusZ;
	/* 0x14 */ f32 mRotationY;
	/* 0x18 */ f32 mCameraDistSq;
	/* 0x1C */ u8 mShadowType;
	/* 0x1D */ u8 mNeedsGroundCheck;
	/* 0x20 */ u32 mActorType;
};

class TAlphaShadowQuad {
public:
	TAlphaShadowQuad()
	    : mRadius(0.01f)
	    , mSquareOutline(nullptr)
	    , mRequest(nullptr)
	    , mNext(nullptr)
	{
	}

	void reset();

public:
	/* 0x0 */ f32 mRadius;
	/* 0x4 */ Mtx mMtx;
	/* 0x34 */ Vec mCorners[4];
	/* 0x64 */ Vec* mSquareOutline;
	/* 0x68 */ TCircleShadowRequest* mRequest;
	/* 0x6C */ TAlphaShadowQuad* mNext;
};

class TAlphaShadowBlendQuad {
public:
	TAlphaShadowBlendQuad()
	{
		mMin.set(-1.0f, -1.0f, -1.0f);
		mMax.set(1.0f, 1.0f, 1.0f);
		unk18 = 0;
		mNext = nullptr;
	}

public:
	/* 0x0 */ JGeometry::TVec3<f32> mMin;
	/* 0xC */ JGeometry::TVec3<f32> mMax;
	/* 0x18 */ u32 unk18;
	/* 0x1C */ TAlphaShadowBlendQuad* mNext;
};

class TAlphaShadowQuadAry {
public:
	TAlphaShadowQuadAry()
	    : mQuadHead(nullptr)
	    , mQuadTail(nullptr)
	    , mBlendHead(nullptr)
	    , mBlendTail(nullptr)
	{
	}

public:
	/* 0x0 */ u32 unk0;
	/* 0x4 */ TAlphaShadowQuad* mQuadHead;
	/* 0x8 */ TAlphaShadowQuad* mQuadTail;
	/* 0xC */ TAlphaShadowBlendQuad* mBlendHead;
	/* 0x10 */ TAlphaShadowBlendQuad* mBlendTail;
};

class TSquareShadowInfo {
public:
	TSquareShadowInfo();

public:
	/* 0x0 */ Vec mPoints[5];
};

class TModelShadowInfo {
public:
	TModelShadowInfo();

public:
	/* 0x0 */ JGeometry::TVec3<f32> mPosition;
	/* 0xC */ bool mIsFar;
	/* 0xD */ bool unkD;
	/* 0x10 */ f32 unk10;
};

class TModelShadow {
public:
	TModelShadow(SDLModelData*, void*, int);

	void update();
	void calc(int, JDrama::TGraphics*);
	void draw(int, JDrama::TGraphics*);
};

class TMBindShadowBody;

class TMBindShadowParts {
public:
	TMBindShadowParts(J3DModel*, u8, TMBindShadowBody*, f32);

	void calc(f32);

public:
	/* 0x0 */ f32 mMinRadius;
	/* 0x4 */ TMBindShadowBody* mBody;
	/* 0x8 */ const char* mJointName;
	/* 0xC */ MtxPtr mJointMtx;
	/* 0x10 */ MtxPtr mChildMtx;
	/* 0x14 */ bool unk14;
	/* 0x15 */ bool mIsCircle;
	/* 0x16 */ bool mIsBody;
};

class TMBindShadowBody {
public:
	TMBindShadowBody(THitActor*, J3DModel*, f32);

	bool isUseThisJoint(int);
	bool isCircleJoint(int);
	bool isBodyJoint(int);
	void entryDrawShadow();
	void calc();

public:
	/* 0x0 */ TMBindShadowParts** mParts;
	/* 0x4 */ THitActor* mActor;
	/* 0x8 */ int mPartsNum;
	/* 0xC */ const char* mActorName;
	/* 0x10 */ f32 mCircleRadius;
	/* 0x14 */ f32 mPartsRadius;
	/* 0x18 */ f32 mBodyRadius;
};

class TMBindShadowManager;

extern TMBindShadowManager* gpBindShadowManager;

class TMBindShadowManager : public JDrama::TViewObj {
public:
	TMBindShadowManager(const char* name = "<TMBindShadowManager>");

	virtual void load(JSUMemoryInputStream& stream);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	void reset();
	void initEntry(TMBindShadowBody*);
	void drawShadowVolume(bool, TAlphaShadowQuad*);
	void drawShadowGD(u32, JDrama::TGraphics*);
	void drawShadow(u32, JDrama::TGraphics*);
	void request(const TCircleShadowRequest&, u32);
	void forceRequest(const TCircleShadowRequest&, u32);
	void calcVtx();

public:
	static f32 mJoinDist;
	static f32 mSquareShadowHeight;
	static f32 mTreeScale;
	static f32 mYScalePlus;
	static u8 mTestSw;
	static u8 mDLSw;

public:
	/* 0x10 */ TCircleShadowRequest* mRequests;
	/* 0x14 */ int mRequestNum;
	/* 0x18 */ TAlphaShadowQuad* mQuads;
	/* 0x1C */ TAlphaShadowQuadAry* mQuadArys;
	/* 0x20 */ int mQuadAryNum;
	/* 0x24 */ TAlphaShadowBlendQuad* mBlendQuads;
	/* 0x28 */ TSquareShadowInfo* mSquareShadow;
	/* 0x2C */ int mSquareShadowNum;
	/* 0x30 */ JGeometry::TVec3<f32> mLightDir;
	/* 0x3C */ SDLModelData** mModelDatas;
	/* 0x40 */ u16 mModelShadowNum;
	/* 0x44 */ u32 unk44;
	/* 0x48 */ u8 unk48;
	/* 0x49 */ u8 unk49;
	/* 0x4C */ JGadget::TList<TMBindShadowBody*> mBodyList;
	/* 0x5C */ GXColor mShadowColor;
	/* 0x60 */ f32 unk60;
	/* 0x64 */ bool unk64;
	/* 0x65 */ bool unk65;
	/* 0x68 */ f32 unk68;
	/* 0x6C */ f32 unk6C;
	/* 0x70 */ TModelShadowInfo* mModelShadows;
};

#endif
