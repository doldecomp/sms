#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DShape.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTexture.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/JSupport.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DShapeFactory.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DJointFactory.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DMaterialFactory.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DMaterialFactory_v21.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>

J3DModelData* J3DModelLoaderDataBase::load(const void* i_data, u32 i_flags)
{
	if (i_data == nullptr)
		return nullptr;

	const JUTDataFileHeader* fileHeader = (const JUTDataFileHeader*)i_data;

	if (fileHeader->mMagic == 'J3D1' && fileHeader->mType == 'bmd1') {
		return nullptr;
	}

	if (fileHeader->mMagic == 'J3D2' && fileHeader->mType == 'bmd2') {
		J3DModelLoader_v21 loader;
		return loader.load(i_data, i_flags);
	}

	if (fileHeader->mMagic == 'J3D2' && fileHeader->mType == 'bmd3') {
		J3DModelLoader_v26 loader;
		return loader.load(i_data, i_flags);
	}

	return nullptr;
}

J3DMaterialTable* J3DModelLoaderDataBase::loadMaterialTable(const void* i_data)
{
	if (!i_data)
		return nullptr;

	const JUTDataFileHeader* fileHeader = (const JUTDataFileHeader*)i_data;

	if (fileHeader->mMagic == 'J3D1' && fileHeader->mType == 'bmt1') {
		return nullptr;
	}

	if (fileHeader->mMagic == 'J3D2' && fileHeader->mType == 'bmt2') {
		J3DModelLoader_v21 loader;
		return loader.loadMaterialTable(i_data);
	}

	if (fileHeader->mMagic == 'J3D2' && fileHeader->mType == 'bmt3') {
		J3DModelLoader_v26 loader;
		return loader.loadMaterialTable(i_data);
	}
	return nullptr;
}

J3DModelData* J3DModelLoader::load(const void* i_data, u32 i_flags)
{
	mpModelData = new J3DModelData();
	mpModelData->clear();
	mpModelData->unk4               = i_data;
	const JUTDataFileHeader* data   = (JUTDataFileHeader*)i_data;
	u32 totalSize                   = 0;
	const JUTDataBlockHeader* block = &data->mFirstBlock;
	for (u32 block_no = 0; block_no < data->mBlockNum; block_no++) {
		switch (block->mType) {
		case 'INF1':
			readInformation((J3DModelInfoBlock*)block, i_flags);
			break;
		case 'VTX1':
			readVertex((J3DVertexBlock*)block);
			break;
		case 'EVP1':
			readEnvelop((J3DEnvelopBlock*)block);
			break;
		case 'DRW1':
			readDraw((J3DDrawBlock*)block);
			break;
		case 'JNT1':
			readJoint((J3DJointBlock*)block);
			break;
		case 'MAT3':
			readMaterial((J3DMaterialBlock*)block, i_flags);
			break;
		case 'MAT2':
			readMaterial_v21((J3DMaterialBlock_v21*)block, i_flags);
			break;
		case 'SHP1':
			readShape((J3DShapeBlock*)block, i_flags);
			break;
		case 'TEX1':
			readTexture((J3DTextureBlock*)block);
			break;
		default:
			break;
		}
		totalSize += block->mSize;
		block = (const JUTDataBlockHeader*)((u8*)block + block->mSize);
	}
	const J3DModelHierarchy* hierarchy = mpModelData->unk8;
	mpModelData->makeHierarchy(nullptr, &hierarchy);
	setupBBoardInfo();
	return mpModelData;
}

J3DMaterialTable* J3DModelLoader::loadMaterialTable(const void* i_data)
{
	u32 thing       = 0x55100000;
	mpMaterialTable = new J3DMaterialTable();
	mpMaterialTable->clear();
	const JUTDataFileHeader* data   = (JUTDataFileHeader*)i_data;
	const JUTDataBlockHeader* block = &data->mFirstBlock;
	for (u32 block_no = 0; block_no < data->mBlockNum; block_no++) {
		switch (block->mType) {
		case 'MAT3':
			readMaterialTable((J3DMaterialBlock*)block, thing);
			break;
		case 'MAT2':
			readMaterialTable_v21((J3DMaterialBlock_v21*)block, thing);
			break;
		case 'TEX1':
			readTextureTable((J3DTextureBlock*)block);
			break;
		default:
			break;
		}
		block = (const JUTDataBlockHeader*)((u8*)block + block->mSize);
	}

	if (mpMaterialTable->mTexture == nullptr)
		mpMaterialTable->mTexture = new J3DTexture(0, nullptr);

	return mpMaterialTable;
}

