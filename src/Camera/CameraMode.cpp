#include <Camera/Camera.hpp>

static const char* dummyMactorStringValue1 = "\0\0\0\0\0\0\0\0\0\0\0";
static const char* SMS_NO_MEMORY_MESSAGE   = "メモリが足りません\n";

const char* CPolarSubCamera::mCamKindNameSaveFile[73] = {
	"/Camera/kindFollow.prm",
	"/Camera/kindParallel.prm",
	"/Camera/kindMultiPlayer.prm",
	"/Camera/kindWallJump.prm",
	"/Camera/kindHipAttack.prm",
	"/Camera/kindRocketJump.prm",
	"/Camera/kindWire.prm",
	"/Camera/kindLNormal.prm",
	"/Camera/kindMareUnderGround.prm",
	"/Camera/kindDefiniteD2.prm",
	"/Camera/kindTalkE.prm",
	"/Camera/kindLeanMirror.prm",
	"/Camera/kindTalkA.prm",
	"/Camera/kindUnderGround.prm",
	"/Camera/kindIndoor.prm",
	"/Camera/kindHang.prm",
	"/Camera/kindWireHang.prm",
	"/Camera/kindSandBird.prm",
	"/Camera/kindHovering.prm",
	"/Camera/kindJumpCode.prm",
	"/Camera/kindDelfino.prm",
	"/Camera/kindClimb.prm",
	"/Camera/kindFixA.prm",
	"/Camera/kindFixB.prm",
	"/Camera/kindFixC.prm",
	"/Camera/kindFixD.prm",
	"/Camera/kindFixE.prm",
	"/Camera/kindFixF.prm",
	"/Camera/kindFixG.prm",
	"/Camera/kindFixH.prm",
	"/Camera/kindDefiniteA.prm",
	"/Camera/kindDefiniteB.prm",
	"/Camera/kindDefiniteC.prm",
	"/Camera/kindDefiniteD.prm",
	"/Camera/kindDefiniteE.prm",
	"/Camera/kindDefiniteF.prm",
	"/Camera/kindDefiniteG.prm",
	"/Camera/kindDefiniteH.prm",
	"/Camera/kindExMap0.prm",
	"/Camera/kindTowerA.prm",
	"/Camera/kindTowerB.prm",
	"/Camera/kindTowerC.prm",
	"/Camera/kindSlider.prm",
	"/Camera/kindDiving.prm",
	"/Camera/kindTurbo.prm",
	"/Camera/kindTalkB.prm",
	"/Camera/kindJetCoaster.prm",
	"/Camera/kindParallelB.prm",
	"/Camera/kindSurfing.prm",
	"/Camera/kindSwimming.prm",
	"/Camera/kindClimbJump.prm",
	"/Camera/kindLookDown.prm",
	"/Camera/kindRailFence.prm",
	"/Camera/kindFollowB.prm",
	"/Camera/kindFollowC.prm",
	"/Camera/kindTowerD.prm",
	"/Camera/kindDelfinoAttic.prm",
	"/Camera/kindBossGeso.prm",
	"/Camera/kindFixI.prm",
	"/Camera/kindDefiniteI.prm",
	"/Camera/kindFence.prm",
	"/Camera/kindMonteFence.prm",
	"/Camera/kindMonteHang.prm",
	"/Camera/kindTalkC.prm",
	"/Camera/kindTalkD.prm",
	"/Camera/kindTowerE.prm",
	"/Camera/kindDelfinoB.prm",
	"/Camera/kindCancan.prm",
	"/Camera/kindAquaticTurbo.prm",
	"/Camera/kindFollowD.prm",
	"/Camera/kindFollowE.prm",
	"/Camera/kindParallelC.prm",
	"/Camera/kindParallelD.prm",
};

bool CPolarSubCamera::isLButtonCameraSpecifyMode(int mode) const
{
	bool result = false;
	switch (mode) {
	case CAMERA_MODE_L_NORMAL:
		result = true;
		break;
	}
	return result;
}

bool CPolarSubCamera::isLButtonCameraInbetween() const
{
	bool result = false;
	if (isNowInbetween()) {
		if (isLButtonCameraSpecifyMode(mMode)
		    || isLButtonCameraSpecifyMode(mPrevMode))
			result = true;
	}
	return result;
}

