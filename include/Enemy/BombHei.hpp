#ifndef ENEMY_BOMBHEI_HPP
#define ENEMY_BOMBHEI_HPP

#include <Enemy/SmallEnemy.hpp>
#include <Enemy/WalkerEnemy.hpp>
#include <Strategic/Nerve.hpp>
#include <Strategic/Spine.hpp>
#include <dolphin/types.h>

class JSUMemoryInputStream;
class THitActor;
class TLiveManager;

DECLARE_NERVE(TNerveBombHeiExplosion, TLiveActor);

DECLARE_NERVE(TNerveBombHeiThrown, TLiveActor);

DECLARE_NERVE(TNerveBombHeiPickUp, TLiveActor);

DECLARE_NERVE(TNerveBombHeiWaitExplosion, TLiveActor);

DECLARE_NERVE(TNerveBombHeiWalkExplosion, TLiveActor);

DECLARE_NERVE(TNerveBombHeiAttack, TLiveActor);

DECLARE_NERVE(TNerveBombHeiGenerate, TLiveActor);

class TBombHeiSaveLoadParams : public TWalkerEnemyParams {
public:
	TBombHeiSaveLoadParams(const char*);

	/* 0x32C */ TParamRT<s32> mSLBombTime;
	/* 0x340 */ TParamRT<f32> mSLBombRange;
	/* 0x354 */ TParamRT<f32> mSLThrownVY;
	/* 0x368 */ TParamRT<f32> mSLThrownRateXZ;
	/* 0x37C */ TParamRT<f32> mSLThrownGravityY;
	/* 0x390 */ TParamRT<f32> mSLShootVelocity;
};

class TBombHei : public TWalkerEnemy {
public:
	TBombHei(const char* name = "ボム兵");

	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void moveObject();
	virtual void kill();
	virtual f32 getGravityY() const;
	virtual const char** getBasNameTable() const;
	virtual void reset();
	virtual void genEventCoin();
	virtual void behaveToWater(THitActor*);
	virtual void changeOut();
	virtual void behaveToTaken(THitActor*);
	virtual void behaveToRelease();
	virtual void setWalkAnm();
	virtual void setDeadAnm();
	virtual void setFreezeAnm();
	virtual void attackToMario();
	virtual void forceKill();
	virtual void setMActorAndKeeper();
	virtual bool isHitValid(u32);
	virtual bool isCollidMove(THitActor*);
	virtual void setAfterDeadEffect() { }
	virtual bool doKeepDistance() { return unk19C; }
	virtual void walkBehavior(int, f32);

	bool isDamageToCannon();
	bool isExplosion();
	void bombIn();

	static bool mSerialBomb;

public:
	/* 0x194 */ TBombHeiSaveLoadParams* unk194;
	/* 0x198 */ s32 unk198;
	/* 0x19C */ u8 unk19C;
	/* 0x1A0 */ f32 unk1A0;
	/* 0x1A4 */ u8 unk1A4;
};

class TBombHeiManager : public TSmallEnemyManager {
public:
	TBombHeiManager(const char* name = "ボム兵マネージャー");

	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();
#ifdef VERSION_GMSP01
	virtual void clipEnemies(JDrama::TGraphics*) { }
#endif

	bool canMakeDeadCoin();

public:
	/* 0x60 */ s32 unk60;
};

#endif // ENEMY_BOMBHEI_HPP
