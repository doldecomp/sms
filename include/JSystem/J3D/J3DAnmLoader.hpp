#ifndef J3D_ANM_LOADER_HPP
#define J3D_ANM_LOADER_HPP

#include <JSystem/J3D/J3DModel.hpp>
#include <JSystem/J3D/J3DAnimation.hpp>

// NOTE: seems to be a plain struct w/ no methods
struct J3DTextureSRTInfo;

class J3DAnmTextureSRTKey : public J3DAnmBase {
public:
	typedef void IDK;
	IDK searchUpdateMaterialID(J3DModelData*);
	IDK calcTransform(float, u16, J3DTextureSRTInfo*) const;
	IDK calcPostTransform(float, u16, J3DTextureSRTInfo*) const;

	virtual ~J3DAnmTextureSRTKey();
};

#endif
