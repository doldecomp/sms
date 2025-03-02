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

	u16 getNum() const { return mResourceCount; }
	ResTIMG* getResTIMG(u16 entry) const { return &mResources[entry]; }
	void setResTIMG(u16 entry, const ResTIMG& timg)
	{
		mResources[entry] = timg;
		mResources[entry].imageDataOffset
		    = ((mResources[entry].imageDataOffset + (u32)&timg
		        - (u32)(mResources + entry)));
		mResources[entry].paletteOffset
		    = ((mResources[entry].paletteOffset + (u32)&timg
		        - (u32)(mResources + entry)));
	}
};

#endif
