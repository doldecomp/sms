#ifndef J3D_GRAPH_BASE_COMPONENTS_BLEND
#define J3D_GRAPH_BASE_COMPONENTS_BLEND

#include <JSystem/J3D/J3DGraphBase/J3DStruct.hpp>
#include <dolphin/gd.h>

extern const J3DBlendInfo j3dDefaultBlendInfo;

class J3DBlend : public J3DBlendInfo {
public:
	J3DBlend() { *(J3DBlendInfo*)this = j3dDefaultBlendInfo; }

	void setBlendInfo(const J3DBlendInfo& info)
	{
		J3DBlendInfo::operator=(info);
	}

	J3DBlend& operator=(const J3DBlend& other)
	{
		mBlendMode = other.mBlendMode;
		mSrcFactor = other.mSrcFactor;
		mDstFactor = other.mDstFactor;
		mLogicOp   = other.mLogicOp;
		return *this;
	}

	void load()
	{
		GDSetBlendMode((GXBlendMode)mBlendMode, (GXBlendFactor)mSrcFactor,
		               (GXBlendFactor)mDstFactor, (GXLogicOp)mLogicOp);
	}
};

#endif
