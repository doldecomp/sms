#ifndef JG_VEC3_HPP
#define JG_VEC3_HPP

#include <dolphin/types.h>
#include <dolphin/mtx.h>
#include <JSystem/JGeometry/JGUtil.hpp>

namespace JGeometry {

template <typename T> class TVec3 { };

template <> struct TVec3<s16> : public S16Vec {
public:
	TVec3();

	void sub(const JGeometry::TVec3<s16>&);
	void add(const JGeometry::TVec3<s16>&);

	// fabricated
	TVec3(s16 x_, s16 y_, s16 z_) { set(x_, y_, z_); }
	template <typename TY> void set(TY x_, TY y_, TY z_)
	{
		x = x_;
		y = y_;
		z = z_;
	}
};

template <> class TVec3<f32> : public Vec {
public:
	// NOTE: this MUST be uninitialized as far as I can tell
	TVec3() { }

	TVec3(const Vec& b) { set(b.x, b.y, b.z); }

	TVec3(const TVec3& other)
	{
		// NOTE: yes, this has to use lwz/stw and not lfs/stf.
		// Checked via MarioCollision.cpp where this is not inlined
		*(Vec*)this = *(Vec*)&other;
	}

	TVec3(f32 x_, f32 y_, f32 z_) { set(x_, y_, z_); }

	TVec3& operator=(const TVec3& other)
	{
		// NOTE: yes, this has to use lwz/stw and not lfs/stf.
		// Checked via enemy.cpp where this is not inlined
		*(Vec*)this = *(Vec*)&other;
		return *this;
	}
	TVec3& operator*=(const TVec3& other);
	TVec3& operator+=(const TVec3& other)
	{
		add(other);
		return *this;
	}
	TVec3& operator-=(const TVec3& other)
	{
		sub(other);
		return *this;
	}

	operator Vec*() { return (Vec*)&x; }
	operator const Vec*() const { return (Vec*)&x; }

	void zero() { x = y = z = 0.0f; }

	void add(const TVec3& operand)
	{
		x += operand.x;
		y += operand.y;
		z += operand.z;
	}

	void add(const TVec3<f32>& a, const TVec3<f32>& b)
	{
		x = a.x + b.x;
		y = a.x + b.y;
		z = a.x + b.z;
	}

	void mul(const TVec3<f32>& b)
	{
		x *= b.x;
		y *= b.y;
		z *= b.z;
	}

	void div(f32 divisor);

	f32 dot(const TVec3<f32>& other) const
	{
		return x * other.x + y * other.y + z * other.z;
	}

	f32 squared() const { return x * x + y * y + z * z; }

	f32 length() const { return TUtil<f32>::sqrt(squared()); }

	void negate()
	{
		x = -x;
		y = -y;
		z = -z;
	}

	void scale(f32 scale)
	{
		x *= scale;
		y *= scale;
		z *= scale;
	}

	void scale(f32 scale, const TVec3<f32>& b)
	{
		x = b.x * scale;
		y = b.y * scale;
		z = b.z * scale;
	}

	void scaleAdd(f32 scale, const TVec3<f32>& b, const TVec3<f32>& c)
	{
		x = b.x + c.x * scale;
		y = b.y + c.y * scale;
		z = b.z + c.z * scale;
	}

	void set(const Vec& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}

	template <typename TY> void set(TY x_, TY y_, TY z_)
	{
		x = x_;
		y = y_;
		z = z_;
	}

	template <typename TY> void set(const TVec3<TY>& other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
	}

	bool isZero() const { return squared() <= TUtil<f32>::epsilon(); }

	// fabricated
	void setLength(f32 length) { setLength(*this, length); }
	void normalize() { setLength(*this, 1.0f); }

	void setLength(const TVec3<f32>& v, f32 length)
	{
		f32 lsq = v.squared();
		if (lsq <= TUtil<f32>::epsilon()) {
			zero();
			return;
		}

		scale(length * JGeometry::TUtil<f32>::inv_sqrt(lsq), v);
	}

	void cross(const TVec3<f32>& a, const TVec3<f32>& b)
	{
		set(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
		    a.x * b.y - a.y * b.x);
	}

	void setMax(const TVec3& other);
	void setMin(const TVec3& other);
	void sub(const TVec3& translate)
	{
		x -= translate.x;
		y -= translate.y;
		z -= translate.z;
	}
	void sub(const TVec3& fst, const TVec3& snd)
	{
		x = fst.x - snd.x;
		y = fst.y - snd.y;
		z = fst.z - snd.z;
	}
};

} // namespace JGeometry

#endif
