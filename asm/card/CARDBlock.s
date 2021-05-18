.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global __CARDGetFatBlock
__CARDGetFatBlock:
/* 800A13C8 0009E308  80 63 00 88 */	lwz r3, 0x88(r3)
/* 800A13CC 0009E30C  4E 80 00 20 */	blr 

WriteCallback:
/* 800A13D0 0009E310  7C 08 02 A6 */	mflr r0
/* 800A13D4 0009E314  90 01 00 04 */	stw r0, 4(r1)
/* 800A13D8 0009E318  94 21 FF E0 */	stwu r1, -0x20(r1)
/* 800A13DC 0009E31C  93 E1 00 1C */	stw r31, 0x1c(r1)
/* 800A13E0 0009E320  93 C1 00 18 */	stw r30, 0x18(r1)
/* 800A13E4 0009E324  7C 9E 23 79 */	or. r30, r4, r4
/* 800A13E8 0009E328  93 A1 00 14 */	stw r29, 0x14(r1)
/* 800A13EC 0009E32C  3B A3 00 00 */	addi r29, r3, 0
/* 800A13F0 0009E330  1C BD 01 10 */	mulli r5, r29, 0x110
/* 800A13F4 0009E334  3C 60 80 3F */	lis r3, __CARDBlock@ha
/* 800A13F8 0009E338  38 03 BF 60 */	addi r0, r3, __CARDBlock@l
/* 800A13FC 0009E33C  7F E0 2A 14 */	add r31, r0, r5
/* 800A1400 0009E340  41 80 00 48 */	blt lbl_800A1448
/* 800A1404 0009E344  80 7F 00 80 */	lwz r3, 0x80(r31)
/* 800A1408 0009E348  80 1F 00 88 */	lwz r0, 0x88(r31)
/* 800A140C 0009E34C  38 83 60 00 */	addi r4, r3, 0x6000
/* 800A1410 0009E350  7C 00 20 40 */	cmplw r0, r4
/* 800A1414 0009E354  3C A3 00 01 */	addis r5, r3, 1
/* 800A1418 0009E358  38 A5 80 00 */	addi r5, r5, -32768
/* 800A141C 0009E35C  40 82 00 18 */	bne lbl_800A1434
/* 800A1420 0009E360  90 BF 00 88 */	stw r5, 0x88(r31)
/* 800A1424 0009E364  38 65 00 00 */	addi r3, r5, 0
/* 800A1428 0009E368  38 A0 20 00 */	li r5, 0x2000
/* 800A142C 0009E36C  4B F6 1D C9 */	bl memcpy
/* 800A1430 0009E370  48 00 00 18 */	b lbl_800A1448
lbl_800A1434:
/* 800A1434 0009E374  90 9F 00 88 */	stw r4, 0x88(r31)
/* 800A1438 0009E378  38 64 00 00 */	addi r3, r4, 0
/* 800A143C 0009E37C  38 85 00 00 */	addi r4, r5, 0
/* 800A1440 0009E380  38 A0 20 00 */	li r5, 0x2000
/* 800A1444 0009E384  4B F6 1D B1 */	bl memcpy
lbl_800A1448:
/* 800A1448 0009E388  80 1F 00 D0 */	lwz r0, 0xd0(r31)
/* 800A144C 0009E38C  28 00 00 00 */	cmplwi r0, 0
/* 800A1450 0009E390  40 82 00 10 */	bne lbl_800A1460
/* 800A1454 0009E394  38 7F 00 00 */	addi r3, r31, 0
/* 800A1458 0009E398  38 9E 00 00 */	addi r4, r30, 0
/* 800A145C 0009E39C  4B FF E7 F1 */	bl __CARDPutControlBlock
lbl_800A1460:
/* 800A1460 0009E3A0  80 1F 00 D8 */	lwz r0, 0xd8(r31)
/* 800A1464 0009E3A4  28 00 00 00 */	cmplwi r0, 0
/* 800A1468 0009E3A8  7C 0C 03 78 */	mr r12, r0
/* 800A146C 0009E3AC  41 82 00 1C */	beq lbl_800A1488
/* 800A1470 0009E3B0  38 00 00 00 */	li r0, 0
/* 800A1474 0009E3B4  7D 88 03 A6 */	mtlr r12
/* 800A1478 0009E3B8  90 1F 00 D8 */	stw r0, 0xd8(r31)
/* 800A147C 0009E3BC  38 7D 00 00 */	addi r3, r29, 0
/* 800A1480 0009E3C0  38 9E 00 00 */	addi r4, r30, 0
/* 800A1484 0009E3C4  4E 80 00 21 */	blrl 
lbl_800A1488:
/* 800A1488 0009E3C8  80 01 00 24 */	lwz r0, 0x24(r1)
/* 800A148C 0009E3CC  83 E1 00 1C */	lwz r31, 0x1c(r1)
/* 800A1490 0009E3D0  83 C1 00 18 */	lwz r30, 0x18(r1)
/* 800A1494 0009E3D4  83 A1 00 14 */	lwz r29, 0x14(r1)
/* 800A1498 0009E3D8  38 21 00 20 */	addi r1, r1, 0x20
/* 800A149C 0009E3DC  7C 08 03 A6 */	mtlr r0
/* 800A14A0 0009E3E0  4E 80 00 20 */	blr 

