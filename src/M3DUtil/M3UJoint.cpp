#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <M3DUtil/M3UJoint.hpp>
#include <JSystem/JMath.hpp>

// TODO: 98.8%, frame 0x100 exact and all 233 instructions in the right order;
// the only residue is a rotation of the callee-saved registers. Retail ranks
// them r31 = &J3DSys::mCurrentS, r30 = &j3dSys.mModel, r29 = (u32)param_1,
// r28 = basic, r27 = param_3, r26 = param_2, r25 = bVar5, r24 = pQuat; we agree
// on r31 and r24 but put the j3dSys model address last (r25) and shift the
// other five up one.
// Diagnostic: `setScaleFlag(param_1, bVar5 != 0)` reproduces retail's ranking
// exactly and leaves only the five bool-normalisation instructions it adds
// (neg/subic/subfe plus the moved mScaleFlagArr load) -- so the body is right
// and something about bVar5's live range is what orders the allocator. A
// cheaper spelling of that extra step is the open question.
// Measured as no improvement: j3dSys.mModel raw (98.5%), a J3DSys&/J3DSys*/
// J3DModel*& /J3DModel** local for the model (95.9-96.1%), a TU-local
// M3UGetModel() wrapper (unchanged), bool/u8 bVar5, an explicit (u8) cast on
// the argument, declaring bVar5 at its initialiser, moving `currentS` first,
// hoisting pQuat, a named copy of bVar5 or of the joint index, a named
// J3DModel* before setScaleFlag, and spelling J3DSys::mCurrentS out instead of
// the currentS reference (90.7%).
// Closure batch 120 added twenty more measurements, all keeping the model
// address in r25 (the bottom of the callee-saved group):
//   - a TU-local `static inline` binding level around setScaleFlag, around the
//     three setAnmMtx sites, or both: byte-identical (levels are inert here);
//   - `J3DModel** ppModel = &j3dSys.mModel;` / `J3DModel*& model` declared at
//     the setScaleFlag site or just after it, used at the setAnmMtx sites:
//     98.5%, 128 differing operands (making the address a source variable does
//     not raise its rank);
//   - `j3dSys.mModel` raw at the setAnmMtx sites only 98.5%, at the
//     setScaleFlag site only 98.8% (unchanged);
//   - bVar5 declared first 98.6%, declared last 98.7% (current position is the
//     best); three named `f32`s for the blended scale instead of local_98
//     97.8%; `Vec* currentS` instead of the reference, and
//     `local_8c = J3DSys::mCurrentS` in the else branch: both unchanged.
// A named `J3DModel* model = j3dSys.getModel();` before setScaleFlag is a
// zero-instruction "+4 named / -4 low" lever: the by-value checkScaleOne Vec
// temporary moves 0x2c -> 0x28 with the frame still 0x100. Retail's is 0x2c,
// so this function does not want it, but the lever is reusable.
// The temp is *not* pinned to the bottom of the group: shortening `currentS`'s
// live range promotes it straight to r31 (spelling J3DSys::mCurrentS out in
// the tail pointer-walk block 96.5%, or everywhere except the checkScaleOne
// argument 90.5%). So its priority sits between `currentS`'s and the
// parameters', and the lever has to lower the parameters' priority (or add one
// short-lived value between the address's definition and `clrlwi r29`, which
// is exactly what the bVar5 normalisation supplies) -- not add an inline level.
// Batch 131 added the batch-127 fork-plus-binding shapes, which are the last
// untried family and all make it worse by perturbing the frame instead:
// a binding level reading `j3dSys.mModel` raw at all four sites (frame 0x120),
// at the setScaleFlag site only (0x108), the same nested twice (0x150) and a
// `static inline J3DSys* M3UJointSys()` fork above `getModel()` (0x150) -- 127
// differing operands each against the base's 55, with the model address still
// in r25. So levels really are inert on this ranking (batch 120) and a level
// that is *not* inert only buys frame; the lever must change the live ranges.
// The cheapest diagnostics are `bVar5 != 0` and `bVar5 ? 1 : 0` (97.0/97.5%,
// nine differing lines: the neg/subic/subfe triple plus the moved
// mScaleFlagArr load); `(bool)bVar5` and `!!bVar5` cost one more.
void M3UMtxCalcBlendAux(u16 param_1, J3DTransformInfo* param_2,
                        J3DTransformInfo* param_3, f32 param_4, bool basic)
{
	Mtx local_7c;
	BOOL bVar5;
	Vec local_8c;
	Vec local_98;
	Quaternion QStack_a8;
	Quaternion QStack_b8;
	Quaternion QStack_c8;

	Vec& currentS = J3DSys::mCurrentS;

	f32 fVar1 = 1.0f - param_4;
	if (basic) {
		local_8c.z = 1.0f;
		local_8c.y = 1.0f;
		local_8c.x = 1.0f;
	} else {
		local_8c = currentS;
	}

	local_98.x = param_2->mScale.x * fVar1 + param_3->mScale.x * param_4;
	local_98.y = param_2->mScale.y * fVar1 + param_3->mScale.y * param_4;
	local_98.z = param_2->mScale.z * fVar1 + param_3->mScale.z * param_4;

	currentS.x *= local_98.x;
	currentS.y *= local_98.y;
	currentS.z *= local_98.z;

	bVar5 = checkScaleOne(currentS);
	j3dSys.getModel()->setScaleFlag(param_1, bVar5);

	JMAEulerToQuat(param_2->mRotation.x, param_2->mRotation.y,
	               param_2->mRotation.z, &QStack_b8);
	Quaternion* pQuat = &QStack_a8;
	JMAEulerToQuat(param_3->mRotation.x, param_3->mRotation.y,
	               param_3->mRotation.z, pQuat);
	JMAQuatLerp(&QStack_b8, pQuat, param_4, &QStack_c8);

	MTXQuat(local_7c, &QStack_c8);
	local_7c[0][3]
	    = local_8c.x
	      * (param_2->mTranslate.x * fVar1 + param_3->mTranslate.x * param_4);
	local_7c[1][3]
	    = local_8c.y
	      * (param_2->mTranslate.y * fVar1 + param_3->mTranslate.y * param_4);
	local_7c[2][3]
	    = local_8c.z
	      * (param_2->mTranslate.z * fVar1 + param_3->mTranslate.z * param_4);
	if (basic) {
		if (!bVar5) {
			local_7c[0][0] *= local_98.x;
			local_7c[0][1] *= local_98.y;
			local_7c[0][2] *= local_98.z;
			local_7c[1][0] *= local_98.x;
			local_7c[1][1] *= local_98.y;
			local_7c[1][2] *= local_98.z;
			local_7c[2][0] *= local_98.x;
			local_7c[2][1] *= local_98.y;
			local_7c[2][2] *= local_98.z;
		}
		MTXConcat(J3DSys::mCurrentMtx, local_7c, J3DSys::mCurrentMtx);
		j3dSys.getModel()->setAnmMtx(param_1, J3DSys::mCurrentMtx);
	} else {
		MTXConcat(J3DSys::mCurrentMtx, local_7c, J3DSys::mCurrentMtx);
		if (bVar5) {
			j3dSys.getModel()->setAnmMtx(param_1, J3DSys::mCurrentMtx);
		} else {
			f32* p = local_7c[0];
			f32* q = J3DSys::mCurrentMtx[0];
			*p++   = *q++ * currentS.x;
			*p++   = *q++ * currentS.y;
			*p++   = *q++ * currentS.z;
			*p++   = *q++;
			*p++   = *q++ * currentS.x;
			*p++   = *q++ * currentS.y;
			*p++   = *q++ * currentS.z;
			*p++   = *q++;
			*p++   = *q++ * currentS.x;
			*p++   = *q++ * currentS.y;
			*p++   = *q++ * currentS.z;
			*p++   = *q++;

			j3dSys.getModel()->setAnmMtx(param_1, local_7c);
		}
	}
}

