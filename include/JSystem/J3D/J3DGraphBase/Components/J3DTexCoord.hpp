#ifndef J3D_GRAPH_BASE_COMPONENTS_TEXCOORD
#define J3D_GRAPH_BASE_COMPONENTS_TEXCOORD

#include <JSystem/J3D/J3DGraphBase/J3DStruct.hpp>

extern const J3DTexCoordInfo j3dDefaultTexCoordInfo[8];

class J3DTexCoord : public J3DTexCoordInfo {
public:
	J3DTexCoord() { J3DTexCoordInfo::operator=(j3dDefaultTexCoordInfo[0]); }

	J3DTexCoord& operator=(const J3DTexCoord& other)
	{
		mTexGenType = other.mTexGenType;
		mTexGenSrc  = other.mTexGenSrc;
		mTexGenMtx  = other.mTexGenMtx;
		return *this;
	}

	GXTexGenType getTexGenType() { return (GXTexGenType)mTexGenType; }
	GXTexGenSrc getTexGenSrc() { return (GXTexGenSrc)mTexGenSrc; }
	u8 getTexGenMtx() { return mTexGenMtx; }
};

#endif
