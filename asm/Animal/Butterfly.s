.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global __dt__19TButterfloidManagerFv
__dt__19TButterfloidManagerFv:
/* 8036F2F4 0036C234  7C 08 02 A6 */	mflr r0
/* 8036F2F8 0036C238  90 01 00 04 */	stw r0, 4(r1)
/* 8036F2FC 0036C23C  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 8036F300 0036C240  93 E1 00 14 */	stw r31, 0x14(r1)
/* 8036F304 0036C244  3B E4 00 00 */	addi r31, r4, 0
/* 8036F308 0036C248  93 C1 00 10 */	stw r30, 0x10(r1)
/* 8036F30C 0036C24C  7C 7E 1B 79 */	or. r30, r3, r3
/* 8036F310 0036C250  41 82 00 2C */	beq lbl_8036F33C
/* 8036F314 0036C254  3C 60 80 3E */	lis r3, __vt__19TButterfloidManager@ha
/* 8036F318 0036C258  38 03 5D C8 */	addi r0, r3, __vt__19TButterfloidManager@l
/* 8036F31C 0036C25C  90 1E 00 00 */	stw r0, 0(r30)
/* 8036F320 0036C260  38 7E 00 00 */	addi r3, r30, 0
/* 8036F324 0036C264  38 80 00 00 */	li r4, 0
/* 8036F328 0036C268  4B EE 36 81 */	bl __dt__13TEnemyManagerFv
/* 8036F32C 0036C26C  7F E0 07 35 */	extsh. r0, r31
/* 8036F330 0036C270  40 81 00 0C */	ble lbl_8036F33C
/* 8036F334 0036C274  7F C3 F3 78 */	mr r3, r30
/* 8036F338 0036C278  4B C9 D7 79 */	bl __dl__FPv
lbl_8036F33C:
/* 8036F33C 0036C27C  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 8036F340 0036C280  7F C3 F3 78 */	mr r3, r30
/* 8036F344 0036C284  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 8036F348 0036C288  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 8036F34C 0036C28C  7C 08 03 A6 */	mtlr r0
/* 8036F350 0036C290  38 21 00 18 */	addi r1, r1, 0x18
/* 8036F354 0036C294  4E 80 00 20 */	blr 

.global createModelData__19TButterfloidManagerFv
createModelData__19TButterfloidManagerFv:
/* 8036F358 0036C298  7C 08 02 A6 */	mflr r0
/* 8036F35C 0036C29C  3C 80 80 3B */	lis r4, entry$2459@ha
/* 8036F360 0036C2A0  90 01 00 04 */	stw r0, 4(r1)
/* 8036F364 0036C2A4  38 84 83 1C */	addi r4, r4, entry$2459@l
/* 8036F368 0036C2A8  94 21 FF F8 */	stwu r1, -8(r1)
/* 8036F36C 0036C2AC  81 83 00 00 */	lwz r12, 0(r3)
/* 8036F370 0036C2B0  81 8C 00 2C */	lwz r12, 0x2c(r12)
/* 8036F374 0036C2B4  7D 88 03 A6 */	mtlr r12
/* 8036F378 0036C2B8  4E 80 00 21 */	blrl 
/* 8036F37C 0036C2BC  80 01 00 0C */	lwz r0, 0xc(r1)
/* 8036F380 0036C2C0  38 21 00 08 */	addi r1, r1, 8
/* 8036F384 0036C2C4  7C 08 03 A6 */	mtlr r0
/* 8036F388 0036C2C8  4E 80 00 20 */	blr 

.global __ct__19TButterfloidManagerFPCc
__ct__19TButterfloidManagerFPCc:
/* 8036F38C 0036C2CC  7C 08 02 A6 */	mflr r0
/* 8036F390 0036C2D0  90 01 00 04 */	stw r0, 4(r1)
/* 8036F394 0036C2D4  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 8036F398 0036C2D8  93 E1 00 14 */	stw r31, 0x14(r1)
/* 8036F39C 0036C2DC  7C 7F 1B 78 */	mr r31, r3
/* 8036F3A0 0036C2E0  4B EE 36 9D */	bl __ct__13TEnemyManagerFPCc
/* 8036F3A4 0036C2E4  3C 60 80 3E */	lis r3, __vt__19TButterfloidManager@ha
/* 8036F3A8 0036C2E8  38 03 5D C8 */	addi r0, r3, __vt__19TButterfloidManager@l
/* 8036F3AC 0036C2EC  90 1F 00 00 */	stw r0, 0(r31)
/* 8036F3B0 0036C2F0  7F E3 FB 78 */	mr r3, r31
/* 8036F3B4 0036C2F4  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 8036F3B8 0036C2F8  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 8036F3BC 0036C2FC  38 21 00 18 */	addi r1, r1, 0x18
/* 8036F3C0 0036C300  7C 08 03 A6 */	mtlr r0
/* 8036F3C4 0036C304  4E 80 00 20 */	blr 

.global createRealoidActor__12TButterfloidFP6MActor
createRealoidActor__12TButterfloidFP6MActor:
/* 8036F3C8 0036C308  7C 08 02 A6 */	mflr r0
/* 8036F3CC 0036C30C  90 01 00 04 */	stw r0, 4(r1)
/* 8036F3D0 0036C310  94 21 FF E0 */	stwu r1, -0x20(r1)
/* 8036F3D4 0036C314  93 E1 00 1C */	stw r31, 0x1c(r1)
/* 8036F3D8 0036C318  3B E4 00 00 */	addi r31, r4, 0
/* 8036F3DC 0036C31C  93 C1 00 18 */	stw r30, 0x18(r1)
/* 8036F3E0 0036C320  3B C3 00 00 */	addi r30, r3, 0
/* 8036F3E4 0036C324  38 60 00 AC */	li r3, 0xac
/* 8036F3E8 0036C328  93 A1 00 14 */	stw r29, 0x14(r1)
/* 8036F3EC 0036C32C  4B C9 D4 C5 */	bl __nw__FUl
/* 8036F3F0 0036C330  7C 7D 1B 79 */	or. r29, r3, r3
/* 8036F3F4 0036C334  41 82 00 28 */	beq lbl_8036F41C
/* 8036F3F8 0036C338  38 7D 00 00 */	addi r3, r29, 0
/* 8036F3FC 0036C33C  38 9F 00 00 */	addi r4, r31, 0
/* 8036F400 0036C340  4B FF 66 45 */	bl __ct__13TRealoidActorFP6MActor
/* 8036F404 0036C344  3C 60 80 3E */	lis r3, __vt__10TButterfly@ha
/* 8036F408 0036C348  38 63 5F 34 */	addi r3, r3, __vt__10TButterfly@l
/* 8036F40C 0036C34C  90 7D 00 00 */	stw r3, 0(r29)
/* 8036F410 0036C350  38 03 00 24 */	addi r0, r3, 0x24
/* 8036F414 0036C354  90 1D 00 20 */	stw r0, 0x20(r29)
/* 8036F418 0036C358  93 DD 00 A8 */	stw r30, 0xa8(r29)
lbl_8036F41C:
/* 8036F41C 0036C35C  80 01 00 24 */	lwz r0, 0x24(r1)
/* 8036F420 0036C360  7F A3 EB 78 */	mr r3, r29
/* 8036F424 0036C364  83 E1 00 1C */	lwz r31, 0x1c(r1)
/* 8036F428 0036C368  83 C1 00 18 */	lwz r30, 0x18(r1)
/* 8036F42C 0036C36C  7C 08 03 A6 */	mtlr r0
/* 8036F430 0036C370  83 A1 00 14 */	lwz r29, 0x14(r1)
/* 8036F434 0036C374  38 21 00 20 */	addi r1, r1, 0x20
/* 8036F438 0036C378  4E 80 00 20 */	blr 

