.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global __dt__11TMewManagerFv
__dt__11TMewManagerFv:
/* 80367334 00364274  7C 08 02 A6 */	mflr r0
/* 80367338 00364278  90 01 00 04 */	stw r0, 4(r1)
/* 8036733C 0036427C  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 80367340 00364280  93 E1 00 14 */	stw r31, 0x14(r1)
/* 80367344 00364284  3B E4 00 00 */	addi r31, r4, 0
/* 80367348 00364288  93 C1 00 10 */	stw r30, 0x10(r1)
/* 8036734C 0036428C  7C 7E 1B 79 */	or. r30, r3, r3
/* 80367350 00364290  41 82 00 3C */	beq lbl_8036738C
/* 80367354 00364294  3C 60 80 3E */	lis r3, __vt__11TMewManager@ha
/* 80367358 00364298  38 03 58 68 */	addi r0, r3, __vt__11TMewManager@l
/* 8036735C 0036429C  90 1E 00 00 */	stw r0, 0(r30)
/* 80367360 003642A0  41 82 00 1C */	beq lbl_8036737C
/* 80367364 003642A4  3C 60 80 3E */	lis r3, __vt__18TAnimalManagerBase@ha
/* 80367368 003642A8  38 03 58 BC */	addi r0, r3, __vt__18TAnimalManagerBase@l
/* 8036736C 003642AC  90 1E 00 00 */	stw r0, 0(r30)
/* 80367370 003642B0  38 7E 00 00 */	addi r3, r30, 0
/* 80367374 003642B4  38 80 00 00 */	li r4, 0
/* 80367378 003642B8  4B EE B6 31 */	bl __dt__13TEnemyManagerFv
lbl_8036737C:
/* 8036737C 003642BC  7F E0 07 35 */	extsh. r0, r31
/* 80367380 003642C0  40 81 00 0C */	ble lbl_8036738C
/* 80367384 003642C4  7F C3 F3 78 */	mr r3, r30
/* 80367388 003642C8  4B CA 57 29 */	bl __dl__FPv
lbl_8036738C:
/* 8036738C 003642CC  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 80367390 003642D0  7F C3 F3 78 */	mr r3, r30
/* 80367394 003642D4  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 80367398 003642D8  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 8036739C 003642DC  7C 08 03 A6 */	mtlr r0
/* 803673A0 003642E0  38 21 00 18 */	addi r1, r1, 0x18
/* 803673A4 003642E4  4E 80 00 20 */	blr 

.global createModelData__11TMewManagerFv
createModelData__11TMewManagerFv:
/* 803673A8 003642E8  7C 08 02 A6 */	mflr r0
/* 803673AC 003642EC  3C 80 80 3A */	lis r4, entry$2542@ha
/* 803673B0 003642F0  90 01 00 04 */	stw r0, 4(r1)
/* 803673B4 003642F4  38 84 7C 10 */	addi r4, r4, entry$2542@l
/* 803673B8 003642F8  94 21 FF F8 */	stwu r1, -8(r1)
/* 803673BC 003642FC  81 83 00 00 */	lwz r12, 0(r3)
/* 803673C0 00364300  81 8C 00 2C */	lwz r12, 0x2c(r12)
/* 803673C4 00364304  7D 88 03 A6 */	mtlr r12
/* 803673C8 00364308  4E 80 00 21 */	blrl 
/* 803673CC 0036430C  80 01 00 0C */	lwz r0, 0xc(r1)
/* 803673D0 00364310  38 21 00 08 */	addi r1, r1, 8
/* 803673D4 00364314  7C 08 03 A6 */	mtlr r0
/* 803673D8 00364318  4E 80 00 20 */	blr 

.global loadAfter__11TMewManagerFv
loadAfter__11TMewManagerFv:
/* 803673DC 0036431C  7C 08 02 A6 */	mflr r0
/* 803673E0 00364320  90 01 00 04 */	stw r0, 4(r1)
/* 803673E4 00364324  94 21 FF D8 */	stwu r1, -0x28(r1)
/* 803673E8 00364328  93 E1 00 24 */	stw r31, 0x24(r1)
/* 803673EC 0036432C  7C 7F 1B 78 */	mr r31, r3
/* 803673F0 00364330  4B CD D4 BD */	bl loadAfter__Q26JDrama8TNameRefFv
/* 803673F4 00364334  80 1F 00 14 */	lwz r0, 0x14(r31)
/* 803673F8 00364338  38 60 38 13 */	li r3, 0x3813
/* 803673FC 0036433C  54 04 04 3E */	clrlwi r4, r0, 0x10
/* 80367400 00364340  4B E1 C3 F9 */	bl createRandPlayVec__Q214MSoundSESystem10MSRandPlayFUlUs
/* 80367404 00364344  80 01 00 2C */	lwz r0, 0x2c(r1)
/* 80367408 00364348  83 E1 00 24 */	lwz r31, 0x24(r1)
/* 8036740C 0036434C  38 21 00 28 */	addi r1, r1, 0x28
/* 80367410 00364350  7C 08 03 A6 */	mtlr r0
/* 80367414 00364354  4E 80 00 20 */	blr 

