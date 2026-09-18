#ifndef ENEMY_KILLER_HPP
#define ENEMY_KILLER_HPP

#include <Enemy/SmallEnemy.hpp>
#include <Enemy/WalkerEnemy.hpp>
#include <Enemy/KillerNerve.hpp>
#include <JSystem/JMath.hpp>
#include <JSystem/JGeometry.hpp>

class TFlyEnemyParams : public TWalkerEnemyParams {
public:
	TFlyEnemyParams(const char* prm);

	f32 getSLNormalFlyGravityY() const { return mSLNormalFlyGravityY.get(); }
	f32 getSLNormalFlySpeed() const { return mSLNormalFlySpeed.get(); }
	f32 getSLChaseFlyGravityY() const { return mSLChaseFlyGravityY.get(); }
	f32 getSLChaseDist() const { return mSLChaseDist.get(); }
	f32 getSLForceGravityY() const { return mSLForceGravityY.get(); }

	/* 0x32C */ TParamRT<f32> mSLNormalFlyGravityY;
	/* 0x340 */ TParamRT<f32> mSLNormalFlySpeed;
	/* 0x354 */ TParamRT<f32> mSLChaseFlyGravityY;
	/* 0x368 */ TParamRT<f32> mSLChaseDist;
	/* 0x37C */ TParamRT<f32> mSLForceGravityY;
};

class TKillerSaveLoadParams : public TFlyEnemyParams {
public:
	TKillerSaveLoadParams(const char* prm);

	f32 getSLWaterAddGravityY() const { return mSLWaterAddGravityY.get(); }
	s32 getSLChaseTimer() const { return mSLChaseTimer.get(); }
	f32 getSLBombRange() const { return mSLBombRange.get(); }

	/* 0x390 */ TParamRT<f32> mSLWaterAddGravityY;
	/* 0x3A4 */ TParamRT<s32> mSLChaseTimer;
	/* 0x3B8 */ TParamRT<f32> mSLBombRange;
};

class TFlyEnemy : public TWalkerEnemy {
public:
	// TFlyEnemy is weak in the map, so the body lives in the class. The
	// default name is the one TKillerManager::createEnemyInstance passes.
	TFlyEnemy(const char* name = "キラー")
	    : TWalkerEnemy(name)
	    , mFlyState(0)
	    , mFlyParams(nullptr)
	    , mFlyTime(0)
	    , unk1A5(true)
	    , mIsGold(false)
	{
	}
	virtual ~TFlyEnemy() { }

	virtual void init(TLiveManager*);
	virtual void bind();
	virtual f32 getGravityY() const;
	virtual void reset();
	virtual void setAfterDeadEffect() { }
	virtual void flyBehavior() { }
	virtual void setChaseFlyAnm() { }
	virtual void setNormalFlyAnm() { }

	void fly();
	void calcChaseParam();
	void flyMove();

	// fabricated
	TFlyEnemyParams* getSaveParam2() const
	{
		return (TFlyEnemyParams*)getSaveParam();
	}

	enum {
		FLY_STATE_NORMAL = 0,
		FLY_STATE_UNK1   = 1,
		FLY_STATE_CHASE  = 2,
	};

	static f32 mTestSp;
	static int mInvalidTime;
	static f32 mTestMarioSpMax;

public:
	/* 0x194 */ f32 mGravityY;
	/* 0x198 */ int mFlyState;
	/* 0x19C */ TFlyEnemyParams* mFlyParams;
	/* 0x1A0 */ int mFlyTime;
	/* 0x1A4 */ bool unk1A4;
	// TODO: cannon.cpp writes unk1A5 and unk1A8 by these names, so they keep
	// them until a batch may also touch that TU. unk1A5 selects the homing
	// ("chase") killer; unk1A8 is the launch velocity the cannon stores and
	// the chase nerve reloads from mVelocity.
	/* 0x1A5 */ bool unk1A5;
	/* 0x1A6 */ bool mIsGold;
	/* 0x1A8 */ JGeometry::TVec3<f32> unk1A8;
};

class TKillerManager : public TSmallEnemyManager {
public:
	TKillerManager(const char* name = "キラーマネージャー");
	virtual ~TKillerManager() { }

	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();
};

class TKiller : public TFlyEnemy {
public:
	TKiller(const char* name = "キラー");
	virtual ~TKiller() { }

	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void bind();
	virtual const char** getBasNameTable() const;
	virtual void reset();
	virtual void genEventCoin();
	virtual void behaveToWater(THitActor*);
	virtual void changeOut();
	virtual void setDeadAnm();
	virtual void attackToMario();
	virtual void forceKill() { }
	virtual void setMActorAndKeeper();
	virtual bool isHitValid(u32);
	virtual bool isCollidMove(THitActor*);
	virtual bool isFindMario(f32);
	virtual void flyBehavior();
	virtual void setChaseFlyAnm();
	virtual void setNormalFlyAnm();

	void setColorType();
	bool isRollFly();

	// fabricated: one inline level over the inherited mSpine, in the
	// TTinKoopa/TBossHanachan shape.
	TSpineBase<TLiveActor>* getSpine() const { return mSpine; }

	// fabricated
	TKillerSaveLoadParams* getSaveParam3() const
	{
		return (TKillerSaveLoadParams*)getSaveParam();
	}

	// TODO: only three of the five bas slots are named by the table; the two
	// gaps are the alphabetically sorted bck files of killer_model1, which is
	// not dumped here yet.
	enum {
		KILLER_ANM_DOWN1   = 0, // downkiller_down1.bas
		KILLER_ANM_UNK1    = 1,
		KILLER_ANM_FLY     = 2,
		KILLER_ANM_SEARCH1 = 3, // killer_search1.bas
	};

	static bool mSerialBomb;
	// TODO: dead-stripped, so only its 1-byte size and .sdata placement (i.e.
	// a non-zero initialiser) are known.
	static u8 mSmokeJntNo;
	static bool mTrampleDie;
	static bool mRollSw;

public:
	/* 0x1B4 */ TKillerSaveLoadParams* mKillerParams;
	/* 0x1B8 */ f32 mRollAngle;
	/* 0x1BC */ TPosition3f mParticleMtx;
	/* 0x1EC */ GXColorS10 mNoseColor;
	/* 0x1F4 */ GXColorS10 mEyesColor;
	/* 0x1FC */ GXColorS10 mBodyColor;
	/* 0x204 */ GXColorS10 mBaseColor;
	/* 0x20C */ f32 mExplosionScaleMax;
};

#endif
