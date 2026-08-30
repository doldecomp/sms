#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <M3DUtil/M3UJoint.hpp>
#include <JSystem/JMath.hpp>

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
