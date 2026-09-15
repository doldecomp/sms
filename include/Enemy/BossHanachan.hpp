#ifndef ENEMY_BOSS_HANACHAN_HPP
#define ENEMY_BOSS_HANACHAN_HPP

#include <Strategic/Nerve.hpp>
#include <Strategic/LiveActor.hpp>

class TBossHanachan;
class TIdxGroupObj;
class TWaterHitActor;
class TMapCollisionMove;

// TODO: recover the remaining animation-state values and their names.
enum EnumBossHanachanNerveAnm {
	BOSS_HANACHAN_NERVE_ANM_UNK0 = 0,
};

enum EnumBossHanachanAnmKind {
	BOSS_HANACHAN_ANM_UNK18 = 18,
};

class TBossHanachanPartsBase : public TLiveActor {
public:
	TBossHanachanPartsBase(TBossHanachan*, u32, int, const char*);
	virtual ~TBossHanachanPartsBase() { }
	virtual const char** getBasNameTable() const;

	void considerSetAnm_(EnumBossHanachanNerveAnm);
	BOOL isReactToTrampleOrHipDrop_() const;
	void calcRotateZWhenGetUp_();
	BOOL isMarioOn_() const;
	const TLiveActor* getSandActor_() const;
	void copyFrameFromOldAnmToNewAnm_();
	BOOL isCurBckAlreadyEnd_() const;
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
	// TODO: recover the motion-controller type.
	/* 0x110 */ void* unk110;
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
