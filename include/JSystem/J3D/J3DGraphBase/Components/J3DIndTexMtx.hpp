#ifndef J3D_GRAPH_BASE_COMPONENTS_INDTEXMTX
#define J3D_GRAPH_BASE_COMPONENTS_INDTEXMTX

#include <JSystem/J3D/J3DGraphBase/J3DStruct.hpp>
#include <JSystem/JRenderer.hpp>

extern const J3DIndTexMtxInfo j3dDefaultIndTexMtxInfo;

class J3DIndTexMtx : public J3DIndTexMtxInfo {
public:
	J3DIndTexMtx() { *(J3DIndTexMtxInfo*)this = j3dDefaultIndTexMtxInfo; }
	J3DIndTexMtx(const J3DIndTexMtxInfo& info)
	{
		*(J3DIndTexMtxInfo*)this = info;
	}

	~J3DIndTexMtx() { }

	void load(u32 idx)
	{
		JRNSetIndTexMtx((GXIndTexMtxID)idx, mOffsetMtx, mScaleExp);
	}
};

#endif
