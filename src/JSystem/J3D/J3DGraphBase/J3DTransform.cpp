#include <JSystem/J3D/J3DGraphBase/J3DTransform.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DStruct.hpp>
#include <JSystem/JMath/JMATrigonometric.hpp>

J3DTransformInfo const j3dDefaultTransformInfo
    = { { 1.0f, 1.0f, 1.0f }, { 0, 0, 0 }, { 0.0f, 0.0f, 0.0f } };

// clang-format off
Mtx const j3dDefaultMtx = {
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	};
// clang-format on

f32 PSMulUnit01[2] = { 0.0f, -1.0f };

f32 Unit01[2] = { 0.0f, 1.0f };

#define qr0 0

#pragma push
#pragma fp_contract off
// TODO: several other functions in this TU use fp_contract,
// but this one for some reason doesn't?!
f32 J3DCalcZValue(MtxPtr m, Vec v)
{
	/* Nonmatching */
	return m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3];
}
#pragma pop

asm bool J3DPSCalcInverseTranspose(register MtxPtr src, register ROMtxPtr dst)
{
	// NOTE: copy-paste of PSMTXInverse
#ifdef __MWERKS__ // clang-format off
	psq_l      f0, 0x0(src), 1, qr0
	psq_l      f1, 0x4(src), 0, qr0
	psq_l      f2, 0x10(src), 1, qr0
	ps_merge10 f6, f1, f0
	psq_l      f3, 0x14(src), 0, qr0
	psq_l      f4, 0x20(src), 1, qr0
	ps_merge10 f7, f3, f2
	psq_l      f5, 0x24(src), 0, qr0
	ps_mul     f11, f3, f6
	ps_merge10 f8, f5, f4
	ps_mul     f13, f5, f7
	ps_msub    f11, f1, f7, f11
	ps_mul     f12, f1, f8
	ps_msub    f13, f3, f8, f13
	ps_msub    f12, f5, f6, f12
	ps_mul     f10, f3, f4
	ps_mul     f9, f0, f5
	ps_mul     f8, f1, f2
	ps_msub    f10, f2, f5, f10
	ps_msub    f9, f1, f4, f9
	ps_msub    f8, f0, f3, f8
	ps_mul     f7, f0, f13
	ps_sub     f1, f1, f1
	ps_madd    f7, f2, f12, f7
	ps_madd    f7, f4, f11, f7

	ps_cmpo0 cr0, f7, f1
	bne skip
	li r3, 0x0
	blr

skip:
	fres     f0, f7
	ps_add   f6, f0, f0
	ps_mul   f5, f0, f0
	ps_nmsub f0, f7, f5, f6
	ps_add   f6, f0, f0
	ps_mul   f5, f0, f0
	ps_nmsub f0, f7, f5, f6
	ps_muls0 f13, f13, f0
	ps_muls0 f12, f12, f0
	psq_st   f13, 0x0(dst), 0, qr0
	ps_muls0 f11, f11, f0
	psq_st   f12, 0xc(dst), 0, qr0
	ps_muls0 f10, f10, f0
	psq_st   f11, 0x18(dst), 0, qr0
	ps_muls0 f9, f9, f0
	psq_st   f10, 0x8(dst), 1, qr0
	ps_muls0 f8, f8, f0
	psq_st   f9, 0x14(dst), 1, qr0
	// TODO: supposedly, this should go one line lower and mwcc
	// should optimize it to happen one line earlier by itself?!
	// #pragma optimizewithasm on doesn't work
	li r3, 0x1
	psq_st   f8, 0x20(dst), 1, qr0
#endif // clang-format on
}

void J3DGetTranslateRotateMtx(const J3DTransformInfo& tx, Mtx dst)
{
	f32 sx = JMASSin(tx.mRotation.x), cx = JMASCos(tx.mRotation.x);
	f32 sy = JMASSin(tx.mRotation.y), cy = JMASCos(tx.mRotation.y);
	f32 sz = JMASSin(tx.mRotation.z), cz = JMASCos(tx.mRotation.z);

	dst[2][0] = -sy;
	dst[0][0] = cz * cy;
	dst[1][0] = sz * cy;
	dst[2][1] = cy * sx;
	dst[2][2] = cy * cx;

	f32 cxsz  = cx * sz;
	f32 sxcz  = sx * cz;
	dst[0][1] = sxcz * sy - cxsz;
	dst[1][2] = cxsz * sy - sxcz;

	f32 sxsz  = sx * sz;
	f32 cxcz  = cx * cz;
	dst[0][2] = cxcz * sy + sxsz;
	dst[1][1] = sxsz * sy + cxcz;

	dst[0][3] = tx.mTranslate.x;
	dst[1][3] = tx.mTranslate.y;
	dst[2][3] = tx.mTranslate.z;
}

