#include <JSystem/JAudio/JALibrary/JALCalc.hpp>
#include <JSystem/JMath.hpp>
#include <math.h>
namespace JALCalc {

const f32 cEqualCSlope = 1.0f;
const f32 cPlusPSlope  = 1.0f;

f32 linearTransform(f32 x, f32 xStart, f32 xEnd, f32 yStart, f32 yEnd,
                    bool unbounded)
{
	f32 result = ((x - xStart) * ((yEnd - yStart) / (xEnd - xStart)) + yStart);

	if (unbounded)
		return result;

	if (yStart < yEnd) {
		return result > yEnd ? yEnd : (result < yStart ? yStart : result);
	} else {
		return result > yStart ? yStart : (result < yEnd ? yEnd : result);
	}
}

f32 getParamByExp(f32 x, f32 xStart, f32 xEnd, f32 y, f32 yStart, f32 yEnd,
                  CurveSign curve)
{
	f32 result;
	if (curve == CS_POSITIVE_CURVE) {
		f32 newX = expf(linearTransform(x, xStart, xEnd, 0.0f, y, true));
		result   = linearTransform(newX, 1.0f, expf(y), yStart, yEnd, true);
	} else if (curve == CS_NEGATIVE_CURVE) {
		f32 newX = expf(linearTransform(x, xStart, xEnd, y, 0.0f, true));
		result   = linearTransform(newX, expf(y), 1.0f, yStart, yEnd, true);
	} else {
		result = linearTransform(x, xStart, xEnd, yStart, yEnd, false);
	}

	if (result > yEnd) {
		return yEnd;
	}

	if (result < yStart) {
		return yStart;
	}

	return result;
}

f32 getRandom(f32 p1, f32 p2, f32 p3)
{
	f32 val0 = 2.0f * p3;
	f32 val1 = -2.0f * (1.0f - p3);

	f32 val2 = getRandom_0_1() < p3 ? val0 : val1;

	p1 *= val2;

	f32 val3 = powf(getRandom_0_1(), p2);
	return val3 * p1;
}

f32 getRandom_0_1()
{
	static JMath::TRandom_fast_ oRandom(0);
	u32 next = (oRandom.next() >> 9) | 0x3F800000;
	return *(f32*)(void*)&next - 1.0f;
}

f32 getDist(Vec* vec1, Vec* vec2) { return std::sqrtf(getDistPow(vec1, vec2)); }

f32 getDistPow(Vec* vec1, Vec* vec2)
{
	if (vec2) {
		float dy = powf(vec1->y - vec2->y, 2.0f);
		float dx = powf(vec1->x - vec2->x, 2.0f);
		float dz = powf(vec1->z - vec2->z, 2.0f);
		return dx + dy + dz;
	} else {
		float dy = std::powf(vec1->y, 2.0f);
		float dx = std::powf(vec1->x, 2.0f);
		float dz = std::powf(vec1->z, 2.0f);
		return dx + dy + dz;
	}
}

} // namespace JALCalc
