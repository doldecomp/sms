.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global registerEventWatcher__12TMarDirectorFP13TEventWatcher
registerEventWatcher__12TMarDirectorFP13TEventWatcher:
/* 800EA294 000E71D4  7C 08 02 A6 */	mflr r0
/* 800EA298 000E71D8  90 01 00 04 */	stw r0, 4(r1)
/* 800EA29C 000E71DC  94 21 FF B0 */	stwu r1, -0x50(r1)
/* 800EA2A0 000E71E0  93 E1 00 4C */	stw r31, 0x4c(r1)
/* 800EA2A4 000E71E4  90 81 00 3C */	stw r4, 0x3c(r1)
/* 800EA2A8 000E71E8  80 83 00 80 */	lwz r4, 0x80(r3)
/* 800EA2AC 000E71EC  38 61 00 20 */	addi r3, r1, 0x20
/* 800EA2B0 000E71F0  3B E4 00 10 */	addi r31, r4, 0x10
/* 800EA2B4 000E71F4  38 9F 00 00 */	addi r4, r31, 0
/* 800EA2B8 000E71F8  4B FD A2 71 */	bl end__Q27JGadget36TList$$0Pv$$4Q27JGadget14TAllocator$$0Pv$$1$$1Fv
/* 800EA2BC 000E71FC  80 01 00 20 */	lwz r0, 0x20(r1)
/* 800EA2C0 000E7200  38 81 00 1C */	addi r4, r1, 0x1c
/* 800EA2C4 000E7204  38 61 00 38 */	addi r3, r1, 0x38
/* 800EA2C8 000E7208  90 01 00 1C */	stw r0, 0x1c(r1)
/* 800EA2CC 000E720C  4B FD 94 49 */	bl __ct__Q37JGadget34TList_pointer$$0PQ26JDrama8TViewObj$$18iteratorFQ37JGadget36TList$$0Pv$$4Q27JGadget14TAllocator$$0Pv$$1$$18iterator
/* 800EA2D0 000E7210  80 01 00 38 */	lwz r0, 0x38(r1)
/* 800EA2D4 000E7214  38 A1 00 2C */	addi r5, r1, 0x2c
/* 800EA2D8 000E7218  38 9F 00 00 */	addi r4, r31, 0
/* 800EA2DC 000E721C  90 01 00 34 */	stw r0, 0x34(r1)
/* 800EA2E0 000E7220  38 61 00 28 */	addi r3, r1, 0x28
/* 800EA2E4 000E7224  38 C1 00 3C */	addi r6, r1, 0x3c
/* 800EA2E8 000E7228  80 01 00 34 */	lwz r0, 0x34(r1)
/* 800EA2EC 000E722C  90 01 00 2C */	stw r0, 0x2c(r1)
/* 800EA2F0 000E7230  4B F4 A0 7D */	bl insert__Q27JGadget18TList_pointer_voidFQ37JGadget36TList$$0Pv$$4Q27JGadget14TAllocator$$0Pv$$1$$18iteratorRCPv
/* 800EA2F4 000E7234  80 01 00 28 */	lwz r0, 0x28(r1)
/* 800EA2F8 000E7238  38 61 00 30 */	addi r3, r1, 0x30
/* 800EA2FC 000E723C  38 81 00 24 */	addi r4, r1, 0x24
/* 800EA300 000E7240  90 01 00 24 */	stw r0, 0x24(r1)
/* 800EA304 000E7244  4B F5 A9 2D */	bl __ct__Q37JGadget36TList$$0Pv$$4Q27JGadget14TAllocator$$0Pv$$1$$18iteratorFRCQ37JGadget36TList$$0Pv$$4Q27JGadget14TAllocator$$0Pv$$1$$18iterator
/* 800EA308 000E7248  80 01 00 54 */	lwz r0, 0x54(r1)
/* 800EA30C 000E724C  83 E1 00 4C */	lwz r31, 0x4c(r1)
/* 800EA310 000E7250  38 21 00 50 */	addi r1, r1, 0x50
/* 800EA314 000E7254  7C 08 03 A6 */	mtlr r0
/* 800EA318 000E7258  4E 80 00 20 */	blr 

.global setup__12TMarDirectorFPQ26JDrama8TDisplayPP13TMarioGamePadUcUc
setup__12TMarDirectorFPQ26JDrama8TDisplayPP13TMarioGamePadUcUc:
/* 800EA31C 000E725C  7C 08 02 A6 */	mflr r0
/* 800EA320 000E7260  7C 68 1B 78 */	mr r8, r3
/* 800EA324 000E7264  90 01 00 04 */	stw r0, 4(r1)
/* 800EA328 000E7268  39 20 00 00 */	li r9, 0
/* 800EA32C 000E726C  94 21 FF E0 */	stwu r1, -0x20(r1)
/* 800EA330 000E7270  93 E1 00 1C */	stw r31, 0x1c(r1)
/* 800EA334 000E7274  90 83 00 C0 */	stw r4, 0xc0(r3)
/* 800EA338 000E7278  3C 80 80 3F */	lis r4, gSetupThread@ha
/* 800EA33C 000E727C  3C 60 80 0F */	lis r3, setupThreadFunc__12TMarDirectorFPv@ha
/* 800EA340 000E7280  90 A8 00 18 */	stw r5, 0x18(r8)
/* 800EA344 000E7284  3B E4 23 90 */	addi r31, r4, gSetupThread@l
/* 800EA348 000E7288  38 83 A3 90 */	addi r4, r3, setupThreadFunc__12TMarDirectorFPv@l
/* 800EA34C 000E728C  98 C8 00 7C */	stb r6, 0x7c(r8)
/* 800EA350 000E7290  7F E3 FB 78 */	mr r3, r31
/* 800EA354 000E7294  38 A8 00 00 */	addi r5, r8, 0
/* 800EA358 000E7298  98 E8 00 7D */	stb r7, 0x7d(r8)
/* 800EA35C 000E729C  3C E0 00 01 */	lis r7, 1
/* 800EA360 000E72A0  39 00 00 11 */	li r8, 0x11
/* 800EA364 000E72A4  80 CD 97 F0 */	lwz r6, gpSetupThreadStack@sda21(r13)
/* 800EA368 000E72A8  3C C6 00 01 */	addis r6, r6, 1
/* 800EA36C 000E72AC  4B FA 8E FD */	bl OSCreateThread
/* 800EA370 000E72B0  7F E3 FB 78 */	mr r3, r31
/* 800EA374 000E72B4  4B FA 94 95 */	bl OSResumeThread
/* 800EA378 000E72B8  80 01 00 24 */	lwz r0, 0x24(r1)
/* 800EA37C 000E72BC  38 60 00 00 */	li r3, 0
/* 800EA380 000E72C0  83 E1 00 1C */	lwz r31, 0x1c(r1)
/* 800EA384 000E72C4  38 21 00 20 */	addi r1, r1, 0x20
/* 800EA388 000E72C8  7C 08 03 A6 */	mtlr r0
/* 800EA38C 000E72CC  4E 80 00 20 */	blr 

