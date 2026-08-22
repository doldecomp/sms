#ifndef ENEMY_BOSS_TELESA_HPP
#define ENEMY_BOSS_TELESA_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Enemy/WalkerEnemy.hpp>
#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/JGeometry.hpp>
#include <MoveBG/MapObjSirena.hpp>
#include <Strategic/HitActor.hpp>
#include <dolphin/gx/GXStruct.h>

class TBossTelesaSaveLoadParams : public TSpineEnemyParams {
public:
	TBossTelesaSaveLoadParams(const char*);

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

class TBossTelesa;
class TBossTelesaBody;
class TBossTelesaTongue;
class TBossTelesaKillSmallEnemy;

class TTelesaSlot : public TSlotDrum {
public:
	TTelesaSlot(const char* name)
	    : TSlotDrum(name)
	    , unk1A0(nullptr)
	    , unk1A4(0)
	{
		unk198[0] = false;
		unk198[1] = false;
		unk198[2] = false;
		unk19B    = 0;
		unk19C    = 0;
		unk1A8[0] = false;
		unk1A8[1] = false;
		unk1A8[2] = false;
	}

	virtual void calcRootMatrix();
	virtual void initMapObj();
	virtual void moveObject();
	virtual u32 touchWater(THitActor*);
	virtual void initNeonMatColor() { }

	void randomReset();
	void moveStart();
	void forceStopSlot(int);
	bool isRollDrum();
	int getSlotResult();
	int getForcastResult(int);
	int getResultFromAng(f32);
	f32* getDrumSpeeds() const { return unk138; }
	void entryObjCollision();
	void calcObjCollision();
	int getDrumResult(int);

	/* 0x198 */ bool unk198[3];
	/* 0x19B */ u8 unk19B;
	/* 0x19C */ u8 unk19C;
	/* 0x19D */ u8 unk19D[3];
	/* 0x1A0 */ void* unk1A0;
	/* 0x1A4 */ s32 unk1A4;
	/* 0x1A8 */ bool unk1A8[3];
	/* 0x1AB */ u8 unk1AB[0x31];
	/* 0x1DC */ TMapCollisionMove* unk1DC;
	/* 0x1E0 */ u8 unk1E0;
	/* 0x1E1 */ u8 unk1E1[3];
	/* 0x1E4 */ f32 unk1E4[3];
};

class TBossTelesa : public TSpineEnemy {
public:
	TBossTelesa(const char*);

	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual MtxPtr getTakingMtx();
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

	static f32 mEnemyGenRate;
	static f32 mItemGenRate;
	static u8 mNormalAlpha;
	static f32 mBaseHoseiPosY;
	static f32 mRouletteUpRate;
	static u32 mTelesaGenerateInterval;
	static f32 mCameraMoveLimit;
	static f32 mCameraMoveSp;

