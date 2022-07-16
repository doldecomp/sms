.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global "__ct__Q26JDrama11TDStageDispFPCcQ26JDrama10TFlagT<Us>"
"__ct__Q26JDrama11TDStageDispFPCcQ26JDrama10TFlagT<Us>":
/* 8004230C 0003F24C  7C 08 02 A6 */	mflr r0
/* 80042310 0003F250  38 E4 00 00 */	addi r7, r4, 0
/* 80042314 0003F254  90 01 00 04 */	stw r0, 4(r1)
/* 80042318 0003F258  38 00 00 01 */	li r0, 1
/* 8004231C 0003F25C  38 80 00 00 */	li r4, 0
/* 80042320 0003F260  94 21 FF B0 */	stwu r1, -0x50(r1)
/* 80042324 0003F264  93 E1 00 4C */	stw r31, 0x4c(r1)
/* 80042328 0003F268  38 C1 00 38 */	addi r6, r1, 0x38
/* 8004232C 0003F26C  93 C1 00 48 */	stw r30, 0x48(r1)
/* 80042330 0003F270  7C BE 2B 78 */	mr r30, r5
/* 80042334 0003F274  38 A0 00 00 */	li r5, 0
/* 80042338 0003F278  93 A1 00 44 */	stw r29, 0x44(r1)
/* 8004233C 0003F27C  90 61 00 08 */	stw r3, 8(r1)
/* 80042340 0003F280  B0 01 00 38 */	sth r0, 0x38(r1)
/* 80042344 0003F284  80 61 00 08 */	lwz r3, 8(r1)
/* 80042348 0003F288  48 00 4A ED */	bl "__ct__Q26JDrama14TViewConnecterFPQ26JDrama8TViewObjPQ26JDrama8TViewObjQ26JDrama10TFlagT<Us>PCc"
/* 8004234C 0003F28C  3C 80 80 3B */	lis r4, __vt__Q26JDrama11TDStageDisp@ha
/* 80042350 0003F290  80 61 00 08 */	lwz r3, 8(r1)
/* 80042354 0003F294  38 04 A5 6C */	addi r0, r4, __vt__Q26JDrama11TDStageDisp@l
/* 80042358 0003F298  90 03 00 00 */	stw r0, 0(r3)
/* 8004235C 0003F29C  38 60 00 24 */	li r3, 0x24
/* 80042360 0003F2A0  4B FC A5 51 */	bl __nw__FUl
/* 80042364 0003F2A4  7C 7D 1B 79 */	or. r29, r3, r3
/* 80042368 0003F2A8  41 82 00 74 */	beq lbl_800423DC
/* 8004236C 0003F2AC  A3 DE 00 00 */	lhz r30, 0(r30)
/* 80042370 0003F2B0  3C 60 80 37 */	lis r3, "@881"@ha
/* 80042374 0003F2B4  38 83 0F 10 */	addi r4, r3, "@881"@l
/* 80042378 0003F2B8  93 A1 00 20 */	stw r29, 0x20(r1)
/* 8004237C 0003F2BC  93 A1 00 28 */	stw r29, 0x28(r1)
/* 80042380 0003F2C0  80 61 00 20 */	lwz r3, 0x20(r1)
/* 80042384 0003F2C4  4B FF E7 85 */	bl __ct__Q26JDrama8TNameRefFPCc
/* 80042388 0003F2C8  80 61 00 20 */	lwz r3, 0x20(r1)
/* 8004238C 0003F2CC  3C 80 80 3B */	lis r4, __vt__Q26JDrama8TViewObj@ha
/* 80042390 0003F2D0  38 04 A0 C0 */	addi r0, r4, __vt__Q26JDrama8TViewObj@l
/* 80042394 0003F2D4  90 03 00 00 */	stw r0, 0(r3)
/* 80042398 0003F2D8  38 63 00 0C */	addi r3, r3, 0xc
/* 8004239C 0003F2DC  38 80 00 00 */	li r4, 0
/* 800423A0 0003F2E0  4B FF E7 61 */	bl "__ct__Q26JDrama10TFlagT<Us>FUs"
/* 800423A4 0003F2E4  83 E1 00 28 */	lwz r31, 0x28(r1)
/* 800423A8 0003F2E8  3C 60 80 3B */	lis r3, __vt__Q26JDrama8TEfbCtrl@ha
/* 800423AC 0003F2EC  38 03 A6 08 */	addi r0, r3, __vt__Q26JDrama8TEfbCtrl@l
/* 800423B0 0003F2F0  90 1F 00 00 */	stw r0, 0(r31)
/* 800423B4 0003F2F4  38 7F 00 10 */	addi r3, r31, 0x10
/* 800423B8 0003F2F8  38 80 00 00 */	li r4, 0
/* 800423BC 0003F2FC  38 A0 00 00 */	li r5, 0
/* 800423C0 0003F300  38 C0 00 00 */	li r6, 0
/* 800423C4 0003F304  38 E0 00 00 */	li r7, 0
/* 800423C8 0003F308  4B FD 0E 55 */	bl set__7JUTRectFiiii
/* 800423CC 0003F30C  3C 60 80 3B */	lis r3, __vt__Q26JDrama12TEfbCtrlDisp@ha
/* 800423D0 0003F310  B3 DF 00 20 */	sth r30, 0x20(r31)
/* 800423D4 0003F314  38 03 A5 E4 */	addi r0, r3, __vt__Q26JDrama12TEfbCtrlDisp@l
/* 800423D8 0003F318  90 1D 00 00 */	stw r0, 0(r29)
lbl_800423DC:
/* 800423DC 0003F31C  80 81 00 08 */	lwz r4, 8(r1)
/* 800423E0 0003F320  38 60 00 20 */	li r3, 0x20
/* 800423E4 0003F324  93 A4 00 10 */	stw r29, 0x10(r4)
/* 800423E8 0003F328  4B FC A4 C9 */	bl __nw__FUl
/* 800423EC 0003F32C  7C 7D 1B 79 */	or. r29, r3, r3
/* 800423F0 0003F330  41 82 00 54 */	beq lbl_80042444
/* 800423F4 0003F334  93 A1 00 1C */	stw r29, 0x1c(r1)
/* 800423F8 0003F338  3C 60 80 37 */	lis r3, "@882"@ha
/* 800423FC 0003F33C  38 83 0F 20 */	addi r4, r3, "@882"@l
/* 80042400 0003F340  93 A1 00 24 */	stw r29, 0x24(r1)
/* 80042404 0003F344  80 61 00 1C */	lwz r3, 0x1c(r1)
/* 80042408 0003F348  4B FF E7 01 */	bl __ct__Q26JDrama8TNameRefFPCc
/* 8004240C 0003F34C  80 61 00 1C */	lwz r3, 0x1c(r1)
/* 80042410 0003F350  3C 80 80 3B */	lis r4, __vt__Q26JDrama8TViewObj@ha
/* 80042414 0003F354  38 04 A0 C0 */	addi r0, r4, __vt__Q26JDrama8TViewObj@l
/* 80042418 0003F358  90 03 00 00 */	stw r0, 0(r3)
/* 8004241C 0003F35C  38 63 00 0C */	addi r3, r3, 0xc
/* 80042420 0003F360  38 80 00 00 */	li r4, 0
/* 80042424 0003F364  4B FF E6 DD */	bl "__ct__Q26JDrama10TFlagT<Us>FUs"
/* 80042428 0003F368  83 E1 00 24 */	lwz r31, 0x24(r1)
/* 8004242C 0003F36C  38 81 00 18 */	addi r4, r1, 0x18
/* 80042430 0003F370  38 7F 00 10 */	addi r3, r31, 0x10
/* 80042434 0003F374  4B FF 1D C9 */	bl "__ct__Q27JGadget18TList_pointer_voidFRCQ27JGadget14TAllocator<Pv>"
/* 80042438 0003F378  3C 60 80 3B */	lis r3, "__vt__Q26JDrama55TViewObjPtrListT<Q26JDrama8TViewObj,Q26JDrama8TViewObj>"@ha
/* 8004243C 0003F37C  38 03 A5 40 */	addi r0, r3, "__vt__Q26JDrama55TViewObjPtrListT<Q26JDrama8TViewObj,Q26JDrama8TViewObj>"@l
/* 80042440 0003F380  90 1F 00 00 */	stw r0, 0(r31)
lbl_80042444:
/* 80042444 0003F384  80 61 00 08 */	lwz r3, 8(r1)
/* 80042448 0003F388  93 A3 00 14 */	stw r29, 0x14(r3)
/* 8004244C 0003F38C  80 01 00 54 */	lwz r0, 0x54(r1)
/* 80042450 0003F390  83 E1 00 4C */	lwz r31, 0x4c(r1)
/* 80042454 0003F394  83 C1 00 48 */	lwz r30, 0x48(r1)
/* 80042458 0003F398  7C 08 03 A6 */	mtlr r0
/* 8004245C 0003F39C  83 A1 00 44 */	lwz r29, 0x44(r1)
/* 80042460 0003F3A0  38 21 00 50 */	addi r1, r1, 0x50
/* 80042464 0003F3A4  4E 80 00 20 */	blr 

