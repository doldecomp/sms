#include <Camera/CameraInbetween.hpp>
#include <Camera/cameralib.hpp>

TCameraInbetween::TCameraInbetween()
    : unk0(1)
    , unk4(0)
    , unk18(0.0f, 0.0f, 0.0f)
    , unk24(0.0f, 0.0f, 0.0f)
    , unk30(0.0f, 0.0f, 0.0f)
    , unk3C(0)
    , unk40(0)
    , unk44(0.0f)
{
}

void TCameraInbetween::calcPolarData_()
{
	CLBCrossToPolar(unk24, unk18, &unk8, &unkC, &unkE);
	CLBCrossToPolar(unk30, unk24, &unk10, &unk14, &unk16);
}

void TCameraInbetween::warpPosAndAt(const Vec& param_1, const Vec& param_2)
{
	unk18.set(param_1);
	unk24.set(param_2);
	calcPolarData_();
}

void TCameraInbetween::addMoveCameraAndMario(const Vec& param_1)
{
	unk18 += param_1;
	unk24 += param_1;
	unk30 += param_1;
	calcPolarData_();
}

void TCameraInbetween::startCameraInbetween(int param_1)
{
	unk4  = param_1;
	unk0  = param_1;
	unk3C = 0;
	calcPolarData_();
}

void TCameraInbetween::initCameraInbetween(const JGeometry::TVec3<f32>& param_1,
                                           const JGeometry::TVec3<f32>& param_2,
                                           const JGeometry::TVec3<f32>& param_3)
{
	unk18.set(param_1);
	unk24.set(param_2);
	unk30.set(param_3);
	calcPolarData_();
}

static void fakeInline(s16* param_1, f32 param_2, s16 param_3)
{
	if (param_2 < 0.001f) {
		*param_1 = param_3;
	} else {
		s16 thing = param_3 - *param_1;
		*param_1 += CLBRoundf<s16>(1.0f / param_2 * thing);
	}
}

void TCameraInbetween::execCameraInbetween(const JGeometry::TVec3<f32>& param_1,
                                           const JGeometry::TVec3<f32>& param_2,
                                           const JGeometry::TVec3<f32>& param_3)
{
	unk18.set(param_1);
	unk24.set(param_2);

	if (unk4 > 0) {
		f32 dVar5 = unk4;

		if (isThing())
			CLBChaseConstantSpecifyFrame(&unk44, 0.0f, dVar5);
		f32 radius;
		s16 vAngle;
		s16 hAngle;
		CLBCrossToPolar(param_3, param_2, &radius, &vAngle, &hAngle);
		CLBChaseConstantSpecifyFrame(&unk10, radius, dVar5);

		fakeInline(&unk14, dVar5, vAngle);
		fakeInline(&unk16, dVar5, hAngle);

		if (CLBAbs(param_3.x - param_2.x) > 0.1f
		    || CLBAbs(param_3.z - param_2.z) > 0.1f) {
			JGeometry::TVec3<f32> tmp;
			CLBPolarToCross(param_3, &tmp, unk10, unk14, unk16);
			unk24.x = tmp.x;
			unk24.z = tmp.z;
		}

		CLBCrossToPolar(param_3, param_2, &radius, &vAngle, &hAngle);
		CLBChaseConstantSpecifyFrame(&unk8, radius, dVar5);
		fakeInline(&unkC, dVar5, vAngle);
		if (unk3C == 0) {
			fakeInline(&unkE, dVar5, hAngle);
		} else {
			unkE += unk40;
		}

		CLBPolarToCross(unk24, &unk18, unk8, unkC, unkE);
		unk4 -= 1;
		if (unk4 == 0)
			unk3C = 0;
	}

	unk30.set(param_3);
}

void TCameraInbetween::execInbetweenAndCalcPosAndAt(
    const JGeometry::TVec3<f32>& param_1, const JGeometry::TVec3<f32>& param_2,
    const JGeometry::TVec3<f32>& param_3, f32 param_4, f32 param_5, f32 param_6,
    f32 param_7, JGeometry::TVec3<f32>* param_8, JGeometry::TVec3<f32>* param_9)
{
}

void TCameraInbetween::setInbetModePosAngleY(
    TCameraInbetween::EnumAngleInbetMode, s16, s16)
{
}
