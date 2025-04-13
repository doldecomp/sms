#ifndef JG_QUAT4_HPP
#define JG_QUAT4_HPP

#include <dolphin/types.h>
#include <JSystem/JGeometry/JGVec3.hpp>

namespace JGeometry {

template <class T> class TQuat4 {
public:
	void slerp(const JGeometry::TQuat4<f32>&, f32);
	void setRotate(const JGeometry::TVec3<f32>&, const JGeometry::TVec3<f32>&,
	               f32);
	void rotate(const JGeometry::TVec3<f32>&, JGeometry::TVec3<f32>&) const;
};

} // namespace JGeometry

#endif
