#include <math.h>

#include <dolphin/gx.h>
#include <dolphin/os.h>

#include <macros.h>
#include <fake_tgmath.h>

#include "__gx.h"

static GXVtxDescList vcd[27];
static GXVtxAttrFmtList vat[27];

// NOTE: original TU contained a TON more functions,
// which influenced the order of float/double constants.
static void dummy(volatile float* f, volatile double* d)
{
	*f = 0.0f;
	*f = 1.0f;
	*f = 3.1415927f;
	*f = -1.f;
	// double <-> int conversion constants?
	*d = 4503601774854144;
	*d = 4503599627370496;
	*f = 6.2831855f;
	*f = 0.57735026f;
}

static void GetVertState(void)
{
	GXGetVtxDescv(vcd);
	GXGetVtxAttrFmtv(GX_VTXFMT3, vat);
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_NRM, GX_DIRECT);
	GXSetVtxAttrFmt(GX_VTXFMT3, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT3, GX_VA_NRM, GX_NRM_XYZ, GX_F32, 0);
}

static void RestoreVertState(void)
{
	GXSetVtxDescv(vcd);
	GXSetVtxAttrFmtv(3, vat);
}

void GXDrawSphere(u8 numMajor, u8 numMinor)
{
	GXAttrType ttype;
	f32 radius    = 1.0f;
	f32 majorStep = 3.1415927f / numMajor;
	f32 minorStep = 6.2831855f / numMinor;
	s32 i, j;
	f32 a, b;
	f32 r0, r1;
	f32 z0, z1;
	f32 c;
	f32 x, y;

	GXGetVtxDesc(GX_VA_TEX0, &ttype);
	GetVertState();
	if (ttype != GX_NONE) {
		GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
		GXSetVtxAttrFmt(GX_VTXFMT3, GX_VA_TEX0, GX_TEX_ST, GX_RGBA6, 0U);
	}
	for (i = 0; i < numMajor; i++) {
		a  = i * majorStep;
		b  = a + majorStep;
		r0 = radius * sinf(a);
		r1 = radius * sinf(b);
		z0 = radius * cosf(a);
		z1 = radius * cosf(b);
		GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT3, (numMinor + 1) * 2);
		for (j = 0; j <= numMinor; j++) {
			c = j * minorStep;
			x = cosf(c);
			y = sinf(c);
			GXPosition3f32(x * r1, y * r1, z1);
			GXNormal3f32((x * r1) / radius, (y * r1) / radius, z1 / radius);
			if (ttype != GX_NONE) {
				GXTexCoord2f32((f32)j / (f32)numMinor,
				               (f32)(i + 1) / (f32)numMajor);
			}
			GXPosition3f32(x * r0, y * r0, z0);
			GXNormal3f32((x * r0) / radius, (y * r0) / radius, z0 / radius);
			if (ttype != GX_NONE) {
				GXTexCoord2f32((f32)j / (f32)numMinor, (f32)i / (f32)numMajor);
			}
		}
		GXEnd();
	}
	RestoreVertState();
}

static void GXDrawCubeFace(f32 nx, f32 ny, f32 nz, f32 tx, f32 ty, f32 tz,
                           f32 bx, f32 by, f32 bz, GXAttrType binormal,
                           GXAttrType texture)
{
	GXPosition3f32(0.57735026f * (nx + tx + bx), 0.57735026f * (ny + ty + by),
	               0.57735026f * (nz + tz + bz));
	GXNormal3f32(nx, ny, nz);
	if (binormal != GX_NONE) {
		GXNormal3f32(tx, ty, tz);
		GXNormal3f32(bx, by, bz);
	}
	if (texture != GX_NONE) {
		GXTexCoord2s8(1, 1);
	}
	GXPosition3f32(0.57735026f * (nx - tx + bx), 0.57735026f * (ny - ty + by),
	               0.57735026f * (nz - tz + bz));
	GXNormal3f32(nx, ny, nz);
	if (binormal != GX_NONE) {
		GXNormal3f32(tx, ty, tz);
		GXNormal3f32(bx, by, bz);
	}
	if (texture != GX_NONE) {
		GXTexCoord2s8(0, 1);
	}
	GXPosition3f32(0.57735026f * (nx - tx - bx), 0.57735026f * (ny - ty - by),
	               0.57735026f * (nz - tz - bz));
	GXNormal3f32(nx, ny, nz);
	if (binormal != GX_NONE) {
		GXNormal3f32(tx, ty, tz);
		GXNormal3f32(bx, by, bz);
	}
	if (texture != GX_NONE) {
		GXTexCoord2s8(0, 0);
	}
	GXPosition3f32(0.57735026f * (nx + tx - bx), 0.57735026f * (ny + ty - by),
	               0.57735026f * (nz + tz - bz));
	GXNormal3f32(nx, ny, nz);
	if (binormal != GX_NONE) {
		GXNormal3f32(tx, ty, tz);
		GXNormal3f32(bx, by, bz);
	}
	if (texture != GX_NONE) {
		GXTexCoord2s8(1, 0);
	}
}

void GXDrawCube(void)
{
	GXAttrType ntype;
	GXAttrType ttype;

	GXGetVtxDesc(GX_VA_NBT, &ntype);
	GXGetVtxDesc(GX_VA_TEX0, &ttype);
	GetVertState();
	if (ntype != GX_NONE) {
		GXSetVtxDesc(GX_VA_NBT, GX_DIRECT);
		GXSetVtxAttrFmt(GX_VTXFMT3, GX_VA_NBT, GX_TEX_ST, GX_RGBA6, 0);
	}
	if (ttype != GX_NONE) {
		GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
		GXSetVtxAttrFmt(GX_VTXFMT3, GX_VA_TEX0, GX_TEX_ST, GX_RGB8, 0);
	}

	GXBegin(GX_QUADS, GX_VTXFMT3, 24);
	GXDrawCubeFace(-1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	               ntype, ttype);
	GXDrawCubeFace(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, ntype,
	               ttype);
	GXDrawCubeFace(0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	               ntype, ttype);
	GXDrawCubeFace(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, ntype,
	               ttype);
	GXDrawCubeFace(0.0f, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	               ntype, ttype);
	GXDrawCubeFace(0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, ntype,
	               ttype);
	GXEnd();

	RestoreVertState();
}
