#include <dolphin.h>
#include <dolphin/mtx.h>
#include <macros.h>

#define qr0 0

// unsorted externs
extern f32 sinf(f32);
extern f32 cosf(f32);

// .sbss
static float Unit01[2] = { 0.0f, 1.0f };

void PSMTXIdentity(register Mtx m)
{
	register f32 c_zero = 0.0f;
	register f32 c_one  = 1.0f;
	register f32 c_01;
	register f32 c_10;

#ifdef __MWERKS__ // clang-format off
	asm {
		psq_st c_zero, 8(m), 0, qr0
		ps_merge01 c_01, c_zero, c_one
		psq_st c_zero, 24(m), 0, qr0
		ps_merge10 c_10, c_one, c_zero
		psq_st c_zero, 32(m), 0, qr0
		psq_st c_01, 16(m), 0, qr0
		psq_st c_10, 0(m), 0, qr0
		psq_st c_10, 40(m), 0, qr0
	}
#endif // clang-format on
}

asm void PSMTXCopy(register Mtx src, register Mtx dst)
{
#ifdef __MWERKS__ // clang-format off
	psq_l f0, 0(src), 0, qr0
	psq_st f0, 0(dst), 0, qr0
	psq_l f1, 8(src), 0, qr0
	psq_st f1, 8(dst), 0, qr0
	psq_l f2, 16(src), 0, qr0
	psq_st f2, 16(dst), 0, qr0
	psq_l f3, 24(src), 0, qr0
	psq_st f3, 24(dst), 0, qr0
	psq_l f4, 32(src), 0, qr0
	psq_st f4, 32(dst), 0, qr0
	psq_l f5, 40(src), 0, qr0
	psq_st f5, 40(dst), 0, qr0
#endif // clang-format on
}

asm void PSMTXConcat(register Mtx mA, register Mtx mB, register Mtx mAB)
{
#ifdef __MWERKS__ // clang-format off
	nofralloc
	stwu r1, -64(r1)
	psq_l f0, 0(mA), 0, qr0
	stfd f14, 8(r1)
	psq_l f6, 0(mB), 0, qr0
	lis r6, Unit01@ha
	psq_l f7, 8(mB), 0, qr0
	stfd f15, 16(r1)
	addi r6, r6, Unit01@l
	stfd f31, 40(r1)
	psq_l f8, 16(mB), 0, qr0
	ps_muls0 f12, f6, f0
	psq_l f2, 16(mA), 0, qr0
	ps_muls0 f13, f7, f0
	psq_l f31, 0(r6), 0, qr0
	ps_muls0 f14, f6, f2
	psq_l f9, 24(mB), 0, qr0
	ps_muls0 f15, f7, f2
	psq_l f1, 8(mA), 0, qr0
	ps_madds1 f12, f8, f0, f12
	psq_l f3, 24(mA), 0, qr0
	ps_madds1 f14, f8, f2, f14
	psq_l f10, 32(mB), 0, qr0
	ps_madds1 f13, f9, f0, f13
	psq_l f11, 40(mB), 0, qr0
	ps_madds1 f15, f9, f2, f15
	psq_l f4, 32(mA), 0, qr0
	psq_l f5, 40(mA), 0, qr0
	ps_madds0 f12, f10, f1, f12
	ps_madds0 f13, f11, f1, f13
	ps_madds0 f14, f10, f3, f14
	ps_madds0 f15, f11, f3, f15
	psq_st f12, 0(mAB), 0, qr0
	ps_muls0 f2, f6, f4
	ps_madds1 f13, f31, f1, f13
	ps_muls0 f0, f7, f4
	psq_st f14, 16(mAB), 0, qr0
	ps_madds1 f15, f31, f3, f15
	psq_st f13, 8(mAB), 0, qr0
	ps_madds1 f2, f8, f4, f2
	ps_madds1 f0, f9, f4, f0
	ps_madds0 f2, f10, f5, f2
	lfd f14, 8(r1)
	psq_st f15, 24(mAB), 0, qr0
	ps_madds0 f0, f11, f5, f0
	psq_st f2, 32(mAB), 0, qr0
	ps_madds1 f0, f31, f5, f0
	lfd f15, 16(r1)
	psq_st f0, 40(mAB), 0, qr0
	lfd f31, 40(r1)
	addi r1, r1, 64
	blr
#endif // clang-format on
}

