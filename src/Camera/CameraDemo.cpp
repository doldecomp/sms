#include <Camera/Camera.hpp>
#include <Camera/CameraBck.hpp>
#include <Camera/CameraInbetween.hpp>
#include <Camera/CameraMapTool.hpp>
#include <Camera/CameraMarioData.hpp>
#include <Camera/camerasave.hpp>
#include <Camera/cameralib.hpp>
#include <Player/Mario.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/MarioFlags.hpp>
#include <System/MarDirector.hpp>
#include <System/MarioGamePad.hpp>
#include <JSystem/JMath.hpp>
#include <JSystem/JGeometry.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <stdio.h>

extern const char* cCameraBckNameGate;

void CPolarSubCamera::startReproduceDemoCamera_(const char*,
                                                const JGeometry::TVec3<f32>*)
{
}

void CPolarSubCamera::restartReproduceDemoCamera_() { }

void CPolarSubCamera::endReproduceDemoCamera_() { }

void CPolarSubCamera::endSimpleDemoCamera_() { }

void CPolarSubCamera::updateDemoCamera_(bool param_1)
{
	if (param_1) {
		if ((mMode == CAMERA_MODE_COUNT) ? true : false) {
			unk2B0->updateDemo(&unk124, &unk148, &mUp, &mFovy);

			if (unk2B4->unk4 != 0.0f) {
				s16 angle = CLBRoundf<s16>(182.04445f * unk2B4->unk4);

				JGeometry::TVec3<f32> origin(0.0f, 0.0f, 0.0f);
				if (unk2B4->unk0 != nullptr)
					origin = *unk2B4->unk0;

				{
					JGeometry::TVec3<f32> result = origin;
					f32 dx                       = unk124.x - origin.x;
					f32 dy                       = unk124.y - origin.y;
					f32 dz                       = unk124.z - origin.z;
					f32 sn                       = JMASSin(angle);
					f32 cs                       = JMASCos(angle);
					result.x += dx * cs + dz * sn;
					result.y += dy;
					result.z += -dx * sn + dz * cs;
					unk124 = result;
				}

				{
					JGeometry::TVec3<f32> result = origin;
					f32 dx                       = unk148.x - origin.x;
					f32 dy                       = unk148.y - origin.y;
					f32 dz                       = unk148.z - origin.z;
					f32 sn                       = JMASSin(angle);
					f32 cs                       = JMASCos(angle);
					result.x += dx * cs + dz * sn;
					result.y += dy;
					result.z += -dx * sn + dz * cs;
					unk148 = result;
				}

				f32 ux = mUp.x;
				f32 uz = mUp.z;
				mUp.x  = ux * JMASCos(angle) + uz * JMASSin(angle);
				mUp.z  = -ux * JMASSin(angle) + uz * JMASCos(angle);
			}

			calcFinalPosAndAt_();
			C_MTXPerspective(unk16C, mFovy, mAspect, mNear, mFar);
			C_MTXLookAt(unk1EC, &unk124, &mUp, &unk148);
		}
	}

	if (unk2B4->unk14 > 0) {
		unk2B4->unk14--;
		if (unk2B4->unk14 == 0) {
			unk120->onNeutralMarioKey();
			unk2B4->setThing(0);
			if (mMode == CAMERA_MODE_COUNT ? true : false)
				return;
			unk2B4->unkC |= 1;
		}
	}
}

void CPolarSubCamera::updateGateDemoCamera_()
{
	f32 fovy;
	unk2B0->updateDemo(nullptr, nullptr, nullptr, &fovy);

	int v = unk6C->unk4;
	if (unk70 != unk2B4->unk8 && v > 0)
		CLBChaseConstantSpecifyFrame<f32>(&mFovy, fovy, (f32)unk6C->unk4);
	else
		mFovy = fovy;

	C_MTXPerspective(unk16C, mFovy, mAspect, mNear, mFar);

	if (unk70 != unk2B4->unk8) {
		if ((unk2B4->unk10 - unk2B4->unk14) > 0xF0)
			changeCamModeSpecifyCamMapToolAndFrame_(unk2B4->unk8, 0x78);
	}
}

void CPolarSubCamera::startGateDemoCamera(const JDrama::TActor* actor)
{
	char buf[0x80];

	snprintf(buf, 0x80, "%s前カメラ", actor->getName());
	TCameraMapTool* tool = (TCameraMapTool*)gpCamMapToolTable->searchF(
	    JDrama::TNameRef::calcKeyCode(buf), buf);
	if (tool) {
		unk2B4->unkC |= 1;
		changeCamModeSpecifyCamMapToolAndFrame_(tool, 0x3C);
		unk64 |= 0x200;
		unk2B0->startDemo(cCameraBckNameGate, nullptr);
		unk2B4->setThing(unk2B0->getTotalDemoFrames());
	}

	snprintf(buf, 0x80, "%sカメラ", actor->getName());
	TCameraMapTool* tool2 = (TCameraMapTool*)gpCamMapToolTable->searchF(
	    JDrama::TNameRef::calcKeyCode(buf), buf);
	if (tool2)
		unk2B4->unk8 = tool2;
}

