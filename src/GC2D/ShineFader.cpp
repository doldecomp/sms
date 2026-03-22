#include <GC2D/ScrnFader.hpp>

void TShineFader::update()
{
	switch (mFadeStatus) {
	case FADE_STATUS_FULLY_FADED_IN:
		mFadeColor.a = 0x00;
		break;
	case FADE_STATUS_FULLY_FADED_OUT:
		mFadeColor.a = 0xff;
		break;
	case FADE_STATUS_FADING_OUT:
		u16 unk12Copy = unk12;
		unk12Copy += 1;
		unk12   = unk12Copy;
		s32 val = unk38 + unk10;
		if (unk12 > val) {
			mFadeStatus = FADE_STATUS_FULLY_FADED_OUT;
			unk1C       = 0;
		} else {
			if (unk12 > unk38) {
				val = unk12 - 1;
				val -= unk38;
				mFadeColor.a = (u8)(((u16)val * 0xff) / unk10);
			}
		}
		break;
	}
}

void TShineFader::perform(u32 param1, JDrama::TGraphics* param2)
{
	if ((param1 & 0x1) != 0) {
		update();
	}

	if ((param1 & 0x8) != 0) {
		draw(param2->getViewport());
	}
}

bool TShineFader::registFadeout(u16 param1, u16 param2)
{
	bool result = false;
	if (param1 == 0) {
		param1 = 1;
	}

	switch (mFadeStatus) {
	case FADE_STATUS_FULLY_FADED_IN:
		result      = true;
		mFadeStatus = FADE_STATUS_FADING_OUT;
		unk12       = 0;
		unk10       = param1;
		unk38       = param2;
		break;
	}

	return result;
}

void TShineFader::load(JSUMemoryInputStream& iStream)
{
	TNameRef::load(iStream);
}
