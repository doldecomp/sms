.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global startAnimSound__12MAnmSoundNPCFPvUlPP8JAISoundP8JAIActorUc
startAnimSound__12MAnmSoundNPCFPvUlPP8JAISoundP8JAIActorUc:
/* 8017C23C 0017917C  7C 08 02 A6 */	mflr r0
/* 8017C240 00179180  90 01 00 04 */	stw r0, 4(r1)
/* 8017C244 00179184  94 21 FF 70 */	stwu r1, -0x90(r1)
/* 8017C248 00179188  DB E1 00 88 */	stfd f31, 0x88(r1)
/* 8017C24C 0017918C  DB C1 00 80 */	stfd f30, 0x80(r1)
/* 8017C250 00179190  DB A1 00 78 */	stfd f29, 0x78(r1)
/* 8017C254 00179194  BF 41 00 60 */	stmw r26, 0x60(r1)
/* 8017C258 00179198  7C 7A 1B 78 */	mr r26, r3
/* 8017C25C 0017919C  3B 65 00 00 */	addi r27, r5, 0
/* 8017C260 001791A0  3B 86 00 00 */	addi r28, r6, 0
/* 8017C264 001791A4  3B A7 00 00 */	addi r29, r7, 0
/* 8017C268 001791A8  3B C8 00 00 */	addi r30, r8, 0
/* 8017C26C 001791AC  38 9B 00 00 */	addi r4, r27, 0
/* 8017C270 001791B0  80 6D 99 F4 */	lwz r3, MSGMSound@sda21(r13)
/* 8017C274 001791B4  48 00 07 DD */	bl gateCheck__6MSoundFUl
/* 8017C278 001791B8  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 8017C27C 001791BC  41 82 01 F8 */	beq lbl_8017C474
/* 8017C280 001791C0  80 1A 00 80 */	lwz r0, 0x80(r26)
/* 8017C284 001791C4  83 FA 00 90 */	lwz r31, 0x90(r26)
/* 8017C288 001791C8  54 05 28 34 */	slwi r5, r0, 5
/* 8017C28C 001791CC  7C 7F 2A 14 */	add r3, r31, r5
/* 8017C290 001791D0  80 63 00 18 */	lwz r3, 0x18(r3)
/* 8017C294 001791D4  54 64 00 1F */	rlwinm. r4, r3, 0, 0, 0xf
/* 8017C298 001791D8  41 82 00 C8 */	beq lbl_8017C360
/* 8017C29C 001791DC  54 80 00 0F */	rlwinm. r0, r4, 0, 0, 7
/* 8017C2A0 001791E0  41 82 00 3C */	beq lbl_8017C2DC
/* 8017C2A4 001791E4  80 DA 00 84 */	lwz r6, 0x84(r26)
/* 8017C2A8 001791E8  54 63 46 3E */	srwi r3, r3, 0x18
/* 8017C2AC 001791EC  38 E3 00 01 */	addi r7, r3, 1
/* 8017C2B0 001791F0  28 06 00 00 */	cmplwi r6, 0
/* 8017C2B4 001791F4  41 82 00 28 */	beq lbl_8017C2DC
/* 8017C2B8 001791F8  88 7A 00 98 */	lbz r3, 0x98(r26)
/* 8017C2BC 001791FC  7C 03 3B 96 */	divwu r0, r3, r7
/* 8017C2C0 00179200  7C 00 39 D6 */	mullw r0, r0, r7
/* 8017C2C4 00179204  7C 00 18 50 */	subf r0, r0, r3
/* 8017C2C8 00179208  7C 66 02 14 */	add r3, r6, r0
/* 8017C2CC 0017920C  7C 03 3B 96 */	divwu r0, r3, r7
/* 8017C2D0 00179210  7C 00 39 D6 */	mullw r0, r0, r7
/* 8017C2D4 00179214  7C 00 18 51 */	subf. r0, r0, r3
/* 8017C2D8 00179218  40 82 01 9C */	bne lbl_8017C474
lbl_8017C2DC:
/* 8017C2DC 0017921C  54 80 02 1F */	rlwinm. r0, r4, 0, 8, 0xf
/* 8017C2E0 00179220  41 82 00 80 */	beq lbl_8017C360
/* 8017C2E4 00179224  3C 60 00 19 */	lis r3, 0x0019660D@ha
/* 8017C2E8 00179228  80 8D 8F B0 */	lwz r4, random__8JAIConst@sda21(r13)
/* 8017C2EC 0017922C  38 03 66 0D */	addi r0, r3, 0x0019660D@l
/* 8017C2F0 00179230  C8 62 AA 10 */	lfd f3, $$21987@sda21(r2)
/* 8017C2F4 00179234  7C 64 01 D6 */	mullw r3, r4, r0
/* 8017C2F8 00179238  C0 02 A9 F8 */	lfs f0, $$21981@sda21(r2)
/* 8017C2FC 0017923C  7C 9F 2A 14 */	add r4, r31, r5
/* 8017C300 00179240  3C 63 3C 6F */	addis r3, r3, 0x3c6f
/* 8017C304 00179244  80 84 00 18 */	lwz r4, 0x18(r4)
/* 8017C308 00179248  38 03 F3 5F */	addi r0, r3, -3233
/* 8017C30C 0017924C  90 0D 8F B0 */	stw r0, random__8JAIConst@sda21(r13)
/* 8017C310 00179250  54 83 86 3E */	rlwinm r3, r4, 0x10, 0x18, 0x1f
/* 8017C314 00179254  38 03 00 01 */	addi r0, r3, 1
/* 8017C318 00179258  80 6D 8F B0 */	lwz r3, random__8JAIConst@sda21(r13)
/* 8017C31C 0017925C  54 00 06 3E */	clrlwi r0, r0, 0x18
/* 8017C320 00179260  90 01 00 5C */	stw r0, 0x5c(r1)
/* 8017C324 00179264  3C 00 43 30 */	lis r0, 0x4330
/* 8017C328 00179268  54 63 BA 7E */	srwi r3, r3, 9
/* 8017C32C 0017926C  64 63 3F 80 */	oris r3, r3, 0x3f80
/* 8017C330 00179270  90 01 00 58 */	stw r0, 0x58(r1)
/* 8017C334 00179274  90 61 00 44 */	stw r3, 0x44(r1)
/* 8017C338 00179278  C8 41 00 58 */	lfd f2, 0x58(r1)
/* 8017C33C 0017927C  C0 21 00 44 */	lfs f1, 0x44(r1)
/* 8017C340 00179280  EC 01 00 28 */	fsubs f0, f1, f0
/* 8017C344 00179284  EC 22 18 28 */	fsubs f1, f2, f3
/* 8017C348 00179288  EC 01 00 32 */	fmuls f0, f1, f0
/* 8017C34C 0017928C  FC 00 00 1E */	fctiwz f0, f0
/* 8017C350 00179290  D8 01 00 50 */	stfd f0, 0x50(r1)
/* 8017C354 00179294  80 01 00 54 */	lwz r0, 0x54(r1)
/* 8017C358 00179298  54 00 06 3F */	clrlwi. r0, r0, 0x18
/* 8017C35C 0017929C  40 82 01 18 */	bne lbl_8017C474
lbl_8017C360:
/* 8017C360 001792A0  38 7B 00 00 */	addi r3, r27, 0
/* 8017C364 001792A4  38 9D 00 00 */	addi r4, r29, 0
/* 8017C368 001792A8  48 00 4F 99 */	bl checkMonoSound__Q214MSoundSESystem8MSoundSEFUlP8JAIActor
/* 8017C36C 001792AC  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 8017C370 001792B0  41 82 01 04 */	beq lbl_8017C474
/* 8017C374 001792B4  38 7B 00 00 */	addi r3, r27, 0
/* 8017C378 001792B8  38 9C 00 00 */	addi r4, r28, 0
/* 8017C37C 001792BC  38 BD 00 00 */	addi r5, r29, 0
/* 8017C380 001792C0  38 FE 00 00 */	addi r7, r30, 0
/* 8017C384 001792C4  38 C0 00 00 */	li r6, 0
/* 8017C388 001792C8  48 00 52 75 */	bl startSoundActorInner__Q214MSoundSESystem8MSoundSEFUlPP8JAISoundP8JAIActorUlUc
/* 8017C38C 001792CC  80 1C 00 00 */	lwz r0, 0(r28)
/* 8017C390 001792D0  28 00 00 00 */	cmplwi r0, 0
/* 8017C394 001792D4  41 82 00 E0 */	beq lbl_8017C474
/* 8017C398 001792D8  80 1A 00 80 */	lwz r0, 0x80(r26)
/* 8017C39C 001792DC  54 00 28 34 */	slwi r0, r0, 5
/* 8017C3A0 001792E0  7C 7F 02 14 */	add r3, r31, r0
/* 8017C3A4 001792E4  80 03 00 18 */	lwz r0, 0x18(r3)
/* 8017C3A8 001792E8  54 00 04 21 */	rlwinm. r0, r0, 0, 0x10, 0x10
/* 8017C3AC 001792EC  40 82 00 C8 */	bne lbl_8017C474
/* 8017C3B0 001792F0  C3 E2 A9 F8 */	lfs f31, $$21981@sda21(r2)
/* 8017C3B4 001792F4  83 7D 00 04 */	lwz r27, 4(r29)
/* 8017C3B8 001792F8  80 6D 99 F4 */	lwz r3, MSGMSound@sda21(r13)
/* 8017C3BC 001792FC  48 00 06 E5 */	bl cameraLooksAtMario__6MSoundFv
/* 8017C3C0 00179300  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 8017C3C4 00179304  41 82 00 60 */	beq lbl_8017C424
/* 8017C3C8 00179308  80 6D 99 F4 */	lwz r3, MSGMSound@sda21(r13)
/* 8017C3CC 0017930C  C0 3B 00 04 */	lfs f1, 4(r27)
/* 8017C3D0 00179310  83 A3 00 AC */	lwz r29, 0xac(r3)
/* 8017C3D4 00179314  C0 42 A9 FC */	lfs f2, $$21982@sda21(r2)
/* 8017C3D8 00179318  C0 1D 00 04 */	lfs f0, 4(r29)
/* 8017C3DC 0017931C  EC 21 00 28 */	fsubs f1, f1, f0
/* 8017C3E0 00179320  4B F0 AE F9 */	bl powf
/* 8017C3E4 00179324  C0 7B 00 00 */	lfs f3, 0(r27)
/* 8017C3E8 00179328  FF A0 08 90 */	fmr f29, f1
/* 8017C3EC 0017932C  C0 1D 00 00 */	lfs f0, 0(r29)
/* 8017C3F0 00179330  C0 42 A9 FC */	lfs f2, $$21982@sda21(r2)
/* 8017C3F4 00179334  EC 23 00 28 */	fsubs f1, f3, f0
/* 8017C3F8 00179338  4B F0 AE E1 */	bl powf
/* 8017C3FC 0017933C  C0 7B 00 08 */	lfs f3, 8(r27)
/* 8017C400 00179340  FF C0 08 90 */	fmr f30, f1
/* 8017C404 00179344  C0 1D 00 08 */	lfs f0, 8(r29)
/* 8017C408 00179348  C0 42 A9 FC */	lfs f2, $$21982@sda21(r2)
/* 8017C40C 0017934C  EC 23 00 28 */	fsubs f1, f3, f0
/* 8017C410 00179350  4B F0 AE C9 */	bl powf
/* 8017C414 00179354  EC 1E E8 2A */	fadds f0, f30, f29
/* 8017C418 00179358  EC 21 00 2A */	fadds f1, f1, f0
/* 8017C41C 0017935C  4B F9 33 89 */	bl sqrtf__3stdFf
/* 8017C420 00179360  48 00 00 08 */	b lbl_8017C428
lbl_8017C424:
/* 8017C424 00179364  C0 22 AA 00 */	lfs f1, $$21983@sda21(r2)
lbl_8017C428:
/* 8017C428 00179368  C0 02 AA 00 */	lfs f0, $$21983@sda21(r2)
/* 8017C42C 0017936C  FC 00 08 00 */	fcmpu cr0, f0, f1
/* 8017C430 00179370  41 82 00 2C */	beq lbl_8017C45C
/* 8017C434 00179374  80 1A 00 80 */	lwz r0, 0x80(r26)
/* 8017C438 00179378  38 80 00 08 */	li r4, 8
/* 8017C43C 0017937C  C0 42 AA 04 */	lfs f2, $$21984@sda21(r2)
/* 8017C440 00179380  54 00 28 34 */	slwi r0, r0, 5
/* 8017C444 00179384  C0 62 AA 08 */	lfs f3, $$21985@sda21(r2)
/* 8017C448 00179388  7C 7F 02 14 */	add r3, r31, r0
/* 8017C44C 0017938C  80 03 00 18 */	lwz r0, 0x18(r3)
/* 8017C450 00179390  54 03 A7 7E */	rlwinm r3, r0, 0x14, 0x1d, 0x1f
/* 8017C454 00179394  48 00 99 61 */	bl calcVolume__8MSHandleFfffUcUc
/* 8017C458 00179398  FF E0 08 90 */	fmr f31, f1
lbl_8017C45C:
/* 8017C45C 0017939C  FC 20 F8 90 */	fmr f1, f31
/* 8017C460 001793A0  80 7C 00 00 */	lwz r3, 0(r28)
/* 8017C464 001793A4  38 80 00 00 */	li r4, 0
/* 8017C468 001793A8  38 A0 00 00 */	li r5, 0
/* 8017C46C 001793AC  38 C0 00 00 */	li r6, 0
/* 8017C470 001793B0  4B ED 9B 59 */	bl setSeInterVolume__8JAISoundFUcfUlUc
lbl_8017C474:
/* 8017C474 001793B4  BB 41 00 60 */	lmw r26, 0x60(r1)
/* 8017C478 001793B8  80 01 00 94 */	lwz r0, 0x94(r1)
/* 8017C47C 001793BC  CB E1 00 88 */	lfd f31, 0x88(r1)
/* 8017C480 001793C0  CB C1 00 80 */	lfd f30, 0x80(r1)
/* 8017C484 001793C4  7C 08 03 A6 */	mtlr r0
/* 8017C488 001793C8  CB A1 00 78 */	lfd f29, 0x78(r1)
/* 8017C48C 001793CC  38 21 00 90 */	addi r1, r1, 0x90
/* 8017C490 001793D0  4E 80 00 20 */	blr 

