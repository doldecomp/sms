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
		mFadeFrame += 1;
		s32 endFrame = mFadeDelayFrames + mFadeDurationFrames;
		if (mFadeFrame > endFrame) {
			mFadeStatus = FADE_STATUS_FULLY_FADED_OUT;
			mUseBoxWipe = 0;
		} else {
			if (mFadeFrame > mFadeDelayFrames) {
				s32 fadeFrame = mFadeFrame - 1 - mFadeDelayFrames;
				mFadeColor.a  = ((u16)fadeFrame * 0xff) / mFadeDurationFrames;
			}
		}
		break;
	}
}

void TShineFader::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_MOVE)
		update();

	if (cue & CUE_DRAW)
		draw(graphics->getViewport());
}

bool TShineFader::registFadeout(u16 duration_frames, u16 delay_frames)
{
	bool result = false;
	if (duration_frames == 0) {
		duration_frames = 1;
	}

	switch (mFadeStatus) {
	case FADE_STATUS_FULLY_FADED_IN:
		result              = true;
		mFadeStatus         = FADE_STATUS_FADING_OUT;
		mFadeFrame          = 0;
		mFadeDurationFrames = duration_frames;
		mFadeDelayFrames    = delay_frames;
		break;
	}

	return result;
}

void TShineFader::load(JSUMemoryInputStream& iStream)
{
	TNameRef::load(iStream);
}
