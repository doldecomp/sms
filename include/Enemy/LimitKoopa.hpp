#ifndef ENEMY_LIMITKOOPA_HPP
#define ENEMY_LIMITKOOPA_HPP

#include <Strategic/Nerve.hpp>
#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Enemy/KoopaJr.hpp>

#include <JSystem/JParticle/JPAEmitter.hpp>
#include <System/EmitterViewObj.hpp>

class TLimitKoopaFlame;
class TLimitKoopaHand;
class TLimitKoopaHead;
class TLimitKoopaBody;

class TLimitKoopaParams : public TSpineEnemyParams {
public:
	TLimitKoopaParams(const char* path)
	    : TSpineEnemyParams(path)
	    , PARAM_INIT(rotationSpeed, 1.0f)
	    , PARAM_INIT(bodyScale, 1.0f)
	    , PARAM_INIT(hipDropInitialSpeedY, 1.0f)
	    , PARAM_INIT(hipDropGravityY, 1.0f)
	    , PARAM_INIT(turnSpeed, 1.6f)
	    , PARAM_INIT(turnAnim, 3.7f)
	    , PARAM_INIT(waitStep, 600.0f)
	    , PARAM_INIT(attackRadius, 800.0f)
	    , PARAM_INIT(attackHeight, 1000.0f)
	    , PARAM_INIT(focusRange, 2.0f)
	    , PARAM_INIT(waitRange, 12.0f)
	    , PARAM_INIT(fireSpeed, 4.0f)
	    , PARAM_INIT(tumbleSpeed, 2.0f)
	    , PARAM_INIT(waitSpeed, 2.0f)
	    , PARAM_INIT(staggerSpeed, 2.0f)
	    , PARAM_INIT(downSpeed, 1.8f)
	    , PARAM_INIT(tumbleWeight, 4.2f)
	    , PARAM_INIT(flameScale, 1.0f)
	    , PARAM_INIT(flameCount, 300)
	    , PARAM_INIT(flameFocusStartStep, 100)
	    , PARAM_INIT(flameFocusEndStep, 300)
	    , PARAM_INIT(flameRadius, 200.0f)
	    , PARAM_INIT(flameHeight, 600.0f)
	    , PARAM_INIT(headRadius, 400.0f)
	    , PARAM_INIT(waterhitSpeed, 2.0f)
	    , PARAM_INIT(flameOverStart, 0.9f)
	    , PARAM_INIT(flameNeckRange, 16.0f)
	    , PARAM_INIT(flameNeckDownRate, 0.3f)
	    , PARAM_INIT(marioEstimationFire, 20.0f)
	    , PARAM_INIT(marioEstimationWait, 10.0f)
	{
		TParams::load(mPrmPath);
		rotationSpeed.set(0.25f);
		bodyScale.set(0.7f);
		hipDropInitialSpeedY.set(40.0f);
		hipDropGravityY.set(0.4f);
	}

