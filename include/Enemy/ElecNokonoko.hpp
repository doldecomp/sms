#ifndef ENEMY_ELECNOKONOKO_HPP
#define ENEMY_ELECNOKONOKO_HPP

#include <Enemy/EnemyAttachment.hpp>
#include <Enemy/WalkerEnemy.hpp>
#include <Strategic/Nerve.hpp>

class TLiveActor;
class TElecNokonoko;
class J3DMaterialTable;

// Names and defaults are the ones PARAM_INIT stringified into .rodata and the
// constants the constructor stores into each TParamRT.
class TElecNokonokoSaveLoadParams : public TWalkerEnemyParams {
public:
	TElecNokonokoSaveLoadParams(const char* prm);

	s32 getSLReadyTime() const { return mSLReadyTime.get(); }
	f32 getSLCarapaceGravity() const { return mSLCarapaceGravity.get(); }
	f32 getSLCarapaceSpeed() const { return mSLCarapaceSpeed.get(); }
	f32 getSLCarapaceTurnSpeed() const { return mSLCarapaceTurnSpeed.get(); }
	f32 getSLCarapaceSpinSpeed() const { return mSLCarapaceSpinSpeed.get(); }
	f32 getSLCarapaceShootRange() const { return mSLCarapaceShootRange.get(); }
	f32 getSLCarapaceFlyDist() const { return mSLCarapaceFlyDist.get(); }

	/* 0x32C */ TParamRT<s32> mSLReadyTime;
	/* 0x340 */ TParamRT<f32> mSLCarapaceGravity;
	/* 0x354 */ TParamRT<f32> mSLCarapaceSpeed;
	/* 0x368 */ TParamRT<f32> mSLCarapaceTurnSpeed;
	/* 0x37C */ TParamRT<f32> mSLCarapaceSpinSpeed;
	/* 0x390 */ TParamRT<f32> mSLCarapaceShootRange;
	/* 0x3A4 */ TParamRT<f32> mSLCarapaceFlyDist;
};

// The electric shell the koopa throws. It is an attachment, so the koopa owns
// it and drives it through its own spine: it spins about Y while it flies, is
// reflected off walls and off anything that hits it, and eventually comes
// home to be picked up again.
class TElecCarapace : public TEnemyAttachment {
public:
	TElecCarapace(const char* name);

	// Declared in vtable order; shoot() is a new slot past TEnemyAttachment.
	virtual ~TElecCarapace() { }
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void calcRootMatrix();
	virtual void bind();
	virtual void kill();
	virtual f32 getPhaseShift() const
	{
		return mSpinReverse ? 0.0f : 180.0f;
	}
	virtual void loadInit(TSpineEnemy* host, const char* model);
	virtual void appear();
	virtual void rebirth() { }
	virtual void sendMessage();
	virtual void behaveToHitGround();
	virtual void behaveToHitWall(const TBGCheckData* wall);
	virtual void setBehavior();
	virtual void recoverScale() { }
	virtual f32 getNowGravity();
	virtual void shoot();

	void reflect(THitActor* other);
	void setZigParameter();
	void move();
	bool isMove();

	// fabricated
	TElecNokonoko* getNokonoko() { return mNokonoko; }

	/* 0x16C */ TElecNokonoko* mNokonoko;
	// Whatever last bounced the shell, so one collision only reflects once.
	/* 0x170 */ THitActor* mReflector;
	// Which way round the shell spins; getPhaseShift turns it into the half
	// cycle the two shells of a pair are apart.
	/* 0x174 */ u8 mSpinReverse;
	// Set while the shell must fly straight instead of zigzagging.
	/* 0x175 */ u8 mStraight;
	/* 0x176 */ u8 mFlying;
	/* 0x178 */ f32 mZigzagCycle;
	/* 0x17C */ f32 mZigzagAngle;
	// Counts 1..5 after a wall bounce so the shell cannot bounce again at once.
	/* 0x180 */ int mReflectTimer;
	/* 0x184 */ u8 mLanded;
	/* 0x188 */ f32 mSpinAngle;
	// Per-frame step home, a sixty-fourth of the distance to the koopa.
	/* 0x18C */ JGeometry::TVec3<f32> mReturnStep;
	/* 0x198 */ f32 unk198;
};

