.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global __ct__20TMapCheckGroundPlaneFv
__ct__20TMapCheckGroundPlaneFv:
/* 801F4458 001F1398  38 00 00 00 */	li r0, 0
/* 801F445C 001F139C  90 03 00 00 */	stw r0, 0(r3)
/* 801F4460 001F13A0  90 03 00 04 */	stw r0, 4(r3)
/* 801F4464 001F13A4  C0 02 D1 00 */	lfs f0, "@2346"@sda21(r2)
/* 801F4468 001F13A8  D0 03 00 08 */	stfs f0, 8(r3)
/* 801F446C 001F13AC  D0 03 00 0C */	stfs f0, 0xc(r3)
/* 801F4470 001F13B0  D0 03 00 10 */	stfs f0, 0x10(r3)
/* 801F4474 001F13B4  D0 03 00 14 */	stfs f0, 0x14(r3)
/* 801F4478 001F13B8  90 03 00 18 */	stw r0, 0x18(r3)
/* 801F447C 001F13BC  4E 80 00 20 */	blr 

.global init__20TMapCheckGroundPlaneFiif
init__20TMapCheckGroundPlaneFiif:
/* 801F4480 001F13C0  7C 08 02 A6 */	mflr r0
/* 801F4484 001F13C4  90 01 00 04 */	stw r0, 4(r1)
/* 801F4488 001F13C8  3C 00 43 30 */	lis r0, 0x4330
/* 801F448C 001F13CC  94 21 FF C8 */	stwu r1, -0x38(r1)
/* 801F4490 001F13D0  93 E1 00 34 */	stw r31, 0x34(r1)
/* 801F4494 001F13D4  93 C1 00 30 */	stw r30, 0x30(r1)
/* 801F4498 001F13D8  7C 7E 1B 78 */	mr r30, r3
/* 801F449C 001F13DC  90 83 00 00 */	stw r4, 0(r3)
/* 801F44A0 001F13E0  90 A3 00 04 */	stw r5, 4(r3)
/* 801F44A4 001F13E4  D0 23 00 08 */	stfs f1, 8(r3)
/* 801F44A8 001F13E8  C0 22 D1 04 */	lfs f1, "@2352"@sda21(r2)
/* 801F44AC 001F13EC  C0 03 00 08 */	lfs f0, 8(r3)
/* 801F44B0 001F13F0  EC 01 00 24 */	fdivs f0, f1, f0
/* 801F44B4 001F13F4  D0 03 00 0C */	stfs f0, 0xc(r3)
/* 801F44B8 001F13F8  80 63 00 00 */	lwz r3, 0(r3)
/* 801F44BC 001F13FC  C8 22 D1 10 */	lfd f1, "@2355"@sda21(r2)
/* 801F44C0 001F1400  6C 63 80 00 */	xoris r3, r3, 0x8000
/* 801F44C4 001F1404  C0 5E 00 08 */	lfs f2, 8(r30)
/* 801F44C8 001F1408  90 61 00 2C */	stw r3, 0x2c(r1)
/* 801F44CC 001F140C  90 01 00 28 */	stw r0, 0x28(r1)
/* 801F44D0 001F1410  C8 01 00 28 */	lfd f0, 0x28(r1)
/* 801F44D4 001F1414  EC 00 08 28 */	fsubs f0, f0, f1
/* 801F44D8 001F1418  EC 02 00 32 */	fmuls f0, f2, f0
/* 801F44DC 001F141C  D0 1E 00 10 */	stfs f0, 0x10(r30)
/* 801F44E0 001F1420  C0 3E 00 10 */	lfs f1, 0x10(r30)
/* 801F44E4 001F1424  C0 02 D1 08 */	lfs f0, "@2353"@sda21(r2)
/* 801F44E8 001F1428  EC 01 00 32 */	fmuls f0, f1, f0
/* 801F44EC 001F142C  D0 1E 00 14 */	stfs f0, 0x14(r30)
/* 801F44F0 001F1430  80 7E 00 04 */	lwz r3, 4(r30)
/* 801F44F4 001F1434  80 1E 00 00 */	lwz r0, 0(r30)
/* 801F44F8 001F1438  7C 03 01 D6 */	mullw r0, r3, r0
/* 801F44FC 001F143C  54 1F 08 3C */	slwi r31, r0, 1
/* 801F4500 001F1440  1C 7F 00 48 */	mulli r3, r31, 0x48
/* 801F4504 001F1444  38 63 00 08 */	addi r3, r3, 8
/* 801F4508 001F1448  4B E1 84 A9 */	bl __nwa__FUl
/* 801F450C 001F144C  3C 80 80 1F */	lis r4, __ct__12TBGCheckDataFv@ha
/* 801F4510 001F1450  38 84 4B C8 */	addi r4, r4, __ct__12TBGCheckDataFv@l
/* 801F4514 001F1454  38 FF 00 00 */	addi r7, r31, 0
/* 801F4518 001F1458  38 A0 00 00 */	li r5, 0
/* 801F451C 001F145C  38 C0 00 48 */	li r6, 0x48
/* 801F4520 001F1460  4B E8 E4 CD */	bl __construct_new_array
/* 801F4524 001F1464  90 7E 00 18 */	stw r3, 0x18(r30)
/* 801F4528 001F1468  80 6D 9A B8 */	lwz r3, gpMapCollisionData@sda21(r13)
/* 801F452C 001F146C  93 C3 02 44 */	stw r30, 0x244(r3)
/* 801F4530 001F1470  80 01 00 3C */	lwz r0, 0x3c(r1)
/* 801F4534 001F1474  83 E1 00 34 */	lwz r31, 0x34(r1)
/* 801F4538 001F1478  83 C1 00 30 */	lwz r30, 0x30(r1)
/* 801F453C 001F147C  7C 08 03 A6 */	mtlr r0
/* 801F4540 001F1480  38 21 00 38 */	addi r1, r1, 0x38
/* 801F4544 001F1484  4E 80 00 20 */	blr 

