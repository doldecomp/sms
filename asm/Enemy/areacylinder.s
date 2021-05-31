.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global __dt__20TAreaCylinderManagerFv
__dt__20TAreaCylinderManagerFv:
/* 8031926C 003161AC  7C 08 02 A6 */	mflr r0
/* 80319270 003161B0  90 01 00 04 */	stw r0, 4(r1)
/* 80319274 003161B4  94 21 FF 98 */	stwu r1, -0x68(r1)
/* 80319278 003161B8  BF 61 00 54 */	stmw r27, 0x54(r1)
/* 8031927C 003161BC  7C 7F 1B 79 */	or. r31, r3, r3
/* 80319280 003161C0  3B 64 00 00 */	addi r27, r4, 0
/* 80319284 003161C4  41 82 00 C4 */	beq lbl_80319348
/* 80319288 003161C8  3C 60 80 3E */	lis r3, __vt__20TAreaCylinderManager@ha
/* 8031928C 003161CC  38 63 0E 38 */	addi r3, r3, __vt__20TAreaCylinderManager@l
/* 80319290 003161D0  34 1F 00 10 */	addic. r0, r31, 0x10
/* 80319294 003161D4  90 7F 00 00 */	stw r3, 0(r31)
/* 80319298 003161D8  41 82 00 80 */	beq lbl_80319318
/* 8031929C 003161DC  38 61 00 4C */	addi r3, r1, 0x4c
/* 803192A0 003161E0  38 9F 00 18 */	addi r4, r31, 0x18
/* 803192A4 003161E4  48 00 01 41 */	bl __ct__Q37JGadget64TList$$0P13TAreaCylinder$$4Q27JGadget28TAllocator$$0P13TAreaCylinder$$1$$18iteratorFPQ37JGadget64TList$$0P13TAreaCylinder$$4Q27JGadget28TAllocator$$0P13TAreaCylinder$$1$$16TNode_
/* 803192A8 003161E8  80 01 00 4C */	lwz r0, 0x4c(r1)
/* 803192AC 003161EC  38 61 00 44 */	addi r3, r1, 0x44
/* 803192B0 003161F0  90 01 00 48 */	stw r0, 0x48(r1)
/* 803192B4 003161F4  80 9F 00 18 */	lwz r4, 0x18(r31)
/* 803192B8 003161F8  48 00 01 2D */	bl __ct__Q37JGadget64TList$$0P13TAreaCylinder$$4Q27JGadget28TAllocator$$0P13TAreaCylinder$$1$$18iteratorFPQ37JGadget64TList$$0P13TAreaCylinder$$4Q27JGadget28TAllocator$$0P13TAreaCylinder$$1$$16TNode_
/* 803192BC 003161FC  80 01 00 44 */	lwz r0, 0x44(r1)
/* 803192C0 00316200  3B C1 00 30 */	addi r30, r1, 0x30
/* 803192C4 00316204  3B A1 00 28 */	addi r29, r1, 0x28
/* 803192C8 00316208  90 01 00 40 */	stw r0, 0x40(r1)
/* 803192CC 0031620C  3B 81 00 24 */	addi r28, r1, 0x24
/* 803192D0 00316210  48 00 00 24 */	b lbl_803192F4
lbl_803192D4:
/* 803192D4 00316214  80 01 00 40 */	lwz r0, 0x40(r1)
/* 803192D8 00316218  38 BE 00 00 */	addi r5, r30, 0
/* 803192DC 0031621C  38 61 00 2C */	addi r3, r1, 0x2c
/* 803192E0 00316220  90 01 00 30 */	stw r0, 0x30(r1)
/* 803192E4 00316224  38 9F 00 10 */	addi r4, r31, 0x10
/* 803192E8 00316228  48 00 00 91 */	bl erase__Q27JGadget64TList$$0P13TAreaCylinder$$4Q27JGadget28TAllocator$$0P13TAreaCylinder$$1$$1FQ37JGadget64TList$$0P13TAreaCylinder$$4Q27JGadget28TAllocator$$0P13TAreaCylinder$$1$$18iterator
/* 803192EC 0031622C  80 01 00 2C */	lwz r0, 0x2c(r1)
/* 803192F0 00316230  90 01 00 40 */	stw r0, 0x40(r1)
lbl_803192F4:
/* 803192F4 00316234  80 A1 00 48 */	lwz r5, 0x48(r1)
/* 803192F8 00316238  7F 83 E3 78 */	mr r3, r28
/* 803192FC 0031623C  80 01 00 40 */	lwz r0, 0x40(r1)
/* 80319300 00316240  7F A4 EB 78 */	mr r4, r29
/* 80319304 00316244  90 A1 00 28 */	stw r5, 0x28(r1)
/* 80319308 00316248  90 01 00 24 */	stw r0, 0x24(r1)
/* 8031930C 0031624C  48 00 00 55 */	bl __eq__7JGadgetFQ37JGadget64TList$$0P13TAreaCylinder$$4Q27JGadget28TAllocator$$0P13TAreaCylinder$$1$$18iteratorQ37JGadget64TList$$0P13TAreaCylinder$$4Q27JGadget28TAllocator$$0P13TAreaCylinder$$1$$18iterator
/* 80319310 00316250  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 80319314 00316254  41 82 FF C0 */	beq lbl_803192D4
lbl_80319318:
/* 80319318 00316258  28 1F 00 00 */	cmplwi r31, 0
/* 8031931C 0031625C  41 82 00 1C */	beq lbl_80319338
/* 80319320 00316260  3C 60 80 3B */	lis r3, __vt__Q26JDrama8TViewObj@ha
/* 80319324 00316264  38 03 A0 C0 */	addi r0, r3, __vt__Q26JDrama8TViewObj@l
/* 80319328 00316268  90 1F 00 00 */	stw r0, 0(r31)
/* 8031932C 0031626C  38 7F 00 00 */	addi r3, r31, 0
/* 80319330 00316270  38 80 00 00 */	li r4, 0
/* 80319334 00316274  4B D2 B4 C9 */	bl __dt__Q26JDrama8TNameRefFv
lbl_80319338:
/* 80319338 00316278  7F 60 07 35 */	extsh. r0, r27
/* 8031933C 0031627C  40 81 00 0C */	ble lbl_80319348
/* 80319340 00316280  7F E3 FB 78 */	mr r3, r31
/* 80319344 00316284  4B CF 37 6D */	bl __dl__FPv
lbl_80319348:
/* 80319348 00316288  7F E3 FB 78 */	mr r3, r31
/* 8031934C 0031628C  BB 61 00 54 */	lmw r27, 0x54(r1)
/* 80319350 00316290  80 01 00 6C */	lwz r0, 0x6c(r1)
/* 80319354 00316294  38 21 00 68 */	addi r1, r1, 0x68
/* 80319358 00316298  7C 08 03 A6 */	mtlr r0
/* 8031935C 0031629C  4E 80 00 20 */	blr 

