.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global __dt__14TNerveSBH_FallFv
__dt__14TNerveSBH_FallFv:
/* 802FC2B8 002F91F8  7C 08 02 A6 */	mflr r0
/* 802FC2BC 002F91FC  90 01 00 04 */	stw r0, 4(r1)
/* 802FC2C0 002F9200  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 802FC2C4 002F9204  93 E1 00 14 */	stw r31, 0x14(r1)
/* 802FC2C8 002F9208  7C 7F 1B 79 */	or. r31, r3, r3
/* 802FC2CC 002F920C  41 82 00 30 */	beq lbl_802FC2FC
/* 802FC2D0 002F9210  3C 60 80 3E */	lis r3, __vt__14TNerveSBH_Fall@ha
/* 802FC2D4 002F9214  38 03 EE E8 */	addi r0, r3, __vt__14TNerveSBH_Fall@l
/* 802FC2D8 002F9218  90 1F 00 00 */	stw r0, 0(r31)
/* 802FC2DC 002F921C  41 82 00 10 */	beq lbl_802FC2EC
/* 802FC2E0 002F9220  3C 60 80 3C */	lis r3, __vt__24TNerveBase$$010TLiveActor$$1@ha
/* 802FC2E4 002F9224  38 03 92 60 */	addi r0, r3, __vt__24TNerveBase$$010TLiveActor$$1@l
/* 802FC2E8 002F9228  90 1F 00 00 */	stw r0, 0(r31)
lbl_802FC2EC:
/* 802FC2EC 002F922C  7C 80 07 35 */	extsh. r0, r4
/* 802FC2F0 002F9230  40 81 00 0C */	ble lbl_802FC2FC
/* 802FC2F4 002F9234  7F E3 FB 78 */	mr r3, r31
/* 802FC2F8 002F9238  4B D1 07 B9 */	bl __dl__FPv
lbl_802FC2FC:
/* 802FC2FC 002F923C  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 802FC300 002F9240  7F E3 FB 78 */	mr r3, r31
/* 802FC304 002F9244  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 802FC308 002F9248  38 21 00 18 */	addi r1, r1, 0x18
/* 802FC30C 002F924C  7C 08 03 A6 */	mtlr r0
/* 802FC310 002F9250  4E 80 00 20 */	blr 

