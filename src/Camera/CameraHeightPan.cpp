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
		fVar10 = unk68->mTargetAtJumpOffsetY;

	CLBChaseGeneralConstantSpecifySpeed(&unk24C, fVar10,
	                                    unk68->mAtJumpOffsetSpeed);
	unk80.unkC.y += unk24C;
	f32 fVar1 = unk80.unk18.y - unkB4.unk18.y;

	if (fabricatedInline()) {
		unk64 |= 0x1;
		unk64 &= ~0x2;
		unk80.unk0.y = unkB4.unk0.y;
		CLBChaseDecrease(&unk80.unk0.y, unk80.unk18.y,
		                 unk68->mHeightPanChaseRateY, 0.0f);
		if (unk80.unk28 != unkB4.unk28)
			unk80.unk0.y += fVar1;
	} else {
		if (unk64 & 0x3) {
			if (unk80.unk28 != unkB4.unk28)
				unk80.unk0.y += fVar1;

			if (unk64 & 0x1) {
				unk64 &= ~0x1;

				bool bVar7 = false;
				switch (mMode) {
				case CAMERA_MODE_JUMP_CODE:
				case CAMERA_MODE_ROCKET_JUMP:
					if (unk256 < unk2D4->mPanWarpAngleX.get())
						bVar7 = true;
					break;
				}

				if (bVar7) {
					mPosition.y = unk80.unk0.y = unk80.unk18.y;
					unk64 &= ~0x2;
				} else {
					unk64 |= 0x2;
				}
			}

			if (unk80.unk0.y != unk80.unk18.y) {
				BOOL pTVar5 = CLBChaseSpecialDecrease(
				    &unk80.unk0.y, unk80.unk18.y, unk2D4->mPanAfterMagnif.get(),
				    unk2D4->mPanAfterMinHeight.get());
				if (!pTVar5 && !marioIsAirborne)
					unk64 &= ~0x2;

			} else {
				if (!marioIsAirborne)
					unk64 &= ~0x2;
			}
		} else {
			unk80.unk0.y = unk80.unk18.y;
		}
	}
}

void CPolarSubCamera::killHeightPan_()
{
	unk64 &= ~0x1;
	unk64 &= ~0x2;
	unk80.unk0.y = unk80.unk18.y;
	unk24C       = 0.0f;
}
