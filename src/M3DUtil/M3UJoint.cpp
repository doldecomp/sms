#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <M3DUtil/M3UJoint.hpp>
#include <JSystem/JMath.hpp>

void M3UMtxCalcBlendAux(u16 param_1, J3DTransformInfo* param_2,
                        J3DTransformInfo* param_3, float param_4, bool param_5)
{
	Vec local_8c;
	float fVar1 = 1.0f - param_4;
	if (param_5) {
		local_8c.z = 1.0f;
		local_8c.y = 1.0f;
		local_8c.x = 1.0f;
	} else {
		local_8c.x = J3DSys::mCurrentS.x;
		local_8c.y = J3DSys::mCurrentS.y;
		local_8c.z = J3DSys::mCurrentS.z;
	}

	float fVar4 = param_2->mScale.x * fVar1 + param_3->mScale.x * param_4;
	float fVar3 = param_2->mScale.y * fVar1 + param_3->mScale.y * param_4;
	float fVar2 = param_2->mScale.z * fVar1 + param_3->mScale.z * param_4;

	J3DSys::mCurrentS.x *= fVar4;
	J3DSys::mCurrentS.y *= fVar3;
	J3DSys::mCurrentS.z *= fVar2;

	bool bVar5;
	if (J3DSys::mCurrentS.x == 1.0f && J3DSys::mCurrentS.y == 1.0f
	    && J3DSys::mCurrentS.z == 1.0f)
		bVar5 = true;
	else
		bVar5 = false;

	j3dSys.mModel->mScaleFlagArr[param_1] = bVar5;

	Quaternion QStack_c8;
	Quaternion QStack_b8;
	Quaternion QStack_a8;
	JMAEulerToQuat(param_2->mRotation.x, param_2->mRotation.y,
	               param_2->mRotation.z, &QStack_b8);
	JMAEulerToQuat(param_3->mRotation.x, param_3->mRotation.y,
	               param_3->mRotation.z, &QStack_a8);
	JMAQuatLerp(&QStack_b8, &QStack_a8, param_4, &QStack_c8);

	Mtx local_7c;
	PSMTXQuat(local_7c, &QStack_c8);
	local_7c[0][3]
	    = local_8c.x
	      * (param_2->mTranslate.x * fVar1 + param_3->mTranslate.x * param_4);
	local_7c[1][3]
	    = local_8c.y
	      * (param_2->mTranslate.y * fVar1 + param_3->mTranslate.y * param_4);
	local_7c[2][3]
	    = local_8c.z
	      * (param_2->mTranslate.z * fVar1 + param_3->mTranslate.z * param_4);
	if (param_5) {
		if (!bVar5) {
			local_7c[0][0] = local_7c[0][0] * fVar4;
			local_7c[0][1] = local_7c[0][1] * fVar3;
			local_7c[0][2] = local_7c[0][2] * fVar2;
			local_7c[1][0] = local_7c[1][0] * fVar4;
			local_7c[1][1] = local_7c[1][1] * fVar3;
			local_7c[1][2] = local_7c[1][2] * fVar2;
			local_7c[2][0] = local_7c[2][0] * fVar4;
			local_7c[2][1] = local_7c[2][1] * fVar3;
			local_7c[2][2] = local_7c[2][2] * fVar2;
		}
		PSMTXConcat(J3DSys::mCurrentMtx, local_7c, J3DSys::mCurrentMtx);
		PSMTXCopy(J3DSys::mCurrentMtx, j3dSys.mModel->mNodeMatrices[param_1]);
	} else {
		PSMTXConcat(J3DSys::mCurrentMtx, local_7c, J3DSys::mCurrentMtx);
		if (bVar5) {
			PSMTXCopy(J3DSys::mCurrentMtx,
			          j3dSys.mModel->mNodeMatrices[param_1]);
		} else {
			local_7c[0][0] = J3DSys::mCurrentMtx[0][0] * J3DSys::mCurrentS.x;
			local_7c[0][1] = J3DSys::mCurrentMtx[0][1] * J3DSys::mCurrentS.y;
			local_7c[0][2] = J3DSys::mCurrentMtx[0][2] * J3DSys::mCurrentS.z;
			local_7c[0][3] = J3DSys::mCurrentMtx[0][3];
			local_7c[1][0] = J3DSys::mCurrentMtx[1][0] * J3DSys::mCurrentS.x;
			local_7c[1][1] = J3DSys::mCurrentMtx[1][1] * J3DSys::mCurrentS.y;
			local_7c[1][2] = J3DSys::mCurrentMtx[1][2] * J3DSys::mCurrentS.z;
			local_7c[1][3] = J3DSys::mCurrentMtx[1][3];
			local_7c[2][0] = J3DSys::mCurrentMtx[2][0] * J3DSys::mCurrentS.x;
			local_7c[2][1] = J3DSys::mCurrentMtx[2][1] * J3DSys::mCurrentS.y;
			local_7c[2][2] = J3DSys::mCurrentMtx[2][2] * J3DSys::mCurrentS.z;
			local_7c[2][3] = J3DSys::mCurrentMtx[2][3];
			PSMTXCopy(local_7c, j3dSys.mModel->mNodeMatrices[param_1]);
		}
	}
}