.global __dt__Q26JDrama8TEfbCtrlFv
__dt__Q26JDrama8TEfbCtrlFv:
/* 80042468 0003F3A8  7C 08 02 A6 */	mflr r0
/* 8004246C 0003F3AC  90 01 00 04 */	stw r0, 4(r1)
/* 80042470 0003F3B0  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 80042474 0003F3B4  93 E1 00 14 */	stw r31, 0x14(r1)
/* 80042478 0003F3B8  3B E4 00 00 */	addi r31, r4, 0
/* 8004247C 0003F3BC  93 C1 00 10 */	stw r30, 0x10(r1)
/* 80042480 0003F3C0  7C 7E 1B 79 */	or. r30, r3, r3
/* 80042484 0003F3C4  41 82 00 3C */	beq lbl_800424C0
/* 80042488 0003F3C8  3C 60 80 3B */	lis r3, __vt__Q26JDrama8TEfbCtrl@ha
/* 8004248C 0003F3CC  38 03 A6 08 */	addi r0, r3, __vt__Q26JDrama8TEfbCtrl@l
/* 80042490 0003F3D0  90 1E 00 00 */	stw r0, 0(r30)
/* 80042494 0003F3D4  41 82 00 1C */	beq lbl_800424B0
/* 80042498 0003F3D8  3C 60 80 3B */	lis r3, __vt__Q26JDrama8TViewObj@ha
/* 8004249C 0003F3DC  38 03 A0 C0 */	addi r0, r3, __vt__Q26JDrama8TViewObj@l
/* 800424A0 0003F3E0  90 1E 00 00 */	stw r0, 0(r30)
/* 800424A4 0003F3E4  38 7E 00 00 */	addi r3, r30, 0
/* 800424A8 0003F3E8  38 80 00 00 */	li r4, 0
/* 800424AC 0003F3EC  48 00 23 51 */	bl __dt__Q26JDrama8TNameRefFv
lbl_800424B0:
/* 800424B0 0003F3F0  7F E0 07 35 */	extsh. r0, r31
/* 800424B4 0003F3F4  40 81 00 0C */	ble lbl_800424C0
/* 800424B8 0003F3F8  7F C3 F3 78 */	mr r3, r30
/* 800424BC 0003F3FC  4B FC A5 F5 */	bl __dl__FPv
lbl_800424C0:
/* 800424C0 0003F400  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 800424C4 0003F404  7F C3 F3 78 */	mr r3, r30
/* 800424C8 0003F408  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 800424CC 0003F40C  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 800424D0 0003F410  7C 08 03 A6 */	mtlr r0
/* 800424D4 0003F414  38 21 00 18 */	addi r1, r1, 0x18
/* 800424D8 0003F418  4E 80 00 20 */	blr 

