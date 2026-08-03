#ifndef ENEMY_BOSS_TELESA_HPP
#define ENEMY_BOSS_TELESA_HPP

#include <Enemy/EnemyManager.hpp>
#include <Enemy/SmallEnemy.hpp>
#include <Enemy/Telesa.hpp>
#include <MoveBG/MapObjSirena.hpp>
#include <Strategic/Nerve.hpp>

class TBossTelesa;
class TBubble;

/* The boss uses the same spine state objects as the regular Telesa, but has
 * its own state machine for the roulette, tongue and bubble attacks. */
DECLARE_NERVE(TNerveBossTelesaFallDemo, TLiveActor);
DECLARE_NERVE(TNerveBossTelesaFreeze, TLiveActor);
DECLARE_NERVE(TNerveBossTelesaPrepareSlot, TLiveActor);
DECLARE_NERVE(TNerveBossTelesaSpitSlotItem, TLiveActor);
DECLARE_NERVE(TNerveBossTelesaSlotStart, TLiveActor);
DECLARE_NERVE(TNerveBossTelesaAppear, TLiveActor);
DECLARE_NERVE(TNerveBossTelesaHideWait, TLiveActor);
DECLARE_NERVE(TNerveBossTelesaHide, TLiveActor);
DECLARE_NERVE(TNerveBossTelesaSpit, TLiveActor);
DECLARE_NERVE(TNerveBossTelesaDie, TLiveActor);
DECLARE_NERVE(TNerveBubbleSplit, TLiveActor);
DECLARE_NERVE(TNerveBubbleLive, TLiveActor);

class TBossTelesaSaveLoadParams : public TSpineEnemyParams {
public:
	TBossTelesaSaveLoadParams(const char* path);

	/* 0x0A8 */ TParamRT<s32> mSLDamageRadius;
	/* 0x0BC */ TParamRT<s32> mSLDamageHeight;
	/* 0x0D0 */ TParamRT<s32> mSLAttackRadius;
	/* 0x0E4 */ TParamRT<s32> mSLAttackHeight;
	/* 0x0F8 */ TParamRT<s32> mSLGenAttackerTime;
	/* 0x10C */ TParamRT<s32> mSLGenBubbleTime;
	/* 0x120 */ TParamRT<f32> mSLHitAngle;
	/* 0x134 */ TParamRT<s32> mSLNumGenBubble;
	/* 0x148 */ TParamRT<f32> mSL1stBubbleSp;
	/* 0x15C */ TParamRT<f32> mSLHideAreaRadius;
	/* 0x170 */ TParamRT<s32> mSLSlotItemNum;
	/* 0x184 */ TParamRT<s32> mSLSlotFruitNum;
	/* 0x198 */ TParamRT<f32> mSLSlotFirstHitCollectRate;
	/* 0x1AC */ TParamRT<f32> mSLSlotHitCollectRate;
	/* 0x1C0 */ TParamRT<f32> mSLTransYOffset;
	/* 0x1D4 */ TParamRT<s32> mSLStopSlotTime0;
	/* 0x1E8 */ TParamRT<s32> mSLStopSlotTime1;
	/* 0x1FC */ TParamRT<s32> mSLStopSlotTime2;
	/* 0x210 */ TParamRT<s32> mSLSpicyTime;
};

class TBubbleSaveLoadParams : public TSpineEnemyParams {
public:
	TBubbleSaveLoadParams(const char* path);

	/* 0x0A8 */ TParamRT<s32> mSLLiveTime;
	/* 0x0BC */ TParamRT<s32> mSLNumDivision;
	/* 0x0D0 */ TParamRT<f32> mSLMaxScale;
	/* 0x0E4 */ TParamRT<f32> mSLAddPosBase;
	/* 0x0F8 */ TParamRT<f32> mSLRateExpand;
	/* 0x10C */ TParamRT<f32> mSLDeadHeight;
};

/* TSlotDrum already owns the geometry, angle and neon material fields. */
class TTelesaSlot : public TSlotDrum {
public:
	TTelesaSlot(const char* name);

	virtual void calcRootMatrix();
	virtual void moveObject();
	virtual void initMapObj();
	virtual u32 touchWater(THitActor*);
	virtual void initNeonMatColor() { }

	int getForcastResult(int);
	int getResultFromAng(f32);
	int getSlotResult();
	u8 isRollDrum();
	void forceStopSlot(int);
	void moveStart();
	void randomReset();
	void entryObjCollision();
	void calcObjCollision();
	int getDrumResult(int);

	/* 0x198 */ u8 mRoll0;
	/* 0x199 */ u8 mRoll1;
	/* 0x19A */ u8 mRoll2;
	/* 0x19B */ s8 mRolling;
	/* 0x19C */ u8 mForceStop;
	/* 0x1A0 */ TBossTelesa* mOwner;
	/* 0x1A4 */ s32 mForceResult;
	/* 0x1A8 */ s8 mStop0;
	/* 0x1A9 */ s8 mStop1;
	/* 0x1AA */ s8 mStop2;
	/* 0x1DC */ TMapCollisionMove* mMapCollisionMove;
	/* 0x1E0 */ u8 mNeonState;
};

class TBossTelesa : public TSpineEnemy {
public:
	TBossTelesa(const char* name = "ボステレサ");

	virtual void loadAfter();
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void moveObject();
	virtual void kill();
	virtual const char** getBasNameTable() const;
	virtual void reset();

