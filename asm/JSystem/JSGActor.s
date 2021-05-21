.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global __dt__Q26JStage6TActorFv
__dt__Q26JStage6TActorFv:
/* 80081980 0007E8C0  7C 08 02 A6 */	mflr r0
/* 80081984 0007E8C4  90 01 00 04 */	stw r0, 4(r1)
/* 80081988 0007E8C8  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 8008198C 0007E8CC  93 E1 00 14 */	stw r31, 0x14(r1)
/* 80081990 0007E8D0  3B E4 00 00 */	addi r31, r4, 0
/* 80081994 0007E8D4  93 C1 00 10 */	stw r30, 0x10(r1)
/* 80081998 0007E8D8  7C 7E 1B 79 */	or. r30, r3, r3
/* 8008199C 0007E8DC  41 82 00 2C */	beq lbl_800819C8
/* 800819A0 0007E8E0  3C 60 80 3B */	lis r3, __vt__Q26JStage6TActor@ha
/* 800819A4 0007E8E4  38 03 CD D0 */	addi r0, r3, __vt__Q26JStage6TActor@l
/* 800819A8 0007E8E8  90 1E 00 00 */	stw r0, 0(r30)
/* 800819AC 0007E8EC  38 7E 00 00 */	addi r3, r30, 0
/* 800819B0 0007E8F0  38 80 00 00 */	li r4, 0
/* 800819B4 0007E8F4  48 00 01 31 */	bl __dt__Q26JStage7TObjectFv
/* 800819B8 0007E8F8  7F E0 07 35 */	extsh. r0, r31
/* 800819BC 0007E8FC  40 81 00 0C */	ble lbl_800819C8
/* 800819C0 0007E900  7F C3 F3 78 */	mr r3, r30
/* 800819C4 0007E904  4B F8 B0 ED */	bl __dl__FPv
lbl_800819C8:
/* 800819C8 0007E908  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 800819CC 0007E90C  7F C3 F3 78 */	mr r3, r30
/* 800819D0 0007E910  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 800819D4 0007E914  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 800819D8 0007E918  7C 08 03 A6 */	mtlr r0
/* 800819DC 0007E91C  38 21 00 18 */	addi r1, r1, 0x18
/* 800819E0 0007E920  4E 80 00 20 */	blr 

.global JSGFGetType__Q26JStage6TActorCFv
JSGFGetType__Q26JStage6TActorCFv:
/* 800819E4 0007E924  38 60 00 02 */	li r3, 2
/* 800819E8 0007E928  4E 80 00 20 */	blr 

.global JSGGetTranslation__Q26JStage6TActorCFP3Vec
JSGGetTranslation__Q26JStage6TActorCFP3Vec:
/* 800819EC 0007E92C  4E 80 00 20 */	blr 

.global JSGSetTranslation__Q26JStage6TActorFRC3Vec
JSGSetTranslation__Q26JStage6TActorFRC3Vec:
/* 800819F0 0007E930  4E 80 00 20 */	blr 

.global JSGGetScaling__Q26JStage6TActorCFP3Vec
JSGGetScaling__Q26JStage6TActorCFP3Vec:
/* 800819F4 0007E934  4E 80 00 20 */	blr 

.global JSGSetScaling__Q26JStage6TActorFRC3Vec
JSGSetScaling__Q26JStage6TActorFRC3Vec:
/* 800819F8 0007E938  4E 80 00 20 */	blr 

.global JSGGetRotation__Q26JStage6TActorCFP3Vec
JSGGetRotation__Q26JStage6TActorCFP3Vec:
/* 800819FC 0007E93C  4E 80 00 20 */	blr 

.global JSGSetRotation__Q26JStage6TActorFRC3Vec
JSGSetRotation__Q26JStage6TActorFRC3Vec:
/* 80081A00 0007E940  4E 80 00 20 */	blr 

.global JSGGetShape__Q26JStage6TActorCFv
JSGGetShape__Q26JStage6TActorCFv:
/* 80081A04 0007E944  38 60 FF FF */	li r3, -1
/* 80081A08 0007E948  4E 80 00 20 */	blr 

.global JSGSetShape__Q26JStage6TActorFUl
JSGSetShape__Q26JStage6TActorFUl:
/* 80081A0C 0007E94C  4E 80 00 20 */	blr 

.global JSGGetAnimation__Q26JStage6TActorCFv
JSGGetAnimation__Q26JStage6TActorCFv:
/* 80081A10 0007E950  38 60 FF FF */	li r3, -1
/* 80081A14 0007E954  4E 80 00 20 */	blr 

.global JSGSetAnimation__Q26JStage6TActorFUl
JSGSetAnimation__Q26JStage6TActorFUl:
/* 80081A18 0007E958  4E 80 00 20 */	blr 

.global JSGGetAnimationFrame__Q26JStage6TActorCFv
JSGGetAnimationFrame__Q26JStage6TActorCFv:
/* 80081A1C 0007E95C  C0 22 89 40 */	lfs f1, $$2126@sda21(r2)
/* 80081A20 0007E960  4E 80 00 20 */	blr 

.global JSGSetAnimationFrame__Q26JStage6TActorFf
JSGSetAnimationFrame__Q26JStage6TActorFf:
/* 80081A24 0007E964  4E 80 00 20 */	blr 

.global JSGGetAnimationFrameMax__Q26JStage6TActorCFv
JSGGetAnimationFrameMax__Q26JStage6TActorCFv:
/* 80081A28 0007E968  C0 22 89 40 */	lfs f1, $$2126@sda21(r2)
/* 80081A2C 0007E96C  4E 80 00 20 */	blr 

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global __vt__Q26JStage6TActor
__vt__Q26JStage6TActor:
	.incbin "baserom.dol", 0x3A9DD0, 0x68

.section .sdata2, "wa"  # 0x8040B460 - 0x80414020
$$2126:
	.incbin "baserom.dol", 0x3E4640, 0x8
