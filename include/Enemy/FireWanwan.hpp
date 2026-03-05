#ifndef ENEMY_FIRE_WANWAN_HPP
#define ENEMY_FIRE_WANWAN_HPP

#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <System/ArrayWrapper.hpp>
#include <Enemy/SmallEnemy.hpp>

struct TBGWallCheckRecord;
class JAISound;

// TODO: mark all virtual methods

class TLerpControl {
public:
	void init(const GXColorS10&, const GXColorS10&, f32);
	void update();

public:
	/* 0x0 */ GXColorS10 unk0;
	/* 0x8 */ GXColorS10 unk8;
	/* 0x10 */ GXColorS10 unk10;
	/* 0x18 */ J3DFrameCtrl unk18;
};

class TTailRubber {
public:
	struct Node {
		/* 0x0 */ JGeometry::TVec3<f32> mPos;
		/* 0xC */ JGeometry::TVec3<f32> mVel;
	};

	TTailRubber(int);
	void reset(const JGeometry::TVec3<f32>&, const JGeometry::TVec3<f32>&);
	void setHeadPos(const JGeometry::TVec3<f32>&);
	void setTailPos(const JGeometry::TVec3<f32>&);
	void movement();
	void bind();
	void bindOne(TTailRubber::Node&);
	void adjust();
	void adjustOne(TTailRubber::Node&, const TTailRubber::Node&,
	               const TTailRubber::Node&);
	void restrict();
	f32 getLength() const;

	static u32 cNodeRadius;

	// fabricated
	Node* getNode(int idx) { return &unk0[idx * 0.25f * 4.0f]; }

public:
	/* 0x0 */ ArrayWrapper<Node> unk0;
	/* 0x8 */ bool unk8;
	/* 0x9 */ bool mIsHeld;
	/* 0xC */ f32 mBoundRate;
	/* 0x10 */ f32 mDecay;
	/* 0x14 */ f32 mMaxLength;
};

class TFireWanwanSaveLoadParams : public TSmallEnemyParams {
public:
	TFireWanwanSaveLoadParams(const char*);

	/* 0x2D4 */ TParamRT<f32> mMarchSpeed;
	/* 0x2E8 */ TParamRT<f32> mAttackSpeedRate;
	/* 0x2FC */ TParamRT<s32> mAttackTimerMax;
	/* 0x310 */ TParamRT<f32> mEscapeSpeedRate;
	/* 0x324 */ TParamRT<s32> mEscapeRecoverTime;
	/* 0x338 */ TParamRT<f32> mSLFlySpeed;
	/* 0x34C */ TParamRT<f32> mSLFlyGravityY;
	/* 0x360 */ TParamRT<f32> mAirFric;
	/* 0x374 */ TParamRT<f32> mFloorFric;
	/* 0x388 */ TParamRT<f32> mRotateY;
	/* 0x39C */ TParamRT<s32> mNoCollideTimerMax;
	/* 0x3B0 */ TParamRT<s32> mStopSearchTimerMax;
	/* 0x3C4 */ TParamRT<f32> mTailLengthToFly;
	/* 0x3D8 */ TParamRT<f32> mCamShakeRange;
	/* 0x3EC */ TParamRT<f32> mContShakeRange;
	/* 0x400 */ TParamRT<s32> mPolluteTimerMax;
	/* 0x414 */ TParamRT<f32> mFreezeTimeThreshold;
	/* 0x428 */ TParamRT<s32> mSLFreezeWaitShort;
	/* 0x43C */ TParamRT<f32> mRecoverRate;
	/* 0x450 */ TParamRT<f32> mPolluteNormalRate;
	/* 0x464 */ TParamRT<f32> mPolluteAttackRate;
	/* 0x478 */ TParamRT<f32> mTailEndPowRate;
	/* 0x48C */ TParamRT<f32> mRubberBoundRate;
	/* 0x4A0 */ TParamRT<f32> mRubberDecay;
	/* 0x4B4 */ TParamRT<f32> mRubberBoundRateHitting;
	/* 0x4C8 */ TParamRT<f32> mRubberDecayHitting;
	/* 0x4DC */ TParamRT<f32> mTailEndColRange;
	/* 0x4F0 */ TParamRT<f32> mTailApartPowRate;
	/* 0x504 */ TParamRT<f32> mTailMaxLength;
	/* 0x518 */ TParamRT<f32> mBoss22HelpRange;
	/* 0x52C */ TParamRT<s32> mBoss23TimerMax;
};

