#ifndef ENEMY_BOSSTELESAOBJ_HPP
#define ENEMY_BOSSTELESAOBJ_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Enemy/SmallEnemy.hpp>
#include <Enemy/WalkerEnemy.hpp>
#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/JGeometry.hpp>
#include <MoveBG/MapObjBase.hpp>
#include <MoveBG/MapObjSirena.hpp>
#include <Strategic/HitActor.hpp>
#include <Strategic/Spine.hpp>

class TBossTelesa;
class TCoin;
class TSharedParts;
class TTelesaManager;

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

class TBossTelesaManager : public TEnemyManager {
public:
	TBossTelesaManager(const char* name = "ボステレサマネージャー");

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();
	virtual void clipEnemies(JDrama::TGraphics*) { }
};

/// The boss' body collision: everything that is not the tongue.
class TBossTelesaBody : public THitActor {
public:
	TBossTelesaBody(const char* name = "ボステレサ体コリジョン")
	    : THitActor(name)
	    , unk6C(false)
	{
	}

	virtual ~TBossTelesaBody() { }
	virtual BOOL receiveMessage(THitActor* sender, u32 message);

	bool checkHit();

public:
	/* 0x68 */ TBossTelesa* mOwner;
	/* 0x6C */ bool unk6C;
};

/// The tongue collision: what Mario can be swallowed by.
class TBossTelesaTongue : public THitActor {
public:
	TBossTelesaTongue(const char* name = "ボステレサ舌コリジョン")
	    : THitActor(name)
	{
	}

	virtual ~TBossTelesaTongue() { }
	virtual BOOL receiveMessage(THitActor* sender, u32 message);

	bool checkHit();

public:
	/* 0x68 */ TBossTelesa* mOwner;
};

/// The wide collision that kills the small enemies the boss spits out.
class TBossTelesaKillSmallEnemy : public THitActor {
public:
	TBossTelesaKillSmallEnemy(const char* name = "ボステレサ雑魚敵死コリジョン")
	    : THitActor(name)
	    , mOwner(nullptr)
	    , unk6C(false)
	{
	}

	virtual ~TBossTelesaKillSmallEnemy() { }

	void checkHit();

public:
	/* 0x68 */ TBossTelesa* mOwner;
	/* 0x6C */ bool unk6C;
};

/// The slot machine the boss plays with. Three drums, driven by TSlotDrum.
class TTelesaSlot : public TSlotDrum {
public:
	// The map has this constructor weak at 0x98 and emitted in
	// MarNameRefGen_MapObj.o, so it is an in-class body; its stores name the
	// members it touches and, via __construct_array(0x1ac, TVec3<f>::TVec3,
	// 0xc, 4), prove unk1AC is four vectors rather than a Mtx.
	TTelesaSlot(const char* name = "btelesaSlot")
	    : TSlotDrum(name)
	    , unk19B(false)
	    , mStopRequested(true)
	    , mForcedResult(2)
	    , unk1E0(0)
	{
		mForceHit[0] = false;
		mForceHit[1] = false;
		mForceHit[2] = false;
		// Dead 16-byte carrier: the ROM's copy of this body reserves 16 low
		// bytes nothing in it references (frame 0x28 against 0x18), and an
		// uninitialised non-trivial class local is the only zero-instruction
		// lever for that -- a `TVec3` reserves 16 rather than its 12 bytes.
		// The drum vectors at unk1AC are the plausible original.
		JGeometry::TVec3<f32> drumPos;
	}

	virtual ~TTelesaSlot() { }
	virtual void calcRootMatrix();
	virtual void moveObject();
	virtual u32 touchWater(THitActor*);
	virtual void initMapObj();
	virtual void initNeonMatColor() { }

	void entryObjCollision();
	void calcObjCollision();
	int getResultFromAng(f32);
	int getForcastResult(int);
	int getDrumResult(int);
	int getSlotResult();
	bool isRollDrum();
	void forceStopSlot(int);
	void moveStart();
	void randomReset();

public:
	/* 0x198 */ bool mIsRolling[3];
	/* 0x19B */ bool unk19B;
	/* 0x19C */ bool mStopRequested;
	/* 0x1A0 */ TBossTelesa* mOwner;
	/* 0x1A4 */ int mForcedResult;
	/* 0x1A8 */ bool mForceHit[3];
	/* 0x1AC */ JGeometry::TVec3<f32> unk1AC[4];
	/* 0x1DC */ TMapCollisionMove* unk1DC;
	/* 0x1E0 */ u8 unk1E0;
	/* 0x1E4 */ f32 mRollSp[3];
};

class TBossTelesa : public TSpineEnemy {
public:
	TBossTelesa(const char* name = "ボステレサ");

	virtual ~TBossTelesa() { }
	virtual void loadAfter();
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual MtxPtr getTakingMtx();
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void moveObject();
	virtual void kill();
	virtual const char** getBasNameTable() const;
	virtual void reset();

