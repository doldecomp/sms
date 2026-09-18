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

// Binding level over a raw member read, worth +8 of low region in
// CPolarSubCamera::calcTowerCenterPos_ (batch 127).
static inline int CameraNormalMode(const CPolarSubCamera* p)
{
	int mode = p->mMode;
	return mode;
}

// `inline` here is what the map asks for: the symbol is weak and its
// function-local static is spelled
// `sPositionNameTable$localstatic0$calcTowerCenterPos___15CPolarSubCameraFP3Vec`,
// the mangling MWCC uses only inside an inline function (an ordinary one gets
// `$NNN`). On its own `inline` also expanded the body into
// ctrlNormalOrTowerCamera_'s tower branch (98.1 -> 78.4), because an inline
// callee has no statement budget at depth 1; the in-class
// CPolarSubCamera::ctrlTowerCamera_ wrapper added in <Camera/Camera.hpp> puts
// this body at depth 2, where the budget is 9 statements and it is a `bl`
// again. Unit data 93.3 -> 100 with the weak mangling.
// TODO: every instruction is exact; the residue is 8 bytes of low-region
// frame (0x18 vs 0x20) with no candidate object left in the body.
inline void CPolarSubCamera::calcTowerCenterPos_(Vec* result)
{
	static const char* sPositionNameTable[5] = {
		"塔カメラＡ中心", "塔カメラＢ中心", "塔カメラＣ中心",
		"塔カメラＤ中心", "塔カメラＥ中心",
	};

	const char* name;
	switch (CameraNormalMode(this)) {
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

// TODO (closure batch 131): 98.1%, three residues, one of which is now closed.
//   1. CLOSED. The 56 missing low bytes (frame 0xb0 against 0x70) are a single
//      binding level over the raw `mCurrentParams` read, taken at all ten of
//      this function's parameter reads: +0x40 exactly, no instruction change,
//      and the differing rows drop from 34 to 16. So this is not the missing
//      `ctrlTowerCamera_` expansion that batch 87 suspected -- that wrapper
//      already expands correctly (there is no `bl` for it in either build and
//      the map has no symbol for it, as an in-class body that is always
//      inlined should not). Per-lever measurements at the other raw reads, for
//      the record: `unk120` +0x20, `gpMarioAngleY` +0x20, `mSaveEx` +0x18,
//      `unk288` +0x18, `unk258` +0x10, `unk64` +0x28 (and one extra
//      instruction), `mCurrentTarget.unk28` +8.
//   2. `fVar2` is retail's f29 and ours f30 (f31 for fVar3 is right).
//      Swapping the two declarations swaps the *loads* instead, so it is
//      allocator ranking, not declaration order. Unmoved by the frame fix.
//   3. the `*gpMarioAngleY - 0x8000` block: retail keeps the **raw** int in
//      r31 and narrows to s16 lazily at each use (`extsh` before the
//      `unk258` subtraction in the CLBAbs arm, three more inside CLBAbs, none
//      at all at the `CLBChaseGeneralConstantSpecifySpeed` call, and the raw
//      r31 straight into the default arm's subtraction). Ours materialises the
//      narrowed value into r31 and keeps the raw in r4, so it has one `extsh`
//      too few and one live register too many. Re-measured with the frame
//      landed: `int sVar9` with no casts is the closest (301 instructions
//      against retail's 302, 15 rows) but deduces `CLBAbs<int>` and puts the
//      one `extsh` at the call site where retail has none; spelling the
//      default arm with `sVar9` instead of re-reading the global gives 302
//      instructions and 17 rows, moving the spare `extsh` from the call into
//      that arm. `int sVar9` with `(s16)` casts (0xa8, 39 rows),
//      `CLBAbs<s16>(sVar9 - unk258)` (0xa8, 39) and dropping the variable
//      entirely (0xa8, 129) are all much worse. What is wanted is an `s16`
//      whose narrowing MWCC defers; no spelling found does that.
// The residue left is 4 bytes: `ctrlTowerCamera_`'s `Vec v` sits at 0x74 here
// against retail's 0x70, i.e. 4 bytes of pool too many below it.
static inline TCameraKindParam* CNParams(const CPolarSubCamera* p)
{
	TCameraKindParam* v = p->mCurrentParams;
	return v;
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
			ctrlTowerCamera_(fVar2);
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
				    CNParams(this)->mJumpFollowSpeedXmin,
				    CNParams(this)->mJumpFollowSpeedXmax, mCurrentTarget.unk28);
			} else {
				unk250 = CLBLinearInbetween(CNParams(this)->mFollowSpeedXmin,
				                            CNParams(this)->mFollowSpeedXmax,
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
						    CNParams(this)->mInHouseMaginfXmin,
						    CNParams(this)->mInHouseMaginfXmax,
						    mCurrentTarget.unk28);
					} else if (SMS_CheckMarioFlag(MARIO_FLAG_OCCLUDED)) {
						f29 = CLBLinearInbetween(
						    CNParams(this)->mObstructMaginfXmin,
						    CNParams(this)->mObstructMaginfXmax,
						    mCurrentTarget.unk28);
					}

					int uVar1 = unk120->mCompSPos[2];
					if (uVar1 & 0xff) {
						f29 *= CLBLinearInbetween(
						    CNParams(this)->mLFollowMaginfXmin,
						    CNParams(this)->mLFollowMaginfXmax,
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
