#ifndef JMATH_HPP
#define JMATH_HPP

#include <dolphin/mtx.h>

#define DEG2SHORTANGLE(deg)   ((deg) * (65536.0f / 360.0f))
#define SHORTANGLE2DEG(angle) ((angle) * (360.0f / 65536.0f))

namespace JMath {

class TRandom_fast_ {
public:
	TRandom_fast_(u32);

	void setSeed(u32 seed) { value = seed; }

	u32 get()
	{
		value = (value * 0x19660d) + 0x3c6ef35f;
		return value;
	}

	u32 get_bit32() { return get(); }

	// NOTE: TP writes this with a union of an f32 and a u32. The union
	// spelling lets MWCC prove that the store to the float bits cannot
	// touch `value`, so back-to-back draws share one load of the seed. The
	// ROM reloads the seed after every store, which only the address-taken
	// local reproduces: taking its address makes the store opaque to the
	// alias analysis and keeps each draw's read separate. Compare
	// JPABaseEmitter::createChildParticle, which draws four times in a row.
	f32 get_ufloat_1()
	{
		u32 s = (get() >> 9) | 0x3f800000;
		return *(f32*)&s - 1.0f;
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

// Reconstructed from TP and TWW
template <class T> class TRandom_ : public T {
public:
	TRandom_(u32 seed)
	    : T(seed)
	{
	}

	f32 get_float(f32);
	// Both bodies come from JASystem::TInstRand::getY, whose frame carries one
	// dead temporary per inlined level: the chain
	// get_sfloat_1 -> get_ufloat -> get_ufloat_1 -> get is the only depth that
	// reproduces its stack layout. 0.9999999f is the largest f32 below 1.0f,
	// so get_sfloat_1 returns (-1, 1).
	f32 get_ufloat(f32 x) { return x * this->get_ufloat_1(); }
	f32 get_sfloat_1() { return this->get_ufloat(2.0f) - 0.9999999f; }
	u32 get_uint32(u32);
	u8 get_uint8(u8 param_1) { return param_1 * this->get_ufloat_1(); }

	// fabricated
	f32 get_float(f32 min, f32 max)
	{
		f32 f = (max - min) * this->get_ufloat_1();
		return min + f;
	}

	// fabricated
	f32 get_float01() { return get_float(0.0f, 1.0f); }
};

typedef TRandom_<TRandom_fast_> TRandomFast;

} // namespace JMath

extern u32 jmaSinShift;
extern f32* jmaSinTable;
extern f32* jmaCosTable;

inline f32 JMASCos(s16 v)
{
	return jmaCosTable[static_cast<u16>(v) >> jmaSinShift];
}

inline f32 JMASSin(s16 v)
{
	return jmaSinTable[static_cast<u16>(v) >> jmaSinShift];
}

inline f32 JMACos(f32 v) { return JMASCos(DEG2SHORTANGLE(v)); }

inline f32 JMASin(f32 v) { return JMASSin(DEG2SHORTANGLE(v)); }

bool JMANewSinTable(u8 numBits);
void JMADeleteSinTable();
void JMAEulerToQuat(s16 x, s16 y, s16 z, Quaternion* out);
void JMAQuatLerp(Quaternion* a, Quaternion* b, f32 t, Quaternion* out);
f32 JMAHermiteInterpolation(f32 frame, f32 time0, f32 value0, f32 tangent0,
                            f32 time1, f32 value1, f32 tangent1);
void JMALagrangeInterpolation(int, f32*, f32*, f32);

#endif
