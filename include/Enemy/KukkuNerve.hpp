#ifndef ENEMY_KUKKUNERVE_HPP
#define ENEMY_KUKKUNERVE_HPP

#include <Enemy/SmallEnemy.hpp>
#include <Strategic/Nerve.hpp>

class TLiveActor;
class MActor;
class TMapObjBase;

// The gull that circles Ricco Harbour, spits mud at Mario and drops coins
// when it is knocked out of the sky. The model folder is "tori", so the .bas
// table and every animation name use that spelling.
// Names and defaults are the ones PARAM_INIT stringified into .rodata and the
// constants TKukkuManager::load stores into each TParamRT.
class TKukkuParams : public TSmallEnemyParams {
public:
	TKukkuParams(const char* prm);

	f32 getMarchSpeed() const { return mMarchSpeed.get(); }
	f32 getTurnSpeed() const { return mTurnSpeed.get(); }
	f32 getWaterPowerY() const { return mWaterPowerY.get(); }
	f32 getShootSpeed() const { return mShootSpeed.get(); }
	s32 getShootInterval() const { return mShootInterval.get(); }
	f32 getSearchRange() const { return mSearchRange.get(); }
	s32 getHabatakiTimer() const { return mHabatakiTimer.get(); }
	f32 getAirFric() const { return mAirFric.get(); }
	f32 getUpperVelocityY() const { return mUpperVelocityY.get(); }
	f32 getDropSpeed() const { return mDropSpeed.get(); }
	f32 getDropAngleX() const { return mDropAngleX.get(); }

	/* 0x2D4 */ TParamRT<f32> mMarchSpeed;
	/* 0x2E8 */ TParamRT<f32> mTurnSpeed;
	/* 0x2FC */ TParamRT<f32> mWaterPowerY;
	/* 0x310 */ TParamRT<f32> mShootSpeed;
	/* 0x324 */ TParamRT<s32> mShootInterval;
	/* 0x338 */ TParamRT<f32> mSearchRange;
	/* 0x34C */ TParamRT<s32> mHabatakiTimer;
	/* 0x360 */ TParamRT<f32> mAirFric;
	/* 0x374 */ TParamRT<f32> mUpperVelocityY;
	/* 0x388 */ TParamRT<f32> mDropSpeed;
	/* 0x39C */ TParamRT<f32> mDropAngleX;
};

// The mud ball the gull spits. It is a bare hit actor carrying its own MActor
// and its own tiny physics step, not a TLiveActor.
class TKukkuBall : public THitActor {
public:
	enum {
		KUKKUBALL_FLAG_DEAD    = 0x1,
		KUKKUBALL_FLAG_NO_DRAW = 0x4,
	};

	TKukkuBall(MActor* actor);

	// Declared in vtable order; only init() is a new slot (0xa4).
	virtual ~TKukkuBall() { }
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual void init();

	bool isDead() const;
	void kill();
	void startToMove(const JGeometry::TVec3<f32>& position,
	                 const JGeometry::TVec3<f32>& velocity);
	void checkHitActors();
	void bind();
	void moveObject();

	/* 0x68 */ MActor* mMActor;
	/* 0x6C */ u32 mFlags;
	/* 0x70 */ JGeometry::TVec3<f32> mVelocity;
	// TODO: only ever written to 0 (in the constructor) and read by isDead(),
	// so nothing in the retail code can tell what it counted.
	/* 0x7C */ int unk7C;
};

// A pool of coins an enemy hands out. Every method is UNUSED, so the bodies
// below are reconstructed from the names and sizes only.
class TEnemyCoinUnit {
public:
	TEnemyCoinUnit(int num);

	void init();
	TMapObjBase* getUnusedItem();

	/* 0x0 */ int mItemNum;
	/* 0x4 */ TMapObjBase** mItems;
};

class TKukku : public TSmallEnemy {
public:
	TKukku(const char* name);

	// Declared in vtable order; TKukku adds no new slots to TSmallEnemy.
	virtual ~TKukku() { }
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void control();
	virtual void bind();
	virtual const char** getBasNameTable() const;
	virtual void reset();
	virtual void behaveToWater(THitActor*);
	virtual void setDeadAnm();
	virtual void setAfterDeadEffect();

	void initParticle();
	void initCollision();
	void initBalls();
	void updateEffect();
	void behaveHitTrample();
	void doFlyToCurPathNode();
	JGeometry::TVec3<f32> calcMomentum(f32 speed);
	void updateRotation();
	void doRecoverToCurPathNode();
	void doHabataki();
	void decideFlyingAnm();
	void shotBall();
	void dropCoins();
	bool isFalling() const;
	bool isRecoveringGraph() const;
	bool isDying() const;
	bool isFindOutMario() const;
	void changeBck(const char* name);
	TKukkuBall* getUnusedBall();
	f32 getWaterPowerY() const;
	f32 getWaterDamageRate() const;

	// fabricated: reading through the const accessor is what makes control()
	// load mHitTimer twice, as retail does.
	int getHitTimer() const { return mHitTimer; }
	int getShootTimer() const { return mShootTimer; }

	// fabricated
	TKukkuParams* getSaveParams() const
	{
		return (TKukkuParams*)getSaveParam();
	}

	/* 0x194 */ TKukkuBall* mBalls[3];
	// The 1UP the gull carries; created up front and released once it has paid
	// out its coins.
	/* 0x1A0 */ TMapObjBase* mOneUp;
	// TODO: only counted down by control() and cleared by reset(); whatever
	// sets it was dead-stripped.
	/* 0x1A4 */ int mHitTimer;
	/* 0x1A8 */ int mCenterJointIndex;
	/* 0x1AC */ int mShootTimer;
	/* 0x1B0 */ int mDroppedCoins;
};

class TKukkuManager : public TSmallEnemyManager {
public:
	TKukkuManager(const char* name);

	virtual ~TKukkuManager() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();

	void initCoins();

	// TODO: unknown. The only evidence that this member exists at all is
	// MarNameRefGen_Enemy's `new 0x64` in front of
	// __ct__13TKukkuManagerFPCc (TKazekunManager, the other member-less
	// TSmallEnemyManager, is allocated at 0x60), and nothing in Kukku.o or
	// MarNameRefGen_Enemy.o touches 0x60. The constructor does not
	// initialise it and initCoins() is UNUSED at 0x4, i.e. an empty body, so
	// whatever read it was dead-stripped. Name and type are guesses; only
	// the size is proven.
	/* 0x60 */ u32 unk60;
};

DECLARE_NERVE(TNerveKukkuFall, TLiveActor)
DECLARE_NERVE(TNerveKukkuGraphWander, TLiveActor)
DECLARE_NERVE(TNerveKukkuHit, TLiveActor)
DECLARE_NERVE(TNerveKukkuPostFall, TLiveActor)
DECLARE_NERVE(TNerveKukkuRecoverGraph, TLiveActor)

#endif