.global setupThreadFunc__12TMarDirectorFPv
setupThreadFunc__12TMarDirectorFPv:
/* 800EA390 000E72D0  7C 08 02 A6 */	mflr r0
/* 800EA394 000E72D4  90 01 00 04 */	stw r0, 4(r1)
/* 800EA398 000E72D8  94 21 FF F8 */	stwu r1, -8(r1)
/* 800EA39C 000E72DC  48 02 01 F5 */	bl loadResource__12TMarDirectorFv
/* 800EA3A0 000E72E0  80 01 00 0C */	lwz r0, 0xc(r1)
/* 800EA3A4 000E72E4  38 21 00 08 */	addi r1, r1, 8
/* 800EA3A8 000E72E8  7C 08 03 A6 */	mtlr r0
/* 800EA3AC 000E72EC  4E 80 00 20 */	blr 

.global __ct__12TMarDirectorFv
__ct__12TMarDirectorFv:
/* 800EA3B0 000E72F0  7C 08 02 A6 */	mflr r0
/* 800EA3B4 000E72F4  3C 80 80 37 */	lis r4, $$21490@ha
/* 800EA3B8 000E72F8  90 01 00 04 */	stw r0, 4(r1)
/* 800EA3BC 000E72FC  94 21 FF A0 */	stwu r1, -0x60(r1)
/* 800EA3C0 000E7300  93 E1 00 5C */	stw r31, 0x5c(r1)
/* 800EA3C4 000E7304  3B E4 32 B8 */	addi r31, r4, $$21490@l
/* 800EA3C8 000E7308  93 C1 00 58 */	stw r30, 0x58(r1)
/* 800EA3CC 000E730C  93 A1 00 54 */	stw r29, 0x54(r1)
/* 800EA3D0 000E7310  93 81 00 50 */	stw r28, 0x50(r1)
/* 800EA3D4 000E7314  90 61 00 08 */	stw r3, 8(r1)
/* 800EA3D8 000E7318  3C 60 80 3B */	lis r3, __vt__Q26JDrama8TNameRef@ha
/* 800EA3DC 000E731C  38 03 A8 60 */	addi r0, r3, __vt__Q26JDrama8TNameRef@l
/* 800EA3E0 000E7320  83 81 00 08 */	lwz r28, 8(r1)
/* 800EA3E4 000E7324  38 7F 00 0C */	addi r3, r31, 0xc
/* 800EA3E8 000E7328  90 1C 00 00 */	stw r0, 0(r28)
/* 800EA3EC 000E732C  90 7C 00 04 */	stw r3, 4(r28)
/* 800EA3F0 000E7330  4B F5 A2 9D */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800EA3F4 000E7334  B0 7C 00 08 */	sth r3, 8(r28)
/* 800EA3F8 000E7338  3C 60 80 3B */	lis r3, __vt__Q26JStage7TObject@ha
/* 800EA3FC 000E733C  38 03 CE 98 */	addi r0, r3, __vt__Q26JStage7TObject@l
/* 800EA400 000E7340  3C 60 80 3B */	lis r3, __vt__Q26JStage7TSystem@ha
/* 800EA404 000E7344  90 1C 00 0C */	stw r0, 0xc(r28)
/* 800EA408 000E7348  38 03 CE C8 */	addi r0, r3, __vt__Q26JStage7TSystem@l
/* 800EA40C 000E734C  3C 60 80 3B */	lis r3, __vt__Q26JDrama9TDirector@ha
/* 800EA410 000E7350  90 1C 00 0C */	stw r0, 0xc(r28)
/* 800EA414 000E7354  38 63 A4 98 */	addi r3, r3, __vt__Q26JDrama9TDirector@l
/* 800EA418 000E7358  90 7C 00 00 */	stw r3, 0(r28)
/* 800EA41C 000E735C  38 03 00 20 */	addi r0, r3, 0x20
/* 800EA420 000E7360  3C 60 80 3B */	lis r3, __vt__12TMarDirector@ha
/* 800EA424 000E7364  90 1C 00 0C */	stw r0, 0xc(r28)
/* 800EA428 000E7368  38 83 3C A0 */	addi r4, r3, __vt__12TMarDirector@l
/* 800EA42C 000E736C  38 A0 00 00 */	li r5, 0
/* 800EA430 000E7370  90 BC 00 10 */	stw r5, 0x10(r28)
/* 800EA434 000E7374  38 04 00 20 */	addi r0, r4, 0x20
/* 800EA438 000E7378  38 60 00 1C */	li r3, 0x1c
/* 800EA43C 000E737C  90 BC 00 14 */	stw r5, 0x14(r28)
/* 800EA440 000E7380  90 9C 00 00 */	stw r4, 0(r28)
/* 800EA444 000E7384  90 1C 00 0C */	stw r0, 0xc(r28)
/* 800EA448 000E7388  90 BC 00 18 */	stw r5, 0x18(r28)
/* 800EA44C 000E738C  90 BC 00 1C */	stw r5, 0x1c(r28)
/* 800EA450 000E7390  90 BC 00 20 */	stw r5, 0x20(r28)
/* 800EA454 000E7394  90 BC 00 24 */	stw r5, 0x24(r28)
/* 800EA458 000E7398  90 BC 00 28 */	stw r5, 0x28(r28)
/* 800EA45C 000E739C  90 BC 00 2C */	stw r5, 0x2c(r28)
/* 800EA460 000E73A0  4B F2 24 51 */	bl __nw__FUl
/* 800EA464 000E73A4  7C 7C 1B 79 */	or. r28, r3, r3
/* 800EA468 000E73A8  41 82 00 4C */	beq lbl_800EA4B4
/* 800EA46C 000E73AC  93 81 00 1C */	stw r28, 0x1c(r1)
/* 800EA470 000E73B0  38 9F 00 18 */	addi r4, r31, 0x18
/* 800EA474 000E73B4  93 81 00 48 */	stw r28, 0x48(r1)
/* 800EA478 000E73B8  80 61 00 1C */	lwz r3, 0x1c(r1)
/* 800EA47C 000E73BC  4B F5 66 8D */	bl __ct__Q26JDrama8TNameRefFPCc
/* 800EA480 000E73C0  80 61 00 1C */	lwz r3, 0x1c(r1)
/* 800EA484 000E73C4  3C 80 80 3B */	lis r4, __vt__Q26JDrama8TViewObj@ha
/* 800EA488 000E73C8  38 04 A0 C0 */	addi r0, r4, __vt__Q26JDrama8TViewObj@l
/* 800EA48C 000E73CC  90 03 00 00 */	stw r0, 0(r3)
/* 800EA490 000E73D0  38 63 00 0C */	addi r3, r3, 0xc
/* 800EA494 000E73D4  38 80 00 00 */	li r4, 0
/* 800EA498 000E73D8  4B F5 66 69 */	bl __ct__Q26JDrama10TFlagT$$0Us$$1FUs
/* 800EA49C 000E73DC  83 A1 00 48 */	lwz r29, 0x48(r1)
/* 800EA4A0 000E73E0  38 7D 00 10 */	addi r3, r29, 0x10
/* 800EA4A4 000E73E4  48 00 02 95 */	bl Initialize___Q27JGadget19TSingleNodeLinkListFv
/* 800EA4A8 000E73E8  3C 60 80 3B */	lis r3, __vt__12TPerformList@ha
/* 800EA4AC 000E73EC  38 03 3F 18 */	addi r0, r3, __vt__12TPerformList@l
/* 800EA4B0 000E73F0  90 1D 00 00 */	stw r0, 0(r29)
lbl_800EA4B4:
/* 800EA4B4 000E73F4  80 81 00 08 */	lwz r4, 8(r1)
/* 800EA4B8 000E73F8  38 60 00 1C */	li r3, 0x1c
/* 800EA4BC 000E73FC  93 84 00 30 */	stw r28, 0x30(r4)
/* 800EA4C0 000E7400  4B F2 23 F1 */	bl __nw__FUl
/* 800EA4C4 000E7404  7C 7C 1B 79 */	or. r28, r3, r3
/* 800EA4C8 000E7408  41 82 00 4C */	beq lbl_800EA514
/* 800EA4CC 000E740C  93 81 00 18 */	stw r28, 0x18(r1)
/* 800EA4D0 000E7410  38 9F 00 18 */	addi r4, r31, 0x18
/* 800EA4D4 000E7414  93 81 00 44 */	stw r28, 0x44(r1)
/* 800EA4D8 000E7418  80 61 00 18 */	lwz r3, 0x18(r1)
/* 800EA4DC 000E741C  4B F5 66 2D */	bl __ct__Q26JDrama8TNameRefFPCc
/* 800EA4E0 000E7420  80 61 00 18 */	lwz r3, 0x18(r1)
/* 800EA4E4 000E7424  3C 80 80 3B */	lis r4, __vt__Q26JDrama8TViewObj@ha
/* 800EA4E8 000E7428  38 04 A0 C0 */	addi r0, r4, __vt__Q26JDrama8TViewObj@l
/* 800EA4EC 000E742C  90 03 00 00 */	stw r0, 0(r3)
/* 800EA4F0 000E7430  38 63 00 0C */	addi r3, r3, 0xc
/* 800EA4F4 000E7434  38 80 00 00 */	li r4, 0
/* 800EA4F8 000E7438  4B F5 66 09 */	bl __ct__Q26JDrama10TFlagT$$0Us$$1FUs
/* 800EA4FC 000E743C  83 A1 00 44 */	lwz r29, 0x44(r1)
/* 800EA500 000E7440  38 7D 00 10 */	addi r3, r29, 0x10
/* 800EA504 000E7444  48 00 02 35 */	bl Initialize___Q27JGadget19TSingleNodeLinkListFv
/* 800EA508 000E7448  3C 60 80 3B */	lis r3, __vt__12TPerformList@ha
/* 800EA50C 000E744C  38 03 3F 18 */	addi r0, r3, __vt__12TPerformList@l
/* 800EA510 000E7450  90 1D 00 00 */	stw r0, 0(r29)
lbl_800EA514:
/* 800EA514 000E7454  80 81 00 08 */	lwz r4, 8(r1)
/* 800EA518 000E7458  38 60 00 1C */	li r3, 0x1c
/* 800EA51C 000E745C  93 84 00 34 */	stw r28, 0x34(r4)
/* 800EA520 000E7460  4B F2 23 91 */	bl __nw__FUl
/* 800EA524 000E7464  7C 7C 1B 79 */	or. r28, r3, r3
/* 800EA528 000E7468  41 82 00 4C */	beq lbl_800EA574
/* 800EA52C 000E746C  93 81 00 14 */	stw r28, 0x14(r1)
/* 800EA530 000E7470  38 9F 00 18 */	addi r4, r31, 0x18
/* 800EA534 000E7474  93 81 00 40 */	stw r28, 0x40(r1)
/* 800EA538 000E7478  80 61 00 14 */	lwz r3, 0x14(r1)
/* 800EA53C 000E747C  4B F5 65 CD */	bl __ct__Q26JDrama8TNameRefFPCc
/* 800EA540 000E7480  80 61 00 14 */	lwz r3, 0x14(r1)
/* 800EA544 000E7484  3C 80 80 3B */	lis r4, __vt__Q26JDrama8TViewObj@ha
/* 800EA548 000E7488  38 04 A0 C0 */	addi r0, r4, __vt__Q26JDrama8TViewObj@l
/* 800EA54C 000E748C  90 03 00 00 */	stw r0, 0(r3)
/* 800EA550 000E7490  38 63 00 0C */	addi r3, r3, 0xc
/* 800EA554 000E7494  38 80 00 00 */	li r4, 0
/* 800EA558 000E7498  4B F5 65 A9 */	bl __ct__Q26JDrama10TFlagT$$0Us$$1FUs
/* 800EA55C 000E749C  83 A1 00 40 */	lwz r29, 0x40(r1)
/* 800EA560 000E74A0  38 7D 00 10 */	addi r3, r29, 0x10
/* 800EA564 000E74A4  48 00 01 D5 */	bl Initialize___Q27JGadget19TSingleNodeLinkListFv
/* 800EA568 000E74A8  3C 60 80 3B */	lis r3, __vt__12TPerformList@ha
/* 800EA56C 000E74AC  38 03 3F 18 */	addi r0, r3, __vt__12TPerformList@l
/* 800EA570 000E74B0  90 1D 00 00 */	stw r0, 0(r29)
lbl_800EA574:
/* 800EA574 000E74B4  80 81 00 08 */	lwz r4, 8(r1)
/* 800EA578 000E74B8  38 60 00 1C */	li r3, 0x1c
/* 800EA57C 000E74BC  93 84 00 38 */	stw r28, 0x38(r4)
/* 800EA580 000E74C0  4B F2 23 31 */	bl __nw__FUl
/* 800EA584 000E74C4  7C 7C 1B 79 */	or. r28, r3, r3
/* 800EA588 000E74C8  41 82 00 4C */	beq lbl_800EA5D4
/* 800EA58C 000E74CC  93 81 00 10 */	stw r28, 0x10(r1)
/* 800EA590 000E74D0  38 9F 00 18 */	addi r4, r31, 0x18
/* 800EA594 000E74D4  93 81 00 3C */	stw r28, 0x3c(r1)
/* 800EA598 000E74D8  80 61 00 10 */	lwz r3, 0x10(r1)
/* 800EA59C 000E74DC  4B F5 65 6D */	bl __ct__Q26JDrama8TNameRefFPCc
/* 800EA5A0 000E74E0  80 61 00 10 */	lwz r3, 0x10(r1)
/* 800EA5A4 000E74E4  3C 80 80 3B */	lis r4, __vt__Q26JDrama8TViewObj@ha
/* 800EA5A8 000E74E8  38 04 A0 C0 */	addi r0, r4, __vt__Q26JDrama8TViewObj@l
/* 800EA5AC 000E74EC  90 03 00 00 */	stw r0, 0(r3)
/* 800EA5B0 000E74F0  38 63 00 0C */	addi r3, r3, 0xc
/* 800EA5B4 000E74F4  38 80 00 00 */	li r4, 0
/* 800EA5B8 000E74F8  4B F5 65 49 */	bl __ct__Q26JDrama10TFlagT$$0Us$$1FUs
/* 800EA5BC 000E74FC  83 A1 00 3C */	lwz r29, 0x3c(r1)
/* 800EA5C0 000E7500  38 7D 00 10 */	addi r3, r29, 0x10
/* 800EA5C4 000E7504  48 00 01 75 */	bl Initialize___Q27JGadget19TSingleNodeLinkListFv
/* 800EA5C8 000E7508  3C 60 80 3B */	lis r3, __vt__12TPerformList@ha
/* 800EA5CC 000E750C  38 03 3F 18 */	addi r0, r3, __vt__12TPerformList@l
/* 800EA5D0 000E7510  90 1D 00 00 */	stw r0, 0(r29)
lbl_800EA5D4:
/* 800EA5D4 000E7514  80 81 00 08 */	lwz r4, 8(r1)
/* 800EA5D8 000E7518  38 60 00 1C */	li r3, 0x1c
/* 800EA5DC 000E751C  93 84 00 3C */	stw r28, 0x3c(r4)
/* 800EA5E0 000E7520  4B F2 22 D1 */	bl __nw__FUl
/* 800EA5E4 000E7524  7C 7C 1B 79 */	or. r28, r3, r3
/* 800EA5E8 000E7528  41 82 00 4C */	beq lbl_800EA634
/* 800EA5EC 000E752C  93 81 00 0C */	stw r28, 0xc(r1)
/* 800EA5F0 000E7530  38 9F 00 18 */	addi r4, r31, 0x18
/* 800EA5F4 000E7534  93 81 00 38 */	stw r28, 0x38(r1)
/* 800EA5F8 000E7538  80 61 00 0C */	lwz r3, 0xc(r1)
/* 800EA5FC 000E753C  4B F5 65 0D */	bl __ct__Q26JDrama8TNameRefFPCc
/* 800EA600 000E7540  80 61 00 0C */	lwz r3, 0xc(r1)
/* 800EA604 000E7544  3C 80 80 3B */	lis r4, __vt__Q26JDrama8TViewObj@ha
/* 800EA608 000E7548  38 04 A0 C0 */	addi r0, r4, __vt__Q26JDrama8TViewObj@l
/* 800EA60C 000E754C  90 03 00 00 */	stw r0, 0(r3)
/* 800EA610 000E7550  38 63 00 0C */	addi r3, r3, 0xc
/* 800EA614 000E7554  38 80 00 00 */	li r4, 0
/* 800EA618 000E7558  4B F5 64 E9 */	bl __ct__Q26JDrama10TFlagT$$0Us$$1FUs
/* 800EA61C 000E755C  83 A1 00 38 */	lwz r29, 0x38(r1)
/* 800EA620 000E7560  38 7D 00 10 */	addi r3, r29, 0x10
/* 800EA624 000E7564  48 00 01 15 */	bl Initialize___Q27JGadget19TSingleNodeLinkListFv
/* 800EA628 000E7568  3C 60 80 3B */	lis r3, __vt__12TPerformList@ha
/* 800EA62C 000E756C  38 03 3F 18 */	addi r0, r3, __vt__12TPerformList@l
/* 800EA630 000E7570  90 1D 00 00 */	stw r0, 0(r29)
lbl_800EA634:
/* 800EA634 000E7574  83 A1 00 08 */	lwz r29, 8(r1)
/* 800EA638 000E7578  3B C0 00 00 */	li r30, 0
/* 800EA63C 000E757C  38 81 00 34 */	addi r4, r1, 0x34
/* 800EA640 000E7580  93 9D 00 40 */	stw r28, 0x40(r29)
/* 800EA644 000E7584  3B 9D 00 88 */	addi r28, r29, 0x88
/* 800EA648 000E7588  38 7C 00 00 */	addi r3, r28, 0
/* 800EA64C 000E758C  93 DD 00 44 */	stw r30, 0x44(r29)
/* 800EA650 000E7590  93 DD 00 48 */	stw r30, 0x48(r29)
/* 800EA654 000E7594  B3 DD 00 4C */	sth r30, 0x4c(r29)
/* 800EA658 000E7598  B3 DD 00 4E */	sth r30, 0x4e(r29)
/* 800EA65C 000E759C  B3 DD 00 50 */	sth r30, 0x50(r29)
/* 800EA660 000E75A0  93 DD 00 54 */	stw r30, 0x54(r29)
/* 800EA664 000E75A4  93 DD 00 68 */	stw r30, 0x68(r29)
/* 800EA668 000E75A8  C0 02 90 40 */	lfs f0, $$22398@sda21(r2)
/* 800EA66C 000E75AC  D0 1D 00 6C */	stfs f0, 0x6c(r29)
/* 800EA670 000E75B0  93 DD 00 80 */	stw r30, 0x80(r29)
/* 800EA674 000E75B4  4B F4 F4 C5 */	bl __ct__Q27JGadget20TVector_pointer_voidFRCQ27JGadget14TAllocator$$0Pv$$1
/* 800EA678 000E75B8  93 DD 00 A0 */	stw r30, 0xa0(r29)
/* 800EA67C 000E75BC  3C 60 80 0F */	lis r3, __ct__Q212TMarDirector9TDemoInfoFv@ha
/* 800EA680 000E75C0  38 83 A7 2C */	addi r4, r3, __ct__Q212TMarDirector9TDemoInfoFv@l
/* 800EA684 000E75C4  93 DD 00 B8 */	stw r30, 0xb8(r29)
/* 800EA688 000E75C8  38 7D 01 2C */	addi r3, r29, 0x12c
/* 800EA68C 000E75CC  38 A0 00 00 */	li r5, 0
/* 800EA690 000E75D0  93 DD 00 BC */	stw r30, 0xbc(r29)
/* 800EA694 000E75D4  38 C0 00 24 */	li r6, 0x24
/* 800EA698 000E75D8  38 E0 00 08 */	li r7, 8
/* 800EA69C 000E75DC  93 DD 00 CC */	stw r30, 0xcc(r29)
/* 800EA6A0 000E75E0  93 DD 00 C8 */	stw r30, 0xc8(r29)
/* 800EA6A4 000E75E4  93 DD 00 D4 */	stw r30, 0xd4(r29)
/* 800EA6A8 000E75E8  93 DD 00 D8 */	stw r30, 0xd8(r29)
/* 800EA6AC 000E75EC  93 DD 00 DC */	stw r30, 0xdc(r29)
/* 800EA6B0 000E75F0  B3 DD 01 28 */	sth r30, 0x128(r29)
/* 800EA6B4 000E75F4  4B F9 81 81 */	bl __construct_array
/* 800EA6B8 000E75F8  9B DD 02 4C */	stb r30, 0x24c(r29)
/* 800EA6BC 000E75FC  38 7C 00 00 */	addi r3, r28, 0
/* 800EA6C0 000E7600  38 80 00 64 */	li r4, 0x64
/* 800EA6C4 000E7604  9B DD 02 4D */	stb r30, 0x24d(r29)
/* 800EA6C8 000E7608  93 DD 02 50 */	stw r30, 0x250(r29)
/* 800EA6CC 000E760C  93 DD 02 5C */	stw r30, 0x25c(r29)
/* 800EA6D0 000E7610  9B DD 02 60 */	stb r30, 0x260(r29)
/* 800EA6D4 000E7614  93 AD 97 E8 */	stw r29, gpMarDirector@sda21(r13)
/* 800EA6D8 000E7618  93 DD 00 58 */	stw r30, 0x58(r29)
/* 800EA6DC 000E761C  93 DD 00 5C */	stw r30, 0x5c(r29)
/* 800EA6E0 000E7620  9B DD 00 64 */	stb r30, 0x64(r29)
/* 800EA6E4 000E7624  4B F4 F9 8D */	bl reserve__Q27JGadget20TVector_pointer_voidFUl
/* 800EA6E8 000E7628  7F A3 EB 78 */	mr r3, r29
/* 800EA6EC 000E762C  48 01 FD 19 */	bl initLoadParticle__12TMarDirectorFv
/* 800EA6F0 000E7630  9B DD 01 26 */	stb r30, 0x126(r29)
/* 800EA6F4 000E7634  38 7D 00 E8 */	addi r3, r29, 0xe8
/* 800EA6F8 000E7638  38 9F 00 24 */	addi r4, r31, 0x24
/* 800EA6FC 000E763C  9B DD 01 25 */	stb r30, 0x125(r29)
/* 800EA700 000E7640  9B DD 01 24 */	stb r30, 0x124(r29)
/* 800EA704 000E7644  4B FA 81 FD */	bl OSInitStopwatch
/* 800EA708 000E7648  80 01 00 64 */	lwz r0, 0x64(r1)
/* 800EA70C 000E764C  7F A3 EB 78 */	mr r3, r29
/* 800EA710 000E7650  83 E1 00 5C */	lwz r31, 0x5c(r1)
/* 800EA714 000E7654  83 C1 00 58 */	lwz r30, 0x58(r1)
/* 800EA718 000E7658  7C 08 03 A6 */	mtlr r0
/* 800EA71C 000E765C  83 A1 00 54 */	lwz r29, 0x54(r1)
/* 800EA720 000E7660  83 81 00 50 */	lwz r28, 0x50(r1)
/* 800EA724 000E7664  38 21 00 60 */	addi r1, r1, 0x60
/* 800EA728 000E7668  4E 80 00 20 */	blr 

