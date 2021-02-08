.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global __dt__23TMapObjRevivalPollutionFv
__dt__23TMapObjRevivalPollutionFv:
/* 801C0180 001BD0C0  7C 08 02 A6 */	mflr r0
/* 801C0184 001BD0C4  90 01 00 04 */	stw r0, 4(r1)
/* 801C0188 001BD0C8  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 801C018C 001BD0CC  93 E1 00 14 */	stw r31, 0x14(r1)
/* 801C0190 001BD0D0  3B E4 00 00 */	addi r31, r4, 0
/* 801C0194 001BD0D4  93 C1 00 10 */	stw r30, 0x10(r1)
/* 801C0198 001BD0D8  7C 7E 1B 79 */	or. r30, r3, r3
/* 801C019C 001BD0DC  41 82 00 3C */	beq lbl_801C01D8
/* 801C01A0 001BD0E0  3C 60 80 3D */	lis r3, __vt__23TMapObjRevivalPollution@ha
/* 801C01A4 001BD0E4  38 03 B4 10 */	addi r0, r3, __vt__23TMapObjRevivalPollution@l
/* 801C01A8 001BD0E8  90 1E 00 00 */	stw r0, 0(r30)
/* 801C01AC 001BD0EC  41 82 00 1C */	beq lbl_801C01C8
/* 801C01B0 001BD0F0  3C 60 80 3B */	lis r3, __vt__Q26JDrama8TViewObj@ha
/* 801C01B4 001BD0F4  38 03 A0 C0 */	addi r0, r3, __vt__Q26JDrama8TViewObj@l
/* 801C01B8 001BD0F8  90 1E 00 00 */	stw r0, 0(r30)
/* 801C01BC 001BD0FC  38 7E 00 00 */	addi r3, r30, 0
/* 801C01C0 001BD100  38 80 00 00 */	li r4, 0
/* 801C01C4 001BD104  4B E8 46 39 */	bl __dt__Q26JDrama8TNameRefFv
lbl_801C01C8:
/* 801C01C8 001BD108  7F E0 07 35 */	extsh. r0, r31
/* 801C01CC 001BD10C  40 81 00 0C */	ble lbl_801C01D8
/* 801C01D0 001BD110  7F C3 F3 78 */	mr r3, r30
/* 801C01D4 001BD114  4B E4 C8 DD */	bl __dl__FPv
lbl_801C01D8:
/* 801C01D8 001BD118  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 801C01DC 001BD11C  7F C3 F3 78 */	mr r3, r30
/* 801C01E0 001BD120  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 801C01E4 001BD124  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 801C01E8 001BD128  7C 08 03 A6 */	mtlr r0
/* 801C01EC 001BD12C  38 21 00 18 */	addi r1, r1, 0x18
/* 801C01F0 001BD130  4E 80 00 20 */	blr 

.global __ct__23TMapObjRevivalPollutionFPCc
__ct__23TMapObjRevivalPollutionFPCc:
/* 801C01F4 001BD134  7C 08 02 A6 */	mflr r0
/* 801C01F8 001BD138  3C A0 80 3B */	lis r5, __vt__Q26JDrama8TNameRef@ha
/* 801C01FC 001BD13C  90 01 00 04 */	stw r0, 4(r1)
/* 801C0200 001BD140  38 05 A8 60 */	addi r0, r5, __vt__Q26JDrama8TNameRef@l
/* 801C0204 001BD144  94 21 FF E0 */	stwu r1, -0x20(r1)
/* 801C0208 001BD148  93 E1 00 1C */	stw r31, 0x1c(r1)
/* 801C020C 001BD14C  3B E3 00 00 */	addi r31, r3, 0
/* 801C0210 001BD150  90 03 00 00 */	stw r0, 0(r3)
/* 801C0214 001BD154  7C 83 23 78 */	mr r3, r4
/* 801C0218 001BD158  90 9F 00 04 */	stw r4, 4(r31)
/* 801C021C 001BD15C  4B E8 44 71 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 801C0220 001BD160  B0 7F 00 08 */	sth r3, 8(r31)
/* 801C0224 001BD164  3C 60 80 3B */	lis r3, __vt__Q26JDrama8TViewObj@ha
/* 801C0228 001BD168  38 03 A0 C0 */	addi r0, r3, __vt__Q26JDrama8TViewObj@l
/* 801C022C 001BD16C  90 1F 00 00 */	stw r0, 0(r31)
/* 801C0230 001BD170  38 80 00 00 */	li r4, 0
/* 801C0234 001BD174  3C 60 80 3D */	lis r3, __vt__23TMapObjRevivalPollution@ha
/* 801C0238 001BD178  B0 9F 00 0C */	sth r4, 0xc(r31)
/* 801C023C 001BD17C  38 03 B4 10 */	addi r0, r3, __vt__23TMapObjRevivalPollution@l
/* 801C0240 001BD180  38 7F 00 00 */	addi r3, r31, 0
/* 801C0244 001BD184  90 1F 00 00 */	stw r0, 0(r31)
/* 801C0248 001BD188  90 9F 00 10 */	stw r4, 0x10(r31)
/* 801C024C 001BD18C  90 9F 00 14 */	stw r4, 0x14(r31)
/* 801C0250 001BD190  80 01 00 24 */	lwz r0, 0x24(r1)
/* 801C0254 001BD194  83 E1 00 1C */	lwz r31, 0x1c(r1)
/* 801C0258 001BD198  38 21 00 20 */	addi r1, r1, 0x20
/* 801C025C 001BD19C  7C 08 03 A6 */	mtlr r0
/* 801C0260 001BD1A0  4E 80 00 20 */	blr 

