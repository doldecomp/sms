#ifndef JG_BOX_HPP
#define JG_BOX_HPP

#include <dolphin/types.h>
#include <JSystem/JGeometry/JGVec2.hpp>
#include <JSystem/JGeometry/JGVec3.hpp>

namespace JGeometry {

// Functions were scrapped from SMG, TP and TWW

template <class T> struct TBox {
	TBox() { }
	TBox(const TBox& other)
	    : i(other.i)
	    , f(other.f)
	{
	}

	TBox& operator=(const TBox& other)
	{
		i = other.i;
		f = other.f;
		return *this;
	}

	f32 getWidth() const { return f.x - i.x; }
	f32 getHeight() const { return f.y - i.y; }
	bool isValid() const { return f.isAbove(i); }

	void extend(const T& param_1)
	{
		i.setMin(param_1);
		f.setMax(param_1);
	}

	void addPos(const T& param_1)
	{
		i += param_1;
		f += param_1;
	}

	void absolute()
	{
		if (!this->isValid()) {
			TBox box(*this);
			this->i.setMin(box.i);
			this->i.setMin(box.f);
			this->f.setMax(box.i);
			this->f.setMax(box.f);
		}
	}

	bool intersect(const TBox& other)
	{
		i.setMax(other.i);
		f.setMin(other.f);
		return isValid();
	}

	void setSize(const T&);
	void transpose(const T&);
	bool intersectsPoint(const T&) const;

	T i, f;
};

template <> struct TBox<TVec2<f32> > {
	TBox() { }
	TBox(const TBox& other)
	    : i(other.i)
	    , f(other.f)
	{
	}

	TBox& operator=(const TBox& other)
	{
		// TP debug says that this must be EXACTLY like this?!
		i.x = other.i.x;
		i.y = other.i.y;
		f.x = other.f.x;
		f.y = other.f.y;
		return *this;
	}

	f32 getWidth() const { return f.x - i.x; }
	f32 getHeight() const { return f.y - i.y; }

	bool isValid() const { return f.isAbove(i); }

	void addPos(f32 x, f32 y) { addPos(TVec2<f32>(x, y)); }

	void setSize(const TVec2<f32>& size);

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
	TBox2(const TBox2& other) { set(other); }
	TBox2& operator=(const TBox2& other)
	{
		this->i = other.i;
		this->f = other.f;
		return *this;
	}
	TBox2(const TVec2<T>& fst, const TVec2<T>& snd)
	{
		this->i.set(fst);
		this->f.set(snd);
	}
	TBox2(T x0, T y0, T x1, T y1) { set(x0, y0, x1, y1); }

	void set(const TVec2<T>& i, const TVec2<T>& f)
	{
		this->i.set(i);
		this->f.set(f);
	}
	void set(T x0, T y0, T x1, T y1)
	{
		this->i.set(x0, y0);
		this->f.set(x1, y1);
	}
	void set(const TBox<TVec2<T> >& other) { set(other.i, other.f); }
};

template <typename T> struct TBox3 : TBox<TVec3<T> > {
	TBox3() { }
	TBox3(const TBox3& other) { set(other); }
	TBox3& operator=(const TBox3& other)
	{
		*(TBox<TVec3<T> >*)this = other;
		return *this;
	}
	TBox3(const TVec3<T>& fst, const TVec3<T>& snd)
	{
		this->i.set(fst);
		this->f.set(snd);
	}
	TBox3(T x0, T y0, T z0, T x1, T y1, T z1) { set(x0, y0, z0, x1, y1, z1); }

	void set(const TVec3<T>& i, const TVec3<T>& f)
	{
		this->i.set(i);
		this->f.set(f);
	}
	void set(T x0, T y0, T x1, T y1)
	{
		this->i.set(x0, y0);
		this->f.set(x1, y1);
	}
	void set(const TBox<TVec3<T> >& other) { set(other.i, other.f); }
};

} // namespace JGeometry

#endif
