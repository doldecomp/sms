#ifndef ENEMY_BOSS_GESSO_HPP
#define ENEMY_BOSS_GESSO_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <M3DUtil/M3UJoint.hpp>
#include <Strategic/Binder.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/Nerve.hpp>
#include <JSystem/J3D/J3DGraphBase/Components/J3DGXColor.hpp>

class TBGTentacle;
class TBGPolDrop;
class TBossGesso;

class TBossGessoParams : public TSpineEnemyParams {
public:
	TBossGessoParams(const char*);

	// fabricated but starting to think that they might be real
	s32 getSLRestTime() const { return mSLRestTime.get(); }
	s32 getSLAmputeeTime() const { return mSLAmputeeTime.get(); }
	s32 getSLStunTime() const { return mSLStunTime.get(); }
	f32 getSLTentacleStretch() const { return mSLTentacleStretch.get(); }

	/* 0xA8 */ TParamRT<s32> mSLUnisonInter;
	/* 0xBC */ TParamRT<s32> mSLUnisonHoming;
	/* 0xD0 */ TParamRT<s32> mSLSingleHoming;
	/* 0xE4 */ TParamRT<s32> mSLBeakHoming;
	/* 0xF8 */ TParamRT<s32> mSLRegenFoot;
	/* 0x10C */ TParamRT<s32> mSLAmputeeTime;
	/* 0x120 */ TParamRT<s32> mSLRestTime;
	/* 0x134 */ TParamRT<s32> mSLStunTime;
	/* 0x148 */ TParamRT<f32> mSLBeakDamageHeight;
	/* 0x15C */ TParamRT<f32> mSLBeakDamageRadius;
	/* 0x170 */ TParamRT<f32> mSLBeakLengthLimit;
	/* 0x184 */ TParamRT<f32> mSLBeakLengthDamage;
	/* 0x198 */ TParamRT<f32> mSLBeakLengthPollute;
	/* 0x1AC */ TParamRT<f32> mSLUnisonAttackSpeed;
	/* 0x1C0 */ TParamRT<f32> mSLDoubleAttackSpeed;
	/* 0x1D4 */ TParamRT<f32> mSLSkipRopeAttackSpeed;
	/* 0x1E8 */ TParamRT<f32> mSLSingleAttackLen;
	/* 0x1FC */ TParamRT<f32> mSLDoubleAttackLen;
	/* 0x210 */ TParamRT<f32> mSLUnisonAttackLen;
	/* 0x224 */ TParamRT<f32> mSLForceUnisonLen;
	/* 0x238 */ TParamRT<f32> mSLGuardLen;
	/* 0x24C */ TParamRT<f32> mSLTentacleStretch;
	/* 0x260 */ TParamRT<f32> mSLBeakStretch;
	/* 0x274 */ TParamRT<f32> mSLEyeDamageRadius;
	/* 0x288 */ TParamRT<f32> mSLEyeDamageHeight;
	/* 0x29C */ TParamRT<f32> mSLShootRadius;
	/* 0x2B0 */ TParamRT<s32> mSLBlurJoint;
	/* 0x2C4 */ TParamRT<f32> mSLBlurScale;
	/* 0x2D8 */ TParamRT<f32> mSLColumnScale;
	/* 0x2EC */ TParamRT<f32> mSLSightAngle;
	/* 0x300 */ TParamRT<s32> mSLAmputeeWait;
};

class TBGBeakHit : public TTakeActor {
public:
	TBGBeakHit(TBossGesso* owner, const char* name = "クチバシ（つかみ）");

	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual MtxPtr getTakingMtx();
	virtual bool moveRequest(const JGeometry::TVec3<f32>&);

public:
	/* 0x70 */ TBossGesso* mOwner;
	/* 0x74 */ TMtx34f unk74;
	/* 0xA4 */ JGeometry::TVec3<f32> unkA4;
};

class TBGEyeHit : public THitActor {
public:
	TBGEyeHit(TBossGesso* owner, int, const char* name = "目");

	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor*, u32);

public:
	/* 0x68 */ TBossGesso* mOwner;
	/* 0x6C */ u32 unk6C;
};

class TBGBodyHit : public THitActor {
public:
	TBGBodyHit(TBossGesso* owner, int, const char* name = "胴体");

	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor*, u32);

public:
	/* 0x68 */ TBossGesso* mOwner;
	/* 0x6C */ u32 unk6C;
};

class TBossGessoMtxCalc : public M3UMtxCalcSIAnmBlendQuat {
public:
	TBossGessoMtxCalc(TBossGesso*);

	void joinAnm(int);
	void setAnm(int);
	void calc(u16);

public:
	/* 0x64 */ TBossGesso* mOwner;
};