.global __eq__7JGadgetFQ37JGadget64TList$$0P13TAreaCylinder$$4Q27JGadget28TAllocator$$0P13TAreaCylinder$$1$$18iteratorQ37JGadget64TList$$0P13TAreaCylinder$$4Q27JGadget28TAllocator$$0P13TAreaCylinder$$1$$18iterator
__eq__7JGadgetFQ37JGadget64TList$$0P13TAreaCylinder$$4Q27JGadget28TAllocator$$0P13TAreaCylinder$$1$$18iteratorQ37JGadget64TList$$0P13TAreaCylinder$$4Q27JGadget28TAllocator$$0P13TAreaCylinder$$1$$18iterator:
/* 80319360 003162A0  80 63 00 00 */	lwz r3, 0(r3)
/* 80319364 003162A4  80 04 00 00 */	lwz r0, 0(r4)
/* 80319368 003162A8  7C 03 00 50 */	subf r0, r3, r0
/* 8031936C 003162AC  7C 00 00 34 */	cntlzw r0, r0
/* 80319370 003162B0  54 03 D9 7E */	srwi r3, r0, 5
/* 80319374 003162B4  4E 80 00 20 */	blr 

.global erase__Q27JGadget64TList$$0P13TAreaCylinder$$4Q27JGadget28TAllocator$$0P13TAreaCylinder$$1$$1FQ37JGadget64TList$$0P13TAreaCylinder$$4Q27JGadget28TAllocator$$0P13TAreaCylinder$$1$$18iterator
erase__Q27JGadget64TList$$0P13TAreaCylinder$$4Q27JGadget28TAllocator$$0P13TAreaCylinder$$1$$1FQ37JGadget64TList$$0P13TAreaCylinder$$4Q27JGadget28TAllocator$$0P13TAreaCylinder$$1$$18iterator:
/* 80319378 003162B8  7C 08 02 A6 */	mflr r0
/* 8031937C 003162BC  90 01 00 04 */	stw r0, 4(r1)
/* 80319380 003162C0  94 21 FF D8 */	stwu r1, -0x28(r1)
/* 80319384 003162C4  93 E1 00 24 */	stw r31, 0x24(r1)
/* 80319388 003162C8  93 C1 00 20 */	stw r30, 0x20(r1)
/* 8031938C 003162CC  3B C4 00 00 */	addi r30, r4, 0
/* 80319390 003162D0  93 A1 00 1C */	stw r29, 0x1c(r1)
/* 80319394 003162D4  3B A3 00 00 */	addi r29, r3, 0
/* 80319398 003162D8  80 A5 00 00 */	lwz r5, 0(r5)
/* 8031939C 003162DC  83 E5 00 00 */	lwz r31, 0(r5)
/* 803193A0 003162E0  7C A3 2B 78 */	mr r3, r5
/* 803193A4 003162E4  80 85 00 04 */	lwz r4, 4(r5)
/* 803193A8 003162E8  93 E4 00 00 */	stw r31, 0(r4)
/* 803193AC 003162EC  80 05 00 04 */	lwz r0, 4(r5)
/* 803193B0 003162F0  90 1F 00 04 */	stw r0, 4(r31)
/* 803193B4 003162F4  4B CF 36 FD */	bl __dl__FPv
/* 803193B8 003162F8  80 7E 00 04 */	lwz r3, 4(r30)
/* 803193BC 003162FC  38 03 FF FF */	addi r0, r3, -1
/* 803193C0 00316300  90 1E 00 04 */	stw r0, 4(r30)
/* 803193C4 00316304  93 FD 00 00 */	stw r31, 0(r29)
/* 803193C8 00316308  80 01 00 2C */	lwz r0, 0x2c(r1)
/* 803193CC 0031630C  83 E1 00 24 */	lwz r31, 0x24(r1)
/* 803193D0 00316310  83 C1 00 20 */	lwz r30, 0x20(r1)
/* 803193D4 00316314  7C 08 03 A6 */	mtlr r0
/* 803193D8 00316318  83 A1 00 1C */	lwz r29, 0x1c(r1)
/* 803193DC 0031631C  38 21 00 28 */	addi r1, r1, 0x28
/* 803193E0 00316320  4E 80 00 20 */	blr 

.global __ct__Q37JGadget64TList$$0P13TAreaCylinder$$4Q27JGadget28TAllocator$$0P13TAreaCylinder$$1$$18iteratorFPQ37JGadget64TList$$0P13TAreaCylinder$$4Q27JGadget28TAllocator$$0P13TAreaCylinder$$1$$16TNode_
__ct__Q37JGadget64TList$$0P13TAreaCylinder$$4Q27JGadget28TAllocator$$0P13TAreaCylinder$$1$$18iteratorFPQ37JGadget64TList$$0P13TAreaCylinder$$4Q27JGadget28TAllocator$$0P13TAreaCylinder$$1$$16TNode_:
/* 803193E4 00316324  90 83 00 00 */	stw r4, 0(r3)
/* 803193E8 00316328  4E 80 00 20 */	blr 