EraseCallback:
/* 800A14A4 0009E3E4  7C 08 02 A6 */	mflr r0
/* 800A14A8 0009E3E8  90 01 00 04 */	stw r0, 4(r1)
/* 800A14AC 0009E3EC  94 21 FF D8 */	stwu r1, -0x28(r1)
/* 800A14B0 0009E3F0  93 E1 00 24 */	stw r31, 0x24(r1)
/* 800A14B4 0009E3F4  3B E3 00 00 */	addi r31, r3, 0
/* 800A14B8 0009E3F8  3C 60 80 3F */	lis r3, __CARDBlock@ha
/* 800A14BC 0009E3FC  93 C1 00 20 */	stw r30, 0x20(r1)
/* 800A14C0 0009E400  1C BF 01 10 */	mulli r5, r31, 0x110
/* 800A14C4 0009E404  93 A1 00 1C */	stw r29, 0x1c(r1)
/* 800A14C8 0009E408  38 03 BF 60 */	addi r0, r3, __CARDBlock@l
/* 800A14CC 0009E40C  7C 9D 23 79 */	or. r29, r4, r4
/* 800A14D0 0009E410  7F C0 2A 14 */	add r30, r0, r5
/* 800A14D4 0009E414  41 80 00 3C */	blt lbl_800A1510
/* 800A14D8 0009E418  80 BE 00 88 */	lwz r5, 0x88(r30)
/* 800A14DC 0009E41C  3C 60 80 0A */	lis r3, WriteCallback@ha
/* 800A14E0 0009E420  80 1E 00 80 */	lwz r0, 0x80(r30)
/* 800A14E4 0009E424  38 E3 13 D0 */	addi r7, r3, WriteCallback@l
/* 800A14E8 0009E428  80 7E 00 0C */	lwz r3, 0xc(r30)
/* 800A14EC 0009E42C  7C 00 28 50 */	subf r0, r0, r5
/* 800A14F0 0009E430  54 00 9B 7E */	srwi r0, r0, 0xd
/* 800A14F4 0009E434  7C 83 01 D6 */	mullw r4, r3, r0
/* 800A14F8 0009E438  38 C5 00 00 */	addi r6, r5, 0
/* 800A14FC 0009E43C  38 7F 00 00 */	addi r3, r31, 0
/* 800A1500 0009E440  38 A0 20 00 */	li r5, 0x2000
/* 800A1504 0009E444  4B FF FE 61 */	bl __CARDWrite
/* 800A1508 0009E448  7C 7D 1B 79 */	or. r29, r3, r3
/* 800A150C 0009E44C  40 80 00 44 */	bge lbl_800A1550
lbl_800A1510:
/* 800A1510 0009E450  80 1E 00 D0 */	lwz r0, 0xd0(r30)
/* 800A1514 0009E454  28 00 00 00 */	cmplwi r0, 0
/* 800A1518 0009E458  40 82 00 10 */	bne lbl_800A1528
/* 800A151C 0009E45C  38 7E 00 00 */	addi r3, r30, 0
/* 800A1520 0009E460  38 9D 00 00 */	addi r4, r29, 0
/* 800A1524 0009E464  4B FF E7 29 */	bl __CARDPutControlBlock
lbl_800A1528:
/* 800A1528 0009E468  80 1E 00 D8 */	lwz r0, 0xd8(r30)
/* 800A152C 0009E46C  28 00 00 00 */	cmplwi r0, 0
/* 800A1530 0009E470  7C 0C 03 78 */	mr r12, r0
/* 800A1534 0009E474  41 82 00 1C */	beq lbl_800A1550
/* 800A1538 0009E478  38 00 00 00 */	li r0, 0
/* 800A153C 0009E47C  7D 88 03 A6 */	mtlr r12
/* 800A1540 0009E480  90 1E 00 D8 */	stw r0, 0xd8(r30)
/* 800A1544 0009E484  38 7F 00 00 */	addi r3, r31, 0
/* 800A1548 0009E488  38 9D 00 00 */	addi r4, r29, 0
/* 800A154C 0009E48C  4E 80 00 21 */	blrl 
lbl_800A1550:
/* 800A1550 0009E490  80 01 00 2C */	lwz r0, 0x2c(r1)
/* 800A1554 0009E494  83 E1 00 24 */	lwz r31, 0x24(r1)
/* 800A1558 0009E498  83 C1 00 20 */	lwz r30, 0x20(r1)
/* 800A155C 0009E49C  83 A1 00 1C */	lwz r29, 0x1c(r1)
/* 800A1560 0009E4A0  38 21 00 28 */	addi r1, r1, 0x28
/* 800A1564 0009E4A4  7C 08 03 A6 */	mtlr r0
/* 800A1568 0009E4A8  4E 80 00 20 */	blr 

