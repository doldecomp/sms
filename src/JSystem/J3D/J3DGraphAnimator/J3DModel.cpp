#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DShape.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTexture.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DMaterialAnm.hpp>

#pragma opt_strength_reduction off

void J3DModelData::clear()
{
	unk4              = 0;
	unk8              = 0;
	unkC              = 0;
	mRootNode         = 0;
	unk14             = 0;
	unk18             = 0;
	unk1A             = 0;
	unk1C             = 0;
	mJointNodePointer = (J3DJoint**)0x0;
	unk24             = 0;
	mMaterials        = (J3DMaterial**)0x0;
	unk2C             = 0;
	mShapes           = nullptr;
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
		case kTypeJoint: {
			J3DJoint** pJoints = mJointNodePointer;
			newNode            = pJoints[(++*hierarchy)->mValue];
		} break;
		case kTypeMaterial: {
			newMaterial = mMaterials[(++*hierarchy)->mValue];
		} break;
		case kTypeShape:;
			newShape = mShapes[(++*hierarchy)->mValue];
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

				pMaterial->getTexCoord(texMtxID)->setTexGenMtx(GX_TEXMTX0
				                                               + texMtxID * 3);

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

	unkC        = (void*)0x0;
	unk8        = 0;
	mModelData  = nullptr;
	unk88       = 0;
	mSkinDeform = nullptr;
	unk14       = 1.0;
	unk18       = 1.0;
	unk1C       = 1.0;
	MTXIdentity(unk20);
	mScaleFlagArr = (u8*)0x0;
	unk54         = (void*)0x0;
	mNodeMatrices = (Mtx*)0x0;
	unk5C         = (void*)0x0;
	unk60[0]      = (void*)0x0;
	unk60[1]      = (void*)0x0;
	unk68[0]      = (void*)0x0;
	unk68[1]      = (void*)0x0;
	unk70[0]      = (void*)0x0;
	unk70[1]      = (void*)0x0;
	unk7C         = (void*)0x0;
	mVertexBuffer = (J3DVertexBuffer*)0x0;
	mMatPackets   = (J3DMatPacket*)0x0;
	mShapePackets = (J3DShapePacket*)0x0;
	unk9C         = (void*)0x0;
	unk90         = (void*)0x0;
	unk94         = (void*)0x0;
}

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
