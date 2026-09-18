#ifndef ENEMY_TOBI_PUKU_HPP
#define ENEMY_TOBI_PUKU_HPP

#include <Enemy/WalkerEnemy.hpp>
#include <Strategic/Nerve.hpp>

// The flying pukupuku. TMoePuku and TPukuPuku are variants that share its
// whole animation interface and differ only in their vtable.
// Names and defaults are the ones PARAM_INIT stringified into .rodata and the
// ones TTobiPukuManager::load stores, not guesses.
class TTobiPukuSaveLoadParams : public TWalkerEnemyParams {
public:
	TTobiPukuSaveLoadParams(const char* prm);

	// Each TParamRT holds its value at +0x10, which is where the accesses
	// at 0x33C, 0x350 and 0x364 land.
	/* 0x32C */ TParamRT<s32> mSLBoundNum;
	/* 0x340 */ TParamRT<f32> mSLBoundVal;
	/* 0x354 */ TParamRT<s32> mSLLifeTimer;
	/* 0x368 */ TParamRT<f32> mSLFlyGravityY;
	/* 0x37C */ TParamRT<f32> mSLPowerFromWater;
	/* 0x390 */ u8 unk390[0];
};

// The launch pad loads a different .prm class from the puku itself: the map
// lists UNUSED __ct__32TTobiPukuLaunchPadSaveLoadParams alongside
// __ct__23TTobiPukuSaveLoadParams. It cannot derive from TWalkerEnemyParams,
// because the slot at 0x2D4 that holds the float mSLZigzagCycle there is read
// as an int here. 0x2D4 is exactly where TSmallEnemyParams ends.
class TTobiPukuLaunchPadSaveLoadParams : public TSmallEnemyParams {
public:
	TTobiPukuLaunchPadSaveLoadParams(const char* prm);

	/* 0x2D4 */ TParamRT<s32> mSLLaunchInterval;
	/* 0x2E8 */ TParamRT<f32> mSLLaunchVelocityY;
	/* 0x2FC */ TParamRT<f32> mSLFlyDist;
	/* 0x310 */ TParamRT<f32> mSLFlySpeed;
	/* 0x324 */ TParamRT<f32> mSLLaunchAngle;
};

class TTobiPukuLaunchPad;

class TTobiPuku : public TWalkerEnemy {
public:
	TTobiPuku(const char* name);

	// Implicit in retail: the map has this dtor weak, so the body is
	// in-class.
	virtual ~TTobiPuku() { }

	// Overrides of base virtuals; these occupy inherited slots.
	virtual void initAttacker(THitActor*);
	virtual BOOL isInhibitedForceMove()
	{
		return checkLiveFlag(LIVE_FLAG_AIRBORNE) ? TRUE : FALSE;
	}
	virtual void generateEffectColumWater();
	virtual void walkBehavior(int, f32);
	virtual void scalingChangeActor();
	virtual void changeOut();
	virtual void reset();
	virtual void kill();
	virtual void forceKill();
	virtual void attackToMario();
	virtual void behaveToWater(THitActor*);
	virtual void calcRootMatrix();

	// New virtuals, declared in the order the vtable lists them
	// (0x1b8 onwards).
	virtual bool isPichiEffect();
	virtual BOOL isJumpBck();
	virtual BOOL isDeadBck();
	virtual bool isJumpStartBck();
	virtual bool isAttackBck();
	virtual BOOL isFallEndLandBck();
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
	virtual const char** getBasNameTable() const;
	virtual f32 getGravityY() const;
	void genEventCoin();
	void init(TLiveManager*);
	virtual void moveObject();
	void hitWall();
	bool isRoll();
	// UNUSED in the map (0x74 / 0x90 / 0x2c / 0xc8); their emission
	// positions bracket hitWater, setJumpStartAnm and calcRootMatrix.
	void fallStart();
	void flyStart();
	bool canBound();
	void bound();


	// Overrides the base slot at 0x154.
	virtual void setDeadAnm();

	// fabricated
	TTobiPukuSaveLoadParams* getSaveParam2() const
	{
		return (TTobiPukuSaveLoadParams*)getSaveParam();
	}

	static f32 mLandAngle;
	static u8 mBoundSw;
	static f32 mBoundVelocityY;
	static u8 mReturnLaunchSw;

	/* 0x194 */ u8 unk194;
	/* 0x198 */ int mBoundCount;
	/* 0x19C */ TTobiPukuSaveLoadParams* unk19C;
	/* 0x1A0 */ JGeometry::TVec3<f32> mFlamePos;
	/* 0x1AC */ u8 unk1AC;
	/* 0x1AD */ u8 unk1AD;
	/* 0x1AE */ u8 unk1AE;
	/* 0x1AF */ u8 unk1AF;
	/* 0x1B0 */ f32 unk1B0;
	// 0x1B4 is a scalar, not a vector: the Land nerve uses 0x1B8 onwards as
	// its own landing position and delta.
	/* 0x1B4 */ f32 mLaunchAngle;
	/* 0x1B8 */ JGeometry::TVec3<f32> mLandPos;
	/* 0x1C4 */ JGeometry::TVec3<f32> mLandDelta;
	/* 0x1D0 */ JGeometry::TVec3<f32> mLaunchVelocity;
	/* 0x1DC */ TTobiPukuLaunchPad* mLaunchPad;
	/* 0x1E0 */ f32 mSwimBaseY;
	/* 0x1E4 */ f32 mFlyVelocityY;
	/* 0x1E8 */ f32 mReturnPitchStep;
	/* 0x1EC */ f32 unk1EC;
	/* 0x1F0 */ f32 mRotStep;
};

