#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <dolphin/os.h>
#include <macros.h>

#include "__gx.h"

void GXProject(f32 x, f32 y, f32 z, f32 mtx[3][4], f32* pm, f32* vp, f32* sx,
               f32* sy, f32* sz)
{
	Vec peye;
	f32 xc;
	f32 yc;
	f32 zc;
	f32 wc;

	peye.x
	    = mtx[0][3] + ((mtx[0][2] * z) + ((mtx[0][0] * x) + (mtx[0][1] * y)));
	peye.y
	    = mtx[1][3] + ((mtx[1][2] * z) + ((mtx[1][0] * x) + (mtx[1][1] * y)));
	peye.z
	    = mtx[2][3] + ((mtx[2][2] * z) + ((mtx[2][0] * x) + (mtx[2][1] * y)));
	if (pm[0] == 0.0f) {
		xc = (peye.x * pm[1]) + (peye.z * pm[2]);
		yc = (peye.y * pm[3]) + (peye.z * pm[4]);
		zc = pm[6] + (peye.z * pm[5]);
		wc = 1.0f / -peye.z;
	} else {
		xc = pm[2] + (peye.x * pm[1]);
		yc = pm[4] + (peye.y * pm[3]);
		zc = pm[6] + (peye.z * pm[5]);
		wc = 1.0f;
	}
	*sx = (vp[2] / 2.0f) + (vp[0] + (wc * (xc * vp[2] / 2.0f)));
	*sy = (vp[3] / 2.0f) + (vp[1] + (wc * (-yc * vp[3] / 2.0f)));
	*sz = vp[5] + (wc * (zc * (vp[5] - vp[4])));
}

void GXSetProjection(f32 mtx[4][4], GXProjectionType type)
{
	u32 reg;

	CHECK_GXBEGIN(0xCD, "GXSetProjection");

	gx->projType   = type;
	gx->projMtx[0] = mtx[0][0];
	gx->projMtx[2] = mtx[1][1];
	gx->projMtx[4] = mtx[2][2];
	gx->projMtx[5] = mtx[2][3];
	if (type == GX_ORTHOGRAPHIC) {
		gx->projMtx[1] = mtx[0][3];
		gx->projMtx[3] = mtx[1][3];
	} else {
		gx->projMtx[1] = mtx[0][2];
		gx->projMtx[3] = mtx[1][2];
	}

	reg = 0x00061020;
	GX_WRITE_U8(GX_LOAD_XF_REG);
	GX_WRITE_U32(reg);
	GX_WRITE_XF_REG_F(32, gx->projMtx[0]);
	GX_WRITE_XF_REG_F(33, gx->projMtx[1]);
	GX_WRITE_XF_REG_F(34, gx->projMtx[2]);
	GX_WRITE_XF_REG_F(35, gx->projMtx[3]);
	GX_WRITE_XF_REG_F(36, gx->projMtx[4]);
	GX_WRITE_XF_REG_F(37, gx->projMtx[5]);
	GX_WRITE_XF_REG_2(38, gx->projType);
	gx->bpSent = 1;
}

#define qr0 0

void GXGetProjectionv(f32* ptr)
{
	ptr[0] = gx->projType;
	ptr[1] = gx->projMtx[0];
	ptr[2] = gx->projMtx[1];
	ptr[3] = gx->projMtx[2];
	ptr[4] = gx->projMtx[3];
	ptr[5] = gx->projMtx[4];
	ptr[6] = gx->projMtx[5];
}

static asm void WriteMTXPS4x3(register f32 mtx[3][4],
                              register volatile f32* dest)
{
#ifdef __MWERKS__ // clang-format off
	psq_l f0, 0x00(mtx), 0, qr0
	psq_l f1, 0x08(mtx), 0, qr0
	psq_l f2, 0x10(mtx), 0, qr0
	psq_l f3, 0x18(mtx), 0, qr0
	psq_l f4, 0x20(mtx), 0, qr0
	psq_l f5, 0x28(mtx), 0, qr0
	psq_st f0, 0(dest), 0, qr0
	psq_st f1, 0(dest), 0, qr0
	psq_st f2, 0(dest), 0, qr0
	psq_st f3, 0(dest), 0, qr0
	psq_st f4, 0(dest), 0, qr0
	psq_st f5, 0(dest), 0, qr0
#endif // clang-format on
}

static asm void WriteMTXPS3x3from3x4(register f32 mtx[3][4],
                                     register volatile f32* dest)
{
#ifdef __MWERKS__ // clang-format off
	psq_l f0, 0x00(mtx), 0, qr0
	lfs   f1, 0x08(mtx)
	psq_l f2, 0x10(mtx), 0, qr0
	lfs   f3, 0x18(mtx)
	psq_l f4, 0x20(mtx), 0, qr0
	lfs   f5, 0x28(mtx)
	psq_st f0, 0(dest), 0, qr0
	stfs   f1, 0(dest)
	psq_st f2, 0(dest), 0, qr0
	stfs   f3, 0(dest)
	psq_st f4, 0(dest), 0, qr0
	stfs   f5, 0(dest)
#endif // clang-format on
}

