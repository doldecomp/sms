#ifndef ENEMY_ELECNOKONOKO_HPP
#define ENEMY_ELECNOKONOKO_HPP

#include <Enemy/EnemyAttachment.hpp>
#include <Enemy/SmallEnemy.hpp>
#include <Enemy/WalkerEnemy.hpp>
#include <JSystem/JDrama/JDRGraphics.hpp>
#include <Strategic/Nerve.hpp>
#include <Strategic/Spine.hpp>
#include <dolphin/types.h>

class J3DMaterialTable;
class JSUMemoryInputStream;
class TBGCheckData;
class THitActor;
class TLiveManager;
class TElecNokonoko;

DECLARE_NERVE(TNerveElecCarapaceReturn, TLiveActor);

DECLARE_NERVE(TNerveElecNokonokoFreeze, TLiveActor);

DECLARE_NERVE(TNerveElecCarapaceWait, TLiveActor);

DECLARE_NERVE(TNerveElecCarapaceMove, TLiveActor);

DECLARE_NERVE(TNerveElecNokonokoCollect, TLiveActor);

DECLARE_NERVE(TNerveElecNokonokoAttack, TLiveActor);

DECLARE_NERVE(TNerveElecNokonokoRebirth, TLiveActor);

DECLARE_NERVE(TNerveElecNokonokoTurn, TLiveActor);

DECLARE_NERVE(TNerveElecNokonokoShoot, TLiveActor);

class TElecNokonokoSaveLoadParams : public TWalkerEnemyParams {
public:
	TElecNokonokoSaveLoadParams(const char*);

public:
	/* 0x32C */ TParamRT<s32> mSLReadyTime;
	/* 0x340 */ TParamRT<f32> mSLCarapaceGravity;
	/* 0x354 */ TParamRT<f32> mSLCarapaceSpeed;
	/* 0x368 */ TParamRT<f32> mSLCarapaceTurnSpeed;
	/* 0x37C */ TParamRT<f32> mSLCarapaceSpinSpeed;
	/* 0x390 */ TParamRT<f32> mSLCarapaceShootRange;
	/* 0x3A4 */ TParamRT<f32> mSLCarapaceFlyDist;
};

class TElecCarapace : public TEnemyAttachment {
public:
	TElecCarapace(const char* name = "ノコノコ甲羅");

	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void calcRootMatrix();
	virtual void bind();
	virtual void kill();
	virtual f32 getPhaseShift() const
	{
		if (unk174 != 0)
			return 0.0f;
		return 180.0f;
	}
	virtual void loadInit(TSpineEnemy*, const char*);
	virtual void appear();
	virtual void rebirth() { }
	virtual void sendMessage();
	virtual void behaveToHitGround();
	virtual void behaveToHitWall(const TBGCheckData*);
	virtual void setBehavior();
	virtual void recoverScale() { }
	virtual f32 getNowGravity();
	virtual void shoot();

	bool isMove();
	void move();
	void reflect(THitActor*);
	void setZigParameter();

public:
	/* 0x16C */ TElecNokonoko* unk16C;
	/* 0x170 */ THitActor* unk170;
	/* 0x174 */ u8 unk174;
	/* 0x175 */ u8 unk175;
	/* 0x176 */ u8 unk176;
	/* 0x178 */ f32 unk178;
	/* 0x17C */ f32 unk17C;
	/* 0x180 */ s32 unk180;
	/* 0x184 */ u8 unk184;
	/* 0x188 */ f32 unk188;
	/* 0x18C */ JGeometry::TVec3<f32> unk18C;
	/* 0x198 */ f32 unk198;
};

class TElecNokonoko : public TWalkerEnemy {
public:
	TElecNokonoko(const char* name = "電気ノコノコ");

	virtual void load(JSUMemoryInputStream&);
	virtual BOOL receiveMessage(THitActor*, u32);
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
	bool hasCarapace() const
	{
		if (unk1A4 == 0)
			return true;
		else
			return false;
	}

	static bool mReflectSw;
	static u8 mCarapaceJntIndex;

public:
	/* 0x194 */ TElecCarapace* unk194;
	/* 0x198 */ s32 unk198;
	/* 0x19C */ s32 unk19C;
	/* 0x1A0 */ TElecNokonokoSaveLoadParams* unk1A0;
	/* 0x1A4 */ s32 unk1A4;
	/* 0x1A8 */ JGeometry::TVec3<f32> unk1A8;
};

class TElecNokonokoManager : public TSmallEnemyManager {
public:
	TElecNokonokoManager(const char* name = "電気ノコノコマネージャー");

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();
	virtual void clipEnemies(JDrama::TGraphics*);
	virtual void initSetEnemies();

public:
	/* 0x60 */ J3DMaterialTable* unk60;
};

#endif
