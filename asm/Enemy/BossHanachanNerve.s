.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0

.global __dt__22TNerveBossHanachanDeadFv
__dt__22TNerveBossHanachanDeadFv:
/* 80303070 002FFFB0  7C 08 02 A6 */	mflr r0
/* 80303074 002FFFB4  90 01 00 04 */	stw r0, 4(r1)
/* 80303078 002FFFB8  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 8030307C 002FFFBC  93 E1 00 14 */	stw r31, 0x14(r1)
/* 80303080 002FFFC0  7C 7F 1B 79 */	or. r31, r3, r3
/* 80303084 002FFFC4  41 82 00 30 */	beq lbl_803030B4
/* 80303088 002FFFC8  3C 60 80 3E */	lis r3, __vt__22TNerveBossHanachanDead@ha
/* 8030308C 002FFFCC  38 03 F5 B0 */	addi r0, r3, __vt__22TNerveBossHanachanDead@l
/* 80303090 002FFFD0  90 1F 00 00 */	stw r0, 0(r31)
/* 80303094 002FFFD4  41 82 00 10 */	beq lbl_803030A4
/* 80303098 002FFFD8  3C 60 80 3C */	lis r3, __vt__24TNerveBase$$010TLiveActor$$1@ha
/* 8030309C 002FFFDC  38 03 92 60 */	addi r0, r3, __vt__24TNerveBase$$010TLiveActor$$1@l
/* 803030A0 002FFFE0  90 1F 00 00 */	stw r0, 0(r31)
lbl_803030A4:
/* 803030A4 002FFFE4  7C 80 07 35 */	extsh. r0, r4
/* 803030A8 002FFFE8  40 81 00 0C */	ble lbl_803030B4
/* 803030AC 002FFFEC  7F E3 FB 78 */	mr r3, r31
/* 803030B0 002FFFF0  4B D0 9A 01 */	bl __dl__FPv
lbl_803030B4:
/* 803030B4 002FFFF4  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 803030B8 002FFFF8  7F E3 FB 78 */	mr r3, r31
/* 803030BC 002FFFFC  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 803030C0 00300000  38 21 00 18 */	addi r1, r1, 0x18
/* 803030C4 00300004  7C 08 03 A6 */	mtlr r0
/* 803030C8 00300008  4E 80 00 20 */	blr 

.global execute__22TNerveBossHanachanDeadCFP24TSpineBase$$010TLiveActor$$1
execute__22TNerveBossHanachanDeadCFP24TSpineBase$$010TLiveActor$$1:
/* 803030CC 0030000C  7C 08 02 A6 */	mflr r0
/* 803030D0 00300010  90 01 00 04 */	stw r0, 4(r1)
/* 803030D4 00300014  94 21 FF E0 */	stwu r1, -0x20(r1)
/* 803030D8 00300018  93 E1 00 1C */	stw r31, 0x1c(r1)
/* 803030DC 0030001C  83 E4 00 00 */	lwz r31, 0(r4)
/* 803030E0 00300020  38 80 00 05 */	li r4, 5
/* 803030E4 00300024  38 7F 00 00 */	addi r3, r31, 0
/* 803030E8 00300028  48 00 15 D5 */	bl considerSetAnm__13TBossHanachanF24EnumBossHanachanNerveAnm
/* 803030EC 0030002C  80 1F 00 F0 */	lwz r0, 0xf0(r31)
/* 803030F0 00300030  54 00 03 5B */	rlwinm. r0, r0, 0, 0xd, 0xd
/* 803030F4 00300034  40 82 00 2C */	bne lbl_80303120
/* 803030F8 00300038  38 7F 00 00 */	addi r3, r31, 0
/* 803030FC 0030003C  38 80 00 0F */	li r4, 0xf
/* 80303100 00300040  48 00 14 A9 */	bl isAllBckAlreadyEnd__13TBossHanachanCF23EnumBossHanachanAnmKind
/* 80303104 00300044  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 80303108 00300048  41 82 00 18 */	beq lbl_80303120
/* 8030310C 0030004C  80 1F 00 F0 */	lwz r0, 0xf0(r31)
/* 80303110 00300050  38 7F 00 00 */	addi r3, r31, 0
/* 80303114 00300054  64 00 00 04 */	oris r0, r0, 4
/* 80303118 00300058  90 1F 00 F0 */	stw r0, 0xf0(r31)
/* 8030311C 0030005C  4B FF C6 4D */	bl removeAllMapCollision__13TBossHanachanFv
lbl_80303120:
/* 80303120 00300060  80 01 00 24 */	lwz r0, 0x24(r1)
/* 80303124 00300064  38 60 00 00 */	li r3, 0
/* 80303128 00300068  83 E1 00 1C */	lwz r31, 0x1c(r1)
/* 8030312C 0030006C  38 21 00 20 */	addi r1, r1, 0x20
/* 80303130 00300070  7C 08 03 A6 */	mtlr r0
/* 80303134 00300074  4E 80 00 20 */	blr 

.global theNerve__22TNerveBossHanachanDeadFv
theNerve__22TNerveBossHanachanDeadFv:
/* 80303138 00300078  7C 08 02 A6 */	mflr r0
/* 8030313C 0030007C  90 01 00 04 */	stw r0, 4(r1)
/* 80303140 00300080  94 21 FF F8 */	stwu r1, -8(r1)
/* 80303144 00300084  88 0D A4 F0 */	lbz r0, init$2425-_SDA_BASE_(r13)
/* 80303148 00300088  7C 00 07 75 */	extsb. r0, r0
/* 8030314C 0030008C  40 82 00 3C */	bne lbl_80303188
/* 80303150 00300090  3C 60 80 3C */	lis r3, __vt__24TNerveBase$$010TLiveActor$$1@ha
/* 80303154 00300094  38 03 92 60 */	addi r0, r3, __vt__24TNerveBase$$010TLiveActor$$1@l
/* 80303158 00300098  3C 60 80 3E */	lis r3, __vt__22TNerveBossHanachanDead@ha
/* 8030315C 0030009C  90 0D A4 F4 */	stw r0, instance$2423-_SDA_BASE_(r13)
/* 80303160 003000A0  38 03 F5 B0 */	addi r0, r3, __vt__22TNerveBossHanachanDead@l
/* 80303164 003000A4  3C 80 80 30 */	lis r4, __dt__22TNerveBossHanachanDeadFv@ha
/* 80303168 003000A8  90 0D A4 F4 */	stw r0, instance$2423-_SDA_BASE_(r13)
/* 8030316C 003000AC  3C 60 80 40 */	lis r3, $$22424@ha
/* 80303170 003000B0  38 A3 EE 20 */	addi r5, r3, $$22424@l
/* 80303174 003000B4  38 84 30 70 */	addi r4, r4, __dt__22TNerveBossHanachanDeadFv@l
/* 80303178 003000B8  38 6D A4 F4 */	addi r3, r13, instance$2423-_SDA_BASE_
/* 8030317C 003000BC  4B D7 F5 AD */	bl __register_global_object
/* 80303180 003000C0  38 00 00 01 */	li r0, 1
/* 80303184 003000C4  98 0D A4 F0 */	stb r0, init$2425-_SDA_BASE_(r13)
lbl_80303188:
/* 80303188 003000C8  80 01 00 0C */	lwz r0, 0xc(r1)
/* 8030318C 003000CC  38 6D A4 F4 */	addi r3, r13, instance$2423-_SDA_BASE_
/* 80303190 003000D0  38 21 00 08 */	addi r1, r1, 8
/* 80303194 003000D4  7C 08 03 A6 */	mtlr r0
/* 80303198 003000D8  4E 80 00 20 */	blr 

.global execute__23TNerveBossHanachanSnortCFP24TSpineBase$$010TLiveActor$$1
execute__23TNerveBossHanachanSnortCFP24TSpineBase$$010TLiveActor$$1:
/* 8030319C 003000DC  7C 08 02 A6 */	mflr r0
/* 803031A0 003000E0  90 01 00 04 */	stw r0, 4(r1)
/* 803031A4 003000E4  94 21 FF C0 */	stwu r1, -0x40(r1)
/* 803031A8 003000E8  93 E1 00 3C */	stw r31, 0x3c(r1)
/* 803031AC 003000EC  7C 9F 23 78 */	mr r31, r4
/* 803031B0 003000F0  93 C1 00 38 */	stw r30, 0x38(r1)
/* 803031B4 003000F4  80 04 00 20 */	lwz r0, 0x20(r4)
/* 803031B8 003000F8  83 C4 00 00 */	lwz r30, 0(r4)
/* 803031BC 003000FC  2C 00 00 C8 */	cmpwi r0, 0xc8
/* 803031C0 00300100  40 82 00 70 */	bne lbl_80303230
/* 803031C4 00300104  80 1E 00 F0 */	lwz r0, 0xf0(r30)
/* 803031C8 00300108  54 00 03 9D */	rlwinm. r0, r0, 0, 0xe, 0xe
/* 803031CC 0030010C  41 82 00 64 */	beq lbl_80303230
/* 803031D0 00300110  80 1E 00 F0 */	lwz r0, 0xf0(r30)
/* 803031D4 00300114  3C 60 80 01 */	lis r3, 0x80010029@ha
/* 803031D8 00300118  38 63 00 29 */	addi r3, r3, 0x80010029@l
/* 803031DC 0030011C  54 00 03 DA */	rlwinm r0, r0, 0, 0xf, 0xd
/* 803031E0 00300120  90 1E 00 F0 */	stw r0, 0xf0(r30)
/* 803031E4 00300124  4B E7 CC B9 */	bl startBGM__5MSBgmFUl
/* 803031E8 00300128  88 1E 01 3C */	lbz r0, 0x13c(r30)
/* 803031EC 0030012C  2C 00 00 02 */	cmpwi r0, 2
/* 803031F0 00300130  41 82 00 14 */	beq lbl_80303204
/* 803031F4 00300134  40 80 00 3C */	bge lbl_80303230
/* 803031F8 00300138  2C 00 00 01 */	cmpwi r0, 1
/* 803031FC 0030013C  40 80 00 20 */	bge lbl_8030321C
/* 80303200 00300140  48 00 00 30 */	b lbl_80303230
lbl_80303204:
/* 80303204 00300144  80 6D 97 EC */	lwz r3, gpMSound-_SDA_BASE_(r13)
/* 80303208 00300148  38 80 00 00 */	li r4, 0
/* 8030320C 0030014C  38 A0 00 01 */	li r5, 1
/* 80303210 00300150  80 63 00 98 */	lwz r3, 0x98(r3)
/* 80303214 00300154  4B E8 38 C9 */	bl changeTempo__8MSModBgmFUcUc
/* 80303218 00300158  48 00 00 18 */	b lbl_80303230
lbl_8030321C:
/* 8030321C 0030015C  80 6D 97 EC */	lwz r3, gpMSound-_SDA_BASE_(r13)
/* 80303220 00300160  38 80 00 01 */	li r4, 1
/* 80303224 00300164  38 A0 00 01 */	li r5, 1
/* 80303228 00300168  80 63 00 98 */	lwz r3, 0x98(r3)
/* 8030322C 0030016C  4B E8 38 B1 */	bl changeTempo__8MSModBgmFUcUc
lbl_80303230:
/* 80303230 00300170  38 7E 00 00 */	addi r3, r30, 0
/* 80303234 00300174  38 80 00 04 */	li r4, 4
/* 80303238 00300178  48 00 14 85 */	bl considerSetAnm__13TBossHanachanF24EnumBossHanachanNerveAnm
/* 8030323C 0030017C  38 7E 00 00 */	addi r3, r30, 0
/* 80303240 00300180  38 80 00 0E */	li r4, 0xe
/* 80303244 00300184  48 00 13 65 */	bl isAllBckAlreadyEnd__13TBossHanachanCF23EnumBossHanachanAnmKind
/* 80303248 00300188  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 8030324C 0030018C  41 82 00 8C */	beq lbl_803032D8
/* 80303250 00300190  7F C3 F3 78 */	mr r3, r30
/* 80303254 00300194  4B FF C8 79 */	bl goToInitialRecoverGraphNode__13TBossHanachanFv
/* 80303258 00300198  88 0D A4 C0 */	lbz r0, init$2361-_SDA_BASE_(r13)
/* 8030325C 0030019C  7C 00 07 75 */	extsb. r0, r0
/* 80303260 003001A0  40 82 00 3C */	bne lbl_8030329C
/* 80303264 003001A4  3C 60 80 3C */	lis r3, __vt__24TNerveBase$$010TLiveActor$$1@ha
/* 80303268 003001A8  38 03 92 60 */	addi r0, r3, __vt__24TNerveBase$$010TLiveActor$$1@l
/* 8030326C 003001AC  3C 60 80 3E */	lis r3, __vt__29TNerveBossHanachanGraphWander@ha
/* 80303270 003001B0  90 0D A4 C4 */	stw r0, instance$2359-_SDA_BASE_(r13)
/* 80303274 003001B4  38 03 F6 10 */	addi r0, r3, __vt__29TNerveBossHanachanGraphWander@l
/* 80303278 003001B8  3C 80 80 30 */	lis r4, __dt__29TNerveBossHanachanGraphWanderFv@ha
/* 8030327C 003001BC  90 0D A4 C4 */	stw r0, instance$2359-_SDA_BASE_(r13)
/* 80303280 003001C0  3C 60 80 40 */	lis r3, $$22360@ha
/* 80303284 003001C4  38 A3 ED D8 */	addi r5, r3, $$22360@l
/* 80303288 003001C8  38 84 32 F4 */	addi r4, r4, __dt__29TNerveBossHanachanGraphWanderFv@l
/* 8030328C 003001CC  38 6D A4 C4 */	addi r3, r13, instance$2359-_SDA_BASE_
/* 80303290 003001D0  4B D7 F4 99 */	bl __register_global_object
/* 80303294 003001D4  38 00 00 01 */	li r0, 1
/* 80303298 003001D8  98 0D A4 C0 */	stb r0, init$2361-_SDA_BASE_(r13)
lbl_8030329C:
/* 8030329C 003001DC  38 8D A4 C4 */	addi r4, r13, instance$2359-_SDA_BASE_
/* 803032A0 003001E0  28 04 00 00 */	cmplwi r4, 0
/* 803032A4 003001E4  41 82 00 2C */	beq lbl_803032D0
/* 803032A8 003001E8  80 BF 00 08 */	lwz r5, 8(r31)
/* 803032AC 003001EC  80 1F 00 04 */	lwz r0, 4(r31)
/* 803032B0 003001F0  7C 05 00 00 */	cmpw r5, r0
/* 803032B4 003001F4  40 80 00 1C */	bge lbl_803032D0
/* 803032B8 003001F8  80 7F 00 0C */	lwz r3, 0xc(r31)
/* 803032BC 003001FC  54 A0 10 3A */	slwi r0, r5, 2
/* 803032C0 00300200  7C 83 01 2E */	stwx r4, r3, r0
/* 803032C4 00300204  80 7F 00 08 */	lwz r3, 8(r31)
/* 803032C8 00300208  38 03 00 01 */	addi r0, r3, 1
/* 803032CC 0030020C  90 1F 00 08 */	stw r0, 8(r31)
lbl_803032D0:
/* 803032D0 00300210  38 60 00 01 */	li r3, 1
/* 803032D4 00300214  48 00 00 08 */	b lbl_803032DC
lbl_803032D8:
/* 803032D8 00300218  38 60 00 00 */	li r3, 0
lbl_803032DC:
/* 803032DC 0030021C  80 01 00 44 */	lwz r0, 0x44(r1)
/* 803032E0 00300220  83 E1 00 3C */	lwz r31, 0x3c(r1)
/* 803032E4 00300224  83 C1 00 38 */	lwz r30, 0x38(r1)
/* 803032E8 00300228  7C 08 03 A6 */	mtlr r0
/* 803032EC 0030022C  38 21 00 40 */	addi r1, r1, 0x40
/* 803032F0 00300230  4E 80 00 20 */	blr 

