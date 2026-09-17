#ifndef ENEMY_WIRETRAP_HPP
#define ENEMY_WIRETRAP_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Enemy/WireBinder.hpp>
#include <Strategic/Nerve.hpp>
#include <dolphin/gx/GXStruct.h>

class TWireTrap;

DECLARE_NERVE(TNerveWireTrapGoWait, TLiveActor)
DECLARE_NERVE(TNerveWireTrapOnewayMove, TLiveActor)
DECLARE_NERVE(TNerveWireTrapOnewayMoveEnd, TLiveActor)
DECLARE_NERVE(TNerveWireTrapOnewayMoveStart, TLiveActor)
DECLARE_NERVE(TNerveWireTrapReturnMove, TLiveActor)
DECLARE_NERVE(TNerveWireTrapSearch, TLiveActor)
DECLARE_NERVE(TNerveWireTrapWait, TLiveActor)

// Names and defaults are the ones PARAM_INIT stringified into .rodata and the
// constants TWireTrapManager::load stores into each TParamRT.
class TWireTrapParams : public TSpineEnemyParams {
public:
	TWireTrapParams(const char* prm);

	/* 0xA8 */ TParamRT<f32> mInWaterPowerRate;
	/* 0xBC */ TParamRT<s32> mScaleTimerMax;
	/* 0xD0 */ TParamRT<s32> mGoTimerMax;
};

// The electric trap that slides along one of the map's wires ("wire_trap").
class TWireTrap : public TSpineEnemy {
public:
	// The value read out of the fourth stream word in load(); it selects the
	// nerve getNerveFromMode() starts the trap on.
	enum {
		// Runs to one end of the wire, then flips direction and comes back.
		WIRETRAP_MODE_RETURN = 0,
		// Runs to one end of the wire, shrinks away and restarts from the
		// other edge.
		WIRETRAP_MODE_ONEWAY = 1,
		// Chases whichever way Mario is while he hangs on the wire.
		WIRETRAP_MODE_SEARCH = 2,
	};

	TWireTrap(const char* name);

	virtual ~TWireTrap() { }
	virtual void load(JSUMemoryInputStream&);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void moveObject();
	virtual void kill();

	BOOL isEndWire() const;
	BOOL isStartWire() const;
	TWireBinder* getWireBinder() const;
	TWireBinder* getWireBinder();
	f32 getRangePosInWire() const;
	static const TNerveBase<TLiveActor>* getNerveFromMode(int mode);
	BOOL isReflect() const;
	// TODO: the ROM returns a reference here (0xc bytes); see the definition.
	const JGeometry::TVec3<f32>& getWireDir() const;
	JGeometry::TVec3<f32> getDirAtWirePos() const;
	void setMoveMode(int mode);
	void emitEffects();
	void updateCollision();
	void checkHitActors();
	void doResetToEdge();
	bool doScaleDown();
	bool doScaleUp();
	bool doSearchMove();
	bool doOnewayMove();
	bool doReturnMove();
	f32 getWaterPow() const;
	void calcMomentum();
	void behaveHitWireTrap(TWireTrap* partner,
	                       const JGeometry::TVec3<f32>& mine,
	                       const JGeometry::TVec3<f32>& theirs);
	void behaveHitWater(THitActor* sender);
	void initThisColor(const GXColorS10* color);
	void initWire();
	void initParticle();
	void initCollision();

	// fabricated
	int getWaterTimer() const { return mWaterTimer; }
	int getCollideTimer() const { return mCollideTimer; }

	// fabricated
	TWireTrapParams* getSaveParams() const
	{
		return (TWireTrapParams*)getSaveParam();
	}

public:
	// TODO: four words TSpineEnemy does not own and wireTrap.cpp never
	// touches. The constructor starts at 0x160, so their type is unknown.
	/* 0x150 */ u8 unk150[0x160 - 0x150];
	/* 0x160 */ int unk160;
	// Counts down after a water hit; while it runs, getWaterPow() scales the
	// momentum by mWaterPower.
	/* 0x164 */ int mWaterTimer;
	// Signed push the water jet gave us, from mInWaterPowerRate.
	/* 0x168 */ f32 mWaterPower;
	// Counts down after bumping another trap, so the pair only reacts once.
	/* 0x16C */ int mCollideTimer;
	// +1 or -1: which way along the wire direction we travel.
	/* 0x170 */ f32 mMoveDir;
	// Frames TNerveWireTrapWait idles for; read from the stream.
	/* 0x174 */ int mWaitTime;
	// 0..1, multiplies the model scale while shrinking or growing.
	/* 0x178 */ f32 mScaleRate;
	// Units per frame along the wire; the stream stores it ten times bigger.
	/* 0x17C */ f32 mSpeed;
	/* 0x180 */ s16 mMoveMode;
};

class TWireTrapManager : public TEnemyManager {
public:
	TWireTrapManager(const char* name);

	virtual ~TWireTrapManager() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
};

#endif
