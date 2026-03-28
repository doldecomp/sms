#include <JSystem/JDrama/JDREfbCtrl.hpp>
#include <JSystem/JDrama/JDREfbSetting.hpp>
#include <dolphin/gx.h>
#include <macros.h>

using namespace JDrama;

void TEfbCtrl::perform(u32 param_1, TGraphics* param_2)
{
	if (param_1 & 0x80) {
		GXSetColorUpdate(!unk20.check(0x100));
		GXSetAlphaUpdate(!unk20.check(0x200));
		GXSetZMode(!unk20.check(0x400), GX_LEQUAL, GX_TRUE);
		param_2->setDisplayRect(unk10);
	}

	if (param_1 & 0x8) {
		// no-op
	}
}

void TEfbCtrl::setSrcRect(const TRect& param_1)
{
	unk10 = param_1;
	unk10.intersect(TRect(0, 0, 640, 528));

	unk10.x1 = ALIGN_NEXT(param_1.x1, 2);
	unk10.y1 = ALIGN_NEXT(param_1.y1, 2);
	unk10.x2 = ALIGN_PREV(param_1.x2, 2);
	unk10.y2 = ALIGN_PREV(param_1.y2, 2);

	unk10.normalize();
}

void TEfbCtrlDisp::perform(u32 param_1, TGraphics* param_2)
{
	if (param_1 & 0x80) {
		IssueGXPixelFormatSetting(param_2->getRenderMode(),
		                          param_2->getUnkFC().check(0x8),
		                          param_2->getUnkFC().check(0x10));
	}

	TEfbCtrl::perform(param_1, param_2);

	if (param_1 & 0x8) {
		if (!param_2->getUnkFC().check(0x40))
			IssueGXCopyDisp(param_2->getFrameBuffer(),
			                param_2->getDisplayRect(), param_2->getRenderMode(),
			                param_2->getClearColor(), param_2->getClearZ(),
			                param_2->getFBClamp(), param_2->getUnkFC().get());
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

	mWidth  = (u32)width;
	mHeight = (u32)height;
}

void TEfbCtrlTex::perform(u32 param_1, TGraphics* param_2)
{
	if (param_1 & 0x80) {
		IssueGXPixelFormatSetting(unk20.check(0x800), unk20.check(0x8),
		                          unk20.check(0x10), false, false);
	}

	TEfbCtrl::perform(param_1, param_2);

	if (param_1 & 0x8) {
		GXSetCopyClamp(mFbClamp);
		IssueGXSetCopyFilter(unk20.check(0x800), unk40, unk20.check(0x20),
		                     unk44);

		if (mImagePtr != nullptr) {
			bool doClear = IssueGXSetCopyClear(unk38, unk3C, unk20.get());
			GXSetTexCopySrc(unk10.x1, unk10.y1, unk10.getWidth(),
			                unk10.getHeight());
			GXSetTexCopyDst(mWidth, mHeight, mTexFmt, unk20.check(0x1000));
			GXCopyTex(mImagePtr, doClear);
		}
	}
}
