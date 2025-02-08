#ifndef J3D_GRAPH_BASE_COMPONENTS_FOG
#define J3D_GRAPH_BASE_COMPONENTS_FOG

#include <JSystem/JRenderer.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DStruct.hpp>

extern const J3DFogInfo j3dDefaultFogInfo;

class J3DFog : public J3DFogInfo {
public:
	J3DFog() { J3DFogInfo::operator=(j3dDefaultFogInfo); }

	void load() const
	{
		J3DGDSetFog(GXFogType(mType), mStartZ, mEndZ, mNearZ, mFarZ, mColor);
		JRNISetFogRangeAdj(mAdjEnable, mCenter, (GXFogAdjTable*)mFogAdjTable);
	}
};

#endif
