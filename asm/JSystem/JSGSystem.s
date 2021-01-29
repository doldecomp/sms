.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0

.global __dt__Q26JStage7TSystemFv
__dt__Q26JStage7TSystemFv:
/* 80081B5C 0007EA9C  7C 08 02 A6 */	mflr r0
/* 80081B60 0007EAA0  90 01 00 04 */	stw r0, 4(r1)
/* 80081B64 0007EAA4  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 80081B68 0007EAA8  93 E1 00 14 */	stw r31, 0x14(r1)
/* 80081B6C 0007EAAC  3B E4 00 00 */	addi r31, r4, 0
/* 80081B70 0007EAB0  93 C1 00 10 */	stw r30, 0x10(r1)
/* 80081B74 0007EAB4  7C 7E 1B 79 */	or. r30, r3, r3
/* 80081B78 0007EAB8  41 82 00 2C */	beq lbl_80081BA4
/* 80081B7C 0007EABC  3C 60 80 3B */	lis r3, __vt__Q26JStage7TSystem@ha
/* 80081B80 0007EAC0  38 03 CE C8 */	addi r0, r3, __vt__Q26JStage7TSystem@l
/* 80081B84 0007EAC4  90 1E 00 00 */	stw r0, 0(r30)
/* 80081B88 0007EAC8  38 7E 00 00 */	addi r3, r30, 0
/* 80081B8C 0007EACC  38 80 00 00 */	li r4, 0
/* 80081B90 0007EAD0  4B FF FF 55 */	bl __dt__Q26JStage7TObjectFv
/* 80081B94 0007EAD4  7F E0 07 35 */	extsh. r0, r31
/* 80081B98 0007EAD8  40 81 00 0C */	ble lbl_80081BA4
/* 80081B9C 0007EADC  7F C3 F3 78 */	mr r3, r30
/* 80081BA0 0007EAE0  4B F8 AF 11 */	bl __dl__FPv
lbl_80081BA4:
/* 80081BA4 0007EAE4  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 80081BA8 0007EAE8  7F C3 F3 78 */	mr r3, r30
/* 80081BAC 0007EAEC  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 80081BB0 0007EAF0  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 80081BB4 0007EAF4  7C 08 03 A6 */	mtlr r0
/* 80081BB8 0007EAF8  38 21 00 18 */	addi r1, r1, 0x18
/* 80081BBC 0007EAFC  4E 80 00 20 */	blr 

.global JSGFGetType__Q26JStage7TSystemCFv
JSGFGetType__Q26JStage7TSystemCFv:
/* 80081BC0 0007EB00  38 60 00 01 */	li r3, 1
/* 80081BC4 0007EB04  4E 80 00 20 */	blr 

.global JSGFindObject__Q26JStage7TSystemCFPCcQ26JStage8TEObject
JSGFindObject__Q26JStage7TSystemCFPCcQ26JStage8TEObject:
/* 80081BC8 0007EB08  38 60 00 00 */	li r3, 0
/* 80081BCC 0007EB0C  4E 80 00 20 */	blr 

.global JSGCreateObject__Q26JStage7TSystemFPCcQ26JStage8TEObjectUl
JSGCreateObject__Q26JStage7TSystemFPCcQ26JStage8TEObjectUl:
/* 80081BD0 0007EB10  38 60 00 00 */	li r3, 0
/* 80081BD4 0007EB14  4E 80 00 20 */	blr 

.global JSGDestroyObject__Q26JStage7TSystemFPQ26JStage7TObject
JSGDestroyObject__Q26JStage7TSystemFPQ26JStage7TObject:
/* 80081BD8 0007EB18  4E 80 00 20 */	blr 

.global JSGGetSystemData__Q26JStage7TSystemFUl
JSGGetSystemData__Q26JStage7TSystemFUl:
/* 80081BDC 0007EB1C  38 60 00 00 */	li r3, 0
/* 80081BE0 0007EB20  4E 80 00 20 */	blr 

.global JSGSetSystemData__Q26JStage7TSystemFUlUl
JSGSetSystemData__Q26JStage7TSystemFUlUl:
/* 80081BE4 0007EB24  4E 80 00 20 */	blr 
/* 80081BE8 0007EB28  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */
/* 80081BEC 0007EB2C  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */
/* 80081BF0 0007EB30  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */
/* 80081BF4 0007EB34  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */
/* 80081BF8 0007EB38  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */
/* 80081BFC 0007EB3C  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */
