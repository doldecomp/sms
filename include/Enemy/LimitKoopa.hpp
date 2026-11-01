#ifndef ENEMY_LIMIT_KOOPA_HPP
#define ENEMY_LIMIT_KOOPA_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Enemy/Koopa.hpp>
#include <Enemy/KoopaJr.hpp>
#include <Strategic/LiveActor.hpp>
#include <System/ParamInst.hpp>

class TLimitKoopa;

// A cut-down Bowser built on the bathtub Bowser's model and animations
// (KOOPA_ANM_*): he turns towards Mario, hip drops onto where Mario stands,
// breathes fire and reacts to water. Unlike TKoopa he stands on flat ground
// instead of the bathtub. Member names come from the .prm strings.
// fabricated name: the class is never emitted, its constructor only appears
// inlined into TLimitKoopaManager::load.
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
		// These four always override the .prm file.
		rotationSpeed.set(0.25f);
		bodyScale.set(0.7f);
		hipDropInitialSpeedY.set(40.0f);
		hipDropGravityY.set(0.4f);
	}

	/* 0xA8 */ TParamRT<f32> rotationSpeed; // degrees per frame
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
	TLimitKoopaManager(const char* name);
	virtual ~TLimitKoopaManager() { }

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();
};

// One of Bowser's hit volumes. They are not part of the enemy manager; they
// join the enemy group so they get performed and collide on their own.
class TLimitKoopaParts : public TLiveActor {
public:
	TLimitKoopaParts(const char* name, u32 actor_type, TLimitKoopa* koopa,
	                 f32 radius);
	virtual ~TLimitKoopaParts() { }

	virtual void perform(u32, JDrama::TGraphics*);
	virtual void attack_(THitActor*) = 0;

	void set(const JGeometry::TVec3<f32>& pos, f32 radius, f32 height);
	void remove();

public:
	/* 0xF4 */ TLimitKoopa* mKoopa;
};

class TLimitKoopaBody : public TLimitKoopaParts {
public:
	TLimitKoopaBody(TLimitKoopa* koopa);
	virtual ~TLimitKoopaBody() { }

	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void attack_(THitActor*);
};

class TLimitKoopaHead : public TLimitKoopaParts {
public:
	TLimitKoopaHead(TLimitKoopa* koopa);
	virtual ~TLimitKoopaHead() { }

	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void attack_(THitActor*);
};

class TLimitKoopaHand : public TLimitKoopaParts {
public:
	TLimitKoopaHand(TLimitKoopa* koopa);
	virtual ~TLimitKoopaHand() { }

	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void attack_(THitActor*);
};

// One puff of fire breath. setUpHitActors lines them up in front of the
// head while Bowser breathes fire.
class TLimitKoopaFlame : public TLimitKoopaParts {
public:
	TLimitKoopaFlame(TLimitKoopa* koopa);
	virtual ~TLimitKoopaFlame() { }

	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void attack_(THitActor*);
};

class TLimitKoopa : public TSpineEnemy {
public:
	TLimitKoopa(const char* name);
	virtual ~TLimitKoopa() { }

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void bind();
	virtual f32 getGravityY() const;
	virtual void reset();

	const TLimitKoopaParams* getParam() const;
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
	void changeBck(int index, f32 rate);
	void setAnimationIndex(int index);
	void setUpHitActors();
	void breathFlame();
	void stopFlame();
	void makeDirection(f32 dir);
	f32 calcTargetDirection();
	bool finishedTurn();
	void moveTurn();
	void moveHipDrop();
	void startHipDrop();
	void moveStop();
	void resetLimitKoopa();

public:
	/* 0x150 */ s32 mTimers[3];
	/* 0x15C */ JGeometry::TVec3<f32> mAcceleration;
	/* 0x168 */ bool mOnGround;
	/* 0x16C */ TDirectionCalc mDirection;
	/* 0x170 */ f32 unk170;
	/* 0x174 */ u32 unk174; // TODO: never touched in this file
	/* 0x178 */ TLimitKoopaFlame* mFlames[10];
	/* 0x1A0 */ TLimitKoopaHand* mHands[2];
	/* 0x1A8 */ TLimitKoopaHead* mHead;
	/* 0x1AC */ TLimitKoopaBody* mBody;
	/* 0x1B0 */ int mHeadJoint;
	/* 0x1B4 */ int mNeckJoint;
	/* 0x1B8 */ int mJawJoint;
};

#endif
