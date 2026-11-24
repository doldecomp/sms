#ifndef ENEMY_KOOPAJR_HPP
#define ENEMY_KOOPAJR_HPP

#include <JSystem/JGeometry/JGQuat4.hpp>
#include <JSystem/JGeometry/JGVec3.hpp>
#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <M3DUtil/MActor.hpp>
#include <Strategic/Nerve.hpp>
#include <dolphin/types.h>

class TDirectionCalc {
public:
	TDirectionCalc(JGeometry::TVec3<f32> direction);
	TDirectionCalc(f32 direction);
	TDirectionCalc();

	static f32 r2d(f32 direction);
	static f32 d2r(f32 direction);
	f32 absDirection(f32 direction);
	JGeometry::TVec3<f32> calcDirectionVector();
	void makeDirection(JGeometry::TVec3<f32> direction);
	f32 calcTurnDirection(f32 direction, f32 turn_speed);
	f32 sub(f32 direction);
	f32 calcNearerDirection(f32 direction);

	// UNUSED - 0x4c size from map
	void normalize();

public:
	/* 0x0 */ f32 mDirection;
};

class TBathtub;
class TBathtubBinder;
class TBathtubKiller;
class TBathtubKillerManager;
class TKoopa;
class TKoopaJr;
class TKoopaJrSubmarine;
class TKoopaJrParams;
class TKoopaJrSubmarineParams;

class TCallbackHitActor : public THitActor {
public:
	// UNUSED - 0x12c size from map
	TCallbackHitActor(const char* name, u32 type, f32 radius, f32 height,
	                  THitActor* actor);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);

public:
	/* 0x68 */ THitActor* unk68;
};

class TKoopaJrManager : public TEnemyManager {
public:
	TKoopaJrManager(const char* name);
	virtual void load(JSUMemoryInputStream& stream);
	virtual void loadAfter();
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();
};

class TKoopaJrSubmarineManager : public TEnemyManager {
public:
	TKoopaJrSubmarineManager(const char* name);
	virtual void load(JSUMemoryInputStream& stream);
	virtual void loadAfter();
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();
};

class TKoopaJr : public TSpineEnemy {
public:
	enum {
		TIMER_DAMAGE               = 0,
		TIMER_KILLER_LAUNCH_NORMAL = 1,
		TIMER_KILLER_LAUNCH_FAST   = 2,
		TIMER_COUNT                = 3,
	};

	TKoopaJr(const char* name);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void init(TLiveManager* manager);
	virtual void calcRootMatrix();
	virtual const char** getBasNameTable() const;
	virtual void reset();

	// UNUSED - 0x34 size from map
	f32 getBathtubY();
	void checkNerveKillerHit();
	void checkNerveKillerLaunchFast();
	void checkNerveKillerLaunchNormal();
	// UNUSED - 0x418 size from map
	void checkNerve();
	// UNUSED - 0x230 size from map
	void startDamageNerve();
	// UNUSED - 0x274 size from map
	void checkSubmarineSwing();
	// UNUSED - 0x240 size from map
	void damageKoopaJr();
	// UNUSED - 0x4c size from map
	void updateTimers();
	// UNUSED - 0x70 size from map
	void setAnimationIndex(int index);
	// UNUSED - 0x4 size from map
	void emitKoopaJrEffects();
	// UNUSED - 0x2c size from map
	void startKoopaJrMessage(u32 message);
	// UNUSED - 0x64 size from map
	void resetKoopaJr();

	// fabricated below
	bool isTimerActive(int index) const { return mTimers[index] > 0; }
	TKoopaJrParams* getParam() const { return (TKoopaJrParams*)getSaveParam(); }
	TBathtub* getBathtub() const { return mBathtub; }
	TKoopa* getKoopa() const { return mKoopa; }
	TBathtubKillerManager* getKillerManager() const { return mKillerManager; }

public:
	/* 0x150 */ s32 mTimers[TIMER_COUNT];
	/* 0x15C */ TBathtub* mBathtub;
	/* 0x160 */ TKoopa* mKoopa;
	/* 0x164 */ TKoopaJrSubmarine* mSubmarine;
	/* 0x168 */ TKoopaJrSubmarineManager* mSubmarineManager;
	/* 0x16C */ TBathtubKillerManager* mKillerManager;
};

class TKoopaJrSubmarineParams : public TSpineEnemyParams {
public:
	TKoopaJrSubmarineParams(const char* name);

public:
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
	TKoopaJrSubmarine(const char* name = "クッパジュニアサブマリン");
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void init(TLiveManager* manager);
	virtual void calcRootMatrix();
	virtual void bind();
	virtual const char** getBasNameTable() const;
	virtual void reset();

	void checkNerve();
	// UNUSED - 0x18c size from map
	void makeDirection();
	void makeRoundVelocity();
	void makeRelativeAngle();
	// UNUSED - 0x8 size from map
	void setKoopaJr(TKoopaJr* actor);
	// UNUSED - 0x1c size from map
	void updateTimers();
	// UNUSED - 0x4 size from map
	void emitKoopaJrSubmarineEffects();
	void makeKillerVelocity(TBathtubKiller* killer,
	                        JGeometry::TVec3<f32> direction);
	void launchKiller();
	// UNUSED - 0x100 size from map
	void checkKillerLaunch();
	bool appearShineKiller(int count);
	// UNUSED - 0x84 size from map
	void prepareKillerLaunchFast(int count);
	// UNUSED - 0x80 size from map
	void prepareKillerLaunch(int count);
	// UNUSED - 0x70 size from map
	void setAnimationIndex(int index);
	// UNUSED - 0xc size from map
	void damageKoopaJrSubmarine();
	// UNUSED - 0x38 size from map
	f32 getWaveAngle();
	// UNUSED - 0x38 size from map
	f32 getSwingAngle();
	void moveSwing();
	void makeCollisionPositions();
	void resetKoopaJrSubmarine();

