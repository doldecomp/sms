#include <Enemy/KoopaJr.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <math.h>

TDirectionCalc::TDirectionCalc()
    : unk0(0.0f)
{
}

TDirectionCalc::TDirectionCalc(f32 direction)
    : unk0(direction)
{
}

TDirectionCalc::TDirectionCalc(JGeometry::TVec3<f32> direction)
{
	makeDirection(direction);
}

void TDirectionCalc::normalize() { unk0 = MsRepeat(unk0, 0.0f, TAU); }

f32 TDirectionCalc::calcNearerDirection(f32 direction)
{
	normalize();
	if (direction >= unk0) {
		f32 difference = direction - unk0;
		if (TAU - difference < difference)
			direction -= TAU;
	} else {
		f32 difference = unk0 - direction;
		if (TAU - difference < difference)
			direction += TAU;
	}
	return direction;
}

f32 TDirectionCalc::sub(f32 direction)
{
	return unk0 - calcNearerDirection(direction);
}

f32 TDirectionCalc::calcTurnDirection(f32 direction, f32 turn_speed)
{
	normalize();
	direction = calcNearerDirection(direction);
	f32 step  = turn_speed;
	if (direction > unk0) {
		if (direction - unk0 < turn_speed)
			step = direction - unk0;
		return unk0 + step;
	} else {
		if (unk0 - direction < turn_speed)
			step = unk0 - direction;
		return unk0 - step;
	}
}

void TDirectionCalc::makeDirection(JGeometry::TVec3<f32> direction)
{
	unk0 = atan2(direction.x, direction.z);
}

JGeometry::TVec3<f32> TDirectionCalc::calcDirectionVector()
{
	return JGeometry::TVec3<f32>(sin(unk0), 0.0f, cos(unk0));
}

f32 TDirectionCalc::absDirection(f32 direction)
{
	f32 difference = unk0 - calcNearerDirection(direction);
	return abs(difference);
}

f32 TDirectionCalc::d2r(f32 direction)
{
	return direction * JGeometry::TUtil<f32>::PI() / 180.0f;
}

f32 TDirectionCalc::r2d(f32 direction) { return 180.0f * direction / M_PI; }
