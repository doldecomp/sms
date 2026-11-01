#ifndef ENEMY_KOOPA_HPP
#define ENEMY_KOOPA_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Strategic/HitActor.hpp>
#include <Strategic/Nerve.hpp>
#include <Strategic/Spine.hpp>
#include <System/ParamInst.hpp>

class TKoopa;

// Bowser's animation indices (bck/btp files of koopa_model.bmd, matching
// koopa_bastable).
enum {
	KOOPA_ANM_DOWN       = 0,
	KOOPA_ANM_DOWN_LOOP  = 1,
	KOOPA_ANM_FALL       = 2,
	KOOPA_ANM_FIRE_END   = 3,
	KOOPA_ANM_FIRE_LOOP  = 4,
	KOOPA_ANM_FIRE_START = 5,
	KOOPA_ANM_FIRST      = 6,
	KOOPA_ANM_GETUP      = 7,
	KOOPA_ANM_HIPDROP    = 8,
	KOOPA_ANM_STAGGER    = 9,
	KOOPA_ANM_TURN_L     = 10,
	KOOPA_ANM_TURN_R     = 11,
	KOOPA_ANM_WAIT       = 12,
	KOOPA_ANM_UNK13      = 13,
	KOOPA_ANM_WATERHIT   = 14,
};

// fabricated name: the class is never emitted, its constructor only appears
// inlined into TKoopaManager::load. Member names come from the .prm strings.
class TKoopaParams : public TSpineEnemyParams {
public:
	TKoopaParams(const char* path)
	    : TSpineEnemyParams(path)
	    , PARAM_INIT(turnSpeed, 1.6f)
	    , PARAM_INIT(turnAnim, 3.7f)
	    , PARAM_INIT(waitStep, 600.0f)
	    , PARAM_INIT(downStep, 1000.0f)
	    , PARAM_INIT(attackRadius, 800.0f)
	    , PARAM_INIT(attackHeight, 1000.0f)
	    , PARAM_INIT(focusRange, 2.0f)
	    , PARAM_INIT(waitRange, 12.0f)
	    , PARAM_INIT(fireSpeed, 3.5f)
	    , PARAM_INIT(tumbleWeight, 8.3f)
	    , PARAM_INIT(tumbleSpeed, 2.0f)
	    , PARAM_INIT(tumbleStartFrame, 95.0f)
	    , PARAM_INIT(tumbleEndFrame, 160.0f)
	    , PARAM_INIT(waitSpeed, 2.0f)
	    , PARAM_INIT(staggerSpeed, 2.0f)
	    , PARAM_INIT(downSpeed, 2.0f)
	    , PARAM_INIT(flameVelocity, 35.0f)
	    , PARAM_INIT(flameScale, 1.0f)
	    , PARAM_INIT(flameCount, 500)
	    , PARAM_INIT(flameFocusStartStep, 25)
	    , PARAM_INIT(flameFocusEndStep, 500)
	    , PARAM_INIT(flameRadius, 300.0f)
	    , PARAM_INIT(flameHeight, 1000.0f)
	    , PARAM_INIT(headRadius, 400.0f)
	    , PARAM_INIT(waterhitSpeed, 2.0f)
	    , PARAM_INIT(flameOverStart, 1.0f)
	    , PARAM_INIT(flameNeckRange, 17.0f)
	    , PARAM_INIT(flameNeckDownRate, 0.3f)
	    , PARAM_INIT(flameJump, 80.0f)
	    , PARAM_INIT(fallSpeed, 2.0f)
	    , PARAM_INIT(marioEstimationFire, 20.0f)
	    , PARAM_INIT(marioEstimationWait, 10.0f)
	{
		TParams::load(mPrmPath);
	}

	/* 0xA8 */ TParamRT<f32> turnSpeed;
	/* 0xBC */ TParamRT<f32> turnAnim;
	/* 0xD0 */ TParamRT<f32> waitStep;
	/* 0xE4 */ TParamRT<f32> downStep;
	/* 0xF8 */ TParamRT<f32> attackRadius;
	/* 0x10C */ TParamRT<f32> attackHeight;
	/* 0x120 */ TParamRT<f32> focusRange;
	/* 0x134 */ TParamRT<f32> waitRange;
	/* 0x148 */ TParamRT<f32> fireSpeed;
	/* 0x15C */ TParamRT<f32> tumbleWeight;
	/* 0x170 */ TParamRT<f32> tumbleSpeed;
	/* 0x184 */ TParamRT<f32> tumbleStartFrame;
	/* 0x198 */ TParamRT<f32> tumbleEndFrame;
	/* 0x1AC */ TParamRT<f32> waitSpeed;
	/* 0x1C0 */ TParamRT<f32> staggerSpeed;
	/* 0x1D4 */ TParamRT<f32> downSpeed;
	/* 0x1E8 */ TParamRT<f32> flameVelocity;
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
	/* 0x2D8 */ TParamRT<f32> flameJump;
	/* 0x2EC */ TParamRT<f32> fallSpeed;
	/* 0x300 */ TParamRT<f32> marioEstimationFire;
	/* 0x314 */ TParamRT<f32> marioEstimationWait;
};