.global load__12TButterfloidFR20JSUMemoryInputStream
load__12TButterfloidFR20JSUMemoryInputStream:
/* 8036F43C 0036C37C  7C 08 02 A6 */	mflr r0
/* 8036F440 0036C380  38 C0 00 00 */	li r6, 0
/* 8036F444 0036C384  90 01 00 04 */	stw r0, 4(r1)
/* 8036F448 0036C388  94 21 FE F8 */	stwu r1, -0x108(r1)
/* 8036F44C 0036C38C  93 E1 01 04 */	stw r31, 0x104(r1)
/* 8036F450 0036C390  93 C1 01 00 */	stw r30, 0x100(r1)
/* 8036F454 0036C394  93 A1 00 FC */	stw r29, 0xfc(r1)
/* 8036F458 0036C398  7C 7D 1B 78 */	mr r29, r3
/* 8036F45C 0036C39C  93 81 00 F8 */	stw r28, 0xf8(r1)
/* 8036F460 0036C3A0  3B 84 00 00 */	addi r28, r4, 0
/* 8036F464 0036C3A4  80 03 01 58 */	lwz r0, 0x158(r3)
/* 8036F468 0036C3A8  3C 60 80 3B */	lis r3, $$21490@ha
/* 8036F46C 0036C3AC  3B E3 82 00 */	addi r31, r3, $$21490@l
/* 8036F470 0036C3B0  54 00 10 3A */	slwi r0, r0, 2
/* 8036F474 0036C3B4  7C 7F 02 14 */	add r3, r31, r0
/* 8036F478 0036C3B8  80 A3 01 10 */	lwz r5, 0x110(r3)
/* 8036F47C 0036C3BC  38 7D 00 00 */	addi r3, r29, 0
/* 8036F480 0036C3C0  4B FF 60 31 */	bl loadDefault__8TRealoidFR20JSUMemoryInputStreamPCci
/* 8036F484 0036C3C4  38 7C 00 00 */	addi r3, r28, 0
/* 8036F488 0036C3C8  38 81 00 9C */	addi r4, r1, 0x9c
/* 8036F48C 0036C3CC  38 A0 00 04 */	li r5, 4
/* 8036F490 0036C3D0  4B C9 F1 01 */	bl read__14JSUInputStreamFPvl
/* 8036F494 0036C3D4  80 1D 01 58 */	lwz r0, 0x158(r29)
/* 8036F498 0036C3D8  2C 00 00 01 */	cmpwi r0, 1
/* 8036F49C 0036C3DC  41 82 00 68 */	beq lbl_8036F504
/* 8036F4A0 0036C3E0  40 80 00 10 */	bge lbl_8036F4B0
/* 8036F4A4 0036C3E4  2C 00 00 00 */	cmpwi r0, 0
/* 8036F4A8 0036C3E8  40 80 00 14 */	bge lbl_8036F4BC
/* 8036F4AC 0036C3EC  48 00 00 B0 */	b lbl_8036F55C
lbl_8036F4B0:
/* 8036F4B0 0036C3F0  2C 00 00 03 */	cmpwi r0, 3
/* 8036F4B4 0036C3F4  40 80 00 A8 */	bge lbl_8036F55C
/* 8036F4B8 0036C3F8  48 00 00 60 */	b lbl_8036F518
lbl_8036F4BC:
/* 8036F4BC 0036C3FC  C0 02 0B 6C */	lfs f0, $$22490@sda21(r2)
/* 8036F4C0 0036C400  38 C1 00 B8 */	addi r6, r1, 0xb8
/* 8036F4C4 0036C404  C0 22 0B 68 */	lfs f1, $$22489@sda21(r2)
/* 8036F4C8 0036C408  38 A1 00 AC */	addi r5, r1, 0xac
/* 8036F4CC 0036C40C  D0 01 00 AC */	stfs f0, 0xac(r1)
/* 8036F4D0 0036C410  38 81 00 A0 */	addi r4, r1, 0xa0
/* 8036F4D4 0036C414  D0 21 00 B8 */	stfs f1, 0xb8(r1)
/* 8036F4D8 0036C418  38 62 0B 70 */	addi r3, r2, $$22491@sda21
/* 8036F4DC 0036C41C  D0 01 00 A0 */	stfs f0, 0xa0(r1)
/* 8036F4E0 0036C420  D0 21 00 BC */	stfs f1, 0xbc(r1)
/* 8036F4E4 0036C424  D0 01 00 B0 */	stfs f0, 0xb0(r1)
/* 8036F4E8 0036C428  D0 01 00 A4 */	stfs f0, 0xa4(r1)
/* 8036F4EC 0036C42C  D0 21 00 C0 */	stfs f1, 0xc0(r1)
/* 8036F4F0 0036C430  D0 01 00 B4 */	stfs f0, 0xb4(r1)
/* 8036F4F4 0036C434  D0 01 00 A8 */	stfs f0, 0xa8(r1)
/* 8036F4F8 0036C438  4B E1 EB A5 */	bl newAndRegisterObj__18TMapObjBaseManagerFPCcRCQ29JGeometry8TVec3$$0f$$1RCQ29JGeometry8TVec3$$0f$$1RCQ29JGeometry8TVec3$$0f$$1
/* 8036F4FC 0036C43C  90 7D 01 5C */	stw r3, 0x15c(r29)
/* 8036F500 0036C440  48 00 00 5C */	b lbl_8036F55C
lbl_8036F504:
/* 8036F504 0036C444  80 61 00 9C */	lwz r3, 0x9c(r1)
/* 8036F508 0036C448  38 82 0B 78 */	addi r4, r2, $$22492@sda21
/* 8036F50C 0036C44C  4B E1 E5 1D */	bl newAndRegisterObjByEventID__18TMapObjBaseManagerFUlPCc
/* 8036F510 0036C450  90 7D 01 5C */	stw r3, 0x15c(r29)
/* 8036F514 0036C454  48 00 00 48 */	b lbl_8036F55C
lbl_8036F518:
/* 8036F518 0036C458  C0 02 0B 6C */	lfs f0, $$22490@sda21(r2)
/* 8036F51C 0036C45C  38 C1 00 DC */	addi r6, r1, 0xdc
/* 8036F520 0036C460  C0 22 0B 68 */	lfs f1, $$22489@sda21(r2)
/* 8036F524 0036C464  38 A1 00 D0 */	addi r5, r1, 0xd0
/* 8036F528 0036C468  D0 01 00 D0 */	stfs f0, 0xd0(r1)
/* 8036F52C 0036C46C  38 81 00 C4 */	addi r4, r1, 0xc4
/* 8036F530 0036C470  D0 21 00 DC */	stfs f1, 0xdc(r1)
/* 8036F534 0036C474  38 7F 01 4C */	addi r3, r31, 0x14c
/* 8036F538 0036C478  D0 01 00 C4 */	stfs f0, 0xc4(r1)
/* 8036F53C 0036C47C  D0 21 00 E0 */	stfs f1, 0xe0(r1)
/* 8036F540 0036C480  D0 01 00 D4 */	stfs f0, 0xd4(r1)
/* 8036F544 0036C484  D0 01 00 C8 */	stfs f0, 0xc8(r1)
/* 8036F548 0036C488  D0 21 00 E4 */	stfs f1, 0xe4(r1)
/* 8036F54C 0036C48C  D0 01 00 D8 */	stfs f0, 0xd8(r1)
/* 8036F550 0036C490  D0 01 00 CC */	stfs f0, 0xcc(r1)
/* 8036F554 0036C494  4B E1 EB 49 */	bl newAndRegisterObj__18TMapObjBaseManagerFPCcRCQ29JGeometry8TVec3$$0f$$1RCQ29JGeometry8TVec3$$0f$$1RCQ29JGeometry8TVec3$$0f$$1
/* 8036F558 0036C498  90 7D 01 5C */	stw r3, 0x15c(r29)
lbl_8036F55C:
/* 8036F55C 0036C49C  C0 02 0B 7C */	lfs f0, $$22586@sda21(r2)
/* 8036F560 0036C4A0  80 7D 01 50 */	lwz r3, 0x150(r29)
/* 8036F564 0036C4A4  D0 03 00 24 */	stfs f0, 0x24(r3)
/* 8036F568 0036C4A8  C0 02 0B 80 */	lfs f0, $$22587@sda21(r2)
/* 8036F56C 0036C4AC  80 7D 01 50 */	lwz r3, 0x150(r29)
/* 8036F570 0036C4B0  D0 03 00 20 */	stfs f0, 0x20(r3)
/* 8036F574 0036C4B4  C0 02 0B 84 */	lfs f0, $$22588@sda21(r2)
/* 8036F578 0036C4B8  80 7D 01 50 */	lwz r3, 0x150(r29)
/* 8036F57C 0036C4BC  D0 03 00 28 */	stfs f0, 0x28(r3)
/* 8036F580 0036C4C0  C0 02 0B 88 */	lfs f0, $$22589@sda21(r2)
/* 8036F584 0036C4C4  80 7D 01 50 */	lwz r3, 0x150(r29)
/* 8036F588 0036C4C8  D0 03 00 2C */	stfs f0, 0x2c(r3)
/* 8036F58C 0036C4CC  C0 02 0B 8C */	lfs f0, $$22590@sda21(r2)
/* 8036F590 0036C4D0  80 7D 01 50 */	lwz r3, 0x150(r29)
/* 8036F594 0036C4D4  D0 03 00 30 */	stfs f0, 0x30(r3)
/* 8036F598 0036C4D8  C0 02 0B 90 */	lfs f0, $$22591@sda21(r2)
/* 8036F59C 0036C4DC  80 7D 01 50 */	lwz r3, 0x150(r29)
/* 8036F5A0 0036C4E0  D0 03 00 34 */	stfs f0, 0x34(r3)
/* 8036F5A4 0036C4E4  80 6D 98 D8 */	lwz r3, gpMarioAddress@sda21(r13)
/* 8036F5A8 0036C4E8  C0 02 0B 6C */	lfs f0, $$22490@sda21(r2)
/* 8036F5AC 0036C4EC  90 61 00 E8 */	stw r3, 0xe8(r1)
/* 8036F5B0 0036C4F0  28 03 00 00 */	cmplwi r3, 0
/* 8036F5B4 0036C4F4  D0 01 00 EC */	stfs f0, 0xec(r1)
/* 8036F5B8 0036C4F8  D0 01 00 F0 */	stfs f0, 0xf0(r1)
/* 8036F5BC 0036C4FC  D0 01 00 F4 */	stfs f0, 0xf4(r1)
/* 8036F5C0 0036C500  41 82 00 1C */	beq lbl_8036F5DC
/* 8036F5C4 0036C504  C0 43 00 18 */	lfs f2, 0x18(r3)
/* 8036F5C8 0036C508  C0 23 00 14 */	lfs f1, 0x14(r3)
/* 8036F5CC 0036C50C  C0 03 00 10 */	lfs f0, 0x10(r3)
/* 8036F5D0 0036C510  D0 01 00 EC */	stfs f0, 0xec(r1)
/* 8036F5D4 0036C514  D0 21 00 F0 */	stfs f1, 0xf0(r1)
/* 8036F5D8 0036C518  D0 41 00 F4 */	stfs f2, 0xf4(r1)
lbl_8036F5DC:
/* 8036F5DC 0036C51C  80 9D 01 50 */	lwz r4, 0x150(r29)
/* 8036F5E0 0036C520  3B C0 00 00 */	li r30, 0
/* 8036F5E4 0036C524  80 01 00 E8 */	lwz r0, 0xe8(r1)
/* 8036F5E8 0036C528  3B 80 00 00 */	li r28, 0
/* 8036F5EC 0036C52C  90 04 00 5C */	stw r0, 0x5c(r4)
/* 8036F5F0 0036C530  80 61 00 EC */	lwz r3, 0xec(r1)
/* 8036F5F4 0036C534  80 01 00 F0 */	lwz r0, 0xf0(r1)
/* 8036F5F8 0036C538  90 64 00 60 */	stw r3, 0x60(r4)
/* 8036F5FC 0036C53C  90 04 00 64 */	stw r0, 0x64(r4)
/* 8036F600 0036C540  80 01 00 F4 */	lwz r0, 0xf4(r1)
/* 8036F604 0036C544  90 04 00 68 */	stw r0, 0x68(r4)
/* 8036F608 0036C548  C0 02 0B 94 */	lfs f0, $$22592@sda21(r2)
/* 8036F60C 0036C54C  80 7D 01 50 */	lwz r3, 0x150(r29)
/* 8036F610 0036C550  D0 03 00 6C */	stfs f0, 0x6c(r3)
/* 8036F614 0036C554  C0 02 0B 98 */	lfs f0, $$22593@sda21(r2)
/* 8036F618 0036C558  80 7D 01 50 */	lwz r3, 0x150(r29)
/* 8036F61C 0036C55C  D0 03 00 70 */	stfs f0, 0x70(r3)
/* 8036F620 0036C560  80 7D 01 50 */	lwz r3, 0x150(r29)
/* 8036F624 0036C564  80 03 00 1C */	lwz r0, 0x1c(r3)
/* 8036F628 0036C568  60 00 00 02 */	ori r0, r0, 2
/* 8036F62C 0036C56C  90 03 00 1C */	stw r0, 0x1c(r3)
/* 8036F630 0036C570  48 00 00 20 */	b lbl_8036F650
lbl_8036F634:
/* 8036F634 0036C574  80 7D 01 54 */	lwz r3, 0x154(r29)
/* 8036F638 0036C578  38 9F 01 5C */	addi r4, r31, 0x15c
/* 8036F63C 0036C57C  7C 63 E0 2E */	lwzx r3, r3, r28
/* 8036F640 0036C580  80 63 00 70 */	lwz r3, 0x70(r3)
/* 8036F644 0036C584  4B D6 4B 19 */	bl setBck__6MActorFPCc
/* 8036F648 0036C588  3B DE 00 01 */	addi r30, r30, 1
/* 8036F64C 0036C58C  3B 9C 00 04 */	addi r28, r28, 4
lbl_8036F650:
/* 8036F650 0036C590  80 7D 01 50 */	lwz r3, 0x150(r29)
/* 8036F654 0036C594  80 03 00 10 */	lwz r0, 0x10(r3)
/* 8036F658 0036C598  7C 1E 00 00 */	cmpw r30, r0
/* 8036F65C 0036C59C  41 80 FF D8 */	blt lbl_8036F634
/* 8036F660 0036C5A0  3B 80 00 00 */	li r28, 0
/* 8036F664 0036C5A4  3B C0 00 00 */	li r30, 0
/* 8036F668 0036C5A8  48 00 00 24 */	b lbl_8036F68C
lbl_8036F66C:
/* 8036F66C 0036C5AC  80 7D 01 54 */	lwz r3, 0x154(r29)
/* 8036F670 0036C5B0  7C 63 F0 2E */	lwzx r3, r3, r30
/* 8036F674 0036C5B4  81 83 00 00 */	lwz r12, 0(r3)
/* 8036F678 0036C5B8  81 8C 00 B4 */	lwz r12, 0xb4(r12)
/* 8036F67C 0036C5BC  7D 88 03 A6 */	mtlr r12
/* 8036F680 0036C5C0  4E 80 00 21 */	blrl 
/* 8036F684 0036C5C4  3B 9C 00 01 */	addi r28, r28, 1
/* 8036F688 0036C5C8  3B DE 00 04 */	addi r30, r30, 4
lbl_8036F68C:
/* 8036F68C 0036C5CC  80 7D 01 50 */	lwz r3, 0x150(r29)
/* 8036F690 0036C5D0  80 03 00 10 */	lwz r0, 0x10(r3)
/* 8036F694 0036C5D4  7C 1C 00 00 */	cmpw r28, r0
/* 8036F698 0036C5D8  41 80 FF D4 */	blt lbl_8036F66C
/* 8036F69C 0036C5DC  80 01 01 0C */	lwz r0, 0x10c(r1)
/* 8036F6A0 0036C5E0  83 E1 01 04 */	lwz r31, 0x104(r1)
/* 8036F6A4 0036C5E4  83 C1 01 00 */	lwz r30, 0x100(r1)
/* 8036F6A8 0036C5E8  7C 08 03 A6 */	mtlr r0
/* 8036F6AC 0036C5EC  83 A1 00 FC */	lwz r29, 0xfc(r1)
/* 8036F6B0 0036C5F0  83 81 00 F8 */	lwz r28, 0xf8(r1)
/* 8036F6B4 0036C5F4  38 21 01 08 */	addi r1, r1, 0x108
/* 8036F6B8 0036C5F8  4E 80 00 20 */	blr 

