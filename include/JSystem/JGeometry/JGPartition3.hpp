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

	TVec3<T> mNormal; // plane normal
	T mDist;          // distance from origin
};

} // namespace JGeometry

#endif