// 電気ノコノコ, the Noki Bay koopa that throws its electrified shell at Mario
// and walks over to pick it up again.
class TElecNokonoko : public TWalkerEnemy {
public:
	TElecNokonoko(const char* name);

	// Declared in vtable order; rest() is a new slot past TWalkerEnemy.
	virtual ~TElecNokonoko() { }
	virtual void load(JSUMemoryInputStream&);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void moveObject();
	virtual const char** getBasNameTable() const;
	virtual void genRandomItem();
	virtual void behaveToWater(THitActor*);
	virtual void setWalkAnm();
	virtual void setDeadAnm();
	virtual void setMeltAnm();
	virtual void setWaitAnm();
	virtual void setRunAnm();
	virtual void attackToMario();
	virtual void setMActorAndKeeper();
	virtual void sendAttackMsgToMario();
	virtual void behaveToFindMario();
	virtual bool isResignationAttack();
	virtual void rest();

	void recoverCarapace();
	bool isDeadByThunder();
	void forceCatchReady();
	bool isCatchReady();
	bool isShootReady();
	void shootIn();
	void catchIn();

	// fabricated
	TElecCarapace* getCarapace() { return mCarapace; }
	TElecNokonokoSaveLoadParams* getSaveParams() const { return mSaveParams; }
	// The explicit if/return form is what the retail object materialises at
	// every call site; `return mHasCarapace == 0;` folds into a bare compare.
	bool hasCarapace() const
	{
		if (mHasCarapace == 0)
			return true;
		return false;
	}

	// When cleared the shell passes through everything instead of bouncing.
	static u8 mReflectSw;
	// UNUSED in the map, so nothing in the retail object reads it.
	static u8 mCarapaceJntIndex;

	/* 0x194 */ TElecCarapace* mCarapace;
	/* 0x198 */ int unk198;
	// Counts down from a random part of mSLReadyTime before the koopa is
	// willing to throw again.
	/* 0x19C */ int mReadyTimer;
	/* 0x1A0 */ TElecNokonokoSaveLoadParams* mSaveParams;
	// 0 while the koopa still wears its shell, 1 once it has been thrown.
	/* 0x1A4 */ int mHasCarapace;
	/* 0x1A8 */ JGeometry::TVec3<f32> mEffectPos;
};

class TElecNokonokoManager : public TSmallEnemyManager {
public:
	TElecNokonokoManager(const char* name);

	// Declared in vtable order; initSetEnemies overrides TSmallEnemyManager's.
	virtual ~TElecNokonokoManager() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();
	virtual void clipEnemies(JDrama::TGraphics*);
	virtual void initSetEnemies();

	// fabricated
	TElecNokonokoSaveLoadParams* getSaveParams() const
	{
		return (TElecNokonokoSaveLoadParams*)unk38;
	}
	TElecNokonoko* getObj(int i) { return (TElecNokonoko*)unk18[i]; }
	J3DMaterialTable* getMaterialTable() { return mMaterialTable; }

	/* 0x60 */ J3DMaterialTable* mMaterialTable;
};

DECLARE_NERVE(TNerveElecCarapaceMove, TLiveActor)
DECLARE_NERVE(TNerveElecCarapaceReturn, TLiveActor)
DECLARE_NERVE(TNerveElecCarapaceWait, TLiveActor)
DECLARE_NERVE(TNerveElecNokonokoAttack, TLiveActor)
DECLARE_NERVE(TNerveElecNokonokoCollect, TLiveActor)
DECLARE_NERVE(TNerveElecNokonokoFreeze, TLiveActor)
DECLARE_NERVE(TNerveElecNokonokoRebirth, TLiveActor)
DECLARE_NERVE(TNerveElecNokonokoShoot, TLiveActor)
DECLARE_NERVE(TNerveElecNokonokoTurn, TLiveActor)

#endif
