.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global perform__13TCamConnecterFUlPQ26JDrama9TGraphics
perform__13TCamConnecterFUlPQ26JDrama9TGraphics:
/* 80045EF4 00042E34  7C 08 02 A6 */	mflr r0
/* 80045EF8 00042E38  90 01 00 04 */	stw r0, 4(r1)
/* 80045EFC 00042E3C  54 80 07 39 */	rlwinm. r0, r4, 0, 0x1c, 0x1c
/* 80045F00 00042E40  94 21 FF D8 */	stwu r1, -0x28(r1)
/* 80045F04 00042E44  93 E1 00 24 */	stw r31, 0x24(r1)
/* 80045F08 00042E48  3B E5 00 00 */	addi r31, r5, 0
/* 80045F0C 00042E4C  93 C1 00 20 */	stw r30, 0x20(r1)
/* 80045F10 00042E50  3B C4 00 00 */	addi r30, r4, 0
/* 80045F14 00042E54  93 A1 00 1C */	stw r29, 0x1c(r1)
/* 80045F18 00042E58  3B A3 00 00 */	addi r29, r3, 0
/* 80045F1C 00042E5C  41 82 00 58 */	beq lbl_80045F74
/* 80045F20 00042E60  80 7D 00 10 */	lwz r3, 0x10(r29)
/* 80045F24 00042E64  28 03 00 00 */	cmplwi r3, 0
/* 80045F28 00042E68  41 82 00 10 */	beq lbl_80045F38
/* 80045F2C 00042E6C  38 BF 00 00 */	addi r5, r31, 0
/* 80045F30 00042E70  38 80 00 10 */	li r4, 0x10
/* 80045F34 00042E74  48 00 10 39 */	bl testPerform__Q26JDrama8TViewObjFUlPQ26JDrama9TGraphics
lbl_80045F38:
/* 80045F38 00042E78  80 7D 00 14 */	lwz r3, 0x14(r29)
/* 80045F3C 00042E7C  28 03 00 00 */	cmplwi r3, 0
/* 80045F40 00042E80  41 82 00 10 */	beq lbl_80045F50
/* 80045F44 00042E84  38 9E 00 00 */	addi r4, r30, 0
/* 80045F48 00042E88  38 BF 00 00 */	addi r5, r31, 0
/* 80045F4C 00042E8C  48 00 10 21 */	bl testPerform__Q26JDrama8TViewObjFUlPQ26JDrama9TGraphics
lbl_80045F50:
/* 80045F50 00042E90  80 7D 00 10 */	lwz r3, 0x10(r29)
/* 80045F54 00042E94  28 03 00 00 */	cmplwi r3, 0
/* 80045F58 00042E98  41 82 00 1C */	beq lbl_80045F74
/* 80045F5C 00042E9C  A0 1D 00 18 */	lhz r0, 0x18(r29)
/* 80045F60 00042EA0  54 00 07 FF */	clrlwi. r0, r0, 0x1f
/* 80045F64 00042EA4  41 82 00 10 */	beq lbl_80045F74
/* 80045F68 00042EA8  38 BF 00 00 */	addi r5, r31, 0
/* 80045F6C 00042EAC  38 80 00 08 */	li r4, 8
/* 80045F70 00042EB0  48 00 0F FD */	bl testPerform__Q26JDrama8TViewObjFUlPQ26JDrama9TGraphics
lbl_80045F74:
/* 80045F74 00042EB4  80 01 00 2C */	lwz r0, 0x2c(r1)
/* 80045F78 00042EB8  83 E1 00 24 */	lwz r31, 0x24(r1)
/* 80045F7C 00042EBC  83 C1 00 20 */	lwz r30, 0x20(r1)
/* 80045F80 00042EC0  7C 08 03 A6 */	mtlr r0
/* 80045F84 00042EC4  83 A1 00 1C */	lwz r29, 0x1c(r1)
/* 80045F88 00042EC8  38 21 00 28 */	addi r1, r1, 0x28
/* 80045F8C 00042ECC  4E 80 00 20 */	blr 

