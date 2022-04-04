.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global __dt__21TCoasterKillerManagerFv
__dt__21TCoasterKillerManagerFv:
/* 803454E8 00342428  7C 08 02 A6 */	mflr r0
/* 803454EC 0034242C  90 01 00 04 */	stw r0, 4(r1)
/* 803454F0 00342430  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 803454F4 00342434  93 E1 00 14 */	stw r31, 0x14(r1)
/* 803454F8 00342438  3B E4 00 00 */	addi r31, r4, 0
/* 803454FC 0034243C  93 C1 00 10 */	stw r30, 0x10(r1)
/* 80345500 00342440  7C 7E 1B 79 */	or. r30, r3, r3
/* 80345504 00342444  41 82 00 3C */	beq lbl_80345540
/* 80345508 00342448  3C 60 80 3E */	lis r3, __vt__21TCoasterKillerManager@ha
/* 8034550C 0034244C  38 03 34 48 */	addi r0, r3, __vt__21TCoasterKillerManager@l
/* 80345510 00342450  90 1E 00 00 */	stw r0, 0(r30)
/* 80345514 00342454  41 82 00 1C */	beq lbl_80345530
/* 80345518 00342458  3C 60 80 3D */	lis r3, __vt__18TSmallEnemyManager@ha
/* 8034551C 0034245C  38 03 6D 88 */	addi r0, r3, __vt__18TSmallEnemyManager@l
/* 80345520 00342460  90 1E 00 00 */	stw r0, 0(r30)
/* 80345524 00342464  38 7E 00 00 */	addi r3, r30, 0
/* 80345528 00342468  38 80 00 00 */	li r4, 0
/* 8034552C 0034246C  4B F0 D4 7D */	bl __dt__13TEnemyManagerFv
lbl_80345530:
/* 80345530 00342470  7F E0 07 35 */	extsh. r0, r31
/* 80345534 00342474  40 81 00 0C */	ble lbl_80345540
/* 80345538 00342478  7F C3 F3 78 */	mr r3, r30
/* 8034553C 0034247C  4B CC 75 75 */	bl __dl__FPv
lbl_80345540:
/* 80345540 00342480  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 80345544 00342484  7F C3 F3 78 */	mr r3, r30
/* 80345548 00342488  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 8034554C 0034248C  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 80345550 00342490  7C 08 03 A6 */	mtlr r0
/* 80345554 00342494  38 21 00 18 */	addi r1, r1, 0x18
/* 80345558 00342498  4E 80 00 20 */	blr 

.global createEnemyInstance__21TCoasterKillerManagerFv
createEnemyInstance__21TCoasterKillerManagerFv:
/* 8034555C 0034249C  7C 08 02 A6 */	mflr r0
/* 80345560 003424A0  38 60 02 00 */	li r3, 0x200
/* 80345564 003424A4  90 01 00 04 */	stw r0, 4(r1)
/* 80345568 003424A8  94 21 FF E0 */	stwu r1, -0x20(r1)
/* 8034556C 003424AC  93 E1 00 1C */	stw r31, 0x1c(r1)
/* 80345570 003424B0  4B CC 73 41 */	bl __nw__FUl
/* 80345574 003424B4  7C 7F 1B 79 */	or. r31, r3, r3
/* 80345578 003424B8  41 82 00 64 */	beq lbl_803455DC
/* 8034557C 003424BC  93 E1 00 0C */	stw r31, 0xc(r1)
/* 80345580 003424C0  3C 60 80 3A */	lis r3, "@2953"@ha
/* 80345584 003424C4  38 83 4C 44 */	addi r4, r3, "@2953"@l
/* 80345588 003424C8  93 E1 00 10 */	stw r31, 0x10(r1)
/* 8034558C 003424CC  80 61 00 0C */	lwz r3, 0xc(r1)
/* 80345590 003424D0  4B F3 FC 5D */	bl __ct__12TWalkerEnemyFPCc
/* 80345594 003424D4  80 81 00 0C */	lwz r4, 0xc(r1)
/* 80345598 003424D8  3C 60 80 3E */	lis r3, __vt__13TCoasterEnemy@ha
/* 8034559C 003424DC  38 63 36 84 */	addi r3, r3, __vt__13TCoasterEnemy@l
/* 803455A0 003424E0  90 64 00 00 */	stw r3, 0(r4)
/* 803455A4 003424E4  38 63 00 24 */	addi r3, r3, 0x24
/* 803455A8 003424E8  38 00 00 00 */	li r0, 0
/* 803455AC 003424EC  90 64 00 20 */	stw r3, 0x20(r4)
/* 803455B0 003424F0  38 64 01 9C */	addi r3, r4, 0x19c
/* 803455B4 003424F4  90 04 01 98 */	stw r0, 0x198(r4)
/* 803455B8 003424F8  4B F5 46 D5 */	bl "__ct__Q29JGeometry8TVec4<f>Fv"
/* 803455BC 003424FC  80 81 00 10 */	lwz r4, 0x10(r1)
/* 803455C0 00342500  3C 60 80 3E */	lis r3, __vt__14TCoasterKiller@ha
/* 803455C4 00342504  38 63 34 B0 */	addi r3, r3, __vt__14TCoasterKiller@l
/* 803455C8 00342508  90 64 00 00 */	stw r3, 0(r4)
/* 803455CC 0034250C  38 03 00 24 */	addi r0, r3, 0x24
/* 803455D0 00342510  38 64 01 D0 */	addi r3, r4, 0x1d0
/* 803455D4 00342514  90 04 00 20 */	stw r0, 0x20(r4)
/* 803455D8 00342518  4B F0 F6 61 */	bl "__ct__Q29JGeometry38TMatrix34<Q29JGeometry13SMatrix34C<f>>Fv"
lbl_803455DC:
/* 803455DC 0034251C  80 01 00 24 */	lwz r0, 0x24(r1)
/* 803455E0 00342520  7F E3 FB 78 */	mr r3, r31
/* 803455E4 00342524  83 E1 00 1C */	lwz r31, 0x1c(r1)
/* 803455E8 00342528  38 21 00 20 */	addi r1, r1, 0x20
/* 803455EC 0034252C  7C 08 03 A6 */	mtlr r0
/* 803455F0 00342530  4E 80 00 20 */	blr 

.global __dt__13TCoasterEnemyFv
__dt__13TCoasterEnemyFv:
/* 803455F4 00342534  7C 08 02 A6 */	mflr r0
/* 803455F8 00342538  90 01 00 04 */	stw r0, 4(r1)
/* 803455FC 0034253C  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 80345600 00342540  93 E1 00 14 */	stw r31, 0x14(r1)
/* 80345604 00342544  3B E4 00 00 */	addi r31, r4, 0
/* 80345608 00342548  93 C1 00 10 */	stw r30, 0x10(r1)
/* 8034560C 0034254C  7C 7E 1B 79 */	or. r30, r3, r3
/* 80345610 00342550  41 82 00 64 */	beq lbl_80345674
/* 80345614 00342554  3C 60 80 3E */	lis r3, __vt__13TCoasterEnemy@ha
/* 80345618 00342558  38 63 36 84 */	addi r3, r3, __vt__13TCoasterEnemy@l
/* 8034561C 0034255C  90 7E 00 00 */	stw r3, 0(r30)
/* 80345620 00342560  38 03 00 24 */	addi r0, r3, 0x24
/* 80345624 00342564  90 1E 00 20 */	stw r0, 0x20(r30)
/* 80345628 00342568  41 82 00 3C */	beq lbl_80345664
/* 8034562C 0034256C  3C 60 80 3D */	lis r3, __vt__12TWalkerEnemy@ha
/* 80345630 00342570  38 63 6F E8 */	addi r3, r3, __vt__12TWalkerEnemy@l
/* 80345634 00342574  90 7E 00 00 */	stw r3, 0(r30)
/* 80345638 00342578  38 03 00 24 */	addi r0, r3, 0x24
/* 8034563C 0034257C  90 1E 00 20 */	stw r0, 0x20(r30)
/* 80345640 00342580  41 82 00 24 */	beq lbl_80345664
/* 80345644 00342584  3C 60 80 3D */	lis r3, __vt__11TSmallEnemy@ha
/* 80345648 00342588  38 63 6B E0 */	addi r3, r3, __vt__11TSmallEnemy@l
/* 8034564C 0034258C  90 7E 00 00 */	stw r3, 0(r30)
/* 80345650 00342590  38 03 00 24 */	addi r0, r3, 0x24
/* 80345654 00342594  38 7E 00 00 */	addi r3, r30, 0
/* 80345658 00342598  90 1E 00 20 */	stw r0, 0x20(r30)
/* 8034565C 0034259C  38 80 00 00 */	li r4, 0
/* 80345660 003425A0  4B F0 AB 59 */	bl __dt__11TSpineEnemyFv
lbl_80345664:
/* 80345664 003425A4  7F E0 07 35 */	extsh. r0, r31
/* 80345668 003425A8  40 81 00 0C */	ble lbl_80345674
/* 8034566C 003425AC  7F C3 F3 78 */	mr r3, r30
/* 80345670 003425B0  4B CC 74 41 */	bl __dl__FPv
lbl_80345674:
/* 80345674 003425B4  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 80345678 003425B8  7F C3 F3 78 */	mr r3, r30
/* 8034567C 003425BC  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 80345680 003425C0  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 80345684 003425C4  7C 08 03 A6 */	mtlr r0
/* 80345688 003425C8  38 21 00 18 */	addi r1, r1, 0x18
/* 8034568C 003425CC  4E 80 00 20 */	blr 

.global createModelData__21TCoasterKillerManagerFv
createModelData__21TCoasterKillerManagerFv:
/* 80345690 003425D0  7C 08 02 A6 */	mflr r0
/* 80345694 003425D4  3C 80 80 3E */	lis r4, entry$2930@ha
/* 80345698 003425D8  90 01 00 04 */	stw r0, 4(r1)
/* 8034569C 003425DC  38 84 34 24 */	addi r4, r4, entry$2930@l
/* 803456A0 003425E0  94 21 FF F8 */	stwu r1, -8(r1)
/* 803456A4 003425E4  81 83 00 00 */	lwz r12, 0(r3)
/* 803456A8 003425E8  81 8C 00 2C */	lwz r12, 0x2c(r12)
/* 803456AC 003425EC  7D 88 03 A6 */	mtlr r12
/* 803456B0 003425F0  4E 80 00 21 */	blrl 
/* 803456B4 003425F4  80 01 00 0C */	lwz r0, 0xc(r1)
/* 803456B8 003425F8  38 21 00 08 */	addi r1, r1, 8
/* 803456BC 003425FC  7C 08 03 A6 */	mtlr r0
/* 803456C0 00342600  4E 80 00 20 */	blr 

.global loadAfter__21TCoasterKillerManagerFv
loadAfter__21TCoasterKillerManagerFv:
/* 803456C4 00342604  7C 08 02 A6 */	mflr r0
/* 803456C8 00342608  90 01 00 04 */	stw r0, 4(r1)
/* 803456CC 0034260C  94 21 FF C8 */	stwu r1, -0x38(r1)
/* 803456D0 00342610  93 E1 00 34 */	stw r31, 0x34(r1)
/* 803456D4 00342614  7C 7F 1B 78 */	mr r31, r3
/* 803456D8 00342618  4B F3 B6 D1 */	bl loadAfter__18TSmallEnemyManagerFv
/* 803456DC 0034261C  80 1F 00 38 */	lwz r0, 0x38(r31)
/* 803456E0 00342620  28 00 00 00 */	cmplwi r0, 0
/* 803456E4 00342624  80 01 00 3C */	lwz r0, 0x3c(r1)
/* 803456E8 00342628  83 E1 00 34 */	lwz r31, 0x34(r1)
/* 803456EC 0034262C  38 21 00 38 */	addi r1, r1, 0x38
/* 803456F0 00342630  7C 08 03 A6 */	mtlr r0
/* 803456F4 00342634  4E 80 00 20 */	blr 

.global load__21TCoasterKillerManagerFR20JSUMemoryInputStream
load__21TCoasterKillerManagerFR20JSUMemoryInputStream:
/* 803456F8 00342638  7C 08 02 A6 */	mflr r0
/* 803456FC 0034263C  90 01 00 04 */	stw r0, 4(r1)
/* 80345700 00342640  94 21 FF 90 */	stwu r1, -0x70(r1)
/* 80345704 00342644  BF 61 00 5C */	stmw r27, 0x5c(r1)
/* 80345708 00342648  7C 7F 1B 78 */	mr r31, r3
/* 8034570C 0034264C  80 03 00 38 */	lwz r0, 0x38(r3)
/* 80345710 00342650  3C 60 80 3A */	lis r3, "@1490"@ha
/* 80345714 00342654  3B 83 4A E8 */	addi r28, r3, "@1490"@l
/* 80345718 00342658  28 00 00 00 */	cmplwi r0, 0
/* 8034571C 0034265C  7F E3 FB 78 */	mr r3, r31
/* 80345720 00342660  4B F3 B6 E1 */	bl load__18TSmallEnemyManagerFR20JSUMemoryInputStream
/* 80345724 00342664  38 60 03 68 */	li r3, 0x368
/* 80345728 00342668  4B CC 71 89 */	bl __nw__FUl
/* 8034572C 0034266C  7C 7E 1B 79 */	or. r30, r3, r3
/* 80345730 00342670  41 82 00 CC */	beq lbl_803457FC
/* 80345734 00342674  93 C1 00 44 */	stw r30, 0x44(r1)
/* 80345738 00342678  38 9C 01 70 */	addi r4, r28, 0x170
/* 8034573C 0034267C  93 C1 00 50 */	stw r30, 0x50(r1)
/* 80345740 00342680  80 61 00 44 */	lwz r3, 0x44(r1)
/* 80345744 00342684  4B F3 FA ED */	bl __ct__18TWalkerEnemyParamsFPCc
/* 80345748 00342688  38 7C 01 8C */	addi r3, r28, 0x18c
/* 8034574C 0034268C  4B CF EF 41 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 80345750 00342690  83 61 00 44 */	lwz r27, 0x44(r1)
/* 80345754 00342694  38 A3 00 00 */	addi r5, r3, 0
/* 80345758 00342698  C0 22 02 70 */	lfs f1, "@3038"@sda21(r2)
/* 8034575C 0034269C  38 DC 01 8C */	addi r6, r28, 0x18c
/* 80345760 003426A0  38 9B 00 00 */	addi r4, r27, 0
/* 80345764 003426A4  38 7B 03 2C */	addi r3, r27, 0x32c
/* 80345768 003426A8  4B D8 21 F5 */	bl "__ct__10TParamT<f>FP7TParamsfUsPCc"
/* 8034576C 003426AC  3C 60 80 3B */	lis r3, "__vt__11TParamRT<f>"@ha
/* 80345770 003426B0  3B A3 29 F4 */	addi r29, r3, "__vt__11TParamRT<f>"@l
/* 80345774 003426B4  93 BB 03 2C */	stw r29, 0x32c(r27)
/* 80345778 003426B8  38 7C 01 A4 */	addi r3, r28, 0x1a4
/* 8034577C 003426BC  4B CF EF 11 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 80345780 003426C0  C0 22 02 70 */	lfs f1, "@3038"@sda21(r2)
/* 80345784 003426C4  38 A3 00 00 */	addi r5, r3, 0
/* 80345788 003426C8  38 9B 00 00 */	addi r4, r27, 0
/* 8034578C 003426CC  38 7B 03 40 */	addi r3, r27, 0x340
/* 80345790 003426D0  38 DC 01 A4 */	addi r6, r28, 0x1a4
/* 80345794 003426D4  4B D8 21 C9 */	bl "__ct__10TParamT<f>FP7TParamsfUsPCc"
/* 80345798 003426D8  93 BB 03 40 */	stw r29, 0x340(r27)
/* 8034579C 003426DC  7F 63 DB 78 */	mr r3, r27
/* 803457A0 003426E0  80 9B 00 00 */	lwz r4, 0(r27)
/* 803457A4 003426E4  4B DB 27 F5 */	bl load__7TParamsFPCc
/* 803457A8 003426E8  C0 0D 8C 50 */	lfs f0, "@2842"@sda21(r13)
/* 803457AC 003426EC  38 7C 01 BC */	addi r3, r28, 0x1bc
/* 803457B0 003426F0  D0 1B 03 3C */	stfs f0, 0x33c(r27)
/* 803457B4 003426F4  C0 0D 8C 54 */	lfs f0, "@2843"@sda21(r13)
/* 803457B8 003426F8  D0 1B 03 50 */	stfs f0, 0x350(r27)
/* 803457BC 003426FC  4B CF EE D1 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 803457C0 00342700  83 61 00 50 */	lwz r27, 0x50(r1)
/* 803457C4 00342704  38 A3 00 00 */	addi r5, r3, 0
/* 803457C8 00342708  38 DC 01 BC */	addi r6, r28, 0x1bc
/* 803457CC 0034270C  38 9B 00 00 */	addi r4, r27, 0
/* 803457D0 00342710  38 7B 03 54 */	addi r3, r27, 0x354
/* 803457D4 00342714  4B D9 54 21 */	bl __ct__10TBaseParamFP7TParamsUsPCc
/* 803457D8 00342718  3C 60 80 3B */	lis r3, "__vt__10TParamT<f>"@ha
/* 803457DC 0034271C  38 03 29 B8 */	addi r0, r3, "__vt__10TParamT<f>"@l
/* 803457E0 00342720  90 1B 03 54 */	stw r0, 0x354(r27)
/* 803457E4 00342724  7F 63 DB 78 */	mr r3, r27
/* 803457E8 00342728  C0 02 02 74 */	lfs f0, "@3041"@sda21(r2)
/* 803457EC 0034272C  D0 1B 03 64 */	stfs f0, 0x364(r27)
/* 803457F0 00342730  93 BB 03 54 */	stw r29, 0x354(r27)
/* 803457F4 00342734  80 9B 00 00 */	lwz r4, 0(r27)
/* 803457F8 00342738  4B DB 27 A1 */	bl load__7TParamsFPCc
lbl_803457FC:
/* 803457FC 0034273C  93 DF 00 38 */	stw r30, 0x38(r31)
/* 80345800 00342740  80 1F 00 38 */	lwz r0, 0x38(r31)
/* 80345804 00342744  28 00 00 00 */	cmplwi r0, 0
/* 80345808 00342748  BB 61 00 5C */	lmw r27, 0x5c(r1)
/* 8034580C 0034274C  80 01 00 74 */	lwz r0, 0x74(r1)
/* 80345810 00342750  38 21 00 70 */	addi r1, r1, 0x70
/* 80345814 00342754  7C 08 03 A6 */	mtlr r0
/* 80345818 00342758  4E 80 00 20 */	blr 

.global __ct__21TCoasterKillerManagerFPCc
__ct__21TCoasterKillerManagerFPCc:
/* 8034581C 0034275C  7C 08 02 A6 */	mflr r0
/* 80345820 00342760  90 01 00 04 */	stw r0, 4(r1)
/* 80345824 00342764  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 80345828 00342768  93 E1 00 14 */	stw r31, 0x14(r1)
/* 8034582C 0034276C  7C 7F 1B 78 */	mr r31, r3
/* 80345830 00342770  4B F3 B6 25 */	bl __ct__18TSmallEnemyManagerFPCc
/* 80345834 00342774  3C 60 80 3E */	lis r3, __vt__21TCoasterKillerManager@ha
/* 80345838 00342778  38 03 34 48 */	addi r0, r3, __vt__21TCoasterKillerManager@l
/* 8034583C 0034277C  90 1F 00 00 */	stw r0, 0(r31)
/* 80345840 00342780  38 00 00 00 */	li r0, 0
/* 80345844 00342784  38 7F 00 00 */	addi r3, r31, 0
/* 80345848 00342788  98 1F 00 60 */	stb r0, 0x60(r31)
/* 8034584C 0034278C  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 80345850 00342790  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 80345854 00342794  38 21 00 18 */	addi r1, r1, 0x18
/* 80345858 00342798  7C 08 03 A6 */	mtlr r0
/* 8034585C 0034279C  4E 80 00 20 */	blr 

.global "execute__28TNerveCoasterKillerExplosionCFP24TSpineBase<10TLiveActor>"
"execute__28TNerveCoasterKillerExplosionCFP24TSpineBase<10TLiveActor>":
/* 80345860 003427A0  7C 08 02 A6 */	mflr r0
/* 80345864 003427A4  90 01 00 04 */	stw r0, 4(r1)
/* 80345868 003427A8  94 21 FF B8 */	stwu r1, -0x48(r1)
/* 8034586C 003427AC  93 E1 00 44 */	stw r31, 0x44(r1)
/* 80345870 003427B0  7C 9F 23 78 */	mr r31, r4
/* 80345874 003427B4  93 C1 00 40 */	stw r30, 0x40(r1)
/* 80345878 003427B8  93 A1 00 3C */	stw r29, 0x3c(r1)
/* 8034587C 003427BC  80 04 00 20 */	lwz r0, 0x20(r4)
/* 80345880 003427C0  83 A4 00 00 */	lwz r29, 0(r4)
/* 80345884 003427C4  2C 00 00 00 */	cmpwi r0, 0
/* 80345888 003427C8  40 82 00 5C */	bne lbl_803458E4
/* 8034588C 003427CC  7F A3 EB 78 */	mr r3, r29
/* 80345890 003427D0  81 9D 00 00 */	lwz r12, 0(r29)
/* 80345894 003427D4  81 8C 01 08 */	lwz r12, 0x108(r12)
/* 80345898 003427D8  7D 88 03 A6 */	mtlr r12
/* 8034589C 003427DC  4E 80 00 21 */	blrl 
/* 803458A0 003427E0  C0 43 03 64 */	lfs f2, 0x364(r3)
/* 803458A4 003427E4  7F A3 EB 78 */	mr r3, r29
/* 803458A8 003427E8  C0 3D 01 48 */	lfs f1, 0x148(r29)
/* 803458AC 003427EC  C0 1D 00 50 */	lfs f0, 0x50(r29)
/* 803458B0 003427F0  EC 22 00 72 */	fmuls f1, f2, f1
/* 803458B4 003427F4  EC 01 00 24 */	fdivs f0, f1, f0
/* 803458B8 003427F8  D0 1D 01 AC */	stfs f0, 0x1ac(r29)
/* 803458BC 003427FC  C0 02 02 78 */	lfs f0, "@3103"@sda21(r2)
/* 803458C0 00342800  D0 1D 00 30 */	stfs f0, 0x30(r29)
/* 803458C4 00342804  81 9D 00 00 */	lwz r12, 0(r29)
/* 803458C8 00342808  81 8C 01 54 */	lwz r12, 0x154(r12)
/* 803458CC 0034280C  7D 88 03 A6 */	mtlr r12
/* 803458D0 00342810  4E 80 00 21 */	blrl 
/* 803458D4 00342814  80 6D A8 C0 */	lwz r3, gpCameraShake@sda21(r13)
/* 803458D8 00342818  38 80 00 06 */	li r4, 6
/* 803458DC 0034281C  C0 22 02 7C */	lfs f1, "@3104"@sda21(r2)
/* 803458E0 00342820  48 01 6B DD */	bl startShake__12TCameraShakeF16EnumCamShakeModef
lbl_803458E4:
/* 803458E4 00342824  C0 3D 01 90 */	lfs f1, 0x190(r29)
/* 803458E8 00342828  C0 1D 01 AC */	lfs f0, 0x1ac(r29)
/* 803458EC 0034282C  FC 01 00 40 */	fcmpo cr0, f1, f0
/* 803458F0 00342830  40 80 00 14 */	bge lbl_80345904
/* 803458F4 00342834  C0 02 02 80 */	lfs f0, "@3105"@sda21(r2)
/* 803458F8 00342838  EC 01 00 32 */	fmuls f0, f1, f0
/* 803458FC 0034283C  D0 1D 01 90 */	stfs f0, 0x190(r29)
/* 80345900 00342840  48 00 00 B8 */	b lbl_803459B8
lbl_80345904:
/* 80345904 00342844  80 1D 00 64 */	lwz r0, 0x64(r29)
/* 80345908 00342848  38 7D 00 00 */	addi r3, r29, 0
/* 8034590C 0034284C  38 80 00 00 */	li r4, 0
/* 80345910 00342850  60 00 00 01 */	ori r0, r0, 1
/* 80345914 00342854  90 1D 00 64 */	stw r0, 0x64(r29)
/* 80345918 00342858  4B F0 83 41 */	bl checkCurAnmEnd__11TSpineEnemyCFi
/* 8034591C 0034285C  2C 03 00 00 */	cmpwi r3, 0
/* 80345920 00342860  41 82 00 98 */	beq lbl_803459B8
/* 80345924 00342864  80 1D 00 F0 */	lwz r0, 0xf0(r29)
/* 80345928 00342868  3B C0 00 00 */	li r30, 0
/* 8034592C 0034286C  38 7D 00 00 */	addi r3, r29, 0
/* 80345930 00342870  60 00 00 01 */	ori r0, r0, 1
/* 80345934 00342874  90 1D 00 F0 */	stw r0, 0xf0(r29)
/* 80345938 00342878  80 1D 00 F0 */	lwz r0, 0xf0(r29)
/* 8034593C 0034287C  60 00 00 08 */	ori r0, r0, 8
/* 80345940 00342880  90 1D 00 F0 */	stw r0, 0xf0(r29)
/* 80345944 00342884  80 1D 00 F0 */	lwz r0, 0xf0(r29)
/* 80345948 00342888  54 00 04 1C */	rlwinm r0, r0, 0, 0x10, 0xe
/* 8034594C 0034288C  90 1D 00 F0 */	stw r0, 0xf0(r29)
/* 80345950 00342890  93 DD 00 68 */	stw r30, 0x68(r29)
/* 80345954 00342894  4B DC B6 F1 */	bl stopAnmSound__10TLiveActorFv
/* 80345958 00342898  93 DF 00 08 */	stw r30, 8(r31)
/* 8034595C 0034289C  4B F3 85 91 */	bl theNerve__19TNerveSmallEnemyDieFv
/* 80345960 003428A0  80 1F 00 14 */	lwz r0, 0x14(r31)
/* 80345964 003428A4  28 00 00 00 */	cmplwi r0, 0
/* 80345968 003428A8  41 82 00 08 */	beq lbl_80345970
/* 8034596C 003428AC  90 1F 00 1C */	stw r0, 0x1c(r31)
lbl_80345970:
/* 80345970 003428B0  38 00 00 00 */	li r0, 0
/* 80345974 003428B4  90 1F 00 20 */	stw r0, 0x20(r31)
/* 80345978 003428B8  90 7F 00 14 */	stw r3, 0x14(r31)
/* 8034597C 003428BC  80 BF 00 18 */	lwz r5, 0x18(r31)
/* 80345980 003428C0  28 05 00 00 */	cmplwi r5, 0
/* 80345984 003428C4  41 82 00 2C */	beq lbl_803459B0
/* 80345988 003428C8  80 9F 00 08 */	lwz r4, 8(r31)
/* 8034598C 003428CC  80 1F 00 04 */	lwz r0, 4(r31)
/* 80345990 003428D0  7C 04 00 00 */	cmpw r4, r0
/* 80345994 003428D4  40 80 00 1C */	bge lbl_803459B0
/* 80345998 003428D8  80 7F 00 0C */	lwz r3, 0xc(r31)
/* 8034599C 003428DC  54 80 10 3A */	slwi r0, r4, 2
/* 803459A0 003428E0  7C A3 01 2E */	stwx r5, r3, r0
/* 803459A4 003428E4  80 7F 00 08 */	lwz r3, 8(r31)
/* 803459A8 003428E8  38 03 00 01 */	addi r0, r3, 1
/* 803459AC 003428EC  90 1F 00 08 */	stw r0, 8(r31)
lbl_803459B0:
/* 803459B0 003428F0  38 60 00 01 */	li r3, 1
/* 803459B4 003428F4  48 00 00 10 */	b lbl_803459C4
lbl_803459B8:
/* 803459B8 003428F8  7F A3 EB 78 */	mr r3, r29
/* 803459BC 003428FC  4B F3 89 2D */	bl expandCollision__11TSmallEnemyFv
/* 803459C0 00342900  38 60 00 00 */	li r3, 0
lbl_803459C4:
/* 803459C4 00342904  80 01 00 4C */	lwz r0, 0x4c(r1)
/* 803459C8 00342908  83 E1 00 44 */	lwz r31, 0x44(r1)
/* 803459CC 0034290C  83 C1 00 40 */	lwz r30, 0x40(r1)
/* 803459D0 00342910  7C 08 03 A6 */	mtlr r0
/* 803459D4 00342914  83 A1 00 3C */	lwz r29, 0x3c(r1)
/* 803459D8 00342918  38 21 00 48 */	addi r1, r1, 0x48
/* 803459DC 0034291C  4E 80 00 20 */	blr 