.global __dt__Q26JDrama14TViewConnecterFv
__dt__Q26JDrama14TViewConnecterFv:
/* 800424DC 0003F41C  7C 08 02 A6 */	mflr r0
/* 800424E0 0003F420  90 01 00 04 */	stw r0, 4(r1)
/* 800424E4 0003F424  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 800424E8 0003F428  93 E1 00 14 */	stw r31, 0x14(r1)
/* 800424EC 0003F42C  3B E4 00 00 */	addi r31, r4, 0
/* 800424F0 0003F430  93 C1 00 10 */	stw r30, 0x10(r1)
/* 800424F4 0003F434  7C 7E 1B 79 */	or. r30, r3, r3
/* 800424F8 0003F438  41 82 00 3C */	beq lbl_80042534
/* 800424FC 0003F43C  3C 60 80 3B */	lis r3, __vt__Q26JDrama14TViewConnecter@ha
/* 80042500 0003F440  38 03 AA 00 */	addi r0, r3, __vt__Q26JDrama14TViewConnecter@l
/* 80042504 0003F444  90 1E 00 00 */	stw r0, 0(r30)
/* 80042508 0003F448  41 82 00 1C */	beq lbl_80042524
/* 8004250C 0003F44C  3C 60 80 3B */	lis r3, __vt__Q26JDrama8TViewObj@ha
/* 80042510 0003F450  38 03 A0 C0 */	addi r0, r3, __vt__Q26JDrama8TViewObj@l
/* 80042514 0003F454  90 1E 00 00 */	stw r0, 0(r30)
/* 80042518 0003F458  38 7E 00 00 */	addi r3, r30, 0
/* 8004251C 0003F45C  38 80 00 00 */	li r4, 0
/* 80042520 0003F460  48 00 22 DD */	bl __dt__Q26JDrama8TNameRefFv
lbl_80042524:
/* 80042524 0003F464  7F E0 07 35 */	extsh. r0, r31
/* 80042528 0003F468  40 81 00 0C */	ble lbl_80042534
/* 8004252C 0003F46C  7F C3 F3 78 */	mr r3, r30
/* 80042530 0003F470  4B FC A5 81 */	bl __dl__FPv
lbl_80042534:
/* 80042534 0003F474  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 80042538 0003F478  7F C3 F3 78 */	mr r3, r30
/* 8004253C 0003F47C  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 80042540 0003F480  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 80042544 0003F484  7C 08 03 A6 */	mtlr r0
/* 80042548 0003F488  38 21 00 18 */	addi r1, r1, 0x18
/* 8004254C 0003F48C  4E 80 00 20 */	blr 

