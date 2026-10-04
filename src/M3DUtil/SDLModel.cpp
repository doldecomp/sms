#include <M3DUtil/SDLModel.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DShape.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <Enemy/Conductor.hpp>
#include <Camera/Camera.hpp>
#include <macros.h>

class SDLDrawBufToken {
public:
	SDLDrawBufToken()
	{
		unk0[0] = nullptr;
		unk0[1] = nullptr;
		mHead   = nullptr;
	}

	void setDrawBufs()
	{
		unk0[0] = j3dSys.getDrawBuffer(0);
		unk0[1] = j3dSys.getDrawBuffer(1);
	}

	bool checkDrawBufs()
	{
		return unk0[0] == j3dSys.getDrawBuffer(0)
		       && unk0[1] == j3dSys.getDrawBuffer(1);
	}

	void push(SDLModel* model)
	{
		model->mNextSameMat = mHead;
		mHead               = model;
	}

	/* 0x0 */ J3DDrawBuffer* unk0[2];
	/* 0x8 */ SDLModel* mHead;
};

void SDLModelData::entrySameMat(J3DMaterial* material, SDLDrawBufToken* token)
{
	SDLModel* head = token->mHead;
	while (head != nullptr) {
		if (head->mSdlFlags & SDLModel::FLAG_UNK1)
			break;
		head = head->mNextSameMat;
	}

	if (head != nullptr) {
		j3dSys.setModel(head);
		j3dSys.setTexture(unk0->getTexture());

		J3DMatPacket* matPacket = head->getMatPacket(material->getIndex());
		matPacket->drawClear();

		J3DShapePacket* shapePacket
		    = head->getShapePacket(material->getShape()->getIndex());
		shapePacket->drawClear();

		matPacket->setShapePacket(shapePacket);

		SDLModel* model = head->mNextSameMat;
		while (model != nullptr) {
			if (model->mSdlFlags & SDLModel::FLAG_UNK1) {
				J3DShapePacket* shapePacket2
				    = model->getShapePacket(material->getShape()->getIndex());
				shapePacket2->drawClear();
				matPacket->addShapePacket(shapePacket2);
			}
			model = model->mNextSameMat;
		}
		token->unk0[material->isDrawModeOpaTexEdge() ? 0 : 1]->entryImm(
		    matPacket, 0);
	}
}