M3UMtxCalcSIAnmBlendQuat::M3UMtxCalcSIAnmBlendQuat(bool param_1)
    : J3DMtxCalcSoftimage()
{
	unk50 = 0.0f;
	unk60 = 0.0f;
	unk58 = nullptr;
	unk54 = nullptr;
	unk5C = param_1;
}

void M3UMtxCalcSIAnmBlendQuat::calcTransform(u16 param_1,
                                             const J3DTransformInfo& param_2)
{
	if (unk5C)
		J3DMtxCalcBasic::calcTransform(param_1, param_2);
	else
		J3DMtxCalcSoftimage::calcTransform(param_1, param_2);
}

void M3UMtxCalcSIAnmBlendQuat::init(const Vec& vec, const Mtx& mtx)
{
	if (unk5C) {
		J3DSys::mCurrentS = vec;
		J3DSys::mParentS  = (Vec) { 1.0f, 1.0f, 1.0f };

		J3DSys::mCurrentMtx[0][0] = mtx[0][0] * vec.x;
		J3DSys::mCurrentMtx[0][1] = mtx[0][1] * vec.y;
		J3DSys::mCurrentMtx[0][2] = mtx[0][2] * vec.z;
		J3DSys::mCurrentMtx[0][3] = mtx[0][3];
		J3DSys::mCurrentMtx[1][0] = mtx[1][0] * vec.x;
		J3DSys::mCurrentMtx[1][1] = mtx[1][1] * vec.y;
		J3DSys::mCurrentMtx[1][2] = mtx[1][2] * vec.z;
		J3DSys::mCurrentMtx[1][3] = mtx[1][3];
		J3DSys::mCurrentMtx[2][0] = mtx[2][0] * vec.x;
		J3DSys::mCurrentMtx[2][1] = mtx[2][1] * vec.y;
		J3DSys::mCurrentMtx[2][2] = mtx[2][2] * vec.z;
		J3DSys::mCurrentMtx[2][3] = mtx[2][3];

	} else {
		J3DSys::mCurrentS = vec;
		MTXCopy((Mtx&)mtx, J3DSys::mCurrentMtx);
	}
}

void M3UMtxCalcSIAnmBlendQuat::calc(u16 param_1)
{
	J3DTransformInfo info;
	J3DTransformInfo infoOld;
	J3DTransformInfo infoNew;

	j3dSys.setCurrentMtxCalc(this);

	if ((unk54 == nullptr && unk58 == nullptr)
	    || (unk54 == nullptr && unk50 == 0.0f)
	    || (unk58 == nullptr && unk50 == 1.0f)) {
		info = j3dSys.getModel()
		           ->getModelData()
		           ->getJointNodePointer(param_1)
		           ->getTransformInfo();
		calcTransform(param_1, info);
	} else if (unk50 == 0.0f || unk58 == nullptr) {
		unk54->getTransform(param_1, &info);
		calcTransform(param_1, info);
	} else if (unk50 == 1.0f || unk54 == nullptr) {
		unk58->getTransform(param_1, &info);
		calcTransform(param_1, info);
	} else {
		unk54->getTransform(param_1, &infoNew);
		J3DTransformInfo* ptr = &infoOld;
		unk58->getTransform(param_1, ptr);
		M3UMtxCalcBlendAux(param_1, &infoNew, &infoOld, unk50, unk5C);
	}
}
