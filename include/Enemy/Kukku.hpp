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

class TKukkuManager : public TSmallEnemyManager {
public:
	TKukkuManager(const char* name = "クックマネージャー");
	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
	void initJParticle();

	// Unused
	void initCoins();
};

class TKukku : public TSmallEnemy {
public:
	TKukku(const char* name = "クック");
	virtual void bind();
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void reset();
	virtual void control();
	virtual void init(TLiveManager*);
	virtual const char** getBasNameTable() const;

	void setAfterDeadEffect();
	void setDeadAnm();
	void dropCoins();
	void updateRotation();
	void calcRootMatrix();
	JGeometry::TVec3<f32> calcMomentum(f32 deltaTime);
	void behaveToWater(THitActor* sender);

	// Unused
	TKukkuBall* getUnusedBall();
	f32 getWaterPowerY() const;
	f32 getWaterDamageRate() const;
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

	// fabricated
	s32 getUnk1A4() const { return unk1A4; }
	s32 getShootTimer() const { return mShootTimer; }

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
	/* 0x1A0 */ TMapObjBase* mOneUp;
	/* 0x1A4 */ u32 unk1A4;
	/* 0x1A8 */ u32 mJointIndex;
	/* 0x1AC */ s32 mShootTimer;
	/* 0x1B0 */ s32 mDroppedCoins;
};

class TKukkuBall : public THitActor {
public:
	TKukkuBall(MActor* actor);
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

	// fabricated
	bool checkDeadFilter(u32 flag) const { return mDeadFilter & flag; }
	void offDeadFilter(u32 flag) { mDeadFilter &= flag; }
	void onDeadFilter(u32 flag) { mDeadFilter |= flag; }

public:
	/* 0x68 */ MActor* mActor;
	/* 0x6C */ s32 mDeadFilter;
	/* 0x70 */ JGeometry::TVec3<f32> mVelocity;
	/* 0x7C */ u32 mHitpoints;
};

DECLARE_NERVE(TNerveKukkuRecoverGraph, TLiveActor);
DECLARE_NERVE(TNerveKukkuPostFall, TLiveActor);
DECLARE_NERVE(TNerveKukkuFall, TLiveActor);
DECLARE_NERVE(TNerveKukkuGraphWander, TLiveActor);
DECLARE_NERVE(TNerveKukkuHit, TLiveActor);

#endif
