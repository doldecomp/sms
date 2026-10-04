#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DShape.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTexture.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DMaterialAnm.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DMaterialAnm.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DCluster.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <dolphin/os/OSCache.h>
#include <math.h>

void J3DModelData::clear()
{
	unk4              = 0;
	unk8              = 0;
	unkC              = 0;
	mRootNode         = 0;
	unk14             = 0;
	unk18             = 0;
	mbHasBillboard    = 0;
	mJointNum         = 0;
	mJointNodePointer = (J3DJoint**)0x0;
	mMaterialNum      = 0;
	mMaterials        = (J3DMaterial**)0x0;
	mShapeNum         = 0;
	mShapeNodePointer = nullptr;
	unk34             = 0;
	unk38             = 0;
	unkA8             = nullptr;
	unkAC             = nullptr;
	unkA4             = 0;
	mWEvlpMtxNum      = 0;
	unk88             = nullptr;
	unk8C             = nullptr;
	unk90             = nullptr;
	unk94             = nullptr;
	unkB0             = nullptr;
	mMaterialName     = nullptr;
	unkB8             = nullptr;
	unk80             = 0;
}

J3DModelData::J3DModelData() { clear(); }

J3DModelData::~J3DModelData() { }

// NOTE: routing exactly one of the two pointer setters below through its
// accessor is what makes the stack frame match. Spelling getDrawMtxData()
// that way instead of getVertexData() matches equally well, so which of the
// two the original went through is not recoverable.
void J3DModelData::makeHierarchy(J3DNode* root_node,
                                 const J3DModelHierarchy** hierarchy)
{
	enum {
		kTypeEnd        = 0x00,
		kTypeBeginChild = 0x01,
		kTypeEndChild   = 0x02,
		kTypeJoint      = 0x10,
		kTypeMaterial   = 0x11,
		kTypeShape      = 0x12,
	};

	J3DNode* curNode = root_node;

	while (true) {
		J3DJoint* newJoint       = nullptr;
		J3DMaterial* newMaterial = nullptr;
		J3DShape* newShape       = nullptr;

		switch ((*hierarchy)->mType) {
		case kTypeBeginChild:
			++*hierarchy;
			makeHierarchy(curNode, hierarchy);
			break;
		case kTypeEndChild:
			++*hierarchy;
			return;
		case kTypeEnd:
			return;
		case kTypeJoint:
			newJoint = mJointNodePointer[((*hierarchy)++)->mValue];
			break;
		case kTypeMaterial:
			newMaterial = mMaterials[((*hierarchy)++)->mValue];
			break;
		case kTypeShape:
			newShape = mShapeNodePointer[((*hierarchy)++)->mValue];
			break;
		}

		if (newJoint != nullptr) {
			curNode = newJoint;
			if (root_node == nullptr)
				mRootNode = newJoint;
			else
				root_node->appendChild(newJoint);
		} else if (newMaterial != nullptr && root_node->getType() == 'NJNT') {
			((J3DJoint*)root_node)->addMesh(newMaterial);
		} else if (newShape != nullptr && root_node->getType() == 'NJNT') {
			((J3DJoint*)root_node)->getMesh()->addShape(newShape);
			newShape->setDrawMtxDataPointer(&mDrawMtxData);
			newShape->setVertexDataPointer(&getVertexData());
			newShape->makeVcdVatCmd();
		}
	}
}

bool J3DModelData::isDeformableVertexFormat() const
{
	const GXVtxAttrFmtList* vtxAttrFmtList;
	bool bVar1 = false;
	for (vtxAttrFmtList = mVertexData.getVtxAttrFmtList();
	     vtxAttrFmtList->attr != GX_VA_NULL; ++vtxAttrFmtList) {
		switch (vtxAttrFmtList->attr) {
		case GX_VA_POS:
			if (vtxAttrFmtList->type != GX_F32
			    || vtxAttrFmtList->cnt != GX_CLR_RGBA)
				return false;
			break;
		case GX_VA_NRM:
			bVar1 = true;
			if (vtxAttrFmtList->type != GX_F32)
				return false;
			break;
		default:
			break;
		}
	}

	if (bVar1)
		return true;

	return false;
}

void J3DModelData::setMaterialTable(J3DMaterialTable* mat_table,
                                    J3DMaterialCopyFlag flag)
{
	if (flag & J3DMatCopyFlag_Material) {
		for (u16 i = 0; i < mat_table->getMaterialNum(); i++) {
			s32 nameIndex = getMaterialName()->getIndex(
			    mat_table->getMaterialName()->getName(i));
			if (nameIndex != -1) {
				J3DMaterial* src = mat_table->getMaterialNodePointer(i);
				getMaterialNodePointer(nameIndex)->copy(src);
			}
		}
	}

	if (flag & J3DMatCopyFlag_Texture) {
		J3DTexture* texture = mat_table->getTexture();
		if (texture->getNum() != 0) {
			setTexture(texture);
			setTextureName(mat_table->getTextureName());
		}
	}
}

int J3DModelData::entryMatColorAnimator(J3DAnmColor* anm)
{
	int ret         = 0;
	u16 materialNum = anm->getUpdateMaterialNum();

	for (u16 i = 0; i < materialNum; i++) {
		u16 materialID = anm->getUpdateMaterialID(i);
		if (materialID != 0xFFFF) {
			J3DMaterial* mat        = getMaterialNodePointer(materialID);
			J3DMaterialAnm* pMatAnm = mat->getMaterialAnm();
			if (pMatAnm == nullptr) {
				ret = 1;
			} else {
				J3DMatColorAnm* matColorAnm = new J3DMatColorAnm(anm, i);
				pMatAnm->setMatColorAnm(0, matColorAnm);
			}
		}
	}

	return ret;
}

