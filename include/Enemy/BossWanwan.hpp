#ifndef ENEMY_BOSS_WANWAN_HPP
#define ENEMY_BOSS_WANWAN_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/JGeometry.hpp>
#include <M3DUtil/M3UJoint.hpp>
#include <M3DUtil/MActorData.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Binder.hpp>
#include <Strategic/Nerve.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/TakeActor.hpp>

class TRope;
class TBossWanwan;
class TBWLeash;

// The big Chain Chomp of Pianta Village ("Chain Chomp's Bath").
//
// Actor types used by this boss:
//   ACTOR_TYPE_BOSS_UNKB - the chomp itself and its two TBWHit hit volumes
//   ACTOR_TYPE_BOSS_UNKC - a link of the chain (TBWLeashNode)
//   ACTOR_TYPE_BOSS_WANWAN - the picket at the end of the chain (TBWPicket)

DECLARE_NERVE(TNerveBWGraphWander, TLiveActor);
DECLARE_NERVE(TNerveBWRoll, TLiveActor);
DECLARE_NERVE(TNerveBWBark, TLiveActor);
DECLARE_NERVE(TNerveBWJump, TLiveActor);
DECLARE_NERVE(TNerveBWStun, TLiveActor);
DECLARE_NERVE(TNerveBWWakeup, TLiveActor);
DECLARE_NERVE(TNerveBWJumpToBath, TLiveActor);
DECLARE_NERVE(TNerveBWDie, TLiveActor);
DECLARE_NERVE(TNerveBWJumpAway, TLiveActor);
DECLARE_NERVE(TNerveBWShake, TLiveActor);
DECLARE_NERVE(TNerveBWFall, TLiveActor);

class TBWParams : public TSpineEnemyParams {
public:
	TBWParams(const char*);

	/* 0x0A8 */ TParamRT<f32> mSLMarchSpeed;
	/* 0x0BC */ TParamRT<f32> mSLTurnSpeed;
	/* 0x0D0 */ TParamRT<f32> mSLLeashNodeLen;
	/* 0x0E4 */ TParamRT<f32> mSLPicketHeight;
	/* 0x0F8 */ TParamRT<f32> mSLPicketRadius;
	/* 0x10C */ TParamRT<f32> mSLChainHitHeight;
	/* 0x120 */ TParamRT<f32> mSLChainHitRadius;
	/* 0x134 */ TParamRT<f32> mSLChainGroundRadius;
	/* 0x148 */ TParamRT<f32> mSLPullLimit;
	/* 0x15C */ TParamRT<f32> mSLAttackSpeed;
	/* 0x170 */ TParamRT<s32> mSLStunTimer;
	/* 0x184 */ TParamRT<f32> mSLSearchLength;
	/* 0x198 */ TParamRT<f32> mSLSearchAngle;
	/* 0x1AC */ TParamRT<u8> mSLBWHitPointMax;
	/* 0x1C0 */ TParamRT<f32> mSLHeadGap;
	/* 0x1D4 */ TParamRT<f32> mSLShakeLengthMax;
	/* 0x1E8 */ TParamRT<f32> mSLShakeLengthMaxHP0;
};

// One link of the chain.
class TBWLeashNode : public THitActor {
public:
	TBWLeashNode(TBWLeash*, int, const char*);

	virtual void perform(u32, JDrama::TGraphics*);

	void calcMatrix();
	void calcTemperature();

public:
	/* 0x68 */ TBWLeash* mLeash;
	/* 0x6C */ MActor* mMActor;
	/* 0x70 */ f32 mTemperature;
	/* 0x74 */ int mIndex;
};

// The chain between the chomp and its picket.
class TBWLeash : public JDrama::TViewObj {
public:
	TBWLeash(TBossWanwan*, int, const char*);

	virtual void perform(u32, JDrama::TGraphics*);

	void pullTail(const JGeometry::TVec3<f32>&);
	void invalidateAllCollision();

public:
	/* 0x10 */ TBossWanwan* mOwner;
	/* 0x14 */ TRope* mRope;
	/* 0x18 */ TBWLeashNode** mNodes;
};

