#ifndef ENEMY_HANASAMBOOBJ_HPP
#define ENEMY_HANASAMBOOBJ_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Enemy/SmallEnemy.hpp>
#include <Enemy/WalkerEnemy.hpp>
#include <JSystem/JDrama/JDRViewObj.hpp>
#include <MoveBG/MapObjBase.hpp>
#include <Strategic/HitActor.hpp>

class SDLModel;
class SDLModelData;
class J3DMaterialTable;
class TMBindShadowBody;
class THanaSambo;
class TSamboFlower;
class TSamboFlowerManager;

// Names are the ones PARAM_INIT stringified into .rodata.
class TSamboHeadSaveLoadParams : public TWalkerEnemyParams {
public:
	TSamboHeadSaveLoadParams(const char* prm);

	/* 0x32C */ TParamRT<f32> mSLAppearDist;
	/* 0x340 */ TParamRT<f32> mSLHideDist;
	/* 0x354 */ TParamRT<f32> mSLMoveDist;
	/* 0x368 */ TParamRT<f32> mSLMoveGravity;
	/* 0x37C */ TParamRT<f32> mSLJumpSp;
	/* 0x390 */ TParamRT<s32> mSLJumpPrepareTime;
	/* 0x3A4 */ TParamRT<f32> mSLHitJumpSpXZ;
	/* 0x3B8 */ TParamRT<f32> mSLHitJumpSpY;
	/* 0x3CC */ TParamRT<f32> mSLHitJumpGravity;
	/* 0x3E0 */ TParamRT<f32> mSLHitJumpSpRateXZ;
	/* 0x3F4 */ TParamRT<f32> mSLHitJumpSpRateY;
	/* 0x408 */ TParamRT<f32> mSLJumpAngY;
};

class THanaSamboSaveLoadParams : public TSmallEnemyParams {
public:
	THanaSamboSaveLoadParams(const char* prm);

	/* 0x2D4 */ TParamRT<f32> mSLAttackDist;
	/* 0x2E8 */ TParamRT<s32> mSLAttackInterval;
	/* 0x2FC */ TParamRT<f32> mSLAppearDist;
	/* 0x310 */ TParamRT<f32> mSLHideDist;
	/* 0x324 */ TParamRT<s32> mSLAttackingTime;
	/* 0x338 */ TParamRT<f32> mSLHeadAttackRadius;
	/* 0x34C */ TParamRT<f32> mSLHeadAttackHeight;
	/* 0x360 */ TParamRT<f32> mSLHeadDamageRadius;
	/* 0x374 */ TParamRT<f32> mSLHeadDamageHeight;
};

class TSamboFlowerSaveLoadParams : public TSpineEnemyParams {
public:
	TSamboFlowerSaveLoadParams(const char* prm);

	/* 0xA8 */ TParamRT<f32> mSLLeafVelocityXZ;
	/* 0xBC */ TParamRT<f32> mSLLeafVelocityY;
	/* 0xD0 */ TParamRT<f32> mSLLeafGravity;
	/* 0xE4 */ TParamRT<f32> mSLBudDist;
	/* 0xF8 */ TParamRT<s32> mSLBloomTimer;
	/* 0x10C */ TParamRT<f32> mSLCoinCircleR;
	/* 0x120 */ TParamRT<f32> mSLCoinVelocityXZ;
	/* 0x134 */ TParamRT<f32> mSLCoinVelocityY;
	/* 0x148 */ TParamRT<f32> mSLSeedShootRange;
	/* 0x15C */ TParamRT<s32> mSLSeedShootInterval;
	/* 0x170 */ TParamRT<f32> mSLSeedGravity;
	/* 0x184 */ TParamRT<f32> mSLSeedSpeedXZ;
	/* 0x198 */ TParamRT<f32> mSLSeedSpeedY;
};

