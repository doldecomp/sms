.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global __ct__17TMapCollisionDataFv
__ct__17TMapCollisionDataFv:
/* 801DEED4 001DBE14  7C 08 02 A6 */	mflr r0
/* 801DEED8 001DBE18  90 01 00 04 */	stw r0, 4(r1)
/* 801DEEDC 001DBE1C  94 21 FF C0 */	stwu r1, -0x40(r1)
/* 801DEEE0 001DBE20  93 E1 00 3C */	stw r31, 0x3c(r1)
/* 801DEEE4 001DBE24  3B E0 00 00 */	li r31, 0
/* 801DEEE8 001DBE28  38 81 00 24 */	addi r4, r1, 0x24
/* 801DEEEC 001DBE2C  93 C1 00 38 */	stw r30, 0x38(r1)
/* 801DEEF0 001DBE30  38 A1 00 18 */	addi r5, r1, 0x18
/* 801DEEF4 001DBE34  38 C1 00 0C */	addi r6, r1, 0xc
/* 801DEEF8 001DBE38  93 A1 00 34 */	stw r29, 0x34(r1)
/* 801DEEFC 001DBE3C  3B A3 00 00 */	addi r29, r3, 0
/* 801DEF00 001DBE40  3C 60 80 3F */	lis r3, mIllegalCheckData__17TMapCollisionData@ha
/* 801DEF04 001DBE44  C0 02 CD C8 */	lfs f0, $$22183@sda21(r2)
/* 801DEF08 001DBE48  3B C3 79 10 */	addi r30, r3, mIllegalCheckData__17TMapCollisionData@l
/* 801DEF0C 001DBE4C  38 7E 00 00 */	addi r3, r30, 0
/* 801DEF10 001DBE50  D0 1D 00 00 */	stfs f0, 0(r29)
/* 801DEF14 001DBE54  D0 1D 00 04 */	stfs f0, 4(r29)
/* 801DEF18 001DBE58  93 FD 00 08 */	stw r31, 8(r29)
/* 801DEF1C 001DBE5C  93 FD 00 0C */	stw r31, 0xc(r29)
/* 801DEF20 001DBE60  93 FD 00 10 */	stw r31, 0x10(r29)
/* 801DEF24 001DBE64  93 FD 00 14 */	stw r31, 0x14(r29)
/* 801DEF28 001DBE68  93 FD 00 18 */	stw r31, 0x18(r29)
/* 801DEF2C 001DBE6C  93 FD 00 1C */	stw r31, 0x1c(r29)
/* 801DEF30 001DBE70  93 FD 00 20 */	stw r31, 0x20(r29)
/* 801DEF34 001DBE74  93 FD 00 24 */	stw r31, 0x24(r29)
/* 801DEF38 001DBE78  93 FD 00 28 */	stw r31, 0x28(r29)
/* 801DEF3C 001DBE7C  93 FD 00 2C */	stw r31, 0x2c(r29)
/* 801DEF40 001DBE80  93 FD 00 30 */	stw r31, 0x30(r29)
/* 801DEF44 001DBE84  93 FD 00 34 */	stw r31, 0x34(r29)
/* 801DEF48 001DBE88  93 FD 00 38 */	stw r31, 0x38(r29)
/* 801DEF4C 001DBE8C  93 FD 00 3C */	stw r31, 0x3c(r29)
/* 801DEF50 001DBE90  B3 FD 00 40 */	sth r31, 0x40(r29)
/* 801DEF54 001DBE94  B3 FD 02 42 */	sth r31, 0x242(r29)
/* 801DEF58 001DBE98  93 FD 02 44 */	stw r31, 0x244(r29)
/* 801DEF5C 001DBE9C  C0 22 CD CC */	lfs f1, $$22184@sda21(r2)
/* 801DEF60 001DBEA0  C0 02 CD D0 */	lfs f0, $$22185@sda21(r2)
/* 801DEF64 001DBEA4  D0 21 00 24 */	stfs f1, 0x24(r1)
/* 801DEF68 001DBEA8  D0 21 00 18 */	stfs f1, 0x18(r1)
/* 801DEF6C 001DBEAC  D0 01 00 0C */	stfs f0, 0xc(r1)
/* 801DEF70 001DBEB0  D0 21 00 28 */	stfs f1, 0x28(r1)
/* 801DEF74 001DBEB4  D0 21 00 1C */	stfs f1, 0x1c(r1)
/* 801DEF78 001DBEB8  D0 21 00 10 */	stfs f1, 0x10(r1)
/* 801DEF7C 001DBEBC  D0 21 00 2C */	stfs f1, 0x2c(r1)
/* 801DEF80 001DBEC0  D0 01 00 20 */	stfs f0, 0x20(r1)
/* 801DEF84 001DBEC4  D0 01 00 14 */	stfs f0, 0x14(r1)
/* 801DEF88 001DBEC8  48 00 48 31 */	bl setVertex__12TBGCheckDataFRCQ29JGeometry8TVec3$$0f$$1RCQ29JGeometry8TVec3$$0f$$1RCQ29JGeometry8TVec3$$0f$$1
/* 801DEF8C 001DBECC  B3 FE 00 00 */	sth r31, 0(r30)
/* 801DEF90 001DBED0  7F A3 EB 78 */	mr r3, r29
/* 801DEF94 001DBED4  A0 1E 00 04 */	lhz r0, 4(r30)
/* 801DEF98 001DBED8  60 00 00 10 */	ori r0, r0, 0x10
/* 801DEF9C 001DBEDC  B0 1E 00 04 */	sth r0, 4(r30)
/* 801DEFA0 001DBEE0  93 AD 9A B8 */	stw r29, gpMapCollisionData@sda21(r13)
/* 801DEFA4 001DBEE4  80 01 00 44 */	lwz r0, 0x44(r1)
/* 801DEFA8 001DBEE8  83 E1 00 3C */	lwz r31, 0x3c(r1)
/* 801DEFAC 001DBEEC  83 C1 00 38 */	lwz r30, 0x38(r1)
/* 801DEFB0 001DBEF0  83 A1 00 34 */	lwz r29, 0x34(r1)
/* 801DEFB4 001DBEF4  38 21 00 40 */	addi r1, r1, 0x40
/* 801DEFB8 001DBEF8  7C 08 03 A6 */	mtlr r0
/* 801DEFBC 001DBEFC  4E 80 00 20 */	blr 

