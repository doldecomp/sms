#ifndef ENEMY_KUMOKUN_HPP
#define ENEMY_KUMOKUN_HPP

#include <Enemy/SmallEnemy.hpp>
#include <Strategic/Nerve.hpp>

class TGraphWeb;
class TWallAtGraph;

class TKumokunParams : public TSmallEnemyParams {
public:
	TKumokunParams(const char*);

	/* 0x2D4 */ TParamRT<f32> mTorqueY;
	/* 0x2E8 */ TParamRT<f32> mMarchSpeed;
	/* 0x2FC */ TParamRT<f32> mAttackSpeed;
	/* 0x310 */ TParamRT<s32> mMarchTimer;
	/* 0x324 */ TParamRT<s32> mWaitTimer;
	/* 0x338 */ TParamRT<f32> mSearchRange;
	/* 0x34C */ TParamRT<f32> mFlySpeed;
};

class TKumokunManager : public TSmallEnemyManager {
public:
	TKumokunManager(const char* name = "くもくんマネージャ");

	virtual void load(JSUMemoryInputStream& stream);
	virtual void createModelData();
};

class TKumokun : public TSmallEnemy {
public:
	TKumokun(const char*);

	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void control();
	virtual void bind();
	virtual void moveObject();
	virtual const char** getBasNameTable() const;
	virtual void reset();
	virtual void behaveToWater(THitActor*);
	virtual void setDeadAnm();
	virtual void attackToMario();
	virtual bool isCollidMove(THitActor*);
	virtual bool doKeepDistance();

	JGeometry::TVec3<f32> getPlaneNormal() const;
	bool isAttack() const;
	bool isOnSamePlaneWithMario() const;
	JGeometry::TVec3<f32>
	rotateGoalDirToLocal(const JGeometry::TVec3<f32>&) const;
	void initCollision();
	void calcShadowPos();
	void behaveHitWater();
	bool behaveHitPunch();
	bool behaveHitTrample();
	bool behaveHitHipdrop();
	void updateAnimation();
	void updateCollision();
	void clearAnmStack();
	void pushNextAnm(const char*, bool);
	bool checkSerialAnmEnd() const;
	void keepDistance(const THitActor&);
	void prepareWalk();
	bool doWalk();
	void decideTarget();
	void decideTargetOnFindingMario(const JGeometry::TVec3<f32>&);
	void decideTargetAtRandom();
	void decideTargetOnGraph();
	bool doAdjustTarget();
	void prepareFly();
	bool doFly();
	bool isOnFloor() const;
	bool isOnRoof() const;
	bool isWalking() const;
	bool isFlying() const;
	bool isCrashing() const;
	bool isHitPlane() const;
	bool isFindOutMario(JGeometry::TVec3<f32>*) const;
	void resetHitPlaneCounter();

	static const TBGCheckData* checkWallPlane(JGeometry::TVec3<f32>*, f32, f32);
	static const TBGCheckData* checkFloorPlane(JGeometry::TVec3<f32>*, f32,
	                                           f32);
	static const TBGCheckData* checkRoofPlane(JGeometry::TVec3<f32>*, f32);
	static bool isSameNormal(const TBGCheckData*, const TBGCheckData*);

	static bool isFenceSound(const TBGCheckData*);
	void decideTargetAtDir(const JGeometry::TVec3<f32>&);
	void bindOnFlying();
	bool checkOnMovingRoof(JGeometry::TVec3<f32>*, const TBGCheckData**,
	                       const JGeometry::TVec3<f32>&,
	                       const JGeometry::TVec3<f32>&) const;
	bool checkOnMovingFloor(JGeometry::TVec3<f32>*, const TBGCheckData**,
	                        const JGeometry::TVec3<f32>&,
	                        const JGeometry::TVec3<f32>&) const;
	bool checkOnMovingWall(JGeometry::TVec3<f32>*, const TBGCheckData**,
	                       const JGeometry::TVec3<f32>&,
	                       const JGeometry::TVec3<f32>&) const;
	void initAttachPlane();
	void changeBck(const char*);

	const JGeometry::TQuat4<f32>& getQuat() const { return unk19C; }

	// fabricated
	TKumokunParams* getSaveParam2() const
	{
		return (TKumokunParams*)getSaveParam();
	}

	// fabricated
	bool isOnWall() const { return !isOnFloor() && !isOnRoof(); }

public:
	/* 0x194 */ int mHitPlaneCounter;
	/* 0x198 */ const TBGCheckData* unk198;
	/* 0x19C */ JGeometry::TQuat4<f32> unk19C;
	/* 0x1AC */ JGeometry::TQuat4<f32> unk1AC;
	/* 0x1BC */ JGeometry::TQuat4<f32> unk1BC;
	/* 0x1CC */ f32 unk1CC;
	/* 0x1D0 */ int unk1D0;
	/* 0x1D4 */ bool unk1D4;
	/* 0x1D8 */ TSolidStack<const char*> unk1D8;
	/* 0x1E8 */ TWallAtGraph* unk1E8;
	/* 0x1EC */ u32 unk1EC;
};

class TLiveActor;

DECLARE_NERVE(TNerveKumokunPreFly, TLiveActor);
DECLARE_NERVE(TNerveKumokunFly, TLiveActor);
DECLARE_NERVE(TNerveKumokunPostFreeze, TLiveActor);
DECLARE_NERVE(TNerveKumokunFreeze, TLiveActor);
DECLARE_NERVE(TNerveKumokunWait, TLiveActor);
DECLARE_NERVE(TNerveKumokunSearch, TLiveActor);
DECLARE_NERVE(TNerveKumokunPostWalk, TLiveActor);
DECLARE_NERVE(TNerveKumokunWalk, TLiveActor);
DECLARE_NERVE(TNerveKumokunPreWalk, TLiveActor);

#endif
