#ifndef ENEMY_BOSS_HANACHAN_HPP
#define ENEMY_BOSS_HANACHAN_HPP

#include <Strategic/LiveActor.hpp>
#include <Strategic/Nerve.hpp>

class TBossHanachan;
class TIdxGroupObj;

// TODO: enumerands unknown; indices parallel bosshanachan_bastable
enum EnumBossHanachanNerveAnm { };
// TODO: enumerands unknown
enum EnumBossHanachanAnmKind { };
// TODO: enumerands unknown
enum EnumBossHanachanStopMotionBlendOnOff { };

class TBossHanachanPartsBase : public TLiveActor {
public:
	TBossHanachanPartsBase(TBossHanachan*, u32, int, const char*);

	virtual const char** getBasNameTable() const;
	virtual void setAnm_(EnumBossHanachanAnmKind,
	                     EnumBossHanachanStopMotionBlendOnOff)
	    = 0;

	void calcRotateZWhenGetUp_();
	void changeTumbleAnmRate_();
	void considerSetAnm_(EnumBossHanachanNerveAnm);
	void copyFrameFromOldAnmToNewAnm_();
	void entryCircleShadow_();
	void getSandActor_() const; // TODO: unknown return type
	void initMapCollisionAndHitActor_(TIdxGroupObj*);
	bool isCurBckAlreadyEnd_() const;
	bool isMarioOn_() const;
	bool isReactToTrampleOrHipDrop_() const;
	void moveMapCollision_();
	void offNonstopMotionBlend_();
	void restartBck_();
	void setDamageFog_(JDrama::TGraphics*);
	void setNonstopMotionBlendRatio_(f32);

	// TODO: fields unknown until BossHanachanParts.cpp is decompiled
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
