.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global __dt__18TJointModelManagerFv
__dt__18TJointModelManagerFv:
/* 801DA380 001D72C0  7C 08 02 A6 */	mflr r0
/* 801DA384 001D72C4  90 01 00 04 */	stw r0, 4(r1)
/* 801DA388 001D72C8  94 21 FF C0 */	stwu r1, -0x40(r1)
/* 801DA38C 001D72CC  93 E1 00 3C */	stw r31, 0x3c(r1)
/* 801DA390 001D72D0  93 C1 00 38 */	stw r30, 0x38(r1)
/* 801DA394 001D72D4  93 A1 00 34 */	stw r29, 0x34(r1)
/* 801DA398 001D72D8  3B A4 00 00 */	addi r29, r4, 0
/* 801DA39C 001D72DC  93 81 00 30 */	stw r28, 0x30(r1)
/* 801DA3A0 001D72E0  7C 7C 1B 79 */	or. r28, r3, r3
/* 801DA3A4 001D72E4  41 82 00 94 */	beq lbl_801DA438
/* 801DA3A8 001D72E8  3C 60 80 3D */	lis r3, __vt__18TJointModelManager@ha
/* 801DA3AC 001D72EC  38 63 FB B8 */	addi r3, r3, __vt__18TJointModelManager@l
/* 801DA3B0 001D72F0  34 1C 00 20 */	addic. r0, r28, 0x20
/* 801DA3B4 001D72F4  90 7C 00 00 */	stw r3, 0(r28)
/* 801DA3B8 001D72F8  41 82 00 50 */	beq lbl_801DA408
/* 801DA3BC 001D72FC  34 1C 00 3C */	addic. r0, r28, 0x3c
/* 801DA3C0 001D7300  41 82 00 48 */	beq lbl_801DA408
/* 801DA3C4 001D7304  3B DC 00 3C */	addi r30, r28, 0x3c
/* 801DA3C8 001D7308  38 61 00 28 */	addi r3, r1, 0x28
/* 801DA3CC 001D730C  38 9E 00 08 */	addi r4, r30, 8
/* 801DA3D0 001D7310  48 00 01 35 */	bl "__ct__Q37JGadget68TList<16MActorSubAnmInfo,Q27JGadget30TAllocator<16MActorSubAnmInfo>>8iteratorFPQ37JGadget68TList<16MActorSubAnmInfo,Q27JGadget30TAllocator<16MActorSubAnmInfo>>6TNode_"
/* 801DA3D4 001D7314  80 01 00 28 */	lwz r0, 0x28(r1)
/* 801DA3D8 001D7318  3B E1 00 24 */	addi r31, r1, 0x24
/* 801DA3DC 001D731C  38 61 00 20 */	addi r3, r1, 0x20
/* 801DA3E0 001D7320  90 01 00 24 */	stw r0, 0x24(r1)
/* 801DA3E4 001D7324  80 9C 00 44 */	lwz r4, 0x44(r28)
/* 801DA3E8 001D7328  48 00 01 1D */	bl "__ct__Q37JGadget68TList<16MActorSubAnmInfo,Q27JGadget30TAllocator<16MActorSubAnmInfo>>8iteratorFPQ37JGadget68TList<16MActorSubAnmInfo,Q27JGadget30TAllocator<16MActorSubAnmInfo>>6TNode_"
/* 801DA3EC 001D732C  80 01 00 20 */	lwz r0, 0x20(r1)
/* 801DA3F0 001D7330  38 A1 00 1C */	addi r5, r1, 0x1c
/* 801DA3F4 001D7334  38 9E 00 00 */	addi r4, r30, 0
/* 801DA3F8 001D7338  90 01 00 1C */	stw r0, 0x1c(r1)
/* 801DA3FC 001D733C  38 DF 00 00 */	addi r6, r31, 0
/* 801DA400 001D7340  38 61 00 18 */	addi r3, r1, 0x18
/* 801DA404 001D7344  48 00 00 59 */	bl "erase__Q27JGadget68TList<16MActorSubAnmInfo,Q27JGadget30TAllocator<16MActorSubAnmInfo>>FQ37JGadget68TList<16MActorSubAnmInfo,Q27JGadget30TAllocator<16MActorSubAnmInfo>>8iteratorQ37JGadget68TList<16MActorSubAnmInfo,Q27JGadget30TAllocator<16MActorSubAnmInfo>>8iterator"
lbl_801DA408:
/* 801DA408 001D7348  28 1C 00 00 */	cmplwi r28, 0
/* 801DA40C 001D734C  41 82 00 1C */	beq lbl_801DA428
/* 801DA410 001D7350  3C 60 80 3B */	lis r3, __vt__Q26JDrama8TViewObj@ha
/* 801DA414 001D7354  38 03 A0 C0 */	addi r0, r3, __vt__Q26JDrama8TViewObj@l
/* 801DA418 001D7358  90 1C 00 00 */	stw r0, 0(r28)
/* 801DA41C 001D735C  38 7C 00 00 */	addi r3, r28, 0
/* 801DA420 001D7360  38 80 00 00 */	li r4, 0
/* 801DA424 001D7364  4B E6 A3 D9 */	bl __dt__Q26JDrama8TNameRefFv
lbl_801DA428:
/* 801DA428 001D7368  7F A0 07 35 */	extsh. r0, r29
/* 801DA42C 001D736C  40 81 00 0C */	ble lbl_801DA438
/* 801DA430 001D7370  7F 83 E3 78 */	mr r3, r28
/* 801DA434 001D7374  4B E3 26 7D */	bl __dl__FPv
lbl_801DA438:
/* 801DA438 001D7378  80 01 00 44 */	lwz r0, 0x44(r1)
/* 801DA43C 001D737C  7F 83 E3 78 */	mr r3, r28
/* 801DA440 001D7380  83 E1 00 3C */	lwz r31, 0x3c(r1)
/* 801DA444 001D7384  83 C1 00 38 */	lwz r30, 0x38(r1)
/* 801DA448 001D7388  7C 08 03 A6 */	mtlr r0
/* 801DA44C 001D738C  83 A1 00 34 */	lwz r29, 0x34(r1)
/* 801DA450 001D7390  83 81 00 30 */	lwz r28, 0x30(r1)
/* 801DA454 001D7394  38 21 00 40 */	addi r1, r1, 0x40
/* 801DA458 001D7398  4E 80 00 20 */	blr 

