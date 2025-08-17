#ifndef MOVE_BG_MAP_OBJ_FENCE_HPP
#define MOVE_BG_MAP_OBJ_FENCE_HPP

#include <MoveBG/MapObjBase.hpp>

// TODO: mark virtual methods as such

class TFence : public TMapObjBase {
public:
	BOOL receiveMessage(THitActor* sender, u32 message);
	void initMapCollisionData();
	void initMapObj();
	TFence(const char* name)
	    : TMapObjBase(name)
	    , unk138(0)
	{
	}

public:
	/* 0x138 */ u8 unk138;
};

class TRevolvingFenceOuter : public TFence {
public:
	BOOL receiveMessage(THitActor* sender, u32 message);
	void initMapCollisionData();
	TRevolvingFenceOuter();
};

class TRevolvingFenceInner : public TFence {
public:
	BOOL receiveMessage(THitActor* sender, u32 message);
	void calcCurrentMtx();
	void controlWall();
	void controlGroundRoof();
	void setGroundCollision();
	void control();
	void initMapCollisionData();
	void initMapObj();

	TRevolvingFenceInner(const char* name = "フェンス内側")
	    : TFence(name)
	    , unk13C(0.0f)
	    , unk140(1)
	{
	}

public:
	/* 0x13C */ f32 unk13C;
	/* 0x140 */ u8 unk140;
};

class TFenceWater : public TFence {
public:
	void draw() const;
	BOOL receiveMessage(THitActor* sender, u32 message);
	void changeStatusToGo();
	void changeStatusToWait();
	void controlRotation();
	void control();
	void initMapCollisionData();
	void initMapObj();
	TFenceWater();
};

class TFenceWaterH : public TFenceWater {
public:
	void control();
	void changeStatusToGo();
	void changeStatusToWait();
};

class TRailFence : public TFence {
public:
	BOOL receiveMessage(THitActor* sender, u32 message);
	void falling();
	void goOnRail();
	void control();
	void initMapCollisionData();
	void load(JSUMemoryInputStream&);
	TRailFence(const char*);
};

#endif
