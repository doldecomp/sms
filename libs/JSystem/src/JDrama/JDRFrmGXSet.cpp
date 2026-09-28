#include <JSystem/JDrama/JDRFrmGXSet.hpp>

using namespace JDrama;

void TFrmGXSet::perform(u32 cue, TGraphics* graphics)
{
	if (cue & CUE_DRAW) {
		graphics->mFrameBuffer = unk10->getCurrentFrameBuffer();
		graphics->mRenderMode  = unk10->getRenderMode();

		graphics->getUnkFC().setBit(0x1, unk10->unk64.check(0x1));
		graphics->getUnkFC().setBit(0x2, unk10->unk64.check(0x2));
		graphics->getUnkFC().setBit(0x4, unk10->unk64.check(0x4));
		graphics->getUnkFC().setBit(0x8, unk10->unk64.check(0x8));
		graphics->getUnkFC().setBit(0x10, unk10->unk64.check(0x10));
		graphics->getUnkFC().setBit(0x20, unk10->unk64.check(0x20));
		graphics->getUnkFC().setBit(0x40, unk10->unk64.check(0x40));

		graphics->mFBClamp    = unk10->getFBClamp();
		graphics->mClearColor = unk10->getClearColor();
		graphics->mClearZ     = unk10->getClearZ();
	}
}