class TMoePuku : public TTobiPuku {
public:
	TMoePuku(const char* name)
	    : TTobiPuku(name)
	{
	}

	// Implicit in retail: the map has this dtor weak, so the body is
	// in-class.
	virtual ~TMoePuku() { }

	// Overrides of base virtuals; these occupy inherited slots.
	virtual void generateEffectColumWater();
	virtual void calcRootMatrix();

	// Overrides of TTobiPuku's animation-state virtuals, in vtable
	// order. The map lists no TMoePuku copy of initAttacker,
	// isInhibitedForceMove, walkBehavior, isReachedToGoalXZ,
	// getGravityY or genEventCoin, so TMoePuku inherits those.
	virtual bool isPichiEffect();
	virtual BOOL isJumpBck();
	virtual BOOL isDeadBck();
	virtual bool isJumpStartBck();
	virtual bool isAttackBck();
	virtual BOOL isFallEndLandBck();
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
	virtual const char** getBasNameTable() const;


	// Overrides the base slot at 0x154.
	virtual void setDeadAnm();
};

class TPukuPuku : public TTobiPuku {
public:
	TPukuPuku(const char* name);

	// Implicit in retail: the map has this dtor weak, so the body is
	// in-class.
	virtual ~TPukuPuku() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void init(TLiveManager*);
	virtual void reset();
};

class TTobiPukuLaunchPad : public TSmallEnemy {
public:
	TTobiPukuLaunchPad(const char* name);

	// Implicit in retail: the map has this dtor weak, so the body is
	// in-class.
	virtual ~TTobiPukuLaunchPad() { }
	virtual void init(TLiveManager*);
	virtual void reset();
	virtual void launch();
	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	void forceLaunch(TTobiPuku*);

	/* 0x194 */ int unk194;
	/* 0x198 */ TTobiPukuLaunchPadSaveLoadParams* unk198;
	/* 0x19C */ f32 unk19C;
	/* 0x1A0 */ u8 unk1A0[0x1A8 - 0x1A0];
	/* 0x1A8 */ TTobiPuku* unk1A8;
};

class TMoePukuLaunchPad : public TTobiPukuLaunchPad {
public:
	TMoePukuLaunchPad(const char* name)
	    : TTobiPukuLaunchPad(name)
	{
	}

	// Implicit in retail: the map has this dtor weak, so the body is
	// in-class.
	virtual ~TMoePukuLaunchPad() { }
	virtual void launch();
};

class TTobiPukuManager : public TSmallEnemyManager {
public:
	TTobiPukuManager(const char* name);

	// Implicit in retail: the map has this dtor weak, so the body is
	// in-class.
	virtual ~TTobiPukuManager() { }
	virtual TSpineEnemy* createEnemyInstance();
	virtual void load(JSUMemoryInputStream&);
};

class TMoePukuManager : public TTobiPukuManager {
public:
	TMoePukuManager(const char* name)
	    : TTobiPukuManager(name)
	{
	}

	// Implicit in retail: the map has this dtor weak, so the body is
	// in-class.
	virtual ~TMoePukuManager() { }
	virtual TSpineEnemy* createEnemyInstance();
};

class TTobiPukuLaunchPadManager : public TSmallEnemyManager {
public:
	TTobiPukuLaunchPadManager(const char* name);

	/* 0x60 */ u8 unk60;

	// Implicit in retail: the map has this dtor weak, so the body is
	// in-class.
	virtual ~TTobiPukuLaunchPadManager() { }
	virtual TSpineEnemy* createEnemyInstance();
	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
};

class TMoePukuLaunchPadManager : public TTobiPukuLaunchPadManager {
public:
	TMoePukuLaunchPadManager(const char* name)
	    : TTobiPukuLaunchPadManager(name)
	{
	}

	// Implicit in retail: the map has this dtor weak, so the body is
	// in-class.
	virtual ~TMoePukuLaunchPadManager() { }
	virtual TSpineEnemy* createEnemyInstance();
};


DECLARE_NERVE(TNerveTobiPukuSwimWander, TLiveActor)
DECLARE_NERVE(TNerveTobiPukuReturnLaunch, TLiveActor)
DECLARE_NERVE(TNerveTobiPukuPrepareFly, TLiveActor)
DECLARE_NERVE(TNerveTobiPukuBound, TLiveActor)
DECLARE_NERVE(TNerveTobiPukuLand, TLiveActor)
DECLARE_NERVE(TNerveTobiPukuDie, TLiveActor)
DECLARE_NERVE(TNerveTobiPukuPitiPiti, TLiveActor)
DECLARE_NERVE(TNerveTobiPukuFall, TLiveActor)
DECLARE_NERVE(TNerveTobiPukuHitWater, TLiveActor)
DECLARE_NERVE(TNerveTobiPukuAttack, TLiveActor)
DECLARE_NERVE(TNerveTobiPukuFly, TLiveActor)
DECLARE_NERVE(TNerveTobiPukuGenerate, TLiveActor)


#endif
