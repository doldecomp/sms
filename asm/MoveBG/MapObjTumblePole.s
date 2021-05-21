.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global __ct__Q37JGadget26TList_pointer$$0P9THitActor$$18iteratorFQ37JGadget36TList$$0Pv$$4Q27JGadget14TAllocator$$0Pv$$1$$18iterator
__ct__Q37JGadget26TList_pointer$$0P9THitActor$$18iteratorFQ37JGadget36TList$$0Pv$$4Q27JGadget14TAllocator$$0Pv$$1$$18iterator:
/* 801CEA7C 001CB9BC  80 04 00 00 */	lwz r0, 0(r4)
/* 801CEA80 001CB9C0  90 03 00 00 */	stw r0, 0(r3)
/* 801CEA84 001CB9C4  4E 80 00 20 */	blr 

.global __sinit_MapObjTumblePole_cpp
__sinit_MapObjTumblePole_cpp:
/* 801CEA88 001CB9C8  7C 08 02 A6 */	mflr r0
/* 801CEA8C 001CB9CC  38 60 FF FF */	li r3, -1
/* 801CEA90 001CB9D0  90 01 00 04 */	stw r0, 4(r1)
/* 801CEA94 001CB9D4  3C 80 80 3F */	lis r4, $$22855@ha
/* 801CEA98 001CB9D8  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 801CEA9C 001CB9DC  93 E1 00 0C */	stw r31, 0xc(r1)
/* 801CEAA0 001CB9E0  3B E4 70 50 */	addi r31, r4, $$22855@l
/* 801CEAA4 001CB9E4  88 0D 97 CC */	lbz r0, __init__smList__15JALList$$05MSBgm$$1@sda21(r13)
/* 801CEAA8 001CB9E8  90 6D 9A 98 */	stw r3, mColorUpper__17TMapObjTumblePole@sda21(r13)
/* 801CEAAC 001CB9EC  7C 00 07 75 */	extsb. r0, r0
/* 801CEAB0 001CB9F0  90 6D 9A 9C */	stw r3, mColorLower__17TMapObjTumblePole@sda21(r13)
/* 801CEAB4 001CB9F4  40 82 00 28 */	bne lbl_801CEADC
/* 801CEAB8 001CB9F8  38 6D 97 A8 */	addi r3, r13, smList__15JALList$$05MSBgm$$1@sda21
/* 801CEABC 001CB9FC  4B E4 01 79 */	bl initiate__10JSUPtrListFv
/* 801CEAC0 001CBA00  3C 60 80 0E */	lis r3, __dt__15JSUList$$05MSBgm$$1Fv@ha
/* 801CEAC4 001CBA04  38 83 6A 44 */	addi r4, r3, __dt__15JSUList$$05MSBgm$$1Fv@l
/* 801CEAC8 001CBA08  38 6D 97 A8 */	addi r3, r13, smList__15JALList$$05MSBgm$$1@sda21
/* 801CEACC 001CBA0C  38 BF 00 00 */	addi r5, r31, 0
/* 801CEAD0 001CBA10  4B EB 3C 59 */	bl __register_global_object
/* 801CEAD4 001CBA14  38 00 00 01 */	li r0, 1
/* 801CEAD8 001CBA18  98 0D 97 CC */	stb r0, __init__smList__15JALList$$05MSBgm$$1@sda21(r13)
lbl_801CEADC:
/* 801CEADC 001CBA1C  88 0D 97 CD */	lbz r0, __init__smList__24JALList$$013MSSetSoundGrp$$1@sda21(r13)
/* 801CEAE0 001CBA20  7C 00 07 75 */	extsb. r0, r0
/* 801CEAE4 001CBA24  40 82 00 28 */	bne lbl_801CEB0C
/* 801CEAE8 001CBA28  38 6D 97 B4 */	addi r3, r13, smList__24JALList$$013MSSetSoundGrp$$1@sda21
/* 801CEAEC 001CBA2C  4B E4 01 49 */	bl initiate__10JSUPtrListFv
/* 801CEAF0 001CBA30  3C 60 80 0E */	lis r3, __dt__24JSUList$$013MSSetSoundGrp$$1Fv@ha
/* 801CEAF4 001CBA34  38 83 69 EC */	addi r4, r3, __dt__24JSUList$$013MSSetSoundGrp$$1Fv@l
/* 801CEAF8 001CBA38  38 6D 97 B4 */	addi r3, r13, smList__24JALList$$013MSSetSoundGrp$$1@sda21
/* 801CEAFC 001CBA3C  38 BF 00 0C */	addi r5, r31, 0xc
/* 801CEB00 001CBA40  4B EB 3C 29 */	bl __register_global_object
/* 801CEB04 001CBA44  38 00 00 01 */	li r0, 1
/* 801CEB08 001CBA48  98 0D 97 CD */	stb r0, __init__smList__24JALList$$013MSSetSoundGrp$$1@sda21(r13)
lbl_801CEB0C:
/* 801CEB0C 001CBA4C  88 0D 97 CE */	lbz r0, __init__smList__21JALList$$010MSSetSound$$1@sda21(r13)
/* 801CEB10 001CBA50  7C 00 07 75 */	extsb. r0, r0
/* 801CEB14 001CBA54  40 82 00 28 */	bne lbl_801CEB3C
/* 801CEB18 001CBA58  38 6D 97 C0 */	addi r3, r13, smList__21JALList$$010MSSetSound$$1@sda21
/* 801CEB1C 001CBA5C  4B E4 01 19 */	bl initiate__10JSUPtrListFv
/* 801CEB20 001CBA60  3C 60 80 0E */	lis r3, __dt__21JSUList$$010MSSetSound$$1Fv@ha
/* 801CEB24 001CBA64  38 83 69 94 */	addi r4, r3, __dt__21JSUList$$010MSSetSound$$1Fv@l
/* 801CEB28 001CBA68  38 6D 97 C0 */	addi r3, r13, smList__21JALList$$010MSSetSound$$1@sda21
/* 801CEB2C 001CBA6C  38 BF 00 18 */	addi r5, r31, 0x18
/* 801CEB30 001CBA70  4B EB 3B F9 */	bl __register_global_object
/* 801CEB34 001CBA74  38 00 00 01 */	li r0, 1
/* 801CEB38 001CBA78  98 0D 97 CE */	stb r0, __init__smList__21JALList$$010MSSetSound$$1@sda21(r13)
lbl_801CEB3C:
/* 801CEB3C 001CBA7C  88 0D 8F 8C */	lbz r0, __init__smList__26JALList$$015JALSeModEffDGrp$$1@sda21(r13)
/* 801CEB40 001CBA80  7C 00 07 75 */	extsb. r0, r0
/* 801CEB44 001CBA84  40 82 00 28 */	bne lbl_801CEB6C
/* 801CEB48 001CBA88  38 6D 8E FC */	addi r3, r13, smList__26JALList$$015JALSeModEffDGrp$$1@sda21
/* 801CEB4C 001CBA8C  4B E4 00 E9 */	bl initiate__10JSUPtrListFv
/* 801CEB50 001CBA90  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffDGrp$$1Fv@ha
/* 801CEB54 001CBA94  38 83 A8 0C */	addi r4, r3, __dt__26JSUList$$015JALSeModEffDGrp$$1Fv@l
/* 801CEB58 001CBA98  38 6D 8E FC */	addi r3, r13, smList__26JALList$$015JALSeModEffDGrp$$1@sda21
/* 801CEB5C 001CBA9C  38 BF 00 24 */	addi r5, r31, 0x24
/* 801CEB60 001CBAA0  4B EB 3B C9 */	bl __register_global_object
/* 801CEB64 001CBAA4  38 00 00 01 */	li r0, 1
/* 801CEB68 001CBAA8  98 0D 8F 8C */	stb r0, __init__smList__26JALList$$015JALSeModEffDGrp$$1@sda21(r13)
lbl_801CEB6C:
/* 801CEB6C 001CBAAC  88 0D 8F 8D */	lbz r0, __init__smList__26JALList$$015JALSeModPitDGrp$$1@sda21(r13)
/* 801CEB70 001CBAB0  7C 00 07 75 */	extsb. r0, r0
/* 801CEB74 001CBAB4  40 82 00 28 */	bne lbl_801CEB9C
/* 801CEB78 001CBAB8  38 6D 8F 08 */	addi r3, r13, smList__26JALList$$015JALSeModPitDGrp$$1@sda21
/* 801CEB7C 001CBABC  4B E4 00 B9 */	bl initiate__10JSUPtrListFv
/* 801CEB80 001CBAC0  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitDGrp$$1Fv@ha
/* 801CEB84 001CBAC4  38 83 A7 B4 */	addi r4, r3, __dt__26JSUList$$015JALSeModPitDGrp$$1Fv@l
/* 801CEB88 001CBAC8  38 6D 8F 08 */	addi r3, r13, smList__26JALList$$015JALSeModPitDGrp$$1@sda21
/* 801CEB8C 001CBACC  38 BF 00 30 */	addi r5, r31, 0x30
/* 801CEB90 001CBAD0  4B EB 3B 99 */	bl __register_global_object
/* 801CEB94 001CBAD4  38 00 00 01 */	li r0, 1
/* 801CEB98 001CBAD8  98 0D 8F 8D */	stb r0, __init__smList__26JALList$$015JALSeModPitDGrp$$1@sda21(r13)
lbl_801CEB9C:
/* 801CEB9C 001CBADC  88 0D 8F 8E */	lbz r0, __init__smList__26JALList$$015JALSeModVolDGrp$$1@sda21(r13)
/* 801CEBA0 001CBAE0  7C 00 07 75 */	extsb. r0, r0
/* 801CEBA4 001CBAE4  40 82 00 28 */	bne lbl_801CEBCC
/* 801CEBA8 001CBAE8  38 6D 8F 14 */	addi r3, r13, smList__26JALList$$015JALSeModVolDGrp$$1@sda21
/* 801CEBAC 001CBAEC  4B E4 00 89 */	bl initiate__10JSUPtrListFv
/* 801CEBB0 001CBAF0  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolDGrp$$1Fv@ha
/* 801CEBB4 001CBAF4  38 83 A7 5C */	addi r4, r3, __dt__26JSUList$$015JALSeModVolDGrp$$1Fv@l
/* 801CEBB8 001CBAF8  38 6D 8F 14 */	addi r3, r13, smList__26JALList$$015JALSeModVolDGrp$$1@sda21
/* 801CEBBC 001CBAFC  38 BF 00 3C */	addi r5, r31, 0x3c
/* 801CEBC0 001CBB00  4B EB 3B 69 */	bl __register_global_object
/* 801CEBC4 001CBB04  38 00 00 01 */	li r0, 1
/* 801CEBC8 001CBB08  98 0D 8F 8E */	stb r0, __init__smList__26JALList$$015JALSeModVolDGrp$$1@sda21(r13)
lbl_801CEBCC:
/* 801CEBCC 001CBB0C  88 0D 8F 8F */	lbz r0, __init__smList__26JALList$$015JALSeModEffFGrp$$1@sda21(r13)
/* 801CEBD0 001CBB10  7C 00 07 75 */	extsb. r0, r0
/* 801CEBD4 001CBB14  40 82 00 28 */	bne lbl_801CEBFC
/* 801CEBD8 001CBB18  38 6D 8F 20 */	addi r3, r13, smList__26JALList$$015JALSeModEffFGrp$$1@sda21
/* 801CEBDC 001CBB1C  4B E4 00 59 */	bl initiate__10JSUPtrListFv
/* 801CEBE0 001CBB20  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffFGrp$$1Fv@ha
/* 801CEBE4 001CBB24  38 83 A7 04 */	addi r4, r3, __dt__26JSUList$$015JALSeModEffFGrp$$1Fv@l
/* 801CEBE8 001CBB28  38 6D 8F 20 */	addi r3, r13, smList__26JALList$$015JALSeModEffFGrp$$1@sda21
/* 801CEBEC 001CBB2C  38 BF 00 48 */	addi r5, r31, 0x48
/* 801CEBF0 001CBB30  4B EB 3B 39 */	bl __register_global_object
/* 801CEBF4 001CBB34  38 00 00 01 */	li r0, 1
/* 801CEBF8 001CBB38  98 0D 8F 8F */	stb r0, __init__smList__26JALList$$015JALSeModEffFGrp$$1@sda21(r13)
lbl_801CEBFC:
/* 801CEBFC 001CBB3C  88 0D 8F 90 */	lbz r0, __init__smList__26JALList$$015JALSeModPitFGrp$$1@sda21(r13)
/* 801CEC00 001CBB40  7C 00 07 75 */	extsb. r0, r0
/* 801CEC04 001CBB44  40 82 00 28 */	bne lbl_801CEC2C
/* 801CEC08 001CBB48  38 6D 8F 2C */	addi r3, r13, smList__26JALList$$015JALSeModPitFGrp$$1@sda21
/* 801CEC0C 001CBB4C  4B E4 00 29 */	bl initiate__10JSUPtrListFv
/* 801CEC10 001CBB50  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitFGrp$$1Fv@ha
/* 801CEC14 001CBB54  38 83 A6 AC */	addi r4, r3, __dt__26JSUList$$015JALSeModPitFGrp$$1Fv@l
/* 801CEC18 001CBB58  38 6D 8F 2C */	addi r3, r13, smList__26JALList$$015JALSeModPitFGrp$$1@sda21
/* 801CEC1C 001CBB5C  38 BF 00 54 */	addi r5, r31, 0x54
/* 801CEC20 001CBB60  4B EB 3B 09 */	bl __register_global_object
/* 801CEC24 001CBB64  38 00 00 01 */	li r0, 1
/* 801CEC28 001CBB68  98 0D 8F 90 */	stb r0, __init__smList__26JALList$$015JALSeModPitFGrp$$1@sda21(r13)
lbl_801CEC2C:
/* 801CEC2C 001CBB6C  88 0D 8F 91 */	lbz r0, __init__smList__26JALList$$015JALSeModVolFGrp$$1@sda21(r13)
/* 801CEC30 001CBB70  7C 00 07 75 */	extsb. r0, r0
/* 801CEC34 001CBB74  40 82 00 28 */	bne lbl_801CEC5C
/* 801CEC38 001CBB78  38 6D 8F 38 */	addi r3, r13, smList__26JALList$$015JALSeModVolFGrp$$1@sda21
/* 801CEC3C 001CBB7C  4B E3 FF F9 */	bl initiate__10JSUPtrListFv
/* 801CEC40 001CBB80  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolFGrp$$1Fv@ha
/* 801CEC44 001CBB84  38 83 A6 54 */	addi r4, r3, __dt__26JSUList$$015JALSeModVolFGrp$$1Fv@l
/* 801CEC48 001CBB88  38 6D 8F 38 */	addi r3, r13, smList__26JALList$$015JALSeModVolFGrp$$1@sda21
/* 801CEC4C 001CBB8C  38 BF 00 60 */	addi r5, r31, 0x60
/* 801CEC50 001CBB90  4B EB 3A D9 */	bl __register_global_object
/* 801CEC54 001CBB94  38 00 00 01 */	li r0, 1
/* 801CEC58 001CBB98  98 0D 8F 91 */	stb r0, __init__smList__26JALList$$015JALSeModVolFGrp$$1@sda21(r13)
lbl_801CEC5C:
/* 801CEC5C 001CBB9C  88 0D 8F 92 */	lbz r0, __init__smList__26JALList$$015JALSeModEffDist$$1@sda21(r13)
/* 801CEC60 001CBBA0  7C 00 07 75 */	extsb. r0, r0
/* 801CEC64 001CBBA4  40 82 00 28 */	bne lbl_801CEC8C
/* 801CEC68 001CBBA8  38 6D 8F 44 */	addi r3, r13, smList__26JALList$$015JALSeModEffDist$$1@sda21
/* 801CEC6C 001CBBAC  4B E3 FF C9 */	bl initiate__10JSUPtrListFv
/* 801CEC70 001CBBB0  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffDist$$1Fv@ha
/* 801CEC74 001CBBB4  38 83 A5 FC */	addi r4, r3, __dt__26JSUList$$015JALSeModEffDist$$1Fv@l
/* 801CEC78 001CBBB8  38 6D 8F 44 */	addi r3, r13, smList__26JALList$$015JALSeModEffDist$$1@sda21
/* 801CEC7C 001CBBBC  38 BF 00 6C */	addi r5, r31, 0x6c
/* 801CEC80 001CBBC0  4B EB 3A A9 */	bl __register_global_object
/* 801CEC84 001CBBC4  38 00 00 01 */	li r0, 1
/* 801CEC88 001CBBC8  98 0D 8F 92 */	stb r0, __init__smList__26JALList$$015JALSeModEffDist$$1@sda21(r13)
lbl_801CEC8C:
/* 801CEC8C 001CBBCC  88 0D 8F 93 */	lbz r0, __init__smList__26JALList$$015JALSeModPitDist$$1@sda21(r13)
/* 801CEC90 001CBBD0  7C 00 07 75 */	extsb. r0, r0
/* 801CEC94 001CBBD4  40 82 00 28 */	bne lbl_801CECBC
/* 801CEC98 001CBBD8  38 6D 8F 50 */	addi r3, r13, smList__26JALList$$015JALSeModPitDist$$1@sda21
/* 801CEC9C 001CBBDC  4B E3 FF 99 */	bl initiate__10JSUPtrListFv
/* 801CECA0 001CBBE0  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitDist$$1Fv@ha
/* 801CECA4 001CBBE4  38 83 A5 A4 */	addi r4, r3, __dt__26JSUList$$015JALSeModPitDist$$1Fv@l
/* 801CECA8 001CBBE8  38 6D 8F 50 */	addi r3, r13, smList__26JALList$$015JALSeModPitDist$$1@sda21
/* 801CECAC 001CBBEC  38 BF 00 78 */	addi r5, r31, 0x78
/* 801CECB0 001CBBF0  4B EB 3A 79 */	bl __register_global_object
/* 801CECB4 001CBBF4  38 00 00 01 */	li r0, 1
/* 801CECB8 001CBBF8  98 0D 8F 93 */	stb r0, __init__smList__26JALList$$015JALSeModPitDist$$1@sda21(r13)
lbl_801CECBC:
/* 801CECBC 001CBBFC  88 0D 8F 94 */	lbz r0, __init__smList__26JALList$$015JALSeModVolDist$$1@sda21(r13)
/* 801CECC0 001CBC00  7C 00 07 75 */	extsb. r0, r0
/* 801CECC4 001CBC04  40 82 00 28 */	bne lbl_801CECEC
/* 801CECC8 001CBC08  38 6D 8F 5C */	addi r3, r13, smList__26JALList$$015JALSeModVolDist$$1@sda21
/* 801CECCC 001CBC0C  4B E3 FF 69 */	bl initiate__10JSUPtrListFv
/* 801CECD0 001CBC10  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolDist$$1Fv@ha
/* 801CECD4 001CBC14  38 83 A5 4C */	addi r4, r3, __dt__26JSUList$$015JALSeModVolDist$$1Fv@l
/* 801CECD8 001CBC18  38 6D 8F 5C */	addi r3, r13, smList__26JALList$$015JALSeModVolDist$$1@sda21
/* 801CECDC 001CBC1C  38 BF 00 84 */	addi r5, r31, 0x84
/* 801CECE0 001CBC20  4B EB 3A 49 */	bl __register_global_object
/* 801CECE4 001CBC24  38 00 00 01 */	li r0, 1
/* 801CECE8 001CBC28  98 0D 8F 94 */	stb r0, __init__smList__26JALList$$015JALSeModVolDist$$1@sda21(r13)
lbl_801CECEC:
/* 801CECEC 001CBC2C  88 0D 8F 95 */	lbz r0, __init__smList__26JALList$$015JALSeModEffFunk$$1@sda21(r13)
/* 801CECF0 001CBC30  7C 00 07 75 */	extsb. r0, r0
/* 801CECF4 001CBC34  40 82 00 28 */	bne lbl_801CED1C
/* 801CECF8 001CBC38  38 6D 8F 68 */	addi r3, r13, smList__26JALList$$015JALSeModEffFunk$$1@sda21
/* 801CECFC 001CBC3C  4B E3 FF 39 */	bl initiate__10JSUPtrListFv
/* 801CED00 001CBC40  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffFunk$$1Fv@ha
/* 801CED04 001CBC44  38 83 A4 F4 */	addi r4, r3, __dt__26JSUList$$015JALSeModEffFunk$$1Fv@l
/* 801CED08 001CBC48  38 6D 8F 68 */	addi r3, r13, smList__26JALList$$015JALSeModEffFunk$$1@sda21
/* 801CED0C 001CBC4C  38 BF 00 90 */	addi r5, r31, 0x90
/* 801CED10 001CBC50  4B EB 3A 19 */	bl __register_global_object
/* 801CED14 001CBC54  38 00 00 01 */	li r0, 1
/* 801CED18 001CBC58  98 0D 8F 95 */	stb r0, __init__smList__26JALList$$015JALSeModEffFunk$$1@sda21(r13)
lbl_801CED1C:
/* 801CED1C 001CBC5C  88 0D 8F 96 */	lbz r0, __init__smList__26JALList$$015JALSeModPitFunk$$1@sda21(r13)
/* 801CED20 001CBC60  7C 00 07 75 */	extsb. r0, r0
/* 801CED24 001CBC64  40 82 00 28 */	bne lbl_801CED4C
/* 801CED28 001CBC68  38 6D 8F 74 */	addi r3, r13, smList__26JALList$$015JALSeModPitFunk$$1@sda21
/* 801CED2C 001CBC6C  4B E3 FF 09 */	bl initiate__10JSUPtrListFv
/* 801CED30 001CBC70  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitFunk$$1Fv@ha
/* 801CED34 001CBC74  38 83 A4 9C */	addi r4, r3, __dt__26JSUList$$015JALSeModPitFunk$$1Fv@l
/* 801CED38 001CBC78  38 6D 8F 74 */	addi r3, r13, smList__26JALList$$015JALSeModPitFunk$$1@sda21
/* 801CED3C 001CBC7C  38 BF 00 9C */	addi r5, r31, 0x9c
/* 801CED40 001CBC80  4B EB 39 E9 */	bl __register_global_object
/* 801CED44 001CBC84  38 00 00 01 */	li r0, 1
/* 801CED48 001CBC88  98 0D 8F 96 */	stb r0, __init__smList__26JALList$$015JALSeModPitFunk$$1@sda21(r13)
lbl_801CED4C:
/* 801CED4C 001CBC8C  88 0D 8F 97 */	lbz r0, __init__smList__26JALList$$015JALSeModVolFunk$$1@sda21(r13)
/* 801CED50 001CBC90  7C 00 07 75 */	extsb. r0, r0
/* 801CED54 001CBC94  40 82 00 28 */	bne lbl_801CED7C
/* 801CED58 001CBC98  38 6D 8F 80 */	addi r3, r13, smList__26JALList$$015JALSeModVolFunk$$1@sda21
/* 801CED5C 001CBC9C  4B E3 FE D9 */	bl initiate__10JSUPtrListFv
/* 801CED60 001CBCA0  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolFunk$$1Fv@ha
/* 801CED64 001CBCA4  38 83 A4 44 */	addi r4, r3, __dt__26JSUList$$015JALSeModVolFunk$$1Fv@l
/* 801CED68 001CBCA8  38 6D 8F 80 */	addi r3, r13, smList__26JALList$$015JALSeModVolFunk$$1@sda21
/* 801CED6C 001CBCAC  38 BF 00 A8 */	addi r5, r31, 0xa8
/* 801CED70 001CBCB0  4B EB 39 B9 */	bl __register_global_object
/* 801CED74 001CBCB4  38 00 00 01 */	li r0, 1
/* 801CED78 001CBCB8  98 0D 8F 97 */	stb r0, __init__smList__26JALList$$015JALSeModVolFunk$$1@sda21(r13)
lbl_801CED7C:
/* 801CED7C 001CBCBC  80 01 00 14 */	lwz r0, 0x14(r1)
/* 801CED80 001CBCC0  83 E1 00 0C */	lwz r31, 0xc(r1)
/* 801CED84 001CBCC4  38 21 00 10 */	addi r1, r1, 0x10
/* 801CED88 001CBCC8  7C 08 03 A6 */	mtlr r0
/* 801CED8C 001CBCCC  4E 80 00 20 */	blr 

.section .ctors, "wa"  # 0x8036FBA0 - 0x8036FF80
	.incbin "baserom.dol", 0x36CD84, 0x4

.section .bss, "wa"  # 0x803E6000 - 0x80408AC0
$$22855:
	.skip 0xC
$$22856:
	.skip 0xC
$$22857:
	.skip 0xC
$$22858:
	.skip 0xC
$$22859:
	.skip 0xC
$$22860:
	.skip 0xC
$$22861:
	.skip 0xC
$$22862:
	.skip 0xC
$$22863:
	.skip 0xC
$$22864:
	.skip 0xC
$$22865:
	.skip 0xC
$$22866:
	.skip 0xC
$$22867:
	.skip 0xC
$$22868:
	.skip 0xC
$$22869:
	.skip 0x10

.section .sbss, "wa"  # 0x804097C0 - 0x8040B45C
mColorUpper__17TMapObjTumblePole:
	.skip 0x4
mColorLower__17TMapObjTumblePole:
	.skip 0x4
