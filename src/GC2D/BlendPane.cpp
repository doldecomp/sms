#include <GC2D/BlendPane.hpp>
#include <JSystem/J2D/J2DScreen.hpp>

TBlendPane::TBlendPane(J2DScreen* screen, u32 tag)
    : TBoundPane(screen, tag)
{
	mBlendActive   = false;
	mBlendStep     = 0.0f;
	mBlendProgress = 0.0f;
}

bool TBlendPane::update()
{
	bool result = TBoundPane::update();
	if (mBlendActive) {
		if (mBlendProgress >= 1.0f) {
			mBlendProgress = 1.0f;
			mBlendActive   = false;
		}

		f32 inverseProgress;
		f32 progress;
		inverseProgress     = 1.0f - (progress = mBlendProgress);
		J2DPicture* picture = (J2DPicture*)getPane();
		picture->setBlendKonstColor(progress, inverseProgress, 1.0f, 1.0f);
		picture->setBlendKonstAlpha(progress, inverseProgress, 1.0f, 1.0f);
		mBlendProgress += mBlendStep;
	}

	return (result && !mBlendActive) ? true : false;
}