.global perform__20TAreaCylinderManagerFUlPQ26JDrama9TGraphics
perform__20TAreaCylinderManagerFUlPQ26JDrama9TGraphics:
/* 803193EC 0031632C  4E 80 00 20 */	blr 

.global getCylinderContains__20TAreaCylinderManagerFRCQ29JGeometry8TVec3$$0f$$1
getCylinderContains__20TAreaCylinderManagerFRCQ29JGeometry8TVec3$$0f$$1:
/* 803193F0 00316330  94 21 FF A8 */	stwu r1, -0x58(r1)
/* 803193F4 00316334  38 03 00 18 */	addi r0, r3, 0x18
/* 803193F8 00316338  80 A3 00 18 */	lwz r5, 0x18(r3)
/* 803193FC 0031633C  90 01 00 4C */	stw r0, 0x4c(r1)
/* 80319400 00316340  80 01 00 4C */	lwz r0, 0x4c(r1)
/* 80319404 00316344  90 A1 00 50 */	stw r5, 0x50(r1)
/* 80319408 00316348  90 01 00 38 */	stw r0, 0x38(r1)
/* 8031940C 0031634C  80 01 00 38 */	lwz r0, 0x38(r1)
/* 80319410 00316350  48 00 00 88 */	b lbl_80319498
lbl_80319414:
/* 80319414 00316354  80 61 00 50 */	lwz r3, 0x50(r1)
/* 80319418 00316358  C0 44 00 04 */	lfs f2, 4(r4)
/* 8031941C 0031635C  80 63 00 08 */	lwz r3, 8(r3)
/* 80319420 00316360  C0 23 00 14 */	lfs f1, 0x14(r3)
/* 80319424 00316364  FC 02 08 40 */	fcmpo cr0, f2, f1
/* 80319428 00316368  41 80 00 14 */	blt lbl_8031943C
/* 8031942C 0031636C  C0 03 00 20 */	lfs f0, 0x20(r3)
/* 80319430 00316370  EC 01 00 2A */	fadds f0, f1, f0
/* 80319434 00316374  FC 00 10 40 */	fcmpo cr0, f0, f2
/* 80319438 00316378  40 80 00 0C */	bge lbl_80319444
lbl_8031943C:
/* 8031943C 0031637C  38 A0 00 00 */	li r5, 0
/* 80319440 00316380  48 00 00 40 */	b lbl_80319480
lbl_80319444:
/* 80319444 00316384  C0 64 00 00 */	lfs f3, 0(r4)
/* 80319448 00316388  C0 43 00 10 */	lfs f2, 0x10(r3)
/* 8031944C 0031638C  C0 24 00 08 */	lfs f1, 8(r4)
/* 80319450 00316390  EC 43 10 28 */	fsubs f2, f3, f2
/* 80319454 00316394  C0 03 00 18 */	lfs f0, 0x18(r3)
/* 80319458 00316398  C0 63 00 1C */	lfs f3, 0x1c(r3)
/* 8031945C 0031639C  EC 21 00 28 */	fsubs f1, f1, f0
/* 80319460 003163A0  EC 42 00 B2 */	fmuls f2, f2, f2
/* 80319464 003163A4  EC 03 00 F2 */	fmuls f0, f3, f3
/* 80319468 003163A8  EC 21 00 72 */	fmuls f1, f1, f1
/* 8031946C 003163AC  EC 22 08 2A */	fadds f1, f2, f1
/* 80319470 003163B0  FC 01 00 40 */	fcmpo cr0, f1, f0
/* 80319474 003163B4  4C 40 13 82 */	cror 2, 0, 2
/* 80319478 003163B8  7C A0 00 26 */	mfcr r5
/* 8031947C 003163BC  54 A5 1F FE */	rlwinm r5, r5, 3, 0x1f, 0x1f
lbl_80319480:
/* 80319480 003163C0  2C 05 00 00 */	cmpwi r5, 0
/* 80319484 003163C4  41 82 00 08 */	beq lbl_8031948C
/* 80319488 003163C8  48 00 00 28 */	b lbl_803194B0
lbl_8031948C:
/* 8031948C 003163CC  80 61 00 50 */	lwz r3, 0x50(r1)
/* 80319490 003163D0  80 63 00 00 */	lwz r3, 0(r3)
/* 80319494 003163D4  90 61 00 50 */	stw r3, 0x50(r1)
lbl_80319498:
/* 80319498 003163D8  80 61 00 50 */	lwz r3, 0x50(r1)
/* 8031949C 003163DC  90 61 00 34 */	stw r3, 0x34(r1)
/* 803194A0 003163E0  80 61 00 34 */	lwz r3, 0x34(r1)
/* 803194A4 003163E4  7C 03 00 40 */	cmplw r3, r0
/* 803194A8 003163E8  40 82 FF 6C */	bne lbl_80319414
/* 803194AC 003163EC  38 60 00 00 */	li r3, 0
lbl_803194B0:
/* 803194B0 003163F0  38 21 00 58 */	addi r1, r1, 0x58
/* 803194B4 003163F4  4E 80 00 20 */	blr 

