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

// Rotation blocks match retail (3-arg TVec3 offset, named x, spelled-out
// JMASSin/JMASCos, `origin + offset` write-back). Frame is exact at 0xc8.
// Residue is the 36-byte pool plus first-ctor load order; see the TODO
// inside the body.
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

				// Research batch 86: retail keeps origin's z and y in
				// f31/f30 across both `bl TVec3::add` calls and re-reads
				// only `.x`. Only a *named* f32 local of this function's
				// body gets a callee-saved FPR; an aggregate member read is
				// reloaded at every use. `z` before `y` is what puts z in
				// f31 and y in f30.
				f32 f31 = origin.z;
				f32 f30 = origin.y;

				JGeometry::TVec3<f32> posOffset(unk124.x - origin.x,
				                                unk124.y - f30,
				                                unk124.z - f31);
				f32 posX    = posOffset.x;
				posOffset.x = posX * JMASCos(angle)
				              + posOffset.z * JMASSin(angle);
				posOffset.z = -posX * JMASSin(angle)
				              + posOffset.z * JMASCos(angle);
				unk124 = origin + posOffset;

				JGeometry::TVec3<f32> atOffset(unk148.x - origin.x,
				                               unk148.y - f30,
				                               unk148.z - f31);
				f32 atX    = atOffset.x;
				atOffset.x = atX * JMASCos(angle)
				             + atOffset.z * JMASSin(angle);
				atOffset.z = -atX * JMASSin(angle)
				             + atOffset.z * JMASCos(angle);
				unk148 = origin + atOffset;

				// TODO: updateDemoCamera_ is 99.7% with the frame exact
				// (0xc8) and every referenced named slot exact: atOffset
				// 0x88, posOffset 0x94, origin 0xa0.  Two residues, both
				// below the named block.  (1) The two 12-byte `origin +
				// offset` by-value temporaries sit at 0x7c/0x70 where
				// retail has 0x58/0x4c -- same 12-byte spacing, so it is
				// not batch 119's two-slots-per-statement geometry; retail
				// simply allocates 36 bytes (three 12-byte objects) of pool
				// between the named block and the first temporary that we
				// do not, and has 36 fewer dead bytes underneath.  The
				// expansions before the first `add` are updateDemo (a bl),
				// CLBDegToShortAngle, origin's ctor and `= *unk0`, and the
				// two three-argument offset constructors; none of them has
				// a temporary in our build.  (2) Retail loads each
				// unk124/unk148 component immediately before its
				// subtrahend (0x128 then 0xa4, 0x12c then 0xa8) while we
				// load origin's component first.  The named y/z locals
				// that buy f30/f31 across `bl add` always hoist those
				// origin loads ahead of unk124.y/z; that tension is the
				// same research class as updateTrans / feetinv.
				// Rejected this pass (all still 99.7%, same ~18 / 0x7c):
				// declaring f31/f30 after the first ctor; assignment-in-
				// expression `unk124.y - (f30 = origin.y)`; a TU-local
				// offset helper returning TVec3 (RVO, no extra pool);
				// a TU-local rotateY helper (instruction-neutral);
				// an unread defaulted `const TVec3& = TVec3()` on that
				// helper (MWCC drops an unused default).  Also rejected
				// earlier: raw origin.y/z (lose the FPRs, 93.2%) and
				// declaring y before z (99.6%).
				// Batch cc24: residue (2) is a CSE, not named locals.
				// Declaring origin as a plain `Vec` (x/y/z stored
				// separately, `origin = *unk0` as a slice) with raw
				// origin.y/z in both offset constructors lets MWCC keep
				// the two components across `bl add` and reproduces
				// retail's interleaved load order exactly (TVec3's cast
				// copy constructor blocks the CSE), but y then lands in
				// f31 and z in f30, the named block drops 4 and the
				// temporaries stay 0x20 high (95.6%); no component order
				// or early-declared offset fixes the swap. Pool levels
				// around the angle, the adds or the origin read, and a
				// rotate-about-origin helper, all leave (1) in place.
				f32 upX = mUp.x;
				mUp.x   = upX * JMASCos(angle) + mUp.z * JMASSin(angle);
				mUp.z   = -upX * JMASSin(angle) + mUp.z * JMASCos(angle);
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
	if (getUnk70() != mCameraDemo->unk8 && v > 0)
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

	const char* name = actor->getName();
	snprintf(buf, 0x80, "%s前カメラ", name);
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

// The 32 bytes ctrlNormalDeadDemo_ used to be short (0x30 against retail's
// 0x50) were three bindings, none of which changes an instruction.  The
// arithmetic is exact: `diff`, the only referenced local, sits at
// 0xc + <low region>, retail has it at 0x30 and so a low region of 36, and the
// baseline is 8.  Accessor and binding levels here come in 8-byte steps
// (measured: the mInbetween fetch at either call site +8 or +0x10, &mPosition
// +8, &mInbetween->mAt +0x10, &mCurrentTarget.mTarget +0x10, a binding fork of
// gpCameraMario or gpMarioOriginal +0x10 each and +0x18 together, a nested
// pair of forks +0x18, &SMS_GetMarioPos() +0x10), so no combination of them
// can reach 36 -- every one lands `diff` on 0x14 + 8k and never on 0x30.  The
// binding around MsVECMag2 is the odd rung: with the two 8-byte ones it makes
// exactly 28.  A dead unwritten 4-byte local declared before `diff` also
// reaches the right frame but puts the 4 bytes of padding on the wrong side of
// it (0x2c or 0x34 instead of 0x30), and a named f32 or a named pointer local
// reserves nothing.
// Ruled out earlier (closure batch 90): a by-value `Vec` parameter on the
// vector overload of CLBChaseDecrease -- it is +16 but emits a three-word copy
// retail lacks (90.0%), so cameralib.hpp's `const Vec&` is right.  MsVECMag2
// and all three CLBChaseDecrease calls are `bl`s in both builds.
static inline TCameraInbetween*
CameraDemoGetInbetween(const CPolarSubCamera* camera)
{
	TCameraInbetween* inbetween = camera->mInbetween;
	return inbetween;
}

static inline const JGeometry::TVec3<f32>*
CameraDemoGetPosition(const CPolarSubCamera* camera)
{
	const JGeometry::TVec3<f32>* position = &camera->mPosition;
	return position;
}

static inline f32
CameraDemoVecMag(Vec* v)
{
	f32 mag = MsVECMag2(v);
	return mag;
}

void CPolarSubCamera::ctrlNormalDeadDemo_()
{
	mCurrentTarget.mTarget.set(gpCameraMario->unk0);
	CameraDemoGetInbetween(this)->execCameraInbetween(
	    *CameraDemoGetPosition(this), mCurrentTarget.mTarget,
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
		f32 distToTarget = CameraDemoVecMag(&diff);
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
