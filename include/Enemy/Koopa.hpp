#ifndef ENEMY_KOOPA_HPP
#define ENEMY_KOOPA_HPP

#include <Enemy/Enemy.hpp>

class TKoopa : public TSpineEnemy {
public:
	TKoopa(const char*);

	void fall();
	void stagger(bool);
	void getDown();
	bool allowsLaunch() const;
};

#endif