// The picket the chain is staked to. Mario can pull it out and carry it.
class TBWPicket : public TTakeActor {
public:
	TBWPicket(TBossWanwan*, const char*);

	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual MtxPtr getTakingMtx();
	virtual BOOL moveRequest(const JGeometry::TVec3<f32>&);

public:
	/* 0x70 */ TBossWanwan* mOwner;
	/* 0x74 */ TPosition3f mTakingMtx;
	/* 0xA4 */ MActor* mMActor;
};

class TBWHit : public THitActor {
public:
	TBWHit(TBossWanwan*, int, const char*);

	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor*, u32);

public:
	/* 0x68 */ TBossWanwan* mOwner;
	/* 0x6C */ int mJointIndex;
};

class TBWBinder : public TBinder {
public:
	TBWBinder();

	virtual void bind(TLiveActor*);
};

class TBossWanwanMtxCalc : public M3UMtxCalcSIAnmBlendQuat {
public:
	TBossWanwanMtxCalc(TBossWanwan*);

	virtual void calc(u16);

	void joinAnm(int);

	// fabricated: the original keeps getMActorAnmData and getAnmPtr as calls
	// at every joinAnm site, which only a 1-statement wrapper around the
	// lookup reproduces (it puts the lookup one inline level deeper).
	J3DAnmTransformKey* getBckAnmPtr(int index) const;

public:
	/* 0x64 */ TBossWanwan* mOwner;
};

class TBossWanwan : public TSpineEnemy {
public:
	TBossWanwan(const char*);

	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void control();
	virtual void kill() { }

	void showMessage(u32);
	void emitEffects();
	void slideToCurPathNode(f32, f32);
	void rollNextGraphNode();
	void reverseNextGraphNode();
	BOOL isHeadPulled();
	BOOL isMarioInSight();
	BOOL isTailBurning();
	BOOL isBurning();
	void startGoldBrk();
	void changeBck(int);
	void releasePicket();
	void takeBath();
	void shakeCamera(int);

	TBWParams* getBWParams() const
	{
		return static_cast<TBWParams*>(getSaveParam());
	}

	// fabricated, same shape as TBossPakkun::getLatestNerve
	const TNerveBase<TLiveActor>* getLatestNerve() const
	{
		return mSpine->getLatestNerve();
	}

public:
	/* 0x150 */ TBossWanwanMtxCalc* mMtxCalc;
	/* 0x154 */ TBWLeash* mLeash;
	/* 0x158 */ TBWPicket* mPicket;
	/* 0x15C */ JGeometry::TVec3<f32> unk15C;
	/* 0x168 */ f32 unk168;
	/* 0x16C */ s32 unk16C;
	/* 0x170 */ TBWHit* unk170[2];
	/* 0x178 */ f32 unk178;
	/* 0x17C */ s32 unk17C;
	/* 0x180 */ s32 unk180;
	/* 0x184 */ s32 unk184;
	/* 0x188 */ s32 unk188;
	/* 0x18C */ s8 unk18C;
	/* 0x18D */ s8 unk18D;
	/* 0x190 */ s32 unk190;
	/* 0x194 */ s8 unk194;
	/* 0x195 */ s8 unk195;
	/* 0x198 */ s32 unk198;
	/* 0x19C */ s32 unk19C;
	/* 0x1A0 */ s8 unk1A0;
	/* 0x1A4 */ JGeometry::TVec3<f32> unk1A4;
	/* 0x1B0 */ s32 unk1B0;
	/* 0x1B4 */ s16 unk1B4;
};

inline J3DAnmTransformKey* TBossWanwanMtxCalc::getBckAnmPtr(int index) const
{
	return mOwner->mMActorKeeper->getMActorAnmData()->mBckAnms->getAnmPtr(index);
}

class TBossWanwanManager : public TEnemyManager {
public:
	TBossWanwanManager(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();

	void initJParticle();
};

#endif
