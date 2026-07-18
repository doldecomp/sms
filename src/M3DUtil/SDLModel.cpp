#include <M3DUtil/SDLModel.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DShape.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <Enemy/Conductor.hpp>
#include <Camera/Camera.hpp>
#include <macros.h>

void SDLModelData::entrySameMat(J3DMaterial* param_1, SDLDrawBufToken* param_2)
{
	SDLModel* model = param_2->unk8;
	while (model != nullptr) {
		if (model->unkA8 & 0x1)
			break;
		model = model->unkA4;
	}

	if (model != nullptr) {
		j3dSys.setModel(model);
		j3dSys.setTexture(unk0->getTexture());

		SDLMatPacket* matPacket
		    = (SDLMatPacket*)model->getMatPacket(param_1->getIndex());
		matPacket->drawClear();

		J3DShapePacket* shapePacket
		    = model->getShapePacket(param_1->getShape()->getIndex());
		shapePacket->drawClear();

		matPacket->setShapePacket(shapePacket);

		SDLModel* model2 = model->unkA4;
		while (model2 != nullptr) {
			if (model->unkA8 & 1) {
				J3DShapePacket* shapePacket2
				    = model2->getShapePacket(param_1->getShape()->getIndex());
				shapePacket2->drawClear();
				matPacket->addShapePacket(shapePacket2);
			}
			model2 = model2->unkA4;
		}
		param_2->unk0[param_1->isDrawModeOpaTexEdge() ? 0 : 1]->entryImm(
		    matPacket, 0);
	}
}

void SDLModelData::entryNode(J3DNode* param_1, SDLDrawBufToken* param_2)
{
	J3DJoint* joint       = (J3DJoint*)param_1;
	J3DMaterial* material = joint->getMesh();
	while (material != nullptr) {
		if (material->getShape()->checkFlag(0x1)) {
			material = material->getNext();
		} else {
			entrySameMat(material, param_2);
			material = material->getNext();
		}
	}
}

void SDLModelData::recursiveEntry(J3DNode* param_1, SDLDrawBufToken* param_2)
{
	if (param_1) {
		entryNode(param_1, param_2);
		recursiveEntry(param_1->getChild(), param_2);
		recursiveEntry(param_1->getYounger(), param_2);
	}
}

SDLModelData::SDLModelData(J3DModelData* model)
    : unk0(model)
    , unk4(0)
    , unk18(0)
{
	gpConductor->registerSDLModelData(this);
}

void SDLModelData::registerSDLModel(SDLModel*) { }

void SDLModelData::entrySDLModels()
{
	if (unk18 & 0x1)
		return;

	typedef JGadget::TList<SDLDrawBufToken*>::iterator I;
	for (I it = unk8.begin(), e = unk8.end(); it != e; ++it) {
		recursiveEntry(unk0->getRootNode(), *it);

		SDLModel* model = (*it)->unk8;
		while (model != nullptr) {
			model->unkA8 &= ~0x1;
			model = model->unkA4;
		}

		(*it)->unk8 = nullptr;
	}
}

SDLMatPacket::SDLMatPacket() { }

void SDLMatPacket::beParasiteDL(J3DMatPacket*) { }

void SDLMatPacket::newSingleDL(u32) { }

SDLModel::SDLModel(SDLModelData* param_1, u32 param_2, u32 param_3)
    : unkA0(param_1)
    , unkA4(nullptr)
    , unkA8(0)
{
	initialize();
	entryModelDataSDL(param_1, param_2, param_3);
}

SDLModel::SDLModel(J3DModelData*, u32) { }

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

void SDLModel::entry()
{
	if (!(unkA8 & 0x8) || !(unkA8 & 2) || !unkA0 || (unkA0->unk18 & 0x1)) {
		unkA8 &= ~0x1;
		J3DModel::entry();
		return;
	}

	unkA8 |= 0x1;
	unkA4 = nullptr;

	typedef JGadget::TList<SDLDrawBufToken*>::iterator I;
	for (I it = unkA0->unk8.begin(); it != unkA0->unk8.end(); ++it) {
		SDLDrawBufToken* token = *it;

		bool found = token->unk0[0] == j3dSys.getDrawBuffer(0)
		             && token->unk0[1] == j3dSys.getDrawBuffer(1);

		if (found) {
			unkA4       = token->unk8;
			token->unk8 = this;
			return;
		}
	}

	SDLDrawBufToken* token = new SDLDrawBufToken;

	token->unk0[0] = j3dSys.getDrawBuffer(0);
	token->unk0[1] = j3dSys.getDrawBuffer(1);
	token->unk8    = this;

	unkA0->unk8.push_back(token);
}

void SDLModel::viewCalcSimple()
{
	swapDrawMtx();
	MtxPtr mA = gpCamera->getUnk1EC();
	for (int i = 0; i < mModelData->getDrawMtxNum(); ++i)
		MTXConcat(mA, mNodeMatrices[i], getDrawMtx(i));
	DCStoreRange(getDrawMtxPtr(), mModelData->getDrawMtxNum() * sizeof(Mtx));
}
