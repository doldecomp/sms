#ifndef J3D_TEXTURE_HPP
#define J3D_TEXTURE_HPP

#include <JSystem/ResTIMG.hpp>

class J3DTexture {
public:
	/* 0x0 */ u16 mResourceCount;
	/* 0x4 */ ResTIMG* mResources;

	ResTIMG* getResTIMG(u16 entry) const { return &mResources[entry]; }
};

#endif
