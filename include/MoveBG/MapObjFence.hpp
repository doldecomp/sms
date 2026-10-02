#ifndef MOVE_BG_MAP_OBJ_FENCE_HPP
#define MOVE_BG_MAP_OBJ_FENCE_HPP

#include <MoveBG/MapObjBase.hpp>

class TGraphTracer;
class TMapObjMessenger;

class TFence : public TMapObjBase {
public:
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void initMapObj();
	virtual void initMapCollisionData();
	TFence(const char* name = "フェンス")
	    : TMapObjBase(name)
	    , unk138(0)
	{
	}

public:
	/* 0x138 */ u8 unk138;
};

class TRevolvingFenceOuter : public TFence {
public:
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void initMapCollisionData();
	TRevolvingFenceOuter(const char* name = "フェンス外側")
	    : TFence(name)
	    , unk13C(nullptr)
	{
	}

public:
	/* 0x13C */ TMapObjBase* unk13C;
};

class TRevolvingFenceInner : public TFence {
public:
	static f32 mSpeed;

	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void setGroundCollision();
	virtual void control();
	virtual void initMapObj();
	virtual void initMapCollisionData();

	void calcCurrentMtx();
	void controlWall();
	void controlGroundRoof();

	TRevolvingFenceInner(const char* name = "フェンス内側")
	    : TFence(name)
	    , unk13C(0.0f)
	    , unk140(1)
	{
	}

	enum {
		STATE_UNK2 = 0x2,
		STATE_UNK3 = 0x3,
		STATE_UNK4 = 0x4,
		STATE_UNK5 = 0x5,
		STATE_UNK6 = 0x6,
	};

public:
	/* 0x13C */ f32 unk13C;
	/* 0x140 */ u8 unk140;
};

class TFenceWater : public TFence {
public:
	static f32 mWaterAccel;
	static f32 mBackSpeed;
	static int mTurnedWaitTime;

	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void control();
	virtual void initMapObj();
	virtual void initMapCollisionData();
	virtual void draw() const;
	virtual void changeStatusToGo();
	virtual void changeStatusToWait();
	void controlRotation();
	TFenceWater(const char* name = "水回転フェンス（垂直）")
	    : TFence(name)
	    , unk13C(0.0f)
	    , unk140(0.0f)
	    , unk144(nullptr)
	{
	}

	enum {
		STATE_UNK2 = 0x2,
		STATE_UNK3 = 0x3,
		STATE_UNK4 = 0x4,
	};

public:
	/* 0x13C */ f32 unk13C;
	/* 0x140 */ f32 unk140;
	/* 0x144 */ TMapObjMessenger* unk144;
};

class TFenceWaterH : public TFenceWater {
public:
	virtual void control();
	virtual void changeStatusToGo();
	virtual void changeStatusToWait();
	TFenceWaterH(const char* name = "水回転フェンス（水平）")
	    : TFenceWater(name)
	{
	}
};

class TRailFence : public TFence {
public:
	virtual void load(JSUMemoryInputStream&);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void control();
	virtual void initMapCollisionData();

	void falling();
	void goOnRail();
	TRailFence(const char* name = "レールフェンス");

	static f32 mFallHeight;
	static int mWaitTime;

	enum {
		STATE_UNK2 = 0x2,
		STATE_UNK3 = 0x3,
		STATE_UNK4 = 0x4,
	};

public:
	/* 0x13C */ TGraphTracer* unk13C;
	/* 0x140 */ f32 unk140;
};

#endif
