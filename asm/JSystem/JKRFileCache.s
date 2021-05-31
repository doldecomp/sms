.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global __dt__13JKRFileFinderFv
__dt__13JKRFileFinderFv:
/* 8000B8D4 00008814  7C 08 02 A6 */	mflr r0
/* 8000B8D8 00008818  90 01 00 04 */	stw r0, 4(r1)
/* 8000B8DC 0000881C  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 8000B8E0 00008820  93 E1 00 14 */	stw r31, 0x14(r1)
/* 8000B8E4 00008824  7C 7F 1B 79 */	or. r31, r3, r3
/* 8000B8E8 00008828  41 82 00 20 */	beq lbl_8000B908
/* 8000B8EC 0000882C  3C 60 80 3B */	lis r3, __vt__13JKRFileFinder@ha
/* 8000B8F0 00008830  38 63 85 98 */	addi r3, r3, __vt__13JKRFileFinder@l
/* 8000B8F4 00008834  7C 80 07 35 */	extsh. r0, r4
/* 8000B8F8 00008838  90 7F 00 0C */	stw r3, 0xc(r31)
/* 8000B8FC 0000883C  40 81 00 0C */	ble lbl_8000B908
/* 8000B900 00008840  7F E3 FB 78 */	mr r3, r31
/* 8000B904 00008844  48 00 11 AD */	bl __dl__FPv
lbl_8000B908:
/* 8000B908 00008848  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 8000B90C 0000884C  7F E3 FB 78 */	mr r3, r31
/* 8000B910 00008850  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 8000B914 00008854  38 21 00 18 */	addi r1, r1, 0x18
/* 8000B918 00008858  7C 08 03 A6 */	mtlr r0
/* 8000B91C 0000885C  4E 80 00 20 */	blr 

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global __vt__13JKRFileFinder
__vt__13JKRFileFinder:
  .4byte 0
  .4byte 0
  .4byte __dt__13JKRFileFinderFv
  .4byte 0
