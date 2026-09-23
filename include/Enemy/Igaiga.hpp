#ifndef ENEMY_IGAIGA_HPP
#define ENEMY_IGAIGA_HPP

#include <Enemy/EnemyAttachment.hpp>
#include <Enemy/WalkerEnemy.hpp>
#include <JSystem/JGeometry.hpp>

class TWaterEmitInfo;
class TGraphWeb;
class TMapEventSink;
class TAreaCylinderManager;

// Shared by the igaiga and the gorogoro; each manager loads it from its own
// .prm. Names are the PARAM_INIT strings in .rodata, defaults are what the
// inlined constructor stores. 0x390 bytes (the managers' operator new).
class TRollEnemySaveLoadParams : public TWalkerEnemyParams {
public:
	TRollEnemySaveLoadParams(const char* prm);

	/* 0x32C */ TParamRT<s32> mSLGenerateInterval;
	/* 0x340 */ TParamRT<f32> mSLExpandRate;
	/* 0x354 */ TParamRT<f32> mSLExpandMax;
	/* 0x368 */ TParamRT<f32> mSLBoundVYMax;
	/* 0x37C */ TParamRT<f32> mSLGroundOffsetY;
};

class TRollEnemy : public TWalkerEnemy {
public:
	TRollEnemy(const char* name);

	virtual void reset();
	virtual void setBehavior();
	virtual void behaveToWater(THitActor*);
	virtual void attackToMario();
	virtual bool isCollidMove(THitActor*);
	virtual void setAfterDeadEffect() { }
	virtual void walkBehavior(int, f32);
	virtual void flagJump();
	virtual bool isReachedToGoalXZ();
	virtual void bound() { }
	virtual bool isRolling() { return false; }
	virtual void rollSE() { }
	virtual void boundSE() { }

	// Rescales the hit and damage cylinders from the swollen body scale.
	// Retail reaches this from behaveToWater, which expands whole into
	// TGorogoro::behaveToWater; the split is what keeps that expansion inside
	// MWCC's depth-1 statement budget while this stays inside the depth-2 one.
	// TODO: in TGorogoro::behaveToWater retail keeps attackRadius /
	// attackHeight / damageRadius in f29 / f30 / f31 (ours f31 / f30 / f29).
	// Inert: C-style top declarations in reverse, ratio declared first;
	// setHitParams(...) with calcEntryRadius dropped from the caller grows
	// the frame by 0x10-0x18.
	void calcHitScale()
	{
		f32 attackRadius = getSaveParams()->getSLAttackRadius();
		f32 attackHeight = getSaveParams()->getSLAttackHeight();
		f32 damageRadius = getSaveParams()->getSLDamageRadius();
		f32 damageHeight = getSaveParams()->getSLDamageHeight();
		f32 ratio        = mBodyScale / unk154;
		mAttackRadius    = attackRadius * ratio;
		mAttackHeight    = attackHeight * ratio;
		mDamageRadius    = damageRadius * ratio;
		mDamageHeight    = damageHeight * ratio;
	}

	static f32 mBoundVal;
	static f32 mTransYOffset;

	/* 0x194 */ f32 mRollAngle;
	/* 0x198 */ f32 unk198;
	/* 0x19C */ f32 unk19C;
	/* 0x1A0 */ f32 unk1A0;
	/* 0x1A4 */ TRollEnemySaveLoadParams* unk1A4;
	/* 0x1A8 */ u8 unk1A8;
	/* 0x1AC */ f32 unk1AC;
	/* 0x1B0 */ f32 unk1B0;
};

class TIgaigaPolluteModelManager : public TEnemyPolluteModelManager {
public:
	TIgaigaPolluteModelManager(const char* name = "イガイガモデル汚染")
	    : TEnemyPolluteModelManager(name)
	{
	}

	virtual void init(TLiveActor*);
};

class TIgaigaPolluteModel : public TEnemyPolluteModel {
public:
	TIgaigaPolluteModel(TLiveActor* actor, SDLModelData* data,
	                    const char* name = "イガイガ汚染モデル")
	    : TEnemyPolluteModel(actor, 0, data, name)
	{
	}

	virtual void setAnm();
};

class TIgaigaManager : public TSmallEnemyManager {
public:
	TIgaigaManager(const char* name);

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();
	virtual void initSetEnemies();

	void requestPolluteModel(JGeometry::TVec3<f32>&, JGeometry::TVec3<f32>&);

	/* 0x60 */ TIgaigaPolluteModelManager* unk60;
	/* 0x64 */ u32 unk64;
	/* 0x68 */ TWaterEmitInfo* unk68;
};

