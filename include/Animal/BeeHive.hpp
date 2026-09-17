#ifndef ANIMAL_BEEHIVE_HPP
#define ANIMAL_BEEHIVE_HPP

#include <Animal/fishoid.hpp>
#include <Strategic/Nerve.hpp>

class TBee;
class TMapObjBase;

// Names and defaults are the ones PARAM_INIT stringified into .rodata and the
// constants TBeeHiveManager::load stores into each TParamRT.
class TBeeHiveParams : public TSpineEnemyParams {
public:
	TBeeHiveParams(const char* prm);

	/* 0xA8 */ TParamRT<s32> mGiveupTimer;
	/* 0xBC */ TParamRT<f32> mGiveupRange;
	/* 0xD0 */ TParamRT<s32> mDecrimentTimer;
	/* 0xE4 */ TParamRT<f32> mRebound;
	/* 0xF8 */ TParamRT<f32> mDecay;
	/* 0x10C */ TParamRT<f32> mAngleMaxAdd;
	/* 0x120 */ TParamRT<f32> mShakePower;
	/* 0x134 */ TParamRT<f32> mFallAngularVel;
	/* 0x148 */ TParamRT<f32> mSearchRange;
};

// The bee hive hanging in Bianco Hills / Pinna Park. It swings about its
// mounting point like a pendulum (mSwingAngle / mSwingSpeed), spits its swarm
// of TBee at Mario when sprayed, and finally snaps off its rope and falls.
class TBeeHive : public TRealoid {
public:
	TBeeHive(const char* name);

	virtual ~TBeeHive() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void control();
	virtual void bind();
	virtual void reset();
	virtual TBee* createRealoidActor(MActor*);

	void loadCoin(JSUMemoryInputStream&);
	void initCollision();
	void initBoids();
	void setBoidBaseParams();
	void setBoidParamOnAttacking();
	void setBoidParamOnWaiting();
	void setBoidParamOnMarioWaterIn();
	void receiveMessageFromChild(TBee* child);
	void controlCollision();
	void controlSound();
	void prepareWait();
	bool doWait();
	void prepareFall();
	void doFall();
	void appearBee(int index);
	void disappearBee(int index);
	void decrimentBee();
	void appearAllBees();
	void dropCoin();
	void prepareBreak();
	void prepareVanish();
	void prepareReset();
	bool doScaling();
	bool isMissMario() const;
	void setShakePower(const JGeometry::TVec3<f32>& to_mario);
	JGeometry::TVec3<f32> getCenterOfGravity() const;
	bool isFalling() const;
	bool isWaiting() const;
	bool isNotBreakOnceYet() const;

	// fabricated
	TBeeHiveParams* getSaveParams() const
	{
		return (TBeeHiveParams*)getSaveParam();
	}

public:
	// Rotation the hive is slerping towards; setShakePower() aims it away
	// from Mario so the hive rocks in the direction it was hit from.
	/* 0x158 */ JGeometry::TQuat4<f32> mGoalRotation;
	// Rotation actually drawn this frame.
	/* 0x168 */ JGeometry::TQuat4<f32> mRotation168;
	// Rotation built from mRotation at reset() time; the pendulum swing is
	// applied on top of it.
	/* 0x178 */ JGeometry::TQuat4<f32> mBaseRotation;
	/* 0x188 */ f32 mSwingAngle;
	/* 0x18C */ f32 mSwingSpeed;
	/* 0x190 */ f32 mSwingAngleMax;
	/* 0x194 */ JGeometry::TVec3<f32> mHomePosition;
	/* 0x1A0 */ JGeometry::TVec3<f32> mBeeCenter;
	/* 0x1AC */ TMapObjBase* mDropItem;
	/* 0x1B0 */ int mCheckBeeIndex;
	/* 0x1B4 */ int mBeeNum;
	/* 0x1B8 */ TMapObjBase** mCoins;
	/* 0x1BC */ int mEatenBeeNum;
};

class TBee : public TRealoidActor {
public:
	TBee(MActor*, TBeeHive*);

	virtual ~TBee() { }
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void init();

	void behaveToEat();

public:
	/* 0xA8 */ TBeeHive* mBeeHive;
};

class TBeeHiveManager : public TEnemyManager {
public:
	TBeeHiveManager(const char* name);

	virtual ~TBeeHiveManager() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
};

DECLARE_NERVE(TNerveBeeHiveWait, TLiveActor)
DECLARE_NERVE(TNerveBeeHiveFall, TLiveActor)
DECLARE_NERVE(TNerveBeeHiveBreak, TLiveActor)
DECLARE_NERVE(TNerveBeeHiveAttack, TLiveActor)
DECLARE_NERVE(TNerveBeeHiveMarioWaterIn, TLiveActor)
DECLARE_NERVE(TNerveBeeHiveReset, TLiveActor)

#endif
