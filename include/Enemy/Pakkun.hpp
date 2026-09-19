#ifndef ENEMY_PAKKUN_HPP
#define ENEMY_PAKKUN_HPP

#include <Enemy/EnemyAttachment.hpp>
#include <Enemy/SmallEnemy.hpp>
#include <dolphin/gx.h>

class TPakkun;
class TPakkunSeed;
class TWaterEmitInfo;

// The piranha plant that hides in the ground, pops up and spits a seed at
// Mario. Two flavours share the code: the plain pakkun, which is spawned by
// TPakkunManager and only ever holds one seed, and the "stay" pakkun, which
// is placed by hand in the stage, holds three seeds at once and has to be
// sprayed to death.
//
// Names and defaults are the ones PARAM_INIT stringified into .rodata and the
// constants TPakkunManager::load stores into each TParamRT.
// TEnemyAttachment::unk150 values the seed uses. 3 and 4 pick which gravity
// TPakkunSeed::getNowGravity hands back, so they are the two shot arcs.
enum {
	SEED_STATE_DEAD  = 0,
	SEED_STATE_HELD  = 1,
	SEED_STATE_DROP  = 2,
	SEED_STATE_LINER = 3,
	SEED_STATE_CURVE = 4,
};

class TPakkunSaveLoadParams : public TSmallEnemyParams {
public:
	TPakkunSaveLoadParams(const char* prm);

	s32 getSLReadyTime() const { return mSLReadyTime.get(); }
	f32 getSLShootRange() const { return mSLShootRange.get(); }
	f32 getSLSeedGravityS() const { return mSLSeedGravityS.get(); }
	f32 getSLSeedSpeedS() const { return mSLSeedSpeedS.get(); }
	f32 getSLSeedGravityC() const { return mSLSeedGravityC.get(); }
	f32 getSLSeedSpeedC() const { return mSLSeedSpeedC.get(); }
	f32 getSLMarioCircle() const { return mSLMarioCircle.get(); }
	f32 getSLLimitMove() const { return mSLLimitMove.get(); }
	f32 getSLMoveDist() const { return mSLMoveDist.get(); }
	s32 getSLGenerateSeedTime() const { return mSLGenerateSeedTime.get(); }
	f32 getSLGenerateSeedDist() const { return mSLGenerateSeedDist.get(); }
	s32 getSLDamageHideTime() const { return mSLDamageHideTime.get(); }

	/* 0x2D4 */ TParamRT<s32> mSLReadyTime;
	/* 0x2E8 */ TParamRT<f32> mSLShootRange;
	/* 0x2FC */ TParamRT<f32> mSLSeedGravityS;
	/* 0x310 */ TParamRT<f32> mSLSeedSpeedS;
	/* 0x324 */ TParamRT<f32> mSLSeedGravityC;
	/* 0x338 */ TParamRT<f32> mSLSeedSpeedC;
	/* 0x34C */ TParamRT<f32> mSLMarioCircle;
	/* 0x360 */ TParamRT<f32> mSLLimitMove;
	/* 0x374 */ TParamRT<f32> mSLMoveDist;
	/* 0x388 */ TParamRT<s32> mSLGenerateSeedTime;
	/* 0x39C */ TParamRT<f32> mSLGenerateSeedDist;
	/* 0x3B0 */ TParamRT<s32> mSLDamageHideTime;
};

class TPakkun : public TSmallEnemy {
public:
	TPakkun(const char* name = "パックン");

	// Declared in vtable order. TPakkun adds shootIn/shoot to TSmallEnemy.
	virtual ~TPakkun() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual void init(TLiveManager*);
	virtual void kill();
	virtual const char** getBasNameTable() const;
	virtual void reset();
	virtual void behaveToWater(THitActor*);
	virtual void setDeadAnm();
	virtual void setFreezeAnm();
	virtual void setWaitAnm();
	virtual void setMActorAndKeeper();
	virtual bool isCollidMove(THitActor*) { return false; }
	virtual BOOL isInhibitedForceMove();
	virtual void decHpByWater(THitActor*) { }
	virtual void shootIn();
	virtual void shoot();

	// fabricated
	TPakkunSaveLoadParams* getSaveLoadParam() const { return mSaveParams; }

