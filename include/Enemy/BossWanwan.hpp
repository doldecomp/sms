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
#include <Enemy/EnemyManager.hpp>
#include <Enemy/Enemy.hpp>
#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <MarioUtil/MtxUtil.hpp>
#include <GC2D/GCConsole2.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>

class TBossWanwan;
class TBWParams;
class TBWLeash;
class TBWLeashNode;
class TRope;
class TBossWanwanMtxCalc;

// fabricated
template <class T> T Wrap(T t, T l, T r);

class TBWHit : public THitActor {
public:
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);

private:
	/* 0x68 */ TBossWanwan* mOwner;
	/* 0x6C */ s32 mJointIndex;
};

class TBWPicket : public TTakeActor {
public:
	TBWPicket(const char* name = "ボスワンワン杭");
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual MtxPtr getTakingMtx();
	virtual BOOL moveRequest(const JGeometry::TVec3<float>&);

private:
	/* 0x70 */ TBossWanwan* mOwner;
	/* 0x74 */ TMtx34f unk74;
};

class TBWBinder : public TBinder {
public:
	TBWBinder() { };
	virtual ~TBWBinder();
	virtual void bind(TLiveActor*);
};

class TBWLeash : public JDrama::TViewObj {
public:
	TBWLeash(TBossWanwan* owner, int nodeCount,
	         const char* name = "ボスワンワンリーシュ");
	virtual ~TBWLeash();
	void perform(u32 cue, JDrama::TGraphics* graphics);
	TBWLeashNode* getNode(s32 idx) const { return mNodes[idx]; } // fabricated
	void pullTail(const JGeometry::TVec3<f32>& pos);
	void invalidateAllCollisions();

	/* 0x10 */ TBossWanwan* mOwner;
	/* 0x14 */ TRope* mRope;
	/* 0x18 */ TBWLeashNode** mNodes;
};

class TBWLeashNode : public THitActor {
public:
	TBWLeashNode(int, const char*);

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	void calcMatrix();
	void calcTemperature();

private:
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

// TBossWanwan size: 0x1b8
class TBossWanwan : public TSpineEnemy {
public:
	TBossWanwan(const char* name = "ボスワンワン");

	virtual void kill();
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
	TBWParams* getSaveParam() const { return (TBWParams*)getSaveParam(); }

	// UNUSED
	bool isHeadPulled();
	bool isMarioInSight();
	bool isTailBurning();
	bool isBurning();
	void startGoldBrk();
	void releasePicket();
	void takeBath();
	void rollNextGraphNode();
	void reverseNextGraphNode();
	void showMessage();

	// fabricated
	const TNerveBase<TLiveActor>* getLatestNerve()
	{
		return mSpine->getLatestNerve();
	}

	/* 0x150 */ TBossWanwanMtxCalc* mMtxCalc;
	/* 0x154 */ TBWLeash* mLeash;
	/* 0x158 */ TBWPicket* mPicket;
	/* 0x15C */ JGeometry::TVec3<f32> mPicketPullDelta; // guessed name
	/* 0x168 */ f32 unk168;
	/* 0x16C */ s32 unk16C;
	/* 0x170 */ u32 unk170;
	/* 0x174 */ u32 unk174;
	/* 0x178 */ f32 unk178;
	/* 0x17C */ BOOL unk17C;
	/* 0x180 */ u32 unk180;
	/* 0x184 */ u32 unk184;
	/* 0x188 */ u32 unk188;
	/* 0x18C */ u8 msInvincible;
	/* 0x18D */ u8 unk18D;
	/* 0x190 */ s32 unk190;
	/* 0x194 */ s8 unk194;
	/* 0x195 */ u8 unk195;
	/* 0x198 */ u32 unk198;
	/* 0x19C */ u32 unk19C;
	/* 0x1A0 */ u8 unk1A0;
	/* 0x1A4 */ f32 unk1A4;
	/* 0x1A8 */ f32 mWaterHitCount;
	/* 0x1AC */ f32 unk1AC;
	/* 0x1B0 */ TBWParams* mParams;
	/* 0x1B4 */ u16 unk1B4;
};

class TBossWanwanMtxCalc : public M3UMtxCalcSIAnmBlendQuat {
public:
	TBossWanwanMtxCalc(TBossWanwan*);
	virtual ~TBossWanwanMtxCalc();
	void calc(u16);

	void joinAnm(int i)
	{
		J3DAnmTransformKey* newAnm
		    = mOwner->getActorKeeper()->getMActorAnmData()->mBckAnms->getAnmPtr(
		        i);

		if (mNewAnm != newAnm) {
			mOldAnm           = mNewAnm;
			mNewAnm           = newAnm;
			mMotionBlendRatio = 1.0f;
		}
	}

public:
	/* 0x64 */ TBossWanwan* mOwner;
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
