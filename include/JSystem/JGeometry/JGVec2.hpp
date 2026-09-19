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
	// stw/lwz. SMS does not have the assignment operator, and this was
	// measured rather than assumed: std::__copy<TCameraMapTool>
	// (System/MarNameRefGen.cpp) copies the class's 0x18 field with a *paired*
	// lwz/lwz/stw/stw. Declaring an operator= takes that function from exact
	// to 88.3%, and spelling the field out as two f32 members does the same,
	// because MWCC copies scalar float members with lfs/stfs.
	//
	// The 0x18 field turned out not to be a TVec2 at all: it is an f32[2].
	// TCameraMapTool's implicit copy constructor word-copies it while
	// copying the TVec3 above it through TVec3's user copy constructor, and
	// MWCC block-copies an *array* member but recurses to lfs/stfs through a
	// class-type member that has no user copy constructor. Giving TVec2 a
	// word-copying copy constructor reproduces that one function too, but it
	// takes TMapCollisionData::intersectLine 71.4 -> 57.3 and pushes the
	// map-exact UNUSED LineInLineXZ from 0x1bc to 0x20c, so TVec2 keeps
	// neither a copy constructor nor an operator=.
	//
	// Enabling operator= does gain TSelectShineManager::initData 97.3 -> 98.0
	// and ::perform 79.6 -> 80.3, and getAngle's UNUSED body only reaches the
	// map's 0x88 with float copies -- but against the above that means
	// getAngle reaches its lfs/stfs some other way (a component-wise sub()
	// against a temporary rather than `toCenter = toCenter - ...`), not
	// through operator=. The copy constructor on its own changes nothing
	// anywhere, so it stays out too.

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

	void scale(f32 scale, const TVec2& b)
	{
		x = b.x * scale;
		y = b.y * scale;
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

	// The two-argument forwarder is not cosmetic: it is the inline level that
	// puts TVec2::dot at depth five inside TBathtubPeach::goTo, so MWCC emits
	// the weak dot__Q29JGeometry8TVec2<f>CFRCQ29JGeometry8TVec2<f> the map
	// lists for BathtubPeach.cpp instead of expanding it. TVec3 has the same
	// pair in JGVec3.hpp.
	void setLength(f32 length) { setLength(*this, length); }

	void normalize() { setLength(*this, TUtil<f32>::one()); }

	void normalize(const TVec2& other) { setLength(other, TUtil<f32>::one()); }

	void setLength(const TVec2& v, f32 length)
	{
		f32 lsq = v.squared();
		if (lsq <= TUtil<f32>::epsilon()) {
			zero();
			return;
		}

		scale(length * JGeometry::TUtil<f32>::inv_sqrt(lsq), v);
	}

	T x;
	T y;
};

} // namespace JGeometry

#endif
