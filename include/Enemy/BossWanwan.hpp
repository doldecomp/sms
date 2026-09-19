#ifndef ENEMY_BOSS_WANWAN_HPP
#define ENEMY_BOSS_WANWAN_HPP

#include <Strategic/Binder.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/Nerve.hpp>
#include <Strategic/TakeActor.hpp>
#include <M3DUtil/M3UJoint.hpp>
#include <JSystem/JGeometry.hpp>
#include <JSystem/JDrama/JDRViewObj.hpp>
#include <Camera/CameraShake.hpp>
#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>

class MActor;
class TBossWanwan;
class TBWLeash;
class TRope;

// The boss chain chomp of Bianco Hills episode 1/2. The body is a
// TSpineEnemy, the chain is a TBWLeash of fifteen TBWLeashNode hit actors
// hanging off a TRope, and the stake that holds the chain down is a
// TBWPicket that Mario can pull out. Two extra TBWHit actors give the head
// and the body their own attack volumes.

// Balloon hints raised once each; the bit is remembered in mBalloonsShown.
enum {
	BALLOON_MSG_BWANWAN_PULL_PICKET = 0x1A,
	BALLOON_MSG_BWANWAN_BARKED      = 0x1B,
	BALLOON_MSG_BWANWAN_LEAD_TO_HOT = 0x1C,
	BALLOON_MSG_BWANWAN_SPRAY_IT    = 0x1D,
};

enum {
	BWANWAN_SHOWN_PULL_PICKET = 0x1,
	BWANWAN_SHOWN_BARKED      = 0x2,
	BWANWAN_SHOWN_LEAD_TO_HOT = 0x4,
	BWANWAN_SHOWN_SPRAY_IT    = 0x8,
};

// BCK slot indices. The .bas table names 0, 2, 4 and 5; the .bck files are
// indexed alphabetically, so the three gaps sit between "bark" and "shake",
// between "shake" and "wait", and after "wait2".
enum {
	BWANWAN_BCK_BARK  = 0,
	BWANWAN_BCK_DOWN  = 1,
	BWANWAN_BCK_SHAKE = 2,
	BWANWAN_BCK_UNK3  = 3,
	BWANWAN_BCK_WAIT  = 4, // played while the boss still has hit points
	BWANWAN_BCK_WAIT2 = 5, // played once the boss has cooled down
	BWANWAN_BCK_WAKE  = 6,
};

class TBWParams : public TSpineEnemyParams {
public:
	TBWParams(const char*);

	/* 0xA8 */ TParamRT<f32> mSLMarchSpeed;
	/* 0xBC */ TParamRT<f32> mSLTurnSpeed;
	/* 0xD0 */ TParamRT<f32> mSLLeashNodeLen;
	/* 0xE4 */ TParamRT<f32> mSLPicketHeight;
	/* 0xF8 */ TParamRT<f32> mSLPicketRadius;
	/* 0x10C */ TParamRT<f32> mSLChainHitHeight;
	/* 0x120 */ TParamRT<f32> mSLChainHitRadius;
	/* 0x134 */ TParamRT<f32> mSLChainGroundRadius;
	/* 0x148 */ TParamRT<f32> mSLPullLimit;
	/* 0x15C */ TParamRT<f32> mSLAttackSpeed;
	/* 0x170 */ TParamRT<s32> mSLStunTimer;
	/* 0x184 */ TParamRT<f32> mSLSearchLength;
	/* 0x198 */ TParamRT<f32> mSLSearchAngle;
	/* 0x1AC */ TParamRT<u8> mSLBWHitPointMax;
	/* 0x1C0 */ TParamRT<f32> mSLHeadGap;
	/* 0x1D4 */ TParamRT<f32> mSLShakeLengthMax;
	/* 0x1E8 */ TParamRT<f32> mSLShakeLengthMaxHP0;
};

// One link of the chain. Every link is its own hit actor so the chain can
// burn Mario, and carries a temperature that trickles down from the link
// closer to the head.
class TBWLeashNode : public THitActor {
public:
	TBWLeashNode(TBWLeash*, int, const char*);

	// Declared in vtable order; TBWLeashNode adds no new slots.
	virtual ~TBWLeashNode() { }
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	void calcMatrix();
	void calcTemperature();

	// fabricated
	f32 getTemperature() const { return mTemperature; }
	void setTemperature(f32 t) { mTemperature = t; }

public:
	/* 0x68 */ TBWLeash* mLeash;
	/* 0x6C */ MActor* mMActor;
	/* 0x70 */ f32 mTemperature;
	/* 0x74 */ int mIndex;
};

// The chain: a rope simulation plus one TBWLeashNode model per rope point.
class TBWLeash : public JDrama::TViewObj {
public:
	TBWLeash(TBossWanwan*, int, const char*);

	// Declared in vtable order; TBWLeash adds no new slots.
	virtual ~TBWLeash() { }
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	void pullTail(const JGeometry::TVec3<f32>&);
	void invalidateAllCollision();

	// fabricated
	TRope* getRope() { return mRope; }
	TBWLeashNode* getNode(int i) { return mNodes[i]; }

public:
	/* 0x10 */ TBossWanwan* mOwner;
	/* 0x14 */ TRope* mRope;
	/* 0x18 */ TBWLeashNode** mNodes;
};

// The stake at the end of the chain. Mario can grab it and drag it around;
// pulling it out of the ground frees the boss.
class TBWPicket : public TTakeActor {
public:
	TBWPicket(TBossWanwan*, const char*);

