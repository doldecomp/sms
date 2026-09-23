#ifndef ENEMY_TINKOOPA_HPP
#define ENEMY_TINKOOPA_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Strategic/Nerve.hpp>
#include <M3DUtil/M3UJoint.hpp>
#include <dolphin/mtx.h>

class TLiveActor;
class TTinKoopa;
class TCoasterKiller;
class TCoasterKillerManager;
class TGraphWeb;
class TMapCollisionMove;
class MActor;

// Names and defaults are the ones PARAM_INIT stringified into .rodata and the
// constants TTinKoopaParams' constructor stores into each TParamRT.
class TTinKoopaParams : public TSpineEnemyParams {
public:
	TTinKoopaParams(const char* prm);

	s32 getSLPartsHP() const { return mSLPartsHP.get(); }
	s32 getSLFlameHP() const { return mSLFlameHP.get(); }
	s32 getSLFlameRevivalTime() const { return mSLFlameRevivalTime.get(); }
	f32 getSLFlameDamageRadius0() const { return mSLFlameDamageRadius0.get(); }
	f32 getSLFlameDamageHeight0() const { return mSLFlameDamageHeight0.get(); }
	f32 getSLFlameDamageRadius1() const { return mSLFlameDamageRadius1.get(); }
	f32 getSLFlameDamageHeight1() const { return mSLFlameDamageHeight1.get(); }
	f32 getSLDamageRadius() const { return mSLDamageRadius.get(); }
	f32 getSLDamageHeight0() const { return mSLDamageHeight0.get(); }
	f32 getSLDamageHeight1() const { return mSLDamageHeight1.get(); }
	s32 getSLKillerInterval() const { return mSLKillerInterval.get(); }
	s32 getSLDefeatWaitTime() const { return mSLDefeatWaitTime.get(); }
	f32 getSLKillerApproachingDistance() const
	{
		return mSLKillerApproachingDistance.get();
	}

	/* 0xA8 */ TParamRT<s32> mSLPartsHP;
	/* 0xBC */ TParamRT<s32> mSLFlameHP;
	/* 0xD0 */ TParamRT<s32> mSLFlameRevivalTime;
	/* 0xE4 */ TParamRT<f32> mSLFlameDamageRadius0;
	/* 0xF8 */ TParamRT<f32> mSLFlameDamageHeight0;
	/* 0x10C */ TParamRT<f32> mSLFlameDamageRadius1;
	/* 0x120 */ TParamRT<f32> mSLFlameDamageHeight1;
	/* 0x134 */ TParamRT<f32> mSLDamageRadius;
	/* 0x148 */ TParamRT<f32> mSLDamageHeight0;
	/* 0x15C */ TParamRT<f32> mSLDamageHeight1;
	/* 0x170 */ TParamRT<s32> mSLKillerInterval;
	/* 0x184 */ TParamRT<s32> mSLDefeatWaitTime;
	/* 0x198 */ TParamRT<f32> mSLKillerApproachingDistance;
};

class TTinKoopaManager : public TEnemyManager {
public:
	TTinKoopaManager(const char* name);

	// Declared in vtable order; TTinKoopaManager adds no new slots.
	virtual ~TTinKoopaManager() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void createModelData();
	virtual BOOL hasMapCollision() const { return TRUE; }
	virtual TSpineEnemy* createEnemyInstance();
};

// The Softimage-style joint calculator the body model would have used for
// motion blending. Nothing in the retail object constructs one: the
// constructor and joinAnm() are both UNUSED, and only the virtual calc()
// override and the vtable survive.
class TTinKoopaMtxCalc : public M3UMtxCalcSIAnmBlendQuat {
public:
	TTinKoopaMtxCalc(TTinKoopa*);

	virtual ~TTinKoopaMtxCalc() { }
	virtual void calc(u16);

	void joinAnm(int);

	/* 0x64 */ TTinKoopa* mTinKoopa;
};

// One of the six armour plates (head, breast, stomach, right arm, left arm and
// leg). Each carries its own moving map collision, and the four that can be
// blown off also carry their own model so the debris can fly away on its own
// animation while the body keeps playing the damage animation.
class TTinKoopaPartsBase : public TLiveActor {
public:
	TTinKoopaPartsBase(const char* name, int index, TTinKoopa* tin_koopa);

