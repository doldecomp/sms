.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global __dt__19TMapEventSirenaSinkFv
__dt__19TMapEventSirenaSinkFv:
/* 801F21D8 001EF118  7C 08 02 A6 */	mflr r0
/* 801F21DC 001EF11C  90 01 00 04 */	stw r0, 4(r1)
/* 801F21E0 001EF120  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 801F21E4 001EF124  93 E1 00 14 */	stw r31, 0x14(r1)
/* 801F21E8 001EF128  3B E4 00 00 */	addi r31, r4, 0
/* 801F21EC 001EF12C  93 C1 00 10 */	stw r30, 0x10(r1)
/* 801F21F0 001EF130  7C 7E 1B 79 */	or. r30, r3, r3
/* 801F21F4 001EF134  41 82 00 5C */	beq lbl_801F2250
/* 801F21F8 001EF138  3C 60 80 3D */	lis r3, __vt__19TMapEventSirenaSink@ha
/* 801F21FC 001EF13C  38 03 06 48 */	addi r0, r3, __vt__19TMapEventSirenaSink@l
/* 801F2200 001EF140  90 1E 00 00 */	stw r0, 0(r30)
/* 801F2204 001EF144  41 82 00 3C */	beq lbl_801F2240
/* 801F2208 001EF148  3C 60 80 3D */	lis r3, __vt__13TMapEventSink@ha
/* 801F220C 001EF14C  38 03 FE D0 */	addi r0, r3, __vt__13TMapEventSink@l
/* 801F2210 001EF150  90 1E 00 00 */	stw r0, 0(r30)
/* 801F2214 001EF154  41 82 00 2C */	beq lbl_801F2240
/* 801F2218 001EF158  3C 60 80 3D */	lis r3, __vt__9TMapEvent@ha
/* 801F221C 001EF15C  38 03 FD 40 */	addi r0, r3, __vt__9TMapEvent@l
/* 801F2220 001EF160  90 1E 00 00 */	stw r0, 0(r30)
/* 801F2224 001EF164  41 82 00 1C */	beq lbl_801F2240
/* 801F2228 001EF168  3C 60 80 3B */	lis r3, __vt__13TEventWatcher@ha
/* 801F222C 001EF16C  38 03 39 40 */	addi r0, r3, __vt__13TEventWatcher@l
/* 801F2230 001EF170  90 1E 00 00 */	stw r0, 0(r30)
/* 801F2234 001EF174  38 7E 00 00 */	addi r3, r30, 0
/* 801F2238 001EF178  38 80 00 00 */	li r4, 0
/* 801F223C 001EF17C  4B E4 E8 61 */	bl __dt__Q26JDrama8TViewObjFv
lbl_801F2240:
/* 801F2240 001EF180  7F E0 07 35 */	extsh. r0, r31
/* 801F2244 001EF184  40 81 00 0C */	ble lbl_801F2250
/* 801F2248 001EF188  7F C3 F3 78 */	mr r3, r30
/* 801F224C 001EF18C  4B E1 A8 65 */	bl __dl__FPv
lbl_801F2250:
/* 801F2250 001EF190  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 801F2254 001EF194  7F C3 F3 78 */	mr r3, r30
/* 801F2258 001EF198  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 801F225C 001EF19C  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 801F2260 001EF1A0  7C 08 03 A6 */	mtlr r0
/* 801F2264 001EF1A4  38 21 00 18 */	addi r1, r1, 0x18
/* 801F2268 001EF1A8  4E 80 00 20 */	blr 

.global initPollution__19TMapEventSirenaSinkFv
initPollution__19TMapEventSirenaSinkFv:
/* 801F226C 001EF1AC  4E 80 00 20 */	blr 

.global makePollutionRecovered__19TMapEventSirenaSinkFUl
makePollutionRecovered__19TMapEventSirenaSinkFUl:
/* 801F2270 001EF1B0  4E 80 00 20 */	blr 

.global __ct__19TMapEventSirenaSinkFPCc
__ct__19TMapEventSirenaSinkFPCc:
/* 801F2274 001EF1B4  7C 08 02 A6 */	mflr r0
/* 801F2278 001EF1B8  90 01 00 04 */	stw r0, 4(r1)
/* 801F227C 001EF1BC  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 801F2280 001EF1C0  93 E1 00 14 */	stw r31, 0x14(r1)
/* 801F2284 001EF1C4  7C 7F 1B 78 */	mr r31, r3
/* 801F2288 001EF1C8  4B FF 00 85 */	bl __ct__13TMapEventSinkFPCc
/* 801F228C 001EF1CC  3C 60 80 3D */	lis r3, __vt__19TMapEventSirenaSink@ha
/* 801F2290 001EF1D0  38 03 06 48 */	addi r0, r3, __vt__19TMapEventSirenaSink@l
/* 801F2294 001EF1D4  90 1F 00 00 */	stw r0, 0(r31)
/* 801F2298 001EF1D8  38 00 00 00 */	li r0, 0
/* 801F229C 001EF1DC  38 7F 00 00 */	addi r3, r31, 0
/* 801F22A0 001EF1E0  98 1F 00 64 */	stb r0, 0x64(r31)
/* 801F22A4 001EF1E4  C0 02 D0 D0 */	lfs f0, $$22744@sda21(r2)
/* 801F22A8 001EF1E8  D0 1F 00 80 */	stfs f0, 0x80(r31)
/* 801F22AC 001EF1EC  D0 1F 00 70 */	stfs f0, 0x70(r31)
/* 801F22B0 001EF1F0  D0 1F 00 6C */	stfs f0, 0x6c(r31)
/* 801F22B4 001EF1F4  D0 1F 00 68 */	stfs f0, 0x68(r31)
/* 801F22B8 001EF1F8  D0 1F 00 7C */	stfs f0, 0x7c(r31)
/* 801F22BC 001EF1FC  D0 1F 00 78 */	stfs f0, 0x78(r31)
/* 801F22C0 001EF200  D0 1F 00 74 */	stfs f0, 0x74(r31)
/* 801F22C4 001EF204  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 801F22C8 001EF208  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 801F22CC 001EF20C  38 21 00 18 */	addi r1, r1, 0x18
/* 801F22D0 001EF210  7C 08 03 A6 */	mtlr r0
/* 801F22D4 001EF214  4E 80 00 20 */	blr 