.global __CARDAllocBlock
__CARDAllocBlock:
/* 800A156C 0009E4AC  7C 08 02 A6 */	mflr r0
/* 800A1570 0009E4B0  1C E3 01 10 */	mulli r7, r3, 0x110
/* 800A1574 0009E4B4  90 01 00 04 */	stw r0, 4(r1)
/* 800A1578 0009E4B8  94 21 FF E0 */	stwu r1, -0x20(r1)
/* 800A157C 0009E4BC  3C C0 80 3F */	lis r6, __CARDBlock@ha
/* 800A1580 0009E4C0  38 06 BF 60 */	addi r0, r6, __CARDBlock@l
/* 800A1584 0009E4C4  93 E1 00 1C */	stw r31, 0x1c(r1)
/* 800A1588 0009E4C8  7D 20 3A 14 */	add r9, r0, r7
/* 800A158C 0009E4CC  93 C1 00 18 */	stw r30, 0x18(r1)
/* 800A1590 0009E4D0  80 09 00 00 */	lwz r0, 0(r9)
/* 800A1594 0009E4D4  2C 00 00 00 */	cmpwi r0, 0
/* 800A1598 0009E4D8  40 82 00 0C */	bne lbl_800A15A4
/* 800A159C 0009E4DC  38 60 FF FD */	li r3, -3
/* 800A15A0 0009E4E0  48 00 00 CC */	b lbl_800A166C
lbl_800A15A4:
/* 800A15A4 0009E4E4  81 09 00 88 */	lwz r8, 0x88(r9)
/* 800A15A8 0009E4E8  A0 08 00 06 */	lhz r0, 6(r8)
/* 800A15AC 0009E4EC  7C 00 20 40 */	cmplw r0, r4
/* 800A15B0 0009E4F0  40 80 00 0C */	bge lbl_800A15BC
/* 800A15B4 0009E4F4  38 60 FF F7 */	li r3, -9
/* 800A15B8 0009E4F8  48 00 00 B4 */	b lbl_800A166C
lbl_800A15BC:
/* 800A15BC 0009E4FC  7C 04 00 50 */	subf r0, r4, r0
/* 800A15C0 0009E500  B0 08 00 06 */	sth r0, 6(r8)
/* 800A15C4 0009E504  3C C0 00 01 */	lis r6, 0x0000FFFF@ha
/* 800A15C8 0009E508  39 86 FF FF */	addi r12, r6, 0x0000FFFF@l
/* 800A15CC 0009E50C  A1 68 00 08 */	lhz r11, 8(r8)
/* 800A15D0 0009E510  38 0C 00 00 */	addi r0, r12, 0
/* 800A15D4 0009E514  3B C0 00 00 */	li r30, 0
/* 800A15D8 0009E518  48 00 00 7C */	b lbl_800A1654
lbl_800A15DC:
/* 800A15DC 0009E51C  A1 49 00 10 */	lhz r10, 0x10(r9)
/* 800A15E0 0009E520  3B DE 00 01 */	addi r30, r30, 1
/* 800A15E4 0009E524  57 C6 04 3E */	clrlwi r6, r30, 0x10
/* 800A15E8 0009E528  38 EA FF FB */	addi r7, r10, -5
/* 800A15EC 0009E52C  7C 07 30 00 */	cmpw r7, r6
/* 800A15F0 0009E530  40 80 00 0C */	bge lbl_800A15FC
/* 800A15F4 0009E534  38 60 FF FA */	li r3, -6
/* 800A15F8 0009E538  48 00 00 74 */	b lbl_800A166C
lbl_800A15FC:
/* 800A15FC 0009E53C  39 6B 00 01 */	addi r11, r11, 1
/* 800A1600 0009E540  55 66 04 3E */	clrlwi r6, r11, 0x10
/* 800A1604 0009E544  28 06 00 05 */	cmplwi r6, 5
/* 800A1608 0009E548  41 80 00 0C */	blt lbl_800A1614
/* 800A160C 0009E54C  7C 06 50 40 */	cmplw r6, r10
/* 800A1610 0009E550  41 80 00 08 */	blt lbl_800A1618
lbl_800A1614:
/* 800A1614 0009E554  39 60 00 05 */	li r11, 5
lbl_800A1618:
/* 800A1618 0009E558  55 66 0B FC */	rlwinm r6, r11, 1, 0xf, 0x1e
/* 800A161C 0009E55C  7C E8 32 14 */	add r7, r8, r6
/* 800A1620 0009E560  A0 C7 00 00 */	lhz r6, 0(r7)
/* 800A1624 0009E564  28 06 00 00 */	cmplwi r6, 0
/* 800A1628 0009E568  40 82 00 2C */	bne lbl_800A1654
/* 800A162C 0009E56C  55 86 04 3E */	clrlwi r6, r12, 0x10
/* 800A1630 0009E570  28 06 FF FF */	cmplwi r6, 0xffff
/* 800A1634 0009E574  40 82 00 0C */	bne lbl_800A1640
/* 800A1638 0009E578  7D 6C 5B 78 */	mr r12, r11
/* 800A163C 0009E57C  48 00 00 0C */	b lbl_800A1648
lbl_800A1640:
/* 800A1640 0009E580  57 E6 0B FC */	rlwinm r6, r31, 1, 0xf, 0x1e
/* 800A1644 0009E584  7D 68 33 2E */	sthx r11, r8, r6
lbl_800A1648:
/* 800A1648 0009E588  B0 07 00 00 */	sth r0, 0(r7)
/* 800A164C 0009E58C  3B EB 00 00 */	addi r31, r11, 0
/* 800A1650 0009E590  38 84 FF FF */	addi r4, r4, -1
lbl_800A1654:
/* 800A1654 0009E594  28 04 00 00 */	cmplwi r4, 0
/* 800A1658 0009E598  40 82 FF 84 */	bne lbl_800A15DC
/* 800A165C 0009E59C  B1 68 00 08 */	sth r11, 8(r8)
/* 800A1660 0009E5A0  7D 04 43 78 */	mr r4, r8
/* 800A1664 0009E5A4  B1 89 00 BE */	sth r12, 0xbe(r9)
/* 800A1668 0009E5A8  48 00 00 1D */	bl __CARDUpdateFatBlock
lbl_800A166C:
/* 800A166C 0009E5AC  80 01 00 24 */	lwz r0, 0x24(r1)
/* 800A1670 0009E5B0  83 E1 00 1C */	lwz r31, 0x1c(r1)
/* 800A1674 0009E5B4  83 C1 00 18 */	lwz r30, 0x18(r1)
/* 800A1678 0009E5B8  38 21 00 20 */	addi r1, r1, 0x20
/* 800A167C 0009E5BC  7C 08 03 A6 */	mtlr r0
/* 800A1680 0009E5C0  4E 80 00 20 */	blr 