.global setSpeedModifySound__9MAnmSoundFP8JAISoundP22JAIAnimeFrameSoundDataf
setSpeedModifySound__9MAnmSoundFP8JAISoundP22JAIAnimeFrameSoundDataf:
/* 8017C494 001793D4  7C 08 02 A6 */	mflr r0
/* 8017C498 001793D8  90 01 00 04 */	stw r0, 4(r1)
/* 8017C49C 001793DC  94 21 FF D0 */	stwu r1, -0x30(r1)
/* 8017C4A0 001793E0  DB E1 00 28 */	stfd f31, 0x28(r1)
/* 8017C4A4 001793E4  FF E0 08 90 */	fmr f31, f1
/* 8017C4A8 001793E8  93 E1 00 24 */	stw r31, 0x24(r1)
/* 8017C4AC 001793EC  3B E5 00 00 */	addi r31, r5, 0
/* 8017C4B0 001793F0  38 A0 00 14 */	li r5, 0x14
/* 8017C4B4 001793F4  93 C1 00 20 */	stw r30, 0x20(r1)
/* 8017C4B8 001793F8  7C 9E 23 78 */	mr r30, r4
/* 8017C4BC 001793FC  93 A1 00 1C */	stw r29, 0x1c(r1)
/* 8017C4C0 00179400  3B A3 00 00 */	addi r29, r3, 0
/* 8017C4C4 00179404  80 04 00 08 */	lwz r0, 8(r4)
/* 8017C4C8 00179408  3C 80 00 10 */	lis r4, 0x10
/* 8017C4CC 0017940C  7C 03 03 78 */	mr r3, r0
/* 8017C4D0 00179410  48 00 04 E5 */	bl getSwitch__6MSoundFUlUlUl
/* 8017C4D4 00179414  28 03 00 00 */	cmplwi r3, 0
/* 8017C4D8 00179418  41 82 00 18 */	beq lbl_8017C4F0
/* 8017C4DC 0017941C  FC 20 F8 90 */	fmr f1, f31
/* 8017C4E0 00179420  38 7D 00 00 */	addi r3, r29, 0
/* 8017C4E4 00179424  38 9E 00 00 */	addi r4, r30, 0
/* 8017C4E8 00179428  38 BF 00 00 */	addi r5, r31, 0
/* 8017C4EC 0017942C  4B EC EC 15 */	bl setSpeedModifySound__13JAIAnimeSoundFP8JAISoundP22JAIAnimeFrameSoundDataf
lbl_8017C4F0:
/* 8017C4F0 00179430  80 01 00 34 */	lwz r0, 0x34(r1)
/* 8017C4F4 00179434  CB E1 00 28 */	lfd f31, 0x28(r1)
/* 8017C4F8 00179438  83 E1 00 24 */	lwz r31, 0x24(r1)
/* 8017C4FC 0017943C  7C 08 03 A6 */	mtlr r0
/* 8017C500 00179440  83 C1 00 20 */	lwz r30, 0x20(r1)
/* 8017C504 00179444  83 A1 00 1C */	lwz r29, 0x1c(r1)
/* 8017C508 00179448  38 21 00 30 */	addi r1, r1, 0x30
/* 8017C50C 0017944C  4E 80 00 20 */	blr 

