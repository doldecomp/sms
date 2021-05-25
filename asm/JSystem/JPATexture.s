.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global __ct__10JPATextureFPCUcP7JKRHeap
__ct__10JPATextureFPCUcP7JKRHeap:
/* 80081298 0007E1D8  7C 08 02 A6 */	mflr r0
/* 8008129C 0007E1DC  90 01 00 04 */	stw r0, 4(r1)
/* 800812A0 0007E1E0  94 21 FF E0 */	stwu r1, -0x20(r1)
/* 800812A4 0007E1E4  93 E1 00 1C */	stw r31, 0x1c(r1)
/* 800812A8 0007E1E8  90 61 00 08 */	stw r3, 8(r1)
/* 800812AC 0007E1EC  80 61 00 08 */	lwz r3, 8(r1)
/* 800812B0 0007E1F0  4B FF E9 AD */	bl __ct__12JPADataBlockFPCUcP7JKRHeap
/* 800812B4 0007E1F4  83 E1 00 08 */	lwz r31, 8(r1)
/* 800812B8 0007E1F8  3C 60 80 3B */	lis r3, __vt__10JPATexture@ha
/* 800812BC 0007E1FC  38 03 CD 00 */	addi r0, r3, __vt__10JPATexture@l
/* 800812C0 0007E200  90 1F 00 00 */	stw r0, 0(r31)
/* 800812C4 0007E204  38 00 00 00 */	li r0, 0
/* 800812C8 0007E208  38 7F 00 08 */	addi r3, r31, 8
/* 800812CC 0007E20C  98 1F 00 58 */	stb r0, 0x58(r31)
/* 800812D0 0007E210  90 1F 00 30 */	stw r0, 0x30(r31)
/* 800812D4 0007E214  80 9F 00 04 */	lwz r4, 4(r31)
/* 800812D8 0007E218  38 84 00 20 */	addi r4, r4, 0x20
/* 800812DC 0007E21C  4B F9 23 A1 */	bl storeTIMG__10JUTTextureFPC7ResTIMG
/* 800812E0 0007E220  80 01 00 24 */	lwz r0, 0x24(r1)
/* 800812E4 0007E224  7F E3 FB 78 */	mr r3, r31
/* 800812E8 0007E228  83 E1 00 1C */	lwz r31, 0x1c(r1)
/* 800812EC 0007E22C  38 21 00 20 */	addi r1, r1, 0x20
/* 800812F0 0007E230  7C 08 03 A6 */	mtlr r0
/* 800812F4 0007E234  4E 80 00 20 */	blr 