	// fabricated
	f32 getRotationSpeed() const { return rotationSpeed.get(); }
	f32 getBodyScale() const { return bodyScale.get(); }
	f32 getHipDropInitialSpeedY() const { return hipDropInitialSpeedY.get(); }
	f32 getHipDropGravityY() const { return hipDropGravityY.get(); }
	f32 getTurnSpeed() const { return turnSpeed.get(); }
	f32 getTurnAnim() const { return turnAnim.get(); }
	f32 getWaitStep() const { return waitStep.get(); }
	f32 getAttackRadius() const { return attackRadius.get(); }
	f32 getAttackHeight() const { return attackHeight.get(); }
	f32 getFocusRange() const { return focusRange.get(); }
	f32 getWaitRange() const { return waitRange.get(); }
	f32 getFireSpeed() const { return fireSpeed.get(); }
	f32 getTumbleSpeed() const { return tumbleSpeed.get(); }
	f32 getWaitSpeed() const { return waitSpeed.get(); }
	f32 getStaggerSpeed() const { return staggerSpeed.get(); }
	f32 getDownSpeed() const { return downSpeed.get(); }
	f32 getTumbleWeight() const { return tumbleWeight.get(); }
	f32 getFlameScale() const { return flameScale.get(); }
	s32 getFlameCount() const { return flameCount.get(); }
	s32 getFlameFocusStartStep() const { return flameFocusStartStep.get(); }
	s32 getFlameFocusEndStep() const { return flameFocusEndStep.get(); }
	f32 getFlameRadius() const { return flameRadius.get(); }
	f32 getFlameHeight() const { return flameHeight.get(); }
	f32 getHeadRadius() const { return headRadius.get(); }
	f32 getWaterhitSpeed() const { return waterhitSpeed.get(); }
	f32 getFlameOverStart() const { return flameOverStart.get(); }
	f32 getFlameNeckRange() const { return flameNeckRange.get(); }
	f32 getFlameNeckDownRate() const { return flameNeckDownRate.get(); }
	f32 getMarioEstimationFire() const { return marioEstimationFire.get(); }
	f32 getMarioEstimationWait() const { return marioEstimationWait.get(); }

private:
	/* 0xA8 */ TParamRT<f32> rotationSpeed;
	/* 0xBC */ TParamRT<f32> bodyScale;
	/* 0xD0 */ TParamRT<f32> hipDropInitialSpeedY;
	/* 0xE4 */ TParamRT<f32> hipDropGravityY;
	/* 0xF8 */ TParamRT<f32> turnSpeed;
	/* 0x10C */ TParamRT<f32> turnAnim;
	/* 0x120 */ TParamRT<f32> waitStep;
	/* 0x134 */ TParamRT<f32> attackRadius;
	/* 0x148 */ TParamRT<f32> attackHeight;
	/* 0x15C */ TParamRT<f32> focusRange;
	/* 0x170 */ TParamRT<f32> waitRange;
	/* 0x184 */ TParamRT<f32> fireSpeed;
	/* 0x198 */ TParamRT<f32> tumbleSpeed;
	/* 0x1AC */ TParamRT<f32> waitSpeed;
	/* 0x1C0 */ TParamRT<f32> staggerSpeed;
	/* 0x1D4 */ TParamRT<f32> downSpeed;
	/* 0x1E8 */ TParamRT<f32> tumbleWeight;
	/* 0x1FC */ TParamRT<f32> flameScale;
	/* 0x210 */ TParamRT<s32> flameCount;
	/* 0x224 */ TParamRT<s32> flameFocusStartStep;
	/* 0x238 */ TParamRT<s32> flameFocusEndStep;
	/* 0x24C */ TParamRT<f32> flameRadius;
	/* 0x260 */ TParamRT<f32> flameHeight;
	/* 0x274 */ TParamRT<f32> headRadius;
	/* 0x288 */ TParamRT<f32> waterhitSpeed;
	/* 0x29C */ TParamRT<f32> flameOverStart;
	/* 0x2B0 */ TParamRT<f32> flameNeckRange;
	/* 0x2C4 */ TParamRT<f32> flameNeckDownRate;
	/* 0x2D8 */ TParamRT<f32> marioEstimationFire;
	/* 0x2EC */ TParamRT<f32> marioEstimationWait;
};

class TLimitKoopaManager : public TEnemyManager {
public:
	TLimitKoopaManager(const char*);
	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();

	// fabricated
	TLimitKoopaParams* getSaveParam()
	{
		return (TLimitKoopaParams*)TEnemyManager::getSaveParam();
	}
};

class TLimitKoopa : public TSpineEnemy {
public:
	TLimitKoopa(const char*);
	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void bind();
	virtual f32 getGravityY() const;
	virtual void reset();

	void startHipDrop();
	void setUpHitActors();
	TLimitKoopaParams* getParam() const;
	void updateTimers();
	void checkMarioWhichSide();
	MtxPtr getHeadMtx() const;
	f32 getAnmEnd() const;
	void fall();
	f32 getAnmFrame() const;
	bool isTumbling() const;
	void stagger(bool);
	void getShowered();
	void getDown();
	f32 getNeckFocus() const;
	bool isFlaming() const;
	f32 getFlameDir() const;
	bool isBreathing() const;
	bool endsAnm() const;
	int getAnmIndex() const;
	void changeBck(int, f32);
	void setAnimationIndex(int);
	void breathFlame();

	// fabricated and fake
	void emitMtxParticle(s32 id, MtxPtr mtx, const JGeometry::TVec3<f32>& scale)
	{
		JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
		    id, mtx, TMarioParticleManager::EFFECT_LOOP_NORMAL, this);
		if (emitter)
			emitter->setGlobalScale(scale);
	}

	void stopFlame();
	void makeDirection(f32);
	f32 calcTargetDirection();
	bool finishedTurn();
	void moveTurn();
	void moveHipDrop();
	void moveStop();
	void resetLimitKoopa();

	// fabricated
	long getTimer(int index) const { return unk150[index]; }
	// fabricated
	void setTimer(int index, long time) { unk150[index] = time; }
	// fabricated
	bool getUnk168() const { return unk168; }
	// fabricated
	void setUnk168(bool value) { unk168 = value; }

