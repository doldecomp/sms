#ifndef ENEMY_BOSS_PAKKUN_HPP
#define ENEMY_BOSS_PAKKUN_HPP

#include <Strategic/HitActor.hpp>
#include <Strategic/Nerve.hpp>
#include <M3DUtil/M3UJoint.hpp>
#include <JSystem/JGeometry.hpp>
#include <JSystem/JDrama/JDRViewObj.hpp>
#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>

class MActor;
class TBossPakkun;
class TLiveActor;
class TAreaCylinderManager;
class TWaterEmitInfo;

// Petey Piranha. Two managers are registered: "BossPakkunManager" with flag 0
// (the full fight, which flies, spits pollution balls and summons a tornado)
// and "KBossPakkunManager" with flag 1 -- the 軽 ("light") variant, which only
// loads three models, no particles, and runs the TNerveBPWaitL/TNerveBPCannonL
// pair.
//
// The body is a TSpineEnemy; the head and the navel are separate THitActors so
// that the mouth can be sprayed and the belly hip-dropped independently, and
// three more actors carry the attacks: TBPPolDrop (the lobbed pollution ball),
// TBPVomit (the puddle it throws up) and TBPTornado (the spiralling twister).

// The four camera shakes this boss uses. TCameraShake::mCamShakeNameSave
// (src/Camera/CamShakeDefine.cpp) names entries 0x0E-0x11
// "/Camera/shakeBopaDown.prm", "shakeBopaPopo.prm", "shakeBopaGetup.prm" and
// "shakeBopaDownLoop.prm", and "Bopa" is this boss.

// BCK slot indices into bosspakkun_bastable. The table names 21 of the 30
// slots; the .bck files are indexed alphabetically, so the gaps can be read off
// their neighbours. 25 is certain ("bosspaku_wait" is a literal in .rodata and
// changeBck compares against it); 7 and 9 complete the usual end/loop/start
// triples, and 0, 1, 5 and 27-29 have no evidence at all.
enum {
	BOSSPAKU_BCK_UNK0          = 0,
	BOSSPAKU_BCK_UNK1          = 1,
	BOSSPAKU_BCK_BALL_END      = 2,
	BOSSPAKU_BCK_DOWN          = 3,
	BOSSPAKU_BCK_DOWN_END      = 4,
	BOSSPAKU_BCK_UNK5          = 5,
	BOSSPAKU_BCK_DOWN_LOOP     = 6,
	BOSSPAKU_BCK_DOWN_START    = 7, // TODO: guessed from the alphabetical gap
	BOSSPAKU_BCK_FALL_END      = 8,
	BOSSPAKU_BCK_FALL_LOOP     = 9, // TODO: guessed from the alphabetical gap
	BOSSPAKU_BCK_FALL_START    = 10,
	BOSSPAKU_BCK_FLY           = 11,
	BOSSPAKU_BCK_FLY_POLLUT    = 12,
	BOSSPAKU_BCK_FLY_START     = 13,
	BOSSPAKU_BCK_GETUP         = 14,
	BOSSPAKU_BCK_HEAD          = 15,
	BOSSPAKU_BCK_HOVERING      = 16,
	BOSSPAKU_BCK_JUMP_REACTION = 17,
	BOSSPAKU_BCK_LAND          = 18,
	BOSSPAKU_BCK_PANPAN        = 19,
	BOSSPAKU_BCK_POLLUT_END    = 20,
	BOSSPAKU_BCK_POLLUT_START  = 21,
	BOSSPAKU_BCK_RETURN        = 22,
	BOSSPAKU_BCK_SLEEP         = 23,
	BOSSPAKU_BCK_TORNADO       = 24,
	BOSSPAKU_BCK_WAIT          = 25,
	BOSSPAKU_BCK_WATER_HIT     = 26,
	BOSSPAKU_BCK_UNK27         = 27,
	BOSSPAKU_BCK_UNK28         = 28,
	BOSSPAKU_BCK_UNK29         = 29,
};

// mUnk16C. The value decides who may be hurt and which reaction runs; 1 is the
// only state in which the navel accepts a hip drop and Mario can ride the boss.
enum {
	BOSSPAKU_STATE_NORMAL   = 0,
	BOSSPAKU_STATE_BELLY_UP = 1,
	BOSSPAKU_STATE_UNK2     = 2,
	BOSSPAKU_STATE_FLYING   = 3,
};