class TFireWanwan;

class TFireWanwanManager : public TSmallEnemyManager {
public:
	enum BodyMsgType { };

	TFireWanwanManager(const char*);
	void load(JSUMemoryInputStream&);
	void createModelData();
	void perform(u32, JDrama::TGraphics*);

	void checkBalloonHelpBoss22();
	void checkBalloonHelpBoss23();
	void checkBalloonHelpBoss24();
	void checkShineAppear();
	void receiveMessageFromTail(int);
	void receiveMessageFromBody(const TFireWanwan*, BodyMsgType);

	static u32 cFailCountMax;

	// fabricated
	TFireWanwanSaveLoadParams* getWanwanParams()
	{
		return (TFireWanwanSaveLoadParams*)unk38;
	}

public:
	/* 0x60 */ bool mBoss22BalloonWasShown;
	/* 0x61 */ bool mShineAppeared;
	/* 0x64 */ int unk64;
	/* 0x68 */ int mWanwanRecoversBeforeHelpBalloon;
	/* 0x6C */ int mWanwansKilled;
	/* 0x70 */ TFireWanwan* mLastKillerWanwan;
};

class TFireWanwanTailNode {
public:
	TFireWanwanTailNode(MActor*);
	void setBarAnmMtx(MtxPtr);
	void perform(u32, JDrama::TGraphics*, const JGeometry::TVec3<f32>&,
	             const JGeometry::TVec3<f32>&);

public:
	/* 0x0 */ MActor* mMActor;
	/* 0x4 */ JGeometry::TVec3<f32> mScale;
	/* 0x10 */ int unk10;
	/* 0x14 */ u32 mJointIdx;
};

class TFireWanwanTailHit : public TTakeActor {
public:
	TFireWanwanTailHit(TFireWanwan&);

	BOOL receiveMessage(THitActor*, u32);
	MtxPtr getTakingMtx();
	bool moveRequest(const JGeometry::TVec3<f32>&);
	void behaveTaken(THitActor*);
	void behaveApart();
	void init();
	void perform(u32, JDrama::TGraphics*);
	void performNodes(u32, JDrama::TGraphics*);
	void clipNodes(JDrama::TGraphics*);
	void movementBody(const JGeometry::TVec3<f32>&);
	void bindBody();
	JGeometry::TVec3<f32> getBodyNthPos(int) const;
	f32 getBodyTailPow() const;
	f32 getBodyHeadPow() const;
	f32 calcApartPow();
	void onFireEffect();
	void offFireEffect();
	void changeBodyToRed(f32);
	void changeBodyToBlack(f32);
	void changeBodyToSilver(f32);
	f32 getTailLength() const;
	const JGeometry::TVec3<f32>& getHostPos() const;

public:
	/* 0x70 */ TFireWanwan* mOwner;
	/* 0x74 */ TPosition3f unk74;
	/* 0xA4 */ TTailRubber* unkA4;
	/* 0xA8 */ TFireWanwanTailNode* unkA8[5];
	/* 0xBC */ TLerpControl* unkBC;
	/* 0xC0 */ bool mIsOnFire;
	/* 0xC4 */ f32 mCurTailLength;
	/* 0xC8 */ f32 mPrevTailLength;
	/* 0xCC */ f32 mThrowPow;
};

class TFireWanwan : public TSmallEnemy {
public:
	TFireWanwan(const char*);