int J3DModelData::entryTexMtxAnimator(J3DAnmTextureSRTKey* anm)
{
	s32 ret         = 0;
	u16 materialNum = anm->getUpdateMaterialNum();

	for (u16 i = 0; i < materialNum; i++) {
		if (anm->isValidUpdateMaterialID(i)) {
			u16 materialID         = anm->getUpdateMaterialID(i);
			J3DMaterial* material  = getMaterialNodePointer(materialID);
			J3DMaterialAnm* matAnm = material->getMaterialAnm();
			u16 texMtxID           = anm->getUpdateTexMtxID(i);

			if (matAnm == nullptr) {
				ret = 1;
				continue;
			}

			if (texMtxID == 0xFF)
				continue;

			if (material->getTexMtx(texMtxID) == nullptr) {
				J3DTexMtx* texMtx = new J3DTexMtx;
				material->setTexMtx(texMtxID, texMtx);
				material->getTexCoord(texMtxID)->setTexGenMtx(
				    (GXTexMtx)(GX_TEXMTX0 + texMtxID * 3));
			}

			J3DTexMtxInfo& texMtxInfo
			    = material->getTexMtx(texMtxID)->getTexMtxInfo();
			J3DTexMtxAnm* pTexMtxAnm = new J3DTexMtxAnm(anm, i);

			texMtxInfo.mInfo
			    = (texMtxInfo.mInfo & 0x7F) | (anm->getTexMtxCalcType() << 7);

			texMtxInfo.mCenter.x = anm->getSRTCenter(i)->x;
			texMtxInfo.mCenter.y = anm->getSRTCenter(i)->y;
			texMtxInfo.mCenter.z = anm->getSRTCenter(i)->z;

			matAnm->setTexMtxAnm(texMtxID, pTexMtxAnm);
		}
	}

	return ret;
}

int J3DModelData::entryTevRegAnimator(J3DAnmTevRegKey* anm)
{
	int ret             = 0;
	u16 cRegMaterialNum = anm->getCRegUpdateMaterialNum();
	u16 kRegMaterialNum = anm->getKRegUpdateMaterialNum();

	for (u16 i = 0; i < cRegMaterialNum; i++) {
		if (anm->isValidCRegUpdateMaterialID(i)) {
			u16 materialID = anm->getCRegUpdateMaterialID(i);
			J3DMaterialAnm* pMatAnm
			    = getMaterialNodePointer(materialID)->getMaterialAnm();
			const J3DAnmCRegKeyTable* table = anm->getAnmCRegKeyTable();
			u8 colorId                      = table[i].mColorId;
			if (pMatAnm == nullptr)
				ret = 1;
			else
				pMatAnm->setTevColorAnm(colorId, new J3DTevColorAnm(anm, i));
		}
	}

	for (u16 i = 0; i < kRegMaterialNum; i++) {
		if (anm->isValidKRegUpdateMaterialID(i)) {
			u16 materialID = anm->getKRegUpdateMaterialID(i);
			J3DMaterialAnm* matAnm
			    = getMaterialNodePointer(materialID)->getMaterialAnm();
			const J3DAnmKRegKeyTable* table = anm->getAnmKRegKeyTable();
			u8 colorId                      = table[i].mColorId;
			if (matAnm == nullptr)
				ret = 1;
			else
				matAnm->setTevKColorAnm(colorId, new J3DTevKColorAnm(anm, i));
		}
	}

	return ret;
}

int J3DModelData::removeMatColorAnimator(J3DAnmColor* anm)
{
	int ret         = 0;
	u16 materialNum = anm->getUpdateMaterialNum();

	for (u16 i = 0; i < materialNum; i++) {
		u16 materialID = anm->getUpdateMaterialID(i);
		if (materialID != 0xFFFF) {
			J3DMaterialAnm* matAnm
			    = getMaterialNodePointer(materialID)->getMaterialAnm();
			if (matAnm == nullptr)
				ret = 1;
			else
				matAnm->setMatColorAnm(0, nullptr);
		}
	}

	return ret;
}

int J3DModelData::removeTexNoAnimator(J3DAnmTexPattern* anm)
{
	int ret         = 0;
	u16 materialNum = anm->getUpdateMaterialNum();

	for (u16 i = 0; i < materialNum; i++) {
		u16 materialID = anm->getUpdateMaterialID(i);
		if (materialID != 0xFFFF) {
			J3DMaterialAnm* matAnm
			    = getMaterialNodePointer(materialID)->getMaterialAnm();
			u8 texNo = anm->getAnmTable()[i].mTexNo;
			if (matAnm == nullptr)
				ret = 1;
			else
				matAnm->setTexNoAnm(texNo, nullptr);
		}
	}

	return ret;
}

