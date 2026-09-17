#ifndef MOVE_BG_MAP_OBJ_FENCE_HPP
#define MOVE_BG_MAP_OBJ_FENCE_HPP

#include <MoveBG/MapObjBase.hpp>
#include <JSystem/JMath.hpp>

class TGraphTracer;
class TMapObjMessenger;
class TRevolvingFenceInner;

/// Plain fence panel. Only reacts to being punched by shaking its model.
/// Every subclass shares the "bamboo" name test that picks the bamboo
/// collision variants.
class TFence : public TMapObjBase {
public:
	TFence(const char* name = "フェンス")
	    : TMapObjBase(name)
	    , mIsBamboo(0)
	{
	}

	virtual ~TFence() { }
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void initMapObj();
	virtual void initMapCollisionData();

public:
	/// Set by initMapObj when the placement name contains "bamboo".
	/* 0x138 */ u8 mIsBamboo;
};

/// The visible frame of a revolving fence. It owns the inner panel that
/// actually turns and forwards the punch shake to it.
class TRevolvingFenceOuter : public TFence {
public:
	TRevolvingFenceOuter(const char* name = "フェンス外側")
	    : TFence(name)
	    , mInner(nullptr)
	{
	}

	virtual ~TRevolvingFenceOuter() { }
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void initMapCollisionData();

public:
	/* 0x13C */ TRevolvingFenceInner* mInner;
};

/// The turning panel of a revolving fence. An upright one (mIsWall) spins
/// about Y under controlWall; one lying flat plays a roll animation instead.
class TRevolvingFenceInner : public TFence {
public:
	TRevolvingFenceInner(const char* name = "フェンス内側")
	    : TFence(name)
	    , mAngle(0.0f)
	    , mIsWall(1)
	{
	}

	virtual ~TRevolvingFenceInner() { }
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void setGroundCollision();
	virtual void control();
	virtual void initMapObj();
	virtual void initMapCollisionData();

	void calcCurrentMtx();
	void controlWall();
	void controlGroundRoof();

	enum {
		STATE_WAIT_FRONT = 1,
		STATE_WAIT_BACK  = 2,
		/// Turning towards the back side, positive direction.
		STATE_TURN_TO_BACK_CW = 3,
		/// Turning back to the front side, positive direction.
		STATE_TURN_TO_FRONT_CW = 4,
		/// Turning towards the back side, negative direction.
		STATE_TURN_TO_BACK_CCW = 5,
		/// Turning back to the front side, negative direction.
		STATE_TURN_TO_FRONT_CCW = 6,
	};

	static f32 mSpeed;

public:
	/// Degrees turned away from the initial yaw.
	/* 0x13C */ f32 mAngle;
	/// True when the panel stands upright, i.e. it spins instead of rolling.
	/* 0x140 */ u8 mIsWall;
};

/// Water-driven revolving fence, vertical variant. A water hit starts it
/// turning; it waits at -90 degrees and then swings back.
class TFenceWater : public TFence {
public:
	TFenceWater(const char* name = "水回転フェンス（垂直）")
	    : TFence(name)
	    , mTurnSpeed(0.0f)
	    , mTurnAngle(0.0f)
	    , mMessenger(nullptr)
	{
	}

	virtual ~TFenceWater() { }
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void control();
	virtual void initMapObj();
	virtual void initMapCollisionData();
	virtual void draw() const;
	virtual void changeStatusToGo();
	virtual void changeStatusToWait();

	void controlRotation();

	enum {
		STATE_WAIT   = 1,
		STATE_GO     = 2,
		STATE_TURNED = 3,
		STATE_BACK   = 4,
	};

	static f32 mWaterAccel;
	static f32 mBackSpeed;
	static int mTurnedWaitTime;

public:
	/// Degrees per frame the panel currently turns.
	/* 0x13C */ f32 mTurnSpeed;
	/// Degrees turned away from the initial rotation, 0 down to -90.
	/* 0x140 */ f32 mTurnAngle;
	/// Hit box that carries the water message, parked one radius away.
	/* 0x144 */ TMapObjMessenger* mMessenger;
};

/// Water-driven revolving fence, horizontal variant. It builds its own
/// matrix because the spin happens about Z rather than Y.
class TFenceWaterH : public TFenceWater {
public:
	TFenceWaterH(const char* name = "水回転フェンス（水平）")
	    : TFenceWater(name)
	{
	}

	virtual ~TFenceWaterH() { }
	virtual void control();
	virtual void changeStatusToGo();
	virtual void changeStatusToWait();
};

/// Fence that rides a rail graph when punched and falls off the world at the
/// end of it, respawning at its initial position.
class TRailFence : public TFence {
public:
	TRailFence(const char* name = "レールフェンス");

	virtual ~TRailFence() { }
	virtual void load(JSUMemoryInputStream&);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void control();
	virtual void initMapCollisionData();

	void falling();
	void goOnRail();

	enum {
		STATE_WAIT    = 1,
		STATE_RUN     = 2,
		STATE_AT_GOAL = 3,
		STATE_FALL    = 4,
	};

	static f32 mFallHeight;
	static int mWaitTime;

public:
	/* 0x13C */ TGraphTracer* mTracer;
	/// Units per frame along the rail.
	/* 0x140 */ f32 mMoveSpeed;
};

#endif