.global execute__14TNerveSBH_FallCFP24TSpineBase$$010TLiveActor$$1
execute__14TNerveSBH_FallCFP24TSpineBase$$010TLiveActor$$1:
/* 802FC314 002F9254  7C 08 02 A6 */	mflr r0
/* 802FC318 002F9258  38 A0 00 00 */	li r5, 0
/* 802FC31C 002F925C  90 01 00 04 */	stw r0, 4(r1)
/* 802FC320 002F9260  94 21 FF C0 */	stwu r1, -0x40(r1)
/* 802FC324 002F9264  93 E1 00 3C */	stw r31, 0x3c(r1)
/* 802FC328 002F9268  7C 9F 23 78 */	mr r31, r4
/* 802FC32C 002F926C  93 C1 00 38 */	stw r30, 0x38(r1)
/* 802FC330 002F9270  83 C4 00 00 */	lwz r30, 0(r4)
/* 802FC334 002F9274  38 80 00 00 */	li r4, 0
/* 802FC338 002F9278  80 7E 00 74 */	lwz r3, 0x74(r30)
/* 802FC33C 002F927C  4B DD 8E 71 */	bl curAnmEndsNext__6MActorFiPc
/* 802FC340 002F9280  2C 03 00 00 */	cmpwi r3, 0
/* 802FC344 002F9284  41 82 00 F0 */	beq lbl_802FC434
/* 802FC348 002F9288  80 BE 01 50 */	lwz r5, 0x150(r30)
/* 802FC34C 002F928C  3C 80 80 3A */	lis r4, $$22311@ha
/* 802FC350 002F9290  80 1E 01 54 */	lwz r0, 0x154(r30)
/* 802FC354 002F9294  3C 60 80 3A */	lis r3, $$22312@ha
/* 802FC358 002F9298  38 84 F8 D4 */	addi r4, r4, $$22311@l
/* 802FC35C 002F929C  90 A1 00 28 */	stw r5, 0x28(r1)
/* 802FC360 002F92A0  38 A3 F8 E8 */	addi r5, r3, $$22312@l
/* 802FC364 002F92A4  80 6D 9A 18 */	lwz r3, gpItemManager-_SDA_BASE_(r13)
/* 802FC368 002F92A8  90 01 00 2C */	stw r0, 0x2c(r1)
/* 802FC36C 002F92AC  80 1E 01 58 */	lwz r0, 0x158(r30)
/* 802FC370 002F92B0  90 01 00 30 */	stw r0, 0x30(r1)
/* 802FC374 002F92B4  C0 21 00 28 */	lfs f1, 0x28(r1)
/* 802FC378 002F92B8  C0 41 00 2C */	lfs f2, 0x2c(r1)
/* 802FC37C 002F92BC  C0 61 00 30 */	lfs f3, 0x30(r1)
/* 802FC380 002F92C0  4B E9 BC 51 */	bl makeShineAppearWithDemo__12TItemManagerFPCcPCcfff
/* 802FC384 002F92C4  80 03 00 F8 */	lwz r0, 0xf8(r3)
/* 802FC388 002F92C8  64 00 20 00 */	oris r0, r0, 0x2000
/* 802FC38C 002F92CC  90 03 00 F8 */	stw r0, 0xf8(r3)
/* 802FC390 002F92D0  80 1E 00 F0 */	lwz r0, 0xf0(r30)
/* 802FC394 002F92D4  60 00 00 01 */	ori r0, r0, 1
/* 802FC398 002F92D8  90 1E 00 F0 */	stw r0, 0xf0(r30)
/* 802FC39C 002F92DC  80 7E 01 5C */	lwz r3, 0x15c(r30)
/* 802FC3A0 002F92E0  A0 03 00 1A */	lhz r0, 0x1a(r3)
/* 802FC3A4 002F92E4  60 00 00 01 */	ori r0, r0, 1
/* 802FC3A8 002F92E8  B0 03 00 1A */	sth r0, 0x1a(r3)
/* 802FC3AC 002F92EC  80 63 00 14 */	lwz r3, 0x14(r3)
/* 802FC3B0 002F92F0  4B DD 46 D9 */	bl SMS_HideAllShapePacket__FP8J3DModel
/* 802FC3B4 002F92F4  88 0D A4 98 */	lbz r0, init$2266-_SDA_BASE_(r13)
/* 802FC3B8 002F92F8  7C 00 07 75 */	extsb. r0, r0
/* 802FC3BC 002F92FC  40 82 00 3C */	bne lbl_802FC3F8
/* 802FC3C0 002F9300  3C 60 80 3C */	lis r3, __vt__24TNerveBase$$010TLiveActor$$1@ha
/* 802FC3C4 002F9304  38 03 92 60 */	addi r0, r3, __vt__24TNerveBase$$010TLiveActor$$1@l
/* 802FC3C8 002F9308  3C 60 80 3E */	lis r3, __vt__23TNerveSBH_SleepContinue@ha
/* 802FC3CC 002F930C  90 0D A4 9C */	stw r0, instance$2264-_SDA_BASE_(r13)
/* 802FC3D0 002F9310  38 03 EE F8 */	addi r0, r3, __vt__23TNerveSBH_SleepContinue@l
/* 802FC3D4 002F9314  3C 80 80 30 */	lis r4, __dt__23TNerveSBH_SleepContinueFv@ha
/* 802FC3D8 002F9318  90 0D A4 9C */	stw r0, instance$2264-_SDA_BASE_(r13)
/* 802FC3DC 002F931C  3C 60 80 40 */	lis r3, $$22265@ha
/* 802FC3E0 002F9320  38 A3 EC 20 */	addi r5, r3, $$22265@l
/* 802FC3E4 002F9324  38 84 C4 50 */	addi r4, r4, __dt__23TNerveSBH_SleepContinueFv@l
/* 802FC3E8 002F9328  38 6D A4 9C */	addi r3, r13, instance$2264-_SDA_BASE_
/* 802FC3EC 002F932C  4B D8 63 3D */	bl __register_global_object
/* 802FC3F0 002F9330  38 00 00 01 */	li r0, 1
/* 802FC3F4 002F9334  98 0D A4 98 */	stb r0, init$2266-_SDA_BASE_(r13)
lbl_802FC3F8:
/* 802FC3F8 002F9338  38 8D A4 9C */	addi r4, r13, instance$2264-_SDA_BASE_
/* 802FC3FC 002F933C  28 04 00 00 */	cmplwi r4, 0
/* 802FC400 002F9340  41 82 00 2C */	beq lbl_802FC42C
/* 802FC404 002F9344  80 BF 00 08 */	lwz r5, 8(r31)
/* 802FC408 002F9348  80 1F 00 04 */	lwz r0, 4(r31)
/* 802FC40C 002F934C  7C 05 00 00 */	cmpw r5, r0
/* 802FC410 002F9350  40 80 00 1C */	bge lbl_802FC42C
/* 802FC414 002F9354  80 7F 00 0C */	lwz r3, 0xc(r31)
/* 802FC418 002F9358  54 A0 10 3A */	slwi r0, r5, 2
/* 802FC41C 002F935C  7C 83 01 2E */	stwx r4, r3, r0
/* 802FC420 002F9360  80 7F 00 08 */	lwz r3, 8(r31)
/* 802FC424 002F9364  38 03 00 01 */	addi r0, r3, 1
/* 802FC428 002F9368  90 1F 00 08 */	stw r0, 8(r31)
lbl_802FC42C:
/* 802FC42C 002F936C  38 60 00 01 */	li r3, 1
/* 802FC430 002F9370  48 00 00 08 */	b lbl_802FC438
lbl_802FC434:
/* 802FC434 002F9374  38 60 00 00 */	li r3, 0
lbl_802FC438:
/* 802FC438 002F9378  80 01 00 44 */	lwz r0, 0x44(r1)
/* 802FC43C 002F937C  83 E1 00 3C */	lwz r31, 0x3c(r1)
/* 802FC440 002F9380  83 C1 00 38 */	lwz r30, 0x38(r1)
/* 802FC444 002F9384  7C 08 03 A6 */	mtlr r0
/* 802FC448 002F9388  38 21 00 40 */	addi r1, r1, 0x40
/* 802FC44C 002F938C  4E 80 00 20 */	blr 

.global __dt__23TNerveSBH_SleepContinueFv
__dt__23TNerveSBH_SleepContinueFv:
/* 802FC450 002F9390  7C 08 02 A6 */	mflr r0
/* 802FC454 002F9394  90 01 00 04 */	stw r0, 4(r1)
/* 802FC458 002F9398  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 802FC45C 002F939C  93 E1 00 14 */	stw r31, 0x14(r1)
/* 802FC460 002F93A0  7C 7F 1B 79 */	or. r31, r3, r3
/* 802FC464 002F93A4  41 82 00 30 */	beq lbl_802FC494
/* 802FC468 002F93A8  3C 60 80 3E */	lis r3, __vt__23TNerveSBH_SleepContinue@ha
/* 802FC46C 002F93AC  38 03 EE F8 */	addi r0, r3, __vt__23TNerveSBH_SleepContinue@l
/* 802FC470 002F93B0  90 1F 00 00 */	stw r0, 0(r31)
/* 802FC474 002F93B4  41 82 00 10 */	beq lbl_802FC484
/* 802FC478 002F93B8  3C 60 80 3C */	lis r3, __vt__24TNerveBase$$010TLiveActor$$1@ha
/* 802FC47C 002F93BC  38 03 92 60 */	addi r0, r3, __vt__24TNerveBase$$010TLiveActor$$1@l
/* 802FC480 002F93C0  90 1F 00 00 */	stw r0, 0(r31)
lbl_802FC484:
/* 802FC484 002F93C4  7C 80 07 35 */	extsh. r0, r4
/* 802FC488 002F93C8  40 81 00 0C */	ble lbl_802FC494
/* 802FC48C 002F93CC  7F E3 FB 78 */	mr r3, r31
/* 802FC490 002F93D0  4B D1 06 21 */	bl __dl__FPv
lbl_802FC494:
/* 802FC494 002F93D4  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 802FC498 002F93D8  7F E3 FB 78 */	mr r3, r31
/* 802FC49C 002F93DC  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 802FC4A0 002F93E0  38 21 00 18 */	addi r1, r1, 0x18
/* 802FC4A4 002F93E4  7C 08 03 A6 */	mtlr r0
/* 802FC4A8 002F93E8  4E 80 00 20 */	blr 

