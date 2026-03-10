#ifndef ENEMY_AMENBO_HPP
#define ENEMY_AMENBO_HPP

#include <Enemy/SmallEnemy.hpp>
#include <Strategic/Nerve.hpp>

class TAmenboParams;
class TMapCollisionWarp;
class TMirrorActor;

class TAmenbo : public TSmallEnemy {
public:
	TAmenbo(const char*);

	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void control();
	virtual void bind();
	virtual void kill();
	virtual const char** getBasNameTable() const;
	virtual void reset();
	virtual void behaveToWater(THitActor*);
	virtual bool changeByJuice();
	virtual void setDeadAnm();
	virtual void attackToMario();
	virtual void forceKill();
	virtual bool isCollidMove(THitActor*);
	virtual bool doKeepDistance();

	void initCollision();
	void initParticle();
	void checkMarioWaterIn();
	void updateCollision();
	void updateRipple();
	void changeBck(const char*, f32);
	void prepareWalk();
	bool doWalk();
	void doAdjustTarget();
	void doChangeWaitAnm();
	void decideTarget();
	void decideTargetOnFingingMario();
	void setWalkDir(const JGeometry::TVec3<f32>&);
	void setWalkDir(const JGeometry::TQuat4<f32>&);
	bool isWalking() const;
	bool isStartMoving() const;
	bool isFindOutMario() const;
	bool isOverTerritory(JGeometry::TVec3<f32>*) const;
	bool isAttacking() const;
	bool isFreeze() const;
	bool isChangedBlock() const;
	bool isWaterFromWaterGun(THitActor*);
	void activateJumpBase();
	void deactivateJumpBase();
	bool isActivateJumpBase() const;

	const JGeometry::TQuat4<f32>& getQuat() const { return mQuat; }

	// fabricated
	TAmenboParams* getSaveParam2() const
	{
		return (TAmenboParams*)getSaveParam();
	}

public:
	struct EffectOnJoint {
		/* 0x0 */ int mJointIdx;
		/* 0x4 */ JGeometry::TVec3<f32> mPos;
	};

	/* 0x194 */ JGeometry::TVec3<f32> mInitialPosition;
	/* 0x1A0 */ bool mIsChasingMario;
	/* 0x1A4 */ int mWaterGunHitCooldown;
	/* 0x1A8 */ int mSearchDisableCooldown;
	/* 0x1AC */ int mOutOfWaterDeathTimer;
	/* 0x1B0 */ JGeometry::TQuat4<f32> mQuat;
	/* 0x1C0 */ JGeometry::TQuat4<f32> unk1C0;
	/* 0x1D0 */ JGeometry::TQuat4<f32> unk1D0;
	/* 0x1E0 */ f32 unk1E0;
	/* 0x1E4 */ TMapCollisionWarp* unk1E4;
	/* 0x1E8 */ TMirrorActor* unk1E8;
	/* 0x1EC */ EffectOnJoint unk1EC[4];
};

class TAmenboParams : public TSmallEnemyParams {
public:
	TAmenboParams(const char*);

	/* 0x2D4 */ TParamRT<f32> mSpeed;
	/* 0x2E8 */ TParamRT<f32> mAttackSpeed;
	/* 0x2FC */ TParamRT<f32> mWaterFric;
	/* 0x310 */ TParamRT<f32> mEpsilon;
	/* 0x324 */ TParamRT<s32> mIsNotSearchTimerMax;
	/* 0x338 */ TParamRT<f32> mSearchRange;
	/* 0x34C */ TParamRT<f32> mSearchHeight;
	/* 0x360 */ TParamRT<f32> mTerritoryRange;
	/* 0x374 */ TParamRT<s32> mHitWaterTimer;
	/* 0x388 */ TParamRT<s32> mPreAttackTimer;
};

class TAmenboManager : public TSmallEnemyManager {
public:
	TAmenboManager(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
};

DECLARE_NERVE(TNerveAmenboSearch, TLiveActor);
DECLARE_NERVE(TNerveAmenboTurn, TLiveActor);
DECLARE_NERVE(TNerveAmenboPreAttack, TLiveActor);
DECLARE_NERVE(TNerveAmenboWalk, TLiveActor);
DECLARE_NERVE(TNerveAmenboHitWater, TLiveActor);

#endif
