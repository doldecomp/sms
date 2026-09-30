#ifndef ENEMY_POPO_HPP
#define ENEMY_POPO_HPP

#include <Enemy/SmallEnemy.hpp>
#include <Enemy/WalkerEnemy.hpp>
#include <JSystem/JDrama/JDRGraphics.hpp>
#include <Strategic/HitActor.hpp>
#include <Strategic/Nerve.hpp>
#include <Strategic/Spine.hpp>
#include <dolphin/types.h>

class JSUMemoryInputStream;
class TLiveManager;
class TPopoCollision;
class TWaterEmitInfo;

DECLARE_NERVE(TNervePopoThrown, TLiveActor);

DECLARE_NERVE(TNervePopoWait, TLiveActor);

DECLARE_NERVE(TNervePopoExplosion, TLiveActor);

DECLARE_NERVE(TNervePopoFly, TLiveActor);

DECLARE_NERVE(TNervePopoAttack, TLiveActor);

DECLARE_NERVE(TNervePopoPossessedNozzle, TLiveActor);

class TPopoSaveLoadParams : public TWalkerEnemyParams {
public:
	TPopoSaveLoadParams(const char*);

public:
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

class TPopo : public TWalkerEnemy {
public:
	TPopo(const char* name = "ポポ");

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);
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
	virtual bool isFindMario(f32);
	virtual void behaveToFindMario();
	virtual void walkBehavior(int, f32);

	void thrownByChorobei();
	void explosionEffect();
	void possessedIn();
	void explosion();
	void flyBehavior();
	void releaseNozzle();
	bool isUseScaleCallBack();
	bool isRollJump();
	bool checkTrigger();
	static u8 mRollSw;
	static u8 mTriggerSw;
	static f32 mTestAng_x;
	static f32 mTestAng_y;
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
	static f32 mTestAng_z;
	static u8 mExplosionSw;

public:
	/* 0x194 */ TPopoSaveLoadParams* unk194;
	/* 0x198 */ f32 unk198;
	/* 0x19C */ s32 unk19C;
	/* 0x1A0 */ f32 unk1A0;
	/* 0x1A4 */ u8 unk1A4;
	/* 0x1A5 */ char unk1A5[0x3];
	/* 0x1A8 */ JGeometry::TVec3<f32> unk1A8;
	/* 0x1B4 */ u8 unk1B4;
	/* 0x1B5 */ char unk1B5[0x3];
	/* 0x1B8 */ f32 unk1B8;
	/* 0x1BC */ u8 unk1BC;
	/* 0x1BD */ char unk1BD[0x3];
	/* 0x1C0 */ JGeometry::TVec3<f32> unk1C0;
	/* 0x1CC */ u8 unk1CC;
	/* 0x1CD */ u8 unk1CD;
	/* 0x1CE */ char unk1CE[2];
	/* 0x1D0 */ Mtx unk1D0;
	/* 0x200 */ Mtx unk200;
	/* 0x230 */ JGeometry::TVec3<f32> unk230;
	/* 0x23C */ TPopoCollision* unk23C;
};

class TPopoCollision : public THitActor {
public:
	TPopoCollision(const char* name = "ポポコリジョン")
	    : THitActor(name)
	{
	}
	virtual BOOL receiveMessage(THitActor*, u32);

	void kill();
	void checkHit();

public:
	/* 0x68 */ TPopo* unk68;
};

class TPopoManager : public TSmallEnemyManager {
public:
	TPopoManager(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();
	virtual void initSetEnemies();

public:
	/* 0x60 */ u8 unk60;
	/* 0x61 */ char unk61[0x3];
	/* 0x64 */ TWaterEmitInfo* unk64;
	/* 0x68 */ TWaterEmitInfo* unk68;
};

#endif // ENEMY_POPO_HPP
