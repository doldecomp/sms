.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global __dt__13TEventWatcherFv
__dt__13TEventWatcherFv:
/* 800DC5A0 000D94E0  7C 08 02 A6 */	mflr r0
/* 800DC5A4 000D94E4  90 01 00 04 */	stw r0, 4(r1)
/* 800DC5A8 000D94E8  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 800DC5AC 000D94EC  93 E1 00 14 */	stw r31, 0x14(r1)
/* 800DC5B0 000D94F0  3B E4 00 00 */	addi r31, r4, 0
/* 800DC5B4 000D94F4  93 C1 00 10 */	stw r30, 0x10(r1)
/* 800DC5B8 000D94F8  7C 7E 1B 79 */	or. r30, r3, r3
/* 800DC5BC 000D94FC  41 82 00 3C */	beq lbl_800DC5F8
/* 800DC5C0 000D9500  3C 60 80 3B */	lis r3, __vt__13TEventWatcher@ha
/* 800DC5C4 000D9504  38 03 39 40 */	addi r0, r3, __vt__13TEventWatcher@l
/* 800DC5C8 000D9508  90 1E 00 00 */	stw r0, 0(r30)
/* 800DC5CC 000D950C  41 82 00 1C */	beq lbl_800DC5E8
/* 800DC5D0 000D9510  3C 60 80 3B */	lis r3, __vt__Q26JDrama8TViewObj@ha
/* 800DC5D4 000D9514  38 03 A0 C0 */	addi r0, r3, __vt__Q26JDrama8TViewObj@l
/* 800DC5D8 000D9518  90 1E 00 00 */	stw r0, 0(r30)
/* 800DC5DC 000D951C  38 7E 00 00 */	addi r3, r30, 0
/* 800DC5E0 000D9520  38 80 00 00 */	li r4, 0
/* 800DC5E4 000D9524  4B F6 82 19 */	bl __dt__Q26JDrama8TNameRefFv
lbl_800DC5E8:
/* 800DC5E8 000D9528  7F E0 07 35 */	extsh. r0, r31
/* 800DC5EC 000D952C  40 81 00 0C */	ble lbl_800DC5F8
/* 800DC5F0 000D9530  7F C3 F3 78 */	mr r3, r30
/* 800DC5F4 000D9534  4B F3 04 BD */	bl __dl__FPv
lbl_800DC5F8:
/* 800DC5F8 000D9538  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 800DC5FC 000D953C  7F C3 F3 78 */	mr r3, r30
/* 800DC600 000D9540  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 800DC604 000D9544  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 800DC608 000D9548  7C 08 03 A6 */	mtlr r0
/* 800DC60C 000D954C  38 21 00 18 */	addi r1, r1, 0x18
/* 800DC610 000D9550  4E 80 00 20 */	blr 

.global perform__13TEventWatcherFUlPQ26JDrama9TGraphics
perform__13TEventWatcherFUlPQ26JDrama9TGraphics:
/* 800DC614 000D9554  7C 08 02 A6 */	mflr r0
/* 800DC618 000D9558  90 01 00 04 */	stw r0, 4(r1)
/* 800DC61C 000D955C  54 80 07 FF */	clrlwi. r0, r4, 0x1f
/* 800DC620 000D9560  94 21 FF F8 */	stwu r1, -8(r1)
/* 800DC624 000D9564  41 82 00 20 */	beq lbl_800DC644
/* 800DC628 000D9568  80 63 00 14 */	lwz r3, 0x14(r3)
/* 800DC62C 000D956C  28 03 00 00 */	cmplwi r3, 0
/* 800DC630 000D9570  41 82 00 14 */	beq lbl_800DC644
/* 800DC634 000D9574  81 83 00 5C */	lwz r12, 0x5c(r3)
/* 800DC638 000D9578  81 8C 00 10 */	lwz r12, 0x10(r12)
/* 800DC63C 000D957C  7D 88 03 A6 */	mtlr r12
/* 800DC640 000D9580  4E 80 00 21 */	blrl 
lbl_800DC644:
/* 800DC644 000D9584  80 01 00 0C */	lwz r0, 0xc(r1)
/* 800DC648 000D9588  38 21 00 08 */	addi r1, r1, 8
/* 800DC64C 000D958C  7C 08 03 A6 */	mtlr r0
/* 800DC650 000D9590  4E 80 00 20 */	blr 

.global launchScript__13TEventWatcherFPCc
launchScript__13TEventWatcherFPCc:
/* 800DC654 000D9594  7C 08 02 A6 */	mflr r0
/* 800DC658 000D9598  90 01 00 04 */	stw r0, 4(r1)
/* 800DC65C 000D959C  94 21 FF E0 */	stwu r1, -0x20(r1)
/* 800DC660 000D95A0  93 E1 00 1C */	stw r31, 0x1c(r1)
/* 800DC664 000D95A4  93 C1 00 18 */	stw r30, 0x18(r1)
/* 800DC668 000D95A8  3B C3 00 00 */	addi r30, r3, 0
/* 800DC66C 000D95AC  38 64 00 00 */	addi r3, r4, 0
/* 800DC670 000D95B0  93 A1 00 14 */	stw r29, 0x14(r1)
/* 800DC674 000D95B4  4B F2 F6 49 */	bl getGlbResource__13JKRFileLoaderFPCc
/* 800DC678 000D95B8  7C 7F 1B 79 */	or. r31, r3, r3
/* 800DC67C 000D95BC  41 82 00 78 */	beq lbl_800DC6F4
/* 800DC680 000D95C0  38 60 00 08 */	li r3, 8
/* 800DC684 000D95C4  4B F3 02 2D */	bl __nw__FUl
/* 800DC688 000D95C8  7C 7D 1B 79 */	or. r29, r3, r3
/* 800DC68C 000D95CC  41 82 00 1C */	beq lbl_800DC6A8
/* 800DC690 000D95D0  38 7D 00 00 */	addi r3, r29, 0
/* 800DC694 000D95D4  38 9F 00 00 */	addi r4, r31, 0
/* 800DC698 000D95D8  48 03 F7 F5 */	bl __ct__10TSpcBinaryFPv
/* 800DC69C 000D95DC  3C 60 80 3B */	lis r3, __vt__32TSpcTypedBinary$$013TEventWatcher$$1@ha
/* 800DC6A0 000D95E0  38 03 39 7C */	addi r0, r3, __vt__32TSpcTypedBinary$$013TEventWatcher$$1@l
/* 800DC6A4 000D95E4  90 1D 00 04 */	stw r0, 4(r29)
lbl_800DC6A8:
/* 800DC6A8 000D95E8  93 BE 00 10 */	stw r29, 0x10(r30)
/* 800DC6AC 000D95EC  80 7E 00 10 */	lwz r3, 0x10(r30)
/* 800DC6B0 000D95F0  48 03 F4 F1 */	bl init__10TSpcBinaryFv
/* 800DC6B4 000D95F4  38 60 00 60 */	li r3, 0x60
/* 800DC6B8 000D95F8  4B F3 01 F9 */	bl __nw__FUl
/* 800DC6BC 000D95FC  7C 7D 1B 79 */	or. r29, r3, r3
/* 800DC6C0 000D9600  41 82 00 30 */	beq lbl_800DC6F0
/* 800DC6C4 000D9604  80 9E 00 10 */	lwz r4, 0x10(r30)
/* 800DC6C8 000D9608  38 7D 00 00 */	addi r3, r29, 0
/* 800DC6CC 000D960C  38 BE 00 00 */	addi r5, r30, 0
/* 800DC6D0 000D9610  38 C0 00 20 */	li r6, 0x20
/* 800DC6D4 000D9614  38 E0 00 20 */	li r7, 0x20
/* 800DC6D8 000D9618  39 00 00 20 */	li r8, 0x20
/* 800DC6DC 000D961C  39 20 00 20 */	li r9, 0x20
/* 800DC6E0 000D9620  48 03 B1 BD */	bl __ct__10TSpcInterpFP10TSpcBinaryPviiii
/* 800DC6E4 000D9624  3C 60 80 3B */	lis r3, __vt__32TSpcTypedInterp$$013TEventWatcher$$1@ha
/* 800DC6E8 000D9628  38 03 39 68 */	addi r0, r3, __vt__32TSpcTypedInterp$$013TEventWatcher$$1@l
/* 800DC6EC 000D962C  90 1D 00 5C */	stw r0, 0x5c(r29)
lbl_800DC6F0:
/* 800DC6F0 000D9630  93 BE 00 14 */	stw r29, 0x14(r30)
lbl_800DC6F4:
/* 800DC6F4 000D9634  80 01 00 24 */	lwz r0, 0x24(r1)
/* 800DC6F8 000D9638  83 E1 00 1C */	lwz r31, 0x1c(r1)
/* 800DC6FC 000D963C  83 C1 00 18 */	lwz r30, 0x18(r1)
/* 800DC700 000D9640  7C 08 03 A6 */	mtlr r0
/* 800DC704 000D9644  83 A1 00 14 */	lwz r29, 0x14(r1)
/* 800DC708 000D9648  38 21 00 20 */	addi r1, r1, 0x20
/* 800DC70C 000D964C  4E 80 00 20 */	blr 

.global __ct__13TEventWatcherFPCcPCc
__ct__13TEventWatcherFPCcPCc:
/* 800DC710 000D9650  7C 08 02 A6 */	mflr r0
/* 800DC714 000D9654  90 01 00 04 */	stw r0, 4(r1)
/* 800DC718 000D9658  94 21 FF E0 */	stwu r1, -0x20(r1)
/* 800DC71C 000D965C  93 E1 00 1C */	stw r31, 0x1c(r1)
/* 800DC720 000D9660  93 C1 00 18 */	stw r30, 0x18(r1)
/* 800DC724 000D9664  3B C5 00 00 */	addi r30, r5, 0
/* 800DC728 000D9668  90 61 00 08 */	stw r3, 8(r1)
/* 800DC72C 000D966C  3C 60 80 3B */	lis r3, __vt__Q26JDrama8TNameRef@ha
/* 800DC730 000D9670  38 03 A8 60 */	addi r0, r3, __vt__Q26JDrama8TNameRef@l
/* 800DC734 000D9674  83 E1 00 08 */	lwz r31, 8(r1)
/* 800DC738 000D9678  7C 83 23 78 */	mr r3, r4
/* 800DC73C 000D967C  90 1F 00 00 */	stw r0, 0(r31)
/* 800DC740 000D9680  90 9F 00 04 */	stw r4, 4(r31)
/* 800DC744 000D9684  4B F6 7F 49 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800DC748 000D9688  B0 7F 00 08 */	sth r3, 8(r31)
/* 800DC74C 000D968C  3C 60 80 3B */	lis r3, __vt__Q26JDrama8TViewObj@ha
/* 800DC750 000D9690  38 03 A0 C0 */	addi r0, r3, __vt__Q26JDrama8TViewObj@l
/* 800DC754 000D9694  90 1F 00 00 */	stw r0, 0(r31)
/* 800DC758 000D9698  38 A0 00 00 */	li r5, 0
/* 800DC75C 000D969C  3C 60 80 3B */	lis r3, __vt__13TEventWatcher@ha
/* 800DC760 000D96A0  B0 BF 00 0C */	sth r5, 0xc(r31)
/* 800DC764 000D96A4  38 03 39 40 */	addi r0, r3, __vt__13TEventWatcher@l
/* 800DC768 000D96A8  38 7F 00 00 */	addi r3, r31, 0
/* 800DC76C 000D96AC  90 1F 00 00 */	stw r0, 0(r31)
/* 800DC770 000D96B0  7F C4 F3 78 */	mr r4, r30
/* 800DC774 000D96B4  90 BF 00 10 */	stw r5, 0x10(r31)
/* 800DC778 000D96B8  90 BF 00 14 */	stw r5, 0x14(r31)
/* 800DC77C 000D96BC  81 9F 00 00 */	lwz r12, 0(r31)
/* 800DC780 000D96C0  81 8C 00 24 */	lwz r12, 0x24(r12)
/* 800DC784 000D96C4  7D 88 03 A6 */	mtlr r12
/* 800DC788 000D96C8  4E 80 00 21 */	blrl 
/* 800DC78C 000D96CC  80 01 00 24 */	lwz r0, 0x24(r1)
/* 800DC790 000D96D0  7F E3 FB 78 */	mr r3, r31
/* 800DC794 000D96D4  83 E1 00 1C */	lwz r31, 0x1c(r1)
/* 800DC798 000D96D8  83 C1 00 18 */	lwz r30, 0x18(r1)
/* 800DC79C 000D96DC  7C 08 03 A6 */	mtlr r0
/* 800DC7A0 000D96E0  38 21 00 20 */	addi r1, r1, 0x20
/* 800DC7A4 000D96E4  4E 80 00 20 */	blr 

.global __ct__13TEventWatcherFPCc
__ct__13TEventWatcherFPCc:
/* 800DC7A8 000D96E8  7C 08 02 A6 */	mflr r0
/* 800DC7AC 000D96EC  3C A0 80 3B */	lis r5, __vt__Q26JDrama8TNameRef@ha
/* 800DC7B0 000D96F0  90 01 00 04 */	stw r0, 4(r1)
/* 800DC7B4 000D96F4  38 05 A8 60 */	addi r0, r5, __vt__Q26JDrama8TNameRef@l
/* 800DC7B8 000D96F8  94 21 FF E0 */	stwu r1, -0x20(r1)
/* 800DC7BC 000D96FC  93 E1 00 1C */	stw r31, 0x1c(r1)
/* 800DC7C0 000D9700  3B E3 00 00 */	addi r31, r3, 0
/* 800DC7C4 000D9704  90 03 00 00 */	stw r0, 0(r3)
/* 800DC7C8 000D9708  7C 83 23 78 */	mr r3, r4
/* 800DC7CC 000D970C  90 9F 00 04 */	stw r4, 4(r31)
/* 800DC7D0 000D9710  4B F6 7E BD */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800DC7D4 000D9714  B0 7F 00 08 */	sth r3, 8(r31)
/* 800DC7D8 000D9718  3C 60 80 3B */	lis r3, __vt__Q26JDrama8TViewObj@ha
/* 800DC7DC 000D971C  38 03 A0 C0 */	addi r0, r3, __vt__Q26JDrama8TViewObj@l
/* 800DC7E0 000D9720  90 1F 00 00 */	stw r0, 0(r31)
/* 800DC7E4 000D9724  38 80 00 00 */	li r4, 0
/* 800DC7E8 000D9728  3C 60 80 3B */	lis r3, __vt__13TEventWatcher@ha
/* 800DC7EC 000D972C  B0 9F 00 0C */	sth r4, 0xc(r31)
/* 800DC7F0 000D9730  38 03 39 40 */	addi r0, r3, __vt__13TEventWatcher@l
/* 800DC7F4 000D9734  38 7F 00 00 */	addi r3, r31, 0
/* 800DC7F8 000D9738  90 1F 00 00 */	stw r0, 0(r31)
/* 800DC7FC 000D973C  90 9F 00 10 */	stw r4, 0x10(r31)
/* 800DC800 000D9740  90 9F 00 14 */	stw r4, 0x14(r31)
/* 800DC804 000D9744  80 01 00 24 */	lwz r0, 0x24(r1)
/* 800DC808 000D9748  83 E1 00 1C */	lwz r31, 0x1c(r1)
/* 800DC80C 000D974C  38 21 00 20 */	addi r1, r1, 0x20
/* 800DC810 000D9750  7C 08 03 A6 */	mtlr r0
/* 800DC814 000D9754  4E 80 00 20 */	blr 

.global initUserBuiltin__32TSpcTypedBinary$$013TEventWatcher$$1Fv
initUserBuiltin__32TSpcTypedBinary$$013TEventWatcher$$1Fv:
/* 800DC818 000D9758  7C 08 02 A6 */	mflr r0
/* 800DC81C 000D975C  3C A0 80 37 */	lis r5, $$21490@ha
/* 800DC820 000D9760  90 01 00 04 */	stw r0, 4(r1)
/* 800DC824 000D9764  3C 80 80 0E */	lis r4, evGetSystemFlag__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DC828 000D9768  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 800DC82C 000D976C  93 E1 00 14 */	stw r31, 0x14(r1)
/* 800DC830 000D9770  3B E3 00 00 */	addi r31, r3, 0
/* 800DC834 000D9774  93 C1 00 10 */	stw r30, 0x10(r1)
/* 800DC838 000D9778  3B C5 29 90 */	addi r30, r5, $$21490@l
/* 800DC83C 000D977C  38 A4 64 1C */	addi r5, r4, evGetSystemFlag__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DC840 000D9780  38 9E 01 58 */	addi r4, r30, 0x158
/* 800DC844 000D9784  48 03 F1 C5 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DC848 000D9788  3C 60 80 0E */	lis r3, evSetSystemFlag__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DC84C 000D978C  38 A3 62 30 */	addi r5, r3, evSetSystemFlag__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DC850 000D9790  38 7F 00 00 */	addi r3, r31, 0
/* 800DC854 000D9794  38 9E 01 68 */	addi r4, r30, 0x168
/* 800DC858 000D9798  48 03 F1 B1 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DC85C 000D979C  3C 60 80 0E */	lis r3, evGetNameRefHandle__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DC860 000D97A0  38 A3 60 E0 */	addi r5, r3, evGetNameRefHandle__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DC864 000D97A4  38 7F 00 00 */	addi r3, r31, 0
/* 800DC868 000D97A8  38 9E 01 78 */	addi r4, r30, 0x178
/* 800DC86C 000D97AC  48 03 F1 9D */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DC870 000D97B0  3C 60 80 0E */	lis r3, evGetNameRefName__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DC874 000D97B4  38 A3 5F 78 */	addi r5, r3, evGetNameRefName__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DC878 000D97B8  38 7F 00 00 */	addi r3, r31, 0
/* 800DC87C 000D97BC  38 9E 01 8C */	addi r4, r30, 0x18c
/* 800DC880 000D97C0  48 03 F1 89 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DC884 000D97C4  3C 60 80 0E */	lis r3, evGetNPCType__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DC888 000D97C8  38 A3 5D 98 */	addi r5, r3, evGetNPCType__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DC88C 000D97CC  38 7F 00 00 */	addi r3, r31, 0
/* 800DC890 000D97D0  38 9E 01 9C */	addi r4, r30, 0x19c
/* 800DC894 000D97D4  48 03 F1 75 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DC898 000D97D8  3C 60 80 0E */	lis r3, evSetFlagNPCDontTalk__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DC89C 000D97DC  38 A3 5B 10 */	addi r5, r3, evSetFlagNPCDontTalk__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DC8A0 000D97E0  38 7F 00 00 */	addi r3, r31, 0
/* 800DC8A4 000D97E4  38 9E 01 A8 */	addi r4, r30, 0x1a8
/* 800DC8A8 000D97E8  48 03 F1 61 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DC8AC 000D97EC  3C 60 80 0E */	lis r3, evSetFlagNPCDontThrow__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DC8B0 000D97F0  38 A3 58 88 */	addi r5, r3, evSetFlagNPCDontThrow__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DC8B4 000D97F4  38 7F 00 00 */	addi r3, r31, 0
/* 800DC8B8 000D97F8  38 9E 01 BC */	addi r4, r30, 0x1bc
/* 800DC8BC 000D97FC  48 03 F1 4D */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DC8C0 000D9800  3C 60 80 0E */	lis r3, evSetFlagNPCDead__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DC8C4 000D9804  38 A3 56 00 */	addi r5, r3, evSetFlagNPCDead__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DC8C8 000D9808  38 7F 00 00 */	addi r3, r31, 0
/* 800DC8CC 000D980C  38 9E 01 D0 */	addi r4, r30, 0x1d0
/* 800DC8D0 000D9810  48 03 F1 39 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DC8D4 000D9814  3C 60 80 0E */	lis r3, evIsNearSameActors__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DC8D8 000D9818  38 A3 50 F8 */	addi r5, r3, evIsNearSameActors__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DC8DC 000D981C  38 7F 00 00 */	addi r3, r31, 0
/* 800DC8E0 000D9820  38 9E 01 E0 */	addi r4, r30, 0x1e0
/* 800DC8E4 000D9824  48 03 F1 25 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DC8E8 000D9828  3C 60 80 0E */	lis r3, evIsNearActors__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DC8EC 000D982C  38 A3 4D 74 */	addi r5, r3, evIsNearActors__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DC8F0 000D9830  38 7F 00 00 */	addi r3, r31, 0
/* 800DC8F4 000D9834  38 9E 01 F4 */	addi r4, r30, 0x1f4
/* 800DC8F8 000D9838  48 03 F1 11 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DC8FC 000D983C  3C 60 80 0E */	lis r3, evGetTalkNPC__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DC900 000D9840  38 A3 4C CC */	addi r5, r3, evGetTalkNPC__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DC904 000D9844  38 7F 00 00 */	addi r3, r31, 0
/* 800DC908 000D9848  38 9E 02 04 */	addi r4, r30, 0x204
/* 800DC90C 000D984C  48 03 F0 FD */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DC910 000D9850  3C 60 80 0E */	lis r3, evGetTalkNPCName__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DC914 000D9854  38 A3 4B 8C */	addi r5, r3, evGetTalkNPCName__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DC918 000D9858  38 7F 00 00 */	addi r3, r31, 0
/* 800DC91C 000D985C  38 9E 02 10 */	addi r4, r30, 0x210
/* 800DC920 000D9860  48 03 F0 E9 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DC924 000D9864  3C 60 80 0E */	lis r3, evSetTalkMsgID__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DC928 000D9868  38 A3 49 8C */	addi r5, r3, evSetTalkMsgID__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DC92C 000D986C  38 7F 00 00 */	addi r3, r31, 0
/* 800DC930 000D9870  38 9E 02 20 */	addi r4, r30, 0x220
/* 800DC934 000D9874  48 03 F0 D5 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DC938 000D9878  3C 60 80 0E */	lis r3, evGetTalkMode__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DC93C 000D987C  38 A3 48 F4 */	addi r5, r3, evGetTalkMode__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DC940 000D9880  38 7F 00 00 */	addi r3, r31, 0
/* 800DC944 000D9884  38 9E 02 30 */	addi r4, r30, 0x230
/* 800DC948 000D9888  48 03 F0 C1 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DC94C 000D988C  3C 60 80 0E */	lis r3, evGetTalkSelectedValue__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DC950 000D9890  38 A3 48 58 */	addi r5, r3, evGetTalkSelectedValue__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DC954 000D9894  38 7F 00 00 */	addi r3, r31, 0
/* 800DC958 000D9898  38 9E 02 3C */	addi r4, r30, 0x23c
/* 800DC95C 000D989C  48 03 F0 AD */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DC960 000D98A0  3C 60 80 0E */	lis r3, evSetValue2TalkVariable__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DC964 000D98A4  38 A3 47 74 */	addi r5, r3, evSetValue2TalkVariable__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DC968 000D98A8  38 7F 00 00 */	addi r3, r31, 0
/* 800DC96C 000D98AC  38 9E 02 54 */	addi r4, r30, 0x254
/* 800DC970 000D98B0  48 03 F0 99 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DC974 000D98B4  3C 60 80 0E */	lis r3, evIsTalkModeNow__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DC978 000D98B8  38 A3 46 B0 */	addi r5, r3, evIsTalkModeNow__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DC97C 000D98BC  38 7F 00 00 */	addi r3, r31, 0
/* 800DC980 000D98C0  38 9E 02 6C */	addi r4, r30, 0x26c
/* 800DC984 000D98C4  48 03 F0 85 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DC988 000D98C8  3C 60 80 0E */	lis r3, evSetFlagNPCCanTaken__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DC98C 000D98CC  38 A3 44 9C */	addi r5, r3, evSetFlagNPCCanTaken__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DC990 000D98D0  38 7F 00 00 */	addi r3, r31, 0
/* 800DC994 000D98D4  38 9E 02 7C */	addi r4, r30, 0x27c
/* 800DC998 000D98D8  48 03 F0 71 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DC99C 000D98DC  3C 60 80 0E */	lis r3, evPushNerve4LiveActor__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DC9A0 000D98E0  38 A3 42 48 */	addi r5, r3, evPushNerve4LiveActor__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DC9A4 000D98E4  38 7F 00 00 */	addi r3, r31, 0
/* 800DC9A8 000D98E8  38 9E 02 90 */	addi r4, r30, 0x290
/* 800DC9AC 000D98EC  48 03 F0 5D */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DC9B0 000D98F0  3C 60 80 0E */	lis r3, evIsOnLiveActorFlag__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DC9B4 000D98F4  38 A3 3F E0 */	addi r5, r3, evIsOnLiveActorFlag__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DC9B8 000D98F8  38 7F 00 00 */	addi r3, r31, 0
/* 800DC9BC 000D98FC  38 9E 02 A4 */	addi r4, r30, 0x2a4
/* 800DC9C0 000D9900  48 03 F0 49 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DC9C4 000D9904  3C 60 80 0E */	lis r3, evSetHide4LiveActor__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DC9C8 000D9908  38 A3 3D B4 */	addi r5, r3, evSetHide4LiveActor__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DC9CC 000D990C  38 7F 00 00 */	addi r3, r31, 0
/* 800DC9D0 000D9910  38 9E 02 B8 */	addi r4, r30, 0x2b8
/* 800DC9D4 000D9914  48 03 F0 35 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DC9D8 000D9918  3C 60 80 0E */	lis r3, evSetDead4LiveActor__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DC9DC 000D991C  38 A3 3B 88 */	addi r5, r3, evSetDead4LiveActor__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DC9E0 000D9920  38 7F 00 00 */	addi r3, r31, 0
/* 800DC9E4 000D9924  38 9E 02 CC */	addi r4, r30, 0x2cc
/* 800DC9E8 000D9928  48 03 F0 21 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DC9EC 000D992C  3C 60 80 0E */	lis r3, evSetTimeLimit__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DC9F0 000D9930  38 A3 3A 34 */	addi r5, r3, evSetTimeLimit__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DC9F4 000D9934  38 7F 00 00 */	addi r3, r31, 0
/* 800DC9F8 000D9938  38 9E 02 E0 */	addi r4, r30, 0x2e0
/* 800DC9FC 000D993C  48 03 F0 0D */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCA00 000D9940  3C 60 80 0E */	lis r3, evSetAttentionTime__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCA04 000D9944  38 A3 39 7C */	addi r5, r3, evSetAttentionTime__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCA08 000D9948  38 7F 00 00 */	addi r3, r31, 0
/* 800DCA0C 000D994C  38 9E 02 F0 */	addi r4, r30, 0x2f0
/* 800DCA10 000D9950  48 03 EF F9 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCA14 000D9954  3C 60 80 0E */	lis r3, evSetPollutionIncreaseCount__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCA18 000D9958  38 A3 38 C4 */	addi r5, r3, evSetPollutionIncreaseCount__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCA1C 000D995C  38 7F 00 00 */	addi r3, r31, 0
/* 800DCA20 000D9960  38 9E 03 04 */	addi r4, r30, 0x304
/* 800DCA24 000D9964  48 03 EF E5 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCA28 000D9968  3C 60 80 0E */	lis r3, evGetRestTime__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCA2C 000D996C  38 A3 37 E8 */	addi r5, r3, evGetRestTime__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCA30 000D9970  38 7F 00 00 */	addi r3, r31, 0
/* 800DCA34 000D9974  38 9E 03 20 */	addi r4, r30, 0x320
/* 800DCA38 000D9978  48 03 EF D1 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCA3C 000D997C  3C 60 80 0E */	lis r3, evGetPollutionLevel__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCA40 000D9980  38 A3 37 50 */	addi r5, r3, evGetPollutionLevel__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCA44 000D9984  38 7F 00 00 */	addi r3, r31, 0
/* 800DCA48 000D9988  38 9E 03 2C */	addi r4, r30, 0x32c
/* 800DCA4C 000D998C  48 03 EF BD */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCA50 000D9990  3C 60 80 0E */	lis r3, evSetNextStage__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCA54 000D9994  38 A3 35 48 */	addi r5, r3, evSetNextStage__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCA58 000D9998  38 7F 00 00 */	addi r3, r31, 0
/* 800DCA5C 000D999C  38 9E 03 40 */	addi r4, r30, 0x340
/* 800DCA60 000D99A0  48 03 EF A9 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCA64 000D99A4  3C 60 80 0E */	lis r3, evRegisterMovie__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCA68 000D99A8  38 A3 34 04 */	addi r5, r3, evRegisterMovie__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCA6C 000D99AC  38 7F 00 00 */	addi r3, r31, 0
/* 800DCA70 000D99B0  38 9E 03 50 */	addi r4, r30, 0x350
/* 800DCA74 000D99B4  48 03 EF 95 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCA78 000D99B8  3C 60 80 0E */	lis r3, evGameOver__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCA7C 000D99BC  38 A3 33 64 */	addi r5, r3, evGameOver__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCA80 000D99C0  38 7F 00 00 */	addi r3, r31, 0
/* 800DCA84 000D99C4  38 9E 03 60 */	addi r4, r30, 0x360
/* 800DCA88 000D99C8  48 03 EF 81 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCA8C 000D99CC  3C 60 80 0E */	lis r3, evIsGraffitoCoverage0__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCA90 000D99D0  38 A3 32 B8 */	addi r5, r3, evIsGraffitoCoverage0__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCA94 000D99D4  38 7F 00 00 */	addi r3, r31, 0
/* 800DCA98 000D99D8  38 9E 03 6C */	addi r4, r30, 0x36c
/* 800DCA9C 000D99DC  48 03 EF 6D */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCAA0 000D99E0  3C 60 80 0E */	lis r3, evSetGraffitoMultiplied__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCAA4 000D99E4  38 A3 31 0C */	addi r5, r3, evSetGraffitoMultiplied__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCAA8 000D99E8  38 7F 00 00 */	addi r3, r31, 0
/* 800DCAAC 000D99EC  38 9E 03 80 */	addi r4, r30, 0x380
/* 800DCAB0 000D99F0  48 03 EF 59 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCAB4 000D99F4  3C 60 80 0E */	lis r3, evIsBossDefeated__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCAB8 000D99F8  38 A3 30 60 */	addi r5, r3, evIsBossDefeated__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCABC 000D99FC  38 7F 00 00 */	addi r3, r31, 0
/* 800DCAC0 000D9A00  38 9E 03 98 */	addi r4, r30, 0x398
/* 800DCAC4 000D9A04  48 03 EF 45 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCAC8 000D9A08  3C 60 80 0E */	lis r3, evLaunchEventClearDemo__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCACC 000D9A0C  38 A3 2F B0 */	addi r5, r3, evLaunchEventClearDemo__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCAD0 000D9A10  38 7F 00 00 */	addi r3, r31, 0
/* 800DCAD4 000D9A14  38 9E 03 A8 */	addi r4, r30, 0x3a8
/* 800DCAD8 000D9A18  48 03 EF 31 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCADC 000D9A1C  3C 60 80 0E */	lis r3, evIsEMarioReachedToGoal__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCAE0 000D9A20  38 A3 2D E8 */	addi r5, r3, evIsEMarioReachedToGoal__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCAE4 000D9A24  38 7F 00 00 */	addi r3, r31, 0
/* 800DCAE8 000D9A28  38 9E 03 C0 */	addi r4, r30, 0x3c0
/* 800DCAEC 000D9A2C  48 03 EF 1D */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCAF0 000D9A30  3C 60 80 0E */	lis r3, evIsEMarioDownWaitingToTalk__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCAF4 000D9A34  38 A3 2C 20 */	addi r5, r3, evIsEMarioDownWaitingToTalk__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCAF8 000D9A38  38 7F 00 00 */	addi r3, r31, 0
/* 800DCAFC 000D9A3C  38 9E 03 D8 */	addi r4, r30, 0x3d8
/* 800DCB00 000D9A40  48 03 EF 09 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCB04 000D9A44  3C 60 80 0E */	lis r3, evStartEMarioRunAway__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCB08 000D9A48  38 A3 2A 5C */	addi r5, r3, evStartEMarioRunAway__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCB0C 000D9A4C  38 7F 00 00 */	addi r3, r31, 0
/* 800DCB10 000D9A50  38 9E 03 F4 */	addi r4, r30, 0x3f4
/* 800DCB14 000D9A54  48 03 EE F5 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCB18 000D9A58  3C 60 80 0E */	lis r3, evStartEMarioGateDrawing__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCB1C 000D9A5C  38 A3 28 98 */	addi r5, r3, evStartEMarioGateDrawing__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCB20 000D9A60  38 7F 00 00 */	addi r3, r31, 0
/* 800DCB24 000D9A64  38 9E 04 08 */	addi r4, r30, 0x408
/* 800DCB28 000D9A68  48 03 EE E1 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCB2C 000D9A6C  3C 60 80 0E */	lis r3, evStartEMarioDisappear__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCB30 000D9A70  38 A3 26 D4 */	addi r5, r3, evStartEMarioDisappear__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCB34 000D9A74  38 7F 00 00 */	addi r3, r31, 0
/* 800DCB38 000D9A78  38 9E 04 20 */	addi r4, r30, 0x420
/* 800DCB3C 000D9A7C  48 03 EE CD */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCB40 000D9A80  3C 60 80 0E */	lis r3, evStartOpenModelGate__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCB44 000D9A84  38 A3 25 10 */	addi r5, r3, evStartOpenModelGate__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCB48 000D9A88  38 7F 00 00 */	addi r3, r31, 0
/* 800DCB4C 000D9A8C  38 9E 04 38 */	addi r4, r30, 0x438
/* 800DCB50 000D9A90  48 03 EE B9 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCB54 000D9A94  3C 60 80 0E */	lis r3, evIsMapEventFinishedAll__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCB58 000D9A98  38 A3 23 3C */	addi r5, r3, evIsMapEventFinishedAll__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCB5C 000D9A9C  38 7F 00 00 */	addi r3, r31, 0
/* 800DCB60 000D9AA0  38 9E 04 4C */	addi r4, r30, 0x44c
/* 800DCB64 000D9AA4  48 03 EE A5 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCB68 000D9AA8  3C 60 80 0E */	lis r3, evRaiseBuilding__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCB6C 000D9AAC  38 A3 21 AC */	addi r5, r3, evRaiseBuilding__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCB70 000D9AB0  38 7F 00 00 */	addi r3, r31, 0
/* 800DCB74 000D9AB4  38 9E 04 64 */	addi r4, r30, 0x464
/* 800DCB78 000D9AB8  48 03 EE 91 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCB7C 000D9ABC  3C 60 80 0E */	lis r3, evForceCloseTalk__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCB80 000D9AC0  38 A3 21 14 */	addi r5, r3, evForceCloseTalk__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCB84 000D9AC4  38 7F 00 00 */	addi r3, r31, 0
/* 800DCB88 000D9AC8  38 9E 04 74 */	addi r4, r30, 0x474
/* 800DCB8C 000D9ACC  48 03 EE 7D */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCB90 000D9AD0  3C 60 80 0E */	lis r3, evInsertTimer__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCB94 000D9AD4  38 A3 1E E0 */	addi r5, r3, evInsertTimer__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCB98 000D9AD8  38 7F 00 00 */	addi r3, r31, 0
/* 800DCB9C 000D9ADC  38 9E 04 84 */	addi r4, r30, 0x484
/* 800DCBA0 000D9AE0  48 03 EE 69 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCBA4 000D9AE4  3C 60 80 0E */	lis r3, evStartTimer__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCBA8 000D9AE8  38 A3 1D 74 */	addi r5, r3, evStartTimer__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCBAC 000D9AEC  38 7F 00 00 */	addi r3, r31, 0
/* 800DCBB0 000D9AF0  38 9E 04 90 */	addi r4, r30, 0x490
/* 800DCBB4 000D9AF4  48 03 EE 55 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCBB8 000D9AF8  3C 60 80 0E */	lis r3, evStartMonteman__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCBBC 000D9AFC  38 A3 1B E8 */	addi r5, r3, evStartMonteman__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCBC0 000D9B00  38 7F 00 00 */	addi r3, r31, 0
/* 800DCBC4 000D9B04  38 9E 04 9C */	addi r4, r30, 0x49c
/* 800DCBC8 000D9B08  48 03 EE 41 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCBCC 000D9B0C  3C 60 80 0E */	lis r3, evStopTimer__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCBD0 000D9B10  38 A3 1B 4C */	addi r5, r3, evStopTimer__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCBD4 000D9B14  38 7F 00 00 */	addi r3, r31, 0
/* 800DCBD8 000D9B18  38 9E 04 AC */	addi r4, r30, 0x4ac
/* 800DCBDC 000D9B1C  48 03 EE 2D */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCBE0 000D9B20  3C 60 80 0E */	lis r3, evMonteManReachFlag__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCBE4 000D9B24  38 A3 1A 60 */	addi r5, r3, evMonteManReachFlag__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCBE8 000D9B28  38 7F 00 00 */	addi r3, r31, 0
/* 800DCBEC 000D9B2C  38 9E 04 B8 */	addi r4, r30, 0x4b8
/* 800DCBF0 000D9B30  48 03 EE 19 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCBF4 000D9B34  3C 60 80 0E */	lis r3, evGetTime__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCBF8 000D9B38  38 A3 19 C4 */	addi r5, r3, evGetTime__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCBFC 000D9B3C  38 7F 00 00 */	addi r3, r31, 0
/* 800DCC00 000D9B40  38 82 8F B0 */	addi r4, r2, $$23756@sda21
/* 800DCC04 000D9B44  48 03 EE 05 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCC08 000D9B48  3C 60 80 0E */	lis r3, evKillShine__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCC0C 000D9B4C  38 A3 17 F4 */	addi r5, r3, evKillShine__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCC10 000D9B50  38 7F 00 00 */	addi r3, r31, 0
/* 800DCC14 000D9B54  38 9E 04 CC */	addi r4, r30, 0x4cc
/* 800DCC18 000D9B58  48 03 ED F1 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCC1C 000D9B5C  3C 60 80 0E */	lis r3, evKillMushroom1up__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCC20 000D9B60  38 A3 15 F8 */	addi r5, r3, evKillMushroom1up__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCC24 000D9B64  38 7F 00 00 */	addi r3, r31, 0
/* 800DCC28 000D9B68  38 9E 04 D8 */	addi r4, r30, 0x4d8
/* 800DCC2C 000D9B6C  48 03 ED DD */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCC30 000D9B70  3C 60 80 0E */	lis r3, evAppearMushroom1up__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCC34 000D9B74  38 A3 13 D4 */	addi r5, r3, evAppearMushroom1up__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCC38 000D9B78  38 7F 00 00 */	addi r3, r31, 0
/* 800DCC3C 000D9B7C  38 9E 04 E8 */	addi r4, r30, 0x4e8
/* 800DCC40 000D9B80  48 03 ED C9 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCC44 000D9B84  3C 60 80 0E */	lis r3, evAppearShineFromNPC__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCC48 000D9B88  38 A3 10 70 */	addi r5, r3, evAppearShineFromNPC__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCC4C 000D9B8C  38 7F 00 00 */	addi r3, r31, 0
/* 800DCC50 000D9B90  38 9E 04 FC */	addi r4, r30, 0x4fc
/* 800DCC54 000D9B94  48 03 ED B5 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCC58 000D9B98  3C 60 80 0E */	lis r3, evAppearShineFromNPCWithoutDemo__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCC5C 000D9B9C  38 A3 0B B8 */	addi r5, r3, evAppearShineFromNPCWithoutDemo__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCC60 000D9BA0  38 7F 00 00 */	addi r3, r31, 0
/* 800DCC64 000D9BA4  38 9E 05 10 */	addi r4, r30, 0x510
/* 800DCC68 000D9BA8  48 03 ED A1 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCC6C 000D9BAC  3C 60 80 0E */	lis r3, evAppearShineFromKageMario__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCC70 000D9BB0  38 A3 08 FC */	addi r5, r3, evAppearShineFromKageMario__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCC74 000D9BB4  38 7F 00 00 */	addi r3, r31, 0
/* 800DCC78 000D9BB8  38 9E 05 30 */	addi r4, r30, 0x530
/* 800DCC7C 000D9BBC  48 03 ED 8D */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCC80 000D9BC0  3C 60 80 0E */	lis r3, evAppearShine__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCC84 000D9BC4  38 A3 0E 48 */	addi r5, r3, evAppearShine__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCC88 000D9BC8  38 7F 00 00 */	addi r3, r31, 0
/* 800DCC8C 000D9BCC  38 9E 05 4C */	addi r4, r30, 0x54c
/* 800DCC90 000D9BD0  48 03 ED 79 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCC94 000D9BD4  3C 60 80 0E */	lis r3, evAppearShineForWoodBox__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCC98 000D9BD8  38 A3 07 90 */	addi r5, r3, evAppearShineForWoodBox__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCC9C 000D9BDC  38 7F 00 00 */	addi r3, r31, 0
/* 800DCCA0 000D9BE0  38 9E 05 58 */	addi r4, r30, 0x558
/* 800DCCA4 000D9BE4  48 03 ED 65 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCCA8 000D9BE8  3C 60 80 0E */	lis r3, evChangeNozzle__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCCAC 000D9BEC  38 A3 06 34 */	addi r5, r3, evChangeNozzle__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCCB0 000D9BF0  38 7F 00 00 */	addi r3, r31, 0
/* 800DCCB4 000D9BF4  38 9E 05 70 */	addi r4, r30, 0x570
/* 800DCCB8 000D9BF8  48 03 ED 51 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCCBC 000D9BFC  3C 60 80 0E */	lis r3, evStartMarioTalking__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCCC0 000D9C00  38 A3 05 9C */	addi r5, r3, evStartMarioTalking__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCCC4 000D9C04  38 7F 00 00 */	addi r3, r31, 0
/* 800DCCC8 000D9C08  38 9E 05 80 */	addi r4, r30, 0x580
/* 800DCCCC 000D9C0C  48 03 ED 3D */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCCD0 000D9C10  3C 60 80 0E */	lis r3, evIsInsideCube__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCCD4 000D9C14  38 A3 FC 24 */	addi r5, r3, evIsInsideCube__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCCD8 000D9C18  38 7F 00 00 */	addi r3, r31, 0
/* 800DCCDC 000D9C1C  38 9E 05 94 */	addi r4, r30, 0x594
/* 800DCCE0 000D9C20  48 03 ED 29 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCCE4 000D9C24  3C 60 80 0E */	lis r3, evSetMarioWaiting__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCCE8 000D9C28  38 A3 FB 7C */	addi r5, r3, evSetMarioWaiting__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCCEC 000D9C2C  38 7F 00 00 */	addi r3, r31, 0
/* 800DCCF0 000D9C30  38 9E 05 A4 */	addi r4, r30, 0x5a4
/* 800DCCF4 000D9C34  48 03 ED 15 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCCF8 000D9C38  3C 60 80 0E */	lis r3, evSetTransScale__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCCFC 000D9C3C  38 A3 F0 44 */	addi r5, r3, evSetTransScale__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCD00 000D9C40  38 7F 00 00 */	addi r3, r31, 0
/* 800DCD04 000D9C44  38 9E 05 B4 */	addi r4, r30, 0x5b4
/* 800DCD08 000D9C48  48 03 ED 01 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCD0C 000D9C4C  3C 60 80 0E */	lis r3, evSetEventID__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCD10 000D9C50  38 A3 ED EC */	addi r5, r3, evSetEventID__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCD14 000D9C54  38 7F 00 00 */	addi r3, r31, 0
/* 800DCD18 000D9C58  38 9E 05 C4 */	addi r4, r30, 0x5c4
/* 800DCD1C 000D9C5C  48 03 EC ED */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCD20 000D9C60  3C 60 80 0E */	lis r3, evStartBGM__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCD24 000D9C64  38 A3 EC 18 */	addi r5, r3, evStartBGM__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCD28 000D9C68  38 7F 00 00 */	addi r3, r31, 0
/* 800DCD2C 000D9C6C  38 9E 05 D0 */	addi r4, r30, 0x5d0
/* 800DCD30 000D9C70  48 03 EC D9 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCD34 000D9C74  3C 60 80 0E */	lis r3, evStopBGM__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCD38 000D9C78  38 A3 E7 3C */	addi r5, r3, evStopBGM__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCD3C 000D9C7C  38 7F 00 00 */	addi r3, r31, 0
/* 800DCD40 000D9C80  38 82 8F B8 */	addi r4, r2, $$23772@sda21
/* 800DCD44 000D9C84  48 03 EC C5 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCD48 000D9C88  3C 60 80 0E */	lis r3, evStartMiss__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCD4C 000D9C8C  38 A3 E3 B8 */	addi r5, r3, evStartMiss__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCD50 000D9C90  38 7F 00 00 */	addi r3, r31, 0
/* 800DCD54 000D9C94  38 9E 05 DC */	addi r4, r30, 0x5dc
/* 800DCD58 000D9C98  48 03 EC B1 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCD5C 000D9C9C  3C 60 80 0E */	lis r3, evStartSE__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCD60 000D9CA0  38 A3 E5 D8 */	addi r5, r3, evStartSE__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCD64 000D9CA4  38 7F 00 00 */	addi r3, r31, 0
/* 800DCD68 000D9CA8  38 82 8F C0 */	addi r4, r2, $$23774@sda21
/* 800DCD6C 000D9CAC  48 03 EC 9D */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCD70 000D9CB0  3C 60 80 0E */	lis r3, evStartEventSE__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCD74 000D9CB4  38 A3 E4 50 */	addi r5, r3, evStartEventSE__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCD78 000D9CB8  38 7F 00 00 */	addi r3, r31, 0
/* 800DCD7C 000D9CBC  38 9E 05 E8 */	addi r4, r30, 0x5e8
/* 800DCD80 000D9CC0  48 03 EC 89 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCD84 000D9CC4  3C 60 80 0E */	lis r3, evChangeSunglass__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCD88 000D9CC8  38 A3 E1 C4 */	addi r5, r3, evChangeSunglass__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCD8C 000D9CCC  38 7F 00 00 */	addi r3, r31, 0
/* 800DCD90 000D9CD0  38 9E 05 F8 */	addi r4, r30, 0x5f8
/* 800DCD94 000D9CD4  48 03 EC 75 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCD98 000D9CD8  3C 60 80 0E */	lis r3, evSetCollision__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCD9C 000D9CDC  38 A3 DF 50 */	addi r5, r3, evSetCollision__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCDA0 000D9CE0  38 7F 00 00 */	addi r3, r31, 0
/* 800DCDA4 000D9CE4  38 9E 06 08 */	addi r4, r30, 0x608
/* 800DCDA8 000D9CE8  48 03 EC 61 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCDAC 000D9CEC  3C 60 80 0E */	lis r3, evWarpMario__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCDB0 000D9CF0  38 A3 DB D0 */	addi r5, r3, evWarpMario__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCDB4 000D9CF4  38 7F 00 00 */	addi r3, r31, 0
/* 800DCDB8 000D9CF8  38 9E 06 18 */	addi r4, r30, 0x618
/* 800DCDBC 000D9CFC  48 03 EC 4D */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCDC0 000D9D00  3C 60 80 0E */	lis r3, evStartAppearJetBalloon__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCDC4 000D9D04  38 A3 D9 74 */	addi r5, r3, evStartAppearJetBalloon__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCDC8 000D9D08  38 7F 00 00 */	addi r3, r31, 0
/* 800DCDCC 000D9D0C  38 9E 06 24 */	addi r4, r30, 0x624
/* 800DCDD0 000D9D10  48 03 EC 39 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCDD4 000D9D14  3C 60 80 0E */	lis r3, evAppear8RedCoinsAndTimer__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCDD8 000D9D18  38 A3 D5 28 */	addi r5, r3, evAppear8RedCoinsAndTimer__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCDDC 000D9D1C  38 7F 00 00 */	addi r3, r31, 0
/* 800DCDE0 000D9D20  38 9E 06 3C */	addi r4, r30, 0x63c
/* 800DCDE4 000D9D24  48 03 EC 25 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCDE8 000D9D28  3C 60 80 0E */	lis r3, evWarpFrontToMario__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCDEC 000D9D2C  38 A3 D2 D8 */	addi r5, r3, evWarpFrontToMario__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCDF0 000D9D30  38 7F 00 00 */	addi r3, r31, 0
/* 800DCDF4 000D9D34  38 9E 06 54 */	addi r4, r30, 0x654
/* 800DCDF8 000D9D38  48 03 EC 11 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCDFC 000D9D3C  3C 60 80 0E */	lis r3, evAppearReadyGo__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCE00 000D9D40  38 A3 D7 10 */	addi r5, r3, evAppearReadyGo__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCE04 000D9D44  38 7F 00 00 */	addi r3, r31, 0
/* 800DCE08 000D9D48  38 9E 06 68 */	addi r4, r30, 0x668
/* 800DCE0C 000D9D4C  48 03 EB FD */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCE10 000D9D50  3C 60 80 0E */	lis r3, evOnNeutralMarioKey__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCE14 000D9D54  38 A3 D2 38 */	addi r5, r3, evOnNeutralMarioKey__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCE18 000D9D58  38 7F 00 00 */	addi r3, r31, 0
/* 800DCE1C 000D9D5C  38 9E 06 78 */	addi r4, r30, 0x678
/* 800DCE20 000D9D60  48 03 EB E9 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCE24 000D9D64  3C 60 80 0E */	lis r3, evInvalidatePad__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCE28 000D9D68  38 A3 D0 F4 */	addi r5, r3, evInvalidatePad__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCE2C 000D9D6C  38 7F 00 00 */	addi r3, r31, 0
/* 800DCE30 000D9D70  38 9E 06 8C */	addi r4, r30, 0x68c
/* 800DCE34 000D9D74  48 03 EB D5 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCE38 000D9D78  3C 60 80 0E */	lis r3, evCheckWoodBox__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCE3C 000D9D7C  38 A3 02 F0 */	addi r5, r3, evCheckWoodBox__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCE40 000D9D80  38 7F 00 00 */	addi r3, r31, 0
/* 800DCE44 000D9D84  38 9E 06 9C */	addi r4, r30, 0x69c
/* 800DCE48 000D9D88  48 03 EB C1 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCE4C 000D9D8C  3C 60 80 0E */	lis r3, evRefreshWoodBox__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCE50 000D9D90  38 A3 00 4C */	addi r5, r3, evRefreshWoodBox__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCE54 000D9D94  38 7F 00 00 */	addi r3, r31, 0
/* 800DCE58 000D9D98  38 9E 06 AC */	addi r4, r30, 0x6ac
/* 800DCE5C 000D9D9C  48 03 EB AD */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCE60 000D9DA0  3C 60 80 0E */	lis r3, evKillWoodBox__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCE64 000D9DA4  38 A3 FD A8 */	addi r5, r3, evKillWoodBox__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCE68 000D9DA8  38 7F 00 00 */	addi r3, r31, 0
/* 800DCE6C 000D9DAC  38 9E 06 BC */	addi r4, r30, 0x6bc
/* 800DCE70 000D9DB0  48 03 EB 99 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCE74 000D9DB4  3C 60 80 0E */	lis r3, evManiCoinDown__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCE78 000D9DB8  38 A3 ED 50 */	addi r5, r3, evManiCoinDown__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCE7C 000D9DBC  38 7F 00 00 */	addi r3, r31, 0
/* 800DCE80 000D9DC0  38 9E 06 C8 */	addi r4, r30, 0x6c8
/* 800DCE84 000D9DC4  48 03 EB 85 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCE88 000D9DC8  3C 60 80 0E */	lis r3, evEggYoshiStartFruit__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCE8C 000D9DCC  38 A3 EA 48 */	addi r5, r3, evEggYoshiStartFruit__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCE90 000D9DD0  38 7F 00 00 */	addi r3, r31, 0
/* 800DCE94 000D9DD4  38 9E 06 DC */	addi r4, r30, 0x6dc
/* 800DCE98 000D9DD8  48 03 EB 71 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCE9C 000D9DDC  3C 60 80 0E */	lis r3, evPutNozzle__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCEA0 000D9DE0  38 A3 E8 78 */	addi r5, r3, evPutNozzle__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCEA4 000D9DE4  38 7F 00 00 */	addi r3, r31, 0
/* 800DCEA8 000D9DE8  38 9E 06 F0 */	addi r4, r30, 0x6f0
/* 800DCEAC 000D9DEC  48 03 EB 5D */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCEB0 000D9DF0  3C 60 80 0E */	lis r3, evStartMareBottleDemo__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCEB4 000D9DF4  38 A3 FA 6C */	addi r5, r3, evStartMareBottleDemo__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCEB8 000D9DF8  38 7F 00 00 */	addi r3, r31, 0
/* 800DCEBC 000D9DFC  38 9E 06 FC */	addi r4, r30, 0x6fc
/* 800DCEC0 000D9E00  48 03 EB 49 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCEC4 000D9E04  3C 60 80 0E */	lis r3, evIsFinishMareBottleDemo__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCEC8 000D9E08  38 A3 F9 78 */	addi r5, r3, evIsFinishMareBottleDemo__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCECC 000D9E0C  38 7F 00 00 */	addi r3, r31, 0
/* 800DCED0 000D9E10  38 9E 07 10 */	addi r4, r30, 0x710
/* 800DCED4 000D9E14  48 03 EB 35 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCED8 000D9E18  3C 60 80 0E */	lis r3, evIsInsideFastCube__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCEDC 000D9E1C  38 A3 F6 B8 */	addi r5, r3, evIsInsideFastCube__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCEE0 000D9E20  38 7F 00 00 */	addi r3, r31, 0
/* 800DCEE4 000D9E24  38 9E 07 28 */	addi r4, r30, 0x728
/* 800DCEE8 000D9E28  48 03 EB 21 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCEEC 000D9E2C  3C 60 80 0E */	lis r3, evSetEventForWaterMelon__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCEF0 000D9E30  38 A3 D7 B0 */	addi r5, r3, evSetEventForWaterMelon__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCEF4 000D9E34  38 7F 00 00 */	addi r3, r31, 0
/* 800DCEF8 000D9E38  38 9E 07 3C */	addi r4, r30, 0x73c
/* 800DCEFC 000D9E3C  48 03 EB 0D */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCF00 000D9E40  3C 60 80 0E */	lis r3, evIsWaterMelonIsReached__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@ha
/* 800DCF04 000D9E44  38 A3 CF 34 */	addi r5, r3, evIsWaterMelonIsReached__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul@l
/* 800DCF08 000D9E48  38 7F 00 00 */	addi r3, r31, 0
/* 800DCF0C 000D9E4C  38 9E 07 54 */	addi r4, r30, 0x754
/* 800DCF10 000D9E50  48 03 EA F9 */	bl bindSystemDataToSymbol__10TSpcBinaryFPCcUl
/* 800DCF14 000D9E54  7F E3 FB 78 */	mr r3, r31
/* 800DCF18 000D9E58  48 09 61 C5 */	bl initNpcBuiltin__9TNpcEventFP32TSpcTypedBinary$$013TEventWatcher$$1
/* 800DCF1C 000D9E5C  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 800DCF20 000D9E60  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 800DCF24 000D9E64  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 800DCF28 000D9E68  7C 08 03 A6 */	mtlr r0
/* 800DCF2C 000D9E6C  38 21 00 18 */	addi r1, r1, 0x18
/* 800DCF30 000D9E70  4E 80 00 20 */	blr 

.global evIsWaterMelonIsReached__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evIsWaterMelonIsReached__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800DCF34 000D9E74  7C 08 02 A6 */	mflr r0
/* 800DCF38 000D9E78  90 01 00 04 */	stw r0, 4(r1)
/* 800DCF3C 000D9E7C  94 21 FF 98 */	stwu r1, -0x68(r1)
/* 800DCF40 000D9E80  93 E1 00 64 */	stw r31, 0x64(r1)
/* 800DCF44 000D9E84  7C 7F 1B 78 */	mr r31, r3
/* 800DCF48 000D9E88  38 A1 00 0C */	addi r5, r1, 0xc
/* 800DCF4C 000D9E8C  90 81 00 0C */	stw r4, 0xc(r1)
/* 800DCF50 000D9E90  38 80 00 01 */	li r4, 1
/* 800DCF54 000D9E94  48 03 A4 DD */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800DCF58 000D9E98  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800DCF5C 000D9E9C  2C 03 00 00 */	cmpwi r3, 0
/* 800DCF60 000D9EA0  41 81 00 38 */	bgt lbl_800DCF98
/* 800DCF64 000D9EA4  3C 60 80 37 */	lis r3, $$23829@ha
/* 800DCF68 000D9EA8  4C C6 31 82 */	crclr 6
/* 800DCF6C 000D9EAC  38 63 30 FC */	addi r3, r3, $$23829@l
/* 800DCF70 000D9EB0  48 00 01 35 */	bl SpcTrace__FPCce
/* 800DCF74 000D9EB4  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800DCF78 000D9EB8  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800DCF7C 000D9EBC  54 00 18 38 */	slwi r0, r0, 3
/* 800DCF80 000D9EC0  7C 63 02 14 */	add r3, r3, r0
/* 800DCF84 000D9EC4  80 03 00 00 */	lwz r0, 0(r3)
/* 800DCF88 000D9EC8  90 01 00 40 */	stw r0, 0x40(r1)
/* 800DCF8C 000D9ECC  80 03 00 04 */	lwz r0, 4(r3)
/* 800DCF90 000D9ED0  90 01 00 44 */	stw r0, 0x44(r1)
/* 800DCF94 000D9ED4  48 00 00 2C */	b lbl_800DCFC0
lbl_800DCF98:
/* 800DCF98 000D9ED8  38 03 FF FF */	addi r0, r3, -1
/* 800DCF9C 000D9EDC  90 1F 00 1C */	stw r0, 0x1c(r31)
/* 800DCFA0 000D9EE0  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800DCFA4 000D9EE4  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800DCFA8 000D9EE8  54 00 18 38 */	slwi r0, r0, 3
/* 800DCFAC 000D9EEC  7C 63 02 14 */	add r3, r3, r0
/* 800DCFB0 000D9EF0  80 03 00 00 */	lwz r0, 0(r3)
/* 800DCFB4 000D9EF4  90 01 00 40 */	stw r0, 0x40(r1)
/* 800DCFB8 000D9EF8  80 03 00 04 */	lwz r0, 4(r3)
/* 800DCFBC 000D9EFC  90 01 00 44 */	stw r0, 0x44(r1)
lbl_800DCFC0:
/* 800DCFC0 000D9F00  80 01 00 40 */	lwz r0, 0x40(r1)
/* 800DCFC4 000D9F04  2C 00 00 01 */	cmpwi r0, 1
/* 800DCFC8 000D9F08  41 82 00 1C */	beq lbl_800DCFE4
/* 800DCFCC 000D9F0C  40 80 00 2C */	bge lbl_800DCFF8
/* 800DCFD0 000D9F10  2C 00 00 00 */	cmpwi r0, 0
/* 800DCFD4 000D9F14  40 80 00 08 */	bge lbl_800DCFDC
/* 800DCFD8 000D9F18  48 00 00 20 */	b lbl_800DCFF8
lbl_800DCFDC:
/* 800DCFDC 000D9F1C  80 61 00 44 */	lwz r3, 0x44(r1)
/* 800DCFE0 000D9F20  48 00 00 1C */	b lbl_800DCFFC
lbl_800DCFE4:
/* 800DCFE4 000D9F24  C0 01 00 44 */	lfs f0, 0x44(r1)
/* 800DCFE8 000D9F28  FC 00 00 1E */	fctiwz f0, f0
/* 800DCFEC 000D9F2C  D8 01 00 58 */	stfd f0, 0x58(r1)
/* 800DCFF0 000D9F30  80 61 00 5C */	lwz r3, 0x5c(r1)
/* 800DCFF4 000D9F34  48 00 00 08 */	b lbl_800DCFFC
lbl_800DCFF8:
/* 800DCFF8 000D9F38  38 60 00 00 */	li r3, 0
lbl_800DCFFC:
/* 800DCFFC 000D9F3C  C0 62 8F C8 */	lfs f3, $$23830@sda21(r2)
/* 800DD000 000D9F40  38 80 00 00 */	li r4, 0
/* 800DD004 000D9F44  C0 23 00 10 */	lfs f1, 0x10(r3)
/* 800DD008 000D9F48  C0 42 8F CC */	lfs f2, $$23831@sda21(r2)
/* 800DD00C 000D9F4C  C0 03 00 18 */	lfs f0, 0x18(r3)
/* 800DD010 000D9F50  EC 63 08 28 */	fsubs f3, f3, f1
/* 800DD014 000D9F54  C0 22 8F D0 */	lfs f1, $$23832@sda21(r2)
/* 800DD018 000D9F58  EC 42 00 28 */	fsubs f2, f2, f0
/* 800DD01C 000D9F5C  C0 02 8F D4 */	lfs f0, $$23833@sda21(r2)
/* 800DD020 000D9F60  EC 23 08 FA */	fmadds f1, f3, f3, f1
/* 800DD024 000D9F64  EC 22 08 BA */	fmadds f1, f2, f2, f1
/* 800DD028 000D9F68  FC 01 00 40 */	fcmpo cr0, f1, f0
/* 800DD02C 000D9F6C  4C 40 13 82 */	cror 2, 0, 2
/* 800DD030 000D9F70  40 82 00 08 */	bne lbl_800DD038
/* 800DD034 000D9F74  38 80 00 01 */	li r4, 1
lbl_800DD038:
/* 800DD038 000D9F78  38 00 00 00 */	li r0, 0
/* 800DD03C 000D9F7C  90 01 00 2C */	stw r0, 0x2c(r1)
/* 800DD040 000D9F80  90 81 00 30 */	stw r4, 0x30(r1)
/* 800DD044 000D9F84  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800DD048 000D9F88  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800DD04C 000D9F8C  7C 03 00 00 */	cmpw r3, r0
/* 800DD050 000D9F90  41 80 00 18 */	blt lbl_800DD068
/* 800DD054 000D9F94  3C 60 80 37 */	lis r3, $$23834@ha
/* 800DD058 000D9F98  4C C6 31 82 */	crclr 6
/* 800DD05C 000D9F9C  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800DD060 000D9FA0  48 00 00 45 */	bl SpcTrace__FPCce
/* 800DD064 000D9FA4  48 00 00 2C */	b lbl_800DD090
lbl_800DD068:
/* 800DD068 000D9FA8  80 BF 00 20 */	lwz r5, 0x20(r31)
/* 800DD06C 000D9FAC  54 64 18 38 */	slwi r4, r3, 3
/* 800DD070 000D9FB0  80 61 00 2C */	lwz r3, 0x2c(r1)
/* 800DD074 000D9FB4  80 01 00 30 */	lwz r0, 0x30(r1)
/* 800DD078 000D9FB8  7C 85 22 14 */	add r4, r5, r4
/* 800DD07C 000D9FBC  90 64 00 00 */	stw r3, 0(r4)
/* 800DD080 000D9FC0  90 04 00 04 */	stw r0, 4(r4)
/* 800DD084 000D9FC4  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800DD088 000D9FC8  38 03 00 01 */	addi r0, r3, 1
/* 800DD08C 000D9FCC  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800DD090:
/* 800DD090 000D9FD0  80 01 00 6C */	lwz r0, 0x6c(r1)
/* 800DD094 000D9FD4  83 E1 00 64 */	lwz r31, 0x64(r1)
/* 800DD098 000D9FD8  38 21 00 68 */	addi r1, r1, 0x68
/* 800DD09C 000D9FDC  7C 08 03 A6 */	mtlr r0
/* 800DD0A0 000D9FE0  4E 80 00 20 */	blr 

.global SpcTrace__FPCce
SpcTrace__FPCce:
/* 800DD0A4 000D9FE4  94 21 FF 90 */	stwu r1, -0x70(r1)
/* 800DD0A8 000D9FE8  40 86 00 24 */	bne cr1, lbl_800DD0CC
/* 800DD0AC 000D9FEC  D8 21 00 28 */	stfd f1, 0x28(r1)
/* 800DD0B0 000D9FF0  D8 41 00 30 */	stfd f2, 0x30(r1)
/* 800DD0B4 000D9FF4  D8 61 00 38 */	stfd f3, 0x38(r1)
/* 800DD0B8 000D9FF8  D8 81 00 40 */	stfd f4, 0x40(r1)
/* 800DD0BC 000D9FFC  D8 A1 00 48 */	stfd f5, 0x48(r1)
/* 800DD0C0 000DA000  D8 C1 00 50 */	stfd f6, 0x50(r1)
/* 800DD0C4 000DA004  D8 E1 00 58 */	stfd f7, 0x58(r1)
/* 800DD0C8 000DA008  D9 01 00 60 */	stfd f8, 0x60(r1)
lbl_800DD0CC:
/* 800DD0CC 000DA00C  90 61 00 08 */	stw r3, 8(r1)
/* 800DD0D0 000DA010  90 81 00 0C */	stw r4, 0xc(r1)
/* 800DD0D4 000DA014  90 A1 00 10 */	stw r5, 0x10(r1)
/* 800DD0D8 000DA018  90 C1 00 14 */	stw r6, 0x14(r1)
/* 800DD0DC 000DA01C  90 E1 00 18 */	stw r7, 0x18(r1)
/* 800DD0E0 000DA020  91 01 00 1C */	stw r8, 0x1c(r1)
/* 800DD0E4 000DA024  91 21 00 20 */	stw r9, 0x20(r1)
/* 800DD0E8 000DA028  91 41 00 24 */	stw r10, 0x24(r1)
/* 800DD0EC 000DA02C  38 21 00 70 */	addi r1, r1, 0x70
/* 800DD0F0 000DA030  4E 80 00 20 */	blr 

.global evInvalidatePad__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evInvalidatePad__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800DD0F4 000DA034  7C 08 02 A6 */	mflr r0
/* 800DD0F8 000DA038  90 01 00 04 */	stw r0, 4(r1)
/* 800DD0FC 000DA03C  94 21 FF 98 */	stwu r1, -0x68(r1)
/* 800DD100 000DA040  93 E1 00 64 */	stw r31, 0x64(r1)
/* 800DD104 000DA044  7C 7F 1B 78 */	mr r31, r3
/* 800DD108 000DA048  38 A1 00 0C */	addi r5, r1, 0xc
/* 800DD10C 000DA04C  90 81 00 0C */	stw r4, 0xc(r1)
/* 800DD110 000DA050  38 80 00 01 */	li r4, 1
/* 800DD114 000DA054  48 03 A3 1D */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800DD118 000DA058  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800DD11C 000DA05C  2C 03 00 00 */	cmpwi r3, 0
/* 800DD120 000DA060  41 81 00 38 */	bgt lbl_800DD158
/* 800DD124 000DA064  3C 60 80 37 */	lis r3, $$23829@ha
/* 800DD128 000DA068  4C C6 31 82 */	crclr 6
/* 800DD12C 000DA06C  38 63 30 FC */	addi r3, r3, $$23829@l
/* 800DD130 000DA070  4B FF FF 75 */	bl SpcTrace__FPCce
/* 800DD134 000DA074  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800DD138 000DA078  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800DD13C 000DA07C  54 00 18 38 */	slwi r0, r0, 3
/* 800DD140 000DA080  7C 63 02 14 */	add r3, r3, r0
/* 800DD144 000DA084  80 03 00 00 */	lwz r0, 0(r3)
/* 800DD148 000DA088  90 01 00 48 */	stw r0, 0x48(r1)
/* 800DD14C 000DA08C  80 03 00 04 */	lwz r0, 4(r3)
/* 800DD150 000DA090  90 01 00 4C */	stw r0, 0x4c(r1)
/* 800DD154 000DA094  48 00 00 2C */	b lbl_800DD180
lbl_800DD158:
/* 800DD158 000DA098  38 03 FF FF */	addi r0, r3, -1
/* 800DD15C 000DA09C  90 1F 00 1C */	stw r0, 0x1c(r31)
/* 800DD160 000DA0A0  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800DD164 000DA0A4  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800DD168 000DA0A8  54 00 18 38 */	slwi r0, r0, 3
/* 800DD16C 000DA0AC  7C 63 02 14 */	add r3, r3, r0
/* 800DD170 000DA0B0  80 03 00 00 */	lwz r0, 0(r3)
/* 800DD174 000DA0B4  90 01 00 48 */	stw r0, 0x48(r1)
/* 800DD178 000DA0B8  80 03 00 04 */	lwz r0, 4(r3)
/* 800DD17C 000DA0BC  90 01 00 4C */	stw r0, 0x4c(r1)
lbl_800DD180:
/* 800DD180 000DA0C0  80 01 00 48 */	lwz r0, 0x48(r1)
/* 800DD184 000DA0C4  2C 00 00 01 */	cmpwi r0, 1
/* 800DD188 000DA0C8  41 82 00 1C */	beq lbl_800DD1A4
/* 800DD18C 000DA0CC  40 80 00 2C */	bge lbl_800DD1B8
/* 800DD190 000DA0D0  2C 00 00 00 */	cmpwi r0, 0
/* 800DD194 000DA0D4  40 80 00 08 */	bge lbl_800DD19C
/* 800DD198 000DA0D8  48 00 00 20 */	b lbl_800DD1B8
lbl_800DD19C:
/* 800DD19C 000DA0DC  80 81 00 4C */	lwz r4, 0x4c(r1)
/* 800DD1A0 000DA0E0  48 00 00 1C */	b lbl_800DD1BC
lbl_800DD1A4:
/* 800DD1A4 000DA0E4  C0 01 00 4C */	lfs f0, 0x4c(r1)
/* 800DD1A8 000DA0E8  FC 00 00 1E */	fctiwz f0, f0
/* 800DD1AC 000DA0EC  D8 01 00 58 */	stfd f0, 0x58(r1)
/* 800DD1B0 000DA0F0  80 81 00 5C */	lwz r4, 0x5c(r1)
/* 800DD1B4 000DA0F4  48 00 00 08 */	b lbl_800DD1BC
lbl_800DD1B8:
/* 800DD1B8 000DA0F8  38 80 00 00 */	li r4, 0
lbl_800DD1BC:
/* 800DD1BC 000DA0FC  80 6D 97 E8 */	lwz r3, gpMarDirector@sda21(r13)
/* 800DD1C0 000DA100  38 00 00 00 */	li r0, 0
/* 800DD1C4 000DA104  80 63 00 18 */	lwz r3, 0x18(r3)
/* 800DD1C8 000DA108  80 63 00 00 */	lwz r3, 0(r3)
/* 800DD1CC 000DA10C  90 83 00 E8 */	stw r4, 0xe8(r3)
/* 800DD1D0 000DA110  90 01 00 38 */	stw r0, 0x38(r1)
/* 800DD1D4 000DA114  90 01 00 3C */	stw r0, 0x3c(r1)
/* 800DD1D8 000DA118  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800DD1DC 000DA11C  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800DD1E0 000DA120  7C 03 00 00 */	cmpw r3, r0
/* 800DD1E4 000DA124  41 80 00 18 */	blt lbl_800DD1FC
/* 800DD1E8 000DA128  3C 60 80 37 */	lis r3, $$23834@ha
/* 800DD1EC 000DA12C  4C C6 31 82 */	crclr 6
/* 800DD1F0 000DA130  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800DD1F4 000DA134  4B FF FE B1 */	bl SpcTrace__FPCce
/* 800DD1F8 000DA138  48 00 00 2C */	b lbl_800DD224
lbl_800DD1FC:
/* 800DD1FC 000DA13C  80 BF 00 20 */	lwz r5, 0x20(r31)
/* 800DD200 000DA140  54 64 18 38 */	slwi r4, r3, 3
/* 800DD204 000DA144  80 61 00 38 */	lwz r3, 0x38(r1)
/* 800DD208 000DA148  80 01 00 3C */	lwz r0, 0x3c(r1)
/* 800DD20C 000DA14C  7C 85 22 14 */	add r4, r5, r4
/* 800DD210 000DA150  90 64 00 00 */	stw r3, 0(r4)
/* 800DD214 000DA154  90 04 00 04 */	stw r0, 4(r4)
/* 800DD218 000DA158  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800DD21C 000DA15C  38 03 00 01 */	addi r0, r3, 1
/* 800DD220 000DA160  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800DD224:
/* 800DD224 000DA164  80 01 00 6C */	lwz r0, 0x6c(r1)
/* 800DD228 000DA168  83 E1 00 64 */	lwz r31, 0x64(r1)
/* 800DD22C 000DA16C  38 21 00 68 */	addi r1, r1, 0x68
/* 800DD230 000DA170  7C 08 03 A6 */	mtlr r0
/* 800DD234 000DA174  4E 80 00 20 */	blr 

.global evOnNeutralMarioKey__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evOnNeutralMarioKey__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800DD238 000DA178  7C 08 02 A6 */	mflr r0
/* 800DD23C 000DA17C  90 01 00 04 */	stw r0, 4(r1)
/* 800DD240 000DA180  94 21 FF D0 */	stwu r1, -0x30(r1)
/* 800DD244 000DA184  93 E1 00 2C */	stw r31, 0x2c(r1)
/* 800DD248 000DA188  7C 7F 1B 78 */	mr r31, r3
/* 800DD24C 000DA18C  38 A1 00 0C */	addi r5, r1, 0xc
/* 800DD250 000DA190  90 81 00 0C */	stw r4, 0xc(r1)
/* 800DD254 000DA194  38 80 00 00 */	li r4, 0
/* 800DD258 000DA198  48 03 A1 D9 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800DD25C 000DA19C  80 6D 97 E8 */	lwz r3, gpMarDirector@sda21(r13)
/* 800DD260 000DA1A0  80 63 00 18 */	lwz r3, 0x18(r3)
/* 800DD264 000DA1A4  80 63 00 00 */	lwz r3, 0(r3)
/* 800DD268 000DA1A8  48 01 E8 71 */	bl onNeutralMarioKey__13TMarioGamePadFv
/* 800DD26C 000DA1AC  38 00 00 00 */	li r0, 0
/* 800DD270 000DA1B0  90 01 00 1C */	stw r0, 0x1c(r1)
/* 800DD274 000DA1B4  90 01 00 20 */	stw r0, 0x20(r1)
/* 800DD278 000DA1B8  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800DD27C 000DA1BC  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800DD280 000DA1C0  7C 03 00 00 */	cmpw r3, r0
/* 800DD284 000DA1C4  41 80 00 18 */	blt lbl_800DD29C
/* 800DD288 000DA1C8  3C 60 80 37 */	lis r3, $$23834@ha
/* 800DD28C 000DA1CC  4C C6 31 82 */	crclr 6
/* 800DD290 000DA1D0  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800DD294 000DA1D4  4B FF FE 11 */	bl SpcTrace__FPCce
/* 800DD298 000DA1D8  48 00 00 2C */	b lbl_800DD2C4
lbl_800DD29C:
/* 800DD29C 000DA1DC  80 BF 00 20 */	lwz r5, 0x20(r31)
/* 800DD2A0 000DA1E0  54 64 18 38 */	slwi r4, r3, 3
/* 800DD2A4 000DA1E4  80 61 00 1C */	lwz r3, 0x1c(r1)
/* 800DD2A8 000DA1E8  80 01 00 20 */	lwz r0, 0x20(r1)
/* 800DD2AC 000DA1EC  7C 85 22 14 */	add r4, r5, r4
/* 800DD2B0 000DA1F0  90 64 00 00 */	stw r3, 0(r4)
/* 800DD2B4 000DA1F4  90 04 00 04 */	stw r0, 4(r4)
/* 800DD2B8 000DA1F8  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800DD2BC 000DA1FC  38 03 00 01 */	addi r0, r3, 1
/* 800DD2C0 000DA200  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800DD2C4:
/* 800DD2C4 000DA204  80 01 00 34 */	lwz r0, 0x34(r1)
/* 800DD2C8 000DA208  83 E1 00 2C */	lwz r31, 0x2c(r1)
/* 800DD2CC 000DA20C  38 21 00 30 */	addi r1, r1, 0x30
/* 800DD2D0 000DA210  7C 08 03 A6 */	mtlr r0
/* 800DD2D4 000DA214  4E 80 00 20 */	blr 

.global evWarpFrontToMario__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evWarpFrontToMario__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800DD2D8 000DA218  7C 08 02 A6 */	mflr r0
/* 800DD2DC 000DA21C  90 01 00 04 */	stw r0, 4(r1)
/* 800DD2E0 000DA220  94 21 FF 50 */	stwu r1, -0xb0(r1)
/* 800DD2E4 000DA224  93 E1 00 AC */	stw r31, 0xac(r1)
/* 800DD2E8 000DA228  38 A1 00 0C */	addi r5, r1, 0xc
/* 800DD2EC 000DA22C  93 C1 00 A8 */	stw r30, 0xa8(r1)
/* 800DD2F0 000DA230  7C 7E 1B 78 */	mr r30, r3
/* 800DD2F4 000DA234  93 A1 00 A4 */	stw r29, 0xa4(r1)
/* 800DD2F8 000DA238  90 81 00 0C */	stw r4, 0xc(r1)
/* 800DD2FC 000DA23C  38 80 00 01 */	li r4, 1
/* 800DD300 000DA240  48 03 A1 31 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800DD304 000DA244  80 7E 00 1C */	lwz r3, 0x1c(r30)
/* 800DD308 000DA248  2C 03 00 00 */	cmpwi r3, 0
/* 800DD30C 000DA24C  41 81 00 38 */	bgt lbl_800DD344
/* 800DD310 000DA250  3C 60 80 37 */	lis r3, $$23829@ha
/* 800DD314 000DA254  4C C6 31 82 */	crclr 6
/* 800DD318 000DA258  38 63 30 FC */	addi r3, r3, $$23829@l
/* 800DD31C 000DA25C  4B FF FD 89 */	bl SpcTrace__FPCce
/* 800DD320 000DA260  80 1E 00 1C */	lwz r0, 0x1c(r30)
/* 800DD324 000DA264  80 7E 00 20 */	lwz r3, 0x20(r30)
/* 800DD328 000DA268  54 00 18 38 */	slwi r0, r0, 3
/* 800DD32C 000DA26C  7C 63 02 14 */	add r3, r3, r0
/* 800DD330 000DA270  80 03 00 00 */	lwz r0, 0(r3)
/* 800DD334 000DA274  90 01 00 7C */	stw r0, 0x7c(r1)
/* 800DD338 000DA278  80 03 00 04 */	lwz r0, 4(r3)
/* 800DD33C 000DA27C  90 01 00 80 */	stw r0, 0x80(r1)
/* 800DD340 000DA280  48 00 00 2C */	b lbl_800DD36C
lbl_800DD344:
/* 800DD344 000DA284  38 03 FF FF */	addi r0, r3, -1
/* 800DD348 000DA288  90 1E 00 1C */	stw r0, 0x1c(r30)
/* 800DD34C 000DA28C  80 1E 00 1C */	lwz r0, 0x1c(r30)
/* 800DD350 000DA290  80 7E 00 20 */	lwz r3, 0x20(r30)
/* 800DD354 000DA294  54 00 18 38 */	slwi r0, r0, 3
/* 800DD358 000DA298  7C 63 02 14 */	add r3, r3, r0
/* 800DD35C 000DA29C  80 03 00 00 */	lwz r0, 0(r3)
/* 800DD360 000DA2A0  90 01 00 7C */	stw r0, 0x7c(r1)
/* 800DD364 000DA2A4  80 03 00 04 */	lwz r0, 4(r3)
/* 800DD368 000DA2A8  90 01 00 80 */	stw r0, 0x80(r1)
lbl_800DD36C:
/* 800DD36C 000DA2AC  80 01 00 7C */	lwz r0, 0x7c(r1)
/* 800DD370 000DA2B0  2C 00 00 01 */	cmpwi r0, 1
/* 800DD374 000DA2B4  41 82 00 1C */	beq lbl_800DD390
/* 800DD378 000DA2B8  40 80 00 2C */	bge lbl_800DD3A4
/* 800DD37C 000DA2BC  2C 00 00 00 */	cmpwi r0, 0
/* 800DD380 000DA2C0  40 80 00 08 */	bge lbl_800DD388
/* 800DD384 000DA2C4  48 00 00 20 */	b lbl_800DD3A4
lbl_800DD388:
/* 800DD388 000DA2C8  83 E1 00 80 */	lwz r31, 0x80(r1)
/* 800DD38C 000DA2CC  48 00 00 1C */	b lbl_800DD3A8
lbl_800DD390:
/* 800DD390 000DA2D0  C0 01 00 80 */	lfs f0, 0x80(r1)
/* 800DD394 000DA2D4  FC 00 00 1E */	fctiwz f0, f0
/* 800DD398 000DA2D8  D8 01 00 98 */	stfd f0, 0x98(r1)
/* 800DD39C 000DA2DC  83 E1 00 9C */	lwz r31, 0x9c(r1)
/* 800DD3A0 000DA2E0  48 00 00 08 */	b lbl_800DD3A8
lbl_800DD3A4:
/* 800DD3A4 000DA2E4  3B E0 00 00 */	li r31, 0
lbl_800DD3A8:
/* 800DD3A8 000DA2E8  80 AD 98 E4 */	lwz r5, gpMarioAngleY@sda21(r13)
/* 800DD3AC 000DA2EC  38 61 00 64 */	addi r3, r1, 0x64
/* 800DD3B0 000DA2F0  C0 22 8F D0 */	lfs f1, $$23832@sda21(r2)
/* 800DD3B4 000DA2F4  38 81 00 84 */	addi r4, r1, 0x84
/* 800DD3B8 000DA2F8  AB A5 00 00 */	lha r29, 0(r5)
/* 800DD3BC 000DA2FC  C0 02 8F D8 */	lfs f0, $$23931@sda21(r2)
/* 800DD3C0 000DA300  D0 21 00 84 */	stfs f1, 0x84(r1)
/* 800DD3C4 000DA304  D0 21 00 88 */	stfs f1, 0x88(r1)
/* 800DD3C8 000DA308  D0 01 00 8C */	stfs f0, 0x8c(r1)
/* 800DD3CC 000DA30C  A0 C5 00 00 */	lhz r6, 0(r5)
/* 800DD3D0 000DA310  80 AD 8D EC */	lwz r5, jmaSinShift@sda21(r13)
/* 800DD3D4 000DA314  80 0D 8D F0 */	lwz r0, jmaSinTable@sda21(r13)
/* 800DD3D8 000DA318  7C C5 2E 30 */	sraw r5, r6, r5
/* 800DD3DC 000DA31C  C0 81 00 84 */	lfs f4, 0x84(r1)
/* 800DD3E0 000DA320  54 A6 10 3A */	slwi r6, r5, 2
/* 800DD3E4 000DA324  80 AD 8D F4 */	lwz r5, jmaCosTable@sda21(r13)
/* 800DD3E8 000DA328  7C E0 32 14 */	add r7, r0, r6
/* 800DD3EC 000DA32C  C0 21 00 8C */	lfs f1, 0x8c(r1)
/* 800DD3F0 000DA330  C0 07 00 00 */	lfs f0, 0(r7)
/* 800DD3F4 000DA334  7C A5 32 14 */	add r5, r5, r6
/* 800DD3F8 000DA338  C0 45 00 00 */	lfs f2, 0(r5)
/* 800DD3FC 000DA33C  FC 60 20 50 */	fneg f3, f4
/* 800DD400 000DA340  EC 01 00 32 */	fmuls f0, f1, f0
/* 800DD404 000DA344  EC 04 00 BA */	fmadds f0, f4, f2, f0
/* 800DD408 000DA348  D0 01 00 84 */	stfs f0, 0x84(r1)
/* 800DD40C 000DA34C  C0 21 00 8C */	lfs f1, 0x8c(r1)
/* 800DD410 000DA350  C0 05 00 00 */	lfs f0, 0(r5)
/* 800DD414 000DA354  C0 47 00 00 */	lfs f2, 0(r7)
/* 800DD418 000DA358  EC 01 00 32 */	fmuls f0, f1, f0
/* 800DD41C 000DA35C  EC 03 00 BA */	fmadds f0, f3, f2, f0
/* 800DD420 000DA360  D0 01 00 8C */	stfs f0, 0x8c(r1)
/* 800DD424 000DA364  48 00 00 E9 */	bl set__Q29JGeometry8TVec3$$0f$$1FRC3Vec
/* 800DD428 000DA368  80 CD 98 DC */	lwz r6, gpMarioPos@sda21(r13)
/* 800DD42C 000DA36C  38 61 00 44 */	addi r3, r1, 0x44
/* 800DD430 000DA370  38 81 00 64 */	addi r4, r1, 0x64
/* 800DD434 000DA374  80 A6 00 00 */	lwz r5, 0(r6)
/* 800DD438 000DA378  80 06 00 04 */	lwz r0, 4(r6)
/* 800DD43C 000DA37C  90 A1 00 44 */	stw r5, 0x44(r1)
/* 800DD440 000DA380  90 01 00 48 */	stw r0, 0x48(r1)
/* 800DD444 000DA384  80 06 00 08 */	lwz r0, 8(r6)
/* 800DD448 000DA388  90 01 00 4C */	stw r0, 0x4c(r1)
/* 800DD44C 000DA38C  4B FE 4E BD */	bl add__Q29JGeometry8TVec3$$0f$$1FRCQ29JGeometry8TVec3$$0f$$1
/* 800DD450 000DA390  80 61 00 44 */	lwz r3, 0x44(r1)
/* 800DD454 000DA394  38 1D 80 00 */	addi r0, r29, -32768
/* 800DD458 000DA398  80 A1 00 48 */	lwz r5, 0x48(r1)
/* 800DD45C 000DA39C  7C 00 07 34 */	extsh r0, r0
/* 800DD460 000DA3A0  6C 04 80 00 */	xoris r4, r0, 0x8000
/* 800DD464 000DA3A4  90 7F 00 10 */	stw r3, 0x10(r31)
/* 800DD468 000DA3A8  3C 60 43 30 */	lis r3, 0x4330
/* 800DD46C 000DA3AC  38 00 00 00 */	li r0, 0
/* 800DD470 000DA3B0  90 BF 00 14 */	stw r5, 0x14(r31)
/* 800DD474 000DA3B4  90 81 00 9C */	stw r4, 0x9c(r1)
/* 800DD478 000DA3B8  80 81 00 4C */	lwz r4, 0x4c(r1)
/* 800DD47C 000DA3BC  90 61 00 98 */	stw r3, 0x98(r1)
/* 800DD480 000DA3C0  90 9F 00 18 */	stw r4, 0x18(r31)
/* 800DD484 000DA3C4  C8 01 00 98 */	lfd f0, 0x98(r1)
/* 800DD488 000DA3C8  C8 22 8F E0 */	lfd f1, $$23934@sda21(r2)
/* 800DD48C 000DA3CC  C0 42 8F DC */	lfs f2, $$23932@sda21(r2)
/* 800DD490 000DA3D0  EC 00 08 28 */	fsubs f0, f0, f1
/* 800DD494 000DA3D4  EC 02 00 32 */	fmuls f0, f2, f0
/* 800DD498 000DA3D8  D0 1F 00 34 */	stfs f0, 0x34(r31)
/* 800DD49C 000DA3DC  90 01 00 54 */	stw r0, 0x54(r1)
/* 800DD4A0 000DA3E0  90 01 00 58 */	stw r0, 0x58(r1)
/* 800DD4A4 000DA3E4  80 7E 00 1C */	lwz r3, 0x1c(r30)
/* 800DD4A8 000DA3E8  80 1E 00 18 */	lwz r0, 0x18(r30)
/* 800DD4AC 000DA3EC  7C 03 00 00 */	cmpw r3, r0
/* 800DD4B0 000DA3F0  41 80 00 18 */	blt lbl_800DD4C8
/* 800DD4B4 000DA3F4  3C 60 80 37 */	lis r3, $$23834@ha
/* 800DD4B8 000DA3F8  4C C6 31 82 */	crclr 6
/* 800DD4BC 000DA3FC  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800DD4C0 000DA400  4B FF FB E5 */	bl SpcTrace__FPCce
/* 800DD4C4 000DA404  48 00 00 2C */	b lbl_800DD4F0
lbl_800DD4C8:
/* 800DD4C8 000DA408  80 BE 00 20 */	lwz r5, 0x20(r30)
/* 800DD4CC 000DA40C  54 64 18 38 */	slwi r4, r3, 3
/* 800DD4D0 000DA410  80 61 00 54 */	lwz r3, 0x54(r1)
/* 800DD4D4 000DA414  80 01 00 58 */	lwz r0, 0x58(r1)
/* 800DD4D8 000DA418  7C 85 22 14 */	add r4, r5, r4
/* 800DD4DC 000DA41C  90 64 00 00 */	stw r3, 0(r4)
/* 800DD4E0 000DA420  90 04 00 04 */	stw r0, 4(r4)
/* 800DD4E4 000DA424  80 7E 00 1C */	lwz r3, 0x1c(r30)
/* 800DD4E8 000DA428  38 03 00 01 */	addi r0, r3, 1
/* 800DD4EC 000DA42C  90 1E 00 1C */	stw r0, 0x1c(r30)
lbl_800DD4F0:
/* 800DD4F0 000DA430  80 01 00 B4 */	lwz r0, 0xb4(r1)
/* 800DD4F4 000DA434  83 E1 00 AC */	lwz r31, 0xac(r1)
/* 800DD4F8 000DA438  83 C1 00 A8 */	lwz r30, 0xa8(r1)
/* 800DD4FC 000DA43C  7C 08 03 A6 */	mtlr r0
/* 800DD500 000DA440  83 A1 00 A4 */	lwz r29, 0xa4(r1)
/* 800DD504 000DA444  38 21 00 B0 */	addi r1, r1, 0xb0
/* 800DD508 000DA448  4E 80 00 20 */	blr 

.global set__Q29JGeometry8TVec3$$0f$$1FRC3Vec
set__Q29JGeometry8TVec3$$0f$$1FRC3Vec:
/* 800DD50C 000DA44C  C0 04 00 00 */	lfs f0, 0(r4)
/* 800DD510 000DA450  D0 03 00 00 */	stfs f0, 0(r3)
/* 800DD514 000DA454  C0 04 00 04 */	lfs f0, 4(r4)
/* 800DD518 000DA458  D0 03 00 04 */	stfs f0, 4(r3)
/* 800DD51C 000DA45C  C0 04 00 08 */	lfs f0, 8(r4)
/* 800DD520 000DA460  D0 03 00 08 */	stfs f0, 8(r3)
/* 800DD524 000DA464  4E 80 00 20 */	blr 

.global evAppear8RedCoinsAndTimer__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evAppear8RedCoinsAndTimer__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800DD528 000DA468  7C 08 02 A6 */	mflr r0
/* 800DD52C 000DA46C  90 01 00 04 */	stw r0, 4(r1)
/* 800DD530 000DA470  94 21 FF 60 */	stwu r1, -0xa0(r1)
/* 800DD534 000DA474  DB E1 00 98 */	stfd f31, 0x98(r1)
/* 800DD538 000DA478  38 A1 00 0C */	addi r5, r1, 0xc
/* 800DD53C 000DA47C  BF 61 00 84 */	stmw r27, 0x84(r1)
/* 800DD540 000DA480  7C 7F 1B 78 */	mr r31, r3
/* 800DD544 000DA484  90 81 00 0C */	stw r4, 0xc(r1)
/* 800DD548 000DA488  38 80 00 00 */	li r4, 0
/* 800DD54C 000DA48C  48 03 9E E5 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800DD550 000DA490  80 6D 8E E0 */	lwz r3, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 800DD554 000DA494  3C 80 80 37 */	lis r4, $$23988@ha
/* 800DD558 000DA498  38 04 31 38 */	addi r0, r4, $$23988@l
/* 800DD55C 000DA49C  7C 1D 03 78 */	mr r29, r0
/* 800DD560 000DA4A0  83 C3 00 04 */	lwz r30, 4(r3)
/* 800DD564 000DA4A4  7F A3 EB 78 */	mr r3, r29
/* 800DD568 000DA4A8  4B F6 71 25 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800DD56C 000DA4AC  81 9E 00 00 */	lwz r12, 0(r30)
/* 800DD570 000DA4B0  38 83 00 00 */	addi r4, r3, 0
/* 800DD574 000DA4B4  38 7E 00 00 */	addi r3, r30, 0
/* 800DD578 000DA4B8  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 800DD57C 000DA4BC  7F A5 EB 78 */	mr r5, r29
/* 800DD580 000DA4C0  7D 88 03 A6 */	mtlr r12
/* 800DD584 000DA4C4  4E 80 00 21 */	blrl 
/* 800DD588 000DA4C8  83 83 01 38 */	lwz r28, 0x138(r3)
/* 800DD58C 000DA4CC  3B 60 00 00 */	li r27, 0
/* 800DD590 000DA4D0  C3 E2 8F E8 */	lfs f31, $$23989@sda21(r2)
/* 800DD594 000DA4D4  3F C0 20 00 */	lis r30, 0x2000000F@ha
lbl_800DD598:
/* 800DD598 000DA4D8  80 6D 9A 18 */	lwz r3, gpItemManager@sda21(r13)
/* 800DD59C 000DA4DC  38 9E 00 0F */	addi r4, r30, 0x2000000F@l
/* 800DD5A0 000DA4E0  48 0B 19 21 */	bl makeObjAppeared__18TMapObjBaseManagerFUl
/* 800DD5A4 000DA4E4  7C 7D 1B 78 */	mr r29, r3
/* 800DD5A8 000DA4E8  81 9D 00 00 */	lwz r12, 0(r29)
/* 800DD5AC 000DA4EC  80 1D 01 50 */	lwz r0, 0x150(r29)
/* 800DD5B0 000DA4F0  81 8C 01 DC */	lwz r12, 0x1dc(r12)
/* 800DD5B4 000DA4F4  7C 80 E0 50 */	subf r4, r0, r28
/* 800DD5B8 000DA4F8  7D 88 03 A6 */	mtlr r12
/* 800DD5BC 000DA4FC  4E 80 00 21 */	blrl 
/* 800DD5C0 000DA500  C0 3D 00 14 */	lfs f1, 0x14(r29)
/* 800DD5C4 000DA504  7F A3 EB 78 */	mr r3, r29
/* 800DD5C8 000DA508  C0 5D 00 18 */	lfs f2, 0x18(r29)
/* 800DD5CC 000DA50C  C0 1D 00 10 */	lfs f0, 0x10(r29)
/* 800DD5D0 000DA510  EC 3F 08 2A */	fadds f1, f31, f1
/* 800DD5D4 000DA514  D0 1D 01 58 */	stfs f0, 0x158(r29)
/* 800DD5D8 000DA518  D0 3D 01 5C */	stfs f1, 0x15c(r29)
/* 800DD5DC 000DA51C  D0 5D 01 60 */	stfs f2, 0x160(r29)
/* 800DD5E0 000DA520  48 03 4C 91 */	bl getModel__10TLiveActorCFv
/* 800DD5E4 000DA524  80 A3 00 58 */	lwz r5, 0x58(r3)
/* 800DD5E8 000DA528  7F A7 EB 78 */	mr r7, r29
/* 800DD5EC 000DA52C  80 6D 97 98 */	lwz r3, gpMarioParticleManager@sda21(r13)
/* 800DD5F0 000DA530  38 80 00 58 */	li r4, 0x58
/* 800DD5F4 000DA534  38 C0 00 00 */	li r6, 0
/* 800DD5F8 000DA538  4B FF DF 95 */	bl emitAndBindToMtxPtr__21TMarioParticleManagerFlPA4_fUcPCv
/* 800DD5FC 000DA53C  3B BD 01 58 */	addi r29, r29, 0x158
/* 800DD600 000DA540  80 6D 97 98 */	lwz r3, gpMarioParticleManager@sda21(r13)
/* 800DD604 000DA544  38 BD 00 00 */	addi r5, r29, 0
/* 800DD608 000DA548  38 80 00 E5 */	li r4, 0xe5
/* 800DD60C 000DA54C  38 C0 00 00 */	li r6, 0
/* 800DD610 000DA550  38 E0 00 00 */	li r7, 0
/* 800DD614 000DA554  4B FF E5 09 */	bl emit__21TMarioParticleManagerFlPCQ29JGeometry8TVec3$$0f$$1UcPCv
/* 800DD618 000DA558  80 6D 97 98 */	lwz r3, gpMarioParticleManager@sda21(r13)
/* 800DD61C 000DA55C  38 BD 00 00 */	addi r5, r29, 0
/* 800DD620 000DA560  38 80 00 E6 */	li r4, 0xe6
/* 800DD624 000DA564  38 C0 00 00 */	li r6, 0
/* 800DD628 000DA568  38 E0 00 00 */	li r7, 0
/* 800DD62C 000DA56C  4B FF E4 F1 */	bl emit__21TMarioParticleManagerFlPCQ29JGeometry8TVec3$$0f$$1UcPCv
/* 800DD630 000DA570  3B 7B 00 01 */	addi r27, r27, 1
/* 800DD634 000DA574  2C 1B 00 08 */	cmpwi r27, 8
/* 800DD638 000DA578  41 80 FF 60 */	blt lbl_800DD598
/* 800DD63C 000DA57C  6F 80 80 00 */	xoris r0, r28, 0x8000
/* 800DD640 000DA580  80 6D 97 E8 */	lwz r3, gpMarDirector@sda21(r13)
/* 800DD644 000DA584  90 01 00 7C */	stw r0, 0x7c(r1)
/* 800DD648 000DA588  3C 00 43 30 */	lis r0, 0x4330
/* 800DD64C 000DA58C  C8 22 8F E0 */	lfd f1, $$23934@sda21(r2)
/* 800DD650 000DA590  38 80 00 01 */	li r4, 1
/* 800DD654 000DA594  90 01 00 78 */	stw r0, 0x78(r1)
/* 800DD658 000DA598  C0 42 8F EC */	lfs f2, $$23990@sda21(r2)
/* 800DD65C 000DA59C  C8 01 00 78 */	lfd f0, 0x78(r1)
/* 800DD660 000DA5A0  80 63 00 74 */	lwz r3, 0x74(r3)
/* 800DD664 000DA5A4  EC 00 08 28 */	fsubs f0, f0, f1
/* 800DD668 000DA5A8  EC 02 00 32 */	fmuls f0, f2, f0
/* 800DD66C 000DA5AC  FC 00 00 1E */	fctiwz f0, f0
/* 800DD670 000DA5B0  D8 01 00 70 */	stfd f0, 0x70(r1)
/* 800DD674 000DA5B4  80 A1 00 74 */	lwz r5, 0x74(r1)
/* 800DD678 000DA5B8  48 12 ED B1 */	bl startAppearTimer__11TGCConsole2Fil
/* 800DD67C 000DA5BC  83 AD 97 E8 */	lwz r29, gpMarDirector@sda21(r13)
/* 800DD680 000DA5C0  38 7D 00 E8 */	addi r3, r29, 0xe8
/* 800DD684 000DA5C4  4B FB 53 B1 */	bl OSCheckStopwatch
/* 800DD688 000DA5C8  90 9D 00 CC */	stw r4, 0xcc(r29)
/* 800DD68C 000DA5CC  38 80 00 0A */	li r4, 0xa
/* 800DD690 000DA5D0  90 7D 00 C8 */	stw r3, 0xc8(r29)
/* 800DD694 000DA5D4  80 6D 97 E8 */	lwz r3, gpMarDirector@sda21(r13)
/* 800DD698 000DA5D8  80 63 00 74 */	lwz r3, 0x74(r3)
/* 800DD69C 000DA5DC  48 12 C7 65 */	bl startMoveTimer__11TGCConsole2Fi
/* 800DD6A0 000DA5E0  38 00 00 00 */	li r0, 0
/* 800DD6A4 000DA5E4  90 01 00 54 */	stw r0, 0x54(r1)
/* 800DD6A8 000DA5E8  90 01 00 58 */	stw r0, 0x58(r1)
/* 800DD6AC 000DA5EC  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800DD6B0 000DA5F0  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800DD6B4 000DA5F4  7C 03 00 00 */	cmpw r3, r0
/* 800DD6B8 000DA5F8  41 80 00 18 */	blt lbl_800DD6D0
/* 800DD6BC 000DA5FC  3C 60 80 37 */	lis r3, $$23834@ha
/* 800DD6C0 000DA600  4C C6 31 82 */	crclr 6
/* 800DD6C4 000DA604  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800DD6C8 000DA608  4B FF F9 DD */	bl SpcTrace__FPCce
/* 800DD6CC 000DA60C  48 00 00 2C */	b lbl_800DD6F8
lbl_800DD6D0:
/* 800DD6D0 000DA610  80 BF 00 20 */	lwz r5, 0x20(r31)
/* 800DD6D4 000DA614  54 64 18 38 */	slwi r4, r3, 3
/* 800DD6D8 000DA618  80 61 00 54 */	lwz r3, 0x54(r1)
/* 800DD6DC 000DA61C  80 01 00 58 */	lwz r0, 0x58(r1)
/* 800DD6E0 000DA620  7C 85 22 14 */	add r4, r5, r4
/* 800DD6E4 000DA624  90 64 00 00 */	stw r3, 0(r4)
/* 800DD6E8 000DA628  90 04 00 04 */	stw r0, 4(r4)
/* 800DD6EC 000DA62C  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800DD6F0 000DA630  38 03 00 01 */	addi r0, r3, 1
/* 800DD6F4 000DA634  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800DD6F8:
/* 800DD6F8 000DA638  BB 61 00 84 */	lmw r27, 0x84(r1)
/* 800DD6FC 000DA63C  80 01 00 A4 */	lwz r0, 0xa4(r1)
/* 800DD700 000DA640  CB E1 00 98 */	lfd f31, 0x98(r1)
/* 800DD704 000DA644  38 21 00 A0 */	addi r1, r1, 0xa0
/* 800DD708 000DA648  7C 08 03 A6 */	mtlr r0
/* 800DD70C 000DA64C  4E 80 00 20 */	blr 

.global evAppearReadyGo__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evAppearReadyGo__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800DD710 000DA650  7C 08 02 A6 */	mflr r0
/* 800DD714 000DA654  90 01 00 04 */	stw r0, 4(r1)
/* 800DD718 000DA658  94 21 FF C0 */	stwu r1, -0x40(r1)
/* 800DD71C 000DA65C  93 E1 00 3C */	stw r31, 0x3c(r1)
/* 800DD720 000DA660  7C 7F 1B 78 */	mr r31, r3
/* 800DD724 000DA664  38 A1 00 0C */	addi r5, r1, 0xc
/* 800DD728 000DA668  90 81 00 0C */	stw r4, 0xc(r1)
/* 800DD72C 000DA66C  38 80 00 00 */	li r4, 0
/* 800DD730 000DA670  48 03 9D 01 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800DD734 000DA674  80 6D 97 E8 */	lwz r3, gpMarDirector@sda21(r13)
/* 800DD738 000DA678  80 63 00 74 */	lwz r3, 0x74(r3)
/* 800DD73C 000DA67C  80 63 00 94 */	lwz r3, 0x94(r3)
/* 800DD740 000DA680  48 15 49 A5 */	bl startAppearReady__11TConsoleStrFv
/* 800DD744 000DA684  38 00 00 00 */	li r0, 0
/* 800DD748 000DA688  90 01 00 24 */	stw r0, 0x24(r1)
/* 800DD74C 000DA68C  90 01 00 28 */	stw r0, 0x28(r1)
/* 800DD750 000DA690  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800DD754 000DA694  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800DD758 000DA698  7C 03 00 00 */	cmpw r3, r0
/* 800DD75C 000DA69C  41 80 00 18 */	blt lbl_800DD774
/* 800DD760 000DA6A0  3C 60 80 37 */	lis r3, $$23834@ha
/* 800DD764 000DA6A4  4C C6 31 82 */	crclr 6
/* 800DD768 000DA6A8  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800DD76C 000DA6AC  4B FF F9 39 */	bl SpcTrace__FPCce
/* 800DD770 000DA6B0  48 00 00 2C */	b lbl_800DD79C
lbl_800DD774:
/* 800DD774 000DA6B4  80 BF 00 20 */	lwz r5, 0x20(r31)
/* 800DD778 000DA6B8  54 64 18 38 */	slwi r4, r3, 3
/* 800DD77C 000DA6BC  80 61 00 24 */	lwz r3, 0x24(r1)
/* 800DD780 000DA6C0  80 01 00 28 */	lwz r0, 0x28(r1)
/* 800DD784 000DA6C4  7C 85 22 14 */	add r4, r5, r4
/* 800DD788 000DA6C8  90 64 00 00 */	stw r3, 0(r4)
/* 800DD78C 000DA6CC  90 04 00 04 */	stw r0, 4(r4)
/* 800DD790 000DA6D0  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800DD794 000DA6D4  38 03 00 01 */	addi r0, r3, 1
/* 800DD798 000DA6D8  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800DD79C:
/* 800DD79C 000DA6DC  80 01 00 44 */	lwz r0, 0x44(r1)
/* 800DD7A0 000DA6E0  83 E1 00 3C */	lwz r31, 0x3c(r1)
/* 800DD7A4 000DA6E4  38 21 00 40 */	addi r1, r1, 0x40
/* 800DD7A8 000DA6E8  7C 08 03 A6 */	mtlr r0
/* 800DD7AC 000DA6EC  4E 80 00 20 */	blr 

.global evSetEventForWaterMelon__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evSetEventForWaterMelon__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800DD7B0 000DA6F0  7C 08 02 A6 */	mflr r0
/* 800DD7B4 000DA6F4  90 01 00 04 */	stw r0, 4(r1)
/* 800DD7B8 000DA6F8  94 21 FF 80 */	stwu r1, -0x80(r1)
/* 800DD7BC 000DA6FC  93 E1 00 7C */	stw r31, 0x7c(r1)
/* 800DD7C0 000DA700  7C 7F 1B 78 */	mr r31, r3
/* 800DD7C4 000DA704  38 A1 00 0C */	addi r5, r1, 0xc
/* 800DD7C8 000DA708  93 C1 00 78 */	stw r30, 0x78(r1)
/* 800DD7CC 000DA70C  93 A1 00 74 */	stw r29, 0x74(r1)
/* 800DD7D0 000DA710  90 81 00 0C */	stw r4, 0xc(r1)
/* 800DD7D4 000DA714  38 80 00 01 */	li r4, 1
/* 800DD7D8 000DA718  48 03 9C 59 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800DD7DC 000DA71C  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800DD7E0 000DA720  2C 03 00 00 */	cmpwi r3, 0
/* 800DD7E4 000DA724  41 81 00 38 */	bgt lbl_800DD81C
/* 800DD7E8 000DA728  3C 60 80 37 */	lis r3, $$23829@ha
/* 800DD7EC 000DA72C  4C C6 31 82 */	crclr 6
/* 800DD7F0 000DA730  38 63 30 FC */	addi r3, r3, $$23829@l
/* 800DD7F4 000DA734  4B FF F8 B1 */	bl SpcTrace__FPCce
/* 800DD7F8 000DA738  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800DD7FC 000DA73C  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800DD800 000DA740  54 00 18 38 */	slwi r0, r0, 3
/* 800DD804 000DA744  7C 63 02 14 */	add r3, r3, r0
/* 800DD808 000DA748  80 03 00 00 */	lwz r0, 0(r3)
/* 800DD80C 000DA74C  90 01 00 54 */	stw r0, 0x54(r1)
/* 800DD810 000DA750  80 03 00 04 */	lwz r0, 4(r3)
/* 800DD814 000DA754  90 01 00 58 */	stw r0, 0x58(r1)
/* 800DD818 000DA758  48 00 00 2C */	b lbl_800DD844
lbl_800DD81C:
/* 800DD81C 000DA75C  38 03 FF FF */	addi r0, r3, -1
/* 800DD820 000DA760  90 1F 00 1C */	stw r0, 0x1c(r31)
/* 800DD824 000DA764  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800DD828 000DA768  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800DD82C 000DA76C  54 00 18 38 */	slwi r0, r0, 3
/* 800DD830 000DA770  7C 63 02 14 */	add r3, r3, r0
/* 800DD834 000DA774  80 03 00 00 */	lwz r0, 0(r3)
/* 800DD838 000DA778  90 01 00 54 */	stw r0, 0x54(r1)
/* 800DD83C 000DA77C  80 03 00 04 */	lwz r0, 4(r3)
/* 800DD840 000DA780  90 01 00 58 */	stw r0, 0x58(r1)
lbl_800DD844:
/* 800DD844 000DA784  80 81 00 54 */	lwz r4, 0x54(r1)
/* 800DD848 000DA788  38 60 00 00 */	li r3, 0
/* 800DD84C 000DA78C  80 01 00 58 */	lwz r0, 0x58(r1)
/* 800DD850 000DA790  90 81 00 5C */	stw r4, 0x5c(r1)
/* 800DD854 000DA794  90 01 00 60 */	stw r0, 0x60(r1)
/* 800DD858 000DA798  80 01 00 5C */	lwz r0, 0x5c(r1)
/* 800DD85C 000DA79C  2C 00 00 01 */	cmpwi r0, 1
/* 800DD860 000DA7A0  41 82 00 9C */	beq lbl_800DD8FC
/* 800DD864 000DA7A4  40 80 00 10 */	bge lbl_800DD874
/* 800DD868 000DA7A8  2C 00 00 00 */	cmpwi r0, 0
/* 800DD86C 000DA7AC  40 80 00 58 */	bge lbl_800DD8C4
/* 800DD870 000DA7B0  48 00 00 8C */	b lbl_800DD8FC
lbl_800DD874:
/* 800DD874 000DA7B4  2C 00 00 03 */	cmpwi r0, 3
/* 800DD878 000DA7B8  40 80 00 84 */	bge lbl_800DD8FC
/* 800DD87C 000DA7BC  2C 00 00 02 */	cmpwi r0, 2
/* 800DD880 000DA7C0  41 82 00 08 */	beq lbl_800DD888
/* 800DD884 000DA7C4  48 00 00 0C */	b lbl_800DD890
lbl_800DD888:
/* 800DD888 000DA7C8  83 C1 00 60 */	lwz r30, 0x60(r1)
/* 800DD88C 000DA7CC  48 00 00 08 */	b lbl_800DD894
lbl_800DD890:
/* 800DD890 000DA7D0  3B C2 8F F0 */	addi r30, r2, $$24053@sda21
lbl_800DD894:
/* 800DD894 000DA7D4  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 800DD898 000DA7D8  7F C3 F3 78 */	mr r3, r30
/* 800DD89C 000DA7DC  83 A4 00 04 */	lwz r29, 4(r4)
/* 800DD8A0 000DA7E0  4B F6 6D ED */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800DD8A4 000DA7E4  81 9D 00 00 */	lwz r12, 0(r29)
/* 800DD8A8 000DA7E8  38 83 00 00 */	addi r4, r3, 0
/* 800DD8AC 000DA7EC  38 7D 00 00 */	addi r3, r29, 0
/* 800DD8B0 000DA7F0  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 800DD8B4 000DA7F4  7F C5 F3 78 */	mr r5, r30
/* 800DD8B8 000DA7F8  7D 88 03 A6 */	mtlr r12
/* 800DD8BC 000DA7FC  4E 80 00 21 */	blrl 
/* 800DD8C0 000DA800  48 00 00 3C */	b lbl_800DD8FC
lbl_800DD8C4:
/* 800DD8C4 000DA804  2C 00 00 01 */	cmpwi r0, 1
/* 800DD8C8 000DA808  41 82 00 1C */	beq lbl_800DD8E4
/* 800DD8CC 000DA80C  40 80 00 2C */	bge lbl_800DD8F8
/* 800DD8D0 000DA810  2C 00 00 00 */	cmpwi r0, 0
/* 800DD8D4 000DA814  40 80 00 08 */	bge lbl_800DD8DC
/* 800DD8D8 000DA818  48 00 00 20 */	b lbl_800DD8F8
lbl_800DD8DC:
/* 800DD8DC 000DA81C  80 61 00 60 */	lwz r3, 0x60(r1)
/* 800DD8E0 000DA820  48 00 00 1C */	b lbl_800DD8FC
lbl_800DD8E4:
/* 800DD8E4 000DA824  C0 01 00 60 */	lfs f0, 0x60(r1)
/* 800DD8E8 000DA828  FC 00 00 1E */	fctiwz f0, f0
/* 800DD8EC 000DA82C  D8 01 00 68 */	stfd f0, 0x68(r1)
/* 800DD8F0 000DA830  80 61 00 6C */	lwz r3, 0x6c(r1)
/* 800DD8F4 000DA834  48 00 00 08 */	b lbl_800DD8FC
lbl_800DD8F8:
/* 800DD8F8 000DA838  38 60 00 00 */	li r3, 0
lbl_800DD8FC:
/* 800DD8FC 000DA83C  48 0D AB 71 */	bl startEvent__14TBigWatermelonFv
/* 800DD900 000DA840  38 00 00 00 */	li r0, 0
/* 800DD904 000DA844  90 01 00 44 */	stw r0, 0x44(r1)
/* 800DD908 000DA848  90 01 00 48 */	stw r0, 0x48(r1)
/* 800DD90C 000DA84C  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800DD910 000DA850  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800DD914 000DA854  7C 03 00 00 */	cmpw r3, r0
/* 800DD918 000DA858  41 80 00 18 */	blt lbl_800DD930
/* 800DD91C 000DA85C  3C 60 80 37 */	lis r3, $$23834@ha
/* 800DD920 000DA860  4C C6 31 82 */	crclr 6
/* 800DD924 000DA864  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800DD928 000DA868  4B FF F7 7D */	bl SpcTrace__FPCce
/* 800DD92C 000DA86C  48 00 00 2C */	b lbl_800DD958
lbl_800DD930:
/* 800DD930 000DA870  80 BF 00 20 */	lwz r5, 0x20(r31)
/* 800DD934 000DA874  54 64 18 38 */	slwi r4, r3, 3
/* 800DD938 000DA878  80 61 00 44 */	lwz r3, 0x44(r1)
/* 800DD93C 000DA87C  80 01 00 48 */	lwz r0, 0x48(r1)
/* 800DD940 000DA880  7C 85 22 14 */	add r4, r5, r4
/* 800DD944 000DA884  90 64 00 00 */	stw r3, 0(r4)
/* 800DD948 000DA888  90 04 00 04 */	stw r0, 4(r4)
/* 800DD94C 000DA88C  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800DD950 000DA890  38 03 00 01 */	addi r0, r3, 1
/* 800DD954 000DA894  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800DD958:
/* 800DD958 000DA898  80 01 00 84 */	lwz r0, 0x84(r1)
/* 800DD95C 000DA89C  83 E1 00 7C */	lwz r31, 0x7c(r1)
/* 800DD960 000DA8A0  83 C1 00 78 */	lwz r30, 0x78(r1)
/* 800DD964 000DA8A4  7C 08 03 A6 */	mtlr r0
/* 800DD968 000DA8A8  83 A1 00 74 */	lwz r29, 0x74(r1)
/* 800DD96C 000DA8AC  38 21 00 80 */	addi r1, r1, 0x80
/* 800DD970 000DA8B0  4E 80 00 20 */	blr 

.global evStartAppearJetBalloon__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evStartAppearJetBalloon__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800DD974 000DA8B4  7C 08 02 A6 */	mflr r0
/* 800DD978 000DA8B8  90 01 00 04 */	stw r0, 4(r1)
/* 800DD97C 000DA8BC  94 21 FF 58 */	stwu r1, -0xa8(r1)
/* 800DD980 000DA8C0  93 E1 00 A4 */	stw r31, 0xa4(r1)
/* 800DD984 000DA8C4  38 A1 00 0C */	addi r5, r1, 0xc
/* 800DD988 000DA8C8  93 C1 00 A0 */	stw r30, 0xa0(r1)
/* 800DD98C 000DA8CC  7C 7E 1B 78 */	mr r30, r3
/* 800DD990 000DA8D0  93 A1 00 9C */	stw r29, 0x9c(r1)
/* 800DD994 000DA8D4  90 81 00 0C */	stw r4, 0xc(r1)
/* 800DD998 000DA8D8  3C 80 80 37 */	lis r4, $$21490@ha
/* 800DD99C 000DA8DC  3B E4 29 90 */	addi r31, r4, $$21490@l
/* 800DD9A0 000DA8E0  38 80 00 02 */	li r4, 2
/* 800DD9A4 000DA8E4  48 03 9A 8D */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800DD9A8 000DA8E8  80 7E 00 1C */	lwz r3, 0x1c(r30)
/* 800DD9AC 000DA8EC  2C 03 00 00 */	cmpwi r3, 0
/* 800DD9B0 000DA8F0  41 81 00 34 */	bgt lbl_800DD9E4
/* 800DD9B4 000DA8F4  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800DD9B8 000DA8F8  4C C6 31 82 */	crclr 6
/* 800DD9BC 000DA8FC  4B FF F6 E9 */	bl SpcTrace__FPCce
/* 800DD9C0 000DA900  80 1E 00 1C */	lwz r0, 0x1c(r30)
/* 800DD9C4 000DA904  80 7E 00 20 */	lwz r3, 0x20(r30)
/* 800DD9C8 000DA908  54 00 18 38 */	slwi r0, r0, 3
/* 800DD9CC 000DA90C  7C 63 02 14 */	add r3, r3, r0
/* 800DD9D0 000DA910  80 03 00 00 */	lwz r0, 0(r3)
/* 800DD9D4 000DA914  90 01 00 80 */	stw r0, 0x80(r1)
/* 800DD9D8 000DA918  80 03 00 04 */	lwz r0, 4(r3)
/* 800DD9DC 000DA91C  90 01 00 84 */	stw r0, 0x84(r1)
/* 800DD9E0 000DA920  48 00 00 2C */	b lbl_800DDA0C
lbl_800DD9E4:
/* 800DD9E4 000DA924  38 03 FF FF */	addi r0, r3, -1
/* 800DD9E8 000DA928  90 1E 00 1C */	stw r0, 0x1c(r30)
/* 800DD9EC 000DA92C  80 1E 00 1C */	lwz r0, 0x1c(r30)
/* 800DD9F0 000DA930  80 7E 00 20 */	lwz r3, 0x20(r30)
/* 800DD9F4 000DA934  54 00 18 38 */	slwi r0, r0, 3
/* 800DD9F8 000DA938  7C 63 02 14 */	add r3, r3, r0
/* 800DD9FC 000DA93C  80 03 00 00 */	lwz r0, 0(r3)
/* 800DDA00 000DA940  90 01 00 80 */	stw r0, 0x80(r1)
/* 800DDA04 000DA944  80 03 00 04 */	lwz r0, 4(r3)
/* 800DDA08 000DA948  90 01 00 84 */	stw r0, 0x84(r1)
lbl_800DDA0C:
/* 800DDA0C 000DA94C  80 01 00 80 */	lwz r0, 0x80(r1)
/* 800DDA10 000DA950  2C 00 00 01 */	cmpwi r0, 1
/* 800DDA14 000DA954  41 82 00 1C */	beq lbl_800DDA30
/* 800DDA18 000DA958  40 80 00 2C */	bge lbl_800DDA44
/* 800DDA1C 000DA95C  2C 00 00 00 */	cmpwi r0, 0
/* 800DDA20 000DA960  40 80 00 08 */	bge lbl_800DDA28
/* 800DDA24 000DA964  48 00 00 20 */	b lbl_800DDA44
lbl_800DDA28:
/* 800DDA28 000DA968  83 A1 00 84 */	lwz r29, 0x84(r1)
/* 800DDA2C 000DA96C  48 00 00 1C */	b lbl_800DDA48
lbl_800DDA30:
/* 800DDA30 000DA970  C0 01 00 84 */	lfs f0, 0x84(r1)
/* 800DDA34 000DA974  FC 00 00 1E */	fctiwz f0, f0
/* 800DDA38 000DA978  D8 01 00 90 */	stfd f0, 0x90(r1)
/* 800DDA3C 000DA97C  83 A1 00 94 */	lwz r29, 0x94(r1)
/* 800DDA40 000DA980  48 00 00 08 */	b lbl_800DDA48
lbl_800DDA44:
/* 800DDA44 000DA984  3B A0 00 00 */	li r29, 0
lbl_800DDA48:
/* 800DDA48 000DA988  80 7E 00 1C */	lwz r3, 0x1c(r30)
/* 800DDA4C 000DA98C  2C 03 00 00 */	cmpwi r3, 0
/* 800DDA50 000DA990  41 81 00 34 */	bgt lbl_800DDA84
/* 800DDA54 000DA994  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800DDA58 000DA998  4C C6 31 82 */	crclr 6
/* 800DDA5C 000DA99C  4B FF F6 49 */	bl SpcTrace__FPCce
/* 800DDA60 000DA9A0  80 1E 00 1C */	lwz r0, 0x1c(r30)
/* 800DDA64 000DA9A4  80 7E 00 20 */	lwz r3, 0x20(r30)
/* 800DDA68 000DA9A8  54 00 18 38 */	slwi r0, r0, 3
/* 800DDA6C 000DA9AC  7C 63 02 14 */	add r3, r3, r0
/* 800DDA70 000DA9B0  80 03 00 00 */	lwz r0, 0(r3)
/* 800DDA74 000DA9B4  90 01 00 78 */	stw r0, 0x78(r1)
/* 800DDA78 000DA9B8  80 03 00 04 */	lwz r0, 4(r3)
/* 800DDA7C 000DA9BC  90 01 00 7C */	stw r0, 0x7c(r1)
/* 800DDA80 000DA9C0  48 00 00 2C */	b lbl_800DDAAC
lbl_800DDA84:
/* 800DDA84 000DA9C4  38 03 FF FF */	addi r0, r3, -1
/* 800DDA88 000DA9C8  90 1E 00 1C */	stw r0, 0x1c(r30)
/* 800DDA8C 000DA9CC  80 1E 00 1C */	lwz r0, 0x1c(r30)
/* 800DDA90 000DA9D0  80 7E 00 20 */	lwz r3, 0x20(r30)
/* 800DDA94 000DA9D4  54 00 18 38 */	slwi r0, r0, 3
/* 800DDA98 000DA9D8  7C 63 02 14 */	add r3, r3, r0
/* 800DDA9C 000DA9DC  80 03 00 00 */	lwz r0, 0(r3)
/* 800DDAA0 000DA9E0  90 01 00 78 */	stw r0, 0x78(r1)
/* 800DDAA4 000DA9E4  80 03 00 04 */	lwz r0, 4(r3)
/* 800DDAA8 000DA9E8  90 01 00 7C */	stw r0, 0x7c(r1)
lbl_800DDAAC:
/* 800DDAAC 000DA9EC  80 01 00 78 */	lwz r0, 0x78(r1)
/* 800DDAB0 000DA9F0  2C 00 00 01 */	cmpwi r0, 1
/* 800DDAB4 000DA9F4  41 82 00 1C */	beq lbl_800DDAD0
/* 800DDAB8 000DA9F8  40 80 00 2C */	bge lbl_800DDAE4
/* 800DDABC 000DA9FC  2C 00 00 00 */	cmpwi r0, 0
/* 800DDAC0 000DAA00  40 80 00 08 */	bge lbl_800DDAC8
/* 800DDAC4 000DAA04  48 00 00 20 */	b lbl_800DDAE4
lbl_800DDAC8:
/* 800DDAC8 000DAA08  80 01 00 7C */	lwz r0, 0x7c(r1)
/* 800DDACC 000DAA0C  48 00 00 1C */	b lbl_800DDAE8
lbl_800DDAD0:
/* 800DDAD0 000DAA10  C0 01 00 7C */	lfs f0, 0x7c(r1)
/* 800DDAD4 000DAA14  FC 00 00 1E */	fctiwz f0, f0
/* 800DDAD8 000DAA18  D8 01 00 90 */	stfd f0, 0x90(r1)
/* 800DDADC 000DAA1C  80 01 00 94 */	lwz r0, 0x94(r1)
/* 800DDAE0 000DAA20  48 00 00 08 */	b lbl_800DDAE8
lbl_800DDAE4:
/* 800DDAE4 000DAA24  38 00 00 00 */	li r0, 0
lbl_800DDAE8:
/* 800DDAE8 000DAA28  2C 00 00 01 */	cmpwi r0, 1
/* 800DDAEC 000DAA2C  41 82 00 40 */	beq lbl_800DDB2C
/* 800DDAF0 000DAA30  40 80 00 10 */	bge lbl_800DDB00
/* 800DDAF4 000DAA34  2C 00 00 00 */	cmpwi r0, 0
/* 800DDAF8 000DAA38  40 80 00 14 */	bge lbl_800DDB0C
/* 800DDAFC 000DAA3C  48 00 00 64 */	b lbl_800DDB60
lbl_800DDB00:
/* 800DDB00 000DAA40  2C 00 00 03 */	cmpwi r0, 3
/* 800DDB04 000DAA44  40 80 00 5C */	bge lbl_800DDB60
/* 800DDB08 000DAA48  48 00 00 44 */	b lbl_800DDB4C
lbl_800DDB0C:
/* 800DDB0C 000DAA4C  2C 1D 00 01 */	cmpwi r29, 1
/* 800DDB10 000DAA50  40 82 00 50 */	bne lbl_800DDB60
/* 800DDB14 000DAA54  80 6D 97 E8 */	lwz r3, gpMarDirector@sda21(r13)
/* 800DDB18 000DAA58  38 80 00 00 */	li r4, 0
/* 800DDB1C 000DAA5C  38 A0 00 08 */	li r5, 8
/* 800DDB20 000DAA60  80 63 00 74 */	lwz r3, 0x74(r3)
/* 800DDB24 000DAA64  48 12 E6 3D */	bl startAppearJetBalloon__11TGCConsole2Fii
/* 800DDB28 000DAA68  48 00 00 38 */	b lbl_800DDB60
lbl_800DDB2C:
/* 800DDB2C 000DAA6C  2C 1D 00 01 */	cmpwi r29, 1
/* 800DDB30 000DAA70  40 82 00 30 */	bne lbl_800DDB60
/* 800DDB34 000DAA74  80 6D 97 E8 */	lwz r3, gpMarDirector@sda21(r13)
/* 800DDB38 000DAA78  38 80 00 01 */	li r4, 1
/* 800DDB3C 000DAA7C  38 A0 00 0A */	li r5, 0xa
/* 800DDB40 000DAA80  80 63 00 74 */	lwz r3, 0x74(r3)
/* 800DDB44 000DAA84  48 12 E6 1D */	bl startAppearJetBalloon__11TGCConsole2Fii
/* 800DDB48 000DAA88  48 00 00 18 */	b lbl_800DDB60
lbl_800DDB4C:
/* 800DDB4C 000DAA8C  2C 1D 00 01 */	cmpwi r29, 1
/* 800DDB50 000DAA90  40 82 00 10 */	bne lbl_800DDB60
/* 800DDB54 000DAA94  80 6D 97 E8 */	lwz r3, gpMarDirector@sda21(r13)
/* 800DDB58 000DAA98  80 63 00 74 */	lwz r3, 0x74(r3)
/* 800DDB5C 000DAA9C  48 12 E2 BD */	bl startAppearRedCoin__11TGCConsole2Fv
lbl_800DDB60:
/* 800DDB60 000DAAA0  38 00 00 00 */	li r0, 0
/* 800DDB64 000DAAA4  90 01 00 5C */	stw r0, 0x5c(r1)
/* 800DDB68 000DAAA8  90 01 00 60 */	stw r0, 0x60(r1)
/* 800DDB6C 000DAAAC  80 7E 00 1C */	lwz r3, 0x1c(r30)
/* 800DDB70 000DAAB0  80 1E 00 18 */	lwz r0, 0x18(r30)
/* 800DDB74 000DAAB4  7C 03 00 00 */	cmpw r3, r0
/* 800DDB78 000DAAB8  41 80 00 14 */	blt lbl_800DDB8C
/* 800DDB7C 000DAABC  38 7F 07 8C */	addi r3, r31, 0x78c
/* 800DDB80 000DAAC0  4C C6 31 82 */	crclr 6
/* 800DDB84 000DAAC4  4B FF F5 21 */	bl SpcTrace__FPCce
/* 800DDB88 000DAAC8  48 00 00 2C */	b lbl_800DDBB4
lbl_800DDB8C:
/* 800DDB8C 000DAACC  80 BE 00 20 */	lwz r5, 0x20(r30)
/* 800DDB90 000DAAD0  54 64 18 38 */	slwi r4, r3, 3
/* 800DDB94 000DAAD4  80 61 00 5C */	lwz r3, 0x5c(r1)
/* 800DDB98 000DAAD8  80 01 00 60 */	lwz r0, 0x60(r1)
/* 800DDB9C 000DAADC  7C 85 22 14 */	add r4, r5, r4
/* 800DDBA0 000DAAE0  90 64 00 00 */	stw r3, 0(r4)
/* 800DDBA4 000DAAE4  90 04 00 04 */	stw r0, 4(r4)
/* 800DDBA8 000DAAE8  80 7E 00 1C */	lwz r3, 0x1c(r30)
/* 800DDBAC 000DAAEC  38 03 00 01 */	addi r0, r3, 1
/* 800DDBB0 000DAAF0  90 1E 00 1C */	stw r0, 0x1c(r30)
lbl_800DDBB4:
/* 800DDBB4 000DAAF4  80 01 00 AC */	lwz r0, 0xac(r1)
/* 800DDBB8 000DAAF8  83 E1 00 A4 */	lwz r31, 0xa4(r1)
/* 800DDBBC 000DAAFC  83 C1 00 A0 */	lwz r30, 0xa0(r1)
/* 800DDBC0 000DAB00  7C 08 03 A6 */	mtlr r0
/* 800DDBC4 000DAB04  83 A1 00 9C */	lwz r29, 0x9c(r1)
/* 800DDBC8 000DAB08  38 21 00 A8 */	addi r1, r1, 0xa8
/* 800DDBCC 000DAB0C  4E 80 00 20 */	blr 

.global evWarpMario__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evWarpMario__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800DDBD0 000DAB10  7C 08 02 A6 */	mflr r0
/* 800DDBD4 000DAB14  90 01 00 04 */	stw r0, 4(r1)
/* 800DDBD8 000DAB18  94 21 FF 20 */	stwu r1, -0xe0(r1)
/* 800DDBDC 000DAB1C  BF 61 00 CC */	stmw r27, 0xcc(r1)
/* 800DDBE0 000DAB20  7C 7B 1B 78 */	mr r27, r3
/* 800DDBE4 000DAB24  38 A1 00 0C */	addi r5, r1, 0xc
/* 800DDBE8 000DAB28  90 81 00 0C */	stw r4, 0xc(r1)
/* 800DDBEC 000DAB2C  3C 80 80 37 */	lis r4, $$21490@ha
/* 800DDBF0 000DAB30  3B E4 29 90 */	addi r31, r4, $$21490@l
/* 800DDBF4 000DAB34  38 80 00 04 */	li r4, 4
/* 800DDBF8 000DAB38  48 03 98 39 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800DDBFC 000DAB3C  80 7B 00 1C */	lwz r3, 0x1c(r27)
/* 800DDC00 000DAB40  2C 03 00 00 */	cmpwi r3, 0
/* 800DDC04 000DAB44  41 81 00 34 */	bgt lbl_800DDC38
/* 800DDC08 000DAB48  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800DDC0C 000DAB4C  4C C6 31 82 */	crclr 6
/* 800DDC10 000DAB50  4B FF F4 95 */	bl SpcTrace__FPCce
/* 800DDC14 000DAB54  80 1B 00 1C */	lwz r0, 0x1c(r27)
/* 800DDC18 000DAB58  80 7B 00 20 */	lwz r3, 0x20(r27)
/* 800DDC1C 000DAB5C  54 00 18 38 */	slwi r0, r0, 3
/* 800DDC20 000DAB60  7C 63 02 14 */	add r3, r3, r0
/* 800DDC24 000DAB64  80 03 00 00 */	lwz r0, 0(r3)
/* 800DDC28 000DAB68  90 01 00 9C */	stw r0, 0x9c(r1)
/* 800DDC2C 000DAB6C  80 03 00 04 */	lwz r0, 4(r3)
/* 800DDC30 000DAB70  90 01 00 A0 */	stw r0, 0xa0(r1)
/* 800DDC34 000DAB74  48 00 00 2C */	b lbl_800DDC60
lbl_800DDC38:
/* 800DDC38 000DAB78  38 03 FF FF */	addi r0, r3, -1
/* 800DDC3C 000DAB7C  90 1B 00 1C */	stw r0, 0x1c(r27)
/* 800DDC40 000DAB80  80 1B 00 1C */	lwz r0, 0x1c(r27)
/* 800DDC44 000DAB84  80 7B 00 20 */	lwz r3, 0x20(r27)
/* 800DDC48 000DAB88  54 00 18 38 */	slwi r0, r0, 3
/* 800DDC4C 000DAB8C  7C 63 02 14 */	add r3, r3, r0
/* 800DDC50 000DAB90  80 03 00 00 */	lwz r0, 0(r3)
/* 800DDC54 000DAB94  90 01 00 9C */	stw r0, 0x9c(r1)
/* 800DDC58 000DAB98  80 03 00 04 */	lwz r0, 4(r3)
/* 800DDC5C 000DAB9C  90 01 00 A0 */	stw r0, 0xa0(r1)
lbl_800DDC60:
/* 800DDC60 000DABA0  80 01 00 9C */	lwz r0, 0x9c(r1)
/* 800DDC64 000DABA4  2C 00 00 01 */	cmpwi r0, 1
/* 800DDC68 000DABA8  41 82 00 1C */	beq lbl_800DDC84
/* 800DDC6C 000DABAC  40 80 00 2C */	bge lbl_800DDC98
/* 800DDC70 000DABB0  2C 00 00 00 */	cmpwi r0, 0
/* 800DDC74 000DABB4  40 80 00 08 */	bge lbl_800DDC7C
/* 800DDC78 000DABB8  48 00 00 20 */	b lbl_800DDC98
lbl_800DDC7C:
/* 800DDC7C 000DABBC  83 C1 00 A0 */	lwz r30, 0xa0(r1)
/* 800DDC80 000DABC0  48 00 00 1C */	b lbl_800DDC9C
lbl_800DDC84:
/* 800DDC84 000DABC4  C0 01 00 A0 */	lfs f0, 0xa0(r1)
/* 800DDC88 000DABC8  FC 00 00 1E */	fctiwz f0, f0
/* 800DDC8C 000DABCC  D8 01 00 C0 */	stfd f0, 0xc0(r1)
/* 800DDC90 000DABD0  83 C1 00 C4 */	lwz r30, 0xc4(r1)
/* 800DDC94 000DABD4  48 00 00 08 */	b lbl_800DDC9C
lbl_800DDC98:
/* 800DDC98 000DABD8  3B C0 00 00 */	li r30, 0
lbl_800DDC9C:
/* 800DDC9C 000DABDC  80 7B 00 1C */	lwz r3, 0x1c(r27)
/* 800DDCA0 000DABE0  2C 03 00 00 */	cmpwi r3, 0
/* 800DDCA4 000DABE4  41 81 00 34 */	bgt lbl_800DDCD8
/* 800DDCA8 000DABE8  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800DDCAC 000DABEC  4C C6 31 82 */	crclr 6
/* 800DDCB0 000DABF0  4B FF F3 F5 */	bl SpcTrace__FPCce
/* 800DDCB4 000DABF4  80 1B 00 1C */	lwz r0, 0x1c(r27)
/* 800DDCB8 000DABF8  80 7B 00 20 */	lwz r3, 0x20(r27)
/* 800DDCBC 000DABFC  54 00 18 38 */	slwi r0, r0, 3
/* 800DDCC0 000DAC00  7C 63 02 14 */	add r3, r3, r0
/* 800DDCC4 000DAC04  80 03 00 00 */	lwz r0, 0(r3)
/* 800DDCC8 000DAC08  90 01 00 94 */	stw r0, 0x94(r1)
/* 800DDCCC 000DAC0C  80 03 00 04 */	lwz r0, 4(r3)
/* 800DDCD0 000DAC10  90 01 00 98 */	stw r0, 0x98(r1)
/* 800DDCD4 000DAC14  48 00 00 2C */	b lbl_800DDD00
lbl_800DDCD8:
/* 800DDCD8 000DAC18  38 03 FF FF */	addi r0, r3, -1
/* 800DDCDC 000DAC1C  90 1B 00 1C */	stw r0, 0x1c(r27)
/* 800DDCE0 000DAC20  80 1B 00 1C */	lwz r0, 0x1c(r27)
/* 800DDCE4 000DAC24  80 7B 00 20 */	lwz r3, 0x20(r27)
/* 800DDCE8 000DAC28  54 00 18 38 */	slwi r0, r0, 3
/* 800DDCEC 000DAC2C  7C 63 02 14 */	add r3, r3, r0
/* 800DDCF0 000DAC30  80 03 00 00 */	lwz r0, 0(r3)
/* 800DDCF4 000DAC34  90 01 00 94 */	stw r0, 0x94(r1)
/* 800DDCF8 000DAC38  80 03 00 04 */	lwz r0, 4(r3)
/* 800DDCFC 000DAC3C  90 01 00 98 */	stw r0, 0x98(r1)
lbl_800DDD00:
/* 800DDD00 000DAC40  80 01 00 94 */	lwz r0, 0x94(r1)
/* 800DDD04 000DAC44  2C 00 00 01 */	cmpwi r0, 1
/* 800DDD08 000DAC48  41 82 00 1C */	beq lbl_800DDD24
/* 800DDD0C 000DAC4C  40 80 00 2C */	bge lbl_800DDD38
/* 800DDD10 000DAC50  2C 00 00 00 */	cmpwi r0, 0
/* 800DDD14 000DAC54  40 80 00 08 */	bge lbl_800DDD1C
/* 800DDD18 000DAC58  48 00 00 20 */	b lbl_800DDD38
lbl_800DDD1C:
/* 800DDD1C 000DAC5C  83 A1 00 98 */	lwz r29, 0x98(r1)
/* 800DDD20 000DAC60  48 00 00 1C */	b lbl_800DDD3C
lbl_800DDD24:
/* 800DDD24 000DAC64  C0 01 00 98 */	lfs f0, 0x98(r1)
/* 800DDD28 000DAC68  FC 00 00 1E */	fctiwz f0, f0
/* 800DDD2C 000DAC6C  D8 01 00 C0 */	stfd f0, 0xc0(r1)
/* 800DDD30 000DAC70  83 A1 00 C4 */	lwz r29, 0xc4(r1)
/* 800DDD34 000DAC74  48 00 00 08 */	b lbl_800DDD3C
lbl_800DDD38:
/* 800DDD38 000DAC78  3B A0 00 00 */	li r29, 0
lbl_800DDD3C:
/* 800DDD3C 000DAC7C  80 7B 00 1C */	lwz r3, 0x1c(r27)
/* 800DDD40 000DAC80  2C 03 00 00 */	cmpwi r3, 0
/* 800DDD44 000DAC84  41 81 00 34 */	bgt lbl_800DDD78
/* 800DDD48 000DAC88  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800DDD4C 000DAC8C  4C C6 31 82 */	crclr 6
/* 800DDD50 000DAC90  4B FF F3 55 */	bl SpcTrace__FPCce
/* 800DDD54 000DAC94  80 1B 00 1C */	lwz r0, 0x1c(r27)
/* 800DDD58 000DAC98  80 7B 00 20 */	lwz r3, 0x20(r27)
/* 800DDD5C 000DAC9C  54 00 18 38 */	slwi r0, r0, 3
/* 800DDD60 000DACA0  7C 63 02 14 */	add r3, r3, r0
/* 800DDD64 000DACA4  80 03 00 00 */	lwz r0, 0(r3)
/* 800DDD68 000DACA8  90 01 00 8C */	stw r0, 0x8c(r1)
/* 800DDD6C 000DACAC  80 03 00 04 */	lwz r0, 4(r3)
/* 800DDD70 000DACB0  90 01 00 90 */	stw r0, 0x90(r1)
/* 800DDD74 000DACB4  48 00 00 2C */	b lbl_800DDDA0
lbl_800DDD78:
/* 800DDD78 000DACB8  38 03 FF FF */	addi r0, r3, -1
/* 800DDD7C 000DACBC  90 1B 00 1C */	stw r0, 0x1c(r27)
/* 800DDD80 000DACC0  80 1B 00 1C */	lwz r0, 0x1c(r27)
/* 800DDD84 000DACC4  80 7B 00 20 */	lwz r3, 0x20(r27)
/* 800DDD88 000DACC8  54 00 18 38 */	slwi r0, r0, 3
/* 800DDD8C 000DACCC  7C 63 02 14 */	add r3, r3, r0
/* 800DDD90 000DACD0  80 03 00 00 */	lwz r0, 0(r3)
/* 800DDD94 000DACD4  90 01 00 8C */	stw r0, 0x8c(r1)
/* 800DDD98 000DACD8  80 03 00 04 */	lwz r0, 4(r3)
/* 800DDD9C 000DACDC  90 01 00 90 */	stw r0, 0x90(r1)
lbl_800DDDA0:
/* 800DDDA0 000DACE0  80 01 00 8C */	lwz r0, 0x8c(r1)
/* 800DDDA4 000DACE4  2C 00 00 01 */	cmpwi r0, 1
/* 800DDDA8 000DACE8  41 82 00 1C */	beq lbl_800DDDC4
/* 800DDDAC 000DACEC  40 80 00 2C */	bge lbl_800DDDD8
/* 800DDDB0 000DACF0  2C 00 00 00 */	cmpwi r0, 0
/* 800DDDB4 000DACF4  40 80 00 08 */	bge lbl_800DDDBC
/* 800DDDB8 000DACF8  48 00 00 20 */	b lbl_800DDDD8
lbl_800DDDBC:
/* 800DDDBC 000DACFC  83 81 00 90 */	lwz r28, 0x90(r1)
/* 800DDDC0 000DAD00  48 00 00 1C */	b lbl_800DDDDC
lbl_800DDDC4:
/* 800DDDC4 000DAD04  C0 01 00 90 */	lfs f0, 0x90(r1)
/* 800DDDC8 000DAD08  FC 00 00 1E */	fctiwz f0, f0
/* 800DDDCC 000DAD0C  D8 01 00 C0 */	stfd f0, 0xc0(r1)
/* 800DDDD0 000DAD10  83 81 00 C4 */	lwz r28, 0xc4(r1)
/* 800DDDD4 000DAD14  48 00 00 08 */	b lbl_800DDDDC
lbl_800DDDD8:
/* 800DDDD8 000DAD18  3B 80 00 00 */	li r28, 0
lbl_800DDDDC:
/* 800DDDDC 000DAD1C  80 7B 00 1C */	lwz r3, 0x1c(r27)
/* 800DDDE0 000DAD20  2C 03 00 00 */	cmpwi r3, 0
/* 800DDDE4 000DAD24  41 81 00 34 */	bgt lbl_800DDE18
/* 800DDDE8 000DAD28  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800DDDEC 000DAD2C  4C C6 31 82 */	crclr 6
/* 800DDDF0 000DAD30  4B FF F2 B5 */	bl SpcTrace__FPCce
/* 800DDDF4 000DAD34  80 1B 00 1C */	lwz r0, 0x1c(r27)
/* 800DDDF8 000DAD38  80 7B 00 20 */	lwz r3, 0x20(r27)
/* 800DDDFC 000DAD3C  54 00 18 38 */	slwi r0, r0, 3
/* 800DDE00 000DAD40  7C 63 02 14 */	add r3, r3, r0
/* 800DDE04 000DAD44  80 03 00 00 */	lwz r0, 0(r3)
/* 800DDE08 000DAD48  90 01 00 84 */	stw r0, 0x84(r1)
/* 800DDE0C 000DAD4C  80 03 00 04 */	lwz r0, 4(r3)
/* 800DDE10 000DAD50  90 01 00 88 */	stw r0, 0x88(r1)
/* 800DDE14 000DAD54  48 00 00 2C */	b lbl_800DDE40
lbl_800DDE18:
/* 800DDE18 000DAD58  38 03 FF FF */	addi r0, r3, -1
/* 800DDE1C 000DAD5C  90 1B 00 1C */	stw r0, 0x1c(r27)
/* 800DDE20 000DAD60  80 1B 00 1C */	lwz r0, 0x1c(r27)
/* 800DDE24 000DAD64  80 7B 00 20 */	lwz r3, 0x20(r27)
/* 800DDE28 000DAD68  54 00 18 38 */	slwi r0, r0, 3
/* 800DDE2C 000DAD6C  7C 63 02 14 */	add r3, r3, r0
/* 800DDE30 000DAD70  80 03 00 00 */	lwz r0, 0(r3)
/* 800DDE34 000DAD74  90 01 00 84 */	stw r0, 0x84(r1)
/* 800DDE38 000DAD78  80 03 00 04 */	lwz r0, 4(r3)
/* 800DDE3C 000DAD7C  90 01 00 88 */	stw r0, 0x88(r1)
lbl_800DDE40:
/* 800DDE40 000DAD80  80 01 00 84 */	lwz r0, 0x84(r1)
/* 800DDE44 000DAD84  2C 00 00 01 */	cmpwi r0, 1
/* 800DDE48 000DAD88  41 82 00 1C */	beq lbl_800DDE64
/* 800DDE4C 000DAD8C  40 80 00 2C */	bge lbl_800DDE78
/* 800DDE50 000DAD90  2C 00 00 00 */	cmpwi r0, 0
/* 800DDE54 000DAD94  40 80 00 08 */	bge lbl_800DDE5C
/* 800DDE58 000DAD98  48 00 00 20 */	b lbl_800DDE78
lbl_800DDE5C:
/* 800DDE5C 000DAD9C  80 01 00 88 */	lwz r0, 0x88(r1)
/* 800DDE60 000DADA0  48 00 00 1C */	b lbl_800DDE7C
lbl_800DDE64:
/* 800DDE64 000DADA4  C0 01 00 88 */	lfs f0, 0x88(r1)
/* 800DDE68 000DADA8  FC 00 00 1E */	fctiwz f0, f0
/* 800DDE6C 000DADAC  D8 01 00 C0 */	stfd f0, 0xc0(r1)
/* 800DDE70 000DADB0  80 01 00 C4 */	lwz r0, 0xc4(r1)
/* 800DDE74 000DADB4  48 00 00 08 */	b lbl_800DDE7C
lbl_800DDE78:
/* 800DDE78 000DADB8  38 00 00 00 */	li r0, 0
lbl_800DDE7C:
/* 800DDE7C 000DADBC  6C 00 80 00 */	xoris r0, r0, 0x8000
/* 800DDE80 000DADC0  C8 82 8F E0 */	lfd f4, $$23934@sda21(r2)
/* 800DDE84 000DADC4  90 01 00 C4 */	stw r0, 0xc4(r1)
/* 800DDE88 000DADC8  6F 83 80 00 */	xoris r3, r28, 0x8000
/* 800DDE8C 000DADCC  3C A0 43 30 */	lis r5, 0x4330
/* 800DDE90 000DADD0  90 61 00 BC */	stw r3, 0xbc(r1)
/* 800DDE94 000DADD4  6F A4 80 00 */	xoris r4, r29, 0x8000
/* 800DDE98 000DADD8  6F C0 80 00 */	xoris r0, r30, 0x8000
/* 800DDE9C 000DADDC  90 A1 00 C0 */	stw r5, 0xc0(r1)
/* 800DDEA0 000DADE0  38 61 00 78 */	addi r3, r1, 0x78
/* 800DDEA4 000DADE4  C8 01 00 C0 */	lfd f0, 0xc0(r1)
/* 800DDEA8 000DADE8  90 81 00 B4 */	stw r4, 0xb4(r1)
/* 800DDEAC 000DADEC  EC 60 20 28 */	fsubs f3, f0, f4
/* 800DDEB0 000DADF0  90 A1 00 B8 */	stw r5, 0xb8(r1)
/* 800DDEB4 000DADF4  C8 01 00 B8 */	lfd f0, 0xb8(r1)
/* 800DDEB8 000DADF8  90 01 00 AC */	stw r0, 0xac(r1)
/* 800DDEBC 000DADFC  EC 20 20 28 */	fsubs f1, f0, f4
/* 800DDEC0 000DAE00  90 A1 00 B0 */	stw r5, 0xb0(r1)
/* 800DDEC4 000DAE04  C8 01 00 B0 */	lfd f0, 0xb0(r1)
/* 800DDEC8 000DAE08  D0 61 00 78 */	stfs f3, 0x78(r1)
/* 800DDECC 000DAE0C  EC 40 20 28 */	fsubs f2, f0, f4
/* 800DDED0 000DAE10  90 A1 00 A8 */	stw r5, 0xa8(r1)
/* 800DDED4 000DAE14  D0 21 00 7C */	stfs f1, 0x7c(r1)
/* 800DDED8 000DAE18  C8 01 00 A8 */	lfd f0, 0xa8(r1)
/* 800DDEDC 000DAE1C  D0 41 00 80 */	stfs f2, 0x80(r1)
/* 800DDEE0 000DAE20  EC 20 20 28 */	fsubs f1, f0, f4
/* 800DDEE4 000DAE24  48 07 4B 31 */	bl "SMS_MarioWarpRequest__FRCQ29JGeometry8TVec3<f>f"
/* 800DDEE8 000DAE28  38 00 00 00 */	li r0, 0
/* 800DDEEC 000DAE2C  90 01 00 60 */	stw r0, 0x60(r1)
/* 800DDEF0 000DAE30  90 01 00 64 */	stw r0, 0x64(r1)
/* 800DDEF4 000DAE34  80 7B 00 1C */	lwz r3, 0x1c(r27)
/* 800DDEF8 000DAE38  80 1B 00 18 */	lwz r0, 0x18(r27)
/* 800DDEFC 000DAE3C  7C 03 00 00 */	cmpw r3, r0
/* 800DDF00 000DAE40  41 80 00 14 */	blt lbl_800DDF14
/* 800DDF04 000DAE44  38 7F 07 8C */	addi r3, r31, 0x78c
/* 800DDF08 000DAE48  4C C6 31 82 */	crclr 6
/* 800DDF0C 000DAE4C  4B FF F1 99 */	bl SpcTrace__FPCce
/* 800DDF10 000DAE50  48 00 00 2C */	b lbl_800DDF3C
lbl_800DDF14:
/* 800DDF14 000DAE54  80 BB 00 20 */	lwz r5, 0x20(r27)
/* 800DDF18 000DAE58  54 64 18 38 */	slwi r4, r3, 3
/* 800DDF1C 000DAE5C  80 61 00 60 */	lwz r3, 0x60(r1)
/* 800DDF20 000DAE60  80 01 00 64 */	lwz r0, 0x64(r1)
/* 800DDF24 000DAE64  7C 85 22 14 */	add r4, r5, r4
/* 800DDF28 000DAE68  90 64 00 00 */	stw r3, 0(r4)
/* 800DDF2C 000DAE6C  90 04 00 04 */	stw r0, 4(r4)
/* 800DDF30 000DAE70  80 7B 00 1C */	lwz r3, 0x1c(r27)
/* 800DDF34 000DAE74  38 03 00 01 */	addi r0, r3, 1
/* 800DDF38 000DAE78  90 1B 00 1C */	stw r0, 0x1c(r27)
lbl_800DDF3C:
/* 800DDF3C 000DAE7C  BB 61 00 CC */	lmw r27, 0xcc(r1)
/* 800DDF40 000DAE80  80 01 00 E4 */	lwz r0, 0xe4(r1)
/* 800DDF44 000DAE84  38 21 00 E0 */	addi r1, r1, 0xe0
/* 800DDF48 000DAE88  7C 08 03 A6 */	mtlr r0
/* 800DDF4C 000DAE8C  4E 80 00 20 */	blr 

.global evSetCollision__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evSetCollision__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800DDF50 000DAE90  7C 08 02 A6 */	mflr r0
/* 800DDF54 000DAE94  90 01 00 04 */	stw r0, 4(r1)
/* 800DDF58 000DAE98  94 21 FF 58 */	stwu r1, -0xa8(r1)
/* 800DDF5C 000DAE9C  BF 61 00 94 */	stmw r27, 0x94(r1)
/* 800DDF60 000DAEA0  7C 7D 1B 78 */	mr r29, r3
/* 800DDF64 000DAEA4  38 A1 00 0C */	addi r5, r1, 0xc
/* 800DDF68 000DAEA8  90 81 00 0C */	stw r4, 0xc(r1)
/* 800DDF6C 000DAEAC  3C 80 80 37 */	lis r4, $$21490@ha
/* 800DDF70 000DAEB0  3B E4 29 90 */	addi r31, r4, $$21490@l
/* 800DDF74 000DAEB4  38 80 00 02 */	li r4, 2
/* 800DDF78 000DAEB8  48 03 94 B9 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800DDF7C 000DAEBC  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800DDF80 000DAEC0  2C 03 00 00 */	cmpwi r3, 0
/* 800DDF84 000DAEC4  41 81 00 34 */	bgt lbl_800DDFB8
/* 800DDF88 000DAEC8  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800DDF8C 000DAECC  4C C6 31 82 */	crclr 6
/* 800DDF90 000DAED0  4B FF F1 15 */	bl SpcTrace__FPCce
/* 800DDF94 000DAED4  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800DDF98 000DAED8  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800DDF9C 000DAEDC  54 00 18 38 */	slwi r0, r0, 3
/* 800DDFA0 000DAEE0  7C 63 02 14 */	add r3, r3, r0
/* 800DDFA4 000DAEE4  80 03 00 00 */	lwz r0, 0(r3)
/* 800DDFA8 000DAEE8  90 01 00 7C */	stw r0, 0x7c(r1)
/* 800DDFAC 000DAEEC  80 03 00 04 */	lwz r0, 4(r3)
/* 800DDFB0 000DAEF0  90 01 00 80 */	stw r0, 0x80(r1)
/* 800DDFB4 000DAEF4  48 00 00 2C */	b lbl_800DDFE0
lbl_800DDFB8:
/* 800DDFB8 000DAEF8  38 03 FF FF */	addi r0, r3, -1
/* 800DDFBC 000DAEFC  90 1D 00 1C */	stw r0, 0x1c(r29)
/* 800DDFC0 000DAF00  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800DDFC4 000DAF04  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800DDFC8 000DAF08  54 00 18 38 */	slwi r0, r0, 3
/* 800DDFCC 000DAF0C  7C 63 02 14 */	add r3, r3, r0
/* 800DDFD0 000DAF10  80 03 00 00 */	lwz r0, 0(r3)
/* 800DDFD4 000DAF14  90 01 00 7C */	stw r0, 0x7c(r1)
/* 800DDFD8 000DAF18  80 03 00 04 */	lwz r0, 4(r3)
/* 800DDFDC 000DAF1C  90 01 00 80 */	stw r0, 0x80(r1)
lbl_800DDFE0:
/* 800DDFE0 000DAF20  80 01 00 7C */	lwz r0, 0x7c(r1)
/* 800DDFE4 000DAF24  2C 00 00 01 */	cmpwi r0, 1
/* 800DDFE8 000DAF28  41 82 00 1C */	beq lbl_800DE004
/* 800DDFEC 000DAF2C  40 80 00 2C */	bge lbl_800DE018
/* 800DDFF0 000DAF30  2C 00 00 00 */	cmpwi r0, 0
/* 800DDFF4 000DAF34  40 80 00 08 */	bge lbl_800DDFFC
/* 800DDFF8 000DAF38  48 00 00 20 */	b lbl_800DE018
lbl_800DDFFC:
/* 800DDFFC 000DAF3C  83 C1 00 80 */	lwz r30, 0x80(r1)
/* 800DE000 000DAF40  48 00 00 1C */	b lbl_800DE01C
lbl_800DE004:
/* 800DE004 000DAF44  C0 01 00 80 */	lfs f0, 0x80(r1)
/* 800DE008 000DAF48  FC 00 00 1E */	fctiwz f0, f0
/* 800DE00C 000DAF4C  D8 01 00 88 */	stfd f0, 0x88(r1)
/* 800DE010 000DAF50  83 C1 00 8C */	lwz r30, 0x8c(r1)
/* 800DE014 000DAF54  48 00 00 08 */	b lbl_800DE01C
lbl_800DE018:
/* 800DE018 000DAF58  3B C0 00 00 */	li r30, 0
lbl_800DE01C:
/* 800DE01C 000DAF5C  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800DE020 000DAF60  2C 03 00 00 */	cmpwi r3, 0
/* 800DE024 000DAF64  41 81 00 34 */	bgt lbl_800DE058
/* 800DE028 000DAF68  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800DE02C 000DAF6C  4C C6 31 82 */	crclr 6
/* 800DE030 000DAF70  4B FF F0 75 */	bl SpcTrace__FPCce
/* 800DE034 000DAF74  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800DE038 000DAF78  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800DE03C 000DAF7C  54 00 18 38 */	slwi r0, r0, 3
/* 800DE040 000DAF80  7C 63 02 14 */	add r3, r3, r0
/* 800DE044 000DAF84  80 03 00 00 */	lwz r0, 0(r3)
/* 800DE048 000DAF88  90 01 00 6C */	stw r0, 0x6c(r1)
/* 800DE04C 000DAF8C  80 03 00 04 */	lwz r0, 4(r3)
/* 800DE050 000DAF90  90 01 00 70 */	stw r0, 0x70(r1)
/* 800DE054 000DAF94  48 00 00 2C */	b lbl_800DE080
lbl_800DE058:
/* 800DE058 000DAF98  38 03 FF FF */	addi r0, r3, -1
/* 800DE05C 000DAF9C  90 1D 00 1C */	stw r0, 0x1c(r29)
/* 800DE060 000DAFA0  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800DE064 000DAFA4  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800DE068 000DAFA8  54 00 18 38 */	slwi r0, r0, 3
/* 800DE06C 000DAFAC  7C 63 02 14 */	add r3, r3, r0
/* 800DE070 000DAFB0  80 03 00 00 */	lwz r0, 0(r3)
/* 800DE074 000DAFB4  90 01 00 6C */	stw r0, 0x6c(r1)
/* 800DE078 000DAFB8  80 03 00 04 */	lwz r0, 4(r3)
/* 800DE07C 000DAFBC  90 01 00 70 */	stw r0, 0x70(r1)
lbl_800DE080:
/* 800DE080 000DAFC0  80 81 00 6C */	lwz r4, 0x6c(r1)
/* 800DE084 000DAFC4  38 60 00 00 */	li r3, 0
/* 800DE088 000DAFC8  80 01 00 70 */	lwz r0, 0x70(r1)
/* 800DE08C 000DAFCC  90 81 00 74 */	stw r4, 0x74(r1)
/* 800DE090 000DAFD0  90 01 00 78 */	stw r0, 0x78(r1)
/* 800DE094 000DAFD4  80 01 00 74 */	lwz r0, 0x74(r1)
/* 800DE098 000DAFD8  2C 00 00 01 */	cmpwi r0, 1
/* 800DE09C 000DAFDC  41 82 00 9C */	beq lbl_800DE138
/* 800DE0A0 000DAFE0  40 80 00 10 */	bge lbl_800DE0B0
/* 800DE0A4 000DAFE4  2C 00 00 00 */	cmpwi r0, 0
/* 800DE0A8 000DAFE8  40 80 00 58 */	bge lbl_800DE100
/* 800DE0AC 000DAFEC  48 00 00 8C */	b lbl_800DE138
lbl_800DE0B0:
/* 800DE0B0 000DAFF0  2C 00 00 03 */	cmpwi r0, 3
/* 800DE0B4 000DAFF4  40 80 00 84 */	bge lbl_800DE138
/* 800DE0B8 000DAFF8  2C 00 00 02 */	cmpwi r0, 2
/* 800DE0BC 000DAFFC  41 82 00 08 */	beq lbl_800DE0C4
/* 800DE0C0 000DB000  48 00 00 0C */	b lbl_800DE0CC
lbl_800DE0C4:
/* 800DE0C4 000DB004  83 81 00 78 */	lwz r28, 0x78(r1)
/* 800DE0C8 000DB008  48 00 00 08 */	b lbl_800DE0D0
lbl_800DE0CC:
/* 800DE0CC 000DB00C  3B 82 8F F0 */	addi r28, r2, $$24053@sda21
lbl_800DE0D0:
/* 800DE0D0 000DB010  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 800DE0D4 000DB014  7F 83 E3 78 */	mr r3, r28
/* 800DE0D8 000DB018  83 64 00 04 */	lwz r27, 4(r4)
/* 800DE0DC 000DB01C  4B F6 65 B1 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800DE0E0 000DB020  81 9B 00 00 */	lwz r12, 0(r27)
/* 800DE0E4 000DB024  38 83 00 00 */	addi r4, r3, 0
/* 800DE0E8 000DB028  38 7B 00 00 */	addi r3, r27, 0
/* 800DE0EC 000DB02C  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 800DE0F0 000DB030  7F 85 E3 78 */	mr r5, r28
/* 800DE0F4 000DB034  7D 88 03 A6 */	mtlr r12
/* 800DE0F8 000DB038  4E 80 00 21 */	blrl 
/* 800DE0FC 000DB03C  48 00 00 3C */	b lbl_800DE138
lbl_800DE100:
/* 800DE100 000DB040  2C 00 00 01 */	cmpwi r0, 1
/* 800DE104 000DB044  41 82 00 1C */	beq lbl_800DE120
/* 800DE108 000DB048  40 80 00 2C */	bge lbl_800DE134
/* 800DE10C 000DB04C  2C 00 00 00 */	cmpwi r0, 0
/* 800DE110 000DB050  40 80 00 08 */	bge lbl_800DE118
/* 800DE114 000DB054  48 00 00 20 */	b lbl_800DE134
lbl_800DE118:
/* 800DE118 000DB058  80 61 00 78 */	lwz r3, 0x78(r1)
/* 800DE11C 000DB05C  48 00 00 1C */	b lbl_800DE138
lbl_800DE120:
/* 800DE120 000DB060  C0 01 00 78 */	lfs f0, 0x78(r1)
/* 800DE124 000DB064  FC 00 00 1E */	fctiwz f0, f0
/* 800DE128 000DB068  D8 01 00 88 */	stfd f0, 0x88(r1)
/* 800DE12C 000DB06C  80 61 00 8C */	lwz r3, 0x8c(r1)
/* 800DE130 000DB070  48 00 00 08 */	b lbl_800DE138
lbl_800DE134:
/* 800DE134 000DB074  38 60 00 00 */	li r3, 0
lbl_800DE138:
/* 800DE138 000DB078  2C 1E 00 00 */	cmpwi r30, 0
/* 800DE13C 000DB07C  40 82 00 14 */	bne lbl_800DE150
/* 800DE140 000DB080  80 03 00 64 */	lwz r0, 0x64(r3)
/* 800DE144 000DB084  60 00 00 01 */	ori r0, r0, 1
/* 800DE148 000DB088  90 03 00 64 */	stw r0, 0x64(r3)
/* 800DE14C 000DB08C  48 00 00 10 */	b lbl_800DE15C
lbl_800DE150:
/* 800DE150 000DB090  80 03 00 64 */	lwz r0, 0x64(r3)
/* 800DE154 000DB094  54 00 00 3C */	rlwinm r0, r0, 0, 0, 0x1e
/* 800DE158 000DB098  90 03 00 64 */	stw r0, 0x64(r3)
lbl_800DE15C:
/* 800DE15C 000DB09C  38 00 00 00 */	li r0, 0
/* 800DE160 000DB0A0  90 01 00 58 */	stw r0, 0x58(r1)
/* 800DE164 000DB0A4  90 01 00 5C */	stw r0, 0x5c(r1)
/* 800DE168 000DB0A8  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800DE16C 000DB0AC  80 1D 00 18 */	lwz r0, 0x18(r29)
/* 800DE170 000DB0B0  7C 03 00 00 */	cmpw r3, r0
/* 800DE174 000DB0B4  41 80 00 14 */	blt lbl_800DE188
/* 800DE178 000DB0B8  38 7F 07 8C */	addi r3, r31, 0x78c
/* 800DE17C 000DB0BC  4C C6 31 82 */	crclr 6
/* 800DE180 000DB0C0  4B FF EF 25 */	bl SpcTrace__FPCce
/* 800DE184 000DB0C4  48 00 00 2C */	b lbl_800DE1B0
lbl_800DE188:
/* 800DE188 000DB0C8  80 BD 00 20 */	lwz r5, 0x20(r29)
/* 800DE18C 000DB0CC  54 64 18 38 */	slwi r4, r3, 3
/* 800DE190 000DB0D0  80 61 00 58 */	lwz r3, 0x58(r1)
/* 800DE194 000DB0D4  80 01 00 5C */	lwz r0, 0x5c(r1)
/* 800DE198 000DB0D8  7C 85 22 14 */	add r4, r5, r4
/* 800DE19C 000DB0DC  90 64 00 00 */	stw r3, 0(r4)
/* 800DE1A0 000DB0E0  90 04 00 04 */	stw r0, 4(r4)
/* 800DE1A4 000DB0E4  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800DE1A8 000DB0E8  38 03 00 01 */	addi r0, r3, 1
/* 800DE1AC 000DB0EC  90 1D 00 1C */	stw r0, 0x1c(r29)
lbl_800DE1B0:
/* 800DE1B0 000DB0F0  BB 61 00 94 */	lmw r27, 0x94(r1)
/* 800DE1B4 000DB0F4  80 01 00 AC */	lwz r0, 0xac(r1)
/* 800DE1B8 000DB0F8  38 21 00 A8 */	addi r1, r1, 0xa8
/* 800DE1BC 000DB0FC  7C 08 03 A6 */	mtlr r0
/* 800DE1C0 000DB100  4E 80 00 20 */	blr 

.global evChangeSunglass__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evChangeSunglass__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800DE1C4 000DB104  7C 08 02 A6 */	mflr r0
/* 800DE1C8 000DB108  90 01 00 04 */	stw r0, 4(r1)
/* 800DE1CC 000DB10C  94 21 FF 88 */	stwu r1, -0x78(r1)
/* 800DE1D0 000DB110  93 E1 00 74 */	stw r31, 0x74(r1)
/* 800DE1D4 000DB114  38 A1 00 0C */	addi r5, r1, 0xc
/* 800DE1D8 000DB118  93 C1 00 70 */	stw r30, 0x70(r1)
/* 800DE1DC 000DB11C  7C 7E 1B 78 */	mr r30, r3
/* 800DE1E0 000DB120  93 A1 00 6C */	stw r29, 0x6c(r1)
/* 800DE1E4 000DB124  93 81 00 68 */	stw r28, 0x68(r1)
/* 800DE1E8 000DB128  90 81 00 0C */	stw r4, 0xc(r1)
/* 800DE1EC 000DB12C  3C 80 80 37 */	lis r4, $$21490@ha
/* 800DE1F0 000DB130  3B E4 29 90 */	addi r31, r4, $$21490@l
/* 800DE1F4 000DB134  38 80 00 01 */	li r4, 1
/* 800DE1F8 000DB138  48 03 92 39 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800DE1FC 000DB13C  80 7E 00 1C */	lwz r3, 0x1c(r30)
/* 800DE200 000DB140  2C 03 00 00 */	cmpwi r3, 0
/* 800DE204 000DB144  41 81 00 34 */	bgt lbl_800DE238
/* 800DE208 000DB148  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800DE20C 000DB14C  4C C6 31 82 */	crclr 6
/* 800DE210 000DB150  4B FF EE 95 */	bl SpcTrace__FPCce
/* 800DE214 000DB154  80 1E 00 1C */	lwz r0, 0x1c(r30)
/* 800DE218 000DB158  80 7E 00 20 */	lwz r3, 0x20(r30)
/* 800DE21C 000DB15C  54 00 18 38 */	slwi r0, r0, 3
/* 800DE220 000DB160  7C 63 02 14 */	add r3, r3, r0
/* 800DE224 000DB164  80 03 00 00 */	lwz r0, 0(r3)
/* 800DE228 000DB168  90 01 00 50 */	stw r0, 0x50(r1)
/* 800DE22C 000DB16C  80 03 00 04 */	lwz r0, 4(r3)
/* 800DE230 000DB170  90 01 00 54 */	stw r0, 0x54(r1)
/* 800DE234 000DB174  48 00 00 2C */	b lbl_800DE260
lbl_800DE238:
/* 800DE238 000DB178  38 03 FF FF */	addi r0, r3, -1
/* 800DE23C 000DB17C  90 1E 00 1C */	stw r0, 0x1c(r30)
/* 800DE240 000DB180  80 1E 00 1C */	lwz r0, 0x1c(r30)
/* 800DE244 000DB184  80 7E 00 20 */	lwz r3, 0x20(r30)
/* 800DE248 000DB188  54 00 18 38 */	slwi r0, r0, 3
/* 800DE24C 000DB18C  7C 63 02 14 */	add r3, r3, r0
/* 800DE250 000DB190  80 03 00 00 */	lwz r0, 0(r3)
/* 800DE254 000DB194  90 01 00 50 */	stw r0, 0x50(r1)
/* 800DE258 000DB198  80 03 00 04 */	lwz r0, 4(r3)
/* 800DE25C 000DB19C  90 01 00 54 */	stw r0, 0x54(r1)
lbl_800DE260:
/* 800DE260 000DB1A0  80 01 00 50 */	lwz r0, 0x50(r1)
/* 800DE264 000DB1A4  2C 00 00 01 */	cmpwi r0, 1
/* 800DE268 000DB1A8  41 82 00 1C */	beq lbl_800DE284
/* 800DE26C 000DB1AC  40 80 00 2C */	bge lbl_800DE298
/* 800DE270 000DB1B0  2C 00 00 00 */	cmpwi r0, 0
/* 800DE274 000DB1B4  40 80 00 08 */	bge lbl_800DE27C
/* 800DE278 000DB1B8  48 00 00 20 */	b lbl_800DE298
lbl_800DE27C:
/* 800DE27C 000DB1BC  83 A1 00 54 */	lwz r29, 0x54(r1)
/* 800DE280 000DB1C0  48 00 00 1C */	b lbl_800DE29C
lbl_800DE284:
/* 800DE284 000DB1C4  C0 01 00 54 */	lfs f0, 0x54(r1)
/* 800DE288 000DB1C8  FC 00 00 1E */	fctiwz f0, f0
/* 800DE28C 000DB1CC  D8 01 00 60 */	stfd f0, 0x60(r1)
/* 800DE290 000DB1D0  83 A1 00 64 */	lwz r29, 0x64(r1)
/* 800DE294 000DB1D4  48 00 00 08 */	b lbl_800DE29C
lbl_800DE298:
/* 800DE298 000DB1D8  3B A0 00 00 */	li r29, 0
lbl_800DE29C:
/* 800DE29C 000DB1DC  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 800DE2A0 000DB1E0  38 7F 07 BC */	addi r3, r31, 0x7bc
/* 800DE2A4 000DB1E4  83 84 00 04 */	lwz r28, 4(r4)
/* 800DE2A8 000DB1E8  4B F6 63 E5 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800DE2AC 000DB1EC  81 9C 00 00 */	lwz r12, 0(r28)
/* 800DE2B0 000DB1F0  38 83 00 00 */	addi r4, r3, 0
/* 800DE2B4 000DB1F4  38 7C 00 00 */	addi r3, r28, 0
/* 800DE2B8 000DB1F8  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 800DE2BC 000DB1FC  38 BF 07 BC */	addi r5, r31, 0x7bc
/* 800DE2C0 000DB200  7D 88 03 A6 */	mtlr r12
/* 800DE2C4 000DB204  4E 80 00 21 */	blrl 
/* 800DE2C8 000DB208  2C 1D 00 00 */	cmpwi r29, 0
/* 800DE2CC 000DB20C  40 82 00 40 */	bne lbl_800DE30C
/* 800DE2D0 000DB210  38 80 00 02 */	li r4, 2
/* 800DE2D4 000DB214  38 A0 00 01 */	li r5, 1
/* 800DE2D8 000DB218  48 15 F7 1D */	bl startFade__9TSunGlassFib
/* 800DE2DC 000DB21C  80 6D 98 B8 */	lwz r3, gpMarioOriginal@sda21(r13)
/* 800DE2E0 000DB220  48 04 90 09 */	bl wearGlass__6TMarioFv
/* 800DE2E4 000DB224  80 6D 97 D0 */	lwz r3, smInstance__12TFlagManager@sda21(r13)
/* 800DE2E8 000DB228  38 80 00 77 */	li r4, 0x77
/* 800DE2EC 000DB22C  48 00 99 F9 */	bl getShineFlag__12TFlagManagerCFUc
/* 800DE2F0 000DB230  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 800DE2F4 000DB234  41 82 00 50 */	beq lbl_800DE344
/* 800DE2F8 000DB238  80 6D 98 B8 */	lwz r3, gpMarioOriginal@sda21(r13)
/* 800DE2FC 000DB23C  80 03 01 18 */	lwz r0, 0x118(r3)
/* 800DE300 000DB240  64 00 00 10 */	oris r0, r0, 0x10
/* 800DE304 000DB244  90 03 01 18 */	stw r0, 0x118(r3)
/* 800DE308 000DB248  48 00 00 3C */	b lbl_800DE344
lbl_800DE30C:
/* 800DE30C 000DB24C  38 80 00 03 */	li r4, 3
/* 800DE310 000DB250  38 A0 00 01 */	li r5, 1
/* 800DE314 000DB254  48 15 F6 E1 */	bl startFade__9TSunGlassFib
/* 800DE318 000DB258  80 6D 98 B8 */	lwz r3, gpMarioOriginal@sda21(r13)
/* 800DE31C 000DB25C  48 04 8F 81 */	bl takeOffGlass__6TMarioFv
/* 800DE320 000DB260  80 6D 97 D0 */	lwz r3, smInstance__12TFlagManager@sda21(r13)
/* 800DE324 000DB264  38 80 00 77 */	li r4, 0x77
/* 800DE328 000DB268  48 00 99 BD */	bl getShineFlag__12TFlagManagerCFUc
/* 800DE32C 000DB26C  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 800DE330 000DB270  41 82 00 14 */	beq lbl_800DE344
/* 800DE334 000DB274  80 6D 98 B8 */	lwz r3, gpMarioOriginal@sda21(r13)
/* 800DE338 000DB278  80 03 01 18 */	lwz r0, 0x118(r3)
/* 800DE33C 000DB27C  54 00 03 14 */	rlwinm r0, r0, 0, 0xc, 0xa
/* 800DE340 000DB280  90 03 01 18 */	stw r0, 0x118(r3)
lbl_800DE344:
/* 800DE344 000DB284  38 00 00 00 */	li r0, 0
/* 800DE348 000DB288  90 01 00 3C */	stw r0, 0x3c(r1)
/* 800DE34C 000DB28C  90 01 00 40 */	stw r0, 0x40(r1)
/* 800DE350 000DB290  80 7E 00 1C */	lwz r3, 0x1c(r30)
/* 800DE354 000DB294  80 1E 00 18 */	lwz r0, 0x18(r30)
/* 800DE358 000DB298  7C 03 00 00 */	cmpw r3, r0
/* 800DE35C 000DB29C  41 80 00 14 */	blt lbl_800DE370
/* 800DE360 000DB2A0  38 7F 07 8C */	addi r3, r31, 0x78c
/* 800DE364 000DB2A4  4C C6 31 82 */	crclr 6
/* 800DE368 000DB2A8  4B FF ED 3D */	bl SpcTrace__FPCce
/* 800DE36C 000DB2AC  48 00 00 2C */	b lbl_800DE398
lbl_800DE370:
/* 800DE370 000DB2B0  80 BE 00 20 */	lwz r5, 0x20(r30)
/* 800DE374 000DB2B4  54 64 18 38 */	slwi r4, r3, 3
/* 800DE378 000DB2B8  80 61 00 3C */	lwz r3, 0x3c(r1)
/* 800DE37C 000DB2BC  80 01 00 40 */	lwz r0, 0x40(r1)
/* 800DE380 000DB2C0  7C 85 22 14 */	add r4, r5, r4
/* 800DE384 000DB2C4  90 64 00 00 */	stw r3, 0(r4)
/* 800DE388 000DB2C8  90 04 00 04 */	stw r0, 4(r4)
/* 800DE38C 000DB2CC  80 7E 00 1C */	lwz r3, 0x1c(r30)
/* 800DE390 000DB2D0  38 03 00 01 */	addi r0, r3, 1
/* 800DE394 000DB2D4  90 1E 00 1C */	stw r0, 0x1c(r30)
lbl_800DE398:
/* 800DE398 000DB2D8  80 01 00 7C */	lwz r0, 0x7c(r1)
/* 800DE39C 000DB2DC  83 E1 00 74 */	lwz r31, 0x74(r1)
/* 800DE3A0 000DB2E0  83 C1 00 70 */	lwz r30, 0x70(r1)
/* 800DE3A4 000DB2E4  7C 08 03 A6 */	mtlr r0
/* 800DE3A8 000DB2E8  83 A1 00 6C */	lwz r29, 0x6c(r1)
/* 800DE3AC 000DB2EC  83 81 00 68 */	lwz r28, 0x68(r1)
/* 800DE3B0 000DB2F0  38 21 00 78 */	addi r1, r1, 0x78
/* 800DE3B4 000DB2F4  4E 80 00 20 */	blr 

.global evStartMiss__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evStartMiss__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800DE3B8 000DB2F8  7C 08 02 A6 */	mflr r0
/* 800DE3BC 000DB2FC  90 01 00 04 */	stw r0, 4(r1)
/* 800DE3C0 000DB300  94 21 FF E0 */	stwu r1, -0x20(r1)
/* 800DE3C4 000DB304  93 E1 00 1C */	stw r31, 0x1c(r1)
/* 800DE3C8 000DB308  7C 7F 1B 78 */	mr r31, r3
/* 800DE3CC 000DB30C  38 A1 00 0C */	addi r5, r1, 0xc
/* 800DE3D0 000DB310  90 81 00 0C */	stw r4, 0xc(r1)
/* 800DE3D4 000DB314  38 80 00 00 */	li r4, 0
/* 800DE3D8 000DB318  48 03 90 59 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800DE3DC 000DB31C  80 6D 98 B8 */	lwz r3, gpMarioOriginal@sda21(r13)
/* 800DE3E0 000DB320  48 04 3E AD */	bl loserExec__6TMarioFv
/* 800DE3E4 000DB324  38 00 00 00 */	li r0, 0
/* 800DE3E8 000DB328  90 01 00 10 */	stw r0, 0x10(r1)
/* 800DE3EC 000DB32C  90 01 00 14 */	stw r0, 0x14(r1)
/* 800DE3F0 000DB330  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800DE3F4 000DB334  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800DE3F8 000DB338  7C 03 00 00 */	cmpw r3, r0
/* 800DE3FC 000DB33C  41 80 00 18 */	blt lbl_800DE414
/* 800DE400 000DB340  3C 60 80 37 */	lis r3, $$23834@ha
/* 800DE404 000DB344  4C C6 31 82 */	crclr 6
/* 800DE408 000DB348  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800DE40C 000DB34C  4B FF EC 99 */	bl SpcTrace__FPCce
/* 800DE410 000DB350  48 00 00 2C */	b lbl_800DE43C
lbl_800DE414:
/* 800DE414 000DB354  80 BF 00 20 */	lwz r5, 0x20(r31)
/* 800DE418 000DB358  54 64 18 38 */	slwi r4, r3, 3
/* 800DE41C 000DB35C  80 61 00 10 */	lwz r3, 0x10(r1)
/* 800DE420 000DB360  80 01 00 14 */	lwz r0, 0x14(r1)
/* 800DE424 000DB364  7C 85 22 14 */	add r4, r5, r4
/* 800DE428 000DB368  90 64 00 00 */	stw r3, 0(r4)
/* 800DE42C 000DB36C  90 04 00 04 */	stw r0, 4(r4)
/* 800DE430 000DB370  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800DE434 000DB374  38 03 00 01 */	addi r0, r3, 1
/* 800DE438 000DB378  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800DE43C:
/* 800DE43C 000DB37C  80 01 00 24 */	lwz r0, 0x24(r1)
/* 800DE440 000DB380  83 E1 00 1C */	lwz r31, 0x1c(r1)
/* 800DE444 000DB384  38 21 00 20 */	addi r1, r1, 0x20
/* 800DE448 000DB388  7C 08 03 A6 */	mtlr r0
/* 800DE44C 000DB38C  4E 80 00 20 */	blr 

.global evStartEventSE__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evStartEventSE__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800DE450 000DB390  7C 08 02 A6 */	mflr r0
/* 800DE454 000DB394  90 01 00 04 */	stw r0, 4(r1)
/* 800DE458 000DB398  94 21 FF A8 */	stwu r1, -0x58(r1)
/* 800DE45C 000DB39C  93 E1 00 54 */	stw r31, 0x54(r1)
/* 800DE460 000DB3A0  38 A1 00 0C */	addi r5, r1, 0xc
/* 800DE464 000DB3A4  93 C1 00 50 */	stw r30, 0x50(r1)
/* 800DE468 000DB3A8  7C 7E 1B 78 */	mr r30, r3
/* 800DE46C 000DB3AC  90 81 00 0C */	stw r4, 0xc(r1)
/* 800DE470 000DB3B0  38 80 00 01 */	li r4, 1
/* 800DE474 000DB3B4  48 03 8F BD */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800DE478 000DB3B8  80 7E 00 1C */	lwz r3, 0x1c(r30)
/* 800DE47C 000DB3BC  2C 03 00 00 */	cmpwi r3, 0
/* 800DE480 000DB3C0  41 81 00 38 */	bgt lbl_800DE4B8
/* 800DE484 000DB3C4  3C 60 80 37 */	lis r3, $$23829@ha
/* 800DE488 000DB3C8  4C C6 31 82 */	crclr 6
/* 800DE48C 000DB3CC  38 63 30 FC */	addi r3, r3, $$23829@l
/* 800DE490 000DB3D0  4B FF EC 15 */	bl SpcTrace__FPCce
/* 800DE494 000DB3D4  80 1E 00 1C */	lwz r0, 0x1c(r30)
/* 800DE498 000DB3D8  80 7E 00 20 */	lwz r3, 0x20(r30)
/* 800DE49C 000DB3DC  54 00 18 38 */	slwi r0, r0, 3
/* 800DE4A0 000DB3E0  7C 63 02 14 */	add r3, r3, r0
/* 800DE4A4 000DB3E4  80 03 00 00 */	lwz r0, 0(r3)
/* 800DE4A8 000DB3E8  90 01 00 38 */	stw r0, 0x38(r1)
/* 800DE4AC 000DB3EC  80 03 00 04 */	lwz r0, 4(r3)
/* 800DE4B0 000DB3F0  90 01 00 3C */	stw r0, 0x3c(r1)
/* 800DE4B4 000DB3F4  48 00 00 2C */	b lbl_800DE4E0
lbl_800DE4B8:
/* 800DE4B8 000DB3F8  38 03 FF FF */	addi r0, r3, -1
/* 800DE4BC 000DB3FC  90 1E 00 1C */	stw r0, 0x1c(r30)
/* 800DE4C0 000DB400  80 1E 00 1C */	lwz r0, 0x1c(r30)
/* 800DE4C4 000DB404  80 7E 00 20 */	lwz r3, 0x20(r30)
/* 800DE4C8 000DB408  54 00 18 38 */	slwi r0, r0, 3
/* 800DE4CC 000DB40C  7C 63 02 14 */	add r3, r3, r0
/* 800DE4D0 000DB410  80 03 00 00 */	lwz r0, 0(r3)
/* 800DE4D4 000DB414  90 01 00 38 */	stw r0, 0x38(r1)
/* 800DE4D8 000DB418  80 03 00 04 */	lwz r0, 4(r3)
/* 800DE4DC 000DB41C  90 01 00 3C */	stw r0, 0x3c(r1)
lbl_800DE4E0:
/* 800DE4E0 000DB420  80 01 00 38 */	lwz r0, 0x38(r1)
/* 800DE4E4 000DB424  2C 00 00 01 */	cmpwi r0, 1
/* 800DE4E8 000DB428  41 82 00 1C */	beq lbl_800DE504
/* 800DE4EC 000DB42C  40 80 00 2C */	bge lbl_800DE518
/* 800DE4F0 000DB430  2C 00 00 00 */	cmpwi r0, 0
/* 800DE4F4 000DB434  40 80 00 08 */	bge lbl_800DE4FC
/* 800DE4F8 000DB438  48 00 00 20 */	b lbl_800DE518
lbl_800DE4FC:
/* 800DE4FC 000DB43C  80 01 00 3C */	lwz r0, 0x3c(r1)
/* 800DE500 000DB440  48 00 00 1C */	b lbl_800DE51C
lbl_800DE504:
/* 800DE504 000DB444  C0 01 00 3C */	lfs f0, 0x3c(r1)
/* 800DE508 000DB448  FC 00 00 1E */	fctiwz f0, f0
/* 800DE50C 000DB44C  D8 01 00 48 */	stfd f0, 0x48(r1)
/* 800DE510 000DB450  80 01 00 4C */	lwz r0, 0x4c(r1)
/* 800DE514 000DB454  48 00 00 08 */	b lbl_800DE51C
lbl_800DE518:
/* 800DE518 000DB458  38 00 00 00 */	li r0, 0
lbl_800DE51C:
/* 800DE51C 000DB45C  2C 00 00 01 */	cmpwi r0, 1
/* 800DE520 000DB460  41 82 00 1C */	beq lbl_800DE53C
/* 800DE524 000DB464  40 80 00 1C */	bge lbl_800DE540
/* 800DE528 000DB468  2C 00 00 00 */	cmpwi r0, 0
/* 800DE52C 000DB46C  40 80 00 08 */	bge lbl_800DE534
/* 800DE530 000DB470  48 00 00 10 */	b lbl_800DE540
lbl_800DE534:
/* 800DE534 000DB474  3B E0 48 42 */	li r31, 0x4842
/* 800DE538 000DB478  48 00 00 08 */	b lbl_800DE540
lbl_800DE53C:
/* 800DE53C 000DB47C  3B E0 48 4F */	li r31, 0x484f
lbl_800DE540:
/* 800DE540 000DB480  80 6D 97 EC */	lwz r3, gpMSound@sda21(r13)
/* 800DE544 000DB484  7F E4 FB 78 */	mr r4, r31
/* 800DE548 000DB488  48 09 E5 09 */	bl gateCheck__6MSoundFUl
/* 800DE54C 000DB48C  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 800DE550 000DB490  41 82 00 18 */	beq lbl_800DE568
/* 800DE554 000DB494  38 7F 00 00 */	addi r3, r31, 0
/* 800DE558 000DB498  38 80 00 00 */	li r4, 0
/* 800DE55C 000DB49C  38 A0 00 00 */	li r5, 0
/* 800DE560 000DB4A0  38 C0 00 00 */	li r6, 0
/* 800DE564 000DB4A4  48 0A 38 25 */	bl startSoundSystemSE__Q214MSoundSESystem8MSoundSEFUlUlPP8JAISoundUl
lbl_800DE568:
/* 800DE568 000DB4A8  38 00 00 00 */	li r0, 0
/* 800DE56C 000DB4AC  90 01 00 2C */	stw r0, 0x2c(r1)
/* 800DE570 000DB4B0  90 01 00 30 */	stw r0, 0x30(r1)
/* 800DE574 000DB4B4  80 7E 00 1C */	lwz r3, 0x1c(r30)
/* 800DE578 000DB4B8  80 1E 00 18 */	lwz r0, 0x18(r30)
/* 800DE57C 000DB4BC  7C 03 00 00 */	cmpw r3, r0
/* 800DE580 000DB4C0  41 80 00 18 */	blt lbl_800DE598
/* 800DE584 000DB4C4  3C 60 80 37 */	lis r3, $$23834@ha
/* 800DE588 000DB4C8  4C C6 31 82 */	crclr 6
/* 800DE58C 000DB4CC  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800DE590 000DB4D0  4B FF EB 15 */	bl SpcTrace__FPCce
/* 800DE594 000DB4D4  48 00 00 2C */	b lbl_800DE5C0
lbl_800DE598:
/* 800DE598 000DB4D8  80 BE 00 20 */	lwz r5, 0x20(r30)
/* 800DE59C 000DB4DC  54 64 18 38 */	slwi r4, r3, 3
/* 800DE5A0 000DB4E0  80 61 00 2C */	lwz r3, 0x2c(r1)
/* 800DE5A4 000DB4E4  80 01 00 30 */	lwz r0, 0x30(r1)
/* 800DE5A8 000DB4E8  7C 85 22 14 */	add r4, r5, r4
/* 800DE5AC 000DB4EC  90 64 00 00 */	stw r3, 0(r4)
/* 800DE5B0 000DB4F0  90 04 00 04 */	stw r0, 4(r4)
/* 800DE5B4 000DB4F4  80 7E 00 1C */	lwz r3, 0x1c(r30)
/* 800DE5B8 000DB4F8  38 03 00 01 */	addi r0, r3, 1
/* 800DE5BC 000DB4FC  90 1E 00 1C */	stw r0, 0x1c(r30)
lbl_800DE5C0:
/* 800DE5C0 000DB500  80 01 00 5C */	lwz r0, 0x5c(r1)
/* 800DE5C4 000DB504  83 E1 00 54 */	lwz r31, 0x54(r1)
/* 800DE5C8 000DB508  83 C1 00 50 */	lwz r30, 0x50(r1)
/* 800DE5CC 000DB50C  7C 08 03 A6 */	mtlr r0
/* 800DE5D0 000DB510  38 21 00 58 */	addi r1, r1, 0x58
/* 800DE5D4 000DB514  4E 80 00 20 */	blr 

.global evStartSE__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evStartSE__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800DE5D8 000DB518  7C 08 02 A6 */	mflr r0
/* 800DE5DC 000DB51C  90 01 00 04 */	stw r0, 4(r1)
/* 800DE5E0 000DB520  94 21 FF B0 */	stwu r1, -0x50(r1)
/* 800DE5E4 000DB524  93 E1 00 4C */	stw r31, 0x4c(r1)
/* 800DE5E8 000DB528  7C 7F 1B 78 */	mr r31, r3
/* 800DE5EC 000DB52C  38 A1 00 0C */	addi r5, r1, 0xc
/* 800DE5F0 000DB530  93 C1 00 48 */	stw r30, 0x48(r1)
/* 800DE5F4 000DB534  90 81 00 0C */	stw r4, 0xc(r1)
/* 800DE5F8 000DB538  38 80 00 01 */	li r4, 1
/* 800DE5FC 000DB53C  48 03 8E 35 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800DE600 000DB540  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800DE604 000DB544  2C 03 00 00 */	cmpwi r3, 0
/* 800DE608 000DB548  41 81 00 38 */	bgt lbl_800DE640
/* 800DE60C 000DB54C  3C 60 80 37 */	lis r3, $$23829@ha
/* 800DE610 000DB550  4C C6 31 82 */	crclr 6
/* 800DE614 000DB554  38 63 30 FC */	addi r3, r3, $$23829@l
/* 800DE618 000DB558  4B FF EA 8D */	bl SpcTrace__FPCce
/* 800DE61C 000DB55C  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800DE620 000DB560  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800DE624 000DB564  54 00 18 38 */	slwi r0, r0, 3
/* 800DE628 000DB568  7C 63 02 14 */	add r3, r3, r0
/* 800DE62C 000DB56C  80 03 00 00 */	lwz r0, 0(r3)
/* 800DE630 000DB570  90 01 00 38 */	stw r0, 0x38(r1)
/* 800DE634 000DB574  80 03 00 04 */	lwz r0, 4(r3)
/* 800DE638 000DB578  90 01 00 3C */	stw r0, 0x3c(r1)
/* 800DE63C 000DB57C  48 00 00 2C */	b lbl_800DE668
lbl_800DE640:
/* 800DE640 000DB580  38 03 FF FF */	addi r0, r3, -1
/* 800DE644 000DB584  90 1F 00 1C */	stw r0, 0x1c(r31)
/* 800DE648 000DB588  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800DE64C 000DB58C  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800DE650 000DB590  54 00 18 38 */	slwi r0, r0, 3
/* 800DE654 000DB594  7C 63 02 14 */	add r3, r3, r0
/* 800DE658 000DB598  80 03 00 00 */	lwz r0, 0(r3)
/* 800DE65C 000DB59C  90 01 00 38 */	stw r0, 0x38(r1)
/* 800DE660 000DB5A0  80 03 00 04 */	lwz r0, 4(r3)
/* 800DE664 000DB5A4  90 01 00 3C */	stw r0, 0x3c(r1)
lbl_800DE668:
/* 800DE668 000DB5A8  80 01 00 38 */	lwz r0, 0x38(r1)
/* 800DE66C 000DB5AC  2C 00 00 01 */	cmpwi r0, 1
/* 800DE670 000DB5B0  41 82 00 1C */	beq lbl_800DE68C
/* 800DE674 000DB5B4  40 80 00 2C */	bge lbl_800DE6A0
/* 800DE678 000DB5B8  2C 00 00 00 */	cmpwi r0, 0
/* 800DE67C 000DB5BC  40 80 00 08 */	bge lbl_800DE684
/* 800DE680 000DB5C0  48 00 00 20 */	b lbl_800DE6A0
lbl_800DE684:
/* 800DE684 000DB5C4  83 C1 00 3C */	lwz r30, 0x3c(r1)
/* 800DE688 000DB5C8  48 00 00 1C */	b lbl_800DE6A4
lbl_800DE68C:
/* 800DE68C 000DB5CC  C0 01 00 3C */	lfs f0, 0x3c(r1)
/* 800DE690 000DB5D0  FC 00 00 1E */	fctiwz f0, f0
/* 800DE694 000DB5D4  D8 01 00 40 */	stfd f0, 0x40(r1)
/* 800DE698 000DB5D8  83 C1 00 44 */	lwz r30, 0x44(r1)
/* 800DE69C 000DB5DC  48 00 00 08 */	b lbl_800DE6A4
lbl_800DE6A0:
/* 800DE6A0 000DB5E0  3B C0 00 00 */	li r30, 0
lbl_800DE6A4:
/* 800DE6A4 000DB5E4  80 6D 97 EC */	lwz r3, gpMSound@sda21(r13)
/* 800DE6A8 000DB5E8  7F C4 F3 78 */	mr r4, r30
/* 800DE6AC 000DB5EC  48 09 E3 A5 */	bl gateCheck__6MSoundFUl
/* 800DE6B0 000DB5F0  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 800DE6B4 000DB5F4  41 82 00 18 */	beq lbl_800DE6CC
/* 800DE6B8 000DB5F8  38 7E 00 00 */	addi r3, r30, 0
/* 800DE6BC 000DB5FC  38 80 00 00 */	li r4, 0
/* 800DE6C0 000DB600  38 A0 00 00 */	li r5, 0
/* 800DE6C4 000DB604  38 C0 00 00 */	li r6, 0
/* 800DE6C8 000DB608  48 0A 36 C1 */	bl startSoundSystemSE__Q214MSoundSESystem8MSoundSEFUlUlPP8JAISoundUl
lbl_800DE6CC:
/* 800DE6CC 000DB60C  38 00 00 00 */	li r0, 0
/* 800DE6D0 000DB610  90 01 00 30 */	stw r0, 0x30(r1)
/* 800DE6D4 000DB614  90 01 00 34 */	stw r0, 0x34(r1)
/* 800DE6D8 000DB618  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800DE6DC 000DB61C  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800DE6E0 000DB620  7C 03 00 00 */	cmpw r3, r0
/* 800DE6E4 000DB624  41 80 00 18 */	blt lbl_800DE6FC
/* 800DE6E8 000DB628  3C 60 80 37 */	lis r3, $$23834@ha
/* 800DE6EC 000DB62C  4C C6 31 82 */	crclr 6
/* 800DE6F0 000DB630  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800DE6F4 000DB634  4B FF E9 B1 */	bl SpcTrace__FPCce
/* 800DE6F8 000DB638  48 00 00 2C */	b lbl_800DE724
lbl_800DE6FC:
/* 800DE6FC 000DB63C  80 BF 00 20 */	lwz r5, 0x20(r31)
/* 800DE700 000DB640  54 64 18 38 */	slwi r4, r3, 3
/* 800DE704 000DB644  80 61 00 30 */	lwz r3, 0x30(r1)
/* 800DE708 000DB648  80 01 00 34 */	lwz r0, 0x34(r1)
/* 800DE70C 000DB64C  7C 85 22 14 */	add r4, r5, r4
/* 800DE710 000DB650  90 64 00 00 */	stw r3, 0(r4)
/* 800DE714 000DB654  90 04 00 04 */	stw r0, 4(r4)
/* 800DE718 000DB658  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800DE71C 000DB65C  38 03 00 01 */	addi r0, r3, 1
/* 800DE720 000DB660  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800DE724:
/* 800DE724 000DB664  80 01 00 54 */	lwz r0, 0x54(r1)
/* 800DE728 000DB668  83 E1 00 4C */	lwz r31, 0x4c(r1)
/* 800DE72C 000DB66C  83 C1 00 48 */	lwz r30, 0x48(r1)
/* 800DE730 000DB670  7C 08 03 A6 */	mtlr r0
/* 800DE734 000DB674  38 21 00 50 */	addi r1, r1, 0x50
/* 800DE738 000DB678  4E 80 00 20 */	blr 

.global evStopBGM__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evStopBGM__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800DE73C 000DB67C  7C 08 02 A6 */	mflr r0
/* 800DE740 000DB680  90 01 00 04 */	stw r0, 4(r1)
/* 800DE744 000DB684  94 21 FF B8 */	stwu r1, -0x48(r1)
/* 800DE748 000DB688  93 E1 00 44 */	stw r31, 0x44(r1)
/* 800DE74C 000DB68C  7C 7F 1B 78 */	mr r31, r3
/* 800DE750 000DB690  38 A1 00 0C */	addi r5, r1, 0xc
/* 800DE754 000DB694  90 81 00 0C */	stw r4, 0xc(r1)
/* 800DE758 000DB698  38 80 00 01 */	li r4, 1
/* 800DE75C 000DB69C  48 03 8C D5 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800DE760 000DB6A0  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800DE764 000DB6A4  2C 03 00 00 */	cmpwi r3, 0
/* 800DE768 000DB6A8  41 81 00 38 */	bgt lbl_800DE7A0
/* 800DE76C 000DB6AC  3C 60 80 37 */	lis r3, $$23829@ha
/* 800DE770 000DB6B0  4C C6 31 82 */	crclr 6
/* 800DE774 000DB6B4  38 63 30 FC */	addi r3, r3, $$23829@l
/* 800DE778 000DB6B8  4B FF E9 2D */	bl SpcTrace__FPCce
/* 800DE77C 000DB6BC  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800DE780 000DB6C0  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800DE784 000DB6C4  54 00 18 38 */	slwi r0, r0, 3
/* 800DE788 000DB6C8  7C 63 02 14 */	add r3, r3, r0
/* 800DE78C 000DB6CC  80 03 00 00 */	lwz r0, 0(r3)
/* 800DE790 000DB6D0  90 01 00 30 */	stw r0, 0x30(r1)
/* 800DE794 000DB6D4  80 03 00 04 */	lwz r0, 4(r3)
/* 800DE798 000DB6D8  90 01 00 34 */	stw r0, 0x34(r1)
/* 800DE79C 000DB6DC  48 00 00 2C */	b lbl_800DE7C8
lbl_800DE7A0:
/* 800DE7A0 000DB6E0  38 03 FF FF */	addi r0, r3, -1
/* 800DE7A4 000DB6E4  90 1F 00 1C */	stw r0, 0x1c(r31)
/* 800DE7A8 000DB6E8  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800DE7AC 000DB6EC  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800DE7B0 000DB6F0  54 00 18 38 */	slwi r0, r0, 3
/* 800DE7B4 000DB6F4  7C 63 02 14 */	add r3, r3, r0
/* 800DE7B8 000DB6F8  80 03 00 00 */	lwz r0, 0(r3)
/* 800DE7BC 000DB6FC  90 01 00 30 */	stw r0, 0x30(r1)
/* 800DE7C0 000DB700  80 03 00 04 */	lwz r0, 4(r3)
/* 800DE7C4 000DB704  90 01 00 34 */	stw r0, 0x34(r1)
lbl_800DE7C8:
/* 800DE7C8 000DB708  80 01 00 30 */	lwz r0, 0x30(r1)
/* 800DE7CC 000DB70C  2C 00 00 01 */	cmpwi r0, 1
/* 800DE7D0 000DB710  41 82 00 1C */	beq lbl_800DE7EC
/* 800DE7D4 000DB714  40 80 00 2C */	bge lbl_800DE800
/* 800DE7D8 000DB718  2C 00 00 00 */	cmpwi r0, 0
/* 800DE7DC 000DB71C  40 80 00 08 */	bge lbl_800DE7E4
/* 800DE7E0 000DB720  48 00 00 20 */	b lbl_800DE800
lbl_800DE7E4:
/* 800DE7E4 000DB724  80 61 00 34 */	lwz r3, 0x34(r1)
/* 800DE7E8 000DB728  48 00 00 1C */	b lbl_800DE804
lbl_800DE7EC:
/* 800DE7EC 000DB72C  C0 01 00 34 */	lfs f0, 0x34(r1)
/* 800DE7F0 000DB730  FC 00 00 1E */	fctiwz f0, f0
/* 800DE7F4 000DB734  D8 01 00 38 */	stfd f0, 0x38(r1)
/* 800DE7F8 000DB738  80 61 00 3C */	lwz r3, 0x3c(r1)
/* 800DE7FC 000DB73C  48 00 00 08 */	b lbl_800DE804
lbl_800DE800:
/* 800DE800 000DB740  38 60 00 00 */	li r3, 0
lbl_800DE804:
/* 800DE804 000DB744  38 80 00 0A */	li r4, 0xa
/* 800DE808 000DB748  48 0A 15 89 */	bl stopBGM__5MSBgmFUlUl
/* 800DE80C 000DB74C  38 00 00 00 */	li r0, 0
/* 800DE810 000DB750  90 01 00 28 */	stw r0, 0x28(r1)
/* 800DE814 000DB754  90 01 00 2C */	stw r0, 0x2c(r1)
/* 800DE818 000DB758  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800DE81C 000DB75C  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800DE820 000DB760  7C 03 00 00 */	cmpw r3, r0
/* 800DE824 000DB764  41 80 00 18 */	blt lbl_800DE83C
/* 800DE828 000DB768  3C 60 80 37 */	lis r3, $$23834@ha
/* 800DE82C 000DB76C  4C C6 31 82 */	crclr 6
/* 800DE830 000DB770  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800DE834 000DB774  4B FF E8 71 */	bl SpcTrace__FPCce
/* 800DE838 000DB778  48 00 00 2C */	b lbl_800DE864
lbl_800DE83C:
/* 800DE83C 000DB77C  80 BF 00 20 */	lwz r5, 0x20(r31)
/* 800DE840 000DB780  54 64 18 38 */	slwi r4, r3, 3
/* 800DE844 000DB784  80 61 00 28 */	lwz r3, 0x28(r1)
/* 800DE848 000DB788  80 01 00 2C */	lwz r0, 0x2c(r1)
/* 800DE84C 000DB78C  7C 85 22 14 */	add r4, r5, r4
/* 800DE850 000DB790  90 64 00 00 */	stw r3, 0(r4)
/* 800DE854 000DB794  90 04 00 04 */	stw r0, 4(r4)
/* 800DE858 000DB798  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800DE85C 000DB79C  38 03 00 01 */	addi r0, r3, 1
/* 800DE860 000DB7A0  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800DE864:
/* 800DE864 000DB7A4  80 01 00 4C */	lwz r0, 0x4c(r1)
/* 800DE868 000DB7A8  83 E1 00 44 */	lwz r31, 0x44(r1)
/* 800DE86C 000DB7AC  38 21 00 48 */	addi r1, r1, 0x48
/* 800DE870 000DB7B0  7C 08 03 A6 */	mtlr r0
/* 800DE874 000DB7B4  4E 80 00 20 */	blr 

.global evPutNozzle__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evPutNozzle__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800DE878 000DB7B8  7C 08 02 A6 */	mflr r0
/* 800DE87C 000DB7BC  90 01 00 04 */	stw r0, 4(r1)
/* 800DE880 000DB7C0  94 21 FF 80 */	stwu r1, -0x80(r1)
/* 800DE884 000DB7C4  93 E1 00 7C */	stw r31, 0x7c(r1)
/* 800DE888 000DB7C8  7C 7F 1B 78 */	mr r31, r3
/* 800DE88C 000DB7CC  38 A1 00 0C */	addi r5, r1, 0xc
/* 800DE890 000DB7D0  93 C1 00 78 */	stw r30, 0x78(r1)
/* 800DE894 000DB7D4  93 A1 00 74 */	stw r29, 0x74(r1)
/* 800DE898 000DB7D8  90 81 00 0C */	stw r4, 0xc(r1)
/* 800DE89C 000DB7DC  38 80 00 01 */	li r4, 1
/* 800DE8A0 000DB7E0  48 03 8B 91 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800DE8A4 000DB7E4  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800DE8A8 000DB7E8  2C 03 00 00 */	cmpwi r3, 0
/* 800DE8AC 000DB7EC  41 81 00 38 */	bgt lbl_800DE8E4
/* 800DE8B0 000DB7F0  3C 60 80 37 */	lis r3, $$23829@ha
/* 800DE8B4 000DB7F4  4C C6 31 82 */	crclr 6
/* 800DE8B8 000DB7F8  38 63 30 FC */	addi r3, r3, $$23829@l
/* 800DE8BC 000DB7FC  4B FF E7 E9 */	bl SpcTrace__FPCce
/* 800DE8C0 000DB800  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800DE8C4 000DB804  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800DE8C8 000DB808  54 00 18 38 */	slwi r0, r0, 3
/* 800DE8CC 000DB80C  7C 63 02 14 */	add r3, r3, r0
/* 800DE8D0 000DB810  80 03 00 00 */	lwz r0, 0(r3)
/* 800DE8D4 000DB814  90 01 00 54 */	stw r0, 0x54(r1)
/* 800DE8D8 000DB818  80 03 00 04 */	lwz r0, 4(r3)
/* 800DE8DC 000DB81C  90 01 00 58 */	stw r0, 0x58(r1)
/* 800DE8E0 000DB820  48 00 00 2C */	b lbl_800DE90C
lbl_800DE8E4:
/* 800DE8E4 000DB824  38 03 FF FF */	addi r0, r3, -1
/* 800DE8E8 000DB828  90 1F 00 1C */	stw r0, 0x1c(r31)
/* 800DE8EC 000DB82C  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800DE8F0 000DB830  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800DE8F4 000DB834  54 00 18 38 */	slwi r0, r0, 3
/* 800DE8F8 000DB838  7C 63 02 14 */	add r3, r3, r0
/* 800DE8FC 000DB83C  80 03 00 00 */	lwz r0, 0(r3)
/* 800DE900 000DB840  90 01 00 54 */	stw r0, 0x54(r1)
/* 800DE904 000DB844  80 03 00 04 */	lwz r0, 4(r3)
/* 800DE908 000DB848  90 01 00 58 */	stw r0, 0x58(r1)
lbl_800DE90C:
/* 800DE90C 000DB84C  80 81 00 54 */	lwz r4, 0x54(r1)
/* 800DE910 000DB850  38 60 00 00 */	li r3, 0
/* 800DE914 000DB854  80 01 00 58 */	lwz r0, 0x58(r1)
/* 800DE918 000DB858  90 81 00 5C */	stw r4, 0x5c(r1)
/* 800DE91C 000DB85C  90 01 00 60 */	stw r0, 0x60(r1)
/* 800DE920 000DB860  80 01 00 5C */	lwz r0, 0x5c(r1)
/* 800DE924 000DB864  2C 00 00 01 */	cmpwi r0, 1
/* 800DE928 000DB868  41 82 00 9C */	beq lbl_800DE9C4
/* 800DE92C 000DB86C  40 80 00 10 */	bge lbl_800DE93C
/* 800DE930 000DB870  2C 00 00 00 */	cmpwi r0, 0
/* 800DE934 000DB874  40 80 00 58 */	bge lbl_800DE98C
/* 800DE938 000DB878  48 00 00 8C */	b lbl_800DE9C4
lbl_800DE93C:
/* 800DE93C 000DB87C  2C 00 00 03 */	cmpwi r0, 3
/* 800DE940 000DB880  40 80 00 84 */	bge lbl_800DE9C4
/* 800DE944 000DB884  2C 00 00 02 */	cmpwi r0, 2
/* 800DE948 000DB888  41 82 00 08 */	beq lbl_800DE950
/* 800DE94C 000DB88C  48 00 00 0C */	b lbl_800DE958
lbl_800DE950:
/* 800DE950 000DB890  83 C1 00 60 */	lwz r30, 0x60(r1)
/* 800DE954 000DB894  48 00 00 08 */	b lbl_800DE95C
lbl_800DE958:
/* 800DE958 000DB898  3B C2 8F F0 */	addi r30, r2, $$24053@sda21
lbl_800DE95C:
/* 800DE95C 000DB89C  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 800DE960 000DB8A0  7F C3 F3 78 */	mr r3, r30
/* 800DE964 000DB8A4  83 A4 00 04 */	lwz r29, 4(r4)
/* 800DE968 000DB8A8  4B F6 5D 25 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800DE96C 000DB8AC  81 9D 00 00 */	lwz r12, 0(r29)
/* 800DE970 000DB8B0  38 83 00 00 */	addi r4, r3, 0
/* 800DE974 000DB8B4  38 7D 00 00 */	addi r3, r29, 0
/* 800DE978 000DB8B8  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 800DE97C 000DB8BC  7F C5 F3 78 */	mr r5, r30
/* 800DE980 000DB8C0  7D 88 03 A6 */	mtlr r12
/* 800DE984 000DB8C4  4E 80 00 21 */	blrl 
/* 800DE988 000DB8C8  48 00 00 3C */	b lbl_800DE9C4
lbl_800DE98C:
/* 800DE98C 000DB8CC  2C 00 00 01 */	cmpwi r0, 1
/* 800DE990 000DB8D0  41 82 00 1C */	beq lbl_800DE9AC
/* 800DE994 000DB8D4  40 80 00 2C */	bge lbl_800DE9C0
/* 800DE998 000DB8D8  2C 00 00 00 */	cmpwi r0, 0
/* 800DE99C 000DB8DC  40 80 00 08 */	bge lbl_800DE9A4
/* 800DE9A0 000DB8E0  48 00 00 20 */	b lbl_800DE9C0
lbl_800DE9A4:
/* 800DE9A4 000DB8E4  80 61 00 60 */	lwz r3, 0x60(r1)
/* 800DE9A8 000DB8E8  48 00 00 1C */	b lbl_800DE9C4
lbl_800DE9AC:
/* 800DE9AC 000DB8EC  C0 01 00 60 */	lfs f0, 0x60(r1)
/* 800DE9B0 000DB8F0  FC 00 00 1E */	fctiwz f0, f0
/* 800DE9B4 000DB8F4  D8 01 00 68 */	stfd f0, 0x68(r1)
/* 800DE9B8 000DB8F8  80 61 00 6C */	lwz r3, 0x6c(r1)
/* 800DE9BC 000DB8FC  48 00 00 08 */	b lbl_800DE9C4
lbl_800DE9C0:
/* 800DE9C0 000DB900  38 60 00 00 */	li r3, 0
lbl_800DE9C4:
/* 800DE9C4 000DB904  81 83 00 00 */	lwz r12, 0(r3)
/* 800DE9C8 000DB908  81 8C 01 A0 */	lwz r12, 0x1a0(r12)
/* 800DE9CC 000DB90C  7D 88 03 A6 */	mtlr r12
/* 800DE9D0 000DB910  4E 80 00 21 */	blrl 
/* 800DE9D4 000DB914  38 00 00 00 */	li r0, 0
/* 800DE9D8 000DB918  90 01 00 44 */	stw r0, 0x44(r1)
/* 800DE9DC 000DB91C  90 01 00 48 */	stw r0, 0x48(r1)
/* 800DE9E0 000DB920  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800DE9E4 000DB924  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800DE9E8 000DB928  7C 03 00 00 */	cmpw r3, r0
/* 800DE9EC 000DB92C  41 80 00 18 */	blt lbl_800DEA04
/* 800DE9F0 000DB930  3C 60 80 37 */	lis r3, $$23834@ha
/* 800DE9F4 000DB934  4C C6 31 82 */	crclr 6
/* 800DE9F8 000DB938  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800DE9FC 000DB93C  4B FF E6 A9 */	bl SpcTrace__FPCce
/* 800DEA00 000DB940  48 00 00 2C */	b lbl_800DEA2C
lbl_800DEA04:
/* 800DEA04 000DB944  80 BF 00 20 */	lwz r5, 0x20(r31)
/* 800DEA08 000DB948  54 64 18 38 */	slwi r4, r3, 3
/* 800DEA0C 000DB94C  80 61 00 44 */	lwz r3, 0x44(r1)
/* 800DEA10 000DB950  80 01 00 48 */	lwz r0, 0x48(r1)
/* 800DEA14 000DB954  7C 85 22 14 */	add r4, r5, r4
/* 800DEA18 000DB958  90 64 00 00 */	stw r3, 0(r4)
/* 800DEA1C 000DB95C  90 04 00 04 */	stw r0, 4(r4)
/* 800DEA20 000DB960  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800DEA24 000DB964  38 03 00 01 */	addi r0, r3, 1
/* 800DEA28 000DB968  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800DEA2C:
/* 800DEA2C 000DB96C  80 01 00 84 */	lwz r0, 0x84(r1)
/* 800DEA30 000DB970  83 E1 00 7C */	lwz r31, 0x7c(r1)
/* 800DEA34 000DB974  83 C1 00 78 */	lwz r30, 0x78(r1)
/* 800DEA38 000DB978  7C 08 03 A6 */	mtlr r0
/* 800DEA3C 000DB97C  83 A1 00 74 */	lwz r29, 0x74(r1)
/* 800DEA40 000DB980  38 21 00 80 */	addi r1, r1, 0x80
/* 800DEA44 000DB984  4E 80 00 20 */	blr 

.global evEggYoshiStartFruit__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evEggYoshiStartFruit__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800DEA48 000DB988  7C 08 02 A6 */	mflr r0
/* 800DEA4C 000DB98C  90 01 00 04 */	stw r0, 4(r1)
/* 800DEA50 000DB990  94 21 FF 80 */	stwu r1, -0x80(r1)
/* 800DEA54 000DB994  93 E1 00 7C */	stw r31, 0x7c(r1)
/* 800DEA58 000DB998  7C 7F 1B 78 */	mr r31, r3
/* 800DEA5C 000DB99C  38 A1 00 0C */	addi r5, r1, 0xc
/* 800DEA60 000DB9A0  93 C1 00 78 */	stw r30, 0x78(r1)
/* 800DEA64 000DB9A4  93 A1 00 74 */	stw r29, 0x74(r1)
/* 800DEA68 000DB9A8  90 81 00 0C */	stw r4, 0xc(r1)
/* 800DEA6C 000DB9AC  38 80 00 01 */	li r4, 1
/* 800DEA70 000DB9B0  48 03 89 C1 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800DEA74 000DB9B4  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800DEA78 000DB9B8  2C 03 00 00 */	cmpwi r3, 0
/* 800DEA7C 000DB9BC  41 81 00 38 */	bgt lbl_800DEAB4
/* 800DEA80 000DB9C0  3C 60 80 37 */	lis r3, $$23829@ha
/* 800DEA84 000DB9C4  4C C6 31 82 */	crclr 6
/* 800DEA88 000DB9C8  38 63 30 FC */	addi r3, r3, $$23829@l
/* 800DEA8C 000DB9CC  4B FF E6 19 */	bl SpcTrace__FPCce
/* 800DEA90 000DB9D0  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800DEA94 000DB9D4  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800DEA98 000DB9D8  54 00 18 38 */	slwi r0, r0, 3
/* 800DEA9C 000DB9DC  7C 63 02 14 */	add r3, r3, r0
/* 800DEAA0 000DB9E0  80 03 00 00 */	lwz r0, 0(r3)
/* 800DEAA4 000DB9E4  90 01 00 58 */	stw r0, 0x58(r1)
/* 800DEAA8 000DB9E8  80 03 00 04 */	lwz r0, 4(r3)
/* 800DEAAC 000DB9EC  90 01 00 5C */	stw r0, 0x5c(r1)
/* 800DEAB0 000DB9F0  48 00 00 2C */	b lbl_800DEADC
lbl_800DEAB4:
/* 800DEAB4 000DB9F4  38 03 FF FF */	addi r0, r3, -1
/* 800DEAB8 000DB9F8  90 1F 00 1C */	stw r0, 0x1c(r31)
/* 800DEABC 000DB9FC  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800DEAC0 000DBA00  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800DEAC4 000DBA04  54 00 18 38 */	slwi r0, r0, 3
/* 800DEAC8 000DBA08  7C 63 02 14 */	add r3, r3, r0
/* 800DEACC 000DBA0C  80 03 00 00 */	lwz r0, 0(r3)
/* 800DEAD0 000DBA10  90 01 00 58 */	stw r0, 0x58(r1)
/* 800DEAD4 000DBA14  80 03 00 04 */	lwz r0, 4(r3)
/* 800DEAD8 000DBA18  90 01 00 5C */	stw r0, 0x5c(r1)
lbl_800DEADC:
/* 800DEADC 000DBA1C  80 81 00 58 */	lwz r4, 0x58(r1)
/* 800DEAE0 000DBA20  38 60 00 00 */	li r3, 0
/* 800DEAE4 000DBA24  80 01 00 5C */	lwz r0, 0x5c(r1)
/* 800DEAE8 000DBA28  90 81 00 60 */	stw r4, 0x60(r1)
/* 800DEAEC 000DBA2C  90 01 00 64 */	stw r0, 0x64(r1)
/* 800DEAF0 000DBA30  80 01 00 60 */	lwz r0, 0x60(r1)
/* 800DEAF4 000DBA34  2C 00 00 01 */	cmpwi r0, 1
/* 800DEAF8 000DBA38  41 82 00 9C */	beq lbl_800DEB94
/* 800DEAFC 000DBA3C  40 80 00 10 */	bge lbl_800DEB0C
/* 800DEB00 000DBA40  2C 00 00 00 */	cmpwi r0, 0
/* 800DEB04 000DBA44  40 80 00 58 */	bge lbl_800DEB5C
/* 800DEB08 000DBA48  48 00 00 8C */	b lbl_800DEB94
lbl_800DEB0C:
/* 800DEB0C 000DBA4C  2C 00 00 03 */	cmpwi r0, 3
/* 800DEB10 000DBA50  40 80 00 84 */	bge lbl_800DEB94
/* 800DEB14 000DBA54  2C 00 00 02 */	cmpwi r0, 2
/* 800DEB18 000DBA58  41 82 00 08 */	beq lbl_800DEB20
/* 800DEB1C 000DBA5C  48 00 00 0C */	b lbl_800DEB28
lbl_800DEB20:
/* 800DEB20 000DBA60  83 C1 00 64 */	lwz r30, 0x64(r1)
/* 800DEB24 000DBA64  48 00 00 08 */	b lbl_800DEB2C
lbl_800DEB28:
/* 800DEB28 000DBA68  3B C2 8F F0 */	addi r30, r2, $$24053@sda21
lbl_800DEB2C:
/* 800DEB2C 000DBA6C  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 800DEB30 000DBA70  7F C3 F3 78 */	mr r3, r30
/* 800DEB34 000DBA74  83 A4 00 04 */	lwz r29, 4(r4)
/* 800DEB38 000DBA78  4B F6 5B 55 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800DEB3C 000DBA7C  81 9D 00 00 */	lwz r12, 0(r29)
/* 800DEB40 000DBA80  38 83 00 00 */	addi r4, r3, 0
/* 800DEB44 000DBA84  38 7D 00 00 */	addi r3, r29, 0
/* 800DEB48 000DBA88  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 800DEB4C 000DBA8C  7F C5 F3 78 */	mr r5, r30
/* 800DEB50 000DBA90  7D 88 03 A6 */	mtlr r12
/* 800DEB54 000DBA94  4E 80 00 21 */	blrl 
/* 800DEB58 000DBA98  48 00 00 3C */	b lbl_800DEB94
lbl_800DEB5C:
/* 800DEB5C 000DBA9C  2C 00 00 01 */	cmpwi r0, 1
/* 800DEB60 000DBAA0  41 82 00 1C */	beq lbl_800DEB7C
/* 800DEB64 000DBAA4  40 80 00 2C */	bge lbl_800DEB90
/* 800DEB68 000DBAA8  2C 00 00 00 */	cmpwi r0, 0
/* 800DEB6C 000DBAAC  40 80 00 08 */	bge lbl_800DEB74
/* 800DEB70 000DBAB0  48 00 00 20 */	b lbl_800DEB90
lbl_800DEB74:
/* 800DEB74 000DBAB4  80 61 00 64 */	lwz r3, 0x64(r1)
/* 800DEB78 000DBAB8  48 00 00 1C */	b lbl_800DEB94
lbl_800DEB7C:
/* 800DEB7C 000DBABC  C0 01 00 64 */	lfs f0, 0x64(r1)
/* 800DEB80 000DBAC0  FC 00 00 1E */	fctiwz f0, f0
/* 800DEB84 000DBAC4  D8 01 00 68 */	stfd f0, 0x68(r1)
/* 800DEB88 000DBAC8  80 61 00 6C */	lwz r3, 0x6c(r1)
/* 800DEB8C 000DBACC  48 00 00 08 */	b lbl_800DEB94
lbl_800DEB90:
/* 800DEB90 000DBAD0  38 60 00 00 */	li r3, 0
lbl_800DEB94:
/* 800DEB94 000DBAD4  80 03 00 F0 */	lwz r0, 0xf0(r3)
/* 800DEB98 000DBAD8  54 00 07 FF */	clrlwi. r0, r0, 0x1f
/* 800DEB9C 000DBADC  40 82 00 08 */	bne lbl_800DEBA4
/* 800DEBA0 000DBAE0  48 0B 57 E1 */	bl startFruit__9TEggYoshiFv
lbl_800DEBA4:
/* 800DEBA4 000DBAE4  38 00 00 00 */	li r0, 0
/* 800DEBA8 000DBAE8  90 01 00 48 */	stw r0, 0x48(r1)
/* 800DEBAC 000DBAEC  90 01 00 4C */	stw r0, 0x4c(r1)
/* 800DEBB0 000DBAF0  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800DEBB4 000DBAF4  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800DEBB8 000DBAF8  7C 03 00 00 */	cmpw r3, r0
/* 800DEBBC 000DBAFC  41 80 00 18 */	blt lbl_800DEBD4
/* 800DEBC0 000DBB00  3C 60 80 37 */	lis r3, $$23834@ha
/* 800DEBC4 000DBB04  4C C6 31 82 */	crclr 6
/* 800DEBC8 000DBB08  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800DEBCC 000DBB0C  4B FF E4 D9 */	bl SpcTrace__FPCce
/* 800DEBD0 000DBB10  48 00 00 2C */	b lbl_800DEBFC
lbl_800DEBD4:
/* 800DEBD4 000DBB14  80 BF 00 20 */	lwz r5, 0x20(r31)
/* 800DEBD8 000DBB18  54 64 18 38 */	slwi r4, r3, 3
/* 800DEBDC 000DBB1C  80 61 00 48 */	lwz r3, 0x48(r1)
/* 800DEBE0 000DBB20  80 01 00 4C */	lwz r0, 0x4c(r1)
/* 800DEBE4 000DBB24  7C 85 22 14 */	add r4, r5, r4
/* 800DEBE8 000DBB28  90 64 00 00 */	stw r3, 0(r4)
/* 800DEBEC 000DBB2C  90 04 00 04 */	stw r0, 4(r4)
/* 800DEBF0 000DBB30  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800DEBF4 000DBB34  38 03 00 01 */	addi r0, r3, 1
/* 800DEBF8 000DBB38  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800DEBFC:
/* 800DEBFC 000DBB3C  80 01 00 84 */	lwz r0, 0x84(r1)
/* 800DEC00 000DBB40  83 E1 00 7C */	lwz r31, 0x7c(r1)
/* 800DEC04 000DBB44  83 C1 00 78 */	lwz r30, 0x78(r1)
/* 800DEC08 000DBB48  7C 08 03 A6 */	mtlr r0
/* 800DEC0C 000DBB4C  83 A1 00 74 */	lwz r29, 0x74(r1)
/* 800DEC10 000DBB50  38 21 00 80 */	addi r1, r1, 0x80
/* 800DEC14 000DBB54  4E 80 00 20 */	blr 

.global evStartBGM__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evStartBGM__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800DEC18 000DBB58  7C 08 02 A6 */	mflr r0
/* 800DEC1C 000DBB5C  90 01 00 04 */	stw r0, 4(r1)
/* 800DEC20 000DBB60  94 21 FF B8 */	stwu r1, -0x48(r1)
/* 800DEC24 000DBB64  93 E1 00 44 */	stw r31, 0x44(r1)
/* 800DEC28 000DBB68  7C 7F 1B 78 */	mr r31, r3
/* 800DEC2C 000DBB6C  38 A1 00 0C */	addi r5, r1, 0xc
/* 800DEC30 000DBB70  90 81 00 0C */	stw r4, 0xc(r1)
/* 800DEC34 000DBB74  38 80 00 01 */	li r4, 1
/* 800DEC38 000DBB78  48 03 87 F9 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800DEC3C 000DBB7C  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800DEC40 000DBB80  2C 03 00 00 */	cmpwi r3, 0
/* 800DEC44 000DBB84  41 81 00 38 */	bgt lbl_800DEC7C
/* 800DEC48 000DBB88  3C 60 80 37 */	lis r3, $$23829@ha
/* 800DEC4C 000DBB8C  4C C6 31 82 */	crclr 6
/* 800DEC50 000DBB90  38 63 30 FC */	addi r3, r3, $$23829@l
/* 800DEC54 000DBB94  4B FF E4 51 */	bl SpcTrace__FPCce
/* 800DEC58 000DBB98  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800DEC5C 000DBB9C  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800DEC60 000DBBA0  54 00 18 38 */	slwi r0, r0, 3
/* 800DEC64 000DBBA4  7C 63 02 14 */	add r3, r3, r0
/* 800DEC68 000DBBA8  80 03 00 00 */	lwz r0, 0(r3)
/* 800DEC6C 000DBBAC  90 01 00 30 */	stw r0, 0x30(r1)
/* 800DEC70 000DBBB0  80 03 00 04 */	lwz r0, 4(r3)
/* 800DEC74 000DBBB4  90 01 00 34 */	stw r0, 0x34(r1)
/* 800DEC78 000DBBB8  48 00 00 2C */	b lbl_800DECA4
lbl_800DEC7C:
/* 800DEC7C 000DBBBC  38 03 FF FF */	addi r0, r3, -1
/* 800DEC80 000DBBC0  90 1F 00 1C */	stw r0, 0x1c(r31)
/* 800DEC84 000DBBC4  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800DEC88 000DBBC8  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800DEC8C 000DBBCC  54 00 18 38 */	slwi r0, r0, 3
/* 800DEC90 000DBBD0  7C 63 02 14 */	add r3, r3, r0
/* 800DEC94 000DBBD4  80 03 00 00 */	lwz r0, 0(r3)
/* 800DEC98 000DBBD8  90 01 00 30 */	stw r0, 0x30(r1)
/* 800DEC9C 000DBBDC  80 03 00 04 */	lwz r0, 4(r3)
/* 800DECA0 000DBBE0  90 01 00 34 */	stw r0, 0x34(r1)
lbl_800DECA4:
/* 800DECA4 000DBBE4  80 01 00 30 */	lwz r0, 0x30(r1)
/* 800DECA8 000DBBE8  2C 00 00 01 */	cmpwi r0, 1
/* 800DECAC 000DBBEC  41 82 00 1C */	beq lbl_800DECC8
/* 800DECB0 000DBBF0  40 80 00 2C */	bge lbl_800DECDC
/* 800DECB4 000DBBF4  2C 00 00 00 */	cmpwi r0, 0
/* 800DECB8 000DBBF8  40 80 00 08 */	bge lbl_800DECC0
/* 800DECBC 000DBBFC  48 00 00 20 */	b lbl_800DECDC
lbl_800DECC0:
/* 800DECC0 000DBC00  80 61 00 34 */	lwz r3, 0x34(r1)
/* 800DECC4 000DBC04  48 00 00 1C */	b lbl_800DECE0
lbl_800DECC8:
/* 800DECC8 000DBC08  C0 01 00 34 */	lfs f0, 0x34(r1)
/* 800DECCC 000DBC0C  FC 00 00 1E */	fctiwz f0, f0
/* 800DECD0 000DBC10  D8 01 00 38 */	stfd f0, 0x38(r1)
/* 800DECD4 000DBC14  80 61 00 3C */	lwz r3, 0x3c(r1)
/* 800DECD8 000DBC18  48 00 00 08 */	b lbl_800DECE0
lbl_800DECDC:
/* 800DECDC 000DBC1C  38 60 00 00 */	li r3, 0
lbl_800DECE0:
/* 800DECE0 000DBC20  48 0A 11 BD */	bl startBGM__5MSBgmFUl
/* 800DECE4 000DBC24  38 00 00 00 */	li r0, 0
/* 800DECE8 000DBC28  90 01 00 28 */	stw r0, 0x28(r1)
/* 800DECEC 000DBC2C  90 01 00 2C */	stw r0, 0x2c(r1)
/* 800DECF0 000DBC30  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800DECF4 000DBC34  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800DECF8 000DBC38  7C 03 00 00 */	cmpw r3, r0
/* 800DECFC 000DBC3C  41 80 00 18 */	blt lbl_800DED14
/* 800DED00 000DBC40  3C 60 80 37 */	lis r3, $$23834@ha
/* 800DED04 000DBC44  4C C6 31 82 */	crclr 6
/* 800DED08 000DBC48  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800DED0C 000DBC4C  4B FF E3 99 */	bl SpcTrace__FPCce
/* 800DED10 000DBC50  48 00 00 2C */	b lbl_800DED3C
lbl_800DED14:
/* 800DED14 000DBC54  80 BF 00 20 */	lwz r5, 0x20(r31)
/* 800DED18 000DBC58  54 64 18 38 */	slwi r4, r3, 3
/* 800DED1C 000DBC5C  80 61 00 28 */	lwz r3, 0x28(r1)
/* 800DED20 000DBC60  80 01 00 2C */	lwz r0, 0x2c(r1)
/* 800DED24 000DBC64  7C 85 22 14 */	add r4, r5, r4
/* 800DED28 000DBC68  90 64 00 00 */	stw r3, 0(r4)
/* 800DED2C 000DBC6C  90 04 00 04 */	stw r0, 4(r4)
/* 800DED30 000DBC70  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800DED34 000DBC74  38 03 00 01 */	addi r0, r3, 1
/* 800DED38 000DBC78  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800DED3C:
/* 800DED3C 000DBC7C  80 01 00 4C */	lwz r0, 0x4c(r1)
/* 800DED40 000DBC80  83 E1 00 44 */	lwz r31, 0x44(r1)
/* 800DED44 000DBC84  38 21 00 48 */	addi r1, r1, 0x48
/* 800DED48 000DBC88  7C 08 03 A6 */	mtlr r0
/* 800DED4C 000DBC8C  4E 80 00 20 */	blr 

.global evManiCoinDown__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evManiCoinDown__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800DED50 000DBC90  7C 08 02 A6 */	mflr r0
/* 800DED54 000DBC94  90 01 00 04 */	stw r0, 4(r1)
/* 800DED58 000DBC98  94 21 FF D0 */	stwu r1, -0x30(r1)
/* 800DED5C 000DBC9C  93 E1 00 2C */	stw r31, 0x2c(r1)
/* 800DED60 000DBCA0  7C 7F 1B 78 */	mr r31, r3
/* 800DED64 000DBCA4  38 A1 00 0C */	addi r5, r1, 0xc
/* 800DED68 000DBCA8  90 81 00 0C */	stw r4, 0xc(r1)
/* 800DED6C 000DBCAC  38 80 00 00 */	li r4, 0
/* 800DED70 000DBCB0  48 03 86 C1 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800DED74 000DBCB4  80 6D 97 E8 */	lwz r3, gpMarDirector@sda21(r13)
/* 800DED78 000DBCB8  80 63 00 74 */	lwz r3, 0x74(r3)
/* 800DED7C 000DBCBC  48 12 C1 DD */	bl startAppearStar__11TGCConsole2Fv
/* 800DED80 000DBCC0  38 00 00 00 */	li r0, 0
/* 800DED84 000DBCC4  90 01 00 1C */	stw r0, 0x1c(r1)
/* 800DED88 000DBCC8  90 01 00 20 */	stw r0, 0x20(r1)
/* 800DED8C 000DBCCC  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800DED90 000DBCD0  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800DED94 000DBCD4  7C 03 00 00 */	cmpw r3, r0
/* 800DED98 000DBCD8  41 80 00 18 */	blt lbl_800DEDB0
/* 800DED9C 000DBCDC  3C 60 80 37 */	lis r3, $$23834@ha
/* 800DEDA0 000DBCE0  4C C6 31 82 */	crclr 6
/* 800DEDA4 000DBCE4  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800DEDA8 000DBCE8  4B FF E2 FD */	bl SpcTrace__FPCce
/* 800DEDAC 000DBCEC  48 00 00 2C */	b lbl_800DEDD8
lbl_800DEDB0:
/* 800DEDB0 000DBCF0  80 BF 00 20 */	lwz r5, 0x20(r31)
/* 800DEDB4 000DBCF4  54 64 18 38 */	slwi r4, r3, 3
/* 800DEDB8 000DBCF8  80 61 00 1C */	lwz r3, 0x1c(r1)
/* 800DEDBC 000DBCFC  80 01 00 20 */	lwz r0, 0x20(r1)
/* 800DEDC0 000DBD00  7C 85 22 14 */	add r4, r5, r4
/* 800DEDC4 000DBD04  90 64 00 00 */	stw r3, 0(r4)
/* 800DEDC8 000DBD08  90 04 00 04 */	stw r0, 4(r4)
/* 800DEDCC 000DBD0C  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800DEDD0 000DBD10  38 03 00 01 */	addi r0, r3, 1
/* 800DEDD4 000DBD14  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800DEDD8:
/* 800DEDD8 000DBD18  80 01 00 34 */	lwz r0, 0x34(r1)
/* 800DEDDC 000DBD1C  83 E1 00 2C */	lwz r31, 0x2c(r1)
/* 800DEDE0 000DBD20  38 21 00 30 */	addi r1, r1, 0x30
/* 800DEDE4 000DBD24  7C 08 03 A6 */	mtlr r0
/* 800DEDE8 000DBD28  4E 80 00 20 */	blr 

.global evSetEventID__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evSetEventID__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800DEDEC 000DBD2C  7C 08 02 A6 */	mflr r0
/* 800DEDF0 000DBD30  90 01 00 04 */	stw r0, 4(r1)
/* 800DEDF4 000DBD34  94 21 FF 58 */	stwu r1, -0xa8(r1)
/* 800DEDF8 000DBD38  BF 61 00 94 */	stmw r27, 0x94(r1)
/* 800DEDFC 000DBD3C  7C 7D 1B 78 */	mr r29, r3
/* 800DEE00 000DBD40  38 A1 00 0C */	addi r5, r1, 0xc
/* 800DEE04 000DBD44  90 81 00 0C */	stw r4, 0xc(r1)
/* 800DEE08 000DBD48  3C 80 80 37 */	lis r4, $$21490@ha
/* 800DEE0C 000DBD4C  3B E4 29 90 */	addi r31, r4, $$21490@l
/* 800DEE10 000DBD50  38 80 00 02 */	li r4, 2
/* 800DEE14 000DBD54  48 03 86 1D */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800DEE18 000DBD58  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800DEE1C 000DBD5C  2C 03 00 00 */	cmpwi r3, 0
/* 800DEE20 000DBD60  41 81 00 34 */	bgt lbl_800DEE54
/* 800DEE24 000DBD64  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800DEE28 000DBD68  4C C6 31 82 */	crclr 6
/* 800DEE2C 000DBD6C  4B FF E2 79 */	bl SpcTrace__FPCce
/* 800DEE30 000DBD70  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800DEE34 000DBD74  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800DEE38 000DBD78  54 00 18 38 */	slwi r0, r0, 3
/* 800DEE3C 000DBD7C  7C 63 02 14 */	add r3, r3, r0
/* 800DEE40 000DBD80  80 03 00 00 */	lwz r0, 0(r3)
/* 800DEE44 000DBD84  90 01 00 7C */	stw r0, 0x7c(r1)
/* 800DEE48 000DBD88  80 03 00 04 */	lwz r0, 4(r3)
/* 800DEE4C 000DBD8C  90 01 00 80 */	stw r0, 0x80(r1)
/* 800DEE50 000DBD90  48 00 00 2C */	b lbl_800DEE7C
lbl_800DEE54:
/* 800DEE54 000DBD94  38 03 FF FF */	addi r0, r3, -1
/* 800DEE58 000DBD98  90 1D 00 1C */	stw r0, 0x1c(r29)
/* 800DEE5C 000DBD9C  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800DEE60 000DBDA0  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800DEE64 000DBDA4  54 00 18 38 */	slwi r0, r0, 3
/* 800DEE68 000DBDA8  7C 63 02 14 */	add r3, r3, r0
/* 800DEE6C 000DBDAC  80 03 00 00 */	lwz r0, 0(r3)
/* 800DEE70 000DBDB0  90 01 00 7C */	stw r0, 0x7c(r1)
/* 800DEE74 000DBDB4  80 03 00 04 */	lwz r0, 4(r3)
/* 800DEE78 000DBDB8  90 01 00 80 */	stw r0, 0x80(r1)
lbl_800DEE7C:
/* 800DEE7C 000DBDBC  80 01 00 7C */	lwz r0, 0x7c(r1)
/* 800DEE80 000DBDC0  2C 00 00 01 */	cmpwi r0, 1
/* 800DEE84 000DBDC4  41 82 00 1C */	beq lbl_800DEEA0
/* 800DEE88 000DBDC8  40 80 00 2C */	bge lbl_800DEEB4
/* 800DEE8C 000DBDCC  2C 00 00 00 */	cmpwi r0, 0
/* 800DEE90 000DBDD0  40 80 00 08 */	bge lbl_800DEE98
/* 800DEE94 000DBDD4  48 00 00 20 */	b lbl_800DEEB4
lbl_800DEE98:
/* 800DEE98 000DBDD8  80 01 00 80 */	lwz r0, 0x80(r1)
/* 800DEE9C 000DBDDC  48 00 00 1C */	b lbl_800DEEB8
lbl_800DEEA0:
/* 800DEEA0 000DBDE0  C0 01 00 80 */	lfs f0, 0x80(r1)
/* 800DEEA4 000DBDE4  FC 00 00 1E */	fctiwz f0, f0
/* 800DEEA8 000DBDE8  D8 01 00 88 */	stfd f0, 0x88(r1)
/* 800DEEAC 000DBDEC  80 01 00 8C */	lwz r0, 0x8c(r1)
/* 800DEEB0 000DBDF0  48 00 00 08 */	b lbl_800DEEB8
lbl_800DEEB4:
/* 800DEEB4 000DBDF4  38 00 00 00 */	li r0, 0
lbl_800DEEB8:
/* 800DEEB8 000DBDF8  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800DEEBC 000DBDFC  54 1E 04 3E */	clrlwi r30, r0, 0x10
/* 800DEEC0 000DBE00  2C 03 00 00 */	cmpwi r3, 0
/* 800DEEC4 000DBE04  41 81 00 34 */	bgt lbl_800DEEF8
/* 800DEEC8 000DBE08  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800DEECC 000DBE0C  4C C6 31 82 */	crclr 6
/* 800DEED0 000DBE10  4B FF E1 D5 */	bl SpcTrace__FPCce
/* 800DEED4 000DBE14  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800DEED8 000DBE18  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800DEEDC 000DBE1C  54 00 18 38 */	slwi r0, r0, 3
/* 800DEEE0 000DBE20  7C 63 02 14 */	add r3, r3, r0
/* 800DEEE4 000DBE24  80 03 00 00 */	lwz r0, 0(r3)
/* 800DEEE8 000DBE28  90 01 00 6C */	stw r0, 0x6c(r1)
/* 800DEEEC 000DBE2C  80 03 00 04 */	lwz r0, 4(r3)
/* 800DEEF0 000DBE30  90 01 00 70 */	stw r0, 0x70(r1)
/* 800DEEF4 000DBE34  48 00 00 2C */	b lbl_800DEF20
lbl_800DEEF8:
/* 800DEEF8 000DBE38  38 03 FF FF */	addi r0, r3, -1
/* 800DEEFC 000DBE3C  90 1D 00 1C */	stw r0, 0x1c(r29)
/* 800DEF00 000DBE40  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800DEF04 000DBE44  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800DEF08 000DBE48  54 00 18 38 */	slwi r0, r0, 3
/* 800DEF0C 000DBE4C  7C 63 02 14 */	add r3, r3, r0
/* 800DEF10 000DBE50  80 03 00 00 */	lwz r0, 0(r3)
/* 800DEF14 000DBE54  90 01 00 6C */	stw r0, 0x6c(r1)
/* 800DEF18 000DBE58  80 03 00 04 */	lwz r0, 4(r3)
/* 800DEF1C 000DBE5C  90 01 00 70 */	stw r0, 0x70(r1)
lbl_800DEF20:
/* 800DEF20 000DBE60  80 81 00 6C */	lwz r4, 0x6c(r1)
/* 800DEF24 000DBE64  38 60 00 00 */	li r3, 0
/* 800DEF28 000DBE68  80 01 00 70 */	lwz r0, 0x70(r1)
/* 800DEF2C 000DBE6C  90 81 00 74 */	stw r4, 0x74(r1)
/* 800DEF30 000DBE70  90 01 00 78 */	stw r0, 0x78(r1)
/* 800DEF34 000DBE74  80 01 00 74 */	lwz r0, 0x74(r1)
/* 800DEF38 000DBE78  2C 00 00 01 */	cmpwi r0, 1
/* 800DEF3C 000DBE7C  41 82 00 9C */	beq lbl_800DEFD8
/* 800DEF40 000DBE80  40 80 00 10 */	bge lbl_800DEF50
/* 800DEF44 000DBE84  2C 00 00 00 */	cmpwi r0, 0
/* 800DEF48 000DBE88  40 80 00 58 */	bge lbl_800DEFA0
/* 800DEF4C 000DBE8C  48 00 00 8C */	b lbl_800DEFD8
lbl_800DEF50:
/* 800DEF50 000DBE90  2C 00 00 03 */	cmpwi r0, 3
/* 800DEF54 000DBE94  40 80 00 84 */	bge lbl_800DEFD8
/* 800DEF58 000DBE98  2C 00 00 02 */	cmpwi r0, 2
/* 800DEF5C 000DBE9C  41 82 00 08 */	beq lbl_800DEF64
/* 800DEF60 000DBEA0  48 00 00 0C */	b lbl_800DEF6C
lbl_800DEF64:
/* 800DEF64 000DBEA4  83 81 00 78 */	lwz r28, 0x78(r1)
/* 800DEF68 000DBEA8  48 00 00 08 */	b lbl_800DEF70
lbl_800DEF6C:
/* 800DEF6C 000DBEAC  3B 82 8F F0 */	addi r28, r2, $$24053@sda21
lbl_800DEF70:
/* 800DEF70 000DBEB0  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 800DEF74 000DBEB4  7F 83 E3 78 */	mr r3, r28
/* 800DEF78 000DBEB8  83 64 00 04 */	lwz r27, 4(r4)
/* 800DEF7C 000DBEBC  4B F6 57 11 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800DEF80 000DBEC0  81 9B 00 00 */	lwz r12, 0(r27)
/* 800DEF84 000DBEC4  38 83 00 00 */	addi r4, r3, 0
/* 800DEF88 000DBEC8  38 7B 00 00 */	addi r3, r27, 0
/* 800DEF8C 000DBECC  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 800DEF90 000DBED0  7F 85 E3 78 */	mr r5, r28
/* 800DEF94 000DBED4  7D 88 03 A6 */	mtlr r12
/* 800DEF98 000DBED8  4E 80 00 21 */	blrl 
/* 800DEF9C 000DBEDC  48 00 00 3C */	b lbl_800DEFD8
lbl_800DEFA0:
/* 800DEFA0 000DBEE0  2C 00 00 01 */	cmpwi r0, 1
/* 800DEFA4 000DBEE4  41 82 00 1C */	beq lbl_800DEFC0
/* 800DEFA8 000DBEE8  40 80 00 2C */	bge lbl_800DEFD4
/* 800DEFAC 000DBEEC  2C 00 00 00 */	cmpwi r0, 0
/* 800DEFB0 000DBEF0  40 80 00 08 */	bge lbl_800DEFB8
/* 800DEFB4 000DBEF4  48 00 00 20 */	b lbl_800DEFD4
lbl_800DEFB8:
/* 800DEFB8 000DBEF8  80 61 00 78 */	lwz r3, 0x78(r1)
/* 800DEFBC 000DBEFC  48 00 00 1C */	b lbl_800DEFD8
lbl_800DEFC0:
/* 800DEFC0 000DBF00  C0 01 00 78 */	lfs f0, 0x78(r1)
/* 800DEFC4 000DBF04  FC 00 00 1E */	fctiwz f0, f0
/* 800DEFC8 000DBF08  D8 01 00 88 */	stfd f0, 0x88(r1)
/* 800DEFCC 000DBF0C  80 61 00 8C */	lwz r3, 0x8c(r1)
/* 800DEFD0 000DBF10  48 00 00 08 */	b lbl_800DEFD8
lbl_800DEFD4:
/* 800DEFD4 000DBF14  38 60 00 00 */	li r3, 0
lbl_800DEFD8:
/* 800DEFD8 000DBF18  93 C3 01 34 */	stw r30, 0x134(r3)
/* 800DEFDC 000DBF1C  38 00 00 00 */	li r0, 0
/* 800DEFE0 000DBF20  90 01 00 58 */	stw r0, 0x58(r1)
/* 800DEFE4 000DBF24  90 01 00 5C */	stw r0, 0x5c(r1)
/* 800DEFE8 000DBF28  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800DEFEC 000DBF2C  80 1D 00 18 */	lwz r0, 0x18(r29)
/* 800DEFF0 000DBF30  7C 03 00 00 */	cmpw r3, r0
/* 800DEFF4 000DBF34  41 80 00 14 */	blt lbl_800DF008
/* 800DEFF8 000DBF38  38 7F 07 8C */	addi r3, r31, 0x78c
/* 800DEFFC 000DBF3C  4C C6 31 82 */	crclr 6
/* 800DF000 000DBF40  4B FF E0 A5 */	bl SpcTrace__FPCce
/* 800DF004 000DBF44  48 00 00 2C */	b lbl_800DF030
lbl_800DF008:
/* 800DF008 000DBF48  80 BD 00 20 */	lwz r5, 0x20(r29)
/* 800DF00C 000DBF4C  54 64 18 38 */	slwi r4, r3, 3
/* 800DF010 000DBF50  80 61 00 58 */	lwz r3, 0x58(r1)
/* 800DF014 000DBF54  80 01 00 5C */	lwz r0, 0x5c(r1)
/* 800DF018 000DBF58  7C 85 22 14 */	add r4, r5, r4
/* 800DF01C 000DBF5C  90 64 00 00 */	stw r3, 0(r4)
/* 800DF020 000DBF60  90 04 00 04 */	stw r0, 4(r4)
/* 800DF024 000DBF64  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800DF028 000DBF68  38 03 00 01 */	addi r0, r3, 1
/* 800DF02C 000DBF6C  90 1D 00 1C */	stw r0, 0x1c(r29)
lbl_800DF030:
/* 800DF030 000DBF70  BB 61 00 94 */	lmw r27, 0x94(r1)
/* 800DF034 000DBF74  80 01 00 AC */	lwz r0, 0xac(r1)
/* 800DF038 000DBF78  38 21 00 A8 */	addi r1, r1, 0xa8
/* 800DF03C 000DBF7C  7C 08 03 A6 */	mtlr r0
/* 800DF040 000DBF80  4E 80 00 20 */	blr 

.global evSetTransScale__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evSetTransScale__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800DF044 000DBF84  7C 08 02 A6 */	mflr r0
/* 800DF048 000DBF88  90 01 00 04 */	stw r0, 4(r1)
/* 800DF04C 000DBF8C  94 21 FE 58 */	stwu r1, -0x1a8(r1)
/* 800DF050 000DBF90  DB E1 01 A0 */	stfd f31, 0x1a0(r1)
/* 800DF054 000DBF94  38 A1 00 0C */	addi r5, r1, 0xc
/* 800DF058 000DBF98  DB C1 01 98 */	stfd f30, 0x198(r1)
/* 800DF05C 000DBF9C  DB A1 01 90 */	stfd f29, 0x190(r1)
/* 800DF060 000DBFA0  DB 81 01 88 */	stfd f28, 0x188(r1)
/* 800DF064 000DBFA4  DB 61 01 80 */	stfd f27, 0x180(r1)
/* 800DF068 000DBFA8  DB 41 01 78 */	stfd f26, 0x178(r1)
/* 800DF06C 000DBFAC  93 E1 01 74 */	stw r31, 0x174(r1)
/* 800DF070 000DBFB0  93 C1 01 70 */	stw r30, 0x170(r1)
/* 800DF074 000DBFB4  7C 7E 1B 78 */	mr r30, r3
/* 800DF078 000DBFB8  93 A1 01 6C */	stw r29, 0x16c(r1)
/* 800DF07C 000DBFBC  93 81 01 68 */	stw r28, 0x168(r1)
/* 800DF080 000DBFC0  90 81 00 0C */	stw r4, 0xc(r1)
/* 800DF084 000DBFC4  3C 80 80 37 */	lis r4, $$21490@ha
/* 800DF088 000DBFC8  3B E4 29 90 */	addi r31, r4, $$21490@l
/* 800DF08C 000DBFCC  38 80 00 07 */	li r4, 7
/* 800DF090 000DBFD0  48 03 83 A1 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800DF094 000DBFD4  80 7E 00 1C */	lwz r3, 0x1c(r30)
/* 800DF098 000DBFD8  2C 03 00 00 */	cmpwi r3, 0
/* 800DF09C 000DBFDC  41 81 00 34 */	bgt lbl_800DF0D0
/* 800DF0A0 000DBFE0  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800DF0A4 000DBFE4  4C C6 31 82 */	crclr 6
/* 800DF0A8 000DBFE8  4B FF DF FD */	bl SpcTrace__FPCce
/* 800DF0AC 000DBFEC  80 1E 00 1C */	lwz r0, 0x1c(r30)
/* 800DF0B0 000DBFF0  80 7E 00 20 */	lwz r3, 0x20(r30)
/* 800DF0B4 000DBFF4  54 00 18 38 */	slwi r0, r0, 3
/* 800DF0B8 000DBFF8  7C 63 02 14 */	add r3, r3, r0
/* 800DF0BC 000DBFFC  80 03 00 00 */	lwz r0, 0(r3)
/* 800DF0C0 000DC000  90 01 01 1C */	stw r0, 0x11c(r1)
/* 800DF0C4 000DC004  80 03 00 04 */	lwz r0, 4(r3)
/* 800DF0C8 000DC008  90 01 01 20 */	stw r0, 0x120(r1)
/* 800DF0CC 000DC00C  48 00 00 2C */	b lbl_800DF0F8
lbl_800DF0D0:
/* 800DF0D0 000DC010  38 03 FF FF */	addi r0, r3, -1
/* 800DF0D4 000DC014  90 1E 00 1C */	stw r0, 0x1c(r30)
/* 800DF0D8 000DC018  80 1E 00 1C */	lwz r0, 0x1c(r30)
/* 800DF0DC 000DC01C  80 7E 00 20 */	lwz r3, 0x20(r30)
/* 800DF0E0 000DC020  54 00 18 38 */	slwi r0, r0, 3
/* 800DF0E4 000DC024  7C 63 02 14 */	add r3, r3, r0
/* 800DF0E8 000DC028  80 03 00 00 */	lwz r0, 0(r3)
/* 800DF0EC 000DC02C  90 01 01 1C */	stw r0, 0x11c(r1)
/* 800DF0F0 000DC030  80 03 00 04 */	lwz r0, 4(r3)
/* 800DF0F4 000DC034  90 01 01 20 */	stw r0, 0x120(r1)
lbl_800DF0F8:
/* 800DF0F8 000DC038  80 01 01 1C */	lwz r0, 0x11c(r1)
/* 800DF0FC 000DC03C  2C 00 00 01 */	cmpwi r0, 1
/* 800DF100 000DC040  41 82 00 38 */	beq lbl_800DF138
/* 800DF104 000DC044  40 80 00 3C */	bge lbl_800DF140
/* 800DF108 000DC048  2C 00 00 00 */	cmpwi r0, 0
/* 800DF10C 000DC04C  40 80 00 08 */	bge lbl_800DF114
/* 800DF110 000DC050  48 00 00 30 */	b lbl_800DF140
lbl_800DF114:
/* 800DF114 000DC054  80 61 01 20 */	lwz r3, 0x120(r1)
/* 800DF118 000DC058  3C 00 43 30 */	lis r0, 0x4330
/* 800DF11C 000DC05C  C8 22 8F E0 */	lfd f1, $$23934@sda21(r2)
/* 800DF120 000DC060  6C 63 80 00 */	xoris r3, r3, 0x8000
/* 800DF124 000DC064  90 61 01 64 */	stw r3, 0x164(r1)
/* 800DF128 000DC068  90 01 01 60 */	stw r0, 0x160(r1)
/* 800DF12C 000DC06C  C8 01 01 60 */	lfd f0, 0x160(r1)
/* 800DF130 000DC070  EF E0 08 28 */	fsubs f31, f0, f1
/* 800DF134 000DC074  48 00 00 10 */	b lbl_800DF144
lbl_800DF138:
/* 800DF138 000DC078  C3 E1 01 20 */	lfs f31, 0x120(r1)
/* 800DF13C 000DC07C  48 00 00 08 */	b lbl_800DF144
lbl_800DF140:
/* 800DF140 000DC080  C3 E2 8F D0 */	lfs f31, $$23832@sda21(r2)
lbl_800DF144:
/* 800DF144 000DC084  80 7E 00 1C */	lwz r3, 0x1c(r30)
/* 800DF148 000DC088  2C 03 00 00 */	cmpwi r3, 0
/* 800DF14C 000DC08C  41 81 00 34 */	bgt lbl_800DF180
/* 800DF150 000DC090  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800DF154 000DC094  4C C6 31 82 */	crclr 6
/* 800DF158 000DC098  4B FF DF 4D */	bl SpcTrace__FPCce
/* 800DF15C 000DC09C  80 1E 00 1C */	lwz r0, 0x1c(r30)
/* 800DF160 000DC0A0  80 7E 00 20 */	lwz r3, 0x20(r30)
/* 800DF164 000DC0A4  54 00 18 38 */	slwi r0, r0, 3
/* 800DF168 000DC0A8  7C 63 02 14 */	add r3, r3, r0
/* 800DF16C 000DC0AC  80 03 00 00 */	lwz r0, 0(r3)
/* 800DF170 000DC0B0  90 01 01 14 */	stw r0, 0x114(r1)
/* 800DF174 000DC0B4  80 03 00 04 */	lwz r0, 4(r3)
/* 800DF178 000DC0B8  90 01 01 18 */	stw r0, 0x118(r1)
/* 800DF17C 000DC0BC  48 00 00 2C */	b lbl_800DF1A8
lbl_800DF180:
/* 800DF180 000DC0C0  38 03 FF FF */	addi r0, r3, -1
/* 800DF184 000DC0C4  90 1E 00 1C */	stw r0, 0x1c(r30)
/* 800DF188 000DC0C8  80 1E 00 1C */	lwz r0, 0x1c(r30)
/* 800DF18C 000DC0CC  80 7E 00 20 */	lwz r3, 0x20(r30)
/* 800DF190 000DC0D0  54 00 18 38 */	slwi r0, r0, 3
/* 800DF194 000DC0D4  7C 63 02 14 */	add r3, r3, r0
/* 800DF198 000DC0D8  80 03 00 00 */	lwz r0, 0(r3)
/* 800DF19C 000DC0DC  90 01 01 14 */	stw r0, 0x114(r1)
/* 800DF1A0 000DC0E0  80 03 00 04 */	lwz r0, 4(r3)
/* 800DF1A4 000DC0E4  90 01 01 18 */	stw r0, 0x118(r1)
lbl_800DF1A8:
/* 800DF1A8 000DC0E8  80 01 01 14 */	lwz r0, 0x114(r1)
/* 800DF1AC 000DC0EC  2C 00 00 01 */	cmpwi r0, 1
/* 800DF1B0 000DC0F0  41 82 00 38 */	beq lbl_800DF1E8
/* 800DF1B4 000DC0F4  40 80 00 3C */	bge lbl_800DF1F0
/* 800DF1B8 000DC0F8  2C 00 00 00 */	cmpwi r0, 0
/* 800DF1BC 000DC0FC  40 80 00 08 */	bge lbl_800DF1C4
/* 800DF1C0 000DC100  48 00 00 30 */	b lbl_800DF1F0
lbl_800DF1C4:
/* 800DF1C4 000DC104  80 61 01 18 */	lwz r3, 0x118(r1)
/* 800DF1C8 000DC108  3C 00 43 30 */	lis r0, 0x4330
/* 800DF1CC 000DC10C  C8 22 8F E0 */	lfd f1, $$23934@sda21(r2)
/* 800DF1D0 000DC110  6C 63 80 00 */	xoris r3, r3, 0x8000
/* 800DF1D4 000DC114  90 61 01 64 */	stw r3, 0x164(r1)
/* 800DF1D8 000DC118  90 01 01 60 */	stw r0, 0x160(r1)
/* 800DF1DC 000DC11C  C8 01 01 60 */	lfd f0, 0x160(r1)
/* 800DF1E0 000DC120  EF C0 08 28 */	fsubs f30, f0, f1
/* 800DF1E4 000DC124  48 00 00 10 */	b lbl_800DF1F4
lbl_800DF1E8:
/* 800DF1E8 000DC128  C3 C1 01 18 */	lfs f30, 0x118(r1)
/* 800DF1EC 000DC12C  48 00 00 08 */	b lbl_800DF1F4
lbl_800DF1F0:
/* 800DF1F0 000DC130  C3 C2 8F D0 */	lfs f30, $$23832@sda21(r2)
lbl_800DF1F4:
/* 800DF1F4 000DC134  80 7E 00 1C */	lwz r3, 0x1c(r30)
/* 800DF1F8 000DC138  2C 03 00 00 */	cmpwi r3, 0
/* 800DF1FC 000DC13C  41 81 00 34 */	bgt lbl_800DF230
/* 800DF200 000DC140  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800DF204 000DC144  4C C6 31 82 */	crclr 6
/* 800DF208 000DC148  4B FF DE 9D */	bl SpcTrace__FPCce
/* 800DF20C 000DC14C  80 1E 00 1C */	lwz r0, 0x1c(r30)
/* 800DF210 000DC150  80 7E 00 20 */	lwz r3, 0x20(r30)
/* 800DF214 000DC154  54 00 18 38 */	slwi r0, r0, 3
/* 800DF218 000DC158  7C 63 02 14 */	add r3, r3, r0
/* 800DF21C 000DC15C  80 03 00 00 */	lwz r0, 0(r3)
/* 800DF220 000DC160  90 01 01 0C */	stw r0, 0x10c(r1)
/* 800DF224 000DC164  80 03 00 04 */	lwz r0, 4(r3)
/* 800DF228 000DC168  90 01 01 10 */	stw r0, 0x110(r1)
/* 800DF22C 000DC16C  48 00 00 2C */	b lbl_800DF258
lbl_800DF230:
/* 800DF230 000DC170  38 03 FF FF */	addi r0, r3, -1
/* 800DF234 000DC174  90 1E 00 1C */	stw r0, 0x1c(r30)
/* 800DF238 000DC178  80 1E 00 1C */	lwz r0, 0x1c(r30)
/* 800DF23C 000DC17C  80 7E 00 20 */	lwz r3, 0x20(r30)
/* 800DF240 000DC180  54 00 18 38 */	slwi r0, r0, 3
/* 800DF244 000DC184  7C 63 02 14 */	add r3, r3, r0
/* 800DF248 000DC188  80 03 00 00 */	lwz r0, 0(r3)
/* 800DF24C 000DC18C  90 01 01 0C */	stw r0, 0x10c(r1)
/* 800DF250 000DC190  80 03 00 04 */	lwz r0, 4(r3)
/* 800DF254 000DC194  90 01 01 10 */	stw r0, 0x110(r1)
lbl_800DF258:
/* 800DF258 000DC198  80 01 01 0C */	lwz r0, 0x10c(r1)
/* 800DF25C 000DC19C  2C 00 00 01 */	cmpwi r0, 1
/* 800DF260 000DC1A0  41 82 00 38 */	beq lbl_800DF298
/* 800DF264 000DC1A4  40 80 00 3C */	bge lbl_800DF2A0
/* 800DF268 000DC1A8  2C 00 00 00 */	cmpwi r0, 0
/* 800DF26C 000DC1AC  40 80 00 08 */	bge lbl_800DF274
/* 800DF270 000DC1B0  48 00 00 30 */	b lbl_800DF2A0
lbl_800DF274:
/* 800DF274 000DC1B4  80 61 01 10 */	lwz r3, 0x110(r1)
/* 800DF278 000DC1B8  3C 00 43 30 */	lis r0, 0x4330
/* 800DF27C 000DC1BC  C8 22 8F E0 */	lfd f1, $$23934@sda21(r2)
/* 800DF280 000DC1C0  6C 63 80 00 */	xoris r3, r3, 0x8000
/* 800DF284 000DC1C4  90 61 01 64 */	stw r3, 0x164(r1)
/* 800DF288 000DC1C8  90 01 01 60 */	stw r0, 0x160(r1)
/* 800DF28C 000DC1CC  C8 01 01 60 */	lfd f0, 0x160(r1)
/* 800DF290 000DC1D0  EF A0 08 28 */	fsubs f29, f0, f1
/* 800DF294 000DC1D4  48 00 00 10 */	b lbl_800DF2A4
lbl_800DF298:
/* 800DF298 000DC1D8  C3 A1 01 10 */	lfs f29, 0x110(r1)
/* 800DF29C 000DC1DC  48 00 00 08 */	b lbl_800DF2A4
lbl_800DF2A0:
/* 800DF2A0 000DC1E0  C3 A2 8F D0 */	lfs f29, $$23832@sda21(r2)
lbl_800DF2A4:
/* 800DF2A4 000DC1E4  80 7E 00 1C */	lwz r3, 0x1c(r30)
/* 800DF2A8 000DC1E8  2C 03 00 00 */	cmpwi r3, 0
/* 800DF2AC 000DC1EC  41 81 00 34 */	bgt lbl_800DF2E0
/* 800DF2B0 000DC1F0  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800DF2B4 000DC1F4  4C C6 31 82 */	crclr 6
/* 800DF2B8 000DC1F8  4B FF DD ED */	bl SpcTrace__FPCce
/* 800DF2BC 000DC1FC  80 1E 00 1C */	lwz r0, 0x1c(r30)
/* 800DF2C0 000DC200  80 7E 00 20 */	lwz r3, 0x20(r30)
/* 800DF2C4 000DC204  54 00 18 38 */	slwi r0, r0, 3
/* 800DF2C8 000DC208  7C 63 02 14 */	add r3, r3, r0
/* 800DF2CC 000DC20C  80 03 00 00 */	lwz r0, 0(r3)
/* 800DF2D0 000DC210  90 01 01 04 */	stw r0, 0x104(r1)
/* 800DF2D4 000DC214  80 03 00 04 */	lwz r0, 4(r3)
/* 800DF2D8 000DC218  90 01 01 08 */	stw r0, 0x108(r1)
/* 800DF2DC 000DC21C  48 00 00 2C */	b lbl_800DF308
lbl_800DF2E0:
/* 800DF2E0 000DC220  38 03 FF FF */	addi r0, r3, -1
/* 800DF2E4 000DC224  90 1E 00 1C */	stw r0, 0x1c(r30)
/* 800DF2E8 000DC228  80 1E 00 1C */	lwz r0, 0x1c(r30)
/* 800DF2EC 000DC22C  80 7E 00 20 */	lwz r3, 0x20(r30)
/* 800DF2F0 000DC230  54 00 18 38 */	slwi r0, r0, 3
/* 800DF2F4 000DC234  7C 63 02 14 */	add r3, r3, r0
/* 800DF2F8 000DC238  80 03 00 00 */	lwz r0, 0(r3)
/* 800DF2FC 000DC23C  90 01 01 04 */	stw r0, 0x104(r1)
/* 800DF300 000DC240  80 03 00 04 */	lwz r0, 4(r3)
/* 800DF304 000DC244  90 01 01 08 */	stw r0, 0x108(r1)
lbl_800DF308:
/* 800DF308 000DC248  80 01 01 04 */	lwz r0, 0x104(r1)
/* 800DF30C 000DC24C  2C 00 00 01 */	cmpwi r0, 1
/* 800DF310 000DC250  41 82 00 38 */	beq lbl_800DF348
/* 800DF314 000DC254  40 80 00 3C */	bge lbl_800DF350
/* 800DF318 000DC258  2C 00 00 00 */	cmpwi r0, 0
/* 800DF31C 000DC25C  40 80 00 08 */	bge lbl_800DF324
/* 800DF320 000DC260  48 00 00 30 */	b lbl_800DF350
lbl_800DF324:
/* 800DF324 000DC264  80 61 01 08 */	lwz r3, 0x108(r1)
/* 800DF328 000DC268  3C 00 43 30 */	lis r0, 0x4330
/* 800DF32C 000DC26C  C8 22 8F E0 */	lfd f1, $$23934@sda21(r2)
/* 800DF330 000DC270  6C 63 80 00 */	xoris r3, r3, 0x8000
/* 800DF334 000DC274  90 61 01 64 */	stw r3, 0x164(r1)
/* 800DF338 000DC278  90 01 01 60 */	stw r0, 0x160(r1)
/* 800DF33C 000DC27C  C8 01 01 60 */	lfd f0, 0x160(r1)
/* 800DF340 000DC280  EF 80 08 28 */	fsubs f28, f0, f1
/* 800DF344 000DC284  48 00 00 10 */	b lbl_800DF354
lbl_800DF348:
/* 800DF348 000DC288  C3 81 01 08 */	lfs f28, 0x108(r1)
/* 800DF34C 000DC28C  48 00 00 08 */	b lbl_800DF354
lbl_800DF350:
/* 800DF350 000DC290  C3 82 8F D0 */	lfs f28, $$23832@sda21(r2)
lbl_800DF354:
/* 800DF354 000DC294  80 7E 00 1C */	lwz r3, 0x1c(r30)
/* 800DF358 000DC298  2C 03 00 00 */	cmpwi r3, 0
/* 800DF35C 000DC29C  41 81 00 34 */	bgt lbl_800DF390
/* 800DF360 000DC2A0  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800DF364 000DC2A4  4C C6 31 82 */	crclr 6
/* 800DF368 000DC2A8  4B FF DD 3D */	bl SpcTrace__FPCce
/* 800DF36C 000DC2AC  80 1E 00 1C */	lwz r0, 0x1c(r30)
/* 800DF370 000DC2B0  80 7E 00 20 */	lwz r3, 0x20(r30)
/* 800DF374 000DC2B4  54 00 18 38 */	slwi r0, r0, 3
/* 800DF378 000DC2B8  7C 63 02 14 */	add r3, r3, r0
/* 800DF37C 000DC2BC  80 03 00 00 */	lwz r0, 0(r3)
/* 800DF380 000DC2C0  90 01 00 FC */	stw r0, 0xfc(r1)
/* 800DF384 000DC2C4  80 03 00 04 */	lwz r0, 4(r3)
/* 800DF388 000DC2C8  90 01 01 00 */	stw r0, 0x100(r1)
/* 800DF38C 000DC2CC  48 00 00 2C */	b lbl_800DF3B8
lbl_800DF390:
/* 800DF390 000DC2D0  38 03 FF FF */	addi r0, r3, -1
/* 800DF394 000DC2D4  90 1E 00 1C */	stw r0, 0x1c(r30)
/* 800DF398 000DC2D8  80 1E 00 1C */	lwz r0, 0x1c(r30)
/* 800DF39C 000DC2DC  80 7E 00 20 */	lwz r3, 0x20(r30)
/* 800DF3A0 000DC2E0  54 00 18 38 */	slwi r0, r0, 3
/* 800DF3A4 000DC2E4  7C 63 02 14 */	add r3, r3, r0
/* 800DF3A8 000DC2E8  80 03 00 00 */	lwz r0, 0(r3)
/* 800DF3AC 000DC2EC  90 01 00 FC */	stw r0, 0xfc(r1)
/* 800DF3B0 000DC2F0  80 03 00 04 */	lwz r0, 4(r3)
/* 800DF3B4 000DC2F4  90 01 01 00 */	stw r0, 0x100(r1)
lbl_800DF3B8:
/* 800DF3B8 000DC2F8  80 01 00 FC */	lwz r0, 0xfc(r1)
/* 800DF3BC 000DC2FC  2C 00 00 01 */	cmpwi r0, 1
/* 800DF3C0 000DC300  41 82 00 38 */	beq lbl_800DF3F8
/* 800DF3C4 000DC304  40 80 00 3C */	bge lbl_800DF400
/* 800DF3C8 000DC308  2C 00 00 00 */	cmpwi r0, 0
/* 800DF3CC 000DC30C  40 80 00 08 */	bge lbl_800DF3D4
/* 800DF3D0 000DC310  48 00 00 30 */	b lbl_800DF400
lbl_800DF3D4:
/* 800DF3D4 000DC314  80 61 01 00 */	lwz r3, 0x100(r1)
/* 800DF3D8 000DC318  3C 00 43 30 */	lis r0, 0x4330
/* 800DF3DC 000DC31C  C8 22 8F E0 */	lfd f1, $$23934@sda21(r2)
/* 800DF3E0 000DC320  6C 63 80 00 */	xoris r3, r3, 0x8000
/* 800DF3E4 000DC324  90 61 01 64 */	stw r3, 0x164(r1)
/* 800DF3E8 000DC328  90 01 01 60 */	stw r0, 0x160(r1)
/* 800DF3EC 000DC32C  C8 01 01 60 */	lfd f0, 0x160(r1)
/* 800DF3F0 000DC330  EF 60 08 28 */	fsubs f27, f0, f1
/* 800DF3F4 000DC334  48 00 00 10 */	b lbl_800DF404
lbl_800DF3F8:
/* 800DF3F8 000DC338  C3 61 01 00 */	lfs f27, 0x100(r1)
/* 800DF3FC 000DC33C  48 00 00 08 */	b lbl_800DF404
lbl_800DF400:
/* 800DF400 000DC340  C3 62 8F D0 */	lfs f27, $$23832@sda21(r2)
lbl_800DF404:
/* 800DF404 000DC344  80 7E 00 1C */	lwz r3, 0x1c(r30)
/* 800DF408 000DC348  2C 03 00 00 */	cmpwi r3, 0
/* 800DF40C 000DC34C  41 81 00 34 */	bgt lbl_800DF440
/* 800DF410 000DC350  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800DF414 000DC354  4C C6 31 82 */	crclr 6
/* 800DF418 000DC358  4B FF DC 8D */	bl SpcTrace__FPCce
/* 800DF41C 000DC35C  80 1E 00 1C */	lwz r0, 0x1c(r30)
/* 800DF420 000DC360  80 7E 00 20 */	lwz r3, 0x20(r30)
/* 800DF424 000DC364  54 00 18 38 */	slwi r0, r0, 3
/* 800DF428 000DC368  7C 63 02 14 */	add r3, r3, r0
/* 800DF42C 000DC36C  80 03 00 00 */	lwz r0, 0(r3)
/* 800DF430 000DC370  90 01 00 F4 */	stw r0, 0xf4(r1)
/* 800DF434 000DC374  80 03 00 04 */	lwz r0, 4(r3)
/* 800DF438 000DC378  90 01 00 F8 */	stw r0, 0xf8(r1)
/* 800DF43C 000DC37C  48 00 00 2C */	b lbl_800DF468
lbl_800DF440:
/* 800DF440 000DC380  38 03 FF FF */	addi r0, r3, -1
/* 800DF444 000DC384  90 1E 00 1C */	stw r0, 0x1c(r30)
/* 800DF448 000DC388  80 1E 00 1C */	lwz r0, 0x1c(r30)
/* 800DF44C 000DC38C  80 7E 00 20 */	lwz r3, 0x20(r30)
/* 800DF450 000DC390  54 00 18 38 */	slwi r0, r0, 3
/* 800DF454 000DC394  7C 63 02 14 */	add r3, r3, r0
/* 800DF458 000DC398  80 03 00 00 */	lwz r0, 0(r3)
/* 800DF45C 000DC39C  90 01 00 F4 */	stw r0, 0xf4(r1)
/* 800DF460 000DC3A0  80 03 00 04 */	lwz r0, 4(r3)
/* 800DF464 000DC3A4  90 01 00 F8 */	stw r0, 0xf8(r1)
lbl_800DF468:
/* 800DF468 000DC3A8  80 01 00 F4 */	lwz r0, 0xf4(r1)
/* 800DF46C 000DC3AC  2C 00 00 01 */	cmpwi r0, 1
/* 800DF470 000DC3B0  41 82 00 38 */	beq lbl_800DF4A8
/* 800DF474 000DC3B4  40 80 00 3C */	bge lbl_800DF4B0
/* 800DF478 000DC3B8  2C 00 00 00 */	cmpwi r0, 0
/* 800DF47C 000DC3BC  40 80 00 08 */	bge lbl_800DF484
/* 800DF480 000DC3C0  48 00 00 30 */	b lbl_800DF4B0
lbl_800DF484:
/* 800DF484 000DC3C4  80 61 00 F8 */	lwz r3, 0xf8(r1)
/* 800DF488 000DC3C8  3C 00 43 30 */	lis r0, 0x4330
/* 800DF48C 000DC3CC  C8 22 8F E0 */	lfd f1, $$23934@sda21(r2)
/* 800DF490 000DC3D0  6C 63 80 00 */	xoris r3, r3, 0x8000
/* 800DF494 000DC3D4  90 61 01 64 */	stw r3, 0x164(r1)
/* 800DF498 000DC3D8  90 01 01 60 */	stw r0, 0x160(r1)
/* 800DF49C 000DC3DC  C8 01 01 60 */	lfd f0, 0x160(r1)
/* 800DF4A0 000DC3E0  EF 40 08 28 */	fsubs f26, f0, f1
/* 800DF4A4 000DC3E4  48 00 00 10 */	b lbl_800DF4B4
lbl_800DF4A8:
/* 800DF4A8 000DC3E8  C3 41 00 F8 */	lfs f26, 0xf8(r1)
/* 800DF4AC 000DC3EC  48 00 00 08 */	b lbl_800DF4B4
lbl_800DF4B0:
/* 800DF4B0 000DC3F0  C3 42 8F D0 */	lfs f26, $$23832@sda21(r2)
lbl_800DF4B4:
/* 800DF4B4 000DC3F4  80 7E 00 1C */	lwz r3, 0x1c(r30)
/* 800DF4B8 000DC3F8  2C 03 00 00 */	cmpwi r3, 0
/* 800DF4BC 000DC3FC  41 81 00 34 */	bgt lbl_800DF4F0
/* 800DF4C0 000DC400  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800DF4C4 000DC404  4C C6 31 82 */	crclr 6
/* 800DF4C8 000DC408  4B FF DB DD */	bl SpcTrace__FPCce
/* 800DF4CC 000DC40C  80 1E 00 1C */	lwz r0, 0x1c(r30)
/* 800DF4D0 000DC410  80 7E 00 20 */	lwz r3, 0x20(r30)
/* 800DF4D4 000DC414  54 00 18 38 */	slwi r0, r0, 3
/* 800DF4D8 000DC418  7C 63 02 14 */	add r3, r3, r0
/* 800DF4DC 000DC41C  80 03 00 00 */	lwz r0, 0(r3)
/* 800DF4E0 000DC420  90 01 00 E4 */	stw r0, 0xe4(r1)
/* 800DF4E4 000DC424  80 03 00 04 */	lwz r0, 4(r3)
/* 800DF4E8 000DC428  90 01 00 E8 */	stw r0, 0xe8(r1)
/* 800DF4EC 000DC42C  48 00 00 2C */	b lbl_800DF518
lbl_800DF4F0:
/* 800DF4F0 000DC430  38 03 FF FF */	addi r0, r3, -1
/* 800DF4F4 000DC434  90 1E 00 1C */	stw r0, 0x1c(r30)
/* 800DF4F8 000DC438  80 1E 00 1C */	lwz r0, 0x1c(r30)
/* 800DF4FC 000DC43C  80 7E 00 20 */	lwz r3, 0x20(r30)
/* 800DF500 000DC440  54 00 18 38 */	slwi r0, r0, 3
/* 800DF504 000DC444  7C 63 02 14 */	add r3, r3, r0
/* 800DF508 000DC448  80 03 00 00 */	lwz r0, 0(r3)
/* 800DF50C 000DC44C  90 01 00 E4 */	stw r0, 0xe4(r1)
/* 800DF510 000DC450  80 03 00 04 */	lwz r0, 4(r3)
/* 800DF514 000DC454  90 01 00 E8 */	stw r0, 0xe8(r1)
lbl_800DF518:
/* 800DF518 000DC458  80 81 00 E4 */	lwz r4, 0xe4(r1)
/* 800DF51C 000DC45C  38 60 00 00 */	li r3, 0
/* 800DF520 000DC460  80 01 00 E8 */	lwz r0, 0xe8(r1)
/* 800DF524 000DC464  90 81 00 EC */	stw r4, 0xec(r1)
/* 800DF528 000DC468  90 01 00 F0 */	stw r0, 0xf0(r1)
/* 800DF52C 000DC46C  80 01 00 EC */	lwz r0, 0xec(r1)
/* 800DF530 000DC470  2C 00 00 01 */	cmpwi r0, 1
/* 800DF534 000DC474  41 82 00 9C */	beq lbl_800DF5D0
/* 800DF538 000DC478  40 80 00 10 */	bge lbl_800DF548
/* 800DF53C 000DC47C  2C 00 00 00 */	cmpwi r0, 0
/* 800DF540 000DC480  40 80 00 58 */	bge lbl_800DF598
/* 800DF544 000DC484  48 00 00 8C */	b lbl_800DF5D0
lbl_800DF548:
/* 800DF548 000DC488  2C 00 00 03 */	cmpwi r0, 3
/* 800DF54C 000DC48C  40 80 00 84 */	bge lbl_800DF5D0
/* 800DF550 000DC490  2C 00 00 02 */	cmpwi r0, 2
/* 800DF554 000DC494  41 82 00 08 */	beq lbl_800DF55C
/* 800DF558 000DC498  48 00 00 0C */	b lbl_800DF564
lbl_800DF55C:
/* 800DF55C 000DC49C  83 A1 00 F0 */	lwz r29, 0xf0(r1)
/* 800DF560 000DC4A0  48 00 00 08 */	b lbl_800DF568
lbl_800DF564:
/* 800DF564 000DC4A4  3B A2 8F F0 */	addi r29, r2, $$24053@sda21
lbl_800DF568:
/* 800DF568 000DC4A8  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 800DF56C 000DC4AC  7F A3 EB 78 */	mr r3, r29
/* 800DF570 000DC4B0  83 84 00 04 */	lwz r28, 4(r4)
/* 800DF574 000DC4B4  4B F6 51 19 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800DF578 000DC4B8  81 9C 00 00 */	lwz r12, 0(r28)
/* 800DF57C 000DC4BC  38 83 00 00 */	addi r4, r3, 0
/* 800DF580 000DC4C0  38 7C 00 00 */	addi r3, r28, 0
/* 800DF584 000DC4C4  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 800DF588 000DC4C8  7F A5 EB 78 */	mr r5, r29
/* 800DF58C 000DC4CC  7D 88 03 A6 */	mtlr r12
/* 800DF590 000DC4D0  4E 80 00 21 */	blrl 
/* 800DF594 000DC4D4  48 00 00 3C */	b lbl_800DF5D0
lbl_800DF598:
/* 800DF598 000DC4D8  2C 00 00 01 */	cmpwi r0, 1
/* 800DF59C 000DC4DC  41 82 00 1C */	beq lbl_800DF5B8
/* 800DF5A0 000DC4E0  40 80 00 2C */	bge lbl_800DF5CC
/* 800DF5A4 000DC4E4  2C 00 00 00 */	cmpwi r0, 0
/* 800DF5A8 000DC4E8  40 80 00 08 */	bge lbl_800DF5B0
/* 800DF5AC 000DC4EC  48 00 00 20 */	b lbl_800DF5CC
lbl_800DF5B0:
/* 800DF5B0 000DC4F0  80 61 00 F0 */	lwz r3, 0xf0(r1)
/* 800DF5B4 000DC4F4  48 00 00 1C */	b lbl_800DF5D0
lbl_800DF5B8:
/* 800DF5B8 000DC4F8  C0 01 00 F0 */	lfs f0, 0xf0(r1)
/* 800DF5BC 000DC4FC  FC 00 00 1E */	fctiwz f0, f0
/* 800DF5C0 000DC500  D8 01 01 60 */	stfd f0, 0x160(r1)
/* 800DF5C4 000DC504  80 61 01 64 */	lwz r3, 0x164(r1)
/* 800DF5C8 000DC508  48 00 00 08 */	b lbl_800DF5D0
lbl_800DF5CC:
/* 800DF5CC 000DC50C  38 60 00 00 */	li r3, 0
lbl_800DF5D0:
/* 800DF5D0 000DC510  7C 7C 1B 78 */	mr r28, r3
/* 800DF5D4 000DC514  81 9C 00 00 */	lwz r12, 0(r28)
/* 800DF5D8 000DC518  81 8C 01 00 */	lwz r12, 0x100(r12)
/* 800DF5DC 000DC51C  7D 88 03 A6 */	mtlr r12
/* 800DF5E0 000DC520  4E 80 00 21 */	blrl 
/* 800DF5E4 000DC524  C0 02 8F D0 */	lfs f0, $$23832@sda21(r2)
/* 800DF5E8 000DC528  7F 83 E3 78 */	mr r3, r28
/* 800DF5EC 000DC52C  D3 41 01 3C */	stfs f26, 0x13c(r1)
/* 800DF5F0 000DC530  38 81 01 3C */	addi r4, r1, 0x13c
/* 800DF5F4 000DC534  38 A1 01 30 */	addi r5, r1, 0x130
/* 800DF5F8 000DC538  D0 01 01 30 */	stfs f0, 0x130(r1)
/* 800DF5FC 000DC53C  38 C1 01 24 */	addi r6, r1, 0x124
/* 800DF600 000DC540  D3 A1 01 24 */	stfs f29, 0x124(r1)
/* 800DF604 000DC544  D3 61 01 40 */	stfs f27, 0x140(r1)
/* 800DF608 000DC548  D0 01 01 34 */	stfs f0, 0x134(r1)
/* 800DF60C 000DC54C  D3 C1 01 28 */	stfs f30, 0x128(r1)
/* 800DF610 000DC550  D3 81 01 44 */	stfs f28, 0x144(r1)
/* 800DF614 000DC554  D0 01 01 38 */	stfs f0, 0x138(r1)
/* 800DF618 000DC558  D3 E1 01 2C */	stfs f31, 0x12c(r1)
/* 800DF61C 000DC55C  81 9C 00 00 */	lwz r12, 0(r28)
/* 800DF620 000DC560  81 8C 01 08 */	lwz r12, 0x108(r12)
/* 800DF624 000DC564  7D 88 03 A6 */	mtlr r12
/* 800DF628 000DC568  4E 80 00 21 */	blrl 
/* 800DF62C 000DC56C  38 00 00 00 */	li r0, 0
/* 800DF630 000DC570  90 01 00 BC */	stw r0, 0xbc(r1)
/* 800DF634 000DC574  90 01 00 C0 */	stw r0, 0xc0(r1)
/* 800DF638 000DC578  80 7E 00 1C */	lwz r3, 0x1c(r30)
/* 800DF63C 000DC57C  80 1E 00 18 */	lwz r0, 0x18(r30)
/* 800DF640 000DC580  7C 03 00 00 */	cmpw r3, r0
/* 800DF644 000DC584  41 80 00 14 */	blt lbl_800DF658
/* 800DF648 000DC588  38 7F 07 8C */	addi r3, r31, 0x78c
/* 800DF64C 000DC58C  4C C6 31 82 */	crclr 6
/* 800DF650 000DC590  4B FF DA 55 */	bl SpcTrace__FPCce
/* 800DF654 000DC594  48 00 00 2C */	b lbl_800DF680
lbl_800DF658:
/* 800DF658 000DC598  80 BE 00 20 */	lwz r5, 0x20(r30)
/* 800DF65C 000DC59C  54 64 18 38 */	slwi r4, r3, 3
/* 800DF660 000DC5A0  80 61 00 BC */	lwz r3, 0xbc(r1)
/* 800DF664 000DC5A4  80 01 00 C0 */	lwz r0, 0xc0(r1)
/* 800DF668 000DC5A8  7C 85 22 14 */	add r4, r5, r4
/* 800DF66C 000DC5AC  90 64 00 00 */	stw r3, 0(r4)
/* 800DF670 000DC5B0  90 04 00 04 */	stw r0, 4(r4)
/* 800DF674 000DC5B4  80 7E 00 1C */	lwz r3, 0x1c(r30)
/* 800DF678 000DC5B8  38 03 00 01 */	addi r0, r3, 1
/* 800DF67C 000DC5BC  90 1E 00 1C */	stw r0, 0x1c(r30)
lbl_800DF680:
/* 800DF680 000DC5C0  80 01 01 AC */	lwz r0, 0x1ac(r1)
/* 800DF684 000DC5C4  CB E1 01 A0 */	lfd f31, 0x1a0(r1)
/* 800DF688 000DC5C8  CB C1 01 98 */	lfd f30, 0x198(r1)
/* 800DF68C 000DC5CC  7C 08 03 A6 */	mtlr r0
/* 800DF690 000DC5D0  CB A1 01 90 */	lfd f29, 0x190(r1)
/* 800DF694 000DC5D4  CB 81 01 88 */	lfd f28, 0x188(r1)
/* 800DF698 000DC5D8  CB 61 01 80 */	lfd f27, 0x180(r1)
/* 800DF69C 000DC5DC  CB 41 01 78 */	lfd f26, 0x178(r1)
/* 800DF6A0 000DC5E0  83 E1 01 74 */	lwz r31, 0x174(r1)
/* 800DF6A4 000DC5E4  83 C1 01 70 */	lwz r30, 0x170(r1)
/* 800DF6A8 000DC5E8  83 A1 01 6C */	lwz r29, 0x16c(r1)
/* 800DF6AC 000DC5EC  83 81 01 68 */	lwz r28, 0x168(r1)
/* 800DF6B0 000DC5F0  38 21 01 A8 */	addi r1, r1, 0x1a8
/* 800DF6B4 000DC5F4  4E 80 00 20 */	blr 

.global evIsInsideFastCube__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evIsInsideFastCube__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800DF6B8 000DC5F8  7C 08 02 A6 */	mflr r0
/* 800DF6BC 000DC5FC  90 01 00 04 */	stw r0, 4(r1)
/* 800DF6C0 000DC600  94 21 FF 78 */	stwu r1, -0x88(r1)
/* 800DF6C4 000DC604  93 E1 00 84 */	stw r31, 0x84(r1)
/* 800DF6C8 000DC608  38 A1 00 0C */	addi r5, r1, 0xc
/* 800DF6CC 000DC60C  93 C1 00 80 */	stw r30, 0x80(r1)
/* 800DF6D0 000DC610  93 A1 00 7C */	stw r29, 0x7c(r1)
/* 800DF6D4 000DC614  7C 7D 1B 78 */	mr r29, r3
/* 800DF6D8 000DC618  90 81 00 0C */	stw r4, 0xc(r1)
/* 800DF6DC 000DC61C  3C 80 80 37 */	lis r4, $$21490@ha
/* 800DF6E0 000DC620  3B E4 29 90 */	addi r31, r4, $$21490@l
/* 800DF6E4 000DC624  38 80 00 02 */	li r4, 2
/* 800DF6E8 000DC628  48 03 7D 49 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800DF6EC 000DC62C  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800DF6F0 000DC630  2C 03 00 00 */	cmpwi r3, 0
/* 800DF6F4 000DC634  41 81 00 34 */	bgt lbl_800DF728
/* 800DF6F8 000DC638  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800DF6FC 000DC63C  4C C6 31 82 */	crclr 6
/* 800DF700 000DC640  4B FF D9 A5 */	bl SpcTrace__FPCce
/* 800DF704 000DC644  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800DF708 000DC648  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800DF70C 000DC64C  54 00 18 38 */	slwi r0, r0, 3
/* 800DF710 000DC650  7C 63 02 14 */	add r3, r3, r0
/* 800DF714 000DC654  80 03 00 00 */	lwz r0, 0(r3)
/* 800DF718 000DC658  90 01 00 54 */	stw r0, 0x54(r1)
/* 800DF71C 000DC65C  80 03 00 04 */	lwz r0, 4(r3)
/* 800DF720 000DC660  90 01 00 58 */	stw r0, 0x58(r1)
/* 800DF724 000DC664  48 00 00 2C */	b lbl_800DF750
lbl_800DF728:
/* 800DF728 000DC668  38 03 FF FF */	addi r0, r3, -1
/* 800DF72C 000DC66C  90 1D 00 1C */	stw r0, 0x1c(r29)
/* 800DF730 000DC670  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800DF734 000DC674  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800DF738 000DC678  54 00 18 38 */	slwi r0, r0, 3
/* 800DF73C 000DC67C  7C 63 02 14 */	add r3, r3, r0
/* 800DF740 000DC680  80 03 00 00 */	lwz r0, 0(r3)
/* 800DF744 000DC684  90 01 00 54 */	stw r0, 0x54(r1)
/* 800DF748 000DC688  80 03 00 04 */	lwz r0, 4(r3)
/* 800DF74C 000DC68C  90 01 00 58 */	stw r0, 0x58(r1)
lbl_800DF750:
/* 800DF750 000DC690  80 01 00 54 */	lwz r0, 0x54(r1)
/* 800DF754 000DC694  2C 00 00 01 */	cmpwi r0, 1
/* 800DF758 000DC698  41 82 00 1C */	beq lbl_800DF774
/* 800DF75C 000DC69C  40 80 00 2C */	bge lbl_800DF788
/* 800DF760 000DC6A0  2C 00 00 00 */	cmpwi r0, 0
/* 800DF764 000DC6A4  40 80 00 08 */	bge lbl_800DF76C
/* 800DF768 000DC6A8  48 00 00 20 */	b lbl_800DF788
lbl_800DF76C:
/* 800DF76C 000DC6AC  80 01 00 58 */	lwz r0, 0x58(r1)
/* 800DF770 000DC6B0  48 00 00 1C */	b lbl_800DF78C
lbl_800DF774:
/* 800DF774 000DC6B4  C0 01 00 58 */	lfs f0, 0x58(r1)
/* 800DF778 000DC6B8  FC 00 00 1E */	fctiwz f0, f0
/* 800DF77C 000DC6BC  D8 01 00 70 */	stfd f0, 0x70(r1)
/* 800DF780 000DC6C0  80 01 00 74 */	lwz r0, 0x74(r1)
/* 800DF784 000DC6C4  48 00 00 08 */	b lbl_800DF78C
lbl_800DF788:
/* 800DF788 000DC6C8  38 00 00 00 */	li r0, 0
lbl_800DF78C:
/* 800DF78C 000DC6CC  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800DF790 000DC6D0  7C 1E 03 78 */	mr r30, r0
/* 800DF794 000DC6D4  2C 03 00 00 */	cmpwi r3, 0
/* 800DF798 000DC6D8  41 81 00 34 */	bgt lbl_800DF7CC
/* 800DF79C 000DC6DC  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800DF7A0 000DC6E0  4C C6 31 82 */	crclr 6
/* 800DF7A4 000DC6E4  4B FF D9 01 */	bl SpcTrace__FPCce
/* 800DF7A8 000DC6E8  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800DF7AC 000DC6EC  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800DF7B0 000DC6F0  54 00 18 38 */	slwi r0, r0, 3
/* 800DF7B4 000DC6F4  7C 63 02 14 */	add r3, r3, r0
/* 800DF7B8 000DC6F8  80 03 00 00 */	lwz r0, 0(r3)
/* 800DF7BC 000DC6FC  90 01 00 4C */	stw r0, 0x4c(r1)
/* 800DF7C0 000DC700  80 03 00 04 */	lwz r0, 4(r3)
/* 800DF7C4 000DC704  90 01 00 50 */	stw r0, 0x50(r1)
/* 800DF7C8 000DC708  48 00 00 2C */	b lbl_800DF7F4
lbl_800DF7CC:
/* 800DF7CC 000DC70C  38 03 FF FF */	addi r0, r3, -1
/* 800DF7D0 000DC710  90 1D 00 1C */	stw r0, 0x1c(r29)
/* 800DF7D4 000DC714  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800DF7D8 000DC718  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800DF7DC 000DC71C  54 00 18 38 */	slwi r0, r0, 3
/* 800DF7E0 000DC720  7C 63 02 14 */	add r3, r3, r0
/* 800DF7E4 000DC724  80 03 00 00 */	lwz r0, 0(r3)
/* 800DF7E8 000DC728  90 01 00 4C */	stw r0, 0x4c(r1)
/* 800DF7EC 000DC72C  80 03 00 04 */	lwz r0, 4(r3)
/* 800DF7F0 000DC730  90 01 00 50 */	stw r0, 0x50(r1)
lbl_800DF7F4:
/* 800DF7F4 000DC734  80 01 00 4C */	lwz r0, 0x4c(r1)
/* 800DF7F8 000DC738  2C 00 00 01 */	cmpwi r0, 1
/* 800DF7FC 000DC73C  41 82 00 1C */	beq lbl_800DF818
/* 800DF800 000DC740  40 80 00 2C */	bge lbl_800DF82C
/* 800DF804 000DC744  2C 00 00 00 */	cmpwi r0, 0
/* 800DF808 000DC748  40 80 00 08 */	bge lbl_800DF810
/* 800DF80C 000DC74C  48 00 00 20 */	b lbl_800DF82C
lbl_800DF810:
/* 800DF810 000DC750  80 A1 00 50 */	lwz r5, 0x50(r1)
/* 800DF814 000DC754  48 00 00 1C */	b lbl_800DF830
lbl_800DF818:
/* 800DF818 000DC758  C0 01 00 50 */	lfs f0, 0x50(r1)
/* 800DF81C 000DC75C  FC 00 00 1E */	fctiwz f0, f0
/* 800DF820 000DC760  D8 01 00 70 */	stfd f0, 0x70(r1)
/* 800DF824 000DC764  80 A1 00 74 */	lwz r5, 0x74(r1)
/* 800DF828 000DC768  48 00 00 08 */	b lbl_800DF830
lbl_800DF82C:
/* 800DF82C 000DC76C  38 A0 00 00 */	li r5, 0
lbl_800DF830:
/* 800DF830 000DC770  80 8D 98 B8 */	lwz r4, gpMarioOriginal@sda21(r13)
/* 800DF834 000DC774  2C 05 00 01 */	cmpwi r5, 1
/* 800DF838 000DC778  C0 02 8F F4 */	lfs f0, $$24803@sda21(r2)
/* 800DF83C 000DC77C  80 64 00 10 */	lwz r3, 0x10(r4)
/* 800DF840 000DC780  80 04 00 14 */	lwz r0, 0x14(r4)
/* 800DF844 000DC784  90 61 00 5C */	stw r3, 0x5c(r1)
/* 800DF848 000DC788  90 01 00 60 */	stw r0, 0x60(r1)
/* 800DF84C 000DC78C  80 04 00 18 */	lwz r0, 0x18(r4)
/* 800DF850 000DC790  90 01 00 64 */	stw r0, 0x64(r1)
/* 800DF854 000DC794  C0 21 00 60 */	lfs f1, 0x60(r1)
/* 800DF858 000DC798  EC 01 00 2A */	fadds f0, f1, f0
/* 800DF85C 000DC79C  D0 01 00 60 */	stfs f0, 0x60(r1)
/* 800DF860 000DC7A0  41 82 00 4C */	beq lbl_800DF8AC
/* 800DF864 000DC7A4  40 80 00 10 */	bge lbl_800DF874
/* 800DF868 000DC7A8  2C 05 00 00 */	cmpwi r5, 0
/* 800DF86C 000DC7AC  40 80 00 14 */	bge lbl_800DF880
/* 800DF870 000DC7B0  48 00 00 94 */	b lbl_800DF904
lbl_800DF874:
/* 800DF874 000DC7B4  2C 05 00 03 */	cmpwi r5, 3
/* 800DF878 000DC7B8  40 80 00 8C */	bge lbl_800DF904
/* 800DF87C 000DC7BC  48 00 00 5C */	b lbl_800DF8D8
lbl_800DF880:
/* 800DF880 000DC7C0  80 6D A8 F0 */	lwz r3, gpCubeFastA@sda21(r13)
/* 800DF884 000DC7C4  38 BE 00 00 */	addi r5, r30, 0
/* 800DF888 000DC7C8  38 81 00 5C */	addi r4, r1, 0x5c
/* 800DF88C 000DC7CC  48 27 FD AD */	bl isInCube__16TCubeManagerBaseCFRC3Vecl
/* 800DF890 000DC7D0  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 800DF894 000DC7D4  41 82 00 0C */	beq lbl_800DF8A0
/* 800DF898 000DC7D8  38 00 00 01 */	li r0, 1
/* 800DF89C 000DC7DC  48 00 00 08 */	b lbl_800DF8A4
lbl_800DF8A0:
/* 800DF8A0 000DC7E0  38 00 00 00 */	li r0, 0
lbl_800DF8A4:
/* 800DF8A4 000DC7E4  7C 03 03 78 */	mr r3, r0
/* 800DF8A8 000DC7E8  48 00 00 60 */	b lbl_800DF908
lbl_800DF8AC:
/* 800DF8AC 000DC7EC  80 6D A8 F4 */	lwz r3, gpCubeFastB@sda21(r13)
/* 800DF8B0 000DC7F0  38 BE 00 00 */	addi r5, r30, 0
/* 800DF8B4 000DC7F4  38 81 00 5C */	addi r4, r1, 0x5c
/* 800DF8B8 000DC7F8  48 27 FD 81 */	bl isInCube__16TCubeManagerBaseCFRC3Vecl
/* 800DF8BC 000DC7FC  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 800DF8C0 000DC800  41 82 00 0C */	beq lbl_800DF8CC
/* 800DF8C4 000DC804  38 00 00 01 */	li r0, 1
/* 800DF8C8 000DC808  48 00 00 08 */	b lbl_800DF8D0
lbl_800DF8CC:
/* 800DF8CC 000DC80C  38 00 00 00 */	li r0, 0
lbl_800DF8D0:
/* 800DF8D0 000DC810  7C 03 03 78 */	mr r3, r0
/* 800DF8D4 000DC814  48 00 00 34 */	b lbl_800DF908
lbl_800DF8D8:
/* 800DF8D8 000DC818  80 6D A8 F8 */	lwz r3, gpCubeFastC@sda21(r13)
/* 800DF8DC 000DC81C  38 BE 00 00 */	addi r5, r30, 0
/* 800DF8E0 000DC820  38 81 00 5C */	addi r4, r1, 0x5c
/* 800DF8E4 000DC824  48 27 FD 55 */	bl isInCube__16TCubeManagerBaseCFRC3Vecl
/* 800DF8E8 000DC828  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 800DF8EC 000DC82C  41 82 00 0C */	beq lbl_800DF8F8
/* 800DF8F0 000DC830  38 00 00 01 */	li r0, 1
/* 800DF8F4 000DC834  48 00 00 08 */	b lbl_800DF8FC
lbl_800DF8F8:
/* 800DF8F8 000DC838  38 00 00 00 */	li r0, 0
lbl_800DF8FC:
/* 800DF8FC 000DC83C  7C 03 03 78 */	mr r3, r0
/* 800DF900 000DC840  48 00 00 08 */	b lbl_800DF908
lbl_800DF904:
/* 800DF904 000DC844  38 60 00 00 */	li r3, 0
lbl_800DF908:
/* 800DF908 000DC848  38 00 00 00 */	li r0, 0
/* 800DF90C 000DC84C  90 01 00 38 */	stw r0, 0x38(r1)
/* 800DF910 000DC850  90 61 00 3C */	stw r3, 0x3c(r1)
/* 800DF914 000DC854  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800DF918 000DC858  80 1D 00 18 */	lwz r0, 0x18(r29)
/* 800DF91C 000DC85C  7C 03 00 00 */	cmpw r3, r0
/* 800DF920 000DC860  41 80 00 14 */	blt lbl_800DF934
/* 800DF924 000DC864  38 7F 07 8C */	addi r3, r31, 0x78c
/* 800DF928 000DC868  4C C6 31 82 */	crclr 6
/* 800DF92C 000DC86C  4B FF D7 79 */	bl SpcTrace__FPCce
/* 800DF930 000DC870  48 00 00 2C */	b lbl_800DF95C
lbl_800DF934:
/* 800DF934 000DC874  80 BD 00 20 */	lwz r5, 0x20(r29)
/* 800DF938 000DC878  54 64 18 38 */	slwi r4, r3, 3
/* 800DF93C 000DC87C  80 61 00 38 */	lwz r3, 0x38(r1)
/* 800DF940 000DC880  80 01 00 3C */	lwz r0, 0x3c(r1)
/* 800DF944 000DC884  7C 85 22 14 */	add r4, r5, r4
/* 800DF948 000DC888  90 64 00 00 */	stw r3, 0(r4)
/* 800DF94C 000DC88C  90 04 00 04 */	stw r0, 4(r4)
/* 800DF950 000DC890  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800DF954 000DC894  38 03 00 01 */	addi r0, r3, 1
/* 800DF958 000DC898  90 1D 00 1C */	stw r0, 0x1c(r29)
lbl_800DF95C:
/* 800DF95C 000DC89C  80 01 00 8C */	lwz r0, 0x8c(r1)
/* 800DF960 000DC8A0  83 E1 00 84 */	lwz r31, 0x84(r1)
/* 800DF964 000DC8A4  83 C1 00 80 */	lwz r30, 0x80(r1)
/* 800DF968 000DC8A8  7C 08 03 A6 */	mtlr r0
/* 800DF96C 000DC8AC  83 A1 00 7C */	lwz r29, 0x7c(r1)
/* 800DF970 000DC8B0  38 21 00 88 */	addi r1, r1, 0x88
/* 800DF974 000DC8B4  4E 80 00 20 */	blr 

.global evIsFinishMareBottleDemo__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evIsFinishMareBottleDemo__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800DF978 000DC8B8  7C 08 02 A6 */	mflr r0
/* 800DF97C 000DC8BC  90 01 00 04 */	stw r0, 4(r1)
/* 800DF980 000DC8C0  94 21 FF B8 */	stwu r1, -0x48(r1)
/* 800DF984 000DC8C4  93 E1 00 44 */	stw r31, 0x44(r1)
/* 800DF988 000DC8C8  7C 7F 1B 78 */	mr r31, r3
/* 800DF98C 000DC8CC  38 A1 00 0C */	addi r5, r1, 0xc
/* 800DF990 000DC8D0  93 C1 00 40 */	stw r30, 0x40(r1)
/* 800DF994 000DC8D4  93 A1 00 3C */	stw r29, 0x3c(r1)
/* 800DF998 000DC8D8  90 81 00 0C */	stw r4, 0xc(r1)
/* 800DF99C 000DC8DC  3C 80 80 37 */	lis r4, $$21490@ha
/* 800DF9A0 000DC8E0  3B C4 29 90 */	addi r30, r4, $$21490@l
/* 800DF9A4 000DC8E4  38 80 00 00 */	li r4, 0
/* 800DF9A8 000DC8E8  48 03 7A 89 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800DF9AC 000DC8EC  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 800DF9B0 000DC8F0  38 7E 07 D0 */	addi r3, r30, 0x7d0
/* 800DF9B4 000DC8F4  83 A4 00 04 */	lwz r29, 4(r4)
/* 800DF9B8 000DC8F8  4B F6 4C D5 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800DF9BC 000DC8FC  81 9D 00 00 */	lwz r12, 0(r29)
/* 800DF9C0 000DC900  38 83 00 00 */	addi r4, r3, 0
/* 800DF9C4 000DC904  38 7D 00 00 */	addi r3, r29, 0
/* 800DF9C8 000DC908  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 800DF9CC 000DC90C  38 BE 07 D0 */	addi r5, r30, 0x7d0
/* 800DF9D0 000DC910  7D 88 03 A6 */	mtlr r12
/* 800DF9D4 000DC914  4E 80 00 21 */	blrl 
/* 800DF9D8 000DC918  80 63 00 74 */	lwz r3, 0x74(r3)
/* 800DF9DC 000DC91C  38 80 00 00 */	li r4, 0
/* 800DF9E0 000DC920  38 A0 00 00 */	li r5, 0
/* 800DF9E4 000DC924  4B FF 57 C9 */	bl curAnmEndsNext__6MActorFiPc
/* 800DF9E8 000DC928  2C 03 00 00 */	cmpwi r3, 0
/* 800DF9EC 000DC92C  41 82 00 0C */	beq lbl_800DF9F8
/* 800DF9F0 000DC930  38 60 00 01 */	li r3, 1
/* 800DF9F4 000DC934  48 00 00 08 */	b lbl_800DF9FC
lbl_800DF9F8:
/* 800DF9F8 000DC938  38 60 00 00 */	li r3, 0
lbl_800DF9FC:
/* 800DF9FC 000DC93C  38 00 00 00 */	li r0, 0
/* 800DFA00 000DC940  90 01 00 28 */	stw r0, 0x28(r1)
/* 800DFA04 000DC944  90 61 00 2C */	stw r3, 0x2c(r1)
/* 800DFA08 000DC948  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800DFA0C 000DC94C  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800DFA10 000DC950  7C 03 00 00 */	cmpw r3, r0
/* 800DFA14 000DC954  41 80 00 14 */	blt lbl_800DFA28
/* 800DFA18 000DC958  38 7E 07 8C */	addi r3, r30, 0x78c
/* 800DFA1C 000DC95C  4C C6 31 82 */	crclr 6
/* 800DFA20 000DC960  4B FF D6 85 */	bl SpcTrace__FPCce
/* 800DFA24 000DC964  48 00 00 2C */	b lbl_800DFA50
lbl_800DFA28:
/* 800DFA28 000DC968  80 BF 00 20 */	lwz r5, 0x20(r31)
/* 800DFA2C 000DC96C  54 64 18 38 */	slwi r4, r3, 3
/* 800DFA30 000DC970  80 61 00 28 */	lwz r3, 0x28(r1)
/* 800DFA34 000DC974  80 01 00 2C */	lwz r0, 0x2c(r1)
/* 800DFA38 000DC978  7C 85 22 14 */	add r4, r5, r4
/* 800DFA3C 000DC97C  90 64 00 00 */	stw r3, 0(r4)
/* 800DFA40 000DC980  90 04 00 04 */	stw r0, 4(r4)
/* 800DFA44 000DC984  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800DFA48 000DC988  38 03 00 01 */	addi r0, r3, 1
/* 800DFA4C 000DC98C  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800DFA50:
/* 800DFA50 000DC990  80 01 00 4C */	lwz r0, 0x4c(r1)
/* 800DFA54 000DC994  83 E1 00 44 */	lwz r31, 0x44(r1)
/* 800DFA58 000DC998  83 C1 00 40 */	lwz r30, 0x40(r1)
/* 800DFA5C 000DC99C  7C 08 03 A6 */	mtlr r0
/* 800DFA60 000DC9A0  83 A1 00 3C */	lwz r29, 0x3c(r1)
/* 800DFA64 000DC9A4  38 21 00 48 */	addi r1, r1, 0x48
/* 800DFA68 000DC9A8  4E 80 00 20 */	blr 

.global evStartMareBottleDemo__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evStartMareBottleDemo__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800DFA6C 000DC9AC  7C 08 02 A6 */	mflr r0
/* 800DFA70 000DC9B0  90 01 00 04 */	stw r0, 4(r1)
/* 800DFA74 000DC9B4  94 21 FF C0 */	stwu r1, -0x40(r1)
/* 800DFA78 000DC9B8  93 E1 00 3C */	stw r31, 0x3c(r1)
/* 800DFA7C 000DC9BC  7C 7F 1B 78 */	mr r31, r3
/* 800DFA80 000DC9C0  38 A1 00 0C */	addi r5, r1, 0xc
/* 800DFA84 000DC9C4  93 C1 00 38 */	stw r30, 0x38(r1)
/* 800DFA88 000DC9C8  93 A1 00 34 */	stw r29, 0x34(r1)
/* 800DFA8C 000DC9CC  90 81 00 0C */	stw r4, 0xc(r1)
/* 800DFA90 000DC9D0  3C 80 80 37 */	lis r4, $$21490@ha
/* 800DFA94 000DC9D4  3B A4 29 90 */	addi r29, r4, $$21490@l
/* 800DFA98 000DC9D8  38 80 00 00 */	li r4, 0
/* 800DFA9C 000DC9DC  48 03 79 95 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800DFAA0 000DC9E0  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 800DFAA4 000DC9E4  38 7D 07 D0 */	addi r3, r29, 0x7d0
/* 800DFAA8 000DC9E8  83 C4 00 04 */	lwz r30, 4(r4)
/* 800DFAAC 000DC9EC  4B F6 4B E1 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800DFAB0 000DC9F0  81 9E 00 00 */	lwz r12, 0(r30)
/* 800DFAB4 000DC9F4  38 83 00 00 */	addi r4, r3, 0
/* 800DFAB8 000DC9F8  38 7E 00 00 */	addi r3, r30, 0
/* 800DFABC 000DC9FC  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 800DFAC0 000DCA00  38 BD 07 D0 */	addi r5, r29, 0x7d0
/* 800DFAC4 000DCA04  7D 88 03 A6 */	mtlr r12
/* 800DFAC8 000DCA08  4E 80 00 21 */	blrl 
/* 800DFACC 000DCA0C  7C 7E 1B 78 */	mr r30, r3
/* 800DFAD0 000DCA10  80 63 00 74 */	lwz r3, 0x74(r3)
/* 800DFAD4 000DCA14  38 9D 07 DC */	addi r4, r29, 0x7dc
/* 800DFAD8 000DCA18  4B FF 46 85 */	bl setBck__6MActorFPCc
/* 800DFADC 000DCA1C  80 ED 98 B8 */	lwz r7, gpMarioOriginal@sda21(r13)
/* 800DFAE0 000DCA20  38 80 13 10 */	li r4, 0x1310
/* 800DFAE4 000DCA24  80 7E 00 10 */	lwz r3, 0x10(r30)
/* 800DFAE8 000DCA28  38 A0 00 00 */	li r5, 0
/* 800DFAEC 000DCA2C  80 1E 00 14 */	lwz r0, 0x14(r30)
/* 800DFAF0 000DCA30  38 C0 00 01 */	li r6, 1
/* 800DFAF4 000DCA34  90 67 00 10 */	stw r3, 0x10(r7)
/* 800DFAF8 000DCA38  38 67 00 00 */	addi r3, r7, 0
/* 800DFAFC 000DCA3C  90 07 00 14 */	stw r0, 0x14(r7)
/* 800DFB00 000DCA40  80 1E 00 18 */	lwz r0, 0x18(r30)
/* 800DFB04 000DCA44  90 07 00 18 */	stw r0, 0x18(r7)
/* 800DFB08 000DCA48  48 05 39 1D */	bl changePlayerStatus__6TMarioFUlUlb
/* 800DFB0C 000DCA4C  38 00 00 00 */	li r0, 0
/* 800DFB10 000DCA50  90 01 00 28 */	stw r0, 0x28(r1)
/* 800DFB14 000DCA54  90 01 00 2C */	stw r0, 0x2c(r1)
/* 800DFB18 000DCA58  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800DFB1C 000DCA5C  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800DFB20 000DCA60  7C 03 00 00 */	cmpw r3, r0
/* 800DFB24 000DCA64  41 80 00 14 */	blt lbl_800DFB38
/* 800DFB28 000DCA68  38 7D 07 8C */	addi r3, r29, 0x78c
/* 800DFB2C 000DCA6C  4C C6 31 82 */	crclr 6
/* 800DFB30 000DCA70  4B FF D5 75 */	bl SpcTrace__FPCce
/* 800DFB34 000DCA74  48 00 00 2C */	b lbl_800DFB60
lbl_800DFB38:
/* 800DFB38 000DCA78  80 BF 00 20 */	lwz r5, 0x20(r31)
/* 800DFB3C 000DCA7C  54 64 18 38 */	slwi r4, r3, 3
/* 800DFB40 000DCA80  80 61 00 28 */	lwz r3, 0x28(r1)
/* 800DFB44 000DCA84  80 01 00 2C */	lwz r0, 0x2c(r1)
/* 800DFB48 000DCA88  7C 85 22 14 */	add r4, r5, r4
/* 800DFB4C 000DCA8C  90 64 00 00 */	stw r3, 0(r4)
/* 800DFB50 000DCA90  90 04 00 04 */	stw r0, 4(r4)
/* 800DFB54 000DCA94  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800DFB58 000DCA98  38 03 00 01 */	addi r0, r3, 1
/* 800DFB5C 000DCA9C  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800DFB60:
/* 800DFB60 000DCAA0  80 01 00 44 */	lwz r0, 0x44(r1)
/* 800DFB64 000DCAA4  83 E1 00 3C */	lwz r31, 0x3c(r1)
/* 800DFB68 000DCAA8  83 C1 00 38 */	lwz r30, 0x38(r1)
/* 800DFB6C 000DCAAC  7C 08 03 A6 */	mtlr r0
/* 800DFB70 000DCAB0  83 A1 00 34 */	lwz r29, 0x34(r1)
/* 800DFB74 000DCAB4  38 21 00 40 */	addi r1, r1, 0x40
/* 800DFB78 000DCAB8  4E 80 00 20 */	blr 

.global evSetMarioWaiting__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evSetMarioWaiting__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800DFB7C 000DCABC  7C 08 02 A6 */	mflr r0
/* 800DFB80 000DCAC0  90 01 00 04 */	stw r0, 4(r1)
/* 800DFB84 000DCAC4  94 21 FF E0 */	stwu r1, -0x20(r1)
/* 800DFB88 000DCAC8  93 E1 00 1C */	stw r31, 0x1c(r1)
/* 800DFB8C 000DCACC  7C 7F 1B 78 */	mr r31, r3
/* 800DFB90 000DCAD0  38 A1 00 0C */	addi r5, r1, 0xc
/* 800DFB94 000DCAD4  90 81 00 0C */	stw r4, 0xc(r1)
/* 800DFB98 000DCAD8  38 80 00 00 */	li r4, 0
/* 800DFB9C 000DCADC  48 03 78 95 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800DFBA0 000DCAE0  3C 80 0C 40 */	lis r4, 0x0C400201@ha
/* 800DFBA4 000DCAE4  80 6D 98 B8 */	lwz r3, gpMarioOriginal@sda21(r13)
/* 800DFBA8 000DCAE8  38 84 02 01 */	addi r4, r4, 0x0C400201@l
/* 800DFBAC 000DCAEC  38 A0 00 00 */	li r5, 0
/* 800DFBB0 000DCAF0  38 C0 00 01 */	li r6, 1
/* 800DFBB4 000DCAF4  48 05 38 71 */	bl changePlayerStatus__6TMarioFUlUlb
/* 800DFBB8 000DCAF8  38 00 00 00 */	li r0, 0
/* 800DFBBC 000DCAFC  90 01 00 10 */	stw r0, 0x10(r1)
/* 800DFBC0 000DCB00  90 01 00 14 */	stw r0, 0x14(r1)
/* 800DFBC4 000DCB04  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800DFBC8 000DCB08  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800DFBCC 000DCB0C  7C 03 00 00 */	cmpw r3, r0
/* 800DFBD0 000DCB10  41 80 00 18 */	blt lbl_800DFBE8
/* 800DFBD4 000DCB14  3C 60 80 37 */	lis r3, $$23834@ha
/* 800DFBD8 000DCB18  4C C6 31 82 */	crclr 6
/* 800DFBDC 000DCB1C  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800DFBE0 000DCB20  4B FF D4 C5 */	bl SpcTrace__FPCce
/* 800DFBE4 000DCB24  48 00 00 2C */	b lbl_800DFC10
lbl_800DFBE8:
/* 800DFBE8 000DCB28  80 BF 00 20 */	lwz r5, 0x20(r31)
/* 800DFBEC 000DCB2C  54 64 18 38 */	slwi r4, r3, 3
/* 800DFBF0 000DCB30  80 61 00 10 */	lwz r3, 0x10(r1)
/* 800DFBF4 000DCB34  80 01 00 14 */	lwz r0, 0x14(r1)
/* 800DFBF8 000DCB38  7C 85 22 14 */	add r4, r5, r4
/* 800DFBFC 000DCB3C  90 64 00 00 */	stw r3, 0(r4)
/* 800DFC00 000DCB40  90 04 00 04 */	stw r0, 4(r4)
/* 800DFC04 000DCB44  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800DFC08 000DCB48  38 03 00 01 */	addi r0, r3, 1
/* 800DFC0C 000DCB4C  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800DFC10:
/* 800DFC10 000DCB50  80 01 00 24 */	lwz r0, 0x24(r1)
/* 800DFC14 000DCB54  83 E1 00 1C */	lwz r31, 0x1c(r1)
/* 800DFC18 000DCB58  38 21 00 20 */	addi r1, r1, 0x20
/* 800DFC1C 000DCB5C  7C 08 03 A6 */	mtlr r0
/* 800DFC20 000DCB60  4E 80 00 20 */	blr 

.global evIsInsideCube__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evIsInsideCube__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800DFC24 000DCB64  7C 08 02 A6 */	mflr r0
/* 800DFC28 000DCB68  90 01 00 04 */	stw r0, 4(r1)
/* 800DFC2C 000DCB6C  94 21 FF A0 */	stwu r1, -0x60(r1)
/* 800DFC30 000DCB70  93 E1 00 5C */	stw r31, 0x5c(r1)
/* 800DFC34 000DCB74  7C 7F 1B 78 */	mr r31, r3
/* 800DFC38 000DCB78  38 A1 00 0C */	addi r5, r1, 0xc
/* 800DFC3C 000DCB7C  90 81 00 0C */	stw r4, 0xc(r1)
/* 800DFC40 000DCB80  38 80 00 01 */	li r4, 1
/* 800DFC44 000DCB84  48 03 77 ED */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800DFC48 000DCB88  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800DFC4C 000DCB8C  2C 03 00 00 */	cmpwi r3, 0
/* 800DFC50 000DCB90  41 81 00 38 */	bgt lbl_800DFC88
/* 800DFC54 000DCB94  3C 60 80 37 */	lis r3, $$23829@ha
/* 800DFC58 000DCB98  4C C6 31 82 */	crclr 6
/* 800DFC5C 000DCB9C  38 63 30 FC */	addi r3, r3, $$23829@l
/* 800DFC60 000DCBA0  4B FF D4 45 */	bl SpcTrace__FPCce
/* 800DFC64 000DCBA4  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800DFC68 000DCBA8  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800DFC6C 000DCBAC  54 00 18 38 */	slwi r0, r0, 3
/* 800DFC70 000DCBB0  7C 63 02 14 */	add r3, r3, r0
/* 800DFC74 000DCBB4  80 03 00 00 */	lwz r0, 0(r3)
/* 800DFC78 000DCBB8  90 01 00 34 */	stw r0, 0x34(r1)
/* 800DFC7C 000DCBBC  80 03 00 04 */	lwz r0, 4(r3)
/* 800DFC80 000DCBC0  90 01 00 38 */	stw r0, 0x38(r1)
/* 800DFC84 000DCBC4  48 00 00 2C */	b lbl_800DFCB0
lbl_800DFC88:
/* 800DFC88 000DCBC8  38 03 FF FF */	addi r0, r3, -1
/* 800DFC8C 000DCBCC  90 1F 00 1C */	stw r0, 0x1c(r31)
/* 800DFC90 000DCBD0  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800DFC94 000DCBD4  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800DFC98 000DCBD8  54 00 18 38 */	slwi r0, r0, 3
/* 800DFC9C 000DCBDC  7C 63 02 14 */	add r3, r3, r0
/* 800DFCA0 000DCBE0  80 03 00 00 */	lwz r0, 0(r3)
/* 800DFCA4 000DCBE4  90 01 00 34 */	stw r0, 0x34(r1)
/* 800DFCA8 000DCBE8  80 03 00 04 */	lwz r0, 4(r3)
/* 800DFCAC 000DCBEC  90 01 00 38 */	stw r0, 0x38(r1)
lbl_800DFCB0:
/* 800DFCB0 000DCBF0  80 01 00 34 */	lwz r0, 0x34(r1)
/* 800DFCB4 000DCBF4  2C 00 00 01 */	cmpwi r0, 1
/* 800DFCB8 000DCBF8  41 82 00 1C */	beq lbl_800DFCD4
/* 800DFCBC 000DCBFC  40 80 00 2C */	bge lbl_800DFCE8
/* 800DFCC0 000DCC00  2C 00 00 00 */	cmpwi r0, 0
/* 800DFCC4 000DCC04  40 80 00 08 */	bge lbl_800DFCCC
/* 800DFCC8 000DCC08  48 00 00 20 */	b lbl_800DFCE8
lbl_800DFCCC:
/* 800DFCCC 000DCC0C  80 01 00 38 */	lwz r0, 0x38(r1)
/* 800DFCD0 000DCC10  48 00 00 1C */	b lbl_800DFCEC
lbl_800DFCD4:
/* 800DFCD4 000DCC14  C0 01 00 38 */	lfs f0, 0x38(r1)
/* 800DFCD8 000DCC18  FC 00 00 1E */	fctiwz f0, f0
/* 800DFCDC 000DCC1C  D8 01 00 50 */	stfd f0, 0x50(r1)
/* 800DFCE0 000DCC20  80 01 00 54 */	lwz r0, 0x54(r1)
/* 800DFCE4 000DCC24  48 00 00 08 */	b lbl_800DFCEC
lbl_800DFCE8:
/* 800DFCE8 000DCC28  38 00 00 00 */	li r0, 0
lbl_800DFCEC:
/* 800DFCEC 000DCC2C  80 ED 98 B8 */	lwz r7, gpMarioOriginal@sda21(r13)
/* 800DFCF0 000DCC30  7C 05 03 78 */	mr r5, r0
/* 800DFCF4 000DCC34  C0 02 8F F4 */	lfs f0, $$24803@sda21(r2)
/* 800DFCF8 000DCC38  38 81 00 3C */	addi r4, r1, 0x3c
/* 800DFCFC 000DCC3C  80 C7 00 10 */	lwz r6, 0x10(r7)
/* 800DFD00 000DCC40  80 07 00 14 */	lwz r0, 0x14(r7)
/* 800DFD04 000DCC44  80 6D A8 EC */	lwz r3, gpCubeArea@sda21(r13)
/* 800DFD08 000DCC48  90 C1 00 3C */	stw r6, 0x3c(r1)
/* 800DFD0C 000DCC4C  90 01 00 40 */	stw r0, 0x40(r1)
/* 800DFD10 000DCC50  80 07 00 18 */	lwz r0, 0x18(r7)
/* 800DFD14 000DCC54  90 01 00 44 */	stw r0, 0x44(r1)
/* 800DFD18 000DCC58  C0 21 00 40 */	lfs f1, 0x40(r1)
/* 800DFD1C 000DCC5C  EC 01 00 2A */	fadds f0, f1, f0
/* 800DFD20 000DCC60  D0 01 00 40 */	stfs f0, 0x40(r1)
/* 800DFD24 000DCC64  48 27 F9 15 */	bl isInCube__16TCubeManagerBaseCFRC3Vecl
/* 800DFD28 000DCC68  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 800DFD2C 000DCC6C  41 82 00 0C */	beq lbl_800DFD38
/* 800DFD30 000DCC70  38 60 00 01 */	li r3, 1
/* 800DFD34 000DCC74  48 00 00 08 */	b lbl_800DFD3C
lbl_800DFD38:
/* 800DFD38 000DCC78  38 60 00 00 */	li r3, 0
lbl_800DFD3C:
/* 800DFD3C 000DCC7C  38 00 00 00 */	li r0, 0
/* 800DFD40 000DCC80  90 01 00 24 */	stw r0, 0x24(r1)
/* 800DFD44 000DCC84  90 61 00 28 */	stw r3, 0x28(r1)
/* 800DFD48 000DCC88  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800DFD4C 000DCC8C  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800DFD50 000DCC90  7C 03 00 00 */	cmpw r3, r0
/* 800DFD54 000DCC94  41 80 00 18 */	blt lbl_800DFD6C
/* 800DFD58 000DCC98  3C 60 80 37 */	lis r3, $$23834@ha
/* 800DFD5C 000DCC9C  4C C6 31 82 */	crclr 6
/* 800DFD60 000DCCA0  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800DFD64 000DCCA4  4B FF D3 41 */	bl SpcTrace__FPCce
/* 800DFD68 000DCCA8  48 00 00 2C */	b lbl_800DFD94
lbl_800DFD6C:
/* 800DFD6C 000DCCAC  80 BF 00 20 */	lwz r5, 0x20(r31)
/* 800DFD70 000DCCB0  54 64 18 38 */	slwi r4, r3, 3
/* 800DFD74 000DCCB4  80 61 00 24 */	lwz r3, 0x24(r1)
/* 800DFD78 000DCCB8  80 01 00 28 */	lwz r0, 0x28(r1)
/* 800DFD7C 000DCCBC  7C 85 22 14 */	add r4, r5, r4
/* 800DFD80 000DCCC0  90 64 00 00 */	stw r3, 0(r4)
/* 800DFD84 000DCCC4  90 04 00 04 */	stw r0, 4(r4)
/* 800DFD88 000DCCC8  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800DFD8C 000DCCCC  38 03 00 01 */	addi r0, r3, 1
/* 800DFD90 000DCCD0  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800DFD94:
/* 800DFD94 000DCCD4  80 01 00 64 */	lwz r0, 0x64(r1)
/* 800DFD98 000DCCD8  83 E1 00 5C */	lwz r31, 0x5c(r1)
/* 800DFD9C 000DCCDC  38 21 00 60 */	addi r1, r1, 0x60
/* 800DFDA0 000DCCE0  7C 08 03 A6 */	mtlr r0
/* 800DFDA4 000DCCE4  4E 80 00 20 */	blr 

.global evKillWoodBox__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evKillWoodBox__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800DFDA8 000DCCE8  7C 08 02 A6 */	mflr r0
/* 800DFDAC 000DCCEC  90 01 00 04 */	stw r0, 4(r1)
/* 800DFDB0 000DCCF0  94 21 FF 58 */	stwu r1, -0xa8(r1)
/* 800DFDB4 000DCCF4  BF 41 00 90 */	stmw r26, 0x90(r1)
/* 800DFDB8 000DCCF8  7C 7B 1B 78 */	mr r27, r3
/* 800DFDBC 000DCCFC  38 A1 00 0C */	addi r5, r1, 0xc
/* 800DFDC0 000DCD00  90 81 00 0C */	stw r4, 0xc(r1)
/* 800DFDC4 000DCD04  3C 80 80 37 */	lis r4, $$21490@ha
/* 800DFDC8 000DCD08  3B C4 29 90 */	addi r30, r4, $$21490@l
/* 800DFDCC 000DCD0C  38 80 00 02 */	li r4, 2
/* 800DFDD0 000DCD10  48 03 76 61 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800DFDD4 000DCD14  80 7B 00 1C */	lwz r3, 0x1c(r27)
/* 800DFDD8 000DCD18  2C 03 00 00 */	cmpwi r3, 0
/* 800DFDDC 000DCD1C  41 81 00 34 */	bgt lbl_800DFE10
/* 800DFDE0 000DCD20  38 7E 07 6C */	addi r3, r30, 0x76c
/* 800DFDE4 000DCD24  4C C6 31 82 */	crclr 6
/* 800DFDE8 000DCD28  4B FF D2 BD */	bl SpcTrace__FPCce
/* 800DFDEC 000DCD2C  80 1B 00 1C */	lwz r0, 0x1c(r27)
/* 800DFDF0 000DCD30  80 7B 00 20 */	lwz r3, 0x20(r27)
/* 800DFDF4 000DCD34  54 00 18 38 */	slwi r0, r0, 3
/* 800DFDF8 000DCD38  7C 63 02 14 */	add r3, r3, r0
/* 800DFDFC 000DCD3C  80 03 00 00 */	lwz r0, 0(r3)
/* 800DFE00 000DCD40  90 01 00 68 */	stw r0, 0x68(r1)
/* 800DFE04 000DCD44  80 03 00 04 */	lwz r0, 4(r3)
/* 800DFE08 000DCD48  90 01 00 6C */	stw r0, 0x6c(r1)
/* 800DFE0C 000DCD4C  48 00 00 2C */	b lbl_800DFE38
lbl_800DFE10:
/* 800DFE10 000DCD50  38 03 FF FF */	addi r0, r3, -1
/* 800DFE14 000DCD54  90 1B 00 1C */	stw r0, 0x1c(r27)
/* 800DFE18 000DCD58  80 1B 00 1C */	lwz r0, 0x1c(r27)
/* 800DFE1C 000DCD5C  80 7B 00 20 */	lwz r3, 0x20(r27)
/* 800DFE20 000DCD60  54 00 18 38 */	slwi r0, r0, 3
/* 800DFE24 000DCD64  7C 63 02 14 */	add r3, r3, r0
/* 800DFE28 000DCD68  80 03 00 00 */	lwz r0, 0(r3)
/* 800DFE2C 000DCD6C  90 01 00 68 */	stw r0, 0x68(r1)
/* 800DFE30 000DCD70  80 03 00 04 */	lwz r0, 4(r3)
/* 800DFE34 000DCD74  90 01 00 6C */	stw r0, 0x6c(r1)
lbl_800DFE38:
/* 800DFE38 000DCD78  80 01 00 68 */	lwz r0, 0x68(r1)
/* 800DFE3C 000DCD7C  2C 00 00 01 */	cmpwi r0, 1
/* 800DFE40 000DCD80  41 82 00 1C */	beq lbl_800DFE5C
/* 800DFE44 000DCD84  40 80 00 2C */	bge lbl_800DFE70
/* 800DFE48 000DCD88  2C 00 00 00 */	cmpwi r0, 0
/* 800DFE4C 000DCD8C  40 80 00 08 */	bge lbl_800DFE54
/* 800DFE50 000DCD90  48 00 00 20 */	b lbl_800DFE70
lbl_800DFE54:
/* 800DFE54 000DCD94  83 A1 00 6C */	lwz r29, 0x6c(r1)
/* 800DFE58 000DCD98  48 00 00 1C */	b lbl_800DFE74
lbl_800DFE5C:
/* 800DFE5C 000DCD9C  C0 01 00 6C */	lfs f0, 0x6c(r1)
/* 800DFE60 000DCDA0  FC 00 00 1E */	fctiwz f0, f0
/* 800DFE64 000DCDA4  D8 01 00 88 */	stfd f0, 0x88(r1)
/* 800DFE68 000DCDA8  83 A1 00 8C */	lwz r29, 0x8c(r1)
/* 800DFE6C 000DCDAC  48 00 00 08 */	b lbl_800DFE74
lbl_800DFE70:
/* 800DFE70 000DCDB0  3B A0 00 00 */	li r29, 0
lbl_800DFE74:
/* 800DFE74 000DCDB4  80 7B 00 1C */	lwz r3, 0x1c(r27)
/* 800DFE78 000DCDB8  2C 03 00 00 */	cmpwi r3, 0
/* 800DFE7C 000DCDBC  41 81 00 34 */	bgt lbl_800DFEB0
/* 800DFE80 000DCDC0  38 7E 07 6C */	addi r3, r30, 0x76c
/* 800DFE84 000DCDC4  4C C6 31 82 */	crclr 6
/* 800DFE88 000DCDC8  4B FF D2 1D */	bl SpcTrace__FPCce
/* 800DFE8C 000DCDCC  80 1B 00 1C */	lwz r0, 0x1c(r27)
/* 800DFE90 000DCDD0  80 7B 00 20 */	lwz r3, 0x20(r27)
/* 800DFE94 000DCDD4  54 00 18 38 */	slwi r0, r0, 3
/* 800DFE98 000DCDD8  7C 63 02 14 */	add r3, r3, r0
/* 800DFE9C 000DCDDC  80 03 00 00 */	lwz r0, 0(r3)
/* 800DFEA0 000DCDE0  90 01 00 60 */	stw r0, 0x60(r1)
/* 800DFEA4 000DCDE4  80 03 00 04 */	lwz r0, 4(r3)
/* 800DFEA8 000DCDE8  90 01 00 64 */	stw r0, 0x64(r1)
/* 800DFEAC 000DCDEC  48 00 00 2C */	b lbl_800DFED8
lbl_800DFEB0:
/* 800DFEB0 000DCDF0  38 03 FF FF */	addi r0, r3, -1
/* 800DFEB4 000DCDF4  90 1B 00 1C */	stw r0, 0x1c(r27)
/* 800DFEB8 000DCDF8  80 1B 00 1C */	lwz r0, 0x1c(r27)
/* 800DFEBC 000DCDFC  80 7B 00 20 */	lwz r3, 0x20(r27)
/* 800DFEC0 000DCE00  54 00 18 38 */	slwi r0, r0, 3
/* 800DFEC4 000DCE04  7C 63 02 14 */	add r3, r3, r0
/* 800DFEC8 000DCE08  80 03 00 00 */	lwz r0, 0(r3)
/* 800DFECC 000DCE0C  90 01 00 60 */	stw r0, 0x60(r1)
/* 800DFED0 000DCE10  80 03 00 04 */	lwz r0, 4(r3)
/* 800DFED4 000DCE14  90 01 00 64 */	stw r0, 0x64(r1)
lbl_800DFED8:
/* 800DFED8 000DCE18  80 01 00 60 */	lwz r0, 0x60(r1)
/* 800DFEDC 000DCE1C  2C 00 00 01 */	cmpwi r0, 1
/* 800DFEE0 000DCE20  41 82 00 1C */	beq lbl_800DFEFC
/* 800DFEE4 000DCE24  40 80 00 2C */	bge lbl_800DFF10
/* 800DFEE8 000DCE28  2C 00 00 00 */	cmpwi r0, 0
/* 800DFEEC 000DCE2C  40 80 00 08 */	bge lbl_800DFEF4
/* 800DFEF0 000DCE30  48 00 00 20 */	b lbl_800DFF10
lbl_800DFEF4:
/* 800DFEF4 000DCE34  80 A1 00 64 */	lwz r5, 0x64(r1)
/* 800DFEF8 000DCE38  48 00 00 1C */	b lbl_800DFF14
lbl_800DFEFC:
/* 800DFEFC 000DCE3C  C0 01 00 64 */	lfs f0, 0x64(r1)
/* 800DFF00 000DCE40  FC 00 00 1E */	fctiwz f0, f0
/* 800DFF04 000DCE44  D8 01 00 88 */	stfd f0, 0x88(r1)
/* 800DFF08 000DCE48  80 A1 00 8C */	lwz r5, 0x8c(r1)
/* 800DFF0C 000DCE4C  48 00 00 08 */	b lbl_800DFF14
lbl_800DFF10:
/* 800DFF10 000DCE50  38 A0 00 00 */	li r5, 0
lbl_800DFF14:
/* 800DFF14 000DCE54  80 9E 01 48 */	lwz r4, 0x148(r30)
/* 800DFF18 000DCE58  3C 60 66 66 */	lis r3, 0x66666667@ha
/* 800DFF1C 000DCE5C  80 1E 01 4C */	lwz r0, 0x14c(r30)
/* 800DFF20 000DCE60  3B 85 00 00 */	addi r28, r5, 0
/* 800DFF24 000DCE64  3B E3 66 67 */	addi r31, r3, 0x66666667@l
/* 800DFF28 000DCE68  90 81 00 70 */	stw r4, 0x70(r1)
/* 800DFF2C 000DCE6C  90 01 00 74 */	stw r0, 0x74(r1)
/* 800DFF30 000DCE70  80 1E 01 50 */	lwz r0, 0x150(r30)
/* 800DFF34 000DCE74  90 01 00 78 */	stw r0, 0x78(r1)
/* 800DFF38 000DCE78  88 1E 01 54 */	lbz r0, 0x154(r30)
/* 800DFF3C 000DCE7C  98 01 00 7C */	stb r0, 0x7c(r1)
/* 800DFF40 000DCE80  48 00 00 9C */	b lbl_800DFFDC
lbl_800DFF44:
/* 800DFF44 000DCE84  2C 1C 00 0A */	cmpwi r28, 0xa
/* 800DFF48 000DCE88  40 80 00 18 */	bge lbl_800DFF60
/* 800DFF4C 000DCE8C  38 1C 00 30 */	addi r0, r28, 0x30
/* 800DFF50 000DCE90  98 01 00 7A */	stb r0, 0x7a(r1)
/* 800DFF54 000DCE94  38 00 00 00 */	li r0, 0
/* 800DFF58 000DCE98  98 01 00 7B */	stb r0, 0x7b(r1)
/* 800DFF5C 000DCE9C  48 00 00 38 */	b lbl_800DFF94
lbl_800DFF60:
/* 800DFF60 000DCEA0  7C 1F E0 96 */	mulhw r0, r31, r28
/* 800DFF64 000DCEA4  7C 04 16 70 */	srawi r4, r0, 2
/* 800DFF68 000DCEA8  7C 00 16 70 */	srawi r0, r0, 2
/* 800DFF6C 000DCEAC  54 03 0F FE */	srwi r3, r0, 0x1f
/* 800DFF70 000DCEB0  7C 00 1A 14 */	add r0, r0, r3
/* 800DFF74 000DCEB4  54 83 0F FE */	srwi r3, r4, 0x1f
/* 800DFF78 000DCEB8  1C 00 00 0A */	mulli r0, r0, 0xa
/* 800DFF7C 000DCEBC  7C 64 1A 14 */	add r3, r4, r3
/* 800DFF80 000DCEC0  38 83 00 30 */	addi r4, r3, 0x30
/* 800DFF84 000DCEC4  7C 60 E0 50 */	subf r3, r0, r28
/* 800DFF88 000DCEC8  98 81 00 7A */	stb r4, 0x7a(r1)
/* 800DFF8C 000DCECC  38 03 00 30 */	addi r0, r3, 0x30
/* 800DFF90 000DCED0  98 01 00 7B */	stb r0, 0x7b(r1)
lbl_800DFF94:
/* 800DFF94 000DCED4  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 800DFF98 000DCED8  38 61 00 70 */	addi r3, r1, 0x70
/* 800DFF9C 000DCEDC  83 44 00 04 */	lwz r26, 4(r4)
/* 800DFFA0 000DCEE0  4B F6 46 ED */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800DFFA4 000DCEE4  81 9A 00 00 */	lwz r12, 0(r26)
/* 800DFFA8 000DCEE8  38 83 00 00 */	addi r4, r3, 0
/* 800DFFAC 000DCEEC  38 7A 00 00 */	addi r3, r26, 0
/* 800DFFB0 000DCEF0  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 800DFFB4 000DCEF4  38 A1 00 70 */	addi r5, r1, 0x70
/* 800DFFB8 000DCEF8  7D 88 03 A6 */	mtlr r12
/* 800DFFBC 000DCEFC  4E 80 00 21 */	blrl 
/* 800DFFC0 000DCF00  28 03 00 00 */	cmplwi r3, 0
/* 800DFFC4 000DCF04  41 82 00 14 */	beq lbl_800DFFD8
/* 800DFFC8 000DCF08  81 83 00 00 */	lwz r12, 0(r3)
/* 800DFFCC 000DCF0C  81 8C 01 04 */	lwz r12, 0x104(r12)
/* 800DFFD0 000DCF10  7D 88 03 A6 */	mtlr r12
/* 800DFFD4 000DCF14  4E 80 00 21 */	blrl 
lbl_800DFFD8:
/* 800DFFD8 000DCF18  3B 9C 00 01 */	addi r28, r28, 1
lbl_800DFFDC:
/* 800DFFDC 000DCF1C  7C 1C E8 00 */	cmpw r28, r29
/* 800DFFE0 000DCF20  40 81 FF 64 */	ble lbl_800DFF44
/* 800DFFE4 000DCF24  38 00 00 00 */	li r0, 0
/* 800DFFE8 000DCF28  90 01 00 50 */	stw r0, 0x50(r1)
/* 800DFFEC 000DCF2C  90 01 00 54 */	stw r0, 0x54(r1)
/* 800DFFF0 000DCF30  80 7B 00 1C */	lwz r3, 0x1c(r27)
/* 800DFFF4 000DCF34  80 1B 00 18 */	lwz r0, 0x18(r27)
/* 800DFFF8 000DCF38  7C 03 00 00 */	cmpw r3, r0
/* 800DFFFC 000DCF3C  41 80 00 14 */	blt lbl_800E0010
/* 800E0000 000DCF40  38 7E 07 8C */	addi r3, r30, 0x78c
/* 800E0004 000DCF44  4C C6 31 82 */	crclr 6
/* 800E0008 000DCF48  4B FF D0 9D */	bl SpcTrace__FPCce
/* 800E000C 000DCF4C  48 00 00 2C */	b lbl_800E0038
lbl_800E0010:
/* 800E0010 000DCF50  80 BB 00 20 */	lwz r5, 0x20(r27)
/* 800E0014 000DCF54  54 64 18 38 */	slwi r4, r3, 3
/* 800E0018 000DCF58  80 61 00 50 */	lwz r3, 0x50(r1)
/* 800E001C 000DCF5C  80 01 00 54 */	lwz r0, 0x54(r1)
/* 800E0020 000DCF60  7C 85 22 14 */	add r4, r5, r4
/* 800E0024 000DCF64  90 64 00 00 */	stw r3, 0(r4)
/* 800E0028 000DCF68  90 04 00 04 */	stw r0, 4(r4)
/* 800E002C 000DCF6C  80 7B 00 1C */	lwz r3, 0x1c(r27)
/* 800E0030 000DCF70  38 03 00 01 */	addi r0, r3, 1
/* 800E0034 000DCF74  90 1B 00 1C */	stw r0, 0x1c(r27)
lbl_800E0038:
/* 800E0038 000DCF78  BB 41 00 90 */	lmw r26, 0x90(r1)
/* 800E003C 000DCF7C  80 01 00 AC */	lwz r0, 0xac(r1)
/* 800E0040 000DCF80  38 21 00 A8 */	addi r1, r1, 0xa8
/* 800E0044 000DCF84  7C 08 03 A6 */	mtlr r0
/* 800E0048 000DCF88  4E 80 00 20 */	blr 

.global evRefreshWoodBox__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evRefreshWoodBox__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E004C 000DCF8C  7C 08 02 A6 */	mflr r0
/* 800E0050 000DCF90  90 01 00 04 */	stw r0, 4(r1)
/* 800E0054 000DCF94  94 21 FF 58 */	stwu r1, -0xa8(r1)
/* 800E0058 000DCF98  BF 41 00 90 */	stmw r26, 0x90(r1)
/* 800E005C 000DCF9C  7C 7B 1B 78 */	mr r27, r3
/* 800E0060 000DCFA0  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E0064 000DCFA4  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E0068 000DCFA8  3C 80 80 37 */	lis r4, $$21490@ha
/* 800E006C 000DCFAC  3B C4 29 90 */	addi r30, r4, $$21490@l
/* 800E0070 000DCFB0  38 80 00 02 */	li r4, 2
/* 800E0074 000DCFB4  48 03 73 BD */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E0078 000DCFB8  80 7B 00 1C */	lwz r3, 0x1c(r27)
/* 800E007C 000DCFBC  2C 03 00 00 */	cmpwi r3, 0
/* 800E0080 000DCFC0  41 81 00 34 */	bgt lbl_800E00B4
/* 800E0084 000DCFC4  38 7E 07 6C */	addi r3, r30, 0x76c
/* 800E0088 000DCFC8  4C C6 31 82 */	crclr 6
/* 800E008C 000DCFCC  4B FF D0 19 */	bl SpcTrace__FPCce
/* 800E0090 000DCFD0  80 1B 00 1C */	lwz r0, 0x1c(r27)
/* 800E0094 000DCFD4  80 7B 00 20 */	lwz r3, 0x20(r27)
/* 800E0098 000DCFD8  54 00 18 38 */	slwi r0, r0, 3
/* 800E009C 000DCFDC  7C 63 02 14 */	add r3, r3, r0
/* 800E00A0 000DCFE0  80 03 00 00 */	lwz r0, 0(r3)
/* 800E00A4 000DCFE4  90 01 00 68 */	stw r0, 0x68(r1)
/* 800E00A8 000DCFE8  80 03 00 04 */	lwz r0, 4(r3)
/* 800E00AC 000DCFEC  90 01 00 6C */	stw r0, 0x6c(r1)
/* 800E00B0 000DCFF0  48 00 00 2C */	b lbl_800E00DC
lbl_800E00B4:
/* 800E00B4 000DCFF4  38 03 FF FF */	addi r0, r3, -1
/* 800E00B8 000DCFF8  90 1B 00 1C */	stw r0, 0x1c(r27)
/* 800E00BC 000DCFFC  80 1B 00 1C */	lwz r0, 0x1c(r27)
/* 800E00C0 000DD000  80 7B 00 20 */	lwz r3, 0x20(r27)
/* 800E00C4 000DD004  54 00 18 38 */	slwi r0, r0, 3
/* 800E00C8 000DD008  7C 63 02 14 */	add r3, r3, r0
/* 800E00CC 000DD00C  80 03 00 00 */	lwz r0, 0(r3)
/* 800E00D0 000DD010  90 01 00 68 */	stw r0, 0x68(r1)
/* 800E00D4 000DD014  80 03 00 04 */	lwz r0, 4(r3)
/* 800E00D8 000DD018  90 01 00 6C */	stw r0, 0x6c(r1)
lbl_800E00DC:
/* 800E00DC 000DD01C  80 01 00 68 */	lwz r0, 0x68(r1)
/* 800E00E0 000DD020  2C 00 00 01 */	cmpwi r0, 1
/* 800E00E4 000DD024  41 82 00 1C */	beq lbl_800E0100
/* 800E00E8 000DD028  40 80 00 2C */	bge lbl_800E0114
/* 800E00EC 000DD02C  2C 00 00 00 */	cmpwi r0, 0
/* 800E00F0 000DD030  40 80 00 08 */	bge lbl_800E00F8
/* 800E00F4 000DD034  48 00 00 20 */	b lbl_800E0114
lbl_800E00F8:
/* 800E00F8 000DD038  83 A1 00 6C */	lwz r29, 0x6c(r1)
/* 800E00FC 000DD03C  48 00 00 1C */	b lbl_800E0118
lbl_800E0100:
/* 800E0100 000DD040  C0 01 00 6C */	lfs f0, 0x6c(r1)
/* 800E0104 000DD044  FC 00 00 1E */	fctiwz f0, f0
/* 800E0108 000DD048  D8 01 00 88 */	stfd f0, 0x88(r1)
/* 800E010C 000DD04C  83 A1 00 8C */	lwz r29, 0x8c(r1)
/* 800E0110 000DD050  48 00 00 08 */	b lbl_800E0118
lbl_800E0114:
/* 800E0114 000DD054  3B A0 00 00 */	li r29, 0
lbl_800E0118:
/* 800E0118 000DD058  80 7B 00 1C */	lwz r3, 0x1c(r27)
/* 800E011C 000DD05C  2C 03 00 00 */	cmpwi r3, 0
/* 800E0120 000DD060  41 81 00 34 */	bgt lbl_800E0154
/* 800E0124 000DD064  38 7E 07 6C */	addi r3, r30, 0x76c
/* 800E0128 000DD068  4C C6 31 82 */	crclr 6
/* 800E012C 000DD06C  4B FF CF 79 */	bl SpcTrace__FPCce
/* 800E0130 000DD070  80 1B 00 1C */	lwz r0, 0x1c(r27)
/* 800E0134 000DD074  80 7B 00 20 */	lwz r3, 0x20(r27)
/* 800E0138 000DD078  54 00 18 38 */	slwi r0, r0, 3
/* 800E013C 000DD07C  7C 63 02 14 */	add r3, r3, r0
/* 800E0140 000DD080  80 03 00 00 */	lwz r0, 0(r3)
/* 800E0144 000DD084  90 01 00 60 */	stw r0, 0x60(r1)
/* 800E0148 000DD088  80 03 00 04 */	lwz r0, 4(r3)
/* 800E014C 000DD08C  90 01 00 64 */	stw r0, 0x64(r1)
/* 800E0150 000DD090  48 00 00 2C */	b lbl_800E017C
lbl_800E0154:
/* 800E0154 000DD094  38 03 FF FF */	addi r0, r3, -1
/* 800E0158 000DD098  90 1B 00 1C */	stw r0, 0x1c(r27)
/* 800E015C 000DD09C  80 1B 00 1C */	lwz r0, 0x1c(r27)
/* 800E0160 000DD0A0  80 7B 00 20 */	lwz r3, 0x20(r27)
/* 800E0164 000DD0A4  54 00 18 38 */	slwi r0, r0, 3
/* 800E0168 000DD0A8  7C 63 02 14 */	add r3, r3, r0
/* 800E016C 000DD0AC  80 03 00 00 */	lwz r0, 0(r3)
/* 800E0170 000DD0B0  90 01 00 60 */	stw r0, 0x60(r1)
/* 800E0174 000DD0B4  80 03 00 04 */	lwz r0, 4(r3)
/* 800E0178 000DD0B8  90 01 00 64 */	stw r0, 0x64(r1)
lbl_800E017C:
/* 800E017C 000DD0BC  80 01 00 60 */	lwz r0, 0x60(r1)
/* 800E0180 000DD0C0  2C 00 00 01 */	cmpwi r0, 1
/* 800E0184 000DD0C4  41 82 00 1C */	beq lbl_800E01A0
/* 800E0188 000DD0C8  40 80 00 2C */	bge lbl_800E01B4
/* 800E018C 000DD0CC  2C 00 00 00 */	cmpwi r0, 0
/* 800E0190 000DD0D0  40 80 00 08 */	bge lbl_800E0198
/* 800E0194 000DD0D4  48 00 00 20 */	b lbl_800E01B4
lbl_800E0198:
/* 800E0198 000DD0D8  80 A1 00 64 */	lwz r5, 0x64(r1)
/* 800E019C 000DD0DC  48 00 00 1C */	b lbl_800E01B8
lbl_800E01A0:
/* 800E01A0 000DD0E0  C0 01 00 64 */	lfs f0, 0x64(r1)
/* 800E01A4 000DD0E4  FC 00 00 1E */	fctiwz f0, f0
/* 800E01A8 000DD0E8  D8 01 00 88 */	stfd f0, 0x88(r1)
/* 800E01AC 000DD0EC  80 A1 00 8C */	lwz r5, 0x8c(r1)
/* 800E01B0 000DD0F0  48 00 00 08 */	b lbl_800E01B8
lbl_800E01B4:
/* 800E01B4 000DD0F4  38 A0 00 00 */	li r5, 0
lbl_800E01B8:
/* 800E01B8 000DD0F8  80 9E 01 38 */	lwz r4, 0x138(r30)
/* 800E01BC 000DD0FC  3C 60 66 66 */	lis r3, 0x66666667@ha
/* 800E01C0 000DD100  80 1E 01 3C */	lwz r0, 0x13c(r30)
/* 800E01C4 000DD104  3B 85 00 00 */	addi r28, r5, 0
/* 800E01C8 000DD108  3B E3 66 67 */	addi r31, r3, 0x66666667@l
/* 800E01CC 000DD10C  90 81 00 70 */	stw r4, 0x70(r1)
/* 800E01D0 000DD110  90 01 00 74 */	stw r0, 0x74(r1)
/* 800E01D4 000DD114  80 1E 01 40 */	lwz r0, 0x140(r30)
/* 800E01D8 000DD118  90 01 00 78 */	stw r0, 0x78(r1)
/* 800E01DC 000DD11C  88 1E 01 44 */	lbz r0, 0x144(r30)
/* 800E01E0 000DD120  98 01 00 7C */	stb r0, 0x7c(r1)
/* 800E01E4 000DD124  48 00 00 9C */	b lbl_800E0280
lbl_800E01E8:
/* 800E01E8 000DD128  2C 1C 00 0A */	cmpwi r28, 0xa
/* 800E01EC 000DD12C  40 80 00 18 */	bge lbl_800E0204
/* 800E01F0 000DD130  38 1C 00 30 */	addi r0, r28, 0x30
/* 800E01F4 000DD134  98 01 00 7A */	stb r0, 0x7a(r1)
/* 800E01F8 000DD138  38 00 00 00 */	li r0, 0
/* 800E01FC 000DD13C  98 01 00 7B */	stb r0, 0x7b(r1)
/* 800E0200 000DD140  48 00 00 38 */	b lbl_800E0238
lbl_800E0204:
/* 800E0204 000DD144  7C 1F E0 96 */	mulhw r0, r31, r28
/* 800E0208 000DD148  7C 04 16 70 */	srawi r4, r0, 2
/* 800E020C 000DD14C  7C 00 16 70 */	srawi r0, r0, 2
/* 800E0210 000DD150  54 03 0F FE */	srwi r3, r0, 0x1f
/* 800E0214 000DD154  7C 00 1A 14 */	add r0, r0, r3
/* 800E0218 000DD158  54 83 0F FE */	srwi r3, r4, 0x1f
/* 800E021C 000DD15C  1C 00 00 0A */	mulli r0, r0, 0xa
/* 800E0220 000DD160  7C 64 1A 14 */	add r3, r4, r3
/* 800E0224 000DD164  38 83 00 30 */	addi r4, r3, 0x30
/* 800E0228 000DD168  7C 60 E0 50 */	subf r3, r0, r28
/* 800E022C 000DD16C  98 81 00 7A */	stb r4, 0x7a(r1)
/* 800E0230 000DD170  38 03 00 30 */	addi r0, r3, 0x30
/* 800E0234 000DD174  98 01 00 7B */	stb r0, 0x7b(r1)
lbl_800E0238:
/* 800E0238 000DD178  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 800E023C 000DD17C  38 61 00 70 */	addi r3, r1, 0x70
/* 800E0240 000DD180  83 44 00 04 */	lwz r26, 4(r4)
/* 800E0244 000DD184  4B F6 44 49 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800E0248 000DD188  81 9A 00 00 */	lwz r12, 0(r26)
/* 800E024C 000DD18C  38 83 00 00 */	addi r4, r3, 0
/* 800E0250 000DD190  38 7A 00 00 */	addi r3, r26, 0
/* 800E0254 000DD194  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 800E0258 000DD198  38 A1 00 70 */	addi r5, r1, 0x70
/* 800E025C 000DD19C  7D 88 03 A6 */	mtlr r12
/* 800E0260 000DD1A0  4E 80 00 21 */	blrl 
/* 800E0264 000DD1A4  28 03 00 00 */	cmplwi r3, 0
/* 800E0268 000DD1A8  41 82 00 14 */	beq lbl_800E027C
/* 800E026C 000DD1AC  81 83 00 00 */	lwz r12, 0(r3)
/* 800E0270 000DD1B0  81 8C 00 FC */	lwz r12, 0xfc(r12)
/* 800E0274 000DD1B4  7D 88 03 A6 */	mtlr r12
/* 800E0278 000DD1B8  4E 80 00 21 */	blrl 
lbl_800E027C:
/* 800E027C 000DD1BC  3B 9C 00 01 */	addi r28, r28, 1
lbl_800E0280:
/* 800E0280 000DD1C0  7C 1C E8 00 */	cmpw r28, r29
/* 800E0284 000DD1C4  40 81 FF 64 */	ble lbl_800E01E8
/* 800E0288 000DD1C8  38 00 00 00 */	li r0, 0
/* 800E028C 000DD1CC  90 01 00 50 */	stw r0, 0x50(r1)
/* 800E0290 000DD1D0  90 01 00 54 */	stw r0, 0x54(r1)
/* 800E0294 000DD1D4  80 7B 00 1C */	lwz r3, 0x1c(r27)
/* 800E0298 000DD1D8  80 1B 00 18 */	lwz r0, 0x18(r27)
/* 800E029C 000DD1DC  7C 03 00 00 */	cmpw r3, r0
/* 800E02A0 000DD1E0  41 80 00 14 */	blt lbl_800E02B4
/* 800E02A4 000DD1E4  38 7E 07 8C */	addi r3, r30, 0x78c
/* 800E02A8 000DD1E8  4C C6 31 82 */	crclr 6
/* 800E02AC 000DD1EC  4B FF CD F9 */	bl SpcTrace__FPCce
/* 800E02B0 000DD1F0  48 00 00 2C */	b lbl_800E02DC
lbl_800E02B4:
/* 800E02B4 000DD1F4  80 BB 00 20 */	lwz r5, 0x20(r27)
/* 800E02B8 000DD1F8  54 64 18 38 */	slwi r4, r3, 3
/* 800E02BC 000DD1FC  80 61 00 50 */	lwz r3, 0x50(r1)
/* 800E02C0 000DD200  80 01 00 54 */	lwz r0, 0x54(r1)
/* 800E02C4 000DD204  7C 85 22 14 */	add r4, r5, r4
/* 800E02C8 000DD208  90 64 00 00 */	stw r3, 0(r4)
/* 800E02CC 000DD20C  90 04 00 04 */	stw r0, 4(r4)
/* 800E02D0 000DD210  80 7B 00 1C */	lwz r3, 0x1c(r27)
/* 800E02D4 000DD214  38 03 00 01 */	addi r0, r3, 1
/* 800E02D8 000DD218  90 1B 00 1C */	stw r0, 0x1c(r27)
lbl_800E02DC:
/* 800E02DC 000DD21C  BB 41 00 90 */	lmw r26, 0x90(r1)
/* 800E02E0 000DD220  80 01 00 AC */	lwz r0, 0xac(r1)
/* 800E02E4 000DD224  38 21 00 A8 */	addi r1, r1, 0xa8
/* 800E02E8 000DD228  7C 08 03 A6 */	mtlr r0
/* 800E02EC 000DD22C  4E 80 00 20 */	blr 

.global evCheckWoodBox__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evCheckWoodBox__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E02F0 000DD230  7C 08 02 A6 */	mflr r0
/* 800E02F4 000DD234  90 01 00 04 */	stw r0, 4(r1)
/* 800E02F8 000DD238  94 21 FF 50 */	stwu r1, -0xb0(r1)
/* 800E02FC 000DD23C  BF 21 00 94 */	stmw r25, 0x94(r1)
/* 800E0300 000DD240  7C 7A 1B 78 */	mr r26, r3
/* 800E0304 000DD244  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E0308 000DD248  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E030C 000DD24C  3C 80 80 37 */	lis r4, $$21490@ha
/* 800E0310 000DD250  3B C4 29 90 */	addi r30, r4, $$21490@l
/* 800E0314 000DD254  38 80 00 02 */	li r4, 2
/* 800E0318 000DD258  48 03 71 19 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E031C 000DD25C  80 7A 00 1C */	lwz r3, 0x1c(r26)
/* 800E0320 000DD260  2C 03 00 00 */	cmpwi r3, 0
/* 800E0324 000DD264  41 81 00 34 */	bgt lbl_800E0358
/* 800E0328 000DD268  38 7E 07 6C */	addi r3, r30, 0x76c
/* 800E032C 000DD26C  4C C6 31 82 */	crclr 6
/* 800E0330 000DD270  4B FF CD 75 */	bl SpcTrace__FPCce
/* 800E0334 000DD274  80 1A 00 1C */	lwz r0, 0x1c(r26)
/* 800E0338 000DD278  80 7A 00 20 */	lwz r3, 0x20(r26)
/* 800E033C 000DD27C  54 00 18 38 */	slwi r0, r0, 3
/* 800E0340 000DD280  7C 63 02 14 */	add r3, r3, r0
/* 800E0344 000DD284  80 03 00 00 */	lwz r0, 0(r3)
/* 800E0348 000DD288  90 01 00 68 */	stw r0, 0x68(r1)
/* 800E034C 000DD28C  80 03 00 04 */	lwz r0, 4(r3)
/* 800E0350 000DD290  90 01 00 6C */	stw r0, 0x6c(r1)
/* 800E0354 000DD294  48 00 00 2C */	b lbl_800E0380
lbl_800E0358:
/* 800E0358 000DD298  38 03 FF FF */	addi r0, r3, -1
/* 800E035C 000DD29C  90 1A 00 1C */	stw r0, 0x1c(r26)
/* 800E0360 000DD2A0  80 1A 00 1C */	lwz r0, 0x1c(r26)
/* 800E0364 000DD2A4  80 7A 00 20 */	lwz r3, 0x20(r26)
/* 800E0368 000DD2A8  54 00 18 38 */	slwi r0, r0, 3
/* 800E036C 000DD2AC  7C 63 02 14 */	add r3, r3, r0
/* 800E0370 000DD2B0  80 03 00 00 */	lwz r0, 0(r3)
/* 800E0374 000DD2B4  90 01 00 68 */	stw r0, 0x68(r1)
/* 800E0378 000DD2B8  80 03 00 04 */	lwz r0, 4(r3)
/* 800E037C 000DD2BC  90 01 00 6C */	stw r0, 0x6c(r1)
lbl_800E0380:
/* 800E0380 000DD2C0  80 01 00 68 */	lwz r0, 0x68(r1)
/* 800E0384 000DD2C4  2C 00 00 01 */	cmpwi r0, 1
/* 800E0388 000DD2C8  41 82 00 1C */	beq lbl_800E03A4
/* 800E038C 000DD2CC  40 80 00 2C */	bge lbl_800E03B8
/* 800E0390 000DD2D0  2C 00 00 00 */	cmpwi r0, 0
/* 800E0394 000DD2D4  40 80 00 08 */	bge lbl_800E039C
/* 800E0398 000DD2D8  48 00 00 20 */	b lbl_800E03B8
lbl_800E039C:
/* 800E039C 000DD2DC  83 A1 00 6C */	lwz r29, 0x6c(r1)
/* 800E03A0 000DD2E0  48 00 00 1C */	b lbl_800E03BC
lbl_800E03A4:
/* 800E03A4 000DD2E4  C0 01 00 6C */	lfs f0, 0x6c(r1)
/* 800E03A8 000DD2E8  FC 00 00 1E */	fctiwz f0, f0
/* 800E03AC 000DD2EC  D8 01 00 88 */	stfd f0, 0x88(r1)
/* 800E03B0 000DD2F0  83 A1 00 8C */	lwz r29, 0x8c(r1)
/* 800E03B4 000DD2F4  48 00 00 08 */	b lbl_800E03BC
lbl_800E03B8:
/* 800E03B8 000DD2F8  3B A0 00 00 */	li r29, 0
lbl_800E03BC:
/* 800E03BC 000DD2FC  80 7A 00 1C */	lwz r3, 0x1c(r26)
/* 800E03C0 000DD300  2C 03 00 00 */	cmpwi r3, 0
/* 800E03C4 000DD304  41 81 00 34 */	bgt lbl_800E03F8
/* 800E03C8 000DD308  38 7E 07 6C */	addi r3, r30, 0x76c
/* 800E03CC 000DD30C  4C C6 31 82 */	crclr 6
/* 800E03D0 000DD310  4B FF CC D5 */	bl SpcTrace__FPCce
/* 800E03D4 000DD314  80 1A 00 1C */	lwz r0, 0x1c(r26)
/* 800E03D8 000DD318  80 7A 00 20 */	lwz r3, 0x20(r26)
/* 800E03DC 000DD31C  54 00 18 38 */	slwi r0, r0, 3
/* 800E03E0 000DD320  7C 63 02 14 */	add r3, r3, r0
/* 800E03E4 000DD324  80 03 00 00 */	lwz r0, 0(r3)
/* 800E03E8 000DD328  90 01 00 60 */	stw r0, 0x60(r1)
/* 800E03EC 000DD32C  80 03 00 04 */	lwz r0, 4(r3)
/* 800E03F0 000DD330  90 01 00 64 */	stw r0, 0x64(r1)
/* 800E03F4 000DD334  48 00 00 2C */	b lbl_800E0420
lbl_800E03F8:
/* 800E03F8 000DD338  38 03 FF FF */	addi r0, r3, -1
/* 800E03FC 000DD33C  90 1A 00 1C */	stw r0, 0x1c(r26)
/* 800E0400 000DD340  80 1A 00 1C */	lwz r0, 0x1c(r26)
/* 800E0404 000DD344  80 7A 00 20 */	lwz r3, 0x20(r26)
/* 800E0408 000DD348  54 00 18 38 */	slwi r0, r0, 3
/* 800E040C 000DD34C  7C 63 02 14 */	add r3, r3, r0
/* 800E0410 000DD350  80 03 00 00 */	lwz r0, 0(r3)
/* 800E0414 000DD354  90 01 00 60 */	stw r0, 0x60(r1)
/* 800E0418 000DD358  80 03 00 04 */	lwz r0, 4(r3)
/* 800E041C 000DD35C  90 01 00 64 */	stw r0, 0x64(r1)
lbl_800E0420:
/* 800E0420 000DD360  80 01 00 60 */	lwz r0, 0x60(r1)
/* 800E0424 000DD364  2C 00 00 01 */	cmpwi r0, 1
/* 800E0428 000DD368  41 82 00 1C */	beq lbl_800E0444
/* 800E042C 000DD36C  40 80 00 2C */	bge lbl_800E0458
/* 800E0430 000DD370  2C 00 00 00 */	cmpwi r0, 0
/* 800E0434 000DD374  40 80 00 08 */	bge lbl_800E043C
/* 800E0438 000DD378  48 00 00 20 */	b lbl_800E0458
lbl_800E043C:
/* 800E043C 000DD37C  80 C1 00 64 */	lwz r6, 0x64(r1)
/* 800E0440 000DD380  48 00 00 1C */	b lbl_800E045C
lbl_800E0444:
/* 800E0444 000DD384  C0 01 00 64 */	lfs f0, 0x64(r1)
/* 800E0448 000DD388  FC 00 00 1E */	fctiwz f0, f0
/* 800E044C 000DD38C  D8 01 00 88 */	stfd f0, 0x88(r1)
/* 800E0450 000DD390  80 C1 00 8C */	lwz r6, 0x8c(r1)
/* 800E0454 000DD394  48 00 00 08 */	b lbl_800E045C
lbl_800E0458:
/* 800E0458 000DD398  38 C0 00 00 */	li r6, 0
lbl_800E045C:
/* 800E045C 000DD39C  80 9E 01 28 */	lwz r4, 0x128(r30)
/* 800E0460 000DD3A0  7C A6 E8 50 */	subf r5, r6, r29
/* 800E0464 000DD3A4  80 1E 01 2C */	lwz r0, 0x12c(r30)
/* 800E0468 000DD3A8  3C 60 66 66 */	lis r3, 0x66666667@ha
/* 800E046C 000DD3AC  3B 66 00 00 */	addi r27, r6, 0
/* 800E0470 000DD3B0  90 81 00 70 */	stw r4, 0x70(r1)
/* 800E0474 000DD3B4  3B 85 00 01 */	addi r28, r5, 1
/* 800E0478 000DD3B8  3B E3 66 67 */	addi r31, r3, 0x66666667@l
/* 800E047C 000DD3BC  90 01 00 74 */	stw r0, 0x74(r1)
/* 800E0480 000DD3C0  80 1E 01 30 */	lwz r0, 0x130(r30)
/* 800E0484 000DD3C4  90 01 00 78 */	stw r0, 0x78(r1)
/* 800E0488 000DD3C8  88 1E 01 34 */	lbz r0, 0x134(r30)
/* 800E048C 000DD3CC  98 01 00 7C */	stb r0, 0x7c(r1)
/* 800E0490 000DD3D0  48 00 00 9C */	b lbl_800E052C
lbl_800E0494:
/* 800E0494 000DD3D4  2C 1B 00 0A */	cmpwi r27, 0xa
/* 800E0498 000DD3D8  40 80 00 18 */	bge lbl_800E04B0
/* 800E049C 000DD3DC  38 1B 00 30 */	addi r0, r27, 0x30
/* 800E04A0 000DD3E0  98 01 00 7A */	stb r0, 0x7a(r1)
/* 800E04A4 000DD3E4  38 00 00 00 */	li r0, 0
/* 800E04A8 000DD3E8  98 01 00 7B */	stb r0, 0x7b(r1)
/* 800E04AC 000DD3EC  48 00 00 38 */	b lbl_800E04E4
lbl_800E04B0:
/* 800E04B0 000DD3F0  7C 1F D8 96 */	mulhw r0, r31, r27
/* 800E04B4 000DD3F4  7C 04 16 70 */	srawi r4, r0, 2
/* 800E04B8 000DD3F8  7C 00 16 70 */	srawi r0, r0, 2
/* 800E04BC 000DD3FC  54 03 0F FE */	srwi r3, r0, 0x1f
/* 800E04C0 000DD400  7C 00 1A 14 */	add r0, r0, r3
/* 800E04C4 000DD404  54 83 0F FE */	srwi r3, r4, 0x1f
/* 800E04C8 000DD408  1C 00 00 0A */	mulli r0, r0, 0xa
/* 800E04CC 000DD40C  7C 64 1A 14 */	add r3, r4, r3
/* 800E04D0 000DD410  38 83 00 30 */	addi r4, r3, 0x30
/* 800E04D4 000DD414  7C 60 D8 50 */	subf r3, r0, r27
/* 800E04D8 000DD418  98 81 00 7A */	stb r4, 0x7a(r1)
/* 800E04DC 000DD41C  38 03 00 30 */	addi r0, r3, 0x30
/* 800E04E0 000DD420  98 01 00 7B */	stb r0, 0x7b(r1)
lbl_800E04E4:
/* 800E04E4 000DD424  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 800E04E8 000DD428  38 61 00 70 */	addi r3, r1, 0x70
/* 800E04EC 000DD42C  83 24 00 04 */	lwz r25, 4(r4)
/* 800E04F0 000DD430  4B F6 41 9D */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800E04F4 000DD434  81 99 00 00 */	lwz r12, 0(r25)
/* 800E04F8 000DD438  38 83 00 00 */	addi r4, r3, 0
/* 800E04FC 000DD43C  38 79 00 00 */	addi r3, r25, 0
/* 800E0500 000DD440  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 800E0504 000DD444  38 A1 00 70 */	addi r5, r1, 0x70
/* 800E0508 000DD448  7D 88 03 A6 */	mtlr r12
/* 800E050C 000DD44C  4E 80 00 21 */	blrl 
/* 800E0510 000DD450  28 03 00 00 */	cmplwi r3, 0
/* 800E0514 000DD454  41 82 00 14 */	beq lbl_800E0528
/* 800E0518 000DD458  80 03 00 F0 */	lwz r0, 0xf0(r3)
/* 800E051C 000DD45C  54 00 07 FF */	clrlwi. r0, r0, 0x1f
/* 800E0520 000DD460  41 82 00 08 */	beq lbl_800E0528
/* 800E0524 000DD464  3B 9C FF FF */	addi r28, r28, -1
lbl_800E0528:
/* 800E0528 000DD468  3B 7B 00 01 */	addi r27, r27, 1
lbl_800E052C:
/* 800E052C 000DD46C  7C 1B E8 00 */	cmpw r27, r29
/* 800E0530 000DD470  40 81 FF 64 */	ble lbl_800E0494
/* 800E0534 000DD474  38 00 00 00 */	li r0, 0
/* 800E0538 000DD478  90 01 00 50 */	stw r0, 0x50(r1)
/* 800E053C 000DD47C  93 81 00 54 */	stw r28, 0x54(r1)
/* 800E0540 000DD480  80 7A 00 1C */	lwz r3, 0x1c(r26)
/* 800E0544 000DD484  80 1A 00 18 */	lwz r0, 0x18(r26)
/* 800E0548 000DD488  7C 03 00 00 */	cmpw r3, r0
/* 800E054C 000DD48C  41 80 00 14 */	blt lbl_800E0560
/* 800E0550 000DD490  38 7E 07 8C */	addi r3, r30, 0x78c
/* 800E0554 000DD494  4C C6 31 82 */	crclr 6
/* 800E0558 000DD498  4B FF CB 4D */	bl SpcTrace__FPCce
/* 800E055C 000DD49C  48 00 00 2C */	b lbl_800E0588
lbl_800E0560:
/* 800E0560 000DD4A0  80 BA 00 20 */	lwz r5, 0x20(r26)
/* 800E0564 000DD4A4  54 64 18 38 */	slwi r4, r3, 3
/* 800E0568 000DD4A8  80 61 00 50 */	lwz r3, 0x50(r1)
/* 800E056C 000DD4AC  80 01 00 54 */	lwz r0, 0x54(r1)
/* 800E0570 000DD4B0  7C 85 22 14 */	add r4, r5, r4
/* 800E0574 000DD4B4  90 64 00 00 */	stw r3, 0(r4)
/* 800E0578 000DD4B8  90 04 00 04 */	stw r0, 4(r4)
/* 800E057C 000DD4BC  80 7A 00 1C */	lwz r3, 0x1c(r26)
/* 800E0580 000DD4C0  38 03 00 01 */	addi r0, r3, 1
/* 800E0584 000DD4C4  90 1A 00 1C */	stw r0, 0x1c(r26)
lbl_800E0588:
/* 800E0588 000DD4C8  BB 21 00 94 */	lmw r25, 0x94(r1)
/* 800E058C 000DD4CC  80 01 00 B4 */	lwz r0, 0xb4(r1)
/* 800E0590 000DD4D0  38 21 00 B0 */	addi r1, r1, 0xb0
/* 800E0594 000DD4D4  7C 08 03 A6 */	mtlr r0
/* 800E0598 000DD4D8  4E 80 00 20 */	blr 

.global evStartMarioTalking__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evStartMarioTalking__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E059C 000DD4DC  7C 08 02 A6 */	mflr r0
/* 800E05A0 000DD4E0  90 01 00 04 */	stw r0, 4(r1)
/* 800E05A4 000DD4E4  94 21 FF E0 */	stwu r1, -0x20(r1)
/* 800E05A8 000DD4E8  93 E1 00 1C */	stw r31, 0x1c(r1)
/* 800E05AC 000DD4EC  7C 7F 1B 78 */	mr r31, r3
/* 800E05B0 000DD4F0  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E05B4 000DD4F4  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E05B8 000DD4F8  38 80 00 00 */	li r4, 0
/* 800E05BC 000DD4FC  48 03 6E 75 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E05C0 000DD500  80 6D 98 B8 */	lwz r3, gpMarioOriginal@sda21(r13)
/* 800E05C4 000DD504  48 06 57 81 */	bl startTalking__6TMarioFv
/* 800E05C8 000DD508  38 00 00 00 */	li r0, 0
/* 800E05CC 000DD50C  90 01 00 10 */	stw r0, 0x10(r1)
/* 800E05D0 000DD510  90 01 00 14 */	stw r0, 0x14(r1)
/* 800E05D4 000DD514  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E05D8 000DD518  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800E05DC 000DD51C  7C 03 00 00 */	cmpw r3, r0
/* 800E05E0 000DD520  41 80 00 18 */	blt lbl_800E05F8
/* 800E05E4 000DD524  3C 60 80 37 */	lis r3, $$23834@ha
/* 800E05E8 000DD528  4C C6 31 82 */	crclr 6
/* 800E05EC 000DD52C  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800E05F0 000DD530  4B FF CA B5 */	bl SpcTrace__FPCce
/* 800E05F4 000DD534  48 00 00 2C */	b lbl_800E0620
lbl_800E05F8:
/* 800E05F8 000DD538  80 BF 00 20 */	lwz r5, 0x20(r31)
/* 800E05FC 000DD53C  54 64 18 38 */	slwi r4, r3, 3
/* 800E0600 000DD540  80 61 00 10 */	lwz r3, 0x10(r1)
/* 800E0604 000DD544  80 01 00 14 */	lwz r0, 0x14(r1)
/* 800E0608 000DD548  7C 85 22 14 */	add r4, r5, r4
/* 800E060C 000DD54C  90 64 00 00 */	stw r3, 0(r4)
/* 800E0610 000DD550  90 04 00 04 */	stw r0, 4(r4)
/* 800E0614 000DD554  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E0618 000DD558  38 03 00 01 */	addi r0, r3, 1
/* 800E061C 000DD55C  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800E0620:
/* 800E0620 000DD560  80 01 00 24 */	lwz r0, 0x24(r1)
/* 800E0624 000DD564  83 E1 00 1C */	lwz r31, 0x1c(r1)
/* 800E0628 000DD568  38 21 00 20 */	addi r1, r1, 0x20
/* 800E062C 000DD56C  7C 08 03 A6 */	mtlr r0
/* 800E0630 000DD570  4E 80 00 20 */	blr 

.global evChangeNozzle__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evChangeNozzle__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E0634 000DD574  7C 08 02 A6 */	mflr r0
/* 800E0638 000DD578  90 01 00 04 */	stw r0, 4(r1)
/* 800E063C 000DD57C  94 21 FF B0 */	stwu r1, -0x50(r1)
/* 800E0640 000DD580  93 E1 00 4C */	stw r31, 0x4c(r1)
/* 800E0644 000DD584  7C 7F 1B 78 */	mr r31, r3
/* 800E0648 000DD588  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E064C 000DD58C  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E0650 000DD590  38 80 00 01 */	li r4, 1
/* 800E0654 000DD594  48 03 6D DD */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E0658 000DD598  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E065C 000DD59C  2C 03 00 00 */	cmpwi r3, 0
/* 800E0660 000DD5A0  41 81 00 38 */	bgt lbl_800E0698
/* 800E0664 000DD5A4  3C 60 80 37 */	lis r3, $$23829@ha
/* 800E0668 000DD5A8  4C C6 31 82 */	crclr 6
/* 800E066C 000DD5AC  38 63 30 FC */	addi r3, r3, $$23829@l
/* 800E0670 000DD5B0  4B FF CA 35 */	bl SpcTrace__FPCce
/* 800E0674 000DD5B4  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800E0678 000DD5B8  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800E067C 000DD5BC  54 00 18 38 */	slwi r0, r0, 3
/* 800E0680 000DD5C0  7C 63 02 14 */	add r3, r3, r0
/* 800E0684 000DD5C4  80 03 00 00 */	lwz r0, 0(r3)
/* 800E0688 000DD5C8  90 01 00 38 */	stw r0, 0x38(r1)
/* 800E068C 000DD5CC  80 03 00 04 */	lwz r0, 4(r3)
/* 800E0690 000DD5D0  90 01 00 3C */	stw r0, 0x3c(r1)
/* 800E0694 000DD5D4  48 00 00 2C */	b lbl_800E06C0
lbl_800E0698:
/* 800E0698 000DD5D8  38 03 FF FF */	addi r0, r3, -1
/* 800E069C 000DD5DC  90 1F 00 1C */	stw r0, 0x1c(r31)
/* 800E06A0 000DD5E0  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800E06A4 000DD5E4  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800E06A8 000DD5E8  54 00 18 38 */	slwi r0, r0, 3
/* 800E06AC 000DD5EC  7C 63 02 14 */	add r3, r3, r0
/* 800E06B0 000DD5F0  80 03 00 00 */	lwz r0, 0(r3)
/* 800E06B4 000DD5F4  90 01 00 38 */	stw r0, 0x38(r1)
/* 800E06B8 000DD5F8  80 03 00 04 */	lwz r0, 4(r3)
/* 800E06BC 000DD5FC  90 01 00 3C */	stw r0, 0x3c(r1)
lbl_800E06C0:
/* 800E06C0 000DD600  80 01 00 38 */	lwz r0, 0x38(r1)
/* 800E06C4 000DD604  2C 00 00 01 */	cmpwi r0, 1
/* 800E06C8 000DD608  41 82 00 1C */	beq lbl_800E06E4
/* 800E06CC 000DD60C  40 80 00 2C */	bge lbl_800E06F8
/* 800E06D0 000DD610  2C 00 00 00 */	cmpwi r0, 0
/* 800E06D4 000DD614  40 80 00 08 */	bge lbl_800E06DC
/* 800E06D8 000DD618  48 00 00 20 */	b lbl_800E06F8
lbl_800E06DC:
/* 800E06DC 000DD61C  80 01 00 3C */	lwz r0, 0x3c(r1)
/* 800E06E0 000DD620  48 00 00 1C */	b lbl_800E06FC
lbl_800E06E4:
/* 800E06E4 000DD624  C0 01 00 3C */	lfs f0, 0x3c(r1)
/* 800E06E8 000DD628  FC 00 00 1E */	fctiwz f0, f0
/* 800E06EC 000DD62C  D8 01 00 40 */	stfd f0, 0x40(r1)
/* 800E06F0 000DD630  80 01 00 44 */	lwz r0, 0x44(r1)
/* 800E06F4 000DD634  48 00 00 08 */	b lbl_800E06FC
lbl_800E06F8:
/* 800E06F8 000DD638  38 00 00 00 */	li r0, 0
lbl_800E06FC:
/* 800E06FC 000DD63C  7C 04 03 78 */	mr r4, r0
/* 800E0700 000DD640  2C 04 00 07 */	cmpwi r4, 7
/* 800E0704 000DD644  40 82 00 10 */	bne lbl_800E0714
/* 800E0708 000DD648  80 6D 98 B8 */	lwz r3, gpMarioOriginal@sda21(r13)
/* 800E070C 000DD64C  48 04 6C 89 */	bl setDivHelm__6TMarioFv
/* 800E0710 000DD650  48 00 00 14 */	b lbl_800E0724
lbl_800E0714:
/* 800E0714 000DD654  80 6D 98 B8 */	lwz r3, gpMarioOriginal@sda21(r13)
/* 800E0718 000DD658  38 A0 00 01 */	li r5, 1
/* 800E071C 000DD65C  80 63 03 E4 */	lwz r3, 0x3e4(r3)
/* 800E0720 000DD660  48 06 8F C9 */	bl changeNozzle__9TWaterGunFQ29TWaterGun11TNozzleTypeb
lbl_800E0724:
/* 800E0724 000DD664  38 00 00 00 */	li r0, 0
/* 800E0728 000DD668  90 01 00 28 */	stw r0, 0x28(r1)
/* 800E072C 000DD66C  90 01 00 2C */	stw r0, 0x2c(r1)
/* 800E0730 000DD670  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E0734 000DD674  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800E0738 000DD678  7C 03 00 00 */	cmpw r3, r0
/* 800E073C 000DD67C  41 80 00 18 */	blt lbl_800E0754
/* 800E0740 000DD680  3C 60 80 37 */	lis r3, $$23834@ha
/* 800E0744 000DD684  4C C6 31 82 */	crclr 6
/* 800E0748 000DD688  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800E074C 000DD68C  4B FF C9 59 */	bl SpcTrace__FPCce
/* 800E0750 000DD690  48 00 00 2C */	b lbl_800E077C
lbl_800E0754:
/* 800E0754 000DD694  80 BF 00 20 */	lwz r5, 0x20(r31)
/* 800E0758 000DD698  54 64 18 38 */	slwi r4, r3, 3
/* 800E075C 000DD69C  80 61 00 28 */	lwz r3, 0x28(r1)
/* 800E0760 000DD6A0  80 01 00 2C */	lwz r0, 0x2c(r1)
/* 800E0764 000DD6A4  7C 85 22 14 */	add r4, r5, r4
/* 800E0768 000DD6A8  90 64 00 00 */	stw r3, 0(r4)
/* 800E076C 000DD6AC  90 04 00 04 */	stw r0, 4(r4)
/* 800E0770 000DD6B0  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E0774 000DD6B4  38 03 00 01 */	addi r0, r3, 1
/* 800E0778 000DD6B8  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800E077C:
/* 800E077C 000DD6BC  80 01 00 54 */	lwz r0, 0x54(r1)
/* 800E0780 000DD6C0  83 E1 00 4C */	lwz r31, 0x4c(r1)
/* 800E0784 000DD6C4  38 21 00 50 */	addi r1, r1, 0x50
/* 800E0788 000DD6C8  7C 08 03 A6 */	mtlr r0
/* 800E078C 000DD6CC  4E 80 00 20 */	blr 

.global evAppearShineForWoodBox__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evAppearShineForWoodBox__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E0790 000DD6D0  7C 08 02 A6 */	mflr r0
/* 800E0794 000DD6D4  90 01 00 04 */	stw r0, 4(r1)
/* 800E0798 000DD6D8  94 21 FF B8 */	stwu r1, -0x48(r1)
/* 800E079C 000DD6DC  93 E1 00 44 */	stw r31, 0x44(r1)
/* 800E07A0 000DD6E0  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E07A4 000DD6E4  93 C1 00 40 */	stw r30, 0x40(r1)
/* 800E07A8 000DD6E8  7C 7E 1B 78 */	mr r30, r3
/* 800E07AC 000DD6EC  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E07B0 000DD6F0  3C 80 80 37 */	lis r4, $$21490@ha
/* 800E07B4 000DD6F4  3B E4 29 90 */	addi r31, r4, $$21490@l
/* 800E07B8 000DD6F8  38 80 00 01 */	li r4, 1
/* 800E07BC 000DD6FC  48 03 6C 75 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E07C0 000DD700  80 7E 00 1C */	lwz r3, 0x1c(r30)
/* 800E07C4 000DD704  2C 03 00 00 */	cmpwi r3, 0
/* 800E07C8 000DD708  41 81 00 34 */	bgt lbl_800E07FC
/* 800E07CC 000DD70C  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800E07D0 000DD710  4C C6 31 82 */	crclr 6
/* 800E07D4 000DD714  4B FF C8 D1 */	bl SpcTrace__FPCce
/* 800E07D8 000DD718  80 1E 00 1C */	lwz r0, 0x1c(r30)
/* 800E07DC 000DD71C  80 7E 00 20 */	lwz r3, 0x20(r30)
/* 800E07E0 000DD720  54 00 18 38 */	slwi r0, r0, 3
/* 800E07E4 000DD724  7C 63 02 14 */	add r3, r3, r0
/* 800E07E8 000DD728  80 03 00 00 */	lwz r0, 0(r3)
/* 800E07EC 000DD72C  90 01 00 30 */	stw r0, 0x30(r1)
/* 800E07F0 000DD730  80 03 00 04 */	lwz r0, 4(r3)
/* 800E07F4 000DD734  90 01 00 34 */	stw r0, 0x34(r1)
/* 800E07F8 000DD738  48 00 00 2C */	b lbl_800E0824
lbl_800E07FC:
/* 800E07FC 000DD73C  38 03 FF FF */	addi r0, r3, -1
/* 800E0800 000DD740  90 1E 00 1C */	stw r0, 0x1c(r30)
/* 800E0804 000DD744  80 1E 00 1C */	lwz r0, 0x1c(r30)
/* 800E0808 000DD748  80 7E 00 20 */	lwz r3, 0x20(r30)
/* 800E080C 000DD74C  54 00 18 38 */	slwi r0, r0, 3
/* 800E0810 000DD750  7C 63 02 14 */	add r3, r3, r0
/* 800E0814 000DD754  80 03 00 00 */	lwz r0, 0(r3)
/* 800E0818 000DD758  90 01 00 30 */	stw r0, 0x30(r1)
/* 800E081C 000DD75C  80 03 00 04 */	lwz r0, 4(r3)
/* 800E0820 000DD760  90 01 00 34 */	stw r0, 0x34(r1)
lbl_800E0824:
/* 800E0824 000DD764  80 01 00 30 */	lwz r0, 0x30(r1)
/* 800E0828 000DD768  2C 00 00 01 */	cmpwi r0, 1
/* 800E082C 000DD76C  41 82 00 1C */	beq lbl_800E0848
/* 800E0830 000DD770  40 80 00 2C */	bge lbl_800E085C
/* 800E0834 000DD774  2C 00 00 00 */	cmpwi r0, 0
/* 800E0838 000DD778  40 80 00 08 */	bge lbl_800E0840
/* 800E083C 000DD77C  48 00 00 20 */	b lbl_800E085C
lbl_800E0840:
/* 800E0840 000DD780  80 01 00 34 */	lwz r0, 0x34(r1)
/* 800E0844 000DD784  48 00 00 1C */	b lbl_800E0860
lbl_800E0848:
/* 800E0848 000DD788  C0 01 00 34 */	lfs f0, 0x34(r1)
/* 800E084C 000DD78C  FC 00 00 1E */	fctiwz f0, f0
/* 800E0850 000DD790  D8 01 00 38 */	stfd f0, 0x38(r1)
/* 800E0854 000DD794  80 01 00 3C */	lwz r0, 0x3c(r1)
/* 800E0858 000DD798  48 00 00 08 */	b lbl_800E0860
lbl_800E085C:
/* 800E085C 000DD79C  38 00 00 00 */	li r0, 0
lbl_800E0860:
/* 800E0860 000DD7A0  2C 00 00 00 */	cmpwi r0, 0
/* 800E0864 000DD7A4  41 82 00 08 */	beq lbl_800E086C
/* 800E0868 000DD7A8  38 00 00 01 */	li r0, 1
lbl_800E086C:
/* 800E086C 000DD7AC  54 00 10 3A */	slwi r0, r0, 2
/* 800E0870 000DD7B0  80 6D 9A 18 */	lwz r3, gpItemManager@sda21(r13)
/* 800E0874 000DD7B4  38 8D 84 48 */	addi r4, r13, sShineViewObjName$3431@sda21
/* 800E0878 000DD7B8  C0 22 8F F8 */	lfs f1, $$25131@sda21(r2)
/* 800E087C 000DD7BC  7C 84 00 2E */	lwzx r4, r4, r0
/* 800E0880 000DD7C0  38 BF 07 F0 */	addi r5, r31, 0x7f0
/* 800E0884 000DD7C4  C0 42 8F FC */	lfs f2, $$25132@sda21(r2)
/* 800E0888 000DD7C8  C0 62 90 00 */	lfs f3, $$25133@sda21(r2)
/* 800E088C 000DD7CC  48 0B 77 45 */	bl makeShineAppearWithDemo__12TItemManagerFPCcPCcfff
/* 800E0890 000DD7D0  38 00 00 00 */	li r0, 0
/* 800E0894 000DD7D4  90 01 00 24 */	stw r0, 0x24(r1)
/* 800E0898 000DD7D8  90 01 00 28 */	stw r0, 0x28(r1)
/* 800E089C 000DD7DC  80 7E 00 1C */	lwz r3, 0x1c(r30)
/* 800E08A0 000DD7E0  80 1E 00 18 */	lwz r0, 0x18(r30)
/* 800E08A4 000DD7E4  7C 03 00 00 */	cmpw r3, r0
/* 800E08A8 000DD7E8  41 80 00 14 */	blt lbl_800E08BC
/* 800E08AC 000DD7EC  38 7F 07 8C */	addi r3, r31, 0x78c
/* 800E08B0 000DD7F0  4C C6 31 82 */	crclr 6
/* 800E08B4 000DD7F4  4B FF C7 F1 */	bl SpcTrace__FPCce
/* 800E08B8 000DD7F8  48 00 00 2C */	b lbl_800E08E4
lbl_800E08BC:
/* 800E08BC 000DD7FC  80 BE 00 20 */	lwz r5, 0x20(r30)
/* 800E08C0 000DD800  54 64 18 38 */	slwi r4, r3, 3
/* 800E08C4 000DD804  80 61 00 24 */	lwz r3, 0x24(r1)
/* 800E08C8 000DD808  80 01 00 28 */	lwz r0, 0x28(r1)
/* 800E08CC 000DD80C  7C 85 22 14 */	add r4, r5, r4
/* 800E08D0 000DD810  90 64 00 00 */	stw r3, 0(r4)
/* 800E08D4 000DD814  90 04 00 04 */	stw r0, 4(r4)
/* 800E08D8 000DD818  80 7E 00 1C */	lwz r3, 0x1c(r30)
/* 800E08DC 000DD81C  38 03 00 01 */	addi r0, r3, 1
/* 800E08E0 000DD820  90 1E 00 1C */	stw r0, 0x1c(r30)
lbl_800E08E4:
/* 800E08E4 000DD824  80 01 00 4C */	lwz r0, 0x4c(r1)
/* 800E08E8 000DD828  83 E1 00 44 */	lwz r31, 0x44(r1)
/* 800E08EC 000DD82C  83 C1 00 40 */	lwz r30, 0x40(r1)
/* 800E08F0 000DD830  7C 08 03 A6 */	mtlr r0
/* 800E08F4 000DD834  38 21 00 48 */	addi r1, r1, 0x48
/* 800E08F8 000DD838  4E 80 00 20 */	blr 

.global evAppearShineFromKageMario__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evAppearShineFromKageMario__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E08FC 000DD83C  7C 08 02 A6 */	mflr r0
/* 800E0900 000DD840  90 01 00 04 */	stw r0, 4(r1)
/* 800E0904 000DD844  94 21 FF 30 */	stwu r1, -0xd0(r1)
/* 800E0908 000DD848  BF 41 00 B8 */	stmw r26, 0xb8(r1)
/* 800E090C 000DD84C  7C 7C 1B 78 */	mr r28, r3
/* 800E0910 000DD850  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E0914 000DD854  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E0918 000DD858  3C 80 80 37 */	lis r4, $$21490@ha
/* 800E091C 000DD85C  3B E4 29 90 */	addi r31, r4, $$21490@l
/* 800E0920 000DD860  38 80 00 03 */	li r4, 3
/* 800E0924 000DD864  48 03 6B 0D */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E0928 000DD868  80 7C 00 1C */	lwz r3, 0x1c(r28)
/* 800E092C 000DD86C  2C 03 00 00 */	cmpwi r3, 0
/* 800E0930 000DD870  41 81 00 34 */	bgt lbl_800E0964
/* 800E0934 000DD874  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800E0938 000DD878  4C C6 31 82 */	crclr 6
/* 800E093C 000DD87C  4B FF C7 69 */	bl SpcTrace__FPCce
/* 800E0940 000DD880  80 1C 00 1C */	lwz r0, 0x1c(r28)
/* 800E0944 000DD884  80 7C 00 20 */	lwz r3, 0x20(r28)
/* 800E0948 000DD888  54 00 18 38 */	slwi r0, r0, 3
/* 800E094C 000DD88C  7C 63 02 14 */	add r3, r3, r0
/* 800E0950 000DD890  80 03 00 00 */	lwz r0, 0(r3)
/* 800E0954 000DD894  90 01 00 A0 */	stw r0, 0xa0(r1)
/* 800E0958 000DD898  80 03 00 04 */	lwz r0, 4(r3)
/* 800E095C 000DD89C  90 01 00 A4 */	stw r0, 0xa4(r1)
/* 800E0960 000DD8A0  48 00 00 2C */	b lbl_800E098C
lbl_800E0964:
/* 800E0964 000DD8A4  38 03 FF FF */	addi r0, r3, -1
/* 800E0968 000DD8A8  90 1C 00 1C */	stw r0, 0x1c(r28)
/* 800E096C 000DD8AC  80 1C 00 1C */	lwz r0, 0x1c(r28)
/* 800E0970 000DD8B0  80 7C 00 20 */	lwz r3, 0x20(r28)
/* 800E0974 000DD8B4  54 00 18 38 */	slwi r0, r0, 3
/* 800E0978 000DD8B8  7C 63 02 14 */	add r3, r3, r0
/* 800E097C 000DD8BC  80 03 00 00 */	lwz r0, 0(r3)
/* 800E0980 000DD8C0  90 01 00 A0 */	stw r0, 0xa0(r1)
/* 800E0984 000DD8C4  80 03 00 04 */	lwz r0, 4(r3)
/* 800E0988 000DD8C8  90 01 00 A4 */	stw r0, 0xa4(r1)
lbl_800E098C:
/* 800E098C 000DD8CC  80 01 00 A0 */	lwz r0, 0xa0(r1)
/* 800E0990 000DD8D0  2C 00 00 01 */	cmpwi r0, 1
/* 800E0994 000DD8D4  41 82 00 1C */	beq lbl_800E09B0
/* 800E0998 000DD8D8  40 80 00 2C */	bge lbl_800E09C4
/* 800E099C 000DD8DC  2C 00 00 00 */	cmpwi r0, 0
/* 800E09A0 000DD8E0  40 80 00 08 */	bge lbl_800E09A8
/* 800E09A4 000DD8E4  48 00 00 20 */	b lbl_800E09C4
lbl_800E09A8:
/* 800E09A8 000DD8E8  80 01 00 A4 */	lwz r0, 0xa4(r1)
/* 800E09AC 000DD8EC  48 00 00 1C */	b lbl_800E09C8
lbl_800E09B0:
/* 800E09B0 000DD8F0  C0 01 00 A4 */	lfs f0, 0xa4(r1)
/* 800E09B4 000DD8F4  FC 00 00 1E */	fctiwz f0, f0
/* 800E09B8 000DD8F8  D8 01 00 B0 */	stfd f0, 0xb0(r1)
/* 800E09BC 000DD8FC  80 01 00 B4 */	lwz r0, 0xb4(r1)
/* 800E09C0 000DD900  48 00 00 08 */	b lbl_800E09C8
lbl_800E09C4:
/* 800E09C4 000DD904  38 00 00 00 */	li r0, 0
lbl_800E09C8:
/* 800E09C8 000DD908  80 7C 00 1C */	lwz r3, 0x1c(r28)
/* 800E09CC 000DD90C  7C 1E 03 78 */	mr r30, r0
/* 800E09D0 000DD910  2C 03 00 00 */	cmpwi r3, 0
/* 800E09D4 000DD914  41 81 00 34 */	bgt lbl_800E0A08
/* 800E09D8 000DD918  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800E09DC 000DD91C  4C C6 31 82 */	crclr 6
/* 800E09E0 000DD920  4B FF C6 C5 */	bl SpcTrace__FPCce
/* 800E09E4 000DD924  80 1C 00 1C */	lwz r0, 0x1c(r28)
/* 800E09E8 000DD928  80 7C 00 20 */	lwz r3, 0x20(r28)
/* 800E09EC 000DD92C  54 00 18 38 */	slwi r0, r0, 3
/* 800E09F0 000DD930  7C 63 02 14 */	add r3, r3, r0
/* 800E09F4 000DD934  80 03 00 00 */	lwz r0, 0(r3)
/* 800E09F8 000DD938  90 01 00 98 */	stw r0, 0x98(r1)
/* 800E09FC 000DD93C  80 03 00 04 */	lwz r0, 4(r3)
/* 800E0A00 000DD940  90 01 00 9C */	stw r0, 0x9c(r1)
/* 800E0A04 000DD944  48 00 00 2C */	b lbl_800E0A30
lbl_800E0A08:
/* 800E0A08 000DD948  38 03 FF FF */	addi r0, r3, -1
/* 800E0A0C 000DD94C  90 1C 00 1C */	stw r0, 0x1c(r28)
/* 800E0A10 000DD950  80 1C 00 1C */	lwz r0, 0x1c(r28)
/* 800E0A14 000DD954  80 7C 00 20 */	lwz r3, 0x20(r28)
/* 800E0A18 000DD958  54 00 18 38 */	slwi r0, r0, 3
/* 800E0A1C 000DD95C  7C 63 02 14 */	add r3, r3, r0
/* 800E0A20 000DD960  80 03 00 00 */	lwz r0, 0(r3)
/* 800E0A24 000DD964  90 01 00 98 */	stw r0, 0x98(r1)
/* 800E0A28 000DD968  80 03 00 04 */	lwz r0, 4(r3)
/* 800E0A2C 000DD96C  90 01 00 9C */	stw r0, 0x9c(r1)
lbl_800E0A30:
/* 800E0A30 000DD970  80 01 00 98 */	lwz r0, 0x98(r1)
/* 800E0A34 000DD974  2C 00 00 02 */	cmpwi r0, 2
/* 800E0A38 000DD978  41 82 00 08 */	beq lbl_800E0A40
/* 800E0A3C 000DD97C  48 00 00 0C */	b lbl_800E0A48
lbl_800E0A40:
/* 800E0A40 000DD980  80 01 00 9C */	lwz r0, 0x9c(r1)
/* 800E0A44 000DD984  48 00 00 08 */	b lbl_800E0A4C
lbl_800E0A48:
/* 800E0A48 000DD988  38 02 8F F0 */	addi r0, r2, $$24053@sda21
lbl_800E0A4C:
/* 800E0A4C 000DD98C  80 7C 00 1C */	lwz r3, 0x1c(r28)
/* 800E0A50 000DD990  7C 1D 03 78 */	mr r29, r0
/* 800E0A54 000DD994  2C 03 00 00 */	cmpwi r3, 0
/* 800E0A58 000DD998  41 81 00 34 */	bgt lbl_800E0A8C
/* 800E0A5C 000DD99C  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800E0A60 000DD9A0  4C C6 31 82 */	crclr 6
/* 800E0A64 000DD9A4  4B FF C6 41 */	bl SpcTrace__FPCce
/* 800E0A68 000DD9A8  80 1C 00 1C */	lwz r0, 0x1c(r28)
/* 800E0A6C 000DD9AC  80 7C 00 20 */	lwz r3, 0x20(r28)
/* 800E0A70 000DD9B0  54 00 18 38 */	slwi r0, r0, 3
/* 800E0A74 000DD9B4  7C 63 02 14 */	add r3, r3, r0
/* 800E0A78 000DD9B8  80 03 00 00 */	lwz r0, 0(r3)
/* 800E0A7C 000DD9BC  90 01 00 90 */	stw r0, 0x90(r1)
/* 800E0A80 000DD9C0  80 03 00 04 */	lwz r0, 4(r3)
/* 800E0A84 000DD9C4  90 01 00 94 */	stw r0, 0x94(r1)
/* 800E0A88 000DD9C8  48 00 00 2C */	b lbl_800E0AB4
lbl_800E0A8C:
/* 800E0A8C 000DD9CC  38 03 FF FF */	addi r0, r3, -1
/* 800E0A90 000DD9D0  90 1C 00 1C */	stw r0, 0x1c(r28)
/* 800E0A94 000DD9D4  80 1C 00 1C */	lwz r0, 0x1c(r28)
/* 800E0A98 000DD9D8  80 7C 00 20 */	lwz r3, 0x20(r28)
/* 800E0A9C 000DD9DC  54 00 18 38 */	slwi r0, r0, 3
/* 800E0AA0 000DD9E0  7C 63 02 14 */	add r3, r3, r0
/* 800E0AA4 000DD9E4  80 03 00 00 */	lwz r0, 0(r3)
/* 800E0AA8 000DD9E8  90 01 00 90 */	stw r0, 0x90(r1)
/* 800E0AAC 000DD9EC  80 03 00 04 */	lwz r0, 4(r3)
/* 800E0AB0 000DD9F0  90 01 00 94 */	stw r0, 0x94(r1)
lbl_800E0AB4:
/* 800E0AB4 000DD9F4  80 01 00 90 */	lwz r0, 0x90(r1)
/* 800E0AB8 000DD9F8  2C 00 00 02 */	cmpwi r0, 2
/* 800E0ABC 000DD9FC  41 82 00 08 */	beq lbl_800E0AC4
/* 800E0AC0 000DDA00  48 00 00 0C */	b lbl_800E0ACC
lbl_800E0AC4:
/* 800E0AC4 000DDA04  80 01 00 94 */	lwz r0, 0x94(r1)
/* 800E0AC8 000DDA08  48 00 00 08 */	b lbl_800E0AD0
lbl_800E0ACC:
/* 800E0ACC 000DDA0C  38 02 8F F0 */	addi r0, r2, $$24053@sda21
lbl_800E0AD0:
/* 800E0AD0 000DDA10  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 800E0AD4 000DDA14  7C 1A 03 78 */	mr r26, r0
/* 800E0AD8 000DDA18  38 7D 00 00 */	addi r3, r29, 0
/* 800E0ADC 000DDA1C  83 64 00 04 */	lwz r27, 4(r4)
/* 800E0AE0 000DDA20  4B F6 3B AD */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800E0AE4 000DDA24  81 9B 00 00 */	lwz r12, 0(r27)
/* 800E0AE8 000DDA28  38 83 00 00 */	addi r4, r3, 0
/* 800E0AEC 000DDA2C  38 7B 00 00 */	addi r3, r27, 0
/* 800E0AF0 000DDA30  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 800E0AF4 000DDA34  7F A5 EB 78 */	mr r5, r29
/* 800E0AF8 000DDA38  7D 88 03 A6 */	mtlr r12
/* 800E0AFC 000DDA3C  4E 80 00 21 */	blrl 
/* 800E0B00 000DDA40  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 800E0B04 000DDA44  3B A3 00 00 */	addi r29, r3, 0
/* 800E0B08 000DDA48  38 7A 00 00 */	addi r3, r26, 0
/* 800E0B0C 000DDA4C  83 64 00 04 */	lwz r27, 4(r4)
/* 800E0B10 000DDA50  4B F6 3B 7D */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800E0B14 000DDA54  81 9B 00 00 */	lwz r12, 0(r27)
/* 800E0B18 000DDA58  38 83 00 00 */	addi r4, r3, 0
/* 800E0B1C 000DDA5C  38 7B 00 00 */	addi r3, r27, 0
/* 800E0B20 000DDA60  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 800E0B24 000DDA64  7F 45 D3 78 */	mr r5, r26
/* 800E0B28 000DDA68  7D 88 03 A6 */	mtlr r12
/* 800E0B2C 000DDA6C  4E 80 00 21 */	blrl 
/* 800E0B30 000DDA70  80 9D 00 10 */	lwz r4, 0x10(r29)
/* 800E0B34 000DDA74  80 1D 00 14 */	lwz r0, 0x14(r29)
/* 800E0B38 000DDA78  90 83 00 10 */	stw r4, 0x10(r3)
/* 800E0B3C 000DDA7C  7F C4 F3 78 */	mr r4, r30
/* 800E0B40 000DDA80  90 03 00 14 */	stw r0, 0x14(r3)
/* 800E0B44 000DDA84  80 1D 00 18 */	lwz r0, 0x18(r29)
/* 800E0B48 000DDA88  90 03 00 18 */	stw r0, 0x18(r3)
/* 800E0B4C 000DDA8C  48 0B 44 4D */	bl appearSimple__6TShineFi
/* 800E0B50 000DDA90  38 00 00 00 */	li r0, 0
/* 800E0B54 000DDA94  90 01 00 7C */	stw r0, 0x7c(r1)
/* 800E0B58 000DDA98  90 01 00 80 */	stw r0, 0x80(r1)
/* 800E0B5C 000DDA9C  80 7C 00 1C */	lwz r3, 0x1c(r28)
/* 800E0B60 000DDAA0  80 1C 00 18 */	lwz r0, 0x18(r28)
/* 800E0B64 000DDAA4  7C 03 00 00 */	cmpw r3, r0
/* 800E0B68 000DDAA8  41 80 00 14 */	blt lbl_800E0B7C
/* 800E0B6C 000DDAAC  38 7F 07 8C */	addi r3, r31, 0x78c
/* 800E0B70 000DDAB0  4C C6 31 82 */	crclr 6
/* 800E0B74 000DDAB4  4B FF C5 31 */	bl SpcTrace__FPCce
/* 800E0B78 000DDAB8  48 00 00 2C */	b lbl_800E0BA4
lbl_800E0B7C:
/* 800E0B7C 000DDABC  80 BC 00 20 */	lwz r5, 0x20(r28)
/* 800E0B80 000DDAC0  54 64 18 38 */	slwi r4, r3, 3
/* 800E0B84 000DDAC4  80 61 00 7C */	lwz r3, 0x7c(r1)
/* 800E0B88 000DDAC8  80 01 00 80 */	lwz r0, 0x80(r1)
/* 800E0B8C 000DDACC  7C 85 22 14 */	add r4, r5, r4
/* 800E0B90 000DDAD0  90 64 00 00 */	stw r3, 0(r4)
/* 800E0B94 000DDAD4  90 04 00 04 */	stw r0, 4(r4)
/* 800E0B98 000DDAD8  80 7C 00 1C */	lwz r3, 0x1c(r28)
/* 800E0B9C 000DDADC  38 03 00 01 */	addi r0, r3, 1
/* 800E0BA0 000DDAE0  90 1C 00 1C */	stw r0, 0x1c(r28)
lbl_800E0BA4:
/* 800E0BA4 000DDAE4  BB 41 00 B8 */	lmw r26, 0xb8(r1)
/* 800E0BA8 000DDAE8  80 01 00 D4 */	lwz r0, 0xd4(r1)
/* 800E0BAC 000DDAEC  38 21 00 D0 */	addi r1, r1, 0xd0
/* 800E0BB0 000DDAF0  7C 08 03 A6 */	mtlr r0
/* 800E0BB4 000DDAF4  4E 80 00 20 */	blr 

.global evAppearShineFromNPCWithoutDemo__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evAppearShineFromNPCWithoutDemo__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E0BB8 000DDAF8  7C 08 02 A6 */	mflr r0
/* 800E0BBC 000DDAFC  90 01 00 04 */	stw r0, 4(r1)
/* 800E0BC0 000DDB00  94 21 FF 50 */	stwu r1, -0xb0(r1)
/* 800E0BC4 000DDB04  BF 61 00 9C */	stmw r27, 0x9c(r1)
/* 800E0BC8 000DDB08  7C 7D 1B 78 */	mr r29, r3
/* 800E0BCC 000DDB0C  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E0BD0 000DDB10  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E0BD4 000DDB14  3C 80 80 37 */	lis r4, $$21490@ha
/* 800E0BD8 000DDB18  3B E4 29 90 */	addi r31, r4, $$21490@l
/* 800E0BDC 000DDB1C  38 80 00 02 */	li r4, 2
/* 800E0BE0 000DDB20  48 03 68 51 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E0BE4 000DDB24  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E0BE8 000DDB28  2C 03 00 00 */	cmpwi r3, 0
/* 800E0BEC 000DDB2C  41 81 00 30 */	bgt lbl_800E0C1C
/* 800E0BF0 000DDB30  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800E0BF4 000DDB34  4C C6 31 82 */	crclr 6
/* 800E0BF8 000DDB38  4B FF C4 AD */	bl SpcTrace__FPCce
/* 800E0BFC 000DDB3C  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E0C00 000DDB40  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E0C04 000DDB44  54 00 18 38 */	slwi r0, r0, 3
/* 800E0C08 000DDB48  7C 63 02 14 */	add r3, r3, r0
/* 800E0C0C 000DDB4C  80 83 00 00 */	lwz r4, 0(r3)
/* 800E0C10 000DDB50  80 03 00 04 */	lwz r0, 4(r3)
/* 800E0C14 000DDB54  90 01 00 80 */	stw r0, 0x80(r1)
/* 800E0C18 000DDB58  48 00 00 28 */	b lbl_800E0C40
lbl_800E0C1C:
/* 800E0C1C 000DDB5C  38 03 FF FF */	addi r0, r3, -1
/* 800E0C20 000DDB60  90 1D 00 1C */	stw r0, 0x1c(r29)
/* 800E0C24 000DDB64  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E0C28 000DDB68  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E0C2C 000DDB6C  54 00 18 38 */	slwi r0, r0, 3
/* 800E0C30 000DDB70  7C 63 02 14 */	add r3, r3, r0
/* 800E0C34 000DDB74  80 83 00 00 */	lwz r4, 0(r3)
/* 800E0C38 000DDB78  80 03 00 04 */	lwz r0, 4(r3)
/* 800E0C3C 000DDB7C  90 01 00 80 */	stw r0, 0x80(r1)
lbl_800E0C40:
/* 800E0C40 000DDB80  90 81 00 84 */	stw r4, 0x84(r1)
/* 800E0C44 000DDB84  80 01 00 80 */	lwz r0, 0x80(r1)
/* 800E0C48 000DDB88  90 01 00 88 */	stw r0, 0x88(r1)
/* 800E0C4C 000DDB8C  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E0C50 000DDB90  2C 03 00 00 */	cmpwi r3, 0
/* 800E0C54 000DDB94  41 81 00 34 */	bgt lbl_800E0C88
/* 800E0C58 000DDB98  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800E0C5C 000DDB9C  4C C6 31 82 */	crclr 6
/* 800E0C60 000DDBA0  4B FF C4 45 */	bl SpcTrace__FPCce
/* 800E0C64 000DDBA4  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E0C68 000DDBA8  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E0C6C 000DDBAC  54 00 18 38 */	slwi r0, r0, 3
/* 800E0C70 000DDBB0  7C 63 02 14 */	add r3, r3, r0
/* 800E0C74 000DDBB4  80 03 00 00 */	lwz r0, 0(r3)
/* 800E0C78 000DDBB8  90 01 00 74 */	stw r0, 0x74(r1)
/* 800E0C7C 000DDBBC  80 03 00 04 */	lwz r0, 4(r3)
/* 800E0C80 000DDBC0  90 01 00 78 */	stw r0, 0x78(r1)
/* 800E0C84 000DDBC4  48 00 00 2C */	b lbl_800E0CB0
lbl_800E0C88:
/* 800E0C88 000DDBC8  38 03 FF FF */	addi r0, r3, -1
/* 800E0C8C 000DDBCC  90 1D 00 1C */	stw r0, 0x1c(r29)
/* 800E0C90 000DDBD0  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E0C94 000DDBD4  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E0C98 000DDBD8  54 00 18 38 */	slwi r0, r0, 3
/* 800E0C9C 000DDBDC  7C 63 02 14 */	add r3, r3, r0
/* 800E0CA0 000DDBE0  80 03 00 00 */	lwz r0, 0(r3)
/* 800E0CA4 000DDBE4  90 01 00 74 */	stw r0, 0x74(r1)
/* 800E0CA8 000DDBE8  80 03 00 04 */	lwz r0, 4(r3)
/* 800E0CAC 000DDBEC  90 01 00 78 */	stw r0, 0x78(r1)
lbl_800E0CB0:
/* 800E0CB0 000DDBF0  80 01 00 74 */	lwz r0, 0x74(r1)
/* 800E0CB4 000DDBF4  2C 00 00 02 */	cmpwi r0, 2
/* 800E0CB8 000DDBF8  41 82 00 08 */	beq lbl_800E0CC0
/* 800E0CBC 000DDBFC  48 00 00 0C */	b lbl_800E0CC8
lbl_800E0CC0:
/* 800E0CC0 000DDC00  80 01 00 78 */	lwz r0, 0x78(r1)
/* 800E0CC4 000DDC04  48 00 00 08 */	b lbl_800E0CCC
lbl_800E0CC8:
/* 800E0CC8 000DDC08  38 02 8F F0 */	addi r0, r2, $$24053@sda21
lbl_800E0CCC:
/* 800E0CCC 000DDC0C  80 61 00 84 */	lwz r3, 0x84(r1)
/* 800E0CD0 000DDC10  7C 1E 03 78 */	mr r30, r0
/* 800E0CD4 000DDC14  80 01 00 88 */	lwz r0, 0x88(r1)
/* 800E0CD8 000DDC18  3B 80 00 00 */	li r28, 0
/* 800E0CDC 000DDC1C  90 61 00 6C */	stw r3, 0x6c(r1)
/* 800E0CE0 000DDC20  90 01 00 70 */	stw r0, 0x70(r1)
/* 800E0CE4 000DDC24  80 01 00 6C */	lwz r0, 0x6c(r1)
/* 800E0CE8 000DDC28  2C 00 00 01 */	cmpwi r0, 1
/* 800E0CEC 000DDC2C  41 82 00 A0 */	beq lbl_800E0D8C
/* 800E0CF0 000DDC30  40 80 00 10 */	bge lbl_800E0D00
/* 800E0CF4 000DDC34  2C 00 00 00 */	cmpwi r0, 0
/* 800E0CF8 000DDC38  40 80 00 5C */	bge lbl_800E0D54
/* 800E0CFC 000DDC3C  48 00 00 90 */	b lbl_800E0D8C
lbl_800E0D00:
/* 800E0D00 000DDC40  2C 00 00 03 */	cmpwi r0, 3
/* 800E0D04 000DDC44  40 80 00 88 */	bge lbl_800E0D8C
/* 800E0D08 000DDC48  2C 00 00 02 */	cmpwi r0, 2
/* 800E0D0C 000DDC4C  41 82 00 08 */	beq lbl_800E0D14
/* 800E0D10 000DDC50  48 00 00 0C */	b lbl_800E0D1C
lbl_800E0D14:
/* 800E0D14 000DDC54  83 81 00 70 */	lwz r28, 0x70(r1)
/* 800E0D18 000DDC58  48 00 00 08 */	b lbl_800E0D20
lbl_800E0D1C:
/* 800E0D1C 000DDC5C  3B 82 8F F0 */	addi r28, r2, $$24053@sda21
lbl_800E0D20:
/* 800E0D20 000DDC60  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 800E0D24 000DDC64  7F 83 E3 78 */	mr r3, r28
/* 800E0D28 000DDC68  83 64 00 04 */	lwz r27, 4(r4)
/* 800E0D2C 000DDC6C  4B F6 39 61 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800E0D30 000DDC70  81 9B 00 00 */	lwz r12, 0(r27)
/* 800E0D34 000DDC74  38 83 00 00 */	addi r4, r3, 0
/* 800E0D38 000DDC78  38 7B 00 00 */	addi r3, r27, 0
/* 800E0D3C 000DDC7C  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 800E0D40 000DDC80  7F 85 E3 78 */	mr r5, r28
/* 800E0D44 000DDC84  7D 88 03 A6 */	mtlr r12
/* 800E0D48 000DDC88  4E 80 00 21 */	blrl 
/* 800E0D4C 000DDC8C  7C 7C 1B 78 */	mr r28, r3
/* 800E0D50 000DDC90  48 00 00 3C */	b lbl_800E0D8C
lbl_800E0D54:
/* 800E0D54 000DDC94  2C 00 00 01 */	cmpwi r0, 1
/* 800E0D58 000DDC98  41 82 00 1C */	beq lbl_800E0D74
/* 800E0D5C 000DDC9C  40 80 00 2C */	bge lbl_800E0D88
/* 800E0D60 000DDCA0  2C 00 00 00 */	cmpwi r0, 0
/* 800E0D64 000DDCA4  40 80 00 08 */	bge lbl_800E0D6C
/* 800E0D68 000DDCA8  48 00 00 20 */	b lbl_800E0D88
lbl_800E0D6C:
/* 800E0D6C 000DDCAC  83 81 00 70 */	lwz r28, 0x70(r1)
/* 800E0D70 000DDCB0  48 00 00 1C */	b lbl_800E0D8C
lbl_800E0D74:
/* 800E0D74 000DDCB4  C0 01 00 70 */	lfs f0, 0x70(r1)
/* 800E0D78 000DDCB8  FC 00 00 1E */	fctiwz f0, f0
/* 800E0D7C 000DDCBC  D8 01 00 90 */	stfd f0, 0x90(r1)
/* 800E0D80 000DDCC0  83 81 00 94 */	lwz r28, 0x94(r1)
/* 800E0D84 000DDCC4  48 00 00 08 */	b lbl_800E0D8C
lbl_800E0D88:
/* 800E0D88 000DDCC8  3B 80 00 00 */	li r28, 0
lbl_800E0D8C:
/* 800E0D8C 000DDCCC  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 800E0D90 000DDCD0  7F C3 F3 78 */	mr r3, r30
/* 800E0D94 000DDCD4  83 64 00 04 */	lwz r27, 4(r4)
/* 800E0D98 000DDCD8  4B F6 38 F5 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800E0D9C 000DDCDC  81 9B 00 00 */	lwz r12, 0(r27)
/* 800E0DA0 000DDCE0  38 83 00 00 */	addi r4, r3, 0
/* 800E0DA4 000DDCE4  38 7B 00 00 */	addi r3, r27, 0
/* 800E0DA8 000DDCE8  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 800E0DAC 000DDCEC  7F C5 F3 78 */	mr r5, r30
/* 800E0DB0 000DDCF0  7D 88 03 A6 */	mtlr r12
/* 800E0DB4 000DDCF4  4E 80 00 21 */	blrl 
/* 800E0DB8 000DDCF8  C0 1C 00 10 */	lfs f0, 0x10(r28)
/* 800E0DBC 000DDCFC  D0 03 00 10 */	stfs f0, 0x10(r3)
/* 800E0DC0 000DDD00  C0 1C 00 14 */	lfs f0, 0x14(r28)
/* 800E0DC4 000DDD04  D0 03 00 14 */	stfs f0, 0x14(r3)
/* 800E0DC8 000DDD08  C0 1C 00 18 */	lfs f0, 0x18(r28)
/* 800E0DCC 000DDD0C  D0 03 00 18 */	stfs f0, 0x18(r3)
/* 800E0DD0 000DDD10  81 83 00 00 */	lwz r12, 0(r3)
/* 800E0DD4 000DDD14  81 8C 01 00 */	lwz r12, 0x100(r12)
/* 800E0DD8 000DDD18  7D 88 03 A6 */	mtlr r12
/* 800E0DDC 000DDD1C  4E 80 00 21 */	blrl 
/* 800E0DE0 000DDD20  38 00 00 00 */	li r0, 0
/* 800E0DE4 000DDD24  90 01 00 58 */	stw r0, 0x58(r1)
/* 800E0DE8 000DDD28  90 01 00 5C */	stw r0, 0x5c(r1)
/* 800E0DEC 000DDD2C  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E0DF0 000DDD30  80 1D 00 18 */	lwz r0, 0x18(r29)
/* 800E0DF4 000DDD34  7C 03 00 00 */	cmpw r3, r0
/* 800E0DF8 000DDD38  41 80 00 14 */	blt lbl_800E0E0C
/* 800E0DFC 000DDD3C  38 7F 07 8C */	addi r3, r31, 0x78c
/* 800E0E00 000DDD40  4C C6 31 82 */	crclr 6
/* 800E0E04 000DDD44  4B FF C2 A1 */	bl SpcTrace__FPCce
/* 800E0E08 000DDD48  48 00 00 2C */	b lbl_800E0E34
lbl_800E0E0C:
/* 800E0E0C 000DDD4C  80 BD 00 20 */	lwz r5, 0x20(r29)
/* 800E0E10 000DDD50  54 64 18 38 */	slwi r4, r3, 3
/* 800E0E14 000DDD54  80 61 00 58 */	lwz r3, 0x58(r1)
/* 800E0E18 000DDD58  80 01 00 5C */	lwz r0, 0x5c(r1)
/* 800E0E1C 000DDD5C  7C 85 22 14 */	add r4, r5, r4
/* 800E0E20 000DDD60  90 64 00 00 */	stw r3, 0(r4)
/* 800E0E24 000DDD64  90 04 00 04 */	stw r0, 4(r4)
/* 800E0E28 000DDD68  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E0E2C 000DDD6C  38 03 00 01 */	addi r0, r3, 1
/* 800E0E30 000DDD70  90 1D 00 1C */	stw r0, 0x1c(r29)
lbl_800E0E34:
/* 800E0E34 000DDD74  BB 61 00 9C */	lmw r27, 0x9c(r1)
/* 800E0E38 000DDD78  80 01 00 B4 */	lwz r0, 0xb4(r1)
/* 800E0E3C 000DDD7C  38 21 00 B0 */	addi r1, r1, 0xb0
/* 800E0E40 000DDD80  7C 08 03 A6 */	mtlr r0
/* 800E0E44 000DDD84  4E 80 00 20 */	blr 

.global evAppearShine__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evAppearShine__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E0E48 000DDD88  7C 08 02 A6 */	mflr r0
/* 800E0E4C 000DDD8C  90 01 00 04 */	stw r0, 4(r1)
/* 800E0E50 000DDD90  94 21 FF 80 */	stwu r1, -0x80(r1)
/* 800E0E54 000DDD94  93 E1 00 7C */	stw r31, 0x7c(r1)
/* 800E0E58 000DDD98  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E0E5C 000DDD9C  93 C1 00 78 */	stw r30, 0x78(r1)
/* 800E0E60 000DDDA0  93 A1 00 74 */	stw r29, 0x74(r1)
/* 800E0E64 000DDDA4  7C 7D 1B 78 */	mr r29, r3
/* 800E0E68 000DDDA8  93 81 00 70 */	stw r28, 0x70(r1)
/* 800E0E6C 000DDDAC  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E0E70 000DDDB0  3C 80 80 37 */	lis r4, $$21490@ha
/* 800E0E74 000DDDB4  3B E4 29 90 */	addi r31, r4, $$21490@l
/* 800E0E78 000DDDB8  38 80 00 02 */	li r4, 2
/* 800E0E7C 000DDDBC  48 03 65 B5 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E0E80 000DDDC0  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E0E84 000DDDC4  2C 03 00 00 */	cmpwi r3, 0
/* 800E0E88 000DDDC8  41 81 00 34 */	bgt lbl_800E0EBC
/* 800E0E8C 000DDDCC  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800E0E90 000DDDD0  4C C6 31 82 */	crclr 6
/* 800E0E94 000DDDD4  4B FF C2 11 */	bl SpcTrace__FPCce
/* 800E0E98 000DDDD8  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E0E9C 000DDDDC  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E0EA0 000DDDE0  54 00 18 38 */	slwi r0, r0, 3
/* 800E0EA4 000DDDE4  7C 63 02 14 */	add r3, r3, r0
/* 800E0EA8 000DDDE8  80 03 00 00 */	lwz r0, 0(r3)
/* 800E0EAC 000DDDEC  90 01 00 68 */	stw r0, 0x68(r1)
/* 800E0EB0 000DDDF0  80 03 00 04 */	lwz r0, 4(r3)
/* 800E0EB4 000DDDF4  90 01 00 6C */	stw r0, 0x6c(r1)
/* 800E0EB8 000DDDF8  48 00 00 2C */	b lbl_800E0EE4
lbl_800E0EBC:
/* 800E0EBC 000DDDFC  38 03 FF FF */	addi r0, r3, -1
/* 800E0EC0 000DDE00  90 1D 00 1C */	stw r0, 0x1c(r29)
/* 800E0EC4 000DDE04  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E0EC8 000DDE08  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E0ECC 000DDE0C  54 00 18 38 */	slwi r0, r0, 3
/* 800E0ED0 000DDE10  7C 63 02 14 */	add r3, r3, r0
/* 800E0ED4 000DDE14  80 03 00 00 */	lwz r0, 0(r3)
/* 800E0ED8 000DDE18  90 01 00 68 */	stw r0, 0x68(r1)
/* 800E0EDC 000DDE1C  80 03 00 04 */	lwz r0, 4(r3)
/* 800E0EE0 000DDE20  90 01 00 6C */	stw r0, 0x6c(r1)
lbl_800E0EE4:
/* 800E0EE4 000DDE24  80 01 00 68 */	lwz r0, 0x68(r1)
/* 800E0EE8 000DDE28  2C 00 00 02 */	cmpwi r0, 2
/* 800E0EEC 000DDE2C  41 82 00 08 */	beq lbl_800E0EF4
/* 800E0EF0 000DDE30  48 00 00 0C */	b lbl_800E0EFC
lbl_800E0EF4:
/* 800E0EF4 000DDE34  80 01 00 6C */	lwz r0, 0x6c(r1)
/* 800E0EF8 000DDE38  48 00 00 08 */	b lbl_800E0F00
lbl_800E0EFC:
/* 800E0EFC 000DDE3C  38 02 8F F0 */	addi r0, r2, $$24053@sda21
lbl_800E0F00:
/* 800E0F00 000DDE40  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E0F04 000DDE44  7C 1E 03 78 */	mr r30, r0
/* 800E0F08 000DDE48  2C 03 00 00 */	cmpwi r3, 0
/* 800E0F0C 000DDE4C  41 81 00 34 */	bgt lbl_800E0F40
/* 800E0F10 000DDE50  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800E0F14 000DDE54  4C C6 31 82 */	crclr 6
/* 800E0F18 000DDE58  4B FF C1 8D */	bl SpcTrace__FPCce
/* 800E0F1C 000DDE5C  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E0F20 000DDE60  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E0F24 000DDE64  54 00 18 38 */	slwi r0, r0, 3
/* 800E0F28 000DDE68  7C 63 02 14 */	add r3, r3, r0
/* 800E0F2C 000DDE6C  80 03 00 00 */	lwz r0, 0(r3)
/* 800E0F30 000DDE70  90 01 00 60 */	stw r0, 0x60(r1)
/* 800E0F34 000DDE74  80 03 00 04 */	lwz r0, 4(r3)
/* 800E0F38 000DDE78  90 01 00 64 */	stw r0, 0x64(r1)
/* 800E0F3C 000DDE7C  48 00 00 2C */	b lbl_800E0F68
lbl_800E0F40:
/* 800E0F40 000DDE80  38 03 FF FF */	addi r0, r3, -1
/* 800E0F44 000DDE84  90 1D 00 1C */	stw r0, 0x1c(r29)
/* 800E0F48 000DDE88  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E0F4C 000DDE8C  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E0F50 000DDE90  54 00 18 38 */	slwi r0, r0, 3
/* 800E0F54 000DDE94  7C 63 02 14 */	add r3, r3, r0
/* 800E0F58 000DDE98  80 03 00 00 */	lwz r0, 0(r3)
/* 800E0F5C 000DDE9C  90 01 00 60 */	stw r0, 0x60(r1)
/* 800E0F60 000DDEA0  80 03 00 04 */	lwz r0, 4(r3)
/* 800E0F64 000DDEA4  90 01 00 64 */	stw r0, 0x64(r1)
lbl_800E0F68:
/* 800E0F68 000DDEA8  80 01 00 60 */	lwz r0, 0x60(r1)
/* 800E0F6C 000DDEAC  2C 00 00 02 */	cmpwi r0, 2
/* 800E0F70 000DDEB0  41 82 00 08 */	beq lbl_800E0F78
/* 800E0F74 000DDEB4  48 00 00 0C */	b lbl_800E0F80
lbl_800E0F78:
/* 800E0F78 000DDEB8  80 01 00 64 */	lwz r0, 0x64(r1)
/* 800E0F7C 000DDEBC  48 00 00 08 */	b lbl_800E0F84
lbl_800E0F80:
/* 800E0F80 000DDEC0  38 02 8F F0 */	addi r0, r2, $$24053@sda21
lbl_800E0F84:
/* 800E0F84 000DDEC4  7C 1C 03 78 */	mr r28, r0
/* 800E0F88 000DDEC8  38 7E 00 00 */	addi r3, r30, 0
/* 800E0F8C 000DDECC  38 82 8F F0 */	addi r4, r2, $$24053@sda21
/* 800E0F90 000DDED0  4B FA 4C CD */	bl strcmp
/* 800E0F94 000DDED4  2C 03 00 00 */	cmpwi r3, 0
/* 800E0F98 000DDED8  41 82 00 24 */	beq lbl_800E0FBC
/* 800E0F9C 000DDEDC  C0 22 8F D0 */	lfs f1, $$23832@sda21(r2)
/* 800E0FA0 000DDEE0  7F 84 E3 78 */	mr r4, r28
/* 800E0FA4 000DDEE4  80 6D 9A 18 */	lwz r3, gpItemManager@sda21(r13)
/* 800E0FA8 000DDEE8  7F C5 F3 78 */	mr r5, r30
/* 800E0FAC 000DDEEC  FC 40 08 90 */	fmr f2, f1
/* 800E0FB0 000DDEF0  FC 60 08 90 */	fmr f3, f1
/* 800E0FB4 000DDEF4  48 0B 6F 5D */	bl makeShineAppearWithDemoOffset__12TItemManagerFPCcPCcfff
/* 800E0FB8 000DDEF8  48 00 00 44 */	b lbl_800E0FFC
lbl_800E0FBC:
/* 800E0FBC 000DDEFC  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 800E0FC0 000DDF00  7F 83 E3 78 */	mr r3, r28
/* 800E0FC4 000DDF04  83 C4 00 04 */	lwz r30, 4(r4)
/* 800E0FC8 000DDF08  4B F6 36 C5 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800E0FCC 000DDF0C  81 9E 00 00 */	lwz r12, 0(r30)
/* 800E0FD0 000DDF10  38 83 00 00 */	addi r4, r3, 0
/* 800E0FD4 000DDF14  38 7E 00 00 */	addi r3, r30, 0
/* 800E0FD8 000DDF18  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 800E0FDC 000DDF1C  7F 85 E3 78 */	mr r5, r28
/* 800E0FE0 000DDF20  7D 88 03 A6 */	mtlr r12
/* 800E0FE4 000DDF24  4E 80 00 21 */	blrl 
/* 800E0FE8 000DDF28  38 80 04 B0 */	li r4, 0x4b0
/* 800E0FEC 000DDF2C  38 A0 FF FF */	li r5, -1
/* 800E0FF0 000DDF30  38 C0 FF FF */	li r6, -1
/* 800E0FF4 000DDF34  38 E0 FF FF */	li r7, -1
/* 800E0FF8 000DDF38  48 0B 41 05 */	bl appearWithTime__6TShineFiiii
lbl_800E0FFC:
/* 800E0FFC 000DDF3C  38 00 00 00 */	li r0, 0
/* 800E1000 000DDF40  90 01 00 50 */	stw r0, 0x50(r1)
/* 800E1004 000DDF44  90 01 00 54 */	stw r0, 0x54(r1)
/* 800E1008 000DDF48  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E100C 000DDF4C  80 1D 00 18 */	lwz r0, 0x18(r29)
/* 800E1010 000DDF50  7C 03 00 00 */	cmpw r3, r0
/* 800E1014 000DDF54  41 80 00 14 */	blt lbl_800E1028
/* 800E1018 000DDF58  38 7F 07 8C */	addi r3, r31, 0x78c
/* 800E101C 000DDF5C  4C C6 31 82 */	crclr 6
/* 800E1020 000DDF60  4B FF C0 85 */	bl SpcTrace__FPCce
/* 800E1024 000DDF64  48 00 00 2C */	b lbl_800E1050
lbl_800E1028:
/* 800E1028 000DDF68  80 BD 00 20 */	lwz r5, 0x20(r29)
/* 800E102C 000DDF6C  54 64 18 38 */	slwi r4, r3, 3
/* 800E1030 000DDF70  80 61 00 50 */	lwz r3, 0x50(r1)
/* 800E1034 000DDF74  80 01 00 54 */	lwz r0, 0x54(r1)
/* 800E1038 000DDF78  7C 85 22 14 */	add r4, r5, r4
/* 800E103C 000DDF7C  90 64 00 00 */	stw r3, 0(r4)
/* 800E1040 000DDF80  90 04 00 04 */	stw r0, 4(r4)
/* 800E1044 000DDF84  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E1048 000DDF88  38 03 00 01 */	addi r0, r3, 1
/* 800E104C 000DDF8C  90 1D 00 1C */	stw r0, 0x1c(r29)
lbl_800E1050:
/* 800E1050 000DDF90  80 01 00 84 */	lwz r0, 0x84(r1)
/* 800E1054 000DDF94  83 E1 00 7C */	lwz r31, 0x7c(r1)
/* 800E1058 000DDF98  83 C1 00 78 */	lwz r30, 0x78(r1)
/* 800E105C 000DDF9C  7C 08 03 A6 */	mtlr r0
/* 800E1060 000DDFA0  83 A1 00 74 */	lwz r29, 0x74(r1)
/* 800E1064 000DDFA4  83 81 00 70 */	lwz r28, 0x70(r1)
/* 800E1068 000DDFA8  38 21 00 80 */	addi r1, r1, 0x80
/* 800E106C 000DDFAC  4E 80 00 20 */	blr 

.global evAppearShineFromNPC__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evAppearShineFromNPC__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E1070 000DDFB0  7C 08 02 A6 */	mflr r0
/* 800E1074 000DDFB4  90 01 00 04 */	stw r0, 4(r1)
/* 800E1078 000DDFB8  94 21 FF 30 */	stwu r1, -0xd0(r1)
/* 800E107C 000DDFBC  BF 41 00 B8 */	stmw r26, 0xb8(r1)
/* 800E1080 000DDFC0  7C 7B 1B 78 */	mr r27, r3
/* 800E1084 000DDFC4  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E1088 000DDFC8  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E108C 000DDFCC  3C 80 80 37 */	lis r4, $$21490@ha
/* 800E1090 000DDFD0  3B E4 29 90 */	addi r31, r4, $$21490@l
/* 800E1094 000DDFD4  38 80 00 03 */	li r4, 3
/* 800E1098 000DDFD8  48 03 63 99 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E109C 000DDFDC  80 7B 00 1C */	lwz r3, 0x1c(r27)
/* 800E10A0 000DDFE0  2C 03 00 00 */	cmpwi r3, 0
/* 800E10A4 000DDFE4  41 81 00 34 */	bgt lbl_800E10D8
/* 800E10A8 000DDFE8  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800E10AC 000DDFEC  4C C6 31 82 */	crclr 6
/* 800E10B0 000DDFF0  4B FF BF F5 */	bl SpcTrace__FPCce
/* 800E10B4 000DDFF4  80 1B 00 1C */	lwz r0, 0x1c(r27)
/* 800E10B8 000DDFF8  80 7B 00 20 */	lwz r3, 0x20(r27)
/* 800E10BC 000DDFFC  54 00 18 38 */	slwi r0, r0, 3
/* 800E10C0 000DE000  7C 63 02 14 */	add r3, r3, r0
/* 800E10C4 000DE004  80 03 00 00 */	lwz r0, 0(r3)
/* 800E10C8 000DE008  90 01 00 9C */	stw r0, 0x9c(r1)
/* 800E10CC 000DE00C  80 03 00 04 */	lwz r0, 4(r3)
/* 800E10D0 000DE010  90 01 00 A0 */	stw r0, 0xa0(r1)
/* 800E10D4 000DE014  48 00 00 2C */	b lbl_800E1100
lbl_800E10D8:
/* 800E10D8 000DE018  38 03 FF FF */	addi r0, r3, -1
/* 800E10DC 000DE01C  90 1B 00 1C */	stw r0, 0x1c(r27)
/* 800E10E0 000DE020  80 1B 00 1C */	lwz r0, 0x1c(r27)
/* 800E10E4 000DE024  80 7B 00 20 */	lwz r3, 0x20(r27)
/* 800E10E8 000DE028  54 00 18 38 */	slwi r0, r0, 3
/* 800E10EC 000DE02C  7C 63 02 14 */	add r3, r3, r0
/* 800E10F0 000DE030  80 03 00 00 */	lwz r0, 0(r3)
/* 800E10F4 000DE034  90 01 00 9C */	stw r0, 0x9c(r1)
/* 800E10F8 000DE038  80 03 00 04 */	lwz r0, 4(r3)
/* 800E10FC 000DE03C  90 01 00 A0 */	stw r0, 0xa0(r1)
lbl_800E1100:
/* 800E1100 000DE040  80 01 00 9C */	lwz r0, 0x9c(r1)
/* 800E1104 000DE044  2C 00 00 02 */	cmpwi r0, 2
/* 800E1108 000DE048  41 82 00 08 */	beq lbl_800E1110
/* 800E110C 000DE04C  48 00 00 0C */	b lbl_800E1118
lbl_800E1110:
/* 800E1110 000DE050  80 01 00 A0 */	lwz r0, 0xa0(r1)
/* 800E1114 000DE054  48 00 00 08 */	b lbl_800E111C
lbl_800E1118:
/* 800E1118 000DE058  38 02 8F F0 */	addi r0, r2, $$24053@sda21
lbl_800E111C:
/* 800E111C 000DE05C  80 7B 00 1C */	lwz r3, 0x1c(r27)
/* 800E1120 000DE060  7C 1D 03 78 */	mr r29, r0
/* 800E1124 000DE064  2C 03 00 00 */	cmpwi r3, 0
/* 800E1128 000DE068  41 81 00 30 */	bgt lbl_800E1158
/* 800E112C 000DE06C  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800E1130 000DE070  4C C6 31 82 */	crclr 6
/* 800E1134 000DE074  4B FF BF 71 */	bl SpcTrace__FPCce
/* 800E1138 000DE078  80 1B 00 1C */	lwz r0, 0x1c(r27)
/* 800E113C 000DE07C  80 7B 00 20 */	lwz r3, 0x20(r27)
/* 800E1140 000DE080  54 00 18 38 */	slwi r0, r0, 3
/* 800E1144 000DE084  7C 63 02 14 */	add r3, r3, r0
/* 800E1148 000DE088  80 83 00 00 */	lwz r4, 0(r3)
/* 800E114C 000DE08C  80 03 00 04 */	lwz r0, 4(r3)
/* 800E1150 000DE090  90 01 00 98 */	stw r0, 0x98(r1)
/* 800E1154 000DE094  48 00 00 28 */	b lbl_800E117C
lbl_800E1158:
/* 800E1158 000DE098  38 03 FF FF */	addi r0, r3, -1
/* 800E115C 000DE09C  90 1B 00 1C */	stw r0, 0x1c(r27)
/* 800E1160 000DE0A0  80 1B 00 1C */	lwz r0, 0x1c(r27)
/* 800E1164 000DE0A4  80 7B 00 20 */	lwz r3, 0x20(r27)
/* 800E1168 000DE0A8  54 00 18 38 */	slwi r0, r0, 3
/* 800E116C 000DE0AC  7C 63 02 14 */	add r3, r3, r0
/* 800E1170 000DE0B0  80 83 00 00 */	lwz r4, 0(r3)
/* 800E1174 000DE0B4  80 03 00 04 */	lwz r0, 4(r3)
/* 800E1178 000DE0B8  90 01 00 98 */	stw r0, 0x98(r1)
lbl_800E117C:
/* 800E117C 000DE0BC  90 81 00 A4 */	stw r4, 0xa4(r1)
/* 800E1180 000DE0C0  80 01 00 98 */	lwz r0, 0x98(r1)
/* 800E1184 000DE0C4  90 01 00 A8 */	stw r0, 0xa8(r1)
/* 800E1188 000DE0C8  80 7B 00 1C */	lwz r3, 0x1c(r27)
/* 800E118C 000DE0CC  2C 03 00 00 */	cmpwi r3, 0
/* 800E1190 000DE0D0  41 81 00 34 */	bgt lbl_800E11C4
/* 800E1194 000DE0D4  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800E1198 000DE0D8  4C C6 31 82 */	crclr 6
/* 800E119C 000DE0DC  4B FF BF 09 */	bl SpcTrace__FPCce
/* 800E11A0 000DE0E0  80 1B 00 1C */	lwz r0, 0x1c(r27)
/* 800E11A4 000DE0E4  80 7B 00 20 */	lwz r3, 0x20(r27)
/* 800E11A8 000DE0E8  54 00 18 38 */	slwi r0, r0, 3
/* 800E11AC 000DE0EC  7C 63 02 14 */	add r3, r3, r0
/* 800E11B0 000DE0F0  80 03 00 00 */	lwz r0, 0(r3)
/* 800E11B4 000DE0F4  90 01 00 8C */	stw r0, 0x8c(r1)
/* 800E11B8 000DE0F8  80 03 00 04 */	lwz r0, 4(r3)
/* 800E11BC 000DE0FC  90 01 00 90 */	stw r0, 0x90(r1)
/* 800E11C0 000DE100  48 00 00 2C */	b lbl_800E11EC
lbl_800E11C4:
/* 800E11C4 000DE104  38 03 FF FF */	addi r0, r3, -1
/* 800E11C8 000DE108  90 1B 00 1C */	stw r0, 0x1c(r27)
/* 800E11CC 000DE10C  80 1B 00 1C */	lwz r0, 0x1c(r27)
/* 800E11D0 000DE110  80 7B 00 20 */	lwz r3, 0x20(r27)
/* 800E11D4 000DE114  54 00 18 38 */	slwi r0, r0, 3
/* 800E11D8 000DE118  7C 63 02 14 */	add r3, r3, r0
/* 800E11DC 000DE11C  80 03 00 00 */	lwz r0, 0(r3)
/* 800E11E0 000DE120  90 01 00 8C */	stw r0, 0x8c(r1)
/* 800E11E4 000DE124  80 03 00 04 */	lwz r0, 4(r3)
/* 800E11E8 000DE128  90 01 00 90 */	stw r0, 0x90(r1)
lbl_800E11EC:
/* 800E11EC 000DE12C  80 01 00 8C */	lwz r0, 0x8c(r1)
/* 800E11F0 000DE130  2C 00 00 02 */	cmpwi r0, 2
/* 800E11F4 000DE134  41 82 00 08 */	beq lbl_800E11FC
/* 800E11F8 000DE138  48 00 00 0C */	b lbl_800E1204
lbl_800E11FC:
/* 800E11FC 000DE13C  80 01 00 90 */	lwz r0, 0x90(r1)
/* 800E1200 000DE140  48 00 00 08 */	b lbl_800E1208
lbl_800E1204:
/* 800E1204 000DE144  38 02 8F F0 */	addi r0, r2, $$24053@sda21
lbl_800E1208:
/* 800E1208 000DE148  80 61 00 A4 */	lwz r3, 0xa4(r1)
/* 800E120C 000DE14C  7C 1C 03 78 */	mr r28, r0
/* 800E1210 000DE150  80 01 00 A8 */	lwz r0, 0xa8(r1)
/* 800E1214 000DE154  3B C0 00 00 */	li r30, 0
/* 800E1218 000DE158  90 61 00 84 */	stw r3, 0x84(r1)
/* 800E121C 000DE15C  90 01 00 88 */	stw r0, 0x88(r1)
/* 800E1220 000DE160  80 01 00 84 */	lwz r0, 0x84(r1)
/* 800E1224 000DE164  2C 00 00 01 */	cmpwi r0, 1
/* 800E1228 000DE168  41 82 00 A0 */	beq lbl_800E12C8
/* 800E122C 000DE16C  40 80 00 10 */	bge lbl_800E123C
/* 800E1230 000DE170  2C 00 00 00 */	cmpwi r0, 0
/* 800E1234 000DE174  40 80 00 5C */	bge lbl_800E1290
/* 800E1238 000DE178  48 00 00 90 */	b lbl_800E12C8
lbl_800E123C:
/* 800E123C 000DE17C  2C 00 00 03 */	cmpwi r0, 3
/* 800E1240 000DE180  40 80 00 88 */	bge lbl_800E12C8
/* 800E1244 000DE184  2C 00 00 02 */	cmpwi r0, 2
/* 800E1248 000DE188  41 82 00 08 */	beq lbl_800E1250
/* 800E124C 000DE18C  48 00 00 0C */	b lbl_800E1258
lbl_800E1250:
/* 800E1250 000DE190  83 C1 00 88 */	lwz r30, 0x88(r1)
/* 800E1254 000DE194  48 00 00 08 */	b lbl_800E125C
lbl_800E1258:
/* 800E1258 000DE198  3B C2 8F F0 */	addi r30, r2, $$24053@sda21
lbl_800E125C:
/* 800E125C 000DE19C  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 800E1260 000DE1A0  7F C3 F3 78 */	mr r3, r30
/* 800E1264 000DE1A4  83 44 00 04 */	lwz r26, 4(r4)
/* 800E1268 000DE1A8  4B F6 34 25 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800E126C 000DE1AC  81 9A 00 00 */	lwz r12, 0(r26)
/* 800E1270 000DE1B0  38 83 00 00 */	addi r4, r3, 0
/* 800E1274 000DE1B4  38 7A 00 00 */	addi r3, r26, 0
/* 800E1278 000DE1B8  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 800E127C 000DE1BC  7F C5 F3 78 */	mr r5, r30
/* 800E1280 000DE1C0  7D 88 03 A6 */	mtlr r12
/* 800E1284 000DE1C4  4E 80 00 21 */	blrl 
/* 800E1288 000DE1C8  7C 7E 1B 78 */	mr r30, r3
/* 800E128C 000DE1CC  48 00 00 3C */	b lbl_800E12C8
lbl_800E1290:
/* 800E1290 000DE1D0  2C 00 00 01 */	cmpwi r0, 1
/* 800E1294 000DE1D4  41 82 00 1C */	beq lbl_800E12B0
/* 800E1298 000DE1D8  40 80 00 2C */	bge lbl_800E12C4
/* 800E129C 000DE1DC  2C 00 00 00 */	cmpwi r0, 0
/* 800E12A0 000DE1E0  40 80 00 08 */	bge lbl_800E12A8
/* 800E12A4 000DE1E4  48 00 00 20 */	b lbl_800E12C4
lbl_800E12A8:
/* 800E12A8 000DE1E8  83 C1 00 88 */	lwz r30, 0x88(r1)
/* 800E12AC 000DE1EC  48 00 00 1C */	b lbl_800E12C8
lbl_800E12B0:
/* 800E12B0 000DE1F0  C0 01 00 88 */	lfs f0, 0x88(r1)
/* 800E12B4 000DE1F4  FC 00 00 1E */	fctiwz f0, f0
/* 800E12B8 000DE1F8  D8 01 00 B0 */	stfd f0, 0xb0(r1)
/* 800E12BC 000DE1FC  83 C1 00 B4 */	lwz r30, 0xb4(r1)
/* 800E12C0 000DE200  48 00 00 08 */	b lbl_800E12C8
lbl_800E12C4:
/* 800E12C4 000DE204  3B C0 00 00 */	li r30, 0
lbl_800E12C8:
/* 800E12C8 000DE208  38 7D 00 00 */	addi r3, r29, 0
/* 800E12CC 000DE20C  38 82 8F F0 */	addi r4, r2, $$24053@sda21
/* 800E12D0 000DE210  4B FA 49 8D */	bl strcmp
/* 800E12D4 000DE214  2C 03 00 00 */	cmpwi r3, 0
/* 800E12D8 000DE218  41 82 00 24 */	beq lbl_800E12FC
/* 800E12DC 000DE21C  80 6D 9A 18 */	lwz r3, gpItemManager@sda21(r13)
/* 800E12E0 000DE220  7F 84 E3 78 */	mr r4, r28
/* 800E12E4 000DE224  C0 3E 00 10 */	lfs f1, 0x10(r30)
/* 800E12E8 000DE228  7F A5 EB 78 */	mr r5, r29
/* 800E12EC 000DE22C  C0 5E 00 14 */	lfs f2, 0x14(r30)
/* 800E12F0 000DE230  C0 7E 00 18 */	lfs f3, 0x18(r30)
/* 800E12F4 000DE234  48 0B 6C DD */	bl makeShineAppearWithDemo__12TItemManagerFPCcPCcfff
/* 800E12F8 000DE238  48 00 00 74 */	b lbl_800E136C
lbl_800E12FC:
/* 800E12FC 000DE23C  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 800E1300 000DE240  7F 83 E3 78 */	mr r3, r28
/* 800E1304 000DE244  83 A4 00 04 */	lwz r29, 4(r4)
/* 800E1308 000DE248  4B F6 33 85 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800E130C 000DE24C  81 9D 00 00 */	lwz r12, 0(r29)
/* 800E1310 000DE250  38 83 00 00 */	addi r4, r3, 0
/* 800E1314 000DE254  38 7D 00 00 */	addi r3, r29, 0
/* 800E1318 000DE258  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 800E131C 000DE25C  7F 85 E3 78 */	mr r5, r28
/* 800E1320 000DE260  7D 88 03 A6 */	mtlr r12
/* 800E1324 000DE264  4E 80 00 21 */	blrl 
/* 800E1328 000DE268  80 9E 00 10 */	lwz r4, 0x10(r30)
/* 800E132C 000DE26C  38 A0 FF FF */	li r5, -1
/* 800E1330 000DE270  80 1E 00 14 */	lwz r0, 0x14(r30)
/* 800E1334 000DE274  38 C0 FF FF */	li r6, -1
/* 800E1338 000DE278  38 E0 FF FF */	li r7, -1
/* 800E133C 000DE27C  90 83 01 0C */	stw r4, 0x10c(r3)
/* 800E1340 000DE280  38 80 04 B0 */	li r4, 0x4b0
/* 800E1344 000DE284  90 03 01 10 */	stw r0, 0x110(r3)
/* 800E1348 000DE288  80 1E 00 18 */	lwz r0, 0x18(r30)
/* 800E134C 000DE28C  90 03 01 14 */	stw r0, 0x114(r3)
/* 800E1350 000DE290  81 1E 00 10 */	lwz r8, 0x10(r30)
/* 800E1354 000DE294  80 1E 00 14 */	lwz r0, 0x14(r30)
/* 800E1358 000DE298  91 03 00 10 */	stw r8, 0x10(r3)
/* 800E135C 000DE29C  90 03 00 14 */	stw r0, 0x14(r3)
/* 800E1360 000DE2A0  80 1E 00 18 */	lwz r0, 0x18(r30)
/* 800E1364 000DE2A4  90 03 00 18 */	stw r0, 0x18(r3)
/* 800E1368 000DE2A8  48 0B 3D 95 */	bl appearWithTime__6TShineFiiii
lbl_800E136C:
/* 800E136C 000DE2AC  38 00 00 00 */	li r0, 0
/* 800E1370 000DE2B0  90 01 00 6C */	stw r0, 0x6c(r1)
/* 800E1374 000DE2B4  90 01 00 70 */	stw r0, 0x70(r1)
/* 800E1378 000DE2B8  80 7B 00 1C */	lwz r3, 0x1c(r27)
/* 800E137C 000DE2BC  80 1B 00 18 */	lwz r0, 0x18(r27)
/* 800E1380 000DE2C0  7C 03 00 00 */	cmpw r3, r0
/* 800E1384 000DE2C4  41 80 00 14 */	blt lbl_800E1398
/* 800E1388 000DE2C8  38 7F 07 8C */	addi r3, r31, 0x78c
/* 800E138C 000DE2CC  4C C6 31 82 */	crclr 6
/* 800E1390 000DE2D0  4B FF BD 15 */	bl SpcTrace__FPCce
/* 800E1394 000DE2D4  48 00 00 2C */	b lbl_800E13C0
lbl_800E1398:
/* 800E1398 000DE2D8  80 BB 00 20 */	lwz r5, 0x20(r27)
/* 800E139C 000DE2DC  54 64 18 38 */	slwi r4, r3, 3
/* 800E13A0 000DE2E0  80 61 00 6C */	lwz r3, 0x6c(r1)
/* 800E13A4 000DE2E4  80 01 00 70 */	lwz r0, 0x70(r1)
/* 800E13A8 000DE2E8  7C 85 22 14 */	add r4, r5, r4
/* 800E13AC 000DE2EC  90 64 00 00 */	stw r3, 0(r4)
/* 800E13B0 000DE2F0  90 04 00 04 */	stw r0, 4(r4)
/* 800E13B4 000DE2F4  80 7B 00 1C */	lwz r3, 0x1c(r27)
/* 800E13B8 000DE2F8  38 03 00 01 */	addi r0, r3, 1
/* 800E13BC 000DE2FC  90 1B 00 1C */	stw r0, 0x1c(r27)
lbl_800E13C0:
/* 800E13C0 000DE300  BB 41 00 B8 */	lmw r26, 0xb8(r1)
/* 800E13C4 000DE304  80 01 00 D4 */	lwz r0, 0xd4(r1)
/* 800E13C8 000DE308  38 21 00 D0 */	addi r1, r1, 0xd0
/* 800E13CC 000DE30C  7C 08 03 A6 */	mtlr r0
/* 800E13D0 000DE310  4E 80 00 20 */	blr 

.global evAppearMushroom1up__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evAppearMushroom1up__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E13D4 000DE314  7C 08 02 A6 */	mflr r0
/* 800E13D8 000DE318  90 01 00 04 */	stw r0, 4(r1)
/* 800E13DC 000DE31C  94 21 FF 78 */	stwu r1, -0x88(r1)
/* 800E13E0 000DE320  93 E1 00 84 */	stw r31, 0x84(r1)
/* 800E13E4 000DE324  7C 7F 1B 78 */	mr r31, r3
/* 800E13E8 000DE328  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E13EC 000DE32C  93 C1 00 80 */	stw r30, 0x80(r1)
/* 800E13F0 000DE330  93 A1 00 7C */	stw r29, 0x7c(r1)
/* 800E13F4 000DE334  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E13F8 000DE338  38 80 00 01 */	li r4, 1
/* 800E13FC 000DE33C  48 03 60 35 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E1400 000DE340  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E1404 000DE344  2C 03 00 00 */	cmpwi r3, 0
/* 800E1408 000DE348  41 81 00 38 */	bgt lbl_800E1440
/* 800E140C 000DE34C  3C 60 80 37 */	lis r3, $$23829@ha
/* 800E1410 000DE350  4C C6 31 82 */	crclr 6
/* 800E1414 000DE354  38 63 30 FC */	addi r3, r3, $$23829@l
/* 800E1418 000DE358  4B FF BC 8D */	bl SpcTrace__FPCce
/* 800E141C 000DE35C  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800E1420 000DE360  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800E1424 000DE364  54 00 18 38 */	slwi r0, r0, 3
/* 800E1428 000DE368  7C 63 02 14 */	add r3, r3, r0
/* 800E142C 000DE36C  80 03 00 00 */	lwz r0, 0(r3)
/* 800E1430 000DE370  90 01 00 5C */	stw r0, 0x5c(r1)
/* 800E1434 000DE374  80 03 00 04 */	lwz r0, 4(r3)
/* 800E1438 000DE378  90 01 00 60 */	stw r0, 0x60(r1)
/* 800E143C 000DE37C  48 00 00 2C */	b lbl_800E1468
lbl_800E1440:
/* 800E1440 000DE380  38 03 FF FF */	addi r0, r3, -1
/* 800E1444 000DE384  90 1F 00 1C */	stw r0, 0x1c(r31)
/* 800E1448 000DE388  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800E144C 000DE38C  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800E1450 000DE390  54 00 18 38 */	slwi r0, r0, 3
/* 800E1454 000DE394  7C 63 02 14 */	add r3, r3, r0
/* 800E1458 000DE398  80 03 00 00 */	lwz r0, 0(r3)
/* 800E145C 000DE39C  90 01 00 5C */	stw r0, 0x5c(r1)
/* 800E1460 000DE3A0  80 03 00 04 */	lwz r0, 4(r3)
/* 800E1464 000DE3A4  90 01 00 60 */	stw r0, 0x60(r1)
lbl_800E1468:
/* 800E1468 000DE3A8  80 81 00 5C */	lwz r4, 0x5c(r1)
/* 800E146C 000DE3AC  38 60 00 00 */	li r3, 0
/* 800E1470 000DE3B0  80 01 00 60 */	lwz r0, 0x60(r1)
/* 800E1474 000DE3B4  90 81 00 64 */	stw r4, 0x64(r1)
/* 800E1478 000DE3B8  90 01 00 68 */	stw r0, 0x68(r1)
/* 800E147C 000DE3BC  80 01 00 64 */	lwz r0, 0x64(r1)
/* 800E1480 000DE3C0  2C 00 00 01 */	cmpwi r0, 1
/* 800E1484 000DE3C4  41 82 00 9C */	beq lbl_800E1520
/* 800E1488 000DE3C8  40 80 00 10 */	bge lbl_800E1498
/* 800E148C 000DE3CC  2C 00 00 00 */	cmpwi r0, 0
/* 800E1490 000DE3D0  40 80 00 58 */	bge lbl_800E14E8
/* 800E1494 000DE3D4  48 00 00 8C */	b lbl_800E1520
lbl_800E1498:
/* 800E1498 000DE3D8  2C 00 00 03 */	cmpwi r0, 3
/* 800E149C 000DE3DC  40 80 00 84 */	bge lbl_800E1520
/* 800E14A0 000DE3E0  2C 00 00 02 */	cmpwi r0, 2
/* 800E14A4 000DE3E4  41 82 00 08 */	beq lbl_800E14AC
/* 800E14A8 000DE3E8  48 00 00 0C */	b lbl_800E14B4
lbl_800E14AC:
/* 800E14AC 000DE3EC  83 C1 00 68 */	lwz r30, 0x68(r1)
/* 800E14B0 000DE3F0  48 00 00 08 */	b lbl_800E14B8
lbl_800E14B4:
/* 800E14B4 000DE3F4  3B C2 8F F0 */	addi r30, r2, $$24053@sda21
lbl_800E14B8:
/* 800E14B8 000DE3F8  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 800E14BC 000DE3FC  7F C3 F3 78 */	mr r3, r30
/* 800E14C0 000DE400  83 A4 00 04 */	lwz r29, 4(r4)
/* 800E14C4 000DE404  4B F6 31 C9 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800E14C8 000DE408  81 9D 00 00 */	lwz r12, 0(r29)
/* 800E14CC 000DE40C  38 83 00 00 */	addi r4, r3, 0
/* 800E14D0 000DE410  38 7D 00 00 */	addi r3, r29, 0
/* 800E14D4 000DE414  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 800E14D8 000DE418  7F C5 F3 78 */	mr r5, r30
/* 800E14DC 000DE41C  7D 88 03 A6 */	mtlr r12
/* 800E14E0 000DE420  4E 80 00 21 */	blrl 
/* 800E14E4 000DE424  48 00 00 3C */	b lbl_800E1520
lbl_800E14E8:
/* 800E14E8 000DE428  2C 00 00 01 */	cmpwi r0, 1
/* 800E14EC 000DE42C  41 82 00 1C */	beq lbl_800E1508
/* 800E14F0 000DE430  40 80 00 2C */	bge lbl_800E151C
/* 800E14F4 000DE434  2C 00 00 00 */	cmpwi r0, 0
/* 800E14F8 000DE438  40 80 00 08 */	bge lbl_800E1500
/* 800E14FC 000DE43C  48 00 00 20 */	b lbl_800E151C
lbl_800E1500:
/* 800E1500 000DE440  80 61 00 68 */	lwz r3, 0x68(r1)
/* 800E1504 000DE444  48 00 00 1C */	b lbl_800E1520
lbl_800E1508:
/* 800E1508 000DE448  C0 01 00 68 */	lfs f0, 0x68(r1)
/* 800E150C 000DE44C  FC 00 00 1E */	fctiwz f0, f0
/* 800E1510 000DE450  D8 01 00 70 */	stfd f0, 0x70(r1)
/* 800E1514 000DE454  80 61 00 74 */	lwz r3, 0x74(r1)
/* 800E1518 000DE458  48 00 00 08 */	b lbl_800E1520
lbl_800E151C:
/* 800E151C 000DE45C  38 60 00 00 */	li r3, 0
lbl_800E1520:
/* 800E1520 000DE460  81 83 00 00 */	lwz r12, 0(r3)
/* 800E1524 000DE464  81 8C 00 FC */	lwz r12, 0xfc(r12)
/* 800E1528 000DE468  7D 88 03 A6 */	mtlr r12
/* 800E152C 000DE46C  4E 80 00 21 */	blrl 
/* 800E1530 000DE470  80 6D 97 EC */	lwz r3, gpMSound@sda21(r13)
/* 800E1534 000DE474  38 80 48 54 */	li r4, 0x4854
/* 800E1538 000DE478  48 09 B5 19 */	bl gateCheck__6MSoundFUl
/* 800E153C 000DE47C  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 800E1540 000DE480  41 82 00 18 */	beq lbl_800E1558
/* 800E1544 000DE484  38 60 48 54 */	li r3, 0x4854
/* 800E1548 000DE488  38 80 00 00 */	li r4, 0
/* 800E154C 000DE48C  38 A0 00 00 */	li r5, 0
/* 800E1550 000DE490  38 C0 00 00 */	li r6, 0
/* 800E1554 000DE494  48 0A 08 35 */	bl startSoundSystemSE__Q214MSoundSESystem8MSoundSEFUlUlPP8JAISoundUl
lbl_800E1558:
/* 800E1558 000DE498  38 00 00 00 */	li r0, 0
/* 800E155C 000DE49C  90 01 00 4C */	stw r0, 0x4c(r1)
/* 800E1560 000DE4A0  90 01 00 50 */	stw r0, 0x50(r1)
/* 800E1564 000DE4A4  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E1568 000DE4A8  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800E156C 000DE4AC  7C 03 00 00 */	cmpw r3, r0
/* 800E1570 000DE4B0  41 80 00 18 */	blt lbl_800E1588
/* 800E1574 000DE4B4  3C 60 80 37 */	lis r3, $$23834@ha
/* 800E1578 000DE4B8  4C C6 31 82 */	crclr 6
/* 800E157C 000DE4BC  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800E1580 000DE4C0  4B FF BB 25 */	bl SpcTrace__FPCce
/* 800E1584 000DE4C4  48 00 00 2C */	b lbl_800E15B0
lbl_800E1588:
/* 800E1588 000DE4C8  80 BF 00 20 */	lwz r5, 0x20(r31)
/* 800E158C 000DE4CC  54 64 18 38 */	slwi r4, r3, 3
/* 800E1590 000DE4D0  80 61 00 4C */	lwz r3, 0x4c(r1)
/* 800E1594 000DE4D4  80 01 00 50 */	lwz r0, 0x50(r1)
/* 800E1598 000DE4D8  7C 85 22 14 */	add r4, r5, r4
/* 800E159C 000DE4DC  90 64 00 00 */	stw r3, 0(r4)
/* 800E15A0 000DE4E0  90 04 00 04 */	stw r0, 4(r4)
/* 800E15A4 000DE4E4  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E15A8 000DE4E8  38 03 00 01 */	addi r0, r3, 1
/* 800E15AC 000DE4EC  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800E15B0:
/* 800E15B0 000DE4F0  80 01 00 8C */	lwz r0, 0x8c(r1)
/* 800E15B4 000DE4F4  83 E1 00 84 */	lwz r31, 0x84(r1)
/* 800E15B8 000DE4F8  83 C1 00 80 */	lwz r30, 0x80(r1)
/* 800E15BC 000DE4FC  7C 08 03 A6 */	mtlr r0
/* 800E15C0 000DE500  83 A1 00 7C */	lwz r29, 0x7c(r1)
/* 800E15C4 000DE504  38 21 00 88 */	addi r1, r1, 0x88
/* 800E15C8 000DE508  4E 80 00 20 */	blr 

.global appear__11TMapObjBaseFv
appear__11TMapObjBaseFv:
/* 800E15CC 000DE50C  7C 08 02 A6 */	mflr r0
/* 800E15D0 000DE510  90 01 00 04 */	stw r0, 4(r1)
/* 800E15D4 000DE514  94 21 FF F8 */	stwu r1, -8(r1)
/* 800E15D8 000DE518  81 83 00 00 */	lwz r12, 0(r3)
/* 800E15DC 000DE51C  81 8C 01 00 */	lwz r12, 0x100(r12)
/* 800E15E0 000DE520  7D 88 03 A6 */	mtlr r12
/* 800E15E4 000DE524  4E 80 00 21 */	blrl 
/* 800E15E8 000DE528  80 01 00 0C */	lwz r0, 0xc(r1)
/* 800E15EC 000DE52C  38 21 00 08 */	addi r1, r1, 8
/* 800E15F0 000DE530  7C 08 03 A6 */	mtlr r0
/* 800E15F4 000DE534  4E 80 00 20 */	blr 

.global evKillMushroom1up__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evKillMushroom1up__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E15F8 000DE538  7C 08 02 A6 */	mflr r0
/* 800E15FC 000DE53C  90 01 00 04 */	stw r0, 4(r1)
/* 800E1600 000DE540  94 21 FF 80 */	stwu r1, -0x80(r1)
/* 800E1604 000DE544  93 E1 00 7C */	stw r31, 0x7c(r1)
/* 800E1608 000DE548  7C 7F 1B 78 */	mr r31, r3
/* 800E160C 000DE54C  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E1610 000DE550  93 C1 00 78 */	stw r30, 0x78(r1)
/* 800E1614 000DE554  93 A1 00 74 */	stw r29, 0x74(r1)
/* 800E1618 000DE558  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E161C 000DE55C  38 80 00 01 */	li r4, 1
/* 800E1620 000DE560  48 03 5E 11 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E1624 000DE564  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E1628 000DE568  2C 03 00 00 */	cmpwi r3, 0
/* 800E162C 000DE56C  41 81 00 38 */	bgt lbl_800E1664
/* 800E1630 000DE570  3C 60 80 37 */	lis r3, $$23829@ha
/* 800E1634 000DE574  4C C6 31 82 */	crclr 6
/* 800E1638 000DE578  38 63 30 FC */	addi r3, r3, $$23829@l
/* 800E163C 000DE57C  4B FF BA 69 */	bl SpcTrace__FPCce
/* 800E1640 000DE580  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800E1644 000DE584  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800E1648 000DE588  54 00 18 38 */	slwi r0, r0, 3
/* 800E164C 000DE58C  7C 63 02 14 */	add r3, r3, r0
/* 800E1650 000DE590  80 03 00 00 */	lwz r0, 0(r3)
/* 800E1654 000DE594  90 01 00 54 */	stw r0, 0x54(r1)
/* 800E1658 000DE598  80 03 00 04 */	lwz r0, 4(r3)
/* 800E165C 000DE59C  90 01 00 58 */	stw r0, 0x58(r1)
/* 800E1660 000DE5A0  48 00 00 2C */	b lbl_800E168C
lbl_800E1664:
/* 800E1664 000DE5A4  38 03 FF FF */	addi r0, r3, -1
/* 800E1668 000DE5A8  90 1F 00 1C */	stw r0, 0x1c(r31)
/* 800E166C 000DE5AC  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800E1670 000DE5B0  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800E1674 000DE5B4  54 00 18 38 */	slwi r0, r0, 3
/* 800E1678 000DE5B8  7C 63 02 14 */	add r3, r3, r0
/* 800E167C 000DE5BC  80 03 00 00 */	lwz r0, 0(r3)
/* 800E1680 000DE5C0  90 01 00 54 */	stw r0, 0x54(r1)
/* 800E1684 000DE5C4  80 03 00 04 */	lwz r0, 4(r3)
/* 800E1688 000DE5C8  90 01 00 58 */	stw r0, 0x58(r1)
lbl_800E168C:
/* 800E168C 000DE5CC  80 81 00 54 */	lwz r4, 0x54(r1)
/* 800E1690 000DE5D0  38 60 00 00 */	li r3, 0
/* 800E1694 000DE5D4  80 01 00 58 */	lwz r0, 0x58(r1)
/* 800E1698 000DE5D8  90 81 00 5C */	stw r4, 0x5c(r1)
/* 800E169C 000DE5DC  90 01 00 60 */	stw r0, 0x60(r1)
/* 800E16A0 000DE5E0  80 01 00 5C */	lwz r0, 0x5c(r1)
/* 800E16A4 000DE5E4  2C 00 00 01 */	cmpwi r0, 1
/* 800E16A8 000DE5E8  41 82 00 9C */	beq lbl_800E1744
/* 800E16AC 000DE5EC  40 80 00 10 */	bge lbl_800E16BC
/* 800E16B0 000DE5F0  2C 00 00 00 */	cmpwi r0, 0
/* 800E16B4 000DE5F4  40 80 00 58 */	bge lbl_800E170C
/* 800E16B8 000DE5F8  48 00 00 8C */	b lbl_800E1744
lbl_800E16BC:
/* 800E16BC 000DE5FC  2C 00 00 03 */	cmpwi r0, 3
/* 800E16C0 000DE600  40 80 00 84 */	bge lbl_800E1744
/* 800E16C4 000DE604  2C 00 00 02 */	cmpwi r0, 2
/* 800E16C8 000DE608  41 82 00 08 */	beq lbl_800E16D0
/* 800E16CC 000DE60C  48 00 00 0C */	b lbl_800E16D8
lbl_800E16D0:
/* 800E16D0 000DE610  83 C1 00 60 */	lwz r30, 0x60(r1)
/* 800E16D4 000DE614  48 00 00 08 */	b lbl_800E16DC
lbl_800E16D8:
/* 800E16D8 000DE618  3B C2 8F F0 */	addi r30, r2, $$24053@sda21
lbl_800E16DC:
/* 800E16DC 000DE61C  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 800E16E0 000DE620  7F C3 F3 78 */	mr r3, r30
/* 800E16E4 000DE624  83 A4 00 04 */	lwz r29, 4(r4)
/* 800E16E8 000DE628  4B F6 2F A5 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800E16EC 000DE62C  81 9D 00 00 */	lwz r12, 0(r29)
/* 800E16F0 000DE630  38 83 00 00 */	addi r4, r3, 0
/* 800E16F4 000DE634  38 7D 00 00 */	addi r3, r29, 0
/* 800E16F8 000DE638  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 800E16FC 000DE63C  7F C5 F3 78 */	mr r5, r30
/* 800E1700 000DE640  7D 88 03 A6 */	mtlr r12
/* 800E1704 000DE644  4E 80 00 21 */	blrl 
/* 800E1708 000DE648  48 00 00 3C */	b lbl_800E1744
lbl_800E170C:
/* 800E170C 000DE64C  2C 00 00 01 */	cmpwi r0, 1
/* 800E1710 000DE650  41 82 00 1C */	beq lbl_800E172C
/* 800E1714 000DE654  40 80 00 2C */	bge lbl_800E1740
/* 800E1718 000DE658  2C 00 00 00 */	cmpwi r0, 0
/* 800E171C 000DE65C  40 80 00 08 */	bge lbl_800E1724
/* 800E1720 000DE660  48 00 00 20 */	b lbl_800E1740
lbl_800E1724:
/* 800E1724 000DE664  80 61 00 60 */	lwz r3, 0x60(r1)
/* 800E1728 000DE668  48 00 00 1C */	b lbl_800E1744
lbl_800E172C:
/* 800E172C 000DE66C  C0 01 00 60 */	lfs f0, 0x60(r1)
/* 800E1730 000DE670  FC 00 00 1E */	fctiwz f0, f0
/* 800E1734 000DE674  D8 01 00 68 */	stfd f0, 0x68(r1)
/* 800E1738 000DE678  80 61 00 6C */	lwz r3, 0x6c(r1)
/* 800E173C 000DE67C  48 00 00 08 */	b lbl_800E1744
lbl_800E1740:
/* 800E1740 000DE680  38 60 00 00 */	li r3, 0
lbl_800E1744:
/* 800E1744 000DE684  81 83 00 00 */	lwz r12, 0(r3)
/* 800E1748 000DE688  81 8C 00 E4 */	lwz r12, 0xe4(r12)
/* 800E174C 000DE68C  7D 88 03 A6 */	mtlr r12
/* 800E1750 000DE690  4E 80 00 21 */	blrl 
/* 800E1754 000DE694  38 00 00 00 */	li r0, 0
/* 800E1758 000DE698  90 01 00 44 */	stw r0, 0x44(r1)
/* 800E175C 000DE69C  90 01 00 48 */	stw r0, 0x48(r1)
/* 800E1760 000DE6A0  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E1764 000DE6A4  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800E1768 000DE6A8  7C 03 00 00 */	cmpw r3, r0
/* 800E176C 000DE6AC  41 80 00 18 */	blt lbl_800E1784
/* 800E1770 000DE6B0  3C 60 80 37 */	lis r3, $$23834@ha
/* 800E1774 000DE6B4  4C C6 31 82 */	crclr 6
/* 800E1778 000DE6B8  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800E177C 000DE6BC  4B FF B9 29 */	bl SpcTrace__FPCce
/* 800E1780 000DE6C0  48 00 00 2C */	b lbl_800E17AC
lbl_800E1784:
/* 800E1784 000DE6C4  80 BF 00 20 */	lwz r5, 0x20(r31)
/* 800E1788 000DE6C8  54 64 18 38 */	slwi r4, r3, 3
/* 800E178C 000DE6CC  80 61 00 44 */	lwz r3, 0x44(r1)
/* 800E1790 000DE6D0  80 01 00 48 */	lwz r0, 0x48(r1)
/* 800E1794 000DE6D4  7C 85 22 14 */	add r4, r5, r4
/* 800E1798 000DE6D8  90 64 00 00 */	stw r3, 0(r4)
/* 800E179C 000DE6DC  90 04 00 04 */	stw r0, 4(r4)
/* 800E17A0 000DE6E0  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E17A4 000DE6E4  38 03 00 01 */	addi r0, r3, 1
/* 800E17A8 000DE6E8  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800E17AC:
/* 800E17AC 000DE6EC  80 01 00 84 */	lwz r0, 0x84(r1)
/* 800E17B0 000DE6F0  83 E1 00 7C */	lwz r31, 0x7c(r1)
/* 800E17B4 000DE6F4  83 C1 00 78 */	lwz r30, 0x78(r1)
/* 800E17B8 000DE6F8  7C 08 03 A6 */	mtlr r0
/* 800E17BC 000DE6FC  83 A1 00 74 */	lwz r29, 0x74(r1)
/* 800E17C0 000DE700  38 21 00 80 */	addi r1, r1, 0x80
/* 800E17C4 000DE704  4E 80 00 20 */	blr 

.global kill__11TMapObjBaseFv
kill__11TMapObjBaseFv:
/* 800E17C8 000DE708  7C 08 02 A6 */	mflr r0
/* 800E17CC 000DE70C  90 01 00 04 */	stw r0, 4(r1)
/* 800E17D0 000DE710  94 21 FF F8 */	stwu r1, -8(r1)
/* 800E17D4 000DE714  81 83 00 00 */	lwz r12, 0(r3)
/* 800E17D8 000DE718  81 8C 01 04 */	lwz r12, 0x104(r12)
/* 800E17DC 000DE71C  7D 88 03 A6 */	mtlr r12
/* 800E17E0 000DE720  4E 80 00 21 */	blrl 
/* 800E17E4 000DE724  80 01 00 0C */	lwz r0, 0xc(r1)
/* 800E17E8 000DE728  38 21 00 08 */	addi r1, r1, 8
/* 800E17EC 000DE72C  7C 08 03 A6 */	mtlr r0
/* 800E17F0 000DE730  4E 80 00 20 */	blr 

.global evKillShine__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evKillShine__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E17F4 000DE734  7C 08 02 A6 */	mflr r0
/* 800E17F8 000DE738  90 01 00 04 */	stw r0, 4(r1)
/* 800E17FC 000DE73C  94 21 FF 80 */	stwu r1, -0x80(r1)
/* 800E1800 000DE740  93 E1 00 7C */	stw r31, 0x7c(r1)
/* 800E1804 000DE744  7C 7F 1B 78 */	mr r31, r3
/* 800E1808 000DE748  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E180C 000DE74C  93 C1 00 78 */	stw r30, 0x78(r1)
/* 800E1810 000DE750  93 A1 00 74 */	stw r29, 0x74(r1)
/* 800E1814 000DE754  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E1818 000DE758  38 80 00 01 */	li r4, 1
/* 800E181C 000DE75C  48 03 5C 15 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E1820 000DE760  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E1824 000DE764  2C 03 00 00 */	cmpwi r3, 0
/* 800E1828 000DE768  41 81 00 38 */	bgt lbl_800E1860
/* 800E182C 000DE76C  3C 60 80 37 */	lis r3, $$23829@ha
/* 800E1830 000DE770  4C C6 31 82 */	crclr 6
/* 800E1834 000DE774  38 63 30 FC */	addi r3, r3, $$23829@l
/* 800E1838 000DE778  4B FF B8 6D */	bl SpcTrace__FPCce
/* 800E183C 000DE77C  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800E1840 000DE780  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800E1844 000DE784  54 00 18 38 */	slwi r0, r0, 3
/* 800E1848 000DE788  7C 63 02 14 */	add r3, r3, r0
/* 800E184C 000DE78C  80 03 00 00 */	lwz r0, 0(r3)
/* 800E1850 000DE790  90 01 00 54 */	stw r0, 0x54(r1)
/* 800E1854 000DE794  80 03 00 04 */	lwz r0, 4(r3)
/* 800E1858 000DE798  90 01 00 58 */	stw r0, 0x58(r1)
/* 800E185C 000DE79C  48 00 00 2C */	b lbl_800E1888
lbl_800E1860:
/* 800E1860 000DE7A0  38 03 FF FF */	addi r0, r3, -1
/* 800E1864 000DE7A4  90 1F 00 1C */	stw r0, 0x1c(r31)
/* 800E1868 000DE7A8  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800E186C 000DE7AC  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800E1870 000DE7B0  54 00 18 38 */	slwi r0, r0, 3
/* 800E1874 000DE7B4  7C 63 02 14 */	add r3, r3, r0
/* 800E1878 000DE7B8  80 03 00 00 */	lwz r0, 0(r3)
/* 800E187C 000DE7BC  90 01 00 54 */	stw r0, 0x54(r1)
/* 800E1880 000DE7C0  80 03 00 04 */	lwz r0, 4(r3)
/* 800E1884 000DE7C4  90 01 00 58 */	stw r0, 0x58(r1)
lbl_800E1888:
/* 800E1888 000DE7C8  80 81 00 54 */	lwz r4, 0x54(r1)
/* 800E188C 000DE7CC  38 60 00 00 */	li r3, 0
/* 800E1890 000DE7D0  80 01 00 58 */	lwz r0, 0x58(r1)
/* 800E1894 000DE7D4  90 81 00 5C */	stw r4, 0x5c(r1)
/* 800E1898 000DE7D8  90 01 00 60 */	stw r0, 0x60(r1)
/* 800E189C 000DE7DC  80 01 00 5C */	lwz r0, 0x5c(r1)
/* 800E18A0 000DE7E0  2C 00 00 01 */	cmpwi r0, 1
/* 800E18A4 000DE7E4  41 82 00 9C */	beq lbl_800E1940
/* 800E18A8 000DE7E8  40 80 00 10 */	bge lbl_800E18B8
/* 800E18AC 000DE7EC  2C 00 00 00 */	cmpwi r0, 0
/* 800E18B0 000DE7F0  40 80 00 58 */	bge lbl_800E1908
/* 800E18B4 000DE7F4  48 00 00 8C */	b lbl_800E1940
lbl_800E18B8:
/* 800E18B8 000DE7F8  2C 00 00 03 */	cmpwi r0, 3
/* 800E18BC 000DE7FC  40 80 00 84 */	bge lbl_800E1940
/* 800E18C0 000DE800  2C 00 00 02 */	cmpwi r0, 2
/* 800E18C4 000DE804  41 82 00 08 */	beq lbl_800E18CC
/* 800E18C8 000DE808  48 00 00 0C */	b lbl_800E18D4
lbl_800E18CC:
/* 800E18CC 000DE80C  83 C1 00 60 */	lwz r30, 0x60(r1)
/* 800E18D0 000DE810  48 00 00 08 */	b lbl_800E18D8
lbl_800E18D4:
/* 800E18D4 000DE814  3B C2 8F F0 */	addi r30, r2, $$24053@sda21
lbl_800E18D8:
/* 800E18D8 000DE818  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 800E18DC 000DE81C  7F C3 F3 78 */	mr r3, r30
/* 800E18E0 000DE820  83 A4 00 04 */	lwz r29, 4(r4)
/* 800E18E4 000DE824  4B F6 2D A9 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800E18E8 000DE828  81 9D 00 00 */	lwz r12, 0(r29)
/* 800E18EC 000DE82C  38 83 00 00 */	addi r4, r3, 0
/* 800E18F0 000DE830  38 7D 00 00 */	addi r3, r29, 0
/* 800E18F4 000DE834  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 800E18F8 000DE838  7F C5 F3 78 */	mr r5, r30
/* 800E18FC 000DE83C  7D 88 03 A6 */	mtlr r12
/* 800E1900 000DE840  4E 80 00 21 */	blrl 
/* 800E1904 000DE844  48 00 00 3C */	b lbl_800E1940
lbl_800E1908:
/* 800E1908 000DE848  2C 00 00 01 */	cmpwi r0, 1
/* 800E190C 000DE84C  41 82 00 1C */	beq lbl_800E1928
/* 800E1910 000DE850  40 80 00 2C */	bge lbl_800E193C
/* 800E1914 000DE854  2C 00 00 00 */	cmpwi r0, 0
/* 800E1918 000DE858  40 80 00 08 */	bge lbl_800E1920
/* 800E191C 000DE85C  48 00 00 20 */	b lbl_800E193C
lbl_800E1920:
/* 800E1920 000DE860  80 61 00 60 */	lwz r3, 0x60(r1)
/* 800E1924 000DE864  48 00 00 1C */	b lbl_800E1940
lbl_800E1928:
/* 800E1928 000DE868  C0 01 00 60 */	lfs f0, 0x60(r1)
/* 800E192C 000DE86C  FC 00 00 1E */	fctiwz f0, f0
/* 800E1930 000DE870  D8 01 00 68 */	stfd f0, 0x68(r1)
/* 800E1934 000DE874  80 61 00 6C */	lwz r3, 0x6c(r1)
/* 800E1938 000DE878  48 00 00 08 */	b lbl_800E1940
lbl_800E193C:
/* 800E193C 000DE87C  38 60 00 00 */	li r3, 0
lbl_800E1940:
/* 800E1940 000DE880  81 83 00 00 */	lwz r12, 0(r3)
/* 800E1944 000DE884  81 8C 00 E4 */	lwz r12, 0xe4(r12)
/* 800E1948 000DE888  7D 88 03 A6 */	mtlr r12
/* 800E194C 000DE88C  4E 80 00 21 */	blrl 
/* 800E1950 000DE890  38 00 00 00 */	li r0, 0
/* 800E1954 000DE894  90 01 00 44 */	stw r0, 0x44(r1)
/* 800E1958 000DE898  90 01 00 48 */	stw r0, 0x48(r1)
/* 800E195C 000DE89C  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E1960 000DE8A0  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800E1964 000DE8A4  7C 03 00 00 */	cmpw r3, r0
/* 800E1968 000DE8A8  41 80 00 18 */	blt lbl_800E1980
/* 800E196C 000DE8AC  3C 60 80 37 */	lis r3, $$23834@ha
/* 800E1970 000DE8B0  4C C6 31 82 */	crclr 6
/* 800E1974 000DE8B4  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800E1978 000DE8B8  4B FF B7 2D */	bl SpcTrace__FPCce
/* 800E197C 000DE8BC  48 00 00 2C */	b lbl_800E19A8
lbl_800E1980:
/* 800E1980 000DE8C0  80 BF 00 20 */	lwz r5, 0x20(r31)
/* 800E1984 000DE8C4  54 64 18 38 */	slwi r4, r3, 3
/* 800E1988 000DE8C8  80 61 00 44 */	lwz r3, 0x44(r1)
/* 800E198C 000DE8CC  80 01 00 48 */	lwz r0, 0x48(r1)
/* 800E1990 000DE8D0  7C 85 22 14 */	add r4, r5, r4
/* 800E1994 000DE8D4  90 64 00 00 */	stw r3, 0(r4)
/* 800E1998 000DE8D8  90 04 00 04 */	stw r0, 4(r4)
/* 800E199C 000DE8DC  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E19A0 000DE8E0  38 03 00 01 */	addi r0, r3, 1
/* 800E19A4 000DE8E4  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800E19A8:
/* 800E19A8 000DE8E8  80 01 00 84 */	lwz r0, 0x84(r1)
/* 800E19AC 000DE8EC  83 E1 00 7C */	lwz r31, 0x7c(r1)
/* 800E19B0 000DE8F0  83 C1 00 78 */	lwz r30, 0x78(r1)
/* 800E19B4 000DE8F4  7C 08 03 A6 */	mtlr r0
/* 800E19B8 000DE8F8  83 A1 00 74 */	lwz r29, 0x74(r1)
/* 800E19BC 000DE8FC  38 21 00 80 */	addi r1, r1, 0x80
/* 800E19C0 000DE900  4E 80 00 20 */	blr 

.global evGetTime__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evGetTime__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E19C4 000DE904  7C 08 02 A6 */	mflr r0
/* 800E19C8 000DE908  90 01 00 04 */	stw r0, 4(r1)
/* 800E19CC 000DE90C  94 21 FF D0 */	stwu r1, -0x30(r1)
/* 800E19D0 000DE910  93 E1 00 2C */	stw r31, 0x2c(r1)
/* 800E19D4 000DE914  7C 7F 1B 78 */	mr r31, r3
/* 800E19D8 000DE918  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E19DC 000DE91C  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E19E0 000DE920  38 80 00 00 */	li r4, 0
/* 800E19E4 000DE924  48 03 5A 4D */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E19E8 000DE928  80 6D 97 E8 */	lwz r3, gpMarDirector@sda21(r13)
/* 800E19EC 000DE92C  80 63 00 74 */	lwz r3, 0x74(r3)
/* 800E19F0 000DE930  48 12 83 F1 */	bl getFinishedTime__11TGCConsole2Fv
/* 800E19F4 000DE934  38 00 00 00 */	li r0, 0
/* 800E19F8 000DE938  90 01 00 1C */	stw r0, 0x1c(r1)
/* 800E19FC 000DE93C  90 61 00 20 */	stw r3, 0x20(r1)
/* 800E1A00 000DE940  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E1A04 000DE944  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800E1A08 000DE948  7C 03 00 00 */	cmpw r3, r0
/* 800E1A0C 000DE94C  41 80 00 18 */	blt lbl_800E1A24
/* 800E1A10 000DE950  3C 60 80 37 */	lis r3, $$23834@ha
/* 800E1A14 000DE954  4C C6 31 82 */	crclr 6
/* 800E1A18 000DE958  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800E1A1C 000DE95C  4B FF B6 89 */	bl SpcTrace__FPCce
/* 800E1A20 000DE960  48 00 00 2C */	b lbl_800E1A4C
lbl_800E1A24:
/* 800E1A24 000DE964  80 BF 00 20 */	lwz r5, 0x20(r31)
/* 800E1A28 000DE968  54 64 18 38 */	slwi r4, r3, 3
/* 800E1A2C 000DE96C  80 61 00 1C */	lwz r3, 0x1c(r1)
/* 800E1A30 000DE970  80 01 00 20 */	lwz r0, 0x20(r1)
/* 800E1A34 000DE974  7C 85 22 14 */	add r4, r5, r4
/* 800E1A38 000DE978  90 64 00 00 */	stw r3, 0(r4)
/* 800E1A3C 000DE97C  90 04 00 04 */	stw r0, 4(r4)
/* 800E1A40 000DE980  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E1A44 000DE984  38 03 00 01 */	addi r0, r3, 1
/* 800E1A48 000DE988  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800E1A4C:
/* 800E1A4C 000DE98C  80 01 00 34 */	lwz r0, 0x34(r1)
/* 800E1A50 000DE990  83 E1 00 2C */	lwz r31, 0x2c(r1)
/* 800E1A54 000DE994  38 21 00 30 */	addi r1, r1, 0x30
/* 800E1A58 000DE998  7C 08 03 A6 */	mtlr r0
/* 800E1A5C 000DE99C  4E 80 00 20 */	blr 

.global evMonteManReachFlag__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evMonteManReachFlag__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E1A60 000DE9A0  7C 08 02 A6 */	mflr r0
/* 800E1A64 000DE9A4  90 01 00 04 */	stw r0, 4(r1)
/* 800E1A68 000DE9A8  94 21 FF C0 */	stwu r1, -0x40(r1)
/* 800E1A6C 000DE9AC  93 E1 00 3C */	stw r31, 0x3c(r1)
/* 800E1A70 000DE9B0  7C 7F 1B 78 */	mr r31, r3
/* 800E1A74 000DE9B4  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E1A78 000DE9B8  93 C1 00 38 */	stw r30, 0x38(r1)
/* 800E1A7C 000DE9BC  93 A1 00 34 */	stw r29, 0x34(r1)
/* 800E1A80 000DE9C0  93 81 00 30 */	stw r28, 0x30(r1)
/* 800E1A84 000DE9C4  3B 80 00 00 */	li r28, 0
/* 800E1A88 000DE9C8  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E1A8C 000DE9CC  3C 80 80 37 */	lis r4, $$21490@ha
/* 800E1A90 000DE9D0  3B C4 29 90 */	addi r30, r4, $$21490@l
/* 800E1A94 000DE9D4  38 80 00 00 */	li r4, 0
/* 800E1A98 000DE9D8  48 03 59 99 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E1A9C 000DE9DC  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 800E1AA0 000DE9E0  38 7E 08 0C */	addi r3, r30, 0x80c
/* 800E1AA4 000DE9E4  83 A4 00 04 */	lwz r29, 4(r4)
/* 800E1AA8 000DE9E8  4B F6 2B E5 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800E1AAC 000DE9EC  81 9D 00 00 */	lwz r12, 0(r29)
/* 800E1AB0 000DE9F0  38 83 00 00 */	addi r4, r3, 0
/* 800E1AB4 000DE9F4  38 7D 00 00 */	addi r3, r29, 0
/* 800E1AB8 000DE9F8  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 800E1ABC 000DE9FC  38 BE 08 0C */	addi r5, r30, 0x80c
/* 800E1AC0 000DEA00  7D 88 03 A6 */	mtlr r12
/* 800E1AC4 000DEA04  4E 80 00 21 */	blrl 
/* 800E1AC8 000DEA08  48 16 B7 45 */	bl isGoal__7TEMarioFv
/* 800E1ACC 000DEA0C  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 800E1AD0 000DEA10  41 82 00 08 */	beq lbl_800E1AD8
/* 800E1AD4 000DEA14  3B 80 00 01 */	li r28, 1
lbl_800E1AD8:
/* 800E1AD8 000DEA18  38 00 00 00 */	li r0, 0
/* 800E1ADC 000DEA1C  90 01 00 28 */	stw r0, 0x28(r1)
/* 800E1AE0 000DEA20  93 81 00 2C */	stw r28, 0x2c(r1)
/* 800E1AE4 000DEA24  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E1AE8 000DEA28  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800E1AEC 000DEA2C  7C 03 00 00 */	cmpw r3, r0
/* 800E1AF0 000DEA30  41 80 00 14 */	blt lbl_800E1B04
/* 800E1AF4 000DEA34  38 7E 07 8C */	addi r3, r30, 0x78c
/* 800E1AF8 000DEA38  4C C6 31 82 */	crclr 6
/* 800E1AFC 000DEA3C  4B FF B5 A9 */	bl SpcTrace__FPCce
/* 800E1B00 000DEA40  48 00 00 2C */	b lbl_800E1B2C
lbl_800E1B04:
/* 800E1B04 000DEA44  80 BF 00 20 */	lwz r5, 0x20(r31)
/* 800E1B08 000DEA48  54 64 18 38 */	slwi r4, r3, 3
/* 800E1B0C 000DEA4C  80 61 00 28 */	lwz r3, 0x28(r1)
/* 800E1B10 000DEA50  80 01 00 2C */	lwz r0, 0x2c(r1)
/* 800E1B14 000DEA54  7C 85 22 14 */	add r4, r5, r4
/* 800E1B18 000DEA58  90 64 00 00 */	stw r3, 0(r4)
/* 800E1B1C 000DEA5C  90 04 00 04 */	stw r0, 4(r4)
/* 800E1B20 000DEA60  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E1B24 000DEA64  38 03 00 01 */	addi r0, r3, 1
/* 800E1B28 000DEA68  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800E1B2C:
/* 800E1B2C 000DEA6C  80 01 00 44 */	lwz r0, 0x44(r1)
/* 800E1B30 000DEA70  83 E1 00 3C */	lwz r31, 0x3c(r1)
/* 800E1B34 000DEA74  83 C1 00 38 */	lwz r30, 0x38(r1)
/* 800E1B38 000DEA78  7C 08 03 A6 */	mtlr r0
/* 800E1B3C 000DEA7C  83 A1 00 34 */	lwz r29, 0x34(r1)
/* 800E1B40 000DEA80  83 81 00 30 */	lwz r28, 0x30(r1)
/* 800E1B44 000DEA84  38 21 00 40 */	addi r1, r1, 0x40
/* 800E1B48 000DEA88  4E 80 00 20 */	blr 

.global evStopTimer__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evStopTimer__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E1B4C 000DEA8C  7C 08 02 A6 */	mflr r0
/* 800E1B50 000DEA90  90 01 00 04 */	stw r0, 4(r1)
/* 800E1B54 000DEA94  94 21 FF D0 */	stwu r1, -0x30(r1)
/* 800E1B58 000DEA98  93 E1 00 2C */	stw r31, 0x2c(r1)
/* 800E1B5C 000DEA9C  7C 7F 1B 78 */	mr r31, r3
/* 800E1B60 000DEAA0  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E1B64 000DEAA4  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E1B68 000DEAA8  38 80 00 00 */	li r4, 0
/* 800E1B6C 000DEAAC  48 03 58 C5 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E1B70 000DEAB0  80 6D 97 E8 */	lwz r3, gpMarDirector@sda21(r13)
/* 800E1B74 000DEAB4  80 63 00 74 */	lwz r3, 0x74(r3)
/* 800E1B78 000DEAB8  48 12 82 71 */	bl stopMoveTimer__11TGCConsole2Fv
/* 800E1B7C 000DEABC  38 00 00 00 */	li r0, 0
/* 800E1B80 000DEAC0  90 01 00 1C */	stw r0, 0x1c(r1)
/* 800E1B84 000DEAC4  90 01 00 20 */	stw r0, 0x20(r1)
/* 800E1B88 000DEAC8  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E1B8C 000DEACC  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800E1B90 000DEAD0  7C 03 00 00 */	cmpw r3, r0
/* 800E1B94 000DEAD4  41 80 00 18 */	blt lbl_800E1BAC
/* 800E1B98 000DEAD8  3C 60 80 37 */	lis r3, $$23834@ha
/* 800E1B9C 000DEADC  4C C6 31 82 */	crclr 6
/* 800E1BA0 000DEAE0  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800E1BA4 000DEAE4  4B FF B5 01 */	bl SpcTrace__FPCce
/* 800E1BA8 000DEAE8  48 00 00 2C */	b lbl_800E1BD4
lbl_800E1BAC:
/* 800E1BAC 000DEAEC  80 BF 00 20 */	lwz r5, 0x20(r31)
/* 800E1BB0 000DEAF0  54 64 18 38 */	slwi r4, r3, 3
/* 800E1BB4 000DEAF4  80 61 00 1C */	lwz r3, 0x1c(r1)
/* 800E1BB8 000DEAF8  80 01 00 20 */	lwz r0, 0x20(r1)
/* 800E1BBC 000DEAFC  7C 85 22 14 */	add r4, r5, r4
/* 800E1BC0 000DEB00  90 64 00 00 */	stw r3, 0(r4)
/* 800E1BC4 000DEB04  90 04 00 04 */	stw r0, 4(r4)
/* 800E1BC8 000DEB08  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E1BCC 000DEB0C  38 03 00 01 */	addi r0, r3, 1
/* 800E1BD0 000DEB10  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800E1BD4:
/* 800E1BD4 000DEB14  80 01 00 34 */	lwz r0, 0x34(r1)
/* 800E1BD8 000DEB18  83 E1 00 2C */	lwz r31, 0x2c(r1)
/* 800E1BDC 000DEB1C  38 21 00 30 */	addi r1, r1, 0x30
/* 800E1BE0 000DEB20  7C 08 03 A6 */	mtlr r0
/* 800E1BE4 000DEB24  4E 80 00 20 */	blr 

.global evStartMonteman__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evStartMonteman__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E1BE8 000DEB28  7C 08 02 A6 */	mflr r0
/* 800E1BEC 000DEB2C  90 01 00 04 */	stw r0, 4(r1)
/* 800E1BF0 000DEB30  94 21 FF 90 */	stwu r1, -0x70(r1)
/* 800E1BF4 000DEB34  93 E1 00 6C */	stw r31, 0x6c(r1)
/* 800E1BF8 000DEB38  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E1BFC 000DEB3C  93 C1 00 68 */	stw r30, 0x68(r1)
/* 800E1C00 000DEB40  93 A1 00 64 */	stw r29, 0x64(r1)
/* 800E1C04 000DEB44  7C 7D 1B 78 */	mr r29, r3
/* 800E1C08 000DEB48  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E1C0C 000DEB4C  3C 80 80 37 */	lis r4, $$21490@ha
/* 800E1C10 000DEB50  3B E4 29 90 */	addi r31, r4, $$21490@l
/* 800E1C14 000DEB54  38 80 00 01 */	li r4, 1
/* 800E1C18 000DEB58  48 03 58 19 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E1C1C 000DEB5C  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 800E1C20 000DEB60  38 7F 08 0C */	addi r3, r31, 0x80c
/* 800E1C24 000DEB64  83 C4 00 04 */	lwz r30, 4(r4)
/* 800E1C28 000DEB68  4B F6 2A 65 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800E1C2C 000DEB6C  81 9E 00 00 */	lwz r12, 0(r30)
/* 800E1C30 000DEB70  38 83 00 00 */	addi r4, r3, 0
/* 800E1C34 000DEB74  38 7E 00 00 */	addi r3, r30, 0
/* 800E1C38 000DEB78  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 800E1C3C 000DEB7C  38 BF 08 0C */	addi r5, r31, 0x80c
/* 800E1C40 000DEB80  7D 88 03 A6 */	mtlr r12
/* 800E1C44 000DEB84  4E 80 00 21 */	blrl 
/* 800E1C48 000DEB88  80 9D 00 1C */	lwz r4, 0x1c(r29)
/* 800E1C4C 000DEB8C  3B C3 00 00 */	addi r30, r3, 0
/* 800E1C50 000DEB90  2C 04 00 00 */	cmpwi r4, 0
/* 800E1C54 000DEB94  41 81 00 30 */	bgt lbl_800E1C84
/* 800E1C58 000DEB98  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800E1C5C 000DEB9C  4C C6 31 82 */	crclr 6
/* 800E1C60 000DEBA0  4B FF B4 45 */	bl SpcTrace__FPCce
/* 800E1C64 000DEBA4  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E1C68 000DEBA8  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E1C6C 000DEBAC  54 00 18 38 */	slwi r0, r0, 3
/* 800E1C70 000DEBB0  7C 63 02 14 */	add r3, r3, r0
/* 800E1C74 000DEBB4  80 83 00 00 */	lwz r4, 0(r3)
/* 800E1C78 000DEBB8  80 03 00 04 */	lwz r0, 4(r3)
/* 800E1C7C 000DEBBC  90 01 00 4C */	stw r0, 0x4c(r1)
/* 800E1C80 000DEBC0  48 00 00 28 */	b lbl_800E1CA8
lbl_800E1C84:
/* 800E1C84 000DEBC4  38 04 FF FF */	addi r0, r4, -1
/* 800E1C88 000DEBC8  90 1D 00 1C */	stw r0, 0x1c(r29)
/* 800E1C8C 000DEBCC  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E1C90 000DEBD0  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E1C94 000DEBD4  54 00 18 38 */	slwi r0, r0, 3
/* 800E1C98 000DEBD8  7C 63 02 14 */	add r3, r3, r0
/* 800E1C9C 000DEBDC  80 83 00 00 */	lwz r4, 0(r3)
/* 800E1CA0 000DEBE0  80 03 00 04 */	lwz r0, 4(r3)
/* 800E1CA4 000DEBE4  90 01 00 4C */	stw r0, 0x4c(r1)
lbl_800E1CA8:
/* 800E1CA8 000DEBE8  90 81 00 50 */	stw r4, 0x50(r1)
/* 800E1CAC 000DEBEC  80 01 00 4C */	lwz r0, 0x4c(r1)
/* 800E1CB0 000DEBF0  90 01 00 54 */	stw r0, 0x54(r1)
/* 800E1CB4 000DEBF4  80 01 00 50 */	lwz r0, 0x50(r1)
/* 800E1CB8 000DEBF8  2C 00 00 01 */	cmpwi r0, 1
/* 800E1CBC 000DEBFC  41 82 00 1C */	beq lbl_800E1CD8
/* 800E1CC0 000DEC00  40 80 00 2C */	bge lbl_800E1CEC
/* 800E1CC4 000DEC04  2C 00 00 00 */	cmpwi r0, 0
/* 800E1CC8 000DEC08  40 80 00 08 */	bge lbl_800E1CD0
/* 800E1CCC 000DEC0C  48 00 00 20 */	b lbl_800E1CEC
lbl_800E1CD0:
/* 800E1CD0 000DEC10  80 01 00 54 */	lwz r0, 0x54(r1)
/* 800E1CD4 000DEC14  48 00 00 1C */	b lbl_800E1CF0
lbl_800E1CD8:
/* 800E1CD8 000DEC18  C0 01 00 54 */	lfs f0, 0x54(r1)
/* 800E1CDC 000DEC1C  FC 00 00 1E */	fctiwz f0, f0
/* 800E1CE0 000DEC20  D8 01 00 58 */	stfd f0, 0x58(r1)
/* 800E1CE4 000DEC24  80 01 00 5C */	lwz r0, 0x5c(r1)
/* 800E1CE8 000DEC28  48 00 00 08 */	b lbl_800E1CF0
lbl_800E1CEC:
/* 800E1CEC 000DEC2C  38 00 00 00 */	li r0, 0
lbl_800E1CF0:
/* 800E1CF0 000DEC30  28 1E 00 00 */	cmplwi r30, 0
/* 800E1CF4 000DEC34  7C 04 03 78 */	mr r4, r0
/* 800E1CF8 000DEC38  41 82 00 0C */	beq lbl_800E1D04
/* 800E1CFC 000DEC3C  7F C3 F3 78 */	mr r3, r30
/* 800E1D00 000DEC40  48 16 B4 85 */	bl startMonteReplay__7TEMarioFUl
lbl_800E1D04:
/* 800E1D04 000DEC44  38 00 00 00 */	li r0, 0
/* 800E1D08 000DEC48  90 01 00 3C */	stw r0, 0x3c(r1)
/* 800E1D0C 000DEC4C  90 01 00 40 */	stw r0, 0x40(r1)
/* 800E1D10 000DEC50  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E1D14 000DEC54  80 1D 00 18 */	lwz r0, 0x18(r29)
/* 800E1D18 000DEC58  7C 03 00 00 */	cmpw r3, r0
/* 800E1D1C 000DEC5C  41 80 00 14 */	blt lbl_800E1D30
/* 800E1D20 000DEC60  38 7F 07 8C */	addi r3, r31, 0x78c
/* 800E1D24 000DEC64  4C C6 31 82 */	crclr 6
/* 800E1D28 000DEC68  4B FF B3 7D */	bl SpcTrace__FPCce
/* 800E1D2C 000DEC6C  48 00 00 2C */	b lbl_800E1D58
lbl_800E1D30:
/* 800E1D30 000DEC70  80 BD 00 20 */	lwz r5, 0x20(r29)
/* 800E1D34 000DEC74  54 64 18 38 */	slwi r4, r3, 3
/* 800E1D38 000DEC78  80 61 00 3C */	lwz r3, 0x3c(r1)
/* 800E1D3C 000DEC7C  80 01 00 40 */	lwz r0, 0x40(r1)
/* 800E1D40 000DEC80  7C 85 22 14 */	add r4, r5, r4
/* 800E1D44 000DEC84  90 64 00 00 */	stw r3, 0(r4)
/* 800E1D48 000DEC88  90 04 00 04 */	stw r0, 4(r4)
/* 800E1D4C 000DEC8C  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E1D50 000DEC90  38 03 00 01 */	addi r0, r3, 1
/* 800E1D54 000DEC94  90 1D 00 1C */	stw r0, 0x1c(r29)
lbl_800E1D58:
/* 800E1D58 000DEC98  80 01 00 74 */	lwz r0, 0x74(r1)
/* 800E1D5C 000DEC9C  83 E1 00 6C */	lwz r31, 0x6c(r1)
/* 800E1D60 000DECA0  83 C1 00 68 */	lwz r30, 0x68(r1)
/* 800E1D64 000DECA4  7C 08 03 A6 */	mtlr r0
/* 800E1D68 000DECA8  83 A1 00 64 */	lwz r29, 0x64(r1)
/* 800E1D6C 000DECAC  38 21 00 70 */	addi r1, r1, 0x70
/* 800E1D70 000DECB0  4E 80 00 20 */	blr 

.global evStartTimer__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evStartTimer__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E1D74 000DECB4  7C 08 02 A6 */	mflr r0
/* 800E1D78 000DECB8  90 01 00 04 */	stw r0, 4(r1)
/* 800E1D7C 000DECBC  94 21 FF 98 */	stwu r1, -0x68(r1)
/* 800E1D80 000DECC0  93 E1 00 64 */	stw r31, 0x64(r1)
/* 800E1D84 000DECC4  7C 7F 1B 78 */	mr r31, r3
/* 800E1D88 000DECC8  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E1D8C 000DECCC  93 C1 00 60 */	stw r30, 0x60(r1)
/* 800E1D90 000DECD0  93 A1 00 5C */	stw r29, 0x5c(r1)
/* 800E1D94 000DECD4  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E1D98 000DECD8  38 80 00 01 */	li r4, 1
/* 800E1D9C 000DECDC  48 03 56 95 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E1DA0 000DECE0  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E1DA4 000DECE4  2C 03 00 00 */	cmpwi r3, 0
/* 800E1DA8 000DECE8  41 81 00 38 */	bgt lbl_800E1DE0
/* 800E1DAC 000DECEC  3C 60 80 37 */	lis r3, $$23829@ha
/* 800E1DB0 000DECF0  4C C6 31 82 */	crclr 6
/* 800E1DB4 000DECF4  38 63 30 FC */	addi r3, r3, $$23829@l
/* 800E1DB8 000DECF8  4B FF B2 ED */	bl SpcTrace__FPCce
/* 800E1DBC 000DECFC  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800E1DC0 000DED00  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800E1DC4 000DED04  54 00 18 38 */	slwi r0, r0, 3
/* 800E1DC8 000DED08  7C 63 02 14 */	add r3, r3, r0
/* 800E1DCC 000DED0C  80 03 00 00 */	lwz r0, 0(r3)
/* 800E1DD0 000DED10  90 01 00 44 */	stw r0, 0x44(r1)
/* 800E1DD4 000DED14  80 03 00 04 */	lwz r0, 4(r3)
/* 800E1DD8 000DED18  90 01 00 48 */	stw r0, 0x48(r1)
/* 800E1DDC 000DED1C  48 00 00 2C */	b lbl_800E1E08
lbl_800E1DE0:
/* 800E1DE0 000DED20  38 03 FF FF */	addi r0, r3, -1
/* 800E1DE4 000DED24  90 1F 00 1C */	stw r0, 0x1c(r31)
/* 800E1DE8 000DED28  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800E1DEC 000DED2C  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800E1DF0 000DED30  54 00 18 38 */	slwi r0, r0, 3
/* 800E1DF4 000DED34  7C 63 02 14 */	add r3, r3, r0
/* 800E1DF8 000DED38  80 03 00 00 */	lwz r0, 0(r3)
/* 800E1DFC 000DED3C  90 01 00 44 */	stw r0, 0x44(r1)
/* 800E1E00 000DED40  80 03 00 04 */	lwz r0, 4(r3)
/* 800E1E04 000DED44  90 01 00 48 */	stw r0, 0x48(r1)
lbl_800E1E08:
/* 800E1E08 000DED48  80 01 00 44 */	lwz r0, 0x44(r1)
/* 800E1E0C 000DED4C  2C 00 00 01 */	cmpwi r0, 1
/* 800E1E10 000DED50  41 82 00 1C */	beq lbl_800E1E2C
/* 800E1E14 000DED54  40 80 00 2C */	bge lbl_800E1E40
/* 800E1E18 000DED58  2C 00 00 00 */	cmpwi r0, 0
/* 800E1E1C 000DED5C  40 80 00 08 */	bge lbl_800E1E24
/* 800E1E20 000DED60  48 00 00 20 */	b lbl_800E1E40
lbl_800E1E24:
/* 800E1E24 000DED64  80 01 00 48 */	lwz r0, 0x48(r1)
/* 800E1E28 000DED68  48 00 00 1C */	b lbl_800E1E44
lbl_800E1E2C:
/* 800E1E2C 000DED6C  C0 01 00 48 */	lfs f0, 0x48(r1)
/* 800E1E30 000DED70  FC 00 00 1E */	fctiwz f0, f0
/* 800E1E34 000DED74  D8 01 00 50 */	stfd f0, 0x50(r1)
/* 800E1E38 000DED78  80 01 00 54 */	lwz r0, 0x54(r1)
/* 800E1E3C 000DED7C  48 00 00 08 */	b lbl_800E1E44
lbl_800E1E40:
/* 800E1E40 000DED80  38 00 00 00 */	li r0, 0
lbl_800E1E44:
/* 800E1E44 000DED84  83 CD 97 E8 */	lwz r30, gpMarDirector@sda21(r13)
/* 800E1E48 000DED88  7C 1D 03 78 */	mr r29, r0
/* 800E1E4C 000DED8C  38 7E 00 E8 */	addi r3, r30, 0xe8
/* 800E1E50 000DED90  4B FB 0B E5 */	bl OSCheckStopwatch
/* 800E1E54 000DED94  90 9E 00 CC */	stw r4, 0xcc(r30)
/* 800E1E58 000DED98  7F A4 EB 78 */	mr r4, r29
/* 800E1E5C 000DED9C  90 7E 00 C8 */	stw r3, 0xc8(r30)
/* 800E1E60 000DEDA0  80 6D 97 E8 */	lwz r3, gpMarDirector@sda21(r13)
/* 800E1E64 000DEDA4  80 63 00 74 */	lwz r3, 0x74(r3)
/* 800E1E68 000DEDA8  48 12 7F 99 */	bl startMoveTimer__11TGCConsole2Fi
/* 800E1E6C 000DEDAC  38 00 00 00 */	li r0, 0
/* 800E1E70 000DEDB0  90 01 00 34 */	stw r0, 0x34(r1)
/* 800E1E74 000DEDB4  90 01 00 38 */	stw r0, 0x38(r1)
/* 800E1E78 000DEDB8  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E1E7C 000DEDBC  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800E1E80 000DEDC0  7C 03 00 00 */	cmpw r3, r0
/* 800E1E84 000DEDC4  41 80 00 18 */	blt lbl_800E1E9C
/* 800E1E88 000DEDC8  3C 60 80 37 */	lis r3, $$23834@ha
/* 800E1E8C 000DEDCC  4C C6 31 82 */	crclr 6
/* 800E1E90 000DEDD0  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800E1E94 000DEDD4  4B FF B2 11 */	bl SpcTrace__FPCce
/* 800E1E98 000DEDD8  48 00 00 2C */	b lbl_800E1EC4
lbl_800E1E9C:
/* 800E1E9C 000DEDDC  80 BF 00 20 */	lwz r5, 0x20(r31)
/* 800E1EA0 000DEDE0  54 64 18 38 */	slwi r4, r3, 3
/* 800E1EA4 000DEDE4  80 61 00 34 */	lwz r3, 0x34(r1)
/* 800E1EA8 000DEDE8  80 01 00 38 */	lwz r0, 0x38(r1)
/* 800E1EAC 000DEDEC  7C 85 22 14 */	add r4, r5, r4
/* 800E1EB0 000DEDF0  90 64 00 00 */	stw r3, 0(r4)
/* 800E1EB4 000DEDF4  90 04 00 04 */	stw r0, 4(r4)
/* 800E1EB8 000DEDF8  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E1EBC 000DEDFC  38 03 00 01 */	addi r0, r3, 1
/* 800E1EC0 000DEE00  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800E1EC4:
/* 800E1EC4 000DEE04  80 01 00 6C */	lwz r0, 0x6c(r1)
/* 800E1EC8 000DEE08  83 E1 00 64 */	lwz r31, 0x64(r1)
/* 800E1ECC 000DEE0C  83 C1 00 60 */	lwz r30, 0x60(r1)
/* 800E1ED0 000DEE10  7C 08 03 A6 */	mtlr r0
/* 800E1ED4 000DEE14  83 A1 00 5C */	lwz r29, 0x5c(r1)
/* 800E1ED8 000DEE18  38 21 00 68 */	addi r1, r1, 0x68
/* 800E1EDC 000DEE1C  4E 80 00 20 */	blr 

.global evInsertTimer__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evInsertTimer__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E1EE0 000DEE20  7C 08 02 A6 */	mflr r0
/* 800E1EE4 000DEE24  90 01 00 04 */	stw r0, 4(r1)
/* 800E1EE8 000DEE28  94 21 FF 60 */	stwu r1, -0xa0(r1)
/* 800E1EEC 000DEE2C  93 E1 00 9C */	stw r31, 0x9c(r1)
/* 800E1EF0 000DEE30  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E1EF4 000DEE34  93 C1 00 98 */	stw r30, 0x98(r1)
/* 800E1EF8 000DEE38  93 A1 00 94 */	stw r29, 0x94(r1)
/* 800E1EFC 000DEE3C  7C 7D 1B 78 */	mr r29, r3
/* 800E1F00 000DEE40  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E1F04 000DEE44  3C 80 80 37 */	lis r4, $$21490@ha
/* 800E1F08 000DEE48  3B E4 29 90 */	addi r31, r4, $$21490@l
/* 800E1F0C 000DEE4C  38 80 00 02 */	li r4, 2
/* 800E1F10 000DEE50  48 03 55 21 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E1F14 000DEE54  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E1F18 000DEE58  2C 03 00 00 */	cmpwi r3, 0
/* 800E1F1C 000DEE5C  41 81 00 34 */	bgt lbl_800E1F50
/* 800E1F20 000DEE60  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800E1F24 000DEE64  4C C6 31 82 */	crclr 6
/* 800E1F28 000DEE68  4B FF B1 7D */	bl SpcTrace__FPCce
/* 800E1F2C 000DEE6C  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E1F30 000DEE70  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E1F34 000DEE74  54 00 18 38 */	slwi r0, r0, 3
/* 800E1F38 000DEE78  7C 63 02 14 */	add r3, r3, r0
/* 800E1F3C 000DEE7C  80 03 00 00 */	lwz r0, 0(r3)
/* 800E1F40 000DEE80  90 01 00 80 */	stw r0, 0x80(r1)
/* 800E1F44 000DEE84  80 03 00 04 */	lwz r0, 4(r3)
/* 800E1F48 000DEE88  90 01 00 84 */	stw r0, 0x84(r1)
/* 800E1F4C 000DEE8C  48 00 00 2C */	b lbl_800E1F78
lbl_800E1F50:
/* 800E1F50 000DEE90  38 03 FF FF */	addi r0, r3, -1
/* 800E1F54 000DEE94  90 1D 00 1C */	stw r0, 0x1c(r29)
/* 800E1F58 000DEE98  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E1F5C 000DEE9C  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E1F60 000DEEA0  54 00 18 38 */	slwi r0, r0, 3
/* 800E1F64 000DEEA4  7C 63 02 14 */	add r3, r3, r0
/* 800E1F68 000DEEA8  80 03 00 00 */	lwz r0, 0(r3)
/* 800E1F6C 000DEEAC  90 01 00 80 */	stw r0, 0x80(r1)
/* 800E1F70 000DEEB0  80 03 00 04 */	lwz r0, 4(r3)
/* 800E1F74 000DEEB4  90 01 00 84 */	stw r0, 0x84(r1)
lbl_800E1F78:
/* 800E1F78 000DEEB8  80 01 00 80 */	lwz r0, 0x80(r1)
/* 800E1F7C 000DEEBC  2C 00 00 01 */	cmpwi r0, 1
/* 800E1F80 000DEEC0  41 82 00 1C */	beq lbl_800E1F9C
/* 800E1F84 000DEEC4  40 80 00 2C */	bge lbl_800E1FB0
/* 800E1F88 000DEEC8  2C 00 00 00 */	cmpwi r0, 0
/* 800E1F8C 000DEECC  40 80 00 08 */	bge lbl_800E1F94
/* 800E1F90 000DEED0  48 00 00 20 */	b lbl_800E1FB0
lbl_800E1F94:
/* 800E1F94 000DEED4  80 01 00 84 */	lwz r0, 0x84(r1)
/* 800E1F98 000DEED8  48 00 00 1C */	b lbl_800E1FB4
lbl_800E1F9C:
/* 800E1F9C 000DEEDC  C0 01 00 84 */	lfs f0, 0x84(r1)
/* 800E1FA0 000DEEE0  FC 00 00 1E */	fctiwz f0, f0
/* 800E1FA4 000DEEE4  D8 01 00 88 */	stfd f0, 0x88(r1)
/* 800E1FA8 000DEEE8  80 01 00 8C */	lwz r0, 0x8c(r1)
/* 800E1FAC 000DEEEC  48 00 00 08 */	b lbl_800E1FB4
lbl_800E1FB0:
/* 800E1FB0 000DEEF0  38 00 00 00 */	li r0, 0
lbl_800E1FB4:
/* 800E1FB4 000DEEF4  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E1FB8 000DEEF8  7C 1E 03 78 */	mr r30, r0
/* 800E1FBC 000DEEFC  2C 03 00 00 */	cmpwi r3, 0
/* 800E1FC0 000DEF00  41 81 00 34 */	bgt lbl_800E1FF4
/* 800E1FC4 000DEF04  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800E1FC8 000DEF08  4C C6 31 82 */	crclr 6
/* 800E1FCC 000DEF0C  4B FF B0 D9 */	bl SpcTrace__FPCce
/* 800E1FD0 000DEF10  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E1FD4 000DEF14  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E1FD8 000DEF18  54 00 18 38 */	slwi r0, r0, 3
/* 800E1FDC 000DEF1C  7C 63 02 14 */	add r3, r3, r0
/* 800E1FE0 000DEF20  80 03 00 00 */	lwz r0, 0(r3)
/* 800E1FE4 000DEF24  90 01 00 78 */	stw r0, 0x78(r1)
/* 800E1FE8 000DEF28  80 03 00 04 */	lwz r0, 4(r3)
/* 800E1FEC 000DEF2C  90 01 00 7C */	stw r0, 0x7c(r1)
/* 800E1FF0 000DEF30  48 00 00 2C */	b lbl_800E201C
lbl_800E1FF4:
/* 800E1FF4 000DEF34  38 03 FF FF */	addi r0, r3, -1
/* 800E1FF8 000DEF38  90 1D 00 1C */	stw r0, 0x1c(r29)
/* 800E1FFC 000DEF3C  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E2000 000DEF40  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E2004 000DEF44  54 00 18 38 */	slwi r0, r0, 3
/* 800E2008 000DEF48  7C 63 02 14 */	add r3, r3, r0
/* 800E200C 000DEF4C  80 03 00 00 */	lwz r0, 0(r3)
/* 800E2010 000DEF50  90 01 00 78 */	stw r0, 0x78(r1)
/* 800E2014 000DEF54  80 03 00 04 */	lwz r0, 4(r3)
/* 800E2018 000DEF58  90 01 00 7C */	stw r0, 0x7c(r1)
lbl_800E201C:
/* 800E201C 000DEF5C  80 01 00 78 */	lwz r0, 0x78(r1)
/* 800E2020 000DEF60  2C 00 00 01 */	cmpwi r0, 1
/* 800E2024 000DEF64  41 82 00 1C */	beq lbl_800E2040
/* 800E2028 000DEF68  40 80 00 2C */	bge lbl_800E2054
/* 800E202C 000DEF6C  2C 00 00 00 */	cmpwi r0, 0
/* 800E2030 000DEF70  40 80 00 08 */	bge lbl_800E2038
/* 800E2034 000DEF74  48 00 00 20 */	b lbl_800E2054
lbl_800E2038:
/* 800E2038 000DEF78  80 01 00 7C */	lwz r0, 0x7c(r1)
/* 800E203C 000DEF7C  48 00 00 1C */	b lbl_800E2058
lbl_800E2040:
/* 800E2040 000DEF80  C0 01 00 7C */	lfs f0, 0x7c(r1)
/* 800E2044 000DEF84  FC 00 00 1E */	fctiwz f0, f0
/* 800E2048 000DEF88  D8 01 00 88 */	stfd f0, 0x88(r1)
/* 800E204C 000DEF8C  80 01 00 8C */	lwz r0, 0x8c(r1)
/* 800E2050 000DEF90  48 00 00 08 */	b lbl_800E2058
lbl_800E2054:
/* 800E2054 000DEF94  38 00 00 00 */	li r0, 0
lbl_800E2058:
/* 800E2058 000DEF98  2C 00 00 01 */	cmpwi r0, 1
/* 800E205C 000DEF9C  40 82 00 1C */	bne lbl_800E2078
/* 800E2060 000DEFA0  80 6D 97 E8 */	lwz r3, gpMarDirector@sda21(r13)
/* 800E2064 000DEFA4  38 BE 00 00 */	addi r5, r30, 0
/* 800E2068 000DEFA8  38 80 00 00 */	li r4, 0
/* 800E206C 000DEFAC  80 63 00 74 */	lwz r3, 0x74(r3)
/* 800E2070 000DEFB0  48 12 A3 B9 */	bl startAppearTimer__11TGCConsole2Fil
/* 800E2074 000DEFB4  48 00 00 30 */	b lbl_800E20A4
lbl_800E2078:
/* 800E2078 000DEFB8  2C 00 00 02 */	cmpwi r0, 2
/* 800E207C 000DEFBC  40 82 00 1C */	bne lbl_800E2098
/* 800E2080 000DEFC0  80 6D 97 E8 */	lwz r3, gpMarDirector@sda21(r13)
/* 800E2084 000DEFC4  38 BE 00 00 */	addi r5, r30, 0
/* 800E2088 000DEFC8  38 80 00 01 */	li r4, 1
/* 800E208C 000DEFCC  80 63 00 74 */	lwz r3, 0x74(r3)
/* 800E2090 000DEFD0  48 12 A3 99 */	bl startAppearTimer__11TGCConsole2Fil
/* 800E2094 000DEFD4  48 00 00 10 */	b lbl_800E20A4
lbl_800E2098:
/* 800E2098 000DEFD8  80 6D 97 E8 */	lwz r3, gpMarDirector@sda21(r13)
/* 800E209C 000DEFDC  80 63 00 74 */	lwz r3, 0x74(r3)
/* 800E20A0 000DEFE0  48 12 A5 95 */	bl startDisappearTimer__11TGCConsole2Fv
lbl_800E20A4:
/* 800E20A4 000DEFE4  38 00 00 00 */	li r0, 0
/* 800E20A8 000DEFE8  90 01 00 5C */	stw r0, 0x5c(r1)
/* 800E20AC 000DEFEC  90 01 00 60 */	stw r0, 0x60(r1)
/* 800E20B0 000DEFF0  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E20B4 000DEFF4  80 1D 00 18 */	lwz r0, 0x18(r29)
/* 800E20B8 000DEFF8  7C 03 00 00 */	cmpw r3, r0
/* 800E20BC 000DEFFC  41 80 00 14 */	blt lbl_800E20D0
/* 800E20C0 000DF000  38 7F 07 8C */	addi r3, r31, 0x78c
/* 800E20C4 000DF004  4C C6 31 82 */	crclr 6
/* 800E20C8 000DF008  4B FF AF DD */	bl SpcTrace__FPCce
/* 800E20CC 000DF00C  48 00 00 2C */	b lbl_800E20F8
lbl_800E20D0:
/* 800E20D0 000DF010  80 BD 00 20 */	lwz r5, 0x20(r29)
/* 800E20D4 000DF014  54 64 18 38 */	slwi r4, r3, 3
/* 800E20D8 000DF018  80 61 00 5C */	lwz r3, 0x5c(r1)
/* 800E20DC 000DF01C  80 01 00 60 */	lwz r0, 0x60(r1)
/* 800E20E0 000DF020  7C 85 22 14 */	add r4, r5, r4
/* 800E20E4 000DF024  90 64 00 00 */	stw r3, 0(r4)
/* 800E20E8 000DF028  90 04 00 04 */	stw r0, 4(r4)
/* 800E20EC 000DF02C  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E20F0 000DF030  38 03 00 01 */	addi r0, r3, 1
/* 800E20F4 000DF034  90 1D 00 1C */	stw r0, 0x1c(r29)
lbl_800E20F8:
/* 800E20F8 000DF038  80 01 00 A4 */	lwz r0, 0xa4(r1)
/* 800E20FC 000DF03C  83 E1 00 9C */	lwz r31, 0x9c(r1)
/* 800E2100 000DF040  83 C1 00 98 */	lwz r30, 0x98(r1)
/* 800E2104 000DF044  7C 08 03 A6 */	mtlr r0
/* 800E2108 000DF048  83 A1 00 94 */	lwz r29, 0x94(r1)
/* 800E210C 000DF04C  38 21 00 A0 */	addi r1, r1, 0xa0
/* 800E2110 000DF050  4E 80 00 20 */	blr 

.global evForceCloseTalk__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evForceCloseTalk__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E2114 000DF054  7C 08 02 A6 */	mflr r0
/* 800E2118 000DF058  90 01 00 04 */	stw r0, 4(r1)
/* 800E211C 000DF05C  94 21 FF D8 */	stwu r1, -0x28(r1)
/* 800E2120 000DF060  93 E1 00 24 */	stw r31, 0x24(r1)
/* 800E2124 000DF064  7C 7F 1B 78 */	mr r31, r3
/* 800E2128 000DF068  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E212C 000DF06C  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E2130 000DF070  38 80 00 00 */	li r4, 0
/* 800E2134 000DF074  48 03 52 FD */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E2138 000DF078  80 6D 9B 68 */	lwz r3, gpTalk2D@sda21(r13)
/* 800E213C 000DF07C  48 13 2D C9 */	bl forceCloseTalk__8TTalk2D2Fv
/* 800E2140 000DF080  38 00 00 00 */	li r0, 0
/* 800E2144 000DF084  90 01 00 14 */	stw r0, 0x14(r1)
/* 800E2148 000DF088  90 01 00 18 */	stw r0, 0x18(r1)
/* 800E214C 000DF08C  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E2150 000DF090  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800E2154 000DF094  7C 03 00 00 */	cmpw r3, r0
/* 800E2158 000DF098  41 80 00 18 */	blt lbl_800E2170
/* 800E215C 000DF09C  3C 60 80 37 */	lis r3, $$23834@ha
/* 800E2160 000DF0A0  4C C6 31 82 */	crclr 6
/* 800E2164 000DF0A4  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800E2168 000DF0A8  4B FF AF 3D */	bl SpcTrace__FPCce
/* 800E216C 000DF0AC  48 00 00 2C */	b lbl_800E2198
lbl_800E2170:
/* 800E2170 000DF0B0  80 BF 00 20 */	lwz r5, 0x20(r31)
/* 800E2174 000DF0B4  54 64 18 38 */	slwi r4, r3, 3
/* 800E2178 000DF0B8  80 61 00 14 */	lwz r3, 0x14(r1)
/* 800E217C 000DF0BC  80 01 00 18 */	lwz r0, 0x18(r1)
/* 800E2180 000DF0C0  7C 85 22 14 */	add r4, r5, r4
/* 800E2184 000DF0C4  90 64 00 00 */	stw r3, 0(r4)
/* 800E2188 000DF0C8  90 04 00 04 */	stw r0, 4(r4)
/* 800E218C 000DF0CC  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E2190 000DF0D0  38 03 00 01 */	addi r0, r3, 1
/* 800E2194 000DF0D4  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800E2198:
/* 800E2198 000DF0D8  80 01 00 2C */	lwz r0, 0x2c(r1)
/* 800E219C 000DF0DC  83 E1 00 24 */	lwz r31, 0x24(r1)
/* 800E21A0 000DF0E0  38 21 00 28 */	addi r1, r1, 0x28
/* 800E21A4 000DF0E4  7C 08 03 A6 */	mtlr r0
/* 800E21A8 000DF0E8  4E 80 00 20 */	blr 

.global evRaiseBuilding__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evRaiseBuilding__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E21AC 000DF0EC  7C 08 02 A6 */	mflr r0
/* 800E21B0 000DF0F0  90 01 00 04 */	stw r0, 4(r1)
/* 800E21B4 000DF0F4  94 21 FF 90 */	stwu r1, -0x70(r1)
/* 800E21B8 000DF0F8  93 E1 00 6C */	stw r31, 0x6c(r1)
/* 800E21BC 000DF0FC  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E21C0 000DF100  93 C1 00 68 */	stw r30, 0x68(r1)
/* 800E21C4 000DF104  7C 7E 1B 78 */	mr r30, r3
/* 800E21C8 000DF108  93 A1 00 64 */	stw r29, 0x64(r1)
/* 800E21CC 000DF10C  93 81 00 60 */	stw r28, 0x60(r1)
/* 800E21D0 000DF110  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E21D4 000DF114  3C 80 80 37 */	lis r4, $$21490@ha
/* 800E21D8 000DF118  3B E4 29 90 */	addi r31, r4, $$21490@l
/* 800E21DC 000DF11C  38 80 00 01 */	li r4, 1
/* 800E21E0 000DF120  48 03 52 51 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E21E4 000DF124  80 7E 00 1C */	lwz r3, 0x1c(r30)
/* 800E21E8 000DF128  2C 03 00 00 */	cmpwi r3, 0
/* 800E21EC 000DF12C  41 81 00 30 */	bgt lbl_800E221C
/* 800E21F0 000DF130  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800E21F4 000DF134  4C C6 31 82 */	crclr 6
/* 800E21F8 000DF138  4B FF AE AD */	bl SpcTrace__FPCce
/* 800E21FC 000DF13C  80 1E 00 1C */	lwz r0, 0x1c(r30)
/* 800E2200 000DF140  80 7E 00 20 */	lwz r3, 0x20(r30)
/* 800E2204 000DF144  54 00 18 38 */	slwi r0, r0, 3
/* 800E2208 000DF148  7C 63 02 14 */	add r3, r3, r0
/* 800E220C 000DF14C  80 83 00 00 */	lwz r4, 0(r3)
/* 800E2210 000DF150  80 03 00 04 */	lwz r0, 4(r3)
/* 800E2214 000DF154  90 01 00 4C */	stw r0, 0x4c(r1)
/* 800E2218 000DF158  48 00 00 28 */	b lbl_800E2240
lbl_800E221C:
/* 800E221C 000DF15C  38 03 FF FF */	addi r0, r3, -1
/* 800E2220 000DF160  90 1E 00 1C */	stw r0, 0x1c(r30)
/* 800E2224 000DF164  80 1E 00 1C */	lwz r0, 0x1c(r30)
/* 800E2228 000DF168  80 7E 00 20 */	lwz r3, 0x20(r30)
/* 800E222C 000DF16C  54 00 18 38 */	slwi r0, r0, 3
/* 800E2230 000DF170  7C 63 02 14 */	add r3, r3, r0
/* 800E2234 000DF174  80 83 00 00 */	lwz r4, 0(r3)
/* 800E2238 000DF178  80 03 00 04 */	lwz r0, 4(r3)
/* 800E223C 000DF17C  90 01 00 4C */	stw r0, 0x4c(r1)
lbl_800E2240:
/* 800E2240 000DF180  90 81 00 50 */	stw r4, 0x50(r1)
/* 800E2244 000DF184  80 01 00 4C */	lwz r0, 0x4c(r1)
/* 800E2248 000DF188  90 01 00 54 */	stw r0, 0x54(r1)
/* 800E224C 000DF18C  80 01 00 50 */	lwz r0, 0x50(r1)
/* 800E2250 000DF190  2C 00 00 01 */	cmpwi r0, 1
/* 800E2254 000DF194  41 82 00 1C */	beq lbl_800E2270
/* 800E2258 000DF198  40 80 00 2C */	bge lbl_800E2284
/* 800E225C 000DF19C  2C 00 00 00 */	cmpwi r0, 0
/* 800E2260 000DF1A0  40 80 00 08 */	bge lbl_800E2268
/* 800E2264 000DF1A4  48 00 00 20 */	b lbl_800E2284
lbl_800E2268:
/* 800E2268 000DF1A8  80 01 00 54 */	lwz r0, 0x54(r1)
/* 800E226C 000DF1AC  48 00 00 1C */	b lbl_800E2288
lbl_800E2270:
/* 800E2270 000DF1B0  C0 01 00 54 */	lfs f0, 0x54(r1)
/* 800E2274 000DF1B4  FC 00 00 1E */	fctiwz f0, f0
/* 800E2278 000DF1B8  D8 01 00 58 */	stfd f0, 0x58(r1)
/* 800E227C 000DF1BC  80 01 00 5C */	lwz r0, 0x5c(r1)
/* 800E2280 000DF1C0  48 00 00 08 */	b lbl_800E2288
lbl_800E2284:
/* 800E2284 000DF1C4  38 00 00 00 */	li r0, 0
lbl_800E2288:
/* 800E2288 000DF1C8  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 800E228C 000DF1CC  7C 1C 03 78 */	mr r28, r0
/* 800E2290 000DF1D0  38 7F 08 18 */	addi r3, r31, 0x818
/* 800E2294 000DF1D4  83 A4 00 04 */	lwz r29, 4(r4)
/* 800E2298 000DF1D8  4B F6 23 F5 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800E229C 000DF1DC  81 9D 00 00 */	lwz r12, 0(r29)
/* 800E22A0 000DF1E0  38 83 00 00 */	addi r4, r3, 0
/* 800E22A4 000DF1E4  38 7D 00 00 */	addi r3, r29, 0
/* 800E22A8 000DF1E8  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 800E22AC 000DF1EC  38 BF 08 18 */	addi r5, r31, 0x818
/* 800E22B0 000DF1F0  7D 88 03 A6 */	mtlr r12
/* 800E22B4 000DF1F4  4E 80 00 21 */	blrl 
/* 800E22B8 000DF1F8  28 03 00 00 */	cmplwi r3, 0
/* 800E22BC 000DF1FC  41 82 00 0C */	beq lbl_800E22C8
/* 800E22C0 000DF200  7F 84 E3 78 */	mr r4, r28
/* 800E22C4 000DF204  48 0F F2 B9 */	bl raiseBuilding__24TMapEventSinkShadowMarioFi
lbl_800E22C8:
/* 800E22C8 000DF208  38 00 00 00 */	li r0, 0
/* 800E22CC 000DF20C  90 01 00 3C */	stw r0, 0x3c(r1)
/* 800E22D0 000DF210  90 01 00 40 */	stw r0, 0x40(r1)
/* 800E22D4 000DF214  80 7E 00 1C */	lwz r3, 0x1c(r30)
/* 800E22D8 000DF218  80 1E 00 18 */	lwz r0, 0x18(r30)
/* 800E22DC 000DF21C  7C 03 00 00 */	cmpw r3, r0
/* 800E22E0 000DF220  41 80 00 14 */	blt lbl_800E22F4
/* 800E22E4 000DF224  38 7F 07 8C */	addi r3, r31, 0x78c
/* 800E22E8 000DF228  4C C6 31 82 */	crclr 6
/* 800E22EC 000DF22C  4B FF AD B9 */	bl SpcTrace__FPCce
/* 800E22F0 000DF230  48 00 00 2C */	b lbl_800E231C
lbl_800E22F4:
/* 800E22F4 000DF234  80 BE 00 20 */	lwz r5, 0x20(r30)
/* 800E22F8 000DF238  54 64 18 38 */	slwi r4, r3, 3
/* 800E22FC 000DF23C  80 61 00 3C */	lwz r3, 0x3c(r1)
/* 800E2300 000DF240  80 01 00 40 */	lwz r0, 0x40(r1)
/* 800E2304 000DF244  7C 85 22 14 */	add r4, r5, r4
/* 800E2308 000DF248  90 64 00 00 */	stw r3, 0(r4)
/* 800E230C 000DF24C  90 04 00 04 */	stw r0, 4(r4)
/* 800E2310 000DF250  80 7E 00 1C */	lwz r3, 0x1c(r30)
/* 800E2314 000DF254  38 03 00 01 */	addi r0, r3, 1
/* 800E2318 000DF258  90 1E 00 1C */	stw r0, 0x1c(r30)
lbl_800E231C:
/* 800E231C 000DF25C  80 01 00 74 */	lwz r0, 0x74(r1)
/* 800E2320 000DF260  83 E1 00 6C */	lwz r31, 0x6c(r1)
/* 800E2324 000DF264  83 C1 00 68 */	lwz r30, 0x68(r1)
/* 800E2328 000DF268  7C 08 03 A6 */	mtlr r0
/* 800E232C 000DF26C  83 A1 00 64 */	lwz r29, 0x64(r1)
/* 800E2330 000DF270  83 81 00 60 */	lwz r28, 0x60(r1)
/* 800E2334 000DF274  38 21 00 70 */	addi r1, r1, 0x70
/* 800E2338 000DF278  4E 80 00 20 */	blr 

.global evIsMapEventFinishedAll__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evIsMapEventFinishedAll__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E233C 000DF27C  7C 08 02 A6 */	mflr r0
/* 800E2340 000DF280  90 01 00 04 */	stw r0, 4(r1)
/* 800E2344 000DF284  94 21 FF 80 */	stwu r1, -0x80(r1)
/* 800E2348 000DF288  93 E1 00 7C */	stw r31, 0x7c(r1)
/* 800E234C 000DF28C  7C 7F 1B 78 */	mr r31, r3
/* 800E2350 000DF290  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E2354 000DF294  93 C1 00 78 */	stw r30, 0x78(r1)
/* 800E2358 000DF298  93 A1 00 74 */	stw r29, 0x74(r1)
/* 800E235C 000DF29C  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E2360 000DF2A0  38 80 00 01 */	li r4, 1
/* 800E2364 000DF2A4  48 03 50 CD */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E2368 000DF2A8  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E236C 000DF2AC  2C 03 00 00 */	cmpwi r3, 0
/* 800E2370 000DF2B0  41 81 00 38 */	bgt lbl_800E23A8
/* 800E2374 000DF2B4  3C 60 80 37 */	lis r3, $$23829@ha
/* 800E2378 000DF2B8  4C C6 31 82 */	crclr 6
/* 800E237C 000DF2BC  38 63 30 FC */	addi r3, r3, $$23829@l
/* 800E2380 000DF2C0  4B FF AD 25 */	bl SpcTrace__FPCce
/* 800E2384 000DF2C4  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800E2388 000DF2C8  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800E238C 000DF2CC  54 00 18 38 */	slwi r0, r0, 3
/* 800E2390 000DF2D0  7C 63 02 14 */	add r3, r3, r0
/* 800E2394 000DF2D4  80 03 00 00 */	lwz r0, 0(r3)
/* 800E2398 000DF2D8  90 01 00 54 */	stw r0, 0x54(r1)
/* 800E239C 000DF2DC  80 03 00 04 */	lwz r0, 4(r3)
/* 800E23A0 000DF2E0  90 01 00 58 */	stw r0, 0x58(r1)
/* 800E23A4 000DF2E4  48 00 00 2C */	b lbl_800E23D0
lbl_800E23A8:
/* 800E23A8 000DF2E8  38 03 FF FF */	addi r0, r3, -1
/* 800E23AC 000DF2EC  90 1F 00 1C */	stw r0, 0x1c(r31)
/* 800E23B0 000DF2F0  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800E23B4 000DF2F4  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800E23B8 000DF2F8  54 00 18 38 */	slwi r0, r0, 3
/* 800E23BC 000DF2FC  7C 63 02 14 */	add r3, r3, r0
/* 800E23C0 000DF300  80 03 00 00 */	lwz r0, 0(r3)
/* 800E23C4 000DF304  90 01 00 54 */	stw r0, 0x54(r1)
/* 800E23C8 000DF308  80 03 00 04 */	lwz r0, 4(r3)
/* 800E23CC 000DF30C  90 01 00 58 */	stw r0, 0x58(r1)
lbl_800E23D0:
/* 800E23D0 000DF310  80 81 00 54 */	lwz r4, 0x54(r1)
/* 800E23D4 000DF314  38 60 00 00 */	li r3, 0
/* 800E23D8 000DF318  80 01 00 58 */	lwz r0, 0x58(r1)
/* 800E23DC 000DF31C  90 81 00 5C */	stw r4, 0x5c(r1)
/* 800E23E0 000DF320  90 01 00 60 */	stw r0, 0x60(r1)
/* 800E23E4 000DF324  80 01 00 5C */	lwz r0, 0x5c(r1)
/* 800E23E8 000DF328  2C 00 00 01 */	cmpwi r0, 1
/* 800E23EC 000DF32C  41 82 00 9C */	beq lbl_800E2488
/* 800E23F0 000DF330  40 80 00 10 */	bge lbl_800E2400
/* 800E23F4 000DF334  2C 00 00 00 */	cmpwi r0, 0
/* 800E23F8 000DF338  40 80 00 58 */	bge lbl_800E2450
/* 800E23FC 000DF33C  48 00 00 8C */	b lbl_800E2488
lbl_800E2400:
/* 800E2400 000DF340  2C 00 00 03 */	cmpwi r0, 3
/* 800E2404 000DF344  40 80 00 84 */	bge lbl_800E2488
/* 800E2408 000DF348  2C 00 00 02 */	cmpwi r0, 2
/* 800E240C 000DF34C  41 82 00 08 */	beq lbl_800E2414
/* 800E2410 000DF350  48 00 00 0C */	b lbl_800E241C
lbl_800E2414:
/* 800E2414 000DF354  83 C1 00 60 */	lwz r30, 0x60(r1)
/* 800E2418 000DF358  48 00 00 08 */	b lbl_800E2420
lbl_800E241C:
/* 800E241C 000DF35C  3B C2 8F F0 */	addi r30, r2, $$24053@sda21
lbl_800E2420:
/* 800E2420 000DF360  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 800E2424 000DF364  7F C3 F3 78 */	mr r3, r30
/* 800E2428 000DF368  83 A4 00 04 */	lwz r29, 4(r4)
/* 800E242C 000DF36C  4B F6 22 61 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800E2430 000DF370  81 9D 00 00 */	lwz r12, 0(r29)
/* 800E2434 000DF374  38 83 00 00 */	addi r4, r3, 0
/* 800E2438 000DF378  38 7D 00 00 */	addi r3, r29, 0
/* 800E243C 000DF37C  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 800E2440 000DF380  7F C5 F3 78 */	mr r5, r30
/* 800E2444 000DF384  7D 88 03 A6 */	mtlr r12
/* 800E2448 000DF388  4E 80 00 21 */	blrl 
/* 800E244C 000DF38C  48 00 00 3C */	b lbl_800E2488
lbl_800E2450:
/* 800E2450 000DF390  2C 00 00 01 */	cmpwi r0, 1
/* 800E2454 000DF394  41 82 00 1C */	beq lbl_800E2470
/* 800E2458 000DF398  40 80 00 2C */	bge lbl_800E2484
/* 800E245C 000DF39C  2C 00 00 00 */	cmpwi r0, 0
/* 800E2460 000DF3A0  40 80 00 08 */	bge lbl_800E2468
/* 800E2464 000DF3A4  48 00 00 20 */	b lbl_800E2484
lbl_800E2468:
/* 800E2468 000DF3A8  80 61 00 60 */	lwz r3, 0x60(r1)
/* 800E246C 000DF3AC  48 00 00 1C */	b lbl_800E2488
lbl_800E2470:
/* 800E2470 000DF3B0  C0 01 00 60 */	lfs f0, 0x60(r1)
/* 800E2474 000DF3B4  FC 00 00 1E */	fctiwz f0, f0
/* 800E2478 000DF3B8  D8 01 00 68 */	stfd f0, 0x68(r1)
/* 800E247C 000DF3BC  80 61 00 6C */	lwz r3, 0x6c(r1)
/* 800E2480 000DF3C0  48 00 00 08 */	b lbl_800E2488
lbl_800E2484:
/* 800E2484 000DF3C4  38 60 00 00 */	li r3, 0
lbl_800E2488:
/* 800E2488 000DF3C8  81 83 00 00 */	lwz r12, 0(r3)
/* 800E248C 000DF3CC  81 8C 00 28 */	lwz r12, 0x28(r12)
/* 800E2490 000DF3D0  7D 88 03 A6 */	mtlr r12
/* 800E2494 000DF3D4  4E 80 00 21 */	blrl 
/* 800E2498 000DF3D8  38 00 00 00 */	li r0, 0
/* 800E249C 000DF3DC  90 01 00 44 */	stw r0, 0x44(r1)
/* 800E24A0 000DF3E0  54 60 06 3E */	clrlwi r0, r3, 0x18
/* 800E24A4 000DF3E4  90 01 00 48 */	stw r0, 0x48(r1)
/* 800E24A8 000DF3E8  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E24AC 000DF3EC  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800E24B0 000DF3F0  7C 03 00 00 */	cmpw r3, r0
/* 800E24B4 000DF3F4  41 80 00 18 */	blt lbl_800E24CC
/* 800E24B8 000DF3F8  3C 60 80 37 */	lis r3, $$23834@ha
/* 800E24BC 000DF3FC  4C C6 31 82 */	crclr 6
/* 800E24C0 000DF400  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800E24C4 000DF404  4B FF AB E1 */	bl SpcTrace__FPCce
/* 800E24C8 000DF408  48 00 00 2C */	b lbl_800E24F4
lbl_800E24CC:
/* 800E24CC 000DF40C  80 BF 00 20 */	lwz r5, 0x20(r31)
/* 800E24D0 000DF410  54 64 18 38 */	slwi r4, r3, 3
/* 800E24D4 000DF414  80 61 00 44 */	lwz r3, 0x44(r1)
/* 800E24D8 000DF418  80 01 00 48 */	lwz r0, 0x48(r1)
/* 800E24DC 000DF41C  7C 85 22 14 */	add r4, r5, r4
/* 800E24E0 000DF420  90 64 00 00 */	stw r3, 0(r4)
/* 800E24E4 000DF424  90 04 00 04 */	stw r0, 4(r4)
/* 800E24E8 000DF428  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E24EC 000DF42C  38 03 00 01 */	addi r0, r3, 1
/* 800E24F0 000DF430  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800E24F4:
/* 800E24F4 000DF434  80 01 00 84 */	lwz r0, 0x84(r1)
/* 800E24F8 000DF438  83 E1 00 7C */	lwz r31, 0x7c(r1)
/* 800E24FC 000DF43C  83 C1 00 78 */	lwz r30, 0x78(r1)
/* 800E2500 000DF440  7C 08 03 A6 */	mtlr r0
/* 800E2504 000DF444  83 A1 00 74 */	lwz r29, 0x74(r1)
/* 800E2508 000DF448  38 21 00 80 */	addi r1, r1, 0x80
/* 800E250C 000DF44C  4E 80 00 20 */	blr 

.global evStartOpenModelGate__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evStartOpenModelGate__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E2510 000DF450  7C 08 02 A6 */	mflr r0
/* 800E2514 000DF454  90 01 00 04 */	stw r0, 4(r1)
/* 800E2518 000DF458  94 21 FF 80 */	stwu r1, -0x80(r1)
/* 800E251C 000DF45C  93 E1 00 7C */	stw r31, 0x7c(r1)
/* 800E2520 000DF460  7C 7F 1B 78 */	mr r31, r3
/* 800E2524 000DF464  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E2528 000DF468  93 C1 00 78 */	stw r30, 0x78(r1)
/* 800E252C 000DF46C  93 A1 00 74 */	stw r29, 0x74(r1)
/* 800E2530 000DF470  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E2534 000DF474  38 80 00 01 */	li r4, 1
/* 800E2538 000DF478  48 03 4E F9 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E253C 000DF47C  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E2540 000DF480  2C 03 00 00 */	cmpwi r3, 0
/* 800E2544 000DF484  41 81 00 38 */	bgt lbl_800E257C
/* 800E2548 000DF488  3C 60 80 37 */	lis r3, $$23829@ha
/* 800E254C 000DF48C  4C C6 31 82 */	crclr 6
/* 800E2550 000DF490  38 63 30 FC */	addi r3, r3, $$23829@l
/* 800E2554 000DF494  4B FF AB 51 */	bl SpcTrace__FPCce
/* 800E2558 000DF498  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800E255C 000DF49C  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800E2560 000DF4A0  54 00 18 38 */	slwi r0, r0, 3
/* 800E2564 000DF4A4  7C 63 02 14 */	add r3, r3, r0
/* 800E2568 000DF4A8  80 03 00 00 */	lwz r0, 0(r3)
/* 800E256C 000DF4AC  90 01 00 54 */	stw r0, 0x54(r1)
/* 800E2570 000DF4B0  80 03 00 04 */	lwz r0, 4(r3)
/* 800E2574 000DF4B4  90 01 00 58 */	stw r0, 0x58(r1)
/* 800E2578 000DF4B8  48 00 00 2C */	b lbl_800E25A4
lbl_800E257C:
/* 800E257C 000DF4BC  38 03 FF FF */	addi r0, r3, -1
/* 800E2580 000DF4C0  90 1F 00 1C */	stw r0, 0x1c(r31)
/* 800E2584 000DF4C4  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800E2588 000DF4C8  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800E258C 000DF4CC  54 00 18 38 */	slwi r0, r0, 3
/* 800E2590 000DF4D0  7C 63 02 14 */	add r3, r3, r0
/* 800E2594 000DF4D4  80 03 00 00 */	lwz r0, 0(r3)
/* 800E2598 000DF4D8  90 01 00 54 */	stw r0, 0x54(r1)
/* 800E259C 000DF4DC  80 03 00 04 */	lwz r0, 4(r3)
/* 800E25A0 000DF4E0  90 01 00 58 */	stw r0, 0x58(r1)
lbl_800E25A4:
/* 800E25A4 000DF4E4  80 81 00 54 */	lwz r4, 0x54(r1)
/* 800E25A8 000DF4E8  38 60 00 00 */	li r3, 0
/* 800E25AC 000DF4EC  80 01 00 58 */	lwz r0, 0x58(r1)
/* 800E25B0 000DF4F0  90 81 00 5C */	stw r4, 0x5c(r1)
/* 800E25B4 000DF4F4  90 01 00 60 */	stw r0, 0x60(r1)
/* 800E25B8 000DF4F8  80 01 00 5C */	lwz r0, 0x5c(r1)
/* 800E25BC 000DF4FC  2C 00 00 01 */	cmpwi r0, 1
/* 800E25C0 000DF500  41 82 00 9C */	beq lbl_800E265C
/* 800E25C4 000DF504  40 80 00 10 */	bge lbl_800E25D4
/* 800E25C8 000DF508  2C 00 00 00 */	cmpwi r0, 0
/* 800E25CC 000DF50C  40 80 00 58 */	bge lbl_800E2624
/* 800E25D0 000DF510  48 00 00 8C */	b lbl_800E265C
lbl_800E25D4:
/* 800E25D4 000DF514  2C 00 00 03 */	cmpwi r0, 3
/* 800E25D8 000DF518  40 80 00 84 */	bge lbl_800E265C
/* 800E25DC 000DF51C  2C 00 00 02 */	cmpwi r0, 2
/* 800E25E0 000DF520  41 82 00 08 */	beq lbl_800E25E8
/* 800E25E4 000DF524  48 00 00 0C */	b lbl_800E25F0
lbl_800E25E8:
/* 800E25E8 000DF528  83 C1 00 60 */	lwz r30, 0x60(r1)
/* 800E25EC 000DF52C  48 00 00 08 */	b lbl_800E25F4
lbl_800E25F0:
/* 800E25F0 000DF530  3B C2 8F F0 */	addi r30, r2, $$24053@sda21
lbl_800E25F4:
/* 800E25F4 000DF534  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 800E25F8 000DF538  7F C3 F3 78 */	mr r3, r30
/* 800E25FC 000DF53C  83 A4 00 04 */	lwz r29, 4(r4)
/* 800E2600 000DF540  4B F6 20 8D */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800E2604 000DF544  81 9D 00 00 */	lwz r12, 0(r29)
/* 800E2608 000DF548  38 83 00 00 */	addi r4, r3, 0
/* 800E260C 000DF54C  38 7D 00 00 */	addi r3, r29, 0
/* 800E2610 000DF550  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 800E2614 000DF554  7F C5 F3 78 */	mr r5, r30
/* 800E2618 000DF558  7D 88 03 A6 */	mtlr r12
/* 800E261C 000DF55C  4E 80 00 21 */	blrl 
/* 800E2620 000DF560  48 00 00 3C */	b lbl_800E265C
lbl_800E2624:
/* 800E2624 000DF564  2C 00 00 01 */	cmpwi r0, 1
/* 800E2628 000DF568  41 82 00 1C */	beq lbl_800E2644
/* 800E262C 000DF56C  40 80 00 2C */	bge lbl_800E2658
/* 800E2630 000DF570  2C 00 00 00 */	cmpwi r0, 0
/* 800E2634 000DF574  40 80 00 08 */	bge lbl_800E263C
/* 800E2638 000DF578  48 00 00 20 */	b lbl_800E2658
lbl_800E263C:
/* 800E263C 000DF57C  80 61 00 60 */	lwz r3, 0x60(r1)
/* 800E2640 000DF580  48 00 00 1C */	b lbl_800E265C
lbl_800E2644:
/* 800E2644 000DF584  C0 01 00 60 */	lfs f0, 0x60(r1)
/* 800E2648 000DF588  FC 00 00 1E */	fctiwz f0, f0
/* 800E264C 000DF58C  D8 01 00 68 */	stfd f0, 0x68(r1)
/* 800E2650 000DF590  80 61 00 6C */	lwz r3, 0x6c(r1)
/* 800E2654 000DF594  48 00 00 08 */	b lbl_800E265C
lbl_800E2658:
/* 800E2658 000DF598  38 60 00 00 */	li r3, 0
lbl_800E265C:
/* 800E265C 000DF59C  48 0E 12 A1 */	bl startOpen__10TModelGateFv
/* 800E2660 000DF5A0  38 00 00 00 */	li r0, 0
/* 800E2664 000DF5A4  90 01 00 44 */	stw r0, 0x44(r1)
/* 800E2668 000DF5A8  90 01 00 48 */	stw r0, 0x48(r1)
/* 800E266C 000DF5AC  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E2670 000DF5B0  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800E2674 000DF5B4  7C 03 00 00 */	cmpw r3, r0
/* 800E2678 000DF5B8  41 80 00 18 */	blt lbl_800E2690
/* 800E267C 000DF5BC  3C 60 80 37 */	lis r3, $$23834@ha
/* 800E2680 000DF5C0  4C C6 31 82 */	crclr 6
/* 800E2684 000DF5C4  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800E2688 000DF5C8  4B FF AA 1D */	bl SpcTrace__FPCce
/* 800E268C 000DF5CC  48 00 00 2C */	b lbl_800E26B8
lbl_800E2690:
/* 800E2690 000DF5D0  80 BF 00 20 */	lwz r5, 0x20(r31)
/* 800E2694 000DF5D4  54 64 18 38 */	slwi r4, r3, 3
/* 800E2698 000DF5D8  80 61 00 44 */	lwz r3, 0x44(r1)
/* 800E269C 000DF5DC  80 01 00 48 */	lwz r0, 0x48(r1)
/* 800E26A0 000DF5E0  7C 85 22 14 */	add r4, r5, r4
/* 800E26A4 000DF5E4  90 64 00 00 */	stw r3, 0(r4)
/* 800E26A8 000DF5E8  90 04 00 04 */	stw r0, 4(r4)
/* 800E26AC 000DF5EC  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E26B0 000DF5F0  38 03 00 01 */	addi r0, r3, 1
/* 800E26B4 000DF5F4  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800E26B8:
/* 800E26B8 000DF5F8  80 01 00 84 */	lwz r0, 0x84(r1)
/* 800E26BC 000DF5FC  83 E1 00 7C */	lwz r31, 0x7c(r1)
/* 800E26C0 000DF600  83 C1 00 78 */	lwz r30, 0x78(r1)
/* 800E26C4 000DF604  7C 08 03 A6 */	mtlr r0
/* 800E26C8 000DF608  83 A1 00 74 */	lwz r29, 0x74(r1)
/* 800E26CC 000DF60C  38 21 00 80 */	addi r1, r1, 0x80
/* 800E26D0 000DF610  4E 80 00 20 */	blr 

.global evStartEMarioDisappear__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evStartEMarioDisappear__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E26D4 000DF614  7C 08 02 A6 */	mflr r0
/* 800E26D8 000DF618  90 01 00 04 */	stw r0, 4(r1)
/* 800E26DC 000DF61C  94 21 FF 80 */	stwu r1, -0x80(r1)
/* 800E26E0 000DF620  93 E1 00 7C */	stw r31, 0x7c(r1)
/* 800E26E4 000DF624  7C 7F 1B 78 */	mr r31, r3
/* 800E26E8 000DF628  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E26EC 000DF62C  93 C1 00 78 */	stw r30, 0x78(r1)
/* 800E26F0 000DF630  93 A1 00 74 */	stw r29, 0x74(r1)
/* 800E26F4 000DF634  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E26F8 000DF638  38 80 00 01 */	li r4, 1
/* 800E26FC 000DF63C  48 03 4D 35 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E2700 000DF640  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E2704 000DF644  2C 03 00 00 */	cmpwi r3, 0
/* 800E2708 000DF648  41 81 00 38 */	bgt lbl_800E2740
/* 800E270C 000DF64C  3C 60 80 37 */	lis r3, $$23829@ha
/* 800E2710 000DF650  4C C6 31 82 */	crclr 6
/* 800E2714 000DF654  38 63 30 FC */	addi r3, r3, $$23829@l
/* 800E2718 000DF658  4B FF A9 8D */	bl SpcTrace__FPCce
/* 800E271C 000DF65C  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800E2720 000DF660  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800E2724 000DF664  54 00 18 38 */	slwi r0, r0, 3
/* 800E2728 000DF668  7C 63 02 14 */	add r3, r3, r0
/* 800E272C 000DF66C  80 03 00 00 */	lwz r0, 0(r3)
/* 800E2730 000DF670  90 01 00 54 */	stw r0, 0x54(r1)
/* 800E2734 000DF674  80 03 00 04 */	lwz r0, 4(r3)
/* 800E2738 000DF678  90 01 00 58 */	stw r0, 0x58(r1)
/* 800E273C 000DF67C  48 00 00 2C */	b lbl_800E2768
lbl_800E2740:
/* 800E2740 000DF680  38 03 FF FF */	addi r0, r3, -1
/* 800E2744 000DF684  90 1F 00 1C */	stw r0, 0x1c(r31)
/* 800E2748 000DF688  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800E274C 000DF68C  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800E2750 000DF690  54 00 18 38 */	slwi r0, r0, 3
/* 800E2754 000DF694  7C 63 02 14 */	add r3, r3, r0
/* 800E2758 000DF698  80 03 00 00 */	lwz r0, 0(r3)
/* 800E275C 000DF69C  90 01 00 54 */	stw r0, 0x54(r1)
/* 800E2760 000DF6A0  80 03 00 04 */	lwz r0, 4(r3)
/* 800E2764 000DF6A4  90 01 00 58 */	stw r0, 0x58(r1)
lbl_800E2768:
/* 800E2768 000DF6A8  80 81 00 54 */	lwz r4, 0x54(r1)
/* 800E276C 000DF6AC  38 60 00 00 */	li r3, 0
/* 800E2770 000DF6B0  80 01 00 58 */	lwz r0, 0x58(r1)
/* 800E2774 000DF6B4  90 81 00 5C */	stw r4, 0x5c(r1)
/* 800E2778 000DF6B8  90 01 00 60 */	stw r0, 0x60(r1)
/* 800E277C 000DF6BC  80 01 00 5C */	lwz r0, 0x5c(r1)
/* 800E2780 000DF6C0  2C 00 00 01 */	cmpwi r0, 1
/* 800E2784 000DF6C4  41 82 00 9C */	beq lbl_800E2820
/* 800E2788 000DF6C8  40 80 00 10 */	bge lbl_800E2798
/* 800E278C 000DF6CC  2C 00 00 00 */	cmpwi r0, 0
/* 800E2790 000DF6D0  40 80 00 58 */	bge lbl_800E27E8
/* 800E2794 000DF6D4  48 00 00 8C */	b lbl_800E2820
lbl_800E2798:
/* 800E2798 000DF6D8  2C 00 00 03 */	cmpwi r0, 3
/* 800E279C 000DF6DC  40 80 00 84 */	bge lbl_800E2820
/* 800E27A0 000DF6E0  2C 00 00 02 */	cmpwi r0, 2
/* 800E27A4 000DF6E4  41 82 00 08 */	beq lbl_800E27AC
/* 800E27A8 000DF6E8  48 00 00 0C */	b lbl_800E27B4
lbl_800E27AC:
/* 800E27AC 000DF6EC  83 C1 00 60 */	lwz r30, 0x60(r1)
/* 800E27B0 000DF6F0  48 00 00 08 */	b lbl_800E27B8
lbl_800E27B4:
/* 800E27B4 000DF6F4  3B C2 8F F0 */	addi r30, r2, $$24053@sda21
lbl_800E27B8:
/* 800E27B8 000DF6F8  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 800E27BC 000DF6FC  7F C3 F3 78 */	mr r3, r30
/* 800E27C0 000DF700  83 A4 00 04 */	lwz r29, 4(r4)
/* 800E27C4 000DF704  4B F6 1E C9 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800E27C8 000DF708  81 9D 00 00 */	lwz r12, 0(r29)
/* 800E27CC 000DF70C  38 83 00 00 */	addi r4, r3, 0
/* 800E27D0 000DF710  38 7D 00 00 */	addi r3, r29, 0
/* 800E27D4 000DF714  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 800E27D8 000DF718  7F C5 F3 78 */	mr r5, r30
/* 800E27DC 000DF71C  7D 88 03 A6 */	mtlr r12
/* 800E27E0 000DF720  4E 80 00 21 */	blrl 
/* 800E27E4 000DF724  48 00 00 3C */	b lbl_800E2820
lbl_800E27E8:
/* 800E27E8 000DF728  2C 00 00 01 */	cmpwi r0, 1
/* 800E27EC 000DF72C  41 82 00 1C */	beq lbl_800E2808
/* 800E27F0 000DF730  40 80 00 2C */	bge lbl_800E281C
/* 800E27F4 000DF734  2C 00 00 00 */	cmpwi r0, 0
/* 800E27F8 000DF738  40 80 00 08 */	bge lbl_800E2800
/* 800E27FC 000DF73C  48 00 00 20 */	b lbl_800E281C
lbl_800E2800:
/* 800E2800 000DF740  80 61 00 60 */	lwz r3, 0x60(r1)
/* 800E2804 000DF744  48 00 00 1C */	b lbl_800E2820
lbl_800E2808:
/* 800E2808 000DF748  C0 01 00 60 */	lfs f0, 0x60(r1)
/* 800E280C 000DF74C  FC 00 00 1E */	fctiwz f0, f0
/* 800E2810 000DF750  D8 01 00 68 */	stfd f0, 0x68(r1)
/* 800E2814 000DF754  80 61 00 6C */	lwz r3, 0x6c(r1)
/* 800E2818 000DF758  48 00 00 08 */	b lbl_800E2820
lbl_800E281C:
/* 800E281C 000DF75C  38 60 00 00 */	li r3, 0
lbl_800E2820:
/* 800E2820 000DF760  48 16 A9 19 */	bl forceDisappear__7TEMarioFv
/* 800E2824 000DF764  38 00 00 00 */	li r0, 0
/* 800E2828 000DF768  90 01 00 44 */	stw r0, 0x44(r1)
/* 800E282C 000DF76C  90 01 00 48 */	stw r0, 0x48(r1)
/* 800E2830 000DF770  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E2834 000DF774  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800E2838 000DF778  7C 03 00 00 */	cmpw r3, r0
/* 800E283C 000DF77C  41 80 00 18 */	blt lbl_800E2854
/* 800E2840 000DF780  3C 60 80 37 */	lis r3, $$23834@ha
/* 800E2844 000DF784  4C C6 31 82 */	crclr 6
/* 800E2848 000DF788  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800E284C 000DF78C  4B FF A8 59 */	bl SpcTrace__FPCce
/* 800E2850 000DF790  48 00 00 2C */	b lbl_800E287C
lbl_800E2854:
/* 800E2854 000DF794  80 BF 00 20 */	lwz r5, 0x20(r31)
/* 800E2858 000DF798  54 64 18 38 */	slwi r4, r3, 3
/* 800E285C 000DF79C  80 61 00 44 */	lwz r3, 0x44(r1)
/* 800E2860 000DF7A0  80 01 00 48 */	lwz r0, 0x48(r1)
/* 800E2864 000DF7A4  7C 85 22 14 */	add r4, r5, r4
/* 800E2868 000DF7A8  90 64 00 00 */	stw r3, 0(r4)
/* 800E286C 000DF7AC  90 04 00 04 */	stw r0, 4(r4)
/* 800E2870 000DF7B0  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E2874 000DF7B4  38 03 00 01 */	addi r0, r3, 1
/* 800E2878 000DF7B8  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800E287C:
/* 800E287C 000DF7BC  80 01 00 84 */	lwz r0, 0x84(r1)
/* 800E2880 000DF7C0  83 E1 00 7C */	lwz r31, 0x7c(r1)
/* 800E2884 000DF7C4  83 C1 00 78 */	lwz r30, 0x78(r1)
/* 800E2888 000DF7C8  7C 08 03 A6 */	mtlr r0
/* 800E288C 000DF7CC  83 A1 00 74 */	lwz r29, 0x74(r1)
/* 800E2890 000DF7D0  38 21 00 80 */	addi r1, r1, 0x80
/* 800E2894 000DF7D4  4E 80 00 20 */	blr 

.global evStartEMarioGateDrawing__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evStartEMarioGateDrawing__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E2898 000DF7D8  7C 08 02 A6 */	mflr r0
/* 800E289C 000DF7DC  90 01 00 04 */	stw r0, 4(r1)
/* 800E28A0 000DF7E0  94 21 FF 80 */	stwu r1, -0x80(r1)
/* 800E28A4 000DF7E4  93 E1 00 7C */	stw r31, 0x7c(r1)
/* 800E28A8 000DF7E8  7C 7F 1B 78 */	mr r31, r3
/* 800E28AC 000DF7EC  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E28B0 000DF7F0  93 C1 00 78 */	stw r30, 0x78(r1)
/* 800E28B4 000DF7F4  93 A1 00 74 */	stw r29, 0x74(r1)
/* 800E28B8 000DF7F8  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E28BC 000DF7FC  38 80 00 01 */	li r4, 1
/* 800E28C0 000DF800  48 03 4B 71 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E28C4 000DF804  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E28C8 000DF808  2C 03 00 00 */	cmpwi r3, 0
/* 800E28CC 000DF80C  41 81 00 38 */	bgt lbl_800E2904
/* 800E28D0 000DF810  3C 60 80 37 */	lis r3, $$23829@ha
/* 800E28D4 000DF814  4C C6 31 82 */	crclr 6
/* 800E28D8 000DF818  38 63 30 FC */	addi r3, r3, $$23829@l
/* 800E28DC 000DF81C  4B FF A7 C9 */	bl SpcTrace__FPCce
/* 800E28E0 000DF820  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800E28E4 000DF824  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800E28E8 000DF828  54 00 18 38 */	slwi r0, r0, 3
/* 800E28EC 000DF82C  7C 63 02 14 */	add r3, r3, r0
/* 800E28F0 000DF830  80 03 00 00 */	lwz r0, 0(r3)
/* 800E28F4 000DF834  90 01 00 54 */	stw r0, 0x54(r1)
/* 800E28F8 000DF838  80 03 00 04 */	lwz r0, 4(r3)
/* 800E28FC 000DF83C  90 01 00 58 */	stw r0, 0x58(r1)
/* 800E2900 000DF840  48 00 00 2C */	b lbl_800E292C
lbl_800E2904:
/* 800E2904 000DF844  38 03 FF FF */	addi r0, r3, -1
/* 800E2908 000DF848  90 1F 00 1C */	stw r0, 0x1c(r31)
/* 800E290C 000DF84C  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800E2910 000DF850  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800E2914 000DF854  54 00 18 38 */	slwi r0, r0, 3
/* 800E2918 000DF858  7C 63 02 14 */	add r3, r3, r0
/* 800E291C 000DF85C  80 03 00 00 */	lwz r0, 0(r3)
/* 800E2920 000DF860  90 01 00 54 */	stw r0, 0x54(r1)
/* 800E2924 000DF864  80 03 00 04 */	lwz r0, 4(r3)
/* 800E2928 000DF868  90 01 00 58 */	stw r0, 0x58(r1)
lbl_800E292C:
/* 800E292C 000DF86C  80 81 00 54 */	lwz r4, 0x54(r1)
/* 800E2930 000DF870  38 60 00 00 */	li r3, 0
/* 800E2934 000DF874  80 01 00 58 */	lwz r0, 0x58(r1)
/* 800E2938 000DF878  90 81 00 5C */	stw r4, 0x5c(r1)
/* 800E293C 000DF87C  90 01 00 60 */	stw r0, 0x60(r1)
/* 800E2940 000DF880  80 01 00 5C */	lwz r0, 0x5c(r1)
/* 800E2944 000DF884  2C 00 00 01 */	cmpwi r0, 1
/* 800E2948 000DF888  41 82 00 9C */	beq lbl_800E29E4
/* 800E294C 000DF88C  40 80 00 10 */	bge lbl_800E295C
/* 800E2950 000DF890  2C 00 00 00 */	cmpwi r0, 0
/* 800E2954 000DF894  40 80 00 58 */	bge lbl_800E29AC
/* 800E2958 000DF898  48 00 00 8C */	b lbl_800E29E4
lbl_800E295C:
/* 800E295C 000DF89C  2C 00 00 03 */	cmpwi r0, 3
/* 800E2960 000DF8A0  40 80 00 84 */	bge lbl_800E29E4
/* 800E2964 000DF8A4  2C 00 00 02 */	cmpwi r0, 2
/* 800E2968 000DF8A8  41 82 00 08 */	beq lbl_800E2970
/* 800E296C 000DF8AC  48 00 00 0C */	b lbl_800E2978
lbl_800E2970:
/* 800E2970 000DF8B0  83 C1 00 60 */	lwz r30, 0x60(r1)
/* 800E2974 000DF8B4  48 00 00 08 */	b lbl_800E297C
lbl_800E2978:
/* 800E2978 000DF8B8  3B C2 8F F0 */	addi r30, r2, $$24053@sda21
lbl_800E297C:
/* 800E297C 000DF8BC  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 800E2980 000DF8C0  7F C3 F3 78 */	mr r3, r30
/* 800E2984 000DF8C4  83 A4 00 04 */	lwz r29, 4(r4)
/* 800E2988 000DF8C8  4B F6 1D 05 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800E298C 000DF8CC  81 9D 00 00 */	lwz r12, 0(r29)
/* 800E2990 000DF8D0  38 83 00 00 */	addi r4, r3, 0
/* 800E2994 000DF8D4  38 7D 00 00 */	addi r3, r29, 0
/* 800E2998 000DF8D8  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 800E299C 000DF8DC  7F C5 F3 78 */	mr r5, r30
/* 800E29A0 000DF8E0  7D 88 03 A6 */	mtlr r12
/* 800E29A4 000DF8E4  4E 80 00 21 */	blrl 
/* 800E29A8 000DF8E8  48 00 00 3C */	b lbl_800E29E4
lbl_800E29AC:
/* 800E29AC 000DF8EC  2C 00 00 01 */	cmpwi r0, 1
/* 800E29B0 000DF8F0  41 82 00 1C */	beq lbl_800E29CC
/* 800E29B4 000DF8F4  40 80 00 2C */	bge lbl_800E29E0
/* 800E29B8 000DF8F8  2C 00 00 00 */	cmpwi r0, 0
/* 800E29BC 000DF8FC  40 80 00 08 */	bge lbl_800E29C4
/* 800E29C0 000DF900  48 00 00 20 */	b lbl_800E29E0
lbl_800E29C4:
/* 800E29C4 000DF904  80 61 00 60 */	lwz r3, 0x60(r1)
/* 800E29C8 000DF908  48 00 00 1C */	b lbl_800E29E4
lbl_800E29CC:
/* 800E29CC 000DF90C  C0 01 00 60 */	lfs f0, 0x60(r1)
/* 800E29D0 000DF910  FC 00 00 1E */	fctiwz f0, f0
/* 800E29D4 000DF914  D8 01 00 68 */	stfd f0, 0x68(r1)
/* 800E29D8 000DF918  80 61 00 6C */	lwz r3, 0x6c(r1)
/* 800E29DC 000DF91C  48 00 00 08 */	b lbl_800E29E4
lbl_800E29E0:
/* 800E29E0 000DF920  38 60 00 00 */	li r3, 0
lbl_800E29E4:
/* 800E29E4 000DF924  48 16 A7 7D */	bl startGateDrawing__7TEMarioFv
/* 800E29E8 000DF928  38 00 00 00 */	li r0, 0
/* 800E29EC 000DF92C  90 01 00 44 */	stw r0, 0x44(r1)
/* 800E29F0 000DF930  90 01 00 48 */	stw r0, 0x48(r1)
/* 800E29F4 000DF934  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E29F8 000DF938  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800E29FC 000DF93C  7C 03 00 00 */	cmpw r3, r0
/* 800E2A00 000DF940  41 80 00 18 */	blt lbl_800E2A18
/* 800E2A04 000DF944  3C 60 80 37 */	lis r3, $$23834@ha
/* 800E2A08 000DF948  4C C6 31 82 */	crclr 6
/* 800E2A0C 000DF94C  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800E2A10 000DF950  4B FF A6 95 */	bl SpcTrace__FPCce
/* 800E2A14 000DF954  48 00 00 2C */	b lbl_800E2A40
lbl_800E2A18:
/* 800E2A18 000DF958  80 BF 00 20 */	lwz r5, 0x20(r31)
/* 800E2A1C 000DF95C  54 64 18 38 */	slwi r4, r3, 3
/* 800E2A20 000DF960  80 61 00 44 */	lwz r3, 0x44(r1)
/* 800E2A24 000DF964  80 01 00 48 */	lwz r0, 0x48(r1)
/* 800E2A28 000DF968  7C 85 22 14 */	add r4, r5, r4
/* 800E2A2C 000DF96C  90 64 00 00 */	stw r3, 0(r4)
/* 800E2A30 000DF970  90 04 00 04 */	stw r0, 4(r4)
/* 800E2A34 000DF974  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E2A38 000DF978  38 03 00 01 */	addi r0, r3, 1
/* 800E2A3C 000DF97C  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800E2A40:
/* 800E2A40 000DF980  80 01 00 84 */	lwz r0, 0x84(r1)
/* 800E2A44 000DF984  83 E1 00 7C */	lwz r31, 0x7c(r1)
/* 800E2A48 000DF988  83 C1 00 78 */	lwz r30, 0x78(r1)
/* 800E2A4C 000DF98C  7C 08 03 A6 */	mtlr r0
/* 800E2A50 000DF990  83 A1 00 74 */	lwz r29, 0x74(r1)
/* 800E2A54 000DF994  38 21 00 80 */	addi r1, r1, 0x80
/* 800E2A58 000DF998  4E 80 00 20 */	blr 

.global evStartEMarioRunAway__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evStartEMarioRunAway__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E2A5C 000DF99C  7C 08 02 A6 */	mflr r0
/* 800E2A60 000DF9A0  90 01 00 04 */	stw r0, 4(r1)
/* 800E2A64 000DF9A4  94 21 FF 80 */	stwu r1, -0x80(r1)
/* 800E2A68 000DF9A8  93 E1 00 7C */	stw r31, 0x7c(r1)
/* 800E2A6C 000DF9AC  7C 7F 1B 78 */	mr r31, r3
/* 800E2A70 000DF9B0  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E2A74 000DF9B4  93 C1 00 78 */	stw r30, 0x78(r1)
/* 800E2A78 000DF9B8  93 A1 00 74 */	stw r29, 0x74(r1)
/* 800E2A7C 000DF9BC  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E2A80 000DF9C0  38 80 00 01 */	li r4, 1
/* 800E2A84 000DF9C4  48 03 49 AD */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E2A88 000DF9C8  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E2A8C 000DF9CC  2C 03 00 00 */	cmpwi r3, 0
/* 800E2A90 000DF9D0  41 81 00 38 */	bgt lbl_800E2AC8
/* 800E2A94 000DF9D4  3C 60 80 37 */	lis r3, $$23829@ha
/* 800E2A98 000DF9D8  4C C6 31 82 */	crclr 6
/* 800E2A9C 000DF9DC  38 63 30 FC */	addi r3, r3, $$23829@l
/* 800E2AA0 000DF9E0  4B FF A6 05 */	bl SpcTrace__FPCce
/* 800E2AA4 000DF9E4  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800E2AA8 000DF9E8  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800E2AAC 000DF9EC  54 00 18 38 */	slwi r0, r0, 3
/* 800E2AB0 000DF9F0  7C 63 02 14 */	add r3, r3, r0
/* 800E2AB4 000DF9F4  80 03 00 00 */	lwz r0, 0(r3)
/* 800E2AB8 000DF9F8  90 01 00 54 */	stw r0, 0x54(r1)
/* 800E2ABC 000DF9FC  80 03 00 04 */	lwz r0, 4(r3)
/* 800E2AC0 000DFA00  90 01 00 58 */	stw r0, 0x58(r1)
/* 800E2AC4 000DFA04  48 00 00 2C */	b lbl_800E2AF0
lbl_800E2AC8:
/* 800E2AC8 000DFA08  38 03 FF FF */	addi r0, r3, -1
/* 800E2ACC 000DFA0C  90 1F 00 1C */	stw r0, 0x1c(r31)
/* 800E2AD0 000DFA10  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800E2AD4 000DFA14  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800E2AD8 000DFA18  54 00 18 38 */	slwi r0, r0, 3
/* 800E2ADC 000DFA1C  7C 63 02 14 */	add r3, r3, r0
/* 800E2AE0 000DFA20  80 03 00 00 */	lwz r0, 0(r3)
/* 800E2AE4 000DFA24  90 01 00 54 */	stw r0, 0x54(r1)
/* 800E2AE8 000DFA28  80 03 00 04 */	lwz r0, 4(r3)
/* 800E2AEC 000DFA2C  90 01 00 58 */	stw r0, 0x58(r1)
lbl_800E2AF0:
/* 800E2AF0 000DFA30  80 81 00 54 */	lwz r4, 0x54(r1)
/* 800E2AF4 000DFA34  38 60 00 00 */	li r3, 0
/* 800E2AF8 000DFA38  80 01 00 58 */	lwz r0, 0x58(r1)
/* 800E2AFC 000DFA3C  90 81 00 5C */	stw r4, 0x5c(r1)
/* 800E2B00 000DFA40  90 01 00 60 */	stw r0, 0x60(r1)
/* 800E2B04 000DFA44  80 01 00 5C */	lwz r0, 0x5c(r1)
/* 800E2B08 000DFA48  2C 00 00 01 */	cmpwi r0, 1
/* 800E2B0C 000DFA4C  41 82 00 9C */	beq lbl_800E2BA8
/* 800E2B10 000DFA50  40 80 00 10 */	bge lbl_800E2B20
/* 800E2B14 000DFA54  2C 00 00 00 */	cmpwi r0, 0
/* 800E2B18 000DFA58  40 80 00 58 */	bge lbl_800E2B70
/* 800E2B1C 000DFA5C  48 00 00 8C */	b lbl_800E2BA8
lbl_800E2B20:
/* 800E2B20 000DFA60  2C 00 00 03 */	cmpwi r0, 3
/* 800E2B24 000DFA64  40 80 00 84 */	bge lbl_800E2BA8
/* 800E2B28 000DFA68  2C 00 00 02 */	cmpwi r0, 2
/* 800E2B2C 000DFA6C  41 82 00 08 */	beq lbl_800E2B34
/* 800E2B30 000DFA70  48 00 00 0C */	b lbl_800E2B3C
lbl_800E2B34:
/* 800E2B34 000DFA74  83 C1 00 60 */	lwz r30, 0x60(r1)
/* 800E2B38 000DFA78  48 00 00 08 */	b lbl_800E2B40
lbl_800E2B3C:
/* 800E2B3C 000DFA7C  3B C2 8F F0 */	addi r30, r2, $$24053@sda21
lbl_800E2B40:
/* 800E2B40 000DFA80  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 800E2B44 000DFA84  7F C3 F3 78 */	mr r3, r30
/* 800E2B48 000DFA88  83 A4 00 04 */	lwz r29, 4(r4)
/* 800E2B4C 000DFA8C  4B F6 1B 41 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800E2B50 000DFA90  81 9D 00 00 */	lwz r12, 0(r29)
/* 800E2B54 000DFA94  38 83 00 00 */	addi r4, r3, 0
/* 800E2B58 000DFA98  38 7D 00 00 */	addi r3, r29, 0
/* 800E2B5C 000DFA9C  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 800E2B60 000DFAA0  7F C5 F3 78 */	mr r5, r30
/* 800E2B64 000DFAA4  7D 88 03 A6 */	mtlr r12
/* 800E2B68 000DFAA8  4E 80 00 21 */	blrl 
/* 800E2B6C 000DFAAC  48 00 00 3C */	b lbl_800E2BA8
lbl_800E2B70:
/* 800E2B70 000DFAB0  2C 00 00 01 */	cmpwi r0, 1
/* 800E2B74 000DFAB4  41 82 00 1C */	beq lbl_800E2B90
/* 800E2B78 000DFAB8  40 80 00 2C */	bge lbl_800E2BA4
/* 800E2B7C 000DFABC  2C 00 00 00 */	cmpwi r0, 0
/* 800E2B80 000DFAC0  40 80 00 08 */	bge lbl_800E2B88
/* 800E2B84 000DFAC4  48 00 00 20 */	b lbl_800E2BA4
lbl_800E2B88:
/* 800E2B88 000DFAC8  80 61 00 60 */	lwz r3, 0x60(r1)
/* 800E2B8C 000DFACC  48 00 00 1C */	b lbl_800E2BA8
lbl_800E2B90:
/* 800E2B90 000DFAD0  C0 01 00 60 */	lfs f0, 0x60(r1)
/* 800E2B94 000DFAD4  FC 00 00 1E */	fctiwz f0, f0
/* 800E2B98 000DFAD8  D8 01 00 68 */	stfd f0, 0x68(r1)
/* 800E2B9C 000DFADC  80 61 00 6C */	lwz r3, 0x6c(r1)
/* 800E2BA0 000DFAE0  48 00 00 08 */	b lbl_800E2BA8
lbl_800E2BA4:
/* 800E2BA4 000DFAE4  38 60 00 00 */	li r3, 0
lbl_800E2BA8:
/* 800E2BA8 000DFAE8  48 16 A6 01 */	bl startRunAway__7TEMarioFv
/* 800E2BAC 000DFAEC  38 00 00 00 */	li r0, 0
/* 800E2BB0 000DFAF0  90 01 00 44 */	stw r0, 0x44(r1)
/* 800E2BB4 000DFAF4  90 01 00 48 */	stw r0, 0x48(r1)
/* 800E2BB8 000DFAF8  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E2BBC 000DFAFC  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800E2BC0 000DFB00  7C 03 00 00 */	cmpw r3, r0
/* 800E2BC4 000DFB04  41 80 00 18 */	blt lbl_800E2BDC
/* 800E2BC8 000DFB08  3C 60 80 37 */	lis r3, $$23834@ha
/* 800E2BCC 000DFB0C  4C C6 31 82 */	crclr 6
/* 800E2BD0 000DFB10  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800E2BD4 000DFB14  4B FF A4 D1 */	bl SpcTrace__FPCce
/* 800E2BD8 000DFB18  48 00 00 2C */	b lbl_800E2C04
lbl_800E2BDC:
/* 800E2BDC 000DFB1C  80 BF 00 20 */	lwz r5, 0x20(r31)
/* 800E2BE0 000DFB20  54 64 18 38 */	slwi r4, r3, 3
/* 800E2BE4 000DFB24  80 61 00 44 */	lwz r3, 0x44(r1)
/* 800E2BE8 000DFB28  80 01 00 48 */	lwz r0, 0x48(r1)
/* 800E2BEC 000DFB2C  7C 85 22 14 */	add r4, r5, r4
/* 800E2BF0 000DFB30  90 64 00 00 */	stw r3, 0(r4)
/* 800E2BF4 000DFB34  90 04 00 04 */	stw r0, 4(r4)
/* 800E2BF8 000DFB38  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E2BFC 000DFB3C  38 03 00 01 */	addi r0, r3, 1
/* 800E2C00 000DFB40  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800E2C04:
/* 800E2C04 000DFB44  80 01 00 84 */	lwz r0, 0x84(r1)
/* 800E2C08 000DFB48  83 E1 00 7C */	lwz r31, 0x7c(r1)
/* 800E2C0C 000DFB4C  83 C1 00 78 */	lwz r30, 0x78(r1)
/* 800E2C10 000DFB50  7C 08 03 A6 */	mtlr r0
/* 800E2C14 000DFB54  83 A1 00 74 */	lwz r29, 0x74(r1)
/* 800E2C18 000DFB58  38 21 00 80 */	addi r1, r1, 0x80
/* 800E2C1C 000DFB5C  4E 80 00 20 */	blr 

.global evIsEMarioDownWaitingToTalk__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evIsEMarioDownWaitingToTalk__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E2C20 000DFB60  7C 08 02 A6 */	mflr r0
/* 800E2C24 000DFB64  90 01 00 04 */	stw r0, 4(r1)
/* 800E2C28 000DFB68  94 21 FF 80 */	stwu r1, -0x80(r1)
/* 800E2C2C 000DFB6C  93 E1 00 7C */	stw r31, 0x7c(r1)
/* 800E2C30 000DFB70  7C 7F 1B 78 */	mr r31, r3
/* 800E2C34 000DFB74  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E2C38 000DFB78  93 C1 00 78 */	stw r30, 0x78(r1)
/* 800E2C3C 000DFB7C  93 A1 00 74 */	stw r29, 0x74(r1)
/* 800E2C40 000DFB80  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E2C44 000DFB84  38 80 00 01 */	li r4, 1
/* 800E2C48 000DFB88  48 03 47 E9 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E2C4C 000DFB8C  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E2C50 000DFB90  2C 03 00 00 */	cmpwi r3, 0
/* 800E2C54 000DFB94  41 81 00 38 */	bgt lbl_800E2C8C
/* 800E2C58 000DFB98  3C 60 80 37 */	lis r3, $$23829@ha
/* 800E2C5C 000DFB9C  4C C6 31 82 */	crclr 6
/* 800E2C60 000DFBA0  38 63 30 FC */	addi r3, r3, $$23829@l
/* 800E2C64 000DFBA4  4B FF A4 41 */	bl SpcTrace__FPCce
/* 800E2C68 000DFBA8  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800E2C6C 000DFBAC  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800E2C70 000DFBB0  54 00 18 38 */	slwi r0, r0, 3
/* 800E2C74 000DFBB4  7C 63 02 14 */	add r3, r3, r0
/* 800E2C78 000DFBB8  80 03 00 00 */	lwz r0, 0(r3)
/* 800E2C7C 000DFBBC  90 01 00 54 */	stw r0, 0x54(r1)
/* 800E2C80 000DFBC0  80 03 00 04 */	lwz r0, 4(r3)
/* 800E2C84 000DFBC4  90 01 00 58 */	stw r0, 0x58(r1)
/* 800E2C88 000DFBC8  48 00 00 2C */	b lbl_800E2CB4
lbl_800E2C8C:
/* 800E2C8C 000DFBCC  38 03 FF FF */	addi r0, r3, -1
/* 800E2C90 000DFBD0  90 1F 00 1C */	stw r0, 0x1c(r31)
/* 800E2C94 000DFBD4  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800E2C98 000DFBD8  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800E2C9C 000DFBDC  54 00 18 38 */	slwi r0, r0, 3
/* 800E2CA0 000DFBE0  7C 63 02 14 */	add r3, r3, r0
/* 800E2CA4 000DFBE4  80 03 00 00 */	lwz r0, 0(r3)
/* 800E2CA8 000DFBE8  90 01 00 54 */	stw r0, 0x54(r1)
/* 800E2CAC 000DFBEC  80 03 00 04 */	lwz r0, 4(r3)
/* 800E2CB0 000DFBF0  90 01 00 58 */	stw r0, 0x58(r1)
lbl_800E2CB4:
/* 800E2CB4 000DFBF4  80 81 00 54 */	lwz r4, 0x54(r1)
/* 800E2CB8 000DFBF8  38 60 00 00 */	li r3, 0
/* 800E2CBC 000DFBFC  80 01 00 58 */	lwz r0, 0x58(r1)
/* 800E2CC0 000DFC00  90 81 00 5C */	stw r4, 0x5c(r1)
/* 800E2CC4 000DFC04  90 01 00 60 */	stw r0, 0x60(r1)
/* 800E2CC8 000DFC08  80 01 00 5C */	lwz r0, 0x5c(r1)
/* 800E2CCC 000DFC0C  2C 00 00 01 */	cmpwi r0, 1
/* 800E2CD0 000DFC10  41 82 00 9C */	beq lbl_800E2D6C
/* 800E2CD4 000DFC14  40 80 00 10 */	bge lbl_800E2CE4
/* 800E2CD8 000DFC18  2C 00 00 00 */	cmpwi r0, 0
/* 800E2CDC 000DFC1C  40 80 00 58 */	bge lbl_800E2D34
/* 800E2CE0 000DFC20  48 00 00 8C */	b lbl_800E2D6C
lbl_800E2CE4:
/* 800E2CE4 000DFC24  2C 00 00 03 */	cmpwi r0, 3
/* 800E2CE8 000DFC28  40 80 00 84 */	bge lbl_800E2D6C
/* 800E2CEC 000DFC2C  2C 00 00 02 */	cmpwi r0, 2
/* 800E2CF0 000DFC30  41 82 00 08 */	beq lbl_800E2CF8
/* 800E2CF4 000DFC34  48 00 00 0C */	b lbl_800E2D00
lbl_800E2CF8:
/* 800E2CF8 000DFC38  83 C1 00 60 */	lwz r30, 0x60(r1)
/* 800E2CFC 000DFC3C  48 00 00 08 */	b lbl_800E2D04
lbl_800E2D00:
/* 800E2D00 000DFC40  3B C2 8F F0 */	addi r30, r2, $$24053@sda21
lbl_800E2D04:
/* 800E2D04 000DFC44  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 800E2D08 000DFC48  7F C3 F3 78 */	mr r3, r30
/* 800E2D0C 000DFC4C  83 A4 00 04 */	lwz r29, 4(r4)
/* 800E2D10 000DFC50  4B F6 19 7D */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800E2D14 000DFC54  81 9D 00 00 */	lwz r12, 0(r29)
/* 800E2D18 000DFC58  38 83 00 00 */	addi r4, r3, 0
/* 800E2D1C 000DFC5C  38 7D 00 00 */	addi r3, r29, 0
/* 800E2D20 000DFC60  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 800E2D24 000DFC64  7F C5 F3 78 */	mr r5, r30
/* 800E2D28 000DFC68  7D 88 03 A6 */	mtlr r12
/* 800E2D2C 000DFC6C  4E 80 00 21 */	blrl 
/* 800E2D30 000DFC70  48 00 00 3C */	b lbl_800E2D6C
lbl_800E2D34:
/* 800E2D34 000DFC74  2C 00 00 01 */	cmpwi r0, 1
/* 800E2D38 000DFC78  41 82 00 1C */	beq lbl_800E2D54
/* 800E2D3C 000DFC7C  40 80 00 2C */	bge lbl_800E2D68
/* 800E2D40 000DFC80  2C 00 00 00 */	cmpwi r0, 0
/* 800E2D44 000DFC84  40 80 00 08 */	bge lbl_800E2D4C
/* 800E2D48 000DFC88  48 00 00 20 */	b lbl_800E2D68
lbl_800E2D4C:
/* 800E2D4C 000DFC8C  80 61 00 60 */	lwz r3, 0x60(r1)
/* 800E2D50 000DFC90  48 00 00 1C */	b lbl_800E2D6C
lbl_800E2D54:
/* 800E2D54 000DFC94  C0 01 00 60 */	lfs f0, 0x60(r1)
/* 800E2D58 000DFC98  FC 00 00 1E */	fctiwz f0, f0
/* 800E2D5C 000DFC9C  D8 01 00 68 */	stfd f0, 0x68(r1)
/* 800E2D60 000DFCA0  80 61 00 6C */	lwz r3, 0x6c(r1)
/* 800E2D64 000DFCA4  48 00 00 08 */	b lbl_800E2D6C
lbl_800E2D68:
/* 800E2D68 000DFCA8  38 60 00 00 */	li r3, 0
lbl_800E2D6C:
/* 800E2D6C 000DFCAC  48 16 A4 61 */	bl isDownWaitingToTalk__7TEMarioCFv
/* 800E2D70 000DFCB0  38 00 00 00 */	li r0, 0
/* 800E2D74 000DFCB4  90 01 00 44 */	stw r0, 0x44(r1)
/* 800E2D78 000DFCB8  54 60 06 3E */	clrlwi r0, r3, 0x18
/* 800E2D7C 000DFCBC  90 01 00 48 */	stw r0, 0x48(r1)
/* 800E2D80 000DFCC0  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E2D84 000DFCC4  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800E2D88 000DFCC8  7C 03 00 00 */	cmpw r3, r0
/* 800E2D8C 000DFCCC  41 80 00 18 */	blt lbl_800E2DA4
/* 800E2D90 000DFCD0  3C 60 80 37 */	lis r3, $$23834@ha
/* 800E2D94 000DFCD4  4C C6 31 82 */	crclr 6
/* 800E2D98 000DFCD8  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800E2D9C 000DFCDC  4B FF A3 09 */	bl SpcTrace__FPCce
/* 800E2DA0 000DFCE0  48 00 00 2C */	b lbl_800E2DCC
lbl_800E2DA4:
/* 800E2DA4 000DFCE4  80 BF 00 20 */	lwz r5, 0x20(r31)
/* 800E2DA8 000DFCE8  54 64 18 38 */	slwi r4, r3, 3
/* 800E2DAC 000DFCEC  80 61 00 44 */	lwz r3, 0x44(r1)
/* 800E2DB0 000DFCF0  80 01 00 48 */	lwz r0, 0x48(r1)
/* 800E2DB4 000DFCF4  7C 85 22 14 */	add r4, r5, r4
/* 800E2DB8 000DFCF8  90 64 00 00 */	stw r3, 0(r4)
/* 800E2DBC 000DFCFC  90 04 00 04 */	stw r0, 4(r4)
/* 800E2DC0 000DFD00  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E2DC4 000DFD04  38 03 00 01 */	addi r0, r3, 1
/* 800E2DC8 000DFD08  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800E2DCC:
/* 800E2DCC 000DFD0C  80 01 00 84 */	lwz r0, 0x84(r1)
/* 800E2DD0 000DFD10  83 E1 00 7C */	lwz r31, 0x7c(r1)
/* 800E2DD4 000DFD14  83 C1 00 78 */	lwz r30, 0x78(r1)
/* 800E2DD8 000DFD18  7C 08 03 A6 */	mtlr r0
/* 800E2DDC 000DFD1C  83 A1 00 74 */	lwz r29, 0x74(r1)
/* 800E2DE0 000DFD20  38 21 00 80 */	addi r1, r1, 0x80
/* 800E2DE4 000DFD24  4E 80 00 20 */	blr 

.global evIsEMarioReachedToGoal__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evIsEMarioReachedToGoal__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E2DE8 000DFD28  7C 08 02 A6 */	mflr r0
/* 800E2DEC 000DFD2C  90 01 00 04 */	stw r0, 4(r1)
/* 800E2DF0 000DFD30  94 21 FF 80 */	stwu r1, -0x80(r1)
/* 800E2DF4 000DFD34  93 E1 00 7C */	stw r31, 0x7c(r1)
/* 800E2DF8 000DFD38  7C 7F 1B 78 */	mr r31, r3
/* 800E2DFC 000DFD3C  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E2E00 000DFD40  93 C1 00 78 */	stw r30, 0x78(r1)
/* 800E2E04 000DFD44  93 A1 00 74 */	stw r29, 0x74(r1)
/* 800E2E08 000DFD48  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E2E0C 000DFD4C  38 80 00 01 */	li r4, 1
/* 800E2E10 000DFD50  48 03 46 21 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E2E14 000DFD54  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E2E18 000DFD58  2C 03 00 00 */	cmpwi r3, 0
/* 800E2E1C 000DFD5C  41 81 00 38 */	bgt lbl_800E2E54
/* 800E2E20 000DFD60  3C 60 80 37 */	lis r3, $$23829@ha
/* 800E2E24 000DFD64  4C C6 31 82 */	crclr 6
/* 800E2E28 000DFD68  38 63 30 FC */	addi r3, r3, $$23829@l
/* 800E2E2C 000DFD6C  4B FF A2 79 */	bl SpcTrace__FPCce
/* 800E2E30 000DFD70  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800E2E34 000DFD74  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800E2E38 000DFD78  54 00 18 38 */	slwi r0, r0, 3
/* 800E2E3C 000DFD7C  7C 63 02 14 */	add r3, r3, r0
/* 800E2E40 000DFD80  80 03 00 00 */	lwz r0, 0(r3)
/* 800E2E44 000DFD84  90 01 00 54 */	stw r0, 0x54(r1)
/* 800E2E48 000DFD88  80 03 00 04 */	lwz r0, 4(r3)
/* 800E2E4C 000DFD8C  90 01 00 58 */	stw r0, 0x58(r1)
/* 800E2E50 000DFD90  48 00 00 2C */	b lbl_800E2E7C
lbl_800E2E54:
/* 800E2E54 000DFD94  38 03 FF FF */	addi r0, r3, -1
/* 800E2E58 000DFD98  90 1F 00 1C */	stw r0, 0x1c(r31)
/* 800E2E5C 000DFD9C  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800E2E60 000DFDA0  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800E2E64 000DFDA4  54 00 18 38 */	slwi r0, r0, 3
/* 800E2E68 000DFDA8  7C 63 02 14 */	add r3, r3, r0
/* 800E2E6C 000DFDAC  80 03 00 00 */	lwz r0, 0(r3)
/* 800E2E70 000DFDB0  90 01 00 54 */	stw r0, 0x54(r1)
/* 800E2E74 000DFDB4  80 03 00 04 */	lwz r0, 4(r3)
/* 800E2E78 000DFDB8  90 01 00 58 */	stw r0, 0x58(r1)
lbl_800E2E7C:
/* 800E2E7C 000DFDBC  80 81 00 54 */	lwz r4, 0x54(r1)
/* 800E2E80 000DFDC0  38 60 00 00 */	li r3, 0
/* 800E2E84 000DFDC4  80 01 00 58 */	lwz r0, 0x58(r1)
/* 800E2E88 000DFDC8  90 81 00 5C */	stw r4, 0x5c(r1)
/* 800E2E8C 000DFDCC  90 01 00 60 */	stw r0, 0x60(r1)
/* 800E2E90 000DFDD0  80 01 00 5C */	lwz r0, 0x5c(r1)
/* 800E2E94 000DFDD4  2C 00 00 01 */	cmpwi r0, 1
/* 800E2E98 000DFDD8  41 82 00 9C */	beq lbl_800E2F34
/* 800E2E9C 000DFDDC  40 80 00 10 */	bge lbl_800E2EAC
/* 800E2EA0 000DFDE0  2C 00 00 00 */	cmpwi r0, 0
/* 800E2EA4 000DFDE4  40 80 00 58 */	bge lbl_800E2EFC
/* 800E2EA8 000DFDE8  48 00 00 8C */	b lbl_800E2F34
lbl_800E2EAC:
/* 800E2EAC 000DFDEC  2C 00 00 03 */	cmpwi r0, 3
/* 800E2EB0 000DFDF0  40 80 00 84 */	bge lbl_800E2F34
/* 800E2EB4 000DFDF4  2C 00 00 02 */	cmpwi r0, 2
/* 800E2EB8 000DFDF8  41 82 00 08 */	beq lbl_800E2EC0
/* 800E2EBC 000DFDFC  48 00 00 0C */	b lbl_800E2EC8
lbl_800E2EC0:
/* 800E2EC0 000DFE00  83 C1 00 60 */	lwz r30, 0x60(r1)
/* 800E2EC4 000DFE04  48 00 00 08 */	b lbl_800E2ECC
lbl_800E2EC8:
/* 800E2EC8 000DFE08  3B C2 8F F0 */	addi r30, r2, $$24053@sda21
lbl_800E2ECC:
/* 800E2ECC 000DFE0C  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 800E2ED0 000DFE10  7F C3 F3 78 */	mr r3, r30
/* 800E2ED4 000DFE14  83 A4 00 04 */	lwz r29, 4(r4)
/* 800E2ED8 000DFE18  4B F6 17 B5 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800E2EDC 000DFE1C  81 9D 00 00 */	lwz r12, 0(r29)
/* 800E2EE0 000DFE20  38 83 00 00 */	addi r4, r3, 0
/* 800E2EE4 000DFE24  38 7D 00 00 */	addi r3, r29, 0
/* 800E2EE8 000DFE28  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 800E2EEC 000DFE2C  7F C5 F3 78 */	mr r5, r30
/* 800E2EF0 000DFE30  7D 88 03 A6 */	mtlr r12
/* 800E2EF4 000DFE34  4E 80 00 21 */	blrl 
/* 800E2EF8 000DFE38  48 00 00 3C */	b lbl_800E2F34
lbl_800E2EFC:
/* 800E2EFC 000DFE3C  2C 00 00 01 */	cmpwi r0, 1
/* 800E2F00 000DFE40  41 82 00 1C */	beq lbl_800E2F1C
/* 800E2F04 000DFE44  40 80 00 2C */	bge lbl_800E2F30
/* 800E2F08 000DFE48  2C 00 00 00 */	cmpwi r0, 0
/* 800E2F0C 000DFE4C  40 80 00 08 */	bge lbl_800E2F14
/* 800E2F10 000DFE50  48 00 00 20 */	b lbl_800E2F30
lbl_800E2F14:
/* 800E2F14 000DFE54  80 61 00 60 */	lwz r3, 0x60(r1)
/* 800E2F18 000DFE58  48 00 00 1C */	b lbl_800E2F34
lbl_800E2F1C:
/* 800E2F1C 000DFE5C  C0 01 00 60 */	lfs f0, 0x60(r1)
/* 800E2F20 000DFE60  FC 00 00 1E */	fctiwz f0, f0
/* 800E2F24 000DFE64  D8 01 00 68 */	stfd f0, 0x68(r1)
/* 800E2F28 000DFE68  80 61 00 6C */	lwz r3, 0x6c(r1)
/* 800E2F2C 000DFE6C  48 00 00 08 */	b lbl_800E2F34
lbl_800E2F30:
/* 800E2F30 000DFE70  38 60 00 00 */	li r3, 0
lbl_800E2F34:
/* 800E2F34 000DFE74  48 16 A2 B9 */	bl isReachedToGate__7TEMarioCFv
/* 800E2F38 000DFE78  38 00 00 00 */	li r0, 0
/* 800E2F3C 000DFE7C  90 01 00 44 */	stw r0, 0x44(r1)
/* 800E2F40 000DFE80  54 60 06 3E */	clrlwi r0, r3, 0x18
/* 800E2F44 000DFE84  90 01 00 48 */	stw r0, 0x48(r1)
/* 800E2F48 000DFE88  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E2F4C 000DFE8C  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800E2F50 000DFE90  7C 03 00 00 */	cmpw r3, r0
/* 800E2F54 000DFE94  41 80 00 18 */	blt lbl_800E2F6C
/* 800E2F58 000DFE98  3C 60 80 37 */	lis r3, $$23834@ha
/* 800E2F5C 000DFE9C  4C C6 31 82 */	crclr 6
/* 800E2F60 000DFEA0  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800E2F64 000DFEA4  4B FF A1 41 */	bl SpcTrace__FPCce
/* 800E2F68 000DFEA8  48 00 00 2C */	b lbl_800E2F94
lbl_800E2F6C:
/* 800E2F6C 000DFEAC  80 BF 00 20 */	lwz r5, 0x20(r31)
/* 800E2F70 000DFEB0  54 64 18 38 */	slwi r4, r3, 3
/* 800E2F74 000DFEB4  80 61 00 44 */	lwz r3, 0x44(r1)
/* 800E2F78 000DFEB8  80 01 00 48 */	lwz r0, 0x48(r1)
/* 800E2F7C 000DFEBC  7C 85 22 14 */	add r4, r5, r4
/* 800E2F80 000DFEC0  90 64 00 00 */	stw r3, 0(r4)
/* 800E2F84 000DFEC4  90 04 00 04 */	stw r0, 4(r4)
/* 800E2F88 000DFEC8  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E2F8C 000DFECC  38 03 00 01 */	addi r0, r3, 1
/* 800E2F90 000DFED0  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800E2F94:
/* 800E2F94 000DFED4  80 01 00 84 */	lwz r0, 0x84(r1)
/* 800E2F98 000DFED8  83 E1 00 7C */	lwz r31, 0x7c(r1)
/* 800E2F9C 000DFEDC  83 C1 00 78 */	lwz r30, 0x78(r1)
/* 800E2FA0 000DFEE0  7C 08 03 A6 */	mtlr r0
/* 800E2FA4 000DFEE4  83 A1 00 74 */	lwz r29, 0x74(r1)
/* 800E2FA8 000DFEE8  38 21 00 80 */	addi r1, r1, 0x80
/* 800E2FAC 000DFEEC  4E 80 00 20 */	blr 

.global evLaunchEventClearDemo__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evLaunchEventClearDemo__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E2FB0 000DFEF0  7C 08 02 A6 */	mflr r0
/* 800E2FB4 000DFEF4  90 01 00 04 */	stw r0, 4(r1)
/* 800E2FB8 000DFEF8  94 21 FF D0 */	stwu r1, -0x30(r1)
/* 800E2FBC 000DFEFC  93 E1 00 2C */	stw r31, 0x2c(r1)
/* 800E2FC0 000DFF00  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E2FC4 000DFF04  93 C1 00 28 */	stw r30, 0x28(r1)
/* 800E2FC8 000DFF08  7C 7E 1B 78 */	mr r30, r3
/* 800E2FCC 000DFF0C  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E2FD0 000DFF10  38 80 00 00 */	li r4, 0
/* 800E2FD4 000DFF14  48 03 44 5D */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E2FD8 000DFF18  80 6D 97 E8 */	lwz r3, gpMarDirector@sda21(r13)
/* 800E2FDC 000DFF1C  83 E3 00 74 */	lwz r31, 0x74(r3)
/* 800E2FE0 000DFF20  80 7F 00 94 */	lwz r3, 0x94(r31)
/* 800E2FE4 000DFF24  48 14 F0 65 */	bl startAppearShineGet__11TConsoleStrFv
/* 800E2FE8 000DFF28  38 00 00 01 */	li r0, 1
/* 800E2FEC 000DFF2C  98 1F 00 47 */	stb r0, 0x47(r31)
/* 800E2FF0 000DFF30  38 00 00 00 */	li r0, 0
/* 800E2FF4 000DFF34  90 01 00 20 */	stw r0, 0x20(r1)
/* 800E2FF8 000DFF38  90 01 00 24 */	stw r0, 0x24(r1)
/* 800E2FFC 000DFF3C  80 7E 00 1C */	lwz r3, 0x1c(r30)
/* 800E3000 000DFF40  80 1E 00 18 */	lwz r0, 0x18(r30)
/* 800E3004 000DFF44  7C 03 00 00 */	cmpw r3, r0
/* 800E3008 000DFF48  41 80 00 18 */	blt lbl_800E3020
/* 800E300C 000DFF4C  3C 60 80 37 */	lis r3, $$23834@ha
/* 800E3010 000DFF50  4C C6 31 82 */	crclr 6
/* 800E3014 000DFF54  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800E3018 000DFF58  4B FF A0 8D */	bl SpcTrace__FPCce
/* 800E301C 000DFF5C  48 00 00 2C */	b lbl_800E3048
lbl_800E3020:
/* 800E3020 000DFF60  80 BE 00 20 */	lwz r5, 0x20(r30)
/* 800E3024 000DFF64  54 64 18 38 */	slwi r4, r3, 3
/* 800E3028 000DFF68  80 61 00 20 */	lwz r3, 0x20(r1)
/* 800E302C 000DFF6C  80 01 00 24 */	lwz r0, 0x24(r1)
/* 800E3030 000DFF70  7C 85 22 14 */	add r4, r5, r4
/* 800E3034 000DFF74  90 64 00 00 */	stw r3, 0(r4)
/* 800E3038 000DFF78  90 04 00 04 */	stw r0, 4(r4)
/* 800E303C 000DFF7C  80 7E 00 1C */	lwz r3, 0x1c(r30)
/* 800E3040 000DFF80  38 03 00 01 */	addi r0, r3, 1
/* 800E3044 000DFF84  90 1E 00 1C */	stw r0, 0x1c(r30)
lbl_800E3048:
/* 800E3048 000DFF88  80 01 00 34 */	lwz r0, 0x34(r1)
/* 800E304C 000DFF8C  83 E1 00 2C */	lwz r31, 0x2c(r1)
/* 800E3050 000DFF90  83 C1 00 28 */	lwz r30, 0x28(r1)
/* 800E3054 000DFF94  7C 08 03 A6 */	mtlr r0
/* 800E3058 000DFF98  38 21 00 30 */	addi r1, r1, 0x30
/* 800E305C 000DFF9C  4E 80 00 20 */	blr 

.global evIsBossDefeated__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evIsBossDefeated__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E3060 000DFFA0  7C 08 02 A6 */	mflr r0
/* 800E3064 000DFFA4  90 01 00 04 */	stw r0, 4(r1)
/* 800E3068 000DFFA8  94 21 FF D8 */	stwu r1, -0x28(r1)
/* 800E306C 000DFFAC  93 E1 00 24 */	stw r31, 0x24(r1)
/* 800E3070 000DFFB0  7C 7F 1B 78 */	mr r31, r3
/* 800E3074 000DFFB4  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E3078 000DFFB8  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E307C 000DFFBC  38 80 00 00 */	li r4, 0
/* 800E3080 000DFFC0  48 03 43 B1 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E3084 000DFFC4  80 6D 9C 28 */	lwz r3, gpConductor@sda21(r13)
/* 800E3088 000DFFC8  48 16 5A 25 */	bl isBossDefeated__10TConductorFv
/* 800E308C 000DFFCC  2C 03 00 00 */	cmpwi r3, 0
/* 800E3090 000DFFD0  41 82 00 0C */	beq lbl_800E309C
/* 800E3094 000DFFD4  38 60 00 01 */	li r3, 1
/* 800E3098 000DFFD8  48 00 00 08 */	b lbl_800E30A0
lbl_800E309C:
/* 800E309C 000DFFDC  38 60 00 00 */	li r3, 0
lbl_800E30A0:
/* 800E30A0 000DFFE0  38 00 00 00 */	li r0, 0
/* 800E30A4 000DFFE4  90 01 00 14 */	stw r0, 0x14(r1)
/* 800E30A8 000DFFE8  90 61 00 18 */	stw r3, 0x18(r1)
/* 800E30AC 000DFFEC  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E30B0 000DFFF0  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800E30B4 000DFFF4  7C 03 00 00 */	cmpw r3, r0
/* 800E30B8 000DFFF8  41 80 00 18 */	blt lbl_800E30D0
/* 800E30BC 000DFFFC  3C 60 80 37 */	lis r3, $$23834@ha
/* 800E30C0 000E0000  4C C6 31 82 */	crclr 6
/* 800E30C4 000E0004  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800E30C8 000E0008  4B FF 9F DD */	bl SpcTrace__FPCce
/* 800E30CC 000E000C  48 00 00 2C */	b lbl_800E30F8
lbl_800E30D0:
/* 800E30D0 000E0010  80 BF 00 20 */	lwz r5, 0x20(r31)
/* 800E30D4 000E0014  54 64 18 38 */	slwi r4, r3, 3
/* 800E30D8 000E0018  80 61 00 14 */	lwz r3, 0x14(r1)
/* 800E30DC 000E001C  80 01 00 18 */	lwz r0, 0x18(r1)
/* 800E30E0 000E0020  7C 85 22 14 */	add r4, r5, r4
/* 800E30E4 000E0024  90 64 00 00 */	stw r3, 0(r4)
/* 800E30E8 000E0028  90 04 00 04 */	stw r0, 4(r4)
/* 800E30EC 000E002C  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E30F0 000E0030  38 03 00 01 */	addi r0, r3, 1
/* 800E30F4 000E0034  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800E30F8:
/* 800E30F8 000E0038  80 01 00 2C */	lwz r0, 0x2c(r1)
/* 800E30FC 000E003C  83 E1 00 24 */	lwz r31, 0x24(r1)
/* 800E3100 000E0040  38 21 00 28 */	addi r1, r1, 0x28
/* 800E3104 000E0044  7C 08 03 A6 */	mtlr r0
/* 800E3108 000E0048  4E 80 00 20 */	blr 

.global evSetGraffitoMultiplied__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evSetGraffitoMultiplied__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E310C 000E004C  7C 08 02 A6 */	mflr r0
/* 800E3110 000E0050  90 01 00 04 */	stw r0, 4(r1)
/* 800E3114 000E0054  94 21 FF 90 */	stwu r1, -0x70(r1)
/* 800E3118 000E0058  93 E1 00 6C */	stw r31, 0x6c(r1)
/* 800E311C 000E005C  7C 7F 1B 78 */	mr r31, r3
/* 800E3120 000E0060  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E3124 000E0064  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E3128 000E0068  38 80 00 01 */	li r4, 1
/* 800E312C 000E006C  48 03 43 05 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E3130 000E0070  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E3134 000E0074  2C 03 00 00 */	cmpwi r3, 0
/* 800E3138 000E0078  41 81 00 34 */	bgt lbl_800E316C
/* 800E313C 000E007C  3C 60 80 37 */	lis r3, $$23829@ha
/* 800E3140 000E0080  4C C6 31 82 */	crclr 6
/* 800E3144 000E0084  38 63 30 FC */	addi r3, r3, $$23829@l
/* 800E3148 000E0088  4B FF 9F 5D */	bl SpcTrace__FPCce
/* 800E314C 000E008C  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800E3150 000E0090  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800E3154 000E0094  54 00 18 38 */	slwi r0, r0, 3
/* 800E3158 000E0098  7C 63 02 14 */	add r3, r3, r0
/* 800E315C 000E009C  80 83 00 00 */	lwz r4, 0(r3)
/* 800E3160 000E00A0  80 03 00 04 */	lwz r0, 4(r3)
/* 800E3164 000E00A4  90 01 00 4C */	stw r0, 0x4c(r1)
/* 800E3168 000E00A8  48 00 00 28 */	b lbl_800E3190
lbl_800E316C:
/* 800E316C 000E00AC  38 03 FF FF */	addi r0, r3, -1
/* 800E3170 000E00B0  90 1F 00 1C */	stw r0, 0x1c(r31)
/* 800E3174 000E00B4  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800E3178 000E00B8  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800E317C 000E00BC  54 00 18 38 */	slwi r0, r0, 3
/* 800E3180 000E00C0  7C 63 02 14 */	add r3, r3, r0
/* 800E3184 000E00C4  80 83 00 00 */	lwz r4, 0(r3)
/* 800E3188 000E00C8  80 03 00 04 */	lwz r0, 4(r3)
/* 800E318C 000E00CC  90 01 00 4C */	stw r0, 0x4c(r1)
lbl_800E3190:
/* 800E3190 000E00D0  90 81 00 50 */	stw r4, 0x50(r1)
/* 800E3194 000E00D4  80 01 00 4C */	lwz r0, 0x4c(r1)
/* 800E3198 000E00D8  90 01 00 54 */	stw r0, 0x54(r1)
/* 800E319C 000E00DC  80 01 00 50 */	lwz r0, 0x50(r1)
/* 800E31A0 000E00E0  2C 00 00 01 */	cmpwi r0, 1
/* 800E31A4 000E00E4  41 82 00 1C */	beq lbl_800E31C0
/* 800E31A8 000E00E8  40 80 00 2C */	bge lbl_800E31D4
/* 800E31AC 000E00EC  2C 00 00 00 */	cmpwi r0, 0
/* 800E31B0 000E00F0  40 80 00 08 */	bge lbl_800E31B8
/* 800E31B4 000E00F4  48 00 00 20 */	b lbl_800E31D4
lbl_800E31B8:
/* 800E31B8 000E00F8  80 01 00 54 */	lwz r0, 0x54(r1)
/* 800E31BC 000E00FC  48 00 00 1C */	b lbl_800E31D8
lbl_800E31C0:
/* 800E31C0 000E0100  C0 01 00 54 */	lfs f0, 0x54(r1)
/* 800E31C4 000E0104  FC 00 00 1E */	fctiwz f0, f0
/* 800E31C8 000E0108  D8 01 00 60 */	stfd f0, 0x60(r1)
/* 800E31CC 000E010C  80 01 00 64 */	lwz r0, 0x64(r1)
/* 800E31D0 000E0110  48 00 00 08 */	b lbl_800E31D8
lbl_800E31D4:
/* 800E31D4 000E0114  38 00 00 00 */	li r0, 0
lbl_800E31D8:
/* 800E31D8 000E0118  2C 00 00 00 */	cmpwi r0, 0
/* 800E31DC 000E011C  80 CD 9A E8 */	lwz r6, gpPollution@sda21(r13)
/* 800E31E0 000E0120  38 E0 00 00 */	li r7, 0
/* 800E31E4 000E0124  38 60 00 00 */	li r3, 0
/* 800E31E8 000E0128  38 80 00 00 */	li r4, 0
/* 800E31EC 000E012C  41 82 00 54 */	beq lbl_800E3240
/* 800E31F0 000E0130  48 00 00 20 */	b lbl_800E3210
lbl_800E31F4:
/* 800E31F4 000E0134  80 A6 00 14 */	lwz r5, 0x14(r6)
/* 800E31F8 000E0138  38 E7 00 01 */	addi r7, r7, 1
/* 800E31FC 000E013C  7C A5 20 2E */	lwzx r5, r5, r4
/* 800E3200 000E0140  38 84 00 04 */	addi r4, r4, 4
/* 800E3204 000E0144  A0 05 00 32 */	lhz r0, 0x32(r5)
/* 800E3208 000E0148  60 00 00 01 */	ori r0, r0, 1
/* 800E320C 000E014C  B0 05 00 32 */	sth r0, 0x32(r5)
lbl_800E3210:
/* 800E3210 000E0150  80 06 00 10 */	lwz r0, 0x10(r6)
/* 800E3214 000E0154  7C 07 00 00 */	cmpw r7, r0
/* 800E3218 000E0158  41 80 FF DC */	blt lbl_800E31F4
/* 800E321C 000E015C  48 00 00 30 */	b lbl_800E324C
/* 800E3220 000E0160  48 00 00 20 */	b lbl_800E3240
lbl_800E3224:
/* 800E3224 000E0164  80 A6 00 14 */	lwz r5, 0x14(r6)
/* 800E3228 000E0168  38 E7 00 01 */	addi r7, r7, 1
/* 800E322C 000E016C  7C A5 18 2E */	lwzx r5, r5, r3
/* 800E3230 000E0170  38 63 00 04 */	addi r3, r3, 4
/* 800E3234 000E0174  A0 05 00 32 */	lhz r0, 0x32(r5)
/* 800E3238 000E0178  54 00 00 3C */	rlwinm r0, r0, 0, 0, 0x1e
/* 800E323C 000E017C  B0 05 00 32 */	sth r0, 0x32(r5)
lbl_800E3240:
/* 800E3240 000E0180  80 06 00 10 */	lwz r0, 0x10(r6)
/* 800E3244 000E0184  7C 07 00 00 */	cmpw r7, r0
/* 800E3248 000E0188  41 80 FF DC */	blt lbl_800E3224
lbl_800E324C:
/* 800E324C 000E018C  38 00 00 00 */	li r0, 0
/* 800E3250 000E0190  90 01 00 34 */	stw r0, 0x34(r1)
/* 800E3254 000E0194  90 01 00 38 */	stw r0, 0x38(r1)
/* 800E3258 000E0198  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E325C 000E019C  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800E3260 000E01A0  7C 03 00 00 */	cmpw r3, r0
/* 800E3264 000E01A4  41 80 00 18 */	blt lbl_800E327C
/* 800E3268 000E01A8  3C 60 80 37 */	lis r3, $$23834@ha
/* 800E326C 000E01AC  4C C6 31 82 */	crclr 6
/* 800E3270 000E01B0  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800E3274 000E01B4  4B FF 9E 31 */	bl SpcTrace__FPCce
/* 800E3278 000E01B8  48 00 00 2C */	b lbl_800E32A4
lbl_800E327C:
/* 800E327C 000E01BC  80 BF 00 20 */	lwz r5, 0x20(r31)
/* 800E3280 000E01C0  54 64 18 38 */	slwi r4, r3, 3
/* 800E3284 000E01C4  80 61 00 34 */	lwz r3, 0x34(r1)
/* 800E3288 000E01C8  80 01 00 38 */	lwz r0, 0x38(r1)
/* 800E328C 000E01CC  7C 85 22 14 */	add r4, r5, r4
/* 800E3290 000E01D0  90 64 00 00 */	stw r3, 0(r4)
/* 800E3294 000E01D4  90 04 00 04 */	stw r0, 4(r4)
/* 800E3298 000E01D8  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E329C 000E01DC  38 03 00 01 */	addi r0, r3, 1
/* 800E32A0 000E01E0  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800E32A4:
/* 800E32A4 000E01E4  80 01 00 74 */	lwz r0, 0x74(r1)
/* 800E32A8 000E01E8  83 E1 00 6C */	lwz r31, 0x6c(r1)
/* 800E32AC 000E01EC  38 21 00 70 */	addi r1, r1, 0x70
/* 800E32B0 000E01F0  7C 08 03 A6 */	mtlr r0
/* 800E32B4 000E01F4  4E 80 00 20 */	blr 

.global evIsGraffitoCoverage0__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evIsGraffitoCoverage0__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E32B8 000E01F8  7C 08 02 A6 */	mflr r0
/* 800E32BC 000E01FC  90 01 00 04 */	stw r0, 4(r1)
/* 800E32C0 000E0200  94 21 FF D8 */	stwu r1, -0x28(r1)
/* 800E32C4 000E0204  93 E1 00 24 */	stw r31, 0x24(r1)
/* 800E32C8 000E0208  7C 7F 1B 78 */	mr r31, r3
/* 800E32CC 000E020C  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E32D0 000E0210  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E32D4 000E0214  38 80 00 00 */	li r4, 0
/* 800E32D8 000E0218  48 03 41 59 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E32DC 000E021C  80 6D 9A E8 */	lwz r3, gpPollution@sda21(r13)
/* 800E32E0 000E0220  48 10 C2 19 */	bl cleanedAll__17TPollutionManagerCFv
/* 800E32E4 000E0224  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 800E32E8 000E0228  41 82 00 0C */	beq lbl_800E32F4
/* 800E32EC 000E022C  38 60 00 01 */	li r3, 1
/* 800E32F0 000E0230  48 00 00 08 */	b lbl_800E32F8
lbl_800E32F4:
/* 800E32F4 000E0234  38 60 00 00 */	li r3, 0
lbl_800E32F8:
/* 800E32F8 000E0238  38 00 00 00 */	li r0, 0
/* 800E32FC 000E023C  90 01 00 14 */	stw r0, 0x14(r1)
/* 800E3300 000E0240  90 61 00 18 */	stw r3, 0x18(r1)
/* 800E3304 000E0244  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E3308 000E0248  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800E330C 000E024C  7C 03 00 00 */	cmpw r3, r0
/* 800E3310 000E0250  41 80 00 18 */	blt lbl_800E3328
/* 800E3314 000E0254  3C 60 80 37 */	lis r3, $$23834@ha
/* 800E3318 000E0258  4C C6 31 82 */	crclr 6
/* 800E331C 000E025C  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800E3320 000E0260  4B FF 9D 85 */	bl SpcTrace__FPCce
/* 800E3324 000E0264  48 00 00 2C */	b lbl_800E3350
lbl_800E3328:
/* 800E3328 000E0268  80 BF 00 20 */	lwz r5, 0x20(r31)
/* 800E332C 000E026C  54 64 18 38 */	slwi r4, r3, 3
/* 800E3330 000E0270  80 61 00 14 */	lwz r3, 0x14(r1)
/* 800E3334 000E0274  80 01 00 18 */	lwz r0, 0x18(r1)
/* 800E3338 000E0278  7C 85 22 14 */	add r4, r5, r4
/* 800E333C 000E027C  90 64 00 00 */	stw r3, 0(r4)
/* 800E3340 000E0280  90 04 00 04 */	stw r0, 4(r4)
/* 800E3344 000E0284  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E3348 000E0288  38 03 00 01 */	addi r0, r3, 1
/* 800E334C 000E028C  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800E3350:
/* 800E3350 000E0290  80 01 00 2C */	lwz r0, 0x2c(r1)
/* 800E3354 000E0294  83 E1 00 24 */	lwz r31, 0x24(r1)
/* 800E3358 000E0298  38 21 00 28 */	addi r1, r1, 0x28
/* 800E335C 000E029C  7C 08 03 A6 */	mtlr r0
/* 800E3360 000E02A0  4E 80 00 20 */	blr 

.global evGameOver__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evGameOver__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E3364 000E02A4  7C 08 02 A6 */	mflr r0
/* 800E3368 000E02A8  90 01 00 04 */	stw r0, 4(r1)
/* 800E336C 000E02AC  94 21 FF C8 */	stwu r1, -0x38(r1)
/* 800E3370 000E02B0  93 E1 00 34 */	stw r31, 0x34(r1)
/* 800E3374 000E02B4  7C 7F 1B 78 */	mr r31, r3
/* 800E3378 000E02B8  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E337C 000E02BC  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E3380 000E02C0  38 80 00 00 */	li r4, 0
/* 800E3384 000E02C4  48 03 40 AD */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E3388 000E02C8  80 8D 97 E8 */	lwz r4, gpMarDirector@sda21(r13)
/* 800E338C 000E02CC  38 00 00 00 */	li r0, 0
/* 800E3390 000E02D0  A0 64 00 4C */	lhz r3, 0x4c(r4)
/* 800E3394 000E02D4  60 63 00 01 */	ori r3, r3, 1
/* 800E3398 000E02D8  B0 64 00 4C */	sth r3, 0x4c(r4)
/* 800E339C 000E02DC  90 01 00 24 */	stw r0, 0x24(r1)
/* 800E33A0 000E02E0  90 01 00 28 */	stw r0, 0x28(r1)
/* 800E33A4 000E02E4  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E33A8 000E02E8  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800E33AC 000E02EC  7C 03 00 00 */	cmpw r3, r0
/* 800E33B0 000E02F0  41 80 00 18 */	blt lbl_800E33C8
/* 800E33B4 000E02F4  3C 60 80 37 */	lis r3, $$23834@ha
/* 800E33B8 000E02F8  4C C6 31 82 */	crclr 6
/* 800E33BC 000E02FC  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800E33C0 000E0300  4B FF 9C E5 */	bl SpcTrace__FPCce
/* 800E33C4 000E0304  48 00 00 2C */	b lbl_800E33F0
lbl_800E33C8:
/* 800E33C8 000E0308  80 BF 00 20 */	lwz r5, 0x20(r31)
/* 800E33CC 000E030C  54 64 18 38 */	slwi r4, r3, 3
/* 800E33D0 000E0310  80 61 00 24 */	lwz r3, 0x24(r1)
/* 800E33D4 000E0314  80 01 00 28 */	lwz r0, 0x28(r1)
/* 800E33D8 000E0318  7C 85 22 14 */	add r4, r5, r4
/* 800E33DC 000E031C  90 64 00 00 */	stw r3, 0(r4)
/* 800E33E0 000E0320  90 04 00 04 */	stw r0, 4(r4)
/* 800E33E4 000E0324  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E33E8 000E0328  38 03 00 01 */	addi r0, r3, 1
/* 800E33EC 000E032C  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800E33F0:
/* 800E33F0 000E0330  80 01 00 3C */	lwz r0, 0x3c(r1)
/* 800E33F4 000E0334  83 E1 00 34 */	lwz r31, 0x34(r1)
/* 800E33F8 000E0338  38 21 00 38 */	addi r1, r1, 0x38
/* 800E33FC 000E033C  7C 08 03 A6 */	mtlr r0
/* 800E3400 000E0340  4E 80 00 20 */	blr 

.global evRegisterMovie__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evRegisterMovie__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E3404 000E0344  7C 08 02 A6 */	mflr r0
/* 800E3408 000E0348  90 01 00 04 */	stw r0, 4(r1)
/* 800E340C 000E034C  94 21 FF A8 */	stwu r1, -0x58(r1)
/* 800E3410 000E0350  93 E1 00 54 */	stw r31, 0x54(r1)
/* 800E3414 000E0354  7C 7F 1B 78 */	mr r31, r3
/* 800E3418 000E0358  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E341C 000E035C  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E3420 000E0360  38 80 00 01 */	li r4, 1
/* 800E3424 000E0364  48 03 40 0D */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E3428 000E0368  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E342C 000E036C  2C 03 00 00 */	cmpwi r3, 0
/* 800E3430 000E0370  41 81 00 34 */	bgt lbl_800E3464
/* 800E3434 000E0374  3C 60 80 37 */	lis r3, $$23829@ha
/* 800E3438 000E0378  4C C6 31 82 */	crclr 6
/* 800E343C 000E037C  38 63 30 FC */	addi r3, r3, $$23829@l
/* 800E3440 000E0380  4B FF 9C 65 */	bl SpcTrace__FPCce
/* 800E3444 000E0384  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800E3448 000E0388  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800E344C 000E038C  54 00 18 38 */	slwi r0, r0, 3
/* 800E3450 000E0390  7C 63 02 14 */	add r3, r3, r0
/* 800E3454 000E0394  80 83 00 00 */	lwz r4, 0(r3)
/* 800E3458 000E0398  80 03 00 04 */	lwz r0, 4(r3)
/* 800E345C 000E039C  90 01 00 38 */	stw r0, 0x38(r1)
/* 800E3460 000E03A0  48 00 00 28 */	b lbl_800E3488
lbl_800E3464:
/* 800E3464 000E03A4  38 03 FF FF */	addi r0, r3, -1
/* 800E3468 000E03A8  90 1F 00 1C */	stw r0, 0x1c(r31)
/* 800E346C 000E03AC  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800E3470 000E03B0  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800E3474 000E03B4  54 00 18 38 */	slwi r0, r0, 3
/* 800E3478 000E03B8  7C 63 02 14 */	add r3, r3, r0
/* 800E347C 000E03BC  80 83 00 00 */	lwz r4, 0(r3)
/* 800E3480 000E03C0  80 03 00 04 */	lwz r0, 4(r3)
/* 800E3484 000E03C4  90 01 00 38 */	stw r0, 0x38(r1)
lbl_800E3488:
/* 800E3488 000E03C8  90 81 00 3C */	stw r4, 0x3c(r1)
/* 800E348C 000E03CC  80 01 00 38 */	lwz r0, 0x38(r1)
/* 800E3490 000E03D0  90 01 00 40 */	stw r0, 0x40(r1)
/* 800E3494 000E03D4  80 01 00 3C */	lwz r0, 0x3c(r1)
/* 800E3498 000E03D8  2C 00 00 01 */	cmpwi r0, 1
/* 800E349C 000E03DC  41 82 00 1C */	beq lbl_800E34B8
/* 800E34A0 000E03E0  40 80 00 2C */	bge lbl_800E34CC
/* 800E34A4 000E03E4  2C 00 00 00 */	cmpwi r0, 0
/* 800E34A8 000E03E8  40 80 00 08 */	bge lbl_800E34B0
/* 800E34AC 000E03EC  48 00 00 20 */	b lbl_800E34CC
lbl_800E34B0:
/* 800E34B0 000E03F0  80 01 00 40 */	lwz r0, 0x40(r1)
/* 800E34B4 000E03F4  48 00 00 1C */	b lbl_800E34D0
lbl_800E34B8:
/* 800E34B8 000E03F8  C0 01 00 40 */	lfs f0, 0x40(r1)
/* 800E34BC 000E03FC  FC 00 00 1E */	fctiwz f0, f0
/* 800E34C0 000E0400  D8 01 00 48 */	stfd f0, 0x48(r1)
/* 800E34C4 000E0404  80 01 00 4C */	lwz r0, 0x4c(r1)
/* 800E34C8 000E0408  48 00 00 08 */	b lbl_800E34D0
lbl_800E34CC:
/* 800E34CC 000E040C  38 00 00 00 */	li r0, 0
lbl_800E34D0:
/* 800E34D0 000E0410  80 6D 97 E8 */	lwz r3, gpMarDirector@sda21(r13)
/* 800E34D4 000E0414  54 04 06 3E */	clrlwi r4, r0, 0x18
/* 800E34D8 000E0418  48 00 A0 F1 */	bl fireStreamingMovie__12TMarDirectorFUc
/* 800E34DC 000E041C  38 00 00 00 */	li r0, 0
/* 800E34E0 000E0420  90 01 00 28 */	stw r0, 0x28(r1)
/* 800E34E4 000E0424  90 01 00 2C */	stw r0, 0x2c(r1)
/* 800E34E8 000E0428  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E34EC 000E042C  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800E34F0 000E0430  7C 03 00 00 */	cmpw r3, r0
/* 800E34F4 000E0434  41 80 00 18 */	blt lbl_800E350C
/* 800E34F8 000E0438  3C 60 80 37 */	lis r3, $$23834@ha
/* 800E34FC 000E043C  4C C6 31 82 */	crclr 6
/* 800E3500 000E0440  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800E3504 000E0444  4B FF 9B A1 */	bl SpcTrace__FPCce
/* 800E3508 000E0448  48 00 00 2C */	b lbl_800E3534
lbl_800E350C:
/* 800E350C 000E044C  80 BF 00 20 */	lwz r5, 0x20(r31)
/* 800E3510 000E0450  54 64 18 38 */	slwi r4, r3, 3
/* 800E3514 000E0454  80 61 00 28 */	lwz r3, 0x28(r1)
/* 800E3518 000E0458  80 01 00 2C */	lwz r0, 0x2c(r1)
/* 800E351C 000E045C  7C 85 22 14 */	add r4, r5, r4
/* 800E3520 000E0460  90 64 00 00 */	stw r3, 0(r4)
/* 800E3524 000E0464  90 04 00 04 */	stw r0, 4(r4)
/* 800E3528 000E0468  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E352C 000E046C  38 03 00 01 */	addi r0, r3, 1
/* 800E3530 000E0470  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800E3534:
/* 800E3534 000E0474  80 01 00 5C */	lwz r0, 0x5c(r1)
/* 800E3538 000E0478  83 E1 00 54 */	lwz r31, 0x54(r1)
/* 800E353C 000E047C  38 21 00 58 */	addi r1, r1, 0x58
/* 800E3540 000E0480  7C 08 03 A6 */	mtlr r0
/* 800E3544 000E0484  4E 80 00 20 */	blr 

.global evSetNextStage__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evSetNextStage__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E3548 000E0488  7C 08 02 A6 */	mflr r0
/* 800E354C 000E048C  90 01 00 04 */	stw r0, 4(r1)
/* 800E3550 000E0490  94 21 FF 78 */	stwu r1, -0x88(r1)
/* 800E3554 000E0494  93 E1 00 84 */	stw r31, 0x84(r1)
/* 800E3558 000E0498  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E355C 000E049C  93 C1 00 80 */	stw r30, 0x80(r1)
/* 800E3560 000E04A0  7C 7E 1B 78 */	mr r30, r3
/* 800E3564 000E04A4  93 A1 00 7C */	stw r29, 0x7c(r1)
/* 800E3568 000E04A8  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E356C 000E04AC  3C 80 80 37 */	lis r4, $$21490@ha
/* 800E3570 000E04B0  3B E4 29 90 */	addi r31, r4, $$21490@l
/* 800E3574 000E04B4  38 80 00 02 */	li r4, 2
/* 800E3578 000E04B8  48 03 3E B9 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E357C 000E04BC  80 7E 00 1C */	lwz r3, 0x1c(r30)
/* 800E3580 000E04C0  2C 03 00 00 */	cmpwi r3, 0
/* 800E3584 000E04C4  41 81 00 30 */	bgt lbl_800E35B4
/* 800E3588 000E04C8  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800E358C 000E04CC  4C C6 31 82 */	crclr 6
/* 800E3590 000E04D0  4B FF 9B 15 */	bl SpcTrace__FPCce
/* 800E3594 000E04D4  80 1E 00 1C */	lwz r0, 0x1c(r30)
/* 800E3598 000E04D8  80 7E 00 20 */	lwz r3, 0x20(r30)
/* 800E359C 000E04DC  54 00 18 38 */	slwi r0, r0, 3
/* 800E35A0 000E04E0  7C 63 02 14 */	add r3, r3, r0
/* 800E35A4 000E04E4  80 83 00 00 */	lwz r4, 0(r3)
/* 800E35A8 000E04E8  80 03 00 04 */	lwz r0, 4(r3)
/* 800E35AC 000E04EC  90 01 00 60 */	stw r0, 0x60(r1)
/* 800E35B0 000E04F0  48 00 00 28 */	b lbl_800E35D8
lbl_800E35B4:
/* 800E35B4 000E04F4  38 03 FF FF */	addi r0, r3, -1
/* 800E35B8 000E04F8  90 1E 00 1C */	stw r0, 0x1c(r30)
/* 800E35BC 000E04FC  80 1E 00 1C */	lwz r0, 0x1c(r30)
/* 800E35C0 000E0500  80 7E 00 20 */	lwz r3, 0x20(r30)
/* 800E35C4 000E0504  54 00 18 38 */	slwi r0, r0, 3
/* 800E35C8 000E0508  7C 63 02 14 */	add r3, r3, r0
/* 800E35CC 000E050C  80 83 00 00 */	lwz r4, 0(r3)
/* 800E35D0 000E0510  80 03 00 04 */	lwz r0, 4(r3)
/* 800E35D4 000E0514  90 01 00 60 */	stw r0, 0x60(r1)
lbl_800E35D8:
/* 800E35D8 000E0518  90 81 00 64 */	stw r4, 0x64(r1)
/* 800E35DC 000E051C  80 01 00 60 */	lwz r0, 0x60(r1)
/* 800E35E0 000E0520  90 01 00 68 */	stw r0, 0x68(r1)
/* 800E35E4 000E0524  80 01 00 64 */	lwz r0, 0x64(r1)
/* 800E35E8 000E0528  2C 00 00 01 */	cmpwi r0, 1
/* 800E35EC 000E052C  41 82 00 1C */	beq lbl_800E3608
/* 800E35F0 000E0530  40 80 00 2C */	bge lbl_800E361C
/* 800E35F4 000E0534  2C 00 00 00 */	cmpwi r0, 0
/* 800E35F8 000E0538  40 80 00 08 */	bge lbl_800E3600
/* 800E35FC 000E053C  48 00 00 20 */	b lbl_800E361C
lbl_800E3600:
/* 800E3600 000E0540  83 A1 00 68 */	lwz r29, 0x68(r1)
/* 800E3604 000E0544  48 00 00 1C */	b lbl_800E3620
lbl_800E3608:
/* 800E3608 000E0548  C0 01 00 68 */	lfs f0, 0x68(r1)
/* 800E360C 000E054C  FC 00 00 1E */	fctiwz f0, f0
/* 800E3610 000E0550  D8 01 00 70 */	stfd f0, 0x70(r1)
/* 800E3614 000E0554  83 A1 00 74 */	lwz r29, 0x74(r1)
/* 800E3618 000E0558  48 00 00 08 */	b lbl_800E3620
lbl_800E361C:
/* 800E361C 000E055C  3B A0 00 00 */	li r29, 0
lbl_800E3620:
/* 800E3620 000E0560  80 7E 00 1C */	lwz r3, 0x1c(r30)
/* 800E3624 000E0564  2C 03 00 00 */	cmpwi r3, 0
/* 800E3628 000E0568  41 81 00 30 */	bgt lbl_800E3658
/* 800E362C 000E056C  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800E3630 000E0570  4C C6 31 82 */	crclr 6
/* 800E3634 000E0574  4B FF 9A 71 */	bl SpcTrace__FPCce
/* 800E3638 000E0578  80 1E 00 1C */	lwz r0, 0x1c(r30)
/* 800E363C 000E057C  80 7E 00 20 */	lwz r3, 0x20(r30)
/* 800E3640 000E0580  54 00 18 38 */	slwi r0, r0, 3
/* 800E3644 000E0584  7C 63 02 14 */	add r3, r3, r0
/* 800E3648 000E0588  80 83 00 00 */	lwz r4, 0(r3)
/* 800E364C 000E058C  80 03 00 04 */	lwz r0, 4(r3)
/* 800E3650 000E0590  90 01 00 50 */	stw r0, 0x50(r1)
/* 800E3654 000E0594  48 00 00 28 */	b lbl_800E367C
lbl_800E3658:
/* 800E3658 000E0598  38 03 FF FF */	addi r0, r3, -1
/* 800E365C 000E059C  90 1E 00 1C */	stw r0, 0x1c(r30)
/* 800E3660 000E05A0  80 1E 00 1C */	lwz r0, 0x1c(r30)
/* 800E3664 000E05A4  80 7E 00 20 */	lwz r3, 0x20(r30)
/* 800E3668 000E05A8  54 00 18 38 */	slwi r0, r0, 3
/* 800E366C 000E05AC  7C 63 02 14 */	add r3, r3, r0
/* 800E3670 000E05B0  80 83 00 00 */	lwz r4, 0(r3)
/* 800E3674 000E05B4  80 03 00 04 */	lwz r0, 4(r3)
/* 800E3678 000E05B8  90 01 00 50 */	stw r0, 0x50(r1)
lbl_800E367C:
/* 800E367C 000E05BC  90 81 00 54 */	stw r4, 0x54(r1)
/* 800E3680 000E05C0  80 01 00 50 */	lwz r0, 0x50(r1)
/* 800E3684 000E05C4  90 01 00 58 */	stw r0, 0x58(r1)
/* 800E3688 000E05C8  80 01 00 54 */	lwz r0, 0x54(r1)
/* 800E368C 000E05CC  2C 00 00 01 */	cmpwi r0, 1
/* 800E3690 000E05D0  41 82 00 1C */	beq lbl_800E36AC
/* 800E3694 000E05D4  40 80 00 2C */	bge lbl_800E36C0
/* 800E3698 000E05D8  2C 00 00 00 */	cmpwi r0, 0
/* 800E369C 000E05DC  40 80 00 08 */	bge lbl_800E36A4
/* 800E36A0 000E05E0  48 00 00 20 */	b lbl_800E36C0
lbl_800E36A4:
/* 800E36A4 000E05E4  80 61 00 58 */	lwz r3, 0x58(r1)
/* 800E36A8 000E05E8  48 00 00 1C */	b lbl_800E36C4
lbl_800E36AC:
/* 800E36AC 000E05EC  C0 01 00 58 */	lfs f0, 0x58(r1)
/* 800E36B0 000E05F0  FC 00 00 1E */	fctiwz f0, f0
/* 800E36B4 000E05F4  D8 01 00 70 */	stfd f0, 0x70(r1)
/* 800E36B8 000E05F8  80 61 00 74 */	lwz r3, 0x74(r1)
/* 800E36BC 000E05FC  48 00 00 08 */	b lbl_800E36C4
lbl_800E36C0:
/* 800E36C0 000E0600  38 60 00 00 */	li r3, 0
lbl_800E36C4:
/* 800E36C4 000E0604  38 03 00 01 */	addi r0, r3, 1
/* 800E36C8 000E0608  80 6D 97 E8 */	lwz r3, gpMarDirector@sda21(r13)
/* 800E36CC 000E060C  57 A4 06 3E */	clrlwi r4, r29, 0x18
/* 800E36D0 000E0610  54 00 40 2E */	slwi r0, r0, 8
/* 800E36D4 000E0614  7C 84 02 14 */	add r4, r4, r0
/* 800E36D8 000E0618  38 A0 00 00 */	li r5, 0
/* 800E36DC 000E061C  48 00 A1 C5 */	bl setNextStage__12TMarDirectorFUsPQ26JDrama6TActor
/* 800E36E0 000E0620  38 00 00 00 */	li r0, 0
/* 800E36E4 000E0624  90 01 00 3C */	stw r0, 0x3c(r1)
/* 800E36E8 000E0628  90 01 00 40 */	stw r0, 0x40(r1)
/* 800E36EC 000E062C  80 7E 00 1C */	lwz r3, 0x1c(r30)
/* 800E36F0 000E0630  80 1E 00 18 */	lwz r0, 0x18(r30)
/* 800E36F4 000E0634  7C 03 00 00 */	cmpw r3, r0
/* 800E36F8 000E0638  41 80 00 14 */	blt lbl_800E370C
/* 800E36FC 000E063C  38 7F 07 8C */	addi r3, r31, 0x78c
/* 800E3700 000E0640  4C C6 31 82 */	crclr 6
/* 800E3704 000E0644  4B FF 99 A1 */	bl SpcTrace__FPCce
/* 800E3708 000E0648  48 00 00 2C */	b lbl_800E3734
lbl_800E370C:
/* 800E370C 000E064C  80 BE 00 20 */	lwz r5, 0x20(r30)
/* 800E3710 000E0650  54 64 18 38 */	slwi r4, r3, 3
/* 800E3714 000E0654  80 61 00 3C */	lwz r3, 0x3c(r1)
/* 800E3718 000E0658  80 01 00 40 */	lwz r0, 0x40(r1)
/* 800E371C 000E065C  7C 85 22 14 */	add r4, r5, r4
/* 800E3720 000E0660  90 64 00 00 */	stw r3, 0(r4)
/* 800E3724 000E0664  90 04 00 04 */	stw r0, 4(r4)
/* 800E3728 000E0668  80 7E 00 1C */	lwz r3, 0x1c(r30)
/* 800E372C 000E066C  38 03 00 01 */	addi r0, r3, 1
/* 800E3730 000E0670  90 1E 00 1C */	stw r0, 0x1c(r30)
lbl_800E3734:
/* 800E3734 000E0674  80 01 00 8C */	lwz r0, 0x8c(r1)
/* 800E3738 000E0678  83 E1 00 84 */	lwz r31, 0x84(r1)
/* 800E373C 000E067C  83 C1 00 80 */	lwz r30, 0x80(r1)
/* 800E3740 000E0680  7C 08 03 A6 */	mtlr r0
/* 800E3744 000E0684  83 A1 00 7C */	lwz r29, 0x7c(r1)
/* 800E3748 000E0688  38 21 00 88 */	addi r1, r1, 0x88
/* 800E374C 000E068C  4E 80 00 20 */	blr 

.global evGetPollutionLevel__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evGetPollutionLevel__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E3750 000E0690  7C 08 02 A6 */	mflr r0
/* 800E3754 000E0694  90 01 00 04 */	stw r0, 4(r1)
/* 800E3758 000E0698  94 21 FF E0 */	stwu r1, -0x20(r1)
/* 800E375C 000E069C  93 E1 00 1C */	stw r31, 0x1c(r1)
/* 800E3760 000E06A0  7C 7F 1B 78 */	mr r31, r3
/* 800E3764 000E06A4  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E3768 000E06A8  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E376C 000E06AC  38 80 00 00 */	li r4, 0
/* 800E3770 000E06B0  48 03 3C C1 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E3774 000E06B4  80 6D 9A E8 */	lwz r3, gpPollution@sda21(r13)
/* 800E3778 000E06B8  48 10 BF 41 */	bl getPollutionDegree__17TPollutionManagerCFv
/* 800E377C 000E06BC  38 00 00 00 */	li r0, 0
/* 800E3780 000E06C0  90 01 00 10 */	stw r0, 0x10(r1)
/* 800E3784 000E06C4  90 61 00 14 */	stw r3, 0x14(r1)
/* 800E3788 000E06C8  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E378C 000E06CC  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800E3790 000E06D0  7C 03 00 00 */	cmpw r3, r0
/* 800E3794 000E06D4  41 80 00 18 */	blt lbl_800E37AC
/* 800E3798 000E06D8  3C 60 80 37 */	lis r3, $$23834@ha
/* 800E379C 000E06DC  4C C6 31 82 */	crclr 6
/* 800E37A0 000E06E0  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800E37A4 000E06E4  4B FF 99 01 */	bl SpcTrace__FPCce
/* 800E37A8 000E06E8  48 00 00 2C */	b lbl_800E37D4
lbl_800E37AC:
/* 800E37AC 000E06EC  80 BF 00 20 */	lwz r5, 0x20(r31)
/* 800E37B0 000E06F0  54 64 18 38 */	slwi r4, r3, 3
/* 800E37B4 000E06F4  80 61 00 10 */	lwz r3, 0x10(r1)
/* 800E37B8 000E06F8  80 01 00 14 */	lwz r0, 0x14(r1)
/* 800E37BC 000E06FC  7C 85 22 14 */	add r4, r5, r4
/* 800E37C0 000E0700  90 64 00 00 */	stw r3, 0(r4)
/* 800E37C4 000E0704  90 04 00 04 */	stw r0, 4(r4)
/* 800E37C8 000E0708  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E37CC 000E070C  38 03 00 01 */	addi r0, r3, 1
/* 800E37D0 000E0710  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800E37D4:
/* 800E37D4 000E0714  80 01 00 24 */	lwz r0, 0x24(r1)
/* 800E37D8 000E0718  83 E1 00 1C */	lwz r31, 0x1c(r1)
/* 800E37DC 000E071C  38 21 00 20 */	addi r1, r1, 0x20
/* 800E37E0 000E0720  7C 08 03 A6 */	mtlr r0
/* 800E37E4 000E0724  4E 80 00 20 */	blr 

.global evGetRestTime__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evGetRestTime__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E37E8 000E0728  7C 08 02 A6 */	mflr r0
/* 800E37EC 000E072C  90 01 00 04 */	stw r0, 4(r1)
/* 800E37F0 000E0730  94 21 FF D0 */	stwu r1, -0x30(r1)
/* 800E37F4 000E0734  93 E1 00 2C */	stw r31, 0x2c(r1)
/* 800E37F8 000E0738  7C 7F 1B 78 */	mr r31, r3
/* 800E37FC 000E073C  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E3800 000E0740  93 C1 00 28 */	stw r30, 0x28(r1)
/* 800E3804 000E0744  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E3808 000E0748  38 80 00 00 */	li r4, 0
/* 800E380C 000E074C  48 03 3C 25 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E3810 000E0750  83 CD 97 E8 */	lwz r30, gpMarDirector@sda21(r13)
/* 800E3814 000E0754  38 7E 00 E8 */	addi r3, r30, 0xe8
/* 800E3818 000E0758  4B FA F2 1D */	bl OSCheckStopwatch
/* 800E381C 000E075C  3C A0 80 00 */	lis r5, 0x800000F8@ha
/* 800E3820 000E0760  80 05 00 F8 */	lwz r0, 0x800000F8@l(r5)
/* 800E3824 000E0764  3C A0 10 62 */	lis r5, 0x10624DD3@ha
/* 800E3828 000E0768  38 A5 4D D3 */	addi r5, r5, 0x10624DD3@l
/* 800E382C 000E076C  54 00 F0 BE */	srwi r0, r0, 2
/* 800E3830 000E0770  7C 05 00 16 */	mulhwu r0, r5, r0
/* 800E3834 000E0774  54 06 D1 BE */	srwi r6, r0, 6
/* 800E3838 000E0778  38 A0 00 00 */	li r5, 0
/* 800E383C 000E077C  4B F9 F4 C9 */	bl __div2i
/* 800E3840 000E0780  38 A0 00 00 */	li r5, 0
/* 800E3844 000E0784  38 C0 00 0A */	li r6, 0xa
/* 800E3848 000E0788  4B F9 F4 BD */	bl __div2i
/* 800E384C 000E078C  38 00 00 00 */	li r0, 0
/* 800E3850 000E0790  90 01 00 20 */	stw r0, 0x20(r1)
/* 800E3854 000E0794  80 1E 01 20 */	lwz r0, 0x120(r30)
/* 800E3858 000E0798  7C 04 00 50 */	subf r0, r4, r0
/* 800E385C 000E079C  90 01 00 24 */	stw r0, 0x24(r1)
/* 800E3860 000E07A0  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E3864 000E07A4  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800E3868 000E07A8  7C 03 00 00 */	cmpw r3, r0
/* 800E386C 000E07AC  41 80 00 18 */	blt lbl_800E3884
/* 800E3870 000E07B0  3C 60 80 37 */	lis r3, $$23834@ha
/* 800E3874 000E07B4  4C C6 31 82 */	crclr 6
/* 800E3878 000E07B8  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800E387C 000E07BC  4B FF 98 29 */	bl SpcTrace__FPCce
/* 800E3880 000E07C0  48 00 00 2C */	b lbl_800E38AC
lbl_800E3884:
/* 800E3884 000E07C4  80 BF 00 20 */	lwz r5, 0x20(r31)
/* 800E3888 000E07C8  54 64 18 38 */	slwi r4, r3, 3
/* 800E388C 000E07CC  80 61 00 20 */	lwz r3, 0x20(r1)
/* 800E3890 000E07D0  80 01 00 24 */	lwz r0, 0x24(r1)
/* 800E3894 000E07D4  7C 85 22 14 */	add r4, r5, r4
/* 800E3898 000E07D8  90 64 00 00 */	stw r3, 0(r4)
/* 800E389C 000E07DC  90 04 00 04 */	stw r0, 4(r4)
/* 800E38A0 000E07E0  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E38A4 000E07E4  38 03 00 01 */	addi r0, r3, 1
/* 800E38A8 000E07E8  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800E38AC:
/* 800E38AC 000E07EC  80 01 00 34 */	lwz r0, 0x34(r1)
/* 800E38B0 000E07F0  83 E1 00 2C */	lwz r31, 0x2c(r1)
/* 800E38B4 000E07F4  83 C1 00 28 */	lwz r30, 0x28(r1)
/* 800E38B8 000E07F8  7C 08 03 A6 */	mtlr r0
/* 800E38BC 000E07FC  38 21 00 30 */	addi r1, r1, 0x30
/* 800E38C0 000E0800  4E 80 00 20 */	blr 

.global evSetPollutionIncreaseCount__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evSetPollutionIncreaseCount__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E38C4 000E0804  7C 08 02 A6 */	mflr r0
/* 800E38C8 000E0808  90 01 00 04 */	stw r0, 4(r1)
/* 800E38CC 000E080C  94 21 FF 90 */	stwu r1, -0x70(r1)
/* 800E38D0 000E0810  93 E1 00 6C */	stw r31, 0x6c(r1)
/* 800E38D4 000E0814  7C 7F 1B 78 */	mr r31, r3
/* 800E38D8 000E0818  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E38DC 000E081C  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E38E0 000E0820  38 80 00 01 */	li r4, 1
/* 800E38E4 000E0824  48 03 3B 4D */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E38E8 000E0828  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E38EC 000E082C  2C 03 00 00 */	cmpwi r3, 0
/* 800E38F0 000E0830  41 81 00 18 */	bgt lbl_800E3908
/* 800E38F4 000E0834  3C 60 80 37 */	lis r3, $$23829@ha
/* 800E38F8 000E0838  4C C6 31 82 */	crclr 6
/* 800E38FC 000E083C  38 63 30 FC */	addi r3, r3, $$23829@l
/* 800E3900 000E0840  4B FF 97 A5 */	bl SpcTrace__FPCce
/* 800E3904 000E0844  48 00 00 0C */	b lbl_800E3910
lbl_800E3908:
/* 800E3908 000E0848  38 03 FF FF */	addi r0, r3, -1
/* 800E390C 000E084C  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800E3910:
/* 800E3910 000E0850  38 00 00 00 */	li r0, 0
/* 800E3914 000E0854  90 01 00 40 */	stw r0, 0x40(r1)
/* 800E3918 000E0858  90 01 00 44 */	stw r0, 0x44(r1)
/* 800E391C 000E085C  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E3920 000E0860  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800E3924 000E0864  7C 03 00 00 */	cmpw r3, r0
/* 800E3928 000E0868  41 80 00 18 */	blt lbl_800E3940
/* 800E392C 000E086C  3C 60 80 37 */	lis r3, $$23834@ha
/* 800E3930 000E0870  4C C6 31 82 */	crclr 6
/* 800E3934 000E0874  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800E3938 000E0878  4B FF 97 6D */	bl SpcTrace__FPCce
/* 800E393C 000E087C  48 00 00 2C */	b lbl_800E3968
lbl_800E3940:
/* 800E3940 000E0880  80 BF 00 20 */	lwz r5, 0x20(r31)
/* 800E3944 000E0884  54 64 18 38 */	slwi r4, r3, 3
/* 800E3948 000E0888  80 61 00 40 */	lwz r3, 0x40(r1)
/* 800E394C 000E088C  80 01 00 44 */	lwz r0, 0x44(r1)
/* 800E3950 000E0890  7C 85 22 14 */	add r4, r5, r4
/* 800E3954 000E0894  90 64 00 00 */	stw r3, 0(r4)
/* 800E3958 000E0898  90 04 00 04 */	stw r0, 4(r4)
/* 800E395C 000E089C  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E3960 000E08A0  38 03 00 01 */	addi r0, r3, 1
/* 800E3964 000E08A4  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800E3968:
/* 800E3968 000E08A8  80 01 00 74 */	lwz r0, 0x74(r1)
/* 800E396C 000E08AC  83 E1 00 6C */	lwz r31, 0x6c(r1)
/* 800E3970 000E08B0  38 21 00 70 */	addi r1, r1, 0x70
/* 800E3974 000E08B4  7C 08 03 A6 */	mtlr r0
/* 800E3978 000E08B8  4E 80 00 20 */	blr 

.global evSetAttentionTime__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evSetAttentionTime__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E397C 000E08BC  7C 08 02 A6 */	mflr r0
/* 800E3980 000E08C0  90 01 00 04 */	stw r0, 4(r1)
/* 800E3984 000E08C4  94 21 FF 90 */	stwu r1, -0x70(r1)
/* 800E3988 000E08C8  93 E1 00 6C */	stw r31, 0x6c(r1)
/* 800E398C 000E08CC  7C 7F 1B 78 */	mr r31, r3
/* 800E3990 000E08D0  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E3994 000E08D4  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E3998 000E08D8  38 80 00 01 */	li r4, 1
/* 800E399C 000E08DC  48 03 3A 95 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E39A0 000E08E0  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E39A4 000E08E4  2C 03 00 00 */	cmpwi r3, 0
/* 800E39A8 000E08E8  41 81 00 18 */	bgt lbl_800E39C0
/* 800E39AC 000E08EC  3C 60 80 37 */	lis r3, $$23829@ha
/* 800E39B0 000E08F0  4C C6 31 82 */	crclr 6
/* 800E39B4 000E08F4  38 63 30 FC */	addi r3, r3, $$23829@l
/* 800E39B8 000E08F8  4B FF 96 ED */	bl SpcTrace__FPCce
/* 800E39BC 000E08FC  48 00 00 0C */	b lbl_800E39C8
lbl_800E39C0:
/* 800E39C0 000E0900  38 03 FF FF */	addi r0, r3, -1
/* 800E39C4 000E0904  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800E39C8:
/* 800E39C8 000E0908  38 00 00 00 */	li r0, 0
/* 800E39CC 000E090C  90 01 00 40 */	stw r0, 0x40(r1)
/* 800E39D0 000E0910  90 01 00 44 */	stw r0, 0x44(r1)
/* 800E39D4 000E0914  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E39D8 000E0918  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800E39DC 000E091C  7C 03 00 00 */	cmpw r3, r0
/* 800E39E0 000E0920  41 80 00 18 */	blt lbl_800E39F8
/* 800E39E4 000E0924  3C 60 80 37 */	lis r3, $$23834@ha
/* 800E39E8 000E0928  4C C6 31 82 */	crclr 6
/* 800E39EC 000E092C  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800E39F0 000E0930  4B FF 96 B5 */	bl SpcTrace__FPCce
/* 800E39F4 000E0934  48 00 00 2C */	b lbl_800E3A20
lbl_800E39F8:
/* 800E39F8 000E0938  80 BF 00 20 */	lwz r5, 0x20(r31)
/* 800E39FC 000E093C  54 64 18 38 */	slwi r4, r3, 3
/* 800E3A00 000E0940  80 61 00 40 */	lwz r3, 0x40(r1)
/* 800E3A04 000E0944  80 01 00 44 */	lwz r0, 0x44(r1)
/* 800E3A08 000E0948  7C 85 22 14 */	add r4, r5, r4
/* 800E3A0C 000E094C  90 64 00 00 */	stw r3, 0(r4)
/* 800E3A10 000E0950  90 04 00 04 */	stw r0, 4(r4)
/* 800E3A14 000E0954  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E3A18 000E0958  38 03 00 01 */	addi r0, r3, 1
/* 800E3A1C 000E095C  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800E3A20:
/* 800E3A20 000E0960  80 01 00 74 */	lwz r0, 0x74(r1)
/* 800E3A24 000E0964  83 E1 00 6C */	lwz r31, 0x6c(r1)
/* 800E3A28 000E0968  38 21 00 70 */	addi r1, r1, 0x70
/* 800E3A2C 000E096C  7C 08 03 A6 */	mtlr r0
/* 800E3A30 000E0970  4E 80 00 20 */	blr 

.global evSetTimeLimit__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evSetTimeLimit__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E3A34 000E0974  7C 08 02 A6 */	mflr r0
/* 800E3A38 000E0978  90 01 00 04 */	stw r0, 4(r1)
/* 800E3A3C 000E097C  94 21 FF 98 */	stwu r1, -0x68(r1)
/* 800E3A40 000E0980  93 E1 00 64 */	stw r31, 0x64(r1)
/* 800E3A44 000E0984  7C 7F 1B 78 */	mr r31, r3
/* 800E3A48 000E0988  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E3A4C 000E098C  93 C1 00 60 */	stw r30, 0x60(r1)
/* 800E3A50 000E0990  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E3A54 000E0994  38 80 00 01 */	li r4, 1
/* 800E3A58 000E0998  48 03 39 D9 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E3A5C 000E099C  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E3A60 000E09A0  2C 03 00 00 */	cmpwi r3, 0
/* 800E3A64 000E09A4  41 81 00 34 */	bgt lbl_800E3A98
/* 800E3A68 000E09A8  3C 60 80 37 */	lis r3, $$23829@ha
/* 800E3A6C 000E09AC  4C C6 31 82 */	crclr 6
/* 800E3A70 000E09B0  38 63 30 FC */	addi r3, r3, $$23829@l
/* 800E3A74 000E09B4  4B FF 96 31 */	bl SpcTrace__FPCce
/* 800E3A78 000E09B8  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800E3A7C 000E09BC  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800E3A80 000E09C0  54 00 18 38 */	slwi r0, r0, 3
/* 800E3A84 000E09C4  7C 63 02 14 */	add r3, r3, r0
/* 800E3A88 000E09C8  80 83 00 00 */	lwz r4, 0(r3)
/* 800E3A8C 000E09CC  80 03 00 04 */	lwz r0, 4(r3)
/* 800E3A90 000E09D0  90 01 00 48 */	stw r0, 0x48(r1)
/* 800E3A94 000E09D4  48 00 00 28 */	b lbl_800E3ABC
lbl_800E3A98:
/* 800E3A98 000E09D8  38 03 FF FF */	addi r0, r3, -1
/* 800E3A9C 000E09DC  90 1F 00 1C */	stw r0, 0x1c(r31)
/* 800E3AA0 000E09E0  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800E3AA4 000E09E4  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800E3AA8 000E09E8  54 00 18 38 */	slwi r0, r0, 3
/* 800E3AAC 000E09EC  7C 63 02 14 */	add r3, r3, r0
/* 800E3AB0 000E09F0  80 83 00 00 */	lwz r4, 0(r3)
/* 800E3AB4 000E09F4  80 03 00 04 */	lwz r0, 4(r3)
/* 800E3AB8 000E09F8  90 01 00 48 */	stw r0, 0x48(r1)
lbl_800E3ABC:
/* 800E3ABC 000E09FC  90 81 00 4C */	stw r4, 0x4c(r1)
/* 800E3AC0 000E0A00  80 01 00 48 */	lwz r0, 0x48(r1)
/* 800E3AC4 000E0A04  90 01 00 50 */	stw r0, 0x50(r1)
/* 800E3AC8 000E0A08  80 01 00 4C */	lwz r0, 0x4c(r1)
/* 800E3ACC 000E0A0C  2C 00 00 01 */	cmpwi r0, 1
/* 800E3AD0 000E0A10  41 82 00 1C */	beq lbl_800E3AEC
/* 800E3AD4 000E0A14  40 80 00 2C */	bge lbl_800E3B00
/* 800E3AD8 000E0A18  2C 00 00 00 */	cmpwi r0, 0
/* 800E3ADC 000E0A1C  40 80 00 08 */	bge lbl_800E3AE4
/* 800E3AE0 000E0A20  48 00 00 20 */	b lbl_800E3B00
lbl_800E3AE4:
/* 800E3AE4 000E0A24  83 C1 00 50 */	lwz r30, 0x50(r1)
/* 800E3AE8 000E0A28  48 00 00 1C */	b lbl_800E3B04
lbl_800E3AEC:
/* 800E3AEC 000E0A2C  C0 01 00 50 */	lfs f0, 0x50(r1)
/* 800E3AF0 000E0A30  FC 00 00 1E */	fctiwz f0, f0
/* 800E3AF4 000E0A34  D8 01 00 58 */	stfd f0, 0x58(r1)
/* 800E3AF8 000E0A38  83 C1 00 5C */	lwz r30, 0x5c(r1)
/* 800E3AFC 000E0A3C  48 00 00 08 */	b lbl_800E3B04
lbl_800E3B00:
/* 800E3B00 000E0A40  3B C0 00 00 */	li r30, 0
lbl_800E3B04:
/* 800E3B04 000E0A44  80 6D 97 E8 */	lwz r3, gpMarDirector@sda21(r13)
/* 800E3B08 000E0A48  38 63 00 E8 */	addi r3, r3, 0xe8
/* 800E3B0C 000E0A4C  4B FA EF 99 */	bl OSResetStopwatch
/* 800E3B10 000E0A50  80 6D 97 E8 */	lwz r3, gpMarDirector@sda21(r13)
/* 800E3B14 000E0A54  38 00 00 00 */	li r0, 0
/* 800E3B18 000E0A58  93 C3 01 20 */	stw r30, 0x120(r3)
/* 800E3B1C 000E0A5C  90 01 00 34 */	stw r0, 0x34(r1)
/* 800E3B20 000E0A60  90 01 00 38 */	stw r0, 0x38(r1)
/* 800E3B24 000E0A64  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E3B28 000E0A68  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800E3B2C 000E0A6C  7C 03 00 00 */	cmpw r3, r0
/* 800E3B30 000E0A70  41 80 00 18 */	blt lbl_800E3B48
/* 800E3B34 000E0A74  3C 60 80 37 */	lis r3, $$23834@ha
/* 800E3B38 000E0A78  4C C6 31 82 */	crclr 6
/* 800E3B3C 000E0A7C  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800E3B40 000E0A80  4B FF 95 65 */	bl SpcTrace__FPCce
/* 800E3B44 000E0A84  48 00 00 2C */	b lbl_800E3B70
lbl_800E3B48:
/* 800E3B48 000E0A88  80 BF 00 20 */	lwz r5, 0x20(r31)
/* 800E3B4C 000E0A8C  54 64 18 38 */	slwi r4, r3, 3
/* 800E3B50 000E0A90  80 61 00 34 */	lwz r3, 0x34(r1)
/* 800E3B54 000E0A94  80 01 00 38 */	lwz r0, 0x38(r1)
/* 800E3B58 000E0A98  7C 85 22 14 */	add r4, r5, r4
/* 800E3B5C 000E0A9C  90 64 00 00 */	stw r3, 0(r4)
/* 800E3B60 000E0AA0  90 04 00 04 */	stw r0, 4(r4)
/* 800E3B64 000E0AA4  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E3B68 000E0AA8  38 03 00 01 */	addi r0, r3, 1
/* 800E3B6C 000E0AAC  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800E3B70:
/* 800E3B70 000E0AB0  80 01 00 6C */	lwz r0, 0x6c(r1)
/* 800E3B74 000E0AB4  83 E1 00 64 */	lwz r31, 0x64(r1)
/* 800E3B78 000E0AB8  83 C1 00 60 */	lwz r30, 0x60(r1)
/* 800E3B7C 000E0ABC  7C 08 03 A6 */	mtlr r0
/* 800E3B80 000E0AC0  38 21 00 68 */	addi r1, r1, 0x68
/* 800E3B84 000E0AC4  4E 80 00 20 */	blr 

.global evSetDead4LiveActor__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evSetDead4LiveActor__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E3B88 000E0AC8  7C 08 02 A6 */	mflr r0
/* 800E3B8C 000E0ACC  90 01 00 04 */	stw r0, 4(r1)
/* 800E3B90 000E0AD0  94 21 FF 68 */	stwu r1, -0x98(r1)
/* 800E3B94 000E0AD4  BF 61 00 84 */	stmw r27, 0x84(r1)
/* 800E3B98 000E0AD8  7C 7D 1B 78 */	mr r29, r3
/* 800E3B9C 000E0ADC  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E3BA0 000E0AE0  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E3BA4 000E0AE4  3C 80 80 37 */	lis r4, $$21490@ha
/* 800E3BA8 000E0AE8  3B E4 29 90 */	addi r31, r4, $$21490@l
/* 800E3BAC 000E0AEC  38 80 00 02 */	li r4, 2
/* 800E3BB0 000E0AF0  48 03 38 81 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E3BB4 000E0AF4  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E3BB8 000E0AF8  2C 03 00 00 */	cmpwi r3, 0
/* 800E3BBC 000E0AFC  41 81 00 30 */	bgt lbl_800E3BEC
/* 800E3BC0 000E0B00  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800E3BC4 000E0B04  4C C6 31 82 */	crclr 6
/* 800E3BC8 000E0B08  4B FF 94 DD */	bl SpcTrace__FPCce
/* 800E3BCC 000E0B0C  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E3BD0 000E0B10  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E3BD4 000E0B14  54 00 18 38 */	slwi r0, r0, 3
/* 800E3BD8 000E0B18  7C 63 02 14 */	add r3, r3, r0
/* 800E3BDC 000E0B1C  80 83 00 00 */	lwz r4, 0(r3)
/* 800E3BE0 000E0B20  80 03 00 04 */	lwz r0, 4(r3)
/* 800E3BE4 000E0B24  90 01 00 6C */	stw r0, 0x6c(r1)
/* 800E3BE8 000E0B28  48 00 00 28 */	b lbl_800E3C10
lbl_800E3BEC:
/* 800E3BEC 000E0B2C  38 03 FF FF */	addi r0, r3, -1
/* 800E3BF0 000E0B30  90 1D 00 1C */	stw r0, 0x1c(r29)
/* 800E3BF4 000E0B34  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E3BF8 000E0B38  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E3BFC 000E0B3C  54 00 18 38 */	slwi r0, r0, 3
/* 800E3C00 000E0B40  7C 63 02 14 */	add r3, r3, r0
/* 800E3C04 000E0B44  80 83 00 00 */	lwz r4, 0(r3)
/* 800E3C08 000E0B48  80 03 00 04 */	lwz r0, 4(r3)
/* 800E3C0C 000E0B4C  90 01 00 6C */	stw r0, 0x6c(r1)
lbl_800E3C10:
/* 800E3C10 000E0B50  90 81 00 70 */	stw r4, 0x70(r1)
/* 800E3C14 000E0B54  80 01 00 6C */	lwz r0, 0x6c(r1)
/* 800E3C18 000E0B58  90 01 00 74 */	stw r0, 0x74(r1)
/* 800E3C1C 000E0B5C  80 01 00 70 */	lwz r0, 0x70(r1)
/* 800E3C20 000E0B60  2C 00 00 01 */	cmpwi r0, 1
/* 800E3C24 000E0B64  41 82 00 1C */	beq lbl_800E3C40
/* 800E3C28 000E0B68  40 80 00 2C */	bge lbl_800E3C54
/* 800E3C2C 000E0B6C  2C 00 00 00 */	cmpwi r0, 0
/* 800E3C30 000E0B70  40 80 00 08 */	bge lbl_800E3C38
/* 800E3C34 000E0B74  48 00 00 20 */	b lbl_800E3C54
lbl_800E3C38:
/* 800E3C38 000E0B78  83 C1 00 74 */	lwz r30, 0x74(r1)
/* 800E3C3C 000E0B7C  48 00 00 1C */	b lbl_800E3C58
lbl_800E3C40:
/* 800E3C40 000E0B80  C0 01 00 74 */	lfs f0, 0x74(r1)
/* 800E3C44 000E0B84  FC 00 00 1E */	fctiwz f0, f0
/* 800E3C48 000E0B88  D8 01 00 78 */	stfd f0, 0x78(r1)
/* 800E3C4C 000E0B8C  83 C1 00 7C */	lwz r30, 0x7c(r1)
/* 800E3C50 000E0B90  48 00 00 08 */	b lbl_800E3C58
lbl_800E3C54:
/* 800E3C54 000E0B94  3B C0 00 00 */	li r30, 0
lbl_800E3C58:
/* 800E3C58 000E0B98  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E3C5C 000E0B9C  2C 03 00 00 */	cmpwi r3, 0
/* 800E3C60 000E0BA0  41 81 00 34 */	bgt lbl_800E3C94
/* 800E3C64 000E0BA4  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800E3C68 000E0BA8  4C C6 31 82 */	crclr 6
/* 800E3C6C 000E0BAC  4B FF 94 39 */	bl SpcTrace__FPCce
/* 800E3C70 000E0BB0  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E3C74 000E0BB4  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E3C78 000E0BB8  54 00 18 38 */	slwi r0, r0, 3
/* 800E3C7C 000E0BBC  7C 63 02 14 */	add r3, r3, r0
/* 800E3C80 000E0BC0  80 03 00 00 */	lwz r0, 0(r3)
/* 800E3C84 000E0BC4  90 01 00 60 */	stw r0, 0x60(r1)
/* 800E3C88 000E0BC8  80 03 00 04 */	lwz r0, 4(r3)
/* 800E3C8C 000E0BCC  90 01 00 64 */	stw r0, 0x64(r1)
/* 800E3C90 000E0BD0  48 00 00 2C */	b lbl_800E3CBC
lbl_800E3C94:
/* 800E3C94 000E0BD4  38 03 FF FF */	addi r0, r3, -1
/* 800E3C98 000E0BD8  90 1D 00 1C */	stw r0, 0x1c(r29)
/* 800E3C9C 000E0BDC  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E3CA0 000E0BE0  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E3CA4 000E0BE4  54 00 18 38 */	slwi r0, r0, 3
/* 800E3CA8 000E0BE8  7C 63 02 14 */	add r3, r3, r0
/* 800E3CAC 000E0BEC  80 03 00 00 */	lwz r0, 0(r3)
/* 800E3CB0 000E0BF0  90 01 00 60 */	stw r0, 0x60(r1)
/* 800E3CB4 000E0BF4  80 03 00 04 */	lwz r0, 4(r3)
/* 800E3CB8 000E0BF8  90 01 00 64 */	stw r0, 0x64(r1)
lbl_800E3CBC:
/* 800E3CBC 000E0BFC  80 01 00 60 */	lwz r0, 0x60(r1)
/* 800E3CC0 000E0C00  2C 00 00 02 */	cmpwi r0, 2
/* 800E3CC4 000E0C04  41 82 00 08 */	beq lbl_800E3CCC
/* 800E3CC8 000E0C08  48 00 00 0C */	b lbl_800E3CD4
lbl_800E3CCC:
/* 800E3CCC 000E0C0C  80 01 00 64 */	lwz r0, 0x64(r1)
/* 800E3CD0 000E0C10  48 00 00 08 */	b lbl_800E3CD8
lbl_800E3CD4:
/* 800E3CD4 000E0C14  38 02 8F F0 */	addi r0, r2, $$24053@sda21
lbl_800E3CD8:
/* 800E3CD8 000E0C18  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 800E3CDC 000E0C1C  7C 1B 03 78 */	mr r27, r0
/* 800E3CE0 000E0C20  38 7B 00 00 */	addi r3, r27, 0
/* 800E3CE4 000E0C24  83 84 00 04 */	lwz r28, 4(r4)
/* 800E3CE8 000E0C28  4B F6 09 A5 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800E3CEC 000E0C2C  81 9C 00 00 */	lwz r12, 0(r28)
/* 800E3CF0 000E0C30  38 83 00 00 */	addi r4, r3, 0
/* 800E3CF4 000E0C34  38 7C 00 00 */	addi r3, r28, 0
/* 800E3CF8 000E0C38  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 800E3CFC 000E0C3C  7F 65 DB 78 */	mr r5, r27
/* 800E3D00 000E0C40  7D 88 03 A6 */	mtlr r12
/* 800E3D04 000E0C44  4E 80 00 21 */	blrl 
/* 800E3D08 000E0C48  28 03 00 00 */	cmplwi r3, 0
/* 800E3D0C 000E0C4C  41 82 00 40 */	beq lbl_800E3D4C
/* 800E3D10 000E0C50  2C 1E 00 00 */	cmpwi r30, 0
/* 800E3D14 000E0C54  41 82 00 20 */	beq lbl_800E3D34
/* 800E3D18 000E0C58  80 03 00 F0 */	lwz r0, 0xf0(r3)
/* 800E3D1C 000E0C5C  60 00 00 01 */	ori r0, r0, 1
/* 800E3D20 000E0C60  90 03 00 F0 */	stw r0, 0xf0(r3)
/* 800E3D24 000E0C64  80 03 00 64 */	lwz r0, 0x64(r3)
/* 800E3D28 000E0C68  60 00 00 01 */	ori r0, r0, 1
/* 800E3D2C 000E0C6C  90 03 00 64 */	stw r0, 0x64(r3)
/* 800E3D30 000E0C70  48 00 00 1C */	b lbl_800E3D4C
lbl_800E3D34:
/* 800E3D34 000E0C74  80 03 00 F0 */	lwz r0, 0xf0(r3)
/* 800E3D38 000E0C78  54 00 00 3C */	rlwinm r0, r0, 0, 0, 0x1e
/* 800E3D3C 000E0C7C  90 03 00 F0 */	stw r0, 0xf0(r3)
/* 800E3D40 000E0C80  80 03 00 64 */	lwz r0, 0x64(r3)
/* 800E3D44 000E0C84  54 00 00 3C */	rlwinm r0, r0, 0, 0, 0x1e
/* 800E3D48 000E0C88  90 03 00 64 */	stw r0, 0x64(r3)
lbl_800E3D4C:
/* 800E3D4C 000E0C8C  38 00 00 00 */	li r0, 0
/* 800E3D50 000E0C90  90 01 00 50 */	stw r0, 0x50(r1)
/* 800E3D54 000E0C94  90 01 00 54 */	stw r0, 0x54(r1)
/* 800E3D58 000E0C98  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E3D5C 000E0C9C  80 1D 00 18 */	lwz r0, 0x18(r29)
/* 800E3D60 000E0CA0  7C 03 00 00 */	cmpw r3, r0
/* 800E3D64 000E0CA4  41 80 00 14 */	blt lbl_800E3D78
/* 800E3D68 000E0CA8  38 7F 07 8C */	addi r3, r31, 0x78c
/* 800E3D6C 000E0CAC  4C C6 31 82 */	crclr 6
/* 800E3D70 000E0CB0  4B FF 93 35 */	bl SpcTrace__FPCce
/* 800E3D74 000E0CB4  48 00 00 2C */	b lbl_800E3DA0
lbl_800E3D78:
/* 800E3D78 000E0CB8  80 BD 00 20 */	lwz r5, 0x20(r29)
/* 800E3D7C 000E0CBC  54 64 18 38 */	slwi r4, r3, 3
/* 800E3D80 000E0CC0  80 61 00 50 */	lwz r3, 0x50(r1)
/* 800E3D84 000E0CC4  80 01 00 54 */	lwz r0, 0x54(r1)
/* 800E3D88 000E0CC8  7C 85 22 14 */	add r4, r5, r4
/* 800E3D8C 000E0CCC  90 64 00 00 */	stw r3, 0(r4)
/* 800E3D90 000E0CD0  90 04 00 04 */	stw r0, 4(r4)
/* 800E3D94 000E0CD4  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E3D98 000E0CD8  38 03 00 01 */	addi r0, r3, 1
/* 800E3D9C 000E0CDC  90 1D 00 1C */	stw r0, 0x1c(r29)
lbl_800E3DA0:
/* 800E3DA0 000E0CE0  BB 61 00 84 */	lmw r27, 0x84(r1)
/* 800E3DA4 000E0CE4  80 01 00 9C */	lwz r0, 0x9c(r1)
/* 800E3DA8 000E0CE8  38 21 00 98 */	addi r1, r1, 0x98
/* 800E3DAC 000E0CEC  7C 08 03 A6 */	mtlr r0
/* 800E3DB0 000E0CF0  4E 80 00 20 */	blr 

.global evSetHide4LiveActor__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evSetHide4LiveActor__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E3DB4 000E0CF4  7C 08 02 A6 */	mflr r0
/* 800E3DB8 000E0CF8  90 01 00 04 */	stw r0, 4(r1)
/* 800E3DBC 000E0CFC  94 21 FF 68 */	stwu r1, -0x98(r1)
/* 800E3DC0 000E0D00  BF 61 00 84 */	stmw r27, 0x84(r1)
/* 800E3DC4 000E0D04  7C 7D 1B 78 */	mr r29, r3
/* 800E3DC8 000E0D08  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E3DCC 000E0D0C  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E3DD0 000E0D10  3C 80 80 37 */	lis r4, $$21490@ha
/* 800E3DD4 000E0D14  3B E4 29 90 */	addi r31, r4, $$21490@l
/* 800E3DD8 000E0D18  38 80 00 02 */	li r4, 2
/* 800E3DDC 000E0D1C  48 03 36 55 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E3DE0 000E0D20  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E3DE4 000E0D24  2C 03 00 00 */	cmpwi r3, 0
/* 800E3DE8 000E0D28  41 81 00 30 */	bgt lbl_800E3E18
/* 800E3DEC 000E0D2C  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800E3DF0 000E0D30  4C C6 31 82 */	crclr 6
/* 800E3DF4 000E0D34  4B FF 92 B1 */	bl SpcTrace__FPCce
/* 800E3DF8 000E0D38  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E3DFC 000E0D3C  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E3E00 000E0D40  54 00 18 38 */	slwi r0, r0, 3
/* 800E3E04 000E0D44  7C 63 02 14 */	add r3, r3, r0
/* 800E3E08 000E0D48  80 83 00 00 */	lwz r4, 0(r3)
/* 800E3E0C 000E0D4C  80 03 00 04 */	lwz r0, 4(r3)
/* 800E3E10 000E0D50  90 01 00 6C */	stw r0, 0x6c(r1)
/* 800E3E14 000E0D54  48 00 00 28 */	b lbl_800E3E3C
lbl_800E3E18:
/* 800E3E18 000E0D58  38 03 FF FF */	addi r0, r3, -1
/* 800E3E1C 000E0D5C  90 1D 00 1C */	stw r0, 0x1c(r29)
/* 800E3E20 000E0D60  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E3E24 000E0D64  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E3E28 000E0D68  54 00 18 38 */	slwi r0, r0, 3
/* 800E3E2C 000E0D6C  7C 63 02 14 */	add r3, r3, r0
/* 800E3E30 000E0D70  80 83 00 00 */	lwz r4, 0(r3)
/* 800E3E34 000E0D74  80 03 00 04 */	lwz r0, 4(r3)
/* 800E3E38 000E0D78  90 01 00 6C */	stw r0, 0x6c(r1)
lbl_800E3E3C:
/* 800E3E3C 000E0D7C  90 81 00 70 */	stw r4, 0x70(r1)
/* 800E3E40 000E0D80  80 01 00 6C */	lwz r0, 0x6c(r1)
/* 800E3E44 000E0D84  90 01 00 74 */	stw r0, 0x74(r1)
/* 800E3E48 000E0D88  80 01 00 70 */	lwz r0, 0x70(r1)
/* 800E3E4C 000E0D8C  2C 00 00 01 */	cmpwi r0, 1
/* 800E3E50 000E0D90  41 82 00 1C */	beq lbl_800E3E6C
/* 800E3E54 000E0D94  40 80 00 2C */	bge lbl_800E3E80
/* 800E3E58 000E0D98  2C 00 00 00 */	cmpwi r0, 0
/* 800E3E5C 000E0D9C  40 80 00 08 */	bge lbl_800E3E64
/* 800E3E60 000E0DA0  48 00 00 20 */	b lbl_800E3E80
lbl_800E3E64:
/* 800E3E64 000E0DA4  83 C1 00 74 */	lwz r30, 0x74(r1)
/* 800E3E68 000E0DA8  48 00 00 1C */	b lbl_800E3E84
lbl_800E3E6C:
/* 800E3E6C 000E0DAC  C0 01 00 74 */	lfs f0, 0x74(r1)
/* 800E3E70 000E0DB0  FC 00 00 1E */	fctiwz f0, f0
/* 800E3E74 000E0DB4  D8 01 00 78 */	stfd f0, 0x78(r1)
/* 800E3E78 000E0DB8  83 C1 00 7C */	lwz r30, 0x7c(r1)
/* 800E3E7C 000E0DBC  48 00 00 08 */	b lbl_800E3E84
lbl_800E3E80:
/* 800E3E80 000E0DC0  3B C0 00 00 */	li r30, 0
lbl_800E3E84:
/* 800E3E84 000E0DC4  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E3E88 000E0DC8  2C 03 00 00 */	cmpwi r3, 0
/* 800E3E8C 000E0DCC  41 81 00 34 */	bgt lbl_800E3EC0
/* 800E3E90 000E0DD0  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800E3E94 000E0DD4  4C C6 31 82 */	crclr 6
/* 800E3E98 000E0DD8  4B FF 92 0D */	bl SpcTrace__FPCce
/* 800E3E9C 000E0DDC  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E3EA0 000E0DE0  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E3EA4 000E0DE4  54 00 18 38 */	slwi r0, r0, 3
/* 800E3EA8 000E0DE8  7C 63 02 14 */	add r3, r3, r0
/* 800E3EAC 000E0DEC  80 03 00 00 */	lwz r0, 0(r3)
/* 800E3EB0 000E0DF0  90 01 00 60 */	stw r0, 0x60(r1)
/* 800E3EB4 000E0DF4  80 03 00 04 */	lwz r0, 4(r3)
/* 800E3EB8 000E0DF8  90 01 00 64 */	stw r0, 0x64(r1)
/* 800E3EBC 000E0DFC  48 00 00 2C */	b lbl_800E3EE8
lbl_800E3EC0:
/* 800E3EC0 000E0E00  38 03 FF FF */	addi r0, r3, -1
/* 800E3EC4 000E0E04  90 1D 00 1C */	stw r0, 0x1c(r29)
/* 800E3EC8 000E0E08  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E3ECC 000E0E0C  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E3ED0 000E0E10  54 00 18 38 */	slwi r0, r0, 3
/* 800E3ED4 000E0E14  7C 63 02 14 */	add r3, r3, r0
/* 800E3ED8 000E0E18  80 03 00 00 */	lwz r0, 0(r3)
/* 800E3EDC 000E0E1C  90 01 00 60 */	stw r0, 0x60(r1)
/* 800E3EE0 000E0E20  80 03 00 04 */	lwz r0, 4(r3)
/* 800E3EE4 000E0E24  90 01 00 64 */	stw r0, 0x64(r1)
lbl_800E3EE8:
/* 800E3EE8 000E0E28  80 01 00 60 */	lwz r0, 0x60(r1)
/* 800E3EEC 000E0E2C  2C 00 00 02 */	cmpwi r0, 2
/* 800E3EF0 000E0E30  41 82 00 08 */	beq lbl_800E3EF8
/* 800E3EF4 000E0E34  48 00 00 0C */	b lbl_800E3F00
lbl_800E3EF8:
/* 800E3EF8 000E0E38  80 01 00 64 */	lwz r0, 0x64(r1)
/* 800E3EFC 000E0E3C  48 00 00 08 */	b lbl_800E3F04
lbl_800E3F00:
/* 800E3F00 000E0E40  38 02 8F F0 */	addi r0, r2, $$24053@sda21
lbl_800E3F04:
/* 800E3F04 000E0E44  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 800E3F08 000E0E48  7C 1B 03 78 */	mr r27, r0
/* 800E3F0C 000E0E4C  38 7B 00 00 */	addi r3, r27, 0
/* 800E3F10 000E0E50  83 84 00 04 */	lwz r28, 4(r4)
/* 800E3F14 000E0E54  4B F6 07 79 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800E3F18 000E0E58  81 9C 00 00 */	lwz r12, 0(r28)
/* 800E3F1C 000E0E5C  38 83 00 00 */	addi r4, r3, 0
/* 800E3F20 000E0E60  38 7C 00 00 */	addi r3, r28, 0
/* 800E3F24 000E0E64  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 800E3F28 000E0E68  7F 65 DB 78 */	mr r5, r27
/* 800E3F2C 000E0E6C  7D 88 03 A6 */	mtlr r12
/* 800E3F30 000E0E70  4E 80 00 21 */	blrl 
/* 800E3F34 000E0E74  28 03 00 00 */	cmplwi r3, 0
/* 800E3F38 000E0E78  41 82 00 40 */	beq lbl_800E3F78
/* 800E3F3C 000E0E7C  2C 1E 00 00 */	cmpwi r30, 0
/* 800E3F40 000E0E80  41 82 00 20 */	beq lbl_800E3F60
/* 800E3F44 000E0E84  80 03 00 F0 */	lwz r0, 0xf0(r3)
/* 800E3F48 000E0E88  60 00 00 02 */	ori r0, r0, 2
/* 800E3F4C 000E0E8C  90 03 00 F0 */	stw r0, 0xf0(r3)
/* 800E3F50 000E0E90  80 03 00 64 */	lwz r0, 0x64(r3)
/* 800E3F54 000E0E94  60 00 00 01 */	ori r0, r0, 1
/* 800E3F58 000E0E98  90 03 00 64 */	stw r0, 0x64(r3)
/* 800E3F5C 000E0E9C  48 00 00 1C */	b lbl_800E3F78
lbl_800E3F60:
/* 800E3F60 000E0EA0  80 03 00 F0 */	lwz r0, 0xf0(r3)
/* 800E3F64 000E0EA4  54 00 07 FA */	rlwinm r0, r0, 0, 0x1f, 0x1d
/* 800E3F68 000E0EA8  90 03 00 F0 */	stw r0, 0xf0(r3)
/* 800E3F6C 000E0EAC  80 03 00 64 */	lwz r0, 0x64(r3)
/* 800E3F70 000E0EB0  54 00 00 3C */	rlwinm r0, r0, 0, 0, 0x1e
/* 800E3F74 000E0EB4  90 03 00 64 */	stw r0, 0x64(r3)
lbl_800E3F78:
/* 800E3F78 000E0EB8  38 00 00 00 */	li r0, 0
/* 800E3F7C 000E0EBC  90 01 00 50 */	stw r0, 0x50(r1)
/* 800E3F80 000E0EC0  90 01 00 54 */	stw r0, 0x54(r1)
/* 800E3F84 000E0EC4  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E3F88 000E0EC8  80 1D 00 18 */	lwz r0, 0x18(r29)
/* 800E3F8C 000E0ECC  7C 03 00 00 */	cmpw r3, r0
/* 800E3F90 000E0ED0  41 80 00 14 */	blt lbl_800E3FA4
/* 800E3F94 000E0ED4  38 7F 07 8C */	addi r3, r31, 0x78c
/* 800E3F98 000E0ED8  4C C6 31 82 */	crclr 6
/* 800E3F9C 000E0EDC  4B FF 91 09 */	bl SpcTrace__FPCce
/* 800E3FA0 000E0EE0  48 00 00 2C */	b lbl_800E3FCC
lbl_800E3FA4:
/* 800E3FA4 000E0EE4  80 BD 00 20 */	lwz r5, 0x20(r29)
/* 800E3FA8 000E0EE8  54 64 18 38 */	slwi r4, r3, 3
/* 800E3FAC 000E0EEC  80 61 00 50 */	lwz r3, 0x50(r1)
/* 800E3FB0 000E0EF0  80 01 00 54 */	lwz r0, 0x54(r1)
/* 800E3FB4 000E0EF4  7C 85 22 14 */	add r4, r5, r4
/* 800E3FB8 000E0EF8  90 64 00 00 */	stw r3, 0(r4)
/* 800E3FBC 000E0EFC  90 04 00 04 */	stw r0, 4(r4)
/* 800E3FC0 000E0F00  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E3FC4 000E0F04  38 03 00 01 */	addi r0, r3, 1
/* 800E3FC8 000E0F08  90 1D 00 1C */	stw r0, 0x1c(r29)
lbl_800E3FCC:
/* 800E3FCC 000E0F0C  BB 61 00 84 */	lmw r27, 0x84(r1)
/* 800E3FD0 000E0F10  80 01 00 9C */	lwz r0, 0x9c(r1)
/* 800E3FD4 000E0F14  38 21 00 98 */	addi r1, r1, 0x98
/* 800E3FD8 000E0F18  7C 08 03 A6 */	mtlr r0
/* 800E3FDC 000E0F1C  4E 80 00 20 */	blr 

.global evIsOnLiveActorFlag__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evIsOnLiveActorFlag__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E3FE0 000E0F20  7C 08 02 A6 */	mflr r0
/* 800E3FE4 000E0F24  90 01 00 04 */	stw r0, 4(r1)
/* 800E3FE8 000E0F28  94 21 FF 50 */	stwu r1, -0xb0(r1)
/* 800E3FEC 000E0F2C  BF 61 00 9C */	stmw r27, 0x9c(r1)
/* 800E3FF0 000E0F30  7C 7D 1B 78 */	mr r29, r3
/* 800E3FF4 000E0F34  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E3FF8 000E0F38  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E3FFC 000E0F3C  3C 80 80 37 */	lis r4, $$21490@ha
/* 800E4000 000E0F40  3B E4 29 90 */	addi r31, r4, $$21490@l
/* 800E4004 000E0F44  38 80 00 02 */	li r4, 2
/* 800E4008 000E0F48  48 03 34 29 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E400C 000E0F4C  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E4010 000E0F50  2C 03 00 00 */	cmpwi r3, 0
/* 800E4014 000E0F54  41 81 00 30 */	bgt lbl_800E4044
/* 800E4018 000E0F58  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800E401C 000E0F5C  4C C6 31 82 */	crclr 6
/* 800E4020 000E0F60  4B FF 90 85 */	bl SpcTrace__FPCce
/* 800E4024 000E0F64  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E4028 000E0F68  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E402C 000E0F6C  54 00 18 38 */	slwi r0, r0, 3
/* 800E4030 000E0F70  7C 63 02 14 */	add r3, r3, r0
/* 800E4034 000E0F74  80 83 00 00 */	lwz r4, 0(r3)
/* 800E4038 000E0F78  80 03 00 04 */	lwz r0, 4(r3)
/* 800E403C 000E0F7C  90 01 00 84 */	stw r0, 0x84(r1)
/* 800E4040 000E0F80  48 00 00 28 */	b lbl_800E4068
lbl_800E4044:
/* 800E4044 000E0F84  38 03 FF FF */	addi r0, r3, -1
/* 800E4048 000E0F88  90 1D 00 1C */	stw r0, 0x1c(r29)
/* 800E404C 000E0F8C  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E4050 000E0F90  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E4054 000E0F94  54 00 18 38 */	slwi r0, r0, 3
/* 800E4058 000E0F98  7C 63 02 14 */	add r3, r3, r0
/* 800E405C 000E0F9C  80 83 00 00 */	lwz r4, 0(r3)
/* 800E4060 000E0FA0  80 03 00 04 */	lwz r0, 4(r3)
/* 800E4064 000E0FA4  90 01 00 84 */	stw r0, 0x84(r1)
lbl_800E4068:
/* 800E4068 000E0FA8  90 81 00 88 */	stw r4, 0x88(r1)
/* 800E406C 000E0FAC  80 01 00 84 */	lwz r0, 0x84(r1)
/* 800E4070 000E0FB0  90 01 00 8C */	stw r0, 0x8c(r1)
/* 800E4074 000E0FB4  80 01 00 88 */	lwz r0, 0x88(r1)
/* 800E4078 000E0FB8  2C 00 00 01 */	cmpwi r0, 1
/* 800E407C 000E0FBC  41 82 00 1C */	beq lbl_800E4098
/* 800E4080 000E0FC0  40 80 00 2C */	bge lbl_800E40AC
/* 800E4084 000E0FC4  2C 00 00 00 */	cmpwi r0, 0
/* 800E4088 000E0FC8  40 80 00 08 */	bge lbl_800E4090
/* 800E408C 000E0FCC  48 00 00 20 */	b lbl_800E40AC
lbl_800E4090:
/* 800E4090 000E0FD0  83 C1 00 8C */	lwz r30, 0x8c(r1)
/* 800E4094 000E0FD4  48 00 00 1C */	b lbl_800E40B0
lbl_800E4098:
/* 800E4098 000E0FD8  C0 01 00 8C */	lfs f0, 0x8c(r1)
/* 800E409C 000E0FDC  FC 00 00 1E */	fctiwz f0, f0
/* 800E40A0 000E0FE0  D8 01 00 90 */	stfd f0, 0x90(r1)
/* 800E40A4 000E0FE4  83 C1 00 94 */	lwz r30, 0x94(r1)
/* 800E40A8 000E0FE8  48 00 00 08 */	b lbl_800E40B0
lbl_800E40AC:
/* 800E40AC 000E0FEC  3B C0 00 00 */	li r30, 0
lbl_800E40B0:
/* 800E40B0 000E0FF0  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E40B4 000E0FF4  2C 03 00 00 */	cmpwi r3, 0
/* 800E40B8 000E0FF8  41 81 00 34 */	bgt lbl_800E40EC
/* 800E40BC 000E0FFC  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800E40C0 000E1000  4C C6 31 82 */	crclr 6
/* 800E40C4 000E1004  4B FF 8F E1 */	bl SpcTrace__FPCce
/* 800E40C8 000E1008  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E40CC 000E100C  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E40D0 000E1010  54 00 18 38 */	slwi r0, r0, 3
/* 800E40D4 000E1014  7C 63 02 14 */	add r3, r3, r0
/* 800E40D8 000E1018  80 03 00 00 */	lwz r0, 0(r3)
/* 800E40DC 000E101C  90 01 00 70 */	stw r0, 0x70(r1)
/* 800E40E0 000E1020  80 03 00 04 */	lwz r0, 4(r3)
/* 800E40E4 000E1024  90 01 00 74 */	stw r0, 0x74(r1)
/* 800E40E8 000E1028  48 00 00 2C */	b lbl_800E4114
lbl_800E40EC:
/* 800E40EC 000E102C  38 03 FF FF */	addi r0, r3, -1
/* 800E40F0 000E1030  90 1D 00 1C */	stw r0, 0x1c(r29)
/* 800E40F4 000E1034  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E40F8 000E1038  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E40FC 000E103C  54 00 18 38 */	slwi r0, r0, 3
/* 800E4100 000E1040  7C 63 02 14 */	add r3, r3, r0
/* 800E4104 000E1044  80 03 00 00 */	lwz r0, 0(r3)
/* 800E4108 000E1048  90 01 00 70 */	stw r0, 0x70(r1)
/* 800E410C 000E104C  80 03 00 04 */	lwz r0, 4(r3)
/* 800E4110 000E1050  90 01 00 74 */	stw r0, 0x74(r1)
lbl_800E4114:
/* 800E4114 000E1054  80 81 00 70 */	lwz r4, 0x70(r1)
/* 800E4118 000E1058  38 60 00 00 */	li r3, 0
/* 800E411C 000E105C  80 01 00 74 */	lwz r0, 0x74(r1)
/* 800E4120 000E1060  90 81 00 78 */	stw r4, 0x78(r1)
/* 800E4124 000E1064  90 01 00 7C */	stw r0, 0x7c(r1)
/* 800E4128 000E1068  80 01 00 78 */	lwz r0, 0x78(r1)
/* 800E412C 000E106C  2C 00 00 01 */	cmpwi r0, 1
/* 800E4130 000E1070  41 82 00 9C */	beq lbl_800E41CC
/* 800E4134 000E1074  40 80 00 10 */	bge lbl_800E4144
/* 800E4138 000E1078  2C 00 00 00 */	cmpwi r0, 0
/* 800E413C 000E107C  40 80 00 58 */	bge lbl_800E4194
/* 800E4140 000E1080  48 00 00 8C */	b lbl_800E41CC
lbl_800E4144:
/* 800E4144 000E1084  2C 00 00 03 */	cmpwi r0, 3
/* 800E4148 000E1088  40 80 00 84 */	bge lbl_800E41CC
/* 800E414C 000E108C  2C 00 00 02 */	cmpwi r0, 2
/* 800E4150 000E1090  41 82 00 08 */	beq lbl_800E4158
/* 800E4154 000E1094  48 00 00 0C */	b lbl_800E4160
lbl_800E4158:
/* 800E4158 000E1098  83 81 00 7C */	lwz r28, 0x7c(r1)
/* 800E415C 000E109C  48 00 00 08 */	b lbl_800E4164
lbl_800E4160:
/* 800E4160 000E10A0  3B 82 8F F0 */	addi r28, r2, $$24053@sda21
lbl_800E4164:
/* 800E4164 000E10A4  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 800E4168 000E10A8  7F 83 E3 78 */	mr r3, r28
/* 800E416C 000E10AC  83 64 00 04 */	lwz r27, 4(r4)
/* 800E4170 000E10B0  4B F6 05 1D */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800E4174 000E10B4  81 9B 00 00 */	lwz r12, 0(r27)
/* 800E4178 000E10B8  38 83 00 00 */	addi r4, r3, 0
/* 800E417C 000E10BC  38 7B 00 00 */	addi r3, r27, 0
/* 800E4180 000E10C0  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 800E4184 000E10C4  7F 85 E3 78 */	mr r5, r28
/* 800E4188 000E10C8  7D 88 03 A6 */	mtlr r12
/* 800E418C 000E10CC  4E 80 00 21 */	blrl 
/* 800E4190 000E10D0  48 00 00 3C */	b lbl_800E41CC
lbl_800E4194:
/* 800E4194 000E10D4  2C 00 00 01 */	cmpwi r0, 1
/* 800E4198 000E10D8  41 82 00 1C */	beq lbl_800E41B4
/* 800E419C 000E10DC  40 80 00 2C */	bge lbl_800E41C8
/* 800E41A0 000E10E0  2C 00 00 00 */	cmpwi r0, 0
/* 800E41A4 000E10E4  40 80 00 08 */	bge lbl_800E41AC
/* 800E41A8 000E10E8  48 00 00 20 */	b lbl_800E41C8
lbl_800E41AC:
/* 800E41AC 000E10EC  80 61 00 7C */	lwz r3, 0x7c(r1)
/* 800E41B0 000E10F0  48 00 00 1C */	b lbl_800E41CC
lbl_800E41B4:
/* 800E41B4 000E10F4  C0 01 00 7C */	lfs f0, 0x7c(r1)
/* 800E41B8 000E10F8  FC 00 00 1E */	fctiwz f0, f0
/* 800E41BC 000E10FC  D8 01 00 90 */	stfd f0, 0x90(r1)
/* 800E41C0 000E1100  80 61 00 94 */	lwz r3, 0x94(r1)
/* 800E41C4 000E1104  48 00 00 08 */	b lbl_800E41CC
lbl_800E41C8:
/* 800E41C8 000E1108  38 60 00 00 */	li r3, 0
lbl_800E41CC:
/* 800E41CC 000E110C  28 03 00 00 */	cmplwi r3, 0
/* 800E41D0 000E1110  38 80 00 00 */	li r4, 0
/* 800E41D4 000E1114  41 82 00 0C */	beq lbl_800E41E0
/* 800E41D8 000E1118  80 03 00 F0 */	lwz r0, 0xf0(r3)
/* 800E41DC 000E111C  7C 04 F0 38 */	and r4, r0, r30
lbl_800E41E0:
/* 800E41E0 000E1120  38 00 00 00 */	li r0, 0
/* 800E41E4 000E1124  90 01 00 5C */	stw r0, 0x5c(r1)
/* 800E41E8 000E1128  90 81 00 60 */	stw r4, 0x60(r1)
/* 800E41EC 000E112C  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E41F0 000E1130  80 1D 00 18 */	lwz r0, 0x18(r29)
/* 800E41F4 000E1134  7C 03 00 00 */	cmpw r3, r0
/* 800E41F8 000E1138  41 80 00 14 */	blt lbl_800E420C
/* 800E41FC 000E113C  38 7F 07 8C */	addi r3, r31, 0x78c
/* 800E4200 000E1140  4C C6 31 82 */	crclr 6
/* 800E4204 000E1144  4B FF 8E A1 */	bl SpcTrace__FPCce
/* 800E4208 000E1148  48 00 00 2C */	b lbl_800E4234
lbl_800E420C:
/* 800E420C 000E114C  80 BD 00 20 */	lwz r5, 0x20(r29)
/* 800E4210 000E1150  54 64 18 38 */	slwi r4, r3, 3
/* 800E4214 000E1154  80 61 00 5C */	lwz r3, 0x5c(r1)
/* 800E4218 000E1158  80 01 00 60 */	lwz r0, 0x60(r1)
/* 800E421C 000E115C  7C 85 22 14 */	add r4, r5, r4
/* 800E4220 000E1160  90 64 00 00 */	stw r3, 0(r4)
/* 800E4224 000E1164  90 04 00 04 */	stw r0, 4(r4)
/* 800E4228 000E1168  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E422C 000E116C  38 03 00 01 */	addi r0, r3, 1
/* 800E4230 000E1170  90 1D 00 1C */	stw r0, 0x1c(r29)
lbl_800E4234:
/* 800E4234 000E1174  BB 61 00 9C */	lmw r27, 0x9c(r1)
/* 800E4238 000E1178  80 01 00 B4 */	lwz r0, 0xb4(r1)
/* 800E423C 000E117C  38 21 00 B0 */	addi r1, r1, 0xb0
/* 800E4240 000E1180  7C 08 03 A6 */	mtlr r0
/* 800E4244 000E1184  4E 80 00 20 */	blr 

.global evPushNerve4LiveActor__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evPushNerve4LiveActor__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E4248 000E1188  7C 08 02 A6 */	mflr r0
/* 800E424C 000E118C  90 01 00 04 */	stw r0, 4(r1)
/* 800E4250 000E1190  94 21 FF 68 */	stwu r1, -0x98(r1)
/* 800E4254 000E1194  BF 61 00 84 */	stmw r27, 0x84(r1)
/* 800E4258 000E1198  7C 7D 1B 78 */	mr r29, r3
/* 800E425C 000E119C  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E4260 000E11A0  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E4264 000E11A4  3C 80 80 37 */	lis r4, $$21490@ha
/* 800E4268 000E11A8  3B C4 29 90 */	addi r30, r4, $$21490@l
/* 800E426C 000E11AC  38 80 00 02 */	li r4, 2
/* 800E4270 000E11B0  48 03 31 C1 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E4274 000E11B4  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E4278 000E11B8  2C 03 00 00 */	cmpwi r3, 0
/* 800E427C 000E11BC  41 81 00 30 */	bgt lbl_800E42AC
/* 800E4280 000E11C0  38 7E 07 6C */	addi r3, r30, 0x76c
/* 800E4284 000E11C4  4C C6 31 82 */	crclr 6
/* 800E4288 000E11C8  4B FF 8E 1D */	bl SpcTrace__FPCce
/* 800E428C 000E11CC  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E4290 000E11D0  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E4294 000E11D4  54 00 18 38 */	slwi r0, r0, 3
/* 800E4298 000E11D8  7C 63 02 14 */	add r3, r3, r0
/* 800E429C 000E11DC  80 83 00 00 */	lwz r4, 0(r3)
/* 800E42A0 000E11E0  80 03 00 04 */	lwz r0, 4(r3)
/* 800E42A4 000E11E4  90 01 00 6C */	stw r0, 0x6c(r1)
/* 800E42A8 000E11E8  48 00 00 28 */	b lbl_800E42D0
lbl_800E42AC:
/* 800E42AC 000E11EC  38 03 FF FF */	addi r0, r3, -1
/* 800E42B0 000E11F0  90 1D 00 1C */	stw r0, 0x1c(r29)
/* 800E42B4 000E11F4  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E42B8 000E11F8  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E42BC 000E11FC  54 00 18 38 */	slwi r0, r0, 3
/* 800E42C0 000E1200  7C 63 02 14 */	add r3, r3, r0
/* 800E42C4 000E1204  80 83 00 00 */	lwz r4, 0(r3)
/* 800E42C8 000E1208  80 03 00 04 */	lwz r0, 4(r3)
/* 800E42CC 000E120C  90 01 00 6C */	stw r0, 0x6c(r1)
lbl_800E42D0:
/* 800E42D0 000E1210  90 81 00 70 */	stw r4, 0x70(r1)
/* 800E42D4 000E1214  80 01 00 6C */	lwz r0, 0x6c(r1)
/* 800E42D8 000E1218  90 01 00 74 */	stw r0, 0x74(r1)
/* 800E42DC 000E121C  80 01 00 70 */	lwz r0, 0x70(r1)
/* 800E42E0 000E1220  2C 00 00 01 */	cmpwi r0, 1
/* 800E42E4 000E1224  41 82 00 1C */	beq lbl_800E4300
/* 800E42E8 000E1228  40 80 00 2C */	bge lbl_800E4314
/* 800E42EC 000E122C  2C 00 00 00 */	cmpwi r0, 0
/* 800E42F0 000E1230  40 80 00 08 */	bge lbl_800E42F8
/* 800E42F4 000E1234  48 00 00 20 */	b lbl_800E4314
lbl_800E42F8:
/* 800E42F8 000E1238  80 01 00 74 */	lwz r0, 0x74(r1)
/* 800E42FC 000E123C  48 00 00 1C */	b lbl_800E4318
lbl_800E4300:
/* 800E4300 000E1240  C0 01 00 74 */	lfs f0, 0x74(r1)
/* 800E4304 000E1244  FC 00 00 1E */	fctiwz f0, f0
/* 800E4308 000E1248  D8 01 00 78 */	stfd f0, 0x78(r1)
/* 800E430C 000E124C  80 01 00 7C */	lwz r0, 0x7c(r1)
/* 800E4310 000E1250  48 00 00 08 */	b lbl_800E4318
lbl_800E4314:
/* 800E4314 000E1254  38 00 00 00 */	li r0, 0
lbl_800E4318:
/* 800E4318 000E1258  7C 03 03 78 */	mr r3, r0
/* 800E431C 000E125C  48 02 FF 6D */	bl NerveGetByIndex__Fi
/* 800E4320 000E1260  80 9D 00 1C */	lwz r4, 0x1c(r29)
/* 800E4324 000E1264  3B E3 00 00 */	addi r31, r3, 0
/* 800E4328 000E1268  2C 04 00 00 */	cmpwi r4, 0
/* 800E432C 000E126C  41 81 00 34 */	bgt lbl_800E4360
/* 800E4330 000E1270  38 7E 07 6C */	addi r3, r30, 0x76c
/* 800E4334 000E1274  4C C6 31 82 */	crclr 6
/* 800E4338 000E1278  4B FF 8D 6D */	bl SpcTrace__FPCce
/* 800E433C 000E127C  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E4340 000E1280  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E4344 000E1284  54 00 18 38 */	slwi r0, r0, 3
/* 800E4348 000E1288  7C 63 02 14 */	add r3, r3, r0
/* 800E434C 000E128C  80 03 00 00 */	lwz r0, 0(r3)
/* 800E4350 000E1290  90 01 00 60 */	stw r0, 0x60(r1)
/* 800E4354 000E1294  80 03 00 04 */	lwz r0, 4(r3)
/* 800E4358 000E1298  90 01 00 64 */	stw r0, 0x64(r1)
/* 800E435C 000E129C  48 00 00 2C */	b lbl_800E4388
lbl_800E4360:
/* 800E4360 000E12A0  38 04 FF FF */	addi r0, r4, -1
/* 800E4364 000E12A4  90 1D 00 1C */	stw r0, 0x1c(r29)
/* 800E4368 000E12A8  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E436C 000E12AC  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E4370 000E12B0  54 00 18 38 */	slwi r0, r0, 3
/* 800E4374 000E12B4  7C 63 02 14 */	add r3, r3, r0
/* 800E4378 000E12B8  80 03 00 00 */	lwz r0, 0(r3)
/* 800E437C 000E12BC  90 01 00 60 */	stw r0, 0x60(r1)
/* 800E4380 000E12C0  80 03 00 04 */	lwz r0, 4(r3)
/* 800E4384 000E12C4  90 01 00 64 */	stw r0, 0x64(r1)
lbl_800E4388:
/* 800E4388 000E12C8  80 01 00 60 */	lwz r0, 0x60(r1)
/* 800E438C 000E12CC  2C 00 00 02 */	cmpwi r0, 2
/* 800E4390 000E12D0  41 82 00 08 */	beq lbl_800E4398
/* 800E4394 000E12D4  48 00 00 0C */	b lbl_800E43A0
lbl_800E4398:
/* 800E4398 000E12D8  80 01 00 64 */	lwz r0, 0x64(r1)
/* 800E439C 000E12DC  48 00 00 08 */	b lbl_800E43A4
lbl_800E43A0:
/* 800E43A0 000E12E0  38 02 8F F0 */	addi r0, r2, $$24053@sda21
lbl_800E43A4:
/* 800E43A4 000E12E4  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 800E43A8 000E12E8  7C 1B 03 78 */	mr r27, r0
/* 800E43AC 000E12EC  38 7B 00 00 */	addi r3, r27, 0
/* 800E43B0 000E12F0  83 84 00 04 */	lwz r28, 4(r4)
/* 800E43B4 000E12F4  4B F6 02 D9 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800E43B8 000E12F8  81 9C 00 00 */	lwz r12, 0(r28)
/* 800E43BC 000E12FC  38 83 00 00 */	addi r4, r3, 0
/* 800E43C0 000E1300  38 7C 00 00 */	addi r3, r28, 0
/* 800E43C4 000E1304  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 800E43C8 000E1308  7F 65 DB 78 */	mr r5, r27
/* 800E43CC 000E130C  7D 88 03 A6 */	mtlr r12
/* 800E43D0 000E1310  4E 80 00 21 */	blrl 
/* 800E43D4 000E1314  28 03 00 00 */	cmplwi r3, 0
/* 800E43D8 000E1318  41 82 00 5C */	beq lbl_800E4434
/* 800E43DC 000E131C  28 1F 00 00 */	cmplwi r31, 0
/* 800E43E0 000E1320  41 82 00 54 */	beq lbl_800E4434
/* 800E43E4 000E1324  80 A3 00 8C */	lwz r5, 0x8c(r3)
/* 800E43E8 000E1328  41 82 00 4C */	beq lbl_800E4434
/* 800E43EC 000E132C  80 05 00 14 */	lwz r0, 0x14(r5)
/* 800E43F0 000E1330  28 00 00 00 */	cmplwi r0, 0
/* 800E43F4 000E1334  41 82 00 34 */	beq lbl_800E4428
/* 800E43F8 000E1338  90 05 00 1C */	stw r0, 0x1c(r5)
/* 800E43FC 000E133C  80 65 00 08 */	lwz r3, 8(r5)
/* 800E4400 000E1340  80 05 00 04 */	lwz r0, 4(r5)
/* 800E4404 000E1344  7C 03 00 00 */	cmpw r3, r0
/* 800E4408 000E1348  40 80 00 20 */	bge lbl_800E4428
/* 800E440C 000E134C  80 85 00 14 */	lwz r4, 0x14(r5)
/* 800E4410 000E1350  54 60 10 3A */	slwi r0, r3, 2
/* 800E4414 000E1354  80 65 00 0C */	lwz r3, 0xc(r5)
/* 800E4418 000E1358  7C 83 01 2E */	stwx r4, r3, r0
/* 800E441C 000E135C  80 65 00 08 */	lwz r3, 8(r5)
/* 800E4420 000E1360  38 03 00 01 */	addi r0, r3, 1
/* 800E4424 000E1364  90 05 00 08 */	stw r0, 8(r5)
lbl_800E4428:
/* 800E4428 000E1368  38 00 00 00 */	li r0, 0
/* 800E442C 000E136C  90 05 00 20 */	stw r0, 0x20(r5)
/* 800E4430 000E1370  93 E5 00 14 */	stw r31, 0x14(r5)
lbl_800E4434:
/* 800E4434 000E1374  38 00 00 00 */	li r0, 0
/* 800E4438 000E1378  90 01 00 50 */	stw r0, 0x50(r1)
/* 800E443C 000E137C  90 01 00 54 */	stw r0, 0x54(r1)
/* 800E4440 000E1380  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E4444 000E1384  80 1D 00 18 */	lwz r0, 0x18(r29)
/* 800E4448 000E1388  7C 03 00 00 */	cmpw r3, r0
/* 800E444C 000E138C  41 80 00 14 */	blt lbl_800E4460
/* 800E4450 000E1390  38 7E 07 8C */	addi r3, r30, 0x78c
/* 800E4454 000E1394  4C C6 31 82 */	crclr 6
/* 800E4458 000E1398  4B FF 8C 4D */	bl SpcTrace__FPCce
/* 800E445C 000E139C  48 00 00 2C */	b lbl_800E4488
lbl_800E4460:
/* 800E4460 000E13A0  80 BD 00 20 */	lwz r5, 0x20(r29)
/* 800E4464 000E13A4  54 64 18 38 */	slwi r4, r3, 3
/* 800E4468 000E13A8  80 61 00 50 */	lwz r3, 0x50(r1)
/* 800E446C 000E13AC  80 01 00 54 */	lwz r0, 0x54(r1)
/* 800E4470 000E13B0  7C 85 22 14 */	add r4, r5, r4
/* 800E4474 000E13B4  90 64 00 00 */	stw r3, 0(r4)
/* 800E4478 000E13B8  90 04 00 04 */	stw r0, 4(r4)
/* 800E447C 000E13BC  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E4480 000E13C0  38 03 00 01 */	addi r0, r3, 1
/* 800E4484 000E13C4  90 1D 00 1C */	stw r0, 0x1c(r29)
lbl_800E4488:
/* 800E4488 000E13C8  BB 61 00 84 */	lmw r27, 0x84(r1)
/* 800E448C 000E13CC  80 01 00 9C */	lwz r0, 0x9c(r1)
/* 800E4490 000E13D0  38 21 00 98 */	addi r1, r1, 0x98
/* 800E4494 000E13D4  7C 08 03 A6 */	mtlr r0
/* 800E4498 000E13D8  4E 80 00 20 */	blr 

.global evSetFlagNPCCanTaken__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evSetFlagNPCCanTaken__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E449C 000E13DC  7C 08 02 A6 */	mflr r0
/* 800E44A0 000E13E0  90 01 00 04 */	stw r0, 4(r1)
/* 800E44A4 000E13E4  94 21 FF 68 */	stwu r1, -0x98(r1)
/* 800E44A8 000E13E8  BF 61 00 84 */	stmw r27, 0x84(r1)
/* 800E44AC 000E13EC  7C 7D 1B 78 */	mr r29, r3
/* 800E44B0 000E13F0  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E44B4 000E13F4  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E44B8 000E13F8  3C 80 80 37 */	lis r4, $$21490@ha
/* 800E44BC 000E13FC  3B E4 29 90 */	addi r31, r4, $$21490@l
/* 800E44C0 000E1400  38 80 00 02 */	li r4, 2
/* 800E44C4 000E1404  48 03 2F 6D */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E44C8 000E1408  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E44CC 000E140C  2C 03 00 00 */	cmpwi r3, 0
/* 800E44D0 000E1410  41 81 00 30 */	bgt lbl_800E4500
/* 800E44D4 000E1414  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800E44D8 000E1418  4C C6 31 82 */	crclr 6
/* 800E44DC 000E141C  4B FF 8B C9 */	bl SpcTrace__FPCce
/* 800E44E0 000E1420  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E44E4 000E1424  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E44E8 000E1428  54 00 18 38 */	slwi r0, r0, 3
/* 800E44EC 000E142C  7C 63 02 14 */	add r3, r3, r0
/* 800E44F0 000E1430  80 83 00 00 */	lwz r4, 0(r3)
/* 800E44F4 000E1434  80 03 00 04 */	lwz r0, 4(r3)
/* 800E44F8 000E1438  90 01 00 6C */	stw r0, 0x6c(r1)
/* 800E44FC 000E143C  48 00 00 28 */	b lbl_800E4524
lbl_800E4500:
/* 800E4500 000E1440  38 03 FF FF */	addi r0, r3, -1
/* 800E4504 000E1444  90 1D 00 1C */	stw r0, 0x1c(r29)
/* 800E4508 000E1448  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E450C 000E144C  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E4510 000E1450  54 00 18 38 */	slwi r0, r0, 3
/* 800E4514 000E1454  7C 63 02 14 */	add r3, r3, r0
/* 800E4518 000E1458  80 83 00 00 */	lwz r4, 0(r3)
/* 800E451C 000E145C  80 03 00 04 */	lwz r0, 4(r3)
/* 800E4520 000E1460  90 01 00 6C */	stw r0, 0x6c(r1)
lbl_800E4524:
/* 800E4524 000E1464  90 81 00 70 */	stw r4, 0x70(r1)
/* 800E4528 000E1468  80 01 00 6C */	lwz r0, 0x6c(r1)
/* 800E452C 000E146C  90 01 00 74 */	stw r0, 0x74(r1)
/* 800E4530 000E1470  80 01 00 70 */	lwz r0, 0x70(r1)
/* 800E4534 000E1474  2C 00 00 01 */	cmpwi r0, 1
/* 800E4538 000E1478  41 82 00 1C */	beq lbl_800E4554
/* 800E453C 000E147C  40 80 00 2C */	bge lbl_800E4568
/* 800E4540 000E1480  2C 00 00 00 */	cmpwi r0, 0
/* 800E4544 000E1484  40 80 00 08 */	bge lbl_800E454C
/* 800E4548 000E1488  48 00 00 20 */	b lbl_800E4568
lbl_800E454C:
/* 800E454C 000E148C  83 C1 00 74 */	lwz r30, 0x74(r1)
/* 800E4550 000E1490  48 00 00 1C */	b lbl_800E456C
lbl_800E4554:
/* 800E4554 000E1494  C0 01 00 74 */	lfs f0, 0x74(r1)
/* 800E4558 000E1498  FC 00 00 1E */	fctiwz f0, f0
/* 800E455C 000E149C  D8 01 00 78 */	stfd f0, 0x78(r1)
/* 800E4560 000E14A0  83 C1 00 7C */	lwz r30, 0x7c(r1)
/* 800E4564 000E14A4  48 00 00 08 */	b lbl_800E456C
lbl_800E4568:
/* 800E4568 000E14A8  3B C0 00 00 */	li r30, 0
lbl_800E456C:
/* 800E456C 000E14AC  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E4570 000E14B0  2C 03 00 00 */	cmpwi r3, 0
/* 800E4574 000E14B4  41 81 00 34 */	bgt lbl_800E45A8
/* 800E4578 000E14B8  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800E457C 000E14BC  4C C6 31 82 */	crclr 6
/* 800E4580 000E14C0  4B FF 8B 25 */	bl SpcTrace__FPCce
/* 800E4584 000E14C4  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E4588 000E14C8  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E458C 000E14CC  54 00 18 38 */	slwi r0, r0, 3
/* 800E4590 000E14D0  7C 63 02 14 */	add r3, r3, r0
/* 800E4594 000E14D4  80 03 00 00 */	lwz r0, 0(r3)
/* 800E4598 000E14D8  90 01 00 60 */	stw r0, 0x60(r1)
/* 800E459C 000E14DC  80 03 00 04 */	lwz r0, 4(r3)
/* 800E45A0 000E14E0  90 01 00 64 */	stw r0, 0x64(r1)
/* 800E45A4 000E14E4  48 00 00 2C */	b lbl_800E45D0
lbl_800E45A8:
/* 800E45A8 000E14E8  38 03 FF FF */	addi r0, r3, -1
/* 800E45AC 000E14EC  90 1D 00 1C */	stw r0, 0x1c(r29)
/* 800E45B0 000E14F0  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E45B4 000E14F4  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E45B8 000E14F8  54 00 18 38 */	slwi r0, r0, 3
/* 800E45BC 000E14FC  7C 63 02 14 */	add r3, r3, r0
/* 800E45C0 000E1500  80 03 00 00 */	lwz r0, 0(r3)
/* 800E45C4 000E1504  90 01 00 60 */	stw r0, 0x60(r1)
/* 800E45C8 000E1508  80 03 00 04 */	lwz r0, 4(r3)
/* 800E45CC 000E150C  90 01 00 64 */	stw r0, 0x64(r1)
lbl_800E45D0:
/* 800E45D0 000E1510  80 01 00 60 */	lwz r0, 0x60(r1)
/* 800E45D4 000E1514  2C 00 00 02 */	cmpwi r0, 2
/* 800E45D8 000E1518  41 82 00 08 */	beq lbl_800E45E0
/* 800E45DC 000E151C  48 00 00 0C */	b lbl_800E45E8
lbl_800E45E0:
/* 800E45E0 000E1520  80 01 00 64 */	lwz r0, 0x64(r1)
/* 800E45E4 000E1524  48 00 00 08 */	b lbl_800E45EC
lbl_800E45E8:
/* 800E45E8 000E1528  38 02 8F F0 */	addi r0, r2, $$24053@sda21
lbl_800E45EC:
/* 800E45EC 000E152C  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 800E45F0 000E1530  7C 1B 03 78 */	mr r27, r0
/* 800E45F4 000E1534  38 7B 00 00 */	addi r3, r27, 0
/* 800E45F8 000E1538  83 84 00 04 */	lwz r28, 4(r4)
/* 800E45FC 000E153C  4B F6 00 91 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800E4600 000E1540  81 9C 00 00 */	lwz r12, 0(r28)
/* 800E4604 000E1544  38 83 00 00 */	addi r4, r3, 0
/* 800E4608 000E1548  38 7C 00 00 */	addi r3, r28, 0
/* 800E460C 000E154C  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 800E4610 000E1550  7F 65 DB 78 */	mr r5, r27
/* 800E4614 000E1554  7D 88 03 A6 */	mtlr r12
/* 800E4618 000E1558  4E 80 00 21 */	blrl 
/* 800E461C 000E155C  28 03 00 00 */	cmplwi r3, 0
/* 800E4620 000E1560  41 82 00 28 */	beq lbl_800E4648
/* 800E4624 000E1564  2C 1E 00 00 */	cmpwi r30, 0
/* 800E4628 000E1568  41 82 00 14 */	beq lbl_800E463C
/* 800E462C 000E156C  80 03 00 F0 */	lwz r0, 0xf0(r3)
/* 800E4630 000E1570  64 00 00 10 */	oris r0, r0, 0x10
/* 800E4634 000E1574  90 03 00 F0 */	stw r0, 0xf0(r3)
/* 800E4638 000E1578  48 00 00 10 */	b lbl_800E4648
lbl_800E463C:
/* 800E463C 000E157C  80 03 00 F0 */	lwz r0, 0xf0(r3)
/* 800E4640 000E1580  54 00 03 14 */	rlwinm r0, r0, 0, 0xc, 0xa
/* 800E4644 000E1584  90 03 00 F0 */	stw r0, 0xf0(r3)
lbl_800E4648:
/* 800E4648 000E1588  38 00 00 00 */	li r0, 0
/* 800E464C 000E158C  90 01 00 50 */	stw r0, 0x50(r1)
/* 800E4650 000E1590  90 01 00 54 */	stw r0, 0x54(r1)
/* 800E4654 000E1594  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E4658 000E1598  80 1D 00 18 */	lwz r0, 0x18(r29)
/* 800E465C 000E159C  7C 03 00 00 */	cmpw r3, r0
/* 800E4660 000E15A0  41 80 00 14 */	blt lbl_800E4674
/* 800E4664 000E15A4  38 7F 07 8C */	addi r3, r31, 0x78c
/* 800E4668 000E15A8  4C C6 31 82 */	crclr 6
/* 800E466C 000E15AC  4B FF 8A 39 */	bl SpcTrace__FPCce
/* 800E4670 000E15B0  48 00 00 2C */	b lbl_800E469C
lbl_800E4674:
/* 800E4674 000E15B4  80 BD 00 20 */	lwz r5, 0x20(r29)
/* 800E4678 000E15B8  54 64 18 38 */	slwi r4, r3, 3
/* 800E467C 000E15BC  80 61 00 50 */	lwz r3, 0x50(r1)
/* 800E4680 000E15C0  80 01 00 54 */	lwz r0, 0x54(r1)
/* 800E4684 000E15C4  7C 85 22 14 */	add r4, r5, r4
/* 800E4688 000E15C8  90 64 00 00 */	stw r3, 0(r4)
/* 800E468C 000E15CC  90 04 00 04 */	stw r0, 4(r4)
/* 800E4690 000E15D0  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E4694 000E15D4  38 03 00 01 */	addi r0, r3, 1
/* 800E4698 000E15D8  90 1D 00 1C */	stw r0, 0x1c(r29)
lbl_800E469C:
/* 800E469C 000E15DC  BB 61 00 84 */	lmw r27, 0x84(r1)
/* 800E46A0 000E15E0  80 01 00 9C */	lwz r0, 0x9c(r1)
/* 800E46A4 000E15E4  38 21 00 98 */	addi r1, r1, 0x98
/* 800E46A8 000E15E8  7C 08 03 A6 */	mtlr r0
/* 800E46AC 000E15EC  4E 80 00 20 */	blr 

.global evIsTalkModeNow__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evIsTalkModeNow__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E46B0 000E15F0  7C 08 02 A6 */	mflr r0
/* 800E46B4 000E15F4  90 01 00 04 */	stw r0, 4(r1)
/* 800E46B8 000E15F8  94 21 FF D0 */	stwu r1, -0x30(r1)
/* 800E46BC 000E15FC  93 E1 00 2C */	stw r31, 0x2c(r1)
/* 800E46C0 000E1600  7C 7F 1B 78 */	mr r31, r3
/* 800E46C4 000E1604  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E46C8 000E1608  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E46CC 000E160C  38 80 00 00 */	li r4, 0
/* 800E46D0 000E1610  48 03 2D 61 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E46D4 000E1614  80 8D 97 E8 */	lwz r4, gpMarDirector@sda21(r13)
/* 800E46D8 000E1618  38 60 00 01 */	li r3, 1
/* 800E46DC 000E161C  88 04 01 24 */	lbz r0, 0x124(r4)
/* 800E46E0 000E1620  28 00 00 01 */	cmplwi r0, 1
/* 800E46E4 000E1624  41 82 00 10 */	beq lbl_800E46F4
/* 800E46E8 000E1628  28 00 00 02 */	cmplwi r0, 2
/* 800E46EC 000E162C  41 82 00 08 */	beq lbl_800E46F4
/* 800E46F0 000E1630  38 60 00 00 */	li r3, 0
lbl_800E46F4:
/* 800E46F4 000E1634  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 800E46F8 000E1638  41 82 00 0C */	beq lbl_800E4704
/* 800E46FC 000E163C  38 60 00 01 */	li r3, 1
/* 800E4700 000E1640  48 00 00 08 */	b lbl_800E4708
lbl_800E4704:
/* 800E4704 000E1644  38 60 00 00 */	li r3, 0
lbl_800E4708:
/* 800E4708 000E1648  38 00 00 00 */	li r0, 0
/* 800E470C 000E164C  90 01 00 18 */	stw r0, 0x18(r1)
/* 800E4710 000E1650  90 61 00 1C */	stw r3, 0x1c(r1)
/* 800E4714 000E1654  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E4718 000E1658  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800E471C 000E165C  7C 03 00 00 */	cmpw r3, r0
/* 800E4720 000E1660  41 80 00 18 */	blt lbl_800E4738
/* 800E4724 000E1664  3C 60 80 37 */	lis r3, $$23834@ha
/* 800E4728 000E1668  4C C6 31 82 */	crclr 6
/* 800E472C 000E166C  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800E4730 000E1670  4B FF 89 75 */	bl SpcTrace__FPCce
/* 800E4734 000E1674  48 00 00 2C */	b lbl_800E4760
lbl_800E4738:
/* 800E4738 000E1678  80 BF 00 20 */	lwz r5, 0x20(r31)
/* 800E473C 000E167C  54 64 18 38 */	slwi r4, r3, 3
/* 800E4740 000E1680  80 61 00 18 */	lwz r3, 0x18(r1)
/* 800E4744 000E1684  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 800E4748 000E1688  7C 85 22 14 */	add r4, r5, r4
/* 800E474C 000E168C  90 64 00 00 */	stw r3, 0(r4)
/* 800E4750 000E1690  90 04 00 04 */	stw r0, 4(r4)
/* 800E4754 000E1694  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E4758 000E1698  38 03 00 01 */	addi r0, r3, 1
/* 800E475C 000E169C  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800E4760:
/* 800E4760 000E16A0  80 01 00 34 */	lwz r0, 0x34(r1)
/* 800E4764 000E16A4  83 E1 00 2C */	lwz r31, 0x2c(r1)
/* 800E4768 000E16A8  38 21 00 30 */	addi r1, r1, 0x30
/* 800E476C 000E16AC  7C 08 03 A6 */	mtlr r0
/* 800E4770 000E16B0  4E 80 00 20 */	blr 

.global evSetValue2TalkVariable__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evSetValue2TalkVariable__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E4774 000E16B4  7C 08 02 A6 */	mflr r0
/* 800E4778 000E16B8  90 01 00 04 */	stw r0, 4(r1)
/* 800E477C 000E16BC  94 21 FF 60 */	stwu r1, -0xa0(r1)
/* 800E4780 000E16C0  93 E1 00 9C */	stw r31, 0x9c(r1)
/* 800E4784 000E16C4  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E4788 000E16C8  93 C1 00 98 */	stw r30, 0x98(r1)
/* 800E478C 000E16CC  7C 7E 1B 78 */	mr r30, r3
/* 800E4790 000E16D0  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E4794 000E16D4  3C 80 80 37 */	lis r4, $$21490@ha
/* 800E4798 000E16D8  3B E4 29 90 */	addi r31, r4, $$21490@l
/* 800E479C 000E16DC  38 80 00 02 */	li r4, 2
/* 800E47A0 000E16E0  48 03 2C 91 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E47A4 000E16E4  80 7E 00 1C */	lwz r3, 0x1c(r30)
/* 800E47A8 000E16E8  2C 03 00 00 */	cmpwi r3, 0
/* 800E47AC 000E16EC  41 81 00 14 */	bgt lbl_800E47C0
/* 800E47B0 000E16F0  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800E47B4 000E16F4  4C C6 31 82 */	crclr 6
/* 800E47B8 000E16F8  4B FF 88 ED */	bl SpcTrace__FPCce
/* 800E47BC 000E16FC  48 00 00 0C */	b lbl_800E47C8
lbl_800E47C0:
/* 800E47C0 000E1700  38 03 FF FF */	addi r0, r3, -1
/* 800E47C4 000E1704  90 1E 00 1C */	stw r0, 0x1c(r30)
lbl_800E47C8:
/* 800E47C8 000E1708  80 7E 00 1C */	lwz r3, 0x1c(r30)
/* 800E47CC 000E170C  2C 03 00 00 */	cmpwi r3, 0
/* 800E47D0 000E1710  41 81 00 14 */	bgt lbl_800E47E4
/* 800E47D4 000E1714  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800E47D8 000E1718  4C C6 31 82 */	crclr 6
/* 800E47DC 000E171C  4B FF 88 C9 */	bl SpcTrace__FPCce
/* 800E47E0 000E1720  48 00 00 0C */	b lbl_800E47EC
lbl_800E47E4:
/* 800E47E4 000E1724  38 03 FF FF */	addi r0, r3, -1
/* 800E47E8 000E1728  90 1E 00 1C */	stw r0, 0x1c(r30)
lbl_800E47EC:
/* 800E47EC 000E172C  38 00 00 00 */	li r0, 0
/* 800E47F0 000E1730  90 01 00 58 */	stw r0, 0x58(r1)
/* 800E47F4 000E1734  90 01 00 5C */	stw r0, 0x5c(r1)
/* 800E47F8 000E1738  80 7E 00 1C */	lwz r3, 0x1c(r30)
/* 800E47FC 000E173C  80 1E 00 18 */	lwz r0, 0x18(r30)
/* 800E4800 000E1740  7C 03 00 00 */	cmpw r3, r0
/* 800E4804 000E1744  41 80 00 14 */	blt lbl_800E4818
/* 800E4808 000E1748  38 7F 07 8C */	addi r3, r31, 0x78c
/* 800E480C 000E174C  4C C6 31 82 */	crclr 6
/* 800E4810 000E1750  4B FF 88 95 */	bl SpcTrace__FPCce
/* 800E4814 000E1754  48 00 00 2C */	b lbl_800E4840
lbl_800E4818:
/* 800E4818 000E1758  80 BE 00 20 */	lwz r5, 0x20(r30)
/* 800E481C 000E175C  54 64 18 38 */	slwi r4, r3, 3
/* 800E4820 000E1760  80 61 00 58 */	lwz r3, 0x58(r1)
/* 800E4824 000E1764  80 01 00 5C */	lwz r0, 0x5c(r1)
/* 800E4828 000E1768  7C 85 22 14 */	add r4, r5, r4
/* 800E482C 000E176C  90 64 00 00 */	stw r3, 0(r4)
/* 800E4830 000E1770  90 04 00 04 */	stw r0, 4(r4)
/* 800E4834 000E1774  80 7E 00 1C */	lwz r3, 0x1c(r30)
/* 800E4838 000E1778  38 03 00 01 */	addi r0, r3, 1
/* 800E483C 000E177C  90 1E 00 1C */	stw r0, 0x1c(r30)
lbl_800E4840:
/* 800E4840 000E1780  80 01 00 A4 */	lwz r0, 0xa4(r1)
/* 800E4844 000E1784  83 E1 00 9C */	lwz r31, 0x9c(r1)
/* 800E4848 000E1788  83 C1 00 98 */	lwz r30, 0x98(r1)
/* 800E484C 000E178C  7C 08 03 A6 */	mtlr r0
/* 800E4850 000E1790  38 21 00 A0 */	addi r1, r1, 0xa0
/* 800E4854 000E1794  4E 80 00 20 */	blr 

.global evGetTalkSelectedValue__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evGetTalkSelectedValue__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E4858 000E1798  7C 08 02 A6 */	mflr r0
/* 800E485C 000E179C  90 01 00 04 */	stw r0, 4(r1)
/* 800E4860 000E17A0  94 21 FF C8 */	stwu r1, -0x38(r1)
/* 800E4864 000E17A4  93 E1 00 34 */	stw r31, 0x34(r1)
/* 800E4868 000E17A8  7C 7F 1B 78 */	mr r31, r3
/* 800E486C 000E17AC  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E4870 000E17B0  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E4874 000E17B4  38 80 00 00 */	li r4, 0
/* 800E4878 000E17B8  48 03 2B B9 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E487C 000E17BC  38 00 00 00 */	li r0, 0
/* 800E4880 000E17C0  80 6D 9B 68 */	lwz r3, gpTalk2D@sda21(r13)
/* 800E4884 000E17C4  90 01 00 1C */	stw r0, 0x1c(r1)
/* 800E4888 000E17C8  88 03 02 14 */	lbz r0, 0x214(r3)
/* 800E488C 000E17CC  7C 00 07 74 */	extsb r0, r0
/* 800E4890 000E17D0  90 01 00 20 */	stw r0, 0x20(r1)
/* 800E4894 000E17D4  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E4898 000E17D8  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800E489C 000E17DC  7C 03 00 00 */	cmpw r3, r0
/* 800E48A0 000E17E0  41 80 00 18 */	blt lbl_800E48B8
/* 800E48A4 000E17E4  3C 60 80 37 */	lis r3, $$23834@ha
/* 800E48A8 000E17E8  4C C6 31 82 */	crclr 6
/* 800E48AC 000E17EC  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800E48B0 000E17F0  4B FF 87 F5 */	bl SpcTrace__FPCce
/* 800E48B4 000E17F4  48 00 00 2C */	b lbl_800E48E0
lbl_800E48B8:
/* 800E48B8 000E17F8  80 BF 00 20 */	lwz r5, 0x20(r31)
/* 800E48BC 000E17FC  54 64 18 38 */	slwi r4, r3, 3
/* 800E48C0 000E1800  80 61 00 1C */	lwz r3, 0x1c(r1)
/* 800E48C4 000E1804  80 01 00 20 */	lwz r0, 0x20(r1)
/* 800E48C8 000E1808  7C 85 22 14 */	add r4, r5, r4
/* 800E48CC 000E180C  90 64 00 00 */	stw r3, 0(r4)
/* 800E48D0 000E1810  90 04 00 04 */	stw r0, 4(r4)
/* 800E48D4 000E1814  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E48D8 000E1818  38 03 00 01 */	addi r0, r3, 1
/* 800E48DC 000E181C  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800E48E0:
/* 800E48E0 000E1820  80 01 00 3C */	lwz r0, 0x3c(r1)
/* 800E48E4 000E1824  83 E1 00 34 */	lwz r31, 0x34(r1)
/* 800E48E8 000E1828  38 21 00 38 */	addi r1, r1, 0x38
/* 800E48EC 000E182C  7C 08 03 A6 */	mtlr r0
/* 800E48F0 000E1830  4E 80 00 20 */	blr 

.global evGetTalkMode__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evGetTalkMode__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E48F4 000E1834  7C 08 02 A6 */	mflr r0
/* 800E48F8 000E1838  90 01 00 04 */	stw r0, 4(r1)
/* 800E48FC 000E183C  94 21 FF C8 */	stwu r1, -0x38(r1)
/* 800E4900 000E1840  93 E1 00 34 */	stw r31, 0x34(r1)
/* 800E4904 000E1844  7C 7F 1B 78 */	mr r31, r3
/* 800E4908 000E1848  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E490C 000E184C  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E4910 000E1850  38 80 00 00 */	li r4, 0
/* 800E4914 000E1854  48 03 2B 1D */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E4918 000E1858  38 00 00 00 */	li r0, 0
/* 800E491C 000E185C  80 6D 9B 68 */	lwz r3, gpTalk2D@sda21(r13)
/* 800E4920 000E1860  90 01 00 20 */	stw r0, 0x20(r1)
/* 800E4924 000E1864  80 03 02 48 */	lwz r0, 0x248(r3)
/* 800E4928 000E1868  90 01 00 24 */	stw r0, 0x24(r1)
/* 800E492C 000E186C  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E4930 000E1870  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800E4934 000E1874  7C 03 00 00 */	cmpw r3, r0
/* 800E4938 000E1878  41 80 00 18 */	blt lbl_800E4950
/* 800E493C 000E187C  3C 60 80 37 */	lis r3, $$23834@ha
/* 800E4940 000E1880  4C C6 31 82 */	crclr 6
/* 800E4944 000E1884  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800E4948 000E1888  4B FF 87 5D */	bl SpcTrace__FPCce
/* 800E494C 000E188C  48 00 00 2C */	b lbl_800E4978
lbl_800E4950:
/* 800E4950 000E1890  80 BF 00 20 */	lwz r5, 0x20(r31)
/* 800E4954 000E1894  54 64 18 38 */	slwi r4, r3, 3
/* 800E4958 000E1898  80 61 00 20 */	lwz r3, 0x20(r1)
/* 800E495C 000E189C  80 01 00 24 */	lwz r0, 0x24(r1)
/* 800E4960 000E18A0  7C 85 22 14 */	add r4, r5, r4
/* 800E4964 000E18A4  90 64 00 00 */	stw r3, 0(r4)
/* 800E4968 000E18A8  90 04 00 04 */	stw r0, 4(r4)
/* 800E496C 000E18AC  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E4970 000E18B0  38 03 00 01 */	addi r0, r3, 1
/* 800E4974 000E18B4  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800E4978:
/* 800E4978 000E18B8  80 01 00 3C */	lwz r0, 0x3c(r1)
/* 800E497C 000E18BC  83 E1 00 34 */	lwz r31, 0x34(r1)
/* 800E4980 000E18C0  38 21 00 38 */	addi r1, r1, 0x38
/* 800E4984 000E18C4  7C 08 03 A6 */	mtlr r0
/* 800E4988 000E18C8  4E 80 00 20 */	blr 

.global evSetTalkMsgID__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evSetTalkMsgID__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E498C 000E18CC  7C 08 02 A6 */	mflr r0
/* 800E4990 000E18D0  90 01 00 04 */	stw r0, 4(r1)
/* 800E4994 000E18D4  94 21 FF 78 */	stwu r1, -0x88(r1)
/* 800E4998 000E18D8  93 E1 00 84 */	stw r31, 0x84(r1)
/* 800E499C 000E18DC  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E49A0 000E18E0  93 C1 00 80 */	stw r30, 0x80(r1)
/* 800E49A4 000E18E4  93 A1 00 7C */	stw r29, 0x7c(r1)
/* 800E49A8 000E18E8  7C 7D 1B 78 */	mr r29, r3
/* 800E49AC 000E18EC  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E49B0 000E18F0  3C 80 80 37 */	lis r4, $$21490@ha
/* 800E49B4 000E18F4  3B E4 29 90 */	addi r31, r4, $$21490@l
/* 800E49B8 000E18F8  38 80 00 02 */	li r4, 2
/* 800E49BC 000E18FC  48 03 2A 75 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E49C0 000E1900  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E49C4 000E1904  2C 03 00 00 */	cmpwi r3, 0
/* 800E49C8 000E1908  41 81 00 30 */	bgt lbl_800E49F8
/* 800E49CC 000E190C  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800E49D0 000E1910  4C C6 31 82 */	crclr 6
/* 800E49D4 000E1914  4B FF 86 D1 */	bl SpcTrace__FPCce
/* 800E49D8 000E1918  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E49DC 000E191C  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E49E0 000E1920  54 00 18 38 */	slwi r0, r0, 3
/* 800E49E4 000E1924  7C 63 02 14 */	add r3, r3, r0
/* 800E49E8 000E1928  80 83 00 00 */	lwz r4, 0(r3)
/* 800E49EC 000E192C  80 03 00 04 */	lwz r0, 4(r3)
/* 800E49F0 000E1930  90 01 00 60 */	stw r0, 0x60(r1)
/* 800E49F4 000E1934  48 00 00 28 */	b lbl_800E4A1C
lbl_800E49F8:
/* 800E49F8 000E1938  38 03 FF FF */	addi r0, r3, -1
/* 800E49FC 000E193C  90 1D 00 1C */	stw r0, 0x1c(r29)
/* 800E4A00 000E1940  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E4A04 000E1944  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E4A08 000E1948  54 00 18 38 */	slwi r0, r0, 3
/* 800E4A0C 000E194C  7C 63 02 14 */	add r3, r3, r0
/* 800E4A10 000E1950  80 83 00 00 */	lwz r4, 0(r3)
/* 800E4A14 000E1954  80 03 00 04 */	lwz r0, 4(r3)
/* 800E4A18 000E1958  90 01 00 60 */	stw r0, 0x60(r1)
lbl_800E4A1C:
/* 800E4A1C 000E195C  90 81 00 64 */	stw r4, 0x64(r1)
/* 800E4A20 000E1960  80 01 00 60 */	lwz r0, 0x60(r1)
/* 800E4A24 000E1964  90 01 00 68 */	stw r0, 0x68(r1)
/* 800E4A28 000E1968  80 01 00 64 */	lwz r0, 0x64(r1)
/* 800E4A2C 000E196C  2C 00 00 01 */	cmpwi r0, 1
/* 800E4A30 000E1970  41 82 00 1C */	beq lbl_800E4A4C
/* 800E4A34 000E1974  40 80 00 2C */	bge lbl_800E4A60
/* 800E4A38 000E1978  2C 00 00 00 */	cmpwi r0, 0
/* 800E4A3C 000E197C  40 80 00 08 */	bge lbl_800E4A44
/* 800E4A40 000E1980  48 00 00 20 */	b lbl_800E4A60
lbl_800E4A44:
/* 800E4A44 000E1984  80 01 00 68 */	lwz r0, 0x68(r1)
/* 800E4A48 000E1988  48 00 00 1C */	b lbl_800E4A64
lbl_800E4A4C:
/* 800E4A4C 000E198C  C0 01 00 68 */	lfs f0, 0x68(r1)
/* 800E4A50 000E1990  FC 00 00 1E */	fctiwz f0, f0
/* 800E4A54 000E1994  D8 01 00 70 */	stfd f0, 0x70(r1)
/* 800E4A58 000E1998  80 01 00 74 */	lwz r0, 0x74(r1)
/* 800E4A5C 000E199C  48 00 00 08 */	b lbl_800E4A64
lbl_800E4A60:
/* 800E4A60 000E19A0  38 00 00 00 */	li r0, 0
lbl_800E4A64:
/* 800E4A64 000E19A4  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E4A68 000E19A8  7C 1E 03 78 */	mr r30, r0
/* 800E4A6C 000E19AC  2C 03 00 00 */	cmpwi r3, 0
/* 800E4A70 000E19B0  41 81 00 30 */	bgt lbl_800E4AA0
/* 800E4A74 000E19B4  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800E4A78 000E19B8  4C C6 31 82 */	crclr 6
/* 800E4A7C 000E19BC  4B FF 86 29 */	bl SpcTrace__FPCce
/* 800E4A80 000E19C0  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E4A84 000E19C4  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E4A88 000E19C8  54 00 18 38 */	slwi r0, r0, 3
/* 800E4A8C 000E19CC  7C 63 02 14 */	add r3, r3, r0
/* 800E4A90 000E19D0  80 83 00 00 */	lwz r4, 0(r3)
/* 800E4A94 000E19D4  80 03 00 04 */	lwz r0, 4(r3)
/* 800E4A98 000E19D8  90 01 00 50 */	stw r0, 0x50(r1)
/* 800E4A9C 000E19DC  48 00 00 28 */	b lbl_800E4AC4
lbl_800E4AA0:
/* 800E4AA0 000E19E0  38 03 FF FF */	addi r0, r3, -1
/* 800E4AA4 000E19E4  90 1D 00 1C */	stw r0, 0x1c(r29)
/* 800E4AA8 000E19E8  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E4AAC 000E19EC  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E4AB0 000E19F0  54 00 18 38 */	slwi r0, r0, 3
/* 800E4AB4 000E19F4  7C 63 02 14 */	add r3, r3, r0
/* 800E4AB8 000E19F8  80 83 00 00 */	lwz r4, 0(r3)
/* 800E4ABC 000E19FC  80 03 00 04 */	lwz r0, 4(r3)
/* 800E4AC0 000E1A00  90 01 00 50 */	stw r0, 0x50(r1)
lbl_800E4AC4:
/* 800E4AC4 000E1A04  90 81 00 54 */	stw r4, 0x54(r1)
/* 800E4AC8 000E1A08  80 01 00 50 */	lwz r0, 0x50(r1)
/* 800E4ACC 000E1A0C  90 01 00 58 */	stw r0, 0x58(r1)
/* 800E4AD0 000E1A10  80 01 00 54 */	lwz r0, 0x54(r1)
/* 800E4AD4 000E1A14  2C 00 00 01 */	cmpwi r0, 1
/* 800E4AD8 000E1A18  41 82 00 1C */	beq lbl_800E4AF4
/* 800E4ADC 000E1A1C  40 80 00 2C */	bge lbl_800E4B08
/* 800E4AE0 000E1A20  2C 00 00 00 */	cmpwi r0, 0
/* 800E4AE4 000E1A24  40 80 00 08 */	bge lbl_800E4AEC
/* 800E4AE8 000E1A28  48 00 00 20 */	b lbl_800E4B08
lbl_800E4AEC:
/* 800E4AEC 000E1A2C  80 01 00 58 */	lwz r0, 0x58(r1)
/* 800E4AF0 000E1A30  48 00 00 1C */	b lbl_800E4B0C
lbl_800E4AF4:
/* 800E4AF4 000E1A34  C0 01 00 58 */	lfs f0, 0x58(r1)
/* 800E4AF8 000E1A38  FC 00 00 1E */	fctiwz f0, f0
/* 800E4AFC 000E1A3C  D8 01 00 70 */	stfd f0, 0x70(r1)
/* 800E4B00 000E1A40  80 01 00 74 */	lwz r0, 0x74(r1)
/* 800E4B04 000E1A44  48 00 00 08 */	b lbl_800E4B0C
lbl_800E4B08:
/* 800E4B08 000E1A48  38 00 00 00 */	li r0, 0
lbl_800E4B0C:
/* 800E4B0C 000E1A4C  80 6D 9B 68 */	lwz r3, gpTalk2D@sda21(r13)
/* 800E4B10 000E1A50  7C 04 03 78 */	mr r4, r0
/* 800E4B14 000E1A54  38 BE 00 00 */	addi r5, r30, 0
/* 800E4B18 000E1A58  48 13 04 8D */	bl setMessageID__8TTalk2D2FUlUl
/* 800E4B1C 000E1A5C  38 00 00 00 */	li r0, 0
/* 800E4B20 000E1A60  90 01 00 3C */	stw r0, 0x3c(r1)
/* 800E4B24 000E1A64  90 01 00 40 */	stw r0, 0x40(r1)
/* 800E4B28 000E1A68  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E4B2C 000E1A6C  80 1D 00 18 */	lwz r0, 0x18(r29)
/* 800E4B30 000E1A70  7C 03 00 00 */	cmpw r3, r0
/* 800E4B34 000E1A74  41 80 00 14 */	blt lbl_800E4B48
/* 800E4B38 000E1A78  38 7F 07 8C */	addi r3, r31, 0x78c
/* 800E4B3C 000E1A7C  4C C6 31 82 */	crclr 6
/* 800E4B40 000E1A80  4B FF 85 65 */	bl SpcTrace__FPCce
/* 800E4B44 000E1A84  48 00 00 2C */	b lbl_800E4B70
lbl_800E4B48:
/* 800E4B48 000E1A88  80 BD 00 20 */	lwz r5, 0x20(r29)
/* 800E4B4C 000E1A8C  54 64 18 38 */	slwi r4, r3, 3
/* 800E4B50 000E1A90  80 61 00 3C */	lwz r3, 0x3c(r1)
/* 800E4B54 000E1A94  80 01 00 40 */	lwz r0, 0x40(r1)
/* 800E4B58 000E1A98  7C 85 22 14 */	add r4, r5, r4
/* 800E4B5C 000E1A9C  90 64 00 00 */	stw r3, 0(r4)
/* 800E4B60 000E1AA0  90 04 00 04 */	stw r0, 4(r4)
/* 800E4B64 000E1AA4  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E4B68 000E1AA8  38 03 00 01 */	addi r0, r3, 1
/* 800E4B6C 000E1AAC  90 1D 00 1C */	stw r0, 0x1c(r29)
lbl_800E4B70:
/* 800E4B70 000E1AB0  80 01 00 8C */	lwz r0, 0x8c(r1)
/* 800E4B74 000E1AB4  83 E1 00 84 */	lwz r31, 0x84(r1)
/* 800E4B78 000E1AB8  83 C1 00 80 */	lwz r30, 0x80(r1)
/* 800E4B7C 000E1ABC  7C 08 03 A6 */	mtlr r0
/* 800E4B80 000E1AC0  83 A1 00 7C */	lwz r29, 0x7c(r1)
/* 800E4B84 000E1AC4  38 21 00 88 */	addi r1, r1, 0x88
/* 800E4B88 000E1AC8  4E 80 00 20 */	blr 

.global evGetTalkNPCName__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evGetTalkNPCName__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E4B8C 000E1ACC  7C 08 02 A6 */	mflr r0
/* 800E4B90 000E1AD0  90 01 00 04 */	stw r0, 4(r1)
/* 800E4B94 000E1AD4  94 21 FF C0 */	stwu r1, -0x40(r1)
/* 800E4B98 000E1AD8  93 E1 00 3C */	stw r31, 0x3c(r1)
/* 800E4B9C 000E1ADC  7C 7F 1B 78 */	mr r31, r3
/* 800E4BA0 000E1AE0  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E4BA4 000E1AE4  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E4BA8 000E1AE8  38 80 00 00 */	li r4, 0
/* 800E4BAC 000E1AEC  48 03 28 85 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E4BB0 000E1AF0  80 6D 97 E8 */	lwz r3, gpMarDirector@sda21(r13)
/* 800E4BB4 000E1AF4  80 63 00 A0 */	lwz r3, 0xa0(r3)
/* 800E4BB8 000E1AF8  28 03 00 00 */	cmplwi r3, 0
/* 800E4BBC 000E1AFC  40 82 00 80 */	bne lbl_800E4C3C
/* 800E4BC0 000E1B00  38 60 00 00 */	li r3, 0
/* 800E4BC4 000E1B04  90 61 00 28 */	stw r3, 0x28(r1)
/* 800E4BC8 000E1B08  38 02 8F F0 */	addi r0, r2, $$24053@sda21
/* 800E4BCC 000E1B0C  28 00 00 00 */	cmplwi r0, 0
/* 800E4BD0 000E1B10  90 61 00 2C */	stw r3, 0x2c(r1)
/* 800E4BD4 000E1B14  38 60 00 02 */	li r3, 2
/* 800E4BD8 000E1B18  90 61 00 28 */	stw r3, 0x28(r1)
/* 800E4BDC 000E1B1C  40 82 00 0C */	bne lbl_800E4BE8
/* 800E4BE0 000E1B20  90 01 00 2C */	stw r0, 0x2c(r1)
/* 800E4BE4 000E1B24  48 00 00 08 */	b lbl_800E4BEC
lbl_800E4BE8:
/* 800E4BE8 000E1B28  90 01 00 2C */	stw r0, 0x2c(r1)
lbl_800E4BEC:
/* 800E4BEC 000E1B2C  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E4BF0 000E1B30  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800E4BF4 000E1B34  7C 03 00 00 */	cmpw r3, r0
/* 800E4BF8 000E1B38  41 80 00 18 */	blt lbl_800E4C10
/* 800E4BFC 000E1B3C  3C 60 80 37 */	lis r3, $$23834@ha
/* 800E4C00 000E1B40  4C C6 31 82 */	crclr 6
/* 800E4C04 000E1B44  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800E4C08 000E1B48  4B FF 84 9D */	bl SpcTrace__FPCce
/* 800E4C0C 000E1B4C  48 00 00 AC */	b lbl_800E4CB8
lbl_800E4C10:
/* 800E4C10 000E1B50  80 BF 00 20 */	lwz r5, 0x20(r31)
/* 800E4C14 000E1B54  54 64 18 38 */	slwi r4, r3, 3
/* 800E4C18 000E1B58  80 61 00 28 */	lwz r3, 0x28(r1)
/* 800E4C1C 000E1B5C  80 01 00 2C */	lwz r0, 0x2c(r1)
/* 800E4C20 000E1B60  7C 85 22 14 */	add r4, r5, r4
/* 800E4C24 000E1B64  90 64 00 00 */	stw r3, 0(r4)
/* 800E4C28 000E1B68  90 04 00 04 */	stw r0, 4(r4)
/* 800E4C2C 000E1B6C  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E4C30 000E1B70  38 03 00 01 */	addi r0, r3, 1
/* 800E4C34 000E1B74  90 1F 00 1C */	stw r0, 0x1c(r31)
/* 800E4C38 000E1B78  48 00 00 80 */	b lbl_800E4CB8
lbl_800E4C3C:
/* 800E4C3C 000E1B7C  80 83 00 04 */	lwz r4, 4(r3)
/* 800E4C40 000E1B80  38 60 00 00 */	li r3, 0
/* 800E4C44 000E1B84  38 00 00 02 */	li r0, 2
/* 800E4C48 000E1B88  90 61 00 20 */	stw r3, 0x20(r1)
/* 800E4C4C 000E1B8C  28 04 00 00 */	cmplwi r4, 0
/* 800E4C50 000E1B90  90 61 00 24 */	stw r3, 0x24(r1)
/* 800E4C54 000E1B94  90 01 00 20 */	stw r0, 0x20(r1)
/* 800E4C58 000E1B98  40 82 00 10 */	bne lbl_800E4C68
/* 800E4C5C 000E1B9C  38 02 8F F0 */	addi r0, r2, $$24053@sda21
/* 800E4C60 000E1BA0  90 01 00 24 */	stw r0, 0x24(r1)
/* 800E4C64 000E1BA4  48 00 00 08 */	b lbl_800E4C6C
lbl_800E4C68:
/* 800E4C68 000E1BA8  90 81 00 24 */	stw r4, 0x24(r1)
lbl_800E4C6C:
/* 800E4C6C 000E1BAC  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E4C70 000E1BB0  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800E4C74 000E1BB4  7C 03 00 00 */	cmpw r3, r0
/* 800E4C78 000E1BB8  41 80 00 18 */	blt lbl_800E4C90
/* 800E4C7C 000E1BBC  3C 60 80 37 */	lis r3, $$23834@ha
/* 800E4C80 000E1BC0  4C C6 31 82 */	crclr 6
/* 800E4C84 000E1BC4  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800E4C88 000E1BC8  4B FF 84 1D */	bl SpcTrace__FPCce
/* 800E4C8C 000E1BCC  48 00 00 2C */	b lbl_800E4CB8
lbl_800E4C90:
/* 800E4C90 000E1BD0  80 BF 00 20 */	lwz r5, 0x20(r31)
/* 800E4C94 000E1BD4  54 64 18 38 */	slwi r4, r3, 3
/* 800E4C98 000E1BD8  80 61 00 20 */	lwz r3, 0x20(r1)
/* 800E4C9C 000E1BDC  80 01 00 24 */	lwz r0, 0x24(r1)
/* 800E4CA0 000E1BE0  7C 85 22 14 */	add r4, r5, r4
/* 800E4CA4 000E1BE4  90 64 00 00 */	stw r3, 0(r4)
/* 800E4CA8 000E1BE8  90 04 00 04 */	stw r0, 4(r4)
/* 800E4CAC 000E1BEC  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E4CB0 000E1BF0  38 03 00 01 */	addi r0, r3, 1
/* 800E4CB4 000E1BF4  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800E4CB8:
/* 800E4CB8 000E1BF8  80 01 00 44 */	lwz r0, 0x44(r1)
/* 800E4CBC 000E1BFC  83 E1 00 3C */	lwz r31, 0x3c(r1)
/* 800E4CC0 000E1C00  38 21 00 40 */	addi r1, r1, 0x40
/* 800E4CC4 000E1C04  7C 08 03 A6 */	mtlr r0
/* 800E4CC8 000E1C08  4E 80 00 20 */	blr 

.global evGetTalkNPC__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evGetTalkNPC__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E4CCC 000E1C0C  7C 08 02 A6 */	mflr r0
/* 800E4CD0 000E1C10  90 01 00 04 */	stw r0, 4(r1)
/* 800E4CD4 000E1C14  94 21 FF C8 */	stwu r1, -0x38(r1)
/* 800E4CD8 000E1C18  93 E1 00 34 */	stw r31, 0x34(r1)
/* 800E4CDC 000E1C1C  7C 7F 1B 78 */	mr r31, r3
/* 800E4CE0 000E1C20  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E4CE4 000E1C24  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E4CE8 000E1C28  38 80 00 00 */	li r4, 0
/* 800E4CEC 000E1C2C  48 03 27 45 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E4CF0 000E1C30  80 6D 97 E8 */	lwz r3, gpMarDirector@sda21(r13)
/* 800E4CF4 000E1C34  38 00 00 00 */	li r0, 0
/* 800E4CF8 000E1C38  80 63 00 A0 */	lwz r3, 0xa0(r3)
/* 800E4CFC 000E1C3C  28 03 00 00 */	cmplwi r3, 0
/* 800E4D00 000E1C40  90 01 00 1C */	stw r0, 0x1c(r1)
/* 800E4D04 000E1C44  40 82 00 08 */	bne lbl_800E4D0C
/* 800E4D08 000E1C48  48 00 00 08 */	b lbl_800E4D10
lbl_800E4D0C:
/* 800E4D0C 000E1C4C  7C 60 1B 78 */	mr r0, r3
lbl_800E4D10:
/* 800E4D10 000E1C50  90 01 00 20 */	stw r0, 0x20(r1)
/* 800E4D14 000E1C54  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E4D18 000E1C58  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800E4D1C 000E1C5C  7C 03 00 00 */	cmpw r3, r0
/* 800E4D20 000E1C60  41 80 00 18 */	blt lbl_800E4D38
/* 800E4D24 000E1C64  3C 60 80 37 */	lis r3, $$23834@ha
/* 800E4D28 000E1C68  4C C6 31 82 */	crclr 6
/* 800E4D2C 000E1C6C  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800E4D30 000E1C70  4B FF 83 75 */	bl SpcTrace__FPCce
/* 800E4D34 000E1C74  48 00 00 2C */	b lbl_800E4D60
lbl_800E4D38:
/* 800E4D38 000E1C78  80 BF 00 20 */	lwz r5, 0x20(r31)
/* 800E4D3C 000E1C7C  54 64 18 38 */	slwi r4, r3, 3
/* 800E4D40 000E1C80  80 61 00 1C */	lwz r3, 0x1c(r1)
/* 800E4D44 000E1C84  80 01 00 20 */	lwz r0, 0x20(r1)
/* 800E4D48 000E1C88  7C 85 22 14 */	add r4, r5, r4
/* 800E4D4C 000E1C8C  90 64 00 00 */	stw r3, 0(r4)
/* 800E4D50 000E1C90  90 04 00 04 */	stw r0, 4(r4)
/* 800E4D54 000E1C94  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E4D58 000E1C98  38 03 00 01 */	addi r0, r3, 1
/* 800E4D5C 000E1C9C  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800E4D60:
/* 800E4D60 000E1CA0  80 01 00 3C */	lwz r0, 0x3c(r1)
/* 800E4D64 000E1CA4  83 E1 00 34 */	lwz r31, 0x34(r1)
/* 800E4D68 000E1CA8  38 21 00 38 */	addi r1, r1, 0x38
/* 800E4D6C 000E1CAC  7C 08 03 A6 */	mtlr r0
/* 800E4D70 000E1CB0  4E 80 00 20 */	blr 

.global evIsNearActors__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evIsNearActors__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E4D74 000E1CB4  7C 08 02 A6 */	mflr r0
/* 800E4D78 000E1CB8  90 01 00 04 */	stw r0, 4(r1)
/* 800E4D7C 000E1CBC  94 21 FF 20 */	stwu r1, -0xe0(r1)
/* 800E4D80 000E1CC0  DB E1 00 D8 */	stfd f31, 0xd8(r1)
/* 800E4D84 000E1CC4  BF 21 00 BC */	stmw r25, 0xbc(r1)
/* 800E4D88 000E1CC8  3B 84 00 00 */	addi r28, r4, 0
/* 800E4D8C 000E1CCC  28 1C 00 03 */	cmplwi r28, 3
/* 800E4D90 000E1CD0  3B 63 00 00 */	addi r27, r3, 0
/* 800E4D94 000E1CD4  3B C0 00 00 */	li r30, 0
/* 800E4D98 000E1CD8  41 80 02 B0 */	blt lbl_800E5048
/* 800E4D9C 000E1CDC  80 1B 00 1C */	lwz r0, 0x1c(r27)
/* 800E4DA0 000E1CE0  3B E0 00 00 */	li r31, 0
/* 800E4DA4 000E1CE4  80 7B 00 20 */	lwz r3, 0x20(r27)
/* 800E4DA8 000E1CE8  7C 1C 00 50 */	subf r0, r28, r0
/* 800E4DAC 000E1CEC  54 00 18 38 */	slwi r0, r0, 3
/* 800E4DB0 000E1CF0  7C 83 02 14 */	add r4, r3, r0
/* 800E4DB4 000E1CF4  80 64 00 00 */	lwz r3, 0(r4)
/* 800E4DB8 000E1CF8  80 04 00 04 */	lwz r0, 4(r4)
/* 800E4DBC 000E1CFC  90 61 00 A0 */	stw r3, 0xa0(r1)
/* 800E4DC0 000E1D00  90 01 00 A4 */	stw r0, 0xa4(r1)
/* 800E4DC4 000E1D04  80 01 00 A0 */	lwz r0, 0xa0(r1)
/* 800E4DC8 000E1D08  2C 00 00 01 */	cmpwi r0, 1
/* 800E4DCC 000E1D0C  41 82 00 A0 */	beq lbl_800E4E6C
/* 800E4DD0 000E1D10  40 80 00 10 */	bge lbl_800E4DE0
/* 800E4DD4 000E1D14  2C 00 00 00 */	cmpwi r0, 0
/* 800E4DD8 000E1D18  40 80 00 5C */	bge lbl_800E4E34
/* 800E4DDC 000E1D1C  48 00 00 90 */	b lbl_800E4E6C
lbl_800E4DE0:
/* 800E4DE0 000E1D20  2C 00 00 03 */	cmpwi r0, 3
/* 800E4DE4 000E1D24  40 80 00 88 */	bge lbl_800E4E6C
/* 800E4DE8 000E1D28  2C 00 00 02 */	cmpwi r0, 2
/* 800E4DEC 000E1D2C  41 82 00 08 */	beq lbl_800E4DF4
/* 800E4DF0 000E1D30  48 00 00 0C */	b lbl_800E4DFC
lbl_800E4DF4:
/* 800E4DF4 000E1D34  83 A1 00 A4 */	lwz r29, 0xa4(r1)
/* 800E4DF8 000E1D38  48 00 00 08 */	b lbl_800E4E00
lbl_800E4DFC:
/* 800E4DFC 000E1D3C  3B A2 8F F0 */	addi r29, r2, $$24053@sda21
lbl_800E4E00:
/* 800E4E00 000E1D40  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 800E4E04 000E1D44  7F A3 EB 78 */	mr r3, r29
/* 800E4E08 000E1D48  83 44 00 04 */	lwz r26, 4(r4)
/* 800E4E0C 000E1D4C  4B F5 F8 81 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800E4E10 000E1D50  81 9A 00 00 */	lwz r12, 0(r26)
/* 800E4E14 000E1D54  38 83 00 00 */	addi r4, r3, 0
/* 800E4E18 000E1D58  38 7A 00 00 */	addi r3, r26, 0
/* 800E4E1C 000E1D5C  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 800E4E20 000E1D60  7F A5 EB 78 */	mr r5, r29
/* 800E4E24 000E1D64  7D 88 03 A6 */	mtlr r12
/* 800E4E28 000E1D68  4E 80 00 21 */	blrl 
/* 800E4E2C 000E1D6C  7C 7F 1B 78 */	mr r31, r3
/* 800E4E30 000E1D70  48 00 00 3C */	b lbl_800E4E6C
lbl_800E4E34:
/* 800E4E34 000E1D74  2C 00 00 01 */	cmpwi r0, 1
/* 800E4E38 000E1D78  41 82 00 1C */	beq lbl_800E4E54
/* 800E4E3C 000E1D7C  40 80 00 2C */	bge lbl_800E4E68
/* 800E4E40 000E1D80  2C 00 00 00 */	cmpwi r0, 0
/* 800E4E44 000E1D84  40 80 00 08 */	bge lbl_800E4E4C
/* 800E4E48 000E1D88  48 00 00 20 */	b lbl_800E4E68
lbl_800E4E4C:
/* 800E4E4C 000E1D8C  83 E1 00 A4 */	lwz r31, 0xa4(r1)
/* 800E4E50 000E1D90  48 00 00 1C */	b lbl_800E4E6C
lbl_800E4E54:
/* 800E4E54 000E1D94  C0 01 00 A4 */	lfs f0, 0xa4(r1)
/* 800E4E58 000E1D98  FC 00 00 1E */	fctiwz f0, f0
/* 800E4E5C 000E1D9C  D8 01 00 B0 */	stfd f0, 0xb0(r1)
/* 800E4E60 000E1DA0  83 E1 00 B4 */	lwz r31, 0xb4(r1)
/* 800E4E64 000E1DA4  48 00 00 08 */	b lbl_800E4E6C
lbl_800E4E68:
/* 800E4E68 000E1DA8  3B E0 00 00 */	li r31, 0
lbl_800E4E6C:
/* 800E4E6C 000E1DAC  28 1F 00 00 */	cmplwi r31, 0
/* 800E4E70 000E1DB0  41 82 01 D8 */	beq lbl_800E5048
/* 800E4E74 000E1DB4  80 7B 00 1C */	lwz r3, 0x1c(r27)
/* 800E4E78 000E1DB8  38 9C FF FE */	addi r4, r28, -2
/* 800E4E7C 000E1DBC  80 BB 00 20 */	lwz r5, 0x20(r27)
/* 800E4E80 000E1DC0  38 03 FF FF */	addi r0, r3, -1
/* 800E4E84 000E1DC4  7C 04 00 50 */	subf r0, r4, r0
/* 800E4E88 000E1DC8  54 00 18 38 */	slwi r0, r0, 3
/* 800E4E8C 000E1DCC  7C 65 02 14 */	add r3, r5, r0
/* 800E4E90 000E1DD0  80 03 00 00 */	lwz r0, 0(r3)
/* 800E4E94 000E1DD4  2C 00 00 01 */	cmpwi r0, 1
/* 800E4E98 000E1DD8  41 82 00 38 */	beq lbl_800E4ED0
/* 800E4E9C 000E1DDC  40 80 00 3C */	bge lbl_800E4ED8
/* 800E4EA0 000E1DE0  2C 00 00 00 */	cmpwi r0, 0
/* 800E4EA4 000E1DE4  40 80 00 08 */	bge lbl_800E4EAC
/* 800E4EA8 000E1DE8  48 00 00 30 */	b lbl_800E4ED8
lbl_800E4EAC:
/* 800E4EAC 000E1DEC  80 63 00 04 */	lwz r3, 4(r3)
/* 800E4EB0 000E1DF0  3C 00 43 30 */	lis r0, 0x4330
/* 800E4EB4 000E1DF4  C8 22 8F E0 */	lfd f1, $$23934@sda21(r2)
/* 800E4EB8 000E1DF8  6C 63 80 00 */	xoris r3, r3, 0x8000
/* 800E4EBC 000E1DFC  90 61 00 B4 */	stw r3, 0xb4(r1)
/* 800E4EC0 000E1E00  90 01 00 B0 */	stw r0, 0xb0(r1)
/* 800E4EC4 000E1E04  C8 01 00 B0 */	lfd f0, 0xb0(r1)
/* 800E4EC8 000E1E08  EF E0 08 28 */	fsubs f31, f0, f1
/* 800E4ECC 000E1E0C  48 00 00 10 */	b lbl_800E4EDC
lbl_800E4ED0:
/* 800E4ED0 000E1E10  C3 E3 00 04 */	lfs f31, 4(r3)
/* 800E4ED4 000E1E14  48 00 00 08 */	b lbl_800E4EDC
lbl_800E4ED8:
/* 800E4ED8 000E1E18  C3 E2 8F D0 */	lfs f31, $$23832@sda21(r2)
lbl_800E4EDC:
/* 800E4EDC 000E1E1C  3B A0 00 02 */	li r29, 2
/* 800E4EE0 000E1E20  3B C0 00 01 */	li r30, 1
/* 800E4EE4 000E1E24  48 00 01 5C */	b lbl_800E5040
lbl_800E4EE8:
/* 800E4EE8 000E1E28  80 1B 00 1C */	lwz r0, 0x1c(r27)
/* 800E4EEC 000E1E2C  7C 9D E0 50 */	subf r4, r29, r28
/* 800E4EF0 000E1E30  80 BB 00 20 */	lwz r5, 0x20(r27)
/* 800E4EF4 000E1E34  38 60 00 00 */	li r3, 0
/* 800E4EF8 000E1E38  7C 04 00 50 */	subf r0, r4, r0
/* 800E4EFC 000E1E3C  54 00 18 38 */	slwi r0, r0, 3
/* 800E4F00 000E1E40  7C A5 02 14 */	add r5, r5, r0
/* 800E4F04 000E1E44  80 85 00 00 */	lwz r4, 0(r5)
/* 800E4F08 000E1E48  80 05 00 04 */	lwz r0, 4(r5)
/* 800E4F0C 000E1E4C  90 81 00 98 */	stw r4, 0x98(r1)
/* 800E4F10 000E1E50  90 01 00 9C */	stw r0, 0x9c(r1)
/* 800E4F14 000E1E54  80 01 00 98 */	lwz r0, 0x98(r1)
/* 800E4F18 000E1E58  2C 00 00 01 */	cmpwi r0, 1
/* 800E4F1C 000E1E5C  41 82 00 9C */	beq lbl_800E4FB8
/* 800E4F20 000E1E60  40 80 00 10 */	bge lbl_800E4F30
/* 800E4F24 000E1E64  2C 00 00 00 */	cmpwi r0, 0
/* 800E4F28 000E1E68  40 80 00 58 */	bge lbl_800E4F80
/* 800E4F2C 000E1E6C  48 00 00 8C */	b lbl_800E4FB8
lbl_800E4F30:
/* 800E4F30 000E1E70  2C 00 00 03 */	cmpwi r0, 3
/* 800E4F34 000E1E74  40 80 00 84 */	bge lbl_800E4FB8
/* 800E4F38 000E1E78  2C 00 00 02 */	cmpwi r0, 2
/* 800E4F3C 000E1E7C  41 82 00 08 */	beq lbl_800E4F44
/* 800E4F40 000E1E80  48 00 00 0C */	b lbl_800E4F4C
lbl_800E4F44:
/* 800E4F44 000E1E84  83 41 00 9C */	lwz r26, 0x9c(r1)
/* 800E4F48 000E1E88  48 00 00 08 */	b lbl_800E4F50
lbl_800E4F4C:
/* 800E4F4C 000E1E8C  3B 42 8F F0 */	addi r26, r2, $$24053@sda21
lbl_800E4F50:
/* 800E4F50 000E1E90  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 800E4F54 000E1E94  7F 43 D3 78 */	mr r3, r26
/* 800E4F58 000E1E98  83 24 00 04 */	lwz r25, 4(r4)
/* 800E4F5C 000E1E9C  4B F5 F7 31 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800E4F60 000E1EA0  81 99 00 00 */	lwz r12, 0(r25)
/* 800E4F64 000E1EA4  38 83 00 00 */	addi r4, r3, 0
/* 800E4F68 000E1EA8  38 79 00 00 */	addi r3, r25, 0
/* 800E4F6C 000E1EAC  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 800E4F70 000E1EB0  7F 45 D3 78 */	mr r5, r26
/* 800E4F74 000E1EB4  7D 88 03 A6 */	mtlr r12
/* 800E4F78 000E1EB8  4E 80 00 21 */	blrl 
/* 800E4F7C 000E1EBC  48 00 00 3C */	b lbl_800E4FB8
lbl_800E4F80:
/* 800E4F80 000E1EC0  2C 00 00 01 */	cmpwi r0, 1
/* 800E4F84 000E1EC4  41 82 00 1C */	beq lbl_800E4FA0
/* 800E4F88 000E1EC8  40 80 00 2C */	bge lbl_800E4FB4
/* 800E4F8C 000E1ECC  2C 00 00 00 */	cmpwi r0, 0
/* 800E4F90 000E1ED0  40 80 00 08 */	bge lbl_800E4F98
/* 800E4F94 000E1ED4  48 00 00 20 */	b lbl_800E4FB4
lbl_800E4F98:
/* 800E4F98 000E1ED8  80 61 00 9C */	lwz r3, 0x9c(r1)
/* 800E4F9C 000E1EDC  48 00 00 1C */	b lbl_800E4FB8
lbl_800E4FA0:
/* 800E4FA0 000E1EE0  C0 01 00 9C */	lfs f0, 0x9c(r1)
/* 800E4FA4 000E1EE4  FC 00 00 1E */	fctiwz f0, f0
/* 800E4FA8 000E1EE8  D8 01 00 B0 */	stfd f0, 0xb0(r1)
/* 800E4FAC 000E1EEC  80 61 00 B4 */	lwz r3, 0xb4(r1)
/* 800E4FB0 000E1EF0  48 00 00 08 */	b lbl_800E4FB8
lbl_800E4FB4:
/* 800E4FB4 000E1EF4  38 60 00 00 */	li r3, 0
lbl_800E4FB8:
/* 800E4FB8 000E1EF8  28 03 00 00 */	cmplwi r3, 0
/* 800E4FBC 000E1EFC  41 82 00 80 */	beq lbl_800E503C
/* 800E4FC0 000E1F00  80 9F 00 10 */	lwz r4, 0x10(r31)
/* 800E4FC4 000E1F04  80 1F 00 14 */	lwz r0, 0x14(r31)
/* 800E4FC8 000E1F08  90 81 00 8C */	stw r4, 0x8c(r1)
/* 800E4FCC 000E1F0C  90 01 00 90 */	stw r0, 0x90(r1)
/* 800E4FD0 000E1F10  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800E4FD4 000E1F14  90 01 00 94 */	stw r0, 0x94(r1)
/* 800E4FD8 000E1F18  C0 21 00 8C */	lfs f1, 0x8c(r1)
/* 800E4FDC 000E1F1C  C0 03 00 10 */	lfs f0, 0x10(r3)
/* 800E4FE0 000E1F20  EC 01 00 28 */	fsubs f0, f1, f0
/* 800E4FE4 000E1F24  D0 01 00 8C */	stfs f0, 0x8c(r1)
/* 800E4FE8 000E1F28  C0 21 00 90 */	lfs f1, 0x90(r1)
/* 800E4FEC 000E1F2C  C0 03 00 14 */	lfs f0, 0x14(r3)
/* 800E4FF0 000E1F30  EC 01 00 28 */	fsubs f0, f1, f0
/* 800E4FF4 000E1F34  D0 01 00 90 */	stfs f0, 0x90(r1)
/* 800E4FF8 000E1F38  C0 21 00 94 */	lfs f1, 0x94(r1)
/* 800E4FFC 000E1F3C  C0 03 00 18 */	lfs f0, 0x18(r3)
/* 800E5000 000E1F40  EC 01 00 28 */	fsubs f0, f1, f0
/* 800E5004 000E1F44  D0 01 00 94 */	stfs f0, 0x94(r1)
/* 800E5008 000E1F48  C0 21 00 8C */	lfs f1, 0x8c(r1)
/* 800E500C 000E1F4C  C0 01 00 90 */	lfs f0, 0x90(r1)
/* 800E5010 000E1F50  C0 41 00 94 */	lfs f2, 0x94(r1)
/* 800E5014 000E1F54  EC 21 00 72 */	fmuls f1, f1, f1
/* 800E5018 000E1F58  EC 00 00 32 */	fmuls f0, f0, f0
/* 800E501C 000E1F5C  EC 42 00 B2 */	fmuls f2, f2, f2
/* 800E5020 000E1F60  EC 01 00 2A */	fadds f0, f1, f0
/* 800E5024 000E1F64  EC 22 00 2A */	fadds f1, f2, f0
/* 800E5028 000E1F68  4B FE B8 89 */	bl sqrt__Q29JGeometry8TUtil$$0f$$1Ff
/* 800E502C 000E1F6C  FC 01 F8 40 */	fcmpo cr0, f1, f31
/* 800E5030 000E1F70  4C 40 13 82 */	cror 2, 0, 2
/* 800E5034 000E1F74  40 82 00 08 */	bne lbl_800E503C
/* 800E5038 000E1F78  3B DE 00 01 */	addi r30, r30, 1
lbl_800E503C:
/* 800E503C 000E1F7C  3B BD 00 01 */	addi r29, r29, 1
lbl_800E5040:
/* 800E5040 000E1F80  7C 1D E0 40 */	cmplw r29, r28
/* 800E5044 000E1F84  41 80 FE A4 */	blt lbl_800E4EE8
lbl_800E5048:
/* 800E5048 000E1F88  3C 60 80 37 */	lis r3, $$23829@ha
/* 800E504C 000E1F8C  3B A3 30 FC */	addi r29, r3, $$23829@l
/* 800E5050 000E1F90  3B 20 00 00 */	li r25, 0
/* 800E5054 000E1F94  48 00 00 2C */	b lbl_800E5080
lbl_800E5058:
/* 800E5058 000E1F98  80 7B 00 1C */	lwz r3, 0x1c(r27)
/* 800E505C 000E1F9C  2C 03 00 00 */	cmpwi r3, 0
/* 800E5060 000E1FA0  41 81 00 14 */	bgt lbl_800E5074
/* 800E5064 000E1FA4  38 7D 00 00 */	addi r3, r29, 0
/* 800E5068 000E1FA8  4C C6 31 82 */	crclr 6
/* 800E506C 000E1FAC  4B FF 80 39 */	bl SpcTrace__FPCce
/* 800E5070 000E1FB0  48 00 00 0C */	b lbl_800E507C
lbl_800E5074:
/* 800E5074 000E1FB4  38 03 FF FF */	addi r0, r3, -1
/* 800E5078 000E1FB8  90 1B 00 1C */	stw r0, 0x1c(r27)
lbl_800E507C:
/* 800E507C 000E1FBC  3B 39 00 01 */	addi r25, r25, 1
lbl_800E5080:
/* 800E5080 000E1FC0  7C 19 E0 00 */	cmpw r25, r28
/* 800E5084 000E1FC4  41 80 FF D4 */	blt lbl_800E5058
/* 800E5088 000E1FC8  38 00 00 00 */	li r0, 0
/* 800E508C 000E1FCC  90 01 00 68 */	stw r0, 0x68(r1)
/* 800E5090 000E1FD0  93 C1 00 6C */	stw r30, 0x6c(r1)
/* 800E5094 000E1FD4  80 7B 00 1C */	lwz r3, 0x1c(r27)
/* 800E5098 000E1FD8  80 1B 00 18 */	lwz r0, 0x18(r27)
/* 800E509C 000E1FDC  7C 03 00 00 */	cmpw r3, r0
/* 800E50A0 000E1FE0  41 80 00 18 */	blt lbl_800E50B8
/* 800E50A4 000E1FE4  3C 60 80 37 */	lis r3, $$23834@ha
/* 800E50A8 000E1FE8  4C C6 31 82 */	crclr 6
/* 800E50AC 000E1FEC  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800E50B0 000E1FF0  4B FF 7F F5 */	bl SpcTrace__FPCce
/* 800E50B4 000E1FF4  48 00 00 2C */	b lbl_800E50E0
lbl_800E50B8:
/* 800E50B8 000E1FF8  80 BB 00 20 */	lwz r5, 0x20(r27)
/* 800E50BC 000E1FFC  54 64 18 38 */	slwi r4, r3, 3
/* 800E50C0 000E2000  80 61 00 68 */	lwz r3, 0x68(r1)
/* 800E50C4 000E2004  80 01 00 6C */	lwz r0, 0x6c(r1)
/* 800E50C8 000E2008  7C 85 22 14 */	add r4, r5, r4
/* 800E50CC 000E200C  90 64 00 00 */	stw r3, 0(r4)
/* 800E50D0 000E2010  90 04 00 04 */	stw r0, 4(r4)
/* 800E50D4 000E2014  80 7B 00 1C */	lwz r3, 0x1c(r27)
/* 800E50D8 000E2018  38 03 00 01 */	addi r0, r3, 1
/* 800E50DC 000E201C  90 1B 00 1C */	stw r0, 0x1c(r27)
lbl_800E50E0:
/* 800E50E0 000E2020  BB 21 00 BC */	lmw r25, 0xbc(r1)
/* 800E50E4 000E2024  80 01 00 E4 */	lwz r0, 0xe4(r1)
/* 800E50E8 000E2028  CB E1 00 D8 */	lfd f31, 0xd8(r1)
/* 800E50EC 000E202C  38 21 00 E0 */	addi r1, r1, 0xe0
/* 800E50F0 000E2030  7C 08 03 A6 */	mtlr r0
/* 800E50F4 000E2034  4E 80 00 20 */	blr 

.global evIsNearSameActors__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evIsNearSameActors__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E50F8 000E2038  7C 08 02 A6 */	mflr r0
/* 800E50FC 000E203C  90 01 00 04 */	stw r0, 4(r1)
/* 800E5100 000E2040  94 21 FE B0 */	stwu r1, -0x150(r1)
/* 800E5104 000E2044  DB E1 01 48 */	stfd f31, 0x148(r1)
/* 800E5108 000E2048  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E510C 000E204C  BF 01 01 28 */	stmw r24, 0x128(r1)
/* 800E5110 000E2050  7C 7D 1B 78 */	mr r29, r3
/* 800E5114 000E2054  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E5118 000E2058  3C 80 80 37 */	lis r4, $$21490@ha
/* 800E511C 000E205C  3B E4 29 90 */	addi r31, r4, $$21490@l
/* 800E5120 000E2060  38 80 00 03 */	li r4, 3
/* 800E5124 000E2064  48 03 23 0D */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E5128 000E2068  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E512C 000E206C  2C 03 00 00 */	cmpwi r3, 0
/* 800E5130 000E2070  41 81 00 34 */	bgt lbl_800E5164
/* 800E5134 000E2074  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800E5138 000E2078  4C C6 31 82 */	crclr 6
/* 800E513C 000E207C  4B FF 7F 69 */	bl SpcTrace__FPCce
/* 800E5140 000E2080  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E5144 000E2084  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E5148 000E2088  54 00 18 38 */	slwi r0, r0, 3
/* 800E514C 000E208C  7C 63 02 14 */	add r3, r3, r0
/* 800E5150 000E2090  80 03 00 00 */	lwz r0, 0(r3)
/* 800E5154 000E2094  90 01 00 FC */	stw r0, 0xfc(r1)
/* 800E5158 000E2098  80 03 00 04 */	lwz r0, 4(r3)
/* 800E515C 000E209C  90 01 01 00 */	stw r0, 0x100(r1)
/* 800E5160 000E20A0  48 00 00 2C */	b lbl_800E518C
lbl_800E5164:
/* 800E5164 000E20A4  38 03 FF FF */	addi r0, r3, -1
/* 800E5168 000E20A8  90 1D 00 1C */	stw r0, 0x1c(r29)
/* 800E516C 000E20AC  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E5170 000E20B0  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E5174 000E20B4  54 00 18 38 */	slwi r0, r0, 3
/* 800E5178 000E20B8  7C 63 02 14 */	add r3, r3, r0
/* 800E517C 000E20BC  80 03 00 00 */	lwz r0, 0(r3)
/* 800E5180 000E20C0  90 01 00 FC */	stw r0, 0xfc(r1)
/* 800E5184 000E20C4  80 03 00 04 */	lwz r0, 4(r3)
/* 800E5188 000E20C8  90 01 01 00 */	stw r0, 0x100(r1)
lbl_800E518C:
/* 800E518C 000E20CC  80 81 00 FC */	lwz r4, 0xfc(r1)
/* 800E5190 000E20D0  38 60 00 00 */	li r3, 0
/* 800E5194 000E20D4  80 01 01 00 */	lwz r0, 0x100(r1)
/* 800E5198 000E20D8  90 81 01 04 */	stw r4, 0x104(r1)
/* 800E519C 000E20DC  90 01 01 08 */	stw r0, 0x108(r1)
/* 800E51A0 000E20E0  80 01 01 04 */	lwz r0, 0x104(r1)
/* 800E51A4 000E20E4  2C 00 00 01 */	cmpwi r0, 1
/* 800E51A8 000E20E8  41 82 00 9C */	beq lbl_800E5244
/* 800E51AC 000E20EC  40 80 00 10 */	bge lbl_800E51BC
/* 800E51B0 000E20F0  2C 00 00 00 */	cmpwi r0, 0
/* 800E51B4 000E20F4  40 80 00 58 */	bge lbl_800E520C
/* 800E51B8 000E20F8  48 00 00 8C */	b lbl_800E5244
lbl_800E51BC:
/* 800E51BC 000E20FC  2C 00 00 03 */	cmpwi r0, 3
/* 800E51C0 000E2100  40 80 00 84 */	bge lbl_800E5244
/* 800E51C4 000E2104  2C 00 00 02 */	cmpwi r0, 2
/* 800E51C8 000E2108  41 82 00 08 */	beq lbl_800E51D0
/* 800E51CC 000E210C  48 00 00 0C */	b lbl_800E51D8
lbl_800E51D0:
/* 800E51D0 000E2110  83 81 01 08 */	lwz r28, 0x108(r1)
/* 800E51D4 000E2114  48 00 00 08 */	b lbl_800E51DC
lbl_800E51D8:
/* 800E51D8 000E2118  3B 82 8F F0 */	addi r28, r2, $$24053@sda21
lbl_800E51DC:
/* 800E51DC 000E211C  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 800E51E0 000E2120  7F 83 E3 78 */	mr r3, r28
/* 800E51E4 000E2124  83 64 00 04 */	lwz r27, 4(r4)
/* 800E51E8 000E2128  4B F5 F4 A5 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800E51EC 000E212C  81 9B 00 00 */	lwz r12, 0(r27)
/* 800E51F0 000E2130  38 83 00 00 */	addi r4, r3, 0
/* 800E51F4 000E2134  38 7B 00 00 */	addi r3, r27, 0
/* 800E51F8 000E2138  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 800E51FC 000E213C  7F 85 E3 78 */	mr r5, r28
/* 800E5200 000E2140  7D 88 03 A6 */	mtlr r12
/* 800E5204 000E2144  4E 80 00 21 */	blrl 
/* 800E5208 000E2148  48 00 00 3C */	b lbl_800E5244
lbl_800E520C:
/* 800E520C 000E214C  2C 00 00 01 */	cmpwi r0, 1
/* 800E5210 000E2150  41 82 00 1C */	beq lbl_800E522C
/* 800E5214 000E2154  40 80 00 2C */	bge lbl_800E5240
/* 800E5218 000E2158  2C 00 00 00 */	cmpwi r0, 0
/* 800E521C 000E215C  40 80 00 08 */	bge lbl_800E5224
/* 800E5220 000E2160  48 00 00 20 */	b lbl_800E5240
lbl_800E5224:
/* 800E5224 000E2164  80 61 01 08 */	lwz r3, 0x108(r1)
/* 800E5228 000E2168  48 00 00 1C */	b lbl_800E5244
lbl_800E522C:
/* 800E522C 000E216C  C0 01 01 08 */	lfs f0, 0x108(r1)
/* 800E5230 000E2170  FC 00 00 1E */	fctiwz f0, f0
/* 800E5234 000E2174  D8 01 01 20 */	stfd f0, 0x120(r1)
/* 800E5238 000E2178  80 61 01 24 */	lwz r3, 0x124(r1)
/* 800E523C 000E217C  48 00 00 08 */	b lbl_800E5244
lbl_800E5240:
/* 800E5240 000E2180  38 60 00 00 */	li r3, 0
lbl_800E5244:
/* 800E5244 000E2184  28 03 00 00 */	cmplwi r3, 0
/* 800E5248 000E2188  40 82 00 5C */	bne lbl_800E52A4
/* 800E524C 000E218C  38 00 00 00 */	li r0, 0
/* 800E5250 000E2190  90 01 00 C8 */	stw r0, 0xc8(r1)
/* 800E5254 000E2194  90 01 00 CC */	stw r0, 0xcc(r1)
/* 800E5258 000E2198  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E525C 000E219C  80 1D 00 18 */	lwz r0, 0x18(r29)
/* 800E5260 000E21A0  7C 03 00 00 */	cmpw r3, r0
/* 800E5264 000E21A4  41 80 00 14 */	blt lbl_800E5278
/* 800E5268 000E21A8  38 7F 07 8C */	addi r3, r31, 0x78c
/* 800E526C 000E21AC  4C C6 31 82 */	crclr 6
/* 800E5270 000E21B0  4B FF 7E 35 */	bl SpcTrace__FPCce
/* 800E5274 000E21B4  48 00 03 74 */	b lbl_800E55E8
lbl_800E5278:
/* 800E5278 000E21B8  80 BD 00 20 */	lwz r5, 0x20(r29)
/* 800E527C 000E21BC  54 64 18 38 */	slwi r4, r3, 3
/* 800E5280 000E21C0  80 61 00 C8 */	lwz r3, 0xc8(r1)
/* 800E5284 000E21C4  80 01 00 CC */	lwz r0, 0xcc(r1)
/* 800E5288 000E21C8  7C 85 22 14 */	add r4, r5, r4
/* 800E528C 000E21CC  90 64 00 00 */	stw r3, 0(r4)
/* 800E5290 000E21D0  90 04 00 04 */	stw r0, 4(r4)
/* 800E5294 000E21D4  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E5298 000E21D8  38 03 00 01 */	addi r0, r3, 1
/* 800E529C 000E21DC  90 1D 00 1C */	stw r0, 0x1c(r29)
/* 800E52A0 000E21E0  48 00 03 48 */	b lbl_800E55E8
lbl_800E52A4:
/* 800E52A4 000E21E4  80 9D 00 1C */	lwz r4, 0x1c(r29)
/* 800E52A8 000E21E8  83 C3 00 4C */	lwz r30, 0x4c(r3)
/* 800E52AC 000E21EC  2C 04 00 00 */	cmpwi r4, 0
/* 800E52B0 000E21F0  41 81 00 34 */	bgt lbl_800E52E4
/* 800E52B4 000E21F4  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800E52B8 000E21F8  4C C6 31 82 */	crclr 6
/* 800E52BC 000E21FC  4B FF 7D E9 */	bl SpcTrace__FPCce
/* 800E52C0 000E2200  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E52C4 000E2204  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E52C8 000E2208  54 00 18 38 */	slwi r0, r0, 3
/* 800E52CC 000E220C  7C 63 02 14 */	add r3, r3, r0
/* 800E52D0 000E2210  80 03 00 00 */	lwz r0, 0(r3)
/* 800E52D4 000E2214  90 01 00 F4 */	stw r0, 0xf4(r1)
/* 800E52D8 000E2218  80 03 00 04 */	lwz r0, 4(r3)
/* 800E52DC 000E221C  90 01 00 F8 */	stw r0, 0xf8(r1)
/* 800E52E0 000E2220  48 00 00 2C */	b lbl_800E530C
lbl_800E52E4:
/* 800E52E4 000E2224  38 04 FF FF */	addi r0, r4, -1
/* 800E52E8 000E2228  90 1D 00 1C */	stw r0, 0x1c(r29)
/* 800E52EC 000E222C  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E52F0 000E2230  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E52F4 000E2234  54 00 18 38 */	slwi r0, r0, 3
/* 800E52F8 000E2238  7C 63 02 14 */	add r3, r3, r0
/* 800E52FC 000E223C  80 03 00 00 */	lwz r0, 0(r3)
/* 800E5300 000E2240  90 01 00 F4 */	stw r0, 0xf4(r1)
/* 800E5304 000E2244  80 03 00 04 */	lwz r0, 4(r3)
/* 800E5308 000E2248  90 01 00 F8 */	stw r0, 0xf8(r1)
lbl_800E530C:
/* 800E530C 000E224C  80 01 00 F4 */	lwz r0, 0xf4(r1)
/* 800E5310 000E2250  2C 00 00 01 */	cmpwi r0, 1
/* 800E5314 000E2254  41 82 00 38 */	beq lbl_800E534C
/* 800E5318 000E2258  40 80 00 3C */	bge lbl_800E5354
/* 800E531C 000E225C  2C 00 00 00 */	cmpwi r0, 0
/* 800E5320 000E2260  40 80 00 08 */	bge lbl_800E5328
/* 800E5324 000E2264  48 00 00 30 */	b lbl_800E5354
lbl_800E5328:
/* 800E5328 000E2268  80 61 00 F8 */	lwz r3, 0xf8(r1)
/* 800E532C 000E226C  3C 00 43 30 */	lis r0, 0x4330
/* 800E5330 000E2270  C8 22 8F E0 */	lfd f1, $$23934@sda21(r2)
/* 800E5334 000E2274  6C 63 80 00 */	xoris r3, r3, 0x8000
/* 800E5338 000E2278  90 61 01 24 */	stw r3, 0x124(r1)
/* 800E533C 000E227C  90 01 01 20 */	stw r0, 0x120(r1)
/* 800E5340 000E2280  C8 01 01 20 */	lfd f0, 0x120(r1)
/* 800E5344 000E2284  EF E0 08 28 */	fsubs f31, f0, f1
/* 800E5348 000E2288  48 00 00 10 */	b lbl_800E5358
lbl_800E534C:
/* 800E534C 000E228C  C3 E1 00 F8 */	lfs f31, 0xf8(r1)
/* 800E5350 000E2290  48 00 00 08 */	b lbl_800E5358
lbl_800E5354:
/* 800E5354 000E2294  C3 E2 8F D0 */	lfs f31, $$23832@sda21(r2)
lbl_800E5358:
/* 800E5358 000E2298  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E535C 000E229C  2C 03 00 00 */	cmpwi r3, 0
/* 800E5360 000E22A0  41 81 00 34 */	bgt lbl_800E5394
/* 800E5364 000E22A4  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800E5368 000E22A8  4C C6 31 82 */	crclr 6
/* 800E536C 000E22AC  4B FF 7D 39 */	bl SpcTrace__FPCce
/* 800E5370 000E22B0  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E5374 000E22B4  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E5378 000E22B8  54 00 18 38 */	slwi r0, r0, 3
/* 800E537C 000E22BC  7C 63 02 14 */	add r3, r3, r0
/* 800E5380 000E22C0  80 03 00 00 */	lwz r0, 0(r3)
/* 800E5384 000E22C4  90 01 00 E4 */	stw r0, 0xe4(r1)
/* 800E5388 000E22C8  80 03 00 04 */	lwz r0, 4(r3)
/* 800E538C 000E22CC  90 01 00 E8 */	stw r0, 0xe8(r1)
/* 800E5390 000E22D0  48 00 00 2C */	b lbl_800E53BC
lbl_800E5394:
/* 800E5394 000E22D4  38 03 FF FF */	addi r0, r3, -1
/* 800E5398 000E22D8  90 1D 00 1C */	stw r0, 0x1c(r29)
/* 800E539C 000E22DC  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E53A0 000E22E0  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E53A4 000E22E4  54 00 18 38 */	slwi r0, r0, 3
/* 800E53A8 000E22E8  7C 63 02 14 */	add r3, r3, r0
/* 800E53AC 000E22EC  80 03 00 00 */	lwz r0, 0(r3)
/* 800E53B0 000E22F0  90 01 00 E4 */	stw r0, 0xe4(r1)
/* 800E53B4 000E22F4  80 03 00 04 */	lwz r0, 4(r3)
/* 800E53B8 000E22F8  90 01 00 E8 */	stw r0, 0xe8(r1)
lbl_800E53BC:
/* 800E53BC 000E22FC  80 61 00 E4 */	lwz r3, 0xe4(r1)
/* 800E53C0 000E2300  3B 60 00 00 */	li r27, 0
/* 800E53C4 000E2304  80 01 00 E8 */	lwz r0, 0xe8(r1)
/* 800E53C8 000E2308  90 61 00 EC */	stw r3, 0xec(r1)
/* 800E53CC 000E230C  90 01 00 F0 */	stw r0, 0xf0(r1)
/* 800E53D0 000E2310  80 01 00 EC */	lwz r0, 0xec(r1)
/* 800E53D4 000E2314  2C 00 00 01 */	cmpwi r0, 1
/* 800E53D8 000E2318  41 82 00 A0 */	beq lbl_800E5478
/* 800E53DC 000E231C  40 80 00 10 */	bge lbl_800E53EC
/* 800E53E0 000E2320  2C 00 00 00 */	cmpwi r0, 0
/* 800E53E4 000E2324  40 80 00 5C */	bge lbl_800E5440
/* 800E53E8 000E2328  48 00 00 90 */	b lbl_800E5478
lbl_800E53EC:
/* 800E53EC 000E232C  2C 00 00 03 */	cmpwi r0, 3
/* 800E53F0 000E2330  40 80 00 88 */	bge lbl_800E5478
/* 800E53F4 000E2334  2C 00 00 02 */	cmpwi r0, 2
/* 800E53F8 000E2338  41 82 00 08 */	beq lbl_800E5400
/* 800E53FC 000E233C  48 00 00 0C */	b lbl_800E5408
lbl_800E5400:
/* 800E5400 000E2340  83 81 00 F0 */	lwz r28, 0xf0(r1)
/* 800E5404 000E2344  48 00 00 08 */	b lbl_800E540C
lbl_800E5408:
/* 800E5408 000E2348  3B 82 8F F0 */	addi r28, r2, $$24053@sda21
lbl_800E540C:
/* 800E540C 000E234C  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 800E5410 000E2350  7F 83 E3 78 */	mr r3, r28
/* 800E5414 000E2354  83 64 00 04 */	lwz r27, 4(r4)
/* 800E5418 000E2358  4B F5 F2 75 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800E541C 000E235C  81 9B 00 00 */	lwz r12, 0(r27)
/* 800E5420 000E2360  38 83 00 00 */	addi r4, r3, 0
/* 800E5424 000E2364  38 7B 00 00 */	addi r3, r27, 0
/* 800E5428 000E2368  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 800E542C 000E236C  7F 85 E3 78 */	mr r5, r28
/* 800E5430 000E2370  7D 88 03 A6 */	mtlr r12
/* 800E5434 000E2374  4E 80 00 21 */	blrl 
/* 800E5438 000E2378  7C 7B 1B 78 */	mr r27, r3
/* 800E543C 000E237C  48 00 00 3C */	b lbl_800E5478
lbl_800E5440:
/* 800E5440 000E2380  2C 00 00 01 */	cmpwi r0, 1
/* 800E5444 000E2384  41 82 00 1C */	beq lbl_800E5460
/* 800E5448 000E2388  40 80 00 2C */	bge lbl_800E5474
/* 800E544C 000E238C  2C 00 00 00 */	cmpwi r0, 0
/* 800E5450 000E2390  40 80 00 08 */	bge lbl_800E5458
/* 800E5454 000E2394  48 00 00 20 */	b lbl_800E5474
lbl_800E5458:
/* 800E5458 000E2398  83 61 00 F0 */	lwz r27, 0xf0(r1)
/* 800E545C 000E239C  48 00 00 1C */	b lbl_800E5478
lbl_800E5460:
/* 800E5460 000E23A0  C0 01 00 F0 */	lfs f0, 0xf0(r1)
/* 800E5464 000E23A4  FC 00 00 1E */	fctiwz f0, f0
/* 800E5468 000E23A8  D8 01 01 20 */	stfd f0, 0x120(r1)
/* 800E546C 000E23AC  83 61 01 24 */	lwz r27, 0x124(r1)
/* 800E5470 000E23B0  48 00 00 08 */	b lbl_800E5478
lbl_800E5474:
/* 800E5474 000E23B4  3B 60 00 00 */	li r27, 0
lbl_800E5478:
/* 800E5478 000E23B8  28 1B 00 00 */	cmplwi r27, 0
/* 800E547C 000E23BC  40 82 00 5C */	bne lbl_800E54D8
/* 800E5480 000E23C0  38 00 00 00 */	li r0, 0
/* 800E5484 000E23C4  90 01 00 B4 */	stw r0, 0xb4(r1)
/* 800E5488 000E23C8  90 01 00 B8 */	stw r0, 0xb8(r1)
/* 800E548C 000E23CC  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E5490 000E23D0  80 1D 00 18 */	lwz r0, 0x18(r29)
/* 800E5494 000E23D4  7C 03 00 00 */	cmpw r3, r0
/* 800E5498 000E23D8  41 80 00 14 */	blt lbl_800E54AC
/* 800E549C 000E23DC  38 7F 07 8C */	addi r3, r31, 0x78c
/* 800E54A0 000E23E0  4C C6 31 82 */	crclr 6
/* 800E54A4 000E23E4  4B FF 7C 01 */	bl SpcTrace__FPCce
/* 800E54A8 000E23E8  48 00 01 40 */	b lbl_800E55E8
lbl_800E54AC:
/* 800E54AC 000E23EC  80 BD 00 20 */	lwz r5, 0x20(r29)
/* 800E54B0 000E23F0  54 64 18 38 */	slwi r4, r3, 3
/* 800E54B4 000E23F4  80 61 00 B4 */	lwz r3, 0xb4(r1)
/* 800E54B8 000E23F8  80 01 00 B8 */	lwz r0, 0xb8(r1)
/* 800E54BC 000E23FC  7C 85 22 14 */	add r4, r5, r4
/* 800E54C0 000E2400  90 64 00 00 */	stw r3, 0(r4)
/* 800E54C4 000E2404  90 04 00 04 */	stw r0, 4(r4)
/* 800E54C8 000E2408  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E54CC 000E240C  38 03 00 01 */	addi r0, r3, 1
/* 800E54D0 000E2410  90 1D 00 1C */	stw r0, 0x1c(r29)
/* 800E54D4 000E2414  48 00 01 14 */	b lbl_800E55E8
lbl_800E54D8:
/* 800E54D8 000E2418  80 6D 9A 10 */	lwz r3, gpMapObjManager@sda21(r13)
/* 800E54DC 000E241C  3B 20 00 00 */	li r25, 0
/* 800E54E0 000E2420  3B 00 00 00 */	li r24, 0
/* 800E54E4 000E2424  83 43 00 14 */	lwz r26, 0x14(r3)
/* 800E54E8 000E2428  3B 80 00 00 */	li r28, 0
/* 800E54EC 000E242C  48 00 00 A0 */	b lbl_800E558C
lbl_800E54F0:
/* 800E54F0 000E2430  80 6D 9A 10 */	lwz r3, gpMapObjManager@sda21(r13)
/* 800E54F4 000E2434  80 63 00 18 */	lwz r3, 0x18(r3)
/* 800E54F8 000E2438  7C 83 E0 2E */	lwzx r4, r3, r28
/* 800E54FC 000E243C  80 04 00 4C */	lwz r0, 0x4c(r4)
/* 800E5500 000E2440  7C 1E 00 40 */	cmplw r30, r0
/* 800E5504 000E2444  40 82 00 80 */	bne lbl_800E5584
/* 800E5508 000E2448  80 7B 00 10 */	lwz r3, 0x10(r27)
/* 800E550C 000E244C  80 1B 00 14 */	lwz r0, 0x14(r27)
/* 800E5510 000E2450  90 61 00 D8 */	stw r3, 0xd8(r1)
/* 800E5514 000E2454  90 01 00 DC */	stw r0, 0xdc(r1)
/* 800E5518 000E2458  80 1B 00 18 */	lwz r0, 0x18(r27)
/* 800E551C 000E245C  90 01 00 E0 */	stw r0, 0xe0(r1)
/* 800E5520 000E2460  C0 21 00 D8 */	lfs f1, 0xd8(r1)
/* 800E5524 000E2464  C0 04 00 10 */	lfs f0, 0x10(r4)
/* 800E5528 000E2468  EC 01 00 28 */	fsubs f0, f1, f0
/* 800E552C 000E246C  D0 01 00 D8 */	stfs f0, 0xd8(r1)
/* 800E5530 000E2470  C0 21 00 DC */	lfs f1, 0xdc(r1)
/* 800E5534 000E2474  C0 04 00 14 */	lfs f0, 0x14(r4)
/* 800E5538 000E2478  EC 01 00 28 */	fsubs f0, f1, f0
/* 800E553C 000E247C  D0 01 00 DC */	stfs f0, 0xdc(r1)
/* 800E5540 000E2480  C0 21 00 E0 */	lfs f1, 0xe0(r1)
/* 800E5544 000E2484  C0 04 00 18 */	lfs f0, 0x18(r4)
/* 800E5548 000E2488  EC 01 00 28 */	fsubs f0, f1, f0
/* 800E554C 000E248C  D0 01 00 E0 */	stfs f0, 0xe0(r1)
/* 800E5550 000E2490  C0 21 00 D8 */	lfs f1, 0xd8(r1)
/* 800E5554 000E2494  C0 01 00 DC */	lfs f0, 0xdc(r1)
/* 800E5558 000E2498  C0 41 00 E0 */	lfs f2, 0xe0(r1)
/* 800E555C 000E249C  EC 21 00 72 */	fmuls f1, f1, f1
/* 800E5560 000E24A0  EC 00 00 32 */	fmuls f0, f0, f0
/* 800E5564 000E24A4  EC 42 00 B2 */	fmuls f2, f2, f2
/* 800E5568 000E24A8  EC 01 00 2A */	fadds f0, f1, f0
/* 800E556C 000E24AC  EC 22 00 2A */	fadds f1, f2, f0
/* 800E5570 000E24B0  4B FE B3 41 */	bl sqrt__Q29JGeometry8TUtil$$0f$$1Ff
/* 800E5574 000E24B4  FC 01 F8 40 */	fcmpo cr0, f1, f31
/* 800E5578 000E24B8  4C 40 13 82 */	cror 2, 0, 2
/* 800E557C 000E24BC  40 82 00 08 */	bne lbl_800E5584
/* 800E5580 000E24C0  3B 39 00 01 */	addi r25, r25, 1
lbl_800E5584:
/* 800E5584 000E24C4  3B 18 00 01 */	addi r24, r24, 1
/* 800E5588 000E24C8  3B 9C 00 04 */	addi r28, r28, 4
lbl_800E558C:
/* 800E558C 000E24CC  7C 18 D0 00 */	cmpw r24, r26
/* 800E5590 000E24D0  41 80 FF 60 */	blt lbl_800E54F0
/* 800E5594 000E24D4  38 00 00 00 */	li r0, 0
/* 800E5598 000E24D8  90 01 00 A4 */	stw r0, 0xa4(r1)
/* 800E559C 000E24DC  93 21 00 A8 */	stw r25, 0xa8(r1)
/* 800E55A0 000E24E0  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E55A4 000E24E4  80 1D 00 18 */	lwz r0, 0x18(r29)
/* 800E55A8 000E24E8  7C 03 00 00 */	cmpw r3, r0
/* 800E55AC 000E24EC  41 80 00 14 */	blt lbl_800E55C0
/* 800E55B0 000E24F0  38 7F 07 8C */	addi r3, r31, 0x78c
/* 800E55B4 000E24F4  4C C6 31 82 */	crclr 6
/* 800E55B8 000E24F8  4B FF 7A ED */	bl SpcTrace__FPCce
/* 800E55BC 000E24FC  48 00 00 2C */	b lbl_800E55E8
lbl_800E55C0:
/* 800E55C0 000E2500  80 BD 00 20 */	lwz r5, 0x20(r29)
/* 800E55C4 000E2504  54 64 18 38 */	slwi r4, r3, 3
/* 800E55C8 000E2508  80 61 00 A4 */	lwz r3, 0xa4(r1)
/* 800E55CC 000E250C  80 01 00 A8 */	lwz r0, 0xa8(r1)
/* 800E55D0 000E2510  7C 85 22 14 */	add r4, r5, r4
/* 800E55D4 000E2514  90 64 00 00 */	stw r3, 0(r4)
/* 800E55D8 000E2518  90 04 00 04 */	stw r0, 4(r4)
/* 800E55DC 000E251C  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E55E0 000E2520  38 03 00 01 */	addi r0, r3, 1
/* 800E55E4 000E2524  90 1D 00 1C */	stw r0, 0x1c(r29)
lbl_800E55E8:
/* 800E55E8 000E2528  BB 01 01 28 */	lmw r24, 0x128(r1)
/* 800E55EC 000E252C  80 01 01 54 */	lwz r0, 0x154(r1)
/* 800E55F0 000E2530  CB E1 01 48 */	lfd f31, 0x148(r1)
/* 800E55F4 000E2534  38 21 01 50 */	addi r1, r1, 0x150
/* 800E55F8 000E2538  7C 08 03 A6 */	mtlr r0
/* 800E55FC 000E253C  4E 80 00 20 */	blr 

.global evSetFlagNPCDead__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evSetFlagNPCDead__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E5600 000E2540  7C 08 02 A6 */	mflr r0
/* 800E5604 000E2544  90 01 00 04 */	stw r0, 4(r1)
/* 800E5608 000E2548  94 21 FF 58 */	stwu r1, -0xa8(r1)
/* 800E560C 000E254C  BF 61 00 94 */	stmw r27, 0x94(r1)
/* 800E5610 000E2550  7C 7D 1B 78 */	mr r29, r3
/* 800E5614 000E2554  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E5618 000E2558  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E561C 000E255C  3C 80 80 37 */	lis r4, $$21490@ha
/* 800E5620 000E2560  3B E4 29 90 */	addi r31, r4, $$21490@l
/* 800E5624 000E2564  38 80 00 02 */	li r4, 2
/* 800E5628 000E2568  48 03 1E 09 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E562C 000E256C  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E5630 000E2570  2C 03 00 00 */	cmpwi r3, 0
/* 800E5634 000E2574  41 81 00 34 */	bgt lbl_800E5668
/* 800E5638 000E2578  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800E563C 000E257C  4C C6 31 82 */	crclr 6
/* 800E5640 000E2580  4B FF 7A 65 */	bl SpcTrace__FPCce
/* 800E5644 000E2584  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E5648 000E2588  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E564C 000E258C  54 00 18 38 */	slwi r0, r0, 3
/* 800E5650 000E2590  7C 63 02 14 */	add r3, r3, r0
/* 800E5654 000E2594  80 03 00 00 */	lwz r0, 0(r3)
/* 800E5658 000E2598  90 01 00 7C */	stw r0, 0x7c(r1)
/* 800E565C 000E259C  80 03 00 04 */	lwz r0, 4(r3)
/* 800E5660 000E25A0  90 01 00 80 */	stw r0, 0x80(r1)
/* 800E5664 000E25A4  48 00 00 2C */	b lbl_800E5690
lbl_800E5668:
/* 800E5668 000E25A8  38 03 FF FF */	addi r0, r3, -1
/* 800E566C 000E25AC  90 1D 00 1C */	stw r0, 0x1c(r29)
/* 800E5670 000E25B0  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E5674 000E25B4  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E5678 000E25B8  54 00 18 38 */	slwi r0, r0, 3
/* 800E567C 000E25BC  7C 63 02 14 */	add r3, r3, r0
/* 800E5680 000E25C0  80 03 00 00 */	lwz r0, 0(r3)
/* 800E5684 000E25C4  90 01 00 7C */	stw r0, 0x7c(r1)
/* 800E5688 000E25C8  80 03 00 04 */	lwz r0, 4(r3)
/* 800E568C 000E25CC  90 01 00 80 */	stw r0, 0x80(r1)
lbl_800E5690:
/* 800E5690 000E25D0  80 01 00 7C */	lwz r0, 0x7c(r1)
/* 800E5694 000E25D4  2C 00 00 01 */	cmpwi r0, 1
/* 800E5698 000E25D8  41 82 00 1C */	beq lbl_800E56B4
/* 800E569C 000E25DC  40 80 00 2C */	bge lbl_800E56C8
/* 800E56A0 000E25E0  2C 00 00 00 */	cmpwi r0, 0
/* 800E56A4 000E25E4  40 80 00 08 */	bge lbl_800E56AC
/* 800E56A8 000E25E8  48 00 00 20 */	b lbl_800E56C8
lbl_800E56AC:
/* 800E56AC 000E25EC  80 01 00 80 */	lwz r0, 0x80(r1)
/* 800E56B0 000E25F0  48 00 00 1C */	b lbl_800E56CC
lbl_800E56B4:
/* 800E56B4 000E25F4  C0 01 00 80 */	lfs f0, 0x80(r1)
/* 800E56B8 000E25F8  FC 00 00 1E */	fctiwz f0, f0
/* 800E56BC 000E25FC  D8 01 00 88 */	stfd f0, 0x88(r1)
/* 800E56C0 000E2600  80 01 00 8C */	lwz r0, 0x8c(r1)
/* 800E56C4 000E2604  48 00 00 08 */	b lbl_800E56CC
lbl_800E56C8:
/* 800E56C8 000E2608  38 00 00 00 */	li r0, 0
lbl_800E56CC:
/* 800E56CC 000E260C  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E56D0 000E2610  7C 80 00 D0 */	neg r4, r0
/* 800E56D4 000E2614  30 04 FF FF */	addic r0, r4, -1
/* 800E56D8 000E2618  2C 03 00 00 */	cmpwi r3, 0
/* 800E56DC 000E261C  7F C0 21 10 */	subfe r30, r0, r4
/* 800E56E0 000E2620  41 81 00 34 */	bgt lbl_800E5714
/* 800E56E4 000E2624  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800E56E8 000E2628  4C C6 31 82 */	crclr 6
/* 800E56EC 000E262C  4B FF 79 B9 */	bl SpcTrace__FPCce
/* 800E56F0 000E2630  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E56F4 000E2634  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E56F8 000E2638  54 00 18 38 */	slwi r0, r0, 3
/* 800E56FC 000E263C  7C 63 02 14 */	add r3, r3, r0
/* 800E5700 000E2640  80 03 00 00 */	lwz r0, 0(r3)
/* 800E5704 000E2644  90 01 00 6C */	stw r0, 0x6c(r1)
/* 800E5708 000E2648  80 03 00 04 */	lwz r0, 4(r3)
/* 800E570C 000E264C  90 01 00 70 */	stw r0, 0x70(r1)
/* 800E5710 000E2650  48 00 00 2C */	b lbl_800E573C
lbl_800E5714:
/* 800E5714 000E2654  38 03 FF FF */	addi r0, r3, -1
/* 800E5718 000E2658  90 1D 00 1C */	stw r0, 0x1c(r29)
/* 800E571C 000E265C  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E5720 000E2660  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E5724 000E2664  54 00 18 38 */	slwi r0, r0, 3
/* 800E5728 000E2668  7C 63 02 14 */	add r3, r3, r0
/* 800E572C 000E266C  80 03 00 00 */	lwz r0, 0(r3)
/* 800E5730 000E2670  90 01 00 6C */	stw r0, 0x6c(r1)
/* 800E5734 000E2674  80 03 00 04 */	lwz r0, 4(r3)
/* 800E5738 000E2678  90 01 00 70 */	stw r0, 0x70(r1)
lbl_800E573C:
/* 800E573C 000E267C  80 81 00 6C */	lwz r4, 0x6c(r1)
/* 800E5740 000E2680  38 60 00 00 */	li r3, 0
/* 800E5744 000E2684  80 01 00 70 */	lwz r0, 0x70(r1)
/* 800E5748 000E2688  90 81 00 74 */	stw r4, 0x74(r1)
/* 800E574C 000E268C  90 01 00 78 */	stw r0, 0x78(r1)
/* 800E5750 000E2690  80 01 00 74 */	lwz r0, 0x74(r1)
/* 800E5754 000E2694  2C 00 00 01 */	cmpwi r0, 1
/* 800E5758 000E2698  41 82 00 9C */	beq lbl_800E57F4
/* 800E575C 000E269C  40 80 00 10 */	bge lbl_800E576C
/* 800E5760 000E26A0  2C 00 00 00 */	cmpwi r0, 0
/* 800E5764 000E26A4  40 80 00 58 */	bge lbl_800E57BC
/* 800E5768 000E26A8  48 00 00 8C */	b lbl_800E57F4
lbl_800E576C:
/* 800E576C 000E26AC  2C 00 00 03 */	cmpwi r0, 3
/* 800E5770 000E26B0  40 80 00 84 */	bge lbl_800E57F4
/* 800E5774 000E26B4  2C 00 00 02 */	cmpwi r0, 2
/* 800E5778 000E26B8  41 82 00 08 */	beq lbl_800E5780
/* 800E577C 000E26BC  48 00 00 0C */	b lbl_800E5788
lbl_800E5780:
/* 800E5780 000E26C0  83 81 00 78 */	lwz r28, 0x78(r1)
/* 800E5784 000E26C4  48 00 00 08 */	b lbl_800E578C
lbl_800E5788:
/* 800E5788 000E26C8  3B 82 8F F0 */	addi r28, r2, $$24053@sda21
lbl_800E578C:
/* 800E578C 000E26CC  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 800E5790 000E26D0  7F 83 E3 78 */	mr r3, r28
/* 800E5794 000E26D4  83 64 00 04 */	lwz r27, 4(r4)
/* 800E5798 000E26D8  4B F5 EE F5 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800E579C 000E26DC  81 9B 00 00 */	lwz r12, 0(r27)
/* 800E57A0 000E26E0  38 83 00 00 */	addi r4, r3, 0
/* 800E57A4 000E26E4  38 7B 00 00 */	addi r3, r27, 0
/* 800E57A8 000E26E8  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 800E57AC 000E26EC  7F 85 E3 78 */	mr r5, r28
/* 800E57B0 000E26F0  7D 88 03 A6 */	mtlr r12
/* 800E57B4 000E26F4  4E 80 00 21 */	blrl 
/* 800E57B8 000E26F8  48 00 00 3C */	b lbl_800E57F4
lbl_800E57BC:
/* 800E57BC 000E26FC  2C 00 00 01 */	cmpwi r0, 1
/* 800E57C0 000E2700  41 82 00 1C */	beq lbl_800E57DC
/* 800E57C4 000E2704  40 80 00 2C */	bge lbl_800E57F0
/* 800E57C8 000E2708  2C 00 00 00 */	cmpwi r0, 0
/* 800E57CC 000E270C  40 80 00 08 */	bge lbl_800E57D4
/* 800E57D0 000E2710  48 00 00 20 */	b lbl_800E57F0
lbl_800E57D4:
/* 800E57D4 000E2714  80 61 00 78 */	lwz r3, 0x78(r1)
/* 800E57D8 000E2718  48 00 00 1C */	b lbl_800E57F4
lbl_800E57DC:
/* 800E57DC 000E271C  C0 01 00 78 */	lfs f0, 0x78(r1)
/* 800E57E0 000E2720  FC 00 00 1E */	fctiwz f0, f0
/* 800E57E4 000E2724  D8 01 00 88 */	stfd f0, 0x88(r1)
/* 800E57E8 000E2728  80 61 00 8C */	lwz r3, 0x8c(r1)
/* 800E57EC 000E272C  48 00 00 08 */	b lbl_800E57F4
lbl_800E57F0:
/* 800E57F0 000E2730  38 60 00 00 */	li r3, 0
lbl_800E57F4:
/* 800E57F4 000E2734  28 03 00 00 */	cmplwi r3, 0
/* 800E57F8 000E2738  41 82 00 28 */	beq lbl_800E5820
/* 800E57FC 000E273C  57 C0 06 3F */	clrlwi. r0, r30, 0x18
/* 800E5800 000E2740  41 82 00 14 */	beq lbl_800E5814
/* 800E5804 000E2744  80 03 00 F0 */	lwz r0, 0xf0(r3)
/* 800E5808 000E2748  60 00 00 01 */	ori r0, r0, 1
/* 800E580C 000E274C  90 03 00 F0 */	stw r0, 0xf0(r3)
/* 800E5810 000E2750  48 00 00 10 */	b lbl_800E5820
lbl_800E5814:
/* 800E5814 000E2754  80 03 00 F0 */	lwz r0, 0xf0(r3)
/* 800E5818 000E2758  54 00 00 3C */	rlwinm r0, r0, 0, 0, 0x1e
/* 800E581C 000E275C  90 03 00 F0 */	stw r0, 0xf0(r3)
lbl_800E5820:
/* 800E5820 000E2760  38 00 00 00 */	li r0, 0
/* 800E5824 000E2764  90 01 00 58 */	stw r0, 0x58(r1)
/* 800E5828 000E2768  90 01 00 5C */	stw r0, 0x5c(r1)
/* 800E582C 000E276C  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E5830 000E2770  80 1D 00 18 */	lwz r0, 0x18(r29)
/* 800E5834 000E2774  7C 03 00 00 */	cmpw r3, r0
/* 800E5838 000E2778  41 80 00 14 */	blt lbl_800E584C
/* 800E583C 000E277C  38 7F 07 8C */	addi r3, r31, 0x78c
/* 800E5840 000E2780  4C C6 31 82 */	crclr 6
/* 800E5844 000E2784  4B FF 78 61 */	bl SpcTrace__FPCce
/* 800E5848 000E2788  48 00 00 2C */	b lbl_800E5874
lbl_800E584C:
/* 800E584C 000E278C  80 BD 00 20 */	lwz r5, 0x20(r29)
/* 800E5850 000E2790  54 64 18 38 */	slwi r4, r3, 3
/* 800E5854 000E2794  80 61 00 58 */	lwz r3, 0x58(r1)
/* 800E5858 000E2798  80 01 00 5C */	lwz r0, 0x5c(r1)
/* 800E585C 000E279C  7C 85 22 14 */	add r4, r5, r4
/* 800E5860 000E27A0  90 64 00 00 */	stw r3, 0(r4)
/* 800E5864 000E27A4  90 04 00 04 */	stw r0, 4(r4)
/* 800E5868 000E27A8  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E586C 000E27AC  38 03 00 01 */	addi r0, r3, 1
/* 800E5870 000E27B0  90 1D 00 1C */	stw r0, 0x1c(r29)
lbl_800E5874:
/* 800E5874 000E27B4  BB 61 00 94 */	lmw r27, 0x94(r1)
/* 800E5878 000E27B8  80 01 00 AC */	lwz r0, 0xac(r1)
/* 800E587C 000E27BC  38 21 00 A8 */	addi r1, r1, 0xa8
/* 800E5880 000E27C0  7C 08 03 A6 */	mtlr r0
/* 800E5884 000E27C4  4E 80 00 20 */	blr 

.global evSetFlagNPCDontThrow__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evSetFlagNPCDontThrow__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E5888 000E27C8  7C 08 02 A6 */	mflr r0
/* 800E588C 000E27CC  90 01 00 04 */	stw r0, 4(r1)
/* 800E5890 000E27D0  94 21 FF 58 */	stwu r1, -0xa8(r1)
/* 800E5894 000E27D4  BF 61 00 94 */	stmw r27, 0x94(r1)
/* 800E5898 000E27D8  7C 7D 1B 78 */	mr r29, r3
/* 800E589C 000E27DC  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E58A0 000E27E0  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E58A4 000E27E4  3C 80 80 37 */	lis r4, $$21490@ha
/* 800E58A8 000E27E8  3B E4 29 90 */	addi r31, r4, $$21490@l
/* 800E58AC 000E27EC  38 80 00 02 */	li r4, 2
/* 800E58B0 000E27F0  48 03 1B 81 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E58B4 000E27F4  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E58B8 000E27F8  2C 03 00 00 */	cmpwi r3, 0
/* 800E58BC 000E27FC  41 81 00 34 */	bgt lbl_800E58F0
/* 800E58C0 000E2800  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800E58C4 000E2804  4C C6 31 82 */	crclr 6
/* 800E58C8 000E2808  4B FF 77 DD */	bl SpcTrace__FPCce
/* 800E58CC 000E280C  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E58D0 000E2810  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E58D4 000E2814  54 00 18 38 */	slwi r0, r0, 3
/* 800E58D8 000E2818  7C 63 02 14 */	add r3, r3, r0
/* 800E58DC 000E281C  80 03 00 00 */	lwz r0, 0(r3)
/* 800E58E0 000E2820  90 01 00 7C */	stw r0, 0x7c(r1)
/* 800E58E4 000E2824  80 03 00 04 */	lwz r0, 4(r3)
/* 800E58E8 000E2828  90 01 00 80 */	stw r0, 0x80(r1)
/* 800E58EC 000E282C  48 00 00 2C */	b lbl_800E5918
lbl_800E58F0:
/* 800E58F0 000E2830  38 03 FF FF */	addi r0, r3, -1
/* 800E58F4 000E2834  90 1D 00 1C */	stw r0, 0x1c(r29)
/* 800E58F8 000E2838  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E58FC 000E283C  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E5900 000E2840  54 00 18 38 */	slwi r0, r0, 3
/* 800E5904 000E2844  7C 63 02 14 */	add r3, r3, r0
/* 800E5908 000E2848  80 03 00 00 */	lwz r0, 0(r3)
/* 800E590C 000E284C  90 01 00 7C */	stw r0, 0x7c(r1)
/* 800E5910 000E2850  80 03 00 04 */	lwz r0, 4(r3)
/* 800E5914 000E2854  90 01 00 80 */	stw r0, 0x80(r1)
lbl_800E5918:
/* 800E5918 000E2858  80 01 00 7C */	lwz r0, 0x7c(r1)
/* 800E591C 000E285C  2C 00 00 01 */	cmpwi r0, 1
/* 800E5920 000E2860  41 82 00 1C */	beq lbl_800E593C
/* 800E5924 000E2864  40 80 00 2C */	bge lbl_800E5950
/* 800E5928 000E2868  2C 00 00 00 */	cmpwi r0, 0
/* 800E592C 000E286C  40 80 00 08 */	bge lbl_800E5934
/* 800E5930 000E2870  48 00 00 20 */	b lbl_800E5950
lbl_800E5934:
/* 800E5934 000E2874  80 01 00 80 */	lwz r0, 0x80(r1)
/* 800E5938 000E2878  48 00 00 1C */	b lbl_800E5954
lbl_800E593C:
/* 800E593C 000E287C  C0 01 00 80 */	lfs f0, 0x80(r1)
/* 800E5940 000E2880  FC 00 00 1E */	fctiwz f0, f0
/* 800E5944 000E2884  D8 01 00 88 */	stfd f0, 0x88(r1)
/* 800E5948 000E2888  80 01 00 8C */	lwz r0, 0x8c(r1)
/* 800E594C 000E288C  48 00 00 08 */	b lbl_800E5954
lbl_800E5950:
/* 800E5950 000E2890  38 00 00 00 */	li r0, 0
lbl_800E5954:
/* 800E5954 000E2894  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E5958 000E2898  7C 80 00 D0 */	neg r4, r0
/* 800E595C 000E289C  30 04 FF FF */	addic r0, r4, -1
/* 800E5960 000E28A0  2C 03 00 00 */	cmpwi r3, 0
/* 800E5964 000E28A4  7F C0 21 10 */	subfe r30, r0, r4
/* 800E5968 000E28A8  41 81 00 34 */	bgt lbl_800E599C
/* 800E596C 000E28AC  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800E5970 000E28B0  4C C6 31 82 */	crclr 6
/* 800E5974 000E28B4  4B FF 77 31 */	bl SpcTrace__FPCce
/* 800E5978 000E28B8  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E597C 000E28BC  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E5980 000E28C0  54 00 18 38 */	slwi r0, r0, 3
/* 800E5984 000E28C4  7C 63 02 14 */	add r3, r3, r0
/* 800E5988 000E28C8  80 03 00 00 */	lwz r0, 0(r3)
/* 800E598C 000E28CC  90 01 00 6C */	stw r0, 0x6c(r1)
/* 800E5990 000E28D0  80 03 00 04 */	lwz r0, 4(r3)
/* 800E5994 000E28D4  90 01 00 70 */	stw r0, 0x70(r1)
/* 800E5998 000E28D8  48 00 00 2C */	b lbl_800E59C4
lbl_800E599C:
/* 800E599C 000E28DC  38 03 FF FF */	addi r0, r3, -1
/* 800E59A0 000E28E0  90 1D 00 1C */	stw r0, 0x1c(r29)
/* 800E59A4 000E28E4  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E59A8 000E28E8  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E59AC 000E28EC  54 00 18 38 */	slwi r0, r0, 3
/* 800E59B0 000E28F0  7C 63 02 14 */	add r3, r3, r0
/* 800E59B4 000E28F4  80 03 00 00 */	lwz r0, 0(r3)
/* 800E59B8 000E28F8  90 01 00 6C */	stw r0, 0x6c(r1)
/* 800E59BC 000E28FC  80 03 00 04 */	lwz r0, 4(r3)
/* 800E59C0 000E2900  90 01 00 70 */	stw r0, 0x70(r1)
lbl_800E59C4:
/* 800E59C4 000E2904  80 81 00 6C */	lwz r4, 0x6c(r1)
/* 800E59C8 000E2908  38 60 00 00 */	li r3, 0
/* 800E59CC 000E290C  80 01 00 70 */	lwz r0, 0x70(r1)
/* 800E59D0 000E2910  90 81 00 74 */	stw r4, 0x74(r1)
/* 800E59D4 000E2914  90 01 00 78 */	stw r0, 0x78(r1)
/* 800E59D8 000E2918  80 01 00 74 */	lwz r0, 0x74(r1)
/* 800E59DC 000E291C  2C 00 00 01 */	cmpwi r0, 1
/* 800E59E0 000E2920  41 82 00 9C */	beq lbl_800E5A7C
/* 800E59E4 000E2924  40 80 00 10 */	bge lbl_800E59F4
/* 800E59E8 000E2928  2C 00 00 00 */	cmpwi r0, 0
/* 800E59EC 000E292C  40 80 00 58 */	bge lbl_800E5A44
/* 800E59F0 000E2930  48 00 00 8C */	b lbl_800E5A7C
lbl_800E59F4:
/* 800E59F4 000E2934  2C 00 00 03 */	cmpwi r0, 3
/* 800E59F8 000E2938  40 80 00 84 */	bge lbl_800E5A7C
/* 800E59FC 000E293C  2C 00 00 02 */	cmpwi r0, 2
/* 800E5A00 000E2940  41 82 00 08 */	beq lbl_800E5A08
/* 800E5A04 000E2944  48 00 00 0C */	b lbl_800E5A10
lbl_800E5A08:
/* 800E5A08 000E2948  83 81 00 78 */	lwz r28, 0x78(r1)
/* 800E5A0C 000E294C  48 00 00 08 */	b lbl_800E5A14
lbl_800E5A10:
/* 800E5A10 000E2950  3B 82 8F F0 */	addi r28, r2, $$24053@sda21
lbl_800E5A14:
/* 800E5A14 000E2954  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 800E5A18 000E2958  7F 83 E3 78 */	mr r3, r28
/* 800E5A1C 000E295C  83 64 00 04 */	lwz r27, 4(r4)
/* 800E5A20 000E2960  4B F5 EC 6D */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800E5A24 000E2964  81 9B 00 00 */	lwz r12, 0(r27)
/* 800E5A28 000E2968  38 83 00 00 */	addi r4, r3, 0
/* 800E5A2C 000E296C  38 7B 00 00 */	addi r3, r27, 0
/* 800E5A30 000E2970  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 800E5A34 000E2974  7F 85 E3 78 */	mr r5, r28
/* 800E5A38 000E2978  7D 88 03 A6 */	mtlr r12
/* 800E5A3C 000E297C  4E 80 00 21 */	blrl 
/* 800E5A40 000E2980  48 00 00 3C */	b lbl_800E5A7C
lbl_800E5A44:
/* 800E5A44 000E2984  2C 00 00 01 */	cmpwi r0, 1
/* 800E5A48 000E2988  41 82 00 1C */	beq lbl_800E5A64
/* 800E5A4C 000E298C  40 80 00 2C */	bge lbl_800E5A78
/* 800E5A50 000E2990  2C 00 00 00 */	cmpwi r0, 0
/* 800E5A54 000E2994  40 80 00 08 */	bge lbl_800E5A5C
/* 800E5A58 000E2998  48 00 00 20 */	b lbl_800E5A78
lbl_800E5A5C:
/* 800E5A5C 000E299C  80 61 00 78 */	lwz r3, 0x78(r1)
/* 800E5A60 000E29A0  48 00 00 1C */	b lbl_800E5A7C
lbl_800E5A64:
/* 800E5A64 000E29A4  C0 01 00 78 */	lfs f0, 0x78(r1)
/* 800E5A68 000E29A8  FC 00 00 1E */	fctiwz f0, f0
/* 800E5A6C 000E29AC  D8 01 00 88 */	stfd f0, 0x88(r1)
/* 800E5A70 000E29B0  80 61 00 8C */	lwz r3, 0x8c(r1)
/* 800E5A74 000E29B4  48 00 00 08 */	b lbl_800E5A7C
lbl_800E5A78:
/* 800E5A78 000E29B8  38 60 00 00 */	li r3, 0
lbl_800E5A7C:
/* 800E5A7C 000E29BC  28 03 00 00 */	cmplwi r3, 0
/* 800E5A80 000E29C0  41 82 00 28 */	beq lbl_800E5AA8
/* 800E5A84 000E29C4  57 C0 06 3F */	clrlwi. r0, r30, 0x18
/* 800E5A88 000E29C8  41 82 00 14 */	beq lbl_800E5A9C
/* 800E5A8C 000E29CC  80 03 00 F0 */	lwz r0, 0xf0(r3)
/* 800E5A90 000E29D0  64 00 20 00 */	oris r0, r0, 0x2000
/* 800E5A94 000E29D4  90 03 00 F0 */	stw r0, 0xf0(r3)
/* 800E5A98 000E29D8  48 00 00 10 */	b lbl_800E5AA8
lbl_800E5A9C:
/* 800E5A9C 000E29DC  80 03 00 F0 */	lwz r0, 0xf0(r3)
/* 800E5AA0 000E29E0  54 00 00 C2 */	rlwinm r0, r0, 0, 3, 1
/* 800E5AA4 000E29E4  90 03 00 F0 */	stw r0, 0xf0(r3)
lbl_800E5AA8:
/* 800E5AA8 000E29E8  38 00 00 00 */	li r0, 0
/* 800E5AAC 000E29EC  90 01 00 58 */	stw r0, 0x58(r1)
/* 800E5AB0 000E29F0  90 01 00 5C */	stw r0, 0x5c(r1)
/* 800E5AB4 000E29F4  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E5AB8 000E29F8  80 1D 00 18 */	lwz r0, 0x18(r29)
/* 800E5ABC 000E29FC  7C 03 00 00 */	cmpw r3, r0
/* 800E5AC0 000E2A00  41 80 00 14 */	blt lbl_800E5AD4
/* 800E5AC4 000E2A04  38 7F 07 8C */	addi r3, r31, 0x78c
/* 800E5AC8 000E2A08  4C C6 31 82 */	crclr 6
/* 800E5ACC 000E2A0C  4B FF 75 D9 */	bl SpcTrace__FPCce
/* 800E5AD0 000E2A10  48 00 00 2C */	b lbl_800E5AFC
lbl_800E5AD4:
/* 800E5AD4 000E2A14  80 BD 00 20 */	lwz r5, 0x20(r29)
/* 800E5AD8 000E2A18  54 64 18 38 */	slwi r4, r3, 3
/* 800E5ADC 000E2A1C  80 61 00 58 */	lwz r3, 0x58(r1)
/* 800E5AE0 000E2A20  80 01 00 5C */	lwz r0, 0x5c(r1)
/* 800E5AE4 000E2A24  7C 85 22 14 */	add r4, r5, r4
/* 800E5AE8 000E2A28  90 64 00 00 */	stw r3, 0(r4)
/* 800E5AEC 000E2A2C  90 04 00 04 */	stw r0, 4(r4)
/* 800E5AF0 000E2A30  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E5AF4 000E2A34  38 03 00 01 */	addi r0, r3, 1
/* 800E5AF8 000E2A38  90 1D 00 1C */	stw r0, 0x1c(r29)
lbl_800E5AFC:
/* 800E5AFC 000E2A3C  BB 61 00 94 */	lmw r27, 0x94(r1)
/* 800E5B00 000E2A40  80 01 00 AC */	lwz r0, 0xac(r1)
/* 800E5B04 000E2A44  38 21 00 A8 */	addi r1, r1, 0xa8
/* 800E5B08 000E2A48  7C 08 03 A6 */	mtlr r0
/* 800E5B0C 000E2A4C  4E 80 00 20 */	blr 

.global evSetFlagNPCDontTalk__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evSetFlagNPCDontTalk__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E5B10 000E2A50  7C 08 02 A6 */	mflr r0
/* 800E5B14 000E2A54  90 01 00 04 */	stw r0, 4(r1)
/* 800E5B18 000E2A58  94 21 FF 58 */	stwu r1, -0xa8(r1)
/* 800E5B1C 000E2A5C  BF 61 00 94 */	stmw r27, 0x94(r1)
/* 800E5B20 000E2A60  7C 7D 1B 78 */	mr r29, r3
/* 800E5B24 000E2A64  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E5B28 000E2A68  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E5B2C 000E2A6C  3C 80 80 37 */	lis r4, $$21490@ha
/* 800E5B30 000E2A70  3B E4 29 90 */	addi r31, r4, $$21490@l
/* 800E5B34 000E2A74  38 80 00 02 */	li r4, 2
/* 800E5B38 000E2A78  48 03 18 F9 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E5B3C 000E2A7C  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E5B40 000E2A80  2C 03 00 00 */	cmpwi r3, 0
/* 800E5B44 000E2A84  41 81 00 34 */	bgt lbl_800E5B78
/* 800E5B48 000E2A88  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800E5B4C 000E2A8C  4C C6 31 82 */	crclr 6
/* 800E5B50 000E2A90  4B FF 75 55 */	bl SpcTrace__FPCce
/* 800E5B54 000E2A94  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E5B58 000E2A98  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E5B5C 000E2A9C  54 00 18 38 */	slwi r0, r0, 3
/* 800E5B60 000E2AA0  7C 63 02 14 */	add r3, r3, r0
/* 800E5B64 000E2AA4  80 03 00 00 */	lwz r0, 0(r3)
/* 800E5B68 000E2AA8  90 01 00 7C */	stw r0, 0x7c(r1)
/* 800E5B6C 000E2AAC  80 03 00 04 */	lwz r0, 4(r3)
/* 800E5B70 000E2AB0  90 01 00 80 */	stw r0, 0x80(r1)
/* 800E5B74 000E2AB4  48 00 00 2C */	b lbl_800E5BA0
lbl_800E5B78:
/* 800E5B78 000E2AB8  38 03 FF FF */	addi r0, r3, -1
/* 800E5B7C 000E2ABC  90 1D 00 1C */	stw r0, 0x1c(r29)
/* 800E5B80 000E2AC0  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E5B84 000E2AC4  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E5B88 000E2AC8  54 00 18 38 */	slwi r0, r0, 3
/* 800E5B8C 000E2ACC  7C 63 02 14 */	add r3, r3, r0
/* 800E5B90 000E2AD0  80 03 00 00 */	lwz r0, 0(r3)
/* 800E5B94 000E2AD4  90 01 00 7C */	stw r0, 0x7c(r1)
/* 800E5B98 000E2AD8  80 03 00 04 */	lwz r0, 4(r3)
/* 800E5B9C 000E2ADC  90 01 00 80 */	stw r0, 0x80(r1)
lbl_800E5BA0:
/* 800E5BA0 000E2AE0  80 01 00 7C */	lwz r0, 0x7c(r1)
/* 800E5BA4 000E2AE4  2C 00 00 01 */	cmpwi r0, 1
/* 800E5BA8 000E2AE8  41 82 00 1C */	beq lbl_800E5BC4
/* 800E5BAC 000E2AEC  40 80 00 2C */	bge lbl_800E5BD8
/* 800E5BB0 000E2AF0  2C 00 00 00 */	cmpwi r0, 0
/* 800E5BB4 000E2AF4  40 80 00 08 */	bge lbl_800E5BBC
/* 800E5BB8 000E2AF8  48 00 00 20 */	b lbl_800E5BD8
lbl_800E5BBC:
/* 800E5BBC 000E2AFC  80 01 00 80 */	lwz r0, 0x80(r1)
/* 800E5BC0 000E2B00  48 00 00 1C */	b lbl_800E5BDC
lbl_800E5BC4:
/* 800E5BC4 000E2B04  C0 01 00 80 */	lfs f0, 0x80(r1)
/* 800E5BC8 000E2B08  FC 00 00 1E */	fctiwz f0, f0
/* 800E5BCC 000E2B0C  D8 01 00 88 */	stfd f0, 0x88(r1)
/* 800E5BD0 000E2B10  80 01 00 8C */	lwz r0, 0x8c(r1)
/* 800E5BD4 000E2B14  48 00 00 08 */	b lbl_800E5BDC
lbl_800E5BD8:
/* 800E5BD8 000E2B18  38 00 00 00 */	li r0, 0
lbl_800E5BDC:
/* 800E5BDC 000E2B1C  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E5BE0 000E2B20  7C 80 00 D0 */	neg r4, r0
/* 800E5BE4 000E2B24  30 04 FF FF */	addic r0, r4, -1
/* 800E5BE8 000E2B28  2C 03 00 00 */	cmpwi r3, 0
/* 800E5BEC 000E2B2C  7F C0 21 10 */	subfe r30, r0, r4
/* 800E5BF0 000E2B30  41 81 00 34 */	bgt lbl_800E5C24
/* 800E5BF4 000E2B34  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800E5BF8 000E2B38  4C C6 31 82 */	crclr 6
/* 800E5BFC 000E2B3C  4B FF 74 A9 */	bl SpcTrace__FPCce
/* 800E5C00 000E2B40  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E5C04 000E2B44  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E5C08 000E2B48  54 00 18 38 */	slwi r0, r0, 3
/* 800E5C0C 000E2B4C  7C 63 02 14 */	add r3, r3, r0
/* 800E5C10 000E2B50  80 03 00 00 */	lwz r0, 0(r3)
/* 800E5C14 000E2B54  90 01 00 6C */	stw r0, 0x6c(r1)
/* 800E5C18 000E2B58  80 03 00 04 */	lwz r0, 4(r3)
/* 800E5C1C 000E2B5C  90 01 00 70 */	stw r0, 0x70(r1)
/* 800E5C20 000E2B60  48 00 00 2C */	b lbl_800E5C4C
lbl_800E5C24:
/* 800E5C24 000E2B64  38 03 FF FF */	addi r0, r3, -1
/* 800E5C28 000E2B68  90 1D 00 1C */	stw r0, 0x1c(r29)
/* 800E5C2C 000E2B6C  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E5C30 000E2B70  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E5C34 000E2B74  54 00 18 38 */	slwi r0, r0, 3
/* 800E5C38 000E2B78  7C 63 02 14 */	add r3, r3, r0
/* 800E5C3C 000E2B7C  80 03 00 00 */	lwz r0, 0(r3)
/* 800E5C40 000E2B80  90 01 00 6C */	stw r0, 0x6c(r1)
/* 800E5C44 000E2B84  80 03 00 04 */	lwz r0, 4(r3)
/* 800E5C48 000E2B88  90 01 00 70 */	stw r0, 0x70(r1)
lbl_800E5C4C:
/* 800E5C4C 000E2B8C  80 81 00 6C */	lwz r4, 0x6c(r1)
/* 800E5C50 000E2B90  38 60 00 00 */	li r3, 0
/* 800E5C54 000E2B94  80 01 00 70 */	lwz r0, 0x70(r1)
/* 800E5C58 000E2B98  90 81 00 74 */	stw r4, 0x74(r1)
/* 800E5C5C 000E2B9C  90 01 00 78 */	stw r0, 0x78(r1)
/* 800E5C60 000E2BA0  80 01 00 74 */	lwz r0, 0x74(r1)
/* 800E5C64 000E2BA4  2C 00 00 01 */	cmpwi r0, 1
/* 800E5C68 000E2BA8  41 82 00 9C */	beq lbl_800E5D04
/* 800E5C6C 000E2BAC  40 80 00 10 */	bge lbl_800E5C7C
/* 800E5C70 000E2BB0  2C 00 00 00 */	cmpwi r0, 0
/* 800E5C74 000E2BB4  40 80 00 58 */	bge lbl_800E5CCC
/* 800E5C78 000E2BB8  48 00 00 8C */	b lbl_800E5D04
lbl_800E5C7C:
/* 800E5C7C 000E2BBC  2C 00 00 03 */	cmpwi r0, 3
/* 800E5C80 000E2BC0  40 80 00 84 */	bge lbl_800E5D04
/* 800E5C84 000E2BC4  2C 00 00 02 */	cmpwi r0, 2
/* 800E5C88 000E2BC8  41 82 00 08 */	beq lbl_800E5C90
/* 800E5C8C 000E2BCC  48 00 00 0C */	b lbl_800E5C98
lbl_800E5C90:
/* 800E5C90 000E2BD0  83 81 00 78 */	lwz r28, 0x78(r1)
/* 800E5C94 000E2BD4  48 00 00 08 */	b lbl_800E5C9C
lbl_800E5C98:
/* 800E5C98 000E2BD8  3B 82 8F F0 */	addi r28, r2, $$24053@sda21
lbl_800E5C9C:
/* 800E5C9C 000E2BDC  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 800E5CA0 000E2BE0  7F 83 E3 78 */	mr r3, r28
/* 800E5CA4 000E2BE4  83 64 00 04 */	lwz r27, 4(r4)
/* 800E5CA8 000E2BE8  4B F5 E9 E5 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800E5CAC 000E2BEC  81 9B 00 00 */	lwz r12, 0(r27)
/* 800E5CB0 000E2BF0  38 83 00 00 */	addi r4, r3, 0
/* 800E5CB4 000E2BF4  38 7B 00 00 */	addi r3, r27, 0
/* 800E5CB8 000E2BF8  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 800E5CBC 000E2BFC  7F 85 E3 78 */	mr r5, r28
/* 800E5CC0 000E2C00  7D 88 03 A6 */	mtlr r12
/* 800E5CC4 000E2C04  4E 80 00 21 */	blrl 
/* 800E5CC8 000E2C08  48 00 00 3C */	b lbl_800E5D04
lbl_800E5CCC:
/* 800E5CCC 000E2C0C  2C 00 00 01 */	cmpwi r0, 1
/* 800E5CD0 000E2C10  41 82 00 1C */	beq lbl_800E5CEC
/* 800E5CD4 000E2C14  40 80 00 2C */	bge lbl_800E5D00
/* 800E5CD8 000E2C18  2C 00 00 00 */	cmpwi r0, 0
/* 800E5CDC 000E2C1C  40 80 00 08 */	bge lbl_800E5CE4
/* 800E5CE0 000E2C20  48 00 00 20 */	b lbl_800E5D00
lbl_800E5CE4:
/* 800E5CE4 000E2C24  80 61 00 78 */	lwz r3, 0x78(r1)
/* 800E5CE8 000E2C28  48 00 00 1C */	b lbl_800E5D04
lbl_800E5CEC:
/* 800E5CEC 000E2C2C  C0 01 00 78 */	lfs f0, 0x78(r1)
/* 800E5CF0 000E2C30  FC 00 00 1E */	fctiwz f0, f0
/* 800E5CF4 000E2C34  D8 01 00 88 */	stfd f0, 0x88(r1)
/* 800E5CF8 000E2C38  80 61 00 8C */	lwz r3, 0x8c(r1)
/* 800E5CFC 000E2C3C  48 00 00 08 */	b lbl_800E5D04
lbl_800E5D00:
/* 800E5D00 000E2C40  38 60 00 00 */	li r3, 0
lbl_800E5D04:
/* 800E5D04 000E2C44  28 03 00 00 */	cmplwi r3, 0
/* 800E5D08 000E2C48  41 82 00 28 */	beq lbl_800E5D30
/* 800E5D0C 000E2C4C  57 C0 06 3F */	clrlwi. r0, r30, 0x18
/* 800E5D10 000E2C50  41 82 00 14 */	beq lbl_800E5D24
/* 800E5D14 000E2C54  80 03 00 F0 */	lwz r0, 0xf0(r3)
/* 800E5D18 000E2C58  64 00 00 01 */	oris r0, r0, 1
/* 800E5D1C 000E2C5C  90 03 00 F0 */	stw r0, 0xf0(r3)
/* 800E5D20 000E2C60  48 00 00 10 */	b lbl_800E5D30
lbl_800E5D24:
/* 800E5D24 000E2C64  80 03 00 F0 */	lwz r0, 0xf0(r3)
/* 800E5D28 000E2C68  54 00 04 1C */	rlwinm r0, r0, 0, 0x10, 0xe
/* 800E5D2C 000E2C6C  90 03 00 F0 */	stw r0, 0xf0(r3)
lbl_800E5D30:
/* 800E5D30 000E2C70  38 00 00 00 */	li r0, 0
/* 800E5D34 000E2C74  90 01 00 58 */	stw r0, 0x58(r1)
/* 800E5D38 000E2C78  90 01 00 5C */	stw r0, 0x5c(r1)
/* 800E5D3C 000E2C7C  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E5D40 000E2C80  80 1D 00 18 */	lwz r0, 0x18(r29)
/* 800E5D44 000E2C84  7C 03 00 00 */	cmpw r3, r0
/* 800E5D48 000E2C88  41 80 00 14 */	blt lbl_800E5D5C
/* 800E5D4C 000E2C8C  38 7F 07 8C */	addi r3, r31, 0x78c
/* 800E5D50 000E2C90  4C C6 31 82 */	crclr 6
/* 800E5D54 000E2C94  4B FF 73 51 */	bl SpcTrace__FPCce
/* 800E5D58 000E2C98  48 00 00 2C */	b lbl_800E5D84
lbl_800E5D5C:
/* 800E5D5C 000E2C9C  80 BD 00 20 */	lwz r5, 0x20(r29)
/* 800E5D60 000E2CA0  54 64 18 38 */	slwi r4, r3, 3
/* 800E5D64 000E2CA4  80 61 00 58 */	lwz r3, 0x58(r1)
/* 800E5D68 000E2CA8  80 01 00 5C */	lwz r0, 0x5c(r1)
/* 800E5D6C 000E2CAC  7C 85 22 14 */	add r4, r5, r4
/* 800E5D70 000E2CB0  90 64 00 00 */	stw r3, 0(r4)
/* 800E5D74 000E2CB4  90 04 00 04 */	stw r0, 4(r4)
/* 800E5D78 000E2CB8  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E5D7C 000E2CBC  38 03 00 01 */	addi r0, r3, 1
/* 800E5D80 000E2CC0  90 1D 00 1C */	stw r0, 0x1c(r29)
lbl_800E5D84:
/* 800E5D84 000E2CC4  BB 61 00 94 */	lmw r27, 0x94(r1)
/* 800E5D88 000E2CC8  80 01 00 AC */	lwz r0, 0xac(r1)
/* 800E5D8C 000E2CCC  38 21 00 A8 */	addi r1, r1, 0xa8
/* 800E5D90 000E2CD0  7C 08 03 A6 */	mtlr r0
/* 800E5D94 000E2CD4  4E 80 00 20 */	blr 

.global evGetNPCType__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evGetNPCType__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E5D98 000E2CD8  7C 08 02 A6 */	mflr r0
/* 800E5D9C 000E2CDC  90 01 00 04 */	stw r0, 4(r1)
/* 800E5DA0 000E2CE0  94 21 FF 70 */	stwu r1, -0x90(r1)
/* 800E5DA4 000E2CE4  93 E1 00 8C */	stw r31, 0x8c(r1)
/* 800E5DA8 000E2CE8  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E5DAC 000E2CEC  93 C1 00 88 */	stw r30, 0x88(r1)
/* 800E5DB0 000E2CF0  7C 7E 1B 78 */	mr r30, r3
/* 800E5DB4 000E2CF4  93 A1 00 84 */	stw r29, 0x84(r1)
/* 800E5DB8 000E2CF8  93 81 00 80 */	stw r28, 0x80(r1)
/* 800E5DBC 000E2CFC  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E5DC0 000E2D00  38 80 00 01 */	li r4, 1
/* 800E5DC4 000E2D04  48 03 16 6D */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E5DC8 000E2D08  80 7E 00 1C */	lwz r3, 0x1c(r30)
/* 800E5DCC 000E2D0C  3B E0 FF FF */	li r31, -1
/* 800E5DD0 000E2D10  2C 03 00 00 */	cmpwi r3, 0
/* 800E5DD4 000E2D14  41 81 00 38 */	bgt lbl_800E5E0C
/* 800E5DD8 000E2D18  3C 60 80 37 */	lis r3, $$23829@ha
/* 800E5DDC 000E2D1C  4C C6 31 82 */	crclr 6
/* 800E5DE0 000E2D20  38 63 30 FC */	addi r3, r3, $$23829@l
/* 800E5DE4 000E2D24  4B FF 72 C1 */	bl SpcTrace__FPCce
/* 800E5DE8 000E2D28  80 1E 00 1C */	lwz r0, 0x1c(r30)
/* 800E5DEC 000E2D2C  80 7E 00 20 */	lwz r3, 0x20(r30)
/* 800E5DF0 000E2D30  54 00 18 38 */	slwi r0, r0, 3
/* 800E5DF4 000E2D34  7C 63 02 14 */	add r3, r3, r0
/* 800E5DF8 000E2D38  80 03 00 00 */	lwz r0, 0(r3)
/* 800E5DFC 000E2D3C  90 01 00 64 */	stw r0, 0x64(r1)
/* 800E5E00 000E2D40  80 03 00 04 */	lwz r0, 4(r3)
/* 800E5E04 000E2D44  90 01 00 68 */	stw r0, 0x68(r1)
/* 800E5E08 000E2D48  48 00 00 2C */	b lbl_800E5E34
lbl_800E5E0C:
/* 800E5E0C 000E2D4C  38 03 FF FF */	addi r0, r3, -1
/* 800E5E10 000E2D50  90 1E 00 1C */	stw r0, 0x1c(r30)
/* 800E5E14 000E2D54  80 1E 00 1C */	lwz r0, 0x1c(r30)
/* 800E5E18 000E2D58  80 7E 00 20 */	lwz r3, 0x20(r30)
/* 800E5E1C 000E2D5C  54 00 18 38 */	slwi r0, r0, 3
/* 800E5E20 000E2D60  7C 63 02 14 */	add r3, r3, r0
/* 800E5E24 000E2D64  80 03 00 00 */	lwz r0, 0(r3)
/* 800E5E28 000E2D68  90 01 00 64 */	stw r0, 0x64(r1)
/* 800E5E2C 000E2D6C  80 03 00 04 */	lwz r0, 4(r3)
/* 800E5E30 000E2D70  90 01 00 68 */	stw r0, 0x68(r1)
lbl_800E5E34:
/* 800E5E34 000E2D74  80 81 00 64 */	lwz r4, 0x64(r1)
/* 800E5E38 000E2D78  38 60 00 00 */	li r3, 0
/* 800E5E3C 000E2D7C  80 01 00 68 */	lwz r0, 0x68(r1)
/* 800E5E40 000E2D80  90 81 00 6C */	stw r4, 0x6c(r1)
/* 800E5E44 000E2D84  90 01 00 70 */	stw r0, 0x70(r1)
/* 800E5E48 000E2D88  80 01 00 6C */	lwz r0, 0x6c(r1)
/* 800E5E4C 000E2D8C  2C 00 00 01 */	cmpwi r0, 1
/* 800E5E50 000E2D90  41 82 00 9C */	beq lbl_800E5EEC
/* 800E5E54 000E2D94  40 80 00 10 */	bge lbl_800E5E64
/* 800E5E58 000E2D98  2C 00 00 00 */	cmpwi r0, 0
/* 800E5E5C 000E2D9C  40 80 00 58 */	bge lbl_800E5EB4
/* 800E5E60 000E2DA0  48 00 00 8C */	b lbl_800E5EEC
lbl_800E5E64:
/* 800E5E64 000E2DA4  2C 00 00 03 */	cmpwi r0, 3
/* 800E5E68 000E2DA8  40 80 00 84 */	bge lbl_800E5EEC
/* 800E5E6C 000E2DAC  2C 00 00 02 */	cmpwi r0, 2
/* 800E5E70 000E2DB0  41 82 00 08 */	beq lbl_800E5E78
/* 800E5E74 000E2DB4  48 00 00 0C */	b lbl_800E5E80
lbl_800E5E78:
/* 800E5E78 000E2DB8  83 A1 00 70 */	lwz r29, 0x70(r1)
/* 800E5E7C 000E2DBC  48 00 00 08 */	b lbl_800E5E84
lbl_800E5E80:
/* 800E5E80 000E2DC0  3B A2 8F F0 */	addi r29, r2, $$24053@sda21
lbl_800E5E84:
/* 800E5E84 000E2DC4  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 800E5E88 000E2DC8  7F A3 EB 78 */	mr r3, r29
/* 800E5E8C 000E2DCC  83 84 00 04 */	lwz r28, 4(r4)
/* 800E5E90 000E2DD0  4B F5 E7 FD */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800E5E94 000E2DD4  81 9C 00 00 */	lwz r12, 0(r28)
/* 800E5E98 000E2DD8  38 83 00 00 */	addi r4, r3, 0
/* 800E5E9C 000E2DDC  38 7C 00 00 */	addi r3, r28, 0
/* 800E5EA0 000E2DE0  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 800E5EA4 000E2DE4  7F A5 EB 78 */	mr r5, r29
/* 800E5EA8 000E2DE8  7D 88 03 A6 */	mtlr r12
/* 800E5EAC 000E2DEC  4E 80 00 21 */	blrl 
/* 800E5EB0 000E2DF0  48 00 00 3C */	b lbl_800E5EEC
lbl_800E5EB4:
/* 800E5EB4 000E2DF4  2C 00 00 01 */	cmpwi r0, 1
/* 800E5EB8 000E2DF8  41 82 00 1C */	beq lbl_800E5ED4
/* 800E5EBC 000E2DFC  40 80 00 2C */	bge lbl_800E5EE8
/* 800E5EC0 000E2E00  2C 00 00 00 */	cmpwi r0, 0
/* 800E5EC4 000E2E04  40 80 00 08 */	bge lbl_800E5ECC
/* 800E5EC8 000E2E08  48 00 00 20 */	b lbl_800E5EE8
lbl_800E5ECC:
/* 800E5ECC 000E2E0C  80 61 00 70 */	lwz r3, 0x70(r1)
/* 800E5ED0 000E2E10  48 00 00 1C */	b lbl_800E5EEC
lbl_800E5ED4:
/* 800E5ED4 000E2E14  C0 01 00 70 */	lfs f0, 0x70(r1)
/* 800E5ED8 000E2E18  FC 00 00 1E */	fctiwz f0, f0
/* 800E5EDC 000E2E1C  D8 01 00 78 */	stfd f0, 0x78(r1)
/* 800E5EE0 000E2E20  80 61 00 7C */	lwz r3, 0x7c(r1)
/* 800E5EE4 000E2E24  48 00 00 08 */	b lbl_800E5EEC
lbl_800E5EE8:
/* 800E5EE8 000E2E28  38 60 00 00 */	li r3, 0
lbl_800E5EEC:
/* 800E5EEC 000E2E2C  28 03 00 00 */	cmplwi r3, 0
/* 800E5EF0 000E2E30  41 82 00 10 */	beq lbl_800E5F00
/* 800E5EF4 000E2E34  80 63 00 4C */	lwz r3, 0x4c(r3)
/* 800E5EF8 000E2E38  3F E3 FC 00 */	addis r31, r3, 0xfc00
/* 800E5EFC 000E2E3C  3B FF FF FF */	addi r31, r31, -1
lbl_800E5F00:
/* 800E5F00 000E2E40  38 00 00 00 */	li r0, 0
/* 800E5F04 000E2E44  90 01 00 50 */	stw r0, 0x50(r1)
/* 800E5F08 000E2E48  93 E1 00 54 */	stw r31, 0x54(r1)
/* 800E5F0C 000E2E4C  80 7E 00 1C */	lwz r3, 0x1c(r30)
/* 800E5F10 000E2E50  80 1E 00 18 */	lwz r0, 0x18(r30)
/* 800E5F14 000E2E54  7C 03 00 00 */	cmpw r3, r0
/* 800E5F18 000E2E58  41 80 00 18 */	blt lbl_800E5F30
/* 800E5F1C 000E2E5C  3C 60 80 37 */	lis r3, $$23834@ha
/* 800E5F20 000E2E60  4C C6 31 82 */	crclr 6
/* 800E5F24 000E2E64  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800E5F28 000E2E68  4B FF 71 7D */	bl SpcTrace__FPCce
/* 800E5F2C 000E2E6C  48 00 00 2C */	b lbl_800E5F58
lbl_800E5F30:
/* 800E5F30 000E2E70  80 BE 00 20 */	lwz r5, 0x20(r30)
/* 800E5F34 000E2E74  54 64 18 38 */	slwi r4, r3, 3
/* 800E5F38 000E2E78  80 61 00 50 */	lwz r3, 0x50(r1)
/* 800E5F3C 000E2E7C  80 01 00 54 */	lwz r0, 0x54(r1)
/* 800E5F40 000E2E80  7C 85 22 14 */	add r4, r5, r4
/* 800E5F44 000E2E84  90 64 00 00 */	stw r3, 0(r4)
/* 800E5F48 000E2E88  90 04 00 04 */	stw r0, 4(r4)
/* 800E5F4C 000E2E8C  80 7E 00 1C */	lwz r3, 0x1c(r30)
/* 800E5F50 000E2E90  38 03 00 01 */	addi r0, r3, 1
/* 800E5F54 000E2E94  90 1E 00 1C */	stw r0, 0x1c(r30)
lbl_800E5F58:
/* 800E5F58 000E2E98  80 01 00 94 */	lwz r0, 0x94(r1)
/* 800E5F5C 000E2E9C  83 E1 00 8C */	lwz r31, 0x8c(r1)
/* 800E5F60 000E2EA0  83 C1 00 88 */	lwz r30, 0x88(r1)
/* 800E5F64 000E2EA4  7C 08 03 A6 */	mtlr r0
/* 800E5F68 000E2EA8  83 A1 00 84 */	lwz r29, 0x84(r1)
/* 800E5F6C 000E2EAC  83 81 00 80 */	lwz r28, 0x80(r1)
/* 800E5F70 000E2EB0  38 21 00 90 */	addi r1, r1, 0x90
/* 800E5F74 000E2EB4  4E 80 00 20 */	blr 

.global evGetNameRefName__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evGetNameRefName__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E5F78 000E2EB8  7C 08 02 A6 */	mflr r0
/* 800E5F7C 000E2EBC  90 01 00 04 */	stw r0, 4(r1)
/* 800E5F80 000E2EC0  94 21 FF B0 */	stwu r1, -0x50(r1)
/* 800E5F84 000E2EC4  93 E1 00 4C */	stw r31, 0x4c(r1)
/* 800E5F88 000E2EC8  7C 7F 1B 78 */	mr r31, r3
/* 800E5F8C 000E2ECC  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E5F90 000E2ED0  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E5F94 000E2ED4  38 80 00 01 */	li r4, 1
/* 800E5F98 000E2ED8  48 03 14 99 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E5F9C 000E2EDC  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E5FA0 000E2EE0  2C 03 00 00 */	cmpwi r3, 0
/* 800E5FA4 000E2EE4  41 81 00 38 */	bgt lbl_800E5FDC
/* 800E5FA8 000E2EE8  3C 60 80 37 */	lis r3, $$23829@ha
/* 800E5FAC 000E2EEC  4C C6 31 82 */	crclr 6
/* 800E5FB0 000E2EF0  38 63 30 FC */	addi r3, r3, $$23829@l
/* 800E5FB4 000E2EF4  4B FF 70 F1 */	bl SpcTrace__FPCce
/* 800E5FB8 000E2EF8  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800E5FBC 000E2EFC  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800E5FC0 000E2F00  54 00 18 38 */	slwi r0, r0, 3
/* 800E5FC4 000E2F04  7C 63 02 14 */	add r3, r3, r0
/* 800E5FC8 000E2F08  80 03 00 00 */	lwz r0, 0(r3)
/* 800E5FCC 000E2F0C  90 01 00 34 */	stw r0, 0x34(r1)
/* 800E5FD0 000E2F10  80 03 00 04 */	lwz r0, 4(r3)
/* 800E5FD4 000E2F14  90 01 00 38 */	stw r0, 0x38(r1)
/* 800E5FD8 000E2F18  48 00 00 2C */	b lbl_800E6004
lbl_800E5FDC:
/* 800E5FDC 000E2F1C  38 03 FF FF */	addi r0, r3, -1
/* 800E5FE0 000E2F20  90 1F 00 1C */	stw r0, 0x1c(r31)
/* 800E5FE4 000E2F24  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800E5FE8 000E2F28  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800E5FEC 000E2F2C  54 00 18 38 */	slwi r0, r0, 3
/* 800E5FF0 000E2F30  7C 63 02 14 */	add r3, r3, r0
/* 800E5FF4 000E2F34  80 03 00 00 */	lwz r0, 0(r3)
/* 800E5FF8 000E2F38  90 01 00 34 */	stw r0, 0x34(r1)
/* 800E5FFC 000E2F3C  80 03 00 04 */	lwz r0, 4(r3)
/* 800E6000 000E2F40  90 01 00 38 */	stw r0, 0x38(r1)
lbl_800E6004:
/* 800E6004 000E2F44  80 01 00 34 */	lwz r0, 0x34(r1)
/* 800E6008 000E2F48  2C 00 00 01 */	cmpwi r0, 1
/* 800E600C 000E2F4C  41 82 00 1C */	beq lbl_800E6028
/* 800E6010 000E2F50  40 80 00 2C */	bge lbl_800E603C
/* 800E6014 000E2F54  2C 00 00 00 */	cmpwi r0, 0
/* 800E6018 000E2F58  40 80 00 08 */	bge lbl_800E6020
/* 800E601C 000E2F5C  48 00 00 20 */	b lbl_800E603C
lbl_800E6020:
/* 800E6020 000E2F60  80 61 00 38 */	lwz r3, 0x38(r1)
/* 800E6024 000E2F64  48 00 00 1C */	b lbl_800E6040
lbl_800E6028:
/* 800E6028 000E2F68  C0 01 00 38 */	lfs f0, 0x38(r1)
/* 800E602C 000E2F6C  FC 00 00 1E */	fctiwz f0, f0
/* 800E6030 000E2F70  D8 01 00 40 */	stfd f0, 0x40(r1)
/* 800E6034 000E2F74  80 61 00 44 */	lwz r3, 0x44(r1)
/* 800E6038 000E2F78  48 00 00 08 */	b lbl_800E6040
lbl_800E603C:
/* 800E603C 000E2F7C  38 60 00 00 */	li r3, 0
lbl_800E6040:
/* 800E6040 000E2F80  2C 03 00 00 */	cmpwi r3, 0
/* 800E6044 000E2F84  41 82 00 0C */	beq lbl_800E6050
/* 800E6048 000E2F88  80 83 00 04 */	lwz r4, 4(r3)
/* 800E604C 000E2F8C  48 00 00 08 */	b lbl_800E6054
lbl_800E6050:
/* 800E6050 000E2F90  38 82 8F F0 */	addi r4, r2, $$24053@sda21
lbl_800E6054:
/* 800E6054 000E2F94  38 60 00 00 */	li r3, 0
/* 800E6058 000E2F98  90 61 00 28 */	stw r3, 0x28(r1)
/* 800E605C 000E2F9C  28 04 00 00 */	cmplwi r4, 0
/* 800E6060 000E2FA0  38 00 00 02 */	li r0, 2
/* 800E6064 000E2FA4  90 61 00 2C */	stw r3, 0x2c(r1)
/* 800E6068 000E2FA8  90 01 00 28 */	stw r0, 0x28(r1)
/* 800E606C 000E2FAC  40 82 00 10 */	bne lbl_800E607C
/* 800E6070 000E2FB0  38 02 8F F0 */	addi r0, r2, $$24053@sda21
/* 800E6074 000E2FB4  90 01 00 2C */	stw r0, 0x2c(r1)
/* 800E6078 000E2FB8  48 00 00 08 */	b lbl_800E6080
lbl_800E607C:
/* 800E607C 000E2FBC  90 81 00 2C */	stw r4, 0x2c(r1)
lbl_800E6080:
/* 800E6080 000E2FC0  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E6084 000E2FC4  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800E6088 000E2FC8  7C 03 00 00 */	cmpw r3, r0
/* 800E608C 000E2FCC  41 80 00 18 */	blt lbl_800E60A4
/* 800E6090 000E2FD0  3C 60 80 37 */	lis r3, $$23834@ha
/* 800E6094 000E2FD4  4C C6 31 82 */	crclr 6
/* 800E6098 000E2FD8  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800E609C 000E2FDC  4B FF 70 09 */	bl SpcTrace__FPCce
/* 800E60A0 000E2FE0  48 00 00 2C */	b lbl_800E60CC
lbl_800E60A4:
/* 800E60A4 000E2FE4  80 BF 00 20 */	lwz r5, 0x20(r31)
/* 800E60A8 000E2FE8  54 64 18 38 */	slwi r4, r3, 3
/* 800E60AC 000E2FEC  80 61 00 28 */	lwz r3, 0x28(r1)
/* 800E60B0 000E2FF0  80 01 00 2C */	lwz r0, 0x2c(r1)
/* 800E60B4 000E2FF4  7C 85 22 14 */	add r4, r5, r4
/* 800E60B8 000E2FF8  90 64 00 00 */	stw r3, 0(r4)
/* 800E60BC 000E2FFC  90 04 00 04 */	stw r0, 4(r4)
/* 800E60C0 000E3000  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E60C4 000E3004  38 03 00 01 */	addi r0, r3, 1
/* 800E60C8 000E3008  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800E60CC:
/* 800E60CC 000E300C  80 01 00 54 */	lwz r0, 0x54(r1)
/* 800E60D0 000E3010  83 E1 00 4C */	lwz r31, 0x4c(r1)
/* 800E60D4 000E3014  38 21 00 50 */	addi r1, r1, 0x50
/* 800E60D8 000E3018  7C 08 03 A6 */	mtlr r0
/* 800E60DC 000E301C  4E 80 00 20 */	blr 

.global evGetNameRefHandle__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evGetNameRefHandle__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E60E0 000E3020  7C 08 02 A6 */	mflr r0
/* 800E60E4 000E3024  90 01 00 04 */	stw r0, 4(r1)
/* 800E60E8 000E3028  94 21 FF 98 */	stwu r1, -0x68(r1)
/* 800E60EC 000E302C  93 E1 00 64 */	stw r31, 0x64(r1)
/* 800E60F0 000E3030  7C 7F 1B 78 */	mr r31, r3
/* 800E60F4 000E3034  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E60F8 000E3038  93 C1 00 60 */	stw r30, 0x60(r1)
/* 800E60FC 000E303C  93 A1 00 5C */	stw r29, 0x5c(r1)
/* 800E6100 000E3040  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E6104 000E3044  38 80 00 01 */	li r4, 1
/* 800E6108 000E3048  48 03 13 29 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E610C 000E304C  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E6110 000E3050  2C 03 00 00 */	cmpwi r3, 0
/* 800E6114 000E3054  41 81 00 38 */	bgt lbl_800E614C
/* 800E6118 000E3058  3C 60 80 37 */	lis r3, $$23829@ha
/* 800E611C 000E305C  4C C6 31 82 */	crclr 6
/* 800E6120 000E3060  38 63 30 FC */	addi r3, r3, $$23829@l
/* 800E6124 000E3064  4B FF 6F 81 */	bl SpcTrace__FPCce
/* 800E6128 000E3068  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800E612C 000E306C  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800E6130 000E3070  54 00 18 38 */	slwi r0, r0, 3
/* 800E6134 000E3074  7C 63 02 14 */	add r3, r3, r0
/* 800E6138 000E3078  80 03 00 00 */	lwz r0, 0(r3)
/* 800E613C 000E307C  90 01 00 4C */	stw r0, 0x4c(r1)
/* 800E6140 000E3080  80 03 00 04 */	lwz r0, 4(r3)
/* 800E6144 000E3084  90 01 00 50 */	stw r0, 0x50(r1)
/* 800E6148 000E3088  48 00 00 2C */	b lbl_800E6174
lbl_800E614C:
/* 800E614C 000E308C  38 03 FF FF */	addi r0, r3, -1
/* 800E6150 000E3090  90 1F 00 1C */	stw r0, 0x1c(r31)
/* 800E6154 000E3094  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800E6158 000E3098  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800E615C 000E309C  54 00 18 38 */	slwi r0, r0, 3
/* 800E6160 000E30A0  7C 63 02 14 */	add r3, r3, r0
/* 800E6164 000E30A4  80 03 00 00 */	lwz r0, 0(r3)
/* 800E6168 000E30A8  90 01 00 4C */	stw r0, 0x4c(r1)
/* 800E616C 000E30AC  80 03 00 04 */	lwz r0, 4(r3)
/* 800E6170 000E30B0  90 01 00 50 */	stw r0, 0x50(r1)
lbl_800E6174:
/* 800E6174 000E30B4  80 01 00 4C */	lwz r0, 0x4c(r1)
/* 800E6178 000E30B8  2C 00 00 02 */	cmpwi r0, 2
/* 800E617C 000E30BC  41 82 00 08 */	beq lbl_800E6184
/* 800E6180 000E30C0  48 00 00 0C */	b lbl_800E618C
lbl_800E6184:
/* 800E6184 000E30C4  83 C1 00 50 */	lwz r30, 0x50(r1)
/* 800E6188 000E30C8  48 00 00 08 */	b lbl_800E6190
lbl_800E618C:
/* 800E618C 000E30CC  3B C2 8F F0 */	addi r30, r2, $$24053@sda21
lbl_800E6190:
/* 800E6190 000E30D0  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 800E6194 000E30D4  7F C3 F3 78 */	mr r3, r30
/* 800E6198 000E30D8  83 A4 00 04 */	lwz r29, 4(r4)
/* 800E619C 000E30DC  4B F5 E4 F1 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800E61A0 000E30E0  81 9D 00 00 */	lwz r12, 0(r29)
/* 800E61A4 000E30E4  38 83 00 00 */	addi r4, r3, 0
/* 800E61A8 000E30E8  38 7D 00 00 */	addi r3, r29, 0
/* 800E61AC 000E30EC  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 800E61B0 000E30F0  7F C5 F3 78 */	mr r5, r30
/* 800E61B4 000E30F4  7D 88 03 A6 */	mtlr r12
/* 800E61B8 000E30F8  4E 80 00 21 */	blrl 
/* 800E61BC 000E30FC  38 00 00 00 */	li r0, 0
/* 800E61C0 000E3100  90 01 00 40 */	stw r0, 0x40(r1)
/* 800E61C4 000E3104  90 61 00 44 */	stw r3, 0x44(r1)
/* 800E61C8 000E3108  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E61CC 000E310C  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800E61D0 000E3110  7C 03 00 00 */	cmpw r3, r0
/* 800E61D4 000E3114  41 80 00 18 */	blt lbl_800E61EC
/* 800E61D8 000E3118  3C 60 80 37 */	lis r3, $$23834@ha
/* 800E61DC 000E311C  4C C6 31 82 */	crclr 6
/* 800E61E0 000E3120  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800E61E4 000E3124  4B FF 6E C1 */	bl SpcTrace__FPCce
/* 800E61E8 000E3128  48 00 00 2C */	b lbl_800E6214
lbl_800E61EC:
/* 800E61EC 000E312C  80 BF 00 20 */	lwz r5, 0x20(r31)
/* 800E61F0 000E3130  54 64 18 38 */	slwi r4, r3, 3
/* 800E61F4 000E3134  80 61 00 40 */	lwz r3, 0x40(r1)
/* 800E61F8 000E3138  80 01 00 44 */	lwz r0, 0x44(r1)
/* 800E61FC 000E313C  7C 85 22 14 */	add r4, r5, r4
/* 800E6200 000E3140  90 64 00 00 */	stw r3, 0(r4)
/* 800E6204 000E3144  90 04 00 04 */	stw r0, 4(r4)
/* 800E6208 000E3148  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E620C 000E314C  38 03 00 01 */	addi r0, r3, 1
/* 800E6210 000E3150  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800E6214:
/* 800E6214 000E3154  80 01 00 6C */	lwz r0, 0x6c(r1)
/* 800E6218 000E3158  83 E1 00 64 */	lwz r31, 0x64(r1)
/* 800E621C 000E315C  83 C1 00 60 */	lwz r30, 0x60(r1)
/* 800E6220 000E3160  7C 08 03 A6 */	mtlr r0
/* 800E6224 000E3164  83 A1 00 5C */	lwz r29, 0x5c(r1)
/* 800E6228 000E3168  38 21 00 68 */	addi r1, r1, 0x68
/* 800E622C 000E316C  4E 80 00 20 */	blr 

.global evSetSystemFlag__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evSetSystemFlag__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E6230 000E3170  7C 08 02 A6 */	mflr r0
/* 800E6234 000E3174  90 01 00 04 */	stw r0, 4(r1)
/* 800E6238 000E3178  94 21 FF A8 */	stwu r1, -0x58(r1)
/* 800E623C 000E317C  93 E1 00 54 */	stw r31, 0x54(r1)
/* 800E6240 000E3180  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E6244 000E3184  93 C1 00 50 */	stw r30, 0x50(r1)
/* 800E6248 000E3188  93 A1 00 4C */	stw r29, 0x4c(r1)
/* 800E624C 000E318C  7C 7D 1B 78 */	mr r29, r3
/* 800E6250 000E3190  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E6254 000E3194  3C 80 80 37 */	lis r4, $$21490@ha
/* 800E6258 000E3198  3B E4 29 90 */	addi r31, r4, $$21490@l
/* 800E625C 000E319C  38 80 00 02 */	li r4, 2
/* 800E6260 000E31A0  48 03 11 D1 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E6264 000E31A4  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E6268 000E31A8  2C 03 00 00 */	cmpwi r3, 0
/* 800E626C 000E31AC  41 81 00 30 */	bgt lbl_800E629C
/* 800E6270 000E31B0  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800E6274 000E31B4  4C C6 31 82 */	crclr 6
/* 800E6278 000E31B8  4B FF 6E 2D */	bl SpcTrace__FPCce
/* 800E627C 000E31BC  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E6280 000E31C0  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E6284 000E31C4  54 00 18 38 */	slwi r0, r0, 3
/* 800E6288 000E31C8  7C 63 02 14 */	add r3, r3, r0
/* 800E628C 000E31CC  83 C3 00 00 */	lwz r30, 0(r3)
/* 800E6290 000E31D0  80 03 00 04 */	lwz r0, 4(r3)
/* 800E6294 000E31D4  90 01 00 2C */	stw r0, 0x2c(r1)
/* 800E6298 000E31D8  48 00 00 28 */	b lbl_800E62C0
lbl_800E629C:
/* 800E629C 000E31DC  38 03 FF FF */	addi r0, r3, -1
/* 800E62A0 000E31E0  90 1D 00 1C */	stw r0, 0x1c(r29)
/* 800E62A4 000E31E4  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E62A8 000E31E8  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E62AC 000E31EC  54 00 18 38 */	slwi r0, r0, 3
/* 800E62B0 000E31F0  7C 63 02 14 */	add r3, r3, r0
/* 800E62B4 000E31F4  83 C3 00 00 */	lwz r30, 0(r3)
/* 800E62B8 000E31F8  80 03 00 04 */	lwz r0, 4(r3)
/* 800E62BC 000E31FC  90 01 00 2C */	stw r0, 0x2c(r1)
lbl_800E62C0:
/* 800E62C0 000E3200  80 01 00 2C */	lwz r0, 0x2c(r1)
/* 800E62C4 000E3204  90 01 00 3C */	stw r0, 0x3c(r1)
/* 800E62C8 000E3208  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E62CC 000E320C  2C 03 00 00 */	cmpwi r3, 0
/* 800E62D0 000E3210  41 81 00 30 */	bgt lbl_800E6300
/* 800E62D4 000E3214  38 7F 07 6C */	addi r3, r31, 0x76c
/* 800E62D8 000E3218  4C C6 31 82 */	crclr 6
/* 800E62DC 000E321C  4B FF 6D C9 */	bl SpcTrace__FPCce
/* 800E62E0 000E3220  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E62E4 000E3224  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E62E8 000E3228  54 00 18 38 */	slwi r0, r0, 3
/* 800E62EC 000E322C  7C 63 02 14 */	add r3, r3, r0
/* 800E62F0 000E3230  80 83 00 00 */	lwz r4, 0(r3)
/* 800E62F4 000E3234  80 03 00 04 */	lwz r0, 4(r3)
/* 800E62F8 000E3238  90 01 00 24 */	stw r0, 0x24(r1)
/* 800E62FC 000E323C  48 00 00 28 */	b lbl_800E6324
lbl_800E6300:
/* 800E6300 000E3240  38 03 FF FF */	addi r0, r3, -1
/* 800E6304 000E3244  90 1D 00 1C */	stw r0, 0x1c(r29)
/* 800E6308 000E3248  80 1D 00 1C */	lwz r0, 0x1c(r29)
/* 800E630C 000E324C  80 7D 00 20 */	lwz r3, 0x20(r29)
/* 800E6310 000E3250  54 00 18 38 */	slwi r0, r0, 3
/* 800E6314 000E3254  7C 63 02 14 */	add r3, r3, r0
/* 800E6318 000E3258  80 83 00 00 */	lwz r4, 0(r3)
/* 800E631C 000E325C  80 03 00 04 */	lwz r0, 4(r3)
/* 800E6320 000E3260  90 01 00 24 */	stw r0, 0x24(r1)
lbl_800E6324:
/* 800E6324 000E3264  80 01 00 24 */	lwz r0, 0x24(r1)
/* 800E6328 000E3268  2C 04 00 01 */	cmpwi r4, 1
/* 800E632C 000E326C  90 01 00 34 */	stw r0, 0x34(r1)
/* 800E6330 000E3270  41 82 00 1C */	beq lbl_800E634C
/* 800E6334 000E3274  40 80 00 2C */	bge lbl_800E6360
/* 800E6338 000E3278  2C 04 00 00 */	cmpwi r4, 0
/* 800E633C 000E327C  40 80 00 08 */	bge lbl_800E6344
/* 800E6340 000E3280  48 00 00 20 */	b lbl_800E6360
lbl_800E6344:
/* 800E6344 000E3284  80 01 00 34 */	lwz r0, 0x34(r1)
/* 800E6348 000E3288  48 00 00 1C */	b lbl_800E6364
lbl_800E634C:
/* 800E634C 000E328C  C0 01 00 34 */	lfs f0, 0x34(r1)
/* 800E6350 000E3290  FC 00 00 1E */	fctiwz f0, f0
/* 800E6354 000E3294  D8 01 00 40 */	stfd f0, 0x40(r1)
/* 800E6358 000E3298  80 01 00 44 */	lwz r0, 0x44(r1)
/* 800E635C 000E329C  48 00 00 08 */	b lbl_800E6364
lbl_800E6360:
/* 800E6360 000E32A0  38 00 00 00 */	li r0, 0
lbl_800E6364:
/* 800E6364 000E32A4  2C 1E 00 01 */	cmpwi r30, 1
/* 800E6368 000E32A8  7C 04 03 78 */	mr r4, r0
/* 800E636C 000E32AC  41 82 00 1C */	beq lbl_800E6388
/* 800E6370 000E32B0  40 80 00 2C */	bge lbl_800E639C
/* 800E6374 000E32B4  2C 1E 00 00 */	cmpwi r30, 0
/* 800E6378 000E32B8  40 80 00 08 */	bge lbl_800E6380
/* 800E637C 000E32BC  48 00 00 20 */	b lbl_800E639C
lbl_800E6380:
/* 800E6380 000E32C0  80 01 00 3C */	lwz r0, 0x3c(r1)
/* 800E6384 000E32C4  48 00 00 1C */	b lbl_800E63A0
lbl_800E6388:
/* 800E6388 000E32C8  C0 01 00 3C */	lfs f0, 0x3c(r1)
/* 800E638C 000E32CC  FC 00 00 1E */	fctiwz f0, f0
/* 800E6390 000E32D0  D8 01 00 40 */	stfd f0, 0x40(r1)
/* 800E6394 000E32D4  80 01 00 44 */	lwz r0, 0x44(r1)
/* 800E6398 000E32D8  48 00 00 08 */	b lbl_800E63A0
lbl_800E639C:
/* 800E639C 000E32DC  38 00 00 00 */	li r0, 0
lbl_800E63A0:
/* 800E63A0 000E32E0  80 6D 97 D0 */	lwz r3, smInstance__12TFlagManager@sda21(r13)
/* 800E63A4 000E32E4  7C 05 03 78 */	mr r5, r0
/* 800E63A8 000E32E8  48 00 1D 31 */	bl setFlag__12TFlagManagerFUll
/* 800E63AC 000E32EC  38 00 00 00 */	li r0, 0
/* 800E63B0 000E32F0  90 01 00 18 */	stw r0, 0x18(r1)
/* 800E63B4 000E32F4  90 01 00 1C */	stw r0, 0x1c(r1)
/* 800E63B8 000E32F8  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E63BC 000E32FC  80 1D 00 18 */	lwz r0, 0x18(r29)
/* 800E63C0 000E3300  7C 03 00 00 */	cmpw r3, r0
/* 800E63C4 000E3304  41 80 00 14 */	blt lbl_800E63D8
/* 800E63C8 000E3308  38 7F 07 8C */	addi r3, r31, 0x78c
/* 800E63CC 000E330C  4C C6 31 82 */	crclr 6
/* 800E63D0 000E3310  4B FF 6C D5 */	bl SpcTrace__FPCce
/* 800E63D4 000E3314  48 00 00 2C */	b lbl_800E6400
lbl_800E63D8:
/* 800E63D8 000E3318  80 BD 00 20 */	lwz r5, 0x20(r29)
/* 800E63DC 000E331C  54 64 18 38 */	slwi r4, r3, 3
/* 800E63E0 000E3320  80 61 00 18 */	lwz r3, 0x18(r1)
/* 800E63E4 000E3324  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 800E63E8 000E3328  7C 85 22 14 */	add r4, r5, r4
/* 800E63EC 000E332C  90 64 00 00 */	stw r3, 0(r4)
/* 800E63F0 000E3330  90 04 00 04 */	stw r0, 4(r4)
/* 800E63F4 000E3334  80 7D 00 1C */	lwz r3, 0x1c(r29)
/* 800E63F8 000E3338  38 03 00 01 */	addi r0, r3, 1
/* 800E63FC 000E333C  90 1D 00 1C */	stw r0, 0x1c(r29)
lbl_800E6400:
/* 800E6400 000E3340  80 01 00 5C */	lwz r0, 0x5c(r1)
/* 800E6404 000E3344  83 E1 00 54 */	lwz r31, 0x54(r1)
/* 800E6408 000E3348  83 C1 00 50 */	lwz r30, 0x50(r1)
/* 800E640C 000E334C  7C 08 03 A6 */	mtlr r0
/* 800E6410 000E3350  83 A1 00 4C */	lwz r29, 0x4c(r1)
/* 800E6414 000E3354  38 21 00 58 */	addi r1, r1, 0x58
/* 800E6418 000E3358  4E 80 00 20 */	blr 

.global evGetSystemFlag__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul
evGetSystemFlag__FP32TSpcTypedInterp$$013TEventWatcher$$1Ul:
/* 800E641C 000E335C  7C 08 02 A6 */	mflr r0
/* 800E6420 000E3360  90 01 00 04 */	stw r0, 4(r1)
/* 800E6424 000E3364  94 21 FF C0 */	stwu r1, -0x40(r1)
/* 800E6428 000E3368  93 E1 00 3C */	stw r31, 0x3c(r1)
/* 800E642C 000E336C  7C 7F 1B 78 */	mr r31, r3
/* 800E6430 000E3370  38 A1 00 0C */	addi r5, r1, 0xc
/* 800E6434 000E3374  90 81 00 0C */	stw r4, 0xc(r1)
/* 800E6438 000E3378  38 80 00 01 */	li r4, 1
/* 800E643C 000E337C  48 03 0F F5 */	bl verifyArgNum__10TSpcInterpFUlPUl
/* 800E6440 000E3380  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E6444 000E3384  2C 03 00 00 */	cmpwi r3, 0
/* 800E6448 000E3388  41 81 00 34 */	bgt lbl_800E647C
/* 800E644C 000E338C  3C 60 80 37 */	lis r3, $$23829@ha
/* 800E6450 000E3390  4C C6 31 82 */	crclr 6
/* 800E6454 000E3394  38 63 30 FC */	addi r3, r3, $$23829@l
/* 800E6458 000E3398  4B FF 6C 4D */	bl SpcTrace__FPCce
/* 800E645C 000E339C  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800E6460 000E33A0  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800E6464 000E33A4  54 00 18 38 */	slwi r0, r0, 3
/* 800E6468 000E33A8  7C 63 02 14 */	add r3, r3, r0
/* 800E646C 000E33AC  80 83 00 00 */	lwz r4, 0(r3)
/* 800E6470 000E33B0  80 03 00 04 */	lwz r0, 4(r3)
/* 800E6474 000E33B4  90 01 00 20 */	stw r0, 0x20(r1)
/* 800E6478 000E33B8  48 00 00 28 */	b lbl_800E64A0
lbl_800E647C:
/* 800E647C 000E33BC  38 03 FF FF */	addi r0, r3, -1
/* 800E6480 000E33C0  90 1F 00 1C */	stw r0, 0x1c(r31)
/* 800E6484 000E33C4  80 1F 00 1C */	lwz r0, 0x1c(r31)
/* 800E6488 000E33C8  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 800E648C 000E33CC  54 00 18 38 */	slwi r0, r0, 3
/* 800E6490 000E33D0  7C 63 02 14 */	add r3, r3, r0
/* 800E6494 000E33D4  80 83 00 00 */	lwz r4, 0(r3)
/* 800E6498 000E33D8  80 03 00 04 */	lwz r0, 4(r3)
/* 800E649C 000E33DC  90 01 00 20 */	stw r0, 0x20(r1)
lbl_800E64A0:
/* 800E64A0 000E33E0  80 01 00 20 */	lwz r0, 0x20(r1)
/* 800E64A4 000E33E4  2C 04 00 01 */	cmpwi r4, 1
/* 800E64A8 000E33E8  90 01 00 28 */	stw r0, 0x28(r1)
/* 800E64AC 000E33EC  41 82 00 1C */	beq lbl_800E64C8
/* 800E64B0 000E33F0  40 80 00 2C */	bge lbl_800E64DC
/* 800E64B4 000E33F4  2C 04 00 00 */	cmpwi r4, 0
/* 800E64B8 000E33F8  40 80 00 08 */	bge lbl_800E64C0
/* 800E64BC 000E33FC  48 00 00 20 */	b lbl_800E64DC
lbl_800E64C0:
/* 800E64C0 000E3400  80 01 00 28 */	lwz r0, 0x28(r1)
/* 800E64C4 000E3404  48 00 00 1C */	b lbl_800E64E0
lbl_800E64C8:
/* 800E64C8 000E3408  C0 01 00 28 */	lfs f0, 0x28(r1)
/* 800E64CC 000E340C  FC 00 00 1E */	fctiwz f0, f0
/* 800E64D0 000E3410  D8 01 00 30 */	stfd f0, 0x30(r1)
/* 800E64D4 000E3414  80 01 00 34 */	lwz r0, 0x34(r1)
/* 800E64D8 000E3418  48 00 00 08 */	b lbl_800E64E0
lbl_800E64DC:
/* 800E64DC 000E341C  38 00 00 00 */	li r0, 0
lbl_800E64E0:
/* 800E64E0 000E3420  80 6D 97 D0 */	lwz r3, smInstance__12TFlagManager@sda21(r13)
/* 800E64E4 000E3424  7C 04 03 78 */	mr r4, r0
/* 800E64E8 000E3428  48 00 1E 21 */	bl getFlag__12TFlagManagerCFUl
/* 800E64EC 000E342C  38 00 00 00 */	li r0, 0
/* 800E64F0 000E3430  90 01 00 14 */	stw r0, 0x14(r1)
/* 800E64F4 000E3434  90 61 00 18 */	stw r3, 0x18(r1)
/* 800E64F8 000E3438  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E64FC 000E343C  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 800E6500 000E3440  7C 03 00 00 */	cmpw r3, r0
/* 800E6504 000E3444  41 80 00 18 */	blt lbl_800E651C
/* 800E6508 000E3448  3C 60 80 37 */	lis r3, $$23834@ha
/* 800E650C 000E344C  4C C6 31 82 */	crclr 6
/* 800E6510 000E3450  38 63 31 1C */	addi r3, r3, $$23834@l
/* 800E6514 000E3454  4B FF 6B 91 */	bl SpcTrace__FPCce
/* 800E6518 000E3458  48 00 00 2C */	b lbl_800E6544
lbl_800E651C:
/* 800E651C 000E345C  80 BF 00 20 */	lwz r5, 0x20(r31)
/* 800E6520 000E3460  54 64 18 38 */	slwi r4, r3, 3
/* 800E6524 000E3464  80 61 00 14 */	lwz r3, 0x14(r1)
/* 800E6528 000E3468  80 01 00 18 */	lwz r0, 0x18(r1)
/* 800E652C 000E346C  7C 85 22 14 */	add r4, r5, r4
/* 800E6530 000E3470  90 64 00 00 */	stw r3, 0(r4)
/* 800E6534 000E3474  90 04 00 04 */	stw r0, 4(r4)
/* 800E6538 000E3478  80 7F 00 1C */	lwz r3, 0x1c(r31)
/* 800E653C 000E347C  38 03 00 01 */	addi r0, r3, 1
/* 800E6540 000E3480  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_800E6544:
/* 800E6544 000E3484  80 01 00 44 */	lwz r0, 0x44(r1)
/* 800E6548 000E3488  83 E1 00 3C */	lwz r31, 0x3c(r1)
/* 800E654C 000E348C  38 21 00 40 */	addi r1, r1, 0x40
/* 800E6550 000E3490  7C 08 03 A6 */	mtlr r0
/* 800E6554 000E3494  4E 80 00 20 */	blr 

.global __dt__32TSpcTypedInterp$$013TEventWatcher$$1Fv
__dt__32TSpcTypedInterp$$013TEventWatcher$$1Fv:
/* 800E6558 000E3498  7C 08 02 A6 */	mflr r0
/* 800E655C 000E349C  90 01 00 04 */	stw r0, 4(r1)
/* 800E6560 000E34A0  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 800E6564 000E34A4  93 E1 00 14 */	stw r31, 0x14(r1)
/* 800E6568 000E34A8  3B E4 00 00 */	addi r31, r4, 0
/* 800E656C 000E34AC  93 C1 00 10 */	stw r30, 0x10(r1)
/* 800E6570 000E34B0  7C 7E 1B 79 */	or. r30, r3, r3
/* 800E6574 000E34B4  41 82 00 2C */	beq lbl_800E65A0
/* 800E6578 000E34B8  3C 60 80 3B */	lis r3, __vt__32TSpcTypedInterp$$013TEventWatcher$$1@ha
/* 800E657C 000E34BC  38 03 39 68 */	addi r0, r3, __vt__32TSpcTypedInterp$$013TEventWatcher$$1@l
/* 800E6580 000E34C0  90 1E 00 5C */	stw r0, 0x5c(r30)
/* 800E6584 000E34C4  38 7E 00 00 */	addi r3, r30, 0
/* 800E6588 000E34C8  38 80 00 00 */	li r4, 0
/* 800E658C 000E34CC  48 03 12 5D */	bl __dt__10TSpcInterpFv
/* 800E6590 000E34D0  7F E0 07 35 */	extsh. r0, r31
/* 800E6594 000E34D4  40 81 00 0C */	ble lbl_800E65A0
/* 800E6598 000E34D8  7F C3 F3 78 */	mr r3, r30
/* 800E659C 000E34DC  4B F2 65 15 */	bl __dl__FPv
lbl_800E65A0:
/* 800E65A0 000E34E0  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 800E65A4 000E34E4  7F C3 F3 78 */	mr r3, r30
/* 800E65A8 000E34E8  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 800E65AC 000E34EC  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 800E65B0 000E34F0  7C 08 03 A6 */	mtlr r0
/* 800E65B4 000E34F4  38 21 00 18 */	addi r1, r1, 0x18
/* 800E65B8 000E34F8  4E 80 00 20 */	blr 

.global __dt__32TSpcTypedBinary$$013TEventWatcher$$1Fv
__dt__32TSpcTypedBinary$$013TEventWatcher$$1Fv:
/* 800E65BC 000E34FC  7C 08 02 A6 */	mflr r0
/* 800E65C0 000E3500  90 01 00 04 */	stw r0, 4(r1)
/* 800E65C4 000E3504  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 800E65C8 000E3508  93 E1 00 14 */	stw r31, 0x14(r1)
/* 800E65CC 000E350C  3B E4 00 00 */	addi r31, r4, 0
/* 800E65D0 000E3510  93 C1 00 10 */	stw r30, 0x10(r1)
/* 800E65D4 000E3514  7C 7E 1B 79 */	or. r30, r3, r3
/* 800E65D8 000E3518  41 82 00 2C */	beq lbl_800E6604
/* 800E65DC 000E351C  3C 60 80 3B */	lis r3, __vt__32TSpcTypedBinary$$013TEventWatcher$$1@ha
/* 800E65E0 000E3520  38 03 39 7C */	addi r0, r3, __vt__32TSpcTypedBinary$$013TEventWatcher$$1@l
/* 800E65E4 000E3524  90 1E 00 04 */	stw r0, 4(r30)
/* 800E65E8 000E3528  38 7E 00 00 */	addi r3, r30, 0
/* 800E65EC 000E352C  38 80 00 00 */	li r4, 0
/* 800E65F0 000E3530  48 03 58 51 */	bl __dt__10TSpcBinaryFv
/* 800E65F4 000E3534  7F E0 07 35 */	extsh. r0, r31
/* 800E65F8 000E3538  40 81 00 0C */	ble lbl_800E6604
/* 800E65FC 000E353C  7F C3 F3 78 */	mr r3, r30
/* 800E6600 000E3540  4B F2 64 B1 */	bl __dl__FPv
lbl_800E6604:
/* 800E6604 000E3544  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 800E6608 000E3548  7F C3 F3 78 */	mr r3, r30
/* 800E660C 000E354C  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 800E6610 000E3550  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 800E6614 000E3554  7C 08 03 A6 */	mtlr r0
/* 800E6618 000E3558  38 21 00 18 */	addi r1, r1, 0x18
/* 800E661C 000E355C  4E 80 00 20 */	blr 

.global dispatchBuiltin__32TSpcTypedInterp$$013TEventWatcher$$1FUlUl
dispatchBuiltin__32TSpcTypedInterp$$013TEventWatcher$$1FUlUl:
/* 800E6620 000E3560  7C 08 02 A6 */	mflr r0
/* 800E6624 000E3564  90 01 00 04 */	stw r0, 4(r1)
/* 800E6628 000E3568  1C 04 00 14 */	mulli r0, r4, 0x14
/* 800E662C 000E356C  94 21 FF F8 */	stwu r1, -8(r1)
/* 800E6630 000E3570  80 C3 00 00 */	lwz r6, 0(r3)
/* 800E6634 000E3574  81 06 00 00 */	lwz r8, 0(r6)
/* 800E6638 000E3578  80 C8 00 10 */	lwz r6, 0x10(r8)
/* 800E663C 000E357C  7C E6 02 14 */	add r7, r6, r0
/* 800E6640 000E3580  7C E8 3A 15 */	add. r7, r8, r7
/* 800E6644 000E3584  41 82 00 40 */	beq lbl_800E6684
/* 800E6648 000E3588  80 07 00 10 */	lwz r0, 0x10(r7)
/* 800E664C 000E358C  28 00 00 00 */	cmplwi r0, 0
/* 800E6650 000E3590  7C 0C 03 78 */	mr r12, r0
/* 800E6654 000E3594  41 82 00 30 */	beq lbl_800E6684
/* 800E6658 000E3598  80 88 00 14 */	lwz r4, 0x14(r8)
/* 800E665C 000E359C  7D 88 03 A6 */	mtlr r12
/* 800E6660 000E35A0  80 07 00 04 */	lwz r0, 4(r7)
/* 800E6664 000E35A4  1C 84 00 14 */	mulli r4, r4, 0x14
/* 800E6668 000E35A8  7C 86 22 14 */	add r4, r6, r4
/* 800E666C 000E35AC  7C 04 02 14 */	add r0, r4, r0
/* 800E6670 000E35B0  7C 08 02 14 */	add r0, r8, r0
/* 800E6674 000E35B4  90 03 00 58 */	stw r0, 0x58(r3)
/* 800E6678 000E35B8  7C A4 2B 78 */	mr r4, r5
/* 800E667C 000E35BC  4E 80 00 21 */	blrl 
/* 800E6680 000E35C0  48 00 00 08 */	b lbl_800E6688
lbl_800E6684:
/* 800E6684 000E35C4  48 03 14 F1 */	bl dispatchBuiltin__10TSpcInterpFUlUl
lbl_800E6688:
/* 800E6688 000E35C8  80 01 00 0C */	lwz r0, 0xc(r1)
/* 800E668C 000E35CC  38 21 00 08 */	addi r1, r1, 8
/* 800E6690 000E35D0  7C 08 03 A6 */	mtlr r0
/* 800E6694 000E35D4  4E 80 00 20 */	blr 

.global __sinit_EventWatcher_cpp
__sinit_EventWatcher_cpp:
/* 800E6698 000E35D8  7C 08 02 A6 */	mflr r0
/* 800E669C 000E35DC  3C 60 80 3F */	lis r3, $$27636@ha
/* 800E66A0 000E35E0  90 01 00 04 */	stw r0, 4(r1)
/* 800E66A4 000E35E4  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 800E66A8 000E35E8  93 E1 00 0C */	stw r31, 0xc(r1)
/* 800E66AC 000E35EC  3B E3 1D D0 */	addi r31, r3, $$27636@l
/* 800E66B0 000E35F0  88 0D 97 CC */	lbz r0, __init__smList__15JALList$$05MSBgm$$1@sda21(r13)
/* 800E66B4 000E35F4  7C 00 07 75 */	extsb. r0, r0
/* 800E66B8 000E35F8  40 82 00 28 */	bne lbl_800E66E0
/* 800E66BC 000E35FC  38 6D 97 A8 */	addi r3, r13, smList__15JALList$$05MSBgm$$1@sda21
/* 800E66C0 000E3600  4B F2 85 75 */	bl initiate__10JSUPtrListFv
/* 800E66C4 000E3604  3C 60 80 0E */	lis r3, __dt__15JSUList$$05MSBgm$$1Fv@ha
/* 800E66C8 000E3608  38 83 6A 44 */	addi r4, r3, __dt__15JSUList$$05MSBgm$$1Fv@l
/* 800E66CC 000E360C  38 6D 97 A8 */	addi r3, r13, smList__15JALList$$05MSBgm$$1@sda21
/* 800E66D0 000E3610  38 BF 00 00 */	addi r5, r31, 0
/* 800E66D4 000E3614  4B F9 C0 55 */	bl __register_global_object
/* 800E66D8 000E3618  38 00 00 01 */	li r0, 1
/* 800E66DC 000E361C  98 0D 97 CC */	stb r0, __init__smList__15JALList$$05MSBgm$$1@sda21(r13)
lbl_800E66E0:
/* 800E66E0 000E3620  88 0D 97 CD */	lbz r0, __init__smList__24JALList$$013MSSetSoundGrp$$1@sda21(r13)
/* 800E66E4 000E3624  7C 00 07 75 */	extsb. r0, r0
/* 800E66E8 000E3628  40 82 00 28 */	bne lbl_800E6710
/* 800E66EC 000E362C  38 6D 97 B4 */	addi r3, r13, smList__24JALList$$013MSSetSoundGrp$$1@sda21
/* 800E66F0 000E3630  4B F2 85 45 */	bl initiate__10JSUPtrListFv
/* 800E66F4 000E3634  3C 60 80 0E */	lis r3, __dt__24JSUList$$013MSSetSoundGrp$$1Fv@ha
/* 800E66F8 000E3638  38 83 69 EC */	addi r4, r3, __dt__24JSUList$$013MSSetSoundGrp$$1Fv@l
/* 800E66FC 000E363C  38 6D 97 B4 */	addi r3, r13, smList__24JALList$$013MSSetSoundGrp$$1@sda21
/* 800E6700 000E3640  38 BF 00 0C */	addi r5, r31, 0xc
/* 800E6704 000E3644  4B F9 C0 25 */	bl __register_global_object
/* 800E6708 000E3648  38 00 00 01 */	li r0, 1
/* 800E670C 000E364C  98 0D 97 CD */	stb r0, __init__smList__24JALList$$013MSSetSoundGrp$$1@sda21(r13)
lbl_800E6710:
/* 800E6710 000E3650  88 0D 97 CE */	lbz r0, __init__smList__21JALList$$010MSSetSound$$1@sda21(r13)
/* 800E6714 000E3654  7C 00 07 75 */	extsb. r0, r0
/* 800E6718 000E3658  40 82 00 28 */	bne lbl_800E6740
/* 800E671C 000E365C  38 6D 97 C0 */	addi r3, r13, smList__21JALList$$010MSSetSound$$1@sda21
/* 800E6720 000E3660  4B F2 85 15 */	bl initiate__10JSUPtrListFv
/* 800E6724 000E3664  3C 60 80 0E */	lis r3, __dt__21JSUList$$010MSSetSound$$1Fv@ha
/* 800E6728 000E3668  38 83 69 94 */	addi r4, r3, __dt__21JSUList$$010MSSetSound$$1Fv@l
/* 800E672C 000E366C  38 6D 97 C0 */	addi r3, r13, smList__21JALList$$010MSSetSound$$1@sda21
/* 800E6730 000E3670  38 BF 00 18 */	addi r5, r31, 0x18
/* 800E6734 000E3674  4B F9 BF F5 */	bl __register_global_object
/* 800E6738 000E3678  38 00 00 01 */	li r0, 1
/* 800E673C 000E367C  98 0D 97 CE */	stb r0, __init__smList__21JALList$$010MSSetSound$$1@sda21(r13)
lbl_800E6740:
/* 800E6740 000E3680  88 0D 8F 8C */	lbz r0, __init__smList__26JALList$$015JALSeModEffDGrp$$1@sda21(r13)
/* 800E6744 000E3684  7C 00 07 75 */	extsb. r0, r0
/* 800E6748 000E3688  40 82 00 28 */	bne lbl_800E6770
/* 800E674C 000E368C  38 6D 8E FC */	addi r3, r13, smList__26JALList$$015JALSeModEffDGrp$$1@sda21
/* 800E6750 000E3690  4B F2 84 E5 */	bl initiate__10JSUPtrListFv
/* 800E6754 000E3694  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffDGrp$$1Fv@ha
/* 800E6758 000E3698  38 83 A8 0C */	addi r4, r3, __dt__26JSUList$$015JALSeModEffDGrp$$1Fv@l
/* 800E675C 000E369C  38 6D 8E FC */	addi r3, r13, smList__26JALList$$015JALSeModEffDGrp$$1@sda21
/* 800E6760 000E36A0  38 BF 00 24 */	addi r5, r31, 0x24
/* 800E6764 000E36A4  4B F9 BF C5 */	bl __register_global_object
/* 800E6768 000E36A8  38 00 00 01 */	li r0, 1
/* 800E676C 000E36AC  98 0D 8F 8C */	stb r0, __init__smList__26JALList$$015JALSeModEffDGrp$$1@sda21(r13)
lbl_800E6770:
/* 800E6770 000E36B0  88 0D 8F 8D */	lbz r0, __init__smList__26JALList$$015JALSeModPitDGrp$$1@sda21(r13)
/* 800E6774 000E36B4  7C 00 07 75 */	extsb. r0, r0
/* 800E6778 000E36B8  40 82 00 28 */	bne lbl_800E67A0
/* 800E677C 000E36BC  38 6D 8F 08 */	addi r3, r13, smList__26JALList$$015JALSeModPitDGrp$$1@sda21
/* 800E6780 000E36C0  4B F2 84 B5 */	bl initiate__10JSUPtrListFv
/* 800E6784 000E36C4  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitDGrp$$1Fv@ha
/* 800E6788 000E36C8  38 83 A7 B4 */	addi r4, r3, __dt__26JSUList$$015JALSeModPitDGrp$$1Fv@l
/* 800E678C 000E36CC  38 6D 8F 08 */	addi r3, r13, smList__26JALList$$015JALSeModPitDGrp$$1@sda21
/* 800E6790 000E36D0  38 BF 00 30 */	addi r5, r31, 0x30
/* 800E6794 000E36D4  4B F9 BF 95 */	bl __register_global_object
/* 800E6798 000E36D8  38 00 00 01 */	li r0, 1
/* 800E679C 000E36DC  98 0D 8F 8D */	stb r0, __init__smList__26JALList$$015JALSeModPitDGrp$$1@sda21(r13)
lbl_800E67A0:
/* 800E67A0 000E36E0  88 0D 8F 8E */	lbz r0, __init__smList__26JALList$$015JALSeModVolDGrp$$1@sda21(r13)
/* 800E67A4 000E36E4  7C 00 07 75 */	extsb. r0, r0
/* 800E67A8 000E36E8  40 82 00 28 */	bne lbl_800E67D0
/* 800E67AC 000E36EC  38 6D 8F 14 */	addi r3, r13, smList__26JALList$$015JALSeModVolDGrp$$1@sda21
/* 800E67B0 000E36F0  4B F2 84 85 */	bl initiate__10JSUPtrListFv
/* 800E67B4 000E36F4  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolDGrp$$1Fv@ha
/* 800E67B8 000E36F8  38 83 A7 5C */	addi r4, r3, __dt__26JSUList$$015JALSeModVolDGrp$$1Fv@l
/* 800E67BC 000E36FC  38 6D 8F 14 */	addi r3, r13, smList__26JALList$$015JALSeModVolDGrp$$1@sda21
/* 800E67C0 000E3700  38 BF 00 3C */	addi r5, r31, 0x3c
/* 800E67C4 000E3704  4B F9 BF 65 */	bl __register_global_object
/* 800E67C8 000E3708  38 00 00 01 */	li r0, 1
/* 800E67CC 000E370C  98 0D 8F 8E */	stb r0, __init__smList__26JALList$$015JALSeModVolDGrp$$1@sda21(r13)
lbl_800E67D0:
/* 800E67D0 000E3710  88 0D 8F 8F */	lbz r0, __init__smList__26JALList$$015JALSeModEffFGrp$$1@sda21(r13)
/* 800E67D4 000E3714  7C 00 07 75 */	extsb. r0, r0
/* 800E67D8 000E3718  40 82 00 28 */	bne lbl_800E6800
/* 800E67DC 000E371C  38 6D 8F 20 */	addi r3, r13, smList__26JALList$$015JALSeModEffFGrp$$1@sda21
/* 800E67E0 000E3720  4B F2 84 55 */	bl initiate__10JSUPtrListFv
/* 800E67E4 000E3724  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffFGrp$$1Fv@ha
/* 800E67E8 000E3728  38 83 A7 04 */	addi r4, r3, __dt__26JSUList$$015JALSeModEffFGrp$$1Fv@l
/* 800E67EC 000E372C  38 6D 8F 20 */	addi r3, r13, smList__26JALList$$015JALSeModEffFGrp$$1@sda21
/* 800E67F0 000E3730  38 BF 00 48 */	addi r5, r31, 0x48
/* 800E67F4 000E3734  4B F9 BF 35 */	bl __register_global_object
/* 800E67F8 000E3738  38 00 00 01 */	li r0, 1
/* 800E67FC 000E373C  98 0D 8F 8F */	stb r0, __init__smList__26JALList$$015JALSeModEffFGrp$$1@sda21(r13)
lbl_800E6800:
/* 800E6800 000E3740  88 0D 8F 90 */	lbz r0, __init__smList__26JALList$$015JALSeModPitFGrp$$1@sda21(r13)
/* 800E6804 000E3744  7C 00 07 75 */	extsb. r0, r0
/* 800E6808 000E3748  40 82 00 28 */	bne lbl_800E6830
/* 800E680C 000E374C  38 6D 8F 2C */	addi r3, r13, smList__26JALList$$015JALSeModPitFGrp$$1@sda21
/* 800E6810 000E3750  4B F2 84 25 */	bl initiate__10JSUPtrListFv
/* 800E6814 000E3754  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitFGrp$$1Fv@ha
/* 800E6818 000E3758  38 83 A6 AC */	addi r4, r3, __dt__26JSUList$$015JALSeModPitFGrp$$1Fv@l
/* 800E681C 000E375C  38 6D 8F 2C */	addi r3, r13, smList__26JALList$$015JALSeModPitFGrp$$1@sda21
/* 800E6820 000E3760  38 BF 00 54 */	addi r5, r31, 0x54
/* 800E6824 000E3764  4B F9 BF 05 */	bl __register_global_object
/* 800E6828 000E3768  38 00 00 01 */	li r0, 1
/* 800E682C 000E376C  98 0D 8F 90 */	stb r0, __init__smList__26JALList$$015JALSeModPitFGrp$$1@sda21(r13)
lbl_800E6830:
/* 800E6830 000E3770  88 0D 8F 91 */	lbz r0, __init__smList__26JALList$$015JALSeModVolFGrp$$1@sda21(r13)
/* 800E6834 000E3774  7C 00 07 75 */	extsb. r0, r0
/* 800E6838 000E3778  40 82 00 28 */	bne lbl_800E6860
/* 800E683C 000E377C  38 6D 8F 38 */	addi r3, r13, smList__26JALList$$015JALSeModVolFGrp$$1@sda21
/* 800E6840 000E3780  4B F2 83 F5 */	bl initiate__10JSUPtrListFv
/* 800E6844 000E3784  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolFGrp$$1Fv@ha
/* 800E6848 000E3788  38 83 A6 54 */	addi r4, r3, __dt__26JSUList$$015JALSeModVolFGrp$$1Fv@l
/* 800E684C 000E378C  38 6D 8F 38 */	addi r3, r13, smList__26JALList$$015JALSeModVolFGrp$$1@sda21
/* 800E6850 000E3790  38 BF 00 60 */	addi r5, r31, 0x60
/* 800E6854 000E3794  4B F9 BE D5 */	bl __register_global_object
/* 800E6858 000E3798  38 00 00 01 */	li r0, 1
/* 800E685C 000E379C  98 0D 8F 91 */	stb r0, __init__smList__26JALList$$015JALSeModVolFGrp$$1@sda21(r13)
lbl_800E6860:
/* 800E6860 000E37A0  88 0D 8F 92 */	lbz r0, __init__smList__26JALList$$015JALSeModEffDist$$1@sda21(r13)
/* 800E6864 000E37A4  7C 00 07 75 */	extsb. r0, r0
/* 800E6868 000E37A8  40 82 00 28 */	bne lbl_800E6890
/* 800E686C 000E37AC  38 6D 8F 44 */	addi r3, r13, smList__26JALList$$015JALSeModEffDist$$1@sda21
/* 800E6870 000E37B0  4B F2 83 C5 */	bl initiate__10JSUPtrListFv
/* 800E6874 000E37B4  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffDist$$1Fv@ha
/* 800E6878 000E37B8  38 83 A5 FC */	addi r4, r3, __dt__26JSUList$$015JALSeModEffDist$$1Fv@l
/* 800E687C 000E37BC  38 6D 8F 44 */	addi r3, r13, smList__26JALList$$015JALSeModEffDist$$1@sda21
/* 800E6880 000E37C0  38 BF 00 6C */	addi r5, r31, 0x6c
/* 800E6884 000E37C4  4B F9 BE A5 */	bl __register_global_object
/* 800E6888 000E37C8  38 00 00 01 */	li r0, 1
/* 800E688C 000E37CC  98 0D 8F 92 */	stb r0, __init__smList__26JALList$$015JALSeModEffDist$$1@sda21(r13)
lbl_800E6890:
/* 800E6890 000E37D0  88 0D 8F 93 */	lbz r0, __init__smList__26JALList$$015JALSeModPitDist$$1@sda21(r13)
/* 800E6894 000E37D4  7C 00 07 75 */	extsb. r0, r0
/* 800E6898 000E37D8  40 82 00 28 */	bne lbl_800E68C0
/* 800E689C 000E37DC  38 6D 8F 50 */	addi r3, r13, smList__26JALList$$015JALSeModPitDist$$1@sda21
/* 800E68A0 000E37E0  4B F2 83 95 */	bl initiate__10JSUPtrListFv
/* 800E68A4 000E37E4  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitDist$$1Fv@ha
/* 800E68A8 000E37E8  38 83 A5 A4 */	addi r4, r3, __dt__26JSUList$$015JALSeModPitDist$$1Fv@l
/* 800E68AC 000E37EC  38 6D 8F 50 */	addi r3, r13, smList__26JALList$$015JALSeModPitDist$$1@sda21
/* 800E68B0 000E37F0  38 BF 00 78 */	addi r5, r31, 0x78
/* 800E68B4 000E37F4  4B F9 BE 75 */	bl __register_global_object
/* 800E68B8 000E37F8  38 00 00 01 */	li r0, 1
/* 800E68BC 000E37FC  98 0D 8F 93 */	stb r0, __init__smList__26JALList$$015JALSeModPitDist$$1@sda21(r13)
lbl_800E68C0:
/* 800E68C0 000E3800  88 0D 8F 94 */	lbz r0, __init__smList__26JALList$$015JALSeModVolDist$$1@sda21(r13)
/* 800E68C4 000E3804  7C 00 07 75 */	extsb. r0, r0
/* 800E68C8 000E3808  40 82 00 28 */	bne lbl_800E68F0
/* 800E68CC 000E380C  38 6D 8F 5C */	addi r3, r13, smList__26JALList$$015JALSeModVolDist$$1@sda21
/* 800E68D0 000E3810  4B F2 83 65 */	bl initiate__10JSUPtrListFv
/* 800E68D4 000E3814  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolDist$$1Fv@ha
/* 800E68D8 000E3818  38 83 A5 4C */	addi r4, r3, __dt__26JSUList$$015JALSeModVolDist$$1Fv@l
/* 800E68DC 000E381C  38 6D 8F 5C */	addi r3, r13, smList__26JALList$$015JALSeModVolDist$$1@sda21
/* 800E68E0 000E3820  38 BF 00 84 */	addi r5, r31, 0x84
/* 800E68E4 000E3824  4B F9 BE 45 */	bl __register_global_object
/* 800E68E8 000E3828  38 00 00 01 */	li r0, 1
/* 800E68EC 000E382C  98 0D 8F 94 */	stb r0, __init__smList__26JALList$$015JALSeModVolDist$$1@sda21(r13)
lbl_800E68F0:
/* 800E68F0 000E3830  88 0D 8F 95 */	lbz r0, __init__smList__26JALList$$015JALSeModEffFunk$$1@sda21(r13)
/* 800E68F4 000E3834  7C 00 07 75 */	extsb. r0, r0
/* 800E68F8 000E3838  40 82 00 28 */	bne lbl_800E6920
/* 800E68FC 000E383C  38 6D 8F 68 */	addi r3, r13, smList__26JALList$$015JALSeModEffFunk$$1@sda21
/* 800E6900 000E3840  4B F2 83 35 */	bl initiate__10JSUPtrListFv
/* 800E6904 000E3844  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffFunk$$1Fv@ha
/* 800E6908 000E3848  38 83 A4 F4 */	addi r4, r3, __dt__26JSUList$$015JALSeModEffFunk$$1Fv@l
/* 800E690C 000E384C  38 6D 8F 68 */	addi r3, r13, smList__26JALList$$015JALSeModEffFunk$$1@sda21
/* 800E6910 000E3850  38 BF 00 90 */	addi r5, r31, 0x90
/* 800E6914 000E3854  4B F9 BE 15 */	bl __register_global_object
/* 800E6918 000E3858  38 00 00 01 */	li r0, 1
/* 800E691C 000E385C  98 0D 8F 95 */	stb r0, __init__smList__26JALList$$015JALSeModEffFunk$$1@sda21(r13)
lbl_800E6920:
/* 800E6920 000E3860  88 0D 8F 96 */	lbz r0, __init__smList__26JALList$$015JALSeModPitFunk$$1@sda21(r13)
/* 800E6924 000E3864  7C 00 07 75 */	extsb. r0, r0
/* 800E6928 000E3868  40 82 00 28 */	bne lbl_800E6950
/* 800E692C 000E386C  38 6D 8F 74 */	addi r3, r13, smList__26JALList$$015JALSeModPitFunk$$1@sda21
/* 800E6930 000E3870  4B F2 83 05 */	bl initiate__10JSUPtrListFv
/* 800E6934 000E3874  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitFunk$$1Fv@ha
/* 800E6938 000E3878  38 83 A4 9C */	addi r4, r3, __dt__26JSUList$$015JALSeModPitFunk$$1Fv@l
/* 800E693C 000E387C  38 6D 8F 74 */	addi r3, r13, smList__26JALList$$015JALSeModPitFunk$$1@sda21
/* 800E6940 000E3880  38 BF 00 9C */	addi r5, r31, 0x9c
/* 800E6944 000E3884  4B F9 BD E5 */	bl __register_global_object
/* 800E6948 000E3888  38 00 00 01 */	li r0, 1
/* 800E694C 000E388C  98 0D 8F 96 */	stb r0, __init__smList__26JALList$$015JALSeModPitFunk$$1@sda21(r13)
lbl_800E6950:
/* 800E6950 000E3890  88 0D 8F 97 */	lbz r0, __init__smList__26JALList$$015JALSeModVolFunk$$1@sda21(r13)
/* 800E6954 000E3894  7C 00 07 75 */	extsb. r0, r0
/* 800E6958 000E3898  40 82 00 28 */	bne lbl_800E6980
/* 800E695C 000E389C  38 6D 8F 80 */	addi r3, r13, smList__26JALList$$015JALSeModVolFunk$$1@sda21
/* 800E6960 000E38A0  4B F2 82 D5 */	bl initiate__10JSUPtrListFv
/* 800E6964 000E38A4  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolFunk$$1Fv@ha
/* 800E6968 000E38A8  38 83 A4 44 */	addi r4, r3, __dt__26JSUList$$015JALSeModVolFunk$$1Fv@l
/* 800E696C 000E38AC  38 6D 8F 80 */	addi r3, r13, smList__26JALList$$015JALSeModVolFunk$$1@sda21
/* 800E6970 000E38B0  38 BF 00 A8 */	addi r5, r31, 0xa8
/* 800E6974 000E38B4  4B F9 BD B5 */	bl __register_global_object
/* 800E6978 000E38B8  38 00 00 01 */	li r0, 1
/* 800E697C 000E38BC  98 0D 8F 97 */	stb r0, __init__smList__26JALList$$015JALSeModVolFunk$$1@sda21(r13)
lbl_800E6980:
/* 800E6980 000E38C0  80 01 00 14 */	lwz r0, 0x14(r1)
/* 800E6984 000E38C4  83 E1 00 0C */	lwz r31, 0xc(r1)
/* 800E6988 000E38C8  38 21 00 10 */	addi r1, r1, 0x10
/* 800E698C 000E38CC  7C 08 03 A6 */	mtlr r0
/* 800E6990 000E38D0  4E 80 00 20 */	blr 

.global __dt__21JSUList$$010MSSetSound$$1Fv
__dt__21JSUList$$010MSSetSound$$1Fv:
/* 800E6994 000E38D4  7C 08 02 A6 */	mflr r0
/* 800E6998 000E38D8  90 01 00 04 */	stw r0, 4(r1)
/* 800E699C 000E38DC  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 800E69A0 000E38E0  93 E1 00 14 */	stw r31, 0x14(r1)
/* 800E69A4 000E38E4  3B E4 00 00 */	addi r31, r4, 0
/* 800E69A8 000E38E8  93 C1 00 10 */	stw r30, 0x10(r1)
/* 800E69AC 000E38EC  7C 7E 1B 79 */	or. r30, r3, r3
/* 800E69B0 000E38F0  41 82 00 20 */	beq lbl_800E69D0
/* 800E69B4 000E38F4  38 7E 00 00 */	addi r3, r30, 0
/* 800E69B8 000E38F8  38 80 00 00 */	li r4, 0
/* 800E69BC 000E38FC  4B F2 82 11 */	bl __dt__10JSUPtrListFv
/* 800E69C0 000E3900  7F E0 07 35 */	extsh. r0, r31
/* 800E69C4 000E3904  40 81 00 0C */	ble lbl_800E69D0
/* 800E69C8 000E3908  7F C3 F3 78 */	mr r3, r30
/* 800E69CC 000E390C  4B F2 60 E5 */	bl __dl__FPv
lbl_800E69D0:
/* 800E69D0 000E3910  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 800E69D4 000E3914  7F C3 F3 78 */	mr r3, r30
/* 800E69D8 000E3918  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 800E69DC 000E391C  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 800E69E0 000E3920  7C 08 03 A6 */	mtlr r0
/* 800E69E4 000E3924  38 21 00 18 */	addi r1, r1, 0x18
/* 800E69E8 000E3928  4E 80 00 20 */	blr 

.global __dt__24JSUList$$013MSSetSoundGrp$$1Fv
__dt__24JSUList$$013MSSetSoundGrp$$1Fv:
/* 800E69EC 000E392C  7C 08 02 A6 */	mflr r0
/* 800E69F0 000E3930  90 01 00 04 */	stw r0, 4(r1)
/* 800E69F4 000E3934  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 800E69F8 000E3938  93 E1 00 14 */	stw r31, 0x14(r1)
/* 800E69FC 000E393C  3B E4 00 00 */	addi r31, r4, 0
/* 800E6A00 000E3940  93 C1 00 10 */	stw r30, 0x10(r1)
/* 800E6A04 000E3944  7C 7E 1B 79 */	or. r30, r3, r3
/* 800E6A08 000E3948  41 82 00 20 */	beq lbl_800E6A28
/* 800E6A0C 000E394C  38 7E 00 00 */	addi r3, r30, 0
/* 800E6A10 000E3950  38 80 00 00 */	li r4, 0
/* 800E6A14 000E3954  4B F2 81 B9 */	bl __dt__10JSUPtrListFv
/* 800E6A18 000E3958  7F E0 07 35 */	extsh. r0, r31
/* 800E6A1C 000E395C  40 81 00 0C */	ble lbl_800E6A28
/* 800E6A20 000E3960  7F C3 F3 78 */	mr r3, r30
/* 800E6A24 000E3964  4B F2 60 8D */	bl __dl__FPv
lbl_800E6A28:
/* 800E6A28 000E3968  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 800E6A2C 000E396C  7F C3 F3 78 */	mr r3, r30
/* 800E6A30 000E3970  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 800E6A34 000E3974  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 800E6A38 000E3978  7C 08 03 A6 */	mtlr r0
/* 800E6A3C 000E397C  38 21 00 18 */	addi r1, r1, 0x18
/* 800E6A40 000E3980  4E 80 00 20 */	blr 

.global __dt__15JSUList$$05MSBgm$$1Fv
__dt__15JSUList$$05MSBgm$$1Fv:
/* 800E6A44 000E3984  7C 08 02 A6 */	mflr r0
/* 800E6A48 000E3988  90 01 00 04 */	stw r0, 4(r1)
/* 800E6A4C 000E398C  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 800E6A50 000E3990  93 E1 00 14 */	stw r31, 0x14(r1)
/* 800E6A54 000E3994  3B E4 00 00 */	addi r31, r4, 0
/* 800E6A58 000E3998  93 C1 00 10 */	stw r30, 0x10(r1)
/* 800E6A5C 000E399C  7C 7E 1B 79 */	or. r30, r3, r3
/* 800E6A60 000E39A0  41 82 00 20 */	beq lbl_800E6A80
/* 800E6A64 000E39A4  38 7E 00 00 */	addi r3, r30, 0
/* 800E6A68 000E39A8  38 80 00 00 */	li r4, 0
/* 800E6A6C 000E39AC  4B F2 81 61 */	bl __dt__10JSUPtrListFv
/* 800E6A70 000E39B0  7F E0 07 35 */	extsh. r0, r31
/* 800E6A74 000E39B4  40 81 00 0C */	ble lbl_800E6A80
/* 800E6A78 000E39B8  7F C3 F3 78 */	mr r3, r30
/* 800E6A7C 000E39BC  4B F2 60 35 */	bl __dl__FPv
lbl_800E6A80:
/* 800E6A80 000E39C0  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 800E6A84 000E39C4  7F C3 F3 78 */	mr r3, r30
/* 800E6A88 000E39C8  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 800E6A8C 000E39CC  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 800E6A90 000E39D0  7C 08 03 A6 */	mtlr r0
/* 800E6A94 000E39D4  38 21 00 18 */	addi r1, r1, 0x18
/* 800E6A98 000E39D8  4E 80 00 20 */	blr 

.section .ctors, "wa"  # 0x8036FBA0 - 0x8036FF80
lbl_constructor:
  .4byte __sinit_EventWatcher_cpp

.section .rodata, "a"  # 0x8036FFA0 - 0x803A8380
.balign 8
$$21490:
  .4byte 0
  .4byte 0
  .4byte 0
$$21937:
	.incbin "baserom.dol", 0x36F99C, 0x14
$$22200:
	.incbin "baserom.dol", 0x36F9B0, 0x30
$$22201:
	.incbin "baserom.dol", 0x36F9E0, 0x38
$$22202:
	.incbin "baserom.dol", 0x36FA18, 0x34
$$22203:
	.incbin "baserom.dol", 0x36FA4C, 0x24
$$22728:
  .4byte 0
  .4byte 0
  .4byte 0
$$22730:
	.incbin "baserom.dol", 0x36FA7C, 0xC
$$23432:
	.incbin "baserom.dol", 0x36FA88, 0x18
$$23433:
	.incbin "baserom.dol", 0x36FAA0, 0x18
$$23441:
	.incbin "baserom.dol", 0x36FAB8, 0x10
$$23457:
	.incbin "baserom.dol", 0x36FAC8, 0x10
$$23471:
	.incbin "baserom.dol", 0x36FAD8, 0x10
$$23708:
	.incbin "baserom.dol", 0x36FAE8, 0x10
$$23709:
	.incbin "baserom.dol", 0x36FAF8, 0x10
$$23710:
	.incbin "baserom.dol", 0x36FB08, 0x14
$$23711:
	.incbin "baserom.dol", 0x36FB1C, 0x10
$$23712:
	.incbin "baserom.dol", 0x36FB2C, 0xC
$$23713:
	.incbin "baserom.dol", 0x36FB38, 0x14
$$23714:
	.incbin "baserom.dol", 0x36FB4C, 0x14
$$23715:
	.incbin "baserom.dol", 0x36FB60, 0x10
$$23716:
	.incbin "baserom.dol", 0x36FB70, 0x14
$$23717:
	.incbin "baserom.dol", 0x36FB84, 0x10
$$23718:
	.incbin "baserom.dol", 0x36FB94, 0xC
$$23719:
	.incbin "baserom.dol", 0x36FBA0, 0x10
$$23720:
	.incbin "baserom.dol", 0x36FBB0, 0x10
$$23721:
	.incbin "baserom.dol", 0x36FBC0, 0xC
$$23722:
	.incbin "baserom.dol", 0x36FBCC, 0x18
$$23723:
	.incbin "baserom.dol", 0x36FBE4, 0x18
$$23724:
	.incbin "baserom.dol", 0x36FBFC, 0x10
$$23725:
	.incbin "baserom.dol", 0x36FC0C, 0x14
$$23726:
	.incbin "baserom.dol", 0x36FC20, 0x14
$$23727:
	.incbin "baserom.dol", 0x36FC34, 0x14
$$23728:
	.incbin "baserom.dol", 0x36FC48, 0x14
$$23729:
	.incbin "baserom.dol", 0x36FC5C, 0x14
$$23730:
	.incbin "baserom.dol", 0x36FC70, 0x10
$$23731:
	.incbin "baserom.dol", 0x36FC80, 0x14
$$23732:
	.incbin "baserom.dol", 0x36FC94, 0x1C
$$23733:
	.incbin "baserom.dol", 0x36FCB0, 0xC
$$23734:
	.incbin "baserom.dol", 0x36FCBC, 0x14
$$23735:
	.incbin "baserom.dol", 0x36FCD0, 0x10
$$23736:
	.incbin "baserom.dol", 0x36FCE0, 0x10
$$23737:
	.incbin "baserom.dol", 0x36FCF0, 0xC
$$23738:
	.incbin "baserom.dol", 0x36FCFC, 0x14
$$23739:
	.incbin "baserom.dol", 0x36FD10, 0x18
$$23740:
	.incbin "baserom.dol", 0x36FD28, 0x10
$$23741:
	.incbin "baserom.dol", 0x36FD38, 0x18
$$23742:
	.incbin "baserom.dol", 0x36FD50, 0x18
$$23743:
	.incbin "baserom.dol", 0x36FD68, 0x1C
$$23744:
	.incbin "baserom.dol", 0x36FD84, 0x14
$$23745:
	.incbin "baserom.dol", 0x36FD98, 0x18
$$23746:
	.incbin "baserom.dol", 0x36FDB0, 0x18
$$23747:
	.incbin "baserom.dol", 0x36FDC8, 0x14
$$23748:
	.incbin "baserom.dol", 0x36FDDC, 0x18
$$23749:
	.incbin "baserom.dol", 0x36FDF4, 0x10
$$23750:
	.incbin "baserom.dol", 0x36FE04, 0x10
$$23751:
	.incbin "baserom.dol", 0x36FE14, 0xC
$$23752:
	.incbin "baserom.dol", 0x36FE20, 0xC
$$23753:
	.incbin "baserom.dol", 0x36FE2C, 0x10
$$23754:
	.incbin "baserom.dol", 0x36FE3C, 0xC
$$23755:
	.incbin "baserom.dol", 0x36FE48, 0x14
$$23757:
	.incbin "baserom.dol", 0x36FE5C, 0xC
$$23758:
	.incbin "baserom.dol", 0x36FE68, 0x10
$$23759:
	.incbin "baserom.dol", 0x36FE78, 0x14
$$23760:
	.incbin "baserom.dol", 0x36FE8C, 0x14
$$23761:
	.incbin "baserom.dol", 0x36FEA0, 0x20
$$23762:
	.incbin "baserom.dol", 0x36FEC0, 0x1C
$$23763:
	.incbin "baserom.dol", 0x36FEDC, 0xC
$$23764:
	.incbin "baserom.dol", 0x36FEE8, 0x18
$$23765:
	.incbin "baserom.dol", 0x36FF00, 0x10
$$23766:
	.incbin "baserom.dol", 0x36FF10, 0x14
$$23767:
	.incbin "baserom.dol", 0x36FF24, 0x10
$$23768:
	.incbin "baserom.dol", 0x36FF34, 0x10
$$23769:
	.incbin "baserom.dol", 0x36FF44, 0x10
$$23770:
	.incbin "baserom.dol", 0x36FF54, 0xC
$$23771:
	.incbin "baserom.dol", 0x36FF60, 0xC
$$23773:
	.incbin "baserom.dol", 0x36FF6C, 0xC
$$23775:
	.incbin "baserom.dol", 0x36FF78, 0x10
$$23776:
	.incbin "baserom.dol", 0x36FF88, 0x10
$$23777:
	.incbin "baserom.dol", 0x36FF98, 0x10
$$23778:
	.incbin "baserom.dol", 0x36FFA8, 0xC
$$23779:
	.incbin "baserom.dol", 0x36FFB4, 0x18
$$23780:
	.incbin "baserom.dol", 0x36FFCC, 0x18
$$23781:
	.incbin "baserom.dol", 0x36FFE4, 0x14
$$23782:
	.incbin "baserom.dol", 0x36FFF8, 0x10
$$23783:
	.incbin "baserom.dol", 0x370008, 0x14
$$23784:
	.incbin "baserom.dol", 0x37001C, 0x10
$$23785:
	.incbin "baserom.dol", 0x37002C, 0x10
$$23786:
	.incbin "baserom.dol", 0x37003C, 0x10
$$23787:
	.incbin "baserom.dol", 0x37004C, 0xC
$$23788:
	.incbin "baserom.dol", 0x370058, 0x14
$$23789:
	.incbin "baserom.dol", 0x37006C, 0x14
$$23790:
	.incbin "baserom.dol", 0x370080, 0xC
$$23791:
	.incbin "baserom.dol", 0x37008C, 0x14
$$23792:
	.incbin "baserom.dol", 0x3700A0, 0x18
$$23793:
	.incbin "baserom.dol", 0x3700B8, 0x14
$$23794:
	.incbin "baserom.dol", 0x3700CC, 0x18
$$23795:
	.incbin "baserom.dol", 0x3700E4, 0x18
$$23829:
	.incbin "baserom.dol", 0x3700FC, 0x20
$$23834:
	.incbin "baserom.dol", 0x37011C, 0x1C
$$23988:
	.incbin "baserom.dol", 0x370138, 0x14
$$24300:
	.incbin "baserom.dol", 0x37014C, 0x14
$$24824:
	.incbin "baserom.dol", 0x370160, 0xC
$$24846:
	.incbin "baserom.dol", 0x37016C, 0x14
$$25130:
	.incbin "baserom.dol", 0x370180, 0x1C
$$25584:
	.incbin "baserom.dol", 0x37019C, 0xC
$$25774:
	.incbin "baserom.dol", 0x3701A8, 0x20

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global __vt__13TEventWatcher
__vt__13TEventWatcher:
  .4byte 0
  .4byte 0
  .4byte __dt__13TEventWatcherFv
  .4byte getType__Q26JDrama8TNameRefCFv
  .4byte load__Q26JDrama8TNameRefFR20JSUMemoryInputStream
  .4byte save__Q26JDrama8TNameRefFR21JSUMemoryOutputStream
  .4byte loadAfter__Q26JDrama8TNameRefFv
  .4byte searchF__Q26JDrama8TNameRefFUsPCc
  .4byte perform__13TEventWatcherFUlPQ26JDrama9TGraphics
  .4byte launchScript__13TEventWatcherFPCc
.global __vt__32TSpcTypedInterp$$013TEventWatcher$$1
__vt__32TSpcTypedInterp$$013TEventWatcher$$1:
  .4byte 0
  .4byte 0
  .4byte dispatchBuiltin__32TSpcTypedInterp$$013TEventWatcher$$1FUlUl
  .4byte __dt__32TSpcTypedInterp$$013TEventWatcher$$1Fv
  .4byte update__10TSpcInterpFv
.global __vt__32TSpcTypedBinary$$013TEventWatcher$$1
__vt__32TSpcTypedBinary$$013TEventWatcher$$1:
  .4byte 0
  .4byte 0
  .4byte __dt__32TSpcTypedBinary$$013TEventWatcher$$1Fv
  .4byte initUserBuiltin__32TSpcTypedBinary$$013TEventWatcher$$1Fv
  .4byte 0

.section .sdata, "wa"  # 0x80408AC0 - 0x804097C0
sShineViewObjName$3431:
  .4byte $$23432
  .4byte $$23433

.section .sdata2, "a"  # 0x8040B460 - 0x80414020
.balign 8
$$23756:
	.incbin "baserom.dol", 0x3E4CB0, 0x8
$$23772:
	.incbin "baserom.dol", 0x3E4CB8, 0x8
$$23774:
	.incbin "baserom.dol", 0x3E4CC0, 0x8
$$23830:
	.incbin "baserom.dol", 0x3E4CC8, 0x4
$$23831:
	.incbin "baserom.dol", 0x3E4CCC, 0x4
$$23832:
  .4byte 0
$$23833:
	.incbin "baserom.dol", 0x3E4CD4, 0x4
$$23931:
	.incbin "baserom.dol", 0x3E4CD8, 0x4
$$23932:
	.incbin "baserom.dol", 0x3E4CDC, 0x4
$$23934:
	.incbin "baserom.dol", 0x3E4CE0, 0x8
$$23989:
	.incbin "baserom.dol", 0x3E4CE8, 0x4
$$23990:
	.incbin "baserom.dol", 0x3E4CEC, 0x4
$$24053:
  .4byte 0
$$24803:
	.incbin "baserom.dol", 0x3E4CF4, 0x4
$$25131:
	.incbin "baserom.dol", 0x3E4CF8, 0x4
$$25132:
	.incbin "baserom.dol", 0x3E4CFC, 0x4
$$25133:
	.incbin "baserom.dol", 0x3E4D00, 0x8

.section .bss, "wa"  # 0x803E6000 - 0x80408AC0
$$27636:
	.skip 0xC
$$27637:
	.skip 0xC
$$27638:
	.skip 0xC
$$27639:
	.skip 0xC
$$27640:
	.skip 0xC
$$27641:
	.skip 0xC
$$27642:
	.skip 0xC
$$27643:
	.skip 0xC
$$27644:
	.skip 0xC
$$27645:
	.skip 0xC
$$27646:
	.skip 0xC
$$27647:
	.skip 0xC
$$27648:
	.skip 0xC
$$27649:
	.skip 0xC
$$27650:
	.skip 0x10

.section .sbss, "wa"  # 0x804097C0 - 0x8040B45C
.global smList__15JALList$$05MSBgm$$1
smList__15JALList$$05MSBgm$$1:
	.skip 0xC
.global smList__24JALList$$013MSSetSoundGrp$$1
smList__24JALList$$013MSSetSoundGrp$$1:
	.skip 0xC
.global smList__21JALList$$010MSSetSound$$1
smList__21JALList$$010MSSetSound$$1:
	.skip 0xC
.global __init__smList__15JALList$$05MSBgm$$1
__init__smList__15JALList$$05MSBgm$$1:
	.skip 0x1
.global __init__smList__24JALList$$013MSSetSoundGrp$$1
__init__smList__24JALList$$013MSSetSoundGrp$$1:
	.skip 0x1
.global __init__smList__21JALList$$010MSSetSound$$1
__init__smList__21JALList$$010MSSetSound$$1:
	.skip 0x2
