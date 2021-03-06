.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global Run
Run:
/* 80091414 0008E354  7C 08 02 A6 */	mflr r0
/* 80091418 0008E358  90 01 00 04 */	stw r0, 4(r1)
/* 8009141C 0008E35C  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 80091420 0008E360  93 E1 00 14 */	stw r31, 0x14(r1)
/* 80091424 0008E364  7C 7F 1B 78 */	mr r31, r3
/* 80091428 0008E368  4B FF ED A5 */	bl OSDisableInterrupts
/* 8009142C 0008E36C  4B FF CC 81 */	bl ICFlashInvalidate
/* 80091430 0008E370  7C 00 04 AC */	sync 0
/* 80091434 0008E374  4C 00 01 2C */	isync 
/* 80091438 0008E378  7F E8 03 A6 */	mtlr r31
/* 8009143C 0008E37C  4E 80 00 20 */	blr 
/* 80091440 0008E380  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 80091444 0008E384  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 80091448 0008E388  38 21 00 18 */	addi r1, r1, 0x18
/* 8009144C 0008E38C  7C 08 03 A6 */	mtlr r0
/* 80091450 0008E390  4E 80 00 20 */	blr 

.global Callback
Callback:
/* 80091454 0008E394  38 00 00 01 */	li r0, 1
/* 80091458 0008E398  90 0D 93 40 */	stw r0, Prepared@sda21(r13)
/* 8009145C 0008E39C  4E 80 00 20 */	blr 

