#ifndef J3D_GRAPH_BASE_COMPONENTS_TEVORDER
#define J3D_GRAPH_BASE_COMPONENTS_TEVORDER

#include <JSystem/J3D/J3DGraphBase/J3DStruct.hpp>

extern const J3DTevOrderInfo j3dDefaultTevOrderInfoNull;

class J3DTevOrder : public J3DTevOrderInfo {
public:
	J3DTevOrder() { J3DTevOrderInfo::operator=(j3dDefaultTevOrderInfoNull); }
	J3DTevOrder(const J3DTevOrderInfo& info)
	{
		J3DTevOrderInfo::operator=(info);
	}

	J3DTevOrder& operator=(const J3DTevOrder& other)
	{
		mTexCoord  = other.mTexCoord;
		mTexMap    = other.mTexMap;
		mColorChan = other.mColorChan;
		return *this;
	}

	J3DTevOrderInfo& getTevOrderInfo() { return *this; }

	void load(u32) const;
};

#endif