asm u32 PSMTXInverse(register Mtx src, register Mtx inv)
{
#ifdef __MWERKS__ // clang-format off
	psq_l f0, 0(src), 1, qr0
	psq_l f1, 4(src), 0, qr0
	psq_l f2, 16(src), 1, qr0
	ps_merge10 f6, f1, f0
	psq_l f3, 20(src), 0, qr0
	psq_l f4, 32(src), 1, qr0
	ps_merge10 f7, f3, f2
	psq_l f5, 36(src), 0, qr0
	ps_mul f11, f3, f6
	ps_mul f13, f5, f7
	ps_merge10 f8, f5, f4
	ps_msub f11, f1, f7, f11
	ps_mul f12, f1, f8
	ps_msub f13, f3, f8, f13
	ps_mul f10, f3, f4
	ps_msub f12, f5, f6, f12
	ps_mul f9, f0, f5
	ps_mul f8, f1, f2
	ps_sub f6, f6, f6
	ps_msub f10, f2, f5, f10
	ps_mul f7, f0, f13
	ps_msub f9, f1, f4, f9
	ps_madd f7, f2, f12, f7
	ps_msub f8, f0, f3, f8
	ps_madd f7, f4, f11, f7
	ps_cmpo0 cr0, f7, f6
	bne skip_return
	li r3, 0
	blr
skip_return:
	ps_res f0, f7
	ps_add f6, f0, f0
	ps_mul f5, f0, f0
	ps_nmsub f0, f7, f5, f6
	ps_add f6, f0, f0
	ps_mul f5, f0, f0
	ps_nmsub f0, f7, f5, f6
	lfs f1, 12(src)
	ps_muls0 f13, f13, f0
	lfs f2, 28(src)
	ps_muls0 f12, f12, f0
	lfs f3, 44(src)
	ps_muls0 f11, f11, f0
	ps_merge00 f5, f13, f12
	ps_muls0 f10, f10, f0
	ps_merge11 f4, f13, f12
	ps_muls0 f9, f9, f0
	psq_st f5, 0(inv), 0, qr0
	ps_mul f6, f13, f1
	psq_st f4, 16(inv), 0, qr0
	ps_muls0 f8, f8, f0
	ps_madd f6, f12, f2, f6
	psq_st f10, 32(inv), 1, qr0
	ps_nmadd f6, f11, f3, f6
	psq_st f9, 36(inv), 1, qr0
	ps_mul f7, f10, f1
	ps_merge00 f5, f11, f6
	psq_st f8, 40(inv), 1, qr0
	ps_merge11 f4, f11, f6
	psq_st f5, 8(inv), 0, qr0
	ps_madd f7, f9, f2, f7
	psq_st f4, 24(inv), 0, qr0
	ps_nmadd f7, f8, f3, f7
	li r3, 1
	psq_st f7, 44(inv), 1, qr0
#endif // clang-format on
}

void C_MTXLookAt(Mtx m, Vec* camPos, Vec* camUp, Vec* target)
{
	Vec vLook;
	Vec vRight;
	Vec vUp;

	ASSERTMSGLINE(0x63E, m, "MTXLookAt():  NULL MtxPtr 'm' ");
	ASSERTMSGLINE(0x63F, camPos, "MTXLookAt():  NULL VecPtr 'camPos' ");
	ASSERTMSGLINE(0x640, camUp, "MTXLookAt():  NULL VecPtr 'camUp' ");
	ASSERTMSGLINE(0x641, target, "MTXLookAt():  NULL Point3dPtr 'target' ");

	vLook.x = camPos->x - target->x;
	vLook.y = camPos->y - target->y;
	vLook.z = camPos->z - target->z;
	VECNormalize(&vLook, &vLook);
	VECCrossProduct(camUp, &vLook, &vRight);
	VECNormalize(&vRight, &vRight);
	VECCrossProduct(&vLook, &vRight, &vUp);
	m[0][0] = vRight.x;
	m[0][1] = vRight.y;
	m[0][2] = vRight.z;
	m[0][3] = -((camPos->z * vRight.z)
	            + ((camPos->x * vRight.x) + (camPos->y * vRight.y)));
	m[1][0] = vUp.x;
	m[1][1] = vUp.y;
	m[1][2] = vUp.z;
	m[1][3]
	    = -((camPos->z * vUp.z) + ((camPos->x * vUp.x) + (camPos->y * vUp.y)));
	m[2][0] = vLook.x;
	m[2][1] = vLook.y;
	m[2][2] = vLook.z;
	m[2][3] = -((camPos->z * vLook.z)
	            + ((camPos->x * vLook.x) + (camPos->y * vLook.y)));
}