void J3DModelLoader::setupBBoardInfo()
{
	for (u16 i = 0; i < mpModelData->getJointNum(); i++) {
		J3DMaterial* mesh = mpModelData->getJointNodePointer(i)->getMesh();
		if (!mesh)
			continue;

		u32 shape_index  = mesh->getShape()->getIndex();
		u16* index_table = JSUConvertOffsetToPtr<u16>(
		    mpShapeBlock, (u32)mpShapeBlock->mpIndexTable);
		J3DShapeInitData* shape_init_data
		    = JSUConvertOffsetToPtr<J3DShapeInitData>(
		        mpShapeBlock, (u32)mpShapeBlock->mpShapeInitData);
		J3DJoint* joint;
		switch (shape_init_data[index_table[shape_index]].mShapeMtxType) {
		case 0:
			joint = mpModelData->getJointNodePointer(i);
			joint->setMtxType(0);
			break;
		case 1:
			joint = mpModelData->getJointNodePointer(i);
			joint->setMtxType(1);
			mpModelData->unk1A = true;
			break;
		case 2:
			joint = mpModelData->getJointNodePointer(i);
			joint->setMtxType(2);
			mpModelData->unk1A = true;
			break;
		case 3:
			joint = mpModelData->getJointNodePointer(i);
			joint->setMtxType(0);
			break;
		default:
			break;
		}
	}
}

void J3DModelLoader::readInformation(const J3DModelInfoBlock* i_block,
                                     u32 i_flags)
{
	mpModelData->unkC = i_flags | i_block->mFlags;
	switch (mpModelData->unkC & 0xF) {
	case 0: // TODO: enum for mtxcalc type (and other load flags)
		mpModelData->unk14 = new J3DMtxCalcBasic();
		break;
	case 1:
		mpModelData->unk14 = new J3DMtxCalcSoftimage();
		break;
	case 2:
		mpModelData->unk14 = new J3DMtxCalcMaya();
		break;
	}
	mpModelData->unk80                = i_block->mPacketNum;
	mpModelData->getVertexData().unk0 = i_block->mVtxNum;
	mpModelData->unk8 = JSUConvertOffsetToPtr<J3DModelHierarchy>(
	    i_block, i_block->mpHierarchy);
}

static GXCompType getFmtType(GXVtxAttrFmtList* i_fmtList, GXAttr i_attr)
{
	for (; i_fmtList->attr != GX_VA_NULL; i_fmtList++) {
		if (i_fmtList->attr == i_attr) {
			return i_fmtList->type;
		}
	}
	return GX_F32;
}

