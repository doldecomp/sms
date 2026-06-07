#include <Camera/Camera.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/MapUtil.hpp>
#include <Enemy/BossGesso.hpp>
#include <Map/MapData.hpp>
#include <Player/Mario.hpp>
#include <Player/MarioAccess.hpp>
#include <Camera/CameraInbetween.hpp>
#include <Camera/CameraMapTool.hpp>
#include <Camera/camerasave.hpp>
#include <Camera/CameraKindParam.hpp>
#include <System/StageUtil.hpp>
#include <System/MarDirector.hpp>
#include <Camera/CameraMarioData.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

void CPolarSubCamera::getLButtonCameraModeByNozzle_() { }

s16 CPolarSubCamera::getCameraInbetweenFrame_(int param_1)
{
	if (param_1 == -1)
		param_1 = unk60->getThing();

	int iVar3 = 1;
	if (mMode < CAMERA_MODE_COUNT && param_1 < CAMERA_MODE_COUNT) {
		TCamSaveKindParam* pTVar4 = unk2D8[mMode];
		switch (param_1) {
		case CAMERA_MODE_FOLLOW:
			iVar3 = pTVar4->mSLInbetFollow.get();
			break;
		case CAMERA_MODE_PARALLEL:
			iVar3 = pTVar4->mSLInbetParallel.get();
			break;
		case CAMERA_MODE_MULTI_PLAYER:
			iVar3 = pTVar4->mSLInbetMultiPlayer.get();
			break;
		case CAMERA_MODE_WALL_JUMP:
			iVar3 = pTVar4->mSLInbetWallJump.get();
			break;
		case CAMERA_MODE_HIP_ATTACK:
			iVar3 = pTVar4->mSLInbetHipAttack.get();
			break;
		case CAMERA_MODE_ROCKET_JUMP:
			iVar3 = pTVar4->mSLInbetRocketJump.get();
			break;
		case CAMERA_MODE_WIRE:
			iVar3 = pTVar4->mSLInbetWire.get();
			break;
		case CAMERA_MODE_L_NORMAL:
			iVar3 = pTVar4->mSLInbetLNormal.get();
			break;
		case CAMERA_MODE_MARE_UNDER_GROUND:
			iVar3 = pTVar4->mSLInbetMareUnderGround.get();
			break;
		case CAMERA_MODE_DEFINITE_D2:
			iVar3 = pTVar4->mSLInbetDefiniteD2.get();
			break;
		case CAMERA_MODE_TALK_E:
			iVar3 = pTVar4->mSLInbetTalkE.get();
			break;
		case CAMERA_MODE_LEAN_MIRROR:
			iVar3 = pTVar4->mSLInbetLeanMirror.get();
			break;
		case CAMERA_MODE_TALK_A:
			iVar3 = pTVar4->mSLInbetTalkA.get();
			break;
		case CAMERA_MODE_UNDER_GROUND:
			iVar3 = pTVar4->mSLInbetUnderGround.get();
			break;
		case CAMERA_MODE_INDOOR:
			iVar3 = pTVar4->mSLInbetIndoor.get();
			break;
		case CAMERA_MODE_HANG:
			iVar3 = pTVar4->mSLInbetHang.get();
			break;
		case CAMERA_MODE_WIRE_HANG:
			iVar3 = pTVar4->mSLInbetWireHang.get();
			break;
		case CAMERA_MODE_SAND_BIRD:
			iVar3 = pTVar4->mSLInbetSandBird.get();
			break;
		case CAMERA_MODE_HOVERING:
			iVar3 = pTVar4->mSLInbetHovering.get();
			break;
		case CAMERA_MODE_JUMP_CODE:
			iVar3 = pTVar4->mSLInbetJumpCode.get();
			break;
		case CAMERA_MODE_DELFINO:
			iVar3 = pTVar4->mSLInbetDelfino.get();
			break;
		case CAMERA_MODE_CLIMB:
			iVar3 = pTVar4->mSLInbetClimb.get();
			break;
		case CAMERA_MODE_FIX_A:
			iVar3 = pTVar4->mSLInbetFixA.get();
			break;
		case CAMERA_MODE_FIX_B:
			iVar3 = pTVar4->mSLInbetFixB.get();
			break;
		case CAMERA_MODE_FIX_C:
			iVar3 = pTVar4->mSLInbetFixC.get();
			break;
		case CAMERA_MODE_FIX_D:
			iVar3 = pTVar4->mSLInbetFixD.get();
			break;
		case CAMERA_MODE_FIX_E:
			iVar3 = pTVar4->mSLInbetFixE.get();
			break;
		case CAMERA_MODE_FIX_F:
			iVar3 = pTVar4->mSLInbetFixF.get();
			break;
		case CAMERA_MODE_FIX_G:
			iVar3 = pTVar4->mSLInbetFixG.get();
			break;
		case CAMERA_MODE_FIX_H:
			iVar3 = pTVar4->mSLInbetFixH.get();
			break;
		case CAMERA_MODE_DEFINITE_A:
			iVar3 = pTVar4->mSLInbetDefiniteA.get();
			break;
		case CAMERA_MODE_DEFINITE_B:
			iVar3 = pTVar4->mSLInbetDefiniteB.get();
			break;
		case CAMERA_MODE_DEFINITE_C:
			iVar3 = pTVar4->mSLInbetDefiniteC.get();
			break;
		case CAMERA_MODE_DEFINITE_D:
			iVar3 = pTVar4->mSLInbetDefiniteD.get();
			break;
		case CAMERA_MODE_DEFINITE_E:
			iVar3 = pTVar4->mSLInbetDefiniteE.get();
			break;
		case CAMERA_MODE_DEFINITE_F:
			iVar3 = pTVar4->mSLInbetDefiniteF.get();
			break;
		case CAMERA_MODE_DEFINITE_G:
			iVar3 = pTVar4->mSLInbetDefiniteG.get();
			break;
		case CAMERA_MODE_DEFINITE_H:
			iVar3 = pTVar4->mSLInbetDefiniteH.get();
			break;
		case CAMERA_MODE_EX_MAP_0:
			iVar3 = pTVar4->mSLInbetExMap0.get();
			break;
		case CAMERA_MODE_TOWER_A:
			iVar3 = pTVar4->mSLInbetTowerA.get();
			break;
		case CAMERA_MODE_TOWER_B:
			iVar3 = pTVar4->mSLInbetTowerB.get();
			break;
		case CAMERA_MODE_TOWER_C:
			iVar3 = pTVar4->mSLInbetTowerC.get();
			break;
		case CAMERA_MODE_SLIDER:
			iVar3 = pTVar4->mSLInbetSlider.get();
			break;
		case CAMERA_MODE_DIVING:
			iVar3 = pTVar4->mSLInbetDiving.get();
			break;
		case CAMERA_MODE_TURBO:
			iVar3 = pTVar4->mSLInbetTurbo.get();
			break;
		case CAMERA_MODE_TALK_B:
			iVar3 = pTVar4->mSLInbetTalkB.get();
			break;
		case CAMERA_MODE_JET_COASTER:
			iVar3 = pTVar4->mSLInbetJetCoaster.get();
			break;
		case CAMERA_MODE_PARALLEL_B:
			iVar3 = pTVar4->mSLInbetParallelB.get();
			break;
		case CAMERA_MODE_SURFING:
			iVar3 = pTVar4->mSLInbetSurfing.get();
			break;
		case CAMERA_MODE_SWIMMING:
			iVar3 = pTVar4->mSLInbetSwimming.get();
			break;
		case CAMERA_MODE_CLIMB_JUMP:
			iVar3 = pTVar4->mSLInbetClimbJump.get();
			break;
		case CAMERA_MODE_LOOK_DOWN:
			iVar3 = pTVar4->mSLInbetLookDown.get();
			break;
		case CAMERA_MODE_RAIL_FENCE:
			iVar3 = pTVar4->mSLInbetRailFence.get();
			break;
		case CAMERA_MODE_FOLLOW_B:
			iVar3 = pTVar4->mSLInbetFollowB.get();
			break;
		case CAMERA_MODE_FOLLOW_C:
			iVar3 = pTVar4->mSLInbetFollowC.get();
			break;
		case CAMERA_MODE_TOWER_D:
			iVar3 = pTVar4->mSLInbetTowerD.get();
			break;
		case CAMERA_MODE_DELFINO_ATTIC:
			iVar3 = pTVar4->mSLInbetDelfinoAttic.get();
			break;
		case CAMERA_MODE_BOSS_GESO:
			iVar3 = pTVar4->mSLInbetBossGeso.get();
			break;
		case CAMERA_MODE_FIX_I:
			iVar3 = pTVar4->mSLInbetFixI.get();
			break;
		case CAMERA_MODE_DEFINITE_I:
			iVar3 = pTVar4->mSLInbetDefiniteI.get();
			break;
		case CAMERA_MODE_FENCE:
			iVar3 = pTVar4->mSLInbetFence.get();
			break;
		case CAMERA_MODE_MONTE_FENCE:
			iVar3 = pTVar4->mSLInbetMonteFence.get();
			break;
		case CAMERA_MODE_MONTE_HANG:
			iVar3 = pTVar4->mSLInbetMonteHang.get();
			break;
		case CAMERA_MODE_TALK_C:
			iVar3 = pTVar4->mSLInbetTalkC.get();
			break;
		case CAMERA_MODE_TALK_D:
			iVar3 = pTVar4->mSLInbetTalkD.get();
			break;
		case CAMERA_MODE_TOWER_E:
			iVar3 = pTVar4->mSLInbetTowerE.get();
			break;
		case CAMERA_MODE_DELFINO_B:
			iVar3 = pTVar4->mSLInbetDelfinoB.get();
			break;
		case CAMERA_MODE_CANCAN:
			iVar3 = pTVar4->mSLInbetCancan.get();
			break;
		case CAMERA_MODE_AQUATIC_TURBO:
			iVar3 = pTVar4->mSLInbetAquaticTurbo.get();
			break;
		case CAMERA_MODE_FOLLOW_D:
			iVar3 = pTVar4->mSLInbetFollowD.get();
			break;
		case CAMERA_MODE_FOLLOW_E:
			iVar3 = pTVar4->mSLInbetFollowE.get();
			break;
		case CAMERA_MODE_PARALLEL_C:
			iVar3 = pTVar4->mSLInbetParallelC.get();
			break;
		case CAMERA_MODE_PARALLEL_D:
			iVar3 = pTVar4->mSLInbetParallelD.get();
			break;
		}
	}
	return iVar3;
}