.global load__19TMapEventSirenaSinkFR20JSUMemoryInputStream
load__19TMapEventSirenaSinkFR20JSUMemoryInputStream:
/* 801F22D8 001EF218  7C 08 02 A6 */	mflr r0
/* 801F22DC 001EF21C  90 01 00 04 */	stw r0, 4(r1)
/* 801F22E0 001EF220  94 21 FF D0 */	stwu r1, -0x30(r1)
/* 801F22E4 001EF224  93 E1 00 2C */	stw r31, 0x2c(r1)
/* 801F22E8 001EF228  3B E4 00 00 */	addi r31, r4, 0
/* 801F22EC 001EF22C  93 C1 00 28 */	stw r30, 0x28(r1)
/* 801F22F0 001EF230  3B C3 00 00 */	addi r30, r3, 0
/* 801F22F4 001EF234  4B FF 00 B5 */	bl load__13TMapEventSinkFR20JSUMemoryInputStream
/* 801F22F8 001EF238  7F E3 FB 78 */	mr r3, r31
/* 801F22FC 001EF23C  4B E1 C2 ED */	bl readString__14JSUInputStreamFv
/* 801F2300 001EF240  38 7F 00 00 */	addi r3, r31, 0
/* 801F2304 001EF244  38 9E 00 74 */	addi r4, r30, 0x74
/* 801F2308 001EF248  38 A0 00 04 */	li r5, 4
/* 801F230C 001EF24C  4B E1 C2 85 */	bl read__14JSUInputStreamFPvl
/* 801F2310 001EF250  38 7F 00 00 */	addi r3, r31, 0
/* 801F2314 001EF254  38 9E 00 78 */	addi r4, r30, 0x78
/* 801F2318 001EF258  38 A0 00 04 */	li r5, 4
/* 801F231C 001EF25C  4B E1 C2 75 */	bl read__14JSUInputStreamFPvl
/* 801F2320 001EF260  38 7F 00 00 */	addi r3, r31, 0
/* 801F2324 001EF264  38 9E 00 7C */	addi r4, r30, 0x7c
/* 801F2328 001EF268  38 A0 00 04 */	li r5, 4
/* 801F232C 001EF26C  4B E1 C2 65 */	bl read__14JSUInputStreamFPvl
/* 801F2330 001EF270  38 7F 00 00 */	addi r3, r31, 0
/* 801F2334 001EF274  38 81 00 20 */	addi r4, r1, 0x20
/* 801F2338 001EF278  38 A0 00 04 */	li r5, 4
/* 801F233C 001EF27C  4B E1 C2 55 */	bl read__14JSUInputStreamFPvl
/* 801F2340 001EF280  38 7F 00 00 */	addi r3, r31, 0
/* 801F2344 001EF284  38 9E 00 80 */	addi r4, r30, 0x80
/* 801F2348 001EF288  38 A0 00 04 */	li r5, 4
/* 801F234C 001EF28C  4B E1 C2 45 */	bl read__14JSUInputStreamFPvl
/* 801F2350 001EF290  3C 60 80 3F */	lis r3, gParticleFlagLoaded@ha
/* 801F2354 001EF294  38 63 28 10 */	addi r3, r3, gParticleFlagLoaded@l
/* 801F2358 001EF298  3B E3 00 68 */	addi r31, r3, 0x68
/* 801F235C 001EF29C  88 03 00 68 */	lbz r0, 0x68(r3)
/* 801F2360 001EF2A0  28 00 00 00 */	cmplwi r0, 0
/* 801F2364 001EF2A4  40 82 00 20 */	bne lbl_801F2384
/* 801F2368 001EF2A8  3C 80 80 39 */	lis r4, $$22771@ha
/* 801F236C 001EF2AC  80 6D 98 50 */	lwz r3, gpResourceManager@sda21(r13)
/* 801F2370 001EF2B0  38 84 2B A8 */	addi r4, r4, $$22771@l
/* 801F2374 001EF2B4  38 A0 00 68 */	li r5, 0x68
/* 801F2378 001EF2B8  4B E8 EC B5 */	bl load__18JPAResourceManagerFPCcUs
/* 801F237C 001EF2BC  38 00 00 01 */	li r0, 1
/* 801F2380 001EF2C0  98 1F 00 00 */	stb r0, 0(r31)
lbl_801F2384:
/* 801F2384 001EF2C4  3C 60 80 3F */	lis r3, gParticleFlagLoaded@ha
/* 801F2388 001EF2C8  38 63 28 10 */	addi r3, r3, gParticleFlagLoaded@l
/* 801F238C 001EF2CC  3B E3 01 E4 */	addi r31, r3, 0x1e4
/* 801F2390 001EF2D0  88 03 01 E4 */	lbz r0, 0x1e4(r3)
/* 801F2394 001EF2D4  28 00 00 00 */	cmplwi r0, 0
/* 801F2398 001EF2D8  40 82 00 20 */	bne lbl_801F23B8
/* 801F239C 001EF2DC  3C 80 80 39 */	lis r4, $$22772@ha
/* 801F23A0 001EF2E0  80 6D 98 50 */	lwz r3, gpResourceManager@sda21(r13)
/* 801F23A4 001EF2E4  38 84 2B CC */	addi r4, r4, $$22772@l
/* 801F23A8 001EF2E8  38 A0 01 E4 */	li r5, 0x1e4
/* 801F23AC 001EF2EC  4B E8 EC 81 */	bl load__18JPAResourceManagerFPCcUs
/* 801F23B0 001EF2F0  38 00 00 01 */	li r0, 1
/* 801F23B4 001EF2F4  98 1F 00 00 */	stb r0, 0(r31)
lbl_801F23B8:
/* 801F23B8 001EF2F8  80 01 00 34 */	lwz r0, 0x34(r1)
/* 801F23BC 001EF2FC  83 E1 00 2C */	lwz r31, 0x2c(r1)
/* 801F23C0 001EF300  83 C1 00 28 */	lwz r30, 0x28(r1)
/* 801F23C4 001EF304  7C 08 03 A6 */	mtlr r0
/* 801F23C8 001EF308  38 21 00 30 */	addi r1, r1, 0x30
/* 801F23CC 001EF30C  4E 80 00 20 */	blr 

