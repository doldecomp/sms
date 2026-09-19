#ifndef MOVEBG_MAPOBJMAMMA_HPP
#define MOVEBG_MAPOBJMAMMA_HPP

#include <MoveBG/MapObjBall.hpp>
#include <MoveBG/MapObjBase.hpp>
#include <MoveBG/MapObjEx.hpp>
#include <MoveBG/MapObjFlag.hpp>
#include <Strategic/HitActor.hpp>
#include <JSystem/JDrama/JDRViewObj.hpp>

class J3DJoint;
class JPABaseEmitter;
class TJointModel;
class TJointObj;
class TSandBase;
class TSandBomb;

/// Hides the parts of the 太陽の塔 building that the mirror camera cannot see.
/// Owns one entry per mirror joint of the "舞台地形" static map model.
class TMammaMirrorMapOperator : public JDrama::TViewObj {
public:
	TMammaMirrorMapOperator(const char* name = "鏡内地形操作");

	virtual ~TMammaMirrorMapOperator() { }
	virtual void loadAfter();
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	// Both UNUSED in the map (0x54 each); reconstructed from perform's two
	// branches, which is the only place the visibility flags are toggled.
	void hide(int i);
	void show(int i);

	enum {
		MIRROR_JOINT_NUM = 8,
		MIRROR_NUM       = 3,
	};

public:
	/* 0x10 */ J3DJoint* mJoints[MIRROR_JOINT_NUM];
	/* 0x30 */ JGeometry::TVec3<f32> mJointCenter[MIRROR_JOINT_NUM];
	/* 0x90 */ f32 mJointRadius[MIRROR_JOINT_NUM];
	/* 0xB0 */ bool mJointHidden[MIRROR_JOINT_NUM];
	/* 0xB8 */ JGeometry::TVec3<f32> mMirrorPos[MIRROR_NUM];
};

/// The watermelon goal ring at the end of the 「お化けスイカ」 mission.
class TGoalWatermelon : public TMapObjBase {
public:
	TGoalWatermelon(const char* name = "スイカゴール");

	virtual ~TGoalWatermelon() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void control();
	virtual void touchActor(THitActor*);

public:
	/* 0x138 */ TMapObjBase* mShine;
	/* 0x13C */ TMapObjBase* mWatermelon;
	/* 0x140 */ JGeometry::TVec3<f32> mShinePosition;
};

/// A watermelon that never rolls; it only rejects water.
class TWatermelonStatic : public TMapObjBase {
public:
	TWatermelonStatic(const char* name = "固定スイカ")
	    : TMapObjBase(name)
	{
	}

	virtual ~TWatermelonStatic() { }
	virtual u32 touchWater(THitActor*);
};

/// Dead in retail: the map lists the class (vtable, destructor and control) but
/// the linker dropped all of it.
/// TODO: the base is either TBigWatermelon or TMapObjBall - both give the
/// 0x1f0-byte vtable and the 0x9c destructor the map records. TBigWatermelon is
/// assumed here because the only override is `control`, which reads as a
/// specialisation of the haunted watermelon's own behaviour.
class TWatermelon : public TBigWatermelon {
public:
	TWatermelon(const char* name = "スイカ")
	    : TBigWatermelon(name)
	{
	}

	virtual ~TWatermelon() { }
	virtual void control();
};

/// The huge sand bird of 「おおすな鳥」; a joint-coin carrier whose blocks are
/// spawned from the joint names of its model.
class TSandBird : public TJointCoin {
public:
	TSandBird(const char* name = "おおすな鳥");

	virtual ~TSandBird() { }
	virtual void control();
	virtual void initMapObj();
	virtual bool nameIsObj(const char*);
	virtual TMapObjBase* makeObjFromJointName(const char*, u16);

public:
	/* 0x148 */ u32 unk148;
	/* 0x14C */ u32 unk14C;
	/* 0x150 */ bool mHelpShown;
	/* 0x151 */ bool mHelpHidden;
};

/// The yacht moored at 太陽の塔; rides the sea wave and carries a flag.
class TMammaYacht : public TMapObjBase {
public:
	// The retail default argument really is 砂の城, the same string
	// TSandCastle uses: the inlined constructor in MarNameRefGen_MapObj.cpp
	// passes the shared @5194 literal.
	TMammaYacht(const char* name = "砂の城")
	    : TMapObjBase(name)
	{
	}

	virtual ~TMammaYacht() { }
	virtual void control();
	virtual void initMapObj();

public:
	/* 0x138 */ TMapObjFlag* mFlag;
};

/// The rotating block inside the 太陽の塔; spraying it turns it over and drives
/// two collision entries plus the building's own joints.
class TMammaBlockRotate : public TMapObjBase {
public:
	TMammaBlockRotate(const char* name = "太陽の塔ブロック");

