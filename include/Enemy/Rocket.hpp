#ifndef ROCKET_H
#define ROCKET_H

#include <Enemy/EnemyManager.hpp>
#include <Enemy/Enemy.hpp>
#include <Enemy/SmallEnemy.hpp>
#include <Strategic/Strategy.hpp>
#include <Map/MapData.hpp>
#include <JSystem/JDrama/JDRGraphics.hpp>
#include <Strategic/Nerve.hpp>
#include <Strategic/Spine.hpp>
#include <System/ParamInst.hpp>

class THitActor;
class TLiveManager;
class TWaterEmitInfo;

class TRocketSaveLoadParams : public TSmallEnemyParams {
public:
	TRocketSaveLoadParams(const char*);

	/* 0x2D4 */ TParamRT<f32> mSLReleaseSpeed;
	/* 0x2E8 */ TParamRT<f32> mSLFlyGravity;
	/* 0x2FC */ TParamRT<s32> mSLFlyLimitTime;
};

class TRocket : public TSmallEnemy {
public:
	TRocket(const char* name = "ロケット");
	virtual ~TRocket() { }

	virtual void load(JSUMemoryInputStream&);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void bind();
	virtual f32 getGravityY() const;
	virtual const char** getBasNameTable() const;
	virtual void reset();
	virtual void behaveToWater(THitActor*);
	virtual void setDeadAnm();
	virtual void attackToMario();
	virtual void setMActorAndKeeper();
	virtual bool isCollidMove(THitActor*);

	bool isAttack();
	void flyBehavior();
	bool checkTrigger();
	void releaseNozzle();
	void possessedNozzle();

	static f32 mTestAng_x;
	static f32 mTestAng_y;
	static f32 mTestAng_z;
	static f32 mNozzleOffsetZ;
	static f32 mColOffsetY;

public:
	/* 0x194 */ JGeometry::TVec3<f32> unk194;
	/* 0x1A0 */ u8 unk1A0;
	/* 0x1A1 */ u8 unk1A1;
	/* 0x1A4 */ TRocketSaveLoadParams* unk1A4;
};

DECLARE_NERVE(TNerveRocketWait, TLiveActor);
DECLARE_NERVE(TNerveRocketFly, TLiveActor);
DECLARE_NERVE(TNerveRocketPossessedNozzle, TLiveActor);

class TRocketManager : public TSmallEnemyManager {
public:
	TRocketManager(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();
	virtual void clipEnemies(JDrama::TGraphics*) { }
	virtual void initSetEnemies();

public:
	/* 0x60 */ u8 unk60;
	/* 0x64 */ s32 unk64;
	/* 0x68 */ TWaterEmitInfo* unk68;
};

#endif