void CPolarSubCamera::startDemoCamera(const char* name,
                                      const JGeometry::TVec3<f32>* offset,
                                      long longArg, f32 f, bool boolArg)
{
	unk2B4->unk0 = offset;
	unk2B4->unk4 = f;

	if (name == nullptr)
		return;

	bool started = false;
	if (unk2B0->isFileExist(name)) {
		unk2B0->startDemo(name, offset);
		unk2B4->setThing(unk2B0->getTotalDemoFrames());
		changeCamModeSpecifyFrame_(CAMERA_MODE_COUNT, 1);
		mNear   = unk2D4->mSLReproduceDemoNearClip.get();
		started = true;
	}

	if (started)
		return;

	if ((mMode == CAMERA_MODE_COUNT) ? true : false)
		return;

	TCameraMapTool* tool = (TCameraMapTool*)gpCamMapToolTable->searchF(
	    JDrama::TNameRef::calcKeyCode(name), name);
	if (tool == nullptr)
		return;

	if (!boolArg) {
		unk2B4->setThing(-1);
		unk2B4->unkC |= 1;
	} else if (longArg == -1) {
		unk2B4->setThing(tool->unk2C);
	} else {
		unk2B4->setThing(longArg);
	}
	changeCamModeSpecifyCamMapTool_(tool);
}

void CPolarSubCamera::endDemoCamera()
{
	if ((mMode == CAMERA_MODE_COUNT) ? true : false) {
		if (mMode == CAMERA_MODE_COUNT) {
			unk2B0->endDemo();
			unk2B4->setThing(0);
			changeCamModeSpecifyFrame_(-1, 1);
			unk120->onNeutralMarioKey();
		}
	} else {
		if (isSimpleDemoCamera()) {
			if (isSimpleDemoCamera()) {
				unk2B4->setThing(0);
				unk2B4->unkC &= ~1U;
				unk120->onNeutralMarioKey();
			}
		}
	}
	unk2B4->unk0 = nullptr;
}

bool CPolarSubCamera::isSimpleDemoCamera() const
{
	bool result = false;
	if (((mMode == CAMERA_MODE_COUNT) ? true : false) == false) {
		if (unk2B4->unk14 > 0 || (unk2B4->unkC & 1U))
			result = true;
	}
	return result;
}

void CPolarSubCamera::getTotalDemoFrames() const { }

int CPolarSubCamera::getRestDemoFrames() const { return unk2B4->unk14; }

void CPolarSubCamera::ctrlNormalDeadDemo_()
{
	unk80.unkC.set(gpCameraMario->unk0);
	unk6C->execCameraInbetween(mPosition, unk80.unkC, SMS_GetMarioPos());

	CLBChaseDecrease(&mTarget, unk6C->unk24, 0.03f, 0.0f);

	if (gpMarioOriginal->checkFlag(MARIO_FLAG_HELMET_FLW_CAMERA))
		return;

	if (unk27E > 0) {
		unk27E--;
		if (unk27E > 0)
			return;
		unk280 = 0x157;
		return;
	}

	if (unk280 <= 0)
		return;

	unk280--;

	Vec diff;
	diff.x   = mTarget.x - mPosition.x;
	diff.y   = mTarget.y - mPosition.y;
	diff.z   = mTarget.z - mPosition.z;
	f32 mag2 = MsVECMag2(&diff);
	if (mag2 > 0.001f) {
		f32 r = MsClamp(10500.0f * (1.0f / mag2), 5.0f, 80.0f);
		CLBChaseConstantSpecifyFrame<f32>(&mFovy, r, (f32)unk280);
	}
}

void CPolarSubCamera::execDeadDemoProc_()
{
	if (unk27C > 0) {
		unk27C--;
		if (unk27C > 0)
			return;
		unk64 |= 0x400;
		unk78 = 1;
		if (unk64 & 0x800)
			return;
		unk27E = 1;
		return;
	}

	if (!SMS_CheckMarioFlag(MARIO_FLAG_GAME_OVER))
		return;

	if (gpMarDirector->isTalkOrDemoModeNow())
		return;
	unk27C = 0x10;
}

bool CPolarSubCamera::isHellDeadDemo() const
{
	bool result = false;
	if (unk64 & 0x400)
		if (unk64 & 0x800)
			result = true;
	return result;
}

bool CPolarSubCamera::isNormalDeadDemo() const
{
	bool result = false;
	if (unk64 & 0x400)
		if (!(unk64 & 0x800))
			result = true;
	return result;
}
