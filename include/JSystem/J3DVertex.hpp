#ifndef J3D_VERTEX_HPP
#define J3D_VERTEX_HPP

#include <dolphin/types.h>

class J3DVertexData {
public:
	J3DVertexData();
	~J3DVertexData();

	u32 unk0;
	u32 unk4;
	u32 unk8;
	u32 unkC;
	void* unk10;
	void* unk14;
	u32 unk18;
	u32 unk1C;
	u32 unk20;
	u32 unk24;
	u32 unk28;
	u32 unk2C;
	u32 unk30;
	u32 unk34;
	u32 unk38;
	u32 unk3C;
	u32 unk40;
};

// TODO: is this an enum?
enum J3DDeformAttachFlag {};

class J3DVertexBuffer
{
public:
	J3DVertexBuffer(J3DVertexData* vertex_data);

	J3DVertexData* mVertexData;
	void* unk4;
	u32 unk8;
	void* unkC;
	u32 unk10;
	u32 unk14;
	u32 unk18;
	// TODO: figure out what actually lives here
	void* unk1C[2];
	void* unk24[2];
	void* unk2C;
	void* unk30;
	u32 unk34;

	virtual ~J3DVertexBuffer(); // vt at 0x38

	typedef void IDK;
	IDK copyLocalVtxArray(J3DDeformAttachFlag);
	IDK copyVtxColorArray(J3DDeformAttachFlag);
	IDK copyTransformedVtxArray();
};

class J3DDrawMtxData {
public:
	J3DDrawMtxData();
	~J3DDrawMtxData();

	s16 unk0;
	u32 unk4;
	u32 unk8;
};

#endif
