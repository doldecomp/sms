#ifndef ENEMY_BOMB_HEI_HPP
#define ENEMY_BOMB_HEI_HPP

#include <Enemy/WalkerEnemy.hpp>
#include <Enemy/BombheiNerve.hpp>

// Names and defaults are the ones PARAM_INIT stringified into .rodata and the
// constants TBombHeiManager::load stores into each TParamRT.
class TBombHeiSaveLoadParams : public TWalkerEnemyParams {
public:
	TBombHeiSaveLoadParams(const char* prm);

	s32 getSLBombTime() const { return mSLBombTime.get(); }
	f32 getSLBombRange() const { return mSLBombRange.get(); }
	f32 getSLThrownVY() const { return mSLThrownVY.get(); }
	f32 getSLThrownRateXZ() const { return mSLThrownRateXZ.get(); }
	f32 getSLThrownGravityY() const { return mSLThrownGravityY.get(); }
	f32 getSLShootVelocity() const { return mSLShootVelocity.get(); }

	/* 0x32C */ TParamRT<s32> mSLBombTime;
	/* 0x340 */ TParamRT<f32> mSLBombRange;
	/* 0x354 */ TParamRT<f32> mSLThrownVY;
	/* 0x368 */ TParamRT<f32> mSLThrownRateXZ;
	/* 0x37C */ TParamRT<f32> mSLThrownGravityY;
	/* 0x390 */ TParamRT<f32> mSLShootVelocity;
};

// The wind-up bomb (nejibomb) the chorobei on top of a cannon throws at Mario.
// It walks until its fuse runs out, can be picked up and thrown, and blows up
// into a water column, a sand column or a smoke puff depending on the ground.
class TBombHei : public TWalkerEnemy {
public:
	TBombHei(const char* name);

	// Declared in vtable order; TBombHei adds no new slots to TWalkerEnemy.
	virtual ~TBombHei() { }
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void moveObject();
	virtual void kill();
	virtual f32 getGravityY() const;
	virtual const char** getBasNameTable() const;
	virtual void reset();
	virtual void genEventCoin();
	virtual void behaveToWater(THitActor*);
	virtual void changeOut();
	virtual void behaveToTaken(THitActor*);
	virtual void behaveToRelease();
	virtual void setWalkAnm();
	virtual void setDeadAnm();
	virtual void setFreezeAnm();
	virtual void attackToMario();
	virtual void forceKill();
	virtual void setMActorAndKeeper();
	virtual bool isHitValid(u32);
	virtual bool isCollidMove(THitActor*);
	virtual void setAfterDeadEffect() { }
	virtual bool doKeepDistance() { return mKeepDistance; }
	virtual void walkBehavior(int, f32);

	bool isDamageToCannon();
	bool isExplosion();
	void bombIn();

	// fabricated
	TBombHeiSaveLoadParams* getSaveParams() const
	{
		return (TBombHeiSaveLoadParams*)getSaveParam();
	}

	// When set, a bomb that touches another exploding bomb explodes too.
	static bool mSerialBomb;

	/* 0x194 */ TBombHeiSaveLoadParams* mSaveParams;
	/* 0x198 */ int mFuseTimer;
	// Keeps the walker code at arm's length from Mario while the bomb is
	// alive; cleared once it is down and burning.
	/* 0x19C */ u8 mKeepDistance;
	// mBodyScale ratio the explosion model grows to before it ends.
	/* 0x1A0 */ f32 mExplosionScaleMax;
	// Set when Mario (and not the chorobei) was the one holding it, which is
	// what makes the bomb pay out a coin.
	/* 0x1A4 */ u8 mThrownByMario;
};

class TBombHeiManager : public TSmallEnemyManager {
public:
	TBombHeiManager(const char* name);

	virtual ~TBombHeiManager() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();
	virtual void clipEnemies(JDrama::TGraphics*) { }

	bool canMakeDeadCoin();

	/* 0x60 */ int mDeadCoinNum;
};

#endif
