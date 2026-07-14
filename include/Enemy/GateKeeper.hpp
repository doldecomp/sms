#ifndef ENEMY_GATE_KEEPER_HPP
#define ENEMY_GATE_KEEPER_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <M3DUtil/M3UJoint.hpp>

class TMultiBtk;

class TGateKeeperParams : public TSpineEnemyParams {
public:
	TGateKeeperParams(const char* path);

public:
	/* 0xA8 */ TParamRT<s32> mSLDiveTimer;
	/* 0xBC */ TParamRT<s32> mSLLoop2Dive;
	/* 0xD0 */ TParamRT<s32> mSLLaunchTimerNormal;
	/* 0xE4 */ TParamRT<s32> mSLLaunchTimerDamage;
};

class TGateKeeperBase;

class TGKHitObj : public THitActor {
public:
	TGKHitObj(TGateKeeperBase*, int, const char*);

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor*, u32);

public:
	/* 0x68 */ TGateKeeperBase* mOwner;
	/* 0x6C */ int mJointIndex;
	/* 0x70 */ BOOL mVulnerable;
};

class TGateKeeperBase : public TSpineEnemy {
public:
	TGateKeeperBase(const char*);

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void kill();

public:
	/* 0x150 */ MActor* mStampModel;
	/* 0x154 */ int unk154;
	/* 0x158 */ f32 unk158;
	/* 0x15C */ TMultiBtk* mMultiBtk;
	/* 0x160 */ s8 mStampEnabled;
	/* 0x161 */ s8 mVulnerable;
	/* 0x164 */ JGeometry::TVec3<f32> mStampScale;
};

class TBiancoGateKeeperManager : public TEnemyManager {
public:
	TBiancoGateKeeperManager(const char*);
	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
	virtual void initJParticle();
};

class TBiancoGateKeeper;

class TBGKMtxCalc : public M3UMtxCalcSIAnmBlendQuat {
public:
	TBGKMtxCalc(TBiancoGateKeeper*);

	virtual void calc(u16);

	void joinAnm(int);
	void setAnm(int);

public:
	/* 0x64 */ TBiancoGateKeeper* mOwner;
};

class TBGKObstacle : public THitActor {
public:
	TBGKObstacle(TBiancoGateKeeper*, const char*);
};

class TBiancoGateKeeper : public TGateKeeperBase {
public:
	TBiancoGateKeeper(const char*);

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual void init(TLiveManager*);
	virtual void kill();
	virtual const char** getBasNameTable() const;

	void changeBck(int);
	void launchGorogoro();
	void launchNamekuri();
	f32 getRumblePow();
	void rumblePad();
	void deathRumble();
	BOOL curBckFinished() const;
	void startBGM();
	void stopBGM();
	void startAppearDemo();
	void startFinishDemo();
	BOOL isHeadHitActive() const;
	BOOL isDamageFogSituation() const;
	void emitParticles();
	void controlCollision();

	TGateKeeperParams* getSaveParams()
	{
		return (TGateKeeperParams*)getSaveParam();
	}

	bool checkUnk290() const { return unk290 >= 0; }
	void resetUnk290() { unk290 = 0; }
	void tickUnk290() { unk290 += 1; }

	f32 getUnk180() const { return unk180; }

public:
	enum {
		VARIANT_AIRPORT          = 0,
		VARIANT_GENERIC          = 1,
		VARIANT_DOLPIC_TOWN      = 2,
		VARIANT_RICO_GATEKEEPER  = 3,
		VARIANT_MAMMA_GATEKEEPER = 4,
	};

	/* 0x170 */ u32 unk170;
	/* 0x174 */ TGKHitObj* mHead;
	/* 0x178 */ TBGKMtxCalc* unk178;
	/* 0x17C */ s16 unk17C;
	/* 0x17E */ s16 unk17E;
	/* 0x180 */ f32 unk180;
	/* 0x184 */ u32 unk184;
	/* 0x188 */ char mDemoName[256];
	/* 0x288 */ s16 unk288;
	/* 0x28A */ s16 unk28A;
	/* 0x28C */ TBGKObstacle* mObstacle;
	/* 0x290 */ s16 unk290;
	/* 0x292 */ s8 mVariant;
	/* 0x293 */ s8 mHintShown;
	/* 0x294 */ s16 mHintTimer;
	/* 0x296 */ s8 unk296;
	/* 0x298 */ s16 unk298;
	/* 0x29C */ f32 mRumblePower;
};

DECLARE_NERVE(TNerveBGKSleep, TLiveActor);
DECLARE_NERVE(TNerveBGKAppear, TLiveActor);
DECLARE_NERVE(TNerveBGKWait, TLiveActor);
DECLARE_NERVE(TNerveBGKWait2, TLiveActor);
DECLARE_NERVE(TNerveBGKSleepDamage, TLiveActor);
DECLARE_NERVE(TNerveBGKAwakeDamage, TLiveActor);
DECLARE_NERVE(TNerveBGKDie, TLiveActor);
DECLARE_NERVE(TNerveBGKDive, TLiveActor);
DECLARE_NERVE(TNerveBGKLaunchGoro, TLiveActor);
DECLARE_NERVE(TNerveBGKLaunchName, TLiveActor);

#endif
