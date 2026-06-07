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
	f32 fVar10           = 0.0;
	if (marioIsAirborne)
		fVar10 = mCurrentParams->mTargetAtJumpOffsetY;

	CLBChaseGeneralConstantSpecifySpeed(&unk24C, fVar10,
	                                    mCurrentParams->mAtJumpOffsetSpeed);
	mCurrentTarget.mTarget.y += unk24C;
	f32 fVar1 = mCurrentTarget.unk18.y - mPreviousTarget.unk18.y;

	if (fabricatedInline()) {
		unk64 |= 0x1;
		unk64 &= ~0x2;
		mCurrentTarget.mPosition.y = mPreviousTarget.mPosition.y;
		CLBChaseDecrease(&mCurrentTarget.mPosition.y, mCurrentTarget.unk18.y,
		                 mCurrentParams->mHeightPanChaseRateY, 0.0f);
		if (mCurrentTarget.unk28 != mPreviousTarget.unk28)
			mCurrentTarget.mPosition.y += fVar1;
	} else {
		if (unk64 & 0x3) {
			if (mCurrentTarget.unk28 != mPreviousTarget.unk28)
				mCurrentTarget.mPosition.y += fVar1;

			if (unk64 & 0x1) {
				unk64 &= ~0x1;

				bool bVar7 = false;
				switch (mMode) {
				case CAMERA_MODE_JUMP_CODE:
				case CAMERA_MODE_ROCKET_JUMP:
					if (unk256 < mSaveEx->mPanWarpAngleX.get())
						bVar7 = true;
					break;
				}

				if (bVar7) {
					mPosition.y = mCurrentTarget.mPosition.y
					    = mCurrentTarget.unk18.y;
					unk64 &= ~0x2;
				} else {
					unk64 |= 0x2;
				}
			}

			if (mCurrentTarget.mPosition.y != mCurrentTarget.unk18.y) {
				BOOL pTVar5 = CLBChaseSpecialDecrease(
				    &mCurrentTarget.mPosition.y, mCurrentTarget.unk18.y,
				    mSaveEx->mPanAfterMagnif.get(),
				    mSaveEx->mPanAfterMinHeight.get());
				if (!pTVar5 && !marioIsAirborne)
					unk64 &= ~0x2;

			} else {
				if (!marioIsAirborne)
					unk64 &= ~0x2;
			}
		} else {
			mCurrentTarget.mPosition.y = mCurrentTarget.unk18.y;
		}
	}
}

void CPolarSubCamera::killHeightPan_()
{
	unk64 &= ~0x1;
	unk64 &= ~0x2;
	mCurrentTarget.mPosition.y = mCurrentTarget.unk18.y;

	unk24C = 0.0f;
}
