#ifndef J3DTRANSFORM_H
#define J3DTRANSFORM_H

#include <JSystem/JGeometry.hpp>
#include <dolphin/mtx.h>

struct J3DTextureSRTInfo;

struct J3DTransformInfo {
	/* 0x00 */ Vec mScale;
	/* 0x0C */ S16Vec mRotation;
	/* 0x14 */ Vec mTranslate;

	inline J3DTransformInfo& operator=(const J3DTransformInfo& b)
	{
		mScale     = b.mScale;
		mRotation  = b.mRotation;
		mTranslate = b.mTranslate;
		return *this;
	}
}; // Size: 0x20

extern J3DTransformInfo const j3dDefaultTransformInfo;
extern Vec const j3dDefaultScale;
extern Mtx const j3dDefaultMtx;
extern f32 PSMulUnit01[];

f32 J3DCalcZValue(MtxPtr m, Vec v);
bool J3DPSCalcInverseTranspose(MtxPtr, ROMtxPtr);
void J3DGetTranslateRotateMtx(J3DTransformInfo const&, Mtx);
void J3DGetTranslateRotateMtx(s16, s16, s16, f32, f32, f32, Mtx);
void J3DGetTextureMtx(const J3DTextureSRTInfo&, Vec, MtxPtr);
void J3DGetTextureMtxOld(const J3DTextureSRTInfo&, Vec, MtxPtr);
void J3DGetTextureMtxMaya(const J3DTextureSRTInfo&, MtxPtr);
void J3DGetTextureMtxMayaOld(const J3DTextureSRTInfo&, MtxPtr);
void J3DScaleNrmMtx33(ROMtxPtr, const Vec&);
void J3DMtxProjConcat(MtxPtr, MtxPtr, MtxPtr);
void J3DPSMtx33Copy(ROMtxPtr src, ROMtxPtr dst);
void J3DPSMtx33CopyFrom34(MtxPtr src, ROMtxPtr dst);
void J3DPSMtxArrayCopy(MtxPtr, MtxPtr, u32);
void J3DMTXConcatArrayIndexedSrc(const float (*)[4], const float (*)[3][4],
                                 const u16*, float (*)[3][4], u32);
void J3DPSMtxArrayConcat(Mtx, Mtx, Mtx, u32);

// TODO: is this used in sms? Probably, but totally inlined

// regalloc issues
inline void J3DPSMulMtxVec(register MtxPtr mtx, register Vec* vec,
                           register Vec* dst)
{
	register f32 fr12;
	register f32 fr11;
	register f32 fr10;
	register f32 fr9;
	register f32 fr8;
	register f32 fr6;
	register f32 fra6;
	register f32 fr5;
	register f32 fra5;
	register f32 fra4;
	register f32 fr4;
	register f32 fr3;
	register f32 fr2;
	register f32 fra2;
	register f32 fr01;
	register f32 fr00;
#ifdef __MWERKS__ // clang-format off
    asm {
        psq_l fr00, 0(vec), 0, 0
        psq_l fr2, 0(mtx), 0, 0
        psq_l fr01, 8(vec), 1, 0
        ps_mul fr4, fr2, fr00
        psq_l fr3, 8(mtx), 0, 0
        ps_madd fr5, fr3, fr01, fr4
        psq_l fr8, 16(mtx), 0, 0
        ps_sum0 fr6, fr5, fr6, fr5
        psq_l fr9, 24(mtx), 0, 0
        ps_mul fr10, fr8, fr00
        psq_st fr6, 0(dst), 1, 0
        ps_madd fr11, fr9, fr01, fr10
        psq_l fra2, 32(mtx), 0, 0
        ps_sum0 fr12, fr11, fr12, fr11
        psq_l fr3, 40(mtx), 0, 0
        ps_mul fra4, fra2, fr00
        psq_st fr12, 4(dst), 1, 0
        ps_madd fra5, fr3, fr01, fra4
        ps_sum0 fra6, fra5, fra6, fra5
        psq_st fra6, 8(dst), 1, 0
    }
#endif // clang-format on
}

