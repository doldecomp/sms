#include <GC2D/ExPane.hpp>
#include <JSystem/J2D/J2DScreen.hpp>

TExPane::TExPane(J2DScreen* screen, u32 tag)
{
	mPane              = screen->search(tag);
	mInitialBounds     = mPane->getBounds();
	mCurrentAlpha      = 255.0f;
	mAlphaStep         = 0.0f;
	mTargetAlpha       = 255;
	mOffsetAnimPending = false;
	mSizeAnimPending   = false;
	mAlphaAnimPending  = false;
}

TExPane::TExPane(JUTTexture* texture, GXCullMode cullMode) { }

bool TExPane::update()
{
	if (mOffsetAnimPending) {
		if (mOffsetInterpolator.update())
			mOffsetAnimPending = false;
		mPane->move(mInitialBounds.x1 + mOffsetInterpolator.getCurrentX(),
		            mInitialBounds.y1 + mOffsetInterpolator.getCurrentY());
	}

	if (mSizeAnimPending) {
		if (mSizeInterpolator.update())
			mSizeAnimPending = false;
		mPane->resize(mSizeInterpolator.getCurrentX(),
		              mSizeInterpolator.getCurrentY());
	}

	if (mAlphaAnimPending) {
		if ((mAlphaStep < 0.0f && mCurrentAlpha <= mTargetAlpha)
		    || (mAlphaStep >= 0.0f && mCurrentAlpha >= mTargetAlpha)) {
			mAlphaAnimPending = false;
		}

		mCurrentAlpha += mAlphaStep;

		// TODO: this is a fakematch, need to figure out the real inline
		s16 computedAlpha = mCurrentAlpha < 0.0f ? 0.0f : mCurrentAlpha;
		s16 finalAlpha;
		if (computedAlpha > 255)
			finalAlpha = (int)255;
		else
			finalAlpha = (int)computedAlpha;
		mPane->setAlpha((int)finalAlpha);
	}

	if (isThing2())
		return true;

	return false;
}
