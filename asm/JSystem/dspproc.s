.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0

.global DSPReleaseHalt__Fv
DSPReleaseHalt__Fv:
/* 80081C00 0007EB40  7C 08 02 A6 */	mflr r0
/* 80081C04 0007EB44  90 01 00 04 */	stw r0, 4(r1)
/* 80081C08 0007EB48  94 21 FF E0 */	stwu r1, -0x20(r1)
/* 80081C0C 0007EB4C  BF C1 00 18 */	stmw r30, 0x18(r1)
/* 80081C10 0007EB50  4B FD DB 51 */	bl DSP_CreateMap__Fv
/* 80081C14 0007EB54  3B C4 00 00 */	addi r30, r4, 0
/* 80081C18 0007EB58  38 A0 00 20 */	li r5, 0x20
/* 80081C1C 0007EB5C  48 00 14 35 */	bl __shr2u
/* 80081C20 0007EB60  90 81 00 08 */	stw r4, 8(r1)
/* 80081C24 0007EB64  38 00 FF FF */	li r0, -1
/* 80081C28 0007EB68  7F C0 00 38 */	and r0, r30, r0
/* 80081C2C 0007EB6C  38 61 00 08 */	addi r3, r1, 8
/* 80081C30 0007EB70  90 01 00 0C */	stw r0, 0xc(r1)
/* 80081C34 0007EB74  38 80 00 00 */	li r4, 0
/* 80081C38 0007EB78  38 A0 00 00 */	li r5, 0
/* 80081C3C 0007EB7C  48 00 02 65 */	bl DSPSendCommands2__FPUlUlPFUs_v
/* 80081C40 0007EB80  80 01 00 24 */	lwz r0, 0x24(r1)
/* 80081C44 0007EB84  BB C1 00 18 */	lmw r30, 0x18(r1)
/* 80081C48 0007EB88  38 21 00 20 */	addi r1, r1, 0x20
/* 80081C4C 0007EB8C  7C 08 03 A6 */	mtlr r0
/* 80081C50 0007EB90  4E 80 00 20 */	blr 
/* 80081C54 0007EB94  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */
/* 80081C58 0007EB98  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */
/* 80081C5C 0007EB9C  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */

.global setup_callback__FUs
setup_callback__FUs:
/* 80081C60 0007EBA0  38 00 00 00 */	li r0, 0
/* 80081C64 0007EBA4  90 0D 92 60 */	stw r0, flag-_SDA_BASE_(r13)
/* 80081C68 0007EBA8  4E 80 00 20 */	blr 
/* 80081C6C 0007EBAC  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */
/* 80081C70 0007EBB0  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */
/* 80081C74 0007EBB4  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */
/* 80081C78 0007EBB8  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */
/* 80081C7C 0007EBBC  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */

.global DsetupTable__FUlUlUlUlUl
DsetupTable__FUlUlUlUlUl:
/* 80081C80 0007EBC0  7C 08 02 A6 */	mflr r0
/* 80081C84 0007EBC4  39 00 00 01 */	li r8, 1
/* 80081C88 0007EBC8  90 01 00 04 */	stw r0, 4(r1)
/* 80081C8C 0007EBCC  54 60 04 3E */	clrlwi r0, r3, 0x10
/* 80081C90 0007EBD0  64 00 81 00 */	oris r0, r0, 0x8100
/* 80081C94 0007EBD4  3C 60 80 08 */	lis r3, setup_callback__FUs@ha
/* 80081C98 0007EBD8  94 21 FF D0 */	stwu r1, -0x30(r1)
/* 80081C9C 0007EBDC  90 01 00 1C */	stw r0, 0x1c(r1)
/* 80081CA0 0007EBE0  38 03 1C 60 */	addi r0, r3, setup_callback__FUs@l
/* 80081CA4 0007EBE4  38 61 00 1C */	addi r3, r1, 0x1c
/* 80081CA8 0007EBE8  90 81 00 20 */	stw r4, 0x20(r1)
/* 80081CAC 0007EBEC  38 80 00 05 */	li r4, 5
/* 80081CB0 0007EBF0  90 A1 00 24 */	stw r5, 0x24(r1)
/* 80081CB4 0007EBF4  7C 05 03 78 */	mr r5, r0
/* 80081CB8 0007EBF8  90 C1 00 28 */	stw r6, 0x28(r1)
/* 80081CBC 0007EBFC  90 E1 00 2C */	stw r7, 0x2c(r1)
/* 80081CC0 0007EC00  91 0D 92 60 */	stw r8, flag-_SDA_BASE_(r13)
/* 80081CC4 0007EC04  48 00 01 DD */	bl DSPSendCommands2__FPUlUlPFUs_v
lbl_80081CC8:
/* 80081CC8 0007EC08  80 0D 92 60 */	lwz r0, flag-_SDA_BASE_(r13)
/* 80081CCC 0007EC0C  2C 00 00 00 */	cmpwi r0, 0
/* 80081CD0 0007EC10  40 82 FF F8 */	bne lbl_80081CC8
/* 80081CD4 0007EC14  80 01 00 34 */	lwz r0, 0x34(r1)
/* 80081CD8 0007EC18  38 21 00 30 */	addi r1, r1, 0x30
/* 80081CDC 0007EC1C  7C 08 03 A6 */	mtlr r0
/* 80081CE0 0007EC20  4E 80 00 20 */	blr 
/* 80081CE4 0007EC24  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */
/* 80081CE8 0007EC28  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */
/* 80081CEC 0007EC2C  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */
/* 80081CF0 0007EC30  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */
/* 80081CF4 0007EC34  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */
/* 80081CF8 0007EC38  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */
/* 80081CFC 0007EC3C  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */

