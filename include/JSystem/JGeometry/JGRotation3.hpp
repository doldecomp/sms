#ifndef JG_ROTATION3_HPP
#define JG_ROTATION3_HPP

#include <dolphin/types.h>
#include <JSystem/JGeometry/JGVec3.hpp>
#include <JSystem/JGeometry/JGQuat4.hpp>
#include <JSystem/JGeometry/JGMatrix34.hpp>

namespace JGeometry {

template <class T> class TRotation3 : public T {
public:
	void identity33();
	TRotation3();

	void setRotate(const JGeometry::TVec3<f32>&, f32);
	void setEular(s16, s16, s16);
	void setEular(f32, f32, f32);
	void setLookDir(const JGeometry::TVec3<f32>&, const JGeometry::TVec3<f32>&);
	void setQuat(const JGeometry::TQuat4<f32>&);
	void getQuat(JGeometry::TQuat4<f32>&) const;
	void setSQ(const JGeometry::TVec3<f32>&, const JGeometry::TQuat4<f32>&);
};

} // namespace JGeometry

#endif
