#include <JSystem/JParticle/JPAMath.hpp>
#include <JSystem/JMath.hpp>
#include <math.h>

f32 JPASqrtf(f32 x)
{
	if (x > 0.0f) {
		return x * (f32)__frsqrte(x);
	}
	return 0.0f;
}

// TODO: all these vec math funcs are equivalent (I think), but matching them
// will be quite the ordeal.

void JPAGetXYZRotateMtx(s16 x, s16 y, s16 z, MtxPtr dst)
{
	f32* ptr = &dst[0][0];

	*ptr++ = JMASCos(z) * JMASCos(y);
	*ptr++ = -JMASSin(z) * JMASCos(x) + JMASCos(z) * JMASSin(y) * JMASSin(x);
	*ptr++ = JMASSin(z) * JMASSin(x) + JMASCos(z) * JMASSin(y) * JMASCos(x);
	*ptr++ = 0.0f;

	*ptr++ = JMASSin(z) * JMASCos(y);
	*ptr++ = JMASCos(z) * JMASCos(x) + JMASSin(z) * JMASSin(y) * JMASSin(x);
	*ptr++ = -JMASCos(z) * JMASSin(x) + JMASSin(z) * JMASSin(y) * JMASCos(x);
	*ptr++ = 0.0f;

	*ptr++ = -JMASSin(y);
	*ptr++ = JMASCos(y) * JMASSin(x);
	*ptr++ = JMASCos(y) * JMASCos(x);
	*ptr++ = 0.0f;
}

void JPAGetXYRotateMtx(s16 x, s16 y, MtxPtr dst)
{
	f32* ptr = &dst[0][0];

	*ptr++ = JMASCos(y);
	*ptr++ = JMASSin(y) * JMASSin(x);
	*ptr++ = JMASSin(y) * JMASCos(x);
	*ptr++ = 0.0f;

	*ptr++ = 0.0f;
	*ptr++ = JMASCos(x);
	*ptr++ = -JMASSin(x);
	*ptr++ = 0.0f;

	*ptr++ = -JMASSin(y);
	*ptr++ = JMASCos(y) * JMASSin(x);
	*ptr++ = JMASCos(y) * JMASCos(x);
	*ptr++ = 0.0f;
}

void JPAGetScaleXYRotateMtx(JGeometry::TVec3<f32>&, s16, s16, MtxPtr) { }

void JPAGetYZRotateMtx(s16 y, s16 z, MtxPtr dst)
{
	f32* ptr = &dst[0][0];

	*ptr++ = JMASCos(z) * JMASCos(y);
	*ptr++ = -JMASSin(z);
	*ptr++ = JMASCos(z) * JMASSin(y);
	*ptr++ = 0.0f;

	*ptr++ = JMASSin(z) * JMASCos(y);
	*ptr++ = JMASCos(z);
	*ptr++ = JMASSin(z) * JMASSin(y);
	*ptr++ = 0.0f;

	*ptr++ = -JMASSin(y);
	*ptr++ = 0.0f;
	*ptr++ = JMASCos(y);
	*ptr++ = 0.0f;
}

void JPAGetScaleYZRotateMtx(JGeometry::TVec3<f32>&, s16, s16, MtxPtr) { }

void JPAGetYRotateMtx(s16 y, MtxPtr dst)
{
	f32* ptr = &dst[0][0];

	*ptr++ = JMASCos(y);
	*ptr++ = 0.0f;
	*ptr++ = JMASSin(y);
	*ptr++ = 0.0f;

	*ptr++ = 0.0f;
	*ptr++ = 1.0f;
	*ptr++ = 0.0f;
	*ptr++ = 0.0f;

	*ptr++ = -JMASSin(y);
	*ptr++ = 0.0f;
	*ptr++ = JMASCos(y);
	*ptr++ = 0.0f;
}