void J3DModelLoader::readVertex(const J3DVertexBlock* i_block)
{
	J3DVertexData& vertex_data  = mpModelData->getVertexData();
	vertex_data.mVtxAttrFmtList = JSUConvertOffsetToPtr<GXVtxAttrFmtList>(
	    i_block, i_block->mpVtxAttrFmtList);
	vertex_data.mVtxPosArray
	    = JSUConvertOffsetToPtr<void>(i_block, i_block->mpVtxPosArray);
	vertex_data.mVtxNormArray
	    = JSUConvertOffsetToPtr<void>(i_block, i_block->mpVtxNrmArray);
	vertex_data.mVtxNBTArray
	    = JSUConvertOffsetToPtr<void>(i_block, i_block->mpVtxNBTArray);
	for (int i = 0; i < 2; i++) {
		vertex_data.mVtxColorArray[i] = (GXColor*)JSUConvertOffsetToPtr<void>(
		    i_block, i_block->mpVtxColorArray[i]);
	}
	for (int i = 0; i < 8; i++) {
		vertex_data.mVtxTexCoordArray[i] = JSUConvertOffsetToPtr<void>(
		    i_block, i_block->mpVtxTexCoordArray[i]);
	}

	void* nrm_end = NULL;
	if (vertex_data.mVtxNBTArray != NULL) {
		nrm_end = vertex_data.mVtxNBTArray;
	} else if (vertex_data.mVtxColorArray[0] != NULL) {
		nrm_end = vertex_data.mVtxColorArray[0];
	} else if (vertex_data.mVtxTexCoordArray[0] != NULL) {
		nrm_end = vertex_data.mVtxTexCoordArray[0];
	}

	if (vertex_data.mVtxNormArray == NULL) {
		vertex_data.unk4 = 0;
	} else if (nrm_end != NULL) {
		vertex_data.unk4
		    = ((u32)nrm_end - (u32)vertex_data.mVtxNormArray) / 12 + 1;
	} else {
		vertex_data.unk4
		    = (i_block->mSize - (u32)i_block->mpVtxNrmArray) / 12 + 1;
	}

	void* color0_end = NULL;
	if (vertex_data.mVtxColorArray[1] != NULL) {
		color0_end = vertex_data.mVtxColorArray[1];
	} else if (vertex_data.mVtxTexCoordArray[0] != NULL) {
		color0_end = vertex_data.mVtxTexCoordArray[0];
	}

	if (vertex_data.mVtxColorArray[0] == NULL) {
		vertex_data.unk8 = 0;
	} else if (color0_end != NULL) {
		vertex_data.unk8
		    = ((u32)color0_end - (u32)vertex_data.mVtxColorArray[0]) / 4 + 1;
	} else {
		vertex_data.unk8
		    = (i_block->mSize - (u32)i_block->mpVtxColorArray[0]) / 4 + 1;
	}
}

void J3DModelLoader::readEnvelop(const J3DEnvelopBlock* i_block)
{
	mpModelData->mWEvlpMtxNum = i_block->mWEvlpMtxNum;
	mpModelData->unk88
	    = JSUConvertOffsetToPtr<u8>(i_block, i_block->mpWEvlpMixMtxNum);
	mpModelData->unk8C
	    = JSUConvertOffsetToPtr<u16>(i_block, i_block->mpWEvlpMixMtxIndex);
	mpModelData->unk90
	    = JSUConvertOffsetToPtr<f32>(i_block, i_block->mpWEvlpMixWeight);
	mpModelData->unk94
	    = JSUConvertOffsetToPtr<Mtx>(i_block, i_block->mpInvJointMtx);
}

void J3DModelLoader::readDraw(const J3DDrawBlock* i_block)
{
	J3DDrawMtxData& data = mpModelData->getDrawMtxData();
	data.mEntryNum       = i_block->mMtxNum;
	data.mDrawMtxFlag
	    = JSUConvertOffsetToPtr<u8>(i_block, i_block->mpDrawMtxFlag);
	data.mDrawMtxIndex
	    = JSUConvertOffsetToPtr<u16>(i_block, i_block->mpDrawMtxIndex);
	u16 i;
	for (i = 0; i < data.mEntryNum; i++) {
		if (data.mDrawMtxFlag[i] == 1) {
			break;
		}
	}
	data.mDrawFullWgtMtxNum = i;
}

void J3DModelLoader::readJoint(const J3DJointBlock* i_block)
{
	J3DJointFactory factory(*i_block);
	mpModelData->mJointNum = i_block->mJointNum;
	if (i_block->mpNameTable != nullptr) {
		mpModelData->unkB0 = new JUTNameTab(
		    JSUConvertOffsetToPtr<ResNTAB>(i_block, i_block->mpNameTable));
	} else {
		mpModelData->unkB0 = nullptr;
	}
	mpModelData->mJointNodePointer = new J3DJoint*[mpModelData->mJointNum];
	for (u16 i = 0; i < mpModelData->mJointNum; i++) {
		mpModelData->mJointNodePointer[i] = factory.create(i);
	}
}