.global theNerve__14TNerveSBH_FallFv
theNerve__14TNerveSBH_FallFv:
/* 802FC4AC 002F93EC  7C 08 02 A6 */	mflr r0
/* 802FC4B0 002F93F0  90 01 00 04 */	stw r0, 4(r1)
/* 802FC4B4 002F93F4  94 21 FF F8 */	stwu r1, -8(r1)
/* 802FC4B8 002F93F8  88 0D A4 A0 */	lbz r0, init$2272-_SDA_BASE_(r13)
/* 802FC4BC 002F93FC  7C 00 07 75 */	extsb. r0, r0
/* 802FC4C0 002F9400  40 82 00 3C */	bne lbl_802FC4FC
/* 802FC4C4 002F9404  3C 60 80 3C */	lis r3, __vt__24TNerveBase$$010TLiveActor$$1@ha
/* 802FC4C8 002F9408  38 03 92 60 */	addi r0, r3, __vt__24TNerveBase$$010TLiveActor$$1@l
/* 802FC4CC 002F940C  3C 60 80 3E */	lis r3, __vt__14TNerveSBH_Fall@ha
/* 802FC4D0 002F9410  90 0D A4 A4 */	stw r0, instance$2270-_SDA_BASE_(r13)
/* 802FC4D4 002F9414  38 03 EE E8 */	addi r0, r3, __vt__14TNerveSBH_Fall@l
/* 802FC4D8 002F9418  3C 80 80 30 */	lis r4, __dt__14TNerveSBH_FallFv@ha
/* 802FC4DC 002F941C  90 0D A4 A4 */	stw r0, instance$2270-_SDA_BASE_(r13)
/* 802FC4E0 002F9420  3C 60 80 40 */	lis r3, $$22271@ha
/* 802FC4E4 002F9424  38 A3 EC 2C */	addi r5, r3, $$22271@l
/* 802FC4E8 002F9428  38 84 C2 B8 */	addi r4, r4, __dt__14TNerveSBH_FallFv@l
/* 802FC4EC 002F942C  38 6D A4 A4 */	addi r3, r13, instance$2270-_SDA_BASE_
/* 802FC4F0 002F9430  4B D8 62 39 */	bl __register_global_object
/* 802FC4F4 002F9434  38 00 00 01 */	li r0, 1
/* 802FC4F8 002F9438  98 0D A4 A0 */	stb r0, init$2272-_SDA_BASE_(r13)
lbl_802FC4FC:
/* 802FC4FC 002F943C  80 01 00 0C */	lwz r0, 0xc(r1)
/* 802FC500 002F9440  38 6D A4 A4 */	addi r3, r13, instance$2270-_SDA_BASE_
/* 802FC504 002F9444  38 21 00 08 */	addi r1, r1, 8
/* 802FC508 002F9448  7C 08 03 A6 */	mtlr r0
/* 802FC50C 002F944C  4E 80 00 20 */	blr 

.global execute__23TNerveSBH_SleepContinueCFP24TSpineBase$$010TLiveActor$$1
execute__23TNerveSBH_SleepContinueCFP24TSpineBase$$010TLiveActor$$1:
/* 802FC510 002F9450  38 60 00 00 */	li r3, 0
/* 802FC514 002F9454  4E 80 00 20 */	blr 

.global theNerve__23TNerveSBH_SleepContinueFv
theNerve__23TNerveSBH_SleepContinueFv:
/* 802FC518 002F9458  7C 08 02 A6 */	mflr r0
/* 802FC51C 002F945C  90 01 00 04 */	stw r0, 4(r1)
/* 802FC520 002F9460  94 21 FF F8 */	stwu r1, -8(r1)
/* 802FC524 002F9464  88 0D A4 98 */	lbz r0, init$2266-_SDA_BASE_(r13)
/* 802FC528 002F9468  7C 00 07 75 */	extsb. r0, r0
/* 802FC52C 002F946C  40 82 00 3C */	bne lbl_802FC568
/* 802FC530 002F9470  3C 60 80 3C */	lis r3, __vt__24TNerveBase$$010TLiveActor$$1@ha
/* 802FC534 002F9474  38 03 92 60 */	addi r0, r3, __vt__24TNerveBase$$010TLiveActor$$1@l
/* 802FC538 002F9478  3C 60 80 3E */	lis r3, __vt__23TNerveSBH_SleepContinue@ha
/* 802FC53C 002F947C  90 0D A4 9C */	stw r0, instance$2264-_SDA_BASE_(r13)
/* 802FC540 002F9480  38 03 EE F8 */	addi r0, r3, __vt__23TNerveSBH_SleepContinue@l
/* 802FC544 002F9484  3C 80 80 30 */	lis r4, __dt__23TNerveSBH_SleepContinueFv@ha
/* 802FC548 002F9488  90 0D A4 9C */	stw r0, instance$2264-_SDA_BASE_(r13)
/* 802FC54C 002F948C  3C 60 80 40 */	lis r3, $$22265@ha
/* 802FC550 002F9490  38 A3 EC 20 */	addi r5, r3, $$22265@l
/* 802FC554 002F9494  38 84 C4 50 */	addi r4, r4, __dt__23TNerveSBH_SleepContinueFv@l
/* 802FC558 002F9498  38 6D A4 9C */	addi r3, r13, instance$2264-_SDA_BASE_
/* 802FC55C 002F949C  4B D8 61 CD */	bl __register_global_object
/* 802FC560 002F94A0  38 00 00 01 */	li r0, 1
/* 802FC564 002F94A4  98 0D A4 98 */	stb r0, init$2266-_SDA_BASE_(r13)
lbl_802FC568:
/* 802FC568 002F94A8  80 01 00 0C */	lwz r0, 0xc(r1)
/* 802FC56C 002F94AC  38 6D A4 9C */	addi r3, r13, instance$2264-_SDA_BASE_
/* 802FC570 002F94B0  38 21 00 08 */	addi r1, r1, 8
/* 802FC574 002F94B4  7C 08 03 A6 */	mtlr r0
/* 802FC578 002F94B8  4E 80 00 20 */	blr 