// The flower a Sambo leaves behind while it hides. Spraying it opens it up
// and, once every flower of a coin unit is open, spawns the coins.
class TSamboFlower : public TSpineEnemy {
public:
	// The default argument is what keeps this out of line: the map lists
	// it as a weak symbol called from createEnemyInstance and duplicated in
	// MarNameRefGen_Enemy, and spelling the name at the call site inlines it.
	TSamboFlower(const char* name = "サンボフラワー")
	    : TSpineEnemy(name)
	{
		mIsBloomed     = false;
		mBloomTimer    = 0;
		mCoinUnitIndex = -1;
		mCoinIndex     = -1;
		mCanClose      = false;
		mBloomCounter  = nullptr;
		mCoin          = nullptr;
	}

	virtual ~TSamboFlower() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void init(TLiveManager*);
	virtual void control() { }
	virtual void moveObject();
	virtual void drawObject(JDrama::TGraphics*);
	virtual void reset();
	virtual void setMActorAndKeeper();

	void hide();
	bool isBloomEnd();
	void bloom();

	// fabricated
	TSamboFlowerSaveLoadParams* getSaveParams() const
	{
		return (TSamboFlowerSaveLoadParams*)getSaveParam();
	}

	/* 0x150 */ u8 mIsBloomed;
	/* 0x154 */ s32 mBloomTimer;
	/* 0x158 */ s32 mCoinUnitIndex;
	/* 0x15C */ s32 mCoinIndex;
	/* 0x160 */ u8 mCanClose;
	/* 0x164 */ s32* mBloomCounter;
	/* 0x168 */ TMapObjBase* mCoin;
	/* 0x16C */ TSamboFlowerSaveLoadParams* mSaveParams;
};

// A ring of flowers sharing one coin item.
class TSamboFlowerCoinUnit {
public:
	TSamboFlowerCoinUnit(int capacity);

	void add(TSamboFlower*);
	void checkGenCoin();

	/* 0x0 */ TSamboFlower** mFlowers;
	/* 0x4 */ JGeometry::TVec3<f32> mCenter;
	/* 0x10 */ int mNum;
	/* 0x14 */ int mCapacity;
	/* 0x18 */ TMapObjBase* mCoinItem;
	/* 0x1C */ s32 mClosedCount;
};

// A petal thrown off when a Sambo pops out of its flower.
class TSamboLeaf : public JDrama::TViewObj {
public:
	TSamboLeaf(TSamboFlowerManager*, SDLModelData*, const char*);

	virtual ~TSamboLeaf() { }
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	void generate(JGeometry::TVec3<f32>&);

	/* 0x10 */ SDLModel* mModel;
	/* 0x14 */ JGeometry::TVec3<f32> mPosition;
	/* 0x20 */ JGeometry::TVec3<f32> mRotation;
	/* 0x2C */ JGeometry::TVec3<f32> mScaling;
	/* 0x38 */ JGeometry::TVec3<f32> mVelocity;
	/* 0x44 */ u8 mIsActive;
	/* 0x48 */ TSamboFlowerManager* mManager;
};

class TSamboFlowerManager : public TEnemyManager {
public:
	TSamboFlowerManager(const char* name);

	virtual ~TSamboFlowerManager() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual TSpineEnemy* createEnemyInstance();
	virtual void createModelData();
	virtual void dropLeaf(JGeometry::TVec3<f32>&, JGeometry::TVec3<f32>&);

	// fabricated
	TSamboFlowerSaveLoadParams* getSaveParams() const
	{
		return (TSamboFlowerSaveLoadParams*)unk38;
	}

	/* 0x54 */ TSamboFlowerCoinUnit** mCoinUnits;
	/* 0x58 */ int mCoinUnitNum;
	/* 0x5C */ int unk5C;
	/* 0x60 */ TSamboLeaf** mLeaves;
	/* 0x64 */ J3DMaterialTable* mMaterialTable;
};

// The Sambo that pops out to chase Mario.
class TSamboHead : public TWalkerEnemy {
public:
	TSamboHead(const char* name);