.global init__12TButterfloidFP12TLiveManager
init__12TButterfloidFP12TLiveManager:
/* 8036F6BC 0036C5FC  7C 08 02 A6 */	mflr r0
/* 8036F6C0 0036C600  90 01 00 04 */	stw r0, 4(r1)
/* 8036F6C4 0036C604  94 21 FF D8 */	stwu r1, -0x28(r1)
/* 8036F6C8 0036C608  93 E1 00 24 */	stw r31, 0x24(r1)
/* 8036F6CC 0036C60C  7C 7F 1B 78 */	mr r31, r3
/* 8036F6D0 0036C610  90 83 00 70 */	stw r4, 0x70(r3)
/* 8036F6D4 0036C614  7F E4 FB 78 */	mr r4, r31
/* 8036F6D8 0036C618  80 63 00 70 */	lwz r3, 0x70(r3)
/* 8036F6DC 0036C61C  4B DA 52 E1 */	bl manageActor__12TLiveManagerFP10TLiveActor
/* 8036F6E0 0036C620  88 0D A1 10 */	lbz r0, init$localstatic1$theNerve__31TNerveWaitForever$$010TLiveActor$$1Fv@sda21(r13)
/* 8036F6E4 0036C624  7C 00 07 75 */	extsb. r0, r0
/* 8036F6E8 0036C628  40 82 00 3C */	bne lbl_8036F724
/* 8036F6EC 0036C62C  3C 60 80 3C */	lis r3, __vt__24TNerveBase$$010TLiveActor$$1@ha
/* 8036F6F0 0036C630  38 03 92 60 */	addi r0, r3, __vt__24TNerveBase$$010TLiveActor$$1@l
/* 8036F6F4 0036C634  3C 60 80 3E */	lis r3, __vt__31TNerveWaitForever$$010TLiveActor$$1@ha
/* 8036F6F8 0036C638  90 0D A1 14 */	stw r0, instance$localstatic0$theNerve__31TNerveWaitForever$$010TLiveActor$$1Fv@sda21(r13)
/* 8036F6FC 0036C63C  38 03 A4 5C */	addi r0, r3, __vt__31TNerveWaitForever$$010TLiveActor$$1@l
/* 8036F700 0036C640  3C 80 80 2B */	lis r4, __dt__31TNerveWaitForever$$010TLiveActor$$1Fv@ha
/* 8036F704 0036C644  90 0D A1 14 */	stw r0, instance$localstatic0$theNerve__31TNerveWaitForever$$010TLiveActor$$1Fv@sda21(r13)
/* 8036F708 0036C648  3C 60 80 40 */	lis r3, $$22422@ha
/* 8036F70C 0036C64C  38 A3 0B 80 */	addi r5, r3, $$22422@l
/* 8036F710 0036C650  38 84 5E 7C */	addi r4, r4, __dt__31TNerveWaitForever$$010TLiveActor$$1Fv@l
/* 8036F714 0036C654  38 6D A1 14 */	addi r3, r13, instance$localstatic0$theNerve__31TNerveWaitForever$$010TLiveActor$$1Fv@sda21
/* 8036F718 0036C658  4B D1 30 11 */	bl __register_global_object
/* 8036F71C 0036C65C  38 00 00 01 */	li r0, 1
/* 8036F720 0036C660  98 0D A1 10 */	stb r0, init$localstatic1$theNerve__31TNerveWaitForever$$010TLiveActor$$1Fv@sda21(r13)
lbl_8036F724:
/* 8036F724 0036C664  81 1F 00 8C */	lwz r8, 0x8c(r31)
/* 8036F728 0036C668  38 E0 00 00 */	li r7, 0
/* 8036F72C 0036C66C  38 0D A1 14 */	addi r0, r13, instance$localstatic0$theNerve__31TNerveWaitForever$$010TLiveActor$$1Fv@sda21
/* 8036F730 0036C670  90 E8 00 08 */	stw r7, 8(r8)
/* 8036F734 0036C674  38 7F 00 00 */	addi r3, r31, 0
/* 8036F738 0036C678  38 80 00 00 */	li r4, 0
/* 8036F73C 0036C67C  90 E8 00 20 */	stw r7, 0x20(r8)
/* 8036F740 0036C680  38 A0 00 01 */	li r5, 1
/* 8036F744 0036C684  38 C0 00 00 */	li r6, 0
/* 8036F748 0036C688  90 08 00 14 */	stw r0, 0x14(r8)
/* 8036F74C 0036C68C  90 08 00 18 */	stw r0, 0x18(r8)
/* 8036F750 0036C690  90 E8 00 1C */	stw r7, 0x1c(r8)
/* 8036F754 0036C694  C0 22 0B 6C */	lfs f1, $$22490@sda21(r2)
/* 8036F758 0036C698  FC 40 08 90 */	fmr f2, f1
/* 8036F75C 0036C69C  FC 60 08 90 */	fmr f3, f1
/* 8036F760 0036C6A0  FC 80 08 90 */	fmr f4, f1
/* 8036F764 0036C6A4  4B DA E0 95 */	bl initHitActor__9THitActorFUlUsiffff
/* 8036F768 0036C6A8  80 1F 00 64 */	lwz r0, 0x64(r31)
/* 8036F76C 0036C6AC  60 00 00 01 */	ori r0, r0, 1
/* 8036F770 0036C6B0  90 1F 00 64 */	stw r0, 0x64(r31)
/* 8036F774 0036C6B4  80 01 00 2C */	lwz r0, 0x2c(r1)
/* 8036F778 0036C6B8  83 E1 00 24 */	lwz r31, 0x24(r1)
/* 8036F77C 0036C6BC  38 21 00 28 */	addi r1, r1, 0x28
/* 8036F780 0036C6C0  7C 08 03 A6 */	mtlr r0
/* 8036F784 0036C6C4  4E 80 00 20 */	blr 