.global init__17TMapCollisionDataFR20JSUMemoryInputStream
init__17TMapCollisionDataFR20JSUMemoryInputStream:
/* 801DEFC0 001DBF00  7C 08 02 A6 */	mflr r0
/* 801DEFC4 001DBF04  38 A0 00 04 */	li r5, 4
/* 801DEFC8 001DBF08  90 01 00 04 */	stw r0, 4(r1)
/* 801DEFCC 001DBF0C  94 21 FF 98 */	stwu r1, -0x68(r1)
/* 801DEFD0 001DBF10  93 E1 00 64 */	stw r31, 0x64(r1)
/* 801DEFD4 001DBF14  3B E3 00 00 */	addi r31, r3, 0
/* 801DEFD8 001DBF18  93 C1 00 60 */	stw r30, 0x60(r1)
/* 801DEFDC 001DBF1C  93 A1 00 5C */	stw r29, 0x5c(r1)
/* 801DEFE0 001DBF20  3B A4 00 00 */	addi r29, r4, 0
/* 801DEFE4 001DBF24  38 7D 00 00 */	addi r3, r29, 0
/* 801DEFE8 001DBF28  38 81 00 40 */	addi r4, r1, 0x40
/* 801DEFEC 001DBF2C  4B E2 F5 A5 */	bl read__14JSUInputStreamFPvl
/* 801DEFF0 001DBF30  80 01 00 40 */	lwz r0, 0x40(r1)
/* 801DEFF4 001DBF34  38 7D 00 00 */	addi r3, r29, 0
/* 801DEFF8 001DBF38  38 81 00 40 */	addi r4, r1, 0x40
/* 801DEFFC 001DBF3C  90 1F 00 08 */	stw r0, 8(r31)
/* 801DF000 001DBF40  38 A0 00 04 */	li r5, 4
/* 801DF004 001DBF44  4B E2 F5 8D */	bl read__14JSUInputStreamFPvl
/* 801DF008 001DBF48  80 01 00 40 */	lwz r0, 0x40(r1)
/* 801DF00C 001DBF4C  38 7D 00 00 */	addi r3, r29, 0
/* 801DF010 001DBF50  38 81 00 40 */	addi r4, r1, 0x40
/* 801DF014 001DBF54  90 1F 00 0C */	stw r0, 0xc(r31)
/* 801DF018 001DBF58  38 A0 00 04 */	li r5, 4
/* 801DF01C 001DBF5C  4B E2 F5 75 */	bl read__14JSUInputStreamFPvl
/* 801DF020 001DBF60  80 01 00 40 */	lwz r0, 0x40(r1)
/* 801DF024 001DBF64  38 7D 00 00 */	addi r3, r29, 0
/* 801DF028 001DBF68  38 81 00 40 */	addi r4, r1, 0x40
/* 801DF02C 001DBF6C  90 1F 00 1C */	stw r0, 0x1c(r31)
/* 801DF030 001DBF70  38 A0 00 04 */	li r5, 4
/* 801DF034 001DBF74  4B E2 F5 5D */	bl read__14JSUInputStreamFPvl
/* 801DF038 001DBF78  80 01 00 40 */	lwz r0, 0x40(r1)
/* 801DF03C 001DBF7C  38 7D 00 00 */	addi r3, r29, 0
/* 801DF040 001DBF80  38 81 00 40 */	addi r4, r1, 0x40
/* 801DF044 001DBF84  90 1F 00 20 */	stw r0, 0x20(r31)
/* 801DF048 001DBF88  38 A0 00 04 */	li r5, 4
/* 801DF04C 001DBF8C  4B E2 F5 45 */	bl read__14JSUInputStreamFPvl
/* 801DF050 001DBF90  80 01 00 40 */	lwz r0, 0x40(r1)
/* 801DF054 001DBF94  3C 60 43 30 */	lis r3, 0x4330
/* 801DF058 001DBF98  90 1F 00 24 */	stw r0, 0x24(r31)
/* 801DF05C 001DBF9C  80 1F 00 08 */	lwz r0, 8(r31)
/* 801DF060 001DBFA0  C8 22 CD D8 */	lfd f1, $$22223@sda21(r2)
/* 801DF064 001DBFA4  7C 00 0E 70 */	srawi r0, r0, 1
/* 801DF068 001DBFA8  C0 42 CD D4 */	lfs f2, $$22221@sda21(r2)
/* 801DF06C 001DBFAC  7C 00 01 94 */	addze r0, r0
/* 801DF070 001DBFB0  6C 00 80 00 */	xoris r0, r0, 0x8000
/* 801DF074 001DBFB4  90 01 00 54 */	stw r0, 0x54(r1)
/* 801DF078 001DBFB8  90 61 00 50 */	stw r3, 0x50(r1)
/* 801DF07C 001DBFBC  C8 01 00 50 */	lfd f0, 0x50(r1)
/* 801DF080 001DBFC0  EC 00 08 28 */	fsubs f0, f0, f1
/* 801DF084 001DBFC4  EC 02 00 32 */	fmuls f0, f2, f0
/* 801DF088 001DBFC8  D0 1F 00 00 */	stfs f0, 0(r31)
/* 801DF08C 001DBFCC  80 1F 00 0C */	lwz r0, 0xc(r31)
/* 801DF090 001DBFD0  7C 00 0E 70 */	srawi r0, r0, 1
/* 801DF094 001DBFD4  7C 00 01 94 */	addze r0, r0
/* 801DF098 001DBFD8  6C 00 80 00 */	xoris r0, r0, 0x8000
/* 801DF09C 001DBFDC  90 01 00 4C */	stw r0, 0x4c(r1)
/* 801DF0A0 001DBFE0  90 61 00 48 */	stw r3, 0x48(r1)
/* 801DF0A4 001DBFE4  C8 01 00 48 */	lfd f0, 0x48(r1)
/* 801DF0A8 001DBFE8  EC 00 08 28 */	fsubs f0, f0, f1
/* 801DF0AC 001DBFEC  EC 02 00 32 */	fmuls f0, f2, f0
/* 801DF0B0 001DBFF0  D0 1F 00 04 */	stfs f0, 4(r31)
/* 801DF0B4 001DBFF4  80 7F 00 08 */	lwz r3, 8(r31)
/* 801DF0B8 001DBFF8  80 1F 00 0C */	lwz r0, 0xc(r31)
/* 801DF0BC 001DBFFC  7C 03 01 D6 */	mullw r0, r3, r0
/* 801DF0C0 001DC000  90 1F 00 10 */	stw r0, 0x10(r31)
/* 801DF0C4 001DC004  83 BF 00 10 */	lwz r29, 0x10(r31)
/* 801DF0C8 001DC008  1C 7D 00 24 */	mulli r3, r29, 0x24
/* 801DF0CC 001DC00C  38 63 00 08 */	addi r3, r3, 8
/* 801DF0D0 001DC010  4B E2 D8 E1 */	bl __nwa__FUl
/* 801DF0D4 001DC014  3C 80 80 1E */	lis r4, __ct__16TBGCheckListRootFv@ha
/* 801DF0D8 001DC018  3B C4 F3 E8 */	addi r30, r4, __ct__16TBGCheckListRootFv@l
/* 801DF0DC 001DC01C  38 9E 00 00 */	addi r4, r30, 0
/* 801DF0E0 001DC020  38 FD 00 00 */	addi r7, r29, 0
/* 801DF0E4 001DC024  38 A0 00 00 */	li r5, 0
/* 801DF0E8 001DC028  38 C0 00 24 */	li r6, 0x24
/* 801DF0EC 001DC02C  4B EA 39 01 */	bl __construct_new_array
/* 801DF0F0 001DC030  90 7F 00 14 */	stw r3, 0x14(r31)
/* 801DF0F4 001DC034  83 BF 00 10 */	lwz r29, 0x10(r31)
/* 801DF0F8 001DC038  1C 7D 00 24 */	mulli r3, r29, 0x24
/* 801DF0FC 001DC03C  38 63 00 08 */	addi r3, r3, 8
/* 801DF100 001DC040  4B E2 D8 B1 */	bl __nwa__FUl
/* 801DF104 001DC044  38 9E 00 00 */	addi r4, r30, 0
/* 801DF108 001DC048  38 FD 00 00 */	addi r7, r29, 0
/* 801DF10C 001DC04C  38 A0 00 00 */	li r5, 0
/* 801DF110 001DC050  38 C0 00 24 */	li r6, 0x24
/* 801DF114 001DC054  4B EA 38 D9 */	bl __construct_new_array
/* 801DF118 001DC058  90 7F 00 18 */	stw r3, 0x18(r31)
/* 801DF11C 001DC05C  83 BF 00 1C */	lwz r29, 0x1c(r31)
/* 801DF120 001DC060  1C 7D 00 48 */	mulli r3, r29, 0x48
/* 801DF124 001DC064  38 63 00 08 */	addi r3, r3, 8
/* 801DF128 001DC068  4B E2 D8 89 */	bl __nwa__FUl
/* 801DF12C 001DC06C  3C 80 80 1F */	lis r4, __ct__12TBGCheckDataFv@ha
/* 801DF130 001DC070  38 84 4B C8 */	addi r4, r4, __ct__12TBGCheckDataFv@l
/* 801DF134 001DC074  38 FD 00 00 */	addi r7, r29, 0
/* 801DF138 001DC078  38 A0 00 00 */	li r5, 0
/* 801DF13C 001DC07C  38 C0 00 48 */	li r6, 0x48
/* 801DF140 001DC080  4B EA 38 AD */	bl __construct_new_array
/* 801DF144 001DC084  90 7F 00 28 */	stw r3, 0x28(r31)
/* 801DF148 001DC088  83 BF 00 20 */	lwz r29, 0x20(r31)
/* 801DF14C 001DC08C  1C 7D 00 0C */	mulli r3, r29, 0xc
/* 801DF150 001DC090  38 63 00 08 */	addi r3, r3, 8
/* 801DF154 001DC094  4B E2 D8 5D */	bl __nwa__FUl
/* 801DF158 001DC098  3C 80 80 1E */	lis r4, __ct__12TBGCheckListFv@ha
/* 801DF15C 001DC09C  38 84 F4 9C */	addi r4, r4, __ct__12TBGCheckListFv@l
/* 801DF160 001DC0A0  38 FD 00 00 */	addi r7, r29, 0
/* 801DF164 001DC0A4  38 A0 00 00 */	li r5, 0
/* 801DF168 001DC0A8  38 C0 00 0C */	li r6, 0xc
/* 801DF16C 001DC0AC  4B EA 38 81 */	bl __construct_new_array
/* 801DF170 001DC0B0  90 7F 00 2C */	stw r3, 0x2c(r31)
/* 801DF174 001DC0B4  83 BF 00 24 */	lwz r29, 0x24(r31)
/* 801DF178 001DC0B8  1C 7D 00 14 */	mulli r3, r29, 0x14
/* 801DF17C 001DC0BC  38 63 00 08 */	addi r3, r3, 8
/* 801DF180 001DC0C0  4B E2 D8 31 */	bl __nwa__FUl
/* 801DF184 001DC0C4  3C 80 80 1E */	lis r4, __ct__16TBGCheckListWarpFv@ha
/* 801DF188 001DC0C8  38 84 F4 68 */	addi r4, r4, __ct__16TBGCheckListWarpFv@l
/* 801DF18C 001DC0CC  38 FD 00 00 */	addi r7, r29, 0
/* 801DF190 001DC0D0  38 A0 00 00 */	li r5, 0
/* 801DF194 001DC0D4  38 C0 00 14 */	li r6, 0x14
/* 801DF198 001DC0D8  4B EA 38 55 */	bl __construct_new_array
/* 801DF19C 001DC0DC  90 7F 00 30 */	stw r3, 0x30(r31)
/* 801DF1A0 001DC0E0  38 80 00 00 */	li r4, 0
/* 801DF1A4 001DC0E4  80 BF 00 10 */	lwz r5, 0x10(r31)
/* 801DF1A8 001DC0E8  80 DF 00 14 */	lwz r6, 0x14(r31)
/* 801DF1AC 001DC0EC  2C 05 00 00 */	cmpwi r5, 0
/* 801DF1B0 001DC0F0  38 65 00 00 */	addi r3, r5, 0
/* 801DF1B4 001DC0F4  41 82 00 98 */	beq lbl_801DF24C
/* 801DF1B8 001DC0F8  54 60 E8 FF */	rlwinm. r0, r3, 0x1d, 3, 0x1f
/* 801DF1BC 001DC0FC  7C 09 03 A6 */	mtctr r0
/* 801DF1C0 001DC100  41 82 00 74 */	beq lbl_801DF234
lbl_801DF1C4:
/* 801DF1C4 001DC104  90 86 00 04 */	stw r4, 4(r6)
/* 801DF1C8 001DC108  90 86 00 10 */	stw r4, 0x10(r6)
/* 801DF1CC 001DC10C  90 86 00 1C */	stw r4, 0x1c(r6)
/* 801DF1D0 001DC110  90 86 00 28 */	stw r4, 0x28(r6)
/* 801DF1D4 001DC114  90 86 00 34 */	stw r4, 0x34(r6)
/* 801DF1D8 001DC118  90 86 00 40 */	stw r4, 0x40(r6)
/* 801DF1DC 001DC11C  90 86 00 4C */	stw r4, 0x4c(r6)
/* 801DF1E0 001DC120  90 86 00 58 */	stw r4, 0x58(r6)
/* 801DF1E4 001DC124  90 86 00 64 */	stw r4, 0x64(r6)
/* 801DF1E8 001DC128  90 86 00 70 */	stw r4, 0x70(r6)
/* 801DF1EC 001DC12C  90 86 00 7C */	stw r4, 0x7c(r6)
/* 801DF1F0 001DC130  90 86 00 88 */	stw r4, 0x88(r6)
/* 801DF1F4 001DC134  90 86 00 94 */	stw r4, 0x94(r6)
/* 801DF1F8 001DC138  90 86 00 A0 */	stw r4, 0xa0(r6)
/* 801DF1FC 001DC13C  90 86 00 AC */	stw r4, 0xac(r6)
/* 801DF200 001DC140  90 86 00 B8 */	stw r4, 0xb8(r6)
/* 801DF204 001DC144  90 86 00 C4 */	stw r4, 0xc4(r6)
/* 801DF208 001DC148  90 86 00 D0 */	stw r4, 0xd0(r6)
/* 801DF20C 001DC14C  90 86 00 DC */	stw r4, 0xdc(r6)
/* 801DF210 001DC150  90 86 00 E8 */	stw r4, 0xe8(r6)
/* 801DF214 001DC154  90 86 00 F4 */	stw r4, 0xf4(r6)
/* 801DF218 001DC158  90 86 01 00 */	stw r4, 0x100(r6)
/* 801DF21C 001DC15C  90 86 01 0C */	stw r4, 0x10c(r6)
/* 801DF220 001DC160  90 86 01 18 */	stw r4, 0x118(r6)
/* 801DF224 001DC164  38 C6 01 20 */	addi r6, r6, 0x120
/* 801DF228 001DC168  42 00 FF 9C */	bdnz lbl_801DF1C4
/* 801DF22C 001DC16C  70 63 00 07 */	andi. r3, r3, 7
/* 801DF230 001DC170  41 82 00 1C */	beq lbl_801DF24C
lbl_801DF234:
/* 801DF234 001DC174  7C 69 03 A6 */	mtctr r3
lbl_801DF238:
/* 801DF238 001DC178  90 86 00 04 */	stw r4, 4(r6)
/* 801DF23C 001DC17C  90 86 00 10 */	stw r4, 0x10(r6)
/* 801DF240 001DC180  90 86 00 1C */	stw r4, 0x1c(r6)
/* 801DF244 001DC184  38 C6 00 24 */	addi r6, r6, 0x24
/* 801DF248 001DC188  42 00 FF F0 */	bdnz lbl_801DF238
lbl_801DF24C:
/* 801DF24C 001DC18C  3B C0 00 00 */	li r30, 0
/* 801DF250 001DC190  93 DF 00 34 */	stw r30, 0x34(r31)
/* 801DF254 001DC194  38 80 00 00 */	li r4, 0
/* 801DF258 001DC198  93 DF 00 38 */	stw r30, 0x38(r31)
/* 801DF25C 001DC19C  B3 DF 00 40 */	sth r30, 0x40(r31)
/* 801DF260 001DC1A0  80 7F 00 20 */	lwz r3, 0x20(r31)
/* 801DF264 001DC1A4  38 03 FF FF */	addi r0, r3, -1
/* 801DF268 001DC1A8  90 1F 00 3C */	stw r0, 0x3c(r31)
/* 801DF26C 001DC1AC  80 1F 00 10 */	lwz r0, 0x10(r31)
/* 801DF270 001DC1B0  80 7F 00 18 */	lwz r3, 0x18(r31)
/* 801DF274 001DC1B4  1C A0 00 24 */	mulli r5, r0, 0x24
/* 801DF278 001DC1B8  4B E2 3E 89 */	bl memset
/* 801DF27C 001DC1BC  38 00 00 04 */	li r0, 4
/* 801DF280 001DC1C0  7C 09 03 A6 */	mtctr r0
/* 801DF284 001DC1C4  38 00 27 0F */	li r0, 0x270f
lbl_801DF288:
/* 801DF288 001DC1C8  7C 7F F2 14 */	add r3, r31, r30
/* 801DF28C 001DC1CC  B0 03 00 42 */	sth r0, 0x42(r3)
/* 801DF290 001DC1D0  3B DE 00 10 */	addi r30, r30, 0x10
/* 801DF294 001DC1D4  B0 03 00 44 */	sth r0, 0x44(r3)
/* 801DF298 001DC1D8  B0 03 00 46 */	sth r0, 0x46(r3)
/* 801DF29C 001DC1DC  B0 03 00 48 */	sth r0, 0x48(r3)
/* 801DF2A0 001DC1E0  B0 03 00 4A */	sth r0, 0x4a(r3)
/* 801DF2A4 001DC1E4  B0 03 00 4C */	sth r0, 0x4c(r3)
/* 801DF2A8 001DC1E8  B0 03 00 4E */	sth r0, 0x4e(r3)
/* 801DF2AC 001DC1EC  B0 03 00 50 */	sth r0, 0x50(r3)
/* 801DF2B0 001DC1F0  7C 7F F2 14 */	add r3, r31, r30
/* 801DF2B4 001DC1F4  3B DE 00 10 */	addi r30, r30, 0x10
/* 801DF2B8 001DC1F8  B0 03 00 42 */	sth r0, 0x42(r3)
/* 801DF2BC 001DC1FC  B0 03 00 44 */	sth r0, 0x44(r3)
/* 801DF2C0 001DC200  B0 03 00 46 */	sth r0, 0x46(r3)
/* 801DF2C4 001DC204  B0 03 00 48 */	sth r0, 0x48(r3)
/* 801DF2C8 001DC208  B0 03 00 4A */	sth r0, 0x4a(r3)
/* 801DF2CC 001DC20C  B0 03 00 4C */	sth r0, 0x4c(r3)
/* 801DF2D0 001DC210  B0 03 00 4E */	sth r0, 0x4e(r3)
/* 801DF2D4 001DC214  B0 03 00 50 */	sth r0, 0x50(r3)
/* 801DF2D8 001DC218  7C 7F F2 14 */	add r3, r31, r30
/* 801DF2DC 001DC21C  3B DE 00 10 */	addi r30, r30, 0x10
/* 801DF2E0 001DC220  B0 03 00 42 */	sth r0, 0x42(r3)
/* 801DF2E4 001DC224  B0 03 00 44 */	sth r0, 0x44(r3)
/* 801DF2E8 001DC228  B0 03 00 46 */	sth r0, 0x46(r3)
/* 801DF2EC 001DC22C  B0 03 00 48 */	sth r0, 0x48(r3)
/* 801DF2F0 001DC230  B0 03 00 4A */	sth r0, 0x4a(r3)
/* 801DF2F4 001DC234  B0 03 00 4C */	sth r0, 0x4c(r3)
/* 801DF2F8 001DC238  B0 03 00 4E */	sth r0, 0x4e(r3)
/* 801DF2FC 001DC23C  B0 03 00 50 */	sth r0, 0x50(r3)
/* 801DF300 001DC240  7C 7F F2 14 */	add r3, r31, r30
/* 801DF304 001DC244  3B DE 00 10 */	addi r30, r30, 0x10
/* 801DF308 001DC248  B0 03 00 42 */	sth r0, 0x42(r3)
/* 801DF30C 001DC24C  B0 03 00 44 */	sth r0, 0x44(r3)
/* 801DF310 001DC250  B0 03 00 46 */	sth r0, 0x46(r3)
/* 801DF314 001DC254  B0 03 00 48 */	sth r0, 0x48(r3)
/* 801DF318 001DC258  B0 03 00 4A */	sth r0, 0x4a(r3)
/* 801DF31C 001DC25C  B0 03 00 4C */	sth r0, 0x4c(r3)
/* 801DF320 001DC260  B0 03 00 4E */	sth r0, 0x4e(r3)
/* 801DF324 001DC264  B0 03 00 50 */	sth r0, 0x50(r3)
/* 801DF328 001DC268  7C 7F F2 14 */	add r3, r31, r30
/* 801DF32C 001DC26C  3B DE 00 10 */	addi r30, r30, 0x10
/* 801DF330 001DC270  B0 03 00 42 */	sth r0, 0x42(r3)
/* 801DF334 001DC274  B0 03 00 44 */	sth r0, 0x44(r3)
/* 801DF338 001DC278  B0 03 00 46 */	sth r0, 0x46(r3)
/* 801DF33C 001DC27C  B0 03 00 48 */	sth r0, 0x48(r3)
/* 801DF340 001DC280  B0 03 00 4A */	sth r0, 0x4a(r3)
/* 801DF344 001DC284  B0 03 00 4C */	sth r0, 0x4c(r3)
/* 801DF348 001DC288  B0 03 00 4E */	sth r0, 0x4e(r3)
/* 801DF34C 001DC28C  B0 03 00 50 */	sth r0, 0x50(r3)
/* 801DF350 001DC290  7C 7F F2 14 */	add r3, r31, r30
/* 801DF354 001DC294  3B DE 00 10 */	addi r30, r30, 0x10
/* 801DF358 001DC298  B0 03 00 42 */	sth r0, 0x42(r3)
/* 801DF35C 001DC29C  B0 03 00 44 */	sth r0, 0x44(r3)
/* 801DF360 001DC2A0  B0 03 00 46 */	sth r0, 0x46(r3)
/* 801DF364 001DC2A4  B0 03 00 48 */	sth r0, 0x48(r3)
/* 801DF368 001DC2A8  B0 03 00 4A */	sth r0, 0x4a(r3)
/* 801DF36C 001DC2AC  B0 03 00 4C */	sth r0, 0x4c(r3)
/* 801DF370 001DC2B0  B0 03 00 4E */	sth r0, 0x4e(r3)
/* 801DF374 001DC2B4  B0 03 00 50 */	sth r0, 0x50(r3)
/* 801DF378 001DC2B8  7C 7F F2 14 */	add r3, r31, r30
/* 801DF37C 001DC2BC  3B DE 00 10 */	addi r30, r30, 0x10
/* 801DF380 001DC2C0  B0 03 00 42 */	sth r0, 0x42(r3)
/* 801DF384 001DC2C4  B0 03 00 44 */	sth r0, 0x44(r3)
/* 801DF388 001DC2C8  B0 03 00 46 */	sth r0, 0x46(r3)
/* 801DF38C 001DC2CC  B0 03 00 48 */	sth r0, 0x48(r3)
/* 801DF390 001DC2D0  B0 03 00 4A */	sth r0, 0x4a(r3)
/* 801DF394 001DC2D4  B0 03 00 4C */	sth r0, 0x4c(r3)
/* 801DF398 001DC2D8  B0 03 00 4E */	sth r0, 0x4e(r3)
/* 801DF39C 001DC2DC  B0 03 00 50 */	sth r0, 0x50(r3)
/* 801DF3A0 001DC2E0  7C 7F F2 14 */	add r3, r31, r30
/* 801DF3A4 001DC2E4  3B DE 00 10 */	addi r30, r30, 0x10
/* 801DF3A8 001DC2E8  B0 03 00 42 */	sth r0, 0x42(r3)
/* 801DF3AC 001DC2EC  B0 03 00 44 */	sth r0, 0x44(r3)
/* 801DF3B0 001DC2F0  B0 03 00 46 */	sth r0, 0x46(r3)
/* 801DF3B4 001DC2F4  B0 03 00 48 */	sth r0, 0x48(r3)
/* 801DF3B8 001DC2F8  B0 03 00 4A */	sth r0, 0x4a(r3)
/* 801DF3BC 001DC2FC  B0 03 00 4C */	sth r0, 0x4c(r3)
/* 801DF3C0 001DC300  B0 03 00 4E */	sth r0, 0x4e(r3)
/* 801DF3C4 001DC304  B0 03 00 50 */	sth r0, 0x50(r3)
/* 801DF3C8 001DC308  42 00 FE C0 */	bdnz lbl_801DF288
/* 801DF3CC 001DC30C  80 01 00 6C */	lwz r0, 0x6c(r1)
/* 801DF3D0 001DC310  83 E1 00 64 */	lwz r31, 0x64(r1)
/* 801DF3D4 001DC314  83 C1 00 60 */	lwz r30, 0x60(r1)
/* 801DF3D8 001DC318  7C 08 03 A6 */	mtlr r0
/* 801DF3DC 001DC31C  83 A1 00 5C */	lwz r29, 0x5c(r1)
/* 801DF3E0 001DC320  38 21 00 68 */	addi r1, r1, 0x68
/* 801DF3E4 001DC324  4E 80 00 20 */	blr 

