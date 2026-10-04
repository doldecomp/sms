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

bool CPolarSubCamera::startReproduceDemoCamera_(
    const char* name, const JGeometry::TVec3<f32>* offset)
{
	bool started = false;
	if (unk2B0->isFileExist(name)) {
		unk2B0->startDemo(name, offset);
		mCameraDemo->setLengthFrames(unk2B0->getTotalDemoFrames());
		changeCamModeSpecifyFrame_(CAMERA_MODE_REPRODUCE_DEMO, 1);
		mNear   = mSaveEx->mSLReproduceDemoNearClip.get();
		started = true;
	}
	return started;
}

void CPolarSubCamera::restartReproduceDemoCamera_() { }

void CPolarSubCamera::endReproduceDemoCamera_()
{
	if (mMode == CAMERA_MODE_REPRODUCE_DEMO) {
		unk2B0->endDemo();
		mCameraDemo->setLengthFrames(0);
		changeCamModeSpecifyFrame_(-1, 1);
		unk120->onNeutralMarioKey();
	}
}

void CPolarSubCamera::endSimpleDemoCamera_()
{
	if (isSimpleDemoCamera()) {
		mCameraDemo->setLengthFrames(0);
		mCameraDemo->unkC &= ~1U;
		unk120->onNeutralMarioKey();
	}
}

