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
// TODO: the map lists this function as **weak** and its function-local static
// as `sPositionNameTable$localstatic0$calcTowerCenterPos___15CPolarSubCameraFP3Vec`
// -- the mangling MWCC uses only inside an inline function (an ordinary one
// gets `$NNN`, which is what we emit), so retail defined this body inside the
// class in <Camera/Camera.hpp>. Measured here: `inline` on the out-of-class
// definition does give weak linkage and the `$localstatic0$` name, but it also
// makes MWCC expand the body into the one call site (there is no statement
// budget for an `inline` callee at depth 1), dropping
// ctrlNormalOrTowerCamera_ from 98.1% to 78.4%. Retail's single
// `bl calcTowerCenterPos_` therefore sits at depth >= 2, i.e. the tower branch
// of ctrlNormalOrTowerCamera_ is itself an inlined private member (the name
// "NormalOrTower" suggests `ctrlTowerCamera_`). Declaring that wrapper is a
// shared-header item; until then this stays a plain out-of-line member and
// `sPositionNameTable` keeps the wrong mangling, which is the unit's whole
// remaining data gap.
void CPolarSubCamera::calcTowerCenterPos_(Vec* result)
{
	static const char* sPositionNameTable[5] = {
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

// TODO (closure batch 87): 98.1%, three separate residues, all of which point
// at one missing inlined private member -- the "Tower" half of this function's
// name, i.e. `ctrlTowerCamera_()` (or `ctrlNormalCamera_()`), declared in the
// shared <Camera/Camera.hpp>:
//   1. frame 0xb0 vs 0x70. The named region is right (`v` 12 bytes, then the
//      int->float magic pair at the top) but the low region is 100 bytes in
//      retail and 44 here, and there are 12 dead bytes between `v` and the
//      magic pair where we have 4. Inline-expansion temporaries, i.e. a callee
//      we do not expand.
//   2. `fVar2` is retail's f29 and ours f30 (f31 for fVar3 is right).
//      Swapping the two declarations swaps the *loads* instead, so it is
//      allocator ranking, not declaration order.
//   3. the `*gpMarioAngleY - 0x8000` block: retail keeps the raw value in r31
//      and sign-extends at each s16-typed use (four `extsh` plus `neg`), we
//      narrow once at the definition (one `extsh`). Measured alternatives, all
//      worse: `CLBAbs<s16>(sVar9 - unk258)` 96.8%, `int sVar9` plus an explicit
//      `(s16)` cast 97.2%, a named `s16` difference 97.2%, spelling arm 2 with
//      `sVar9` instead of re-reading gpMarioAngleY 97.1%. Retail also loads
//      unk258 before sVar9; ours is the other way round.
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
						f30 = CLBAbs(sVar9 - unk258)
						      * (2.0f / 65536.0f);
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
