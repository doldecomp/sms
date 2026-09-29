#ifndef ENEMY_EFFECTENEMY_HPP
#define ENEMY_EFFECTENEMY_HPP

#include <Enemy/SmallEnemy.hpp>
#include <Enemy/WalkerEnemy.hpp>
#include <JSystem/JDrama/JDRGraphics.hpp>
#include <dolphin/types.h>

class JSUMemoryInputStream;
class THitActor;
class TLiveManager;

class TEffectEnemy : public TWalkerEnemy {
public:
	TEffectEnemy(const char* name = "エフェクト敵");

	virtual void perform(u32, JDrama::TGraphics*);
	virtual void init(TLiveManager*);
	virtual void kill();
	virtual void reset();
	virtual void behaveToWater(THitActor*);
	virtual void setDeadAnm();
	virtual void forceKill();
	virtual void setMActorAndKeeper();
	virtual void sendAttackMsgToMario();

	void emitEffect();

public:
	/* 0x194 */ s32 unk194;
};

class TEffectEnemyManager : public TSmallEnemyManager {
public:
	TEffectEnemyManager(const char* name = "エフェクト敵マネージャー")
	    : TSmallEnemyManager(name)
	{
	}

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual TSpineEnemy* createEnemyInstance();
	virtual void initSetEnemies();
};

#endif // ENEMY_EFFECTENEMY_HPP
