#include <JSystem/J3D/J3DGraphLoader/J3DShapeFactory.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DShape.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <JSystem/JSupport.hpp>

J3DShapeFactory::J3DShapeFactory(const J3DShapeBlock& block)
{
	mpShapeInitData = JSUConvertOffsetToPtr<J3DShapeInitData>(
	    &block, (u32)block.mpShapeInitData);
	mpIndexTable  = JSUConvertOffsetToPtr<u16>(&block, (u32)block.mpIndexTable);
	mpVtxDescList = JSUConvertOffsetToPtr<GXVtxDescList>(
	    &block, (u32)block.mpVtxDescList);
	mpMtxTable = JSUConvertOffsetToPtr<u16>(&block, (u32)block.mpMtxTable);
	mpDisplayListData
	    = JSUConvertOffsetToPtr<u8>(&block, (u32)block.mpDisplayListData);
	mpMtxInitData = JSUConvertOffsetToPtr<J3DShapeMtxInitData>(
	    &block, (u32)block.mpMtxInitData);
	mpDrawInitData = JSUConvertOffsetToPtr<J3DShapeDrawInitData>(
	    &block, (u32)block.mpDrawInitData);
	mpVcdVatCmdBuffer = nullptr;
}

J3DShape* J3DShapeFactory::create(int no, J3DMdlDataFlag flag,
                                  GXVtxDescList* vtxDesc)
{
	J3DShape* shape      = new J3DShape();
	shape->mElementCount = getMtxGroupNum(no);
	shape->unkC          = getRadius(no);
	shape->unk2C         = getVtxDescList(no);
	shape->mMatrices     = new J3DShapeMtx*[shape->mElementCount];
	shape->mDraws        = new J3DShapeDraw*[shape->mElementCount];
	shape->unk10         = getMin(no);
	shape->unk1C         = getMax(no);
	shape->mGDCommands   = mpVcdVatCmdBuffer + no * J3DShape::kVcdVatDLSize;

	for (s32 i = 0; i < shape->mElementCount; i++) {
		shape->mMatrices[i] = newShapeMtx(no, i);
		shape->mDraws[i]    = newShapeDraw(no, i, flag);
	}

	shape->unk4 = no;
	return shape;
}

enum {
	J3DShapeMtxType_Mtx     = 0x00,
	J3DShapeMtxType_BBoard  = 0x01,
	J3DShapeMtxType_YBBoard = 0x02,
	J3DShapeMtxType_Multi   = 0x03,
};

J3DShapeMtx* J3DShapeFactory::newShapeMtx(int shapeNo, int mtxGroupNo) const
{
	J3DShapeMtx* ret = nullptr;
	const J3DShapeInitData& shapeInitData
	    = mpShapeInitData[mpIndexTable[shapeNo]];
	const J3DShapeMtxInitData& mtxInitData
	    = (&mpMtxInitData[shapeInitData.mMtxInitDataIndex])[mtxGroupNo];

	switch (shapeInitData.mShapeMtxType) {
	case J3DShapeMtxType_Mtx:
	case J3DShapeMtxType_BBoard:
	case J3DShapeMtxType_YBBoard:
		ret = new J3DShapeMtx(mtxInitData.mUseMtxIndex);
		break;
	case J3DShapeMtxType_Multi:
		ret = new J3DShapeMtxMulti(mtxInitData.mUseMtxIndex,
		                           mtxInitData.mUseMtxCount,
		                           &mpMtxTable[mtxInitData.mFirstUseMtxIndex]);
		break;
	}
	return ret;
}

J3DShapeDraw* J3DShapeFactory::newShapeDraw(int shapeNo, int mtxGroupNo,
                                            J3DMdlDataFlag) const
{
	const J3DShapeInitData& shapeInitData
	    = mpShapeInitData[mpIndexTable[shapeNo]];
	const J3DShapeDrawInitData& drawInitData
	    = (&mpDrawInitData[shapeInitData.mDrawInitDataIndex])[mtxGroupNo];
	return new J3DShapeDraw(&mpDisplayListData[drawInitData.mDisplayListIndex],
	                        drawInitData.mDisplayListSize);
}

void J3DShapeFactory::allocVcdVatCmdBuffer(u32 count)
{
	mpVcdVatCmdBuffer = new (0x20) u8[J3DShape::kVcdVatDLSize * count];
}
