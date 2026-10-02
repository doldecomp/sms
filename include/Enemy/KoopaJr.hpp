#ifndef ENEMY_KOOPAJR_HPP
#define ENEMY_KOOPAJR_HPP

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

#endif
