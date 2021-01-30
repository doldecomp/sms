.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global __dt__11TGateShadowFv
__dt__11TGateShadowFv:
/* 801AEECC 001ABE0C  7C 08 02 A6 */	mflr r0
/* 801AEED0 001ABE10  90 01 00 04 */	stw r0, 4(r1)
/* 801AEED4 001ABE14  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 801AEED8 001ABE18  93 E1 00 14 */	stw r31, 0x14(r1)
/* 801AEEDC 001ABE1C  3B E4 00 00 */	addi r31, r4, 0
/* 801AEEE0 001ABE20  93 C1 00 10 */	stw r30, 0x10(r1)
/* 801AEEE4 001ABE24  7C 7E 1B 79 */	or. r30, r3, r3
/* 801AEEE8 001ABE28  41 82 00 3C */	beq lbl_801AEF24
/* 801AEEEC 001ABE2C  3C 60 80 3D */	lis r3, __vt__11TGateShadow@ha
/* 801AEEF0 001ABE30  38 03 8A A0 */	addi r0, r3, __vt__11TGateShadow@l
/* 801AEEF4 001ABE34  90 1E 00 00 */	stw r0, 0(r30)
/* 801AEEF8 001ABE38  41 82 00 1C */	beq lbl_801AEF14
/* 801AEEFC 001ABE3C  3C 60 80 3B */	lis r3, __vt__Q26JDrama8TViewObj@ha
/* 801AEF00 001ABE40  38 03 A0 C0 */	addi r0, r3, __vt__Q26JDrama8TViewObj@l
/* 801AEF04 001ABE44  90 1E 00 00 */	stw r0, 0(r30)
/* 801AEF08 001ABE48  38 7E 00 00 */	addi r3, r30, 0
/* 801AEF0C 001ABE4C  38 80 00 00 */	li r4, 0
/* 801AEF10 001ABE50  4B E9 58 ED */	bl __dt__Q26JDrama8TNameRefFv
lbl_801AEF14:
/* 801AEF14 001ABE54  7F E0 07 35 */	extsh. r0, r31
/* 801AEF18 001ABE58  40 81 00 0C */	ble lbl_801AEF24
/* 801AEF1C 001ABE5C  7F C3 F3 78 */	mr r3, r30
/* 801AEF20 001ABE60  4B E5 DB 91 */	bl __dl__FPv
lbl_801AEF24:
/* 801AEF24 001ABE64  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 801AEF28 001ABE68  7F C3 F3 78 */	mr r3, r30
/* 801AEF2C 001ABE6C  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 801AEF30 001ABE70  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 801AEF34 001ABE74  7C 08 03 A6 */	mtlr r0
/* 801AEF38 001ABE78  38 21 00 18 */	addi r1, r1, 0x18
/* 801AEF3C 001ABE7C  4E 80 00 20 */	blr 

.global perform__11TGateShadowFUlPQ26JDrama9TGraphics
perform__11TGateShadowFUlPQ26JDrama9TGraphics:
/* 801AEF40 001ABE80  4E 80 00 20 */	blr 

