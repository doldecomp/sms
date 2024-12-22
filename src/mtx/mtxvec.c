#include <dolphin.h>
#include <dolphin/mtx.h>

#define qr0 0

asm void PSMTXMultVec(register Mtx44 m, register Vec *src, register Vec *dst) {
    nofralloc
    psq_l f0, Vec.x(src), 0, qr0
    psq_l f2, 0(m), 0, qr0
    psq_l f1, Vec.z(src), 1, qr0
    ps_mul f4, f2, f0
    psq_l f3, 8(m), 0, qr0
    ps_madd f5, f3, f1, f4
    psq_l f8, 16(m), 0, qr0
    ps_sum0 f6, f5, f6, f5
    psq_l f9, 24(m), 0, qr0
    ps_mul f10, f8, f0
    psq_st f6, Vec.x(dst), 1, qr0
    ps_madd f11, f9, f1, f10
    psq_l f2, 32(m), 0, qr0
    ps_sum0 f12, f11, f12, f11
    psq_l f3, 40(m), 0, qr0
    ps_mul f4, f2, f0
    psq_st f12, Vec.y(dst), 1, qr0
    ps_madd f5, f3, f1, f4
    ps_sum0 f6, f5, f6, f5
    psq_st f6, Vec.z(dst), 1, qr0
    blr
}

asm void PSMTXMultVecArray(register Mtx m, register Vec *srcBase, register Vec *dstBase, register u32 count) {
	psq_l f0, 0(m), 0, qr0
	subi count, count, 1
	psq_l f6, Vec.x(srcBase), 0, qr0
	mtctr count
	psq_l f7, Vec.z(srcBase), 1, qr0
	psq_l f1, 8(m), 0, qr0
	addi srcBase, srcBase, 8
	psq_l f2, 16(m), 0, qr0
	subi dstBase, dstBase, 4
    
	psq_l f3, 24(m), 0, qr0
	ps_mul f8, f0, f6
	psq_l f4, 32(m), 0, qr0
	ps_mul f10, f2, f6
	psq_l f5, 40(m), 0, qr0
	ps_mul f12, f4, f6
	psq_lu f6, Vec.y(srcBase), 0, qr0
	ps_madd f8, f1, f7, f8
	ps_madd f10, f3, f7, f10
	ps_madd f12, f5, f7, f12
	psq_lu f7, Vec.z(srcBase), 1, qr0
	ps_sum0 f9, f8, f8, f8
loop:
	ps_sum0 f11, f10, f10, f10
	ps_mul f8, f0, f6
	ps_sum0 f13, f12, f12, f12
	ps_mul f10, f2, f6
	psq_stu f9, Vec.y(dstBase), 1, qr0
	ps_mul f12, f4, f6
	psq_stu f11, Vec.y(dstBase), 1, qr0
	ps_madd f8, f1, f7, f8
	psq_stu f13, Vec.y(dstBase), 1, qr0
	ps_madd f10, f3, f7, f10
	psq_lu f6, Vec.y(srcBase), 0, qr0
	ps_madd f12, f5, f7, f12
	psq_lu f7, Vec.z(srcBase), 1, qr0
	ps_sum0 f9, f8, f8, f8
	bdnz loop
	ps_sum0 f11, f10, f10, f10
	ps_sum0 f13, f12, f12, f12
	psq_stu f9, Vec.y(dstBase), 1, qr0
	psq_stu f11, Vec.y(dstBase), 1, qr0
	psq_stu f13, Vec.y(dstBase), 1, qr0
}