.global startFall__18TSleepBossHanachanFfff
startFall__18TSleepBossHanachanFfff:
/* 802FC57C 002F94BC  7C 08 02 A6 */	mflr r0
/* 802FC580 002F94C0  3C 80 00 05 */	lis r4, 0x0005000B@ha
/* 802FC584 002F94C4  90 01 00 04 */	stw r0, 4(r1)
/* 802FC588 002F94C8  38 A4 00 0B */	addi r5, r4, 0x0005000B@l
/* 802FC58C 002F94CC  38 80 00 01 */	li r4, 1
/* 802FC590 002F94D0  94 21 FF B8 */	stwu r1, -0x48(r1)
/* 802FC594 002F94D4  DB E1 00 40 */	stfd f31, 0x40(r1)
/* 802FC598 002F94D8  FF E0 18 90 */	fmr f31, f3
/* 802FC59C 002F94DC  DB C1 00 38 */	stfd f30, 0x38(r1)
/* 802FC5A0 002F94E0  FF C0 10 90 */	fmr f30, f2
/* 802FC5A4 002F94E4  DB A1 00 30 */	stfd f29, 0x30(r1)
/* 802FC5A8 002F94E8  FF A0 08 90 */	fmr f29, f1
/* 802FC5AC 002F94EC  93 E1 00 2C */	stw r31, 0x2c(r1)
/* 802FC5B0 002F94F0  7C 7F 1B 78 */	mr r31, r3
/* 802FC5B4 002F94F4  80 0D 97 D0 */	lwz r0, smInstance__12TFlagManager-_SDA_BASE_(r13)
/* 802FC5B8 002F94F8  7C 03 03 78 */	mr r3, r0
/* 802FC5BC 002F94FC  4B DE B8 E5 */	bl setBool__12TFlagManagerFbUl
/* 802FC5C0 002F9500  D3 BF 01 50 */	stfs f29, 0x150(r31)
/* 802FC5C4 002F9504  38 80 00 00 */	li r4, 0
/* 802FC5C8 002F9508  D3 DF 01 54 */	stfs f30, 0x154(r31)
/* 802FC5CC 002F950C  D3 FF 01 58 */	stfs f31, 0x158(r31)
/* 802FC5D0 002F9510  80 7F 00 74 */	lwz r3, 0x74(r31)
/* 802FC5D4 002F9514  4B DD 7B 4D */	bl setBckFromIndex__6MActorFi
/* 802FC5D8 002F9518  7F E3 FB 78 */	mr r3, r31
/* 802FC5DC 002F951C  4B E1 4A AD */	bl setCurAnmSound__10TLiveActorFv
/* 802FC5E0 002F9520  88 0D A4 A0 */	lbz r0, init$2272-_SDA_BASE_(r13)
/* 802FC5E4 002F9524  7C 00 07 75 */	extsb. r0, r0
/* 802FC5E8 002F9528  40 82 00 3C */	bne lbl_802FC624
/* 802FC5EC 002F952C  3C 60 80 3C */	lis r3, __vt__24TNerveBase$$010TLiveActor$$1@ha
/* 802FC5F0 002F9530  38 03 92 60 */	addi r0, r3, __vt__24TNerveBase$$010TLiveActor$$1@l
/* 802FC5F4 002F9534  3C 60 80 3E */	lis r3, __vt__14TNerveSBH_Fall@ha
/* 802FC5F8 002F9538  90 0D A4 A4 */	stw r0, instance$2270-_SDA_BASE_(r13)
/* 802FC5FC 002F953C  38 03 EE E8 */	addi r0, r3, __vt__14TNerveSBH_Fall@l
/* 802FC600 002F9540  3C 80 80 30 */	lis r4, __dt__14TNerveSBH_FallFv@ha
/* 802FC604 002F9544  90 0D A4 A4 */	stw r0, instance$2270-_SDA_BASE_(r13)
/* 802FC608 002F9548  3C 60 80 40 */	lis r3, $$22271@ha
/* 802FC60C 002F954C  38 A3 EC 2C */	addi r5, r3, $$22271@l
/* 802FC610 002F9550  38 84 C2 B8 */	addi r4, r4, __dt__14TNerveSBH_FallFv@l
/* 802FC614 002F9554  38 6D A4 A4 */	addi r3, r13, instance$2270-_SDA_BASE_
/* 802FC618 002F9558  4B D8 61 11 */	bl __register_global_object
/* 802FC61C 002F955C  38 00 00 01 */	li r0, 1
/* 802FC620 002F9560  98 0D A4 A0 */	stb r0, init$2272-_SDA_BASE_(r13)
lbl_802FC624:
/* 802FC624 002F9564  80 9F 00 8C */	lwz r4, 0x8c(r31)
/* 802FC628 002F9568  38 6D A4 A4 */	addi r3, r13, instance$2270-_SDA_BASE_
/* 802FC62C 002F956C  80 04 00 14 */	lwz r0, 0x14(r4)
/* 802FC630 002F9570  28 00 00 00 */	cmplwi r0, 0
/* 802FC634 002F9574  41 82 00 08 */	beq lbl_802FC63C
/* 802FC638 002F9578  90 04 00 1C */	stw r0, 0x1c(r4)
lbl_802FC63C:
/* 802FC63C 002F957C  38 00 00 00 */	li r0, 0
/* 802FC640 002F9580  90 04 00 20 */	stw r0, 0x20(r4)
/* 802FC644 002F9584  90 64 00 14 */	stw r3, 0x14(r4)
/* 802FC648 002F9588  80 01 00 4C */	lwz r0, 0x4c(r1)
/* 802FC64C 002F958C  CB E1 00 40 */	lfd f31, 0x40(r1)
/* 802FC650 002F9590  CB C1 00 38 */	lfd f30, 0x38(r1)
/* 802FC654 002F9594  CB A1 00 30 */	lfd f29, 0x30(r1)
/* 802FC658 002F9598  83 E1 00 2C */	lwz r31, 0x2c(r1)
/* 802FC65C 002F959C  38 21 00 48 */	addi r1, r1, 0x48
/* 802FC660 002F95A0  7C 08 03 A6 */	mtlr r0
/* 802FC664 002F95A4  4E 80 00 20 */	blr 