void J3DGetTranslateRotateMtx(s16 rx, s16 ry, s16 rz, f32 tx, f32 ty, f32 tz,
                              Mtx dst)
{
	f32 sx = JMASSin(rx), cx = JMASCos(rx);
	f32 sy = JMASSin(ry), cy = JMASCos(ry);
	f32 sz = JMASSin(rz), cz = JMASCos(rz);

	dst[2][0] = -sy;
	dst[0][0] = cz * cy;
	dst[1][0] = sz * cy;
	dst[2][1] = cy * sx;
	dst[2][2] = cy * cx;

	f32 cxsz  = cx * sz;
	f32 sxcz  = sx * cz;
	dst[0][1] = sxcz * sy - cxsz;
	dst[1][2] = cxsz * sy - sxcz;

	f32 sxsz  = sx * sz;
	f32 cxcz  = cx * cz;
	dst[0][2] = cxcz * sy + sxsz;
	dst[1][1] = sxsz * sy + cxcz;

	dst[0][3] = tx;
	dst[1][3] = ty;
	dst[2][3] = tz;
}

void J3DGetTextureMtx(const J3DTextureSRTInfo& srt, Vec center, Mtx dst)
{
	f32 sr = JMASSin(srt.mRotation), cr = JMASCos(srt.mRotation);

	dst[0][0] = srt.mScaleX * cr;
	dst[0][1] = -srt.mScaleX * sr;
	dst[0][2] = (-srt.mScaleX * cr * center.x + srt.mScaleX * sr * center.y)
	            + center.x + srt.mTranslationX;

	dst[1][0] = srt.mScaleY * sr;
	dst[1][1] = srt.mScaleY * cr;
	dst[1][2] = (-srt.mScaleY * sr * center.x - srt.mScaleY * cr * center.y)
	            + center.y + srt.mTranslationY;

	dst[2][3] = 0.0f;
	dst[2][1] = 0.0f;
	dst[2][0] = 0.0f;
	dst[1][3] = 0.0f;
	dst[0][3] = 0.0f;
	dst[2][2] = 1.0f;
}

void J3DGetTextureMtxOld(const J3DTextureSRTInfo& srt, Vec center, Mtx dst)
{
	f32 sr = JMASSin(srt.mRotation), cr = JMASCos(srt.mRotation);

	dst[0][0] = srt.mScaleX * cr;
	dst[0][1] = -srt.mScaleX * sr;
	dst[0][3] = (-srt.mScaleX * cr * center.x + srt.mScaleX * sr * center.y)
	            + center.x + srt.mTranslationX;

	dst[1][0] = srt.mScaleY * sr;
	dst[1][1] = srt.mScaleY * cr;
	dst[1][3] = (-srt.mScaleY * sr * center.x - srt.mScaleY * cr * center.y)
	            + center.y + srt.mTranslationY;

	dst[2][3] = 0.0f;
	dst[2][1] = 0.0f;
	dst[2][0] = 0.0f;
	dst[1][2] = 0.0f;
	dst[0][2] = 0.0f;
	dst[2][2] = 1.0f;
}

void J3DGetTextureMtxMaya(const J3DTextureSRTInfo& srt, MtxPtr dst)
{
	dst[0][0] = srt.mScaleX * JMASCos(srt.mRotation);
	dst[0][1] = srt.mScaleY * JMASSin(srt.mRotation);
	dst[0][2]
	    = (srt.mTranslationX - 0.5f) * JMASCos(srt.mRotation)
	      - JMASSin(srt.mRotation) * ((srt.mTranslationY - 0.5f) + srt.mScaleY)
	      + 0.5f;
	dst[0][3] = 0.0f;

	dst[1][0] = -srt.mScaleX * JMASSin(srt.mRotation);
	dst[1][1] = srt.mScaleY * JMASCos(srt.mRotation);
	dst[1][2]
	    = -(srt.mTranslationX - 0.5f) * JMASSin(srt.mRotation)
	      - JMASCos(srt.mRotation) * ((srt.mTranslationY - 0.5f) + srt.mScaleY)
	      + 0.5f;
	dst[1][3] = 0.0f;

	dst[2][0] = 0.0f;
	dst[2][1] = 0.0f;
	dst[2][2] = 1.0f;
	dst[2][3] = 0.0f;
}

