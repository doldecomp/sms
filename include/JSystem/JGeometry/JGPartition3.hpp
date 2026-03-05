#ifndef JG_PARTITION3_HPP
#define JG_PARTITION3_HPP

#include <JSystem/JGeometry/JGVec3.hpp>

namespace JGeometry {

template <typename T> class TPartition3 {
public:
	TPartition3() { }
	TPartition3(TVec3<T> normal, T dist)
	    : mNormal(normal)
	    , mDist(dist)
	{
	}

	void set(const TVec3<T>& param_1, const TVec3<f32> param_2,
	         const TVec3<f32> param_3)
	{
		TVec3<f32> v1 = param_2 - param_1;
		TVec3<f32> v2 = param_3 - param_2;
		mNormal.cross(v1, v2);
		mNormal.normalize();
		mDist = mNormal.dot(param_1);
	}

	void set(const TVec3<T>& param_1, const TVec3<f32> param_2)
	{
		mNormal = param_1;
		mDist   = mNormal.dot(param_2);
	}

	TVec3<T> mNormal; // plane normal
	T mDist;          // distance from origin
};

} // namespace JGeometry

#endif