.global DsetMixerLevel__Ff
DsetMixerLevel__Ff:
/* 80081D00 0007EC40  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 80081D04 0007EC44  C0 02 89 50 */	lfs f0, $$271-_SDA2_BASE_(r2)
/* 80081D08 0007EC48  EC 00 00 72 */	fmuls f0, f0, f1
/* 80081D0C 0007EC4C  FC 00 00 1E */	fctiwz f0, f0
/* 80081D10 0007EC50  D8 01 00 10 */	stfd f0, 0x10(r1)
/* 80081D14 0007EC54  80 01 00 14 */	lwz r0, 0x14(r1)
/* 80081D18 0007EC58  B0 0D 82 18 */	sth r0, DSP_MIXERLEVEL-_SDA_BASE_(r13)
/* 80081D1C 0007EC5C  38 21 00 18 */	addi r1, r1, 0x18
/* 80081D20 0007EC60  4E 80 00 20 */	blr 
/* 80081D24 0007EC64  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */
/* 80081D28 0007EC68  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */
/* 80081D2C 0007EC6C  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */
/* 80081D30 0007EC70  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */
/* 80081D34 0007EC74  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */
/* 80081D38 0007EC78  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */
/* 80081D3C 0007EC7C  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */

.global DsyncFrame__FUlUlUl
DsyncFrame__FUlUlUl:
/* 80081D40 0007EC80  7C 08 02 A6 */	mflr r0
/* 80081D44 0007EC84  54 66 82 1E */	rlwinm r6, r3, 0x10, 8, 0xf
/* 80081D48 0007EC88  90 01 00 04 */	stw r0, 4(r1)
/* 80081D4C 0007EC8C  64 C6 82 00 */	oris r6, r6, 0x8200
/* 80081D50 0007EC90  94 21 FF D8 */	stwu r1, -0x28(r1)
/* 80081D54 0007EC94  A0 0D 82 18 */	lhz r0, DSP_MIXERLEVEL-_SDA_BASE_(r13)
/* 80081D58 0007EC98  38 61 00 18 */	addi r3, r1, 0x18
/* 80081D5C 0007EC9C  7C C0 03 78 */	or r0, r6, r0
/* 80081D60 0007ECA0  90 01 00 18 */	stw r0, 0x18(r1)
/* 80081D64 0007ECA4  90 81 00 1C */	stw r4, 0x1c(r1)
/* 80081D68 0007ECA8  38 80 00 03 */	li r4, 3
/* 80081D6C 0007ECAC  90 A1 00 20 */	stw r5, 0x20(r1)
/* 80081D70 0007ECB0  38 A0 00 00 */	li r5, 0
/* 80081D74 0007ECB4  48 00 01 2D */	bl DSPSendCommands2__FPUlUlPFUs_v
/* 80081D78 0007ECB8  80 01 00 2C */	lwz r0, 0x2c(r1)
/* 80081D7C 0007ECBC  38 21 00 28 */	addi r1, r1, 0x28
/* 80081D80 0007ECC0  7C 08 03 A6 */	mtlr r0
/* 80081D84 0007ECC4  4E 80 00 20 */	blr 
/* 80081D88 0007ECC8  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */
/* 80081D8C 0007ECCC  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */
/* 80081D90 0007ECD0  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */
/* 80081D94 0007ECD4  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */
/* 80081D98 0007ECD8  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */
/* 80081D9C 0007ECDC  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */
