#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTransform.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DDrawBuffer.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DShape.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DMaterialAnm.hpp>

void J3DMtxCalcAnm::calc(u16 param_0) { }

J3DMtxCalcBasic::J3DMtxCalcBasic() { }

void J3DMtxCalcBasic::recursiveUpdate(J3DNode*) { }

void J3DMtxCalcBasic::recursiveCalc(J3DNode*) { }

void J3DMtxCalcBasic::recursiveEntry(J3DNode*) { }

void J3DMtxCalcBasic::calcTransform(u16, const J3DTransformInfo&) { }

void J3DMtxCalcBasic::calc(u16) { }

void J3DMtxCalcSoftimage::calcTransform(u16, const J3DTransformInfo&) { }

void J3DMtxCalcMaya::calcTransform(u16, const J3DTransformInfo&) { }

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
