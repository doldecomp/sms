#ifndef JG_ROTATION3_HPP
#define JG_ROTATION3_HPP

#include <dolphin/types.h>
#include <JSystem/JGeometry/JGVec3.hpp>
#include <JSystem/JGeometry/JGQuat4.hpp>
#include <JSystem/JGeometry/JGMatrix34.hpp>

namespace JGeometry {

template <class T> class TRotation3 : public T {
public:
	TRotation3() { }

	void identity33()
	{
		this->ref(0, 0) = 1.0f;
		this->ref(1, 0) = 0.0f;
		this->ref(2, 0) = 0.0f;

		this->ref(0, 1) = 0.0f;
		this->ref(1, 1) = 1.0f;
		this->ref(2, 1) = 0.0f;

		this->ref(0, 2) = 0.0f;
		this->ref(1, 2) = 0.0f;
		this->ref(2, 2) = 1.0f;
	}

	void setRotate(const JGeometry::TVec3<f32>&, f32);
	void setEular(s16, s16, s16);
	void setEular(f32, f32, f32);
	void setLookDir(const JGeometry::TVec3<f32>&, const JGeometry::TVec3<f32>&);
	void setQuat(const JGeometry::TQuat4<f32>&);
	void getQuat(JGeometry::TQuat4<f32>&) const;
	void setSQ(const JGeometry::TVec3<f32>&, const JGeometry::TQuat4<f32>&);

	// From SMG
	void setXDir(const TVec3<f32>& param_1)
	{
		this->ref(0, 0) = param_1.x;
		this->ref(1, 0) = param_1.y;
		this->ref(2, 0) = param_1.z;
	}
	void setXDir(f32 x, f32 y, f32 z)
	{
		this->ref(0, 0) = x;
		this->ref(1, 0) = y;
		this->ref(2, 0) = z;
	}
	void setYDir(const TVec3<f32>& param_1)
	{
		this->ref(0, 1) = param_1.x;
		this->ref(1, 1) = param_1.y;
		this->ref(2, 1) = param_1.z;
	}
	void setYDir(f32 x, f32 y, f32 z)
	{
		this->ref(0, 1) = x;
		this->ref(1, 1) = y;
		this->ref(2, 1) = z;
	}
	void setZDir(const TVec3<f32>& param_1)
	{
		this->ref(0, 2) = param_1.x;
		this->ref(1, 2) = param_1.y;
		this->ref(2, 2) = param_1.z;
	}
	void setZDir(f32 x, f32 y, f32 z)
	{
		this->ref(0, 2) = x;
		this->ref(1, 2) = y;
		this->ref(2, 2) = z;
	}
	void setXYZDir(const TVec3<f32>& param_1, const TVec3<f32>& param_2,
	               const TVec3<f32>& param_3)

	{
		setXDir(param_1);
		setYDir(param_2);
		setZDir(param_3);
	}
};

} // namespace JGeometry

#endif