	// Declared in vtable order; reset() is a new slot past TLiveActor.
	virtual ~TTinKoopaPartsBase() { }
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void reset();

	void initTinKoopaPartsBase();
	void resetTinKoopaPartsBase();
	void startBreaking();
	void emitPartsTrackEffects();
	void emitPartsTrackEffects(const char** joint_names, int num);
	void emitPartsDisappearEffects();
	void emitPartsDisappearEffects(const char** joint_names, int num,
	                               f32 scale);

	// fabricated
	bool isBreaking() const { return mBreaking; }
	int getPartsIndex() const { return mPartsIndex; }

	/* 0xF4 */ TMapCollisionMove* mCollision;
	/* 0xF8 */ u8 mBreaking;
	/* 0xFC */ int mPartsIndex;
	/* 0x100 */ TTinKoopa* mTinKoopa;
	// Debris model. Separate from TLiveActor::mMActor, which this actor never
	// gets: the plate is drawn by the body model until it breaks off.
	/* 0x104 */ MActor* mPartsMActor;
	/* 0x108 */ JGeometry::TVec3<f32> mEffectPos[6];
};

// The flame Mecha-Bowser breathes. It is a bare hit actor bound to the
// "fire_col_null" joint; spraying it puts it out for mSLFlameRevivalTime.
class TTinKoopaFlame : public THitActor {
public:
	TTinKoopaFlame(const char* name, TTinKoopa* tin_koopa);

	// Declared in vtable order; TTinKoopaFlame adds no new slots.
	virtual ~TTinKoopaFlame() { }
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);

	void resetTinKoopaFlame();
	void makeHitCollision();
	void emitFlameEffects();
	void checkMario();
	void hitWater();
	bool isHighPosition();

	/* 0x68 */ TTinKoopa* mTinKoopa;
	/* 0x6C */ f32 mScale;
	/* 0x70 */ s16 mHitPoints;
	/* 0x72 */ u8 mSprayed;
};

// One entry of the killer launch schedule: at animation frame mFrame of lap
// mLap, fire mCount killers (-1 means "as many as the damage stage allows")
// towards mDirection.
class TTinKoopaLaunchOrder {
public:
	TTinKoopaLaunchOrder(TTinKoopa*);

	void makeOrder(s8 lap, long frame, s8 count, s8 direction);
	void checkOrder();

	/* 0x0 */ TTinKoopa* mTinKoopa;
	/* 0x4 */ s8 mLap;
	/* 0x8 */ int mFrame;
	/* 0xC */ s8 mCount;
	/* 0xD */ u8 mDirection;
};

class TTinKoopaLaunchSchedule {
public:
	TTinKoopaLaunchSchedule(u8 num, TTinKoopa*);

	void checkOrder();

	// fabricated
	TTinKoopaLaunchOrder* getOrder(int i) { return mOrders[i]; }
	u8 getOrderNum() const { return mOrderNum; }

	/* 0x0 */ u8 mOrderNum;
	/* 0x4 */ TTinKoopa* mTinKoopa;
	/* 0x8 */ TTinKoopaLaunchOrder** mOrders;
};

enum {
	TINKOOPA_TIMER_KILLER,      // until the next Bullet Bill may launch
	TINKOOPA_TIMER_FLAME_STOP,  // flame breath held off
	TINKOOPA_TIMER_DEFEAT_WAIT, // wait after the last hit
	TINKOOPA_TIMER_NUM,
};

// Mecha-Bowser (ニセクッパ), the Pinna Park boss. It rides the roller coaster
// track with Mario, so almost everything it does is timed off the coaster's
// own animation (gpMarioOriginal->mKoopaRail) rather than off its own spine.
class TTinKoopa : public TSpineEnemy {
public:
	TTinKoopa(const char* name);

	// Declared in vtable order; TTinKoopa adds no new slots.
	virtual ~TTinKoopa() { }
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void init(TLiveManager*);
	virtual BOOL hasMapCollision() const { return TRUE; }
	virtual const char** getBasNameTable() const;
	virtual void reset();

