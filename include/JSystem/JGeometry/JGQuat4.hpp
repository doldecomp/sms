#ifndef JG_QUAT4_HPP
#define JG_QUAT4_HPP

#include <dolphin/types.h>
#include <JSystem/JGeometry/JGVec3.hpp>
#include <JSystem/JGeometry/JGVec4.hpp>

namespace JGeometry {

template <typename T> struct TQuat4 : public TVec4<T> {
public:
	TQuat4() { }

	TQuat4(T xyz, T _w)
	{
		this->x = xyz;
		this->y = xyz;
		this->z = xyz;
		this->w = _w;
	}

	TQuat4(T _x, T _y, T _z, T _w)
	{
		this->x = _x;
		this->y = _y;
		this->z = _z;
		this->w = _w;
	}

	template <typename A> TQuat4(A _x, A _y, A _z, A _w)
	{
		this->x = _x;
		this->y = _y;
		this->z = _z;
		this->w = _w;
	}

	void conjugate() { this->xyz().negate(); }

	void mul(const TQuat4& other)
	{
		// clang-format off
		T _x = this->w * other.x + this->x * other.w + this->y * other.z - this->z * other.y;
		T _y = this->w * other.y + this->y * other.w + this->z * other.x - this->x * other.z;
		T _z = this->w * other.z + this->z * other.w + this->x * other.y - this->y * other.x;
		T _w = this->w * other.w - this->x * other.x - this->y * other.y - this->z * other.z;
		// clang-format on

		set(_x, _y, _z, _w);
	}

	// TODO: measured tree-wide on 2026-09-23 (header round c-qmul): writing
	// both products straight into set() with no _x.._w locals (BeeHive's
	// mulQuat) gains 13 functions: flyAroundMario +3.62, doAttackPose +5.23,
	// Kazekun attack execute 98.93 -> 99.79, KoopaJrSubmarine calcRootMatrix
	// +3.28, decideTargetAtDir +3.48, makeInitialVelocity 96.29 -> 99.88,
	// moveCoaster +3.74, updatePosture +2.06, Kukku calcRootMatrix +1.91,
	// wireTrap calcRootMatrix +0.96, KoopaNeckCallBack +1.28, two nerves.
	// TYumbo::shotSeeds (-2.12) is recovered by `roll.mul(pitch);
	// roll.rotate(dir, dir);` (95.74). The blocker is SMS_Eular2Quat: its
	// two-result spelling falls 91.88 -> 91.36, and every in-place spelling
	// (97.0, retail's slots) is auto-inlined into TAnimalBase::execWalk,
	// which retail calls out of line (89.89 -> 50.1); ten more respellings
	// are listed in docs/catalog/findings-claude.md. The same no-locals form
	// for the one-argument mul adds makeQuat +4.03 and updatePosture +1.0
	// but drops shotSeeds under every spelling tried (93.5).
	void mul(const TQuat4& a, const TQuat4& b)
	{
		// clang-format off
		T _x = a.x * b.w + a.w * b.x + a.y * b.z - a.z * b.y;
		T _y = a.y * b.w + a.w * b.y + a.z * b.x - a.x * b.z;
		T _z = a.z * b.w + a.w * b.z + a.x * b.y - a.y * b.x;
		T _w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;
		// clang-format on

		set(_x, _y, _z, _w);
	}

	// NOTE: SMG contains a "normalize" implementation here that reset the
	// quaternion to identity in case of zero length -- welp, not in SMS! Here
	// we just leave it all to TVec4 and zero out the quaternion to divide by
	// zero even more! Fun!

	void getXDir(TVec3<T>& rDest) const
	{
		f32 _x = 1.0f - this->y * this->y * 2.0f - this->z * this->z * 2.0f;
		f32 _y = this->x * this->y * 2.0f + this->w * this->z * 2.0f;
		f32 _z = this->x * this->z * 2.0f - this->w * this->y * 2.0f;
		rDest.set(_x, _y, _z);
	}

	void getYDir(TVec3<T>& rDest) const
	{
		f32 _x = this->x * this->y * 2.0f - this->w * this->z * 2.0f;
		f32 _y = 1.0f - this->x * this->x * 2.0f - this->z * this->z * 2.0f;
		f32 _z = this->y * this->z * 2.0f + this->w * this->x * 2.0f;
		rDest.set(_x, _y, _z);
	}

	void getZDir(TVec3<T>& rDest) const
	{
		f32 _x = this->x * this->z * 2.0f + this->w * this->y * 2.0f;
		f32 _y = this->y * this->z * 2.0f - this->w * this->x * 2.0f;
		f32 _z = 1.0f - this->x * this->x * 2.0f - this->y * this->y * 2.0f;
		rDest.set(_x, _y, _z);
	}

	void getEuler(TVec3<T>& rDest) const;
	void setEuler(T _x, T _y, T _z);
	void setEulerX(T _x)
	{
		f32 s   = sin(0.5f * _x);
		f32 c   = cos(0.5f * _x);
		this->x = s;
		this->y = 0.0f;
		this->z = 0.0f;
		this->w = c;
	}
	void setEulerY(T _y)
	{
		f32 s   = sin(0.5f * _y);
		f32 c   = cos(0.5f * _y);
		this->x = 0.0f;
		this->y = s;
		this->z = 0.0f;
		this->w = c;
	}
	void setEulerZ(T _z)
	{
		f32 s   = sin(0.5f * _z);
		f32 c   = cos(0.5f * _z);
		this->x = 0.0f;
		this->y = 0.0f;
		this->z = s;
		this->w = c;
	}

	void setRotate(const TVec3<T>& pVec, f32 pAngle)
	{
		this->xyz().scale(sinf(pAngle * 0.5f), pVec);
		this->w = cosf(pAngle * 0.5f);
	}

	// Matches the weak copy in BeeHive.o and colours the inlined copies
	// (makeInitialVelocity, decideTargetAtDir, initAttachPlane, getGravityDir)
	// as retail does: the cross product is stored straight into the axis
	// component by component. The three named scalars (s, eps, c) are what
	// give the weak copy its 0x70 frame; without them it is 0x68 with every
	// instruction equal. Naming the cross terms instead (f32 cx = ...) also
	// matches the weak copy but permutes f28-f31 at every inline site.
	// TKukku::calcRootMatrix (-0.12) and TBathtubKiller::makeQuat (-0.16)
	// lose a little here; both carry their own caller-side colouring residue.
	void setRotate(const TVec3<T>& from, const TVec3<T>& to, T amount)
	{
		TVec3<T> axis;
		axis.x = from.y * to.z - from.z * to.y;
		axis.y = from.z * to.x - from.x * to.z;
		axis.z = from.x * to.y - from.y * to.x;

		f32 len = axis.length();
		f32 eps = TUtil<f32>::epsilon();
		if (len <= eps) {
			this->set(0.0f, 0.0f, 0.0f, 1.0f);
			return;
		}

		f32 halfAngle = 0.5f * atan2(len, from.dot(to)) * amount;
		f32 s = sin(halfAngle) / len;
		this->xyz().scale(s, axis);
		f32 c = cos(halfAngle);
		this->w = c;
	}

	void setRotate(const TVec3<T>& a, const TVec3<T>& b)
	{
		setRotate(a, b, 1.0f);
	}

	// Assumes unit quaternion. These were renamed to "transform" in SMG, where
	// the in-place transform(TVec3&) has its own body rather than forwarding
	// to transform(const TVec3&, TVec3&). Modelled the same way here:
	//
	// The in-place rotate goes through two inline levels. Retail's in-place
	// deep sites (execWalk, fireWanwan's FindMario/RecoverGraph nerves,
	// TabePuku Drag) `bl` the empty TVec4<f>::TVec4() for the first product
	// and TVec3<f>::set<f> for the result; with the TQuat4 temporary and the
	// set in rotateQ, two levels below rotate(rDest), both fall past the
	// inliner's depth limit exactly there. rotateQ reads the vector through
	// its reference: by-value vector components give the deep sites stack
	// homes retail does not have (TabePuku Drag 0xc0 -> 0x110). It reads the
	// quaternion's members after declaring the product and has no `* 0`
	// terms (fireWanwan's doAdjustTarget fork, header round 2026-09-23:
	// execWalk 86.78 -> 89.89, fireWanwan's nerves unchanged without the
	// fork, nothing else moves; the old x/y/z/w-parameter form is gone).
	//
	// The two-argument rotate keeps the one-level vx/vy/vz body.
	// TODO: measured tree-wide on 2026-09-23, the two-argument rotate as a
	// one-level copy of rotateQ's body (member reads, no `* 0`, no second
	// TQuat4) makes Kumokun's weak rotate 90.39 -> 100 and gains
	// rotateGoalDirToLocal 97.73 -> 100, makeKillerVelocity +2.32,
	// calcBathtubData +2.22, getGravityDir +1.05, shotSeeds +0.91,
	// moveCoaster +0.62, flyAroundMario +0.59, dropCoins +0.07,
	// doFlyToCurPathNode +0.02, but costs doAttackPose 78.94 -> 75.69,
	// makeQuat 92.61 -> 90.40 and fireWanwan bindBody 99.53 -> 98.39 (frame
	// 0x1b0 vs retail 0x1e8; rotateInPlace or rotate(v) there is worse).
	// Kazekun's three doAttackPose rotates are inert to rotateInPlace. As a
	// two-level forwarder to rotateQ it gains the same sites (and Bird's weak
	// set<f>) but inlines at Kumokun's Wait nerve, where retail `bl`s rotate,
	// so the weak copy vanishes (90.39 -> 0). The one-level member-read body
	// is very likely retail's; the three losing sites are what blocks it.
	// Re-measured 2026-09-23 (c-hdr2): the same gains plus doLanding +0.22;
	// losses doAttackPose 82.36 -> 79.63 (frame exact at 0x218, the loss is
	// FPR colouring in getAroundQuat/mul), makeQuat 92.61 -> 90.40 (0x1f0
	// vs 0x1e0; param FPRs f29/f30 vs retail f24/f25), bindBody 99.53 ->
	// 98.39. Refuted as fixes: a q2 result temporary, vx/vy/vz or x/y/z/w
	// copies in the body (all lose Kumokun's weak rotate; the old locals are
	// what give bindBody retail's 0x1e8); at the sites, rotating a TVec3
	// temporary or straight into mVelocity, in-place or rotateInPlace calls,
	// split source/destination, scope and declaration order, and a
	// `const TQuat4& cur = mQuat` binder (80.7). lever-search on bindBody
	// finds only forks (98.42).
	void rotateQ(const TVec3<T>& v, TVec3<T>& rDest) const
	{
		// clang-format off
		TQuat4 q;
		q.x =  this->y * v.z - this->z * v.y + this->w * v.x;
		q.y = -this->x * v.z + this->z * v.x + this->w * v.y;
		q.z =  this->x * v.y - this->y * v.x + this->w * v.z;
		q.w = -this->x * v.x - this->y * v.y - this->z * v.z;

		rDest.set( q.x *  this->w + q.y * -this->z - q.z * -this->y + q.w * -this->x,
		          -q.x * -this->z + q.y *  this->w + q.z * -this->x + q.w * -this->y,
		           q.x * -this->y - q.y * -this->x + q.z *  this->w + q.w * -this->z);
		// clang-format on
	}

	void rotateInPlace(const TVec3<T>& v, TVec3<T>& rDest) const
	{
		rotateQ(v, rDest);
	}

	void rotate(const TVec3<T>& v, TVec3<T>& rDest) const
	{
		// Incollect regalloc
		f32 vx = v.x;
		f32 vy = v.y;
		f32 vz = v.z;

		T w = this->w;
		T z = this->z;
		T y = this->y;
		T x = this->x;

		// clang-format off
		TQuat4 q;
		q.x =  w *  0 + y * vz - z * vy + w * vx;
		q.y = -x * vz + y *  0 + z * vx + w * vy;
		q.z =  x * vy - y * vx + z *  0 + w * vz;
		q.w = -x * vx - y * vy - z * vz + w *  0;

		TQuat4 q2;
		q2.x =  q.x *  w + q.y * -z - q.z * -y + q.w * -x;
		q2.y = -q.x * -z + q.y *  w + q.z * -x + q.w * -y;
		q2.z =  q.x * -y - q.y * -x + q.z *  w + q.w * -z;
		// clang-format on

		rDest.set(q2.x, q2.y, q2.z);
	}

	void rotate(TVec3<T>& rDest) const { rotateInPlace(rDest, rDest); }

	void slerp(const TQuat4<T>& a1, const TQuat4<T>& a2, T a3)
	{
		this->x = a1.x;
		this->y = a1.y;
		this->z = a1.z;
		this->w = a1.w;
		this->slerp(a2, a3);
	}

	// Matches the weak copy in fireWanwan.o (0x248). The result goes out
	// through TVec4::set(x, y, z, w) in one call, not four member stores:
	// the set's four by-value parameters are the 16 bytes of frame the
	// member-store spelling was missing (0xb8, not 0xa8). A TQuat4 temporary
	// passed to set(const TVec4&) overshoots to 0xc8 and 0xd8.
	void slerp(const TQuat4<T>& param_1, T param_2)
	{
		TQuat4<f32> q1;
		q1.normalize(*this);
		TQuat4<f32> q2;
		q2.normalize(param_1);

		f32 fVar13 = q1.dot(q2);

		bool bVar9;
		if (fVar13 < 0.0f) {
			bVar9  = true;
			fVar13 = -fVar13;
		} else {
			bVar9 = false;
		}

		f32 fVar92;
		if (1.0f - fVar13 <= TUtil<f32>::epsilon()) {
			fVar92 = 1.0f - param_2;
		} else {
			f32 fVar11 = acosf(fVar13);
			f32 fVar12 = sinf(fVar11);

			fVar92  = sinf((1.0f - param_2) * fVar11) / fVar12;
			param_2 = sinf(param_2 * fVar11) / fVar12;
		}

		if (bVar9)
			param_2 = -param_2;

		this->set(fVar92 * q1.x + param_2 * q2.x, fVar92 * q1.y + param_2 * q2.y,
		          fVar92 * q1.z + param_2 * q2.z, fVar92 * q1.w + param_2 * q2.w);
	}
};

} // namespace JGeometry

#endif
