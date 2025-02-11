#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTransform.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DDrawBuffer.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DShape.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DMaterialAnm.hpp>

#pragma opt_strength_reduction off

void J3DMtxCalcAnm::calc(u16 param_0)
{
	j3dSys.setCurrentMtxCalc(this);
	J3DTransformInfo info;
	if (mOne[0]) {
		mOne[0]->getTransform(param_0, &info);
	} else {
		info = j3dSys.getModel()
		           ->getModelData()
		           ->getJointNodePointer(param_0)
		           ->getTransformInfo();
	}
	calcTransform(param_0, info);
}

J3DMtxCalcBasic::J3DMtxCalcBasic() { }

void J3DMtxCalcBasic::recursiveUpdate(J3DNode*) { }

void J3DMtxCalcBasic::recursiveCalc(J3DNode* node)
{
	if (!node)
		return;

	J3DMtxCalcBasic mtxCalc;
	char trash[0x8]; // TODO:

	MTXCopy(J3DSys::mCurrentMtx, mtxCalc.getBackupMtx());
	mtxCalc.setBackupS(J3DSys::mCurrentS);
	mtxCalc.setBackupParentS(J3DSys::mParentS);
	node->calcIn();

	if (node->getCallBack())
		node->getCallBack()(node, 0);

	recursiveCalc(node->getChild());
	MTXCopy(mtxCalc.getBackupMtx(), J3DSys::mCurrentMtx);
	J3DSys::mCurrentS = mtxCalc.getBackupS();
	J3DSys::mParentS  = mtxCalc.getBackupParentS();
	node->calcOut();

	if (node->getCallBack())
		node->getCallBack()(node, 1);

	recursiveCalc(node->getYounger());
}

void J3DMtxCalcBasic::recursiveEntry(J3DNode*) { }

// TODO: what is this? This isn't part of mtx.h =/
inline BOOL checkScaleOne(Vec v)
{
	if (v.x == 1.0f && v.y == 1.0f && v.z == 1.0f) {
		return TRUE;
	} else {
		return FALSE;
	}
}

void J3DMtxCalcBasic::calcTransform(u16 param_0, const J3DTransformInfo& info)
{
	J3DSys::mCurrentS.x *= info.mScale.x;
	J3DSys::mCurrentS.y *= info.mScale.y;
	J3DSys::mCurrentS.z *= info.mScale.z;
	s32 r29;
	if (checkScaleOne(J3DSys::mCurrentS)) {
		j3dSys.getModel()->setScaleFlag(param_0, 1);
		r29 = 1;
	} else {
		j3dSys.getModel()->setScaleFlag(param_0, 0);
		r29 = 0;
	}
	Mtx mtx;
	J3DGetTranslateRotateMtx(info, mtx);
	if (r29 == 0) {
		mtx[0][0] *= info.mScale.x;
		mtx[0][1] *= info.mScale.y;
		mtx[0][2] *= info.mScale.z;
		mtx[1][0] *= info.mScale.x;
		mtx[1][1] *= info.mScale.y;
		mtx[1][2] *= info.mScale.z;
		mtx[2][0] *= info.mScale.x;
		mtx[2][1] *= info.mScale.y;
		mtx[2][2] *= info.mScale.z;
	}
	MTXConcat(J3DSys::mCurrentMtx, mtx, J3DSys::mCurrentMtx);
	// TODO: this looks to be a fakematch. Using the getAnmMtx inline does not
	// work.
	J3DModel* model = j3dSys.getModel();
	MTXCopy(J3DSys::mCurrentMtx, model->mNodeMatrices[param_0]);
}

void J3DMtxCalcBasic::calc(u16 param_0)
{
	j3dSys.setCurrentMtxCalc(this);
	calcTransform(param_0, j3dSys.getModel()
	                           ->getModelData()
	                           ->getJointNodePointer(param_0)
	                           ->getTransformInfo());
}