.global loadAfter__19TMapEventSirenaSinkFv
loadAfter__19TMapEventSirenaSinkFv:
/* 801F23D0 001EF310  7C 08 02 A6 */	mflr r0
/* 801F23D4 001EF314  90 01 00 04 */	stw r0, 4(r1)
/* 801F23D8 001EF318  94 21 FF C0 */	stwu r1, -0x40(r1)
/* 801F23DC 001EF31C  93 E1 00 3C */	stw r31, 0x3c(r1)
/* 801F23E0 001EF320  93 C1 00 38 */	stw r30, 0x38(r1)
/* 801F23E4 001EF324  93 A1 00 34 */	stw r29, 0x34(r1)
/* 801F23E8 001EF328  7C 7D 1B 78 */	mr r29, r3
/* 801F23EC 001EF32C  4B E5 24 C1 */	bl loadAfter__Q26JDrama8TNameRefFv
/* 801F23F0 001EF330  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 801F23F4 001EF334  3C 60 80 39 */	lis r3, $$22784@ha
/* 801F23F8 001EF338  3B E3 2B F0 */	addi r31, r3, $$22784@l
/* 801F23FC 001EF33C  83 C4 00 04 */	lwz r30, 4(r4)
/* 801F2400 001EF340  7F E3 FB 78 */	mr r3, r31
/* 801F2404 001EF344  4B E5 22 89 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 801F2408 001EF348  81 9E 00 00 */	lwz r12, 0(r30)
/* 801F240C 001EF34C  38 83 00 00 */	addi r4, r3, 0
/* 801F2410 001EF350  38 7E 00 00 */	addi r3, r30, 0
/* 801F2414 001EF354  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 801F2418 001EF358  38 BF 00 00 */	addi r5, r31, 0
/* 801F241C 001EF35C  7D 88 03 A6 */	mtlr r12
/* 801F2420 001EF360  4E 80 00 21 */	blrl 
/* 801F2424 001EF364  80 63 00 2C */	lwz r3, 0x2c(r3)
/* 801F2428 001EF368  38 00 00 F0 */	li r0, 0xf0
/* 801F242C 001EF36C  90 7D 00 40 */	stw r3, 0x40(r29)
/* 801F2430 001EF370  90 1D 00 44 */	stw r0, 0x44(r29)
/* 801F2434 001EF374  90 1D 00 48 */	stw r0, 0x48(r29)
/* 801F2438 001EF378  C0 02 D0 D4 */	lfs f0, $$22785@sda21(r2)
/* 801F243C 001EF37C  D0 1D 00 38 */	stfs f0, 0x38(r29)
/* 801F2440 001EF380  C0 02 D0 D0 */	lfs f0, $$22744@sda21(r2)
/* 801F2444 001EF384  D0 1D 00 68 */	stfs f0, 0x68(r29)
/* 801F2448 001EF388  C0 02 D0 D8 */	lfs f0, $$22786@sda21(r2)
/* 801F244C 001EF38C  D0 1D 00 6C */	stfs f0, 0x6c(r29)
/* 801F2450 001EF390  C0 02 D0 DC */	lfs f0, $$22787@sda21(r2)
/* 801F2454 001EF394  D0 1D 00 70 */	stfs f0, 0x70(r29)
/* 801F2458 001EF398  80 01 00 44 */	lwz r0, 0x44(r1)
/* 801F245C 001EF39C  83 E1 00 3C */	lwz r31, 0x3c(r1)
/* 801F2460 001EF3A0  83 C1 00 38 */	lwz r30, 0x38(r1)
/* 801F2464 001EF3A4  83 A1 00 34 */	lwz r29, 0x34(r1)
/* 801F2468 001EF3A8  38 21 00 40 */	addi r1, r1, 0x40
/* 801F246C 001EF3AC  7C 08 03 A6 */	mtlr r0
/* 801F2470 001EF3B0  4E 80 00 20 */	blr 

