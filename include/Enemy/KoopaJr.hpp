#ifndef ENEMY_KOOPAJR_HPP
#define ENEMY_KOOPAJR_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <M3DUtil/MActor.hpp>
#include <JSystem/JGeometry/JGVec3.hpp>
#include <Strategic/HitActor.hpp>
#include <System/ParamInst.hpp>
#include <dolphin/types.h>

class TDirectionCalc {
public:
	TDirectionCalc(JGeometry::TVec3<f32>);
	TDirectionCalc(f32);
	TDirectionCalc();

	static f32 r2d(f32);
	static f32 d2r(f32);
	f32 absDirection(f32);
	JGeometry::TVec3<f32> calcDirectionVector();
	void makeDirection(JGeometry::TVec3<f32>);
	f32 calcTurnDirection(f32, f32);
	f32 sub(f32);
	f32 calcNearerDirection(f32);
	void normalize();
	void normalizeFmod();

	// Fabricated
	f32 getUnk0() const { return unk0; }

public:
	/* 0x0 */ f32 unk0;
};

class TCallbackHitActor : public THitActor {
public:
	TCallbackHitActor(const char* name, u32 actorType, f32 damageRadius,
	                  f32 damageHeight, THitActor* owner);
	virtual ~TCallbackHitActor() { }

	virtual BOOL receiveMessage(THitActor*, u32);

public:
	/* 0x68 */ THitActor* mOwner;
};

class TBathtub;
class TBathtubBinder;
class TBathtubKiller;
class TKoopa;
class TKoopaJrSubmarine;
class TKoopaJrSubmarineManager;

DECLARE_NERVE(TNerveKoopaJrWait, TLiveActor);

DECLARE_NERVE(TNerveKoopaJrDamage, TLiveActor);

DECLARE_NERVE(TNerveKoopaJrDemo, TLiveActor);

DECLARE_NERVE(TNerveKoopaJrLaunch, TLiveActor);

DECLARE_NERVE(TNerveKoopaJrYahoo, TLiveActor);

DECLARE_NERVE(TNerveKoopaJrSubmarineWait, TLiveActor);

DECLARE_NERVE(TNerveKoopaJrSubmarineCannonOpenClose, TLiveActor);

DECLARE_NERVE(TNerveKoopaJrSubmarineLaunchKiller, TLiveActor);

class TKoopaJrParams : public TSpineEnemyParams {
public:
	TKoopaJrParams(const char* path);

	/* 0xA8 */ TParamRT<f32> mSLLaunchKillerLimit;
	/* 0xBC */ TParamRT<f32> mSLDamageRadius;
	/* 0xD0 */ TParamRT<f32> mSLDamageHeight;
	/* 0xE4 */ TParamRT<f32> mSLKoopaJrScale;
	/* 0xF8 */ TParamRT<f32> mSLFastLaunchDistance;
	/* 0x10C */ TParamRT<s32> mSLDamagePeriod;
	/* 0x120 */ TParamRT<s32> mSLLaunchKillerPeriod;
	/* 0x134 */ TParamRT<s32> mSLLaunchKillerPeriodFast;
};

class TKoopaJr : public TSpineEnemy {
public:
	enum {
		TIMER_DAMAGE,
		TIMER_LAUNCH,
		TIMER_LAUNCH_FAST,
	};

	TKoopaJr(const char* name = "クッパジュニア");
	virtual ~TKoopaJr() { }

	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual const char** getBasNameTable() const;
	virtual void reset();

	void setAnimationIndex(int index);

	void resetKoopaJr();
	void damageKoopaJr();
	void startDamageNerve();
	void checkNerveKillerHit();
	void checkSubmarineSwing();
	void updateTimers();
	void checkNerve();
	void checkNerveKillerLaunchNormal();
	void checkNerveKillerLaunchFast();
	f32 getBathtubY();
	void emitKoopaJrEffects();
	void startKoopaJrMessage(u32);

	const TKoopaJrParams* getParam() const
	{
		return (const TKoopaJrParams*)getSaveParam();
	}

public:
	/* 0x150 */ s32 mTimers[3];
	/* 0x15C */ TBathtub* mBathtub;
	/* 0x160 */ TKoopa* mKoopa;
	/* 0x164 */ TKoopaJrSubmarine* mSubmarine;
	/* 0x168 */ TKoopaJrSubmarineManager* mSubmarineManager;
	/* 0x16C */ TEnemyManager* mKillerManager;
};

class TKoopaJrManager : public TEnemyManager {
public:
	TKoopaJrManager(const char* name = "クッパジュニアマネージャー");
	virtual ~TKoopaJrManager() { }

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();
};

class TKoopaJrSubmarineParams : public TSpineEnemyParams {
public:
	TKoopaJrSubmarineParams(const char* path);

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

class TKoopaJrSubmarine : public TSpineEnemy {
public:
	enum {
		TIMER_KILLER,
	};

	TKoopaJrSubmarine(const char* name = "クッパジュニアサブマリン");
	virtual ~TKoopaJrSubmarine() { }

	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void bind();
	virtual const char** getBasNameTable() const;
	virtual void reset();

	void resetKoopaJrSubmarine();
	void prepareKillerLaunch(int num);
	void prepareKillerLaunchFast(int num);
	bool appearShineKiller(int num);
	void makeCollisionPositions();
	void makeRelativeAngle();
	void makeRoundVelocity();
	void makeDirection();
	void emitKoopaJrSubmarineEffects();
	f32 getWaveAngle();
	f32 getSwingAngle();
	void moveSwing();
	void checkKillerLaunch();
	void checkNerve();
	void launchKiller();
	void makeKillerVelocity(TBathtubKiller*, JGeometry::TVec3<f32>);
	void damageKoopaJrSubmarine();
	void updateTimers();
	void setKoopaJr(TKoopaJr* koopaJr);
	void setAnimationIndex(int index);

	const TKoopaJrSubmarineParams* getParam() const
	{
		return (const TKoopaJrSubmarineParams*)getSaveParam();
	}

public:
	/* 0x150 */ s32 mTimers[1];
	/* 0x154 */ f32 unk154;
	/* 0x158 */ f32 unk158;
	/* 0x15C */ f32 unk15C;
	/* 0x160 */ f32 unk160;
	/* 0x164 */ TDirectionCalc mRoundAngle;
	/* 0x168 */ f32 mRoundDistance;
	/* 0x16C */ TDirectionCalc mDirection;
	/* 0x170 */ bool mIsNearRoundPos;
	/* 0x174 */ TBathtubBinder* mBathtubBinder;
	/* 0x178 */ s8 mKillerTypes[8];
	/* 0x180 */ s32 mKillerIndex;
	/* 0x184 */ s32 mKillerNum;
	/* 0x188 */ f32 mAnmRate;
	/* 0x18C */ bool mIsSprayed;
	/* 0x190 */ f32 mSwingAmplitude;
	/* 0x194 */ f32 mSwingPhase;
	/* 0x198 */ f32 mWaveAmplitude;
	/* 0x19C */ f32 mWavePhase;
	/* 0x1A0 */ TKoopaJr* mKoopaJr;
	/* 0x1A4 */ TCallbackHitActor* mRearBody;
	/* 0x1A8 */ TCallbackHitActor* mFrontBody;
};

class TKoopaJrSubmarineManager : public TEnemyManager {
public:
	TKoopaJrSubmarineManager(const char* name
	                         = "クッパジュニアサブマリンマネージャー");
	virtual ~TKoopaJrSubmarineManager() { }

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();
};

#endif