	void init(TLiveManager*);
	void setMActorAndKeeper();
	void reset();
	void initParticle();
	void doWalk();
	void doEscape();
	bool doAttack();
	void decideTarget(const JGeometry::TVec3<f32>&);
	void doAdjustTarget();
	int getFreezeTime() const;
	bool isFindMario(f32);
	bool isMissMario() const;
	bool isOverApproachRumble() const;
	bool isOverHungTailRumble() const;
	BOOL receiveMessage(THitActor*, u32);
	void behaveToWater(THitActor*);
	void behaveHitComrades();
	void changeBodyToRed(f32);
	void changeBodyToBlack(f32);
	void changeBodyToSilver(f32);
	void startThrownSound();
	void stopTriggerSound();
	void checkInPond();
	void kill();
	bool isHitValid(u32);
	void perform(u32, JDrama::TGraphics*);
	void calcRootMatrix();
	void moveObject();
	void updateCollisionFromParam();
	void updateCameraShake();
	void updateRumble();
	void updatePollute();
	void updateHitPoint();
	void emitEffects();
	void emitEffectsOnHittingWall(const JGeometry::TVec3<f32>&,
	                              const JGeometry::TVec3<f32>&);
	void checkHitActors();
	void checkHungTail();
	void emitTailHitEffect();
	void initTurnNextGraphNode();
	void initEscapeNextGraphNode();
	void recoverFire();
	bool isDefeat() const;
	bool canTakenByMario() const;
	bool isTurning() const;
	bool isFlying() const;
	bool isWalking() const;
	bool isWandering() const;
	bool isAttacking() const;
	bool isFreeze() const;
	bool isReadyToFly() const;
	bool isRecovering() const;
	bool isCameraShake() const;
	bool isHungTailNerve() const;
	void prepareTurn();
	bool doTurn();
	void sendAttackMsgToMario();
	void attackToMario();
	void bind();
	int bindBody(JGeometry::TVec3<f32>*, JGeometry::TVec3<f32>*,
	             const JGeometry::TVec3<f32>&);
	void bindPoint(JGeometry::TVec3<f32>*, const JGeometry::TVec3<f32>&,
	               const JGeometry::TVec3<f32>&, f32, TBGWallCheckRecord*);
	bool checkWalls(JGeometry::TVec3<f32>*, TBGWallCheckRecord*, f32);
	bool behaveHitWallOnFlying(const TBGCheckData*);
	void calcShadowPos();
	void calcRipplePos();
	f32 getGravityY() const;
	MtxPtr getTailMtx() const;
	const char** getBasNameTable() const;
	bool doKeepDistance();
	bool isCollidMove(THitActor*);

	static u8 mTailJntIndex;

	TFireWanwanSaveLoadParams* getSaveParam2() const
	{
		return (TFireWanwanSaveLoadParams*)getSaveParam();
	}

public:
	/* 0x194 */ TFireWanwanTailHit* unk194;
	/* 0x198 */ f32 mTurnTargetAngle;
	/* 0x19C */ int mNoCollideTimer;
	/* 0x1A0 */ int mStopSearchTimer;
	/* 0x1A4 */ int mApproachRumbleTimer;
	/* 0x1A8 */ int mHungTailRumbleTimer;
	/* 0x1AC */ int mPolluteTimer;
	/* 0x1B0 */ int mFreezeWait;
	/* 0x1B4 */ f32 mRecoverTimer;
	/* 0x1B8 */ JAISound* unk1B8;
	/* 0x1BC */ JGeometry::TQuat4<f32> unk1BC;
	/* 0x1CC */ JGeometry::TQuat4<f32> unk1CC;
	/* 0x1DC */ JGeometry::TVec3<f32> mInitialPosition;
	/* 0x1E8 */ int mCenterJointIdx;
	/* 0x1EC */ int mHeadJointIdx;
	/* 0x1F0 */ JGeometry::TVec3<f32> unk1F0;
	/* 0x1FC */ Mtx unk1FC;
	/* 0x22C */ JGeometry::TVec3<f32> mRipplePos;
	/* 0x238 */ TLerpControl* unk238;
};

DECLARE_NERVE(TNerveFireWanwanGraphWander, TLiveActor);
DECLARE_NERVE(TNerveFireWanwanTurn, TLiveActor);
DECLARE_NERVE(TNerveFireWanwanFindMario, TLiveActor);
DECLARE_NERVE(TNerveFireWanwanAttack, TLiveActor);
DECLARE_NERVE(TNerveFireWanwanTired, TLiveActor);
DECLARE_NERVE(TNerveFireWanwanRecoverGraph, TLiveActor);
DECLARE_NERVE(TNerveFireWanwanRecover, TLiveActor);
DECLARE_NERVE(TNerveFireWanwanDie, TLiveActor);
DECLARE_NERVE(TNerveFireWanwanHungTail, TLiveActor);
DECLARE_NERVE(TNerveFireWanwanFly, TLiveActor);
DECLARE_NERVE(TNerveFireWanwanFreeze, TLiveActor);
DECLARE_NERVE(TNerveFireWanwanEscape, TLiveActor);

#endif