class TIgaiga : public TRollEnemy {
public:
	TIgaiga(const char* name = "イガイガ");

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void moveObject();
	virtual void kill();
	virtual const char** getBasNameTable() const;
	virtual void reset();
	virtual void behaveToWater(THitActor*);
	virtual void setWalkAnm();
	virtual void setDeadAnm();
	virtual void setMeltAnm();
	virtual void setMActorAndKeeper();
	virtual bool isHitValid(u32);
	virtual void walkBehavior(int, f32);
	virtual bool isReachedToGoalXZ();
	virtual void bound();
	virtual bool isRolling();
	virtual void rollSE();
	virtual void boundSE();

	void shoot(JGeometry::TVec3<f32>&);

	// UNUSED in the map.
	void rollMove();
	void waterExplosion();

	static f32 mReachNodeDist;
	static f32 mTremblePow;
	static f32 mTrembleAcc;
	static f32 mTrembleBrk;

	/* 0x1B4 */ s32 unk1B4;
	/* 0x1B8 */ s32 unk1B8;
	/* 0x1BC */ u8 unk1BC;
	/* 0x1C0 */ JGeometry::TVec3<f32> unk1C0;
	/* 0x1CC */ f32 unk1CC;
	/* 0x1D0 */ u32 unk1D0;
	/* 0x1D4 */ u8 unk1D4[4];
	/* 0x1D8 */ JGeometry::TVec3<f32> mShootVelocity;
	/* 0x1E4 */ f32 unk1E4;
	/* 0x1E8 */ s32 unk1E8;
};

DECLARE_NERVE(TNerveIgaigaRollOnGraph, TLiveActor);
DECLARE_NERVE(TNerveIgaigaWaterHit, TLiveActor);
DECLARE_NERVE(TNerveIgaigaShootFromCannon, TLiveActor);

class TGorogoroPolluteModelManager : public TEnemyPolluteModelManager {
public:
	TGorogoroPolluteModelManager(const char* name = "ゴロゴロモデル汚染")
	    : TEnemyPolluteModelManager(name)
	{
	}

	virtual void init(TLiveActor*);
};

class TGorogoroPolluteModel : public TEnemyPolluteModel {
public:
	TGorogoroPolluteModel(TLiveActor* actor, SDLModelData* data,
	                      const char* name = "汚染モデル")
	    : TEnemyPolluteModel(actor, 0, data, name)
	{
	}

	virtual void setAnm();
};

class TGorogoro;

class TGorogoroManager : public TSmallEnemyManager {
public:
	TGorogoroManager(const char* name);

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();
	virtual void initSetEnemies();

	// UNUSED in the map.
	BOOL inArea(const JGeometry::TVec3<f32>&);
	void requestPolluteModel(JGeometry::TVec3<f32>&, JGeometry::TVec3<f32>&);

	// Frames since the last spawn, the sinking event it waits on, whether the
	// next spawn is the first, the stamp manager, and the cylinder Mario has
	// to be inside for spawning to run.
	/* 0x60 */ s32 unk60;
	/* 0x64 */ TMapEventSink* unk64;
	/* 0x68 */ u8 unk68;
	/* 0x6C */ TGorogoroPolluteModelManager* unk6C;
	/* 0x70 */ TAreaCylinderManager* unk70;
};

class TGorogoro : public TRollEnemy {
public:
	TGorogoro(const char* name = "ゴロゴロ");

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void kill();
	virtual const char** getBasNameTable() const;
	virtual void reset();
	virtual void behaveToWater(THitActor*);
	virtual void setDeadAnm();
	virtual void setMeltAnm();
	virtual void forceKill();
	virtual void setMActorAndKeeper();
	virtual void walkBehavior(int, f32);
	virtual void flagJump();
	virtual void bound();
	virtual bool isRolling();
	virtual void rollSE();
	virtual void boundSE();

	void generateByGateKeeper(const JGeometry::TVec3<f32>&,
	                          const JGeometry::TVec3<f32>&);

	// UNUSED in the map.
	void setGenerateGraphIdx(int);

	// unk1B4 is the matrix it dies in; setMeltAnm copies the base matrix
	// there and pins its translation row to the ground.
	/* 0x1B4 */ TPosition3f unk1B4;
	/* 0x1E4 */ u8 unk1E4;
	/* 0x1E8 */ s32 mGenerateGraphIdx;
	/* 0x1EC */ u8 unk1EC;
	/* 0x1ED */ GXColor mTevKColor;
};

DECLARE_NERVE(TNerveGorogoroRollOnGraph, TLiveActor);
DECLARE_NERVE(TNerveGorogoroDie, TLiveActor);

#endif
