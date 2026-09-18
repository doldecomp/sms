#include <Camera/Camera.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <Camera/cameralib.hpp>
#include <Camera/CameraKindParam.hpp>
#include <Player/Mario.hpp>
#include <Player/MarioAccess.hpp>
#include <JSystem/JMath.hpp>

// TODO: macro in cameralib?
#define ABS(x) ((x) >= 0 ? (x) : -(x))

// TODO: frame 0x60 vs retail 0x68; the body is instruction-exact and every r1
// displacement is 8 low, so this is the "last 8 bytes" shape (one 8-byte
// object below every local, docs/catalog/frame-gaps.md). The object has to be
// a *trivial* 8-byte aggregate of this body: execSecureView_ inlines this
// function and is exact without it, and the table says a trivial POD local of
// an inlined callee is dropped while the same local in its own body is +8.
// Measured and rejected: naming the two output products (calc 0x60, batches
// the sine/cosine lookups, 87%), naming the sine (calc 0x58 and a wrong
// fmadds), SMS_GetMarioAngleY() here (+0 for calc, +8 for the caller),
// CLBAbs/predicate/accessor trials (batch 32). No source-level evidence names
// the object, so it stays nonmatching rather than padded.
void CPolarSubCamera::calcSecureViewTarget_(s16 angle, f32* outX, f32* outZ)
{
	s16 base = SMS_GetMarioAngleY() - 0x8000;
	s16 diff = angle - base;

	f32 first = CLBLinearInbetween<f32>(mCurrentParams->mSecureViewDistXMin,
	                                    mCurrentParams->mSecureViewDistXMax,
	                                    mCurrentTarget.unk28);
	f32 cos_d = JMASCos(diff);

	f32 second;
	if (cos_d >= 0.0f) {
		second = 0.0f;
	} else {
		second = CLBLinearInbetween<f32>(mCurrentParams->mSecureViewDistZMin,
		                                 mCurrentParams->mSecureViewDistZMax,
		                                 mCurrentTarget.unk28);
	}

	f32 mag = -ABS(first * JMASSin(diff) + second * cos_d);

	*outX = mag * JMASSin(base);
	*outZ = mag * JMASCos(base);
}

void CPolarSubCamera::execSecureView_(s16 angle, Vec* out)
{
	// The pair has to be one aggregate: calcSecureViewTarget_ is inlined here
	// and the two stores fold away, so two `f32` locals lose their slots and
	// leave the frame 8 bytes short (same shape as TMessageLoader's header
	// pair). The other 8 bytes come from SMS_GetMarioAngleY() in the inlined
	// body above.
	f32 target[2];
	calcSecureViewTarget_(angle, &target[0], &target[1]);

	s16 diff  = ABS(SMS_GetMarioAngleY() - gpMarioOriginal->getUnk9C());
	f32 ratio = SHORTANGLE2DEG(diff);
	f32 inv;
	if (ratio <= 1.0f)
		inv = 1.0f;
	else
		inv = 1.0f / ratio;

	f32 v = MsClamp(mCurrentParams->mSecureViewChase * inv, 0.0f, 1.0f);

	CLBChaseDecrease(&unk294, target[0], v, 0.0f);
	CLBChaseDecrease(&unk298, target[1], v, 0.0f);

	out->x += unk294;
	out->z += unk298;
}
