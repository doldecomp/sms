.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global load__15TStageEventInfoFR20JSUMemoryInputStream
load__15TStageEventInfoFR20JSUMemoryInputStream:
/* 800FC46C 000F93AC  7C 08 02 A6 */	mflr r0
/* 800FC470 000F93B0  90 01 00 04 */	stw r0, 4(r1)
/* 800FC474 000F93B4  94 21 FF D8 */	stwu r1, -0x28(r1)
/* 800FC478 000F93B8  93 E1 00 24 */	stw r31, 0x24(r1)
/* 800FC47C 000F93BC  3B E4 00 00 */	addi r31, r4, 0
/* 800FC480 000F93C0  93 C1 00 20 */	stw r30, 0x20(r1)
/* 800FC484 000F93C4  3B C3 00 00 */	addi r30, r3, 0
/* 800FC488 000F93C8  4B F4 83 C9 */	bl load__Q26JDrama8TNameRefFR20JSUMemoryInputStream
/* 800FC48C 000F93CC  38 7F 00 00 */	addi r3, r31, 0
/* 800FC490 000F93D0  38 81 00 18 */	addi r4, r1, 0x18
/* 800FC494 000F93D4  38 A0 00 04 */	li r5, 4
/* 800FC498 000F93D8  4B F1 20 F9 */	bl read__14JSUInputStreamFPvl
/* 800FC49C 000F93DC  80 01 00 18 */	lwz r0, 0x18(r1)
/* 800FC4A0 000F93E0  7F E3 FB 78 */	mr r3, r31
/* 800FC4A4 000F93E4  90 1E 00 0C */	stw r0, 0xc(r30)
/* 800FC4A8 000F93E8  4B F1 21 41 */	bl readString__14JSUInputStreamFv
/* 800FC4AC 000F93EC  90 7E 00 10 */	stw r3, 0x10(r30)
/* 800FC4B0 000F93F0  7F E3 FB 78 */	mr r3, r31
/* 800FC4B4 000F93F4  4B F1 21 35 */	bl readString__14JSUInputStreamFv
/* 800FC4B8 000F93F8  90 7E 00 14 */	stw r3, 0x14(r30)
/* 800FC4BC 000F93FC  7F E3 FB 78 */	mr r3, r31
/* 800FC4C0 000F9400  4B F1 21 29 */	bl readString__14JSUInputStreamFv
/* 800FC4C4 000F9404  90 7E 00 18 */	stw r3, 0x18(r30)
/* 800FC4C8 000F9408  7F E3 FB 78 */	mr r3, r31
/* 800FC4CC 000F940C  4B F1 21 1D */	bl readString__14JSUInputStreamFv
/* 800FC4D0 000F9410  90 7E 00 1C */	stw r3, 0x1c(r30)
/* 800FC4D4 000F9414  7F E3 FB 78 */	mr r3, r31
/* 800FC4D8 000F9418  4B F1 21 11 */	bl readString__14JSUInputStreamFv
/* 800FC4DC 000F941C  90 7E 00 20 */	stw r3, 0x20(r30)
/* 800FC4E0 000F9420  7F E3 FB 78 */	mr r3, r31
/* 800FC4E4 000F9424  4B F1 21 05 */	bl readString__14JSUInputStreamFv
/* 800FC4E8 000F9428  90 7E 00 24 */	stw r3, 0x24(r30)
/* 800FC4EC 000F942C  80 01 00 2C */	lwz r0, 0x2c(r1)
/* 800FC4F0 000F9430  83 E1 00 24 */	lwz r31, 0x24(r1)
/* 800FC4F4 000F9434  83 C1 00 20 */	lwz r30, 0x20(r1)
/* 800FC4F8 000F9438  7C 08 03 A6 */	mtlr r0
/* 800FC4FC 000F943C  38 21 00 28 */	addi r1, r1, 0x28
/* 800FC500 000F9440  4E 80 00 20 */	blr 

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global __vt__15TStageEventInfo
__vt__15TStageEventInfo:
	.incbin "baserom.dol", 0x3B10C8, 0x20
