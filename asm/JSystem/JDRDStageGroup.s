.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global perform__Q26JDrama12TDStageGroupFUlPQ26JDrama9TGraphics
perform__Q26JDrama12TDStageGroupFUlPQ26JDrama9TGraphics:
/* 80042668 0003F5A8  7C 08 02 A6 */	mflr r0
/* 8004266C 0003F5AC  90 01 00 04 */	stw r0, 4(r1)
/* 80042670 0003F5B0  54 80 07 39 */	rlwinm. r0, r4, 0, 0x1c, 0x1c
/* 80042674 0003F5B4  94 21 FF D8 */	stwu r1, -0x28(r1)
/* 80042678 0003F5B8  93 E1 00 24 */	stw r31, 0x24(r1)
/* 8004267C 0003F5BC  3B E5 00 00 */	addi r31, r5, 0
/* 80042680 0003F5C0  93 C1 00 20 */	stw r30, 0x20(r1)
/* 80042684 0003F5C4  3B C4 00 00 */	addi r30, r4, 0
/* 80042688 0003F5C8  93 A1 00 1C */	stw r29, 0x1c(r1)
/* 8004268C 0003F5CC  3B A3 00 00 */	addi r29, r3, 0
/* 80042690 0003F5D0  41 82 00 50 */	beq lbl_800426E0
/* 80042694 0003F5D4  38 7D 00 20 */	addi r3, r29, 0x20
/* 80042698 0003F5D8  81 9D 00 20 */	lwz r12, 0x20(r29)
/* 8004269C 0003F5DC  38 9E 00 00 */	addi r4, r30, 0
/* 800426A0 0003F5E0  38 BF 00 00 */	addi r5, r31, 0
/* 800426A4 0003F5E4  81 8C 00 20 */	lwz r12, 0x20(r12)
/* 800426A8 0003F5E8  7D 88 03 A6 */	mtlr r12
/* 800426AC 0003F5EC  4E 80 00 21 */	blrl 
/* 800426B0 0003F5F0  38 7D 00 00 */	addi r3, r29, 0
/* 800426B4 0003F5F4  38 9E 00 00 */	addi r4, r30, 0
/* 800426B8 0003F5F8  38 BF 00 00 */	addi r5, r31, 0
/* 800426BC 0003F5FC  48 00 25 81 */	bl "perform__Q26JDrama55TViewObjPtrListT<Q26JDrama8TViewObj,Q26JDrama8TViewObj>FUlPQ26JDrama9TGraphics"
/* 800426C0 0003F600  38 7D 00 20 */	addi r3, r29, 0x20
/* 800426C4 0003F604  81 9D 00 20 */	lwz r12, 0x20(r29)
/* 800426C8 0003F608  57 C0 07 76 */	rlwinm r0, r30, 0, 0x1d, 0x1b
/* 800426CC 0003F60C  38 BF 00 00 */	addi r5, r31, 0
/* 800426D0 0003F610  81 8C 00 20 */	lwz r12, 0x20(r12)
/* 800426D4 0003F614  60 04 01 00 */	ori r4, r0, 0x100
/* 800426D8 0003F618  7D 88 03 A6 */	mtlr r12
/* 800426DC 0003F61C  4E 80 00 21 */	blrl 
lbl_800426E0:
/* 800426E0 0003F620  80 01 00 2C */	lwz r0, 0x2c(r1)
/* 800426E4 0003F624  83 E1 00 24 */	lwz r31, 0x24(r1)
/* 800426E8 0003F628  83 C1 00 20 */	lwz r30, 0x20(r1)
/* 800426EC 0003F62C  7C 08 03 A6 */	mtlr r0
/* 800426F0 0003F630  83 A1 00 1C */	lwz r29, 0x1c(r1)
/* 800426F4 0003F634  38 21 00 28 */	addi r1, r1, 0x28
/* 800426F8 0003F638  4E 80 00 20 */	blr 

