#include <Camera/Camera.hpp>
#include <Camera/CameraInbetween.hpp>
#include <Camera/CameraKindParam.hpp>
#include <Camera/CameraMarioData.hpp>
#include <Camera/cameralib.hpp>
#include <JSystem/JGeometry.hpp>
#include <MarioUtil/MathUtil.hpp>

void CPolarSubCamera::warpPosAndAt(const Vec& pos, const Vec& at)
{
	if (mMode < CAMERA_MODE_REPRODUCE_DEMO) {
		mCurrentParams->copySaveParam(*mSaveKindParam[mMode]);
		killHeightPan_();

		mPosition.set(pos);
		mTarget.set(at);
		unk124.set(pos);
		unk148.set(at);

		mInbetween->warpPosAndAt(pos, at);
		mInbetween->mFramesRemaining = 0;

		calcNowTargetFromPosAndAt_(pos, at);

		mPreviousTarget = mCurrentTarget;
	}
}

void CPolarSubCamera::warpPosAndAt(f32 ratio, s16 yAngle)
{
	if (mMode < CAMERA_MODE_REPRODUCE_DEMO) {
		mCurrentParams->copySaveParam(*mSaveKindParam[mMode]);

		JGeometry::TVec3<f32> usualLookat;
		usualLookat.set(getUsualLookat());

		if (isLButtonCameraSpecifyMode(mMode))
			mCurrentTarget.unk28 = MsClamp<f32>(ratio, 0.0f, 1.0f);
		else
			mCurrentTarget.unk28 = MsClamp<f32>(ratio, unk268, unk26C);

		mCurrentTarget.mPitch = calcAngleXFromXRotRatio_();
		mCurrentTarget.mYaw   = yAngle;

		Vec pos;
		CLBPolarToCross(usualLookat, &pos, calcDistFromXRotRatio_(),
		                mCurrentTarget.mPitch, mCurrentTarget.mYaw);

		warpPosAndAt(pos, usualLookat);
	}
}

void CPolarSubCamera::addMoveCameraAndMario(const Vec& v)
{
	mPosition += v;
	mTarget += v;
	unk124 += v;
	unk148 += v;

	gpCameraMario->addMoveCameraAndMario(v);

	mInbetween->addMoveCameraAndMario(v);

	mCurrentTarget.mPosition += v;
	mCurrentTarget.mTarget += v;
	mCurrentTarget.unk18 += v;

	mPreviousTarget.mPosition += v;
	mPreviousTarget.mTarget += v;
	mPreviousTarget.unk18 += v;
}