.global watch__19TMapEventSirenaSinkFv
watch__19TMapEventSirenaSinkFv:
/* 801F2474 001EF3B4  7C 08 02 A6 */	mflr r0
/* 801F2478 001EF3B8  90 01 00 04 */	stw r0, 4(r1)
/* 801F247C 001EF3BC  94 21 FF B0 */	stwu r1, -0x50(r1)
/* 801F2480 001EF3C0  93 E1 00 4C */	stw r31, 0x4c(r1)
/* 801F2484 001EF3C4  93 C1 00 48 */	stw r30, 0x48(r1)
/* 801F2488 001EF3C8  7C 7E 1B 78 */	mr r30, r3
/* 801F248C 001EF3CC  88 03 00 64 */	lbz r0, 0x64(r3)
/* 801F2490 001EF3D0  3C 60 80 39 */	lis r3, $$21490@ha
/* 801F2494 001EF3D4  3B E3 2A B0 */	addi r31, r3, $$21490@l
/* 801F2498 001EF3D8  28 00 00 00 */	cmplwi r0, 0
/* 801F249C 001EF3DC  41 82 00 D4 */	beq lbl_801F2570
/* 801F24A0 001EF3E0  80 6D 9A E8 */	lwz r3, gpPollution@sda21(r13)
/* 801F24A4 001EF3E4  38 00 00 00 */	li r0, 0
/* 801F24A8 001EF3E8  39 61 00 44 */	addi r11, r1, 0x44
/* 801F24AC 001EF3EC  80 63 00 14 */	lwz r3, 0x14(r3)
/* 801F24B0 001EF3F0  38 9F 01 40 */	addi r4, r31, 0x140
/* 801F24B4 001EF3F4  38 BE 00 68 */	addi r5, r30, 0x68
/* 801F24B8 001EF3F8  81 43 00 00 */	lwz r10, 0(r3)
/* 801F24BC 001EF3FC  38 C0 FF FF */	li r6, -1
/* 801F24C0 001EF400  38 E0 00 01 */	li r7, 1
/* 801F24C4 001EF404  A0 6A 00 32 */	lhz r3, 0x32(r10)
/* 801F24C8 001EF408  39 00 00 00 */	li r8, 0
/* 801F24CC 001EF40C  39 20 00 00 */	li r9, 0
/* 801F24D0 001EF410  60 63 00 02 */	ori r3, r3, 2
/* 801F24D4 001EF414  B0 6A 00 32 */	sth r3, 0x32(r10)
/* 801F24D8 001EF418  39 40 00 00 */	li r10, 0
/* 801F24DC 001EF41C  90 1E 00 28 */	stw r0, 0x28(r30)
/* 801F24E0 001EF420  80 6D 97 E8 */	lwz r3, gpMarDirector@sda21(r13)
/* 801F24E4 001EF424  B0 01 00 44 */	sth r0, 0x44(r1)
/* 801F24E8 001EF428  91 61 00 08 */	stw r11, 8(r1)
/* 801F24EC 001EF42C  C0 22 D0 D0 */	lfs f1, $$22744@sda21(r2)
/* 801F24F0 001EF430  4B EF B2 D1 */	bl fireStartDemoCamera__12TMarDirectorFPCcPCQ29JGeometry8TVec3$$0f$$1lfbPFUlUl_lUlPQ26JDrama6TActorQ26JDrama10TFlagT$$0Us$$1
/* 801F24F4 001EF434  80 6D 9A 18 */	lwz r3, gpItemManager@sda21(r13)
/* 801F24F8 001EF438  38 9F 01 54 */	addi r4, r31, 0x154
/* 801F24FC 001EF43C  C0 3E 00 68 */	lfs f1, 0x68(r30)
/* 801F2500 001EF440  38 BF 01 70 */	addi r5, r31, 0x170
/* 801F2504 001EF444  C0 5E 00 6C */	lfs f2, 0x6c(r30)
/* 801F2508 001EF448  C0 7E 00 70 */	lfs f3, 0x70(r30)
/* 801F250C 001EF44C  4B FA 5A C5 */	bl makeShineAppearWithDemo__12TItemManagerFPCcPCcfff
/* 801F2510 001EF450  3C 80 00 05 */	lis r4, 0x00050008@ha
/* 801F2514 001EF454  80 6D 97 D0 */	lwz r3, smInstance__12TFlagManager@sda21(r13)
/* 801F2518 001EF458  38 A4 00 08 */	addi r5, r4, 0x00050008@l
/* 801F251C 001EF45C  38 80 00 01 */	li r4, 1
/* 801F2520 001EF460  4B EF 59 81 */	bl setBool__12TFlagManagerFbUl
/* 801F2524 001EF464  38 7E 00 74 */	addi r3, r30, 0x74
/* 801F2528 001EF468  C0 3E 00 80 */	lfs f1, 0x80(r30)
/* 801F252C 001EF46C  4B F6 04 E9 */	bl SMS_MarioWarpRequest__FRCQ29JGeometry8TVec3$$0f$$1f
/* 801F2530 001EF470  80 AD 9A 10 */	lwz r5, gpMapObjManager@sda21(r13)
/* 801F2534 001EF474  38 80 00 68 */	li r4, 0x68
/* 801F2538 001EF478  80 6D 97 98 */	lwz r3, gpMarioParticleManager@sda21(r13)
/* 801F253C 001EF47C  38 C0 00 00 */	li r6, 0
/* 801F2540 001EF480  38 A5 00 44 */	addi r5, r5, 0x44
/* 801F2544 001EF484  38 E0 00 00 */	li r7, 0
/* 801F2548 001EF488  4B EE 95 D5 */	bl emit__21TMarioParticleManagerFlPCQ29JGeometry8TVec3$$0f$$1UcPCv
/* 801F254C 001EF48C  80 AD 9A 10 */	lwz r5, gpMapObjManager@sda21(r13)
/* 801F2550 001EF490  38 80 01 E4 */	li r4, 0x1e4
/* 801F2554 001EF494  80 6D 97 98 */	lwz r3, gpMarioParticleManager@sda21(r13)
/* 801F2558 001EF498  38 C0 00 02 */	li r6, 2
/* 801F255C 001EF49C  38 A5 00 44 */	addi r5, r5, 0x44
/* 801F2560 001EF4A0  38 E0 00 00 */	li r7, 0
/* 801F2564 001EF4A4  4B EE 95 B9 */	bl emit__21TMarioParticleManagerFlPCQ29JGeometry8TVec3$$0f$$1UcPCv
/* 801F2568 001EF4A8  38 60 00 01 */	li r3, 1
/* 801F256C 001EF4AC  48 00 00 08 */	b lbl_801F2574
lbl_801F2570:
/* 801F2570 001EF4B0  38 60 00 00 */	li r3, 0
lbl_801F2574:
/* 801F2574 001EF4B4  80 01 00 54 */	lwz r0, 0x54(r1)
/* 801F2578 001EF4B8  83 E1 00 4C */	lwz r31, 0x4c(r1)
/* 801F257C 001EF4BC  83 C1 00 48 */	lwz r30, 0x48(r1)
/* 801F2580 001EF4C0  7C 08 03 A6 */	mtlr r0
/* 801F2584 001EF4C4  38 21 00 50 */	addi r1, r1, 0x50
/* 801F2588 001EF4C8  4E 80 00 20 */	blr 

