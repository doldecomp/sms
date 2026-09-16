#ifndef ENEMY_KOOPA_HPP
#define ENEMY_KOOPA_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/KoopaNerve.hpp>

// TODO: fabricated stub; only what koopajr.cpp touches is declared here.
class TKoopa : public TSpineEnemy {
public:
	TKoopa(const char*);

	f32 getFlameDirDegree() const;
	BOOL isFlaming() const;
};

#endif
