#ifndef ENEMY_KUKKU_HPP
#define ENEMY_KUKKU_HPP

#include <Enemy/SmallEnemy.hpp>
#include <Strategic/HitActor.hpp>

class MActor;
class TMapObjBase;

// The birds ("tori") that fly around a graph, drop polluting goop on Mario
// and fall out of the sky when sprayed. Member names come from the .prm
// strings.
class TKukkuParams : public TSmallEnemyParams {
public:
	TKukkuParams(const char* path);

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

class TKukkuManager : public TSmallEnemyManager {
public:
	TKukkuManager(const char* name = "クックマネージャー");
	virtual ~TKukkuManager() { }

	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();

	void initCoins();

public:
	/* 0x60 */ u32 unk60;
};

// TODO: nothing linked uses this class, so only its signatures are known.
class TEnemyCoinUnit {
public:
	TEnemyCoinUnit(int);
	void init();
	TMapObjBase* getUnusedItem();
};

// The goop the bird drops. It pollutes the ground where it lands.
class TKukkuBall : public THitActor {
public:
	TKukkuBall(MActor* actor);
	virtual ~TKukkuBall() { }

	virtual void perform(u32, JDrama::TGraphics*);
	virtual void init();

	bool isDead() const;
	void kill();
	void startToMove(const JGeometry::TVec3<f32>& pos,
	                 const JGeometry::TVec3<f32>& velocity);
	void checkHitActors();
	void bind();
	void moveObject();

public:
	/* 0x68 */ MActor* mMActor;
	/* 0x6C */ u32 mFlags;
	/* 0x70 */ JGeometry::TVec3<f32> mVelocity;
	/* 0x7C */ u32 unk7C;
};

class TKukku : public TSmallEnemy {
public:
	TKukku(const char* name = "クック");
	virtual ~TKukku() { }

	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void control();
	virtual void bind();
	virtual const char** getBasNameTable() const;
	virtual void reset();
	virtual void behaveToWater(THitActor*);
	virtual void setDeadAnm();
	virtual void setAfterDeadEffect();

	TKukkuBall* getUnusedBall();
	f32 getWaterPowerY() const;
	f32 getWaterDamageRate() const;
	void changeBck(const char* name);
	bool isFindOutMario() const;
	bool isDying() const;
	bool isRecoveringGraph() const;
	bool isFalling() const;
	void dropCoins();
	void shotBall();
	void decideFlyingAnm();
	bool doHabataki();
	void doRecoverToCurPathNode();
	JGeometry::TVec3<f32> calcMomentum(f32 speed);
	void updateRotation();
	void doFlyToCurPathNode();
	void behaveHitTrample();
	void updateEffect();
	void initParticle();
	void initCollision();
	void initBalls();

	// fabricated: both timers are loaded again right after being checked,
	// which is what an inline accessor in the check does (as in wireTrap).
	s32 getUnk1A4() const { return unk1A4; }
	s32 getShootTimer() const { return mShootTimer; }

	// fabricated
	const TKukkuParams* getParam() const
	{
		return (const TKukkuParams*)getSaveParam();
	}

public:
	/* 0x194 */ TKukkuBall* mBalls[3];
	/* 0x1A0 */ TMapObjBase* mOneUp;
	/* 0x1A4 */ s32 unk1A4;
	/* 0x1A8 */ s32 mCenterJoint;
	/* 0x1AC */ s32 mShootTimer;
	/* 0x1B0 */ s32 mDroppedNum;
};

DECLARE_NERVE(TNerveKukkuGraphWander, TLiveActor);
DECLARE_NERVE(TNerveKukkuHit, TLiveActor);
DECLARE_NERVE(TNerveKukkuFall, TLiveActor);
DECLARE_NERVE(TNerveKukkuPostFall, TLiveActor);
DECLARE_NERVE(TNerveKukkuRecoverGraph, TLiveActor);

#endif
