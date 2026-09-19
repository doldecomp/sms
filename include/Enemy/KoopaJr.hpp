#ifndef ENEMY_KOOPAJR_HPP
#define ENEMY_KOOPAJR_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Strategic/Nerve.hpp>

class TKoopa;
class TKoopaJr;
class TBathtub;
class TBathtubBinder;
class TBathtubKiller;
class TCallbackHitActor;

// A direction on the XZ plane in radians, wrapped to [0, 2pi).
class TDirectionCalc {
public:
	TDirectionCalc();
	TDirectionCalc(f32 direction);
	TDirectionCalc(JGeometry::TVec3<f32> dir);

	// static: the ROM's call sites set only f1 and leave r3 holding whatever
	// the previous call returned.
	static f32 r2d(f32 rad);
	static f32 d2r(f32 deg);
	f32 absDirection(f32 dir);
	JGeometry::TVec3<f32> calcDirectionVector();
	void makeDirection(JGeometry::TVec3<f32> dir);
	f32 calcTurnDirection(f32 target, f32 step);
	f32 sub(f32 dir);
	f32 calcNearerDirection(f32 dir);
	void normalize();

	// fabricated
	f32 get() const { return mDirection; }

	/* 0x0 */ f32 mDirection;
};

// A hit box that forwards every message to its owner.
class TCallbackHitActor : public THitActor {
public:
	TCallbackHitActor(const char* name, u32 actor_type, f32 radius, f32 height,
	                  THitActor* owner);

	virtual ~TCallbackHitActor() { }
	virtual BOOL receiveMessage(THitActor* sender, u32 message);

	/* 0x68 */ THitActor* mOwner;
};

// Names are the ones PARAM_INIT stringified into .rodata.
class TKoopaJrSubmarineParams : public TSpineEnemyParams {
public:
	TKoopaJrSubmarineParams(const char* prm);

	/* 0xA8 */ TParamRT<f32> killerTargetDistanceMin;
	/* 0xBC */ TParamRT<f32> killerTargetDistance;
	/* 0xD0 */ TParamRT<f32> bottomHeight;
	/* 0xE4 */ TParamRT<f32> centerZ;
	/* 0xF8 */ TParamRT<f32> aboidKoopaFlameAngle;
	/* 0x10C */ TParamRT<f32> traceMarioAngle;
	/* 0x120 */ TParamRT<f32> mSLWavePhaseVelocity;
	/* 0x134 */ TParamRT<f32> mSLWaveAmplitudeMin;
	/* 0x148 */ TParamRT<f32> mSLWaveAmplitudeMaxLaunch;
	/* 0x15C */ TParamRT<f32> mSLWaveAmplitudeMax;
	/* 0x170 */ TParamRT<f32> mSLSwingPhaseVelocity;
	/* 0x184 */ TParamRT<f32> mSLSwingAmplitudeMin;
	/* 0x198 */ TParamRT<f32> mSLSwingAmplitudeMax;
	/* 0x1AC */ TParamRT<f32> mSLRoundAngleVelocity;
	/* 0x1C0 */ TParamRT<f32> mSLRoundDistance;
	/* 0x1D4 */ TParamRT<f32> mSLAcceleration;
	/* 0x1E8 */ TParamRT<f32> mSLRotationSpeed;
	/* 0x1FC */ TParamRT<f32> mSLSpeedMax;
	/* 0x210 */ TParamRT<f32> mSLKoopaJrSubmarineScale;
	/* 0x224 */ TParamRT<f32> mSLDamageRadius;
	/* 0x238 */ TParamRT<f32> mSLDamageHeight;
	/* 0x24C */ TParamRT<f32> shineKillerProbability0;
	/* 0x260 */ TParamRT<f32> shineKillerProbability1;
	/* 0x274 */ TParamRT<s32> mSLKillerIntervalFast;
	/* 0x288 */ TParamRT<s32> mSLKillerInterval;
};

class TKoopaJrParams : public TSpineEnemyParams {
public:
	TKoopaJrParams(const char* prm);

	/* 0xA8 */ TParamRT<f32> mSLLaunchKillerLimit;
	/* 0xBC */ TParamRT<f32> mSLDamageRadius;
	/* 0xD0 */ TParamRT<f32> mSLDamageHeight;
	/* 0xE4 */ TParamRT<f32> mSLKoopaJrScale;
	/* 0xF8 */ TParamRT<f32> mSLFastLaunchDistance;
	/* 0x10C */ TParamRT<s32> mSLDamagePeriod;
	/* 0x120 */ TParamRT<s32> mSLLaunchKillerPeriod;
	/* 0x134 */ TParamRT<s32> mSLLaunchKillerPeriodFast;
};

class TKoopaJrSubmarine : public TSpineEnemy {
public:
	TKoopaJrSubmarine(const char* name = "クッパジュニアサブマリン");

	virtual ~TKoopaJrSubmarine() { }
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void bind();
	virtual const char** getBasNameTable() const;
	virtual void reset();

