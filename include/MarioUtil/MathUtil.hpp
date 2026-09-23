#ifndef MARIO_UTIL_MATH_UTIL_HPP
#define MARIO_UTIL_MATH_UTIL_HPP

#include <JSystem/JGeometry.hpp>
#include <JSystem/JMath.hpp>

/**
 * @brief Steps an integer @p value one increment towards @p target without
 * overshooting, using a separate step size for each direction of travel.
 *
 * @details When @p value is below @p target it is raised by @p inc; when it is
 * at or above @p target it is lowered by @p dec. In either case the result is
 * clamped so that a single call can never move @p value past @p target.
 *
 * @param value the current value
 * @param target the value to converge towards
 * @param inc amount added per call while @p value is below @p target
 * @param dec amount subtracted per call while @p value is at or above @p target
 * @return @p value advanced by one step towards @p target
 */
int IConverge(int value, int target, int inc, int dec);

/**
 * @brief Floating-point counterpart of IConverge(): steps @p value one
 * increment towards @p target without overshooting, using a separate step size
 * for each direction of travel.
 *
 * @details When @p value is below @p target it is raised by @p inc; when it is
 * at or above @p target it is lowered by @p dec. In either case the result is
 * clamped so that a single call can never move @p value past @p target.
 *
 * @param value the current value
 * @param target the value to converge towards
 * @param inc amount added per call while @p value is below @p target
 * @param dec amount subtracted per call while @p value is at or above @p target
 * @return @p value advanced by one step towards @p target
 */
f32 FConverge(f32 value, f32 target, f32 inc, f32 dec);

/**
 * @brief Computes the direction of the 2D vector (@p x, @p y) as a binary
 * angle, i.e. atan2(@p y, @p x) measured from the +@p x axis towards the +@p y
 * axis.
 *
 * @param x the component along the zero-angle (reference) axis
 * @param y the component along the quarter-turn axis
 * @return the angle of (@p x, @p y) as a signed 16-bit binary angle
 */
s16 matan(f32 x, f32 y);

// The game's degree-argument sine and cosine. They must stay one wrapper level
// above JMASSin/JMASCos: reached through JMASin/JMACos the table lookup stays
// out of line, which is what the two 0x38-byte weak copies in killer.o are.
// killer.cpp is the only translation unit in the ROM that emits them.
inline f32 MsSin(f32 degrees) { return JMASin(degrees); }
inline f32 MsCos(f32 degrees) { return JMACos(degrees); }

// fabricated
inline f32 MsAtan2(f32 y, f32 x)
{
	return abs(matan(y, x) * (360.0f / 65536.0f));
}

// TODO: retail expands this at depth 2 inside TBossGesso::inSight (the
// doAttackSingle shoot site and the UNUSED inSightAngle body) but calls it at
// depth 3. Our body never inlines there, and the reason is not its cost.
// Header round 2026-09-23, measured in a scratch harness that reproduces
// bossgesso: a member function holding a local TVec3, inlined at depth 1 into
// a caller that uses its result (not `return w1();`).
// - With an `if` or `else` anywhere in the body (early-return `if` + ternary,
//   `if`-assign-return, the current nested form, Bth: `if` ternary / `if`
//   return / named `theta`), it stays out of line at depth 2 with 0..11
//   filler statements. It is blocked outright, not over budget. The same
//   bodies do inline in a free-function harness (A costs ternary+2, B/Bel
//   +6, Bth +7), and when the depth-1 call is a bare `return w1();`.
// - An expression-only body (one `?:` return) has a flat cost. Expression
//   size, the number of `?:` and calls, and inlined helper levels (their
//   statements are free to the host) cost nothing. So it inlines even at
//   depth 3 (doAttackDouble 76.3, moveObject 87.7).
//   Only statements count: each named initialised local is +1 (z, x, the
//   360/65536 scale and the result give at most +4), and the depth-2/depth-3
//   window needs +7..+9.
// A real spelling in that window was not found. A TU-local copy with 7..9
// `(void)0;` fillers reaches doAttackSingle 99.5, but that is refused.
// Retail calls MsWrap<f> out of line at the same depth, which fits the same
// block on control-flow bodies.
// Header round 2026-09-23: fireWanwan's doAdjustTarget (nerve, depth 2)
// does inline the z == 0 branch spelt as one conditional return
// (`return axis.x >= 0.0f ? 90.0f : -90.0f;`, flat tail, named theta), so
// `if` bodies are not blocked everywhere. Moved here it gains nothing
// outside fireWanwan (bossgesso and igaiga unchanged) and costs
// MsIsInSight 100 -> 99.95 (the ternary's arm branches to a `b` instead of
// straight to the join; a split if/else call site does not fix it),
// TBGKMtxCalc::calc 100 -> 99.60 (result in f30 not f31) and
// walkToCurPathNode 99.10 -> 98.85 (calcBoids +0.02). Keeping the nested
// `if`/`else` for axis.x or dropping the else keeps fireWanwan out of line
// (85.95/87.98); dropping theta costs about 40 functions.
inline f32 MsGetRotFromZaxisY(const JGeometry::TVec3<f32>& axis)
{
	if (axis.z == 0.0f) {
		if (axis.x >= 0.0f)
			return 90.0f;
		else
			return -90.0f;
	}

	if (axis.z >= 0.0f) {
		return (360.0f / 65536.0f) * matan(axis.z, axis.x);
	} else {
		f32 theta = matan(-axis.z, axis.x) * (360.0f / 65536.0f);
		return 180.0f - theta;
	}
}

