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

	u32 get()
	{
		value = (value * 0x19660d) + 0x3c6ef35f;
		return value;
	}

	f32 get_ufloat_1()
	{
		union {
			f32 f;
			u32 s;
		} out;
		out.s = (get() >> 9) | 0x3f800000;
		return out.f - 1.0f;
	}

public:
	u32 value;
};

class TRandom_enough_ {
public:
	TRandom_enough_(u32 seed) { setSeed(seed); }

	void setSeed(u32);
	u32 get();

private:
	/* 0x0 */ u32 unk0;
	/* 0x4 */ u32 unk4;
	/* 0x8 */ u32 unk8[32];
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
