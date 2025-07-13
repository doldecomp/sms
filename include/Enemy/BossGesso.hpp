#ifndef ENEMY_BOSS_GESSO_HPP
#define ENEMY_BOSS_GESSO_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <M3DUtil/M3UJoint.hpp>
#include <Strategic/Binder.hpp>
#include <Strategic/Nerve.hpp>

class TBossGesso;

class TBossGessoParams : public TParams {
public:
	TBossGessoParams(const char*);
};

class TBGBeakHit : public TTakeActor {
public:
	TBGBeakHit(TBossGesso*, const char*);

	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual MtxPtr getTakingMtx();
	virtual void moveRequest(const JGeometry::TVec3<f32>&);
};

class TBGEyeHit : public THitActor {
public:
	TBGEyeHit(TBossGesso*, int, const char*);

	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor*, u32);
};

class TBGBodyHit : public THitActor {
public:
	TBGBodyHit(TBossGesso*, int, const char*);

	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor*, u32);
};

class TBossGessoMtxCalc : public M3UMtxCalcSIAnmBlendQuat {
public:
	TBossGessoMtxCalc(TBossGesso*);

	void joinAnm(int);
	void setAnm(int);
	void calc(u16);
};

class TBGBinder : public TBinder {
public:
	TBGBinder();
	virtual void bind(TLiveActor*);
};

class TBGCork {
public:
	TBGCork(TBossGesso*);

	void crush();
	void perform(u32, JDrama::TGraphics*);
};

class TBossGesso : public TSpineEnemy {
public:
	TBossGesso(const char*);

	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void moveObject();
	virtual const char** getBasNameTable() const;
	virtual void reset();

	void rumblePad(int, const JGeometry::TVec3<f32>&);
	void definiteRumble();
	void continuousRumble();
	void lenFromToeToMario();
	void showMessage(u32);
	void checkTakeMsg();
	void changeBck(int);
	void inSightAngle(f32);
	void inSight();
	void is2ndFightNow() const;
	void stopIfRoll();
	void changeAttackMode(int);
	void gotTentacleDamage();
	void gotEyeDamage();
	void gotBeakDamage();
	void changeAllTentacleState(int);
	void forceAllTentacleState(int);
	void startPollute();
	void stopPollute();
	void launchPolDrop();
	void setEyeDamageBtp(int);
	void tentacleHeld() const;
	void tentacleAttack();
	void beakHeld() const;
	void tentacleWait();
	void doAttackSingle();
	void doAttackDouble();
	void doAttackSkipRope();
	void doAttackUnison();
	void doAttackShoot();
	void doAttackGuard();
	void doAttackRoll();
	void performInContainer(u32, JDrama::TGraphics*);
};

class TBossGessoManager : public TEnemyManager {
public:
	TBossGessoManager(const char*);

	virtual void load(JSUMemoryInputStream&);

	void createModelData();
	void initJParticle();
};

DECLARE_NERVE(TNerveBGWait, TLiveActor);
DECLARE_NERVE(TNerveBGEyeDamage, TLiveActor);
DECLARE_NERVE(TNerveBGBeakDamage, TLiveActor);
DECLARE_NERVE(TNerveBGTentacleDamage, TLiveActor);
DECLARE_NERVE(TNerveBGTug, TLiveActor);
DECLARE_NERVE(TNerveBGDie, TLiveActor);
DECLARE_NERVE(TNerveBGPollute, TLiveActor);
DECLARE_NERVE(TNerveBGPolDrop, TLiveActor);
DECLARE_NERVE(TNerveBGRoll, TLiveActor);

#endif