// TBPPolDrop::mState.
enum {
	BOSSPAKU_POLDROP_DEAD    = 0,
	BOSSPAKU_POLDROP_FLYING  = 1,
	BOSSPAKU_POLDROP_STAMPED = 2,
};

// TBPTornado::mState.
enum {
	BOSSPAKU_TORNADO_DEAD    = 0,
	BOSSPAKU_TORNADO_MOVING  = 1,
	BOSSPAKU_TORNADO_VANISH  = 2,
};

class TBossPakkunParams : public TSpineEnemyParams {
public:
	TBossPakkunParams(const char* prm);

	/* 0xA8 */ TParamRT<s32> mSLWaitFrameStg0;
	/* 0xBC */ TParamRT<s32> mSLWaterMarkLimit;
	/* 0xD0 */ TParamRT<f32> mSLSwingLength;
	/* 0xE4 */ TParamRT<f32> mSLPollBallStampScale;
	/* 0xF8 */ TParamRT<s32> mSLTumbleTime;
	/* 0x10C */ TParamRT<s32> mSLAnmBlendTime0;
	/* 0x120 */ TParamRT<f32> mSLFlySpeed;
	/* 0x134 */ TParamRT<f32> mSLPivotSpeed;
	/* 0x148 */ TParamRT<f32> mSLPivotSpeedAware;
	/* 0x15C */ TParamRT<f32> mSLVomitAnmRate;
	/* 0x170 */ TParamRT<f32> mSLHeadHomingLimit;
	/* 0x184 */ TParamRT<f32> mSLDamageAngle;
	/* 0x198 */ TParamRT<f32> mSLTornadoProp;
	/* 0x1AC */ TParamRT<f32> mSLTornadoSpeed;
	/* 0x1C0 */ TParamRT<f32> mSLTornadoRollSpeed;
	/* 0x1D4 */ TParamRT<f32> mSLTornadoMoveInit;
	/* 0x1E8 */ TParamRT<f32> mSLTornadoMoveInc;
	/* 0x1FC */ TParamRT<f32> mSLTornadoMoveLimit;
	/* 0x210 */ TParamRT<s32> mSLWaterHitTimer;
	/* 0x224 */ TParamRT<s32> mSLHoverTimer;
	/* 0x238 */ TParamRT<f32> mSLPollBallRange;
	/* 0x24C */ TParamRT<f32> mSLPollBallSpeed;
	/* 0x260 */ TParamRT<f32> mSLPollBallFront;
};

// The pollution ball the boss lobs at Mario. While it is in the air it is the
// "pollut_ball" model; once it lands it stamps "pollut_ball_stamp" into the
// pollution map and plays that model's splat animation.
class TBPPolDrop : public THitActor {
public:
	TBPPolDrop(TBossPakkun*, const char*);

	// Declared in vtable order; TBPPolDrop adds no new slots.
	virtual ~TBPPolDrop() { }
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	void drop();
	void move();
	void launch(const JGeometry::TVec3<f32>&, const JGeometry::TVec3<f32>&);

public:
	/* 0x68 */ TBossPakkun* mOwner;
	/* 0x6C */ JGeometry::TVec3<f32> mVelocity;
	/* 0x78 */ MActor* mBallMActor;
	/* 0x7C */ MActor* mStampMActor;
	/* 0x80 */ int mState;
	/* 0x84 */ int mFlyTimer;
	/* 0x88 */ f32 mGroundY;
};

// The pollution puddle the boss throws up. It is not a hit actor -- the puddle
// only has to draw and stamp itself, the damage comes from the pollution map.
class TBPVomit : public JDrama::TViewObj {
public:
	TBPVomit(TBossPakkun*, const char*);

	// Declared in vtable order; TBPVomit adds no new slots.
	virtual ~TBPVomit() { }
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	void vomit();
	void vomitFinished();

public:
	/* 0x10 */ TBossPakkun* mOwner;
	/* 0x14 */ MActor* mMActor;
	/* 0x18 */ MActor* mStampMActor;
};

// The twister. It spirals outwards around a centre that walks towards
// mTarget, and knocks Mario over on contact.
class TBPTornado : public THitActor {
public:
	TBPTornado(TBossPakkun*, const char*);