.global checkPlaneGround__20TMapCheckGroundPlaneFfffPPC12TBGCheckData
checkPlaneGround__20TMapCheckGroundPlaneFfffPPC12TBGCheckData:
/* 801F4548 001F1488  94 21 FF 78 */	stwu r1, -0x88(r1)
/* 801F454C 001F148C  C0 C3 00 14 */	lfs f6, 0x14(r3)
/* 801F4550 001F1490  FC 00 30 50 */	fneg f0, f6
/* 801F4554 001F1494  FC 01 00 40 */	fcmpo cr0, f1, f0
/* 801F4558 001F1498  41 80 00 1C */	blt lbl_801F4574
/* 801F455C 001F149C  FC 06 08 40 */	fcmpo cr0, f6, f1
/* 801F4560 001F14A0  41 80 00 14 */	blt lbl_801F4574
/* 801F4564 001F14A4  FC 03 00 40 */	fcmpo cr0, f3, f0
/* 801F4568 001F14A8  41 80 00 0C */	blt lbl_801F4574
/* 801F456C 001F14AC  FC 06 18 40 */	fcmpo cr0, f6, f3
/* 801F4570 001F14B0  40 80 00 18 */	bge lbl_801F4588
lbl_801F4574:
/* 801F4574 001F14B4  3C 60 80 3F */	lis r3, mIllegalCheckData__17TMapCollisionData@ha
/* 801F4578 001F14B8  38 03 79 10 */	addi r0, r3, mIllegalCheckData__17TMapCollisionData@l
/* 801F457C 001F14BC  90 04 00 00 */	stw r0, 0(r4)
/* 801F4580 001F14C0  C0 22 D1 18 */	lfs f1, "@2385"@sda21(r2)
/* 801F4584 001F14C4  48 00 00 F0 */	b lbl_801F4674
lbl_801F4588:
/* 801F4588 001F14C8  EC 41 30 2A */	fadds f2, f1, f6
/* 801F458C 001F14CC  C0 A3 00 0C */	lfs f5, 0xc(r3)
/* 801F4590 001F14D0  EC 03 30 2A */	fadds f0, f3, f6
/* 801F4594 001F14D4  3C A0 43 30 */	lis r5, 0x4330
/* 801F4598 001F14D8  C8 82 D1 10 */	lfd f4, "@2355"@sda21(r2)
/* 801F459C 001F14DC  EC 45 00 B2 */	fmuls f2, f5, f2
/* 801F45A0 001F14E0  C0 E3 00 08 */	lfs f7, 8(r3)
/* 801F45A4 001F14E4  EC A5 00 32 */	fmuls f5, f5, f0
/* 801F45A8 001F14E8  FC 00 10 1E */	fctiwz f0, f2
/* 801F45AC 001F14EC  FC A0 28 1E */	fctiwz f5, f5
/* 801F45B0 001F14F0  D8 01 00 80 */	stfd f0, 0x80(r1)
/* 801F45B4 001F14F4  80 C1 00 84 */	lwz r6, 0x84(r1)
/* 801F45B8 001F14F8  D8 A1 00 68 */	stfd f5, 0x68(r1)
/* 801F45BC 001F14FC  6C C0 80 00 */	xoris r0, r6, 0x8000
/* 801F45C0 001F1500  90 01 00 74 */	stw r0, 0x74(r1)
/* 801F45C4 001F1504  80 01 00 6C */	lwz r0, 0x6c(r1)
/* 801F45C8 001F1508  90 A1 00 70 */	stw r5, 0x70(r1)
/* 801F45CC 001F150C  6C 00 80 00 */	xoris r0, r0, 0x8000
/* 801F45D0 001F1510  90 01 00 64 */	stw r0, 0x64(r1)
/* 801F45D4 001F1514  C8 01 00 70 */	lfd f0, 0x70(r1)
/* 801F45D8 001F1518  90 A1 00 60 */	stw r5, 0x60(r1)
/* 801F45DC 001F151C  EC 40 20 28 */	fsubs f2, f0, f4
/* 801F45E0 001F1520  C8 01 00 60 */	lfd f0, 0x60(r1)
/* 801F45E4 001F1524  D8 A1 00 78 */	stfd f5, 0x78(r1)
/* 801F45E8 001F1528  EC 42 31 F8 */	fmsubs f2, f2, f7, f6
/* 801F45EC 001F152C  EC 00 20 28 */	fsubs f0, f0, f4
/* 801F45F0 001F1530  80 E1 00 7C */	lwz r7, 0x7c(r1)
/* 801F45F4 001F1534  EC 41 10 28 */	fsubs f2, f1, f2
/* 801F45F8 001F1538  EC 00 31 F8 */	fmsubs f0, f0, f7, f6
/* 801F45FC 001F153C  EC 47 10 28 */	fsubs f2, f7, f2
/* 801F4600 001F1540  EC 03 00 28 */	fsubs f0, f3, f0
/* 801F4604 001F1544  FC 02 00 40 */	fcmpo cr0, f2, f0
/* 801F4608 001F1548  40 81 00 24 */	ble lbl_801F462C
/* 801F460C 001F154C  80 03 00 04 */	lwz r0, 4(r3)
/* 801F4610 001F1550  80 63 00 18 */	lwz r3, 0x18(r3)
/* 801F4614 001F1554  7C 07 01 D6 */	mullw r0, r7, r0
/* 801F4618 001F1558  7C 06 02 14 */	add r0, r6, r0
/* 801F461C 001F155C  54 00 08 3C */	slwi r0, r0, 1
/* 801F4620 001F1560  1C 00 00 48 */	mulli r0, r0, 0x48
/* 801F4624 001F1564  7C 63 02 14 */	add r3, r3, r0
/* 801F4628 001F1568  48 00 00 24 */	b lbl_801F464C
lbl_801F462C:
/* 801F462C 001F156C  80 03 00 04 */	lwz r0, 4(r3)
/* 801F4630 001F1570  80 A3 00 18 */	lwz r5, 0x18(r3)
/* 801F4634 001F1574  7C 07 01 D6 */	mullw r0, r7, r0
/* 801F4638 001F1578  7C 06 02 14 */	add r0, r6, r0
/* 801F463C 001F157C  54 03 08 3C */	slwi r3, r0, 1
/* 801F4640 001F1580  38 03 00 01 */	addi r0, r3, 1
/* 801F4644 001F1584  1C 00 00 48 */	mulli r0, r0, 0x48
/* 801F4648 001F1588  7C 65 02 14 */	add r3, r5, r0
lbl_801F464C:
/* 801F464C 001F158C  C0 03 00 3C */	lfs f0, 0x3c(r3)
/* 801F4650 001F1590  C0 83 00 34 */	lfs f4, 0x34(r3)
/* 801F4654 001F1594  EC 43 00 32 */	fmuls f2, f3, f0
/* 801F4658 001F1598  C0 63 00 40 */	lfs f3, 0x40(r3)
/* 801F465C 001F159C  C0 03 00 38 */	lfs f0, 0x38(r3)
/* 801F4660 001F15A0  EC 21 11 3A */	fmadds f1, f1, f4, f2
/* 801F4664 001F15A4  90 64 00 00 */	stw r3, 0(r4)
/* 801F4668 001F15A8  EC 23 08 2A */	fadds f1, f3, f1
/* 801F466C 001F15AC  FC 20 08 50 */	fneg f1, f1
/* 801F4670 001F15B0  EC 21 00 24 */	fdivs f1, f1, f0
lbl_801F4674:
/* 801F4674 001F15B4  38 21 00 88 */	addi r1, r1, 0x88
/* 801F4678 001F15B8  4E 80 00 20 */	blr 