.global __dt__29TNerveBossHanachanGraphWanderFv
__dt__29TNerveBossHanachanGraphWanderFv:
/* 803032F4 00300234  7C 08 02 A6 */	mflr r0
/* 803032F8 00300238  90 01 00 04 */	stw r0, 4(r1)
/* 803032FC 0030023C  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 80303300 00300240  93 E1 00 14 */	stw r31, 0x14(r1)
/* 80303304 00300244  7C 7F 1B 79 */	or. r31, r3, r3
/* 80303308 00300248  41 82 00 30 */	beq lbl_80303338
/* 8030330C 0030024C  3C 60 80 3E */	lis r3, __vt__29TNerveBossHanachanGraphWander@ha
/* 80303310 00300250  38 03 F6 10 */	addi r0, r3, __vt__29TNerveBossHanachanGraphWander@l
/* 80303314 00300254  90 1F 00 00 */	stw r0, 0(r31)
/* 80303318 00300258  41 82 00 10 */	beq lbl_80303328
/* 8030331C 0030025C  3C 60 80 3C */	lis r3, __vt__24TNerveBase$$010TLiveActor$$1@ha
/* 80303320 00300260  38 03 92 60 */	addi r0, r3, __vt__24TNerveBase$$010TLiveActor$$1@l
/* 80303324 00300264  90 1F 00 00 */	stw r0, 0(r31)
lbl_80303328:
/* 80303328 00300268  7C 80 07 35 */	extsh. r0, r4
/* 8030332C 0030026C  40 81 00 0C */	ble lbl_80303338
/* 80303330 00300270  7F E3 FB 78 */	mr r3, r31
/* 80303334 00300274  4B D0 97 7D */	bl __dl__FPv
lbl_80303338:
/* 80303338 00300278  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 8030333C 0030027C  7F E3 FB 78 */	mr r3, r31
/* 80303340 00300280  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 80303344 00300284  38 21 00 18 */	addi r1, r1, 0x18
/* 80303348 00300288  7C 08 03 A6 */	mtlr r0
/* 8030334C 0030028C  4E 80 00 20 */	blr 

.global theNerve__23TNerveBossHanachanSnortFv
theNerve__23TNerveBossHanachanSnortFv:
/* 80303350 00300290  7C 08 02 A6 */	mflr r0
/* 80303354 00300294  90 01 00 04 */	stw r0, 4(r1)
/* 80303358 00300298  94 21 FF F8 */	stwu r1, -8(r1)
/* 8030335C 0030029C  88 0D A4 E8 */	lbz r0, init$2410-_SDA_BASE_(r13)
/* 80303360 003002A0  7C 00 07 75 */	extsb. r0, r0
/* 80303364 003002A4  40 82 00 3C */	bne lbl_803033A0
/* 80303368 003002A8  3C 60 80 3C */	lis r3, __vt__24TNerveBase$$010TLiveActor$$1@ha
/* 8030336C 003002AC  38 03 92 60 */	addi r0, r3, __vt__24TNerveBase$$010TLiveActor$$1@l
/* 80303370 003002B0  3C 60 80 3E */	lis r3, __vt__23TNerveBossHanachanSnort@ha
/* 80303374 003002B4  90 0D A4 EC */	stw r0, instance$2408-_SDA_BASE_(r13)
/* 80303378 003002B8  38 03 F5 C0 */	addi r0, r3, __vt__23TNerveBossHanachanSnort@l
/* 8030337C 003002BC  3C 80 80 30 */	lis r4, __dt__23TNerveBossHanachanSnortFv@ha
/* 80303380 003002C0  90 0D A4 EC */	stw r0, instance$2408-_SDA_BASE_(r13)
/* 80303384 003002C4  3C 60 80 40 */	lis r3, $$22409@ha
/* 80303388 003002C8  38 A3 EE 14 */	addi r5, r3, $$22409@l
/* 8030338C 003002CC  38 84 33 B4 */	addi r4, r4, __dt__23TNerveBossHanachanSnortFv@l
/* 80303390 003002D0  38 6D A4 EC */	addi r3, r13, instance$2408-_SDA_BASE_
/* 80303394 003002D4  4B D7 F3 95 */	bl __register_global_object
/* 80303398 003002D8  38 00 00 01 */	li r0, 1
/* 8030339C 003002DC  98 0D A4 E8 */	stb r0, init$2410-_SDA_BASE_(r13)
lbl_803033A0:
/* 803033A0 003002E0  80 01 00 0C */	lwz r0, 0xc(r1)
/* 803033A4 003002E4  38 6D A4 EC */	addi r3, r13, instance$2408-_SDA_BASE_
/* 803033A8 003002E8  38 21 00 08 */	addi r1, r1, 8
/* 803033AC 003002EC  7C 08 03 A6 */	mtlr r0
/* 803033B0 003002F0  4E 80 00 20 */	blr 

.global __dt__23TNerveBossHanachanSnortFv
__dt__23TNerveBossHanachanSnortFv:
/* 803033B4 003002F4  7C 08 02 A6 */	mflr r0
/* 803033B8 003002F8  90 01 00 04 */	stw r0, 4(r1)
/* 803033BC 003002FC  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 803033C0 00300300  93 E1 00 14 */	stw r31, 0x14(r1)
/* 803033C4 00300304  7C 7F 1B 79 */	or. r31, r3, r3
/* 803033C8 00300308  41 82 00 30 */	beq lbl_803033F8
/* 803033CC 0030030C  3C 60 80 3E */	lis r3, __vt__23TNerveBossHanachanSnort@ha
/* 803033D0 00300310  38 03 F5 C0 */	addi r0, r3, __vt__23TNerveBossHanachanSnort@l
/* 803033D4 00300314  90 1F 00 00 */	stw r0, 0(r31)
/* 803033D8 00300318  41 82 00 10 */	beq lbl_803033E8
/* 803033DC 0030031C  3C 60 80 3C */	lis r3, __vt__24TNerveBase$$010TLiveActor$$1@ha
/* 803033E0 00300320  38 03 92 60 */	addi r0, r3, __vt__24TNerveBase$$010TLiveActor$$1@l
/* 803033E4 00300324  90 1F 00 00 */	stw r0, 0(r31)
lbl_803033E8:
/* 803033E8 00300328  7C 80 07 35 */	extsh. r0, r4
/* 803033EC 0030032C  40 81 00 0C */	ble lbl_803033F8
/* 803033F0 00300330  7F E3 FB 78 */	mr r3, r31
/* 803033F4 00300334  4B D0 96 BD */	bl __dl__FPv
lbl_803033F8:
/* 803033F8 00300338  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 803033FC 0030033C  7F E3 FB 78 */	mr r3, r31
/* 80303400 00300340  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 80303404 00300344  38 21 00 18 */	addi r1, r1, 0x18
/* 80303408 00300348  7C 08 03 A6 */	mtlr r0
/* 8030340C 0030034C  4E 80 00 20 */	blr 

