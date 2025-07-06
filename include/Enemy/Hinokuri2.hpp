#ifndef ENEMY_HINOKURI2_HPP
#define ENEMY_HINOKURI2_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Enemy/FeetInv.hpp>

class THino2Params : public TSpineEnemyParams {
public:
	THino2Params(const char*);
};

class THinokuri2Manager : public TEnemyManager {
public:
	THinokuri2Manager(const char*);

	virtual void load(JSUMemoryInputStream&);
	void createModelData();
	TSpineEnemy* createEnemyInstance();
};

class THino2MtxCalc : public TMtxCalcFootInv {
public:
	THino2MtxCalc(u16, u16, u16, u16, u16, u16, f32);
	virtual void calc(u16);
};

class THinokuri2;

class THino2Hit : public THitActor {
public:
	THino2Hit(THinokuri2*, int, const char*);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor*, u32);
};

class THino2Mask {
public:
	THino2Mask(THinokuri2*);
	void setMatrix(MtxPtr);
	void breakMask();
	void startDamageMotion();
	void perform(u32, JDrama::TGraphics*);
};

class THinokuri2 : public TSpineEnemy {
	THinokuri2(const char*);

	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void init(TLiveManager*);
	virtual void moveObject();
	virtual void kill();
	virtual void reset();

	void emitPolParticle();
	void stopPolParticle();
	void updatePolTrans();
	void resetPolInterval();
	void invalidateCollisionAll();
	void validateCollisionAll();
	void emitWaterParticle();
	void shakeCamera(int);
	void makeQuake(float);
	void setLevel(int);
	void generateEnemy();
	void updateAnmSound();
	void changeBck(int);
	void receiveMessageLv0(THitActor*, u32);
	void receiveMessageLv1(THitActor*, u32);
	void receiveMessageLv2(THitActor*, u32);
};

DECLARE_NERVE(TNerveHino2Appear, TLiveActor);
DECLARE_NERVE(TNerveHino2GraphWander, TLiveActor);
DECLARE_NERVE(TNerveHino2Fly, TLiveActor);
DECLARE_NERVE(TNerveHino2JumpIn, TLiveActor);
DECLARE_NERVE(TNerveHino2Landing, TLiveActor);
DECLARE_NERVE(TNerveHino2Turn, TLiveActor);
DECLARE_NERVE(TNerveHino2PrePol, TLiveActor);
DECLARE_NERVE(TNerveHino2Pollute, TLiveActor);
DECLARE_NERVE(TNerveHino2Damage, TLiveActor);
DECLARE_NERVE(TNerveHino2Squat, TLiveActor);
DECLARE_NERVE(TNerveHino2Burst, TLiveActor);
DECLARE_NERVE(TNerveHino2Die, TLiveActor);
DECLARE_NERVE(TNerveHino2Stamp, TLiveActor);
DECLARE_NERVE(TNerveHino2Freeze, TLiveActor);
DECLARE_NERVE(TNerveHino2WaitAnm, TLiveActor);

#endif
