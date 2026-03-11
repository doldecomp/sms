#ifndef ENEMY_BATHTUB_KILLER_HPP
#define ENEMY_BATHTUB_KILLER_HPP

#include <Enemy/SmallEnemy.hpp>
#include <Strategic/Nerve.hpp>

class TBathtubKillerParams;
class TBathtub;

class TBathtubKillerPersonality {
public:
	TBathtubKillerPersonality();

	void makeFast(const TBathtubKillerParams*);
	void makeShine(const TBathtubKillerParams*);
	void makeNormal(const TBathtubKillerParams*);
};

class TBathtubKillerParams : public TSmallEnemyParams {
public:
	TBathtubKillerParams(const char* name = "バスタブキラー");

	/* 0x2d4 */ TParamRT<f32> fastAccelerationQuatRate;
	/* 0x2e8 */ TParamRT<f32> fastChaseAcceleration;
	/* 0x2fc */ TParamRT<f32> fastChaseSpeed;
	/* 0x310 */ TParamRT<f32> fastInitialSpeed;
	/* 0x324 */ TParamRT<s32> fastDeadPeriod;
	/* 0x338 */ TParamRT<f32> shineAccelerationQuatRate;
	/* 0x34c */ TParamRT<f32> shineChaseAcceleration;
	/* 0x360 */ TParamRT<f32> shineChaseSpeed;
	/* 0x374 */ TParamRT<f32> shineInitialSpeed;
	/* 0x388 */ TParamRT<s32> shineDeadPeriod;
	/* 0x39c */ TParamRT<f32> mushroomProbability;
	/* 0x3b0 */ TParamRT<f32> mSLColorChangeRateDelta;
	/* 0x3c4 */ TParamRT<f32> mSLAccelerationQuatRate;
	/* 0x3d8 */ TParamRT<f32> mSLChaseAcceleration;
	/* 0x3ec */ TParamRT<f32> mSLChaseSpeed;
	/* 0x400 */ TParamRT<f32> mSLInitialSpeed;
	/* 0x414 */ TParamRT<s32> mSLDeadPeriod;
	/* 0x428 */ TParamRT<f32> mSLStraightDistance;
	/* 0x43c */ TParamRT<f32> mSLChaseMinY;
	/* 0x450 */ TParamRT<f32> mSLChaseMaxY;
	/* 0x464 */ TParamRT<f32> mSLAboidDistanceY;
	/* 0x478 */ TParamRT<f32> mSLAboidDistance;
	/* 0x48c */ TParamRT<f32> mSLChaseDistanceY;
	/* 0x4a0 */ TParamRT<f32> mSLChaseDistance;
	/* 0x4b4 */ TParamRT<f32> mSLTrampleVelocity;
	/* 0x4c8 */ TParamRT<f32> mSLFlyingSpeedMax;
	/* 0x4dc */ TParamRT<f32> mSLFlyingGravityY;
	/* 0x4f0 */ TParamRT<f32> mSLBombRange;
	/* 0x504 */ TParamRT<f32> aboidAngle;
	/* 0x518 */ TParamRT<s32> mSLChaseStraightPeriod;
	/* 0x52c */ TParamRT<s32> mSLSmokeInterval;
	/* 0x540 */ TParamRT<s32> mSLLaunchingPeriod;
	/* 0x554 */ TParamRT<s32> noCollisionAmongKillers;
};

class TBathtubKiller : public TSmallEnemy {
public:
	TBathtubKiller(const char* name = "バスタブキラー");

	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void bind();
	virtual f32 getGravityY() const;
	virtual const char** getBasNameTable() const;
	virtual void reset();
	virtual void behaveToWater(THitActor*);
	virtual void attackToMario();
	virtual void setMActorAndKeeper();
	virtual bool isCollidMove(THitActor*);

	void resetBathtubKiller();
	void generateItemBathtubKiller();
	void killBathtubKiller();
	void breakBathtubKiller();
	void explodeBathtubKiller();
	void makeNoseColor();
	f32 getBathtubY();
	void makeInitialVelocity(JGeometry::TVec3<f32>);
	void moveParabolic();
	void moveChasing();
	void moveStraight();
	void makeVelocityQuat();
	void makeAccelerationQuat();
	void makeQuat(JGeometry::TVec3<f32>, f32, f32);
	void makeScrewQuat(JGeometry::TVec3<f32>, f32, f32);
	void setNormalBathtubKillerAnm();
	void setChaseBathtubKillerAnm();
	void setStraightBathtubKillerAnm();
	void setDeadBathtubKillerAnm();
	void updateTimers();
	bool isAttackable();
	bool isAboided();
	bool canChase();
	void generateExplosion();

	// fabricated
	TBathtubKillerParams* getSaveParam2() const
	{
		return (TBathtubKillerParams*)getSaveParam();
	}

public:
	/* 0x194 */ u8 unk194;
	/* 0x198 */ f32 unk198;
	/* 0x19C */ f32 unk19C;
	/* 0x1A0 */ f32 unk1A0;
	/* 0x1A4 */ f32 unk1A4;
	/* 0x1A8 */ s32 unk1A8;
	/* 0x1AC */ JGeometry::TQuat4<f32> mQuat;
	/* 0x1BC */ JGeometry::TVec3<f32> unk1BC;
	/* 0x1C8 */ char unk1C8[4];
	/* 0x1CC */ TBathtub* unk1CC;
	/* 0x1D0 */ char unk1D0[4];
	/* 0x1D4 */ int unk1D4;
	/* 0x1D8 */ GXColorS10 unk1D8;
	/* 0x1E0 */ GXColorS10 unk1E0;
	/* 0x1E8 */ GXColorS10 unk1E8;
	/* 0x1F0 */ GXColorS10 unk1F0;
	/* 0x1F8 */ f32 unk1F8;
	/* 0x1FC */ f32 unk1FC;
	/* 0x200 */ f32 unk200;
	/* 0x204 */ f32 unk204;
	/* 0x208 */ int unk208;
	/* 0x20C */ int unk20C;
	/* 0x210 */ int unk210;
	/* 0x214 */ int unk214;
	/* 0x218 */ int unk218;
	/* 0x21C */ u32 unk21C;
	/* 0x220 */ TMtx34f unk220;
};

DECLARE_NERVE(TNerveBathtubKillerWander, TLiveActor);
DECLARE_NERVE(TNerveBathtubKillerChase, TLiveActor);
DECLARE_NERVE(TNerveBathtubKillerChaseStraight, TLiveActor);
DECLARE_NERVE(TNerveBathtubKillerStraight, TLiveActor);
DECLARE_NERVE(TNerveBathtubKillerBreak, TLiveActor);
DECLARE_NERVE(TNerveBathtubKillerExplosion, TLiveActor);

class TBathtubKillerManager : public TSmallEnemyManager {
public:
	TBathtubKillerManager(const char* name = "バスタブキラーマネージャー");

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();

	void generateMushroom(JGeometry::TVec3<f32>);
	int countActiveKillers();
	int countActiveShineKillers();
};

#endif
