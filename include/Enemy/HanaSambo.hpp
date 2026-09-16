#ifndef ENEMY_HANASAMBO_HPP
#define ENEMY_HANASAMBO_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Enemy/SmallEnemy.hpp>
#include <Enemy/WalkerEnemy.hpp>
#include <JSystem/JDrama/JDRGraphics.hpp>
#include <JSystem/JDrama/JDRNameRef.hpp>
#include <JSystem/JGeometry/JGVec3.hpp>
#include <Strategic/HitActor.hpp>
#include <Strategic/Nerve.hpp>
#include <Strategic/Spine.hpp>
#include <System/BaseParam.hpp>
#include <System/ParamInst.hpp>
#include <dolphin/types.h>

class JSUMemoryInputStream;
class SDLModelData;
class THitActor;
class TLiveManager;
class TSamboHead;
class TSamboHeadManager;
class TSamboHeadSaveLoadParams;
class THanaSambo;
class THanaSamboHead;
class THanaSamboManager;
class THanaSamboSaveLoadParams;
class TSamboFlower;
class TSamboFlowerManager;
class TSamboFlowerSaveLoadParams;
class TSamboLeaf;
class TSamboFlowerCoinUnit;

DECLARE_NERVE(TNerveSamboHeadHitWall, TLiveActor);

DECLARE_NERVE(TNerveSamboHeadRecoverWater, TLiveActor);

DECLARE_NERVE(TNerveSamboHeadHitWater, TLiveActor);

DECLARE_NERVE(TNerveSamboHeadAttack, TLiveActor);

DECLARE_NERVE(TNerveSamboHeadHide, TLiveActor);

DECLARE_NERVE(TNerveSamboHeadAppear, TLiveActor);

class TSamboHead : public TWalkerEnemy {
public:
	TSamboHead(const char*);

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
	static u8 mBodyJntIndex;

public:
	/* 0x194 */ void* unk194;
	/* 0x198 */ s32 unk198;
	/* 0x19C */ s32 unk19C;
	/* 0x1A0 */ char unk1A0[0xC];
	/* 0x1AC */ f32 unk1AC;
	/* 0x1B0 */ u8 unk1B0;
};

class TSamboHeadManager : public TSmallEnemyManager {
public:
	TSamboHeadManager(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();
};

class TSamboHeadSaveLoadParams : public TWalkerEnemyParams {
public:
	TSamboHeadSaveLoadParams(const char*);

public:
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

DECLARE_NERVE(TNerveHanaSamboFreeze, TLiveActor);

DECLARE_NERVE(TNerveHanaSamboDie, TLiveActor);

DECLARE_NERVE(TNerveHanaSamboHide, TLiveActor);

DECLARE_NERVE(TNerveHanaSamboAppear, TLiveActor);

DECLARE_NERVE(TNerveHanaSamboAttack, TLiveActor);

DECLARE_NERVE(TNerveHanaSamboWait, TLiveActor);

class THanaSambo : public TSmallEnemy {
public:
	THanaSambo(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);
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
		if (message == 0xB)
			return true;
		return false;
	}
	virtual bool isCollidMove(THitActor*) { return false; }

	void initFlower();
	void createPollen();
	void waterDamage();
	void setAttackAnm();
	static u8 mHeadJntIndex;
	static u8 mPollenJntIndex;

public:
	/* 0x194 */ THitActor* unk194;
	/* 0x198 */ THanaSamboSaveLoadParams* unk198;
	/* 0x19C */ JGeometry::TVec3<f32> unk19C;
	/* 0x1A8 */ s32 unk1A8;
	/* 0x1AC */ class TMBindShadowBody* unk1AC;
	/* 0x1B0 */ u8 unk1B0;
	/* 0x1B4 */ JGeometry::TVec3<f32> unk1B4[4];
};

class THanaSamboHead : public THitActor {
public:
	THanaSamboHead(); // TODO: constructor not in mario.MAP for this TU
	virtual BOOL receiveMessage(THitActor*, u32);

