.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global getY__Q28JASystem9TInstRandCFii
getY__Q28JASystem9TInstRandCFii:
/* 8005A940 00057880  7C 08 02 A6 */	mflr r0
/* 8005A944 00057884  90 01 00 04 */	stw r0, 4(r1)
/* 8005A948 00057888  94 21 FF C8 */	stwu r1, -0x38(r1)
/* 8005A94C 0005788C  93 E1 00 34 */	stw r31, 0x34(r1)
/* 8005A950 00057890  3B E3 00 00 */	addi r31, r3, 0
/* 8005A954 00057894  88 0D 90 A0 */	lbz r0, init$332@sda21(r13)
/* 8005A958 00057898  7C 00 07 75 */	extsb. r0, r0
/* 8005A95C 0005789C  40 82 00 18 */	bne lbl_8005A974
/* 8005A960 000578A0  38 6D 90 A4 */	addi r3, r13, oRandom$331@sda21
/* 8005A964 000578A4  38 80 00 00 */	li r4, 0
/* 8005A968 000578A8  48 00 E1 B5 */	bl __ct__Q25JMath13TRandom_fast_FUl
/* 8005A96C 000578AC  38 00 00 01 */	li r0, 1
/* 8005A970 000578B0  98 0D 90 A0 */	stb r0, init$332@sda21(r13)
lbl_8005A974:
/* 8005A974 000578B4  3C 60 00 19 */	lis r3, 0x0019660D@ha
/* 8005A978 000578B8  80 8D 90 A4 */	lwz r4, oRandom$331@sda21(r13)
/* 8005A97C 000578BC  38 03 66 0D */	addi r0, r3, 0x0019660D@l
/* 8005A980 000578C0  C0 02 86 0C */	lfs f0, $$2346@sda21(r2)
/* 8005A984 000578C4  7C 64 01 D6 */	mullw r3, r4, r0
/* 8005A988 000578C8  C0 82 86 08 */	lfs f4, $$2345@sda21(r2)
/* 8005A98C 000578CC  C0 22 86 10 */	lfs f1, $$2347@sda21(r2)
/* 8005A990 000578D0  3C 63 3C 6F */	addis r3, r3, 0x3c6f
/* 8005A994 000578D4  38 03 F3 5F */	addi r0, r3, -3233
/* 8005A998 000578D8  90 0D 90 A4 */	stw r0, oRandom$331@sda21(r13)
/* 8005A99C 000578DC  80 0D 90 A4 */	lwz r0, oRandom$331@sda21(r13)
/* 8005A9A0 000578E0  54 00 BA 7E */	srwi r0, r0, 9
/* 8005A9A4 000578E4  64 00 3F 80 */	oris r0, r0, 0x3f80
/* 8005A9A8 000578E8  90 01 00 2C */	stw r0, 0x2c(r1)
/* 8005A9AC 000578EC  C0 61 00 2C */	lfs f3, 0x2c(r1)
/* 8005A9B0 000578F0  80 01 00 3C */	lwz r0, 0x3c(r1)
/* 8005A9B4 000578F4  EC 63 00 28 */	fsubs f3, f3, f0
/* 8005A9B8 000578F8  C0 5F 00 0C */	lfs f2, 0xc(r31)
/* 8005A9BC 000578FC  C0 1F 00 08 */	lfs f0, 8(r31)
/* 8005A9C0 00057900  7C 08 03 A6 */	mtlr r0
/* 8005A9C4 00057904  83 E1 00 34 */	lwz r31, 0x34(r1)
/* 8005A9C8 00057908  EC 64 00 F2 */	fmuls f3, f4, f3
/* 8005A9CC 0005790C  38 21 00 38 */	addi r1, r1, 0x38
/* 8005A9D0 00057910  EC 23 08 28 */	fsubs f1, f3, f1
/* 8005A9D4 00057914  EC 21 00 B2 */	fmuls f1, f1, f2
/* 8005A9D8 00057918  EC 21 00 2A */	fadds f1, f1, f0
/* 8005A9DC 0005791C  4E 80 00 20 */	blr 

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global __vt__Q28JASystem9TInstRand
__vt__Q28JASystem9TInstRand:
	.incbin "baserom.dol", 0x3A7EA8, 0x10

.section .sdata2, "wa"  # 0x8040B460 - 0x80414020
$$2345:
	.incbin "baserom.dol", 0x3E4308, 0x4
$$2346:
	.incbin "baserom.dol", 0x3E430C, 0x4
$$2347:
	.incbin "baserom.dol", 0x3E4310, 0x8

.section .sbss, "wa"  # 0x804097C0 - 0x8040B45C
init$332:
	.skip 0x4
oRandom$331:
	.skip 0x4