	// Declared in vtable order.
	virtual ~TBWPicket() { }
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual MtxPtr getTakingMtx();
	virtual BOOL moveRequest(const JGeometry::TVec3<f32>& where_to);

public:
	/* 0x70 */ TBossWanwan* mOwner;
	/* 0x74 */ JGeometry::SMatrix34C<f32> mTakingMtx;
	/* 0xA4 */ MActor* mMActor;
};

// A bare attack volume pinned to one of the boss's joints. Index 3 is the
// head; index -1 keeps the volume at the position perform() is handed.
class TBWHit : public THitActor {
public:
	TBWHit(TBossWanwan*, int, const char*);

	// Declared in vtable order; TBWHit adds no new slots.
	virtual ~TBWHit() { }
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);

public:
	/* 0x68 */ TBossWanwan* mOwner;
	/* 0x6C */ int mJointIndex;
};

// Keeps the boss on its graph, rolls the body by the distance travelled and
// reels it back in when the chain runs out.
class TBWBinder : public TBinder {
public:
	TBWBinder();

	virtual ~TBWBinder() { }
	virtual void bind(TLiveActor*);
};

// Adds the body roll on top of the Softimage joint animation.
class TBossWanwanMtxCalc : public M3UMtxCalcSIAnmBlendQuat {
public:
	TBossWanwanMtxCalc(TBossWanwan*);

	virtual ~TBossWanwanMtxCalc() { }
	virtual void calc(u16);

	void joinAnm(int);

public:
	/* 0x64 */ TBossWanwan* mOwner;
};

class TBossWanwan : public TSpineEnemy {
public:
	TBossWanwan(const char* name = "ボスワンワン");

	// Declared in vtable order.
	virtual ~TBossWanwan() { }
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void control();
	// Weak in the map, so in-class, and 0x4 bytes: the boss never dies the
	// ordinary way; the Die nerve takes it apart instead.
	virtual void kill() { }

	void emitEffects();
	void shakeCamera(int);
	void slideToCurPathNode(f32 march_speed, f32 turn_speed);
	void showMessage(u32);
	void rollNextGraphNode();
	void reverseNextGraphNode();
	BOOL isHeadPulled();
	bool isMarioInSight();
	bool isTailBurning();
	bool isBurning();
	void startGoldBrk();
	void changeBck(int);
	void releasePicket();
	void takeBath();

	// fabricated, and the inline level is the point of it: retail calls
	// TSpineBase<TLiveActor>::getLatestNerve() (weak, 0x1c, linked from
	// Animal/Bird.o) from TBWPicket::moveRequest, while a direct
	// mOwner->mSpine->getLatestNerve() expands it. TBossGesso has the same
	// one-line forwarder for the same reason; see the note in bosswanwan.cpp.
	const TNerveBase<TLiveActor>* getLatestNerve() const
	{
		return mSpine->getLatestNerve();
	}

	// fabricated
	TBWParams* getSaveParam2() const { return (TBWParams*)getSaveParam(); }
	TBWLeash* getLeash() { return mLeash; }
	TBWPicket* getPicket() { return mPicket; }
	bool isPicketFixed() const { return mIsPicketFixed; }
	bool checkBalloonShown(u32 mask) const { return mBalloonsShown & mask; }
	void onBalloonShown(u32 mask) { mBalloonsShown |= mask; }

public:
	/* 0x150 */ TBossWanwanMtxCalc* mMtxCalc;
	/* 0x154 */ TBWLeash* mLeash;
	/* 0x158 */ TBWPicket* mPicket;
	/* 0x15C */ JGeometry::TVec3<f32> mPullVelocity;
	/* 0x168 */ f32 mRollAngle;
	/* 0x16C */ int mIsRolling;
	/* 0x170 */ TBWHit* mHits[2];
	/* 0x178 */ f32 mMotionBlendStep;
	/* 0x17C */ int mIsPicketPlanted;
	/* 0x180 */ int mCoolDownTimer;
	/* 0x184 */ int mPulledTimer;
	/* 0x188 */ int mIsLeashStretched;
	/* 0x18C */ s8 mIsInBath;
	/* 0x18D */ s8 mShineAppeared;
	/* 0x190 */ int mSparkRequest;
	/* 0x194 */ s8 mIsPicketFixed;
	/* 0x195 */ s8 mSplashDone;
	/* 0x198 */ u32 mBalloonsShown;
	/* 0x19C */ int mIdleTimer;
	/* 0x1A0 */ s8 mWasSprayed;
	/* 0x1A4 */ JGeometry::TVec3<f32> mJumpSmokePos;
	/* 0x1B0 */ int unk1B0;
	/* 0x1B4 */ s16 unk1B4;
};

class TBossWanwanManager : public TEnemyManager {
public:
	TBossWanwanManager(const char* name = "ボスワンワンマネージャー");

	// Declared in vtable order; TBossWanwanManager adds no new slots.
	virtual ~TBossWanwanManager() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();

	void initJParticle();
};

DECLARE_NERVE(TNerveBWGraphWander, TLiveActor);
DECLARE_NERVE(TNerveBWRoll, TLiveActor);
DECLARE_NERVE(TNerveBWBark, TLiveActor);
DECLARE_NERVE(TNerveBWJump, TLiveActor);
DECLARE_NERVE(TNerveBWStun, TLiveActor);
DECLARE_NERVE(TNerveBWWakeup, TLiveActor);
DECLARE_NERVE(TNerveBWJumpToBath, TLiveActor);
DECLARE_NERVE(TNerveBWDie, TLiveActor);
DECLARE_NERVE(TNerveBWJumpAway, TLiveActor);
DECLARE_NERVE(TNerveBWShake, TLiveActor);
DECLARE_NERVE(TNerveBWFall, TLiveActor);

#endif
