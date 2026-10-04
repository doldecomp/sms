#ifndef ENEMY_BOSS_HANACHAN_PARTS_HPP
#define ENEMY_BOSS_HANACHAN_PARTS_HPP

#include <Enemy/BossHanachanAnm.hpp>
#include <NPC/NpcInbetween.hpp>
#include <Strategic/LiveActor.hpp>

class TBossHanachan;
class TWaterHitActor;
class TMapCollisionBase;
class TIdxGroupObj;

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
	void setNonstopMotionBlendRatio_(f32 ratio)
	{
		unk110->mForcedBlendRatio = ratio;
	}
	void offNonstopMotionBlend_() { unk110->mForcedBlendRatio = 0.0f; }
	void initMapCollisionAndHitActor_(TIdxGroupObj*);

public:
	/* 0xF4 */ s32 unkF4;
	/* 0xF8 */ s32 unkF8;
	/* 0xFC */ TBossHanachan* unkFC;
	/* 0x100 */ TWaterHitActor* unk100;
	/* 0x104 */ TMapCollisionBase* unk104;
	/* 0x108 */ MtxPtr unk108;
	/* 0x10C */ s32 unk10C;
	/* 0x110 */ TNpcInbetween* unk110;
};

#endif