.global "erase__Q27JGadget68TList<16MActorSubAnmInfo,Q27JGadget30TAllocator<16MActorSubAnmInfo>>FQ37JGadget68TList<16MActorSubAnmInfo,Q27JGadget30TAllocator<16MActorSubAnmInfo>>8iteratorQ37JGadget68TList<16MActorSubAnmInfo,Q27JGadget30TAllocator<16MActorSubAnmInfo>>8iterator"
"erase__Q27JGadget68TList<16MActorSubAnmInfo,Q27JGadget30TAllocator<16MActorSubAnmInfo>>FQ37JGadget68TList<16MActorSubAnmInfo,Q27JGadget30TAllocator<16MActorSubAnmInfo>>8iteratorQ37JGadget68TList<16MActorSubAnmInfo,Q27JGadget30TAllocator<16MActorSubAnmInfo>>8iterator":
/* 801DA45C 001D739C  7C 08 02 A6 */	mflr r0
/* 801DA460 001D73A0  90 01 00 04 */	stw r0, 4(r1)
/* 801DA464 001D73A4  94 21 FF 98 */	stwu r1, -0x68(r1)
/* 801DA468 001D73A8  BF 61 00 54 */	stmw r27, 0x54(r1)
/* 801DA46C 001D73AC  3B 83 00 00 */	addi r28, r3, 0
/* 801DA470 001D73B0  3B A4 00 00 */	addi r29, r4, 0
/* 801DA474 001D73B4  3B C5 00 00 */	addi r30, r5, 0
/* 801DA478 001D73B8  3B E6 00 00 */	addi r31, r6, 0
/* 801DA47C 001D73BC  48 00 00 44 */	b lbl_801DA4C0
lbl_801DA480:
/* 801DA480 001D73C0  80 01 00 18 */	lwz r0, 0x18(r1)
/* 801DA484 001D73C4  90 01 00 44 */	stw r0, 0x44(r1)
/* 801DA488 001D73C8  80 61 00 44 */	lwz r3, 0x44(r1)
/* 801DA48C 001D73CC  83 63 00 00 */	lwz r27, 0(r3)
/* 801DA490 001D73D0  80 83 00 04 */	lwz r4, 4(r3)
/* 801DA494 001D73D4  93 64 00 00 */	stw r27, 0(r4)
/* 801DA498 001D73D8  80 81 00 44 */	lwz r4, 0x44(r1)
/* 801DA49C 001D73DC  80 04 00 04 */	lwz r0, 4(r4)
/* 801DA4A0 001D73E0  90 1B 00 04 */	stw r0, 4(r27)
/* 801DA4A4 001D73E4  4B E3 26 0D */	bl __dl__FPv
/* 801DA4A8 001D73E8  80 7D 00 04 */	lwz r3, 4(r29)
/* 801DA4AC 001D73EC  38 03 FF FF */	addi r0, r3, -1
/* 801DA4B0 001D73F0  90 1D 00 04 */	stw r0, 4(r29)
/* 801DA4B4 001D73F4  93 61 00 48 */	stw r27, 0x48(r1)
/* 801DA4B8 001D73F8  80 01 00 48 */	lwz r0, 0x48(r1)
/* 801DA4BC 001D73FC  90 1E 00 00 */	stw r0, 0(r30)
lbl_801DA4C0:
/* 801DA4C0 001D7400  80 1F 00 00 */	lwz r0, 0(r31)
/* 801DA4C4 001D7404  90 01 00 38 */	stw r0, 0x38(r1)
/* 801DA4C8 001D7408  80 7E 00 00 */	lwz r3, 0(r30)
/* 801DA4CC 001D740C  80 01 00 38 */	lwz r0, 0x38(r1)
/* 801DA4D0 001D7410  90 61 00 18 */	stw r3, 0x18(r1)
/* 801DA4D4 001D7414  80 61 00 18 */	lwz r3, 0x18(r1)
/* 801DA4D8 001D7418  90 61 00 34 */	stw r3, 0x34(r1)
/* 801DA4DC 001D741C  80 61 00 34 */	lwz r3, 0x34(r1)
/* 801DA4E0 001D7420  7C 03 00 40 */	cmplw r3, r0
/* 801DA4E4 001D7424  40 82 FF 9C */	bne lbl_801DA480
/* 801DA4E8 001D7428  80 1E 00 00 */	lwz r0, 0(r30)
/* 801DA4EC 001D742C  90 1C 00 00 */	stw r0, 0(r28)
/* 801DA4F0 001D7430  BB 61 00 54 */	lmw r27, 0x54(r1)
/* 801DA4F4 001D7434  80 01 00 6C */	lwz r0, 0x6c(r1)
/* 801DA4F8 001D7438  38 21 00 68 */	addi r1, r1, 0x68
/* 801DA4FC 001D743C  7C 08 03 A6 */	mtlr r0
/* 801DA500 001D7440  4E 80 00 20 */	blr 

.global "__ct__Q37JGadget68TList<16MActorSubAnmInfo,Q27JGadget30TAllocator<16MActorSubAnmInfo>>8iteratorFPQ37JGadget68TList<16MActorSubAnmInfo,Q27JGadget30TAllocator<16MActorSubAnmInfo>>6TNode_"
"__ct__Q37JGadget68TList<16MActorSubAnmInfo,Q27JGadget30TAllocator<16MActorSubAnmInfo>>8iteratorFPQ37JGadget68TList<16MActorSubAnmInfo,Q27JGadget30TAllocator<16MActorSubAnmInfo>>6TNode_":
/* 801DA504 001D7444  90 83 00 00 */	stw r4, 0(r3)
/* 801DA508 001D7448  4E 80 00 20 */	blr 

