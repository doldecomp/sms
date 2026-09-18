#ifndef ENEMY_EFFECT_ENEMY_HPP
#define ENEMY_EFFECT_ENEMY_HPP

#include <Enemy/WalkerEnemy.hpp>

// A short-lived fire effect that damages Mario and burns out. Its behaviour on
// contact is selected by unk194.
class TEffectEnemy : public TWalkerEnemy {
public:
	TEffectEnemy(const char* name);

	virtual ~TEffectEnemy() { }
	virtual void init(TLiveManager*);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual void setMActorAndKeeper();
	virtual void kill();
	virtual void forceKill();
	virtual void reset();
	virtual void behaveToWater(THitActor*);
	virtual void sendAttackMsgToMario();
	virtual void setDeadAnm();

	void emitEffect();

	/* 0x194 */ int unk194;
};

class TEffectEnemyManager : public TSmallEnemyManager {
public:
	TEffectEnemyManager(const char* name)
	    : TSmallEnemyManager(name)
	{
	}

	virtual ~TEffectEnemyManager() { }
	virtual void load(JSUMemoryInputStream& stream);
	virtual void loadAfter();
	virtual TSpineEnemy* createEnemyInstance();
	virtual void initSetEnemies();
};

#endif