.global getCheckData__20TMapCheckGroundPlaneCFiii
getCheckData__20TMapCheckGroundPlaneCFiii:
/* 801F467C 001F15BC  80 03 00 04 */	lwz r0, 4(r3)
/* 801F4680 001F15C0  80 63 00 18 */	lwz r3, 0x18(r3)
/* 801F4684 001F15C4  7C 05 01 D6 */	mullw r0, r5, r0
/* 801F4688 001F15C8  7C 04 02 14 */	add r0, r4, r0
/* 801F468C 001F15CC  54 00 08 3C */	slwi r0, r0, 1
/* 801F4690 001F15D0  7C 06 02 14 */	add r0, r6, r0
/* 801F4694 001F15D4  1C 00 00 48 */	mulli r0, r0, 0x48
/* 801F4698 001F15D8  7C 63 02 14 */	add r3, r3, r0
/* 801F469C 001F15DC  4E 80 00 20 */	blr 

.global __sinit_MapCollisionPlane_cpp
__sinit_MapCollisionPlane_cpp:
/* 801F46A0 001F15E0  7C 08 02 A6 */	mflr r0
/* 801F46A4 001F15E4  3C 60 80 40 */	lis r3, "@2330"@ha
/* 801F46A8 001F15E8  90 01 00 04 */	stw r0, 4(r1)
/* 801F46AC 001F15EC  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 801F46B0 001F15F0  93 E1 00 0C */	stw r31, 0xc(r1)
/* 801F46B4 001F15F4  3B E3 89 A0 */	addi r31, r3, "@2330"@l
/* 801F46B8 001F15F8  88 0D 97 CC */	lbz r0, "__init__smList__15JALList<5MSBgm>"@sda21(r13)
/* 801F46BC 001F15FC  7C 00 07 75 */	extsb. r0, r0
/* 801F46C0 001F1600  40 82 00 28 */	bne lbl_801F46E8
/* 801F46C4 001F1604  38 6D 97 A8 */	addi r3, r13, "smList__15JALList<5MSBgm>"@sda21
/* 801F46C8 001F1608  4B E1 A5 6D */	bl initiate__10JSUPtrListFv
/* 801F46CC 001F160C  3C 60 80 0E */	lis r3, "__dt__15JSUList<5MSBgm>Fv"@ha
/* 801F46D0 001F1610  38 83 6A 44 */	addi r4, r3, "__dt__15JSUList<5MSBgm>Fv"@l
/* 801F46D4 001F1614  38 6D 97 A8 */	addi r3, r13, "smList__15JALList<5MSBgm>"@sda21
/* 801F46D8 001F1618  38 BF 00 00 */	addi r5, r31, 0
/* 801F46DC 001F161C  4B E8 E0 4D */	bl __register_global_object
/* 801F46E0 001F1620  38 00 00 01 */	li r0, 1
/* 801F46E4 001F1624  98 0D 97 CC */	stb r0, "__init__smList__15JALList<5MSBgm>"@sda21(r13)
lbl_801F46E8:
/* 801F46E8 001F1628  88 0D 97 CD */	lbz r0, "__init__smList__24JALList<13MSSetSoundGrp>"@sda21(r13)
/* 801F46EC 001F162C  7C 00 07 75 */	extsb. r0, r0
/* 801F46F0 001F1630  40 82 00 28 */	bne lbl_801F4718
/* 801F46F4 001F1634  38 6D 97 B4 */	addi r3, r13, "smList__24JALList<13MSSetSoundGrp>"@sda21
/* 801F46F8 001F1638  4B E1 A5 3D */	bl initiate__10JSUPtrListFv
/* 801F46FC 001F163C  3C 60 80 0E */	lis r3, "__dt__24JSUList<13MSSetSoundGrp>Fv"@ha
/* 801F4700 001F1640  38 83 69 EC */	addi r4, r3, "__dt__24JSUList<13MSSetSoundGrp>Fv"@l
/* 801F4704 001F1644  38 6D 97 B4 */	addi r3, r13, "smList__24JALList<13MSSetSoundGrp>"@sda21
/* 801F4708 001F1648  38 BF 00 0C */	addi r5, r31, 0xc
/* 801F470C 001F164C  4B E8 E0 1D */	bl __register_global_object
/* 801F4710 001F1650  38 00 00 01 */	li r0, 1
/* 801F4714 001F1654  98 0D 97 CD */	stb r0, "__init__smList__24JALList<13MSSetSoundGrp>"@sda21(r13)
lbl_801F4718:
/* 801F4718 001F1658  88 0D 97 CE */	lbz r0, "__init__smList__21JALList<10MSSetSound>"@sda21(r13)
/* 801F471C 001F165C  7C 00 07 75 */	extsb. r0, r0
/* 801F4720 001F1660  40 82 00 28 */	bne lbl_801F4748
/* 801F4724 001F1664  38 6D 97 C0 */	addi r3, r13, "smList__21JALList<10MSSetSound>"@sda21
/* 801F4728 001F1668  4B E1 A5 0D */	bl initiate__10JSUPtrListFv
/* 801F472C 001F166C  3C 60 80 0E */	lis r3, "__dt__21JSUList<10MSSetSound>Fv"@ha
/* 801F4730 001F1670  38 83 69 94 */	addi r4, r3, "__dt__21JSUList<10MSSetSound>Fv"@l
/* 801F4734 001F1674  38 6D 97 C0 */	addi r3, r13, "smList__21JALList<10MSSetSound>"@sda21
/* 801F4738 001F1678  38 BF 00 18 */	addi r5, r31, 0x18
/* 801F473C 001F167C  4B E8 DF ED */	bl __register_global_object
/* 801F4740 001F1680  38 00 00 01 */	li r0, 1
/* 801F4744 001F1684  98 0D 97 CE */	stb r0, "__init__smList__21JALList<10MSSetSound>"@sda21(r13)
lbl_801F4748:
/* 801F4748 001F1688  88 0D 8F 8C */	lbz r0, "__init__smList__26JALList<15JALSeModEffDGrp>"@sda21(r13)
/* 801F474C 001F168C  7C 00 07 75 */	extsb. r0, r0
/* 801F4750 001F1690  40 82 00 28 */	bne lbl_801F4778
/* 801F4754 001F1694  38 6D 8E FC */	addi r3, r13, "smList__26JALList<15JALSeModEffDGrp>"@sda21
/* 801F4758 001F1698  4B E1 A4 DD */	bl initiate__10JSUPtrListFv
/* 801F475C 001F169C  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModEffDGrp>Fv"@ha
/* 801F4760 001F16A0  38 83 A8 0C */	addi r4, r3, "__dt__26JSUList<15JALSeModEffDGrp>Fv"@l
/* 801F4764 001F16A4  38 6D 8E FC */	addi r3, r13, "smList__26JALList<15JALSeModEffDGrp>"@sda21
/* 801F4768 001F16A8  38 BF 00 24 */	addi r5, r31, 0x24
/* 801F476C 001F16AC  4B E8 DF BD */	bl __register_global_object
/* 801F4770 001F16B0  38 00 00 01 */	li r0, 1
/* 801F4774 001F16B4  98 0D 8F 8C */	stb r0, "__init__smList__26JALList<15JALSeModEffDGrp>"@sda21(r13)
lbl_801F4778:
/* 801F4778 001F16B8  88 0D 8F 8D */	lbz r0, "__init__smList__26JALList<15JALSeModPitDGrp>"@sda21(r13)
/* 801F477C 001F16BC  7C 00 07 75 */	extsb. r0, r0
/* 801F4780 001F16C0  40 82 00 28 */	bne lbl_801F47A8
/* 801F4784 001F16C4  38 6D 8F 08 */	addi r3, r13, "smList__26JALList<15JALSeModPitDGrp>"@sda21
/* 801F4788 001F16C8  4B E1 A4 AD */	bl initiate__10JSUPtrListFv
/* 801F478C 001F16CC  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModPitDGrp>Fv"@ha
/* 801F4790 001F16D0  38 83 A7 B4 */	addi r4, r3, "__dt__26JSUList<15JALSeModPitDGrp>Fv"@l
/* 801F4794 001F16D4  38 6D 8F 08 */	addi r3, r13, "smList__26JALList<15JALSeModPitDGrp>"@sda21
/* 801F4798 001F16D8  38 BF 00 30 */	addi r5, r31, 0x30
/* 801F479C 001F16DC  4B E8 DF 8D */	bl __register_global_object
/* 801F47A0 001F16E0  38 00 00 01 */	li r0, 1
/* 801F47A4 001F16E4  98 0D 8F 8D */	stb r0, "__init__smList__26JALList<15JALSeModPitDGrp>"@sda21(r13)
lbl_801F47A8:
/* 801F47A8 001F16E8  88 0D 8F 8E */	lbz r0, "__init__smList__26JALList<15JALSeModVolDGrp>"@sda21(r13)
/* 801F47AC 001F16EC  7C 00 07 75 */	extsb. r0, r0
/* 801F47B0 001F16F0  40 82 00 28 */	bne lbl_801F47D8
/* 801F47B4 001F16F4  38 6D 8F 14 */	addi r3, r13, "smList__26JALList<15JALSeModVolDGrp>"@sda21
/* 801F47B8 001F16F8  4B E1 A4 7D */	bl initiate__10JSUPtrListFv
/* 801F47BC 001F16FC  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModVolDGrp>Fv"@ha
/* 801F47C0 001F1700  38 83 A7 5C */	addi r4, r3, "__dt__26JSUList<15JALSeModVolDGrp>Fv"@l
/* 801F47C4 001F1704  38 6D 8F 14 */	addi r3, r13, "smList__26JALList<15JALSeModVolDGrp>"@sda21
/* 801F47C8 001F1708  38 BF 00 3C */	addi r5, r31, 0x3c
/* 801F47CC 001F170C  4B E8 DF 5D */	bl __register_global_object
/* 801F47D0 001F1710  38 00 00 01 */	li r0, 1
/* 801F47D4 001F1714  98 0D 8F 8E */	stb r0, "__init__smList__26JALList<15JALSeModVolDGrp>"@sda21(r13)
lbl_801F47D8:
/* 801F47D8 001F1718  88 0D 8F 8F */	lbz r0, "__init__smList__26JALList<15JALSeModEffFGrp>"@sda21(r13)
/* 801F47DC 001F171C  7C 00 07 75 */	extsb. r0, r0
/* 801F47E0 001F1720  40 82 00 28 */	bne lbl_801F4808
/* 801F47E4 001F1724  38 6D 8F 20 */	addi r3, r13, "smList__26JALList<15JALSeModEffFGrp>"@sda21
/* 801F47E8 001F1728  4B E1 A4 4D */	bl initiate__10JSUPtrListFv
/* 801F47EC 001F172C  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModEffFGrp>Fv"@ha
/* 801F47F0 001F1730  38 83 A7 04 */	addi r4, r3, "__dt__26JSUList<15JALSeModEffFGrp>Fv"@l
/* 801F47F4 001F1734  38 6D 8F 20 */	addi r3, r13, "smList__26JALList<15JALSeModEffFGrp>"@sda21
/* 801F47F8 001F1738  38 BF 00 48 */	addi r5, r31, 0x48
/* 801F47FC 001F173C  4B E8 DF 2D */	bl __register_global_object
/* 801F4800 001F1740  38 00 00 01 */	li r0, 1
/* 801F4804 001F1744  98 0D 8F 8F */	stb r0, "__init__smList__26JALList<15JALSeModEffFGrp>"@sda21(r13)
lbl_801F4808:
/* 801F4808 001F1748  88 0D 8F 90 */	lbz r0, "__init__smList__26JALList<15JALSeModPitFGrp>"@sda21(r13)
/* 801F480C 001F174C  7C 00 07 75 */	extsb. r0, r0
/* 801F4810 001F1750  40 82 00 28 */	bne lbl_801F4838
/* 801F4814 001F1754  38 6D 8F 2C */	addi r3, r13, "smList__26JALList<15JALSeModPitFGrp>"@sda21
/* 801F4818 001F1758  4B E1 A4 1D */	bl initiate__10JSUPtrListFv
/* 801F481C 001F175C  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModPitFGrp>Fv"@ha
/* 801F4820 001F1760  38 83 A6 AC */	addi r4, r3, "__dt__26JSUList<15JALSeModPitFGrp>Fv"@l
/* 801F4824 001F1764  38 6D 8F 2C */	addi r3, r13, "smList__26JALList<15JALSeModPitFGrp>"@sda21
/* 801F4828 001F1768  38 BF 00 54 */	addi r5, r31, 0x54
/* 801F482C 001F176C  4B E8 DE FD */	bl __register_global_object
/* 801F4830 001F1770  38 00 00 01 */	li r0, 1
/* 801F4834 001F1774  98 0D 8F 90 */	stb r0, "__init__smList__26JALList<15JALSeModPitFGrp>"@sda21(r13)
lbl_801F4838:
/* 801F4838 001F1778  88 0D 8F 91 */	lbz r0, "__init__smList__26JALList<15JALSeModVolFGrp>"@sda21(r13)
/* 801F483C 001F177C  7C 00 07 75 */	extsb. r0, r0
/* 801F4840 001F1780  40 82 00 28 */	bne lbl_801F4868
/* 801F4844 001F1784  38 6D 8F 38 */	addi r3, r13, "smList__26JALList<15JALSeModVolFGrp>"@sda21
/* 801F4848 001F1788  4B E1 A3 ED */	bl initiate__10JSUPtrListFv
/* 801F484C 001F178C  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModVolFGrp>Fv"@ha
/* 801F4850 001F1790  38 83 A6 54 */	addi r4, r3, "__dt__26JSUList<15JALSeModVolFGrp>Fv"@l
/* 801F4854 001F1794  38 6D 8F 38 */	addi r3, r13, "smList__26JALList<15JALSeModVolFGrp>"@sda21
/* 801F4858 001F1798  38 BF 00 60 */	addi r5, r31, 0x60
/* 801F485C 001F179C  4B E8 DE CD */	bl __register_global_object
/* 801F4860 001F17A0  38 00 00 01 */	li r0, 1
/* 801F4864 001F17A4  98 0D 8F 91 */	stb r0, "__init__smList__26JALList<15JALSeModVolFGrp>"@sda21(r13)
lbl_801F4868:
/* 801F4868 001F17A8  88 0D 8F 92 */	lbz r0, "__init__smList__26JALList<15JALSeModEffDist>"@sda21(r13)
/* 801F486C 001F17AC  7C 00 07 75 */	extsb. r0, r0
/* 801F4870 001F17B0  40 82 00 28 */	bne lbl_801F4898
/* 801F4874 001F17B4  38 6D 8F 44 */	addi r3, r13, "smList__26JALList<15JALSeModEffDist>"@sda21
/* 801F4878 001F17B8  4B E1 A3 BD */	bl initiate__10JSUPtrListFv
/* 801F487C 001F17BC  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModEffDist>Fv"@ha
/* 801F4880 001F17C0  38 83 A5 FC */	addi r4, r3, "__dt__26JSUList<15JALSeModEffDist>Fv"@l
/* 801F4884 001F17C4  38 6D 8F 44 */	addi r3, r13, "smList__26JALList<15JALSeModEffDist>"@sda21
/* 801F4888 001F17C8  38 BF 00 6C */	addi r5, r31, 0x6c
/* 801F488C 001F17CC  4B E8 DE 9D */	bl __register_global_object
/* 801F4890 001F17D0  38 00 00 01 */	li r0, 1
/* 801F4894 001F17D4  98 0D 8F 92 */	stb r0, "__init__smList__26JALList<15JALSeModEffDist>"@sda21(r13)
lbl_801F4898:
/* 801F4898 001F17D8  88 0D 8F 93 */	lbz r0, "__init__smList__26JALList<15JALSeModPitDist>"@sda21(r13)
/* 801F489C 001F17DC  7C 00 07 75 */	extsb. r0, r0
/* 801F48A0 001F17E0  40 82 00 28 */	bne lbl_801F48C8
/* 801F48A4 001F17E4  38 6D 8F 50 */	addi r3, r13, "smList__26JALList<15JALSeModPitDist>"@sda21
/* 801F48A8 001F17E8  4B E1 A3 8D */	bl initiate__10JSUPtrListFv
/* 801F48AC 001F17EC  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModPitDist>Fv"@ha
/* 801F48B0 001F17F0  38 83 A5 A4 */	addi r4, r3, "__dt__26JSUList<15JALSeModPitDist>Fv"@l
/* 801F48B4 001F17F4  38 6D 8F 50 */	addi r3, r13, "smList__26JALList<15JALSeModPitDist>"@sda21
/* 801F48B8 001F17F8  38 BF 00 78 */	addi r5, r31, 0x78
/* 801F48BC 001F17FC  4B E8 DE 6D */	bl __register_global_object
/* 801F48C0 001F1800  38 00 00 01 */	li r0, 1
/* 801F48C4 001F1804  98 0D 8F 93 */	stb r0, "__init__smList__26JALList<15JALSeModPitDist>"@sda21(r13)
lbl_801F48C8:
/* 801F48C8 001F1808  88 0D 8F 94 */	lbz r0, "__init__smList__26JALList<15JALSeModVolDist>"@sda21(r13)
/* 801F48CC 001F180C  7C 00 07 75 */	extsb. r0, r0
/* 801F48D0 001F1810  40 82 00 28 */	bne lbl_801F48F8
/* 801F48D4 001F1814  38 6D 8F 5C */	addi r3, r13, "smList__26JALList<15JALSeModVolDist>"@sda21
/* 801F48D8 001F1818  4B E1 A3 5D */	bl initiate__10JSUPtrListFv
/* 801F48DC 001F181C  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModVolDist>Fv"@ha
/* 801F48E0 001F1820  38 83 A5 4C */	addi r4, r3, "__dt__26JSUList<15JALSeModVolDist>Fv"@l
/* 801F48E4 001F1824  38 6D 8F 5C */	addi r3, r13, "smList__26JALList<15JALSeModVolDist>"@sda21
/* 801F48E8 001F1828  38 BF 00 84 */	addi r5, r31, 0x84
/* 801F48EC 001F182C  4B E8 DE 3D */	bl __register_global_object
/* 801F48F0 001F1830  38 00 00 01 */	li r0, 1
/* 801F48F4 001F1834  98 0D 8F 94 */	stb r0, "__init__smList__26JALList<15JALSeModVolDist>"@sda21(r13)
lbl_801F48F8:
/* 801F48F8 001F1838  88 0D 8F 95 */	lbz r0, "__init__smList__26JALList<15JALSeModEffFunk>"@sda21(r13)
/* 801F48FC 001F183C  7C 00 07 75 */	extsb. r0, r0
/* 801F4900 001F1840  40 82 00 28 */	bne lbl_801F4928
/* 801F4904 001F1844  38 6D 8F 68 */	addi r3, r13, "smList__26JALList<15JALSeModEffFunk>"@sda21
/* 801F4908 001F1848  4B E1 A3 2D */	bl initiate__10JSUPtrListFv
/* 801F490C 001F184C  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModEffFunk>Fv"@ha
/* 801F4910 001F1850  38 83 A4 F4 */	addi r4, r3, "__dt__26JSUList<15JALSeModEffFunk>Fv"@l
/* 801F4914 001F1854  38 6D 8F 68 */	addi r3, r13, "smList__26JALList<15JALSeModEffFunk>"@sda21
/* 801F4918 001F1858  38 BF 00 90 */	addi r5, r31, 0x90
/* 801F491C 001F185C  4B E8 DE 0D */	bl __register_global_object
/* 801F4920 001F1860  38 00 00 01 */	li r0, 1
/* 801F4924 001F1864  98 0D 8F 95 */	stb r0, "__init__smList__26JALList<15JALSeModEffFunk>"@sda21(r13)
lbl_801F4928:
/* 801F4928 001F1868  88 0D 8F 96 */	lbz r0, "__init__smList__26JALList<15JALSeModPitFunk>"@sda21(r13)
/* 801F492C 001F186C  7C 00 07 75 */	extsb. r0, r0
/* 801F4930 001F1870  40 82 00 28 */	bne lbl_801F4958
/* 801F4934 001F1874  38 6D 8F 74 */	addi r3, r13, "smList__26JALList<15JALSeModPitFunk>"@sda21
/* 801F4938 001F1878  4B E1 A2 FD */	bl initiate__10JSUPtrListFv
/* 801F493C 001F187C  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModPitFunk>Fv"@ha
/* 801F4940 001F1880  38 83 A4 9C */	addi r4, r3, "__dt__26JSUList<15JALSeModPitFunk>Fv"@l
/* 801F4944 001F1884  38 6D 8F 74 */	addi r3, r13, "smList__26JALList<15JALSeModPitFunk>"@sda21
/* 801F4948 001F1888  38 BF 00 9C */	addi r5, r31, 0x9c
/* 801F494C 001F188C  4B E8 DD DD */	bl __register_global_object
/* 801F4950 001F1890  38 00 00 01 */	li r0, 1
/* 801F4954 001F1894  98 0D 8F 96 */	stb r0, "__init__smList__26JALList<15JALSeModPitFunk>"@sda21(r13)
lbl_801F4958:
/* 801F4958 001F1898  88 0D 8F 97 */	lbz r0, "__init__smList__26JALList<15JALSeModVolFunk>"@sda21(r13)
/* 801F495C 001F189C  7C 00 07 75 */	extsb. r0, r0
/* 801F4960 001F18A0  40 82 00 28 */	bne lbl_801F4988
/* 801F4964 001F18A4  38 6D 8F 80 */	addi r3, r13, "smList__26JALList<15JALSeModVolFunk>"@sda21
/* 801F4968 001F18A8  4B E1 A2 CD */	bl initiate__10JSUPtrListFv
/* 801F496C 001F18AC  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModVolFunk>Fv"@ha
/* 801F4970 001F18B0  38 83 A4 44 */	addi r4, r3, "__dt__26JSUList<15JALSeModVolFunk>Fv"@l
/* 801F4974 001F18B4  38 6D 8F 80 */	addi r3, r13, "smList__26JALList<15JALSeModVolFunk>"@sda21
/* 801F4978 001F18B8  38 BF 00 A8 */	addi r5, r31, 0xa8
/* 801F497C 001F18BC  4B E8 DD AD */	bl __register_global_object
/* 801F4980 001F18C0  38 00 00 01 */	li r0, 1
/* 801F4984 001F18C4  98 0D 8F 97 */	stb r0, "__init__smList__26JALList<15JALSeModVolFunk>"@sda21(r13)
lbl_801F4988:
/* 801F4988 001F18C8  80 01 00 14 */	lwz r0, 0x14(r1)
/* 801F498C 001F18CC  83 E1 00 0C */	lwz r31, 0xc(r1)
/* 801F4990 001F18D0  38 21 00 10 */	addi r1, r1, 0x10
/* 801F4994 001F18D4  7C 08 03 A6 */	mtlr r0
/* 801F4998 001F18D8  4E 80 00 20 */	blr 

.section .ctors, "wa"  # 0x8036FBA0 - 0x8036FF80
lbl_constructor:
  .4byte __sinit_MapCollisionPlane_cpp

.section .sdata2, "a"  # 0x8040B460 - 0x80414020
.balign 8
"@2346":
  .4byte 0
"@2352":
	.incbin "baserom.dol", 0x3E8E04, 0x4
"@2353":
	.incbin "baserom.dol", 0x3E8E08, 0x8
"@2355":
	.incbin "baserom.dol", 0x3E8E10, 0x8
"@2385":
	.incbin "baserom.dol", 0x3E8E18, 0x8

.section .bss, "wa"  # 0x803E6000 - 0x80408AC0
"@2330":
	.skip 0xC
"@2331":
	.skip 0xC
"@2332":
	.skip 0xC
"@2333":
	.skip 0xC
"@2334":
	.skip 0xC
"@2335":
	.skip 0xC
"@2336":
	.skip 0xC
"@2337":
	.skip 0xC
"@2338":
	.skip 0xC
"@2339":
	.skip 0xC
"@2340":
	.skip 0xC
"@2341":
	.skip 0xC
"@2342":
	.skip 0xC
"@2343":
	.skip 0xC
"@2344":
	.skip 0x10
