#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DShape.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTexture.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
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
	unk1A             = 0;
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
		J3DNode* newNode         = nullptr;
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
			newNode = mJointNodePointer[(++*hierarchy)->mValue];
			break;
		case kTypeMaterial:
			newMaterial = mMaterials[(++*hierarchy)->mValue];
			break;
		case kTypeShape:
			newShape = mShapeNodePointer[(++*hierarchy)->mValue];
			break;
		}

		if (newNode != nullptr) {
			curNode = newNode;
			if (root_node == nullptr)
				mRootNode = (J3DJoint*)newNode;
			else
				root_node->appendChild(newNode);
		} else if (newMaterial != nullptr && root_node->getType() == 'NJNT') {
			((J3DJoint*)root_node)->addMesh(newMaterial);
		} else if (newShape != nullptr && root_node->getType() == 'NJNT') {
			((J3DJoint*)root_node)->getMesh()->addShape(newShape);
			newShape->unk48 = &mDrawMtxData;
			newShape->unk44 = &mVertexData;
			newShape->makeVcdVatCmd();
		}
	}
}

bool J3DModelData::isDeformableVertexFormat() const
{
	GXVtxAttrFmtList* vtxAttrFmtList;
	bool bVar1 = false;
	for (vtxAttrFmtList = mVertexData.mVtxAttrFmtList;
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
			if (nameIndex != -1)
				getMaterialNodePointer(nameIndex)->copy(
				    mat_table->getMaterialNodePointer(i));
		}
	}

	if (flag & J3DMatCopyFlag_Texture) {
		if (mat_table->getTexture()->getNum() != 0) {
			setTexture(mat_table->getTexture());
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
			if (pMatAnm == NULL) {
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
		u16 materialID = anm->getUpdateMaterialID(i);
		if (materialID != 0xFFFF) {
			J3DMaterial* pMaterial  = getMaterialNodePointer(materialID);
			J3DMaterialAnm* pMatAnm = pMaterial->getMaterialAnm();
			u32 texMtxID            = anm->getUpdateTexMtxID(i);

			if (pMatAnm == NULL) {
				ret = 1;
				continue;
			}

			if (texMtxID != 0xFF) {
				if (pMaterial->getTexGenBlock()->getTexMtx(texMtxID) == NULL) {
					J3DTexMtx* pMtx = new J3DTexMtx;
					pMaterial->getTexGenBlock()->setTexMtx(texMtxID, pMtx);
				}

				pMaterial->getTexCoord(texMtxID)->setTexGenMtx(
				    (GXTexMtx)(GX_TEXMTX0 + texMtxID * 3));

				J3DTexMtx* pTexMtx       = pMaterial->getTexMtx(texMtxID);
				J3DTexMtxAnm* pTexMtxAnm = new J3DTexMtxAnm(anm, i);

				pTexMtx->mInfo = ((pTexMtx->mInfo) & 0x7F)
				                 | (anm->getTexMtxCalcType() << 7);

				pTexMtx->mCenter.x = anm->getSRTCenter(i).x;
				pTexMtx->mCenter.y = anm->getSRTCenter(i).y;
				pTexMtx->mCenter.z = anm->getSRTCenter(i).z;

				pMatAnm->setTexMtxAnm(texMtxID, pTexMtxAnm);
			}
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
		u16 materialID = anm->getCRegUpdateMaterialID(i);
		if (materialID != 0xFFFF) {
			J3DMaterial* mat        = getMaterialNodePointer(materialID);
			J3DMaterialAnm* pMatAnm = mat->getMaterialAnm();
			u8 colorId              = anm->getAnmCRegKeyTable()[i].mColorId;
			if (pMatAnm == NULL)
				ret = 1;
			else
				pMatAnm->setTevColorAnm(colorId, new J3DTevColorAnm(anm, i));
		}
	}

	for (u16 i = 0; i < kRegMaterialNum; i++) {
		u16 materialID = anm->getKRegUpdateMaterialID(i);
		if (materialID != 0xFFFF) {
			J3DMaterial* mat       = getMaterialNodePointer(materialID);
			J3DMaterialAnm* matAnm = mat->getMaterialAnm();
			u8 colorId             = anm->getAnmKRegKeyTable()[i].mColorId;
			if (matAnm == NULL)
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
			if (matAnm == NULL)
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
			if (matAnm == NULL)
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
			if (pMatAnm == NULL)
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
	int ret         = 0;
	u16 materialNum = anm->getUpdateMaterialNum();

	for (u16 i = 0; i < materialNum; i++) {
		u16 materialID          = anm->getUpdateMaterialID(i);
		J3DMaterial* pMaterial  = getMaterialNodePointer(materialID);
		J3DMaterialAnm* pMatAnm = pMaterial->getMaterialAnm();
		u32 texMtxID            = anm->getUpdateTexMtxID(i);
		if (pMatAnm == NULL) {
			ret = 1;
		} else if (texMtxID != 0xFF) {
			J3DTexMtx* pTexMtx = pMaterial->getTexMtx(texMtxID);
			pTexMtx->mInfo
			    = ((pTexMtx->mInfo) & 0x7F) | (anm->getTexMtxCalcType() << 7);

			pTexMtx->mCenter.x = anm->getSRTCenter(i).x;
			pTexMtx->mCenter.y = anm->getSRTCenter(i).y;
			pTexMtx->mCenter.z = anm->getSRTCenter(i).z;

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
			if (pMatAnm == NULL)
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
			if (pMatAnm == NULL) {
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

	unkC         = nullptr;
	unk8         = 0;
	mModelData   = nullptr;
	mDeformData  = 0;
	mSkinDeform  = nullptr;
	mBaseScale.x = 1.0;
	mBaseScale.y = 1.0;
	mBaseScale.z = 1.0;

	MTXIdentity(mBaseMtx);

	mScaleFlagArr     = nullptr;
	mEvlpScaleFlagArr = nullptr;
	mNodeMatrices     = nullptr;
	unk5C             = nullptr;

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
	unk9C          = (void*)0x0;
	unk90          = (void*)0x0;
	unk94          = (void*)0x0;
}

void J3DModel::entryModelData(J3DModelData* param_1, u32 param_2, u32 param_3)
{
	mModelData = param_1;
	if (param_1->getJointNum()) {
		mScaleFlagArr = new u8[param_1->getJointNum()];
		if (param_1->getWEvlpMtxNum())
			mEvlpScaleFlagArr = new u8[param_1->getWEvlpMtxNum()];
		mNodeMatrices = new Mtx[param_1->getJointNum()];
	}

	if (param_1->getWEvlpMtxNum())
		unk5C = new Mtx[param_1->getWEvlpMtxNum()];

	if (param_3 != 0) {
		for (int i = 0; i < 2; ++i) {
			mDrawMtxBuf[i] = new Mtx*[param_3];
			mNrmMtxBuf[i]  = new Mtx33*[param_3];
			mBumpMtxArr[i] = nullptr;
		}
	}

	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < param_3; ++j) {
			if (param_1->getDrawMtxNum()) {
				mDrawMtxBuf[i][j] = new (0x20) Mtx[param_1->getDrawMtxNum()];
				mNrmMtxBuf[i][j]  = new (0x20) Mtx33[param_1->getDrawMtxNum()];
			}
		}
	}

	if (param_1->getShapeNum()) {
		mShapePackets = new J3DShapePacket[param_1->getShapeNum()];

		for (int i = 0; i < param_1->getShapeNum(); ++i)
			mShapePackets[i].unk14 = param_1->getShapeNodePointer(i);
	}

	if (param_1->getMaterialNum()) {
		mMatPackets = new J3DMatPacket[param_1->getMaterialNum()];

		for (int i = 0; i < param_1->getMaterialNum(); ++i) {
			mMatPackets[i].unk38 = param_1->getMaterialNodePointer(i);
			mMatPackets[i].addShapePacket(
			    &mShapePackets
			        [param_1->getMaterialNodePointer(i)->getShape()->unk4]);
			mMatPackets[i].mTexture = param_1->getTexture();

			if (param_2 & 0x20000) {
				J3DMaterial* mat     = param_1->getMaterialNodePointer(i);
				u32 dlSize           = mat->countDLSize();
				mMatPackets[i].unk30 = mat->newSharedDisplayList(dlSize);
			} else {
				u32 dlSize = param_1->getMaterialNodePointer(i)->countDLSize();
				J3DMatPacket* packet = &mMatPackets[i];
				packet->unk30        = new J3DDisplayListObj;
				packet->unk30->newDisplayList(dlSize);
			}
		}
	}

	u16 totalBumpMatrices     = 0;
	u16 totalMatsWithNbtScale = 0;
	for (int i = 0; i < param_1->mMaterialNum; ++i) {
		J3DMaterial* mat      = mModelData->getMaterialNodePointer(i);
		J3DNBTScale* nbtScale = mat->getTexGenBlock()->getNBTScale();
		if (nbtScale->mbHasScale == 1) {
			totalBumpMatrices += mat->getShape()->countBumpMtxNum();
			++totalMatsWithNbtScale;
		}
	}

	if (totalBumpMatrices != 0 && param_3 != 0) {
		for (int i = 0; i < 2; ++i) {
			mBumpMtxArr[i] = new Mtx33**[totalMatsWithNbtScale];
		}
	}

	for (int i = 0; i < 2; ++i) {
		u32 nextBumpMtx = 0;
		for (int j = 0; j < param_1->mMaterialNum; ++j) {
			J3DMaterial* mat      = mModelData->getMaterialNodePointer(j);
			J3DNBTScale* nbtScale = mat->getTexGenBlock()->getNBTScale();
			if (nbtScale->mbHasScale == 1) {
				mBumpMtxArr[i][nextBumpMtx] = new Mtx33*[param_3];

				mat->getShape()->unk5C = nextBumpMtx;
				++nextBumpMtx;
			}
		}
	}

	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < param_1->mMaterialNum; ++j) {
			J3DMaterial* mat      = mModelData->getMaterialNodePointer(j);
			J3DNBTScale* nbtScale = mat->getTexGenBlock()->getNBTScale();
			if (nbtScale->mbHasScale == 1) {

				for (int k = 0; k < param_3; ++k)
					mBumpMtxArr[i][j][k]
					    = new (0x20) Mtx33[param_1->getDrawMtxNum()];
			}
		}
	}

	if (totalMatsWithNbtScale != 0) {
		mModelData->unk18 = 1;
	}

	mVertexBuffer = new J3DVertexBuffer(&param_1->mVertexData);
}

void J3DModel::lock()
{
	u16 matNum = mModelData->getMaterialNum();

	for (int i = 0; i < matNum; ++i)
		mMatPackets[i].lock();
}

void J3DModel::unlock()
{
	u16 matNum = mModelData->getMaterialNum();

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

#pragma dont_inline on
void J3DModel::calcWeightEnvelopeMtx() { }
#pragma dont_inline off

void J3DModel::update()
{
	j3dSys.setModel(this);

	if (checkFlag(4)) {
		j3dSys.mFlags |= 0x4;
	} else {
		j3dSys.mFlags &= ~0x4;
	}

	if (checkFlag(0x8)) {
		j3dSys.mFlags |= 0x8;
	} else {
		j3dSys.mFlags &= ~0x8;
	}

	mVertexBuffer->frameInit();

	// TODO: missing unknown virtual calls

	if (mDeformData != NULL)
		mDeformData->deform(this);

	// TODO: missing unknown virtual calls

	j3dSys.setCurrentMtxCalc(mModelData->unk14);
	mModelData->unk14->init(mBaseScale, mBaseMtx);
	j3dSys.setTexture(mModelData->getTexture());
	mModelData->unk14->recursiveCalc(mModelData->mRootNode);

	calcWeightEnvelopeMtx();

	if (mSkinDeform)
		mSkinDeform->deform(this);

	if (unkC)
		unkC(this, 0);
}

void J3DModel::calc()
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

	mVertexBuffer->frameInit();

	// TODO: missing unknown virtual calls

	if (mDeformData != NULL)
		mDeformData->deform(this);

	// TODO: missing unknown virtual calls

	j3dSys.setCurrentMtxCalc(mModelData->unk14);
	mModelData->unk14->init(mBaseScale, mBaseMtx);
	j3dSys.setTexture(mModelData->getTexture());
	mModelData->unk14->recursiveCalc(mModelData->mRootNode);

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
			MTXCopy(unk5C[i],
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
	DCStoreRange(getDrawMtxPtr(), mModelData->getDrawMtxNum() * sizeof(Mtx));
	DCStoreRange(getNrmMtxPtr(), mModelData->getDrawMtxNum() * sizeof(Mtx33));
	prepareShapePackets();
}

void J3DModel::calcNrmMtx()
{
	// TODO: probably a fakematch, the references make 0 sense

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
	for (s32 i = 0; i < getModelData()->getMaterialNum(); i++) {
		J3DMaterial* pMaterial = getModelData()->getMaterialNodePointer(i);
		if (pMaterial->getNBTScale()->mbHasScale == 1) {

			pMaterial->getShape()->calcNBTScale(
			    *pMaterial->getNBTScale()->getScale(), getNrmMtxPtr(),
			    getBumpMtxPtr(nextBumpMtx));

			DCStoreRange(getBumpMtxPtr(nextBumpMtx),
			             getModelData()->getDrawMtxNum() * sizeof(Mtx33));
			nextBumpMtx++;
		}
	}
}

void J3DModel::calcBBoard()
{
	if (getModelData()->checkBBoardFlag()) {
		for (u16 i = 0; i < getModelData()->getDrawMtxNum(); i++) {
			if (getModelData()->getDrawMtxFlag(i) != 0)
				continue;

			u8 mtxType
			    = getModelData()
			          ->getJointNodePointer(getModelData()->getDrawMtxIndex(i))
			          ->getMtxType();
			if (mtxType == J3DJntMtxType_BBoard) {
				Mtx& drawMtx = getDrawMtx(i);

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
			} else if (mtxType == J3DJntMtxType_YBBoard) {
				Mtx& drawMtx = getDrawMtx(i);

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

				Mtx33& nrmMtx = getNrmMtx(i);
				J3DPSCalcInverseTranspose(drawMtx, nrmMtx);
			}
		}
	}
}

void J3DModel::prepareShapePackets()
{
	u16 shapeNum = getModelData()->getShapeNum();

	for (u16 i = 0; i < shapeNum; i++) {
		J3DShapePacket* pkt = &mShapePackets[i];

		J3DShape* shape = mModelData->getShapeNodePointer(i);
		shape->setScaleFlagArray(mScaleFlagArr);

		if (checkFlag(4))
			shape->onFlag(0x4);
		else
			shape->offFlag(0x4);

		if (checkFlag(8) && !shape->checkFlag(16))
			shape->onFlag(0x8);
		else
			shape->offFlag(0x8);

		pkt->unk24 = mVertexBuffer->unk2C;
		pkt->unk28 = mVertexBuffer->unk30;
		pkt->unk2C = mVertexBuffer->unk34;
		pkt->setDrawMtx(mDrawMtxBuf[1]);
		pkt->setNrmMtx(mNrmMtxBuf[1]);
		pkt->setCurrentViewNoPtr(&mCurrentViewNo);
	}

	for (s32 i = 0; i < getModelData()->getMaterialNum(); i++) {
		J3DMaterial* pMaterial = getModelData()->getMaterialNodePointer(i);
		if (pMaterial->getTexGenBlock()->getNBTScale()->mbHasScale == 1) {
			u16 shapeIdx    = pMaterial->getShape()->getIndex();
			u32 bumpMtxOffs = pMaterial->getShape()->getBumpMtxOffset();
			mShapePackets[shapeIdx].setNrmMtx(mBumpMtxArr[1][bumpMtxOffs]);
		}
	}
}
