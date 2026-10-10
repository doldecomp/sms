#ifndef ENEMY_KOOPAJR_HPP
#define ENEMY_KOOPAJR_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <JSystem/JGeometry/JGVec3.hpp>
#include <dolphin/types.h>

class TDirectionCalc {
public:
	TDirectionCalc(JGeometry::TVec3<f32>);
	TDirectionCalc(f32);
	TDirectionCalc();

	static f32 r2d(f32);
	static f32 d2r(f32);
	f32 absDirection(f32);
	JGeometry::TVec3<f32> calcDirectionVector();
	void makeDirection(JGeometry::TVec3<f32>);
	f32 calcTurnDirection(f32, f32);
	f32 sub(f32);
	f32 calcNearerDirection(f32);
	void normalize();

public:
	/* 0x0 */ f32 unk0;
};

// fabricated: declaration only, layout unknown
class TKoopaJr : public TSpineEnemy {
public:
	TKoopaJr(const char*);

public:
	// TODO: unknown layout, size from getNameRef_BossEnemy's operator new
	/* 0x150 */ u8 unk150[0x20];
};

// fabricated: declaration only, layout unknown beyond TEnemyManager
class TKoopaJrManager : public TEnemyManager {
public:
	TKoopaJrManager(const char*);
};

// fabricated: declaration only, layout unknown
class TKoopaJrSubmarine : public TSpineEnemy {
public:
	TKoopaJrSubmarine(const char*);

public:
	// TODO: unknown layout, size from getNameRef_BossEnemy's operator new
	/* 0x150 */ u8 unk150[0x5C];
};

// fabricated: declaration only, layout unknown beyond TEnemyManager
class TKoopaJrSubmarineManager : public TEnemyManager {
public:
	TKoopaJrSubmarineManager(const char*);
};

#endif