.global load__11TMewManagerFR20JSUMemoryInputStream
load__11TMewManagerFR20JSUMemoryInputStream:
/* 80367418 00364358  7C 08 02 A6 */	mflr r0
/* 8036741C 0036435C  90 01 00 04 */	stw r0, 4(r1)
/* 80367420 00364360  94 21 FF C0 */	stwu r1, -0x40(r1)
/* 80367424 00364364  93 E1 00 3C */	stw r31, 0x3c(r1)
/* 80367428 00364368  7C 7F 1B 78 */	mr r31, r3
/* 8036742C 0036436C  93 C1 00 38 */	stw r30, 0x38(r1)
/* 80367430 00364370  4B EE B3 D5 */	bl load__13TEnemyManagerFR20JSUMemoryInputStream
/* 80367434 00364374  38 60 00 BC */	li r3, 0xbc
/* 80367438 00364378  4B CA 54 79 */	bl __nw__FUl
/* 8036743C 0036437C  7C 7E 1B 79 */	or. r30, r3, r3
/* 80367440 00364380  41 82 00 14 */	beq lbl_80367454
/* 80367444 00364384  3C 60 80 3A */	lis r3, $$22573@ha
/* 80367448 00364388  38 83 7C 28 */	addi r4, r3, $$22573@l
/* 8036744C 0036438C  38 7E 00 00 */	addi r3, r30, 0
/* 80367450 00364390  48 00 04 91 */	bl __ct__21TAnimalSaveIndividualFPCc
lbl_80367454:
/* 80367454 00364394  93 DF 00 5C */	stw r30, 0x5c(r31)
/* 80367458 00364398  80 7F 00 5C */	lwz r3, 0x5c(r31)
/* 8036745C 0036439C  C0 03 00 2C */	lfs f0, 0x2c(r3)
/* 80367460 003643A0  D0 1F 00 54 */	stfs f0, 0x54(r31)
/* 80367464 003643A4  80 7F 00 5C */	lwz r3, 0x5c(r31)
/* 80367468 003643A8  38 03 00 40 */	addi r0, r3, 0x40
/* 8036746C 003643AC  90 1F 00 58 */	stw r0, 0x58(r31)
/* 80367470 003643B0  80 7F 00 5C */	lwz r3, 0x5c(r31)
/* 80367474 003643B4  C0 03 00 54 */	lfs f0, 0x54(r3)
/* 80367478 003643B8  D0 1F 00 3C */	stfs f0, 0x3c(r31)
/* 8036747C 003643BC  80 01 00 44 */	lwz r0, 0x44(r1)
/* 80367480 003643C0  83 E1 00 3C */	lwz r31, 0x3c(r1)
/* 80367484 003643C4  83 C1 00 38 */	lwz r30, 0x38(r1)
/* 80367488 003643C8  38 21 00 40 */	addi r1, r1, 0x40
/* 8036748C 003643CC  7C 08 03 A6 */	mtlr r0
/* 80367490 003643D0  4E 80 00 20 */	blr 

