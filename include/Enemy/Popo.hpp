#ifndef ENEMY_POPO_HPP
#define ENEMY_POPO_HPP

#include <Enemy/WalkerEnemy.hpp>
#include <Strategic/Nerve.hpp>

class TPopo;
class TWaterEmitInfo;

// Names and defaults are the ones PARAM_INIT stringified into .rodata.
class TPopoSaveLoadParams : public TWalkerEnemyParams {
public:
	TPopoSaveLoadParams(const char* prm);

	f32 getSLMoveDist() const { return mSLMoveDist.get(); }
	f32 getSLMoveGravity() const { return mSLMoveGravity.get(); }
	f32 getSLMoveJumpSp() const { return mSLMoveJumpSp.get(); }
	f32 getSLAttackDist() const { return mSLAttackDist.get(); }
	f32 getSLAttackGravity() const { return mSLAttackGravity.get(); }
	f32 getSLAttackJumpSp() const { return mSLAttackJumpSp.get(); }
	f32 getSLReleaseSpeed() const { return mSLReleaseSpeed.get(); }
	f32 getSLFlyGravity() const { return mSLFlyGravity.get(); }
	s32 getSLFlyLimitTime() const { return mSLFlyLimitTime.get(); }
	s32 getSLExplosionEmitTime() const { return mSLExplosionEmitTime.get(); }
	f32 getSLWaterScaleMax() const { return mSLWaterScaleMax.get(); }
	f32 getSLThrownGravity() const { return mSLThrownGravity.get(); }
	f32 getSLPumpRate() const { return mSLPumpRate.get(); }
	f32 getSLLevelLimit() const { return mSLLevelLimit.get(); }
	f32 getSLScaleRate() const { return mSLScaleRate.get(); }

	/* 0x32C */ TParamRT<f32> mSLMoveDist;
	/* 0x340 */ TParamRT<f32> mSLMoveGravity;
	/* 0x354 */ TParamRT<f32> mSLMoveJumpSp;
	/* 0x368 */ TParamRT<f32> mSLAttackDist;
	/* 0x37C */ TParamRT<f32> mSLAttackGravity;
	/* 0x390 */ TParamRT<f32> mSLAttackJumpSp;
	/* 0x3A4 */ TParamRT<f32> mSLReleaseSpeed;
	/* 0x3B8 */ TParamRT<f32> mSLFlyGravity;
	/* 0x3CC */ TParamRT<s32> mSLFlyLimitTime;
	/* 0x3E0 */ TParamRT<s32> mSLExplosionEmitTime;
	/* 0x3F4 */ TParamRT<f32> mSLWaterScaleMax;
	/* 0x408 */ TParamRT<f32> mSLThrownGravity;
	/* 0x41C */ TParamRT<f32> mSLPumpRate;
	/* 0x430 */ TParamRT<f32> mSLLevelLimit;
	/* 0x444 */ TParamRT<f32> mSLScaleRate;
};

// The hit box that stands in for the popo while it is riding the nozzle.
class TPopoCollision : public THitActor {
public:
	TPopoCollision(const char* name)
	    : THitActor(name)
	{
	}

	virtual ~TPopoCollision() { }
	virtual BOOL receiveMessage(THitActor* sender, u32 message);

	void kill();
	void checkHit();

	/* 0x68 */ TPopo* mPopo;
};

class TPopo : public TWalkerEnemy {
public:
	TPopo(const char* name);

	virtual ~TPopo() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void bind();
	virtual void kill();
	virtual f32 getGravityY() const;
	virtual const char** getBasNameTable() const;
	virtual void reset();
	virtual void behaveToWater(THitActor*);
	virtual void attackToMario();
	virtual void forceKill();
	virtual void setMActorAndKeeper();
	virtual bool isHitValid(u32);
	virtual bool isCollidMove(THitActor*);
	virtual bool isFindMario(float);
	virtual void behaveToFindMario();
	virtual void walkBehavior(int, float);

	void thrownByChorobei();
	void explosionEffect();
	void possessedIn();
	void explosion();
	void flyBehavior();
	void releaseNozzle();
	bool isUseScaleCallBack();
	bool isRollJump();
	bool checkTrigger();

	// fabricated
	TPopoSaveLoadParams* getSaveParams() const { return mSaveParams; }
	TPopoCollision* getCollision() const { return mCollision; }

	static u8 mRollSw;
	static u8 mTriggerSw;
	static f32 mTestAng_x;
	static f32 mTestAng_y;
	static f32 mTestAng_z;
	static f32 mNozzleOffsetZ;
	static u8 mCenterJntIndex;
	static u8 mMouthJntIndex;
	static u8 mRLegJntIndex;
	static u8 mLLegJntIndex;
	static u8 mRHandJntIndex;
	static u8 mLHandJntIndex;
	static f32 mTestBodyScale;
	static u8 mBrkFlag;
	static f32 mColOffsetY;
	static f32 mColMinVal;
	static u8 mLevelShootSw;
	static u8 mExplosionSw;

	/* 0x194 */ TPopoSaveLoadParams* mSaveParams;
	/* 0x198 */ f32 mPumpScale;
	/* 0x19C */ int mFlyTimer;
	/* 0x1A0 */ f32 mBrkFrames;
	/* 0x1A4 */ u8 mIsLoaded;
	/* 0x1A8 */ JGeometry::TVec3<f32> mInitialPos;
	/* 0x1B4 */ u8 mIsPossessed;
	/* 0x1B8 */ f32 mRollAngle;
	/* 0x1BC */ u8 mIsPumping;
	/* 0x1C0 */ JGeometry::TVec3<f32> mEffectPos;
	/* 0x1CC */ u8 mIsLevelReached;
	/* 0x1CD */ u8 unk1CD;
	/* 0x1D0 */ Mtx mMouthMtx;
	/* 0x200 */ Mtx mPumpMtx;
	/* 0x230 */ JGeometry::TVec3<f32> mEffectScale;
	/* 0x23C */ TPopoCollision* mCollision;
};

class TPopoManager : public TSmallEnemyManager {
public:
	TPopoManager(const char* name);

	virtual ~TPopoManager() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();
	virtual void initSetEnemies();

	/* 0x60 */ u8 mIsNozzleFree;
	/* 0x64 */ TWaterEmitInfo* mFlyWater;
	/* 0x68 */ TWaterEmitInfo* mExplosionWater;
};

extern TPopo* gpCurPopo;

DECLARE_NERVE(TNervePopoAttack, TLiveActor)
DECLARE_NERVE(TNervePopoExplosion, TLiveActor)
DECLARE_NERVE(TNervePopoFly, TLiveActor)
DECLARE_NERVE(TNervePopoPossessedNozzle, TLiveActor)
DECLARE_NERVE(TNervePopoThrown, TLiveActor)
DECLARE_NERVE(TNervePopoWait, TLiveActor)

#endif
