#include <Camera/Camera.hpp>
#include <Camera/cameralib.hpp>
#include <Camera/camerasave.hpp>
#include <Camera/CameraKindParam.hpp>
#include <Camera/CameraMarioData.hpp>
#include <Player/MarioAccess.hpp>

void CPolarSubCamera::killHeightPanWhenChangeCamMode_()
{
	bool bVar2 = false;

	if (isNotHeightPanCamMode_())
		bVar2 = true;

	switch (mPrevMode) {
	case CAMERA_MODE_MONTE_HANG:
	case CAMERA_MODE_HANG:
		bVar2 = true;
		break;
	}

	if (bVar2)
		killHeightPan_();
}

void CPolarSubCamera::execHeightPan_()
{
	bool marioIsAirborne = SMS_IsMarioTouchGround4cm() == false;
	f32 offset           = 0.0;
	if (marioIsAirborne)
		offset = mCurrentParams->mTargetAtJumpOffsetY;

	CLBChaseGeneralConstantSpecifySpeed(&mHeightPanOffset, offset,
	                                    mCurrentParams->mAtJumpOffsetSpeed);
	mCurrentTarget.mTarget.y += mHeightPanOffset;

	f32 deltaDesiredY = mCurrentTarget.unk18.y - mPreviousTarget.unk18.y;

	if (fabricatedInline()) {
		unk64 |= CAMERA_FLAG_UNK1;
		unk64 &= ~CAMERA_FLAG_UNK2;
		mCurrentTarget.mPosition.y = mPreviousTarget.mPosition.y;
		CLBChaseDecrease(&mCurrentTarget.mPosition.y, mCurrentTarget.unk18.y,
		                 mCurrentParams->mHeightPanChaseRateY, 0.0f);
		if (mCurrentTarget.unk28 != mPreviousTarget.unk28)
			mCurrentTarget.mPosition.y += deltaDesiredY;
	} else {
		if (unk64 & (CAMERA_FLAG_UNK1 | CAMERA_FLAG_UNK2)) {
			if (mCurrentTarget.unk28 != mPreviousTarget.unk28)
				mCurrentTarget.mPosition.y += deltaDesiredY;

			if (unk64 & CAMERA_FLAG_UNK1) {
				unk64 &= ~CAMERA_FLAG_UNK1;

				bool shouldInstaWarp = false;
				switch (mMode) {
				case CAMERA_MODE_JUMP_CODE:
				case CAMERA_MODE_ROCKET_JUMP:
					if (unk256 < mSaveEx->mPanWarpAngleX.get())
						shouldInstaWarp = true;
					break;
				}

				if (shouldInstaWarp) {
					mPosition.y = mCurrentTarget.mPosition.y
					    = mCurrentTarget.unk18.y;
					unk64 &= ~CAMERA_FLAG_UNK2;
				} else {
					unk64 |= CAMERA_FLAG_UNK2;
				}
			}

			if (mCurrentTarget.mPosition.y != mCurrentTarget.unk18.y) {
				BOOL stillChasing = CLBChaseSpecialDecrease(
				    &mCurrentTarget.mPosition.y, mCurrentTarget.unk18.y,
				    mSaveEx->mPanAfterMagnif.get(),
				    mSaveEx->mPanAfterMinHeight.get());
				if (!stillChasing && !marioIsAirborne)
					unk64 &= ~CAMERA_FLAG_UNK2;

			} else {
				if (!marioIsAirborne)
					unk64 &= ~CAMERA_FLAG_UNK2;
			}
		} else {
			mCurrentTarget.mPosition.y = mCurrentTarget.unk18.y;
		}
	}
}

void CPolarSubCamera::killHeightPan_()
{
	unk64 &= ~CAMERA_FLAG_UNK1;
	unk64 &= ~CAMERA_FLAG_UNK2;
	mCurrentTarget.mPosition.y = mCurrentTarget.unk18.y;

	mHeightPanOffset = 0.0f;
}
