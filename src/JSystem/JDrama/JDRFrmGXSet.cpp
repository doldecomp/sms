#include <JSystem/JDrama/JDRFrmGXSet.hpp>

using namespace JDrama;

void TFrmGXSet::perform(u32 param_1, TGraphics* param_2)
{
	if (param_1 & 0x8) {
		param_2->mFrameBuffer = unk10->getCurrentFrameBuffer();
		param_2->mRenderMode  = unk10->getRenderMode();

		// TODO: wtf is going on here? Maybe TFlagT magic? Maybe bitfields?
		param_2->getUnkFC().setBit(0x1, unk10->unk64 & 0x1);
		param_2->getUnkFC().setBit(0x2, unk10->unk64 & 0x2);
		param_2->getUnkFC().setBit(0x4, unk10->unk64 & 0x4);
		param_2->getUnkFC().setBit(0x8, unk10->unk64 & 0x8);
		param_2->getUnkFC().setBit(0x10, unk10->unk64 & 0x10);
		param_2->getUnkFC().setBit(0x20, unk10->unk64 & 0x20);
		param_2->getUnkFC().setBit(0x40, unk10->unk64 & 0x40);

		param_2->mFBClamp    = unk10->getFBClamp();
		param_2->mClearColor = unk10->getClearColor();
		param_2->mClearZ     = unk10->getClearZ();
	}
}