.global __dt__28TNerveCoasterKillerExplosionFv
__dt__28TNerveCoasterKillerExplosionFv:
/* 803459E0 00342920  7C 08 02 A6 */	mflr r0
/* 803459E4 00342924  90 01 00 04 */	stw r0, 4(r1)
/* 803459E8 00342928  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 803459EC 0034292C  93 E1 00 14 */	stw r31, 0x14(r1)
/* 803459F0 00342930  7C 7F 1B 79 */	or. r31, r3, r3
/* 803459F4 00342934  41 82 00 30 */	beq lbl_80345A24
/* 803459F8 00342938  3C 60 80 3E */	lis r3, __vt__28TNerveCoasterKillerExplosion@ha
/* 803459FC 0034293C  38 03 34 A0 */	addi r0, r3, __vt__28TNerveCoasterKillerExplosion@l
/* 80345A00 00342940  90 1F 00 00 */	stw r0, 0(r31)
/* 80345A04 00342944  41 82 00 10 */	beq lbl_80345A14
/* 80345A08 00342948  3C 60 80 3C */	lis r3, "__vt__24TNerveBase<10TLiveActor>"@ha
/* 80345A0C 0034294C  38 03 92 60 */	addi r0, r3, "__vt__24TNerveBase<10TLiveActor>"@l
/* 80345A10 00342950  90 1F 00 00 */	stw r0, 0(r31)
lbl_80345A14:
/* 80345A14 00342954  7C 80 07 35 */	extsh. r0, r4
/* 80345A18 00342958  40 81 00 0C */	ble lbl_80345A24
/* 80345A1C 0034295C  7F E3 FB 78 */	mr r3, r31
/* 80345A20 00342960  4B CC 70 91 */	bl __dl__FPv
lbl_80345A24:
/* 80345A24 00342964  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 80345A28 00342968  7F E3 FB 78 */	mr r3, r31
/* 80345A2C 0034296C  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 80345A30 00342970  38 21 00 18 */	addi r1, r1, 0x18
/* 80345A34 00342974  7C 08 03 A6 */	mtlr r0
/* 80345A38 00342978  4E 80 00 20 */	blr 

.global setDeadAnm__14TCoasterKillerFv
setDeadAnm__14TCoasterKillerFv:
/* 80345A3C 0034297C  7C 08 02 A6 */	mflr r0
/* 80345A40 00342980  3C 80 80 3A */	lis r4, "@2932"@ha
/* 80345A44 00342984  90 01 00 04 */	stw r0, 4(r1)
/* 80345A48 00342988  38 84 4C 2C */	addi r4, r4, "@2932"@l
/* 80345A4C 0034298C  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 80345A50 00342990  93 E1 00 14 */	stw r31, 0x14(r1)
/* 80345A54 00342994  93 C1 00 10 */	stw r30, 0x10(r1)
/* 80345A58 00342998  7C 7E 1B 78 */	mr r30, r3
/* 80345A5C 0034299C  80 63 00 78 */	lwz r3, 0x78(r3)
/* 80345A60 003429A0  4B DD 0F 61 */	bl getMActor__13TMActorKeeperCFPCc
/* 80345A64 003429A4  90 7E 00 74 */	stw r3, 0x74(r30)
/* 80345A68 003429A8  38 7E 00 00 */	addi r3, r30, 0
/* 80345A6C 003429AC  38 80 00 00 */	li r4, 0
/* 80345A70 003429B0  81 9E 00 00 */	lwz r12, 0(r30)
/* 80345A74 003429B4  81 8C 01 90 */	lwz r12, 0x190(r12)
/* 80345A78 003429B8  7D 88 03 A6 */	mtlr r12
/* 80345A7C 003429BC  4E 80 00 21 */	blrl 
/* 80345A80 003429C0  3C 80 80 3A */	lis r4, "@3125"@ha
/* 80345A84 003429C4  80 6D 9C 28 */	lwz r3, gpConductor@sda21(r13)
/* 80345A88 003429C8  38 A4 4C B4 */	addi r5, r4, "@3125"@l
/* 80345A8C 003429CC  38 9E 00 10 */	addi r4, r30, 0x10
/* 80345A90 003429D0  38 C0 00 01 */	li r6, 1
/* 80345A94 003429D4  4B F0 2C 59 */	bl "makeOneEnemyAppear__10TConductorFRCQ29JGeometry8TVec3<f>PCci"
/* 80345A98 003429D8  7C 7F 1B 79 */	or. r31, r3, r3
/* 80345A9C 003429DC  41 82 00 3C */	beq lbl_80345AD8
/* 80345AA0 003429E0  38 7F 00 00 */	addi r3, r31, 0
/* 80345AA4 003429E4  38 9E 00 10 */	addi r4, r30, 0x10
/* 80345AA8 003429E8  38 BE 00 24 */	addi r5, r30, 0x24
/* 80345AAC 003429EC  4B F0 46 55 */	bl "generate__16TEffectExplosionFRQ29JGeometry8TVec3<f>RQ29JGeometry8TVec3<f>"
/* 80345AB0 003429F0  C0 1F 00 24 */	lfs f0, 0x24(r31)
/* 80345AB4 003429F4  C0 22 02 84 */	lfs f1, "@3126"@sda21(r2)
/* 80345AB8 003429F8  EC 00 00 72 */	fmuls f0, f0, f1
/* 80345ABC 003429FC  D0 1F 00 24 */	stfs f0, 0x24(r31)
/* 80345AC0 00342A00  C0 1F 00 28 */	lfs f0, 0x28(r31)
/* 80345AC4 00342A04  EC 00 00 72 */	fmuls f0, f0, f1
/* 80345AC8 00342A08  D0 1F 00 28 */	stfs f0, 0x28(r31)
/* 80345ACC 00342A0C  C0 1F 00 2C */	lfs f0, 0x2c(r31)
/* 80345AD0 00342A10  EC 00 00 72 */	fmuls f0, f0, f1
/* 80345AD4 00342A14  D0 1F 00 2C */	stfs f0, 0x2c(r31)
lbl_80345AD8:
/* 80345AD8 00342A18  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 80345ADC 00342A1C  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 80345AE0 00342A20  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 80345AE4 00342A24  7C 08 03 A6 */	mtlr r0
/* 80345AE8 00342A28  38 21 00 18 */	addi r1, r1, 0x18
/* 80345AEC 00342A2C  4E 80 00 20 */	blr 

.global setNormalFlyAnm__14TCoasterKillerFv
setNormalFlyAnm__14TCoasterKillerFv:
/* 80345AF0 00342A30  7C 08 02 A6 */	mflr r0
/* 80345AF4 00342A34  3C 80 80 3A */	lis r4, "@2931"@ha
/* 80345AF8 00342A38  90 01 00 04 */	stw r0, 4(r1)
/* 80345AFC 00342A3C  38 84 4C 18 */	addi r4, r4, "@2931"@l
/* 80345B00 00342A40  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 80345B04 00342A44  93 E1 00 14 */	stw r31, 0x14(r1)
/* 80345B08 00342A48  7C 7F 1B 78 */	mr r31, r3
/* 80345B0C 00342A4C  80 63 00 78 */	lwz r3, 0x78(r3)
/* 80345B10 00342A50  4B DD 0E B1 */	bl getMActor__13TMActorKeeperCFPCc
/* 80345B14 00342A54  90 7F 00 74 */	stw r3, 0x74(r31)
/* 80345B18 00342A58  38 7F 00 00 */	addi r3, r31, 0
/* 80345B1C 00342A5C  38 80 00 03 */	li r4, 3
/* 80345B20 00342A60  81 9F 00 00 */	lwz r12, 0(r31)
/* 80345B24 00342A64  81 8C 01 90 */	lwz r12, 0x190(r12)
/* 80345B28 00342A68  7D 88 03 A6 */	mtlr r12
/* 80345B2C 00342A6C  4E 80 00 21 */	blrl 
/* 80345B30 00342A70  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 80345B34 00342A74  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 80345B38 00342A78  38 21 00 18 */	addi r1, r1, 0x18
/* 80345B3C 00342A7C  7C 08 03 A6 */	mtlr r0
/* 80345B40 00342A80  4E 80 00 20 */	blr 

.global getBasNameTable__14TCoasterKillerCFv
getBasNameTable__14TCoasterKillerCFv:
/* 80345B44 00342A84  3C 60 80 3E */	lis r3, killer_bastable@ha
/* 80345B48 00342A88  38 63 34 10 */	addi r3, r3, killer_bastable@l
/* 80345B4C 00342A8C  4E 80 00 20 */	blr 

.global behaveToWater__14TCoasterKillerFP9THitActor
behaveToWater__14TCoasterKillerFP9THitActor:
/* 80345B50 00342A90  7C 08 02 A6 */	mflr r0
/* 80345B54 00342A94  90 01 00 04 */	stw r0, 4(r1)
/* 80345B58 00342A98  94 21 FF C8 */	stwu r1, -0x38(r1)
/* 80345B5C 00342A9C  93 E1 00 34 */	stw r31, 0x34(r1)
/* 80345B60 00342AA0  3B E3 00 00 */	addi r31, r3, 0
/* 80345B64 00342AA4  88 0D A8 08 */	lbz r0, init$2914@sda21(r13)
/* 80345B68 00342AA8  7C 00 07 75 */	extsb. r0, r0
/* 80345B6C 00342AAC  40 82 00 3C */	bne lbl_80345BA8
/* 80345B70 00342AB0  3C 60 80 3C */	lis r3, "__vt__24TNerveBase<10TLiveActor>"@ha
/* 80345B74 00342AB4  38 03 92 60 */	addi r0, r3, "__vt__24TNerveBase<10TLiveActor>"@l
/* 80345B78 00342AB8  3C 60 80 3E */	lis r3, __vt__28TNerveCoasterKillerExplosion@ha
/* 80345B7C 00342ABC  90 0D A8 0C */	stw r0, instance$2912@sda21(r13)
/* 80345B80 00342AC0  38 03 34 A0 */	addi r0, r3, __vt__28TNerveCoasterKillerExplosion@l
/* 80345B84 00342AC4  3C 80 80 34 */	lis r4, __dt__28TNerveCoasterKillerExplosionFv@ha
/* 80345B88 00342AC8  90 0D A8 0C */	stw r0, instance$2912@sda21(r13)
/* 80345B8C 00342ACC  3C 60 80 40 */	lis r3, "@2913"@ha
/* 80345B90 00342AD0  38 A3 00 EC */	addi r5, r3, "@2913"@l
/* 80345B94 00342AD4  38 84 59 E0 */	addi r4, r4, __dt__28TNerveCoasterKillerExplosionFv@l
/* 80345B98 00342AD8  38 6D A8 0C */	addi r3, r13, instance$2912@sda21
/* 80345B9C 00342ADC  4B D3 CB 8D */	bl __register_global_object
/* 80345BA0 00342AE0  38 00 00 01 */	li r0, 1
/* 80345BA4 00342AE4  98 0D A8 08 */	stb r0, init$2914@sda21(r13)
lbl_80345BA8:
/* 80345BA8 00342AE8  80 9F 00 8C */	lwz r4, 0x8c(r31)
/* 80345BAC 00342AEC  38 6D A8 0C */	addi r3, r13, instance$2912@sda21
/* 80345BB0 00342AF0  80 04 00 14 */	lwz r0, 0x14(r4)
/* 80345BB4 00342AF4  7C 00 18 40 */	cmplw r0, r3
/* 80345BB8 00342AF8  41 82 00 9C */	beq lbl_80345C54
/* 80345BBC 00342AFC  88 0D A8 08 */	lbz r0, init$2914@sda21(r13)
/* 80345BC0 00342B00  7C 00 07 75 */	extsb. r0, r0
/* 80345BC4 00342B04  40 82 00 38 */	bne lbl_80345BFC
/* 80345BC8 00342B08  3C 80 80 3C */	lis r4, "__vt__24TNerveBase<10TLiveActor>"@ha
/* 80345BCC 00342B0C  38 04 92 60 */	addi r0, r4, "__vt__24TNerveBase<10TLiveActor>"@l
/* 80345BD0 00342B10  3C 80 80 3E */	lis r4, __vt__28TNerveCoasterKillerExplosion@ha
/* 80345BD4 00342B14  90 0D A8 0C */	stw r0, instance$2912@sda21(r13)
/* 80345BD8 00342B18  38 04 34 A0 */	addi r0, r4, __vt__28TNerveCoasterKillerExplosion@l
/* 80345BDC 00342B1C  3C 80 80 34 */	lis r4, __dt__28TNerveCoasterKillerExplosionFv@ha
/* 80345BE0 00342B20  90 0D A8 0C */	stw r0, instance$2912@sda21(r13)
/* 80345BE4 00342B24  3C A0 80 40 */	lis r5, "@2913"@ha
/* 80345BE8 00342B28  38 84 59 E0 */	addi r4, r4, __dt__28TNerveCoasterKillerExplosionFv@l
/* 80345BEC 00342B2C  38 A5 00 EC */	addi r5, r5, "@2913"@l
/* 80345BF0 00342B30  4B D3 CB 39 */	bl __register_global_object
/* 80345BF4 00342B34  38 00 00 01 */	li r0, 1
/* 80345BF8 00342B38  98 0D A8 08 */	stb r0, init$2914@sda21(r13)
lbl_80345BFC:
/* 80345BFC 00342B3C  38 CD A8 0C */	addi r6, r13, instance$2912@sda21
/* 80345C00 00342B40  80 BF 00 8C */	lwz r5, 0x8c(r31)
/* 80345C04 00342B44  28 06 00 00 */	cmplwi r6, 0
/* 80345C08 00342B48  41 82 00 4C */	beq lbl_80345C54
/* 80345C0C 00342B4C  80 05 00 14 */	lwz r0, 0x14(r5)
/* 80345C10 00342B50  28 00 00 00 */	cmplwi r0, 0
/* 80345C14 00342B54  41 82 00 34 */	beq lbl_80345C48
/* 80345C18 00342B58  90 05 00 1C */	stw r0, 0x1c(r5)
/* 80345C1C 00342B5C  80 65 00 08 */	lwz r3, 8(r5)
/* 80345C20 00342B60  80 05 00 04 */	lwz r0, 4(r5)
/* 80345C24 00342B64  7C 03 00 00 */	cmpw r3, r0
/* 80345C28 00342B68  40 80 00 20 */	bge lbl_80345C48
/* 80345C2C 00342B6C  80 85 00 14 */	lwz r4, 0x14(r5)
/* 80345C30 00342B70  54 60 10 3A */	slwi r0, r3, 2
/* 80345C34 00342B74  80 65 00 0C */	lwz r3, 0xc(r5)
/* 80345C38 00342B78  7C 83 01 2E */	stwx r4, r3, r0
/* 80345C3C 00342B7C  80 65 00 08 */	lwz r3, 8(r5)
/* 80345C40 00342B80  38 03 00 01 */	addi r0, r3, 1
/* 80345C44 00342B84  90 05 00 08 */	stw r0, 8(r5)
lbl_80345C48:
/* 80345C48 00342B88  38 00 00 00 */	li r0, 0
/* 80345C4C 00342B8C  90 05 00 20 */	stw r0, 0x20(r5)
/* 80345C50 00342B90  90 C5 00 14 */	stw r6, 0x14(r5)
lbl_80345C54:
/* 80345C54 00342B94  80 01 00 3C */	lwz r0, 0x3c(r1)
/* 80345C58 00342B98  83 E1 00 34 */	lwz r31, 0x34(r1)
/* 80345C5C 00342B9C  38 21 00 38 */	addi r1, r1, 0x38
/* 80345C60 00342BA0  7C 08 03 A6 */	mtlr r0
/* 80345C64 00342BA4  4E 80 00 20 */	blr 

.global isCollidMove__14TCoasterKillerFP9THitActor
isCollidMove__14TCoasterKillerFP9THitActor:
/* 80345C68 00342BA8  7C 08 02 A6 */	mflr r0
/* 80345C6C 00342BAC  3C A0 80 40 */	lis r5, "@2869"@ha
/* 80345C70 00342BB0  90 01 00 04 */	stw r0, 4(r1)
/* 80345C74 00342BB4  94 21 FF B8 */	stwu r1, -0x48(r1)
/* 80345C78 00342BB8  93 E1 00 44 */	stw r31, 0x44(r1)
/* 80345C7C 00342BBC  3B E5 00 E0 */	addi r31, r5, "@2869"@l
/* 80345C80 00342BC0  93 C1 00 40 */	stw r30, 0x40(r1)
/* 80345C84 00342BC4  3B C4 00 00 */	addi r30, r4, 0
/* 80345C88 00342BC8  93 A1 00 3C */	stw r29, 0x3c(r1)
/* 80345C8C 00342BCC  3B A3 00 00 */	addi r29, r3, 0
/* 80345C90 00342BD0  88 0D A8 08 */	lbz r0, init$2914@sda21(r13)
/* 80345C94 00342BD4  7C 00 07 75 */	extsb. r0, r0
/* 80345C98 00342BD8  40 82 00 38 */	bne lbl_80345CD0
/* 80345C9C 00342BDC  3C 60 80 3C */	lis r3, "__vt__24TNerveBase<10TLiveActor>"@ha
/* 80345CA0 00342BE0  38 03 92 60 */	addi r0, r3, "__vt__24TNerveBase<10TLiveActor>"@l
/* 80345CA4 00342BE4  3C 60 80 3E */	lis r3, __vt__28TNerveCoasterKillerExplosion@ha
/* 80345CA8 00342BE8  90 0D A8 0C */	stw r0, instance$2912@sda21(r13)
/* 80345CAC 00342BEC  38 03 34 A0 */	addi r0, r3, __vt__28TNerveCoasterKillerExplosion@l
/* 80345CB0 00342BF0  3C 60 80 34 */	lis r3, __dt__28TNerveCoasterKillerExplosionFv@ha
/* 80345CB4 00342BF4  90 0D A8 0C */	stw r0, instance$2912@sda21(r13)
/* 80345CB8 00342BF8  38 83 59 E0 */	addi r4, r3, __dt__28TNerveCoasterKillerExplosionFv@l
/* 80345CBC 00342BFC  38 6D A8 0C */	addi r3, r13, instance$2912@sda21
/* 80345CC0 00342C00  38 BF 00 0C */	addi r5, r31, 0xc
/* 80345CC4 00342C04  4B D3 CA 65 */	bl __register_global_object
/* 80345CC8 00342C08  38 00 00 01 */	li r0, 1
/* 80345CCC 00342C0C  98 0D A8 08 */	stb r0, init$2914@sda21(r13)
lbl_80345CD0:
/* 80345CD0 00342C10  80 7D 00 8C */	lwz r3, 0x8c(r29)
/* 80345CD4 00342C14  38 0D A8 0C */	addi r0, r13, instance$2912@sda21
/* 80345CD8 00342C18  80 63 00 14 */	lwz r3, 0x14(r3)
/* 80345CDC 00342C1C  7C 03 00 40 */	cmplw r3, r0
/* 80345CE0 00342C20  40 82 00 0C */	bne lbl_80345CEC
/* 80345CE4 00342C24  38 60 00 00 */	li r3, 0
/* 80345CE8 00342C28  48 00 01 90 */	b lbl_80345E78
lbl_80345CEC:
/* 80345CEC 00342C2C  80 7E 00 4C */	lwz r3, 0x4c(r30)
/* 80345CF0 00342C30  3C 03 F8 00 */	addis r0, r3, 0xf800
/* 80345CF4 00342C34  28 00 00 1F */	cmplwi r0, 0x1f
/* 80345CF8 00342C38  40 82 00 0C */	bne lbl_80345D04
/* 80345CFC 00342C3C  38 00 00 01 */	li r0, 1
/* 80345D00 00342C40  48 00 00 08 */	b lbl_80345D08
lbl_80345D04:
/* 80345D04 00342C44  38 00 00 00 */	li r0, 0
lbl_80345D08:
/* 80345D08 00342C48  54 00 06 3F */	clrlwi. r0, r0, 0x18
/* 80345D0C 00342C4C  41 82 00 9C */	beq lbl_80345DA8
/* 80345D10 00342C50  88 0D A8 08 */	lbz r0, init$2914@sda21(r13)
/* 80345D14 00342C54  7C 00 07 75 */	extsb. r0, r0
/* 80345D18 00342C58  40 82 00 38 */	bne lbl_80345D50
/* 80345D1C 00342C5C  3C 60 80 3C */	lis r3, "__vt__24TNerveBase<10TLiveActor>"@ha
/* 80345D20 00342C60  38 03 92 60 */	addi r0, r3, "__vt__24TNerveBase<10TLiveActor>"@l
/* 80345D24 00342C64  3C 60 80 3E */	lis r3, __vt__28TNerveCoasterKillerExplosion@ha
/* 80345D28 00342C68  90 0D A8 0C */	stw r0, instance$2912@sda21(r13)
/* 80345D2C 00342C6C  38 03 34 A0 */	addi r0, r3, __vt__28TNerveCoasterKillerExplosion@l
/* 80345D30 00342C70  3C 60 80 34 */	lis r3, __dt__28TNerveCoasterKillerExplosionFv@ha
/* 80345D34 00342C74  90 0D A8 0C */	stw r0, instance$2912@sda21(r13)
/* 80345D38 00342C78  38 83 59 E0 */	addi r4, r3, __dt__28TNerveCoasterKillerExplosionFv@l
/* 80345D3C 00342C7C  38 6D A8 0C */	addi r3, r13, instance$2912@sda21
/* 80345D40 00342C80  38 BF 00 0C */	addi r5, r31, 0xc
/* 80345D44 00342C84  4B D3 C9 E5 */	bl __register_global_object
/* 80345D48 00342C88  38 00 00 01 */	li r0, 1
/* 80345D4C 00342C8C  98 0D A8 08 */	stb r0, init$2914@sda21(r13)
lbl_80345D50:
/* 80345D50 00342C90  38 CD A8 0C */	addi r6, r13, instance$2912@sda21
/* 80345D54 00342C94  80 BD 00 8C */	lwz r5, 0x8c(r29)
/* 80345D58 00342C98  28 06 00 00 */	cmplwi r6, 0
/* 80345D5C 00342C9C  41 82 00 4C */	beq lbl_80345DA8
/* 80345D60 00342CA0  80 05 00 14 */	lwz r0, 0x14(r5)
/* 80345D64 00342CA4  28 00 00 00 */	cmplwi r0, 0
/* 80345D68 00342CA8  41 82 00 34 */	beq lbl_80345D9C
/* 80345D6C 00342CAC  90 05 00 1C */	stw r0, 0x1c(r5)
/* 80345D70 00342CB0  80 65 00 08 */	lwz r3, 8(r5)
/* 80345D74 00342CB4  80 05 00 04 */	lwz r0, 4(r5)
/* 80345D78 00342CB8  7C 03 00 00 */	cmpw r3, r0
/* 80345D7C 00342CBC  40 80 00 20 */	bge lbl_80345D9C
/* 80345D80 00342CC0  80 85 00 14 */	lwz r4, 0x14(r5)
/* 80345D84 00342CC4  54 60 10 3A */	slwi r0, r3, 2
/* 80345D88 00342CC8  80 65 00 0C */	lwz r3, 0xc(r5)
/* 80345D8C 00342CCC  7C 83 01 2E */	stwx r4, r3, r0
/* 80345D90 00342CD0  80 65 00 08 */	lwz r3, 8(r5)
/* 80345D94 00342CD4  38 03 00 01 */	addi r0, r3, 1
/* 80345D98 00342CD8  90 05 00 08 */	stw r0, 8(r5)
lbl_80345D9C:
/* 80345D9C 00342CDC  38 00 00 00 */	li r0, 0
/* 80345DA0 00342CE0  90 05 00 20 */	stw r0, 0x20(r5)
/* 80345DA4 00342CE4  90 C5 00 14 */	stw r6, 0x14(r5)
lbl_80345DA8:
/* 80345DA8 00342CE8  80 7E 00 4C */	lwz r3, 0x4c(r30)
/* 80345DAC 00342CEC  3C 03 F0 00 */	addis r0, r3, 0xf000
/* 80345DB0 00342CF0  28 00 00 2B */	cmplwi r0, 0x2b
/* 80345DB4 00342CF4  40 82 00 0C */	bne lbl_80345DC0
/* 80345DB8 00342CF8  38 00 00 01 */	li r0, 1
/* 80345DBC 00342CFC  48 00 00 08 */	b lbl_80345DC4
lbl_80345DC0:
/* 80345DC0 00342D00  38 00 00 00 */	li r0, 0
lbl_80345DC4:
/* 80345DC4 00342D04  54 00 06 3F */	clrlwi. r0, r0, 0x18
/* 80345DC8 00342D08  41 82 00 AC */	beq lbl_80345E74
/* 80345DCC 00342D0C  7F C3 F3 78 */	mr r3, r30
/* 80345DD0 00342D10  4B FD 69 39 */	bl isAttack__7TRocketFv
/* 80345DD4 00342D14  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 80345DD8 00342D18  41 82 00 9C */	beq lbl_80345E74
/* 80345DDC 00342D1C  88 0D A8 08 */	lbz r0, init$2914@sda21(r13)
/* 80345DE0 00342D20  7C 00 07 75 */	extsb. r0, r0
/* 80345DE4 00342D24  40 82 00 38 */	bne lbl_80345E1C
/* 80345DE8 00342D28  3C 60 80 3C */	lis r3, "__vt__24TNerveBase<10TLiveActor>"@ha
/* 80345DEC 00342D2C  38 03 92 60 */	addi r0, r3, "__vt__24TNerveBase<10TLiveActor>"@l
/* 80345DF0 00342D30  3C 60 80 3E */	lis r3, __vt__28TNerveCoasterKillerExplosion@ha
/* 80345DF4 00342D34  90 0D A8 0C */	stw r0, instance$2912@sda21(r13)
/* 80345DF8 00342D38  38 03 34 A0 */	addi r0, r3, __vt__28TNerveCoasterKillerExplosion@l
/* 80345DFC 00342D3C  3C 60 80 34 */	lis r3, __dt__28TNerveCoasterKillerExplosionFv@ha
/* 80345E00 00342D40  90 0D A8 0C */	stw r0, instance$2912@sda21(r13)
/* 80345E04 00342D44  38 83 59 E0 */	addi r4, r3, __dt__28TNerveCoasterKillerExplosionFv@l
/* 80345E08 00342D48  38 6D A8 0C */	addi r3, r13, instance$2912@sda21
/* 80345E0C 00342D4C  38 BF 00 0C */	addi r5, r31, 0xc
/* 80345E10 00342D50  4B D3 C9 19 */	bl __register_global_object
/* 80345E14 00342D54  38 00 00 01 */	li r0, 1
/* 80345E18 00342D58  98 0D A8 08 */	stb r0, init$2914@sda21(r13)
lbl_80345E1C:
/* 80345E1C 00342D5C  38 CD A8 0C */	addi r6, r13, instance$2912@sda21
/* 80345E20 00342D60  80 BD 00 8C */	lwz r5, 0x8c(r29)
/* 80345E24 00342D64  28 06 00 00 */	cmplwi r6, 0
/* 80345E28 00342D68  41 82 00 4C */	beq lbl_80345E74
/* 80345E2C 00342D6C  80 05 00 14 */	lwz r0, 0x14(r5)
/* 80345E30 00342D70  28 00 00 00 */	cmplwi r0, 0
/* 80345E34 00342D74  41 82 00 34 */	beq lbl_80345E68
/* 80345E38 00342D78  90 05 00 1C */	stw r0, 0x1c(r5)
/* 80345E3C 00342D7C  80 65 00 08 */	lwz r3, 8(r5)
/* 80345E40 00342D80  80 05 00 04 */	lwz r0, 4(r5)
/* 80345E44 00342D84  7C 03 00 00 */	cmpw r3, r0
/* 80345E48 00342D88  40 80 00 20 */	bge lbl_80345E68
/* 80345E4C 00342D8C  80 85 00 14 */	lwz r4, 0x14(r5)
/* 80345E50 00342D90  54 60 10 3A */	slwi r0, r3, 2
/* 80345E54 00342D94  80 65 00 0C */	lwz r3, 0xc(r5)
/* 80345E58 00342D98  7C 83 01 2E */	stwx r4, r3, r0
/* 80345E5C 00342D9C  80 65 00 08 */	lwz r3, 8(r5)
/* 80345E60 00342DA0  38 03 00 01 */	addi r0, r3, 1
/* 80345E64 00342DA4  90 05 00 08 */	stw r0, 8(r5)
lbl_80345E68:
/* 80345E68 00342DA8  38 00 00 00 */	li r0, 0
/* 80345E6C 00342DAC  90 05 00 20 */	stw r0, 0x20(r5)
/* 80345E70 00342DB0  90 C5 00 14 */	stw r6, 0x14(r5)
lbl_80345E74:
/* 80345E74 00342DB4  38 60 00 01 */	li r3, 1
lbl_80345E78:
/* 80345E78 00342DB8  80 01 00 4C */	lwz r0, 0x4c(r1)
/* 80345E7C 00342DBC  83 E1 00 44 */	lwz r31, 0x44(r1)
/* 80345E80 00342DC0  83 C1 00 40 */	lwz r30, 0x40(r1)
/* 80345E84 00342DC4  7C 08 03 A6 */	mtlr r0
/* 80345E88 00342DC8  83 A1 00 3C */	lwz r29, 0x3c(r1)
/* 80345E8C 00342DCC  38 21 00 48 */	addi r1, r1, 0x48
/* 80345E90 00342DD0  4E 80 00 20 */	blr 

