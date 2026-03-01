#include <GC2D/ExPane.hpp>
#include <JSystem/J2D/J2DScreen.hpp>

TExPane::TExPane(J2DScreen* param_1, u32 param_2)
{
	mPane              = param_1->search(param_2);
	mInitialBounds     = mPane->getBounds();
	mCurrentAlpha      = 255.0f;
	mAlphaStep         = 0.0f;
	mTargetAlpha       = 255;
	mOffsetAnimPending = false;
	mSizeAnimPending   = false;
	mAlphaAnimPending  = false;
}

TExPane::TExPane(JUTTexture*, GXCullMode) { }

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
		s16 fVar1 = mCurrentAlpha < 0.0f ? 0.0f : mCurrentAlpha;
		s16 fVar2;
		if (fVar1 > 255)
			fVar2 = (int)255;
		else
			fVar2 = (int)fVar1;
		mPane->setAlpha((int)fVar2);
	}

	if (isThing2())
		return true;

	return false;
}