	void fanfale();
	void forceHide();
	bool isForceRestart();
	void rollRouletteCircle();
	void forceAllItemKill();
	bool checkAllItemDead();
	void checkSlot();
	void fruitCollisionOn();
	void generateSlotItem();
	bool checkSlotResult();
	bool slotStop();
	void slotStart();
	void rouletteStart();
	bool isInDamage();
	void setBckAnm(int);
	void genAttacker();
	void offAllCollision();
	void onAllCollision();
	void flashItem(int);
	void openWaterPlace();
	bool slotFall();
	bool rouletteFall();
	void tongueHitWater();
	void damageRecover();
	void setSpicy(TLiveActor*);
	void checkHitObject(THitActor*);
	BOOL checkMessage(THitActor*, u32);
	void prepareGenerate();

	// fabricated
	TBossTelesaSaveLoadParams* getSaveParam2() const { return mParams; }

	static f32 mEnemyGenRate;
	static f32 mItemGenRate;
	static u8 mNormalAlpha;
	static f32 mBaseHoseiPosY;
	static f32 mRouletteUpRate;
	static s32 mTelesaGenerateInterval;
	static f32 mCameraMoveLimit;
	static f32 mCameraMoveSp;
	static bool mGenerateTelesa;
	static bool mGenerateBubble;
	static int mEnemyTestType;

public:
	/* 0x150 */ u8 unk150;
	/* 0x154 */ int unk154;
	/* 0x158 */ int unk158;
	/* 0x15C */ TBossTelesaSaveLoadParams* mParams;
	/* 0x160 */ int unk160;
	/* 0x164 */ int unk164;
	/* 0x168 */ f32 unk168;
	/* 0x16C */ TBossTelesaBody* mBody;
	/* 0x170 */ TBossTelesaTongue* mTongue;
	/* 0x174 */ TBossTelesaKillSmallEnemy* mKillSmallEnemy;
	/* 0x178 */ TRoulette* mRoulettes[3];
	/* 0x184 */ TTelesaSlot* mSlot;
	/* 0x188 */ TSharedParts* mSlotFrame;
	/* 0x18C */ bool unk18C;
	// TODO: never read or written anywhere in the TU. Twelve bytes between a
	// bool and a position, so most likely a leftover vector.
	/* 0x190 */ JGeometry::TVec3<f32> unk190;
	/* 0x19C */ JGeometry::TVec3<f32> mSoundPos;
	/* 0x1A8 */ int unk1A8;
	/* 0x1AC */ TLiveActor* mSlotItems[50];
	/* 0x274 */ int mSlotItemNum;
	/* 0x278 */ JGeometry::SMatrix34C<f32> unk278;
	/* 0x2A8 */ TMapObjBase* mFruits[20];
	/* 0x2F8 */ TMapObjBase* mPeppers[10];
	/* 0x320 */ TCoin* mCoins[10];
	/* 0x348 */ GXColor unk348;
	/* 0x34C */ GXColor unk34C;
	/* 0x350 */ bool unk350;
	/* 0x354 */ TTelesaManager* mTelesaManager;
	/* 0x358 */ s16 mMarioHP;
	/* 0x35A */ bool unk35A;
	/* 0x35B */ bool unk35B;
	/* 0x35C */ int unk35C;
	/* 0x360 */ f32 unk360;
	/* 0x364 */ f32 unk364;
	/* 0x368 */ int unk368;
	/* 0x36C */ int unk36C;
	/* 0x370 */ u8 unk370;
	/* 0x374 */ JGeometry::TVec3<f32> unk374;
	/* 0x380 */ int unk380;
	/* 0x384 */ bool unk384;
	/* 0x388 */ int unk388;
};

class TBubbleSaveLoadParams : public TWalkerEnemyParams {
public:
	TBubbleSaveLoadParams(const char* path);

	/* 0x32C */ TParamRT<s32> mSLLiveTime;
	/* 0x340 */ TParamRT<s32> mSLNumDivision;
	/* 0x354 */ TParamRT<f32> mSLMaxScale;
	/* 0x368 */ TParamRT<f32> mSLAddPosBase;
	/* 0x37C */ TParamRT<f32> mSLRateExpand;
	/* 0x390 */ TParamRT<f32> mSLDeadHeight;
};

class TBubbleManager : public TSmallEnemyManager {
public:
	TBubbleManager(const char* name = "バブルマネージャー");

	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
	virtual TSmallEnemy* createEnemyInstance();
};

/// A bubble the boss spits out, carrying an enemy or an item.
class TBubble : public TWalkerEnemy {
public:
	TBubble(const char* name = "バブル");

	virtual ~TBubble() { }
	virtual MtxPtr getTakingMtx();
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void kill();
	virtual f32 getGravityY() const;
	virtual const char** getBasNameTable() const;
	virtual void reset();
	virtual void behaveToWater(THitActor*);
	virtual void setDeadAnm();
	virtual void attackToMario();
	virtual void setAfterDeadEffect() { }

	void split();
	void appendEnemy();
	void appendItem();

	// fabricated
	TBubbleSaveLoadParams* getSaveParam2() const { return mParams; }

public:
	/* 0x194 */ TBubbleSaveLoadParams* mParams;
	/* 0x198 */ TLiveActor* mEnemyInside;
	/* 0x19C */ TPosition3f unk19C;
	/* 0x1CC */ f32 mFloatHeight;
	/* 0x1D0 */ bool mIsSplit;
	/* 0x1D1 */ bool mHasInitialVelocity;
	// Only ever cleared. When set, kill() would hand the captured enemy a
	// HIT_MESSAGE_THROWN instead of killing it outright.
	/* 0x1D2 */ bool unk1D2;
};

#endif
