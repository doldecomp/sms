#ifndef J3D_TEXTURE_HPP
#define J3D_TEXTURE_HPP

#include <JSystem/J3D/J3DAssert.hpp>
#include <JSystem/ResTIMG.hpp>
#include <stdint.h>

class J3DTexture {
public:
	/* 0x0 */ u16 mNum;
	/* 0x4 */ ResTIMG* mResources;

public:
	J3DTexture(u16 num, ResTIMG* res)
	    : mNum(num)
	    , mResources(res)
	{
		J3D_ASSERT_NULLPTR(res != nullptr || num == 0);
	}
	virtual ~J3DTexture() { }

	u16 getNum() const { return mNum; }
	ResTIMG* getResTIMG(u16 index) const
	{
		J3D_ASSERT_RANGE(index < mNum);
		return &mResources[index];
	}
	void setResTIMG(u16 index, const ResTIMG& timg)
	{
		J3D_ASSERT_RANGE(index < mNum);

		mResources[index] = timg;
		mResources[index].imageDataOffset
		    = ((mResources[index].imageDataOffset + (uintptr_t)&timg
		        - (uintptr_t)(mResources + index)));
		mResources[index].paletteOffset
		    = ((mResources[index].paletteOffset + (uintptr_t)&timg
		        - (uintptr_t)(mResources + index)));
	}
};

#endif
