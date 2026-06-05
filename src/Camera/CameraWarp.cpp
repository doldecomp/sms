#include <Camera/Camera.hpp>
#include <Camera/CameraInbetween.hpp>
#include <Camera/CameraKindParam.hpp>
#include <Camera/CameraMarioData.hpp>
#include <Camera/cameralib.hpp>
#include <JSystem/JGeometry.hpp>
#include <MarioUtil/MathUtil.hpp>

void CPolarSubCamera::warpPosAndAt(const Vec& pos, const Vec& at)
{
	if (mMode < 0x49) {
		unk68->copySaveParam(*unk2D8[mMode]);
		killHeightPan_();

		mPosition.set(pos);
		mTarget.set(at);
		unk124.set(pos);
		unk148.set(at);

		unk6C->warpPosAndAt(pos, at);
		unk6C->unk4 = 0;

		calcNowTargetFromPosAndAt_(pos, at);

		unkB4 = unk80;
	}
}

void CPolarSubCamera::warpPosAndAt(f32 ratio, s16 yAngle)
{
	if (mMode < 0x49) {
		unk68->copySaveParam(*unk2D8[mMode]);

		JGeometry::TVec3<f32> usualLookat;
		usualLookat.set(getUsualLookat());

		if (isLButtonCameraSpecifyMode(mMode))
			unk80.unk28 = MsClamp<f32>(ratio, 0.0f, 1.0f);
		else
			unk80.unk28 = MsClamp<f32>(ratio, unk268, unk26C);

		unk80.unk24 = calcAngleXFromXRotRatio_();
		unk80.unk26 = yAngle;

		Vec pos;
		CLBPolarToCross(usualLookat, &pos, calcDistFromXRotRatio_(),
		                unk80.unk24, unk80.unk26);

		warpPosAndAt(pos, usualLookat);
	}
}

void CPolarSubCamera::addMoveCameraAndMario(const Vec& v)
{
	mPosition += v;
	mTarget += v;
	unk124 += v;
	unk148 += v;

	gpCameraMario->unk0 += v;

	unk6C->addMoveCameraAndMario(v);

	unk80.unk0 += v;
	unk80.unkC += v;
	unk80.unk18 += v;

	unkB4.unk0 += v;
	unkB4.unkC += v;
	unkB4.unk18 += v;
}
