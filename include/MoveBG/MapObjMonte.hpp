#ifndef MOVE_BG_MAP_OBJ_MONTE_HPP
#define MOVE_BG_MAP_OBJ_MONTE_HPP

#include <MoveBG/MapObjBase.hpp>
#include <MoveBG/MapObjBlock.hpp>

// TODO: mark virtual methods as such

class TMapObjMonteRoot : public TMapObjBase {
public:
	void initMapObj();
	TMapObjMonteRoot();
};

class TJumpMushroom : public TMapObjBase {
public:
	u32 receiveMessage(THitActor*, unsigned long);
	void load(JSUMemoryInputStream&);
	TJumpMushroom();
};

class THangingBridgeBoard : public TLeanBlock {
public:
	void drawOneRope(const JGeometry::TVec3<f32>&) const;
	void drawRopes() const;
	void push(f32);
	void pushNeighbor(f32);
	void control();
	void calcDefaultMtx();
	void setGroundCollision();
	void initMapObj();
	THangingBridgeBoard(const char*);
};

class THangingBridge : public JDrama::TViewObj {
public:
	void drawLowerMinus(const JGeometry::TVec3<f32>&,
	                    const JGeometry::TVec3<f32>&,
	                    const JGeometry::TVec2<f32>&, int) const;
	void drawLowerPlus(const JGeometry::TVec3<f32>&,
	                   const JGeometry::TVec3<f32>&,
	                   const JGeometry::TVec2<f32>&, int) const;
	void drawUpper(const JGeometry::TVec3<f32>&, const JGeometry::TVec3<f32>&,
	               const JGeometry::TVec2<f32>&, int) const;
	void setDrawPos(int, f32, JGeometry::TVec3<f32>*) const;
	void drawRopeBetweenBoards(f32, int) const;
	void initDraw() const;
	void perform(unsigned long, JDrama::TGraphics*);
	void initMonte();
	void loadAfter();
	THangingBridge(const char*);
};

class TSwingBoard : public TMapObjBase {
public:
	void drawOneRope(const JGeometry::TVec3<f32>&,
	                 const JGeometry::TVec3<f32>&) const;
	void initDraw() const;
	void draw() const;
	void swing();
	void control();
	void load(JSUMemoryInputStream&);
	TSwingBoard(const char*);
};

class TGoalFlag : public TMapObjBase {
public:
	void getRadiusAtY(f32) const;
	void touchActor(THitActor*);
	void initMapObj();
	TGoalFlag();
};

class TFluff : public TMapObjBase {
public:
	void getRadiusAtY(f32) const;
	u32 touchWater(THitActor*);
	void move();
	void kill();
	void control();
	void appear();
	void initMapObj();
	TFluff(const char*);
};

class TFluffManager : public TMapObjBase {
public:
	void findNextFluff();
	void control();
	void registerNextFluff(TFluff*);
	void setUpNextFluff();
	void newFluff(const char*);
	void getRandomX() const;
	void getRandomZ() const;
	void loadAfter();
	void load(JSUMemoryInputStream&);
	TFluffManager(const char*);
};

#endif