.global __ct__16TBGCheckListRootFv
__ct__16TBGCheckListRootFv:
/* 801DF3E8 001DC328  7C 08 02 A6 */	mflr r0
/* 801DF3EC 001DC32C  3C 80 80 1E */	lis r4, __ct__12TBGCheckListFv@ha
/* 801DF3F0 001DC330  90 01 00 04 */	stw r0, 4(r1)
/* 801DF3F4 001DC334  38 84 F4 9C */	addi r4, r4, __ct__12TBGCheckListFv@l
/* 801DF3F8 001DC338  38 A0 00 00 */	li r5, 0
/* 801DF3FC 001DC33C  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 801DF400 001DC340  38 C0 00 0C */	li r6, 0xc
/* 801DF404 001DC344  38 E0 00 03 */	li r7, 3
/* 801DF408 001DC348  93 E1 00 14 */	stw r31, 0x14(r1)
/* 801DF40C 001DC34C  3B E3 00 00 */	addi r31, r3, 0
/* 801DF410 001DC350  4B EA 34 25 */	bl __construct_array
/* 801DF414 001DC354  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 801DF418 001DC358  7F E3 FB 78 */	mr r3, r31
/* 801DF41C 001DC35C  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 801DF420 001DC360  38 21 00 18 */	addi r1, r1, 0x18
/* 801DF424 001DC364  7C 08 03 A6 */	mtlr r0
/* 801DF428 001DC368  4E 80 00 20 */	blr 