.global startAnimSound__9MAnmSoundFPvUlPP8JAISoundP8JAIActorUc
startAnimSound__9MAnmSoundFPvUlPP8JAISoundP8JAIActorUc:
/* 8017C510 00179450  7C 08 02 A6 */	mflr r0
/* 8017C514 00179454  90 01 00 04 */	stw r0, 4(r1)
/* 8017C518 00179458  94 21 FF C8 */	stwu r1, -0x38(r1)
/* 8017C51C 0017945C  93 E1 00 34 */	stw r31, 0x34(r1)
/* 8017C520 00179460  3B E8 00 00 */	addi r31, r8, 0
/* 8017C524 00179464  93 C1 00 30 */	stw r30, 0x30(r1)
/* 8017C528 00179468  3B C7 00 00 */	addi r30, r7, 0
/* 8017C52C 0017946C  93 A1 00 2C */	stw r29, 0x2c(r1)
/* 8017C530 00179470  3B A6 00 00 */	addi r29, r6, 0
/* 8017C534 00179474  93 81 00 28 */	stw r28, 0x28(r1)
/* 8017C538 00179478  7C BC 2B 78 */	mr r28, r5
/* 8017C53C 0017947C  38 9C 00 00 */	addi r4, r28, 0
/* 8017C540 00179480  80 6D 99 F4 */	lwz r3, MSGMSound@sda21(r13)
/* 8017C544 00179484  48 00 05 0D */	bl gateCheck__6MSoundFUl
/* 8017C548 00179488  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 8017C54C 0017948C  41 82 00 9C */	beq lbl_8017C5E8
/* 8017C550 00179490  57 83 17 BF */	rlwinm. r3, r28, 2, 0x1e, 0x1f
/* 8017C554 00179494  57 80 A7 3E */	rlwinm r0, r28, 0x14, 0x1c, 0x1f
/* 8017C558 00179498  40 82 00 08 */	bne lbl_8017C560
/* 8017C55C 0017949C  48 00 00 28 */	b lbl_8017C584
lbl_8017C560:
/* 8017C560 001794A0  28 03 00 02 */	cmplwi r3, 2
/* 8017C564 001794A4  40 82 00 0C */	bne lbl_8017C570
/* 8017C568 001794A8  38 00 00 10 */	li r0, 0x10
/* 8017C56C 001794AC  48 00 00 18 */	b lbl_8017C584
lbl_8017C570:
/* 8017C570 001794B0  28 03 00 03 */	cmplwi r3, 3
/* 8017C574 001794B4  40 82 00 0C */	bne lbl_8017C580
/* 8017C578 001794B8  38 00 00 11 */	li r0, 0x11
/* 8017C57C 001794BC  48 00 00 08 */	b lbl_8017C584
lbl_8017C580:
/* 8017C580 001794C0  38 00 FF FF */	li r0, -1
lbl_8017C584:
/* 8017C584 001794C4  2C 00 00 07 */	cmpwi r0, 7
/* 8017C588 001794C8  41 82 00 28 */	beq lbl_8017C5B0
/* 8017C58C 001794CC  40 80 00 44 */	bge lbl_8017C5D0
/* 8017C590 001794D0  2C 00 00 00 */	cmpwi r0, 0
/* 8017C594 001794D4  41 82 00 08 */	beq lbl_8017C59C
/* 8017C598 001794D8  48 00 00 38 */	b lbl_8017C5D0
lbl_8017C59C:
/* 8017C59C 001794DC  80 1E 00 0C */	lwz r0, 0xc(r30)
/* 8017C5A0 001794E0  54 00 04 E6 */	rlwinm r0, r0, 0, 0x13, 0x13
/* 8017C5A4 001794E4  28 00 10 00 */	cmplwi r0, 0x1000
/* 8017C5A8 001794E8  40 82 00 28 */	bne lbl_8017C5D0
/* 8017C5AC 001794EC  48 00 00 3C */	b lbl_8017C5E8
lbl_8017C5B0:
/* 8017C5B0 001794F0  80 BE 00 0C */	lwz r5, 0xc(r30)
/* 8017C5B4 001794F4  7F 84 E3 78 */	mr r4, r28
/* 8017C5B8 001794F8  80 6D 99 F4 */	lwz r3, MSGMSound@sda21(r13)
/* 8017C5BC 001794FC  54 A0 47 3E */	rlwinm r0, r5, 8, 0x1c, 0x1f
/* 8017C5C0 00179500  54 A6 27 3E */	srwi r6, r5, 0x1c
/* 8017C5C4 00179504  7C 05 07 34 */	extsh r5, r0
/* 8017C5C8 00179508  48 00 09 7D */	bl startMarioVoice__6MSoundFUlsUc
/* 8017C5CC 0017950C  48 00 00 1C */	b lbl_8017C5E8
lbl_8017C5D0:
/* 8017C5D0 00179510  38 7C 00 00 */	addi r3, r28, 0
/* 8017C5D4 00179514  38 9D 00 00 */	addi r4, r29, 0
/* 8017C5D8 00179518  38 BE 00 00 */	addi r5, r30, 0
/* 8017C5DC 0017951C  38 FF 00 00 */	addi r7, r31, 0
/* 8017C5E0 00179520  38 C0 00 00 */	li r6, 0
/* 8017C5E4 00179524  48 00 50 19 */	bl startSoundActorInner__Q214MSoundSESystem8MSoundSEFUlPP8JAISoundP8JAIActorUlUc
lbl_8017C5E8:
/* 8017C5E8 00179528  80 01 00 3C */	lwz r0, 0x3c(r1)
/* 8017C5EC 0017952C  83 E1 00 34 */	lwz r31, 0x34(r1)
/* 8017C5F0 00179530  83 C1 00 30 */	lwz r30, 0x30(r1)
/* 8017C5F4 00179534  7C 08 03 A6 */	mtlr r0
/* 8017C5F8 00179538  83 A1 00 2C */	lwz r29, 0x2c(r1)
/* 8017C5FC 0017953C  83 81 00 28 */	lwz r28, 0x28(r1)
/* 8017C600 00179540  38 21 00 38 */	addi r1, r1, 0x38
/* 8017C604 00179544  4E 80 00 20 */	blr 