.global __ct__12TButterfloidFiPCc
__ct__12TButterfloidFiPCc:
/* 8036F788 0036C6C8  7C 08 02 A6 */	mflr r0
/* 8036F78C 0036C6CC  90 01 00 04 */	stw r0, 4(r1)
/* 8036F790 0036C6D0  94 21 FF E0 */	stwu r1, -0x20(r1)
/* 8036F794 0036C6D4  93 E1 00 1C */	stw r31, 0x1c(r1)
/* 8036F798 0036C6D8  3B E4 00 00 */	addi r31, r4, 0
/* 8036F79C 0036C6DC  38 85 00 00 */	addi r4, r5, 0
/* 8036F7A0 0036C6E0  93 C1 00 18 */	stw r30, 0x18(r1)
/* 8036F7A4 0036C6E4  3B C3 00 00 */	addi r30, r3, 0
/* 8036F7A8 0036C6E8  4B FF 5E B5 */	bl __ct__8TRealoidFPCc
/* 8036F7AC 0036C6EC  3C 60 80 3E */	lis r3, __vt__12TButterfloid@ha
/* 8036F7B0 0036C6F0  38 63 5E 1C */	addi r3, r3, __vt__12TButterfloid@l
/* 8036F7B4 0036C6F4  90 7E 00 00 */	stw r3, 0(r30)
/* 8036F7B8 0036C6F8  38 63 00 24 */	addi r3, r3, 0x24
/* 8036F7BC 0036C6FC  38 00 00 00 */	li r0, 0
/* 8036F7C0 0036C700  90 7E 00 20 */	stw r3, 0x20(r30)
/* 8036F7C4 0036C704  7F C3 F3 78 */	mr r3, r30
/* 8036F7C8 0036C708  93 FE 01 58 */	stw r31, 0x158(r30)
/* 8036F7CC 0036C70C  90 1E 01 5C */	stw r0, 0x15c(r30)
/* 8036F7D0 0036C710  90 1E 01 60 */	stw r0, 0x160(r30)
/* 8036F7D4 0036C714  80 01 00 24 */	lwz r0, 0x24(r1)
/* 8036F7D8 0036C718  83 E1 00 1C */	lwz r31, 0x1c(r1)
/* 8036F7DC 0036C71C  83 C1 00 18 */	lwz r30, 0x18(r1)
/* 8036F7E0 0036C720  38 21 00 20 */	addi r1, r1, 0x20
/* 8036F7E4 0036C724  7C 08 03 A6 */	mtlr r0
/* 8036F7E8 0036C728  4E 80 00 20 */	blr 

