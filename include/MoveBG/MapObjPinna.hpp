#ifndef MOVE_BG_MAP_OBJ_PINNA_HPP
#define MOVE_BG_MAP_OBJ_PINNA_HPP

#include <MoveBG/MapObjBase.hpp>
#include <MoveBG/MapObjGeneral.hpp>
#include <MoveBG/MapObjTown.hpp>
#include <JSystem/JMath.hpp>
#include <MarioUtil/MathUtil.hpp>

class TCoin;
class TMapCollisionMove;
class J3DJoint;

/// One gondola-carrying big wheel; the gondolas themselves are separate
/// "FerrisGondola" objects whose matrices this object copies out of its own
/// joints every frame.
class TFerrisWheel : public TMapObjBase {
public:
	virtual ~TFerrisWheel() { }

	/// Demo-camera callback. Fired with the wheel address in @p param_1 (the
	/// pointer the camera was started with) once the "back to normal" camera
	/// reaches its end.
	static s32 becomeCalmlyCallback(u32 param_1, u32 param_2);

	virtual void control();
	virtual void initMapObj();
	TFerrisWheel(const char* name = "観覧車");

	int getGondolaNum() const { return mGondolaNum; }

public:
	/* 0x138 */ int mGondolaNum;
	/* 0x13C */ TMapObjBase** mGondolas;
	/* 0x140 */ f32 mAnmRate;
};

/// A swing that travels along a sine in X and a cosine in Y, i.e. around a
/// circle of radius mSwingRadius centred above its initial position.
class THorizontalViking : public TMapObjBase {
public:
	virtual ~THorizontalViking() { }

	void updateTrans();
	void moveNormal();
	virtual void control();
	virtual void reset();
	virtual void initMapObj();
	THorizontalViking(const char*);

public:
	enum {
		/// Swinging towards negative angles.
		STATE_SWING_DOWN = 0x1,
		/// Swinging towards positive angles.
		STATE_SWING_UP = 0x2,
		/// Rolling states, see TViking::roll().
		STATE_ROLL_DOWN_FWD  = 0x1,
		STATE_ROLL_UP_FWD    = 0x2,
		STATE_ROLL_DOWN_BACK = 0x3,
		STATE_ROLL_UP_BACK   = 0x4,
	};

	/* 0x138 */ f32 mSwingRadius;
	/* 0x13C */ f32 mSwingAccel;
	/* 0x140 */ f32 mSwingSpeedInit;
	/* 0x144 */ f32 mSwingSpeed;
	/* 0x148 */ f32 mSwingAngle;
};

/// The Pinna Park pirate ship. Same geometry as THorizontalViking, but it can
/// also loop the whole way around (mMode == 1).
class TViking : public THorizontalViking {
public:
	virtual ~TViking() { }

	void roll();
	virtual void control();
	virtual void reset();
	virtual void loadAfter();
	virtual void initMapObj();

	// The constructor stores TViking's vtable before initialising the four
	// fields below, so they belong to this class and not to
	// THorizontalViking even though nothing but TViking reads them.
	TViking(const char* name = "バイキング");

public:
	enum {
		/// Plain back-and-forth swing (THorizontalViking behaviour).
		MODE_SWING = 0x0,
		/// Full loops, see roll().
		MODE_ROLL = 0x1,
	};

	/* 0x14C */ int mMode;
	/* 0x150 */ f32 mSwingSpeedMin;
	/* 0x154 */ f32 mSpeedGainFwd;
	/* 0x158 */ f32 mSpeedGainBack;
};

/// One of the six shells of the shell cup ride. Not a TMapObjBase: it is a
/// bare hit actor living inside TShellCup's array, driven by its owner.
class TShellCup;

class TPinnaShell : public THitActor {
public:
	virtual ~TPinnaShell() { }

	void opened();
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	void control();

	/// fabricated: the map has no symbol for it, but TShellCup::calcAfter()
	/// loops over the shells with the shell as `this` and reaches
	/// MsMtxSetRotX one inline level down, which is what keeps that helper a
	/// call there and an expansion in control().
	void calcJointMtx()
	{
		Mtx mtx;
		MsMtxSetRotX(mtx, mRotX);
		TMapObjBase::concatOnlyRotFromRight(mJointMtx, mtx, mJointMtx);
	}
	TPinnaShell(const char*);
	// TODO: 99.9%. Retail spills `this` at 0xc(r1), ours at 8: one 4-byte
	// item below it. A defaulted `int` parameter lands it (so it is an
	// argument-shaped temporary) but the map fixes the Fv signature. Tried:
	// a name-returning static/free inline (with and without a named local or
	// a pass-through parameter), a trailing `return;`, a by-value f32 level
	// around initHitActor, casts on the enum and the 0x80000000 argument.
	TPinnaShell()
	    : THitActor("シェル")
	    , mState(STATE_CLOSED)
	    , mRotX(0.0f)
	    , mCloseSpeed(0.0f)
	    , mJointMtx(nullptr)
	    , mJoint(nullptr)
	    , mTimer(0)
	    , mContent(nullptr)
	    , mCollision(nullptr)
	    , mDamageObj(nullptr)
	    , mOwner(nullptr)
	{
		initHitActor(0x4000013A, 1, 0x80000000, 250.0f, 400.0f, 250.0f,
		             200.0f);
	}

public:
	enum {
		/// Shut, drifting back to zero.
		STATE_CLOSED = 0x0,
		/// Opening after being hit by water.
		STATE_OPENING = 0x1,
		/// Fully open, waiting out mTimer.
		STATE_OPEN = 0x2,
		/// Snapping shut again.
		STATE_CLOSING = 0x3,
	};

