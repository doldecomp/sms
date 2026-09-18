#ifndef M3DUTIL_M3U_JOINT_HPP
#define M3DUTIL_M3U_JOINT_HPP

#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <dolphin/mtx.h>

/**
 * @brief Softimage|3D style MtxCalc that can blend between two different
 * transform animation for smooth poses change. Aka motion blending.
 * Optionally can behave like a basic-style calc w.r.t. scale.
 */
class M3UMtxCalcSIAnmBlendQuat : public J3DMtxCalcSoftimage {
public:
	// One constructor with a default argument, not two: retail's weak
	// __ct__24M3UMtxCalcSIAnmBlendQuatFv (0x24, enemyMario.o) is the
	// no-argument entry point MWCC synthesises for the default, and its
	// body is `li r5, 0` plus a tail `bl __ct__...Fb` that forwards the
	// hidden virtual-base flag in r4 -- a thunk, not a written body.
	// `new M3UMtxCalcSIAnmBlendQuat[2]` is what needs that signature.
	M3UMtxCalcSIAnmBlendQuat(bool basic = false);

	virtual void init(const Vec&, const Mtx&);
	virtual void calcTransform(u16, const J3DTransformInfo&);
	virtual void calc(u16);

	void advanceMotionBlend(f32 delta)
	{
		mMotionBlendRatio += delta;
		if (mMotionBlendRatio < 0.0f)
			mMotionBlendRatio = 0.0f;
		else if (mMotionBlendRatio > 1.0f)
			mMotionBlendRatio = 1.0f;
	}

	void joinAnm(J3DAnmTransform* anm)
	{
		if (mNewAnm == anm)
			return;

		mOldAnm           = mNewAnm;
		mNewAnm           = anm;
		mMotionBlendRatio = 1.0f;
	}

	void setAnm(J3DAnmTransform* anm)
	{
		mNewAnm           = anm;
		mOldAnm           = 0;
		mMotionBlendRatio = 0.0f;
	}

public:
	/* 0x50 */ f32 mMotionBlendRatio;
	/* 0x54 */ J3DAnmTransform* mNewAnm; ///< for mMotionBlendRatio == 0
	/* 0x58 */ J3DAnmTransform* mOldAnm; ///< for mMotionBlendRatio == 1
	/* 0x5C */ bool mBehaveAsBasic;
	/* 0x60 */ f32 unk60;
};

void M3UMtxCalcBlendAux(u16 param_1, J3DTransformInfo* param_2,
                        J3DTransformInfo* param_3, f32 param_4, bool param_5);

#endif
