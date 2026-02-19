#ifndef ENEMY_SMALL_ENEMY_HPP
#define ENEMY_SMALL_ENEMY_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Strategic/Nerve.hpp>

class TCoin;
class TSmallEnemy;
class TJuiceBlock;

class TSmallEnemyParams : public TSpineEnemyParams {
public:
	TSmallEnemyParams(const char*);

	f32 getSLJumpForce() const { return mSLJumpForce.get(); }
	f32 getSLSearchLength() const { return mSLSearchLength.get(); }
	f32 getSLSearchHeight() const { return mSLSearchHeight.get(); }
	f32 getSLSearchAware() const { return mSLSearchAware.get(); }
	f32 getSLSearchAngle() const { return mSLSearchAngle.get(); }
	f32 getSLGiveUpLength() const { return mSLGiveUpLength.get(); }
	f32 getSLGiveUpHeight() const { return mSLGiveUpHeight.get(); }
	s32 getSLAttackWait() const { return mSLAttackWait.get(); }
	s32 getSLFreezeWait() const { return mSLFreezeWait.get(); }
	s32 getSLDamageRadius() const { return mSLDamageRadius.get(); }
	s32 getSLDamageHeight() const { return mSLDamageHeight.get(); }
	s32 getSLAttackRadius() const { return mSLAttackRadius.get(); }
	s32 getSLAttackHeight() const { return mSLAttackHeight.get(); }
	f32 getSLTurnSpeedLow() const { return mSLTurnSpeedLow.get(); }
	f32 getSLTurnSpeedHigh() const { return mSLTurnSpeedHigh.get(); }
	f32 getSLBodyScaleLow() const { return mSLBodyScaleLow.get(); }
	f32 getSLBodyScaleHigh() const { return mSLBodyScaleHigh.get(); }
	f32 getSLGenItemRate() const { return mSLGenItemRate.get(); }
	f32 getSLGenEggRate() const { return mSLGenEggRate.get(); }
	u8 getSLPolluteRange() const { return mSLPolluteRange.get(); }
	s32 getSLWaitTime() const { return mSLWaitTime.get(); }
	s32 getSLPolluteRMin() const { return mSLPolluteRMin.get(); }
	s32 getSLPolluteRMax() const { return mSLPolluteRMax.get(); }
	s32 getSLPolluteCycle() const { return mSLPolluteCycle.get(); }
	u8 getSLStampRange() const { return mSLStampRange.get(); }
	s32 getSLPolluteInterval() const { return mSLPolluteInterval.get(); }
	u8 getSLGenerateOnlyDead() const { return mSLGenerateOnlyDead.get(); }

	/* 0xA8 */ TParamRT<f32> mSLJumpForce;
	/* 0xBC */ TParamRT<f32> mSLSearchLength;
	/* 0xD0 */ TParamRT<f32> mSLSearchHeight;
	/* 0xE4 */ TParamRT<f32> mSLSearchAware;
	/* 0xF8 */ TParamRT<f32> mSLSearchAngle;
	/* 0x10C */ TParamRT<f32> mSLGiveUpLength;
	/* 0x120 */ TParamRT<f32> mSLGiveUpHeight;
	/* 0x134 */ TParamRT<s32> mSLAttackWait;
	/* 0x148 */ TParamRT<s32> mSLFreezeWait;
	/* 0x15C */ TParamRT<s32> mSLDamageRadius;
	/* 0x170 */ TParamRT<s32> mSLDamageHeight;
	/* 0x184 */ TParamRT<s32> mSLAttackRadius;
	/* 0x198 */ TParamRT<s32> mSLAttackHeight;
	/* 0x1AC */ TParamRT<f32> mSLTurnSpeedLow;
	/* 0x1C0 */ TParamRT<f32> mSLTurnSpeedHigh;
	/* 0x1D4 */ TParamRT<f32> mSLBodyScaleLow;
	/* 0x1E8 */ TParamRT<f32> mSLBodyScaleHigh;
	/* 0x1FC */ TParamRT<f32> mSLGenItemRate;
	/* 0x210 */ TParamRT<f32> mSLGenEggRate;
	/* 0x224 */ TParamRT<u8> mSLPolluteRange;
	/* 0x238 */ TParamRT<s32> mSLWaitTime;
	/* 0x24C */ TParamRT<s32> mSLPolluteRMin;
	/* 0x260 */ TParamRT<s32> mSLPolluteRMax;
	/* 0x274 */ TParamRT<s32> mSLPolluteCycle;
	/* 0x288 */ TParamRT<u8> mSLStampRange;
	/* 0x29C */ TParamRT<s32> mSLPolluteInterval;
	/* 0x2B0 */ TParamRT<u8> mSLGenerateOnlyDead;
	/* 0x2C4 */ f32 unk2C4;
	/* 0x2C8 */ f32 unk2C8;
	/* 0x2CC */ f32 unk2CC;
	/* 0x2D0 */ f32 unk2D0;
};