	// Declared in vtable order; TBPTornado adds no new slots and does not
	// override receiveMessage.
	virtual ~TBPTornado() { }
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	void launch(const JGeometry::TVec3<f32>&);
	void vanish();

public:
	/* 0x68 */ TBossPakkun* mOwner;
	/* 0x6C */ MActor* mMActor;
	/* 0x70 */ JGeometry::TVec3<f32> mTarget;
	/* 0x7C */ JGeometry::TVec3<f32> mCenter;
	/* 0x88 */ JGeometry::TVec3<f32> mVelocity;
	/* 0x94 */ f32 mMove;
	/* 0x98 */ int mState;
};

// The head's attack volume. It follows joint 1 and throws Mario off when the
// boss swings its head (BCK slot 15, "bosspaku_head").
class TBPHeadHit : public THitActor {
public:
	TBPHeadHit(TBossPakkun*, const char*);

	// Declared in vtable order; TBPHeadHit adds no new slots.
	virtual ~TBPHeadHit() { }
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);

	void throwActor(THitActor*);

public:
	/* 0x68 */ TBossPakkun* mOwner;
};

// The belly's damage volume, pinned to joint 6. Hip-dropping it is what takes
// a hit point off the boss.
class TBPNavel : public THitActor {
public:
	TBPNavel(TBossPakkun*, const char*);

	// Declared in vtable order; TBPNavel adds no new slots.
	virtual ~TBPNavel() { }
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);

public:
	/* 0x68 */ TBossPakkun* mOwner;
};

// Adds the head's homing yaw and the belly's swelling scale on top of the
// Softimage joint animation.
class TBossPakkunMtxCalc : public M3UMtxCalcSIAnmBlendQuat {
public:
	TBossPakkunMtxCalc(TBossPakkun*);

	virtual ~TBossPakkunMtxCalc() { }
	virtual void calc(u16);

	void calcHeadDir(u16);
	void calcBellyScale(u16);
	void joinAnm(int);
	void setAnm(int);

public:
	/* 0x64 */ TBossPakkun* mOwner;
};

class TBossPakkun : public TSpineEnemy {
public:
	TBossPakkun(const char* name = "ボスパックン改");

	// Declared in vtable order.
	virtual ~TBossPakkun() { }
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void init(TLiveManager*);
	virtual void setGroundCollision();
	virtual void kill();
	virtual const char** getBasNameTable() const;

	BOOL checkMarioRiding();
	void startBGM();
	void rumblePad(int, const JGeometry::TVec3<f32>&);
	void showMessage(u32);
	bool is2ndFightNow() const;
	void ignoreWaterCheck();
	void startTornadoBlur();
	void resetWaterMark();
	BOOL inArea(const JGeometry::TVec3<f32>&);
	void gotFlyingDamage();
	void gotWaterDamage();
	void gotHipDropDamage();
	void gotTrampleDamage();
	void launchPolDrop();
	void launchTornado();
	void killSmallEnemies();
	void changeBck(int);
	void flyToCurPathNode(f32, f32);

	// fabricated
	TBossPakkunParams* getSaveParam2() const
	{
		return (TBossPakkunParams*)getSaveParam();
	}
	bool checkBalloonShown(u32 mask) const { return mBalloonsShown & mask; }

public:
	/* 0x150 */ TBossPakkunMtxCalc* mMtxCalc;
	/* 0x154 */ f32 mMotionBlendStep;
	/* 0x158 */ TBPPolDrop* mPolDrop;
	/* 0x15C */ TBPVomit* mVomit;
	/* 0x160 */ TBPTornado* mTornado;
	/* 0x164 */ TBPHeadHit* mHeadHit;
	/* 0x168 */ TBPNavel* mNavel;
	/* 0x16C */ s8 mState;
	/* 0x170 */ int unk170;
	/* 0x174 */ int unk174;
	// Water the boss has swallowed. It drains while mIsDrinking is set and
	// the belly scale is driven off it.
	/* 0x178 */ int mWaterMark;
	// Selects which counter drives the belly scale: while set,
	// TBossPakkunMtxCalc::calcBellyScale reads unk1B8 instead of mWaterMark.
	/* 0x17C */ s8 unk17C;
	// The "bosspaku_end.bmd" model, swapped in for the death animation. Only
	// the full version loads it.
	/* 0x180 */ MActor* mEndMActor;
	/* 0x184 */ f32 mHeadYaw;
	// The "ゲロエリアマネージャー" area the light variant checks Mario against
	// before spitting; looked up lazily.
	/* 0x188 */ TAreaCylinderManager* mVomitArea;
	/* 0x18C */ TWaterEmitInfo* mWaterEmitInfo;
	/* 0x190 */ s8 mIsMarioRiding;
	/* 0x194 */ JGeometry::TVec3<f32> unk194;
	/* 0x1A0 */ JGeometry::TVec3<f32> unk1A0;
	/* 0x1AC */ JGeometry::TVec3<f32> unk1AC;
	/* 0x1B8 */ int unk1B8;
	/* 0x1BC */ s8 unk1BC;
	/* 0x1C0 */ u32 mBalloonsShown;
	/* 0x1C4 */ s8 unk1C4;
	/* 0x1C5 */ u8 unk1C5[3];
	/* 0x1C8 */ f32 mRumblePower;
	/* 0x1CC */ s8 unk1CC;
};

