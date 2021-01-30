.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global CLBCalcRatio$$0l$$1__Flll
CLBCalcRatio$$0l$$1__Flll:
/* 80200684 001FD5C4  94 21 FF D8 */	stwu r1, -0x28(r1)
/* 80200688 001FD5C8  7C 03 20 00 */	cmpw r3, r4
/* 8020068C 001FD5CC  C0 22 D3 B0 */	lfs f1, $$21680-_SDA2_BASE_(r2)
/* 80200690 001FD5D0  41 82 00 48 */	beq lbl_802006D8
/* 80200694 001FD5D4  7C 03 20 50 */	subf r0, r3, r4
/* 80200698 001FD5D8  C8 42 D3 B8 */	lfd f2, $$21683-_SDA2_BASE_(r2)
/* 8020069C 001FD5DC  6C 00 80 00 */	xoris r0, r0, 0x8000
/* 802006A0 001FD5E0  C0 22 D3 B4 */	lfs f1, $$21681-_SDA2_BASE_(r2)
/* 802006A4 001FD5E4  90 01 00 1C */	stw r0, 0x1c(r1)
/* 802006A8 001FD5E8  3C 00 43 30 */	lis r0, 0x4330
/* 802006AC 001FD5EC  7C 63 28 50 */	subf r3, r3, r5
/* 802006B0 001FD5F0  90 01 00 18 */	stw r0, 0x18(r1)
/* 802006B4 001FD5F4  6C 63 80 00 */	xoris r3, r3, 0x8000
/* 802006B8 001FD5F8  C8 01 00 18 */	lfd f0, 0x18(r1)
/* 802006BC 001FD5FC  90 61 00 24 */	stw r3, 0x24(r1)
/* 802006C0 001FD600  EC 00 10 28 */	fsubs f0, f0, f2
/* 802006C4 001FD604  90 01 00 20 */	stw r0, 0x20(r1)
/* 802006C8 001FD608  EC 01 00 24 */	fdivs f0, f1, f0
/* 802006CC 001FD60C  C8 21 00 20 */	lfd f1, 0x20(r1)
/* 802006D0 001FD610  EC 21 10 28 */	fsubs f1, f1, f2
/* 802006D4 001FD614  EC 21 00 32 */	fmuls f1, f1, f0
lbl_802006D8:
/* 802006D8 001FD618  38 21 00 28 */	addi r1, r1, 0x28
/* 802006DC 001FD61C  4E 80 00 20 */	blr 

.section .sdata2, "wa"  # 0x8040B460 - 0x80414020
.global $$21680
$$21680:
	.incbin "baserom.dol", 0x3E90B0, 0x4
.global $$21681
$$21681:
	.incbin "baserom.dol", 0x3E90B4, 0x4
.global $$21683
$$21683:
	.incbin "baserom.dol", 0x3E90B8, 0x8