.global contain__20TAreaCylinderManagerFRCQ29JGeometry8TVec3$$0f$$1
contain__20TAreaCylinderManagerFRCQ29JGeometry8TVec3$$0f$$1:
/* 803194B8 003163F8  94 21 FF B0 */	stwu r1, -0x50(r1)
/* 803194BC 003163FC  38 03 00 18 */	addi r0, r3, 0x18
/* 803194C0 00316400  80 A3 00 18 */	lwz r5, 0x18(r3)
/* 803194C4 00316404  90 01 00 44 */	stw r0, 0x44(r1)
/* 803194C8 00316408  80 01 00 44 */	lwz r0, 0x44(r1)
/* 803194CC 0031640C  90 A1 00 48 */	stw r5, 0x48(r1)
/* 803194D0 00316410  90 01 00 30 */	stw r0, 0x30(r1)
/* 803194D4 00316414  80 01 00 30 */	lwz r0, 0x30(r1)
/* 803194D8 00316418  48 00 00 8C */	b lbl_80319564
lbl_803194DC:
/* 803194DC 0031641C  80 61 00 48 */	lwz r3, 0x48(r1)
/* 803194E0 00316420  C0 44 00 04 */	lfs f2, 4(r4)
/* 803194E4 00316424  80 63 00 08 */	lwz r3, 8(r3)
/* 803194E8 00316428  C0 23 00 14 */	lfs f1, 0x14(r3)
/* 803194EC 0031642C  FC 02 08 40 */	fcmpo cr0, f2, f1
/* 803194F0 00316430  41 80 00 14 */	blt lbl_80319504
/* 803194F4 00316434  C0 03 00 20 */	lfs f0, 0x20(r3)
/* 803194F8 00316438  EC 01 00 2A */	fadds f0, f1, f0
/* 803194FC 0031643C  FC 00 10 40 */	fcmpo cr0, f0, f2
/* 80319500 00316440  40 80 00 0C */	bge lbl_8031950C
lbl_80319504:
/* 80319504 00316444  38 60 00 00 */	li r3, 0
/* 80319508 00316448  48 00 00 40 */	b lbl_80319548
lbl_8031950C:
/* 8031950C 0031644C  C0 64 00 00 */	lfs f3, 0(r4)
/* 80319510 00316450  C0 43 00 10 */	lfs f2, 0x10(r3)
/* 80319514 00316454  C0 24 00 08 */	lfs f1, 8(r4)
/* 80319518 00316458  EC 43 10 28 */	fsubs f2, f3, f2
/* 8031951C 0031645C  C0 03 00 18 */	lfs f0, 0x18(r3)
/* 80319520 00316460  C0 63 00 1C */	lfs f3, 0x1c(r3)
/* 80319524 00316464  EC 21 00 28 */	fsubs f1, f1, f0
/* 80319528 00316468  EC 42 00 B2 */	fmuls f2, f2, f2
/* 8031952C 0031646C  EC 03 00 F2 */	fmuls f0, f3, f3
/* 80319530 00316470  EC 21 00 72 */	fmuls f1, f1, f1
/* 80319534 00316474  EC 22 08 2A */	fadds f1, f2, f1
/* 80319538 00316478  FC 01 00 40 */	fcmpo cr0, f1, f0
/* 8031953C 0031647C  4C 40 13 82 */	cror 2, 0, 2
/* 80319540 00316480  7C 60 00 26 */	mfcr r3
/* 80319544 00316484  54 63 1F FE */	rlwinm r3, r3, 3, 0x1f, 0x1f
lbl_80319548:
/* 80319548 00316488  2C 03 00 00 */	cmpwi r3, 0
/* 8031954C 0031648C  41 82 00 0C */	beq lbl_80319558
/* 80319550 00316490  38 60 00 01 */	li r3, 1
/* 80319554 00316494  48 00 00 28 */	b lbl_8031957C
lbl_80319558:
/* 80319558 00316498  80 61 00 48 */	lwz r3, 0x48(r1)
/* 8031955C 0031649C  80 63 00 00 */	lwz r3, 0(r3)
/* 80319560 003164A0  90 61 00 48 */	stw r3, 0x48(r1)
lbl_80319564:
/* 80319564 003164A4  80 61 00 48 */	lwz r3, 0x48(r1)
/* 80319568 003164A8  90 61 00 2C */	stw r3, 0x2c(r1)
/* 8031956C 003164AC  80 61 00 2C */	lwz r3, 0x2c(r1)
/* 80319570 003164B0  7C 03 00 40 */	cmplw r3, r0
/* 80319574 003164B4  40 82 FF 68 */	bne lbl_803194DC
/* 80319578 003164B8  38 60 00 00 */	li r3, 0
lbl_8031957C:
/* 8031957C 003164BC  38 21 00 50 */	addi r1, r1, 0x50
/* 80319580 003164C0  4E 80 00 20 */	blr 