public:
	/* 0x150 */ long unk150[3];
	/* 0x15C */ JGeometry::TVec3<f32> unk15C;
	/* 0x168 */ bool unk168;
	/* 0x16C */ TDirectionCalc unk16C;
	/* 0x170 */ f32 unk170;
	/* 0x174 */ int unk174;
	/* 0x178 */ TLimitKoopaFlame* unk178[10];
	/* 0x1A0 */ TLimitKoopaHand* unk1A0[2];
	/* 0x1A8 */ TLimitKoopaHead* unk1A8;
	/* 0x1AC */ TLimitKoopaBody* unk1AC;
	/* 0x1B0 */ int unk1B0;
	/* 0x1B4 */ int unk1B4;
	/* 0x1B8 */ int unk1B8;
	/* 0x1BC */ u8 unk1BC[0xC];
};

class TLimitKoopaParts : public TLiveActor {
public:
	TLimitKoopaParts(const char*, u32, TLimitKoopa*, f32);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual void attack_(THitActor*) = 0;
	void remove();
	void set(const JGeometry::TVec3<f32>&, f32, f32);
	/* 0xF4 */ TLimitKoopa* unkF4;
};

class TLimitKoopaFlame : public TLimitKoopaParts {
public:
	TLimitKoopaFlame(TLimitKoopa*);
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void attack_(THitActor*);
};

class TLimitKoopaHand : public TLimitKoopaParts {
public:
	TLimitKoopaHand(TLimitKoopa*);
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void attack_(THitActor*);
};

class TLimitKoopaHead : public TLimitKoopaParts {
public:
	TLimitKoopaHead(TLimitKoopa*);
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void attack_(THitActor*);
};

class TLimitKoopaBody : public TLimitKoopaParts {
public:
	TLimitKoopaBody(TLimitKoopa*);
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void attack_(THitActor*);
};

class TNerveLimitKoopaTurn : public TNerveBase<TLiveActor> {
public:
};

class TNerveLimitKoopaHipDropJump : public TNerveBase<TLiveActor> {
public:
	virtual BOOL execute(TSpineBase<TLiveActor>*) const;
	static const TNerveLimitKoopaHipDropJump& theNerve();
};

class TNerveLimitKoopaHipDropStart : public TNerveBase<TLiveActor> {
public:
	virtual BOOL execute(TSpineBase<TLiveActor>*) const;
	static const TNerveLimitKoopaHipDropStart& theNerve();
};

class TNerveLimitKoopaTurnR : public TNerveLimitKoopaTurn {
public:
	virtual BOOL execute(TSpineBase<TLiveActor>*) const;
	static const TNerveLimitKoopaTurnR& theNerve();
};

class TNerveLimitKoopaTurnL : public TNerveLimitKoopaTurn {
public:
	virtual BOOL execute(TSpineBase<TLiveActor>*) const;
	static const TNerveLimitKoopaTurnL& theNerve();
};

class TNerveLimitKoopaTumble : public TNerveLimitKoopaTurn {
public:
	virtual BOOL execute(TSpineBase<TLiveActor>*) const;
	static const TNerveLimitKoopaTumble& theNerve();
};

class TNerveLimitKoopaWait : public TNerveLimitKoopaTurn {
public:
	virtual BOOL execute(TSpineBase<TLiveActor>*) const;
	static const TNerveLimitKoopaWait& theNerve();
};

class TNerveLimitKoopaFlame : public TNerveLimitKoopaTurn {
public:
	virtual BOOL execute(TSpineBase<TLiveActor>*) const;
	static const TNerveLimitKoopaFlame& theNerve();
};

class TNerveLimitKoopaGetDown : public TNerveBase<TLiveActor> {
public:
	virtual BOOL execute(TSpineBase<TLiveActor>*) const;
	static const TNerveLimitKoopaGetDown& theNerve();
};

class TNerveLimitKoopaStagger : public TNerveBase<TLiveActor> {
public:
	virtual BOOL execute(TSpineBase<TLiveActor>*) const;
	static const TNerveLimitKoopaStagger& theNerve();
};

class TNerveLimitKoopaFall : public TNerveBase<TLiveActor> {
public:
	virtual BOOL execute(TSpineBase<TLiveActor>*) const;
	static const TNerveLimitKoopaFall& theNerve();
};

class TNerveLimitKoopaGetShowered : public TNerveBase<TLiveActor> {
public:
	virtual BOOL execute(TSpineBase<TLiveActor>*) const;
	static const TNerveLimitKoopaGetShowered& theNerve();
};

#endif