.global load__23TMapObjRevivalPollutionFR20JSUMemoryInputStream
load__23TMapObjRevivalPollutionFR20JSUMemoryInputStream:
/* 801C0264 001BD1A4  7C 08 02 A6 */	mflr r0
/* 801C0268 001BD1A8  90 01 00 04 */	stw r0, 4(r1)
/* 801C026C 001BD1AC  94 21 FF 88 */	stwu r1, -0x78(r1)
/* 801C0270 001BD1B0  BF 41 00 60 */	stmw r26, 0x60(r1)
/* 801C0274 001BD1B4  3B 43 00 00 */	addi r26, r3, 0
/* 801C0278 001BD1B8  3B 64 00 00 */	addi r27, r4, 0
/* 801C027C 001BD1BC  4B E8 45 D5 */	bl load__Q26JDrama8TNameRefFR20JSUMemoryInputStream
/* 801C0280 001BD1C0  38 7B 00 00 */	addi r3, r27, 0
/* 801C0284 001BD1C4  38 9A 00 10 */	addi r4, r26, 0x10
/* 801C0288 001BD1C8  38 A0 00 04 */	li r5, 4
/* 801C028C 001BD1CC  4B E4 E3 05 */	bl read__14JSUInputStreamFPvl
/* 801C0290 001BD1D0  83 DA 00 10 */	lwz r30, 0x10(r26)
/* 801C0294 001BD1D4  1C 7E 00 1C */	mulli r3, r30, 0x1c
/* 801C0298 001BD1D8  38 63 00 08 */	addi r3, r3, 8
/* 801C029C 001BD1DC  4B E4 C7 15 */	bl __nwa__FUl
/* 801C02A0 001BD1E0  3C 80 80 1C */	lis r4, __ct__16TRevivalPolluterFv@ha
/* 801C02A4 001BD1E4  38 84 04 58 */	addi r4, r4, __ct__16TRevivalPolluterFv@l
/* 801C02A8 001BD1E8  38 FE 00 00 */	addi r7, r30, 0
/* 801C02AC 001BD1EC  38 A0 00 00 */	li r5, 0
/* 801C02B0 001BD1F0  38 C0 00 1C */	li r6, 0x1c
/* 801C02B4 001BD1F4  4B EC 27 39 */	bl __construct_new_array
/* 801C02B8 001BD1F8  3C 80 80 39 */	lis r4, $$22600@ha
/* 801C02BC 001BD1FC  90 7A 00 14 */	stw r3, 0x14(r26)
/* 801C02C0 001BD200  3B C4 FB D0 */	addi r30, r4, $$22600@l
/* 801C02C4 001BD204  3B 80 00 00 */	li r28, 0
/* 801C02C8 001BD208  3B E0 00 00 */	li r31, 0
/* 801C02CC 001BD20C  48 00 00 60 */	b lbl_801C032C
lbl_801C02D0:
/* 801C02D0 001BD210  80 1A 00 14 */	lwz r0, 0x14(r26)
/* 801C02D4 001BD214  38 7B 00 00 */	addi r3, r27, 0
/* 801C02D8 001BD218  38 81 00 18 */	addi r4, r1, 0x18
/* 801C02DC 001BD21C  7F A0 FA 14 */	add r29, r0, r31
/* 801C02E0 001BD220  38 A0 00 04 */	li r5, 4
/* 801C02E4 001BD224  4B E4 E2 AD */	bl read__14JSUInputStreamFPvl
/* 801C02E8 001BD228  80 01 00 18 */	lwz r0, 0x18(r1)
/* 801C02EC 001BD22C  38 7B 00 00 */	addi r3, r27, 0
/* 801C02F0 001BD230  38 9D 00 18 */	addi r4, r29, 0x18
/* 801C02F4 001BD234  90 1D 00 00 */	stw r0, 0(r29)
/* 801C02F8 001BD238  38 A0 00 04 */	li r5, 4
/* 801C02FC 001BD23C  4B E4 E2 95 */	bl read__14JSUInputStreamFPvl
/* 801C0300 001BD240  80 DD 00 00 */	lwz r6, 0(r29)
/* 801C0304 001BD244  38 BE 00 00 */	addi r5, r30, 0
/* 801C0308 001BD248  38 61 00 1C */	addi r3, r1, 0x1c
/* 801C030C 001BD24C  4C C6 31 82 */	crclr 6
/* 801C0310 001BD250  38 80 00 40 */	li r4, 0x40
/* 801C0314 001BD254  4B EC 3D B1 */	bl snprintf
/* 801C0318 001BD258  38 61 00 1C */	addi r3, r1, 0x1c
/* 801C031C 001BD25C  4B E4 B9 A1 */	bl getGlbResource__13JKRFileLoaderFPCc
/* 801C0320 001BD260  90 7D 00 04 */	stw r3, 4(r29)
/* 801C0324 001BD264  3B 9C 00 01 */	addi r28, r28, 1
/* 801C0328 001BD268  3B FF 00 1C */	addi r31, r31, 0x1c
lbl_801C032C:
/* 801C032C 001BD26C  80 1A 00 10 */	lwz r0, 0x10(r26)
/* 801C0330 001BD270  7C 1C 00 00 */	cmpw r28, r0
/* 801C0334 001BD274  41 80 FF 9C */	blt lbl_801C02D0
/* 801C0338 001BD278  BB 41 00 60 */	lmw r26, 0x60(r1)
/* 801C033C 001BD27C  80 01 00 7C */	lwz r0, 0x7c(r1)
/* 801C0340 001BD280  38 21 00 78 */	addi r1, r1, 0x78
/* 801C0344 001BD284  7C 08 03 A6 */	mtlr r0
/* 801C0348 001BD288  4E 80 00 20 */	blr 

.global loadAfter__23TMapObjRevivalPollutionFv
loadAfter__23TMapObjRevivalPollutionFv:
/* 801C034C 001BD28C  7C 08 02 A6 */	mflr r0
/* 801C0350 001BD290  90 01 00 04 */	stw r0, 4(r1)
/* 801C0354 001BD294  94 21 FF A0 */	stwu r1, -0x60(r1)
/* 801C0358 001BD298  93 E1 00 5C */	stw r31, 0x5c(r1)
/* 801C035C 001BD29C  93 C1 00 58 */	stw r30, 0x58(r1)
/* 801C0360 001BD2A0  7C 7E 1B 78 */	mr r30, r3
/* 801C0364 001BD2A4  93 A1 00 54 */	stw r29, 0x54(r1)
/* 801C0368 001BD2A8  93 81 00 50 */	stw r28, 0x50(r1)
/* 801C036C 001BD2AC  4B E8 45 41 */	bl loadAfter__Q26JDrama8TNameRefFv
/* 801C0370 001BD2B0  3B E0 00 00 */	li r31, 0
/* 801C0374 001BD2B4  3B A0 00 00 */	li r29, 0
/* 801C0378 001BD2B8  48 00 00 54 */	b lbl_801C03CC
lbl_801C037C:
/* 801C037C 001BD2BC  80 1E 00 14 */	lwz r0, 0x14(r30)
/* 801C0380 001BD2C0  38 A0 00 00 */	li r5, 0
/* 801C0384 001BD2C4  80 ED 9A E8 */	lwz r7, gpPollution-_SDA_BASE_(r13)
/* 801C0388 001BD2C8  38 C0 00 00 */	li r6, 0
/* 801C038C 001BD2CC  7F 80 EA 14 */	add r28, r0, r29
/* 801C0390 001BD2D0  80 9C 00 00 */	lwz r4, 0(r28)
/* 801C0394 001BD2D4  38 67 00 70 */	addi r3, r7, 0x70
/* 801C0398 001BD2D8  80 E7 00 14 */	lwz r7, 0x14(r7)
/* 801C039C 001BD2DC  54 80 10 3A */	slwi r0, r4, 2
/* 801C03A0 001BD2E0  81 5C 00 04 */	lwz r10, 4(r28)
/* 801C03A4 001BD2E4  7D 07 00 2E */	lwzx r8, r7, r0
/* 801C03A8 001BD2E8  81 3C 00 18 */	lwz r9, 0x18(r28)
/* 801C03AC 001BD2EC  80 E8 00 60 */	lwz r7, 0x60(r8)
/* 801C03B0 001BD2F0  80 08 00 5C */	lwz r0, 0x5c(r8)
/* 801C03B4 001BD2F4  7C E8 07 34 */	extsh r8, r7
/* 801C03B8 001BD2F8  7C 07 07 34 */	extsh r7, r0
/* 801C03BC 001BD2FC  48 02 C8 B5 */	bl registerRevivalTexStamp__22TPollutionCounterLayerFissssiP7ResTIMG
/* 801C03C0 001BD300  B0 7C 00 08 */	sth r3, 8(r28)
/* 801C03C4 001BD304  3B FF 00 01 */	addi r31, r31, 1
/* 801C03C8 001BD308  3B BD 00 1C */	addi r29, r29, 0x1c
lbl_801C03CC:
/* 801C03CC 001BD30C  80 1E 00 10 */	lwz r0, 0x10(r30)
/* 801C03D0 001BD310  7C 1F 00 00 */	cmpw r31, r0
/* 801C03D4 001BD314  41 80 FF A8 */	blt lbl_801C037C
/* 801C03D8 001BD318  80 01 00 64 */	lwz r0, 0x64(r1)
/* 801C03DC 001BD31C  83 E1 00 5C */	lwz r31, 0x5c(r1)
/* 801C03E0 001BD320  83 C1 00 58 */	lwz r30, 0x58(r1)
/* 801C03E4 001BD324  7C 08 03 A6 */	mtlr r0
/* 801C03E8 001BD328  83 A1 00 54 */	lwz r29, 0x54(r1)
/* 801C03EC 001BD32C  83 81 00 50 */	lwz r28, 0x50(r1)
/* 801C03F0 001BD330  38 21 00 60 */	addi r1, r1, 0x60
/* 801C03F4 001BD334  4E 80 00 20 */	blr 