.global insert__Q27JGadget64TList$$0P13TAreaCylinder$$4Q27JGadget28TAllocator$$0P13TAreaCylinder$$1$$1FQ37JGadget64TList$$0P13TAreaCylinder$$4Q27JGadget28TAllocator$$0P13TAreaCylinder$$1$$18iteratorRCP13TAreaCylinder
insert__Q27JGadget64TList$$0P13TAreaCylinder$$4Q27JGadget28TAllocator$$0P13TAreaCylinder$$1$$1FQ37JGadget64TList$$0P13TAreaCylinder$$4Q27JGadget28TAllocator$$0P13TAreaCylinder$$1$$18iteratorRCP13TAreaCylinder:
/* 80319584 003164C4  7C 08 02 A6 */	mflr r0
/* 80319588 003164C8  90 01 00 04 */	stw r0, 4(r1)
/* 8031958C 003164CC  94 21 FF B8 */	stwu r1, -0x48(r1)
/* 80319590 003164D0  93 E1 00 44 */	stw r31, 0x44(r1)
/* 80319594 003164D4  93 C1 00 40 */	stw r30, 0x40(r1)
/* 80319598 003164D8  93 A1 00 3C */	stw r29, 0x3c(r1)
/* 8031959C 003164DC  3B A6 00 00 */	addi r29, r6, 0
/* 803195A0 003164E0  93 81 00 38 */	stw r28, 0x38(r1)
/* 803195A4 003164E4  3B 84 00 00 */	addi r28, r4, 0
/* 803195A8 003164E8  90 61 00 08 */	stw r3, 8(r1)
/* 803195AC 003164EC  38 60 00 0C */	li r3, 0xc
/* 803195B0 003164F0  83 E5 00 00 */	lwz r31, 0(r5)
/* 803195B4 003164F4  83 DF 00 04 */	lwz r30, 4(r31)
/* 803195B8 003164F8  4B CF 32 F9 */	bl __nw__FUl
/* 803195BC 003164FC  28 03 00 00 */	cmplwi r3, 0
/* 803195C0 00316500  40 82 00 0C */	bne lbl_803195CC
/* 803195C4 00316504  38 60 00 00 */	li r3, 0
/* 803195C8 00316508  48 00 00 20 */	b lbl_803195E8
lbl_803195CC:
/* 803195CC 0031650C  93 E3 00 00 */	stw r31, 0(r3)
/* 803195D0 00316510  34 83 00 08 */	addic. r4, r3, 8
/* 803195D4 00316514  93 C3 00 04 */	stw r30, 4(r3)
/* 803195D8 00316518  90 81 00 2C */	stw r4, 0x2c(r1)
/* 803195DC 0031651C  41 82 00 0C */	beq lbl_803195E8
/* 803195E0 00316520  80 1D 00 00 */	lwz r0, 0(r29)
/* 803195E4 00316524  90 04 00 00 */	stw r0, 0(r4)
lbl_803195E8:
/* 803195E8 00316528  28 03 00 00 */	cmplwi r3, 0
/* 803195EC 0031652C  40 82 00 14 */	bne lbl_80319600
/* 803195F0 00316530  80 61 00 08 */	lwz r3, 8(r1)
/* 803195F4 00316534  38 1C 00 08 */	addi r0, r28, 8
/* 803195F8 00316538  90 03 00 00 */	stw r0, 0(r3)
/* 803195FC 0031653C  48 00 00 20 */	b lbl_8031961C
lbl_80319600:
/* 80319600 00316540  90 7F 00 04 */	stw r3, 4(r31)
/* 80319604 00316544  90 7E 00 00 */	stw r3, 0(r30)
/* 80319608 00316548  80 9C 00 04 */	lwz r4, 4(r28)
/* 8031960C 0031654C  38 04 00 01 */	addi r0, r4, 1
/* 80319610 00316550  90 1C 00 04 */	stw r0, 4(r28)
/* 80319614 00316554  80 81 00 08 */	lwz r4, 8(r1)
/* 80319618 00316558  90 64 00 00 */	stw r3, 0(r4)
lbl_8031961C:
/* 8031961C 0031655C  80 01 00 4C */	lwz r0, 0x4c(r1)
/* 80319620 00316560  83 E1 00 44 */	lwz r31, 0x44(r1)
/* 80319624 00316564  83 C1 00 40 */	lwz r30, 0x40(r1)
/* 80319628 00316568  7C 08 03 A6 */	mtlr r0
/* 8031962C 0031656C  83 A1 00 3C */	lwz r29, 0x3c(r1)
/* 80319630 00316570  83 81 00 38 */	lwz r28, 0x38(r1)
/* 80319634 00316574  38 21 00 48 */	addi r1, r1, 0x48
/* 80319638 00316578  4E 80 00 20 */	blr 

.global perform__13TAreaCylinderFUlPQ26JDrama9TGraphics
perform__13TAreaCylinderFUlPQ26JDrama9TGraphics:
/* 8031963C 0031657C  4E 80 00 20 */	blr 

