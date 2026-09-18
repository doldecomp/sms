#ifndef MTX_UTIL_HPP
#define MTX_UTIL_HPP

#include <JSystem/JGeometry.hpp>
#include <System/Params.hpp>
#include <System/ParamInst.hpp>

class J3DNode;
class J3DModel;
class THitActor;

enum TMtxEffectType {
	TMTX_EFFECT_TIME_LAG,
	TMTX_EFFECT_SWING_RZ,
	TMTX_EFFECT_SWING_RZ_REVERSE_XZ,

	TMTX_EFFECT_TYPE_MAX
};

void MtxToQuat(MtxPtr, Quaternion*);

class TMtxEffectBase {
public:
	TMtxEffectBase()
	    : mFlags(0)
	{
	}

	virtual TParams* getParams() { return nullptr; }

	bool checkFlag(int flag) const { return mFlags & flag; }
	void offFlag(int flag) { mFlags &= ~flag; }
	void onFlag(int flag) { mFlags |= flag; }

	u16 mFlags;
};

class TMtxTimeLag : public TMtxEffectBase {
public:
	class TDeParams : public TParams {
	public:
		TDeParams(const char* prm);

		/* 0x08 */ TParamRT<f32> mPosAccel;
		/* 0x1C */ TParamRT<f32> mPosBrake;
		/* 0x30 */ TParamRT<f32> mPosLimit;
		/* 0x44 */ TParamRT<f32> mQuatAccel;
		/* 0x58 */ TParamRT<f32> mQuatBrake;
	};

	TMtxTimeLag(const char* prm)
	    : TMtxEffectBase()
	    , mParams(prm)
	{
	}

	virtual TParams* getParams() { return &mParams; }
	void calc(MtxPtr);
	TDeParams* getSwingRZParams() { return (TDeParams*)getParams(); }

	/* 0x08 */ Vec unk08;
	/* 0x14 */ Vec unk14;
	/* 0x20 */ Quaternion unk20;
	/* 0x30 */ Quaternion unk30;
	/* 0x40 */ TDeParams mParams;
};

int TMtxTimeLagCallBack(J3DNode*, int);

class TMtxSwingRZ : public TMtxEffectBase {
public:
	class TDeParams : public TParams {
	public:
		// no ctor exists in symbol map so weak inlined?
		TDeParams(const char* prm)
		    : TParams(prm)
		    , PARAM_INIT(mAcc, JGeometry::TVec3<f32>(0.0f, -4.0f, 0.0f))
		    , PARAM_INIT(mL, 50.0f)
		    , PARAM_INIT(mBrake, 0.7f)
		    , PARAM_INIT(mVelScale, 1.0f)
		{
			TParams::load(mPrmPath);
		}

		/* 0x08 */ TParamVec mAcc;
		/* 0x24 */ TParamRT<f32> mL;
		/* 0x38 */ TParamRT<f32> mBrake;
		/* 0x4C */ TParamRT<f32> mVelScale;
	};

	TMtxSwingRZ(const char* prm)
	    : TMtxEffectBase()
	    , mParams(prm)
	{
	}

	virtual TParams* getParams() { return &mParams; }
	void calcLocalXY(MtxPtr, Vec*, Vec*);
	void calc(MtxPtr);
	TDeParams* getSwingRZParams() { return (TDeParams*)getParams(); }

public:
	/* 0x08 */ Vec unk08;
	/* 0x14 */ Vec unk14;
	/* 0x20 */ TDeParams mParams;
};

int TMtxSwingRZCallBack(J3DNode*, int);

class TMtxSwingRZReverseXZ : public TMtxSwingRZ {
public:
	TMtxSwingRZReverseXZ(const char* prm)
	    : TMtxSwingRZ(prm)
	{
	}
	void calc(MtxPtr);
};

int TMtxSwingRZReverseXZCallBack(J3DNode*, int);

class TMultiMtxEffect {
public:
	void setup(J3DModel*, const char*);
	void setUserArea();

	// Unused
	void add();

	// Unused
	void remove();

	// Fabricated
	void flagOn(u32 flag)
	{
		for (int i = 0; i < mNumBones; ++i) {
			mMtxEffectTbl[i]->mFlags |= flag;
		}
	}
	// Fabricated
	void flagOff(u32 flag)
	{
		for (int i = 0; i < mNumBones; ++i) {
			mMtxEffectTbl[i]->mFlags &= ~flag;
		}
	}

public:
	/* 0x00 */ u16 mNumBones;      // number of bones to be manipulated
	/* 0x04 */ u8* mMtxEffectType; // array of TMtxEffectBase types
	/* 0x08 */ u16* mBoneIDs;      // array of bone IDs to be manipulated
	/* 0x0C */ J3DModel* mModel;   // model to be manipulated
	/* 0x10 */ TMtxEffectBase** mMtxEffectTbl; // array of TMtxEffectBase
};

void SMS_MakeJointsToArc(J3DModel*, const JGeometry::TVec3<f32>&,
                         const JGeometry::TVec3<f32>&,
                         const JGeometry::TVec3<f32>&);
void SMS_MtxLookAt(MtxPtr, const JGeometry::TVec3<f32>&,
                   const JGeometry::TVec3<f32>&, const JGeometry::TVec3<f32>&);
void SMS_GetLightPerspectiveForEffectMtx(MtxPtr);

class TRopePoint {
public:
	// __ct__10TRopePointFv is weak (4 bytes) in the map: the implicit
	// default constructor, non-trivial only because TVec3<f32> has one.

	/* 0x00 */ JGeometry::TVec3<f32> unk0;
	/* 0x0C */ JGeometry::TVec3<f32> unkC;
	/* 0x18 */ JGeometry::TVec3<f32> unk18;
	/* 0x24 */ f32 unk24;
	/* 0x28 */ u32 unk28;
};

class TRope {
public:
	TRope(u16, const JGeometry::TVec3<f32>&, f32, f32, f32, f32);
	void collision();
	void constraintHead(const JGeometry::TVec3<f32>&);
	void constraintTail(const JGeometry::TVec3<f32>&);
	void moveHead(const JGeometry::TVec3<f32>&);
	void moveHeadAndTail(const JGeometry::TVec3<f32>&,
	                     const JGeometry::TVec3<f32>&);

public:
	/* 0x00 */ u16 mNumPoints;
	/* 0x04 */ TRopePoint* mPoints;
	/* 0x08 */ f32 unk8;
	/* 0x0C */ f32 unkC;
	/* 0x10 */ f32 unk10;
};

void SMS_GetActorMtx(const THitActor&, MtxPtr);

// Not an inline: the map has exactly one global copy of this, emitted from
// Enemy/Kazekun.cpp, and fireWanwan and wireTrap both call it out of line.
void SMS_CalcToDirMatrix(TPosition3f& mtx, const JGeometry::TVec3<f32>& dir,
                         const JGeometry::TVec3<f32>& up);

#endif