int J3DModelData::removeTexMtxAnimator(J3DAnmTextureSRTKey* anm)
{
	int ret         = 0;
	u16 materialNum = anm->getUpdateMaterialNum();

	for (u16 i = 0; i < materialNum; i++) {
		u16 materialID         = anm->getUpdateMaterialID(i);
		J3DMaterial* pMaterial = getMaterialNodePointer(materialID);
		J3DMaterialAnm* matAnm = pMaterial->getMaterialAnm();
		u32 texMtxID           = anm->getUpdateTexMtxID(i);
		if (matAnm == nullptr) {
			ret = 1;
		} else if (texMtxID != 0xFF) {
			matAnm->setTexMtxAnm(texMtxID, nullptr);
		}
	}

	return ret;
}

int J3DModelData::removeTevRegAnimator(J3DAnmTevRegKey* anm)
{
	int ret             = 0;
	u16 cRegMaterialNum = anm->getCRegUpdateMaterialNum();
	u16 kRegMaterialNum = anm->getKRegUpdateMaterialNum();

	for (u16 i = 0; i < cRegMaterialNum; i++) {
		if (anm->getCRegUpdateMaterialID(i) != 0xFFFF) {
			J3DMaterialAnm* pMatAnm
			    = getMaterialNodePointer(anm->getCRegUpdateMaterialID(i))
			          ->getMaterialAnm();
			u8 colorId = anm->getAnmCRegKeyTable()[i].mColorId;
			if (pMatAnm == nullptr)
				ret = 1;
			else
				pMatAnm->setTevColorAnm(colorId, nullptr);
		}
	}

	for (u16 i = 0; i < kRegMaterialNum; i++) {
		if (anm->getKRegUpdateMaterialID(i) != 0xFFFF) {
			J3DMaterialAnm* pMatAnm
			    = getMaterialNodePointer(anm->getKRegUpdateMaterialID(i))
			          ->getMaterialAnm();
			u8 colorId = anm->getAnmKRegKeyTable()[i].mColorId;
			if (pMatAnm == nullptr) {
				ret = 1;
			} else {
				pMatAnm->setTevKColorAnm(colorId, nullptr);
			}
		}
	}

	return ret;
}

int J3DModelData::setMatColorAnimator(J3DAnmColor* anm, J3DMatColorAnm* anm_r)
{
	int ret         = 0;
	u16 materialNum = anm->getUpdateMaterialNum();

	for (u16 i = 0; i < materialNum; i++) {
		u16 materialID = anm->getUpdateMaterialID(i);
		if (materialID != 0xFFFF) {
			J3DMaterialAnm* matAnm
			    = getMaterialNodePointer(materialID)->getMaterialAnm();
			if (matAnm == nullptr)
				ret = 1;
			else
				matAnm->setMatColorAnm(0, &anm_r[i]);
		}
	}

	return ret;
}

int J3DModelData::setTexNoAnimator(J3DAnmTexPattern* anm, J3DTexNoAnm* anm_r)
{
	int ret         = 0;
	u16 materialNum = anm->getUpdateMaterialNum();

	for (u16 i = 0; i < materialNum; i++) {
		u16 materialID = anm->getUpdateMaterialID(i);
		if (materialID != 0xFFFF) {
			J3DMaterialAnm* pMatAnm
			    = getMaterialNodePointer(materialID)->getMaterialAnm();
			u8 texNo = anm->getAnmTable()[i].mTexNo;
			if (pMatAnm == nullptr)
				ret = 1;
			else
				pMatAnm->setTexNoAnm(texNo, &anm_r[i]);
		}
	}

	return ret;
}

int J3DModelData::setTexMtxAnimator(J3DAnmTextureSRTKey* anm,
                                    J3DTexMtxAnm* tex_anm,
                                    J3DTexMtxAnm* dual_anm_r)
{
	J3D_ASSERT_NULLPTR(anm != nullptr);

	int ret         = 0;
	u16 materialNum = anm->getUpdateMaterialNum();

	for (u16 i = 0; i < materialNum; i++) {
		u16 materialID         = anm->getUpdateMaterialID(i);
		J3DMaterial* pMaterial = getMaterialNodePointer(materialID);
		J3DMaterialAnm* pMatAnm
		    = getMaterialNodePointer(materialID)->getMaterialAnm();
		u16 texMtxID = anm->getUpdateTexMtxID(i);
		if (pMatAnm == nullptr) {
			ret = 1;
		} else if (texMtxID != 0xFF) {
			J3DTexMtxInfo& texMtxInfo
			    = pMaterial->getTexMtx(texMtxID)->getTexMtxInfo();
			texMtxInfo.mInfo
			    = (texMtxInfo.mInfo & 0x7F) | (anm->getTexMtxCalcType() << 7);

			texMtxInfo.mCenter.x = anm->getSRTCenter(i)->x;
			texMtxInfo.mCenter.y = anm->getSRTCenter(i)->y;
			texMtxInfo.mCenter.z = anm->getSRTCenter(i)->z;

			pMatAnm->setTexMtxAnm(texMtxID, &tex_anm[i]);
		}
	}

	return ret;
}