void JPAGetZRotateMtx(s16 z, MtxPtr dst)
{
	f32* ptr = &dst[0][0];

	*ptr++ = JMASCos(z);
	*ptr++ = JMASSin(z);
	*ptr++ = 0.0f;
	*ptr++ = 0.0f;

	*ptr++ = -JMASSin(z);
	*ptr++ = JMASCos(z);
	*ptr++ = 0.0f;
	*ptr++ = 0.0f;

	*ptr++ = 0.0f;
	*ptr++ = 0.0f;
	*ptr++ = 1.0f;
	*ptr++ = 0.0f;
}

// TODO: 62% and structurally open (batch 152).  Retail's `axis` is
// *memory-resident*: the cross product's three results are stored to
// 0x14..0x1c(r1), reloaded for the sum of squares, written again by the
// zero/scale branch and loaded once more for the matrix, which is why retail
// reloads a.y/b.y for the dot product (the stores invalidate them) and why
// x*y and z*x survive in f31/f30 across the dst stores.  Anything that keeps
// the vector in registers -- every spelling of cross(), a POD `Vec`, an
// `f32[3]`, an explicit `&axis` passed to a static helper -- costs those 19
// instructions.  Only a pointer variable aliasing the local (`f32* p =
// &axis.x`) reproduces it, and that plus named x/y/z locals and the products
// written inline gets to 111 instructions against retail's 109, with retail's
// sum of squares still contracted into two fmadds (so its operands are
// locals, not member reads: `axis.x * axis.x + ...` on a memory vector keeps
// three fmuls apart).  The `if (sin <= epsilon())` order below is confirmed
// (cror eq,lt,eq then bne, 56.9 -> 62.3).
// TODO: 62.3%.  The shape is right (cross, squared, dot, `TUtil<f32>::sqrt`,
// zero-or-scale, then the nine matrix products) but retail's `axis` lives in
// *memory* at 0x14..0x1f: it stores the three cross components, reloads them
// for `squared()`, zeroes them in z, y, x order and multiplies them in place
// in the `__fres` branch, while our `axis` is scalar-replaced into FPRs
// throughout (frame 0x38 against retail's 0x30).  Structural pass 167 confirmed
// that MWCC's scalar replacement is not blocked by taking the address through
// an inlined helper (a TU-local `static inline f32 f(TVec3<f32>*)` wrapping
// `squared()` changes nothing), so the blocker in retail is something else;
// the z, y, x zero order does match our `zero()` (`x = y = z = 0.0f` evaluates
// right to left), so the local really is a `TVec3<f32>` and not an array.
// Note that JGVec3.hpp's `cross` body is the tree-wide-measured shape, so the
// next step here is a mechanism for the memory residency, not a new `cross`.
// Also inert on the residency (still 62.3%): `axis.scale(__fres(sin), axis)`,
// a TU-local helper holding the zero-or-scale `if` (by reference or pointer,
// with or without the sqrt), and `cross` through a pointer helper; routing
// `cross` through a by-value return costs 23-49 instructions.
void JPAVecToRotaMtx(MtxPtr dst, JGeometry::TVec3<f32> a,
                     JGeometry::TVec3<f32> b)
{
	JGeometry::TVec3<f32> axis;
	axis.cross(a, b);

	f32 sq  = axis.squared();
	f32 cos = a.dot(b);
	f32 sin = JGeometry::TUtil<f32>::sqrt(sq);

	if (sin <= JGeometry::TUtil<f32>::epsilon()) {
		axis.zero();
	} else {
		axis.scale(__fres(sin));
	}

	f32 oneMinusCos = 1.0f - cos;

	dst[0][0] = cos * (1.0f - axis.x * axis.x) + axis.x * axis.x;
	dst[0][1] = axis.x * axis.y * oneMinusCos + axis.z * sin;
	dst[0][2] = axis.x * axis.z * oneMinusCos - axis.y * sin;
	dst[0][3] = 0.0f;

	dst[1][0] = axis.y * axis.x * oneMinusCos - axis.z * sin;
	dst[1][1] = cos * (1.0f - axis.y * axis.y) + axis.y * axis.y;
	dst[1][2] = axis.y * axis.z * oneMinusCos + axis.x * sin;
	dst[1][3] = 0.0f;

	dst[2][0] = axis.z * axis.x * oneMinusCos + axis.y * sin;
	dst[2][1] = axis.z * axis.y * oneMinusCos - axis.x * sin;
	dst[2][2] = cos * (1.0f - axis.z * axis.z) + axis.z * axis.z;
	dst[2][3] = 0.0f;
}