.global initMoveCollision__17TMapCollisionDataFv
initMoveCollision__17TMapCollisionDataFv:
/* 801DF42C 001DC36C  7C 08 02 A6 */	mflr r0
/* 801DF430 001DC370  38 80 00 00 */	li r4, 0
/* 801DF434 001DC374  90 01 00 04 */	stw r0, 4(r1)
/* 801DF438 001DC378  94 21 FF F8 */	stwu r1, -8(r1)
/* 801DF43C 001DC37C  80 A3 00 20 */	lwz r5, 0x20(r3)
/* 801DF440 001DC380  38 05 FF FF */	addi r0, r5, -1
/* 801DF444 001DC384  90 03 00 3C */	stw r0, 0x3c(r3)
/* 801DF448 001DC388  80 03 00 10 */	lwz r0, 0x10(r3)
/* 801DF44C 001DC38C  80 63 00 18 */	lwz r3, 0x18(r3)
/* 801DF450 001DC390  1C A0 00 24 */	mulli r5, r0, 0x24
/* 801DF454 001DC394  4B E2 3C AD */	bl memset
/* 801DF458 001DC398  80 01 00 0C */	lwz r0, 0xc(r1)
/* 801DF45C 001DC39C  38 21 00 08 */	addi r1, r1, 8
/* 801DF460 001DC3A0  7C 08 03 A6 */	mtlr r0
/* 801DF464 001DC3A4  4E 80 00 20 */	blr 

