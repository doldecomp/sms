.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global __dt__19TStickyStainManagerFv
__dt__19TStickyStainManagerFv:
/* 8020060C 001FD54C  7C 08 02 A6 */	mflr r0
/* 80200610 001FD550  90 01 00 04 */	stw r0, 4(r1)
/* 80200614 001FD554  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 80200618 001FD558  93 E1 00 14 */	stw r31, 0x14(r1)
/* 8020061C 001FD55C  3B E4 00 00 */	addi r31, r4, 0
/* 80200620 001FD560  93 C1 00 10 */	stw r30, 0x10(r1)
/* 80200624 001FD564  7C 7E 1B 79 */	or. r30, r3, r3
/* 80200628 001FD568  41 82 00 3C */	beq lbl_80200664
/* 8020062C 001FD56C  3C 60 80 3D */	lis r3, __vt__19TStickyStainManager@ha
/* 80200630 001FD570  38 03 0E 50 */	addi r0, r3, __vt__19TStickyStainManager@l
/* 80200634 001FD574  90 1E 00 00 */	stw r0, 0(r30)
/* 80200638 001FD578  41 82 00 1C */	beq lbl_80200654
/* 8020063C 001FD57C  3C 60 80 3B */	lis r3, __vt__Q26JDrama8TViewObj@ha
/* 80200640 001FD580  38 03 A0 C0 */	addi r0, r3, __vt__Q26JDrama8TViewObj@l
/* 80200644 001FD584  90 1E 00 00 */	stw r0, 0(r30)
/* 80200648 001FD588  38 7E 00 00 */	addi r3, r30, 0
/* 8020064C 001FD58C  38 80 00 00 */	li r4, 0
/* 80200650 001FD590  4B E4 41 AD */	bl __dt__Q26JDrama8TNameRefFv
lbl_80200654:
/* 80200654 001FD594  7F E0 07 35 */	extsh. r0, r31
/* 80200658 001FD598  40 81 00 0C */	ble lbl_80200664
/* 8020065C 001FD59C  7F C3 F3 78 */	mr r3, r30
/* 80200660 001FD5A0  4B E0 C4 51 */	bl __dl__FPv
lbl_80200664:
/* 80200664 001FD5A4  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 80200668 001FD5A8  7F C3 F3 78 */	mr r3, r30
/* 8020066C 001FD5AC  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 80200670 001FD5B0  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 80200674 001FD5B4  7C 08 03 A6 */	mtlr r0
/* 80200678 001FD5B8  38 21 00 18 */	addi r1, r1, 0x18
/* 8020067C 001FD5BC  4E 80 00 20 */	blr 

.global perform__19TStickyStainManagerFUlPQ26JDrama9TGraphics
perform__19TStickyStainManagerFUlPQ26JDrama9TGraphics:
/* 80200680 001FD5C0  4E 80 00 20 */	blr 

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global __vt__19TStickyStainManager
__vt__19TStickyStainManager:
  .4byte 0
  .4byte 0
  .4byte __dt__19TStickyStainManagerFv
  .4byte getType__Q26JDrama8TNameRefCFv
  .4byte load__Q26JDrama8TNameRefFR20JSUMemoryInputStream
  .4byte save__Q26JDrama8TNameRefFR21JSUMemoryOutputStream
  .4byte loadAfter__Q26JDrama8TNameRefFv
  .4byte searchF__Q26JDrama8TNameRefFUsPCc
  .4byte perform__19TStickyStainManagerFUlPQ26JDrama9TGraphics
  .4byte 0