.global attackToMario__14TCoasterKillerFv
attackToMario__14TCoasterKillerFv:
/* 80345E94 00342DD4  7C 08 02 A6 */	mflr r0
/* 80345E98 00342DD8  90 01 00 04 */	stw r0, 4(r1)
/* 80345E9C 00342DDC  94 21 FF C8 */	stwu r1, -0x38(r1)
/* 80345EA0 00342DE0  93 E1 00 34 */	stw r31, 0x34(r1)
/* 80345EA4 00342DE4  3B E3 00 00 */	addi r31, r3, 0
/* 80345EA8 00342DE8  88 0D A8 08 */	lbz r0, init$2914@sda21(r13)
/* 80345EAC 00342DEC  7C 00 07 75 */	extsb. r0, r0
/* 80345EB0 00342DF0  40 82 00 3C */	bne lbl_80345EEC
/* 80345EB4 00342DF4  3C 60 80 3C */	lis r3, "__vt__24TNerveBase<10TLiveActor>"@ha
/* 80345EB8 00342DF8  38 03 92 60 */	addi r0, r3, "__vt__24TNerveBase<10TLiveActor>"@l
/* 80345EBC 00342DFC  3C 60 80 3E */	lis r3, __vt__28TNerveCoasterKillerExplosion@ha
/* 80345EC0 00342E00  90 0D A8 0C */	stw r0, instance$2912@sda21(r13)
/* 80345EC4 00342E04  38 03 34 A0 */	addi r0, r3, __vt__28TNerveCoasterKillerExplosion@l
/* 80345EC8 00342E08  3C 80 80 34 */	lis r4, __dt__28TNerveCoasterKillerExplosionFv@ha
/* 80345ECC 00342E0C  90 0D A8 0C */	stw r0, instance$2912@sda21(r13)
/* 80345ED0 00342E10  3C 60 80 40 */	lis r3, "@2913"@ha
/* 80345ED4 00342E14  38 A3 00 EC */	addi r5, r3, "@2913"@l
/* 80345ED8 00342E18  38 84 59 E0 */	addi r4, r4, __dt__28TNerveCoasterKillerExplosionFv@l
/* 80345EDC 00342E1C  38 6D A8 0C */	addi r3, r13, instance$2912@sda21
/* 80345EE0 00342E20  4B D3 C8 49 */	bl __register_global_object
/* 80345EE4 00342E24  38 00 00 01 */	li r0, 1
/* 80345EE8 00342E28  98 0D A8 08 */	stb r0, init$2914@sda21(r13)
lbl_80345EEC:
/* 80345EEC 00342E2C  80 9F 00 8C */	lwz r4, 0x8c(r31)
/* 80345EF0 00342E30  38 6D A8 0C */	addi r3, r13, instance$2912@sda21
/* 80345EF4 00342E34  80 04 00 14 */	lwz r0, 0x14(r4)
/* 80345EF8 00342E38  7C 00 18 40 */	cmplw r0, r3
/* 80345EFC 00342E3C  41 82 00 B8 */	beq lbl_80345FB4
/* 80345F00 00342E40  88 0D A8 08 */	lbz r0, init$2914@sda21(r13)
/* 80345F04 00342E44  7C 00 07 75 */	extsb. r0, r0
/* 80345F08 00342E48  40 82 00 38 */	bne lbl_80345F40
/* 80345F0C 00342E4C  3C 80 80 3C */	lis r4, "__vt__24TNerveBase<10TLiveActor>"@ha
/* 80345F10 00342E50  38 04 92 60 */	addi r0, r4, "__vt__24TNerveBase<10TLiveActor>"@l
/* 80345F14 00342E54  3C 80 80 3E */	lis r4, __vt__28TNerveCoasterKillerExplosion@ha
/* 80345F18 00342E58  90 0D A8 0C */	stw r0, instance$2912@sda21(r13)
/* 80345F1C 00342E5C  38 04 34 A0 */	addi r0, r4, __vt__28TNerveCoasterKillerExplosion@l
/* 80345F20 00342E60  3C 80 80 34 */	lis r4, __dt__28TNerveCoasterKillerExplosionFv@ha
/* 80345F24 00342E64  90 0D A8 0C */	stw r0, instance$2912@sda21(r13)
/* 80345F28 00342E68  3C A0 80 40 */	lis r5, "@2913"@ha
/* 80345F2C 00342E6C  38 84 59 E0 */	addi r4, r4, __dt__28TNerveCoasterKillerExplosionFv@l
/* 80345F30 00342E70  38 A5 00 EC */	addi r5, r5, "@2913"@l
/* 80345F34 00342E74  4B D3 C7 F5 */	bl __register_global_object
/* 80345F38 00342E78  38 00 00 01 */	li r0, 1
/* 80345F3C 00342E7C  98 0D A8 08 */	stb r0, init$2914@sda21(r13)
lbl_80345F40:
/* 80345F40 00342E80  38 CD A8 0C */	addi r6, r13, instance$2912@sda21
/* 80345F44 00342E84  80 BF 00 8C */	lwz r5, 0x8c(r31)
/* 80345F48 00342E88  28 06 00 00 */	cmplwi r6, 0
/* 80345F4C 00342E8C  41 82 00 4C */	beq lbl_80345F98
/* 80345F50 00342E90  80 05 00 14 */	lwz r0, 0x14(r5)
/* 80345F54 00342E94  28 00 00 00 */	cmplwi r0, 0
/* 80345F58 00342E98  41 82 00 34 */	beq lbl_80345F8C
/* 80345F5C 00342E9C  90 05 00 1C */	stw r0, 0x1c(r5)
/* 80345F60 00342EA0  80 65 00 08 */	lwz r3, 8(r5)
/* 80345F64 00342EA4  80 05 00 04 */	lwz r0, 4(r5)
/* 80345F68 00342EA8  7C 03 00 00 */	cmpw r3, r0
/* 80345F6C 00342EAC  40 80 00 20 */	bge lbl_80345F8C
/* 80345F70 00342EB0  80 85 00 14 */	lwz r4, 0x14(r5)
/* 80345F74 00342EB4  54 60 10 3A */	slwi r0, r3, 2
/* 80345F78 00342EB8  80 65 00 0C */	lwz r3, 0xc(r5)
/* 80345F7C 00342EBC  7C 83 01 2E */	stwx r4, r3, r0
/* 80345F80 00342EC0  80 65 00 08 */	lwz r3, 8(r5)
/* 80345F84 00342EC4  38 03 00 01 */	addi r0, r3, 1
/* 80345F88 00342EC8  90 05 00 08 */	stw r0, 8(r5)
lbl_80345F8C:
/* 80345F8C 00342ECC  38 00 00 00 */	li r0, 0
/* 80345F90 00342ED0  90 05 00 20 */	stw r0, 0x20(r5)
/* 80345F94 00342ED4  90 C5 00 14 */	stw r6, 0x14(r5)
lbl_80345F98:
/* 80345F98 00342ED8  80 7F 00 70 */	lwz r3, 0x70(r31)
/* 80345F9C 00342EDC  88 03 00 60 */	lbz r0, 0x60(r3)
/* 80345FA0 00342EE0  28 00 00 00 */	cmplwi r0, 0
/* 80345FA4 00342EE4  40 82 00 10 */	bne lbl_80345FB4
/* 80345FA8 00342EE8  38 7F 00 00 */	addi r3, r31, 0
/* 80345FAC 00342EEC  38 80 00 0E */	li r4, 0xe
/* 80345FB0 00342EF0  4B E0 CC 05 */	bl SMS_SendMessageToMario__FP9THitActorUl
lbl_80345FB4:
/* 80345FB4 00342EF4  80 01 00 3C */	lwz r0, 0x3c(r1)
/* 80345FB8 00342EF8  83 E1 00 34 */	lwz r31, 0x34(r1)
/* 80345FBC 00342EFC  38 21 00 38 */	addi r1, r1, 0x38
/* 80345FC0 00342F00  7C 08 03 A6 */	mtlr r0
/* 80345FC4 00342F04  4E 80 00 20 */	blr 

.global setMActorAndKeeper__14TCoasterKillerFv
setMActorAndKeeper__14TCoasterKillerFv:
/* 80345FC8 00342F08  7C 08 02 A6 */	mflr r0
/* 80345FCC 00342F0C  3C 80 80 3A */	lis r4, "@1490"@ha
/* 80345FD0 00342F10  90 01 00 04 */	stw r0, 4(r1)
/* 80345FD4 00342F14  94 21 FF 60 */	stwu r1, -0xa0(r1)
/* 80345FD8 00342F18  BF 61 00 8C */	stmw r27, 0x8c(r1)
/* 80345FDC 00342F1C  3B C3 00 00 */	addi r30, r3, 0
/* 80345FE0 00342F20  3B E4 4A E8 */	addi r31, r4, "@1490"@l
/* 80345FE4 00342F24  38 60 00 18 */	li r3, 0x18
/* 80345FE8 00342F28  4B CC 68 C9 */	bl __nw__FUl
/* 80345FEC 00342F2C  7C 7B 1B 79 */	or. r27, r3, r3
/* 80345FF0 00342F30  41 82 00 14 */	beq lbl_80346004
/* 80345FF4 00342F34  80 9E 00 70 */	lwz r4, 0x70(r30)
/* 80345FF8 00342F38  38 7B 00 00 */	addi r3, r27, 0
/* 80345FFC 00342F3C  38 A0 00 02 */	li r5, 2
/* 80346000 00342F40  4B DD 03 39 */	bl __ct__13TMActorKeeperFP12TLiveManagerUs
lbl_80346004:
/* 80346004 00342F44  93 7E 00 78 */	stw r27, 0x78(r30)
/* 80346008 00342F48  38 9F 01 30 */	addi r4, r31, 0x130
/* 8034600C 00342F4C  38 A0 00 00 */	li r5, 0
/* 80346010 00342F50  80 7E 00 78 */	lwz r3, 0x78(r30)
/* 80346014 00342F54  4B DD 04 F5 */	bl createMActor__13TMActorKeeperFPCcUl
/* 80346018 00342F58  90 7E 00 74 */	stw r3, 0x74(r30)
/* 8034601C 00342F5C  38 9F 01 44 */	addi r4, r31, 0x144
/* 80346020 00342F60  38 A0 00 03 */	li r5, 3
/* 80346024 00342F64  80 7E 00 78 */	lwz r3, 0x78(r30)
/* 80346028 00342F68  4B DD 04 E1 */	bl createMActor__13TMActorKeeperFPCcUl
/* 8034602C 00342F6C  80 7E 00 78 */	lwz r3, 0x78(r30)
/* 80346030 00342F70  38 9F 01 30 */	addi r4, r31, 0x130
/* 80346034 00342F74  4B DD 09 8D */	bl getMActor__13TMActorKeeperCFPCc
/* 80346038 00342F78  80 63 00 04 */	lwz r3, 4(r3)
/* 8034603C 00342F7C  38 9F 01 E8 */	addi r4, r31, 0x1e8
/* 80346040 00342F80  80 63 00 04 */	lwz r3, 4(r3)
/* 80346044 00342F84  80 63 00 B4 */	lwz r3, 0xb4(r3)
/* 80346048 00342F88  4B CC CF A9 */	bl getIndex__10JUTNameTabCFPCc
/* 8034604C 00342F8C  80 1E 00 78 */	lwz r0, 0x78(r30)
/* 80346050 00342F90  3B 63 00 00 */	addi r27, r3, 0
/* 80346054 00342F94  38 9F 01 30 */	addi r4, r31, 0x130
/* 80346058 00342F98  7C 03 03 78 */	mr r3, r0
/* 8034605C 00342F9C  4B DD 09 65 */	bl getMActor__13TMActorKeeperCFPCc
/* 80346060 00342FA0  80 63 00 04 */	lwz r3, 4(r3)
/* 80346064 00342FA4  38 9F 01 F4 */	addi r4, r31, 0x1f4
/* 80346068 00342FA8  80 63 00 04 */	lwz r3, 4(r3)
/* 8034606C 00342FAC  80 63 00 B4 */	lwz r3, 0xb4(r3)
/* 80346070 00342FB0  4B CC CF 81 */	bl getIndex__10JUTNameTabCFPCc
/* 80346074 00342FB4  80 1E 00 78 */	lwz r0, 0x78(r30)
/* 80346078 00342FB8  3B 83 00 00 */	addi r28, r3, 0
/* 8034607C 00342FBC  38 9F 01 30 */	addi r4, r31, 0x130
/* 80346080 00342FC0  7C 03 03 78 */	mr r3, r0
/* 80346084 00342FC4  4B DD 09 3D */	bl getMActor__13TMActorKeeperCFPCc
/* 80346088 00342FC8  80 63 00 04 */	lwz r3, 4(r3)
/* 8034608C 00342FCC  38 82 02 88 */	addi r4, r2, "@3300"@sda21
/* 80346090 00342FD0  80 63 00 04 */	lwz r3, 4(r3)
/* 80346094 00342FD4  80 63 00 B4 */	lwz r3, 0xb4(r3)
/* 80346098 00342FD8  4B CC CF 59 */	bl getIndex__10JUTNameTabCFPCc
/* 8034609C 00342FDC  80 BE 00 74 */	lwz r5, 0x74(r30)
/* 803460A0 00342FE0  3B A3 00 00 */	addi r29, r3, 0
/* 803460A4 00342FE4  57 64 04 3E */	clrlwi r4, r27, 0x10
/* 803460A8 00342FE8  80 65 00 04 */	lwz r3, 4(r5)
/* 803460AC 00342FEC  38 DE 01 B8 */	addi r6, r30, 0x1b8
/* 803460B0 00342FF0  38 A0 00 01 */	li r5, 1
/* 803460B4 00342FF4  4B D8 AF A1 */	bl SMS_InitPacket_OneTevColor__FP8J3DModelUs11_GXTevRegIDPC11_GXColorS10
/* 803460B8 00342FF8  80 7E 00 74 */	lwz r3, 0x74(r30)
/* 803460BC 00342FFC  57 84 04 3E */	clrlwi r4, r28, 0x10
/* 803460C0 00343000  38 DE 01 C0 */	addi r6, r30, 0x1c0
/* 803460C4 00343004  80 63 00 04 */	lwz r3, 4(r3)
/* 803460C8 00343008  38 A0 00 01 */	li r5, 1
/* 803460CC 0034300C  4B D8 AF 89 */	bl SMS_InitPacket_OneTevColor__FP8J3DModelUs11_GXTevRegIDPC11_GXColorS10
/* 803460D0 00343010  80 7E 00 74 */	lwz r3, 0x74(r30)
/* 803460D4 00343014  57 A4 04 3E */	clrlwi r4, r29, 0x10
/* 803460D8 00343018  38 DE 01 B0 */	addi r6, r30, 0x1b0
/* 803460DC 0034301C  80 63 00 04 */	lwz r3, 4(r3)
/* 803460E0 00343020  38 A0 00 01 */	li r5, 1
/* 803460E4 00343024  4B D8 AF 71 */	bl SMS_InitPacket_OneTevColor__FP8J3DModelUs11_GXTevRegIDPC11_GXColorS10
/* 803460E8 00343028  80 7E 00 78 */	lwz r3, 0x78(r30)
/* 803460EC 0034302C  38 9F 01 44 */	addi r4, r31, 0x144
/* 803460F0 00343030  4B DD 08 D1 */	bl getMActor__13TMActorKeeperCFPCc
/* 803460F4 00343034  80 63 00 04 */	lwz r3, 4(r3)
/* 803460F8 00343038  38 DE 01 C8 */	addi r6, r30, 0x1c8
/* 803460FC 0034303C  38 80 00 00 */	li r4, 0
/* 80346100 00343040  38 A0 00 01 */	li r5, 1
/* 80346104 00343044  4B D8 AF 51 */	bl SMS_InitPacket_OneTevColor__FP8J3DModelUs11_GXTevRegIDPC11_GXColorS10
/* 80346108 00343048  BB 61 00 8C */	lmw r27, 0x8c(r1)
/* 8034610C 0034304C  80 01 00 A4 */	lwz r0, 0xa4(r1)
/* 80346110 00343050  38 21 00 A0 */	addi r1, r1, 0xa0
/* 80346114 00343054  7C 08 03 A6 */	mtlr r0
/* 80346118 00343058  4E 80 00 20 */	blr 

.global perform__14TCoasterKillerFUlPQ26JDrama9TGraphics
perform__14TCoasterKillerFUlPQ26JDrama9TGraphics:
/* 8034611C 0034305C  7C 08 02 A6 */	mflr r0
/* 80346120 00343060  90 01 00 04 */	stw r0, 4(r1)
/* 80346124 00343064  94 21 FF A8 */	stwu r1, -0x58(r1)
/* 80346128 00343068  DB E1 00 50 */	stfd f31, 0x50(r1)
/* 8034612C 0034306C  93 E1 00 4C */	stw r31, 0x4c(r1)
/* 80346130 00343070  3B E3 00 00 */	addi r31, r3, 0
/* 80346134 00343074  93 C1 00 48 */	stw r30, 0x48(r1)
/* 80346138 00343078  3B C4 00 00 */	addi r30, r4, 0
/* 8034613C 0034307C  4B F3 7E 15 */	bl perform__11TSmallEnemyFUlPQ26JDrama9TGraphics
/* 80346140 00343080  57 C0 07 BD */	rlwinm. r0, r30, 0, 0x1e, 0x1e
/* 80346144 00343084  41 82 01 24 */	beq lbl_80346268
/* 80346148 00343088  80 1F 00 F0 */	lwz r0, 0xf0(r31)
/* 8034614C 0034308C  54 00 07 FF */	clrlwi. r0, r0, 0x1f
/* 80346150 00343090  40 82 01 18 */	bne lbl_80346268
/* 80346154 00343094  38 7F 01 D0 */	addi r3, r31, 0x1d0
/* 80346158 00343098  38 9F 01 9C */	addi r4, r31, 0x19c
/* 8034615C 0034309C  4B F5 61 6D */	bl "setQuat__Q29JGeometry64TRotation3<Q29JGeometry38TMatrix34<Q29JGeometry13SMatrix34C<f>>>FRCQ29JGeometry9TQuat4<f>"
/* 80346160 003430A0  C0 1F 00 10 */	lfs f0, 0x10(r31)
/* 80346164 003430A4  38 BF 01 D0 */	addi r5, r31, 0x1d0
/* 80346168 003430A8  38 FF 00 00 */	addi r7, r31, 0
/* 8034616C 003430AC  D0 1F 01 DC */	stfs f0, 0x1dc(r31)
/* 80346170 003430B0  38 80 01 74 */	li r4, 0x174
/* 80346174 003430B4  38 C0 00 01 */	li r6, 1
/* 80346178 003430B8  C0 1F 00 14 */	lfs f0, 0x14(r31)
/* 8034617C 003430BC  D0 1F 01 EC */	stfs f0, 0x1ec(r31)
/* 80346180 003430C0  C0 1F 00 18 */	lfs f0, 0x18(r31)
/* 80346184 003430C4  D0 1F 01 FC */	stfs f0, 0x1fc(r31)
/* 80346188 003430C8  80 6D 97 98 */	lwz r3, gpMarioParticleManager@sda21(r13)
/* 8034618C 003430CC  4B D9 54 01 */	bl emitAndBindToMtxPtr__21TMarioParticleManagerFlPA4_fUcPCv
/* 80346190 003430D0  88 0D A8 08 */	lbz r0, init$2914@sda21(r13)
/* 80346194 003430D4  7C 00 07 75 */	extsb. r0, r0
/* 80346198 003430D8  40 82 00 3C */	bne lbl_803461D4
/* 8034619C 003430DC  3C 60 80 3C */	lis r3, "__vt__24TNerveBase<10TLiveActor>"@ha
/* 803461A0 003430E0  38 03 92 60 */	addi r0, r3, "__vt__24TNerveBase<10TLiveActor>"@l
/* 803461A4 003430E4  3C 60 80 3E */	lis r3, __vt__28TNerveCoasterKillerExplosion@ha
/* 803461A8 003430E8  90 0D A8 0C */	stw r0, instance$2912@sda21(r13)
/* 803461AC 003430EC  38 03 34 A0 */	addi r0, r3, __vt__28TNerveCoasterKillerExplosion@l
/* 803461B0 003430F0  3C 80 80 34 */	lis r4, __dt__28TNerveCoasterKillerExplosionFv@ha
/* 803461B4 003430F4  90 0D A8 0C */	stw r0, instance$2912@sda21(r13)
/* 803461B8 003430F8  3C 60 80 40 */	lis r3, "@2913"@ha
/* 803461BC 003430FC  38 A3 00 EC */	addi r5, r3, "@2913"@l
/* 803461C0 00343100  38 84 59 E0 */	addi r4, r4, __dt__28TNerveCoasterKillerExplosionFv@l
/* 803461C4 00343104  38 6D A8 0C */	addi r3, r13, instance$2912@sda21
/* 803461C8 00343108  4B D3 C5 61 */	bl __register_global_object
/* 803461CC 0034310C  38 00 00 01 */	li r0, 1
/* 803461D0 00343110  98 0D A8 08 */	stb r0, init$2914@sda21(r13)
lbl_803461D4:
/* 803461D4 00343114  80 7F 00 8C */	lwz r3, 0x8c(r31)
/* 803461D8 00343118  38 0D A8 0C */	addi r0, r13, instance$2912@sda21
/* 803461DC 0034311C  80 63 00 14 */	lwz r3, 0x14(r3)
/* 803461E0 00343120  7C 03 00 40 */	cmplw r3, r0
/* 803461E4 00343124  41 82 00 84 */	beq lbl_80346268
/* 803461E8 00343128  80 6D 98 DC */	lwz r3, gpMarioPos@sda21(r13)
/* 803461EC 0034312C  C0 7F 00 10 */	lfs f3, 0x10(r31)
/* 803461F0 00343130  C0 43 00 00 */	lfs f2, 0(r3)
/* 803461F4 00343134  C0 3F 00 14 */	lfs f1, 0x14(r31)
/* 803461F8 00343138  EC 83 10 28 */	fsubs f4, f3, f2
/* 803461FC 0034313C  C0 03 00 04 */	lfs f0, 4(r3)
/* 80346200 00343140  C0 5F 00 18 */	lfs f2, 0x18(r31)
/* 80346204 00343144  EC 61 00 28 */	fsubs f3, f1, f0
/* 80346208 00343148  C0 03 00 08 */	lfs f0, 8(r3)
/* 8034620C 0034314C  EC 42 00 28 */	fsubs f2, f2, f0
/* 80346210 00343150  EC 24 01 32 */	fmuls f1, f4, f4
/* 80346214 00343154  EC 03 00 F2 */	fmuls f0, f3, f3
/* 80346218 00343158  EC 42 00 B2 */	fmuls f2, f2, f2
/* 8034621C 0034315C  EC 01 00 2A */	fadds f0, f1, f0
/* 80346220 00343160  EC 22 00 2A */	fadds f1, f2, f0
/* 80346224 00343164  4B D8 A6 8D */	bl "sqrt__Q29JGeometry8TUtil<f>Ff"
/* 80346228 00343168  FF E0 08 90 */	fmr f31, f1
/* 8034622C 0034316C  80 6D 97 EC */	lwz r3, gpMSound@sda21(r13)
/* 80346230 00343170  38 80 20 FF */	li r4, 0x20ff
/* 80346234 00343174  4B E3 68 1D */	bl gateCheck__6MSoundFUl
/* 80346238 00343178  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 8034623C 0034317C  41 82 00 2C */	beq lbl_80346268
/* 80346240 00343180  FC 20 F8 90 */	fmr f1, f31
/* 80346244 00343184  38 9F 00 10 */	addi r4, r31, 0x10
/* 80346248 00343188  38 60 20 FF */	li r3, 0x20ff
/* 8034624C 0034318C  38 A0 00 00 */	li r5, 0
/* 80346250 00343190  38 C0 00 00 */	li r6, 0
/* 80346254 00343194  38 E0 00 00 */	li r7, 0
/* 80346258 00343198  39 00 00 00 */	li r8, 0
/* 8034625C 0034319C  39 20 00 00 */	li r9, 0
/* 80346260 003431A0  39 40 00 04 */	li r10, 4
/* 80346264 003431A4  4B E3 B8 B5 */	bl startSoundActorWithInfo__Q214MSoundSESystem8MSoundSEFUlPC3VecP3VecfUlUlPP8JAISoundUlUc
lbl_80346268:
/* 80346268 003431A8  80 01 00 5C */	lwz r0, 0x5c(r1)
/* 8034626C 003431AC  CB E1 00 50 */	lfd f31, 0x50(r1)
/* 80346270 003431B0  83 E1 00 4C */	lwz r31, 0x4c(r1)
/* 80346274 003431B4  7C 08 03 A6 */	mtlr r0
/* 80346278 003431B8  83 C1 00 48 */	lwz r30, 0x48(r1)
/* 8034627C 003431BC  38 21 00 58 */	addi r1, r1, 0x58
/* 80346280 003431C0  4E 80 00 20 */	blr 

