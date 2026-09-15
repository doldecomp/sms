#ifndef ENEMY_BOSS_HANACHAN_HPP
#define ENEMY_BOSS_HANACHAN_HPP

#include <Strategic/Nerve.hpp>
#include <Strategic/LiveActor.hpp>
#include <Enemy/Enemy.hpp>

class TBossHanachan;
class TIdxGroupObj;
class TWaterHitActor;
class TMapCollisionMove;
class TNpcInbetween;
class TFootHitActor;
class JUTNameTab;
class TBossHanachanPartsBody;
class TBossHanachanPartsHead;
class TBossHanachanCommonSaveParams;
class TBossHanachanChangeSaveParams;
class TSphereLink;

extern const char* cMapCollisionJointName;
extern const char* cBodyMapCollisionFileName;
extern const char* cHeadMapCollisionFileName;
extern const char* cLegJointName_L3;
extern const char* cLegJointName_R3;
extern const char* cNoseHallJointName_L;
extern const char* cNoseHallJointName_R;

void CalcMtxPtrFromJointName(JUTNameTab*, const char*, J3DModel*, MtxPtr*);

// TODO: recover descriptive names for the animation states.
enum EnumBossHanachanNerveAnm {
	BOSS_HANACHAN_NERVE_ANM_UNK0 = 0,
	BOSS_HANACHAN_NERVE_ANM_UNK1 = 1,
	BOSS_HANACHAN_NERVE_ANM_UNK2 = 2,
	BOSS_HANACHAN_NERVE_ANM_UNK3 = 3,
	BOSS_HANACHAN_NERVE_ANM_UNK4 = 4,
	BOSS_HANACHAN_NERVE_ANM_UNK5 = 5,
};

enum EnumBossHanachanAnmKind {
	BOSS_HANACHAN_ANM_UNK0 = 0,
	BOSS_HANACHAN_ANM_UNK1 = 1,
	BOSS_HANACHAN_ANM_UNK2 = 2,
	BOSS_HANACHAN_ANM_UNK3 = 3,
	BOSS_HANACHAN_ANM_UNK4 = 4,
	BOSS_HANACHAN_ANM_UNK5 = 5,
	BOSS_HANACHAN_ANM_UNK6 = 6,
	BOSS_HANACHAN_ANM_UNK7 = 7,
	BOSS_HANACHAN_ANM_UNK8 = 8,
	BOSS_HANACHAN_ANM_UNK9 = 9,
	BOSS_HANACHAN_ANM_UNK10 = 10,
	BOSS_HANACHAN_ANM_UNK11 = 11,
	BOSS_HANACHAN_ANM_UNK12 = 12,
	BOSS_HANACHAN_ANM_UNK13 = 13,
	BOSS_HANACHAN_ANM_UNK14 = 14,
	BOSS_HANACHAN_ANM_UNK15 = 15,
	BOSS_HANACHAN_ANM_UNK16 = 16,
	BOSS_HANACHAN_ANM_UNK17 = 17,
	BOSS_HANACHAN_ANM_UNK18 = 18,
};

enum EnumBossHanachanStopMotionBlendOnOff {
	BOSS_HANACHAN_STOP_MOTION_BLEND_OFF = 0,
	BOSS_HANACHAN_STOP_MOTION_BLEND_ON = 1,
};

class TBossHanachanPartsBase : public TLiveActor {
public:
	TBossHanachanPartsBase(TBossHanachan*, u32, int, const char*);
	virtual ~TBossHanachanPartsBase() { }
	virtual const char** getBasNameTable() const;
	virtual bool setAnm_(EnumBossHanachanAnmKind,
	                     EnumBossHanachanStopMotionBlendOnOff) = 0;

