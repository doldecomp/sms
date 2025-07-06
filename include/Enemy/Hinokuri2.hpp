#ifndef ENEMY_HINOKURI2_HPP
#define ENEMY_HINOKURI2_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Enemy/FeetInv.hpp>

class TWaterEmitInfo;
class TMBindShadowBody;

class THino2Params : public TSpineEnemyParams {
public:
	THino2Params(const char*);

	// TODO: did they really have accessors for all of these or nah???
	// fabricated
	int getSLFreezeTimerLv0() { return mSLFreezeTimerLv0.get(); }
	int getSLDamageTimer() { return mSLDamageTimer.get(); }

	/* 0xA8 */ TParamRT<f32> mSLMarchSpeedLv0;
	/* 0xBC */ TParamRT<f32> mSLMarchSpeedLv1;
	/* 0xD0 */ TParamRT<f32> mSLMarchSpeedLv2;
	/* 0xE4 */ TParamRT<f32> mSLTurnSpeedLv0;
	/* 0xF8 */ TParamRT<f32> mSLTurnSpeedLv1;
	/* 0x10C */ TParamRT<f32> mSLTurnSpeedLv2;
	/* 0x120 */ TParamRT<f32> mSLBodyScale;
	/* 0x134 */ TParamRT<f32> mSLWalkShake;
	/* 0x148 */ TParamRT<f32> mSLJumpShake;
	/* 0x15C */ TParamRT<f32> mSLGravityY;
	/* 0x170 */ TParamRT<s32> mSLPrePolWait;
	/* 0x184 */ TParamRT<s32> mSLPolWaitCount;
	/* 0x198 */ TParamRT<s32> mSLPolIntervalMin;
	/* 0x1AC */ TParamRT<s32> mSLPolIntervalMax;
	/* 0x1C0 */ TParamRT<f32> mSLDamageHeadScale;
	/* 0x1D4 */ TParamRT<s32> mSLDamageTimer;
	/* 0x1E8 */ TParamRT<f32> mSLHeadHitR;
	/* 0x1FC */ TParamRT<f32> mSLHeadHitH;
	/* 0x210 */ TParamRT<f32> mSLBodyHitR;
	/* 0x224 */ TParamRT<f32> mSLBodyHitH;
	/* 0x238 */ TParamRT<f32> mSLBodyHitR0;
	/* 0x24C */ TParamRT<f32> mSLBodyHitH0;
	/* 0x260 */ TParamRT<f32> mSLBankProp;
	/* 0x274 */ TParamRT<f32> mSLBankLimit;
	/* 0x288 */ TParamRT<f32> mSLJumpQuakeLen;
	/* 0x29C */ TParamRT<f32> mSLStampProb;
	/* 0x2B0 */ TParamRT<s32> mSLStampCount;
	/* 0x2C4 */ TParamRT<f32> mSLStampQuakeLen;
	/* 0x2D8 */ TParamRT<f32> mSLWaterEmitPos;
	/* 0x2EC */ TParamRT<u8> mSLHitPointMaxLv0;
	/* 0x300 */ TParamRT<u8> mSLHitPointMaxLv1;
	/* 0x314 */ TParamRT<u8> mSLHitPointMaxLv2;
	/* 0x328 */ TParamRT<s32> mSLFreezeTimerLv0;
	/* 0x33C */ TParamRT<s32> mSLInvincibleTimer;
	/* 0x350 */ TParamRT<f32> mSLWalkSpeedRateLv0;
};

class THinokuri2Manager : public TEnemyManager {
public:
	THinokuri2Manager(const char*);

	virtual void load(JSUMemoryInputStream&);
	void createModelData();
	TSpineEnemy* createEnemyInstance();
};

class THino2MtxCalc : public TMtxCalcFootInv {
public:
	THino2MtxCalc(u16, u16, u16, u16, u16, u16, f32);
	virtual void calc(u16);

public:
	/* 0x78 */ f32 unk78;
};

class THinokuri2;

class THino2Hit : public THitActor {
public:
	THino2Hit(THinokuri2* owner, int joint_idx, const char* name);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor*, u32);

public:
	/* 0x68 */ THinokuri2* mOwner;
	/* 0x6C */ int mJointIdx;
};

class THino2Mask {
public:
	THino2Mask(THinokuri2*);
	void setMatrix(MtxPtr);
	void breakMask();
	void startDamageMotion();
	void perform(u32, JDrama::TGraphics*);

	// fabricated
	void reset()
	{
		unk4 = 2;
		unk8 = 0;
		unk1C.zero();
		unk28.zero();
		unk34.set(0.0f, 0.0f, -5.0f);
		unk40.set(0.0f, 0.0f, 5.0f);
	}

