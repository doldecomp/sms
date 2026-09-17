#ifndef ENEMY_CANNON_HPP
#define ENEMY_CANNON_HPP

#include <Enemy/SmallEnemy.hpp>

// fabricated: declaration recovered from mario.MAP statics, layout unknown

class TCannon : public TSmallEnemy {
public:
	TCannon(const char*); // declared only: suppresses the implicit default
	                      // constructor
	// static members (map: .sdata)
	static u8 mChorobeiJntIdx;
	static u8 mChorobeiHandJntIdx;
	static f32 mVelocityRate;
	static f32 mSearchRate;
};

#endif
