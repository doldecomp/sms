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
	case 0x7:
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
		    || isLButtonCameraSpecifyMode(unk54))
			result = true;
	}
	return result;
}

bool CPolarSubCamera::isJetCoaster1stCamera() const
{
	bool result = false;
	if (mMode == 0x2E && unk2B8 != nullptr && (*((u8*)unk2B8 + 0xC) & 0x1))
		result = true;
	return result;
}

bool CPolarSubCamera::isTalkCameraSpecifyMode(int param) const
{
	bool result = false;
	switch (param) {
	case 0xA:
	case 0xC:
	case 0x2D:
	case 0x3F:
	case 0x40:
		result = true;
		break;
	}
	return result;
}

bool CPolarSubCamera::isTalkCameraInbetween() const
{
	bool result = false;
	if (isNowInbetween()) {
		if (isTalkCameraSpecifyMode(mMode) || isTalkCameraSpecifyMode(unk54))
			result = true;
	}
	return result;
}

bool CPolarSubCamera::isNormalCameraSpecifyMode(int param) const
{
	bool result = false;
	switch (param) {
	case 0x0:
	case 0x1:
	case 0x3:
	case 0x4:
	case 0x5:
	case 0x6:
	case 0x8:
	case 0xB:
	case 0xD:
	case 0xE:
	case 0xF:
	case 0x10:
	case 0x11:
	case 0x12:
	case 0x13:
	case 0x14:
	case 0x15:
	case 0x26:
	case 0x2A:
	case 0x2B:
	case 0x2C:
	case 0x2F:
	case 0x30:
	case 0x31:
	case 0x32:
	case 0x33:
	case 0x34:
	case 0x35:
	case 0x36:
	case 0x38:
	case 0x39:
	case 0x3C:
	case 0x3D:
	case 0x3E:
	case 0x42:
	case 0x43:
	case 0x44:
	case 0x45:
	case 0x46:
	case 0x47:
	case 0x48:
		result = true;
		break;
	}
	return result;
}

bool CPolarSubCamera::isNormalCameraCompletely() const
{
	bool result = false;
	if (isNormalCameraSpecifyMode(mMode)
	    && (!isNowInbetween() || isNormalCameraSpecifyMode(unk54)))
		result = true;

	return result;
}

bool CPolarSubCamera::isTowerCameraSpecifyMode(int param) const
{
	bool result = false;
	switch (param) {
	case 0x27:
	case 0x28:
	case 0x29:
	case 0x37:
	case 0x41:
		result = true;
		break;
	}
	return result;
}

bool CPolarSubCamera::isFollowCameraSpecifyMode(int param) const
{
	bool result = false;
	switch (param) {
	case 0x0:
	case 0x35:
	case 0x36:
	case 0x45:
	case 0x46:
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
	case 0x16:
	case 0x17:
	case 0x18:
	case 0x19:
	case 0x1A:
	case 0x1B:
	case 0x1C:
	case 0x1D:
	case 0x3A:
		result = true;
		break;
	}
	return result;
}

bool CPolarSubCamera::isDefiniteCameraSpecifyMode(int param) const
{
	bool result = false;
	switch (param) {
	case 0x9:
	case 0x1E:
	case 0x1F:
	case 0x20:
	case 0x21:
	case 0x22:
	case 0x23:
	case 0x24:
	case 0x25:
	case 0x3B:
		result = true;
		break;
	}
	return result;
}

bool CPolarSubCamera::isOverHipAttackSpecifyMode(int param) const
{
	bool result = false;
	if (isThing4(param)) {
		result = true;
	} else {
		switch (param) {
		case 0x8:
		case 0xB:
		case 0xD:
		case 0xE:
		case 0x11:
		case 0x14:
		case 0x26:
		case 0x27:
		case 0x28:
		case 0x29:
		case 0x2A:
		case 0x2F:
		case 0x33:
		case 0x37:
		case 0x38:
		case 0x39:
		case 0x41:
		case 0x42:
		case 0x43:
		case 0x47:
		case 0x48:
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
	case 0x0:
	case 0x1:
	case 0x4:
	case 0xB:
	case 0xE:
	case 0x14:
	case 0x26:
	case 0x27:
	case 0x28:
	case 0x29:
	case 0x2A:
	case 0x2C:
	case 0x2F:
	case 0x35:
	case 0x36:
	case 0x37:
	case 0x38:
	case 0x41:
	case 0x42:
	case 0x45:
	case 0x46:
	case 0x47:
	case 0x48:
		result = true;
		break;
	}
	return result;
}