.global __ct__Q26JDrama7TScreenFRCQ26JDrama5TRectPCc
__ct__Q26JDrama7TScreenFRCQ26JDrama5TRectPCc:
/* 80045F90 00042ED0  7C 08 02 A6 */	mflr r0
/* 80045F94 00042ED4  38 E5 00 00 */	addi r7, r5, 0
/* 80045F98 00042ED8  90 01 00 04 */	stw r0, 4(r1)
/* 80045F9C 00042EDC  38 00 00 00 */	li r0, 0
/* 80045FA0 00042EE0  38 A0 00 00 */	li r5, 0
/* 80045FA4 00042EE4  94 21 FF D8 */	stwu r1, -0x28(r1)
/* 80045FA8 00042EE8  93 E1 00 24 */	stw r31, 0x24(r1)
/* 80045FAC 00042EEC  7C 9F 23 78 */	mr r31, r4
/* 80045FB0 00042EF0  38 C1 00 18 */	addi r6, r1, 0x18
/* 80045FB4 00042EF4  93 C1 00 20 */	stw r30, 0x20(r1)
/* 80045FB8 00042EF8  38 80 00 00 */	li r4, 0
/* 80045FBC 00042EFC  90 61 00 08 */	stw r3, 8(r1)
/* 80045FC0 00042F00  B0 01 00 18 */	sth r0, 0x18(r1)
/* 80045FC4 00042F04  80 61 00 08 */	lwz r3, 8(r1)
/* 80045FC8 00042F08  48 00 0E 6D */	bl "__ct__Q26JDrama14TViewConnecterFPQ26JDrama8TViewObjPQ26JDrama8TViewObjQ26JDrama10TFlagT<Us>PCc"
/* 80045FCC 00042F0C  3C 80 80 3B */	lis r4, __vt__Q26JDrama7TScreen@ha
/* 80045FD0 00042F10  80 61 00 08 */	lwz r3, 8(r1)
/* 80045FD4 00042F14  38 04 A8 D8 */	addi r0, r4, __vt__Q26JDrama7TScreen@l
/* 80045FD8 00042F18  90 03 00 00 */	stw r0, 0(r3)
/* 80045FDC 00042F1C  38 60 00 24 */	li r3, 0x24
/* 80045FE0 00042F20  4B FC 68 D1 */	bl __nw__FUl
/* 80045FE4 00042F24  7C 7E 1B 79 */	or. r30, r3, r3
/* 80045FE8 00042F28  41 82 00 18 */	beq lbl_80046000
/* 80045FEC 00042F2C  3C 60 80 37 */	lis r3, "@293"@ha
/* 80045FF0 00042F30  38 A3 10 E0 */	addi r5, r3, "@293"@l
/* 80045FF4 00042F34  38 7E 00 00 */	addi r3, r30, 0
/* 80045FF8 00042F38  38 9F 00 00 */	addi r4, r31, 0
/* 80045FFC 00042F3C  48 00 0F D9 */	bl __ct__Q26JDrama9TViewportFRCQ26JDrama5TRectPCc
lbl_80046000:
/* 80046000 00042F40  80 81 00 08 */	lwz r4, 8(r1)
/* 80046004 00042F44  38 60 00 1C */	li r3, 0x1c
/* 80046008 00042F48  93 C4 00 10 */	stw r30, 0x10(r4)
/* 8004600C 00042F4C  4B FC 68 A5 */	bl __nw__FUl
/* 80046010 00042F50  7C 7E 1B 79 */	or. r30, r3, r3
/* 80046014 00042F54  41 82 00 34 */	beq lbl_80046048
/* 80046018 00042F58  38 00 00 00 */	li r0, 0
/* 8004601C 00042F5C  3C 80 80 37 */	lis r4, "@294"@ha
/* 80046020 00042F60  B0 01 00 14 */	sth r0, 0x14(r1)
/* 80046024 00042F64  38 E4 10 EC */	addi r7, r4, "@294"@l
/* 80046028 00042F68  38 7E 00 00 */	addi r3, r30, 0
/* 8004602C 00042F6C  38 C1 00 14 */	addi r6, r1, 0x14
/* 80046030 00042F70  38 80 00 00 */	li r4, 0
/* 80046034 00042F74  38 A0 00 00 */	li r5, 0
/* 80046038 00042F78  48 00 0D FD */	bl "__ct__Q26JDrama14TViewConnecterFPQ26JDrama8TViewObjPQ26JDrama8TViewObjQ26JDrama10TFlagT<Us>PCc"
/* 8004603C 00042F7C  3C 60 80 3B */	lis r3, __vt__13TCamConnecter@ha
/* 80046040 00042F80  38 03 A8 FC */	addi r0, r3, __vt__13TCamConnecter@l
/* 80046044 00042F84  90 1E 00 00 */	stw r0, 0(r30)
lbl_80046048:
/* 80046048 00042F88  80 61 00 08 */	lwz r3, 8(r1)
/* 8004604C 00042F8C  93 C3 00 14 */	stw r30, 0x14(r3)
/* 80046050 00042F90  80 01 00 2C */	lwz r0, 0x2c(r1)
/* 80046054 00042F94  83 E1 00 24 */	lwz r31, 0x24(r1)
/* 80046058 00042F98  83 C1 00 20 */	lwz r30, 0x20(r1)
/* 8004605C 00042F9C  7C 08 03 A6 */	mtlr r0
/* 80046060 00042FA0  38 21 00 28 */	addi r1, r1, 0x28
/* 80046064 00042FA4  4E 80 00 20 */	blr 

