#include <JSystem/J3D/J3DGraphBase/J3DVertex.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <macros.h>

#pragma opt_strength_reduction off

J3DVertexData::J3DVertexData()
{
	unk0            = 0;
	unk4            = 0;
	unk8            = 0;
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
	mVertexData = vertex_data;
	unk4        = vertex_data->mVtxPosArray;
	unkC        = vertex_data->mVtxNormArray;
	unk14       = vertex_data->mVtxColorArray[0];
	unk8        = 0;
	unk10       = 0;
	unk18       = 0;
	unk1C[0]    = vertex_data->mVtxPosArray;
	unk24[0]    = vertex_data->mVtxNormArray;
	unk1C[1]    = 0;
	unk24[1]    = 0;
	unk2C       = vertex_data->mVtxPosArray;
	unk30       = vertex_data->mVtxNormArray;
	unk34       = vertex_data->mVtxColorArray[0];
}

J3DVertexBuffer::~J3DVertexBuffer() { }

void J3DVertexBuffer::copyTransformedVtxArray()
{
	char trash[0x10];
	if (unk1C[0] != 0 && unk1C[1] != 0) {
		return;
	}

	for (int i = 0; i < 2; ++i) {
		// TODO: the `new`s are probably for vec3s
		if (i == 0 || unk1C[i] == 0) {
			unk1C[i] = new (0x20) char[mVertexData->unk0 * 0xc];
		}
		if (i == 0 || unk24[i] == 0) {
			unk24[i] = new (0x20) char[mVertexData->unk4 * 0xc];
		}
	}
}

J3DDrawMtxData::J3DDrawMtxData()
{
	unk0 = 0;
	unk4 = 0;
	unk8 = 0;
}

J3DDrawMtxData::~J3DDrawMtxData() { }
