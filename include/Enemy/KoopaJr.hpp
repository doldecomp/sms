#ifndef ENEMY_KOOPAJR_HPP
#define ENEMY_KOOPAJR_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <JSystem/JGeometry/JGVec3.hpp>
#include <dolphin/types.h>

class TDirectionCalc {
public:
	TDirectionCalc(JGeometry::TVec3<f32>);
	TDirectionCalc(f32);
	TDirectionCalc();

	static f32 r2d(f32);
	static f32 d2r(f32);
	f32 absDirection(f32);
	JGeometry::TVec3<f32> calcDirectionVector();
	void makeDirection(JGeometry::TVec3<f32>);
	f32 calcTurnDirection(f32, f32);
	f32 sub(f32);
	f32 calcNearerDirection(f32);
	void normalize();

public:
	/* 0x0 */ f32 unk0;
};

class TBathtub;
class TBathtubBinder;
class TBathtubKiller;
class TCallbackHitActor;
class TKoopa;
class TKoopaJrSubmarine;
class TKoopaJrSubmarineManager;

class TKoopaJr : public TSpineEnemy {
public:
	TKoopaJr(const char*);
	virtual ~TKoopaJr() { }
	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual const char** getBasNameTable() const;
	virtual void reset();

	void checkNerveKillerHit();
	void checkNerveKillerLaunchFast();
	void checkNerveKillerLaunchNormal();
	void checkNerve();
	void checkSubmarineSwing();
	void damageKoopaJr();
	void emitKoopaJrEffects();
	f32 getBathtubY();
	void resetKoopaJr();
	void setAnimationIndex(int);
	void startDamageNerve();
	void startKoopaJrMessage(u32);
	void updateTimers();

public:
	/* 0x150 */ s32 unk150;
	/* 0x154 */ s32 unk154;
	/* 0x158 */ s32 unk158;
	/* 0x15C */ TBathtub* unk15C;
	/* 0x160 */ TKoopa* unk160;
	/* 0x164 */ TKoopaJrSubmarine* unk164;
	/* 0x168 */ TKoopaJrSubmarineManager* unk168;
	/* 0x16C */ TEnemyManager* unk16C;
};

class TKoopaJrSubmarine : public TSpineEnemy {
public:
	TKoopaJrSubmarine(const char*);
	virtual ~TKoopaJrSubmarine() { }
	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void bind();
	virtual const char** getBasNameTable() const;
	virtual void reset();

	BOOL appearShineKiller(int);
	void checkKillerLaunch();
	void checkNerve();
	void damageKoopaJrSubmarine();
	void emitKoopaJrSubmarineEffects();
	f32 getSwingAngle();
	f32 getWaveAngle();
	void launchKiller();
	void makeCollisionPositions();
	void makeDirection();
	void makeKillerVelocity(TBathtubKiller*, JGeometry::TVec3<f32>);
	void makeRelativeAngle();
	void makeRoundVelocity();
	void moveSwing();
	void prepareKillerLaunchFast(int);
	void prepareKillerLaunch(int);
	void resetKoopaJrSubmarine();
	void setAnimationIndex(int);
	void setKoopaJr(TKoopaJr*);
	void updateTimers();

public:
	/* 0x150 */ s32 unk150;
	/* 0x154 */ f32 unk154;
	/* 0x158 */ f32 unk158;
	/* 0x15C */ f32 unk15C;
	/* 0x160 */ f32 unk160;
	/* 0x164 */ f32 unk164;
	/* 0x168 */ f32 unk168;
	/* 0x16C */ TDirectionCalc unk16C;
	/* 0x170 */ u8 unk170;
	/* 0x174 */ TBathtubBinder* unk174;
	/* 0x178 */ s8 unk178[8];
	/* 0x180 */ s32 unk180;
	/* 0x184 */ s32 unk184;
	/* 0x188 */ f32 unk188;
	/* 0x18C */ u8 unk18C;
	/* 0x190 */ f32 unk190;
	/* 0x194 */ f32 unk194;
	/* 0x198 */ f32 unk198;
	/* 0x19C */ f32 unk19C;
	/* 0x1A0 */ TKoopaJr* unk1A0;
	/* 0x1A4 */ TCallbackHitActor* unk1A4;
	/* 0x1A8 */ TCallbackHitActor* unk1A8;
};

class TKoopaJrManager : public TEnemyManager {
public:
	TKoopaJrManager(const char*);

	virtual ~TKoopaJrManager() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();
};

class TKoopaJrSubmarineManager : public TEnemyManager {
public:
	TKoopaJrSubmarineManager(const char*);

	virtual ~TKoopaJrSubmarineManager() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();
};

#endif