class TSmallEnemyManager : public TEnemyManager {
public:
	TSmallEnemyManager(const char*);
	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void createModelData();
	virtual void createEnemies(int);
	virtual void initSetEnemies() { }

	TSmallEnemy* getHolder(int);

	// fabricated
	TSmallEnemyParams* getSaveParam2() const
	{
		return (TSmallEnemyParams*)unk38;
	}
	u16 getUnk58() const { return unk58; }

	static int mBlockWaitTime;
	static f32 mBlockMoveSpeed;
	static f32 mBlockWaitMoveY;
	static u32 mChangeBlockTime;
	static f32 mBlockXZScale;
	static f32 mBlockYScale;

	static u8 mTestJuiceType;

public:
	/* 0x54 */ char unk54[0x4];
	/* 0x58 */ u16 unk58;
	/* 0x5C */ u32 unk5C;
};

class TSmallEnemy : public TSpineEnemy {
public:
	TSmallEnemy(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void init(TLiveManager*);
	virtual void moveObject();
	virtual void kill();
	virtual void updateAnmSound();
	virtual void reset();
	virtual void genRandomItem();
	virtual void genEventCoin();
	virtual void generateItem();
	virtual bool isEatenByYosshi() { return true; }
	virtual void setBehavior();
	virtual void jumpBehavior() { }
	virtual void behaveToWater(THitActor*);
	virtual bool changeByJuice();
	virtual bool changeMove();
	virtual int getChangeBlockTime();
	virtual void scalingChangeActor();
	virtual void changeOut();
	virtual void behaveToTaken(THitActor*) { }
	virtual void behaveToRelease() { }
	virtual void setGenerateAnm() { }
	virtual void setWalkAnm();
	virtual void setDeadAnm() { }
	virtual void setFreezeAnm() { }
	virtual void setMeltAnm() { }
	virtual void setWaitAnm() { }
	virtual void setRunAnm();
	virtual void attackToMario();
	virtual void forceKill();
	virtual void setMActorAndKeeper();
	virtual void initAttacker(THitActor*);
	virtual bool isHitValid(u32)
	{
		return checkLiveFlag(LIVE_FLAG_HIDDEN) ? false : true;
	}
	virtual bool isCollidMove(THitActor*);
	virtual BOOL isInhibitedForceMove() { return FALSE; }
	virtual void endHitWaterJump() { }
	virtual void sendAttackMsgToMario();
	virtual void decHpByWater(THitActor*);
	virtual void setBckAnm(int index);
	virtual void setDeadEffect() { }
	virtual void setAfterDeadEffect();
	virtual bool doKeepDistance() { return false; }
	virtual void generateEffectColumWater();
	virtual bool isFindMario(float);

	bool isMarioInWater() const;
	bool isFindMarioFromParam(float) const;
	void expandCollision();
	bool isEaten();
	bool isHitWallInBound();
	void behaveToHitOthers(THitActor*);

	// fabricated
	TSmallEnemyParams* getSaveParam2() const
	{
		return (TSmallEnemyParams*)TSpineEnemy::getSaveParam();
	}
	f32 getUnk158() const { return unk158; }
	u8 getUnk184() const { return unk184; }

	static bool mIsPolluter;
	static bool mIsAmpPolluter;

	bool checkUnk150(u32 param_1) const { return unk150 & param_1; }
	bool isBckAnm(int index) const
	{
		return mCurrentBckAnm == index ? true : false;
	}
	bool unsetUnk165()
	{
		bool result = unk165;
		if (unk165)
			unk165 = false;
		return result;
	}

public:
	/* 0x150 */ u32 unk150;
	/* 0x154 */ f32 unk154;
	/* 0x158 */ f32 unk158;
	/* 0x15C */ int mCurrentBckAnm;
	/* 0x160 */ int mSprayedByWaterCooldown;
	/* 0x164 */ u8 unk164;
	/* 0x165 */ bool unk165;
	/* 0x166 */ char unk166[2];
	/* 0x168 */ char unk168[0x174 - 0x168];
	/* 0x174 */ u32 unk174;
	/* 0x178 */ TJuiceBlock* mJuiceBlock;
	/* 0x17C */ int mCoinId;
	/* 0x180 */ TCoin* mCoin;
	/* 0x184 */ u8 unk184;
	/* 0x185 */ u8 unk185;
	/* 0x188 */ f32 unk188;
	/* 0x18C */ int unk18C;
	/* 0x190 */ f32 unk190;
};

class TLiveActor;

DECLARE_NERVE(TNerveSmallEnemyDie, TLiveActor);
DECLARE_NERVE(TNerveSmallEnemyFreeze, TLiveActor);
DECLARE_NERVE(TNerveSmallEnemyJump, TLiveActor);
DECLARE_NERVE(TNerveSmallEnemyHitWaterJump, TLiveActor);
DECLARE_NERVE(TNerveSmallEnemyWait, TLiveActor);
DECLARE_NERVE(TNerveSmallEnemyChange, TLiveActor);

#endif
