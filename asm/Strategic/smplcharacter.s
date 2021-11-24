.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global __dt__13TSMSSmplCharaFv
__dt__13TSMSSmplCharaFv:
/* 8011D5C0 0011A500  7C 08 02 A6 */	mflr r0
/* 8011D5C4 0011A504  90 01 00 04 */	stw r0, 4(r1)
/* 8011D5C8 0011A508  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 8011D5CC 0011A50C  93 E1 00 14 */	stw r31, 0x14(r1)
/* 8011D5D0 0011A510  3B E4 00 00 */	addi r31, r4, 0
/* 8011D5D4 0011A514  93 C1 00 10 */	stw r30, 0x10(r1)
/* 8011D5D8 0011A518  7C 7E 1B 79 */	or. r30, r3, r3
/* 8011D5DC 0011A51C  41 82 00 48 */	beq lbl_8011D624
/* 8011D5E0 0011A520  3C 60 80 3B */	lis r3, __vt__13TSMSSmplChara@ha
/* 8011D5E4 0011A524  38 03 53 78 */	addi r0, r3, __vt__13TSMSSmplChara@l
/* 8011D5E8 0011A528  90 1E 00 00 */	stw r0, 0(r30)
/* 8011D5EC 0011A52C  80 7E 00 0C */	lwz r3, 0xc(r30)
/* 8011D5F0 0011A530  4B EE F5 25 */	bl __dla__FPv
/* 8011D5F4 0011A534  28 1E 00 00 */	cmplwi r30, 0
/* 8011D5F8 0011A538  41 82 00 1C */	beq lbl_8011D614
/* 8011D5FC 0011A53C  3C 60 80 3B */	lis r3, __vt__Q26JDrama10TCharacter@ha
/* 8011D600 0011A540  38 03 A4 50 */	addi r0, r3, __vt__Q26JDrama10TCharacter@l
/* 8011D604 0011A544  90 1E 00 00 */	stw r0, 0(r30)
/* 8011D608 0011A548  38 7E 00 00 */	addi r3, r30, 0
/* 8011D60C 0011A54C  38 80 00 00 */	li r4, 0
/* 8011D610 0011A550  4B F2 71 ED */	bl __dt__Q26JDrama8TNameRefFv
lbl_8011D614:
/* 8011D614 0011A554  7F E0 07 35 */	extsh. r0, r31
/* 8011D618 0011A558  40 81 00 0C */	ble lbl_8011D624
/* 8011D61C 0011A55C  7F C3 F3 78 */	mr r3, r30
/* 8011D620 0011A560  4B EE F4 91 */	bl __dl__FPv
lbl_8011D624:
/* 8011D624 0011A564  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 8011D628 0011A568  7F C3 F3 78 */	mr r3, r30
/* 8011D62C 0011A56C  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 8011D630 0011A570  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 8011D634 0011A574  7C 08 03 A6 */	mtlr r0
/* 8011D638 0011A578  38 21 00 18 */	addi r1, r1, 0x18
/* 8011D63C 0011A57C  4E 80 00 20 */	blr 

.global getRes__13TSMSSmplCharaCFPCc
getRes__13TSMSSmplCharaCFPCc:
/* 8011D640 0011A580  7C 08 02 A6 */	mflr r0
/* 8011D644 0011A584  38 83 00 00 */	addi r4, r3, 0
/* 8011D648 0011A588  90 01 00 04 */	stw r0, 4(r1)
/* 8011D64C 0011A58C  4C C6 31 82 */	crclr 6
/* 8011D650 0011A590  38 A2 97 68 */	addi r5, r2, $$21540@sda21
/* 8011D654 0011A594  94 21 FE F0 */	stwu r1, -0x110(r1)
/* 8011D658 0011A598  38 61 00 10 */	addi r3, r1, 0x10
/* 8011D65C 0011A59C  80 C4 00 0C */	lwz r6, 0xc(r4)
/* 8011D660 0011A5A0  38 E3 00 00 */	addi r7, r3, 0
/* 8011D664 0011A5A4  38 80 01 00 */	li r4, 0x100
/* 8011D668 0011A5A8  4B F6 6A 5D */	bl snprintf
/* 8011D66C 0011A5AC  38 61 00 10 */	addi r3, r1, 0x10
/* 8011D670 0011A5B0  4B EE E6 4D */	bl getGlbResource__13JKRFileLoaderFPCc
/* 8011D674 0011A5B4  80 01 01 14 */	lwz r0, 0x114(r1)
/* 8011D678 0011A5B8  38 21 01 10 */	addi r1, r1, 0x110
/* 8011D67C 0011A5BC  7C 08 03 A6 */	mtlr r0
/* 8011D680 0011A5C0  4E 80 00 20 */	blr 

.global load__13TSMSSmplCharaFR20JSUMemoryInputStream
load__13TSMSSmplCharaFR20JSUMemoryInputStream:
/* 8011D684 0011A5C4  7C 08 02 A6 */	mflr r0
/* 8011D688 0011A5C8  90 01 00 04 */	stw r0, 4(r1)
/* 8011D68C 0011A5CC  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 8011D690 0011A5D0  93 E1 00 14 */	stw r31, 0x14(r1)
/* 8011D694 0011A5D4  3B E4 00 00 */	addi r31, r4, 0
/* 8011D698 0011A5D8  93 C1 00 10 */	stw r30, 0x10(r1)
/* 8011D69C 0011A5DC  3B C3 00 00 */	addi r30, r3, 0
/* 8011D6A0 0011A5E0  4B F2 71 B1 */	bl load__Q26JDrama8TNameRefFR20JSUMemoryInputStream
/* 8011D6A4 0011A5E4  80 9E 00 0C */	lwz r4, 0xc(r30)
/* 8011D6A8 0011A5E8  38 7F 00 00 */	addi r3, r31, 0
/* 8011D6AC 0011A5EC  38 A0 01 00 */	li r5, 0x100
/* 8011D6B0 0011A5F0  4B EF 10 0D */	bl readString__14JSUInputStreamFPcUs
/* 8011D6B4 0011A5F4  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 8011D6B8 0011A5F8  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 8011D6BC 0011A5FC  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 8011D6C0 0011A600  7C 08 03 A6 */	mtlr r0
/* 8011D6C4 0011A604  38 21 00 18 */	addi r1, r1, 0x18
/* 8011D6C8 0011A608  4E 80 00 20 */	blr 

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global __vt__13TSMSSmplChara
__vt__13TSMSSmplChara:
  .4byte 0
  .4byte 0
  .4byte __dt__13TSMSSmplCharaFv
  .4byte getType__Q26JDrama8TNameRefCFv
  .4byte load__13TSMSSmplCharaFR20JSUMemoryInputStream
  .4byte save__Q26JDrama8TNameRefFR21JSUMemoryOutputStream
  .4byte loadAfter__Q26JDrama8TNameRefFv
  .4byte searchF__Q26JDrama8TNameRefFUsPCc
  .4byte getRes__13TSMSSmplCharaCFPCc
  .4byte 0

.section .sdata2, "a"  # 0x8040B460 - 0x80414020
.balign 8
$$21540:
	.incbin "baserom.dol", 0x3E5468, 0x8