.global perform__23TMapObjRevivalPollutionFUlPQ26JDrama9TGraphics
perform__23TMapObjRevivalPollutionFUlPQ26JDrama9TGraphics:
/* 801C03F8 001BD338  54 80 07 BD */	rlwinm. r0, r4, 0, 0x1e, 0x1e
/* 801C03FC 001BD33C  4D 82 00 20 */	beqlr 
/* 801C0400 001BD340  80 A3 00 10 */	lwz r5, 0x10(r3)
/* 801C0404 001BD344  38 C0 00 00 */	li r6, 0
/* 801C0408 001BD348  7C 06 28 00 */	cmpw r6, r5
/* 801C040C 001BD34C  4C 80 00 20 */	bgelr 
/* 801C0410 001BD350  2C 05 00 08 */	cmpwi r5, 8
/* 801C0414 001BD354  38 85 FF F8 */	addi r4, r5, -8
/* 801C0418 001BD358  40 81 00 20 */	ble lbl_801C0438
/* 801C041C 001BD35C  38 04 00 07 */	addi r0, r4, 7
/* 801C0420 001BD360  54 00 E8 FE */	srwi r0, r0, 3
/* 801C0424 001BD364  2C 04 00 00 */	cmpwi r4, 0
/* 801C0428 001BD368  7C 09 03 A6 */	mtctr r0
/* 801C042C 001BD36C  40 81 00 0C */	ble lbl_801C0438
lbl_801C0430:
/* 801C0430 001BD370  38 C6 00 08 */	addi r6, r6, 8
/* 801C0434 001BD374  42 00 FF FC */	bdnz lbl_801C0430
lbl_801C0438:
/* 801C0438 001BD378  7C 06 28 50 */	subf r0, r6, r5
/* 801C043C 001BD37C  7C 06 28 00 */	cmpw r6, r5
/* 801C0440 001BD380  7C 09 03 A6 */	mtctr r0
/* 801C0444 001BD384  4C 80 00 20 */	bgelr 
lbl_801C0448:
/* 801C0448 001BD388  42 00 00 00 */	bdnz lbl_801C0448
/* 801C044C 001BD38C  4E 80 00 20 */	blr 
/* 801C0450 001BD390  4B FF FF E8 */	b lbl_801C0438
/* 801C0454 001BD394  4E 80 00 20 */	blr 

.global __ct__16TRevivalPolluterFv
__ct__16TRevivalPolluterFv:
/* 801C0458 001BD398  38 00 00 00 */	li r0, 0
/* 801C045C 001BD39C  90 03 00 00 */	stw r0, 0(r3)
/* 801C0460 001BD3A0  90 03 00 04 */	stw r0, 4(r3)
/* 801C0464 001BD3A4  B0 03 00 08 */	sth r0, 8(r3)
/* 801C0468 001BD3A8  C0 02 C8 40 */	lfs f0, $$22649-_SDA2_BASE_(r2)
/* 801C046C 001BD3AC  D0 03 00 0C */	stfs f0, 0xc(r3)
/* 801C0470 001BD3B0  D0 03 00 10 */	stfs f0, 0x10(r3)
/* 801C0474 001BD3B4  D0 03 00 14 */	stfs f0, 0x14(r3)
/* 801C0478 001BD3B8  90 03 00 18 */	stw r0, 0x18(r3)
/* 801C047C 001BD3BC  4E 80 00 20 */	blr 

.global __ct__13TPolluterBaseFPCc
__ct__13TPolluterBaseFPCc:
/* 801C0480 001BD3C0  7C 08 02 A6 */	mflr r0
/* 801C0484 001BD3C4  90 01 00 04 */	stw r0, 4(r1)
/* 801C0488 001BD3C8  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 801C048C 001BD3CC  93 E1 00 14 */	stw r31, 0x14(r1)
/* 801C0490 001BD3D0  7C 7F 1B 78 */	mr r31, r3
/* 801C0494 001BD3D4  4B FC 75 E9 */	bl __ct__11TMapObjBaseFPCc
/* 801C0498 001BD3D8  3C 60 80 3D */	lis r3, __vt__13TPolluterBase@ha
/* 801C049C 001BD3DC  38 63 B4 34 */	addi r3, r3, __vt__13TPolluterBase@l
/* 801C04A0 001BD3E0  90 7F 00 00 */	stw r3, 0(r31)
/* 801C04A4 001BD3E4  38 63 00 24 */	addi r3, r3, 0x24
/* 801C04A8 001BD3E8  38 00 00 00 */	li r0, 0
/* 801C04AC 001BD3EC  90 7F 00 20 */	stw r3, 0x20(r31)
/* 801C04B0 001BD3F0  7F E3 FB 78 */	mr r3, r31
/* 801C04B4 001BD3F4  90 1F 01 38 */	stw r0, 0x138(r31)
/* 801C04B8 001BD3F8  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 801C04BC 001BD3FC  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 801C04C0 001BD400  38 21 00 18 */	addi r1, r1, 0x18
/* 801C04C4 001BD404  7C 08 03 A6 */	mtlr r0
/* 801C04C8 001BD408  4E 80 00 20 */	blr 