.global load__13TAreaCylinderFR20JSUMemoryInputStream
load__13TAreaCylinderFR20JSUMemoryInputStream:
/* 80319640 00316580  7C 08 02 A6 */	mflr r0
/* 80319644 00316584  90 01 00 04 */	stw r0, 4(r1)
/* 80319648 00316588  94 21 FF 60 */	stwu r1, -0xa0(r1)
/* 8031964C 0031658C  93 E1 00 9C */	stw r31, 0x9c(r1)
/* 80319650 00316590  3B E4 00 00 */	addi r31, r4, 0
/* 80319654 00316594  93 C1 00 98 */	stw r30, 0x98(r1)
/* 80319658 00316598  3B C3 00 00 */	addi r30, r3, 0
/* 8031965C 0031659C  93 A1 00 94 */	stw r29, 0x94(r1)
/* 80319660 003165A0  93 81 00 90 */	stw r28, 0x90(r1)
/* 80319664 003165A4  4B D2 B1 ED */	bl load__Q26JDrama8TNameRefFR20JSUMemoryInputStream
/* 80319668 003165A8  38 7F 00 00 */	addi r3, r31, 0
/* 8031966C 003165AC  38 9E 00 10 */	addi r4, r30, 0x10
/* 80319670 003165B0  38 A0 00 04 */	li r5, 4
/* 80319674 003165B4  4B CF 4F 1D */	bl read__14JSUInputStreamFPvl
/* 80319678 003165B8  38 7F 00 00 */	addi r3, r31, 0
/* 8031967C 003165BC  38 9E 00 14 */	addi r4, r30, 0x14
/* 80319680 003165C0  38 A0 00 04 */	li r5, 4
/* 80319684 003165C4  4B CF 4F 0D */	bl read__14JSUInputStreamFPvl
/* 80319688 003165C8  38 7F 00 00 */	addi r3, r31, 0
/* 8031968C 003165CC  38 9E 00 18 */	addi r4, r30, 0x18
/* 80319690 003165D0  38 A0 00 04 */	li r5, 4
/* 80319694 003165D4  4B CF 4E FD */	bl read__14JSUInputStreamFPvl
/* 80319698 003165D8  38 7F 00 00 */	addi r3, r31, 0
/* 8031969C 003165DC  38 81 00 78 */	addi r4, r1, 0x78
/* 803196A0 003165E0  38 A0 00 04 */	li r5, 4
/* 803196A4 003165E4  4B CF 4E ED */	bl read__14JSUInputStreamFPvl
/* 803196A8 003165E8  38 7F 00 00 */	addi r3, r31, 0
/* 803196AC 003165EC  38 81 00 7C */	addi r4, r1, 0x7c
/* 803196B0 003165F0  38 A0 00 04 */	li r5, 4
/* 803196B4 003165F4  4B CF 4E DD */	bl read__14JSUInputStreamFPvl
/* 803196B8 003165F8  3B A1 00 80 */	addi r29, r1, 0x80
/* 803196BC 003165FC  38 7F 00 00 */	addi r3, r31, 0
/* 803196C0 00316600  38 9D 00 00 */	addi r4, r29, 0
/* 803196C4 00316604  38 A0 00 04 */	li r5, 4
/* 803196C8 00316608  4B CF 4E C9 */	bl read__14JSUInputStreamFPvl
/* 803196CC 0031660C  38 7F 00 00 */	addi r3, r31, 0
/* 803196D0 00316610  38 9E 00 1C */	addi r4, r30, 0x1c
/* 803196D4 00316614  38 A0 00 04 */	li r5, 4
/* 803196D8 00316618  4B CF 4E B9 */	bl read__14JSUInputStreamFPvl
/* 803196DC 0031661C  38 7F 00 00 */	addi r3, r31, 0
/* 803196E0 00316620  38 9E 00 20 */	addi r4, r30, 0x20
/* 803196E4 00316624  38 A0 00 04 */	li r5, 4
/* 803196E8 00316628  4B CF 4E A9 */	bl read__14JSUInputStreamFPvl
/* 803196EC 0031662C  38 7F 00 00 */	addi r3, r31, 0
/* 803196F0 00316630  38 9D 00 00 */	addi r4, r29, 0
/* 803196F4 00316634  38 A0 00 04 */	li r5, 4
/* 803196F8 00316638  4B CF 4E 99 */	bl read__14JSUInputStreamFPvl
/* 803196FC 0031663C  C0 1E 00 1C */	lfs f0, 0x1c(r30)
/* 80319700 00316640  7F E3 FB 78 */	mr r3, r31
/* 80319704 00316644  C0 22 FB C0 */	lfs f1, $$22137@sda21(r2)
/* 80319708 00316648  EC 00 00 72 */	fmuls f0, f0, f1
/* 8031970C 0031664C  D0 1E 00 1C */	stfs f0, 0x1c(r30)
/* 80319710 00316650  C0 1E 00 20 */	lfs f0, 0x20(r30)
/* 80319714 00316654  EC 00 00 72 */	fmuls f0, f0, f1
/* 80319718 00316658  D0 1E 00 20 */	stfs f0, 0x20(r30)
/* 8031971C 0031665C  4B CF 4E CD */	bl readString__14JSUInputStreamFv
/* 80319720 00316660  38 7F 00 00 */	addi r3, r31, 0
/* 80319724 00316664  38 81 00 6C */	addi r4, r1, 0x6c
/* 80319728 00316668  38 A0 00 04 */	li r5, 4
/* 8031972C 0031666C  4B CF 4E 65 */	bl read__14JSUInputStreamFPvl
/* 80319730 00316670  83 A1 00 6C */	lwz r29, 0x6c(r1)
/* 80319734 00316674  3B 80 00 00 */	li r28, 0
/* 80319738 00316678  48 00 00 20 */	b lbl_80319758
lbl_8031973C:
/* 8031973C 0031667C  38 7F 00 00 */	addi r3, r31, 0
/* 80319740 00316680  38 81 00 68 */	addi r4, r1, 0x68
/* 80319744 00316684  38 A0 00 04 */	li r5, 4
/* 80319748 00316688  4B CF 4E 49 */	bl read__14JSUInputStreamFPvl
/* 8031974C 0031668C  7F E3 FB 78 */	mr r3, r31
/* 80319750 00316690  4B CF 4E 99 */	bl readString__14JSUInputStreamFv
/* 80319754 00316694  3B 9C 00 01 */	addi r28, r28, 1
lbl_80319758:
/* 80319758 00316698  7C 1C E8 00 */	cmpw r28, r29
/* 8031975C 0031669C  41 80 FF E0 */	blt lbl_8031973C
/* 80319760 003166A0  7F E3 FB 78 */	mr r3, r31
/* 80319764 003166A4  4B CF 4E 85 */	bl readString__14JSUInputStreamFv
/* 80319768 003166A8  83 AD 9C 28 */	lwz r29, gpConductor@sda21(r13)
/* 8031976C 003166AC  7C 7C 1B 78 */	mr r28, r3
/* 80319770 003166B0  4B D2 AF 1D */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 80319774 003166B4  81 9D 00 00 */	lwz r12, 0(r29)
/* 80319778 003166B8  38 83 00 00 */	addi r4, r3, 0
/* 8031977C 003166BC  38 7D 00 00 */	addi r3, r29, 0
/* 80319780 003166C0  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 80319784 003166C4  7F 85 E3 78 */	mr r5, r28
/* 80319788 003166C8  7D 88 03 A6 */	mtlr r12
/* 8031978C 003166CC  4E 80 00 21 */	blrl 
/* 80319790 003166D0  28 03 00 00 */	cmplwi r3, 0
/* 80319794 003166D4  38 83 00 00 */	addi r4, r3, 0
/* 80319798 003166D8  40 82 00 80 */	bne lbl_80319818
/* 8031979C 003166DC  38 60 00 20 */	li r3, 0x20
/* 803197A0 003166E0  4B CF 31 11 */	bl __nw__FUl
/* 803197A4 003166E4  7C 7D 1B 79 */	or. r29, r3, r3
/* 803197A8 003166E8  41 82 00 6C */	beq lbl_80319814
/* 803197AC 003166EC  3C 60 80 3B */	lis r3, __vt__Q26JDrama8TNameRef@ha
/* 803197B0 003166F0  93 A1 00 64 */	stw r29, 0x64(r1)
/* 803197B4 003166F4  38 03 A8 60 */	addi r0, r3, __vt__Q26JDrama8TNameRef@l
/* 803197B8 003166F8  90 1D 00 00 */	stw r0, 0(r29)
/* 803197BC 003166FC  7F 83 E3 78 */	mr r3, r28
/* 803197C0 00316700  93 9D 00 04 */	stw r28, 4(r29)
/* 803197C4 00316704  4B D2 AE C9 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 803197C8 00316708  3C 80 80 3B */	lis r4, __vt__Q26JDrama8TViewObj@ha
/* 803197CC 0031670C  B0 7D 00 08 */	sth r3, 8(r29)
/* 803197D0 00316710  38 04 A0 C0 */	addi r0, r4, __vt__Q26JDrama8TViewObj@l
/* 803197D4 00316714  90 1D 00 00 */	stw r0, 0(r29)
/* 803197D8 00316718  38 A0 00 00 */	li r5, 0
/* 803197DC 0031671C  3C 60 80 3E */	lis r3, __vt__20TAreaCylinderManager@ha
/* 803197E0 00316720  B0 BD 00 0C */	sth r5, 0xc(r29)
/* 803197E4 00316724  38 03 0E 38 */	addi r0, r3, __vt__20TAreaCylinderManager@l
/* 803197E8 00316728  80 C1 00 64 */	lwz r6, 0x64(r1)
/* 803197EC 0031672C  90 06 00 00 */	stw r0, 0(r6)
/* 803197F0 00316730  38 06 00 18 */	addi r0, r6, 0x18
/* 803197F4 00316734  7C C4 33 78 */	mr r4, r6
/* 803197F8 00316738  88 61 00 54 */	lbz r3, 0x54(r1)
/* 803197FC 0031673C  98 66 00 10 */	stb r3, 0x10(r6)
/* 80319800 00316740  90 A6 00 14 */	stw r5, 0x14(r6)
/* 80319804 00316744  90 06 00 18 */	stw r0, 0x18(r6)
/* 80319808 00316748  90 06 00 1C */	stw r0, 0x1c(r6)
/* 8031980C 0031674C  80 6D 9C 28 */	lwz r3, gpConductor@sda21(r13)
/* 80319810 00316750  4B F2 F7 A5 */	bl registerAreaCylinderManager__10TConductorFP20TAreaCylinderManager
lbl_80319814:
/* 80319814 00316754  7F A4 EB 78 */	mr r4, r29
lbl_80319818:
/* 80319818 00316758  38 04 00 18 */	addi r0, r4, 0x18
/* 8031981C 0031675C  93 C1 00 60 */	stw r30, 0x60(r1)
/* 80319820 00316760  38 A1 00 4C */	addi r5, r1, 0x4c
/* 80319824 00316764  90 01 00 50 */	stw r0, 0x50(r1)
/* 80319828 00316768  38 61 00 48 */	addi r3, r1, 0x48
/* 8031982C 0031676C  38 84 00 10 */	addi r4, r4, 0x10
/* 80319830 00316770  80 01 00 50 */	lwz r0, 0x50(r1)
/* 80319834 00316774  38 C1 00 60 */	addi r6, r1, 0x60
/* 80319838 00316778  90 01 00 4C */	stw r0, 0x4c(r1)
/* 8031983C 0031677C  4B FF FD 49 */	bl insert__Q27JGadget64TList$$0P13TAreaCylinder$$4Q27JGadget28TAllocator$$0P13TAreaCylinder$$1$$1FQ37JGadget64TList$$0P13TAreaCylinder$$4Q27JGadget28TAllocator$$0P13TAreaCylinder$$1$$18iteratorRCP13TAreaCylinder
/* 80319840 00316780  38 7F 00 00 */	addi r3, r31, 0
/* 80319844 00316784  38 81 00 5C */	addi r4, r1, 0x5c
/* 80319848 00316788  38 A0 00 04 */	li r5, 4
/* 8031984C 0031678C  4B CF 4D 45 */	bl read__14JSUInputStreamFPvl
/* 80319850 00316790  80 61 00 5C */	lwz r3, 0x5c(r1)
/* 80319854 00316794  3C 00 43 30 */	lis r0, 0x4330
/* 80319858 00316798  C8 42 FB C8 */	lfd f2, $$22140@sda21(r2)
/* 8031985C 0031679C  6C 63 80 00 */	xoris r3, r3, 0x8000
/* 80319860 003167A0  C0 02 FB C4 */	lfs f0, $$22138@sda21(r2)
/* 80319864 003167A4  90 61 00 8C */	stw r3, 0x8c(r1)
/* 80319868 003167A8  90 01 00 88 */	stw r0, 0x88(r1)
/* 8031986C 003167AC  C8 21 00 88 */	lfd f1, 0x88(r1)
/* 80319870 003167B0  EC 21 10 28 */	fsubs f1, f1, f2
/* 80319874 003167B4  EC 01 00 24 */	fdivs f0, f1, f0
/* 80319878 003167B8  D0 1E 00 24 */	stfs f0, 0x24(r30)
/* 8031987C 003167BC  80 01 00 A4 */	lwz r0, 0xa4(r1)
/* 80319880 003167C0  83 E1 00 9C */	lwz r31, 0x9c(r1)
/* 80319884 003167C4  83 C1 00 98 */	lwz r30, 0x98(r1)
/* 80319888 003167C8  83 A1 00 94 */	lwz r29, 0x94(r1)
/* 8031988C 003167CC  83 81 00 90 */	lwz r28, 0x90(r1)
/* 80319890 003167D0  38 21 00 A0 */	addi r1, r1, 0xa0
/* 80319894 003167D4  7C 08 03 A6 */	mtlr r0
/* 80319898 003167D8  4E 80 00 20 */	blr 

