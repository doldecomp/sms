#ifndef ENEMY_BOSS_HANACHAN_HPP
#define ENEMY_BOSS_HANACHAN_HPP

#include <Strategic/Nerve.hpp>
#include <Enemy/Enemy.hpp>
#include <Enemy/BossHanachanAnm.hpp>

class TLiveActor;
class TBossHanachanPartsBase;
class TBossHanachanCommonSaveParams;
class TBossHanachanChangeSaveParams;
class TSphereLink;

class TBossHanachan : public TSpineEnemy {
public:
	TBossHanachan(const char*);
	static void staticLoadParticle();
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

public:
	/* 0x150 */ TBossHanachanPartsBase* unk150[8];
	/* 0x170 */ TBossHanachanPartsBase* unk170;
	/* 0x174 */ s32 unk174;
	/* 0x178 */ TSphereLink* unk178;
	/* 0x17C */ JGeometry::TVec3<f32> unk17C;
	/* 0x188 */ JGeometry::TVec3<f32> unk188;
	/* 0x194 */ f32 unk194;
	/* 0x198 */ f32 unk198;
	/* 0x19C */ MActor* unk19C;
	/* 0x1A0 */ JGeometry::TVec3<f32> unk1A0;
	/* 0x1AC */ JGeometry::TVec3<f32> unk1AC;
	/* 0x1B8 */ s32 unk1B8;
	/* 0x1BC */ TBossHanachanCommonSaveParams* unk1BC;
	/* 0x1C0 */ TBossHanachanChangeSaveParams* unk1C0;
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