	// fabricated
	TKoopaJrSubmarineParams* getParam() const
	{
		return (TKoopaJrSubmarineParams*)getSaveParam();
	}

	// fabricated
	void resumeAnimation()
	{
		J3DFrameCtrl* ctrl = getMActor()->getFrameCtrl(ANM_TYPE_BCK);
		ctrl->setRate(unk188);
	}
	// fabricated
	bool isSwingBelowLimit(f32 limit) const { return unk190 < limit; }

public:
	/* 0x150 */ s32 unk150[1];
	/* 0x154 */ JGeometry::TQuat4<f32> unk154;
	/* 0x164 */ TDirectionCalc unk164;
	/* 0x168 */ f32 unk168;
	/* 0x16C */ TDirectionCalc unk16C;
	/* 0x170 */ u8 unk170;
	/* 0x174 */ TBathtubBinder* mBathtubBinder;
	/* 0x178 */ s8 unk178[8];
	/* 0x180 */ s32 unk180;
	/* 0x184 */ s32 unk184;
	/* 0x188 */ f32 unk188;
	/* 0x18C */ u8 unk18C;
	/* 0x190 */ f32 unk190;
	/* 0x194 */ f32 unk194;
	/* 0x198 */ f32 unk198;
	/* 0x19C */ f32 unk19C;
	/* 0x1A0 */ TKoopaJr* mKoopaJr;
	/* 0x1A4 */ TCallbackHitActor* unk1A4;
	/* 0x1A8 */ TCallbackHitActor* unk1A8;
};

class TNerveKoopaJrWait : public TNerveBase<TLiveActor> {
public:
	virtual BOOL execute(TSpineBase<TLiveActor>* spine) const;
	// UNUSED - 0x64 size from map
	static const TNerveKoopaJrWait& theNerve();
};
class TNerveKoopaJrDamage : public TNerveBase<TLiveActor> {
public:
	virtual BOOL execute(TSpineBase<TLiveActor>* spine) const;
	static const TNerveKoopaJrDamage& theNerve();
};
class TNerveKoopaJrDemo : public TNerveBase<TLiveActor> {
public:
	virtual BOOL execute(TSpineBase<TLiveActor>* spine) const;
	// UNUSED - 0x64 size from map
	static const TNerveKoopaJrDemo& theNerve();
};
class TNerveKoopaJrLaunch : public TNerveBase<TLiveActor> {
public:
	virtual BOOL execute(TSpineBase<TLiveActor>* spine) const;
	// UNUSED - 0x64 size from map
	static const TNerveKoopaJrLaunch& theNerve();
};
class TNerveKoopaJrYahoo : public TNerveBase<TLiveActor> {
public:
	virtual BOOL execute(TSpineBase<TLiveActor>* spine) const;
	// UNUSED - 0x64 size from map
	static const TNerveKoopaJrYahoo& theNerve();
};
class TNerveKoopaJrSubmarineWait : public TNerveBase<TLiveActor> {
public:
	virtual BOOL execute(TSpineBase<TLiveActor>* spine) const;
	// UNUSED - 0x64 size from map
	static const TNerveKoopaJrSubmarineWait& theNerve();
};
class TNerveKoopaJrSubmarineCannonOpenClose : public TNerveBase<TLiveActor> {
public:
	virtual BOOL execute(TSpineBase<TLiveActor>* spine) const;
	// UNUSED - 0x64 size from map
	static const TNerveKoopaJrSubmarineCannonOpenClose& theNerve();
};
class TNerveKoopaJrSubmarineLaunchKiller : public TNerveBase<TLiveActor> {
public:
	virtual BOOL execute(TSpineBase<TLiveActor>* spine) const;
	// UNUSED - 0x64 size from map
	static const TNerveKoopaJrSubmarineLaunchKiller& theNerve();
};

// UNUSED - 0x18 size from map
int TKoopaJr_getJointIndex(int index);

class TKoopaJrParams : public TSpineEnemyParams {
public:
	TKoopaJrParams(const char* name);

public:
	/* 0xA8 */ TParamRT<f32> mSLLaunchKillerLimit;
	/* 0xBC */ TParamRT<f32> mSLDamageRadius;
	/* 0xD0 */ TParamRT<f32> mSLDamageHeight;
	/* 0xE4 */ TParamRT<f32> mSLKoopaJrScale;
	/* 0xF8 */ TParamRT<f32> mSLFastLaunchDistance;
	/* 0x10C */ TParamRT<s32> mSLDamagePeriod;
	/* 0x120 */ TParamRT<s32> mSLLaunchKillerPeriod;
	/* 0x134 */ TParamRT<s32> mSLLaunchKillerPeriodFast;
};

#endif