.global __ct__Q212TMarDirector9TDemoInfoFv
__ct__Q212TMarDirector9TDemoInfoFv:
/* 800EA72C 000E766C  38 00 00 00 */	li r0, 0
/* 800EA730 000E7670  B0 03 00 20 */	sth r0, 0x20(r3)
/* 800EA734 000E7674  4E 80 00 20 */	blr 

.global Initialize___Q27JGadget19TSingleNodeLinkListFv
Initialize___Q27JGadget19TSingleNodeLinkListFv:
/* 800EA738 000E7678  38 80 00 00 */	li r4, 0
/* 800EA73C 000E767C  90 83 00 00 */	stw r4, 0(r3)
/* 800EA740 000E7680  38 03 00 04 */	addi r0, r3, 4
/* 800EA744 000E7684  90 83 00 04 */	stw r4, 4(r3)
/* 800EA748 000E7688  90 03 00 08 */	stw r0, 8(r3)
/* 800EA74C 000E768C  4E 80 00 20 */	blr 

.global __sinit_MarDirector_cpp
__sinit_MarDirector_cpp:
/* 800EA750 000E7690  7C 08 02 A6 */	mflr r0
/* 800EA754 000E7694  3C 60 80 3F */	lis r3, $$22269@ha
/* 800EA758 000E7698  90 01 00 04 */	stw r0, 4(r1)
/* 800EA75C 000E769C  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 800EA760 000E76A0  93 E1 00 0C */	stw r31, 0xc(r1)
/* 800EA764 000E76A4  3B E3 1F 40 */	addi r31, r3, $$22269@l
/* 800EA768 000E76A8  88 0D 97 CC */	lbz r0, __init__smList__15JALList$$05MSBgm$$1@sda21(r13)
/* 800EA76C 000E76AC  7C 00 07 75 */	extsb. r0, r0
/* 800EA770 000E76B0  40 82 00 28 */	bne lbl_800EA798
/* 800EA774 000E76B4  38 6D 97 A8 */	addi r3, r13, smList__15JALList$$05MSBgm$$1@sda21
/* 800EA778 000E76B8  4B F2 44 BD */	bl initiate__10JSUPtrListFv
/* 800EA77C 000E76BC  3C 60 80 0E */	lis r3, __dt__15JSUList$$05MSBgm$$1Fv@ha
/* 800EA780 000E76C0  38 83 6A 44 */	addi r4, r3, __dt__15JSUList$$05MSBgm$$1Fv@l
/* 800EA784 000E76C4  38 6D 97 A8 */	addi r3, r13, smList__15JALList$$05MSBgm$$1@sda21
/* 800EA788 000E76C8  38 BF 00 00 */	addi r5, r31, 0
/* 800EA78C 000E76CC  4B F9 7F 9D */	bl __register_global_object
/* 800EA790 000E76D0  38 00 00 01 */	li r0, 1
/* 800EA794 000E76D4  98 0D 97 CC */	stb r0, __init__smList__15JALList$$05MSBgm$$1@sda21(r13)
lbl_800EA798:
/* 800EA798 000E76D8  88 0D 97 CD */	lbz r0, __init__smList__24JALList$$013MSSetSoundGrp$$1@sda21(r13)
/* 800EA79C 000E76DC  7C 00 07 75 */	extsb. r0, r0
/* 800EA7A0 000E76E0  40 82 00 28 */	bne lbl_800EA7C8
/* 800EA7A4 000E76E4  38 6D 97 B4 */	addi r3, r13, smList__24JALList$$013MSSetSoundGrp$$1@sda21
/* 800EA7A8 000E76E8  4B F2 44 8D */	bl initiate__10JSUPtrListFv
/* 800EA7AC 000E76EC  3C 60 80 0E */	lis r3, __dt__24JSUList$$013MSSetSoundGrp$$1Fv@ha
/* 800EA7B0 000E76F0  38 83 69 EC */	addi r4, r3, __dt__24JSUList$$013MSSetSoundGrp$$1Fv@l
/* 800EA7B4 000E76F4  38 6D 97 B4 */	addi r3, r13, smList__24JALList$$013MSSetSoundGrp$$1@sda21
/* 800EA7B8 000E76F8  38 BF 00 0C */	addi r5, r31, 0xc
/* 800EA7BC 000E76FC  4B F9 7F 6D */	bl __register_global_object
/* 800EA7C0 000E7700  38 00 00 01 */	li r0, 1
/* 800EA7C4 000E7704  98 0D 97 CD */	stb r0, __init__smList__24JALList$$013MSSetSoundGrp$$1@sda21(r13)
lbl_800EA7C8:
/* 800EA7C8 000E7708  88 0D 97 CE */	lbz r0, __init__smList__21JALList$$010MSSetSound$$1@sda21(r13)
/* 800EA7CC 000E770C  7C 00 07 75 */	extsb. r0, r0
/* 800EA7D0 000E7710  40 82 00 28 */	bne lbl_800EA7F8
/* 800EA7D4 000E7714  38 6D 97 C0 */	addi r3, r13, smList__21JALList$$010MSSetSound$$1@sda21
/* 800EA7D8 000E7718  4B F2 44 5D */	bl initiate__10JSUPtrListFv
/* 800EA7DC 000E771C  3C 60 80 0E */	lis r3, __dt__21JSUList$$010MSSetSound$$1Fv@ha
/* 800EA7E0 000E7720  38 83 69 94 */	addi r4, r3, __dt__21JSUList$$010MSSetSound$$1Fv@l
/* 800EA7E4 000E7724  38 6D 97 C0 */	addi r3, r13, smList__21JALList$$010MSSetSound$$1@sda21
/* 800EA7E8 000E7728  38 BF 00 18 */	addi r5, r31, 0x18
/* 800EA7EC 000E772C  4B F9 7F 3D */	bl __register_global_object
/* 800EA7F0 000E7730  38 00 00 01 */	li r0, 1
/* 800EA7F4 000E7734  98 0D 97 CE */	stb r0, __init__smList__21JALList$$010MSSetSound$$1@sda21(r13)
lbl_800EA7F8:
/* 800EA7F8 000E7738  88 0D 8F 8C */	lbz r0, __init__smList__26JALList$$015JALSeModEffDGrp$$1@sda21(r13)
/* 800EA7FC 000E773C  7C 00 07 75 */	extsb. r0, r0
/* 800EA800 000E7740  40 82 00 28 */	bne lbl_800EA828
/* 800EA804 000E7744  38 6D 8E FC */	addi r3, r13, smList__26JALList$$015JALSeModEffDGrp$$1@sda21
/* 800EA808 000E7748  4B F2 44 2D */	bl initiate__10JSUPtrListFv
/* 800EA80C 000E774C  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffDGrp$$1Fv@ha
/* 800EA810 000E7750  38 83 A8 0C */	addi r4, r3, __dt__26JSUList$$015JALSeModEffDGrp$$1Fv@l
/* 800EA814 000E7754  38 6D 8E FC */	addi r3, r13, smList__26JALList$$015JALSeModEffDGrp$$1@sda21
/* 800EA818 000E7758  38 BF 00 24 */	addi r5, r31, 0x24
/* 800EA81C 000E775C  4B F9 7F 0D */	bl __register_global_object
/* 800EA820 000E7760  38 00 00 01 */	li r0, 1
/* 800EA824 000E7764  98 0D 8F 8C */	stb r0, __init__smList__26JALList$$015JALSeModEffDGrp$$1@sda21(r13)
lbl_800EA828:
/* 800EA828 000E7768  88 0D 8F 8D */	lbz r0, __init__smList__26JALList$$015JALSeModPitDGrp$$1@sda21(r13)
/* 800EA82C 000E776C  7C 00 07 75 */	extsb. r0, r0
/* 800EA830 000E7770  40 82 00 28 */	bne lbl_800EA858
/* 800EA834 000E7774  38 6D 8F 08 */	addi r3, r13, smList__26JALList$$015JALSeModPitDGrp$$1@sda21
/* 800EA838 000E7778  4B F2 43 FD */	bl initiate__10JSUPtrListFv
/* 800EA83C 000E777C  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitDGrp$$1Fv@ha
/* 800EA840 000E7780  38 83 A7 B4 */	addi r4, r3, __dt__26JSUList$$015JALSeModPitDGrp$$1Fv@l
/* 800EA844 000E7784  38 6D 8F 08 */	addi r3, r13, smList__26JALList$$015JALSeModPitDGrp$$1@sda21
/* 800EA848 000E7788  38 BF 00 30 */	addi r5, r31, 0x30
/* 800EA84C 000E778C  4B F9 7E DD */	bl __register_global_object
/* 800EA850 000E7790  38 00 00 01 */	li r0, 1
/* 800EA854 000E7794  98 0D 8F 8D */	stb r0, __init__smList__26JALList$$015JALSeModPitDGrp$$1@sda21(r13)
lbl_800EA858:
/* 800EA858 000E7798  88 0D 8F 8E */	lbz r0, __init__smList__26JALList$$015JALSeModVolDGrp$$1@sda21(r13)
/* 800EA85C 000E779C  7C 00 07 75 */	extsb. r0, r0
/* 800EA860 000E77A0  40 82 00 28 */	bne lbl_800EA888
/* 800EA864 000E77A4  38 6D 8F 14 */	addi r3, r13, smList__26JALList$$015JALSeModVolDGrp$$1@sda21
/* 800EA868 000E77A8  4B F2 43 CD */	bl initiate__10JSUPtrListFv
/* 800EA86C 000E77AC  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolDGrp$$1Fv@ha
/* 800EA870 000E77B0  38 83 A7 5C */	addi r4, r3, __dt__26JSUList$$015JALSeModVolDGrp$$1Fv@l
/* 800EA874 000E77B4  38 6D 8F 14 */	addi r3, r13, smList__26JALList$$015JALSeModVolDGrp$$1@sda21
/* 800EA878 000E77B8  38 BF 00 3C */	addi r5, r31, 0x3c
/* 800EA87C 000E77BC  4B F9 7E AD */	bl __register_global_object
/* 800EA880 000E77C0  38 00 00 01 */	li r0, 1
/* 800EA884 000E77C4  98 0D 8F 8E */	stb r0, __init__smList__26JALList$$015JALSeModVolDGrp$$1@sda21(r13)
lbl_800EA888:
/* 800EA888 000E77C8  88 0D 8F 8F */	lbz r0, __init__smList__26JALList$$015JALSeModEffFGrp$$1@sda21(r13)
/* 800EA88C 000E77CC  7C 00 07 75 */	extsb. r0, r0
/* 800EA890 000E77D0  40 82 00 28 */	bne lbl_800EA8B8
/* 800EA894 000E77D4  38 6D 8F 20 */	addi r3, r13, smList__26JALList$$015JALSeModEffFGrp$$1@sda21
/* 800EA898 000E77D8  4B F2 43 9D */	bl initiate__10JSUPtrListFv
/* 800EA89C 000E77DC  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffFGrp$$1Fv@ha
/* 800EA8A0 000E77E0  38 83 A7 04 */	addi r4, r3, __dt__26JSUList$$015JALSeModEffFGrp$$1Fv@l
/* 800EA8A4 000E77E4  38 6D 8F 20 */	addi r3, r13, smList__26JALList$$015JALSeModEffFGrp$$1@sda21
/* 800EA8A8 000E77E8  38 BF 00 48 */	addi r5, r31, 0x48
/* 800EA8AC 000E77EC  4B F9 7E 7D */	bl __register_global_object
/* 800EA8B0 000E77F0  38 00 00 01 */	li r0, 1
/* 800EA8B4 000E77F4  98 0D 8F 8F */	stb r0, __init__smList__26JALList$$015JALSeModEffFGrp$$1@sda21(r13)
lbl_800EA8B8:
/* 800EA8B8 000E77F8  88 0D 8F 90 */	lbz r0, __init__smList__26JALList$$015JALSeModPitFGrp$$1@sda21(r13)
/* 800EA8BC 000E77FC  7C 00 07 75 */	extsb. r0, r0
/* 800EA8C0 000E7800  40 82 00 28 */	bne lbl_800EA8E8
/* 800EA8C4 000E7804  38 6D 8F 2C */	addi r3, r13, smList__26JALList$$015JALSeModPitFGrp$$1@sda21
/* 800EA8C8 000E7808  4B F2 43 6D */	bl initiate__10JSUPtrListFv
/* 800EA8CC 000E780C  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitFGrp$$1Fv@ha
/* 800EA8D0 000E7810  38 83 A6 AC */	addi r4, r3, __dt__26JSUList$$015JALSeModPitFGrp$$1Fv@l
/* 800EA8D4 000E7814  38 6D 8F 2C */	addi r3, r13, smList__26JALList$$015JALSeModPitFGrp$$1@sda21
/* 800EA8D8 000E7818  38 BF 00 54 */	addi r5, r31, 0x54
/* 800EA8DC 000E781C  4B F9 7E 4D */	bl __register_global_object
/* 800EA8E0 000E7820  38 00 00 01 */	li r0, 1
/* 800EA8E4 000E7824  98 0D 8F 90 */	stb r0, __init__smList__26JALList$$015JALSeModPitFGrp$$1@sda21(r13)
lbl_800EA8E8:
/* 800EA8E8 000E7828  88 0D 8F 91 */	lbz r0, __init__smList__26JALList$$015JALSeModVolFGrp$$1@sda21(r13)
/* 800EA8EC 000E782C  7C 00 07 75 */	extsb. r0, r0
/* 800EA8F0 000E7830  40 82 00 28 */	bne lbl_800EA918
/* 800EA8F4 000E7834  38 6D 8F 38 */	addi r3, r13, smList__26JALList$$015JALSeModVolFGrp$$1@sda21
/* 800EA8F8 000E7838  4B F2 43 3D */	bl initiate__10JSUPtrListFv
/* 800EA8FC 000E783C  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolFGrp$$1Fv@ha
/* 800EA900 000E7840  38 83 A6 54 */	addi r4, r3, __dt__26JSUList$$015JALSeModVolFGrp$$1Fv@l
/* 800EA904 000E7844  38 6D 8F 38 */	addi r3, r13, smList__26JALList$$015JALSeModVolFGrp$$1@sda21
/* 800EA908 000E7848  38 BF 00 60 */	addi r5, r31, 0x60
/* 800EA90C 000E784C  4B F9 7E 1D */	bl __register_global_object
/* 800EA910 000E7850  38 00 00 01 */	li r0, 1
/* 800EA914 000E7854  98 0D 8F 91 */	stb r0, __init__smList__26JALList$$015JALSeModVolFGrp$$1@sda21(r13)
lbl_800EA918:
/* 800EA918 000E7858  88 0D 8F 92 */	lbz r0, __init__smList__26JALList$$015JALSeModEffDist$$1@sda21(r13)
/* 800EA91C 000E785C  7C 00 07 75 */	extsb. r0, r0
/* 800EA920 000E7860  40 82 00 28 */	bne lbl_800EA948
/* 800EA924 000E7864  38 6D 8F 44 */	addi r3, r13, smList__26JALList$$015JALSeModEffDist$$1@sda21
/* 800EA928 000E7868  4B F2 43 0D */	bl initiate__10JSUPtrListFv
/* 800EA92C 000E786C  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffDist$$1Fv@ha
/* 800EA930 000E7870  38 83 A5 FC */	addi r4, r3, __dt__26JSUList$$015JALSeModEffDist$$1Fv@l
/* 800EA934 000E7874  38 6D 8F 44 */	addi r3, r13, smList__26JALList$$015JALSeModEffDist$$1@sda21
/* 800EA938 000E7878  38 BF 00 6C */	addi r5, r31, 0x6c
/* 800EA93C 000E787C  4B F9 7D ED */	bl __register_global_object
/* 800EA940 000E7880  38 00 00 01 */	li r0, 1
/* 800EA944 000E7884  98 0D 8F 92 */	stb r0, __init__smList__26JALList$$015JALSeModEffDist$$1@sda21(r13)
lbl_800EA948:
/* 800EA948 000E7888  88 0D 8F 93 */	lbz r0, __init__smList__26JALList$$015JALSeModPitDist$$1@sda21(r13)
/* 800EA94C 000E788C  7C 00 07 75 */	extsb. r0, r0
/* 800EA950 000E7890  40 82 00 28 */	bne lbl_800EA978
/* 800EA954 000E7894  38 6D 8F 50 */	addi r3, r13, smList__26JALList$$015JALSeModPitDist$$1@sda21
/* 800EA958 000E7898  4B F2 42 DD */	bl initiate__10JSUPtrListFv
/* 800EA95C 000E789C  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitDist$$1Fv@ha
/* 800EA960 000E78A0  38 83 A5 A4 */	addi r4, r3, __dt__26JSUList$$015JALSeModPitDist$$1Fv@l
/* 800EA964 000E78A4  38 6D 8F 50 */	addi r3, r13, smList__26JALList$$015JALSeModPitDist$$1@sda21
/* 800EA968 000E78A8  38 BF 00 78 */	addi r5, r31, 0x78
/* 800EA96C 000E78AC  4B F9 7D BD */	bl __register_global_object
/* 800EA970 000E78B0  38 00 00 01 */	li r0, 1
/* 800EA974 000E78B4  98 0D 8F 93 */	stb r0, __init__smList__26JALList$$015JALSeModPitDist$$1@sda21(r13)
lbl_800EA978:
/* 800EA978 000E78B8  88 0D 8F 94 */	lbz r0, __init__smList__26JALList$$015JALSeModVolDist$$1@sda21(r13)
/* 800EA97C 000E78BC  7C 00 07 75 */	extsb. r0, r0
/* 800EA980 000E78C0  40 82 00 28 */	bne lbl_800EA9A8
/* 800EA984 000E78C4  38 6D 8F 5C */	addi r3, r13, smList__26JALList$$015JALSeModVolDist$$1@sda21
/* 800EA988 000E78C8  4B F2 42 AD */	bl initiate__10JSUPtrListFv
/* 800EA98C 000E78CC  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolDist$$1Fv@ha
/* 800EA990 000E78D0  38 83 A5 4C */	addi r4, r3, __dt__26JSUList$$015JALSeModVolDist$$1Fv@l
/* 800EA994 000E78D4  38 6D 8F 5C */	addi r3, r13, smList__26JALList$$015JALSeModVolDist$$1@sda21
/* 800EA998 000E78D8  38 BF 00 84 */	addi r5, r31, 0x84
/* 800EA99C 000E78DC  4B F9 7D 8D */	bl __register_global_object
/* 800EA9A0 000E78E0  38 00 00 01 */	li r0, 1
/* 800EA9A4 000E78E4  98 0D 8F 94 */	stb r0, __init__smList__26JALList$$015JALSeModVolDist$$1@sda21(r13)
lbl_800EA9A8:
/* 800EA9A8 000E78E8  88 0D 8F 95 */	lbz r0, __init__smList__26JALList$$015JALSeModEffFunk$$1@sda21(r13)
/* 800EA9AC 000E78EC  7C 00 07 75 */	extsb. r0, r0
/* 800EA9B0 000E78F0  40 82 00 28 */	bne lbl_800EA9D8
/* 800EA9B4 000E78F4  38 6D 8F 68 */	addi r3, r13, smList__26JALList$$015JALSeModEffFunk$$1@sda21
/* 800EA9B8 000E78F8  4B F2 42 7D */	bl initiate__10JSUPtrListFv
/* 800EA9BC 000E78FC  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffFunk$$1Fv@ha
/* 800EA9C0 000E7900  38 83 A4 F4 */	addi r4, r3, __dt__26JSUList$$015JALSeModEffFunk$$1Fv@l
/* 800EA9C4 000E7904  38 6D 8F 68 */	addi r3, r13, smList__26JALList$$015JALSeModEffFunk$$1@sda21
/* 800EA9C8 000E7908  38 BF 00 90 */	addi r5, r31, 0x90
/* 800EA9CC 000E790C  4B F9 7D 5D */	bl __register_global_object
/* 800EA9D0 000E7910  38 00 00 01 */	li r0, 1
/* 800EA9D4 000E7914  98 0D 8F 95 */	stb r0, __init__smList__26JALList$$015JALSeModEffFunk$$1@sda21(r13)
lbl_800EA9D8:
/* 800EA9D8 000E7918  88 0D 8F 96 */	lbz r0, __init__smList__26JALList$$015JALSeModPitFunk$$1@sda21(r13)
/* 800EA9DC 000E791C  7C 00 07 75 */	extsb. r0, r0
/* 800EA9E0 000E7920  40 82 00 28 */	bne lbl_800EAA08
/* 800EA9E4 000E7924  38 6D 8F 74 */	addi r3, r13, smList__26JALList$$015JALSeModPitFunk$$1@sda21
/* 800EA9E8 000E7928  4B F2 42 4D */	bl initiate__10JSUPtrListFv
/* 800EA9EC 000E792C  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitFunk$$1Fv@ha
/* 800EA9F0 000E7930  38 83 A4 9C */	addi r4, r3, __dt__26JSUList$$015JALSeModPitFunk$$1Fv@l
/* 800EA9F4 000E7934  38 6D 8F 74 */	addi r3, r13, smList__26JALList$$015JALSeModPitFunk$$1@sda21
/* 800EA9F8 000E7938  38 BF 00 9C */	addi r5, r31, 0x9c
/* 800EA9FC 000E793C  4B F9 7D 2D */	bl __register_global_object
/* 800EAA00 000E7940  38 00 00 01 */	li r0, 1
/* 800EAA04 000E7944  98 0D 8F 96 */	stb r0, __init__smList__26JALList$$015JALSeModPitFunk$$1@sda21(r13)
lbl_800EAA08:
/* 800EAA08 000E7948  88 0D 8F 97 */	lbz r0, __init__smList__26JALList$$015JALSeModVolFunk$$1@sda21(r13)
/* 800EAA0C 000E794C  7C 00 07 75 */	extsb. r0, r0
/* 800EAA10 000E7950  40 82 00 28 */	bne lbl_800EAA38
/* 800EAA14 000E7954  38 6D 8F 80 */	addi r3, r13, smList__26JALList$$015JALSeModVolFunk$$1@sda21
/* 800EAA18 000E7958  4B F2 42 1D */	bl initiate__10JSUPtrListFv
/* 800EAA1C 000E795C  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolFunk$$1Fv@ha
/* 800EAA20 000E7960  38 83 A4 44 */	addi r4, r3, __dt__26JSUList$$015JALSeModVolFunk$$1Fv@l
/* 800EAA24 000E7964  38 6D 8F 80 */	addi r3, r13, smList__26JALList$$015JALSeModVolFunk$$1@sda21
/* 800EAA28 000E7968  38 BF 00 A8 */	addi r5, r31, 0xa8
/* 800EAA2C 000E796C  4B F9 7C FD */	bl __register_global_object
/* 800EAA30 000E7970  38 00 00 01 */	li r0, 1
/* 800EAA34 000E7974  98 0D 8F 97 */	stb r0, __init__smList__26JALList$$015JALSeModVolFunk$$1@sda21(r13)
lbl_800EAA38:
/* 800EAA38 000E7978  80 01 00 14 */	lwz r0, 0x14(r1)
/* 800EAA3C 000E797C  83 E1 00 0C */	lwz r31, 0xc(r1)
/* 800EAA40 000E7980  38 21 00 10 */	addi r1, r1, 0x10
/* 800EAA44 000E7984  7C 08 03 A6 */	mtlr r0
/* 800EAA48 000E7988  4E 80 00 20 */	blr 

