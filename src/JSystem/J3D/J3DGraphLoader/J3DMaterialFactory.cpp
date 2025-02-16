#include <JSystem/J3D/J3DGraphLoader/J3DMaterialFactory.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphBase/Blocks/J3DColorBlocks.hpp>
#include <JSystem/J3D/J3DGraphBase/Blocks/J3DPEBlocks.hpp>
#include <JSystem/J3D/J3DGraphBase/Blocks/J3DIndBlocks.hpp>
#include <JSystem/JSupport.hpp>

J3DMaterialFactory::J3DMaterialFactory(const J3DMaterialBlock& block)
{

	mMaterialNum       = block.mMaterialNum;
	mpMaterialInitData = JSUConvertOffsetToPtr<J3DMaterialInitData>(
	    &block, block.mpMaterialInitData);
	mpMaterialID = JSUConvertOffsetToPtr<u16>(&block, block.mpMaterialID);

	if (block.mpIndInitData != nullptr
	    && ((u32)block.mpIndInitData - (u32)block.mpNameTable) > 4)
		mpIndInitData = JSUConvertOffsetToPtr<J3DIndInitData>(
		    &block, block.mpIndInitData);
	else
		mpIndInitData = nullptr;

	mpCullMode = JSUConvertOffsetToPtr<GXCullMode>(&block, block.mpCullMode);
	mpMatColor = JSUConvertOffsetToPtr<GXColor>(&block, block.mpMatColor);
	mpColorChanNum  = JSUConvertOffsetToPtr<u8>(&block, block.mpColorChanNum);
	mpColorChanInfo = JSUConvertOffsetToPtr<J3DColorChanInfo>(
	    &block, block.mpColorChanInfo);
	mpAmbColor = JSUConvertOffsetToPtr<GXColor>(&block, block.mpAmbColor);
	mpLightInfo
	    = JSUConvertOffsetToPtr<J3DLightInfo>(&block, block.mpLightInfo);
	mpTexGenNum = JSUConvertOffsetToPtr<u8>(&block, block.mpTexGenNum);
	mpTexCoordInfo
	    = JSUConvertOffsetToPtr<J3DTexCoordInfo>(&block, block.mpTexCoordInfo);
	mpTexCoord2Info = JSUConvertOffsetToPtr<J3DTexCoord2Info>(
	    &block, block.mpTexCoord2Info);
	mpTexMtxInfo
	    = JSUConvertOffsetToPtr<J3DTexMtxInfo>(&block, block.mpTexMtxInfo);
	field_0x44 = JSUConvertOffsetToPtr<J3DTexMtxInfo>(&block, block.field_0x44);
	mpTexNo    = JSUConvertOffsetToPtr<u16>(&block, block.mpTexNo);
	mpTevOrderInfo
	    = JSUConvertOffsetToPtr<J3DTevOrderInfo>(&block, block.mpTevOrderInfo);
	mpTevColor    = JSUConvertOffsetToPtr<GXColorS10>(&block, block.mpTevColor);
	mpTevKColor   = JSUConvertOffsetToPtr<GXColor>(&block, block.mpTevKColor);
	mpTevStageNum = JSUConvertOffsetToPtr<u8>(&block, block.mpTevStageNum);
	mpTevStageInfo
	    = JSUConvertOffsetToPtr<J3DTevStageInfo>(&block, block.mpTevStageInfo);
	mpTevSwapModeInfo = JSUConvertOffsetToPtr<J3DTevSwapModeInfo>(
	    &block, block.mpTevSwapModeInfo);
	mpTevSwapModeTableInfo = JSUConvertOffsetToPtr<J3DTevSwapModeTableInfo>(
	    &block, block.mpTevSwapModeTableInfo);
	mpFogInfo = JSUConvertOffsetToPtr<J3DFogInfo>(&block, block.mpFogInfo);
	mpAlphaCompInfo = JSUConvertOffsetToPtr<J3DAlphaCompInfo>(
	    &block, block.mpAlphaCompInfo);
	mpBlendInfo
	    = JSUConvertOffsetToPtr<J3DBlendInfo>(&block, block.mpBlendInfo);
	mpZModeInfo
	    = JSUConvertOffsetToPtr<J3DZModeInfo>(&block, block.mpZModeInfo);
	mpZCompLoc = JSUConvertOffsetToPtr<u8>(&block, block.mpZCompLoc);
	mpDither   = JSUConvertOffsetToPtr<u8>(&block, block.mpDither);
	mpNBTScaleInfo
	    = JSUConvertOffsetToPtr<J3DNBTScaleInfo>(&block, block.mpNBTScaleInfo);
}