.global __OSReboot
__OSReboot:
/* 80091460 0008E3A0  7C 08 02 A6 */	mflr r0
/* 80091464 0008E3A4  90 01 00 04 */	stw r0, 4(r1)
/* 80091468 0008E3A8  94 21 FC C0 */	stwu r1, -0x340(r1)
/* 8009146C 0008E3AC  93 E1 03 3C */	stw r31, 0x33c(r1)
/* 80091470 0008E3B0  93 C1 03 38 */	stw r30, 0x338(r1)
/* 80091474 0008E3B4  3C 60 80 3F */	lis r3, Header@ha
/* 80091478 0008E3B8  3B C3 B1 20 */	addi r30, r3, Header@l
/* 8009147C 0008E3BC  4B FF ED 51 */	bl OSDisableInterrupts
/* 80091480 0008E3C0  80 AD 93 38 */	lwz r5, SaveStart@sda21(r13)
/* 80091484 0008E3C4  3C 80 81 30 */	lis r4, 0x812FDFF0@ha
/* 80091488 0008E3C8  80 0D 93 3C */	lwz r0, SaveEnd@sda21(r13)
/* 8009148C 0008E3CC  38 60 00 00 */	li r3, 0
/* 80091490 0008E3D0  3F E0 81 80 */	lis r31, 0x817FFFFC@ha
/* 80091494 0008E3D4  38 E0 00 01 */	li r7, 1
/* 80091498 0008E3D8  90 7F FF FC */	stw r3, 0x817FFFFC@l(r31)
/* 8009149C 0008E3DC  3C C0 80 00 */	lis r6, 0x800030E2@ha
/* 800914A0 0008E3E0  90 7F FF F8 */	stw r3, -8(r31)
/* 800914A4 0008E3E4  38 61 00 70 */	addi r3, r1, 0x70
/* 800914A8 0008E3E8  98 E6 30 E2 */	stb r7, 0x800030E2@l(r6)
/* 800914AC 0008E3EC  90 A4 DF F0 */	stw r5, 0x812FDFF0@l(r4)
/* 800914B0 0008E3F0  90 04 DF EC */	stw r0, -0x2014(r4)
/* 800914B4 0008E3F4  4B FF D5 31 */	bl OSClearContext
/* 800914B8 0008E3F8  38 61 00 70 */	addi r3, r1, 0x70
/* 800914BC 0008E3FC  4B FF D3 61 */	bl OSSetCurrentContext
/* 800914C0 0008E400  48 00 54 2D */	bl DVDInit
/* 800914C4 0008E404  38 60 00 01 */	li r3, 1
/* 800914C8 0008E408  48 00 76 49 */	bl DVDSetAutoInvalidation
/* 800914CC 0008E40C  3C 60 80 09 */	lis r3, Callback@ha
/* 800914D0 0008E410  38 63 14 54 */	addi r3, r3, Callback@l
/* 800914D4 0008E414  48 00 7A 79 */	bl __DVDPrepareResetAsync
/* 800914D8 0008E418  48 00 79 91 */	bl DVDCheckDisk
/* 800914DC 0008E41C  2C 03 00 00 */	cmpwi r3, 0
/* 800914E0 0008E420  40 82 00 0C */	bne lbl_800914EC
/* 800914E4 0008E424  80 7F FF FC */	lwz r3, -4(r31)
/* 800914E8 0008E428  48 00 02 2D */	bl __OSDoHotReset
lbl_800914EC:
/* 800914EC 0008E42C  38 60 FF E0 */	li r3, -32
/* 800914F0 0008E430  4B FF F0 A5 */	bl __OSMaskInterrupts
/* 800914F4 0008E434  38 60 04 00 */	li r3, 0x400
/* 800914F8 0008E438  4B FF F1 25 */	bl __OSUnmaskInterrupts
/* 800914FC 0008E43C  4B FF EC E5 */	bl OSEnableInterrupts
/* 80091500 0008E440  48 00 00 04 */	b lbl_80091504
lbl_80091504:
/* 80091504 0008E444  48 00 00 04 */	b lbl_80091508
lbl_80091508:
/* 80091508 0008E448  80 0D 93 40 */	lwz r0, Prepared@sda21(r13)
/* 8009150C 0008E44C  2C 00 00 00 */	cmpwi r0, 0
/* 80091510 0008E450  41 82 FF F8 */	beq lbl_80091508
/* 80091514 0008E454  7F C4 F3 78 */	mr r4, r30
/* 80091518 0008E458  38 61 00 40 */	addi r3, r1, 0x40
/* 8009151C 0008E45C  38 A0 00 20 */	li r5, 0x20
/* 80091520 0008E460  38 C0 24 40 */	li r6, 0x2440
/* 80091524 0008E464  38 E0 00 00 */	li r7, 0
/* 80091528 0008E468  48 00 6F 41 */	bl DVDReadAbsAsyncForBS
/* 8009152C 0008E46C  3F E0 81 80 */	lis r31, 0x8180
/* 80091530 0008E470  48 00 00 04 */	b lbl_80091534
lbl_80091534:
/* 80091534 0008E474  48 00 00 04 */	b lbl_80091538
lbl_80091538:
/* 80091538 0008E478  80 01 00 4C */	lwz r0, 0x4c(r1)
/* 8009153C 0008E47C  2C 00 00 01 */	cmpwi r0, 1
/* 80091540 0008E480  41 82 FF F8 */	beq lbl_80091538
/* 80091544 0008E484  40 80 00 14 */	bge lbl_80091558
/* 80091548 0008E488  2C 00 FF FF */	cmpwi r0, -1
/* 8009154C 0008E48C  41 82 00 18 */	beq lbl_80091564
/* 80091550 0008E490  40 80 00 20 */	bge lbl_80091570
/* 80091554 0008E494  4B FF FF E4 */	b lbl_80091538
lbl_80091558:
/* 80091558 0008E498  2C 00 00 0C */	cmpwi r0, 0xc
/* 8009155C 0008E49C  40 80 FF DC */	bge lbl_80091538
/* 80091560 0008E4A0  48 00 00 04 */	b lbl_80091564
lbl_80091564:
/* 80091564 0008E4A4  80 7F FF FC */	lwz r3, -4(r31)
/* 80091568 0008E4A8  48 00 01 AD */	bl __OSDoHotReset
/* 8009156C 0008E4AC  4B FF FF CC */	b lbl_80091538
lbl_80091570:
/* 80091570 0008E4B0  80 7E 00 18 */	lwz r3, 0x18(r30)
/* 80091574 0008E4B4  80 9E 00 14 */	lwz r4, 0x14(r30)
/* 80091578 0008E4B8  38 03 00 1F */	addi r0, r3, 0x1f
/* 8009157C 0008E4BC  38 84 00 20 */	addi r4, r4, 0x20
/* 80091580 0008E4C0  54 1E 00 34 */	rlwinm r30, r0, 0, 0, 0x1a
/* 80091584 0008E4C4  48 00 00 04 */	b lbl_80091588
lbl_80091588:
/* 80091588 0008E4C8  48 00 00 04 */	b lbl_8009158C
lbl_8009158C:
/* 8009158C 0008E4CC  80 0D 93 40 */	lwz r0, Prepared@sda21(r13)
/* 80091590 0008E4D0  2C 00 00 00 */	cmpwi r0, 0
/* 80091594 0008E4D4  41 82 FF F8 */	beq lbl_8009158C
/* 80091598 0008E4D8  7F C5 F3 78 */	mr r5, r30
/* 8009159C 0008E4DC  38 61 00 10 */	addi r3, r1, 0x10
/* 800915A0 0008E4E0  38 C4 24 40 */	addi r6, r4, 0x2440
/* 800915A4 0008E4E4  3C 80 81 30 */	lis r4, 0x8130
/* 800915A8 0008E4E8  38 E0 00 00 */	li r7, 0
/* 800915AC 0008E4EC  48 00 6E BD */	bl DVDReadAbsAsyncForBS
/* 800915B0 0008E4F0  3F E0 81 80 */	lis r31, 0x8180
/* 800915B4 0008E4F4  48 00 00 04 */	b lbl_800915B8
lbl_800915B8:
/* 800915B8 0008E4F8  48 00 00 04 */	b lbl_800915BC
lbl_800915BC:
/* 800915BC 0008E4FC  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 800915C0 0008E500  2C 00 00 01 */	cmpwi r0, 1
/* 800915C4 0008E504  41 82 FF F8 */	beq lbl_800915BC
/* 800915C8 0008E508  40 80 00 14 */	bge lbl_800915DC
/* 800915CC 0008E50C  2C 00 FF FF */	cmpwi r0, -1
/* 800915D0 0008E510  41 82 00 18 */	beq lbl_800915E8
/* 800915D4 0008E514  40 80 00 20 */	bge lbl_800915F4
/* 800915D8 0008E518  4B FF FF E4 */	b lbl_800915BC
lbl_800915DC:
/* 800915DC 0008E51C  2C 00 00 0C */	cmpwi r0, 0xc
/* 800915E0 0008E520  40 80 FF DC */	bge lbl_800915BC
/* 800915E4 0008E524  48 00 00 04 */	b lbl_800915E8
lbl_800915E8:
/* 800915E8 0008E528  80 7F FF FC */	lwz r3, -4(r31)
/* 800915EC 0008E52C  48 00 01 29 */	bl __OSDoHotReset
/* 800915F0 0008E530  4B FF FF CC */	b lbl_800915BC
lbl_800915F4:
/* 800915F4 0008E534  3C 60 81 30 */	lis r3, 0x8130
/* 800915F8 0008E538  7F C4 F3 78 */	mr r4, r30
/* 800915FC 0008E53C  4B FF CA 79 */	bl ICInvalidateRange
/* 80091600 0008E540  3C 60 81 30 */	lis r3, 0x8130
/* 80091604 0008E544  4B FF FE 11 */	bl Run
/* 80091608 0008E548  80 01 03 44 */	lwz r0, 0x344(r1)
/* 8009160C 0008E54C  83 E1 03 3C */	lwz r31, 0x33c(r1)
/* 80091610 0008E550  83 C1 03 38 */	lwz r30, 0x338(r1)
/* 80091614 0008E554  38 21 03 40 */	addi r1, r1, 0x340
/* 80091618 0008E558  7C 08 03 A6 */	mtlr r0
/* 8009161C 0008E55C  4E 80 00 20 */	blr 

.section .bss, "wa"  # 0x803E6000 - 0x80408AC0
Header:
	.skip 0x20

.section .sbss, "wa"  # 0x804097C0 - 0x8040B45C
SaveStart:
	.skip 0x4
SaveEnd:
	.skip 0x4
Prepared:
	.skip 0x8