.global getEfbCtrlDisp__Q26JDrama11TDStageDispFv
getEfbCtrlDisp__Q26JDrama11TDStageDispFv:
/* 80042550 0003F490  80 63 00 10 */	lwz r3, 0x10(r3)
/* 80042554 0003F494  4E 80 00 20 */	blr 

.global "__dt__Q26JDrama55TViewObjPtrListT<Q26JDrama8TViewObj,Q26JDrama8TViewObj>Fv"
"__dt__Q26JDrama55TViewObjPtrListT<Q26JDrama8TViewObj,Q26JDrama8TViewObj>Fv":
/* 80042558 0003F498  7C 08 02 A6 */	mflr r0
/* 8004255C 0003F49C  90 01 00 04 */	stw r0, 4(r1)
/* 80042560 0003F4A0  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 80042564 0003F4A4  93 E1 00 14 */	stw r31, 0x14(r1)
/* 80042568 0003F4A8  3B E4 00 00 */	addi r31, r4, 0
/* 8004256C 0003F4AC  93 C1 00 10 */	stw r30, 0x10(r1)
/* 80042570 0003F4B0  7C 7E 1B 79 */	or. r30, r3, r3
/* 80042574 0003F4B4  41 82 00 54 */	beq lbl_800425C8
/* 80042578 0003F4B8  3C 60 80 3B */	lis r3, "__vt__Q26JDrama55TViewObjPtrListT<Q26JDrama8TViewObj,Q26JDrama8TViewObj>"@ha
/* 8004257C 0003F4BC  38 63 A5 40 */	addi r3, r3, "__vt__Q26JDrama55TViewObjPtrListT<Q26JDrama8TViewObj,Q26JDrama8TViewObj>"@l
/* 80042580 0003F4C0  34 1E 00 10 */	addic. r0, r30, 0x10
/* 80042584 0003F4C4  90 7E 00 00 */	stw r3, 0(r30)
/* 80042588 0003F4C8  41 82 00 10 */	beq lbl_80042598
/* 8004258C 0003F4CC  38 7E 00 10 */	addi r3, r30, 0x10
/* 80042590 0003F4D0  38 80 00 00 */	li r4, 0
/* 80042594 0003F4D4  4B FF 1C F5 */	bl __dt__Q27JGadget18TList_pointer_voidFv
lbl_80042598:
/* 80042598 0003F4D8  28 1E 00 00 */	cmplwi r30, 0
/* 8004259C 0003F4DC  41 82 00 1C */	beq lbl_800425B8
/* 800425A0 0003F4E0  3C 60 80 3B */	lis r3, __vt__Q26JDrama8TViewObj@ha
/* 800425A4 0003F4E4  38 03 A0 C0 */	addi r0, r3, __vt__Q26JDrama8TViewObj@l
/* 800425A8 0003F4E8  90 1E 00 00 */	stw r0, 0(r30)
/* 800425AC 0003F4EC  38 7E 00 00 */	addi r3, r30, 0
/* 800425B0 0003F4F0  38 80 00 00 */	li r4, 0
/* 800425B4 0003F4F4  48 00 22 49 */	bl __dt__Q26JDrama8TNameRefFv
lbl_800425B8:
/* 800425B8 0003F4F8  7F E0 07 35 */	extsh. r0, r31
/* 800425BC 0003F4FC  40 81 00 0C */	ble lbl_800425C8
/* 800425C0 0003F500  7F C3 F3 78 */	mr r3, r30
/* 800425C4 0003F504  4B FC A4 ED */	bl __dl__FPv
lbl_800425C8:
/* 800425C8 0003F508  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 800425CC 0003F50C  7F C3 F3 78 */	mr r3, r30
/* 800425D0 0003F510  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 800425D4 0003F514  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 800425D8 0003F518  7C 08 03 A6 */	mtlr r0
/* 800425DC 0003F51C  38 21 00 18 */	addi r1, r1, 0x18
/* 800425E0 0003F520  4E 80 00 20 */	blr 

