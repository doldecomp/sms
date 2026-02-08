#include <JSystem/JDrama/JDREfbCtrl.hpp>
#include <JSystem/JDrama/JDREfbSetting.hpp>
#include <dolphin/gx.h>

using namespace JDrama;

void TEfbCtrl::perform(u32 param_1, TGraphics* param_2)
{

	if (!(param_1 & 0x80))
		return;

	GXSetColorUpdate(!unk20.check(0x100));
	GXSetAlphaUpdate(!unk20.check(0x200));
	GXSetZMode(!unk20.check(0x400), GX_LEQUAL, GX_TRUE);

	param_2->mDisplayRect = unk10;
}

void TEfbCtrl::setSrcRect(const TRect& param_1)
{
	unk10 = param_1;
	unk10.intersect(TRect(0, 0, 640, 528));

	unk10.x1 = (param_1.x1 + 1U) & ~1U;
	unk10.y1 = (param_1.y1 + 1U) & ~1U;
	unk10.x2 = param_1.x2 & ~1U;
	unk10.y2 = param_1.y2 & ~1U;

	unk10.normalize();
}

void TEfbCtrlDisp::perform(u32 param_1, TGraphics* param_2)
{
	if ((param_1 & 0x80) != 0) {
		IssueGXPixelFormatSetting(param_2->mRenderMode,
		                          param_2->unkFC.check(0x8),
		                          param_2->unkFC.check(0x10));
	}

	if ((param_1 & 0x80) != 0) {
		GXSetColorUpdate(!unk20.check(0x100));
		GXSetAlphaUpdate(!unk20.check(0x200));
		GXSetZMode(!unk20.check(0x400), GX_LEQUAL, 1);

		param_2->mDisplayRect = unk10;
	}

	if (((param_1 & 8) != 0) && !param_2->unkFC.check(0x40)) {
		IssueGXCopyDisp(param_2->mFrameBuffer, param_2->mDisplayRect,
		                param_2->mRenderMode, param_2->mClearColor,
		                param_2->mClearZ, param_2->mFBClamp,
		                param_2->unkFC.mValue);
	}
}

TEfbCtrlTex::TEfbCtrlTex(const char* name, TFlagT<u16> flag)
    : TEfbCtrl(name, flag)
    , mImagePtr(nullptr)
    , mFbClamp((GXFBClamp)(GX_CLAMP_TOP | GX_CLAMP_BOTTOM))
    , unk38(0, 0, 0, 0)
    , unk3C(0xffffff)
    , unk40(nullptr)
    , unk44(nullptr)
{
}

void TEfbCtrlTex::setTexAttb(const GXTexObj& param_1)
{
	GXTexWrapMode wrap_s;
	GXTexWrapMode wrap_t;
	u16 width;
	u16 height;
	u8 mipmap;

	GXGetTexObjAll(&param_1, &mImagePtr, &width, &height, &mTexFmt, &wrap_s,
	               &wrap_t, &mipmap);

	// TODO: inline
	char trash[0x10];
	mWidth  = width;
	mHeight = height;
}

void TEfbCtrlTex::perform(u32 param_1, TGraphics* param_2)
{
	if ((param_1 & 0x80) != 0) {
		IssueGXPixelFormatSetting(unk20.check(0x800), unk20.check(0x8),
		                          unk20.check(0x10), false, false);
	}

	if ((param_1 & 0x80) != 0) {
		GXSetColorUpdate(!unk20.check(0x100));
		GXSetAlphaUpdate(!unk20.check(0x200));
		GXSetZMode(!unk20.check(0x400), GX_LEQUAL, 1);

		param_2->mDisplayRect = unk10;
	}

	if ((param_1 & 8)) {
		GXSetCopyClamp(mFbClamp);
		IssueGXSetCopyFilter(unk20.check(0x800), (u8(*)[2])unk40,
		                     unk20.check(0x20), unk44);

		if (mImagePtr != nullptr) {
			u32 iVar2 = IssueGXSetCopyClear(unk38, unk3C, unk20.get());
			GXSetTexCopySrc(unk10.x1, unk10.y1, unk10.x2, unk10.y2);
			GXSetTexCopyDst(mWidth, mHeight, mTexFmt, unk20.check(0x1000));
			GXCopyTex(mImagePtr, iVar2);
		}
	}
}
