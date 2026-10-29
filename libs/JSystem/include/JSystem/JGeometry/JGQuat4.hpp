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
		T _x = this->x * other.w + this->w * other.x + this->y * other.z - this->z * other.y;
		T _y = this->y * other.w + this->w * other.y + this->z * other.x - this->x * other.z;
		T _z = this->z * other.w + this->w * other.z + this->x * other.y - this->y * other.x;
		T _w = this->w * other.w - this->x * other.x - this->y * other.y - this->z * other.z;
		// clang-format on

		this->set(_x, _y, _z, _w);
	}

	void mul(const TQuat4& a, const TQuat4& b)
	{
		// clang-format off
		this->set(a.x * b.w + a.w * b.x + a.y * b.z - a.z * b.y,
		          a.y * b.w + a.w * b.y + a.z * b.x - a.x * b.z,
		          a.z * b.w + a.w * b.z + a.x * b.y - a.y * b.x,
		          a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z);
		// clang-format on
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

	void setRotate(const TVec3<T>& from, const TVec3<T>& to, T amount)
	{
		TVec3<T> axis;
		axis.cross(from, to);

		f32 len = axis.length();
		if (len <= TUtil<f32>::epsilon()) {
			this->set(0.0f, 0.0f, 0.0f, 1.0f);
			return;
		}

		f32 halfAngle = 0.5f * atan2(len, from.dot(to)) * amount;
		this->xyz().scale(sin(halfAngle) / len, axis);
		this->w = cos(halfAngle);
	}

	void setRotate(const TVec3<T>& a, const TVec3<T>& b)
	{
		setRotate(a, b, 1.0f);
	}

	// Assumes unit quaternion. These were renamed to "transform" in SMG.
	void rotate(const TVec3<T>& v, TVec3<T>& rDest) const
	{
		TQuat4 q;
		q.x = this->y * v.z - this->z * v.y + this->w * v.x;
		q.y = -this->x * v.z + this->z * v.x + this->w * v.y;
		q.z = this->x * v.y - this->y * v.x + this->w * v.z;
		q.w = -this->x * v.x - this->y * v.y - this->z * v.z;

		rDest.set(
		    q.x * this->w + q.y * -this->z - q.z * -this->y + q.w * -this->x,
		    -q.x * -this->z + q.y * this->w + q.z * -this->x + q.w * -this->y,
		    q.x * -this->y - q.y * -this->x + q.z * this->w + q.w * -this->z);
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

	void slerp(const TQuat4<T>& param_1, T param_2)
	{
		f32 fVar93 = param_2;
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

		f32 fVar11;
		f32 fVar92;
		if (1.0f - fVar13 <= TUtil<f32>::epsilon()) {
			fVar92 = 1.0f - param_2;
		} else {
			fVar11 = acosf(fVar13);
			f32 fVar12 = sinf(fVar11);

			fVar92 = sinf((1.0f - param_2) * fVar11) / fVar12;
			fVar93 = sinf(param_2 * fVar11) / fVar12;
		}

		if (bVar9)
			fVar93 = -fVar93;

		this->set(
		    fVar92 * q1.x + fVar93 * q2.x, fVar92 * q1.y + fVar93 * q2.y,
		    fVar92 * q1.z + fVar93 * q2.z, fVar92 * q1.w + fVar93 * q2.w);
	}
};

} // namespace JGeometry

#endif
