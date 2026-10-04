#ifndef JG_VEC2_HPP
#define JG_VEC2_HPP

#include <JSystem/JGeometry/JGUtil.hpp>

namespace JGeometry {

template <typename T> struct TVec2 {
	// NOTE: A whole bunch of inlines are taken from SMG and TP
public:
	TVec2() { }
	template <typename U> TVec2(U x_, U y_) { set(x_, y_); }

	// NOTE: Present in TP, presumably to force use of stfs/lfs instead of
	// stw/lwz, but it seems like SMS didn't have them yet?

	// TVec2(const TVec2& other)
	// {
	// 	x = other.x;
	// 	y = other.y;
	// }
	// TVec2& operator=(const TVec2& other)
	// {
	// 	// no `set` call according to TP debug
	// 	x = other.x;
	// 	y = other.y;
	// 	return *this;
	// }

	template <typename U> void setAll(U value)
	{
		x = value;
		y = value;
	}

	template <typename U> void set(U x_, U y_)
	{
		x = x_;
		y = y_;
	}

	template <typename U> void set(const TVec2<U>& other)
	{
		x = other.x;
		y = other.y;
	}

	void zero() { x = y = 0.0f; }

	bool equals(const TVec2& other) const
	{
		return x == other.x && y == other.y;
	}

	bool operator==(const TVec2& other) const { return equals(other); }

	// === arithmetic stuff ===

	void setMin(const TVec2& min)
	{
		if (x >= min.x)
			x = min.x;
		if (y >= min.y)
			y = min.y;
	}

	void setMax(const TVec2& max)
	{
		if (x <= max.x)
			x = max.x;
		if (y <= max.y)
			y = max.y;
	}

	void add(const TVec2& other)
	{
		x += other.x;
		y += other.y;
	}

	void add(const TVec2& fst, const TVec2& snd)
	{
		x = fst.x + snd.x;
		y = fst.y + snd.y;
	}

	void sub(const TVec2& other)
	{
		x -= other.x;
		y -= other.y;
	}

	void sub(const TVec2& fst, const TVec2& snd)
	{
		x = fst.x - snd.x;
		y = fst.y - snd.y;
	}

	void scale(f32 scale)
	{
		x *= scale;
		y *= scale;
	}

	// fabricated
	void rotate(f32 angle)
	{
		f32 cosTheta = cosf(angle);
		f32 sinTheta = sinf(angle);
		set(x * cosTheta - y * sinTheta, x * sinTheta + y * cosTheta);
	}

	bool isAbove(const TVec2& other) const
	{
		return (x >= other.x) && (y >= other.y) ? true : false;
	}

	T dot(const TVec2& other) const { return x * other.x + y * other.y; }

	T cross(const TVec2& other) const { return x * other.y - y * other.x; }

	TVec2& operator+=(const TVec2& other)
	{
		add(other);
		return *this;
	}

	TVec2& operator-=(const TVec2& other)
	{
		sub(other);
		return *this;
	}

	TVec2 operator*(T scalar) const { return TVec2(x * scalar, y * scalar); }

	const TVec2& operator+(const TVec2& other) const
	{
		TVec2 result = *this;
		result += other;
		return result;
	}

	const TVec2& operator-(const TVec2& other) const
	{
		TVec2 result = *this;
		result -= other;
		return result;
	}

	// === length stuff ===

	T squared() const { return dot(*this); }

	T squared(const TVec2& other) const
	{
		T dx = x - other.x;
		T dy = y - other.y;
		return dx * dx + dy * dy;
	}

	T length() const { return TUtil<T>::sqrt(squared()); }

	T distance(const TVec2& other) const
	{
		T dx = x - other.x;
		T dy = y - other.y;
		return TUtil<T>::sqrt(dx * dx + dy * dy);
	}

	// === normalize stuff lifted from JGVec3.hpp ===

	// fabricated
	void setLength(f32 length)
	{
		f32 lsq = squared();
		if (lsq <= TUtil<f32>::epsilon()) {
			zero();
			return;
		}

		scale(length * JGeometry::TUtil<f32>::inv_sqrt(lsq));
	}

	// fabricated
	void normalize() { setLength(1.0f); }

	T x;
	T y;
};

} // namespace JGeometry

#endif