.global load__13TPolluterBaseFR20JSUMemoryInputStream
load__13TPolluterBaseFR20JSUMemoryInputStream:
/* 801C04CC 001BD40C  7C 08 02 A6 */	mflr r0
/* 801C04D0 001BD410  90 01 00 04 */	stw r0, 4(r1)
/* 801C04D4 001BD414  94 21 FF 68 */	stwu r1, -0x98(r1)
/* 801C04D8 001BD418  93 E1 00 94 */	stw r31, 0x94(r1)
/* 801C04DC 001BD41C  7C 7F 1B 78 */	mr r31, r3
/* 801C04E0 001BD420  4B FC 76 45 */	bl load__11TMapObjBaseFR20JSUMemoryInputStream
/* 801C04E4 001BD424  3C 60 80 39 */	lis r3, $$22703@ha
/* 801C04E8 001BD428  80 DF 00 F4 */	lwz r6, 0xf4(r31)
/* 801C04EC 001BD42C  38 A3 FB F8 */	addi r5, r3, $$22703@l
/* 801C04F0 001BD430  4C C6 31 82 */	crclr 6
/* 801C04F4 001BD434  38 61 00 38 */	addi r3, r1, 0x38
/* 801C04F8 001BD438  38 80 00 40 */	li r4, 0x40
/* 801C04FC 001BD43C  4B EC 3B C9 */	bl snprintf
/* 801C0500 001BD440  80 6D 9A 10 */	lwz r3, gpMapObjManager-_SDA_BASE_(r13)
/* 801C0504 001BD444  4B F5 5A C5 */	bl getMActorAnmData__11TObjManagerFv
/* 801C0508 001BD448  38 83 00 00 */	addi r4, r3, 0
/* 801C050C 001BD44C  38 61 00 38 */	addi r3, r1, 0x38
/* 801C0510 001BD450  38 A0 00 03 */	li r5, 3
/* 801C0514 001BD454  3C C0 10 21 */	lis r6, 0x1021
/* 801C0518 001BD458  4B F1 95 D5 */	bl SMS_MakeMActorWithAnmData__FPCcP13MActorAnmDataUlUl
/* 801C051C 001BD45C  90 7F 01 38 */	stw r3, 0x138(r31)
/* 801C0520 001BD460  38 81 00 38 */	addi r4, r1, 0x38
/* 801C0524 001BD464  80 7F 00 F4 */	lwz r3, 0xf4(r31)
/* 801C0528 001BD468  4B FD 1C 65 */	bl makeLowerStr__11TMapObjBaseFPCcPc
/* 801C052C 001BD46C  80 7F 01 38 */	lwz r3, 0x138(r31)
/* 801C0530 001BD470  38 81 00 38 */	addi r4, r1, 0x38
/* 801C0534 001BD474  4B F1 3C 29 */	bl setBck__6MActorFPCc
/* 801C0538 001BD478  80 7F 01 38 */	lwz r3, 0x138(r31)
/* 801C053C 001BD47C  38 81 00 38 */	addi r4, r1, 0x38
/* 801C0540 001BD480  4B F1 39 09 */	bl setBpk__6MActorFPCc
/* 801C0544 001BD484  80 7F 01 38 */	lwz r3, 0x138(r31)
/* 801C0548 001BD488  38 81 00 38 */	addi r4, r1, 0x38
/* 801C054C 001BD48C  4B F1 36 69 */	bl setBtp__6MActorFPCc
/* 801C0550 001BD490  80 7F 01 38 */	lwz r3, 0x138(r31)
/* 801C0554 001BD494  38 81 00 38 */	addi r4, r1, 0x38
/* 801C0558 001BD498  4B F1 33 B9 */	bl setBtk__6MActorFPCc
/* 801C055C 001BD49C  C0 1F 00 38 */	lfs f0, 0x38(r31)
/* 801C0560 001BD4A0  C0 42 C8 44 */	lfs f2, $$22704-_SDA2_BASE_(r2)
/* 801C0564 001BD4A4  C0 3F 00 34 */	lfs f1, 0x34(r31)
/* 801C0568 001BD4A8  EC 02 00 32 */	fmuls f0, f2, f0
/* 801C056C 001BD4AC  C0 7F 00 30 */	lfs f3, 0x30(r31)
/* 801C0570 001BD4B0  EC 22 00 72 */	fmuls f1, f2, f1
/* 801C0574 001BD4B4  80 7F 01 38 */	lwz r3, 0x138(r31)
/* 801C0578 001BD4B8  EC 42 00 F2 */	fmuls f2, f2, f3
/* 801C057C 001BD4BC  FC 80 08 1E */	fctiwz f4, f1
/* 801C0580 001BD4C0  80 63 00 04 */	lwz r3, 4(r3)
/* 801C0584 001BD4C4  FC A0 10 1E */	fctiwz f5, f2
/* 801C0588 001BD4C8  C0 7F 00 18 */	lfs f3, 0x18(r31)
/* 801C058C 001BD4CC  FC 00 00 1E */	fctiwz f0, f0
/* 801C0590 001BD4D0  D8 A1 00 88 */	stfd f5, 0x88(r1)
/* 801C0594 001BD4D4  38 63 00 20 */	addi r3, r3, 0x20
/* 801C0598 001BD4D8  C0 5F 00 14 */	lfs f2, 0x14(r31)
/* 801C059C 001BD4DC  D8 81 00 80 */	stfd f4, 0x80(r1)
/* 801C05A0 001BD4E0  C0 3F 00 10 */	lfs f1, 0x10(r31)
/* 801C05A4 001BD4E4  D8 01 00 78 */	stfd f0, 0x78(r1)
/* 801C05A8 001BD4E8  80 81 00 8C */	lwz r4, 0x8c(r1)
/* 801C05AC 001BD4EC  80 A1 00 84 */	lwz r5, 0x84(r1)
/* 801C05B0 001BD4F0  80 C1 00 7C */	lwz r6, 0x7c(r1)
/* 801C05B4 001BD4F4  4B F0 58 35 */	bl MsMtxSetXYZRPH__FPA4_ffffsss
/* 801C05B8 001BD4F8  80 9F 01 38 */	lwz r4, 0x138(r31)
/* 801C05BC 001BD4FC  80 7F 00 24 */	lwz r3, 0x24(r31)
/* 801C05C0 001BD500  80 84 00 04 */	lwz r4, 4(r4)
/* 801C05C4 001BD504  80 1F 00 28 */	lwz r0, 0x28(r31)
/* 801C05C8 001BD508  90 64 00 14 */	stw r3, 0x14(r4)
/* 801C05CC 001BD50C  90 04 00 18 */	stw r0, 0x18(r4)
/* 801C05D0 001BD510  80 1F 00 2C */	lwz r0, 0x2c(r31)
/* 801C05D4 001BD514  90 04 00 1C */	stw r0, 0x1c(r4)
/* 801C05D8 001BD518  80 01 00 9C */	lwz r0, 0x9c(r1)
/* 801C05DC 001BD51C  83 E1 00 94 */	lwz r31, 0x94(r1)
/* 801C05E0 001BD520  38 21 00 98 */	addi r1, r1, 0x98
/* 801C05E4 001BD524  7C 08 03 A6 */	mtlr r0
/* 801C05E8 001BD528  4E 80 00 20 */	blr 

.global perform__13TPolluterBaseFUlPQ26JDrama9TGraphics
perform__13TPolluterBaseFUlPQ26JDrama9TGraphics:
/* 801C05EC 001BD52C  7C 08 02 A6 */	mflr r0
/* 801C05F0 001BD530  90 01 00 04 */	stw r0, 4(r1)
/* 801C05F4 001BD534  54 80 07 BD */	rlwinm. r0, r4, 0, 0x1e, 0x1e
/* 801C05F8 001BD538  94 21 FF D8 */	stwu r1, -0x28(r1)
/* 801C05FC 001BD53C  93 E1 00 24 */	stw r31, 0x24(r1)
/* 801C0600 001BD540  3B E4 00 00 */	addi r31, r4, 0
/* 801C0604 001BD544  93 C1 00 20 */	stw r30, 0x20(r1)
/* 801C0608 001BD548  3B C3 00 00 */	addi r30, r3, 0
/* 801C060C 001BD54C  41 82 00 0C */	beq lbl_801C0618
/* 801C0610 001BD550  80 7E 01 38 */	lwz r3, 0x138(r30)
/* 801C0614 001BD554  4B F1 45 35 */	bl calcAnm__6MActorFv
lbl_801C0618:
/* 801C0618 001BD558  57 E0 05 AD */	rlwinm. r0, r31, 0, 0x16, 0x16
/* 801C061C 001BD55C  41 82 00 14 */	beq lbl_801C0630
/* 801C0620 001BD560  80 9E 01 38 */	lwz r4, 0x138(r30)
/* 801C0624 001BD564  80 6D 9A E8 */	lwz r3, gpPollution-_SDA_BASE_(r13)
/* 801C0628 001BD568  80 84 00 04 */	lwz r4, 4(r4)
/* 801C062C 001BD56C  48 02 F4 B1 */	bl stampModel__17TPollutionManagerFP8J3DModel
lbl_801C0630:
/* 801C0630 001BD570  80 01 00 2C */	lwz r0, 0x2c(r1)
/* 801C0634 001BD574  83 E1 00 24 */	lwz r31, 0x24(r1)
/* 801C0638 001BD578  83 C1 00 20 */	lwz r30, 0x20(r1)
/* 801C063C 001BD57C  7C 08 03 A6 */	mtlr r0
/* 801C0640 001BD580  38 21 00 28 */	addi r1, r1, 0x28
/* 801C0644 001BD584  4E 80 00 20 */	blr 