.global receiveMessage__10TButterflyFP9THitActorUl
receiveMessage__10TButterflyFP9THitActorUl:
/* 8036F7EC 0036C72C  7C 08 02 A6 */	mflr r0
/* 8036F7F0 0036C730  2C 05 00 08 */	cmpwi r5, 8
/* 8036F7F4 0036C734  90 01 00 04 */	stw r0, 4(r1)
/* 8036F7F8 0036C738  94 21 FF C8 */	stwu r1, -0x38(r1)
/* 8036F7FC 0036C73C  93 E1 00 34 */	stw r31, 0x34(r1)
/* 8036F800 0036C740  3B E3 00 00 */	addi r31, r3, 0
/* 8036F804 0036C744  93 C1 00 30 */	stw r30, 0x30(r1)
/* 8036F808 0036C748  41 82 00 50 */	beq lbl_8036F858
/* 8036F80C 0036C74C  40 80 00 10 */	bge lbl_8036F81C
/* 8036F810 0036C750  2C 05 00 04 */	cmpwi r5, 4
/* 8036F814 0036C754  41 82 00 14 */	beq lbl_8036F828
/* 8036F818 0036C758  48 00 01 34 */	b lbl_8036F94C
lbl_8036F81C:
/* 8036F81C 0036C75C  2C 05 00 0B */	cmpwi r5, 0xb
/* 8036F820 0036C760  41 82 00 54 */	beq lbl_8036F874
/* 8036F824 0036C764  48 00 01 28 */	b lbl_8036F94C
lbl_8036F828:
/* 8036F828 0036C768  80 1F 00 68 */	lwz r0, 0x68(r31)
/* 8036F82C 0036C76C  28 00 00 00 */	cmplwi r0, 0
/* 8036F830 0036C770  40 82 01 1C */	bne lbl_8036F94C
/* 8036F834 0036C774  90 9F 00 68 */	stw r4, 0x68(r31)
/* 8036F838 0036C778  38 BF 00 10 */	addi r5, r31, 0x10
/* 8036F83C 0036C77C  38 80 00 E7 */	li r4, 0xe7
/* 8036F840 0036C780  80 6D 97 98 */	lwz r3, gpMarioParticleManager@sda21(r13)
/* 8036F844 0036C784  38 C0 00 00 */	li r6, 0
/* 8036F848 0036C788  38 E0 00 00 */	li r7, 0
/* 8036F84C 0036C78C  4B D6 C2 D1 */	bl emit__21TMarioParticleManagerFlPCQ29JGeometry8TVec3$$0f$$1UcPCv
/* 8036F850 0036C790  38 60 00 01 */	li r3, 1
/* 8036F854 0036C794  48 00 00 FC */	b lbl_8036F950
lbl_8036F858:
/* 8036F858 0036C798  80 1F 00 68 */	lwz r0, 0x68(r31)
/* 8036F85C 0036C79C  28 00 00 00 */	cmplwi r0, 0
/* 8036F860 0036C7A0  41 82 00 EC */	beq lbl_8036F94C
/* 8036F864 0036C7A4  38 00 00 00 */	li r0, 0
/* 8036F868 0036C7A8  90 1F 00 68 */	stw r0, 0x68(r31)
/* 8036F86C 0036C7AC  38 60 00 01 */	li r3, 1
/* 8036F870 0036C7B0  48 00 00 E0 */	b lbl_8036F950
lbl_8036F874:
/* 8036F874 0036C7B4  80 1F 00 74 */	lwz r0, 0x74(r31)
/* 8036F878 0036C7B8  80 BF 00 A8 */	lwz r5, 0xa8(r31)
/* 8036F87C 0036C7BC  60 00 00 04 */	ori r0, r0, 4
/* 8036F880 0036C7C0  90 1F 00 74 */	stw r0, 0x74(r31)
/* 8036F884 0036C7C4  80 1F 00 64 */	lwz r0, 0x64(r31)
/* 8036F888 0036C7C8  60 00 00 01 */	ori r0, r0, 1
/* 8036F88C 0036C7CC  90 1F 00 64 */	stw r0, 0x64(r31)
/* 8036F890 0036C7D0  80 65 01 60 */	lwz r3, 0x160(r5)
/* 8036F894 0036C7D4  38 83 00 01 */	addi r4, r3, 1
/* 8036F898 0036C7D8  90 85 01 60 */	stw r4, 0x160(r5)
/* 8036F89C 0036C7DC  80 65 01 50 */	lwz r3, 0x150(r5)
/* 8036F8A0 0036C7E0  80 03 00 10 */	lwz r0, 0x10(r3)
/* 8036F8A4 0036C7E4  7C 04 00 00 */	cmpw r4, r0
/* 8036F8A8 0036C7E8  40 82 00 9C */	bne lbl_8036F944
/* 8036F8AC 0036C7EC  83 C5 01 5C */	lwz r30, 0x15c(r5)
/* 8036F8B0 0036C7F0  28 1E 00 00 */	cmplwi r30, 0
/* 8036F8B4 0036C7F4  41 82 00 90 */	beq lbl_8036F944
/* 8036F8B8 0036C7F8  80 7E 00 4C */	lwz r3, 0x4c(r30)
/* 8036F8BC 0036C7FC  3C 03 E0 00 */	addis r0, r3, 0xe000
/* 8036F8C0 0036C800  28 00 00 0E */	cmplwi r0, 0xe
/* 8036F8C4 0036C804  40 82 00 0C */	bne lbl_8036F8D0
/* 8036F8C8 0036C808  38 00 00 01 */	li r0, 1
/* 8036F8CC 0036C80C  48 00 00 08 */	b lbl_8036F8D4
lbl_8036F8D0:
/* 8036F8D0 0036C810  38 00 00 00 */	li r0, 0
lbl_8036F8D4:
/* 8036F8D4 0036C814  54 00 06 3F */	clrlwi. r0, r0, 0x18
/* 8036F8D8 0036C818  41 82 00 18 */	beq lbl_8036F8F0
/* 8036F8DC 0036C81C  3C 80 20 00 */	lis r4, 0x2000000E@ha
/* 8036F8E0 0036C820  80 6D 9A 18 */	lwz r3, gpItemManager@sda21(r13)
/* 8036F8E4 0036C824  38 84 00 0E */	addi r4, r4, 0x2000000E@l
/* 8036F8E8 0036C828  4B E1 F6 C1 */	bl makeObjAppear__18TMapObjBaseManagerFUl
/* 8036F8EC 0036C82C  7C 7E 1B 78 */	mr r30, r3
lbl_8036F8F0:
/* 8036F8F0 0036C830  28 1E 00 00 */	cmplwi r30, 0
/* 8036F8F4 0036C834  41 82 00 50 */	beq lbl_8036F944
/* 8036F8F8 0036C838  7F C3 F3 78 */	mr r3, r30
/* 8036F8FC 0036C83C  81 9E 00 00 */	lwz r12, 0(r30)
/* 8036F900 0036C840  81 8C 00 FC */	lwz r12, 0xfc(r12)
/* 8036F904 0036C844  7D 88 03 A6 */	mtlr r12
/* 8036F908 0036C848  4E 80 00 21 */	blrl 
/* 8036F90C 0036C84C  80 7F 00 10 */	lwz r3, 0x10(r31)
/* 8036F910 0036C850  80 1F 00 14 */	lwz r0, 0x14(r31)
/* 8036F914 0036C854  90 7E 00 10 */	stw r3, 0x10(r30)
/* 8036F918 0036C858  90 1E 00 14 */	stw r0, 0x14(r30)
/* 8036F91C 0036C85C  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 8036F920 0036C860  90 1E 00 18 */	stw r0, 0x18(r30)
/* 8036F924 0036C864  C0 22 0B 6C */	lfs f1, $$22490@sda21(r2)
/* 8036F928 0036C868  D0 3E 00 AC */	stfs f1, 0xac(r30)
/* 8036F92C 0036C86C  C0 02 0B 9C */	lfs f0, $$22616@sda21(r2)
/* 8036F930 0036C870  D0 1E 00 B0 */	stfs f0, 0xb0(r30)
/* 8036F934 0036C874  D0 3E 00 B4 */	stfs f1, 0xb4(r30)
/* 8036F938 0036C878  80 1E 00 F0 */	lwz r0, 0xf0(r30)
/* 8036F93C 0036C87C  54 00 07 34 */	rlwinm r0, r0, 0, 0x1c, 0x1a
/* 8036F940 0036C880  90 1E 00 F0 */	stw r0, 0xf0(r30)
lbl_8036F944:
/* 8036F944 0036C884  38 60 00 01 */	li r3, 1
/* 8036F948 0036C888  48 00 00 08 */	b lbl_8036F950
lbl_8036F94C:
/* 8036F94C 0036C88C  38 60 00 00 */	li r3, 0
lbl_8036F950:
/* 8036F950 0036C890  80 01 00 3C */	lwz r0, 0x3c(r1)
/* 8036F954 0036C894  83 E1 00 34 */	lwz r31, 0x34(r1)
/* 8036F958 0036C898  83 C1 00 30 */	lwz r30, 0x30(r1)
/* 8036F95C 0036C89C  7C 08 03 A6 */	mtlr r0
/* 8036F960 0036C8A0  38 21 00 38 */	addi r1, r1, 0x38
/* 8036F964 0036C8A4  4E 80 00 20 */	blr 