static asm void WriteMTXPS4x2(register f32 mtx[2][4],
                              register volatile f32* dest) {
#ifdef __MWERKS__ // clang-format off
	psq_l f0, 0x00(mtx), 0, qr0
	psq_l f1, 0x08(mtx), 0, qr0
	psq_l f2, 0x10(mtx), 0, qr0
	psq_l f3, 0x18(mtx), 0, qr0
	psq_st f0, 0(dest), 0, qr0
	psq_st f1, 0(dest), 0, qr0
	psq_st f2, 0(dest), 0, qr0
	psq_st f3, 0(dest), 0, qr0
#endif // clang-format on
}

#define GX_WRITE_MTX_ELEM(addr, value)                                         \
	do {                                                                       \
		f32 xfData = (value);                                                  \
		GX_WRITE_F32(value);                                                   \
		VERIF_MTXLIGHT((addr), *(u32*)&xfData);                                \
	} while (0)

void GXLoadPosMtxImm(f32 mtx[3][4], u32 id)
{
	u32 reg;
	u32 addr;

	CHECK_GXBEGIN(0x1D8, "GXLoadPosMtxImm");

	addr = id * 4;
	reg  = addr | 0xB0000;

	GX_WRITE_U8(GX_LOAD_XF_REG);
	GX_WRITE_U32(reg);

	WriteMTXPS4x3(mtx, &GXWGFifo.f32);
}

// this one uses cmpwi instead of cmplwi for some reason
#define SET_REG_FIELD_(line, reg, size, shift, val)                            \
	do {                                                                       \
		(reg) = ((u32)(reg) & ~(((1 << (size)) - 1) << (shift)))               \
		        | ((u32)(val) << (shift));                                     \
	} while (0)

void GXLoadPosMtxIndx(u16 mtx_indx, u32 id)
{
	u32 offset;
	u32 reg;

	CHECK_GXBEGIN(0x208, "GXLoadPosMtxIndx");
	offset = id * 4;
	reg    = 0;
	SET_REG_FIELD(0x20E, reg, 12, 0, offset);
	SET_REG_FIELD(0x20F, reg, 4, 12, 11);
	SET_REG_FIELD_(0x210, reg, 16, 16, mtx_indx);
	GX_WRITE_U8(0x20);
	GX_WRITE_U32(reg);
}

void GXLoadNrmMtxImm(f32 mtx[3][4], u32 id)
{
	u32 reg;
	u32 addr;

	CHECK_GXBEGIN(0x229, "GXLoadNrmMtxImm");

	addr = id * 3 + 0x400;
	reg  = addr | 0x80000;

	GX_WRITE_U8(GX_LOAD_XF_REG);
	GX_WRITE_U32(reg);
	WriteMTXPS3x3from3x4(mtx, &GXWGFifo.f32);
}

void GXLoadNrmMtxIndx3x3(u16 mtx_indx, u32 id)
{
	u32 offset;
	u32 reg;

	CHECK_GXBEGIN(0x284, "GXLoadNrmMtxIndx3x3");
	offset = id * 3 + 0x400;
	reg    = 0;
	SET_REG_FIELD(0x28A, reg, 12, 0, offset);
	SET_REG_FIELD(0x28B, reg, 4, 12, 8);
	SET_REG_FIELD_(0x28C, reg, 16, 16, mtx_indx);
	GX_WRITE_U8(0x28);
	GX_WRITE_U32(reg);
}

void GXSetCurrentMtx(u32 id)
{
	CHECK_GXBEGIN(0x2A1, "GXSetCurrentMtx");
	SET_REG_FIELD(0x2A5, gx->matIdxA, 6, 0, id);
	__GXSetMatrixIndex(GX_VA_PNMTXIDX);
}

void GXLoadTexMtxImm(f32 mtx[][4], u32 id, GXTexMtxType type)
{
	u32 reg;
	u32 addr;
	u32 count;

	CHECK_GXBEGIN(0x2C2, "GXLoadTexMtxImm");

	if (id >= GX_PTTEXMTX0) {
		addr = (id - GX_PTTEXMTX0) * 4 + 0x500;
	} else {
		addr = id * 4;
	}
	count = (type == GX_MTX2x4) ? 8 : 12;
	reg   = addr | ((count - 1) << 16);

	GX_WRITE_U8(GX_LOAD_XF_REG);
	GX_WRITE_U32(reg);
	if (type == GX_MTX3x4) {
		WriteMTXPS4x3(mtx, &GXWGFifo.f32);
	} else {
		WriteMTXPS4x2(mtx, &GXWGFifo.f32);
	}
}