// regalloc issues
inline void J3DPSMulMtxVec(register MtxPtr mtx, register S16Vec* vec,
                           register S16Vec* dst)
{
	register f32 fr12;
	register f32 fr11;
	register f32 fr10;
	register f32 fr9;
	register f32 fr8;
	register f32 fr6;
	register f32 fra6;
	register f32 fr5;
	register f32 fra5;
	register f32 fra4;
	register f32 fr4;
	register f32 fr3;
	register f32 fr2;
	register f32 fra2;
	register f32 fr01;
	register f32 fr00;
#ifdef __MWERKS__ // clang-format off
    asm {
        psq_l fr00, 0(vec), 0, 7
        psq_l fr2, 0(mtx), 0, 0
        psq_l fr01, 4(vec), 1, 7
        ps_mul fr4, fr2, fr00
        psq_l fr3, 8(mtx), 0, 0
        ps_madd fr5, fr3, fr01, fr4
        psq_l fr8, 16(mtx), 0, 0
        ps_sum0 fr6, fr5, fr6, fr5
        psq_l fr9, 24(mtx), 0, 0
        ps_mul fr10, fr8, fr00
        psq_st fr6, 0(dst), 1, 7
        ps_madd fr11, fr9, fr01, fr10
        psq_l fra2, 32(mtx), 0, 0
        ps_sum0 fr12, fr11, fr12, fr11
        psq_l fr3, 40(mtx), 0, 0
        ps_mul fra4, fra2, fr00
        psq_st fr12, 2(dst), 1, 7
        ps_madd fra5, fr3, fr01, fra4
        ps_sum0 fra6, fra5, fra6, fra5
        psq_st fra6, 4(dst), 1, 7
    }
#endif // clang-format on
}

// regalloc issues
inline void J3DPSMulMtxVec(register ROMtxPtr mtx, register Vec* vec,
                           register Vec* dst)
{
	register f32* punit;
	register f32 unit;
	register f32 fr12;
	register f32 fr11;
	register f32 fr10;
	register f32 fr9;
	register f32 fr8;
	register f32 fr6;
	register f32 fr5;
	register f32 fr4;
	register f32 fr3;
	register f32 fr2;
	register f32 fr01;
	register f32 fr00;
#ifdef __MWERKS__ // clang-format off
    asm {
        lis punit, PSMulUnit01@ha
        psq_l fr00, 0(vec), 0, 0
        addi punit, punit, PSMulUnit01@l
        psq_l fr2, 0(mtx), 0, 0
        psq_l unit, 0(punit), 0, 0
        psq_l fr01, 8(vec), 1, 0
        ps_add fr01, unit, fr01
        psq_l fr3, 8(mtx), 1, 0
        ps_mul fr4, fr2, fr00
        psq_l fr8, 12(mtx), 0, 0
        ps_madd fr5, fr3, fr01, fr4
        ps_sum0 fr6, fr5, fr6, fr5
        psq_l fr9, 20(mtx), 1, 0
        ps_mul fr10, fr8, fr00
        psq_st fr6, 0(dst), 1, 0
        ps_madd fr11, fr9, fr01, fr10
        psq_l fr2, 24(mtx), 0, 0
        ps_sum0 fr12, fr11, fr12, fr11
        psq_l fr3, 32(mtx), 1, 0
        ps_mul fr4, fr2, fr00
        psq_st fr12, 4(dst), 1, 0
        ps_madd fr5, fr3, fr01, fr4
        ps_sum0 fr6, fr5, fr6, fr5
        psq_st fr6, 8(dst), 1, 0
    }
#endif // clang-format on
}

// regalloc issues
inline void J3DPSMulMtxVec(register ROMtxPtr mtx, register S16Vec* vec,
                           register S16Vec* dst)
{
	register f32* punit;
	register f32 unit;
	register f32 fr6;
	register f32 fr5;
	register f32 fr4;
	register f32 fr3;
	register f32 fr2;
	register f32 fr01;
	register f32 fr00;
#ifdef __MWERKS__ // clang-format off
    asm {
        lis punit, PSMulUnit01@ha
        psq_l fr00, 0(vec), 0, 7
        addi punit, punit, PSMulUnit01@l
        psq_l fr2, 0(mtx), 0, 0
        psq_l unit, 0(punit), 0, 0
        psq_l fr01, 4(vec), 1, 7
        ps_add fr01, unit, fr01
        psq_l fr3, 8(mtx), 1, 0
        ps_mul fr4, fr2, fr00
        psq_l fr2, 12(mtx), 0, 0
        ps_madd fr5, fr3, fr01, fr4
        ps_sum0 fr6, fr5, fr6, fr5
        psq_l fr3, 20(mtx), 1, 0
        ps_mul fr4, fr2, fr00
        psq_st fr6, 0(dst), 1, 7
        ps_madd fr5, fr3, fr01, fr4
        psq_l fr2, 24(mtx), 0, 0
        ps_sum0 fr6, fr5, fr6, fr5
        psq_l fr3, 32(mtx), 1, 0
        ps_mul fr4, fr2, fr00
        psq_st fr6, 2(dst), 1, 7
        ps_madd fr5, fr3, fr01, fr4
        ps_sum0 fr6, fr5, fr6, fr5
        psq_st fr6, 4(dst), 1, 7
    }
#endif // clang-format on
}

#endif