class TBossPakkunManager : public TEnemyManager {
public:
	TBossPakkunManager(const char* name = "ボスパックンマネージャー",
	                   int is_light_version = 0);

	// Declared in vtable order; TBossPakkunManager adds no new slots.
	virtual ~TBossPakkunManager() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();

	void initJParticle();

public:
	// Set from the constructor argument: 0 for "BossPakkunManager", 1 for
	// "KBossPakkunManager". The light variant loads three models instead of
	// seven, no particles, and none of the flying behaviour.
	/* 0x54 */ int mIsLightVersion;
};

DECLARE_NERVE(TNerveBPWait, TLiveActor);
DECLARE_NERVE(TNerveBPCannon, TLiveActor);
DECLARE_NERVE(TNerveBPVomit, TLiveActor);
DECLARE_NERVE(TNerveBPTornado, TLiveActor);
DECLARE_NERVE(TNerveBPPivot, TLiveActor);
DECLARE_NERVE(TNerveBPSwallow, TLiveActor);
DECLARE_NERVE(TNerveBPTumbleIn, TLiveActor);
DECLARE_NERVE(TNerveBPTumble, TLiveActor);
DECLARE_NERVE(TNerveBPTumbleOut, TLiveActor);
DECLARE_NERVE(TNerveBPGetUp, TLiveActor);
DECLARE_NERVE(TNerveBPSwing, TLiveActor);
DECLARE_NERVE(TNerveBPStompReact, TLiveActor);
DECLARE_NERVE(TNerveBPJumpReact, TLiveActor);
DECLARE_NERVE(TNerveBPPreDie, TLiveActor);
DECLARE_NERVE(TNerveBPDie, TLiveActor);
DECLARE_NERVE(TNerveBPTakeOff, TLiveActor);
DECLARE_NERVE(TNerveBPFly, TLiveActor);
DECLARE_NERVE(TNerveBPTouchDown, TLiveActor);
DECLARE_NERVE(TNerveBPFlyCannon, TLiveActor);
DECLARE_NERVE(TNerveBPFlyPivot, TLiveActor);
DECLARE_NERVE(TNerveBPHover, TLiveActor);
DECLARE_NERVE(TNerveBPFall, TLiveActor);
DECLARE_NERVE(TNerveBPSleep, TLiveActor);
DECLARE_NERVE(TNerveBPBreakSleep, TLiveActor);
DECLARE_NERVE(TNerveBPWaitL, TLiveActor);
DECLARE_NERVE(TNerveBPCannonL, TLiveActor);
DECLARE_NERVE(TNervePakkunGenerate, TLiveActor);
DECLARE_NERVE(TNervePakkunStay, TLiveActor);
DECLARE_NERVE(TNervePakkunAppear, TLiveActor);
DECLARE_NERVE(TNervePakkunHide, TLiveActor);
DECLARE_NERVE(TNervePakkunShoot, TLiveActor);
DECLARE_NERVE(TNervePakkunFreeze, TLiveActor);
DECLARE_NERVE(TNerveStayPakkunHide, TLiveActor);
DECLARE_NERVE(TNerveStayPakkunAppear, TLiveActor);

#endif
