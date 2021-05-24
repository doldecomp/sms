.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global load__20TScenarioArchiveNameFR20JSUMemoryInputStream
load__20TScenarioArchiveNameFR20JSUMemoryInputStream:
/* 800FBA14 000F8954  7C 08 02 A6 */	mflr r0
/* 800FBA18 000F8958  90 01 00 04 */	stw r0, 4(r1)
/* 800FBA1C 000F895C  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 800FBA20 000F8960  93 E1 00 14 */	stw r31, 0x14(r1)
/* 800FBA24 000F8964  3B E4 00 00 */	addi r31, r4, 0
/* 800FBA28 000F8968  93 C1 00 10 */	stw r30, 0x10(r1)
/* 800FBA2C 000F896C  3B C3 00 00 */	addi r30, r3, 0
/* 800FBA30 000F8970  4B F4 8E 21 */	bl load__Q26JDrama8TNameRefFR20JSUMemoryInputStream
/* 800FBA34 000F8974  7F E3 FB 78 */	mr r3, r31
/* 800FBA38 000F8978  4B F1 2B B1 */	bl readString__14JSUInputStreamFv
/* 800FBA3C 000F897C  90 7E 00 0C */	stw r3, 0xc(r30)
/* 800FBA40 000F8980  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 800FBA44 000F8984  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 800FBA48 000F8988  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 800FBA4C 000F898C  7C 08 03 A6 */	mtlr r0
/* 800FBA50 000F8990  38 21 00 18 */	addi r1, r1, 0x18
/* 800FBA54 000F8994  4E 80 00 20 */	blr 

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global __vt__20TScenarioArchiveName
__vt__20TScenarioArchiveName:
  .4byte 0
  .4byte 0
  .4byte __dt__20TScenarioArchiveNameFv
  .4byte getType__Q26JDrama8TNameRefCFv
  .4byte load__20TScenarioArchiveNameFR20JSUMemoryInputStream
  .4byte save__Q26JDrama8TNameRefFR21JSUMemoryOutputStream
  .4byte loadAfter__Q26JDrama8TNameRefFv
  .4byte searchF__Q26JDrama8TNameRefFUsPCc
