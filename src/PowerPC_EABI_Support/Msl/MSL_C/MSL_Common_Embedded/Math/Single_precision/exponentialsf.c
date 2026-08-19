#include <PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/math.h>

#ifndef __INFINITY
static const signed int _inf = 0x7f800000;
#define __INFINITY (*(float*)&_inf)

static const signed int _nan = 0x7fffffff;
#define __NAN (*(float*)&_nan)
#endif

static const float __log2_F[] = {
	-3.75000000e-01, -3.63772745e-01, -3.52632187e-01, -3.41576998e-01,
	-3.30605881e-01, -3.19717564e-01, -3.08910810e-01, -2.98184403e-01,
	-2.87537159e-01, -2.76967917e-01, -2.66475543e-01, -2.56058927e-01,
	-2.45716983e-01, -2.35448648e-01, -2.25252880e-01, -2.15128663e-01,
	-2.05074999e-01, -1.95090910e-01, -1.85175441e-01, -1.75327655e-01,
	-1.65546634e-01, -1.55831480e-01, -1.46181310e-01, -1.36595261e-01,
	-1.27072487e-01, -1.17612157e-01, -1.08213459e-01, -9.88755947e-02,
	-8.95977811e-02, -8.03792511e-02, -7.12192518e-02, -6.21170447e-02,
	-5.30719051e-02, -4.40831219e-02, -3.51499971e-02, -2.62718458e-02,
	-1.74479954e-02, -8.67778575e-03, 3.94313469e-05,  8.70429247e-03,
	1.73174228e-02,  2.58794363e-02,  3.43909361e-02,  4.28525149e-02,
	5.12647547e-02,  5.96282276e-02,  6.79434958e-02,  7.62111118e-02,
	8.44316186e-02,  9.26055501e-02,  1.00733431e-01,  1.08815777e-01,
	1.16853096e-01,  1.24845887e-01,  1.32794640e-01,  1.40699838e-01,
	1.48561956e-01,  1.56381461e-01,  1.64158811e-01,  1.71894460e-01,
	1.79588852e-01,  1.87242424e-01,  1.94855608e-01,  2.02428828e-01,
	2.09962501e-01,  2.17457037e-01,  2.24912842e-01,  2.32330314e-01,
	2.39709844e-01,  2.47051819e-01,  2.54356620e-01,  2.61624621e-01,
	2.68856190e-01,  2.76051691e-01,  2.83211483e-01,  2.90335917e-01,
	2.97425342e-01,  3.04480100e-01,  3.11500527e-01,  3.18486957e-01,
	3.25439718e-01,  3.32359132e-01,  3.39245518e-01,  3.46099189e-01,
	3.52920455e-01,  3.59709620e-01,  3.66466986e-01,  3.73192850e-01,
	3.79887502e-01,  3.86551232e-01,  3.93184325e-01,  3.99787060e-01,
	4.06359714e-01,  4.12902559e-01,  4.19415866e-01,  4.25899900e-01,
	4.32354922e-01,  4.38781191e-01,  4.45178962e-01,  4.51548487e-01,
	4.57890014e-01,  4.64203788e-01,  4.70490051e-01,  4.76749041e-01,
	4.82980995e-01,  4.89186145e-01,  4.95364720e-01,  5.01516947e-01,
	5.07643049e-01,  5.13743249e-01,  5.19817763e-01,  5.25866808e-01,
	5.31890596e-01,  5.37889336e-01,  5.43863237e-01,  5.49812504e-01,
	5.55737338e-01,  5.61637939e-01,  5.67514505e-01,  5.73367232e-01,
	5.79196310e-01,  5.85001932e-01,  5.90784285e-01,  5.96543554e-01,
	6.02279923e-01,  6.07993575e-01,  6.13684687e-01,  6.19353437e-01,
	6.25000000e-01,
};

static const float __two_to_x[] = {
	0.693147182464599609375,     0.240226507186889648438,
	0.0555041581392288208008,    0.00961813423782587051392,
	0.00133318256121128797531,   0.000154019769979640841484,
	0.0000154832741827704012394, 0.0000013392817663770983927,
	1.02999983653262461303e-7,
};

// used in the expf and powf
extern const float __one_over_F[129];

extern const float __two_to_log2e_m1_tI[178];

#define V_EXPF_MAX (88.72284f)
#define V_EXPF_MIN (-87.33655f)