.global newJointModel__18TJointModelManagerCFi
newJointModel__18TJointModelManagerCFi:
/* 801DA50C 001D744C  7C 08 02 A6 */	mflr r0
/* 801DA510 001D7450  38 60 00 30 */	li r3, 0x30
/* 801DA514 001D7454  90 01 00 04 */	stw r0, 4(r1)
/* 801DA518 001D7458  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 801DA51C 001D745C  93 E1 00 14 */	stw r31, 0x14(r1)
/* 801DA520 001D7460  4B E3 23 91 */	bl __nw__FUl
/* 801DA524 001D7464  7C 7F 1B 79 */	or. r31, r3, r3
/* 801DA528 001D7468  41 82 00 0C */	beq lbl_801DA534
/* 801DA52C 001D746C  7F E3 FB 78 */	mr r3, r31
/* 801DA530 001D7470  4B FF F9 31 */	bl __ct__11TJointModelFv
lbl_801DA534:
/* 801DA534 001D7474  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 801DA538 001D7478  7F E3 FB 78 */	mr r3, r31
/* 801DA53C 001D747C  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 801DA540 001D7480  38 21 00 18 */	addi r1, r1, 0x18
/* 801DA544 001D7484  7C 08 03 A6 */	mtlr r0
/* 801DA548 001D7488  4E 80 00 20 */	blr 

.global __ct__18TJointModelManagerFPCc
__ct__18TJointModelManagerFPCc:
/* 801DA54C 001D748C  7C 08 02 A6 */	mflr r0
/* 801DA550 001D7490  90 01 00 04 */	stw r0, 4(r1)
/* 801DA554 001D7494  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 801DA558 001D7498  93 E1 00 14 */	stw r31, 0x14(r1)
/* 801DA55C 001D749C  90 61 00 08 */	stw r3, 8(r1)
/* 801DA560 001D74A0  3C 60 80 3B */	lis r3, __vt__Q26JDrama8TNameRef@ha
/* 801DA564 001D74A4  38 03 A8 60 */	addi r0, r3, __vt__Q26JDrama8TNameRef@l
/* 801DA568 001D74A8  83 E1 00 08 */	lwz r31, 8(r1)
/* 801DA56C 001D74AC  38 64 00 00 */	addi r3, r4, 0
/* 801DA570 001D74B0  90 1F 00 00 */	stw r0, 0(r31)
/* 801DA574 001D74B4  90 9F 00 04 */	stw r4, 4(r31)
/* 801DA578 001D74B8  4B E6 A1 15 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 801DA57C 001D74BC  B0 7F 00 08 */	sth r3, 8(r31)
/* 801DA580 001D74C0  3C 60 80 3B */	lis r3, __vt__Q26JDrama8TViewObj@ha
/* 801DA584 001D74C4  38 03 A0 C0 */	addi r0, r3, __vt__Q26JDrama8TViewObj@l
/* 801DA588 001D74C8  90 1F 00 00 */	stw r0, 0(r31)
/* 801DA58C 001D74CC  38 80 00 00 */	li r4, 0
/* 801DA590 001D74D0  3C 60 80 3D */	lis r3, __vt__18TJointModelManager@ha
/* 801DA594 001D74D4  B0 9F 00 0C */	sth r4, 0xc(r31)
/* 801DA598 001D74D8  38 03 FB B8 */	addi r0, r3, __vt__18TJointModelManager@l
/* 801DA59C 001D74DC  38 7F 00 20 */	addi r3, r31, 0x20
/* 801DA5A0 001D74E0  90 1F 00 00 */	stw r0, 0(r31)
/* 801DA5A4 001D74E4  90 9F 00 10 */	stw r4, 0x10(r31)
/* 801DA5A8 001D74E8  90 9F 00 14 */	stw r4, 0x14(r31)
/* 801DA5AC 001D74EC  90 9F 00 18 */	stw r4, 0x18(r31)
/* 801DA5B0 001D74F0  90 9F 00 1C */	stw r4, 0x1c(r31)
/* 801DA5B4 001D74F4  4B EF D8 51 */	bl __ct__13MActorAnmDataFv
/* 801DA5B8 001D74F8  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 801DA5BC 001D74FC  7F E3 FB 78 */	mr r3, r31
/* 801DA5C0 001D7500  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 801DA5C4 001D7504  38 21 00 18 */	addi r1, r1, 0x18
/* 801DA5C8 001D7508  7C 08 03 A6 */	mtlr r0
/* 801DA5CC 001D750C  4E 80 00 20 */	blr 

