#ifndef MSL_MATH_H_
#define MSL_MATH_H_

#include "float.h"

#define NAN       (*(float*)__float_nan)
#define HUGE_VALF (*(float*)__float_huge)

#define LONG_TAU   6.2831854820251465
#define TAU        6.2831855f
#define HALF_PI    1.5707964f
#define THIRD_PI   1.0471976f
#define QUARTER_PI 0.7853982f
#define SIN_2_5    0.43633234f
#define M_SQRT3    1.73205f
#define M_PI       3.14159265358979323846f

#define DEG_TO_RAD(degrees) (degrees * (M_PI / 180.0f))
#define RAD_TO_DEG(radians) (radians * (180.0f / M_PI))

#ifdef __cplusplus
extern "C" {
#endif

int abs(int);
double acos(double);
float acosf(float);
double asin(double);
double atan(double);
float atanf(float);
double atan2(double, double);
float atan2f(float, float);
double ceil(double);
double copysign(double, double);
double cos(double);
float cosf(float);
double exp(double);
float expf(float);

extern double __fabs(double);
extern float __fabsf(float);
inline double fabs(double f) { return __fabs(f); }
inline float fabsf(float f) { return __fabsf(f); }

double __frsqrte(double);
float __fres(float);

double floor(double);
double fmod(double, double);

double frexp(double, int*);
double ldexp(double, int);
double modf(double, double*);
double pow(double, double);
float powf(float, float);
double sin(double);
float sinf(float);
double tan(double);
float tanf(float);

extern inline double sqrt(double x)
{
	if (x > 0.0) {
		double guess = __frsqrte(x); /* returns an approximation to    */
		guess
		    = .5 * guess * (3.0 - guess * guess * x); /* now have 8 sig bits */
		guess
		    = .5 * guess * (3.0 - guess * guess * x); /* now have 16 sig bits */
		guess
		    = .5 * guess * (3.0 - guess * guess * x); /* now have 32 sig bits */
		guess = .5 * guess
		        * (3.0 - guess * guess * x); /* now have > 53 sig bits        */
		return x * guess;
	} else if (x == 0)
		return 0;
	else if (x)
		return NAN;

	return HUGE_VALF;
}

/* Audited against the map: sqrtf is the *only* float entry point that MSL
 * defines in this header and that C translation units therefore need a body
 * for. The others resolve as follows.
 *
 *   sinf/cosf/tanf  real out-of-line globals (trigf.c, linked at 0x8033c7e4,
 *                   0x8033c650, 0x8033c5cc); the prototypes above are all a C
 *                   unit needs. GXDraw.c, mtx.c and mtx44.c match on them.
 *   atan2f          same, inverse_trig.c at 0x8033c4f4.
 *   powf            same, exponentialsf.c at 0x8033c9b8.
 *   fmodf           never emitted unqualified; the map's only copy is the weak
 *                   fmodf__3stdFff in wireTrap.cpp, so no C unit ever sees it,
 *                   and `fmod` itself is a prototype with no definition
 *                   anywhere in the image.
 *   fabsf / fabs    already declared inline outside this guard, and the map
 *                   proves C units get them: fabsf__Ff is weak in
 *                   hyperbolicsf.c and fabs__Fd weak in e_asin.c, both .c
 *                   files, both matching.
 *
 * So there is nothing else to move out of namespace std. */
#ifndef __cplusplus
#define _MSL_HAS_SQRTF
/* In C++ this lives in namespace std (MAnmSound.cpp carries the weak copy).
 * C has no namespaces, so the same body is a plain global there, and that is
 * how the map records it: hx_wiper.c's local statics are named
 * _half$localstatic0$sqrtf__Ff and _three$localstatic1$sqrtf__Ff, with no std
 * qualifier. */
extern inline float sqrtf(float x)
{
	const double _half  = .5;
	const double _three = 3.0;
	volatile float y;
	if (x > 0.0f) {
		double guess = __frsqrte((double)x);
		guess        = _half * guess * (_three - guess * guess * x);
		guess        = _half * guess * (_three - guess * guess * x);
		guess        = _half * guess * (_three - guess * guess * x);
		y            = (float)(x * guess);
		return y;
	}
	return x;
}
#endif

#ifdef __cplusplus
};

