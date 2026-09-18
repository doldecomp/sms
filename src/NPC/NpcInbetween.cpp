#include <NPC/NpcInbetween.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <M3DUtil/MActor.hpp>

// TODO: float-register permutation only (99.1%). Retail keeps the ratio in f1
// and the per-component base in f2; we get f2/f1, the instruction stream and
// the 0x20 frame are otherwise identical. An inline ratio accessor
// (`f32 getPosInbetweenRatio() const`) restores every register but costs
// exactly 8 bytes of frame, and there is no room: the function is a leaf whose
// only temporaries are the two int->double conversion doubles at 0x10 and 0x18.
// Measured inline-expansion temp cost here: `this` alone or one parameter +8,
// two parameters +16, so no accessor shape can be free. Also tried with no
// effect: operand swap in the product, `progress * diff` in the three
// components, split declaration, `const f32`, casts on either operand or the
// whole product, and declaring `progress` before the decrement. Worse:
// `(1.0f / frame) * timer` and `1.0f / frame * timer` (98.1%, swaps the two
// conversion slots), a named `rate` local (+8), the rate-only accessor (+8),
// `mCurrentPos = *cur_pos` over `set()` (77.2%), and spelling the accessor out
// at all three components (recomputes, 45.9%).
void TNpcInbetween::execPosInbetween(JGeometry::TVec3<f32>* cur_pos)
{
	mCurrentPos.set(*cur_pos);
	if (mPosInbetweenTimer >= 2) {
		mPosInbetweenTimer -= 1;

		f32 progress = mPosInbetweenTimer * (1.0f / mPosInbetweenFrame);

		cur_pos->x = mCurrentPos.x + (mTargetPos.x - mCurrentPos.x) * progress;
		cur_pos->y = mCurrentPos.y + (mTargetPos.y - mCurrentPos.y) * progress;
		cur_pos->z = mCurrentPos.z + (mTargetPos.z - mCurrentPos.z) * progress;
	} else {
		mTargetPos.set(*cur_pos);
		mPosInbetweenTimer = 0;
	}
}

void TNpcInbetween::execMotionBlend(MActor* mactor)
{
	f32 ratio = 0.0f;
	if (isForcedBlendRatio()) {
		mMotionBlendTimer = 0;

		J3DAnmTransform* anm = mactor->getBckOldMotionBlendAnmPtr();

		if (anm != nullptr) {
			J3DFrameCtrl ctrl = *mactor->getFrameCtrl(ANM_TYPE_BCK);
			ctrl.update();
			anm->setFrame(ctrl.getFrame());
			ratio = mForcedBlendRatio;
		}
	} else if (isMotionBlending()) {
		if (mMotionBlendTimer > 0)
			mMotionBlendTimer -= 1;
		if (mMotionBlendTimer > 0) {
			J3DAnmTransform* anm = mactor->getBckOldMotionBlendAnmPtr();

			if (anm != nullptr)
				anm->setFrame(mactor->getBckOldMotionBlendFrame());

			ratio = mMotionBlendTimer * (1.0f / mMotionBlendFrame);
		}
	}

	mactor->setMotionBlendRatioForBck(ratio);
}