.section .ctors, "wa"  # 0x8036FBA0 - 0x8036FF80
  .4byte __sinit_MarDirector_cpp

.section .rodata, "wa"  # 0x8036FFA0 - 0x803A8380
$$21490:
  .4byte 0
  .4byte 0
  .4byte 0
$$22396:
	.incbin "baserom.dol", 0x3702C4, 0xC
$$22397:
	.incbin "baserom.dol", 0x3702D0, 0xC
$$22399:
	.incbin "baserom.dol", 0x3702DC, 0x1C

.section .sdata2, "wa"  # 0x8040B460 - 0x80414020
$$22398:
	.incbin "baserom.dol", 0x3E4D40, 0x8

.section .bss, "wa"  # 0x803E6000 - 0x80408AC0
$$22269:
	.skip 0xC
$$22270:
	.skip 0xC
$$22271:
	.skip 0xC
$$22272:
	.skip 0xC
$$22273:
	.skip 0xC
$$22274:
	.skip 0xC
$$22275:
	.skip 0xC
$$22276:
	.skip 0xC
$$22277:
	.skip 0xC
$$22278:
	.skip 0xC
$$22279:
	.skip 0xC
$$22280:
	.skip 0xC
$$22281:
	.skip 0xC
$$22282:
	.skip 0xC
$$22283:
	.skip 0x10

.section .sbss, "wa"  # 0x804097C0 - 0x8040B45C
.balign 8
.global gpSceneCmnDat
gpSceneCmnDat:
	.skip 0x4
.global gpSceneCmnDatSize
gpSceneCmnDatSize:
	.skip 0x4