u16 J3DMaterialFactory::countUniqueMaterials()
{
	u16 i;
	for (i = 0; i < mMaterialNum; ++i) {
		if (mpMaterialID[i] < i) {
			break;
		}
	}
	return i;
}

u32 J3DMaterialFactory::countTexGens(int idx) const
{
	u8 no = mpMaterialInitData[mpMaterialID[idx]].mTexGenNumIdx;
	if (no != 0xFF)
		return mpTexGenNum[no];
	return 0;
}

u32 J3DMaterialFactory::countStages(int idx) const
{
	J3DMaterialInitData* initData = &mpMaterialInitData[mpMaterialID[idx]];

	u32 texGenNum   = 0;
	u32 tevStageNum = 0;

	u8 no = initData->mTevStageNumIdx;
	if (no != 0xFF)
		tevStageNum = mpTevStageNum[no];

	for (s32 i = 0; i < 8; i++)
		if (initData->mTexNoIdx[i] != 0xFFFF)
			texGenNum++;

	if (tevStageNum != texGenNum && texGenNum != 0) {
		return tevStageNum > texGenNum ? tevStageNum : texGenNum;
	} else {
		return tevStageNum;
	}
}

J3DMaterial* J3DMaterialFactory::create(J3DMaterial* mat, int idx,
                                        u32 flag) const
{
	u32 stageNum = countStages(idx);

	u32 tevStageNum
	    = stageNum > ((flag >> 16) & 0x1F) ? stageNum : ((flag >> 16) & 0x1F);

	u32 texNum = tevStageNum > 8 ? 8 : tevStageNum;

	u32 colorFlag  = flag & 0x40000000 ? 0 : 1;
	u32 texGenFlag = flag & 0x04000000 ? 0 : 1;
	u32 peFlag     = flag & 0x10000000 ? 0 : 1;
	u32 indFlag    = flag & 0x01000000 ? 1 : 0;

	if (mat == nullptr)
		mat = new J3DMaterial();

	mat->mColorBlock  = J3DMaterial::createColorBlock(colorFlag);
	mat->mTexGenBlock = J3DMaterial::createTexGenBlock(texGenFlag);
	mat->mTevBlock    = J3DMaterial::createTevBlock((u16)tevStageNum);
	mat->mIndBlock    = J3DMaterial::createIndBlock(indFlag);
	mat->mPEBlock = J3DMaterial::createPEBlock(peFlag, getMaterialMode(idx));

	mat->unkC = idx;
	mat->unk8 = getMaterialMode(idx);
	mat->mColorBlock->setColorChanNum(newColorChanNum(idx));
	mat->mColorBlock->setCullMode(newCullMode(idx));
	mat->mTexGenBlock->setTexGenNum(newTexGenNum(idx));
	mat->mTexGenBlock->setNBTScale(newNBTScale(idx));
	mat->mPEBlock->setFog(newFog(idx));
	mat->mPEBlock->setAlphaComp(newAlphaComp(idx));
	mat->mPEBlock->setBlend(newBlend(idx));
	mat->mPEBlock->setZMode(newZMode(idx));
	mat->mPEBlock->setZCompLoc(newZCompLoc(idx));
	mat->mPEBlock->setDither(newDither(idx));
	mat->mTevBlock->setTevStageNum(newTevStageNum(idx));

	for (u8 i = 0; i < texNum; i++)
		mat->mTevBlock->setTexNo(i, newTexNo(idx, i));
	for (u8 i = 0; i < tevStageNum; i++)
		mat->mTevBlock->setTevOrder(i, newTevOrder(idx, i));
	for (u8 i = 0; i < tevStageNum; i++) {
		J3DMaterialInitData* initData = &mpMaterialInitData[mpMaterialID[idx]];
		mat->mTevBlock->setTevStage(i, newTevStage(idx, i));
		if (initData->mTevSwapModeIdx[i] != 0xFFFF) {
			mat->mTevBlock->getTevStage(i)->setTexSel(
			    mpTevSwapModeInfo[initData->mTevSwapModeIdx[i]].mTexSel);
			mat->mTevBlock->getTevStage(i)->setRasSel(
			    mpTevSwapModeInfo[initData->mTevSwapModeIdx[i]].mRasSel);
		}
	}
	for (u8 i = 0; i < 4; i++)
		mat->mTevBlock->setTevKColor(i, newTevKColor(idx, i));
	for (u8 i = 0; i < 4; i++)
		mat->mTevBlock->setTevColor(i, newTevColor(idx, i));
	for (u8 i = 0; i < 4; i++)
		mat->mTevBlock->setTevSwapModeTable(i, newTevSwapModeTable(idx, i));
	for (u8 i = 0; i < 2; i++)
		mat->mColorBlock->setAmbColor(i, newAmbColor(idx, i));
	for (u8 i = 0; i < 2; i++)
		mat->mColorBlock->setMatColor(i, newMatColor(idx, i));
	for (u8 i = 0; i < 4; i++) {
		J3DColorChan colorChan = newColorChan(idx, i);
		mat->mColorBlock->setColorChan(i, colorChan);
	}
	for (u8 i = 0; i < 8; i++) {
		J3DTexCoord texCoord = newTexCoord(idx, i);
		mat->mTexGenBlock->setTexCoord(i, &texCoord);
	}
	for (u8 i = 0; i < 8; i++) {
		mat->mTexGenBlock->setTexMtx(i, newTexMtx(idx, i));
	}

	J3DMaterialInitData* initData = &mpMaterialInitData[mpMaterialID[idx]];
	for (u8 i = 0; i < tevStageNum; i++)
		mat->mTevBlock->setTevKColorSel(i, initData->mTevKColorSel[i]);
	for (u8 i = 0; i < tevStageNum; i++)
		mat->mTevBlock->setTevKAlphaSel(i, initData->mTevKAlphaSel[i]);

	if (mpIndInitData != nullptr) {
		u8 indTexStageNum = newIndTexStageNum(idx);
		mat->mIndBlock->setIndTexStageNum(newIndTexStageNum(idx));
		for (u8 i = 0; i < indTexStageNum; i++)
			mat->mIndBlock->setIndTexMtx(i, newIndTexMtx(idx, i));
		for (u8 i = 0; i < indTexStageNum; i++)
			mat->mIndBlock->setIndTexOrder(i, newIndTexOrder(idx, i));
		for (u8 i = 0; i < indTexStageNum; i++)
			mat->mIndBlock->setIndTexCoordScale(i, newIndTexCoordScale(idx, i));
		for (u8 i = 0; i < tevStageNum; i++)
			mat->mTevBlock->setIndTevStage(i, newIndTevStage(idx, i));
	}
	return mat;
}

