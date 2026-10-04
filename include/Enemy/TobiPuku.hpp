#ifndef ENEMY_TOBIPUKU_HPP
#define ENEMY_TOBIPUKU_HPP

#include <Enemy/SmallEnemy.hpp>
#include <Enemy/WalkerEnemy.hpp>
#include <JSystem/JDrama/JDRGraphics.hpp>
#include <JSystem/JGeometry/JGVec3.hpp>
#include <Strategic/Nerve.hpp>
#include <Strategic/Spine.hpp>
#include <dolphin/types.h>

class JSUMemoryInputStream;
class THitActor;
class TLiveManager;
class TMoePuku;
class TPukuPuku;
class TTobiPuku;
class TMoePukuLaunchPad;
class TTobiPukuLaunchPad;
class TMoePukuManager;
class TTobiPukuManager;
class TMoePukuLaunchPadManager;
class TTobiPukuLaunchPadManager;
class TTobiPukuSaveLoadParams;
class TTobiPukuLaunchPadSaveLoadParams;

DECLARE_NERVE(TNerveTobiPukuSwimWander, TLiveActor);

DECLARE_NERVE(TNerveTobiPukuReturnLaunch, TLiveActor);

DECLARE_NERVE(TNerveTobiPukuPrepareFly, TLiveActor);

DECLARE_NERVE(TNerveTobiPukuBound, TLiveActor);

DECLARE_NERVE(TNerveTobiPukuLand, TLiveActor);

DECLARE_NERVE(TNerveTobiPukuPitiPiti, TLiveActor);

DECLARE_NERVE(TNerveTobiPukuDie, TLiveActor);

DECLARE_NERVE(TNerveTobiPukuFall, TLiveActor);

DECLARE_NERVE(TNerveTobiPukuHitWater, TLiveActor);

DECLARE_NERVE(TNerveTobiPukuAttack, TLiveActor);

DECLARE_NERVE(TNerveTobiPukuFly, TLiveActor);

DECLARE_NERVE(TNerveTobiPukuGenerate, TLiveActor);

class TTobiPuku : public TWalkerEnemy {
public:
	TTobiPuku(const char*);

	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void moveObject();
	virtual void kill();
	virtual f32 getGravityY() const;
	virtual const char** getBasNameTable() const;
	virtual void reset();
	virtual void genEventCoin();
	virtual void behaveToWater(THitActor*);
	virtual void scalingChangeActor();
	virtual void changeOut();
	virtual void setDeadAnm();
	virtual void attackToMario();
	virtual void forceKill();
	virtual void initAttacker(THitActor*);
	virtual BOOL isInhibitedForceMove()
	{
		if (checkLiveFlag(LIVE_FLAG_AIRBORNE))
			return 1;
		return 0;
	}
	virtual void generateEffectColumWater();
	virtual void walkBehavior(int, f32);
	virtual bool isPichiEffect();
	virtual bool isJumpBck();
	virtual bool isDeadBck();
	virtual bool isJumpStartBck();
	virtual bool isAttackBck();
	virtual bool isFallEndLandBck();
	virtual void setJumpAnm();
	virtual void setJumpStartAnm();
	virtual void setSwimAnm();
	virtual void setAttackAnm();
	virtual void setPichiAnm();
	virtual void setFallAnm();
	virtual void setDownAirAnm();
	virtual void setDownLandAnm();
	virtual void setFallEndLandAnm();
	virtual void swimEffect();
	virtual bool isReachedToGoalXZ();
	virtual void hitWater();

	void fallStart();
	void flyStart();
	bool isRoll();
	bool canBound();
	void bound();
	void hitWall();
	static f32 mLandAngle;
	static bool mBoundSw;
	static f32 mBoundVelocityY;
	static bool mReturnLaunchSw;

public:
	/* 0x194 */ u8 unk194;
	/* 0x195 */ char unk195[0x3];
	/* 0x198 */ s32 unk198;
	/* 0x19C */ TTobiPukuSaveLoadParams* unk19C;
	/* 0x1A0 */ JGeometry::TVec3<f32> unk1A0;
	/* 0x1AC */ u8 unk1AC;
	/* 0x1AD */ u8 unk1AD;
	/* 0x1AE */ u8 unk1AE;
	/* 0x1AF */ char unk1AF[0x1];
	/* 0x1B0 */ f32 unk1B0;
	/* 0x1B4 */ f32 unk1B4;
	/* 0x1B8 */ JGeometry::TVec3<f32> unk1B8[2];
	/* 0x1D0 */ JGeometry::TVec3<f32> unk1D0;
	/* 0x1DC */ TTobiPukuLaunchPad* unk1DC;
	/* 0x1E0 */ f32 unk1E0;
	/* 0x1E4 */ f32 unk1E4;
	/* 0x1E8 */ f32 unk1E8;
	/* 0x1EC */ f32 unk1EC;
	/* 0x1F0 */ f32 unk1F0;
};

