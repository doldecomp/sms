#ifndef ENEMY_TOBI_PUKU_HPP
#define ENEMY_TOBI_PUKU_HPP

#include <Enemy/WalkerEnemy.hpp>

// fabricated: declaration recovered from mario.MAP statics, layout unknown

class TTobiPuku : public TWalkerEnemy {
public:
	TTobiPuku(const char*); // declared only: suppresses the implicit default constructor
	// static members (map: .sdata)
	static f32 mLandAngle;
	static bool mBoundSw;
	static f32 mBoundVelocityY;
	static bool mReturnLaunchSw;
};

#endif