J3DGXColor J3DMaterialFactory::newMatColor(int idx, int stage) const
{
	J3DGXColor ret((GXColor) { 0xFF, 0xFF, 0xFF, 0xFF });
	J3DMaterialInitData* initData = &mpMaterialInitData[mpMaterialID[idx]];
	u16 no = mpMaterialInitData[mpMaterialID[idx]].mMatColorIdx[stage];
	if (no != 0xFFFF)
		return mpMatColor[no];
	else
		return ret;
}

u8 J3DMaterialFactory::newColorChanNum(int idx) const
{
	u8 no = mpMaterialInitData[mpMaterialID[idx]].mColorChanNumIdx;
	if (no != 0xFF)
		return mpColorChanNum[no];
	else
		return 0;
}

J3DColorChan J3DMaterialFactory::newColorChan(int idx, int stage) const
{
	u16 no = mpMaterialInitData[mpMaterialID[idx]].mColorChanIdx[stage];
	if (no != 0xFFFF)
		return J3DColorChan(mpColorChanInfo[no]);
	else
		return J3DColorChan();
}

J3DGXColor J3DMaterialFactory::newAmbColor(int idx, int stage) const
{
	J3DGXColor ret((GXColor) { 0x32, 0x32, 0x32, 0x32 });

	J3DMaterialInitData* initData = &mpMaterialInitData[mpMaterialID[idx]];
	u16 no = mpMaterialInitData[mpMaterialID[idx]].mAmbColorIdx[stage];
	if (no != 0xFFFF)
		return mpAmbColor[no];
	else
		return ret;
}

