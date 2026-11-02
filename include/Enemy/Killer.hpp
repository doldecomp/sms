#ifndef ENEMY_KILLER_HPP
#define ENEMY_KILLER_HPP

#include <Enemy/WalkerEnemy.hpp>

class TFlyEnemyParams : public TWalkerEnemyParams {
public:
	TFlyEnemyParams(const char*);
	~TFlyEnemyParams();

	/* 0x32C */ TParamRT<f32> mSLNormalFlyGravityY;
	/* 0x340 */ TParamRT<f32> mSLNormalFlySpeed;
	/* 0x354 */ TParamRT<f32> mSLChaseFlyGravityY;
	/* 0x368 */ TParamRT<f32> mSLChaseDist;
	/* 0x37C */ TParamRT<f32> mSLForceGravityY;
};

class TKillerSaveLoadParams : public TFlyEnemyParams {
public:
	TKillerSaveLoadParams(const char*);

	/* 0x390 */ TParamRT<f32> mSLWaterAddGravityY;
	/* 0x3A4 */ TParamRT<s32> mSLChaseTimer;
	/* 0x3B8 */ TParamRT<f32> mSLBombRange;
};

class TKillerManager : public TSmallEnemyManager {
public:
	TKillerManager(const char* = "キラーマネージャー");
	virtual ~TKillerManager() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();
};

class TFlyEnemy : public TWalkerEnemy {
public:
	enum FlyMode {
		FLY_MODE_DIVE    = 0,
		FLY_MODE_NEUTRAL = 1,
		FLY_MODE_HOMING  = 2,
	};

	TFlyEnemy(const char* name)
	    : TWalkerEnemy(name)
	    , mFlyMode(FLY_MODE_DIVE)
	    , mFlyParams(nullptr)
	    , mFlyTimer(0)
	    , mIsHoming(true)
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

	void flyMove();
	void calcChaseParam();
	void fly();

	FlyMode getFlyMode() const { return mFlyMode; }
	TFlyEnemyParams* getFlyParams() const { return mFlyParams; }
	s32 getFlyTimer() const { return mFlyTimer; }
	bool isForceChase() const { return mForceChase; }
	bool isHoming() const { return mIsHoming; }
	void setHoming(bool homing) { mIsHoming = homing; }
	bool isGold() const { return mIsGold; }

	static f32 mTestSp;
	static s32 mInvalidTime;
	static f32 mTestMarioSpMax;

public:
	/* 0x194 */ f32 mChaseGravityY;
	/* 0x198 */ FlyMode mFlyMode;
	/* 0x19C */ TFlyEnemyParams* mFlyParams;
	/* 0x1A0 */ s32 mFlyTimer;
	/* 0x1A4 */ bool mForceChase;
	/* 0x1A5 */ bool mIsHoming;
	/* 0x1A6 */ bool mIsGold;
	/* 0x1A8 */ JGeometry::TVec3<f32> mLaunchVelocity;
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

	f32 getRollAngle() const { return mRollAngle; }

	static bool mSerialBomb;
	static u8 mSmokeJntNo;
	static bool mTrampleDie;
	static bool mRollSw;

public:
	/* 0x1B4 */ TKillerSaveLoadParams* mKillerParams;
	/* 0x1B8 */ f32 mRollAngle;
	/* 0x1BC */ TPosition3f mTrailEmitterTransform;
	/* 0x1EC */ GXColorS10 mNoseColor;
	/* 0x1F4 */ GXColorS10 mEyesColor;
	/* 0x1FC */ GXColorS10 mBodyColor;
	/* 0x204 */ GXColorS10 mDownBodyColor;
	/* 0x20C */ f32 mBombScaleMax;
};

extern TKiller* gpCurKiller;

DECLARE_NERVE(TNerveFlyEnemyNormalFly, TLiveActor);
DECLARE_NERVE(TNerveFlyEnemyChaseFly, TLiveActor);
DECLARE_NERVE(TNerveKillerExplosion, TLiveActor);

#endif