	void kill();
	bool checkHit();

public:
	/* 0x68 */ THanaSambo* unk68;
};

class THanaSamboManager : public TSmallEnemyManager {
public:
	THanaSamboManager(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();
};

class THanaSamboSaveLoadParams : public TSmallEnemyParams {
public:
	THanaSamboSaveLoadParams(const char*);

public:
	/* 0x2D4 */ TParamRT<f32> mSLAttackDist;
	/* 0x2E8 */ TParamRT<s32> mSLAttackInterval;
	/* 0x2FC */ TParamRT<f32> mSLHideDist;
	/* 0x310 */ TParamRT<f32> mSLAppearDist;
	/* 0x324 */ TParamRT<s32> mSLAttackingTime;
	/* 0x338 */ TParamRT<f32> mSLHeadAttackRadius;
	/* 0x34C */ TParamRT<f32> mSLHeadAttackHeight;
	/* 0x360 */ TParamRT<f32> mSLHeadDamageRadius;
	/* 0x374 */ TParamRT<f32> mSLHeadDamageHeight;
};

class TSamboFlower : public TSpineEnemy {
public:
	TSamboFlower(const char* name = "サンボフラワー");
	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void init(TLiveManager*);
	virtual void control() { }
	virtual void moveObject();
	virtual void drawObject(JDrama::TGraphics*);
	virtual void reset();
	virtual void setMActorAndKeeper();

	void hide();
	bool isBloomEnd();
	void bloom();

public:
	/* 0x150 */ u8 unk150;
	/* 0x151 */ char unk151[0x3];
	/* 0x154 */ s32 unk154;
	/* 0x158 */ s32 unk158;
	/* 0x15C */ s32 unk15C;
	/* 0x160 */ u8 unk160;
	/* 0x161 */ char unk161[0x3];
	/* 0x164 */ TSamboFlowerCoinUnit* unk164;
	/* 0x168 */ class TMapObjBase* unk168;
	/* 0x16C */ TSpineEnemyParams* unk16C;
};

class TSamboFlowerManager : public TEnemyManager {
public:
	TSamboFlowerManager(const char* name = "サンボフラワーマネージャー")
	    : TEnemyManager(name)
	    , unk54(0)
	    , unk58(0)
	    , unk5C(0)
	    , unk60(0)
	{
	}

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();
	virtual void dropLeaf(JGeometry::TVec3<f32>&, JGeometry::TVec3<f32>&);

public:
	/* 0x54 */ TSamboFlowerCoinUnit** unk54;
	/* 0x58 */ s32 unk58;
	/* 0x5C */ s32 unk5C;
	/* 0x60 */ TSamboLeaf** unk60;
	/* 0x64 */ class J3DMaterialTable* unk64;
};

class TSamboFlowerSaveLoadParams : public TSpineEnemyParams {
public:
	TSamboFlowerSaveLoadParams(const char*);

public:
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

class TSamboLeaf : public JDrama::TViewObj {
public:
	TSamboLeaf(TSamboFlowerManager*, SDLModelData*, const char*);

	virtual void perform(u32, JDrama::TGraphics*);

	void generate(JGeometry::TVec3<f32>&);

public:
	/* 0xC */ char unkC[0x4];
	/* 0x10 */ void* unk10;
	/* 0x14 */ f32 unk14;
	/* 0x18 */ f32 unk18;
	/* 0x1C */ f32 unk1C;
	/* 0x20 */ char unk20[0x4];
	/* 0x24 */ f32 unk24;
	/* 0x28 */ char unk28[0x4];
	/* 0x2C */ s32 unk2C;
	/* 0x30 */ s32 unk30;
	/* 0x34 */ s32 unk34;
	/* 0x38 */ f32 unk38;
	/* 0x3C */ f32 unk3C;
	/* 0x40 */ f32 unk40;
	/* 0x44 */ u8 unk44;
	/* 0x45 */ char unk45[0x3];
	/* 0x48 */ void* unk48;
};

class TSamboFlowerCoinUnit {
public:
	TSamboFlowerCoinUnit(int);

	bool checkGenCoin();
	void add(TSamboFlower*);

public:
	/* 0x0 */ s32 unk0;
	/* 0x4 */ s32 unk4;
	/* 0x8 */ s32 unk8;
	/* 0xC */ s32 unkC;
	/* 0x10 */ s32 unk10;
	/* 0x14 */ char unk14[0x4];
	/* 0x18 */ s32 unk18;
};

#endif // ENEMY_HANASAMBO_HPP