bool CPolarSubCamera::isJetCoaster1stCamera() const
{
	bool result = false;
	if (mMode == CAMERA_MODE_JET_COASTER && unk2B8 != nullptr
	    && (*((u8*)unk2B8 + 0xC) & 0x1))
		result = true;
	return result;
}

bool CPolarSubCamera::isTalkCameraSpecifyMode(int param) const
{
	bool result = false;
	switch (param) {
	case CAMERA_MODE_TALK_E:
	case CAMERA_MODE_TALK_A:
	case CAMERA_MODE_TALK_B:
	case CAMERA_MODE_TALK_C:
	case CAMERA_MODE_TALK_D:
		result = true;
		break;
	}
	return result;
}

bool CPolarSubCamera::isTalkCameraInbetween() const
{
	bool result = false;
	if (isNowInbetween()) {
		if (isTalkCameraSpecifyMode(mMode)
		    || isTalkCameraSpecifyMode(mPrevMode))
			result = true;
	}
	return result;
}

bool CPolarSubCamera::isNormalCameraSpecifyMode(int param) const
{
	bool result = false;
	switch (param) {
	case CAMERA_MODE_FOLLOW:
	case CAMERA_MODE_PARALLEL:
	case CAMERA_MODE_WALL_JUMP:
	case CAMERA_MODE_HIP_ATTACK:
	case CAMERA_MODE_ROCKET_JUMP:
	case CAMERA_MODE_WIRE:
	case CAMERA_MODE_MARE_UNDER_GROUND:
	case CAMERA_MODE_LEAN_MIRROR:
	case CAMERA_MODE_UNDER_GROUND:
	case CAMERA_MODE_INDOOR:
	case CAMERA_MODE_HANG:
	case CAMERA_MODE_WIRE_HANG:
	case CAMERA_MODE_SAND_BIRD:
	case CAMERA_MODE_HOVERING:
	case CAMERA_MODE_JUMP_CODE:
	case CAMERA_MODE_DELFINO:
	case CAMERA_MODE_CLIMB:
	case CAMERA_MODE_EX_MAP_0:
	case CAMERA_MODE_SLIDER:
	case CAMERA_MODE_DIVING:
	case CAMERA_MODE_TURBO:
	case CAMERA_MODE_PARALLEL_B:
	case CAMERA_MODE_SURFING:
	case CAMERA_MODE_SWIMMING:
	case CAMERA_MODE_CLIMB_JUMP:
	case CAMERA_MODE_LOOK_DOWN:
	case CAMERA_MODE_RAIL_FENCE:
	case CAMERA_MODE_FOLLOW_B:
	case CAMERA_MODE_FOLLOW_C:
	case CAMERA_MODE_DELFINO_ATTIC:
	case CAMERA_MODE_BOSS_GESO:
	case CAMERA_MODE_FENCE:
	case CAMERA_MODE_MONTE_FENCE:
	case CAMERA_MODE_MONTE_HANG:
	case CAMERA_MODE_DELFINO_B:
	case CAMERA_MODE_CANCAN:
	case CAMERA_MODE_AQUATIC_TURBO:
	case CAMERA_MODE_FOLLOW_D:
	case CAMERA_MODE_FOLLOW_E:
	case CAMERA_MODE_PARALLEL_C:
	case CAMERA_MODE_PARALLEL_D:
		result = true;
		break;
	}
	return result;
}

bool CPolarSubCamera::isNormalCameraCompletely() const
{
	bool result = false;
	if (isNormalCameraSpecifyMode(mMode)
	    && (!isNowInbetween() || isNormalCameraSpecifyMode(mPrevMode)))
		result = true;

	return result;
}

bool CPolarSubCamera::isTowerCameraSpecifyMode(int param) const
{
	bool result = false;
	switch (param) {
	case CAMERA_MODE_TOWER_A:
	case CAMERA_MODE_TOWER_B:
	case CAMERA_MODE_TOWER_C:
	case CAMERA_MODE_TOWER_D:
	case CAMERA_MODE_TOWER_E:
		result = true;
		break;
	}
	return result;
}

bool CPolarSubCamera::isFollowCameraSpecifyMode(int param) const
{
	bool result = false;
	switch (param) {
	case CAMERA_MODE_FOLLOW:
	case CAMERA_MODE_FOLLOW_B:
	case CAMERA_MODE_FOLLOW_C:
	case CAMERA_MODE_FOLLOW_D:
	case CAMERA_MODE_FOLLOW_E:
		result = true;
		break;
	}
	return result;
}

