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

// TODO: 99.8%, every instruction matches and only `r1` displacements differ.
// `pos` is declared before `usualLookat` because retail's `pos` has the
// higher slot (0x38 against usualLookat's 0x2c) and named locals are laid out
// downward from the top in declaration order.
// What is left is 4 bytes of low region: retail's `getUsualLookat()` return
// buffer sits at 0x20, ours at 0x1c, i.e. retail has one dead 4-byte
// temporary between the five-slot CLBPolarToCross argument area (0x8..0x1c)
// and that buffer; the named block then lands on 0x2c/0x38 and the frame's
// 8-byte alignment supplies the 4 bytes of padding at 0x44 that make 0x58.
// Every lever tried is either 0 or 8, never 4:
//   +8  a `getSaveKindParam(mode)` accessor for `mSaveKindParam[mMode]`,
//       whether parked here as a two-parameter `static inline` (returning a
//       pointer or a reference) or added to Camera.hpp as a real one-argument
//       member -- all three land the frame on 0x58 but push every slot 4 too
//       high. Putting it in the `const Vec&, const Vec&` overload instead
//       costs the same 8 here and breaks that overload (100 -> 99.9).
//   +8  any 4-byte named local declared first (grows the top, leaves the
//       return buffer at 0x1c).
//   0   `isLButtonCamera()` for `isLButtonCameraSpecifyMode(mMode)`, a
//       one-parameter parked accessor for `mSaveKindParam[mMode]` or for
//       `mCurrentParams` (both, too), a named `f32 dist` for
//       calcDistFromXRotRatio_(), a named `s16` pitch,
//       `JGeometry::TUtil<f32>::one()` as MsClamp's upper bound.
//   instruction changes: `usualLookat = getUsualLookat()` instead of
//       `set()`, a `const TVec3&` bound to the returned temporary, a named
//       MsClamp result, a table accessor indexed at the call site.
// Closure batch 115 localised the 4 bytes exactly. A dead 4-byte named local
// declared *last* (`s16 pitch;` after `usualLookat`) lands the frame on 0x58
// and every other displacement -- `usualLookat` 0x2c, `pos` 0x38, the r30/r31/
// f31 saves at 0x48/0x4c/0x50 -- on the target, leaving the getUsualLookat
// return buffer as the single remaining slot (0x1c against 0x20). So the
// residue is not a named local at all: retail's inline-temporary pool is
// 0xc..0x2c with the buffer at its *top* (0x20..0x2c) and 20 dead bytes below,
// while ours is 0xc..0x28 with the buffer at 0x1c..0x28 and only 16 below.
// What is missing is one 4-byte expansion temporary allocated *after* the
// getUsualLookat sret (batch 70's "an inlined callee's parameter temporary is
// a placeable 4-byte slot"), and the dead named local only fakes its size.
// Binding levels measured for it, all with the buffer's offset in brackets:
//   +0   a binding level on `isLButtonCameraSpecifyMode(mMode)` [0x1c]
//        (a level above a real `bl` is free, as the catalog says);
//   +0   a `CameraWarpSetVec(&usualLookat, getUsualLookat())` level binding
//        the destination pointer [0x1c];
//   +0   `pos` declared `JGeometry::TVec3<f32>` instead of `Vec` [0x1c];
//   +8   a five-parameter `static inline` level above CLBPolarToCross: frame
//        0x58 exactly, but it reorders the call (151 instructions to 145);
//   +0x10 a binding level around MsClamp<f32> (two expansions, +1 instruction);
//   the only spelling that reaches [0x20] is a `static inline` returning
//        `TVec3<f32>` by value above getUsualLookat, which allocates a second
//        12-byte buffer (frame 0x68, 158 instructions).
// Next: the carrier is probably inside the byte-exact `(const Vec&, const
// Vec&)` overload that is inlined here -- e.g. an inlined setter binding
// `mInbetween` for `mFramesRemaining = 0` -- so it has to be found without
// moving that overload.
void CPolarSubCamera::warpPosAndAt(f32 ratio, s16 yAngle)
{
	if (getCamMode() < CAMERA_MODE_REPRODUCE_DEMO) {
		mCurrentParams->copySaveParam(*mSaveKindParam[getCamMode()]);

		Vec pos;

		JGeometry::TVec3<f32> usualLookat;
		usualLookat.set(getUsualLookat());

		if (isLButtonCameraSpecifyMode(getCamMode()))
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