	void forceHide();
	void fanfale();
	bool isForceRestart();
	void rollRouletteCircle();
	void forceAllItemKill();
	bool checkAllItemDead();
	bool checkSlot();
	void fruitCollisionOn();
	void generateSlotItem();
	int checkSlotResult();
	void slotStop();
	void slotStart();
	bool isInDamage();
	void setBckAnm(int);
	void offAllCollision();
	void onAllCollision();
	void openWaterPlace();
	void tongueHitWater();
	BOOL checkMessage(THitActor*, u32);
	void prepareGenerate();
	void rouletteStart();
	void genAttacker();
	void flashItem(int);
	u8 slotFall();
	u8 rouletteFall();
	void damageRecover();
	void setSpicy(TLiveActor*);
	BOOL checkHitObject(THitActor*);
	MtxPtr getTakingMtx();

	static f32 mEnemyGenRate;
	static f32 mItemGenRate;
	static u8 mNormalAlpha;
	static f32 mBaseHoseiPosY;
	static f32 mRouletteUpRate;
	static s32 mTelesaGenerateInterval;
	static f32 mCameraMoveLimit;
	static f32 mCameraMoveSp;

	/* 0x150 */ s32 mBattleState;
	/* 0x154 */ s32 mBattleTimer;
	/* 0x158 */ s32 mBattlePhase;
	/* 0x15C */ TLiveActor* mMario;
	/* 0x160 */ s32 mCurrentItem;
	/* 0x164 */ s32 mCurrentFruit;
	/* 0x168 */ f32 mFallSpeed;
	/* 0x16C */ THitActor* mBodyCollision;
	/* 0x170 */ THitActor* mTongueCollision;
	/* 0x174 */ THitActor* mKillSmallEnemy;
	/* 0x178 */ void* mItemList;
	/* 0x17C */ void* mBubbleList;
	/* 0x180 */ TBossTelesaSaveLoadParams* mSaveParams;
	/* 0x184 */ TTelesaSlot* mSlot;
	/* 0x188 */ JDrama::TViewObj* mTakingActor;
	/* 0x18C */ u8 mIsSpicy;
	/* 0x19C */ f32 mDamageBlend;
	/* 0x1A0 */ f32 mDamageBlendSpeed;
	/* 0x1A4 */ f32 mDamageBlendTarget;
	/* 0x1A8 */ s32 mDamageTimer;
	/* 0x274 */ s32 mStateTimer;
	/* 0x348 */ u8 mItem0;
	/* 0x349 */ u8 mItem1;
	/* 0x34A */ u8 mItem2;
	/* 0x34B */ u8 mItem3;
	/* 0x34C */ u8 mFruit0;
	/* 0x34D */ u8 mFruit1;
	/* 0x34E */ u8 mFruit2;
	/* 0x34F */ u8 mAlpha;
	/* 0x350 */ u8 mDamage;
	/* 0x354 */ s32 mGenerateTimer;
	/* 0x358 */ s16 mMarioHP;
	/* 0x35A */ u8 mForceRestart;
	/* 0x35B */ u8 mInDamage;
	/* 0x35C */ s32 mRouletteTimer;
	/* 0x360 */ f32 mRoulettePosY;
	/* 0x364 */ f32 mRouletteSp;
	/* 0x368 */ s32 mSlotResult;
	/* 0x36C */ s32 mSlotItemCount;
	/* 0x370 */ u8 mSlotStop;
	/* 0x374 */ JGeometry::TVec3<f32> mEffectPos;
	/* 0x384 */ s8 mDead;
};

class TBossTelesaKillSmallEnemy : public THitActor {
public:
	TBossTelesaKillSmallEnemy(TBossTelesa* owner = nullptr)
	    : THitActor("ボステレサ小敵キラー"), mOwner(owner), mHit(false)
	{
	}

	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	void checkHit();

	/* 0x68 */ TBossTelesa* mOwner;
	/* 0x6C */ bool mHit;
};

class TBossTelesaTongue : public THitActor {
public:
	TBossTelesaTongue(TBossTelesa* owner = nullptr)
	    : THitActor("ボステレサ舌"), mOwner(owner)
	{
	}

	virtual BOOL receiveMessage(THitActor*, u32);
	void checkHit();
	TBossTelesa* mOwner;
};

class TBossTelesaBody : public THitActor {
public:
	TBossTelesaBody(TBossTelesa* owner = nullptr)
	    : THitActor("ボステレサ本体"), mOwner(owner)
	{
	}

	virtual BOOL receiveMessage(THitActor*, u32);
	void checkHit();
	TBossTelesa* mOwner;
};

class TBossTelesaManager : public TEnemyManager {
public:
	TBossTelesaManager(const char* name = "ボステレサマネージャー");

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();
	virtual void clipEnemies(JDrama::TGraphics*) { }
};

class TBubble : public TWalkerEnemy {
public:
	TBubble(const char* name = "バブル");

	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void behaveToWater(THitActor*);
	virtual void kill();
	virtual f32 getGravityY() const;
	virtual const char** getBasNameTable() const;
	virtual void reset();

	void appendEnemy();
	void appendItem();
	void setDeadAnm();
	void attackToMario();
	void split();
	void setAfterDeadEffect() { }
	MtxPtr getTakingMtx();

	/* 0x194 */ TBubbleSaveLoadParams* mSaveParams;
	/* 0x198 */ TSmallEnemy* mCarriedEnemy;
	/* 0x1CC */ f32 mHeight;
	/* 0x1D0 */ u8 mIsSplit;
	/* 0x1D1 */ u8 mIsMoving;
	/* 0x1D2 */ u8 mNoEffect;
};

class TBubbleManager : public TSmallEnemyManager {
public:
	TBubbleManager(const char* name = "バブルマネージャー");

	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
	virtual TSmallEnemy* createEnemyInstance();
};

#endif
