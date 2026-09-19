#ifndef MOVE_BG_MAP_OBJ_MONTE_HPP
#define MOVE_BG_MAP_OBJ_MONTE_HPP

#include <MoveBG/MapObjBase.hpp>
#include <MoveBG/MapObjBlock.hpp>
#include <JSystem/JGeometry/JGVec2.hpp>

class JAISound;
class TFluffManager;
class THangingBridge;

/// The root of the 「モンテの木」 that Mario climbs; it only sizes its own
/// collision sphere to its scale.
class TMapObjMonteRoot : public TMapObjBase {
public:
	TMapObjMonteRoot(const char* name = "根っこ")
	    : TMapObjBase(name)
	{
	}

	virtual ~TMapObjMonteRoot() { }
	virtual void initMapObj();
};

/// The bouncy mushroom of 「ジャンプきのこ」. Its collision data id comes from
/// the map data and any message replays the squash animation.
class TJumpMushroom : public TMapObjBase {
public:
	TJumpMushroom(const char* name = "ジャンプきのこ")
	    : TMapObjBase(name)
	{
	}

	virtual ~TJumpMushroom() { }
	virtual void load(JSUMemoryInputStream&);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
};

/// One plank of the hanging bridge. Planks are chained two deep in both
/// directions so that Mario's weight also pulls the neighbours down.
class THangingBridgeBoard : public TLeanBlock {
public:
	THangingBridgeBoard(const char* name);

	virtual ~THangingBridgeBoard() { }
	virtual void setGroundCollision();
	virtual void control();
	virtual void initMapObj();
	virtual void calcDefaultMtx();

	void drawOneRope(const JGeometry::TVec3<f32>&) const;

	// UNUSED in the map (0x6c, 0x10 and 0x94); all three are the shapes
	// THangingBridge::perform and control expand.
	void drawRopes() const;
	void push(f32 accel);
	void pushNeighbor(f32 accel);

	static f32 mMarioAccelY;
	static f32 mMarioHipDropAccelY;
	static f32 mReturnAccelRate;
	static f32 mSpeedDownRate;
	static f32 mRopeWidthX;
	static f32 mRopeWidthZ;
	static f32 mTexPosRate;

public:
	/* 0x194 */ THangingBridgeBoard* mPrev;
	/* 0x198 */ THangingBridgeBoard* mNext;
	/* 0x19C */ THangingBridgeBoard* mPrev2;
	/* 0x1A0 */ THangingBridgeBoard* mNext2;
	/* 0x1A4 */ JGeometry::TVec3<f32> mRopeTop[2];
	/* 0x1BC */ THangingBridge* mBridge;
};

/// The 「つり橋」 itself: it owns the planks, the sag table for the ropes
/// between them, and all of the rope drawing.
class THangingBridge : public JDrama::TViewObj {
public:
	THangingBridge(const char* name = "つり橋");

	virtual ~THangingBridge() { }
	virtual void loadAfter();
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	void initDraw() const;
	void drawRopeBetweenBoards(f32 yOffset, int divide) const;
	void drawUpper(const JGeometry::TVec3<f32>&, const JGeometry::TVec3<f32>&,
	               const JGeometry::TVec2<f32>&, int) const;
	void drawLowerPlus(const JGeometry::TVec3<f32>&,
	                   const JGeometry::TVec3<f32>&,
	                   const JGeometry::TVec2<f32>&, int) const;
	void drawLowerMinus(const JGeometry::TVec3<f32>&,
	                    const JGeometry::TVec3<f32>&,
	                    const JGeometry::TVec2<f32>&, int) const;

	// UNUSED in the map (0x120 and 0x10).
	void initMonte();
	void setDrawPos(int, f32, JGeometry::TVec3<f32>*) const;

	static f32 mRopeWidthBetweenBoards;
	static f32 mRopeWidthBetweenBoardsY;
	static int mPointNumBetweenBoards;
	static f32 mBetweenBoardsTexPosRate;
	static f32 mRopeHeight;

public:
	/* 0x10 */ int mBoardNum;
	/* 0x14 */ THangingBridgeBoard** mBoards;
	/* 0x18 */ JGeometry::TVec3<f32> mStart;
	/* 0x24 */ JGeometry::TVec3<f32> mEnd;
	/* 0x30 */ JGeometry::TVec2<f32> mSideDir;
	/* 0x38 */ f32* mRopeSag;
	/* 0x3C */ f32 mRopeOffset;
	/* 0x40 */ f32 mNeighborRate;
	/* 0x44 */ f32 mNeighbor2Rate;
};

