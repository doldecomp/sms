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
// copy and MapObjCorona.cpp, limitkoopa.cpp, BathtubPeach.cpp, Koopa.cpp and
// koopajr.cpp all carry unreferenced duplicates of it. The body below is
// byte-exact against that copy (0x5c, 23 instructions, verified through
// objdiff on wireTrap.o), and it is the same computation as
// JGeometry::TUtil<f32>::mod. The named `unsigned long long quotient` is
// load-bearing: folding the conversion into the return expression gives the
// same 23 instructions with a 0x20 frame instead of the ROM's 0x28.
//
// Header round 13 replaced the old `return ::fmod(x, y);` forwarder with it.
// That forwarder was certainly wrong -- the map has no `fmod` symbol at all,
// so every site was emitting a `bl` to a function that does not exist in the
// image -- but it scored better at the sites where MWCC expands this body
// instead of calling it, so the switch costs two nonmatching functions some
// fuzzy score (TDirectionCalc::calcNearerDirection 97.3% -> 69.9%,
// TNervePeachEscape::execute 94.2% -> 93.9%) while improving eleven others
// and making this symbol exact.
//
// TODO: the open question is *why* the ROM never expands this body. It is not
// inline depth. TDirectionCalc::calcNearerDirection (koopajr.cpp, 0xa0, whose
// asm is the wrap written out with `lo`/`range` as separate literal loads)
// `bl`s it from depth one. Our build expands it at depth one and two and calls
// it from depth three, which is why the wrap helpers in wireTrap.cpp,
// MapObjCorona.cpp, BathtubPeach.cpp, KoopaNerve.hpp and koopajr.cpp are
// two levels deep: that is the only lever we have, and it is a stand-in, not
// the mechanism. Tried and ruled out for the declaration: `extern inline`
// (no change), a named `long long` temporary, the body split into three, five
// and seven statements, plain `inline`. Caller size is irrelevant. Something
// about the real declaration refuses expansion outright; find it and the two
// regressions above, plus the six remaining expanded sites, all close.
inline float fmodf(float x, float y)
{
	if (::fabsf(y) > ::fabsf(x))
		return x;
	unsigned long long quotient = (unsigned long long)(x / y);
	return x - y * (float)(long long)quotient;
}
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