void SDLModelData::entryNode(J3DNode* node, SDLDrawBufToken* token)
{
	J3DJoint* joint       = (J3DJoint*)node;
	J3DMaterial* material = joint->getMesh();
	while (material != nullptr) {
		if (material->getShape()->checkFlag(J3DShpFlag_Visible)) {
			material = material->getNext();
		} else {
			entrySameMat(material, token);
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
    , mDlHost(0)
    , unk18(0)
{
	gpConductor->registerSDLModelData(this);
}

void SDLModelData::registerSDLModel(SDLModel* model)
{
	typedef JGadget::TList<SDLDrawBufToken*>::iterator I;
	for (I it = mDbTokenList.begin(), e = mDbTokenList.end(); it != e; it++) {
		if ((*it)->checkDrawBufs()) {
			(*it)->push(model);
			return;
		}
	}

	SDLDrawBufToken* token = new SDLDrawBufToken;

	token->setDrawBufs();
	token->push(model);

	mDbTokenList.push_back(token);
}

void SDLModelData::entrySDLModels()
{
	if (unk18 & 0x1)
		return;

	typedef JGadget::TList<SDLDrawBufToken*>::iterator I;
	for (I it = mDbTokenList.begin(), e = mDbTokenList.end(); it != e; it++) {
		recursiveEntry(unk0->getRootNode(), *it);

		SDLModel* model = (*it)->mHead;
		while (model != nullptr) {
			model->mSdlFlags &= ~SDLModel::FLAG_UNK1;
			model = model->mNextSameMat;
		}

		(*it)->mHead = nullptr;
	}
}

SDLMatPacket::SDLMatPacket() { }

void SDLMatPacket::beParasiteDL(J3DMatPacket* host)
{
	mpDisplayListObj            = new J3DDisplayListObj;
	mpDisplayListObj->mpData[0] = host->getDisplayListObj()->mpData[0];
	mpDisplayListObj->mpData[1] = host->getDisplayListObj()->mpData[1];
	mpDisplayListObj->mSize     = host->getDisplayListObj()->mSize;
	mpDisplayListObj->mCapacity = host->getDisplayListObj()->mCapacity;
}

void SDLMatPacket::newSingleDL(u32 size)
{
	mpDisplayListObj            = new J3DDisplayListObj;
	mpDisplayListObj->mCapacity = ALIGN_NEXT(size, 0x20);
	mpDisplayListObj->mpData[0] = new (0x20) u8[mpDisplayListObj->mCapacity];
	mpDisplayListObj->mpData[1] = mpDisplayListObj->mpData[0];
	mpDisplayListObj->mSize     = 0;
}

SDLModel::SDLModel(SDLModelData* model_data, u32 flags, u32 mtx_num)
    : mSdlModelData(model_data)
    , mNextSameMat(nullptr)
    , mSdlFlags(0)
{
	initialize();
	entryModelDataSDL(model_data, flags, mtx_num);
}

SDLModel::SDLModel(J3DModelData* model_data, u32 flags)
    : mSdlModelData(nullptr)
    , mNextSameMat(nullptr)
    , mSdlFlags(0)
{
	// NOTE: this is guesswork on dead code
	initialize();
	// BUG: stack use-after-free. Don't call this ;)
	SDLModelData sdlModelData(model_data);
	mSdlModelData = &sdlModelData;
	entryModelDataSDL(mSdlModelData, flags, 1);
}

void SDLModel::entryModelDataSDL(SDLModelData* model_data, u32 flags,
                                 u32 mtx_num)
{
	J3DModelData* md = model_data->unk0;

	mModelData = md;
	if (flags & 1)
		onSdlFlag(FLAG_UNK4);
	if (flags & 2)
		onSdlFlag(FLAG_UNK2);
	if (flags & 4)
		onSdlFlag(FLAG_UNK8);

	if (md->getJointNum()) {
		mScaleFlagArr = new u8[md->getJointNum()];
		if (md->getWEvlpMtxNum())
			mEvlpScaleFlagArr = new u8[md->getWEvlpMtxNum()];
		mNodeMatrices = new Mtx[md->getJointNum()];
	}
	if (md->getWEvlpMtxNum())
		mWeightEvlpMatrices = new Mtx[md->getWEvlpMtxNum()];
	if (mtx_num) {
		for (int i = 0; i < 2; ++i) {
			mDrawMtxBuf[i] = new Mtx*[mtx_num];
			mNrmMtxBuf[i]  = new Mtx33*[mtx_num];
			mBumpMtxArr[i] = nullptr;
		}
	}
	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < mtx_num; ++j) {
			if (md->getDrawMtxNum()) {
				mDrawMtxBuf[i][j] = new (0x20) Mtx[md->getDrawMtxNum()];
				mNrmMtxBuf[i][j]  = new (0x20) Mtx33[md->getDrawMtxNum()];
			}
		}
	}

	if (md->getShapeNum()) {
		mShapePackets = new J3DShapePacket[md->getShapeNum()];
		for (int i = 0; i < md->getShapeNum(); ++i)
			mShapePackets[i].setShape(md->getShapeNodePointer(i));
	}

	if (md->mMaterialNum) {
		if ((flags & 2) && model_data->mDlHost != nullptr) {
			SDLMatPacket* matPackets = new SDLMatPacket[md->mMaterialNum];
			J3DMatPacket* otherMatPackets
			    = model_data->mDlHost->getMatPacket(0);
			for (int i = 0; i < md->mMaterialNum; ++i) {
				matPackets[i].setMaterial(md->getMaterialNodePointer(i));
				matPackets[i].addShapePacket(getShapePacket(
				    md->getMaterialNodePointer(i)->getShape()->getIndex()));
				matPackets[i].setTexture(md->getTexture());
				matPackets[i].beParasiteDL(&otherMatPackets[i]);
			}
			mMatPackets = matPackets;
		} else {
			SDLMatPacket* matPackets = new SDLMatPacket[md->mMaterialNum];
			for (int i = 0; i < md->mMaterialNum; ++i) {
				matPackets[i].setMaterial(md->getMaterialNodePointer(i));
				matPackets[i].addShapePacket(getShapePacket(
				    md->getMaterialNodePointer(i)->getShape()->getIndex()));
				matPackets[i].setTexture(md->getTexture());
				if (flags & 1) {
					matPackets[i].newSingleDL(
					    md->getMaterialNodePointer(i)->countDLSize());
				} else {
					matPackets[i].newDisplayList(
					    md->getMaterialNodePointer(i)->countDLSize());
				}
			}
			mMatPackets = matPackets;
			if (!model_data->mDlHost)
				model_data->mDlHost = this;
		}
	}

	u16 totalBumpMtxs         = 0;
	u16 totalMatsWithBumpMtxs = 0;
	for (int i = 0; i < md->mMaterialNum; ++i) {
		J3DMaterial* mat      = mModelData->getMaterialNodePointer(i);
		J3DNBTScale* nbtScale = mat->getNBTScale();
		if (nbtScale->mbHasScale == 1) {
			totalBumpMtxs += mat->getShape()->countBumpMtxNum();
			++totalMatsWithBumpMtxs;
		}
	}

	if (totalBumpMtxs && mtx_num)
		for (int i = 0; i < 2; ++i)
			mBumpMtxArr[i] = new Mtx33**[totalMatsWithBumpMtxs];

	for (int i = 0; i < 2; ++i) {
		u32 matsWithBumpMtxs = 0;
		for (int j = 0; j < md->mMaterialNum; ++j) {
			J3DMaterial* mat = mModelData->getMaterialNodePointer(j);
			if (mat->getNBTScale()->mbHasScale == 1) {
				mBumpMtxArr[i][matsWithBumpMtxs] = new Mtx33*[mtx_num];
				mat->getShape()->setBumpMtxOffset(matsWithBumpMtxs);
				++matsWithBumpMtxs;
			}
		}
	}

	for (int i = 0; i < 2; ++i) {
		u32 matsWithBumpMtxs = 0;
		for (int j = 0; j < md->mMaterialNum; ++j) {
			J3DMaterial* mat = mModelData->getMaterialNodePointer(j);
			if (mat->getNBTScale()->mbHasScale == 1) {
				for (int k = 0; k < mtx_num; ++k) {
					mBumpMtxArr[i][matsWithBumpMtxs][k]
					    = new (0x20) Mtx33[md->getDrawMtxNum()];
				}
				++matsWithBumpMtxs;
			}
		}
	}

	if (totalMatsWithBumpMtxs)
		mModelData->unk18 = true;

	mVertexBuffer = new J3DVertexBuffer(&md->getVertexData());
}

void SDLModel::entry()
{
	if (!checkSdlFlag(FLAG_UNK8) || !checkSdlFlag(FLAG_UNK2) || !mSdlModelData
	    || (mSdlModelData->unk18 & 0x1)) {
		offSdlFlag(FLAG_UNK1);
		J3DModel::entry();
		return;
	}

	onSdlFlag(FLAG_UNK1);
	mNextSameMat = nullptr;

	mSdlModelData->registerSDLModel(this);
}

void SDLModel::viewCalcSimple()
{
	swapDrawMtx();
	MtxPtr mA = gpCamera->getUnk1EC();
	for (int i = 0; i < mModelData->getDrawMtxNum(); ++i)
		MTXConcat(mA, mNodeMatrices[i], getDrawMtx(i));
	DCStoreRange(getDrawMtxPtr(), mModelData->getDrawMtxNum() * sizeof(Mtx));
}