void C_MTXLightFrustum(Mtx m, f32 t, f32 b, f32 l, f32 r, f32 n, f32 scaleS,
                       f32 scaleT, f32 transS, f32 transT)
{
	f32 tmp;

	ASSERTMSGLINE(0x6A2, m, "MTXLightFrustum():  NULL MtxPtr 'm' ");
	ASSERTMSGLINE(0x6A3, (t != b),
	              "MTXLightFrustum():  't' and 'b' clipping planes are equal ");
	ASSERTMSGLINE(0x6A4, (l != r),
	              "MTXLightFrustum():  'l' and 'r' clipping planes are equal ");

	tmp     = 1 / (r - l);
	m[0][0] = (scaleS * (2 * n * tmp));
	m[0][1] = 0;
	m[0][2] = (scaleS * (tmp * (r + l))) - transS;
	m[0][3] = 0;
	tmp     = 1 / (t - b);
	m[1][0] = 0;
	m[1][1] = (scaleT * (2 * n * tmp));
	m[1][2] = (scaleT * (tmp * (t + b))) - transT;
	m[1][3] = 0;
	m[2][0] = 0;
	m[2][1] = 0;
	m[2][2] = -1;
	m[2][3] = 0;
}

void C_MTXLightPerspective(Mtx m, f32 fovY, f32 aspect, f32 scaleS, f32 scaleT,
                           f32 transS, f32 transT)
{
	f32 angle;
	f32 cot;

	ASSERTMSGLINE(0x6DF, m, "MTXLightPerspective():  NULL MtxPtr 'm' ");
	ASSERTMSGLINE(0x6E0, (fovY > 0.0) && (fovY < 180.0),
	              "MTXLightPerspective():  'fovY' out of range ");
	ASSERTMSGLINE(0x6E1, 0 != aspect, "MTXLightPerspective():  'aspect' is 0 ");

	angle   = (0.5f * fovY);
	angle   = angle * 0.017453293f;
	cot     = 1 / tanf(angle);
	m[0][0] = (scaleS * (cot / aspect));
	m[0][1] = 0;
	m[0][2] = -transS;
	m[0][3] = 0;
	m[1][0] = 0;
	m[1][1] = (cot * scaleT);
	m[1][2] = -transT;
	m[1][3] = 0;
	m[2][0] = 0;
	m[2][1] = 0;
	m[2][2] = -1;
	m[2][3] = 0;
}

void C_MTXLightOrtho(Mtx m, f32 t, f32 b, f32 l, f32 r, f32 scaleS, f32 scaleT,
                     f32 transS, f32 transT)
{
	f32 tmp;

	ASSERTMSGLINE(0x720, m, "MTXLightOrtho():  NULL MtxPtr 'm' ");
	ASSERTMSGLINE(0x721, (t != b),
	              "MTXLightOrtho():  't' and 'b' clipping planes are equal ");
	ASSERTMSGLINE(0x722, (l != r),
	              "MTXLightOrtho():  'l' and 'r' clipping planes are equal ");
	tmp     = 1 / (r - l);
	m[0][0] = (2 * tmp * scaleS);
	m[0][1] = 0;
	m[0][2] = 0;
	m[0][3] = (transS + (scaleS * (tmp * -(r + l))));
	tmp     = 1 / (t - b);
	m[1][0] = 0;
	m[1][1] = (2 * tmp * scaleT);
	m[1][2] = 0;
	m[1][3] = (transT + (scaleT * (tmp * -(t + b))));
	m[2][0] = 0;
	m[2][1] = 0;
	m[2][2] = 0;
	m[2][3] = 1;
}