.global __ct__16TBGCheckListWarpFv
__ct__16TBGCheckListWarpFv:
/* 801DF468 001DC3A8  3C 80 80 3D */	lis r4, __vt__12TBGCheckList@ha
/* 801DF46C 001DC3AC  38 04 FC 7C */	addi r0, r4, __vt__12TBGCheckList@l
/* 801DF470 001DC3B0  90 03 00 00 */	stw r0, 0(r3)
/* 801DF474 001DC3B4  38 A0 00 00 */	li r5, 0
/* 801DF478 001DC3B8  3C 80 80 3D */	lis r4, __vt__16TBGCheckListWarp@ha
/* 801DF47C 001DC3BC  90 A3 00 04 */	stw r5, 4(r3)
/* 801DF480 001DC3C0  38 04 FC 70 */	addi r0, r4, __vt__16TBGCheckListWarp@l
/* 801DF484 001DC3C4  90 A3 00 08 */	stw r5, 8(r3)
/* 801DF488 001DC3C8  90 03 00 00 */	stw r0, 0(r3)
/* 801DF48C 001DC3CC  90 A3 00 0C */	stw r5, 0xc(r3)
/* 801DF490 001DC3D0  B0 A3 00 10 */	sth r5, 0x10(r3)
/* 801DF494 001DC3D4  B0 A3 00 12 */	sth r5, 0x12(r3)
/* 801DF498 001DC3D8  4E 80 00 20 */	blr 

.global __ct__12TBGCheckListFv
__ct__12TBGCheckListFv:
/* 801DF49C 001DC3DC  3C 80 80 3D */	lis r4, __vt__12TBGCheckList@ha
/* 801DF4A0 001DC3E0  38 04 FC 7C */	addi r0, r4, __vt__12TBGCheckList@l
/* 801DF4A4 001DC3E4  90 03 00 00 */	stw r0, 0(r3)
/* 801DF4A8 001DC3E8  38 00 00 00 */	li r0, 0
/* 801DF4AC 001DC3EC  90 03 00 04 */	stw r0, 4(r3)
/* 801DF4B0 001DC3F0  90 03 00 08 */	stw r0, 8(r3)
/* 801DF4B4 001DC3F4  4E 80 00 20 */	blr 

.global setPreNode__16TBGCheckListWarpFP12TBGCheckList
setPreNode__16TBGCheckListWarpFP12TBGCheckList:
/* 801DF4B8 001DC3F8  90 83 00 0C */	stw r4, 0xc(r3)
/* 801DF4BC 001DC3FC  4E 80 00 20 */	blr 

.global setPreNode__12TBGCheckListFP12TBGCheckList
setPreNode__12TBGCheckListFP12TBGCheckList:
/* 801DF4C0 001DC400  4E 80 00 20 */	blr 

