#ifndef MAP_MAP_COLLISION_ENTRY_HPP
#define MAP_MAP_COLLISION_ENTRY_HPP

#include <dolphin/types.h>
#include <JSystem/JGeometry.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <types.h>

class TLiveActor;
class TBGCheckData;

class TMapCollisionBase {
public:
	TMapCollisionBase()
	    : unk4(nullptr)
	    , unk8(0)
	    , unkC(0)
	    , unk10(0)
	    , unk14(nullptr)
	    , unk18(0)
	    , unk1C(nullptr)
	    , unk5C(0)
	{
		MTXIdentity(unk20);
		unk50.x = 0;
		unk50.y = 0;
		unk50.z = 0;
	}

	virtual void init(const char*, u16, const TLiveActor*);
	virtual void moveSRT(const JGeometry::TVec3<f32>&,
	                     const JGeometry::TVec3<f32>&,
	                     const JGeometry::TVec3<f32>&)
	{
	}
	virtual void moveTrans(const JGeometry::TVec3<f32>&) { }
	virtual void moveMtx(MtxPtr) { }
	virtual void setUp() { }
	virtual void setUpTrans(const JGeometry::TVec3<f32>& param_1)
	{
		JGeometry::TVec3<f32> vec2(0.0f, 0.0f, 0.0f);
		JGeometry::TVec3<f32> vec3(1.875f, 1.875f, 1.875f);
		MsMtxSetTRS(unk20, param_1.x, param_1.y, param_1.z, vec2.x, vec2.y,
		            vec2.z, vec3.x, vec3.y, vec3.z);
		setUp();
	}
	virtual void remove() { unk5C |= 1; }

	void setMtx(MtxPtr);
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
	void isSetUp() const;

	bool checkFlag(u16 flag) const { return unk5C & flag ? true : false; }

public:
	/* 0x4 */ TBGCheckData* unk4;
	/* 0x8 */ u32 unk8;
	/* 0xC */ u32 unkC;
	/* 0x10 */ u32 unk10;
	/* 0x14 */ Vec* unk14;
	/* 0x18 */ u32 unk18;
	/* 0x1C */ void* unk1C;
	/* 0x20 */ Mtx unk20;
	/* 0x50 */ Vec unk50;
	/* 0x5C */ u16 unk5C;
};

class TMapCollisionStatic : public TMapCollisionBase {
public:
	TMapCollisionStatic();

	virtual void init(const char*, u16, const TLiveActor*);
	virtual void setUp();
	virtual void remove() { }

public:
	/* 0x60 */ const TLiveActor* unk60;
};

class TMapCollisionWarp : public TMapCollisionBase {
public:
	TMapCollisionWarp();

	virtual void init(const char*, u16, const TLiveActor*);
	virtual void setUp();
	virtual void setUpTrans(const JGeometry::TVec3<f32>&);
	virtual void remove();

public:
	/* 0x60 */ u16 unk60;
	/* 0x64 */ u32 unk64;
};

class TMapCollisionMove : public TMapCollisionBase {
public:
	TMapCollisionMove();

	virtual void init(const char*, u16, const TLiveActor*);
	virtual void moveSRT(const JGeometry::TVec3<f32>& param_1,
	                     const JGeometry::TVec3<f32>& param_2,
	                     const JGeometry::TVec3<f32>& param_3)
	{
		MsMtxSetTRS(unk20, param_1.x, param_1.y, param_1.z, param_2.x,
		            param_2.y, param_2.z, param_3.x, param_3.y, param_3.z);

		// this is TMapCollisionMove::move, but inlined? How?
		if (checkFlag(1))
			return;

		if (checkFlag(0x4000)) {
			setList();
		} else {
			if (checkFlag(0x8000)) {
				JGeometry::TVec3<f32> local_18;
				local_18.x = unk20[0][3];
				local_18.y = unk20[1][3];
				local_18.z = unk20[2][3];
				TMapCollisionBase::updateTrans(local_18);
			} else {
				TMapCollisionBase::update();
			}
		}
	}
	virtual void moveTrans(const JGeometry::TVec3<f32>&);
	virtual void moveMtx(MtxPtr mtx) { MTXCopy(mtx, unk20); }

	void init(u32, u16, s16, const TLiveActor*);
	void move();
	void setList();
};

#endif