.global execute__24TNerveBossHanachanDamageCFP24TSpineBase$$010TLiveActor$$1
execute__24TNerveBossHanachanDamageCFP24TSpineBase$$010TLiveActor$$1:
/* 80303410 00300350  7C 08 02 A6 */	mflr r0
/* 80303414 00300354  90 01 00 04 */	stw r0, 4(r1)
/* 80303418 00300358  94 21 FF C8 */	stwu r1, -0x38(r1)
/* 8030341C 0030035C  93 E1 00 34 */	stw r31, 0x34(r1)
/* 80303420 00300360  7C 9F 23 78 */	mr r31, r4
/* 80303424 00300364  93 C1 00 30 */	stw r30, 0x30(r1)
/* 80303428 00300368  83 C4 00 00 */	lwz r30, 0(r4)
/* 8030342C 0030036C  38 80 00 03 */	li r4, 3
/* 80303430 00300370  38 7E 00 00 */	addi r3, r30, 0
/* 80303434 00300374  48 00 12 89 */	bl considerSetAnm__13TBossHanachanF24EnumBossHanachanNerveAnm
/* 80303438 00300378  7F C3 F3 78 */	mr r3, r30
/* 8030343C 0030037C  4B FF C7 29 */	bl execSlip__13TBossHanachanFv
/* 80303440 00300380  C0 22 F7 C0 */	lfs f1, $$22521-_SDA2_BASE_(r2)
/* 80303444 00300384  C0 1E 01 40 */	lfs f0, 0x140(r30)
/* 80303448 00300388  FC 01 00 00 */	fcmpu cr0, f1, f0
/* 8030344C 0030038C  40 82 00 A0 */	bne lbl_803034EC
/* 80303450 00300390  80 7E 01 C0 */	lwz r3, 0x1c0(r30)
/* 80303454 00300394  80 9F 00 20 */	lwz r4, 0x20(r31)
/* 80303458 00300398  A8 03 01 BC */	lha r0, 0x1bc(r3)
/* 8030345C 0030039C  7C 04 00 00 */	cmpw r4, r0
/* 80303460 003003A0  41 80 00 8C */	blt lbl_803034EC
/* 80303464 003003A4  7F C3 F3 78 */	mr r3, r30
/* 80303468 003003A8  48 00 15 D9 */	bl setAnmTimerWhenGetUp__13TBossHanachanFv
/* 8030346C 003003AC  88 0D A4 D8 */	lbz r0, init$2393-_SDA_BASE_(r13)
/* 80303470 003003B0  7C 00 07 75 */	extsb. r0, r0
/* 80303474 003003B4  40 82 00 3C */	bne lbl_803034B0
/* 80303478 003003B8  3C 60 80 3C */	lis r3, __vt__24TNerveBase$$010TLiveActor$$1@ha
/* 8030347C 003003BC  38 03 92 60 */	addi r0, r3, __vt__24TNerveBase$$010TLiveActor$$1@l
/* 80303480 003003C0  3C 60 80 3E */	lis r3, __vt__23TNerveBossHanachanGetUp@ha
/* 80303484 003003C4  90 0D A4 DC */	stw r0, instance$2391-_SDA_BASE_(r13)
/* 80303488 003003C8  38 03 F5 E0 */	addi r0, r3, __vt__23TNerveBossHanachanGetUp@l
/* 8030348C 003003CC  3C 80 80 30 */	lis r4, __dt__23TNerveBossHanachanGetUpFv@ha
/* 80303490 003003D0  90 0D A4 DC */	stw r0, instance$2391-_SDA_BASE_(r13)
/* 80303494 003003D4  3C 60 80 40 */	lis r3, $$22392@ha
/* 80303498 003003D8  38 A3 ED FC */	addi r5, r3, $$22392@l
/* 8030349C 003003DC  38 84 35 08 */	addi r4, r4, __dt__23TNerveBossHanachanGetUpFv@l
/* 803034A0 003003E0  38 6D A4 DC */	addi r3, r13, instance$2391-_SDA_BASE_
/* 803034A4 003003E4  4B D7 F2 85 */	bl __register_global_object
/* 803034A8 003003E8  38 00 00 01 */	li r0, 1
/* 803034AC 003003EC  98 0D A4 D8 */	stb r0, init$2393-_SDA_BASE_(r13)
lbl_803034B0:
/* 803034B0 003003F0  38 8D A4 DC */	addi r4, r13, instance$2391-_SDA_BASE_
/* 803034B4 003003F4  28 04 00 00 */	cmplwi r4, 0
/* 803034B8 003003F8  41 82 00 2C */	beq lbl_803034E4
/* 803034BC 003003FC  80 BF 00 08 */	lwz r5, 8(r31)
/* 803034C0 00300400  80 1F 00 04 */	lwz r0, 4(r31)
/* 803034C4 00300404  7C 05 00 00 */	cmpw r5, r0
/* 803034C8 00300408  40 80 00 1C */	bge lbl_803034E4
/* 803034CC 0030040C  80 7F 00 0C */	lwz r3, 0xc(r31)
/* 803034D0 00300410  54 A0 10 3A */	slwi r0, r5, 2
/* 803034D4 00300414  7C 83 01 2E */	stwx r4, r3, r0
/* 803034D8 00300418  80 7F 00 08 */	lwz r3, 8(r31)
/* 803034DC 0030041C  38 03 00 01 */	addi r0, r3, 1
/* 803034E0 00300420  90 1F 00 08 */	stw r0, 8(r31)
lbl_803034E4:
/* 803034E4 00300424  38 60 00 01 */	li r3, 1
/* 803034E8 00300428  48 00 00 08 */	b lbl_803034F0
lbl_803034EC:
/* 803034EC 0030042C  38 60 00 00 */	li r3, 0
lbl_803034F0:
/* 803034F0 00300430  80 01 00 3C */	lwz r0, 0x3c(r1)
/* 803034F4 00300434  83 E1 00 34 */	lwz r31, 0x34(r1)
/* 803034F8 00300438  83 C1 00 30 */	lwz r30, 0x30(r1)
/* 803034FC 0030043C  7C 08 03 A6 */	mtlr r0
/* 80303500 00300440  38 21 00 38 */	addi r1, r1, 0x38
/* 80303504 00300444  4E 80 00 20 */	blr 

.global __dt__23TNerveBossHanachanGetUpFv
__dt__23TNerveBossHanachanGetUpFv:
/* 80303508 00300448  7C 08 02 A6 */	mflr r0
/* 8030350C 0030044C  90 01 00 04 */	stw r0, 4(r1)
/* 80303510 00300450  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 80303514 00300454  93 E1 00 14 */	stw r31, 0x14(r1)
/* 80303518 00300458  7C 7F 1B 79 */	or. r31, r3, r3
/* 8030351C 0030045C  41 82 00 30 */	beq lbl_8030354C
/* 80303520 00300460  3C 60 80 3E */	lis r3, __vt__23TNerveBossHanachanGetUp@ha
/* 80303524 00300464  38 03 F5 E0 */	addi r0, r3, __vt__23TNerveBossHanachanGetUp@l
/* 80303528 00300468  90 1F 00 00 */	stw r0, 0(r31)
/* 8030352C 0030046C  41 82 00 10 */	beq lbl_8030353C
/* 80303530 00300470  3C 60 80 3C */	lis r3, __vt__24TNerveBase$$010TLiveActor$$1@ha
/* 80303534 00300474  38 03 92 60 */	addi r0, r3, __vt__24TNerveBase$$010TLiveActor$$1@l
/* 80303538 00300478  90 1F 00 00 */	stw r0, 0(r31)
lbl_8030353C:
/* 8030353C 0030047C  7C 80 07 35 */	extsh. r0, r4
/* 80303540 00300480  40 81 00 0C */	ble lbl_8030354C
/* 80303544 00300484  7F E3 FB 78 */	mr r3, r31
/* 80303548 00300488  4B D0 95 69 */	bl __dl__FPv
lbl_8030354C:
/* 8030354C 0030048C  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 80303550 00300490  7F E3 FB 78 */	mr r3, r31
/* 80303554 00300494  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 80303558 00300498  38 21 00 18 */	addi r1, r1, 0x18
/* 8030355C 0030049C  7C 08 03 A6 */	mtlr r0
/* 80303560 003004A0  4E 80 00 20 */	blr 

.global theNerve__24TNerveBossHanachanDamageFv
theNerve__24TNerveBossHanachanDamageFv:
/* 80303564 003004A4  7C 08 02 A6 */	mflr r0
/* 80303568 003004A8  90 01 00 04 */	stw r0, 4(r1)
/* 8030356C 003004AC  94 21 FF F8 */	stwu r1, -8(r1)
/* 80303570 003004B0  88 0D A4 E0 */	lbz r0, init$2401-_SDA_BASE_(r13)
/* 80303574 003004B4  7C 00 07 75 */	extsb. r0, r0
/* 80303578 003004B8  40 82 00 3C */	bne lbl_803035B4
/* 8030357C 003004BC  3C 60 80 3C */	lis r3, __vt__24TNerveBase$$010TLiveActor$$1@ha
/* 80303580 003004C0  38 03 92 60 */	addi r0, r3, __vt__24TNerveBase$$010TLiveActor$$1@l
/* 80303584 003004C4  3C 60 80 3E */	lis r3, __vt__24TNerveBossHanachanDamage@ha
/* 80303588 003004C8  90 0D A4 E4 */	stw r0, instance$2399-_SDA_BASE_(r13)
/* 8030358C 003004CC  38 03 F5 D0 */	addi r0, r3, __vt__24TNerveBossHanachanDamage@l
/* 80303590 003004D0  3C 80 80 30 */	lis r4, __dt__24TNerveBossHanachanDamageFv@ha
/* 80303594 003004D4  90 0D A4 E4 */	stw r0, instance$2399-_SDA_BASE_(r13)
/* 80303598 003004D8  3C 60 80 40 */	lis r3, $$22400@ha
/* 8030359C 003004DC  38 A3 EE 08 */	addi r5, r3, $$22400@l
/* 803035A0 003004E0  38 84 35 C8 */	addi r4, r4, __dt__24TNerveBossHanachanDamageFv@l
/* 803035A4 003004E4  38 6D A4 E4 */	addi r3, r13, instance$2399-_SDA_BASE_
/* 803035A8 003004E8  4B D7 F1 81 */	bl __register_global_object
/* 803035AC 003004EC  38 00 00 01 */	li r0, 1
/* 803035B0 003004F0  98 0D A4 E0 */	stb r0, init$2401-_SDA_BASE_(r13)
lbl_803035B4:
/* 803035B4 003004F4  80 01 00 0C */	lwz r0, 0xc(r1)
/* 803035B8 003004F8  38 6D A4 E4 */	addi r3, r13, instance$2399-_SDA_BASE_
/* 803035BC 003004FC  38 21 00 08 */	addi r1, r1, 8
/* 803035C0 00300500  7C 08 03 A6 */	mtlr r0
/* 803035C4 00300504  4E 80 00 20 */	blr 

.global __dt__24TNerveBossHanachanDamageFv
__dt__24TNerveBossHanachanDamageFv:
/* 803035C8 00300508  7C 08 02 A6 */	mflr r0
/* 803035CC 0030050C  90 01 00 04 */	stw r0, 4(r1)
/* 803035D0 00300510  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 803035D4 00300514  93 E1 00 14 */	stw r31, 0x14(r1)
/* 803035D8 00300518  7C 7F 1B 79 */	or. r31, r3, r3
/* 803035DC 0030051C  41 82 00 30 */	beq lbl_8030360C
/* 803035E0 00300520  3C 60 80 3E */	lis r3, __vt__24TNerveBossHanachanDamage@ha
/* 803035E4 00300524  38 03 F5 D0 */	addi r0, r3, __vt__24TNerveBossHanachanDamage@l
/* 803035E8 00300528  90 1F 00 00 */	stw r0, 0(r31)
/* 803035EC 0030052C  41 82 00 10 */	beq lbl_803035FC
/* 803035F0 00300530  3C 60 80 3C */	lis r3, __vt__24TNerveBase$$010TLiveActor$$1@ha
/* 803035F4 00300534  38 03 92 60 */	addi r0, r3, __vt__24TNerveBase$$010TLiveActor$$1@l
/* 803035F8 00300538  90 1F 00 00 */	stw r0, 0(r31)
lbl_803035FC:
/* 803035FC 0030053C  7C 80 07 35 */	extsh. r0, r4
/* 80303600 00300540  40 81 00 0C */	ble lbl_8030360C
/* 80303604 00300544  7F E3 FB 78 */	mr r3, r31
/* 80303608 00300548  4B D0 94 A9 */	bl __dl__FPv
lbl_8030360C:
/* 8030360C 0030054C  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 80303610 00300550  7F E3 FB 78 */	mr r3, r31
/* 80303614 00300554  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 80303618 00300558  38 21 00 18 */	addi r1, r1, 0x18
/* 8030361C 0030055C  7C 08 03 A6 */	mtlr r0
/* 80303620 00300560  4E 80 00 20 */	blr 