.global __dt__Q26JDrama12TDStageGroupFv
__dt__Q26JDrama12TDStageGroupFv:
/* 800426FC 0003F63C  7C 08 02 A6 */	mflr r0
/* 80042700 0003F640  90 01 00 04 */	stw r0, 4(r1)
/* 80042704 0003F644  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 80042708 0003F648  93 E1 00 14 */	stw r31, 0x14(r1)
/* 8004270C 0003F64C  3B E4 00 00 */	addi r31, r4, 0
/* 80042710 0003F650  93 C1 00 10 */	stw r30, 0x10(r1)
/* 80042714 0003F654  7C 7E 1B 79 */	or. r30, r3, r3
/* 80042718 0003F658  41 82 00 9C */	beq lbl_800427B4
/* 8004271C 0003F65C  3C 60 80 3B */	lis r3, __vt__Q26JDrama12TDStageGroup@ha
/* 80042720 0003F660  38 63 A5 90 */	addi r3, r3, __vt__Q26JDrama12TDStageGroup@l
/* 80042724 0003F664  34 1E 00 20 */	addic. r0, r30, 0x20
/* 80042728 0003F668  90 7E 00 00 */	stw r3, 0(r30)
/* 8004272C 0003F66C  41 82 00 30 */	beq lbl_8004275C
/* 80042730 0003F670  3C 60 80 3B */	lis r3, __vt__Q26JDrama9TFrmGXSet@ha
/* 80042734 0003F674  38 63 A6 30 */	addi r3, r3, __vt__Q26JDrama9TFrmGXSet@l
/* 80042738 0003F678  34 1E 00 20 */	addic. r0, r30, 0x20
/* 8004273C 0003F67C  90 7E 00 20 */	stw r3, 0x20(r30)
/* 80042740 0003F680  41 82 00 1C */	beq lbl_8004275C
/* 80042744 0003F684  3C 60 80 3B */	lis r3, __vt__Q26JDrama8TViewObj@ha
/* 80042748 0003F688  38 03 A0 C0 */	addi r0, r3, __vt__Q26JDrama8TViewObj@l
/* 8004274C 0003F68C  90 1E 00 20 */	stw r0, 0x20(r30)
/* 80042750 0003F690  38 7E 00 20 */	addi r3, r30, 0x20
/* 80042754 0003F694  38 80 00 00 */	li r4, 0
/* 80042758 0003F698  48 00 20 A5 */	bl __dt__Q26JDrama8TNameRefFv
lbl_8004275C:
/* 8004275C 0003F69C  28 1E 00 00 */	cmplwi r30, 0
/* 80042760 0003F6A0  41 82 00 44 */	beq lbl_800427A4
/* 80042764 0003F6A4  3C 60 80 3B */	lis r3, "__vt__Q26JDrama55TViewObjPtrListT<Q26JDrama8TViewObj,Q26JDrama8TViewObj>"@ha
/* 80042768 0003F6A8  38 63 A5 40 */	addi r3, r3, "__vt__Q26JDrama55TViewObjPtrListT<Q26JDrama8TViewObj,Q26JDrama8TViewObj>"@l
/* 8004276C 0003F6AC  34 1E 00 10 */	addic. r0, r30, 0x10
/* 80042770 0003F6B0  90 7E 00 00 */	stw r3, 0(r30)
/* 80042774 0003F6B4  41 82 00 10 */	beq lbl_80042784
/* 80042778 0003F6B8  38 7E 00 10 */	addi r3, r30, 0x10
/* 8004277C 0003F6BC  38 80 00 00 */	li r4, 0
/* 80042780 0003F6C0  4B FF 1B 09 */	bl __dt__Q27JGadget18TList_pointer_voidFv
lbl_80042784:
/* 80042784 0003F6C4  28 1E 00 00 */	cmplwi r30, 0
/* 80042788 0003F6C8  41 82 00 1C */	beq lbl_800427A4
/* 8004278C 0003F6CC  3C 60 80 3B */	lis r3, __vt__Q26JDrama8TViewObj@ha
/* 80042790 0003F6D0  38 03 A0 C0 */	addi r0, r3, __vt__Q26JDrama8TViewObj@l
/* 80042794 0003F6D4  90 1E 00 00 */	stw r0, 0(r30)
/* 80042798 0003F6D8  38 7E 00 00 */	addi r3, r30, 0
/* 8004279C 0003F6DC  38 80 00 00 */	li r4, 0
/* 800427A0 0003F6E0  48 00 20 5D */	bl __dt__Q26JDrama8TNameRefFv
lbl_800427A4:
/* 800427A4 0003F6E4  7F E0 07 35 */	extsh. r0, r31
/* 800427A8 0003F6E8  40 81 00 0C */	ble lbl_800427B4
/* 800427AC 0003F6EC  7F C3 F3 78 */	mr r3, r30
/* 800427B0 0003F6F0  4B FC A3 01 */	bl __dl__FPv
lbl_800427B4:
/* 800427B4 0003F6F4  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 800427B8 0003F6F8  7F C3 F3 78 */	mr r3, r30
/* 800427BC 0003F6FC  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 800427C0 0003F700  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 800427C4 0003F704  7C 08 03 A6 */	mtlr r0
/* 800427C8 0003F708  38 21 00 18 */	addi r1, r1, 0x18
/* 800427CC 0003F70C  4E 80 00 20 */	blr 

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global __vt__Q26JDrama12TDStageGroup
__vt__Q26JDrama12TDStageGroup:
  .4byte 0
  .4byte 0
  .4byte __dt__Q26JDrama12TDStageGroupFv
  .4byte getType__Q26JDrama8TNameRefCFv
  .4byte "load__Q26JDrama55TViewObjPtrListT<Q26JDrama8TViewObj,Q26JDrama8TViewObj>FR20JSUMemoryInputStream"
  .4byte save__Q26JDrama8TNameRefFR21JSUMemoryOutputStream
  .4byte "loadAfter__Q26JDrama55TViewObjPtrListT<Q26JDrama8TViewObj,Q26JDrama8TViewObj>Fv"
  .4byte "searchF__Q26JDrama55TViewObjPtrListT<Q26JDrama8TViewObj,Q26JDrama8TViewObj>FUsPCc"
  .4byte perform__Q26JDrama12TDStageGroupFUlPQ26JDrama9TGraphics
  .4byte "loadSuper__Q26JDrama55TViewObjPtrListT<Q26JDrama8TViewObj,Q26JDrama8TViewObj>FR20JSUMemoryInputStream"
  .4byte "loadAfterSuper__Q26JDrama55TViewObjPtrListT<Q26JDrama8TViewObj,Q26JDrama8TViewObj>Fv"
  .4byte 0
