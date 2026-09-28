#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTransform.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DDrawBuffer.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DShape.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DMaterialAnm.hpp>

void J3DMtxCalcAnm::calc(u16 jntNo)
{
	j3dSys.setCurrentMtxCalc(this);
	J3DTransformInfo info;
	if (mOne[0]) {
		mOne[0]->getTransform(jntNo, &info);
	} else {
		info = j3dSys.getModel()
		           ->getModelData()
		           ->getJointNodePointer(jntNo)
		           ->getTransformInfo();
	}
	calcTransform(jntNo, info);
}

J3DMtxCalcBasic::J3DMtxCalcBasic() { }

void J3DMtxCalcBasic::recursiveUpdate(J3DNode* node)
{
	if (!node)
		return;

	J3DMtxCalcBasic mtxCalc;
	char trash[0x8]; // TODO:

	MTXCopy(J3DSys::mCurrentMtx, mtxCalc.getBackupMtx());
	mtxCalc.setBackupS(J3DSys::mCurrentS);
	mtxCalc.setBackupParentS(J3DSys::mParentS);
	node->updateIn();

	if (node->getCallBack())
		node->getCallBack()(node, 0);

	recursiveUpdate(node->getChild());
	MTXCopy(mtxCalc.getBackupMtx(), J3DSys::mCurrentMtx);
	J3DSys::mCurrentS = mtxCalc.getBackupS();
	J3DSys::mParentS  = mtxCalc.getBackupParentS();
	node->updateOut();

	if (node->getCallBack())
		node->getCallBack()(node, 1);

	recursiveUpdate(node->getYounger());
}

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

void J3DMtxCalcBasic::recursiveEntry(J3DNode* node)
{
	if (node) {
		node->entryIn();
		recursiveEntry(node->getChild());
		recursiveEntry(node->getYounger());
	}
}

