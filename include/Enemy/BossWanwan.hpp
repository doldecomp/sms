#ifndef ENEMY_BOSS_WANWAN_HPP
#define ENEMY_BOSS_WANWAN_HPP

#include <Strategic/Nerve.hpp>
#include <Strategic/HitActor.hpp>
#include <Strategic/TakeActor.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/Binder.hpp>
#include <Strategic/LiveManager.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/LiveActor.hpp>
#include <M3DUtil/M3UJoint.hpp>
#include <M3DUtil/MActorData.hpp>
#include <M3DUtil/MActor.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Enemy/Enemy.hpp>
#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <MarioUtil/MtxUtil.hpp>
#include <GC2D/GCConsole2.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/Particles.hpp>
#include <Enemy/Graph.hpp>
#include <Player/MarioAccess.hpp>
#include <System/MarDirector.hpp>

class TBossWanwan;
class TBWParams;
class TBWLeash;
class TBWLeashNode;
class TRope;
class TBossWanwanMtxCalc;

class TBWHit : public THitActor {
public:
	TBWHit(TBossWanwan* owner, int jointIndex,
	       const char* name = "ボスワンワンヒット");
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);

private:
	/* 0x68 */ TBossWanwan* mOwner;
	/* 0x6C */ s32 mJointIndex;
};

class TBWPicket : public TTakeActor {
public:
	TBWPicket(TBossWanwan* owner, const char* name = "ボスワンワンつかみ");
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual MtxPtr getTakingMtx();
	virtual BOOL moveRequest(const JGeometry::TVec3<float>&);

private:
	/* 0x70 */ TBossWanwan* mOwner;
	/* 0x74 */ TMtx34f unk74;
	/* 0xA4 */ MActor* mActor;
};

class TBWBinder : public TBinder {
public:
	TBWBinder();
	virtual ~TBWBinder() { };
	virtual void bind(TLiveActor*);
};

class TBWLeash : public JDrama::TViewObj {
public:
	TBWLeash(TBossWanwan* owner, int nodeCount,
	         const char* name = "ボスワンワン鎖");
	virtual ~TBWLeash() { };
	void perform(u32 cue, JDrama::TGraphics* graphics);
	TBWLeashNode* getNode(s32 idx) const { return mNodes[idx]; } // fabricated

	// UNUSED
	void pullTail(const JGeometry::TVec3<f32>& pos);
	void invalidateAllCollision();

	/* 0x10 */ TBossWanwan* mOwner;
	/* 0x14 */ TRope* mRope;
	/* 0x18 */ TBWLeashNode** mNodes;
};

class TBWLeashNode : public THitActor {
public:
	TBWLeashNode(TBWLeash*, int, const char*);

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	void calcMatrix();
	void calcTemperature();

	TBWLeash* mLeash; // +0x68
	MActor* mMActor;  // +0x6C
	f32 mTemperature; // +0x70
	int mIndex;       // +0x74
};

class TBWParams : public TSpineEnemyParams {
public:
	/* * TSpineEnemyParams ends at 0xA8. *
	 * Every TParamRT<T> occupies 0x14 bytes:
	 * * +0x00 : TParam / base data
	 * * +0x04 : ...
	 * * +0x0C : ...
	 * * +0x10 : actual parameter value
	 * * * Therefore: * parameter start = X * parameter value = X + 0x10 */
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

class TBossWanwanMtxCalc : public M3UMtxCalcSIAnmBlendQuat {
public:
	TBossWanwanMtxCalc(TBossWanwan*);
	virtual ~TBossWanwanMtxCalc() { };
	void calc(u16);

	void joinAnm(int i);

public:
	/* 0x64 */ TBossWanwan* mOwner;
};

// TBossWanwan size: 0x1b8
class TBossWanwan : public TSpineEnemy {
public:
	TBossWanwan(const char* name = "ボスワンワン");

	virtual void kill() { };
	virtual void init(TLiveManager*);
	void shakeCamera(int shakeType);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void calcRootMatrix();
	void slideToCurPathNode(float, float);
	virtual void control();
	void emitEffects();
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	f32 getUnk168() const { return unk168; }

	// fabricated
	f32 getHeadHeight() const { return mHeadHeight * mBodyScale; }

	// UNUSED
	BOOL isHeadPulled();
	bool isMarioInSight();
	bool isTailBurning();
	bool isBurning();
	void startGoldBrk();
	void releasePicket();
	void takeBath();
	void rollNextGraphNode();
	void reverseNextGraphNode();
	void showMessage(u32 msg);
	void changeBck(int);

	// fabricated
	const TNerveBase<TLiveActor>* getLatestNerve()
	{
		return mSpine->getLatestNerve();
	}

	// fabricated!!!
	MActorAnmData* getMActorAnmData() const
	{
		return mMActorKeeper->getMActorAnmData();
	}

	// fabricated
	void setBckFromIndex(int idx)
	{
		mMtxCalc->joinAnm(idx);
		getMActor()->getAnmBck()->setFrameCtrl(idx);
	}

	// fabricated
	static inline void setRotDirY(JGeometry::TVec3<f32>& dir, f32 rot)
	{
		u16 angle = (int)(rot * 182.04445f);
		dir.set(JMASSin(angle), 0.0f, JMASCos(angle));
	}

	// fabricated
	JGeometry::TVec3<f32> getRotDirY(f32 rot) const
	{
		JGeometry::TVec3<f32> rotdir;
		setRotDirY(rotdir, rot);
		return rotdir;
	}

	/* 0x150 */ TBossWanwanMtxCalc* mMtxCalc;
	/* 0x154 */ TBWLeash* mLeash;
	/* 0x158 */ TBWPicket* mPicket;
	/* 0x15C */ JGeometry::TVec3<f32> mPicketPullDelta;
	/* 0x168 */ f32 unk168;
	/* 0x16C */ s32 unk16C;
	/* 0x170 */ TBWHit* mHits[2];
	/* 0x178 */ f32 unk178;
	/* 0x17C */ BOOL mIsNearMario;
	/* 0x180 */ s32 unk180;
	/* 0x184 */ s32 unk184;
	/* 0x188 */ BOOL mIsBurnt;
	/* 0x18C */ s8 msInvincible;
	/* 0x18D */ s8 unk18D;
	/* 0x190 */ s32 unk190;
	/* 0x194 */ s8 unk194;
	/* 0x195 */ s8 mHasSpawnedWater;
	/* 0x198 */ u32 unk198;
	/* 0x19C */ s32 unk19C;
	/* 0x1A0 */ s8 unk1A0;
	/* 0x1A4 */ JGeometry::TVec3<f32> unk1A4;
	/* 0x1B0 */ TBWParams* mParams;
	/* 0x1B4 */ u16 unk1B4;
};

class TBossWanwanManager : public TEnemyManager {
public:
	TBossWanwanManager(const char* name = "ボスワンワンマネージャ");

	virtual void load(JSUMemoryInputStream&);
	virtual TSpineEnemy* createEnemyInstance();
	virtual void createModelData();
	void initJParticle();
};

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

#endif
