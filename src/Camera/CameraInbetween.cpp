#include <Camera/CameraInbetween.hpp>
#include <Camera/cameralib.hpp>

// TODO: move to cameralib & replace CLBAbs with this
#define ABS(x) ((x) >= 0 ? (x) : -(x))

TCameraInbetween::TCameraInbetween()
    : unk0(1)
    , mFramesRemaining(0)
    , mPos(0.0f, 0.0f, 0.0f)
    , mAt(0.0f, 0.0f, 0.0f)
    , unk30(0.0f, 0.0f, 0.0f)
    , unk3C(0)
    , unk40(0)
    , unk44(0.0f)
{
}

void TCameraInbetween::calcPolarData_()
{
	CLBCrossToPolar(mAt, mPos, &unk8, &unkC, &unkE);
	CLBCrossToPolar(unk30, mAt, &unk10, &unk14, &unk16);
}

void TCameraInbetween::warpPosAndAt(const Vec& pos, const Vec& at)
{
	mPos.set(pos);
	mAt.set(at);
	calcPolarData_();
}

void TCameraInbetween::addMoveCameraAndMario(const Vec& offset)
{
	mPos += offset;
	mAt += offset;
	unk30 += offset;
	calcPolarData_();
}

void TCameraInbetween::startCameraInbetween(int frames)
{
	mFramesRemaining = frames;
	unk0             = frames;
	unk3C            = 0;
	calcPolarData_();
}

void TCameraInbetween::initCameraInbetween(const JGeometry::TVec3<f32>& pos,
                                           const JGeometry::TVec3<f32>& at,
                                           const JGeometry::TVec3<f32>& param_3)
{
	mPos.set(pos);
	mAt.set(at);
	unk30.set(param_3);
	calcPolarData_();
}

void TCameraInbetween::execCameraInbetween(const JGeometry::TVec3<f32>& pos,
                                           const JGeometry::TVec3<f32>& at,
                                           const JGeometry::TVec3<f32>& param_3)
{
	mPos.set(pos);
	mAt.set(at);

	if (mFramesRemaining > 0) {
		f32 remFrames = mFramesRemaining;

		if (isThing())
			CLBChaseConstantSpecifyFrame(&unk44, 0.0f, remFrames);
		f32 radius;
		s16 pitch;
		s16 yaw;
		CLBCrossToPolar(param_3, at, &radius, &pitch, &yaw);

		CLBChaseConstantSpecifyFrame(&unk10, radius, remFrames);
		CLBChaseConstantSpecifyFrame(&unk14, pitch, remFrames);
		CLBChaseConstantSpecifyFrame(&unk16, yaw, remFrames);

		if (ABS(param_3.x - at.x) > 0.1f || ABS(param_3.z - at.z) > 0.1f) {
			JGeometry::TVec3<f32> tmp;
			CLBPolarToCross(param_3, &tmp, unk10, unk14, unk16);
			mAt.x = tmp.x;
			mAt.z = tmp.z;
		}

		CLBCrossToPolar(mAt, pos, &radius, &pitch, &yaw);
		CLBChaseConstantSpecifyFrame(&unk8, radius, remFrames);
		CLBChaseConstantSpecifyFrame(&unkC, pitch, remFrames);
		if (unk3C == 0) {
			CLBChaseConstantSpecifyFrame(&unkE, yaw, remFrames);
		} else {
			unkE += unk40;
		}

		CLBPolarToCross(mAt, &mPos, unk8, unkC, unkE);
		mFramesRemaining -= 1;
		if (mFramesRemaining == 0)
			unk3C = 0;
	}

	unk30.set(param_3);
}

void TCameraInbetween::execInbetweenAndCalcPosAndAt(
    const JGeometry::TVec3<f32>& param_1, const JGeometry::TVec3<f32>& param_2,
    const JGeometry::TVec3<f32>& param_3, f32 param_4, f32 param_5, f32 param_6,
    f32 param_7, JGeometry::TVec3<f32>* param_8, JGeometry::TVec3<f32>* param_9)
{
	// TODO: partial reconstruction (0x7c of the map's 0xec). Deadstripped, so
	// there is no disassembly; the only hard evidence is the .sdata2 literal
	// pool. @1663 (0.0f) carries a lower id than execCameraInbetween's own
	// 0.001f/0.1f, so 0.0f is first requested by this function, which is
	// emitted just before it under -inline deferred. param_4..param_7 are
	// still unused here.
	unk44 = 0.0f;
	execCameraInbetween(param_1, param_2, param_3);
	param_8->set(mPos);
	param_9->set(mAt);
}

// The map's .sdata2 layout for this TU pins both of the functions below.
// Retail's pool is 1.0f, <4-byte gap>, the signed int->float magic double,
// 0.0f, 0.001f, 0.1f, and ids are assigned in emission order, i.e. reverse
// source order. So 1.0f and the int->float conversion are first requested by
// the *last* function in the file, not by execCameraInbetween: with both stubs
// empty our pool started at 0.0f, our block lost the 4-byte alignment hole in
// front of the double, and the DOL's .sdata2 came out 8 bytes short, which is
// why this unit could not be source-linked before.
void TCameraInbetween::setInbetModePosAngleY(
    TCameraInbetween::EnumAngleInbetMode mode, s16 angle_y, s16 param_3)
{
	// TODO: partial reconstruction (0x70 of the map's 0xbc). unk40 is the
	// per-frame yaw step execCameraInbetween adds to unkE while unk3C != 0,
	// hence the reciprocal of a frame count; the exact spelling and which of
	// the two s16 arguments is the target angle are unverified.
	unk3C = mode;
	unk40 = (s16)((angle_y - param_3) * (1.0f / (f32)mFramesRemaining));
}