.global execute__23TNerveBossHanachanGetUpCFP24TSpineBase$$010TLiveActor$$1
execute__23TNerveBossHanachanGetUpCFP24TSpineBase$$010TLiveActor$$1:
/* 80303624 00300564  7C 08 02 A6 */	mflr r0
/* 80303628 00300568  90 01 00 04 */	stw r0, 4(r1)
/* 8030362C 0030056C  94 21 FF D8 */	stwu r1, -0x28(r1)
/* 80303630 00300570  93 E1 00 24 */	stw r31, 0x24(r1)
/* 80303634 00300574  7C 9F 23 78 */	mr r31, r4
/* 80303638 00300578  93 C1 00 20 */	stw r30, 0x20(r1)
/* 8030363C 0030057C  83 C4 00 00 */	lwz r30, 0(r4)
/* 80303640 00300580  38 80 00 02 */	li r4, 2
/* 80303644 00300584  38 7E 00 00 */	addi r3, r30, 0
/* 80303648 00300588  48 00 10 75 */	bl considerSetAnm__13TBossHanachanF24EnumBossHanachanNerveAnm
/* 8030364C 0030058C  7F C3 F3 78 */	mr r3, r30
/* 80303650 00300590  48 00 10 11 */	bl isFinishedGetUp__13TBossHanachanCFv
/* 80303654 00300594  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 80303658 00300598  41 82 00 94 */	beq lbl_803036EC
/* 8030365C 0030059C  7F C3 F3 78 */	mr r3, r30
/* 80303660 003005A0  4B FF F3 59 */	bl setRandomWeakBodyIndex__13TBossHanachanFv
/* 80303664 003005A4  7F C3 F3 78 */	mr r3, r30
/* 80303668 003005A8  48 00 13 61 */	bl setAnmTimerWhenSnort__13TBossHanachanFv
/* 8030366C 003005AC  88 0D A4 E8 */	lbz r0, init$2410-_SDA_BASE_(r13)
/* 80303670 003005B0  7C 00 07 75 */	extsb. r0, r0
/* 80303674 003005B4  40 82 00 3C */	bne lbl_803036B0
/* 80303678 003005B8  3C 60 80 3C */	lis r3, __vt__24TNerveBase$$010TLiveActor$$1@ha
/* 8030367C 003005BC  38 03 92 60 */	addi r0, r3, __vt__24TNerveBase$$010TLiveActor$$1@l
/* 80303680 003005C0  3C 60 80 3E */	lis r3, __vt__23TNerveBossHanachanSnort@ha
/* 80303684 003005C4  90 0D A4 EC */	stw r0, instance$2408-_SDA_BASE_(r13)
/* 80303688 003005C8  38 03 F5 C0 */	addi r0, r3, __vt__23TNerveBossHanachanSnort@l
/* 8030368C 003005CC  3C 80 80 30 */	lis r4, __dt__23TNerveBossHanachanSnortFv@ha
/* 80303690 003005D0  90 0D A4 EC */	stw r0, instance$2408-_SDA_BASE_(r13)
/* 80303694 003005D4  3C 60 80 40 */	lis r3, $$22409@ha
/* 80303698 003005D8  38 A3 EE 14 */	addi r5, r3, $$22409@l
/* 8030369C 003005DC  38 84 33 B4 */	addi r4, r4, __dt__23TNerveBossHanachanSnortFv@l
/* 803036A0 003005E0  38 6D A4 EC */	addi r3, r13, instance$2408-_SDA_BASE_
/* 803036A4 003005E4  4B D7 F0 85 */	bl __register_global_object
/* 803036A8 003005E8  38 00 00 01 */	li r0, 1
/* 803036AC 003005EC  98 0D A4 E8 */	stb r0, init$2410-_SDA_BASE_(r13)
lbl_803036B0:
/* 803036B0 003005F0  38 8D A4 EC */	addi r4, r13, instance$2408-_SDA_BASE_
/* 803036B4 003005F4  28 04 00 00 */	cmplwi r4, 0
/* 803036B8 003005F8  41 82 00 2C */	beq lbl_803036E4
/* 803036BC 003005FC  80 BF 00 08 */	lwz r5, 8(r31)
/* 803036C0 00300600  80 1F 00 04 */	lwz r0, 4(r31)
/* 803036C4 00300604  7C 05 00 00 */	cmpw r5, r0
/* 803036C8 00300608  40 80 00 1C */	bge lbl_803036E4
/* 803036CC 0030060C  80 7F 00 0C */	lwz r3, 0xc(r31)
/* 803036D0 00300610  54 A0 10 3A */	slwi r0, r5, 2
/* 803036D4 00300614  7C 83 01 2E */	stwx r4, r3, r0
/* 803036D8 00300618  80 7F 00 08 */	lwz r3, 8(r31)
/* 803036DC 0030061C  38 03 00 01 */	addi r0, r3, 1
/* 803036E0 00300620  90 1F 00 08 */	stw r0, 8(r31)
lbl_803036E4:
/* 803036E4 00300624  38 60 00 01 */	li r3, 1
/* 803036E8 00300628  48 00 00 08 */	b lbl_803036F0
lbl_803036EC:
/* 803036EC 0030062C  38 60 00 00 */	li r3, 0
lbl_803036F0:
/* 803036F0 00300630  80 01 00 2C */	lwz r0, 0x2c(r1)
/* 803036F4 00300634  83 E1 00 24 */	lwz r31, 0x24(r1)
/* 803036F8 00300638  83 C1 00 20 */	lwz r30, 0x20(r1)
/* 803036FC 0030063C  7C 08 03 A6 */	mtlr r0
/* 80303700 00300640  38 21 00 28 */	addi r1, r1, 0x28
/* 80303704 00300644  4E 80 00 20 */	blr 

.global theNerve__23TNerveBossHanachanGetUpFv
theNerve__23TNerveBossHanachanGetUpFv:
/* 80303708 00300648  7C 08 02 A6 */	mflr r0
/* 8030370C 0030064C  90 01 00 04 */	stw r0, 4(r1)
/* 80303710 00300650  94 21 FF F8 */	stwu r1, -8(r1)
/* 80303714 00300654  88 0D A4 D8 */	lbz r0, init$2393-_SDA_BASE_(r13)
/* 80303718 00300658  7C 00 07 75 */	extsb. r0, r0
/* 8030371C 0030065C  40 82 00 3C */	bne lbl_80303758
/* 80303720 00300660  3C 60 80 3C */	lis r3, __vt__24TNerveBase$$010TLiveActor$$1@ha
/* 80303724 00300664  38 03 92 60 */	addi r0, r3, __vt__24TNerveBase$$010TLiveActor$$1@l
/* 80303728 00300668  3C 60 80 3E */	lis r3, __vt__23TNerveBossHanachanGetUp@ha
/* 8030372C 0030066C  90 0D A4 DC */	stw r0, instance$2391-_SDA_BASE_(r13)
/* 80303730 00300670  38 03 F5 E0 */	addi r0, r3, __vt__23TNerveBossHanachanGetUp@l
/* 80303734 00300674  3C 80 80 30 */	lis r4, __dt__23TNerveBossHanachanGetUpFv@ha
/* 80303738 00300678  90 0D A4 DC */	stw r0, instance$2391-_SDA_BASE_(r13)
/* 8030373C 0030067C  3C 60 80 40 */	lis r3, $$22392@ha
/* 80303740 00300680  38 A3 ED FC */	addi r5, r3, $$22392@l
/* 80303744 00300684  38 84 35 08 */	addi r4, r4, __dt__23TNerveBossHanachanGetUpFv@l
/* 80303748 00300688  38 6D A4 DC */	addi r3, r13, instance$2391-_SDA_BASE_
/* 8030374C 0030068C  4B D7 EF DD */	bl __register_global_object
/* 80303750 00300690  38 00 00 01 */	li r0, 1
/* 80303754 00300694  98 0D A4 D8 */	stb r0, init$2393-_SDA_BASE_(r13)
lbl_80303758:
/* 80303758 00300698  80 01 00 0C */	lwz r0, 0xc(r1)
/* 8030375C 0030069C  38 6D A4 DC */	addi r3, r13, instance$2391-_SDA_BASE_
/* 80303760 003006A0  38 21 00 08 */	addi r1, r1, 8
/* 80303764 003006A4  7C 08 03 A6 */	mtlr r0
/* 80303768 003006A8  4E 80 00 20 */	blr 

.global execute__22TNerveBossHanachanDownCFP24TSpineBase$$010TLiveActor$$1
execute__22TNerveBossHanachanDownCFP24TSpineBase$$010TLiveActor$$1:
/* 8030376C 003006AC  7C 08 02 A6 */	mflr r0
/* 80303770 003006B0  90 01 00 04 */	stw r0, 4(r1)
/* 80303774 003006B4  94 21 FF C8 */	stwu r1, -0x38(r1)
/* 80303778 003006B8  93 E1 00 34 */	stw r31, 0x34(r1)
/* 8030377C 003006BC  7C 9F 23 78 */	mr r31, r4
/* 80303780 003006C0  93 C1 00 30 */	stw r30, 0x30(r1)
/* 80303784 003006C4  83 C4 00 00 */	lwz r30, 0(r4)
/* 80303788 003006C8  38 80 00 01 */	li r4, 1
/* 8030378C 003006CC  38 7E 00 00 */	addi r3, r30, 0
/* 80303790 003006D0  48 00 0F 2D */	bl considerSetAnm__13TBossHanachanF24EnumBossHanachanNerveAnm
/* 80303794 003006D4  80 7E 01 C0 */	lwz r3, 0x1c0(r30)
/* 80303798 003006D8  80 9F 00 20 */	lwz r4, 0x20(r31)
/* 8030379C 003006DC  A8 03 01 A8 */	lha r0, 0x1a8(r3)
/* 803037A0 003006E0  7C 04 00 00 */	cmpw r4, r0
/* 803037A4 003006E4  41 80 00 8C */	blt lbl_80303830
/* 803037A8 003006E8  7F C3 F3 78 */	mr r3, r30
/* 803037AC 003006EC  48 00 12 95 */	bl setAnmTimerWhenGetUp__13TBossHanachanFv
/* 803037B0 003006F0  88 0D A4 D8 */	lbz r0, init$2393-_SDA_BASE_(r13)
/* 803037B4 003006F4  7C 00 07 75 */	extsb. r0, r0
/* 803037B8 003006F8  40 82 00 3C */	bne lbl_803037F4
/* 803037BC 003006FC  3C 60 80 3C */	lis r3, __vt__24TNerveBase$$010TLiveActor$$1@ha
/* 803037C0 00300700  38 03 92 60 */	addi r0, r3, __vt__24TNerveBase$$010TLiveActor$$1@l
/* 803037C4 00300704  3C 60 80 3E */	lis r3, __vt__23TNerveBossHanachanGetUp@ha
/* 803037C8 00300708  90 0D A4 DC */	stw r0, instance$2391-_SDA_BASE_(r13)
/* 803037CC 0030070C  38 03 F5 E0 */	addi r0, r3, __vt__23TNerveBossHanachanGetUp@l
/* 803037D0 00300710  3C 80 80 30 */	lis r4, __dt__23TNerveBossHanachanGetUpFv@ha
/* 803037D4 00300714  90 0D A4 DC */	stw r0, instance$2391-_SDA_BASE_(r13)
/* 803037D8 00300718  3C 60 80 40 */	lis r3, $$22392@ha
/* 803037DC 0030071C  38 A3 ED FC */	addi r5, r3, $$22392@l
/* 803037E0 00300720  38 84 35 08 */	addi r4, r4, __dt__23TNerveBossHanachanGetUpFv@l
/* 803037E4 00300724  38 6D A4 DC */	addi r3, r13, instance$2391-_SDA_BASE_
/* 803037E8 00300728  4B D7 EF 41 */	bl __register_global_object
/* 803037EC 0030072C  38 00 00 01 */	li r0, 1
/* 803037F0 00300730  98 0D A4 D8 */	stb r0, init$2393-_SDA_BASE_(r13)
lbl_803037F4:
/* 803037F4 00300734  38 8D A4 DC */	addi r4, r13, instance$2391-_SDA_BASE_
/* 803037F8 00300738  28 04 00 00 */	cmplwi r4, 0
/* 803037FC 0030073C  41 82 00 2C */	beq lbl_80303828
/* 80303800 00300740  80 BF 00 08 */	lwz r5, 8(r31)
/* 80303804 00300744  80 1F 00 04 */	lwz r0, 4(r31)
/* 80303808 00300748  7C 05 00 00 */	cmpw r5, r0
/* 8030380C 0030074C  40 80 00 1C */	bge lbl_80303828
/* 80303810 00300750  80 7F 00 0C */	lwz r3, 0xc(r31)
/* 80303814 00300754  54 A0 10 3A */	slwi r0, r5, 2
/* 80303818 00300758  7C 83 01 2E */	stwx r4, r3, r0
/* 8030381C 0030075C  80 7F 00 08 */	lwz r3, 8(r31)
/* 80303820 00300760  38 03 00 01 */	addi r0, r3, 1
/* 80303824 00300764  90 1F 00 08 */	stw r0, 8(r31)
lbl_80303828:
/* 80303828 00300768  38 60 00 01 */	li r3, 1
/* 8030382C 0030076C  48 00 00 08 */	b lbl_80303834
lbl_80303830:
/* 80303830 00300770  38 60 00 00 */	li r3, 0
lbl_80303834:
/* 80303834 00300774  80 01 00 3C */	lwz r0, 0x3c(r1)
/* 80303838 00300778  83 E1 00 34 */	lwz r31, 0x34(r1)
/* 8030383C 0030077C  83 C1 00 30 */	lwz r30, 0x30(r1)
/* 80303840 00300780  7C 08 03 A6 */	mtlr r0
/* 80303844 00300784  38 21 00 38 */	addi r1, r1, 0x38
/* 80303848 00300788  4E 80 00 20 */	blr 