.global getBasNameTable__18TSleepBossHanachanCFv
getBasNameTable__18TSleepBossHanachanCFv:
/* 802FC668 002F95A8  38 6D 8A 88 */	addi r3, r13, sleepBossHanachan_bastable-_SDA_BASE_
/* 802FC66C 002F95AC  4E 80 00 20 */	blr 

.global calcRootMatrix__18TSleepBossHanachanFv
calcRootMatrix__18TSleepBossHanachanFv:
/* 802FC670 002F95B0  4E 80 00 20 */	blr 

.global init__18TSleepBossHanachanFP12TLiveManager
init__18TSleepBossHanachanFP12TLiveManager:
/* 802FC674 002F95B4  7C 08 02 A6 */	mflr r0
/* 802FC678 002F95B8  38 A0 00 03 */	li r5, 3
/* 802FC67C 002F95BC  90 01 00 04 */	stw r0, 4(r1)
/* 802FC680 002F95C0  94 21 FF 88 */	stwu r1, -0x78(r1)
/* 802FC684 002F95C4  DB E1 00 70 */	stfd f31, 0x70(r1)
/* 802FC688 002F95C8  DB C1 00 68 */	stfd f30, 0x68(r1)
/* 802FC68C 002F95CC  DB A1 00 60 */	stfd f29, 0x60(r1)
/* 802FC690 002F95D0  DB 81 00 58 */	stfd f28, 0x58(r1)
/* 802FC694 002F95D4  DB 61 00 50 */	stfd f27, 0x50(r1)
/* 802FC698 002F95D8  DB 41 00 48 */	stfd f26, 0x48(r1)
/* 802FC69C 002F95DC  93 E1 00 44 */	stw r31, 0x44(r1)
/* 802FC6A0 002F95E0  3B E3 00 00 */	addi r31, r3, 0
/* 802FC6A4 002F95E4  93 C1 00 40 */	stw r30, 0x40(r1)
/* 802FC6A8 002F95E8  48 00 02 C9 */	bl initBase__17TDemoBossHanachanFP12TLiveManagerUl
/* 802FC6AC 002F95EC  88 0D A4 98 */	lbz r0, init$2266-_SDA_BASE_(r13)
/* 802FC6B0 002F95F0  7C 00 07 75 */	extsb. r0, r0
/* 802FC6B4 002F95F4  40 82 00 3C */	bne lbl_802FC6F0
/* 802FC6B8 002F95F8  3C 60 80 3C */	lis r3, __vt__24TNerveBase$$010TLiveActor$$1@ha
/* 802FC6BC 002F95FC  38 03 92 60 */	addi r0, r3, __vt__24TNerveBase$$010TLiveActor$$1@l
/* 802FC6C0 002F9600  3C 60 80 3E */	lis r3, __vt__23TNerveSBH_SleepContinue@ha
/* 802FC6C4 002F9604  90 0D A4 9C */	stw r0, instance$2264-_SDA_BASE_(r13)
/* 802FC6C8 002F9608  38 03 EE F8 */	addi r0, r3, __vt__23TNerveSBH_SleepContinue@l
/* 802FC6CC 002F960C  3C 80 80 30 */	lis r4, __dt__23TNerveSBH_SleepContinueFv@ha
/* 802FC6D0 002F9610  90 0D A4 9C */	stw r0, instance$2264-_SDA_BASE_(r13)
/* 802FC6D4 002F9614  3C 60 80 40 */	lis r3, $$22265@ha
/* 802FC6D8 002F9618  38 A3 EC 20 */	addi r5, r3, $$22265@l
/* 802FC6DC 002F961C  38 84 C4 50 */	addi r4, r4, __dt__23TNerveSBH_SleepContinueFv@l
/* 802FC6E0 002F9620  38 6D A4 9C */	addi r3, r13, instance$2264-_SDA_BASE_
/* 802FC6E4 002F9624  4B D8 60 45 */	bl __register_global_object
/* 802FC6E8 002F9628  38 00 00 01 */	li r0, 1
/* 802FC6EC 002F962C  98 0D A4 98 */	stb r0, init$2266-_SDA_BASE_(r13)
lbl_802FC6F0:
/* 802FC6F0 002F9630  81 1F 00 8C */	lwz r8, 0x8c(r31)
/* 802FC6F4 002F9634  38 E0 00 00 */	li r7, 0
/* 802FC6F8 002F9638  3C 80 08 00 */	lis r4, 0x08000016@ha
/* 802FC6FC 002F963C  90 E8 00 08 */	stw r7, 8(r8)
/* 802FC700 002F9640  38 0D A4 9C */	addi r0, r13, instance$2264-_SDA_BASE_
/* 802FC704 002F9644  38 7F 00 00 */	addi r3, r31, 0
/* 802FC708 002F9648  90 E8 00 20 */	stw r7, 0x20(r8)
/* 802FC70C 002F964C  38 84 00 16 */	addi r4, r4, 0x08000016@l
/* 802FC710 002F9650  38 A0 00 00 */	li r5, 0
/* 802FC714 002F9654  90 08 00 14 */	stw r0, 0x14(r8)
/* 802FC718 002F9658  38 C0 00 00 */	li r6, 0
/* 802FC71C 002F965C  90 08 00 18 */	stw r0, 0x18(r8)
/* 802FC720 002F9660  90 E8 00 1C */	stw r7, 0x1c(r8)
/* 802FC724 002F9664  C0 22 F6 30 */	lfs f1, $$22378-_SDA2_BASE_(r2)
/* 802FC728 002F9668  FC 40 08 90 */	fmr f2, f1
/* 802FC72C 002F966C  FC 60 08 90 */	fmr f3, f1
/* 802FC730 002F9670  FC 80 08 90 */	fmr f4, f1
/* 802FC734 002F9674  4B E2 10 C5 */	bl initHitActor__9THitActorFUlUsiffff
/* 802FC738 002F9678  80 1F 00 64 */	lwz r0, 0x64(r31)
/* 802FC73C 002F967C  38 7F 00 00 */	addi r3, r31, 0
/* 802FC740 002F9680  60 00 00 01 */	ori r0, r0, 1
/* 802FC744 002F9684  90 1F 00 64 */	stw r0, 0x64(r31)
/* 802FC748 002F9688  4B E1 4B 41 */	bl initAnmSound__10TLiveActorFv
/* 802FC74C 002F968C  80 7F 00 74 */	lwz r3, 0x74(r31)
/* 802FC750 002F9690  38 80 00 01 */	li r4, 1
/* 802FC754 002F9694  4B DD 79 CD */	bl setBckFromIndex__6MActorFi
/* 802FC758 002F9698  7F E3 FB 78 */	mr r3, r31
/* 802FC75C 002F969C  4B E1 49 2D */	bl setCurAnmSound__10TLiveActorFv
/* 802FC760 002F96A0  C3 5F 00 38 */	lfs f26, 0x38(r31)
/* 802FC764 002F96A4  7F E3 FB 78 */	mr r3, r31
/* 802FC768 002F96A8  C3 7F 00 34 */	lfs f27, 0x34(r31)
/* 802FC76C 002F96AC  C3 9F 00 30 */	lfs f28, 0x30(r31)
/* 802FC770 002F96B0  C3 BF 00 18 */	lfs f29, 0x18(r31)
/* 802FC774 002F96B4  C3 DF 00 14 */	lfs f30, 0x14(r31)
/* 802FC778 002F96B8  C3 FF 00 10 */	lfs f31, 0x10(r31)
/* 802FC77C 002F96BC  4B E1 5A F5 */	bl getModel__10TLiveActorCFv
/* 802FC780 002F96C0  C0 02 F6 34 */	lfs f0, $$22379-_SDA2_BASE_(r2)
/* 802FC784 002F96C4  FC 20 F8 90 */	fmr f1, f31
/* 802FC788 002F96C8  38 63 00 20 */	addi r3, r3, 0x20
/* 802FC78C 002F96CC  EC 60 07 32 */	fmuls f3, f0, f28
/* 802FC790 002F96D0  EC 40 06 F2 */	fmuls f2, f0, f27
/* 802FC794 002F96D4  EC 00 06 B2 */	fmuls f0, f0, f26
/* 802FC798 002F96D8  FC 60 18 1E */	fctiwz f3, f3
/* 802FC79C 002F96DC  FC 80 10 1E */	fctiwz f4, f2
/* 802FC7A0 002F96E0  FC 00 00 1E */	fctiwz f0, f0
/* 802FC7A4 002F96E4  D8 61 00 38 */	stfd f3, 0x38(r1)
/* 802FC7A8 002F96E8  FC 40 F0 90 */	fmr f2, f30
/* 802FC7AC 002F96EC  FC 60 E8 90 */	fmr f3, f29
/* 802FC7B0 002F96F0  D8 81 00 30 */	stfd f4, 0x30(r1)
/* 802FC7B4 002F96F4  80 81 00 3C */	lwz r4, 0x3c(r1)
/* 802FC7B8 002F96F8  D8 01 00 28 */	stfd f0, 0x28(r1)
/* 802FC7BC 002F96FC  80 A1 00 34 */	lwz r5, 0x34(r1)
/* 802FC7C0 002F9700  80 C1 00 2C */	lwz r6, 0x2c(r1)
/* 802FC7C4 002F9704  4B DC 96 25 */	bl MsMtxSetXYZRPH__FPA4_ffffsss
/* 802FC7C8 002F9708  38 60 00 1C */	li r3, 0x1c
/* 802FC7CC 002F970C  4B D1 00 E5 */	bl __nw__FUl
/* 802FC7D0 002F9710  7C 7E 1B 79 */	or. r30, r3, r3
/* 802FC7D4 002F9714  41 82 00 14 */	beq lbl_802FC7E8
/* 802FC7D8 002F9718  3C 60 80 3A */	lis r3, $$22380@ha
/* 802FC7DC 002F971C  38 83 F8 FC */	addi r4, r3, $$22380@l
/* 802FC7E0 002F9720  38 7E 00 00 */	addi r3, r30, 0
/* 802FC7E4 002F9724  4B E2 16 91 */	bl __ct__12TMirrorActorFPCc
lbl_802FC7E8:
/* 802FC7E8 002F9728  93 DF 01 5C */	stw r30, 0x15c(r31)
/* 802FC7EC 002F972C  7F E3 FB 78 */	mr r3, r31
/* 802FC7F0 002F9730  4B E1 5A 81 */	bl getModel__10TLiveActorCFv
/* 802FC7F4 002F9734  7C 64 1B 78 */	mr r4, r3
/* 802FC7F8 002F9738  80 7F 01 5C */	lwz r3, 0x15c(r31)
/* 802FC7FC 002F973C  38 A0 00 0A */	li r5, 0xa
/* 802FC800 002F9740  4B E2 16 ED */	bl init__12TMirrorActorFP8J3DModelUs
/* 802FC804 002F9744  80 01 00 7C */	lwz r0, 0x7c(r1)
/* 802FC808 002F9748  CB E1 00 70 */	lfd f31, 0x70(r1)
/* 802FC80C 002F974C  CB C1 00 68 */	lfd f30, 0x68(r1)
/* 802FC810 002F9750  7C 08 03 A6 */	mtlr r0
/* 802FC814 002F9754  CB A1 00 60 */	lfd f29, 0x60(r1)
/* 802FC818 002F9758  CB 81 00 58 */	lfd f28, 0x58(r1)
/* 802FC81C 002F975C  CB 61 00 50 */	lfd f27, 0x50(r1)
/* 802FC820 002F9760  CB 41 00 48 */	lfd f26, 0x48(r1)
/* 802FC824 002F9764  83 E1 00 44 */	lwz r31, 0x44(r1)
/* 802FC828 002F9768  83 C1 00 40 */	lwz r30, 0x40(r1)
/* 802FC82C 002F976C  38 21 00 78 */	addi r1, r1, 0x78
/* 802FC830 002F9770  4E 80 00 20 */	blr 