void J3DGetTextureMtxMayaOld(const J3DTextureSRTInfo& srt, Mtx dst)
{
	dst[0][0] = srt.mScaleX * JMASCos(srt.mRotation);
	dst[0][1] = srt.mScaleY * JMASSin(srt.mRotation);
	dst[0][2] = 0.0f;
	dst[0][3]
	    = (srt.mTranslationX - 0.5f) * JMASCos(srt.mRotation)
	      - JMASSin(srt.mRotation) * ((srt.mTranslationY - 0.5f) + srt.mScaleY)
	      + 0.5f;

	dst[1][0] = -srt.mScaleX * JMASSin(srt.mRotation);
	dst[1][1] = srt.mScaleY * JMASCos(srt.mRotation);
	dst[1][2] = 0.0f;
	dst[1][3]
	    = -(srt.mTranslationX - 0.5f) * JMASSin(srt.mRotation)
	      - JMASCos(srt.mRotation) * ((srt.mTranslationY - 0.5f) + srt.mScaleY)
	      + 0.5f;

	dst[2][0] = 0.0f;
	dst[2][1] = 0.0f;
	dst[2][2] = 1.0f;
	dst[2][3] = 0.0f;
}

void J3DScaleNrmMtx33(register ROMtxPtr mtx, const register Vec& scl)
{
	register f32 mtx0_xy, mtx0_z_;
	register f32 mtx1_xy, mtx1_z_;
	register f32 mtx2_xy, mtx2_z_;
	register f32 scl__xy, scl__z_;
#ifdef __MWERKS__ // clang-format off
	asm {
		psq_l  mtx0_xy, 0(mtx), 0, qr0
		psq_l  scl__xy, 0(scl), 0, qr0
		lfs    mtx0_z_, 8(mtx)
		lfs    scl__z_, 8(scl)
		ps_mul mtx0_xy, mtx0_xy, scl__xy
		psq_l  mtx1_xy, 12(mtx), 0, qr0
		fmuls  mtx0_z_, mtx0_z_, scl__z_
		lfs    mtx1_z_, 20(mtx)
		ps_mul mtx1_xy, mtx1_xy, scl__xy
		psq_l  mtx2_xy, 24(mtx), 0, qr0
		fmuls  mtx1_z_, mtx1_z_, scl__z_
		lfs    mtx2_z_, 32(mtx)
		ps_mul mtx2_xy, mtx2_xy, scl__xy
		psq_st mtx0_xy, 0(mtx), 0, qr0
		fmuls  mtx2_z_, mtx2_z_, scl__z_
		stfs   mtx0_z_, 8(mtx)
		psq_st mtx1_xy, 12(mtx), 0, qr0
		stfs   mtx1_z_, 20(mtx)
		psq_st mtx2_xy, 24(mtx), 0, qr0
		stfs   mtx2_z_, 32(mtx)
	}
#endif // clang-format on
}

