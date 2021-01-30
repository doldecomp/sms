.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global TRK_main
TRK_main:
/* 8008BFCC 00088F0C  7C 08 02 A6 */	mflr r0
/* 8008BFD0 00088F10  90 01 00 04 */	stw r0, 4(r1)
/* 8008BFD4 00088F14  94 21 FF F8 */	stwu r1, -8(r1)
/* 8008BFD8 00088F18  4B FF BE ED */	bl TRKInitializeNub
/* 8008BFDC 00088F1C  3C 80 80 3F */	lis r4, TRK_mainError@ha
/* 8008BFE0 00088F20  94 64 B0 68 */	stwu r3, TRK_mainError@l(r4)
/* 8008BFE4 00088F24  80 04 00 00 */	lwz r0, 0(r4)
/* 8008BFE8 00088F28  2C 00 00 00 */	cmpwi r0, 0
/* 8008BFEC 00088F2C  40 82 00 0C */	bne lbl_8008BFF8
/* 8008BFF0 00088F30  4B FF BF CD */	bl TRKNubWelcome
/* 8008BFF4 00088F34  4B FF BB 81 */	bl TRKNubMainLoop
lbl_8008BFF8:
/* 8008BFF8 00088F38  4B FF BF A1 */	bl TRKTerminateNub
/* 8008BFFC 00088F3C  3C 80 80 3F */	lis r4, TRK_mainError@ha
/* 8008C000 00088F40  90 64 B0 68 */	stw r3, TRK_mainError@l(r4)
/* 8008C004 00088F44  38 21 00 08 */	addi r1, r1, 8
/* 8008C008 00088F48  80 01 00 04 */	lwz r0, 4(r1)
/* 8008C00C 00088F4C  7C 08 03 A6 */	mtlr r0
/* 8008C010 00088F50  4E 80 00 20 */	blr 

.section .bss, "wa"  # 0x803E6000 - 0x80408AC0
.global TRK_mainError
TRK_mainError:
	.skip 0x18