.global __CARDUpdateFatBlock
__CARDUpdateFatBlock:
/* 800A1684 0009E5C4  7C 08 02 A6 */	mflr r0
/* 800A1688 0009E5C8  90 01 00 04 */	stw r0, 4(r1)
/* 800A168C 0009E5CC  94 21 FF D8 */	stwu r1, -0x28(r1)
/* 800A1690 0009E5D0  93 E1 00 24 */	stw r31, 0x24(r1)
/* 800A1694 0009E5D4  93 C1 00 20 */	stw r30, 0x20(r1)
/* 800A1698 0009E5D8  3B C5 00 00 */	addi r30, r5, 0
/* 800A169C 0009E5DC  93 A1 00 1C */	stw r29, 0x1c(r1)
/* 800A16A0 0009E5E0  3B A4 00 00 */	addi r29, r4, 0
/* 800A16A4 0009E5E4  38 BD 00 00 */	addi r5, r29, 0
/* 800A16A8 0009E5E8  93 81 00 18 */	stw r28, 0x18(r1)
/* 800A16AC 0009E5EC  3B 83 00 00 */	addi r28, r3, 0
/* 800A16B0 0009E5F0  38 DD 00 02 */	addi r6, r29, 2
/* 800A16B4 0009E5F4  A0 64 00 04 */	lhz r3, 4(r4)
/* 800A16B8 0009E5F8  1C 9C 01 10 */	mulli r4, r28, 0x110
/* 800A16BC 0009E5FC  38 03 00 01 */	addi r0, r3, 1
/* 800A16C0 0009E600  3C 60 80 3F */	lis r3, __CARDBlock@ha
/* 800A16C4 0009E604  B0 1D 00 04 */	sth r0, 4(r29)
/* 800A16C8 0009E608  38 03 BF 60 */	addi r0, r3, __CARDBlock@l
/* 800A16CC 0009E60C  7F E0 22 14 */	add r31, r0, r4
/* 800A16D0 0009E610  38 7D 00 04 */	addi r3, r29, 4
/* 800A16D4 0009E614  38 80 1F FC */	li r4, 0x1ffc
/* 800A16D8 0009E618  48 00 02 BD */	bl __CARDCheckSum
/* 800A16DC 0009E61C  38 7D 00 00 */	addi r3, r29, 0
/* 800A16E0 0009E620  38 80 20 00 */	li r4, 0x2000
/* 800A16E4 0009E624  4B FE C8 FD */	bl DCStoreRange
/* 800A16E8 0009E628  93 DF 00 D8 */	stw r30, 0xd8(r31)
/* 800A16EC 0009E62C  3C 60 80 0A */	lis r3, EraseCallback@ha
/* 800A16F0 0009E630  38 A3 14 A4 */	addi r5, r3, EraseCallback@l
/* 800A16F4 0009E634  80 1F 00 80 */	lwz r0, 0x80(r31)
/* 800A16F8 0009E638  7F 83 E3 78 */	mr r3, r28
/* 800A16FC 0009E63C  80 9F 00 0C */	lwz r4, 0xc(r31)
/* 800A1700 0009E640  7C 00 E8 50 */	subf r0, r0, r29
/* 800A1704 0009E644  54 00 9B 7E */	srwi r0, r0, 0xd
/* 800A1708 0009E648  7C 84 01 D6 */	mullw r4, r4, r0
/* 800A170C 0009E64C  4B FF E2 D5 */	bl __CARDEraseSector
/* 800A1710 0009E650  80 01 00 2C */	lwz r0, 0x2c(r1)
/* 800A1714 0009E654  83 E1 00 24 */	lwz r31, 0x24(r1)
/* 800A1718 0009E658  83 C1 00 20 */	lwz r30, 0x20(r1)
/* 800A171C 0009E65C  83 A1 00 1C */	lwz r29, 0x1c(r1)
/* 800A1720 0009E660  83 81 00 18 */	lwz r28, 0x18(r1)
/* 800A1724 0009E664  38 21 00 28 */	addi r1, r1, 0x28
/* 800A1728 0009E668  7C 08 03 A6 */	mtlr r0
/* 800A172C 0009E66C  4E 80 00 20 */	blr 