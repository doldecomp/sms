.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global "__ct__Q26JDrama14TViewConnecterFPQ26JDrama8TViewObjPQ26JDrama8TViewObjQ26JDrama10TFlagT<Us>PCc"
"__ct__Q26JDrama14TViewConnecterFPQ26JDrama8TViewObjPQ26JDrama8TViewObjQ26JDrama10TFlagT<Us>PCc":
/* 80046E34 00043D74  7C 08 02 A6 */	mflr r0
/* 80046E38 00043D78  3D 00 80 3B */	lis r8, __vt__Q26JDrama8TNameRef@ha
/* 80046E3C 00043D7C  90 01 00 04 */	stw r0, 4(r1)
/* 80046E40 00043D80  38 08 A8 60 */	addi r0, r8, __vt__Q26JDrama8TNameRef@l
/* 80046E44 00043D84  94 21 FF D0 */	stwu r1, -0x30(r1)
/* 80046E48 00043D88  93 E1 00 2C */	stw r31, 0x2c(r1)
/* 80046E4C 00043D8C  3B E6 00 00 */	addi r31, r6, 0
/* 80046E50 00043D90  93 C1 00 28 */	stw r30, 0x28(r1)
/* 80046E54 00043D94  3B C5 00 00 */	addi r30, r5, 0
/* 80046E58 00043D98  93 A1 00 24 */	stw r29, 0x24(r1)
/* 80046E5C 00043D9C  7C 9D 23 78 */	mr r29, r4
/* 80046E60 00043DA0  93 81 00 20 */	stw r28, 0x20(r1)
/* 80046E64 00043DA4  3B 83 00 00 */	addi r28, r3, 0
/* 80046E68 00043DA8  90 03 00 00 */	stw r0, 0(r3)
/* 80046E6C 00043DAC  90 E3 00 04 */	stw r7, 4(r3)
/* 80046E70 00043DB0  38 67 00 00 */	addi r3, r7, 0
/* 80046E74 00043DB4  4B FF D8 19 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 80046E78 00043DB8  B0 7C 00 08 */	sth r3, 8(r28)
/* 80046E7C 00043DBC  3C 60 80 3B */	lis r3, __vt__Q26JDrama8TViewObj@ha
/* 80046E80 00043DC0  38 03 A0 C0 */	addi r0, r3, __vt__Q26JDrama8TViewObj@l
/* 80046E84 00043DC4  90 1C 00 00 */	stw r0, 0(r28)
/* 80046E88 00043DC8  38 00 00 00 */	li r0, 0
/* 80046E8C 00043DCC  3C 60 80 3B */	lis r3, __vt__Q26JDrama14TViewConnecter@ha
/* 80046E90 00043DD0  B0 1C 00 0C */	sth r0, 0xc(r28)
/* 80046E94 00043DD4  38 03 AA 00 */	addi r0, r3, __vt__Q26JDrama14TViewConnecter@l
/* 80046E98 00043DD8  38 7C 00 00 */	addi r3, r28, 0
/* 80046E9C 00043DDC  90 1C 00 00 */	stw r0, 0(r28)
/* 80046EA0 00043DE0  93 BC 00 10 */	stw r29, 0x10(r28)
/* 80046EA4 00043DE4  93 DC 00 14 */	stw r30, 0x14(r28)
/* 80046EA8 00043DE8  A0 1F 00 00 */	lhz r0, 0(r31)
/* 80046EAC 00043DEC  B0 1C 00 18 */	sth r0, 0x18(r28)
/* 80046EB0 00043DF0  80 01 00 34 */	lwz r0, 0x34(r1)
/* 80046EB4 00043DF4  83 E1 00 2C */	lwz r31, 0x2c(r1)
/* 80046EB8 00043DF8  83 C1 00 28 */	lwz r30, 0x28(r1)
/* 80046EBC 00043DFC  83 A1 00 24 */	lwz r29, 0x24(r1)
/* 80046EC0 00043E00  83 81 00 20 */	lwz r28, 0x20(r1)
/* 80046EC4 00043E04  38 21 00 30 */	addi r1, r1, 0x30
/* 80046EC8 00043E08  7C 08 03 A6 */	mtlr r0
/* 80046ECC 00043E0C  4E 80 00 20 */	blr 

