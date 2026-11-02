#ifndef ENEMY_KUKKU_HPP
#define ENEMY_KUKKU_HPP

#include <Enemy/SmallEnemy.hpp>
#include <Strategic/Spine.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <M3DUtil/MActor.hpp>
#include <MoveBG/MapObjManager.hpp>

class TKukkuBall;

// Unused 0x3B0
class TKukkuParams : public TSmallEnemyParams {
public:
	TKukkuParams(const char*);

	/* 0x2D4 */ TParamRT<f32> mMarchSpeed;
	/* 0x2E8 */ TParamRT<f32> mTurnSpeed;
	/* 0x2FC */ TParamRT<f32> mWaterPowerY;
	/* 0x310 */ TParamRT<f32> mShootSpeed;
	/* 0x324 */ TParamRT<s32> mShootInterval;
	/* 0x338 */ TParamRT<f32> mSearchRange;
	/* 0x34C */ TParamRT<s32> mHabatakiTimer;
	/* 0x360 */ TParamRT<f32> mAirFric;
	/* 0x374 */ TParamRT<f32> mUpperVelocityY;
	/* 0x388 */ TParamRT<f32> mDropSpeed;
	/* 0x39C */ TParamRT<f32> mDropAngleX;
};

class TEnemyCoinUnit {
	TEnemyCoinUnit(int);
	void init();
	TMapObjBase* getUnusedItem();
};

class TKukku : public TSmallEnemy {
public:
	TKukku(const char* name = "");
	virtual ~TKukku() { };

	void setAfterDeadEffect();
	void setDeadAnm();
	void dropCoins();
	void updateRotation();
	void calcRootMatrix();
	JGeometry::TVec3<f32> calcMomentum(f32 deltaTime);
	void behaveToWater(THitActor* sender);
	void bind();
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	void reset();
	void control();
	virtual void init(TLiveManager*);
	virtual const char** getBasNameTable() const;

	// Unused
	TKukkuBall* getUnusedBall();
	f32 getWaterPowerY() const;
	void getWaterDamageRate() const;
	void changeBck(const char*);
	BOOL isFindOutMario() const;
	BOOL isDying() const;
	BOOL isRecoveringGraph() const;
	BOOL isFalling() const;
	void shotBall();
	void decideFlyingAnm();
	BOOL doHabataki();
	void doRecoverToCurPathNode();
	void doFlyToCurPathNode();
	void behaveHitTrample();
	void updateEffect();
	void initParticle();
	void initCollision();
	void initBalls();
	void getUnusedItem(TEnemyCoinUnit*);

	// fabricated
	s32 getUnk1A4() const { return unk1A4; }
	s32 getUnk1AC() const { return unk1AC; }

	// fabricated
	const TKukkuParams* getKukkuParams() const
	{
		return (const TKukkuParams*)getSaveParam();
	}

	// fabricated
	JGeometry::TVec3<f32> polarXZ(f32 theta, f32 radius)
	{
		f32 c = radius * MsCos(theta);
		f32 s = radius * MsSin(theta);
		return JGeometry::TVec3<f32>(s, 0.0f, c);
	}

public:
	/* 0x194 */ TKukkuBall* mBalls[3];
	/* 0x1A0 */ TMapObjBase* mMapObj;
	/* 0x1A4 */ u32 unk1A4;
	/* 0x1A8 */ u32 unk1A8;
	/* 0x1AC */ s32 unk1AC;
	/* 0x1B0 */ s32 unk1B0;
};

class TKukkuManager : public TSmallEnemyManager {
public:
	TKukkuManager(const char* name = "");
	virtual ~TKukkuManager() { };
	virtual void load(JSUMemoryInputStream&);
	virtual TSpineEnemy* createEnemyInstance();
	virtual void createModelData();
	void initJParticle();

	// Unused
	void initCoins();
};

// class Thitactor adds all the bloat libraries horrible
class TKukkuBall : public THitActor {
public:
	TKukkuBall(MActor* actor);
	virtual ~TKukkuBall() { };
	virtual void init();
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	// UNUSED
	BOOL isDead() const;
	void kill();
	void startToMove(const JGeometry::TVec3<f32>& start,
	                 const JGeometry::TVec3<f32>& end);
	void checkHitActors();
	void bind();
	void moveObject();

public:
	/* 0x68 */ MActor* actor;
	/* 0x6C */ s32 unk6C;
	/* 0x70 */ JGeometry::TVec3<f32> mVelocity;
	/* 0x7C */ u32 unk7C;
};

DECLARE_NERVE(TNerveKukkuRecoverGraph, TLiveActor);
DECLARE_NERVE(TNerveKukkuPostFall, TLiveActor);
DECLARE_NERVE(TNerveKukkuFall, TLiveActor);
DECLARE_NERVE(TNerveKukkuGraphWander, TLiveActor);
DECLARE_NERVE(TNerveKikkuHit, TLiveActor);

#endif