	/* 0x68 */ int mState;
	/// Lid rotation about X in degrees; negative is open.
	/* 0x6C */ f32 mRotX;
	/* 0x70 */ f32 mCloseSpeed;
	/* 0x74 */ MtxPtr mJointMtx;
	/* 0x78 */ J3DJoint* mJoint;
	/* 0x7C */ int mTimer;
	/// The coin (or blue coin) parked inside this shell, if any.
	/* 0x80 */ TMapObjBase* mContent;
	/* 0x84 */ TMapCollisionMove* mCollision;
	/* 0x88 */ TDamageObj* mDamageObj;
	/* 0x8C */ TShellCup* mOwner;
};

/// The six-shell carousel in Pinna Park.
class TShellCup : public TMapObjBase {
public:
	virtual ~TShellCup() { }

	virtual void control();
	void attachCoin(TCoin*, int);
	void calcAfter();
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual void loadAfter();
	virtual void initMapObj();
	TShellCup(const char* name = "シェルカップ");

	TPinnaShell* getShell(int i) { return &mShells[i]; }

public:
	static f32 mOpenRotMax;
	static f32 mAutoOpenRot;
	static f32 mShellDamageRot;
	static f32 mWaterOpenAccel;
	static f32 mCloseAccel;
	static int mWaitTimeToClose;

	/* 0x138 */ TPinnaShell mShells[6];
	/* 0x498 */ TMapObjBase* mBlueCoin;
	/* 0x49C */ TCoin* mCoin0;
	/* 0x4A0 */ TCoin* mCoin1;
};

class TMerryPole;

/// The merry-go-round: two yoshi eggs, nine poles with their own collision,
/// and a warp object, all bound to joints of this model.
class TMerrygoround : public TMapObjBase {
public:
	virtual ~TMerrygoround() { }

	virtual void control();
	virtual void draw() const;
	virtual void initMapObj();
	TMerrygoround(const char* name = "メリーゴーランド");

public:
	static f32 mRotSpeed;

	/* 0x138 */ TMapObjBase* mEggs[2];
	/* 0x140 */ u16 mEggJoints[2];
	/* 0x144 */ TMerryPole* mPoles[9];
	/* 0x168 */ TMapCollisionMove* mPoleCollisions[9];
	/* 0x18C */ u16 mPoleJoints[9];
	/* 0x1A0 */ TMapObjChangeStage* mWarp;
	/* 0x1A4 */ u16 mWarpJoint;
};

class TChangeStageMerrygoround : public TMapObjChangeStage {
public:
	virtual ~TChangeStageMerrygoround() { }
	virtual void touchPlayer(THitActor*);
	virtual void calc();

	TChangeStageMerrygoround()
	    : TMapObjChangeStage("ステージ切り替え（メリーゴーランド用）")
	    , unk13C(0)
	{
	}

public:
	/* 0x13C */ u8 unk13C;
};

class TBalloonKoopaJr : public TMapObjGeneral {
public:
	virtual ~TBalloonKoopaJr() { }
	virtual void touchActor(THitActor*);
	virtual void kill();
	virtual void load(JSUMemoryInputStream&);

	// The map has no out-of-line copy of any of the remaining constructors in
	// this file, so they were defined in the class and inlined wherever the
	// object factory builds them.
	TBalloonKoopaJr(const char* name = "風船（クッパＪｒ）")
	    : TMapObjGeneral(name)
	{
		mCenterPos.x = mCenterPos.y = mCenterPos.z = 0.0f;
	}

public:
	/// World position of the "center" joint, where the pop effects are emitted.
	/* 0x148 */ JGeometry::TVec3<f32> mCenterPos;
};

class TPinnaEntrance : public TMapObjBase {
public:
	virtual ~TPinnaEntrance() { }
	virtual void loadAfter();

	TPinnaEntrance(const char* name = "ピンナ入り口")
	    : TMapObjBase(name)
	{
	}
};

class TWaterRecoverObj : public TMapObjBase {
public:
	virtual ~TWaterRecoverObj() { }
	virtual void touchPlayer(THitActor*);

	TWaterRecoverObj(const char* name = "水回復オブジェ")
	    : TMapObjBase(name)
	{
	}
};

/// The manta boss's gate guardian net. Sleeps until the gate it stands on is
/// broken, then flies off and hands out the shine.
class TAmiKing : public TMapObjBase {
public:
	virtual ~TAmiKing() { }
	virtual u32 touchWater(THitActor*) { return 1; }
	virtual void loadAfter();
	virtual void initMapObj();
	virtual void moveObject();
	virtual void calc();
	virtual void bind();
	virtual void touchPlayer(THitActor*);

	TAmiKing(const char* name = "アミキング")
	    : TMapObjBase(name)
	    , mFlying(false)
	{
	}

public:
	/* 0x138 */ u8 mFlying;
	/// Position of the effect emitted ahead of the net while it is asleep.
	/* 0x13C */ JGeometry::TVec3<f32> mEffectPos;
};

/// The roller-coaster. Its own model rides a separate "CoasterRail" MActor
/// whose animated joint matrix drives this object's position.
class TPinnaCoaster : public TMapObjBase {
public:
	virtual ~TPinnaCoaster() { }

	virtual void control();
	virtual void initMapObj();
	TPinnaCoaster(const char* name = "コースター");

public:
	/* 0x138 */ MActor* mRail;
	/* 0x13C */ u32 unk13C;
	/* 0x140 */ JGeometry::TVec3<f32> mPrevPos;
};

class TMerryPole : public TMapObjBase {
public:
	virtual Mtx* getRootJointMtx() const { return (Mtx*)unk138.mMtx; }

	TMerryPole()
	    : TMapObjBase("メリーゴーランド用ポール")
	{
		unk138.identity();
	}

public:
	/* 0x138 */ TPosition3f unk138;
};

#endif
