#ifndef ENEMY_TABEPUKUNERVE_HPP
#define ENEMY_TABEPUKUNERVE_HPP

#include <Enemy/SmallEnemy.hpp>
#include <Strategic/Nerve.hpp>
#include <JSystem/JGeometry/JGPosition3.hpp>

class TLiveActor;
class TTabePuku;

// The big pufferfish that swallows Mario and drags him along the sea floor.
// Names and defaults are the ones PARAM_INIT stringified into .rodata and the
// constants TTabePukuManager::load stores into each TParamRT.
class TTabePukuParams : public TSmallEnemyParams {
public:
	TTabePukuParams(const char* prm);

	f32 getMarchSpeed() const { return mMarchSpeed.get(); }
	f32 getAttackSpeed() const { return mAttackSpeed.get(); }
	f32 getDiveSpeed() const { return mDiveSpeed.get(); }
	f32 getWaterFric() const { return mWaterFric.get(); }
	f32 getTurnSlerpRate() const { return mTurnSlerpRate.get(); }
	f32 getApartHeight() const { return mApartHeight.get(); }
	f32 getCorrectY() const { return mCorrectY.get(); }
	f32 getCorrectZ() const { return mCorrectZ.get(); }
	f32 getTerritoryRange() const { return mTerritoryRange.get(); }
	f32 getDragLength() const { return mDragLength.get(); }

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

// The mouth of the pufferfish: a hit actor that carries its own little binder
// (ground height, wall radius, velocity) so that the bite volume can lead the
// body and still be pushed out of the terrain.
class TTPHitActor : public THitActor {
public:
	TTPHitActor(TTabePuku& owner);

	// Declared in vtable order; only init() is a new slot (0xa4).
	virtual ~TTPHitActor() { }
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void init();

	void checkHitActors();
	void updateObjCollision();
	void updateTerrainCollsion();
	void bind();

	/* 0x68 */ TTabePuku* mOwner;
	/* 0x6C */ JGeometry::TVec3<f32> mVelocity;
	// Half the bite height: how far below the owner the mouth sits, and the
	// Y offset the ground check uses.
	/* 0x78 */ f32 mCheckHeight;
	/* 0x7C */ f32 mCheckRadius;
	/* 0x80 */ f32 mGroundHeight;
	/* 0x84 */ const TBGCheckData* mGroundPlane;
	/* 0x88 */ u8 mAirborne;
	/* 0x89 */ u8 mTouchedWall;
};

class TTabePuku : public TSmallEnemy {
public:
	TTabePuku(const char* name);

	// Declared in vtable order; TTabePuku adds no new slots to TSmallEnemy.
	virtual ~TTabePuku() { }
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
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

	void initParams();
	void initCollision();
	void updateSound();
	void emitEffects();
	bool isMissMario() const;
	bool isTouchedPlane() const;
	bool isGraphWander() const;
	bool isAttacking() const;
	bool isBiting() const;
	void swimToCurPathNode(const JGeometry::TVec3<f32>& offset);
	void doBite();
	void prepareDive();
	bool doDive();
	void prepareDrag();
	bool doDrag();
	void swimTo(const JGeometry::TVec3<f32>& dir);
	void setMomentumFromQuat();
	void detach();

	// fabricated
	TTabePukuParams* getSaveParams() const
	{
		return (TTabePukuParams*)getSaveParam();
	}

	/* 0x194 */ TTPHitActor* mMouthHit;
	/* 0x198 */ JGeometry::TQuat4<f32> mQuat;
	// The matrix Mario is parented to while he is being swallowed. It is a
	// member because getTakingMtx() has to hand out a pointer to it.
	/* 0x1A8 */ JGeometry::TPosition3<
	    JGeometry::TMatrix34<JGeometry::SMatrix34C<f32> > >
	    mTakingMtx;
	/* 0x1D8 */ int mMouthJointIndex;
	// Mirror of the mouth actor's wall-touch result, copied every bind().
	/* 0x1DC */ u8 mTouchedWall;
	/* 0x1E0 */ f32 mDiveStartY;
	/* 0x1E4 */ JGeometry::TVec3<f32> mDragDir;
};

class TTabePukuManager : public TSmallEnemyManager {
public:
	TTabePukuManager(const char* name);

	virtual ~TTabePukuManager() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
};

DECLARE_NERVE(TNerveTabePukuAttack, TLiveActor)
DECLARE_NERVE(TNerveTabePukuBite, TLiveActor)
DECLARE_NERVE(TNerveTabePukuDive, TLiveActor)
DECLARE_NERVE(TNerveTabePukuDrag, TLiveActor)
DECLARE_NERVE(TNerveTabePukuFound, TLiveActor)
DECLARE_NERVE(TNerveTabePukuGraphWander, TLiveActor)
DECLARE_NERVE(TNerveTabePukuRecoverGraph, TLiveActor)

#endif
