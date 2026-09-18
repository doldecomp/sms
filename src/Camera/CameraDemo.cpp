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

// TODO: 93.2% (57.8% before closure batch 83). The rotation blocks are now
// retail's shape: each offset is a three-argument TVec3 constructor over the
// component differences (a `sub(a, b)` call interleaves the stfs where retail
// batches the three fsubs), only the x component is named so that MWCC CSEs
// `-x` into retail's early `fneg`, JMASSin/JMASCos are spelled out at every
// use (naming `sn`/`cs` loads each table entry once where retail reloads them
// four times from two CSE'd addresses), and the write-back is `origin +
// offset`, whose by-value left operand is retail's three-word copy in front of
// `bl TVec3::add`. What is left: retail keeps `origin.y` in f31 and `origin.z`
// in f30 across both `bl add` calls and reloads only `origin.x` per block, so
// its frame is 0xc8 to our 0xb8 -- exactly the two FPR saves, the local area
// being the same size -- and the two `operator+` temporaries sit at 0x58/0x4c
// instead of 0x7c/0x70, i.e. three more 12-byte temporaries are expanded ahead
// of them. The same "x reloaded, y and z promoted to callee-saved FPRs"
// asymmetry blocks TMapCollisionBase::updateTrans in Map/MapMakeData.cpp; it
// is one unexplained MWCC pattern, not two.
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

				// TODO: updateDemoCamera_ is 99.7% with the frame exact.
				// Two residues: retail loads each member of unk124/unk148
				// immediately before its subtrahend (0x124, 0xa0, 0x128,
				// 0xa4, ...) while we load the two named scalars one slot
				// early, and our two 12-byte `origin + offset` temporaries
				// sit 0x24 higher than retail's (0x7c/0x70 vs 0x58/0x4c)
				// although the total frame matches, so there is a 36-byte
				// hole elsewhere in the low region.
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

// TODO: 99.9%. Instruction-exact and the frame is right (0x30); only `fovy`
// sits 4 bytes low (0x18 vs 0x1c), i.e. we have one 4-byte temporary too many
// between it and the int-to-float magic pair at 0x20. Removing an inline level
// around `mInbetween->getUnk4()` or the `(f32)v` conversion is the lever to
// try; a +4 low step is never an accessor (those come in 8s).
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

// TODO: 99.9%. Every instruction matches; frame 0x50 vs 0x30 with `diff` the
// only referenced local (0x30 vs 0x14), so retail has 28 dead bytes below it
// and 4 above. Dead-low-region family with no carrier identified: the only
// inlined callees here are MsVECMag2, MsClamp and the CameraInbetween call,
// and this TU's UNUSED list (getTotalDemoFrames, endSimpleDemoCamera_,
// endReproduceDemoCamera_, restartReproduceDemoCamera_ 0x44 still empty,
// startReproduceDemoCamera_) contains nothing this function reaches.
// TODO: 99.9%, every instruction exact, frame 0x30 vs 0x50.  The named region
// is identical relative to the frame top (the int->float magic pair highest,
// then `diff`), so all 32 bytes are inline-temp/outgoing-parameter area below
// `diff`: retail's locals start at 0x30 (0x8-0x2f, the standard 32-byte
// outgoing-parameter area plus 4 of alignment), ours at 0x14.  Ruled out
// (closure batch 90): a by-value `Vec` parameter on the vector overload of
// CLBChaseDecrease, which is the only construct here that could reserve a
// parameter area of that size -- it is +16 but emits retail's missing
// three-word copy (90.0%, +6 instructions), so cameralib.hpp's `const Vec&`
// is right.  MsVECMag2 and all three CLBChaseDecrease calls are already `bl`s
// in both builds.
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
