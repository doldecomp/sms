#ifndef J3D_GRAPH_BASE_COMPONENTS_NBTSCALE
#define J3D_GRAPH_BASE_COMPONENTS_NBTSCALE

#include <JSystem/J3D/J3DGraphBase/J3DStruct.hpp>

extern const J3DNBTScaleInfo j3dDefaultNBTScaleInfo;

class J3DNBTScale : public J3DNBTScaleInfo {
public:
	J3DNBTScale() { *(J3DNBTScaleInfo*)this = j3dDefaultNBTScaleInfo; }
	explicit J3DNBTScale(const J3DNBTScaleInfo& info)
	{
		*(J3DNBTScaleInfo*)this = info;
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