int J3DModelData::setTevRegAnimator(J3DAnmTevRegKey* pAnm,
                                    J3DTevColorAnm* pCRegAnmR,
                                    J3DTevKColorAnm* pKRegAnmR)
{
	s32 ret             = 0;
	u16 cRegMaterialNum = pAnm->getCRegUpdateMaterialNum();
	u16 kRegMaterialNum = pAnm->getKRegUpdateMaterialNum();

	for (u16 i = 0; i < cRegMaterialNum; i++) {
		if (pAnm->getCRegUpdateMaterialID(i) != 0xFFFF) {
			J3DMaterialAnm* pMatAnm
			    = getMaterialNodePointer(pAnm->getCRegUpdateMaterialID(i))
			          ->getMaterialAnm();
			u8 colorId = pAnm->getAnmCRegKeyTable()[i].mColorId;
			if (pMatAnm == nullptr)
				ret = 1;
			else
				pMatAnm->setTevColorAnm(colorId, &pCRegAnmR[i]);
		}
	}

	for (u16 i = 0; i < kRegMaterialNum; i++) {
		if (pAnm->getKRegUpdateMaterialID(i) != 0xFFFF) {
			J3DMaterialAnm* pMatAnm
			    = getMaterialNodePointer(pAnm->getKRegUpdateMaterialID(i))
			          ->getMaterialAnm();
			u8 colorId = pAnm->getAnmKRegKeyTable()[i].mColorId;
			if (pMatAnm == nullptr) {
				ret = 1;
			} else {
				pMatAnm->setTevKColorAnm(colorId, &pKRegAnmR[i]);
			}
		}
	}

	return ret;
}

J3DModel::J3DModel() { initialize(); }

J3DModel::J3DModel(J3DModelData* model_data, u32 model_flag,
                   u32 mtx_buffer_flag)
{
	initialize();
	entryModelData(model_data, model_flag, mtx_buffer_flag);
}

J3DModel::~J3DModel() { }

void J3DModel::initialize()
{
	unkC        = nullptr;
	unk8        = 0;
	mModelData  = nullptr;
	mDeformData = 0;
	mSkinDeform = nullptr;
	unk14.x     = 1.0;
	unk14.y     = 1.0;
	unk14.z     = 1.0;

	MTXIdentity(unk20);

	mScaleFlagArr       = nullptr;
	mEvlpScaleFlagArr   = nullptr;
	mNodeMatrices       = nullptr;
	mWeightEvlpMatrices = nullptr;

	mDrawMtxBuf[0] = nullptr;
	mDrawMtxBuf[1] = nullptr;
	mNrmMtxBuf[0]  = nullptr;
	mNrmMtxBuf[1]  = nullptr;
	mBumpMtxArr[0] = nullptr;
	mBumpMtxArr[1] = nullptr;

	mCurrentViewNo = 0;
	mVertexBuffer  = nullptr;
	mMatPackets    = nullptr;
	mShapePackets  = nullptr;
	unk9C          = nullptr;
	unk90          = nullptr;
	unk94          = nullptr;
}

void J3DModel::entryModelData(J3DModelData* pModelData, u32 mdlFlags,
                              u32 mtxNum)
{
	J3D_ASSERT_NULLPTR(pModelData != nullptr);
	J3D_ASSERT_NONZEROARG(mtxNum != 0);

	mModelData = pModelData;
	if (pModelData->getJointNum()) {
		mScaleFlagArr = new u8[pModelData->getJointNum()];
		if (pModelData->getWEvlpMtxNum())
			mEvlpScaleFlagArr = new u8[pModelData->getWEvlpMtxNum()];
		mNodeMatrices = new Mtx[pModelData->getJointNum()];
	}

	if (pModelData->getWEvlpMtxNum())
		mWeightEvlpMatrices = new Mtx[pModelData->getWEvlpMtxNum()];

	if (mtxNum != 0) {
		for (int i = 0; i < 2; ++i) {
			mDrawMtxBuf[i] = new Mtx*[mtxNum];
			mNrmMtxBuf[i]  = new Mtx33*[mtxNum];
			mBumpMtxArr[i] = nullptr;
		}
	}

	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < mtxNum; ++j) {
			if (pModelData->getDrawMtxNum()) {
				mDrawMtxBuf[i][j] = new (0x20) Mtx[pModelData->getDrawMtxNum()];
				mNrmMtxBuf[i][j]
				    = new (0x20) Mtx33[pModelData->getDrawMtxNum()];
			}
		}
	}

	if (pModelData->getShapeNum()) {
		mShapePackets = new J3DShapePacket[pModelData->getShapeNum()];

		for (int i = 0; i < pModelData->getShapeNum(); ++i)
			mShapePackets[i].setShape(pModelData->getShapeNodePointer(i));
	}

	if (pModelData->getMaterialNum()) {
		mMatPackets = new J3DMatPacket[pModelData->getMaterialNum()];

		for (int i = 0; i < pModelData->getMaterialNum(); ++i) {
			mMatPackets[i].setMaterial(pModelData->getMaterialNodePointer(i));
			J3DShape* shape = pModelData->getMaterialNodePointer(i)->getShape();
			mMatPackets[i].addShapePacket(&mShapePackets[shape->getIndex()]);
			mMatPackets[i].setTexture(pModelData->getTexture());

			if (mdlFlags & 0x20000) {
				J3DMaterial* mat = pModelData->getMaterialNodePointer(i);
				u32 dlSize       = mat->countDLSize();
				mMatPackets[i].setDisplayListObj(
				    mat->newSharedDisplayList(dlSize));
			} else {
				J3DMaterial* mat     = pModelData->getMaterialNodePointer(i);
				u32 dlSize           = mat->countDLSize();
				J3DMatPacket* packet = &mMatPackets[i];
				packet->setDisplayListObj(new J3DDisplayListObj);
				packet->getDisplayListObj()->newDisplayList(dlSize);
			}
		}
	}

	u16 totalBumpMatrices     = 0;
	u16 totalMatsWithNbtScale = 0;
	for (int i = 0; i < pModelData->getMaterialNum(); ++i) {
		J3DMaterial* mat = mModelData->getMaterialNodePointer(i);
		if (mat->getNBTScale()->mbHasScale == 1) {
			totalBumpMatrices += mat->getShape()->countBumpMtxNum();
			++totalMatsWithNbtScale;
		}
	}

	if (totalBumpMatrices != 0 && mtxNum != 0) {
		for (int i = 0; i < 2; ++i) {
			mBumpMtxArr[i] = new Mtx33**[totalMatsWithNbtScale];
		}
	}

	for (int i = 0; i < 2; ++i) {
		u32 nextBumpMtx = 0;
		for (int j = 0; j < pModelData->getMaterialNum(); ++j) {
			J3DMaterial* mat = mModelData->getMaterialNodePointer(j);
			if (mat->getNBTScale()->mbHasScale == 1) {
				mBumpMtxArr[i][nextBumpMtx] = new Mtx33*[mtxNum];

				mat->getShape()->setBumpMtxOffset(nextBumpMtx);
				++nextBumpMtx;
			}
		}
	}

	for (int i = 0; i < 2; ++i) {
		int j2 = 0;
		for (int j = 0; j < pModelData->getMaterialNum(); ++j) {
			J3DMaterial* mat = mModelData->getMaterialNodePointer(j);

			if (mat->getNBTScale()->mbHasScale != 1)
				continue;

			for (int k = 0; k < mtxNum; ++k)
				mBumpMtxArr[i][j2][k]
				    = new (0x20) Mtx33[pModelData->getDrawMtxNum()];
			++j2;
		}
	}

	if (totalMatsWithNbtScale != 0) {
		mModelData->setBumpFlag(1);
	}

	mVertexBuffer = new J3DVertexBuffer(&pModelData->getVertexData());
}

