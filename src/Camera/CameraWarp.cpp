#include <Camera/Camera.hpp>
#include <Camera/CameraInbetween.hpp>
#include <Camera/CameraKindParam.hpp>
#include <Camera/CameraMarioData.hpp>
#include <Camera/cameralib.hpp>
#include <JSystem/JGeometry.hpp>
#include <MarioUtil/MathUtil.hpp>

void CPolarSubCamera::warpPosAndAt(const Vec& pos, const Vec& at)
{
	if (getCamMode() < CAMERA_MODE_REPRODUCE_DEMO) {
		mCurrentParams->copySaveParam(*mSaveKindParam[getCamMode()]);
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

// Closed in batch 131. The residue was the *inverse* of what the old TODO
// read off the diff: our getUsualLookat() return buffer sat at 0x24 against
// retail's 0x20, i.e. we had one 4-byte inline temporary too many hoisted to
// the bottom of the pool, not one too few, and the lever was therefore a -4,
// not a +4 -- which is why every accessor level tried came out 0 or +8.
// Reading `mMode` raw at this overload's three mode sites (the mode test, the
// `mSaveKindParam` index and the `isLButtonCameraSpecifyMode` argument)
// instead of through `getCamMode()` removes it and lands every displacement;
// the `(const Vec&, const Vec&)` overload, which is inlined here, keeps
// `getCamMode()` and stays byte-exact. Raw `mMode` is this class's own house
// style for exactly these expressions (Camera.hpp's `isLButtonCamera()` and
// `isLButtonCameraSpecifyMode(mMode)` sites).
// A named `int mode = getCamMode();` is the opposite lever (+8 and four extra
// instructions), and `Vec usualLookat = getUsualLookat();` costs five.
// `pos` is declared before `usualLookat` because retail's `pos` has the
// higher slot (0x38 against usualLookat's 0x2c) and named locals are laid out
// downward from the top in declaration order.
void CPolarSubCamera::warpPosAndAt(f32 ratio, s16 yAngle)
{
	if (mMode < CAMERA_MODE_REPRODUCE_DEMO) {
		mCurrentParams->copySaveParam(*mSaveKindParam[mMode]);

		Vec pos;

		JGeometry::TVec3<f32> usualLookat;
		usualLookat.set(getUsualLookat());

		if (isLButtonCameraSpecifyMode(mMode))
			mCurrentTarget.unk28 = MsClamp<f32>(ratio, 0.0f, 1.0f);
		else
			mCurrentTarget.unk28 = MsClamp<f32>(ratio, unk268, unk26C);

		mCurrentTarget.mPitch = calcAngleXFromXRotRatio_();
		mCurrentTarget.mYaw   = yAngle;

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