void J3DMtxProjConcat(register Mtx param_1, register Mtx param_2,
                      register Mtx result)
{
	// TODO: register naming (but does it really matter though)
#ifdef __MWERKS__ // clang-format off
	asm {
		psq_l      f2, 0x0(param_1), 0, qr0
		psq_l      f3, 0x8(param_1), 0, qr0
		ps_merge00 f6, f2, f2
		ps_merge11 f7, f2, f2
		ps_merge00 f8, f3, f3
		ps_merge11 f9, f3, f3
		psq_l      f10, 0x0(param_2),  0, qr0
		psq_l      f11, 0x10(param_2), 0, qr0
		psq_l      f12, 0x20(param_2), 0, qr0
		psq_l      f13, 0x30(param_2), 0, qr0
		ps_mul     f0, f6, f10
		ps_madd    f0, f7, f11, f0
		ps_madd    f0, f8, f12, f0
		ps_madd    f0, f9, f13, f0
		psq_st     f0, 0x0(result), 0, 0

		psq_l      f10, 0x8(param_2),  0, qr0
		psq_l      f11, 0x18(param_2), 0, qr0
		psq_l      f12, 0x28(param_2), 0, qr0
		psq_l      f13, 0x38(param_2), 0, qr0
		ps_mul     f0, f6, f10
		ps_madd    f0, f7, f11, f0
		ps_madd    f0, f8, f12, f0
		ps_madd    f0, f9, f13, f0
		psq_st     f0, 0x8(result), 0, qr0

		psq_l      f2, 0x10(param_1), 0, qr0
		psq_l      f3, 0x18(param_1), 0, qr0
		ps_merge00 f6, f2, f2
		ps_merge11 f7, f2, f2
		ps_merge00 f8, f3, f3
		ps_merge11 f9, f3, f3
		psq_l      f10, 0x0(param_2),  0, qr0
		psq_l      f11, 0x10(param_2), 0, qr0
		psq_l      f12, 0x20(param_2), 0, qr0
		psq_l      f13, 0x30(param_2), 0, qr0
		ps_mul     f0, f6, f10
		ps_madd    f0, f7, f11, f0
		ps_madd    f0, f8, f12, f0
		ps_madd    f0, f9, f13, f0
		psq_st     f0, 0x10(result), 0, qr0

		psq_l      f10, 0x8(param_2),  0, qr0
		psq_l      f11, 0x18(param_2), 0, qr0
		psq_l      f12, 0x28(param_2), 0, qr0
		psq_l      f13, 0x38(param_2), 0, qr0
		ps_mul     f0, f6, f10
		ps_madd    f0, f7, f11, f0
		ps_madd    f0, f8, f12, f0
		ps_madd    f0, f9, f13, f0
		psq_st     f0, 0x18(result), 0, qr0

		psq_l      f2, 0x20(param_1), 0, qr0
		psq_l      f3, 0x28(param_1), 0, qr0
		ps_merge00 f6, f2, f2
		ps_merge11 f7, f2, f2
		ps_merge00 f8, f3, f3
		ps_merge11 f9, f3, f3
		psq_l      f10, 0x0(param_2),  0, qr0
		psq_l      f11, 0x10(param_2), 0, qr0
		psq_l      f12, 0x20(param_2), 0, qr0
		psq_l      f13, 0x30(param_2), 0, qr0
		ps_mul     f0, f6, f10
		ps_madd    f0, f7, f11, f0
		ps_madd    f0, f8, f12, f0
		ps_madd    f0, f9, f13, f0
		psq_st     f0, 0x20(result), 0, qr0

		psq_l      f10, 0x8(param_2),  0, qr0
		psq_l      f11, 0x18(param_2), 0, qr0
		psq_l      f12, 0x28(param_2), 0, qr0
		psq_l      f13, 0x38(param_2), 0, qr0
		ps_mul     f0, f6, f10
		ps_madd    f0, f7, f11, f0
		ps_madd    f0, f8, f12, f0
		ps_madd    f0, f9, f13, f0
		psq_st     f0, 0x28(result), 0, qr0
	}
#endif // clang-format on
}

void J3DPSMtx33Copy(register ROMtxPtr src, register ROMtxPtr dst)
{
	register f32 x1_y1;
	register f32 z1_x2;
	register f32 y2_z2;
	register f32 x3_y3;
	register f32 z3;
#ifdef __MWERKS__ // clang-format off
	asm {
		psq_l x1_y1, 0(src),  0, qr0
		psq_l z1_x2, 8(src),  0, qr0
		psq_l y2_z2, 16(src), 0, qr0
		psq_l x3_y3, 24(src), 0, qr0
		lfs z3, 32(src)

		psq_st x1_y1, 0(dst),  0, qr0
		psq_st z1_x2, 8(dst),  0, qr0
		psq_st y2_z2, 16(dst), 0, qr0
		psq_st x3_y3, 24(dst), 0, qr0
		stfs z3, 32(dst)
	}
#endif // clang-format on
}