	void checkNerve();
	void makeDirection();
	void makeRoundVelocity();
	void makeRelativeAngle();
	void setKoopaJr(TKoopaJr*);
	void updateTimers();
	void emitKoopaJrSubmarineEffects();
	void makeKillerVelocity(TBathtubKiller*, JGeometry::TVec3<f32>);
	void launchKiller();
	bool checkKillerLaunch();
	bool appearShineKiller(int);
	void prepareKillerLaunchFast(int);
	void prepareKillerLaunch(int);
	void setAnimationIndex(int);
	void damageKoopaJrSubmarine();
	f32 getWaveAngle();
	f32 getSwingAngle();
	void moveSwing();
	void makeCollisionPositions();
	void resetKoopaJrSubmarine();

	// fabricated
	TKoopaJrSubmarineParams* getSaveParams() const
	{
		return (TKoopaJrSubmarineParams*)getSaveParam();
	}

	/* 0x150 */ s32 mKillerTimer;
	/* 0x154 */ f32 unk154;
	/* 0x158 */ f32 unk158;
	/* 0x15C */ f32 unk15C;
	/* 0x160 */ f32 unk160;
	/* 0x164 */ TDirectionCalc mDirection;
	/* 0x168 */ f32 mRoundDistance;
	/* 0x16C */ TDirectionCalc mBodyDirection;
	/* 0x170 */ u8 mIsNearTarget;
	/* 0x174 */ TBathtubBinder* mBathtubBinder;
	/* 0x178 */ u8 mKillerTypes[8];
	/* 0x180 */ s32 mKillerIndex;
	/* 0x184 */ s32 mKillerNum;
	/* 0x188 */ f32 mAnmRate;

public:
	// fabricated
	s32 getKillerIndex() const { return mKillerIndex; }
	s32 getKillerNum() const { return mKillerNum; }
	s32 getKillerTimer() const { return mKillerTimer; }
	f32 getSwingAmplitude() const { return mSwingAmplitude; }
	f32 getWaveAmplitude() const { return mWaveAmplitude; }

	/* 0x18C */ u8 mIsDamaged;
	/* 0x190 */ f32 mSwingAmplitude; // roll from being hit
	/* 0x194 */ f32 mSwingPhase;
	/* 0x198 */ f32 mWaveAmplitude; // pitch from moving / launching
	/* 0x19C */ f32 mWavePhase;
	/* 0x1A0 */ TKoopaJr* mKoopaJr;
	/* 0x1A4 */ TCallbackHitActor* mRearBody;
	/* 0x1A8 */ TCallbackHitActor* mFrontBody;
};

class TKoopaJr : public TSpineEnemy {
public:
	TKoopaJr(const char* name);

	virtual ~TKoopaJr() { }
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual const char** getBasNameTable() const;
	virtual void reset();

	f32 getBathtubY();
	void checkNerveKillerHit();
	void checkNerveKillerLaunchFast();
	void checkNerveKillerLaunchNormal();
	void checkNerve();
	void startDamageNerve();
	void checkSubmarineSwing();
	void damageKoopaJr();
	void updateTimers();
	void setAnimationIndex(int);
	void emitKoopaJrEffects();
	void startKoopaJrMessage(u32);
	void resetKoopaJr();

	// fabricated
	TKoopaJrParams* getSaveParams() const
	{
		return (TKoopaJrParams*)getSaveParam();
	}

	/* 0x150 */ s32 mDamageTimer;
	/* 0x154 */ s32 mLaunchTimer;
	/* 0x158 */ s32 mFastLaunchTimer;
	/* 0x15C */ TBathtub* mBathtub;
	/* 0x160 */ TKoopa* mKoopa;
	/* 0x164 */ TKoopaJrSubmarine* mSubmarine;
	/* 0x168 */ TEnemyManager* mSubmarineManager;
	/* 0x16C */ TEnemyManager* mKillerManager;
};

class TKoopaJrSubmarineManager : public TEnemyManager {
public:
	TKoopaJrSubmarineManager(const char* name);

	virtual ~TKoopaJrSubmarineManager() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();
};

class TKoopaJrManager : public TEnemyManager {
public:
	TKoopaJrManager(const char* name);

	virtual ~TKoopaJrManager() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();
};

DECLARE_NERVE(TNerveKoopaJrDamage, TLiveActor)
DECLARE_NERVE(TNerveKoopaJrDemo, TLiveActor)
DECLARE_NERVE(TNerveKoopaJrLaunch, TLiveActor)
DECLARE_NERVE(TNerveKoopaJrSubmarineCannonOpenClose, TLiveActor)
DECLARE_NERVE(TNerveKoopaJrSubmarineLaunchKiller, TLiveActor)
DECLARE_NERVE(TNerveKoopaJrSubmarineWait, TLiveActor)
DECLARE_NERVE(TNerveKoopaJrWait, TLiveActor)
DECLARE_NERVE(TNerveKoopaJrYahoo, TLiveActor)

#endif
