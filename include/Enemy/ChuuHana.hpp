#ifndef ENEMY_CHUUHANA_HPP
#define ENEMY_CHUUHANA_HPP

#include <Enemy/WalkerEnemy.hpp>
#include <Strategic/Nerve.hpp>
#include <JSystem/JParticle/JPACallback.hpp>

class TChuuHana;
class TGraphWeb;
class JPABaseEmitter;
class JPABaseParticle;

// Names and defaults are the ones PARAM_INIT stringified into .rodata and the
// constructor stores; the class is 0x534 bytes (the manager's operator new).
class TChuuHanaSaveLoadParams : public TWalkerEnemyParams {
public:
	TChuuHanaSaveLoadParams(const char* prm);

	/* 0x32C */ TParamRT<f32> mSLGetWaterPow;
	/* 0x340 */ TParamRT<f32> mSLGetGroundPow;
	/* 0x354 */ TParamRT<s32> mSLKeepBalanceTime;
	/* 0x368 */ TParamRT<s32> mSLCheckFrame;
	/* 0x37C */ TParamRT<f32> mSLReverseHeightS;
	/* 0x390 */ TParamRT<f32> mSLStretchHeightS;
	/* 0x3A4 */ TParamRT<f32> mSLMediumStretchHeightS;
	/* 0x3B8 */ TParamRT<f32> mSLSmallStretchHeightS;
	/* 0x3CC */ TParamRT<f32> mSLReverseHeightM;
	/* 0x3E0 */ TParamRT<f32> mSLStretchHeightM;
	/* 0x3F4 */ TParamRT<f32> mSLMediumStretchHeightM;
	/* 0x408 */ TParamRT<f32> mSLSmallStretchHeightM;
	/* 0x41C */ TParamRT<f32> mSLReverseHeightL;
	/* 0x430 */ TParamRT<f32> mSLStretchHeightL;
	/* 0x444 */ TParamRT<f32> mSLMediumStretchHeightL;
	/* 0x458 */ TParamRT<f32> mSLSmallStretchHeightL;
	/* 0x46C */ TParamRT<f32> mSLWalkGravity;
	/* 0x480 */ TParamRT<f32> mSLWaterHitGravity;
	/* 0x494 */ TParamRT<f32> mSLJumpGravity;
	/* 0x4A8 */ TParamRT<f32> mSLJumpSp;
	/* 0x4BC */ TParamRT<f32> mSLJumpHeight;
	/* 0x4D0 */ TParamRT<f32> mSLGetWaterPow2;
	/* 0x4E4 */ TParamRT<f32> mSLTacklePow;
	/* 0x4F8 */ TParamRT<f32> mSLDashRate;
	/* 0x50C */ TParamRT<s32> mSLAttackTimer;
	/* 0x520 */ TParamRT<s32> mSLHitWaterTimer;
};

class TChuuHanaAseParCallback
    : public JPACallBackBase2<JPABaseEmitter*, JPABaseParticle*> {
public:
	TChuuHanaAseParCallback(TChuuHana*);
	void execute(JPABaseEmitter*, JPABaseParticle*);
	void draw(JPABaseEmitter*, JPABaseParticle*);

public:
	/* 0x4 */ TChuuHana* mOwner;
};

class TChuuHana : public TWalkerEnemy {
public:
	TChuuHana(const char* name = "チュウハナ");

	// Inherited slots this class overrides, in vtable order.
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void bind();
	virtual void moveObject();
	virtual void kill();
	virtual f32 getGravityY() const;
	virtual const char** getBasNameTable() const;
	virtual void reset();
	virtual void behaveToWater(THitActor*);
	virtual void setWalkAnm();
	virtual void attackToMario();
	virtual void forceKill();
	virtual void setMActorAndKeeper();
	virtual bool isCollidMove(THitActor*);
	virtual void setBckAnm(int);
	virtual bool isFindMario(f32) { return false; }