.global initJointModel__18TJointModelManagerFPCcPPCc
initJointModel__18TJointModelManagerFPCcPPCc:
/* 801DA5D0 001D7510  7C 08 02 A6 */	mflr r0
/* 801DA5D4 001D7514  90 01 00 04 */	stw r0, 4(r1)
/* 801DA5D8 001D7518  94 21 FF D0 */	stwu r1, -0x30(r1)
/* 801DA5DC 001D751C  93 E1 00 2C */	stw r31, 0x2c(r1)
/* 801DA5E0 001D7520  93 C1 00 28 */	stw r30, 0x28(r1)
/* 801DA5E4 001D7524  93 A1 00 24 */	stw r29, 0x24(r1)
/* 801DA5E8 001D7528  3B A5 00 00 */	addi r29, r5, 0
/* 801DA5EC 001D752C  93 81 00 20 */	stw r28, 0x20(r1)
/* 801DA5F0 001D7530  7C 7C 1B 78 */	mr r28, r3
/* 801DA5F4 001D7534  80 03 00 10 */	lwz r0, 0x10(r3)
/* 801DA5F8 001D7538  2C 00 00 00 */	cmpwi r0, 0
/* 801DA5FC 001D753C  41 82 00 9C */	beq lbl_801DA698
/* 801DA600 001D7540  90 9C 00 1C */	stw r4, 0x1c(r28)
/* 801DA604 001D7544  38 7C 00 20 */	addi r3, r28, 0x20
/* 801DA608 001D7548  38 A0 00 00 */	li r5, 0
/* 801DA60C 001D754C  80 9C 00 1C */	lwz r4, 0x1c(r28)
/* 801DA610 001D7550  4B EF D1 4D */	bl init__13MActorAnmDataFPCcPPCc
/* 801DA614 001D7554  80 1C 00 10 */	lwz r0, 0x10(r28)
/* 801DA618 001D7558  54 03 10 3A */	slwi r3, r0, 2
/* 801DA61C 001D755C  4B E3 23 95 */	bl __nwa__FUl
/* 801DA620 001D7560  90 7C 00 14 */	stw r3, 0x14(r28)
/* 801DA624 001D7564  3B C0 00 00 */	li r30, 0
/* 801DA628 001D7568  3B E0 00 00 */	li r31, 0
/* 801DA62C 001D756C  48 00 00 60 */	b lbl_801DA68C
lbl_801DA630:
/* 801DA630 001D7570  7F 83 E3 78 */	mr r3, r28
/* 801DA634 001D7574  81 9C 00 00 */	lwz r12, 0(r28)
/* 801DA638 001D7578  7F C4 F3 78 */	mr r4, r30
/* 801DA63C 001D757C  81 8C 00 24 */	lwz r12, 0x24(r12)
/* 801DA640 001D7580  7D 88 03 A6 */	mtlr r12
/* 801DA644 001D7584  4E 80 00 21 */	blrl 
/* 801DA648 001D7588  80 BC 00 14 */	lwz r5, 0x14(r28)
/* 801DA64C 001D758C  57 C0 04 3E */	clrlwi r0, r30, 0x10
/* 801DA650 001D7590  38 9C 00 00 */	addi r4, r28, 0
/* 801DA654 001D7594  7C 65 F9 2E */	stwx r3, r5, r31
/* 801DA658 001D7598  38 DC 00 20 */	addi r6, r28, 0x20
/* 801DA65C 001D759C  80 7C 00 14 */	lwz r3, 0x14(r28)
/* 801DA660 001D75A0  7C 63 F8 2E */	lwzx r3, r3, r31
/* 801DA664 001D75A4  90 03 00 04 */	stw r0, 4(r3)
/* 801DA668 001D75A8  80 7C 00 14 */	lwz r3, 0x14(r28)
/* 801DA66C 001D75AC  7C BD F8 2E */	lwzx r5, r29, r31
/* 801DA670 001D75B0  7C 63 F8 2E */	lwzx r3, r3, r31
/* 801DA674 001D75B4  81 83 00 00 */	lwz r12, 0(r3)
/* 801DA678 001D75B8  81 8C 00 28 */	lwz r12, 0x28(r12)
/* 801DA67C 001D75BC  7D 88 03 A6 */	mtlr r12
/* 801DA680 001D75C0  4E 80 00 21 */	blrl 
/* 801DA684 001D75C4  3B DE 00 01 */	addi r30, r30, 1
/* 801DA688 001D75C8  3B FF 00 04 */	addi r31, r31, 4
lbl_801DA68C:
/* 801DA68C 001D75CC  80 1C 00 10 */	lwz r0, 0x10(r28)
/* 801DA690 001D75D0  7C 1E 00 00 */	cmpw r30, r0
/* 801DA694 001D75D4  41 80 FF 9C */	blt lbl_801DA630
lbl_801DA698:
/* 801DA698 001D75D8  80 01 00 34 */	lwz r0, 0x34(r1)
/* 801DA69C 001D75DC  83 E1 00 2C */	lwz r31, 0x2c(r1)
/* 801DA6A0 001D75E0  83 C1 00 28 */	lwz r30, 0x28(r1)
/* 801DA6A4 001D75E4  7C 08 03 A6 */	mtlr r0
/* 801DA6A8 001D75E8  83 A1 00 24 */	lwz r29, 0x24(r1)
/* 801DA6AC 001D75EC  83 81 00 20 */	lwz r28, 0x20(r1)
/* 801DA6B0 001D75F0  38 21 00 30 */	addi r1, r1, 0x30
/* 801DA6B4 001D75F4  4E 80 00 20 */	blr 

.global perform__18TJointModelManagerFUlPQ26JDrama9TGraphics
perform__18TJointModelManagerFUlPQ26JDrama9TGraphics:
/* 801DA6B8 001D75F8  7C 08 02 A6 */	mflr r0
/* 801DA6BC 001D75FC  90 01 00 04 */	stw r0, 4(r1)
/* 801DA6C0 001D7600  94 21 FF D0 */	stwu r1, -0x30(r1)
/* 801DA6C4 001D7604  BF 61 00 1C */	stmw r27, 0x1c(r1)
/* 801DA6C8 001D7608  3B 63 00 00 */	addi r27, r3, 0
/* 801DA6CC 001D760C  3B 84 00 00 */	addi r28, r4, 0
/* 801DA6D0 001D7610  3B A5 00 00 */	addi r29, r5, 0
/* 801DA6D4 001D7614  3B C0 00 00 */	li r30, 0
/* 801DA6D8 001D7618  3B E0 00 00 */	li r31, 0
/* 801DA6DC 001D761C  48 00 00 2C */	b lbl_801DA708
lbl_801DA6E0:
/* 801DA6E0 001D7620  80 7B 00 14 */	lwz r3, 0x14(r27)
/* 801DA6E4 001D7624  38 9C 00 00 */	addi r4, r28, 0
/* 801DA6E8 001D7628  38 BD 00 00 */	addi r5, r29, 0
/* 801DA6EC 001D762C  7C 63 F8 2E */	lwzx r3, r3, r31
/* 801DA6F0 001D7630  81 83 00 00 */	lwz r12, 0(r3)
/* 801DA6F4 001D7634  81 8C 00 2C */	lwz r12, 0x2c(r12)
/* 801DA6F8 001D7638  7D 88 03 A6 */	mtlr r12
/* 801DA6FC 001D763C  4E 80 00 21 */	blrl 
/* 801DA700 001D7640  3B DE 00 01 */	addi r30, r30, 1
/* 801DA704 001D7644  3B FF 00 04 */	addi r31, r31, 4
lbl_801DA708:
/* 801DA708 001D7648  80 1B 00 10 */	lwz r0, 0x10(r27)
/* 801DA70C 001D764C  7C 1E 00 00 */	cmpw r30, r0
/* 801DA710 001D7650  41 80 FF D0 */	blt lbl_801DA6E0
/* 801DA714 001D7654  BB 61 00 1C */	lmw r27, 0x1c(r1)
/* 801DA718 001D7658  80 01 00 34 */	lwz r0, 0x34(r1)
/* 801DA71C 001D765C  38 21 00 30 */	addi r1, r1, 0x30
/* 801DA720 001D7660  7C 08 03 A6 */	mtlr r0
/* 801DA724 001D7664  4E 80 00 20 */	blr 