// Yes, they really had additional overloaded wrappers for when C++ is enabled.
inline float abs(float x) { return fabsf(x); }
inline double abs(double x) { return fabs(x); }
inline float sin(float x) { return sinf(x); }
inline float cos(float x) { return cosf(x); }
inline float atan2(float x, float y) { return atan2f(x, y); }

namespace std {
inline float fabsf(float f) { return ::fabsf(f); }
inline float abs(float f) { return ::fabs(f); }
// The ROM never inlines this: wireTrap.cpp carries the surviving weak 0x5c
// copy and koopajr.cpp, Koopa.cpp, MapObjCorona.cpp and BathtubPeach.cpp all
// call it. That copy's body is the same as JGeometry::TUtil<f32>::mod:
//
//     if (::fabsf(y) > ::fabsf(x))
//         return x;
//     return x - y * (float)(long long)(unsigned long long)(x / y);
//
// which compiles to exactly 0x5c under our flags. It is *not* spelled here,
// because writing it makes MWCC inline it at every call site we have and every
// probe gets worse (TNervePeachEscape::execute 93.5% -> 79.0%).
//
// TODO: the blocker is inline depth, not the spelling. Measured in a scratch TU
// with the game flags (-O4,p -inline auto,deferred), varying only the callee's
// compiled size and the number of inline wrappers between it and a real
// function:
//
//     body size | depth 2 | depth 3 | depth 4
//     ----------+---------+---------+--------
//       0x5c    | inline  | inline  | CALL
//       0x6c    | inline  | inline  | CALL
//       0x74    | inline  | CALL    | CALL
//       0x84    | inline  | CALL    | CALL
//       0x8c    | CALL    | CALL    | CALL
//
// So MWCC's per-expansion size allowance shrinks with depth, and a 0x5c body is
// only refused from depth four down. Everything that could be changed here was
// tried and none of it moves the decision: plain `inline`, `extern inline`, the
// body split into three, five or seven statements, and a named `long long`
// temporary. Caller size is irrelevant too -- a 0x7bf8 caller still inlines it
// at depth one.
//
// The fix therefore is not in this header: our call sites reach std::fmodf at
// depth two (nerve -> faceTo -> std::fmodf), and the ROM's reach it at depth
// four, so two inline wrappers are missing above it. The shape of those
// wrappers is visible in the ROM: every site computes
// `l + std::fmodf((r - l) + (t - l), r - l)`, i.e. a wrap-into-[l,r) helper
// distinct from MathUtil.hpp's loop-based MsWrap<f> (0x48, Animal/boid.o).
// Recovering that helper pair belongs with the four .cpp files that call it.
// #pragma dont_inline is not an acceptable stand-in.
inline float fmodf(float x, float y) { return ::fmod(x, y); }
inline float atan2f(float y, float x) { return ::atan2((double)y, (double)x); }
inline float sinf(float x) { return ::sin((double)x); }
inline float cosf(float x) { return ::cos((double)x); }
inline float tanf(float x) { return ::tan((double)x); }
inline float powf(float e, float x) { return ::powf(e, x); }

inline float fabs(float x)
{
	(*(unsigned int*)&x) &= 0x7fffffff;

	return x;
}

extern inline float sqrtf(float x)
{
	const double _half  = .5;
	const double _three = 3.0;
	volatile float y;
	if (x > 0.0f) {
		double guess = __frsqrte((double)x); // returns an approximation to
		guess        = _half * guess
		        * (_three - guess * guess * x); // now have 12 sig bits
		guess = _half * guess
		        * (_three - guess * guess * x); // now have 24 sig bits
		guess = _half * guess
		        * (_three - guess * guess * x); // now have 32 sig bits
		y = (float)(x * guess);
		return y;
	}
	return x;
}
}; // namespace std
#endif

#endif