	void considerSetAnm_(EnumBossHanachanNerveAnm);
	bool isReactToTrampleOrHipDrop_() const;
	void calcRotateZWhenGetUp_();
	bool isMarioOn_() const;
	const TLiveActor* getSandActor_() const;
	void copyFrameFromOldAnmToNewAnm_();
	bool isCurBckAlreadyEnd_() const;
	void setDamageFog_(JDrama::TGraphics*);
	void entryCircleShadow_();
	void moveMapCollision_();
	void changeTumbleAnmRate_();
	void restartBck_();
	void setNonstopMotionBlendRatio_(f32);
	void offNonstopMotionBlend_();
	void initMapCollisionAndHitActor_(TIdxGroupObj*);

public:
	/* 0xF4 */ EnumBossHanachanAnmKind mCurrentAnm;
	/* 0xF8 */ EnumBossHanachanAnmKind mPreviousAnm;
	/* 0xFC */ TBossHanachan* unkFC;
	/* 0x100 */ TWaterHitActor* unk100;
	/* 0x104 */ TMapCollisionMove* unk104;
	/* 0x108 */ MtxPtr unk108;
	/* 0x10C */ s32 unk10C;
	/* 0x110 */ TNpcInbetween* mInbetween;
};

class TBossHanachanPartsBody : public TBossHanachanPartsBase {
public:
	TBossHanachanPartsBody(TBossHanachan*, const char*);
	virtual ~TBossHanachanPartsBody() { }
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual bool setAnm_(EnumBossHanachanAnmKind,
	                     EnumBossHanachanStopMotionBlendOnOff);
	void initFootHitActor_(TIdxGroupObj*);

public:
	/* 0x114 */ s32 unk114;
	/* 0x118 */ TFootHitActor* mFeet[2];
	/* 0x120 */ JGeometry::TVec3<f32> unk120;
	/* 0x12C */ JGeometry::TVec3<f32> unk12C;
	/* 0x138 */ JGeometry::TVec3<f32> unk138;
	/* 0x144 */ f32 unk144;
	/* 0x148 */ f32 unk148;
	/* 0x14C */ MtxPtr mLeftLegMtx;
	/* 0x150 */ MtxPtr mRightLegMtx;
	/* 0x154 */ JGeometry::TVec3<f32> unk154;
};

class TBossHanachanPartsHead : public TBossHanachanPartsBase {
public:
	TBossHanachanPartsHead(TBossHanachan*, const char*);
	virtual ~TBossHanachanPartsHead() { }
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual bool setAnm_(EnumBossHanachanAnmKind,
	                     EnumBossHanachanStopMotionBlendOnOff);

public:
	/* 0x114 */ MtxPtr mLeftNoseMtx;
	/* 0x118 */ MtxPtr mRightNoseMtx;
};

class TBossHanachanCommonSaveParams : public TParams {
public:
	TBossHanachanCommonSaveParams(const char*);

	/* 0x8 */ TParamRT<f32> mSLViewClipFar;
	/* 0x1C */ TParamRT<f32> mSLViewClipRadius;
	/* 0x30 */ TParamRT<f32> mSLHeadHitOffsetY;
	/* 0x44 */ TParamRT<f32> mSLHeadAttackRadius;
	/* 0x58 */ TParamRT<f32> mSLHeadAttackHeight;
	/* 0x6C */ TParamRT<f32> mSLHeadDamageRadius;
	/* 0x80 */ TParamRT<f32> mSLHeadDamageHeight;
	/* 0x94 */ TParamRT<f32> mSLBodyHitOffsetY;
	/* 0xA8 */ TParamRT<f32> mSLBodyAttackRadius;
	/* 0xBC */ TParamRT<f32> mSLBodyAttackHeight;
	/* 0xD0 */ TParamRT<f32> mSLBodyDamageRadius;
	/* 0xE4 */ TParamRT<f32> mSLBodyDamageHeight;
	/* 0xF8 */ TParamRT<f32> mSLFootHitOffsetY;
	/* 0x10C */ TParamRT<f32> mSLFootAttackRadius;
	/* 0x120 */ TParamRT<f32> mSLFootAttackHeight;
	/* 0x134 */ TParamRT<f32> mSLFootDamageRadius;
	/* 0x148 */ TParamRT<f32> mSLFootDamageHeight;
	/* 0x15C */ TParamRT<f32> mSLHeadLength;
	/* 0x170 */ TParamRT<f32> mSLBodyLength;
	/* 0x184 */ TParamRT<f32> mSLHeadShadowSize;
	/* 0x198 */ TParamRT<f32> mSLBodyShadowSize;
	/* 0x1AC */ TParamRT<f32> mSLHeadPlusYByRotateZ;
	/* 0x1C0 */ TParamRT<f32> mSLBodyPlusYByRotateZ;
	/* 0x1D4 */ TParamRT<f32> mSLRecoverSearchDist;
	/* 0x1E8 */ TParamRT<f32> mSLRecoverSearchDegree;
	/* 0x1FC */ TParamRT<f32> mSLShineAppearOffsetY;
	/* 0x210 */ TParamRT<f32> mSLCamShakeZeroDist;
	/* 0x224 */ TParamRT<f32> mSLCamShakeMaxDist;
	/* 0x238 */ TParamRT<s16> mSLMotionBlendFrames;
};