void CPolarSubCamera::setUpToLButtonCamera_(int param_1)
{
	unk80.unk30 = unk80.unk28;
	TCameraKindParam param;
	param.copySaveParam(*unk2D8[param_1]);
	unkB4.unk28 = unk80.unk28
	    = MsClamp(CLBCalcRatio<s16>(param.mXAngleMin, param.mXAngleMax,
	                                -param.mOffsetAngleX),
	              0.0f, 1.0f);
}

void CPolarSubCamera::setUpFromLButtonCamera_()
{
	unkB4.unk28 = unk80.unk28 = unk80.unk30;
}

void CPolarSubCamera::changeCamModeSub_(int param_1, int param_2, bool param_3)
{
	bool bVar11 = false;
	if (param_1 == -1) {
		param_1 = unk60->getThing();
		bVar11  = true;
	}

	if (!param_3 && mMode == param_1)
		return;

	if (param_2 < 0)
		return;

	if (param_2 == 0)
		param_2 = 1;

	mPrevMode = mMode;

	if (bVar11) {
		unk60->popThing();
	} else {
		unk60->doStuff(mMode);
	}

	if (param_1 < CAMERA_MODE_COUNT) {
		if (mMode == CAMERA_MODE_DELFINO && param_1 == CAMERA_MODE_DELFINO_B) {
			unk80.unk28 = 1.0f;
			unkB4.unk28 = 1.0f;
		} else if (mMode == CAMERA_MODE_LOOK_DOWN
		           && param_1 == CAMERA_MODE_MONTE_HANG) {
			unk80.unk28 = 1.0f;
			unkB4.unk28 = 1.0f;
		} else {
			if (!isLButtonCameraSpecifyMode(mMode)) {
				if (isLButtonCameraSpecifyMode(param_1)) {
					setUpToLButtonCamera_(param_1);
					unk120->onNeutralMarioKey();
				}
			} else {
				if (!isLButtonCameraSpecifyMode(param_1)) {
					setUpFromLButtonCamera_();
					unk120->onNeutralMarioKey();
				}
			}
		}
		unk6C->startCameraInbetween(param_2);
	}

	mMode = param_1;

	if (mMode < CAMERA_MODE_COUNT && mPrevMode < CAMERA_MODE_COUNT) {
		bool bVar1  = isThing4(mMode) && unk70 != nullptr;
		bool bVar11 = isThing4(mPrevMode);
		if (!bVar11 && bVar1 == true) {
			unkE8  = unk80;
			unk11C = unk70->unk28;
		}
		if (bVar11) {
			if (unk11C & 1) {
				unkB4 = unk80 = unkE8;
				killHeightPan_();
			} else {
				calcNowTargetFromPosAndAt_(mPosition, mTarget);
			}

			switch (mPrevMode) {
			case CAMERA_MODE_DEFINITE_D2:
			case CAMERA_MODE_FIX_B:
			case CAMERA_MODE_FIX_D:
			case CAMERA_MODE_DEFINITE_B:
			case CAMERA_MODE_DEFINITE_D:
				warpPosAndAt(unk80.unk28, unk80.unk26);
				break;
			case CAMERA_MODE_FIX_H:
			case CAMERA_MODE_DEFINITE_H:
				unk80.unk26 = *gpMarioAngleY - 0x8000;
				break;
			case CAMERA_MODE_FIX_I:
			case CAMERA_MODE_DEFINITE_I:
				unk80.unk26 = *gpMarioAngleY - 0x8000;
				warpPosAndAt(unk80.unk28, unk80.unk26);
			}
		}

		if (bVar1) {
			const TCameraMapTool* tool = unk70;
			bool bVar11                = (tool->unk28 & 0x2) != 0;
			switch (mMode) {
			case CAMERA_MODE_FIX_A:
			case CAMERA_MODE_FIX_B:
			case CAMERA_MODE_DEFINITE_A:
			case CAMERA_MODE_DEFINITE_B: {
				JGeometry::TVec3<f32> save;
				if (bVar11)
					save = unk80.unk0;
				tool->calcPosAndAt(&unk80.unk0, &unk80.unkC);
				if (bVar11)
					unk80.unk0.set(save);
			} break;

			default: {
				JGeometry::TVec3<f32> save;
				if (bVar11)
					save = mPosition;
				tool->calcPosAndAt(&mPosition, &mTarget);
				if (bVar11)
					mPosition.set(save);
				warpPosAndAt(mPosition, mTarget);
			} break;
			}
		}

		unk78 = 0;
		unk7C = 0;
		switch (mPrevMode) {
		case CAMERA_MODE_DEFINITE_G:
		case CAMERA_MODE_FIX_G:
			onMoveApproach_();
			break;
		default:
			offMoveApproach_();
			break;
		}
	}

	if (!isNormalCameraSpecifyMode(mMode) && !isTowerCameraSpecifyMode(mMode))
		unk64 &= ~0x1C;

	if (mPrevMode == CAMERA_MODE_LOOK_DOWN && mMode == CAMERA_MODE_MONTE_HANG) {
		if (unk278 < 120)
			unk278 = 120;
	} else if (mPrevMode == CAMERA_MODE_MONTE_HANG
	           && mMode == CAMERA_MODE_LOOK_DOWN) {
		if (unk27A < 120)
			unk27A = 120;
	}

	killHeightPanWhenChangeCamMode_();
	unk2AC->reset();
}