.global perform__Q26JDrama14TViewConnecterFUlPQ26JDrama9TGraphics
perform__Q26JDrama14TViewConnecterFUlPQ26JDrama9TGraphics:
/* 80046ED0 00043E10  7C 08 02 A6 */	mflr r0
/* 80046ED4 00043E14  90 01 00 04 */	stw r0, 4(r1)
/* 80046ED8 00043E18  54 80 07 39 */	rlwinm. r0, r4, 0, 0x1c, 0x1c
/* 80046EDC 00043E1C  94 21 FF D8 */	stwu r1, -0x28(r1)
/* 80046EE0 00043E20  93 E1 00 24 */	stw r31, 0x24(r1)
/* 80046EE4 00043E24  3B E5 00 00 */	addi r31, r5, 0
/* 80046EE8 00043E28  93 C1 00 20 */	stw r30, 0x20(r1)
/* 80046EEC 00043E2C  3B C4 00 00 */	addi r30, r4, 0
/* 80046EF0 00043E30  93 A1 00 1C */	stw r29, 0x1c(r1)
/* 80046EF4 00043E34  3B A3 00 00 */	addi r29, r3, 0
/* 80046EF8 00043E38  41 82 00 58 */	beq lbl_80046F50
/* 80046EFC 00043E3C  80 7D 00 10 */	lwz r3, 0x10(r29)
/* 80046F00 00043E40  28 03 00 00 */	cmplwi r3, 0
/* 80046F04 00043E44  41 82 00 10 */	beq lbl_80046F14
/* 80046F08 00043E48  38 BF 00 00 */	addi r5, r31, 0
/* 80046F0C 00043E4C  38 80 00 80 */	li r4, 0x80
/* 80046F10 00043E50  48 00 00 5D */	bl testPerform__Q26JDrama8TViewObjFUlPQ26JDrama9TGraphics
lbl_80046F14:
/* 80046F14 00043E54  80 7D 00 14 */	lwz r3, 0x14(r29)
/* 80046F18 00043E58  28 03 00 00 */	cmplwi r3, 0
/* 80046F1C 00043E5C  41 82 00 10 */	beq lbl_80046F2C
/* 80046F20 00043E60  38 9E 00 00 */	addi r4, r30, 0
/* 80046F24 00043E64  38 BF 00 00 */	addi r5, r31, 0
/* 80046F28 00043E68  48 00 00 45 */	bl testPerform__Q26JDrama8TViewObjFUlPQ26JDrama9TGraphics
lbl_80046F2C:
/* 80046F2C 00043E6C  80 7D 00 10 */	lwz r3, 0x10(r29)
/* 80046F30 00043E70  28 03 00 00 */	cmplwi r3, 0
/* 80046F34 00043E74  41 82 00 1C */	beq lbl_80046F50
/* 80046F38 00043E78  A0 1D 00 18 */	lhz r0, 0x18(r29)
/* 80046F3C 00043E7C  54 00 07 FF */	clrlwi. r0, r0, 0x1f
/* 80046F40 00043E80  41 82 00 10 */	beq lbl_80046F50
/* 80046F44 00043E84  38 BF 00 00 */	addi r5, r31, 0
/* 80046F48 00043E88  38 80 00 08 */	li r4, 8
/* 80046F4C 00043E8C  48 00 00 21 */	bl testPerform__Q26JDrama8TViewObjFUlPQ26JDrama9TGraphics
lbl_80046F50:
/* 80046F50 00043E90  80 01 00 2C */	lwz r0, 0x2c(r1)
/* 80046F54 00043E94  83 E1 00 24 */	lwz r31, 0x24(r1)
/* 80046F58 00043E98  83 C1 00 20 */	lwz r30, 0x20(r1)
/* 80046F5C 00043E9C  7C 08 03 A6 */	mtlr r0
/* 80046F60 00043EA0  83 A1 00 1C */	lwz r29, 0x1c(r1)
/* 80046F64 00043EA4  38 21 00 28 */	addi r1, r1, 0x28
/* 80046F68 00043EA8  4E 80 00 20 */	blr 

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global __vt__Q26JDrama14TViewConnecter
__vt__Q26JDrama14TViewConnecter:
  .4byte 0
  .4byte 0
  .4byte __dt__Q26JDrama14TViewConnecterFv
  .4byte getType__Q26JDrama8TNameRefCFv
  .4byte load__Q26JDrama8TNameRefFR20JSUMemoryInputStream
  .4byte save__Q26JDrama8TNameRefFR21JSUMemoryOutputStream
  .4byte loadAfter__Q26JDrama8TNameRefFv
  .4byte searchF__Q26JDrama8TNameRefFUsPCc
  .4byte perform__Q26JDrama14TViewConnecterFUlPQ26JDrama9TGraphics
  .4byte 0