f32 JPAConvertFixToFloat(s16 param_1)
{
	if (param_1 == 0x7fff)
		return 1.0f;

	f32 f0 = (float)param_1 * (1.0f / 32768.0f) * 100000.0f;
	int r5 = (int)f0;
	int r0 = (int)f0 % 10;
	if (r0 >= 5) {
		r5 += (10 - r0);
	} else {
		r5 -= r0;
	}
	return (float)r5 * 1e-05f;
}

s16 JPAConvertFloatToFix(f32) { }

void JPAConvertFixVecToFloatVec(JGeometry::TVec3<f32>& param_1,
                                const JGeometry::TVec3<s16>& param_2)
{
	// The uninitialised vector is what retail's frame says: without a
	// 12-byte class local reserving 16 bytes of dead low region the three
	// inlined JPAConvertFixToFloat conversion temporaries sit at 0x10 rather
	// than retail's 0x20 (frame 0x28 against 0x38).  With it every
	// instruction matches; writing the results through it instead
	// (`v.x = ...; param_1 = v;`) costs the copy.
	JGeometry::TVec3<f32> v;

	param_1.x = JPAConvertFixToFloat(param_2.x);
	param_1.y = JPAConvertFixToFloat(param_2.y);
	param_1.z = JPAConvertFixToFloat(param_2.z);
}

void JPAConvertFloatVecToFixVec(JGeometry::TVec3<s16>&,
                                const JGeometry::TVec3<f32>&)
{
}

void JPABound(JGeometry::TVec3<f32>&, const JGeometry::TVec3<f32>&,
              const JGeometry::TVec3<f32>&, f32, f32)
{
}

void JPAGetSVecElement(MtxPtr param_1, JGeometry::TVec3<f32>& param_2)
{
	param_2.x = std::sqrtf(param_1[0][0] * param_1[0][0]
	                       + param_1[1][0] * param_1[1][0]
	                       + param_1[2][0] * param_1[2][0]);
	param_2.y = std::sqrtf(param_1[0][1] * param_1[0][1]
	                       + param_1[1][1] * param_1[1][1]
	                       + param_1[2][1] * param_1[2][1]);
	param_2.z = std::sqrtf(param_1[0][2] * param_1[0][2]
	                       + param_1[1][2] * param_1[1][2]
	                       + param_1[2][2] * param_1[2][2]);
}

void JPAGetRMtxSTVecElement(MtxPtr param_1, MtxPtr param_2,
                            JGeometry::TVec3<f32>& param_3,
                            JGeometry::TVec3<f32>& param_4)
{
	JPAGetSVecElement(param_1, param_3);

	MTXIdentity(param_2);
	if (param_3.x != 0.0f) {
		param_2[0][0] = param_1[0][0] / param_3.x;
		param_2[1][0] = param_1[1][0] / param_3.x;
		param_2[2][0] = param_1[2][0] / param_3.x;
	}
	if (param_3.y != 0.0f) {
		param_2[0][1] = param_1[0][1] / param_3.y;
		param_2[1][1] = param_1[1][1] / param_3.y;
		param_2[2][1] = param_1[2][1] / param_3.y;
	}
	if (param_3.z != 0.0f) {
		param_2[0][2] = param_1[0][2] / param_3.z;
		param_2[1][2] = param_1[1][2] / param_3.z;
		param_2[2][2] = param_1[2][2] / param_3.z;
	}

	param_4.set(param_1[0][3], param_1[1][3], param_1[2][3]);
}

