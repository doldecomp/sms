#ifndef J3D_TEXTURE_HPP
#define J3D_TEXTURE_HPP

#include <JSystem/ResTIMG.hpp>

class J3DTexture {
public:
	/* 0x0 */ u16 mResourceCount;
	/* 0x4 */ ResTIMG* mResources;

	J3DTexture(u16 num, ResTIMG* res)
	    : mResourceCount(num)
	    , mResources(res)
	{
	}
	virtual ~J3DTexture() { }

	ResTIMG* getResTIMG(u16 entry) const { return &mResources[entry]; }
	u16 getNum() const { return mResourceCount; }
};

#endif