.global createModelData__25TSleepBossHanachanManagerFv
createModelData__25TSleepBossHanachanManagerFv:
/* 802FC834 002F9774  7C 08 02 A6 */	mflr r0
/* 802FC838 002F9778  3C 80 80 3A */	lis r4, entry$2260@ha
/* 802FC83C 002F977C  90 01 00 04 */	stw r0, 4(r1)
/* 802FC840 002F9780  38 84 F8 BC */	addi r4, r4, entry$2260@l
/* 802FC844 002F9784  94 21 FF F8 */	stwu r1, -8(r1)
/* 802FC848 002F9788  81 83 00 00 */	lwz r12, 0(r3)
/* 802FC84C 002F978C  81 8C 00 2C */	lwz r12, 0x2c(r12)
/* 802FC850 002F9790  7D 88 03 A6 */	mtlr r12
/* 802FC854 002F9794  4E 80 00 21 */	blrl 
/* 802FC858 002F9798  80 01 00 0C */	lwz r0, 0xc(r1)
/* 802FC85C 002F979C  38 21 00 08 */	addi r1, r1, 8
/* 802FC860 002F97A0  7C 08 03 A6 */	mtlr r0
/* 802FC864 002F97A4  4E 80 00 20 */	blr 

.global __dt__18TSleepBossHanachanFv
__dt__18TSleepBossHanachanFv:
/* 802FC868 002F97A8  7C 08 02 A6 */	mflr r0
/* 802FC86C 002F97AC  90 01 00 04 */	stw r0, 4(r1)
/* 802FC870 002F97B0  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 802FC874 002F97B4  93 E1 00 14 */	stw r31, 0x14(r1)
/* 802FC878 002F97B8  3B E4 00 00 */	addi r31, r4, 0
/* 802FC87C 002F97BC  93 C1 00 10 */	stw r30, 0x10(r1)
/* 802FC880 002F97C0  7C 7E 1B 79 */	or. r30, r3, r3
/* 802FC884 002F97C4  41 82 00 4C */	beq lbl_802FC8D0
/* 802FC888 002F97C8  3C 60 80 3E */	lis r3, __vt__18TSleepBossHanachan@ha
/* 802FC88C 002F97CC  38 63 EF 08 */	addi r3, r3, __vt__18TSleepBossHanachan@l
/* 802FC890 002F97D0  90 7E 00 00 */	stw r3, 0(r30)
/* 802FC894 002F97D4  38 03 00 24 */	addi r0, r3, 0x24
/* 802FC898 002F97D8  90 1E 00 20 */	stw r0, 0x20(r30)
/* 802FC89C 002F97DC  41 82 00 24 */	beq lbl_802FC8C0
/* 802FC8A0 002F97E0  3C 60 80 3E */	lis r3, __vt__17TDemoBossHanachan@ha
/* 802FC8A4 002F97E4  38 63 F0 70 */	addi r3, r3, __vt__17TDemoBossHanachan@l
/* 802FC8A8 002F97E8  90 7E 00 00 */	stw r3, 0(r30)
/* 802FC8AC 002F97EC  38 03 00 24 */	addi r0, r3, 0x24
/* 802FC8B0 002F97F0  38 7E 00 00 */	addi r3, r30, 0
/* 802FC8B4 002F97F4  90 1E 00 20 */	stw r0, 0x20(r30)
/* 802FC8B8 002F97F8  38 80 00 00 */	li r4, 0
/* 802FC8BC 002F97FC  4B F5 38 FD */	bl __dt__11TSpineEnemyFv
lbl_802FC8C0:
/* 802FC8C0 002F9800  7F E0 07 35 */	extsh. r0, r31
/* 802FC8C4 002F9804  40 81 00 0C */	ble lbl_802FC8D0
/* 802FC8C8 002F9808  7F C3 F3 78 */	mr r3, r30
/* 802FC8CC 002F980C  4B D1 01 E5 */	bl __dl__FPv
lbl_802FC8D0:
/* 802FC8D0 002F9810  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 802FC8D4 002F9814  7F C3 F3 78 */	mr r3, r30
/* 802FC8D8 002F9818  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 802FC8DC 002F981C  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 802FC8E0 002F9820  7C 08 03 A6 */	mtlr r0
/* 802FC8E4 002F9824  38 21 00 18 */	addi r1, r1, 0x18
/* 802FC8E8 002F9828  4E 80 00 20 */	blr 

