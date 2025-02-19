#ifndef J_GEOMETRY_HPP
#define J_GEOMETRY_HPP

#include <types.h>
#include <dolphin/mtx.h>

namespace JGeometry {

template <typename T> struct TUtil;

template <> struct TUtil<float> {
	static float inv_sqrt(float);
};

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

template <typename T> class TVec3 {
public:
	TVec3()
	{
		x = 0;
		y = 0;
		z = 0;
	}
	TVec3(const TVec3& other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
	}

	TVec3(T x_, T y_, T z_) { set(x_, y_, z_); }

	TVec3& operator=(const TVec3& other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
	}
	TVec3& operator*=(const TVec3& operand);
	TVec3& operator-=(const TVec3& operand);

	void add(const TVec3& operand);
	void div(f32 divisor);
	T dot(const TVec3<T>& other) const
	{
		return x * other.x + y * other.y + z * other.z;
	}
	void negate();
	void scale(f32 scale);
	void scale(f32 scale, const TVec3& operand);
	void scaleAdd(f32 scale, const TVec3& operand, const TVec3& translate);
	void set(const Vec&);

	template <typename TY> void set(TY x_, TY y_, TY z_)
	{
		x = x_;
		y = y_;
		z = z_;
	}
	template <typename TY> void set(const TVec3<TY>&);

	Vec* toVec() const { return (Vec*)this; }

	void setLength(const TVec3<T>& v, f32 length)
	{
		const f32 lsq = v.dot(v);
		if (lsq <= 0.0000038146973f) {
			x = y = z = 0;
		} else {
			float invLen = length * JGeometry::TUtil<f32>::inv_sqrt(lsq);
			x            = v.x * invLen;
			y            = v.y * invLen;
			z            = v.z * invLen;
		}
	}

	void cross(const TVec3<T>& fst, const TVec3<T>& snd)
	{
		x = fst.y * snd.z - fst.z * snd.y;
		y = fst.z * snd.x - fst.x * snd.z;
		z = fst.x * snd.y - fst.y * snd.x;
	}

	void setMax(const TVec3& other);
	void setMin(const TVec3& other);
	void sub(const TVec3& translate);
	void sub(const TVec3& base, const TVec3& translate);

	T x;
	T y;
	T z;
};

template <typename T> struct SMatrix34C {
	T mMtx[3][4];
};

// NOTE: this HAS to be an explicit specialization for function prologue &
// epilogue to match. Whether it has to have 8-byte alignment is not certain. It
// helps some stack frame layouts to match, but it also might just be that
// function calls got inlined in those places and matrices don't really need the
// alignment.
// TODO: figure out whether we need 8-byte alignment here
template <> struct SMatrix34C<f32> {

	SMatrix34C() { }

	void set(float, float, float, float, float, float, float, float, float,
	         float, float, float);

	f32 mMtx[3][4] __attribute__((aligned(8)));
};

template <typename T> struct TMatrix34 : public T { };

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

typedef JGeometry::SMatrix34C<f32> TSMtxf;
typedef JGeometry::TMatrix34<TSMtxf> TMtx34f;

#endif
