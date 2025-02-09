#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTransform.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>

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

void J3DJoint::initialize() { }

void J3DJoint::addMesh(J3DMaterial*) { }

void J3DJoint::updateIn() { }

void J3DJoint::updateOut() { }

void J3DJoint::calcIn() { }

void J3DJoint::calcOut() { }

void J3DJoint::entryIn() { }