.global __sinit_MapEventSirena_cpp
__sinit_MapEventSirena_cpp:
/* 801F258C 001EF4CC  7C 08 02 A6 */	mflr r0
/* 801F2590 001EF4D0  3C 60 80 40 */	lis r3, $$22812@ha
/* 801F2594 001EF4D4  90 01 00 04 */	stw r0, 4(r1)
/* 801F2598 001EF4D8  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 801F259C 001EF4DC  93 E1 00 0C */	stw r31, 0xc(r1)
/* 801F25A0 001EF4E0  3B E3 87 00 */	addi r31, r3, $$22812@l
/* 801F25A4 001EF4E4  88 0D 97 CC */	lbz r0, __init__smList__15JALList$$05MSBgm$$1@sda21(r13)
/* 801F25A8 001EF4E8  7C 00 07 75 */	extsb. r0, r0
/* 801F25AC 001EF4EC  40 82 00 28 */	bne lbl_801F25D4
/* 801F25B0 001EF4F0  38 6D 97 A8 */	addi r3, r13, smList__15JALList$$05MSBgm$$1@sda21
/* 801F25B4 001EF4F4  4B E1 C6 81 */	bl initiate__10JSUPtrListFv
/* 801F25B8 001EF4F8  3C 60 80 0E */	lis r3, __dt__15JSUList$$05MSBgm$$1Fv@ha
/* 801F25BC 001EF4FC  38 83 6A 44 */	addi r4, r3, __dt__15JSUList$$05MSBgm$$1Fv@l
/* 801F25C0 001EF500  38 6D 97 A8 */	addi r3, r13, smList__15JALList$$05MSBgm$$1@sda21
/* 801F25C4 001EF504  38 BF 00 00 */	addi r5, r31, 0
/* 801F25C8 001EF508  4B E9 01 61 */	bl __register_global_object
/* 801F25CC 001EF50C  38 00 00 01 */	li r0, 1
/* 801F25D0 001EF510  98 0D 97 CC */	stb r0, __init__smList__15JALList$$05MSBgm$$1@sda21(r13)
lbl_801F25D4:
/* 801F25D4 001EF514  88 0D 97 CD */	lbz r0, __init__smList__24JALList$$013MSSetSoundGrp$$1@sda21(r13)
/* 801F25D8 001EF518  7C 00 07 75 */	extsb. r0, r0
/* 801F25DC 001EF51C  40 82 00 28 */	bne lbl_801F2604
/* 801F25E0 001EF520  38 6D 97 B4 */	addi r3, r13, smList__24JALList$$013MSSetSoundGrp$$1@sda21
/* 801F25E4 001EF524  4B E1 C6 51 */	bl initiate__10JSUPtrListFv
/* 801F25E8 001EF528  3C 60 80 0E */	lis r3, __dt__24JSUList$$013MSSetSoundGrp$$1Fv@ha
/* 801F25EC 001EF52C  38 83 69 EC */	addi r4, r3, __dt__24JSUList$$013MSSetSoundGrp$$1Fv@l
/* 801F25F0 001EF530  38 6D 97 B4 */	addi r3, r13, smList__24JALList$$013MSSetSoundGrp$$1@sda21
/* 801F25F4 001EF534  38 BF 00 0C */	addi r5, r31, 0xc
/* 801F25F8 001EF538  4B E9 01 31 */	bl __register_global_object
/* 801F25FC 001EF53C  38 00 00 01 */	li r0, 1
/* 801F2600 001EF540  98 0D 97 CD */	stb r0, __init__smList__24JALList$$013MSSetSoundGrp$$1@sda21(r13)
lbl_801F2604:
/* 801F2604 001EF544  88 0D 97 CE */	lbz r0, __init__smList__21JALList$$010MSSetSound$$1@sda21(r13)
/* 801F2608 001EF548  7C 00 07 75 */	extsb. r0, r0
/* 801F260C 001EF54C  40 82 00 28 */	bne lbl_801F2634
/* 801F2610 001EF550  38 6D 97 C0 */	addi r3, r13, smList__21JALList$$010MSSetSound$$1@sda21
/* 801F2614 001EF554  4B E1 C6 21 */	bl initiate__10JSUPtrListFv
/* 801F2618 001EF558  3C 60 80 0E */	lis r3, __dt__21JSUList$$010MSSetSound$$1Fv@ha
/* 801F261C 001EF55C  38 83 69 94 */	addi r4, r3, __dt__21JSUList$$010MSSetSound$$1Fv@l
/* 801F2620 001EF560  38 6D 97 C0 */	addi r3, r13, smList__21JALList$$010MSSetSound$$1@sda21
/* 801F2624 001EF564  38 BF 00 18 */	addi r5, r31, 0x18
/* 801F2628 001EF568  4B E9 01 01 */	bl __register_global_object
/* 801F262C 001EF56C  38 00 00 01 */	li r0, 1
/* 801F2630 001EF570  98 0D 97 CE */	stb r0, __init__smList__21JALList$$010MSSetSound$$1@sda21(r13)
lbl_801F2634:
/* 801F2634 001EF574  88 0D 8F 8C */	lbz r0, __init__smList__26JALList$$015JALSeModEffDGrp$$1@sda21(r13)
/* 801F2638 001EF578  7C 00 07 75 */	extsb. r0, r0
/* 801F263C 001EF57C  40 82 00 28 */	bne lbl_801F2664
/* 801F2640 001EF580  38 6D 8E FC */	addi r3, r13, smList__26JALList$$015JALSeModEffDGrp$$1@sda21
/* 801F2644 001EF584  4B E1 C5 F1 */	bl initiate__10JSUPtrListFv
/* 801F2648 001EF588  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffDGrp$$1Fv@ha
/* 801F264C 001EF58C  38 83 A8 0C */	addi r4, r3, __dt__26JSUList$$015JALSeModEffDGrp$$1Fv@l
/* 801F2650 001EF590  38 6D 8E FC */	addi r3, r13, smList__26JALList$$015JALSeModEffDGrp$$1@sda21
/* 801F2654 001EF594  38 BF 00 24 */	addi r5, r31, 0x24
/* 801F2658 001EF598  4B E9 00 D1 */	bl __register_global_object
/* 801F265C 001EF59C  38 00 00 01 */	li r0, 1
/* 801F2660 001EF5A0  98 0D 8F 8C */	stb r0, __init__smList__26JALList$$015JALSeModEffDGrp$$1@sda21(r13)
lbl_801F2664:
/* 801F2664 001EF5A4  88 0D 8F 8D */	lbz r0, __init__smList__26JALList$$015JALSeModPitDGrp$$1@sda21(r13)
/* 801F2668 001EF5A8  7C 00 07 75 */	extsb. r0, r0
/* 801F266C 001EF5AC  40 82 00 28 */	bne lbl_801F2694
/* 801F2670 001EF5B0  38 6D 8F 08 */	addi r3, r13, smList__26JALList$$015JALSeModPitDGrp$$1@sda21
/* 801F2674 001EF5B4  4B E1 C5 C1 */	bl initiate__10JSUPtrListFv
/* 801F2678 001EF5B8  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitDGrp$$1Fv@ha
/* 801F267C 001EF5BC  38 83 A7 B4 */	addi r4, r3, __dt__26JSUList$$015JALSeModPitDGrp$$1Fv@l
/* 801F2680 001EF5C0  38 6D 8F 08 */	addi r3, r13, smList__26JALList$$015JALSeModPitDGrp$$1@sda21
/* 801F2684 001EF5C4  38 BF 00 30 */	addi r5, r31, 0x30
/* 801F2688 001EF5C8  4B E9 00 A1 */	bl __register_global_object
/* 801F268C 001EF5CC  38 00 00 01 */	li r0, 1
/* 801F2690 001EF5D0  98 0D 8F 8D */	stb r0, __init__smList__26JALList$$015JALSeModPitDGrp$$1@sda21(r13)
lbl_801F2694:
/* 801F2694 001EF5D4  88 0D 8F 8E */	lbz r0, __init__smList__26JALList$$015JALSeModVolDGrp$$1@sda21(r13)
/* 801F2698 001EF5D8  7C 00 07 75 */	extsb. r0, r0
/* 801F269C 001EF5DC  40 82 00 28 */	bne lbl_801F26C4
/* 801F26A0 001EF5E0  38 6D 8F 14 */	addi r3, r13, smList__26JALList$$015JALSeModVolDGrp$$1@sda21
/* 801F26A4 001EF5E4  4B E1 C5 91 */	bl initiate__10JSUPtrListFv
/* 801F26A8 001EF5E8  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolDGrp$$1Fv@ha
/* 801F26AC 001EF5EC  38 83 A7 5C */	addi r4, r3, __dt__26JSUList$$015JALSeModVolDGrp$$1Fv@l
/* 801F26B0 001EF5F0  38 6D 8F 14 */	addi r3, r13, smList__26JALList$$015JALSeModVolDGrp$$1@sda21
/* 801F26B4 001EF5F4  38 BF 00 3C */	addi r5, r31, 0x3c
/* 801F26B8 001EF5F8  4B E9 00 71 */	bl __register_global_object
/* 801F26BC 001EF5FC  38 00 00 01 */	li r0, 1
/* 801F26C0 001EF600  98 0D 8F 8E */	stb r0, __init__smList__26JALList$$015JALSeModVolDGrp$$1@sda21(r13)
lbl_801F26C4:
/* 801F26C4 001EF604  88 0D 8F 8F */	lbz r0, __init__smList__26JALList$$015JALSeModEffFGrp$$1@sda21(r13)
/* 801F26C8 001EF608  7C 00 07 75 */	extsb. r0, r0
/* 801F26CC 001EF60C  40 82 00 28 */	bne lbl_801F26F4
/* 801F26D0 001EF610  38 6D 8F 20 */	addi r3, r13, smList__26JALList$$015JALSeModEffFGrp$$1@sda21
/* 801F26D4 001EF614  4B E1 C5 61 */	bl initiate__10JSUPtrListFv
/* 801F26D8 001EF618  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffFGrp$$1Fv@ha
/* 801F26DC 001EF61C  38 83 A7 04 */	addi r4, r3, __dt__26JSUList$$015JALSeModEffFGrp$$1Fv@l
/* 801F26E0 001EF620  38 6D 8F 20 */	addi r3, r13, smList__26JALList$$015JALSeModEffFGrp$$1@sda21
/* 801F26E4 001EF624  38 BF 00 48 */	addi r5, r31, 0x48
/* 801F26E8 001EF628  4B E9 00 41 */	bl __register_global_object
/* 801F26EC 001EF62C  38 00 00 01 */	li r0, 1
/* 801F26F0 001EF630  98 0D 8F 8F */	stb r0, __init__smList__26JALList$$015JALSeModEffFGrp$$1@sda21(r13)
lbl_801F26F4:
/* 801F26F4 001EF634  88 0D 8F 90 */	lbz r0, __init__smList__26JALList$$015JALSeModPitFGrp$$1@sda21(r13)
/* 801F26F8 001EF638  7C 00 07 75 */	extsb. r0, r0
/* 801F26FC 001EF63C  40 82 00 28 */	bne lbl_801F2724
/* 801F2700 001EF640  38 6D 8F 2C */	addi r3, r13, smList__26JALList$$015JALSeModPitFGrp$$1@sda21
/* 801F2704 001EF644  4B E1 C5 31 */	bl initiate__10JSUPtrListFv
/* 801F2708 001EF648  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitFGrp$$1Fv@ha
/* 801F270C 001EF64C  38 83 A6 AC */	addi r4, r3, __dt__26JSUList$$015JALSeModPitFGrp$$1Fv@l
/* 801F2710 001EF650  38 6D 8F 2C */	addi r3, r13, smList__26JALList$$015JALSeModPitFGrp$$1@sda21
/* 801F2714 001EF654  38 BF 00 54 */	addi r5, r31, 0x54
/* 801F2718 001EF658  4B E9 00 11 */	bl __register_global_object
/* 801F271C 001EF65C  38 00 00 01 */	li r0, 1
/* 801F2720 001EF660  98 0D 8F 90 */	stb r0, __init__smList__26JALList$$015JALSeModPitFGrp$$1@sda21(r13)
lbl_801F2724:
/* 801F2724 001EF664  88 0D 8F 91 */	lbz r0, __init__smList__26JALList$$015JALSeModVolFGrp$$1@sda21(r13)
/* 801F2728 001EF668  7C 00 07 75 */	extsb. r0, r0
/* 801F272C 001EF66C  40 82 00 28 */	bne lbl_801F2754
/* 801F2730 001EF670  38 6D 8F 38 */	addi r3, r13, smList__26JALList$$015JALSeModVolFGrp$$1@sda21
/* 801F2734 001EF674  4B E1 C5 01 */	bl initiate__10JSUPtrListFv
/* 801F2738 001EF678  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolFGrp$$1Fv@ha
/* 801F273C 001EF67C  38 83 A6 54 */	addi r4, r3, __dt__26JSUList$$015JALSeModVolFGrp$$1Fv@l
/* 801F2740 001EF680  38 6D 8F 38 */	addi r3, r13, smList__26JALList$$015JALSeModVolFGrp$$1@sda21
/* 801F2744 001EF684  38 BF 00 60 */	addi r5, r31, 0x60
/* 801F2748 001EF688  4B E8 FF E1 */	bl __register_global_object
/* 801F274C 001EF68C  38 00 00 01 */	li r0, 1
/* 801F2750 001EF690  98 0D 8F 91 */	stb r0, __init__smList__26JALList$$015JALSeModVolFGrp$$1@sda21(r13)
lbl_801F2754:
/* 801F2754 001EF694  88 0D 8F 92 */	lbz r0, __init__smList__26JALList$$015JALSeModEffDist$$1@sda21(r13)
/* 801F2758 001EF698  7C 00 07 75 */	extsb. r0, r0
/* 801F275C 001EF69C  40 82 00 28 */	bne lbl_801F2784
/* 801F2760 001EF6A0  38 6D 8F 44 */	addi r3, r13, smList__26JALList$$015JALSeModEffDist$$1@sda21
/* 801F2764 001EF6A4  4B E1 C4 D1 */	bl initiate__10JSUPtrListFv
/* 801F2768 001EF6A8  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffDist$$1Fv@ha
/* 801F276C 001EF6AC  38 83 A5 FC */	addi r4, r3, __dt__26JSUList$$015JALSeModEffDist$$1Fv@l
/* 801F2770 001EF6B0  38 6D 8F 44 */	addi r3, r13, smList__26JALList$$015JALSeModEffDist$$1@sda21
/* 801F2774 001EF6B4  38 BF 00 6C */	addi r5, r31, 0x6c
/* 801F2778 001EF6B8  4B E8 FF B1 */	bl __register_global_object
/* 801F277C 001EF6BC  38 00 00 01 */	li r0, 1
/* 801F2780 001EF6C0  98 0D 8F 92 */	stb r0, __init__smList__26JALList$$015JALSeModEffDist$$1@sda21(r13)
lbl_801F2784:
/* 801F2784 001EF6C4  88 0D 8F 93 */	lbz r0, __init__smList__26JALList$$015JALSeModPitDist$$1@sda21(r13)
/* 801F2788 001EF6C8  7C 00 07 75 */	extsb. r0, r0
/* 801F278C 001EF6CC  40 82 00 28 */	bne lbl_801F27B4
/* 801F2790 001EF6D0  38 6D 8F 50 */	addi r3, r13, smList__26JALList$$015JALSeModPitDist$$1@sda21
/* 801F2794 001EF6D4  4B E1 C4 A1 */	bl initiate__10JSUPtrListFv
/* 801F2798 001EF6D8  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitDist$$1Fv@ha
/* 801F279C 001EF6DC  38 83 A5 A4 */	addi r4, r3, __dt__26JSUList$$015JALSeModPitDist$$1Fv@l
/* 801F27A0 001EF6E0  38 6D 8F 50 */	addi r3, r13, smList__26JALList$$015JALSeModPitDist$$1@sda21
/* 801F27A4 001EF6E4  38 BF 00 78 */	addi r5, r31, 0x78
/* 801F27A8 001EF6E8  4B E8 FF 81 */	bl __register_global_object
/* 801F27AC 001EF6EC  38 00 00 01 */	li r0, 1
/* 801F27B0 001EF6F0  98 0D 8F 93 */	stb r0, __init__smList__26JALList$$015JALSeModPitDist$$1@sda21(r13)
lbl_801F27B4:
/* 801F27B4 001EF6F4  88 0D 8F 94 */	lbz r0, __init__smList__26JALList$$015JALSeModVolDist$$1@sda21(r13)
/* 801F27B8 001EF6F8  7C 00 07 75 */	extsb. r0, r0
/* 801F27BC 001EF6FC  40 82 00 28 */	bne lbl_801F27E4
/* 801F27C0 001EF700  38 6D 8F 5C */	addi r3, r13, smList__26JALList$$015JALSeModVolDist$$1@sda21
/* 801F27C4 001EF704  4B E1 C4 71 */	bl initiate__10JSUPtrListFv
/* 801F27C8 001EF708  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolDist$$1Fv@ha
/* 801F27CC 001EF70C  38 83 A5 4C */	addi r4, r3, __dt__26JSUList$$015JALSeModVolDist$$1Fv@l
/* 801F27D0 001EF710  38 6D 8F 5C */	addi r3, r13, smList__26JALList$$015JALSeModVolDist$$1@sda21
/* 801F27D4 001EF714  38 BF 00 84 */	addi r5, r31, 0x84
/* 801F27D8 001EF718  4B E8 FF 51 */	bl __register_global_object
/* 801F27DC 001EF71C  38 00 00 01 */	li r0, 1
/* 801F27E0 001EF720  98 0D 8F 94 */	stb r0, __init__smList__26JALList$$015JALSeModVolDist$$1@sda21(r13)
lbl_801F27E4:
/* 801F27E4 001EF724  88 0D 8F 95 */	lbz r0, __init__smList__26JALList$$015JALSeModEffFunk$$1@sda21(r13)
/* 801F27E8 001EF728  7C 00 07 75 */	extsb. r0, r0
/* 801F27EC 001EF72C  40 82 00 28 */	bne lbl_801F2814
/* 801F27F0 001EF730  38 6D 8F 68 */	addi r3, r13, smList__26JALList$$015JALSeModEffFunk$$1@sda21
/* 801F27F4 001EF734  4B E1 C4 41 */	bl initiate__10JSUPtrListFv
/* 801F27F8 001EF738  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffFunk$$1Fv@ha
/* 801F27FC 001EF73C  38 83 A4 F4 */	addi r4, r3, __dt__26JSUList$$015JALSeModEffFunk$$1Fv@l
/* 801F2800 001EF740  38 6D 8F 68 */	addi r3, r13, smList__26JALList$$015JALSeModEffFunk$$1@sda21
/* 801F2804 001EF744  38 BF 00 90 */	addi r5, r31, 0x90
/* 801F2808 001EF748  4B E8 FF 21 */	bl __register_global_object
/* 801F280C 001EF74C  38 00 00 01 */	li r0, 1
/* 801F2810 001EF750  98 0D 8F 95 */	stb r0, __init__smList__26JALList$$015JALSeModEffFunk$$1@sda21(r13)
lbl_801F2814:
/* 801F2814 001EF754  88 0D 8F 96 */	lbz r0, __init__smList__26JALList$$015JALSeModPitFunk$$1@sda21(r13)
/* 801F2818 001EF758  7C 00 07 75 */	extsb. r0, r0
/* 801F281C 001EF75C  40 82 00 28 */	bne lbl_801F2844
/* 801F2820 001EF760  38 6D 8F 74 */	addi r3, r13, smList__26JALList$$015JALSeModPitFunk$$1@sda21
/* 801F2824 001EF764  4B E1 C4 11 */	bl initiate__10JSUPtrListFv
/* 801F2828 001EF768  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitFunk$$1Fv@ha
/* 801F282C 001EF76C  38 83 A4 9C */	addi r4, r3, __dt__26JSUList$$015JALSeModPitFunk$$1Fv@l
/* 801F2830 001EF770  38 6D 8F 74 */	addi r3, r13, smList__26JALList$$015JALSeModPitFunk$$1@sda21
/* 801F2834 001EF774  38 BF 00 9C */	addi r5, r31, 0x9c
/* 801F2838 001EF778  4B E8 FE F1 */	bl __register_global_object
/* 801F283C 001EF77C  38 00 00 01 */	li r0, 1
/* 801F2840 001EF780  98 0D 8F 96 */	stb r0, __init__smList__26JALList$$015JALSeModPitFunk$$1@sda21(r13)
lbl_801F2844:
/* 801F2844 001EF784  88 0D 8F 97 */	lbz r0, __init__smList__26JALList$$015JALSeModVolFunk$$1@sda21(r13)
/* 801F2848 001EF788  7C 00 07 75 */	extsb. r0, r0
/* 801F284C 001EF78C  40 82 00 28 */	bne lbl_801F2874
/* 801F2850 001EF790  38 6D 8F 80 */	addi r3, r13, smList__26JALList$$015JALSeModVolFunk$$1@sda21
/* 801F2854 001EF794  4B E1 C3 E1 */	bl initiate__10JSUPtrListFv
/* 801F2858 001EF798  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolFunk$$1Fv@ha
/* 801F285C 001EF79C  38 83 A4 44 */	addi r4, r3, __dt__26JSUList$$015JALSeModVolFunk$$1Fv@l
/* 801F2860 001EF7A0  38 6D 8F 80 */	addi r3, r13, smList__26JALList$$015JALSeModVolFunk$$1@sda21
/* 801F2864 001EF7A4  38 BF 00 A8 */	addi r5, r31, 0xa8
/* 801F2868 001EF7A8  4B E8 FE C1 */	bl __register_global_object
/* 801F286C 001EF7AC  38 00 00 01 */	li r0, 1
/* 801F2870 001EF7B0  98 0D 8F 97 */	stb r0, __init__smList__26JALList$$015JALSeModVolFunk$$1@sda21(r13)
lbl_801F2874:
/* 801F2874 001EF7B4  80 01 00 14 */	lwz r0, 0x14(r1)
/* 801F2878 001EF7B8  83 E1 00 0C */	lwz r31, 0xc(r1)
/* 801F287C 001EF7BC  38 21 00 10 */	addi r1, r1, 0x10
/* 801F2880 001EF7C0  7C 08 03 A6 */	mtlr r0
/* 801F2884 001EF7C4  4E 80 00 20 */	blr 

