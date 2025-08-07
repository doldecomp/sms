#ifndef ENEMY_MAME_GESSO_HPP
#define ENEMY_MAME_GESSO_HPP

#include <Enemy/SmallEnemy.hpp>
#include <Enemy/WalkerEnemy.hpp>

class TMapCollisionMove;

class TMameGessoSaveLoadParams : public TWalkerEnemyParams {
public:
	TMameGessoSaveLoadParams(const char* path);

	/* 0x32C */ TParamRT<f32> mSLJumpWanderGravityY;
	/* 0x340 */ TParamRT<f32> mSLJumpWanderSpeed;
	/* 0x354 */ TParamRT<s32> mSLPickUpTime;
	/* 0x368 */ TParamRT<f32> mSLThrownGravityY;
	/* 0x37C */ TParamRT<f32> mSLThrownVY;
	/* 0x390 */ TParamRT<f32> mSLThrownRateXZ;
	/* 0x3A4 */ TParamRT<s32> mSLGenerateInterval;
	/* 0x3B8 */ TParamRT<s32> mSLWaitTimeOnGround;
	/* 0x3CC */ TParamRT<s32> mSLWaitTimeInWater;
	/* 0x3E0 */ TParamRT<s32> mSLObjectRecoverTime;
	/* 0x3F4 */ TParamRT<f32> mSLObjectScale;
	/* 0x408 */ TParamRT<f32> mSLCollisionScale;
	/* 0x41C */ TParamRT<f32> mSLReturnJumpDist;
	/* 0x430 */ TParamRT<f32> mSLReturnJumpSp;
};

class TMameGesso;

class TMameGessoManager : public TSmallEnemyManager {
public:
	TMameGessoManager(const char* name);

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);
	virtual TSmallEnemy* createEnemyInstance();
	virtual void initSetEnemies();

	TMameGesso* getObj(int i) { return (TMameGesso*)TObjManager::getObj(i); }
};

class TMameGesso : public TWalkerEnemy {
public:
	TMameGesso(const char* name = "まめゲッソー");

	virtual void load(JSUMemoryInputStream&);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void moveObject();
	virtual void kill();
	virtual f32 getGravityY() const;
	virtual const char** getBasNameTable() const;
	virtual void reset();
	virtual bool isEatenByYosshi() { return false; }
	virtual void behaveToWater(THitActor*);
	virtual bool changeByJuice() { return false; }
	virtual void behaveToTaken(THitActor*);
	virtual void behaveToRelease();
	virtual void setDeadAnm();
	virtual void setFreezeAnm();
	virtual void setWaitAnm();
	virtual void attackToMario();
	virtual void forceKill() { }
	virtual void setMActorAndKeeper();
	virtual bool isHitValid(u32);
	virtual bool isCollidMove(THitActor*);
	virtual BOOL isInhibitedForceMove();
	virtual bool doKeepDistance();

	void rebirth();
	void calcObjCollision();
	void entryObjCollision();
	void checkMarioState();

	TMameGessoSaveLoadParams* getSaveLoadParam() const
	{
		return (TMameGessoSaveLoadParams*)getSaveParam();
	}

	static f32 mBaseHoseiTransY;

public:
	/* 0x194 */ TMameGessoSaveLoadParams* unk194;
	/* 0x198 */ TMapCollisionMove* unk198;
	/* 0x19C */ JGeometry::TVec3<f32> unk19C[4];
	/* 0x1CC */ int unk1CC;
	/* 0x1D0 */ u8 unk1D0;
	/* 0x1D1 */ u8 unk1D1;
	/* 0x1D2 */ u8 unk1D2;
	/* 0x1D4 */ JGeometry::TVec3<f32> unk1D4;
	/* 0x1E0 */ f32 unk1E0;
	/* 0x1E4 */ f32 unk1E4;
	/* 0x1E8 */ f32 unk1E8;
	/* 0x1EC */ u8 unk1EC;
};

DECLARE_NERVE(TNerveMameGessoGraphJumpWander, TLiveActor);
DECLARE_NERVE(TNerveMameGessoDamage, TLiveActor);
DECLARE_NERVE(TNerveMameGessoJitabata, TLiveActor);
DECLARE_NERVE(TNerveMameGessoTrample, TLiveActor);
DECLARE_NERVE(TNerveMameGessoPickUp, TLiveActor);
DECLARE_NERVE(TNerveMameGessoThrown, TLiveActor);
DECLARE_NERVE(TNerveMameGessoObject, TLiveActor);
DECLARE_NERVE(TNerveMameGessoWait, TLiveActor);

#endif
