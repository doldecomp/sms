#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>

void J3DModelData::clear()
{
	unk4              = 0;
	unk8              = 0;
	unkC              = 0;
	unk10             = 0;
	unk14             = 0;
	unk18             = 0;
	unk1A             = 0;
	unk1C             = 0;
	mJointNodePointer = (J3DJoint**)0x0;
	unk24             = 0;
	mMaterials        = (J3DMaterial**)0x0;
	unk2C             = 0;
	unk30             = 0;
	unk34             = 0;
	unk38             = 0;
	unkA8             = nullptr;
	unkAC             = nullptr;
	unkA4             = 0;
	unk84             = 0;
	unk88             = 0;
	unk8C             = 0;
	unk90             = 0;
	unk94             = 0;
	unkB0             = 0;
	mMaterialName     = nullptr;
	unkB8             = 0;
	unk80             = 0;
}

J3DModelData::J3DModelData() { clear(); }

J3DModelData::~J3DModelData() { }

void J3DModelData::makeHierarchy(J3DNode* joint,
                                 const J3DModelHierarchy** hierarchy)
{
}

void J3DModelData::isDeformableVertexFormat() const { }

void J3DModelData::setMaterialTable(J3DMaterialTable*, J3DMaterialCopyFlag) { }

void J3DModelData::entryMatColorAnimator(J3DAnmColor*) { }

void J3DModelData::entryTexMtxAnimator(J3DAnmTextureSRTKey*) { }

void J3DModelData::entryTevRegAnimator(J3DAnmTevRegKey*) { }

void J3DModelData::removeMatColorAnimator(J3DAnmColor*) { }

void J3DModelData::removeTexNoAnimator(J3DAnmTexPattern*) { }

void J3DModelData::removeTexMtxAnimator(J3DAnmTextureSRTKey*) { }

void J3DModelData::removeTevRegAnimator(J3DAnmTevRegKey*) { }

void J3DModelData::setMatColorAnimator(J3DAnmColor*, J3DMatColorAnm*) { }

void J3DModelData::setTexNoAnimator(J3DAnmTexPattern*, J3DTexNoAnm*) { }

void J3DModelData::setTexMtxAnimator(J3DAnmTextureSRTKey*, J3DTexMtxAnm*,
                                     J3DTexMtxAnm*)
{
}

void J3DModelData::setTevRegAnimator(J3DAnmTevRegKey*, J3DTevColorAnm*,
                                     J3DTevKColorAnm*)
{
}

J3DModel::J3DModel() { }

J3DModel::J3DModel(J3DModelData*, u32, u32) { }

J3DModel::~J3DModel() { }

void J3DModel::initialize() { }

void J3DModel::entryModelData(J3DModelData*, u32, u32) { }

void J3DModel::lock() { }

void J3DModel::unlock() { }

void J3DModel::makeDL() { }

void J3DModel::setSkinDeform(J3DSkinDeform*, J3DDeformAttachFlag) { }

void J3DModel::calcWeightEnvelopeMtx() { }

void J3DModel::update() { }

void J3DModel::calc() { }

void J3DModel::entry() { }

void J3DModel::viewCalc() { }

void J3DModel::calcNrmMtx() { }

void J3DModel::calcBumpMtx() { }

void J3DModel::calcBBoard() { }

void J3DModel::prepareShapePackets() { }
