.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global sysDramSetup__Q28JASystem6KernelFP12JKRSolidHeap
sysDramSetup__Q28JASystem6KernelFP12JKRSolidHeap:
/* 80063DD4 00060D14  7C 08 02 A6 */	mflr r0
/* 80063DD8 00060D18  28 03 00 00 */	cmplwi r3, 0
/* 80063DDC 00060D1C  90 01 00 04 */	stw r0, 4(r1)
/* 80063DE0 00060D20  94 21 FF F8 */	stwu r1, -8(r1)
/* 80063DE4 00060D24  41 82 00 0C */	beq lbl_80063DF0
/* 80063DE8 00060D28  90 6D 92 00 */	stw r3, JASDram@sda21(r13)
/* 80063DEC 00060D2C  48 00 00 30 */	b lbl_80063E1C
lbl_80063DF0:
/* 80063DF0 00060D30  80 6D 82 00 */	lwz r3, audioDramSize__Q28JASystem6Kernel@sda21(r13)
/* 80063DF4 00060D34  38 80 00 00 */	li r4, 0
/* 80063DF8 00060D38  38 A0 00 00 */	li r5, 0
/* 80063DFC 00060D3C  38 63 01 00 */	addi r3, r3, 0x100
/* 80063E00 00060D40  4B FA 9D 89 */	bl create__10JKRStdHeapFUlP7JKRHeapb
/* 80063E04 00060D44  80 0D 82 00 */	lwz r0, audioDramSize__Q28JASystem6Kernel@sda21(r13)
/* 80063E08 00060D48  38 83 00 00 */	addi r4, r3, 0
/* 80063E0C 00060D4C  38 A0 00 00 */	li r5, 0
/* 80063E10 00060D50  7C 03 03 78 */	mr r3, r0
/* 80063E14 00060D54  4B FA 95 C5 */	bl create__12JKRSolidHeapFUlP7JKRHeapb
/* 80063E18 00060D58  90 6D 92 00 */	stw r3, JASDram@sda21(r13)
lbl_80063E1C:
/* 80063E1C 00060D5C  80 01 00 0C */	lwz r0, 0xc(r1)
/* 80063E20 00060D60  38 21 00 08 */	addi r1, r1, 8
/* 80063E24 00060D64  7C 08 03 A6 */	mtlr r0
/* 80063E28 00060D68  4E 80 00 20 */	blr 

.global allocFromSysDram__Q28JASystem6KernelFUl
allocFromSysDram__Q28JASystem6KernelFUl:
/* 80063E2C 00060D6C  7C 08 02 A6 */	mflr r0
/* 80063E30 00060D70  90 01 00 04 */	stw r0, 4(r1)
/* 80063E34 00060D74  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 80063E38 00060D78  93 E1 00 14 */	stw r31, 0x14(r1)
/* 80063E3C 00060D7C  7C 7F 1B 78 */	mr r31, r3
/* 80063E40 00060D80  93 C1 00 10 */	stw r30, 0x10(r1)
/* 80063E44 00060D84  48 02 C3 89 */	bl OSDisableInterrupts
/* 80063E48 00060D88  80 8D 92 00 */	lwz r4, JASDram@sda21(r13)
/* 80063E4C 00060D8C  3B C3 00 00 */	addi r30, r3, 0
/* 80063E50 00060D90  38 7F 00 00 */	addi r3, r31, 0
/* 80063E54 00060D94  38 A0 00 20 */	li r5, 0x20
/* 80063E58 00060D98  4B FA 8B ED */	bl __nwa__FUlP7JKRHeapi
/* 80063E5C 00060D9C  3B E3 00 00 */	addi r31, r3, 0
/* 80063E60 00060DA0  38 7E 00 00 */	addi r3, r30, 0
/* 80063E64 00060DA4  48 02 C3 91 */	bl OSRestoreInterrupts
/* 80063E68 00060DA8  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 80063E6C 00060DAC  7F E3 FB 78 */	mr r3, r31
/* 80063E70 00060DB0  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 80063E74 00060DB4  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 80063E78 00060DB8  7C 08 03 A6 */	mtlr r0
/* 80063E7C 00060DBC  38 21 00 18 */	addi r1, r1, 0x18
/* 80063E80 00060DC0  4E 80 00 20 */	blr 

