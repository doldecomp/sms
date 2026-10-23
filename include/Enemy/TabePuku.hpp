#ifndef ENEMY_TABEPUKU_HPP
#define ENEMY_TABEPUKU_HPP

#include <Enemy/SmallEnemy.hpp>
#include <JSystem/JDrama/JDRGraphics.hpp>
#include <JSystem/JGeometry.hpp>
#include <JSystem/JGeometry/JGQuat4.hpp>
#include <JSystem/JGeometry/JGVec3.hpp>
#include <Strategic/HitActor.hpp>
#include <Strategic/Nerve.hpp>
#include <Strategic/Spine.hpp>
#include <System/ParamInst.hpp>
#include <dolphin/types.h>

class JSUMemoryInputStream;
class TLiveManager;
class TTPHitActor;

DECLARE_NERVE(TNerveTabePukuDrag, TLiveActor);

DECLARE_NERVE(TNerveTabePukuRecoverGraph, TLiveActor);

DECLARE_NERVE(TNerveTabePukuDive, TLiveActor);

DECLARE_NERVE(TNerveTabePukuBite, TLiveActor);

DECLARE_NERVE(TNerveTabePukuAttack, TLiveActor);

DECLARE_NERVE(TNerveTabePukuGraphWander, TLiveActor);

DECLARE_NERVE(TNerveTabePukuFound, TLiveActor);

class TTabePukuManager : public TSmallEnemyManager {
public:
	TTabePukuManager(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
};

class TTabePuku : public TSmallEnemy {
public:
	TTabePuku(const char*);

	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual MtxPtr getTakingMtx();
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void control();
	virtual void bind();
	virtual const char** getBasNameTable() const;
	virtual void reset();
	virtual void behaveToWater(THitActor*);
	virtual void attackToMario();
	virtual void forceKill();
	virtual bool doKeepDistance();
	virtual bool isFindMario(f32);

	void detach();
	void setMomentumFromQuat();
	void swimTo(const JGeometry::TVec3<f32>&);
	bool doDrag();
	void prepareDrag();
	bool doDive();
	void prepareDive();
	void doBite();
	void swimToCurPathNode(const JGeometry::TVec3<f32>&);
	bool isBiting() const;
	bool isAttacking() const;
	bool isGraphWander() const;
	bool isTouchedPlane() const;
	bool isMissMario() const;
	void emitEffects();
	void updateSound();
	void initParams();
	void initCollision();

public:
	/* 0x194 */ TTPHitActor* unk194;
	/* 0x198 */ JGeometry::TQuat4<f32> mQuat;
	/* 0x1A8 */ TPosition3f mTakeMtx;
	/* 0x1D8 */ s32 mMouthJointIndex;
	/* 0x1DC */ u8 mTouchedWall;
	/* 0x1E0 */ f32 mDiveStartY;
	/* 0x1E4 */ JGeometry::TVec3<f32> mDragDir;
};

class TTPHitActor : public THitActor {
public:
	TTPHitActor(TTabePuku&);

	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void init();

	void checkHitActors();
	void bind();
	void updateTerrainCollsion();
	void updateObjCollision();

public:
	/* 0x68 */ TTabePuku* mOwner;
	/* 0x6C */ JGeometry::TVec3<f32> mVelocity;
	/* 0x78 */ f32 mCheckHeight;
	/* 0x7C */ f32 mCheckRadius;
	/* 0x80 */ f32 mGroundHeight;
	/* 0x84 */ const TBGCheckData* mGroundPlane;
	/* 0x88 */ u8 mAirborne;
	/* 0x89 */ u8 mTouchedWall;
};

class TTabePukuParams : public TSmallEnemyParams {
public:
	TTabePukuParams(const char*);

	/* 0x2D4 */ TParamRT<f32> mMarchSpeed;
	/* 0x2E8 */ TParamRT<f32> mAttackSpeed;
	/* 0x2FC */ TParamRT<f32> mDiveSpeed;
	/* 0x310 */ TParamRT<f32> mWaterFric;
	/* 0x324 */ TParamRT<f32> mTurnSlerpRate;
	/* 0x338 */ TParamRT<f32> mApartHeight;
	/* 0x34C */ TParamRT<f32> mCorrectY;
	/* 0x360 */ TParamRT<f32> mCorrectZ;
	/* 0x374 */ TParamRT<f32> mTerritoryRange;
	/* 0x388 */ TParamRT<f32> mDragLength;
};

#endif // ENEMY_TABEPUKU_HPP
