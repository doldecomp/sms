#ifndef ENEMY_ELEC_NOKONOKO_HPP
#define ENEMY_ELEC_NOKONOKO_HPP

#include <Enemy/WalkerEnemy.hpp>

// fabricated: declaration recovered from mario.MAP statics, layout unknown

class TElecNokonoko : public TWalkerEnemy {
public:
	TElecNokonoko(const char*); // declared only: suppresses the implicit default constructor
	// static members (map: .sdata)
	static u8 mReflectSw;
};

#endif