.global reset__14TCoasterKillerFv
reset__14TCoasterKillerFv:
/* 80346284 003431C4  7C 08 02 A6 */	mflr r0
/* 80346288 003431C8  90 01 00 04 */	stw r0, 4(r1)
/* 8034628C 003431CC  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 80346290 003431D0  93 E1 00 14 */	stw r31, 0x14(r1)
/* 80346294 003431D4  7C 7F 1B 78 */	mr r31, r3
/* 80346298 003431D8  4B F3 E9 41 */	bl reset__12TWalkerEnemyFv
/* 8034629C 003431DC  80 7F 00 F0 */	lwz r3, 0xf0(r31)
/* 803462A0 003431E0  38 00 00 00 */	li r0, 0
/* 803462A4 003431E4  54 63 00 3C */	rlwinm r3, r3, 0, 0, 0x1e
/* 803462A8 003431E8  90 7F 00 F0 */	stw r3, 0xf0(r31)
/* 803462AC 003431EC  80 7F 00 F0 */	lwz r3, 0xf0(r31)
/* 803462B0 003431F0  60 63 00 80 */	ori r3, r3, 0x80
/* 803462B4 003431F4  90 7F 00 F0 */	stw r3, 0xf0(r31)
/* 803462B8 003431F8  90 1F 01 98 */	stw r0, 0x198(r31)
/* 803462BC 003431FC  90 1F 01 94 */	stw r0, 0x194(r31)
/* 803462C0 00343200  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 803462C4 00343204  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 803462C8 00343208  38 21 00 18 */	addi r1, r1, 0x18
/* 803462CC 0034320C  7C 08 03 A6 */	mtlr r0
/* 803462D0 00343210  4E 80 00 20 */	blr 

.global init__14TCoasterKillerFP12TLiveManager
init__14TCoasterKillerFP12TLiveManager:
/* 803462D4 00343214  7C 08 02 A6 */	mflr r0
/* 803462D8 00343218  90 01 00 04 */	stw r0, 4(r1)
/* 803462DC 0034321C  94 21 FF D0 */	stwu r1, -0x30(r1)
/* 803462E0 00343220  93 E1 00 2C */	stw r31, 0x2c(r1)
/* 803462E4 00343224  7C 7F 1B 78 */	mr r31, r3
/* 803462E8 00343228  4B F3 ED D9 */	bl init__12TWalkerEnemyFP12TLiveManager
/* 803462EC 0034322C  80 6D 9C 28 */	lwz r3, gpConductor@sda21(r13)
/* 803462F0 00343230  38 82 02 9C */	addi r4, r2, "@3398"@sda21
/* 803462F4 00343234  4B F0 2A 55 */	bl getGraphByName__10TConductorFPCc
/* 803462F8 00343238  80 9F 01 24 */	lwz r4, 0x124(r31)
/* 803462FC 0034323C  38 00 FF FF */	li r0, -1
/* 80346300 00343240  90 64 00 00 */	stw r3, 0(r4)
/* 80346304 00343244  7F E3 FB 78 */	mr r3, r31
/* 80346308 00343248  80 9F 01 24 */	lwz r4, 0x124(r31)
/* 8034630C 0034324C  90 04 00 08 */	stw r0, 8(r4)
/* 80346310 00343250  4B F0 91 F1 */	bl goToShortestNextGraphNode__11TSpineEnemyFv
/* 80346314 00343254  88 0D A8 00 */	lbz r0, init$2870@sda21(r13)
/* 80346318 00343258  7C 00 07 75 */	extsb. r0, r0
/* 8034631C 0034325C  40 82 00 38 */	bne lbl_80346354
/* 80346320 00343260  38 6D A8 04 */	addi r3, r13, instance$2868@sda21
/* 80346324 00343264  4B F1 47 69 */	bl "__ct__24TNerveBase<10TLiveActor>Fv"
/* 80346328 00343268  3C 60 80 3E */	lis r3, __vt__24TNerveCoasterEnemyWander@ha
/* 8034632C 0034326C  38 03 36 74 */	addi r0, r3, __vt__24TNerveCoasterEnemyWander@l
/* 80346330 00343270  3C 80 80 34 */	lis r4, __dt__24TNerveCoasterEnemyWanderFv@ha
/* 80346334 00343274  90 0D A8 04 */	stw r0, instance$2868@sda21(r13)
/* 80346338 00343278  3C 60 80 40 */	lis r3, "@2869"@ha
/* 8034633C 0034327C  38 A3 00 E0 */	addi r5, r3, "@2869"@l
/* 80346340 00343280  38 84 64 14 */	addi r4, r4, __dt__24TNerveCoasterEnemyWanderFv@l
/* 80346344 00343284  38 6D A8 04 */	addi r3, r13, instance$2868@sda21
/* 80346348 00343288  4B D3 C3 E1 */	bl __register_global_object
/* 8034634C 0034328C  38 00 00 01 */	li r0, 1
/* 80346350 00343290  98 0D A8 00 */	stb r0, init$2870@sda21(r13)
lbl_80346354:
/* 80346354 00343294  80 DF 00 8C */	lwz r6, 0x8c(r31)
/* 80346358 00343298  38 A0 00 00 */	li r5, 0
/* 8034635C 0034329C  3C 60 08 00 */	lis r3, 0x0800001F@ha
/* 80346360 003432A0  90 A6 00 08 */	stw r5, 8(r6)
/* 80346364 003432A4  38 8D A8 04 */	addi r4, r13, instance$2868@sda21
/* 80346368 003432A8  38 63 00 1F */	addi r3, r3, 0x0800001F@l
/* 8034636C 003432AC  90 A6 00 20 */	stw r5, 0x20(r6)
/* 80346370 003432B0  38 00 00 11 */	li r0, 0x11
/* 80346374 003432B4  90 86 00 14 */	stw r4, 0x14(r6)
/* 80346378 003432B8  90 86 00 18 */	stw r4, 0x18(r6)
/* 8034637C 003432BC  90 A6 00 1C */	stw r5, 0x1c(r6)
/* 80346380 003432C0  80 9F 00 F0 */	lwz r4, 0xf0(r31)
/* 80346384 003432C4  60 84 00 01 */	ori r4, r4, 1
/* 80346388 003432C8  90 9F 00 F0 */	stw r4, 0xf0(r31)
/* 8034638C 003432CC  80 9F 00 F0 */	lwz r4, 0xf0(r31)
/* 80346390 003432D0  60 84 00 10 */	ori r4, r4, 0x10
/* 80346394 003432D4  90 9F 00 F0 */	stw r4, 0xf0(r31)
/* 80346398 003432D8  C0 02 02 78 */	lfs f0, "@3103"@sda21(r2)
/* 8034639C 003432DC  D0 1F 01 9C */	stfs f0, 0x19c(r31)
/* 803463A0 003432E0  D0 1F 01 A0 */	stfs f0, 0x1a0(r31)
/* 803463A4 003432E4  D0 1F 01 A4 */	stfs f0, 0x1a4(r31)
/* 803463A8 003432E8  C0 02 02 7C */	lfs f0, "@3104"@sda21(r2)
/* 803463AC 003432EC  D0 1F 01 A8 */	stfs f0, 0x1a8(r31)
/* 803463B0 003432F0  90 7F 00 4C */	stw r3, 0x4c(r31)
/* 803463B4 003432F4  90 1F 01 50 */	stw r0, 0x150(r31)
/* 803463B8 003432F8  80 1F 00 F0 */	lwz r0, 0xf0(r31)
/* 803463BC 003432FC  60 00 04 00 */	ori r0, r0, 0x400
/* 803463C0 00343300  90 1F 00 F0 */	stw r0, 0xf0(r31)
/* 803463C4 00343304  80 1F 00 F0 */	lwz r0, 0xf0(r31)
/* 803463C8 00343308  54 00 05 66 */	rlwinm r0, r0, 0, 0x15, 0x13
/* 803463CC 0034330C  90 1F 00 F0 */	stw r0, 0xf0(r31)
/* 803463D0 00343310  B0 BF 01 B0 */	sth r5, 0x1b0(r31)
/* 803463D4 00343314  B0 BF 01 B2 */	sth r5, 0x1b2(r31)
/* 803463D8 00343318  B0 BF 01 B4 */	sth r5, 0x1b4(r31)
/* 803463DC 0034331C  B0 BF 01 B8 */	sth r5, 0x1b8(r31)
/* 803463E0 00343320  B0 BF 01 BA */	sth r5, 0x1ba(r31)
/* 803463E4 00343324  B0 BF 01 BC */	sth r5, 0x1bc(r31)
/* 803463E8 00343328  B0 BF 01 C0 */	sth r5, 0x1c0(r31)
/* 803463EC 0034332C  B0 BF 01 C2 */	sth r5, 0x1c2(r31)
/* 803463F0 00343330  B0 BF 01 C4 */	sth r5, 0x1c4(r31)
/* 803463F4 00343334  B0 BF 01 C8 */	sth r5, 0x1c8(r31)
/* 803463F8 00343338  B0 BF 01 CA */	sth r5, 0x1ca(r31)
/* 803463FC 0034333C  B0 BF 01 CC */	sth r5, 0x1cc(r31)
/* 80346400 00343340  80 01 00 34 */	lwz r0, 0x34(r1)
/* 80346404 00343344  83 E1 00 2C */	lwz r31, 0x2c(r1)
/* 80346408 00343348  38 21 00 30 */	addi r1, r1, 0x30
/* 8034640C 0034334C  7C 08 03 A6 */	mtlr r0
/* 80346410 00343350  4E 80 00 20 */	blr 

.global __dt__24TNerveCoasterEnemyWanderFv
__dt__24TNerveCoasterEnemyWanderFv:
/* 80346414 00343354  7C 08 02 A6 */	mflr r0
/* 80346418 00343358  90 01 00 04 */	stw r0, 4(r1)
/* 8034641C 0034335C  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 80346420 00343360  93 E1 00 14 */	stw r31, 0x14(r1)
/* 80346424 00343364  7C 7F 1B 79 */	or. r31, r3, r3
/* 80346428 00343368  41 82 00 30 */	beq lbl_80346458
/* 8034642C 0034336C  3C 60 80 3E */	lis r3, __vt__24TNerveCoasterEnemyWander@ha
/* 80346430 00343370  38 03 36 74 */	addi r0, r3, __vt__24TNerveCoasterEnemyWander@l
/* 80346434 00343374  90 1F 00 00 */	stw r0, 0(r31)
/* 80346438 00343378  41 82 00 10 */	beq lbl_80346448
/* 8034643C 0034337C  3C 60 80 3C */	lis r3, "__vt__24TNerveBase<10TLiveActor>"@ha
/* 80346440 00343380  38 03 92 60 */	addi r0, r3, "__vt__24TNerveBase<10TLiveActor>"@l
/* 80346444 00343384  90 1F 00 00 */	stw r0, 0(r31)
lbl_80346448:
/* 80346448 00343388  7C 80 07 35 */	extsh. r0, r4
/* 8034644C 0034338C  40 81 00 0C */	ble lbl_80346458
/* 80346450 00343390  7F E3 FB 78 */	mr r3, r31
/* 80346454 00343394  4B CC 66 5D */	bl __dl__FPv
lbl_80346458:
/* 80346458 00343398  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 8034645C 0034339C  7F E3 FB 78 */	mr r3, r31
/* 80346460 003433A0  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 80346464 003433A4  38 21 00 18 */	addi r1, r1, 0x18
/* 80346468 003433A8  7C 08 03 A6 */	mtlr r0
/* 8034646C 003433AC  4E 80 00 20 */	blr 

.global __ct__14TCoasterKillerFPCc
__ct__14TCoasterKillerFPCc:
/* 80346470 003433B0  7C 08 02 A6 */	mflr r0
/* 80346474 003433B4  90 01 00 04 */	stw r0, 4(r1)
/* 80346478 003433B8  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 8034647C 003433BC  93 E1 00 14 */	stw r31, 0x14(r1)
/* 80346480 003433C0  7C 7F 1B 78 */	mr r31, r3
/* 80346484 003433C4  4B F3 ED 69 */	bl __ct__12TWalkerEnemyFPCc
/* 80346488 003433C8  3C 60 80 3E */	lis r3, __vt__13TCoasterEnemy@ha
/* 8034648C 003433CC  38 63 36 84 */	addi r3, r3, __vt__13TCoasterEnemy@l
/* 80346490 003433D0  90 7F 00 00 */	stw r3, 0(r31)
/* 80346494 003433D4  38 03 00 24 */	addi r0, r3, 0x24
/* 80346498 003433D8  3C 60 80 3E */	lis r3, __vt__14TCoasterKiller@ha
/* 8034649C 003433DC  90 1F 00 20 */	stw r0, 0x20(r31)
/* 803464A0 003433E0  38 00 00 00 */	li r0, 0
/* 803464A4 003433E4  38 83 34 B0 */	addi r4, r3, __vt__14TCoasterKiller@l
/* 803464A8 003433E8  90 1F 01 98 */	stw r0, 0x198(r31)
/* 803464AC 003433EC  38 04 00 24 */	addi r0, r4, 0x24
/* 803464B0 003433F0  38 7F 00 00 */	addi r3, r31, 0
/* 803464B4 003433F4  90 9F 00 00 */	stw r4, 0(r31)
/* 803464B8 003433F8  90 1F 00 20 */	stw r0, 0x20(r31)
/* 803464BC 003433FC  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 803464C0 00343400  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 803464C4 00343404  38 21 00 18 */	addi r1, r1, 0x18
/* 803464C8 00343408  7C 08 03 A6 */	mtlr r0
/* 803464CC 0034340C  4E 80 00 20 */	blr 

.global "execute__24TNerveCoasterEnemyWanderCFP24TSpineBase<10TLiveActor>"
"execute__24TNerveCoasterEnemyWanderCFP24TSpineBase<10TLiveActor>":
/* 803464D0 00343410  7C 08 02 A6 */	mflr r0
/* 803464D4 00343414  90 01 00 04 */	stw r0, 4(r1)
/* 803464D8 00343418  94 21 FF A8 */	stwu r1, -0x58(r1)
/* 803464DC 0034341C  93 E1 00 54 */	stw r31, 0x54(r1)
/* 803464E0 00343420  80 04 00 20 */	lwz r0, 0x20(r4)
/* 803464E4 00343424  83 E4 00 00 */	lwz r31, 0(r4)
/* 803464E8 00343428  2C 00 00 00 */	cmpwi r0, 0
/* 803464EC 0034342C  40 82 00 4C */	bne lbl_80346538
/* 803464F0 00343430  7F E3 FB 78 */	mr r3, r31
/* 803464F4 00343434  81 9F 00 00 */	lwz r12, 0(r31)
/* 803464F8 00343438  81 8C 01 50 */	lwz r12, 0x150(r12)
/* 803464FC 0034343C  7D 88 03 A6 */	mtlr r12
/* 80346500 00343440  4E 80 00 21 */	blrl 
/* 80346504 00343444  7F E3 FB 78 */	mr r3, r31
/* 80346508 00343448  81 9F 00 00 */	lwz r12, 0(r31)
/* 8034650C 0034344C  81 8C 01 B4 */	lwz r12, 0x1b4(r12)
/* 80346510 00343450  7D 88 03 A6 */	mtlr r12
/* 80346514 00343454  4E 80 00 21 */	blrl 
/* 80346518 00343458  80 9F 01 94 */	lwz r4, 0x194(r31)
/* 8034651C 0034345C  80 7F 01 24 */	lwz r3, 0x124(r31)
/* 80346520 00343460  4B F1 81 D5 */	bl setTo__12TGraphTracerFi
/* 80346524 00343464  7F E3 FB 78 */	mr r3, r31
/* 80346528 00343468  81 9F 00 00 */	lwz r12, 0(r31)
/* 8034652C 0034346C  81 8C 01 B8 */	lwz r12, 0x1b8(r12)
/* 80346530 00343470  7D 88 03 A6 */	mtlr r12
/* 80346534 00343474  4E 80 00 21 */	blrl 
lbl_80346538:
/* 80346538 00343478  7F E3 FB 78 */	mr r3, r31
/* 8034653C 0034347C  81 9F 00 00 */	lwz r12, 0(r31)
/* 80346540 00343480  81 8C 01 10 */	lwz r12, 0x110(r12)
/* 80346544 00343484  7D 88 03 A6 */	mtlr r12
/* 80346548 00343488  4E 80 00 21 */	blrl 
/* 8034654C 0034348C  2C 03 00 00 */	cmpwi r3, 0
/* 80346550 00343490  41 82 00 8C */	beq lbl_803465DC
/* 80346554 00343494  80 1F 01 98 */	lwz r0, 0x198(r31)
/* 80346558 00343498  2C 00 00 00 */	cmpwi r0, 0
/* 8034655C 0034349C  40 82 00 34 */	bne lbl_80346590
/* 80346560 003434A0  80 7F 01 94 */	lwz r3, 0x194(r31)
/* 80346564 003434A4  38 03 00 01 */	addi r0, r3, 1
/* 80346568 003434A8  90 1F 01 94 */	stw r0, 0x194(r31)
/* 8034656C 003434AC  80 7F 01 24 */	lwz r3, 0x124(r31)
/* 80346570 003434B0  80 9F 01 94 */	lwz r4, 0x194(r31)
/* 80346574 003434B4  80 63 00 00 */	lwz r3, 0(r3)
/* 80346578 003434B8  80 03 00 08 */	lwz r0, 8(r3)
/* 8034657C 003434BC  7C 04 00 00 */	cmpw r4, r0
/* 80346580 003434C0  41 80 00 3C */	blt lbl_803465BC
/* 80346584 003434C4  38 00 00 00 */	li r0, 0
/* 80346588 003434C8  90 1F 01 94 */	stw r0, 0x194(r31)
/* 8034658C 003434CC  48 00 00 30 */	b lbl_803465BC
lbl_80346590:
/* 80346590 003434D0  80 7F 01 94 */	lwz r3, 0x194(r31)
/* 80346594 003434D4  38 03 FF FF */	addi r0, r3, -1
/* 80346598 003434D8  90 1F 01 94 */	stw r0, 0x194(r31)
/* 8034659C 003434DC  80 1F 01 94 */	lwz r0, 0x194(r31)
/* 803465A0 003434E0  2C 00 00 00 */	cmpwi r0, 0
/* 803465A4 003434E4  40 80 00 18 */	bge lbl_803465BC
/* 803465A8 003434E8  80 7F 01 24 */	lwz r3, 0x124(r31)
/* 803465AC 003434EC  80 63 00 00 */	lwz r3, 0(r3)
/* 803465B0 003434F0  80 63 00 08 */	lwz r3, 8(r3)
/* 803465B4 003434F4  38 03 FF FF */	addi r0, r3, -1
/* 803465B8 003434F8  90 1F 01 94 */	stw r0, 0x194(r31)
lbl_803465BC:
/* 803465BC 003434FC  80 9F 01 94 */	lwz r4, 0x194(r31)
/* 803465C0 00343500  80 7F 01 24 */	lwz r3, 0x124(r31)
/* 803465C4 00343504  4B F1 81 31 */	bl setTo__12TGraphTracerFi
/* 803465C8 00343508  7F E3 FB 78 */	mr r3, r31
/* 803465CC 0034350C  81 9F 00 00 */	lwz r12, 0(r31)
/* 803465D0 00343510  81 8C 01 B8 */	lwz r12, 0x1b8(r12)
/* 803465D4 00343514  7D 88 03 A6 */	mtlr r12
/* 803465D8 00343518  4E 80 00 21 */	blrl 
lbl_803465DC:
/* 803465DC 0034351C  7F E3 FB 78 */	mr r3, r31
/* 803465E0 00343520  81 9F 00 00 */	lwz r12, 0(r31)
/* 803465E4 00343524  81 8C 01 BC */	lwz r12, 0x1bc(r12)
/* 803465E8 00343528  7D 88 03 A6 */	mtlr r12
/* 803465EC 0034352C  4E 80 00 21 */	blrl 
/* 803465F0 00343530  80 01 00 5C */	lwz r0, 0x5c(r1)
/* 803465F4 00343534  38 60 00 00 */	li r3, 0
/* 803465F8 00343538  83 E1 00 54 */	lwz r31, 0x54(r1)
/* 803465FC 0034353C  38 21 00 58 */	addi r1, r1, 0x58
/* 80346600 00343540  7C 08 03 A6 */	mtlr r0
/* 80346604 00343544  4E 80 00 20 */	blr 

.global setWalkAnm__13TCoasterEnemyFv
setWalkAnm__13TCoasterEnemyFv:
/* 80346608 00343548  7C 08 02 A6 */	mflr r0
/* 8034660C 0034354C  90 01 00 04 */	stw r0, 4(r1)
/* 80346610 00343550  94 21 FF F8 */	stwu r1, -8(r1)
/* 80346614 00343554  81 83 00 00 */	lwz r12, 0(r3)
/* 80346618 00343558  81 8C 01 C0 */	lwz r12, 0x1c0(r12)
/* 8034661C 0034355C  7D 88 03 A6 */	mtlr r12
/* 80346620 00343560  4E 80 00 21 */	blrl 
/* 80346624 00343564  80 01 00 0C */	lwz r0, 0xc(r1)
/* 80346628 00343568  38 21 00 08 */	addi r1, r1, 8
/* 8034662C 0034356C  7C 08 03 A6 */	mtlr r0
/* 80346630 00343570  4E 80 00 20 */	blr 

.global setNormalFlyAnm__13TCoasterEnemyFv
setNormalFlyAnm__13TCoasterEnemyFv:
/* 80346634 00343574  4E 80 00 20 */	blr 

