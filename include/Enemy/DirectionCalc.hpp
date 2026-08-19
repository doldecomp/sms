#ifndef ENEMY_DIRECTION_CALC_HPP
#define ENEMY_DIRECTION_CALC_HPP

#include <JSystem/JGeometry/JGVec3.hpp>
#include <dolphin/types.h>

class TDirectionCalc {
public:
	TDirectionCalc();
	TDirectionCalc(f32);
	TDirectionCalc(JGeometry::TVec3<f32>);

	static f32 r2d(f32);
	static f32 d2r(f32);

	f32 absDirection(f32);
	JGeometry::TVec3<f32> calcDirectionVector();
	void makeDirection(JGeometry::TVec3<f32>);
	f32 calcTurnDirection(f32, f32);
	f32 calcNearerDirection(f32);
	f32 sub(f32);

	f32 mDirection;
};

#endif
