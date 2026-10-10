#ifndef ENEMY_BOSSTELESA_HPP
#define ENEMY_BOSSTELESA_HPP

#include <Enemy/Enemy.hpp>
#include <JSystem/JGeometry.hpp>
#include <dolphin/gx/GXStruct.h>
#include <Enemy/EnemyManager.hpp>
#include <Enemy/SmallEnemy.hpp>

class TTelesaSlot;
class TBossTelesaBody;
class TBossTelesaTongue;
class TBossTelesaKillSmallEnemy;
class TRoulette;
class TMapObjBase;
class TObjManager;

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
	/* 0x178 */ TRoulette* unk178[3];
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

class TBossTelesaManager : public TEnemyManager {
public:
	TBossTelesaManager(const char*);

	virtual ~TBossTelesaManager() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();
	virtual void clipEnemies(JDrama::TGraphics*);
};

class TBubbleManager : public TSmallEnemyManager {
public:
	TBubbleManager(const char*);

	virtual ~TBubbleManager() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
	virtual TSmallEnemy* createEnemyInstance();
};

#endif
