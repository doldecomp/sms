#ifndef JDR_SM_J3D_SCN_HPP
#define JDR_SM_J3D_SCN_HPP

#include <JSystem/JDrama/JDRViewObjPtrList.hpp>
#include <JSystem/JDrama/JDRGraphics.hpp>
#include <dolphin/types.h>

class JSUMemoryInputStream;
class J3DDrawBuffer;

namespace JDrama {

class TLightMap;

class TSmJ3DScn : public TViewObjPtrListT<TViewObj, TViewObj> {
public:
	TSmJ3DScn(const char*, s32);
	void perform(u32, TGraphics*);
	void loadSuper(JSUMemoryInputStream&);
	~TSmJ3DScn() { }

public:
	/* 0x20 */ TLightMap* mLightMap;
	/* 0x24 */ J3DDrawBuffer** mDrawBuffers;
	/* 0x28 */ s32 mDrawBufferCount;
}; // size 0x2C

}; // namespace JDrama

#endif