inline f32 MsGetRotFromYaxisZ(const JGeometry::TVec3<f32>& axis)
{
	if (axis.y == 0.0f) {
		if (axis.x >= 0.0f)
			return -90.0f;
		else
			return 90.0f;
	}

	if (axis.y >= 0.0f) {
		return -((360.0f / 65536.0f) * matan(axis.y, axis.x));
	} else {
		f32 theta = matan(-axis.y, axis.x) * (360.0f / 65536.0f);
		return 180.0f + theta;
	}
}

JGeometry::TVec3<f32> MsGetRotFromZaxis(const JGeometry::TVec3<f32>&);
JGeometry::TQuat4<f32> SMS_Eular2Quat(const JGeometry::TVec3<f32>&);
void MsMtxSetRotRPH(MtxPtr mtx, f32 x, f32 y, f32 z);

// Both are weak in the map (MsMtxSetRotX from MapObjPinna.o, MsMtxSetRotY from
// MapObjFence.o), so they are header inlines. jmaSinTable is an f32*, so every
// store below would invalidate the cached table pointer: both lookups have to
// be named up front.
// The inlined expansions at HauntLegCallback, PopoRollCallback and
// TItemSlotDrum::generateItem differ from retail by two tells: retail keeps
// &mtx in a saved register across the following MTXConcat/MTXMultVec, and
// loads the 0.0f/1.0f literals only after the preceding stores (the stores
// go through a pointer, as in the weak out-of-line bodies). The one spelling
// found that reproduces both is a two-level pointer: a non-substitutable
// argument (`MsMtxSetRotZ((MtxPtr)spin, a)`) plus `MtxPtr m = mtx;` in the
// body (HauntLegCallback 94.0 -> 100, generateItem 94.5 -> 99.6 with its old
// 0x10 frame gap, PopoRollCallback 94.9 -> 96.6). Neither half alone moves
// anything, the copy in this header regresses at least 9 functions,
// including the weak MsMtxSetRotX/Y bodies (100 -> 99.87), and the cast is
// a no-op conversion, so neither is landed. Inert: row pointers, `&mtx[0]`,
// an `Mtx` or `Mtx&` parameter, a forwarding level (loses the sin/cos
// expansion), a switch on the axis, a guard on `mtx`, and
// TRotation3f/TPosition3f/TMtx34f locals at the call site (their conversion
// operator adds 8 bytes of frame).
inline void MsMtxSetRotX(MtxPtr mtx, f32 angle)
{
	f32 sin = JMASSin(DEG2SHORTANGLE(angle));
	f32 cos = JMASCos(DEG2SHORTANGLE(angle));

	mtx[0][0] = 1.0f;
	mtx[0][1] = 0.0f;
	mtx[0][2] = 0.0f;
	mtx[0][3] = 0.0f;

	mtx[1][0] = 0.0f;
	mtx[1][1] = cos;
	mtx[1][2] = -sin;
	mtx[1][3] = 0.0f;

	mtx[2][0] = 0.0f;
	mtx[2][1] = sin;
	mtx[2][2] = cos;
	mtx[2][3] = 0.0f;
}

