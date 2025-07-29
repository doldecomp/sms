#ifndef JG_VEC2_HPP
#define JG_VEC2_HPP

#include <JSystem/JGeometry/JGUtil.hpp>

namespace JGeometry {

template <typename T> struct TVec2 {
	TVec2() { }
	TVec2(T x, T y) { set(x, y); }

	void set(T x, T y)
	{
		this->x = x;
		this->y = y;
	}

	void set(const TVec2<T>& other)
	{
		x = other.x;
		y = other.y;
	}

	void zero() { x = y = 0.0f; }

	// === arithmetic stuff ===

	void setMin(const TVec2<T>& min)
	{
		if (x >= min.x)
			x = min.x;
		if (y >= min.y)
			y = min.y;
	}

	void setMax(const TVec2<T>& max)
	{
		if (x <= max.x)
			x = max.x;
		if (y <= max.y)
			y = max.y;
	}

	void add(const TVec2<T>& other)
	{
		x += other.x;
		y += other.y;
	}

	void scale(f32 scale)
	{
		x *= scale;
		y *= scale;
	}

	bool isAbove(const TVec2<T>& other) const
	{
		return (x >= other.x) && (y >= other.y) ? true : false;
	}

	T dot(const TVec2<T>& other) { return x * other.x + y * other.y; }

	// === length stuff ===

	T squared() { return dot(*this); }

	T length()
	{
		T sqr = squared();
		return TUtil<T>::sqrt(sqr);
	}

	T x;
	T y;
};

} // namespace JGeometry

#endif
