.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global MarErrInit__Fv
MarErrInit__Fv:
/* 800F04D0 000ED410  7C 08 02 A6 */	mflr r0
/* 800F04D4 000ED414  90 01 00 04 */	stw r0, 4(r1)
/* 800F04D8 000ED418  94 21 FF E0 */	stwu r1, -0x20(r1)
/* 800F04DC 000ED41C  93 E1 00 1C */	stw r31, 0x1c(r1)
/* 800F04E0 000ED420  4B F2 0D 19 */	bl start__14JUTDirectPrintFv
/* 800F04E4 000ED424  3B E3 00 00 */	addi r31, r3, 0
/* 800F04E8 000ED428  38 60 00 00 */	li r3, 0
/* 800F04EC 000ED42C  4B F4 F8 59 */	bl createManager__17JUTConsoleManagerFP7JKRHeap
/* 800F04F0 000ED430  7F E3 FB 78 */	mr r3, r31
/* 800F04F4 000ED434  4B F1 F1 19 */	bl create__12JUTExceptionFP14JUTDirectPrint
/* 800F04F8 000ED438  38 60 14 00 */	li r3, 0x1400
/* 800F04FC 000ED43C  4B F1 C4 B5 */	bl __nwa__FUl
/* 800F0500 000ED440  38 80 14 00 */	li r4, 0x1400
/* 800F0504 000ED444  4B F2 0A E9 */	bl createConsole__12JUTExceptionFPvUl
/* 800F0508 000ED448  3C 60 80 37 */	lis r3, "@1503"@ha
/* 800F050C 000ED44C  38 63 3D 68 */	addi r3, r3, "@1503"@l
/* 800F0510 000ED450  4B F2 06 05 */	bl appendMapFile__12JUTExceptionFPc
/* 800F0514 000ED454  3C 60 80 0F */	lis r3, MarErrException__FUsP9OSContextUlUl@ha
/* 800F0518 000ED458  38 63 05 50 */	addi r3, r3, MarErrException__FUsP9OSContextUlUl@l
/* 800F051C 000ED45C  4B F2 05 E9 */	bl setPreUserCallback__12JUTExceptionFPFUsP9OSContextUlUl_v
/* 800F0520 000ED460  80 8D 8D 90 */	lwz r4, sErrorManager__12JUTException@sda21(r13)
/* 800F0524 000ED464  38 60 FF FF */	li r3, -1
/* 800F0528 000ED468  38 00 00 08 */	li r0, 8
/* 800F052C 000ED46C  90 64 00 68 */	stw r3, 0x68(r4)
/* 800F0530 000ED470  90 64 00 6C */	stw r3, 0x6c(r4)
/* 800F0534 000ED474  80 6D 8D 90 */	lwz r3, sErrorManager__12JUTException@sda21(r13)
/* 800F0538 000ED478  90 03 00 80 */	stw r0, 0x80(r3)
/* 800F053C 000ED47C  80 01 00 24 */	lwz r0, 0x24(r1)
/* 800F0540 000ED480  83 E1 00 1C */	lwz r31, 0x1c(r1)
/* 800F0544 000ED484  38 21 00 20 */	addi r1, r1, 0x20
/* 800F0548 000ED488  7C 08 03 A6 */	mtlr r0
/* 800F054C 000ED48C  4E 80 00 20 */	blr 

.global MarErrException__FUsP9OSContextUlUl
MarErrException__FUsP9OSContextUlUl:
/* 800F0550 000ED490  7C 08 02 A6 */	mflr r0
/* 800F0554 000ED494  3C 60 80 37 */	lis r3, "cDispExceptionCommand__9@unnamed@"@ha
/* 800F0558 000ED498  90 01 00 04 */	stw r0, 4(r1)
/* 800F055C 000ED49C  94 21 FF D0 */	stwu r1, -0x30(r1)
/* 800F0560 000ED4A0  93 E1 00 2C */	stw r31, 0x2c(r1)
/* 800F0564 000ED4A4  3B E3 3D 50 */	addi r31, r3, "cDispExceptionCommand__9@unnamed@"@l
/* 800F0568 000ED4A8  93 C1 00 28 */	stw r30, 0x28(r1)
/* 800F056C 000ED4AC  93 A1 00 24 */	stw r29, 0x24(r1)
/* 800F0570 000ED4B0  3B A0 00 00 */	li r29, 0
/* 800F0574 000ED4B4  48 00 00 48 */	b lbl_800F05BC
lbl_800F0578:
/* 800F0578 000ED4B8  38 60 00 64 */	li r3, 0x64
/* 800F057C 000ED4BC  4B F2 04 0D */	bl waitTime__12JUTExceptionFl
/* 800F0580 000ED4C0  80 6D 8D 90 */	lwz r3, sErrorManager__12JUTException@sda21(r13)
/* 800F0584 000ED4C4  38 81 00 18 */	addi r4, r1, 0x18
/* 800F0588 000ED4C8  38 A0 00 00 */	li r5, 0
/* 800F058C 000ED4CC  4B F1 FD 6D */	bl readPad__12JUTExceptionFPUlPUl
/* 800F0590 000ED4D0  80 61 00 18 */	lwz r3, 0x18(r1)
/* 800F0594 000ED4D4  28 03 00 00 */	cmplwi r3, 0
/* 800F0598 000ED4D8  41 82 00 24 */	beq lbl_800F05BC
/* 800F059C 000ED4DC  80 1E 00 00 */	lwz r0, 0(r30)
/* 800F05A0 000ED4E0  54 63 04 3E */	clrlwi r3, r3, 0x10
/* 800F05A4 000ED4E4  7C 03 00 40 */	cmplw r3, r0
/* 800F05A8 000ED4E8  40 82 00 0C */	bne lbl_800F05B4
/* 800F05AC 000ED4EC  38 1D 00 01 */	addi r0, r29, 1
/* 800F05B0 000ED4F0  48 00 00 08 */	b lbl_800F05B8
lbl_800F05B4:
/* 800F05B4 000ED4F4  38 00 00 00 */	li r0, 0
lbl_800F05B8:
/* 800F05B8 000ED4F8  7C 1D 03 78 */	mr r29, r0
lbl_800F05BC:
/* 800F05BC 000ED4FC  57 A0 10 3A */	slwi r0, r29, 2
/* 800F05C0 000ED500  7F DF 02 14 */	add r30, r31, r0
/* 800F05C4 000ED504  80 1E 00 00 */	lwz r0, 0(r30)
/* 800F05C8 000ED508  28 00 00 00 */	cmplwi r0, 0
/* 800F05CC 000ED50C  40 82 FF AC */	bne lbl_800F0578
/* 800F05D0 000ED510  80 01 00 34 */	lwz r0, 0x34(r1)
/* 800F05D4 000ED514  83 E1 00 2C */	lwz r31, 0x2c(r1)
/* 800F05D8 000ED518  83 C1 00 28 */	lwz r30, 0x28(r1)
/* 800F05DC 000ED51C  7C 08 03 A6 */	mtlr r0
/* 800F05E0 000ED520  83 A1 00 24 */	lwz r29, 0x24(r1)
/* 800F05E4 000ED524  38 21 00 30 */	addi r1, r1, 0x30
/* 800F05E8 000ED528  4E 80 00 20 */	blr 

.section .rodata, "a"  # 0x8036FFA0 - 0x803A8380
.balign 8
"cDispExceptionCommand__9@unnamed@":
	.incbin "baserom.dol", 0x370D50, 0x18
"@1503":
	.incbin "baserom.dol", 0x370D68, 0x10
