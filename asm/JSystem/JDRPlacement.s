.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global load__Q26JDrama10TPlacementFR20JSUMemoryInputStream
load__Q26JDrama10TPlacementFR20JSUMemoryInputStream:
/* 80045A2C 0004296C  7C 08 02 A6 */	mflr r0
/* 80045A30 00042970  90 01 00 04 */	stw r0, 4(r1)
/* 80045A34 00042974  94 21 FF D8 */	stwu r1, -0x28(r1)
/* 80045A38 00042978  93 E1 00 24 */	stw r31, 0x24(r1)
/* 80045A3C 0004297C  3B E4 00 00 */	addi r31, r4, 0
/* 80045A40 00042980  93 C1 00 20 */	stw r30, 0x20(r1)
/* 80045A44 00042984  3B C3 00 00 */	addi r30, r3, 0
/* 80045A48 00042988  4B FF EE 09 */	bl load__Q26JDrama8TNameRefFR20JSUMemoryInputStream
/* 80045A4C 0004298C  38 7F 00 00 */	addi r3, r31, 0
/* 80045A50 00042990  38 9E 00 10 */	addi r4, r30, 0x10
/* 80045A54 00042994  38 A0 00 04 */	li r5, 4
/* 80045A58 00042998  4B FC 8B 39 */	bl read__14JSUInputStreamFPvl
/* 80045A5C 0004299C  38 7F 00 00 */	addi r3, r31, 0
/* 80045A60 000429A0  38 9E 00 14 */	addi r4, r30, 0x14
/* 80045A64 000429A4  38 A0 00 04 */	li r5, 4
/* 80045A68 000429A8  4B FC 8B 29 */	bl read__14JSUInputStreamFPvl
/* 80045A6C 000429AC  38 7F 00 00 */	addi r3, r31, 0
/* 80045A70 000429B0  38 9E 00 18 */	addi r4, r30, 0x18
/* 80045A74 000429B4  38 A0 00 04 */	li r5, 4
/* 80045A78 000429B8  4B FC 8B 19 */	bl read__14JSUInputStreamFPvl
/* 80045A7C 000429BC  80 01 00 2C */	lwz r0, 0x2c(r1)
/* 80045A80 000429C0  83 E1 00 24 */	lwz r31, 0x24(r1)
/* 80045A84 000429C4  83 C1 00 20 */	lwz r30, 0x20(r1)
/* 80045A88 000429C8  7C 08 03 A6 */	mtlr r0
/* 80045A8C 000429CC  38 21 00 28 */	addi r1, r1, 0x28
/* 80045A90 000429D0  4E 80 00 20 */	blr 

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global __vt__Q26JDrama10TPlacement
__vt__Q26JDrama10TPlacement:
	.incbin "baserom.dol", 0x3A78B0, 0x28