class TBGBinder : public TBinder {
public:
	TBGBinder();
	virtual void bind(TLiveActor*);
};

class TBGCork {
public:
	TBGCork(TBossGesso* owner);

	void crush();
	void perform(u32, JDrama::TGraphics*);

public:
	/* 0x0 */ TBossGesso* mOwner;
	/* 0x4 */ MActor* unk4;
	/* 0x8 */ MActor* unk8;
	/* 0xC */ int unkC;
};

class TBossGesso : public TSpineEnemy {
public:
	TBossGesso(const char*);

	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void moveObject();
	virtual const char** getBasNameTable() const;
	virtual void reset();

	void rumblePad(int, const JGeometry::TVec3<f32>&);
	void definiteRumble();
	void continuousRumble();
	f32 lenFromToeToMario();
	void showMessage(u32);
	void checkTakeMsg();
	void changeBck(int);
	bool inSightAngle(f32);
	f32 inSight();
	BOOL is2ndFightNow() const;
	void stopIfRoll();
	void changeAttackMode(int new_mode);
	void gotTentacleDamage();
	void gotEyeDamage();
	void gotBeakDamage();
	void changeAllTentacleState(int);
	void forceAllTentacleState(int);
	void startPollute();
	void stopPollute();
	void launchPolDrop();
	void setEyeDamageBtp(int);
	BOOL tentacleHeld() const;
	void tentacleAttack();
	BOOL beakHeld() const;
	void tentacleWait();
	void doAttackSingle();
	void doAttackDouble();
	void doAttackSkipRope();
	void doAttackUnison();
	void doAttackShoot();
	void doAttackGuard();
	void doAttackRoll();
	void performInContainer(u32, JDrama::TGraphics*);

	// fabricated
	TBossGessoParams* getSaveParam() const
	{
		return (TBossGessoParams*)getSaveParam();
	}

	// fabricated
	TNerveBase<TLiveActor>* getLatestNerve()
	{
		return mSpine->getLatestNerve();
	}

	// fabricated
	enum { TENTACLE_NUM = 4 };

	int getAttackMode() const { return mAttackMode; }

	// fabricated
	f32 getAttackSpeed() const
	{
		if (mAttackMode == 2)
			return getSaveParam()->mSLUnisonAttackSpeed.get();

		if (mAttackMode == 1)
			return getSaveParam()->mSLDoubleAttackSpeed.get();

		if (mAttackMode == 4)
			return getSaveParam()->mSLSkipRopeAttackSpeed.get();

		return 1.0f;
	}

public:
	/* 0x150 */ TBGTentacle* unk150[TENTACLE_NUM];
	/* 0x160 */ TBGBeakHit* mBeak;
	/* 0x164 */ TBossGessoMtxCalc* unk164;
	/* 0x168 */ int mAttackMode;
	/* 0x16C */ int unk16C;
	/* 0x170 */ TBGEyeHit* unk170;
	/* 0x174 */ TBGEyeHit* unk174;
	/* 0x178 */ MActor* unk178;
	/* 0x17C */ u8 unk17C;
	/* 0x180 */ TBGPolDrop* unk180;
	/* 0x184 */ TBGBodyHit* unk184;
	/* 0x188 */ f32 unk188;
	/* 0x18C */ TBGCork* unk18C;
	/* 0x190 */ J3DGXColor unk190;
	/* 0x194 */ u8 unk194;
	/* 0x195 */ s8 unk195;
	/* 0x196 */ s8 unk196;
	/* 0x198 */ u32 unk198;
	/* 0x19C */ u32 unk19C;
	/* 0x1A0 */ u8 unk1A0;
	/* 0x1A1 */ s8 unk1A1;
	/* 0x1A4 */ f32 unk1A4;
	/* 0x1A8 */ int unk1A8;
	/* 0x1AC */ s16 unk1AC;
	/* 0x1AE */ s16 unk1AE;
};

class TBossGessoManager : public TEnemyManager {
public:
	TBossGessoManager(const char*);

	virtual void load(JSUMemoryInputStream&);

	void createModelData();
	void initJParticle();
};

DECLARE_NERVE(TNerveBGWait, TLiveActor);
DECLARE_NERVE(TNerveBGEyeDamage, TLiveActor);
DECLARE_NERVE(TNerveBGBeakDamage, TLiveActor);
DECLARE_NERVE(TNerveBGTentacleDamage, TLiveActor);
DECLARE_NERVE(TNerveBGTug, TLiveActor);
DECLARE_NERVE(TNerveBGDie, TLiveActor);
DECLARE_NERVE(TNerveBGPollute, TLiveActor);
DECLARE_NERVE(TNerveBGPolDrop, TLiveActor);
DECLARE_NERVE(TNerveBGRoll, TLiveActor);

#endif