.global __dt__Q26JDrama11TDStageDispFv
__dt__Q26JDrama11TDStageDispFv:
/* 800425E4 0003F524  7C 08 02 A6 */	mflr r0
/* 800425E8 0003F528  90 01 00 04 */	stw r0, 4(r1)
/* 800425EC 0003F52C  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 800425F0 0003F530  93 E1 00 14 */	stw r31, 0x14(r1)
/* 800425F4 0003F534  3B E4 00 00 */	addi r31, r4, 0
/* 800425F8 0003F538  93 C1 00 10 */	stw r30, 0x10(r1)
/* 800425FC 0003F53C  7C 7E 1B 79 */	or. r30, r3, r3
/* 80042600 0003F540  41 82 00 4C */	beq lbl_8004264C
/* 80042604 0003F544  3C 60 80 3B */	lis r3, __vt__Q26JDrama11TDStageDisp@ha
/* 80042608 0003F548  38 03 A5 6C */	addi r0, r3, __vt__Q26JDrama11TDStageDisp@l
/* 8004260C 0003F54C  90 1E 00 00 */	stw r0, 0(r30)
/* 80042610 0003F550  41 82 00 2C */	beq lbl_8004263C
/* 80042614 0003F554  3C 60 80 3B */	lis r3, __vt__Q26JDrama14TViewConnecter@ha
/* 80042618 0003F558  38 03 AA 00 */	addi r0, r3, __vt__Q26JDrama14TViewConnecter@l
/* 8004261C 0003F55C  90 1E 00 00 */	stw r0, 0(r30)
/* 80042620 0003F560  41 82 00 1C */	beq lbl_8004263C
/* 80042624 0003F564  3C 60 80 3B */	lis r3, __vt__Q26JDrama8TViewObj@ha
/* 80042628 0003F568  38 03 A0 C0 */	addi r0, r3, __vt__Q26JDrama8TViewObj@l
/* 8004262C 0003F56C  90 1E 00 00 */	stw r0, 0(r30)
/* 80042630 0003F570  38 7E 00 00 */	addi r3, r30, 0
/* 80042634 0003F574  38 80 00 00 */	li r4, 0
/* 80042638 0003F578  48 00 21 C5 */	bl __dt__Q26JDrama8TNameRefFv
lbl_8004263C:
/* 8004263C 0003F57C  7F E0 07 35 */	extsh. r0, r31
/* 80042640 0003F580  40 81 00 0C */	ble lbl_8004264C
/* 80042644 0003F584  7F C3 F3 78 */	mr r3, r30
/* 80042648 0003F588  4B FC A4 69 */	bl __dl__FPv
lbl_8004264C:
/* 8004264C 0003F58C  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 80042650 0003F590  7F C3 F3 78 */	mr r3, r30
/* 80042654 0003F594  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 80042658 0003F598  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 8004265C 0003F59C  7C 08 03 A6 */	mtlr r0
/* 80042660 0003F5A0  38 21 00 18 */	addi r1, r1, 0x18
/* 80042664 0003F5A4  4E 80 00 20 */	blr 