.global __dt__25TSleepBossHanachanManagerFv
__dt__25TSleepBossHanachanManagerFv:
/* 802FC8EC 002F982C  7C 08 02 A6 */	mflr r0
/* 802FC8F0 002F9830  90 01 00 04 */	stw r0, 4(r1)
/* 802FC8F4 002F9834  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 802FC8F8 002F9838  93 E1 00 14 */	stw r31, 0x14(r1)
/* 802FC8FC 002F983C  3B E4 00 00 */	addi r31, r4, 0
/* 802FC900 002F9840  93 C1 00 10 */	stw r30, 0x10(r1)
/* 802FC904 002F9844  7C 7E 1B 79 */	or. r30, r3, r3
/* 802FC908 002F9848  41 82 00 3C */	beq lbl_802FC944
/* 802FC90C 002F984C  3C 60 80 3E */	lis r3, __vt__25TSleepBossHanachanManager@ha
/* 802FC910 002F9850  38 03 F0 1C */	addi r0, r3, __vt__25TSleepBossHanachanManager@l
/* 802FC914 002F9854  90 1E 00 00 */	stw r0, 0(r30)
/* 802FC918 002F9858  41 82 00 1C */	beq lbl_802FC934
/* 802FC91C 002F985C  3C 60 80 3E */	lis r3, __vt__24TDemoBossHanachanManager@ha
/* 802FC920 002F9860  38 03 F1 84 */	addi r0, r3, __vt__24TDemoBossHanachanManager@l
/* 802FC924 002F9864  90 1E 00 00 */	stw r0, 0(r30)
/* 802FC928 002F9868  38 7E 00 00 */	addi r3, r30, 0
/* 802FC92C 002F986C  38 80 00 00 */	li r4, 0
/* 802FC930 002F9870  4B F5 60 79 */	bl __dt__13TEnemyManagerFv
lbl_802FC934:
/* 802FC934 002F9874  7F E0 07 35 */	extsh. r0, r31
/* 802FC938 002F9878  40 81 00 0C */	ble lbl_802FC944
/* 802FC93C 002F987C  7F C3 F3 78 */	mr r3, r30
/* 802FC940 002F9880  4B D1 01 71 */	bl __dl__FPv
lbl_802FC944:
/* 802FC944 002F9884  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 802FC948 002F9888  7F C3 F3 78 */	mr r3, r30
/* 802FC94C 002F988C  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 802FC950 002F9890  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 802FC954 002F9894  7C 08 03 A6 */	mtlr r0
/* 802FC958 002F9898  38 21 00 18 */	addi r1, r1, 0x18
/* 802FC95C 002F989C  4E 80 00 20 */	blr 

