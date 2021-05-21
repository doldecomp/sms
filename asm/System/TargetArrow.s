.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global __dt__12TTargetArrowFv
__dt__12TTargetArrowFv:
/* 80110F00 0010DE40  7C 08 02 A6 */	mflr r0
/* 80110F04 0010DE44  90 01 00 04 */	stw r0, 4(r1)
/* 80110F08 0010DE48  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 80110F0C 0010DE4C  93 E1 00 14 */	stw r31, 0x14(r1)
/* 80110F10 0010DE50  3B E4 00 00 */	addi r31, r4, 0
/* 80110F14 0010DE54  93 C1 00 10 */	stw r30, 0x10(r1)
/* 80110F18 0010DE58  7C 7E 1B 79 */	or. r30, r3, r3
/* 80110F1C 0010DE5C  41 82 00 3C */	beq lbl_80110F58
/* 80110F20 0010DE60  3C 60 80 3B */	lis r3, __vt__12TTargetArrow@ha
/* 80110F24 0010DE64  38 03 4B 80 */	addi r0, r3, __vt__12TTargetArrow@l
/* 80110F28 0010DE68  90 1E 00 00 */	stw r0, 0(r30)
/* 80110F2C 0010DE6C  41 82 00 1C */	beq lbl_80110F48
/* 80110F30 0010DE70  3C 60 80 3B */	lis r3, __vt__Q26JDrama8TViewObj@ha
/* 80110F34 0010DE74  38 03 A0 C0 */	addi r0, r3, __vt__Q26JDrama8TViewObj@l
/* 80110F38 0010DE78  90 1E 00 00 */	stw r0, 0(r30)
/* 80110F3C 0010DE7C  38 7E 00 00 */	addi r3, r30, 0
/* 80110F40 0010DE80  38 80 00 00 */	li r4, 0
/* 80110F44 0010DE84  4B F3 38 B9 */	bl __dt__Q26JDrama8TNameRefFv
lbl_80110F48:
/* 80110F48 0010DE88  7F E0 07 35 */	extsh. r0, r31
/* 80110F4C 0010DE8C  40 81 00 0C */	ble lbl_80110F58
/* 80110F50 0010DE90  7F C3 F3 78 */	mr r3, r30
/* 80110F54 0010DE94  4B EF BB 5D */	bl __dl__FPv
lbl_80110F58:
/* 80110F58 0010DE98  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 80110F5C 0010DE9C  7F C3 F3 78 */	mr r3, r30
/* 80110F60 0010DEA0  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 80110F64 0010DEA4  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 80110F68 0010DEA8  7C 08 03 A6 */	mtlr r0
/* 80110F6C 0010DEAC  38 21 00 18 */	addi r1, r1, 0x18
/* 80110F70 0010DEB0  4E 80 00 20 */	blr 

.global setPos__12TTargetArrowFRCQ29JGeometry8TVec3$$0f$$1
setPos__12TTargetArrowFRCQ29JGeometry8TVec3$$0f$$1:
/* 80110F74 0010DEB4  80 63 00 10 */	lwz r3, 0x10(r3)
/* 80110F78 0010DEB8  28 03 00 00 */	cmplwi r3, 0
/* 80110F7C 0010DEBC  4D 82 00 20 */	beqlr 
/* 80110F80 0010DEC0  80 63 00 04 */	lwz r3, 4(r3)
/* 80110F84 0010DEC4  C0 04 00 00 */	lfs f0, 0(r4)
/* 80110F88 0010DEC8  38 63 00 20 */	addi r3, r3, 0x20
/* 80110F8C 0010DECC  D0 03 00 0C */	stfs f0, 0xc(r3)
/* 80110F90 0010DED0  C0 04 00 04 */	lfs f0, 4(r4)
/* 80110F94 0010DED4  D0 03 00 1C */	stfs f0, 0x1c(r3)
/* 80110F98 0010DED8  C0 04 00 08 */	lfs f0, 8(r4)
/* 80110F9C 0010DEDC  D0 03 00 2C */	stfs f0, 0x2c(r3)
/* 80110FA0 0010DEE0  4E 80 00 20 */	blr 