.global initialize__17JPADefaultTextureFP7JKRHeap
initialize__17JPADefaultTextureFP7JKRHeap:
/* 800812F8 0007E238  7C 08 02 A6 */	mflr r0
/* 800812FC 0007E23C  38 A4 00 00 */	addi r5, r4, 0
/* 80081300 0007E240  90 01 00 04 */	stw r0, 4(r1)
/* 80081304 0007E244  38 80 00 20 */	li r4, 0x20
/* 80081308 0007E248  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 8008130C 0007E24C  93 E1 00 14 */	stw r31, 0x14(r1)
/* 80081310 0007E250  3B E3 00 00 */	addi r31, r3, 0
/* 80081314 0007E254  38 60 00 80 */	li r3, 0x80
/* 80081318 0007E258  4B F8 B1 35 */	bl alloc__7JKRHeapFUliP7JKRHeap
/* 8008131C 0007E25C  90 7F 00 00 */	stw r3, 0(r31)
/* 80081320 0007E260  38 E0 00 00 */	li r7, 0
/* 80081324 0007E264  38 C0 00 FF */	li r6, 0xff
/* 80081328 0007E268  80 9F 00 00 */	lwz r4, 0(r31)
/* 8008132C 0007E26C  38 A0 00 1A */	li r5, 0x1a
/* 80081330 0007E270  38 60 00 80 */	li r3, 0x80
/* 80081334 0007E274  98 E4 00 00 */	stb r7, 0(r4)
/* 80081338 0007E278  38 00 00 D6 */	li r0, 0xd6
/* 8008133C 0007E27C  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081340 0007E280  98 C4 00 01 */	stb r6, 1(r4)
/* 80081344 0007E284  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081348 0007E288  98 E4 00 02 */	stb r7, 2(r4)
/* 8008134C 0007E28C  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081350 0007E290  98 C4 00 03 */	stb r6, 3(r4)
/* 80081354 0007E294  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081358 0007E298  98 E4 00 04 */	stb r7, 4(r4)
/* 8008135C 0007E29C  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081360 0007E2A0  98 C4 00 05 */	stb r6, 5(r4)
/* 80081364 0007E2A4  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081368 0007E2A8  98 E4 00 06 */	stb r7, 6(r4)
/* 8008136C 0007E2AC  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081370 0007E2B0  98 C4 00 07 */	stb r6, 7(r4)
/* 80081374 0007E2B4  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081378 0007E2B8  98 E4 00 08 */	stb r7, 8(r4)
/* 8008137C 0007E2BC  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081380 0007E2C0  98 C4 00 09 */	stb r6, 9(r4)
/* 80081384 0007E2C4  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081388 0007E2C8  98 A4 00 0A */	stb r5, 0xa(r4)
/* 8008138C 0007E2CC  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081390 0007E2D0  98 C4 00 0B */	stb r6, 0xb(r4)
/* 80081394 0007E2D4  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081398 0007E2D8  98 64 00 0C */	stb r3, 0xc(r4)
/* 8008139C 0007E2DC  80 9F 00 00 */	lwz r4, 0(r31)
/* 800813A0 0007E2E0  98 C4 00 0D */	stb r6, 0xd(r4)
/* 800813A4 0007E2E4  80 9F 00 00 */	lwz r4, 0(r31)
/* 800813A8 0007E2E8  98 04 00 0E */	stb r0, 0xe(r4)
/* 800813AC 0007E2EC  80 9F 00 00 */	lwz r4, 0(r31)
/* 800813B0 0007E2F0  98 C4 00 0F */	stb r6, 0xf(r4)
/* 800813B4 0007E2F4  80 9F 00 00 */	lwz r4, 0(r31)
/* 800813B8 0007E2F8  98 E4 00 10 */	stb r7, 0x10(r4)
/* 800813BC 0007E2FC  80 9F 00 00 */	lwz r4, 0(r31)
/* 800813C0 0007E300  98 C4 00 11 */	stb r6, 0x11(r4)
/* 800813C4 0007E304  80 9F 00 00 */	lwz r4, 0(r31)
/* 800813C8 0007E308  98 64 00 12 */	stb r3, 0x12(r4)
/* 800813CC 0007E30C  80 9F 00 00 */	lwz r4, 0(r31)
/* 800813D0 0007E310  98 C4 00 13 */	stb r6, 0x13(r4)
/* 800813D4 0007E314  80 9F 00 00 */	lwz r4, 0(r31)
/* 800813D8 0007E318  98 C4 00 14 */	stb r6, 0x14(r4)
/* 800813DC 0007E31C  80 9F 00 00 */	lwz r4, 0(r31)
/* 800813E0 0007E320  98 C4 00 15 */	stb r6, 0x15(r4)
/* 800813E4 0007E324  80 9F 00 00 */	lwz r4, 0(r31)
/* 800813E8 0007E328  98 C4 00 16 */	stb r6, 0x16(r4)
/* 800813EC 0007E32C  80 9F 00 00 */	lwz r4, 0(r31)
/* 800813F0 0007E330  98 C4 00 17 */	stb r6, 0x17(r4)
/* 800813F4 0007E334  80 9F 00 00 */	lwz r4, 0(r31)
/* 800813F8 0007E338  98 E4 00 18 */	stb r7, 0x18(r4)
/* 800813FC 0007E33C  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081400 0007E340  98 C4 00 19 */	stb r6, 0x19(r4)
/* 80081404 0007E344  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081408 0007E348  98 04 00 1A */	stb r0, 0x1a(r4)
/* 8008140C 0007E34C  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081410 0007E350  98 C4 00 1B */	stb r6, 0x1b(r4)
/* 80081414 0007E354  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081418 0007E358  98 C4 00 1C */	stb r6, 0x1c(r4)
/* 8008141C 0007E35C  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081420 0007E360  98 C4 00 1D */	stb r6, 0x1d(r4)
/* 80081424 0007E364  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081428 0007E368  98 C4 00 1E */	stb r6, 0x1e(r4)
/* 8008142C 0007E36C  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081430 0007E370  98 C4 00 1F */	stb r6, 0x1f(r4)
/* 80081434 0007E374  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081438 0007E378  98 E4 00 20 */	stb r7, 0x20(r4)
/* 8008143C 0007E37C  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081440 0007E380  98 C4 00 21 */	stb r6, 0x21(r4)
/* 80081444 0007E384  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081448 0007E388  98 E4 00 22 */	stb r7, 0x22(r4)
/* 8008144C 0007E38C  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081450 0007E390  98 C4 00 23 */	stb r6, 0x23(r4)
/* 80081454 0007E394  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081458 0007E398  98 E4 00 24 */	stb r7, 0x24(r4)
/* 8008145C 0007E39C  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081460 0007E3A0  98 C4 00 25 */	stb r6, 0x25(r4)
/* 80081464 0007E3A4  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081468 0007E3A8  98 E4 00 26 */	stb r7, 0x26(r4)
/* 8008146C 0007E3AC  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081470 0007E3B0  98 C4 00 27 */	stb r6, 0x27(r4)
/* 80081474 0007E3B4  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081478 0007E3B8  98 04 00 28 */	stb r0, 0x28(r4)
/* 8008147C 0007E3BC  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081480 0007E3C0  98 C4 00 29 */	stb r6, 0x29(r4)
/* 80081484 0007E3C4  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081488 0007E3C8  98 64 00 2A */	stb r3, 0x2a(r4)
/* 8008148C 0007E3CC  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081490 0007E3D0  98 C4 00 2B */	stb r6, 0x2b(r4)
/* 80081494 0007E3D4  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081498 0007E3D8  98 A4 00 2C */	stb r5, 0x2c(r4)
/* 8008149C 0007E3DC  80 9F 00 00 */	lwz r4, 0(r31)
/* 800814A0 0007E3E0  98 C4 00 2D */	stb r6, 0x2d(r4)
/* 800814A4 0007E3E4  80 9F 00 00 */	lwz r4, 0(r31)
/* 800814A8 0007E3E8  98 E4 00 2E */	stb r7, 0x2e(r4)
/* 800814AC 0007E3EC  80 9F 00 00 */	lwz r4, 0(r31)
/* 800814B0 0007E3F0  98 C4 00 2F */	stb r6, 0x2f(r4)
/* 800814B4 0007E3F4  80 9F 00 00 */	lwz r4, 0(r31)
/* 800814B8 0007E3F8  98 C4 00 30 */	stb r6, 0x30(r4)
/* 800814BC 0007E3FC  80 9F 00 00 */	lwz r4, 0(r31)
/* 800814C0 0007E400  98 C4 00 31 */	stb r6, 0x31(r4)
/* 800814C4 0007E404  80 9F 00 00 */	lwz r4, 0(r31)
/* 800814C8 0007E408  98 C4 00 32 */	stb r6, 0x32(r4)
/* 800814CC 0007E40C  80 9F 00 00 */	lwz r4, 0(r31)
/* 800814D0 0007E410  98 C4 00 33 */	stb r6, 0x33(r4)
/* 800814D4 0007E414  80 9F 00 00 */	lwz r4, 0(r31)
/* 800814D8 0007E418  98 64 00 34 */	stb r3, 0x34(r4)
/* 800814DC 0007E41C  80 9F 00 00 */	lwz r4, 0(r31)
/* 800814E0 0007E420  98 C4 00 35 */	stb r6, 0x35(r4)
/* 800814E4 0007E424  80 9F 00 00 */	lwz r4, 0(r31)
/* 800814E8 0007E428  98 E4 00 36 */	stb r7, 0x36(r4)
/* 800814EC 0007E42C  80 9F 00 00 */	lwz r4, 0(r31)
/* 800814F0 0007E430  98 C4 00 37 */	stb r6, 0x37(r4)
/* 800814F4 0007E434  80 9F 00 00 */	lwz r4, 0(r31)
/* 800814F8 0007E438  98 C4 00 38 */	stb r6, 0x38(r4)
/* 800814FC 0007E43C  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081500 0007E440  98 C4 00 39 */	stb r6, 0x39(r4)
/* 80081504 0007E444  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081508 0007E448  98 C4 00 3A */	stb r6, 0x3a(r4)
/* 8008150C 0007E44C  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081510 0007E450  98 C4 00 3B */	stb r6, 0x3b(r4)
/* 80081514 0007E454  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081518 0007E458  98 04 00 3C */	stb r0, 0x3c(r4)
/* 8008151C 0007E45C  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081520 0007E460  98 C4 00 3D */	stb r6, 0x3d(r4)
/* 80081524 0007E464  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081528 0007E468  98 E4 00 3E */	stb r7, 0x3e(r4)
/* 8008152C 0007E46C  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081530 0007E470  98 C4 00 3F */	stb r6, 0x3f(r4)
/* 80081534 0007E474  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081538 0007E478  98 E4 00 40 */	stb r7, 0x40(r4)
/* 8008153C 0007E47C  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081540 0007E480  98 C4 00 41 */	stb r6, 0x41(r4)
/* 80081544 0007E484  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081548 0007E488  98 04 00 42 */	stb r0, 0x42(r4)
/* 8008154C 0007E48C  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081550 0007E490  98 C4 00 43 */	stb r6, 0x43(r4)
/* 80081554 0007E494  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081558 0007E498  98 C4 00 44 */	stb r6, 0x44(r4)
/* 8008155C 0007E49C  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081560 0007E4A0  98 C4 00 45 */	stb r6, 0x45(r4)
/* 80081564 0007E4A4  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081568 0007E4A8  98 C4 00 46 */	stb r6, 0x46(r4)
/* 8008156C 0007E4AC  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081570 0007E4B0  98 C4 00 47 */	stb r6, 0x47(r4)
/* 80081574 0007E4B4  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081578 0007E4B8  98 E4 00 48 */	stb r7, 0x48(r4)
/* 8008157C 0007E4BC  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081580 0007E4C0  98 C4 00 49 */	stb r6, 0x49(r4)
/* 80081584 0007E4C4  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081588 0007E4C8  98 64 00 4A */	stb r3, 0x4a(r4)
/* 8008158C 0007E4CC  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081590 0007E4D0  98 C4 00 4B */	stb r6, 0x4b(r4)
/* 80081594 0007E4D4  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081598 0007E4D8  98 C4 00 4C */	stb r6, 0x4c(r4)
/* 8008159C 0007E4DC  80 9F 00 00 */	lwz r4, 0(r31)
/* 800815A0 0007E4E0  98 C4 00 4D */	stb r6, 0x4d(r4)
/* 800815A4 0007E4E4  80 9F 00 00 */	lwz r4, 0(r31)
/* 800815A8 0007E4E8  98 C4 00 4E */	stb r6, 0x4e(r4)
/* 800815AC 0007E4EC  80 9F 00 00 */	lwz r4, 0(r31)
/* 800815B0 0007E4F0  98 C4 00 4F */	stb r6, 0x4f(r4)
/* 800815B4 0007E4F4  80 9F 00 00 */	lwz r4, 0(r31)
/* 800815B8 0007E4F8  98 E4 00 50 */	stb r7, 0x50(r4)
/* 800815BC 0007E4FC  80 9F 00 00 */	lwz r4, 0(r31)
/* 800815C0 0007E500  98 C4 00 51 */	stb r6, 0x51(r4)
/* 800815C4 0007E504  80 9F 00 00 */	lwz r4, 0(r31)
/* 800815C8 0007E508  98 A4 00 52 */	stb r5, 0x52(r4)
/* 800815CC 0007E50C  80 9F 00 00 */	lwz r4, 0(r31)
/* 800815D0 0007E510  98 C4 00 53 */	stb r6, 0x53(r4)
/* 800815D4 0007E514  80 9F 00 00 */	lwz r4, 0(r31)
/* 800815D8 0007E518  98 64 00 54 */	stb r3, 0x54(r4)
/* 800815DC 0007E51C  80 9F 00 00 */	lwz r4, 0(r31)
/* 800815E0 0007E520  98 C4 00 55 */	stb r6, 0x55(r4)
/* 800815E4 0007E524  80 9F 00 00 */	lwz r4, 0(r31)
/* 800815E8 0007E528  98 04 00 56 */	stb r0, 0x56(r4)
/* 800815EC 0007E52C  80 9F 00 00 */	lwz r4, 0(r31)
/* 800815F0 0007E530  98 C4 00 57 */	stb r6, 0x57(r4)
/* 800815F4 0007E534  80 9F 00 00 */	lwz r4, 0(r31)
/* 800815F8 0007E538  98 E4 00 58 */	stb r7, 0x58(r4)
/* 800815FC 0007E53C  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081600 0007E540  98 C4 00 59 */	stb r6, 0x59(r4)
/* 80081604 0007E544  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081608 0007E548  98 E4 00 5A */	stb r7, 0x5a(r4)
/* 8008160C 0007E54C  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081610 0007E550  98 C4 00 5B */	stb r6, 0x5b(r4)
/* 80081614 0007E554  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081618 0007E558  98 E4 00 5C */	stb r7, 0x5c(r4)
/* 8008161C 0007E55C  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081620 0007E560  98 C4 00 5D */	stb r6, 0x5d(r4)
/* 80081624 0007E564  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081628 0007E568  98 E4 00 5E */	stb r7, 0x5e(r4)
/* 8008162C 0007E56C  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081630 0007E570  98 C4 00 5F */	stb r6, 0x5f(r4)
/* 80081634 0007E574  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081638 0007E578  98 C4 00 60 */	stb r6, 0x60(r4)
/* 8008163C 0007E57C  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081640 0007E580  98 C4 00 61 */	stb r6, 0x61(r4)
/* 80081644 0007E584  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081648 0007E588  98 C4 00 62 */	stb r6, 0x62(r4)
/* 8008164C 0007E58C  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081650 0007E590  98 C4 00 63 */	stb r6, 0x63(r4)
/* 80081654 0007E594  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081658 0007E598  98 04 00 64 */	stb r0, 0x64(r4)
/* 8008165C 0007E59C  81 1F 00 00 */	lwz r8, 0(r31)
/* 80081660 0007E5A0  38 80 00 80 */	li r4, 0x80
/* 80081664 0007E5A4  98 C8 00 65 */	stb r6, 0x65(r8)
/* 80081668 0007E5A8  81 1F 00 00 */	lwz r8, 0(r31)
/* 8008166C 0007E5AC  98 E8 00 66 */	stb r7, 0x66(r8)
/* 80081670 0007E5B0  81 1F 00 00 */	lwz r8, 0(r31)
/* 80081674 0007E5B4  98 C8 00 67 */	stb r6, 0x67(r8)
/* 80081678 0007E5B8  81 1F 00 00 */	lwz r8, 0(r31)
/* 8008167C 0007E5BC  98 C8 00 68 */	stb r6, 0x68(r8)
/* 80081680 0007E5C0  81 1F 00 00 */	lwz r8, 0(r31)
/* 80081684 0007E5C4  98 C8 00 69 */	stb r6, 0x69(r8)
/* 80081688 0007E5C8  81 1F 00 00 */	lwz r8, 0(r31)
/* 8008168C 0007E5CC  98 C8 00 6A */	stb r6, 0x6a(r8)
/* 80081690 0007E5D0  81 1F 00 00 */	lwz r8, 0(r31)
/* 80081694 0007E5D4  98 C8 00 6B */	stb r6, 0x6b(r8)
/* 80081698 0007E5D8  81 1F 00 00 */	lwz r8, 0(r31)
/* 8008169C 0007E5DC  98 68 00 6C */	stb r3, 0x6c(r8)
/* 800816A0 0007E5E0  81 1F 00 00 */	lwz r8, 0(r31)
/* 800816A4 0007E5E4  98 C8 00 6D */	stb r6, 0x6d(r8)
/* 800816A8 0007E5E8  81 1F 00 00 */	lwz r8, 0(r31)
/* 800816AC 0007E5EC  98 E8 00 6E */	stb r7, 0x6e(r8)
/* 800816B0 0007E5F0  81 1F 00 00 */	lwz r8, 0(r31)
/* 800816B4 0007E5F4  98 C8 00 6F */	stb r6, 0x6f(r8)
/* 800816B8 0007E5F8  81 1F 00 00 */	lwz r8, 0(r31)
/* 800816BC 0007E5FC  98 08 00 70 */	stb r0, 0x70(r8)
/* 800816C0 0007E600  81 1F 00 00 */	lwz r8, 0(r31)
/* 800816C4 0007E604  98 C8 00 71 */	stb r6, 0x71(r8)
/* 800816C8 0007E608  81 1F 00 00 */	lwz r8, 0(r31)
/* 800816CC 0007E60C  98 68 00 72 */	stb r3, 0x72(r8)
/* 800816D0 0007E610  80 7F 00 00 */	lwz r3, 0(r31)
/* 800816D4 0007E614  98 C3 00 73 */	stb r6, 0x73(r3)
/* 800816D8 0007E618  80 7F 00 00 */	lwz r3, 0(r31)
/* 800816DC 0007E61C  98 A3 00 74 */	stb r5, 0x74(r3)
/* 800816E0 0007E620  80 7F 00 00 */	lwz r3, 0(r31)
/* 800816E4 0007E624  98 C3 00 75 */	stb r6, 0x75(r3)
/* 800816E8 0007E628  80 7F 00 00 */	lwz r3, 0(r31)
/* 800816EC 0007E62C  98 E3 00 76 */	stb r7, 0x76(r3)
/* 800816F0 0007E630  80 7F 00 00 */	lwz r3, 0(r31)
/* 800816F4 0007E634  98 C3 00 77 */	stb r6, 0x77(r3)
/* 800816F8 0007E638  80 7F 00 00 */	lwz r3, 0(r31)
/* 800816FC 0007E63C  98 E3 00 78 */	stb r7, 0x78(r3)
/* 80081700 0007E640  80 7F 00 00 */	lwz r3, 0(r31)
/* 80081704 0007E644  98 C3 00 79 */	stb r6, 0x79(r3)
/* 80081708 0007E648  80 7F 00 00 */	lwz r3, 0(r31)
/* 8008170C 0007E64C  98 E3 00 7A */	stb r7, 0x7a(r3)
/* 80081710 0007E650  80 7F 00 00 */	lwz r3, 0(r31)
/* 80081714 0007E654  98 C3 00 7B */	stb r6, 0x7b(r3)
/* 80081718 0007E658  80 7F 00 00 */	lwz r3, 0(r31)
/* 8008171C 0007E65C  98 E3 00 7C */	stb r7, 0x7c(r3)
/* 80081720 0007E660  80 7F 00 00 */	lwz r3, 0(r31)
/* 80081724 0007E664  98 C3 00 7D */	stb r6, 0x7d(r3)
/* 80081728 0007E668  80 7F 00 00 */	lwz r3, 0(r31)
/* 8008172C 0007E66C  98 E3 00 7E */	stb r7, 0x7e(r3)
/* 80081730 0007E670  80 7F 00 00 */	lwz r3, 0(r31)
/* 80081734 0007E674  98 C3 00 7F */	stb r6, 0x7f(r3)
/* 80081738 0007E678  80 7F 00 00 */	lwz r3, 0(r31)
/* 8008173C 0007E67C  48 00 C8 A5 */	bl DCStoreRange
/* 80081740 0007E680  80 9F 00 00 */	lwz r4, 0(r31)
/* 80081744 0007E684  38 7F 00 04 */	addi r3, r31, 4
/* 80081748 0007E688  38 A0 00 08 */	li r5, 8
/* 8008174C 0007E68C  38 C0 00 08 */	li r6, 8
/* 80081750 0007E690  38 E0 00 03 */	li r7, 3
/* 80081754 0007E694  39 00 00 01 */	li r8, 1
/* 80081758 0007E698  39 20 00 01 */	li r9, 1
/* 8008175C 0007E69C  39 40 00 00 */	li r10, 0
/* 80081760 0007E6A0  48 02 8C 99 */	bl GXInitTexObj
/* 80081764 0007E6A4  C0 22 89 28 */	lfs f1, $$21494@sda21(r2)
/* 80081768 0007E6A8  38 7F 00 04 */	addi r3, r31, 4
/* 8008176C 0007E6AC  38 80 00 01 */	li r4, 1
/* 80081770 0007E6B0  FC 40 08 90 */	fmr f2, f1
/* 80081774 0007E6B4  38 A0 00 01 */	li r5, 1
/* 80081778 0007E6B8  FC 60 08 90 */	fmr f3, f1
/* 8008177C 0007E6BC  38 C0 00 00 */	li r6, 0
/* 80081780 0007E6C0  38 E0 00 00 */	li r7, 0
/* 80081784 0007E6C4  39 00 00 00 */	li r8, 0
/* 80081788 0007E6C8  48 02 8F 2D */	bl GXInitTexObjLOD
/* 8008178C 0007E6CC  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 80081790 0007E6D0  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 80081794 0007E6D4  38 21 00 18 */	addi r1, r1, 0x18
/* 80081798 0007E6D8  7C 08 03 A6 */	mtlr r0
/* 8008179C 0007E6DC  4E 80 00 20 */	blr 