.section .ctors, "wa"  # 0x8036FBA0 - 0x8036FF80
  .4byte __sinit_MapEventSirena_cpp

.section .rodata, "wa"  # 0x8036FFA0 - 0x803A8380
$$21490:
	.incbin "baserom.dol", 0x38FAB0, 0xC
$$22111:
	.incbin "baserom.dol", 0x38FABC, 0x14
$$22253:
	.incbin "baserom.dol", 0x38FAD0, 0x30
$$22254:
	.incbin "baserom.dol", 0x38FB00, 0x38
$$22255:
	.incbin "baserom.dol", 0x38FB38, 0x34
$$22256:
	.incbin "baserom.dol", 0x38FB6C, 0x24
$$22585:
	.incbin "baserom.dol", 0x38FB90, 0xC
$$22587:
	.incbin "baserom.dol", 0x38FB9C, 0xC
$$22771:
	.incbin "baserom.dol", 0x38FBA8, 0x24
$$22772:
	.incbin "baserom.dol", 0x38FBCC, 0x24
$$22784:
	.incbin "baserom.dol", 0x38FBF0, 0x14
$$22810:
	.incbin "baserom.dol", 0x38FC04, 0x1C
$$22811:
	.incbin "baserom.dol", 0x38FC20, 0x20

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global __vt__19TMapEventSirenaSink
__vt__19TMapEventSirenaSink:
  .4byte 0
  .4byte 0
  .4byte __dt__19TMapEventSirenaSinkFv
  .4byte getType__Q26JDrama8TNameRefCFv
  .4byte load__19TMapEventSirenaSinkFR20JSUMemoryInputStream
  .4byte save__Q26JDrama8TNameRefFR21JSUMemoryOutputStream
  .4byte loadAfter__19TMapEventSirenaSinkFv
  .4byte searchF__Q26JDrama8TNameRefFUsPCc
  .4byte perform__9TMapEventFUlPQ26JDrama9TGraphics
  .4byte launchScript__13TEventWatcherFPCc
  .4byte isFinishedAll__13TMapEventSinkCFv
  .4byte watch__19TMapEventSirenaSinkFv
  .4byte startControl__13TMapEventSinkFv
  .4byte control__13TMapEventSinkFv
  .4byte finishControl__13TMapEventSinkFv
  .4byte getBuilding__13TMapEventSinkCFi
  .4byte initBuilding__13TMapEventSinkFiR20JSUMemoryInputStream
  .4byte initWithBuildingNum__13TMapEventSinkFR20JSUMemoryInputStream
  .4byte rising__13TMapEventSinkFv
  .4byte getSinkOffsetY__13TMapEventSinkCFv
  .4byte makeBuildingRecovered__13TMapEventSinkFi
  .4byte initPollution__19TMapEventSirenaSinkFv
  .4byte makePollutionRecovered__19TMapEventSirenaSinkFUl
  .4byte 0

.section .sdata2, "wa"  # 0x8040B460 - 0x80414020
$$22744:
	.incbin "baserom.dol", 0x3E8DD0, 0x4
$$22785:
	.incbin "baserom.dol", 0x3E8DD4, 0x4
$$22786:
	.incbin "baserom.dol", 0x3E8DD8, 0x4
$$22787:
	.incbin "baserom.dol", 0x3E8DDC, 0x4

.section .bss, "wa"  # 0x803E6000 - 0x80408AC0
$$22812:
	.skip 0xC
$$22813:
	.skip 0xC
$$22814:
	.skip 0xC
$$22815:
	.skip 0xC
$$22816:
	.skip 0xC
$$22817:
	.skip 0xC
$$22818:
	.skip 0xC
$$22819:
	.skip 0xC
$$22820:
	.skip 0xC
$$22821:
	.skip 0xC
$$22822:
	.skip 0xC
$$22823:
	.skip 0xC
$$22824:
	.skip 0xC
$$22825:
	.skip 0xC
$$22826:
	.skip 0x10