	void resetTinKoopa();
	void makeHitCollision();
	void makeLaunchSchedule();
	void makeKillerQueue(int num, s8 direction);
	void checkKillerLaunch();
	void launchKiller(int direction);
	void startBreakingParts();
	void hitParts();
	void changeBck(int index);
	void updateTimers();
	void checkLap();
	bool checkTruckAnimationPass(int frame);
	void emitTinKoopaEffects();
	void makeEyeBeamEffect();
	void startTinKoopaMessage(u32 id);
	void checkTinKoopaMessage();
	void checkTinKoopaFirstFlameMessage();
	void checkTinKoopaFirstRocketMessage();
	void checkTinKoopaKillerApproachingMessage();
	bool checkKillerApproachingFromBack(TCoasterKiller* killer,
	                                    JGeometry::TVec3<f32> pos, f32 limit);
	void makeCoasterDistanceTable();
	f32 calcCoasterDistance(int from, int to);
	f32 calcCoasterDistanceInOrder(int from, int to);

	// fabricated
	TTinKoopaParams* getSaveParams() const
	{
		return (TTinKoopaParams*)getSaveParam();
	}
	int getDamageStage() const { return mDamageStage; }
	int getLap() const { return mLap; }
	MActor* getTruckMActor() { return mTruckMActor; }
	TTinKoopaFlame* getFlame() { return mFlame; }
	TTinKoopaPartsBase* getParts(int i) { return mParts[i]; }

	// How many plates have been knocked off; 4 means Mecha-Bowser is done.
	// It selects the wait/damage/break animation and the plate that goes next.
	/* 0x150 */ int mDamageStage;
	/* 0x154 */ int unk154;
	/* 0x158 */ int unk158;
	// Which of the three laps of the coaster track we are on.
	/* 0x15C */ int mLap;
	/* 0x160 */ TTinKoopaFlame* mFlame;
	// The coaster's own model actor, borrowed from TMario. Every scripted
	// event in the fight is keyed off a frame of its animation.
	/* 0x164 */ MActor* mTruckMActor;
	/* 0x168 */ u8 mFirstFlameMessageDone;
	/* 0x169 */ s8 mKillerDirs[4];
	/* 0x170 */ int mKillerNum;
	/* 0x174 */ int mKillerIndex;
	// Countdowns decremented together by updateTimers(); the ROM's unrolled
	// indexed loop there is the evidence for the array.
	/* 0x178 */ int mTimers[TINKOOPA_TIMER_NUM]; // TINKOOPA_TIMER_*
	/* 0x184 */ JGeometry::TVec3<f32> mHeadPos;
	/* 0x190 */ JGeometry::TVec3<f32> mBreastPos;
	/* 0x19C */ JGeometry::TVec3<f32> mRightArmPos;
	/* 0x1A8 */ JGeometry::TVec3<f32> mLeftArmPos;
	// TODO: these four are only written by resetTinKoopa (0, 30, 15, 45) and
	// only read by the UNUSED makeEyeBeamEffect, so their meaning is a guess:
	// an eye-beam sweep angle plus its length and half-angles.
	/* 0x1B4 */ f32 unk1B4;
	/* 0x1B8 */ f32 unk1B8;
	/* 0x1BC */ f32 unk1BC;
	/* 0x1C0 */ f32 unk1C0;
	/* 0x1C4 */ f32 unk1C4;
	/* 0x1C8 */ int mPartsHitPoints;
	/* 0x1CC */ TTinKoopaPartsBase* mParts[6];
	/* 0x1E4 */ TTinKoopaPartsBase* mBreakingParts;
	// Length of every link of the killer graph, so the distance between two
	// nodes is a partial sum.
	/* 0x1E8 */ f32* mCoasterDistances;
	/* 0x1EC */ TGraphWeb* mKillerGraph;
	/* 0x1F0 */ TCoasterKillerManager* mKillerManager;
	/* 0x1F4 */ TTinKoopaLaunchSchedule* mLaunchSchedule;
	/* 0x1F8 */ int unk1F8;
};

DECLARE_NERVE(TNerveTinKoopaBreak, TLiveActor)
DECLARE_NERVE(TNerveTinKoopaDamage, TLiveActor)
DECLARE_NERVE(TNerveTinKoopaWait, TLiveActor)

#endif
