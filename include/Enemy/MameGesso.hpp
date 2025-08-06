#ifndef ENEMY_MAME_GESSO_HPP
#define ENEMY_MAME_GESSO_HPP

#include <Enemy/SmallEnemy.hpp>
#include <Enemy/WalkerEnemy.hpp>

class TMameGessoSaveLoadParams {
public:
	TMameGessoSaveLoadParams(const char* path);
};

class TMameGessoManager : public TSmallEnemyManager {
public:
	TMameGessoManager(const char* name);

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);
	virtual TSmallEnemy* createEnemyInstance();
	virtual void initSetEnemies();
};

class TMameGesso : public TWalkerEnemy {
public:
	TMameGesso(const char* name);

	virtual void load(JSUMemoryInputStream&);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void moveObject();
	virtual void kill();
	virtual f32 getGravityY() const;
	virtual const char** getBasNameTable() const;
	virtual void reset();
	virtual bool isEatenByYosshi() { return false; }
	virtual void behaveToWater(THitActor*);
	virtual bool changeByJuice() { return false; }
	virtual void behaveToTaken(THitActor*);
	virtual void behaveToRelease();
	virtual void setDeadAnm();
	virtual void setFreezeAnm();
	virtual void setWaitAnm();
	virtual void attackToMario();
	virtual void forceKill() { }
	virtual void setMActorAndKeeper();
	virtual bool isHitValid(unsigned long);
	virtual bool isCollidMove(THitActor*);
	virtual BOOL isInhibitedForceMove();
	virtual bool doKeepDistance();

	void rebirth();
	void calcObjCollision();
	void entryObjCollision();
	void checkMarioState();

	static f32 mBaseHoseiTransY;
};

DECLARE_NERVE(TNerveMameGessoGraphJumpWander, TLiveActor);
DECLARE_NERVE(TNerveMameGessoDamage, TLiveActor);
DECLARE_NERVE(TNerveMameGessoJitabata, TLiveActor);
DECLARE_NERVE(TNerveMameGessoTrample, TLiveActor);
DECLARE_NERVE(TNerveMameGessoPickUp, TLiveActor);
DECLARE_NERVE(TNerveMameGessoThrown, TLiveActor);
DECLARE_NERVE(TNerveMameGessoObject, TLiveActor);
DECLARE_NERVE(TNerveMameGessoWait, TLiveActor);

#endif