.global animeLoop__9MAnmSoundFP3VecffUlUc
animeLoop__9MAnmSoundFP3VecffUlUc:
/* 8017C608 00179548  7C 08 02 A6 */	mflr r0
/* 8017C60C 0017954C  39 04 00 00 */	addi r8, r4, 0
/* 8017C610 00179550  90 01 00 04 */	stw r0, 4(r1)
/* 8017C614 00179554  38 E6 00 00 */	addi r7, r6, 0
/* 8017C618 00179558  94 21 FF F8 */	stwu r1, -8(r1)
/* 8017C61C 0017955C  80 03 00 90 */	lwz r0, 0x90(r3)
/* 8017C620 00179560  28 00 00 00 */	cmplwi r0, 0
/* 8017C624 00179564  38 05 00 00 */	addi r0, r5, 0
/* 8017C628 00179568  41 82 00 14 */	beq lbl_8017C63C
/* 8017C62C 0017956C  80 8D 8F A0 */	lwz r4, basic__8JAIBasic@sda21(r13)
/* 8017C630 00179570  38 A8 00 00 */	addi r5, r8, 0
/* 8017C634 00179574  7C 06 03 78 */	mr r6, r0
/* 8017C638 00179578  4B EC E3 F5 */	bl setAnimSoundVec__13JAIAnimeSoundFP8JAIBasicP3VecffUlUc
lbl_8017C63C:
/* 8017C63C 0017957C  80 01 00 0C */	lwz r0, 0xc(r1)
/* 8017C640 00179580  38 21 00 08 */	addi r1, r1, 8
/* 8017C644 00179584  7C 08 03 A6 */	mtlr r0
/* 8017C648 00179588  4E 80 00 20 */	blr 

.global initAnmSound__9MAnmSoundFPvUlf
initAnmSound__9MAnmSoundFPvUlf:
/* 8017C64C 0017958C  7C 08 02 A6 */	mflr r0
/* 8017C650 00179590  90 01 00 04 */	stw r0, 4(r1)
/* 8017C654 00179594  94 21 FF F8 */	stwu r1, -8(r1)
/* 8017C658 00179598  4B EC E2 81 */	bl initActorAnimSound__13JAIAnimeSoundFPvUlf
/* 8017C65C 0017959C  80 01 00 0C */	lwz r0, 0xc(r1)
/* 8017C660 001795A0  38 21 00 08 */	addi r1, r1, 8
/* 8017C664 001795A4  7C 08 03 A6 */	mtlr r0
/* 8017C668 001795A8  4E 80 00 20 */	blr 