	MtxPtr getUnk4C() { return unk4C; }

public:
	/* 0x0 */ THinokuri2* unk0;
	/* 0x4 */ int unk4;
	/* 0x8 */ int unk8;
	/* 0xC */ int unkC;
	/* 0x10 */ MActor* unk10;
	/* 0x14 */ MActor* unk14;
	/* 0x18 */ MActor* unk18;
	/* 0x1C */ JGeometry::TVec3<f32> unk1C;
	/* 0x28 */ JGeometry::TVec3<f32> unk28;
	/* 0x34 */ JGeometry::TVec3<f32> unk34;
	/* 0x40 */ JGeometry::TVec3<f32> unk40;
	/* 0x4C */ Mtx unk4C;
};

class THinokuri2 : public TSpineEnemy {
public:
	THinokuri2(const char*);

	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void init(TLiveManager*);
	virtual void moveObject();
	virtual void kill();
	virtual void reset();

	void emitPolParticle();
	void stopPolParticle();
	void updatePolTrans();
	void resetPolInterval();
	void invalidateCollisionAll();
	void validateCollisionAll();
	void emitWaterParticle();
	void shakeCamera(int);
	void makeQuake(f32);
	void setLevel(int);
	void generateEnemy();
	void updateAnmSound();
	void changeBck(int);
	BOOL receiveMessageLv0(THitActor*, u32);
	BOOL receiveMessageLv1(THitActor*, u32);
	BOOL receiveMessageLv2(THitActor*, u32);

	// fabricated
	THino2Params* getSaveParam() const
	{
		return (THino2Params*)TSpineEnemy::getSaveParam();
	}

	// fabricated
	u8 calcHitPoints()
	{
		switch (mLevel) {
		case 0:
			return getSaveParam()->mSLHitPointMaxLv0.get();
			break;
		case 1:
			return getSaveParam()->mSLHitPointMaxLv1.get();
			break;
		case 2:
			return getSaveParam()->mSLHitPointMaxLv2.get();
			break;
		default:
			if (getSaveParam())
				return getSaveParam()->mSLHitPointMax.get();
			break;
		}

		return 1;
	}

public:
	/* 0x150 */ TMBindShadowBody* unk150;
	/* 0x154 */ int mCurrentBck;
	/* 0x158 */ int unk158;
	/* 0x15C */ u32 unk15C;
	/* 0x160 */ int unk160;
	/* 0x164 */ int unk164;
	/* 0x168 */ int unk168;
	/* 0x16C */ THino2Hit* mHead;
	/* 0x170 */ THino2Hit* mBody;
	/* 0x174 */ THino2Hit* unk174;
	/* 0x178 */ THino2Hit* unk178;
	/* 0x17C */ int mJointIdxMessageCameFrom;
	/* 0x180 */ int unk180;
	/* 0x184 */ u32 unk184;
	/* 0x188 */ int unk188;
	/* 0x18C */ int unk18C;
	/* 0x190 */ int unk190;
	/* 0x194 */ f32 unk194;
	/* 0x198 */ f32 unk198;
	/* 0x19C */ TWaterEmitInfo* unk19C;
	/* 0x1A0 */ THino2MtxCalc* unk1A0;
	/* 0x1A4 */ THino2Mask* unk1A4;
	/* 0x1A8 */ s32 mLevel;
};

DECLARE_NERVE(TNerveHino2Appear, TLiveActor);
DECLARE_NERVE(TNerveHino2GraphWander, TLiveActor);
DECLARE_NERVE(TNerveHino2Fly, TLiveActor);
DECLARE_NERVE(TNerveHino2JumpIn, TLiveActor);
DECLARE_NERVE(TNerveHino2Landing, TLiveActor);
DECLARE_NERVE(TNerveHino2Turn, TLiveActor);
DECLARE_NERVE(TNerveHino2PrePol, TLiveActor);
DECLARE_NERVE(TNerveHino2Pollute, TLiveActor);
DECLARE_NERVE(TNerveHino2Damage, TLiveActor);
DECLARE_NERVE(TNerveHino2Squat, TLiveActor);
DECLARE_NERVE(TNerveHino2Burst, TLiveActor);
DECLARE_NERVE(TNerveHino2Die, TLiveActor);
DECLARE_NERVE(TNerveHino2Stamp, TLiveActor);
DECLARE_NERVE(TNerveHino2Freeze, TLiveActor);
DECLARE_NERVE(TNerveHino2WaitAnm, TLiveActor);

#endif