void J3DMtxCalcBasic::calcTransform(u16 jntNo, const J3DTransformInfo& info)
{
	J3DSys::mCurrentS.x *= info.mScale.x;
	J3DSys::mCurrentS.y *= info.mScale.y;
	J3DSys::mCurrentS.z *= info.mScale.z;
	BOOL noScale;
	if (checkScaleOne(J3DSys::mCurrentS)) {
		j3dSys.getModel()->setScaleFlag(jntNo, 1);
		noScale = TRUE;
	} else {
		j3dSys.getModel()->setScaleFlag(jntNo, 0);
		noScale = FALSE;
	}
	Mtx mtx;
	J3DGetTranslateRotateMtx(info, mtx);
	if (!noScale) {
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
	j3dSys.getModel()->setAnmMtx(jntNo, J3DSys::mCurrentMtx);
}

void J3DMtxCalcBasic::calc(u16 param_0)
{
	j3dSys.setCurrentMtxCalc(this);
	calcTransform(param_0, j3dSys.getModel()
	                           ->getModelData()
	                           ->getJointNodePointer(param_0)
	                           ->getTransformInfo());
}

void J3DMtxCalcSoftimage::calcTransform(u16 jntNo, const J3DTransformInfo& info)
{
	Mtx mtx;
	J3DGetTranslateRotateMtx(info.mRotation.x, info.mRotation.y,
	                         info.mRotation.z,
	                         info.mTranslate.x * J3DSys::mCurrentS.x,
	                         info.mTranslate.y * J3DSys::mCurrentS.y,
	                         info.mTranslate.z * J3DSys::mCurrentS.z, mtx);
	MTXConcat(J3DSys::mCurrentMtx, mtx, J3DSys::mCurrentMtx);

	J3DSys::mCurrentS.x *= info.mScale.x;
	J3DSys::mCurrentS.y *= info.mScale.y;
	J3DSys::mCurrentS.z *= info.mScale.z;

	BOOL b;
	if (checkScaleOne(J3DSys::mCurrentS)) {
		j3dSys.getModel()->setScaleFlag(jntNo, 1);
		b = true;
	} else {
		j3dSys.getModel()->setScaleFlag(jntNo, 0);
		b = false;
	}

	if (!b) {
		f32* p = mtx[0];
		f32* q = J3DSys::mCurrentMtx[0];
		*p++   = *q++ * J3DSys::mCurrentS.x;
		*p++   = *q++ * J3DSys::mCurrentS.y;
		*p++   = *q++ * J3DSys::mCurrentS.z;
		*p++   = *q++;
		*p++   = *q++ * J3DSys::mCurrentS.x;
		*p++   = *q++ * J3DSys::mCurrentS.y;
		*p++   = *q++ * J3DSys::mCurrentS.z;
		*p++   = *q++;
		*p++   = *q++ * J3DSys::mCurrentS.x;
		*p++   = *q++ * J3DSys::mCurrentS.y;
		*p++   = *q++ * J3DSys::mCurrentS.z;
		*p++   = *q++;

		j3dSys.getModel()->setAnmMtx(jntNo, mtx);
	} else {
		j3dSys.getModel()->setAnmMtx(jntNo, J3DSys::mCurrentMtx);
	}
}

void J3DMtxCalcMaya::calcTransform(u16 jntNo, const J3DTransformInfo& param_2)
{
	Mtx mtx;
	u8 scaleCompensate = j3dSys.getModel()
	                         ->getModelData()
	                         ->getJointNodePointer(jntNo)
	                         ->getScaleCompensate();
	BOOL b;
	if (param_2.mScale.x == 1.0f && param_2.mScale.y == 1.0f
	    && param_2.mScale.z == 1.0f) {
		j3dSys.getModel()->setScaleFlag(jntNo, 1);
		b = TRUE;
	} else {
		j3dSys.getModel()->setScaleFlag(jntNo, 0);
		b = FALSE;
	}
	J3DGetTranslateRotateMtx(param_2, mtx);
	if (b == 0) {
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
		Vec inv;
		inv.x = 1.0f / J3DSys::mParentS.x;
		inv.y = 1.0f / J3DSys::mParentS.y;
		inv.z = 1.0f / J3DSys::mParentS.z;

		f32* p = mtx[0];
		*p++ *= inv.x;
		*p++ *= inv.x;
		*p++ *= inv.x;
		p++;
		*p++ *= inv.y;
		*p++ *= inv.y;
		*p++ *= inv.y;
		p++;
		*p++ *= inv.z;
		*p++ *= inv.z;
		*p++ *= inv.z;
	}
	MTXConcat(J3DSys::mCurrentMtx, mtx, J3DSys::mCurrentMtx);
	j3dSys.getModel()->setAnmMtx(jntNo, J3DSys::mCurrentMtx);
	J3DSys::mParentS.x = param_2.mScale.x;
	J3DSys::mParentS.y = param_2.mScale.y;
	J3DSys::mParentS.z = param_2.mScale.z;
}

J3DMtxCalc* J3DNewMtxCalcAnm(u32 type, J3DAnmTransform* transform)
{
	switch (type) {
	case J3DMLF_MtxCalcBasic:
		return new J3DMtxCalcBasicAnm(transform);
	case J3DMLF_MtxCalcSoftImage:
		return new J3DMtxCalcSoftimageAnm(transform);
	case J3DMLF_MtxCalcMaya:
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
	mMtxCalcOverride = nullptr;
	mOldMtxCalc      = nullptr;
	mMesh            = nullptr;
}

J3DJoint::J3DJoint(u16, const J3DTransformInfo&)
{
	// UNUSED
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

void J3DJoint::updateIn()
{
	if (mMtxCalcOverride) {
		mOldMtxCalc = j3dSys.getCurrentMtxCalc();
		mMtxCalcOverride->calc(mJntNo);
	} else if (j3dSys.getCurrentMtxCalc()) {
		j3dSys.getCurrentMtxCalc()->calc(mJntNo);
	}

	j3dSys.getDrawBuffer(0)->setZMtx(j3dSys.getModel()->getAnmMtx(getJntNo()));
	j3dSys.getDrawBuffer(1)->setZMtx(j3dSys.getModel()->getAnmMtx(getJntNo()));

	for (J3DMaterial* mesh = mMesh; mesh != nullptr;) {
		if (mesh->getShape()->checkFlag(J3DShpFlag_Visible)) {
			mesh = mesh->getNext();
		} else {
			J3DMatPacket* matPacket
			    = j3dSys.getModel()->getMatPacket(mesh->getIndex());
			J3DShapePacket* shapePacket = j3dSys.getModel()->getShapePacket(
			    mesh->getShape()->getIndex());

			if (mesh->getMaterialAnm())
				mesh->getMaterialAnm()->calc(mesh);

			mesh->calc(j3dSys.getModel()->getAnmMtx(mJntNo));
			mesh->setCurrentMtx();

			matPacket->setMaterialAnmID(mesh->getMaterialAnm());
			matPacket->setShapePacket(shapePacket);
			J3DDrawBuffer* drawBuffer
			    = j3dSys.getDrawBuffer(!mesh->isDrawModeOpaTexEdge());

			if (matPacket->entry(drawBuffer)) {
				j3dSys.setMatPacket(matPacket);
				if (!mesh->getSomeFlag())
					mesh->makeDisplayList();
				else
					mesh->safeMakeDisplayList();
			}
			mesh = mesh->getNext();
		}
	}
}

void J3DJoint::updateOut()
{
	if (!mOldMtxCalc)
		return;

	j3dSys.setCurrentMtxCalc(mOldMtxCalc);
	mOldMtxCalc = nullptr;
}

void J3DJoint::calcIn()
{
	if (mMtxCalcOverride) {
		mOldMtxCalc = j3dSys.getCurrentMtxCalc();
		mMtxCalcOverride->calc(mJntNo);
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
	j3dSys.getDrawBuffer(0)->setZMtx(j3dSys.getModel()->getAnmMtx(getJntNo()));
	j3dSys.getDrawBuffer(1)->setZMtx(j3dSys.getModel()->getAnmMtx(getJntNo()));

	for (J3DMaterial* mesh = mMesh; mesh != nullptr;) {
		if (mesh->getShape()->checkFlag(J3DShpFlag_Visible)) {
			mesh = mesh->getNext();
		} else {
			J3DMatPacket* matPacket
			    = j3dSys.getModel()->getMatPacket(mesh->getIndex());
			J3DShapePacket* shapePacket = j3dSys.getModel()->getShapePacket(
			    mesh->getShape()->getIndex());

			if (!matPacket->isLocked()) {
				if (mesh->getMaterialAnm())
					mesh->getMaterialAnm()->calc(mesh);

				MtxPtr anmMtx = j3dSys.getModel()->getAnmMtx(mJntNo);
				mesh->calc(anmMtx);
				mesh->setCurrentMtx();
			}

			matPacket->setMaterialAnmID(mesh->getMaterialAnm());
			matPacket->setShapePacket(shapePacket);

			bool b = !mesh->isDrawModeOpaTexEdge();
			bool r = matPacket->entry(j3dSys.getDrawBuffer(b));
			if (r) {
				j3dSys.setMatPacket(matPacket);
				J3DDrawBuffer::entryNum++;
				mesh->makeDisplayList();
			}
			mesh = mesh->getNext();
		}
	}
}
