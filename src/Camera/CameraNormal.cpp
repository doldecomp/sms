#include <Camera/Camera.hpp>
#include <Strategic/LiveActor.hpp>
#include <System/MarioGamePad.hpp>
#include <System/PositionHolder.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <Player/Mario.hpp>
#include <Player/MarioAccess.hpp>
#include <Enemy/FireWanwan.hpp>
#include <Camera/camerasave.hpp>
#include <Camera/cameralib.hpp>
#include <Camera/CameraKindParam.hpp>
#include <Camera/CameraMarioData.hpp>

static const char* dummyMactorStringValue1 = "\0\0\0\0\0\0\0\0\0\0\0";
static const char* SMS_NO_MEMORY_MESSAGE   = "メモリが足りません\n";

static const char* MtxCalcTypeName[] = {
	"MActorMtxCalcType_Basic クラシックスケールＯＮ",
	"MActorMtxCalcType_Softimage クラシックスケールＯＦＦ",
	"MActorMtxCalcType_MotionBlend モーションブレンド",
	"MActorMtxCalcType_User ユーザー定義",
};

// TODO: this should be an inline that doesn't get inlined,
// but it actually gets inlined :(
void CPolarSubCamera::calcTowerCenterPos_(Vec* result)
{
	static const char* sPositionNameTable[6] = {
		"塔カメラＡ中心", "塔カメラＢ中心", "塔カメラＣ中心",
		"塔カメラＤ中心", "塔カメラＥ中心",
	};

	const char* name;
	switch (mMode) {
	case CAMERA_MODE_TOWER_A:
		name = sPositionNameTable[0];
		break;
	case CAMERA_MODE_TOWER_B:
		name = sPositionNameTable[1];
		break;
	case CAMERA_MODE_TOWER_C:
		name = sPositionNameTable[2];
		break;
	case CAMERA_MODE_TOWER_D:
		name = sPositionNameTable[3];
		break;
	case CAMERA_MODE_TOWER_E:
		name = sPositionNameTable[4];
		break;
	default:
		result->x = 0.0f;
		result->y = 0.0f;
		result->z = 0.0f;
		return;
	}

	TStagePositionInfo* info = (TStagePositionInfo*)gpPositionHolder->searchF(
	    JDrama::TNameRef::calcKeyCode(name), name);
	if (info != nullptr) {
		*result = info->unkC;
	} else {
		result->x = 0.0f;
		result->y = 0.0f;
		result->z = 0.0f;
	}
}

