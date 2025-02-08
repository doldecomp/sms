#ifndef J3D_GRAPH_BASE_COMPONENTS_TEXMTX
#define J3D_GRAPH_BASE_COMPONENTS_TEXMTX

#include <JSystem/J3D/J3DGraphBase/J3DStruct.hpp>

extern const J3DTexMtxInfo j3dDefaultTexMtxInfo;

class J3DTexMtx : public J3DTexMtxInfo {
public:
	J3DTexMtx() { J3DTexMtxInfo::operator=(j3dDefaultTexMtxInfo); }

	void calc();
	void load(u32) const;

	Mtx& getViewMtx() { return mViewMtx; }
	void setViewMtx(const Mtx viewMtx) { MTXCopy((MtxPtr)viewMtx, mViewMtx); }

	// Made up
	u32 getMode() { return mInfo & 0x7F; }

	/* 0x64 */ Mtx mTotalMtx;
	/* 0x94 */ Mtx mViewMtx;
};

#endif