void GXSetViewportJitter(f32 left, f32 top, f32 wd, f32 ht, f32 nearz, f32 farz,
                         u32 field)
{
	f32 sx;
	f32 sy;
	f32 sz;
	f32 ox;
	f32 oy;
	f32 oz;
	f32 zmin;
	f32 zmax;
	u32 reg;

	CHECK_GXBEGIN(0x340, "GXSetViewport"); // not the correct function name

	if (field == 0) {
		top -= 0.5f;
	}
	sx          = wd / 2.0f;
	sy          = -ht / 2.0f;
	ox          = 342.0f + (left + (wd / 2.0f));
	oy          = 342.0f + (top + (ht / 2.0f));
	zmin        = 1.6777215e7f * nearz;
	zmax        = 1.6777215e7f * farz;
	sz          = zmax - zmin;
	oz          = zmax;
	gx->vpLeft  = left;
	gx->vpTop   = top;
	gx->vpWd    = wd;
	gx->vpHt    = ht;
	gx->vpNearz = nearz;
	gx->vpFarz  = farz;
	if (gx->fgRange != 0) {
		__GXSetRange(nearz, gx->fgSideX);
	}
	reg = 0x5101A;
	GX_WRITE_U8(GX_LOAD_XF_REG);
	GX_WRITE_U32(reg);
	GX_WRITE_XF_REG_F(26, sx);
	GX_WRITE_XF_REG_F(27, sy);
	GX_WRITE_XF_REG_F(28, sz);
	GX_WRITE_XF_REG_F(29, ox);
	GX_WRITE_XF_REG_F(30, oy);
	GX_WRITE_XF_REG_F(31, oz);
	gx->bpSent = 1;
}

void GXSetViewport(f32 left, f32 top, f32 wd, f32 ht, f32 nearz, f32 farz)
{
	GXSetViewportJitter(left, top, wd, ht, nearz, farz, 1U);
}

void GXGetViewportv(f32* vp)
{
	vp[0] = gx->vpLeft;
	vp[1] = gx->vpTop;
	vp[2] = gx->vpWd;
	vp[3] = gx->vpHt;
	vp[4] = gx->vpNearz;
	vp[5] = gx->vpFarz;
}

void GXSetScissor(u32 left, u32 top, u32 wd, u32 ht)
{
	u32 tp;
	u32 lf;
	u32 bm;
	u32 rt;

	CHECK_GXBEGIN(0x3B4, "GXSetScissor");

	tp = top + 342;
	lf = left + 342;
	bm = tp + ht - 1;
	rt = lf + wd - 1;

	SET_REG_FIELD(0x3BF, gx->suScis0, 11, 0, tp);
	SET_REG_FIELD(0x3C0, gx->suScis0, 11, 12, lf);
	SET_REG_FIELD(0x3C2, gx->suScis1, 11, 0, bm);
	SET_REG_FIELD(0x3C3, gx->suScis1, 11, 12, rt);

	GX_WRITE_BP_REG(gx->suScis0);
	GX_WRITE_BP_REG(gx->suScis1);
	gx->bpSent = 0;
}

void GXSetScissorBoxOffset(s32 x_off, s32 y_off)
{
	u32 reg = 0;
	u32 hx;
	u32 hy;

	CHECK_GXBEGIN(0x3FB, "GXSetScissorBoxOffset");

	hx = (u32)(x_off + 342) >> 1;
	hy = (u32)(y_off + 342) >> 1;

	SET_REG_FIELD(0x405, reg, 10, 0, hx);
	SET_REG_FIELD(0x406, reg, 10, 10, hy);
	SET_REG_FIELD(0x407, reg, 8, 24, 0x59);
	GX_WRITE_BP_REG(reg);
	gx->bpSent = 0;
}

void GXSetClipMode(GXClipMode mode)
{
	CHECK_GXBEGIN(0x41B, "GXSetClipMode");
	GX_WRITE_XF_REG(5, mode);
	gx->bpSent = 1;
}

void __GXSetMatrixIndex(GXAttr matIdxAttr)
{
	if (matIdxAttr < GX_VA_TEX4MTXIDX) {
		GX_WRITE_SOME_REG4(GX_LOAD_CP_REG, 0x30, gx->matIdxA, -12);
		GX_WRITE_XF_REG(24, gx->matIdxA);
	} else {
		GX_WRITE_SOME_REG4(GX_LOAD_CP_REG, 0x40, gx->matIdxB, -12);
		GX_WRITE_XF_REG(25, gx->matIdxB);
	}
	gx->bpSent = 1;
}
