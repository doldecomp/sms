#include <JSystem/J3D/J3DGraphBase/J3DTransform.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DStruct.hpp>
#include <JSystem/JMath/JMATrigonometric.hpp>

// This symbol needs to go in .data, but as it's only 8 bytes long, it would
// normally go in .sdata or .sdata2. But if the array doesn't have a size
// specified in its declaration, and its definition comes *after* it gets used,
// that forces it into .data instead because the size is unknown when it's used.
// Even const is ignored.
f32 Unit01[2] = { 0.0f, -1.0f };

f32 J3DCalcZValue(MtxPtr m, Vec v)
{
	/* Nonmatching */
	return m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3];
}

void J3DPSCalcInverseTranspose(MtxPtr src, ROMtxPtr dst)
{ /* Nonmatching */
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

	f32 cx = srt.mScaleX * cr;
	f32 sx = srt.mScaleX * sr;
	f32 sy = srt.mScaleY * sr;
	f32 cy = srt.mScaleY * cr;

	dst[0][0] = cx;
	dst[0][1] = -sx;
	dst[0][2] = (-cx * center.x + sx * center.y) + center.x + srt.mTranslationX;

	dst[1][0] = sy;
	dst[1][1] = cy;
	dst[1][2] = (-sy * center.x - cy * center.y) + center.y + srt.mTranslationY;

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

	f32 cx = srt.mScaleX * cr;
	f32 sx = srt.mScaleX * sr;
	f32 sy = srt.mScaleY * sr;
	f32 cy = srt.mScaleY * cr;

	dst[0][0] = cx;
	dst[0][1] = -sx;
	dst[0][3] = (-cx * center.x + sx * center.y) + center.x + srt.mTranslationX;

	dst[1][0] = sy;
	dst[1][1] = cy;
	dst[1][3] = (-sy * center.x - cy * center.y) + center.y + srt.mTranslationY;

	dst[2][3] = 0.0f;
	dst[2][1] = 0.0f;
	dst[2][0] = 0.0f;
	dst[1][2] = 0.0f;
	dst[0][2] = 0.0f;
	dst[2][2] = 1.0f;
}

void J3DGetTextureMtxMaya(const J3DTextureSRTInfo& srt, Mtx dst)
{
	f32 sr = JMASSin(srt.mRotation), cr = JMASCos(srt.mRotation);
	f32 tx = srt.mTranslationX - 0.5f;
	f32 ty = srt.mTranslationY - 0.5f;

	dst[0][0] = srt.mScaleX * cr;
	dst[0][1] = srt.mScaleY * sr;
	dst[0][2] = tx * cr - sr * (ty + srt.mScaleY) + 0.5f;

	dst[1][0] = -srt.mScaleX * sr;
	dst[1][1] = srt.mScaleY * cr;
	dst[1][2] = -tx * sr - cr * (ty + srt.mScaleY) + 0.5f;

	dst[2][3] = 0.0f;
	dst[2][1] = 0.0f;
	dst[2][0] = 0.0f;
	dst[1][3] = 0.0f;
	dst[0][3] = 0.0f;
	dst[2][2] = 1.0f;
}

void J3DGetTextureMtxMayaOld(const J3DTextureSRTInfo& srt, Mtx dst)
{
	f32 sr = JMASSin(srt.mRotation), cr = JMASCos(srt.mRotation);
	f32 tx = srt.mTranslationX - 0.5f;
	f32 ty = srt.mTranslationY - 0.5f;

	dst[0][0] = srt.mScaleX * cr;
	dst[0][1] = srt.mScaleY * sr;
	dst[0][3] = tx * cr - sr * (ty + srt.mScaleY) + 0.5f;

	dst[1][0] = -srt.mScaleX * sr;
	dst[1][1] = srt.mScaleY * cr;
	dst[1][3] = -tx * sr - cr * (ty + srt.mScaleY) + 0.5f;

	dst[2][3] = 0.0f;
	dst[2][1] = 0.0f;
	dst[2][0] = 0.0f;
	dst[1][2] = 0.0f;
	dst[0][2] = 0.0f;
	dst[2][2] = 1.0f;
}