.global $$232$$2__dt__18TSleepBossHanachanFv
$$232$$2__dt__18TSleepBossHanachanFv:
/* 802FC960 002F98A0  38 63 FF E0 */	addi r3, r3, -32
/* 802FC964 002F98A4  4B FF FF 04 */	b __dt__18TSleepBossHanachanFv

.section .rodata, "wa"  # 0x8036FFA0 - 0x803A8380
.global $$21490
$$21490:
	.incbin "baserom.dol", 0x39C790, 0xC
.global $$21700
$$21700:
	.incbin "baserom.dol", 0x39C79C, 0x14
.global $$21797
$$21797:
	.incbin "baserom.dol", 0x39C7B0, 0x30
.global $$21798
$$21798:
	.incbin "baserom.dol", 0x39C7E0, 0x38
.global $$21799
$$21799:
	.incbin "baserom.dol", 0x39C818, 0x34
.global $$21800
$$21800:
	.incbin "baserom.dol", 0x39C84C, 0x24
.global $$22258
$$22258:
	.incbin "baserom.dol", 0x39C870, 0x34
.global $$22261
$$22261:
	.incbin "baserom.dol", 0x39C8A4, 0x18
.global entry$2260
entry$2260:
	.incbin "baserom.dol", 0x39C8BC, 0x18
.global $$22311
$$22311:
	.incbin "baserom.dol", 0x39C8D4, 0x14
.global $$22312
$$22312:
	.incbin "baserom.dol", 0x39C8E8, 0x14
.global $$22380
$$22380:
	.incbin "baserom.dol", 0x39C8FC, 0x1C

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global __vt__14TNerveSBH_Fall
__vt__14TNerveSBH_Fall:
	.incbin "baserom.dol", 0x3DBEE8, 0x10
.global __vt__23TNerveSBH_SleepContinue
__vt__23TNerveSBH_SleepContinue:
	.incbin "baserom.dol", 0x3DBEF8, 0x10
.global __vt__18TSleepBossHanachan
__vt__18TSleepBossHanachan:
	.incbin "baserom.dol", 0x3DBF08, 0x114
.global __vt__25TSleepBossHanachanManager
__vt__25TSleepBossHanachanManager:
	.incbin "baserom.dol", 0x3DC01C, 0x54

.section .sdata, "wa"  # 0x80408AC0 - 0x804097C0
.global sleepBossHanachan_bastable
sleepBossHanachan_bastable:
	.incbin "baserom.dol", 0x3E3A88, 0x8

.section .sdata2, "wa"  # 0x8040B460 - 0x80414020
.global $$22378
$$22378:
	.incbin "baserom.dol", 0x3EB330, 0x4
.global $$22379
$$22379:
	.incbin "baserom.dol", 0x3EB334, 0x4

.section .bss, "wa"  # 0x803E6000 - 0x80408AC0
.global $$22265
$$22265:
	.skip 0xC
.global $$22271
$$22271:
	.skip 0xC

.section .sbss, "wa"  # 0x804097C0 - 0x8040B45C
.global init$2266
init$2266:
	.skip 0x4
.global instance$2264
instance$2264:
	.skip 0x4
.global init$2272
init$2272:
	.skip 0x4
.global instance$2270
instance$2270:
	.skip 0x4
