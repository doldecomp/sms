#ifndef JG_VEC4_HPP
#define JG_VEC4_HPP

#include <dolphin/types.h>
#include <dolphin/mtx.h>
#include <JSystem/JGeometry/JGVec3.hpp>

namespace JGeometry {

template <typename T> class TVec4 {
public:
	TVec4();
	TVec4(const JGeometry::TVec4<T>&);
	void operator=(const JGeometry::TVec4<T>&);

	template <class U> void set(U x_, U y_, U z_, U w_)
	{
		x = x_;
		y = y_;
		z = z_;
		w = w_;
	}

	void scale(T, const JGeometry::TVec4<T>&);

	T dot(const JGeometry::TVec4<T>& other) const
	{
		return x * other.x + y * other.y + z * other.z + w * other.w;
	}

	JGeometry::TVec3<T>& xyz() { return (JGeometry::TVec3<T>&)*this; }

	T x, y, z, w;
};

} // namespace JGeometry

#endif