.global init__10TButterflyFv
init__10TButterflyFv:
/* 8036F968 0036C8A8  7C 08 02 A6 */	mflr r0
/* 8036F96C 0036C8AC  3C 80 10 00 */	lis r4, 0x10000030@ha
/* 8036F970 0036C8B0  90 01 00 04 */	stw r0, 4(r1)
/* 8036F974 0036C8B4  38 84 00 30 */	addi r4, r4, 0x10000030@l
/* 8036F978 0036C8B8  38 A0 00 00 */	li r5, 0
/* 8036F97C 0036C8BC  94 21 FF 98 */	stwu r1, -0x68(r1)
/* 8036F980 0036C8C0  38 C0 00 00 */	li r6, 0
/* 8036F984 0036C8C4  93 E1 00 64 */	stw r31, 0x64(r1)
/* 8036F988 0036C8C8  93 C1 00 60 */	stw r30, 0x60(r1)
/* 8036F98C 0036C8CC  C0 22 0B 6C */	lfs f1, $$22490@sda21(r2)
/* 8036F990 0036C8D0  C0 62 0B A0 */	lfs f3, $$22710@sda21(r2)
/* 8036F994 0036C8D4  90 61 00 08 */	stw r3, 8(r1)
/* 8036F998 0036C8D8  FC 40 08 90 */	fmr f2, f1
/* 8036F99C 0036C8DC  FC 80 18 90 */	fmr f4, f3
/* 8036F9A0 0036C8E0  80 61 00 08 */	lwz r3, 8(r1)
/* 8036F9A4 0036C8E4  4B DA DE 55 */	bl initHitActor__9THitActorFUlUsiffff
/* 8036F9A8 0036C8E8  80 81 00 08 */	lwz r4, 8(r1)
/* 8036F9AC 0036C8EC  3C 60 80 3B */	lis r3, $$22711@ha
/* 8036F9B0 0036C8F0  3B E3 83 6C */	addi r31, r3, $$22711@l
/* 8036F9B4 0036C8F4  80 04 00 64 */	lwz r0, 0x64(r4)
/* 8036F9B8 0036C8F8  38 7F 00 00 */	addi r3, r31, 0
/* 8036F9BC 0036C8FC  54 00 00 3C */	rlwinm r0, r0, 0, 0, 0x1e
/* 8036F9C0 0036C900  90 04 00 64 */	stw r0, 0x64(r4)
/* 8036F9C4 0036C904  80 04 00 64 */	lwz r0, 0x64(r4)
/* 8036F9C8 0036C908  60 00 00 02 */	ori r0, r0, 2
/* 8036F9CC 0036C90C  90 04 00 64 */	stw r0, 0x64(r4)
/* 8036F9D0 0036C910  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 8036F9D4 0036C914  83 C4 00 04 */	lwz r30, 4(r4)
/* 8036F9D8 0036C918  4B CD 4C B5 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 8036F9DC 0036C91C  81 9E 00 00 */	lwz r12, 0(r30)
/* 8036F9E0 0036C920  38 83 00 00 */	addi r4, r3, 0
/* 8036F9E4 0036C924  38 7E 00 00 */	addi r3, r30, 0
/* 8036F9E8 0036C928  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 8036F9EC 0036C92C  38 BF 00 00 */	addi r5, r31, 0
/* 8036F9F0 0036C930  7D 88 03 A6 */	mtlr r12
/* 8036F9F4 0036C934  4E 80 00 21 */	blrl 
/* 8036F9F8 0036C938  3B E3 00 10 */	addi r31, r3, 0x10
/* 8036F9FC 0036C93C  38 61 00 38 */	addi r3, r1, 0x38
/* 8036FA00 0036C940  38 9F 00 08 */	addi r4, r31, 8
/* 8036FA04 0036C944  4B CC 49 61 */	bl __ct__Q37JGadget36TList$$0Pv$$4Q27JGadget14TAllocator$$0Pv$$1$$18iteratorFPQ37JGadget36TList$$0Pv$$4Q27JGadget14TAllocator$$0Pv$$1$$16TNode_
/* 8036FA08 0036C948  80 01 00 38 */	lwz r0, 0x38(r1)
/* 8036FA0C 0036C94C  38 61 00 54 */	addi r3, r1, 0x54
/* 8036FA10 0036C950  38 81 00 34 */	addi r4, r1, 0x34
/* 8036FA14 0036C954  90 01 00 34 */	stw r0, 0x34(r1)
/* 8036FA18 0036C958  4B CD 52 19 */	bl __ct__Q37JGadget36TList$$0Pv$$4Q27JGadget14TAllocator$$0Pv$$1$$18iteratorFRCQ37JGadget36TList$$0Pv$$4Q27JGadget14TAllocator$$0Pv$$1$$18iterator
/* 8036FA1C 0036C95C  80 01 00 54 */	lwz r0, 0x54(r1)
/* 8036FA20 0036C960  38 A1 00 44 */	addi r5, r1, 0x44
/* 8036FA24 0036C964  38 9F 00 00 */	addi r4, r31, 0
/* 8036FA28 0036C968  90 01 00 50 */	stw r0, 0x50(r1)
/* 8036FA2C 0036C96C  38 61 00 40 */	addi r3, r1, 0x40
/* 8036FA30 0036C970  38 C1 00 08 */	addi r6, r1, 8
/* 8036FA34 0036C974  80 01 00 50 */	lwz r0, 0x50(r1)
/* 8036FA38 0036C978  90 01 00 44 */	stw r0, 0x44(r1)
/* 8036FA3C 0036C97C  4B CC 49 31 */	bl insert__Q27JGadget18TList_pointer_voidFQ37JGadget36TList$$0Pv$$4Q27JGadget14TAllocator$$0Pv$$1$$18iteratorRCPv
/* 8036FA40 0036C980  80 01 00 6C */	lwz r0, 0x6c(r1)
/* 8036FA44 0036C984  83 E1 00 64 */	lwz r31, 0x64(r1)
/* 8036FA48 0036C988  83 C1 00 60 */	lwz r30, 0x60(r1)
/* 8036FA4C 0036C98C  7C 08 03 A6 */	mtlr r0
/* 8036FA50 0036C990  38 21 00 68 */	addi r1, r1, 0x68
/* 8036FA54 0036C994  4E 80 00 20 */	blr 

