#ifndef MOVE_BG_MAP_OBJ_FENCE_HPP
#define MOVE_BG_MAP_OBJ_FENCE_HPP

#include <MoveBG/MapObjBase.hpp>

// TODO: mark virtual methods as such

class TFence : public TMapObjBase {
public:
	u32 receiveMessage(THitActor*, u32);
	void initMapCollisionData();
	void initMapObj();
	TFence();
};

class TRevolvingFenceOuter : public TFence {
public:
	u32 receiveMessage(THitActor*, u32);
	void initMapCollisionData();
};

class TRevolvingFenceInner : public TFence {
public:
	u32 receiveMessage(THitActor*, u32);
	void calcCurrentMtx();
	void controlWall();
	void controlGroundRoof();
	void setGroundCollision();
	void control();
	void initMapCollisionData();
	void initMapObj();
	TRevolvingFenceInner();
};

class TFenceWater : public TFence {
public:
	void draw() const;
	u32 receiveMessage(THitActor*, u32);
	void changeStatusToGo();
	void changeStatusToWait();
	void controlRotation();
	void control();
	void initMapCollisionData();
	void initMapObj();
};

class TFenceWaterH : public TFenceWater {
public:
	void control();
	void changeStatusToGo();
	void changeStatusToWait();
};

class TRailFence : public TFence {
public:
	u32 receiveMessage(THitActor*, u32);
	void falling();
	void goOnRail();
	void control();
	void initMapCollisionData();
	void load(JSUMemoryInputStream&);
	TRailFence(const char*);
};

#endif