.global __sinit_MapCollisionData_cpp
__sinit_MapCollisionData_cpp:
/* 801DF4C4 001DC404  7C 08 02 A6 */	mflr r0
/* 801DF4C8 001DC408  3C 60 80 3F */	lis r3, mIllegalCheckData__17TMapCollisionData@ha
/* 801DF4CC 001DC40C  90 01 00 04 */	stw r0, 4(r1)
/* 801DF4D0 001DC410  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 801DF4D4 001DC414  93 E1 00 0C */	stw r31, 0xc(r1)
/* 801DF4D8 001DC418  3B E3 79 10 */	addi r31, r3, mIllegalCheckData__17TMapCollisionData@l
/* 801DF4DC 001DC41C  38 7F 00 00 */	addi r3, r31, 0
/* 801DF4E0 001DC420  48 01 56 E9 */	bl __ct__12TBGCheckDataFv
/* 801DF4E4 001DC424  88 0D 97 CC */	lbz r0, __init__smList__15JALList$$05MSBgm$$1@sda21(r13)
/* 801DF4E8 001DC428  7C 00 07 75 */	extsb. r0, r0
/* 801DF4EC 001DC42C  40 82 00 28 */	bne lbl_801DF514
/* 801DF4F0 001DC430  38 6D 97 A8 */	addi r3, r13, smList__15JALList$$05MSBgm$$1@sda21
/* 801DF4F4 001DC434  4B E2 F7 41 */	bl initiate__10JSUPtrListFv
/* 801DF4F8 001DC438  3C 60 80 0E */	lis r3, __dt__15JSUList$$05MSBgm$$1Fv@ha
/* 801DF4FC 001DC43C  38 83 6A 44 */	addi r4, r3, __dt__15JSUList$$05MSBgm$$1Fv@l
/* 801DF500 001DC440  38 6D 97 A8 */	addi r3, r13, smList__15JALList$$05MSBgm$$1@sda21
/* 801DF504 001DC444  38 BF 00 48 */	addi r5, r31, 0x48
/* 801DF508 001DC448  4B EA 32 21 */	bl __register_global_object
/* 801DF50C 001DC44C  38 00 00 01 */	li r0, 1
/* 801DF510 001DC450  98 0D 97 CC */	stb r0, __init__smList__15JALList$$05MSBgm$$1@sda21(r13)
lbl_801DF514:
/* 801DF514 001DC454  88 0D 97 CD */	lbz r0, __init__smList__24JALList$$013MSSetSoundGrp$$1@sda21(r13)
/* 801DF518 001DC458  7C 00 07 75 */	extsb. r0, r0
/* 801DF51C 001DC45C  40 82 00 28 */	bne lbl_801DF544
/* 801DF520 001DC460  38 6D 97 B4 */	addi r3, r13, smList__24JALList$$013MSSetSoundGrp$$1@sda21
/* 801DF524 001DC464  4B E2 F7 11 */	bl initiate__10JSUPtrListFv
/* 801DF528 001DC468  3C 60 80 0E */	lis r3, __dt__24JSUList$$013MSSetSoundGrp$$1Fv@ha
/* 801DF52C 001DC46C  38 83 69 EC */	addi r4, r3, __dt__24JSUList$$013MSSetSoundGrp$$1Fv@l
/* 801DF530 001DC470  38 6D 97 B4 */	addi r3, r13, smList__24JALList$$013MSSetSoundGrp$$1@sda21
/* 801DF534 001DC474  38 BF 00 54 */	addi r5, r31, 0x54
/* 801DF538 001DC478  4B EA 31 F1 */	bl __register_global_object
/* 801DF53C 001DC47C  38 00 00 01 */	li r0, 1
/* 801DF540 001DC480  98 0D 97 CD */	stb r0, __init__smList__24JALList$$013MSSetSoundGrp$$1@sda21(r13)
lbl_801DF544:
/* 801DF544 001DC484  88 0D 97 CE */	lbz r0, __init__smList__21JALList$$010MSSetSound$$1@sda21(r13)
/* 801DF548 001DC488  7C 00 07 75 */	extsb. r0, r0
/* 801DF54C 001DC48C  40 82 00 28 */	bne lbl_801DF574
/* 801DF550 001DC490  38 6D 97 C0 */	addi r3, r13, smList__21JALList$$010MSSetSound$$1@sda21
/* 801DF554 001DC494  4B E2 F6 E1 */	bl initiate__10JSUPtrListFv
/* 801DF558 001DC498  3C 60 80 0E */	lis r3, __dt__21JSUList$$010MSSetSound$$1Fv@ha
/* 801DF55C 001DC49C  38 83 69 94 */	addi r4, r3, __dt__21JSUList$$010MSSetSound$$1Fv@l
/* 801DF560 001DC4A0  38 6D 97 C0 */	addi r3, r13, smList__21JALList$$010MSSetSound$$1@sda21
/* 801DF564 001DC4A4  38 BF 00 60 */	addi r5, r31, 0x60
/* 801DF568 001DC4A8  4B EA 31 C1 */	bl __register_global_object
/* 801DF56C 001DC4AC  38 00 00 01 */	li r0, 1
/* 801DF570 001DC4B0  98 0D 97 CE */	stb r0, __init__smList__21JALList$$010MSSetSound$$1@sda21(r13)
lbl_801DF574:
/* 801DF574 001DC4B4  88 0D 8F 8C */	lbz r0, __init__smList__26JALList$$015JALSeModEffDGrp$$1@sda21(r13)
/* 801DF578 001DC4B8  7C 00 07 75 */	extsb. r0, r0
/* 801DF57C 001DC4BC  40 82 00 28 */	bne lbl_801DF5A4
/* 801DF580 001DC4C0  38 6D 8E FC */	addi r3, r13, smList__26JALList$$015JALSeModEffDGrp$$1@sda21
/* 801DF584 001DC4C4  4B E2 F6 B1 */	bl initiate__10JSUPtrListFv
/* 801DF588 001DC4C8  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffDGrp$$1Fv@ha
/* 801DF58C 001DC4CC  38 83 A8 0C */	addi r4, r3, __dt__26JSUList$$015JALSeModEffDGrp$$1Fv@l
/* 801DF590 001DC4D0  38 6D 8E FC */	addi r3, r13, smList__26JALList$$015JALSeModEffDGrp$$1@sda21
/* 801DF594 001DC4D4  38 BF 00 6C */	addi r5, r31, 0x6c
/* 801DF598 001DC4D8  4B EA 31 91 */	bl __register_global_object
/* 801DF59C 001DC4DC  38 00 00 01 */	li r0, 1
/* 801DF5A0 001DC4E0  98 0D 8F 8C */	stb r0, __init__smList__26JALList$$015JALSeModEffDGrp$$1@sda21(r13)
lbl_801DF5A4:
/* 801DF5A4 001DC4E4  88 0D 8F 8D */	lbz r0, __init__smList__26JALList$$015JALSeModPitDGrp$$1@sda21(r13)
/* 801DF5A8 001DC4E8  7C 00 07 75 */	extsb. r0, r0
/* 801DF5AC 001DC4EC  40 82 00 28 */	bne lbl_801DF5D4
/* 801DF5B0 001DC4F0  38 6D 8F 08 */	addi r3, r13, smList__26JALList$$015JALSeModPitDGrp$$1@sda21
/* 801DF5B4 001DC4F4  4B E2 F6 81 */	bl initiate__10JSUPtrListFv
/* 801DF5B8 001DC4F8  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitDGrp$$1Fv@ha
/* 801DF5BC 001DC4FC  38 83 A7 B4 */	addi r4, r3, __dt__26JSUList$$015JALSeModPitDGrp$$1Fv@l
/* 801DF5C0 001DC500  38 6D 8F 08 */	addi r3, r13, smList__26JALList$$015JALSeModPitDGrp$$1@sda21
/* 801DF5C4 001DC504  38 BF 00 78 */	addi r5, r31, 0x78
/* 801DF5C8 001DC508  4B EA 31 61 */	bl __register_global_object
/* 801DF5CC 001DC50C  38 00 00 01 */	li r0, 1
/* 801DF5D0 001DC510  98 0D 8F 8D */	stb r0, __init__smList__26JALList$$015JALSeModPitDGrp$$1@sda21(r13)
lbl_801DF5D4:
/* 801DF5D4 001DC514  88 0D 8F 8E */	lbz r0, __init__smList__26JALList$$015JALSeModVolDGrp$$1@sda21(r13)
/* 801DF5D8 001DC518  7C 00 07 75 */	extsb. r0, r0
/* 801DF5DC 001DC51C  40 82 00 28 */	bne lbl_801DF604
/* 801DF5E0 001DC520  38 6D 8F 14 */	addi r3, r13, smList__26JALList$$015JALSeModVolDGrp$$1@sda21
/* 801DF5E4 001DC524  4B E2 F6 51 */	bl initiate__10JSUPtrListFv
/* 801DF5E8 001DC528  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolDGrp$$1Fv@ha
/* 801DF5EC 001DC52C  38 83 A7 5C */	addi r4, r3, __dt__26JSUList$$015JALSeModVolDGrp$$1Fv@l
/* 801DF5F0 001DC530  38 6D 8F 14 */	addi r3, r13, smList__26JALList$$015JALSeModVolDGrp$$1@sda21
/* 801DF5F4 001DC534  38 BF 00 84 */	addi r5, r31, 0x84
/* 801DF5F8 001DC538  4B EA 31 31 */	bl __register_global_object
/* 801DF5FC 001DC53C  38 00 00 01 */	li r0, 1
/* 801DF600 001DC540  98 0D 8F 8E */	stb r0, __init__smList__26JALList$$015JALSeModVolDGrp$$1@sda21(r13)
lbl_801DF604:
/* 801DF604 001DC544  88 0D 8F 8F */	lbz r0, __init__smList__26JALList$$015JALSeModEffFGrp$$1@sda21(r13)
/* 801DF608 001DC548  7C 00 07 75 */	extsb. r0, r0
/* 801DF60C 001DC54C  40 82 00 28 */	bne lbl_801DF634
/* 801DF610 001DC550  38 6D 8F 20 */	addi r3, r13, smList__26JALList$$015JALSeModEffFGrp$$1@sda21
/* 801DF614 001DC554  4B E2 F6 21 */	bl initiate__10JSUPtrListFv
/* 801DF618 001DC558  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffFGrp$$1Fv@ha
/* 801DF61C 001DC55C  38 83 A7 04 */	addi r4, r3, __dt__26JSUList$$015JALSeModEffFGrp$$1Fv@l
/* 801DF620 001DC560  38 6D 8F 20 */	addi r3, r13, smList__26JALList$$015JALSeModEffFGrp$$1@sda21
/* 801DF624 001DC564  38 BF 00 90 */	addi r5, r31, 0x90
/* 801DF628 001DC568  4B EA 31 01 */	bl __register_global_object
/* 801DF62C 001DC56C  38 00 00 01 */	li r0, 1
/* 801DF630 001DC570  98 0D 8F 8F */	stb r0, __init__smList__26JALList$$015JALSeModEffFGrp$$1@sda21(r13)
lbl_801DF634:
/* 801DF634 001DC574  88 0D 8F 90 */	lbz r0, __init__smList__26JALList$$015JALSeModPitFGrp$$1@sda21(r13)
/* 801DF638 001DC578  7C 00 07 75 */	extsb. r0, r0
/* 801DF63C 001DC57C  40 82 00 28 */	bne lbl_801DF664
/* 801DF640 001DC580  38 6D 8F 2C */	addi r3, r13, smList__26JALList$$015JALSeModPitFGrp$$1@sda21
/* 801DF644 001DC584  4B E2 F5 F1 */	bl initiate__10JSUPtrListFv
/* 801DF648 001DC588  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitFGrp$$1Fv@ha
/* 801DF64C 001DC58C  38 83 A6 AC */	addi r4, r3, __dt__26JSUList$$015JALSeModPitFGrp$$1Fv@l
/* 801DF650 001DC590  38 6D 8F 2C */	addi r3, r13, smList__26JALList$$015JALSeModPitFGrp$$1@sda21
/* 801DF654 001DC594  38 BF 00 9C */	addi r5, r31, 0x9c
/* 801DF658 001DC598  4B EA 30 D1 */	bl __register_global_object
/* 801DF65C 001DC59C  38 00 00 01 */	li r0, 1
/* 801DF660 001DC5A0  98 0D 8F 90 */	stb r0, __init__smList__26JALList$$015JALSeModPitFGrp$$1@sda21(r13)
lbl_801DF664:
/* 801DF664 001DC5A4  88 0D 8F 91 */	lbz r0, __init__smList__26JALList$$015JALSeModVolFGrp$$1@sda21(r13)
/* 801DF668 001DC5A8  7C 00 07 75 */	extsb. r0, r0
/* 801DF66C 001DC5AC  40 82 00 28 */	bne lbl_801DF694
/* 801DF670 001DC5B0  38 6D 8F 38 */	addi r3, r13, smList__26JALList$$015JALSeModVolFGrp$$1@sda21
/* 801DF674 001DC5B4  4B E2 F5 C1 */	bl initiate__10JSUPtrListFv
/* 801DF678 001DC5B8  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolFGrp$$1Fv@ha
/* 801DF67C 001DC5BC  38 83 A6 54 */	addi r4, r3, __dt__26JSUList$$015JALSeModVolFGrp$$1Fv@l
/* 801DF680 001DC5C0  38 6D 8F 38 */	addi r3, r13, smList__26JALList$$015JALSeModVolFGrp$$1@sda21
/* 801DF684 001DC5C4  38 BF 00 A8 */	addi r5, r31, 0xa8
/* 801DF688 001DC5C8  4B EA 30 A1 */	bl __register_global_object
/* 801DF68C 001DC5CC  38 00 00 01 */	li r0, 1
/* 801DF690 001DC5D0  98 0D 8F 91 */	stb r0, __init__smList__26JALList$$015JALSeModVolFGrp$$1@sda21(r13)
lbl_801DF694:
/* 801DF694 001DC5D4  88 0D 8F 92 */	lbz r0, __init__smList__26JALList$$015JALSeModEffDist$$1@sda21(r13)
/* 801DF698 001DC5D8  7C 00 07 75 */	extsb. r0, r0
/* 801DF69C 001DC5DC  40 82 00 28 */	bne lbl_801DF6C4
/* 801DF6A0 001DC5E0  38 6D 8F 44 */	addi r3, r13, smList__26JALList$$015JALSeModEffDist$$1@sda21
/* 801DF6A4 001DC5E4  4B E2 F5 91 */	bl initiate__10JSUPtrListFv
/* 801DF6A8 001DC5E8  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffDist$$1Fv@ha
/* 801DF6AC 001DC5EC  38 83 A5 FC */	addi r4, r3, __dt__26JSUList$$015JALSeModEffDist$$1Fv@l
/* 801DF6B0 001DC5F0  38 6D 8F 44 */	addi r3, r13, smList__26JALList$$015JALSeModEffDist$$1@sda21
/* 801DF6B4 001DC5F4  38 BF 00 B4 */	addi r5, r31, 0xb4
/* 801DF6B8 001DC5F8  4B EA 30 71 */	bl __register_global_object
/* 801DF6BC 001DC5FC  38 00 00 01 */	li r0, 1
/* 801DF6C0 001DC600  98 0D 8F 92 */	stb r0, __init__smList__26JALList$$015JALSeModEffDist$$1@sda21(r13)
lbl_801DF6C4:
/* 801DF6C4 001DC604  88 0D 8F 93 */	lbz r0, __init__smList__26JALList$$015JALSeModPitDist$$1@sda21(r13)
/* 801DF6C8 001DC608  7C 00 07 75 */	extsb. r0, r0
/* 801DF6CC 001DC60C  40 82 00 28 */	bne lbl_801DF6F4
/* 801DF6D0 001DC610  38 6D 8F 50 */	addi r3, r13, smList__26JALList$$015JALSeModPitDist$$1@sda21
/* 801DF6D4 001DC614  4B E2 F5 61 */	bl initiate__10JSUPtrListFv
/* 801DF6D8 001DC618  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitDist$$1Fv@ha
/* 801DF6DC 001DC61C  38 83 A5 A4 */	addi r4, r3, __dt__26JSUList$$015JALSeModPitDist$$1Fv@l
/* 801DF6E0 001DC620  38 6D 8F 50 */	addi r3, r13, smList__26JALList$$015JALSeModPitDist$$1@sda21
/* 801DF6E4 001DC624  38 BF 00 C0 */	addi r5, r31, 0xc0
/* 801DF6E8 001DC628  4B EA 30 41 */	bl __register_global_object
/* 801DF6EC 001DC62C  38 00 00 01 */	li r0, 1
/* 801DF6F0 001DC630  98 0D 8F 93 */	stb r0, __init__smList__26JALList$$015JALSeModPitDist$$1@sda21(r13)
lbl_801DF6F4:
/* 801DF6F4 001DC634  88 0D 8F 94 */	lbz r0, __init__smList__26JALList$$015JALSeModVolDist$$1@sda21(r13)
/* 801DF6F8 001DC638  7C 00 07 75 */	extsb. r0, r0
/* 801DF6FC 001DC63C  40 82 00 28 */	bne lbl_801DF724
/* 801DF700 001DC640  38 6D 8F 5C */	addi r3, r13, smList__26JALList$$015JALSeModVolDist$$1@sda21
/* 801DF704 001DC644  4B E2 F5 31 */	bl initiate__10JSUPtrListFv
/* 801DF708 001DC648  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolDist$$1Fv@ha
/* 801DF70C 001DC64C  38 83 A5 4C */	addi r4, r3, __dt__26JSUList$$015JALSeModVolDist$$1Fv@l
/* 801DF710 001DC650  38 6D 8F 5C */	addi r3, r13, smList__26JALList$$015JALSeModVolDist$$1@sda21
/* 801DF714 001DC654  38 BF 00 CC */	addi r5, r31, 0xcc
/* 801DF718 001DC658  4B EA 30 11 */	bl __register_global_object
/* 801DF71C 001DC65C  38 00 00 01 */	li r0, 1
/* 801DF720 001DC660  98 0D 8F 94 */	stb r0, __init__smList__26JALList$$015JALSeModVolDist$$1@sda21(r13)
lbl_801DF724:
/* 801DF724 001DC664  88 0D 8F 95 */	lbz r0, __init__smList__26JALList$$015JALSeModEffFunk$$1@sda21(r13)
/* 801DF728 001DC668  7C 00 07 75 */	extsb. r0, r0
/* 801DF72C 001DC66C  40 82 00 28 */	bne lbl_801DF754
/* 801DF730 001DC670  38 6D 8F 68 */	addi r3, r13, smList__26JALList$$015JALSeModEffFunk$$1@sda21
/* 801DF734 001DC674  4B E2 F5 01 */	bl initiate__10JSUPtrListFv
/* 801DF738 001DC678  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffFunk$$1Fv@ha
/* 801DF73C 001DC67C  38 83 A4 F4 */	addi r4, r3, __dt__26JSUList$$015JALSeModEffFunk$$1Fv@l
/* 801DF740 001DC680  38 6D 8F 68 */	addi r3, r13, smList__26JALList$$015JALSeModEffFunk$$1@sda21
/* 801DF744 001DC684  38 BF 00 D8 */	addi r5, r31, 0xd8
/* 801DF748 001DC688  4B EA 2F E1 */	bl __register_global_object
/* 801DF74C 001DC68C  38 00 00 01 */	li r0, 1
/* 801DF750 001DC690  98 0D 8F 95 */	stb r0, __init__smList__26JALList$$015JALSeModEffFunk$$1@sda21(r13)
lbl_801DF754:
/* 801DF754 001DC694  88 0D 8F 96 */	lbz r0, __init__smList__26JALList$$015JALSeModPitFunk$$1@sda21(r13)
/* 801DF758 001DC698  7C 00 07 75 */	extsb. r0, r0
/* 801DF75C 001DC69C  40 82 00 28 */	bne lbl_801DF784
/* 801DF760 001DC6A0  38 6D 8F 74 */	addi r3, r13, smList__26JALList$$015JALSeModPitFunk$$1@sda21
/* 801DF764 001DC6A4  4B E2 F4 D1 */	bl initiate__10JSUPtrListFv
/* 801DF768 001DC6A8  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitFunk$$1Fv@ha
/* 801DF76C 001DC6AC  38 83 A4 9C */	addi r4, r3, __dt__26JSUList$$015JALSeModPitFunk$$1Fv@l
/* 801DF770 001DC6B0  38 6D 8F 74 */	addi r3, r13, smList__26JALList$$015JALSeModPitFunk$$1@sda21
/* 801DF774 001DC6B4  38 BF 00 E4 */	addi r5, r31, 0xe4
/* 801DF778 001DC6B8  4B EA 2F B1 */	bl __register_global_object
/* 801DF77C 001DC6BC  38 00 00 01 */	li r0, 1
/* 801DF780 001DC6C0  98 0D 8F 96 */	stb r0, __init__smList__26JALList$$015JALSeModPitFunk$$1@sda21(r13)
lbl_801DF784:
/* 801DF784 001DC6C4  88 0D 8F 97 */	lbz r0, __init__smList__26JALList$$015JALSeModVolFunk$$1@sda21(r13)
/* 801DF788 001DC6C8  7C 00 07 75 */	extsb. r0, r0
/* 801DF78C 001DC6CC  40 82 00 28 */	bne lbl_801DF7B4
/* 801DF790 001DC6D0  38 6D 8F 80 */	addi r3, r13, smList__26JALList$$015JALSeModVolFunk$$1@sda21
/* 801DF794 001DC6D4  4B E2 F4 A1 */	bl initiate__10JSUPtrListFv
/* 801DF798 001DC6D8  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolFunk$$1Fv@ha
/* 801DF79C 001DC6DC  38 83 A4 44 */	addi r4, r3, __dt__26JSUList$$015JALSeModVolFunk$$1Fv@l
/* 801DF7A0 001DC6E0  38 6D 8F 80 */	addi r3, r13, smList__26JALList$$015JALSeModVolFunk$$1@sda21
/* 801DF7A4 001DC6E4  38 BF 00 F0 */	addi r5, r31, 0xf0
/* 801DF7A8 001DC6E8  4B EA 2F 81 */	bl __register_global_object
/* 801DF7AC 001DC6EC  38 00 00 01 */	li r0, 1
/* 801DF7B0 001DC6F0  98 0D 8F 97 */	stb r0, __init__smList__26JALList$$015JALSeModVolFunk$$1@sda21(r13)
lbl_801DF7B4:
/* 801DF7B4 001DC6F4  80 01 00 14 */	lwz r0, 0x14(r1)
/* 801DF7B8 001DC6F8  83 E1 00 0C */	lwz r31, 0xc(r1)
/* 801DF7BC 001DC6FC  38 21 00 10 */	addi r1, r1, 0x10
/* 801DF7C0 001DC700  7C 08 03 A6 */	mtlr r0
/* 801DF7C4 001DC704  4E 80 00 20 */	blr 