	bool willFall(long);
	void setGoal();
	void checkStretchType();

	// UNUSED in the map; inlined into the nerves and methods above.
	MtxPtr getEffectMtx();
	void eventKill();
	void entryCollision();
	void rollStart();
	void rolling();
	void setSafeGoal();
	bool checkOnPanel();
	void margeVelocity(JGeometry::TVec3<f32>&);
	void forceRoll();
	bool isRolling();

	// fabricated
	TChuuHanaSaveLoadParams* getSaveParam2() const
	{
		return (TChuuHanaSaveLoadParams*)getSaveParam();
	}

	static s32 mCheckOnPanelTimeRoll;
	static s32 mCheckOnPanelTime;
	static u8 mBodyJntIndex;
	static u8 mEyeJntIndex;
	static u8 mFootJntIndex;
	static u8 mNewSw;
	static u8 mCompareHeight;
	static f32 mSmallMirrorR;
	static f32 mMediumMirrorR;
	static f32 mLargeMirrorR;
	static u8 mAttackVersion;
	static u8 mDamageSw;

	/* 0x194 */ f32 unk194;
	/* 0x198 */ f32 unk198;
	/* 0x19C */ f32 unk19C;
	/* 0x1A0 */ s32 unk1A0;
	/* 0x1A4 */ s32 unk1A4;
	/* 0x1A8 */ f32 unk1A8;
	/* 0x1AC */ u32 unk1AC;
	/* 0x1B0 */ u8 unk1B0;
	/* 0x1B1 */ u8 unk1B1;
	/* 0x1B2 */ u8 unk1B2;
	/* 0x1B4 */ TChuuHanaSaveLoadParams* unk1B4;
	/* 0x1B8 */ f32 unk1B8;
	/* 0x1BC */ u8 unk1BC[0x1EC - 0x1BC];
	/* 0x1EC */ JGeometry::TVec3<f32> unk1EC;
	/* 0x1F8 */ JGeometry::TVec3<f32> unk1F8;
	/* 0x204 */ JGeometry::TVec3<f32> unk204;
	/* 0x210 */ f32 unk210;
	/* 0x214 */ u8 unk214;
	/* 0x215 */ u8 unk215;
	/* 0x218 */ THitActor* unk218;
	/* 0x21C */ u8* unk21C;
	/* 0x220 */ f32 unk220;
	/* 0x224 */ s32 unk224;
	/* 0x228 */ TChuuHanaAseParCallback mAseParCallback;
};

class TChuuHanaManager : public TSmallEnemyManager {
public:
	TChuuHanaManager(const char* name);

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual TSpineEnemy* createEnemyInstance();
	virtual void initSetEnemies();

	/* 0x60 */ s32 unk60;
	/* 0x64 */ u8 unk64[3];
	/* 0x68 */ s32 unk68;
	/* 0x6C */ s32 unk6C;
	/* 0x70 */ s32 unk70;
};

DECLARE_NERVE(TNerveChuuHanaAttack, TLiveActor)
DECLARE_NERVE(TNerveChuuHanaFall, TLiveActor)
DECLARE_NERVE(TNerveChuuHanaFall2, TLiveActor)
DECLARE_NERVE(TNerveChuuHanaForceJumped, TLiveActor)
DECLARE_NERVE(TNerveChuuHanaJumpPrepare, TLiveActor)
DECLARE_NERVE(TNerveChuuHanaKeepBalance, TLiveActor)
DECLARE_NERVE(TNerveChuuHanaObject, TLiveActor)
DECLARE_NERVE(TNerveChuuHanaRoll, TLiveActor)
DECLARE_NERVE(TNerveChuuHanaStick, TLiveActor)
DECLARE_NERVE(TNerveChuuHanaWait, TLiveActor)
DECLARE_NERVE(TNerveChuuHanaWalkOnPanel, TLiveActor)

#endif