	virtual ~TMammaBlockRotate() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void control();
	virtual void initMapObj();
	virtual u32 touchWater(THitActor*);

	enum {
		STATE_WAIT      = 0x1,
		STATE_GOING     = 0x2,
		STATE_GOAL_WAIT = 0x3,
		STATE_BACKING   = 0x4,
	};

	static f32 mRotSpeed;
	static f32 mRotReturnSpeed;
	static f32 mRotEnd;
	static f32 mMapGoSpeed;
	static f32 mMapBackSpeed;
	static int mWaitTime;

public:
	/* 0x138 */ TJointModel* mBuilding;
	/* 0x13C */ TJointObj* mDownJointObj;
	/* 0x140 */ TJointObj* mUpJointObj;
	/* 0x144 */ TMapCollisionMove* mDownCollision;
	/* 0x148 */ TMapCollisionMove* mUpCollision;
};

/// The 太陽石 at the top of 太陽の塔. Four mirror models plus the stone itself;
/// each TLeanMirror that finishes its demo lights one more of them.
class TShiningStone : public THitActor {
public:
	TShiningStone(const char* name = "太陽石");

	virtual ~TShiningStone() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	void putOnLight(TLiveActor*);
	// UNUSED in the map (0x98).
	void endDemo();

	int getLightNum() const { return mLightNum; }

	enum {
		MIRROR_NUM = 4,
	};

public:
	/* 0x68 */ MActor** mMirrors;
	/* 0x6C */ MActor* mTop;
	/* 0x70 */ bool mLitS;
	/* 0x71 */ bool mLitM;
	/* 0x72 */ bool mLitL;
	/* 0x73 */ bool mLitAll;
	/* 0x74 */ int mLightNum;
	/* 0x78 */ JPABaseEmitter* mEmitter;
	/* 0x7C */ f32 mEmitterRate;
};

/// One of the three 「ぐらぐら鏡」 mirrors. Mario, enemies and water pushes tilt
/// it; once its hit counter runs out it swings to point at the 太陽石.
class TLeanMirror : public TMapObjBase {
public:
	TLeanMirror(const char* name = "ぐらぐら鏡");

	virtual ~TLeanMirror() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void control();
	virtual void initMapObj();
	virtual u32 getSDLModelFlag() const;
	virtual void draw() const;
	virtual void touchPlayer(THitActor*);
	virtual void touchEnemy(THitActor*);

	void controlShake();
	void controlGoTarget();
	void release();

	// UNUSED in the map (0x100, 0xac and 0x1c). The first two are the shared
	// bodies of controlShake/controlGoTarget and of the four push sites;
	// enemyIsOn is the one-line predicate touchEnemy tests.
	void calcCurrentMtx(Mtx);
	void updateSpeedVec(const JGeometry::TVec3<f32>&, f32);
	bool enemyIsOn() const;

	enum {
		STATE_SHAKE     = 0x1,
		STATE_GO_TARGET = 0x2,
		STATE_LIGHT     = 0x3,
		STATE_DONE      = 0x4,
	};

	static int mGoTargetTime;
	static int mDemoWaitTime;
	static int mDemoLightTime;

public:
	/* 0x138 */ f32 mSize;
	/* 0x13C */ f32 mDefaultSize;
	/* 0x140 */ JGeometry::TVec3<f32> unk140;
	/* 0x14C */ JGeometry::TVec3<f32> mSpeed;
	/* 0x158 */ f32 mSpeedRate;
	/* 0x15C */ f32 mSpeedDownRate;
	/* 0x160 */ f32 mMarioPower;
	/* 0x164 */ f32 mWaterPower;
	/* 0x168 */ f32 mWaterJetPower;
	/* 0x16C */ f32 mEnemyPower;
	/* 0x170 */ f32 mFruitPower;
	/* 0x174 */ f32 mLeanLimit;
	/* 0x178 */ f32 mReboundRate;
	/* 0x17C */ TShiningStone* mShiningStone;
	/* 0x180 */ JGeometry::TVec3<f32> mToStone;
	/* 0x18C */ JGeometry::TVec3<f32> mRotAxis;
	/* 0x198 */ f32 mRotSpeed;
	/* 0x19C */ int mHitNum;
	/* 0x1A0 */ JGeometry::TVec3<f32> mMarioMovePos;
	/* 0x1AC */ bool mBgmStarted;
	/* 0x1AE */ s16 unk1AE;
};

/// Base of the sand objects that grow while sprayed and wither afterwards.
class TSandBase : public TMapObjBase {
public:
	TSandBase(const char* name);

	virtual ~TSandBase() { }
	virtual void grow() = 0;
	virtual bool withering();

	// UNUSED in the map (0x34).
	bool isDown() const;