.global calcRootMatrix__13TCoasterEnemyFv
calcRootMatrix__13TCoasterEnemyFv:
/* 80346638 00343578  7C 08 02 A6 */	mflr r0
/* 8034663C 0034357C  90 01 00 04 */	stw r0, 4(r1)
/* 80346640 00343580  94 21 FF B8 */	stwu r1, -0x48(r1)
/* 80346644 00343584  93 E1 00 44 */	stw r31, 0x44(r1)
/* 80346648 00343588  93 C1 00 40 */	stw r30, 0x40(r1)
/* 8034664C 0034358C  3B C3 00 00 */	addi r30, r3, 0
/* 80346650 00343590  38 61 00 0C */	addi r3, r1, 0xc
/* 80346654 00343594  38 9E 01 9C */	addi r4, r30, 0x19c
/* 80346658 00343598  4B F5 5C 71 */	bl "setQuat__Q29JGeometry64TRotation3<Q29JGeometry38TMatrix34<Q29JGeometry13SMatrix34C<f>>>FRCQ29JGeometry9TQuat4<f>"
/* 8034665C 0034359C  C0 1E 00 10 */	lfs f0, 0x10(r30)
/* 80346660 003435A0  7F C3 F3 78 */	mr r3, r30
/* 80346664 003435A4  D0 01 00 18 */	stfs f0, 0x18(r1)
/* 80346668 003435A8  C0 1E 00 14 */	lfs f0, 0x14(r30)
/* 8034666C 003435AC  D0 01 00 28 */	stfs f0, 0x28(r1)
/* 80346670 003435B0  C0 1E 00 18 */	lfs f0, 0x18(r30)
/* 80346674 003435B4  D0 01 00 38 */	stfs f0, 0x38(r1)
/* 80346678 003435B8  4B DC BB F9 */	bl getModel__10TLiveActorCFv
/* 8034667C 003435BC  80 9E 00 24 */	lwz r4, 0x24(r30)
/* 80346680 003435C0  3B E1 00 0C */	addi r31, r1, 0xc
/* 80346684 003435C4  80 1E 00 28 */	lwz r0, 0x28(r30)
/* 80346688 003435C8  90 83 00 14 */	stw r4, 0x14(r3)
/* 8034668C 003435CC  90 03 00 18 */	stw r0, 0x18(r3)
/* 80346690 003435D0  80 1E 00 2C */	lwz r0, 0x2c(r30)
/* 80346694 003435D4  90 03 00 1C */	stw r0, 0x1c(r3)
/* 80346698 003435D8  7F C3 F3 78 */	mr r3, r30
/* 8034669C 003435DC  4B DC BB D5 */	bl getModel__10TLiveActorCFv
/* 803466A0 003435E0  38 83 00 00 */	addi r4, r3, 0
/* 803466A4 003435E4  38 7F 00 00 */	addi r3, r31, 0
/* 803466A8 003435E8  38 84 00 20 */	addi r4, r4, 0x20
/* 803466AC 003435EC  4B D4 DC 31 */	bl PSMTXCopy
/* 803466B0 003435F0  80 01 00 4C */	lwz r0, 0x4c(r1)
/* 803466B4 003435F4  83 E1 00 44 */	lwz r31, 0x44(r1)
/* 803466B8 003435F8  83 C1 00 40 */	lwz r30, 0x40(r1)
/* 803466BC 003435FC  7C 08 03 A6 */	mtlr r0
/* 803466C0 00343600  38 21 00 48 */	addi r1, r1, 0x48
/* 803466C4 00343604  4E 80 00 20 */	blr 

.global moveCoaster__13TCoasterEnemyFv
moveCoaster__13TCoasterEnemyFv:
/* 803466C8 00343608  7C 08 02 A6 */	mflr r0
/* 803466CC 0034360C  90 01 00 04 */	stw r0, 4(r1)
/* 803466D0 00343610  94 21 FD F8 */	stwu r1, -0x208(r1)
/* 803466D4 00343614  DB E1 02 00 */	stfd f31, 0x200(r1)
/* 803466D8 00343618  DB C1 01 F8 */	stfd f30, 0x1f8(r1)
/* 803466DC 0034361C  DB A1 01 F0 */	stfd f29, 0x1f0(r1)
/* 803466E0 00343620  DB 81 01 E8 */	stfd f28, 0x1e8(r1)
/* 803466E4 00343624  DB 61 01 E0 */	stfd f27, 0x1e0(r1)
/* 803466E8 00343628  DB 41 01 D8 */	stfd f26, 0x1d8(r1)
/* 803466EC 0034362C  DB 21 01 D0 */	stfd f25, 0x1d0(r1)
/* 803466F0 00343630  DB 01 01 C8 */	stfd f24, 0x1c8(r1)
/* 803466F4 00343634  DA E1 01 C0 */	stfd f23, 0x1c0(r1)
/* 803466F8 00343638  DA C1 01 B8 */	stfd f22, 0x1b8(r1)
/* 803466FC 0034363C  DA A1 01 B0 */	stfd f21, 0x1b0(r1)
/* 80346700 00343640  93 E1 01 AC */	stw r31, 0x1ac(r1)
/* 80346704 00343644  7C 7F 1B 78 */	mr r31, r3
/* 80346708 00343648  80 83 01 24 */	lwz r4, 0x124(r3)
/* 8034670C 0034364C  38 61 01 3C */	addi r3, r1, 0x13c
/* 80346710 00343650  80 04 00 00 */	lwz r0, 0(r4)
/* 80346714 00343654  80 A4 00 04 */	lwz r5, 4(r4)
/* 80346718 00343658  7C 04 03 78 */	mr r4, r0
/* 8034671C 0034365C  4B F1 88 C1 */	bl indexToPoint__9TGraphWebCFi
/* 80346720 00343660  80 01 01 3C */	lwz r0, 0x13c(r1)
/* 80346724 00343664  80 61 01 40 */	lwz r3, 0x140(r1)
/* 80346728 00343668  90 01 01 98 */	stw r0, 0x198(r1)
/* 8034672C 0034366C  80 01 01 44 */	lwz r0, 0x144(r1)
/* 80346730 00343670  90 61 01 9C */	stw r3, 0x19c(r1)
/* 80346734 00343674  90 01 01 A0 */	stw r0, 0x1a0(r1)
/* 80346738 00343678  C0 21 01 98 */	lfs f1, 0x198(r1)
/* 8034673C 0034367C  C0 1F 00 10 */	lfs f0, 0x10(r31)
/* 80346740 00343680  EC 01 00 28 */	fsubs f0, f1, f0
/* 80346744 00343684  D0 01 01 98 */	stfs f0, 0x198(r1)
/* 80346748 00343688  C0 21 01 9C */	lfs f1, 0x19c(r1)
/* 8034674C 0034368C  C0 1F 00 14 */	lfs f0, 0x14(r31)
/* 80346750 00343690  EC 01 00 28 */	fsubs f0, f1, f0
/* 80346754 00343694  D0 01 01 9C */	stfs f0, 0x19c(r1)
/* 80346758 00343698  C0 21 01 A0 */	lfs f1, 0x1a0(r1)
/* 8034675C 0034369C  C0 1F 00 18 */	lfs f0, 0x18(r31)
/* 80346760 003436A0  EC 01 00 28 */	fsubs f0, f1, f0
/* 80346764 003436A4  D0 01 01 A0 */	stfs f0, 0x1a0(r1)
/* 80346768 003436A8  C0 21 01 98 */	lfs f1, 0x198(r1)
/* 8034676C 003436AC  C0 61 01 A0 */	lfs f3, 0x1a0(r1)
/* 80346770 003436B0  C0 01 01 9C */	lfs f0, 0x19c(r1)
/* 80346774 003436B4  EC 41 00 72 */	fmuls f2, f1, f1
/* 80346778 003436B8  EC 63 00 F2 */	fmuls f3, f3, f3
/* 8034677C 003436BC  EC 20 00 32 */	fmuls f1, f0, f0
/* 80346780 003436C0  C0 02 02 A4 */	lfs f0, "@3755"@sda21(r2)
/* 80346784 003436C4  EC 22 08 2A */	fadds f1, f2, f1
/* 80346788 003436C8  EC 23 08 2A */	fadds f1, f3, f1
/* 8034678C 003436CC  FC 01 00 40 */	fcmpo cr0, f1, f0
/* 80346790 003436D0  4C 40 13 82 */	cror 2, 0, 2
/* 80346794 003436D4  40 82 00 18 */	bne lbl_803467AC
/* 80346798 003436D8  C0 02 02 78 */	lfs f0, "@3103"@sda21(r2)
/* 8034679C 003436DC  D0 01 01 A0 */	stfs f0, 0x1a0(r1)
/* 803467A0 003436E0  D0 01 01 9C */	stfs f0, 0x19c(r1)
/* 803467A4 003436E4  D0 01 01 98 */	stfs f0, 0x198(r1)
/* 803467A8 003436E8  48 00 00 34 */	b lbl_803467DC
lbl_803467AC:
/* 803467AC 003436EC  4B D2 65 C5 */	bl "inv_sqrt__Q29JGeometry8TUtil<f>Ff"
/* 803467B0 003436F0  C0 42 02 7C */	lfs f2, "@3104"@sda21(r2)
/* 803467B4 003436F4  C0 01 01 98 */	lfs f0, 0x198(r1)
/* 803467B8 003436F8  EC 22 00 72 */	fmuls f1, f2, f1
/* 803467BC 003436FC  EC 00 00 72 */	fmuls f0, f0, f1
/* 803467C0 00343700  D0 01 01 98 */	stfs f0, 0x198(r1)
/* 803467C4 00343704  C0 01 01 9C */	lfs f0, 0x19c(r1)
/* 803467C8 00343708  EC 00 00 72 */	fmuls f0, f0, f1
/* 803467CC 0034370C  D0 01 01 9C */	stfs f0, 0x19c(r1)
/* 803467D0 00343710  C0 01 01 A0 */	lfs f0, 0x1a0(r1)
/* 803467D4 00343714  EC 00 00 72 */	fmuls f0, f0, f1
/* 803467D8 00343718  D0 01 01 A0 */	stfs f0, 0x1a0(r1)
lbl_803467DC:
/* 803467DC 0034371C  80 1F 01 98 */	lwz r0, 0x198(r31)
/* 803467E0 00343720  2C 00 00 00 */	cmpwi r0, 0
/* 803467E4 00343724  40 82 00 20 */	bne lbl_80346804
/* 803467E8 00343728  7F E3 FB 78 */	mr r3, r31
/* 803467EC 0034372C  81 9F 00 00 */	lwz r12, 0(r31)
/* 803467F0 00343730  81 8C 01 08 */	lwz r12, 0x108(r12)
/* 803467F4 00343734  7D 88 03 A6 */	mtlr r12
/* 803467F8 00343738  4E 80 00 21 */	blrl 
/* 803467FC 0034373C  C0 63 03 3C */	lfs f3, 0x33c(r3)
/* 80346800 00343740  48 00 00 1C */	b lbl_8034681C
lbl_80346804:
/* 80346804 00343744  7F E3 FB 78 */	mr r3, r31
/* 80346808 00343748  81 9F 00 00 */	lwz r12, 0(r31)
/* 8034680C 0034374C  81 8C 01 08 */	lwz r12, 0x108(r12)
/* 80346810 00343750  7D 88 03 A6 */	mtlr r12
/* 80346814 00343754  4E 80 00 21 */	blrl 
/* 80346818 00343758  C0 63 03 50 */	lfs f3, 0x350(r3)
lbl_8034681C:
/* 8034681C 0034375C  C0 01 01 98 */	lfs f0, 0x198(r1)
/* 80346820 00343760  D0 1F 00 AC */	stfs f0, 0xac(r31)
/* 80346824 00343764  C0 01 01 9C */	lfs f0, 0x19c(r1)
/* 80346828 00343768  D0 1F 00 B0 */	stfs f0, 0xb0(r31)
/* 8034682C 0034376C  C0 41 01 A0 */	lfs f2, 0x1a0(r1)
/* 80346830 00343770  D0 5F 00 B4 */	stfs f2, 0xb4(r31)
/* 80346834 00343774  C0 3F 00 AC */	lfs f1, 0xac(r31)
/* 80346838 00343778  EC 21 00 F2 */	fmuls f1, f1, f3
/* 8034683C 0034377C  D0 3F 00 AC */	stfs f1, 0xac(r31)
/* 80346840 00343780  C0 3F 00 B0 */	lfs f1, 0xb0(r31)
/* 80346844 00343784  EC 21 00 F2 */	fmuls f1, f1, f3
/* 80346848 00343788  D0 3F 00 B0 */	stfs f1, 0xb0(r31)
/* 8034684C 0034378C  C0 3F 00 B4 */	lfs f1, 0xb4(r31)
/* 80346850 00343790  EC 21 00 F2 */	fmuls f1, f1, f3
/* 80346854 00343794  D0 3F 00 B4 */	stfs f1, 0xb4(r31)
/* 80346858 00343798  C1 9F 01 A0 */	lfs f12, 0x1a0(r31)
/* 8034685C 0034379C  C1 5F 01 A8 */	lfs f10, 0x1a8(r31)
/* 80346860 003437A0  C1 3F 01 9C */	lfs f9, 0x19c(r31)
/* 80346864 003437A4  EC AC 03 32 */	fmuls f5, f12, f12
/* 80346868 003437A8  C1 7F 01 A4 */	lfs f11, 0x1a4(r31)
/* 8034686C 003437AC  EC 2A 03 32 */	fmuls f1, f10, f12
/* 80346870 003437B0  C1 02 02 98 */	lfs f8, "@3359"@sda21(r2)
/* 80346874 003437B4  EC C9 02 F2 */	fmuls f6, f9, f11
/* 80346878 003437B8  EC 28 00 72 */	fmuls f1, f8, f1
/* 8034687C 003437BC  C0 62 02 7C */	lfs f3, "@3104"@sda21(r2)
/* 80346880 003437C0  EC 89 02 72 */	fmuls f4, f9, f9
/* 80346884 003437C4  C2 C1 01 98 */	lfs f22, 0x198(r1)
/* 80346888 003437C8  EF 88 09 BA */	fmadds f28, f8, f6, f1
/* 8034688C 003437CC  EC EC 02 F2 */	fmuls f7, f12, f11
/* 80346890 003437D0  C0 22 02 78 */	lfs f1, "@3103"@sda21(r2)
/* 80346894 003437D4  EC CA 02 72 */	fmuls f6, f10, f9
/* 80346898 003437D8  ED A8 19 3C */	fnmsubs f13, f8, f4, f3
/* 8034689C 003437DC  EE A8 01 F2 */	fmuls f21, f8, f7
/* 803468A0 003437E0  EC E8 01 B2 */	fmuls f7, f8, f6
/* 803468A4 003437E4  EF 48 69 7C */	fnmsubs f26, f8, f5, f13
/* 803468A8 003437E8  EC 7C 00 B2 */	fmuls f3, f28, f2
/* 803468AC 003437EC  EF 75 38 28 */	fsubs f27, f21, f7
/* 803468B0 003437F0  EC BA 00 32 */	fmuls f5, f26, f0
/* 803468B4 003437F4  EE FA 1D B8 */	fmsubs f23, f26, f22, f3
/* 803468B8 003437F8  EC 9B 05 B2 */	fmuls f4, f27, f22
/* 803468BC 003437FC  EF 1B 28 B8 */	fmsubs f24, f27, f2, f5
/* 803468C0 00343800  EC 77 05 F2 */	fmuls f3, f23, f23
/* 803468C4 00343804  EC AA 02 F2 */	fmuls f5, f10, f11
/* 803468C8 00343808  EE DC 20 38 */	fmsubs f22, f28, f0, f4
/* 803468CC 0034380C  EC 78 1E 3A */	fmadds f3, f24, f24, f3
/* 803468D0 00343810  EC 8B 02 F2 */	fmuls f4, f11, f11
/* 803468D4 00343814  EC C9 03 32 */	fmuls f6, f9, f12
/* 803468D8 00343818  EF 36 1D BA */	fmadds f25, f22, f22, f3
/* 803468DC 0034381C  EC 68 01 72 */	fmuls f3, f8, f5
/* 803468E0 00343820  EF C8 69 3C */	fnmsubs f30, f8, f4, f13
/* 803468E4 00343824  FC 19 08 40 */	fcmpo cr0, f25, f1
/* 803468E8 00343828  EF E8 19 B8 */	fmsubs f31, f8, f6, f3
/* 803468EC 0034382C  EF B5 38 2A */	fadds f29, f21, f7
/* 803468F0 00343830  4C 40 13 82 */	cror 2, 0, 2
/* 803468F4 00343834  40 82 00 08 */	bne lbl_803468FC
/* 803468F8 00343838  48 00 00 28 */	b lbl_80346920
lbl_803468FC:
/* 803468FC 0034383C  FC A0 C8 34 */	frsqrte f5, f25
/* 80346900 00343840  C0 82 02 90 */	lfs f4, "@3340"@sda21(r2)
/* 80346904 00343844  C0 22 02 94 */	lfs f1, "@3341"@sda21(r2)
/* 80346908 00343848  FC A0 28 18 */	frsp f5, f5
/* 8034690C 0034384C  EC 65 01 72 */	fmuls f3, f5, f5
/* 80346910 00343850  EC 84 01 72 */	fmuls f4, f4, f5
/* 80346914 00343854  EC 39 08 FC */	fnmsubs f1, f25, f3, f1
/* 80346918 00343858  EC 24 00 72 */	fmuls f1, f4, f1
/* 8034691C 0034385C  EF 39 00 72 */	fmuls f25, f25, f1
lbl_80346920:
/* 80346920 00343860  C0 22 02 A4 */	lfs f1, "@3755"@sda21(r2)
/* 80346924 00343864  FC 19 08 40 */	fcmpo cr0, f25, f1
/* 80346928 00343868  4C 40 13 82 */	cror 2, 0, 2
/* 8034692C 0034386C  40 82 00 20 */	bne lbl_8034694C
/* 80346930 00343870  C0 22 02 78 */	lfs f1, "@3103"@sda21(r2)
/* 80346934 00343874  C0 02 02 7C */	lfs f0, "@3104"@sda21(r2)
/* 80346938 00343878  D0 21 01 70 */	stfs f1, 0x170(r1)
/* 8034693C 0034387C  D0 21 01 74 */	stfs f1, 0x174(r1)
/* 80346940 00343880  D0 21 01 78 */	stfs f1, 0x178(r1)
/* 80346944 00343884  D0 01 01 7C */	stfs f0, 0x17c(r1)
/* 80346948 00343888  48 00 00 5C */	b lbl_803469A4
lbl_8034694C:
/* 8034694C 0034388C  EC 1B 00 32 */	fmuls f0, f27, f0
/* 80346950 00343890  C0 21 01 98 */	lfs f1, 0x198(r1)
/* 80346954 00343894  EC 1C 00 7A */	fmadds f0, f28, f1, f0
/* 80346958 00343898  FC 20 C8 90 */	fmr f1, f25
/* 8034695C 0034389C  EC 5A 00 BA */	fmadds f2, f26, f2, f0
/* 80346960 003438A0  4B D4 04 B5 */	bl atan2f
/* 80346964 003438A4  C0 02 02 90 */	lfs f0, "@3340"@sda21(r2)
/* 80346968 003438A8  C0 42 02 A8 */	lfs f2, "@3756"@sda21(r2)
/* 8034696C 003438AC  EC 00 00 72 */	fmuls f0, f0, f1
/* 80346970 003438B0  EE A2 00 32 */	fmuls f21, f2, f0
/* 80346974 003438B4  FC 20 A8 90 */	fmr f1, f21
/* 80346978 003438B8  4B D4 07 8D */	bl sinf
/* 8034697C 003438BC  EC 21 C8 24 */	fdivs f1, f1, f25
/* 80346980 003438C0  EC 18 00 72 */	fmuls f0, f24, f1
/* 80346984 003438C4  EC 57 00 72 */	fmuls f2, f23, f1
/* 80346988 003438C8  D0 01 01 70 */	stfs f0, 0x170(r1)
/* 8034698C 003438CC  EC 16 00 72 */	fmuls f0, f22, f1
/* 80346990 003438D0  FC 20 A8 90 */	fmr f1, f21
/* 80346994 003438D4  D0 41 01 74 */	stfs f2, 0x174(r1)
/* 80346998 003438D8  D0 01 01 78 */	stfs f0, 0x178(r1)
/* 8034699C 003438DC  4B D4 05 D5 */	bl cosf
/* 803469A0 003438E0  D0 21 01 7C */	stfs f1, 0x17c(r1)
lbl_803469A4:
/* 803469A4 003438E4  C0 A1 01 7C */	lfs f5, 0x17c(r1)
/* 803469A8 003438E8  C1 5F 01 9C */	lfs f10, 0x19c(r31)
/* 803469AC 003438EC  C1 1F 01 A0 */	lfs f8, 0x1a0(r31)
/* 803469B0 003438F0  C1 21 01 70 */	lfs f9, 0x170(r1)
/* 803469B4 003438F4  EC 05 02 B2 */	fmuls f0, f5, f10
/* 803469B8 003438F8  C0 DF 01 A8 */	lfs f6, 0x1a8(r31)
/* 803469BC 003438FC  EC 25 02 32 */	fmuls f1, f5, f8
/* 803469C0 00343900  C1 61 01 74 */	lfs f11, 0x174(r1)
/* 803469C4 00343904  C0 9F 01 A4 */	lfs f4, 0x1a4(r31)
/* 803469C8 00343908  EC 49 01 BA */	fmadds f2, f9, f6, f0
/* 803469CC 0034390C  C0 E1 01 78 */	lfs f7, 0x178(r1)
/* 803469D0 00343910  EC 05 01 32 */	fmuls f0, f5, f4
/* 803469D4 00343914  EC 2B 09 BA */	fmadds f1, f11, f6, f1
/* 803469D8 00343918  EC 69 02 B2 */	fmuls f3, f9, f10
/* 803469DC 0034391C  EC 4B 11 3A */	fmadds f2, f11, f4, f2
/* 803469E0 00343920  EC 07 01 BA */	fmadds f0, f7, f6, f0
/* 803469E4 00343924  EC 27 0A BA */	fmadds f1, f7, f10, f1
/* 803469E8 00343928  EC 65 19 B8 */	fmsubs f3, f5, f6, f3
/* 803469EC 0034392C  EC 47 12 3C */	fnmsubs f2, f7, f8, f2
/* 803469F0 00343930  EC 09 02 3A */	fmadds f0, f9, f8, f0
/* 803469F4 00343934  EC 29 09 3C */	fnmsubs f1, f9, f4, f1
/* 803469F8 00343938  D0 5F 01 9C */	stfs f2, 0x19c(r31)
/* 803469FC 0034393C  EC 6B 1A 3C */	fnmsubs f3, f11, f8, f3
/* 80346A00 00343940  EC 0B 02 BC */	fnmsubs f0, f11, f10, f0
/* 80346A04 00343944  D0 3F 01 A0 */	stfs f1, 0x1a0(r31)
/* 80346A08 00343948  EC 47 19 3C */	fnmsubs f2, f7, f4, f3
/* 80346A0C 0034394C  D0 1F 01 A4 */	stfs f0, 0x1a4(r31)
/* 80346A10 00343950  D0 5F 01 A8 */	stfs f2, 0x1a8(r31)
/* 80346A14 00343954  C0 42 02 78 */	lfs f2, "@3103"@sda21(r2)
/* 80346A18 00343958  C0 22 02 7C */	lfs f1, "@3104"@sda21(r2)
/* 80346A1C 0034395C  EC 7B 00 B2 */	fmuls f3, f27, f2
/* 80346A20 00343960  EC 1C 00 B2 */	fmuls f0, f28, f2
/* 80346A24 00343964  EE DA 18 7C */	fnmsubs f22, f26, f1, f3
/* 80346A28 00343968  EE FA 00 B8 */	fmsubs f23, f26, f2, f0
/* 80346A2C 0034396C  EF 1C 18 78 */	fmsubs f24, f28, f1, f3
/* 80346A30 00343970  EC 17 05 F2 */	fmuls f0, f23, f23
/* 80346A34 00343974  EC 16 05 BA */	fmadds f0, f22, f22, f0
/* 80346A38 00343978  EC 38 06 3A */	fmadds f1, f24, f24, f0
/* 80346A3C 0034397C  FC 01 10 40 */	fcmpo cr0, f1, f2
/* 80346A40 00343980  4C 40 13 82 */	cror 2, 0, 2
/* 80346A44 00343984  40 82 00 0C */	bne lbl_80346A50
/* 80346A48 00343988  FC 00 08 90 */	fmr f0, f1
/* 80346A4C 0034398C  48 00 00 28 */	b lbl_80346A74
lbl_80346A50:
/* 80346A50 00343990  FC 80 08 34 */	frsqrte f4, f1
/* 80346A54 00343994  C0 62 02 90 */	lfs f3, "@3340"@sda21(r2)
/* 80346A58 00343998  C0 02 02 94 */	lfs f0, "@3341"@sda21(r2)
/* 80346A5C 0034399C  FC 80 20 18 */	frsp f4, f4
/* 80346A60 003439A0  EC 44 01 32 */	fmuls f2, f4, f4
/* 80346A64 003439A4  EC 63 01 32 */	fmuls f3, f3, f4
/* 80346A68 003439A8  EC 01 00 BC */	fnmsubs f0, f1, f2, f0
/* 80346A6C 003439AC  EC 03 00 32 */	fmuls f0, f3, f0
/* 80346A70 003439B0  EC 01 00 32 */	fmuls f0, f1, f0
lbl_80346A74:
/* 80346A74 003439B4  C3 22 02 78 */	lfs f25, "@3103"@sda21(r2)
/* 80346A78 003439B8  FC 00 C8 40 */	fcmpo cr0, f0, f25
/* 80346A7C 003439BC  40 81 02 2C */	ble lbl_80346CA8
/* 80346A80 003439C0  C0 02 02 A4 */	lfs f0, "@3755"@sda21(r2)
/* 80346A84 003439C4  FC 01 00 40 */	fcmpo cr0, f1, f0
/* 80346A88 003439C8  4C 40 13 82 */	cror 2, 0, 2
/* 80346A8C 003439CC  40 82 00 10 */	bne lbl_80346A9C
/* 80346A90 003439D0  FE C0 C8 90 */	fmr f22, f25
/* 80346A94 003439D4  FE A0 B0 90 */	fmr f21, f22
/* 80346A98 003439D8  48 00 00 1C */	b lbl_80346AB4
lbl_80346A9C:
/* 80346A9C 003439DC  4B D2 62 D5 */	bl "inv_sqrt__Q29JGeometry8TUtil<f>Ff"
/* 80346AA0 003439E0  C0 02 02 7C */	lfs f0, "@3104"@sda21(r2)
/* 80346AA4 003439E4  EC 00 00 72 */	fmuls f0, f0, f1
/* 80346AA8 003439E8  EE B6 00 32 */	fmuls f21, f22, f0
/* 80346AAC 003439EC  EE D7 00 32 */	fmuls f22, f23, f0
/* 80346AB0 003439F0  EF 38 00 32 */	fmuls f25, f24, f0
lbl_80346AB4:
/* 80346AB4 003439F4  C0 22 02 AC */	lfs f1, "@3757"@sda21(r2)
/* 80346AB8 003439F8  4B D4 06 4D */	bl sinf
/* 80346ABC 003439FC  EC 75 00 72 */	fmuls f3, f21, f1
/* 80346AC0 00343A00  EC 56 00 72 */	fmuls f2, f22, f1
/* 80346AC4 00343A04  EC 19 00 72 */	fmuls f0, f25, f1
/* 80346AC8 00343A08  D0 61 01 54 */	stfs f3, 0x154(r1)
/* 80346ACC 00343A0C  D0 41 01 58 */	stfs f2, 0x158(r1)
/* 80346AD0 00343A10  D0 01 01 5C */	stfs f0, 0x15c(r1)
/* 80346AD4 00343A14  C0 22 02 AC */	lfs f1, "@3757"@sda21(r2)
/* 80346AD8 00343A18  4B D4 04 99 */	bl cosf
/* 80346ADC 00343A1C  D0 21 01 60 */	stfs f1, 0x160(r1)
/* 80346AE0 00343A20  C1 21 01 5C */	lfs f9, 0x15c(r1)
/* 80346AE4 00343A24  C0 A1 01 54 */	lfs f5, 0x154(r1)
/* 80346AE8 00343A28  EC 29 07 32 */	fmuls f1, f9, f28
/* 80346AEC 00343A2C  C1 61 01 58 */	lfs f11, 0x158(r1)
/* 80346AF0 00343A30  FC 00 28 50 */	fneg f0, f5
/* 80346AF4 00343A34  C1 41 01 60 */	lfs f10, 0x160(r1)
/* 80346AF8 00343A38  EC 69 06 F2 */	fmuls f3, f9, f27
/* 80346AFC 00343A3C  EC 40 0E BA */	fmadds f2, f0, f26, f1
/* 80346B00 00343A40  C0 22 02 78 */	lfs f1, "@3103"@sda21(r2)
/* 80346B04 00343A44  EC 8B 1E B8 */	fmsubs f4, f11, f26, f3
/* 80346B08 00343A48  EC 6B 07 32 */	fmuls f3, f11, f28
/* 80346B0C 00343A4C  ED 0A 16 FA */	fmadds f8, f10, f27, f2
/* 80346B10 00343A50  EC 4B 06 F2 */	fmuls f2, f11, f27
/* 80346B14 00343A54  EC C5 1E F8 */	fmsubs f6, f5, f27, f3
/* 80346B18 00343A58  FD 80 48 50 */	fneg f12, f9
/* 80346B1C 00343A5C  EC A0 17 38 */	fmsubs f5, f0, f28, f2
/* 80346B20 00343A60  EC EA 27 3A */	fmadds f7, f10, f28, f4
/* 80346B24 00343A64  EC 88 03 32 */	fmuls f4, f8, f12
/* 80346B28 00343A68  EC 68 02 B2 */	fmuls f3, f8, f10
/* 80346B2C 00343A6C  FD 60 58 50 */	fneg f11, f11
/* 80346B30 00343A70  EC 48 00 32 */	fmuls f2, f8, f0
/* 80346B34 00343A74  EC CA 36 BA */	fmadds f6, f10, f26, f6
/* 80346B38 00343A78  EC 87 22 BA */	fmadds f4, f7, f10, f4
/* 80346B3C 00343A7C  EC 47 12 F8 */	fmsubs f2, f7, f11, f2
/* 80346B40 00343A80  EC 67 1A 7A */	fmadds f3, f7, f9, f3
/* 80346B44 00343A84  EC A9 2E BC */	fnmsubs f5, f9, f26, f5
/* 80346B48 00343A88  EC 86 22 FC */	fnmsubs f4, f6, f11, f4
/* 80346B4C 00343A8C  EC 46 12 BA */	fmadds f2, f6, f10, f2
/* 80346B50 00343A90  EC 66 18 3A */	fmadds f3, f6, f0, f3
/* 80346B54 00343A94  EC 85 20 3A */	fmadds f4, f5, f0, f4
/* 80346B58 00343A98  EC C5 13 3A */	fmadds f6, f5, f12, f2
/* 80346B5C 00343A9C  EC A5 1A FA */	fmadds f5, f5, f11, f3
/* 80346B60 00343AA0  EC 1E 01 32 */	fmuls f0, f30, f4
/* 80346B64 00343AA4  EC 5F 01 B2 */	fmuls f2, f31, f6
/* 80346B68 00343AA8  EC 7D 01 72 */	fmuls f3, f29, f5
/* 80346B6C 00343AAC  EF 1F 01 78 */	fmsubs f24, f31, f5, f0
/* 80346B70 00343AB0  EE FD 11 38 */	fmsubs f23, f29, f4, f2
/* 80346B74 00343AB4  EE DE 19 B8 */	fmsubs f22, f30, f6, f3
/* 80346B78 00343AB8  EC 17 05 F2 */	fmuls f0, f23, f23
/* 80346B7C 00343ABC  EC 16 05 BA */	fmadds f0, f22, f22, f0
/* 80346B80 00343AC0  EF 38 06 3A */	fmadds f25, f24, f24, f0
/* 80346B84 00343AC4  FC 19 08 40 */	fcmpo cr0, f25, f1
/* 80346B88 00343AC8  4C 40 13 82 */	cror 2, 0, 2
/* 80346B8C 00343ACC  40 82 00 08 */	bne lbl_80346B94
/* 80346B90 00343AD0  48 00 00 28 */	b lbl_80346BB8
lbl_80346B94:
/* 80346B94 00343AD4  FC 60 C8 34 */	frsqrte f3, f25
/* 80346B98 00343AD8  C0 42 02 90 */	lfs f2, "@3340"@sda21(r2)
/* 80346B9C 00343ADC  C0 02 02 94 */	lfs f0, "@3341"@sda21(r2)
/* 80346BA0 00343AE0  FC 60 18 18 */	frsp f3, f3
/* 80346BA4 00343AE4  EC 23 00 F2 */	fmuls f1, f3, f3
/* 80346BA8 00343AE8  EC 42 00 F2 */	fmuls f2, f2, f3
/* 80346BAC 00343AEC  EC 19 00 7C */	fnmsubs f0, f25, f1, f0
/* 80346BB0 00343AF0  EC 02 00 32 */	fmuls f0, f2, f0
/* 80346BB4 00343AF4  EF 39 00 32 */	fmuls f25, f25, f0
lbl_80346BB8:
/* 80346BB8 00343AF8  C0 02 02 A4 */	lfs f0, "@3755"@sda21(r2)
/* 80346BBC 00343AFC  FC 19 00 40 */	fcmpo cr0, f25, f0
/* 80346BC0 00343B00  4C 40 13 82 */	cror 2, 0, 2
/* 80346BC4 00343B04  40 82 00 20 */	bne lbl_80346BE4
/* 80346BC8 00343B08  C0 02 02 78 */	lfs f0, "@3103"@sda21(r2)
/* 80346BCC 00343B0C  D0 01 01 70 */	stfs f0, 0x170(r1)
/* 80346BD0 00343B10  D0 01 01 74 */	stfs f0, 0x174(r1)
/* 80346BD4 00343B14  D0 01 01 78 */	stfs f0, 0x178(r1)
/* 80346BD8 00343B18  C0 02 02 7C */	lfs f0, "@3104"@sda21(r2)
/* 80346BDC 00343B1C  D0 01 01 7C */	stfs f0, 0x17c(r1)
/* 80346BE0 00343B20  48 00 00 58 */	b lbl_80346C38
lbl_80346BE4:
/* 80346BE4 00343B24  EC 1E 01 72 */	fmuls f0, f30, f5
/* 80346BE8 00343B28  FC 20 C8 90 */	fmr f1, f25
/* 80346BEC 00343B2C  EC 1F 01 3A */	fmadds f0, f31, f4, f0
/* 80346BF0 00343B30  EC 5D 01 BA */	fmadds f2, f29, f6, f0
/* 80346BF4 00343B34  4B D4 02 21 */	bl atan2f
/* 80346BF8 00343B38  C0 02 02 90 */	lfs f0, "@3340"@sda21(r2)
/* 80346BFC 00343B3C  C0 42 02 A8 */	lfs f2, "@3756"@sda21(r2)
/* 80346C00 00343B40  EC 00 00 72 */	fmuls f0, f0, f1
/* 80346C04 00343B44  EE A2 00 32 */	fmuls f21, f2, f0
/* 80346C08 00343B48  FC 20 A8 90 */	fmr f1, f21
/* 80346C0C 00343B4C  4B D4 04 F9 */	bl sinf
/* 80346C10 00343B50  EC 21 C8 24 */	fdivs f1, f1, f25
/* 80346C14 00343B54  EC 16 00 72 */	fmuls f0, f22, f1
/* 80346C18 00343B58  EC 57 00 72 */	fmuls f2, f23, f1
/* 80346C1C 00343B5C  D0 01 01 70 */	stfs f0, 0x170(r1)
/* 80346C20 00343B60  EC 18 00 72 */	fmuls f0, f24, f1
/* 80346C24 00343B64  FC 20 A8 90 */	fmr f1, f21
/* 80346C28 00343B68  D0 41 01 74 */	stfs f2, 0x174(r1)
/* 80346C2C 00343B6C  D0 01 01 78 */	stfs f0, 0x178(r1)
/* 80346C30 00343B70  4B D4 03 41 */	bl cosf
/* 80346C34 00343B74  D0 21 01 7C */	stfs f1, 0x17c(r1)
lbl_80346C38:
/* 80346C38 00343B78  C0 A1 01 7C */	lfs f5, 0x17c(r1)
/* 80346C3C 00343B7C  C1 5F 01 9C */	lfs f10, 0x19c(r31)
/* 80346C40 00343B80  C1 1F 01 A0 */	lfs f8, 0x1a0(r31)
/* 80346C44 00343B84  C1 21 01 70 */	lfs f9, 0x170(r1)
/* 80346C48 00343B88  EC 05 02 B2 */	fmuls f0, f5, f10
/* 80346C4C 00343B8C  C0 DF 01 A8 */	lfs f6, 0x1a8(r31)
/* 80346C50 00343B90  EC 25 02 32 */	fmuls f1, f5, f8
/* 80346C54 00343B94  C0 9F 01 A4 */	lfs f4, 0x1a4(r31)
/* 80346C58 00343B98  C1 61 01 74 */	lfs f11, 0x174(r1)
/* 80346C5C 00343B9C  EC 49 01 BA */	fmadds f2, f9, f6, f0
/* 80346C60 00343BA0  C0 E1 01 78 */	lfs f7, 0x178(r1)
/* 80346C64 00343BA4  EC 05 01 32 */	fmuls f0, f5, f4
/* 80346C68 00343BA8  EC 2B 09 BA */	fmadds f1, f11, f6, f1
/* 80346C6C 00343BAC  EC 69 02 B2 */	fmuls f3, f9, f10
/* 80346C70 00343BB0  EC 4B 11 3A */	fmadds f2, f11, f4, f2
/* 80346C74 00343BB4  EC 07 01 BA */	fmadds f0, f7, f6, f0
/* 80346C78 00343BB8  EC 27 0A BA */	fmadds f1, f7, f10, f1
/* 80346C7C 00343BBC  EC 65 19 B8 */	fmsubs f3, f5, f6, f3
/* 80346C80 00343BC0  EC 47 12 3C */	fnmsubs f2, f7, f8, f2
/* 80346C84 00343BC4  EC 09 02 3A */	fmadds f0, f9, f8, f0
/* 80346C88 00343BC8  EC 6B 1A 3C */	fnmsubs f3, f11, f8, f3
/* 80346C8C 00343BCC  EC 29 09 3C */	fnmsubs f1, f9, f4, f1
/* 80346C90 00343BD0  D0 5F 01 9C */	stfs f2, 0x19c(r31)
/* 80346C94 00343BD4  EC 0B 02 BC */	fnmsubs f0, f11, f10, f0
/* 80346C98 00343BD8  EC 47 19 3C */	fnmsubs f2, f7, f4, f3
/* 80346C9C 00343BDC  D0 3F 01 A0 */	stfs f1, 0x1a0(r31)
/* 80346CA0 00343BE0  D0 1F 01 A4 */	stfs f0, 0x1a4(r31)
/* 80346CA4 00343BE4  D0 5F 01 A8 */	stfs f2, 0x1a8(r31)
lbl_80346CA8:
/* 80346CA8 00343BE8  C0 3F 01 9C */	lfs f1, 0x19c(r31)
/* 80346CAC 00343BEC  C0 1F 01 A0 */	lfs f0, 0x1a0(r31)
/* 80346CB0 00343BF0  EC 21 00 72 */	fmuls f1, f1, f1
/* 80346CB4 00343BF4  C0 5F 01 A4 */	lfs f2, 0x1a4(r31)
/* 80346CB8 00343BF8  EC 00 00 32 */	fmuls f0, f0, f0
/* 80346CBC 00343BFC  C0 7F 01 A8 */	lfs f3, 0x1a8(r31)
/* 80346CC0 00343C00  EC 42 00 B2 */	fmuls f2, f2, f2
/* 80346CC4 00343C04  EC 21 00 2A */	fadds f1, f1, f0
/* 80346CC8 00343C08  C0 02 02 A4 */	lfs f0, "@3755"@sda21(r2)
/* 80346CCC 00343C0C  EC 63 00 F2 */	fmuls f3, f3, f3
/* 80346CD0 00343C10  EC 22 08 2A */	fadds f1, f2, f1
/* 80346CD4 00343C14  EC 23 08 2A */	fadds f1, f3, f1
/* 80346CD8 00343C18  FC 01 00 40 */	fcmpo cr0, f1, f0
/* 80346CDC 00343C1C  4C 40 13 82 */	cror 2, 0, 2
/* 80346CE0 00343C20  40 82 00 1C */	bne lbl_80346CFC
/* 80346CE4 00343C24  C0 02 02 78 */	lfs f0, "@3103"@sda21(r2)
/* 80346CE8 00343C28  D0 1F 01 A8 */	stfs f0, 0x1a8(r31)
/* 80346CEC 00343C2C  D0 1F 01 A4 */	stfs f0, 0x1a4(r31)
/* 80346CF0 00343C30  D0 1F 01 A0 */	stfs f0, 0x1a0(r31)
/* 80346CF4 00343C34  D0 1F 01 9C */	stfs f0, 0x19c(r31)
/* 80346CF8 00343C38  48 00 00 40 */	b lbl_80346D38
lbl_80346CFC:
/* 80346CFC 00343C3C  4B D2 60 75 */	bl "inv_sqrt__Q29JGeometry8TUtil<f>Ff"
/* 80346D00 00343C40  C0 42 02 7C */	lfs f2, "@3104"@sda21(r2)
/* 80346D04 00343C44  C0 1F 01 9C */	lfs f0, 0x19c(r31)
/* 80346D08 00343C48  EC 22 00 72 */	fmuls f1, f2, f1
/* 80346D0C 00343C4C  EC 00 00 72 */	fmuls f0, f0, f1
/* 80346D10 00343C50  D0 1F 01 9C */	stfs f0, 0x19c(r31)
/* 80346D14 00343C54  C0 1F 01 A0 */	lfs f0, 0x1a0(r31)
/* 80346D18 00343C58  EC 00 00 72 */	fmuls f0, f0, f1
/* 80346D1C 00343C5C  D0 1F 01 A0 */	stfs f0, 0x1a0(r31)
/* 80346D20 00343C60  C0 1F 01 A4 */	lfs f0, 0x1a4(r31)
/* 80346D24 00343C64  EC 00 00 72 */	fmuls f0, f0, f1
/* 80346D28 00343C68  D0 1F 01 A4 */	stfs f0, 0x1a4(r31)
/* 80346D2C 00343C6C  C0 1F 01 A8 */	lfs f0, 0x1a8(r31)
/* 80346D30 00343C70  EC 00 00 72 */	fmuls f0, f0, f1
/* 80346D34 00343C74  D0 1F 01 A8 */	stfs f0, 0x1a8(r31)
lbl_80346D38:
/* 80346D38 00343C78  80 01 02 0C */	lwz r0, 0x20c(r1)
/* 80346D3C 00343C7C  CB E1 02 00 */	lfd f31, 0x200(r1)
/* 80346D40 00343C80  CB C1 01 F8 */	lfd f30, 0x1f8(r1)
/* 80346D44 00343C84  7C 08 03 A6 */	mtlr r0
/* 80346D48 00343C88  CB A1 01 F0 */	lfd f29, 0x1f0(r1)
/* 80346D4C 00343C8C  CB 81 01 E8 */	lfd f28, 0x1e8(r1)
/* 80346D50 00343C90  CB 61 01 E0 */	lfd f27, 0x1e0(r1)
/* 80346D54 00343C94  CB 41 01 D8 */	lfd f26, 0x1d8(r1)
/* 80346D58 00343C98  CB 21 01 D0 */	lfd f25, 0x1d0(r1)
/* 80346D5C 00343C9C  CB 01 01 C8 */	lfd f24, 0x1c8(r1)
/* 80346D60 00343CA0  CA E1 01 C0 */	lfd f23, 0x1c0(r1)
/* 80346D64 00343CA4  CA C1 01 B8 */	lfd f22, 0x1b8(r1)
/* 80346D68 00343CA8  CA A1 01 B0 */	lfd f21, 0x1b0(r1)
/* 80346D6C 00343CAC  83 E1 01 AC */	lwz r31, 0x1ac(r1)
/* 80346D70 00343CB0  38 21 02 08 */	addi r1, r1, 0x208
/* 80346D74 00343CB4  4E 80 00 20 */	blr 

