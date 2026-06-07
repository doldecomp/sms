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
	if (mMode < 0x49 && param_1 < 0x49) {
		TCamSaveKindParam* pTVar4 = unk2D8[mMode];
		switch (param_1) {
		case 0:
			iVar3 = pTVar4->mSLInbetFollow.get();
			break;
		case 1:
			iVar3 = pTVar4->mSLInbetParallel.get();
			break;
		case 2:
			iVar3 = pTVar4->mSLInbetMultiPlayer.get();
			break;
		case 3:
			iVar3 = pTVar4->mSLInbetWallJump.get();
			break;
		case 4:
			iVar3 = pTVar4->mSLInbetHipAttack.get();
			break;
		case 5:
			iVar3 = pTVar4->mSLInbetRocketJump.get();
			break;
		case 6:
			iVar3 = pTVar4->mSLInbetWire.get();
			break;
		case 7:
			iVar3 = pTVar4->mSLInbetLNormal.get();
			break;
		case 8:
			iVar3 = pTVar4->mSLInbetMareUnderGround.get();
			break;
		case 9:
			iVar3 = pTVar4->mSLInbetDefiniteD2.get();
			break;
		case 10:
			iVar3 = pTVar4->mSLInbetTalkE.get();
			break;
		case 0xb:
			iVar3 = pTVar4->mSLInbetLeanMirror.get();
			break;
		case 0xc:
			iVar3 = pTVar4->mSLInbetTalkA.get();
			break;
		case 0xd:
			iVar3 = pTVar4->mSLInbetUnderGround.get();
			break;
		case 0xe:
			iVar3 = pTVar4->mSLInbetIndoor.get();
			break;
		case 0xf:
			iVar3 = pTVar4->mSLInbetHang.get();
			break;
		case 0x10:
			iVar3 = pTVar4->mSLInbetWireHang.get();
			break;
		case 0x11:
			iVar3 = pTVar4->mSLInbetSandBird.get();
			break;
		case 0x12:
			iVar3 = pTVar4->mSLInbetHovering.get();
			break;
		case 0x13:
			iVar3 = pTVar4->mSLInbetJumpCode.get();
			break;
		case 0x14:
			iVar3 = pTVar4->mSLInbetDelfino.get();
			break;
		case 0x15:
			iVar3 = pTVar4->mSLInbetClimb.get();
			break;
		case 0x16:
			iVar3 = pTVar4->mSLInbetFixA.get();
			break;
		case 0x17:
			iVar3 = pTVar4->mSLInbetFixB.get();
			break;
		case 0x18:
			iVar3 = pTVar4->mSLInbetFixC.get();
			break;
		case 0x19:
			iVar3 = pTVar4->mSLInbetFixD.get();
			break;
		case 0x1a:
			iVar3 = pTVar4->mSLInbetFixE.get();
			break;
		case 0x1b:
			iVar3 = pTVar4->mSLInbetFixF.get();
			break;
		case 0x1c:
			iVar3 = pTVar4->mSLInbetFixG.get();
			break;
		case 0x1d:
			iVar3 = pTVar4->mSLInbetFixH.get();
			break;
		case 0x1e:
			iVar3 = pTVar4->mSLInbetDefiniteA.get();
			break;
		case 0x1f:
			iVar3 = pTVar4->mSLInbetDefiniteB.get();
			break;
		case 0x20:
			iVar3 = pTVar4->mSLInbetDefiniteC.get();
			break;
		case 0x21:
			iVar3 = pTVar4->mSLInbetDefiniteD.get();
			break;
		case 0x22:
			iVar3 = pTVar4->mSLInbetDefiniteE.get();
			break;
		case 0x23:
			iVar3 = pTVar4->mSLInbetDefiniteF.get();
			break;
		case 0x24:
			iVar3 = pTVar4->mSLInbetDefiniteG.get();
			break;
		case 0x25:
			iVar3 = pTVar4->mSLInbetDefiniteH.get();
			break;
		case 0x26:
			iVar3 = pTVar4->mSLInbetExMap0.get();
			break;
		case 0x27:
			iVar3 = pTVar4->mSLInbetTowerA.get();
			break;
		case 0x28:
			iVar3 = pTVar4->mSLInbetTowerB.get();
			break;
		case 0x29:
			iVar3 = pTVar4->mSLInbetTowerC.get();
			break;
		case 0x2a:
			iVar3 = pTVar4->mSLInbetSlider.get();
			break;
		case 0x2b:
			iVar3 = pTVar4->mSLInbetDiving.get();
			break;
		case 0x2c:
			iVar3 = pTVar4->mSLInbetTurbo.get();
			break;
		case 0x2d:
			iVar3 = pTVar4->mSLInbetTalkB.get();
			break;
		case 0x2e:
			iVar3 = pTVar4->mSLInbetJetCoaster.get();
			break;
		case 0x2f:
			iVar3 = pTVar4->mSLInbetParallelB.get();
			break;
		case 0x30:
			iVar3 = pTVar4->mSLInbetSurfing.get();
			break;
		case 0x31:
			iVar3 = pTVar4->mSLInbetSwimming.get();
			break;
		case 0x32:
			iVar3 = pTVar4->mSLInbetClimbJump.get();
			break;
		case 0x33:
			iVar3 = pTVar4->mSLInbetLookDown.get();
			break;
		case 0x34:
			iVar3 = pTVar4->mSLInbetRailFence.get();
			break;
		case 0x35:
			iVar3 = pTVar4->mSLInbetFollowB.get();
			break;
		case 0x36:
			iVar3 = pTVar4->mSLInbetFollowC.get();
			break;
		case 0x37:
			iVar3 = pTVar4->mSLInbetTowerD.get();
			break;
		case 0x38:
			iVar3 = pTVar4->mSLInbetDelfinoAttic.get();
			break;
		case 0x39:
			iVar3 = pTVar4->mSLInbetBossGeso.get();
			break;
		case 0x3a:
			iVar3 = pTVar4->mSLInbetFixI.get();
			break;
		case 0x3b:
			iVar3 = pTVar4->mSLInbetDefiniteI.get();
			break;
		case 0x3c:
			iVar3 = pTVar4->mSLInbetFence.get();
			break;
		case 0x3d:
			iVar3 = pTVar4->mSLInbetMonteFence.get();
			break;
		case 0x3e:
			iVar3 = pTVar4->mSLInbetMonteHang.get();
			break;
		case 0x3f:
			iVar3 = pTVar4->mSLInbetTalkC.get();
			break;
		case 0x40:
			iVar3 = pTVar4->mSLInbetTalkD.get();
			break;
		case 0x41:
			iVar3 = pTVar4->mSLInbetTowerE.get();
			break;
		case 0x42:
			iVar3 = pTVar4->mSLInbetDelfinoB.get();
			break;
		case 0x43:
			iVar3 = pTVar4->mSLInbetCancan.get();
			break;
		case 0x44:
			iVar3 = pTVar4->mSLInbetAquaticTurbo.get();
			break;
		case 0x45:
			iVar3 = pTVar4->mSLInbetFollowD.get();
			break;
		case 0x46:
			iVar3 = pTVar4->mSLInbetFollowE.get();
			break;
		case 0x47:
			iVar3 = pTVar4->mSLInbetParallelC.get();
			break;
		case 0x48:
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

	unk54 = mMode;

	if (bVar11) {
		unk60->popThing();
	} else {
		unk60->doStuff(mMode);
	}

	if (param_1 < 0x49) {
		if (mMode == 0x14 && param_1 == 0x42) {
			unk80.unk28 = 1.0f;
			unkB4.unk28 = 1.0f;
		} else if (mMode == 0x33 && param_1 == 0x3E) {
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

	if (mMode < 0x49 && unk54 < 0x49) {
		bool bVar1  = isThing4(mMode) && unk70 != nullptr;
		bool bVar11 = isThing4(unk54);
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

			switch (unk54) {
			case 9:
			case 0x17:
			case 0x19:
			case 0x1f:
			case 0x21:
				warpPosAndAt(unk80.unk28, unk80.unk26);
				break;
			case 0x1d:
			case 0x25:
				unk80.unk26 = *gpMarioAngleY - 0x8000;
				break;
			case 0x3a:
			case 0x3b:
				unk80.unk26 = *gpMarioAngleY - 0x8000;
				warpPosAndAt(unk80.unk28, unk80.unk26);
			}
		}

		if (bVar1) {
			const TCameraMapTool* tool = unk70;
			bool bVar11                = (tool->unk28 & 0x2) != 0;
			switch (mMode) {
			case 0x16:
			case 0x17:
			case 0x1E:
			case 0x1F: {
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
		switch (unk54) {
		case 0x24:
		case 0x1C:
			onMoveApproach_();
			break;
		default:
			offMoveApproach_();
			break;
		}
	}

	if (!isNormalCameraSpecifyMode(mMode) && !isTowerCameraSpecifyMode(mMode))
		unk64 &= ~0x1C;

	if (unk54 == 0x33 && mMode == 0x3E) {
		if (unk278 < 120)
			unk278 = 120;
	} else if (unk54 == 0x3E && mMode == 0x33) {
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
		changeCamMode_(7);
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
		changeCamMode_(2);
		return;
	}

	if (SMS_GetMarioStatus() == MARIO_STATUS_TOROCCO) {
		changeCamMode_(0x2E);
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
		newMode = 0x2B;
	} else if (SMS_CheckMarioFlag(2)) {
		if (currentMap == 9)
			newMode = 8;
		else
			newMode = 0xD;
	} else {
		if (gpMarioOriginal->checkFlag(MARIO_FLAG_FLUDD_EMITTING)) {
			if (currentMap != 7
			    && ((status & MARIO_STATUS_FLAG_SWIMMING)
			        || ((prevStatus & MARIO_STATUS_FLAG_SWIMMING)
			            && (status & MARIO_STATUS_FLAG_JUMPING))))
				newMode = 0x44;
			else
				newMode = 0x2C;
		} else if (currentMap != 7 && (status & MARIO_STATUS_FLAG_SWIMMING)) {
			newMode = 0x31;
		} else if (SMS_IsMarioOnWire()
		           && (status == MARIO_STATUS_WIRE_WAIT_TO_HANG
		               || status == MARIO_STATUS_WIRE_HANGING
		               || status == MARIO_STATUS_WIRE_ROLLING)) {
			newMode = 0x10;
		} else if (SMS_IsMarioOnWire() || status == MARIO_STATUS_WIRE_JUMP
		           || (prevStatus == MARIO_STATUS_WIRE_JUMP
		               && status == MARIO_STATUS_HIP_DROP)) {
			newMode = 6;
		} else if (status & MARIO_STATUS_FLAG_UNK10000) {
			newMode = 0x30;
		} else if ((status & MARIO_STATUS_FLAG_UNK20000000)
		           && gpMarioOriginal->unk2C0 != nullptr
		           && gpMarioOriginal->unk2C0->getActorType() == 0x4000006C) {
			newMode = 0x34;
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
					newMode = 0x3D;
				else
					newMode = 0x3C;
			} else if (gpCameraMario->isMarioSlider()) {
				newMode = 0x2A;
			} else if (gpCameraMario->isMarioLeanMirror()) {
				newMode = 0xB;
			} else if (gpCameraMario->isMarioIndoor()) {
				if (param_1 >= 0 && param_1 < 0x49)
					newMode = param_1;
				else if (currentMap == 7)
					newMode = 0x14;
				else
					newMode = 0xE;
			} else if (gpCameraMario->isMarioRocketing() && param_1 != 0x41) {
				if ((int)SMS_GetMarioWaterGun()->mCurrentNozzle == 4)
					newMode = 0x12;
				else
					newMode = 5;
			} else if ((status & MARIO_STATUS_FLAG_UNK200000)
			           && status != MARIO_STATUS_KICK_ROOF_ROLL_UP) {
				if (currentMap == 8)
					newMode = 0x3E;
				else
					newMode = 0xF;
			} else if (isChangeToBossGesoCamera_()) {
				newMode = 0x39;
			} else {
				bool isCancan = false;
				if (gpMarioOriginal->getHeldObject() != nullptr
				    && gpMarioOriginal->getHeldObject()->getActorType()
				           == 0x10000028)
					isCancan = true;
				if (isCancan) {
					newMode = 0x43;
				} else {
					bool onPlatform_2C9 = false;
					if (SMS_GetGroundActor(SMS_GetMarioGrPlane(), 0x400002C9))
						onPlatform_2C9 = true;
					if (onPlatform_2C9) {
						newMode = 0x11;
					} else if (currentMap != 7
					           && status == MARIO_STATUS_FORCE_JUMP) {
						newMode = 0x13;
					} else if (isChangeToParallelCameraByMoveBG_()) {
						newMode = 1;
					} else {
						bool onPlatform_12F = false;
						if (SMS_GetGroundActor(SMS_GetMarioGrPlane(),
						                       0x4000012F))
							onPlatform_12F = true;
						if (onPlatform_12F) {
							newMode = 0x47;
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
									newMode = 0x26;
								else
									newMode = 4;
							}
						} else if (param_1 >= 0 && param_1 < 0x49
						           && !isFollowCameraSpecifyMode(param_1)) {
							newMode = param_1;
						} else if (gpCameraMario->isMarioClimb(status)) {
							newMode = 0x15;
						} else {
							switch (status) {
							case MARIO_STATUS_WALL_JUMP:
							case MARIO_STATUS_WALL_SLIDE:
								if (gpCameraMario->isMarioClimb(prevStatus)
								    && (mMode == 0x15 || unk54 == 0x15)) {
									newMode = 0x32;
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
										newMode = 0x26;
									else
										newMode = 3;
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
									newMode = 0x26;
								else if (isFollowCameraSpecifyMode(param_1))
									newMode = param_1;
								else
									newMode = 0;
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
