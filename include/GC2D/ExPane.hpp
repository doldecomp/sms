#ifndef GC2D_EX_PANE_HPP
#define GC2D_EX_PANE_HPP

#include <GC2D/Coord2D.hpp>
#include <JSystem/JUtility/JUTRect.hpp>
#include <JSystem/J2D/J2DPane.hpp>
#include <dolphin/gx/GXEnum.h>

class J2DScreen;
class JUTTexture;
class J2DPane;

/**
 * @brief Wrapper around J2DPane that supports animating the pane's position,
 * size, and alpha over a specified number of frames.
 */
class TExPane {
public:
	TExPane(JUTTexture*, GXCullMode);
	TExPane(J2DScreen*, u32);

	/// Performs one frame of the animations.
	bool update();

	/// Initiates a pane's position animation.
	void setPaneOffset(s32 time, s32 target_x, s32 target_y, s32 initial_x,
	                   s32 initial_y)
	{
		mOffsetInterpolator.setValue(time, target_x, target_y, initial_x,
		                             initial_y);
		mPane->move(mInitialBounds.x1 + initial_x,
		            mInitialBounds.y1 + initial_y);
		mOffsetAnimPending = true;
	}

	/// Restarts the position animation from wherever it currently is.
	/// Confirmed by the ROM: TGCConsole2::loadAfter inlines exactly this.
	void updatePaneOffset(s32 time, s32 target_x, s32 target_y)
	{
		setPaneOffset(time, target_x, target_y,
		              mOffsetInterpolator.getCurrentX(),
		              mOffsetInterpolator.getCurrentY());
	}

	/// Initiates a pane's size animation.
	void setPaneSize(s32 time, s32 target_w, s32 target_h, s32 initial_w,
	                 s32 initial_h)
	{
		mSizeInterpolator.setValue(time, target_w, target_h, initial_w,
		                           initial_h);
		mPane->resize(initial_w, initial_h);
		mSizeAnimPending = true;
	}

	// fabricated
	void updatePaneSize(s32 time, s32 target_w, s32 target_h)
	{
		setPaneSize(time, target_w, target_h, mSizeInterpolator.getCurrentX(),
		            mSizeInterpolator.getCurrentY());
	}

	/// Initiates a pane's alpha animation.
	void setPaneAlpha(s32 time, s16 target_alpha, s16 initial_alpha)
	{
		s16 a = initial_alpha < 0 ? s16(0) : initial_alpha;
		if (a > 255)
			a = 255;
		mPane->setAlpha(a);

		mCurrentAlpha     = initial_alpha;
		mAlphaStep        = f32(target_alpha - initial_alpha) / time;
		mTargetAlpha      = target_alpha;
		mAlphaAnimPending = true;
	}

	// Fabricated name: the map has no setCenteredSize, so it was inlined at
	// every site. The body is right, though -- ConsoleStr::processReady
	// matches exactly with the offsets as written here.
	void setCenteredSize(s32 time, s32 target_w, s32 target_h, s32 initial_w,
	                     s32 initial_h)
	{
		setPaneSize(time, target_w, target_h, initial_w, initial_h);
		s32 initH = mInitialBounds.getHeight();
		s32 initW = mInitialBounds.getWidth();
		setPaneOffset(time, (initW - target_w) * 0.5f,
		              (initH - target_h) * 0.5f, (initW - initial_w) * 0.5f,
		              (initH - initial_h) * 0.5f);
	}

	// Fabricated name. CardSave/CardLoad initialize the size from the
	// offset interpolator here, rather than the size interpolator.
	void updateCenteredSize(s32 time, s32 target_w, s32 target_h)
	{
		setCenteredSize(time, target_w, target_h,
		                mOffsetInterpolator.getCurrentX(),
		                mOffsetInterpolator.getCurrentY());
	}

	J2DPane* getPane() const { return mPane; }
	const JUTRect& getInitialBounds() const { return mInitialBounds; }

	bool isBoundsAnimationCompleted() const
	{
		bool result = false;
		if (!mOffsetAnimPending && !mSizeAnimPending)
			result = true;
		return result;
	}

	bool isThing2() const
	{
		bool result = false;
		if (isBoundsAnimationCompleted() && !mAlphaAnimPending)
			result = true;
		return result;
	}

	bool isInterpolatorAtZero() const
	{
		return mOffsetInterpolator.getCurrentX() == 0
		       && mOffsetInterpolator.getCurrentY() == 0;
	}

public:
	/* 0x0 */ J2DPane* mPane;
	/* 0x4 */ JUTRect mInitialBounds;
	/* 0x14 */ TCoord2D mOffsetInterpolator;
	/* 0x2C */ TCoord2D mSizeInterpolator;
	/* 0x44 */ f32 mCurrentAlpha;
	/* 0x48 */ f32 mAlphaStep;
	/* 0x4C */ s16 mTargetAlpha;
	/* 0x4E */ bool mOffsetAnimPending;
	/* 0x4F */ bool mSizeAnimPending;
	/* 0x50 */ bool mAlphaAnimPending;
};

#endif