// TODO: every instruction and the frame match; retail holds scale.y in f29 and
// scale.z in f30 where we hold y in f30 and z in f29 (24 operands).  The three
// values are an inlined callee's locals, on which declaration order is inert,
// and spelling JPAGetSVecElement's body as param_2.set(...) reorders the stores
// and shrinks both callers, so the rotation is still open.
// FPR re-pass 172: the ranking does not fit research 171 either. Retail is
// x f31, *z f30, y f29*; neither the reverse order rule for an inlined
// callee's temps nor the forward order rule for the function's own named
// locals puts x above a reversed {z, y}. The sibling JPAGetRMtxSTVecElement
// shares this body with a reference parameter in place of the local and is
// byte-exact, which pins JPAGetSVecElement: any change there breaks it.
// cc29: caller-level named copies of scale.x/y/z in four declaration orders
// (xzy, xyz, zyx, yzx) are all inert (+8 frame), and routing the body through
// the UNUSED JPAGetRMtxElement (plain: a `bl`; forced inline as a diagnostic:
// 55%) is refuted.
void JPAGetRMtxTVecElement(MtxPtr param_1, MtxPtr param_2,
                           JGeometry::TVec3<f32>& param_3)
{
	JGeometry::TVec3<f32> scale;

	JPAGetSVecElement(param_1, scale);

	MTXIdentity(param_2);
	if (scale.x != 0.0f) {
		param_2[0][0] = param_1[0][0] / scale.x;
		param_2[1][0] = param_1[1][0] / scale.x;
		param_2[2][0] = param_1[2][0] / scale.x;
	}
	if (scale.y != 0.0f) {
		param_2[0][1] = param_1[0][1] / scale.y;
		param_2[1][1] = param_1[1][1] / scale.y;
		param_2[2][1] = param_1[2][1] / scale.y;
	}
	if (scale.z != 0.0f) {
		param_2[0][2] = param_1[0][2] / scale.z;
		param_2[1][2] = param_1[1][2] / scale.z;
		param_2[2][2] = param_1[2][2] / scale.z;
	}

	param_3.set(param_1[0][3], param_1[1][3], param_1[2][3]);
}

// Reconstructed from the map size (0x24c, exact): the normalisation half of
// JPAGetRMtxTVecElement without the translation tail.  Retail inlined it
// nowhere (it is 588 bytes), so only the size vouches for it.
void JPAGetRMtxElement(MtxPtr param_1, MtxPtr param_2)
{
	JGeometry::TVec3<f32> scale;

	JPAGetSVecElement(param_1, scale);

	MTXIdentity(param_2);
	if (scale.x != 0.0f) {
		param_2[0][0] = param_1[0][0] / scale.x;
		param_2[1][0] = param_1[1][0] / scale.x;
		param_2[2][0] = param_1[2][0] / scale.x;
	}
	if (scale.y != 0.0f) {
		param_2[0][1] = param_1[0][1] / scale.y;
		param_2[1][1] = param_1[1][1] / scale.y;
		param_2[2][1] = param_1[2][1] / scale.y;
	}
	if (scale.z != 0.0f) {
		param_2[0][2] = param_1[0][2] / scale.z;
		param_2[1][2] = param_1[1][2] / scale.z;
		param_2[2][2] = param_1[2][2] / scale.z;
	}

}

void JPAGetRTMtxElement(MtxPtr, MtxPtr) { }

void JPARadVecToSVec(JGeometry::TVec3<f32>&, JGeometry::TVec3<s16>&) { }

f32 JPAGetKeyFrameValue(f32 time, u16 frame_num, f32* frames)
{
	return JPAGetKeyFrameInterpolation<f32>(time, frame_num, frames);
}