.global clipEnemies__18TAnimalManagerBaseFPQ26JDrama9TGraphics
clipEnemies__18TAnimalManagerBaseFPQ26JDrama9TGraphics:
/* 80367494 003643D4  7C 08 02 A6 */	mflr r0
/* 80367498 003643D8  90 01 00 04 */	stw r0, 4(r1)
/* 8036749C 003643DC  94 21 FF A8 */	stwu r1, -0x58(r1)
/* 803674A0 003643E0  DB E1 00 50 */	stfd f31, 0x50(r1)
/* 803674A4 003643E4  BF 41 00 38 */	stmw r26, 0x38(r1)
/* 803674A8 003643E8  7C 7B 1B 78 */	mr r27, r3
/* 803674AC 003643EC  7C 9C 23 78 */	mr r28, r4
/* 803674B0 003643F0  80 AD A8 B0 */	lwz r5, gpCamera-_SDA_BASE_(r13)
/* 803674B4 003643F4  C0 63 00 54 */	lfs f3, 0x54(r3)
/* 803674B8 003643F8  80 63 00 58 */	lwz r3, 0x58(r3)
/* 803674BC 003643FC  C0 45 00 4C */	lfs f2, 0x4c(r5)
/* 803674C0 00364400  C0 25 00 48 */	lfs f1, 0x48(r5)
/* 803674C4 00364404  C0 83 00 00 */	lfs f4, 0(r3)
/* 803674C8 00364408  4B D5 9F 4D */	bl SetViewFrustumClipCheckPerspective__Fffff
/* 803674CC 0036440C  83 FB 00 14 */	lwz r31, 0x14(r27)
/* 803674D0 00364410  3B A0 00 00 */	li r29, 0
/* 803674D4 00364414  C3 E2 09 F0 */	lfs f31, $$22600-_SDA2_BASE_(r2)
/* 803674D8 00364418  3B 40 00 00 */	li r26, 0
/* 803674DC 0036441C  48 00 00 98 */	b lbl_80367574
lbl_803674E0:
/* 803674E0 00364420  80 7B 00 18 */	lwz r3, 0x18(r27)
/* 803674E4 00364424  7F C3 D0 2E */	lwzx r30, r3, r26
/* 803674E8 00364428  80 7E 00 10 */	lwz r3, 0x10(r30)
/* 803674EC 0036442C  80 1E 00 14 */	lwz r0, 0x14(r30)
/* 803674F0 00364430  90 61 00 24 */	stw r3, 0x24(r1)
/* 803674F4 00364434  90 01 00 28 */	stw r0, 0x28(r1)
/* 803674F8 00364438  80 1E 00 18 */	lwz r0, 0x18(r30)
/* 803674FC 0036443C  90 01 00 2C */	stw r0, 0x2c(r1)
/* 80367500 00364440  C0 01 00 28 */	lfs f0, 0x28(r1)
/* 80367504 00364444  EC 00 F8 2A */	fadds f0, f0, f31
/* 80367508 00364448  D0 01 00 28 */	stfs f0, 0x28(r1)
/* 8036750C 0036444C  80 1E 00 F0 */	lwz r0, 0xf0(r30)
/* 80367510 00364450  54 00 04 A5 */	rlwinm. r0, r0, 0, 0x12, 0x12
/* 80367514 00364454  41 82 00 24 */	beq lbl_80367538
/* 80367518 00364458  38 61 00 24 */	addi r3, r1, 0x24
/* 8036751C 0036445C  4B FF 7E 5D */	bl SMS_IsInOtherFastCube__FRC3Vec
/* 80367520 00364460  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 80367524 00364464  41 82 00 14 */	beq lbl_80367538
/* 80367528 00364468  80 1E 00 F0 */	lwz r0, 0xf0(r30)
/* 8036752C 0036446C  60 00 00 04 */	ori r0, r0, 4
/* 80367530 00364470  90 1E 00 F0 */	stw r0, 0xf0(r30)
/* 80367534 00364474  48 00 00 38 */	b lbl_8036756C
lbl_80367538:
/* 80367538 00364478  C0 3B 00 3C */	lfs f1, 0x3c(r27)
/* 8036753C 0036447C  38 7C 00 00 */	addi r3, r28, 0
/* 80367540 00364480  38 9E 00 10 */	addi r4, r30, 0x10
/* 80367544 00364484  4B D5 9D 61 */	bl ViewFrustumClipCheck__FPQ26JDrama9TGraphicsP3Vecf
/* 80367548 00364488  2C 03 00 00 */	cmpwi r3, 0
/* 8036754C 0036448C  41 82 00 14 */	beq lbl_80367560
/* 80367550 00364490  80 1E 00 F0 */	lwz r0, 0xf0(r30)
/* 80367554 00364494  54 00 07 B8 */	rlwinm r0, r0, 0, 0x1e, 0x1c
/* 80367558 00364498  90 1E 00 F0 */	stw r0, 0xf0(r30)
/* 8036755C 0036449C  48 00 00 10 */	b lbl_8036756C
lbl_80367560:
/* 80367560 003644A0  80 1E 00 F0 */	lwz r0, 0xf0(r30)
/* 80367564 003644A4  60 00 00 04 */	ori r0, r0, 4
/* 80367568 003644A8  90 1E 00 F0 */	stw r0, 0xf0(r30)
lbl_8036756C:
/* 8036756C 003644AC  3B BD 00 01 */	addi r29, r29, 1
/* 80367570 003644B0  3B 5A 00 04 */	addi r26, r26, 4
lbl_80367574:
/* 80367574 003644B4  7C 1D F8 00 */	cmpw r29, r31
/* 80367578 003644B8  41 80 FF 68 */	blt lbl_803674E0
/* 8036757C 003644BC  BB 41 00 38 */	lmw r26, 0x38(r1)
/* 80367580 003644C0  80 01 00 5C */	lwz r0, 0x5c(r1)
/* 80367584 003644C4  CB E1 00 50 */	lfd f31, 0x50(r1)
/* 80367588 003644C8  38 21 00 58 */	addi r1, r1, 0x58
/* 8036758C 003644CC  7C 08 03 A6 */	mtlr r0
/* 80367590 003644D0  4E 80 00 20 */	blr 

.global __ct__18TAnimalManagerBaseFPCc
__ct__18TAnimalManagerBaseFPCc:
/* 80367594 003644D4  7C 08 02 A6 */	mflr r0
/* 80367598 003644D8  90 01 00 04 */	stw r0, 4(r1)
/* 8036759C 003644DC  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 803675A0 003644E0  93 E1 00 14 */	stw r31, 0x14(r1)
/* 803675A4 003644E4  7C 7F 1B 78 */	mr r31, r3
/* 803675A8 003644E8  4B EE B4 95 */	bl __ct__13TEnemyManagerFPCc
/* 803675AC 003644EC  3C 60 80 3E */	lis r3, __vt__18TAnimalManagerBase@ha
/* 803675B0 003644F0  38 03 58 BC */	addi r0, r3, __vt__18TAnimalManagerBase@l
/* 803675B4 003644F4  90 1F 00 00 */	stw r0, 0(r31)
/* 803675B8 003644F8  38 00 00 00 */	li r0, 0
/* 803675BC 003644FC  38 7F 00 00 */	addi r3, r31, 0
/* 803675C0 00364500  C0 02 09 F4 */	lfs f0, $$22613-_SDA2_BASE_(r2)
/* 803675C4 00364504  D0 1F 00 54 */	stfs f0, 0x54(r31)
/* 803675C8 00364508  90 1F 00 58 */	stw r0, 0x58(r31)
/* 803675CC 0036450C  90 1F 00 5C */	stw r0, 0x5c(r31)
/* 803675D0 00364510  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 803675D4 00364514  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 803675D8 00364518  38 21 00 18 */	addi r1, r1, 0x18
/* 803675DC 0036451C  7C 08 03 A6 */	mtlr r0
/* 803675E0 00364520  4E 80 00 20 */	blr 