.global theNerve__22TNerveBossHanachanDownFv
theNerve__22TNerveBossHanachanDownFv:
/* 8030384C 0030078C  7C 08 02 A6 */	mflr r0
/* 80303850 00300790  90 01 00 04 */	stw r0, 4(r1)
/* 80303854 00300794  94 21 FF F8 */	stwu r1, -8(r1)
/* 80303858 00300798  88 0D A4 D0 */	lbz r0, init$2385-_SDA_BASE_(r13)
/* 8030385C 0030079C  7C 00 07 75 */	extsb. r0, r0
/* 80303860 003007A0  40 82 00 3C */	bne lbl_8030389C
/* 80303864 003007A4  3C 60 80 3C */	lis r3, __vt__24TNerveBase$$010TLiveActor$$1@ha
/* 80303868 003007A8  38 03 92 60 */	addi r0, r3, __vt__24TNerveBase$$010TLiveActor$$1@l
/* 8030386C 003007AC  3C 60 80 3E */	lis r3, __vt__22TNerveBossHanachanDown@ha
/* 80303870 003007B0  90 0D A4 D4 */	stw r0, instance$2383-_SDA_BASE_(r13)
/* 80303874 003007B4  38 03 F5 F0 */	addi r0, r3, __vt__22TNerveBossHanachanDown@l
/* 80303878 003007B8  3C 80 80 30 */	lis r4, __dt__22TNerveBossHanachanDownFv@ha
/* 8030387C 003007BC  90 0D A4 D4 */	stw r0, instance$2383-_SDA_BASE_(r13)
/* 80303880 003007C0  3C 60 80 40 */	lis r3, $$22384@ha
/* 80303884 003007C4  38 A3 ED F0 */	addi r5, r3, $$22384@l
/* 80303888 003007C8  38 84 38 B0 */	addi r4, r4, __dt__22TNerveBossHanachanDownFv@l
/* 8030388C 003007CC  38 6D A4 D4 */	addi r3, r13, instance$2383-_SDA_BASE_
/* 80303890 003007D0  4B D7 EE 99 */	bl __register_global_object
/* 80303894 003007D4  38 00 00 01 */	li r0, 1
/* 80303898 003007D8  98 0D A4 D0 */	stb r0, init$2385-_SDA_BASE_(r13)
lbl_8030389C:
/* 8030389C 003007DC  80 01 00 0C */	lwz r0, 0xc(r1)
/* 803038A0 003007E0  38 6D A4 D4 */	addi r3, r13, instance$2383-_SDA_BASE_
/* 803038A4 003007E4  38 21 00 08 */	addi r1, r1, 8
/* 803038A8 003007E8  7C 08 03 A6 */	mtlr r0
/* 803038AC 003007EC  4E 80 00 20 */	blr 

.global __dt__22TNerveBossHanachanDownFv
__dt__22TNerveBossHanachanDownFv:
/* 803038B0 003007F0  7C 08 02 A6 */	mflr r0
/* 803038B4 003007F4  90 01 00 04 */	stw r0, 4(r1)
/* 803038B8 003007F8  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 803038BC 003007FC  93 E1 00 14 */	stw r31, 0x14(r1)
/* 803038C0 00300800  7C 7F 1B 79 */	or. r31, r3, r3
/* 803038C4 00300804  41 82 00 30 */	beq lbl_803038F4
/* 803038C8 00300808  3C 60 80 3E */	lis r3, __vt__22TNerveBossHanachanDown@ha
/* 803038CC 0030080C  38 03 F5 F0 */	addi r0, r3, __vt__22TNerveBossHanachanDown@l
/* 803038D0 00300810  90 1F 00 00 */	stw r0, 0(r31)
/* 803038D4 00300814  41 82 00 10 */	beq lbl_803038E4
/* 803038D8 00300818  3C 60 80 3C */	lis r3, __vt__24TNerveBase$$010TLiveActor$$1@ha
/* 803038DC 0030081C  38 03 92 60 */	addi r0, r3, __vt__24TNerveBase$$010TLiveActor$$1@l
/* 803038E0 00300820  90 1F 00 00 */	stw r0, 0(r31)
lbl_803038E4:
/* 803038E4 00300824  7C 80 07 35 */	extsh. r0, r4
/* 803038E8 00300828  40 81 00 0C */	ble lbl_803038F4
/* 803038EC 0030082C  7F E3 FB 78 */	mr r3, r31
/* 803038F0 00300830  4B D0 91 C1 */	bl __dl__FPv
lbl_803038F4:
/* 803038F4 00300834  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 803038F8 00300838  7F E3 FB 78 */	mr r3, r31
/* 803038FC 0030083C  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 80303900 00300840  38 21 00 18 */	addi r1, r1, 0x18
/* 80303904 00300844  7C 08 03 A6 */	mtlr r0
/* 80303908 00300848  4E 80 00 20 */	blr 

.global execute__24TNerveBossHanachanTumbleCFP24TSpineBase$$010TLiveActor$$1
execute__24TNerveBossHanachanTumbleCFP24TSpineBase$$010TLiveActor$$1:
/* 8030390C 0030084C  7C 08 02 A6 */	mflr r0
/* 80303910 00300850  90 01 00 04 */	stw r0, 4(r1)
/* 80303914 00300854  94 21 FF C0 */	stwu r1, -0x40(r1)
/* 80303918 00300858  93 E1 00 3C */	stw r31, 0x3c(r1)
/* 8030391C 0030085C  7C 9F 23 78 */	mr r31, r4
/* 80303920 00300860  93 C1 00 38 */	stw r30, 0x38(r1)
/* 80303924 00300864  80 04 00 20 */	lwz r0, 0x20(r4)
/* 80303928 00300868  83 C4 00 00 */	lwz r30, 0(r4)
/* 8030392C 0030086C  2C 00 00 00 */	cmpwi r0, 0
/* 80303930 00300870  40 82 00 14 */	bne lbl_80303944
/* 80303934 00300874  38 7E 00 00 */	addi r3, r30, 0
/* 80303938 00300878  38 80 00 01 */	li r4, 1
/* 8030393C 0030087C  48 00 11 79 */	bl setTumbleAnm__13TBossHanachanF36EnumBossHanachanStopMotionBlendOnOff
/* 80303940 00300880  48 00 00 10 */	b lbl_80303950
lbl_80303944:
/* 80303944 00300884  38 7E 00 00 */	addi r3, r30, 0
/* 80303948 00300888  38 80 00 00 */	li r4, 0
/* 8030394C 0030088C  48 00 0D 71 */	bl considerSetAnm__13TBossHanachanF24EnumBossHanachanNerveAnm
lbl_80303950:
/* 80303950 00300890  7F C3 F3 78 */	mr r3, r30
/* 80303954 00300894  4B FF C2 11 */	bl execSlip__13TBossHanachanFv
/* 80303958 00300898  C0 22 F7 C0 */	lfs f1, $$22521-_SDA2_BASE_(r2)
/* 8030395C 0030089C  C0 1E 01 40 */	lfs f0, 0x140(r30)
/* 80303960 003008A0  FC 01 00 00 */	fcmpu cr0, f1, f0
/* 80303964 003008A4  40 82 00 AC */	bne lbl_80303A10
/* 80303968 003008A8  7F C3 F3 78 */	mr r3, r30
/* 8030396C 003008AC  4B FF C9 AD */	bl isTumbleCompletelyAllBody__13TBossHanachanCFv
/* 80303970 003008B0  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 80303974 003008B4  41 82 00 9C */	beq lbl_80303A10
/* 80303978 003008B8  80 AD 97 E8 */	lwz r5, gpMarDirector-_SDA_BASE_(r13)
/* 8030397C 003008BC  3C 60 00 0E */	lis r3, 0x000E0007@ha
/* 80303980 003008C0  38 83 00 07 */	addi r4, r3, 0x000E0007@l
/* 80303984 003008C4  80 65 00 74 */	lwz r3, 0x74(r5)
/* 80303988 003008C8  38 A0 00 01 */	li r5, 1
/* 8030398C 003008CC  4B F0 7C B9 */	bl startAppearBalloon__11TGCConsole2FUlb
/* 80303990 003008D0  88 0D A4 D0 */	lbz r0, init$2385-_SDA_BASE_(r13)
/* 80303994 003008D4  7C 00 07 75 */	extsb. r0, r0
/* 80303998 003008D8  40 82 00 3C */	bne lbl_803039D4
/* 8030399C 003008DC  3C 60 80 3C */	lis r3, __vt__24TNerveBase$$010TLiveActor$$1@ha
/* 803039A0 003008E0  38 03 92 60 */	addi r0, r3, __vt__24TNerveBase$$010TLiveActor$$1@l
/* 803039A4 003008E4  3C 60 80 3E */	lis r3, __vt__22TNerveBossHanachanDown@ha
/* 803039A8 003008E8  90 0D A4 D4 */	stw r0, instance$2383-_SDA_BASE_(r13)
/* 803039AC 003008EC  38 03 F5 F0 */	addi r0, r3, __vt__22TNerveBossHanachanDown@l
/* 803039B0 003008F0  3C 80 80 30 */	lis r4, __dt__22TNerveBossHanachanDownFv@ha
/* 803039B4 003008F4  90 0D A4 D4 */	stw r0, instance$2383-_SDA_BASE_(r13)
/* 803039B8 003008F8  3C 60 80 40 */	lis r3, $$22384@ha
/* 803039BC 003008FC  38 A3 ED F0 */	addi r5, r3, $$22384@l
/* 803039C0 00300900  38 84 38 B0 */	addi r4, r4, __dt__22TNerveBossHanachanDownFv@l
/* 803039C4 00300904  38 6D A4 D4 */	addi r3, r13, instance$2383-_SDA_BASE_
/* 803039C8 00300908  4B D7 ED 61 */	bl __register_global_object
/* 803039CC 0030090C  38 00 00 01 */	li r0, 1
/* 803039D0 00300910  98 0D A4 D0 */	stb r0, init$2385-_SDA_BASE_(r13)
lbl_803039D4:
/* 803039D4 00300914  38 8D A4 D4 */	addi r4, r13, instance$2383-_SDA_BASE_
/* 803039D8 00300918  28 04 00 00 */	cmplwi r4, 0
/* 803039DC 0030091C  41 82 00 2C */	beq lbl_80303A08
/* 803039E0 00300920  80 BF 00 08 */	lwz r5, 8(r31)
/* 803039E4 00300924  80 1F 00 04 */	lwz r0, 4(r31)
/* 803039E8 00300928  7C 05 00 00 */	cmpw r5, r0
/* 803039EC 0030092C  40 80 00 1C */	bge lbl_80303A08
/* 803039F0 00300930  80 7F 00 0C */	lwz r3, 0xc(r31)
/* 803039F4 00300934  54 A0 10 3A */	slwi r0, r5, 2
/* 803039F8 00300938  7C 83 01 2E */	stwx r4, r3, r0
/* 803039FC 0030093C  80 7F 00 08 */	lwz r3, 8(r31)
/* 80303A00 00300940  38 03 00 01 */	addi r0, r3, 1
/* 80303A04 00300944  90 1F 00 08 */	stw r0, 8(r31)
lbl_80303A08:
/* 80303A08 00300948  38 60 00 01 */	li r3, 1
/* 80303A0C 0030094C  48 00 00 08 */	b lbl_80303A14
lbl_80303A10:
/* 80303A10 00300950  38 60 00 00 */	li r3, 0
lbl_80303A14:
/* 80303A14 00300954  80 01 00 44 */	lwz r0, 0x44(r1)
/* 80303A18 00300958  83 E1 00 3C */	lwz r31, 0x3c(r1)
/* 80303A1C 0030095C  83 C1 00 38 */	lwz r30, 0x38(r1)
/* 80303A20 00300960  7C 08 03 A6 */	mtlr r0
/* 80303A24 00300964  38 21 00 40 */	addi r1, r1, 0x40
/* 80303A28 00300968  4E 80 00 20 */	blr 

