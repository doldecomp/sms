#ifndef ENEMY_YUNBO_HPP
#define ENEMY_YUNBO_HPP

#include <Enemy/SmallEnemy.hpp>
#include <Strategic/HitActor.hpp>
#include <Strategic/Nerve.hpp>

class TYumbo;
class MActor;
class J3DMaterialTable;

// Names and defaults are the ones PARAM_INIT stringified into .rodata and the
// constants TYumboManager::load stores into each TParamRT.
class TYumboParams : public TSmallEnemyParams {
public:
	TYumboParams(const char* prm);

	s32 getSLRecoverTimer() const { return mRecoverTimer.get(); }
	f32 getSLShootSpeed() const { return mShootSpeed.get(); }
	f32 getSLShootAngleX() const { return mShootAngleX.get(); }
	s32 getSLSeedLife() const { return mSeedLife.get(); }
	f32 getSLSeedAirFric() const { return mSeedAirFric.get(); }
	f32 getSLSeedGravityY() const { return mSeedGravityY.get(); }

	/* 0x2D4 */ TParamRT<s32> mRecoverTimer;
	/* 0x2E8 */ TParamRT<f32> mShootSpeed;
	/* 0x2FC */ TParamRT<f32> mShootAngleX;
	/* 0x310 */ TParamRT<s32> mSeedLife;
	/* 0x324 */ TParamRT<f32> mSeedAirFric;
	/* 0x338 */ TParamRT<f32> mSeedGravityY;
};

// One of the sixteen seeds ("ユンボ種") the flower spits at Mario.
class TYumboSeed : public THitActor {
public:
	enum {
		// The seed is back in the pool, waiting to be shot again.
		SEED_FLAG_SPENT  = 0x1,
		// Never set in this TU; the model is skipped while it is on.
		SEED_FLAG_HIDDEN = 0x4,
	};

	TYumboSeed(MActor*, const TYumbo&);

	virtual ~TYumboSeed() { }
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual void init();

	void startToMove(const JGeometry::TVec3<f32>& pos,
	                 const JGeometry::TVec3<f32>& vel, int life);
	void checkHitActors();

	/* 0x68 */ const TYumbo* mYumbo;
	/* 0x6C */ MActor* mActor;
	/* 0x70 */ u32 mSeedFlags;
	/* 0x74 */ int mLifeTimer;
	/* 0x78 */ JGeometry::TVec3<f32> mVelocity;
};

// The sambo head that hides in the ground and turns into a seed-spitting
// flower when Mario comes close.
class TYumbo : public TSmallEnemy {
public:
	TYumbo(const char* name);

	virtual ~TYumbo() { }
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void init(TLiveManager*);
	virtual void moveObject();
	virtual const char** getBasNameTable() const;
	virtual void reset();
	virtual void behaveToWater(THitActor*);
	virtual void setDeadAnm();
	virtual void attackToMario();
	virtual bool doKeepDistance();

	void initMActorAndKeeper();
	void setMaterialToMActor(MActor*, J3DMaterialTable*);
	void initCollision();
	void behaveHitAttack();
	void updateCollision();
	void updateEffect();
	bool isFindOutMario() const;
	bool isWantToAppear() const;
	bool isAllSeedBroken() const;
	bool isChangedBlock() const;
	void shotSeeds();
	void lookatMario();
	void changeToFlower();
	void changeToYumbo();
	bool isWaterproof() const;
	bool isFreeze() const;
	bool isDead() const;
	TYumboSeed* getUnusedSeed();

	// fabricated
	TYumboParams* getSaveParams() const
	{
		return (TYumboParams*)getSaveParam();
	}

	/* 0x194 */ TYumboSeed* mSeeds[16];
	/* 0x1D4 */ int mCenterJntIndex;
	/* 0x1D8 */ u8 mHideEffectDone;
};

class TYumboManager : public TSmallEnemyManager {
public:
	TYumboManager(const char* name);

	virtual ~TYumboManager() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();

	void loadMaterialTable(J3DMaterialTable**, const char*);

	/* 0x60 */ J3DMaterialTable* mMaterialTable;
};

DECLARE_NERVE(TNerveYumboAppearing, TLiveActor)
DECLARE_NERVE(TNerveYumboAttack, TLiveActor)
DECLARE_NERVE(TNerveYumboDancing, TLiveActor)
DECLARE_NERVE(TNerveYumboFreeze, TLiveActor)
DECLARE_NERVE(TNerveYumboHiding, TLiveActor)

#endif