void J3DModelLoader_v26::readMaterial(const J3DMaterialBlock* i_block,
                                      u32 i_flags)
{
	J3DMaterialFactory factory(*i_block);
	mpModelData->mMaterialNum = i_block->mMaterialNum;
	mpModelData->unk34        = factory.countUniqueMaterials();
	if (i_block->mpNameTable != nullptr) {
		mpModelData->mMaterialName = new JUTNameTab(
		    JSUConvertOffsetToPtr<ResNTAB>(i_block, i_block->mpNameTable));
	} else {
		mpModelData->mMaterialName = nullptr;
	}
	mpModelData->mMaterials = new J3DMaterial*[mpModelData->mMaterialNum];
	if (i_flags & 0x200000) {
		mpModelData->unk38 = new (0x20) J3DMaterial[mpModelData->unk34];
	} else {
		mpModelData->unk38 = nullptr;
	}
	if (i_flags & 0x200000) {
		for (u16 i = 0; i < mpModelData->unk34; i++) {
			factory.create(&mpModelData->unk38[i], i, i_flags);
			mpModelData->unk38[i].unk18 = (u32)&mpModelData->unk38[i] >> 4;
		}
	}
	for (u16 i = 0; i < mpModelData->mMaterialNum; i++) {
		mpModelData->mMaterials[i] = factory.create(nullptr, i, i_flags);
	}
	if (i_flags & 0x200000) {
		for (u16 i = 0; i < mpModelData->mMaterialNum; i++) {
			mpModelData->mMaterials[i]->unk18
			    = (u32)&mpModelData->unk38[factory.getMaterialID(i)] >> 4;
			mpModelData->mMaterials[i]->mOriginalMaterial
			    = &mpModelData->unk38[factory.getMaterialID(i)];
		}
	} else {
		for (u16 i = 0; i < mpModelData->mMaterialNum; i++) {
			mpModelData->mMaterials[i]->unk18 = 0xc0000000;
		}
	}
}

void J3DModelLoader_v21::readMaterial_v21(const J3DMaterialBlock_v21* i_block,
                                          u32 i_flags)
{
	J3DMaterialFactory_v21 factory(*i_block);
	mpModelData->mMaterialNum = i_block->mMaterialNum;
	mpModelData->unk34        = factory.countUniqueMaterials();
	if (i_block->mpNameTable != nullptr) {
		mpModelData->mMaterialName = new JUTNameTab(
		    JSUConvertOffsetToPtr<ResNTAB>(i_block, i_block->mpNameTable));
	} else {
		mpModelData->mMaterialName = nullptr;
	}
	mpModelData->mMaterials = new J3DMaterial*[mpModelData->mMaterialNum];
	if (i_flags & 0x200000) {
		mpModelData->unk38 = new (0x20) J3DMaterial[mpModelData->unk34];
	} else {
		mpModelData->unk38 = nullptr;
	}
	if (i_flags & 0x200000) {
		for (u16 i = 0; i < mpModelData->unk34; i++) {
			factory.create(&mpModelData->unk38[i], i, i_flags);
			mpModelData->unk38[i].unk18 = (u32)&mpModelData->unk38[i] >> 4;
		}
	}
	for (u16 i = 0; i < mpModelData->mMaterialNum; i++) {
		mpModelData->mMaterials[i] = factory.create(nullptr, i, i_flags);
	}
	if (i_flags & 0x200000) {
		for (u16 i = 0; i < mpModelData->mMaterialNum; i++) {
			mpModelData->mMaterials[i]->unk18
			    = (u32)&mpModelData->unk38[factory.getMaterialID(i)] >> 4;
			mpModelData->mMaterials[i]->mOriginalMaterial
			    = &mpModelData->unk38[factory.getMaterialID(i)];
		}
	} else {
		for (u16 i = 0; i < mpModelData->mMaterialNum; i++) {
			mpModelData->mMaterials[i]->unk18 = 0xC0000000;
		}
	}
}

void J3DModelLoader::readShape(const J3DShapeBlock* i_block, u32 i_flags)
{
	mpShapeBlock = i_block;
	J3DShapeFactory factory(*i_block);
	mpModelData->mShapeNum = i_block->mShapeNum;
	if (i_block->mpNameTable != nullptr) {
		mpModelData->unkB8 = new JUTNameTab(
		    JSUConvertOffsetToPtr<ResNTAB>(i_block, i_block->mpNameTable));
	} else {
		mpModelData->unkB8 = nullptr;
	}
	mpModelData->mShapeNodePointer = new J3DShape*[mpModelData->mShapeNum];
	factory.allocVcdVatCmdBuffer(mpModelData->mShapeNum);
	const J3DModelHierarchy* hierarchy_entry = mpModelData->unk8;
	GXVtxDescList* vtx_desc_list             = nullptr;
	for (; hierarchy_entry->mType != 0; hierarchy_entry++) {
		if (hierarchy_entry->mType == 0x12) {
			mpModelData->mShapeNodePointer[hierarchy_entry->mValue]
			    = factory.create(hierarchy_entry->mValue,
			                     (J3DMdlDataFlag)i_flags, vtx_desc_list);
			vtx_desc_list = factory.getVtxDescList(hierarchy_entry->mValue);
		}
	}
}

