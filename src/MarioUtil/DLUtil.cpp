#include <MarioUtil/DLUtil.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <dolphin/os/OSCache.h>
#include <dolphin/gx.h>
#include <macros.h>

// TODO: some of this file REEKS of fakematches

static u8 uv[8] __attribute__((aligned(0x20)))
= { 0x0, 0x0, 0x80, 0x0, 0x80, 0x80, 0x0, 0x80 };

void TDLTexQuad::initValue(u16 param_1) { }

void TDLTexQuad::createDLBuffer(u16 param_1)
{
	u32 dlSize = ALIGN_NEXT(
	    sizeof(u16) + sizeof(u8) + unkA * 8 + unkA * param_1 * 4, 0x20);
	unkC[0]    = new (0x20) u8[dlSize];
	unkC[1]    = new (0x20) u8[dlSize];
	unkC[0][0] = 0x80;
	unkC[1][0] = 0x80;
	for (int i = 0; i < unkA * 8; ++i) {
		unkC[0][3 + i] = 0;
		unkC[1][3 + i] = 0;
	}
}

void TDLTexQuad::createPosArrayBuffer(u16 param_1)
{
	unk14[0] = new (0x20) f32[param_1 * 12];
	unk14[1] = new (0x20) f32[param_1 * 12];
}

void TDLTexQuad::createBuffer(u16 param_1)
{
	unk6  = param_1;
	unk4  = 0;
	unk1C = 0;
	reset();
	unkA = 3;
	createDLBuffer(param_1);
	createPosArrayBuffer(param_1);
}

bool TDLTexQuad::request(const JGeometry::TVec3<f32>* param_1)
{
	if (unk8 < unk6) {
		*(Vec*)&unk14[unk4][(unk8 * 4 + 0) * 3] = param_1[0];
		*(Vec*)&unk14[unk4][(unk8 * 4 + 1) * 3] = param_1[1];
		*(Vec*)&unk14[unk4][(unk8 * 4 + 2) * 3] = param_1[2];
		*(Vec*)&unk14[unk4][(unk8 * 4 + 3) * 3] = param_1[3];

		DL* dl    = (DL*)unkC[unk4];
		u8* bytes = dl->indices + unkA * 8 + unk8 * unkA * 4;

		u8 hi = ((unk8 * 4) & 0xff00) >> 8;
		u8 lo = unk8 * 4;

		*bytes++ = hi;
		*bytes++ = lo + 0;
		*bytes++ = 0;

		*bytes++ = hi;
		*bytes++ = lo + 1;
		*bytes++ = 1;

		*bytes++ = hi;
		*bytes++ = lo + 2;
		*bytes++ = 2;

		*bytes++ = hi;
		*bytes++ = lo + 3;
		*bytes++ = 3;

		++unk8;
		return true;
	}
	return false;
}

void TDLTexQuad::setEnd()
{
	unkC[unk4][1] = (((unk8 + 2) * 4) & 0xff00) >> 8;
	unkC[unk4][2] = (((unk8 + 2) * 4) & 0x00ff) >> 0;

	unk1C = sizeof(u16) + sizeof(u8) + (unkA * 8) + (unkA * unk8 * 4);

	int remainder = ALIGN_NEXT(unk1C, 0x20) - unk1C;
	u8* tail      = &unkC[unk4][unk1C];
	for (int i = 0; i < remainder; ++i)
		*tail++ = 0;

	DCStoreRange(unkC[unk4], ALIGN_NEXT(unk1C, 0x20));
	DCStoreRange(unk14[unk4], unk8 * sizeof(float) * 12);
}

void TDLTexQuad::draw()
{
	if (unk8 != 0) {
		GXClearVtxDesc();
		GXSetVtxDesc(GX_VA_POS, GX_INDEX16);
		GXSetVtxDesc(GX_VA_TEX0, GX_INDEX8);
		GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
		GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_U8, 7);
		GXSetArray(GX_VA_POS, unk14[unk4], 12);
		GXSetArray(GX_VA_TEX0, &uv, 2);
		GXInvalidateVtxCache();

		for (int i = 0; i < unkA * 8; ++i) {
			((DL*)unkC[unk4])->indices[i] = 0;
		}

		GXCallDisplayList(unkC[unk4], ALIGN_NEXT(unk1C, 0x20));
	}
}

void TDLTexQuadMulti::setChangeNum(u16) { }

void TDLTexQuadMulti::createDLBuffer(u16) { }

bool TDLTexQuadMulti::request(const JGeometry::TVec3<f32>*) { }

void TDLTexQuadMulti::setEnd() { }

void TDLTexQuadMulti::draw(int) { }

void TDLColorTexQuad::createColorArrayBuffer(u16 param_1)
{
	unk20[0] = new (0x20) GXColor[param_1];
	unk20[1] = new (0x20) GXColor[param_1];
}

void TDLColorTexQuad::createBuffer(u16 param_1)
{
	unk6  = param_1;
	unk4  = 0;
	unk1C = 0;
	reset();
	unkA = 4;
	createDLBuffer(param_1);
	createPosArrayBuffer(param_1);
	createColorArrayBuffer(param_1);
	unkC[0][0] = 0x80;
	unkC[1][0] = 0x80;
}

bool TDLColorTexQuad::requestCol(const JGeometry::TVec3<f32>* param_1,
                                 GXColor param_2, int param_3)
{
	if (unk8 < unk6) {
		*(Vec*)&unk14[unk4][(unk8 * 4 + 0) * 3] = param_1[0];
		*(Vec*)&unk14[unk4][(unk8 * 4 + 1) * 3] = param_1[1];
		*(Vec*)&unk14[unk4][(unk8 * 4 + 2) * 3] = param_1[2];
		*(Vec*)&unk14[unk4][(unk8 * 4 + 3) * 3] = param_1[3];

		unk20[unk4][unk8] = param_2;

		DL* dl    = (DL*)unkC[unk4];
		u8* bytes = dl->indices + unkA * 8 + unk8 * 0x10;

		u8 hi    = ((unk8 * 4) & 0xff00) >> 8;
		u8 lo    = ((unk8 * 4) & 0x00ff);
		u8 thing = ((unk8 * 4) & 0x00ff) >> 2;

		*bytes++ = hi;
		*bytes++ = lo + 0;
		*bytes++ = thing;
		*bytes++ = param_3 + 0 & 3;

		*bytes++ = hi;
		*bytes++ = lo + 1;
		*bytes++ = thing;
		*bytes++ = param_3 + 1 & 3;

		*bytes++ = hi;
		*bytes++ = lo + 2;
		*bytes++ = thing;
		*bytes++ = param_3 + 2 & 3;

		*bytes++ = hi;
		*bytes++ = lo + 3;
		*bytes++ = thing;
		*bytes++ = param_3 + 3 & 3;

		++unk8;
		return true;
	}
	return false;
}

void TDLColorTexQuad::setEnd()
{
	TDLTexQuad::setEnd();

	DCStoreRange(unkC[unk4], unk1C);
	DCStoreRange(unk20[unk4], unk8 * sizeof(GXColor));
}

void TDLColorTexQuad::draw()
{
	if (unk8 != 0) {
		GXSetArray(GX_VA_POS, unk14[unk4], 12);
		GXSetArray(GX_VA_CLR0, unk20[unk4], 4);
		GXSetArray(GX_VA_TEX0, &uv, 2);
		GXInvalidateVtxCache();
		GXCallDisplayList(unkC[unk4], ALIGN_NEXT(unk1C, 0x20));
	}
}
