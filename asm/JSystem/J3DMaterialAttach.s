.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global clear__16J3DMaterialTableFv
clear__16J3DMaterialTableFv:
/* 8006C4E0 00069420  38 00 00 00 */	li r0, 0
/* 8006C4E4 00069424  B0 03 00 04 */	sth r0, 4(r3)
/* 8006C4E8 00069428  90 03 00 08 */	stw r0, 8(r3)
/* 8006C4EC 0006942C  90 03 00 0C */	stw r0, 0xc(r3)
/* 8006C4F0 00069430  90 03 00 10 */	stw r0, 0x10(r3)
/* 8006C4F4 00069434  90 03 00 14 */	stw r0, 0x14(r3)
/* 8006C4F8 00069438  4E 80 00 20 */	blr 

.global __ct__16J3DMaterialTableFv
__ct__16J3DMaterialTableFv:
/* 8006C4FC 0006943C  3C 80 80 3B */	lis r4, __vt__16J3DMaterialTable@ha
/* 8006C500 00069440  38 04 C4 20 */	addi r0, r4, __vt__16J3DMaterialTable@l
/* 8006C504 00069444  90 03 00 00 */	stw r0, 0(r3)
/* 8006C508 00069448  38 00 00 00 */	li r0, 0
/* 8006C50C 0006944C  B0 03 00 04 */	sth r0, 4(r3)
/* 8006C510 00069450  90 03 00 08 */	stw r0, 8(r3)
/* 8006C514 00069454  90 03 00 0C */	stw r0, 0xc(r3)
/* 8006C518 00069458  90 03 00 10 */	stw r0, 0x10(r3)
/* 8006C51C 0006945C  90 03 00 14 */	stw r0, 0x14(r3)
/* 8006C520 00069460  4E 80 00 20 */	blr 

.global __dt__16J3DMaterialTableFv
__dt__16J3DMaterialTableFv:
/* 8006C524 00069464  7C 08 02 A6 */	mflr r0
/* 8006C528 00069468  90 01 00 04 */	stw r0, 4(r1)
/* 8006C52C 0006946C  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 8006C530 00069470  93 E1 00 14 */	stw r31, 0x14(r1)
/* 8006C534 00069474  7C 7F 1B 79 */	or. r31, r3, r3
/* 8006C538 00069478  41 82 00 20 */	beq lbl_8006C558
/* 8006C53C 0006947C  3C 60 80 3B */	lis r3, __vt__16J3DMaterialTable@ha
/* 8006C540 00069480  38 63 C4 20 */	addi r3, r3, __vt__16J3DMaterialTable@l
/* 8006C544 00069484  7C 80 07 35 */	extsh. r0, r4
/* 8006C548 00069488  90 7F 00 00 */	stw r3, 0(r31)
/* 8006C54C 0006948C  40 81 00 0C */	ble lbl_8006C558
/* 8006C550 00069490  7F E3 FB 78 */	mr r3, r31
/* 8006C554 00069494  4B FA 05 5D */	bl __dl__FPv
lbl_8006C558:
/* 8006C558 00069498  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 8006C55C 0006949C  7F E3 FB 78 */	mr r3, r31
/* 8006C560 000694A0  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 8006C564 000694A4  38 21 00 18 */	addi r1, r1, 0x18
/* 8006C568 000694A8  7C 08 03 A6 */	mtlr r0
/* 8006C56C 000694AC  4E 80 00 20 */	blr 

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global __vt__16J3DMaterialTable
__vt__16J3DMaterialTable:
	.incbin "baserom.dol", 0x3A9420, 0x10
