.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global load__18TStagePositionInfoFR20JSUMemoryInputStream
load__18TStagePositionInfoFR20JSUMemoryInputStream:
/* 800FC71C 000F965C  7C 08 02 A6 */	mflr r0
/* 800FC720 000F9660  90 01 00 04 */	stw r0, 4(r1)
/* 800FC724 000F9664  94 21 FF C0 */	stwu r1, -0x40(r1)
/* 800FC728 000F9668  93 E1 00 3C */	stw r31, 0x3c(r1)
/* 800FC72C 000F966C  3B E4 00 00 */	addi r31, r4, 0
/* 800FC730 000F9670  93 C1 00 38 */	stw r30, 0x38(r1)
/* 800FC734 000F9674  3B C3 00 00 */	addi r30, r3, 0
/* 800FC738 000F9678  4B F4 81 19 */	bl load__Q26JDrama8TNameRefFR20JSUMemoryInputStream
/* 800FC73C 000F967C  38 7F 00 00 */	addi r3, r31, 0
/* 800FC740 000F9680  38 9E 00 0C */	addi r4, r30, 0xc
/* 800FC744 000F9684  38 A0 00 04 */	li r5, 4
/* 800FC748 000F9688  4B F1 1E 49 */	bl read__14JSUInputStreamFPvl
/* 800FC74C 000F968C  38 7F 00 00 */	addi r3, r31, 0
/* 800FC750 000F9690  38 9E 00 10 */	addi r4, r30, 0x10
/* 800FC754 000F9694  38 A0 00 04 */	li r5, 4
/* 800FC758 000F9698  4B F1 1E 39 */	bl read__14JSUInputStreamFPvl
/* 800FC75C 000F969C  38 7F 00 00 */	addi r3, r31, 0
/* 800FC760 000F96A0  38 9E 00 14 */	addi r4, r30, 0x14
/* 800FC764 000F96A4  38 A0 00 04 */	li r5, 4
/* 800FC768 000F96A8  4B F1 1E 29 */	bl read__14JSUInputStreamFPvl
/* 800FC76C 000F96AC  38 7F 00 00 */	addi r3, r31, 0
/* 800FC770 000F96B0  38 81 00 34 */	addi r4, r1, 0x34
/* 800FC774 000F96B4  38 A0 00 04 */	li r5, 4
/* 800FC778 000F96B8  4B F1 1E 19 */	bl read__14JSUInputStreamFPvl
/* 800FC77C 000F96BC  38 7F 00 00 */	addi r3, r31, 0
/* 800FC780 000F96C0  38 81 00 30 */	addi r4, r1, 0x30
/* 800FC784 000F96C4  38 A0 00 04 */	li r5, 4
/* 800FC788 000F96C8  4B F1 1E 09 */	bl read__14JSUInputStreamFPvl
/* 800FC78C 000F96CC  38 7F 00 00 */	addi r3, r31, 0
/* 800FC790 000F96D0  38 81 00 2C */	addi r4, r1, 0x2c
/* 800FC794 000F96D4  38 A0 00 04 */	li r5, 4
/* 800FC798 000F96D8  4B F1 1D F9 */	bl read__14JSUInputStreamFPvl
/* 800FC79C 000F96DC  38 7F 00 00 */	addi r3, r31, 0
/* 800FC7A0 000F96E0  38 81 00 28 */	addi r4, r1, 0x28
/* 800FC7A4 000F96E4  38 A0 00 04 */	li r5, 4
/* 800FC7A8 000F96E8  4B F1 1D E9 */	bl read__14JSUInputStreamFPvl
/* 800FC7AC 000F96EC  38 7F 00 00 */	addi r3, r31, 0
/* 800FC7B0 000F96F0  38 81 00 24 */	addi r4, r1, 0x24
/* 800FC7B4 000F96F4  38 A0 00 04 */	li r5, 4
/* 800FC7B8 000F96F8  4B F1 1D D9 */	bl read__14JSUInputStreamFPvl
/* 800FC7BC 000F96FC  38 7F 00 00 */	addi r3, r31, 0
/* 800FC7C0 000F9700  38 81 00 20 */	addi r4, r1, 0x20
/* 800FC7C4 000F9704  38 A0 00 04 */	li r5, 4
/* 800FC7C8 000F9708  4B F1 1D C9 */	bl read__14JSUInputStreamFPvl
/* 800FC7CC 000F970C  80 01 00 44 */	lwz r0, 0x44(r1)
/* 800FC7D0 000F9710  83 E1 00 3C */	lwz r31, 0x3c(r1)
/* 800FC7D4 000F9714  83 C1 00 38 */	lwz r30, 0x38(r1)
/* 800FC7D8 000F9718  7C 08 03 A6 */	mtlr r0
/* 800FC7DC 000F971C  38 21 00 40 */	addi r1, r1, 0x40
/* 800FC7E0 000F9720  4E 80 00 20 */	blr 

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global __vt__18TStagePositionInfo
__vt__18TStagePositionInfo:
  .4byte 0
  .4byte 0
  .4byte __dt__18TStagePositionInfoFv
  .4byte getType__Q26JDrama8TNameRefCFv
  .4byte load__18TStagePositionInfoFR20JSUMemoryInputStream
  .4byte save__Q26JDrama8TNameRefFR21JSUMemoryOutputStream
  .4byte loadAfter__Q26JDrama8TNameRefFv
  .4byte searchF__Q26JDrama8TNameRefFUsPCc

.section .sbss, "wa"  # 0x804097C0 - 0x8040B45C
.global gpPositionHolder
gpPositionHolder:
	.skip 0x8