void CPolarSubCamera::changeCamModeSpecifyFrame_(int param_1, int param_2)
{
	unk74 = unk70;
	unk70 = nullptr;
	changeCamModeSub_(param_1, param_2, false);
}

void CPolarSubCamera::changeCamModeSpecifyCamMapTool_(
    const TCameraMapTool* param_1)
{
	int newMode = param_1->getCameraMode();
	if (mMode != newMode || unk70 != param_1) {
		unk74 = unk70;
		unk70 = param_1;
		changeCamModeSub_(newMode, getCameraInbetweenFrame_(newMode), true);
	}
}

void CPolarSubCamera::changeCamModeSpecifyCamMapToolAndFrame_(
    const TCameraMapTool* param_1, int param_2)
{
	int newMode = param_1->getCameraMode();
	if (mMode != newMode || unk70 != param_1) {
		unk74 = unk70;
		unk70 = param_1;
		changeCamModeSub_(newMode, param_2, true);
	}
}

void CPolarSubCamera::execFrontRotate_()
{
	(void)0;
	(void)0;
	(void)0;
	(void)0;
	if (!isLButtonCameraSpecifyMode(mMode)
	    && SMS_GetMarioStatus() != MARIO_STATUS_HIP_DROP) {
		unk64 &= ~0x10;
		unk64 |= 0x4;
		unk274 = *gpMarioAngleY - 0x8000;
		if (unk120->checkFrameMeaning(0x4000)) {
			unk276 = unk2D4->mYButtonRotateChase.get();
			unk64 |= 0x8;
		} else if (unk120->checkFrameMeaning(0x8000)) {
			unk276 = unk2D4->mLButtonRotateChase.get();
			unk64 &= ~0x8;
			SMSGetMSound()->startSoundSystemSE(0x4826, 0, nullptr, 0);
		}
	}
}

