#ifndef ENEMY_KILLER_HPP
#define ENEMY_KILLER_HPP

#include <Enemy/WalkerEnemy.hpp>

class TFlyEnemyParams;

class TFlyEnemy : public TWalkerEnemy {
public:
	TFlyEnemy(const char*);

	virtual void init(TLiveManager*);
	virtual void bind();
	virtual f32 getGravityY() const;
	virtual void reset();
	virtual void setAfterDeadEffect();
	virtual void flyBehavior();
	virtual void setChaseFlyAnm();
	virtual void setNormalFlyAnm();

	void flyMove();
	void calcChaseParam();
	void fly();

public:
	/* 0x194 */ f32 unk194;
	/* 0x198 */ s32 unk198;
	/* 0x19C */ TFlyEnemyParams* unk19C;
	/* 0x1A0 */ s32 unk1A0;
	/* 0x1A4 */ u8 unk1A4;
	/* 0x1A5 */ u8 unk1A5;
	/* 0x1A6 */ u8 unk1A6;
	/* 0x1A7 */ u8 unk1A7;
	/* 0x1A8 */ JGeometry::TVec3<f32> unk1A8;
};

class TKiller : public TFlyEnemy {
public:
	TKiller(const char* name = "キラー");

	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void bind();
	virtual const char** getBasNameTable() const;
	virtual void reset();
	virtual void genEventCoin();
	virtual void behaveToWater(THitActor*);
	virtual void changeOut();
	virtual void setDeadAnm();
	virtual void attackToMario();
	virtual void forceKill();
	virtual void setMActorAndKeeper();
	virtual bool isHitValid(u32);
	virtual bool isCollidMove(THitActor*);
	virtual bool isFindMario(f32);
	virtual void flyBehavior();
	virtual void setChaseFlyAnm();
	virtual void setNormalFlyAnm();

	void setColorType();
	bool isRollFly();
};

#endif
