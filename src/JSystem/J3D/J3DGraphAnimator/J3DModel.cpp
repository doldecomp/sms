#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>

void J3DModelData::clear() { }

J3DModelData::J3DModelData() { }

J3DModelData::~J3DModelData() { }

void J3DModelData::makeHierarchy(J3DNode*, const J3DModelHierarchy**) { }

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