.global __dt__13TPolluterBaseFv
__dt__13TPolluterBaseFv:
/* 801C0648 001BD588  7C 08 02 A6 */	mflr r0
/* 801C064C 001BD58C  90 01 00 04 */	stw r0, 4(r1)
/* 801C0650 001BD590  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 801C0654 001BD594  93 E1 00 14 */	stw r31, 0x14(r1)
/* 801C0658 001BD598  3B E4 00 00 */	addi r31, r4, 0
/* 801C065C 001BD59C  93 C1 00 10 */	stw r30, 0x10(r1)
/* 801C0660 001BD5A0  7C 7E 1B 79 */	or. r30, r3, r3
/* 801C0664 001BD5A4  41 82 00 4C */	beq lbl_801C06B0
/* 801C0668 001BD5A8  3C 60 80 3D */	lis r3, __vt__13TPolluterBase@ha
/* 801C066C 001BD5AC  38 63 B4 34 */	addi r3, r3, __vt__13TPolluterBase@l
/* 801C0670 001BD5B0  90 7E 00 00 */	stw r3, 0(r30)
/* 801C0674 001BD5B4  38 03 00 24 */	addi r0, r3, 0x24
/* 801C0678 001BD5B8  90 1E 00 20 */	stw r0, 0x20(r30)
/* 801C067C 001BD5BC  41 82 00 24 */	beq lbl_801C06A0
/* 801C0680 001BD5C0  3C 60 80 3C */	lis r3, __vt__11TMapObjBase@ha
/* 801C0684 001BD5C4  38 63 A4 B0 */	addi r3, r3, __vt__11TMapObjBase@l
/* 801C0688 001BD5C8  90 7E 00 00 */	stw r3, 0(r30)
/* 801C068C 001BD5CC  38 03 00 24 */	addi r0, r3, 0x24
/* 801C0690 001BD5D0  38 7E 00 00 */	addi r3, r30, 0
/* 801C0694 001BD5D4  90 1E 00 20 */	stw r0, 0x20(r30)
/* 801C0698 001BD5D8  38 80 00 00 */	li r4, 0
/* 801C069C 001BD5DC  4B F5 1F 59 */	bl __dt__10TLiveActorFv
lbl_801C06A0:
/* 801C06A0 001BD5E0  7F E0 07 35 */	extsh. r0, r31
/* 801C06A4 001BD5E4  40 81 00 0C */	ble lbl_801C06B0
/* 801C06A8 001BD5E8  7F C3 F3 78 */	mr r3, r30
/* 801C06AC 001BD5EC  4B E4 C4 05 */	bl __dl__FPv
lbl_801C06B0:
/* 801C06B0 001BD5F0  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 801C06B4 001BD5F4  7F C3 F3 78 */	mr r3, r30
/* 801C06B8 001BD5F8  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 801C06BC 001BD5FC  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 801C06C0 001BD600  7C 08 03 A6 */	mtlr r0
/* 801C06C4 001BD604  38 21 00 18 */	addi r1, r1, 0x18
/* 801C06C8 001BD608  4E 80 00 20 */	blr 