void J3DModel::lock()
{
	int matNum = mModelData->getMaterialNum();

	for (int i = 0; i < matNum; ++i)
		mMatPackets[i].lock();
}

void J3DModel::unlock()
{
	int matNum = mModelData->getMaterialNum();

	for (int i = 0; i < matNum; ++i)
		mMatPackets[i].unlock();
}

void J3DModel::makeDL()
{
	j3dSys.setModel(this);
	j3dSys.setTexture(mModelData->getTexture());
	for (u16 i = 0; i < mModelData->getMaterialNum(); ++i) {
		j3dSys.setMatPacket(&mMatPackets[i]);
		mModelData->getMaterialNodePointer(i)->makeDisplayList();
	}
}

void J3DModel::setSkinDeform(J3DSkinDeform* pSkinDeform,
                             J3DDeformAttachFlag flags)
{
	mSkinDeform = pSkinDeform;

	if (pSkinDeform == nullptr) {
		unk8 &= ~0x4;
		unk8 &= ~0x8;
	} else {
		unk8 |= 0x4;
		unk8 |= 0x8;
		mSkinDeform->initMtxIndexArray(mModelData);
		mVertexBuffer->copyTransformedVtxArray();
	}
}

static f32 J3DUnit01[] = { 0.0f, 1.0f };

#define qr0 0