class TKoopaManager : public TEnemyManager {
public:
	TKoopaManager(const char* name = "クッパマネージャー");
	virtual ~TKoopaManager() { }

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();
};

// One of Bowser's hit volumes. They are not part of the enemy manager; they
// join the enemy group so they get performed and collide on their own.
class TKoopaParts : public THitActor {
public:
	TKoopaParts(const char* name, u32 actor_type, TKoopa* koopa, f32 radius);
	virtual ~TKoopaParts() { }

	virtual void perform(u32, JDrama::TGraphics*);
	virtual void control() { }
	virtual void attack_(THitActor*) = 0;

	void set(const JGeometry::TVec3<f32>& pos, f32 radius, f32 height);
	void remove();

public:
	/* 0x68 */ TKoopa* mKoopa;
};

class TKoopaBody : public TKoopaParts {
public:
	TKoopaBody(TKoopa* koopa);
	virtual ~TKoopaBody() { }

	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void attack_(THitActor*);
};

class TKoopaHead : public TKoopaParts {
public:
	TKoopaHead(TKoopa* koopa);
	virtual ~TKoopaHead() { }

	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void attack_(THitActor*);
};

class TKoopaHand : public TKoopaParts {
public:
	TKoopaHand(TKoopa* koopa);
	virtual ~TKoopaHand() { }

	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void attack_(THitActor*);
};

// One puff of fire breath, travelling along a straight line.
class TKoopaFlame : public TKoopaParts {
public:
	TKoopaFlame(TKoopa* koopa);
	virtual ~TKoopaFlame() { }

	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void control();
	virtual void attack_(THitActor*);

	void fire(const JGeometry::TVec3<f32>& origin,
	          const JGeometry::TVec3<f32>& dir, f32 speed, f32 length,
	          f32 radius, f32 height);
	void resetFlame();
	f32 getLength() const;
	bool isAlive() const;

public:
	/* 0x6C */ JGeometry::TVec3<f32> mOrigin;
	/* 0x78 */ JGeometry::TVec3<f32> mDir;
	/* 0x84 */ f32 mSpeed;
	/* 0x88 */ f32 mMaxLength;
	/* 0x8C */ f32 mLength;
	/* 0x90 */ f32 mRadius;
	/* 0x94 */ f32 mHeight;
};

class TKoopa : public TSpineEnemy {
public:
	TKoopa(const char*);
	virtual ~TKoopa() { }

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void updateAnmSound();
	virtual const char** getBasNameTable() const;
	virtual void reset();

	const TKoopaParams* getParam() const;
	void checkMarioWhichSide();
	MtxPtr getHeadMtx() const;
	f32 getAnmEnd() const;
	void fall();
	bool canTumble() const;
	bool passesAnmFrame(f32) const;
	f32 getAnmFrameNext() const;
	f32 getAnmFrame() const;
	f32 getTargetDir(const JGeometry::TVec3<f32>&) const;
	bool isTumbling() const;
	void stagger(bool);
	bool getShowered();
	bool effectsTumble() const;
	void getDown();
	bool allowsLaunch() const;
	f32 getNeckFocus() const;
	bool isProvoking() const;
	bool isFlaming() const;
	f32 getFlameDirRate() const;
	void setIgnoreMario(long);
	bool ignoresMario() const;
	bool isBreathing() const;
	void laugh();
	bool endsAnm() const;
	int getAnmIndex() const;
	void changeAnm(int bck, int btp, f32 speed);
	void setUpHitActors();
	void resetFlame_();
	void breathFlame();
	void stopFlame();
	f32 getFlameDirDegree() const;

public:
	/* 0x150 */ f32 mTargetDir; // degrees
	/* 0x154 */ bool mFlameFlipped;
	/* 0x155 */ bool mWillLaugh;
	/* 0x158 */ JGeometry::TVec3<f32> mSoundPos;
	/* 0x164 */ TKoopaFlame* mFlames[10];
	/* 0x18C */ TKoopaHand* mHands[2];
	/* 0x194 */ TKoopaHead* mHead;
	/* 0x198 */ TKoopaBody* mBody;
	/* 0x19C */ s32 mIgnoreMarioTimer;
	/* 0x1A0 */ int mHeadJoint;
	/* 0x1A4 */ int mNeckJoint;
	/* 0x1A8 */ int mJawJoint;
	/* 0x1AC */ char unk1AC[0xC];
	/* 0x1B8 */ f32 mNeckFocus;
};

#endif