.global __sinit_MapObjPollution_cpp
__sinit_MapObjPollution_cpp:
/* 801C06CC 001BD60C  7C 08 02 A6 */	mflr r0
/* 801C06D0 001BD610  3C 60 80 3F */	lis r3, $$22737@ha
/* 801C06D4 001BD614  90 01 00 04 */	stw r0, 4(r1)
/* 801C06D8 001BD618  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 801C06DC 001BD61C  93 E1 00 0C */	stw r31, 0xc(r1)
/* 801C06E0 001BD620  3B E3 69 D0 */	addi r31, r3, $$22737@l
/* 801C06E4 001BD624  88 0D 97 CC */	lbz r0, __init__smList__15JALList$$05MSBgm$$1-_SDA_BASE_(r13)
/* 801C06E8 001BD628  7C 00 07 75 */	extsb. r0, r0
/* 801C06EC 001BD62C  40 82 00 28 */	bne lbl_801C0714
/* 801C06F0 001BD630  38 6D 97 A8 */	addi r3, r13, smList__15JALList$$05MSBgm$$1-_SDA_BASE_
/* 801C06F4 001BD634  4B E4 E5 41 */	bl initiate__10JSUPtrListFv
/* 801C06F8 001BD638  3C 60 80 0E */	lis r3, __dt__15JSUList$$05MSBgm$$1Fv@ha
/* 801C06FC 001BD63C  38 83 6A 44 */	addi r4, r3, __dt__15JSUList$$05MSBgm$$1Fv@l
/* 801C0700 001BD640  38 6D 97 A8 */	addi r3, r13, smList__15JALList$$05MSBgm$$1-_SDA_BASE_
/* 801C0704 001BD644  38 BF 00 00 */	addi r5, r31, 0
/* 801C0708 001BD648  4B EC 20 21 */	bl __register_global_object
/* 801C070C 001BD64C  38 00 00 01 */	li r0, 1
/* 801C0710 001BD650  98 0D 97 CC */	stb r0, __init__smList__15JALList$$05MSBgm$$1-_SDA_BASE_(r13)
lbl_801C0714:
/* 801C0714 001BD654  88 0D 97 CD */	lbz r0, __init__smList__24JALList$$013MSSetSoundGrp$$1-_SDA_BASE_(r13)
/* 801C0718 001BD658  7C 00 07 75 */	extsb. r0, r0
/* 801C071C 001BD65C  40 82 00 28 */	bne lbl_801C0744
/* 801C0720 001BD660  38 6D 97 B4 */	addi r3, r13, smList__24JALList$$013MSSetSoundGrp$$1-_SDA_BASE_
/* 801C0724 001BD664  4B E4 E5 11 */	bl initiate__10JSUPtrListFv
/* 801C0728 001BD668  3C 60 80 0E */	lis r3, __dt__24JSUList$$013MSSetSoundGrp$$1Fv@ha
/* 801C072C 001BD66C  38 83 69 EC */	addi r4, r3, __dt__24JSUList$$013MSSetSoundGrp$$1Fv@l
/* 801C0730 001BD670  38 6D 97 B4 */	addi r3, r13, smList__24JALList$$013MSSetSoundGrp$$1-_SDA_BASE_
/* 801C0734 001BD674  38 BF 00 0C */	addi r5, r31, 0xc
/* 801C0738 001BD678  4B EC 1F F1 */	bl __register_global_object
/* 801C073C 001BD67C  38 00 00 01 */	li r0, 1
/* 801C0740 001BD680  98 0D 97 CD */	stb r0, __init__smList__24JALList$$013MSSetSoundGrp$$1-_SDA_BASE_(r13)
lbl_801C0744:
/* 801C0744 001BD684  88 0D 97 CE */	lbz r0, __init__smList__21JALList$$010MSSetSound$$1-_SDA_BASE_(r13)
/* 801C0748 001BD688  7C 00 07 75 */	extsb. r0, r0
/* 801C074C 001BD68C  40 82 00 28 */	bne lbl_801C0774
/* 801C0750 001BD690  38 6D 97 C0 */	addi r3, r13, smList__21JALList$$010MSSetSound$$1-_SDA_BASE_
/* 801C0754 001BD694  4B E4 E4 E1 */	bl initiate__10JSUPtrListFv
/* 801C0758 001BD698  3C 60 80 0E */	lis r3, __dt__21JSUList$$010MSSetSound$$1Fv@ha
/* 801C075C 001BD69C  38 83 69 94 */	addi r4, r3, __dt__21JSUList$$010MSSetSound$$1Fv@l
/* 801C0760 001BD6A0  38 6D 97 C0 */	addi r3, r13, smList__21JALList$$010MSSetSound$$1-_SDA_BASE_
/* 801C0764 001BD6A4  38 BF 00 18 */	addi r5, r31, 0x18
/* 801C0768 001BD6A8  4B EC 1F C1 */	bl __register_global_object
/* 801C076C 001BD6AC  38 00 00 01 */	li r0, 1
/* 801C0770 001BD6B0  98 0D 97 CE */	stb r0, __init__smList__21JALList$$010MSSetSound$$1-_SDA_BASE_(r13)
lbl_801C0774:
/* 801C0774 001BD6B4  88 0D 8F 8C */	lbz r0, __init__smList__26JALList$$015JALSeModEffDGrp$$1-_SDA_BASE_(r13)
/* 801C0778 001BD6B8  7C 00 07 75 */	extsb. r0, r0
/* 801C077C 001BD6BC  40 82 00 28 */	bne lbl_801C07A4
/* 801C0780 001BD6C0  38 6D 8E FC */	addi r3, r13, smList__26JALList$$015JALSeModEffDGrp$$1-_SDA_BASE_
/* 801C0784 001BD6C4  4B E4 E4 B1 */	bl initiate__10JSUPtrListFv
/* 801C0788 001BD6C8  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffDGrp$$1Fv@ha
/* 801C078C 001BD6CC  38 83 A8 0C */	addi r4, r3, __dt__26JSUList$$015JALSeModEffDGrp$$1Fv@l
/* 801C0790 001BD6D0  38 6D 8E FC */	addi r3, r13, smList__26JALList$$015JALSeModEffDGrp$$1-_SDA_BASE_
/* 801C0794 001BD6D4  38 BF 00 24 */	addi r5, r31, 0x24
/* 801C0798 001BD6D8  4B EC 1F 91 */	bl __register_global_object
/* 801C079C 001BD6DC  38 00 00 01 */	li r0, 1
/* 801C07A0 001BD6E0  98 0D 8F 8C */	stb r0, __init__smList__26JALList$$015JALSeModEffDGrp$$1-_SDA_BASE_(r13)
lbl_801C07A4:
/* 801C07A4 001BD6E4  88 0D 8F 8D */	lbz r0, __init__smList__26JALList$$015JALSeModPitDGrp$$1-_SDA_BASE_(r13)
/* 801C07A8 001BD6E8  7C 00 07 75 */	extsb. r0, r0
/* 801C07AC 001BD6EC  40 82 00 28 */	bne lbl_801C07D4
/* 801C07B0 001BD6F0  38 6D 8F 08 */	addi r3, r13, smList__26JALList$$015JALSeModPitDGrp$$1-_SDA_BASE_
/* 801C07B4 001BD6F4  4B E4 E4 81 */	bl initiate__10JSUPtrListFv
/* 801C07B8 001BD6F8  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitDGrp$$1Fv@ha
/* 801C07BC 001BD6FC  38 83 A7 B4 */	addi r4, r3, __dt__26JSUList$$015JALSeModPitDGrp$$1Fv@l
/* 801C07C0 001BD700  38 6D 8F 08 */	addi r3, r13, smList__26JALList$$015JALSeModPitDGrp$$1-_SDA_BASE_
/* 801C07C4 001BD704  38 BF 00 30 */	addi r5, r31, 0x30
/* 801C07C8 001BD708  4B EC 1F 61 */	bl __register_global_object
/* 801C07CC 001BD70C  38 00 00 01 */	li r0, 1
/* 801C07D0 001BD710  98 0D 8F 8D */	stb r0, __init__smList__26JALList$$015JALSeModPitDGrp$$1-_SDA_BASE_(r13)
lbl_801C07D4:
/* 801C07D4 001BD714  88 0D 8F 8E */	lbz r0, __init__smList__26JALList$$015JALSeModVolDGrp$$1-_SDA_BASE_(r13)
/* 801C07D8 001BD718  7C 00 07 75 */	extsb. r0, r0
/* 801C07DC 001BD71C  40 82 00 28 */	bne lbl_801C0804
/* 801C07E0 001BD720  38 6D 8F 14 */	addi r3, r13, smList__26JALList$$015JALSeModVolDGrp$$1-_SDA_BASE_
/* 801C07E4 001BD724  4B E4 E4 51 */	bl initiate__10JSUPtrListFv
/* 801C07E8 001BD728  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolDGrp$$1Fv@ha
/* 801C07EC 001BD72C  38 83 A7 5C */	addi r4, r3, __dt__26JSUList$$015JALSeModVolDGrp$$1Fv@l
/* 801C07F0 001BD730  38 6D 8F 14 */	addi r3, r13, smList__26JALList$$015JALSeModVolDGrp$$1-_SDA_BASE_
/* 801C07F4 001BD734  38 BF 00 3C */	addi r5, r31, 0x3c
/* 801C07F8 001BD738  4B EC 1F 31 */	bl __register_global_object
/* 801C07FC 001BD73C  38 00 00 01 */	li r0, 1
/* 801C0800 001BD740  98 0D 8F 8E */	stb r0, __init__smList__26JALList$$015JALSeModVolDGrp$$1-_SDA_BASE_(r13)
lbl_801C0804:
/* 801C0804 001BD744  88 0D 8F 8F */	lbz r0, __init__smList__26JALList$$015JALSeModEffFGrp$$1-_SDA_BASE_(r13)
/* 801C0808 001BD748  7C 00 07 75 */	extsb. r0, r0
/* 801C080C 001BD74C  40 82 00 28 */	bne lbl_801C0834
/* 801C0810 001BD750  38 6D 8F 20 */	addi r3, r13, smList__26JALList$$015JALSeModEffFGrp$$1-_SDA_BASE_
/* 801C0814 001BD754  4B E4 E4 21 */	bl initiate__10JSUPtrListFv
/* 801C0818 001BD758  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffFGrp$$1Fv@ha
/* 801C081C 001BD75C  38 83 A7 04 */	addi r4, r3, __dt__26JSUList$$015JALSeModEffFGrp$$1Fv@l
/* 801C0820 001BD760  38 6D 8F 20 */	addi r3, r13, smList__26JALList$$015JALSeModEffFGrp$$1-_SDA_BASE_
/* 801C0824 001BD764  38 BF 00 48 */	addi r5, r31, 0x48
/* 801C0828 001BD768  4B EC 1F 01 */	bl __register_global_object
/* 801C082C 001BD76C  38 00 00 01 */	li r0, 1
/* 801C0830 001BD770  98 0D 8F 8F */	stb r0, __init__smList__26JALList$$015JALSeModEffFGrp$$1-_SDA_BASE_(r13)
lbl_801C0834:
/* 801C0834 001BD774  88 0D 8F 90 */	lbz r0, __init__smList__26JALList$$015JALSeModPitFGrp$$1-_SDA_BASE_(r13)
/* 801C0838 001BD778  7C 00 07 75 */	extsb. r0, r0
/* 801C083C 001BD77C  40 82 00 28 */	bne lbl_801C0864
/* 801C0840 001BD780  38 6D 8F 2C */	addi r3, r13, smList__26JALList$$015JALSeModPitFGrp$$1-_SDA_BASE_
/* 801C0844 001BD784  4B E4 E3 F1 */	bl initiate__10JSUPtrListFv
/* 801C0848 001BD788  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitFGrp$$1Fv@ha
/* 801C084C 001BD78C  38 83 A6 AC */	addi r4, r3, __dt__26JSUList$$015JALSeModPitFGrp$$1Fv@l
/* 801C0850 001BD790  38 6D 8F 2C */	addi r3, r13, smList__26JALList$$015JALSeModPitFGrp$$1-_SDA_BASE_
/* 801C0854 001BD794  38 BF 00 54 */	addi r5, r31, 0x54
/* 801C0858 001BD798  4B EC 1E D1 */	bl __register_global_object
/* 801C085C 001BD79C  38 00 00 01 */	li r0, 1
/* 801C0860 001BD7A0  98 0D 8F 90 */	stb r0, __init__smList__26JALList$$015JALSeModPitFGrp$$1-_SDA_BASE_(r13)
lbl_801C0864:
/* 801C0864 001BD7A4  88 0D 8F 91 */	lbz r0, __init__smList__26JALList$$015JALSeModVolFGrp$$1-_SDA_BASE_(r13)
/* 801C0868 001BD7A8  7C 00 07 75 */	extsb. r0, r0
/* 801C086C 001BD7AC  40 82 00 28 */	bne lbl_801C0894
/* 801C0870 001BD7B0  38 6D 8F 38 */	addi r3, r13, smList__26JALList$$015JALSeModVolFGrp$$1-_SDA_BASE_
/* 801C0874 001BD7B4  4B E4 E3 C1 */	bl initiate__10JSUPtrListFv
/* 801C0878 001BD7B8  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolFGrp$$1Fv@ha
/* 801C087C 001BD7BC  38 83 A6 54 */	addi r4, r3, __dt__26JSUList$$015JALSeModVolFGrp$$1Fv@l
/* 801C0880 001BD7C0  38 6D 8F 38 */	addi r3, r13, smList__26JALList$$015JALSeModVolFGrp$$1-_SDA_BASE_
/* 801C0884 001BD7C4  38 BF 00 60 */	addi r5, r31, 0x60
/* 801C0888 001BD7C8  4B EC 1E A1 */	bl __register_global_object
/* 801C088C 001BD7CC  38 00 00 01 */	li r0, 1
/* 801C0890 001BD7D0  98 0D 8F 91 */	stb r0, __init__smList__26JALList$$015JALSeModVolFGrp$$1-_SDA_BASE_(r13)
lbl_801C0894:
/* 801C0894 001BD7D4  88 0D 8F 92 */	lbz r0, __init__smList__26JALList$$015JALSeModEffDist$$1-_SDA_BASE_(r13)
/* 801C0898 001BD7D8  7C 00 07 75 */	extsb. r0, r0
/* 801C089C 001BD7DC  40 82 00 28 */	bne lbl_801C08C4
/* 801C08A0 001BD7E0  38 6D 8F 44 */	addi r3, r13, smList__26JALList$$015JALSeModEffDist$$1-_SDA_BASE_
/* 801C08A4 001BD7E4  4B E4 E3 91 */	bl initiate__10JSUPtrListFv
/* 801C08A8 001BD7E8  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffDist$$1Fv@ha
/* 801C08AC 001BD7EC  38 83 A5 FC */	addi r4, r3, __dt__26JSUList$$015JALSeModEffDist$$1Fv@l
/* 801C08B0 001BD7F0  38 6D 8F 44 */	addi r3, r13, smList__26JALList$$015JALSeModEffDist$$1-_SDA_BASE_
/* 801C08B4 001BD7F4  38 BF 00 6C */	addi r5, r31, 0x6c
/* 801C08B8 001BD7F8  4B EC 1E 71 */	bl __register_global_object
/* 801C08BC 001BD7FC  38 00 00 01 */	li r0, 1
/* 801C08C0 001BD800  98 0D 8F 92 */	stb r0, __init__smList__26JALList$$015JALSeModEffDist$$1-_SDA_BASE_(r13)
lbl_801C08C4:
/* 801C08C4 001BD804  88 0D 8F 93 */	lbz r0, __init__smList__26JALList$$015JALSeModPitDist$$1-_SDA_BASE_(r13)
/* 801C08C8 001BD808  7C 00 07 75 */	extsb. r0, r0
/* 801C08CC 001BD80C  40 82 00 28 */	bne lbl_801C08F4
/* 801C08D0 001BD810  38 6D 8F 50 */	addi r3, r13, smList__26JALList$$015JALSeModPitDist$$1-_SDA_BASE_
/* 801C08D4 001BD814  4B E4 E3 61 */	bl initiate__10JSUPtrListFv
/* 801C08D8 001BD818  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitDist$$1Fv@ha
/* 801C08DC 001BD81C  38 83 A5 A4 */	addi r4, r3, __dt__26JSUList$$015JALSeModPitDist$$1Fv@l
/* 801C08E0 001BD820  38 6D 8F 50 */	addi r3, r13, smList__26JALList$$015JALSeModPitDist$$1-_SDA_BASE_
/* 801C08E4 001BD824  38 BF 00 78 */	addi r5, r31, 0x78
/* 801C08E8 001BD828  4B EC 1E 41 */	bl __register_global_object
/* 801C08EC 001BD82C  38 00 00 01 */	li r0, 1
/* 801C08F0 001BD830  98 0D 8F 93 */	stb r0, __init__smList__26JALList$$015JALSeModPitDist$$1-_SDA_BASE_(r13)
lbl_801C08F4:
/* 801C08F4 001BD834  88 0D 8F 94 */	lbz r0, __init__smList__26JALList$$015JALSeModVolDist$$1-_SDA_BASE_(r13)
/* 801C08F8 001BD838  7C 00 07 75 */	extsb. r0, r0
/* 801C08FC 001BD83C  40 82 00 28 */	bne lbl_801C0924
/* 801C0900 001BD840  38 6D 8F 5C */	addi r3, r13, smList__26JALList$$015JALSeModVolDist$$1-_SDA_BASE_
/* 801C0904 001BD844  4B E4 E3 31 */	bl initiate__10JSUPtrListFv
/* 801C0908 001BD848  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolDist$$1Fv@ha
/* 801C090C 001BD84C  38 83 A5 4C */	addi r4, r3, __dt__26JSUList$$015JALSeModVolDist$$1Fv@l
/* 801C0910 001BD850  38 6D 8F 5C */	addi r3, r13, smList__26JALList$$015JALSeModVolDist$$1-_SDA_BASE_
/* 801C0914 001BD854  38 BF 00 84 */	addi r5, r31, 0x84
/* 801C0918 001BD858  4B EC 1E 11 */	bl __register_global_object
/* 801C091C 001BD85C  38 00 00 01 */	li r0, 1
/* 801C0920 001BD860  98 0D 8F 94 */	stb r0, __init__smList__26JALList$$015JALSeModVolDist$$1-_SDA_BASE_(r13)
lbl_801C0924:
/* 801C0924 001BD864  88 0D 8F 95 */	lbz r0, __init__smList__26JALList$$015JALSeModEffFunk$$1-_SDA_BASE_(r13)
/* 801C0928 001BD868  7C 00 07 75 */	extsb. r0, r0
/* 801C092C 001BD86C  40 82 00 28 */	bne lbl_801C0954
/* 801C0930 001BD870  38 6D 8F 68 */	addi r3, r13, smList__26JALList$$015JALSeModEffFunk$$1-_SDA_BASE_
/* 801C0934 001BD874  4B E4 E3 01 */	bl initiate__10JSUPtrListFv
/* 801C0938 001BD878  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffFunk$$1Fv@ha
/* 801C093C 001BD87C  38 83 A4 F4 */	addi r4, r3, __dt__26JSUList$$015JALSeModEffFunk$$1Fv@l
/* 801C0940 001BD880  38 6D 8F 68 */	addi r3, r13, smList__26JALList$$015JALSeModEffFunk$$1-_SDA_BASE_
/* 801C0944 001BD884  38 BF 00 90 */	addi r5, r31, 0x90
/* 801C0948 001BD888  4B EC 1D E1 */	bl __register_global_object
/* 801C094C 001BD88C  38 00 00 01 */	li r0, 1
/* 801C0950 001BD890  98 0D 8F 95 */	stb r0, __init__smList__26JALList$$015JALSeModEffFunk$$1-_SDA_BASE_(r13)
lbl_801C0954:
/* 801C0954 001BD894  88 0D 8F 96 */	lbz r0, __init__smList__26JALList$$015JALSeModPitFunk$$1-_SDA_BASE_(r13)
/* 801C0958 001BD898  7C 00 07 75 */	extsb. r0, r0
/* 801C095C 001BD89C  40 82 00 28 */	bne lbl_801C0984
/* 801C0960 001BD8A0  38 6D 8F 74 */	addi r3, r13, smList__26JALList$$015JALSeModPitFunk$$1-_SDA_BASE_
/* 801C0964 001BD8A4  4B E4 E2 D1 */	bl initiate__10JSUPtrListFv
/* 801C0968 001BD8A8  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitFunk$$1Fv@ha
/* 801C096C 001BD8AC  38 83 A4 9C */	addi r4, r3, __dt__26JSUList$$015JALSeModPitFunk$$1Fv@l
/* 801C0970 001BD8B0  38 6D 8F 74 */	addi r3, r13, smList__26JALList$$015JALSeModPitFunk$$1-_SDA_BASE_
/* 801C0974 001BD8B4  38 BF 00 9C */	addi r5, r31, 0x9c
/* 801C0978 001BD8B8  4B EC 1D B1 */	bl __register_global_object
/* 801C097C 001BD8BC  38 00 00 01 */	li r0, 1
/* 801C0980 001BD8C0  98 0D 8F 96 */	stb r0, __init__smList__26JALList$$015JALSeModPitFunk$$1-_SDA_BASE_(r13)
lbl_801C0984:
/* 801C0984 001BD8C4  88 0D 8F 97 */	lbz r0, __init__smList__26JALList$$015JALSeModVolFunk$$1-_SDA_BASE_(r13)
/* 801C0988 001BD8C8  7C 00 07 75 */	extsb. r0, r0
/* 801C098C 001BD8CC  40 82 00 28 */	bne lbl_801C09B4
/* 801C0990 001BD8D0  38 6D 8F 80 */	addi r3, r13, smList__26JALList$$015JALSeModVolFunk$$1-_SDA_BASE_
/* 801C0994 001BD8D4  4B E4 E2 A1 */	bl initiate__10JSUPtrListFv
/* 801C0998 001BD8D8  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolFunk$$1Fv@ha
/* 801C099C 001BD8DC  38 83 A4 44 */	addi r4, r3, __dt__26JSUList$$015JALSeModVolFunk$$1Fv@l
/* 801C09A0 001BD8E0  38 6D 8F 80 */	addi r3, r13, smList__26JALList$$015JALSeModVolFunk$$1-_SDA_BASE_
/* 801C09A4 001BD8E4  38 BF 00 A8 */	addi r5, r31, 0xa8
/* 801C09A8 001BD8E8  4B EC 1D 81 */	bl __register_global_object
/* 801C09AC 001BD8EC  38 00 00 01 */	li r0, 1
/* 801C09B0 001BD8F0  98 0D 8F 97 */	stb r0, __init__smList__26JALList$$015JALSeModVolFunk$$1-_SDA_BASE_(r13)
lbl_801C09B4:
/* 801C09B4 001BD8F4  80 01 00 14 */	lwz r0, 0x14(r1)
/* 801C09B8 001BD8F8  83 E1 00 0C */	lwz r31, 0xc(r1)
/* 801C09BC 001BD8FC  38 21 00 10 */	addi r1, r1, 0x10
/* 801C09C0 001BD900  7C 08 03 A6 */	mtlr r0
/* 801C09C4 001BD904  4E 80 00 20 */	blr 

