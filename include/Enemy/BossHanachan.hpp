#ifndef ENEMY_BOSS_HANACHAN_HPP
#define ENEMY_BOSS_HANACHAN_HPP

#include <Strategic/Nerve.hpp>
#include <Strategic/LiveActor.hpp>

class TBossHanachan;
class TIdxGroupObj;
class TWaterHitActor;
class TMapCollisionMove;
class TNpcInbetween;
class TFootHitActor;
class JUTNameTab;

extern const char* cMapCollisionJointName;
extern const char* cBodyMapCollisionFileName;
extern const char* cHeadMapCollisionFileName;
extern const char* cLegJointName_L3;
extern const char* cLegJointName_R3;
extern const char* cNoseHallJointName_L;
extern const char* cNoseHallJointName_R;

void CalcMtxPtrFromJointName(JUTNameTab*, const char*, J3DModel*, MtxPtr*);

// TODO: recover the remaining animation-state values and their names.
enum EnumBossHanachanNerveAnm {
	BOSS_HANACHAN_NERVE_ANM_UNK0 = 0,
};

enum EnumBossHanachanAnmKind {
	BOSS_HANACHAN_ANM_UNK8 = 8,
	BOSS_HANACHAN_ANM_UNK11 = 11,
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
	virtual BOOL setAnm_(EnumBossHanachanAnmKind,
	                     EnumBossHanachanStopMotionBlendOnOff) = 0;

	void considerSetAnm_(EnumBossHanachanNerveAnm);
	BOOL isReactToTrampleOrHipDrop_() const;
	void calcRotateZWhenGetUp_();
	BOOL isMarioOn_() const;
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
	virtual BOOL setAnm_(EnumBossHanachanAnmKind,
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
	virtual BOOL setAnm_(EnumBossHanachanAnmKind,
	                     EnumBossHanachanStopMotionBlendOnOff);

public:
	/* 0x114 */ MtxPtr mLeftNoseMtx;
	/* 0x118 */ MtxPtr mRightNoseMtx;
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