.global theNerve__24TNerveBossHanachanTumbleFv
theNerve__24TNerveBossHanachanTumbleFv:
/* 80303A2C 0030096C  7C 08 02 A6 */	mflr r0
/* 80303A30 00300970  90 01 00 04 */	stw r0, 4(r1)
/* 80303A34 00300974  94 21 FF F8 */	stwu r1, -8(r1)
/* 80303A38 00300978  88 0D A4 C8 */	lbz r0, init$2373-_SDA_BASE_(r13)
/* 80303A3C 0030097C  7C 00 07 75 */	extsb. r0, r0
/* 80303A40 00300980  40 82 00 3C */	bne lbl_80303A7C
/* 80303A44 00300984  3C 60 80 3C */	lis r3, __vt__24TNerveBase$$010TLiveActor$$1@ha
/* 80303A48 00300988  38 03 92 60 */	addi r0, r3, __vt__24TNerveBase$$010TLiveActor$$1@l
/* 80303A4C 0030098C  3C 60 80 3E */	lis r3, __vt__24TNerveBossHanachanTumble@ha
/* 80303A50 00300990  90 0D A4 CC */	stw r0, instance$2371-_SDA_BASE_(r13)
/* 80303A54 00300994  38 03 F6 00 */	addi r0, r3, __vt__24TNerveBossHanachanTumble@l
/* 80303A58 00300998  3C 80 80 30 */	lis r4, __dt__24TNerveBossHanachanTumbleFv@ha
/* 80303A5C 0030099C  90 0D A4 CC */	stw r0, instance$2371-_SDA_BASE_(r13)
/* 80303A60 003009A0  3C 60 80 40 */	lis r3, $$22372@ha
/* 80303A64 003009A4  38 A3 ED E4 */	addi r5, r3, $$22372@l
/* 80303A68 003009A8  38 84 3A 90 */	addi r4, r4, __dt__24TNerveBossHanachanTumbleFv@l
/* 80303A6C 003009AC  38 6D A4 CC */	addi r3, r13, instance$2371-_SDA_BASE_
/* 80303A70 003009B0  4B D7 EC B9 */	bl __register_global_object
/* 80303A74 003009B4  38 00 00 01 */	li r0, 1
/* 80303A78 003009B8  98 0D A4 C8 */	stb r0, init$2373-_SDA_BASE_(r13)
lbl_80303A7C:
/* 80303A7C 003009BC  80 01 00 0C */	lwz r0, 0xc(r1)
/* 80303A80 003009C0  38 6D A4 CC */	addi r3, r13, instance$2371-_SDA_BASE_
/* 80303A84 003009C4  38 21 00 08 */	addi r1, r1, 8
/* 80303A88 003009C8  7C 08 03 A6 */	mtlr r0
/* 80303A8C 003009CC  4E 80 00 20 */	blr 

.global __dt__24TNerveBossHanachanTumbleFv
__dt__24TNerveBossHanachanTumbleFv:
/* 80303A90 003009D0  7C 08 02 A6 */	mflr r0
/* 80303A94 003009D4  90 01 00 04 */	stw r0, 4(r1)
/* 80303A98 003009D8  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 80303A9C 003009DC  93 E1 00 14 */	stw r31, 0x14(r1)
/* 80303AA0 003009E0  7C 7F 1B 79 */	or. r31, r3, r3
/* 80303AA4 003009E4  41 82 00 30 */	beq lbl_80303AD4
/* 80303AA8 003009E8  3C 60 80 3E */	lis r3, __vt__24TNerveBossHanachanTumble@ha
/* 80303AAC 003009EC  38 03 F6 00 */	addi r0, r3, __vt__24TNerveBossHanachanTumble@l
/* 80303AB0 003009F0  90 1F 00 00 */	stw r0, 0(r31)
/* 80303AB4 003009F4  41 82 00 10 */	beq lbl_80303AC4
/* 80303AB8 003009F8  3C 60 80 3C */	lis r3, __vt__24TNerveBase$$010TLiveActor$$1@ha
/* 80303ABC 003009FC  38 03 92 60 */	addi r0, r3, __vt__24TNerveBase$$010TLiveActor$$1@l
/* 80303AC0 00300A00  90 1F 00 00 */	stw r0, 0(r31)
lbl_80303AC4:
/* 80303AC4 00300A04  7C 80 07 35 */	extsh. r0, r4
/* 80303AC8 00300A08  40 81 00 0C */	ble lbl_80303AD4
/* 80303ACC 00300A0C  7F E3 FB 78 */	mr r3, r31
/* 80303AD0 00300A10  4B D0 8F E1 */	bl __dl__FPv
lbl_80303AD4:
/* 80303AD4 00300A14  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 80303AD8 00300A18  7F E3 FB 78 */	mr r3, r31
/* 80303ADC 00300A1C  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 80303AE0 00300A20  38 21 00 18 */	addi r1, r1, 0x18
/* 80303AE4 00300A24  7C 08 03 A6 */	mtlr r0
/* 80303AE8 00300A28  4E 80 00 20 */	blr 

.global execute__29TNerveBossHanachanGraphWanderCFP24TSpineBase$$010TLiveActor$$1
execute__29TNerveBossHanachanGraphWanderCFP24TSpineBase$$010TLiveActor$$1:
/* 80303AEC 00300A2C  7C 08 02 A6 */	mflr r0
/* 80303AF0 00300A30  90 01 00 04 */	stw r0, 4(r1)
/* 80303AF4 00300A34  94 21 FF D8 */	stwu r1, -0x28(r1)
/* 80303AF8 00300A38  93 E1 00 24 */	stw r31, 0x24(r1)
/* 80303AFC 00300A3C  7C 9F 23 78 */	mr r31, r4
/* 80303B00 00300A40  93 C1 00 20 */	stw r30, 0x20(r1)
/* 80303B04 00300A44  80 04 00 20 */	lwz r0, 0x20(r4)
/* 80303B08 00300A48  83 C4 00 00 */	lwz r30, 0(r4)
/* 80303B0C 00300A4C  2C 00 00 00 */	cmpwi r0, 0
/* 80303B10 00300A50  40 82 00 14 */	bne lbl_80303B24
/* 80303B14 00300A54  38 7E 00 00 */	addi r3, r30, 0
/* 80303B18 00300A58  38 80 00 00 */	li r4, 0
/* 80303B1C 00300A5C  38 A0 00 01 */	li r5, 1
/* 80303B20 00300A60  48 00 11 39 */	bl setHeadAndBodyAnm__13TBossHanachanF23EnumBossHanachanAnmKind36EnumBossHanachanStopMotionBlendOnOff
lbl_80303B24:
/* 80303B24 00300A64  38 7E 00 00 */	addi r3, r30, 0
/* 80303B28 00300A68  38 80 00 01 */	li r4, 1
/* 80303B2C 00300A6C  4B FF C3 D9 */	bl execWalk__13TBossHanachanFb
/* 80303B30 00300A70  7F C3 F3 78 */	mr r3, r30
/* 80303B34 00300A74  4B FF C6 DD */	bl checkFallDecideAndSetup__13TBossHanachanFv
/* 80303B38 00300A78  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 80303B3C 00300A7C  41 82 00 84 */	beq lbl_80303BC0
/* 80303B40 00300A80  88 0D A4 C8 */	lbz r0, init$2373-_SDA_BASE_(r13)
/* 80303B44 00300A84  7C 00 07 75 */	extsb. r0, r0
/* 80303B48 00300A88  40 82 00 3C */	bne lbl_80303B84
/* 80303B4C 00300A8C  3C 60 80 3C */	lis r3, __vt__24TNerveBase$$010TLiveActor$$1@ha
/* 80303B50 00300A90  38 03 92 60 */	addi r0, r3, __vt__24TNerveBase$$010TLiveActor$$1@l
/* 80303B54 00300A94  3C 60 80 3E */	lis r3, __vt__24TNerveBossHanachanTumble@ha
/* 80303B58 00300A98  90 0D A4 CC */	stw r0, instance$2371-_SDA_BASE_(r13)
/* 80303B5C 00300A9C  38 03 F6 00 */	addi r0, r3, __vt__24TNerveBossHanachanTumble@l
/* 80303B60 00300AA0  3C 80 80 30 */	lis r4, __dt__24TNerveBossHanachanTumbleFv@ha
/* 80303B64 00300AA4  90 0D A4 CC */	stw r0, instance$2371-_SDA_BASE_(r13)
/* 80303B68 00300AA8  3C 60 80 40 */	lis r3, $$22372@ha
/* 80303B6C 00300AAC  38 A3 ED E4 */	addi r5, r3, $$22372@l
/* 80303B70 00300AB0  38 84 3A 90 */	addi r4, r4, __dt__24TNerveBossHanachanTumbleFv@l
/* 80303B74 00300AB4  38 6D A4 CC */	addi r3, r13, instance$2371-_SDA_BASE_
/* 80303B78 00300AB8  4B D7 EB B1 */	bl __register_global_object
/* 80303B7C 00300ABC  38 00 00 01 */	li r0, 1
/* 80303B80 00300AC0  98 0D A4 C8 */	stb r0, init$2373-_SDA_BASE_(r13)
lbl_80303B84:
/* 80303B84 00300AC4  38 8D A4 CC */	addi r4, r13, instance$2371-_SDA_BASE_
/* 80303B88 00300AC8  28 04 00 00 */	cmplwi r4, 0
/* 80303B8C 00300ACC  41 82 00 2C */	beq lbl_80303BB8
/* 80303B90 00300AD0  80 BF 00 08 */	lwz r5, 8(r31)
/* 80303B94 00300AD4  80 1F 00 04 */	lwz r0, 4(r31)
/* 80303B98 00300AD8  7C 05 00 00 */	cmpw r5, r0
/* 80303B9C 00300ADC  40 80 00 1C */	bge lbl_80303BB8
/* 80303BA0 00300AE0  80 7F 00 0C */	lwz r3, 0xc(r31)
/* 80303BA4 00300AE4  54 A0 10 3A */	slwi r0, r5, 2
/* 80303BA8 00300AE8  7C 83 01 2E */	stwx r4, r3, r0
/* 80303BAC 00300AEC  80 7F 00 08 */	lwz r3, 8(r31)
/* 80303BB0 00300AF0  38 03 00 01 */	addi r0, r3, 1
/* 80303BB4 00300AF4  90 1F 00 08 */	stw r0, 8(r31)
lbl_80303BB8:
/* 80303BB8 00300AF8  38 60 00 01 */	li r3, 1
/* 80303BBC 00300AFC  48 00 00 08 */	b lbl_80303BC4
lbl_80303BC0:
/* 80303BC0 00300B00  38 60 00 00 */	li r3, 0
lbl_80303BC4:
/* 80303BC4 00300B04  80 01 00 2C */	lwz r0, 0x2c(r1)
/* 80303BC8 00300B08  83 E1 00 24 */	lwz r31, 0x24(r1)
/* 80303BCC 00300B0C  83 C1 00 20 */	lwz r30, 0x20(r1)
/* 80303BD0 00300B10  7C 08 03 A6 */	mtlr r0
/* 80303BD4 00300B14  38 21 00 28 */	addi r1, r1, 0x28
/* 80303BD8 00300B18  4E 80 00 20 */	blr 

.global theNerve__29TNerveBossHanachanGraphWanderFv
theNerve__29TNerveBossHanachanGraphWanderFv:
/* 80303BDC 00300B1C  7C 08 02 A6 */	mflr r0
/* 80303BE0 00300B20  90 01 00 04 */	stw r0, 4(r1)
/* 80303BE4 00300B24  94 21 FF F8 */	stwu r1, -8(r1)
/* 80303BE8 00300B28  88 0D A4 C0 */	lbz r0, init$2361-_SDA_BASE_(r13)
/* 80303BEC 00300B2C  7C 00 07 75 */	extsb. r0, r0
/* 80303BF0 00300B30  40 82 00 3C */	bne lbl_80303C2C
/* 80303BF4 00300B34  3C 60 80 3C */	lis r3, __vt__24TNerveBase$$010TLiveActor$$1@ha
/* 80303BF8 00300B38  38 03 92 60 */	addi r0, r3, __vt__24TNerveBase$$010TLiveActor$$1@l
/* 80303BFC 00300B3C  3C 60 80 3E */	lis r3, __vt__29TNerveBossHanachanGraphWander@ha
/* 80303C00 00300B40  90 0D A4 C4 */	stw r0, instance$2359-_SDA_BASE_(r13)
/* 80303C04 00300B44  38 03 F6 10 */	addi r0, r3, __vt__29TNerveBossHanachanGraphWander@l
/* 80303C08 00300B48  3C 80 80 30 */	lis r4, __dt__29TNerveBossHanachanGraphWanderFv@ha
/* 80303C0C 00300B4C  90 0D A4 C4 */	stw r0, instance$2359-_SDA_BASE_(r13)
/* 80303C10 00300B50  3C 60 80 40 */	lis r3, $$22360@ha
/* 80303C14 00300B54  38 A3 ED D8 */	addi r5, r3, $$22360@l
/* 80303C18 00300B58  38 84 32 F4 */	addi r4, r4, __dt__29TNerveBossHanachanGraphWanderFv@l
/* 80303C1C 00300B5C  38 6D A4 C4 */	addi r3, r13, instance$2359-_SDA_BASE_
/* 80303C20 00300B60  4B D7 EB 09 */	bl __register_global_object
/* 80303C24 00300B64  38 00 00 01 */	li r0, 1
/* 80303C28 00300B68  98 0D A4 C0 */	stb r0, init$2361-_SDA_BASE_(r13)
lbl_80303C2C:
/* 80303C2C 00300B6C  80 01 00 0C */	lwz r0, 0xc(r1)
/* 80303C30 00300B70  38 6D A4 C4 */	addi r3, r13, instance$2359-_SDA_BASE_
/* 80303C34 00300B74  38 21 00 08 */	addi r1, r1, 8
/* 80303C38 00300B78  7C 08 03 A6 */	mtlr r0
/* 80303C3C 00300B7C  4E 80 00 20 */	blr 