.global $$232$$2__dt__13TPolluterBaseFv
$$232$$2__dt__13TPolluterBaseFv:
/* 801C09C8 001BD908  38 63 FF E0 */	addi r3, r3, -32
/* 801C09CC 001BD90C  4B FF FC 7C */	b __dt__13TPolluterBaseFv

.section .ctors, "wa"  # 0x8036FBA0 - 0x8036FF80
	.incbin "baserom.dol", 0x36CD60, 0x4

.section .rodata, "wa"  # 0x8036FFA0 - 0x803A8380
$$22600:
	.incbin "baserom.dol", 0x38CBD0, 0x28
$$22703:
	.incbin "baserom.dol", 0x38CBF8, 0x18

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global __vt__23TMapObjRevivalPollution
__vt__23TMapObjRevivalPollution:
	.incbin "baserom.dol", 0x3C8410, 0x24
.global __vt__13TPolluterBase
__vt__13TPolluterBase:
	.incbin "baserom.dol", 0x3C8434, 0x164

.section .sdata2, "wa"  # 0x8040B460 - 0x80414020
$$22649:
	.incbin "baserom.dol", 0x3E8540, 0x4
$$22704:
	.incbin "baserom.dol", 0x3E8544, 0x4

.section .bss, "wa"  # 0x803E6000 - 0x80408AC0
$$22737:
	.skip 0xC
$$22738:
	.skip 0xC
$$22739:
	.skip 0xC
$$22740:
	.skip 0xC
$$22741:
	.skip 0xC
$$22742:
	.skip 0xC
$$22743:
	.skip 0xC
$$22744:
	.skip 0xC
$$22745:
	.skip 0xC
$$22746:
	.skip 0xC
$$22747:
	.skip 0xC
$$22748:
	.skip 0xC
$$22749:
	.skip 0xC
$$22750:
	.skip 0xC
$$22751:
	.skip 0x10