	virtual ~TSamboHead() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void kill();
	virtual f32 getGravityY() const;
	virtual const char** getBasNameTable() const;
	virtual void reset();
	virtual void genEventCoin();
	virtual void behaveToWater(THitActor*);
	virtual void setDeadAnm();
	virtual void attackToMario();
	virtual void setMActorAndKeeper();
	virtual void setAfterDeadEffect();

	bool isUseCallBack();
	void setCrashAnm();
	void initFlower();

	// fabricated
	TSamboHeadSaveLoadParams* getSaveParams() const
	{
		return (TSamboHeadSaveLoadParams*)getSaveParam();
	}

	static u8 mBodyJntIndex;

	/* 0x194 */ TSamboHeadSaveLoadParams* mSaveParams;
	/* 0x198 */ TSamboFlower* mFlower;
	/* 0x19C */ s32 mJumpTimer;
	/* 0x1A0 */ JGeometry::TVec3<f32> mHitVelocity;
	/* 0x1AC */ f32 mRollAngle;
	/* 0x1B0 */ u8 mIsTurnedBack;
};

class TSamboHeadManager : public TSmallEnemyManager {
public:
	TSamboHeadManager(const char* name);

	virtual ~TSamboHeadManager() { }
	virtual void load(JSUMemoryInputStream&);
	virtual TSpineEnemy* createEnemyInstance();
	virtual void createModelData();
};

// The hit box on the Sambo's flower head; hands every message to its owner.
class THanaSamboHead : public THitActor {
public:
	THanaSamboHead(const char* name)
	    : THitActor(name)
	{
	}

	virtual ~THanaSamboHead() { }
	virtual BOOL receiveMessage(THitActor* sender, u32 message);

	void kill();
	void checkHit();

	/* 0x68 */ THanaSambo* mOwner;
};

// The Sambo that stays put and shoots pollen.
class THanaSambo : public TSmallEnemy {
public:
	THanaSambo(const char* name);

	virtual ~THanaSambo() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual void init(TLiveManager*);
	virtual void moveObject();
	virtual void drawObject(JDrama::TGraphics*);
	virtual void kill();
	virtual const char** getBasNameTable() const;
	virtual void reset();
	virtual void behaveToWater(THitActor*) { }
	virtual void setDeadAnm();
	virtual void setWaitAnm();
	virtual void setMActorAndKeeper();
	virtual bool isHitValid(u32 message)
	{
		if (message == HIT_MESSAGE_UNKB) {
			onLiveFlag(LIVE_FLAG_HIDDEN);
			return true;
		}
		return false;
	}
	virtual bool isCollidMove(THitActor*) { return false; }

	void initFlower();
	void createPollen();
	void waterDamage();
	void setAttackAnm();

	// fabricated
	THanaSamboSaveLoadParams* getSaveParams() const
	{
		return (THanaSamboSaveLoadParams*)getSaveParam();
	}

	static u8 mHeadJntIndex;
	static u8 mPollenJntIndex;

	/* 0x194 */ THanaSamboHead* mHead;
	/* 0x198 */ THanaSamboSaveLoadParams* mSaveParams;
	/* 0x19C */ JGeometry::TVec3<f32> mInitialPos;
	/* 0x1A8 */ TSamboFlower* mFlower;
	/* 0x1AC */ TMBindShadowBody* mShadow;
	/* 0x1B0 */ u8 mIsAttacking;
	/* 0x1B4 */ JGeometry::TVec3<f32> mDieEffectPos[4];
};

class THanaSamboManager : public TSmallEnemyManager {
public:
	THanaSamboManager(const char* name);

	virtual ~THanaSamboManager() { }
	virtual void load(JSUMemoryInputStream&);
	virtual TSpineEnemy* createEnemyInstance();
	virtual void createModelData();
};

extern TSamboHead* gpCurSamboHead;

#endif
