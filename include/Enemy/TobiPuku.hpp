#ifndef ENEMY_TOBI_PUKU_HPP
#define ENEMY_TOBI_PUKU_HPP

#include <Enemy/WalkerEnemy.hpp>
#include <Strategic/Nerve.hpp>

// The flying pukupuku. TMoePuku and TPukuPuku are variants that share its
// whole animation interface and differ only in their vtable.
class TTobiPuku : public TWalkerEnemy {
public:
	TTobiPuku(const char* name)
	    : TWalkerEnemy(name)
	{
	}

	virtual ~TTobiPuku();

	// Overrides of base virtuals; these occupy inherited slots.
	virtual void initAttacker(THitActor*);
	virtual BOOL isInhibitedForceMove();
	virtual void generateEffectColumWater();
	virtual void walkBehavior(int, f32);

	// New virtuals, declared in the order the vtable lists them
	// (0x1b8 onwards).
	virtual bool isPichiEffect();
	virtual BOOL isJumpBck();
	virtual BOOL isDeadBck();
	virtual BOOL isJumpStartBck();
	virtual BOOL isAttackBck();
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

	// Not virtual: the map records no vtable slot for it.
	void setDeadAnm();

	/* 0x194 */ u8 unk194[0x1E0 - 0x194];
	/* 0x1E0 */ f32 mSwimBaseY;
};

class TMoePuku : public TTobiPuku {
public:
	TMoePuku(const char* name)
	    : TTobiPuku(name)
	{
	}

	virtual ~TMoePuku();

	// Overrides of base virtuals; these occupy inherited slots.
	virtual void initAttacker(THitActor*);
	virtual BOOL isInhibitedForceMove();
	virtual void generateEffectColumWater();
	virtual void walkBehavior(int, f32);

	// New virtuals, declared in the order the vtable lists them
	// (0x1b8 onwards).
	virtual bool isPichiEffect();
	virtual BOOL isJumpBck();
	virtual BOOL isDeadBck();
	virtual BOOL isJumpStartBck();
	virtual BOOL isAttackBck();
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

	// Not virtual: the map records no vtable slot for it.
	void setDeadAnm();
};

class TPukuPuku : public TTobiPuku {
public:
	TPukuPuku(const char* name)
	    : TTobiPuku(name)
	{
	}

	virtual ~TPukuPuku();
};

class TTobiPukuLaunchPad : public TSmallEnemy {
public:
	TTobiPukuLaunchPad(const char* name)
	    : TSmallEnemy(name)
	{
	}

	virtual ~TTobiPukuLaunchPad();
};

class TMoePukuLaunchPad : public TTobiPukuLaunchPad {
public:
	TMoePukuLaunchPad(const char* name)
	    : TTobiPukuLaunchPad(name)
	{
	}

	virtual ~TMoePukuLaunchPad();
};

class TTobiPukuManager : public TSmallEnemyManager {
public:
	TTobiPukuManager(const char* name)
	    : TSmallEnemyManager(name)
	{
	}

	virtual ~TTobiPukuManager();
};

class TMoePukuManager : public TTobiPukuManager {
public:
	TMoePukuManager(const char* name)
	    : TTobiPukuManager(name)
	{
	}

	virtual ~TMoePukuManager();
};

class TTobiPukuLaunchPadManager : public TSmallEnemyManager {
public:
	TTobiPukuLaunchPadManager(const char* name)
	    : TSmallEnemyManager(name)
	{
	}

	virtual ~TTobiPukuLaunchPadManager();
};

class TMoePukuLaunchPadManager : public TTobiPukuLaunchPadManager {
public:
	TMoePukuLaunchPadManager(const char* name)
	    : TTobiPukuLaunchPadManager(name)
	{
	}

	virtual ~TMoePukuLaunchPadManager();
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
