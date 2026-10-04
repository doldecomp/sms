#ifndef ENEMY_BOSS_HANACHAN_HPP
#define ENEMY_BOSS_HANACHAN_HPP

#include <Strategic/Nerve.hpp>
#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Enemy/BossHanachanAnm.hpp>

class TLiveActor;
class TBossHanachanPartsBase;
class TBossHanachanPartsBody;
class TBossHanachanCommonSaveParams;
class TBossHanachanChangeSaveParams;
class TSphereLink;

class TBossHanachan : public TSpineEnemy {
public:
	TBossHanachan(const char*);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual void init(TLiveManager*);
	virtual void bind();
	virtual void moveObject();
	virtual void kill();
	virtual BOOL hasMapCollision() const;

	static void staticLoadParticle();
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

class TBossHanachanManager : public TEnemyManager {
public:
	TBossHanachanManager(const char* name = "?");

	virtual void loadAfter();
	virtual void createModelData();
	virtual BOOL hasMapCollision() const;
	virtual void clipEnemies(JDrama::TGraphics*);

public:
	/* 0x54 */ TBossHanachanCommonSaveParams* unk54;
	/* 0x58 */ TBossHanachanChangeSaveParams* unk58[3];
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
