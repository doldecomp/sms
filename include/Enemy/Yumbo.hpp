#ifndef ENEMY_YUMBO_HPP
#define ENEMY_YUMBO_HPP

#include <Enemy/SmallEnemy.hpp>
#include <JSystem/JDrama/JDRGraphics.hpp>
#include <JSystem/JGeometry/JGVec3.hpp>
#include <Strategic/HitActor.hpp>
#include <Strategic/Nerve.hpp>
#include <Strategic/Spine.hpp>
#include <dolphin/types.h>

class J3DMaterialTable;
class JSUMemoryInputStream;
class MActor;
class TLiveManager;
class TYumboSeed;

DECLARE_NERVE(TNerveYumboFreeze, TLiveActor);

DECLARE_NERVE(TNerveYumboDancing, TLiveActor);

DECLARE_NERVE(TNerveYumboAttack, TLiveActor);

DECLARE_NERVE(TNerveYumboAppearing, TLiveActor);

DECLARE_NERVE(TNerveYumboHiding, TLiveActor);

class TYumbo : public TSmallEnemy {
public:
	TYumbo(const char*);

	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void init(TLiveManager*);
	virtual void moveObject();
	virtual const char** getBasNameTable() const;
	virtual void reset();
	virtual void behaveToWater(THitActor*);
	virtual void setDeadAnm();
	virtual void attackToMario();
	virtual bool doKeepDistance();

	TYumboSeed* getUnusedSeed();
	bool isDead() const;
	bool isFreeze() const;
	bool isWaterproof() const;
	void changeToYumbo();
	void changeToFlower();
	void lookatMario();
	void shotSeeds();
	bool isAllSeedBroken() const;
	bool isWantToAppear() const;
	bool isFindOutMario() const;
	void updateEffect();
	void updateCollision();
	void behaveHitAttack();
	void initCollision();
	static void setMaterialToMActor(MActor*, J3DMaterialTable*);
	void initMActorAndKeeper();

public:
	/* 0x194 */ TYumboSeed* unk194[16];
	/* 0x1D4 */ s32 unk1D4;
	/* 0x1D8 */ u8 unk1D8;
};

class TYumboManager : public TSmallEnemyManager {
public:
	TYumboManager(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();

	void loadMaterialTable(J3DMaterialTable**, const char*);

public:
	/* 0x60 */ J3DMaterialTable* unk60;
};

class TYumboParams : public TSmallEnemyParams {
public:
	TYumboParams(const char*);

	/* 0x2D4 */ TParamRT<s32> mRecoverTimer;
	/* 0x2E8 */ TParamRT<f32> mShootSpeed;
	/* 0x2FC */ TParamRT<f32> mShootAngleX;
	/* 0x310 */ TParamRT<s32> mSeedLife;
	/* 0x324 */ TParamRT<f32> mSeedAirFric;
	/* 0x338 */ TParamRT<f32> mSeedGravityY;
};

class TYumboSeed : public THitActor {
public:
	TYumboSeed(MActor*, const TYumbo&);

	virtual void perform(u32, JDrama::TGraphics*);
	virtual void init();

	void startToMove(const JGeometry::TVec3<f32>&, const JGeometry::TVec3<f32>&,
	                 int);
	void checkHitActors();

public:
	/* 0x68 */ const TYumbo* unk68;
	/* 0x6C */ MActor* unk6C;
	/* 0x70 */ s32 unk70;
	/* 0x74 */ s32 unk74;
	/* 0x78 */ JGeometry::TVec3<f32> unk78;
};

#endif // ENEMY_YUMBO_HPP