	enum {
		STATE_GROWN    = 0x1,
		STATE_WITHER   = 0x2,
		STATE_REVIVING = 0x3,
		STATE_GROWING  = 0x4,
		STATE_REVIVED  = 0x5,
	};

	static int mWitherTime;
	static f32 mScaleMin;

public:
	/* 0x138 */ f32 mGrowSpeed;
	/* 0x13C */ f32 mWitherSpeed;
	/* 0x140 */ int mReviveTime;
	// The object that reacts to the water jet on this one's behalf. It is a
	// 「すなやまの芽」/「すなやま爆弾」 for the growing objects and the
	// 「砂の城爆発の絵」 picture for TSandCastle, which shares the same
	// owner pointer at 0x138 and the same `appear` slot.
	/* 0x144 */ TSandBomb* mTrigger;
};

/// The bud that grows out of the sand when sprayed.
class TSandLeafBase : public TSandBase {
public:
	TSandLeafBase(const char* name = "すなやまの芽の土台")
	    : TSandBase(name)
	{
	}

	virtual ~TSandLeafBase() { }
	virtual void control();
	virtual void initMapObj();
	virtual void grow();
};

/// The sand bomb pyramid family: grows, waits, explodes and throws Mario.
class TSandBombBase : public TSandBase {
public:
	TSandBombBase(const char* name = "すなやま爆弾の土台");

	virtual ~TSandBombBase() { }
	virtual void loadAfter();
	virtual void control();
	virtual void initMapObj();
	virtual void grow();
	virtual void waitBeforeExplode();
	virtual void explode();
	virtual void exploding();
	virtual void expanded();
	virtual void withered();
	virtual TSandBomb* findTriggerActor();

	enum {
		STATE_FIRING   = 0x5,
		STATE_WAIT_BOM = 0x6,
		STATE_EXPLODE  = 0x7,
		STATE_EXPANDED = 0x8,
	};

	static f32 mFiringFrameSpeed;
	static f32 mFiringFrameDownSpeed;
	static f32 mExplodeFrameSpeed;
	static f32 mMarioJumpRate;
	static int mExlodingRumbleTime;

public:
	/* 0x148 */ int mExplodeWaitTime;
	/* 0x14C */ f32 mExplodeEffectScale;
	/* 0x150 */ f32 mExpandFrameSpeed;
	/* 0x154 */ f32 mMarioJumpRange;
};

/// The 砂の城 of 「砂の城爆発の絵」: a sand bomb whose trigger is a picture and
/// whose explosion fires a demo camera and opens the stage exit.
class TSandCastle : public TSandBombBase {
public:
	TSandCastle(const char* name = "砂の城");

	virtual ~TSandCastle() { }
	virtual void loadAfter();
	virtual void calcRootMatrix();
	virtual void initMapObj();
	virtual bool withering();
	virtual void waitBeforeExplode();
	virtual void explode();
	virtual void expanded();
	virtual TSandBomb* findTriggerActor();

	static f32 mCollisionRate;

public:
	/* 0x158 */ TMapObjBase* mChangeStage;
	/* 0x15C */ bool mDemoFired;
};

/// The little sand leaf that takes the water jet for its owner.
class TSandLeaf : public TMapObjBase {
public:
	TSandLeaf(const char* name = "すなやまの芽")
	    : TMapObjBase(name)
	    , mOwner(nullptr)
	{
	}

	// The map keeps the surviving weak copy in MapObjManager.cpp and marks
	// this unit's as an unreferenced duplicate, but the vtable lives here.
	virtual ~TSandLeaf() { }
	virtual void control();
	virtual u32 touchWater(THitActor*);

public:
	/* 0x138 */ TSandBase* mOwner;
};

/// The sand bomb bud: same role as TSandLeaf, plus the firing animation.
class TSandBomb : public TSandLeaf {
public:
	TSandBomb(const char* name = "すなやま爆弾")
	    : TSandLeaf(name)
	    , unk13C(0)
	    , unk140(false)
	{
	}

	virtual ~TSandBomb() { }
	virtual void makeObjAppeared();
	virtual void initMapObj();
	virtual u32 getSDLModelFlag() const;
	virtual u32 touchWater(THitActor*);

public:
	/* 0x13C */ int unk13C;
	// Cleared by TSandBombBase::control on every frame in which nothing
	// collided with this object.
	/* 0x140 */ bool unk140;
};

/// The sand egg of 「すなのたまご」; only its shadow/model flag differs from the
/// plain map object.
class TSandEgg : public TMapObjBase {
public:
	TSandEgg(const char* name = "すなのたまご")
	    : TMapObjBase(name)
	{
	}

	virtual ~TSandEgg() { }
	virtual u32 getSDLModelFlag() const;
};

#endif