inline void MsMtxSetRotY(MtxPtr mtx, f32 angle)
{
	f32 sin = JMASSin(DEG2SHORTANGLE(angle));
	f32 cos = JMASCos(DEG2SHORTANGLE(angle));

	mtx[0][0] = cos;
	mtx[0][1] = 0.0f;
	mtx[0][2] = sin;
	mtx[0][3] = 0.0f;

	mtx[1][0] = 0.0f;
	mtx[1][1] = 1.0f;
	mtx[1][2] = 0.0f;
	mtx[1][3] = 0.0f;

	mtx[2][0] = -sin;
	mtx[2][1] = 0.0f;
	mtx[2][2] = cos;
	mtx[2][3] = 0.0f;
}
// The third sibling. The map has no MsMtxSetRotZ symbol anywhere, so unlike
// MsMtxSetRotX and MsMtxSetRotY it happens to be expanded in every TU that
// uses it; it still belongs beside them rather than in hauntLeg.cpp.
inline void MsMtxSetRotZ(MtxPtr mtx, f32 angle)
{
	f32 sin = JMASSin(DEG2SHORTANGLE(angle));
	f32 cos = JMASCos(DEG2SHORTANGLE(angle));

	mtx[0][0] = cos;
	mtx[0][1] = -sin;
	mtx[0][2] = 0.0f;
	mtx[0][3] = 0.0f;

	mtx[1][0] = sin;
	mtx[1][1] = cos;
	mtx[1][2] = 0.0f;
	mtx[1][3] = 0.0f;

	mtx[2][0] = 0.0f;
	mtx[2][1] = 0.0f;
	mtx[2][2] = 1.0f;
	mtx[2][3] = 0.0f;
}

void MsMtxSetXYZRPH(MtxPtr mtx, f32 x, f32 y, f32 z, s16 r, s16 p, s16 h);
inline void MsMtxSetXYZRPH(MtxPtr mtx, f32 x, f32 y, f32 z, f32 r, f32 p, f32 h)
{
	MsMtxSetXYZRPH(mtx, x, y, z, static_cast<s16>(r * (65536.0f / 360.0f)),
	               static_cast<s16>(p * (65536.0f / 360.0f)),
	               static_cast<s16>(h * (65536.0f / 360.0f)));
}
void MsMtxSetTRS(MtxPtr result, f32 x, f32 y, f32 z, f32 r, f32 p, f32 h,
                 f32 sx, f32 sy, f32 sz);

inline void MsMtxSetTRS(MtxPtr result, const JGeometry::TVec3<f32>& trans,
                        const JGeometry::TVec3<f32>& rot,
                        const JGeometry::TVec3<f32>& scale)
{
	MsMtxSetTRS(result, trans.x, trans.y, trans.z, rot.x, rot.y, rot.z, scale.x,
	            scale.y, scale.z);
}

// Fabricated AND wrong!
inline void MsMtxSetTRS(MtxPtr result, f32 x, f32 y, f32 z, s16 r, s16 p, s16 h,
                        f32 sx, f32 sy, f32 sz)
{
	MsMtxSetTRS(result, x, y, z, r * (360.0f / 65536.0f),
	            p * (360.0f / 65536.0f), h * (360.0f / 65536.0f), sx, sy, sz);
}

template <class T> inline T MsWrap(T t, T l, T r)
{
	if (l >= r)
		return l;

	while (t >= r)
		t -= r - l;
	while (t < l)
		t += r - l;

	return t;
}

// Ruled out: no declaration form here makes the ROM's out-of-line
// `MsClamp<f>` calls appear. The map's three local 0x20 copies (AnimalBase,
// CameraChange, cameragc) are per *call site*, not per TU: cameragc has a
// dozen uses and exactly one `bl`, in CPolarSubCamera::perform's expansion of
// calcExternalData_, while loadAfter expands the identical expansion -- and at
// that same site the ROM also calls TVec3::set<f> and TUtil<f32>::one(), which
// no MathUtil change could touch. Three unrelated inlines flipping together at
// one site is the caller-size family in docs/catalog/codegen-tells.md, so the
// lever is that caller's frame, not this declaration. Any extra level added
// *below* this function would also emit the wrong symbol name.
template <class T> inline T MsClamp(T t, T l, T r)
{
	if (t > r)
		t = r;
	else if (t < l)
		t = l;
	return t;
}