void J3DModel::calcWeightEnvelopeMtx()
{
	register MtxPtr weightAnmMtx;
	register Mtx* worldMtx;
	register MtxPtr invMtx;
	register f32 weight;
	int idx;
	int j;
	int mixNum;
	int i;
	int max;
	u8* pScale;
	u16* indices;
	f32* weights;

	register f32 var_f0;
	register f32 var_f1;
	register f32 var_f2;
	register f32 var_f3;
	register f32 var_f4;
	register f32 var_f5;
	register f32 var_f6;
	register f32 var_f7;
	register f32 var_f8;
	register f32 var_f9;
	register f32 var_f10;
	register f32 var_f11;
	register f32 var_f12;
	register f32 var_f13;
	register f32 var_f31;
	register f32 var_f30;
	register f32 var_f29;
	register f32 var_f28;
	register f32 var_f27;
	register f32* var_r7 = J3DUnit01;

	i       = -1;
	max     = mModelData->getWEvlpMtxNum();
	indices = mModelData->getWEvlpMixMtxIndex() - 1;
	weights = mModelData->getWEvlpMixWeight() - 1;

#ifdef __MWERKS__ // clang-format off
	asm {
		psq_l var_f27, 0x0(var_r7), 0, qr0
		ps_merge00 var_f10, var_f27, var_f27
		ps_merge00 var_f12, var_f27, var_f27
		ps_merge00 var_f31, var_f27, var_f27
	}
#endif // clang-format on

	while (++i < max)
	{
		pScale       = &mEvlpScaleFlagArr[i];
		*pScale      = 1;
		weightAnmMtx = mWeightEvlpMatrices[i];

#ifdef __MWERKS__ // clang-format off
		asm {
			ps_merge00 var_f9, var_f27, var_f27
			ps_merge00 var_f11, var_f27, var_f27
			ps_merge00 var_f13, var_f27, var_f27
		}
#endif // clang-format on

		j      = 0;
		mixNum = mModelData->getWEvlpMixMtxNum(i);

		do {
			idx      = *++indices;
			invMtx   = mModelData->getInvJointMtx((u16)idx);
			worldMtx = &mNodeMatrices[idx];

#ifdef __MWERKS__ // clang-format off
			asm {
				psq_l var_f0, 0x0(invMtx), 0, qr0
				psq_l var_f1, 0x0(worldMtx), 0, qr0
				psq_l var_f3, 0x10(worldMtx), 0, qr0
				psq_l var_f5, 0x20(worldMtx), 0, qr0
				ps_muls0 var_f8, var_f0, var_f1
				psq_l var_f6, 0x10(invMtx), 0, qr0
				ps_muls0 var_f30, var_f0, var_f3
				ps_muls0 var_f29, var_f0, var_f5
				psq_l var_f7, 0x20(invMtx), 0, qr0
				ps_madds1 var_f8, var_f6, var_f1, var_f8
				psq_l var_f2, 0x8(worldMtx), 0, qr0
				ps_madds1 var_f30, var_f6, var_f3, var_f30
				psq_l var_f4, 0x18(worldMtx), 0, qr0
				ps_madds1 var_f29, var_f6, var_f5, var_f29
				psq_l var_f6, 0x28(worldMtx), 0, qr0
				ps_madds0 var_f8, var_f7, var_f2, var_f8
			}
#endif // clang-format on

			weight = *++weights;

#ifdef __MWERKS__ // clang-format off
			asm {
				ps_madds0 var_f30, var_f7, var_f4, var_f30
				ps_madds0 var_f29, var_f7, var_f6, var_f29
				psq_l var_f7, 0x8(invMtx), 0, qr0
				ps_madds0 var_f9, var_f8, weight, var_f9
				ps_madds0 var_f11, var_f30, weight, var_f11
				ps_madds0 var_f13, var_f29, weight, var_f13
				psq_l var_f8, 0x18(invMtx), 0, qr0
				ps_muls0 var_f30, var_f7, var_f1
				ps_muls0 var_f29, var_f7, var_f3
				ps_muls0 var_f28, var_f7, var_f5
				psq_l var_f7, 0x28(invMtx), 0, qr0
				psq_st var_f9, 0x0(weightAnmMtx), 0, qr0
				ps_madds1 var_f30, var_f8, var_f1, var_f30
				ps_madds1 var_f29, var_f8, var_f3, var_f29
				ps_madds1 var_f28, var_f8, var_f5, var_f28
				ps_madds0 var_f30, var_f7, var_f2, var_f30
				ps_madds0 var_f29, var_f7, var_f4, var_f29
				ps_madds0 var_f28, var_f7, var_f6, var_f28
				psq_st var_f11, 0x10(weightAnmMtx), 0, qr0
				psq_st var_f13, 0x20(weightAnmMtx), 0, qr0
				ps_madd var_f30, var_f27, var_f2, var_f30
				ps_madd var_f29, var_f27, var_f4, var_f29
				ps_madd var_f28, var_f27, var_f6, var_f28
				ps_madds0 var_f10, var_f30, weight, var_f10
				ps_madds0 var_f12, var_f29, weight, var_f12
				ps_madds0 var_f31, var_f28, weight, var_f31
			}
#endif // clang-format on

			*pScale &= mScaleFlagArr[idx];
		} while (++j < mixNum);

#ifdef __MWERKS__ // clang-format off
		asm {
			psq_st var_f10, 0x8(weightAnmMtx), 0, qr0
			ps_merge00 var_f10, var_f27, var_f27
			psq_st var_f12, 0x18(weightAnmMtx), 0, qr0
			ps_merge00 var_f12, var_f27, var_f27
			psq_st var_f31, 0x28(weightAnmMtx), 0, qr0
			ps_merge00 var_f31, var_f27, var_f27
		}
#endif // clang-format on
	}
}

void J3DModel::update()
{
	j3dSys.setModel(this);

	if (checkFlag(4)) {
		j3dSys.onFlag(0x4);
	} else {
		j3dSys.offFlag(0x4);
	}

	if (checkFlag(0x8)) {
		j3dSys.onFlag(0x8);
	} else {
		j3dSys.offFlag(0x8);
	}

	mVertexBuffer->frameInit();

	if (unk9C != nullptr)
		unk9C->calc(mModelData);

	if (mDeformData != nullptr)
		mDeformData->deform(this);

	if (unk90 != nullptr)
		unk90->calc(this);

	if (unk94 != nullptr)
		unk94->calc(this);

	j3dSys.setCurrentMtxCalc(mModelData->getMtxCalc());
	mModelData->getMtxCalc()->init(unk14, unk20);
	j3dSys.setTexture(mModelData->getTexture());
	mModelData->getMtxCalc()->recursiveUpdate(mModelData->getRootNode());
	calcWeightEnvelopeMtx();

	if (mSkinDeform)
		mSkinDeform->deform(this);

	if (unkC)
		unkC(this, 0);
}

void J3DModel::calc()
{
	j3dSys.setModel(this);

	if (checkFlag(4)) {
		j3dSys.onFlag(0x4);
	} else {
		j3dSys.offFlag(0x4);
	}

	if (checkFlag(0x8)) {
		j3dSys.onFlag(0x8);
	} else {
		j3dSys.offFlag(0x8);
	}

	mVertexBuffer->frameInit();

	if (unk9C != nullptr)
		unk9C->calc(mModelData);

	if (mDeformData != nullptr)
		mDeformData->deform(this);

	if (unk90 != nullptr)
		unk90->calc(this);

	if (unk94 != nullptr)
		unk94->calc(this);

	j3dSys.setCurrentMtxCalc(mModelData->getMtxCalc());
	mModelData->getMtxCalc()->init(unk14, unk20);
	j3dSys.setTexture(mModelData->getTexture());
	mModelData->getMtxCalc()->recursiveCalc(mModelData->getRootNode());

	calcWeightEnvelopeMtx();

	if (mSkinDeform)
		mSkinDeform->deform(this);

	if (unkC)
		unkC(this, 0);
}