void J3DModelLoader::readTexture(const J3DTextureBlock* i_block)
{
	u16 texture_num = i_block->mTextureNum;
	ResTIMG* texture_res
	    = JSUConvertOffsetToPtr<ResTIMG>(i_block, i_block->mpTextureRes);
	if (i_block->mpNameTable != nullptr) {
		mpModelData->unkA8 = new JUTNameTab(
		    JSUConvertOffsetToPtr<ResNTAB>(i_block, i_block->mpNameTable));
	} else {
		mpModelData->unkA8 = nullptr;
	}
	mpModelData->unkAC = new J3DTexture(texture_num, texture_res);
}

void J3DModelLoader_v26::readMaterialTable(const J3DMaterialBlock* i_block,
                                           u32 i_flags)
{
	J3DMaterialFactory factory(*i_block);
	mpMaterialTable->mMaterialNum = i_block->mMaterialNum;
	if (i_block->mpNameTable != nullptr) {
		mpMaterialTable->mMaterialName = new JUTNameTab(
		    JSUConvertOffsetToPtr<ResNTAB>(i_block, i_block->mpNameTable));
	} else {
		mpMaterialTable->mMaterialName = nullptr;
	}
	mpMaterialTable->mMaterials
	    = new J3DMaterial*[mpMaterialTable->mMaterialNum];
	for (u16 i = 0; i < mpMaterialTable->mMaterialNum; i++) {
		mpMaterialTable->mMaterials[i] = factory.create(nullptr, i, i_flags);
	}
	for (u16 i = 0; i < mpMaterialTable->mMaterialNum; i++) {
		mpMaterialTable->mMaterials[i]->unk18
		    = (u32)mpMaterialTable->mMaterials + factory.getMaterialID(i);
	}
}

void J3DModelLoader_v21::readMaterialTable_v21(
    const J3DMaterialBlock_v21* i_block, u32 i_flags)
{
	J3DMaterialFactory_v21 factory(*i_block);
	mpMaterialTable->mMaterialNum = i_block->mMaterialNum;
	if (i_block->mpNameTable != nullptr) {
		mpMaterialTable->mMaterialName = new JUTNameTab(
		    JSUConvertOffsetToPtr<ResNTAB>(i_block, i_block->mpNameTable));
	} else {
		mpMaterialTable->mMaterialName = nullptr;
	}
	mpMaterialTable->mMaterials
	    = new J3DMaterial*[mpMaterialTable->mMaterialNum];
	for (u16 i = 0; i < mpMaterialTable->mMaterialNum; i++) {
		mpMaterialTable->mMaterials[i] = factory.create(nullptr, i, i_flags);
	}
	for (u16 i = 0; i < mpMaterialTable->mMaterialNum; i++) {
		mpMaterialTable->mMaterials[i]->unk18
		    = ((u32)mpMaterialTable->mMaterials) + factory.getMaterialID(i);
	}
}

void J3DModelLoader::readTextureTable(const J3DTextureBlock* i_block)
{
	u16 texture_num = i_block->mTextureNum;
	ResTIMG* texture_res
	    = JSUConvertOffsetToPtr<ResTIMG>(i_block, i_block->mpTextureRes);
	if (i_block->mpNameTable != nullptr) {
		mpMaterialTable->mTextureName = new JUTNameTab(
		    JSUConvertOffsetToPtr<ResNTAB>(i_block, i_block->mpNameTable));
	} else {
		mpMaterialTable->mTextureName = nullptr;
	}
	mpMaterialTable->mTexture = new J3DTexture(texture_num, texture_res);
}