.global assignCamera__Q26JDrama7TScreenFPQ26JDrama8TViewObj
assignCamera__Q26JDrama7TScreenFPQ26JDrama8TViewObj:
/* 80046068 00042FA8  80 63 00 14 */	lwz r3, 0x14(r3)
/* 8004606C 00042FAC  90 83 00 10 */	stw r4, 0x10(r3)
/* 80046070 00042FB0  4E 80 00 20 */	blr 

.global assignViewObj__Q26JDrama7TScreenFPQ26JDrama8TViewObj
assignViewObj__Q26JDrama7TScreenFPQ26JDrama8TViewObj:
/* 80046074 00042FB4  80 63 00 14 */	lwz r3, 0x14(r3)
/* 80046078 00042FB8  90 83 00 14 */	stw r4, 0x14(r3)
/* 8004607C 00042FBC  4E 80 00 20 */	blr 

.global __dt__Q26JDrama7TScreenFv
__dt__Q26JDrama7TScreenFv:
/* 80046080 00042FC0  7C 08 02 A6 */	mflr r0
/* 80046084 00042FC4  90 01 00 04 */	stw r0, 4(r1)
/* 80046088 00042FC8  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 8004608C 00042FCC  93 E1 00 14 */	stw r31, 0x14(r1)
/* 80046090 00042FD0  3B E4 00 00 */	addi r31, r4, 0
/* 80046094 00042FD4  93 C1 00 10 */	stw r30, 0x10(r1)
/* 80046098 00042FD8  7C 7E 1B 79 */	or. r30, r3, r3
/* 8004609C 00042FDC  41 82 00 4C */	beq lbl_800460E8
/* 800460A0 00042FE0  3C 60 80 3B */	lis r3, __vt__Q26JDrama7TScreen@ha
/* 800460A4 00042FE4  38 03 A8 D8 */	addi r0, r3, __vt__Q26JDrama7TScreen@l
/* 800460A8 00042FE8  90 1E 00 00 */	stw r0, 0(r30)
/* 800460AC 00042FEC  41 82 00 2C */	beq lbl_800460D8
/* 800460B0 00042FF0  3C 60 80 3B */	lis r3, __vt__Q26JDrama14TViewConnecter@ha
/* 800460B4 00042FF4  38 03 AA 00 */	addi r0, r3, __vt__Q26JDrama14TViewConnecter@l
/* 800460B8 00042FF8  90 1E 00 00 */	stw r0, 0(r30)
/* 800460BC 00042FFC  41 82 00 1C */	beq lbl_800460D8
/* 800460C0 00043000  3C 60 80 3B */	lis r3, __vt__Q26JDrama8TViewObj@ha
/* 800460C4 00043004  38 03 A0 C0 */	addi r0, r3, __vt__Q26JDrama8TViewObj@l
/* 800460C8 00043008  90 1E 00 00 */	stw r0, 0(r30)
/* 800460CC 0004300C  38 7E 00 00 */	addi r3, r30, 0
/* 800460D0 00043010  38 80 00 00 */	li r4, 0
/* 800460D4 00043014  4B FF E7 29 */	bl __dt__Q26JDrama8TNameRefFv
lbl_800460D8:
/* 800460D8 00043018  7F E0 07 35 */	extsh. r0, r31
/* 800460DC 0004301C  40 81 00 0C */	ble lbl_800460E8
/* 800460E0 00043020  7F C3 F3 78 */	mr r3, r30
/* 800460E4 00043024  4B FC 69 CD */	bl __dl__FPv
lbl_800460E8:
/* 800460E8 00043028  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 800460EC 0004302C  7F C3 F3 78 */	mr r3, r30
/* 800460F0 00043030  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 800460F4 00043034  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 800460F8 00043038  7C 08 03 A6 */	mtlr r0
/* 800460FC 0004303C  38 21 00 18 */	addi r1, r1, 0x18
/* 80046100 00043040  4E 80 00 20 */	blr 