void CPolarSubCamera::ctrlNormalOrTowerCamera_()
{
	f32 fVar2 = unk120->mCompSPos[6];
	f32 fVar3 = unk120->mCompSPos[7];

	unk250 = 0.0f;

	if (mTargetFreezeFrames == 0)
		mCurrentTarget.mTarget.set(gpCameraMario->unk0);

	if (mPosFreezeFrames == 0) {
		if (unk64 & CAMERA_FLAG_UNK4) {
			if (!CLBChaseAngleDecrease(&mCurrentTarget.mYaw, unk274, unk276)) {
				unk64 &= ~CAMERA_FLAG_UNK4;
				if (unk64 & CAMERA_FLAG_UNK8) {
					unk64 &= ~CAMERA_FLAG_UNK8;
					unk64 |= CAMERA_FLAG_UNK10;
				}
			}
		} else if (isTowerCameraSpecifyMode(mMode)) {
			if (fVar2 != 0.0f) {
				rotateY_ByStickX_(fVar2);
				execInvalidAutoChase_();
				unk64 |= CAMERA_FLAG_UNK80;
			} else {
				if (!(unk64 & CAMERA_FLAG_UNK80) && !isMarioCrabWalk_()) {
					Vec v;
					calcTowerCenterPos_(&v);
					calcNoticeTargetYrot_(v);
				}
			}
		} else if (fVar2 != 0.0f) {
			rotateY_ByStickX_(fVar2);
			execInvalidAutoChase_();
		} else if (mMode == CAMERA_MODE_BOSS_GESO) {
			if (isChangeToBossGesoCamera_())
				calcNoticeTargetYrot_(unk2A8->mPosition);
		} else if (mMode == CAMERA_MODE_CANCAN) {
			if (isChangeToCancanCamera_())
				calcNoticeTargetYrot_(
				    ((TFireWanwanTailHit*)gpMarioOriginal->getHeldObject())
				        ->getHostPos());
		} else {
			if (!SMS_IsMarioTouchGround4cm()) {
				unk250 = CLBLinearInbetween(
				    mCurrentParams->mJumpFollowSpeedXmin,
				    mCurrentParams->mJumpFollowSpeedXmax, mCurrentTarget.unk28);
			} else {
				unk250 = CLBLinearInbetween(mCurrentParams->mFollowSpeedXmin,
				                            mCurrentParams->mFollowSpeedXmax,
				                            mCurrentTarget.unk28);
			}

			if (isMomentDefinite_()) {
				mCurrentTarget.mYaw = matan(
				    mPreviousTarget.mPosition.z - mCurrentTarget.mTarget.z,
				    mPreviousTarget.mPosition.x - mCurrentTarget.mTarget.x);
			} else if (!(unk64 & CAMERA_FLAG_UNK80) && !isMarioCrabWalk_()) {
				if (isMarioAimWithGun_() && !isChangeToParallelCameraByMoveBG_()
				    && !isChangeToParallelCameraCByMoveBG_()) {
					if (unk288 != 0.0f) {
						f32 fVar15 = CLBEaseInInbetween(0.0f, 1.0f,
						                                mCurrentTarget.unk28);
						s16 uVar9  = CLBEaseInInbetween(
                            mSaveEx->mSLAimAngleYChaseMin.get(),
                            mSaveEx->mSLAimAngleYChaseMax.get(), fVar15);
						CLBChaseAngleDecrease(&mCurrentTarget.mYaw,
						                      *gpMarioAngleY - 0x8000, uVar9);
					}
				} else {
					f32 f29;
					f32 f30;

					s16 sVar9 = *gpMarioAngleY - 0x8000;
					switch (mMode) {
					case CAMERA_MODE_DIVING:
					case CAMERA_MODE_HOVERING:
						f30 = CLBAbs(sVar9 - unk258) * (2.0f / 65536.0f);
						break;
					default:
						f30 = (1.0f
						       - JMASCos((*gpMarioAngleY - 0x8000 - unk258)
						                 * 2))
						      * 0.5f;
					} // 0,000030517578

					f29 = 1.0f;
					if (unk2CA != -1) {
						f29 = CLBLinearInbetween(
						    mCurrentParams->mInHouseMaginfXmin,
						    mCurrentParams->mInHouseMaginfXmax,
						    mCurrentTarget.unk28);
					} else if (SMS_CheckMarioFlag(MARIO_FLAG_OCCLUDED)) {
						f29 = CLBLinearInbetween(
						    mCurrentParams->mObstructMaginfXmin,
						    mCurrentParams->mObstructMaginfXmax,
						    mCurrentTarget.unk28);
					}

					int uVar1 = unk120->mCompSPos[2];
					if (uVar1 & 0xff) {
						f29 *= CLBLinearInbetween(
						    mCurrentParams->mLFollowMaginfXmin,
						    mCurrentParams->mLFollowMaginfXmax,
						    mCurrentTarget.unk28);
					}
					f32 fVar4;
					switch (mMode) {
					case CAMERA_MODE_DIVING:
					case CAMERA_MODE_HOVERING:
						fVar4 = 100.0f;
						break;
					default:
						fVar4 = gpCameraMario->mFrameMoveDistHorizontal;
						break;
					}
					f32 kek = unk250 * f30 * f29 * fVar4 * unk288;
					if (kek > 32766.998f)
						kek = 32766.998f;
					CLBChaseGeneralConstantSpecifySpeed<s16>(
					    &mCurrentTarget.mYaw, sVar9, CLBRoundf<s16>(kek));
				}
			}
		}

		rotateX_ByStickY_(fVar3);
	}

	calcPosAndAt_();
}