.global __ct__13TAreaCylinderFPCc
__ct__13TAreaCylinderFPCc:
/* 8031989C 003167DC  7C 08 02 A6 */	mflr r0
/* 803198A0 003167E0  3C A0 80 3B */	lis r5, __vt__Q26JDrama8TNameRef@ha
/* 803198A4 003167E4  90 01 00 04 */	stw r0, 4(r1)
/* 803198A8 003167E8  38 05 A8 60 */	addi r0, r5, __vt__Q26JDrama8TNameRef@l
/* 803198AC 003167EC  94 21 FF E0 */	stwu r1, -0x20(r1)
/* 803198B0 003167F0  93 E1 00 1C */	stw r31, 0x1c(r1)
/* 803198B4 003167F4  3B E3 00 00 */	addi r31, r3, 0
/* 803198B8 003167F8  90 03 00 00 */	stw r0, 0(r3)
/* 803198BC 003167FC  7C 83 23 78 */	mr r3, r4
/* 803198C0 00316800  90 9F 00 04 */	stw r4, 4(r31)
/* 803198C4 00316804  4B D2 AD C9 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 803198C8 00316808  B0 7F 00 08 */	sth r3, 8(r31)
/* 803198CC 0031680C  3C 60 80 3B */	lis r3, __vt__Q26JDrama8TViewObj@ha
/* 803198D0 00316810  38 03 A0 C0 */	addi r0, r3, __vt__Q26JDrama8TViewObj@l
/* 803198D4 00316814  90 1F 00 00 */	stw r0, 0(r31)
/* 803198D8 00316818  38 00 00 00 */	li r0, 0
/* 803198DC 0031681C  3C 60 80 3E */	lis r3, __vt__13TAreaCylinder@ha
/* 803198E0 00316820  B0 1F 00 0C */	sth r0, 0xc(r31)
/* 803198E4 00316824  38 03 0E 5C */	addi r0, r3, __vt__13TAreaCylinder@l
/* 803198E8 00316828  38 7F 00 00 */	addi r3, r31, 0
/* 803198EC 0031682C  90 1F 00 00 */	stw r0, 0(r31)
/* 803198F0 00316830  C0 02 FB D0 */	lfs f0, $$22149@sda21(r2)
/* 803198F4 00316834  D0 1F 00 18 */	stfs f0, 0x18(r31)
/* 803198F8 00316838  D0 1F 00 14 */	stfs f0, 0x14(r31)
/* 803198FC 0031683C  D0 1F 00 10 */	stfs f0, 0x10(r31)
/* 80319900 00316840  D0 1F 00 1C */	stfs f0, 0x1c(r31)
/* 80319904 00316844  80 01 00 24 */	lwz r0, 0x24(r1)
/* 80319908 00316848  83 E1 00 1C */	lwz r31, 0x1c(r1)
/* 8031990C 0031684C  38 21 00 20 */	addi r1, r1, 0x20
/* 80319910 00316850  7C 08 03 A6 */	mtlr r0
/* 80319914 00316854  4E 80 00 20 */	blr 