// TODO: nonmatching, instruction stream is exact but FP registers are
// renumbered: the target keeps the long-lived values (constants, pow2,
// table element) in f6-f9 while we get f0-f4, so the Horner chain temps
// start at f9 instead of f5. Source-order and const-local/inline-literal
// permutations were tried and do not flip the allocation.
float expf(float x)
{
	static const float __exp_to_x[] = {
		0.999999880790710449219f,    0.499999970197677612305f,
		0.166667982935905456543f,    0.0416668877005577087402f,
		0.00832859613001346588135f,  0.00138827599585056304932f,
		0.000204699928872287273407f, 0.0000254991846304619684815f,
	};

	const float almost_recip_128     = 0.007812501f; // close to 1.f / 128.f
	const float compliment_recip_128 = (1.f - (1.f / 128.f));

	int int_x, int_x_index;
	float x_fract, estimate, pow2, finalVal;
	if (x > V_EXPF_MAX)
		return __INFINITY;
	if (x < V_EXPF_MIN)
		return 0.0f;

	int_x       = (int)x;
	int_x_index = int_x + 88; // halfway index of __two_to_log2e_m1_tI
	// not really __HI cuz its not a double, kinda just *(int*)&pow2
	__HI(pow2) = (int_x_index + 39) << 23; // essentially ((int)x + 127) << 23,
	                                       // results in (float)2**(int)x
	x_fract = x - (float)(int_x);

	// Horner expansion
	estimate = __exp_to_x[7];
	estimate = x_fract * estimate + __exp_to_x[6];
	estimate = x_fract * estimate + __exp_to_x[5];
	estimate = x_fract * estimate + __exp_to_x[4];
	estimate = x_fract * estimate + __exp_to_x[3];
	estimate = x_fract * estimate + __exp_to_x[2];
	estimate = x_fract * estimate + __exp_to_x[1];
	estimate = x_fract * estimate + __exp_to_x[0];
	finalVal = x_fract * estimate;

	return __two_to_log2e_m1_tI[int_x_index]
	       * (pow2 * (finalVal + almost_recip_128 + compliment_recip_128));
}

#pragma cplusplus on
// this name is mangled so enable c++ mode
inline float __log2f(float x)
{
	// inferred by presence of this var's mangled name
	static const float __log2e_m1[2] = { 0.41015625f, 0.03253879088896f };

	float unkConsts[2] = { -0.72135162353515625f, 0.4808933f };

	float frac, r;
	int exp, index;
	int hi_bits, lo_bits;
	int lo_mask;

	lo_mask = *(int*)&x & 0x7FFFFF;
	exp     = ((unsigned int)*(int*)&x >> 23) - 0x80;
	index   = ((unsigned int)*(int*)&x >> 16) & 0x7F;

	if ((unsigned short)*(int*)&x != 0) {
		hi_bits = (*(int*)&x & 0x7F0000) | 0x3F800000;
		lo_bits = lo_mask | 0x3F800000;

		if (*(int*)&x & 0x8000) {
			index++;
			hi_bits += 0x10000;
		}

		frac = (*(float*)&lo_bits - *(float*)&hi_bits) * __one_over_F[index];

		r = 1.375f + (float)exp
		    + (__log2_F[index]
		       + (frac
		          + ((__log2e_m1[0] * frac)
		             + ((__log2e_m1[1] * frac)
		                + (frac * frac
		                   * ((frac * unkConsts[1]) + unkConsts[0]))))));
	} else {
		r = 1.375f + (float)exp + __log2_F[index];
	}

	return r;
}

// fabricated
inline float __exp2f(float f)
{
	// assumed presence due to how powf tends to work, also seems like theres
	// stack padding
	float p;

	p = __two_to_x[8];
	p = f * p + __two_to_x[7];
	p = f * p + __two_to_x[6];
	p = f * p + __two_to_x[5];
	p = f * p + __two_to_x[4];
	p = f * p + __two_to_x[3];
	p = f * p + __two_to_x[2];
	p = f * p + __two_to_x[1];
	p = f * p + __two_to_x[0];

	p = f * p;
	return 0.75f + (0.25f + p);
}

#pragma cplusplus off

// TODO: nonmatching, every opcode matches but the same FP renumbering as
// expf remains (via the __log2f/__exp2f inlines) plus a 0x10 stack frame
// excess (0xa0 here vs 0x90 in the target).
float powf(float x, float y)
{
	int iy, n;
	float t, f, r;
	int cx, cy;

	/* x > 0 */
	if (x > 0.0f) {
		t = y * __log2f(x);

		n = (int)t;
		f = t - (float)n;

		if (n > 128)
			return __INFINITY;
		if (n < -127)
			return 0.0f;

		n = n + 127;
		n = n << 23;
		return *(float*)&n * __exp2f(f);
	}

	/* x < 0 */
	if (x < 0.0f) {
		iy = (int)y;
		if ((y - (float)(int)y) != 0.0f)
			return __NAN;
		if ((iy % 2) != 0) {
			t = y * __log2f(-x);

			n = (int)t;
			f = t - (float)n;

			if (n > 128)
				r = __INFINITY;
			else if (n < -127)
				r = 0.0f;
			else {
				n = n + 127;
				n = n << 23;
				r = *(float*)&n * __exp2f(f);
			}
			return -r;
		}

		t = y * __log2f(-x);

		n = (int)t;
		f = t - (float)n;

		if (n > 128)
			return __INFINITY;
		if (n < -127)
			return 0.0f;

		n = n + 127;
		n = n << 23;
		return *(float*)&n * __exp2f(f);
	}

	cx = fpclassify(x);
	if (cx == FP_QNAN)
		return x;

	cy = fpclassify(y);

	switch (cy) {
	case 3:
		return 1.0f;
	case 1:
	case 2:
		return __NAN;
	case 4:
	case 5:
		if (y < 0.0f) {
			if (x == -0.0f)
				return -__INFINITY;
			return __INFINITY;
		}
		break;
	default:
		break;
	}
	return 0.0f;
}