.global __dt__13TCamConnecterFv
__dt__13TCamConnecterFv:
/* 80046104 00043044  7C 08 02 A6 */	mflr r0
/* 80046108 00043048  90 01 00 04 */	stw r0, 4(r1)
/* 8004610C 0004304C  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 80046110 00043050  93 E1 00 14 */	stw r31, 0x14(r1)
/* 80046114 00043054  3B E4 00 00 */	addi r31, r4, 0
/* 80046118 00043058  93 C1 00 10 */	stw r30, 0x10(r1)
/* 8004611C 0004305C  7C 7E 1B 79 */	or. r30, r3, r3
/* 80046120 00043060  41 82 00 4C */	beq lbl_8004616C
/* 80046124 00043064  3C 60 80 3B */	lis r3, __vt__13TCamConnecter@ha
/* 80046128 00043068  38 03 A8 FC */	addi r0, r3, __vt__13TCamConnecter@l
/* 8004612C 0004306C  90 1E 00 00 */	stw r0, 0(r30)
/* 80046130 00043070  41 82 00 2C */	beq lbl_8004615C
/* 80046134 00043074  3C 60 80 3B */	lis r3, __vt__Q26JDrama14TViewConnecter@ha
/* 80046138 00043078  38 03 AA 00 */	addi r0, r3, __vt__Q26JDrama14TViewConnecter@l
/* 8004613C 0004307C  90 1E 00 00 */	stw r0, 0(r30)
/* 80046140 00043080  41 82 00 1C */	beq lbl_8004615C
/* 80046144 00043084  3C 60 80 3B */	lis r3, __vt__Q26JDrama8TViewObj@ha
/* 80046148 00043088  38 03 A0 C0 */	addi r0, r3, __vt__Q26JDrama8TViewObj@l
/* 8004614C 0004308C  90 1E 00 00 */	stw r0, 0(r30)
/* 80046150 00043090  38 7E 00 00 */	addi r3, r30, 0
/* 80046154 00043094  38 80 00 00 */	li r4, 0
/* 80046158 00043098  4B FF E6 A5 */	bl __dt__Q26JDrama8TNameRefFv
lbl_8004615C:
/* 8004615C 0004309C  7F E0 07 35 */	extsh. r0, r31
/* 80046160 000430A0  40 81 00 0C */	ble lbl_8004616C
/* 80046164 000430A4  7F C3 F3 78 */	mr r3, r30
/* 80046168 000430A8  4B FC 69 49 */	bl __dl__FPv
lbl_8004616C:
/* 8004616C 000430AC  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 80046170 000430B0  7F C3 F3 78 */	mr r3, r30
/* 80046174 000430B4  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 80046178 000430B8  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 8004617C 000430BC  7C 08 03 A6 */	mtlr r0
/* 80046180 000430C0  38 21 00 18 */	addi r1, r1, 0x18
/* 80046184 000430C4  4E 80 00 20 */	blr 

.section .rodata, "a"  # 0x8036FFA0 - 0x803A8380
.balign 8
"@293":
	.asciz "<Viewport>"
	.balign 4
"@294":
	.asciz "<CamConnecter>"
	.balign 4
	.4byte 0

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global __vt__Q26JDrama7TScreen
__vt__Q26JDrama7TScreen:
  .4byte 0
  .4byte 0
  .4byte __dt__Q26JDrama7TScreenFv
  .4byte getType__Q26JDrama8TNameRefCFv
  .4byte load__Q26JDrama8TNameRefFR20JSUMemoryInputStream
  .4byte save__Q26JDrama8TNameRefFR21JSUMemoryOutputStream
  .4byte loadAfter__Q26JDrama8TNameRefFv
  .4byte searchF__Q26JDrama8TNameRefFUsPCc
  .4byte perform__Q26JDrama14TViewConnecterFUlPQ26JDrama9TGraphics
.global __vt__13TCamConnecter
__vt__13TCamConnecter:
  .4byte 0
  .4byte 0
  .4byte __dt__13TCamConnecterFv
  .4byte getType__Q26JDrama8TNameRefCFv
  .4byte load__Q26JDrama8TNameRefFR20JSUMemoryInputStream
  .4byte save__Q26JDrama8TNameRefFR21JSUMemoryOutputStream
  .4byte loadAfter__Q26JDrama8TNameRefFv
  .4byte searchF__Q26JDrama8TNameRefFUsPCc
  .4byte perform__13TCamConnecterFUlPQ26JDrama9TGraphics