.global __dt__13TAreaCylinderFv
__dt__13TAreaCylinderFv:
/* 80319918 00316858  7C 08 02 A6 */	mflr r0
/* 8031991C 0031685C  90 01 00 04 */	stw r0, 4(r1)
/* 80319920 00316860  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 80319924 00316864  93 E1 00 14 */	stw r31, 0x14(r1)
/* 80319928 00316868  3B E4 00 00 */	addi r31, r4, 0
/* 8031992C 0031686C  93 C1 00 10 */	stw r30, 0x10(r1)
/* 80319930 00316870  7C 7E 1B 79 */	or. r30, r3, r3
/* 80319934 00316874  41 82 00 3C */	beq lbl_80319970
/* 80319938 00316878  3C 60 80 3E */	lis r3, __vt__13TAreaCylinder@ha
/* 8031993C 0031687C  38 03 0E 5C */	addi r0, r3, __vt__13TAreaCylinder@l
/* 80319940 00316880  90 1E 00 00 */	stw r0, 0(r30)
/* 80319944 00316884  41 82 00 1C */	beq lbl_80319960
/* 80319948 00316888  3C 60 80 3B */	lis r3, __vt__Q26JDrama8TViewObj@ha
/* 8031994C 0031688C  38 03 A0 C0 */	addi r0, r3, __vt__Q26JDrama8TViewObj@l
/* 80319950 00316890  90 1E 00 00 */	stw r0, 0(r30)
/* 80319954 00316894  38 7E 00 00 */	addi r3, r30, 0
/* 80319958 00316898  38 80 00 00 */	li r4, 0
/* 8031995C 0031689C  4B D2 AE A1 */	bl __dt__Q26JDrama8TNameRefFv
lbl_80319960:
/* 80319960 003168A0  7F E0 07 35 */	extsh. r0, r31
/* 80319964 003168A4  40 81 00 0C */	ble lbl_80319970
/* 80319968 003168A8  7F C3 F3 78 */	mr r3, r30
/* 8031996C 003168AC  4B CF 31 45 */	bl __dl__FPv
lbl_80319970:
/* 80319970 003168B0  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 80319974 003168B4  7F C3 F3 78 */	mr r3, r30
/* 80319978 003168B8  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 8031997C 003168BC  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 80319980 003168C0  7C 08 03 A6 */	mtlr r0
/* 80319984 003168C4  38 21 00 18 */	addi r1, r1, 0x18
/* 80319988 003168C8  4E 80 00 20 */	blr 

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global __vt__20TAreaCylinderManager
__vt__20TAreaCylinderManager:
  .4byte 0
  .4byte 0
  .4byte __dt__20TAreaCylinderManagerFv
  .4byte getType__Q26JDrama8TNameRefCFv
  .4byte load__Q26JDrama8TNameRefFR20JSUMemoryInputStream
  .4byte save__Q26JDrama8TNameRefFR21JSUMemoryOutputStream
  .4byte loadAfter__Q26JDrama8TNameRefFv
  .4byte searchF__Q26JDrama8TNameRefFUsPCc
  .4byte perform__20TAreaCylinderManagerFUlPQ26JDrama9TGraphics
.global __vt__13TAreaCylinder
__vt__13TAreaCylinder:
  .4byte 0
  .4byte 0
  .4byte __dt__13TAreaCylinderFv
  .4byte getType__Q26JDrama8TNameRefCFv
  .4byte load__13TAreaCylinderFR20JSUMemoryInputStream
  .4byte save__Q26JDrama8TNameRefFR21JSUMemoryOutputStream
  .4byte loadAfter__Q26JDrama8TNameRefFv
  .4byte searchF__Q26JDrama8TNameRefFUsPCc
  .4byte perform__13TAreaCylinderFUlPQ26JDrama9TGraphics

.section .sdata2, "wa"  # 0x8040B460 - 0x80414020
$$22137:
	.incbin "baserom.dol", 0x3EB8C0, 0x4
$$22138:
	.incbin "baserom.dol", 0x3EB8C4, 0x4
$$22140:
	.incbin "baserom.dol", 0x3EB8C8, 0x8
$$22149:
  .4byte 0
  .4byte 0
