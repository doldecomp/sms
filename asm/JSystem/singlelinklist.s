.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0

.global __dt__Q27JGadget19TSingleNodeLinkListFv
__dt__Q27JGadget19TSingleNodeLinkListFv:
/* 800408D4 0003D814  7C 08 02 A6 */	mflr r0
/* 800408D8 0003D818  90 01 00 04 */	stw r0, 4(r1)
/* 800408DC 0003D81C  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 800408E0 0003D820  93 E1 00 14 */	stw r31, 0x14(r1)
/* 800408E4 0003D824  7C 7F 1B 79 */	or. r31, r3, r3
/* 800408E8 0003D828  41 82 00 14 */	beq lbl_800408FC
/* 800408EC 0003D82C  7C 80 07 35 */	extsh. r0, r4
/* 800408F0 0003D830  40 81 00 0C */	ble lbl_800408FC
/* 800408F4 0003D834  7F E3 FB 78 */	mr r3, r31
/* 800408F8 0003D838  4B FC C1 B9 */	bl __dl__FPv
lbl_800408FC:
/* 800408FC 0003D83C  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 80040900 0003D840  7F E3 FB 78 */	mr r3, r31
/* 80040904 0003D844  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 80040908 0003D848  38 21 00 18 */	addi r1, r1, 0x18
/* 8004090C 0003D84C  7C 08 03 A6 */	mtlr r0
/* 80040910 0003D850  4E 80 00 20 */	blr 

.global Insert__Q27JGadget19TSingleNodeLinkListFQ37JGadget19TSingleNodeLinkList8iteratorPQ27JGadget19TSingleLinkListNode
Insert__Q27JGadget19TSingleNodeLinkListFQ37JGadget19TSingleNodeLinkList8iteratorPQ27JGadget19TSingleLinkListNode:
/* 80040914 0003D854  80 E5 00 00 */	lwz r7, 0(r5)
/* 80040918 0003D858  80 07 00 00 */	lwz r0, 0(r7)
/* 8004091C 0003D85C  90 06 00 00 */	stw r0, 0(r6)
/* 80040920 0003D860  28 00 00 00 */	cmplwi r0, 0
/* 80040924 0003D864  90 C7 00 00 */	stw r6, 0(r7)
/* 80040928 0003D868  40 82 00 08 */	bne lbl_80040930
/* 8004092C 0003D86C  90 C4 00 08 */	stw r6, 8(r4)
lbl_80040930:
/* 80040930 0003D870  80 C4 00 00 */	lwz r6, 0(r4)
/* 80040934 0003D874  38 06 00 01 */	addi r0, r6, 1
/* 80040938 0003D878  90 04 00 00 */	stw r0, 0(r4)
/* 8004093C 0003D87C  80 05 00 00 */	lwz r0, 0(r5)
/* 80040940 0003D880  90 03 00 00 */	stw r0, 0(r3)
/* 80040944 0003D884  4E 80 00 20 */	blr 