	enum {
		LIVE_FLAG_UNK10000 = 0x10000,
	};

public:
	/* 0x150 */ u8 unk150;
	/* 0x151 */ u8 unk151[0x3];
	/* 0x154 */ TTelesaSlot* unk154;
	/* 0x158 */ void* unk158;
	/* 0x15C */ void* unk15C;
	/* 0x160 */ s32 unk160;
	/* 0x164 */ s32 unk164;
	/* 0x168 */ f32 unk168;
	/* 0x16C */ TBossTelesaBody* unk16C;
	/* 0x170 */ TBossTelesaTongue* unk170;
	/* 0x174 */ TBossTelesaKillSmallEnemy* unk174;
	/* 0x178 */ TRoulette* unk178;
	/* 0x17C */ TRoulette* unk17C;
	/* 0x180 */ TRoulette* unk180;
	/* 0x184 */ TTelesaSlot* unk184;
	/* 0x188 */ JDrama::TViewObj* unk188;
	/* 0x18C */ u8 unk18C;
	/* 0x18D */ u8 unk18D[0xF];
	/* 0x19C */ JGeometry::TVec3<f32> unk19C;
	/* 0x1A8 */ s32 unk1A8;
	/* 0x1AC */ TLiveActor* unk1AC[50];
	/* 0x274 */ s32 unk274;
	/* 0x278 */ JGeometry::TMatrix34<JGeometry::SMatrix34C<f32> > unk278;
	/* 0x2A8 */ TMapObjBase* unk2A8[20];
	/* 0x2F8 */ TMapObjBase* unk2F8[10];
	/* 0x320 */ TLiveActor* unk320[10];
	/* 0x348 */ GXColor unk348;
	/* 0x34C */ GXColor unk34C;
	/* 0x350 */ u8 unk350;
	/* 0x351 */ u8 unk351[0x3];
	/* 0x354 */ TObjManager* unk354;
	/* 0x358 */ u16 unk358;
	/* 0x35A */ u8 unk35A;
	/* 0x35B */ u8 unk35B;
	/* 0x35C */ s32 unk35C;
	/* 0x360 */ f32 unk360;
	/* 0x364 */ f32 unk364;
	/* 0x368 */ s32 unk368;
	/* 0x36C */ s32 unk36C;
	/* 0x370 */ u8 unk370;
	/* 0x371 */ u8 unk371[0x3];
	/* 0x374 */ JGeometry::TVec3<f32> unk374;
	/* 0x380 */ s32 unk380;
	/* 0x384 */ u8 unk384;
	/* 0x385 */ u8 unk385[0x3];
	/* 0x388 */ s32 unk388;
};

class TBossTelesaBody : public THitActor {
public:
	TBossTelesaBody(const char* name)
	    : THitActor(name)
	    , unk6C(0)
	{
	}

	virtual BOOL receiveMessage(THitActor*, u32);
	void checkHit();

public:
	/* 0x68 */ TBossTelesa* unk68;
	/* 0x6C */ u8 unk6C;
};

class TBossTelesaTongue : public THitActor {
public:
	TBossTelesaTongue(const char* name)
	    : THitActor(name)
	{
	}

	virtual BOOL receiveMessage(THitActor*, u32);
	void checkHit();

public:
	/* 0x68 */ TBossTelesa* unk68;
};

class TBossTelesaKillSmallEnemy : public THitActor {
public:
	TBossTelesaKillSmallEnemy(const char* name)
	    : THitActor(name)
	    , unk68(nullptr)
	    , unk6C(0)
	{
	}

	void checkHit();

public:
	/* 0x68 */ TBossTelesa* unk68;
	/* 0x6C */ u8 unk6C;
};

class TBossTelesaManager : public TEnemyManager {
public:
	TBossTelesaManager(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();
	virtual void clipEnemies(JDrama::TGraphics*) { }
};

class TBubbleSaveLoadParams : public TWalkerEnemyParams {
public:
	TBubbleSaveLoadParams(const char*);

	/* 0x32C */ TParamRT<s32> mSLLiveTime;
	/* 0x340 */ TParamRT<s32> mSLNumDivision;
	/* 0x354 */ TParamRT<f32> mSLMaxScale;
	/* 0x368 */ TParamRT<f32> mSLAddPosBase;
	/* 0x37C */ TParamRT<f32> mSLRateExpand;
	/* 0x390 */ TParamRT<f32> mSLDeadHeight;
};

class TBubble : public TWalkerEnemy {
public:
	TBubble(const char* name = "バブル");

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

	void appendEnemy();
	void appendItem();
	void split();

public:
	/* 0x194 */ TBubbleSaveLoadParams* unk194;
	/* 0x198 */ TLiveActor* unk198;
	/* 0x19C */ JGeometry::TMatrix34<JGeometry::SMatrix34C<f32> > unk19C;
	/* 0x1CC */ f32 unk1CC;
	/* 0x1D0 */ u8 unk1D0;
	/* 0x1D1 */ u8 unk1D1;
	/* 0x1D2 */ u8 unk1D2;
	/* 0x1D3 */ u8 unk1D3;
};

class TBubbleManager : public TSmallEnemyManager {
public:
	TBubbleManager(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
	virtual TSmallEnemy* createEnemyInstance();
};

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

#endif