.global makeCoasterGoalPath__13TCoasterEnemyFv
makeCoasterGoalPath__13TCoasterEnemyFv:
/* 80346D78 00343CB8  7C 08 02 A6 */	mflr r0
/* 80346D7C 00343CBC  90 01 00 04 */	stw r0, 4(r1)
/* 80346D80 00343CC0  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 80346D84 00343CC4  93 E1 00 14 */	stw r31, 0x14(r1)
/* 80346D88 00343CC8  7C 7F 1B 78 */	mr r31, r3
/* 80346D8C 00343CCC  4B F0 88 99 */	bl setGoalPathFromGraph__11TSpineEnemyFv
/* 80346D90 00343CD0  38 00 00 00 */	li r0, 0
/* 80346D94 00343CD4  B0 1F 01 28 */	sth r0, 0x128(r31)
/* 80346D98 00343CD8  C0 02 02 78 */	lfs f0, "@3103"@sda21(r2)
/* 80346D9C 00343CDC  D0 1F 01 2C */	stfs f0, 0x12c(r31)
/* 80346DA0 00343CE0  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 80346DA4 00343CE4  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 80346DA8 00343CE8  38 21 00 18 */	addi r1, r1, 0x18
/* 80346DAC 00343CEC  7C 08 03 A6 */	mtlr r0
/* 80346DB0 00343CF0  4E 80 00 20 */	blr 

.global getGravityY__13TCoasterEnemyCFv
getGravityY__13TCoasterEnemyCFv:
/* 80346DB4 00343CF4  C0 22 02 78 */	lfs f1, "@3103"@sda21(r2)
/* 80346DB8 00343CF8  4E 80 00 20 */	blr 

.global perform__13TCoasterEnemyFUlPQ26JDrama9TGraphics
perform__13TCoasterEnemyFUlPQ26JDrama9TGraphics:
/* 80346DBC 00343CFC  7C 08 02 A6 */	mflr r0
/* 80346DC0 00343D00  90 01 00 04 */	stw r0, 4(r1)
/* 80346DC4 00343D04  94 21 FF E0 */	stwu r1, -0x20(r1)
/* 80346DC8 00343D08  93 E1 00 1C */	stw r31, 0x1c(r1)
/* 80346DCC 00343D0C  7C 9F 23 78 */	mr r31, r4
/* 80346DD0 00343D10  4B F3 71 81 */	bl perform__11TSmallEnemyFUlPQ26JDrama9TGraphics
/* 80346DD4 00343D14  80 01 00 24 */	lwz r0, 0x24(r1)
/* 80346DD8 00343D18  83 E1 00 1C */	lwz r31, 0x1c(r1)
/* 80346DDC 00343D1C  38 21 00 20 */	addi r1, r1, 0x20
/* 80346DE0 00343D20  7C 08 03 A6 */	mtlr r0
/* 80346DE4 00343D24  4E 80 00 20 */	blr 

.global reset__13TCoasterEnemyFv
reset__13TCoasterEnemyFv:
/* 80346DE8 00343D28  7C 08 02 A6 */	mflr r0
/* 80346DEC 00343D2C  90 01 00 04 */	stw r0, 4(r1)
/* 80346DF0 00343D30  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 80346DF4 00343D34  93 E1 00 14 */	stw r31, 0x14(r1)
/* 80346DF8 00343D38  7C 7F 1B 78 */	mr r31, r3
/* 80346DFC 00343D3C  4B F3 DD DD */	bl reset__12TWalkerEnemyFv
/* 80346E00 00343D40  80 7F 00 F0 */	lwz r3, 0xf0(r31)
/* 80346E04 00343D44  38 00 00 00 */	li r0, 0
/* 80346E08 00343D48  54 63 00 3C */	rlwinm r3, r3, 0, 0, 0x1e
/* 80346E0C 00343D4C  90 7F 00 F0 */	stw r3, 0xf0(r31)
/* 80346E10 00343D50  80 7F 00 F0 */	lwz r3, 0xf0(r31)
/* 80346E14 00343D54  60 63 00 80 */	ori r3, r3, 0x80
/* 80346E18 00343D58  90 7F 00 F0 */	stw r3, 0xf0(r31)
/* 80346E1C 00343D5C  90 1F 01 98 */	stw r0, 0x198(r31)
/* 80346E20 00343D60  90 1F 01 94 */	stw r0, 0x194(r31)
/* 80346E24 00343D64  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 80346E28 00343D68  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 80346E2C 00343D6C  38 21 00 18 */	addi r1, r1, 0x18
/* 80346E30 00343D70  7C 08 03 A6 */	mtlr r0
/* 80346E34 00343D74  4E 80 00 20 */	blr 

.global bind__13TCoasterEnemyFv
bind__13TCoasterEnemyFv:
/* 80346E38 00343D78  7C 08 02 A6 */	mflr r0
/* 80346E3C 00343D7C  90 01 00 04 */	stw r0, 4(r1)
/* 80346E40 00343D80  94 21 FF C0 */	stwu r1, -0x40(r1)
/* 80346E44 00343D84  93 E1 00 3C */	stw r31, 0x3c(r1)
/* 80346E48 00343D88  7C 7F 1B 78 */	mr r31, r3
/* 80346E4C 00343D8C  80 83 00 10 */	lwz r4, 0x10(r3)
/* 80346E50 00343D90  80 03 00 14 */	lwz r0, 0x14(r3)
/* 80346E54 00343D94  38 61 00 10 */	addi r3, r1, 0x10
/* 80346E58 00343D98  90 81 00 28 */	stw r4, 0x28(r1)
/* 80346E5C 00343D9C  38 9F 00 10 */	addi r4, r31, 0x10
/* 80346E60 00343DA0  90 01 00 2C */	stw r0, 0x2c(r1)
/* 80346E64 00343DA4  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 80346E68 00343DA8  90 01 00 30 */	stw r0, 0x30(r1)
/* 80346E6C 00343DAC  C0 21 00 28 */	lfs f1, 0x28(r1)
/* 80346E70 00343DB0  C0 1F 00 94 */	lfs f0, 0x94(r31)
/* 80346E74 00343DB4  EC 01 00 2A */	fadds f0, f1, f0
/* 80346E78 00343DB8  D0 01 00 28 */	stfs f0, 0x28(r1)
/* 80346E7C 00343DBC  C0 21 00 2C */	lfs f1, 0x2c(r1)
/* 80346E80 00343DC0  C0 1F 00 98 */	lfs f0, 0x98(r31)
/* 80346E84 00343DC4  EC 01 00 2A */	fadds f0, f1, f0
/* 80346E88 00343DC8  D0 01 00 2C */	stfs f0, 0x2c(r1)
/* 80346E8C 00343DCC  C0 21 00 30 */	lfs f1, 0x30(r1)
/* 80346E90 00343DD0  C0 1F 00 9C */	lfs f0, 0x9c(r31)
/* 80346E94 00343DD4  EC 01 00 2A */	fadds f0, f1, f0
/* 80346E98 00343DD8  D0 01 00 30 */	stfs f0, 0x30(r1)
/* 80346E9C 00343DDC  C0 21 00 28 */	lfs f1, 0x28(r1)
/* 80346EA0 00343DE0  C0 1F 00 AC */	lfs f0, 0xac(r31)
/* 80346EA4 00343DE4  EC 01 00 2A */	fadds f0, f1, f0
/* 80346EA8 00343DE8  D0 01 00 28 */	stfs f0, 0x28(r1)
/* 80346EAC 00343DEC  C0 21 00 2C */	lfs f1, 0x2c(r1)
/* 80346EB0 00343DF0  C0 1F 00 B0 */	lfs f0, 0xb0(r31)
/* 80346EB4 00343DF4  EC 01 00 2A */	fadds f0, f1, f0
/* 80346EB8 00343DF8  D0 01 00 2C */	stfs f0, 0x2c(r1)
/* 80346EBC 00343DFC  C0 21 00 30 */	lfs f1, 0x30(r1)
/* 80346EC0 00343E00  C0 1F 00 B4 */	lfs f0, 0xb4(r31)
/* 80346EC4 00343E04  EC 01 00 2A */	fadds f0, f1, f0
/* 80346EC8 00343E08  D0 01 00 30 */	stfs f0, 0x30(r1)
/* 80346ECC 00343E0C  80 01 00 28 */	lwz r0, 0x28(r1)
/* 80346ED0 00343E10  80 A1 00 2C */	lwz r5, 0x2c(r1)
/* 80346ED4 00343E14  90 01 00 10 */	stw r0, 0x10(r1)
/* 80346ED8 00343E18  80 01 00 30 */	lwz r0, 0x30(r1)
/* 80346EDC 00343E1C  90 A1 00 14 */	stw r5, 0x14(r1)
/* 80346EE0 00343E20  90 01 00 18 */	stw r0, 0x18(r1)
/* 80346EE4 00343E24  4B D7 B1 A5 */	bl "sub__Q29JGeometry8TVec3<f>FRCQ29JGeometry8TVec3<f>"
/* 80346EE8 00343E28  80 61 00 10 */	lwz r3, 0x10(r1)
/* 80346EEC 00343E2C  80 01 00 14 */	lwz r0, 0x14(r1)
/* 80346EF0 00343E30  90 7F 00 94 */	stw r3, 0x94(r31)
/* 80346EF4 00343E34  90 1F 00 98 */	stw r0, 0x98(r31)
/* 80346EF8 00343E38  80 01 00 18 */	lwz r0, 0x18(r1)
/* 80346EFC 00343E3C  90 1F 00 9C */	stw r0, 0x9c(r31)
/* 80346F00 00343E40  80 01 00 44 */	lwz r0, 0x44(r1)
/* 80346F04 00343E44  83 E1 00 3C */	lwz r31, 0x3c(r1)
/* 80346F08 00343E48  38 21 00 40 */	addi r1, r1, 0x40
/* 80346F0C 00343E4C  7C 08 03 A6 */	mtlr r0
/* 80346F10 00343E50  4E 80 00 20 */	blr 

