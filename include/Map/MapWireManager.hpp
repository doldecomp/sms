#ifndef MAP_MAP_WIRE_MANAGER_HPP
#define MAP_MAP_WIRE_MANAGER_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>
#include <Strategic/TakeActor.hpp>

class TMapWireActorManager;

class TMapWireActor : public TTakeActor {
public:
	MtxPtr getTakingMtx() { return nullptr; }
	void checkTakingActor();
	void getPosInWire() const;
	void getTipPoints(JGeometry::TVec3<f32>*, JGeometry::TVec3<f32>*) const;
	u32 receiveMessage(THitActor*, u32);
	void init(TMapWireActorManager*);
	TMapWireActor(const char*);
};

class TMapWireActorManager {
public:
	void doActorToWire();
	void doWireToActor();
	TMapWireActorManager(TTakeActor*);
};

class TMapWireManager;

TMapWireManager* gpMapWireManager;

class TMapWireManager : public JDrama::TViewObj {
public:
	void getWireNo(const JGeometry::TVec3<f32>&) const;
	void getPointPosInNthWire(int, const JGeometry::TVec3<f32>&,
	                          JGeometry::TVec3<f32>*) const;
	void getPointPosInWire(const JGeometry::TVec3<f32>&,
	                       JGeometry::TVec3<f32>*) const;
	void perform(u32, JDrama::TGraphics*);
	void entry(TTakeActor*);
	void loadAfter();
	void load(JSUMemoryInputStream&);
	TMapWireManager(const char*);

	static u32 mUpperSurface;
	static u32 mLowerSurface;
};

#endif