class TBossHanachanChangeSaveParams : public TParams {
public:
	TBossHanachanChangeSaveParams(const char*);

	/* 0x8 */ TParamRT<f32> mSLWalkBckRateMagnif;
	/* 0x1C */ TParamRT<f32> mSLWalkBckRateMin;
	/* 0x30 */ TParamRT<f32> mSLWalkAnmMarchSpeed;
	/* 0x44 */ TParamRT<f32> mSLRunAnmMarchSpeed;
	/* 0x58 */ TParamRT<f32> mSLMaxMarchSpeed;
	/* 0x6C */ TParamRT<f32> mSLMarchAccel;
	/* 0x80 */ TParamRT<f32> mSLMarchDecrease;
	/* 0x94 */ TParamRT<f32> mSLWalkTurnSpeed;
	/* 0xA8 */ TParamRT<f32> mSLFallDecideRotateZ;
	/* 0xBC */ TParamRT<f32> mSLWaveFallDownSpeed;
	/* 0xD0 */ TParamRT<f32> mSLFallDecideMinSpeed;
	/* 0xE4 */ TParamRT<f32> mSLSandSlopeForce;
	/* 0xF8 */ TParamRT<f32> mSLMaxRotateZNotSand;
	/* 0x10C */ TParamRT<f32> mSLRotateZLeanSpeed;
	/* 0x120 */ TParamRT<f32> mSLRotateZRestorationSpeed;
	/* 0x134 */ TParamRT<f32> mSLDiffMaxRotateZ;
	/* 0x148 */ TParamRT<f32> mSLCentrifugalForce;
	/* 0x15C */ TParamRT<f32> mSLCentrifugalSpeed;
	/* 0x170 */ TParamRT<f32> mSLWaveVelocity;
	/* 0x184 */ TParamRT<f32> mSLWaveDecrease;
	/* 0x198 */ TParamRT<s16> mSLDownFrames;
	/* 0x1AC */ TParamRT<s16> mSLDamageFrames;
	/* 0x1C0 */ TParamRT<s16> mSLNotFallDownFrames;
	/* 0x1D4 */ TParamRT<u8> mSLNormalBckFrameDiff;
	/* 0x1E8 */ TParamRT<u8> mSLGetUpFrameDiff;
	/* 0x1FC */ TParamRT<u8> mSLSnortFrameDiff;
	/* 0x210 */ TParamRT<u8> mSLDamageFrameDiff;
	/* 0x224 */ TParamRT<u8> mSLDeadFrameDiff;
	/* 0x238 */ TParamRT<f32> mSLParticleProbability;
	/* 0x24C */ TParamRT<f32> mSLThrowTotalPower;
	/* 0x260 */ TParamRT<f32> mSLThrowSpeedMin;
	/* 0x274 */ TParamRT<f32> mSLThrowSpeedMax;
	/* 0x288 */ TParamRT<f32> mSLThrowMoveDirPower;
	/* 0x29C */ TParamRT<f32> mSLThrowVecY;
};