.global __dt__12TButterfloidFv
__dt__12TButterfloidFv:
/* 8036FA58 0036C998  7C 08 02 A6 */	mflr r0
/* 8036FA5C 0036C99C  90 01 00 04 */	stw r0, 4(r1)
/* 8036FA60 0036C9A0  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 8036FA64 0036C9A4  93 E1 00 14 */	stw r31, 0x14(r1)
/* 8036FA68 0036C9A8  3B E4 00 00 */	addi r31, r4, 0
/* 8036FA6C 0036C9AC  93 C1 00 10 */	stw r30, 0x10(r1)
/* 8036FA70 0036C9B0  7C 7E 1B 79 */	or. r30, r3, r3
/* 8036FA74 0036C9B4  41 82 00 4C */	beq lbl_8036FAC0
/* 8036FA78 0036C9B8  3C 60 80 3E */	lis r3, __vt__12TButterfloid@ha
/* 8036FA7C 0036C9BC  38 63 5E 1C */	addi r3, r3, __vt__12TButterfloid@l
/* 8036FA80 0036C9C0  90 7E 00 00 */	stw r3, 0(r30)
/* 8036FA84 0036C9C4  38 03 00 24 */	addi r0, r3, 0x24
/* 8036FA88 0036C9C8  90 1E 00 20 */	stw r0, 0x20(r30)
/* 8036FA8C 0036C9CC  41 82 00 24 */	beq lbl_8036FAB0
/* 8036FA90 0036C9D0  3C 60 80 3E */	lis r3, __vt__8TRealoid@ha
/* 8036FA94 0036C9D4  38 63 55 7C */	addi r3, r3, __vt__8TRealoid@l
/* 8036FA98 0036C9D8  90 7E 00 00 */	stw r3, 0(r30)
/* 8036FA9C 0036C9DC  38 03 00 24 */	addi r0, r3, 0x24
/* 8036FAA0 0036C9E0  38 7E 00 00 */	addi r3, r30, 0
/* 8036FAA4 0036C9E4  90 1E 00 20 */	stw r0, 0x20(r30)
/* 8036FAA8 0036C9E8  38 80 00 00 */	li r4, 0
/* 8036FAAC 0036C9EC  4B EE 07 0D */	bl __dt__11TSpineEnemyFv
lbl_8036FAB0:
/* 8036FAB0 0036C9F0  7F E0 07 35 */	extsh. r0, r31
/* 8036FAB4 0036C9F4  40 81 00 0C */	ble lbl_8036FAC0
/* 8036FAB8 0036C9F8  7F C3 F3 78 */	mr r3, r30
/* 8036FABC 0036C9FC  4B C9 CF F5 */	bl __dl__FPv
lbl_8036FAC0:
/* 8036FAC0 0036CA00  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 8036FAC4 0036CA04  7F C3 F3 78 */	mr r3, r30
/* 8036FAC8 0036CA08  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 8036FACC 0036CA0C  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 8036FAD0 0036CA10  7C 08 03 A6 */	mtlr r0
/* 8036FAD4 0036CA14  38 21 00 18 */	addi r1, r1, 0x18
/* 8036FAD8 0036CA18  4E 80 00 20 */	blr 

.global __dt__10TButterflyFv
__dt__10TButterflyFv:
/* 8036FADC 0036CA1C  7C 08 02 A6 */	mflr r0
/* 8036FAE0 0036CA20  90 01 00 04 */	stw r0, 4(r1)
/* 8036FAE4 0036CA24  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 8036FAE8 0036CA28  93 E1 00 14 */	stw r31, 0x14(r1)
/* 8036FAEC 0036CA2C  3B E4 00 00 */	addi r31, r4, 0
/* 8036FAF0 0036CA30  93 C1 00 10 */	stw r30, 0x10(r1)
/* 8036FAF4 0036CA34  7C 7E 1B 79 */	or. r30, r3, r3
/* 8036FAF8 0036CA38  41 82 00 64 */	beq lbl_8036FB5C
/* 8036FAFC 0036CA3C  3C 60 80 3E */	lis r3, __vt__10TButterfly@ha
/* 8036FB00 0036CA40  38 63 5F 34 */	addi r3, r3, __vt__10TButterfly@l
/* 8036FB04 0036CA44  90 7E 00 00 */	stw r3, 0(r30)
/* 8036FB08 0036CA48  38 03 00 24 */	addi r0, r3, 0x24
/* 8036FB0C 0036CA4C  90 1E 00 20 */	stw r0, 0x20(r30)
/* 8036FB10 0036CA50  41 82 00 3C */	beq lbl_8036FB4C
/* 8036FB14 0036CA54  3C 60 80 3E */	lis r3, __vt__13TRealoidActor@ha
/* 8036FB18 0036CA58  38 63 56 94 */	addi r3, r3, __vt__13TRealoidActor@l
/* 8036FB1C 0036CA5C  90 7E 00 00 */	stw r3, 0(r30)
/* 8036FB20 0036CA60  38 03 00 24 */	addi r0, r3, 0x24
/* 8036FB24 0036CA64  90 1E 00 20 */	stw r0, 0x20(r30)
/* 8036FB28 0036CA68  41 82 00 24 */	beq lbl_8036FB4C
/* 8036FB2C 0036CA6C  3C 60 80 3B */	lis r3, __vt__10TTakeActor@ha
/* 8036FB30 0036CA70  38 63 46 8C */	addi r3, r3, __vt__10TTakeActor@l
/* 8036FB34 0036CA74  90 7E 00 00 */	stw r3, 0(r30)
/* 8036FB38 0036CA78  38 03 00 24 */	addi r0, r3, 0x24
/* 8036FB3C 0036CA7C  38 7E 00 00 */	addi r3, r30, 0
/* 8036FB40 0036CA80  90 1E 00 20 */	stw r0, 0x20(r30)
/* 8036FB44 0036CA84  38 80 00 00 */	li r4, 0
/* 8036FB48 0036CA88  4B D9 54 71 */	bl __dt__9THitActorFv
lbl_8036FB4C:
/* 8036FB4C 0036CA8C  7F E0 07 35 */	extsh. r0, r31
/* 8036FB50 0036CA90  40 81 00 0C */	ble lbl_8036FB5C
/* 8036FB54 0036CA94  7F C3 F3 78 */	mr r3, r30
/* 8036FB58 0036CA98  4B C9 CF 59 */	bl __dl__FPv
lbl_8036FB5C:
/* 8036FB5C 0036CA9C  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 8036FB60 0036CAA0  7F C3 F3 78 */	mr r3, r30
/* 8036FB64 0036CAA4  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 8036FB68 0036CAA8  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 8036FB6C 0036CAAC  7C 08 03 A6 */	mtlr r0
/* 8036FB70 0036CAB0  38 21 00 18 */	addi r1, r1, 0x18
/* 8036FB74 0036CAB4  4E 80 00 20 */	blr 

.global $$232$$2__dt__10TButterflyFv
$$232$$2__dt__10TButterflyFv:
/* 8036FB78 0036CAB8  38 63 FF E0 */	addi r3, r3, -32
/* 8036FB7C 0036CABC  4B FF FF 60 */	b __dt__10TButterflyFv

.global $$232$$2__dt__12TButterfloidFv
$$232$$2__dt__12TButterfloidFv:
/* 8036FB80 0036CAC0  38 63 FF E0 */	addi r3, r3, -32
/* 8036FB84 0036CAC4  4B FF FE D4 */	b __dt__12TButterfloidFv

.section .rodata, "wa"  # 0x8036FFA0 - 0x803A8380
$$21490:
  .4byte 0
  .4byte 0
  .4byte 0
$$21526:
	.incbin "baserom.dol", 0x3A520C, 0x14
$$21585:
	.incbin "baserom.dol", 0x3A5220, 0x30
$$21586:
	.incbin "baserom.dol", 0x3A5250, 0x38
$$21587:
	.incbin "baserom.dol", 0x3A5288, 0x34
$$21588:
	.incbin "baserom.dol", 0x3A52BC, 0x24
$$22393:
	.incbin "baserom.dol", 0x3A52E0, 0x10
$$22394:
	.incbin "baserom.dol", 0x3A52F0, 0x10
$$22395:
	.incbin "baserom.dol", 0x3A5300, 0x10
cButterflyMdlNames__9$$2unnamed$$2:
  .4byte $$22393
  .4byte $$22394
  .4byte $$22395
entry$2459:
    .4byte $$22393
    .4byte 0x10210000
    .4byte 0
    .4byte $$22394
    .4byte 0x10210000
    .4byte 0
    .4byte $$22395
    .4byte 0x10210000
    .4byte 0
    .4byte 0
    .4byte 0
    .4byte 0
$$22493:
	.asciz "mushroom1upR"
.balign 4
$$22594:
	.asciz "butterfly_fly"
.balign 4
$$22711:
	.incbin "baserom.dol", 0x3A536C, 0xB

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global __vt__19TButterfloidManager
__vt__19TButterfloidManager:
  .4byte 0
  .4byte 0
  .4byte __dt__19TButterfloidManagerFv
  .4byte getType__Q26JDrama8TNameRefCFv
  .4byte load__13TEnemyManagerFR20JSUMemoryInputStream
  .4byte save__Q26JDrama8TNameRefFR21JSUMemoryOutputStream
  .4byte loadAfter__Q26JDrama8TNameRefFv
  .4byte searchF__11TObjManagerFUsPCc
  .4byte perform__13TEnemyManagerFUlPQ26JDrama9TGraphics
  .4byte createModelData__19TButterfloidManagerFv
  .4byte createAnmData__11TObjManagerFv
  .4byte createModelDataArray__11TObjManagerFPC19TModelDataLoadEntry
  .4byte clipActors__12TLiveManagerFPQ26JDrama9TGraphics
  .4byte setFlagOutOfCube__12TLiveManagerFv
  .4byte createSpcBinary__12TLiveManagerFv
  .4byte hasMapCollision__12TLiveManagerCFv
  .4byte createEnemyInstance__13TEnemyManagerFv
  .4byte clipEnemies__13TEnemyManagerFPQ26JDrama9TGraphics
  .4byte restoreDrawBuffer__13TEnemyManagerFUl
  .4byte createEnemies__13TEnemyManagerFi
  .4byte changeDrawBuffer__13TEnemyManagerFUl
