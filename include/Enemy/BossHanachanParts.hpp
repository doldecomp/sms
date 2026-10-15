#ifndef ENEMY_BOSS_HANACHAN_PARTS_HPP
#define ENEMY_BOSS_HANACHAN_PARTS_HPP

#include <Enemy/BossHanachanAnm.hpp>
#include <NPC/NpcInbetween.hpp>
#include <Strategic/LiveActor.hpp>
#include <Player/ModelWaterManager.hpp>

class TBossHanachan;
class TWaterHitActor;
class TMapCollisionBase;
class TIdxGroupObj;

class TFootHitActor : public TWaterHitActor {
public:
	TFootHitActor(const char* name)
	    : TWaterHitActor(name)
	{
	}
	/* 0x6C */ MtxPtr unk6C;
};

class TBossHanachanPartsBase : public TLiveActor {
public:
	TBossHanachanPartsBase(TBossHanachan*, u32, int, const char*);
	virtual const char** getBasNameTable() const;
	virtual bool setAnm_(EnumBossHanachanAnmKind,
	                     EnumBossHanachanStopMotionBlendOnOff)
	    = 0;

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
	void setNonstopMotionBlendRatio_(f32 ratio);
	void offNonstopMotionBlend_();
	void initMapCollisionAndHitActor_(TIdxGroupObj*);

public:
	/* 0xF4 */ s32 mCurAnm;
	/* 0xF8 */ s32 mPrevAnm;
	/* 0xFC */ TBossHanachan* mOwner;
	/* 0x100 */ TWaterHitActor* unk100;
	/* 0x104 */ TMapCollisionBase* unk104;
	/* 0x108 */ MtxPtr unk108;
	/* 0x10C */ s32 unk10C;
	/* 0x110 */ TNpcInbetween* unk110;
};

class TBossHanachanPartsHead : public TBossHanachanPartsBase {
public:
	TBossHanachanPartsHead(TBossHanachan* owner,
	                       const char* name = "ボスハナチャンの頭");
	virtual ~TBossHanachanPartsHead() { }
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual bool setAnm_(EnumBossHanachanAnmKind,
	                     EnumBossHanachanStopMotionBlendOnOff);

	/* 0x114 */ MtxPtr unk114[2];
};

class TBossHanachanPartsBody : public TBossHanachanPartsBase {
public:
	TBossHanachanPartsBody(TBossHanachan* owner,
	                       const char* name = "ボスハナチャンの体");
	virtual ~TBossHanachanPartsBody() { }
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual bool setAnm_(EnumBossHanachanAnmKind,
	                     EnumBossHanachanStopMotionBlendOnOff);
	void initFootHitActor_(TIdxGroupObj*);

	/* 0x114 */ int unk114;
	/* 0x118 */ TFootHitActor* unk118[2];
	/* 0x120 */ f32 unk120;
	/* 0x124 */ JGeometry::TVec3<f32> unk124;
	/* 0x130 */ JGeometry::TVec3<f32> unk130;
	/* 0x13C */ f32 unk13C;
	/* 0x140 */ f32 unk140;
	/* 0x144 */ f32 unk144;
	/* 0x148 */ f32 unk148;
	/* 0x14C */ MtxPtr unk14C[2];
	/* 0x154 */ JGeometry::TVec3<f32> unk154;
};

#endif