.global perform__12TTargetArrowFUlPQ26JDrama9TGraphics
perform__12TTargetArrowFUlPQ26JDrama9TGraphics:
/* 80110FA4 0010DEE4  7C 08 02 A6 */	mflr r0
/* 80110FA8 0010DEE8  90 01 00 04 */	stw r0, 4(r1)
/* 80110FAC 0010DEEC  94 21 FF F8 */	stwu r1, -8(r1)
/* 80110FB0 0010DEF0  80 C3 00 10 */	lwz r6, 0x10(r3)
/* 80110FB4 0010DEF4  28 06 00 00 */	cmplwi r6, 0
/* 80110FB8 0010DEF8  41 82 00 18 */	beq lbl_80110FD0
/* 80110FBC 0010DEFC  88 03 00 14 */	lbz r0, 0x14(r3)
/* 80110FC0 0010DF00  28 00 00 00 */	cmplwi r0, 0
/* 80110FC4 0010DF04  41 82 00 0C */	beq lbl_80110FD0
/* 80110FC8 0010DF08  7C C3 33 78 */	mr r3, r6
/* 80110FCC 0010DF0C  4B FC 35 0D */	bl perform__6MActorFUlPQ26JDrama9TGraphics
lbl_80110FD0:
/* 80110FD0 0010DF10  80 01 00 0C */	lwz r0, 0xc(r1)
/* 80110FD4 0010DF14  38 21 00 08 */	addi r1, r1, 8
/* 80110FD8 0010DF18  7C 08 03 A6 */	mtlr r0
/* 80110FDC 0010DF1C  4E 80 00 20 */	blr 

.global loadAfter__12TTargetArrowFv
loadAfter__12TTargetArrowFv:
/* 80110FE0 0010DF20  7C 08 02 A6 */	mflr r0
/* 80110FE4 0010DF24  90 01 00 04 */	stw r0, 4(r1)
/* 80110FE8 0010DF28  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 80110FEC 0010DF2C  93 E1 00 14 */	stw r31, 0x14(r1)
/* 80110FF0 0010DF30  7C 7F 1B 78 */	mr r31, r3
/* 80110FF4 0010DF34  80 6D 84 70 */	lwz r3, cTargetArrowBmdFileName@sda21(r13)
/* 80110FF8 0010DF38  4B EF AC C5 */	bl getGlbResource__13JKRFileLoaderFPCc
/* 80110FFC 0010DF3C  28 03 00 00 */	cmplwi r3, 0
/* 80111000 0010DF40  41 82 00 30 */	beq lbl_80111030
/* 80111004 0010DF44  3C 60 80 38 */	lis r3, $$21708@ha
/* 80111008 0010DF48  80 8D 84 70 */	lwz r4, cTargetArrowBmdFileName@sda21(r13)
/* 8011100C 0010DF4C  38 63 9F 4C */	addi r3, r3, $$21708@l
/* 80111010 0010DF50  38 A0 00 03 */	li r5, 3
/* 80111014 0010DF54  3C C0 10 01 */	lis r6, 0x1001
/* 80111018 0010DF58  4B FC 8A 5D */	bl SMS_MakeMActor__FPCcPCcUlUl
/* 8011101C 0010DF5C  90 7F 00 10 */	stw r3, 0x10(r31)
/* 80111020 0010DF60  3C 60 80 38 */	lis r3, $$21709@ha
/* 80111024 0010DF64  38 83 9F 64 */	addi r4, r3, $$21709@l
/* 80111028 0010DF68  80 7F 00 10 */	lwz r3, 0x10(r31)
/* 8011102C 0010DF6C  4B FC 31 31 */	bl setBck__6MActorFPCc
lbl_80111030:
/* 80111030 0010DF70  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 80111034 0010DF74  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 80111038 0010DF78  38 21 00 18 */	addi r1, r1, 0x18
/* 8011103C 0010DF7C  7C 08 03 A6 */	mtlr r0
/* 80111040 0010DF80  4E 80 00 20 */	blr 

.section .rodata, "wa"  # 0x8036FFA0 - 0x803A8380
$$21490:
	.incbin "baserom.dol", 0x376E40, 0xC
$$21526:
	.incbin "baserom.dol", 0x376E4C, 0x14
$$21582:
	.incbin "baserom.dol", 0x376E60, 0x30
$$21583:
	.incbin "baserom.dol", 0x376E90, 0x38
$$21584:
	.incbin "baserom.dol", 0x376EC8, 0x34
$$21585:
	.incbin "baserom.dol", 0x376EFC, 0x24
$$21681:
	.incbin "baserom.dol", 0x376F20, 0x2C
$$21708:
	.incbin "baserom.dol", 0x376F4C, 0x18
$$21709:
	.incbin "baserom.dol", 0x376F64, 0x14

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global __vt__12TTargetArrow
__vt__12TTargetArrow:
	.incbin "baserom.dol", 0x3B1B80, 0x28

.section .sdata, "wa"  # 0x80408AC0 - 0x804097C0
cTargetArrowBmdFileName:
	.incbin "baserom.dol", 0x3E3470, 0x8

.section .sbss, "wa"  # 0x804097C0 - 0x8040B45C
.global gpTargetArrow
gpTargetArrow:
	.skip 0x8