.global __ct__9MAnmSoundFP6MSound
__ct__9MAnmSoundFP6MSound:
/* 8017C66C 001795AC  7C 08 02 A6 */	mflr r0
/* 8017C670 001795B0  90 01 00 04 */	stw r0, 4(r1)
/* 8017C674 001795B4  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 8017C678 001795B8  93 E1 00 14 */	stw r31, 0x14(r1)
/* 8017C67C 001795BC  7C 7F 1B 78 */	mr r31, r3
/* 8017C680 001795C0  4B EC E1 E5 */	bl __ct__13JAIAnimeSoundFv
/* 8017C684 001795C4  3C 60 80 3C */	lis r3, __vt__9MAnmSound@ha
/* 8017C688 001795C8  38 03 9C 38 */	addi r0, r3, __vt__9MAnmSound@l
/* 8017C68C 001795CC  90 1F 00 94 */	stw r0, 0x94(r31)
/* 8017C690 001795D0  38 00 00 00 */	li r0, 0
/* 8017C694 001795D4  38 7F 00 00 */	addi r3, r31, 0
/* 8017C698 001795D8  90 1F 00 90 */	stw r0, 0x90(r31)
/* 8017C69C 001795DC  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 8017C6A0 001795E0  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 8017C6A4 001795E4  38 21 00 18 */	addi r1, r1, 0x18
/* 8017C6A8 001795E8  7C 08 03 A6 */	mtlr r0
/* 8017C6AC 001795EC  4E 80 00 20 */	blr 