	void onShootLiner(JGeometry::TVec3<f32>& dir);
	void onShootCurve(JGeometry::TVec3<f32>& dir);
	void seedPollute(JGeometry::TVec3<f32>& pos);
	void createPakkunSmoke(JGeometry::TVec3<f32>& pos);
	bool isHideEnd() const;

	// Index of the "null_seed" joint, the point the seed sits on while it is
	// still in the mouth. The model is shared, so one lookup serves them all.
	static u8 mHeadJntIndex;

public:
	/* 0x194 */ TPakkunSeed* mSeed;
	// Picks which arc TPakkunSeed::shoot gives the seed: 0 spits it straight
	// at Mario, 1 lobs it over whatever the last seed bounced off.
	/* 0x198 */ u8 mIsCurveShot;
	// Set by TStayPakkun::load. Everything the hand-placed variant does
	// differently keys off this rather than off a virtual.
	/* 0x199 */ u8 mIsStay;
	// The two extra seeds a stay pakkun fires either side of mSeed.
	/* 0x19C */ TPakkunSeed** mSubSeeds;
	/* 0x1A0 */ TPakkunSaveLoadParams* mSaveParams;
	/* 0x1A4 */ JGeometry::TVec3<f32> mEffectPos;
	// Set when a seed hits a wall, which makes the next shot a lob.
	/* 0x1B0 */ u8 mSeedHitWall;
	// While set, the seed is still in the mouth: it does not spin and it is
	// taken back instead of sprouting.
	/* 0x1B1 */ u8 mIsHoldingSeed;
	// Fed to the model's TEV konst colour; the alpha carries the damage fade.
	/* 0x1B2 */ GXColor mBodyColor;
	/* 0x1B8 */ f32 mExplosionScale;
	// Set while the stay pakkun is hiding because it was sprayed, so that it
	// waits out mSLDamageHideTime before popping back up.
	/* 0x1BC */ u8 mDamageHiding;
};

class TStayPakkun : public TPakkun {
public:
	TStayPakkun(const char* name)
	    : TPakkun(name)
	{
	}

	// Declared in vtable order; TStayPakkun adds no new slots.
	virtual ~TStayPakkun() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void kill();
	virtual void reset();
	virtual TSpineEnemyParams* getSaveParam() const;
	virtual void genRandomItem();
	virtual void setBehavior();
	virtual void setDeadAnm();
	virtual bool isHitValid(u32);
	virtual void setAfterDeadEffect() { }
	virtual void shootIn();
	virtual void shoot();
};

class TPakkunSeed : public TEnemyAttachment {
public:
	TPakkunSeed(const char* name);

	// Declared in vtable order. TPakkunSeed adds shoot to TEnemyAttachment.
	virtual ~TPakkunSeed() { }
	virtual void calcRootMatrix();
	virtual void moveObject();
	virtual void loadInit(TSpineEnemy*, const char*);
	virtual void appear();
	virtual void rebirth();
	virtual void set();
	virtual void behaveToHost();
	virtual void behaveToHitGround();
	virtual void behaveToHitWall(const TBGCheckData*);
	virtual void forceKill();
	virtual f32 getNowGravity();
	virtual void shoot();

	void seedSet();

	// fabricated. The generate nerve materialises a bool at this test, which
	// is what an inlined predicate written with an explicit return looks
	// like; comparing unk150 inline folds to a bare branch.
	bool isHeld() const
	{
		if (unk150 == SEED_STATE_HELD)
			return true;
		return false;
	}

public:
	/* 0x16C */ TPakkun* mPakkun;
	/* 0x170 */ f32 mSpinAngle;
};

class TPakkunManager : public TSmallEnemyManager {
public:
	TPakkunManager(const char* name);

	// Declared in vtable order.
	virtual ~TPakkunManager() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();
	virtual void clipEnemies(JDrama::TGraphics*);

	// How much bigger the head gets by the time a stay pakkun is about to
	// burst, and the pitch and water height the test code was tuned with.
	static f32 mRootExplosionScaleRate;
	static f32 mTestFlyAngX;
	static f32 mIgnoreHitWaterY;

public:
	// The stay pakkuns read their parameters out of a second .prm file.
	/* 0x60 */ TPakkunSaveLoadParams* mStayParams;
	/* 0x64 */ TWaterEmitInfo* mRipWater;
	/* 0x68 */ TWaterEmitInfo* mHideWater;
};

#endif