u32 J3DMaterialFactory::newTexGenNum(int idx) const
{
	u8 no = mpMaterialInitData[mpMaterialID[idx]].mTexGenNumIdx;
	if (no != 0xFF)
		return mpTexGenNum[no];
	else
		return 0;
}

J3DTexCoord J3DMaterialFactory::newTexCoord(int idx, int stage) const
{
	u16 no = mpMaterialInitData[mpMaterialID[idx]].mTexCoordIdx[stage];
	if (no != 0xFFFF)
		return J3DTexCoord(mpTexCoordInfo[no]);
	else
		return J3DTexCoord();
}

J3DTexMtx* J3DMaterialFactory::newTexMtx(int idx, int stage) const
{
	J3DTexMtx* ret = nullptr;

	J3DMaterialInitData* initData = &mpMaterialInitData[mpMaterialID[idx]];
	if (initData->mTexMtxIdx[stage] != 0xFFFF)
		ret = new J3DTexMtx(mpTexMtxInfo[initData->mTexMtxIdx[stage]]);
	return ret;
}

u8 J3DMaterialFactory::newCullMode(int idx) const
{
	u8 no = mpMaterialInitData[mpMaterialID[idx]].mCullModeIdx;
	if (no != 0xFF)
		return mpCullMode[no];
	else
		return 0xFF;
}

u16 J3DMaterialFactory::newTexNo(int idx, int stage) const
{
	u16 no = mpMaterialInitData[mpMaterialID[idx]].mTexNoIdx[stage];
	if (no != 0xFFFF)
		return mpTexNo[no];
	else
		return 0xFFFF;
}

J3DTevOrder J3DMaterialFactory::newTevOrder(int idx, int stage) const
{
	u16 no = mpMaterialInitData[mpMaterialID[idx]].mTevOrderIdx[stage];
	if (no != 0xFFFF)
		return J3DTevOrder(mpTevOrderInfo[no]);
	else
		return J3DTevOrder();
}

J3DGXColorS10 J3DMaterialFactory::newTevColor(int idx, int stage) const
{
	J3DGXColorS10 ret((GXColorS10) { 0x00, 0x00, 0x00, 0x00 });
	J3DMaterialInitData* initData = &mpMaterialInitData[mpMaterialID[idx]];
	u16 no = mpMaterialInitData[mpMaterialID[idx]].mTevColorIdx[stage];
	if (no != 0xFFFF)
		return mpTevColor[no];
	else
		return ret;
}

J3DGXColor J3DMaterialFactory::newTevKColor(int idx, int stage) const
{
	J3DGXColor ret((GXColor) { 0xFF, 0xFF, 0xFF, 0xFF });
	J3DMaterialInitData* initData = &mpMaterialInitData[mpMaterialID[idx]];
	u16 no = mpMaterialInitData[mpMaterialID[idx]].mTevKColorIdx[stage];
	if (no != 0xFFFF)
		return mpTevKColor[no];
	else
		return ret;
}

u8 J3DMaterialFactory::newTevStageNum(int idx) const
{
	u8 no = mpMaterialInitData[mpMaterialID[idx]].mTevStageNumIdx;
	if (no != 0xFF)
		return mpTevStageNum[no];
	else
		return 0xFF;
}

J3DTevStage J3DMaterialFactory::newTevStage(int idx, int stage) const
{
	u16 no = mpMaterialInitData[mpMaterialID[idx]].mTevStageIdx[stage];
	if (no != 0xFFFF)
		return J3DTevStage(mpTevStageInfo[no]);
	else
		return J3DTevStage();
}