.global __sinit_MAnmSound_cpp
__sinit_MAnmSound_cpp:
/* 8017C6B0 001795F0  7C 08 02 A6 */	mflr r0
/* 8017C6B4 001795F4  3C 60 80 3F */	lis r3, $$22044@ha
/* 8017C6B8 001795F8  90 01 00 04 */	stw r0, 4(r1)
/* 8017C6BC 001795FC  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 8017C6C0 00179600  93 E1 00 0C */	stw r31, 0xc(r1)
/* 8017C6C4 00179604  3B E3 44 D0 */	addi r31, r3, $$22044@l
/* 8017C6C8 00179608  88 0D 97 CC */	lbz r0, __init__smList__15JALList$$05MSBgm$$1@sda21(r13)
/* 8017C6CC 0017960C  7C 00 07 75 */	extsb. r0, r0
/* 8017C6D0 00179610  40 82 00 28 */	bne lbl_8017C6F8
/* 8017C6D4 00179614  38 6D 97 A8 */	addi r3, r13, smList__15JALList$$05MSBgm$$1@sda21
/* 8017C6D8 00179618  4B E9 25 5D */	bl initiate__10JSUPtrListFv
/* 8017C6DC 0017961C  3C 60 80 0E */	lis r3, __dt__15JSUList$$05MSBgm$$1Fv@ha
/* 8017C6E0 00179620  38 83 6A 44 */	addi r4, r3, __dt__15JSUList$$05MSBgm$$1Fv@l
/* 8017C6E4 00179624  38 6D 97 A8 */	addi r3, r13, smList__15JALList$$05MSBgm$$1@sda21
/* 8017C6E8 00179628  38 BF 00 00 */	addi r5, r31, 0
/* 8017C6EC 0017962C  4B F0 60 3D */	bl __register_global_object
/* 8017C6F0 00179630  38 00 00 01 */	li r0, 1
/* 8017C6F4 00179634  98 0D 97 CC */	stb r0, __init__smList__15JALList$$05MSBgm$$1@sda21(r13)
lbl_8017C6F8:
/* 8017C6F8 00179638  88 0D 97 CD */	lbz r0, __init__smList__24JALList$$013MSSetSoundGrp$$1@sda21(r13)
/* 8017C6FC 0017963C  7C 00 07 75 */	extsb. r0, r0
/* 8017C700 00179640  40 82 00 28 */	bne lbl_8017C728
/* 8017C704 00179644  38 6D 97 B4 */	addi r3, r13, smList__24JALList$$013MSSetSoundGrp$$1@sda21
/* 8017C708 00179648  4B E9 25 2D */	bl initiate__10JSUPtrListFv
/* 8017C70C 0017964C  3C 60 80 0E */	lis r3, __dt__24JSUList$$013MSSetSoundGrp$$1Fv@ha
/* 8017C710 00179650  38 83 69 EC */	addi r4, r3, __dt__24JSUList$$013MSSetSoundGrp$$1Fv@l
/* 8017C714 00179654  38 6D 97 B4 */	addi r3, r13, smList__24JALList$$013MSSetSoundGrp$$1@sda21
/* 8017C718 00179658  38 BF 00 0C */	addi r5, r31, 0xc
/* 8017C71C 0017965C  4B F0 60 0D */	bl __register_global_object
/* 8017C720 00179660  38 00 00 01 */	li r0, 1
/* 8017C724 00179664  98 0D 97 CD */	stb r0, __init__smList__24JALList$$013MSSetSoundGrp$$1@sda21(r13)
lbl_8017C728:
/* 8017C728 00179668  88 0D 97 CE */	lbz r0, __init__smList__21JALList$$010MSSetSound$$1@sda21(r13)
/* 8017C72C 0017966C  7C 00 07 75 */	extsb. r0, r0
/* 8017C730 00179670  40 82 00 28 */	bne lbl_8017C758
/* 8017C734 00179674  38 6D 97 C0 */	addi r3, r13, smList__21JALList$$010MSSetSound$$1@sda21
/* 8017C738 00179678  4B E9 24 FD */	bl initiate__10JSUPtrListFv
/* 8017C73C 0017967C  3C 60 80 0E */	lis r3, __dt__21JSUList$$010MSSetSound$$1Fv@ha
/* 8017C740 00179680  38 83 69 94 */	addi r4, r3, __dt__21JSUList$$010MSSetSound$$1Fv@l
/* 8017C744 00179684  38 6D 97 C0 */	addi r3, r13, smList__21JALList$$010MSSetSound$$1@sda21
/* 8017C748 00179688  38 BF 00 18 */	addi r5, r31, 0x18
/* 8017C74C 0017968C  4B F0 5F DD */	bl __register_global_object
/* 8017C750 00179690  38 00 00 01 */	li r0, 1
/* 8017C754 00179694  98 0D 97 CE */	stb r0, __init__smList__21JALList$$010MSSetSound$$1@sda21(r13)
lbl_8017C758:
/* 8017C758 00179698  88 0D 8F 8C */	lbz r0, __init__smList__26JALList$$015JALSeModEffDGrp$$1@sda21(r13)
/* 8017C75C 0017969C  7C 00 07 75 */	extsb. r0, r0
/* 8017C760 001796A0  40 82 00 28 */	bne lbl_8017C788
/* 8017C764 001796A4  38 6D 8E FC */	addi r3, r13, smList__26JALList$$015JALSeModEffDGrp$$1@sda21
/* 8017C768 001796A8  4B E9 24 CD */	bl initiate__10JSUPtrListFv
/* 8017C76C 001796AC  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffDGrp$$1Fv@ha
/* 8017C770 001796B0  38 83 A8 0C */	addi r4, r3, __dt__26JSUList$$015JALSeModEffDGrp$$1Fv@l
/* 8017C774 001796B4  38 6D 8E FC */	addi r3, r13, smList__26JALList$$015JALSeModEffDGrp$$1@sda21
/* 8017C778 001796B8  38 BF 00 24 */	addi r5, r31, 0x24
/* 8017C77C 001796BC  4B F0 5F AD */	bl __register_global_object
/* 8017C780 001796C0  38 00 00 01 */	li r0, 1
/* 8017C784 001796C4  98 0D 8F 8C */	stb r0, __init__smList__26JALList$$015JALSeModEffDGrp$$1@sda21(r13)
lbl_8017C788:
/* 8017C788 001796C8  88 0D 8F 8D */	lbz r0, __init__smList__26JALList$$015JALSeModPitDGrp$$1@sda21(r13)
/* 8017C78C 001796CC  7C 00 07 75 */	extsb. r0, r0
/* 8017C790 001796D0  40 82 00 28 */	bne lbl_8017C7B8
/* 8017C794 001796D4  38 6D 8F 08 */	addi r3, r13, smList__26JALList$$015JALSeModPitDGrp$$1@sda21
/* 8017C798 001796D8  4B E9 24 9D */	bl initiate__10JSUPtrListFv
/* 8017C79C 001796DC  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitDGrp$$1Fv@ha
/* 8017C7A0 001796E0  38 83 A7 B4 */	addi r4, r3, __dt__26JSUList$$015JALSeModPitDGrp$$1Fv@l
/* 8017C7A4 001796E4  38 6D 8F 08 */	addi r3, r13, smList__26JALList$$015JALSeModPitDGrp$$1@sda21
/* 8017C7A8 001796E8  38 BF 00 30 */	addi r5, r31, 0x30
/* 8017C7AC 001796EC  4B F0 5F 7D */	bl __register_global_object
/* 8017C7B0 001796F0  38 00 00 01 */	li r0, 1
/* 8017C7B4 001796F4  98 0D 8F 8D */	stb r0, __init__smList__26JALList$$015JALSeModPitDGrp$$1@sda21(r13)
lbl_8017C7B8:
/* 8017C7B8 001796F8  88 0D 8F 8E */	lbz r0, __init__smList__26JALList$$015JALSeModVolDGrp$$1@sda21(r13)
/* 8017C7BC 001796FC  7C 00 07 75 */	extsb. r0, r0
/* 8017C7C0 00179700  40 82 00 28 */	bne lbl_8017C7E8
/* 8017C7C4 00179704  38 6D 8F 14 */	addi r3, r13, smList__26JALList$$015JALSeModVolDGrp$$1@sda21
/* 8017C7C8 00179708  4B E9 24 6D */	bl initiate__10JSUPtrListFv
/* 8017C7CC 0017970C  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolDGrp$$1Fv@ha
/* 8017C7D0 00179710  38 83 A7 5C */	addi r4, r3, __dt__26JSUList$$015JALSeModVolDGrp$$1Fv@l
/* 8017C7D4 00179714  38 6D 8F 14 */	addi r3, r13, smList__26JALList$$015JALSeModVolDGrp$$1@sda21
/* 8017C7D8 00179718  38 BF 00 3C */	addi r5, r31, 0x3c
/* 8017C7DC 0017971C  4B F0 5F 4D */	bl __register_global_object
/* 8017C7E0 00179720  38 00 00 01 */	li r0, 1
/* 8017C7E4 00179724  98 0D 8F 8E */	stb r0, __init__smList__26JALList$$015JALSeModVolDGrp$$1@sda21(r13)
lbl_8017C7E8:
/* 8017C7E8 00179728  88 0D 8F 8F */	lbz r0, __init__smList__26JALList$$015JALSeModEffFGrp$$1@sda21(r13)
/* 8017C7EC 0017972C  7C 00 07 75 */	extsb. r0, r0
/* 8017C7F0 00179730  40 82 00 28 */	bne lbl_8017C818
/* 8017C7F4 00179734  38 6D 8F 20 */	addi r3, r13, smList__26JALList$$015JALSeModEffFGrp$$1@sda21
/* 8017C7F8 00179738  4B E9 24 3D */	bl initiate__10JSUPtrListFv
/* 8017C7FC 0017973C  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffFGrp$$1Fv@ha
/* 8017C800 00179740  38 83 A7 04 */	addi r4, r3, __dt__26JSUList$$015JALSeModEffFGrp$$1Fv@l
/* 8017C804 00179744  38 6D 8F 20 */	addi r3, r13, smList__26JALList$$015JALSeModEffFGrp$$1@sda21
/* 8017C808 00179748  38 BF 00 48 */	addi r5, r31, 0x48
/* 8017C80C 0017974C  4B F0 5F 1D */	bl __register_global_object
/* 8017C810 00179750  38 00 00 01 */	li r0, 1
/* 8017C814 00179754  98 0D 8F 8F */	stb r0, __init__smList__26JALList$$015JALSeModEffFGrp$$1@sda21(r13)
lbl_8017C818:
/* 8017C818 00179758  88 0D 8F 90 */	lbz r0, __init__smList__26JALList$$015JALSeModPitFGrp$$1@sda21(r13)
/* 8017C81C 0017975C  7C 00 07 75 */	extsb. r0, r0
/* 8017C820 00179760  40 82 00 28 */	bne lbl_8017C848
/* 8017C824 00179764  38 6D 8F 2C */	addi r3, r13, smList__26JALList$$015JALSeModPitFGrp$$1@sda21
/* 8017C828 00179768  4B E9 24 0D */	bl initiate__10JSUPtrListFv
/* 8017C82C 0017976C  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitFGrp$$1Fv@ha
/* 8017C830 00179770  38 83 A6 AC */	addi r4, r3, __dt__26JSUList$$015JALSeModPitFGrp$$1Fv@l
/* 8017C834 00179774  38 6D 8F 2C */	addi r3, r13, smList__26JALList$$015JALSeModPitFGrp$$1@sda21
/* 8017C838 00179778  38 BF 00 54 */	addi r5, r31, 0x54
/* 8017C83C 0017977C  4B F0 5E ED */	bl __register_global_object
/* 8017C840 00179780  38 00 00 01 */	li r0, 1
/* 8017C844 00179784  98 0D 8F 90 */	stb r0, __init__smList__26JALList$$015JALSeModPitFGrp$$1@sda21(r13)
lbl_8017C848:
/* 8017C848 00179788  88 0D 8F 91 */	lbz r0, __init__smList__26JALList$$015JALSeModVolFGrp$$1@sda21(r13)
/* 8017C84C 0017978C  7C 00 07 75 */	extsb. r0, r0
/* 8017C850 00179790  40 82 00 28 */	bne lbl_8017C878
/* 8017C854 00179794  38 6D 8F 38 */	addi r3, r13, smList__26JALList$$015JALSeModVolFGrp$$1@sda21
/* 8017C858 00179798  4B E9 23 DD */	bl initiate__10JSUPtrListFv
/* 8017C85C 0017979C  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolFGrp$$1Fv@ha
/* 8017C860 001797A0  38 83 A6 54 */	addi r4, r3, __dt__26JSUList$$015JALSeModVolFGrp$$1Fv@l
/* 8017C864 001797A4  38 6D 8F 38 */	addi r3, r13, smList__26JALList$$015JALSeModVolFGrp$$1@sda21
/* 8017C868 001797A8  38 BF 00 60 */	addi r5, r31, 0x60
/* 8017C86C 001797AC  4B F0 5E BD */	bl __register_global_object
/* 8017C870 001797B0  38 00 00 01 */	li r0, 1
/* 8017C874 001797B4  98 0D 8F 91 */	stb r0, __init__smList__26JALList$$015JALSeModVolFGrp$$1@sda21(r13)
lbl_8017C878:
/* 8017C878 001797B8  88 0D 8F 92 */	lbz r0, __init__smList__26JALList$$015JALSeModEffDist$$1@sda21(r13)
/* 8017C87C 001797BC  7C 00 07 75 */	extsb. r0, r0
/* 8017C880 001797C0  40 82 00 28 */	bne lbl_8017C8A8
/* 8017C884 001797C4  38 6D 8F 44 */	addi r3, r13, smList__26JALList$$015JALSeModEffDist$$1@sda21
/* 8017C888 001797C8  4B E9 23 AD */	bl initiate__10JSUPtrListFv
/* 8017C88C 001797CC  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffDist$$1Fv@ha
/* 8017C890 001797D0  38 83 A5 FC */	addi r4, r3, __dt__26JSUList$$015JALSeModEffDist$$1Fv@l
/* 8017C894 001797D4  38 6D 8F 44 */	addi r3, r13, smList__26JALList$$015JALSeModEffDist$$1@sda21
/* 8017C898 001797D8  38 BF 00 6C */	addi r5, r31, 0x6c
/* 8017C89C 001797DC  4B F0 5E 8D */	bl __register_global_object
/* 8017C8A0 001797E0  38 00 00 01 */	li r0, 1
/* 8017C8A4 001797E4  98 0D 8F 92 */	stb r0, __init__smList__26JALList$$015JALSeModEffDist$$1@sda21(r13)
lbl_8017C8A8:
/* 8017C8A8 001797E8  88 0D 8F 93 */	lbz r0, __init__smList__26JALList$$015JALSeModPitDist$$1@sda21(r13)
/* 8017C8AC 001797EC  7C 00 07 75 */	extsb. r0, r0
/* 8017C8B0 001797F0  40 82 00 28 */	bne lbl_8017C8D8
/* 8017C8B4 001797F4  38 6D 8F 50 */	addi r3, r13, smList__26JALList$$015JALSeModPitDist$$1@sda21
/* 8017C8B8 001797F8  4B E9 23 7D */	bl initiate__10JSUPtrListFv
/* 8017C8BC 001797FC  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitDist$$1Fv@ha
/* 8017C8C0 00179800  38 83 A5 A4 */	addi r4, r3, __dt__26JSUList$$015JALSeModPitDist$$1Fv@l
/* 8017C8C4 00179804  38 6D 8F 50 */	addi r3, r13, smList__26JALList$$015JALSeModPitDist$$1@sda21
/* 8017C8C8 00179808  38 BF 00 78 */	addi r5, r31, 0x78
/* 8017C8CC 0017980C  4B F0 5E 5D */	bl __register_global_object
/* 8017C8D0 00179810  38 00 00 01 */	li r0, 1
/* 8017C8D4 00179814  98 0D 8F 93 */	stb r0, __init__smList__26JALList$$015JALSeModPitDist$$1@sda21(r13)
lbl_8017C8D8:
/* 8017C8D8 00179818  88 0D 8F 94 */	lbz r0, __init__smList__26JALList$$015JALSeModVolDist$$1@sda21(r13)
/* 8017C8DC 0017981C  7C 00 07 75 */	extsb. r0, r0
/* 8017C8E0 00179820  40 82 00 28 */	bne lbl_8017C908
/* 8017C8E4 00179824  38 6D 8F 5C */	addi r3, r13, smList__26JALList$$015JALSeModVolDist$$1@sda21
/* 8017C8E8 00179828  4B E9 23 4D */	bl initiate__10JSUPtrListFv
/* 8017C8EC 0017982C  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolDist$$1Fv@ha
/* 8017C8F0 00179830  38 83 A5 4C */	addi r4, r3, __dt__26JSUList$$015JALSeModVolDist$$1Fv@l
/* 8017C8F4 00179834  38 6D 8F 5C */	addi r3, r13, smList__26JALList$$015JALSeModVolDist$$1@sda21
/* 8017C8F8 00179838  38 BF 00 84 */	addi r5, r31, 0x84
/* 8017C8FC 0017983C  4B F0 5E 2D */	bl __register_global_object
/* 8017C900 00179840  38 00 00 01 */	li r0, 1
/* 8017C904 00179844  98 0D 8F 94 */	stb r0, __init__smList__26JALList$$015JALSeModVolDist$$1@sda21(r13)
lbl_8017C908:
/* 8017C908 00179848  88 0D 8F 95 */	lbz r0, __init__smList__26JALList$$015JALSeModEffFunk$$1@sda21(r13)
/* 8017C90C 0017984C  7C 00 07 75 */	extsb. r0, r0
/* 8017C910 00179850  40 82 00 28 */	bne lbl_8017C938
/* 8017C914 00179854  38 6D 8F 68 */	addi r3, r13, smList__26JALList$$015JALSeModEffFunk$$1@sda21
/* 8017C918 00179858  4B E9 23 1D */	bl initiate__10JSUPtrListFv
/* 8017C91C 0017985C  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffFunk$$1Fv@ha
/* 8017C920 00179860  38 83 A4 F4 */	addi r4, r3, __dt__26JSUList$$015JALSeModEffFunk$$1Fv@l
/* 8017C924 00179864  38 6D 8F 68 */	addi r3, r13, smList__26JALList$$015JALSeModEffFunk$$1@sda21
/* 8017C928 00179868  38 BF 00 90 */	addi r5, r31, 0x90
/* 8017C92C 0017986C  4B F0 5D FD */	bl __register_global_object
/* 8017C930 00179870  38 00 00 01 */	li r0, 1
/* 8017C934 00179874  98 0D 8F 95 */	stb r0, __init__smList__26JALList$$015JALSeModEffFunk$$1@sda21(r13)
lbl_8017C938:
/* 8017C938 00179878  88 0D 8F 96 */	lbz r0, __init__smList__26JALList$$015JALSeModPitFunk$$1@sda21(r13)
/* 8017C93C 0017987C  7C 00 07 75 */	extsb. r0, r0
/* 8017C940 00179880  40 82 00 28 */	bne lbl_8017C968
/* 8017C944 00179884  38 6D 8F 74 */	addi r3, r13, smList__26JALList$$015JALSeModPitFunk$$1@sda21
/* 8017C948 00179888  4B E9 22 ED */	bl initiate__10JSUPtrListFv
/* 8017C94C 0017988C  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitFunk$$1Fv@ha
/* 8017C950 00179890  38 83 A4 9C */	addi r4, r3, __dt__26JSUList$$015JALSeModPitFunk$$1Fv@l
/* 8017C954 00179894  38 6D 8F 74 */	addi r3, r13, smList__26JALList$$015JALSeModPitFunk$$1@sda21
/* 8017C958 00179898  38 BF 00 9C */	addi r5, r31, 0x9c
/* 8017C95C 0017989C  4B F0 5D CD */	bl __register_global_object
/* 8017C960 001798A0  38 00 00 01 */	li r0, 1
/* 8017C964 001798A4  98 0D 8F 96 */	stb r0, __init__smList__26JALList$$015JALSeModPitFunk$$1@sda21(r13)
lbl_8017C968:
/* 8017C968 001798A8  88 0D 8F 97 */	lbz r0, __init__smList__26JALList$$015JALSeModVolFunk$$1@sda21(r13)
/* 8017C96C 001798AC  7C 00 07 75 */	extsb. r0, r0
/* 8017C970 001798B0  40 82 00 28 */	bne lbl_8017C998
/* 8017C974 001798B4  38 6D 8F 80 */	addi r3, r13, smList__26JALList$$015JALSeModVolFunk$$1@sda21
/* 8017C978 001798B8  4B E9 22 BD */	bl initiate__10JSUPtrListFv
/* 8017C97C 001798BC  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolFunk$$1Fv@ha
/* 8017C980 001798C0  38 83 A4 44 */	addi r4, r3, __dt__26JSUList$$015JALSeModVolFunk$$1Fv@l
/* 8017C984 001798C4  38 6D 8F 80 */	addi r3, r13, smList__26JALList$$015JALSeModVolFunk$$1@sda21
/* 8017C988 001798C8  38 BF 00 A8 */	addi r5, r31, 0xa8
/* 8017C98C 001798CC  4B F0 5D 9D */	bl __register_global_object
/* 8017C990 001798D0  38 00 00 01 */	li r0, 1
/* 8017C994 001798D4  98 0D 8F 97 */	stb r0, __init__smList__26JALList$$015JALSeModVolFunk$$1@sda21(r13)
lbl_8017C998:
/* 8017C998 001798D8  80 01 00 14 */	lwz r0, 0x14(r1)
/* 8017C99C 001798DC  83 E1 00 0C */	lwz r31, 0xc(r1)
/* 8017C9A0 001798E0  38 21 00 10 */	addi r1, r1, 0x10
/* 8017C9A4 001798E4  7C 08 03 A6 */	mtlr r0
/* 8017C9A8 001798E8  4E 80 00 20 */	blr 