.global sysAramSetup__Q28JASystem6KernelFUl
sysAramSetup__Q28JASystem6KernelFUl:
/* 80063E84 00060DC4  7C 08 02 A6 */	mflr r0
/* 80063E88 00060DC8  90 01 00 04 */	stw r0, 4(r1)
/* 80063E8C 00060DCC  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 80063E90 00060DD0  93 E1 00 14 */	stw r31, 0x14(r1)
/* 80063E94 00060DD4  7C 7F 1B 79 */	or. r31, r3, r3
/* 80063E98 00060DD8  40 82 00 08 */	bne lbl_80063EA0
/* 80063E9C 00060DDC  83 ED 82 04 */	lwz r31, audioAramSize__Q28JASystem6Kernel@sda21(r13)
lbl_80063EA0:
/* 80063EA0 00060DE0  48 03 9A 89 */	bl ARGetBaseAddress
/* 80063EA4 00060DE4  90 6D 92 08 */	stw r3, audioAramTop__Q28JASystem6Kernel@sda21(r13)
/* 80063EA8 00060DE8  38 00 00 40 */	li r0, 0x40
/* 80063EAC 00060DEC  3C 60 80 3E */	lis r3, audioAramHeap__Q28JASystem6Kernel@ha
/* 80063EB0 00060DF0  80 CD 92 08 */	lwz r6, audioAramTop__Q28JASystem6Kernel@sda21(r13)
/* 80063EB4 00060DF4  38 63 6A A4 */	addi r3, r3, audioAramHeap__Q28JASystem6Kernel@l
/* 80063EB8 00060DF8  90 0D 92 0C */	stw r0, CARD_SECURITY_BUFFER__Q28JASystem6Kernel@sda21(r13)
/* 80063EBC 00060DFC  3C A6 00 01 */	addis r5, r6, 1
/* 80063EC0 00060E00  3C 86 00 01 */	addis r4, r6, 1
/* 80063EC4 00060E04  90 CD 92 04 */	stw r6, JASAramDmaBufferTop__Q28JASystem6Kernel@sda21(r13)
/* 80063EC8 00060E08  38 05 C0 00 */	addi r0, r5, -16384
/* 80063ECC 00060E0C  7C A0 F8 50 */	subf r5, r0, r31
/* 80063ED0 00060E10  38 84 C0 00 */	addi r4, r4, -16384
/* 80063ED4 00060E14  4B FF FD C9 */	bl init__Q38JASystem6Kernel10TSolidHeapFPUcl
/* 80063ED8 00060E18  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 80063EDC 00060E1C  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 80063EE0 00060E20  38 21 00 18 */	addi r1, r1, 0x18
/* 80063EE4 00060E24  7C 08 03 A6 */	mtlr r0
/* 80063EE8 00060E28  4E 80 00 20 */	blr 

