#ifndef ENEMY_AMINOKO_HPP
#define ENEMY_AMINOKO_HPP

#include <Enemy/WalkerEnemy.hpp>
#include <Strategic/Nerve.hpp>
#include <dolphin/mtx.h>

class TAmiNoko;
class TBGCheckData;

// Names and defaults are the ones PARAM_INIT stringified into .rodata.
class TAmiNokoSaveLoadParams : public TWalkerEnemyParams {
public:
	TAmiNokoSaveLoadParams(const char* prm);

	f32 getSLElecRange() const { return mSLElecRange.get(); }
	f32 getSLMtxRotSpeed() const { return mSLMtxRotSpeed.get(); }

	/* 0x32C */ TParamRT<f32> mSLElecRange;
	/* 0x340 */ TParamRT<f32> mSLMtxRotSpeed;
};

// The hit box that rides in front of the amiNoko along its facing direction
// and forwards Mario's touches to the owner.
class TAmiHit : public THitActor {
public:
	TAmiHit(TAmiNoko* owner, const char* name);

	virtual ~TAmiHit() { }
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);

	/* 0x68 */ TAmiNoko* mOwner;
};

class TAmiNoko : public TWalkerEnemy {
public:
	// Which surface the amiNoko is currently crawling on. Set by
	// calcDirection() from whichever of the three collision queries won.
	enum {
		AMINOKO_SURFACE_GROUND = 0,
		AMINOKO_SURFACE_ROOF   = 1,
		AMINOKO_SURFACE_WALL   = 2,
	};

	TAmiNoko(const char* name);

	virtual ~TAmiNoko() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void bind();
	virtual f32 getGravityY() const;
	virtual const char** getBasNameTable() const;
	virtual void reset();
	virtual void behaveToWater(THitActor*);
	virtual void setWalkAnm();
	virtual void attackToMario();
	virtual void setMActorAndKeeper();
	virtual bool isHitValid(u32);
	virtual bool isCollidMove(THitActor*) { return false; }

	void calcDirection();
	void emitEffects();
	void creepToCurPathNode(f32 max_speed);
	bool isDeadByWall();

	// fabricated
	TAmiNokoSaveLoadParams* getSaveParams() const
	{
		return (TAmiNokoSaveLoadParams*)getSaveParam();
	}

	/* 0x194 */ const TBGCheckData* mFencePlane;
	/* 0x198 */ int mFenceKind;
	/* 0x19C */ JGeometry::TVec3<f32> mUp;
	/* 0x1A8 */ JGeometry::TVec3<f32> mFront;
	/* 0x1B4 */ JGeometry::TVec3<f32> mPrevUp;
	/* 0x1C0 */ JGeometry::TVec3<f32> mPrevFront;
	/* 0x1CC */ Mtx mRootMtx;
	/* 0x1FC */ JGeometry::TVec3<f32> mSparkPos;
	/* 0x208 */ TAmiNokoSaveLoadParams* mSaveParams;
	// Picks between the two halves of the .bck table: every animation exists
	// twice (aminoko_run1/run2, aminoko_turn1/turn2) three indices apart, and
	// init() switches to the second set on map 8.
	/* 0x20C */ u8 mUseAnmSet1;
	/* 0x210 */ TAmiHit* mAmiHit;
};

class TAmiNokoManager : public TSmallEnemyManager {
public:
	TAmiNokoManager(const char* name);

	virtual ~TAmiNokoManager() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();
};

class TLiveActor;

DECLARE_NERVE(TNerveAmiNokoAttack, TLiveActor)
DECLARE_NERVE(TNerveAmiNokoDie, TLiveActor)
DECLARE_NERVE(TNerveAmiNokoFreeze, TLiveActor)
DECLARE_NERVE(TNerveAmiNokoTurn, TLiveActor)
DECLARE_NERVE(TNerveAmiNokoWalkOnFence, TLiveActor)

#endif
