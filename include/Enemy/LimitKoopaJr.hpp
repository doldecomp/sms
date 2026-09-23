#ifndef ENEMY_LIMITKOOPAJR_HPP
#define ENEMY_LIMITKOOPAJR_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Enemy/KoopaJr.hpp>
#include <Strategic/Nerve.hpp>
#include <System/ParamInst.hpp>

class TLiveActor;

// The Bowser Jr. that circles the bathtub during the Corona Mountain fight.
// Parameter names are the ones PARAM_INIT stringified into .rodata; the .prm
// is /enemy/limitkoopajr.prm and every value is overridden right after
// TParams::load(), so the PARAM_INIT defaults never reach the game.
class TLimitKoopaJrParams : public TSpineEnemyParams {
public:
	TLimitKoopaJrParams(const char* prm);

	/* 0x0A8 */ TParamRT<f32> mSLAcceleration;
	/* 0x0BC */ TParamRT<f32> mSLRotationSpeed;
	/* 0x0D0 */ TParamRT<f32> mSLSpeedMax;
	/* 0x0E4 */ TParamRT<f32> mSLRoundAngleVelocity;
	/* 0x0F8 */ TParamRT<f32> mSLRoundRadius;
	/* 0x10C */ TParamRT<f32> mSLRoundHeight;
	/* 0x120 */ TParamRT<f32> mSLDamageRadius;
	/* 0x134 */ TParamRT<f32> mSLDamageHeight;
	/* 0x148 */ TParamRT<f32> mSLKoopaJrScale;
	/* 0x15C */ TParamRT<s32> mSLShotDoodlePeriod;
	/* 0x170 */ TParamRT<s32> mSLDamagePeriod;
};

class TLimitKoopaJr : public TSpineEnemy {
public:
	TLimitKoopaJr(const char* name = "リミットクッパジュニア");

	virtual ~TLimitKoopaJr() { }
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void bind();
	virtual const char** getBasNameTable() const;
	virtual void reset();

	TDirectionCalc makeDirection(JGeometry::TVec3<f32> point);
	TDirectionCalc calcTargetDirection();
	void moveWait();
	bool canYahoo();
	bool canRun();
	void moveRun();
	void checkNerve();
	void updateTimers();
	void setAnimationIndex(int index);
	void emitKoopaJrEffects();
	void startKoopaJrMessage(u32 message);
	void resetLimitKoopaJr();

	// fabricated
	TLimitKoopaJrParams* getSaveParams() const
	{
		return (TLimitKoopaJrParams*)getSaveParam();
	}

	enum {
		LIMITKOOPAJR_ANM_DAMAGE = 1,
		LIMITKOOPAJR_ANM_RUN    = 2,
		LIMITKOOPAJR_ANM_YAHOO  = 3,
	};

public:
	// The bathtub he circles; looked up by name on the first perform().
	/* 0x150 */ THitActor* mBathtub;
	// Bowser himself, the first object of クッパマネージャー; the messages
	// Bowser Jr. relays go to him.
	/* 0x154 */ THitActor* mKoopa;
	// Countdowns decremented together by updateTimers(); the ROM's unrolled
	// indexed loop there is the evidence for the array. [1] counts down from
	// mSLShotDoodlePeriod; at zero he launches a doodle. [0] is never read.
	/* 0x158 */ int mTimers[2];
	/* 0x160 */ JGeometry::TVec3<f32> unk160;
	/* 0x16C */ f32 unk16C;
	/* 0x170 */ TDirectionCalc mRoundDirection;
	// The radius he is currently orbiting the bathtub at.
	/* 0x174 */ f32 mRoundRadius;
	/* 0x178 */ TDirectionCalc mBodyDirection;
};

class TLimitKoopaJrManager : public TEnemyManager {
public:
	TLimitKoopaJrManager(const char* name = "リミットクッパジュニアマネージャー");

	virtual ~TLimitKoopaJrManager() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();
};

DECLARE_NERVE(TNerveLimitKoopaJrLaunch, TLiveActor)
DECLARE_NERVE(TNerveLimitKoopaJrRun, TLiveActor)
DECLARE_NERVE(TNerveLimitKoopaJrWait, TLiveActor)
DECLARE_NERVE(TNerveLimitKoopaJrYahoo, TLiveActor)

#endif
