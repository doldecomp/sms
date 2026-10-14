#ifndef ENEMY_TIN_KOOPA_HPP
#define ENEMY_TIN_KOOPA_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <M3DUtil/M3UJoint.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/Nerve.hpp>

class TTinKoopa;
class TCoasterKiller;
class TCoasterKillerManager;
class TGraphWeb;
class TMapCollisionMove;

class TTinKoopaParams : public TSpineEnemyParams {
public:
	TTinKoopaParams(const char* path);

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
	virtual ~TTinKoopaManager() { }

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void createModelData();
	virtual BOOL hasMapCollision() const { return true; }
	virtual TSpineEnemy* createEnemyInstance();
};

class TTinKoopaMtxCalc : public M3UMtxCalcSIAnmBlendQuat {
public:
	TTinKoopaMtxCalc(TTinKoopa* owner);

	virtual void calc(u16);

	void joinAnm(int);

	/* 0x64 */ TTinKoopa* mOwner;
};

class TTinKoopaPartsBase : public TLiveActor {
public:
	TTinKoopaPartsBase(const char* name, int index, TTinKoopa* owner);

	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void reset();

	void emitPartsDisappearEffects(const char** joint_names, int num,
	                               f32 scale);
	void emitPartsDisappearEffects();
	void emitPartsTrackEffects(const char** joint_names, int num);
	void emitPartsTrackEffects();
	void startBreaking();
	void resetTinKoopaPartsBase();
	void initTinKoopaPartsBase();

	/* 0xF4 */ TMapCollisionMove* mCollision;
	/* 0xF8 */ bool mIsBreaking;
	/* 0xFC */ int mIndex;
	/* 0x100 */ TTinKoopa* mOwner;
	/* 0x104 */ MActor* unk104;
	/* 0x108 */ JGeometry::TVec3<f32> unk108[6];
};

class TTinKoopaFlame : public THitActor {
public:
	TTinKoopaFlame(const char* name, TTinKoopa* owner);

	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);

	void emitFlameEffects();
	bool isHighPosition();
	void checkMario();
	void hitWater();
	void resetTinKoopaFlame();
	void makeHitCollision();

	/* 0x68 */ TTinKoopa* mOwner;
	/* 0x6C */ f32 unk6C;
	/* 0x70 */ s16 unk70;
	/* 0x72 */ bool unk72;
};

class TTinKoopaLaunchOrder {
public:
	TTinKoopaLaunchOrder(TTinKoopa* owner);

	void checkOrder();
	void makeOrder(s8, s32, s8, s8);

	/* 0x0 */ TTinKoopa* mOwner;
	/* 0x4 */ s8 mLap;
	/* 0x8 */ s32 mFrame;
	/* 0xC */ s8 mKillerNum;
	/* 0xD */ s8 mKillerDir;
};

class TTinKoopaLaunchSchedule {
public:
	TTinKoopaLaunchSchedule(u8 num, TTinKoopa* owner);

	void checkOrder();

	/* 0x0 */ u8 mOrderNum;
	/* 0x4 */ TTinKoopa* mOwner;
	/* 0x8 */ TTinKoopaLaunchOrder** mOrders;
};

class TTinKoopa : public TSpineEnemy {
public:
	TTinKoopa(const char* name);
	virtual ~TTinKoopa() { }

	virtual void init(TLiveManager* manager);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual BOOL hasMapCollision() const { return true; }
	virtual const char** getBasNameTable() const;
	virtual void reset();

	void emitTinKoopaEffects();
	void startTinKoopaMessage(u32);
	void checkTinKoopaFirstFlameMessage();
	void checkTinKoopaFirstRocketMessage();
	void checkTinKoopaKillerApproachingMessage();
	void checkTinKoopaMessage();
	void checkKillerLaunch();
	void launchKiller(int);
	void startBreakingParts();
	void hitParts();
	void changeBck(int);
	void updateTimers();
	void makeEyeBeamEffect();
	bool checkTruckAnimationPass(int);
	void checkLap();
	void makeKillerQueue(int, s8);
	void makeHitCollision();
	void resetTinKoopa();
	bool checkKillerApproachingFromBack(TCoasterKiller*, JGeometry::TVec3<f32>,
	                                    f32);
	f32 calcCoasterDistanceInOrder(int, int);
	f32 calcCoasterDistance(int, int);
	void makeCoasterDistanceTable();
	void makeLaunchSchedule();

	enum {
		TIMER_KILLER_INTERVAL,
		TIMER_FLAME_REVIVAL,
		TIMER_DEFEAT_WAIT,
	};

	TTinKoopaParams* getSaveParam2() const
	{
		return (TTinKoopaParams*)getSaveParam();
	}

	/* 0x150 */ int mBreakCount;
	/* 0x154 */ int unk154;
	/* 0x158 */ int unk158;
	/* 0x15C */ int mLap;
	/* 0x160 */ TTinKoopaFlame* mFlame;
	/* 0x164 */ MActor* mTruckActor; // the rollercoaster cart
	/* 0x168 */ bool mFirstFlameMessageShown;
	/* 0x169 */ s8 mQueuedKillerDirs[4];
	/* 0x170 */ int mQueuedKillerNum;
	/* 0x174 */ int mQueuedKillersLaunchedNum;
	/* 0x178 */ int mTimers[3];
	/* 0x184 */ JGeometry::TVec3<f32> unk184;
	/* 0x190 */ JGeometry::TVec3<f32> unk190;
	/* 0x19C */ JGeometry::TVec3<f32> unk19C;
	/* 0x1A8 */ JGeometry::TVec3<f32> unk1A8;
	/* 0x1B4 */ f32 unk1B4;
	/* 0x1B8 */ f32 unk1B8;
	/* 0x1BC */ f32 unk1BC;
	/* 0x1C0 */ f32 unk1C0;
	/* 0x1C4 */ char unk1C4[4];
	/* 0x1C8 */ int mPartsHp;
	/* 0x1CC */ TTinKoopaPartsBase* mParts[6];
	/* 0x1E4 */ TTinKoopaPartsBase* unk1E4;
	/* 0x1E8 */ f32* unk1E8;
	/* 0x1EC */ TGraphWeb* unk1EC;
	/* 0x1F0 */ TCoasterKillerManager* unk1F0;
	/* 0x1F4 */ TTinKoopaLaunchSchedule* mLaunchSchedule;
	/* 0x1F8 */ int unk1F8;
};

DECLARE_NERVE(TNerveTinKoopaWait, TLiveActor);
DECLARE_NERVE(TNerveTinKoopaDamage, TLiveActor);
DECLARE_NERVE(TNerveTinKoopaBreak, TLiveActor);

#endif
