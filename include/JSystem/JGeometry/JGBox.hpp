#ifndef JG_BOX_HPP
#define JG_BOX_HPP

#include <dolphin/types.h>
#include <JSystem/JGeometry/JGVec2.hpp>

namespace JGeometry {

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

#endif