void CPolarSubCamera::doLButtonCameraOn_()
{
	if (!isLButtonCameraSpecifyMode(mMode) && !isLButtonCameraInbetween()) {
		unk282 = 60;
		SMSGetMSound()->startSoundSystemSE(0x4824, 0, nullptr, 0);
		changeCamMode_(CAMERA_MODE_L_NORMAL);
	}
}

#pragma dont_inline on
void CPolarSubCamera::doLButtonCameraOff_(bool param_1)
{
	if (isThing2()) {
		unk282 = 60;
		if (param_1) {
			changeCamModeSpecifyFrame_(-1, 1);
		} else {
			SMSGetMSound()->startSoundSystemSE(0x4825, 0, nullptr, 0);
			changeCamMode_(-1);
		}

		if (unk64 & 0x20)
			execNoticeOnOffProc_(NOTICE_MODE_UNK0);
	}
}

bool CPolarSubCamera::isChangeToBossGesoCamera_() const
{
	bool result     = false;
	THitActor* held = gpMarioOriginal->getHeldObject();
	if (held != nullptr
	    && (held->getActorType() == 0x8000006
	        || held->getActorType() == 0x8000008)) {
		TBossGesso* gesso = static_cast<TBossGesso*>(unk2A8);
		if (gesso != nullptr && (gesso->beakHeld() || gesso->tentacleHeld()))
			result = true;
	}
	return result;
}
#pragma dont_inline off