void J3DMtxCalcSoftimage::calcTransform(u16 param_0,
                                        const J3DTransformInfo& info)
{
	Mtx mtx;
	J3DGetTranslateRotateMtx(info.mRotation.x, info.mRotation.y,
	                         info.mRotation.z,
	                         info.mTranslate.x * J3DSys::mCurrentS.x,
	                         info.mTranslate.y * J3DSys::mCurrentS.y,
	                         info.mTranslate.z * J3DSys::mCurrentS.z, mtx);
	MTXConcat(J3DSys::mCurrentMtx, mtx, J3DSys::mCurrentMtx);
	J3DSys::mCurrentS.x = J3DSys::mCurrentS.x * info.mScale.x;
	J3DSys::mCurrentS.y = J3DSys::mCurrentS.y * info.mScale.y;
	J3DSys::mCurrentS.z = J3DSys::mCurrentS.z * info.mScale.z;
	s32 var2;
	if (checkScaleOne(J3DSys::mCurrentS)) {
		j3dSys.getModel()->setScaleFlag(param_0, 1);
		var2 = 1;
	} else {
		j3dSys.getModel()->setScaleFlag(param_0, 0);
		var2 = 0;
	}
	if (!var2) {
		mtx[0][0] = J3DSys::mCurrentMtx[0][0] * J3DSys::mCurrentS.x;
		mtx[0][1] = J3DSys::mCurrentMtx[0][1] * J3DSys::mCurrentS.y;
		mtx[0][2] = J3DSys::mCurrentMtx[0][2] * J3DSys::mCurrentS.z;
		mtx[0][3] = J3DSys::mCurrentMtx[0][3];

		mtx[1][0] = J3DSys::mCurrentMtx[1][0] * J3DSys::mCurrentS.x;
		mtx[1][1] = J3DSys::mCurrentMtx[1][1] * J3DSys::mCurrentS.y;
		mtx[1][2] = J3DSys::mCurrentMtx[1][2] * J3DSys::mCurrentS.z;
		mtx[1][3] = J3DSys::mCurrentMtx[1][3];

		mtx[2][0] = J3DSys::mCurrentMtx[2][0] * J3DSys::mCurrentS.x;
		mtx[2][1] = J3DSys::mCurrentMtx[2][1] * J3DSys::mCurrentS.y;
		mtx[2][2] = J3DSys::mCurrentMtx[2][2] * J3DSys::mCurrentS.z;
		mtx[2][3] = J3DSys::mCurrentMtx[2][3];

		J3DModel* model = j3dSys.getModel();
		MTXCopy(mtx, model->mNodeMatrices[param_0]);
	} else {
		J3DModel* model = j3dSys.getModel();
		MTXCopy(J3DSys::mCurrentMtx, model->mNodeMatrices[param_0]);
	}
}

void J3DMtxCalcMaya::calcTransform(u16 param_1, const J3DTransformInfo& param_2)
{
	J3DModel* model    = j3dSys.getModel();
	u8 scaleCompensate = model->getModelData()
	                         ->getJointNodePointer(param_1)
	                         ->getScaleCompensate();
	s32 tmp;
	if (param_2.mScale.x == 1.0f && param_2.mScale.y == 1.0f
	    && param_2.mScale.z == 1.0f) {
		model->setScaleFlag(param_1, 1);
		tmp = true;
	} else {
		model->setScaleFlag(param_1, 0);
		tmp = false;
	}
	Mtx mtx;
	J3DGetTranslateRotateMtx(param_2, mtx);
	if (tmp == 0) {
		mtx[0][0] *= param_2.mScale.x;
		mtx[0][1] *= param_2.mScale.y;
		mtx[0][2] *= param_2.mScale.z;
		mtx[1][0] *= param_2.mScale.x;
		mtx[1][1] *= param_2.mScale.y;
		mtx[1][2] *= param_2.mScale.z;
		mtx[2][0] *= param_2.mScale.x;
		mtx[2][1] *= param_2.mScale.y;
		mtx[2][2] *= param_2.mScale.z;
	}
	if (scaleCompensate == 1) {
		f32 x = 1.0f / J3DSys::mParentS.x;
		f32 y = 1.0f / J3DSys::mParentS.y;
		f32 z = 1.0f / J3DSys::mParentS.z;
		mtx[0][0] *= x;
		mtx[0][1] *= x;
		mtx[0][2] *= x;
		mtx[1][0] *= y;
		mtx[1][1] *= y;
		mtx[1][2] *= y;
		mtx[2][0] *= z;
		mtx[2][1] *= z;
		mtx[2][2] *= z;
	}
	MTXConcat(J3DSys::mCurrentMtx, mtx, J3DSys::mCurrentMtx);
	model = j3dSys.getModel();
	MTXCopy(J3DSys::mCurrentMtx, model->getAnmMtx(param_1));
	J3DSys::mParentS.x = param_2.mScale.x;
	J3DSys::mParentS.y = param_2.mScale.y;
	J3DSys::mParentS.z = param_2.mScale.z;
}