.global moveObject__13TCoasterEnemyFv
moveObject__13TCoasterEnemyFv:
/* 80346F14 00343E54  7C 08 02 A6 */	mflr r0
/* 80346F18 00343E58  90 01 00 04 */	stw r0, 4(r1)
/* 80346F1C 00343E5C  94 21 FF F8 */	stwu r1, -8(r1)
/* 80346F20 00343E60  4B F3 DF D1 */	bl moveObject__12TWalkerEnemyFv
/* 80346F24 00343E64  80 01 00 0C */	lwz r0, 0xc(r1)
/* 80346F28 00343E68  38 21 00 08 */	addi r1, r1, 8
/* 80346F2C 00343E6C  7C 08 03 A6 */	mtlr r0
/* 80346F30 00343E70  4E 80 00 20 */	blr 

.global init__13TCoasterEnemyFP12TLiveManager
init__13TCoasterEnemyFP12TLiveManager:
/* 80346F34 00343E74  7C 08 02 A6 */	mflr r0
/* 80346F38 00343E78  90 01 00 04 */	stw r0, 4(r1)
/* 80346F3C 00343E7C  94 21 FF D0 */	stwu r1, -0x30(r1)
/* 80346F40 00343E80  93 E1 00 2C */	stw r31, 0x2c(r1)
/* 80346F44 00343E84  7C 7F 1B 78 */	mr r31, r3
/* 80346F48 00343E88  4B F3 E1 79 */	bl init__12TWalkerEnemyFP12TLiveManager
/* 80346F4C 00343E8C  80 6D 9C 28 */	lwz r3, gpConductor@sda21(r13)
/* 80346F50 00343E90  38 82 02 9C */	addi r4, r2, "@3398"@sda21
/* 80346F54 00343E94  4B F0 1D F5 */	bl getGraphByName__10TConductorFPCc
/* 80346F58 00343E98  80 9F 01 24 */	lwz r4, 0x124(r31)
/* 80346F5C 00343E9C  38 00 FF FF */	li r0, -1
/* 80346F60 00343EA0  90 64 00 00 */	stw r3, 0(r4)
/* 80346F64 00343EA4  7F E3 FB 78 */	mr r3, r31
/* 80346F68 00343EA8  80 9F 01 24 */	lwz r4, 0x124(r31)
/* 80346F6C 00343EAC  90 04 00 08 */	stw r0, 8(r4)
/* 80346F70 00343EB0  4B F0 85 91 */	bl goToShortestNextGraphNode__11TSpineEnemyFv
/* 80346F74 00343EB4  88 0D A8 00 */	lbz r0, init$2870@sda21(r13)
/* 80346F78 00343EB8  7C 00 07 75 */	extsb. r0, r0
/* 80346F7C 00343EBC  40 82 00 3C */	bne lbl_80346FB8
/* 80346F80 00343EC0  3C 60 80 3C */	lis r3, "__vt__24TNerveBase<10TLiveActor>"@ha
/* 80346F84 00343EC4  38 03 92 60 */	addi r0, r3, "__vt__24TNerveBase<10TLiveActor>"@l
/* 80346F88 00343EC8  3C 60 80 3E */	lis r3, __vt__24TNerveCoasterEnemyWander@ha
/* 80346F8C 00343ECC  90 0D A8 04 */	stw r0, instance$2868@sda21(r13)
/* 80346F90 00343ED0  38 03 36 74 */	addi r0, r3, __vt__24TNerveCoasterEnemyWander@l
/* 80346F94 00343ED4  3C 80 80 34 */	lis r4, __dt__24TNerveCoasterEnemyWanderFv@ha
/* 80346F98 00343ED8  90 0D A8 04 */	stw r0, instance$2868@sda21(r13)
/* 80346F9C 00343EDC  3C 60 80 40 */	lis r3, "@2869"@ha
/* 80346FA0 00343EE0  38 A3 00 E0 */	addi r5, r3, "@2869"@l
/* 80346FA4 00343EE4  38 84 64 14 */	addi r4, r4, __dt__24TNerveCoasterEnemyWanderFv@l
/* 80346FA8 00343EE8  38 6D A8 04 */	addi r3, r13, instance$2868@sda21
/* 80346FAC 00343EEC  4B D3 B7 7D */	bl __register_global_object
/* 80346FB0 00343EF0  38 00 00 01 */	li r0, 1
/* 80346FB4 00343EF4  98 0D A8 00 */	stb r0, init$2870@sda21(r13)
lbl_80346FB8:
/* 80346FB8 00343EF8  80 9F 00 8C */	lwz r4, 0x8c(r31)
/* 80346FBC 00343EFC  38 60 00 00 */	li r3, 0
/* 80346FC0 00343F00  38 0D A8 04 */	addi r0, r13, instance$2868@sda21
/* 80346FC4 00343F04  90 64 00 08 */	stw r3, 8(r4)
/* 80346FC8 00343F08  90 64 00 20 */	stw r3, 0x20(r4)
/* 80346FCC 00343F0C  90 04 00 14 */	stw r0, 0x14(r4)
/* 80346FD0 00343F10  90 04 00 18 */	stw r0, 0x18(r4)
/* 80346FD4 00343F14  90 64 00 1C */	stw r3, 0x1c(r4)
/* 80346FD8 00343F18  80 1F 00 F0 */	lwz r0, 0xf0(r31)
/* 80346FDC 00343F1C  60 00 00 01 */	ori r0, r0, 1
/* 80346FE0 00343F20  90 1F 00 F0 */	stw r0, 0xf0(r31)
/* 80346FE4 00343F24  80 1F 00 F0 */	lwz r0, 0xf0(r31)
/* 80346FE8 00343F28  60 00 00 10 */	ori r0, r0, 0x10
/* 80346FEC 00343F2C  90 1F 00 F0 */	stw r0, 0xf0(r31)
/* 80346FF0 00343F30  C0 02 02 78 */	lfs f0, "@3103"@sda21(r2)
/* 80346FF4 00343F34  D0 1F 01 9C */	stfs f0, 0x19c(r31)
/* 80346FF8 00343F38  D0 1F 01 A0 */	stfs f0, 0x1a0(r31)
/* 80346FFC 00343F3C  D0 1F 01 A4 */	stfs f0, 0x1a4(r31)
/* 80347000 00343F40  C0 02 02 7C */	lfs f0, "@3104"@sda21(r2)
/* 80347004 00343F44  D0 1F 01 A8 */	stfs f0, 0x1a8(r31)
/* 80347008 00343F48  80 01 00 34 */	lwz r0, 0x34(r1)
/* 8034700C 00343F4C  83 E1 00 2C */	lwz r31, 0x2c(r1)
/* 80347010 00343F50  38 21 00 30 */	addi r1, r1, 0x30
/* 80347014 00343F54  7C 08 03 A6 */	mtlr r0
/* 80347018 00343F58  4E 80 00 20 */	blr 

.global __dt__14TCoasterKillerFv
__dt__14TCoasterKillerFv:
/* 8034701C 00343F5C  7C 08 02 A6 */	mflr r0
/* 80347020 00343F60  90 01 00 04 */	stw r0, 4(r1)
/* 80347024 00343F64  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 80347028 00343F68  93 E1 00 14 */	stw r31, 0x14(r1)
/* 8034702C 00343F6C  3B E4 00 00 */	addi r31, r4, 0
/* 80347030 00343F70  93 C1 00 10 */	stw r30, 0x10(r1)
/* 80347034 00343F74  7C 7E 1B 79 */	or. r30, r3, r3
/* 80347038 00343F78  41 82 00 64 */	beq lbl_8034709C
/* 8034703C 00343F7C  3C 60 80 3E */	lis r3, __vt__14TCoasterKiller@ha
/* 80347040 00343F80  38 63 34 B0 */	addi r3, r3, __vt__14TCoasterKiller@l
/* 80347044 00343F84  90 7E 00 00 */	stw r3, 0(r30)
/* 80347048 00343F88  38 03 00 24 */	addi r0, r3, 0x24
/* 8034704C 00343F8C  90 1E 00 20 */	stw r0, 0x20(r30)
/* 80347050 00343F90  41 82 00 3C */	beq lbl_8034708C
/* 80347054 00343F94  3C 60 80 3E */	lis r3, __vt__13TCoasterEnemy@ha
/* 80347058 00343F98  38 63 36 84 */	addi r3, r3, __vt__13TCoasterEnemy@l
/* 8034705C 00343F9C  90 7E 00 00 */	stw r3, 0(r30)
/* 80347060 00343FA0  38 03 00 24 */	addi r0, r3, 0x24
/* 80347064 00343FA4  90 1E 00 20 */	stw r0, 0x20(r30)
/* 80347068 00343FA8  41 82 00 24 */	beq lbl_8034708C
/* 8034706C 00343FAC  3C 60 80 3D */	lis r3, __vt__12TWalkerEnemy@ha
/* 80347070 00343FB0  38 63 6F E8 */	addi r3, r3, __vt__12TWalkerEnemy@l
/* 80347074 00343FB4  90 7E 00 00 */	stw r3, 0(r30)
/* 80347078 00343FB8  38 03 00 24 */	addi r0, r3, 0x24
/* 8034707C 00343FBC  38 7E 00 00 */	addi r3, r30, 0
/* 80347080 00343FC0  90 1E 00 20 */	stw r0, 0x20(r30)
/* 80347084 00343FC4  38 80 00 00 */	li r4, 0
/* 80347088 00343FC8  4B F1 6B D1 */	bl __dt__11TSmallEnemyFv
lbl_8034708C:
/* 8034708C 00343FCC  7F E0 07 35 */	extsh. r0, r31
/* 80347090 00343FD0  40 81 00 0C */	ble lbl_8034709C
/* 80347094 00343FD4  7F C3 F3 78 */	mr r3, r30
/* 80347098 00343FD8  4B CC 5A 19 */	bl __dl__FPv
lbl_8034709C:
/* 8034709C 00343FDC  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 803470A0 00343FE0  7F C3 F3 78 */	mr r3, r30
/* 803470A4 00343FE4  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 803470A8 00343FE8  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 803470AC 00343FEC  7C 08 03 A6 */	mtlr r0
/* 803470B0 00343FF0  38 21 00 18 */	addi r1, r1, 0x18
/* 803470B4 00343FF4  4E 80 00 20 */	blr 

.global __sinit_coasterkiller_cpp
__sinit_coasterkiller_cpp:
/* 803470B8 00343FF8  7C 08 02 A6 */	mflr r0
/* 803470BC 00343FFC  3C 60 80 40 */	lis r3, "@2869"@ha
/* 803470C0 00344000  90 01 00 04 */	stw r0, 4(r1)
/* 803470C4 00344004  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 803470C8 00344008  93 E1 00 0C */	stw r31, 0xc(r1)
/* 803470CC 0034400C  3B E3 00 E0 */	addi r31, r3, "@2869"@l
/* 803470D0 00344010  88 0D 97 CC */	lbz r0, "__init__smList__15JALList<5MSBgm>"@sda21(r13)
/* 803470D4 00344014  7C 00 07 75 */	extsb. r0, r0
/* 803470D8 00344018  40 82 00 28 */	bne lbl_80347100
/* 803470DC 0034401C  38 6D 97 A8 */	addi r3, r13, "smList__15JALList<5MSBgm>"@sda21
/* 803470E0 00344020  4B CC 7B 55 */	bl initiate__10JSUPtrListFv
/* 803470E4 00344024  3C 60 80 0E */	lis r3, "__dt__15JSUList<5MSBgm>Fv"@ha
/* 803470E8 00344028  38 83 6A 44 */	addi r4, r3, "__dt__15JSUList<5MSBgm>Fv"@l
/* 803470EC 0034402C  38 6D 97 A8 */	addi r3, r13, "smList__15JALList<5MSBgm>"@sda21
/* 803470F0 00344030  38 BF 00 18 */	addi r5, r31, 0x18
/* 803470F4 00344034  4B D3 B6 35 */	bl __register_global_object
/* 803470F8 00344038  38 00 00 01 */	li r0, 1
/* 803470FC 0034403C  98 0D 97 CC */	stb r0, "__init__smList__15JALList<5MSBgm>"@sda21(r13)
lbl_80347100:
/* 80347100 00344040  88 0D 97 CD */	lbz r0, "__init__smList__24JALList<13MSSetSoundGrp>"@sda21(r13)
/* 80347104 00344044  7C 00 07 75 */	extsb. r0, r0
/* 80347108 00344048  40 82 00 28 */	bne lbl_80347130
/* 8034710C 0034404C  38 6D 97 B4 */	addi r3, r13, "smList__24JALList<13MSSetSoundGrp>"@sda21
/* 80347110 00344050  4B CC 7B 25 */	bl initiate__10JSUPtrListFv
/* 80347114 00344054  3C 60 80 0E */	lis r3, "__dt__24JSUList<13MSSetSoundGrp>Fv"@ha
/* 80347118 00344058  38 83 69 EC */	addi r4, r3, "__dt__24JSUList<13MSSetSoundGrp>Fv"@l
/* 8034711C 0034405C  38 6D 97 B4 */	addi r3, r13, "smList__24JALList<13MSSetSoundGrp>"@sda21
/* 80347120 00344060  38 BF 00 24 */	addi r5, r31, 0x24
/* 80347124 00344064  4B D3 B6 05 */	bl __register_global_object
/* 80347128 00344068  38 00 00 01 */	li r0, 1
/* 8034712C 0034406C  98 0D 97 CD */	stb r0, "__init__smList__24JALList<13MSSetSoundGrp>"@sda21(r13)
lbl_80347130:
/* 80347130 00344070  88 0D 97 CE */	lbz r0, "__init__smList__21JALList<10MSSetSound>"@sda21(r13)
/* 80347134 00344074  7C 00 07 75 */	extsb. r0, r0
/* 80347138 00344078  40 82 00 28 */	bne lbl_80347160
/* 8034713C 0034407C  38 6D 97 C0 */	addi r3, r13, "smList__21JALList<10MSSetSound>"@sda21
/* 80347140 00344080  4B CC 7A F5 */	bl initiate__10JSUPtrListFv
/* 80347144 00344084  3C 60 80 0E */	lis r3, "__dt__21JSUList<10MSSetSound>Fv"@ha
/* 80347148 00344088  38 83 69 94 */	addi r4, r3, "__dt__21JSUList<10MSSetSound>Fv"@l
/* 8034714C 0034408C  38 6D 97 C0 */	addi r3, r13, "smList__21JALList<10MSSetSound>"@sda21
/* 80347150 00344090  38 BF 00 30 */	addi r5, r31, 0x30
/* 80347154 00344094  4B D3 B5 D5 */	bl __register_global_object
/* 80347158 00344098  38 00 00 01 */	li r0, 1
/* 8034715C 0034409C  98 0D 97 CE */	stb r0, "__init__smList__21JALList<10MSSetSound>"@sda21(r13)
lbl_80347160:
/* 80347160 003440A0  88 0D 8F 8C */	lbz r0, "__init__smList__26JALList<15JALSeModEffDGrp>"@sda21(r13)
/* 80347164 003440A4  7C 00 07 75 */	extsb. r0, r0
/* 80347168 003440A8  40 82 00 28 */	bne lbl_80347190
/* 8034716C 003440AC  38 6D 8E FC */	addi r3, r13, "smList__26JALList<15JALSeModEffDGrp>"@sda21
/* 80347170 003440B0  4B CC 7A C5 */	bl initiate__10JSUPtrListFv
/* 80347174 003440B4  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModEffDGrp>Fv"@ha
/* 80347178 003440B8  38 83 A8 0C */	addi r4, r3, "__dt__26JSUList<15JALSeModEffDGrp>Fv"@l
/* 8034717C 003440BC  38 6D 8E FC */	addi r3, r13, "smList__26JALList<15JALSeModEffDGrp>"@sda21
/* 80347180 003440C0  38 BF 00 3C */	addi r5, r31, 0x3c
/* 80347184 003440C4  4B D3 B5 A5 */	bl __register_global_object
/* 80347188 003440C8  38 00 00 01 */	li r0, 1
/* 8034718C 003440CC  98 0D 8F 8C */	stb r0, "__init__smList__26JALList<15JALSeModEffDGrp>"@sda21(r13)
lbl_80347190:
/* 80347190 003440D0  88 0D 8F 8D */	lbz r0, "__init__smList__26JALList<15JALSeModPitDGrp>"@sda21(r13)
/* 80347194 003440D4  7C 00 07 75 */	extsb. r0, r0
/* 80347198 003440D8  40 82 00 28 */	bne lbl_803471C0
/* 8034719C 003440DC  38 6D 8F 08 */	addi r3, r13, "smList__26JALList<15JALSeModPitDGrp>"@sda21
/* 803471A0 003440E0  4B CC 7A 95 */	bl initiate__10JSUPtrListFv
/* 803471A4 003440E4  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModPitDGrp>Fv"@ha
/* 803471A8 003440E8  38 83 A7 B4 */	addi r4, r3, "__dt__26JSUList<15JALSeModPitDGrp>Fv"@l
/* 803471AC 003440EC  38 6D 8F 08 */	addi r3, r13, "smList__26JALList<15JALSeModPitDGrp>"@sda21
/* 803471B0 003440F0  38 BF 00 48 */	addi r5, r31, 0x48
/* 803471B4 003440F4  4B D3 B5 75 */	bl __register_global_object
/* 803471B8 003440F8  38 00 00 01 */	li r0, 1
/* 803471BC 003440FC  98 0D 8F 8D */	stb r0, "__init__smList__26JALList<15JALSeModPitDGrp>"@sda21(r13)
lbl_803471C0:
/* 803471C0 00344100  88 0D 8F 8E */	lbz r0, "__init__smList__26JALList<15JALSeModVolDGrp>"@sda21(r13)
/* 803471C4 00344104  7C 00 07 75 */	extsb. r0, r0
/* 803471C8 00344108  40 82 00 28 */	bne lbl_803471F0
/* 803471CC 0034410C  38 6D 8F 14 */	addi r3, r13, "smList__26JALList<15JALSeModVolDGrp>"@sda21
/* 803471D0 00344110  4B CC 7A 65 */	bl initiate__10JSUPtrListFv
/* 803471D4 00344114  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModVolDGrp>Fv"@ha
/* 803471D8 00344118  38 83 A7 5C */	addi r4, r3, "__dt__26JSUList<15JALSeModVolDGrp>Fv"@l
/* 803471DC 0034411C  38 6D 8F 14 */	addi r3, r13, "smList__26JALList<15JALSeModVolDGrp>"@sda21
/* 803471E0 00344120  38 BF 00 54 */	addi r5, r31, 0x54
/* 803471E4 00344124  4B D3 B5 45 */	bl __register_global_object
/* 803471E8 00344128  38 00 00 01 */	li r0, 1
/* 803471EC 0034412C  98 0D 8F 8E */	stb r0, "__init__smList__26JALList<15JALSeModVolDGrp>"@sda21(r13)
lbl_803471F0:
/* 803471F0 00344130  88 0D 8F 8F */	lbz r0, "__init__smList__26JALList<15JALSeModEffFGrp>"@sda21(r13)
/* 803471F4 00344134  7C 00 07 75 */	extsb. r0, r0
/* 803471F8 00344138  40 82 00 28 */	bne lbl_80347220
/* 803471FC 0034413C  38 6D 8F 20 */	addi r3, r13, "smList__26JALList<15JALSeModEffFGrp>"@sda21
/* 80347200 00344140  4B CC 7A 35 */	bl initiate__10JSUPtrListFv
/* 80347204 00344144  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModEffFGrp>Fv"@ha
/* 80347208 00344148  38 83 A7 04 */	addi r4, r3, "__dt__26JSUList<15JALSeModEffFGrp>Fv"@l
/* 8034720C 0034414C  38 6D 8F 20 */	addi r3, r13, "smList__26JALList<15JALSeModEffFGrp>"@sda21
/* 80347210 00344150  38 BF 00 60 */	addi r5, r31, 0x60
/* 80347214 00344154  4B D3 B5 15 */	bl __register_global_object
/* 80347218 00344158  38 00 00 01 */	li r0, 1
/* 8034721C 0034415C  98 0D 8F 8F */	stb r0, "__init__smList__26JALList<15JALSeModEffFGrp>"@sda21(r13)
lbl_80347220:
/* 80347220 00344160  88 0D 8F 90 */	lbz r0, "__init__smList__26JALList<15JALSeModPitFGrp>"@sda21(r13)
/* 80347224 00344164  7C 00 07 75 */	extsb. r0, r0
/* 80347228 00344168  40 82 00 28 */	bne lbl_80347250
/* 8034722C 0034416C  38 6D 8F 2C */	addi r3, r13, "smList__26JALList<15JALSeModPitFGrp>"@sda21
/* 80347230 00344170  4B CC 7A 05 */	bl initiate__10JSUPtrListFv
/* 80347234 00344174  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModPitFGrp>Fv"@ha
/* 80347238 00344178  38 83 A6 AC */	addi r4, r3, "__dt__26JSUList<15JALSeModPitFGrp>Fv"@l
/* 8034723C 0034417C  38 6D 8F 2C */	addi r3, r13, "smList__26JALList<15JALSeModPitFGrp>"@sda21
/* 80347240 00344180  38 BF 00 6C */	addi r5, r31, 0x6c
/* 80347244 00344184  4B D3 B4 E5 */	bl __register_global_object
/* 80347248 00344188  38 00 00 01 */	li r0, 1
/* 8034724C 0034418C  98 0D 8F 90 */	stb r0, "__init__smList__26JALList<15JALSeModPitFGrp>"@sda21(r13)
lbl_80347250:
/* 80347250 00344190  88 0D 8F 91 */	lbz r0, "__init__smList__26JALList<15JALSeModVolFGrp>"@sda21(r13)
/* 80347254 00344194  7C 00 07 75 */	extsb. r0, r0
/* 80347258 00344198  40 82 00 28 */	bne lbl_80347280
/* 8034725C 0034419C  38 6D 8F 38 */	addi r3, r13, "smList__26JALList<15JALSeModVolFGrp>"@sda21
/* 80347260 003441A0  4B CC 79 D5 */	bl initiate__10JSUPtrListFv
/* 80347264 003441A4  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModVolFGrp>Fv"@ha
/* 80347268 003441A8  38 83 A6 54 */	addi r4, r3, "__dt__26JSUList<15JALSeModVolFGrp>Fv"@l
/* 8034726C 003441AC  38 6D 8F 38 */	addi r3, r13, "smList__26JALList<15JALSeModVolFGrp>"@sda21
/* 80347270 003441B0  38 BF 00 78 */	addi r5, r31, 0x78
/* 80347274 003441B4  4B D3 B4 B5 */	bl __register_global_object
/* 80347278 003441B8  38 00 00 01 */	li r0, 1
/* 8034727C 003441BC  98 0D 8F 91 */	stb r0, "__init__smList__26JALList<15JALSeModVolFGrp>"@sda21(r13)
lbl_80347280:
/* 80347280 003441C0  88 0D 8F 92 */	lbz r0, "__init__smList__26JALList<15JALSeModEffDist>"@sda21(r13)
/* 80347284 003441C4  7C 00 07 75 */	extsb. r0, r0
/* 80347288 003441C8  40 82 00 28 */	bne lbl_803472B0
/* 8034728C 003441CC  38 6D 8F 44 */	addi r3, r13, "smList__26JALList<15JALSeModEffDist>"@sda21
/* 80347290 003441D0  4B CC 79 A5 */	bl initiate__10JSUPtrListFv
/* 80347294 003441D4  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModEffDist>Fv"@ha
/* 80347298 003441D8  38 83 A5 FC */	addi r4, r3, "__dt__26JSUList<15JALSeModEffDist>Fv"@l
/* 8034729C 003441DC  38 6D 8F 44 */	addi r3, r13, "smList__26JALList<15JALSeModEffDist>"@sda21
/* 803472A0 003441E0  38 BF 00 84 */	addi r5, r31, 0x84
/* 803472A4 003441E4  4B D3 B4 85 */	bl __register_global_object
/* 803472A8 003441E8  38 00 00 01 */	li r0, 1
/* 803472AC 003441EC  98 0D 8F 92 */	stb r0, "__init__smList__26JALList<15JALSeModEffDist>"@sda21(r13)
lbl_803472B0:
/* 803472B0 003441F0  88 0D 8F 93 */	lbz r0, "__init__smList__26JALList<15JALSeModPitDist>"@sda21(r13)
/* 803472B4 003441F4  7C 00 07 75 */	extsb. r0, r0
/* 803472B8 003441F8  40 82 00 28 */	bne lbl_803472E0
/* 803472BC 003441FC  38 6D 8F 50 */	addi r3, r13, "smList__26JALList<15JALSeModPitDist>"@sda21
/* 803472C0 00344200  4B CC 79 75 */	bl initiate__10JSUPtrListFv
/* 803472C4 00344204  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModPitDist>Fv"@ha
/* 803472C8 00344208  38 83 A5 A4 */	addi r4, r3, "__dt__26JSUList<15JALSeModPitDist>Fv"@l
/* 803472CC 0034420C  38 6D 8F 50 */	addi r3, r13, "smList__26JALList<15JALSeModPitDist>"@sda21
/* 803472D0 00344210  38 BF 00 90 */	addi r5, r31, 0x90
/* 803472D4 00344214  4B D3 B4 55 */	bl __register_global_object
/* 803472D8 00344218  38 00 00 01 */	li r0, 1
/* 803472DC 0034421C  98 0D 8F 93 */	stb r0, "__init__smList__26JALList<15JALSeModPitDist>"@sda21(r13)
lbl_803472E0:
/* 803472E0 00344220  88 0D 8F 94 */	lbz r0, "__init__smList__26JALList<15JALSeModVolDist>"@sda21(r13)
/* 803472E4 00344224  7C 00 07 75 */	extsb. r0, r0
/* 803472E8 00344228  40 82 00 28 */	bne lbl_80347310
/* 803472EC 0034422C  38 6D 8F 5C */	addi r3, r13, "smList__26JALList<15JALSeModVolDist>"@sda21
/* 803472F0 00344230  4B CC 79 45 */	bl initiate__10JSUPtrListFv
/* 803472F4 00344234  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModVolDist>Fv"@ha
/* 803472F8 00344238  38 83 A5 4C */	addi r4, r3, "__dt__26JSUList<15JALSeModVolDist>Fv"@l
/* 803472FC 0034423C  38 6D 8F 5C */	addi r3, r13, "smList__26JALList<15JALSeModVolDist>"@sda21
/* 80347300 00344240  38 BF 00 9C */	addi r5, r31, 0x9c
/* 80347304 00344244  4B D3 B4 25 */	bl __register_global_object
/* 80347308 00344248  38 00 00 01 */	li r0, 1
/* 8034730C 0034424C  98 0D 8F 94 */	stb r0, "__init__smList__26JALList<15JALSeModVolDist>"@sda21(r13)
lbl_80347310:
/* 80347310 00344250  88 0D 8F 95 */	lbz r0, "__init__smList__26JALList<15JALSeModEffFunk>"@sda21(r13)
/* 80347314 00344254  7C 00 07 75 */	extsb. r0, r0
/* 80347318 00344258  40 82 00 28 */	bne lbl_80347340
/* 8034731C 0034425C  38 6D 8F 68 */	addi r3, r13, "smList__26JALList<15JALSeModEffFunk>"@sda21
/* 80347320 00344260  4B CC 79 15 */	bl initiate__10JSUPtrListFv
/* 80347324 00344264  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModEffFunk>Fv"@ha
/* 80347328 00344268  38 83 A4 F4 */	addi r4, r3, "__dt__26JSUList<15JALSeModEffFunk>Fv"@l
/* 8034732C 0034426C  38 6D 8F 68 */	addi r3, r13, "smList__26JALList<15JALSeModEffFunk>"@sda21
/* 80347330 00344270  38 BF 00 A8 */	addi r5, r31, 0xa8
/* 80347334 00344274  4B D3 B3 F5 */	bl __register_global_object
/* 80347338 00344278  38 00 00 01 */	li r0, 1
/* 8034733C 0034427C  98 0D 8F 95 */	stb r0, "__init__smList__26JALList<15JALSeModEffFunk>"@sda21(r13)
lbl_80347340:
/* 80347340 00344280  88 0D 8F 96 */	lbz r0, "__init__smList__26JALList<15JALSeModPitFunk>"@sda21(r13)
/* 80347344 00344284  7C 00 07 75 */	extsb. r0, r0
/* 80347348 00344288  40 82 00 28 */	bne lbl_80347370
/* 8034734C 0034428C  38 6D 8F 74 */	addi r3, r13, "smList__26JALList<15JALSeModPitFunk>"@sda21
/* 80347350 00344290  4B CC 78 E5 */	bl initiate__10JSUPtrListFv
/* 80347354 00344294  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModPitFunk>Fv"@ha
/* 80347358 00344298  38 83 A4 9C */	addi r4, r3, "__dt__26JSUList<15JALSeModPitFunk>Fv"@l
/* 8034735C 0034429C  38 6D 8F 74 */	addi r3, r13, "smList__26JALList<15JALSeModPitFunk>"@sda21
/* 80347360 003442A0  38 BF 00 B4 */	addi r5, r31, 0xb4
/* 80347364 003442A4  4B D3 B3 C5 */	bl __register_global_object
/* 80347368 003442A8  38 00 00 01 */	li r0, 1
/* 8034736C 003442AC  98 0D 8F 96 */	stb r0, "__init__smList__26JALList<15JALSeModPitFunk>"@sda21(r13)
lbl_80347370:
/* 80347370 003442B0  88 0D 8F 97 */	lbz r0, "__init__smList__26JALList<15JALSeModVolFunk>"@sda21(r13)
/* 80347374 003442B4  7C 00 07 75 */	extsb. r0, r0
/* 80347378 003442B8  40 82 00 28 */	bne lbl_803473A0
/* 8034737C 003442BC  38 6D 8F 80 */	addi r3, r13, "smList__26JALList<15JALSeModVolFunk>"@sda21
/* 80347380 003442C0  4B CC 78 B5 */	bl initiate__10JSUPtrListFv
/* 80347384 003442C4  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModVolFunk>Fv"@ha
/* 80347388 003442C8  38 83 A4 44 */	addi r4, r3, "__dt__26JSUList<15JALSeModVolFunk>Fv"@l
/* 8034738C 003442CC  38 6D 8F 80 */	addi r3, r13, "smList__26JALList<15JALSeModVolFunk>"@sda21
/* 80347390 003442D0  38 BF 00 C0 */	addi r5, r31, 0xc0
/* 80347394 003442D4  4B D3 B3 95 */	bl __register_global_object
/* 80347398 003442D8  38 00 00 01 */	li r0, 1
/* 8034739C 003442DC  98 0D 8F 97 */	stb r0, "__init__smList__26JALList<15JALSeModVolFunk>"@sda21(r13)
lbl_803473A0:
/* 803473A0 003442E0  80 01 00 14 */	lwz r0, 0x14(r1)
/* 803473A4 003442E4  83 E1 00 0C */	lwz r31, 0xc(r1)
/* 803473A8 003442E8  38 21 00 10 */	addi r1, r1, 0x10
/* 803473AC 003442EC  7C 08 03 A6 */	mtlr r0
/* 803473B0 003442F0  4E 80 00 20 */	blr 

