.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global main
main:
/* 80005600 00002540  7C 08 02 A6 */	mflr r0
/* 80005604 00002544  3C 60 80 3E */	lis r3, gpApplication@ha
/* 80005608 00002548  90 01 00 04 */	stw r0, 4(r1)
/* 8000560C 0000254C  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 80005610 00002550  93 E1 00 0C */	stw r31, 0xc(r1)
/* 80005614 00002554  3B E3 60 00 */	addi r31, r3, gpApplication@l
/* 80005618 00002558  38 7F 00 00 */	addi r3, r31, 0
/* 8000561C 0000255C  48 0F 57 AD */	bl initialize__12TApplicationFv
/* 80005620 00002560  7F E3 FB 78 */	mr r3, r31
/* 80005624 00002564  48 0F 47 79 */	bl proc__12TApplicationFv
/* 80005628 00002568  7F E3 FB 78 */	mr r3, r31
/* 8000562C 0000256C  48 0F 4D A5 */	bl finalize__12TApplicationFv
/* 80005630 00002570  80 01 00 14 */	lwz r0, 0x14(r1)
/* 80005634 00002574  83 E1 00 0C */	lwz r31, 0xc(r1)
/* 80005638 00002578  38 21 00 10 */	addi r1, r1, 0x10
/* 8000563C 0000257C  7C 08 03 A6 */	mtlr r0
/* 80005640 00002580  4E 80 00 20 */	blr 

.global __sinit_main_cpp
__sinit_main_cpp:
/* 80005644 00002584  7C 08 02 A6 */	mflr r0
/* 80005648 00002588  3C 60 80 3E */	lis r3, gpApplication@ha
/* 8000564C 0000258C  90 01 00 04 */	stw r0, 4(r1)
/* 80005650 00002590  38 63 60 00 */	addi r3, r3, gpApplication@l
/* 80005654 00002594  94 21 FF F8 */	stwu r1, -8(r1)
/* 80005658 00002598  48 0F 5E B5 */	bl __ct__12TApplicationFv
/* 8000565C 0000259C  80 01 00 0C */	lwz r0, 0xc(r1)
/* 80005660 000025A0  38 21 00 08 */	addi r1, r1, 8
/* 80005664 000025A4  7C 08 03 A6 */	mtlr r0
/* 80005668 000025A8  4E 80 00 20 */	blr 

.section .ctors, "wa"  # 0x8036FBA0 - 0x8036FF80
	.incbin "baserom.dol", 0x36CBA4, 0x4

.section .bss, "wa"  # 0x803E6000 - 0x80408AC0
.global gpApplication
gpApplication:
	.skip 0x50
