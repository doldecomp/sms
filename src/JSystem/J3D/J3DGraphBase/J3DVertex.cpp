#include <JSystem/J3D/J3DGraphBase/J3DVertex.hpp>

#include <JSystem/JKernel/JKRHeap.hpp>

J3DVertexData::J3DVertexData()
{
	unk0  = 0;
	unk4  = 0;
	unk8  = 0;
	unkC  = 0;
	unk10 = 0;
	unk14 = 0;
	unk18 = 0;
	unk1C = 0;
	unk20 = 0;
	unk24 = 0;
	unk28 = 0;
	unk2C = 0;
	unk30 = 0;
	unk34 = 0;
	unk38 = 0;
	unk3C = 0;
	unk40 = 0;
}

J3DVertexData::~J3DVertexData() { }

J3DVertexBuffer::J3DVertexBuffer(J3DVertexData* vertex_data)
{
	mVertexData = vertex_data;
	unk4        = vertex_data->unk10;
	unkC        = vertex_data->unk14;
	unk14       = vertex_data->unk1C;
	unk8        = 0;
	unk10       = 0;
	unk18       = 0;
	unk1C[0]    = vertex_data->unk10;
	unk24[0]    = vertex_data->unk14;
	unk1C[1]    = 0;
	unk24[1]    = 0;
	unk2C       = vertex_data->unk10;
	unk30       = vertex_data->unk14;
	unk34       = vertex_data->unk1C;
}

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

J3DVertexBuffer::~J3DVertexBuffer() { }

J3DDrawMtxData::J3DDrawMtxData()
{
	unk0 = 0;
	unk4 = 0;
	unk8 = 0;
}

J3DDrawMtxData::~J3DDrawMtxData() { }
