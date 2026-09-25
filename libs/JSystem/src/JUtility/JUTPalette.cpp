#include <JSystem/JUtility/JUTPalette.hpp>
#include <dolphin/gx.h>
#include <dolphin/os.h>

void JUTPalette::storeTLUT(GXTlut param_0, ResTLUT* tlut)
{
	if (tlut == NULL) {
		OSPanic("JUTPalette.cpp", 35, "JUTTexture: TLUT is NULL\n");
	}
	mTlutName     = param_0;
	mFormat       = tlut->format;
	mTransparency = tlut->transparency;
	mNumColors    = tlut->numColors;
	mColorTable   = tlut + 8;
	GXInitTlutObj(&mTlutObj, (void*)mColorTable, (GXTlutFmt)mFormat,
	              mNumColors);
}

void JUTPalette::storeTLUT(GXTlut param_0, GXTlutFmt param_1,
                           JUTTransparency param_2, u16 param_3, void* param_4)
{
	mTlutName     = param_0;
	mFormat       = param_1;
	mTransparency = param_2;
	mNumColors    = param_3;
	mColorTable   = (ResTLUT*)param_4;
	GXInitTlutObj(&mTlutObj, (void*)mColorTable, (GXTlutFmt)mFormat,
	              mNumColors);
}

JUTPalette::JUTPalette(GXTlut p1, GXTlutFmt p2, JUTTransparency p3, u16 p4,
                       void* p5)
{
	this->storeTLUT(p1, p2, p3, p4, p5);
}

bool JUTPalette::load()
{
	bool check = mNumColors != 0;
	if (check) {
		GXLoadTlut(&mTlutObj, mTlutName);
	}

	return check;
}
