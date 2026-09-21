#ifndef ENEMY_BOSS_WANWAN_HPP
#define ENEMY_BOSS_WANWAN_HPP

#include <Strategic/Nerve.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/LiveManager.hpp>
#include <Strategic/Binder.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Enemy/Enemy.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>

class TLiveActor;
class TBossWanwan;
class TBWLeash;
class TBWLeashNode;

class TBWHit : public THitActor {
public:
	TBWHit(TBossWanwan* owner, int joint_index,
	       const char* name = "idk"); // yeah

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	void getTakingMtx();
	void moveRequest(const JGeometry::TVec3<float>&);

private:
	TBossWanwan* mOwner;
	u32 mJointIndex;
};

class TBWPicket : public THitActor {
public:
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	void getTakingMtx();
	void moveRequest(const JGeometry::TVec3<float>&);
};

class TBWBinder : public TBinder {
public:
	TBWBinder();
	virtual void bind(TLiveActor*);
};

class TBWLeash {
public:
	TBWLeash(TBossWanwan*, int, const char*);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	TBWLeashNode* getNode(s32 idx) const { return mNodes[idx]; } // fabricated

	/* 0x4  */ u32 unk0; // fabr
	/* 0x8  */ u32 unk1; // fabr
	/* 0xc  */ u32 unk2; // fabr
	/* 0x10 */ f32 unk3; // fabr
	/* 0x14 */ f32 unk4; // fabr
	/* 0x18 */ TBWLeashNode** mNodes;
};

class TBWLeashNode : public THitActor {
public:
	TBWLeashNode(int, const char*);

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual void calcMatrix();
	virtual void calcTemperature();

private:
	// THitActor ends at 0x68
	TBWLeash* mLeash; // +0x68
	f32 unk0;         // +0x6C
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
	BOOL receiveMessage(THitActor* sender, u32 message);
	void calcRootMatrix();
	void slideToCurPathNode(float, float);
	void control();
	void emitEffects();
	void perform(u32 cue, JDrama::TGraphics* graphics);

private:
	/* 0x150 */ TBWLeash* mLeash;
	/* 0x154 */ TBWPicket* mPicket;
	/* 0x158 */ TBWLeashNode* mChainRoot;
	/* 0x15C */ u16 unk15C;
	/* 0x160 */ u32 unk160;
	/* 0x164 */ u32 unk164;
	/* 0x168 */ u32 unk168;
	/* 0x16C */ u32 unk16C;
	/* 0x170 */ u32 unk170;
	/* 0x174 */ u32 unk174;
	/* 0x178 */ u32 unk178;
	/* 0x17C */ u32 unk17C;
	/* 0x180 */ u32 unk180;
	/* 0x184 */ u32 unk184;
	/* 0x188 */ u32 unk188;
	/* 0x18C */ u32 unk18C;
	/* 0x190 */ u16 unk190;
	/* 0x1A0 */ BOOL unk1a0;
	/* 0x1A8 */ u32 mWaterHitCount;
	/* 0x1AC */ u32 mDistToMarioSquared;
	/* 0x1B0 */ TBWParams* mParams;
	/* 0x18C */ u8 msInvincible;
};

class TBossWanwanMtxCalc : public J3DMtxCalcSoftimageAnm {
public:
	TBossWanwanMtxCalc(TBossWanwan*);

	virtual void calc(u16);

public:
	TBossWanwan mOwner;
};

class TBossWanwanManager : public TEnemyManager {
public:
	TBossWanwanManager(const char* name = "ボスワンワンマネージャ");

	virtual void load(JSUMemoryInputStream&);
	TSpineEnemy* createEnemyInstance();
	void createModelData();
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
