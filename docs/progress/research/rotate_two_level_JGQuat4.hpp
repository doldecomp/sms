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

	// Assumes unit quaternion. These were renamed to "transform" in SMG.
	//
	// Two inline levels, not one: retail's deep sites (execWalk, Bird doWalk,
	// fireWanwan's doAdjustTarget sites, TabePuku Drag) `bl` the empty
	// TVec4<f>::TVec4() for the first product and TVec3<f>::set<f> for the
	// result while the shallow ones expand both. With the TQuat4 temporary and
	// the set one level below rotate, rotate reached at depth 2 puts the set at
	// depth 4 (3 statements > 2: call) and TVec4() at depth 5 (never), and
	// rotate at depth 1 expands everything. rotate must then fit depth 2's
	// nine statements and rotateQ depth 3's six, which is why the vector is
	// read through its reference rather than seven named locals.
	//
	// TODO: the shallow sites still prefer the old one-level body with vx/vy/vz
	// locals and a second TQuat4 (Kumokun's weak copy 90.4 -> 83.0, makeQuat,
	// Kazekun doAttackPose, bindBody, doLanding, rotateGoalDirToLocal); the
	// real split between rotate and its helper is not found yet.
	void rotateQ(const TVec3<T>& v, T w, T z, T y, T x, TVec3<T>& rDest) const
	{
		// clang-format off
		TQuat4 q;
		q.x =  w *  0 + y * v.z - z * v.y + w * v.x;
		q.y = -x * v.z + y *  0 + z * v.x + w * v.y;
		q.z =  x * v.y - y * v.x + z *  0 + w * v.z;
		q.w = -x * v.x - y * v.y - z * v.z + w *  0;

		rDest.set( q.x *  w + q.y * -z - q.z * -y + q.w * -x,
		          -q.x * -z + q.y *  w + q.z * -x + q.w * -y,
		           q.x * -y - q.y * -x + q.z *  w + q.w * -z);
		// clang-format on
	}

	void rotate(const TVec3<T>& v, TVec3<T>& rDest) const
	{
		T x = this->x;
		T y = this->y;
		T z = this->z;
		T w = this->w;
		rotateQ(v, w, z, y, x, rDest);
	}

	void rotate(TVec3<T>& rDest) const { rotate(rDest, rDest); }

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
