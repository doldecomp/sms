#include <JSystem/J3D/J3DGraphBase/J3DVertex.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <macros.h>

J3DVertexData::J3DVertexData()
{
	mVtxNum         = 0;
	mNrmNum         = 0;
	mColNum         = 0;
	mVtxAttrFmtList = nullptr;
	mVtxPosArray    = nullptr;
	mVtxNormArray   = nullptr;
	mVtxNBTArray    = 0;
	for (int i = 0; i < ARRAY_COUNT(mVtxColorArray); ++i)
		mVtxColorArray[i] = nullptr;
	for (int i = 0; i < ARRAY_COUNT(mVtxTexCoordArray); ++i)
		mVtxTexCoordArray[i] = nullptr;
}

J3DVertexData::~J3DVertexData() { }

J3DVertexBuffer::J3DVertexBuffer(J3DVertexData* vertex_data)
{
	mVertexData                = vertex_data;
	mVtxPosArray[0]            = vertex_data->getVtxPosArray();
	mVtxNrmArray[0]            = vertex_data->getVtxNormArray();
	mVtxColArray[0]            = vertex_data->getVtxColorArray(0);
	mVtxPosArray[1]            = nullptr;
	mVtxNrmArray[1]            = nullptr;
	mVtxColArray[1]            = nullptr;
	mTransformedVtxPosArray[0] = vertex_data->getVtxPosArray();
	mTransformedVtxNrmArray[0] = vertex_data->getVtxNormArray();
	mTransformedVtxPosArray[1] = nullptr;
	mTransformedVtxNrmArray[1] = nullptr;
	unk2C                      = vertex_data->getVtxPosArray();
	unk30                      = vertex_data->getVtxNormArray();
	unk34                      = vertex_data->getVtxColorArray(0);
}

J3DVertexBuffer::~J3DVertexBuffer() { }

void J3DVertexBuffer::copyLocalVtxArray(J3DDeformAttachFlag)
{
	// UNUSED
}

void J3DVertexBuffer::copyTransformedVtxArray()
{
	if (mTransformedVtxPosArray[0] != 0 && mTransformedVtxPosArray[1] != 0)
		return;

	for (int i = 0; i < 2; ++i) {
		if (i == 0 || mTransformedVtxPosArray[i] == 0)
			mTransformedVtxPosArray[i]
			    = new (0x20) u8[mVertexData->getVtxNum() * 0xc];

		if (i == 0 || mTransformedVtxNrmArray[i] == 0)
			mTransformedVtxNrmArray[i]
			    = new (0x20) u8[mVertexData->getNrmNum() * 0xc];
	}
}

void J3DVertexBuffer::copyVtxColorArray(J3DDeformAttachFlag)
{
	// UNUSED
}

J3DDrawMtxData::J3DDrawMtxData()
{
	mEntryNum     = 0;
	mDrawMtxFlag  = 0;
	mDrawMtxIndex = 0;
}

J3DDrawMtxData::~J3DDrawMtxData() { }
