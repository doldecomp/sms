#ifndef MAP_MAP_COLLISION_ENTRY_HPP
#define MAP_MAP_COLLISION_ENTRY_HPP

#include <dolphin/types.h>
#include <JSystem/JGeometry.hpp>

class TLiveActor;
class TBGCheckData;

class TMapCollisionBase {
public:
	TMapCollisionBase();

	virtual void init(const char*, u16, const TLiveActor*);
	virtual void moveSRT(const JGeometry::TVec3<f32>&,
	                     const JGeometry::TVec3<f32>&,
	                     const JGeometry::TVec3<f32>&);
	virtual void moveTrans(const JGeometry::TVec3<f32>&);
	virtual void moveMtx(MtxPtr);
	virtual void setUp();
	virtual void setUpTrans(const JGeometry::TVec3<f32>&);
	virtual void remove();

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
	void checkFlag(u16) const;
	void isSetUp() const;
};

class TMapCollisionStatic : public TMapCollisionBase {
public:
	TMapCollisionStatic();

	virtual void init(const char*, u16, const TLiveActor*);
	virtual void setUp();
	virtual void remove();
};

#endif
