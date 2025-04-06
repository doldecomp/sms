#ifndef ENEMY_SMALL_ENEMY_HPP
#define ENEMY_SMALL_ENEMY_HPP

#include <Enemy/Enemy.hpp>
#include <Strategic/Nerve.hpp>

class TSmallEnemyParams {
public:
	TSmallEnemyParams(const char*);
};

class TSmallEnemyManager {
public:
	void getHolder(int);
	void createEnemies(int);
	void loadAfter();
	void load(JSUMemoryInputStream&);
	void createModelData();
	TSmallEnemyManager(const char*);
};

class TSmallEnemy : public TSpineEnemy {
public:
	void setDeadAnm();
	void setWaitAnm();

	TSmallEnemy(const char*);
	void setMActorAndKeeper();
	void init(TLiveManager*);
	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	void behaveToWater(THitActor*);
	void sendAttackMsgToMario();
	void attackToMario();
	void reset();
	void forceKill();
	void genRandomItem();
	void genEventCoin();
	void setAfterDeadEffect();
	void generateItem();
	void isCollidMove(THitActor*);
	void moveObject();
	void updateAnmSound();
	virtual u32 receiveMessage(THitActor*, u32);
	void changeByJuice();
	void getChangeBlockTime();
	void changeMove();
	void scalingChangeActor();
	void changeOut();
	void decHpByWater(THitActor*);
	void kill();
	void isFindMario(f32);
	void isMarioInWater() const;
	void isFindMarioFromParam(f32) const;
	void generateEffectColumWater();
	void setBckAnm(int);
	void expandCollision();
	void isEaten();
	void isHitWallInBound();
	void behaveToHitOthers(THitActor*);
	virtual void perform(u32, JDrama::TGraphics*);
};

class TLiveActor;

DECLARE_NERVE(TNerveSmallEnemyDie, TLiveActor);
DECLARE_NERVE(TNerveSmallEnemyFreeze, TLiveActor);
DECLARE_NERVE(TNerveSmallEnemyJump, TLiveActor);
DECLARE_NERVE(TNerveSmallEnemyHitWaterJump, TLiveActor);
DECLARE_NERVE(TNerveSmallEnemyWait, TLiveActor);
DECLARE_NERVE(TNerveSmallEnemyChange, TLiveActor);

#endif