asm void J3DPSMtx33CopyFrom34(register MtxPtr src, register ROMtxPtr dst)
{
#ifdef __MWERKS__ // clang-format off
	psq_l  f0, 0(src), 0, qr0
	psq_st f0, 0(dst), 0, qr0

	lfs    f1, 8(src)
	stfs   f1, 8(dst)

	psq_l  f2, 16(src), 0, qr0
	psq_st f2, 12(dst), 0, qr0

	lfs    f3, 24(src)
	stfs   f3, 20(dst)

	psq_l  f4, 32(src), 0, qr0
	psq_st f4, 24(dst), 0, qr0

	lfs    f5, 40(src)
	stfs   f5, 32(dst)
#endif // clang-format on
}

asm void J3DPSMtxArrayCopy(register MtxPtr src, register MtxPtr dst,
                           register u32 size)
{
	// NOTE: I'm almost sure this weird `subi` was not generated automatically
	// and rather the entire function was coded in asm
#ifdef __MWERKS__ // clang-format off
	subi src, src, 0x8
	subi dst, dst, 0x8
	mtctr size
loop:
	psq_l   f0, 0x8(src),  0, qr0
	psq_st  f0, 0x8(dst),  0, qr0

	psq_l   f1, 0x10(src), 0, qr0
	psq_st  f1, 0x10(dst), 0, qr0

	psq_l   f2, 0x18(src), 0, qr0
	psq_st  f2, 0x18(dst), 0, qr0

	psq_l   f3, 0x20(src), 0, qr0
	psq_st  f3, 0x20(dst), 0, qr0

	psq_l   f4, 0x28(src), 0, qr0
	psq_st  f4, 0x28(dst), 0, qr0

	psq_lu  f5, 0x30(src), 0, qr0
	psq_stu f5, 0x30(dst), 0, qr0
	bdnz loop
#endif // clang-format on
}

void J3DMTXConcatArrayIndexedSrc(register const float (*mat1)[4],
                                 register const float (*mat2)[3][4],
                                 register const u16* param_3, register Mtx* dst,
                                 register u32 count)
{
	register float* unit01 = Unit01;
	register u16 tmp       = param_3[0];

#ifdef __MWERKS__ // clang-format off
	asm {
		psq_l f0, 0x0(mat1), 0, qr0
		mtctr count

		psq_l     f2, 0x10(mat1), 0, qr0
		mulli     tmp, tmp, 0x30
		psq_l     f4, 0x20(mat1), 0, qr0
		psq_lx    f6, mat2, tmp, 0, qr0
		add       r7, mat2, tmp
		ps_muls0  f9, f6, f0
		psq_l     f7, 0x10(r7), 0, qr0
		ps_muls0  f10, f6, f2
		ps_muls0  f11, f6, f4
		psq_l     f8, 0x20(r7), 0, qr0
		ps_madds1 f9, f7, f0, f9
		psq_l     f1, 0x8(mat1), 0, qr0
		ps_madds1 f10, f7, f2, f10
		psq_l     f3, 0x18(mat1), 0, qr0
		ps_madds1 f11, f7, f4, f11
		psq_l     f5, 0x28(mat1), 0, qr0
		ps_madds0 f9, f8, f1, f9
		psq_l     f6, 0x8(r7), 0, qr0
		ps_madds0 f10, f8, f3, f10
		ps_madds0 f11, f8, f5, f11
		psq_l     f7, 0x18(r7), 0, qr0
		psq_st    f9, 0x0(dst), 0, qr0
		ps_muls0  f8, f6, f0
		ps_muls0  f9, f6, f2
		ps_muls0  f12, f6, f4
		psq_l     f6, 0x28(r7), 0, qr0
		psq_st    f10, 0x10(dst), 0, qr0
		ps_madds1 f8, f7, f0, f8
		ps_madds1 f9, f7, f2, f9
		ps_madds1 f12, f7, f4, f12
		psq_l     f13, 0x0(unit01), 0, qr0
		psq_st    f11, 0x20(dst), 0, qr0

	loop:
		ps_madds0 f8,  f6, f1, f8
		ps_madds0 f9,  f6, f3, f9
		ps_madds0 f12, f6, f5, f12
		ps_madd   f8,  f13, f1, f8
		ps_madd   f9,  f13, f3, f9
		ps_madd   f12, f13, f5, f12
		psq_st    f8,  0x8(dst),  0, qr0
		psq_st    f9,  0x18(dst), 0, qr0
		psq_st    f12, 0x28(dst), 0, qr0

		bdz end

		lhzu      tmp, 0x2(param_3)
		addi      dst, dst, 0x30
		mulli     tmp, tmp, 0x30

		psq_lx    f6, mat2, tmp, 0, qr0
		add       r7, mat2, tmp
		ps_muls0  f9, f6, f0
		psq_l     f7, 0x10(r7), 0, qr0
		ps_muls0  f10, f6, f2
		ps_muls0  f11, f6, f4
		psq_l     f8, 0x20(r7), 0, qr0
		ps_madds1 f9, f7, f0, f9
		ps_madds1 f10, f7, f2, f10
		ps_madds1 f11, f7, f4, f11
		psq_l     f6, 0x8(r7), 0, qr0
		ps_madds0 f9, f8, f1, f9
		ps_madds0 f10, f8, f3, f10
		ps_madds0 f11, f8, f5, f11
		psq_l     f7, 0x18(r7), 0, qr0
		psq_st    f9, 0x0(dst), 0, qr0
		ps_muls0  f8, f6, f0
		ps_muls0  f9, f6, f2
		ps_muls0  f12, f6, f4
		psq_l     f6, 0x28(r7), 0, qr0
		psq_st    f10, 0x10(dst), 0, qr0
		ps_madds1 f8, f7, f0, f8
		ps_madds1 f9, f7, f2, f9
		ps_madds1 f12, f7, f4, f12
		psq_st    f11, 0x20(dst), 0, qr0
		b loop
	end:
	}
#endif // clang-format on
}

