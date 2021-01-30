.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global atan2
atan2:
/* 80086B24 00083A64  7C 08 02 A6 */	mflr r0
/* 80086B28 00083A68  90 01 00 04 */	stw r0, 4(r1)
/* 80086B2C 00083A6C  94 21 FF F8 */	stwu r1, -8(r1)
/* 80086B30 00083A70  4B FF FA 81 */	bl __ieee754_atan2
/* 80086B34 00083A74  80 01 00 0C */	lwz r0, 0xc(r1)
/* 80086B38 00083A78  38 21 00 08 */	addi r1, r1, 8
/* 80086B3C 00083A7C  7C 08 03 A6 */	mtlr r0
/* 80086B40 00083A80  4E 80 00 20 */	blr 
