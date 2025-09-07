#include "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/math.h"
#include "dolphin/types.h"

#define __PI_O2 ((f32)M_PI / 2)

f32 atanf(f32);
f32 _inv_sqrtf(f32);
f32 atan__Ff(f32);

f32 atan2f(f32 __y, f32 __x)
{
	if (signbit(__x) == signbit(__y)) {
		if (signbit(__x) != 0) {
			return atanf(__y / __x) - PI;
		} else if (__x) {
			return atanf(__y / __x);
		} else {
			return HALF_PI;
		}
	} else if (__x < 0.0f) {
		return PI + atanf(__y / __x);
	} else if (__x) {
		return atanf(__y / __x);
	}
	__HI(__y) = signbit(__y) + 0x3fc90fdb; // PI / 2

	return __y;
}

f32 acosf(f32 x) { return HALF_PI - atan__Ff(x * _inv_sqrtf(1.0f - x * x)); }

#pragma dont_inline on
__declspec(weak) f32 _inv_sqrtf(f32 x)
{
	const f32 _half  = .5f;
	const f32 _three = 3.0f;

	if (x > 0.0f) {
		f32 guess = __frsqrte((f64)x); /* returns an approximation to  */
		guess     = _half * guess
		        * (_three - guess * guess * x); /* now have 8  sig bits */
		guess = _half * guess
		        * (_three - guess * guess * x); /* now have 16 sig bits */
		guess = _half * guess
		        * (_three - guess * guess * x); /* now have >24 sig bits */
		return guess;
	} else if (x) {
		return NAN;
	}
	return HUGE_VALF;
}

/*
 * --INFO--
 * Address:	8021B924
 * Size:	000020
 */
__declspec(weak) f32 atan__Ff(f32 x) { return atanf(x); }

#pragma dont_inline reset
/*
 * --INFO--
 * Address:	8021B730
 * Size:	0001F4
 */
f32 atanf(f32 x)
{
	f32 z, z_square;
	int index = -1, inv = 0;
	const int sign = (*(int*)&x) & 0x80000000;

	/* poly # 4964-- poly for  [0,tan(pi/8)]  */

	static const f32 atan_coeff[]
	    = { .999999999f,   -.3333333213f,   .19999886356f, -.14281650536f,
		    .11041179874f, -.084597554152f, .04714243524f /*.76f */ };

	/* data for argument reduction  */
	// static const f32  one_over_xi[]={2.414213562f, 1.49660575f,1.00000000f,
	//                                    .668178618f, .414213568f,.198912367f};

	static const f32 onep_one_over_xisqr_hi[]
	    = { 6.82842f,  3.239828f,   2.0f, /*   6.828427125f  */
		    1.446462f, 1.17157292f, 1.039566130f };

	static const f32 onep_one_over_xisqr_lo[]
	    = { .000007135f, 0.00000082f, .0f, 0.00000063f, 0.0f, 0.0f };

	static const f32 atan_xi_hi[]
	    = { 0.0f,     .39269f,   .5890486f, .7853981f, /* .3926990817f  */
		    .981747f, 1.178097f, 1.374446f };
	static const f32 atan_xi_lo[]
	    = { 0.0f,        .000009081698724f, .000000023f, .000000063f,
		    .000000704f, .00000025f,        .00000079f };

	static const f32 one_over_xi_hi[]
	    = { 2.414213f,   1.49660575f, 1.00000000f,
		    .668178618f, .414213568f, .198912367f };

	static const f32 one_over_xi_lo[]
	    = { .000000562f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

	(*(int*)&x) &= 0x7fffffff; /*  |x| */

	if (x >= 2.414213565f) /* x is <= 1.0 */ // 0x401A827A
	{
		z = 1.0f / x;
		inv++;
	} else if (.4142135624f < x) // 0x3ED413CD  //(4.14213583e-01
	{
		index++; // index is now 0
		switch (__HI(x) & 0x7f800000) {
		case 0x3F000000: /* .5  <= x < 1 */
			if ((*(int*)&x) >= 0x3F08D5B9)
				index++; // 0.5345111 == tan(5pi/32)
			if ((*(int*)&x) >= 0x3F521801)
				index++; // 0.8206788
			break;
			break;
		case 0x3F800000: /* 1 <= x < 2    */
			index += 2;
			if ((*(int*)&x) >= 0x3F9bf7ec)
				index++; // 1.2185035
			if ((*(int*)&x) >= 0x3FEF789E)
				index++; // 1.8708684
			break;
		case 0x40000000: /* 2 <= x <  2.414213565f   */
			index += 4;
			break;
		}

		z = 1.0f / (one_over_xi_hi[index] + (one_over_xi_lo[index] + x));
		z = (one_over_xi_hi[index] - onep_one_over_xisqr_hi[index] * z)
		    + (one_over_xi_lo[index] - onep_one_over_xisqr_lo[index] * z);

	} else
		z = x;

	z_square = z * z;
	z += z * z_square
	     * (atan_coeff[1]
	        + z_square
	              * (atan_coeff[2]
	                 + z_square
	                       * (atan_coeff[3]
	                          + z_square
	                                * (atan_coeff[4]
	                                   + z_square
	                                         * (atan_coeff[5]
	                                            + z_square * atan_coeff[6])))));
	z += atan_xi_lo[index + 1];
	z += atan_xi_hi[index + 1];

	if (inv) {
		z -= __PI_O2;
		if (sign)
			return z;

		return -z;
	}

	(*(int*)&z) |= sign;
	return z;
}