asm void J3DPSMtxArrayConcat(register Mtx fst, register Mtx snd,
                             register Mtx dst, register u32 size)
{
#ifdef __MWERKS__ // clang-format off
	nofralloc

	stwu r1, -0x40(r1)
	stfd f14, 0x8(r1)
	lis r7, Unit01@ha
	stfd f15, 0x10(r1)
	addi r7, r7, Unit01@l
	stfd f31, 0x28(r1)

	subi snd, snd, 0x8
	subi dst, dst, 0x8

	mtctr size

loop:
	psq_l     f0, 0x0(fst), 0, qr0
	psq_l     f6, 0x8(snd), 0, qr0
	psq_l     f7, 0x10(snd), 0, qr0
	psq_l     f8, 0x18(snd), 0, qr0
	ps_muls0  f12, f6, f0
	psq_l     f2, 0x10(fst), 0, qr0
	ps_muls0  f13, f7, f0
	psq_l     f31, 0x0(r7), 0, qr0
	ps_muls0  f14, f6, f2
	psq_l     f9, 0x20(snd), 0, qr0
	ps_muls0  f15, f7, f2
	psq_l     f1, 0x8(fst), 0, qr0
	ps_madds1 f12, f8, f0, f12
	psq_l     f3, 0x18(fst), 0, qr0
	ps_madds1 f14, f8, f2, f14
	psq_l     f10, 0x28(snd), 0, qr0
	ps_madds1 f13, f9, f0, f13
	psq_lu    f11, 0x30(snd), 0, qr0
	ps_madds1 f15, f9, f2, f15
	psq_l     f4, 0x20(fst), 0, qr0
	psq_l     f5, 0x28(fst), 0, qr0
	ps_madds0 f12, f10, f1, f12
	ps_madds0 f13, f11, f1, f13
	ps_madds0 f14, f10, f3, f14
	ps_madds0 f15, f11, f3, f15
	psq_st    f12, 0x8(dst), 0, qr0
	ps_muls0  f2, f6, f4
	ps_madds1 f13, f31, f1, f13
	ps_muls0  f0, f7, f4
	psq_st    f14, 0x18(dst), 0, qr0
	ps_madds1 f15, f31, f3, f15
	psq_st    f13, 0x10(dst), 0, qr0
	ps_madds1 f2, f8, f4, f2
	ps_madds1 f0, f9, f4, f0
	ps_madds0 f2, f10, f5, f2
	psq_st    f15, 0x20(dst), 0, qr0
	ps_madds0 f0, f11, f5, f0
	psq_st    f2, 0x28(dst), 0, qr0
	ps_madds1 f0, f31, f5, f0
	psq_stu   f0, 0x30(dst), 0, qr0

	bdnz loop

	lfd f14, 0x8(r1)
	lfd f15, 0x10(r1)
	lfd f31, 0x28(r1)
	addi r1, r1, 0x40

	blr
#endif // clang-format on
}