.global __sinit_AnimalManager_cpp
__sinit_AnimalManager_cpp:
/* 803675E4 00364524  7C 08 02 A6 */	mflr r0
/* 803675E8 00364528  3C 60 80 40 */	lis r3, $$22616@ha
/* 803675EC 0036452C  90 01 00 04 */	stw r0, 4(r1)
/* 803675F0 00364530  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 803675F4 00364534  93 E1 00 0C */	stw r31, 0xc(r1)
/* 803675F8 00364538  3B E3 08 98 */	addi r31, r3, $$22616@l
/* 803675FC 0036453C  88 0D 97 CC */	lbz r0, __init__smList__15JALList$$05MSBgm$$1-_SDA_BASE_(r13)
/* 80367600 00364540  7C 00 07 75 */	extsb. r0, r0
/* 80367604 00364544  40 82 00 28 */	bne lbl_8036762C
/* 80367608 00364548  38 6D 97 A8 */	addi r3, r13, smList__15JALList$$05MSBgm$$1-_SDA_BASE_
/* 8036760C 0036454C  4B CA 76 29 */	bl initiate__10JSUPtrListFv
/* 80367610 00364550  3C 60 80 0E */	lis r3, __dt__15JSUList$$05MSBgm$$1Fv@ha
/* 80367614 00364554  38 83 6A 44 */	addi r4, r3, __dt__15JSUList$$05MSBgm$$1Fv@l
/* 80367618 00364558  38 6D 97 A8 */	addi r3, r13, smList__15JALList$$05MSBgm$$1-_SDA_BASE_
/* 8036761C 0036455C  38 BF 00 00 */	addi r5, r31, 0
/* 80367620 00364560  4B D1 B1 09 */	bl __register_global_object
/* 80367624 00364564  38 00 00 01 */	li r0, 1
/* 80367628 00364568  98 0D 97 CC */	stb r0, __init__smList__15JALList$$05MSBgm$$1-_SDA_BASE_(r13)
lbl_8036762C:
/* 8036762C 0036456C  88 0D 97 CD */	lbz r0, __init__smList__24JALList$$013MSSetSoundGrp$$1-_SDA_BASE_(r13)
/* 80367630 00364570  7C 00 07 75 */	extsb. r0, r0
/* 80367634 00364574  40 82 00 28 */	bne lbl_8036765C
/* 80367638 00364578  38 6D 97 B4 */	addi r3, r13, smList__24JALList$$013MSSetSoundGrp$$1-_SDA_BASE_
/* 8036763C 0036457C  4B CA 75 F9 */	bl initiate__10JSUPtrListFv
/* 80367640 00364580  3C 60 80 0E */	lis r3, __dt__24JSUList$$013MSSetSoundGrp$$1Fv@ha
/* 80367644 00364584  38 83 69 EC */	addi r4, r3, __dt__24JSUList$$013MSSetSoundGrp$$1Fv@l
/* 80367648 00364588  38 6D 97 B4 */	addi r3, r13, smList__24JALList$$013MSSetSoundGrp$$1-_SDA_BASE_
/* 8036764C 0036458C  38 BF 00 0C */	addi r5, r31, 0xc
/* 80367650 00364590  4B D1 B0 D9 */	bl __register_global_object
/* 80367654 00364594  38 00 00 01 */	li r0, 1
/* 80367658 00364598  98 0D 97 CD */	stb r0, __init__smList__24JALList$$013MSSetSoundGrp$$1-_SDA_BASE_(r13)
lbl_8036765C:
/* 8036765C 0036459C  88 0D 97 CE */	lbz r0, __init__smList__21JALList$$010MSSetSound$$1-_SDA_BASE_(r13)
/* 80367660 003645A0  7C 00 07 75 */	extsb. r0, r0
/* 80367664 003645A4  40 82 00 28 */	bne lbl_8036768C
/* 80367668 003645A8  38 6D 97 C0 */	addi r3, r13, smList__21JALList$$010MSSetSound$$1-_SDA_BASE_
/* 8036766C 003645AC  4B CA 75 C9 */	bl initiate__10JSUPtrListFv
/* 80367670 003645B0  3C 60 80 0E */	lis r3, __dt__21JSUList$$010MSSetSound$$1Fv@ha
/* 80367674 003645B4  38 83 69 94 */	addi r4, r3, __dt__21JSUList$$010MSSetSound$$1Fv@l
/* 80367678 003645B8  38 6D 97 C0 */	addi r3, r13, smList__21JALList$$010MSSetSound$$1-_SDA_BASE_
/* 8036767C 003645BC  38 BF 00 18 */	addi r5, r31, 0x18
/* 80367680 003645C0  4B D1 B0 A9 */	bl __register_global_object
/* 80367684 003645C4  38 00 00 01 */	li r0, 1
/* 80367688 003645C8  98 0D 97 CE */	stb r0, __init__smList__21JALList$$010MSSetSound$$1-_SDA_BASE_(r13)
lbl_8036768C:
/* 8036768C 003645CC  88 0D 8F 8C */	lbz r0, __init__smList__26JALList$$015JALSeModEffDGrp$$1-_SDA_BASE_(r13)
/* 80367690 003645D0  7C 00 07 75 */	extsb. r0, r0
/* 80367694 003645D4  40 82 00 28 */	bne lbl_803676BC
/* 80367698 003645D8  38 6D 8E FC */	addi r3, r13, smList__26JALList$$015JALSeModEffDGrp$$1-_SDA_BASE_
/* 8036769C 003645DC  4B CA 75 99 */	bl initiate__10JSUPtrListFv
/* 803676A0 003645E0  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffDGrp$$1Fv@ha
/* 803676A4 003645E4  38 83 A8 0C */	addi r4, r3, __dt__26JSUList$$015JALSeModEffDGrp$$1Fv@l
/* 803676A8 003645E8  38 6D 8E FC */	addi r3, r13, smList__26JALList$$015JALSeModEffDGrp$$1-_SDA_BASE_
/* 803676AC 003645EC  38 BF 00 24 */	addi r5, r31, 0x24
/* 803676B0 003645F0  4B D1 B0 79 */	bl __register_global_object
/* 803676B4 003645F4  38 00 00 01 */	li r0, 1
/* 803676B8 003645F8  98 0D 8F 8C */	stb r0, __init__smList__26JALList$$015JALSeModEffDGrp$$1-_SDA_BASE_(r13)
lbl_803676BC:
/* 803676BC 003645FC  88 0D 8F 8D */	lbz r0, __init__smList__26JALList$$015JALSeModPitDGrp$$1-_SDA_BASE_(r13)
/* 803676C0 00364600  7C 00 07 75 */	extsb. r0, r0
/* 803676C4 00364604  40 82 00 28 */	bne lbl_803676EC
/* 803676C8 00364608  38 6D 8F 08 */	addi r3, r13, smList__26JALList$$015JALSeModPitDGrp$$1-_SDA_BASE_
/* 803676CC 0036460C  4B CA 75 69 */	bl initiate__10JSUPtrListFv
/* 803676D0 00364610  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitDGrp$$1Fv@ha
/* 803676D4 00364614  38 83 A7 B4 */	addi r4, r3, __dt__26JSUList$$015JALSeModPitDGrp$$1Fv@l
/* 803676D8 00364618  38 6D 8F 08 */	addi r3, r13, smList__26JALList$$015JALSeModPitDGrp$$1-_SDA_BASE_
/* 803676DC 0036461C  38 BF 00 30 */	addi r5, r31, 0x30
/* 803676E0 00364620  4B D1 B0 49 */	bl __register_global_object
/* 803676E4 00364624  38 00 00 01 */	li r0, 1
/* 803676E8 00364628  98 0D 8F 8D */	stb r0, __init__smList__26JALList$$015JALSeModPitDGrp$$1-_SDA_BASE_(r13)
lbl_803676EC:
/* 803676EC 0036462C  88 0D 8F 8E */	lbz r0, __init__smList__26JALList$$015JALSeModVolDGrp$$1-_SDA_BASE_(r13)
/* 803676F0 00364630  7C 00 07 75 */	extsb. r0, r0
/* 803676F4 00364634  40 82 00 28 */	bne lbl_8036771C
/* 803676F8 00364638  38 6D 8F 14 */	addi r3, r13, smList__26JALList$$015JALSeModVolDGrp$$1-_SDA_BASE_
/* 803676FC 0036463C  4B CA 75 39 */	bl initiate__10JSUPtrListFv
/* 80367700 00364640  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolDGrp$$1Fv@ha
/* 80367704 00364644  38 83 A7 5C */	addi r4, r3, __dt__26JSUList$$015JALSeModVolDGrp$$1Fv@l
/* 80367708 00364648  38 6D 8F 14 */	addi r3, r13, smList__26JALList$$015JALSeModVolDGrp$$1-_SDA_BASE_
/* 8036770C 0036464C  38 BF 00 3C */	addi r5, r31, 0x3c
/* 80367710 00364650  4B D1 B0 19 */	bl __register_global_object
/* 80367714 00364654  38 00 00 01 */	li r0, 1
/* 80367718 00364658  98 0D 8F 8E */	stb r0, __init__smList__26JALList$$015JALSeModVolDGrp$$1-_SDA_BASE_(r13)
lbl_8036771C:
/* 8036771C 0036465C  88 0D 8F 8F */	lbz r0, __init__smList__26JALList$$015JALSeModEffFGrp$$1-_SDA_BASE_(r13)
/* 80367720 00364660  7C 00 07 75 */	extsb. r0, r0
/* 80367724 00364664  40 82 00 28 */	bne lbl_8036774C
/* 80367728 00364668  38 6D 8F 20 */	addi r3, r13, smList__26JALList$$015JALSeModEffFGrp$$1-_SDA_BASE_
/* 8036772C 0036466C  4B CA 75 09 */	bl initiate__10JSUPtrListFv
/* 80367730 00364670  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffFGrp$$1Fv@ha
/* 80367734 00364674  38 83 A7 04 */	addi r4, r3, __dt__26JSUList$$015JALSeModEffFGrp$$1Fv@l
/* 80367738 00364678  38 6D 8F 20 */	addi r3, r13, smList__26JALList$$015JALSeModEffFGrp$$1-_SDA_BASE_
/* 8036773C 0036467C  38 BF 00 48 */	addi r5, r31, 0x48
/* 80367740 00364680  4B D1 AF E9 */	bl __register_global_object
/* 80367744 00364684  38 00 00 01 */	li r0, 1
/* 80367748 00364688  98 0D 8F 8F */	stb r0, __init__smList__26JALList$$015JALSeModEffFGrp$$1-_SDA_BASE_(r13)
lbl_8036774C:
/* 8036774C 0036468C  88 0D 8F 90 */	lbz r0, __init__smList__26JALList$$015JALSeModPitFGrp$$1-_SDA_BASE_(r13)
/* 80367750 00364690  7C 00 07 75 */	extsb. r0, r0
/* 80367754 00364694  40 82 00 28 */	bne lbl_8036777C
/* 80367758 00364698  38 6D 8F 2C */	addi r3, r13, smList__26JALList$$015JALSeModPitFGrp$$1-_SDA_BASE_
/* 8036775C 0036469C  4B CA 74 D9 */	bl initiate__10JSUPtrListFv
/* 80367760 003646A0  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitFGrp$$1Fv@ha
/* 80367764 003646A4  38 83 A6 AC */	addi r4, r3, __dt__26JSUList$$015JALSeModPitFGrp$$1Fv@l
/* 80367768 003646A8  38 6D 8F 2C */	addi r3, r13, smList__26JALList$$015JALSeModPitFGrp$$1-_SDA_BASE_
/* 8036776C 003646AC  38 BF 00 54 */	addi r5, r31, 0x54
/* 80367770 003646B0  4B D1 AF B9 */	bl __register_global_object
/* 80367774 003646B4  38 00 00 01 */	li r0, 1
/* 80367778 003646B8  98 0D 8F 90 */	stb r0, __init__smList__26JALList$$015JALSeModPitFGrp$$1-_SDA_BASE_(r13)
lbl_8036777C:
/* 8036777C 003646BC  88 0D 8F 91 */	lbz r0, __init__smList__26JALList$$015JALSeModVolFGrp$$1-_SDA_BASE_(r13)
/* 80367780 003646C0  7C 00 07 75 */	extsb. r0, r0
/* 80367784 003646C4  40 82 00 28 */	bne lbl_803677AC
/* 80367788 003646C8  38 6D 8F 38 */	addi r3, r13, smList__26JALList$$015JALSeModVolFGrp$$1-_SDA_BASE_
/* 8036778C 003646CC  4B CA 74 A9 */	bl initiate__10JSUPtrListFv
/* 80367790 003646D0  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolFGrp$$1Fv@ha
/* 80367794 003646D4  38 83 A6 54 */	addi r4, r3, __dt__26JSUList$$015JALSeModVolFGrp$$1Fv@l
/* 80367798 003646D8  38 6D 8F 38 */	addi r3, r13, smList__26JALList$$015JALSeModVolFGrp$$1-_SDA_BASE_
/* 8036779C 003646DC  38 BF 00 60 */	addi r5, r31, 0x60
/* 803677A0 003646E0  4B D1 AF 89 */	bl __register_global_object
/* 803677A4 003646E4  38 00 00 01 */	li r0, 1
/* 803677A8 003646E8  98 0D 8F 91 */	stb r0, __init__smList__26JALList$$015JALSeModVolFGrp$$1-_SDA_BASE_(r13)
lbl_803677AC:
/* 803677AC 003646EC  88 0D 8F 92 */	lbz r0, __init__smList__26JALList$$015JALSeModEffDist$$1-_SDA_BASE_(r13)
/* 803677B0 003646F0  7C 00 07 75 */	extsb. r0, r0
/* 803677B4 003646F4  40 82 00 28 */	bne lbl_803677DC
/* 803677B8 003646F8  38 6D 8F 44 */	addi r3, r13, smList__26JALList$$015JALSeModEffDist$$1-_SDA_BASE_
/* 803677BC 003646FC  4B CA 74 79 */	bl initiate__10JSUPtrListFv
/* 803677C0 00364700  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffDist$$1Fv@ha
/* 803677C4 00364704  38 83 A5 FC */	addi r4, r3, __dt__26JSUList$$015JALSeModEffDist$$1Fv@l
/* 803677C8 00364708  38 6D 8F 44 */	addi r3, r13, smList__26JALList$$015JALSeModEffDist$$1-_SDA_BASE_
/* 803677CC 0036470C  38 BF 00 6C */	addi r5, r31, 0x6c
/* 803677D0 00364710  4B D1 AF 59 */	bl __register_global_object
/* 803677D4 00364714  38 00 00 01 */	li r0, 1
/* 803677D8 00364718  98 0D 8F 92 */	stb r0, __init__smList__26JALList$$015JALSeModEffDist$$1-_SDA_BASE_(r13)
lbl_803677DC:
/* 803677DC 0036471C  88 0D 8F 93 */	lbz r0, __init__smList__26JALList$$015JALSeModPitDist$$1-_SDA_BASE_(r13)
/* 803677E0 00364720  7C 00 07 75 */	extsb. r0, r0
/* 803677E4 00364724  40 82 00 28 */	bne lbl_8036780C
/* 803677E8 00364728  38 6D 8F 50 */	addi r3, r13, smList__26JALList$$015JALSeModPitDist$$1-_SDA_BASE_
/* 803677EC 0036472C  4B CA 74 49 */	bl initiate__10JSUPtrListFv
/* 803677F0 00364730  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitDist$$1Fv@ha
/* 803677F4 00364734  38 83 A5 A4 */	addi r4, r3, __dt__26JSUList$$015JALSeModPitDist$$1Fv@l
/* 803677F8 00364738  38 6D 8F 50 */	addi r3, r13, smList__26JALList$$015JALSeModPitDist$$1-_SDA_BASE_
/* 803677FC 0036473C  38 BF 00 78 */	addi r5, r31, 0x78
/* 80367800 00364740  4B D1 AF 29 */	bl __register_global_object
/* 80367804 00364744  38 00 00 01 */	li r0, 1
/* 80367808 00364748  98 0D 8F 93 */	stb r0, __init__smList__26JALList$$015JALSeModPitDist$$1-_SDA_BASE_(r13)
lbl_8036780C:
/* 8036780C 0036474C  88 0D 8F 94 */	lbz r0, __init__smList__26JALList$$015JALSeModVolDist$$1-_SDA_BASE_(r13)
/* 80367810 00364750  7C 00 07 75 */	extsb. r0, r0
/* 80367814 00364754  40 82 00 28 */	bne lbl_8036783C
/* 80367818 00364758  38 6D 8F 5C */	addi r3, r13, smList__26JALList$$015JALSeModVolDist$$1-_SDA_BASE_
/* 8036781C 0036475C  4B CA 74 19 */	bl initiate__10JSUPtrListFv
/* 80367820 00364760  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolDist$$1Fv@ha
/* 80367824 00364764  38 83 A5 4C */	addi r4, r3, __dt__26JSUList$$015JALSeModVolDist$$1Fv@l
/* 80367828 00364768  38 6D 8F 5C */	addi r3, r13, smList__26JALList$$015JALSeModVolDist$$1-_SDA_BASE_
/* 8036782C 0036476C  38 BF 00 84 */	addi r5, r31, 0x84
/* 80367830 00364770  4B D1 AE F9 */	bl __register_global_object
/* 80367834 00364774  38 00 00 01 */	li r0, 1
/* 80367838 00364778  98 0D 8F 94 */	stb r0, __init__smList__26JALList$$015JALSeModVolDist$$1-_SDA_BASE_(r13)
lbl_8036783C:
/* 8036783C 0036477C  88 0D 8F 95 */	lbz r0, __init__smList__26JALList$$015JALSeModEffFunk$$1-_SDA_BASE_(r13)
/* 80367840 00364780  7C 00 07 75 */	extsb. r0, r0
/* 80367844 00364784  40 82 00 28 */	bne lbl_8036786C
/* 80367848 00364788  38 6D 8F 68 */	addi r3, r13, smList__26JALList$$015JALSeModEffFunk$$1-_SDA_BASE_
/* 8036784C 0036478C  4B CA 73 E9 */	bl initiate__10JSUPtrListFv
/* 80367850 00364790  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffFunk$$1Fv@ha
/* 80367854 00364794  38 83 A4 F4 */	addi r4, r3, __dt__26JSUList$$015JALSeModEffFunk$$1Fv@l
/* 80367858 00364798  38 6D 8F 68 */	addi r3, r13, smList__26JALList$$015JALSeModEffFunk$$1-_SDA_BASE_
/* 8036785C 0036479C  38 BF 00 90 */	addi r5, r31, 0x90
/* 80367860 003647A0  4B D1 AE C9 */	bl __register_global_object
/* 80367864 003647A4  38 00 00 01 */	li r0, 1
/* 80367868 003647A8  98 0D 8F 95 */	stb r0, __init__smList__26JALList$$015JALSeModEffFunk$$1-_SDA_BASE_(r13)
lbl_8036786C:
/* 8036786C 003647AC  88 0D 8F 96 */	lbz r0, __init__smList__26JALList$$015JALSeModPitFunk$$1-_SDA_BASE_(r13)
/* 80367870 003647B0  7C 00 07 75 */	extsb. r0, r0
/* 80367874 003647B4  40 82 00 28 */	bne lbl_8036789C
/* 80367878 003647B8  38 6D 8F 74 */	addi r3, r13, smList__26JALList$$015JALSeModPitFunk$$1-_SDA_BASE_
/* 8036787C 003647BC  4B CA 73 B9 */	bl initiate__10JSUPtrListFv
/* 80367880 003647C0  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitFunk$$1Fv@ha
/* 80367884 003647C4  38 83 A4 9C */	addi r4, r3, __dt__26JSUList$$015JALSeModPitFunk$$1Fv@l
/* 80367888 003647C8  38 6D 8F 74 */	addi r3, r13, smList__26JALList$$015JALSeModPitFunk$$1-_SDA_BASE_
/* 8036788C 003647CC  38 BF 00 9C */	addi r5, r31, 0x9c
/* 80367890 003647D0  4B D1 AE 99 */	bl __register_global_object
/* 80367894 003647D4  38 00 00 01 */	li r0, 1
/* 80367898 003647D8  98 0D 8F 96 */	stb r0, __init__smList__26JALList$$015JALSeModPitFunk$$1-_SDA_BASE_(r13)
lbl_8036789C:
/* 8036789C 003647DC  88 0D 8F 97 */	lbz r0, __init__smList__26JALList$$015JALSeModVolFunk$$1-_SDA_BASE_(r13)
/* 803678A0 003647E0  7C 00 07 75 */	extsb. r0, r0
/* 803678A4 003647E4  40 82 00 28 */	bne lbl_803678CC
/* 803678A8 003647E8  38 6D 8F 80 */	addi r3, r13, smList__26JALList$$015JALSeModVolFunk$$1-_SDA_BASE_
/* 803678AC 003647EC  4B CA 73 89 */	bl initiate__10JSUPtrListFv
/* 803678B0 003647F0  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolFunk$$1Fv@ha
/* 803678B4 003647F4  38 83 A4 44 */	addi r4, r3, __dt__26JSUList$$015JALSeModVolFunk$$1Fv@l
/* 803678B8 003647F8  38 6D 8F 80 */	addi r3, r13, smList__26JALList$$015JALSeModVolFunk$$1-_SDA_BASE_
/* 803678BC 003647FC  38 BF 00 A8 */	addi r5, r31, 0xa8
/* 803678C0 00364800  4B D1 AE 69 */	bl __register_global_object
/* 803678C4 00364804  38 00 00 01 */	li r0, 1
/* 803678C8 00364808  98 0D 8F 97 */	stb r0, __init__smList__26JALList$$015JALSeModVolFunk$$1-_SDA_BASE_(r13)
lbl_803678CC:
/* 803678CC 0036480C  80 01 00 14 */	lwz r0, 0x14(r1)
/* 803678D0 00364810  83 E1 00 0C */	lwz r31, 0xc(r1)
/* 803678D4 00364814  38 21 00 10 */	addi r1, r1, 0x10
/* 803678D8 00364818  7C 08 03 A6 */	mtlr r0
/* 803678DC 0036481C  4E 80 00 20 */	blr 