bool CPolarSubCamera::isChangeToCancanCamera_() const
{
	bool result = false;

	if (gpMarioOriginal->getHeldObject() != nullptr
	    && gpMarioOriginal->getHeldObject()->getActorType() == 0x10000028)
		result = true;

	return result;
}

bool CPolarSubCamera::isChangeToParallelCameraByMoveBG_() const
{
	bool iVar5        = false;
	THitActor* holder = gpMarioOriginal->getHolder();
	if (holder != nullptr) {
		switch (holder->getActorType()) {
		case 0x400000BB:
		case 0x40000049:
			iVar5 = true;
			break;
		}
	}
	if (!iVar5 && SMS_GetMarioGrPlane()) {
		const TLiveActor* grActor = SMS_GetMarioGrPlane()->getActor();
		if (grActor != nullptr) {
			switch (grActor->getActorType()) {
			case 0x4000012E:
				iVar5 = true;
				break;
			case 0x400000A5:
			case 0x4000009C:
			case 0x40000249:
			case 0x4000022E:
				if (SMS_IsMarioTouchGround4cm())
					iVar5 = true;
				break;
			}
		}
	}

	return iVar5;
}

bool CPolarSubCamera::isChangeToParallelCameraCByMoveBG_() const
{
	bool result = false;

	const TLiveActor* grActor
	    = SMS_GetGroundActor(SMS_GetMarioGrPlane(), 0x4000012F);
	if (grActor != nullptr)
		result = true;

	return result;
}