.section .rodata, "a"  # 0x8036FFA0 - 0x803A8380
.balign 8
"@881":
	.asciz "<EfbCtrlDisp>"
	.balign 4
"@882":
	.asciz "<TViewObjPtrListT>"
	.balign 4
	.4byte 0

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global "__vt__Q26JDrama55TViewObjPtrListT<Q26JDrama8TViewObj,Q26JDrama8TViewObj>"
"__vt__Q26JDrama55TViewObjPtrListT<Q26JDrama8TViewObj,Q26JDrama8TViewObj>":
  .4byte 0
  .4byte 0
  .4byte "__dt__Q26JDrama55TViewObjPtrListT<Q26JDrama8TViewObj,Q26JDrama8TViewObj>Fv"
  .4byte getType__Q26JDrama8TNameRefCFv
  .4byte "load__Q26JDrama55TViewObjPtrListT<Q26JDrama8TViewObj,Q26JDrama8TViewObj>FR20JSUMemoryInputStream"
  .4byte save__Q26JDrama8TNameRefFR21JSUMemoryOutputStream
  .4byte "loadAfter__Q26JDrama55TViewObjPtrListT<Q26JDrama8TViewObj,Q26JDrama8TViewObj>Fv"
  .4byte "searchF__Q26JDrama55TViewObjPtrListT<Q26JDrama8TViewObj,Q26JDrama8TViewObj>FUsPCc"
  .4byte "perform__Q26JDrama55TViewObjPtrListT<Q26JDrama8TViewObj,Q26JDrama8TViewObj>FUlPQ26JDrama9TGraphics"
  .4byte "loadSuper__Q26JDrama55TViewObjPtrListT<Q26JDrama8TViewObj,Q26JDrama8TViewObj>FR20JSUMemoryInputStream"
  .4byte "loadAfterSuper__Q26JDrama55TViewObjPtrListT<Q26JDrama8TViewObj,Q26JDrama8TViewObj>Fv"
.global __vt__Q26JDrama11TDStageDisp
__vt__Q26JDrama11TDStageDisp:
  .4byte 0
  .4byte 0
  .4byte __dt__Q26JDrama11TDStageDispFv
  .4byte getType__Q26JDrama8TNameRefCFv
  .4byte load__Q26JDrama8TNameRefFR20JSUMemoryInputStream
  .4byte save__Q26JDrama8TNameRefFR21JSUMemoryOutputStream
  .4byte loadAfter__Q26JDrama8TNameRefFv
  .4byte searchF__Q26JDrama8TNameRefFUsPCc
  .4byte perform__Q26JDrama14TViewConnecterFUlPQ26JDrama9TGraphics
