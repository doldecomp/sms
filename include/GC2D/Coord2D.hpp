#ifndef GC2D_COORD2D_HPP
#define GC2D_COORD2D_HPP

#include <JSystem/JGeometry/JGVec2.hpp>

/**
 * @brief Simple interpolator. Moves a 2D point from a starting position to a
 * target position at a constant speed over the specified amount of frames.
 */
class TCoord2D {
public:
	TCoord2D();

	/// Set the target and starting positions, and calculate the velocity.
	void setValue(s32 time, f32 target_x, f32 target_y, f32 start_x,
	              f32 start_y);

	/// Updates the current position.
	bool update();

	template <class T> T roundf(f32 v) const
	{
		return v + (v > 0.0f ? 0.5f : -0.5f);
	}

	int getCurrentX() const { return roundf<int>(mCurrentPos.x); }
	int getCurrentY() const { return roundf<int>(mCurrentPos.y); }

	f32 getVelocityX() const { return mVelocity.x; }
	f32 getVelocityY() const { return mVelocity.y; }

	bool isReachedTarget() const
	{
		return mCurrentPos.x == mTargetPos.x && mCurrentPos.y == mTargetPos.y
		           ? true
		           : false;
	}

public:
	/* 0x0 */ JGeometry::TVec2<f32> mCurrentPos;
	/* 0x8 */ JGeometry::TVec2<f32> mTargetPos;
	/* 0x10 */ JGeometry::TVec2<f32> mVelocity;
};

#endif // GC2D_COORD2D_HPP
