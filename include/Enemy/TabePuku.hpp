#ifndef ENEMY_TABE_PUKU_HPP
#define ENEMY_TABE_PUKU_HPP

#include <Enemy/SmallEnemy.hpp>
#include <JSystem/JGeometry.hpp>
#include <Strategic/HitActor.hpp>

class TBGCheckData;
class TTabePuku;

// The cheep cheeps ("pukupuku") that patrol a rail graph underwater, bite
// Mario and drag him away. Member names come from the .prm strings.
class TTabePukuParams : public TSmallEnemyParams {
public:
	TTabePukuParams(const char* path);

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

class TTabePukuManager : public TSmallEnemyManager {
public:
	TTabePukuManager(const char* name = "プクプク(レール巡回)マネージャー");
	virtual ~TTabePukuManager() { }

	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
};

// The fish's own collision, which moves with it through the water and
// collides with the map on its behalf.
class TTPHitActor : public THitActor {
public:
	TTPHitActor(TTabePuku& owner);
	virtual ~TTPHitActor() { }

	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void init();

	void checkHitActors();
	void bind();
	void updateTerrainCollsion();
	void updateObjCollision();

public:
	/* 0x68 */ TTabePuku* mOwner;
	/* 0x6C */ JGeometry::TVec3<f32> mPositionDelta;
	/* 0x78 */ f32 mHeight;
	/* 0x7C */ f32 mRadius;
	/* 0x80 */ f32 mGroundHeight;
	/* 0x84 */ const TBGCheckData* mGroundPlane;
	/* 0x88 */ bool mAirborne;
	/* 0x89 */ bool mTouchedWall;
};

class TTabePuku : public TSmallEnemy {
public:
	TTabePuku(const char* name = "プクプク(レール巡回)");
	virtual ~TTabePuku() { }

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
	void swimTo(const JGeometry::TVec3<f32>& dir);
	bool doDrag();
	void prepareDrag();
	bool doDive();
	void prepareDive();
	void doBite();
	void swimToCurPathNode(const JGeometry::TVec3<f32>& offset);
	bool isBiting() const;
	bool isAttacking() const;
	bool isGraphWander() const;
	bool isTouchedPlane() const;
	bool isMissMario() const;
	void emitEffects();
	void updateSound();
	void initParams();
	void initCollision();

	// fabricated
	const TTabePukuParams* getParam() const
	{
		return (const TTabePukuParams*)getSaveParam();
	}

public:
	/* 0x194 */ TTPHitActor* mHitActor;
	/* 0x198 */ JGeometry::TQuat4<f32> mQuat;
	/* 0x1A8 */ TPosition3f mTakingMtx;
	/* 0x1D8 */ int mMouthJoint;
	/* 0x1DC */ bool mTouchedWall;
	/* 0x1E0 */ f32 mDiveStartY;
	/* 0x1E4 */ JGeometry::TVec3<f32> mDragDir;
};

DECLARE_NERVE(TNerveTabePukuGraphWander, TLiveActor);
DECLARE_NERVE(TNerveTabePukuFound, TLiveActor);
DECLARE_NERVE(TNerveTabePukuRecoverGraph, TLiveActor);
DECLARE_NERVE(TNerveTabePukuAttack, TLiveActor);
DECLARE_NERVE(TNerveTabePukuBite, TLiveActor);
DECLARE_NERVE(TNerveTabePukuDive, TLiveActor);
DECLARE_NERVE(TNerveTabePukuDrag, TLiveActor);

#endif