void J3DScaleNrmMtx(register Mtx mtx, const register Vec& scl)
{
	register f32 mtx_xy, mtx_z_, scl_xy, scl_z_;
#ifdef __MWERKS__ // clang-format off
    asm {
        /* Row 0 */
        psq_l  scl_xy, 0(scl), 0, 0
        psq_l  mtx_xy, 0(mtx), 0, 0
        lfs    scl_z_, 8(scl)
        lfs    mtx_z_, 8(mtx)
        ps_mul f4, mtx_xy, scl_xy
        psq_st f4, 0(mtx), 0, 0
        fmuls  f4, mtx_z_, scl_z_
        stfs   f4, 8(mtx)

        /* Row 1 */
        psq_l  scl_xy, 0(scl), 0, 0
        psq_l  mtx_xy, 16(mtx), 0, 0
        lfs    scl_z_, 8(scl)
        lfs    mtx_z_, 24(mtx)
        ps_mul f4, mtx_xy, scl_xy
        psq_st f4, 16(mtx), 0, 0
        fmuls  f4, mtx_z_, scl_z_
        stfs   f4, 24(mtx)

        /* Row 2 */
        psq_l  scl_xy, 0(scl), 0, 0
        psq_l  mtx_xy, 32(mtx), 0, 0
        lfs    scl_z_, 8(scl)
        lfs    mtx_z_, 40(mtx)
        ps_mul f4, mtx_xy, scl_xy
        psq_st f4, 32(mtx), 0, 0
        fmuls  f4, mtx_z_, scl_z_
        stfs   f4, 40(mtx)
    }
#endif // clang-format on
}

void J3DScaleNrmMtx33(register ROMtxPtr mtx, const register Vec& scl)
{
	register f32 mtx0_xy, mtx0_z_;
	register f32 mtx1_xy, mtx1_z_;
	register f32 mtx2_xy, mtx2_z_;
	register f32 scl__xy, scl__z_;
#ifdef __MWERKS__ // clang-format off
    asm {
        psq_l  mtx0_xy, 0(mtx), 0, 0
        psq_l  scl__xy, 0(scl), 0, 0
        lfs    mtx0_z_, 8(mtx)
        lfs    scl__z_, 8(scl)
        ps_mul mtx0_xy, mtx0_xy, scl__xy
        psq_l  mtx1_xy, 12(mtx), 0, 0
        fmuls  mtx0_z_, mtx0_z_, scl__z_
        lfs    mtx1_z_, 20(mtx)
        ps_mul mtx1_xy, mtx1_xy, scl__xy
        psq_l  mtx2_xy, 24(mtx), 0, 0
        fmuls  mtx1_z_, mtx1_z_, scl__z_
        lfs    mtx2_z_, 32(mtx)
        ps_mul mtx2_xy, mtx2_xy, scl__xy
        psq_st mtx0_xy, 0(mtx), 0, 0
        fmuls  mtx2_z_, mtx2_z_, scl__z_
        stfs   mtx0_z_, 8(mtx)
        psq_st mtx1_xy, 12(mtx), 0, 0
        stfs   mtx1_z_, 20(mtx)
        psq_st mtx2_xy, 24(mtx), 0, 0
        stfs   mtx2_z_, 32(mtx)
    }
#endif // clang-format on
}

void J3DMtxProjConcat(Mtx, Mtx, Mtx)
{ /* Nonmatching */
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
        psq_l x1_y1, 0(src), 0, 0
        psq_l z1_x2, 8(src), 0, 0
        psq_l y2_z2, 16(src), 0, 0
        psq_l x3_y3, 24(src), 0, 0
        lfs z3, 32(src)
        psq_st x1_y1, 0(dst), 0, 0
        psq_st z1_x2, 8(dst), 0, 0
        psq_st y2_z2, 16(dst), 0, 0
        psq_st x3_y3, 24(dst), 0, 0
        stfs z3, 32(dst)
    }
#endif // clang-format on
}

void J3DPSMtx33CopyFrom34(register MtxPtr src, register ROMtxPtr dst)
{
#ifdef __MWERKS__ // clang-format off
    asm {
        psq_l  f0, 0(src), 0, 0
        psq_st f0, 0(dst), 0, 0
        lfs    f1, 8(src)
        stfs   f1, 8(dst)
        psq_l  f2, 16(src), 0, 0
        psq_st f2, 12(dst), 0, 0
        lfs    f3, 24(src)
        stfs   f3, 20(dst)
        psq_l  f4, 32(src), 0, 0
        psq_st f4, 24(dst), 0, 0
        lfs    f5, 40(src)
        stfs   f5, 32(dst)
    }
#endif // clang-format on
}

void J3DPSMtxArrayConcat(Mtx, Mtx, Mtx, u32)
{ /* Nonmatching */
}