class TPukuPuku : public TTobiPuku {
public:
	TPukuPuku(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void init(TLiveManager*);
	virtual void reset();
};

class TMoePuku : public TTobiPuku {
public:
	TMoePuku(const char* name = "モエプク")
	    : TTobiPuku(name)
	{
	}
	virtual void calcRootMatrix();
	virtual const char** getBasNameTable() const;
	virtual void setDeadAnm();
	virtual void generateEffectColumWater();
	virtual bool isPichiEffect();
	virtual bool isJumpBck();
	virtual bool isDeadBck();
	virtual bool isJumpStartBck();
	virtual bool isAttackBck();
	virtual bool isFallEndLandBck();
	virtual void setJumpAnm();
	virtual void setJumpStartAnm();
	virtual void setSwimAnm();
	virtual void setAttackAnm();
	virtual void setPichiAnm();
	virtual void setFallAnm();
	virtual void setDownAirAnm();
	virtual void setDownLandAnm();
	virtual void setFallEndLandAnm();
	virtual void swimEffect() { }
	virtual void hitWater();
};

class TTobiPukuLaunchPad : public TSmallEnemy {
public:
	TTobiPukuLaunchPad(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual void init(TLiveManager*);
	virtual void reset();
	virtual void launch();

	void forceLaunch(TTobiPuku*);

public:
	/* 0x194 */ s32 unk194;
	/* 0x198 */ TTobiPukuLaunchPadSaveLoadParams* unk198;
	/* 0x19C */ f32 unk19C;
	/* 0x1A0 */ char unk1A0[0x8];
	/* 0x1A8 */ TTobiPuku* unk1A8;
};

class TMoePukuLaunchPad : public TTobiPukuLaunchPad {
public:
	TMoePukuLaunchPad(const char* name = "モエプク発射台")
	    : TTobiPukuLaunchPad(name)
	{
	}
	virtual void launch();
};

class TTobiPukuManager : public TSmallEnemyManager {
public:
	TTobiPukuManager(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual TSpineEnemy* createEnemyInstance();
};

class TMoePukuManager : public TTobiPukuManager {
public:
	TMoePukuManager(const char* name = "モエプクマネージャー")
	    : TTobiPukuManager(name)
	{
	}
	virtual TSpineEnemy* createEnemyInstance();
};

class TTobiPukuLaunchPadManager : public TSmallEnemyManager {
public:
	TTobiPukuLaunchPadManager(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual TSpineEnemy* createEnemyInstance();

public:
	/* 0x60 */ u8 unk60;
};

class TMoePukuLaunchPadManager : public TTobiPukuLaunchPadManager {
public:
	TMoePukuLaunchPadManager(const char* name = "モエプク発射台マネージャー")
	    : TTobiPukuLaunchPadManager(name)
	{
	}
	virtual TSpineEnemy* createEnemyInstance();
};

class TTobiPukuSaveLoadParams : public TWalkerEnemyParams {
public:
	TTobiPukuSaveLoadParams(const char*);

	/* 0x32C */ TParamRT<s32> mSLBoundNum;
	/* 0x340 */ TParamRT<f32> mSLBoundVal;
	/* 0x354 */ TParamRT<s32> mSLLifeTimer;
	/* 0x368 */ TParamRT<f32> mSLFlyGravityY;
	/* 0x37C */ TParamRT<f32> mSLPowerFromWater;
};

class TTobiPukuLaunchPadSaveLoadParams : public TSmallEnemyParams {
public:
	TTobiPukuLaunchPadSaveLoadParams(const char*);

	/* 0x2D4 */ TParamRT<s32> mSLLaunchInterval;
	/* 0x2E8 */ TParamRT<f32> mSLLaunchVelocityY;
	/* 0x2FC */ TParamRT<f32> mSLFlyDist;
	/* 0x310 */ TParamRT<f32> mSLFlySpeed;
	/* 0x324 */ TParamRT<f32> mSLLaunchAngle;
};

#endif // ENEMY_TOBIPUKU_HPP
