#ifndef ROCKET_H
#define ROCKET_H

#include <Enemy/EnemyManager.hpp>
#include <Enemy/Enemy.hpp>
#include <Enemy/SmallEnemy.hpp>
#include <Strategic/Strategy.hpp>
#include <Map/MapData.hpp>

class TRocket : public TSmallEnemy {
public:
	TRocket(const char* name = "ロケット");
	virtual ~TRocket() { }

	// TODO: add other methods
	bool isAttack();

public:
	// static members (map: .sdata)
	static f32 mTestAng_y;
	static f32 mNozzleOffsetZ;
	static f32 mColOffsetY;
};

#endif