.global __sinit_JointModelManager_cpp
__sinit_JointModelManager_cpp:
/* 801DA728 001D7668  7C 08 02 A6 */	mflr r0
/* 801DA72C 001D766C  3C 60 80 3F */	lis r3, "@2405"@ha
/* 801DA730 001D7670  90 01 00 04 */	stw r0, 4(r1)
/* 801DA734 001D7674  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 801DA738 001D7678  93 E1 00 0C */	stw r31, 0xc(r1)
/* 801DA73C 001D767C  3B E3 76 28 */	addi r31, r3, "@2405"@l
/* 801DA740 001D7680  88 0D 97 CC */	lbz r0, "__init__smList__15JALList<5MSBgm>"@sda21(r13)
/* 801DA744 001D7684  7C 00 07 75 */	extsb. r0, r0
/* 801DA748 001D7688  40 82 00 28 */	bne lbl_801DA770
/* 801DA74C 001D768C  38 6D 97 A8 */	addi r3, r13, "smList__15JALList<5MSBgm>"@sda21
/* 801DA750 001D7690  4B E3 44 E5 */	bl initiate__10JSUPtrListFv
/* 801DA754 001D7694  3C 60 80 0E */	lis r3, "__dt__15JSUList<5MSBgm>Fv"@ha
/* 801DA758 001D7698  38 83 6A 44 */	addi r4, r3, "__dt__15JSUList<5MSBgm>Fv"@l
/* 801DA75C 001D769C  38 6D 97 A8 */	addi r3, r13, "smList__15JALList<5MSBgm>"@sda21
/* 801DA760 001D76A0  38 BF 00 00 */	addi r5, r31, 0
/* 801DA764 001D76A4  4B EA 7F C5 */	bl __register_global_object
/* 801DA768 001D76A8  38 00 00 01 */	li r0, 1
/* 801DA76C 001D76AC  98 0D 97 CC */	stb r0, "__init__smList__15JALList<5MSBgm>"@sda21(r13)
lbl_801DA770:
/* 801DA770 001D76B0  88 0D 97 CD */	lbz r0, "__init__smList__24JALList<13MSSetSoundGrp>"@sda21(r13)
/* 801DA774 001D76B4  7C 00 07 75 */	extsb. r0, r0
/* 801DA778 001D76B8  40 82 00 28 */	bne lbl_801DA7A0
/* 801DA77C 001D76BC  38 6D 97 B4 */	addi r3, r13, "smList__24JALList<13MSSetSoundGrp>"@sda21
/* 801DA780 001D76C0  4B E3 44 B5 */	bl initiate__10JSUPtrListFv
/* 801DA784 001D76C4  3C 60 80 0E */	lis r3, "__dt__24JSUList<13MSSetSoundGrp>Fv"@ha
/* 801DA788 001D76C8  38 83 69 EC */	addi r4, r3, "__dt__24JSUList<13MSSetSoundGrp>Fv"@l
/* 801DA78C 001D76CC  38 6D 97 B4 */	addi r3, r13, "smList__24JALList<13MSSetSoundGrp>"@sda21
/* 801DA790 001D76D0  38 BF 00 0C */	addi r5, r31, 0xc
/* 801DA794 001D76D4  4B EA 7F 95 */	bl __register_global_object
/* 801DA798 001D76D8  38 00 00 01 */	li r0, 1
/* 801DA79C 001D76DC  98 0D 97 CD */	stb r0, "__init__smList__24JALList<13MSSetSoundGrp>"@sda21(r13)
lbl_801DA7A0:
/* 801DA7A0 001D76E0  88 0D 97 CE */	lbz r0, "__init__smList__21JALList<10MSSetSound>"@sda21(r13)
/* 801DA7A4 001D76E4  7C 00 07 75 */	extsb. r0, r0
/* 801DA7A8 001D76E8  40 82 00 28 */	bne lbl_801DA7D0
/* 801DA7AC 001D76EC  38 6D 97 C0 */	addi r3, r13, "smList__21JALList<10MSSetSound>"@sda21
/* 801DA7B0 001D76F0  4B E3 44 85 */	bl initiate__10JSUPtrListFv
/* 801DA7B4 001D76F4  3C 60 80 0E */	lis r3, "__dt__21JSUList<10MSSetSound>Fv"@ha
/* 801DA7B8 001D76F8  38 83 69 94 */	addi r4, r3, "__dt__21JSUList<10MSSetSound>Fv"@l
/* 801DA7BC 001D76FC  38 6D 97 C0 */	addi r3, r13, "smList__21JALList<10MSSetSound>"@sda21
/* 801DA7C0 001D7700  38 BF 00 18 */	addi r5, r31, 0x18
/* 801DA7C4 001D7704  4B EA 7F 65 */	bl __register_global_object
/* 801DA7C8 001D7708  38 00 00 01 */	li r0, 1
/* 801DA7CC 001D770C  98 0D 97 CE */	stb r0, "__init__smList__21JALList<10MSSetSound>"@sda21(r13)
lbl_801DA7D0:
/* 801DA7D0 001D7710  88 0D 8F 8C */	lbz r0, "__init__smList__26JALList<15JALSeModEffDGrp>"@sda21(r13)
/* 801DA7D4 001D7714  7C 00 07 75 */	extsb. r0, r0
/* 801DA7D8 001D7718  40 82 00 28 */	bne lbl_801DA800
/* 801DA7DC 001D771C  38 6D 8E FC */	addi r3, r13, "smList__26JALList<15JALSeModEffDGrp>"@sda21
/* 801DA7E0 001D7720  4B E3 44 55 */	bl initiate__10JSUPtrListFv
/* 801DA7E4 001D7724  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModEffDGrp>Fv"@ha
/* 801DA7E8 001D7728  38 83 A8 0C */	addi r4, r3, "__dt__26JSUList<15JALSeModEffDGrp>Fv"@l
/* 801DA7EC 001D772C  38 6D 8E FC */	addi r3, r13, "smList__26JALList<15JALSeModEffDGrp>"@sda21
/* 801DA7F0 001D7730  38 BF 00 24 */	addi r5, r31, 0x24
/* 801DA7F4 001D7734  4B EA 7F 35 */	bl __register_global_object
/* 801DA7F8 001D7738  38 00 00 01 */	li r0, 1
/* 801DA7FC 001D773C  98 0D 8F 8C */	stb r0, "__init__smList__26JALList<15JALSeModEffDGrp>"@sda21(r13)
lbl_801DA800:
/* 801DA800 001D7740  88 0D 8F 8D */	lbz r0, "__init__smList__26JALList<15JALSeModPitDGrp>"@sda21(r13)
/* 801DA804 001D7744  7C 00 07 75 */	extsb. r0, r0
/* 801DA808 001D7748  40 82 00 28 */	bne lbl_801DA830
/* 801DA80C 001D774C  38 6D 8F 08 */	addi r3, r13, "smList__26JALList<15JALSeModPitDGrp>"@sda21
/* 801DA810 001D7750  4B E3 44 25 */	bl initiate__10JSUPtrListFv
/* 801DA814 001D7754  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModPitDGrp>Fv"@ha
/* 801DA818 001D7758  38 83 A7 B4 */	addi r4, r3, "__dt__26JSUList<15JALSeModPitDGrp>Fv"@l
/* 801DA81C 001D775C  38 6D 8F 08 */	addi r3, r13, "smList__26JALList<15JALSeModPitDGrp>"@sda21
/* 801DA820 001D7760  38 BF 00 30 */	addi r5, r31, 0x30
/* 801DA824 001D7764  4B EA 7F 05 */	bl __register_global_object
/* 801DA828 001D7768  38 00 00 01 */	li r0, 1
/* 801DA82C 001D776C  98 0D 8F 8D */	stb r0, "__init__smList__26JALList<15JALSeModPitDGrp>"@sda21(r13)
lbl_801DA830:
/* 801DA830 001D7770  88 0D 8F 8E */	lbz r0, "__init__smList__26JALList<15JALSeModVolDGrp>"@sda21(r13)
/* 801DA834 001D7774  7C 00 07 75 */	extsb. r0, r0
/* 801DA838 001D7778  40 82 00 28 */	bne lbl_801DA860
/* 801DA83C 001D777C  38 6D 8F 14 */	addi r3, r13, "smList__26JALList<15JALSeModVolDGrp>"@sda21
/* 801DA840 001D7780  4B E3 43 F5 */	bl initiate__10JSUPtrListFv
/* 801DA844 001D7784  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModVolDGrp>Fv"@ha
/* 801DA848 001D7788  38 83 A7 5C */	addi r4, r3, "__dt__26JSUList<15JALSeModVolDGrp>Fv"@l
/* 801DA84C 001D778C  38 6D 8F 14 */	addi r3, r13, "smList__26JALList<15JALSeModVolDGrp>"@sda21
/* 801DA850 001D7790  38 BF 00 3C */	addi r5, r31, 0x3c
/* 801DA854 001D7794  4B EA 7E D5 */	bl __register_global_object
/* 801DA858 001D7798  38 00 00 01 */	li r0, 1
/* 801DA85C 001D779C  98 0D 8F 8E */	stb r0, "__init__smList__26JALList<15JALSeModVolDGrp>"@sda21(r13)
lbl_801DA860:
/* 801DA860 001D77A0  88 0D 8F 8F */	lbz r0, "__init__smList__26JALList<15JALSeModEffFGrp>"@sda21(r13)
/* 801DA864 001D77A4  7C 00 07 75 */	extsb. r0, r0
/* 801DA868 001D77A8  40 82 00 28 */	bne lbl_801DA890
/* 801DA86C 001D77AC  38 6D 8F 20 */	addi r3, r13, "smList__26JALList<15JALSeModEffFGrp>"@sda21
/* 801DA870 001D77B0  4B E3 43 C5 */	bl initiate__10JSUPtrListFv
/* 801DA874 001D77B4  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModEffFGrp>Fv"@ha
/* 801DA878 001D77B8  38 83 A7 04 */	addi r4, r3, "__dt__26JSUList<15JALSeModEffFGrp>Fv"@l
/* 801DA87C 001D77BC  38 6D 8F 20 */	addi r3, r13, "smList__26JALList<15JALSeModEffFGrp>"@sda21
/* 801DA880 001D77C0  38 BF 00 48 */	addi r5, r31, 0x48
/* 801DA884 001D77C4  4B EA 7E A5 */	bl __register_global_object
/* 801DA888 001D77C8  38 00 00 01 */	li r0, 1
/* 801DA88C 001D77CC  98 0D 8F 8F */	stb r0, "__init__smList__26JALList<15JALSeModEffFGrp>"@sda21(r13)
lbl_801DA890:
/* 801DA890 001D77D0  88 0D 8F 90 */	lbz r0, "__init__smList__26JALList<15JALSeModPitFGrp>"@sda21(r13)
/* 801DA894 001D77D4  7C 00 07 75 */	extsb. r0, r0
/* 801DA898 001D77D8  40 82 00 28 */	bne lbl_801DA8C0
/* 801DA89C 001D77DC  38 6D 8F 2C */	addi r3, r13, "smList__26JALList<15JALSeModPitFGrp>"@sda21
/* 801DA8A0 001D77E0  4B E3 43 95 */	bl initiate__10JSUPtrListFv
/* 801DA8A4 001D77E4  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModPitFGrp>Fv"@ha
/* 801DA8A8 001D77E8  38 83 A6 AC */	addi r4, r3, "__dt__26JSUList<15JALSeModPitFGrp>Fv"@l
/* 801DA8AC 001D77EC  38 6D 8F 2C */	addi r3, r13, "smList__26JALList<15JALSeModPitFGrp>"@sda21
/* 801DA8B0 001D77F0  38 BF 00 54 */	addi r5, r31, 0x54
/* 801DA8B4 001D77F4  4B EA 7E 75 */	bl __register_global_object
/* 801DA8B8 001D77F8  38 00 00 01 */	li r0, 1
/* 801DA8BC 001D77FC  98 0D 8F 90 */	stb r0, "__init__smList__26JALList<15JALSeModPitFGrp>"@sda21(r13)
lbl_801DA8C0:
/* 801DA8C0 001D7800  88 0D 8F 91 */	lbz r0, "__init__smList__26JALList<15JALSeModVolFGrp>"@sda21(r13)
/* 801DA8C4 001D7804  7C 00 07 75 */	extsb. r0, r0
/* 801DA8C8 001D7808  40 82 00 28 */	bne lbl_801DA8F0
/* 801DA8CC 001D780C  38 6D 8F 38 */	addi r3, r13, "smList__26JALList<15JALSeModVolFGrp>"@sda21
/* 801DA8D0 001D7810  4B E3 43 65 */	bl initiate__10JSUPtrListFv
/* 801DA8D4 001D7814  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModVolFGrp>Fv"@ha
/* 801DA8D8 001D7818  38 83 A6 54 */	addi r4, r3, "__dt__26JSUList<15JALSeModVolFGrp>Fv"@l
/* 801DA8DC 001D781C  38 6D 8F 38 */	addi r3, r13, "smList__26JALList<15JALSeModVolFGrp>"@sda21
/* 801DA8E0 001D7820  38 BF 00 60 */	addi r5, r31, 0x60
/* 801DA8E4 001D7824  4B EA 7E 45 */	bl __register_global_object
/* 801DA8E8 001D7828  38 00 00 01 */	li r0, 1
/* 801DA8EC 001D782C  98 0D 8F 91 */	stb r0, "__init__smList__26JALList<15JALSeModVolFGrp>"@sda21(r13)
lbl_801DA8F0:
/* 801DA8F0 001D7830  88 0D 8F 92 */	lbz r0, "__init__smList__26JALList<15JALSeModEffDist>"@sda21(r13)
/* 801DA8F4 001D7834  7C 00 07 75 */	extsb. r0, r0
/* 801DA8F8 001D7838  40 82 00 28 */	bne lbl_801DA920
/* 801DA8FC 001D783C  38 6D 8F 44 */	addi r3, r13, "smList__26JALList<15JALSeModEffDist>"@sda21
/* 801DA900 001D7840  4B E3 43 35 */	bl initiate__10JSUPtrListFv
/* 801DA904 001D7844  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModEffDist>Fv"@ha
/* 801DA908 001D7848  38 83 A5 FC */	addi r4, r3, "__dt__26JSUList<15JALSeModEffDist>Fv"@l
/* 801DA90C 001D784C  38 6D 8F 44 */	addi r3, r13, "smList__26JALList<15JALSeModEffDist>"@sda21
/* 801DA910 001D7850  38 BF 00 6C */	addi r5, r31, 0x6c
/* 801DA914 001D7854  4B EA 7E 15 */	bl __register_global_object
/* 801DA918 001D7858  38 00 00 01 */	li r0, 1
/* 801DA91C 001D785C  98 0D 8F 92 */	stb r0, "__init__smList__26JALList<15JALSeModEffDist>"@sda21(r13)
lbl_801DA920:
/* 801DA920 001D7860  88 0D 8F 93 */	lbz r0, "__init__smList__26JALList<15JALSeModPitDist>"@sda21(r13)
/* 801DA924 001D7864  7C 00 07 75 */	extsb. r0, r0
/* 801DA928 001D7868  40 82 00 28 */	bne lbl_801DA950
/* 801DA92C 001D786C  38 6D 8F 50 */	addi r3, r13, "smList__26JALList<15JALSeModPitDist>"@sda21
/* 801DA930 001D7870  4B E3 43 05 */	bl initiate__10JSUPtrListFv
/* 801DA934 001D7874  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModPitDist>Fv"@ha
/* 801DA938 001D7878  38 83 A5 A4 */	addi r4, r3, "__dt__26JSUList<15JALSeModPitDist>Fv"@l
/* 801DA93C 001D787C  38 6D 8F 50 */	addi r3, r13, "smList__26JALList<15JALSeModPitDist>"@sda21
/* 801DA940 001D7880  38 BF 00 78 */	addi r5, r31, 0x78
/* 801DA944 001D7884  4B EA 7D E5 */	bl __register_global_object
/* 801DA948 001D7888  38 00 00 01 */	li r0, 1
/* 801DA94C 001D788C  98 0D 8F 93 */	stb r0, "__init__smList__26JALList<15JALSeModPitDist>"@sda21(r13)
lbl_801DA950:
/* 801DA950 001D7890  88 0D 8F 94 */	lbz r0, "__init__smList__26JALList<15JALSeModVolDist>"@sda21(r13)
/* 801DA954 001D7894  7C 00 07 75 */	extsb. r0, r0
/* 801DA958 001D7898  40 82 00 28 */	bne lbl_801DA980
/* 801DA95C 001D789C  38 6D 8F 5C */	addi r3, r13, "smList__26JALList<15JALSeModVolDist>"@sda21
/* 801DA960 001D78A0  4B E3 42 D5 */	bl initiate__10JSUPtrListFv
/* 801DA964 001D78A4  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModVolDist>Fv"@ha
/* 801DA968 001D78A8  38 83 A5 4C */	addi r4, r3, "__dt__26JSUList<15JALSeModVolDist>Fv"@l
/* 801DA96C 001D78AC  38 6D 8F 5C */	addi r3, r13, "smList__26JALList<15JALSeModVolDist>"@sda21
/* 801DA970 001D78B0  38 BF 00 84 */	addi r5, r31, 0x84
/* 801DA974 001D78B4  4B EA 7D B5 */	bl __register_global_object
/* 801DA978 001D78B8  38 00 00 01 */	li r0, 1
/* 801DA97C 001D78BC  98 0D 8F 94 */	stb r0, "__init__smList__26JALList<15JALSeModVolDist>"@sda21(r13)
lbl_801DA980:
/* 801DA980 001D78C0  88 0D 8F 95 */	lbz r0, "__init__smList__26JALList<15JALSeModEffFunk>"@sda21(r13)
/* 801DA984 001D78C4  7C 00 07 75 */	extsb. r0, r0
/* 801DA988 001D78C8  40 82 00 28 */	bne lbl_801DA9B0
/* 801DA98C 001D78CC  38 6D 8F 68 */	addi r3, r13, "smList__26JALList<15JALSeModEffFunk>"@sda21
/* 801DA990 001D78D0  4B E3 42 A5 */	bl initiate__10JSUPtrListFv
/* 801DA994 001D78D4  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModEffFunk>Fv"@ha
/* 801DA998 001D78D8  38 83 A4 F4 */	addi r4, r3, "__dt__26JSUList<15JALSeModEffFunk>Fv"@l
/* 801DA99C 001D78DC  38 6D 8F 68 */	addi r3, r13, "smList__26JALList<15JALSeModEffFunk>"@sda21
/* 801DA9A0 001D78E0  38 BF 00 90 */	addi r5, r31, 0x90
/* 801DA9A4 001D78E4  4B EA 7D 85 */	bl __register_global_object
/* 801DA9A8 001D78E8  38 00 00 01 */	li r0, 1
/* 801DA9AC 001D78EC  98 0D 8F 95 */	stb r0, "__init__smList__26JALList<15JALSeModEffFunk>"@sda21(r13)
lbl_801DA9B0:
/* 801DA9B0 001D78F0  88 0D 8F 96 */	lbz r0, "__init__smList__26JALList<15JALSeModPitFunk>"@sda21(r13)
/* 801DA9B4 001D78F4  7C 00 07 75 */	extsb. r0, r0
/* 801DA9B8 001D78F8  40 82 00 28 */	bne lbl_801DA9E0
/* 801DA9BC 001D78FC  38 6D 8F 74 */	addi r3, r13, "smList__26JALList<15JALSeModPitFunk>"@sda21
/* 801DA9C0 001D7900  4B E3 42 75 */	bl initiate__10JSUPtrListFv
/* 801DA9C4 001D7904  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModPitFunk>Fv"@ha
/* 801DA9C8 001D7908  38 83 A4 9C */	addi r4, r3, "__dt__26JSUList<15JALSeModPitFunk>Fv"@l
/* 801DA9CC 001D790C  38 6D 8F 74 */	addi r3, r13, "smList__26JALList<15JALSeModPitFunk>"@sda21
/* 801DA9D0 001D7910  38 BF 00 9C */	addi r5, r31, 0x9c
/* 801DA9D4 001D7914  4B EA 7D 55 */	bl __register_global_object
/* 801DA9D8 001D7918  38 00 00 01 */	li r0, 1
/* 801DA9DC 001D791C  98 0D 8F 96 */	stb r0, "__init__smList__26JALList<15JALSeModPitFunk>"@sda21(r13)
lbl_801DA9E0:
/* 801DA9E0 001D7920  88 0D 8F 97 */	lbz r0, "__init__smList__26JALList<15JALSeModVolFunk>"@sda21(r13)
/* 801DA9E4 001D7924  7C 00 07 75 */	extsb. r0, r0
/* 801DA9E8 001D7928  40 82 00 28 */	bne lbl_801DAA10
/* 801DA9EC 001D792C  38 6D 8F 80 */	addi r3, r13, "smList__26JALList<15JALSeModVolFunk>"@sda21
/* 801DA9F0 001D7930  4B E3 42 45 */	bl initiate__10JSUPtrListFv
/* 801DA9F4 001D7934  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModVolFunk>Fv"@ha
/* 801DA9F8 001D7938  38 83 A4 44 */	addi r4, r3, "__dt__26JSUList<15JALSeModVolFunk>Fv"@l
/* 801DA9FC 001D793C  38 6D 8F 80 */	addi r3, r13, "smList__26JALList<15JALSeModVolFunk>"@sda21
/* 801DAA00 001D7940  38 BF 00 A8 */	addi r5, r31, 0xa8
/* 801DAA04 001D7944  4B EA 7D 25 */	bl __register_global_object
/* 801DAA08 001D7948  38 00 00 01 */	li r0, 1
/* 801DAA0C 001D794C  98 0D 8F 97 */	stb r0, "__init__smList__26JALList<15JALSeModVolFunk>"@sda21(r13)
lbl_801DAA10:
/* 801DAA10 001D7950  80 01 00 14 */	lwz r0, 0x14(r1)
/* 801DAA14 001D7954  83 E1 00 0C */	lwz r31, 0xc(r1)
/* 801DAA18 001D7958  38 21 00 10 */	addi r1, r1, 0x10
/* 801DAA1C 001D795C  7C 08 03 A6 */	mtlr r0
/* 801DAA20 001D7960  4E 80 00 20 */	blr 