.global allocFromSysAramFull__Q28JASystem6KernelFPUl
allocFromSysAramFull__Q28JASystem6KernelFPUl:
/* 80063EEC 00060E2C  7C 08 02 A6 */	mflr r0
/* 80063EF0 00060E30  3C 80 80 3E */	lis r4, audioAramHeap__Q28JASystem6Kernel@ha
/* 80063EF4 00060E34  90 01 00 04 */	stw r0, 4(r1)
/* 80063EF8 00060E38  94 21 FF E0 */	stwu r1, -0x20(r1)
/* 80063EFC 00060E3C  93 E1 00 1C */	stw r31, 0x1c(r1)
/* 80063F00 00060E40  93 C1 00 18 */	stw r30, 0x18(r1)
/* 80063F04 00060E44  3B C4 6A A4 */	addi r30, r4, audioAramHeap__Q28JASystem6Kernel@l
/* 80063F08 00060E48  93 A1 00 14 */	stw r29, 0x14(r1)
/* 80063F0C 00060E4C  3B A3 00 00 */	addi r29, r3, 0
/* 80063F10 00060E50  38 7E 00 00 */	addi r3, r30, 0
/* 80063F14 00060E54  4B FF FD D1 */	bl getRemain__Q38JASystem6Kernel10TSolidHeapFv
/* 80063F18 00060E58  3B E3 00 00 */	addi r31, r3, 0
/* 80063F1C 00060E5C  38 7E 00 00 */	addi r3, r30, 0
/* 80063F20 00060E60  38 9F FF E0 */	addi r4, r31, -32
/* 80063F24 00060E64  4B FF FD 1D */	bl alloc__Q38JASystem6Kernel10TSolidHeapFl
/* 80063F28 00060E68  38 1F FF E0 */	addi r0, r31, -32
/* 80063F2C 00060E6C  90 1D 00 00 */	stw r0, 0(r29)
/* 80063F30 00060E70  80 01 00 24 */	lwz r0, 0x24(r1)
/* 80063F34 00060E74  83 E1 00 1C */	lwz r31, 0x1c(r1)
/* 80063F38 00060E78  83 C1 00 18 */	lwz r30, 0x18(r1)
/* 80063F3C 00060E7C  7C 08 03 A6 */	mtlr r0
/* 80063F40 00060E80  83 A1 00 14 */	lwz r29, 0x14(r1)
/* 80063F44 00060E84  38 21 00 20 */	addi r1, r1, 0x20
/* 80063F48 00060E88  4E 80 00 20 */	blr 

.global __sinit_JASSystemHeap_cpp
__sinit_JASSystemHeap_cpp:
/* 80063F4C 00060E8C  7C 08 02 A6 */	mflr r0
/* 80063F50 00060E90  3C 60 80 3E */	lis r3, audioAramHeap__Q28JASystem6Kernel@ha
/* 80063F54 00060E94  90 01 00 04 */	stw r0, 4(r1)
/* 80063F58 00060E98  38 63 6A A4 */	addi r3, r3, audioAramHeap__Q28JASystem6Kernel@l
/* 80063F5C 00060E9C  94 21 FF F8 */	stwu r1, -8(r1)
/* 80063F60 00060EA0  4B FF FC C5 */	bl __ct__Q38JASystem6Kernel10TSolidHeapFv
/* 80063F64 00060EA4  3C 80 80 05 */	lis r4, __dt__Q38JASystem6Kernel10TSolidHeapFv@ha
/* 80063F68 00060EA8  3C A0 80 3E */	lis r5, $$2241@ha
/* 80063F6C 00060EAC  38 84 4A A0 */	addi r4, r4, __dt__Q38JASystem6Kernel10TSolidHeapFv@l
/* 80063F70 00060EB0  38 A5 6A 98 */	addi r5, r5, $$2241@l
/* 80063F74 00060EB4  48 01 E7 B5 */	bl __register_global_object
/* 80063F78 00060EB8  80 01 00 0C */	lwz r0, 0xc(r1)
/* 80063F7C 00060EBC  38 21 00 08 */	addi r1, r1, 8
/* 80063F80 00060EC0  7C 08 03 A6 */	mtlr r0
/* 80063F84 00060EC4  4E 80 00 20 */	blr 

.section .ctors, "wa"  # 0x8036FBA0 - 0x8036FF80
	.incbin "baserom.dol", 0x36CBF8, 0x4

.section .sdata, "wa"  # 0x80408AC0 - 0x804097C0
audioDramSize__Q28JASystem6Kernel:
	.incbin "baserom.dol", 0x3E3200, 0x4
audioAramSize__Q28JASystem6Kernel:
	.incbin "baserom.dol", 0x3E3204, 0x4

.section .bss, "wa"  # 0x803E6000 - 0x80408AC0
$$2241:
	.skip 0xC
audioAramHeap__Q28JASystem6Kernel:
	.skip 0x14

.section .sbss, "wa"  # 0x804097C0 - 0x8040B45C
.global JASDram
JASDram:
	.skip 0x4
JASAramDmaBufferTop__Q28JASystem6Kernel:
	.skip 0x4
audioAramTop__Q28JASystem6Kernel:
	.skip 0x4
CARD_SECURITY_BUFFER__Q28JASystem6Kernel:
	.skip 0x4
