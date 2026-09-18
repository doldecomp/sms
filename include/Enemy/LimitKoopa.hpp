#ifndef ENEMY_LIMITKOOPA_HPP
#define ENEMY_LIMITKOOPA_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Enemy/Koopa.hpp>
#include <Enemy/KoopaJr.hpp>
#include <Strategic/Nerve.hpp>
#include <System/ParamInst.hpp>

class TLimitKoopa;
class J3DNode;

// The Corona Mountain Bowser. The parameter names come from
// /enemy/limitkoopa.prm's key strings, which are plain camelCase here rather
// than the mSL* convention the small-enemy parameter classes use.
class TLimitKoopaParams : public TSpineEnemyParams {
public:
	// The map has no symbol for this constructor anywhere: it is inlined
	// whole into TLimitKoopaManager::load, which is why it lives in the class
	// body.
	TLimitKoopaParams(const char* prm)
	    : TSpineEnemyParams(prm)
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
		// The four overrides land in .sdata because TParamRT<T>::set takes a
		// reference, so the literals need an address.
		rotationSpeed.set(0.25f);
		bodyScale.set(0.7f);
		hipDropInitialSpeedY.set(40.0f);
		hipDropGravityY.set(0.4f);
	}

	// fabricated: the params-class rung of the ladder the nerves reach the
	// animation speeds through.
	f32 getStaggerSpeed() const { return staggerSpeed.get(); }
	f32 getWaterhitSpeed() const { return waterhitSpeed.get(); }
	f32 getDownSpeed() const { return downSpeed.get(); }
	f32 getTumbleSpeed() const { return tumbleSpeed.get(); }

	/* 0x0A8 */ TParamRT<f32> rotationSpeed;
	/* 0x0BC */ TParamRT<f32> bodyScale;
	/* 0x0D0 */ TParamRT<f32> hipDropInitialSpeedY;
	/* 0x0E4 */ TParamRT<f32> hipDropGravityY;
	/* 0x0F8 */ TParamRT<f32> turnSpeed;
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

// One of Bowser's hit boxes. TLimitKoopaParts::perform forwards every actor
// that collided this frame to attack_, which each part implements.
class TLimitKoopaParts : public TLiveActor {
public:
	TLimitKoopaParts(const char* name, u32 actor_type, TLimitKoopa* owner,
	                 f32 radius);
	virtual ~TLimitKoopaParts() { }

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual void attack_(THitActor*) = 0;

	void remove();
	void set(const JGeometry::TVec3<f32>& position, f32 radius, f32 height);

	/* 0xF4 */ TLimitKoopa* mOwner;
};

class TLimitKoopaBody : public TLimitKoopaParts {
public:
	TLimitKoopaBody(TLimitKoopa* owner);
	virtual ~TLimitKoopaBody() { }

	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void attack_(THitActor*);
};

class TLimitKoopaHead : public TLimitKoopaParts {
public:
	TLimitKoopaHead(TLimitKoopa* owner);
	virtual ~TLimitKoopaHead() { }

	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void attack_(THitActor*);
};

class TLimitKoopaHand : public TLimitKoopaParts {
public:
	TLimitKoopaHand(TLimitKoopa* owner);
	virtual ~TLimitKoopaHand() { }

	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void attack_(THitActor*);
};

class TLimitKoopaFlame : public TLimitKoopaParts {
public:
	TLimitKoopaFlame(TLimitKoopa* owner);
	virtual ~TLimitKoopaFlame() { }

	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void attack_(THitActor*);
};

class TLimitKoopaManager : public TEnemyManager {
public:
	TLimitKoopaManager(const char* name = "クッパマネージャー");
	virtual ~TLimitKoopaManager() { }

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();
};

class TLimitKoopa : public TSpineEnemy {
public:
	TLimitKoopa(const char* name = "クッパ");
	virtual ~TLimitKoopa() { }

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void bind();
	virtual f32 getGravityY() const;
	virtual void reset();

	TLimitKoopaParams* getParam() const;
	void updateTimers();
	void checkMarioWhichSide();
	MtxPtr getHeadMtx() const;
	BOOL getAnmEnd() const;
	void fall();
	f32 getAnmFrame() const;
	BOOL isTumbling() const;
	void stagger(bool);
	void getShowered();
	void getDown();
	f32 getNeckFocus() const;
	BOOL isFlaming() const;
	JGeometry::TVec3<f32> getFlameDir() const;
	BOOL isBreathing() const;
	BOOL endsAnm() const;
	int getAnmIndex() const;
	void changeBck(int index, f32 rate);
	void setAnimationIndex(int index);
	void setUpHitActors();
	void breathFlame();
	void stopFlame();
	f32 makeDirection(f32);
	void calcTargetDirection();
	BOOL finishedTurn();
	void moveTurn();
	void moveHipDrop();
	void startHipDrop();
	void moveStop();
	void resetLimitKoopa();

	/* 0x150 */ int mWaitTimer;
	/* 0x154 */ int mHipDropTimer;
	/* 0x158 */ int mFlameTimer;
	/* 0x15C */ JGeometry::TVec3<f32> mAcceleration;
	/* 0x168 */ bool mLanded;
	/* 0x16C */ TDirectionCalc mBodyDirection;
	/* 0x170 */ f32 unk170;
	/* 0x174 */ u32 unk174;
	/* 0x178 */ THitActor* mFlames[10];
	/* 0x1A0 */ THitActor* mHands[2];
	/* 0x1A8 */ THitActor* mHead;
	/* 0x1AC */ THitActor* mBody;
	/* 0x1B0 */ int mHeadJntIndex;
	/* 0x1B4 */ int mNeckJntIndex;
	/* 0x1B8 */ int mAgoJntIndex;
	// TODO: unknown, 0xc bytes. The only evidence is
	// MarNameRefGen_BossEnemy's `new 0x1c8` in front of
	// __ct__11TLimitKoopaFPCc; nothing in limitkoopa.o touches 0x1bc..0x1c7
	// on a TLimitKoopa (the hits at those offsets there are
	// TLimitKoopaParams' own TParamRTs and gParticleFlagLoaded). 0xc is
	// exactly a TVec3<f32> and getFlameDir() (UNUSED, 0x180) returns one by
	// value, but resetLimitKoopa() (UNUSED, 0x2c) is the only body that could
	// prove it and it is not reconstructed yet, so this stays a placeholder.
	/* 0x1BC */ u8 unk1BC[0xc];
};

#endif