M3UMtxCalcSIAnmBlendQuat::M3UMtxCalcSIAnmBlendQuat(bool basic)
    : J3DMtxCalcSoftimage()
{
	mMotionBlendRatio = 0.0f;
	unk60             = 0.0f;
	mOldAnm           = nullptr;
	mNewAnm           = nullptr;
	mBehaveAsBasic    = basic;
}

void M3UMtxCalcSIAnmBlendQuat::calcTransform(u16 param_1,
                                             const J3DTransformInfo& param_2)
{
	if (mBehaveAsBasic)
		J3DMtxCalcBasic::calcTransform(param_1, param_2);
	else
		J3DMtxCalcSoftimage::calcTransform(param_1, param_2);
}

void M3UMtxCalcSIAnmBlendQuat::init(const Vec& vec, const Mtx& mtx)
{
	if (mBehaveAsBasic)
		J3DMtxCalcBasic::init(vec, mtx);
	else
		J3DMtxCalcSoftimage::init(vec, mtx);
}

void M3UMtxCalcSIAnmBlendQuat::calc(u16 param_1)
{
	J3DTransformInfo info;
	J3DTransformInfo infoOld;
	J3DTransformInfo infoNew;

	j3dSys.setCurrentMtxCalc(this);

	if ((mNewAnm == nullptr && mOldAnm == nullptr)
	    || (mNewAnm == nullptr && mMotionBlendRatio == 0.0f)
	    || (mOldAnm == nullptr && mMotionBlendRatio == 1.0f)) {
		info = j3dSys.getModel()
		           ->getModelData()
		           ->getJointNodePointer(param_1)
		           ->getTransformInfo();
		calcTransform(param_1, info);
	} else if (mMotionBlendRatio == 0.0f || mOldAnm == nullptr) {
		mNewAnm->getTransform(param_1, &info);
		calcTransform(param_1, info);
	} else if (mMotionBlendRatio == 1.0f || mNewAnm == nullptr) {
		mOldAnm->getTransform(param_1, &info);
		calcTransform(param_1, info);
	} else {
		mNewAnm->getTransform(param_1, &infoNew);
		J3DTransformInfo* ptr = &infoOld;
		mOldAnm->getTransform(param_1, ptr);
		M3UMtxCalcBlendAux(param_1, &infoNew, &infoOld, mMotionBlendRatio,
		                   mBehaveAsBasic);
	}
}
