#ifndef J3D_GRAPH_BASE_COMPONENTS_NBTSCALE
#define J3D_GRAPH_BASE_COMPONENTS_NBTSCALE

#include <JSystem/J3D/J3DGraphBase/J3DStruct.hpp>

extern const J3DNBTScaleInfo j3dDefaultNBTScaleInfo;

class J3DNBTScale : public J3DNBTScaleInfo {
public:
	J3DNBTScale()
	{
		J3DNBTScaleInfo* dst = getNbtScale();
		*dst                 = j3dDefaultNBTScaleInfo;
	}

	// see J3DIndTevStage
	J3DNBTScale* getNbtScale() { return this; }

	explicit J3DNBTScale(const J3DNBTScaleInfo& info)
	{
		J3DNBTScaleInfo::operator=(info);
	}

	bool hasScale() { return mbHasScale == 1; }
	Vec* getScale() { return &mScale; }
};

#endif
