#include <dolphin.h>
#include <dolphin/mtx.h>
#include "fake_tgmath.h"

// defines to make asm work
#define qr0 0

asm void PSVECAdd(register Vec *a, register Vec *b, register Vec *c) {
    psq_l f2, Vec.x(a), 0, qr0
    psq_l f4, Vec.x(b), 0, qr0
    ps_add f6, f2, f4
    psq_st f6, Vec.x(c), 0, qr0
    psq_l f3, Vec.z(a), 1, qr0
    psq_l f5, Vec.z(b), 1, qr0
    ps_add f7, f3, f5
    psq_st f7, Vec.z(c), 1, qr0
}

asm void PSVECSubtract(register Vec *a, register Vec *b, register Vec *c) {
    psq_l f2, Vec.x(a), 0, qr0
    psq_l f4, Vec.x(b), 0, qr0
    ps_sub f6, f2, f4
    psq_st f6, Vec.x(c), 0, qr0
    psq_l f3, Vec.z(a), 1, qr0
    psq_l f5, Vec.z(b), 1, qr0
    ps_sub f7, f3, f5
    psq_st f7, Vec.z(c), 1, qr0
}

asm void PSVECScale(register Vec *src, register Vec *dst, register f32 mult) {
    psq_l f2, Vec.x(src), 0, qr0
    ps_merge00 f4, mult, mult
    ps_mul f6, f2, f4
    psq_st f6, Vec.x(dst), 0, qr0
    psq_l f3, Vec.z(src), 1, qr0
    ps_mul f7, f3, f4
    psq_st f7, Vec.z(dst), 1, qr0
}

void PSVECNormalize(register Vec *vec1, register Vec *dst) {
    register float c_half = 0.5f;
    register float c_three = 3.0f;
    register float v1_xy;
    register float v1_z;
    register float xx_zz;
    register float xx_yy;
    register float sqsum;
    register float rsqrt;
    register float nwork0;
    register float nwork1;

    asm {
        psq_l v1_xy, Vec.x(vec1), 0, qr0
        ps_mul xx_yy, v1_xy, v1_xy
        psq_l v1_z, Vec.z(vec1), 1, qr0
        ps_madd xx_zz, v1_z, v1_z, xx_yy
        ps_sum0 sqsum, xx_zz, v1_z, xx_yy
        ps_rsqrte rsqrt, sqsum
        fmuls nwork0, rsqrt, rsqrt
        fmuls nwork1, rsqrt, c_half
        fmuls nwork0, nwork0, sqsum
        fsubs nwork0, c_three, nwork0
        fmuls rsqrt, nwork0, nwork1
        ps_muls0 v1_xy, v1_xy, rsqrt
        psq_st v1_xy, Vec.x(dst), 0, qr0
        ps_muls0 v1_z, v1_z, rsqrt
        psq_st v1_z, Vec.z(dst), 1, qr0
    }
}

asm f32 PSVECDotProduct(register Vec *vec1, register Vec *vec2) {
    psq_l f2, Vec.y(vec1), 0, qr0
    psq_l f3, Vec.y(vec2), 0, qr0
    ps_mul f2, f2, f3
    psq_l f5, Vec.x(vec1), 0, qr0
    psq_l f4, Vec.x(vec2), 0, qr0
    ps_madd f3, f5, f4, f2
    ps_sum0 f1, f3, f2, f2
}

asm void PSVECCrossProduct(register Vec *vec1, register Vec *vec2, register Vec *dst) {
    psq_l f1, Vec.x(vec2), 0, qr0
    lfs f2, Vec.z(vec1)
    psq_l f0, Vec.x(vec1), 0, qr0
    ps_merge10 f6, f1, f1
    lfs f3, Vec.z(vec2)
    ps_mul f4, f1, f2
    ps_muls0 f7, f1, f0
    ps_msub f5, f0, f3, f4
    ps_msub f8, f0, f6, f7
    ps_merge11 f9, f5, f5
    ps_merge01 f10, f5, f8
    psq_st f9, Vec.x(dst), 1, qr0
    ps_neg f10, f10
    psq_st f10, Vec.y(dst), 0, qr0
}

asm f32 PSVECSquareDistance(register Vec *vec1, register Vec *vec2) {
    psq_l f2, Vec.y(vec1), 0, qr0
    psq_l f3, Vec.y(vec2), 0, qr0
    ps_sub f2, f2, f3
    psq_l f5, Vec.x(vec1), 0, qr0
    psq_l f6, Vec.x(vec2), 0, qr0
    ps_mul f4, f2, f2
    ps_sub f6, f5, f6
    ps_madd f5, f6, f6, f4
    ps_sum0 f1, f5, f4, f4    
}
