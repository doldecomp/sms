#ifndef ENEMY_ROCKET_HPP
#define ENEMY_ROCKET_HPP

#include <Enemy/SmallEnemy.hpp>
#include <Strategic/Nerve.hpp>

class TWaterEmitInfo;

// The pet-bottle rocket nozzle pickup. Nearly a sibling of TPopo: it shares
// the nozzle-possession protocol (mIsNozzleFree / releaseNozzle /
// possessedNozzle / checkTrigger / flyBehavior) and the same three .prm
// parameter names.
//
// Names and defaults are the ones PARAM_INIT stringified into .rodata; the
// defaults are what the constructor inlined into TRocketManager::load stores.
class TRocketSaveLoadParams : public TSmallEnemyParams {
public:
	TRocketSaveLoadParams(const char* prm);

	f32 getSLReleaseSpeed() const { return mSLReleaseSpeed.get(); }
	f32 getSLFlyGravity() const { return mSLFlyGravity.get(); }
	s32 getSLFlyLimitTime() const { return mSLFlyLimitTime.get(); }

	/* 0x2D4 */ TParamRT<f32> mSLReleaseSpeed;
	/* 0x2E8 */ TParamRT<f32> mSLFlyGravity;
	/* 0x2FC */ TParamRT<s32> mSLFlyLimitTime;
};

class TRocket : public TSmallEnemy {
public:
	TRocket(const char* name = "ロケット");

	virtual ~TRocket() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void bind();
	virtual f32 getGravityY() const;
	virtual const char** getBasNameTable() const;
	virtual void reset();
	virtual void behaveToWater(THitActor*);
	virtual void setDeadAnm();
	virtual void attackToMario();
	virtual void setMActorAndKeeper();
	virtual bool isCollidMove(THitActor*);

	bool isAttack();
	void flyBehavior();
	bool checkTrigger();
	void releaseNozzle();
	void possessedNozzle();

	// fabricated
	TRocketSaveLoadParams* getSaveParams() const { return mSaveParams; }

	static f32 mTestAng_x;
	static f32 mTestAng_y;
	static f32 mTestAng_z;
	static f32 mNozzleOffsetZ;
	static f32 mColOffsetY;

	/* 0x194 */ JGeometry::TVec3<f32> mInitialPos;
	/* 0x1A0 */ u8 mIsPossessed;
	/* 0x1A1 */ u8 mIsLoaded;
	/* 0x1A4 */ TRocketSaveLoadParams* mSaveParams;
};

class TRocketManager : public TSmallEnemyManager {
public:
	TRocketManager(const char* name);

	virtual ~TRocketManager() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();
	virtual void clipEnemies(JDrama::TGraphics*) { }
	virtual void initSetEnemies();

	/* 0x60 */ u8 mIsNozzleFree;
	// Initialised to null and never touched again; the popo manager has a
	// fly-water info in the same slot.
	/* 0x64 */ TWaterEmitInfo* mFlyWater;
	/* 0x68 */ TWaterEmitInfo* mExplosionWater;
};

class TLiveActor;

DECLARE_NERVE(TNerveRocketFly, TLiveActor)
DECLARE_NERVE(TNerveRocketPossessedNozzle, TLiveActor)
DECLARE_NERVE(TNerveRocketWait, TLiveActor)

#endif