.section .ctors, "wa"  # 0x8036FBA0 - 0x8036FF80
	.incbin "baserom.dol", 0x36CF54, 0x4

.section .rodata, "wa"  # 0x8036FFA0 - 0x803A8380
$$21490:
	.incbin "baserom.dol", 0x3A4B20, 0xC
$$21937:
	.incbin "baserom.dol", 0x3A4B2C, 0x14
$$22337:
	.incbin "baserom.dol", 0x3A4B40, 0x30
$$22338:
	.incbin "baserom.dol", 0x3A4B70, 0x38
$$22339:
	.incbin "baserom.dol", 0x3A4BA8, 0x34
$$22340:
	.incbin "baserom.dol", 0x3A4BDC, 0x24
$$22543:
	.incbin "baserom.dol", 0x3A4C00, 0x10
entry$2542:
	.incbin "baserom.dol", 0x3A4C10, 0x18
$$22573:
	.incbin "baserom.dol", 0x3A4C28, 0x10

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global __vt__11TMewManager
__vt__11TMewManager:
	.incbin "baserom.dol", 0x3E2868, 0x54
.global __vt__18TAnimalManagerBase
__vt__18TAnimalManagerBase:
	.incbin "baserom.dol", 0x3E28BC, 0x54

.section .sdata2, "wa"  # 0x8040B460 - 0x80414020
$$22600:
	.incbin "baserom.dol", 0x3EC6F0, 0x4
$$22613:
	.incbin "baserom.dol", 0x3EC6F4, 0x4

.section .bss, "wa"  # 0x803E6000 - 0x80408AC0
$$22616:
	.skip 0xC
$$22617:
	.skip 0xC
$$22618:
	.skip 0xC
$$22619:
	.skip 0xC
$$22620:
	.skip 0xC
$$22621:
	.skip 0xC
$$22622:
	.skip 0xC
$$22623:
	.skip 0xC
$$22624:
	.skip 0xC
$$22625:
	.skip 0xC
$$22626:
	.skip 0xC
$$22627:
	.skip 0xC
$$22628:
	.skip 0xC
$$22629:
	.skip 0xC
$$22630:
	.skip 0x10
