#include <GC2D/Coord2D.hpp>
#include <Camera/cameralib.hpp>

TCoord2D::TCoord2D()
{
	mCurrentPos.zero();
	mTargetPos.zero();
	mVelocity.zero();
}

void TCoord2D::setValue(s32 time, f32 target_x, f32 target_y, f32 start_x,
                        f32 start_y)
{
	mTargetPos.set(target_x, target_y);
	mCurrentPos.set(start_x, start_y);
	if (time <= 0) {
		mVelocity.set(0.0f, 0.0f);
	} else {
		mVelocity.set((target_x - start_x) / time, (target_y - start_y) / time);
	}
}

bool TCoord2D::update()
{
	bool result = false;

	CLBChaseGeneralConstantSpecifySpeed(&mCurrentPos.x, mTargetPos.x,
	                                    getVelocityX());
	CLBChaseGeneralConstantSpecifySpeed(&mCurrentPos.y, mTargetPos.y,
	                                    getVelocityY());

	if (isReachedTarget()) {
		mVelocity.set(0.0f, 0.0f);
		result = true;
	}

	return result;
}