/// The single plank on two ropes of 「つり橋」 in Monte village; the water jet
/// pushes it and it swings back on its own.
class TSwingBoard : public TMapObjBase {
public:
	TSwingBoard(const char* name = "つり橋");

	virtual ~TSwingBoard() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void control();
	virtual void draw() const;

	void initDraw() const;
	void drawOneRope(const JGeometry::TVec3<f32>&,
	                 const JGeometry::TVec3<f32>&) const;

	// UNUSED in the map (0xa8): the water-jet push at the head of control.
	void swing();

	static f32 mBoardWidth;
	static f32 mRopeWidthX;
	static f32 mRopeWidthZ;
	static f32 mTexPosRate;
	static f32 mReturnAccelRate;
	static f32 mSpeedDownRate;

public:
	/* 0x138 */ f32 mRopeLength;
	/* 0x13C */ f32 mAngle;
	/* 0x140 */ f32 mAccelRate;
	/* 0x144 */ f32 mAngleSpeed;
	/* 0x148 */ f32 mAngleSpeedMax;
	/* 0x14C */ JGeometry::SMatrix34C<f32> mBaseMtx;
	/* 0x17C */ JGeometry::TVec3<f32> mAnchor;
	/* 0x188 */ JAISound* mSound;
};

/// The goal flag of the 「モンテの村」 race; touching it sets the clear flag.
class TGoalFlag : public TMapObjBase {
public:
	TGoalFlag(const char* name = "ゴールフラグ")
	    : TMapObjBase(name)
	{
	}

	virtual ~TGoalFlag() { }
	virtual f32 getRadiusAtY(f32) const { return 20.0f; }
	virtual void initMapObj();
	virtual void touchActor(THitActor*);
};

/// One dandelion seed. Falls, swings around its base point and is carried by
/// the manager's wind.
class TFluff : public TMapObjBase {
public:
	TFluff(const char* name);

	virtual ~TFluff() { }
	virtual f32 getRadiusAtY(f32) const { return 20.0f; }
	virtual void control();
	virtual void kill();
	virtual void appear();
	virtual void initMapObj();
	virtual u32 touchWater(THitActor*);

	void move();

	static f32 mScaleUpSpeed;
	static f32 mScaleDownSpeed;

	enum {
		STATE_FLYING     = 0x1,
		STATE_APPEARING  = 0x2,
		STATE_VANISHING  = 0x3,
		STATE_WAIT_APPEAR = 0x4,
	};

public:
	/* 0x138 */ f32 mSwingRadius;
	/* 0x13C */ f32 mFallSpeed;
	/* 0x140 */ f32 mSwingSin;
	/* 0x144 */ f32 mSwingCos;
	/* 0x148 */ f32 mSwingAngle;
	/* 0x14C */ f32 mSwingAngleSpeed;
	/* 0x150 */ f32 mWindRate;
	/* 0x154 */ JGeometry::TVec3<f32> mDrift;
	/* 0x160 */ f32 mWaterPushRate;
	/* 0x164 */ f32 mSpeedDownRate;
	/* 0x168 */ TFluffManager* mManager;
	/* 0x16C */ bool mIsRideable;
};

/// Spawns and recycles the seeds of 「特別な綿毛」 and drives the wind that
/// carries them.
class TFluffManager : public TMapObjBase {
public:
	TFluffManager(const char* name = "特別な綿毛");

	virtual ~TFluffManager() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void control();

	// UNUSED in the map (0x118, 0x40, 0x10c, 0x6c, 0x64, 0x64).
	void findNextFluff();
	void registerNextFluff(TFluff*);
	void setUpNextFluff();
	TFluff* newFluff(const char*);
	f32 getRandomX() const;
	f32 getRandomZ() const;

	static f32 mWindMin;

	enum {
		STATE_WAIT = 0x1,
		STATE_BLOW = 0x2,
		STATE_CALM = 0x3,
	};

public:
	/* 0x138 */ f32 mRangeX;
	/* 0x13C */ f32 mRangeZ;
	/* 0x140 */ f32 mBlowHeight;
	/* 0x144 */ s32 mBlowTime;
	/* 0x148 */ JGeometry::TVec3<f32> mWind;
	/* 0x154 */ f32 mWindDownRate;
	/* 0x158 */ TFluff* mRideFluff;
	/* 0x15C */ TFluff* mNextFluff;
	/* 0x160 */ int mFluffNum;
	/* 0x164 */ int mFluffMax;
	/* 0x168 */ TFluff** mFluffs;
};

#endif