class TBossHanachan : public TSpineEnemy {
public:
	TBossHanachan(const char*);
	virtual ~TBossHanachan() { }
	virtual void perform(u32, JDrama::TGraphics*);
	virtual void init(TLiveManager*);
	virtual void bind();
	virtual void moveObject();
	virtual void kill();
	virtual BOOL hasMapCollision() const;

	void removeAllMapCollision();
	void execDamage();
	void goToInitialRecoverGraphNode();
	void execSlip();
	void execWalk(bool);
	bool isCanWalk() const;
	f32 getBodyMaxRotateZ() const;
	bool checkFallDecideAndSetup();
	bool isTumbleCompletelyAllBody() const;
	void execBodyCalcAnim_();
	void execHeadCalcAnim_();
	void throwMario_(THitActor*);
	void setRandomWeakBodyIndex();
	void changeAnmRateAndFrameUpdate_();
	void copyFrameFromOldAnmToNewAnm_();
	void setHeadAndBodyNonstopMotionBlendRatio_(f32);
	void offHeadAndBodyNonstopMotionBlend_();
	bool isAllBckAlreadyEnd(EnumBossHanachanAnmKind) const;
	bool isFinishedGetUp() const;
	void considerSetAnm(EnumBossHanachanNerveAnm);
	void setAnmTimerWhenDead();
	void setAnmTimerWhenDamage();
	void setAnmTimerWhenSnort();
	void setAnmTimerWhenGetUp();
	void setTumbleAnm(EnumBossHanachanStopMotionBlendOnOff);
	void setTumbleBckRate_(TBossHanachanPartsBase*);
	void setHeadAndBodyAnm(EnumBossHanachanAnmKind,
	                       EnumBossHanachanStopMotionBlendOnOff);
	void emitCamShake_();
	void emitOneTimeSandPillar_(TBossHanachanPartsBody*);
	void emitParticle_();
	static void staticLoadParticle();

public:
	/* 0x150 */ TBossHanachanPartsBody* mBodies[8];
	/* 0x170 */ TBossHanachanPartsHead* mHead;
	/* 0x174 */ s32 mWeakBodyIndex;
	// TODO: recover the remaining field meanings from their consumers.
	/* 0x178 */ TSphereLink* unk178;
	/* 0x17C */ f32 unk17C;
	/* 0x180 */ f32 unk180;
	/* 0x184 */ f32 unk184;
	/* 0x188 */ f32 unk188;
	/* 0x18C */ f32 unk18C;
	/* 0x190 */ f32 unk190;
	/* 0x194 */ f32 unk194;
	/* 0x198 */ f32 unk198;
	/* 0x19C */ s32 unk19C;
	/* 0x1A0 */ f32 unk1A0;
	/* 0x1A4 */ f32 unk1A4;
	/* 0x1A8 */ f32 unk1A8;
	/* 0x1AC */ f32 unk1AC;
	/* 0x1B0 */ f32 unk1B0;
	/* 0x1B4 */ f32 unk1B4;
	/* 0x1B8 */ s32 unk1B8;
	/* 0x1BC */ TBossHanachanCommonSaveParams* mCommonParams;
	/* 0x1C0 */ TBossHanachanChangeSaveParams* mChangeParams;
};

DECLARE_NERVE(TNerveSBH_Fall, TLiveActor);
DECLARE_NERVE(TNerveSBH_SleepContinue, TLiveActor);
DECLARE_NERVE(TNerveBossHanachanDead, TLiveActor);
DECLARE_NERVE(TNerveBossHanachanSnort, TLiveActor);
DECLARE_NERVE(TNerveBossHanachanDamage, TLiveActor);
DECLARE_NERVE(TNerveBossHanachanGetUp, TLiveActor);
DECLARE_NERVE(TNerveBossHanachanDown, TLiveActor);
DECLARE_NERVE(TNerveBossHanachanTumble, TLiveActor);
DECLARE_NERVE(TNerveBossHanachanGraphWander, TLiveActor);

#endif
