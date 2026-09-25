#ifndef J3D_GRAPH_BASE_COMPONENTS_INDTEXCOORDSCALE
#define J3D_GRAPH_BASE_COMPONENTS_INDTEXCOORDSCALE

#include <JSystem/J3D/J3DGraphBase/J3DStruct.hpp>

extern const J3DIndTexCoordScaleInfo j3dDefaultIndTexCoordScaleInfo;

class J3DIndTexCoordScale : public J3DIndTexCoordScaleInfo {
public:
	J3DIndTexCoordScale()
	{
		*(J3DIndTexCoordScaleInfo*)this = j3dDefaultIndTexCoordScaleInfo;
	}
	J3DIndTexCoordScale(const J3DIndTexCoordScaleInfo& info)
	{
		*(J3DIndTexCoordScaleInfo*)this = info;
	}
	~J3DIndTexCoordScale() { }

	u8 getScaleS() { return mScaleS; }
	u8 getScaleT() { return mScaleT; }

	void load(u32) const { }
};

#endif
