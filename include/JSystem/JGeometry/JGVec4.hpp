#ifndef JG_VEC4_HPP
#define JG_VEC4_HPP

#include <dolphin/types.h>
#include <dolphin/mtx.h>
#include <JSystem/JGeometry/JGUtil.hpp>
#include <JSystem/JGeometry/JGVec3.hpp>

namespace JGeometry {

template <typename T> class TVec4 : public Quaternion {
public:
	/* Constructors */
	inline TVec4() { }

	void zero() { x = y = z = w = 0.0f; }

	template <typename A> TVec4(A _x, A _y, A _z, A _h)
	{
		x = _x;
		y = _y;
		z = _z;
		w = _h;
	}

	/* General operations */
	template <typename A> void set(const JGeometry::TVec4<A>&);

	template <typename A> void set(A _x, A _y, A _z, A _w)
	{
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}

	void scale(T val)
	{
		x *= val;
		y *= val;
		z *= val;
		w *= val;
	}

	void scale(T scale, const TVec4<T>& b)
	{
		x = b.x * scale;
		y = b.y * scale;
		z = b.z * scale;
		w = b.w * scale;
	}

	inline TVec3<T>* toTVec3() { return (TVec3<T>*)this; }

	f32 squared() const { return x * x + y * y + z * z + w * w; }

	f32 length() const { return TUtil<f32>::sqrt(squared()); }

	bool isZero() const { return squared() <= TUtil<f32>::epsilon(); }

	// present in tww so likely real
	void setLength(f32 length) { setLength(*this, length); }

	void setLength(const TVec4<f32>& v, f32 length)
	{
		f32 lsq = v.squared();
		if (lsq <= TUtil<f32>::epsilon()) {
			zero();
			return;
		}

		scale(length * JGeometry::TUtil<f32>::inv_sqrt(lsq), v);
	}

	void normalize() { setLength(*this, 1.0f); }
};

} // namespace JGeometry

#endif
