#include <M3DUtil/SDLModel.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DShape.hpp>
#include <macros.h>

void SDLModelData::entrySameMat(J3DMaterial* param_1, SDLDrawBufToken* param_2)
{
}

void SDLModelData::entryNode(J3DNode* param_1, SDLDrawBufToken* param_2) { }

void SDLModelData::recursiveEntry(J3DNode* param_1, SDLDrawBufToken* param_2) {
}

SDLModelData::SDLModelData(J3DModelData* model)
    : unk0(model)
    , unk4(0)
    , unk18(0)
{
}

void SDLModelData::entrySDLModels() { }

SDLMatPacket::SDLMatPacket() { }

SDLModel::SDLModel(SDLModelData* param_1, u32 param_2, u32 param_3)
    : unkA0(param_1)
    , unkA4(nullptr)
    , unkA8(0)
{
	initialize();
	entryModelDataSDL(param_1, param_2, param_3);
}

void SDLModel::entryModelDataSDL(SDLModelData* param_1, u32 param_2,
                                 u32 param_3)
{
	J3DModelData* md = param_1->unk0;

	mModelData = md;
	if (param_2 & 1)
		unkA8 |= 0x4;
	if (param_2 & 2)
		unkA8 |= 2;
	if (param_2 & 4)
		unkA8 |= 8;

	if (md->getJointNum()) {
		mScaleFlagArr = new u8[md->getJointNum()];
		if (md->getWEvlpMtxNum())
			mEvlpScaleFlagArr = new u8[md->getWEvlpMtxNum()];
		mNodeMatrices = new Mtx[md->getJointNum()];
	}
	if (md->getWEvlpMtxNum())
		unk5C = new Mtx[md->getWEvlpMtxNum()];
	if (param_3) {
		for (int i = 0; i < 2; ++i) {
			mDrawMtxBuf[i] = new Mtx*[param_3];
			mNrmMtxBuf[i]  = new Mtx33*[param_3];
			mBumpMtxArr[i] = nullptr;
		}
	}
	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < param_3; ++j) {
			u32 mats = md->getDrawMtxData().mEntryNum;
			if (mats) {
				mDrawMtxBuf[i][j] = new (0x20) Mtx[mats];
				mNrmMtxBuf[i][j]  = new (0x20) Mtx33[mats];
			}
		}
	}

	if (md->getShapeNum()) {
		mShapePackets = new J3DShapePacket[md->getShapeNum()];
		for (int i = 0; i < md->getShapeNum(); ++i)
			mShapePackets[i].setShape(md->getShapeNodePointer(i));
	}

	if (md->getMaterialNum()) {
		if ((param_2 & 2) && param_1->unk4 != 0) {
			J3DMatPacket* matPackets = new SDLMatPacket[md->getMaterialNum()];
			J3DMatPacket* otherMatPackets = param_1->unk4->mMatPackets;
			for (int i = 0; i < md->getMaterialNum(); ++i) {
				J3DMatPacket* matPacket = &matPackets[i];
				matPacket->unk38        = md->getMaterialNodePointer(i);
				matPacket->addShapePacket(
				    &mShapePackets[md->getMaterialNodePointer(i)
				                       ->getShape()
				                       ->getIndex()]);
				matPacket->setTexture(md->getTexture());
				matPacket->setDisplayListObj(new J3DDisplayListObj);
				*matPacket->getDisplayListObj()
				    = *otherMatPackets[i].getDisplayListObj();
			}
			mMatPackets = matPackets;
		} else {
			J3DMatPacket* matPackets = new SDLMatPacket[md->getMaterialNum()];
			for (int i = 0; i < md->getMaterialNum(); ++i) {
				J3DMatPacket* matPacket = &matPackets[i];
				matPacket->unk38        = md->getMaterialNodePointer(i);
				matPacket->addShapePacket(
				    &mShapePackets[md->getMaterialNodePointer(i)
				                       ->getShape()
				                       ->getIndex()]);
				matPacket->setTexture(md->getTexture());
				if (param_2 & 1) {
					u32 dlSize = md->getMaterialNodePointer(i)->countDLSize();
					matPacket->setDisplayListObj(new J3DDisplayListObj);
					matPacket->getDisplayListObj()->unkC
					    = ALIGN_NEXT(dlSize, 0x20);
					matPacket->getDisplayListObj()->unk0
					    = new (0x20) u8[matPacket->getDisplayListObj()->unkC];
					matPacket->getDisplayListObj()->unk4
					    = matPacket->getDisplayListObj()->unk0;
					matPacket->getDisplayListObj()->unk8 = 0;
				} else {
					u32 dlSize = md->getMaterialNodePointer(i)->countDLSize();
					matPacket->setDisplayListObj(new J3DDisplayListObj);
					matPacket->getDisplayListObj()->newDisplayList(dlSize);
				}
			}
			mMatPackets = matPackets;
			if (!param_1->unk4)
				param_1->unk4 = this;
		}
	}

	u16 totalBumpMtxs         = 0;
	u16 totalMatsWithBumpMtxs = 0;
	for (int i = 0; i < md->getMaterialNum(); ++i) {
		J3DMaterial* mat      = mModelData->getMaterialNodePointer(i);
		J3DNBTScale* nbtScale = mat->getTexGenBlock()->getNBTScale();
		if (nbtScale->mbHasScale == 1) {
			totalBumpMtxs += mat->getShape()->countBumpMtxNum();
			++totalMatsWithBumpMtxs;
		}
	}

	if (totalBumpMtxs && param_3)
		for (int i = 0; i < 2; ++i)
			mBumpMtxArr[i] = new Mtx33**[totalMatsWithBumpMtxs];

	for (int i = 0; i < 2; ++i) {
		u32 matsWithBumpMtxs = 0;
		for (int j = 0; j < md->getMaterialNum(); ++j) {
			J3DMaterial* mat      = mModelData->getMaterialNodePointer(j);
			J3DNBTScale* nbtScale = mat->getTexGenBlock()->getNBTScale();
			if (nbtScale->mbHasScale == 1) {
				mBumpMtxArr[i][matsWithBumpMtxs] = new Mtx33*[param_3];
				mat->getShape()->unk5C           = matsWithBumpMtxs;
				++matsWithBumpMtxs;
			}
		}
	}

	for (int i = 0; i < 2; ++i) {
		u32 matsWithBumpMtxs = 0;
		for (int j = 0; j < md->getMaterialNum(); ++j) {
			J3DMaterial* mat      = mModelData->getMaterialNodePointer(j);
			J3DNBTScale* nbtScale = mat->getTexGenBlock()->getNBTScale();
			if (nbtScale->mbHasScale == 1) {
				for (int k = 0; k < param_3; ++k) {
					mBumpMtxArr[i][matsWithBumpMtxs][k]
					    = new (0x20) Mtx33[md->getDrawMtxData().mEntryNum];
					++matsWithBumpMtxs;
				}
			}
		}
	}

	if (totalMatsWithBumpMtxs)
		mModelData->unk18 = true;

	mVertexBuffer = new J3DVertexBuffer(&md->getVertexData());
}

void SDLModel::entry() { }

void SDLModel::viewCalcSimple() { }
