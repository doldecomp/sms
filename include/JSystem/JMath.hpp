#ifndef JMATH_HPP
#define JMATH_HPP

#include <dolphin/mtx.h>

namespace JMath {

class TRandom_fast_ {
public:
	TRandom_fast_(u32);

	inline u32 next()
	{
		value = value * 0x19660d + 0x3c6ef35f;
		return value;
	}

	void get();
	void get_ufloat_1();

private:
	u32 value;
};

} // namespace JMath

void JMANewSinTable(u8);
void JMADeleteSinTable();
void JMAEulerToQuat(s16, s16, s16, Quaternion*);
void JMAQuatLerp(Quaternion*, Quaternion*, f32, Quaternion*);
f32 JMAHermiteInterpolation(f32 frame, f32 time0, f32 value0, f32 tangent0,
                            f32 time1, f32 value1, f32 tangent1);
void JMALagrangeInterpolation(int, f32*, f32*, f32);

#endif
