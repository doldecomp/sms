#ifndef MAP_MAP_COLLISION_ENTRY_HPP
#define MAP_MAP_COLLISION_ENTRY_HPP

#include <dolphin/types.h>
#include <dolphin/mtx.h>
#include <JSystem/JGeometry.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <types.h>

class TLiveActor;
class TBGCheckData;

class TMapCollisionBase {
public:
	TMapCollisionBase()
	    : mCheckDatas(nullptr)
	    , mKind(0)
	    , mCheckDataNum(0)
	    , mVertexNum(0)
	    , mVertices(nullptr)
	    , mCollisionGroupNum(0)
	    , mCollisionGroups(nullptr)
	    , mFlags(0)
	{
		MTXIdentity(unk20);
		mPrevTranslation.x = 0.0f;
		mPrevTranslation.y = 0.0f;
		mPrevTranslation.z = 0.0f;
	}

	virtual void init(const char*, u16, const TLiveActor*);
	virtual void moveSRT(const JGeometry::TVec3<f32>&,
	                     const JGeometry::TVec3<f32>&,
	                     const JGeometry::TVec3<f32>&)
	{
	}
	virtual void moveTrans(const JGeometry::TVec3<f32>&) { }
	virtual void moveMtx(MtxPtr) { }
	virtual void setUp() { offFlag(FLAG_NEEDS_SETUP); }
	virtual void setUpTrans(const JGeometry::TVec3<f32>& param_1)
	{
		MsMtxSetTRS(unk20, param_1, (Vec) { 0.0f, 0.0f, 0.0f },
		            (Vec) { 1.0f, 1.0f, 1.0f });
		setUp();
	}
	virtual void remove() { onFlag(FLAG_NEEDS_SETUP); }

	void setMtx(MtxPtr mtx) { MTXCopy(mtx, unk20); }
	void setAllActor(const TLiveActor*);
	void setAllData(s16);
	void setAllBGType(u16);
	void setVertexData(u32, const JGeometry::TVec3<f32>&,
	                   const JGeometry::TVec3<f32>&,
	                   const JGeometry::TVec3<f32>&);
	void initCheckData(s16, u16, const TLiveActor*);
	void initAllCheckData(s16, const f32*, u16, const TLiveActor*);
	void update();
	void updateCheckData(const f32*);
	void updateVertexPos(f32*);
	void updateTrans(const JGeometry::TVec3<f32>&);
	void setCheckData(const f32*, const s16*, TBGCheckData*, int);
	bool isSetUp() const
	{
		if (checkFlag(FLAG_NEEDS_SETUP))
			return false;
		return true;
	}

	// fabricated
	bool checkFlag(u16 flag) const
	{
		if (mFlags & flag)
			return true;
		return false;
	}
	void onFlag(u16 flag) { mFlags |= flag; }
	void offFlag(u16 flag) { mFlags &= ~flag; }
	s32 getUnk8() const { return mKind; }
	u32 getUnkC() const { return mCheckDataNum; }
	void setUpMtx(MtxPtr mtx)
	{
		setMtx(mtx);
		setUp();
	}

public:
	enum {
		KIND_STATIC = 0,
		KIND_MOVE   = 1,
		KIND_WARP   = 2,
	};

	enum {
		FLAG_NEEDS_SETUP = 0x1,
		FLAG_UNK4000     = 0x4000,
		FLAG_UNK8000     = 0x8000,
	};

	/* 0x4 */ TBGCheckData* mCheckDatas;
	/* 0x8 */ s32 mKind;
	/* 0xC */ u32 mCheckDataNum;
	/* 0x10 */ u32 mVertexNum;
	/* 0x14 */ Vec* mVertices;

	enum {
		HAS_ADDITIONAL_DATA = 0x1,
		WAS_PATCHED         = 0x8000,
	};

	struct TMapCollisionGroup {
		/* 0x0 */ u16 mBGType;
		/* 0x2 */ s16 mTriangleNum;
		/* 0x4 */ u16 mFlags;
		/* 0x8 */ s16* mIndices;
		/* 0xC */ u8* unkC;
		/* 0x10 */ u8* unk10;
		/* 0x14 */ s16* mAdditionalDatas;
	};

	/* 0x18 */ u32 mCollisionGroupNum;
	/* 0x1C */ TMapCollisionGroup* mCollisionGroups;
	/* 0x20 */ Mtx unk20;
	/* 0x50 */ JGeometry::TVec3<f32> mPrevTranslation;
	/* 0x5C */ u16 mFlags;
};

class TMapCollisionStatic : public TMapCollisionBase {
public:
	TMapCollisionStatic();

	virtual void init(const char*, u16, const TLiveActor*);
	virtual void setUp();
	virtual void remove() { }

public:
	/* 0x60 */ const TLiveActor* mOwnerActor;
};

class TMapCollisionWarp : public TMapCollisionBase {
public:
	TMapCollisionWarp();

	virtual void init(const char*, u16, const TLiveActor*);
	virtual void setUp();
	virtual void setUpTrans(const JGeometry::TVec3<f32>&);
	virtual void remove();

public:
	/* 0x60 */ u16 mEntryId;
	/* 0x64 */ u32 mEntrySize;
};

class TMapCollisionMove : public TMapCollisionBase {
public:
	TMapCollisionMove();

	virtual void init(const char*, u16, const TLiveActor*);
	virtual void moveSRT(const JGeometry::TVec3<f32>& param_1,
	                     const JGeometry::TVec3<f32>& param_2,
	                     const JGeometry::TVec3<f32>& param_3)
	{
		MsMtxSetTRS(unk20, param_1, param_2, param_3);
		move();
	}
	virtual void moveTrans(const JGeometry::TVec3<f32>&);
	virtual void moveMtx(MtxPtr mtx) { MTXCopy(mtx, unk20); }

	void init(u32, u16 bg_type, s16 data, const TLiveActor* actor);
	void move();
	void setList();
};

#endif
