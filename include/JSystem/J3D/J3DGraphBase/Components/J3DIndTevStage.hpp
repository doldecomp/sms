#ifndef J3D_GRAPH_BASE_COMPONENTS_INDTEVSTAGE
#define J3D_GRAPH_BASE_COMPONENTS_INDTEVSTAGE

#include <JSystem/JRenderer.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DStruct.hpp>

extern const J3DIndTevStageInfo j3dDefaultIndTevStageInfo;

class J3DIndTevStage : public J3DIndTevStageInfo {
public:
	J3DIndTevStage()
	{
		J3DIndTevStageInfo::operator=(j3dDefaultIndTevStageInfo);
	}

	J3DIndTevStage& operator=(const J3DIndTevStage& other)
	{
		mIndStage  = other.mIndStage;
		mIndFormat = other.mIndFormat;
		mBiasSel   = other.mBiasSel;
		mMtxSel    = other.mMtxSel;
		mWrapS     = other.mWrapS;
		mWrapT     = other.mWrapT;
		mPrev      = other.mPrev;
		mLod       = other.mLod;
		mAlphaSel  = other.mAlphaSel;
		return *this;
	}

	void load(u32 param_1)
	{
		JRNSetTevIndirect((GXTevStageID)param_1, (GXIndTexStageID)mIndStage,
		                  (GXIndTexFormat)mIndFormat, (GXIndTexBiasSel)mBiasSel,
		                  (GXIndTexMtxID)mMtxSel, (GXIndTexWrap)mWrapS,
		                  (GXIndTexWrap)mWrapT, mPrev, mLod,
		                  (GXIndTexAlphaSel)mAlphaSel);
	}
};

#endif
