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
		if (head->checkSdlFlag(SDLModel::FLAG_UNK1))
			break;
		head = head->mNextSameMat;
	}

	if (head != nullptr) {
		j3dSys.setModel(head);
		j3dSys.setTexture(getModelData()->getTexture());

		J3DMatPacket* matPacket = head->getMatPacket(material->getIndex());
		matPacket->drawClear();

		J3DShapePacket* shapePacket
		    = head->getShapePacket(material->getShape()->getIndex());
		shapePacket->drawClear();

		matPacket->setShapePacket(shapePacket);

		SDLModel* model = head->mNextSameMat;
		while (model != nullptr) {
			if (model->checkSdlFlag(SDLModel::FLAG_UNK1)) {
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
		recursiveEntry(getModelData()->getRootNode(), *it);

		SDLModel* model = (*it)->mHead;
		while (model != nullptr) {
			model->offSdlFlag(SDLModel::FLAG_UNK1);
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

// Binding level worth +16 of low region, landing SDLModel::entry's frame at
// 0xb8 (batch 124).
static inline u32 SDLModelCheckSdlFlag(const SDLModel* p, u32 i)
{
	u32 sdlFlag = p->checkSdlFlag(i);
	return sdlFlag;
}

// TODO: instruction-exact; the frame is now 0xb8 as retail (the two
// SDLModelCheckSdlFlag binding levels below bought it) but the eight
// referenced slots of the inlined registerSDLModel() are still misplaced:
// ours 0x50/0x54/0x58/0x64/0x68/0x74/0x78/0x7c against retail's
// 0x5c/0x60/0x64/0x70/0x74/0x7c/0x80/0x84.
//
// Read as (dead bytes above 0xc)[group sizes], retail is
// [80][3][8][2][4][3][28 above] and we are [68][3][8][2][8][3][36 above]:
// one extra 4-byte temporary in the second gap, 12 bytes missing at the
// bottom and 8 bytes too many above the pool.
//
// Closure batch 129 sharpened this: **spelling the loop `++it` instead of
// `it++` reproduces retail's grouping exactly** -- [60][3][8][2][4][3][32] --
// i.e. the extra temporary in the second gap is post-increment's iterator
// copy, and the residue collapses to a pure translation: the whole pool then
// wants to sit 20 bytes higher with 4 bytes fewer above it (frame 0xa8 vs
// 0xb8). `registerSDLModel`'s own out-of-line copy is unmoved by `++it`
// (frame 0x98, still exact), so the lever is entry-side. Nothing measured
// adds bytes strictly *below* the pool here: the flag binding-level pair is
// (+4 slots, +16 frame), a further nested level does not compile against
// the const receiver, and `registerSDLModel` is emitted and exact so it
// cannot carry a dead local. Still a std-list.hpp research item; `++it`
// is left unapplied because on its own it costs 16 bytes of frame.
//
// Closure batch 212 measured the ladder that pays for `++it`, writing the
// slot map as [bottom dead][3]+g1[2]+g2[3]+top and the target as
// [84][3]+8[2]+4[3]+28 at frame 0xb8. With `++it` alone we are [64]...[2]+4
// [3]+32 at 0xa8. A TU-local `SDLModelGetData(p)` binder over the
// `mSdlModelData` member read is +8 bottom / +0 top / +8 frame per site, so
// two sites (the `unk18` read and the `registerSDLModel` call) land [80] at
// 0xb8; nesting a direct-return fork inside that binder at the `unk18` site
// adds the last +4 bottom and lands [84][3]+8[2]+4 exactly, but also +4 top,
// so the frame overshoots to 0xc0 (five markers, pool byte-exact).
// Turning `SDLModelCheckSdlFlag` into a direct-return fork instead pays the
// 8 back (frame 0xb8, ten markers) but re-opens the second gap to 8: the
// second gap is 4 only while both flag sites go through a *binding* level.
// So the residue is now one word that must move from the top region to the
// second gap at constant frame, and none of these levers does that: the
// binder rungs are all +4 bottom *and* +4 top. Inert here: naming the
// binder's result in the caller, a fork nested in the flag binder, the
// binder at one or three sites. Left unapplied (all of it is fabricated
// machinery for +0.1% with the function still nonmatching).
//
// Research batch 133 settled which word that is: respelling
// `TList::iterator::operator++(int)` so that it builds its result straight
// from the node (`TNode_* p = p_; p_ = p_->pNext_; return iterator(p);`), with
// no `iterator copy` local at all, is byte-identical to the stock body and the
// extra word survives -- it is post-increment's **by-value return slot**, not
// its copy. Retail has no such word, so retail's list walks are `++it`; the
// spelling stays `it++` here only because the frame it leaves is exact and
// this function is nonmatching either way. The leftover "+20 low / -4 high,
// +16 frame" translation is the same signature that an *implicit*
// derived-from-base conversion on a `return` produces (std-list.hpp's
// `TList_pointer<T>::insert`, closed in the same batch), but nothing on the
// entry side of this function supplies it.
//
// TMirrorActor::init has the same shape (frame exact at 0xd8, first pair 16
// low, second pair 8 low, the two named locals and the argument word in place),
// which is why header round 15 looked for one shared cause in std-list.hpp.
// Rejected there, each measured on this function's frame/instruction count
// (target 0xb8/96):
//   an explicit TList::iterator copy constructor            0xb0 /  83
//   push_back as `iterator it = end(); insert(it, what);`   0xb0 / 100
//   insert taking `const iterator& where`                   0xa0 /  94
//   operator==/!= taking `const iterator&`                  0x98 /  85
//   `iterator copy(*this); ++(*this);` in operator++(int)   0xa8 /  96 (identical)
// Also rejected earlier: getSDLModelData() at one or all three sites (+0), a
// named `*it` in registerSDLModel (97 instructions), push_back spelled as
// insert(end(), token) (+0), and hoisting `e` out of the for-init (98
// instructions).
void SDLModel::entry()
{
	if (!SDLModelCheckSdlFlag(this, FLAG_UNK8)
	    || !SDLModelCheckSdlFlag(this, FLAG_UNK2) || !mSdlModelData
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
	for (int i = 0; i < getModelData()->getDrawMtxNum(); ++i)
		MTXConcat(mA, mNodeMatrices[i], getDrawMtx(i));
	DCStoreRange(getDrawMtxPtr(), getModelData()->getDrawMtxNum() * sizeof(Mtx));
}
