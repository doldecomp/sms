#ifndef J3D_GRAPH_BASE_COMPONENTS_LIGHTOBJ
#define J3D_GRAPH_BASE_COMPONENTS_LIGHTOBJ

#include <JSystem/J3D/J3DGraphBase/J3DStruct.hpp>

extern const J3DLightInfo j3dDefaultLightInfo;

class J3DLightObj : public J3DLightInfo {
public:
	J3DLightObj() { setDefault(); }

	// Completely made up to force J3DLightInfo::operator= to NOT inline
	void setDefault() { setInfo(j3dDefaultLightInfo); }
	void setInfo(const J3DLightInfo& info)
	{
		J3DLightInfo::operator=(j3dDefaultLightInfo);
	}

	void load(u32) const;

public:
	char unk34[0x40];
};

#endif