void CPolarSubCamera::updateDemoCamera_(bool param_1)
{
	if (param_1) {
		if (isBckDemoCamera()) {
			unk2B0->updateDemo(&unk124, &unk148, &mUp, &mFovy);

			if (mCameraDemo->unk4 != 0.0f) {
				s16 angle = CLBDegToShortAngle(mCameraDemo->unk4);

				JGeometry::TVec3<f32> origin(0.0f, 0.0f, 0.0f);
				if (mCameraDemo->unk0 != nullptr)
					origin = *mCameraDemo->unk0;

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

	if (mCameraDemo->mRemainingFrames > 0) {
		mCameraDemo->mRemainingFrames -= 1;
		if (mCameraDemo->mRemainingFrames == 0) {
			unk120->onNeutralMarioKey();
			mCameraDemo->setLengthFrames(0);
			if (isBckDemoCamera())
				return;
			mCameraDemo->unkC |= 1;
		}
	}
}

void CPolarSubCamera::updateGateDemoCamera_()
{
	f32 fovy;
	unk2B0->updateDemo(nullptr, nullptr, nullptr, &fovy);

	int v = mInbetween->getUnk4();
	if (unk70 != mCameraDemo->unk8 && v > 0)
		CLBChaseConstantSpecifyFrame<f32>(&mFovy, fovy, (f32)v);
	else
		mFovy = fovy;

	C_MTXPerspective(unk16C, mFovy, mAspect, mNear, mFar);

	if (unk70 != mCameraDemo->unk8)
		if (mCameraDemo->mTotalFrames - mCameraDemo->mRemainingFrames > 240)
			changeCamModeSpecifyCamMapToolAndFrame_(mCameraDemo->unk8, 120);
}

void CPolarSubCamera::startGateDemoCamera(const JDrama::TActor* actor)
{
	char buf[0x80];

	snprintf(buf, 0x80, "%s前カメラ", actor->getName());
	TCameraMapTool* tool = (TCameraMapTool*)gpCamMapToolTable->searchF(
	    JDrama::TNameRef::calcKeyCode(buf), buf);
	if (tool) {
		mCameraDemo->unkC |= 1;
		changeCamModeSpecifyCamMapToolAndFrame_(tool, 0x3C);
		unk64 |= CAMERA_FLAG_GATE_DEMO;
		unk2B0->startDemo(cCameraBckNameGate, nullptr);
		mCameraDemo->setLengthFrames(unk2B0->getTotalDemoFrames());
	}

	snprintf(buf, 0x80, "%sカメラ", actor->getName());
	TCameraMapTool* tool2 = (TCameraMapTool*)gpCamMapToolTable->searchF(
	    JDrama::TNameRef::calcKeyCode(buf), buf);
	if (tool2)
		mCameraDemo->unk8 = tool2;
}

void CPolarSubCamera::startDemoCamera(const char* name,
                                      const JGeometry::TVec3<f32>* offset,
                                      s32 length_frames, f32 f, bool boolArg)
{
	mCameraDemo->unk0 = offset;
	mCameraDemo->unk4 = f;

	if (name == nullptr)
		return;

	if (startReproduceDemoCamera_(name, offset))
		return;

	if (isBckDemoCamera())
		return;

	TCameraMapTool* tool = (TCameraMapTool*)gpCamMapToolTable->searchF(
	    JDrama::TNameRef::calcKeyCode(name), name);
	if (tool != nullptr) {
		if (!boolArg) {
			mCameraDemo->setLengthFrames(-1);
			mCameraDemo->unkC |= 1;
		} else if (length_frames == -1) {
			mCameraDemo->setLengthFrames(tool->getDemoLengthFrames());
		} else {
			mCameraDemo->setLengthFrames(length_frames);
		}
		changeCamModeSpecifyCamMapTool_(tool);
	}
}

void CPolarSubCamera::endDemoCamera()
{
	if (isBckDemoCamera())
		endReproduceDemoCamera_();
	else if (isSimpleDemoCamera())
		endSimpleDemoCamera_();

	mCameraDemo->unk0 = nullptr;
}

bool CPolarSubCamera::isSimpleDemoCamera() const
{
	bool result = false;
	if (isBckDemoCamera() == false)
		if (mCameraDemo->mRemainingFrames > 0 || (mCameraDemo->unkC & 1U))
			result = true;

	return result;
}

int CPolarSubCamera::getTotalDemoFrames() const
{
	return mCameraDemo->mTotalFrames;
}

int CPolarSubCamera::getRestDemoFrames() const
{
	return mCameraDemo->mRemainingFrames;
}

void CPolarSubCamera::ctrlNormalDeadDemo_()
{
	mCurrentTarget.mTarget.set(gpCameraMario->unk0);
	mInbetween->execCameraInbetween(mPosition, mCurrentTarget.mTarget,
	                                SMS_GetMarioPos());

	CLBChaseDecrease(&mTarget, mInbetween->mAt, 0.03f, 0.0f);

	if (gpMarioOriginal->checkFlag(MARIO_FLAG_HELMET_FLW_CAMERA))
		return;

	if (mDeadDemoCountdownToFovZoom > 0) {
		mDeadDemoCountdownToFovZoom -= 1;
		if (mDeadDemoCountdownToFovZoom > 0)
			return;
		mDeadDemoFovZoomTimer = 343;
		return;
	}

	if (mDeadDemoFovZoomTimer > 0) {
		mDeadDemoFovZoomTimer -= 1;

		Vec diff;
		diff.x           = mTarget.x - mPosition.x;
		diff.y           = mTarget.y - mPosition.y;
		diff.z           = mTarget.z - mPosition.z;
		f32 distToTarget = MsVECMag2(&diff);
		if (distToTarget > 0.001f) {
			f32 r = MsClamp(10500.0f * (1.0f / distToTarget), 5.0f, 80.0f);
			CLBChaseConstantSpecifyFrame<f32>(&mFovy, r,
			                                  (f32)mDeadDemoFovZoomTimer);
		}
	}
}

void CPolarSubCamera::execDeadDemoProc_()
{
	if (mDeadDemoCountdown > 0) {
		mDeadDemoCountdown -= 1;
		if (mDeadDemoCountdown > 0)
			return;
		unk64 |= CAMERA_FLAG_DEAD_DEMO;
		mPosFreezeFrames = 1;
		if (unk64 & CAMERA_FLAG_HELL_DEAD_DEMO)
			return;
		mDeadDemoCountdownToFovZoom = 1;
		return;
	}

	if (SMS_CheckMarioFlag(MARIO_FLAG_GAME_OVER)
	    && !gpMarDirector->isTalkOrDemoModeNow())
		mDeadDemoCountdown = 16;
}

bool CPolarSubCamera::isHellDeadDemo() const
{
	bool result = false;
	if (unk64 & CAMERA_FLAG_DEAD_DEMO)
		if (unk64 & CAMERA_FLAG_HELL_DEAD_DEMO)
			result = true;
	return result;
}

bool CPolarSubCamera::isNormalDeadDemo() const
{
	bool result = false;
	if (unk64 & CAMERA_FLAG_DEAD_DEMO)
		if (!(unk64 & CAMERA_FLAG_HELL_DEAD_DEMO))
			result = true;
	return result;
}