.global __sinit_MapObjSample_cpp
__sinit_MapObjSample_cpp:
/* 801AEF44 001ABE84  7C 08 02 A6 */	mflr r0
/* 801AEF48 001ABE88  3C 60 80 3F */	lis r3, $$22483@ha
/* 801AEF4C 001ABE8C  90 01 00 04 */	stw r0, 4(r1)
/* 801AEF50 001ABE90  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 801AEF54 001ABE94  93 E1 00 0C */	stw r31, 0xc(r1)
/* 801AEF58 001ABE98  3B E3 62 80 */	addi r31, r3, $$22483@l
/* 801AEF5C 001ABE9C  88 0D 97 CC */	lbz r0, __init__smList__15JALList$$05MSBgm$$1-_SDA_BASE_(r13)
/* 801AEF60 001ABEA0  7C 00 07 75 */	extsb. r0, r0
/* 801AEF64 001ABEA4  40 82 00 28 */	bne lbl_801AEF8C
/* 801AEF68 001ABEA8  38 6D 97 A8 */	addi r3, r13, smList__15JALList$$05MSBgm$$1-_SDA_BASE_
/* 801AEF6C 001ABEAC  4B E5 FC C9 */	bl initiate__10JSUPtrListFv
/* 801AEF70 001ABEB0  3C 60 80 0E */	lis r3, __dt__15JSUList$$05MSBgm$$1Fv@ha
/* 801AEF74 001ABEB4  38 83 6A 44 */	addi r4, r3, __dt__15JSUList$$05MSBgm$$1Fv@l
/* 801AEF78 001ABEB8  38 6D 97 A8 */	addi r3, r13, smList__15JALList$$05MSBgm$$1-_SDA_BASE_
/* 801AEF7C 001ABEBC  38 BF 00 00 */	addi r5, r31, 0
/* 801AEF80 001ABEC0  4B ED 37 A9 */	bl __register_global_object
/* 801AEF84 001ABEC4  38 00 00 01 */	li r0, 1
/* 801AEF88 001ABEC8  98 0D 97 CC */	stb r0, __init__smList__15JALList$$05MSBgm$$1-_SDA_BASE_(r13)
lbl_801AEF8C:
/* 801AEF8C 001ABECC  88 0D 97 CD */	lbz r0, __init__smList__24JALList$$013MSSetSoundGrp$$1-_SDA_BASE_(r13)
/* 801AEF90 001ABED0  7C 00 07 75 */	extsb. r0, r0
/* 801AEF94 001ABED4  40 82 00 28 */	bne lbl_801AEFBC
/* 801AEF98 001ABED8  38 6D 97 B4 */	addi r3, r13, smList__24JALList$$013MSSetSoundGrp$$1-_SDA_BASE_
/* 801AEF9C 001ABEDC  4B E5 FC 99 */	bl initiate__10JSUPtrListFv
/* 801AEFA0 001ABEE0  3C 60 80 0E */	lis r3, __dt__24JSUList$$013MSSetSoundGrp$$1Fv@ha
/* 801AEFA4 001ABEE4  38 83 69 EC */	addi r4, r3, __dt__24JSUList$$013MSSetSoundGrp$$1Fv@l
/* 801AEFA8 001ABEE8  38 6D 97 B4 */	addi r3, r13, smList__24JALList$$013MSSetSoundGrp$$1-_SDA_BASE_
/* 801AEFAC 001ABEEC  38 BF 00 0C */	addi r5, r31, 0xc
/* 801AEFB0 001ABEF0  4B ED 37 79 */	bl __register_global_object
/* 801AEFB4 001ABEF4  38 00 00 01 */	li r0, 1
/* 801AEFB8 001ABEF8  98 0D 97 CD */	stb r0, __init__smList__24JALList$$013MSSetSoundGrp$$1-_SDA_BASE_(r13)
lbl_801AEFBC:
/* 801AEFBC 001ABEFC  88 0D 97 CE */	lbz r0, __init__smList__21JALList$$010MSSetSound$$1-_SDA_BASE_(r13)
/* 801AEFC0 001ABF00  7C 00 07 75 */	extsb. r0, r0
/* 801AEFC4 001ABF04  40 82 00 28 */	bne lbl_801AEFEC
/* 801AEFC8 001ABF08  38 6D 97 C0 */	addi r3, r13, smList__21JALList$$010MSSetSound$$1-_SDA_BASE_
/* 801AEFCC 001ABF0C  4B E5 FC 69 */	bl initiate__10JSUPtrListFv
/* 801AEFD0 001ABF10  3C 60 80 0E */	lis r3, __dt__21JSUList$$010MSSetSound$$1Fv@ha
/* 801AEFD4 001ABF14  38 83 69 94 */	addi r4, r3, __dt__21JSUList$$010MSSetSound$$1Fv@l
/* 801AEFD8 001ABF18  38 6D 97 C0 */	addi r3, r13, smList__21JALList$$010MSSetSound$$1-_SDA_BASE_
/* 801AEFDC 001ABF1C  38 BF 00 18 */	addi r5, r31, 0x18
/* 801AEFE0 001ABF20  4B ED 37 49 */	bl __register_global_object
/* 801AEFE4 001ABF24  38 00 00 01 */	li r0, 1
/* 801AEFE8 001ABF28  98 0D 97 CE */	stb r0, __init__smList__21JALList$$010MSSetSound$$1-_SDA_BASE_(r13)
lbl_801AEFEC:
/* 801AEFEC 001ABF2C  88 0D 8F 8C */	lbz r0, __init__smList__26JALList$$015JALSeModEffDGrp$$1-_SDA_BASE_(r13)
/* 801AEFF0 001ABF30  7C 00 07 75 */	extsb. r0, r0
/* 801AEFF4 001ABF34  40 82 00 28 */	bne lbl_801AF01C
/* 801AEFF8 001ABF38  38 6D 8E FC */	addi r3, r13, smList__26JALList$$015JALSeModEffDGrp$$1-_SDA_BASE_
/* 801AEFFC 001ABF3C  4B E5 FC 39 */	bl initiate__10JSUPtrListFv
/* 801AF000 001ABF40  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffDGrp$$1Fv@ha
/* 801AF004 001ABF44  38 83 A8 0C */	addi r4, r3, __dt__26JSUList$$015JALSeModEffDGrp$$1Fv@l
/* 801AF008 001ABF48  38 6D 8E FC */	addi r3, r13, smList__26JALList$$015JALSeModEffDGrp$$1-_SDA_BASE_
/* 801AF00C 001ABF4C  38 BF 00 24 */	addi r5, r31, 0x24
/* 801AF010 001ABF50  4B ED 37 19 */	bl __register_global_object
/* 801AF014 001ABF54  38 00 00 01 */	li r0, 1
/* 801AF018 001ABF58  98 0D 8F 8C */	stb r0, __init__smList__26JALList$$015JALSeModEffDGrp$$1-_SDA_BASE_(r13)
lbl_801AF01C:
/* 801AF01C 001ABF5C  88 0D 8F 8D */	lbz r0, __init__smList__26JALList$$015JALSeModPitDGrp$$1-_SDA_BASE_(r13)
/* 801AF020 001ABF60  7C 00 07 75 */	extsb. r0, r0
/* 801AF024 001ABF64  40 82 00 28 */	bne lbl_801AF04C
/* 801AF028 001ABF68  38 6D 8F 08 */	addi r3, r13, smList__26JALList$$015JALSeModPitDGrp$$1-_SDA_BASE_
/* 801AF02C 001ABF6C  4B E5 FC 09 */	bl initiate__10JSUPtrListFv
/* 801AF030 001ABF70  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitDGrp$$1Fv@ha
/* 801AF034 001ABF74  38 83 A7 B4 */	addi r4, r3, __dt__26JSUList$$015JALSeModPitDGrp$$1Fv@l
/* 801AF038 001ABF78  38 6D 8F 08 */	addi r3, r13, smList__26JALList$$015JALSeModPitDGrp$$1-_SDA_BASE_
/* 801AF03C 001ABF7C  38 BF 00 30 */	addi r5, r31, 0x30
/* 801AF040 001ABF80  4B ED 36 E9 */	bl __register_global_object
/* 801AF044 001ABF84  38 00 00 01 */	li r0, 1
/* 801AF048 001ABF88  98 0D 8F 8D */	stb r0, __init__smList__26JALList$$015JALSeModPitDGrp$$1-_SDA_BASE_(r13)
lbl_801AF04C:
/* 801AF04C 001ABF8C  88 0D 8F 8E */	lbz r0, __init__smList__26JALList$$015JALSeModVolDGrp$$1-_SDA_BASE_(r13)
/* 801AF050 001ABF90  7C 00 07 75 */	extsb. r0, r0
/* 801AF054 001ABF94  40 82 00 28 */	bne lbl_801AF07C
/* 801AF058 001ABF98  38 6D 8F 14 */	addi r3, r13, smList__26JALList$$015JALSeModVolDGrp$$1-_SDA_BASE_
/* 801AF05C 001ABF9C  4B E5 FB D9 */	bl initiate__10JSUPtrListFv
/* 801AF060 001ABFA0  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolDGrp$$1Fv@ha
/* 801AF064 001ABFA4  38 83 A7 5C */	addi r4, r3, __dt__26JSUList$$015JALSeModVolDGrp$$1Fv@l
/* 801AF068 001ABFA8  38 6D 8F 14 */	addi r3, r13, smList__26JALList$$015JALSeModVolDGrp$$1-_SDA_BASE_
/* 801AF06C 001ABFAC  38 BF 00 3C */	addi r5, r31, 0x3c
/* 801AF070 001ABFB0  4B ED 36 B9 */	bl __register_global_object
/* 801AF074 001ABFB4  38 00 00 01 */	li r0, 1
/* 801AF078 001ABFB8  98 0D 8F 8E */	stb r0, __init__smList__26JALList$$015JALSeModVolDGrp$$1-_SDA_BASE_(r13)
lbl_801AF07C:
/* 801AF07C 001ABFBC  88 0D 8F 8F */	lbz r0, __init__smList__26JALList$$015JALSeModEffFGrp$$1-_SDA_BASE_(r13)
/* 801AF080 001ABFC0  7C 00 07 75 */	extsb. r0, r0
/* 801AF084 001ABFC4  40 82 00 28 */	bne lbl_801AF0AC
/* 801AF088 001ABFC8  38 6D 8F 20 */	addi r3, r13, smList__26JALList$$015JALSeModEffFGrp$$1-_SDA_BASE_
/* 801AF08C 001ABFCC  4B E5 FB A9 */	bl initiate__10JSUPtrListFv
/* 801AF090 001ABFD0  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffFGrp$$1Fv@ha
/* 801AF094 001ABFD4  38 83 A7 04 */	addi r4, r3, __dt__26JSUList$$015JALSeModEffFGrp$$1Fv@l
/* 801AF098 001ABFD8  38 6D 8F 20 */	addi r3, r13, smList__26JALList$$015JALSeModEffFGrp$$1-_SDA_BASE_
/* 801AF09C 001ABFDC  38 BF 00 48 */	addi r5, r31, 0x48
/* 801AF0A0 001ABFE0  4B ED 36 89 */	bl __register_global_object
/* 801AF0A4 001ABFE4  38 00 00 01 */	li r0, 1
/* 801AF0A8 001ABFE8  98 0D 8F 8F */	stb r0, __init__smList__26JALList$$015JALSeModEffFGrp$$1-_SDA_BASE_(r13)
lbl_801AF0AC:
/* 801AF0AC 001ABFEC  88 0D 8F 90 */	lbz r0, __init__smList__26JALList$$015JALSeModPitFGrp$$1-_SDA_BASE_(r13)
/* 801AF0B0 001ABFF0  7C 00 07 75 */	extsb. r0, r0
/* 801AF0B4 001ABFF4  40 82 00 28 */	bne lbl_801AF0DC
/* 801AF0B8 001ABFF8  38 6D 8F 2C */	addi r3, r13, smList__26JALList$$015JALSeModPitFGrp$$1-_SDA_BASE_
/* 801AF0BC 001ABFFC  4B E5 FB 79 */	bl initiate__10JSUPtrListFv
/* 801AF0C0 001AC000  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitFGrp$$1Fv@ha
/* 801AF0C4 001AC004  38 83 A6 AC */	addi r4, r3, __dt__26JSUList$$015JALSeModPitFGrp$$1Fv@l
/* 801AF0C8 001AC008  38 6D 8F 2C */	addi r3, r13, smList__26JALList$$015JALSeModPitFGrp$$1-_SDA_BASE_
/* 801AF0CC 001AC00C  38 BF 00 54 */	addi r5, r31, 0x54
/* 801AF0D0 001AC010  4B ED 36 59 */	bl __register_global_object
/* 801AF0D4 001AC014  38 00 00 01 */	li r0, 1
/* 801AF0D8 001AC018  98 0D 8F 90 */	stb r0, __init__smList__26JALList$$015JALSeModPitFGrp$$1-_SDA_BASE_(r13)
lbl_801AF0DC:
/* 801AF0DC 001AC01C  88 0D 8F 91 */	lbz r0, __init__smList__26JALList$$015JALSeModVolFGrp$$1-_SDA_BASE_(r13)
/* 801AF0E0 001AC020  7C 00 07 75 */	extsb. r0, r0
/* 801AF0E4 001AC024  40 82 00 28 */	bne lbl_801AF10C
/* 801AF0E8 001AC028  38 6D 8F 38 */	addi r3, r13, smList__26JALList$$015JALSeModVolFGrp$$1-_SDA_BASE_
/* 801AF0EC 001AC02C  4B E5 FB 49 */	bl initiate__10JSUPtrListFv
/* 801AF0F0 001AC030  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolFGrp$$1Fv@ha
/* 801AF0F4 001AC034  38 83 A6 54 */	addi r4, r3, __dt__26JSUList$$015JALSeModVolFGrp$$1Fv@l
/* 801AF0F8 001AC038  38 6D 8F 38 */	addi r3, r13, smList__26JALList$$015JALSeModVolFGrp$$1-_SDA_BASE_
/* 801AF0FC 001AC03C  38 BF 00 60 */	addi r5, r31, 0x60
/* 801AF100 001AC040  4B ED 36 29 */	bl __register_global_object
/* 801AF104 001AC044  38 00 00 01 */	li r0, 1
/* 801AF108 001AC048  98 0D 8F 91 */	stb r0, __init__smList__26JALList$$015JALSeModVolFGrp$$1-_SDA_BASE_(r13)
lbl_801AF10C:
/* 801AF10C 001AC04C  88 0D 8F 92 */	lbz r0, __init__smList__26JALList$$015JALSeModEffDist$$1-_SDA_BASE_(r13)
/* 801AF110 001AC050  7C 00 07 75 */	extsb. r0, r0
/* 801AF114 001AC054  40 82 00 28 */	bne lbl_801AF13C
/* 801AF118 001AC058  38 6D 8F 44 */	addi r3, r13, smList__26JALList$$015JALSeModEffDist$$1-_SDA_BASE_
/* 801AF11C 001AC05C  4B E5 FB 19 */	bl initiate__10JSUPtrListFv
/* 801AF120 001AC060  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffDist$$1Fv@ha
/* 801AF124 001AC064  38 83 A5 FC */	addi r4, r3, __dt__26JSUList$$015JALSeModEffDist$$1Fv@l
/* 801AF128 001AC068  38 6D 8F 44 */	addi r3, r13, smList__26JALList$$015JALSeModEffDist$$1-_SDA_BASE_
/* 801AF12C 001AC06C  38 BF 00 6C */	addi r5, r31, 0x6c
/* 801AF130 001AC070  4B ED 35 F9 */	bl __register_global_object
/* 801AF134 001AC074  38 00 00 01 */	li r0, 1
/* 801AF138 001AC078  98 0D 8F 92 */	stb r0, __init__smList__26JALList$$015JALSeModEffDist$$1-_SDA_BASE_(r13)
lbl_801AF13C:
/* 801AF13C 001AC07C  88 0D 8F 93 */	lbz r0, __init__smList__26JALList$$015JALSeModPitDist$$1-_SDA_BASE_(r13)
/* 801AF140 001AC080  7C 00 07 75 */	extsb. r0, r0
/* 801AF144 001AC084  40 82 00 28 */	bne lbl_801AF16C
/* 801AF148 001AC088  38 6D 8F 50 */	addi r3, r13, smList__26JALList$$015JALSeModPitDist$$1-_SDA_BASE_
/* 801AF14C 001AC08C  4B E5 FA E9 */	bl initiate__10JSUPtrListFv
/* 801AF150 001AC090  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitDist$$1Fv@ha
/* 801AF154 001AC094  38 83 A5 A4 */	addi r4, r3, __dt__26JSUList$$015JALSeModPitDist$$1Fv@l
/* 801AF158 001AC098  38 6D 8F 50 */	addi r3, r13, smList__26JALList$$015JALSeModPitDist$$1-_SDA_BASE_
/* 801AF15C 001AC09C  38 BF 00 78 */	addi r5, r31, 0x78
/* 801AF160 001AC0A0  4B ED 35 C9 */	bl __register_global_object
/* 801AF164 001AC0A4  38 00 00 01 */	li r0, 1
/* 801AF168 001AC0A8  98 0D 8F 93 */	stb r0, __init__smList__26JALList$$015JALSeModPitDist$$1-_SDA_BASE_(r13)
lbl_801AF16C:
/* 801AF16C 001AC0AC  88 0D 8F 94 */	lbz r0, __init__smList__26JALList$$015JALSeModVolDist$$1-_SDA_BASE_(r13)
/* 801AF170 001AC0B0  7C 00 07 75 */	extsb. r0, r0
/* 801AF174 001AC0B4  40 82 00 28 */	bne lbl_801AF19C
/* 801AF178 001AC0B8  38 6D 8F 5C */	addi r3, r13, smList__26JALList$$015JALSeModVolDist$$1-_SDA_BASE_
/* 801AF17C 001AC0BC  4B E5 FA B9 */	bl initiate__10JSUPtrListFv
/* 801AF180 001AC0C0  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolDist$$1Fv@ha
/* 801AF184 001AC0C4  38 83 A5 4C */	addi r4, r3, __dt__26JSUList$$015JALSeModVolDist$$1Fv@l
/* 801AF188 001AC0C8  38 6D 8F 5C */	addi r3, r13, smList__26JALList$$015JALSeModVolDist$$1-_SDA_BASE_
/* 801AF18C 001AC0CC  38 BF 00 84 */	addi r5, r31, 0x84
/* 801AF190 001AC0D0  4B ED 35 99 */	bl __register_global_object
/* 801AF194 001AC0D4  38 00 00 01 */	li r0, 1
/* 801AF198 001AC0D8  98 0D 8F 94 */	stb r0, __init__smList__26JALList$$015JALSeModVolDist$$1-_SDA_BASE_(r13)
lbl_801AF19C:
/* 801AF19C 001AC0DC  88 0D 8F 95 */	lbz r0, __init__smList__26JALList$$015JALSeModEffFunk$$1-_SDA_BASE_(r13)
/* 801AF1A0 001AC0E0  7C 00 07 75 */	extsb. r0, r0
/* 801AF1A4 001AC0E4  40 82 00 28 */	bne lbl_801AF1CC
/* 801AF1A8 001AC0E8  38 6D 8F 68 */	addi r3, r13, smList__26JALList$$015JALSeModEffFunk$$1-_SDA_BASE_
/* 801AF1AC 001AC0EC  4B E5 FA 89 */	bl initiate__10JSUPtrListFv
/* 801AF1B0 001AC0F0  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffFunk$$1Fv@ha
/* 801AF1B4 001AC0F4  38 83 A4 F4 */	addi r4, r3, __dt__26JSUList$$015JALSeModEffFunk$$1Fv@l
/* 801AF1B8 001AC0F8  38 6D 8F 68 */	addi r3, r13, smList__26JALList$$015JALSeModEffFunk$$1-_SDA_BASE_
/* 801AF1BC 001AC0FC  38 BF 00 90 */	addi r5, r31, 0x90
/* 801AF1C0 001AC100  4B ED 35 69 */	bl __register_global_object
/* 801AF1C4 001AC104  38 00 00 01 */	li r0, 1
/* 801AF1C8 001AC108  98 0D 8F 95 */	stb r0, __init__smList__26JALList$$015JALSeModEffFunk$$1-_SDA_BASE_(r13)
lbl_801AF1CC:
/* 801AF1CC 001AC10C  88 0D 8F 96 */	lbz r0, __init__smList__26JALList$$015JALSeModPitFunk$$1-_SDA_BASE_(r13)
/* 801AF1D0 001AC110  7C 00 07 75 */	extsb. r0, r0
/* 801AF1D4 001AC114  40 82 00 28 */	bne lbl_801AF1FC
/* 801AF1D8 001AC118  38 6D 8F 74 */	addi r3, r13, smList__26JALList$$015JALSeModPitFunk$$1-_SDA_BASE_
/* 801AF1DC 001AC11C  4B E5 FA 59 */	bl initiate__10JSUPtrListFv
/* 801AF1E0 001AC120  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitFunk$$1Fv@ha
/* 801AF1E4 001AC124  38 83 A4 9C */	addi r4, r3, __dt__26JSUList$$015JALSeModPitFunk$$1Fv@l
/* 801AF1E8 001AC128  38 6D 8F 74 */	addi r3, r13, smList__26JALList$$015JALSeModPitFunk$$1-_SDA_BASE_
/* 801AF1EC 001AC12C  38 BF 00 9C */	addi r5, r31, 0x9c
/* 801AF1F0 001AC130  4B ED 35 39 */	bl __register_global_object
/* 801AF1F4 001AC134  38 00 00 01 */	li r0, 1
/* 801AF1F8 001AC138  98 0D 8F 96 */	stb r0, __init__smList__26JALList$$015JALSeModPitFunk$$1-_SDA_BASE_(r13)
lbl_801AF1FC:
/* 801AF1FC 001AC13C  88 0D 8F 97 */	lbz r0, __init__smList__26JALList$$015JALSeModVolFunk$$1-_SDA_BASE_(r13)
/* 801AF200 001AC140  7C 00 07 75 */	extsb. r0, r0
/* 801AF204 001AC144  40 82 00 28 */	bne lbl_801AF22C
/* 801AF208 001AC148  38 6D 8F 80 */	addi r3, r13, smList__26JALList$$015JALSeModVolFunk$$1-_SDA_BASE_
/* 801AF20C 001AC14C  4B E5 FA 29 */	bl initiate__10JSUPtrListFv
/* 801AF210 001AC150  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolFunk$$1Fv@ha
/* 801AF214 001AC154  38 83 A4 44 */	addi r4, r3, __dt__26JSUList$$015JALSeModVolFunk$$1Fv@l
/* 801AF218 001AC158  38 6D 8F 80 */	addi r3, r13, smList__26JALList$$015JALSeModVolFunk$$1-_SDA_BASE_
/* 801AF21C 001AC15C  38 BF 00 A8 */	addi r5, r31, 0xa8
/* 801AF220 001AC160  4B ED 35 09 */	bl __register_global_object
/* 801AF224 001AC164  38 00 00 01 */	li r0, 1
/* 801AF228 001AC168  98 0D 8F 97 */	stb r0, __init__smList__26JALList$$015JALSeModVolFunk$$1-_SDA_BASE_(r13)
lbl_801AF22C:
/* 801AF22C 001AC16C  80 01 00 14 */	lwz r0, 0x14(r1)
/* 801AF230 001AC170  83 E1 00 0C */	lwz r31, 0xc(r1)
/* 801AF234 001AC174  38 21 00 10 */	addi r1, r1, 0x10
/* 801AF238 001AC178  7C 08 03 A6 */	mtlr r0
/* 801AF23C 001AC17C  4E 80 00 20 */	blr 

.section .ctors, "wa"  # 0x8036FBA0 - 0x8036FF80
	.incbin "baserom.dol", 0x36CD38, 0x4

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global __vt__11TGateShadow
__vt__11TGateShadow:
	.incbin "baserom.dol", 0x3C5AA0, 0x28

.section .bss, "wa"  # 0x803E6000 - 0x80408AC0
.global $$22483
$$22483:
	.skip 0xC
.global $$22484
$$22484:
	.skip 0xC
.global $$22485
$$22485:
	.skip 0xC
.global $$22486
$$22486:
	.skip 0xC
.global $$22487
$$22487:
	.skip 0xC
.global $$22488
$$22488:
	.skip 0xC
.global $$22489
$$22489:
	.skip 0xC
.global $$22490
$$22490:
	.skip 0xC
.global $$22491
$$22491:
	.skip 0xC
.global $$22492
$$22492:
	.skip 0xC
.global $$22493
$$22493:
	.skip 0xC
.global $$22494
$$22494:
	.skip 0xC
.global $$22495
$$22495:
	.skip 0xC
.global $$22496
$$22496:
	.skip 0xC
.global $$22497
$$22497:
	.skip 0x10
