.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0

.global __dt__7TBinderFv
__dt__7TBinderFv:
/* 8011D968 0011A8A8  7C 08 02 A6 */	mflr r0
/* 8011D96C 0011A8AC  90 01 00 04 */	stw r0, 4(r1)
/* 8011D970 0011A8B0  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 8011D974 0011A8B4  93 E1 00 14 */	stw r31, 0x14(r1)
/* 8011D978 0011A8B8  7C 7F 1B 79 */	or. r31, r3, r3
/* 8011D97C 0011A8BC  41 82 00 20 */	beq lbl_8011D99C
/* 8011D980 0011A8C0  3C 60 80 3B */	lis r3, __vt__7TBinder@ha
/* 8011D984 0011A8C4  38 63 54 48 */	addi r3, r3, __vt__7TBinder@l
/* 8011D988 0011A8C8  7C 80 07 35 */	extsh. r0, r4
/* 8011D98C 0011A8CC  90 7F 00 00 */	stw r3, 0(r31)
/* 8011D990 0011A8D0  40 81 00 0C */	ble lbl_8011D99C
/* 8011D994 0011A8D4  7F E3 FB 78 */	mr r3, r31
/* 8011D998 0011A8D8  4B EE F1 19 */	bl __dl__FPv
lbl_8011D99C:
/* 8011D99C 0011A8DC  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 8011D9A0 0011A8E0  7F E3 FB 78 */	mr r3, r31
/* 8011D9A4 0011A8E4  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 8011D9A8 0011A8E8  38 21 00 18 */	addi r1, r1, 0x18
/* 8011D9AC 0011A8EC  7C 08 03 A6 */	mtlr r0
/* 8011D9B0 0011A8F0  4E 80 00 20 */	blr 

.global __ct__7TBinderFv
__ct__7TBinderFv:
/* 8011D9B4 0011A8F4  3C 80 80 3B */	lis r4, __vt__7TBinder@ha
/* 8011D9B8 0011A8F8  38 04 54 48 */	addi r0, r4, __vt__7TBinder@l
/* 8011D9BC 0011A8FC  90 03 00 00 */	stw r0, 0(r3)
/* 8011D9C0 0011A900  4E 80 00 20 */	blr 
