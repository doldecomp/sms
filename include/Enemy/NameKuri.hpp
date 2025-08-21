#ifndef ENEMY_NAME_KURI_HPP
#define ENEMY_NAME_KURI_HPP

#include <Strategic/Nerve.hpp>
#include <Enemy/SmallEnemy.hpp>
#include <Enemy/Launcher.hpp>
#include <Enemy/WalkerEnemy.hpp>
#include <JSystem/JParticle/JPACallback.hpp>

class TLiveActor;
class JPABaseEmitter;
class JPABaseParticle;

class TNameKuriLauncher : public TLauncher {
public:
	TNameKuriLauncher(const char*);

	virtual void stateLaunch();
};

class TNameKuriSaveLoadParams {
public:
	TNameKuriSaveLoadParams(const char* path);
};

class TNameKuriManager : public TSmallEnemyManager {
public:
	TNameKuriManager(const char* name);

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);
	virtual void createModelData();
	virtual TSmallEnemy* createEnemyInstance();
	virtual void initSetEnemies();

	static f32 mExplosionSpeed;
	static u32 mStopMinScaleFrame;
};

class TNameKuri;

class TNameIndParCallback
    : public JPACallBackBase2<JPABaseEmitter*, JPABaseParticle*> {
public:
	TNameIndParCallback(TNameKuri*);
	void execute(JPABaseEmitter*, JPABaseParticle*);
	void draw(JPABaseEmitter*, JPABaseParticle*);
};

class TNameKuri : public TWalkerEnemy {
public:
	TNameKuri(const char* name = "ナメクリ");

	virtual void perform(u32, JDrama::TGraphics*);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void moveObject();
	virtual f32 getGravityY() const;
	virtual const char** getBasNameTable() const;
	virtual void reset();
	virtual void setBehavior();
	virtual void behaveToWater(THitActor*);
	virtual void setGenerateAnm();
	virtual void setWalkAnm();
	virtual void setDeadAnm();
	virtual void setMeltAnm();
	virtual void setWaitAnm();
	virtual void attackToMario();
	virtual void setMActorAndKeeper();
	virtual bool isHitValid(u32);
	virtual bool isCollidMove(THitActor*);
	virtual void endHitWaterJump();
	virtual void setBckAnm(int);
	virtual void setAfterDeadEffect();
	virtual void behaveToFindMario();

	void isAttackJump() const;
	void isHitWaterJump() const;
	void canJumpAttack() const;
};

class TDiffusionNameKuriManager : public TNameKuriManager {
public:
	TDiffusionNameKuriManager(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual TSmallEnemy* createEnemyInstance();
};

class TDiffusionNameKuri : public TNameKuri {
public:
	TDiffusionNameKuri(const char* name = "拡散ナメクリ")
	    : TNameKuri(name)
	{
	}
};

DECLARE_NERVE(TNerveNameKuriLand, TLiveActor);
DECLARE_NERVE(TNerveNameKuriJumpAttack, TLiveActor);
DECLARE_NERVE(TNerveNameKuriJumpAttackPrepare, TLiveActor);
DECLARE_NERVE(TNerveNameKuriExplosion, TLiveActor);
DECLARE_NERVE(TNerveNameKuriDiffuse, TLiveActor);
DECLARE_NERVE(TNerveNameKuriDrawPollute, TLiveActor);
DECLARE_NERVE(TNerveNKFollowMario, TLiveActor);

#endif