.section .ctors, "wa"  # 0x8036FBA0 - 0x8036FF80
  .4byte __sinit_MapCollisionData_cpp

.section .data, "wa"  # 0x803A8380 - 0x803E6000
    .balign 8
.global __vt__16TBGCheckListWarp
__vt__16TBGCheckListWarp:
  .4byte 0
  .4byte 0
  .4byte setPreNode__16TBGCheckListWarpFP12TBGCheckList
.global __vt__12TBGCheckList
__vt__12TBGCheckList:
  .4byte 0
  .4byte 0
  .4byte setPreNode__12TBGCheckListFP12TBGCheckList

.section .sdata2, "a"  # 0x8040B460 - 0x80414020
.balign 8
$$22183:
  .4byte 0
$$22184:
	.incbin "baserom.dol", 0x3E8ACC, 0x4
$$22185:
	.incbin "baserom.dol", 0x3E8AD0, 0x4
$$22221:
	.incbin "baserom.dol", 0x3E8AD4, 0x4
$$22223:
	.incbin "baserom.dol", 0x3E8AD8, 0x8

.section .bss, "wa"  # 0x803E6000 - 0x80408AC0
.global mIllegalCheckData__17TMapCollisionData
mIllegalCheckData__17TMapCollisionData:
	.skip 0x48
$$22159:
	.skip 0xC
$$22160:
	.skip 0xC
$$22161:
	.skip 0xC
$$22162:
	.skip 0xC
$$22163:
	.skip 0xC
$$22164:
	.skip 0xC
$$22165:
	.skip 0xC
$$22166:
	.skip 0xC
$$22167:
	.skip 0xC
$$22168:
	.skip 0xC
$$22169:
	.skip 0xC
$$22170:
	.skip 0xC
$$22171:
	.skip 0xC
$$22172:
	.skip 0xC
$$22173:
	.skip 0x10

.section .sbss, "wa"  # 0x804097C0 - 0x8040B45C
.global gpMapCollisionData
gpMapCollisionData:
	.skip 0x8
