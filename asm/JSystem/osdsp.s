.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global DSPAddTask
DSPAddTask:
/* 800820A0 0007EFE0  7C 08 02 A6 */	mflr r0
/* 800820A4 0007EFE4  90 01 00 04 */	stw r0, 4(r1)
/* 800820A8 0007EFE8  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 800820AC 0007EFEC  BF C1 00 10 */	stmw r30, 0x10(r1)
/* 800820B0 0007EFF0  3B C3 00 00 */	addi r30, r3, 0
/* 800820B4 0007EFF4  80 0D 92 80 */	lwz r0, DSP_prior_task@sda21(r13)
/* 800820B8 0007EFF8  28 00 00 00 */	cmplwi r0, 0
/* 800820BC 0007EFFC  40 82 00 1C */	bne lbl_800820D8
/* 800820C0 0007F000  3C 60 80 37 */	lis r3, $$2347@ha
/* 800820C4 0007F004  38 63 14 70 */	addi r3, r3, $$2347@l
/* 800820C8 0007F008  4C C6 31 82 */	crclr 6
/* 800820CC 0007F00C  48 00 CE 99 */	bl OSReport
/* 800820D0 0007F010  38 60 00 00 */	li r3, 0
/* 800820D4 0007F014  48 00 00 34 */	b lbl_80082108
lbl_800820D8:
/* 800820D8 0007F018  48 00 E0 F5 */	bl OSDisableInterrupts
/* 800820DC 0007F01C  38 03 00 00 */	addi r0, r3, 0
/* 800820E0 0007F020  38 7E 00 00 */	addi r3, r30, 0
/* 800820E4 0007F024  7C 1F 03 78 */	mr r31, r0
/* 800820E8 0007F028  48 01 CA 31 */	bl __DSP_insert_task
/* 800820EC 0007F02C  38 60 00 00 */	li r3, 0
/* 800820F0 0007F030  38 00 00 01 */	li r0, 1
/* 800820F4 0007F034  90 7E 00 00 */	stw r3, 0(r30)
/* 800820F8 0007F038  7F E3 FB 78 */	mr r3, r31
/* 800820FC 0007F03C  90 1E 00 08 */	stw r0, 8(r30)
/* 80082100 0007F040  48 00 E0 F5 */	bl OSRestoreInterrupts
/* 80082104 0007F044  7F C3 F3 78 */	mr r3, r30
lbl_80082108:
/* 80082108 0007F048  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 8008210C 0007F04C  BB C1 00 10 */	lmw r30, 0x10(r1)
/* 80082110 0007F050  38 21 00 18 */	addi r1, r1, 0x18
/* 80082114 0007F054  7C 08 03 A6 */	mtlr r0
/* 80082118 0007F058  4E 80 00 20 */	blr 
/* 8008211C 0007F05C  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */

.global DSPAddPriorTask__FP15STRUCT_DSP_TASK
DSPAddPriorTask__FP15STRUCT_DSP_TASK:
/* 80082120 0007F060  7C 08 02 A6 */	mflr r0
/* 80082124 0007F064  90 01 00 04 */	stw r0, 4(r1)
/* 80082128 0007F068  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 8008212C 0007F06C  BF C1 00 10 */	stmw r30, 0x10(r1)
/* 80082130 0007F070  3B C3 00 00 */	addi r30, r3, 0
/* 80082134 0007F074  80 0D 92 80 */	lwz r0, DSP_prior_task@sda21(r13)
/* 80082138 0007F078  28 00 00 00 */	cmplwi r0, 0
/* 8008213C 0007F07C  41 82 00 18 */	beq lbl_80082154
/* 80082140 0007F080  3C 60 80 37 */	lis r3, $$2351@ha
/* 80082144 0007F084  38 63 14 8C */	addi r3, r3, $$2351@l
/* 80082148 0007F088  4C C6 31 82 */	crclr 6
/* 8008214C 0007F08C  48 00 CE 19 */	bl OSReport
/* 80082150 0007F090  48 00 00 30 */	b lbl_80082180
lbl_80082154:
/* 80082154 0007F094  48 00 E0 79 */	bl OSDisableInterrupts
/* 80082158 0007F098  93 CD 92 80 */	stw r30, DSP_prior_task@sda21(r13)
/* 8008215C 0007F09C  38 80 00 00 */	li r4, 0
/* 80082160 0007F0A0  3B E3 00 00 */	addi r31, r3, 0
/* 80082164 0007F0A4  38 00 00 01 */	li r0, 1
/* 80082168 0007F0A8  90 9E 00 00 */	stw r4, 0(r30)
/* 8008216C 0007F0AC  38 7E 00 00 */	addi r3, r30, 0
/* 80082170 0007F0B0  90 1E 00 08 */	stw r0, 8(r30)
/* 80082174 0007F0B4  48 01 C8 19 */	bl __DSP_boot_task
/* 80082178 0007F0B8  7F E3 FB 78 */	mr r3, r31
/* 8008217C 0007F0BC  48 00 E0 79 */	bl OSRestoreInterrupts
lbl_80082180:
/* 80082180 0007F0C0  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 80082184 0007F0C4  BB C1 00 10 */	lmw r30, 0x10(r1)
/* 80082188 0007F0C8  38 21 00 18 */	addi r1, r1, 0x18
/* 8008218C 0007F0CC  7C 08 03 A6 */	mtlr r0
/* 80082190 0007F0D0  4E 80 00 20 */	blr 
/* 80082194 0007F0D4  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */
/* 80082198 0007F0D8  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */
/* 8008219C 0007F0DC  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */

.section .rodata, "a"  # 0x8036FFA0 - 0x803A8380
.balign 8
$$2347:
	.incbin "baserom.dol", 0x36E470, 0x1C
$$2351:
	.incbin "baserom.dol", 0x36E48C, 0x24

.section .sbss, "wa"  # 0x804097C0 - 0x8040B45C
.global __DSP_tmp_task
__DSP_tmp_task:
	.skip 0x8