bool CPolarSubCamera::isRailCameraSpecifyMode(int) const { return false; }

bool CPolarSubCamera::isFixCameraSpecifyMode(int param) const
{
	bool result = false;
	switch (param) {
	case CAMERA_MODE_FIX_A:
	case CAMERA_MODE_FIX_B:
	case CAMERA_MODE_FIX_C:
	case CAMERA_MODE_FIX_D:
	case CAMERA_MODE_FIX_E:
	case CAMERA_MODE_FIX_F:
	case CAMERA_MODE_FIX_G:
	case CAMERA_MODE_FIX_H:
	case CAMERA_MODE_FIX_I:
		result = true;
		break;
	}
	return result;
}

bool CPolarSubCamera::isDefiniteCameraSpecifyMode(int param) const
{
	bool result = false;
	switch (param) {
	case CAMERA_MODE_DEFINITE_D2:
	case CAMERA_MODE_DEFINITE_A:
	case CAMERA_MODE_DEFINITE_B:
	case CAMERA_MODE_DEFINITE_C:
	case CAMERA_MODE_DEFINITE_D:
	case CAMERA_MODE_DEFINITE_E:
	case CAMERA_MODE_DEFINITE_F:
	case CAMERA_MODE_DEFINITE_G:
	case CAMERA_MODE_DEFINITE_H:
	case CAMERA_MODE_DEFINITE_I:
		result = true;
		break;
	}
	return result;
}

bool CPolarSubCamera::isOverHipAttackSpecifyMode(int param) const
{
	bool result = false;
	if (isFixOrDefiniteCameraSpecifyMode(param)) {
		result = true;
	} else {
		switch (param) {
		case CAMERA_MODE_MARE_UNDER_GROUND:
		case CAMERA_MODE_LEAN_MIRROR:
		case CAMERA_MODE_UNDER_GROUND:
		case CAMERA_MODE_INDOOR:
		case CAMERA_MODE_SAND_BIRD:
		case CAMERA_MODE_DELFINO:
		case CAMERA_MODE_EX_MAP_0:
		case CAMERA_MODE_TOWER_A:
		case CAMERA_MODE_TOWER_B:
		case CAMERA_MODE_TOWER_C:
		case CAMERA_MODE_SLIDER:
		case CAMERA_MODE_PARALLEL_B:
		case CAMERA_MODE_LOOK_DOWN:
		case CAMERA_MODE_TOWER_D:
		case CAMERA_MODE_DELFINO_ATTIC:
		case CAMERA_MODE_BOSS_GESO:
		case CAMERA_MODE_TOWER_E:
		case CAMERA_MODE_DELFINO_B:
		case CAMERA_MODE_CANCAN:
		case CAMERA_MODE_PARALLEL_C:
		case CAMERA_MODE_PARALLEL_D:
			result = true;
			break;
		}
	}
	return result;
}

bool CPolarSubCamera::isSlopeCameraMode() const
{
	bool result = false;
	switch (mMode) {
	case CAMERA_MODE_FOLLOW:
	case CAMERA_MODE_PARALLEL:
	case CAMERA_MODE_HIP_ATTACK:
	case CAMERA_MODE_LEAN_MIRROR:
	case CAMERA_MODE_INDOOR:
	case CAMERA_MODE_DELFINO:
	case CAMERA_MODE_EX_MAP_0:
	case CAMERA_MODE_TOWER_A:
	case CAMERA_MODE_TOWER_B:
	case CAMERA_MODE_TOWER_C:
	case CAMERA_MODE_SLIDER:
	case CAMERA_MODE_TURBO:
	case CAMERA_MODE_PARALLEL_B:
	case CAMERA_MODE_FOLLOW_B:
	case CAMERA_MODE_FOLLOW_C:
	case CAMERA_MODE_TOWER_D:
	case CAMERA_MODE_DELFINO_ATTIC:
	case CAMERA_MODE_TOWER_E:
	case CAMERA_MODE_DELFINO_B:
	case CAMERA_MODE_FOLLOW_D:
	case CAMERA_MODE_FOLLOW_E:
	case CAMERA_MODE_PARALLEL_C:
	case CAMERA_MODE_PARALLEL_D:
		result = true;
		break;
	}
	return result;
}