.global "@32@__dt__13TCoasterEnemyFv"
"@32@__dt__13TCoasterEnemyFv":
/* 803473B4 003442F4  38 63 FF E0 */	addi r3, r3, -32
/* 803473B8 003442F8  4B FF E2 3C */	b __dt__13TCoasterEnemyFv

.global "@32@__dt__14TCoasterKillerFv"
"@32@__dt__14TCoasterKillerFv":
/* 803473BC 003442FC  38 63 FF E0 */	addi r3, r3, -32
/* 803473C0 00344300  4B FF FC 5C */	b __dt__14TCoasterKillerFv

.section .ctors, "wa"  # 0x8036FBA0 - 0x8036FF80
lbl_constructor:
  .4byte __sinit_coasterkiller_cpp

.section .rodata, "a"  # 0x8036FFA0 - 0x803A8380
.balign 8
"@1490":
  .4byte 0
  .4byte 0
  .4byte 0
"@2355":
	.incbin "baserom.dol", 0x3A1AF4, 0x14
"@2422":
	.incbin "baserom.dol", 0x3A1B08, 0x30
"@2423":
	.incbin "baserom.dol", 0x3A1B38, 0x38
"@2424":
	.incbin "baserom.dol", 0x3A1B70, 0x34
"@2425":
	.incbin "baserom.dol", 0x3A1BA4, 0x24
"@2839":
	.incbin "baserom.dol", 0x3A1BC8, 0x28
"@2840":
	.incbin "baserom.dol", 0x3A1BF0, 0x28
"@2931":
	.incbin "baserom.dol", 0x3A1C18, 0x14
"@2932":
	.incbin "baserom.dol", 0x3A1C2C, 0x18
"@2953":
	.incbin "baserom.dol", 0x3A1C44, 0x14
"@3036":
	.incbin "baserom.dol", 0x3A1C58, 0x1C
"@3037":
	.incbin "baserom.dol", 0x3A1C74, 0x18
"@3039":
	.incbin "baserom.dol", 0x3A1C8C, 0x18
"@3040":
	.incbin "baserom.dol", 0x3A1CA4, 0x10
"@3125":
	.incbin "baserom.dol", 0x3A1CB4, 0x1C
"@3298":
	.incbin "baserom.dol", 0x3A1CD0, 0xC
"@3299":
	.incbin "baserom.dol", 0x3A1CDC, 0xC

.section .data, "wa"  # 0x803A8380 - 0x803E6000
killer_bastable:
  .4byte "@2839"
  .4byte 0
  .4byte 0
  .4byte "@2840"
  .4byte 0
entry$2930:
  .4byte "@2931"
  .4byte 0x10230000
  .4byte 0
  .4byte "@2932"
  .4byte 0x10210000
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
.global __vt__21TCoasterKillerManager
__vt__21TCoasterKillerManager:
  .4byte 0
  .4byte 0
  .4byte __dt__21TCoasterKillerManagerFv
  .4byte getType__Q26JDrama8TNameRefCFv
  .4byte load__21TCoasterKillerManagerFR20JSUMemoryInputStream
  .4byte save__Q26JDrama8TNameRefFR21JSUMemoryOutputStream
  .4byte loadAfter__21TCoasterKillerManagerFv
  .4byte searchF__11TObjManagerFUsPCc
  .4byte perform__13TEnemyManagerFUlPQ26JDrama9TGraphics
  .4byte createModelData__21TCoasterKillerManagerFv
  .4byte createAnmData__11TObjManagerFv
  .4byte createModelDataArray__11TObjManagerFPC19TModelDataLoadEntry
  .4byte clipActors__12TLiveManagerFPQ26JDrama9TGraphics
  .4byte setFlagOutOfCube__12TLiveManagerFv
  .4byte createSpcBinary__12TLiveManagerFv
  .4byte hasMapCollision__12TLiveManagerCFv
  .4byte createEnemyInstance__21TCoasterKillerManagerFv
  .4byte clipEnemies__13TEnemyManagerFPQ26JDrama9TGraphics
  .4byte restoreDrawBuffer__13TEnemyManagerFUl
  .4byte createEnemies__18TSmallEnemyManagerFi
  .4byte changeDrawBuffer__13TEnemyManagerFUl
  .4byte initSetEnemies__18TSmallEnemyManagerFv
.global __vt__28TNerveCoasterKillerExplosion
__vt__28TNerveCoasterKillerExplosion:
  .4byte 0
  .4byte 0
  .4byte __dt__28TNerveCoasterKillerExplosionFv
  .4byte "execute__28TNerveCoasterKillerExplosionCFP24TSpineBase<10TLiveActor>"
.global __vt__14TCoasterKiller
__vt__14TCoasterKiller:
  .4byte 0
  .4byte 0
  .4byte __dt__14TCoasterKillerFv
  .4byte getType__Q26JDrama6TActorCFv
  .4byte load__11TSmallEnemyFR20JSUMemoryInputStream
  .4byte save__Q26JDrama8TNameRefFR21JSUMemoryOutputStream
  .4byte loadAfter__11TSmallEnemyFv
  .4byte searchF__Q26JDrama8TNameRefFUsPCc
  .4byte perform__14TCoasterKillerFUlPQ26JDrama9TGraphics
  .4byte 0
  .4byte 0
  .4byte "@32@__dt__14TCoasterKillerFv"
  .4byte JSGFGetType__Q26JStage6TActorCFv
  .4byte JSGGetName__Q26JStage7TObjectCFv
  .4byte JSGGetFlag__Q26JStage7TObjectCFv
  .4byte JSGSetFlag__Q26JStage7TObjectFUl
  .4byte JSGGetData__Q26JStage7TObjectCFUlPvUl
  .4byte JSGSetData__Q26JStage7TObjectFUlPCvUl
  .4byte JSGGetParent__Q26JStage7TObjectCFPPQ26JStage7TObjectPUl
  .4byte JSGSetParent__Q26JStage7TObjectFPQ26JStage7TObjectUl
  .4byte JSGSetRelation__Q26JStage7TObjectFbPQ26JStage7TObjectUl
  .4byte "@32@JSGGetTranslation__Q26JDrama6TActorCFP3Vec"
  .4byte "@32@JSGSetTranslation__Q26JDrama6TActorFRC3Vec"
  .4byte "@32@JSGGetScaling__Q26JDrama6TActorCFP3Vec"
  .4byte "@32@JSGSetScaling__Q26JDrama6TActorFRC3Vec"
  .4byte "@32@JSGGetRotation__Q26JDrama6TActorCFP3Vec"
  .4byte "@32@JSGSetRotation__Q26JDrama6TActorFRC3Vec"
  .4byte JSGGetShape__Q26JStage6TActorCFv
  .4byte JSGSetShape__Q26JStage6TActorFUl
  .4byte JSGGetAnimation__Q26JStage6TActorCFv
  .4byte JSGSetAnimation__Q26JStage6TActorFUl
  .4byte JSGGetAnimationFrame__Q26JStage6TActorCFv
  .4byte JSGSetAnimationFrame__Q26JStage6TActorFf
  .4byte JSGGetAnimationFrameMax__Q26JStage6TActorCFv
  .4byte JSGGetTranslation__Q26JDrama6TActorCFP3Vec
  .4byte JSGSetTranslation__Q26JDrama6TActorFRC3Vec
  .4byte JSGGetScaling__Q26JDrama6TActorCFP3Vec
  .4byte JSGSetScaling__Q26JDrama6TActorFRC3Vec
  .4byte JSGGetRotation__Q26JDrama6TActorCFP3Vec
  .4byte JSGSetRotation__Q26JDrama6TActorFRC3Vec
  .4byte receiveMessage__11TSmallEnemyFP9THitActorUl
  .4byte getTakingMtx__10TLiveActorFv
  .4byte ensureTakeSituation__10TTakeActorFv
  .4byte "moveRequest__10TTakeActorFRCQ29JGeometry8TVec3<f>"
  .4byte getRadiusAtY__10TTakeActorCFf
  .4byte belongToGround__10TLiveActorCFv
  .4byte getRootJointMtx__10TLiveActorCFv
  .4byte init__14TCoasterKillerFP12TLiveManager
  .4byte calcRootMatrix__13TCoasterEnemyFv
  .4byte setGroundCollision__10TLiveActorFv
  .4byte control__10TLiveActorFv
  .4byte bind__13TCoasterEnemyFv
  .4byte moveObject__13TCoasterEnemyFv
  .4byte requestShadow__10TLiveActorFv
  .4byte drawObject__10TLiveActorFPQ26JDrama9TGraphics
  .4byte performOnlyDraw__10TLiveActorFUlPQ26JDrama9TGraphics
  .4byte getShadowType__10TLiveActorFv
  .4byte kill__11TSmallEnemyFv
  .4byte getGravityY__13TCoasterEnemyCFv
  .4byte hasMapCollision__10TLiveActorCFv
  .4byte getFocalPoint__10TLiveActorCFv
  .4byte updateAnmSound__11TSmallEnemyFv
  .4byte getBasNameTable__14TCoasterKillerCFv
  .4byte reset__14TCoasterKillerFv
  .4byte "resetToPosition__11TSpineEnemyFRCQ29JGeometry8TVec3<f>"
  .4byte "resetSRTV__11TSpineEnemyFRCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f>"
  .4byte getSaveParam__11TSpineEnemyCFv
  .4byte getPhaseShift__11TSpineEnemyCFv
  .4byte isReachedToGoal__11TSpineEnemyCFv
  .4byte genRandomItem__11TSmallEnemyFv
  .4byte genEventCoin__11TSmallEnemyFv
  .4byte generateItem__11TSmallEnemyFv
  .4byte isEatenByYosshi__11TSmallEnemyFv
  .4byte setBehavior__11TSmallEnemyFv
  .4byte jumpBehavior__11TSmallEnemyFv
  .4byte behaveToWater__14TCoasterKillerFP9THitActor
  .4byte changeByJuice__11TSmallEnemyFv
  .4byte changeMove__11TSmallEnemyFv
  .4byte getChangeBlockTime__11TSmallEnemyFv
  .4byte scalingChangeActor__11TSmallEnemyFv
  .4byte changeOut__11TSmallEnemyFv
  .4byte behaveToTaken__11TSmallEnemyFP9THitActor
  .4byte behaveToRelease__11TSmallEnemyFv
  .4byte setGenerateAnm__11TSmallEnemyFv
  .4byte setWalkAnm__13TCoasterEnemyFv
  .4byte setDeadAnm__14TCoasterKillerFv
  .4byte setFreezeAnm__11TSmallEnemyFv
  .4byte setMeltAnm__11TSmallEnemyFv
  .4byte setWaitAnm__11TSmallEnemyFv
  .4byte setRunAnm__11TSmallEnemyFv
  .4byte attackToMario__14TCoasterKillerFv
  .4byte forceKill__11TSmallEnemyFv
  .4byte setMActorAndKeeper__14TCoasterKillerFv
  .4byte initAttacker__12TWalkerEnemyFP9THitActor
  .4byte isHitValid__11TSmallEnemyFUl
  .4byte isCollidMove__14TCoasterKillerFP9THitActor
  .4byte isInhibitedForceMove__11TSmallEnemyFv
  .4byte endHitWaterJump__11TSmallEnemyFv
  .4byte sendAttackMsgToMario__11TSmallEnemyFv
  .4byte decHpByWater__11TSmallEnemyFP9THitActor
  .4byte setBckAnm__11TSmallEnemyFi
  .4byte setDeadEffect__11TSmallEnemyFv
  .4byte setAfterDeadEffect__11TSmallEnemyFv
  .4byte doKeepDistance__11TSmallEnemyFv
  .4byte generateEffectColumWater__11TSmallEnemyFv
  .4byte isFindMario__11TSmallEnemyFf
  .4byte behaveToFindMario__12TWalkerEnemyFv
  .4byte walkBehavior__12TWalkerEnemyFif
  .4byte isResignationAttack__12TWalkerEnemyFv
  .4byte initialGraphNode__12TWalkerEnemyFv
  .4byte makeCoasterGoalPath__13TCoasterEnemyFv
  .4byte moveCoaster__13TCoasterEnemyFv
  .4byte setNormalFlyAnm__14TCoasterKillerFv
.global __vt__24TNerveCoasterEnemyWander
__vt__24TNerveCoasterEnemyWander:
  .4byte 0
  .4byte 0
  .4byte __dt__24TNerveCoasterEnemyWanderFv
  .4byte "execute__24TNerveCoasterEnemyWanderCFP24TSpineBase<10TLiveActor>"
.global __vt__13TCoasterEnemy
__vt__13TCoasterEnemy:
  .4byte 0
  .4byte 0
  .4byte __dt__13TCoasterEnemyFv
  .4byte getType__Q26JDrama6TActorCFv
  .4byte load__11TSmallEnemyFR20JSUMemoryInputStream
  .4byte save__Q26JDrama8TNameRefFR21JSUMemoryOutputStream
  .4byte loadAfter__11TSmallEnemyFv
  .4byte searchF__Q26JDrama8TNameRefFUsPCc
  .4byte perform__13TCoasterEnemyFUlPQ26JDrama9TGraphics
  .4byte 0
  .4byte 0
  .4byte "@32@__dt__13TCoasterEnemyFv"
  .4byte JSGFGetType__Q26JStage6TActorCFv
  .4byte JSGGetName__Q26JStage7TObjectCFv
  .4byte JSGGetFlag__Q26JStage7TObjectCFv
  .4byte JSGSetFlag__Q26JStage7TObjectFUl
  .4byte JSGGetData__Q26JStage7TObjectCFUlPvUl
  .4byte JSGSetData__Q26JStage7TObjectFUlPCvUl
  .4byte JSGGetParent__Q26JStage7TObjectCFPPQ26JStage7TObjectPUl
  .4byte JSGSetParent__Q26JStage7TObjectFPQ26JStage7TObjectUl
  .4byte JSGSetRelation__Q26JStage7TObjectFbPQ26JStage7TObjectUl
  .4byte "@32@JSGGetTranslation__Q26JDrama6TActorCFP3Vec"
  .4byte "@32@JSGSetTranslation__Q26JDrama6TActorFRC3Vec"
  .4byte "@32@JSGGetScaling__Q26JDrama6TActorCFP3Vec"
  .4byte "@32@JSGSetScaling__Q26JDrama6TActorFRC3Vec"
  .4byte "@32@JSGGetRotation__Q26JDrama6TActorCFP3Vec"
  .4byte "@32@JSGSetRotation__Q26JDrama6TActorFRC3Vec"
  .4byte JSGGetShape__Q26JStage6TActorCFv
  .4byte JSGSetShape__Q26JStage6TActorFUl
  .4byte JSGGetAnimation__Q26JStage6TActorCFv
  .4byte JSGSetAnimation__Q26JStage6TActorFUl
  .4byte JSGGetAnimationFrame__Q26JStage6TActorCFv
  .4byte JSGSetAnimationFrame__Q26JStage6TActorFf
  .4byte JSGGetAnimationFrameMax__Q26JStage6TActorCFv
  .4byte JSGGetTranslation__Q26JDrama6TActorCFP3Vec
  .4byte JSGSetTranslation__Q26JDrama6TActorFRC3Vec
  .4byte JSGGetScaling__Q26JDrama6TActorCFP3Vec
  .4byte JSGSetScaling__Q26JDrama6TActorFRC3Vec
  .4byte JSGGetRotation__Q26JDrama6TActorCFP3Vec
  .4byte JSGSetRotation__Q26JDrama6TActorFRC3Vec
  .4byte receiveMessage__11TSmallEnemyFP9THitActorUl
  .4byte getTakingMtx__10TLiveActorFv
  .4byte ensureTakeSituation__10TTakeActorFv
  .4byte "moveRequest__10TTakeActorFRCQ29JGeometry8TVec3<f>"
  .4byte getRadiusAtY__10TTakeActorCFf
  .4byte belongToGround__10TLiveActorCFv
  .4byte getRootJointMtx__10TLiveActorCFv
  .4byte init__13TCoasterEnemyFP12TLiveManager
  .4byte calcRootMatrix__13TCoasterEnemyFv
  .4byte setGroundCollision__10TLiveActorFv
  .4byte control__10TLiveActorFv
  .4byte bind__13TCoasterEnemyFv
  .4byte moveObject__13TCoasterEnemyFv
  .4byte requestShadow__10TLiveActorFv
  .4byte drawObject__10TLiveActorFPQ26JDrama9TGraphics
  .4byte performOnlyDraw__10TLiveActorFUlPQ26JDrama9TGraphics
  .4byte getShadowType__10TLiveActorFv
  .4byte kill__11TSmallEnemyFv
  .4byte getGravityY__13TCoasterEnemyCFv
  .4byte hasMapCollision__10TLiveActorCFv
  .4byte getFocalPoint__10TLiveActorCFv
  .4byte updateAnmSound__11TSmallEnemyFv
  .4byte getBasNameTable__10TLiveActorCFv
  .4byte reset__13TCoasterEnemyFv
  .4byte "resetToPosition__11TSpineEnemyFRCQ29JGeometry8TVec3<f>"
  .4byte "resetSRTV__11TSpineEnemyFRCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f>"
  .4byte getSaveParam__11TSpineEnemyCFv
  .4byte getPhaseShift__11TSpineEnemyCFv
  .4byte isReachedToGoal__11TSpineEnemyCFv
  .4byte genRandomItem__11TSmallEnemyFv
  .4byte genEventCoin__11TSmallEnemyFv
  .4byte generateItem__11TSmallEnemyFv
  .4byte isEatenByYosshi__11TSmallEnemyFv
  .4byte setBehavior__11TSmallEnemyFv
  .4byte jumpBehavior__11TSmallEnemyFv
  .4byte behaveToWater__11TSmallEnemyFP9THitActor
  .4byte changeByJuice__11TSmallEnemyFv
  .4byte changeMove__11TSmallEnemyFv
  .4byte getChangeBlockTime__11TSmallEnemyFv
  .4byte scalingChangeActor__11TSmallEnemyFv
  .4byte changeOut__11TSmallEnemyFv
  .4byte behaveToTaken__11TSmallEnemyFP9THitActor
  .4byte behaveToRelease__11TSmallEnemyFv
  .4byte setGenerateAnm__11TSmallEnemyFv
  .4byte setWalkAnm__13TCoasterEnemyFv
  .4byte setDeadAnm__11TSmallEnemyFv
  .4byte setFreezeAnm__11TSmallEnemyFv
  .4byte setMeltAnm__11TSmallEnemyFv
  .4byte setWaitAnm__11TSmallEnemyFv
  .4byte setRunAnm__11TSmallEnemyFv
  .4byte attackToMario__12TWalkerEnemyFv
  .4byte forceKill__11TSmallEnemyFv
  .4byte setMActorAndKeeper__11TSmallEnemyFv
  .4byte initAttacker__12TWalkerEnemyFP9THitActor
  .4byte isHitValid__11TSmallEnemyFUl
  .4byte isCollidMove__11TSmallEnemyFP9THitActor
  .4byte isInhibitedForceMove__11TSmallEnemyFv
  .4byte endHitWaterJump__11TSmallEnemyFv
  .4byte sendAttackMsgToMario__11TSmallEnemyFv
  .4byte decHpByWater__11TSmallEnemyFP9THitActor
  .4byte setBckAnm__11TSmallEnemyFi
  .4byte setDeadEffect__11TSmallEnemyFv
  .4byte setAfterDeadEffect__11TSmallEnemyFv
  .4byte doKeepDistance__11TSmallEnemyFv
  .4byte generateEffectColumWater__11TSmallEnemyFv
  .4byte isFindMario__11TSmallEnemyFf
  .4byte behaveToFindMario__12TWalkerEnemyFv
  .4byte walkBehavior__12TWalkerEnemyFif
  .4byte isResignationAttack__12TWalkerEnemyFv
  .4byte initialGraphNode__12TWalkerEnemyFv
  .4byte makeCoasterGoalPath__13TCoasterEnemyFv
  .4byte moveCoaster__13TCoasterEnemyFv
  .4byte setNormalFlyAnm__13TCoasterEnemyFv

.section .sdata, "wa"  # 0x80408AC0 - 0x804097C0
"@2842":
	.incbin "baserom.dol", 0x3E3C50, 0x4
"@2843":
	.incbin "baserom.dol", 0x3E3C54, 0x4

.section .sdata2, "a"  # 0x8040B460 - 0x80414020
.balign 8
"@3038":
	.incbin "baserom.dol", 0x3EBF70, 0x4
"@3041":
	.incbin "baserom.dol", 0x3EBF74, 0x4
"@3103":
  .4byte 0
"@3104":
	.incbin "baserom.dol", 0x3EBF7C, 0x4
"@3105":
	.incbin "baserom.dol", 0x3EBF80, 0x4
"@3126":
	.incbin "baserom.dol", 0x3EBF84, 0x4
"@3300":
	.incbin "baserom.dol", 0x3EBF88, 0x8
"@3340":
	.incbin "baserom.dol", 0x3EBF90, 0x4
"@3341":
	.incbin "baserom.dol", 0x3EBF94, 0x4
"@3359":
	.incbin "baserom.dol", 0x3EBF98, 0x4
"@3398":
	.incbin "baserom.dol", 0x3EBF9C, 0x8
"@3755":
	.incbin "baserom.dol", 0x3EBFA4, 0x4
"@3756":
	.incbin "baserom.dol", 0x3EBFA8, 0x4
"@3757":
	.incbin "baserom.dol", 0x3EBFAC, 0x4

.section .bss, "wa"  # 0x803E6000 - 0x80408AC0
"@2869":
	.skip 0xC
"@2913":
	.skip 0xC
"@3890":
	.skip 0xC
"@3891":
	.skip 0xC
"@3892":
	.skip 0xC
"@3893":
	.skip 0xC
"@3894":
	.skip 0xC
"@3895":
	.skip 0xC
"@3896":
	.skip 0xC
"@3897":
	.skip 0xC
"@3898":
	.skip 0xC
"@3899":
	.skip 0xC
"@3900":
	.skip 0xC
"@3901":
	.skip 0xC
"@3902":
	.skip 0xC
"@3903":
	.skip 0xC
"@3904":
	.skip 0x10

.section .sbss, "wa"  # 0x804097C0 - 0x8040B45C
init$2870:
	.skip 0x4
instance$2868:
	.skip 0x4
init$2914:
	.skip 0x4
instance$2912:
	.skip 0x4