.global __sinit_BossHanachanNerve_cpp
__sinit_BossHanachanNerve_cpp:
/* 80303C40 00300B80  7C 08 02 A6 */	mflr r0
/* 80303C44 00300B84  3C 60 80 40 */	lis r3, $$22360@ha
/* 80303C48 00300B88  90 01 00 04 */	stw r0, 4(r1)
/* 80303C4C 00300B8C  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 80303C50 00300B90  93 E1 00 0C */	stw r31, 0xc(r1)
/* 80303C54 00300B94  3B E3 ED D8 */	addi r31, r3, $$22360@l
/* 80303C58 00300B98  88 0D 97 CC */	lbz r0, __init__smList__15JALList$$05MSBgm$$1-_SDA_BASE_(r13)
/* 80303C5C 00300B9C  7C 00 07 75 */	extsb. r0, r0
/* 80303C60 00300BA0  40 82 00 28 */	bne lbl_80303C88
/* 80303C64 00300BA4  38 6D 97 A8 */	addi r3, r13, smList__15JALList$$05MSBgm$$1-_SDA_BASE_
/* 80303C68 00300BA8  4B D0 AF CD */	bl initiate__10JSUPtrListFv
/* 80303C6C 00300BAC  3C 60 80 0E */	lis r3, __dt__15JSUList$$05MSBgm$$1Fv@ha
/* 80303C70 00300BB0  38 83 6A 44 */	addi r4, r3, __dt__15JSUList$$05MSBgm$$1Fv@l
/* 80303C74 00300BB4  38 6D 97 A8 */	addi r3, r13, smList__15JALList$$05MSBgm$$1-_SDA_BASE_
/* 80303C78 00300BB8  38 BF 00 54 */	addi r5, r31, 0x54
/* 80303C7C 00300BBC  4B D7 EA AD */	bl __register_global_object
/* 80303C80 00300BC0  38 00 00 01 */	li r0, 1
/* 80303C84 00300BC4  98 0D 97 CC */	stb r0, __init__smList__15JALList$$05MSBgm$$1-_SDA_BASE_(r13)
lbl_80303C88:
/* 80303C88 00300BC8  88 0D 97 CD */	lbz r0, __init__smList__24JALList$$013MSSetSoundGrp$$1-_SDA_BASE_(r13)
/* 80303C8C 00300BCC  7C 00 07 75 */	extsb. r0, r0
/* 80303C90 00300BD0  40 82 00 28 */	bne lbl_80303CB8
/* 80303C94 00300BD4  38 6D 97 B4 */	addi r3, r13, smList__24JALList$$013MSSetSoundGrp$$1-_SDA_BASE_
/* 80303C98 00300BD8  4B D0 AF 9D */	bl initiate__10JSUPtrListFv
/* 80303C9C 00300BDC  3C 60 80 0E */	lis r3, __dt__24JSUList$$013MSSetSoundGrp$$1Fv@ha
/* 80303CA0 00300BE0  38 83 69 EC */	addi r4, r3, __dt__24JSUList$$013MSSetSoundGrp$$1Fv@l
/* 80303CA4 00300BE4  38 6D 97 B4 */	addi r3, r13, smList__24JALList$$013MSSetSoundGrp$$1-_SDA_BASE_
/* 80303CA8 00300BE8  38 BF 00 60 */	addi r5, r31, 0x60
/* 80303CAC 00300BEC  4B D7 EA 7D */	bl __register_global_object
/* 80303CB0 00300BF0  38 00 00 01 */	li r0, 1
/* 80303CB4 00300BF4  98 0D 97 CD */	stb r0, __init__smList__24JALList$$013MSSetSoundGrp$$1-_SDA_BASE_(r13)
lbl_80303CB8:
/* 80303CB8 00300BF8  88 0D 97 CE */	lbz r0, __init__smList__21JALList$$010MSSetSound$$1-_SDA_BASE_(r13)
/* 80303CBC 00300BFC  7C 00 07 75 */	extsb. r0, r0
/* 80303CC0 00300C00  40 82 00 28 */	bne lbl_80303CE8
/* 80303CC4 00300C04  38 6D 97 C0 */	addi r3, r13, smList__21JALList$$010MSSetSound$$1-_SDA_BASE_
/* 80303CC8 00300C08  4B D0 AF 6D */	bl initiate__10JSUPtrListFv
/* 80303CCC 00300C0C  3C 60 80 0E */	lis r3, __dt__21JSUList$$010MSSetSound$$1Fv@ha
/* 80303CD0 00300C10  38 83 69 94 */	addi r4, r3, __dt__21JSUList$$010MSSetSound$$1Fv@l
/* 80303CD4 00300C14  38 6D 97 C0 */	addi r3, r13, smList__21JALList$$010MSSetSound$$1-_SDA_BASE_
/* 80303CD8 00300C18  38 BF 00 6C */	addi r5, r31, 0x6c
/* 80303CDC 00300C1C  4B D7 EA 4D */	bl __register_global_object
/* 80303CE0 00300C20  38 00 00 01 */	li r0, 1
/* 80303CE4 00300C24  98 0D 97 CE */	stb r0, __init__smList__21JALList$$010MSSetSound$$1-_SDA_BASE_(r13)
lbl_80303CE8:
/* 80303CE8 00300C28  88 0D 8F 8C */	lbz r0, __init__smList__26JALList$$015JALSeModEffDGrp$$1-_SDA_BASE_(r13)
/* 80303CEC 00300C2C  7C 00 07 75 */	extsb. r0, r0
/* 80303CF0 00300C30  40 82 00 28 */	bne lbl_80303D18
/* 80303CF4 00300C34  38 6D 8E FC */	addi r3, r13, smList__26JALList$$015JALSeModEffDGrp$$1-_SDA_BASE_
/* 80303CF8 00300C38  4B D0 AF 3D */	bl initiate__10JSUPtrListFv
/* 80303CFC 00300C3C  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffDGrp$$1Fv@ha
/* 80303D00 00300C40  38 83 A8 0C */	addi r4, r3, __dt__26JSUList$$015JALSeModEffDGrp$$1Fv@l
/* 80303D04 00300C44  38 6D 8E FC */	addi r3, r13, smList__26JALList$$015JALSeModEffDGrp$$1-_SDA_BASE_
/* 80303D08 00300C48  38 BF 00 78 */	addi r5, r31, 0x78
/* 80303D0C 00300C4C  4B D7 EA 1D */	bl __register_global_object
/* 80303D10 00300C50  38 00 00 01 */	li r0, 1
/* 80303D14 00300C54  98 0D 8F 8C */	stb r0, __init__smList__26JALList$$015JALSeModEffDGrp$$1-_SDA_BASE_(r13)
lbl_80303D18:
/* 80303D18 00300C58  88 0D 8F 8D */	lbz r0, __init__smList__26JALList$$015JALSeModPitDGrp$$1-_SDA_BASE_(r13)
/* 80303D1C 00300C5C  7C 00 07 75 */	extsb. r0, r0
/* 80303D20 00300C60  40 82 00 28 */	bne lbl_80303D48
/* 80303D24 00300C64  38 6D 8F 08 */	addi r3, r13, smList__26JALList$$015JALSeModPitDGrp$$1-_SDA_BASE_
/* 80303D28 00300C68  4B D0 AF 0D */	bl initiate__10JSUPtrListFv
/* 80303D2C 00300C6C  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitDGrp$$1Fv@ha
/* 80303D30 00300C70  38 83 A7 B4 */	addi r4, r3, __dt__26JSUList$$015JALSeModPitDGrp$$1Fv@l
/* 80303D34 00300C74  38 6D 8F 08 */	addi r3, r13, smList__26JALList$$015JALSeModPitDGrp$$1-_SDA_BASE_
/* 80303D38 00300C78  38 BF 00 84 */	addi r5, r31, 0x84
/* 80303D3C 00300C7C  4B D7 E9 ED */	bl __register_global_object
/* 80303D40 00300C80  38 00 00 01 */	li r0, 1
/* 80303D44 00300C84  98 0D 8F 8D */	stb r0, __init__smList__26JALList$$015JALSeModPitDGrp$$1-_SDA_BASE_(r13)
lbl_80303D48:
/* 80303D48 00300C88  88 0D 8F 8E */	lbz r0, __init__smList__26JALList$$015JALSeModVolDGrp$$1-_SDA_BASE_(r13)
/* 80303D4C 00300C8C  7C 00 07 75 */	extsb. r0, r0
/* 80303D50 00300C90  40 82 00 28 */	bne lbl_80303D78
/* 80303D54 00300C94  38 6D 8F 14 */	addi r3, r13, smList__26JALList$$015JALSeModVolDGrp$$1-_SDA_BASE_
/* 80303D58 00300C98  4B D0 AE DD */	bl initiate__10JSUPtrListFv
/* 80303D5C 00300C9C  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolDGrp$$1Fv@ha
/* 80303D60 00300CA0  38 83 A7 5C */	addi r4, r3, __dt__26JSUList$$015JALSeModVolDGrp$$1Fv@l
/* 80303D64 00300CA4  38 6D 8F 14 */	addi r3, r13, smList__26JALList$$015JALSeModVolDGrp$$1-_SDA_BASE_
/* 80303D68 00300CA8  38 BF 00 90 */	addi r5, r31, 0x90
/* 80303D6C 00300CAC  4B D7 E9 BD */	bl __register_global_object
/* 80303D70 00300CB0  38 00 00 01 */	li r0, 1
/* 80303D74 00300CB4  98 0D 8F 8E */	stb r0, __init__smList__26JALList$$015JALSeModVolDGrp$$1-_SDA_BASE_(r13)
lbl_80303D78:
/* 80303D78 00300CB8  88 0D 8F 8F */	lbz r0, __init__smList__26JALList$$015JALSeModEffFGrp$$1-_SDA_BASE_(r13)
/* 80303D7C 00300CBC  7C 00 07 75 */	extsb. r0, r0
/* 80303D80 00300CC0  40 82 00 28 */	bne lbl_80303DA8
/* 80303D84 00300CC4  38 6D 8F 20 */	addi r3, r13, smList__26JALList$$015JALSeModEffFGrp$$1-_SDA_BASE_
/* 80303D88 00300CC8  4B D0 AE AD */	bl initiate__10JSUPtrListFv
/* 80303D8C 00300CCC  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffFGrp$$1Fv@ha
/* 80303D90 00300CD0  38 83 A7 04 */	addi r4, r3, __dt__26JSUList$$015JALSeModEffFGrp$$1Fv@l
/* 80303D94 00300CD4  38 6D 8F 20 */	addi r3, r13, smList__26JALList$$015JALSeModEffFGrp$$1-_SDA_BASE_
/* 80303D98 00300CD8  38 BF 00 9C */	addi r5, r31, 0x9c
/* 80303D9C 00300CDC  4B D7 E9 8D */	bl __register_global_object
/* 80303DA0 00300CE0  38 00 00 01 */	li r0, 1
/* 80303DA4 00300CE4  98 0D 8F 8F */	stb r0, __init__smList__26JALList$$015JALSeModEffFGrp$$1-_SDA_BASE_(r13)
lbl_80303DA8:
/* 80303DA8 00300CE8  88 0D 8F 90 */	lbz r0, __init__smList__26JALList$$015JALSeModPitFGrp$$1-_SDA_BASE_(r13)
/* 80303DAC 00300CEC  7C 00 07 75 */	extsb. r0, r0
/* 80303DB0 00300CF0  40 82 00 28 */	bne lbl_80303DD8
/* 80303DB4 00300CF4  38 6D 8F 2C */	addi r3, r13, smList__26JALList$$015JALSeModPitFGrp$$1-_SDA_BASE_
/* 80303DB8 00300CF8  4B D0 AE 7D */	bl initiate__10JSUPtrListFv
/* 80303DBC 00300CFC  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitFGrp$$1Fv@ha
/* 80303DC0 00300D00  38 83 A6 AC */	addi r4, r3, __dt__26JSUList$$015JALSeModPitFGrp$$1Fv@l
/* 80303DC4 00300D04  38 6D 8F 2C */	addi r3, r13, smList__26JALList$$015JALSeModPitFGrp$$1-_SDA_BASE_
/* 80303DC8 00300D08  38 BF 00 A8 */	addi r5, r31, 0xa8
/* 80303DCC 00300D0C  4B D7 E9 5D */	bl __register_global_object
/* 80303DD0 00300D10  38 00 00 01 */	li r0, 1
/* 80303DD4 00300D14  98 0D 8F 90 */	stb r0, __init__smList__26JALList$$015JALSeModPitFGrp$$1-_SDA_BASE_(r13)
lbl_80303DD8:
/* 80303DD8 00300D18  88 0D 8F 91 */	lbz r0, __init__smList__26JALList$$015JALSeModVolFGrp$$1-_SDA_BASE_(r13)
/* 80303DDC 00300D1C  7C 00 07 75 */	extsb. r0, r0
/* 80303DE0 00300D20  40 82 00 28 */	bne lbl_80303E08
/* 80303DE4 00300D24  38 6D 8F 38 */	addi r3, r13, smList__26JALList$$015JALSeModVolFGrp$$1-_SDA_BASE_
/* 80303DE8 00300D28  4B D0 AE 4D */	bl initiate__10JSUPtrListFv
/* 80303DEC 00300D2C  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolFGrp$$1Fv@ha
/* 80303DF0 00300D30  38 83 A6 54 */	addi r4, r3, __dt__26JSUList$$015JALSeModVolFGrp$$1Fv@l
/* 80303DF4 00300D34  38 6D 8F 38 */	addi r3, r13, smList__26JALList$$015JALSeModVolFGrp$$1-_SDA_BASE_
/* 80303DF8 00300D38  38 BF 00 B4 */	addi r5, r31, 0xb4
/* 80303DFC 00300D3C  4B D7 E9 2D */	bl __register_global_object
/* 80303E00 00300D40  38 00 00 01 */	li r0, 1
/* 80303E04 00300D44  98 0D 8F 91 */	stb r0, __init__smList__26JALList$$015JALSeModVolFGrp$$1-_SDA_BASE_(r13)
lbl_80303E08:
/* 80303E08 00300D48  88 0D 8F 92 */	lbz r0, __init__smList__26JALList$$015JALSeModEffDist$$1-_SDA_BASE_(r13)
/* 80303E0C 00300D4C  7C 00 07 75 */	extsb. r0, r0
/* 80303E10 00300D50  40 82 00 28 */	bne lbl_80303E38
/* 80303E14 00300D54  38 6D 8F 44 */	addi r3, r13, smList__26JALList$$015JALSeModEffDist$$1-_SDA_BASE_
/* 80303E18 00300D58  4B D0 AE 1D */	bl initiate__10JSUPtrListFv
/* 80303E1C 00300D5C  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffDist$$1Fv@ha
/* 80303E20 00300D60  38 83 A5 FC */	addi r4, r3, __dt__26JSUList$$015JALSeModEffDist$$1Fv@l
/* 80303E24 00300D64  38 6D 8F 44 */	addi r3, r13, smList__26JALList$$015JALSeModEffDist$$1-_SDA_BASE_
/* 80303E28 00300D68  38 BF 00 C0 */	addi r5, r31, 0xc0
/* 80303E2C 00300D6C  4B D7 E8 FD */	bl __register_global_object
/* 80303E30 00300D70  38 00 00 01 */	li r0, 1
/* 80303E34 00300D74  98 0D 8F 92 */	stb r0, __init__smList__26JALList$$015JALSeModEffDist$$1-_SDA_BASE_(r13)
lbl_80303E38:
/* 80303E38 00300D78  88 0D 8F 93 */	lbz r0, __init__smList__26JALList$$015JALSeModPitDist$$1-_SDA_BASE_(r13)
/* 80303E3C 00300D7C  7C 00 07 75 */	extsb. r0, r0
/* 80303E40 00300D80  40 82 00 28 */	bne lbl_80303E68
/* 80303E44 00300D84  38 6D 8F 50 */	addi r3, r13, smList__26JALList$$015JALSeModPitDist$$1-_SDA_BASE_
/* 80303E48 00300D88  4B D0 AD ED */	bl initiate__10JSUPtrListFv
/* 80303E4C 00300D8C  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitDist$$1Fv@ha
/* 80303E50 00300D90  38 83 A5 A4 */	addi r4, r3, __dt__26JSUList$$015JALSeModPitDist$$1Fv@l
/* 80303E54 00300D94  38 6D 8F 50 */	addi r3, r13, smList__26JALList$$015JALSeModPitDist$$1-_SDA_BASE_
/* 80303E58 00300D98  38 BF 00 CC */	addi r5, r31, 0xcc
/* 80303E5C 00300D9C  4B D7 E8 CD */	bl __register_global_object
/* 80303E60 00300DA0  38 00 00 01 */	li r0, 1
/* 80303E64 00300DA4  98 0D 8F 93 */	stb r0, __init__smList__26JALList$$015JALSeModPitDist$$1-_SDA_BASE_(r13)
lbl_80303E68:
/* 80303E68 00300DA8  88 0D 8F 94 */	lbz r0, __init__smList__26JALList$$015JALSeModVolDist$$1-_SDA_BASE_(r13)
/* 80303E6C 00300DAC  7C 00 07 75 */	extsb. r0, r0
/* 80303E70 00300DB0  40 82 00 28 */	bne lbl_80303E98
/* 80303E74 00300DB4  38 6D 8F 5C */	addi r3, r13, smList__26JALList$$015JALSeModVolDist$$1-_SDA_BASE_
/* 80303E78 00300DB8  4B D0 AD BD */	bl initiate__10JSUPtrListFv
/* 80303E7C 00300DBC  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolDist$$1Fv@ha
/* 80303E80 00300DC0  38 83 A5 4C */	addi r4, r3, __dt__26JSUList$$015JALSeModVolDist$$1Fv@l
/* 80303E84 00300DC4  38 6D 8F 5C */	addi r3, r13, smList__26JALList$$015JALSeModVolDist$$1-_SDA_BASE_
/* 80303E88 00300DC8  38 BF 00 D8 */	addi r5, r31, 0xd8
/* 80303E8C 00300DCC  4B D7 E8 9D */	bl __register_global_object
/* 80303E90 00300DD0  38 00 00 01 */	li r0, 1
/* 80303E94 00300DD4  98 0D 8F 94 */	stb r0, __init__smList__26JALList$$015JALSeModVolDist$$1-_SDA_BASE_(r13)
lbl_80303E98:
/* 80303E98 00300DD8  88 0D 8F 95 */	lbz r0, __init__smList__26JALList$$015JALSeModEffFunk$$1-_SDA_BASE_(r13)
/* 80303E9C 00300DDC  7C 00 07 75 */	extsb. r0, r0
/* 80303EA0 00300DE0  40 82 00 28 */	bne lbl_80303EC8
/* 80303EA4 00300DE4  38 6D 8F 68 */	addi r3, r13, smList__26JALList$$015JALSeModEffFunk$$1-_SDA_BASE_
/* 80303EA8 00300DE8  4B D0 AD 8D */	bl initiate__10JSUPtrListFv
/* 80303EAC 00300DEC  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffFunk$$1Fv@ha
/* 80303EB0 00300DF0  38 83 A4 F4 */	addi r4, r3, __dt__26JSUList$$015JALSeModEffFunk$$1Fv@l
/* 80303EB4 00300DF4  38 6D 8F 68 */	addi r3, r13, smList__26JALList$$015JALSeModEffFunk$$1-_SDA_BASE_
/* 80303EB8 00300DF8  38 BF 00 E4 */	addi r5, r31, 0xe4
/* 80303EBC 00300DFC  4B D7 E8 6D */	bl __register_global_object
/* 80303EC0 00300E00  38 00 00 01 */	li r0, 1
/* 80303EC4 00300E04  98 0D 8F 95 */	stb r0, __init__smList__26JALList$$015JALSeModEffFunk$$1-_SDA_BASE_(r13)
lbl_80303EC8:
/* 80303EC8 00300E08  88 0D 8F 96 */	lbz r0, __init__smList__26JALList$$015JALSeModPitFunk$$1-_SDA_BASE_(r13)
/* 80303ECC 00300E0C  7C 00 07 75 */	extsb. r0, r0
/* 80303ED0 00300E10  40 82 00 28 */	bne lbl_80303EF8
/* 80303ED4 00300E14  38 6D 8F 74 */	addi r3, r13, smList__26JALList$$015JALSeModPitFunk$$1-_SDA_BASE_
/* 80303ED8 00300E18  4B D0 AD 5D */	bl initiate__10JSUPtrListFv
/* 80303EDC 00300E1C  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitFunk$$1Fv@ha
/* 80303EE0 00300E20  38 83 A4 9C */	addi r4, r3, __dt__26JSUList$$015JALSeModPitFunk$$1Fv@l
/* 80303EE4 00300E24  38 6D 8F 74 */	addi r3, r13, smList__26JALList$$015JALSeModPitFunk$$1-_SDA_BASE_
/* 80303EE8 00300E28  38 BF 00 F0 */	addi r5, r31, 0xf0
/* 80303EEC 00300E2C  4B D7 E8 3D */	bl __register_global_object
/* 80303EF0 00300E30  38 00 00 01 */	li r0, 1
/* 80303EF4 00300E34  98 0D 8F 96 */	stb r0, __init__smList__26JALList$$015JALSeModPitFunk$$1-_SDA_BASE_(r13)
lbl_80303EF8:
/* 80303EF8 00300E38  88 0D 8F 97 */	lbz r0, __init__smList__26JALList$$015JALSeModVolFunk$$1-_SDA_BASE_(r13)
/* 80303EFC 00300E3C  7C 00 07 75 */	extsb. r0, r0
/* 80303F00 00300E40  40 82 00 28 */	bne lbl_80303F28
/* 80303F04 00300E44  38 6D 8F 80 */	addi r3, r13, smList__26JALList$$015JALSeModVolFunk$$1-_SDA_BASE_
/* 80303F08 00300E48  4B D0 AD 2D */	bl initiate__10JSUPtrListFv
/* 80303F0C 00300E4C  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolFunk$$1Fv@ha
/* 80303F10 00300E50  38 83 A4 44 */	addi r4, r3, __dt__26JSUList$$015JALSeModVolFunk$$1Fv@l
/* 80303F14 00300E54  38 6D 8F 80 */	addi r3, r13, smList__26JALList$$015JALSeModVolFunk$$1-_SDA_BASE_
/* 80303F18 00300E58  38 BF 00 FC */	addi r5, r31, 0xfc
/* 80303F1C 00300E5C  4B D7 E8 0D */	bl __register_global_object
/* 80303F20 00300E60  38 00 00 01 */	li r0, 1
/* 80303F24 00300E64  98 0D 8F 97 */	stb r0, __init__smList__26JALList$$015JALSeModVolFunk$$1-_SDA_BASE_(r13)
lbl_80303F28:
/* 80303F28 00300E68  80 01 00 14 */	lwz r0, 0x14(r1)
/* 80303F2C 00300E6C  83 E1 00 0C */	lwz r31, 0xc(r1)
/* 80303F30 00300E70  38 21 00 10 */	addi r1, r1, 0x10
/* 80303F34 00300E74  7C 08 03 A6 */	mtlr r0
/* 80303F38 00300E78  4E 80 00 20 */	blr 
