.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global __dt__Q26JStage7TObjectFv
__dt__Q26JStage7TObjectFv:
/* 80081AE4 0007EA24  7C 08 02 A6 */	mflr r0
/* 80081AE8 0007EA28  90 01 00 04 */	stw r0, 4(r1)
/* 80081AEC 0007EA2C  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 80081AF0 0007EA30  93 E1 00 14 */	stw r31, 0x14(r1)
/* 80081AF4 0007EA34  7C 7F 1B 79 */	or. r31, r3, r3
/* 80081AF8 0007EA38  41 82 00 20 */	beq lbl_80081B18
/* 80081AFC 0007EA3C  3C 60 80 3B */	lis r3, __vt__Q26JStage7TObject@ha
/* 80081B00 0007EA40  38 63 CE 98 */	addi r3, r3, __vt__Q26JStage7TObject@l
/* 80081B04 0007EA44  7C 80 07 35 */	extsh. r0, r4
/* 80081B08 0007EA48  90 7F 00 00 */	stw r3, 0(r31)
/* 80081B0C 0007EA4C  40 81 00 0C */	ble lbl_80081B18
/* 80081B10 0007EA50  7F E3 FB 78 */	mr r3, r31
/* 80081B14 0007EA54  4B F8 AF 9D */	bl __dl__FPv
lbl_80081B18:
/* 80081B18 0007EA58  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 80081B1C 0007EA5C  7F E3 FB 78 */	mr r3, r31
/* 80081B20 0007EA60  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 80081B24 0007EA64  38 21 00 18 */	addi r1, r1, 0x18
/* 80081B28 0007EA68  7C 08 03 A6 */	mtlr r0
/* 80081B2C 0007EA6C  4E 80 00 20 */	blr 

.global JSGGetName__Q26JStage7TObjectCFv
JSGGetName__Q26JStage7TObjectCFv:
/* 80081B30 0007EA70  38 60 00 00 */	li r3, 0
/* 80081B34 0007EA74  4E 80 00 20 */	blr 

.global JSGGetFlag__Q26JStage7TObjectCFv
JSGGetFlag__Q26JStage7TObjectCFv:
/* 80081B38 0007EA78  38 60 00 00 */	li r3, 0
/* 80081B3C 0007EA7C  4E 80 00 20 */	blr 

.global JSGSetFlag__Q26JStage7TObjectFUl
JSGSetFlag__Q26JStage7TObjectFUl:
/* 80081B40 0007EA80  4E 80 00 20 */	blr 

.global JSGGetData__Q26JStage7TObjectCFUlPvUl
JSGGetData__Q26JStage7TObjectCFUlPvUl:
/* 80081B44 0007EA84  38 60 00 00 */	li r3, 0
/* 80081B48 0007EA88  4E 80 00 20 */	blr 

.global JSGSetData__Q26JStage7TObjectFUlPCvUl
JSGSetData__Q26JStage7TObjectFUlPCvUl:
/* 80081B4C 0007EA8C  4E 80 00 20 */	blr 

.global JSGGetParent__Q26JStage7TObjectCFPPQ26JStage7TObjectPUl
JSGGetParent__Q26JStage7TObjectCFPPQ26JStage7TObjectPUl:
/* 80081B50 0007EA90  4E 80 00 20 */	blr 

.global JSGSetParent__Q26JStage7TObjectFPQ26JStage7TObjectUl
JSGSetParent__Q26JStage7TObjectFPQ26JStage7TObjectUl:
/* 80081B54 0007EA94  4E 80 00 20 */	blr 

.global JSGSetRelation__Q26JStage7TObjectFbPQ26JStage7TObjectUl
JSGSetRelation__Q26JStage7TObjectFbPQ26JStage7TObjectUl:
/* 80081B58 0007EA98  4E 80 00 20 */	blr 

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global __vt__Q26JStage7TObject
__vt__Q26JStage7TObject:
	.incbin "baserom.dol", 0x3A9E98, 0x30
