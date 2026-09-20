#ifndef ENEMY_PAKKUN_HPP
#define ENEMY_PAKKUN_HPP

#include <Enemy/EnemyAttachment.hpp>
#include <Enemy/SmallEnemy.hpp>

class TWaterEmitInfo;

DECLARE_NERVE(TNerveStayPakkunAppear, TLiveActor);
DECLARE_NERVE(TNervePakkunStay, TLiveActor);
DECLARE_NERVE(TNerveStayPakkunHide, TLiveActor);
DECLARE_NERVE(TNervePakkunFreeze, TLiveActor);
DECLARE_NERVE(TNervePakkunShoot, TLiveActor);
DECLARE_NERVE(TNervePakkunHide, TLiveActor);
DECLARE_NERVE(TNervePakkunAppear, TLiveActor);
DECLARE_NERVE(TNervePakkunGenerate, TLiveActor);

// TODO: Need to validate that these are actually real
enum TPakkunSeedState {
	PAKKUN_SEED_STATE_HIDE   = 0,
	PAKKUN_SEED_STATE_APPEAR = 1,
	PAKKUN_SEED_STATE_SET    = 2,
	PAKKUN_SEED_STATE_SHOOT  = 3,
	PAKKUN_SEED_STATE_CIRCLE = 4,
};

// An enum wrapper for the bas animations
enum TPakkunAnimation {
	PAKKUN_ANM_CRUSH_TO_HIDE = 0,
	PAKKUN_ANM_DAMAGE        = 1,
	PAKKUN_ANM_DOWN          = 2,
	PAKKUN_ANM_HIDE          = 3,
	PAKKUN_ANM_UNK4          = 4,
	PAKKUN_ANM_UNK5          = 5,
	PAKKUN_ANM_UNK6          = 6,
	PAKKUN_ANM_SET           = 7,
	PAKKUN_ANM_SHOOT         = 8,
	PAKKUN_ANM_WAIT          = 9,
};

class TPakkunSaveLoadParams : public TSmallEnemyParams {
public:
	// UNUSED
	TPakkunSaveLoadParams(const char* path);

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

class TPakkunSeed : public TEnemyAttachment {
public:
	// UNUSED
	TPakkunSeed(const char* name = "パックン種");

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
	// UNUSED
	void seedSet();

public:
	/* 0x16C */ class TPakkun* unk16C;
	/* 0x170 */ f32 unk170;
};

class TPakkun : public TSmallEnemy {
public:
	TPakkun(const char* name = "パックン");

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

	void onShootLiner(JGeometry::TVec3<f32>&);
	// UNUSED
	void onShootCurve(JGeometry::TVec3<f32>&);
	// UNUSED
	void seedPollute(JGeometry::TVec3<f32>&);
	// UNUSED
	void createPakkunSmoke(JGeometry::TVec3<f32>&);
	// UNUSED
	bool isHideEnd() const;

	static u8 mHeadJntIndex;

public:
	/* 0x194 */ TPakkunSeed* unk194;
	/* 0x198 */ u8 unk198;
	/* 0x199 */ u8 unk199;
	/* 0x19A */ u8 unk19A[2];
	/* 0x19C */ TPakkunSeed** unk19C;
	/* 0x1A0 */ TPakkunSaveLoadParams* unk1A0;
	/* 0x1A4 */ JGeometry::TVec3<f32> unk1A4;
	/* 0x1B0 */ bool unk1B0;
	/* 0x1B1 */ bool unk1B1;
	/* 0x1B2 */ GXColor unk1B2;
	/* 0x1B6 */ u8 unk1B6[2];
	/* 0x1B8 */ f32 unk1B8;
	/* 0x1BC */ u8 unk1BC;
	/* 0x1BD */ u8 unk1BD[3];
};

class TStayPakkun : public TPakkun {
public:
	virtual void load(JSUMemoryInputStream&);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void kill();
	virtual TPakkunSaveLoadParams* getSaveParam() const;
	virtual void genRandomItem();
	virtual void reset();
	virtual void setBehavior();
	virtual void setDeadAnm();
	virtual bool isHitValid(u32);
	virtual void setAfterDeadEffect() { }
	virtual void shootIn();
	virtual void shoot();
};

class TPakkunManager : public TSmallEnemyManager {
public:
	TPakkunManager(const char* name = "パックンマネージャー");

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void createModelData();
	virtual TSmallEnemy* createEnemyInstance();
	virtual void clipEnemies(JDrama::TGraphics*);

	static f32 mRootExplosionScaleRate;
	static f32 mTestFlyAngX;
	static f32 mIgnoreHitWaterY;

public:
	/* 0x60 */ TPakkunSaveLoadParams* unk60;
	/* 0x64 */ TWaterEmitInfo* unk64;
	/* 0x68 */ TWaterEmitInfo* unk68;
};

#endif
