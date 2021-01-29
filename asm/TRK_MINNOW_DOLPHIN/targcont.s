.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0

.global TRKTargetContinue
TRKTargetContinue:
/* 8008C36C 000892AC  7C 08 02 A6 */	mflr r0
/* 8008C370 000892B0  38 60 00 00 */	li r3, 0
/* 8008C374 000892B4  90 01 00 04 */	stw r0, 4(r1)
/* 8008C378 000892B8  94 21 FF F8 */	stwu r1, -8(r1)
/* 8008C37C 000892BC  4B FF F3 41 */	bl TRKTargetSetStopped
/* 8008C380 000892C0  4B FF FF 99 */	bl UnreserveEXI2Port
/* 8008C384 000892C4  4B FF ED 35 */	bl TRKSwapAndGo
/* 8008C388 000892C8  4B FF FF 61 */	bl ReserveEXI2Port
/* 8008C38C 000892CC  38 60 00 00 */	li r3, 0
/* 8008C390 000892D0  38 21 00 08 */	addi r1, r1, 8
/* 8008C394 000892D4  80 01 00 04 */	lwz r0, 4(r1)
/* 8008C398 000892D8  7C 08 03 A6 */	mtlr r0
/* 8008C39C 000892DC  4E 80 00 20 */	blr 