// TODO: inlining is NOT working out in a bunch of places in this function,
// hence the hacks above...
void CPolarSubCamera::execCameraModeChangeProc_(int param_1)
{
	if (SMS_isMultiPlayerMap()) {
		changeCamMode_(CAMERA_MODE_MULTI_PLAYER);
		return;
	}

	if (SMS_GetMarioStatus() == MARIO_STATUS_TOROCCO) {
		changeCamMode_(CAMERA_MODE_JET_COASTER);
		return;
	}

	if (isThing4(param_1))
		return;

	if (unk64 & 0x20)
		execNoticeOnOffProc_(NOTICE_MODE_UNK0);

	int prevMode = mMode;

	if (gpMarioOriginal->isSleeping() || SMS_CheckMarioFlag(2)
	    || (SMS_GetMarioStatus() & MARIO_STATUS_FLAG_UNK10000)
	    || gpCameraMario->isMarioRocketing()
	    || gpMarioOriginal->checkFlag(MARIO_FLAG_FLUDD_EMITTING)
	    || gpCameraMario->isMarioClimb(SMS_GetMarioStatus())) {
		if (isLButtonCameraSpecifyMode(mMode))
			doLButtonCameraOff_(true);
		if (unk120->checkFrameMeaning(0x8000))
			execFrontRotate_();
	} else {
		if (isLButtonCameraSpecifyMode(mMode)) {
			if (SMS_GetMarioStatus() & MARIO_STATUS_FLAG_UNK20000) {
				doLButtonCameraOff_(true);
			} else if (!isLButtonCameraInbetween()
			           && unk120->checkFrameMeaning(0x14000) && unk282 == 0) {
				doLButtonCameraOff_(false);
			}
		} else if (isNormalCameraSpecifyMode(mMode)
		           || isTowerCameraSpecifyMode(mMode)) {
			if (unk64 & 0x10) {
				unk64 &= ~0x10;
				doLButtonCameraOn_();
			} else if (unk120->checkFrameMeaning(0xC000)) {
				bool doCheck = true;
				if (unk120->checkFrameMeaning(0x4000)) {
					if (unk282 != 0)
						doCheck = false;
					else
						execNoticeOnOffProc_((EnumNoticeOnOffMode)2);
				}
				if (doCheck) {
					if (unk64 & 0x20) {
						doLButtonCameraOn_();
					} else if (!isLButtonCameraInbetween()) {
						execFrontRotate_();
					}
				}
			}
		}
	}

	if (prevMode != mMode)
		return;
	if (isLButtonCameraSpecifyMode(mMode))
		return;

	u32 status     = SMS_GetMarioStatus();
	u32 prevStatus = gpMarioOriginal->getPreviousStatus();
	int currentMap = gpMarDirector->getCurrentMap();

	int newMode;
	if (gpMarioOriginal->checkFlag(MARIO_FLAG_HELMET_FLW_CAMERA)) {
		newMode = CAMERA_MODE_DIVING;
	} else if (SMS_CheckMarioFlag(2)) {
		if (currentMap == 9)
			newMode = CAMERA_MODE_MARE_UNDER_GROUND;
		else
			newMode = CAMERA_MODE_UNDER_GROUND;
	} else {
		if (gpMarioOriginal->checkFlag(MARIO_FLAG_FLUDD_EMITTING)) {
			if (currentMap != 7
			    && ((status & MARIO_STATUS_FLAG_SWIMMING)
			        || ((prevStatus & MARIO_STATUS_FLAG_SWIMMING)
			            && (status & MARIO_STATUS_FLAG_JUMPING))))
				newMode = CAMERA_MODE_AQUATIC_TURBO;
			else
				newMode = CAMERA_MODE_TURBO;
		} else if (currentMap != 7 && (status & MARIO_STATUS_FLAG_SWIMMING)) {
			newMode = CAMERA_MODE_SWIMMING;
		} else if (SMS_IsMarioOnWire()
		           && (status == MARIO_STATUS_WIRE_WAIT_TO_HANG
		               || status == MARIO_STATUS_WIRE_HANGING
		               || status == MARIO_STATUS_WIRE_ROLLING)) {
			newMode = CAMERA_MODE_WIRE_HANG;
		} else if (SMS_IsMarioOnWire() || status == MARIO_STATUS_WIRE_JUMP
		           || (prevStatus == MARIO_STATUS_WIRE_JUMP
		               && status == MARIO_STATUS_HIP_DROP)) {
			newMode = CAMERA_MODE_WIRE;
		} else if (status & MARIO_STATUS_FLAG_UNK10000) {
			newMode = CAMERA_MODE_SURFING;
		} else if ((status & MARIO_STATUS_FLAG_UNK20000000)
		           && gpMarioOriginal->unk2C0 != nullptr
		           && gpMarioOriginal->unk2C0->getActorType() == 0x4000006C) {
			newMode = CAMERA_MODE_RAIL_FENCE;
		} else {
			bool isFenceish = false;
			switch (SMS_GetMarioStatus()) {
			case 0x30000569:
			case MARIO_STATUS_FENCE_MOVE:
			case MARIO_STATUS_FENCE_PUNCH:
			case MARIO_STATUS_FENCE_CATCH:
			case MARIO_STATUS_FENCE_JUMP_CATCH:
				isFenceish = true;
				break;
			}
			if (isFenceish) {
				if (currentMap == 8)
					newMode = CAMERA_MODE_MONTE_FENCE;
				else
					newMode = CAMERA_MODE_FENCE;
			} else if (gpCameraMario->isMarioSlider()) {
				newMode = CAMERA_MODE_SLIDER;
			} else if (gpCameraMario->isMarioLeanMirror()) {
				newMode = CAMERA_MODE_LEAN_MIRROR;
			} else if (gpCameraMario->isMarioIndoor()) {
				if (param_1 >= 0 && param_1 < CAMERA_MODE_COUNT)
					newMode = param_1;
				else if (currentMap == 7)
					newMode = CAMERA_MODE_DELFINO;
				else
					newMode = CAMERA_MODE_INDOOR;
			} else if (gpCameraMario->isMarioRocketing()
			           && param_1 != CAMERA_MODE_TOWER_E) {
				if ((int)SMS_GetMarioWaterGun()->mCurrentNozzle == 4)
					newMode = CAMERA_MODE_HOVERING;
				else
					newMode = CAMERA_MODE_ROCKET_JUMP;
			} else if ((status & MARIO_STATUS_FLAG_UNK200000)
			           && status != MARIO_STATUS_KICK_ROOF_ROLL_UP) {
				if (currentMap == 8)
					newMode = CAMERA_MODE_MONTE_HANG;
				else
					newMode = CAMERA_MODE_HANG;
			} else if (isChangeToBossGesoCamera_()) {
				newMode = CAMERA_MODE_BOSS_GESO;
			} else {
				bool isCancan = false;
				if (gpMarioOriginal->getHeldObject() != nullptr
				    && gpMarioOriginal->getHeldObject()->getActorType()
				           == 0x10000028)
					isCancan = true;
				if (isCancan) {
					newMode = CAMERA_MODE_CANCAN;
				} else {
					bool onPlatform_2C9 = false;
					if (SMS_GetGroundActor(SMS_GetMarioGrPlane(), 0x400002C9))
						onPlatform_2C9 = true;
					if (onPlatform_2C9) {
						newMode = CAMERA_MODE_SAND_BIRD;
					} else if (currentMap != 7
					           && status == MARIO_STATUS_FORCE_JUMP) {
						newMode = CAMERA_MODE_JUMP_CODE;
					} else if (isChangeToParallelCameraByMoveBG_()) {
						newMode = CAMERA_MODE_PARALLEL;
					} else {
						bool onPlatform_12F = false;
						if (SMS_GetGroundActor(SMS_GetMarioGrPlane(),
						                       0x4000012F))
							onPlatform_12F = true;
						if (onPlatform_12F) {
							newMode = CAMERA_MODE_PARALLEL_C;
						} else if (status == MARIO_STATUS_HIP_DROP) {
							if (isOverHipAttackSpecifyMode(param_1)) {
								newMode = param_1;
							} else {
								bool exMap = false;
								if (SMS_isExMap()) {
									switch (gpMarDirector->getCurrentMap()) {
									case 0x1D:
									case 0x1E:
										break;
									default:
										exMap = true;
										break;
									}
								}
								if (exMap)
									newMode = CAMERA_MODE_EX_MAP_0;
								else
									newMode = CAMERA_MODE_HIP_ATTACK;
							}
						} else if (param_1 >= 0 && param_1 < CAMERA_MODE_COUNT
						           && !isFollowCameraSpecifyMode(param_1)) {
							newMode = param_1;
						} else if (gpCameraMario->isMarioClimb(status)) {
							newMode = CAMERA_MODE_CLIMB;
						} else {
							switch (status) {
							case MARIO_STATUS_WALL_JUMP:
							case MARIO_STATUS_WALL_SLIDE:
								if (gpCameraMario->isMarioClimb(prevStatus)
								    && (mMode == CAMERA_MODE_CLIMB
								        || mPrevMode == CAMERA_MODE_CLIMB)) {
									newMode = CAMERA_MODE_CLIMB_JUMP;
								} else {
									bool exMap = false;
									if (SMS_isExMap()) {
										switch (
										    gpMarDirector->getCurrentMap()) {
										case 0x1D:
										case 0x1E:
											break;
										default:
											exMap = true;
											break;
										}
									}
									if (exMap)
										newMode = CAMERA_MODE_EX_MAP_0;
									else
										newMode = CAMERA_MODE_WALL_JUMP;
								}
								break;

							default:
								bool exMap = false;
								if (SMS_isExMap()) {
									switch (gpMarDirector->getCurrentMap()) {
									case 0x1D:
									case 0x1E:
										break;
									default:
										exMap = true;
										break;
									}
								}
								if (exMap)
									newMode = CAMERA_MODE_EX_MAP_0;
								else if (isFollowCameraSpecifyMode(param_1))
									newMode = param_1;
								else
									newMode = CAMERA_MODE_FOLLOW;
								break;
							}
						}
					}
				}
			}
		}
	}

	changeCamMode_(newMode);
}