.section .ctors, "wa"  # 0x8036FBA0 - 0x8036FF80
lbl_constructor:
  .4byte __sinit_JointModelManager_cpp

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global __vt__18TJointModelManager
__vt__18TJointModelManager:
  .4byte 0
  .4byte 0
  .4byte __dt__18TJointModelManagerFv
  .4byte getType__Q26JDrama8TNameRefCFv
  .4byte load__Q26JDrama8TNameRefFR20JSUMemoryInputStream
  .4byte save__Q26JDrama8TNameRefFR21JSUMemoryOutputStream
  .4byte loadAfter__Q26JDrama8TNameRefFv
  .4byte searchF__Q26JDrama8TNameRefFUsPCc
  .4byte perform__18TJointModelManagerFUlPQ26JDrama9TGraphics
  .4byte newJointModel__18TJointModelManagerCFi

.section .bss, "wa"  # 0x803E6000 - 0x80408AC0
"@2405":
	.skip 0xC
"@2406":
	.skip 0xC
"@2407":
	.skip 0xC
"@2408":
	.skip 0xC
"@2409":
	.skip 0xC
"@2410":
	.skip 0xC
"@2411":
	.skip 0xC
"@2412":
	.skip 0xC
"@2413":
	.skip 0xC
"@2414":
	.skip 0xC
"@2415":
	.skip 0xC
"@2416":
	.skip 0xC
"@2417":
	.skip 0xC
"@2418":
	.skip 0xC
"@2419":
	.skip 0x10