.section .ctors, "wa"  # 0x8036FBA0 - 0x8036FF80
	.incbin "baserom.dol", 0x36CCD8, 0x4

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global __vt__12MAnmSoundNPC
__vt__12MAnmSoundNPC:
  .4byte 0
  .4byte 0
  .4byte startAnimSound__12MAnmSoundNPCFPvUlPP8JAISoundP8JAIActorUc
  .4byte setSpeedModifySound__9MAnmSoundFP8JAISoundP22JAIAnimeFrameSoundDataf
.global __vt__9MAnmSound
__vt__9MAnmSound:
  .4byte 0
  .4byte 0
  .4byte startAnimSound__9MAnmSoundFPvUlPP8JAISoundP8JAIActorUc
  .4byte setSpeedModifySound__9MAnmSoundFP8JAISoundP22JAIAnimeFrameSoundDataf

.section .sdata2, "wa"  # 0x8040B460 - 0x80414020
$$21981:
	.incbin "baserom.dol", 0x3E66F8, 0x4
$$21982:
	.incbin "baserom.dol", 0x3E66FC, 0x4
$$21983:
	.incbin "baserom.dol", 0x3E6700, 0x4
$$21984:
	.incbin "baserom.dol", 0x3E6704, 0x4
$$21985:
	.incbin "baserom.dol", 0x3E6708, 0x8
$$21987:
	.incbin "baserom.dol", 0x3E6710, 0x8

.section .bss, "wa"  # 0x803E6000 - 0x80408AC0
$$22044:
	.skip 0xC
$$22045:
	.skip 0xC
$$22046:
	.skip 0xC
$$22047:
	.skip 0xC
$$22048:
	.skip 0xC
$$22049:
	.skip 0xC
$$22050:
	.skip 0xC
$$22051:
	.skip 0xC
$$22052:
	.skip 0xC
$$22053:
	.skip 0xC
$$22054:
	.skip 0xC
$$22055:
	.skip 0xC
$$22056:
	.skip 0xC
$$22057:
	.skip 0xC
$$22058:
	.skip 0x10
