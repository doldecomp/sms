#ifndef ENEMY_LIMITKOOPAJR_HPP
#define ENEMY_LIMITKOOPAJR_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Enemy/KoopaJr.hpp>
#include <JSystem/JDrama/JDRGraphics.hpp>
#include <JSystem/JGeometry/JGVec3.hpp>
#include <Strategic/Nerve.hpp>
#include <Strategic/Spine.hpp>
#include <System/BaseParam.hpp>
#include <dolphin/types.h>

class JSUMemoryInputStream;
class THitActor;
class TLiveManager;
class TBathtub;
class TKoopa;

class TLimitKoopaJrManager : public TEnemyManager {
public:
	TLimitKoopaJrManager(const char* name
	                     = "リミットクッパジュニアマネージャー");

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();
};

DECLARE_NERVE(TNerveLimitKoopaJrYahoo, TLiveActor);

DECLARE_NERVE(TNerveLimitKoopaJrWait, TLiveActor);

DECLARE_NERVE(TNerveLimitKoopaJrLaunch, TLiveActor);

DECLARE_NERVE(TNerveLimitKoopaJrRun, TLiveActor);

class TLimitKoopaJr : public TSpineEnemy {
public:
	TLimitKoopaJr(const char* name = "リミットクッパジュニア");

	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void bind();
	virtual const char** getBasNameTable() const;
	virtual void reset();

	void makeDirection(JGeometry::TVec3<f32>);
	f32 calcTargetDirection();
	void moveWait();
	bool canYahoo();
	bool canRun();
	void moveRun();
	void checkNerve();
	void updateTimers();
	void setAnimationIndex(int);
	void emitKoopaJrEffects();
	void startKoopaJrMessage(u32);
	void resetLimitKoopaJr();

public:
	/* 0x150 */ TBathtub* mBathtub;
	/* 0x154 */ TKoopa* mKoopa;
	/* 0x158 */ s32 unk158[2];
	/* 0x160 */ f32 unk160;
	/* 0x164 */ f32 unk164;
	/* 0x168 */ f32 unk168;
	/* 0x16C */ f32 unk16C;
	/* 0x170 */ TDirectionCalc unk170;
	/* 0x174 */ f32 mRoundRadius;
	/* 0x178 */ TDirectionCalc unk178;
};

class TLimitKoopaJrParams : public TSpineEnemyParams {
public:
	TLimitKoopaJrParams(const char*);

public:
	/* 0xA8 */ TParamRT<f32> mSLAcceleration;
	/* 0xBC */ TParamRT<f32> mSLRotationSpeed;
	/* 0xD0 */ TParamRT<f32> mSLSpeedMax;
	/* 0xE4 */ TParamRT<f32> mSLRoundAngleVelocity;
	/* 0xF8 */ TParamRT<f32> mSLRoundRadius;
	/* 0x10C */ TParamRT<f32> mSLRoundHeight;
	/* 0x120 */ TParamRT<f32> mSLDamageRadius;
	/* 0x134 */ TParamRT<f32> mSLDamageHeight;
	/* 0x148 */ TParamRT<f32> mSLKoopaJrScale;
	/* 0x15C */ TParamRT<s32> mSLShotDoodlePeriod;
	/* 0x170 */ TParamRT<s32> mSLDamagePeriod;
};

#endif // ENEMY_LIMITKOOPAJR_HPP
