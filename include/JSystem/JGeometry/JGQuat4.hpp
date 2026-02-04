#ifndef JG_QUAT4_HPP
#define JG_QUAT4_HPP

#include <dolphin/types.h>
#include <JSystem/JGeometry/JGVec3.hpp>
#include <JSystem/JGeometry/JGVec4.hpp>

namespace JGeometry {

template <typename T> struct TQuat4 : public TVec4<T> {
public:
	/* Constructors */
	inline TQuat4() { }

	inline TQuat4(T xyz, T _w)
	{
		this->x = xyz;
		this->y = xyz;
		this->z = xyz;
		this->w = _w;
	}

	template <typename A> TQuat4(A _x, A _y, A _z, A _w)
	{
		this->x = _x;
		this->y = _y;
		this->z = _z;
		this->w = _w;
	}

	inline TVec3<T>* toTvec() { return (TVec3<T>*)this; }

	template <typename A> inline void set(A _x, A _y, A _z, A _w)
	{
		TVec4<T>::set(_x, _y, _z, _w);
	}

	template <typename A> void mul(const TVec4<A>& other)
	{
		T _x = this->w * other.x + this->x * other.w + this->y * other.z
		       - this->z * other.y;
		T _y = this->w * other.y - this->x * other.z + this->y * other.w
		       + this->z * other.x;
		T _z = this->w * other.z + this->x * other.y - this->y * other.x
		       + this->z * other.w;
		T _w = this->w * other.w - this->x * other.x - this->y * other.y
		       - this->z * other.z;

		set(_x, _y, _z, _w);
	}

	template <typename A> void mul(const TQuat4<A>& a, const TQuat4<A>& b)
	{
		T _x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y;
		T _y = a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x;
		T _z = a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w;
		T _w = a.w * b.w - a.x * a.x - a.y * a.y - a.z * a.z;
		set(_x, _y, _z, _w);
	}

	/* General operations */
	void normalize(const TQuat4<T>& rSrc);

	void getXDir(TVec3<T>& rDest) const
	{
		rDest.template set<T>(
		    1.0f - this->y * this->y * 2.0f - this->z * this->z * 2.0f,
		    this->x * this->y * 2.0f + this->w * this->z * 2.0f,
		    this->x * this->z * 2.0f - this->w * this->y * 2.0f);
	}

	void getYDir(TVec3<T>& rDest) const
	{
		rDest.template set<T>(
		    this->x * this->y * 2.0f - this->w * this->z * 2.0f,
		    1.0f - this->x * this->x * 2.0f - this->z * this->z * 2.0f,
		    this->y * this->z * 2.0f + this->w * this->x * 2.0f);
	}

	void getZDir(TVec3<T>& rDest) const
	{
		rDest.template set<T>(
		    this->x * this->z * 2.0f + this->w * this->y * 2.0f,
		    this->y * this->z * 2.0f - this->w * this->x * 2.0f,
		    1.0f - this->x * this->x * 2.0f - this->y * this->y * 2.0f);
	}

	void getEuler(TVec3<T>& rDest) const;
	void setEuler(T _x, T _y, T _z);
	void setEulerZ(T _z);

	void setRotate(const TVec3<T>& pVec, f32 pAngle)
	{
		f32 halfAngle = pAngle * 0.5f;
		toTvec()->scale(sinf(halfAngle), pVec);
		this->w = cosf(halfAngle);
	}

	void setRotate(const TVec3<T>& from, const TVec3<T>& to, T amount)
	{
		TVec3<T> axis;
		axis.cross(from, to);

		f32 len = axis.length();
		if (len <= TUtil<f32>::epsilon()) {
			set(0.0f, 0.0f, 0.0f, 1.0f);
		} else {
			f32 calcAngle = atan2f(len, from.dot(to));
			f32 halfAngle = amount * (calcAngle * 0.5f);
			toTvec()->scale(sinf(halfAngle) / len, axis);
			this->w = cosf(halfAngle);
		}
	}

	void setRotate(const TVec3<T>&, const TVec3<T>&);

	// Might be fake
	void setRotate(f32 x, f32 y, f32 z, f32 pAngle)
	{
		TVec3<T> pVec(x, y, z);
		setRotate(pVec, pAngle);
	}

	void rotate(TVec3<T>& rDest) const;

	void slerp(const TQuat4<T>& a1, const TQuat4<T>& a2, T a3)
	{
		this->x = a1.x;
		this->y = a1.y;
		this->z = a1.z;
		this->w = a1.w;
		this->slerp(a2, a3);
	}

	void scale(f32 scalar, const TVec3<T>& rVec)
	{
		TVec3<T>::scale(scalar, rVec);
	}

	void slerp(const TQuat4<T>&, T);

	void transform(const TVec3<T>& v, TVec3<T>& rDest)
	{
		TQuat4<T> q((this->w * v.x) + (this->y * v.z) - (this->z * v.y),
		            (this->w * v.y) - (this->x * v.z) + (this->z * v.x),
		            (this->w * v.z) + (this->x * v.y) - (this->y * v.x),
		            -((this->x * v.x) + (this->y * v.y) + (this->z * v.z)));

		rDest.set((-q.w * this->x) + (q.x * this->w) - (q.y * this->z)
		              + (q.z * this->y),
		          (-q.w * this->y) + (q.x * this->z) + (q.y * this->w)
		              - (q.z * this->x),
		          (-q.w * this->z) - (q.x * this->y) + (q.y * this->x)
		              + (q.z * this->w));
	}

	void transform(TVec3<T>& rDest) const;

	/* Operators */
	TQuat4<T>& operator=(const TQuat4<T>& rSrc);
};

} // namespace JGeometry

#endif