// fabricated
//
// TODO: `const f32& alpha` was tried, because the ROM re-reads rot.x from the
// stack after the inlined MsWrap in TFruitsBoat::moveObject's pitch block and a
// reference is what produces that reload (moveObject 93.95% -> 94.28%,
// fruitsboat 98.40% -> 98.49%). It is wrong: by value is what every other
// caller wants, and the reference costs
//     TAnimalBase::getRotationFlyToDir  100.00% -> 95.51%
//     TAnimalBase::execWalk              78.97% -> 75.90%
//     TSpineEnemy::turnToCurPathNode     99.89% -> 97.04%
//     THinokuri2::moveObject             99.13% -> 98.28%
//     TNerveHino2Turn::execute           96.76% -> 94.60%
//     TNerveFireWanwanTurn::execute      99.13% -> 94.70%
//     TBGTentacle::decideOwnState        99.86% -> 95.17%
//     TMonumentShine::control            97.00% -> 89.38%
//     TLiveActor::calcRideMomentum       99.73% -> 93.67%
// so the reload at the fruitsboat site comes from that call site's own shape,
// not from this signature.
// Also ruled out: spelling the upper bound `180.0f + alpha` instead of
// `alpha + 180.0f`. MWCC normalises a commutative float add, so it is not one
// instruction different anywhere in the tree, and TAnimalBird::doLanding's
// residual (85.8%) is a 0x18 frame gap with a float-register permutation, not
// an operand order here.
inline f32 MsAngleDiff(f32 alpha, f32 beta)
{
	return alpha - MsWrap(beta, alpha - 180.0f, alpha + 180.0f);
}

// fabricated
inline f32 MsAngleWrap(f32 angle) { return MsWrap(angle, 0.0f, 360.0f); }

/**
 * @brief Checks whether the point \p target is within the line of sight of
 * an \p eye looking in direction \p sight, given a sight cone \p angle and
 * \p length, as well as a distance \p aware, which is the distance at which
 * the target is considered to be in sight regardless of the angle.
 *
 * @param eye the point from which the sight is checked
 * @param sight the direction of sight
 * @param target the point to check
 * @param length the distance of the sight
 * @param angle the sweep angle of the sight cone
 * @param aware distance closer than which angle is not considered
 * @return BOOL true if the target is in sight, false otherwise
 */
BOOL MsIsInSight(const JGeometry::TVec3<f32>& eye, f32 sight,
                 const JGeometry::TVec3<f32>& target, f32 length, f32 angle,
                 f32 aware);

void SMS_GoRotate(const JGeometry::TVec3<f32>&, const JGeometry::TVec3<f32>&,
                  f32, f32*);
void SMSCalcJumpVelocityY(const JGeometry::TVec3<f32>&,
                          const JGeometry::TVec3<f32>&, f32, f32, f32,
                          JGeometry::TVec3<f32>*);
void SMSCalcJumpVelocityXZ(const JGeometry::TVec3<f32>&,
                           const JGeometry::TVec3<f32>&, f32, f32,
                           JGeometry::TVec3<f32>*);

void MsVECNormalize(Vec*, Vec*);
f32 MsVECMag2(Vec*);

// Only bosswanwan.o emits it out of line (weak); MapWire, MapWireManager and
// MapStaticObject inline it and are unchanged by the body below.
// TODO: 96.1%: every instruction right but retail spills the hidden return
// pointer to 8(r1) (the only leaf in the game that does), keeps t in f2 and
// has a frame 0x18 larger (0x58). `return param_1 + diff`, `diff *= t`, a
// named numerator and an `operator-` diff were all worse or inert.
inline JGeometry::TVec3<f32>
MsPerpendicFootToLineR(const JGeometry::TVec3<f32>& param_1,
                       const JGeometry::TVec3<f32>& param_2,
                       const JGeometry::TVec3<f32>& param_3)
{
	JGeometry::TVec3<f32> diff = param_2;
	diff -= param_1;
	f32 t = (param_3.dot(diff) - param_1.dot(diff)) / diff.squared();

	if (t < 0.0f)
		t = 0.0f;
	else if (t > 1.0f)
		t = 1.0f;

	diff.scale(t);
	JGeometry::TVec3<f32> foot = param_1;
	foot += diff;
	return JGeometry::TVec3<f32>(foot);
}

inline f32 MsSqrtf(f32 x)
{
	// We love copy-pasting code, don't we?
	volatile float y;
	if (x > 0.0f) {
		double guess = __frsqrte((double)x);
		guess        = .5 * guess * (3.0 - guess * guess * x);
		y            = (float)(x * guess);
		return y;
	}
	return x;
}

#endif