.global __vt__12TButterfloid
__vt__12TButterfloid:
  .4byte 0
  .4byte 0
  .4byte __dt__12TButterfloidFv
  .4byte getType__Q26JDrama6TActorCFv
  .4byte load__12TButterfloidFR20JSUMemoryInputStream
  .4byte save__Q26JDrama8TNameRefFR21JSUMemoryOutputStream
  .4byte loadAfter__Q26JDrama8TNameRefFv
  .4byte searchF__Q26JDrama8TNameRefFUsPCc
  .4byte perform__8TRealoidFUlPQ26JDrama9TGraphics
  .4byte 0
  .4byte 0
  .4byte $$232$$2__dt__12TButterfloidFv
  .4byte JSGFGetType__Q26JStage6TActorCFv
  .4byte JSGGetName__Q26JStage7TObjectCFv
  .4byte JSGGetFlag__Q26JStage7TObjectCFv
  .4byte JSGSetFlag__Q26JStage7TObjectFUl
  .4byte JSGGetData__Q26JStage7TObjectCFUlPvUl
  .4byte JSGSetData__Q26JStage7TObjectFUlPCvUl
  .4byte JSGGetParent__Q26JStage7TObjectCFPPQ26JStage7TObjectPUl
  .4byte JSGSetParent__Q26JStage7TObjectFPQ26JStage7TObjectUl
  .4byte JSGSetRelation__Q26JStage7TObjectFbPQ26JStage7TObjectUl
  .4byte $$232$$2JSGGetTranslation__Q26JDrama6TActorCFP3Vec
  .4byte $$232$$2JSGSetTranslation__Q26JDrama6TActorFRC3Vec
  .4byte $$232$$2JSGGetScaling__Q26JDrama6TActorCFP3Vec
  .4byte $$232$$2JSGSetScaling__Q26JDrama6TActorFRC3Vec
  .4byte $$232$$2JSGGetRotation__Q26JDrama6TActorCFP3Vec
  .4byte $$232$$2JSGSetRotation__Q26JDrama6TActorFRC3Vec
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
  .4byte receiveMessage__11TSpineEnemyFP9THitActorUl
  .4byte getTakingMtx__10TLiveActorFv
  .4byte ensureTakeSituation__10TTakeActorFv
  .4byte moveRequest__10TTakeActorFRCQ29JGeometry8TVec3$$0f$$1
  .4byte getRadiusAtY__10TTakeActorCFf
  .4byte belongToGround__10TLiveActorCFv
  .4byte getRootJointMtx__10TLiveActorCFv
  .4byte init__12TButterfloidFP12TLiveManager
  .4byte calcRootMatrix__11TSpineEnemyFv
  .4byte setGroundCollision__10TLiveActorFv
  .4byte control__10TLiveActorFv
  .4byte bind__10TLiveActorFv
  .4byte moveObject__10TLiveActorFv
  .4byte requestShadow__10TLiveActorFv
  .4byte drawObject__10TLiveActorFPQ26JDrama9TGraphics
  .4byte performOnlyDraw__10TLiveActorFUlPQ26JDrama9TGraphics
  .4byte getShadowType__10TLiveActorFv
  .4byte kill__10TLiveActorFv
  .4byte getGravityY__10TLiveActorCFv
  .4byte hasMapCollision__10TLiveActorCFv
  .4byte getFocalPoint__10TLiveActorCFv
  .4byte updateAnmSound__10TLiveActorFv
  .4byte getBasNameTable__10TLiveActorCFv
  .4byte reset__11TSpineEnemyFv
  .4byte resetToPosition__11TSpineEnemyFRCQ29JGeometry8TVec3$$0f$$1
  .4byte resetSRTV__11TSpineEnemyFRCQ29JGeometry8TVec3$$0f$$1RCQ29JGeometry8TVec3$$0f$$1RCQ29JGeometry8TVec3$$0f$$1RCQ29JGeometry8TVec3$$0f$$1
  .4byte getSaveParam__11TSpineEnemyCFv
  .4byte getPhaseShift__11TSpineEnemyCFv
  .4byte isReachedToGoal__11TSpineEnemyCFv
  .4byte createRealoidActor__12TButterfloidFP6MActor
.global __vt__10TButterfly
__vt__10TButterfly:
  .4byte 0
  .4byte 0
  .4byte __dt__10TButterflyFv
  .4byte getType__Q26JDrama6TActorCFv
  .4byte load__Q26JDrama6TActorFR20JSUMemoryInputStream
  .4byte save__Q26JDrama8TNameRefFR21JSUMemoryOutputStream
  .4byte loadAfter__Q26JDrama8TNameRefFv
  .4byte searchF__Q26JDrama8TNameRefFUsPCc
  .4byte perform__13TRealoidActorFUlPQ26JDrama9TGraphics
  .4byte 0
  .4byte 0
  .4byte $$232$$2__dt__10TButterflyFv
  .4byte JSGFGetType__Q26JStage6TActorCFv
  .4byte JSGGetName__Q26JStage7TObjectCFv
  .4byte JSGGetFlag__Q26JStage7TObjectCFv
  .4byte JSGSetFlag__Q26JStage7TObjectFUl
  .4byte JSGGetData__Q26JStage7TObjectCFUlPvUl
  .4byte JSGSetData__Q26JStage7TObjectFUlPCvUl
  .4byte JSGGetParent__Q26JStage7TObjectCFPPQ26JStage7TObjectPUl
  .4byte JSGSetParent__Q26JStage7TObjectFPQ26JStage7TObjectUl
  .4byte JSGSetRelation__Q26JStage7TObjectFbPQ26JStage7TObjectUl
  .4byte $$232$$2JSGGetTranslation__Q26JDrama6TActorCFP3Vec
  .4byte $$232$$2JSGSetTranslation__Q26JDrama6TActorFRC3Vec
  .4byte $$232$$2JSGGetScaling__Q26JDrama6TActorCFP3Vec
  .4byte $$232$$2JSGSetScaling__Q26JDrama6TActorFRC3Vec
  .4byte $$232$$2JSGGetRotation__Q26JDrama6TActorCFP3Vec
  .4byte $$232$$2JSGSetRotation__Q26JDrama6TActorFRC3Vec
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
  .4byte receiveMessage__10TButterflyFP9THitActorUl
  .4byte getTakingMtx__13TRealoidActorFv
  .4byte ensureTakeSituation__10TTakeActorFv
  .4byte moveRequest__10TTakeActorFRCQ29JGeometry8TVec3$$0f$$1
  .4byte getRadiusAtY__10TTakeActorCFf
  .4byte init__10TButterflyFv

.section .sdata2, "wa"  # 0x8040B460 - 0x80414020
$$22489:
	.incbin "baserom.dol", 0x3EC868, 0x4
$$22490:
  .4byte 0
$$22491:
	.incbin "baserom.dol", 0x3EC870, 0x8
$$22492:
  .4byte 0
$$22586:
	.incbin "baserom.dol", 0x3EC87C, 0x4
$$22587:
	.incbin "baserom.dol", 0x3EC880, 0x4
$$22588:
	.incbin "baserom.dol", 0x3EC884, 0x4
$$22589:
	.incbin "baserom.dol", 0x3EC888, 0x4
$$22590:
	.incbin "baserom.dol", 0x3EC88C, 0x4
$$22591:
	.incbin "baserom.dol", 0x3EC890, 0x4
$$22592:
	.incbin "baserom.dol", 0x3EC894, 0x4
$$22593:
	.incbin "baserom.dol", 0x3EC898, 0x4
$$22616:
	.incbin "baserom.dol", 0x3EC89C, 0x4
$$22710:
	.incbin "baserom.dol", 0x3EC8A0, 0x4


.section .bss, "wa"  # 0x803E6000 - 0x80408AC0
$$22422:
	.skip 0xC