J3DTevSwapModeTable J3DMaterialFactory::newTevSwapModeTable(int idx,
                                                            int stage) const
{
	u16 no = mpMaterialInitData[mpMaterialID[idx]].mTevSwapModeTableIdx[stage];
	if (no != 0xFFFF)
		return J3DTevSwapModeTable(mpTevSwapModeTableInfo[no]);
	else
		return J3DTevSwapModeTable(j3dDefaultTevSwapModeTable);
}

u8 J3DMaterialFactory::newIndTexStageNum(int idx) const
{
	if (mpIndInitData[idx].mEnabled == true)
		return mpIndInitData[idx].mIndTexStageNum;
	else
		return 0;
}

J3DIndTexOrder J3DMaterialFactory::newIndTexOrder(int idx, int stage) const
{
	J3DIndTexOrder ret;
	if (mpIndInitData[idx].mEnabled == true)
		return J3DIndTexOrder(mpIndInitData[idx].mIndTexOrderInfo[stage]);
	else
		return ret;
}

J3DIndTexMtx J3DMaterialFactory::newIndTexMtx(int idx, int stage) const
{
	J3DIndTexMtx ret;
	if (mpIndInitData[idx].mEnabled == true)
		return J3DIndTexMtx(mpIndInitData[idx].mIndTexMtxInfo[stage]);
	else
		return ret;
}

J3DIndTevStage J3DMaterialFactory::newIndTevStage(int idx, int stage) const
{
	J3DIndTevStage ret;
	if (mpIndInitData[idx].mEnabled == true)
		return J3DIndTevStage(mpIndInitData[idx].mIndTevStageInfo[stage]);
	else
		return ret;
}

J3DIndTexCoordScale J3DMaterialFactory::newIndTexCoordScale(int idx,
                                                            int stage) const
{
	J3DIndTexCoordScale ret;
	if (mpIndInitData[idx].mEnabled == true)
		return J3DIndTexCoordScale(
		    mpIndInitData[idx].mIndTexCoordScaleInfo[stage]);
	else
		return ret;
}

J3DFog* J3DMaterialFactory::newFog(int idx) const
{
	J3DFog* ret                   = nullptr;
	J3DMaterialInitData* initData = &mpMaterialInitData[mpMaterialID[idx]];
	if (initData->mFogIdx != 0xFFFF)
		ret = new J3DFog(mpFogInfo[initData->mFogIdx]);
	return ret;
}

J3DAlphaComp J3DMaterialFactory::newAlphaComp(int idx) const
{
	u16 no = mpMaterialInitData[mpMaterialID[idx]].mAlphaCompIdx;
	if (no != 0xFFFF)
		return J3DAlphaComp(mpAlphaCompInfo[no]);
	else
		return J3DAlphaComp(0xFFFF);
}

J3DBlend J3DMaterialFactory::newBlend(int idx) const
{
	u16 no = mpMaterialInitData[mpMaterialID[idx]].mBlendIdx;
	if (no != 0xFFFF)
		return J3DBlend(mpBlendInfo[no]);
	else
		return J3DBlend();
}

J3DZMode J3DMaterialFactory::newZMode(int idx) const
{
	u8 no = mpMaterialInitData[mpMaterialID[idx]].mZModeIdx;
	if (no != 0xFF)
		return J3DZMode(mpZModeInfo[no]);
	else
		return J3DZMode(0xffff);
}

u8 J3DMaterialFactory::newZCompLoc(int idx) const
{
	u8 no = mpMaterialInitData[mpMaterialID[idx]].mZCompLocIdx;
	if (no != 0xFF)
		return mpZCompLoc[no];
	else
		return 0xFF;
}

u8 J3DMaterialFactory::newDither(int idx) const
{
	u8 no = mpMaterialInitData[mpMaterialID[idx]].mDitherIdx;
	if (no != 0xFF)
		return mpDither[no];
	else
		return 0xFF;
}

J3DNBTScale J3DMaterialFactory::newNBTScale(int idx) const
{
	J3DNBTScale ret;
	u16 no = mpMaterialInitData[mpMaterialID[idx]].mNBTScaleIdx;
	if (no != 0xFFFF)
		return J3DNBTScale(mpNBTScaleInfo[no]);
	else
		return ret;
}
