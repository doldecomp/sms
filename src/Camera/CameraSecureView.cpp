#include <Camera/Camera.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <Camera/cameralib.hpp>
#include <Camera/CameraKindParam.hpp>
#include <Player/Mario.hpp>
#include <Player/MarioAccess.hpp>
#include <JSystem/JMath.hpp>

// TODO: macro in cameralib?
#define ABS(x) ((x) >= 0 ? (x) : -(x))

// The secure-view distances for the two axes. These have no symbol in the map,
// which dates them file-scope `inline`: an inlined free function's by-pointer
// parameter binds 8 bytes when the helper returns a *computed* value (it binds
// nothing when the body is only a member read), and that binding is exactly
// the 8 bytes calcSecureViewTarget_ was short of retail's 0x68. The X site
// alone is enough (the second expansion saturates); spelling the helper with
// `min`/`max` as parameters instead buys the same 8 bytes but loads the pair
// max-first, which retail does not.
static inline f32 CameraSecureViewDistX(const CPolarSubCamera* camera)
{
	return CLBLinearInbetween<f32>(camera->mCurrentParams->mSecureViewDistXMin,
	                               camera->mCurrentParams->mSecureViewDistXMax,
	                               camera->mCurrentTarget.unk28);
}

static inline f32 CameraSecureViewDistZ(const CPolarSubCamera* camera)
{
	return CLBLinearInbetween<f32>(camera->mCurrentParams->mSecureViewDistZMin,
	                               camera->mCurrentParams->mSecureViewDistZMax,
	                               camera->mCurrentTarget.unk28);
}

void CPolarSubCamera::calcSecureViewTarget_(s16 angle, f32* outX, f32* outZ)
{
	s16 base = SMS_GetMarioAngleY() - 0x8000;
	s16 diff = angle - base;

	f32 first = CameraSecureViewDistX(this);
	f32 cos_d = JMASCos(diff);

	f32 second;
	if (cos_d >= 0.0f) {
		second = 0.0f;
	} else {
		second = CameraSecureViewDistZ(this);
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
