#include <JSystem/JDrama/JDREfbCtrl.hpp>
#include <JSystem/JDrama/JDREfbSetting.hpp>
#include <dolphin/gx.h>
#include <macros.h>

using namespace JDrama;

void TEfbCtrl::perform(u32 cue, TGraphics* graphics)
{
	if (cue & CUE_DRAW_INIT) {
		GXSetColorUpdate(!unk20.check(0x100));
		GXSetAlphaUpdate(!unk20.check(0x200));
		GXSetZMode(!unk20.check(0x400), GX_LEQUAL, GX_TRUE);
		graphics->setDisplayRect(unk10);
	}

	if (cue & CUE_DRAW) {
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

void TEfbCtrlDisp::perform(u32 cue, TGraphics* graphics)
{
	if (cue & CUE_DRAW_INIT) {
		IssueGXPixelFormatSetting(graphics->getRenderMode(),
		                          graphics->getUnkFC().check(0x8),
		                          graphics->getUnkFC().check(0x10));
	}

	TEfbCtrl::perform(cue, graphics);

	if (cue & CUE_DRAW) {
		if (!graphics->getUnkFC().check(0x40))
			IssueGXCopyDisp(graphics->getFrameBuffer(),
			                graphics->getDisplayRect(),
			                graphics->getRenderMode(),
			                graphics->getClearColor(), graphics->getClearZ(),
			                graphics->getFBClamp(), graphics->getUnkFC().get());
	}
}

void TEfbCtrlDisp::setSrcRect(const GXRenderModeObj& obj)
{
	TRect rect(0, 0, obj.fbWidth, obj.efbHeight);
	TEfbCtrl::setSrcRect(rect);
}

TEfbCtrlTex::TEfbCtrlTex(const char* name, TFlagT<u16> flag)
    : TEfbCtrl(name, flag)
    , mImagePtr(nullptr)
    , mFbClamp((GXFBClamp)(GX_CLAMP_TOP | GX_CLAMP_BOTTOM))
    , mClearColor(0, 0, 0, 0)
    , mClearZ(0xffffff)
    , mSamplePattern(nullptr)
    , mVFilter(nullptr)
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

	setDstSize(TSize(width, height));
}

void TEfbCtrlTex::perform(u32 cue, TGraphics* graphics)
{
	if (cue & CUE_DRAW_INIT) {
		IssueGXPixelFormatSetting(unk20.check(0x800), unk20.check(0x8),
		                          unk20.check(0x10), false, false);
	}

	TEfbCtrl::perform(cue, graphics);

	if (cue & CUE_DRAW) {
		GXSetCopyClamp(mFbClamp);
		IssueGXSetCopyFilter(unk20.check(0x800), mSamplePattern,
		                     unk20.check(0x20), mVFilter);

		if (mImagePtr != nullptr) {
			bool doClear
			    = IssueGXSetCopyClear(mClearColor, mClearZ, unk20.get());
			GXSetTexCopySrc(unk10.x1, unk10.y1, unk10.getWidth(),
			                unk10.getHeight());
			GXSetTexCopyDst(mSize.mWidth, mSize.mHeight, mTexFmt,
			                unk20.check(0x1000));
			GXCopyTex(mImagePtr, doClear);
		}
	}
}
