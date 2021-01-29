.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0

.global createObjects__12TMarDirectorFv
createObjects__12TMarDirectorFv:
/* 8010C334 00109274  7C 08 02 A6 */	mflr r0
/* 8010C338 00109278  38 60 00 08 */	li r3, 8
/* 8010C33C 0010927C  90 01 00 04 */	stw r0, 4(r1)
/* 8010C340 00109280  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 8010C344 00109284  93 E1 00 14 */	stw r31, 0x14(r1)
/* 8010C348 00109288  4B F0 05 69 */	bl __nw__FUl
/* 8010C34C 0010928C  28 03 00 00 */	cmplwi r3, 0
/* 8010C350 00109290  41 82 00 1C */	beq lbl_8010C36C
/* 8010C354 00109294  3C 80 80 3B */	lis r4, __vt__Q26JDrama11TNameRefGen@ha
/* 8010C358 00109298  38 04 A8 A0 */	addi r0, r4, __vt__Q26JDrama11TNameRefGen@l
/* 8010C35C 0010929C  3C 80 80 3B */	lis r4, __vt__14TMarNameRefGen@ha
/* 8010C360 001092A0  90 03 00 00 */	stw r0, 0(r3)
/* 8010C364 001092A4  38 04 3D 10 */	addi r0, r4, __vt__14TMarNameRefGen@l
/* 8010C368 001092A8  90 03 00 00 */	stw r0, 0(r3)
lbl_8010C36C:
/* 8010C36C 001092AC  90 6D 8E E0 */	stw r3, instance__Q26JDrama11TNameRefGen-_SDA_BASE_(r13)
/* 8010C370 001092B0  38 60 00 58 */	li r3, 0x58
/* 8010C374 001092B4  4B F0 05 3D */	bl __nw__FUl
/* 8010C378 001092B8  7C 7F 1B 79 */	or. r31, r3, r3
/* 8010C37C 001092BC  41 82 00 14 */	beq lbl_8010C390
/* 8010C380 001092C0  3C 60 80 38 */	lis r3, $$21816@ha
/* 8010C384 001092C4  38 83 9A 28 */	addi r4, r3, $$21816@l
/* 8010C388 001092C8  38 7F 00 00 */	addi r3, r31, 0
/* 8010C38C 001092CC  4B FB 75 59 */	bl __ct__22TLightWithDBSetManagerFPCc
lbl_8010C390:
/* 8010C390 001092D0  93 ED 97 6C */	stw r31, gpLightManager-_SDA_BASE_(r13)
/* 8010C394 001092D4  38 60 00 00 */	li r3, 0
/* 8010C398 001092D8  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 8010C39C 001092DC  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 8010C3A0 001092E0  38 21 00 18 */	addi r1, r1, 0x18
/* 8010C3A4 001092E4  7C 08 03 A6 */	mtlr r0
/* 8010C3A8 001092E8  4E 80 00 20 */	blr 