void J3DModel::entry()
{

	j3dSys.setModel(this);

	if (unk8 & 0x4 ? 1 : 0) {
		j3dSys.mFlags |= 0x4;
	} else {
		j3dSys.mFlags &= ~0x4;
	}

	if (unk8 & 0x8 ? 1 : 0) {
		j3dSys.mFlags |= 0x8;
	} else {
		j3dSys.mFlags &= ~0x8;
	}

	j3dSys.setTexture(mModelData->getTexture());

	mModelData->unk14->recursiveEntry(mModelData->mRootNode);
}

void J3DModel::viewCalc()
{
	swapDrawMtx();
	swapNrmMtx();

	if (checkFlag(1)) {
		for (u16 i = 0; i < mModelData->getDrawFullWgtMtxNum(); i++) {
			MTXCopy(getAnmMtx(mModelData->getDrawMtxIndex(i)), getDrawMtx(i));
		}
		for (u16 i = 0; i < mModelData->getWEvlpMtxNum(); i++) {
			MTXCopy(mWeightEvlpMatrices[i],
			        getDrawMtx(mModelData->getDrawFullWgtMtxNum() + i));
		}
	} else {
		MtxPtr viewMtx = j3dSys.getViewMtx();
		if (mModelData->getDrawFullWgtMtxNum() != 0) {
			J3DMTXConcatArrayIndexedSrc(
			    viewMtx, mNodeMatrices, mModelData->mDrawMtxData.mDrawMtxIndex,
			    getDrawMtxPtr(), mModelData->getDrawFullWgtMtxNum());
		}
		if (mModelData->getDrawMtxNum() > mModelData->getDrawFullWgtMtxNum()) {
			J3DPSMtxArrayConcat(viewMtx, getWeightAnmMtx(0),
			                    getDrawMtx(mModelData->getDrawFullWgtMtxNum()),
			                    mModelData->getWEvlpMtxNum());
		}
	}

	calcNrmMtx();
	calcBBoard();
	calcBumpMtx();
	DCStoreRange(getDrawMtxPtr(),
	             getModelData()->getDrawMtxNum() * sizeof(Mtx));
	DCStoreRange(getNrmMtxPtr(),
	             getModelData()->getDrawMtxNum() * sizeof(Mtx33));
	prepareShapePackets();
}

void J3DModel::calcNrmMtx()
{
	for (u16 i = 0; i < mModelData->getDrawMtxNum(); i++) {
		if (mModelData->getDrawMtxFlag(i) == 0) {
			if (getScaleFlag(mModelData->getDrawMtxIndex(i)) == 1) {
				Mtx& drawMtx = mDrawMtxBuf[1][mCurrentViewNo][i];
				J3DPSMtx33CopyFrom34(drawMtx, mNrmMtxBuf[1][mCurrentViewNo][i]);
			} else {
				Mtx33& nrmMtx = mNrmMtxBuf[1][mCurrentViewNo][i];
				J3DPSCalcInverseTranspose(mDrawMtxBuf[1][mCurrentViewNo][i],
				                          nrmMtx);
			}
		} else {
			if (getEnvScaleFlag(mModelData->getDrawMtxIndex(i)) == 1) {
				Mtx& drawMtx = mDrawMtxBuf[1][mCurrentViewNo][i];
				J3DPSMtx33CopyFrom34(drawMtx, mNrmMtxBuf[1][mCurrentViewNo][i]);
			} else {
				Mtx33& nrmMtx = mNrmMtxBuf[1][mCurrentViewNo][i];
				J3DPSCalcInverseTranspose(mDrawMtxBuf[1][mCurrentViewNo][i],
				                          nrmMtx);
			}
		}
	}
}

void J3DModel::calcBumpMtx()
{
	if (mModelData->unk18 != 1)
		return;

	s32 nextBumpMtx = 0;
	for (s32 i = 0; i < mModelData->getMaterialNum(); i++) {
		J3DMaterial* pMaterial = getModelData()->getMaterialNodePointer(i);
		if (pMaterial->getNBTScale()->mbHasScale == 1) {

			pMaterial->getShape()->calcNBTScale(
			    *pMaterial->getNBTScale()->getScale(), getNrmMtxPtr(),
			    getBumpMtxPtr(nextBumpMtx));

			DCStoreRange(getBumpMtxPtr(nextBumpMtx),
			             mModelData->getDrawMtxNum() * sizeof(Mtx33));
			nextBumpMtx++;
		}
	}
}

