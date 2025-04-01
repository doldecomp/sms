#ifndef J_GEOMETRY_HPP
#define J_GEOMETRY_HPP

#include <types.h>
#include <math.h>
#include <dolphin/mtx.h>

namespace JGeometry {

template <typename T> struct TUtil;

template <> struct TUtil<f32> {
	static f32 epsilon() { return 3.81469727e-06f; }

	static f32 sqrt(f32 mag)
	{
		if (mag <= 0.0f) {
			return mag;
		} else {
			f32 root = __frsqrte(mag);
			return 0.5f * root * (3.0f - mag * (root * root)) * mag;
		}
	}

	static f32 inv_sqrt(f32 mag);
};

#pragma dont_inline on
inline f32 TUtil<f32>::inv_sqrt(f32 mag)
{
	if (mag <= 0.0f) {
		return mag;
	} else {
		f32 root = __frsqrte(mag);
		return 0.5f * root * (3.0f - mag * (root * root));
	}
}
#pragma dont_inline off

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

	bool isAbove(const TVec2<T>& other) const
	{
		return (x >= other.x) && (y >= other.y) ? true : false;
	}

	T dot(const TVec2<T>& other) { return x * other.x + y * other.y; }

	T squared() { return dot(*this); }

	T length()
	{
		T sqr = squared();
		return TUtil<T>::sqrt(sqr);
	}

	T x;
	T y;
};

template <typename T> class TVec3 { };

template <> struct TVec3<s16> : public S16Vec { };

template <> class TVec3<f32> : public Vec {
public:
	// NOTE: this MUST be uninitialized as far as I can tell
	TVec3() { }

	TVec3(const TVec3& other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
	}

	TVec3(f32 x_, f32 y_, f32 z_) { set(x_, y_, z_); }

	TVec3& operator=(const TVec3& other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}
	TVec3& operator*=(const TVec3& operand);
	TVec3& operator-=(const TVec3& operand);

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

template <typename T> struct SMatrix34C {
	T mMtx[3][4];
};

template <typename T> struct SMatrix44C {
	T mMtx[4][4];
};

// NOTE: this HAS to be an explicit specialization for function prologue &
// epilogue to match. Whether it has to have 8-byte alignment is not certain. It
// helps some stack frame layouts to match, but it also might just be that
// function calls got inlined in those places and matrices don't really need the
// alignment.
// TODO: figure out whether we need 8-byte alignment here
template <> struct SMatrix34C<f32> {
	SMatrix34C() { }

	void set(f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32);

	typedef f32 ArrType[4];
	operator ArrType*() { return mMtx; }
	operator const ArrType*() const { return mMtx; }

	f32 mMtx[3][4];
};

// This one blows up in size if not explicitly specialized...
template <> struct SMatrix44C<f32> {
	f32 mMtx[4][4];
};

template <typename T> struct TMatrix34 : public T { };
template <typename T> struct TMatrix44 : public T {
	TMatrix44() { }
};

template <class T> struct TBox {
	TBox()
	    : i()
	    , f()
	{
	}
	TBox(const TBox& other)
	    : i(other.i)
	    , f(other.f)
	{
	}

	T i, f;
};

template <> struct TBox<TVec2<f32> > {
	f32 getWidth() const { return f.x - i.x; }
	f32 getHeight() const { return f.y - i.y; }

	bool isValid() const { return f.isAbove(i); }

	void addPos(f32 x, f32 y) { addPos(TVec2<f32>(x, y)); }

	void setSize(TVec2<f32> size)
	{
		f.x = size.x;
		f.y = size.y;
	}

	void addPos(const TVec2<f32>& pos)
	{
		i.add(pos);
		f.add(pos);
	}

	bool intersect(const TBox<TVec2<f32> >& other)
	{
		i.setMax(other.i);
		f.setMin(other.f);
		return isValid();
	}

	TVec2<f32> i, f;
};

template <typename T> struct TBox2 : TBox<TVec2<T> > {
	TBox2() { }
	TBox2(const TVec2<f32>& i, const TVec2<f32> f) { set(i, f); }
	TBox2(f32 x0, f32 y0, f32 x1, f32 y1) { set(x0, y0, x1, y1); }

	void absolute()
	{
		if (!this->isValid()) {
			TBox2<T> box(*this);
			this->i.setMin(box.i);
			this->i.setMin(box.f);
			this->f.setMax(box.i);
			this->f.setMax(box.f);
		}
	}

	void set(const TBox2& other) { set(other.i, other.f); }
	void set(const TVec2<f32>& i, const TVec2<f32>& f)
	{
		this->i.set(i), this->f.set(f);
	}
	void set(f32 x0, f32 y0, f32 x1, f32 y1)
	{
		this->i.set(x0, y0);
		this->f.set(x1, y1);
	}
};

} // namespace JGeometry

typedef JGeometry::SMatrix34C<f32> TSMtx34f;
typedef JGeometry::SMatrix44C<f32> TSMtx44f;
typedef JGeometry::TMatrix34<TSMtx34f> TMtx34f;
typedef JGeometry::TMatrix44<TSMtx44f> TMtx44f;

#endif
