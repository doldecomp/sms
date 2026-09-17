#ifndef ENEMY_KILLER_HPP
#define ENEMY_KILLER_HPP

#include <Enemy/WalkerEnemy.hpp>

// fabricated: declaration recovered from mario.MAP statics, layout unknown

class TFlyEnemy : public TWalkerEnemy {
public:
	TFlyEnemy(const char*); // declared only: suppresses the implicit default
	                        // constructor
	// static members (map: .sdata)
	static f32 mTestSp;
	static s32 mInvalidTime;
	static f32 mTestMarioSpMax;
};

// fabricated: declaration recovered from mario.MAP statics, layout unknown

class TKiller : public TFlyEnemy {
public:
	TKiller(const char*); // declared only: suppresses the implicit default
	                      // constructor
	// static members (map: .sdata)
	static u8 mSerialBomb;
	static u8 mTrampleDie;
};

#endif
