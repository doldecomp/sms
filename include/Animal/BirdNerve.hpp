#ifndef ANIMAL_BIRDNERVE_HPP
#define ANIMAL_BIRDNERVE_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Strategic/Nerve.hpp>
#include <System/ParamInst.hpp>
#include <dolphin/gx.h>

class TLiveActor;
class TMapObjBase;
class TWireBinder;

// The seagulls ("(幸せの青い)鳥", the happy blue bird) that wander the graph
// paths of Delfino. Each one carries an item it turns into when squirted --
// the event id read by load() decides which, and the item's actor type also
// picks the body tint out of cColorTable.
//
// Names and defaults are the ones PARAM_INIT stringified into .rodata and the
// constants the constructor stores into each TParamRT; the .prm is
// /Animal/bird.prm.
class TAnimalBirdParams : public TSpineEnemyParams {
public:
	TAnimalBirdParams(const char* prm);

	/* 0x0A8 */ TParamRT<f32> mMarchSpeed;
	/* 0x0BC */ TParamRT<f32> mTurnSpeed;
	/* 0x0D0 */ TParamRT<s32> mReturnTimer;
	/* 0x0E4 */ TParamRT<f32> mSearchLength;
	/* 0x0F8 */ TParamRT<f32> mSearchHeight;
	/* 0x10C */ TParamRT<f32> mSearchAware;
	/* 0x120 */ TParamRT<f32> mSearchAngle;
	/* 0x134 */ TParamRT<s32> mActionTimer;
	/* 0x148 */ TParamRT<s32> mWaterproofTimerMax;
	/* 0x15C */ TParamRT<s32> mFloatingTimerMax;
	/* 0x170 */ TParamRT<f32> mLandingGravityY;
	/* 0x184 */ TParamRT<f32> mLandingTorqueY;
	/* 0x198 */ TParamRT<f32> mWalkingTorqueY;
	/* 0x1AC */ TParamRT<f32> mWalkingSpeed;
	/* 0x1C0 */ TParamRT<s32> mWalkTimer;
	/* 0x1D4 */ TParamRT<f32> mLandingFric;
	/* 0x1E8 */ TParamRT<s32> mActionTimerAdd;
	/* 0x1FC */ TParamRT<f32> mWaterPowerY;
};

class TAnimalBird : public TSpineEnemy {
public:
	TAnimalBird(const char* name = "(幸せの青い)鳥");

	virtual ~TAnimalBird() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void bind();
	virtual void moveObject();
	virtual const char** getBasNameTable() const;

	void initTevColor(const GXColorS10*);
	void initCollision();
	void initParams();

	f32 getFootGroundHeight();
	f32 getWaterPowerY() const;
	f32 getWaterDamageRate() const;
	f32 getMyMarchSpeed() const;
	void setBckAnm(int index);
	void setParamsOnLanding();
	void setParamsOnFloating();
	void setGoalToComeback();
	void doGotoRandomNextGraphNode();
	bool doLanding(bool takeoff);
	void doWalk();
	void doFlyToCurPathNode();
	void doDropCoin();
	bool isFlying() const;
	bool isChanged() const;
	bool isCheckWithWireBinder() const;
	bool isGroundShaken() const;
	bool isChangeToItem() const;
	bool isFindMario() const;
	bool isWantToRest() const;
	bool isWantToAction() const;
	bool isWantToFly() const;
	void updateSound();
	void checkNotAppear(s32 event_id);
	void checkChangeToItem();
	void checkFalling();
	bool isOnGroundNerve() const;
	void behaveHitWater();

	// fabricated
	TAnimalBirdParams* getSaveParams() const
	{
		return (TAnimalBirdParams*)getSaveParam();
	}

	enum {
		BIRD_ANM_STOP        = 1,
		BIRD_ANM_FLY         = 3,
		BIRD_ANM_START       = 5,
		BIRD_ANM_WAIT        = 7,
		BIRD_ANM_WALK        = 8,
	};

public:
	// The item the bird turns into once its hit points run out; created by
	// load() from the event id in the stage's object table.
	/* 0x150 */ TMapObjBase* mItem;
	/* 0x154 */ TWireBinder* mWireBinder;
	// Where the bird was placed, raised by 90 units: the Comeback nerve
	// flies back here and the landing rotation is taken from mHomeRotation.
	/* 0x158 */ JGeometry::TVec3<f32> mHomePosition;
	/* 0x164 */ JGeometry::TVec3<f32> mHomeRotation;
	// +1 or -1; flipped every time the WalkOnGround nerve starts so that
	// consecutive walks turn the other way.
	/* 0x170 */ f32 mTurnDir;
	// Per-instance randomisation in [0.95, 1.05] applied to the march speed
	// and to all three sight ranges, so a flock does not move in lockstep.
	/* 0x174 */ f32 mPowerRate;
	// Counts down from mWaterproofTimerMax after being squirted; while it
	// runs the bird is dragged down by mWaterPowerY and wants to fly away.
	/* 0x178 */ int mWaterHitTimer;
	// How long the bird has been off the ground while still on a ground
	// nerve; past mFloatingTimerMax it takes off.
	/* 0x17C */ int mFloatingTimer;
	/* 0x180 */ int mColorIndex;
};

class TAnimalBirdManager : public TEnemyManager {
public:
	TAnimalBirdManager(const char* name = "(幸せの青い)鳥マネージャー");

	virtual ~TAnimalBirdManager() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void createModelData();
};

DECLARE_NERVE(TNerveAnimalBirdActionOnGround, TLiveActor)
DECLARE_NERVE(TNerveAnimalBirdChangeToCoin, TLiveActor)
DECLARE_NERVE(TNerveAnimalBirdComeback, TLiveActor)
DECLARE_NERVE(TNerveAnimalBirdGraphWander, TLiveActor)
DECLARE_NERVE(TNerveAnimalBirdLanding, TLiveActor)
DECLARE_NERVE(TNerveAnimalBirdPreLanding, TLiveActor)
DECLARE_NERVE(TNerveAnimalBirdTakeoff, TLiveActor)
DECLARE_NERVE(TNerveAnimalBirdWaitOnGround, TLiveActor)
DECLARE_NERVE(TNerveAnimalBirdWalkOnGround, TLiveActor)

#endif