void J3DModel::calcBBoard()
{
	// TP debug confirms that they were literally choosing at random
	// whether to call getModelData() or access mModelData directly,
	// so this might be real.

	if (!getModelData()->checkBBoardFlag())
		return;

	for (u16 i = 0; i < mModelData->getDrawMtxNum(); i++) {
		if (getModelData()->getDrawMtxFlag(i) != 0)
			continue;

		u16 index = getModelData()->getDrawMtxIndex(i);

		if (mModelData->getJointNodePointer(index)->getMtxType()
		    == J3DJntMtxType_BBoard) {
			MtxPtr drawMtx = getDrawMtx(i);

			f32 sx = std::sqrtf(drawMtx[0][0] * drawMtx[0][0]
			                    + drawMtx[1][0] * drawMtx[1][0]
			                    + drawMtx[2][0] * drawMtx[2][0]);
			f32 sy = std::sqrtf(drawMtx[0][1] * drawMtx[0][1]
			                    + drawMtx[1][1] * drawMtx[1][1]
			                    + drawMtx[2][1] * drawMtx[2][1]);
			f32 sz = std::sqrtf(drawMtx[0][2] * drawMtx[0][2]
			                    + drawMtx[1][2] * drawMtx[1][2]
			                    + drawMtx[2][2] * drawMtx[2][2]);

			drawMtx[0][0] = sx;
			drawMtx[0][1] = 0.0f;
			drawMtx[0][2] = 0.0f;

			drawMtx[1][0] = 0.0f;
			drawMtx[1][1] = sy;
			drawMtx[1][2] = 0.0f;

			drawMtx[2][0] = 0.0f;
			drawMtx[2][1] = 0.0f;
			drawMtx[2][2] = sz;

			Mtx33& nrmMtx = getNrmMtx(i);

			nrmMtx[0][0] = 1.0f / sx;
			nrmMtx[0][1] = 0.0f;
			nrmMtx[0][2] = 0.0f;

			nrmMtx[1][0] = 0.0f;
			nrmMtx[1][1] = 1.0f / sy;
			nrmMtx[1][2] = 0.0f;

			nrmMtx[2][0] = 0.0f;
			nrmMtx[2][1] = 0.0f;
			nrmMtx[2][2] = 1.0f / sz;
		} else if (mModelData->getJointNodePointer(index)->getMtxType()
		           == J3DJntMtxType_YBBoard) {
			MtxPtr drawMtx = getDrawMtx(i);

			f32 sx = std::sqrtf(drawMtx[0][0] * drawMtx[0][0]
			                    + drawMtx[1][0] * drawMtx[1][0]
			                    + drawMtx[2][0] * drawMtx[2][0]);
			f32 sy = std::sqrtf(drawMtx[0][1] * drawMtx[0][1]
			                    + drawMtx[1][1] * drawMtx[1][1]
			                    + drawMtx[2][1] * drawMtx[2][1]);
			f32 sz = std::sqrtf(drawMtx[0][2] * drawMtx[0][2]
			                    + drawMtx[1][2] * drawMtx[1][2]
			                    + drawMtx[2][2] * drawMtx[2][2]);

			Vec axisX, axisY, axisZ;

			axisX.x = 1.0f;
			axisX.y = 0.0f;
			axisX.z = 0.0f;

			axisY.x = drawMtx[0][1];
			axisY.y = drawMtx[1][1];
			axisY.z = drawMtx[2][1];

			VECCrossProduct(&axisX, &axisY, &axisZ);
			VECNormalize(&axisY, &axisY);
			VECNormalize(&axisZ, &axisZ);

			drawMtx[0][0] = axisX.x * sx;
			drawMtx[0][1] = axisY.x * sy;
			drawMtx[0][2] = axisZ.x * sz;

			drawMtx[1][0] = axisX.y * sx;
			drawMtx[1][1] = axisY.y * sy;
			drawMtx[1][2] = axisZ.y * sz;

			drawMtx[2][0] = axisX.z * sx;
			drawMtx[2][1] = axisY.z * sy;
			drawMtx[2][2] = axisZ.z * sz;

			J3DPSCalcInverseTranspose(drawMtx, getNrmMtx(i));
		}
	}
}

void J3DModel::prepareShapePackets()
{
	u16 shapeNum = mModelData->getShapeNum();

	for (u16 i = 0; i < shapeNum; i++) {
		J3DShape* shape     = mModelData->getShapeNodePointer(i);
		J3DShapePacket* pkt = &mShapePackets[i];
		shape->setScaleFlagArray(mScaleFlagArr);

		if (checkFlag(0x4))
			shape->onFlag(J3DShpFlag_SkinPosCpu);
		else
			shape->offFlag(J3DShpFlag_SkinPosCpu);

		if (checkFlag(0x8) && !shape->checkFlag(J3DShpFlag_EnableLod))
			shape->onFlag(J3DShpFlag_SkinNrmCpu);
		else
			shape->offFlag(J3DShpFlag_SkinNrmCpu);

		pkt->setVtxPos(mVertexBuffer->getCurrentVtxPos());
		pkt->setVtxNrm(mVertexBuffer->getCurrentVtxNrm());
		pkt->setVtxCol(mVertexBuffer->getCurrentVtxCol());
		pkt->setDrawMtx(mDrawMtxBuf[1]);
		pkt->setNrmMtx(mNrmMtxBuf[1]);
		pkt->setCurrentViewNoPtr(&mCurrentViewNo);
	}

	for (s32 i = 0; i < mModelData->getMaterialNum(); i++) {
		J3DMaterial* pMaterial = mModelData->getMaterialNodePointer(i);
		if (pMaterial->getNBTScale()->mbHasScale == 1) {
			J3DShape* matShape = pMaterial->getShape();
			getShapePacket(matShape->getIndex())
			    ->setNrmMtx(mBumpMtxArr[1][matShape->getBumpMtxOffset()]);
		}
	}
}
