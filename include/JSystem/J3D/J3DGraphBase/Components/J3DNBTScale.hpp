#ifndef J3D_GRAPH_BASE_COMPONENTS_NBTSCALE
#define J3D_GRAPH_BASE_COMPONENTS_NBTSCALE

#include <JSystem/J3D/J3DGraphBase/J3DStruct.hpp>

extern const J3DNBTScaleInfo j3dDefaultNBTScaleInfo;

class J3DNBTScale : public J3DNBTScaleInfo {
public:
	J3DNBTScale()
	{
		mbHasScale = j3dDefaultNBTScaleInfo.mbHasScale;
		mScale.x   = j3dDefaultNBTScaleInfo.mScale.x;
		mScale.y   = j3dDefaultNBTScaleInfo.mScale.y;
		mScale.z   = j3dDefaultNBTScaleInfo.mScale.z;
	}
	explicit J3DNBTScale(const J3DNBTScaleInfo& info)
	{
		mbHasScale = info.mbHasScale;
		mScale.x   = info.mScale.x;
		mScale.y   = info.mScale.y;
		mScale.z   = info.mScale.z;
	}

	J3DNBTScale& operator=(const J3DNBTScale& other)
	{
		mbHasScale = other.mbHasScale;
		mScale.x   = other.mScale.x;
		mScale.y   = other.mScale.y;
		mScale.z   = other.mScale.z;
		return *this;
	}

	Vec* getScale() { return &mScale; }
};

#endif