J3DMtxCalc* J3DNewMtxCalcAnm(u32 type, J3DAnmTransform* transform)
{
	switch (type) {
	case 0:
		return new J3DMtxCalcBasicAnm(transform);
	case 1:
		return new J3DMtxCalcSoftimageAnm(transform);
	case 2:
		return new J3DMtxCalcMayaAnm(transform);
		break;
	default:
		return nullptr;
	}
}

void J3DJoint::initialize()
{

	mJntNo           = 0;
	mKind            = 1;
	mScaleCompensate = 0;
	mTransformInfo   = j3dDefaultTransformInfo;
	mRadius          = 0.0f;
	mMin             = (Vec) { 0.0f, 0.0f, 0.0f };
	mMax             = (Vec) { 0.0f, 0.0f, 0.0f };
	mMtxCalc         = nullptr;
	mOldMtxCalc      = nullptr;
	mMesh            = nullptr;
}

void J3DJoint::addMesh(J3DMaterial* material)
{
	if (!mMesh) {
		mMesh = material;
		return;
	}
	material->setNext(mMesh);
	mMesh = material;
}

void J3DJoint::updateIn() { }

void J3DJoint::updateOut() { }

void J3DJoint::calcIn()
{
	if (mMtxCalc) {
		mOldMtxCalc = j3dSys.getCurrentMtxCalc();
		mMtxCalc->calc(mJntNo);
	} else if (j3dSys.getCurrentMtxCalc()) {
		j3dSys.getCurrentMtxCalc()->calc(mJntNo);
	}
}

void J3DJoint::calcOut()
{
	if (!mOldMtxCalc)
		return;

	j3dSys.setCurrentMtxCalc(mOldMtxCalc);
	mOldMtxCalc = nullptr;
}

void J3DJoint::entryIn()
{
	j3dSys.getDrawBuffer(0)->setZMtx(j3dSys.getModel()->getAnmMtx(mJntNo));
	j3dSys.getDrawBuffer(1)->setZMtx(j3dSys.getModel()->getAnmMtx(mJntNo));

	for (J3DMaterial* mesh = mMesh; mesh != nullptr;) {
		if (mesh->getShape()->checkFlag(1)) {
			mesh = mesh->getNext();
		} else {
			J3DMatPacket* matPacket
			    = j3dSys.getModel()->getMatPacket(mesh->getIndex());
			J3DShapePacket* shapePacket = j3dSys.getModel()->getShapePacket(
			    mesh->getShape()->getIndex());

			if (!matPacket->isLocked()) {
				if (mesh->getMaterialAnm())
					mesh->getMaterialAnm()->calc(mesh);

				mesh->calc(j3dSys.getModel()->getAnmMtx(mJntNo));
				mesh->setCurrentMtx();
			}

			matPacket->setMaterialAnmID(mesh->getMaterialAnm());
			matPacket->setShapePacket(shapePacket);
			J3DDrawBuffer* drawBuffer
			    = j3dSys.getDrawBuffer(!mesh->isDrawModeOpaTexEdge());

			if ((u8)matPacket->entry(drawBuffer)) {
				j3dSys.setMatPacket(matPacket);
				J3DDrawBuffer::entryNum++;
				mesh->makeDisplayList();
			}
			mesh = mesh->getNext();
		}
	}
}