.global __dt__10JPATextureFv
__dt__10JPATextureFv:
/* 800817A0 0007E6E0  7C 08 02 A6 */	mflr r0
/* 800817A4 0007E6E4  90 01 00 04 */	stw r0, 4(r1)
/* 800817A8 0007E6E8  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 800817AC 0007E6EC  93 E1 00 14 */	stw r31, 0x14(r1)
/* 800817B0 0007E6F0  3B E4 00 00 */	addi r31, r4, 0
/* 800817B4 0007E6F4  93 C1 00 10 */	stw r30, 0x10(r1)
/* 800817B8 0007E6F8  7C 7E 1B 79 */	or. r30, r3, r3
/* 800817BC 0007E6FC  41 82 00 40 */	beq lbl_800817FC
/* 800817C0 0007E700  3C 60 80 3B */	lis r3, __vt__10JPATexture@ha
/* 800817C4 0007E704  38 03 CD 00 */	addi r0, r3, __vt__10JPATexture@l
/* 800817C8 0007E708  90 1E 00 00 */	stw r0, 0(r30)
/* 800817CC 0007E70C  38 7E 00 08 */	addi r3, r30, 8
/* 800817D0 0007E710  38 80 FF FF */	li r4, -1
/* 800817D4 0007E714  4B F9 1E 39 */	bl __dt__10JUTTextureFv
/* 800817D8 0007E718  28 1E 00 00 */	cmplwi r30, 0
/* 800817DC 0007E71C  41 82 00 10 */	beq lbl_800817EC
/* 800817E0 0007E720  3C 60 80 3B */	lis r3, __vt__12JPADataBlock@ha
/* 800817E4 0007E724  38 03 CB F4 */	addi r0, r3, __vt__12JPADataBlock@l
/* 800817E8 0007E728  90 1E 00 00 */	stw r0, 0(r30)
lbl_800817EC:
/* 800817EC 0007E72C  7F E0 07 35 */	extsh. r0, r31
/* 800817F0 0007E730  40 81 00 0C */	ble lbl_800817FC
/* 800817F4 0007E734  7F C3 F3 78 */	mr r3, r30
/* 800817F8 0007E738  4B F8 B2 B9 */	bl __dl__FPv
lbl_800817FC:
/* 800817FC 0007E73C  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 80081800 0007E740  7F C3 F3 78 */	mr r3, r30
/* 80081804 0007E744  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 80081808 0007E748  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 8008180C 0007E74C  7C 08 03 A6 */	mtlr r0
/* 80081810 0007E750  38 21 00 18 */	addi r1, r1, 0x18
/* 80081814 0007E754  4E 80 00 20 */	blr 

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global __vt__10JPATexture
__vt__10JPATexture:
  .4byte 0
  .4byte 0
  .4byte __dt__10JPATextureFv
  .4byte 0

.section .sdata2, "wa"  # 0x8040B460 - 0x80414020
$$21494:
	.incbin "baserom.dol", 0x3E4628, 0x8
